#ifndef __LITE_M_API_H__
#define __LITE_M_API_H__
#ifndef u32
#define u32 unsigned int
#endif

#ifndef u16
#define u16 unsigned short
#endif

#ifndef u8
#define u8 unsigned char
#endif

#ifndef ulong
typedef unsigned long	ulong;
#endif

/*******************************************************************************************************/
void  Beep(void);
void  BeepF(u8 mode, u16 DlyTime);
u8  SetTime(u8 *time);
void  GetTime(u8 *time);
void DelayMs(u16 Ms);
void  TimerSet(u8 TimerNo, u16 Cnts);
u16 TimerCheck(u8 TimerNo);

void  ReadSN(u8 *SerialNo);

u32  GetTimerCount(void);
u8  ReadVerInfo(u8 *VerInfo);
int GetTermInfo(u8 *out_info);
int Reboot();
void  PCIGetRandom(u8 *random);
int SysSleep(u8 *DownCtrl);
void SysIdle(void);
u8  BatteryCheck(void);
void PowerOff(void);
int GetTermInfoExt (char *keyword,char *attr,u32 attrLen);
u8 GetEnv(char*filename, char *name, u8 *value);
u8 PutEnv(char*filename, char *name, u8 *value);
int SysConfig(u8 *ConfigInfoIn,int InfoInLen);
int SysReset(void);

/*******************************************************************************************************/
#define RSA_KEY_RET_ERR_BIT        -1
#define RSA_KEY_RET_ERR_PUKE    -2
#define RSA_KEY_RET_ERR_DATA    -3
#define RSA_KEY_RET_ERR_RANDOM    -4
#define RSA_KEY_RET_ERR_ENCRY    -5
#define RSA_KEY_RET_ERR_DECRY    -6
#define RSA_KEY_RET_ERR_VERIF    -7

#define ENCRYPT 1        //for des function
#define DECRYPT 0        //for des function
// SHA types
#define SHA_TYPE_1        0
#define SHA_TYPE_224    1
#define SHA_TYPE_256    2
#define SHA_TYPE_384    3
#define SHA_TYPE_512    4

#define MIN_RSA_MODULUS_BITS    508
/* 
    PGP 2.6.2 Now allows 2048-bit keys changing below will allow this.
    It does lengthen key generation slightly if the value is increased.
*/

#define MAX_RSA_MODULUS_BITS    2048

#define MAX_RSA_MODULUS_LEN        ((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS        ((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN        ((MAX_RSA_PRIME_BITS + 7) / 8)

typedef struct {
    unsigned short int bits;                     /* length in bits of modulus */
    unsigned char modulus[MAX_RSA_MODULUS_LEN];  /* modulus */
    unsigned char exponent[MAX_RSA_MODULUS_LEN]; /* public exponent */
} R_RSA_PUBLIC_KEY;

typedef struct {
    unsigned short int bits;                            /* length in bits of modulus */
    unsigned char modulus[MAX_RSA_MODULUS_LEN];            /* modulus */
    unsigned char publicExponent[MAX_RSA_MODULUS_LEN];    /* public exponent */  
    unsigned char exponent[MAX_RSA_MODULUS_LEN];        /* private exponent */
    unsigned char prime[2][MAX_RSA_PRIME_LEN];            /* prime factors */
    unsigned char primeExponent[2][MAX_RSA_PRIME_LEN];    /* exponents for CRT */
    unsigned char coefficient[MAX_RSA_PRIME_LEN];        /* CRT coefficient */
} R_RSA_PRIVATE_KEY;

void des(u8 *input, u8 *output,u8 *deskey, int mode);
void Hash(u8* DataIn, u32 DataInLen, u8* DataOut);
int RSARecover(u8 *pbyModule, u32 dwModuleLen, 
               u8 *pbyExp, u32 dwExpLen, 
               u8 *pbyDataIn, u8 *pbyDataOut);
int RSAKeyPairGen (R_RSA_PUBLIC_KEY *pPublicKeyOut, R_RSA_PRIVATE_KEY *pPrivateKeyOut,
    int iProtoKeyBit, int iPubEType);
int RSAKeyPairVerify (R_RSA_PUBLIC_KEY PublicKey, R_RSA_PRIVATE_KEY PrivateKey);

