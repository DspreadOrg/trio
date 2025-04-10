#include "app.h"
#include "emv.h"
#include "file.h"
#include "secapi.h"
#include "utils.h"

static int          m_isEmvDebug = 0;	
static ST_TRANS     m_emvTransInfo; 
static ST_DEFAULT   m_emvDefaultParam; // only set once
static int          s_cur_emv_kernel_type= EMV;
static int          m_IsOnlinePin = 1;	

extern void s_AE_FixCfg(void);
extern void s_AE_NotFixCfg();
extern void s_PaypassFixCfg();
extern void s_PaypassNotFixCfg(void);
extern int EmvL2ScExchange(const char *DeviceId, const C_APDU *c_apdu, R_APDU *r_apdu);

static int (*fun_EMVL2CoreInit)(void);
static int (*fun_EMVL2GetKernelType)(void);
static int (*fun_EMVL2PreProcess)(void);
static int (*fun_EMVL2AppSel)(u32 Cardtype, u32 *CandListNum, ST_CANDLIST *CandList);
static int (*fun_EMVL2AppFinalSel)(u32 FinalNo);
static int (*fun_EMVL2InitApp)(void);
static int (*fun_EMVL2ReadAppData) (void);
static int (*fun_EMVL2DataAuth)(void);
static int (*fun_EMVL2ProcRestric)(void);
static int (*fun_EMVL2TermRiskManage)(void);
static int (*fun_EMVL2CVMProcess)(u32 CVMStep, u32 CVMProcResult,u8 *CVMType);
static int (*fun_EMVL2TermActAnalysis)(void);    
static int (*fun_EMVL2Completion)(u32 OnlineResult, u32 Scriptlen, u8 *Script, u32 Issuerlen, u8 *IssuerData);    
static int (*fun_EMVL2BaseTLVOperate)(u32 OpCode, u32 TagListLen,u8 *TagList, u32 *TLVDataLen,u8 *TLVData);
static int (*fun_EMVL2BaseParamOperate) (u32 OpCode, ST_PARAM_EXTEND * ExParam);
static int (*fun_EMVL2PINVerify )(void);
static int (*fun_EmvGetCandListBySeqNumber)(int no, ST_CANDLIST * finalApp);
static int (*fun_Database_SetTlv)(u32 index, u8 *pval, u16 len);
static int (*fun_Database_GetTlvData)(u32 index, u8* value, u16 *len);

ST_TRANS* EMV_GetTrans(){
	return &m_emvTransInfo;
}

ST_DEFAULT* EMV_GetDefault(){
	return &m_emvDefaultParam;
}

void init_emv_api(u32 *apis)
{
    int idx = 0;
	
	DBG_STR("init_emv_api =%08x,%08x,%08x,%08x,%08x",apis[idx],apis[idx+1],apis[idx+2],apis[idx+3],apis[idx+4]);	
    fun_EMVL2CoreInit    = (void*)apis[idx++];
    fun_EMVL2GetKernelType                		= (void*)apis[idx++];
    fun_EMVL2PreProcess         = (void*)apis[idx++];
    fun_EMVL2AppSel                	= (void*)apis[idx++];
    fun_EMVL2AppFinalSel                	= (void*)apis[idx++];
    fun_EMVL2InitApp                	= (void*)apis[idx++];
    fun_EMVL2ReadAppData                	= (void*)apis[idx++];
    fun_EMVL2DataAuth                	= (void*)apis[idx++];
    fun_EMVL2ProcRestric                	= (void*)apis[idx++];
    fun_EMVL2TermRiskManage                	= (void*)apis[idx++];
    fun_EMVL2CVMProcess                	= (void*)apis[idx++];
    fun_EMVL2TermActAnalysis                	= (void*)apis[idx++];
    fun_EMVL2Completion                	= (void*)apis[idx++];
    fun_EMVL2BaseTLVOperate                	= (void*)apis[idx++];
    fun_EMVL2BaseParamOperate                	= (void*)apis[idx++];
    fun_EMVL2PINVerify                	= (void*)apis[idx++];
    fun_EmvGetCandListBySeqNumber                	= (void*)apis[idx++];
    fun_Database_SetTlv                	= (void*)apis[idx++];
    fun_Database_GetTlvData                	= (void*)apis[idx++];

}

int EMVL2CoreInit(void){
	if(fun_EMVL2CoreInit) return fun_EMVL2CoreInit();
	return EMV_NO_FUNC;
}
int EMVL2GetKernelType(void){
	if(fun_EMVL2GetKernelType) return fun_EMVL2GetKernelType();
	return EMV_NO_FUNC;
}
int EMVL2PreProcess(void){
	if(fun_EMVL2PreProcess) return fun_EMVL2PreProcess();
	return EMV_NO_FUNC;
}
int EMVL2AppSel(u32 Cardtype, u32 *CandListNum, ST_CANDLIST *CandList){
	if(fun_EMVL2AppSel) return fun_EMVL2AppSel(Cardtype, CandListNum,CandList);
	return EMV_NO_FUNC;
}
int EMVL2AppFinalSel(u32 FinalNo){
	if(fun_EMVL2AppFinalSel) return fun_EMVL2AppFinalSel(FinalNo);
	return EMV_NO_FUNC;
}
int EMVL2InitApp(void){
	if(fun_EMVL2InitApp) return fun_EMVL2InitApp();
	return EMV_NO_FUNC;
}
int EMVL2ReadAppData(void){
	if(fun_EMVL2ReadAppData) return fun_EMVL2ReadAppData();
	return EMV_NO_FUNC;
}
int EMVL2DataAuth(void){
	if(fun_EMVL2DataAuth) return fun_EMVL2DataAuth();
	return EMV_NO_FUNC;
}
int EMVL2ProcRestric(void){
	if(fun_EMVL2ProcRestric) return fun_EMVL2ProcRestric();
	return EMV_NO_FUNC;
}
int EMVL2TermRiskManage(void){
	if(fun_EMVL2TermRiskManage) return fun_EMVL2TermRiskManage();
	return EMV_NO_FUNC;
}
int EMVL2CVMProcess(u32 CVMStep, u32 CVMProcResult,u8 *CVMType){
	if(fun_EMVL2CVMProcess) return fun_EMVL2CVMProcess(CVMStep, CVMProcResult,CVMType);
	return EMV_NO_FUNC;
}
int EMVL2TermActAnalysis(void){
	if(fun_EMVL2TermActAnalysis) return fun_EMVL2TermActAnalysis();
	return EMV_NO_FUNC;
}    
int EMVL2Completion(u32 OnlineResult, u32 Scriptlen, u8 *Script, u32 Issuerlen, u8 *IssuerData){
	if(fun_EMVL2Completion) return fun_EMVL2Completion(OnlineResult, Scriptlen,Script, Issuerlen, IssuerData);
	return EMV_NO_FUNC;
}    
int EMVL2BaseTLVOperate(u32 OpCode, u32 TagListLen,u8 *TagList, u32 *TLVDataLen,u8 *TLVData){
	if(fun_EMVL2BaseTLVOperate) return fun_EMVL2BaseTLVOperate(OpCode,TagListLen,TagList, TLVDataLen,TLVData);
	return EMV_NO_FUNC;
}
int EMVL2BaseParamOperate(u32 OpCode, ST_PARAM_EXTEND * ExParam){
	if(fun_EMVL2BaseParamOperate) return fun_EMVL2BaseParamOperate(OpCode, ExParam);
	return EMV_NO_FUNC;
}
int EMVL2PINVerify(void){
	if(fun_EMVL2PINVerify) return fun_EMVL2PINVerify();
	return EMV_NO_FUNC;
}
int EmvGetCandListBySeqNumber(int no, ST_CANDLIST * finalApp){
	if(fun_EmvGetCandListBySeqNumber) return fun_EmvGetCandListBySeqNumber( no, finalApp);
	return EMV_NO_FUNC;
}
int Database_SetTlv(u32 index, u8 *pval, u16 len){
	if(fun_Database_SetTlv) return fun_Database_SetTlv(index, pval, len);
	return EMV_NO_FUNC;
}
int Database_GetTlvData(u32 index, u8* value, u16 *len){
	if(fun_Database_GetTlvData) return fun_Database_GetTlvData(index, value, len);
	return EMV_NO_FUNC;
}

void EMV_SetDebug(int isDebug)
{
	m_isEmvDebug = isDebug;
}

int EMV_GetDebug()
{
	return m_isEmvDebug;
}

void EMV_SetIsInputPIN(int isInput) 
{
	m_IsOnlinePin = isInput;
}

int EMV_GetCurKernelType()
{
    return s_cur_emv_kernel_type;
}

static void EMV_SetCurKernelType(uint type)
{
    s_cur_emv_kernel_type = type;
}

int EMV_ClearAID()
{
	return remove(AID_PARAM_FILE);
}

