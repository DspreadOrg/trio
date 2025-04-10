#include "app.h"
#include "secapi.h"
#include "ui.h"
#include "utils.h"
#include "func.h"
#include "emv.h"
#include "trans.h"
#include "emvproc.h"

#ifdef TRANS_DEBUG
#define TRANS_STRLOG DBG_STR
#define TRANS_HEXLOG DBG_HEX
#else
#define TRANS_STRLOG  
#define TRANS_HEXLOG 
#endif

void displayWarning(char *title, char *warning)
{
	int line;
	int len;
	int maxLen;
	byte tmp[26];
	
	if(title == NULL || strlen(title) <= 0){
		Ui_ClearLine(1, Ui_GetLineNum()-2);
	}else{
		Ui_Clear();
		Ui_DispTitle(title);
	}

	Ui_ClearLine(1, Ui_GetLineNum()-2);
	maxLen = strlen(warning);
	line = 2;
	if(maxLen > 40){
		line = 1;
	}
	for(len = 0; len < maxLen; line ++){
		memset(tmp, 0, sizeof(tmp));
		if(maxLen - len < Ui_GetLineEnNum()){
			len += Utils_StrCopy(tmp, warning + len, maxLen - len);
			Ui_DispTextLineAlign(line, DISPLAY_CENTER, tmp, 0);
		}else{
			len += Utils_StrCopy(tmp, warning + len, Ui_GetLineEnNum());
			Ui_DispTextLineAlign(line, DISPLAY_CENTER, tmp, 0);
		}
		Utils_BeepFAIL();

		if(line % (Ui_GetLineNum()-2)== 0 || len >= maxLen){
			Ui_WaitKey(5000);
			line = 0;
			Ui_ClearLine(1, Ui_GetLineNum()-2);
		}
	}
	
	return;
}

static int Trans_IsChipCard()
{
	byte *seperator;
	ST_TRANLOG * 	pxLOG = App_GetTranLog();

	if(pxLOG->track2Len == 0) {
		return APP_RET_FAIL;
	}

	seperator = strchr(pxLOG->track2, 'D');
	if(seperator) {
		if( (*(seperator+5) == '2') || (*(seperator+5) == '6'))	{
			return APP_RET_OK;
		}
		return APP_RET_FAIL;
	}
	return APP_RET_FAIL;
}

int Trans_GetCardInfo()
{
	int i;
	ST_TRANLOG *    pxLOG = App_GetTranLog();
	
	i = 0;
	while(i < pxLOG->track2Len) {
		if (pxLOG->track2[i] != '=' && pxLOG->track2[i] != 'D'){
			i++; 
			if(i > 19) {
				return APP_RET_ERR_SWIPE;
			}
			continue;
		}
		if( i < 13 || i > 19) {
			return APP_RET_ERR_SWIPE;
		}
		memcpy(pxLOG->card, pxLOG->track2, i);
		return APP_RET_OK;
	}
	
	i = 0;
	while(i < pxLOG->track3Len) {
		if (pxLOG->track3[i] != '=' && pxLOG->track3[i] != 'D'){
			i++; 
			if(i > 21) {
				return APP_RET_ERR_SWIPE;
			}
			continue;
		}
		if( i < 15 || i > 21) {
			return APP_RET_ERR_SWIPE;
		}
		memcpy(pxLOG->card, pxLOG->track3+2, i-2);
		return APP_RET_OK;
	}
	
	return APP_RET_ERR_SWIPE;
}

