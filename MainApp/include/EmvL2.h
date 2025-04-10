#ifndef _KERN_DIREC_H_
#define _KERN_DIREC_H_
#include <liteos_api.h>

void AddFixTLVToList(u8 *buff, u16 len, u32 ListIndexDes);
void AddListToList(u32 ListIndexSrc,  u32 ListIndexDes, u8 isTLV);
void AddTagListToList(u32 ListIndexSrc,  u32 ListIndexDes);
void AddTAGToList(u32 indexsrc, u32 indexdes);
void AddTLVListToList(u32 ListIndexSrc,  u32 ListIndexDes);
void AddTLVToBuff(u32 indexsrc, u8 *Destbuff, u16 *position);
void AddTLVToList(u32 indexsrc, u32 ListIndexDes);
void AddToList(u32 ListIndexSrc,  u32 ListIndexDes, u8 isTLV);
int CheckStop(void);
int CreateEMVDataRecord(void);
int CreateEMVDiscretionaryData(void);
int IsEmpty(u32 index);
int IsEmptyList(u32 index);
int IsKnown(u32 Tag);
int IsNotEmpty(u32 index);
int IsNotEmptyList(u32 index);
int IsNotPresent(u32 index);
int IsPresent(u32 index);

#define UPDATE_K           (0x01)
#define UPDATE_RA          (0x02)
#define UPDATE_ACT         (0x04)
#define UPDATE_DET         (0x08)


#define MAX_CANDLIST      ( 10)

typedef enum
{
    EMV = 0, 
    QPBOC = 1, 
    PAYPASS = 2, 
    PAYWAVE = 3, 
    AMERICAEXPRESS = 4, 
    DISCOVER = 5, 
    JCB = 6, 
    EMV_KERNEL_MAX
}EMV_KERNEL_TYPE;

typedef struct
{
    u32   index;
    u8   Format;
    u32  Tag;
    u16  MaxLen;
    u16  Template;
    u8   UpdateSrc;
    u8   UpdateKernel;
}ST_TAG_RULE;


