#ifndef __EMVPROC_H__
#define __EMVPROC_H__

//#define CHINA_EMVPRM

typedef struct {
  unsigned char   tagLen;
  const char *    tag;
  unsigned char   dataSize;
  unsigned char * data;
  unsigned char * dataLen;
  unsigned char * flag;
}ST_TAG_DEF;

void EMV_InitCAPK();
void EMV_InitAidList();
int EMV_Process(int cardType);
int EMV_Process_Simple(int cardType);
int EMV_QPSProcess();

int EMV_SelectApp(int isRetry, int num, ST_CANDLIST * appList);
int EMV_CheckId(byte type, byte * no);
int EMV_InputOnlinePIN();
int EMV_ReadCardData();
int EMV_InitDefault(void * pxPARAM);
int EMV_DecodeTlv(int dataLen, unsigned char * data, int tagNum, const ST_TAG_DEF * tagList);
int EMV_GetTagValueLen(int dataLen, unsigned char * data, int * valLen);
int EMV_GetTag(int dataLen, unsigned char * data, unsigned char * tag);

#endif