int SHA (int Mode, const u8 *DataIn, int DataInLen, u8 *ShaOut);
int AES (const u8 *Input, u8 *Output, const u8 *AesKey, int KeyLen, int Mode);
/*******************************************************************************************************/
//Defined for input key values
#define     KEY0                0x30
#define     KEY1                0x31
#define     KEY2                0x32
#define     KEY3                0x33
#define     KEY4                0x34
#define     KEY5                0x35
#define     KEY6                0x36
#define     KEY7                0x37
#define     KEY8                0x38
#define     KEY9                0x39
#define     KEY00               0x3a

//Function keys
 
#define     KEYF1               0x01
#define     KEYF2               0x02
#define     KEYF3               0x03
#define     KEYF4               0x04
#define     KEYUP               0x05
#define     KEYDOWN             0x06
#define     KEYALPHA            0x07
#define     KEYCLEAR            0x08
#define     KEYBACKSPACE        0x09 
#define     KEYF5               0x09
#define     KEYF6               0x0a
#define     KEYENTER            0x0d

#define     KEYMENU             0x14
#define     KEYFN               0x15
#define     KEYSETTLE           0x16
#define     KEYVOID             0x17
#define     KEYREPRINT          0x18
#define     KEYPRNUP            0x19
#define     KEYPRNDOWN          0x1a
#define     KEYCANCEL           0x1b

u8 kbhit(void);
void  kbflush(void);
u8 getkey(void);  
void  kbmute(u8 flag);


/*******************************************************************************************************/
typedef struct __ST_LCDINFO {
    u32 width;
    u32 height;
    u32 ppl;
    u32 ppc;
    u32 fgColor;
    u32 bgColor;
    int reserved[8];
} ST_LCD_INFO;

int CLcdBacklightCtrl(int brightness);
int CLcdGetInfo (ST_LCD_INFO *pstLcdInfo);
int CLcdSetFgColor (u32 color);
int CLcdSetBgColor (u32 color);
int CLcdBgDrawBox (u32 left, u32 top, u32 right, u32 bottom, u32 color);
int CLcdDrawPixel(u32 x, u32 y, u32 color);
int CLcdDrawRect (u32 left, u32 top, u32 right, u32 bottom, u32 color);
int CLcdClrRect (u32 left, u32 top, u32 right, u32 bottom, u32 mode);
int CLcdTextOut (u32 x,u32 y,char *fmt, ...);
int CLcdDrawArea(u32 left,u32 top, u32 width, u32 height, u8* data);
int CLcdDispStatus (int flag);

/*******************************************************************************************************/
void    MagReset(void);
void    MagOpen(void);
void    MagClose(void);
u8   MagSwiped(void);
u8   MagRead(u8 *Track1, u8 *Track2, u8 *Track3);

/*******************************************************************************************************/
typedef struct
{
    u8       Command[4];
    u16      Lc;
    u8       DataIn[512];
    u16      Le;
}APDU_SEND;

typedef struct
{
    u16      LenOut;
    u8       DataOut[512];
    u8       SWA;
    u8       SWB;
}APDU_RESP;

//Smart card reader functions
u8   IccInit(u8 slot,u8 *ATR);
void    IccClose(u8 slot);
u8   IccIsoCommand(u8 slot, APDU_SEND *ApduSend, APDU_RESP *ApduRecv);
u8   IccDetect(u8 slot);

/*******************************************************************************************************/
// PICC函数返回码定义
#define RET_RF_OK                  0x00    // 操作成功
#define RET_RF_ERR_PARAM           0x01    // 参数错误
#define RET_RF_ERR_NO_OPEN         0x02    // 射频卡模块未开启
#define RET_RF_ERR_NOT_ACT         0x03    // 无卡
#define RET_RF_ERR_MULTI_CARD      0x04    // 多卡冲突

#define RET_RF_ERR_TIMEOUT         0x05    // 超时无响应
#define RET_RF_ERR_PROTOCOL        0x06    // 协议错误
#define RET_RF_ERR_TRANSMIT        0x07    // 通信传输错误
#define RET_RF_ERR_AUTH            0x08    // 验证失败
#define RET_RF_ERR_NO_AUTH         0x09    // 扇区未认证
#define RET_RF_ERR_VAL             0x0A    // 数值块数据格式有误
#define RET_RF_ERR_CARD_EXIST      0x0B    // 卡片仍在感应区内
#define RET_RF_ERR_STATUS          0x0C    // 卡片未进入协议态时调用APDU命令,或已进入协议态时调用M1命令// added by liuxl 20070925
//PICC LED显示控制宏

