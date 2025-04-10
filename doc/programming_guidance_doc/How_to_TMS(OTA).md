## **How to TMS/OTA**



The operation of OTA update APP is divided into the following two steps.

Step 1:  The device downloads and stores the APP file from the server, for example the name is `ota.asc`.The premise of this step is to push the compiled app file to the remote server. 

Step 2:  Execute `FileToApp to` complete APP update. for example the name of saved file in trio device is `ota.asc`, you need to execute `FileToApp(“ota.asc”)`.

**Note: **

**1. Currently TMS  example does not support the resumable upload function, and the original downloaded incomplete files will be cleared every time it is updated.**

**2. Currently TMS example only supports APP update**.

**3. For the tms example in Demo, dspread has pushed a different [LiteMApiTestApp_sign.bin](https://gitlab.com/dspread/trio/-/blob/main/release/LiteMApiTestApp_sign.bin) file to the remote server.The difference between the APP files on the remote server is that the version number in menu 8 is [F.OTA.Version], as shown in the figure below.**

![](https://i.postimg.cc/bwVwGkj4/20230714154020.png)


For the above, please refer to the OTA_Cellular and OTA_Wifi functions in the Demo code.

```C
int OTA_Cellular(void * pxPARAM)
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

	Ota_Process();

	
	return 0;

}
```

```C
int OTA_Wifi(void * pxPARAM)
{

    int ret;
    ST_WIFIAP Ap;
    ST_WIFI_PARAM WifiParam;


    Ui_ClearLine(1, 9);
    Ui_ClearKey();

	if(WifiCheck(NULL)<0)
	{
		Ui_DispTextLineAlign(2, DISPLAY_CENTER, "Searching WiFi", 0);
		Ui_DispTextLineAlign(3, DISPLAY_CENTER, "Pls wait", 0);   
		ret = WifiOpen();
		OTA_STRLOG("ret:%d-", ret);
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
			Ui_ClearLine(1, 3);
			Ui_Print(3,0,"Connecting %s...", Ap.apName);
			ret = WifiCheck(NULL);
			if(ret) break;
			DelayMs(200);
			if(Ui_GetKey()==KB_CANCEL) goto EXIT;
		}
		
	}

	RouteSetDefault(WIFI_ROUTE);//wifi
	Ota_Process();

EXIT:
	

	return 0;

}
```

```C
int Ota_Process(void)
{
	int ret;
    int route_type = RouteGetDefault();
	char buf[128]={0};
    Ui_Clear();
	sprintf(buf,"OTA TEST WITH %s", (route_type==WNET_ROUTE?"Cellular":"WiFi"));
    Ui_DispTitle(buf);
	Ui_DispTextLineAlign(3, DISPLAY_CENTER, "OTA Requesting...", 0); 	
	//remove(OTA_FILE);
	/*tms_dspread_dl parameter explanation
	The first parameter of  tms_dspread_dl indicates posid.
	the second parameter currently needs to pass in a fixed character "dspread".
	The third parameter indicates the address of the http request.
	The fourth parameter uses "./" by default
	*/
	ret = tms_dspread_dl("50100120230214000044", "dspread","http://www.dspreadserv.net:9997/api/v1/heartbeat", "./");
	memset(buf, 0, sizeof(buf));
	tms_dspread_getfilename_dl(buf, sizeof(buf));
	if(!fexist(buf) && ret == TMS_DSPREAD_ERR_OK)
	{
		DBG_STR("check file ok file_len = %ld", filesize(buf));
		ret = FileToApp(buf);
		DBG_STR("ota result=%d", ret);
	}
}
```