typedef enum
{
    TAG_42_IIN = 0,
    TAG_4F_SC_AID,
    TAG_50_APP_LABEL,
    TAG_57_TRACK2_EQU_DATA,
    TAG_5A_APP_PAN,
    TAG_6F_FCI_TEMPLATE,
    TAG_71_ISSUER_SCRIPT_71,
    TAG_72_ISSUER_SCRIPT_72,
    TAG_81_AMT_B,
    TAG_82_AIP,
    TAG_84_DF_NAME,
    TAG_87_APP_PRIORITY,
    TAG_88_SFI,
    TAG_89_AUTH_CODE,
    TAG_8A_AUTH_RESP,
    TAG_8C_CDOL1,
    TAG_8D_CDOL2,
    TAG_8E_CVM_LIST,
    TAG_8F_CAPK_INDEX,
    TAG_90_IPK_CERTIFICATE,
    TAG_91_ISS_AUTH_DATA,
    TAG_92_IPK_REMAINDER,
    TAG_93_SSAPD,
    TAG_94_AFL,
    TAG_95_TVR,
    TAG_97_TDOL,
    TAG_98_TC_HASH,
    TAG_99_PIN_DATA,
    TAG_9A_TS_DATE,
    TAG_9B_TSI,
    TAG_9C_TS_TYPE,
    TAG_9D_DDF_NAME,
    TAG_A5_FCI_PRO_TEMP,
    TAG_5f20_CARDHOLDER_NAME,
    TAG_5f24_APP_EXPRATION_DATE,
    TAG_5f25_APP_EFFECTIVE_DATE,
    TAG_5f28_ISSUER_COUNTRY_CODE,
    TAG_5F2A_TS_CUR_CODE,
    TAG_5f2D_LANGUAGE_PREFERENCE,
    TAG_5f30_SERVICE_CODE,
    TAG_5f34_PAN_SEQ_NUM,
    TAG_5F36_TS_CUR_EXP,
    TAG_5f50_ISSUER_URL,
    TAG_5f53_IBAN,
    TAG_5f54_BIC,
    TAG_5f55_ISSUER_COUNTRY_CODE2,
    TAG_5f56_ISSUER_COUNTRY_CODE3,
    TAG_5F57_ACCOUNT_TYPE,
    TAG_9F01_AQ_ID,
    TAG_9F02_AMT_N,
    TAG_9F03_AMT_OTHER_N,
    TAG_9F04_AMT_OTHER_B,
    TAG_9f05_APP_DISCRE_DATA,
    TAG_9F06_TM_AID,
    TAG_9f07_APP_USAGE_CONTROL,
    TAG_9f08_CARD_APPVERSION,
    TAG_9F09_TM_APPVERSION,
    TAG_9f0B_CARDHOLDER_EX_NAME,
    TAG_9f0D_IAC_DEFAULT,
    TAG_9f0E_IAC_DENIAL,
    TAG_9f0F_IAC_ONLINE,
    TAG_9f10_ISSUER_APP_DATA,
    TAG_9f11_ISSUER_CODE_TAB_IDX,
    TAG_9f12_APP_PREFER_NAME,
    TAG_9f13_LAST_ATC_REGISTER,
    TAG_9f14_LOW_CONS_OFF_LMT,
    TAG_9F15_MER_CATE_CODE,
    TAG_9F16_MER_ID,
    TAG_9f17_PIN_TRY_COUNTER,
    TAG_9F19_TOKEN_REQUEST_ID,  // bulletin SB197
    TAG_9F1A_TM_COUNTRY_CODE,
    TAG_9F1B_TM_FLOOR_LMT,
    TAG_9F1C_TM_ID,
    TAG_9F1D_TRM,
    TAG_9F1E_IFD_SERIAL,
    TAG_9F1F_TRACK1,
    TAG_9f20_TRACK2,
    TAG_9F21_TS_TIME,
    TAG_9F22_TM_CAPK_INDEX,
    TAG_9f23_UP_CONS_OFF_LMT,
    TAG_9f24_PAR,
    TAG_9F25_LAST4_DIGITS_PAN,  //// bulletin SB197
    TAG_9f26_APP_CRYPTOGRAM,
    TAG_9f27_CARD_CID,
    TAG_9f2D_ICCPIN_CERT,
    TAG_9f2E_ICCPIN_EXPONENT,
    TAG_9f2F_ICCPIN_REMAINDER,
    TAG_9f32_IPK_EXPONENT,
    TAG_9F33_TM_CAPA,
    TAG_9F34_CVM_RESULTS,
    TAG_9F35_TM_TYPE,
    TAG_9f36_ATC,
    TAG_9F37_UPREDICT_NUM,
    TAG_9f38_PDOL,
    TAG_9F39_POS_ENTRY_MODE,
    TAG_9F3A_AMT_REF_CURR_CODE,
    TAG_9f3B_APP_REFER_CURR,
    TAG_9F3C_TS_REF_CURR_CODE,
    TAG_9F3D_TS_REF_CURR_EXP,
    TAG_9F40_TM_ADD_CAPA,
    TAG_9F41_TS_SEQ_COUNTER,
    TAG_9f42_APP_CURR_CODE,
    TAG_9f43_APP_REFE_CURR_EXP,
    TAG_9f44_APP_CURR_EXPONENT,
    TAG_9f45_DATA_AUTH_CODE,
    TAG_9f46_ICCPK_CERT,
    TAG_9f47_ICCPK_EXPONENT,
    TAG_9f48_ICCPK_REMAIDER,
    TAG_9f49_DDOL,
    TAG_9f4a_SSATL,
    TAG_9f4b_SDAD,
    TAG_9f4C_ICC_DYNAMIC_NUMBER,
    TAG_9f4D_LOG_ENTRY,
    TAG_9F4E_MER_NAME_LOC,
    TAG_9f4F_LOG_FORMAT,
    TAG_BF0C_FCI_DISDATA,
    
    TAG_9F5D_AOSO_PBOC,
    TAG_9F61_HOLDER_CERT_INFO_PBOC,
    TAG_9F62_HOLDER_CERT_TYPE_PBOC,
    TAG_9F63_CARD_PRODUCT_INF_PBOC,
    TAG_9F66_TTQ_PBOC,
    TAG_9F69_CARDAUTHDATA_PBOC,
    TAG_9F6C_CTQ_PBOC,
    TAG_9F6D_EC_RT_PBOC,
    TAG_9F6E_CFFI_PBOC,
    TAG_9F74_EC_IAC_PBOC,
    TAG_9F77_EC_BL_PBOC,
    TAG_9F78_EC_BL_PBOC,
    TAG_9F79_EC_CB_PBOC,
    TAG_9F7A_EC_TSI_PBOC,
    TAG_9F7B_EC_TRANS_LIMIT_PBOC,
    TAG_9F7C_CED_PBOC,
    DF4D_LOG_LOAD_ENTRY_PBOC,
    DF4F_LOG_LOAD_FORMAT_PBOC,
    TAG_DF51_SCRIPT_RESULT,
    TAG_DF69_SM_FLAG_PBOC,
    TAG_DF70_LAST_TRANS_AMT_PBOC,
    
    TAG_56_TRACK1_DATA,
    TAG_9F50_OFFLINE_ACCUM_BALANCE_MC,
    TAG_9F51_DRDOL_MC,
    TAG_9F53_TRANS_CATE_CODE_MC,
    TAG_9F54_DS_ODS_CARD_MC,
    TAG_9F5B_DSDOL_MC,
    TAG_9F5C_DS_REQUEST_OP_ID_MC,
    TAG_9F5D_APP_CAPA_INFO_MC,
    TAG_9F5E_DS_ID_MC,
    TAG_9F5F_DS_SLOT_AVAILABLE_MC,
    TAG_9F60_CVC3_TRACK1_MC,
    TAG_9F61_CVC3_TRACK2_MC,
    TAG_9F62_PCVC3_TRACK1_MC,
    TAG_9F63_PUNATC_TRACK1_MC,
    TAG_9F64_NATC_TRACK1_MC,
    TAG_9F65_PCVC3_TRACK2_MC,
    TAG_9F66_PUNATC_TRACK2_MC,
    TAG_9F67_NATC_TRACK2_MC,
    TAG_9F69_UDOL_MC,
    TAG_9F6A_UPRENUM_NUMERIC_MC,
    TAG_9F6B_TRACK2_DATA_MC,
    TAG_9F6D_DS_MAG_VER_MC,
    TAG_9F6E_THIRD_PARTY_DATA_MC,
    TAG_9F6F_DS_SLOT_MANAGE_MC,
    TAG_9F70_PROTECT_DATA_ENVELOPE1_MC,
    TAG_9F71_PROTECT_DATA_ENVELOPE2_MC,
    TAG_9F72_PROTECT_DATA_ENVELOPE3_MC,
    TAG_9F73_PROTECT_DATA_ENVELOPE4_MC,
    TAG_9F74_PROTECT_DATA_ENVELOPE5_MC,
    TAG_9F75_UPRROC_DATA_ENVELOPE1_MC,
    TAG_9F76_UPRROC_DATA_ENVELOPE2_MC,
    TAG_9F77_UPRROC_DATA_ENVELOPE3_MC,
    TAG_9F78_UPRROC_DATA_ENVELOPE4_MC,
    TAG_9F79_UPRROC_DATA_ENVELOPE5_MC,
    TAG_9F7C_MERCHANT_CUSTOM_MC,
    TAG_9F7D_DS_SUMMARY_1_MC,
    TAG_9F7E_MOBILE_SUPPORT_INC_MC,
    TAG_9F7F_DS_UNPRE_NUM_MC,
    DF4B_PCII,
    DF60_DS_INPUT_CARD_MC,
    DF61_DS_DEGIST_MC,
    DF62_DS_ODS_INFO_MC,
    DF63_DS_ODS_TERM_MC,
    DF8101_DS_SUMMARY_2_MC,
    DF8102_DS_SUMMARY_3_MC,
    DF8104_BALANCE_BEFOR_GAC_MC,
    DF8105_BALANCE_AFTER_GAC_MC,
    DF8106_DATA_NEEDED_MC,
    DF8107_CDOL1_RELATED_DATA_MC,
    DF8108_DS_ACTYPE_MC,
    DF8109_DS_INPUT_TERM_MC,
    DF810A_DS_ODS_INFO_FOR_READER_MC,
    DF810B_DS_SUMMARY_STATUS_MC,
    DF810C_KERNEL_ID_MC,
    DF810D_DSVN_Term_MC,
    DF810E_POST_GAC_STATUS_MC,
    DF810F_PRE_GAC_STATUS_MC,
    DF8110_PROCEED_FIRST_WRITE_FLAG_MC,
    DF8111_PDOL_RELATED_DATA_MC,
    DF8112_TAGS_TO_READ_MC,
    DF8113_DRDOL_RELATED_MC,
    DF8114_REFER_CONTROL_PARAM_MC,
    DF8115_ERROR_INDICATION_MC,
    DF8116_UI_QEQ_DATA_MC,
    DF8117_CARD_DATA_INPUT_CAPA_MC,
    DF8118_CVM_CAPA_MC,
    DF8119_CVM_CAPA_NOCVM_REQUIRE_MC,
    DF811A_DEFAULT_UDOL_MC,
    DF811B_KERNEL_CONFIG_MC,
    DF811C_MAXILIFETIME_TORN_LOG_MC,
    DF811D_MAX_TORN_NUM_MC,
    DF811E_MAGSTRIPE_CVM_CAPA_MC,
    DF811F_SEC_CAPA_MC,
    DF8120_TAC_DEFAULT_MC,
    DF8121_TAC_DENIAL_MC,
    DF8122_TAC_ONLINE_MC,
    DF8123_RD_CLSS_FLOOR_LIMIT_MC,
    DF8124_RD_CLSS_TRANS_LIMIT_NOT_ON_DEVICE_MC,
    DF8125_RD_CLSS_TRANS_LIMIT_ON_DEVICE_MC,
    DF8126_RD_CVM_LIMIT_MC,
    DF8127_TIME_OUT_VALUE_MC,
    DF8128_IDS_STATUS_MC,
    DF8129_OUTCOME_MC,
    DF812A_DDD_CARD_TRACK1_MC,
    DF812B_DDD_CARD_TRACK2_MC,
    DF812C_MAGSTRIPE_CVM_CAPA_NO_CVM_MC,
    DF812D_MESSAGE_HOLD_TIME_MC,
    DF8130_HOLDER_TIME_VALUE_MC,
    DF8131_PHONE_MESSAGE_TABLE_MC,
    DF8132_MINIMUM_RELAY_GRACE_SPERIED_MC,
    DF8133_MAXIMUM_RELAY_GRACE_SPERIED_MC,
    DF8134_TERM_EXPECT_CAPDU_MC,
    DF8135_TERM_EXPEC_RAPDU_MC,
    DF8136_RELAY_RESIS_ACCU_THRESHOLD_MC,
    DF8137_RELAY_RESIS_MISMATCH_THRESHOLD_MC,
    DF8178_TRANS_MODE,
    DF8179_CONFIG_TEST,
    DF8301_TERM_RELAY_RESIS_ENTRYOPY_MC,
    DF8302_DEVICE_RELAY_RESIS_ENTRYOPY_MC,
    DF8303_MIN_TIME_RELAY_MC,
    DF8304_MAX_TIME_RELAY_MC,
    DF8305_ESTIMATE_TIME_RAPDU_MC,
    DF8306_MEASURED_RELAY_TIME_MC,
    DF8307_RRP_COUNTER_MC,
    FF8101_TORN_RECORD_MC,
    FF8102_TAGS_TO_WRITE_BEFORE_GAC_MC,
    FF8103_TAGS_TO_WRITE_AFTER_GAC_MC,
    FF8104_DATA_SEND_MC,
    FF8105_DATA_RECORD_MC,
    FF8106_DISCRE_DATA_MC,
    FFFF01_TAGS_TO_READ_YET,
    FFFF02_TAGS_TO_WRITE_YET_AFTER_GAC,
    FFFF03_TAGS_TO_WRITE_YET_BEFORE_GAC,
    FFFF04_ODA_STATUS,
    FFFF05_TORN_TEMP_RECORD,
    FFFF06_FINALSEL_DATA,
    FFFF07_STATE_INFO,
    FFFF08_RRTIME,
    FFFF09_ACTYPE,
    TAG_9F0A_ASRPD_WAVE,
    TAG_9F51_APP_CURRCODE_WAVE,
    TAG_9F52_ADA_WAVE,
    TAG_9F53_CTCIL_WAVE,
    TAG_9F54_CTTAL_WAVE,
    TAG_9F57_ISS_COUNCODE_WAVE,
    TAG_9F58_CTCL_WAVE,
    TAG_9F59_CTCUL_WAVE,
    TAG_9F5A_PROGRAM_ID_WAVE,
    TAG_9F5B_SCRIPTRESULT_WAVE,
    TAG_9F5C_CTTAUL_WAVE,
    TAG_9F5D_AOSO_WAVE,
    TAG_9F5E_CTIUL_WAVE,
    TAG_9F63_OFF_CIV_WAVE,
    TAG_9F66_TTQ_WAVE,
    TAG_9F68_CAP_WAVE,
    TAG_9F69_CARD_WAVE,
    TAG_9F6B_CARD_CVMLIMIT_WAVE,
    TAG_9F6C_CTQ_WAVE,
    TAG_9F6D_VLPRT_WAVE,
    TAG_9F6E_FFI_WAVE,
    TAG_9F73_CONVER_CURRCODE_WAVE,
    TAG_9F7C_CED_WAVE,
    DF01_APP_CAPA_WAVE,
    DF8161_STATUS_CHECK,
    DF8162_ZERO_AMT_CHECK,
    DF8163_CLSS_TX_LIMIT,
    DF8164_CLSS_CVM_LIMIT,
    DF8165_CLSS_FLOOR_LIMIT,
    DF8166_FLOOR_LIMIT,
    DF8167_TERM_CAPA_SELFDEFINE,
    DF8168_EXCEPTION_FILE,
    DF8169_TRACK2_MAP,
    DF8170_REVOCLIST,
    DF8171_CVM_TYPE,
    DF8172_CLSS_FLOOR_LIMIT_CHECK,
    DF8173_CLSS_CVM_LIMIT_CHECK,
    DF8174_CLSS_TX_LIMIT_CHECK,
    DF8175_TIMEOUT_TIME,

    TAG_9F0A_ASRPD_AE,
    TAG_9f2A_KERNEL_ID_AE,
    TAG_9F50_APP_DUAL_CURR_CODE_AE,
    TAG_9F5A_MEMBER_PROD_ID_AE, // 290
    TAG_9F5B_PROD_MEMBER_NUMBER_AE,
    TAG_9F67_FORM_FACTOR_AE,
    TAG_9F6D_CTLS_RD_CAPA_AE,
    TAG_9F6E_ENHANCE_CTLS_RD_CAPA_AE,
    TAG_9F70_CARD_IF_PAYMT_CAPA_AE,
    TAG_9F71_MOBILE_CVM_RESULT_AE,
    TAG_9F77_APP_SPEC_VER_AE,
    TAG_DF8140_PRE_PROC_RESULT_AE,
    TAG_DF8141_DYNAMIC_LIMIT_SET_AE,
    TAG_DF8142_SUPPORT_DELAY_AUTH_AE,
    TAG_DF8143_UNABLE_ONLINE_AE,
    TAG_DF8144_UN_RANGE_AE, // 300
    TAG_DF8145_ATC_GET_DATA_AE,
    TAG_DF8146_TRANS_RESTART_AE,
    TAG_DF8147_PSEUDO_TRACK1_AE,
    TAG_DF8148_PSEUDO_TRACK2_AE,
    TAG_DF8149_EXCEPT_FILE_AE,
    TAG_DF814A_SPECIFIC_USE_AE,
    TAG_DF814B_CONF_NUMBER_AE,
    TAG_ELEMENT_MAX_INDEX,
}TAG_DEFINE ;

