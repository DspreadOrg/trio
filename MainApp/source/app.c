#include "app.h"
#include "ui.h"
#include "trans.h"
#include "utils.h"
#include "emvl2.h"
#include "emv.h"
#include "emvproc.h"
#include "cls.h"
#include "aa.h"
#include "Liteui.h"

#include <errno.h>

#ifdef APP_DEBUG
#define APP_STRLOG DBG_STR
#define APP_HEXLOG DBG_HEX
#else
#define APP_STRLOG  
#define APP_HEXLOG 
#endif

#define  EMVL2_ADDR     		   0x14138220

u8 g_load_param_flag = 0;

static ST_TRANLOG m_translog;
extern int test_printer(void * pxPARAM);
extern int Wlan_TestMenu(void * pxPARAM);
extern int WifiFunTest(void * pxPARAM);
extern int EmvL2FileRead(u32 index, u32 offset, u8 *data, u32 len);
extern int EmvL2FileWrite(u32 index, u32 offset, u8 *data, u32 len);
extern int EmvL2FileDel(u32 startindex, u32 endindex);
extern int EmvL2GetSysTime(SYS_TIME * time);
extern int EmvL2SetSysTime(SYS_TIME * time);
extern int EmvL2GetSysTimer(ST_Timer_UNIT *timer,u32 timeout);
extern int EmvL2SysTimerLeft(ST_Timer_UNIT *timer);
extern int EmvL2GetSysTick();
extern void EmvL2DelayMs(u32 ms);
extern void EmvL2Des(unsigned char *input,unsigned char *output,unsigned char *key, u32 mode);
extern int EmvL2Aes(unsigned char *Input, unsigned char *Output, unsigned char *Aeskey, u32 keyLen, u32 Mode);
extern int EmvL2Rsa(u32 bitsize, const u8 *input, u8 *output, u8 *mod, u32 expLen,u8 *exp);
extern int EmvL2Sha(u32 type,u8 *hash, u32 dataLen, u8 *data);
extern int EmvL2ScExchange(const char *DeviceId, const C_APDU *c_apdu, R_APDU *r_apdu);
extern int EmvL2GetRandom(u8 *rand,u32 len);
extern int EmvL2OfflinePinVerify(u32 offline_type, ST_SCPINKEY *RsaKey, u8 *status_word);

extern int Demo_DispImage(void);
int s_sysStatusProc()
{
    CLcdDispStatus(0);
	Ui_Clear();
	Ui_DispTextLineAlign(0, DISPLAY_CENTER, "close sys status", 1);
	Ui_DispTextLineAlign(1, DISPLAY_LEFT, "ok", 0);
	Ui_DispTextLineAlign(2, DISPLAY_RIGHT, "ok", 0);
	Ui_DispTextLineAlign(3, DISPLAY_CENTER, "ok", 0);
	Ui_WaitKey(30000);
	
    CLcdDispStatus(1);
	Ui_Clear();
	Ui_DispTextLineAlign(0, DISPLAY_CENTER, "open sys status", 1);
	Ui_DispTextLineAlign(1, DISPLAY_LEFT, "ok", 0);
	Ui_DispTextLineAlign(2, DISPLAY_RIGHT, "ok", 0);
	Ui_DispTextLineAlign(3, DISPLAY_CENTER, "ok", 0);
	Ui_WaitKey(30000);
	return 0;
}

ST_TRANLOG * App_GetTranLog()
{
	return &m_translog;	
}

void App_InitTran()
{
	byte     bcdtime[10];
	byte     asctime[20];
	ST_TRANLOG * pxLOG = App_GetTranLog();
	
	memset((char *)pxLOG, 0, sizeof(ST_TRANLOG));
	pxLOG->type = 0;
	memset(bcdtime,0,sizeof(bcdtime));	
	GetTime(bcdtime);
	APP_HEXLOG("GetTime:",bcdtime,7);
	memset(asctime,0,sizeof(asctime));	
	Utils_Bcd2Asc(bcdtime,6,asctime);
	memcpy(pxLOG->date,"20",2);
	memcpy(pxLOG->date+2,asctime,6);
	memcpy(pxLOG->time,asctime+6,6);
	strcpy(pxLOG->cardUnit, "CUP");
}

