#include "app.h"
#include "Liteui.h"

#include "pngTestIcon.h"//use for test
#include "bmpTestPic.h"
#include <stdarg.h>
#include "ui.h"

#define FONT_BOLD   1
#define FONT_NOBLD  0
#define FONT_BACKGROUND_ENABLE  1
#define FONT_BACKGROUND_DISABLE 0
#define COLOR_WITE  0xFFFF
#define COLOR_BLACK 0x0000
#define COLOR_BLUE  0x001F
#define COLOR_GREEN 0x0400
#define COLOR_PINK  0xFE19

#define TEST_ICON   "icon.png"
#define TEST_BMP    "pic.bmp"
#define TEST_CELL   "cell.png"

// #define LOAD_PNG_PIC_FILE
// #define LOAD_PNG_CELL_FILE
#define LOAD_BMP_PIC_FILE

int lcdDrawPictureBMP(int x, int y, char *filename);
int Print_drawPictureBMP(int x, char *filename);

extern int s_GetFarsiStringBuff(u16 *str_buff, int destBuffLen, const char * str);
extern u8 BitRotate(u8 data);
extern void bmpGetRGB565Data(u32 width, u32 height, const char * data, char *outData);
extern int Ui_DispBMP(u32 hpixel,u32 vpixel, u32 width, u32 height, const char * bmpdata);
static int testPngFileLoad()
{
	int ret;
	int fid;
	char fileName[9] = "icon.png";
    char bmpfileNale[8] = "pic.bmp";
    char cellIcon[9] = "cell.png";
    static int loadFlag=1;

    if(loadFlag)
        return 0;

#ifdef LOAD_PNG_PIC_FILE
	if(!fexist(fileName))
	{
		remove(fileName);

		DBG_STR("remove file");
	}

	fid = open(fileName, O_CREATE);
	if(fid == -1)
	{
		DBG_STR("O_CREATE failed");
		return -1;
	}
	ret = write(fid, iconFile, sizeof(iconFile));//iconn //iconFile
	if(ret == -1)
	{
		DBG_STR("write file failed");
		return -2;
	}
	close(fid);
#endif
#ifdef LOAD_BMP_PIC_FILE
    if(!fexist(bmpfileNale))
	{
		remove(bmpfileNale);

		DBG_STR("remove bmp file");
	}

	fid = open(bmpfileNale, O_CREATE);
	if(fid == -1)
	{
		DBG_STR("O_CREATE bmp failed");
		return -1;
	}
	ret = write(fid, gImage_a, sizeof(gImage_a));
	if(ret == -1)
	{
		DBG_STR("write bmp file failed");
		return -2;
	}
	close(fid);
#endif
#if LOAD_PNG_CELL_FILE
    //load battely icon
    if(!fexist(cellIcon))
	{
		remove(cellIcon);

		DBG_STR("remove cell file");
	}

	fid = open(cellIcon, O_CREATE);
	if(fid == -1)
	{
		DBG_STR("O_CREATE cell failed");
		return -1;
	}
	ret = write(fid, batteryIcon, sizeof(batteryIcon));
	if(ret == -1)
	{
		DBG_STR("write cell file failed");
		return -2;
	}
	close(fid);
#endif
    loadFlag=1;
    return 0;
}

//#define CUSTOM_TEST_CODE

#if defined(CUSTOM_TEST_CODE)
void Customer_Image(void)
{
	int ret;
    cLcdClear();
    ret = cLcddrawPicture(0, 0, "mainbg.png");
    if(ret == ERROR_FILE){
        cLcdPrintf(0, 0, UI_ALIGN_CENTER, "No PNG file"); 
    }
	cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);
    cFontAttrSet(FONT_ATTR_BLOD, FONT_NOBLD);
    cFontAttrSet(FONT_ATTR_COLOR, COLOR_WITE);
    cFontAttrSet(FONT_ATTR_BACKGROUND, FONT_BACKGROUND_DISABLE);   
    cLcdPrintf(40, 120, UI_ALIGN_NO, "تاجر آزمایشی");
    cLcdPrintf(52, 150, UI_ALIGN_NO, "123456789");
    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MIN);
    cLcdPrintf(300, 10, UI_ALIGN_NO, "08");
    cLcdPrintf(300, 50, UI_ALIGN_NO, "12");
    cLcdPrintf(290, 90, UI_ALIGN_NO, "1401");
    cLcdPrintf(280-5, 130, UI_ALIGN_NO, "دوشنبه");
    ret = cLcddrawPicture(320-35-1, 170, "wi.png");
    if(ret == ERROR_FILE){
        cLcdPrintf(280, 170, UI_ALIGN_CENTER, "No PNG file"); 
    }
    getkey();
}


