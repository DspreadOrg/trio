#include "app.h"
#include "utils.h"

int Utils_Int2Hex(uint IntData, byte * HexBuf, int HexLen)
{
	byte off = 0;
	
	if(HexLen >= 4) {
		HexBuf[off++] = (byte)((IntData >> 24) & 0xFF);
	}
	if(HexLen >= 3) {
		HexBuf[off++] = (byte)((IntData >> 16) & 0xFF);
	}
	if(HexLen >= 2) {
		HexBuf[off++] = (byte)((IntData >> 8) & 0xFF);
	}
	HexBuf[off++] = (byte)((IntData) & 0xFF);
	
	return 0;
}

uint Utils_Hex2Int(byte *HexBuf,  int HexLen)
{
	uint iVal = 0;
	byte off = 0;
	
	while(off < HexLen) {
		iVal <<= 8;
		iVal |= HexBuf[off++];
	}
	return iVal;
}

int Utils_Int2Bcd(uint IntData, byte * BcdBuf, int BcdLen)
{
	byte tmp[32];

	memset(tmp, 0, sizeof(tmp));
	Utils_Int2Str((int)IntData, tmp, BcdLen * 2);
	Utils_Asc2Bcd(tmp, strlen(tmp), BcdBuf);

	return 0;
}

uint Utils_Bcd2Int(byte *BcdBuf,  int BcdLen)
{
	uint iVal = 0;
	byte tmp[32];

	memset(tmp, 0, sizeof(tmp));
	Utils_Bcd2Asc(BcdBuf, BcdLen, tmp);
	return (uint)atoi(tmp);
}

int Utils_Bcd2Asc(byte *bcd, uint bcdLen, byte *asc)
{
	const char ucHexToChar[16 + 1] = {"0123456789ABCDEF"};
	int uiCnt;

	if ((bcd == NULL) || (asc == NULL) || (bcdLen <= 0))
	{
		return -1;
	}

	for (uiCnt = 0; uiCnt < bcdLen; uiCnt++)
	{
		asc[2 * uiCnt] = (byte)ucHexToChar[(bcd[uiCnt] >> 4)];
		asc[2 * uiCnt + 1] = (byte)ucHexToChar[(bcd[uiCnt] & 0x0F)];
	}
	
	return 0;
}

static int Utils_Char2Bcd(char ch)
{
	if ((ch >= 'a') && (ch <= 'f'))
	{
		return (ch - 'a' + 0x0A);
	}
	if ((ch >= 'A') && (ch <= 'F'))
	{
		return (ch - 'A' + 0x0A);
	}
	else if((ch >= '0') && (ch <= '9'))
	{
		return (ch & 0x0F);
	}

	return -1;
}

int Utils_Asc2Bcd(byte *asc, uint ascLen, byte *bcd)
{
	int uiCnt;

	if ((asc == NULL) || (bcd == NULL) || (ascLen <= 0))
	{
		return -1;
	}

	for (uiCnt = 0; uiCnt < ascLen; uiCnt += 2)
	{
		byte tmp;
	
		tmp = Utils_Char2Bcd(asc[uiCnt]);
		if (tmp < 0)
		{
			return -1;
		}
		bcd[uiCnt / 2] = (tmp << 4);

		tmp = Utils_Char2Bcd(asc[uiCnt + 1]);
		if (tmp < 0)
		{
			return -1;
		}
		bcd[uiCnt / 2] |= tmp;
	}

	return 0;
}

int Utils_Int2Str(int iVal, char * str, uint len)
{
#if 0
	int sign;
	char *ptr;
	if(str == NULL) {
		return -1;
	}
	
	ptr = str;

	if ((sign = iVal) < 0) {
		iVal = -iVal;
	}

	do{
		*ptr++ = iVal % 10 + '0';
	}while((iVal/=10)>0);

	if(sign<0){
		*ptr++='-';
	}
	
	*ptr='\0';
	
	reverse(str);
	if(strlen(str) < len) {
		memmove(str+len-strlen(str), str, strlen(str));
		memset(str, '0', len-strlen(str));
	}
#else
	char tmp[20];
	sprintf(tmp, "%d", iVal);
	if(strlen(tmp) < len) {
		memset(str, '0', len-strlen(tmp));
		memcpy(str + len -strlen(tmp), tmp, strlen(tmp));
	} else {
		memcpy(str, tmp, strlen(tmp));
	}
#endif
	return 0;
}

