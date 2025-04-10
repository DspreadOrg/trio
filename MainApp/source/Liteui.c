#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "app.h"
#include "Farsi_api.h"
#include "Liteui.h"
#include "spng.h"
#include "draw_png.h"
#include "ui.h"

#define STRING_LINE_WIDTH 320
#define STRING_LINE_HIGTH 48
#define MAX_STRING_LENGTH 512
static u16 	*s_lcdMap = NULL;
static u16 	*s_lcdBackGroundMap = NULL;
static int 	s_lcdWidth=0;
static int	s_lcdHeight=0;

static u16 	s_fontColor = 0x0000;//deful black
static u16	s_fontBackGronndColor = 0xffff;//deful wite
static u32	s_fontSize=0;
static u32	s_fontBlod=0;
static int  s_fontRetract=0;
static u8 	s_fontBackGroundEnable = 0;


#define PRINTER_LINE_MAX_WIDTH	48	//Byte, 384bit
#define PRINTER_LINE_MAX_HIGTH	48	
#define PRINTER_STRING_MULT_LINE 10
static u32 s_printBuffOffSet = 0;
static u32 s_printBuffLine = 0;
static u32 s_printBuffLineCount = 0;
static u8 *s_printMultiLineBuff = NULL;

extern int png_decode_init(u8 * png, u32 len);
extern int png_decode_size(int context, size_t *size, u32 *width, u32 *height);
extern int png_decode(int context, u8 *out, size_t size);
extern void png_decoder_uninit(int context);

static int s_Utf8ToUtf16(char *dest,const char *src,int destMaxLen)
{
    int destLen=0;
    int i=0;
    char temp;
    while(i<strlen(src))
    {
       if(destMaxLen-destLen>=2)
        {
            if ((src[i] | '\x7f')=='\x7f')
			//U+0000-U+007F
            {
                dest[destLen++]=0x00;
                dest[destLen++]=src[i];
                i=i+1;
            }
			else if((src[i] | '\x1f')=='\xdf')
			//U+0080-U+07FF
			{
				dest[destLen++]=((0x1c & src[i])>>2);
				dest[destLen++]=((0x3f & src[i+1]))+((0x03 &src[i])*64);
				i=i+2;
			}
			else if((src[i] | '\x0f')=='\xef')
			//U+0800-U+FFFF
			{
				dest[destLen++]=((0x0f & src[i])*16)+((0x3c & src[i+1])>>2);
				dest[destLen++]=(0x03 & src[i+1])*64+(0x3f & src[i+2]);
				i=i+3;
			}
			else {
				temp=src[i];
				// usb_debug("%x\n",(temp | 0x0f));
				// usb_debug("convert error!\n");
				destLen = -1;
				break;
			}
        }
        else{
                // usb_debug("Destination string capicity is not enough!Transformation terminated!\n");
				destLen = -2;
                break;
        }
    }
    return destLen;
}
									
static int s_getFarsiCodeBuff(const u8 *inBuff, int inBuffLen, u16 *outPut, int outPutMaxLen)
{
	int i;

	if((inBuffLen/2) > outPutMaxLen)
	{
		return -1;
	}

	for(i = 0; i < (inBuffLen/2); i++)
	{
		outPut[i] = ((u16)inBuff[i*2]<<8) + inBuff[i*2+1];
	}
	outPut[inBuffLen/2] = 0x00;

	return 0;
}

int s_GetFarsiStringBuff(u16 *str_buff, int destBuffLen, const char * str)
{
	u8 strBuff[MAX_STRING_LENGTH] = {0};
	int ret,strBuffLen = 0;

	memset(str_buff, 0, destBuffLen);

	strBuffLen = s_Utf8ToUtf16(strBuff, str, sizeof(strBuff));
	if(strBuffLen < 0)
	{
		return -1;
	}

	ret = s_getFarsiCodeBuff(strBuff, strBuffLen, str_buff, destBuffLen);
	if(ret)
	{
		return -2;
	}

	return 0;
}

static void bitMapToLcdData(u16 *lcdData, Farsi_Dot_t *dot)
{
	int i,j,k,col;
	int fontLineBytes;
	
	fontLineBytes = (dot->width+7)/8;
	for( i = 0; i < dot->height*fontLineBytes*8; i++)
	{
		lcdData[i] = s_fontBackGronndColor;
	}

	for( i = 0; i < dot->height; i++)
	{
		col = 0;
		for(j=0;j<fontLineBytes;j++)
		{
			for(k = 0; k < 8; k++)
			{
				if(dot->bitmap[i*fontLineBytes + j] & (1<<k))
				{
					lcdData[col + i* dot->width] = s_fontColor;
				}
				col++;
				if(col == dot->width) break;
			}
		}
	}
}