int SleepWakeUp_Test(void *pxPARAM)
{
	unsigned char track1[128] = { 0 };
	unsigned char track2[128] = { 0 };
	unsigned char track3[128] = { 0 };
	u8 uRet = 0;
	
	DBG_STR("Before Sleep \r\n");
	int wake_ret = SysSleep("\x00\x01");

	switch (wake_ret)
	{
	case 1://When the wake-up source is to swipe the magnetic stripe card, the magnetic stripe card data must be taken away .
		{
			DBG_STR("The wake-up source is to swipe the magnetic stripe card.\r\n");
			uRet = MagSwiped();
			if (uRet == 0)
			{
				DBG_STR("MagSwiped Error");
				return -1;

			}
			uRet = MagRead(track1, track2, track3);
			if (uRet == 0)
				DBG_STR("MagRead Error\r\n");
		}
		break;
	case 2:
		DBG_STR("The wake-up source is to insert IC card.\r\n");
		break;
	case 3:
		DBG_STR("The wake-up source is to press key.\r\n");
		break;
	case -1:
	case -2:
	case -3:
	case -4:
		return -1;
	default:
		break;
	}

	return 0;

}

int App_CardMenu(void *pxPARAM)
{
	int i=0;
	char tmp1[30],tmp2[30],tmp3[30];
	ST_MENUITEM menu[4];
	
	if(!g_load_param_flag)
	{
		tmp1[1] = 1;
		EMV_InitCore();//once power on excute
		Trans_MkskKeyInit(tmp1);
		EMV_InitDefault(tmp1);
		g_load_param_flag = 1;
		
	}	

	memset(tmp1,0,sizeof(tmp1));
	menu[i].kb = i+'1';
	sprintf(tmp1,"%d.%s",i+1,"mag card test");
	menu[i].text = tmp1;
	menu[i].func= Trans_MagcardTest;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp2,0,sizeof(tmp2));
	menu[i].kb = i+'1';		
	sprintf(tmp2,"%d.%s",i+1,"chip card test");
	menu[i].text = tmp2;
	menu[i].func= Trans_IcccardTest;
	menu[i].param= (void *)NULL;
	i++;

#ifdef CTLS_KERNAL_ENABLE
	memset(tmp3,0,sizeof(tmp3));
	menu[i].kb = i+'1';		
	sprintf(tmp3,"%d.%s",i+1,"tap card test");
	menu[i].text = tmp3;
	menu[i].func= Trans_PicccardTest;
	menu[i].param= (void *)NULL;
	i++;
#endif

	Ui_Menu("card test", menu, i, APP_UI_TIMEOUT);
  	return APP_RET_NO_DISP;
}

int Param_Reset(void * pxPARAM)
{
	Ui_Clear();
	Ui_DispTitle("Sys Reset");
	Ui_DispTextLineAlign(1, DISPLAY_CENTER, "WARNING", 0);
	Ui_DispTextLineAlign(2, DISPLAY_CENTER, "operate will clear all data", 0);
	Ui_DispTextLineAlign(3, DISPLAY_CENTER, "[enter]go [cancel]quite", 0);
	Utils_BeepFAIL();
	if(Ui_WaitKey(APP_UI_TIMEOUT) != KB_ENTER) {
		return 0;
	}
	SysReset();
	Reboot();
	while(1);
	
	return 0;
}

int File_test(void * pxPARAM){
	int iRet,hwd,num=0,i;
	char fname[32],data[128];
	
	Ui_Clear();
	Ui_DispTitle("File Test");
	Ui_DispTextLineAlign(1, DISPLAY_CENTER, "file testing...", 0);

	for(num=0;num<256;num++){
		memset(fname,0,sizeof(fname));
		sprintf(fname,"file%d",num);
		hwd = open(fname,O_CREATE|O_RDWR);
		APP_STRLOG("%d,open %s=%d",num,fname,hwd);
		if(hwd < 0){
			break;
		}
		
		iRet = write(hwd, (byte*)fname, strlen(fname));
		if(iRet != strlen(fname)){
			close(hwd);
			APP_STRLOG("err write(%s) = %d, %d\r\n", fname, hwd, iRet);
			break;
		}

		iRet = seek(hwd, 0, SEEK_SET);
		if(iRet < 0) {
			close(hwd);
			APP_STRLOG("err seek(%d):%d\r\n", hwd, iRet);
			return -1;
		}

		memset(data,0,sizeof(data));
		iRet = read(hwd, (byte*)data, strlen(fname));
		if(iRet != strlen(fname)){
			close(hwd);
			APP_STRLOG("err read(%s) = %d, %d\r\n", fname, hwd, iRet);
			return -1;
		}
		APP_STRLOG("read(%s) = %d, %d\r\n", data, hwd, iRet);
		close(hwd);
	}
	
	for(i=0;i<num;i++){
		memset(fname,0,sizeof(fname));
		sprintf(fname,"file%d",i);
		remove(fname);
	}

	memset(data,0,sizeof(data));
	sprintf(data,"%d file test",num);
	Ui_DispTextLineAlign(2, DISPLAY_CENTER, data, 0);
	Ui_DispTextLineAlign(3, DISPLAY_CENTER, "SUCCESS", 0);
	Utils_BeepOK();
	Ui_WaitKey(5000);
	return 0;
}

