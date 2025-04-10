#include "app.h"
#include "EMVL2.h"
#include "tag.h"
#include "type.h"
#include "emv.h"
#include "emvproc.h"

#ifdef AEPRM_DEBUG
#define AEPRM_STRLOG DBG_STR
#define AEPRM_HEXLOG DBG_HEX
#else
#define AEPRM_STRLOG  
#define AEPRM_HEXLOG 
#endif

extern ST_TRANS* EMV_GetTrans();

void s_AE_FixCfg(void)
{
    ST_TRANS *stTrans = (ST_TRANS *)EMV_GetTrans();
    Database_SetTlv(TAG_5F2A_TS_CUR_CODE, &(stTrans->TransCurrCode[0]),2);
    Database_SetTlv(TAG_5F36_TS_CUR_EXP, &(stTrans->TransCurrExp[0]),1);

    Database_SetTlv(TAG_9F6D_CTLS_RD_CAPA_AE, (unsigned char*)"\xC0", 1);//Expresspay ¡Ý 3.0 EMV and Magstripe ¨C CVM Not Required
#if 1
    Database_SetTlv(TAG_9F6E_ENHANCE_CTLS_RD_CAPA_AE, (unsigned char*)"\x58\x00\x00\x03", 4);
    Database_SetTlv(TAG_DF8142_SUPPORT_DELAY_AUTH_AE, "\x00", 1);
#else
    //b7=1:Delayed Authorization Terminal
    Database_SetTlv(TAG_9F6E_ENHANCE_CTLS_RD_CAPA_AE, (unsigned char*)"\x58\x00\x00\x43", 4);
    Database_SetTlv(TAG_DF8142_SUPPORT_DELAY_AUTH_AE, "\x01", 1);
#endif

}

void s_AE_NotFixCfg()
{
    u8 tag_7F10[256], tag[4] = {0}, len_7F10=0;
    u16 aidlen=0, len;
    int ret;
    u32 tmplen=0;
    u8 AID[16], CAPA[16];
    ST_AID_COND AidCond;
    ST_AID_EXT AidExtTag;
    const ST_TAG_DEF tagList[] = {
        {2, TAG_7F10, 255, tag_7F10, &len_7F10, NULL}, //kernel configs
    };
    Database_SetTlv(TAG_9F09_TM_APPVERSION, (unsigned char*)"\x00\x01", 2); //Application Version Number 0002

    Database_GetTlvData(TAG_84_DF_NAME, AID, &aidlen);

    memset((char *)&AidExtTag, 0, sizeof(ST_AID_EXT));
    AidCond.AIDLen = aidlen;
    memcpy(AidCond.AID, AID, aidlen);
    ret = EMV_GetAIDEXT(&AidCond, &AidExtTag);
    if (ret < 0) {
        AEPRM_STRLOG("EMV_ReadAidExtTags=%d", ret);
        return;
    }

    AEPRM_HEXLOG("AID EXT:", AidExtTag.AID, AidExtTag.AIDLen[0]);
    AEPRM_HEXLOG("AID EXT:", AidExtTag.DynamicTags, AidExtTag.Tagslen[0]);

    ret = EMV_DecodeTlv(AidExtTag.Tagslen[0], AidExtTag.DynamicTags, sizeof(tagList) / sizeof(ST_TAG_DEF), tagList);
    if (ret < 0) return;

    AEPRM_HEXLOG("tag_7F10:", tag_7F10, len_7F10);

    if(len_7F10){
        tmplen = len_7F10;
        ret = EMVL2BaseTLVOperate(OP_UPDATE_AE, 0, NULL, &tmplen, tag_7F10);
        AEPRM_STRLOG("ret:%d", ret);
    }

    memset(CAPA, 0X00, sizeof(CAPA));
    len = 0;
    Database_GetTlvData(TAG_9F6E_ENHANCE_CTLS_RD_CAPA_AE, CAPA, &len);
    if((len==4)&& (CAPA[3]&0x40)) Database_SetTlv(TAG_DF8142_SUPPORT_DELAY_AUTH_AE, "\x01", 1);
    else Database_SetTlv(TAG_DF8142_SUPPORT_DELAY_AUTH_AE, "\x00", 1);
    AEPRM_HEXLOG("TAG_9F6E_ENHANCE_CTLS_RD_CAPA_AE:", CAPA, len);
}