int cLcdMapInit()
{
	int ret=0,i;

	s_lcdWidth = Ui_GetLcdWidth();
	s_lcdHeight = Ui_GetLcdHeight();

	/*malloc for lcd background map*/
	if(s_lcdBackGroundMap == NULL){
		s_lcdBackGroundMap = malloc(s_lcdWidth*s_lcdHeight*2);
		if(s_lcdBackGroundMap == NULL)
		{
			ret = -1;
			goto end;
		}
		
		for(i=0;i<(s_lcdWidth*s_lcdHeight);i++)
		{
			s_lcdBackGroundMap[i] = 0xffff;
		}
	}

	/*malloc for lcd map*/
	if(s_lcdMap == NULL){
		s_lcdMap = malloc(s_lcdWidth*s_lcdHeight*2);
		if(s_lcdMap == NULL)
		{
			ret = -2;
			goto end;
		}
		
		for(i=0;i<(s_lcdWidth*s_lcdHeight);i++)
		{
			s_lcdMap[i] = 0xffff;
		}
	}

end:
	if(ret!=0)
	{
		if(s_lcdBackGroundMap!=NULL)
		{
			free(s_lcdBackGroundMap);
		}
		if(s_lcdMap!=NULL)
		{
			free(s_lcdMap);
		}
	}

	if(s_fontSize==0)
	{
		LiteMGetFarsiDot(24, 24, 0, NULL, NULL);
		s_fontSize=24;
		s_fontBlod=0;
	}
	
	return ret;
}

int cLcdPrintf(int x,int y,UI_ALIGN_MODE align_mode,const char * fmt,...)
{
	va_list varg;
	Farsi_Dot_t farsiDot;
    int i,j,ret,count;
    char strBuff[MAX_STRING_LENGTH];
	u16 lpBuf[MAX_STRING_LENGTH/2] = {0};
	u16 drawData[STRING_LINE_HIGTH*STRING_LINE_WIDTH] = {0};
	u16 drawDataWithBackColor[STRING_LINE_HIGTH*STRING_LINE_WIDTH] = {0};
	int draw_x, draw_y;
	int farsiStrWidth, farsiStrHeight;

	if(s_lcdMap==NULL)
	{
		ret=cLcdMapInit();
		if(ret!=0){
			return ERROR_MEMORY;
		}
	}
	
	memset(strBuff, 0, sizeof(strBuff));  
    
    va_start( varg, fmt );                  
    ret = vsprintf(strBuff,  fmt,  varg); 
    va_end( varg );
	if(ret >= sizeof(strBuff)){
		strBuff[sizeof(strBuff)-1] = 0;
	}

	//DBG_STR("dataStr>>%s", strBuff);
	memset(farsiDot.bitmap, 0, sizeof(farsiDot.bitmap));

	ret = s_GetFarsiStringBuff(lpBuf, sizeof(lpBuf), strBuff);
	if(ret < 0)
	{
		//DBG_STR("s_GetFarsiStringBuff failed>>%d", ret);
		return ERROR_STRFORMAT;
	}

	ret = LiteMGetFarsiDot(0, 0, 0, lpBuf, &farsiDot);

	bitMapToLcdData(drawData, &farsiDot);
	farsiStrWidth = farsiDot.width;
	farsiStrHeight = farsiDot.height;
	if(farsiStrWidth>STRING_LINE_WIDTH){
		farsiStrWidth=STRING_LINE_WIDTH;
	}
	if(farsiStrHeight>STRING_LINE_HIGTH){
		farsiStrHeight=STRING_LINE_HIGTH;
	}
	/*set align*/
	draw_y = y;
	if(align_mode == UI_ALIGN_LEFT)
	{
		draw_x = 0 + s_fontRetract;
	}
	else if(align_mode == UI_ALIGN_CENTER)
	{
		draw_x = (Ui_GetLcdWidth() - farsiStrWidth)/2 + s_fontRetract;
	}
	else if(align_mode == UI_ALIGN_RIGHT)
	{
		draw_x = Ui_GetLcdWidth() - farsiStrWidth + s_fontRetract;
	}
	else
	{
		draw_x = x;
	}

	if(s_fontBackGroundEnable)
	{
		count = 0;
		for(i=0;i<farsiStrHeight;i++)
		{
			for(j=0;j<farsiStrWidth;j++)
			{
				if(((i+draw_y)*s_lcdWidth)+(j+draw_x)>=(s_lcdHeight*s_lcdWidth))
					break;

				s_lcdMap[((i+draw_y)*s_lcdWidth)+(j+draw_x)] = drawData[count++];
			}
		}
	}
	else
	{
		/*copy back color from lcdMap(if the font without back color)*/
		count = 0;
		for(i=draw_y; i<(draw_y+farsiStrHeight); i++)
		{
			for(j=draw_x; j<(draw_x+farsiStrWidth); j++)
			{
				drawDataWithBackColor[count++] = s_lcdMap[i*s_lcdWidth+j];
			}
		}
		/*fill font color*/
		for(i=0; i<(sizeof(drawData)/2); i++)
		{
			if(drawData[i] == s_fontColor)
			{
				drawDataWithBackColor[i] = s_fontColor;
			}
		}

		/*copy bmp to s_LcdMap*/
		count = 0;
		for(i=0;i<farsiStrHeight;i++)
		{
			for(j=0;j<farsiStrWidth;j++)
			{
				if(((i+draw_y)*s_lcdWidth)+(j+draw_x)>=(s_lcdHeight*s_lcdWidth))
					break;
					
				if(j+draw_x<s_lcdWidth)
					s_lcdMap[((i+draw_y)*s_lcdWidth)+(j+draw_x)] = drawDataWithBackColor[count];
				
				count++;
			}
		}
	}
	
	CLcdDrawArea(0, 0, s_lcdWidth, s_lcdHeight, s_lcdMap);

	return 0;
}

