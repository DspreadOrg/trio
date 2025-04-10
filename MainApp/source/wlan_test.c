#include "app.h"
#include "ui.h"

#ifdef WNET_DEBUG
#define WNET_STRLOG DBG_STR
#define WNET_HEXLOG DBG_HEX
#else
#define WNET_STRLOG  
#define WNET_HEXLOG 
#endif


static int m_CommHandle;

int WlanGetSign(void)
{
    u8 sign_level, ret;
    u8 buf[16];

    ret = WlGetSignal(&sign_level);
	WNET_STRLOG("ret=%d,sig:%d", ret,sign_level);
    if(ret>=0) return sign_level;
	else return 5;
}

int s_WnetApiTest2()
{
    u8 txbuf[1024], rxbuf[1024], buf[64], tm_no;
    int ret, txoff, txlen,rxoff, rxlen, cnt, i,j,sta, socket_id=-1;
    u32 timeout=0, speed=0;
    ST_WLAN_CFG cfg;
    struct net_sockaddr addr;

    RouteSetDefault(WNET_ROUTE);

    memset((u8*)&cfg, 0x00, sizeof(ST_WLAN_CFG));
    cfg.SimCardSel = 0;//SIM0;
    strcpy(cfg.SimPin, "card");
    strcpy(cfg.AccessPointName, "CMNET");   
    
    WlInit(cfg);

    Ui_ClearLine(1, 9);
    Ui_ClearKey();

    tm_no = 2;
    sta=0;
    timeout = 30000;
    TimerSet(tm_no, timeout/100);
    while(1){
        Ui_ClearLine(1, 1);
        Ui_Print(1,0,"sta:%08x, Get SIG...", sta);
        if(WlanGetSign()!=5){
            break;
        }
        if(Ui_WaitKey(500)==KB_CANCEL) {
            return -1;
        }
    }

    cnt=0;
PPP_LOG:
    Ui_ClearLine(1, 1);
    memset(buf, 0x00, sizeof(buf));
	WlGetSignal(buf);
    Ui_Print(1,0,"sig:%d, [%d]", buf[0], timeout-TimerCheck(tm_no));

    socket_id = NetSocket(NET_AF_INET, NET_SOCK_STREAM, 0);
    Ui_ClearLine(2, 2);Ui_Print(2,0,"NetSocket:%d", socket_id);
    sta = 0;
    timeout = 30000;
    TimerSet(tm_no, timeout/100);
    while(1){
        sta = Netioctl(socket_id, CMD_TO_SET, 0);
        Ui_ClearLine(2, 2);Ui_Print(2,0,"sta:%2x, %d", sta&0xff, TimerCheck(tm_no));
        if(sta & SOCK_EVENT_ACCEPT) {
            Ui_ClearLine(2, 2);
            Ui_Print(2,0,"PPP Connect success,[%d]ms", timeout-TimerCheck(tm_no));
            break;
        }
        if(0==TimerCheck(tm_no)) {
            Ui_WaitKey(0xffffffff);ret = sta;goto EXIT;//return sta;
        }
        if(Ui_GetKey()==KB_CANCEL) goto EXIT;
    }

TCP_CONNECT:
    Ui_ClearLine(3, 9);
	WNET_STRLOG("********cnt:%d, STA:%08x******\r\n", cnt, Netioctl(socket_id, CMD_TO_SET, 0));
    timeout = 30000;

    SockAddrSet(&addr, "47.98.106.63", 6666);
    ret = NetConnect(socket_id, &addr, sizeof(addr));
    sta = 0;
    TimerSet(tm_no, timeout/100);
    while(1){
        sta = Netioctl(socket_id, CMD_TO_SET, 0);
        Ui_ClearLine(3, 3);Ui_Print(3,0,"sta:%2x, %d", sta&0xff, TimerCheck(tm_no));
        if(sta & SOCK_EVENT_CONN) {
            Ui_ClearLine(3, 3);
            Ui_Print(3,0,"Connect success,[%d]ms", timeout-TimerCheck(tm_no));
            break;
        }
        if(0==TimerCheck(tm_no)) {
            WNET_STRLOG("sta:%2x, %d", sta&0xff, TimerCheck(tm_no));
            Ui_WaitKey(0xffffffff);ret = sta;goto EXIT;//return sta;
        }
        if(Ui_GetKey()==KB_CANCEL) goto EXIT;
    }

    Ui_ClearLine(4, 4);
    while(1){
        txoff = 0;
        txlen = (100+cnt*100)%1024;
        if(Ui_GetKey()==KB_CANCEL) goto EXIT;
        for(i=0;i<txlen; i++) txbuf[i] = 0x30+i%10;

        do{
            ret = NetSend(socket_id, txbuf+txoff, txlen-txoff, 0);
            if(ret<0){
                WNET_STRLOG("************tx fail: %d", ret);
                Ui_ClearLine(4, 4);Ui_Print(4,0, "tx fail: %d", ret);
                if (ret) {
                    Ui_WaitKey(0xffffffff);goto EXIT;//return ret;
                }            
            }
            txoff += ret;
            if (txoff == txlen) break;
        }while(1);

        timeout = 30000;
        TimerSet(tm_no, timeout/100);
        memset(rxbuf, 0x00, sizeof(rxbuf));
        rxoff = 0;
        do{
            if(Ui_GetKey()==KB_CANCEL) goto EXIT;
            Ui_ClearLine(4, 4);Ui_Print(4,0,"rx[%d]:%02x", cnt, WlanGetSign()&0xff);
            ret = NetRecv(socket_id, rxbuf+rxoff, txlen-rxoff, 0);
            if(ret<0){
                WNET_STRLOG("****************rx fail: %d", ret);
                if (ret) {
                    Ui_ClearLine(4, 4);Ui_Print(4,0,"rx ret:%d",ret);
                    Ui_WaitKey(0xffffffff);goto EXIT;//return ret;
                }            
            }
            rxoff += ret;
            if (rxoff == txlen) {
                speed = (txlen*1000)/(timeout - TimerCheck(tm_no));
                break;
            }
            if(0==TimerCheck(tm_no)) {
                WNET_STRLOG("********rx timeout[%02x],%d,%d", WlanGetSign()&0xff, rxoff, ret);

                Ui_ClearLine(1, 1);
                WlGetSignal(buf);
                Ui_Print(1,0,"sig:%d[%d]", buf[0], timeout-TimerCheck(tm_no));

                Ui_ClearLine(4, 4);Ui_Print(4,0, "rx err,%d,%d", rxoff, ret);

                Ui_WaitKey(0xffffffff);goto EXIT;//return ret;
            }
            //DEBUG_PRINT("rxoff:%d,ret:%d, :%d\r\n", rxoff, ret, cnt);
        }while(1);
        if (memcmp(rxbuf, txbuf, txlen)== 0) {
            WNET_STRLOG("[%d][%d],txlen:%d, rx==tx\r\n", j,cnt, txlen);
            Ui_ClearLine(5, 5);Ui_Print(5,0,"OK[%d][%d][%d B/s]", cnt, txlen, speed);
        }
        else {
            WNET_STRLOG("[%d][%d],txlen:%d, rx!=tx\r\n",j,cnt, txlen);
            Ui_ClearLine(5, 5);Ui_Print(5,0,"FAIL[%d][%d][%d]rx!=tx\r\n", j,cnt, txlen); 
            for(i=0; i<txlen; i++){
                if(rxbuf[i]==txbuf[i]) continue;
                DBG_HEX("rxbuf:", rxbuf+i, txlen-i);
                DBG_HEX("txbuf:", txbuf+i, txlen-i);
                break;
            }

            Ui_WaitKey(0xffffffff);
        } 

        if(Ui_GetKey()==KB_CANCEL) goto EXIT;
        else break;
    }//while(1)      
    NetCloseSocket(socket_id);
    cnt++;

    sta = 0;
    TimerSet(tm_no, 600);    
    while(1){
        sta = Netioctl(socket_id, CMD_TO_SET, 0);
        Ui_ClearLine(6, 6);Ui_Print(6,0,"CLOSE sta:%2x, %d", sta&0xff, TimerCheck(tm_no));
        if(!(sta & SOCK_EVENT_CONN)) {
            WNET_STRLOG("app close ok");
            Ui_ClearLine(6, 6);Ui_Print(6,0,"TCP CLOSE OK");
            break;
        }
        if(0==TimerCheck(tm_no)) {
            WNET_STRLOG("sta:%2x, %d", sta&0xff, TimerCheck(tm_no));
            Ui_WaitKey(0xffffffff);ret = sta;goto EXIT;//return sta;
        }
        if(Ui_GetKey()==KB_CANCEL) goto EXIT;
    }

    //WlPppLogout();
    DelayMs(1000);
    goto PPP_LOG;


EXIT:
    //sta = Netioctl(socket_id, CMD_EVENT_GET, 0);
    //if(sta&SOCK_EVENT_CONN){
        TimerSet(tm_no, 600);
        NetCloseSocket(socket_id);    
        while(1){
            sta = Netioctl(socket_id, CMD_TO_SET, 0);
            Ui_ClearLine(6, 6);Ui_Print(6,0,"CLOSE sta:%2x, %d", sta&0xff, TimerCheck(tm_no));
            if(!(sta & SOCK_EVENT_CONN)) {
                WNET_STRLOG("app close ok");
                Ui_ClearLine(6, 6);Ui_Print(6,0,"TCP CLOSE OK");
                break;
            }
            if(0==TimerCheck(tm_no)) {
                WNET_STRLOG("sta:%2x, %d", sta&0xff, TimerCheck(tm_no));
                Ui_WaitKey(0xffffffff);ret = sta;goto EXIT;//return sta;
            }
            if(Ui_GetKey()==KB_CANCEL) break;
        }
    //}

    //WlPppLogout();

    return ret;
}