int App_SysMenu(void * pxPARAM)
{	
	int i=0;
	char tmp1[30],tmp2[30],tmp3[30],tmp4[30],tmp5[30],tmp6[30],tmp7[30];
	ST_MENUITEM menu[7];

	memset(tmp1,0,sizeof(tmp1));
	menu[i].kb = i+'1';
	sprintf(tmp1,"%d.%s",i+1,"MKSk INIT");
	menu[i].text = tmp1;
	menu[i].func= Trans_MkskKeyInit;
	menu[i].param= (void *)NULL;
	i++;
	
	memset(tmp2,0,sizeof(tmp2));
	menu[i].kb = i+'1'; 	
	sprintf(tmp2,"%d.%s",i+1,"load emv prm");
	menu[i].text = tmp2;
	menu[i].func= EMV_InitDefault;
	menu[i].param= (void *)NULL;
	i++;	

	memset(tmp3,0,sizeof(tmp3));
	menu[i].kb = i+'1'; 	
	sprintf(tmp3,"%d.%s",i+1,"sys status test");
	menu[i].text = tmp3;
	menu[i].func= s_sysStatusProc;
	menu[i].param= (void *)NULL;
	i++;	
	
	memset(tmp5,0,sizeof(tmp5));  
	menu[i].kb = i+'1'; 	
	sprintf(tmp5,"%d.%s",i+1,"File test");
	menu[i].text = tmp5;
	menu[i].func= File_test;
	menu[i].param= (void *)NULL;
	i++;

	extern int set_common_type(void *pxPARAM);
	memset(tmp6, 0, sizeof(tmp6));
	menu[i].kb = i+'1'; 	
	sprintf(tmp6,"%d.%s",i+1,"Communication Set");
	menu[i].text = tmp6;
	menu[i].func= set_common_type;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp7, 0, sizeof(tmp7));
	menu[i].kb = i+'1'; 	
	sprintf(tmp7,"%d.%s",i+1,"Sleep awakening test");
	menu[i].text = tmp7;
	menu[i].func= SleepWakeUp_Test;
	menu[i].param= (void *)NULL;
	i++;
	
	Ui_Menu("param set", menu, i, APP_UI_TIMEOUT);
  	return APP_RET_NO_DISP;
}

int App_lcdflash(void * pxPARAM){

	Ui_ClearBlockColor(0,0,320,240,DISPLAY_COLOR_RED);
	DelayMs(500);
	Ui_ClearBlockColor(0,0,320,240,DISPLAY_COLOR_GRAY);
	DelayMs(500);
	Ui_ClearBlockColor(0,0,320,240,DISPLAY_COLOR_BLACK);
	DelayMs(500);
	Ui_ClearBlockColor(0,0,320,240,DISPALY_COLOR_GREEN);
	DelayMs(500);
	Ui_ClearBlockColor(0,0,320,240,DISPLAY_COLOR_WHITE);
	DelayMs(500);
	Ui_DispBMP(88, 108, 144, 72, cls_array);
	DelayMs(500);
	Ui_DispBMP(0, 0, 320, 240, aa_array);
	DelayMs(500);
	return 0;
}

int AboutVersion(void * pxPARAM)
{
	u8 ver[10]={0},strver[48]={0};
	u8 info[30] = {0};

	memset(ver,0,sizeof(ver));
	ReadVerInfo(ver);
	Ui_Clear();
	Ui_DispTitle("Terminal Information");
	//Ui_DispTextLineAlign(1, DISPLAY_CENTER, "Terminal Information", 0);
	memset(strver,0,sizeof(strver));
	sprintf(strver,"APP:%s\0",APP_VER);
	Ui_DispTextLineAlign(3, DISPLAY_LEFT, strver, 0);
	memset(strver,0,sizeof(strver));
	sprintf(strver,"sbi:%d.%d OS:%d.%d.%d%%",ver[0],ver[1],ver[2],ver[3],ver[4]);
	Ui_DispTextLineAlign(5, DISPLAY_LEFT, strver, 0);

	GetTermInfo(info);
	u8* disp_str = (info[21] == 0x00?"Debug Mode Device":"NoDebug Mode Device");
	Ui_DispTextLineAlign(7, DISPLAY_LEFT, disp_str, 0);

	Ui_WaitKey(5000);	

	return 0;
}

