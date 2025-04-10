#include "app.h"
#include "ui.h"
#include "utils.h"

#ifdef WIFI_DEBUG
#define WIFI_STRLOG DBG_STR
#define WIFI_HEXLOG DBG_HEX
#else
#define WIFI_STRLOG  
#define WIFI_HEXLOG 
#endif

#define MAX_WIFIAP_NUM SCAN_AP_MAX

int s_WifiSelectAp(ST_WIFIAP *outAps, ST_WIFI_PARAM *WifiParam)
{
    int PAGE_SIZE=6;//8
    int         num, ret;
    int         pageNo;
    int         page;
    int         selected;
    u8        tmp[100];
    ST_WIFIAP * apList;
	int scan_cnt=0;
    
    Ui_Clear();
    Ui_DispTitle("Scan APs");
	Ui_DispTextLineAlign(2, DISPLAY_CENTER, "pls wait...", 0);
    apList = (ST_WIFIAP *)malloc(sizeof(ST_WIFIAP) * MAX_WIFIAP_NUM);
    if(apList == NULL) {
        return APP_RET_ERR_MEM;
    }

SCAN_AP:
    memset((char *)apList, 0, sizeof(ST_WIFIAP) * MAX_WIFIAP_NUM);
    num  = WifiScan(apList, MAX_WIFIAP_NUM);
WIFI_STRLOG("num:%d", num);
    if(num <= 0) {
		if(num == WIFI_RET_ERROR_NOTOPEN){
			DelayMs(100);
			goto SCAN_AP;
		}
		if(scan_cnt < 3){
			scan_cnt++;
			goto SCAN_AP;
		}
        Ui_ClearLine(1, 9);
        Ui_DispTextLineAlign(3, DISPLAY_CENTER, "No AP available", 0);
        Utils_BeepFAIL();
        Ui_WaitKey(APP_PROMPT_TIMEOUT);
        ret = APP_RET_ABORT;
        goto FINISH;
    }

SELECT:
    page = (num + PAGE_SIZE - 1) / PAGE_SIZE;
    selected = -1;
    Ui_Clear();
    Ui_DispTitle("Select a AP");
    for(pageNo = 0; pageNo < page; ) {
        int line = 0;
        int lineNo;
        u8 key;
        
        Ui_ClearLine(1, 9);
        line = num - pageNo * PAGE_SIZE;
        if(line > PAGE_SIZE) {
            line = PAGE_SIZE;
        }
        for(lineNo = 0; lineNo < line; lineNo ++) {
            char name[60];
            memset(name, 0X00, sizeof(name));
            sprintf(name, "%d.%s,%sdB", 1+lineNo, apList[pageNo * PAGE_SIZE + lineNo].apName, apList[pageNo * PAGE_SIZE + lineNo].apSignal);
            Ui_DispTextLineAlign(1+lineNo, DISPLAY_LEFT, name, 0);
        }

        if(lineNo<8) Ui_DispTextLineAlign(8, DISPLAY_CENTER, "KBCLEAR-Retry", 0);
        key = Ui_WaitKey(APP_UI_TIMEOUT);
WIFI_STRLOG("KEY:%02x, pageNo:%02x", key, pageNo);
        if(key >= KB1 && key <= (KB0 + line)) {
            selected = pageNo * PAGE_SIZE + (key - KB0) - 1;
            break;
        }
        
        switch(key)
        {
        case KB_CLEAR:
             goto SCAN_AP;
        case KB_CANCEL:
        case KB_NONE:
            ret = APP_RET_TIMEOUT;
            goto FINISH;
            
        case KB_UP:
            if(pageNo > 0) {
                pageNo --;
            }
            break;
            
        case KB_DOWN:
            if(pageNo < page - 1) {
                pageNo ++;
            }
            break;
        }
    }

    Ui_Clear();
    Ui_DispTitle("Confirm AP");
    Ui_DispTextLineAlign(1, DISPLAY_LEFT, "Selected:", 0);
    Ui_DispTextLineAlign(2, DISPLAY_LEFT, apList[selected].apName, 0);
    Ui_DispTextLineAlign(3, DISPLAY_LEFT, "[CONFIRM]-continue", 0);
    Ui_DispTextLineAlign(4, DISPLAY_LEFT, "[CANCEL]-sel another", 0);

    while(1)
    {
        u8 key;
        key = Ui_WaitKey(APP_UI_TIMEOUT);
        if(key == KB_ENTER) {            
            WIFI_STRLOG("select SSID:%s", apList[selected].apName);
            WIFI_STRLOG("select MODE:%s", apList[selected].apMode);
            memcpy((u8*)outAps, (u8*)&apList[selected], sizeof(ST_WIFIAP));
            ret = 0;
            break;
        }
        if(key == KB_NONE) {
            ret = APP_RET_TIMEOUT;
            goto FINISH;
        }
        if(key == KB_CANCEL) {
            goto SELECT;
        }
    }

    //Ui_Clear();
    //Ui_DispTitle("Set AP's PWD");
    //Ui_DispTextLineAlign(1, DISPLAY_LEFT, "Current PWD:", 0);
    //Ui_DispTextLineAlign(8, DISPLAY_CENTER, "DOWN--Alphabet", 0);
    memset(tmp, 0, sizeof(tmp));
    strcpy(tmp, "12345678");
	num = Ui_InputStrSelectMode(3, "Set AP's PWD", "Pls input PWD:", tmp, INPUT_STR, 50, 0, APP_UI_TIMEOUT);
    //num = Ui_InputStr(NULL, "Pls input PWD:", tmp, 50, 0, APP_UI_TIMEOUT);
    if(num == UI_RET_SUCCESS) {
        WifiParam->DhcpEnable =1;
        strcpy(WifiParam->psw, tmp);
        WIFI_STRLOG("input key:%s, ret:%d", tmp, ret);
        ret = 0;
    }
    else ret = num;
FINISH:
    free(apList);
    
    return ret;
}

