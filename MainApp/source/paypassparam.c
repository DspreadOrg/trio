#include "app.h"
#include "EMVL2.h"
#include "paypassparam.h"
#include "tag.h"
#include "type.h"
#include "emv.h"
#include "emvproc.h"

#ifdef PAYPASSPRM_DEBUG
#define PAYPASS_STRLOG DBG_STR
#define PAYPASS_HEXLOG DBG_HEX
#else
#define PAYPASS_STRLOG  
#define PAYPASS_HEXLOG 
#endif

#define MASTERCARD_AID "\xA0\x00\x00\x00\x04\x10\x10"
#define MESTRO_AID "\xA0\x00\x00\x00\x04\x30\x60"
#define TEST_AID "\xB0\x12\x34\x56\x78"

#define MASTERCARD_AID_LEN 7
#define MESTRO_AID_LEN 7
#define TEST_AID_LEN 5

extern ST_TRANS* EMV_GetTrans();
extern ST_DEFAULT* EMV_GetDefault();
void s_PaypassFixCfg()
{
    unsigned char ucTransType;
    ST_DEFAULT * stDefault = EMV_GetDefault();
    ST_TRANS *stTrans = (ST_TRANS *)EMV_GetTrans();
	
PAYPASS_HEXLOG("Capability:", &(stDefault->Capability[0]), 3);
    Database_SetTlv(TAG_5F57_ACCOUNT_TYPE, NULL, 0);
    Database_SetTlv(TAG_9F09_TM_APPVERSION, (unsigned char*)"\x00\x02", 2); //Application Version Number 0002
    Database_SetTlv(TAG_9F40_TM_ADD_CAPA, (unsigned char*)"\x00\x00\x00\x00\x00", 5);  
    Database_SetTlv(DF8104_BALANCE_BEFOR_GAC_MC, NULL, TAG_NOT_PRESENT);
    Database_SetTlv(DF8105_BALANCE_AFTER_GAC_MC, NULL, TAG_NOT_PRESENT);

    Database_SetTlv(TAG_5F2A_TS_CUR_CODE, &(stTrans->TransCurrCode[0]),2);
    Database_SetTlv(TAG_5F36_TS_CUR_EXP, &(stTrans->TransCurrExp[0]),1);
PAYPASS_HEXLOG("TransCurrCode:", &(stTrans->TransCurrCode[0]), 2);
PAYPASS_HEXLOG("TransCurrExp:", &(stTrans->TransCurrExp[0]), 1);

    Database_SetTlv(DF8117_CARD_DATA_INPUT_CAPA_MC, (unsigned char*)"\x00", 1);  
    Database_SetTlv(DF8118_CVM_CAPA_MC, &(stDefault->Capability[1]), 1);
    Database_SetTlv(DF8119_CVM_CAPA_NOCVM_REQUIRE_MC, &(stDefault->Capability[1]), 1);
    Database_SetTlv(DF811F_SEC_CAPA_MC, (unsigned char*)"\x08", 1);

    Database_SetTlv(DF811A_DEFAULT_UDOL_MC, (unsigned char*)"\x9F\x6A\x04",3);


    Database_SetTlv(DF8108_DS_ACTYPE_MC, NULL, 0);
    Database_SetTlv(DF60_DS_INPUT_CARD_MC, NULL, 0);
    Database_SetTlv(DF8109_DS_INPUT_TERM_MC, NULL, 0);
    Database_SetTlv(DF62_DS_ODS_INFO_MC, NULL, 0);    
    Database_SetTlv(DF810A_DS_ODS_INFO_FOR_READER_MC, NULL, 0);
    Database_SetTlv(DF63_DS_ODS_TERM_MC, NULL, 0);
    Database_SetTlv(TAG_9F5C_DS_REQUEST_OP_ID_MC, NULL, TAG_NOT_PRESENT);    
    Database_SetTlv(DF810D_DSVN_Term_MC, NULL, 0);    

    Database_SetTlv(DF8130_HOLDER_TIME_VALUE_MC, NULL, TAG_NOT_PRESENT);    

    Database_SetTlv(DF810C_KERNEL_ID_MC, (unsigned char*)"\x02", 1);
    Database_SetTlv(TAG_9F6D_DS_MAG_VER_MC, (unsigned char*)"\x00\x01", 2);
    Database_SetTlv(DF811C_MAXILIFETIME_TORN_LOG_MC, (unsigned char*)"\x00\x00", 2);
    Database_SetTlv(DF811D_MAX_TORN_NUM_MC, (unsigned char*)"\x00", 1);

    
    Database_SetTlv(DF812D_MESSAGE_HOLD_TIME_MC, NULL, TAG_NOT_PRESENT);
    Database_SetTlv(TAG_9F7E_MOBILE_SUPPORT_INC_MC, NULL, 0);   
    
    /*
    Reader Contactless Floor Limit
    Indicates the transaction amount above which transactions must be authorized online.
    */ 
    Database_SetTlv(DF8123_RD_CLSS_FLOOR_LIMIT_MC,  (unsigned char*)"\x00\x00\x00\x01\x00\x00", 6);

    /*
    Reader Contactless Transaction Limit (No Ondevice CVM)
    Indicates the transaction amount above which the transaction is not allowed, when on device cardholder verification is not supported.
    */
    Database_SetTlv(DF8124_RD_CLSS_TRANS_LIMIT_NOT_ON_DEVICE_MC,  (unsigned char*)"\x00\x00\x00\x03\x00\x00", 6);

    /*
    Reader Contactless Transaction Limit (On-device CVM)
    Indicates the transaction amount above which the transaction is not allowed, when on device cardholder verification is supported
    */
    Database_SetTlv(DF8125_RD_CLSS_TRANS_LIMIT_ON_DEVICE_MC,  (unsigned char*)"\x00\x00\x00\x05\x00\x00", 6);


    Database_SetTlv(DF8110_PROCEED_FIRST_WRITE_FLAG_MC, NULL, TAG_NOT_PRESENT);
    Database_SetTlv(DF8112_TAGS_TO_READ_MC, NULL, TAG_NOT_PRESENT);
    Database_SetTlv(FF8102_TAGS_TO_WRITE_BEFORE_GAC_MC, NULL, TAG_NOT_PRESENT);
    Database_SetTlv(FF8103_TAGS_TO_WRITE_AFTER_GAC_MC, NULL, TAG_NOT_PRESENT);

    Database_SetTlv(DF8120_TAC_DEFAULT_MC, (unsigned char*)"\x00\x00\x00\x00\x00", 5);
    Database_SetTlv(DF8121_TAC_DENIAL_MC, (unsigned char*)"\x00\x00\x00\x00\x00", 5);
    Database_SetTlv(DF8122_TAC_ONLINE_MC, (unsigned char*)"\x00\x00\x00\x00\x00", 5);

    Database_SetTlv(TAG_9F33_TM_CAPA, NULL, 0);
    Database_SetTlv(TAG_9F1A_TM_COUNTRY_CODE, &(stDefault->CountryCode[0]), 2);
    Database_SetTlv(TAG_9F1C_TM_ID, NULL, 0);
    Database_SetTlv(DF8127_TIME_OUT_VALUE_MC, NULL, TAG_NOT_PRESENT);


    Database_SetTlv(TAG_9F35_TM_TYPE, &(stDefault->TerminalType[0]), 1); 

    //below set the default value for not fix data
    Database_SetTlv(DF811E_MAGSTRIPE_CVM_CAPA_MC, (unsigned char*)"\x10", 1);
    Database_SetTlv(DF812C_MAGSTRIPE_CVM_CAPA_NO_CVM_MC,  (unsigned char*)"\x00", 1);     

    Database_SetTlv(DF811B_KERNEL_CONFIG_MC, (unsigned char*)"\x20", 1);   
    Database_SetTlv(TAG_9F1D_TRM, (unsigned char *)"\x6C\xFF\x00\x00\x00\x00\x00\x00", 8);  

    /*
    Reader CVM Required Limit
    Indicates the transaction amount above which the Kernel instantiates the CVM capabilities field in Terminal Capabilities
    with CVM Capability ?CVM Required.
    */
    Database_SetTlv(DF8126_RD_CVM_LIMIT_MC, (unsigned char *)"\x00\x00\x00\x00\x10\x00", 6);     
}