#define PICC_LED_RED    0x01  //红灯
#define PICC_LED_GREEN  0x02//绿灯
#define PICC_LED_YELLOW 0x04 //黄灯
#define PICC_LED_BLUE   0x08//蓝灯
/*******************************************************************************************************/
u8   PiccOpen(void);
//u8   PiccSetup(u8 mode,PICC_PARA *picc_para);

u8   PiccDetect(u8 Mode,u8 *CardType,u8 *SerialNo,u8 *CID,u8 *Other);
u8   PiccIsoCommand(u8 cid,APDU_SEND *ApduSend,APDU_RESP *ApduRecv);
u8   PiccRemove(u8 mode,u8 cid);
void    PiccClose(void);
void    PiccLight(u8 ucLedIndex,u8 ucOnOff);
u8 PiccCmdExchange (u32 uiSendLen, u8* paucInData, u32* puiRecLen, u8* paucOutData);


/*******************************************************************************************************/
#define     PRN_OK              0x00
#define     PRN_BUSY            0x01
#define     PRN_PAPEROUT        0x02
#define     PRN_WRONG_PACKAGE   0x03
#define     PRN_FAULT           0x04
#define     PRN_TOOHEAT         0x08
#define     PRN_UNFINISHED      0xf0
#define     PRN_NOFONTLIB       0xfc
#define     PRN_OUTOFMEMORY     0xfe

u8   PrnInit(void);
u8   PrnStart(u8* DotBitmap, u32 lines);
u8   PrnStatus(void);
void    PrnSetGray(int Level);

/*******************************************************************************************************/
#define     PED_RET_ERR_START	-300
#define     PED_RET_ERR_END	-500
#define     PED_RET_OK	0
#define     PED_RET_ERR_NO_KEY	PED_RET_ERR_START-1
#define     PED_RET_ERR_KEYIDX_ERR	PED_RET_ERR_START-2
#define     PED_RET_ERR_DERIVE_ERR	PED_RET_ERR_START-3
#define     PED_RET_ERR_CHECK_KEY_FAIL	PED_RET_ERR_START-4
#define     PED_RET_ERR_NO_PIN_INPUT	PED_RET_ERR_START-5
#define     PED_RET_ERR_INPUT_CANCEL	PED_RET_ERR_START-6
#define     PED_RET_ERR_WAIT_INTERVAL	PED_RET_ERR_START-7
#define     PED_RET_ERR_CHECK_MODE_ERR	PED_RET_ERR_START-8
#define     PED_RET_ERR_NO_RIGHT_USE	PED_RET_ERR_START-9
#define     PED_RET_ERR_KEY_TYPE_ERR	PED_RET_ERR_START-10
#define     PED_RET_ERR_EXPLEN_ERR	PED_RET_ERR_START-11
#define     PED_RET_ERR_DSTKEY_IDX_ERR	PED_RET_ERR_START-12
#define     PED_RET_ERR_SRCKEY_IDX_ERR	PED_RET_ERR_START-13
#define     PED_RET_ERR_KEY_LEN_ERR	PED_RET_ERR_START-14
#define     PED_RET_ERR_INPUT_TIMEOUT	PED_RET_ERR_START-15
#define     PED_RET_ERR_NO_ICC	PED_RET_ERR_START-16
#define     PED_RET_ERR_ICC_NO_INIT	PED_RET_ERR_START-17
#define     PED_RET_ERR_GROUP_IDX_ERR	PED_RET_ERR_START-18
#define     PED_RET_ERR_PARAM_PTR_NULL	PED_RET_ERR_START-19
#define     PED_RET_ERR_LOCKED	PED_RET_ERR_START-20
#define     PED_RET_ERROR	PED_RET_ERR_START-21
#define     PED_RET_ERR_NOMORE_BUF	PED_RET_ERR_START-22
#define     PED_RET_ERR_NEED_ADMIN	PED_RET_ERR_START-23
#define     PED_RET_ERR_DUKPT_OVERFLOW	PED_RET_ERR_START-24
#define     PED_RET_ERR_KCV_CHECK_FAIL	PED_RET_ERR_START-25
#define     PED_RET_ERR_SRCKEY_TYPE_ERR	PED_RET_ERR_START-26
#define     PED_RET_ERR_UNSPT_CMD	PED_RET_ERR_START-27
#define     PED_RET_ERR_COMM_ERR	PED_RET_ERR_START-28
#define     PED_RET_ERR_NO_UAPUK	PED_RET_ERR_START-29
#define     PED_RET_ERR_ADMIN_ERR	PED_RET_ERR_START-30
#define     PED_RET_ERR_DOWNLOAD_INACTIVE	PED_RET_ERR_START-31
#define     PED_RET_ERR_KCV_ODD_CHECK_FAIL	PED_RET_ERR_START-32
#define     PED_RET_ERR_PED_DATA_RW_FAIL	PED_RET_ERR_START-33
#define     PED_RET_ERR_ICC_CMD_ERR	PED_RET_ERR_START-34
#define     PED_RET_ERR_INPUT_CLEAR	PED_RET_ERR_START-39
#define     PED_RET_ERR_NO_FREE_FLASH	PED_RET_ERR_START-43
#define     PED_RET_ERR_DUKPT_NEED_INC_KSN	PED_RET_ERR_START-44
#define     PED_RET_ERR_KCV_MODE_ERR	PED_RET_ERR_START-45
#define     PED_RET_ERR_DUKPT_NO_KCV	PED_RET_ERR_START-46
#define     PED_RET_ERR_PIN_BYPASS_BYFUNKEY	PED_RET_ERR_START-47
#define     PED_RET_ERR_MAC_ERR	PED_RET_ERR_START-48
#define     PED_RET_ERR_CRC_ERR	PED_RET_ERR_START-49
#define     PED_RET_ERR_PARAM_INVALID	PED_RET_ERR_START-50