#define TRANSMODE_INIT 0
#define TRANSMODE_MAG 1
#define TRANSMODE_MCHIP 2

#define OUTCOME_STATUS_APPROVE 0x10
#define OUTCOME_STATUS_DECLINED 0x20
#define OUTCOME_STATUS_ONLINE_REQUEST 0x30
#define OUTCOME_STATUS_END_APPLICATION 0x40
#define OUTCOME_STATUS_SEE_NEXT 0x50
#define OUTCOME_STATUS_TRY_ANOTHER_INTERFACE 0x60
#define OUTCOME_STATUS_TRY_AGAIN 0x70
#define OUTCOME_STATUS_NA 0xF0

#define OUTCOME_CVM_NO_CVM                    0x00
#define OUTCOME_CVM_SIG                       0x10
#define OUTCOME_CVM_ONLINE_PIN                0x20
#define OUTCOME_CVM_CONFIRM_CODE_VERIFY       0x30
#define OUTCOME_CVM_NA                        0xF0

#define KERNEL_NOT_SUPPORT_STEP 0xFF

#define TAG_NOT_PRESENT 0xffff

#define SELECT_NEXTAPP_MAXLIMIT_EXCEED   8

#define EMV_REENTER_PIN_LAST     7
#define EMV_REENTER_PIN          6
#define CVM_STEP_NEXT            5
#define TRY_AGAIN                4
#define SELECT_NEXT_APP          3
#define ONLINE_REQUEST           2
#define APPROVE                  1
#define CONTINUE                 0
#define DECLINED                                    -4000
#define TRY_ANOTHER_INTERFACE                       -4001
#define ENDAPPLICATION                              -4002
#define SEE_PHONE                                   -4003
#define DECLINED_CAPKINREVO                         -4004
#define ONLINE_REQUEST_CAPKINREVO                   -4005
#define FINALSELECT_DATA_ERR                        -4006
#define ENDAPPLICATION_EXCEPTFILE                   -4007
#define ENDAPPLICATION_OTHERCARD                    -4008

