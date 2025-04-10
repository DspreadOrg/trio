#ifndef __FUNC_H__
#define __FUNC_H__

#define     KEYNONE             0x00

void Func_DispTransTitle(byte * title);
void Func_DispRetCode(byte * title,const char * rspMsg);
void Func_RemoveCard();
int Func_EnterPIN(int isRetry);
int Func_InputNum(const char * title, const char * text, char * value, uint max, uint min, uint timeMS);
int Func_InputPwd(const char * title, const char * text, char * value, uint max, uint min, uint timeMS);

#endif