#define PED_TLK   0x01  //BKLK
#define PED_TMK   0x02 //TMK
#define PED_TPK   0x03 //PIN Key
#define PED_TAK   0x04 //MAC Key
#define PED_TDK   0x05 //DES Key
#define PED_TIK   0X06 //DUKPT ipek

/*
密钥信息，用于写入密钥信息时传入密钥的相关信息
*/
typedef struct
{
    /* 发散该密钥的源密钥的密钥类型，
    PED_GMK,PED_TMK,PED_TPK,PED_TAK,PED_TDK, 
    不得低于ucDstKeyType所在的密钥级别*/
    u8 ucSrcKeyType;
    /* 发散该密钥的源密钥索引，
    索引一般从1开始，如果该变量为0，
    则表示这个密钥的写入是明文形式*/
    u8 ucSrcKeyIdx;
    /* 目的密钥的密钥类型，
    PED_GMK,PED_TMK,PED_TPK,PED_TAK,PED_TDK 
    */
    u8 ucDstKeyType;
    /*目的密钥索引*/
    u8 ucDstKeyIdx;
    /*目的密钥长度，8,16,24*/
    int iDstKeyLen;
    /*写入密钥的内容*/
    u8 aucDstKeyValue[24];
}ST_KEY_INFO;
/*密钥的kcv校验信息*/
typedef struct
{
  /*
  KCV 模式
  */
  int iCheckMode;
  /*
  kcv数据，根据iCheckMode进行解析
  */
  u8 aucCheckBuf[128];
}ST_KCV_INFO;


//define密钥下载命令码定义
#define PED_DOWNLOAD_CMD_TYPE        0X00
/*读取版本*/
#define     PED_CMD_GETVER            0x00
/*请求系统敏感服务授权并读取随机数*/
#define     PED_CMD_REQ_SYS_SSA    0X01
/*回送随机数密文并获取*/
#define     PED_CMD_GET_TEK        0X02
/*格式化ped*/
#define     PED_CMD_FORMAT_PED        0X03//
/*写入MKSK 密钥*/
#define        PED_CMD_WRITEKEY        0x04//
/*写入TIK*/
#define     PED_CMD_WRITETIK        0x05//
/*设置密钥标签*/
#define     PED_CMD_SETKEYTAG        0X06//
/*PED下载完成*/
#define        PED_CMD_DOWNLOAD_COMPLETE    0X07    


int PedWriteKey(ST_KEY_INFO * KeyInfoIn, ST_KCV_INFO * KcvInfoIn);
int PedWriteTIK(u8 GroupIdx,u8 SrcKeyIdx,u8 KeyLen,u8 * KeyValueIn,
                    u8 * KsnIn, ST_KCV_INFO * KcvInfoIn);
int PedGetPinBlock(u8 KeyIdx, u8 *ExpPinLenIn, u8 * DataIn, 
                        u8 *PinBlockOut, u8 Mode, u32 TimeoutMs);
int PedSetPosition(u32 startX,u32 startY);
int PedGetMac(u8 KeyIdx, u8 *DataIn, u16 DataInLen, u8 *MacOut, u8 Mode);
int PedCalcDES(u8 KeyIdx, u8 * DataIn,u16 DataInLen,u8 * DataOut, u8 Mode);
int PedGetPinDukpt(u8 GroupIdx,u8 *ExpPinLenIn, u8 * DataIn,
        u8* KsnOut,u8 * PinBlockOut, u8 Mode,ulong TimeoutMs);