#define SELECT_6A82_AE                              -4096 // AMEX
#define TRY_AGAIN_CVM_AE                            -4097 // AMEX
#define TRYAGAIN_CMD_SWAB_6984                      -4098 // AMEX     
#define SEEPHONE_CMD_SWAB_6986                      -4099
#define ENDAPPLICATION_CMD_ERR                      -4100
#define ENDAPPLICATION_CMD_TIMEOUT                  -4101
#define ENDAPPLICATION_CMD_SWAB_6985                -4102
#define ENDAPPLICATION_CMD_RSP_ERR                  -4103
#define ENDAPPLICATION_CARD_BLOCK                   -4104    
#define ENDAPPLICATION_APP_BLOCK                    -4105
#define ENDAPPLICATION_TMAPP_EMPTY                  -4106      
#define ENDAPPLICATION_NO_SCAPP                     -4107
#define ENDAPPLICATION_DATA_ERR                     -4108
#define ENDAPPLICATION_DATA_DUPLICATE               -4109
#define ENDAPPLICATION_NOT_ACCEPT                   -4110
#define ENDAPPLICATION_CARD_EXPIRED                 -4111
                                                    
#define EMV_NO_PREPARAM                             -4112
                                                    
#define ENDAPPLICATION_L1_TIMEOUT_ERR               -4113         
#define ENDAPPLICATION_L1_TRANSMISSION_ERR          -4114    
#define ENDAPPLICATION_L1_PROTOCAL_ERR              -4115
#define ENDAPPLICATION_L2_CARD_DATA_MISSING         -4116 
#define ENDAPPLICATION_L2_CAM_FAIL                  -4117          
#define ENDAPPLICATION_L2_STATUS_BYTE               -4118         
#define ENDAPPLICATION_L2_PARSING_ERR               -4119         
#define ENDAPPLICATION_L2_MAX_LIMIT_EXEED           -4120     
#define ENDAPPLICATION_L2_CARD_DATA_ERR             -4121
#define ENDAPPLICATION_L2_MAG_NOT_SUPPORT           -4122
#define ENDAPPLICATION_L2_NO_PPSE                   -4123             
#define ENDAPPLICATION_L2_PPSE_FAULT                -4124     
#define ENDAPPLICATION_L2_EMPTY_CAND_LIST           -4125     
#define ENDAPPLICATION_L2_IDS_READ_ERR              -4126 
#define ENDAPPLICATION_L2_IDS_WRITE_ERR             -4127     
#define ENDAPPLICATION_L2_IDS_DATA_ERRR             -4128 
#define ENDAPPLICATION_L2_IDS_NO_MATCH_AC           -4129     
#define ENDAPPLICATION_L2_TERMINAL_DATA_ERR         -4130
#define ENDAPPLICATION_L3_TIMEOUT                   -4131
#define ENDAPPLICATION_L3_STOP                      -4132          
#define ENDAPPLICATION_L3_AMOUNT_NOT_PRESENT        -4133        
#define ENDAPPLICATION_REPRESENT_CARD               -4134
#define ENDAPPLICATION_OHTER_CARD_WITHRECORD        -4135
#define ENDAPPLICATION_OHTER_CARD                   -4136
                                                    