int EMV_AddAID(ST_AID * pxAID)
{
	return File_Append(AID_PARAM_FILE, (byte *)pxAID, sizeof(ST_AID));
}

static int EMV_GetAIDNumber()
{
	return File_GetRecordNum(AID_PARAM_FILE, sizeof(ST_AID));
}

static int EMV_GetAIDByNo(uint no, ST_AID * pxAID)
{
	return File_ReadRecord(AID_PARAM_FILE, no, sizeof(ST_AID), (byte *)pxAID);
}

static int EMV_MatchAID(int no, byte * data, uint len, void * param)
{
	ST_AID_COND *stCond = (ST_AID_COND *)param;
	ST_AID *stAid = (ST_AID *)data;
	
	if (stAid == NULL || stCond == NULL || len != sizeof(ST_AID))
	{
		return RET_FILE_NOT_MATCH;
	}
		
	if (memcmp(stCond->AID, stAid->AID, stAid->AIDLen[0]) == 0 
	&& (stAid->AIDLen[0] == stCond->AIDLen || stAid->SelFlag[0] == 0))
	{
		if(EMV_GetDebug()) {
			DBG_STR("found no = %d", no);
			DBG_STR("stAid->AIDLen = %d", stAid->AIDLen[0]);
			DBG_HEX("stAid->AID", stAid->AID, stAid->AIDLen[0]);
			DBG_STR("stAid->SelFlag = %d", stAid->SelFlag[0]);
		}
	  	return no;
	}
	
	return RET_FILE_NOT_MATCH;	
}

int EMV_GetAID(ST_AID_COND * condition, ST_AID * pxAid)
{
	return File_ForEachRecord(AID_PARAM_FILE, sizeof(ST_AID), (byte *) pxAid, EMV_MatchAID, (void *) condition);
}

///////////////////////////////////////////  AID_EXT
int EMV_ClearAIDEXT()
{
	return remove(AIDEXT_PARAM_FILE);
}

int EMV_AddAIDEXT(ST_AID_EXT* pxAID)
{
	return File_Append(AIDEXT_PARAM_FILE, (byte *)pxAID, sizeof(ST_AID_EXT));
}

static int EMV_MatchAIDEXT(int no, byte * data, uint len, void * param)
{
	ST_AID_COND *stCond = (ST_AID_COND *)param;
	ST_AID_EXT* stAid = (ST_AID_EXT *)data;
	
	if (stAid == NULL || stCond == NULL || len != sizeof(ST_AID_EXT))
	{
		return RET_FILE_NOT_MATCH;
	}
		
	if (memcmp(stCond->AID, stAid->AID, stAid->AIDLen[0]) == 0 
	&& (stAid->AIDLen[0] == stCond->AIDLen || stAid->SelFlag[0] == 0))
	{
		if(EMV_GetDebug()) {
			DBG_STR("found no = %d", no);
			DBG_STR("stAid->AIDLen = %d", stAid->AIDLen[0]);
			DBG_HEX("stAid->AID", stAid->AID, stAid->AIDLen[0]);
			DBG_STR("stAid->SelFlag = %d", stAid->SelFlag[0]);
		}
	  	return no;
	}
	
	return RET_FILE_NOT_MATCH;	
}

int EMV_GetAIDEXT(ST_AID_COND * condition, ST_AID_EXT* pxAid)
{
	return File_ForEachRecord(AIDEXT_PARAM_FILE, sizeof(ST_AID_EXT), (byte *) pxAid, EMV_MatchAIDEXT, (void *) condition);
}
///////////////////////////////////////////
int EMV_ClearCAPK()
{
	return File_Remove(CAPK_PARAM_FILE);
}

int EMV_AddCAPK(ST_CAPK * pxCAPK)
{
	return File_Append(CAPK_PARAM_FILE, (byte *)pxCAPK, sizeof(ST_CAPK));
}

static int EMV_MatchCAPK(int no, byte * data, uint len, void * param)
{	
	ST_CAPK_COND *stCond = (ST_CAPK_COND *)param;
	ST_CAPK *stCapk = (ST_CAPK *)data;
	
	if (stCapk == NULL || stCond == NULL || len != sizeof(ST_CAPK))
	{
		return RET_FILE_NOT_MATCH;
	}
	
	if (memcmp(stCond->RID, stCapk->RID, 5) == 0 && stCond->Index == stCapk->Index)
	{
		if(EMV_GetDebug()) {
			DBG_STR("found no = %d", no);
			DBG_HEX("stCapk->RID", stCapk->RID, 5);
			DBG_STR("stCapk->Index = %d", stCapk->Index);
			DBG_STR("stCapk->HashInd = %d", stCapk->HashInd);
			DBG_STR("stCapk->ArithInd = %d", stCapk->ArithInd);
			DBG_HEX("stCapk->Exponent", stCapk->Exponent, stCapk->ExponentLen);
			DBG_HEX("stCapk->Modul", stCapk->Modul, stCapk->ModulLen);
			DBG_HEX("stCapk->ExpDate", stCapk->ExpDate, 3);
			DBG_HEX("stCapk->CheckSum", stCapk->CheckSum, 20);
		}
	 	return no;
	}
	
	return RET_FILE_NOT_MATCH;	
}

static int EMV_GetCAPK(ST_CAPK_COND * condition, ST_CAPK * pxCAPK)
{
	return File_ForEachRecord(CAPK_PARAM_FILE, sizeof(ST_CAPK), (byte *) pxCAPK, EMV_MatchCAPK, (void *) condition);
}

int EMV_SetDefault(ST_DEFAULT * pxDEF)
{
	File_Remove(DEFAULT_FILE);
	
	return File_Append(DEFAULT_FILE, (byte *)pxDEF, sizeof(ST_DEFAULT));
}

static int EMV_ReadDefault(ST_DEFAULT * pxDEF)
{
	int iRet;
	
	iRet = File_ReadRecord(DEFAULT_FILE, 0, sizeof(ST_DEFAULT), (byte *) pxDEF);
	if(iRet != RET_FILE_OK) {
		return iRet;
	}
	
	return EMV_RET_OK;
}


static void EMV_Default2AidParam(const ST_DEFAULT *stDefault, ST_AIDPARAM * stAidParam)
{
  memcpy(stAidParam->AcquierId, stDefault->AcquierId, 6);         
  stAidParam->tDOLLen = stDefault->tDOLLen[0];                    
  memcpy(stAidParam->tDOL, stDefault->tDOL, 32);                  
  stAidParam->RMDLen = stDefault->RMDLen[0];                      
  memcpy(stAidParam->RiskManData, stDefault->RiskManData, 8);     
  memcpy(stAidParam->MerchName, stDefault->MerchName, 128);       
  memcpy(stAidParam->MerchCateCode, stDefault->MerchCateCode, 2); 
  memcpy(stAidParam->MerchId, stDefault->MerchId, 16);            
  memcpy(stAidParam->TermId, stDefault->TermId, 8);               
  // memcpy(stAidParam->TransCurrCode, stDefault->TransCurrCode, 2); 
  // stAidParam->TransCurrExp = stDefault->TransCurrExp[0];          
  // memcpy(stAidParam->ReferCurrCode, stDefault->ReferCurrCode, 2); 
  // stAidParam->ReferCurrExp = stDefault->ReferCurrExp[0];          
  stAidParam->RandTransSel = 1;                                   
  stAidParam->VelocityCheck = 1;                                  

  return;
}

static void EMV_Default2TermParam(const ST_DEFAULT *stDefault, ST_TERMPARAM * stTerm)
{
  stTerm->TerminalType = stDefault->TerminalType[0];          
  memcpy(stTerm->IFDSn, stDefault->IFDSn, 8);                 
  memcpy(stTerm->CountryCode, stDefault->CountryCode, 2);     
  memcpy(stTerm->TermAIP, stDefault->TermAIP, 2);             
  memcpy(stTerm->Capability, stDefault->Capability, 3);       
  memcpy(stTerm->AddCapability, stDefault->AddCapability, 5); 
  memcpy(stTerm->TTQ, stDefault->TTQ, 4);                     
  memcpy(stTerm->ECTTLVal, stDefault->ECTTLVal, 6);           

  if (memcmp(stTerm->IFDSn, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) == 0)
  {
    char sn[30];
	memset(sn, 0, sizeof(sn));
    memcpy(sn,"12345678",8);
    if (strlen(sn) > 8)
    {
      memcpy(stTerm->IFDSn, sn + strlen(sn) - 8, 8);
    }
    else
    {
      memcpy(stTerm->IFDSn, sn, strlen(sn));
    }
  }
  if (memcmp(stTerm->TermAIP, "\x00\x00", 2))
  { 
    stTerm->UseTermAIPFlg = 1;
  }
  if (memcmp(stTerm->ECTTLVal, "\x00\x00\x00\x00\x00\x00", 6)) 
  {
    stTerm->ECTSIFlg = 1; 
    stTerm->ECTSIVal = 1; 
    stTerm->ECTTLFlg = 1; 
  }

  stTerm->ForceOnline = 0;   
  stTerm->GetDataPIN = 1;    
  stTerm->SurportPSESel = 1; 
  stTerm->BypassAllFlg = 0;  // whether bypass all other pin when one pin has been bypassed 1-Yes, 0-No
  stTerm->BypassPin = 1;     
  stTerm->BatchCapture = 0;  // 0-online data capture, 1-batch capture
  stTerm->ScriptMode = 0;
  stTerm->AdviceFlag = 1;
  stTerm->IsSupportSM = 1;            
  stTerm->IsSupportTransLog = 1;      
  stTerm->IsSupportMultiLang = 1;     
  stTerm->IsSupportExceptFile = 1;    
  stTerm->IsSupportAccountSelect = 1; 
  stTerm->IsReadLogInCard = 0;        
}