static void Trans_DispReadCard(int cardType)
{
	char 			ucDispbuf[100];
	ST_TRANLOG * 	pxLOG = App_GetTranLog();
		
	memset(ucDispbuf, 0, sizeof(ucDispbuf));
	if(cardType == CARD_MAG) strcpy(ucDispbuf,"pls swip");
	if(cardType == CARD_ICC) strcpy(ucDispbuf,"pls insert");
	if(cardType == CARD_PICC) strcpy(ucDispbuf,"pls tap");
	if(cardType == (CARD_MAG | CARD_ICC)) strcpy(ucDispbuf,"pls insert/swip");
	if(cardType == (CARD_MAG | CARD_PICC)) strcpy(ucDispbuf,"pls swip/tap");
	if(cardType == (CARD_ICC | CARD_PICC)) strcpy(ucDispbuf,"pls insert/tap");
	if(cardType == (CARD_PICC | CARD_MAG |CARD_ICC)) strcpy(ucDispbuf,"pls insert/swip/tap");

	Ui_ClearLine(1, Ui_GetLineNum()-2);
	if(atoi(pxLOG->amt) != 0) {
		Ui_DispTextLineAlign(2, DISPLAY_CENTER, ucDispbuf, 0);
		if(cardType & CARD_INPUT){
			Ui_DispTextLineAlign(3, DISPLAY_CENTER, "or input", 0);
		}
		memset(ucDispbuf, 0, sizeof(ucDispbuf));
		Utils_Asc2Amt(pxLOG->amt, ucDispbuf, sizeof(ucDispbuf));
		Ui_DispTextLineAlign(1, DISPLAY_RIGHT, ucDispbuf, 0);
		if(cardType & CARD_PICC) {
			//Ui_DispBMP(88, 108, 144, 72, cls_array);
			Ui_DispTextLineAlign(Ui_GetLineNum()-2, DISPLAY_CENTER, "tap on/reverse keyboard", 0);
		}
	} else {
		Ui_DispTextLineAlign(1, DISPLAY_LEFT, ucDispbuf, 0);
		if(cardType & CARD_INPUT){
			Ui_DispTextLineAlign(2, DISPLAY_CENTER, "or input", 0);
		}
		if(cardType & CARD_PICC) {
			//Ui_DispBMP(88, 84, 144, 72, cls_array);
			
			Ui_DispTextLineAlign(Ui_GetLineNum()-2, DISPLAY_CENTER, "tap on/reverse keyboard", 0);
		}
	}

}

static int Trans_ReadMagCard()
{
  byte * pxStar;
  byte * pxSplit;
  int    iRet;
  ST_TRANLOG *	  pxLOG = App_GetTranLog();
  
  pxLOG->track1Len = pxLOG->track2Len = pxLOG->track3Len = 0;
  memset(pxLOG->track1, 0, sizeof(pxLOG->track1));
  memset(pxLOG->track2, 0, sizeof(pxLOG->track2));
  memset(pxLOG->track3, 0, sizeof(pxLOG->track3));
  
  iRet = MagSwiped();//not card
  if(iRet==0){
	TRANS_STRLOG("MagSwiped = 0x%08X", iRet);
	goto END;
  }
  iRet = MagRead(pxLOG->track1, pxLOG->track2, pxLOG->track3);
  if(iRet < 0) {
  	TRANS_STRLOG("MagRead = 0x%08X", iRet);
    goto END;
  }
  if(iRet == 0) {
	  goto END;
  }
  TRANS_STRLOG("MagRead = 0x%08X", iRet);
 
  if(iRet & 0xFF00) {
    iRet = APP_RET_ERR_SWIPE;
    goto END;
  }
  
  if( (iRet & 0x01) == 0x01) {
    pxLOG->track1Len = strlen(pxLOG->track1);
	TRANS_STRLOG("track1len=%d,pxAPP->track1 = %s", pxLOG->track1Len,pxLOG->track1);
  }
  
  if( (iRet & 0x02) == 0x02) {
  	byte * pxStart = pxLOG->track2;
	pxLOG->track2Len = strlen(pxLOG->track2);
  	while(1)
    {
      byte * pxSplit = strstr(pxStart, "=");
      if(pxSplit == NULL) {
        break;
      }
      pxSplit[0] = 'D';
      pxStart = pxSplit + 1;
    }
	TRANS_STRLOG("track2len=%d,pxAPP->track2 = %s", pxLOG->track2Len,pxLOG->track2);
  }
  
  if( (iRet & 0x04) == 0x04) {
  	byte * pxStart = pxLOG->track3;
	pxLOG->track3Len = strlen(pxLOG->track3);
  	while(1)
    {
      byte * pxSplit = strstr(pxStart, "=");
      if(pxSplit == NULL) {
        break;
      }
      pxSplit[0] = 'D';
      pxStart = pxSplit + 1;
    }
	TRANS_STRLOG("track3len=%d,pxAPP->track3 = %s", pxLOG->track3Len,pxLOG->track3);
  }
  iRet = CARD_MAG;
END:
	MagClose();
  return iRet;
}

