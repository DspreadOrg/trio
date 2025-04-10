#include "app.h"
#include "secapi.h"
#include "func.h"
#include "ui.h"
#include "utils.h"

#ifdef FUNC_DEBUG
#define FUNC_STRLOG DBG_STR
#define FUNC_HEXLOG DBG_HEX
#else
#define FUNC_STRLOG
#define FUNC_HEXLOG
#endif

void Func_DispTransTitle(byte * title)
{
	Ui_Clear();
	Ui_ClearLineColor(0, DISPLAY_FONT_COLOR);
	Ui_DispTextLineAlign(0, DISPLAY_CENTER, title, 1);
}

void Func_DispRetCode(byte * title,const char * rspMsg)
{	
	int i;
	char buf[DISPLAY_LINE_EN_NUM + 1];
	
	Func_DispTransTitle(title);
	memset(buf, 0, sizeof(buf));
	i = Utils_StrCopy(buf, rspMsg, Ui_GetLineEnNum());
	Ui_DispTextLineAlign(2, DISPLAY_CENTER, buf, 0);
	if(i < strlen(rspMsg)) {
		Ui_DispTextLineAlign(3, DISPLAY_CENTER, rspMsg+ i, 0);
	}
	Ui_WaitKey(5000);
	return;
}

int Func_DispInfo(const char *pTitle, const char *pMsg, int timeOutMs)
{
	int   num=0;
	int   page;
	int   pageNo;
	int   pageSize;
	int   titleFlag = 0;
	char* pStr=NULL;
	char* pStart=NULL;
	char  apList[20][30]={0};
	char  Tem[128]={0};

	Ui_Clear();
	Ui_ClearKey();
	if(pTitle != NULL) {
		Ui_DispTitle(pTitle);
		titleFlag = 1;
	}
	pStart = (char *)pMsg;
	while(1)
	{
		pStr=strstr(pStart,"\n");
		if(pStr==NULL)
			break;
		memset(Tem,0,sizeof(Tem));
		memcpy(Tem,pStart,pStr-pStart);
        if(strlen(Tem) <= Ui_GetLineEnNum()) {
			strcpy(apList[num++],Tem);
        }
		else
		{
			int i=0,j=0;
			while(Tem[i])
			{
				if(j >= Ui_GetLineEnNum())
				{
					num++;
					j=0;
				}
				if(Tem[i] & 0x80)
				{
				   if((j + 2) > Ui_GetLineEnNum()) {
				   		num++;
						j = 0;
				   }
				   memcpy(apList[num]+j,Tem+i,2);	
				   i+=2;
				   j+=2;
				}
				else
				    apList[num][j++]=Tem[i++];	
			}
		    num++;
		}
		pStart=pStr+1;
	}
	pageSize = Ui_GetLineNum() - 1 - titleFlag;
	page = (num + pageSize - 1) / pageSize;
	for(pageNo = 0; pageNo < page; ) {
		int line = 0;
		int lineNo;
		byte key;
		Ui_ClearLine(titleFlag, pageSize);
		line = num - pageNo * pageSize;
		if(line > pageSize) {
			line = pageSize;
		}
		for(lineNo = 0; lineNo < line; lineNo ++) {
			Ui_DispTextLineAlign(titleFlag+lineNo, DISPLAY_LEFT, apList[pageNo * pageSize + lineNo], 0);
		}
		key = Ui_WaitKey(timeOutMs);
		switch(key)
		{
			case KB_CANCEL:
			case KB_NONE : 
				return KB_CANCEL;
			case KB_ENTER:
				return KB_ENTER;
			case KB_UP:
				if(pageNo > 0) {
					pageNo --;
				}
				break;
			case KB_DOWN:
				if(pageNo < page - 1) {
					pageNo ++;
				}
				break;
		}
	}
}

void Func_RemoveCard()
{
	char tmp[100];
	
	if(IccDetect(0) == 0) {
		Ui_ClearLine(1, Ui_GetLineNum()-2);
		Utils_BeepFAIL();
		Ui_DispTextLineAlign(3, DISPLAY_CENTER, "pls unplug card", 0);
		while (IccDetect(0)== 0);
	}
	if(PiccDetect(0x01,NULL,tmp,NULL,NULL) == 0) {
		Ui_ClearLine(1, Ui_GetLineNum()-2);
		Utils_BeepFAIL();
		Ui_DispTextLineAlign(3, DISPLAY_CENTER, "pls remove card", 0);
		while (PiccDetect(0x01,NULL,tmp,NULL,NULL) == 0);
	}
	PiccRemove('e',0);
	PiccClose();
	IccClose(0);
}