static void EMV_Aid2AidList(const ST_AID * stAid, ST_TERMAIDLIST * stAidList)
{
  stAidList->AidLen = stAid->AIDLen[0];
  memcpy(stAidList->AID, stAid->AID, stAid->AIDLen[0]);
  stAidList->SelFlag = stAid->SelFlag[0];

  if (memcmp(stAidList->AID, "\xA0\x00\x00\x00\x03", 5) == 0)
    stAidList->KernType = PAYWAVE;
  else if (memcmp(stAidList->AID, "\xA0\x00\x00\x00\x04", 5) == 0)
    stAidList->KernType = PAYPASS;
  else if (memcmp(stAidList->AID, "\xA0\x00\x00\x03\x33", 5) == 0)
    stAidList->KernType = QPBOC;
  else if (memcmp(stAidList->AID, "\xA0\x00\x00\x00\x25", 5) == 0)
    stAidList->KernType = AMERICAEXPRESS;
  else if (memcmp(stAidList->AID, "\xA0\x00\x00\x00\x65", 5) == 0)
    stAidList->KernType = JCB;
  // else if(memcmp(stAidList->AID, "\xA0\x00\x00\x00\x05", 5) == 0) 
  // stAidList->KernType = PAYPASS; //Meastro
}

static void EMV_Aid2AidParam(const ST_AID * stAid, ST_AIDPARAM * stAidParam)
{
  stAidParam->TargetPer = stAid->TargetPer[0];             
  stAidParam->MaxTargetPer = stAid->MaxTargetPer[0];       
  stAidParam->FloorLimitCheck = stAid->FloorLimitCheck[0]; 
  memcpy(stAidParam->FloorLimit, stAid->FloorLimit, 4); 
  memcpy(stAidParam->Threshold, stAid->Threshold, 4);   
  memcpy(stAidParam->TACDenial, stAid->TACDenial, 5);   
  memcpy(stAidParam->TACOnline, stAid->TACOnline, 5);   
  memcpy(stAidParam->TACDefault, stAid->TACDefault, 5); 
  stAidParam->dDOLLen = stAid->dDOLLen[0];                 
  memcpy(stAidParam->dDOL, stAid->dDOL, 32);            
  memcpy(stAidParam->Version, stAid->Version, 2);      
  
  if(EMV_GetDebug()) {
	  DBG_STR("stAidParam->TargetPer=%d", stAidParam->TargetPer);
	  DBG_STR("stAidParam->MaxTargetPer=%d", stAidParam->MaxTargetPer);
	  DBG_STR("stAidParam->FloorLimitCheck=%d", stAidParam->FloorLimitCheck);
	  DBG_HEX("stAidParam->FloorLimit:", stAidParam->FloorLimit, 4);
	  DBG_HEX("stAidParam->Threshold:", stAidParam->Threshold, 4);
	  DBG_HEX("stAidParam->TACDenial:", stAidParam->TACDenial, 5);
	  DBG_HEX("stAidParam->TACOnline:", stAidParam->TACOnline, 5);
	  DBG_HEX("stAidParam->TACDefault:", stAidParam->TACDefault, 5);
	  DBG_HEX("stAidParam->dDOL:", stAidParam->dDOL, stAidParam->dDOLLen);
	  DBG_HEX("stAidParam->Version:", stAidParam->Version, 2);
  }
}

static void EMV_Aid2PreParam(const ST_AID * stAid, ST_PREPARAM * stPreParam)
{
  stPreParam->AidLen = stAid->AIDLen[0];
  memcpy(stPreParam->AID, stAid->AID, stAid->AIDLen[0]);
  
  stPreParam->IsExistCVMLmt = stAid->IsExistCVMLmt[0];
  memcpy(stPreParam->CVMLmt, stAid->CVMLmt, 6);
  stPreParam->IsExistTermClssLmt = stAid->IsExistTermClssLmt[0];
  memcpy(stPreParam->TermClssLmt, stAid->TermClssLmt, 6);
  
  if(stPreParam->IsExistTermClssLmt == 0) {
  	stPreParam->IsExistTermClssLmt = 1;
	memcpy(stPreParam->TermClssLmt, "\x99\x99\x99\x99\x99\x99", 6);
  }
  stPreParam->IsExistTermClssOfflineFloorLmt = stAid->IsExistTermClssOfflineFloorLmt[0];
  memcpy(stPreParam->TermClssOfflineFloorLmt, stAid->TermClssOfflineFloorLmt, 6);
  stPreParam->IsExistTermOfflineFloorLmt = stAid->IsExistTermOfflineFloorLmt[0];
  memcpy(stPreParam->TermOfflineFloorLmt, stAid->TermOfflineFloorLmt, 6);
  stPreParam->IsTermCheckStatus = 1;

  if(EMV_GetDebug()) {
	  DBG_HEX("stPreParam->AID:", stPreParam->AID, stPreParam->AidLen);
	  DBG_STR("stPreParam->IsExistCVMLmt=%d\r\n", stPreParam->IsExistCVMLmt);
	  DBG_HEX("stPreParam->CVMLmt:", stPreParam->CVMLmt, 6);
	  DBG_STR("stPreParam->IsExistTermClssLmt=%d\r\n", stPreParam->IsExistTermClssLmt);
	  DBG_HEX("stPreParam->TermClssLmt:", stPreParam->TermClssLmt, 6);
	  DBG_STR("stPreParam->IsExistTermClssOfflineFloorLmt=%d\r\n", stPreParam->IsExistTermClssOfflineFloorLmt);
	  DBG_HEX("stPreParam->TermClssOfflineFloorLmt:", stPreParam->TermClssOfflineFloorLmt, 6);
	  DBG_STR("stPreParam->IsExistTermOfflineFloorLmt=%d\r\n", stPreParam->IsExistTermOfflineFloorLmt);
	  DBG_HEX("stPreParam->TermOfflineFloorLmt:", stPreParam->TermOfflineFloorLmt, 6);
  }
}

static void EMV_Trans2AidParam(const ST_TRANS * stTrans, ST_AIDPARAM * stAidParam)
{
  memcpy(stAidParam->TransCurrCode, stTrans->TransCurrCode, 2);
  stAidParam->TransCurrExp = stTrans->TransCurrExp[0];
  memcpy(stAidParam->ReferCurrCode, stTrans->ReferCurrCode, 2);
  stAidParam->ReferCurrExp = stTrans->ReferCurrExp[0];
  memcpy(stAidParam->ReferCurrCon, stTrans->ReferCurrCon, 4);
}

static int EMV_Trans2TermParam(const ST_TRANS * stTrans, ST_TERMPARAM * stTerm)
{
  stTerm->ForceOnline = stTrans->ForceOnline[0];         
  memcpy(stTerm->TTQ, stTrans->TTQ, 4);                  
  stTerm->IsReadLogInCard = stTrans->IsReadLogInCard[0]; 
}

static int EMV_LoadTermParam2Kernal()
{
  ST_PARAM_EXTEND stParam;
  ST_TERMPARAM stTerm;

  memset((char *)&stTerm, 0, sizeof(ST_TERMPARAM));
  EMV_Default2TermParam(EMV_GetDefault(), &stTerm);
  EMV_Trans2TermParam(EMV_GetTrans(), &stTerm);

  if(EMV_GetDebug()) {
	  DBG_HEX("CountryCode = ", stTerm.CountryCode, 2);
	  DBG_STR("TerminalType = %d", stTerm.TerminalType);
	  DBG_HEX("Capability = ", stTerm.Capability, 3);
	  DBG_HEX("AddCapability = ", stTerm.AddCapability, 5);
	  DBG_HEX("TermAIP = ", stTerm.TermAIP, 2);
	  DBG_HEX("IFDSn = ", stTerm.IFDSn, 8);
	  DBG_HEX("TTQ = ", stTerm.TTQ, 4);
	  DBG_STR("BypassAllFlg = %d", stTerm.BypassAllFlg);
	  DBG_STR("BypassPin = %d", stTerm.BypassPin);
	  DBG_STR("ECTSIFlg = %d", stTerm.ECTSIFlg);
	  DBG_STR("ECTSIVal = %d", stTerm.ECTSIVal);
	  DBG_STR("ECTTLFlg = %d", stTerm.ECTTLFlg);
	  DBG_HEX("ECTTLVal = %d", stTerm.ECTTLVal, 6);
	  DBG_STR("ForceOnline = %d", stTerm.ForceOnline);
  }

  stParam.tag = TYPE_TERMPARAM;
  stParam.len = sizeof(ST_TERMPARAM);
  stParam.value = &stTerm;

  return EMVL2BaseParamOperate(OP_UPDATE, &stParam);
}