static const char * m_keyMap_farsi[10] =
{
	"0.,*#; ~`!@$%(){}[]<>&_|\\:\"\'?/^&-+=",
	"1QZqz", 
	"2ABCabc", 
	"3DEFdef",
	"4GHIghi", 
	"5JKLjkl", 
	"6MNOmno",
	"7PRSprs", 
	"8TUVtuv", 
	"9WXYwxy",
};


#define input_str_bgp1_name "in_bgp1.png"
#define input_str_bgp2_name "in_bgp2.png"
int cLcdInputFasriStr(int start, const char *title, const char *text, char *value, int mode, uint max, uint min, bool cipher_code, uint timeMS)
{
    char data[INPUT_MAX_SIZE + 1], key_buf[64]={0}, last_key = 0, ex_key = 0, ex_times = 0;
	byte kbVal = 0;
    char disp[INPUT_MAX_SIZE + 2];
    int len = 0;
    int line, ret, same_key=0;
	uint times;
	int tm = 1;

    if (value == NULL || max == 0 || max > INPUT_MAX_SIZE || strlen(value) > max)
    {
        return UI_RET_PARAM;
    }

	cLcdClear();

	cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);
    cFontAttrSet(FONT_ATTR_BLOD, FONT_NOBLD);
	cFontAttrSet(FONT_ATTR_COLOR, COLOR_BLACK);
	cFontAttrSet(FONT_ATTR_BACKGROUND, FONT_BACKGROUND_DISABLE);

    if (title != NULL && strlen(title))
    {
    	cLcdPrintf(0,0, UI_ALIGN_CENTER, title);
    }
	else
	{
	    ret = cLcddrawPicture(0, 0, input_str_bgp1_name);
		if(ret == ERROR_FILE){
        	cLcdPrintf(0, 0, UI_ALIGN_CENTER, "No BMP file"); 
    	}

	}
	
	
    if (text != NULL)
    {
        if(strlen(text)){
            cLcdPrintf(0, 100, UI_ALIGN_CENTER, text);
        }
    }

    // deal default value
    memset(data, 0, sizeof(data));
    strcpy(data, value);
	
    while (1)
    {
        len = strlen(data);
		
        memset(disp, 0, sizeof(disp));
        memcpy(disp, data, len);
	    ret = cLcddrawPicture(25, 95, input_str_bgp2_name);
		if(ret == ERROR_FILE){
        	cLcdPrintf(25, 95, UI_ALIGN_CENTER, "No BMP file"); 
    	}

		cLcdPrintf(27, 102, UI_ALIGN_CENTER, disp);

        // wait input
        kbVal = Ui_WaitKey(timeMS);
		switch (kbVal)
		{
		case KB_NONE:
			return UI_RET_TIMEOUT;
		case KB_CANCEL:
			return UI_RET_ABORT;
		case KB_CLEAR:
			if(len<=0)
				break;
			data[--len] = 0;
			break;
		case KB0:
		case KB1:
		case KB2:
		case KB3:
		case KB4:
		case KB5:
		case KB6:
		case KB7:
		case KB8:
		case KB9:
			if(len>=max)
				break;

			if(cipher_code)
			{
				data[len++] = '*';
				break;
			}

			same_key = 0;
			if(last_key == 0)
			{
				ex_times = 0;				
				
			}
			else
			{
				if(last_key == kbVal)//same key
				{
					if(TimerCheck(tm) != 0)
					{					
						ex_times++;
						same_key = 1;
					}
					else{
						ex_times = 0;
					}
				}
				else
				{
					ex_times = 0;
				}
			}
			//DBG_STR("last_key = %02x, same_key = %d ex_times = %d, TimerCheck(tm) =%d", last_key, same_key, ex_times, TimerCheck(tm));

			memset(key_buf, 0, sizeof(key_buf));
			memcpy(key_buf, m_keyMap_farsi[(kbVal-'0')], strlen(m_keyMap_farsi[(kbVal-'0')]));
			ex_key = key_buf[ex_times%strlen(key_buf)];

			if(last_key == kbVal && same_key == 1) len--;

			data[len++] = ex_key;			

			last_key = kbVal;
			TimerSet(tm, 5);
			break;
		case KB_UP:
			memset(data, 0, sizeof(data));
			len = 0;
			break;
		default:
			break;
		}
    }
}
#endif