int Utils_Str2Int(const char * str)
{
	return atoi(str);
}

void Utils_Int2Amt(uint amt, char * strAmt)
{
	char tmp[20];
	char tmp2[5];

	memset(tmp, 0, sizeof(tmp));
	Utils_Int2Str(amt/100, tmp, 0);
	memset(tmp2, 0, sizeof(tmp2));
	Utils_Int2Str(amt%100, tmp2, 2);
	strcpy(strAmt, tmp);
	strcat(strAmt, ".");
	strcpy(strAmt, tmp2);
}

uint Utils_Amt2Int(const char * strAmt)
{
	char tmp[20];
	char * pxDot;

	memset(tmp, 0, sizeof(tmp));
	
	pxDot = strstr(strAmt, ".");
	if(pxDot == NULL) {
		strcpy(tmp, strAmt);
		strcat(tmp, "00");
	} else {
		int less = strlen(pxDot+1);
		memcpy(tmp, strAmt, pxDot-strAmt);
		if(less == 0){
			strcat(tmp, "00");
		} else if(less == 1) {
			strcat(tmp, "0");
		} else {
			memcpy(tmp, pxDot + 1, 2);
		}
	}

	return (uint) atol(tmp);
}

int Utils_Amt2Asc(const char * strAmt, char * ascAmt, int amtSize)
{
	char * pxDot;
	int  iLen ;
	char tmp[30];

	if(strAmt == NULL || ascAmt == NULL || amtSize <= strlen(strAmt) + 2) {
		return -1;
	}
	
	memset(tmp, 0, sizeof(tmp));
	pxDot = strstr(strAmt, ".");
	if(pxDot == NULL) {
		strcpy(tmp, strAmt);
		strcat(tmp, "00");
	} else {
		iLen =  strlen(strAmt) - strlen(pxDot);
		memcpy(tmp, strAmt, iLen);
		iLen =  strlen(pxDot + 1);
		if(iLen >= 2) {
			memcpy(tmp + strlen(tmp), pxDot + 1, 2);
		}else if( iLen == 1) {
			strcat(tmp, pxDot + 1);
			strcat(tmp, "0");
		} else {
			strcat(tmp, "00");
		}
	}

	iLen = 0;
	while(tmp[iLen++] == '0');
	
	strcpy(ascAmt, tmp);

	return 0;
}

int Utils_Asc2Amt(const char * ascAmt, char * strAmt, int strSize)
{
	int len;
	int idx = 0;

	while(ascAmt[idx] == '0'){ 
		idx ++;
	}
	
	len = strlen(ascAmt + idx);
	if(len +  4 > strSize){
		return 0;
	}
	if(0 >= len){
		strcpy(strAmt, "0.00");
	}
	else if(1 == len){
		strcpy(strAmt, "0.0");
		strcat(strAmt, ascAmt + idx);
	}
	else if(2 == len){
		strcpy(strAmt, "0.");
		strcat(strAmt, ascAmt + idx);
	}	else {
		memcpy(strAmt, ascAmt + idx, len - 2);
		strcat(strAmt, ".");
		strcat(strAmt,  ascAmt + idx + len - 2);
	}

	return 0;
}

int Utils_StrCopy(char * dst, const char *src, int len)
{
	char *p = dst;
	int icNum = 0, i = 0, ic = 0, pos = 0;

	memcpy(p, src, len);
	p[len] = 0x00;
	ic = strlen(p);

	if (ic>0){
		for(i=0; i<ic; i++){
			if ( (unsigned char)(*(p+i))==0x00 ) { break;   }
			if ( (unsigned char)(*(p+i))>0xA0 )  { icNum++; pos = i; }
		}
		if ( icNum%2==1 ) { *(p+pos) = 0x00; ic = pos; } 
	}

	return (ic);

}