int WifiFunTest(void * pxPARAM)
{
    u8 txbuf[1024], rxbuf[1024], buf[1024], tm=1;
    int ret, txoff, txlen,rxoff, rxlen, i,j,sta, socket_id;
    ST_WIFIAP Ap, con_AP;
    ST_WIFI_PARAM WifiParam;
    u32 timeout=0, speed=0,total_cnt, suc_cnt, fail_cnt, cnt;
    struct net_sockaddr addr;
    socklen_t addrlen;

    Ui_ClearLine(1, 9);
    Ui_ClearKey();


    Ui_DispTextLineAlign(2, DISPLAY_CENTER, "Initializing", 0);
    Ui_DispTextLineAlign(3, DISPLAY_CENTER, "Pls wait", 0);   
    ret = WifiOpen();
WIFI_STRLOG("ret:%d-", ret);
    if(ret < 0) {
        Ui_ClearLine(3, 4);
        Ui_DispTextLineAlign(3, DISPLAY_CENTER, "Init failed!", 0);
        Ui_Print(4, 0, "ret:%d", ret);
        while(1)
        {
            if(Ui_GetKey()==KB_ENTER) return 0;
        }        
    }

    if(s_WifiSelectAp(&Ap, &WifiParam) < 0) {
        return 0;
    }
    
    if(ret!=0) return ret;

    ret = WifiConnect(&Ap, &WifiParam);
    if(ret!=0) return ret;

    while(1){   
        Ui_ClearLine(1, 4);
        Ui_Print(3,0,"Connecting %s...", Ap.apName);
        ret = WifiCheck(NULL);
        if(ret) break;
        DelayMs(200);
        if(Ui_GetKey()==KB_CANCEL) goto EXIT;
    }
    
    sta=0;
    timeout = 30000;

    Ui_DispTitle("WIFI TEST");
    cnt=0;
    total_cnt=1000;
    suc_cnt=0;
    fail_cnt = 0;
PPP_LOG:
    Ui_ClearLine(1, 1);
    ret = WifiCheck(&con_AP);
    Ui_ClearLine(1, 3);
    Ui_Print(1,0,"Connected %s, %d dB, level:%d", con_AP.apName, con_AP.apSignal, ret); 
    WIFI_STRLOG("Connected %s, %s dB, level:%d", con_AP.apName, con_AP.apSignal, ret);   

    RouteSetDefault(WIFI_ROUTE);//wifi
    ret = NetSocket(NET_AF_INET, NET_SOCK_STREAM, 0);
    Ui_ClearLine(2, 2);Ui_Print(2,0,"NetSocket:%d", ret);
    if(ret<0) goto EXIT;

    socket_id = ret;
    sta = 0;
    timeout = 30000;
    
    TimerSet(tm, timeout/100);
    while(1){
        sta = Netioctl(socket_id, CMD_TO_SET, 0);
        Ui_ClearLine(2, 2);Ui_Print(2,0,"sta:%2x, %d", sta&0xff, TimerCheck(tm));
        if(sta & SOCK_EVENT_ACCEPT) {
            Ui_ClearLine(2, 2);
            Ui_Print(2,0,"WIFI Connect success,[%d]ms", timeout-TimerCheck(tm));
            break;
        }
        if(0==TimerCheck(tm)) {
            fail_cnt++;
            goto TCP_DISCONNECT;
            //Ui_WaitKey(0xffffffff);ret = sta;goto EXIT;//return sta;
        }
        if(Ui_GetKey()==KB_CANCEL) goto EXIT;
    }


TCP_CONNECT:
    Ui_ClearLine(3, 9);
    timeout = 30000;

    SockAddrSet(&addr, "47.98.106.63", 6666);
    ret = NetConnect(socket_id, &addr, sizeof(struct net_sockaddr));
    sta = 0;
    TimerSet(tm, timeout/100);
    while(1){
        sta = Netioctl(socket_id, CMD_TO_SET, 0);
        Ui_ClearLine(3, 3);Ui_Print(3,0,"sta:%2x, %d", sta&0xff, TimerCheck(tm));
        if(sta & SOCK_EVENT_CONN) {
            Ui_ClearLine(3, 3);
            Ui_Print(3,0,"Connect success,[%d]ms", timeout-TimerCheck(tm));
            break;
        }
        if(0==TimerCheck(tm)) {
            WIFI_STRLOG("sta:%2x, %d", sta&0xff, TimerCheck(tm));
            fail_cnt++;
            goto TCP_DISCONNECT;
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
                WIFI_STRLOG("************tx fail: %d", ret);
                Ui_ClearLine(4, 4);Ui_Print(4,0, "tx fail: %d", ret);
                if (ret) {
                    //Ui_WaitKey(0xffffffff);goto EXIT;//return ret;
                    fail_cnt++;
                    goto TCP_DISCONNECT;
                }            
            }
            txoff += ret;
            if (txoff == txlen) break;
        }while(1);

        timeout = 30000;
        TimerSet(tm, timeout/100);
        memset(rxbuf, 0x00, sizeof(rxbuf));
        rxoff = 0;
        do{
            if(Ui_GetKey()==KB_CANCEL) goto EXIT;
            Ui_ClearLine(4, 4);Ui_Print(4,0,"rx[%d]", cnt);
            ret = NetRecv(socket_id, rxbuf+rxoff, txlen-rxoff, 0);
            if(ret<0){
                WIFI_STRLOG("****************rx fail: %d", ret);
                if (ret) {
                    Ui_ClearLine(4, 4);Ui_Print(4,0,"rx ret:%d",ret);
                    //Ui_WaitKey(0xffffffff);goto EXIT;//return ret;
                    fail_cnt++;
                    goto TCP_DISCONNECT;
                }            
            }
            rxoff += ret;
            if (rxoff == txlen) {
                speed = (txlen*1000)/(timeout - TimerCheck(tm));
                break;
            }
            if(0==TimerCheck(tm)) {
                WIFI_STRLOG("********rx timeout,%d,%d", rxoff, ret);

                Ui_ClearLine(4, 4);Ui_Print(4,0, "rx err,%d,%d", rxoff, ret);

                //Ui_WaitKey(0xffffffff);goto EXIT;//return ret;
                fail_cnt++;
                goto TCP_DISCONNECT;
            }
            //DEBUG_PRINT("rxoff:%d,ret:%d, :%d\r\n", rxoff, ret, cnt);
        }while(1);
        if (memcmp(rxbuf, txbuf, txlen)== 0) {
            WIFI_STRLOG("[%d][%d],txlen:%d, rx==tx\r\n", j,cnt, txlen);
            Ui_ClearLine(5, 5);Ui_Print(5,0,"OK[%d][%d][%d B/s]", cnt, txlen, speed);
        }
        else {
            WIFI_STRLOG("[%d][%d],txlen:%d, rx!=tx\r\n",j,cnt, txlen);
            Ui_ClearLine(5, 5);Ui_Print(5,0,"FAIL[%d][%d][%d]rx!=tx\r\n", j,cnt, txlen); 
            for(i=0; i<txlen; i++){
                if(rxbuf[i]==txbuf[i]) continue;
                DBG_HEX("rxbuf:", rxbuf+i, txlen-i);
                DBG_HEX("txbuf:", txbuf+i, txlen-i);
                break;
            }

            //Ui_WaitKey(0xffffffff);
            fail_cnt++;
            goto TCP_DISCONNECT;
        } 

        if(Ui_GetKey()==KB_CANCEL) goto EXIT;
        else break;
    }//while(1)      

