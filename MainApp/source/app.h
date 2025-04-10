#ifndef __APP_H__
#define __APP_H__
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <liteos_api.h>
#include "liteMapi.h"
#include "type.h"

//#define APP_DEBUG

#ifdef APP_DEBUG
#define  WNET_DEBUG
#define  WIFI_DEBUG
#define EMV_DEBUG
#define FUNC_DEBUG
#define TRANS_DEBUG
#define PRT_DEBUG
#define PAYPASSPRM_DEBUG
#define AEPRM_DEBUG
#endif

#define SALE 		0x00	
#define BALANCE 	0x01	
#define REFUND 		0x02	
#define EMV_SCRIPT_UP  0x03

#define HWND_APP 1000

#define SEC_MAINKEY_INDEX   11	
#define SEC_KEK_INDEX       1	
#define SEC_MACKEY_INDEX    2	
#define SEC_PINKEY_INDEX    3	
#define SEC_TRACKKEY_INDEX  4	

#if !defined(APP_VER)
#define APP_VER ("V0.0")
#else

#endif

typedef struct {
	byte kernalType;		
	byte TC[8];				
	byte ARQC[8];			
	byte ARPC[8];			
	byte TVR[6];			
	byte TSI[3];			
	byte ATC[3];			
	byte CVM[3];			
	byte appLable[32+1];	
	byte appName[32+1];		
	byte aid[32+1];			
	uint iccLen;			
	byte icc[255];			
}ST_ICCINFO;

typedef struct {
 	byte payWay[20+1];		
 	byte payURL[128+1];		
 	byte payCode[32+1];		
 	byte payTrace[32+1];	
 	byte payId[32+1];		
 	byte nonceCode[20+1];	
 	byte status[20+1];		
}ST_QRINFO;

typedef union {
	ST_ICCINFO icc;
	ST_QRINFO qr;
}UN_EXTINFO;

typedef struct {
	byte type;			
	byte subType;			
	byte isBatch;           
	byte isRev;				
	byte isSendNotice;		
	byte isSendSign;		
	byte sendTimes;			
	uint signRecNo;			
	byte isT0;              
	uint trace;				
	uint orgTrace;			
	uint batch;				
	uint orgBatch;			
	uint processCode;		
	byte amt[12+1];			
	byte currencycode[2+1];			
	byte otheramt[12+1];			
	byte rspAmt[12+1];		
	byte balanceFlag;		
	byte balance[12+1];		
	byte oper[2+1];			
	byte date[8+1];			
	byte time[6+1];			
	byte orgDate[8+1];		
	byte orgTime[6+1];		
	byte entryMode[3+1];	
	byte isFallback;		
	byte card[20+1];		
	byte cardSn[3+1];		
	byte cardExpire[4+1];	
	byte cardUnit[3+1];		
	byte settleDate[4+1];	
	byte centerId[11+1];    
	byte acqBank[8+1];		
	byte issBank[8+1];		
	byte reference[12+1];	
	byte orgReference[12+1];
	byte authCode[6+1];		
	byte rspCode[2+1];      
	byte TransResult;		
	byte bitmap[8];         
	byte dupReason[2+1];	
	byte isNoPin;			
	byte isNoSign;			
	
	uint track1Len;
	byte track1[80];		
	uint track2Len;
	byte track2[40];		
	uint track3Len;
	byte track3[128];		
	byte dupIcc[256];		
	byte dupIccLen;			 
	byte pin[8+1];			
	
	UN_EXTINFO extInfo;		
}ST_TRANLOG;

#define CARD_ICC          0x01
#define CARD_PICC         0x02
#define CARD_MAG          0x04
#define CARD_INPUT        0x08

#define  ENTRY_MANUAL	"012"  
#define  ENTRY_SWIPE	"022"  
#define  ENTRY_INSERT	"052"  
#define  ENTRY_FALLBACK	"802"  
#define  ENTRY_CLSS		"072"  
#define  ENTRY_MOBILE   "962"  
#define  ENTRY_PHONE    "922" 


#define PR_PARA			0x01	
#define PR_STATU		0x02	
#define PR_LOGON		0x04	
#define PR_CA			0x08	
#define PR_ICPARA		0x10	
#define PR_TMS			0x20	
#define PR_BLACK		0x40	
#define PR_RATE			0x80	


#define APP_UI_TIMEOUT 60000
#define APP_PROMPT_TIMEOUT 2000