int FontAlignTest(void * pxPARAM)
{
    #if defined(CUSTOM_TEST_CODE)
    char input_buf[32] = {0};
	memset(input_buf, 0, sizeof(input_buf));
	cLcdInputFasriStr(0, NULL, NULL, input_buf, NULL, 16, 1, 0, 100000);
	Customer_Image();
    #endif

    cFontAttrSet(FONT_ATTR_COLOR, COLOR_BLACK);
   	cFontAttrSet(FONT_ATTR_BACKGROUND, FONT_BACKGROUND_ENABLE);


    cLcdClear();

    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);
    cFontAttrSet(FONT_ATTR_BLOD, FONT_NOBLD);

    cLcdPrintf(0, 0, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");
    cLcdPrintf(0, 32, UI_ALIGN_CENTER, "این متن ازمون فارسی اس");
    cLcdPrintf(0, 64, UI_ALIGN_RIGHT, "این متن ازمون فارسی اس");

    cLcdPrintf(0, 96, UI_ALIGN_LEFT, "TestString:%d",123);
    cLcdPrintf(0, 128, UI_ALIGN_CENTER, "TestString:%d",1);
    cLcdPrintf(0, 160, UI_ALIGN_RIGHT, "TestString:%d",1);

    getkey();

    return 0;
}

int FontDisplayWithXY(void * pxPARAM)
{
    int ret;
    
    testPngFileLoad();

    cLcdClear();

    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);
    cFontAttrSet(FONT_ATTR_BLOD, FONT_NOBLD);


    cLcdPrintf(20, 20, UI_ALIGN_NO, "این متن ازمون فارسی اس");

    cLcdPrintf(60, 60, UI_ALIGN_NO, "این متن ازمون فارسی اس");

    cFontAttrSet(FONT_ATTR_RETRACT, 50);//set retract(50 pix)

    cLcdPrintf(0, 96, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");

    cFontAttrSet(FONT_ATTR_RETRACT, 0);

    /*display the string in png*/
    ret = cLcddrawPicture(120, 140, TEST_ICON);
    if(ret == ERROR_FILE){
        cLcdPrintf(0, 160, UI_ALIGN_CENTER, "No PNG file"); 
    }
    else{
        cLcdPrintf(0, 160, UI_ALIGN_CENTER, "TestStr"); 
    }

    getkey();

    cLcdClear();
    ret = lcdDrawPictureBMP(120, 100, TEST_BMP);
    if(ret == ERROR_FILE){
        cLcdPrintf(0, 160, UI_ALIGN_CENTER, "No BMP file"); 
    }

    getkey();

    return 0;
}

int cLcdPrintfDirectionReverse(int x,int y,UI_ALIGN_MODE align_mode,const char * fmt,...);
int FontSetAttr(void * pxPARAM)
{
    cLcdClear();

    //set font size FONT_SIZE_MIN
    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MIN);
    //set font no bold
    cFontAttrSet(FONT_ATTR_BLOD, FONT_NOBLD);
    //set font color
    cFontAttrSet(FONT_ATTR_COLOR, COLOR_GREEN);
    //set font background color
    cFontAttrSet(FONT_ATTR_BACKCOLOR, COLOR_PINK);
    //enable font background
    cFontAttrSet(FONT_ATTR_BACKGROUND, FONT_BACKGROUND_ENABLE);
    cLcdPrintf(0, 0, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");

    //set font size FONT_SIZE_MEDIUM
    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);
    //set color inversion
    cFontAttrSet(FONT_COLOR_INVERSION,0);
    cLcdPrintf(0, 30, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");

    //disable font background
    cFontAttrSet(FONT_ATTR_BACKGROUND, FONT_BACKGROUND_DISABLE);
    cLcdPrintf(0, 60, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");

    //set font bold
    cFontAttrSet(FONT_ATTR_BLOD, FONT_BOLD);
    cLcdPrintf(0, 90, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");

    //set font size FONT_SIZE_MAX
    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MAX);
    cLcdPrintf(0, 130, UI_ALIGN_LEFT, "ازمون فارسی اس");
    

    getkey();

    cLcdClear();
    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);
    cLcdPrintf(0, 50, UI_ALIGN_LEFT, "DirectionReverse");
    cLcdPrintfDirectionReverse(0, 100, UI_ALIGN_LEFT, "DirectionReverse");

    getkey();

    //after end test, reset fontattr
    cFontAttrSet(FONT_ATTR_COLOR, COLOR_BLACK);
    cFontAttrSet(FONT_ATTR_BACKCOLOR, COLOR_WITE);

    return 0;
}

