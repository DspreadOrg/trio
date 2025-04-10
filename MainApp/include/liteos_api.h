#ifndef __LITEOS_API_
#define __LITEOS_API_

#ifndef u32
#define u32 unsigned int
#endif

#ifndef u16
#define u16 unsigned short
#endif

#ifndef u8
#define u8 unsigned char
#endif

#ifndef NULL
#define NULL    ((void *)0)
#endif

#undef  SEEK_CUR
#undef  SEEK_SET
#undef  SEEK_END
#define SEEK_CUR        0
#define SEEK_SET        1
#define SEEK_END        2

#define LVOSAPI_ERROR_INDEX (-99999999)

typedef struct {
	u32 starttime;
	u32 timeout;	
}ST_Timer_UNIT;

typedef struct
{
    u16 year;
    u16 mon;
    u16 day;
    u16 hour;
    u16 min;
    u16 sec;
    u16 wday;
}SYS_TIME;

typedef struct{
    u8      Command[4];
    u16     Lc;
    u8      indata[256];
    u16     Le;
}C_APDU;

typedef struct{
    u16     outlen;
    u8      outdata[256];
    u8      SWA;
    u8      SWB;
}R_APDU; 

typedef  struct{
    u32     modlen;
    u8      mod[256];
    u8      exponent[4];
    u8      randlen;
    u8      random[8];
}ST_SCPINKEY; 

int dbg_putchar(u8 c);
int usb_tx(u8 c);
int udelay(u32 us);
int mdelay(unsigned int ms);
/**MSR**/
int LvosMsrRead(u32 hWnd,u8 *Track1,u32 *T1len,char *Track2,u32 *T2len,char *Track3,u32 *T3len);

/**Smart Card Reader**/
int LvosScPoll(u32 hWnd);
int LvosScActive(u32 hWnd, u8 *rsp);
int LvosScExchange(u32 hWnd, const C_APDU *c_apdu, R_APDU *r_apdu);
int LvosScRemoval(u32 hWnd);
int LvosScPinVerify(u32 hWnd, ST_SCPINKEY  *key, u16 * SWAB);

/**PED**/
int LvosStoreKey(u32 hWnd,u32 key_type,void* key,void *kcv);
int LvosDataCalc(u32 hWnd,u32 key_type,void* key,u8 *datain,u32 datainlen,u8* dataout,u32 *dataoutlen);
int LvosMacCalc(u32 hWnd,u32 key_type,void* key,u8 *datain,u32 datainlen,u8* dataout,u32 *dataoutlen);
int LvosGetPinBlock(u32 hWnd,u32 key_type,void* key,void *pinblock);
int LvosSecCtrl(u32 hWnd,u32 key_type,void* key,u8 *datain,u32 datainlen,u8* dataout,u32 *dataoutlen);
int SecOfflinePinVerify(u32 offline_type, ST_SCPINKEY *RsaKey, u8 *status_word);/**kernel use**/

/**Filesystem**/
int LvosFopen(const char *filename);
int LvosFread(int fd, u8 *data, u32 len);
int LvosFwrite(int fd, u8 *data,u32 len);
int LvosFclose(int fd);
int LvosFseek(int fd, int offset,u32 fromwhere);
int LvosFsize(const char *filename);
int LvosFremove(const char *filename);
int LvosFFreeSize(void);

/**Communication**/
int LvosCommRequest(u32 hWnd);
int LvosCommConnect(u32 hWnd);
int LvosCommSend(u32 hWnd,u8*src,u32 len);
int LvosCommRecv(u32 hWnd,u8*dst,u32 len);
int LvosCommDisconnect(u32 hWnd);
int LvosCommRelease(u32 hWnd,u32 force);
int LvosCommClear(u32 hWnd,u32 flags);
int LvosCommCheck(u32 hWnd,u32 *Status);

/**UI**/
int LvosUiSetAttr(u32 hWnd, u32 type, u32 value);
int LvosUiSetArea(u32 hWnd,u32 hpixel,u32 vpixel, u32 width, u32 height);
int LvosUiPrintf(u32 hWnd,const char * fmt,...);
int LvosUiScanf(u32 hWnd);
int LvosUiDrawArea(u32 hWnd, u32 hpixel,u32 vpixel, u32 width, u32 height, u8* data);

/**System**/
int LvosSysCfgRead(char *keyword,char *attr,u32 attrLen);
int LvosSysGetTimer(ST_Timer_UNIT *timer,u32 timeout);
int LvosSysTimerLeft(ST_Timer_UNIT *timer);
void LvosSysDelayMs(u32 ms);
int LvosSysGetTime(SYS_TIME * time);
int LvosSysSetTime(SYS_TIME * time);
int LvosSysPower(u32 mode);
int LvosSysSoundPlay(char mode,char *param);
int LvosSysRandom(u8 *rand,u32 len);
u32 LvosSysTick();

/**User Paramters**/
int LvosParamEnum(u32 hWnd,u32 index, char *keyword,char *value,u32 valLen);
int LvosParamRead(u32 hWnd,const char *keyword,u8 *value,u32 vallen);
int LvosParamWrite(u32 hWnd,const char *keyword,u8 *value);
int LvosSysUpdate(char type, char *filename);

/**Alg**/
int LvosDes(u8 *indata, u8 *outdata, u8 *key, u32 mode);
int LvosAes(u8 *indata, u8 *outdata, u8 *key, u32 keylen, u32 mode);
int LvosHash(u8* DataIn, u32 DataInLen, u8* DataOut);
int LvosSha(u32 type,u8 *HashOut, u32 DataLen, u8 *DataIn);
int LvosRsaRecover(u8 *mod, u32 modlen,u8 *exp, u32 explen,u8 *indata, u8 *outdata);

#endif