#define ENDAPPLICATION_CMD_RSP_ERR_GPO              -4137
#define ENDAPPLICATION_L2_CARD_DATA_FINALSEL        -4138
#define ENDAPPLICATION_L3_NO_DET_DATA               -4139
#define ENDAPPLICATION_KERNEL_NOT_SUPPORT           -4140
#define ENDAPPLICATION_CLSS_LIMIT_EXCEED            -4141
#define ENDAPPLICATION_ZERO_AMOUNT                  -4142
#define TRY_ANOTHER_INTERFACE_PREPROC               -4144

#define EMV_INVALID_PARAM                           -4500
#define EMV_SUM_ERR                                 -4501
#define EMV_PARAM_NOT_EXIST                         -4502
#define EMV_PARAM_DATA_ERROR                        -4503
#define PBOC_NO_LOG                                 -4504
#define PBOC_LOG_DATA_ERR                           -4505

#define EMV_NO_DATA -4506
#define PBOC_NO_LOG_FMT -4507 // 20180731

#define CVM_MOBILE_CVM_AE        0x01    // mobile cvm AE
/*CVM BYTE0*/
#define CVM_FAIL_CVM         0x00    // fail CVM
#define CVM_PLAIN_PIN        0x01    // plaintext PIN
#define CVM_ONLINE_PIN       0x02    // online enciphered PIN
#define CVM_PPIN_SIG         0x03    // plaintext PIN + signature
#define CVM_ENCIPH_PIN       0x04    // enciphered PIN
#define CVM_EPIN_SIG         0x05    // enciphered PIN + signature
#define CVM_SIG              0x1E    // signature
#define CVM_NO_CVM           0x1F    // no CVM
#define CVM_CONSUMER_DEVICE           0x21    // no CVM
#define CVM_CERT                0x20     // cardholder certificate
#define CVM_FAIL_NEXT        0x40    // Apply succeeding CV Rule if this CVM is unsuccessful
#define CVM_NULL             0xFF


