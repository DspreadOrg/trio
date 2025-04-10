#ifndef __UTILS_H__
#define __UTILS_H__

#define MAX_BARCODE_HEIGHT 80

#define MAX_TAG_LEN     2
#define MAX_TAG_LEN_LEN 4
#define MAX_TAG_VAL_LEN 255

typedef struct
{
  ushort tag;
  byte   dataSize;
  byte * data;
  byte * dataLen;
  byte * flag;
}ST_EMVTAG;

int Utils_Int2Hex(uint IntData, byte * HexBuf, int HexLen);

uint Utils_Hex2Int(byte *HexBuf,  int HexLen);
int Utils_Int2Bcd(uint IntData, byte * BcdBuf, int BcdLen);

uint Utils_Bcd2Int(byte *BcdBuf,  int BcdLen);

int Utils_Bcd2Asc(byte *bcd, uint bcdLen, byte *asc);

int Utils_Asc2Bcd(byte *asc, uint ascLen, byte *bcd);

int Utils_Int2Str(int iVal, char * str, uint len);

int Utils_Str2Int(const char * str);

void Utils_Int2Amt(uint amt, char * strAmt);

uint Utils_Amt2Int(const char * strAmt);

int Utils_Amt2Asc(const char * strAmt, char * ascAmt, int amtSize);

int Utils_Asc2Amt(const char * ascAmt, char * strAmt, int strSize);

int Utils_StrCopy(char * dst, const char *src, int len);

byte Utils_AscAdd(byte *augend, byte *addend, uint len);

byte Utils_AscSub(byte *minuend, byte *subtrahend, uint len);

void Utils_Xor(byte *psVect1, byte *psVect2, uint uiLength, byte *psOut);


int Uitls_AddEMVTLV(byte * data, uint dataLen, ushort tag, byte *val, uint valLen);
int Utils_DelEMVTLV(byte * data, uint dataLen, ushort tag);
int Utils_ModifiyTLV(byte * data, uint dataLen, ushort tag, byte *newVal, uint newValLen);
int Utils_GetEMVTLV(byte * data, uint dataLen, ushort tag, byte * val, uint valSize);
int Utils_DecodeEMVTlv(byte * data, uint dataLen, const ST_EMVTAG * tagList, uint tagNum);

void Utils_Escape(const char * src, char * dst);

char * Utils_TrimStr(char *pszString);

int Utils_IsLeapYear(int year);

int Utils_DayOfMonth(int month, int year);

int Utils_GBK2UTF8(const     char *pGBK, char *pUTF8, int number);

int Utils_UTF82GBK(const char *pUTF8, char *pGBK, int number);

int Utils_IsTextUTF8(const char * str, int length);

void Utils_BeepFAIL();
void Utils_BeepOK();

#endif