int Display_Image(void * pxPARAM)
{
	Demo_DispImage();
	return 0;

}


int keyBoardTest(void * pxPARAM)
{
	char keyValue;
	char string[128];

	Ui_Clear();
	Ui_DispText(0,2*24,"wait for key", 0);
	while (1)
	{
		keyValue = getkey();
		if(keyValue == KEYENTER)
		{
			break;
		}
		
		memset(string, 0, sizeof(string));
		sprintf(string, "Key:0x%02X", keyValue);

		Ui_Clear();
		Ui_DispText(0,2*24,string, 0);
		Ui_DispText(0,4*24,"key\"Enter\"(0x0d) to exit", 0);
	}
	
}

extern int FontAlignTest(void * pxPARAM);
extern int FontDisplayWithXY(void * pxPARAM);
extern int FontSetAttr(void * pxPARAM);
extern int LcdBackGroundTest(void * pxPARAM);
extern int printerTest(void * pxPARAM);
extern int icon_unLockTest(void * pxPARAM);
extern int backLitghtTest(void * pxPARAM);
extern int 	ota_demo(void * pxPARAM);


int Other_Func(void * pxPARAM)
{
	int i=0;
	char tmp1[30],tmp2[30],tmp3[30],tmp4[30],tmp5[30],tmp6[30],tmp7[30],tmp8[30],tmp9[30];
	ST_MENUITEM menu[9];

	memset(tmp1,0,sizeof(tmp1));
	menu[i].kb = i+'1';
	sprintf(tmp1,"%d.%s",i+1,"A-2/3 Align Direction");
	menu[i].text = tmp1;
	menu[i].func= FontAlignTest;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp2,0,sizeof(tmp2));
	menu[i].kb = i+'1';
	sprintf(tmp2,"%d.%s",i+1,"A-4/5/6 XY Retract PNG");
	menu[i].text = tmp2;
	menu[i].func= FontDisplayWithXY;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp3,0,sizeof(tmp3));
	menu[i].kb = i+'1';
	sprintf(tmp3,"%d.%s",i+1,"A-8/16 SetFontAttr");
	menu[i].text = tmp3;
	menu[i].func= FontSetAttr;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp4,0,sizeof(tmp4));
	menu[i].kb = i+'1';
	sprintf(tmp4,"%d.%s",i+1,"A-27/28 background clear");
	menu[i].text = tmp4;
	menu[i].func= LcdBackGroundTest;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp5,0,sizeof(tmp5));
	menu[i].kb = i+'1';
	sprintf(tmp5,"%d.%s",i+1,"printf");
	menu[i].text = tmp5;
	menu[i].func= printerTest;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp6,0,sizeof(tmp6));
	menu[i].kb = i+'1';
	sprintf(tmp6,"%d.%s",i+1,"icon bar unlock");
	menu[i].text = tmp6;
	menu[i].func= icon_unLockTest;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp7,0,sizeof(tmp7));
	menu[i].kb = i+'1';
	sprintf(tmp7,"%d.%s",i+1,"LCD BackLitght");
	menu[i].text = tmp7;
	menu[i].func= backLitghtTest;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp8,0,sizeof(tmp8));
	menu[i].kb = i+'1';
	sprintf(tmp8,"%d.%s",i+1,"OTA Demo");
	menu[i].text = tmp8;
	menu[i].func= ota_demo;
	menu[i].param= (void *)NULL;
	i++;

	#if 0
	memset(tmp9, 0, sizeof(tmp9));
	menu[i].kb = i+'1';
	sprintf(tmp9,"%d.%s",i+1,"KeyBoard");
	menu[i].text = tmp9;
	menu[i].func= keyBoardTest;
	menu[i].param= (void *)NULL;
	i++;
	#else
	extern int test_tls(void *argv);
	memset(tmp9, 0, sizeof(tmp9));
	menu[i].kb = i+'1';
	sprintf(tmp9,"%d.%s",i+1,"TLS_Test");
	menu[i].text = tmp9;
	menu[i].func = test_tls;
	menu[i].param = (void *)NULL;
	i++;
	#endif



	return Ui_Menu("Other Func", menu, i, APP_UI_TIMEOUT);

}