#define UI_OTHER_CARD 0x00
#define UI_TRY_AGAIN 0x01
#define UI_CLEAR_DISPLAY 0x02
#define UI_READY_TO_READ 0x03
#define UI_DECLINED 0x04
#define UI_OTHER_CARD_HOLDTIME 0x05

typedef enum
{
	RESULT_APPROVE = 1,
	RESULT_APPROVE_WITHBALANCE,
	RESULT_DECLINE,
	RESULT_TRYOTHERINTERFACE,
	RESULT_ONLINE_REQUEST,
	RESULT_ONLINE_REQUEST_REPRESENT,
	RESULT_ONLINE_REQUEST_HOLDERON,
	RESULT_TRYAGAIN,
	RESULT_SELECTNEXT,
	RESULT_ENDAPPLICATION,
	RESULT_ENDAPPLICATION_WITHRESTART,

} EMV_RESULT;

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
    STEP_COMPLETION,//11
    STEP_PAYPASS_SINGLE,
    STEP_PAYPASS_LOOP,
    STEP_CLEAN_TORN_LOG,
    STEP_CLEANALL_TORN_LOG,
    STEP_PAYWAVE_SINGLE,
    STEP_PAYWAVE_LOOP,


}EMV_STEP;


typedef enum 
{
    OP_UPDATE = 1,
    OP_CLEAR,
    OP_DEL, 
    OP_QUERY,
    OP_ENUM,
    OP_INITALL,
    OP_UPDATE_PAYPASS,
    OP_SET_NOT_PRESENT,
    OP_UPDATE_PAYWAVE,
    OP_QUERY_PAYWAVE,
    OP_QUERY_PAYPASS,
    OP_FOR_LOG,
    OP_UPDATE_AE = 20,
    OP_QUERY_AE,
    OP_CLEAR_CARD,
}EMV_STORE_OP;