static int EMV_LoadAidList2Kernel()
{
	int num;
	int idx;

	num = EMV_GetAIDNumber();
	if(num <= 0) {
		return EMV_RET_ERR_AID;
	}
	
	for(idx = 0; idx < num; idx ++) {		
		int iRet = 0;
		ST_AID stAID;
		ST_TERMAIDLIST AidItem;
		ST_PREPARAM PreParam;
		
		ST_PARAM_EXTEND ExTlv;
		iRet = EMV_GetAIDByNo(idx, &stAID);
		if(iRet < 0) {
			return EMV_RET_ERR_AID;
		}
		
		memset((char *)&AidItem, 0, sizeof(ST_TERMAIDLIST));
		EMV_Aid2AidList(&stAID, &AidItem);
		
		ExTlv.tag = TYPE_TERMAIDLIST;
		ExTlv.len = sizeof(ST_TERMAIDLIST);
		ExTlv.value = &AidItem;
		iRet = EMVL2BaseParamOperate(OP_UPDATE, &ExTlv);

 		if(EMV_GetDebug()) {	
			DBG_STR("EMVL2BaseParamOperate = %d", iRet);
			DBG_HEX("AidItem.AID", AidItem.AID, AidItem.AidLen);
			DBG_STR("AidItem.SelFlag = %d", AidItem.SelFlag);
			DBG_STR("AidItem.KernType = %d", AidItem.KernType);
			DBG_STR("------");
  		}
		// add AID ctls prm
		memset((char *)&PreParam, 0, sizeof(ST_PREPARAM));
		EMV_Aid2PreParam(&stAID, &PreParam);
		
		ExTlv.tag = TYPE_PREPARAM;
		ExTlv.len = sizeof(ST_PREPARAM);
		ExTlv.value = &PreParam;
		iRet = EMVL2BaseParamOperate(OP_UPDATE, &ExTlv);
		
 		if(EMV_GetDebug()) {
			DBG_STR("EMVL2BaseParamOperate = %d", iRet);
			DBG_HEX("PreParam.AID", PreParam.AID, PreParam.AidLen);
			DBG_STR("PreParam.IsTermCheckStatus = %d", PreParam.IsTermCheckStatus);
			DBG_STR("PreParam.IsExistCVMLmt = %d", PreParam.IsExistCVMLmt);
			DBG_HEX("PreParam.CVMLmt", PreParam.CVMLmt, 6);
			DBG_STR("PreParam.IsExistTermClssLmt = %d", PreParam.IsExistTermClssLmt);
			DBG_HEX("PreParam.TermClssLmt", PreParam.TermClssLmt, 6);
			DBG_STR("PreParam.IsExistTermClssOfflineFloorLmt = %d", PreParam.IsExistTermClssOfflineFloorLmt);
			DBG_HEX("PreParam.TermClssOfflineFloorLmt", PreParam.TermClssOfflineFloorLmt, 6);
			DBG_STR("PreParam.IsExistTermOfflineFloorLmt = %d", PreParam.IsExistTermOfflineFloorLmt);
			DBG_HEX("PreParam.TermOfflineFloorLmt", PreParam.TermOfflineFloorLmt, 6);
			DBG_STR("------");
 		}
	}
	
	return EMV_RET_OK;
}

static int EMV_LoadAidParam2Kernel(byte * Aid, byte AidLen)
{
  int iRet = 0;
  ST_AID AppParam;
  ST_AID_COND AidCond;
  ST_PARAM_EXTEND stParam;
  ST_AIDPARAM AidParam;

  memset((char *)&AppParam, 0, sizeof(ST_AID));
  AidCond.AIDLen = AidLen;
  memcpy(AidCond.AID, Aid, AidLen);
  iRet = EMV_GetAID(&AidCond, &AppParam);
  if (iRet < 0){
  	if(EMV_GetDebug()) {
		DBG_HEX("AID = ", Aid, AidLen);
 		DBG_STR("EMV_GetAID=%d", iRet);
  	}
    return iRet;
  }

  memset((char *)&AidParam, 0, sizeof(ST_AIDPARAM));
  EMV_Default2AidParam(EMV_GetDefault(), &AidParam);
  EMV_Aid2AidParam(&AppParam, &AidParam);
  EMV_Trans2AidParam(EMV_GetTrans(), &AidParam);

  stParam.tag = TYPE_AIDPARAM;
  stParam.len = sizeof(ST_AIDPARAM);
  stParam.value = &AidParam;
  iRet = EMVL2BaseParamOperate(OP_UPDATE, &stParam);
  if(iRet) {
  	if(EMV_GetDebug()) {
		DBG_HEX("AID = ", Aid, AidLen);
  		DBG_STR("EMVL2BaseParamOperate=%d", iRet);
  	}
  }

  return iRet;
}

static int EMV_LoadCAPK2Kernel()
{
  int iRet = 0;
  u8 Aid[32];
  u32 AidLen = 0;
  u8 Index[32];
  u32 IndexLen = 0;
  int len;
  char tmp[50];
  ST_CAPK_COND stCond;
  ST_CAPK stCapk;
  ST_PARAM_EXTEND stParam;

  memset((char *)tmp, 0, sizeof(tmp));
  len = sizeof(tmp);
  iRet = EMV_GetKernalData(0x84, tmp, &len);
  if (iRet < 0){
    return ENDAPPLICATION_DATA_ERR;
  }
  
  memcpy(stCond.RID, tmp, 5);

  memset((char *)tmp, 0, sizeof(tmp));
  len = sizeof(tmp);
  iRet = EMV_GetKernalData(0x8F, tmp, &len);
  if (iRet < 0){
    return ENDAPPLICATION_DATA_ERR;
  }
  
  stCond.Index = tmp[0];
  
  memset((char *)&stCapk, 0, sizeof(ST_CAPK));
  iRet = EMV_GetCAPK(&stCond, &stCapk);
  if (iRet < 0) {
  	if(EMV_GetDebug()) {
		DBG_HEX("RID = ", stCond.RID, 5);
  		DBG_STR("IDX =%d", stCond.Index);
  		DBG_STR("EMV_GetCAPK=%d", iRet);
  	}
    return ENDAPPLICATION_DATA_ERR;
  }

  stParam.tag = TYPE_CAPK;
  stParam.len = sizeof(ST_CAPK);
  stParam.value = &stCapk;
  iRet = EMVL2BaseParamOperate(OP_UPDATE, &stParam);
  if(iRet) {
	 if(EMV_GetDebug()) {
		DBG_HEX("RID = ", stCond.RID, 5);
  		DBG_STR("IDX =%d", stCond.Index);
		DBG_STR("EMVL2BaseParamOperate=%d", iRet);
	 }
  }

  return CONTINUE;
}

int EMV_SetKernalData(ushort usTag, byte *val, int valLen)
{
    int inLen = 0,iRet = 0,kerneltype=EMV;
    unsigned char buff[512] = {0};
    uint update_cmd = OP_UPDATE;

	if(usTag & 0xFF00) {
		buff[0] = (byte)(usTag >> 8);
		buff[1] = (byte)(usTag & 0xFF);
		inLen += 2;
	} else {
		buff[0] = (byte)(usTag & 0xFF);
		inLen += 1;
	}
	
    if(valLen < 128) {
        buff[inLen++] = (unsigned char)valLen;
    } else if(valLen <= 255){
        buff[inLen++] = 0x81;
        buff[inLen++] = (unsigned char)valLen;
    } else {
        return EMV_RET_PARAM;
    }
    memcpy(&buff[inLen], val, valLen);
    inLen += valLen;

    kerneltype = EMV_GetCurKernelType();	
    if (kerneltype == PAYPASS) update_cmd = OP_UPDATE_PAYPASS;
    else if(kerneltype==PAYWAVE /*|| kerneltype==QPBOC*/) update_cmd = OP_UPDATE_PAYWAVE;
    else update_cmd = OP_UPDATE;

    iRet = EMVL2BaseTLVOperate(update_cmd, 0, NULL, &inLen, buff);
	if(EMV_GetDebug()){
    	DBG_STR("kerneltype) = %d\r\n", kerneltype);
		DBG_STR("EMVL2BaseTLVOperate(%d) = %d", update_cmd, iRet);
    	DBG_HEX("UPDATETLV:", buff, inLen);
	}
    if(iRet==0){
		return EMV_RET_OK;
    }
	
    return EMV_RET_FAIL;
}