/*获取数字串长度*/
uint Utils_GetNumLen(byte * data, uint len)
{
	uint rlen;
	for(rlen=0; rlen<len; rlen++)
	{
		if(data[rlen] < '0' || data[rlen] > '9')
		{
			break;
		}
	}
	return rlen;
}

/*ASC数字串右对齐，左补'0' */
void Utils_PaddingRight(byte * data, uint len)
{
	uint newlen;
	newlen = Utils_GetNumLen(data, len);
	if(len != newlen)
	{
		memmove(data+len-newlen, data, newlen);
		memset(data, '0', len-newlen);
	}
}

byte Utils_AscAdd(byte *augend, byte *addend, uint len) 
{
	uint len2;
	byte carry,by,ch2;

	Utils_PaddingRight(augend, len);
	len2=Utils_GetNumLen(addend, len);

	carry = 0xa0;
	while(1)
	{
		len--;
		if(len2 != 0)
		{
			len2--;
			ch2 = addend[len2];
		}
		else
		{
			ch2 = '0';
		}
		by = augend[len] + carry+ ch2;
		carry = 0xa0;
		if (by > 9) 
		{
			by += 6;
			carry = 0xa1;
      	}
      	augend[len] = by|'0';
		if(len == 0) break;
	}
	
	return (carry-0xa0);
}

byte Utils_AscSub(byte *minuend, byte *subtrahend, uint len) 
{
	uint len2;
	byte carry,tmp,ch2;

	Utils_PaddingRight(minuend, len);
	len2=Utils_GetNumLen(subtrahend, len);

	carry=0;
	while(1) 
	{
		len--;
		if(len2!=0)
		{
			len2--;
			ch2=subtrahend[len2];
		}
		else
		{
			ch2='0';
		}

		tmp = minuend[len];
		if(tmp >= ch2 + carry){
			tmp = tmp - ch2 - carry;
			carry = 0;
		}
		else{
			tmp = tmp+10-ch2-carry;
			carry = 1;
		}
   	  	minuend[len]=tmp|'0';
		if(len == 0) break;
	}
	
	return (carry);
}

void Utils_Xor(byte *psVect1, byte *psVect2, uint uiLength, byte *psOut)
{
    uint   uiCnt;

	if ((psVect1 == NULL) || (psVect2 == NULL) || (psOut == NULL))
	{
		return;
	}

	memcpy(psOut, psVect1, uiLength); 
	
    for(uiCnt = 0; uiCnt < uiLength; uiCnt++)
    {
        psOut[uiCnt] ^= psVect2[uiCnt];
    }
}

static int Utils_GetEMVTag(byte * data, uint dataLen, ushort * tag)
{
  uint len = 0;

  while(len < dataLen)
  {
    if(len == 0 && (data[len] & 0x1F) == 0x1F) {
      len ++;
      continue;
    } else if(len > 0 && (data[len] & 0x80) == 0x80) {
      len ++;
      continue;
    }
    len ++;
    if(len > MAX_TAG_LEN)
    {
      return -1;
    }
	*tag = (ushort)Utils_Hex2Int(data, len);
    return len;
  }
  
  return -1;
}

static int Utils_GetEMVTagValueLen(byte * data, uint dataLen, int * valLen)
{
  uint lenLen = 0;

  if((data[0] & 0x80) == 0x80)
  {
    lenLen = (data[0] & 0x7F);
    if(lenLen > MAX_TAG_LEN_LEN){
      return -1;
    }
	*valLen = (int)Utils_Hex2Int(&data[1], lenLen);
    return lenLen + 1;
  }
  *valLen = data[0];
  
  return 1;
}

static ST_EMVTAG * Utils_GetEMVTagDef(const ST_EMVTAG * tagList, uint tagNum, ushort tag)
{
  uint idx = 0;

  for(idx = 0; idx < tagNum; idx ++)
  {
    if(tagList[idx].tag == tag)
    {
      return (ST_EMVTAG *)&tagList[idx];
    }
  }

  return NULL;
}