int PedGetMacDukpt(u8 GroupIdx,u8 *DataIn, 
        u16     DataInLen, u8 *MacOut, u8 * KsnOut, u8 Mode);

int PedVerifyOfflinePin(u32 offline_type, ST_SCPINKEY *RsaKey, u8 *status_word);
//int PedVerifyCipherPin (u8 IccSlot, u8 *ExpPinLenIn, RSA_PINKEY *RsaPinKeyIn, u8 *IccRespOut, u8 Mode,ulong TimeoutMs);
int PedGetKcv(u8 KeyType, u8 KeyIdx,ST_KCV_INFO *KcvInfo);
//int PedWriteKeyVar (u8 KeyType,u8 SrcKeyIdx, u8 DstKeyIdx,u8 * XorVarIn,
//                    ST_KCV_INFO * KcvInfoIn);
int PedGetVer(u8 * VerInfoOut);
int PedErase(void);
//int PedSetIntervaltime(ulong TPKIntervalTimeMs, ulong TAKIntervalTimeMs);
//int PedSetKeyTag(u8 * KeyTagIn);
//int PedSetFunctionKey(u8 ucKey);
int PedGetDukptKSN(u8 GroupIdx, u8 * KsnOut);
//int PedRsaRecover (u8 RSAKeyIndex, u8 *pucDataIn, u8 * pucDataOut, u8* pucKeyInfoOut);
//int PedWriteRsaKey(u8 RSAKeyIndex, ST_RSA_KEY* pstRsakeyIn);
int PedDukptDes(    u8 GroupIdx, u8 KeyVarType, u8* pucIV, u16 DataInLen, 
                    u8 *DataIn, u8 * DataOut, u8*KsnOut , u8 Mode);
int PedGetDukptKSN(u8 GroupIdx, u8 * KsnOut);

int PedDukptIncreaseKsn(u8 GroupIdx);

//int PedReadRsaKey(u8 RSAKeyIndex, ST_RSA_KEY* pstRsakeyOut);
//int PedWriteAesKey(ST_AES_KEY_INFO * AesKeyInfoIn, ST_KCV_INFO * KcvInfoIn);
//int PedCalcAes(unsigned char KeyIdx,unsigned char * InitVector,
//            const unsigned char *DataIn,unsigned short DataInLen, unsigned char *DataOut, unsigned char Mode);
//int PedWriteKeyEncByRsa (const u8 *DataIn,u16 DataInLen,u8 DataInFormat,
//                            u8 PrvKeyIdx,u8 DstKeyType,u8 DstKeyIdx,const u8 *KsnIn,ST_KCV_INFO * KcvInfoIn);
//int PedSetOfflinePinMode(u8 Mode,u8 TpkIdx,u8 *PinBlock,u16 PinBlockLen);
/*******************************************************************************************************/
#define     FILE_EXIST          1
#define     FILE_NOEXIST        2
#define     MEM_OVERFLOW        3
#define     TOO_MANY_FILES      4
#define     INVALID_HANDLE      5
#define     INVALID_MODE        6
#define     NO_FILESYS          7
#define     FILE_NOT_OPENED     8
#define     FILE_OPENED         9
#define     END_OVERFLOW        10
#define     TOP_OVERFLOW        11
#define     NO_PERMISSION       12
#define     FS_CORRUPT          13

#define     O_RDWR              0x01
#define     O_CREATE            0x02
#define     O_ENCRYPT           0x04

#define     SEEK_CUR            0
#define     SEEK_SET            1
#define     SEEK_END            2

#define FTO_RET_OK                    0
#define FTO_RET_APPINFO_ERR            -1
#define FTO_RET_FILE_NO_EXIST        -2
#define FTO_RET_SIG_ERR                -3
#define FTO_RET_TOOMANY_APP            -4
#define FTO_RET_NAME_DEUPT            -5
#define FTO_RET_APP_TYPE_ERR        -6
#define FTO_RET_WRITE_FILE_ERR        -7
#define FTO_RET_READ_FILE_ERR        -8
#define FTO_RET_WITHOUT_APP_NAME    -9
#define FTO_RET_TOOMANY_FILE        -10
#define FTO_RET_NO_APP                -11
#define FTO_RET_PARAM_ERR            -12
#define FTO_RET_FONT_ERR            -13
#define FTO_RET_FILE_TOOBIG            -14
#define FTO_RET_NO_SPACE            -15