static void s_PaypassDynamicTags()
{
    u8 tag_7F10[256],tag_7F11[256], tag[4] = {0}, len_7F10=0, len_7F11=0;
    u16 aidlen=0;
    int ret, offset = 0, tagLen = 0, lenLen, valLen, first_9c=0, next_9c=0;
    u32 tmplen=0;
    u8 ucTransType, AID[16];
    ST_AID_COND AidCond;
    ST_AID_EXT AidExtTag;
    const ST_TAG_DEF tagList[] = {
        {2, TAG_7F10, 255, tag_7F10, &len_7F10, NULL}, //kernel configs
        {2, TAG_7F11, 255, tag_7F11, &len_7F11, NULL}, //9C Conditional processing.
    };

    Database_GetTlvData(TAG_9C_TS_TYPE, &ucTransType, NULL);//Current 9C tag value    
    Database_GetTlvData(TAG_84_DF_NAME, AID, &aidlen);

    memset((char *)&AidExtTag, 0, sizeof(ST_AID_EXT));
    AidCond.AIDLen = aidlen;
    memcpy(AidCond.AID, AID, aidlen);
	
    ret = EMV_GetAIDEXT(&AidCond, &AidExtTag);
    if (ret < 0) {
        PAYPASS_STRLOG("EMV_ReadAidExtTags=%d", ret);
        return;
    }

    PAYPASS_HEXLOG("AID EXT:", AidExtTag.AID, AidExtTag.AIDLen[0]);
    PAYPASS_HEXLOG("AID EXT:", AidExtTag.DynamicTags, AidExtTag.Tagslen[0]);

    ret = EMV_DecodeTlv(AidExtTag.Tagslen[0], AidExtTag.DynamicTags, sizeof(tagList) / sizeof(ST_TAG_DEF), tagList);
	if (ret < 0) return;

    PAYPASS_HEXLOG("tag_7F10:", tag_7F10, len_7F10);
    PAYPASS_HEXLOG("tag_7F11:", tag_7F11, len_7F11);
    if(len_7F10){
        tmplen = len_7F10;
        ret = EMVL2BaseTLVOperate(OP_UPDATE_PAYPASS, 0, NULL, &tmplen, tag_7F10);
        PAYPASS_STRLOG("ret:%d", ret);
    }

    if(len_7F11){
        first_9c = next_9c = -1;
        while(offset < len_7F11) {
            tagLen = EMV_GetTag(len_7F11 - offset, tag_7F11 + offset, tag);
            if(tagLen <= 0){
                break;
            }
            PAYPASS_STRLOG("EMV_GetTag = %d", tagLen);
            PAYPASS_HEXLOG("tag", tag, tagLen);
            offset += tagLen;

            lenLen = EMV_GetTagValueLen(len_7F11 - offset, tag_7F11 + offset, &valLen);
            if(lenLen <= 0)
            {
                PAYPASS_HEXLOG("tag", tag, tagLen);
                PAYPASS_STRLOG("EMV_GetTagValueLen = %d", lenLen);
                PAYPASS_STRLOG("valLen = %d", valLen);
                break;
            }            

            if((tagLen!=1) || (tag[0]!=0x9C)) {
                offset += lenLen;
                offset += valLen;
                continue;
            }
            if((first_9c<0) && (ucTransType!=tag_7F11[offset+1])) {
                offset += lenLen;
                offset += valLen;
                continue;
            }
            PAYPASS_STRLOG("offset = %d, first_9c = %d, next_9c = %d", offset, first_9c, next_9c);
            if(first_9c<0) first_9c = offset;
            else next_9c = offset;

            offset += lenLen;
            offset += valLen;
            if((first_9c>=0) && (next_9c>0)) break;                      
        }
        if((first_9c>=0) && (next_9c==-1)){
            tmplen =  len_7F11 - (first_9c+2);
            ret = EMVL2BaseTLVOperate(OP_UPDATE_PAYPASS, 0, NULL, &tmplen, &tag_7F11[first_9c+2]);
            PAYPASS_HEXLOG("9C Condition tags:", &tag_7F11[first_9c+2], tmplen);
            PAYPASS_STRLOG("ret:%d", ret); 
        }
        else if((first_9c>=0) && (next_9c>0)){
            tmplen =  next_9c - first_9c -2 -1;
            ret = EMVL2BaseTLVOperate(OP_UPDATE_PAYPASS, 0, NULL, &tmplen, &tag_7F11[first_9c+2]);
            PAYPASS_HEXLOG("9C Condition tags:", &tag_7F11[first_9c+2], tmplen);
            PAYPASS_STRLOG("ret:%d", ret); 
        }
    }
}