TCP_DISCONNECT:
    NetCloseSocket(socket_id);
    cnt++;

    sta = 0;
    TimerSet(tm, 600);    
    while(1){
        sta = Netioctl(socket_id, CMD_TO_SET, 0);
        Ui_ClearLine(6, 6);Ui_Print(6,0,"CLOSE sta:%2x, %d", sta&0xff, TimerCheck(tm));
        if(!(sta & SOCK_EVENT_CONN)) {
            WIFI_STRLOG("app close ok");
            Ui_ClearLine(6, 6);Ui_Print(6,0,"TCP CLOSE OK");
            break;
        }
        if(0==TimerCheck(tm)) {
            WIFI_STRLOG("sta:%2x, %d", sta&0xff, TimerCheck(tm));
            Ui_WaitKey(0xffffffff);ret = sta;goto EXIT;//return sta;
        }
        if(Ui_GetKey()==KB_CANCEL) goto EXIT;
    }

    suc_cnt++;
    goto PPP_LOG;
EXIT:
    //sta = Netioctl(socket_id, CMD_TO_SET, 0);
    //if(sta>0 && (sta&SOCK_EVENT_CONN)){
        TimerSet(tm, 600);
        NetCloseSocket(socket_id);    
        while(1){
            sta = Netioctl(socket_id, CMD_TO_SET, 0);
            Ui_ClearLine(6, 6);Ui_Print(6,0,"CLOSE sta:%02x, %d", sta&0xff, TimerCheck(tm));
            if(!(sta & SOCK_EVENT_CONN)) {
                WIFI_STRLOG("app close ok");
                Ui_ClearLine(6, 6);Ui_Print(6,0,"TCP CLOSE OK");
                break;
            }
            if(0==TimerCheck(tm)) {
                WIFI_STRLOG("sta:%2x, %d", sta&0xff, TimerCheck(tm));
                Ui_WaitKey(0xffffffff);ret = sta;goto EXIT;//return sta;
            }
            if(Ui_GetKey()==KB_CANCEL) break;
        }
    //}

    WifiDisconnect();

    return ret;
}


