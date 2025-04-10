
#include "app.h"
#include "ui.h"
#include "utils.h"
#include "tms_dspread.h"


#ifdef OTA_DEBUG
#define OTA_STRLOG DBG_STR
#define OTA_HEXLOG DBG_HEX
#else
#define OTA_STRLOG  
#define OTA_HEXLOG 
#endif

#define OTA_FILE ("ota.asc")

extern int tms_dspread_dl(const char* id, const char* custcode, const char* heartbeatpath, const char* dir);
extern void tms_dspread_getfilename_dl(char* filename, int filenamemaxLen);
extern int set_common_type(void *pxPARAM);
extern int setcomm_wlan(void *pxPARAM);
extern int WlanGetSign(void);

#if 0
extern int LvoslitEnable(char *key);
extern int LvoslitWriteFlash(u32, u8 *data, u32 len);

typedef enum {
  UPDATA_APP,
  UPDATE_OS,
  UPDATE_FONT,
}TMS_UPDATE_TYPE_e;

#define OS_UPDATE_ADDR 0x1000000
#define OS_MAX_SIZE (170 * 1024)


int TMS_Write_File(u32 start_addr, u32 MaxLen, u8* FileName)
{
    int fd, i_file_size=0, remain_size=0, has_read_len = 0;
    int iRet, offset=0;
	u8 *name = FileName, *pReadBuf=NULL;
	int readMaxLen = 1024*16, readLen = 0;


    if(name == NULL || (i_file_size = filesize(name))==0){
		OTA_STRLOG("ERR\r\n");
        return -1;
    }	

	if(i_file_size > MaxLen)
	{
		OTA_STRLOG("ERR\r\n");
		return -1;
	}

	if((pReadBuf = malloc(readMaxLen)) == NULL)
	{
		 OTA_STRLOG("cannot apply for memory\r\n");
		 return -1;
	}

    fd = open(name,O_CREATE|O_RDWR);
    if(fd < 0){
        OTA_STRLOG("open(%s):%d\r\n", name, fd);
        return -1;
    }


	remain_size = i_file_size;	
	LvoslitEnable("lmtascrdenbal"); 	
	while(1)
	{   
		readLen = (remain_size>readMaxLen?readMaxLen:remain_size);

		iRet = seek(fd, has_read_len, SEEK_SET);
		if(iRet < 0) {
			iRet = -1;
			break;
		}		

		iRet = read(fd, (byte*)pReadBuf, readLen);
		
		if(iRet != readLen){
			OTA_STRLOG("read file iRet:%d\r\n", iRet);
			iRet = -1;
			break;
		}		

		iRet = LvoslitWriteFlash(start_addr + has_read_len, pReadBuf, readLen);
		if (iRet < 0) {
			OTA_STRLOG("WriteFlash iRet:%d\r\n", iRet);
			iRet = -1;
			break;
		}

		has_read_len += readLen;
		if (has_read_len == i_file_size) {
			iRet = 0;
			break;
		}

		remain_size -= readLen;
	}

	close(fd);

	free(pReadBuf);

	if(!iRet)	{remove(FileName);Reboot();}

    return iRet;
}

int FileToUpdate(TMS_UPDATE_TYPE_e type, u8 *FileName)
{
	switch (type)
	{
	case UPDATA_APP:
		return FileToApp(FileName);
	case UPDATE_OS:
		return TMS_Write_File(OS_UPDATE_ADDR, OS_MAX_SIZE, FileName);	
	default:
		break;
	}
}
#endif
int Ota_Process(void)
{
	int ret;
    int route_type = 0;
	char buf[128] = { 0 };
	static int wlan_set_flag = 0;

	if (WifiCheck(NULL) <= 0)
    {
        if (set_common_type(NULL) < 0)
            return -1;
	}

	route_type = RouteGetDefault();

	Ui_Clear();
	sprintf(buf, "OTA TEST WITH %s", (route_type == WNET_ROUTE ? "Cellular" : "WiFi"));
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

int ota_process_cellular2(void * pxPARAM)
{
	char *ota_name = OTA_FILE;
	#if 0
	
	int retv = -1;
	long file_len = 0;
	//testPngFileLoad();

	DBG_STR("check file");
	if(!fexist(ota_name))
	{
		file_len = filesize(ota_name);
		DBG_STR("check file ok file_len = %d", file_len);
		retv = FileToUpdate(UPDATE_OS, (u8*)ota_name);
		DBG_STR("ota result=%d", retv);
	}
	#endif

	remove(ota_name);
	
	return 0;

}

/*
int ota_process_cellular3(void * pxPARAM)
{
	Reboot();
	return 0;

}
*/
int ota_demo(void * pxPARAM)
{
	return Ota_Process();

}