typedef enum 
{
	TYPE_ALL = 0,
	TYPE_CAPK,
	TYPE_REVOCLIST,
	TYPE_CANDLIST,
	TYPE_TERMAIDLIST,
	TYPE_TERMPARAM,
	TYPE_PREPARAM,
	TYPE_TRANSOUTCOME,
	TYPE_AIDPARAM,
} EMV_PARAM_TYPE;




typedef struct 
{ 
    u32   tag;
    u32   len;
    void* value;   
}ST_PARAM_EXTEND;





typedef struct
{ 
    unsigned char  AidLen;             
    unsigned char  AID[16];             
    unsigned char  SelFlag;             //(PART_MATCH  0;  FULL_MATCH  1) 
    unsigned char  KernType;
    unsigned char  reserved;           //The reserved byte value must be 0
}ST_TERMAIDLIST; /*align 4 Bytes*/


typedef struct
{  
    unsigned char  TargetPer;           //Target percentage
    unsigned char  MaxTargetPer;        //Maximum target percentage
    unsigned char  FloorLimitCheck;     //Whether to check the minimum
    unsigned char  FloorLimit[4];      //Minimum, big-end storage 9F1B
    unsigned char  RandTransSel;        //Whether to conduct random transaction selection
    unsigned char  VelocityCheck;       //Whether frequency detection is performed
    unsigned char  Threshold[4];        //threshold value
    unsigned char  TACDenial[5];        //Terminal behavior code (reject)
    unsigned char  TACOnline[5];        //Terminal behavior code (online)
    unsigned char  TACDefault[5];       //Terminal behavior code (default)
    unsigned char  AcquierId[6];        //Receiving line sign 9F01
    unsigned char  dDOLLen;             //
    unsigned char  dDOL[32];           //default DDOL
    unsigned char  tDOLLen;             //
    unsigned char  tDOL[32];           //default TDOL
    unsigned char  Version[2];          //
    unsigned char  RMDLen;              //Risk management data length
    unsigned char  RiskManData[8];      //Risk management data
    unsigned char  MerchName[128];      //merchant name 9F4E
    unsigned char  MerchCateCode[2];    //Merchant class code (not required)9F15
    unsigned char  MerchId[16];         //Merchant logo (merchant number)9F16
    unsigned char  TermId[8];           //(POS no)
    unsigned char  TransCurrCode[2];    //5F2A
    unsigned char  TransCurrExp;        //5F36    
    unsigned char  ReferCurrCode[2];    //9F3C
    unsigned char  ReferCurrExp;        //9F3D
    unsigned char  ReferCurrCon[4];     //(The exchange rate of the trading currency against the reference currency*1000)
    unsigned char  reserved;          
} ST_AIDPARAM; /*align 4 Bytes*/



typedef struct
{  
    u16 Index;              //The ordinal number applied to the candidate column
    u8  AID[16];           //card AID，end of '\0' 
    u8  AidLen;            //
    u8  AppPreName[17];    //Apply the preferred name，end of '\0'
    u8  AppLabel[17];      //app label，end of '0'
    u8  IssDiscrData[244]; //tag 'BF0C'data：1 byte length+'BF0C' the maximum value is 222 bytes，end of '\0' 
    u8  Priority;          //Priority flag
    u8  AppName[33];       //
    u8  KernType;        //Non-application kernel type
} ST_CANDLIST; /*align 4 Bytes*/