int LcdBackGroundTest(void * pxPARAM)
{
    UI_AREA_ST uiAreaSet;
	uiAreaSet.x = 0;
	uiAreaSet.y = 0;
	uiAreaSet.width = 320;
	uiAreaSet.height = 240;

    //set background color
	cLcdSetAttr(uiAreaSet, UI_ATTR_BCOLOR, COLOR_BLUE);
    cLcdClear();

    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);
    cLcdPrintf(0, 32, UI_ALIGN_CENTER, "این متن ازمون فارسی اس");
    cLcdPrintf(0, 64, UI_ALIGN_CENTER, "%s", "PressKey to clear Lcd");

    getkey();

    cLcdClear();
    cLcdPrintf(0, 64, UI_ALIGN_CENTER, "%s", "PressKey to end test");

    getkey();

    //after end test, reset background
    cLcdSetAttr(uiAreaSet, UI_ATTR_BCOLOR, COLOR_WITE);

    return 0;
}


int Print_Printf_WithCharInterval(int interval, int ifDirectionReverse, const char * fmt,...);

void print_left_center_right(void)
{
    /*set multiline print*/
    Print_SetAttr(PRINTER_ATTR_REVERSECOLOR, 1);
    Print_SetAttr(PRINTER_ATTR_MULTILINE, 2);
    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MAX);
    cFontAttrSet(FONT_ATTR_BLOD,FONT_NOBLD);

    /*The following means left, right, center in the same line···*/
    Print_SetAttr(PRINTER_ATTR_COMPLEXLINE, 3);
    Print_Printf(0, UI_ALIGN_RIGHT, "تاجر آزمایشی");
    Print_Printf(0, UI_ALIGN_LEFT,  "01234");
    Print_Printf(3, UI_ALIGN_CENTER, "1");


    Print_SetAttr(PRINTER_ATTR_REVERSECOLOR, 0);
    Print_SetAttr(PRINTER_ATTR_COMPLEXLINE, 2);
    Print_Printf(2, UI_ALIGN_RIGHT, "تاجر آزمایشی" );    
    Print_Printf(2, UI_ALIGN_LEFT, "43210");
    Print_SetAttr(PRINTER_ATTR_COMPLEXLINE, 0);

}