int EMV_GetKernalData(ushort usTag, byte * val, uint *len)
{
    int iRet = 0, kerneltype=EMV;
    uint query_cmd = OP_QUERY;
	byte buff[2+1];
	byte tagLen;
	
    kerneltype = EMV_GetCurKernelType();
    if (kerneltype==PAYPASS) query_cmd = OP_QUERY_PAYPASS;
    else if(kerneltype==PAYWAVE /*|| kerneltype==QPBOC*/) query_cmd = OP_QUERY_PAYWAVE;
    else query_cmd = OP_QUERY;

	if(usTag & 0xFF00) {
		buff[0] = (byte)(usTag >> 8);
		buff[1] = (byte)(usTag & 0xFF);
		tagLen =2;
	} else {
		buff[0] = (byte)(usTag & 0xFF);
		tagLen = 1;
	}

    iRet =  EMVL2BaseTLVOperate(query_cmd, tagLen, buff, len, val);
	if(EMV_GetDebug()){
    	DBG_STR("kerneltype) = %d\r\n", kerneltype);
		DBG_STR("EMVL2BaseTLVOperate(%d) = %d", query_cmd, iRet);
		DBG_HEX("GETTLV:", (byte*)buff, tagLen);
	}
    if(iRet == 0 || *len > 0) {
		if(tagLen == 2) {
			tagLen = (val[2] & 0x80);
			if(*len < tagLen + 3) {
				return EMV_RET_FAIL;
			}
			*len -= (tagLen + 3);
			memmove(val, val + tagLen + 3, *len);
		} else {
			tagLen = (val[1] & 0x80);
			if(*len < tagLen + 2) {
				return EMV_RET_FAIL;
			}
			*len -= (tagLen + 2);
			memmove(val, val + tagLen + 2, *len);
		}
		
		if(EMV_GetDebug()){
		    DBG_HEX("DATA:", val, *len);
		}
		
		return EMV_RET_OK;
	}
	*len = 0;
	
    return EMV_RET_FAIL;
}

int EMV_GetKernalTLVs(const ushort * tagList, uint num, byte * data, uint * dataLen)
{
	byte idx;
	byte off;
	byte tags[100];
	int iRet;
	int kerneltype=EMV;
    uint query_cmd = OP_QUERY;
	
	memset(tags, 0, sizeof(tags));
	for(idx = 0, off = 0; idx < num; idx ++) {
		byte tag[2];
		Utils_Int2Hex(tagList[idx], tag, 2);
		if(tag[0] == 0) {
			memcpy(tags+off, tag + 1, 1);
			off += 1;
		} else {
			memcpy(tags+off, tag, 2);
			off += 2;
		}
	}
	
    kerneltype = EMV_GetCurKernelType();
    if (kerneltype==PAYPASS) query_cmd = OP_QUERY_PAYPASS;
    else if(kerneltype==PAYWAVE /*|| kerneltype==QPBOC*/) query_cmd = OP_QUERY_PAYWAVE;
    else query_cmd = OP_QUERY;

    iRet =  EMVL2BaseTLVOperate(query_cmd, off, tags, dataLen, data);
    if(EMV_GetDebug()){
    	DBG_STR("kerneltype) = %d\r\n", kerneltype);
		DBG_STR("EMVL2BaseTLVOperate(%d) = %d", query_cmd, iRet);
	}
	if(iRet == 0) {
		if(EMV_GetDebug()){
		    DBG_HEX("GETTLV:", (byte*)tags, off);
		    DBG_HEX("DATA:", data, *dataLen);
		}
		return EMV_RET_OK;
	}
	
	return iRet;
}

int EMV_GetCardData(ushort usTag, byte *val, int *valLen)
{
	byte iRet;
	byte p1, p2;
	C_APDU req;
	R_APDU rsp;
	
	p1 = (byte)(usTag >> 8);
	p2 = (byte)(usTag & 0xFF);
	req.Command[0] = 0x80;
	req.Command[1] = 0xca;
	req.Command[2] = p1;
	req.Command[3] = p2;
	req.Lc = 0;
	memset(req.indata, 0x00, sizeof(req.indata));
	req.Le = 256;
	memset(&rsp, '\x00', sizeof(R_APDU));
	
	if(EMV_GetCurKernelType() == EMV) {
		iRet = EmvL2ScExchange("SCC", &req, &rsp);
	} else {
		iRet = EmvL2ScExchange("SCCL", &req, &rsp);
	}
	
	if(iRet != 0) {
		if(EMV_GetDebug()){
			DBG_STR("GET CARD DATA %04X = %d", usTag, iRet);
		}
		return EMV_RET_FAIL;
	}
	
	if(rsp.SWA != 0x90 ||rsp.SWB != 0x00) {
		if(EMV_GetDebug()){
			DBG_STR("GET CARD DATA %04X = %02X%02X", usTag, rsp.SWA, rsp.SWB);
		}
		return EMV_RET_FAIL;
	}
	
	memcpy(val, rsp.outdata+3, rsp.outlen-3);
	*valLen = rsp.outlen-3;
	
	if(EMV_GetDebug()){
		DBG_STR("GET CARD DATA %04X = %d", usTag, iRet);
		DBG_HEX("DATA:", val, *valLen);
	}
	
	return EMV_RET_OK;
}

int EMV_LoadParam2Kernel()
{
  int iRet = 0;
  ST_PARAM_EXTEND stParam;


  //clear all kernal file
  stParam.tag = TYPE_ALL;
  EMVL2BaseParamOperate(OP_CLEAR, &stParam);

  // load default prm
  iRet = EMV_ReadDefault(EMV_GetDefault());
  if(iRet) {
  	return iRet;
  }
  
  // load aid 
  iRet = EMV_LoadAidList2Kernel();
  if(iRet) {
	 return iRet;
  }

  return EMV_RET_OK;
}

void SetConfig_Fix(void)
{
    int kernel_type;
    kernel_type = EMVL2GetKernelType();
	if(EMV_GetDebug()) 
		DBG_STR("kernel_type:%d", kernel_type);
    s_PaypassFixCfg();
    s_AE_FixCfg();
}


void SetConfig_NotFix(void)
{
    int kernel_type;
    kernel_type = EMVL2GetKernelType();
	if(EMV_GetDebug()) 
		DBG_STR("kernel_type:%d", kernel_type);
    if(kernel_type==PAYPASS) s_PaypassNotFixCfg();
    if(kernel_type==AMERICAEXPRESS) s_AE_NotFixCfg();
}

int EMV_InitCore()
{
  int iRet = 0;

  iRet = EMVL2CoreInit();
  if(EMV_GetDebug()) {
 	DBG_STR("EMVL2CoreInit=%d", iRet);
  }
  // iRet = EMV_LoadParam2Kernel();
  // if(EMV_GetDebug()) {
	// DBG_STR("EMV_LoadParam2Kernel=%d", iRet);
  // }

  return iRet;
}

int EMV_InitTransaction(int transType)
{
  int iRet = 0;
  ST_TRANS * pxTrans = EMV_GetTrans();

  iRet = EMV_GetTransParameter(transType, pxTrans);
  if(iRet) {
  	return iRet;
  }

  EMV_SetCurKernelType(EMV);

  iRet = EMV_LoadTermParam2Kernal();
  
  iRet = EMVL2BaseTLVOperate(OP_CLEAR, 0, NULL, NULL, NULL);

  EMV_SetKernalData(0x9C, &pxTrans->Type[0], 1);

  EMV_SetKernalData(0x9F53, &pxTrans->Category[0], 2);

  EMV_SetKernalData(0x9F41, &pxTrans->TSN[0], 4);

  EMV_SetKernalData(0x9A, &pxTrans->Date[0], 3);
  EMV_SetKernalData(0x9F21, &pxTrans->Time[0], 3);

  EMV_SetKernalData(0x9F02, &pxTrans->Amt[0], 6);

  EMV_SetKernalData(0x9F03, &pxTrans->OtherAmt[0], 6);

  EMV_SetKernalData(0x9F66, &pxTrans->TTQ[0], 4);

#ifdef CTLS_KERNAL_ENABLE  
  //set fix config
  SetConfig_Fix();
#endif
  return EMV_RET_OK;
}

// ???????????
int EMV_CLSSPreProcess()
{
    int iRet = 0;
	
PREPROCESS:
    // ???????????
    iRet = EMVL2PreProcess();
    if (iRet < 0)
    {
    	if(EMV_GetDebug()) {
			DBG_STR("EMVL2PreProcess = %d", iRet);
		}
      return iRet;
    }


    return iRet;
}