int Utils_AddEMVTLV(byte * data, uint dataLen, ushort tag, byte *val, uint valLen)
{
	byte tagName[2];
	
	if(data == NULL || val == NULL || valLen > 127) {
		return -1;
	}
	
	Utils_Int2Hex(tag, tagName, 2);
	if(tagName[0] == 0) {
		memcpy(data + dataLen, tagName+1, 1);
		dataLen += 1;
	} else {
		memcpy(data + dataLen, tagName, 2);
		dataLen += 2;
	}
	data[dataLen++] = (byte)valLen;
	memcpy(data + dataLen, val, valLen);
	
	return dataLen + valLen;
}

int Utils_DelEMVTLV(byte * data, uint dataLen, ushort tag)
{
  int iRet = 0;
  int offset = 0;

  while(offset < dataLen)
  {
    int          valLen = 0;
    int          lenLen = 0;
    int          tagLen = 0;
    ushort       tagName;

    tagLen = Utils_GetEMVTag(data + offset, dataLen - offset, &tagName);
    if(tagLen <= 0)
    {
      return -1;
    }
    offset += tagLen;

    lenLen = Utils_GetEMVTagValueLen(data+ offset, dataLen - offset, &valLen);
    if(lenLen < 0)
    {
      return -1;
    }
    offset += lenLen;

    if(offset + valLen > dataLen)
    {
      return -1;
    }
	offset += valLen;
	if(tagName != tag) {
		continue;
	}
	
	tagLen = (tagLen + lenLen + valLen);
	memmove(data + offset - tagLen, data + offset, tagLen);
	return dataLen - tagLen;
  }
  
  return dataLen;
}

int Utils_ModifiyTLV(byte * data, uint dataLen, ushort tag, byte *newVal, uint newValLen)
{
	int newLen = Utils_DelEMVTLV(data, dataLen, tag);
	if(newLen < 0) {
		return newLen;
	}
	return Utils_AddEMVTLV(data, (uint)newLen, tag, newVal, newValLen);
}

int Utils_GetEMVTLV(byte * data, uint dataLen, ushort tag, byte * val, uint valSize)
{
  int iRet = 0;
  int offset = 0;

  while(offset < dataLen)
  {
    int          valLen = 0;
    int          lenLen = 0;
    int          tagLen = 0;
    ushort       tagName;

    tagLen = Utils_GetEMVTag(data + offset, dataLen - offset, &tagName);
    if(tagLen <= 0)
    {
      return -1;
    }
    offset += tagLen;

    lenLen = Utils_GetEMVTagValueLen(data+ offset, dataLen - offset, &valLen);
    if(lenLen < 0)
    {
      return -1;
    }
    offset += lenLen;

    if(offset + valLen > dataLen)
    {
      return -1;
    }
	if(tagName != tag) {
		offset += valLen;
		continue;
	}
	
    if(val != NULL)
    {
    	if(valLen > valSize) {
			return -2;
    	}
    	memcpy(val, data + offset, valLen);
    }
	
	return valLen;
  }
  
  return -3;
}


int Utils_DecodeEMVTlv(byte * data, uint dataLen, const ST_EMVTAG * tagList, uint tagNum)
{
  int iRet = 0;
  int offset = 0;
  
  // DBG_STR("dataLen = %d\r\n", dataLen);

  while(offset < dataLen)
  {
    int          valLen = 0;
    int          lenLen = 0;
    int          tagLen = 0;
    ushort       tag;
    ST_EMVTAG *  tagItem = NULL;

    tagLen = Utils_GetEMVTag(data + offset, dataLen - offset, &tag);
    if(tagLen <= 0)
    {
      return -1;
    }
    offset += tagLen;

    lenLen = Utils_GetEMVTagValueLen(data + offset, dataLen - offset, &valLen);
    if(lenLen < 0)
    {
      return -2;
    }
    offset += lenLen;

    if(offset + valLen > dataLen)
    {
      // DBG_STR("dataLen = %d, offset = %d, tagLen = %d, lenLen = %d, valLen = %d, tag = %04X\r\n", dataLen, offset, tagLen, lenLen, valLen, tag);
      return -3;
    }
    tagItem = Utils_GetEMVTagDef(tagList, tagNum, tag);
    if(tagItem == NULL)
    {// tag not found in list
        offset += valLen;
        continue;
    }
    if(tagItem->dataSize < valLen || valLen < 0 || valLen > MAX_TAG_VAL_LEN)
    {// value length more then data size
      return -4;
    }
    if(tagItem->data != NULL)
    {// set data
      memcpy(tagItem->data, data + offset, valLen);
    }
    if(tagItem->dataLen != NULL)
    {// set data len
      *tagItem->dataLen = (unsigned char)valLen;
    }
    if(tagItem->flag != NULL)
    {// set data flag
      *tagItem->flag = 1;
    }

    offset += valLen;
  }

  return 0;
}