int printerTest(void * pxPARAM)
{
    int ret;
    testPngFileLoad();
	
	print_left_center_right();
    Print_SetAttr(PRINTER_ATTR_COMPLEXLINE, 0);
	Print_SetAttr(PRINTER_ATTR_REVERSECOLOR, 1);
    /*set multiline print*/
    Print_SetAttr(PRINTER_ATTR_MULTILINE, 10);
    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MAX);
    cFontAttrSet(FONT_ATTR_BLOD,FONT_NOBLD);
    Print_Printf(0, UI_ALIGN_RIGHT, "AB خوش آمدید");
    cFontAttrSet(FONT_ATTR_BLOD,FONT_NOBLD);
    Print_Printf(0, UI_ALIGN_RIGHT, "4444ABC>%d", 33);
    Print_Printf(0, UI_ALIGN_RIGHT, "این متن ازمون فارسی اس");
    cFontAttrSet(FONT_ATTR_BLOD,FONT_BOLD);
    Print_SetAttr(PRINTER_ATTR_REVERSECOLOR, 0);
    Print_Printf(0, UI_ALIGN_CENTER, "این متن ازمون فارسی اس");
    cFontAttrSet(FONT_ATTR_BLOD,FONT_NOBLD);
    Print_Printf(0, UI_ALIGN_RIGHT, "این متن ازمون فارسی اس");
    Print_Printf(0, UI_ALIGN_RIGHT, "5555ABC>%d", 33);
    cFontAttrSet(FONT_ATTR_BLOD,FONT_BOLD);
    Print_Printf(0, UI_ALIGN_CENTER, "این متن ازمون فارسی اس");
    Print_Printf(0, UI_ALIGN_RIGHT, "6666ABC>%d", 33);
    cFontAttrSet(FONT_ATTR_BLOD,FONT_NOBLD);
    Print_Printf(0, UI_ALIGN_RIGHT, "این متن ازمون فارسی اس");
    Print_Printf(0, UI_ALIGN_RIGHT, "End multiline");
	
	Print_SetAttr(PRINTER_ATTR_REVERSECOLOR, 0);
    //Print_SetAttr(PRINTER_ATTR_MULTILINE, 10);
    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MIN);
    cFontAttrSet(FONT_ATTR_BLOD,FONT_NOBLD);
    Print_Printf(0, UI_ALIGN_LEFT, "1111ABC>%d", 12);
    Print_Printf(0, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");    
    cFontAttrSet(FONT_ATTR_BLOD,FONT_BOLD);
    Print_Printf(0, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");
    

    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);
    cFontAttrSet(FONT_ATTR_BLOD,FONT_NOBLD);
    Print_Printf(0, UI_ALIGN_CENTER, "2222ABC>%d", 12);
    Print_Printf(0, UI_ALIGN_CENTER, "این متن ازمون فارسی اس");
    cFontAttrSet(FONT_ATTR_BLOD,FONT_BOLD);
    Print_Printf(0, UI_ALIGN_CENTER, "این متن ازمون فارسی اس");

    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MAX);
    cFontAttrSet(FONT_ATTR_BLOD,FONT_NOBLD);
    Print_Printf(0, UI_ALIGN_RIGHT, "3333ABC>%d", 12);
    Print_Printf(0, UI_ALIGN_RIGHT, "این متن ازمون فارسی اس");
    cFontAttrSet(FONT_ATTR_BLOD,FONT_BOLD);
    Print_Printf(0, UI_ALIGN_CENTER, "این متن ازمون فارسی اس");

    ret = Print_drawPicture(0, TEST_ICON);
    if(ret == ERROR_FILE){
        Print_Printf(0, UI_ALIGN_CENTER, "no png file");
    }
    else{
        addRowHeight(24);
        Print_drawPicture(20, TEST_ICON);
    }
    
    ret = Print_drawPictureBMP(0, TEST_BMP);
    if(ret == ERROR_FILE){
        Print_Printf(0, UI_ALIGN_CENTER, "no bmp file");
    }

    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);

    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MEDIUM);
    Print_Printf(0, UI_ALIGN_NO, "ABCDEFG>%d", 9);
    Print_SetAttr(PRINTER_ATTR_CHARINTERVAL, 10);
    Print_Printf(0, UI_ALIGN_NO, "ABCDEFG>%d", 9);
    Print_SetAttr(PRINTER_ATTR_CHARINTERVAL, 0);

    Print_SetAttr(PRINTER_ATTR_STRING_DIRECTIONREVERSE, 1);
    Print_Printf(0, UI_ALIGN_NO, "ABCDEFG>%d", 9);
    Print_SetAttr(PRINTER_ATTR_CHARINTERVAL, 10);
    Print_Printf(0, UI_ALIGN_NO, "ABCDEFG>%d", 9);
    Print_SetAttr(PRINTER_ATTR_STRING_DIRECTIONREVERSE, 0);
    Print_SetAttr(PRINTER_ATTR_CHARINTERVAL, 0);

    addRowHeight(150);
}

static int s_strlen_utf16(u16 *string)
{
	int count = 0;

	while (1)
	{
		if(string[count] == 0)
			break;

		count++;
	}
	
	return count;
}