typedef enum{
	APP_VERSION,
	PCI_INFO,
	TERMINAL_IMEI,
	TERMINAL_SN,
}TerminalInfoType_E;

#define PRODUCT_NAME "QPOS Trio"
#define HD_VER "001"
#define PCI_HD_VER "001"
#define PCI_FW_VER "1.0.1"

int ShowTerminalInfo(void * pxPARAM){

	u8 tempBuf[126+1]={0}, sn[33]={0}, imei[33]={0};

	Ui_Clear();
	switch (((u8*)pxPARAM)[0])
	{
	case APP_VERSION:
		AboutVersion(NULL);
		break;
	case PCI_INFO:

		memset(tempBuf, 0, sizeof(tempBuf));
		sprintf(tempBuf, "Product Name:%s", PRODUCT_NAME);
		Ui_DispTextLineAlign(1, DISPLAY_LEFT, tempBuf, 0);

		memset(tempBuf, 0, sizeof(tempBuf));
		sprintf(tempBuf, "HardWare Version:%s", HD_VER);
		Ui_DispTextLineAlign(3, DISPLAY_LEFT, tempBuf, 0);

		memset(tempBuf, 0, sizeof(tempBuf));
		sprintf(tempBuf, "PCI Firmware Version:%s", PCI_FW_VER);
		Ui_DispTextLineAlign(5, DISPLAY_LEFT, tempBuf, 0);

		memset(tempBuf, 0, sizeof(tempBuf));
		sprintf(tempBuf, "PCI Hardware Version:%s", PCI_HD_VER);
		Ui_DispTextLineAlign(7, DISPLAY_LEFT, tempBuf, 0);

		break;
	case TERMINAL_IMEI:
		memset(tempBuf, 0, sizeof(tempBuf));
		memset(imei, 0, sizeof(imei));
		GetTermInfoExt("GPRS_IMEI", imei, 31);
		sprintf(tempBuf, "IMEI:%s", imei);
		Ui_DispTextLineAlign(1, DISPLAY_LEFT, tempBuf, 0);
		break;
	case TERMINAL_SN:
		memset(tempBuf, 0, sizeof(tempBuf));
		memset(sn, 0, sizeof(sn));
		ReadSN(sn);
		sprintf(tempBuf, "Serial number:");
		Ui_DispTextLineAlign(1, DISPLAY_LEFT, tempBuf, 0);

		memset(tempBuf, 0, sizeof(tempBuf));
		sprintf(tempBuf, "%s", sn);
		Ui_DispTextLineAlign(2, DISPLAY_LEFT, tempBuf, 0);

		break;
	default:
		break;
	}

	Ui_WaitKey(100*1000);

}

int AboutDevice(void * pxPARAM)
{
	int i=0;
	char tmp1[30],tmp2[30],tmp3[30],tmp4[30];
	ST_MENUITEM menu[4];
	u8 type1 = 0, type2 = 0, type3 = 0, type4 = 0;

	memset(tmp2,0,sizeof(tmp2));
	menu[i].kb = i+'1';
	sprintf(tmp2,"%d.%s",i+1,"Version Information");
	menu[i].text = tmp2;
	menu[i].func= ShowTerminalInfo;
	type1 = PCI_INFO;
	menu[i].param= (void *)&type1;
	i++;

	memset(tmp3,0,sizeof(tmp3));
	menu[i].kb = i+'1';
	sprintf(tmp3,"%d.%s",i+1,"IMEI Number");
	menu[i].text = tmp3;
	menu[i].func= ShowTerminalInfo;
	type2 = TERMINAL_IMEI;
	menu[i].param= (void *)&type2;
	i++;


	memset(tmp1,0,sizeof(tmp1));
	menu[i].kb = i+'1';
	sprintf(tmp1,"%d.%s",i+1,"SN Number");
	menu[i].text = tmp1;
	menu[i].func= ShowTerminalInfo;
	type3 = TERMINAL_SN;
	menu[i].param= (void *)&type3;
	i++;


	return Ui_Menu("About Device", menu, i, APP_UI_TIMEOUT);

}