int Utils_IsLeapYear(int year)
{
    if ((year % 400) == 0) {
        return 1;
    } else if ((year % 100) == 0) {
        return 0;
    } else if ((year % 4) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int Utils_DayOfMonth(int month, int year)
{
	const unsigned char g_day_per_mon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month != 2) {
        return g_day_per_mon[month - 1];
    } else {
        return g_day_per_mon[1] + Utils_IsLeapYear(year);
    }
}

// for level 2 
void Util_Bcd2Asc(unsigned char *bcd, int bcdLen, unsigned char *asc)
{
	Utils_Bcd2Asc(bcd, (uint)bcdLen, asc);
}

void Util_Asc2Bcd(unsigned char *asc, int ascLen, unsigned char *bcd)
{
	Utils_Asc2Bcd(asc, (uint)ascLen, bcd);
}


void Utils_Escape(const char * src, char * dst)
{
	int idx;
	int dispLen;
    int len = strlen(src);
	
	for(idx = 0, dispLen = 0; idx < strlen(src); idx++) {
		if(src[idx] == '%') {
			dst[dispLen ++] = '%';
			dst[dispLen ++] = '%';
		} else {
			dst[dispLen ++] = src[idx];
		}
	}
}

char * Utils_TrimStr(char *pszString)
{
#define ISSPACE(ch) ( ((ch) == ' ')  || ((ch) == '\t') || \
	((ch) == '\n') || ((ch) == '\r') )

	char *p, *q;

	if( !pszString || !*pszString )
	{
		return pszString;
	}

	p = &pszString[strlen((char*)pszString) - 1];
	while(( p > pszString) && ISSPACE(*p) )
	{
		*p-- = 0;
	}
	if( (p == pszString) && ISSPACE(*p) )  *p = 0;

	for(p = pszString; *p && ISSPACE(*p); p++);
	if( p != pszString )
	{
		q = pszString;
		while( *p )   *q++ = *p++;
		*q = 0;
	}
#undef ISSPACE

	return pszString;
}

void Utils_PrintHex(byte *data, uint len)
{
    uint i;
    char hex[80];
	char disp[20];

	if (len <= 0)
		return;

	memset(hex, 0, sizeof(hex));
	memset(disp, 0, sizeof(disp));
    for(i=0; i<len; ){
		sprintf(hex + strlen(hex), "%02X ", data[i]);
		disp[i%16] = data[i];
		if(data[i] < 0x20 || data[i] > 0x7E ) {
			disp[i%16] = '.';
		}
        i++;
        if(!(i%16)) {
			strcat(hex, " : ");
			strcat(hex, disp);
			strcat(hex, "\r\n");
            usb_debug(hex);
            //DelayMs(10);
			memset(hex, 0, sizeof(hex));
			memset(disp, 0, sizeof(disp));
			//usb_debug("\r\n======i=%d===len=%d=\r\n", i, len);
		}
		
	}
	strcat(hex, " : ");
	strcat(hex, disp);
	strcat(hex, "\r\n");
    usb_debug(hex);
    //DelayMs(10);
}

void Utils_BeepOK()
{
    SoundPlay("4000:250;", 2);
}

void Utils_BeepFAIL()
{
    SoundPlay("4750:250;", 2);
    DelayMs(350);
    SoundPlay("4750:250;", 2);
    DelayMs(350);
    SoundPlay("4750:250;", 2);
    DelayMs(350);
}