static int Trans_ReadCardInner(int *type, int isIdle)
{
	int 			iRet;
	int 			mode=0;
	byte 			tmp[100];
	int				cardType = *type;
	ST_TRANLOG *    pxLOG = App_GetTranLog();

	if(cardType == 0) {
		return APP_RET_TRANS_ESC;
	}
	
	memset(tmp, 0, sizeof(tmp));
	if ((cardType & CARD_ICC) && IccDetect(0x00) == 0 && IccInit(0x00,tmp) == 0) {
		TRANS_HEXLOG("ICC RESET:\r\n", tmp, 20);
		mode = CARD_ICC;
		goto PROCESS;
	}

	memset(tmp, 0, sizeof(tmp));
	iRet = PiccDetect(0x01,NULL,tmp,NULL,NULL);
	//TRANS_STRLOG("PiccDetect=%d",iRet);
 	if ((cardType & CARD_PICC) && iRet== 0) {
		TRANS_HEXLOG("PICC RESET:\r\n", tmp, 20);
		mode = CARD_PICC;
		goto PROCESS;
    }else if(iRet == RET_RF_ERR_MULTI_CARD){
		Ui_ClearLine(1, Ui_GetLineNum()-2);
		Ui_DispTextLineAlign(2, DISPLAY_CENTER, "multiple card conflict", 0);
		Ui_DispTextLineAlign(4, DISPLAY_CENTER, "pls remove a card go on", 0);
		Ui_DispTextLineAlign(6, DISPLAY_CENTER, "or <CANCEL> quit", 0);	
		Utils_BeepFAIL();	
		*type = CARD_PICC;
		Ui_WaitKey(3000);
		return APP_RET_ERR_RETRY;
	}
	
	if ( (cardType & CARD_MAG) ) {
	    iRet = Trans_ReadMagCard();
		if(iRet < 0 && !isIdle) {
			Ui_ClearLine(1, Ui_GetLineNum()-2);
			if(Ui_GetLcdHeight() == 64) {
				Ui_DispTextLineAlign(1, DISPLAY_CENTER, "swip error", 0);
				Ui_DispTextLineAlign(2, DISPLAY_CENTER, "pls go on swip", 0);
				Ui_DispTextLineAlign(3, DISPLAY_CENTER, "or <CANCEL> quit", 0);	
			} else {
				Ui_DispTextLineAlign(2, DISPLAY_CENTER, "swip error", 0);
				Ui_DispTextLineAlign(4, DISPLAY_CENTER, "pls go on swip", 0);
				Ui_DispTextLineAlign(6, DISPLAY_CENTER, "or <CANCEL> quit", 0);	
			}
			Utils_BeepFAIL();
			*type = CARD_MAG;
			
			return APP_RET_ERR_RETRY;
		}
		if(iRet != CARD_MAG) {
			return 0;
		}
	    mode = CARD_MAG;
		goto PROCESS;
	}
	return 0;

PROCESS:
  	TRANS_STRLOG("mode = %d\r\n", mode);
	if(mode == CARD_MAG) {
		pxLOG->isFallback = 1;//allow fallback
		TRANS_STRLOG("pxLOG->isFallback = %d\r\n", pxLOG->isFallback);
		if(Trans_IsChipCard() == APP_RET_OK && pxLOG->isFallback != 1) {
			Ui_ClearLine(1, Ui_GetLineNum()-2);
			Ui_DispTextLineAlign(2, DISPLAY_CENTER, "chip card", 0);
			Ui_DispTextLineAlign(3, DISPLAY_CENTER, "pls insert/tap", 0);
			*type = CARD_PICC | CARD_ICC;
			Utils_BeepFAIL();
			Ui_WaitKey(APP_PROMPT_TIMEOUT);
			
			return APP_RET_ERR_RETRY;
		}
		
		if(Trans_IsChipCard() != APP_RET_OK) {
			pxLOG->isFallback = 0;
		}
		
		iRet = Trans_GetCardInfo();
		if(iRet != APP_RET_OK) {
			TRANS_STRLOG("Trans_GetCardInfo = %d\r\n", iRet);
			return APP_RET_ERR_SWIPE;
		}
		Ui_ClearLine(1, Ui_GetLineNum()-2);
		Ui_DispTextLineAlign(1, DISPLAY_LEFT, "confirm card No:", 0);
		Ui_DispTextLineAlign(2, DISPLAY_LEFT, pxLOG->card, 0);
		while(1) {
			iRet = Ui_WaitKey(APP_UI_TIMEOUT);
			switch(iRet) {
			case KEYCANCEL:
			case KEYNONE:
				return  APP_RET_TRANS_ESC;
			case KEYENTER:
				strcpy(pxLOG->entryMode, ENTRY_SWIPE);
				return mode;
			}
		}
	}
	
	if(mode == CARD_ICC) {
		strcpy(pxLOG->entryMode, ENTRY_INSERT);
		return mode;
	}
	
	if(mode == CARD_PICC) {
		strcpy(pxLOG->entryMode, ENTRY_CLSS);
		return mode;
	}
	
	return APP_RET_ERR_SWIPE;
}