static int EMV_CVMProcess(unsigned char *cvmtype)
{
  int iRet;
  int cvmstep;
  int cvmresult;
  int len;
  int type;
  byte pinTimes[10];
  byte certType[10];
  byte certId[50];

  cvmstep = 0;
  cvmresult = 0;
  while (1)
  {
    iRet = EMVL2CVMProcess(cvmstep, cvmresult, cvmtype);
	if(EMV_GetDebug()) {
		DBG_STR("EMVL2CVMProcess(%d, %d, %d) = %d\r\n", cvmstep, cvmresult, *cvmtype, iRet);
	}
    if (iRet < 0)
    { 
      return iRet;
    }
    if (iRet != CVM_STEP_NEXT)
    { 
      return CONTINUE;
    }
    cvmstep = CVM_STEP_NEXT;

  	len = 0;
  RETRY_PIN:
    type = *cvmtype;
    switch(type)
    {
	    case CVM_ONLINE_PIN:
		  if(m_IsOnlinePin != 1) {
		  	cvmresult = CVM_SUCSEC;
		  	break;
		  }
	      iRet = EMV_InputOnlinePIN();
		  if(iRet == SEC_RET_OK ) {
		  	cvmresult = CVM_SUCSEC;
		  }
		  else if(iRet == SEC_ERR_NOPIN) {
		  	cvmresult = CVM_PINBYPASS;
		  }
		  else if(iRet == SEC_ERR_INPUT_CANCEL) {
		  	cvmresult = CVM_PINCANCEL;
			return EMV_RET_USER_ABORT; 
		  } else {
		  	cvmresult = CVM_PINPADERROR;
		  }
	      break;
	    case CVM_CERT:
	 	  len = sizeof(certType);
	      iRet = EMV_GetKernalData(0x9F62, certType, &len);
	 	  len = sizeof(certId);
	      iRet = EMV_GetKernalData(0x9F61, certId, &len);
	      iRet = EMV_CheckId(certType[0], certId);
		  if(iRet == EMV_RET_OK) {
		  	cvmresult = CVM_SUCSEC;
		  } else {
		  	cvmresult = CVM_CERT_FAIL;
		  }
	      break;
	    default:
		  cvmresult = CVM_SUCSEC;
	      break;
    }
	
	if(EMV_GetDebug()) {
	  DBG_STR("cvmresult = %d", cvmresult);
	  DBG_STR("cvmtype = %d", *cvmtype);
	}

    if (cvmresult == CVM_SUCSEC && (*cvmtype == CVM_PLAIN_PIN || *cvmtype == CVM_PPIN_SIG || *cvmtype == CVM_ENCIPH_PIN || *cvmtype == CVM_EPIN_SIG))
    { 
      iRet = EMVL2PINVerify();
	  if(EMV_GetDebug()) {
     	DBG_STR("EMVL2PINVerify = %d", iRet);
	  }
      if (iRet == EMV_REENTER_PIN || iRet == EMV_REENTER_PIN_LAST)
      {
        goto RETRY_PIN;
      }
    }
    else if((cvmresult == CVM_SUCSEC || cvmresult == CVM_PINBYPASS) && *cvmtype == CVM_ONLINE_PIN)
	{
        continue;
    }
  }

  return ENDAPPLICATION_DATA_ERR;
}

#define MAX_CANDLIST_NUM 5 

int EMV_EMVProcess_Simple()
{
    int iRet,i;
	int isRetry;
    u32 CandListNum;
    ST_CANDLIST CandList[MAX_CANDLIST_NUM];
    ST_CANDLIST finalApp;

	isRetry = 0;
    CandListNum = 0;
    memset((char *)CandList, 0, sizeof(CandList));
    memset((char *)&finalApp, 0, sizeof(finalApp));
    iRet = EMVL2AppSel(2, &CandListNum, CandList);
    if (iRet < 0){
		if(EMV_GetDebug()) {
	   		DBG_STR("EMVL2AppSel = %d", iRet);
		}
        goto END_PROC;
    }

FINAL_SELECT:
    if(EMV_GetDebug()) {
   		DBG_STR("CandListNum = %d", CandListNum);
	}
    if (CandListNum == 0)
    {
        iRet = EmvGetCandListBySeqNumber(0, &finalApp);
        if (iRet < 0) {
			if(EMV_GetDebug()) {
		   		DBG_STR("EmvGetCandListBySeqNumber = %d", iRet);
			}
          goto END_PROC;
        }
		if(EMV_GetDebug()) {
			DBG_HEX("finalAPP:", (char *)&finalApp, sizeof(ST_CANDLIST));
		}
    }
    else 
    {
        if (CandListNum == 1 && (0==(CandList[0].Priority&0x80))) {
            iRet = 0;
        }
        else {/**Mult-app, cardholder App selsetion**/
            iRet = EMV_SelectApp(isRetry, CandListNum, CandList);
			if(EMV_GetDebug()) {
				DBG_STR("EMV_SelectApp = %d", iRet);
			}
            if (iRet < 0) iRet = 0;                
			isRetry = 1;
        }

        memcpy((char *)&finalApp, (char *)&CandList[iRet], sizeof(ST_CANDLIST));
       
        if (iRet < MAX_CANDLIST_NUM - 1)
        {
            for(i=iRet+1; i<CandListNum; i++ ) {
                CandList[i].Index--;
            }
            memmove((char *)&CandList[iRet], (char *)&CandList[iRet + 1],
            sizeof(ST_CANDLIST) * (CandListNum - 1));
        }
        CandListNum -= 1;
        memset((char *)&CandList[MAX_CANDLIST_NUM - 1], 0, sizeof(ST_CANDLIST));
    }
	
	if(EMV_GetDebug()) {
 		DBG_STR("finalApp.Index = %d", finalApp.Index);
		DBG_HEX("finalAPP:", (char *)&finalApp, sizeof(ST_CANDLIST));
	}

    iRet = EMV_LoadAidParam2Kernel(finalApp.AID, finalApp.AidLen);
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMV_LoadAidParam2Kernel = %d", iRet);
		}
        goto END_PROC;
    }

    iRet = EMVL2AppFinalSel(finalApp.Index);
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMVL2AppFinalSel(%d) = %d", finalApp.Index, iRet);
		}
        goto END_PROC;
    }

    iRet = EMVL2InitApp();
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMVL2InitApp = %d", iRet);
		}
        goto END_PROC;
    }

    iRet = EMVL2ReadAppData();
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMVL2ReadAppData = %d", iRet);
		}
        goto END_PROC;
    }
	
	// iRet = EMV_RET_OK;
	
END_PROC:
    if (iRet == SELECT_NEXT_APP || 
        iRet == ENDAPPLICATION_APP_BLOCK ||
        iRet == SELECT_NEXTAPP_MAXLIMIT_EXCEED )
    {
		if(EMV_GetDebug()) {
        	DBG_STR("iRet = %d", iRet);
		}
        goto FINAL_SELECT;
    }
		
	if(iRet >= 0) {
    	EMV_ReadCardData();
	}

    return iRet;
}

// type =0 before GPO pxPARAM = NULL
// type =1 after ReadAppData PxPARAM = NULL
// return 0 -CONTINUE, OTHER - ABORT
extern int EMV_Callback(byte type, void * pxPARAM);

int EMV_EMVProcess()
{
    int iRet;
    u8 cvmtype;

	iRet = EMV_EMVProcess_Simple();
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMV_EMVProcess = %d", iRet);
		}
	  goto END_PROC;
    }
	
	iRet = EMV_Callback(1, NULL);
	if (iRet) {
		if(EMV_GetDebug()) {
			DBG_STR("EMV_Callback after EMVL2ReadAppData = %d", iRet);
		}
		goto END_PROC;
	}
    iRet = EMV_LoadCAPK2Kernel();
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMV_LoadCAPK2Kernel = %d", iRet);
		}
    }

    iRet = EMVL2DataAuth();
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMVL2DataAuth = %d", iRet);
		}
        goto END_PROC;
    }

    iRet = EMVL2ProcRestric();
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMVL2ProcRestric = %d", iRet);
		}
        goto END_PROC;
    }

    cvmtype = 0;
    iRet = EMV_CVMProcess(&cvmtype);
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMV_CVMProcess = %d", iRet);
		}
        goto END_PROC;
    }
	
	if(EMV_GetDebug()) {
    	DBG_STR("cvmtype = %d", cvmtype);
	}

    iRet = EMVL2TermRiskManage();
    if (iRet < 0) {
		if(EMV_GetDebug()) {
			DBG_STR("EMVL2TermRiskManage = %d", iRet);
		}
        goto END_PROC;
    }

    iRet = EMVL2TermActAnalysis();
	if(EMV_GetDebug()) {
		DBG_STR("EMVL2TermActAnalysis = %d", iRet);
	}
	
END_PROC:

    return iRet;
}