int App_TransMenu(void * pxPARAM)
{
	int i=0;
	char tmp1[30],tmp2[30],tmp3[30],tmp4[30],tmp5[30],tmp6[30],tmp7[30],tmp8[30],tmp9[30];
	ST_MENUITEM menu[18];

	memset(tmp1,0,sizeof(tmp1));
	menu[i].kb = i+'1';
	sprintf(tmp1,"%d.%s",i+1,"card test");
	menu[i].text = tmp1;
	menu[i].func= App_CardMenu;
	menu[i].param= (void *)NULL;
	i++;
	
	memset(tmp2,0,sizeof(tmp2));
	menu[i].kb = i+'1';
	sprintf(tmp2,"%d.%s",i+1,"param set");
	menu[i].text = tmp2;
	menu[i].func= App_SysMenu;
	menu[i].param= (void *)NULL;
	i++;
	
	memset(tmp3,0,sizeof(tmp3));
	menu[i].kb = i+'1';
	sprintf(tmp3,"%d.%s",i+1,"lcd flash");
	menu[i].text = tmp3;
	menu[i].func= App_lcdflash;
	menu[i].param= (void *)NULL;
	i++;
	
	memset(tmp4,0,sizeof(tmp4));
	menu[i].kb = i+'1';
	sprintf(tmp4,"%d.%s",i+1,"printer test");
	menu[i].text = tmp4;
	menu[i].func= test_printer;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp5,0,sizeof(tmp5));
	menu[i].kb = i+'1';
	sprintf(tmp5,"%d.%s",i+1,"wlan test");
	menu[i].text = tmp5;
	menu[i].func= Wlan_TestMenu;
	menu[i].param= (void *)NULL;
	i++;
	
	memset(tmp6,0,sizeof(tmp6));
	menu[i].kb = i+'1';
	sprintf(tmp6,"%d.%s",i+1,"wifi test");
	menu[i].text = tmp6;
	menu[i].func= WifiFunTest;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp7,0,sizeof(tmp7));
	menu[i].kb = i+'1';
	sprintf(tmp7,"%d.%s",i+1,"Other Func");
	menu[i].text = tmp7;
	menu[i].func= Other_Func;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp8,0,sizeof(tmp8));
	menu[i].kb = i+'1';
	sprintf(tmp8,"%d.%s",i+1,"About Device");
	menu[i].text = tmp8;
	menu[i].func= AboutDevice;
	menu[i].param= (void *)NULL;
	i++;

	memset(tmp9,0,sizeof(tmp9));
	menu[i].kb = i+'1';
	sprintf(tmp9,"%d.%s",i+1,"Version");
	menu[i].text = tmp9;
	menu[i].func= AboutVersion;
	menu[i].param= (void *)NULL;
	i++;

	return Ui_Menu("main menu", menu, i, APP_UI_TIMEOUT);
}

void s_IconUpdateProc(u32 ex_period_ms)
{
    u32 flash_period=3000;//ms
    static u32 cnt=0;
	u8 sign_level;

    if(ex_period_ms) {
        if(cnt++%(flash_period/ex_period_ms)) return; 
    }
	WlGetSignal(&sign_level);
	BatteryCheck();
}

const u32 api_array[] = {
    //platform api
    (u32)usb_debug,
    (u32)EmvL2FileRead,
    (u32)EmvL2FileWrite,
	(u32)EmvL2FileDel,
	(u32)EmvL2GetSysTime,
	(u32)EmvL2SetSysTime,
	(u32)EmvL2GetSysTimer,
	(u32)EmvL2SysTimerLeft,
	(u32)EmvL2GetSysTick,
	(u32)EmvL2DelayMs,
	(u32)EmvL2Des,
	(u32)EmvL2Aes,
	(u32)EmvL2Rsa,
	(u32)EmvL2Sha,
	(u32)EmvL2ScExchange,
	(u32)EmvL2GetRandom,
	(u32)EmvL2OfflinePinVerify,
};

int run_emv_kernel()
{
	u32 emventry;
	u32 (*fun_entry)(void *funs);
	
	fun_entry = (void*)EMVL2_ADDR;
	emventry = fun_entry((void*)api_array);
	init_emv_api(emventry);

    return 0;
}

void App_Main(void)
{
    u32 regval, cnt;
    u8 kb, buf[32];
    ST_LCD_INFO LcdInfo;
    int ret;
    u8 rsp[64];
    int type;
	
	LiteMapiInit();
	WifiOpen();

	Ui_Clear();
	Ui_ClearKey();
	Ui_DispTextLineAlign(2, DISPLAY_LEFT, "param init...", 0);
	
	run_emv_kernel();
#ifdef EMV_DEBUG
	EMV_SetDebug(1);
#endif

    while(1) {
		s_IconUpdateProc(0);
		App_TransMenu((void *)NULL);
		APP_STRLOG("while begin");
    }
    APP_STRLOG("APP Exist");
    return;
}
