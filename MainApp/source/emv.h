#ifndef __DEV_H__
#define __DEV_H__

#include <EmvL2.h>

#define UP_PIN_FREE	 	

#ifdef	UP_PIN_FREE

#define	UP_DEBITAID			"A000000333010101"
#define	UP_CREDITAID		"A000000333010102"
#define	UP_QUASICREDITAID	"A000000333010103"
#define	UP_EC				"A000000333010106"

#define	ONLINEPIN_CVM		0x80
#define	SIGNATURE_CVM		0x40
#define	CD_CVM				0x80
#define	NO_CVM				0x00

enum{
	UP_DEBITAPP,
	UP_CREDITAPP,
	UP_QUASICREDITAPP,
	UP_ECAPP,
	UP_UNKNOWAPP
};

enum{ 
	ONLINEPIN_REQUIR ,
	SIGNATURE_REQUIR,
	ONLINEPIN_FREE,
	ONLINEPIN_UNKNOW
};

#endif

#define EMV_CASH          0x01       
#define EMV_GOODS	      0x02       
#define EMV_SERVICE       0x04       
#define EMV_CASHBACK      0x08       

#define BLACK_FILE		"EMVBLACK"   
#define AID_PARAM_FILE 	"EMVAID"	 
#define CAPK_PARAM_FILE	"EMVCAPK"	 
#define DEFAULT_FILE    "EMVDEFAULT" 
#define AIDEXT_PARAM_FILE 	"EMVAIDEXT"	 

#define EMV_RET_OK          0
#define EMV_RET_BASE        -2000
#define EMV_RET_FAIL        (EMV_RET_BASE-0)
#define EMV_RET_PARAM       (EMV_RET_BASE-1)
#define EMV_RET_ERR_AID     (EMV_RET_BASE-2)
#define EMV_RET_ERR_DEFAUT  (EMV_RET_BASE-3)
#define EMV_RET_ERR_CAPK    (EMV_RET_BASE-4)
#define EMV_RET_FALLBACK    (EMV_RET_BASE-5)
#define EMV_RET_ONLINE_APPROVE (EMV_RET_BASE-6)
#define EMV_RET_ONLINE_DENIAL  (EMV_RET_BASE-7)
#define EMV_RET_USER_ABORT     (EMV_RET_BASE-8)
#define EMV_RET_ONLINE_ABORT   (EMV_RET_BASE-9)
#define EMV_NO_FUNC   -4000
// for emvcompletion
#ifndef EMV_ONLINE_OUTCOME
typedef enum {
	ONLINE_APPROVE,
	ONLINE_DENIAL,	
	ONLINE_FAILED,
	ONLINE_DENIAL_05
}EMV_ONLINE_OUTCOME;
#endif

#if 0
typedef enum 
{
 STEP_PREPROC=1, 
 STEP_APPBUILDLIST ,  
 STEP_FINALSEL, 
 STEP_INITAPP,
 STEP_READAPP,
 STEP_DATAAUTH,
 STEP_PROCRESTRIC, 
 STEP_TERMRISK, 
 STEP_CVMPROC, 
 STEP_TAA, 
 STEP_COMPLETION,
 STEP_PAYPASS_WHOLE,

}EMV_STEP;
#endif

typedef enum {
  EMV_RS_SUCCESS = 0,      // Success
  EMV_RS_APPROVE,          // Offline Approve
  EMV_RS_DECLIENED,        // Offline Declined
  EMV_RS_ONLINE_APPROVE,   // Online Approve
  EMV_RS_ONLINE_DECLIENED, // Online Declined
  EMV_RS_ABORT,            // Abort by Cardholder
  EMV_RS_TRY_AGAIN,        // Try Again
  EMV_RS_OTHER_INTERFACE,  // Use Other Interface
  EMV_RS_OTHER_CARD,       // Use Other Card
  EMV_RS_ONLINE_REQUEST,   // Online Request
  EMV_RS_ERROR,            // Error
}E_EMVRESULT;