int Paypass_CVMProcess()
{
    unsigned char cvmtype;
    int iRet = 0;
    u8 outcome[8];
	
    iRet = Database_GetTlvData(DF8129_OUTCOME_MC,outcome,NULL);
    if(iRet!=0) return iRet;

    cvmtype = outcome[3];
	if(EMV_GetDebug()) 
    	DBG_STR("cvm:%d ", cvmtype);

    switch(cvmtype)
    {
        case OUTCOME_CVM_NO_CVM: return CONTINUE;
        case OUTCOME_CVM_CONFIRM_CODE_VERIFY: return CONTINUE;
        case OUTCOME_CVM_SIG: return CONTINUE;

        case OUTCOME_CVM_ONLINE_PIN:
			if(EMV_GetDebug()) 
            	DBG_STR("\r\n Please Input online PIN! \r\n");
            iRet = EMV_InputOnlinePIN();
        break;
    }

    return iRet;
}

static int EMV_PayPassProcess()
{
  int iRet;
  u8 cvmtype = 0;

  // ??ио????
  iRet = EMVL2InitApp();
  if (iRet){
	if(EMV_GetDebug()) 
    	DBG_STR("EMVL2InitApp = %d", iRet);
    goto END_PROC;
  }

  // ?????????
  iRet = EMVL2ReadAppData();
  if (iRet){
	if(EMV_GetDebug()) 
    	DBG_STR("EMVL2ReadAppData = %d", iRet);
    goto END_PROC;
  }

  // ????CAPK
  iRet = EMV_LoadCAPK2Kernel();
  if (iRet < 0) {
	if(EMV_GetDebug()) 
    	DBG_STR("EMV_LoadCAPK2Kernel = %d", iRet);
  }

  // ??????????
  iRet = EMVL2TermActAnalysis();
  if(EMV_GetDebug()) 
  	DBG_STR("EMVL2TermActAnalysis = %d", iRet);
  if(iRet==APPROVE || iRet== ONLINE_REQUEST){
    Paypass_CVMProcess();
  }

END_PROC:
    if(iRet==TRY_AGAIN) {
        //EMV_InitTransaction();
    }
  // switch (iRet)
  // {
  // case ONLINE_REQUEST:                // ????????
  // case APPROVE:                       // ???????
  // case DECLINED:                      // ??????
  // case TRY_AGAIN:                     // ???????
  // case TRY_ANOTHER_INTERFACE:         // ?????????????(???/????)????
  // case TRY_ANOTHER_INTERFACE_PREPROC: // ?????????????(???/????)????
  // case ENDAPPLICATION:                // ??????????
  // default:
  //   break;
  // }

  return iRet;
}

int PayWave_CVMProcess(unsigned char cvmtype)
{
    int iRet = 0;
	
	if(EMV_GetDebug()) 
    	DBG_STR("cvm:%d \r\n", cvmtype);

    switch(cvmtype)
    {
        case CVM_NO_CVM: return CONTINUE;
        case CVM_CONSUMER_DEVICE: return CONTINUE;
        case CVM_SIG: return CONTINUE;

        case CVM_ONLINE_PIN:
        case CVM_PLAIN_PIN:
        case CVM_PPIN_SIG:
        case CVM_ENCIPH_PIN:
        case CVM_EPIN_SIG: 
			if(EMV_GetDebug()) 
            	DBG_STR("\r\n Please Input online PIN! \r\n");
            iRet = EMV_InputOnlinePIN();
        break;
    }

    return iRet;
}

static int EMV_PayWaveProcess()
{
  int iRet = 0;
  int entryMode = 0x07; //qVSDC WAVE
  u32 OnlineResult = 0;
  u32 Scriptlen = 0;
  u8 Script[256] = {0};
  u32 Issuerlen = 0;
  u8 IssuerData[256] = {0};
  u8 cvmtype = 0;

  // ??ио????
  iRet = EMVL2InitApp();
  if (iRet) {
	if(EMV_GetDebug()) 
    	DBG_STR("EMVL2InitApp = %d", iRet);
    goto END_PROC;
  }

  // ?????????
  iRet = EMVL2ReadAppData();
  if (iRet){
	if(EMV_GetDebug()) 
    	DBG_STR("EMVL2ReadAppData = %d", iRet);
    if(iRet == ONLINE_REQUEST) goto WAVE_SUCCESS_PROC;
    goto END_PROC;
  }

  iRet = EMVL2ProcRestric();
  if (iRet){
	if(EMV_GetDebug()) 
    	DBG_STR("EMVL2ProcRestric = %d", iRet);
    if(iRet == ONLINE_REQUEST) goto WAVE_SUCCESS_PROC;
    goto END_PROC;
  }

  // ????CAPK
  iRet = EMV_LoadCAPK2Kernel();
  if (iRet < 0) {
	if(EMV_GetDebug()) 
    	DBG_STR("EMV_LoadCAPK2Kernel = %d", iRet);
    goto END_PROC;
  }

  // ??????????
  iRet = EMVL2DataAuth();
  if (iRet) {
  	if(EMV_GetDebug())
    	DBG_STR("EMVL2DataAuth = %d", iRet);
    goto END_PROC;
  }

WAVE_SUCCESS_PROC:
  iRet = EMVL2CVMProcess(0, 0, &cvmtype);
  if (iRet < 0) {
  	if(EMV_GetDebug())
    	DBG_STR("EMV_LoadCAPK2Kernel = %d", iRet);
    goto END_PROC;
  }
  if(cvmtype>=CVM_PLAIN_PIN && cvmtype<=CVM_EPIN_SIG) {
    iRet = PayWave_CVMProcess(cvmtype);
  }
END_PROC: 
  return iRet;
}

int AE_CVMProcess(unsigned char cvmtype)
{
    int iRet = 0;
	
  	if(EMV_GetDebug())
    	DBG_STR("cvm:%d \r\n", cvmtype);

    switch(cvmtype)
    {
        case CVM_NO_CVM: return CONTINUE;
        case CVM_MOBILE_CVM_AE: return CONTINUE;
        case CVM_SIG: return CONTINUE;

        case CVM_ONLINE_PIN:
			if(EMV_GetDebug())
            	DBG_STR("\r\n Please Input online PIN! \r\n");
            iRet = EMV_InputOnlinePIN();
        break;
    }

    return iRet;
}

static int EMV_AE_Process()
{
    int iRet;
    u8 cvmtype = 0, tvr_buf[5];
    u16 len;
    static u8 s_tryagain_cvm_flag = 0;

    if(s_tryagain_cvm_flag) {
        Database_SetTlv(TAG_DF8146_TRANS_RESTART_AE, "\x01", 1);
        s_tryagain_cvm_flag = 0;
    }

    iRet = EMVL2InitApp();
    if (iRet){
		if(EMV_GetDebug())
        	DBG_STR("EMVL2InitApp = %d", iRet);
        goto END_PROC;
    }

    iRet = EMVL2ReadAppData();
    if (iRet){
		if(EMV_GetDebug())
        	DBG_STR("EMVL2ReadAppData = %d", iRet);
        if(iRet == ONLINE_REQUEST) goto AE_SUCCESS_PROC;
        goto END_PROC;
    }

    iRet = EMVL2ProcRestric();
    if (iRet){
		if(EMV_GetDebug())
        	DBG_STR("EMVL2ProcRestric = %d", iRet);
        if(iRet == ONLINE_REQUEST)goto AE_SUCCESS_PROC;
        goto END_PROC;
    }

    iRet = EMV_LoadCAPK2Kernel();
    if (iRet < 0) {
		if(EMV_GetDebug())
        	DBG_STR("EMV_LoadCAPK2Kernel = %d", iRet);
        goto END_PROC;
    }

    iRet = EMVL2DataAuth();
    if (iRet) {
		if(EMV_GetDebug())
        	DBG_STR("EMVL2DataAuth = %d", iRet);
        goto END_PROC;
    }

AE_SUCCESS_PROC:
    iRet = EMVL2CVMProcess(0, 0, &cvmtype);
	if(EMV_GetDebug())
    	DBG_STR("EMVL2CVMProcess return: %d, cvm_type:%d", iRet, cvmtype);
    if (iRet) {
        if(iRet == TRY_AGAIN_CVM_AE) {
			if(EMV_GetDebug())
            	DBG_STR("\r\nSee Phone for Instructions!\r\n Present Card Again!\r\n");
            s_tryagain_cvm_flag = 1;
        }
        goto END_PROC;
    }

    iRet = EMVL2TermRiskManage();
    if (iRet) {
		if(EMV_GetDebug())
        	DBG_STR("EMVL2TermRiskManage return: %d", iRet);
        goto END_PROC;
    }

    iRet = EMVL2TermActAnalysis();
    if (iRet) {
		if(EMV_GetDebug())
        	DBG_STR("EMVL2TermActAnalysis return: %d", iRet);
        if(iRet == TRYAGAIN_CMD_SWAB_6984){
			if(EMV_GetDebug())
            	DBG_STR("\r\nSee Phone for Instructions!\r\n Present Card Again!\r\n");
            s_tryagain_cvm_flag = 1;
        }
        goto END_PROC;
    }
END_PROC: 
    if((cvmtype!=CVM_FAIL_CVM) && (cvmtype!=CVM_NO_CVM)) {
        iRet = AE_CVMProcess(cvmtype);
    }
  return iRet;
}