typedef struct
{
    u8       fid;
    u8       attr;
    u8       type;
    char        name[17];
    ulong       length;
} FILE_INFO;

int open (char *filename, u8 mode);
int read (int fid, u8 *dat, int len);
int write(int fid, u8 *dat, int len);
int close (int fid);
int seek (int fid, long offset, u8 fromwhere);
long filesize (char *filename);
long freesize (void);
int truncate (int fid, long len);
int fexist (char *filename);
int ex_open (char *filename, u8 mode, u8 *attr);
int remove (const char *filename);
int GetFileInfo (FILE_INFO* finfo);
int FileToApp(u8 *FileName);


int     GetLastError(void);

/*******************************************************************************************************/
//11 USBDEV
#define USB_DEV (11)
unsigned char PortOpen(unsigned char channel, unsigned char *para);
unsigned char PortClose(unsigned char channel);
unsigned char PortSend(unsigned char channel, unsigned char ch);
unsigned char PortRecv(unsigned char channel, unsigned char *ch, unsigned short ms);
unsigned char PortReset(unsigned char channel);
unsigned char PortSends(unsigned char channel,unsigned char *str,unsigned short str_len);
unsigned char PortTxPoolCheck(unsigned char channel);
int PortRecvs(u8 channel,     u8 * pszBuf, u16 usBufLen, u16 usTimeoutMs);

/*******************************************************************************************************/
#define NET_OK        0      /* No error, everything OK. */
#define NET_ERR_MEM  -1      /* Out of memory error.     */
#define NET_ERR_BUF  -2      /* Buffer error.            */

#define NET_ERR_ABRT -3      /* Connection aborted.      */
#define NET_ERR_RST  -4      /* Connection reset.        */
#define NET_ERR_CLSD -5      /* Connection closed.       */
#define NET_ERR_CONN -6      /* Not connected.           */

#define NET_ERR_VAL  -7      /* Illegal value.           */

#define NET_ERR_ARG  -8      /* Illegal argument.        */

#define NET_ERR_RTE  -9      /* Routing problem.         */

#define NET_ERR_USE  -10     /* Address in use.          */

#define NET_ERR_IF   -11     /* Low-level netif error    */
#define NET_ERR_ISCONN -12   /* Already connected.       */

#define NET_ERR_TIMEOUT -13  /* time out */

#define NET_ERR_AGAIN  -14   /*no block, try again */
#define NET_ERR_EXIST  -15   /* exist already */
#define NET_ERR_SYS    -16   /* sys don not support */

/* PPP ERROR code */
#define NET_ERR_PASSWD -17 /* error password */
#define NET_ERR_MODEM  -18 /* modem dial is fail */
#define NET_ERR_LINKDOWN -19
#define NET_ERR_LINKDOWN -19/* DataLink Down */
#define NET_ERR_LOGOUT   -20/* User Logout */
#define NET_ERR_PPP      -21/* PPP Link Down*/

#define NET_ERR_STR     -22 /* String Too Long, Illeage */
#define NET_ERR_DNS     -23 /* DNS Failure: No such Name */
#define NET_ERR_INIT    -24 /* No Init */

#define NET_AF_INET 1

enum
{
         NET_SOCK_STREAM=1, /* TCP */
         NET_SOCK_DGRAM                  /* UDP */
};
#define MAX_IPV4_ADDR_STR_LEN            15
struct net_in_addr 
{
    unsigned long s_addr;
};

struct net_sockaddr_in 
{
         char  sin_len;
         char  sin_family;
         short sin_port;
         struct net_in_addr sin_addr;
         char sin_zero[8];
         char host[256];
};
struct net_sockaddr
{
         char sa_len;
         char sa_family;
         char sa_data[14];
         char host[256];
};
#ifndef socklen_t
#define socklen_t int
#endif
int NetSocket(int domain, int type, int protocol);
int NetConnect(int socket, struct net_sockaddr *addr, socklen_t addrlen);
int NetSend(int socket, void *buf/* IN */, int size, int flags);
int NetRecv(int socket, void *buf/* OUT */, int size, int flags);
int NetCloseSocket(int socket);


#define NOBLOCK 1
#define BLOCK   0/* 阻塞标志位 */
#define SOCK_EVENT_READ    (1<<0)
#define SOCK_EVENT_WRITE   (1<<1)
#define SOCK_EVENT_CONN    (1<<2)
#define SOCK_EVENT_ACCEPT  (1<<3)
//#define SOCK_EVENT_ERROR   (1<<4)
#define SOCK_EVENT_MASK    (0xff)