#define MAX_STRING_LENGTH 128
#define PRINTER_LINE_MAX_WIDTH	48
#define PRINTER_LINE_MAX_HIGTH	48	
int Print_Printf_WithCharInterval(int interval, int ifDirectionReverse, const char * fmt,...)
{
	Farsi_Dot_t dot;
	va_list varg;
	int i,j,k,ret;
	u8 prnwith = 0,ch;
	char strBuff[MAX_STRING_LENGTH];
	u16 lpBuf[MAX_STRING_LENGTH/2] = {0};
    u16 tempLpBuf[MAX_STRING_LENGTH/2] = {0};
    u16 tempCharBuff[2]={0};
	u8 Printbuf[PRINTER_LINE_MAX_HIGTH*PRINTER_LINE_MAX_WIDTH];
    u8 tempBitMap[PRINTER_LINE_MAX_HIGTH*PRINTER_LINE_MAX_WIDTH];
    int charWidth, offset;

	memset(strBuff, 0, sizeof(strBuff));  
    va_start( varg, fmt );                  
    ret = vsprintf(strBuff,  fmt,  varg); 
    va_end( varg );
	if(ret >= sizeof(strBuff)){
		strBuff[sizeof(strBuff)-1] = 0;
	}
	ret = s_GetFarsiStringBuff(lpBuf, sizeof(lpBuf), strBuff);
	if(ret < 0)
	{
		return -1;
	}

	memset(Printbuf,0x00,sizeof(Printbuf));
	memset(dot.bitmap, 0, sizeof(dot.bitmap));
    memset(tempBitMap,0x00,sizeof(Printbuf));

    offset = 0;

    j=0;
    if(ifDirectionReverse){
        for(i=s_strlen_utf16(lpBuf)-1; i>=0; i--){
            tempLpBuf[j++] = lpBuf[i];
        }
        for(i=0;i<s_strlen_utf16(lpBuf);i++){
            lpBuf[i] = tempLpBuf[i];

        }
    }

    for(i=0;i<s_strlen_utf16(lpBuf);i++){
        tempCharBuff[0] = lpBuf[i];
        memset(dot.bitmap, 0, sizeof(dot.bitmap));
        LiteMGetFarsiDot(0, 0, 0, tempCharBuff, &dot);

        charWidth = (dot.width+7)/8;
        for(j=0;j<dot.height;j++){
            for(k=0;k<charWidth;k++){
                tempBitMap[j*PRINTER_LINE_MAX_WIDTH+k+offset+(interval/8)] = BitRotate(dot.bitmap[j*charWidth+k]);
            }
        }

        offset+=charWidth+(interval/8); //if want more precision,set bit instead of byte
    }
    
	PrnInit();
	PrnStart(tempBitMap, dot.height);

	return 0;
}

int inttoBCD(int decimal)
{
	int sum = 0;

	for (int i = 0; decimal > 0; i++)
	{
		sum |= ((decimal % 10 ) << ( 4*i));

		decimal /= 10;
	}

	return sum;
}

int BCDtoint( int bcd)
{
	int sum = 0, c = 1; 

	for(int i = 1; bcd > 0; i++)
	{
		if( i >= 2)
		{
			c*=10;
		}

		sum += (bcd%16) * c;

		bcd /= 16; 
	}

	return sum;
}

int icon_unLockTest(void * pxPARAM)
{
    int i,j,ret;
    UI_AREA_ST uiAreaSet;
	uiAreaSet.x = 0;
	uiAreaSet.y = 0;
	uiAreaSet.width = 320;
	uiAreaSet.height = 240;
    u8 timeBuf[32]={0};
    u8 setTime[6]={0};
    static u8 setTimeFlag=0;
    
    testPngFileLoad();

	CLcdDispStatus(0);//close the sys icon bar

    cFontAttrSet(FONT_ATTR_SIZE, FONT_SIZE_MIN);
    cLcdSetAttr(uiAreaSet, UI_ATTR_BCOLOR, COLOR_BLUE);
    uiAreaSet.x = 0;
	uiAreaSet.y = 0;
	uiAreaSet.width = 320;
	uiAreaSet.height = 24;
    cLcdSetAttr(uiAreaSet, UI_ATTR_BCOLOR, COLOR_BLACK);
    cLcdClear();

    if(!setTimeFlag){
        i=0;
        setTime[i++] = inttoBCD(23);    //year
        setTime[i++] = inttoBCD(6);    //mon
        setTime[i++] = inttoBCD(29);    //day
        setTime[i++] = inttoBCD(15);    //hour
        setTime[i++] = inttoBCD(30);    //min
        setTime[i++] = inttoBCD(00);    //sec

        SetTime(setTime);
        setTimeFlag=1;
        mdelay(3000);//wait for setting take effect
    }
    
    cFontAttrSet(FONT_ATTR_COLOR, COLOR_WITE);
    cFontAttrSet(FONT_ATTR_BACKCOLOR, COLOR_BLACK);

    ret = cLcddrawPicture(0, 0, TEST_CELL);//can use customized icon to debug
    if(ret == ERROR_FILE){
        cLcdPrintf(0, 0, UI_ALIGN_CENTER, "noFile");
    }
    cLcdPrintf(0, 64, UI_ALIGN_CENTER, "test end in %ds", 10);
    for(i=0;i<10;i++){
        cFontAttrSet(FONT_ATTR_BACKGROUND, FONT_BACKGROUND_ENABLE);
        GetTime(timeBuf);
        cLcdPrintf(0, 0, UI_ALIGN_CENTER, "%02d:%02d:%02d", BCDtoint(timeBuf[3]), BCDtoint(timeBuf[4]), BCDtoint(timeBuf[5]));

        mdelay(1000);
    }

/************* end of test**************/
    //after end test, reset background
    uiAreaSet.x = 0;
	uiAreaSet.y = 0;
	uiAreaSet.width = 320;
	uiAreaSet.height = 240;
    
    cLcdSetAttr(uiAreaSet, UI_ATTR_BCOLOR, COLOR_WITE);
    cLcdClear();
	
    cFontAttrSet(FONT_ATTR_BACKCOLOR, COLOR_WITE);
    cFontAttrSet(FONT_ATTR_COLOR, COLOR_BLACK);
    cFontAttrSet(FONT_ATTR_BACKGROUND, FONT_BACKGROUND_DISABLE);
	CLcdDispStatus(1);//exit test reset
}