static int Trans_ReadCard(int type)
{
	int 			iRet;
	ST_Timer_UNIT   timer;
	int magflag=0;
	u8 tm=1;
	
	TRANS_STRLOG("cardType= %d\n", type);
	Trans_DispReadCard(type);
	
	if(type & CARD_MAG){
		MagOpen();
		MagReset();
		magflag = 1;
	}
	
	Ui_ClearKey();
	TimerSet(tm,600);
	while(0!=TimerCheck(tm)) 
	{		
		if(Ui_GetKey() == KB_CANCEL){
			iRet=APP_RET_TRANS_ESC;
			goto READEND;
		}
		iRet = Trans_ReadCardInner(&type, 0);
		if(iRet == APP_RET_ERR_RETRY) {
			Trans_DispReadCard(type);
			continue;
		}
		if(iRet != 0) {
			TRANS_STRLOG("Trans_ReadCardInner = %d", iRet);
			goto READEND;
		}
	}
	iRet=APP_RET_TRANS_ESC;
READEND:	
	if(magflag){
		MagClose();
	}

	return iRet;
}

int Trans_MagcardTest(void * pxPARAM){
	ST_TRANLOG *    pxLOG = App_GetTranLog();
	int iRet;
	byte tmp[20];
	
	App_InitTran();
	Func_DispTransTitle("mag card test");
	
INPUT_AMOUT:
	Ui_ClearLine(1, Ui_GetLineEnNum() - 2);
	memset(tmp, 0, sizeof(tmp));
	if(Ui_InputAmt(NULL, "pls input amt:", tmp, 10, 1, APP_UI_TIMEOUT) != UI_RET_SUCCESS) {
		return APP_RET_TRANS_ESC;
	}
	memset(pxLOG->amt, '0', 12);
	memcpy(pxLOG->amt + 12-strlen(tmp), tmp, strlen(tmp));
	
	memset((char *)pxLOG->entryMode, 0, sizeof(pxLOG->entryMode));
	pxLOG->isFallback = 1;//allow fallback
	iRet = Trans_ReadCard(CARD_MAG);
	if(iRet < 0) {
		TRANS_STRLOG("Trans_ReadCard = %d", iRet);
		return iRet;
	}
	
	TRANS_STRLOG("pxLOG->entryMode = %s", pxLOG->entryMode);
	iRet = Func_EnterPIN(0);
	if(iRet != SEC_RET_OK && iRet != SEC_ERR_NOPIN) {
		TRANS_STRLOG("Func_EnterPIN = %d", iRet);
		Func_DispRetCode("mag card test","pwd is error");
		return APP_RET_TRANS_ESC;
	}
	Func_DispRetCode("mag card test","test sucess");
	return 0;
}