int WlanGetInfoTest()
{
    u8 mnc[16], mcc[16], lac[16], cellId[16], rssi[16];
    u8 imei[32],imsi[32];

    Ui_ClearLine(1, 9);
    Ui_ClearKey();

    memset(rssi, 0x00, sizeof(rssi));
    memset(lac, 0x00, sizeof(lac));
    memset(cellId, 0x00, sizeof(cellId));
    memset(mnc, 0x00, sizeof(mnc));
    memset(mcc, 0x00, sizeof(mcc));
	WlGetSignal(rssi);
    GetTermInfoExt("IMEI", imei, 31);
    GetTermInfoExt("IMSI", imsi, 31);
    GetTermInfoExt("CELL_LAC", lac, 15);
    GetTermInfoExt("CELL_CI", cellId, 15);
    GetTermInfoExt("CELL_MNC", mnc, 15);
    GetTermInfoExt("CELL_MCC", mcc, 15);

    Ui_Print(1,0,"RSSI:%d", rssi[0]);
    Ui_Print(2,0,"IMEI:%s", imei);
    Ui_Print(3,0,"IMSI:%s", imsi);
    Ui_Print(4,0,"LAC:%s, CI:%s", lac, cellId);
    Ui_Print(5,0,"MNC:%s, MCC:%s", mnc, mcc);

    Ui_WaitKey(0xffffffff);
}