/*AID*/
typedef struct {
	// other
	byte OnlinePin[1]; //Terminal online PIN support capability DF18
	// to ST_AIDLIST
	byte AIDLen[1];  
	byte AID[16];	  // 9F06
	byte SelFlag[1]; //(PART_MATCH 0;  FULL_MATCH  1) DF01
	// to ST_AIDPARAM
	byte TargetPer[1]; 	  //Target percentage DF17
	byte MaxTargetPer[1];	  //Maximum target percentage DF16
	byte FloorLimitCheck[1]; //Whether to check the minimum
	byte FloorLimit[4];	  //Minimum, big-end storage 9F1B
	byte Threshold[4]; 	  //threshold value DF15
	byte TACDenial[5]; 	  //Terminal behavior code (reject) DF13
	byte TACOnline[5]; 	  //Terminal behavior code (online) DF12
	byte TACDefault[5];	  //Terminal behavior code (default) DF11
	byte dDOLLen[1];		  //default DDOL length
	byte dDOL[32]; 		  //default DDOL DF14
	byte Version[2];		  //app ver 9F09
	// TO ST_PREPARAM
	byte IsExistCVMLmt[1]; 				 // Cardholder limit
	byte CVMLmt[6];						 // big-end store DF21
	byte IsExistTermClssLmt[1];			 // Terminal non-transaction limit
	byte TermClssLmt[6];					 // big-end store DF20
	byte IsExistTermClssOfflineFloorLmt[1]; // Terminal off-line minimum
	byte TermClssOfflineFloorLmt[6];		 // DF19
	byte IsExistTermOfflineFloorLmt[1];	 // The terminal off-line minimum
	byte TermOfflineFloorLmt[6];			 // 9F7B same as ECTTLVal
}ST_AID;

/**
 * Terminal default parameter structure
 * 该结构体与应用规范相关
 **/
typedef struct
{
  unsigned char AIDLen[1];  //AID的长度
  unsigned char AID[16];    //应用标志 9F06
  unsigned char SelFlag[1]; //选择标志(PART_MATCH 部分匹配 0;  FULL_MATCH 全匹配 1) DF01
  unsigned char Tagslen[1];
  unsigned char DynamicTags[237];    //拓展taglist DF31
} ST_AID_EXT;

/**
 * Terminal default parameter structure
 **/
typedef struct
{
  byte Version[4];       // param ver，default 0
  byte MerchCateCode[2]; // Merchant class code (not required) 9F15
  byte MerchId[16];      // Merchant logo (merchant number) 9F16
  byte MerchName[128];   // Merchant name 9F4E
  byte CountryCode[2];   // Terminal country code 9F1A
  byte TerminalType[1];  // Terminal type 9F35
  byte TermId[8];        // Terminal mark (POS number) 9F1C
  byte Capability[3];    // Terminal performance 9F33
  byte AddCapability[5]; // Terminal expansion performance 9F40
  byte RMDLen[1];        // Risk management data length
  byte RiskManData[8];   // Risk management data 9F1D
  byte tDOLLen[1];       // default TDOL length
  byte tDOL[32];         // default TDOL 97
  byte TermAIP[2];       // terminal AIP 82
  byte IFDSn[8];         // IFD serial no 9F1E
  byte AcquierId[6];     // Receiving line sign 9F01
  byte TransCurrCode[2]; // Transaction currency code 5F2A
  byte TransCurrExp[1];  //  5F36
  byte ReferCurrCode[2]; // 9F3C
  byte ReferCurrExp[1];  // 9F3D
  byte ReferCurrCon[4];  // 9F3A (The exchange rate of the trading currency against the reference currency*1000)
  byte TTQ[4];           // Terminal transaction attributes (ctls used) 9F66
  byte ECTTLVal[6];		 // Electronic cash terminal transaction limit 9F7B
  byte Category[2];      // Transaction class code 9F53
  byte TSN[4];           // Transaction serial number HEX TAG_9F41_TS_SEQ_COUNTER
} ST_DEFAULT;

/*Transaction dynamic parameter*/
typedef struct
{
  byte Type[1];            // transaction type TAG_9C_TS_TYPE
  byte Category[2];        // Transaction class code 9F53
  byte TSN[4];             // Transaction serial number HEX TAG_9F41_TS_SEQ_COUNTER
  byte Date[3];            // The transaction date can be left unset，BCD YYHHMM TAG_9A_TS_DATE
  byte Time[3];            // Trading hours can be left unset，BCD HHMMSS TAG_9F21_TS_TIME
  byte Amt[6];             // transaction amount BCD TAG_9F02_AMT_N
  byte OtherAmt[6];        // other amount 9F03
  byte TTQ[4];             // Terminal Transaction Qualifiers 9F66
  byte TransCurrCode[2];   // Transaction currency code 5F2A
  byte TransCurrExp[1];    // 5F36
  byte ReferCurrCode[2];   // TAG_9F3C_TS_REF_CURR_CODE,
  byte ReferCurrExp[1];    // TAG_9F3D_TS_REF_CURR_EXP,
  byte ReferCurrCon[4];    // 9F3A 
  byte IsReadLogInCard[1]; // Whether to view the card log FF900B
  //other
  byte ForceOnline[1];  // Force online or not FF9005
  byte TrackEncrypt[1]; // Whether the track is encrypted FF9008
  byte Reversal[10];    // reserve
} ST_TRANS;