int Func_EnterPIN(int isBypass)
{
	int iRet;
	ST_TRANLOG *    pxLOG = App_GetTranLog();
	
	Ui_ClearLine(1, Ui_GetLineNum()-2);
	if(strlen(pxLOG->amt) > 0) {
		char disp[20];
		memset(disp, 0, sizeof(disp));
		Utils_Asc2Amt(pxLOG->amt, disp, 20);
		Ui_DispTextLineAlign(1, DISPLAY_RIGHT, disp, 0);
		Ui_DispTextLineAlign(2, DISPLAY_LEFT, "pls input online pin:", 0);
	} else {
		Ui_DispTextLineAlign(1, DISPLAY_LEFT, "pls input online pin:", 0);
	}
	PedSetPosition(120,96);	
	memset(pxLOG->pin, 0, sizeof(pxLOG->pin));
	iRet =PedGetPinBlock(SEC_PINKEY_INDEX,"0,4,5,6,7,8,9,10,11,12", pxLOG->card, pxLOG->pin,0x00,APP_UI_TIMEOUT);
	if(iRet == SEC_RET_OK)
		pxLOG->entryMode[2] = '1';

	return iRet;
}

int Func_Input(int start, const char *title, const char *text, char *value, int mode, uint max, uint min, uint timeMS)
{
    char data[INPUT_MAX_SIZE + 1];
    int line;
	char curKey;
	uint times;

    if (text == NULL || value == NULL || max == 0 || max > INPUT_MAX_SIZE || strlen(value) > max)
    {
        return UI_RET_PARAM;
    }

	Ui_ClearKey();
	line = start;
    if (title != NULL && strlen(title))
    {
    	Ui_DispTitle(title);
    }
	// Ui_ClearLine(start, Ui_GetLineNum()-1);
	
    if (text != NULL)
    {
        if(strlen(text)){
            Ui_DispTextLineAlign(line++, DISPLAY_LEFT, text, 0);
        }
    }

    // deal default value
    memset(data, 0, sizeof(data));
    strcpy(data, value);
	if(strlen(data)) {
		times = 0;
		curKey = Ui_GetKeyMask(data[strlen(data) - 1], &times);
	} else {
		curKey = KB_NONE;
	}
	
    while (1)
    {
        byte kbVal = 0;
        char disp[INPUT_MAX_SIZE + 2];
        int len = strlen(data);
		
        // format to display
        memset(disp, 0, sizeof(disp));
        if (len > Ui_GetLineEnNum())
        {
        	if(mode == INPUT_PWD) {
				memset(disp, '*', Ui_GetLineEnNum());
				disp[Ui_GetLineEnNum()] = 0;
        	} else {
            	strcpy(disp, data + len - Ui_GetLineEnNum());
        	}
        }
        else
        {
        	if(mode == INPUT_PWD) {
				memset(disp, '*', len);
				disp[len] = 0;
        	} else {
           		strcpy(disp, data);
        	}
        }
		
		Ui_ClearLine(line, line);
		Ui_DispTextLineAlign(line, DISPLAY_RIGHT, disp, 0);
        
        // wait input
        kbVal = Ui_WaitKey(timeMS);
        if (kbVal == KB_NONE)
        {
            return UI_RET_TIMEOUT;
        }
		if((kbVal == KB_FN || kbVal == KB_ALPHA) && curKey != KB_NONE) {
			if(mode == INPUT_STR || mode == INPUT_HEX || mode == INPUT_IP) {
				times ++;
				kbVal = Ui_GetRealKey(curKey, times, mode);
		        data[len-1] = kbVal;
			}
			continue;
		}
		
		if(kbVal >= KB0 && kbVal <= KB9) {
			curKey = kbVal;
			times = 0;
	        if (len < max) {
	            data[len] = curKey;
	        } else {
	            data[len-1] = curKey;
	        }
			continue;
		}
		
        switch (kbVal)
        {
		case KB_DOWN:
			if(mode == INPUT_IP) {
				curKey = kbVal;
				times = 0;
		        if (len < max) {
		            data[len] = '.';
		        } else {
		            data[len-1] = '.';
		        }
			}
			break;
			
        case KB_CLEAR:
            if (len > 0) {
                data[len-1] = 0;
				times = 0;
				curKey = Ui_GetKeyMask(data[len-1], &times);
            }
            break;

        case KB_ENTER:
            if (len >= min)
            {
                strcpy(value, data);
                return UI_RET_SUCCESS;
            }
            break;

        case KB_CANCEL:
            return UI_RET_ABORT;

        default:
            break;
        }
    }
}

int Func_InputNum(const char * title, const char * text, char * value, uint max, uint min, uint timeMS)
{
	return Func_Input(1, title, text,  value, INPUT_NUM, max, min, timeMS);
}

int Func_InputPwd(const char * title, const char * text, char * value, uint max, uint min, uint timeMS)
{
	return Func_Input(1, title, text,  value, INPUT_PWD, max, min, timeMS);
}