int setcomm_wifi(void *pxPARAM)
{
    int ret;
    ST_WIFIAP Ap;
    ST_WIFI_PARAM WifiParam;

    Ui_ClearLine(1, 9);
    Ui_ClearKey();	
	Ui_DispTextLineAlign(2, DISPLAY_CENTER, "Searching WiFi", 0);
    Ui_DispTextLineAlign(3, DISPLAY_CENTER, "Pls wait", 0);
    if (WifiCheck(NULL) == 0)
    {
        WifiClose();
    }

    ret = WifiOpen();
	WNET_STRLOG("ret:%d-", ret);
	if(ret < 0) {
		Ui_ClearLine(3, 4);
		Ui_DispTextLineAlign(3, DISPLAY_CENTER, "Init failed!", 0);
		Ui_Print(4, 0, "ret:%d", ret);
		while(1)
		{
			if(Ui_GetKey()==KB_ENTER) return -1;
		}        
	}

    extern int s_WifiSelectAp(ST_WIFIAP *outAps, ST_WIFI_PARAM *WifiParam);
    if (s_WifiSelectAp(&Ap, &WifiParam) < 0)
    {
		return -1;
	}
	
	if(ret!=0) return ret;

	ret = WifiConnect(&Ap, &WifiParam);
	if(ret!=0) return ret;

	while(1){   
		Ui_ClearLine(1, 3);
		Ui_Print(3,0,"Connecting %s...", Ap.apName);
		ret = WifiCheck(NULL);
		if(ret) break;
		DelayMs(200);
		if(Ui_GetKey()==KB_CANCEL) return -1;
	}	


	return 0;

}