#define SETATTR_SUCCEED -5
int cFontAttrSet(FONT_ATTR_TYPE fontAttr, int value)
{
	int ret=0;
	u16 tempColor;

	if(fontAttr==FONT_ATTR_SIZE)
	{
		if(s_fontSize==value)
			return 0;

		ret=LiteMGetFarsiDot(value, value, s_fontBlod, NULL, NULL);
		if(ret==SETATTR_SUCCEED){
			s_fontSize = value;
		}
	}
	else if(fontAttr==FONT_ATTR_BLOD)
	{
		if(s_fontBlod==value)
			return 0;

		ret=LiteMGetFarsiDot(s_fontSize, s_fontSize, value, NULL, NULL);
		if(ret==SETATTR_SUCCEED){
			s_fontBlod = value;
		}
	}
	else if(fontAttr==FONT_ATTR_BACKGROUND)
	{
		s_fontBackGroundEnable=value;
	}
	else if(fontAttr==FONT_ATTR_BACKCOLOR)
	{
		s_fontBackGronndColor=(u16)value;
	}
	else if(fontAttr==FONT_ATTR_COLOR)
	{
		if(s_fontBackGronndColor == (u16)value)
		{
			if(s_fontBackGronndColor==0xffff){
				s_fontBackGronndColor--;
			}
			else s_fontBackGronndColor++;
		}
		s_fontColor=(u16)value;
	}
	else if(fontAttr==FONT_ATTR_RETRACT)
	{
		if(s_fontRetract<0||s_fontRetract>=320)
			return -1;

		s_fontRetract=value;
	}
	else if(fontAttr==FONT_COLOR_INVERSION)
	{
		tempColor = s_fontColor;
		s_fontColor = s_fontBackGronndColor;
		s_fontBackGronndColor = tempColor;
	}
	else
	{
		ret=-1;
	}

	return ret;
}

static int rgb888_to_rgb565(void * psrc, int w, int h, void * pdst)
{
    unsigned char * psrc_temp;
    unsigned short * pdst_temp;
    u32 i,j;

    if (!psrc || !pdst || w <= 0 || h <= 0) {
        DBG_STR("rgb888_to_rgb565 : parameter error\n");
        return -1;
    }
 
    psrc_temp = (unsigned char *)psrc;
    pdst_temp = (unsigned short *)pdst;
    for (i=0; i<h; i++) {
        for (j=0; j<w; j++) {
            //888 r|g|b -> 565 b|g|r
            *pdst_temp =(((psrc_temp[0] >> 3) & 0x1F) << 11)//r
						|(((psrc_temp[1] >> 2) & 0x3F) << 5)//g
						|(((psrc_temp[2] >> 3) & 0x1F) << 0);//b 

            psrc_temp += 3;
            pdst_temp ++;
        }
       // psrc_temp +=(3*16);
    }
 
    return 0;
}