typedef struct BMP_FILE_HEADER  /* size 14 */
{
    char bfType[2];         //fix as "BM"
    int  bfSize;            //file size
    char bfReserved1[2];    //reserverd as 0
    char bfReserved2[2];    //reserverd as 0
    int bfOffBits;          //bit data offset
}BMP_FH;

typedef struct BMP_INFO_HEADER  /* size: 40 */
{
   u32  biSize;		                
   u32  biWidth;		            // width of pic, unit:pix
   u32  biHeight;		            // hight of pic, unit:pix
   u16  biPlanes;		
   u16  biBitCount;	                // 1、4、8、16、24、32
   u32  biCompression;
   u32  biSizeImage;	
   u32  biXPelsPerMeter;
   u32  biYPelsPerMeter;
   u32  biClrUsed;	
   u32  biClrImportant;
} BMP_IH;

#define PRINTER_LINE_MAX_WIDTH	48	//Byte, 384bit
#define PRINTER_LINE_MAX_HIGTH	48	
//tip:this function only support BMP 16bit(RBG565)format data
int Print_drawPictureBMP(int x, char *filename)
{
    int ret = 0;
	int fid;
	long fsize;
	u8 *bmpData = NULL;
    int png_ctx;
	int i,j;
    int image_width;
    int image_height;
    BMP_IH *bfInfo;
    u16 r,g,b,gray;
    u8 *bitMap = NULL;
    int fix=0;
    u8 *printBuf = NULL;
    int bmpDataSize;
    u8* bmpRGB656Data = NULL;
    int count=0;
    u32 printBufHeight = 0;
    int writePixCount = 0;

    if(fexist(filename)<=-1)
	{
		ret = ERROR_FILE;
		goto end;
	}

	fid = open(filename, O_RDWR);
	if(fid == -1)
	{
		ret = -1;
		goto end;
	}
	fsize = filesize(filename);
	if(fsize <= 0)
	{
		ret = -1;
		goto end;
	}

	bmpData = malloc(fsize);
	if(bmpData == NULL)
	{
        DBG_STR("malloc for bmpData failed");
		ret = -1;
		goto end;
	}

	ret = read(fid, bmpData, fsize);
	if(ret == -1)
	{
		ret = -1;
		goto end;
	}
    bfInfo = bmpData+14;

    image_width = bfInfo->biWidth;
    image_height = bfInfo->biHeight;
    bmpDataSize = fsize-54;
    if(image_width%8){
		fix = 1;
	}else{
		fix = 0;
	}

    //bmpRGB656Data
    bmpRGB656Data = malloc((bmpDataSize/3)*2+(fix*image_height));//(fsize-54)/3*2
    if(bmpRGB656Data == NULL)
    {
        ret = -1;
        goto end;
    }

    bmpGetRGB565Data(image_width, image_height,bmpData,bmpRGB656Data);
    if(bmpData != NULL){
        free(bmpData);
        bmpData = NULL;
    }

    bitMap = (u8*)malloc((bmpDataSize/3)*2+(fix*image_height));
	if(bitMap==NULL){
		ret = -1;
        DBG_STR("mallco bitMap failed");
        goto end;
	}
    memset(bitMap, 0, (bmpDataSize/3)*2+(fix*image_height));

    j=0;
    writePixCount = 1;
	for(i=0;i<bmpDataSize/2;i++){
		r = (((u16*)bmpRGB656Data)[i]&0xf800)>>8;
		g = (((u16*)bmpRGB656Data)[i]&0x07e0)>>3;
		b = (((u16*)bmpRGB656Data)[i]&0x001f)<<3;
		gray = (r*30 + g*59 + b*11)/100;
		if(gray<128){
			bitMap[j/8] |= (0x80>>(j%8));
		}
		
        writePixCount++;
		j++;
		if(fix==1 && !(writePixCount%(image_width+1))){
			j += (8-(image_width%8));
			writePixCount=1;
		}
	}
    if(bmpRGB656Data != NULL){
        free(bmpRGB656Data);
        bmpRGB656Data = NULL;
    }
	
    printBufHeight = image_height+1;
	printBuf = (u8*)malloc(PRINTER_LINE_MAX_WIDTH*printBufHeight);
	if(printBuf == NULL) {
		ret = -1;
        goto end;
    }
    memset(printBuf, 0, (PRINTER_LINE_MAX_WIDTH*printBufHeight));

    count=0;
    for(i = image_height-1;i>=0; i--){
		for(j = 0; j < (image_width/8+fix); j++){
			printBuf[i*PRINTER_LINE_MAX_WIDTH+j+(x/8)] =bitMap[(count)*(image_width/8+fix)+j];
		}
        count++;
	}
    if(bitMap != NULL){
        free(bitMap);
        bitMap = NULL;
    }

	PrnInit();
	PrnStart(printBuf, printBufHeight);

end:
	if(bmpData!=NULL){
		free(bmpData);
        bmpData = NULL;
	}

    if(printBuf!=NULL){
		free(printBuf);
        printBuf = NULL;
	}

	if(bitMap!=NULL){
		free(bitMap);
        bitMap = NULL;
	}

    if(bmpRGB656Data != NULL){
        free(bmpRGB656Data);
        bmpRGB656Data = NULL;
    }

	return ret;
}