int setcomm_wlan(void * pxPARAM)
{
	ST_WLAN_CFG cfg;
    int tm_no = 1;

	RouteSetDefault(WNET_ROUTE);
    memset((u8*)&cfg, 0x00, sizeof(ST_WLAN_CFG));
    cfg.SimCardSel = 0;//SIM0;
    strcpy(cfg.SimPin, "card");
    strcpy(cfg.AccessPointName, "CMNET"); 
	WlInit(cfg);

	TimerSet(tm_no,30000);
	Ui_Clear();
	while(0!=TimerCheck(tm_no))
	{        
        Ui_Print(1,0,"Check Network");
        if(WlanGetSign()!=5){
			Ui_Print(1,0,"Network Regist OK");
			
            break;
        }
        if(Ui_WaitKey(500)==KB_CANCEL) {
            return -1;
        }
    } 

	if(0==TimerCheck(tm_no))
	{
		return -1;
	}

	
	return 0;

}

int set_common_type(void * pxPARAM)
{
    u8 	key = KB_NONE;
    u8  disp_buf[64] = { 0 };
    int route_flag = RouteGetDefault();
    int loop = 1;

RE_SET:
    Ui_Clear();
    memset(disp_buf, 0, sizeof(disp_buf));
    sprintf(disp_buf, "Set Commu Type [%s]", route_flag==WIFI_ROUTE?"WiFi":"Wlan");
    Ui_DispTitle(disp_buf);
    Ui_DispTextLineAlign(1, DISPLAY_LEFT, "1. WiFi", 0);
    Ui_DispTextLineAlign(2, DISPLAY_LEFT, "2. Wlan", 0);
    Ui_ClearKey();
    while (1)
    {
        switch (Ui_WaitKey(1000))
        {
            case KB1:
                if (!setcomm_wifi(NULL))
                {
                    Ui_ClearLine(1, 9);
                    Ui_DispTextLineAlign(1, DISPLAY_CENTER, "WiFi Set Success", 0);
                    Ui_WaitKey(3 * 1000);
                    RouteSetDefault(WIFI_ROUTE);
                    goto RE_SET;
                }
                else
                {
                    Ui_ClearLine(1, 9);
                    Ui_DispTextLineAlign(1, DISPLAY_CENTER, "WiFi Set Err", 0);
                    Ui_WaitKey(3 * 1000);
                    goto RE_SET;
                }
               
                break;
            case KB2:
                if (!setcomm_wlan(NULL))
                {
                    Ui_ClearLine(1, 9);
                    Ui_DispTextLineAlign(1, DISPLAY_CENTER, "Wlan Set Success", 0);
                    Ui_WaitKey(3 * 1000);
                    RouteSetDefault(WNET_ROUTE);
                    goto RE_SET;
                }
                else
                {
                    Ui_ClearLine(1, 9);
                    Ui_DispTextLineAlign(1, DISPLAY_CENTER, "Wlan Set Err", 0);
                    Ui_WaitKey(3 * 1000);
                    goto RE_SET;   
                }
                
                break;
            case KB_CANCEL:
                return -1;
            case KB_ENTER:
                return 0;
                break;
            default:
                break;
        }
            
    }

    return 0;

}


int Wlan_TestMenu(void * pxPARAM)
{
    int ret;
    const ST_MENUITEM menu[] = {
        {KB1,	"1.COMM",			s_WnetApiTest2,			(void *)NULL},
        {KB2,	"2.INFO",			WlanGetInfoTest,		(void *)NULL},
    };

    return Ui_Menu("GPRS TEST", menu, sizeof(menu)/sizeof(ST_MENUITEM), APP_UI_TIMEOUT);
}