enum
{
    CMD_IO_SET=1,
    CMD_IO_GET,
    CMD_TO_SET,/* timeout set */
    CMD_TO_GET,/* timeout get */
    CMD_IF_SET,/* net_socket bind dev IF (interface)*/
    CMD_IF_GET,/* get the dev if net_socket bind */
    CMD_EVENT_GET,/* get event , such as SOCK_EVENT_READ,SOCK_EVENT_WRITE, etc*/
    CMD_BUF_GET,/* get send or recv buffer , only for SOCK_STREAM */
    CMD_FD_GET,/* get free net_socket fd num */
};

/**
    cmd             arg
CMD_EVENT_GET       0
**/
int Netioctl(int socket, int cmd, int arg);

int SockAddrSet(struct net_sockaddr *addr/* OUT */, char *ip_str/* IN */, short port/* IN */);
int SockAddrGet(struct net_sockaddr *addr/* IN */, char *ip_str/* OUT */, short *port/* OUT */);


//Route API
#define ETH_ROUTE  0
#define PPP_ROUTE  10
#define WNET_ROUTE  11
#define WIFI_ROUTE  12

int RouteSetDefault(int ifIndex);
int RouteGetDefault(void);
/*******************************************************************************************************/
//模块口被占用
#define WL_RET_ERR_PORTINUSE     -201
//模块没有应答
#define WL_RET_ERR_NORSP        -202
//模块应答错误
#define WL_RET_ERR_RSPERR        -203
//模块串口没有打开
#define WL_RET_ERR_PORTNOOPEN -204
//需要PIN码
#define WL_RET_ERR_NEEDPIN    -205
//需要PUK解PIN码
#define WL_RET_ERR_NEEDPUK    -206
//参数错误
#define WL_RET_ERR_PARAMER    -207
//密码错误
#define WL_RET_ERR_ERRPIN        -208
//没有插入SIM卡
#define    WL_RET_ERR_NOSIM      -209
//不支持的类型
#define WL_RET_ERR_NOTYPE        -210
//网络没有注册
#define WL_RET_ERR_NOREG        -211
//模块曾初始化
#define WL_RET_ERR_INIT_ONCE    -212
//没有连接
#define WL_RET_ERR_NOCONNECT  -213
//线路断开
//#define WL_RET_ERR_LINEOFF    -214
//没有socket可用
#define WL_RET_ERR_NOSOCKETUSE    -215
//超时
#define WL_RET_ERR_TIMEOUT        -216
//正在拨号中
#define WL_RET_ERR_CALLING         -217
//重复的socket请求
#define WL_RET_ERR_REPEAT_SOCKET    -218
//socket 已经断开
#define WL_RET_ERR_SOCKET_DROP    -219
//socket 正在连接中
#define WL_RET_ERR_CONNECTING     -220
//socket 正在关闭
#define WL_RET_ERR_SOCKET_CLOSING    -221
//网络注册中
#define WL_RET_ERR_REGING            -222
//关闭串口出错
#define WL_RET_ERR_PORTCLOSE      -223
//发送失败
#define WL_RET_ERR_SENDFAIL        -224
//错误的模块版本
#define WL_RET_ERR_MODEVER        -225
//拨号中
#define WL_RET_ERR_DIALING        -226
//挂机中
#define WL_RET_ERR_ONHOOK        -227
//发现模块复位
#define WL_RET_ERR_PPP_BRK         -228

#define WL_RET_ERR_OTHER            -300

typedef struct
{
    u32  SimCardSel;
    char SimPin[16];
    char AccessPointName[64];
    char UserName[16];
    char UserKey[16];
    char DialNumber[16];
    char ServerDomain[64];
}ST_WLAN_CFG;

int WlInit(ST_WLAN_CFG cfg);
int WlGetSignal(u8 * SingnalLevel);
void WlPppLogout (void);

/*******************************************************************************************************/