int cLcddrawPicture(int x,int y,char *filename)
{
	spng_ctx *ctx;
    int ret = 0;
	int fid;
	long fsize;
	char *pngData = NULL;
    int png_ctx;
    int image_width;
    int image_height;
	size_t size;
    u8 *out = NULL;
	u8 *rgb565Buff = NULL;
	int i,j,count;

	if(s_lcdMap==NULL)
	{
		ret=cLcdMapInit();
		if(ret!=0){
			return ERROR_MEMORY;
		}
	}

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
	// DBG_STR("file size:%d", fsize);
	pngData = malloc(fsize);
	if(pngData == NULL)
	{
		ret = ERROR_MEMORY;
		goto end;
	}
	ret = read(fid, pngData, fsize);
	if(ret == -1)
	{
		ret = ERROR_FILE;
		goto end;
	}

	/*decode png data*/
    png_ctx = png_decode_init(pngData, fsize);
    if (png_ctx == 0) {
        ret = ERROR_MEMORY;
		goto end;
    }
    ret = png_decode_size(png_ctx, &size, &image_width, &image_height);
    if (ret < 0) {
		ret = ERROR_PNGFORMAT;
        goto end;
    }
    out = (u8 *)malloc(size);
    if (out == NULL) {
		ret = ERROR_MEMORY;
        goto end;
    }
    ret = png_decode(png_ctx, out, size);
    if (ret < 0) {
		ret = ERROR_PNGFORMAT;
        goto end;
    }
	if(pngData != NULL)
	{
		free(pngData);
		pngData = NULL;
	}

	rgb565Buff = (u8*)malloc(size/3*2);
	if(rgb565Buff == NULL) {
		ret = ERROR_MEMORY;
        goto end;
    }

	ret = rgb888_to_rgb565(out, image_width, image_height, rgb565Buff);
	if(ret)
	{
		ret = ERROR_PNGFORMAT;
		goto end;
	}
	if(out != NULL)
	{
		free(out);
		out = NULL;
	}

	//use bit map to display png
	count = 0;
	for(i=0;i<image_height;i++)
	{
		for(j=0;j<image_width;j++)
		{
			if(((i+y)*s_lcdWidth)+(j+x)>=(s_lcdHeight*s_lcdWidth))
				break;

			if(j+x<s_lcdWidth)
				s_lcdMap[((i+y)*s_lcdWidth)+(j+x)] = ((u16*)rgb565Buff)[count];

			count++;
		}
	}
	CLcdDrawArea(0, 0, s_lcdWidth, s_lcdHeight, s_lcdMap);
    //CLcdDrawArea(x, y, image_width, image_height, (u16*)out);

end:
	if(fid > 0){
		close(fid);
	}
	
	if(png_ctx!=0){
		png_decoder_uninit(png_ctx);
	}

    if (out != NULL) {
        free(out);
    }

	if(rgb565Buff != NULL) {
		free(rgb565Buff);
    }

	if(pngData!=NULL){
		free(pngData);
	}
	
	return ret;
}

int cLcdSetAttr(UI_AREA_ST area,UI_ATTR_TYPE type, int value)
{
	int i,j,line,ret;

	s_lcdWidth = Ui_GetLcdWidth();
	s_lcdHeight = Ui_GetLcdHeight();

	if(s_lcdBackGroundMap == NULL)
	{
		ret=cLcdMapInit();
		if(ret!=0){
			return ERROR_MEMORY;
		}
	}

	if(((area.width+area.x)>s_lcdWidth)||((area.height+area.y)>s_lcdHeight))
	{
		return ERROR_PARAM;
	}

	if(type == UI_ATTR_BCOLOR){
		for(i=area.y; i<(area.height+area.y); i++)
		{
			for(j=area.x; j<(area.width+area.x); j++)
			{
				s_lcdBackGroundMap[(i*DISPLAY_WIDTH)+j] = (u16)value;
			}
		}
	}
	else if(type == UI_ATTR_FCOLOR)
	{

	}

	//CLcdDrawArea(0,0,s_lcdWidth,s_lcdHeight,s_lcdBackGroundMap);

	return 0;
}

/*clear lcd use background color*/
void cLcdClear()
{
	int i;

	if(s_lcdBackGroundMap == NULL)
	{
		Ui_Clear();
	}
	else
	{
		s_lcdWidth = Ui_GetLcdWidth();
		s_lcdHeight = Ui_GetLcdHeight();
		CLcdDrawArea(0,0,s_lcdWidth,s_lcdHeight,s_lcdBackGroundMap);//use s_lcdBackGroundMap to clear

		for(i=0;i<(s_lcdWidth*s_lcdHeight);i++)
		{
			s_lcdMap[i] = s_lcdBackGroundMap[i];
		}
	}
}