typedef struct {
    u8  RID[5];            //ID of the application registration service provider
    u8  Index;             //Key index
    u8  HashInd;           //HASH algrithm indicate
    u8  ArithInd;           //RSA algrithm indicate
    u8  ModulLen;          //
    u8  Modul[248];        //
    u8  ExponentLen;       //
    u8  Exponent[3];       //
    u8  ExpDate[3];        //(YYMMDD)
    u8  CheckSum[20];      //checksum
}ST_CAPK;/*align 4 Bytes*/


typedef  struct 
{
    u8  RID[5];                  // RID
    u8  Index;                   // CA index
    u8  CertSn[3];            // Issuing bank certificate serial number
    u8  reserved[3];           //must be 0
}ST_REVOCLIST; /*align 4 Bytes*/


typedef struct
{
    u8  AidLen; 
    u8  AID[16];                         
    u8  IsTermCheckStatus;               // Whether the terminal supports status check
    u8  IsExistCVMLmt;                   // Cardholder limit
    u8  CVMLmt[6];                      // Big-endian storage
    u8  IsExistTermClssLmt;              // Terminal non-transaction limit
    u8  TermClssLmt[6];                 // Big-endian storage
    u8  IsExistTermClssOfflineFloorLmt;    // Terminal off-line minimum
    u8  TermClssOfflineFloorLmt[6];     // Big-endian storage
    u8  IsExistTermOfflineFloorLmt;      // The terminal off-line minimum
    u8  TermOfflineFloorLmt[6];         // Big-endian storage
    u8  reserved[2];                    //must be 0
}ST_PREPARAM;  /*align 4 Bytes*/


typedef struct{
    u8  IFDSn[8];               // IFD serial no 9F1E
    u8  TerminalType;           // terminal type 9F35
    u8  CountryCode[2];         // Terminal country code 9F1A
    u8  ForceOnline;            // Merchant compulsory on-line(1 always online)
    u8  GetDataPIN;             // Number of read retries before password detection
    u8  SurportPSESel;          // Whether to support the PSE option
    u8  UseTermAIPFlg;          // Whether to conduct risk management based on card AIP,0-card AIP,1- terminal AIP,default 0
    u8  TermAIP[2];             // Whether the terminal forcibly performs risk management，byte1-bit4为1：force；byte1-bit4为0：no 。default Both bytes are 0。
    u8  BypassAllFlg;           // whether bypass all other pin when one pin has been bypassed 1-Yes, 0-No
    u8  BypassPin;              // 0-not surport，1－surport，default surport
    u8  BatchCapture;           // 0-online data capture, 1-batch capture
    u8  ECTSIFlg;               // TSI exist? 1-exist （EC Terminal Support Indicator）
    u8  ECTSIVal;               // Electronic cash terminal support indicator = 1,支持
    u8  ECTTLFlg;               // TTL exist? 1-exist（EC Terminal Transaction Limit）
    u8  ECTTLVal[6];            // EC Terminal Transaction Limit
    u8  Capability[3];          // Terminal performance 9F33
    u8  AddCapability[5];       // Terminal expansion performance 9F40
    u8  ScriptMode;                     
    u8  AdviceFlag;             
    u8  IsSupportSM;            // 
    u8  IsSupportTransLog;      // Whether to support trading LOG
    u8  IsSupportMultiLang;     // Whether multiple languages are supported
    u8  IsSupportExceptFile;    // Whether exception files are supported
    u8  IsSupportAccountSelect; // Whether to support account selection
    u8  TTQ[4];                 // Terminal transaction attributes (ctls used)
    u8  IsReadLogInCard;        // Whether to read the card transaction record application selection process
    u8  reserved[3];               //must be 0
}ST_TERMPARAM; /*align 4 Bytes*/



typedef struct{
    u8  LastStep;    //The last step to perform
    u8  CVMType;   
    u8  TransResult; 
    u8  ReceiptFlag;  //whether need print the receipt
    u8  ScriptResultlen; 
    u8  ScriptResult[80]; //script result
    u8  Reserved[3];    //must be 0  
}ST_OUTCOME; /*align 4 Bytes*/

typedef struct
{
	int paramLen[9];
	u8 *paramData[9];
} ST_PARAM_EMV;

typedef enum  
{
	CVM_SUCSEC,
	CVM_PINCANCEL,
	CVM_PINBYPASS,
	CVM_PINPADERROR,
	CVM_CERT_FAIL,
}CVM_R;

#endif