#define APP_RET_OK      0
#define APP_RET_BASE    -1000
#define APP_RET_FAIL    (APP_RET_BASE - 0)
#define APP_RET_CANCLE  (APP_RET_BASE - 1)
#define APP_RET_TIMEOUT (APP_RET_BASE - 2)
#define APP_RET_PARAM 	(APP_RET_BASE - 3)
#define APP_RET_ABORT   (APP_RET_BASE - 4)
#define APP_RET_TRANS_ESC   (APP_RET_BASE - 5)
#define APP_RET_TRANS_FAIL  (APP_RET_BASE - 6)
#define APP_RET_NO_RECORD   (APP_RET_BASE - 7)
#define APP_RET_NO_JY       (APP_RET_BASE - 8)
#define APP_RET_ERR_MP      (APP_RET_BASE - 9)
#define APP_RET_ERR_CONNECT (APP_RET_BASE - 10)
#define APP_RET_ERR_FP      (APP_RET_BASE - 11)
#define APP_RET_ERR_RP      (APP_RET_BASE - 12)
#define APP_RET_ERR_JP      (APP_RET_BASE - 13)
#define APP_RET_DUP_FAIL    (APP_RET_BASE - 14)
#define APP_RET_ERR_TRANS   (APP_RET_BASE - 15)
#define APP_RET_NO_OLD_JY   (APP_RET_BASE - 16)
#define APP_RET_ERR_ESCED   (APP_RET_BASE - 17)
#define APP_RET_ERR_VOID    (APP_RET_BASE - 18)
#define APP_RET_ERR_SWIPE   (APP_RET_BASE - 19)
#define APP_RET_NO_EQU_AMT  (APP_RET_BASE - 20)
#define APP_RET_ERR_MAC     (APP_RET_BASE - 21)
#define APP_RET_ERR_MEM     (APP_RET_BASE - 22)
#define APP_RET_NO_KEY      (APP_RET_BASE - 23)
#define APP_RET_TIP_FIN     (APP_RET_BASE - 24)
#define APP_RET_CNTCT_ISSU  (APP_RET_BASE - 25)
#define APP_RET_INVALID_TIP (APP_RET_BASE - 26)
#define APP_RET_TIP_NO_OPEN (APP_RET_BASE - 27)
#define APP_RET_ERR_ADJUESTED (APP_RET_BASE - 28)
#define APP_RET_TRANS_REF   (APP_RET_BASE - 29)
#define APP_RET_NO_MOBILE   (APP_RET_BASE - 30)
#define APP_RET_NO_UPCASH   (APP_RET_BASE - 31)
#define APP_RET_ERR_BLACK   (APP_RET_BASE - 32)
#define APP_RET_ERR_TUI_DATE (APP_RET_BASE - 33)
#define APP_RET_ERR_AMT     (APP_RET_BASE - 34)
#define APP_RET_ERR_CLSS    (APP_RET_BASE - 35)
#define APP_RET_ERR_EMV     (APP_RET_BASE - 36)
#define APP_RET_NO_TUI_JY   (APP_RET_BASE - 37)

#define APP_RET_ERR_MSGID   (APP_RET_BASE - 38)
#define APP_RET_ERR_PROCODE (APP_RET_BASE - 39)
#define APP_RET_ERR_RSPAMT  (APP_RET_BASE - 40)
#define APP_RET_ERR_TRACE   (APP_RET_BASE - 41)
#define APP_RET_ERR_TERMID  (APP_RET_BASE - 42)
#define APP_RET_ERR_MERID   (APP_RET_BASE - 43)
#define APP_RET_ERR_KEY     (APP_RET_BASE - 44)
#define APP_RET_FALLBACK    (APP_RET_BASE - 45)
#define APP_RET_NO_DISP     (APP_RET_BASE - 46)
#define APP_RET_OTHERFACE   (APP_RET_BASE - 47)
#define APP_RET_ERR_BAT     (APP_RET_BASE - 48)
#define APP_RET_ERR_SPACE   (APP_RET_BASE - 49)

#define APP_RET_ERR_PAYCODE (APP_RET_BASE - 50)
#define APP_RET_ERR_PWD     (APP_RET_BASE - 51)
#define APP_RET_ERR_OPER	(APP_RET_BASE - 52)
#define APP_RET_ERR_RETRY	(APP_RET_BASE - 53)
#define APP_RET_ERR_QPSONLY	(APP_RET_BASE - 54)


ST_TRANLOG * App_GetTranLog();
void App_InitTran();

#endif