#define WIFI_RET_OK                  (0  )     /* 无错误,正常                          */
#define WIFI_RET_ERROR_NODEV         (-1 )     /* 设备错误                             */
#define WIFI_RET_ERROR_NORESPONSE    (-2 )     /* WIFI模块无响应                       */
#define WIFI_RET_ERROR_NOTOPEN       (-3 )     /* WIFI模块未打开                       */
#define WIFI_RET_ERROR_NOTCONN       (-4 )     /* WIFI模块未连接AP                     */
#define WIFI_RET_ERROR_NULL          (-5 )     /* 参数为空                             */
#define WIFI_RET_ERROR_PARAMERROR    (-6 )     /* 参数错误                             */
#define WIFI_RET_ERROR_STATUSERROR   (-7 )     /* WIFI模块当前状态不支持接口的操作     */
#define WIFI_RET_ERROR_AUTHERROR     (-9 )     /* 密码认证错误                         */
#define WIFI_RET_ERROR_NOAP          (-10)     /* 扫描AP异常                           */
#define WIFI_RET_ERROR_IPCONF        (-11)     /* 设置或获取IP失败                     */
#define WIFI_RET_ERROR_NOTSUPPORT    (-13)     /* WIFI模块不支持这一特性               */
#define IPLEN                        (4  )     /* IP长度                               */
#define KEY_WEP_LEN_MAX              (16 )     /* WEP密钥最大长度                      */
#define KEY_WEP_LEN_64               (5  )     /*                                      */
#define KEY_WEP_LEN_128              (13 )     /*                                      */
#define KEY_WEP_LEN_152              (16 )     /*                                      */
#define KEY_WPA_MAXLEN               (63 )     /* WPA密钥最大长度                      */
#define SSID_MAXLEN                  (32 )     /* SSID最大长度                         */
#define SCAN_AP_MAX                  (15 )     /* 扫描AP最大个数                       */
#define WLAN_SEC_UNSEC               (0  )     /* 无加密                               */
#define WLAN_SEC_WEP                 (1  )     /* WEP加密                              */
#define WLAN_SEC_WPA_WPA2            (2  )     /* WPA/WPA2加密                         */
#define WLAN_SEC_WPAPSK_WPA2PSK      (3  )     /* WPA-PSK/WPA2-PSK加密                 */
#define WLAN_SEC_WPAPSK              (4  )     /* WPA-PSK加密                          */
#define WLAN_SEC_WPA2PSK             (5  )     /* WPA2-PSK加密                         */
#define WIFI_ROUTE_NUM               (12 )     /* WIFI的路由编号                       */

typedef struct {
    u8 apName[50+1];        // WIFI热点名称
    u8 apSignal[5+1];        // WIFI信号强度
    u8 apMode[32+1];        // WIFI热点密钥方式
}ST_WIFIAP;

typedef struct { 
    u8 Key[4][KEY_WEP_LEN_MAX]; /* WEP 密码数据 */ 
    int KeyLen; /* WEP 密码长度 5 or 13 or 16*/ 
    int Idx; /* WEP密钥索引[0..3] */ 
} ST_WEP_KEY;
typedef struct { 
    int DhcpEnable; /* DHCP使能，0-关闭，1-开启 */ 
    u8 Ip[IPLEN]; /* 静态IP*/ 
    u8 Mask[IPLEN]; /* 掩码 */ 
    u8 Gate[IPLEN]; /* 网关 */
    u8 Dns[IPLEN]; /* DNS */ 
    u8 psw[32]; /* wep密码, string ,"012345" */ 
    u8 Wpa[KEY_WPA_MAXLEN]; /* WPA连接密码*/ 
}ST_WIFI_PARAM;

int WifiOpen(void);
int WifiClose(void);
int WifiScan (ST_WIFIAP *outAps,unsigned int ApCount);
int WifiConnect(ST_WIFIAP *Ap, ST_WIFI_PARAM *WifiParam);
int WifiDisconnect(void);
int WifiCheck(ST_WIFIAP *Ap);


/*******************************************************************************************************/
int SoundPlay(char *param, char type);
/*******************************************************************************************************/
/*******************************************************************************************************/
void LiteMapiInit();

typedef struct 
{
    int width;
    int high;
    const unsigned char *data;
}IMG_INFO_T;

//extern void uart_debug(const char *str, ...);
//extern int uart_hex_dump(void* buf, u32 len);
//#define DBG_STR(fmt, args...) uart_debug("%s:%d "fmt"\r\n", __FUNCTION__, __LINE__, ##args);
//#define DBG_HEX(title, data, len) {DBG_STR(title); uart_hex_dump(data, len);}
extern void usb_debug(const char *str, ...);
void Utils_PrintHex(unsigned char *data, u32 len);
#define DBG_STR(fmt, args...) {usb_debug("%s:%d "fmt"\r\n", __FUNCTION__, __LINE__, ##args); }
#define DBG_HEX(title, data, len) {usb_debug(title); Utils_PrintHex(data, len);}

#endif