// ctls simple process
int EMV_CLSSProcess_Simple()
{
  int iRet;
  int finalNo = 0;
  ST_CANDLIST finalApp;
  ST_TRANS * pxTrans = EMV_GetTrans();
    	
  iRet = EMVL2AppSel(1, NULL, NULL);
  if (iRet) {
	 if(EMV_GetDebug()) {
     	DBG_STR("EMVL2AppSel = %d", iRet);
	 }
     goto END_PROC;
  }

FINAL_SELECT:
  // get the first app of the candidate list 
  finalNo = 0;
  memset((char *)&finalApp, 0, sizeof(finalApp));
  iRet = EmvGetCandListBySeqNumber(finalNo, &finalApp);
  if (iRet) {
	if(EMV_GetDebug()) {
     	DBG_STR("EmvGetCandListBySeqNumber = %d", iRet);
	}
    goto END_PROC;
  }
  
  if(EMV_GetDebug()) {
 	DBG_STR("finalApp.Index = %d", finalApp.Index);
 	DBG_HEX("finalApp.AID", finalApp.AID, finalApp.AidLen);
  }

  // load AIDPARAM
  iRet = EMV_LoadAidParam2Kernel(finalApp.AID, finalApp.AidLen);
  if (iRet) {
	if(EMV_GetDebug()) {
     	DBG_STR("EMV_LoadAidParam2Kernel = %d", iRet);
	}
    goto END_PROC;
  }

  // final select
  iRet = EMVL2AppFinalSel(finalApp.Index);
  if (iRet) {
	if(EMV_GetDebug()) {
     	DBG_STR("EMVL2AppFinalSel = %d", iRet);
	}
    goto END_PROC;
  }

#ifdef CTLS_KERNAL_ENABLE  
  SetConfig_NotFix();
  EMV_SetCurKernelType(EMVL2GetKernelType());
  finalApp.KernType = EMV_GetCurKernelType();

  // ????????????и░??????????
  if(EMV_GetDebug())
  	DBG_STR("finalApp.KernType = %d", finalApp.KernType);
  switch (finalApp.KernType)
  {
  case PAYPASS:
    iRet = EMV_PayPassProcess();
	if(EMV_GetDebug())
    	DBG_STR("EMV_PayPassProcess = %d", iRet);
    break;
  case QPBOC:
  case PAYWAVE:
    iRet = EMV_PayWaveProcess();
	if(EMV_GetDebug())
    	DBG_STR("EMV_PayWaveProcess = %d", iRet);
    break;
  case AMERICAEXPRESS:
    iRet = EMV_AE_Process();
	if(EMV_GetDebug())
    	DBG_STR("EMV_AE_Process = %d", iRet);
    break;
  default:
    iRet = KERNEL_NOT_SUPPORT_STEP;
    goto END_PROC;
  }

END_PROC:
  if (iRet == SELECT_NEXT_APP || iRet == SELECT_NEXTAPP_MAXLIMIT_EXCEED)
  {
    goto FINAL_SELECT;
  }

  return iRet;
#else
  finalApp.KernType = EMVL2GetKernelType();
  EMV_SetCurKernelType(finalApp.KernType);
  if(finalApp.KernType != QPBOC) {
 	if(EMV_GetDebug()) {
  		DBG_STR("finalApp.KernType = %d", finalApp.KernType);
	}
	iRet = DECLINED;
  	goto END_PROC;
  }
  
  EMV_SetKernalData(0x9F66, &pxTrans->TTQ[0], 4);

  iRet = EMV_Callback(0, NULL);
  if (iRet) {
	if(EMV_GetDebug()) {
     	DBG_STR("EMV_Callback before GPO = %d", iRet);
	}
    goto END_PROC;
  }
  
  iRet = EMVL2InitApp();
  if (iRet) {
	if(EMV_GetDebug()) {
     	DBG_STR("EMVL2InitApp = %d", iRet);
	}
    goto END_PROC;
  }

  iRet = EMVL2ReadAppData();
  if (iRet){
	if(EMV_GetDebug()) {
    	DBG_STR("EMVL2ReadAppData = %d", iRet);
	}
    goto END_PROC;
  }
  
END_PROC: 
  return iRet;
#endif
}

// ctls complete process
int EMV_CLSSProcess()
{
  int iRet = 0;
  int result = 0;

  result = EMV_CLSSProcess_Simple();
  if (result) {
	if(EMV_GetDebug()) {
    	DBG_STR("EMV_CLSSProcess_Simple = %d", result);
	}
    goto END_PROC;
  }
  
  // card No. confirm
  result = EMV_Callback(1, NULL);
  if (result) {
	if(EMV_GetDebug()) {
     	DBG_STR("EMV_Callback after EMVL2ReadAppData = %d", result);
	}
    goto END_PROC;
  }
  
  // load CAPK
  iRet = EMV_LoadCAPK2Kernel();
  if (iRet < 0) {
	if(EMV_GetDebug()) {
    	DBG_STR("EMV_LoadCAPK2Kernel = %d", iRet);
	}
	result = DECLINED;
    goto END_PROC;
  }

  // offline data auth
  result = EMVL2DataAuth();
  if (result) {
	if(EMV_GetDebug()) {
    	DBG_STR("EMVL2DataAuth = %d", result);
	}
    goto END_PROC;
  }

END_PROC: 
  return result;
}

int EMV_Completion(int OnlineResult, byte * IssuAuthenData, byte IssuAuthenDataLen, 
	byte * Issu71Script, byte Issu71ScriptLen, byte * Issu72Script, byte Issu72ScriptLen)
{
	int iRet = 0;
	byte AuthData[255];
	uint AuthDataLen;
	byte Script[255];
	uint ScriptLen = 0;

	if(OnlineResult == ONLINE_APPROVE) {
		EMV_SetKernalData(0x8A, "\x30\x30", 2);
	}else {
		EMV_SetKernalData(0x8A, "\x30\x35", 2);
	}

	AuthDataLen = 0;
	memset(AuthData, 0, sizeof(AuthData));
	if(IssuAuthenDataLen) {
	    AuthData[0] = (byte)0x91;
	    AuthData[1] = (byte)IssuAuthenDataLen&0xff;
		memcpy(AuthData + 2, IssuAuthenData, IssuAuthenDataLen);
		AuthDataLen = IssuAuthenDataLen + 2;
	}
		
	ScriptLen = 0;
	memset(Script, 0, sizeof(Script));
    if(Issu71ScriptLen) {
        Script[0] = (byte)0x71;
        Script[1] = (byte)Issu71ScriptLen&0xff;
		memcpy(Script+2, Issu71Script, Issu71ScriptLen);
        ScriptLen = Issu71ScriptLen+2;
    }

    if(Issu72ScriptLen) {
        Script[ScriptLen] = (byte)0x72;
        Script[ScriptLen+1] = (byte)Issu72ScriptLen&0xff;
		memcpy(Script+ScriptLen+2, Issu72Script, Issu72ScriptLen);
        ScriptLen += (Issu72ScriptLen+2);
    }
		
    iRet = EMVL2Completion(OnlineResult, ScriptLen, Script, AuthDataLen, AuthData);	
	if(EMV_GetDebug()) {
    	DBG_STR("EMVL2Completion = %d", iRet);
	}
	
	return iRet;
}

int EMV_GetOutCome(ST_OUTCOME * stOutCome)
{
  int ret = 0;

  ST_PARAM_EXTEND stParam;

  stParam.tag = TYPE_TRANSOUTCOME;
  stParam.len = sizeof(ST_OUTCOME);
  stParam.value = stOutCome;
  ret = EMVL2BaseParamOperate(OP_QUERY, &stParam);
  
  if(EMV_GetDebug()) {
	  DBG_STR("EMVL2BaseParamOperate(%d)", ret);
	  DBG_STR("stOutCome.CVMType(%d)", stOutCome->CVMType);
	  DBG_STR("stOutCome.LastStep(%d)", stOutCome->LastStep);
	  DBG_STR("stOutCome.TransResult(%d)", stOutCome->TransResult);
	  DBG_STR("stOutCome.ReceiptFlag(%d)", stOutCome->ReceiptFlag);
  }

  return ret;
}

int EMV_CheckCVMType()
{
	int iRet;
	ST_OUTCOME stOut;

	memset((char *)&stOut, 0, sizeof(stOut));
	iRet = EMV_GetOutCome(&stOut);
	if(iRet) {
		return ONLINEPIN_UNKNOW;
	}
	
	switch(stOut.CVMType) {
	case CVM_ONLINE_PIN:
		return ONLINEPIN_REQUIR;
	case CVM_PPIN_SIG:
	case CVM_EPIN_SIG:
	case CVM_SIG:
		return SIGNATURE_REQUIR;
	}
	
	return ONLINEPIN_FREE;
}