#define BITS2BYTES(n)		(( n + 7) >> 3)
#define BYTES2BITS(n)		( (n) << 3)
u8 BitRotate(u8 data)  // bit0>bit7,bi1->bit6.......
{
   	u8 ch,n,m;   
	
	n=0;
	ch = data;	
	for(m=0;m<8;m++)
	{
		n = n<<1;
		if(ch&0x01)
		{
			n=n|0x01;
		}
		ch = ch>>1;	
	}

	return n;
}


/**************************************
 * this function use to set multiline string for print
 * if setting multiline line,the printer will start print 
 * when s_printBuffLineCount equal to s_printBuffLine(the setting line)
 **************************************/
static int s_Print_StringMultilineLineSet(u32 line)
{
	int printBuffSize;
	if (line > PRINTER_STRING_MULT_LINE) return -1;

	if(s_printMultiLineBuff != NULL){
		free(s_printMultiLineBuff);
		s_printMultiLineBuff = NULL;
	}

	if(line > 1){
		printBuffSize = PRINTER_LINE_MAX_WIDTH * (PRINTER_LINE_MAX_HIGTH+1) * line;
		s_printMultiLineBuff = (u8*)malloc(printBuffSize);
		if(s_printMultiLineBuff == NULL){
			s_printBuffLine = 0;
			return -2;
		}

		memset(s_printMultiLineBuff, 0, printBuffSize);
	}

	s_printBuffLine = line;

	return 0;
}

static int s_Printet_isReverseColor=0;
static int s_Print_StringSetReverse(int isReverse)
{
	s_Printet_isReverseColor = isReverse;

	return 0;
}

static int s_printerComplexLine=0;
static int s_PrintSetComplexLine(int copmlexLine)
{
	s_printerComplexLine = copmlexLine;

	return 0;
}

static int s_ifStringDirectionReverse=0;
static int s_PrintSetStringDirection(int isReverse)
{
	s_ifStringDirectionReverse = isReverse;

	return 0;
}

static int s_printCharInterval=0;
static int s_PrintSetCharInterval(int interval)
{
	s_printCharInterval = interval;

	return 0;
}

int Print_SetAttr(PRINTER_ATTR_TYPE printerAttr, int value)
{
	int ret;

	switch (printerAttr)
	{
	case PRINTER_ATTR_MULTILINE:
		ret = s_Print_StringMultilineLineSet(value);
		break;
	case PRINTER_ATTR_REVERSECOLOR:
		ret = s_Print_StringSetReverse(value);
		break;
	case PRINTER_ATTR_COMPLEXLINE:
		ret = s_PrintSetComplexLine(value);
		break;
	case PRINTER_ATTR_STRING_DIRECTIONREVERSE:
		ret = s_PrintSetStringDirection(value);
		break;
	case PRINTER_ATTR_CHARINTERVAL:
		ret = s_PrintSetCharInterval(value);
		break;
	default:
		ret = ERROR_PARAM;
		break;
	}

	return ret;
}