int lcdDrawPictureBMP(int x, int y, char *filename)
{
    int ret = 0;
	int fid;
	long fsize;
	char *bmpData = NULL;
    int png_ctx;
    int image_height=384;
	int i,j;
    BMP_FH *bfHeader;
    BMP_IH *bfInfo;

    if(fexist(filename)<=-1)
	{
		ret = ERROR_FILE;
		goto end;
	}
    
	fid = open(filename, O_RDWR);
	if(fid == -1)
	{
		ret = ERROR_FILE;
		goto end;
	}
	fsize = filesize(filename);
	if(fsize <= 0)
	{
		ret = ERROR_FILE;
		goto end;
	}
	bmpData = malloc(fsize);
	if(bmpData == NULL)
	{
		ret = ERROR_FILE;
		goto end;
	}
	ret = read(fid, bmpData, fsize);
	if(ret == -1)
	{
		ret = ERROR_FILE;
		goto end;
	}

    bfHeader = bmpData;
    bfInfo = bmpData+14;
    
    Ui_DispBMP(x, y, bfInfo->biWidth, bfInfo->biHeight, bmpData);

end:
	if(bmpData!=NULL){
		free(bmpData);
	}
	
	return ret;
}

int cLcdPrintfDirectionReverse(int x,int y,UI_ALIGN_MODE align_mode,const char * fmt,...)
{
	va_list varg;
	Farsi_Dot_t farsiDot;
    int i,j,ret;
    char strBuff[MAX_STRING_LENGTH]={0};
    char tempStrBuff[MAX_FARSI_HEIGHT]={0};
    
    va_start( varg, fmt );                  
    ret = vsprintf(strBuff,  fmt,  varg); 
    va_end( varg );
	if(ret >= sizeof(strBuff)){
		strBuff[sizeof(strBuff)-1] = 0;
	}

    for(i=strlen(strBuff)-1; i>=0; i--){
        tempStrBuff[j++] = strBuff[i];
    }

    ret = cLcdPrintf(x, y, align_mode, tempStrBuff);

	return ret;
}

int backLitghtTest(void * pxPARAM)
{
    CLcdBacklightCtrl(0);

    getkey();

    CLcdBacklightCtrl(100);
}