/*Public key parameter query conditions*/
typedef struct
{
  byte RID[5];
  byte Expire[4];
  byte Index;
} ST_CAPK_COND;

/*aid parameter query conditions*/
typedef struct
{
  byte AIDLen;  
  byte AID[16]; 
} ST_AID_COND;

void EMV_SetDebug(int isDebug);
int EMV_GetDebug();
	
void EMV_SetIsInputPIN(int isInput);

int EMV_GetCurKernelType();

int EMV_ClearAID();
int EMV_AddAID(ST_AID * pxAid);
	
int EMV_ClearCAPK();
int EMV_AddCAPK(ST_CAPK * pxCAPK);
	
int EMV_SetDefault(ST_DEFAULT * pxDEF);

int EMV_SetKernalData(ushort usTag, byte *val, int valLen);

int EMV_GetKernalData(ushort usTag, byte * val, uint *len);

int EMV_GetKernalTLVs(const ushort * tagList, uint num, byte * data, uint * dataLen);

int EMV_GetCardData(ushort usTag, byte *val, int *valLen);

int EMV_ClearAIDEXT();
int EMV_AddAIDEXT(ST_AID_EXT* pxAID);

/*
How to obtain the CVM
ONLINEPIN_FREE 
ONLINEPIN_REQUIR 
SIGNATURE_REQUIR
ONLINEPIN_UNKNOW
*/
int EMV_CheckCVMType();


int EMV_GetCurKernelType();

int EMV_InitCore();

/**
Called when the parameter has changed
**/
int EMV_LoadParam2Kernel();

/**
Transaction initialization, called before card reading
**/
int EMV_InitTransaction(int transType);

/**
 Transaction is not processed, called before reading the card
**/
int EMV_CLSSPreProcess();

int EMV_EMVProcess_Simple();

int EMV_EMVProcess();

int EMV_CLSSProcess_Simple();

int EMV_CLSSProcess();

int EMV_GetAID(ST_AID_COND * condition, ST_AID * pxAid);
int EMV_GetAIDEXT(ST_AID_COND * condition, ST_AID_EXT* pxAid);

/**
Complete process online post processing
**/
int EMV_Completion(int OnlineResult, byte * IssuAuthenData, byte IssuAuthenDataLen, 
	byte * Issu71Script, byte Issu71ScriptLen, byte * Issu72Script, byte Issu72ScriptLen);

/**
Obtain transaction results
**/
int EMV_GetOutCome(ST_OUTCOME * stOutCome);

extern int EMV_GetTransParameter(int type, ST_TRANS * pxTRANS);
extern int EMV_CheckId(byte type, byte * no);
extern int EMV_SelectApp(int isRetry, int num, ST_CANDLIST * appList);
extern int EMV_InputOnlinePIN();
extern int EMV_ReadCardData();


void init_emv_api(u32 *apis);

int EMVL2CoreInit(void);
int EMVL2GetKernelType(void);
int EMVL2PreProcess(void);
int EMVL2AppSel(u32 Cardtype, u32 *CandListNum, ST_CANDLIST *CandList);
int EMVL2AppFinalSel(u32 FinalNo);
int EMVL2InitApp(void);
int EMVL2ReadAppData(void);
int EMVL2DataAuth(void);
int EMVL2ProcRestric(void);
int EMVL2TermRiskManage(void);
int EMVL2CVMProcess(u32 CVMStep, u32 CVMProcResult,u8 *CVMType);
int EMVL2TermActAnalysis(void);    
int EMVL2Completion(u32 OnlineResult, u32 Scriptlen, u8 *Script, u32 Issuerlen, u8 *IssuerData);    
int EMVL2BaseTLVOperate(u32 OpCode, u32 TagListLen,u8 *TagList, u32 *TLVDataLen,u8 *TLVData);
int EMVL2BaseParamOperate(u32 OpCode, ST_PARAM_EXTEND * ExParam);
int EMVL2PINVerify(void);
int EmvGetCandListBySeqNumber(int no, ST_CANDLIST * finalApp);
int Database_SetTlv(u32 index, u8 *pval, u16 len);
int Database_GetTlvData(u32 index, u8* value, u16 *len);

#endif