//can add to function printerTest to set row height
void addRowHeight(int rowHeight)
{
    int i;
    u8 empyRowBuf[48*48]={0};
    int fullLoop, marginRow;

    fullLoop = rowHeight/48;
    marginRow = rowHeight%48;

	if(s_printBuffLine>1){
		memcpy(&s_printMultiLineBuff[s_printBuffOffSet], empyRowBuf, sizeof(empyRowBuf));
		s_printBuffLineCount++;
		s_printBuffOffSet += sizeof(empyRowBuf);

		if(s_printBuffLineCount == s_printBuffLine){
			PrnInit();
			PrnStart(s_printMultiLineBuff, (s_printBuffLineCount*PRINTER_LINE_MAX_HIGTH));
			s_printBuffLineCount = 0;
			s_printBuffOffSet = 0;

			if(s_printMultiLineBuff != NULL){
				free(s_printMultiLineBuff);
				s_printMultiLineBuff = NULL;
			}
			s_printBuffLine = 0;

			return ;
		}
		else{
			return;
		}
	}
    PrnInit();
    for(i=0; i<fullLoop; i++){
        PrnStart(empyRowBuf, 48);
    }
    if(marginRow){
        PrnStart(empyRowBuf, marginRow);
    }
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

static void s_getCharIntervalBitMap(u16* farsiStr, u8* outData)
{
	int i,j,k;
	int charWidth;
	u16 tempCharBuff[2]={0};
	Farsi_Dot_t dot;
	int offset = 0;
	u8 tempBitMap[PRINTER_LINE_MAX_HIGTH*PRINTER_LINE_MAX_WIDTH];

	memset(tempBitMap,0,sizeof(tempBitMap));

	for(i=0;i<s_strlen_utf16(farsiStr);i++){
		tempCharBuff[0] = farsiStr[i];
		memset(dot.bitmap, 0, sizeof(dot.bitmap));
		LiteMGetFarsiDot(0, 0, 0, tempCharBuff, &dot);

		charWidth = (dot.width+7)/8;
		for(j=0;j<dot.height;j++){
			for(k=0;k<charWidth;k++){
				tempBitMap[j*PRINTER_LINE_MAX_WIDTH+k+offset+(s_printCharInterval/8)] = BitRotate(dot.bitmap[j*charWidth+k]);
			}
		}

		offset+=charWidth+(s_printCharInterval/8); //if want more precision,set bit instead of byte
	}

	memcpy(outData, tempBitMap, sizeof(tempBitMap));
}

int Print_Printf(int x, UI_ALIGN_MODE align_mode, const char * fmt,...)
{
	Farsi_Dot_t dot;
	va_list varg;
	int i,j,ret;
	u8 prnwith = 0;
	char strBuff[MAX_STRING_LENGTH];
	u16 lpBuf[MAX_STRING_LENGTH/2] = {0};
	u16 tempLpBuf[MAX_STRING_LENGTH/2] = {0};
	u8 Printbuf[(PRINTER_LINE_MAX_HIGTH+1)*PRINTER_LINE_MAX_WIDTH];
	int PrintfBufDataSize = PRINTER_LINE_MAX_HIGTH*PRINTER_LINE_MAX_WIDTH;
	int offset=0;
	int printHigth;

	// use to complex string
	static u8 complexPrintBuf[(PRINTER_LINE_MAX_HIGTH+1)*PRINTER_LINE_MAX_WIDTH];
	static u8 complexCount = 0;

	//init printer buff
	memset(strBuff, 0, sizeof(strBuff));  
	if(complexCount == 0)
	{
		memset(complexPrintBuf, 0, sizeof(complexPrintBuf));
	}
	memset(Printbuf,0x00,sizeof(Printbuf));
	memset(dot.bitmap, 0, sizeof(dot.bitmap));

	//format string info
    va_start( varg, fmt );                  
    ret = vsprintf(strBuff,  fmt,  varg); 
    va_end( varg );
	if(ret >= sizeof(strBuff)){
	strBuff[sizeof(strBuff)-1] = 0;
	}
	ret = s_GetFarsiStringBuff(lpBuf, sizeof(lpBuf), strBuff);
	if(ret < 0)
	{
		return ERROR_STRFORMAT;
	}

	//handle lpBuf if set string direction reverse
	j=0;
    if(s_ifStringDirectionReverse){
        for(i=s_strlen_utf16(lpBuf)-1; i>=0; i--){
            tempLpBuf[j++] = lpBuf[i];
        }
        for(i=0;i<s_strlen_utf16(lpBuf);i++){
            lpBuf[i] = tempLpBuf[i];
        }
    }

	//get farsi string bmp info
	LiteMGetFarsiDot(0, 0, 0, lpBuf, &dot);
	prnwith = (dot.width+7)/8;

	//get align mode and set print offset
	if(align_mode == UI_ALIGN_CENTER){
		offset = ((PRINTER_LINE_MAX_WIDTH-prnwith)/2);
	}
	else if(align_mode == UI_ALIGN_RIGHT){
		offset = PRINTER_LINE_MAX_WIDTH-prnwith;
	}
	else if(align_mode == UI_ALIGN_LEFT){
		offset = 0;
	}
	
	for(i=0;i<dot.height;i++)
	{	
		for(j=0;j<prnwith;j++)
		{
			Printbuf[i*PRINTER_LINE_MAX_WIDTH+j+offset+(x/8)] =BitRotate(dot.bitmap[i*prnwith+j]);
		}
	}

	//if set char interval, reformat printf buf
	if(s_printCharInterval){
		memset(Printbuf,0x00,sizeof(Printbuf));
		s_getCharIntervalBitMap(lpBuf, Printbuf);
	}

	//format data to complex line
	complexCount++;
	for(i=0;i<PrintfBufDataSize;i++)
	{	
		complexPrintBuf[i] |= Printbuf[i];
	}
	if((complexCount != s_printerComplexLine) && s_printerComplexLine){
		return PRINTER_COMPLEX_NOENOUGH;
	}
	complexCount = 0;

	//if set reverse, reverse this line data 
	if(s_Printet_isReverseColor){
		for(i=0;i<PrintfBufDataSize;i++)
		{
			complexPrintBuf[i] = ~complexPrintBuf[i];
		}
	}

	//if set multi line printf, add line data to multi printf data.
	if(s_printBuffLine>1){
		memcpy(&s_printMultiLineBuff[s_printBuffOffSet], complexPrintBuf, PrintfBufDataSize);
		s_printBuffLineCount++;
		s_printBuffOffSet += PrintfBufDataSize;

		//printf if data is enough
		if(s_printBuffLineCount == s_printBuffLine){
			printHigth = s_printBuffLineCount*(PRINTER_LINE_MAX_HIGTH+1);
			PrnInit();
			PrnStart(s_printMultiLineBuff, printHigth);
			s_printBuffLineCount = 0;
			s_printBuffOffSet = 0;

			if(s_printMultiLineBuff != NULL){
				free(s_printMultiLineBuff);
				s_printMultiLineBuff = NULL;
			}
			s_printBuffLine = 0;

			return PRINTER_PRINTF_SUCCEED;
		}
		else{
			return PRINTER_MULTILINE_NOENOUGH;
		}
	}

	//print one line data
	memset(&complexPrintBuf[dot.height*PRINTER_LINE_MAX_WIDTH], 0, PRINTER_LINE_MAX_WIDTH);
	printHigth = dot.height + 1;
	PrnInit();
	PrnStart(complexPrintBuf, printHigth);
	//PrnStart(complexPrintBuf, printHigth);

	return PRINTER_PRINTF_SUCCEED;
}

int Print_drawPicture(int x, char *filename)
{
	spng_ctx *ctx;
    int ret = 0;
	int fid,fix;
	long fsize;
	char *pngData = NULL;
    int png_ctx;
    int image_width;
    int image_height;
	size_t size;
    u8 *out = NULL;
	u8 *printBuf = NULL;
	int i,j;
	u16 r,g,b,gray;
	u8 *bitMap = NULL;
	u32 printBufHeight = NULL;
	int writePixCount=0;
	u8 *rgb565Buff = NULL;

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
	pngData = malloc(fsize);
	if(pngData == NULL)
	{
		ret = ERROR_MEMORY;
		goto end;
	}
	ret = read(fid, pngData, fsize);
	if(ret == -1)
	{
		ret = ERROR_FILE;
		goto end;
	}

	/*decode png data*/
    png_ctx = png_decode_init(pngData, fsize);
    if (png_ctx == 0) {
        ret = ERROR_MEMORY;
		goto end;
    }
    ret = png_decode_size(png_ctx, &size, &image_width, &image_height);
    if (ret < 0) {
		ret = ERROR_PNGFORMAT;
        goto end;
    }
    out = (u8 *)malloc(size);
    if (out == NULL) {
		ret = ERROR_MEMORY;
        goto end;
    }
    ret = png_decode(png_ctx, out, size);
    if (ret < 0) {
		ret = ERROR_PNGFORMAT;
        goto end;
    }

	rgb565Buff = (u8*)malloc(size/3*2);
	if(rgb565Buff == NULL) {
		ret = ERROR_MEMORY;
        goto end;
    }

	ret = rgb888_to_rgb565(out, image_width, image_height, rgb565Buff);
	if(ret)
	{
		ret = ERROR_PNGFORMAT;
		goto end;
	}
	if(out != NULL)
	{
		free(out);
		out = NULL;
	}

	if(image_width%8){
		fix = 1;
	}else{
		fix = 0;
	}

	bitMap = (u8*)malloc(size/2+(fix*image_height));
	if(bitMap==NULL){
		ret = ERROR_MEMORY;
        goto end;
	}
	memset(bitMap, 0, (size/2+(fix*image_height)));

	j = 0;
	writePixCount = 1;
	for(i=0;i<size/2;i++){
		r = (((u16*)rgb565Buff)[i]&0xf800)>>8;
		g = (((u16*)rgb565Buff)[i]&0x07e0)>>3;
		b = (((u16*)rgb565Buff)[i]&0x001f)<<3;
		gray = (r*30 + g*59 + b*11)/100;
		if(gray<128){
			bitMap[j/8] |= (0x80>>(j%8));
		}
		j++;
		writePixCount++;
		// if((j%image_width)==0&&fix==1){
		// 	j++;
		// }

		// j++;
		if(fix==1 && !(writePixCount%(image_width+1))){
			j += (8-(image_width%8));
			writePixCount=1;
		}
		//usb_debug("%d\r\n", gray);
	}

	printBufHeight = image_height+1;
	printBuf = (u8*)malloc(PRINTER_LINE_MAX_WIDTH * printBufHeight);
	if(printBuf == NULL) {
		ret = ERROR_MEMORY;
        goto end;
    }
	memset(printBuf, 0, PRINTER_LINE_MAX_WIDTH * printBufHeight);

	for(i = 0;i < image_height; i++){
		for(j = 0; j < (image_width/8+fix); j++){
			printBuf[i*PRINTER_LINE_MAX_WIDTH+j+(x/8)] =bitMap[i*(image_width/8+fix)+j];
		}
	}

	PrnInit();
	PrnStart(printBuf, printBufHeight);
end:
	if(fid > 0){
		close(fid);
	}

	if(png_ctx!=0){
		png_decoder_uninit(png_ctx);
	}
	
    if (out != NULL) {
        free(out);
    }

	if(rgb565Buff != NULL) {
		free(rgb565Buff);
    }

	if(pngData!=NULL){
		free(pngData);
	}

	if(printBuf!=NULL){
		free(printBuf);
	}

	if(bitMap!=NULL){
		free(bitMap);
	}
	
	return ret;
}


int Print_Printf_ComplexString(int complexLine, UI_ALIGN_MODE align_mode,const char * fmt,...)
{
	Farsi_Dot_t dot;
	va_list varg;
	u32 i,j,ret;
	u8 prnwith = 0,ch;
	char strBuff[MAX_STRING_LENGTH];
	u16 lpBuf[MAX_STRING_LENGTH/2] = {0};
	u8 Printbuf[PRINTER_LINE_MAX_HIGTH*PRINTER_LINE_MAX_WIDTH];
	int PrintfBufDataSize = PRINTER_LINE_MAX_HIGTH*PRINTER_LINE_MAX_WIDTH;
	int offset=0;

	//use to compound string
	static u8 complexPrintBuf[PRINTER_LINE_MAX_HIGTH*PRINTER_LINE_MAX_WIDTH];
	static u8 complexCount = 0;
	if(complexCount == 0)
	{
		memset(complexPrintBuf, 0, sizeof(complexPrintBuf));
	}

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
		return ERROR_STRFORMAT;
	}

	memset(Printbuf,0x00,sizeof(Printbuf));
	memset(dot.bitmap, 0, sizeof(dot.bitmap));

	LiteMGetFarsiDot(0, 0, 0, lpBuf, &dot);
	
	prnwith = (dot.width+7)/8;

	if(align_mode == UI_ALIGN_CENTER){
		offset = ((PRINTER_LINE_MAX_WIDTH-prnwith)/2);
	}
	else if(align_mode == UI_ALIGN_RIGHT){
		offset = PRINTER_LINE_MAX_WIDTH-prnwith;
	}
	else if(align_mode == UI_ALIGN_LEFT){
		offset = 0;
	}
	
	for(i=0;i<dot.height;i++)
	{	
		for(j=0;j<prnwith;j++)
		{
			Printbuf[i*PRINTER_LINE_MAX_WIDTH+j+offset] =BitRotate(dot.bitmap[i*prnwith+j]);
		}
	}

	complexCount++;
	for(i=0;i<(PRINTER_LINE_MAX_WIDTH*PRINTER_LINE_MAX_HIGTH);i++)
	{	
		if(Printbuf[i] != 0)//if have data copy to complexPrintBuf, this is byte manipulation, for more precise, can use bit manipulation
		{
			complexPrintBuf[i] = Printbuf[i];
		}
	}
	if(complexCount != complexLine)//need complex "complexLine" string to print
	{
		return complexCount;
	}

	if(s_Printet_isReverseColor){
		for(i=0;i<PrintfBufDataSize;i++)
		{
			complexPrintBuf[i] = ~complexPrintBuf[i];
		}
	}


	complexCount = 0;

	if(s_printBuffLine>1){
		memcpy(&s_printMultiLineBuff[s_printBuffOffSet], complexPrintBuf, sizeof(complexPrintBuf));
		s_printBuffLineCount++;
		s_printBuffOffSet += sizeof(complexPrintBuf);

		if(s_printBuffLineCount == s_printBuffLine){
			PrnInit();
			PrnStart(s_printMultiLineBuff, (s_printBuffLineCount*PRINTER_LINE_MAX_HIGTH));
			s_printBuffLineCount = 0;
			s_printBuffOffSet = 0;

			if(s_printMultiLineBuff != NULL){
				free(s_printMultiLineBuff);
				s_printMultiLineBuff = NULL;
			}
			s_printBuffLine = 0;

			return 0;
		}
		else{
			return s_printBuffLineCount;
		}
	}

	PrnInit();
	PrnStart(complexPrintBuf, PRINTER_LINE_MAX_HIGTH);
	
	//finish print, reset count
	return 0;
}