int Trans_IcccardTest(void * pxPARAM){
	ST_TRANLOG *	pxLOG = App_GetTranLog();
	int iRet;
	byte tmp[20];
	u8 pTempBuffer[1024];

	App_InitTran();
	Func_DispTransTitle("chip card test");
INPUT_AMOUT:
	Ui_ClearLine(1, Ui_GetLineEnNum() - 2);
	memset(tmp, 0, sizeof(tmp));
	if(Ui_InputAmt(NULL, "pls input amt:", tmp, 10, 1, APP_UI_TIMEOUT) != UI_RET_SUCCESS) {
		return APP_RET_TRANS_ESC;
	}
	memset(pxLOG->amt, '0', 12);
	memcpy(pxLOG->amt + 12-strlen(tmp), tmp, strlen(tmp));
	memset(pxLOG->otheramt, '0', 12);
	memcpy(pxLOG->otheramt + 12-strlen(tmp), tmp, strlen(tmp));
	memset((char *)pxLOG->entryMode, 0, sizeof(pxLOG->entryMode));
	
	EMV_InitTransaction(SALE);
	iRet = Trans_ReadCard(CARD_ICC);
	if(iRet < 0) {
		TRANS_STRLOG("Trans_ReadCard = %d", iRet);
		Func_DispRetCode("chip card test","test fail");
		Func_RemoveCard();
		return iRet;
	}
	
	Ui_ClearLine(1, Ui_GetLineNum()-2);
	Ui_DispTextLineAlign(2, DISPLAY_CENTER, "trans is process", 0);
	Ui_DispTextLineAlign(3, DISPLAY_CENTER, "pls waiting", 0);
	
	iRet = EMV_Process(CARD_ICC);
	if(iRet != APP_RET_OK) {
		TRANS_STRLOG("EMV_Process = %d\r\n", iRet);
		Func_DispRetCode("chip card test","test fail");
		Func_RemoveCard();
		return iRet;
	}
	Func_DispRetCode("chip card test","test success");
	Func_RemoveCard();
	return 0;
}

int Trans_PicccardTest(void * pxPARAM){
	ST_TRANLOG *	pxLOG = App_GetTranLog();
	int iRet;
	byte tmp[20];
	u8 pTempBuffer[1024];

	App_InitTran();
	Func_DispTransTitle("tap card test");
INPUT_AMOUT:
	Ui_ClearLine(1, Ui_GetLineEnNum() - 2);
	memset(tmp, 0, sizeof(tmp));
	if(Ui_InputAmt(NULL, "pls input amt:", tmp, 10, 1, APP_UI_TIMEOUT) != UI_RET_SUCCESS) {
		return APP_RET_TRANS_ESC;
	}
	memset(pxLOG->amt, '0', 12);
	memcpy(pxLOG->amt + 12-strlen(tmp), tmp, strlen(tmp));
	memset(pxLOG->otheramt, '0', 12);
	memcpy(pxLOG->otheramt + 12-strlen(tmp), tmp, strlen(tmp));
	memset((char *)pxLOG->entryMode, 0, sizeof(pxLOG->entryMode));
	
	EMV_InitTransaction(SALE);
	//preprocess
	iRet = EMV_CLSSPreProcess();
	if(iRet == ENDAPPLICATION_CLSS_LIMIT_EXCEED) {
		Func_DispRetCode("tap card test","amt limit exceed!");
		return iRet;
	} else if(iRet < 0) {
		return iRet;
	}
	
	iRet = Trans_ReadCard(CARD_PICC);
	if(iRet < 0) {
		TRANS_STRLOG("Trans_ReadCard = %d", iRet);
		Func_DispRetCode("tap card test","test fail");
		Func_RemoveCard();
		return iRet;
	}

	Ui_ClearLine(1, Ui_GetLineNum()-2);
	Ui_DispTextLineAlign(2, DISPLAY_CENTER, "trans is process", 0);
	Ui_DispTextLineAlign(3, DISPLAY_CENTER, "pls waiting", 0);
	iRet = EMV_Process_Simple(CARD_PICC);
	TRANS_STRLOG("EMV_Process_Simple = %d\r\n", iRet);
	if(iRet != ONLINE_REQUEST) {
		if(iRet == APP_RET_FALLBACK) 
			Func_DispRetCode("tap card test","fallback");
		else if(iRet == APP_RET_OTHERFACE) 
			Func_DispRetCode("tap card test","use other interface");
		else{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp,"fail:%d",iRet);
			Func_DispRetCode("tap card test",tmp);
		}
		Func_RemoveCard();
		return iRet;
	}
	Func_DispRetCode("tap card test","test success");
	Func_RemoveCard();
	return 0;
}