void s_PaypassNotFixCfg(void)
{
    unsigned char ucTransType, AID[16];
    u16 aidlen=0;
    int ret;

    ST_AID AppParam;
    ST_AID_COND AidCond;
    ST_PARAM_EXTEND stParam;
    ST_AIDPARAM AidParam;

    memset((char *)&AidParam, 0, sizeof(ST_AIDPARAM));
    stParam.tag = TYPE_AIDPARAM;
    stParam.len = sizeof(ST_AIDPARAM);
    stParam.value = &AidParam;
    EMVL2BaseParamOperate(OP_QUERY, &stParam);

    Database_GetTlvData(TAG_9C_TS_TYPE, &ucTransType, NULL);    
    Database_GetTlvData(TAG_84_DF_NAME, AID, &aidlen);


    PAYPASS_STRLOG("\r\nucTransType:%02x,AID[%d]:%02x %02x %02x %02x %02x %02x %02x\r\n", ucTransType, aidlen, AID[0],AID[1],AID[2],AID[3],AID[4],AID[5],AID[6]);

    if( ucTransType == 0x17 && memcmp(AID, MASTERCARD_AID, MASTERCARD_AID_LEN) == 0)
    {
        Database_SetTlv(DF811E_MAGSTRIPE_CVM_CAPA_MC, (unsigned char*)"\x20", 1);
    }
    else if( ucTransType == 0x17 && memcmp(AID, MESTRO_AID, MESTRO_AID_LEN) == 0)
    {
        Database_SetTlv(DF811E_MAGSTRIPE_CVM_CAPA_MC, (unsigned char*)"\xF0", 1);
    }
    else
    {
        Database_SetTlv(DF811E_MAGSTRIPE_CVM_CAPA_MC, (unsigned char*)"\x10", 1);
    }
    
    if( ucTransType == 0x17 && memcmp(AID, MESTRO_AID, MESTRO_AID_LEN) == 0)
    {
        Database_SetTlv(DF812C_MAGSTRIPE_CVM_CAPA_NO_CVM_MC,  (unsigned char*)"\xf0", 1);     
    }
    else
    {
        Database_SetTlv(DF812C_MAGSTRIPE_CVM_CAPA_NO_CVM_MC,  (unsigned char*)"\x00", 1);     
    }

    if (!memcmp(AID, MASTERCARD_AID, MASTERCARD_AID_LEN))
    {
        Database_SetTlv(DF811B_KERNEL_CONFIG_MC, (unsigned char*)"\x20", 1);   
        
        if(ucTransType == 0x00)
        {
            Database_SetTlv(TAG_9F1D_TRM, (&AidParam.RiskManData[0]), AidParam.RMDLen);   
        }
        else if(ucTransType == 0x09)
        {
            Database_SetTlv(TAG_9F1D_TRM, NULL, 0);   
        }
        
    }
    else if (!memcmp(AID, MESTRO_AID, MESTRO_AID_LEN))
    {
        Database_SetTlv(DF811B_KERNEL_CONFIG_MC, (unsigned char*)"\xA0", 1);   
        Database_SetTlv(TAG_9F1D_TRM, (&AidParam.RiskManData[0]), AidParam.RMDLen);   
    }


    PAYPASS_HEXLOG("AidParam.TACDenial", AidParam.TACDenial, 5);
    PAYPASS_HEXLOG("AidParam.TACOnline", AidParam.TACOnline, 5);
    PAYPASS_HEXLOG("AidParam.TACDefault", AidParam.TACDefault, 5);    
    Database_SetTlv(DF8121_TAC_DENIAL_MC, (&AidParam.TACDenial[0]), 5);
    Database_SetTlv(DF8122_TAC_ONLINE_MC, (&AidParam.TACOnline[0]), 5);
    Database_SetTlv(DF8120_TAC_DEFAULT_MC, (&AidParam.TACDefault[0]), 5);

    //read preParam
    memset((char *)&AppParam, 0, sizeof(ST_AID));
    AidCond.AIDLen = aidlen;
    memcpy(AidCond.AID, AID, aidlen);
    ret = EMV_GetAID(&AidCond, &AppParam);
    PAYPASS_STRLOG("EMV_ReadAid=%d", ret);
    if (ret < 0) return;

    if(AppParam.IsExistCVMLmt) Database_SetTlv(DF8126_RD_CVM_LIMIT_MC, AppParam.CVMLmt, 6);     
    if(AppParam.IsExistTermClssOfflineFloorLmt) Database_SetTlv(DF8123_RD_CLSS_FLOOR_LIMIT_MC,  AppParam.TermClssOfflineFloorLmt, 6);
    if(AppParam.IsExistTermClssLmt) {
        Database_SetTlv(DF8124_RD_CLSS_TRANS_LIMIT_NOT_ON_DEVICE_MC,  AppParam.TermClssLmt, 6);
        Database_SetTlv(DF8125_RD_CLSS_TRANS_LIMIT_ON_DEVICE_MC,  AppParam.TermClssLmt, 6);
    }

    PAYPASS_STRLOG("AppParam.IsExistCVMLmt = %d", AppParam.IsExistCVMLmt[0]);
    PAYPASS_HEXLOG("AppParam.CVMLmt", AppParam.CVMLmt, 6);
    PAYPASS_STRLOG("AppParam.IsExistTermClssLmt = %d", AppParam.IsExistTermClssLmt[0]);
    PAYPASS_HEXLOG("AppParam.TermClssLmt", AppParam.TermClssLmt, 6);
    PAYPASS_STRLOG("AppParam.IsExistTermClssOfflineFloorLmt = %d", AppParam.IsExistTermClssOfflineFloorLmt[0]);
    PAYPASS_HEXLOG("AppParam.TermClssOfflineFloorLmt", AppParam.TermClssOfflineFloorLmt, 6);
    PAYPASS_STRLOG("AppParam.IsExistTermOfflineFloorLmt = %d", AppParam.IsExistTermOfflineFloorLmt[0]);
    PAYPASS_HEXLOG("AppParam.TermOfflineFloorLmt", AppParam.TermOfflineFloorLmt, 6);
    PAYPASS_STRLOG("------");

    s_PaypassDynamicTags();
}