int Trans_MkskKeyInit(void * pxPARAM){
	int iRet;
	ST_KEY_INFO keyinfo;
	ST_KCV_INFO kcvinfo;

	TRANS_STRLOG("Trans_MkskKeyInit begin");
	memset(&keyinfo,0,sizeof(keyinfo));
	keyinfo.ucSrcKeyType = PED_TLK;
	keyinfo.ucSrcKeyIdx = 0;
	keyinfo.ucDstKeyType = PED_TMK;
	keyinfo.ucDstKeyIdx = SEC_KEK_INDEX;
	keyinfo.iDstKeyLen = 16;
	Utils_Asc2Bcd("F028AE33CE298B9A12324124244343AD",32,keyinfo.aucDstKeyValue);
	memset(&kcvinfo,0,sizeof(kcvinfo));
	kcvinfo.iCheckMode=0x00;//no kcv
	//save kek
	iRet = PedWriteKey(&keyinfo,&kcvinfo);
	TRANS_STRLOG("PedWriteKey KEK=%d",iRet);
	if(iRet != 0)
		return iRet;

	memset(&keyinfo,0,sizeof(keyinfo));
	keyinfo.ucSrcKeyType = PED_TLK;
	keyinfo.ucSrcKeyIdx = SEC_KEK_INDEX;
	keyinfo.ucDstKeyType = PED_TMK;
	keyinfo.ucDstKeyIdx = SEC_MAINKEY_INDEX;
	keyinfo.iDstKeyLen = 16;
	Utils_Asc2Bcd("38A3CF0198BCDFD7C08B40A7250BE289",32,keyinfo.aucDstKeyValue);
	memset(&kcvinfo,0,sizeof(kcvinfo));
	kcvinfo.iCheckMode=0x00;//no kcv
	//save mainkey
	iRet = PedWriteKey(&keyinfo,&kcvinfo);
	TRANS_STRLOG("PedWriteKey MAINKEY=%d",iRet);
	if(iRet != 0)
		return iRet;

	memset(&keyinfo,0,sizeof(keyinfo));
	keyinfo.ucSrcKeyType = /*PED_TMK*/SEC_MKSK;
	keyinfo.ucSrcKeyIdx = SEC_MAINKEY_INDEX;
	keyinfo.ucDstKeyType = PED_TPK;
	keyinfo.ucDstKeyIdx = SEC_PINKEY_INDEX;
	keyinfo.iDstKeyLen = 16;
	Utils_Asc2Bcd("088CAED653BCAAA3088CAED653BCAAA3",32,keyinfo.aucDstKeyValue);
	memset(&kcvinfo,0,sizeof(kcvinfo));
	kcvinfo.iCheckMode=0x00;//no kcv
	//save pinkey
	iRet = PedWriteKey(&keyinfo,&kcvinfo);
	TRANS_STRLOG("PedWriteKey PINKEY=%d",iRet);
	
	if(((u8*)pxPARAM)[0] == 0x01)
	{
		u8 disp_buf[32] = {0};
		if(iRet == 0)
		{
			sprintf(disp_buf, "%s\0", "write KEY suc");
		}
		else{
			sprintf(disp_buf, "%s\0", "write KEY fail");
		}
		Ui_DispTextLineAlign(3, DISPLAY_CENTER, disp_buf, 0);
	}
	else{
		if(iRet == 0)
			Func_DispRetCode("MKSK INIT","write KEY suc");
		else
			Func_DispRetCode("MKSK INIT","write KEY fail");
	}
	

	return iRet;
}



int Trans_MkskKeyTest(void * pxPARAM){
	int iRet;
	ST_KEY_INFO keyinfo;
	ST_KCV_INFO kcvinfo;

	memset(&keyinfo,0,sizeof(keyinfo));
	keyinfo.ucSrcKeyType = PED_TMK;
	keyinfo.ucSrcKeyIdx = 2;
	keyinfo.ucDstKeyType = PED_TDK;
	keyinfo.ucDstKeyIdx = 8;
	keyinfo.iDstKeyLen = 16;
	Utils_Asc2Bcd("0123456789ABCDEFFEDCBA9876543210",32,keyinfo.aucDstKeyValue);
	memset(&kcvinfo,0,sizeof(kcvinfo));
	kcvinfo.iCheckMode=0x00;//no kcv

	iRet = PedWriteKey(&keyinfo,&kcvinfo);
	TRANS_STRLOG("PedWriteKey TDK=%d",iRet);
	if(iRet != 0)
		return iRet;

	int ret = 0;
	unsigned char time[32] = { 0 };
	unsigned char rsp[32] = { 0 };
	memset(time, 0, sizeof(time));
	ret = PedCalcDES(8, time, 16, rsp, 1);
	DBG_STR("Encryption Data 1 ret : %d",ret);
	DBG_HEX("Encryption Data 1: ", rsp, sizeof(rsp));

	return iRet;
}