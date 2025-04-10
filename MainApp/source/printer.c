#include "app.h"
#include "ui.h"
#include "QrCode.h"
#include "trans.h"
#include "printer.h"

#ifdef PRT_DEBUG
#define PRT_STRLOG DBG_STR
#define PRT_HEXLOG DBG_HEX
#else
#define PRT_STRLOG  
#define PRT_HEXLOG 
#endif


extern int Ui_PrintQRCode(E_DISPALIGN align,const char *text,u8 * buf);


int ticketPrint();
int Prn_Data();


#define PRT_LINE_DOT     	(384)
#define PRT_LINE_BYTE	   	(PRT_LINE_DOT/8)
#define PRT_FONT_BITMAP_W  	(PRT_LINE_DOT)
#define PRT_FONT_BITMAP_H  	(32)
#define MAX_PRINTER_BUFFER   60*1024

static unsigned char *sg_printBuf=NULL;
static unsigned int sg_printerOff = 0;

extern bool gutArabicSrcStrCovPrintStr(u32* u16Src,u8 u8StrLen);
extern int utf8_to_unicode(const char* utf8_str, u32* unicode_str, size_t unicode_len);
extern int getBmpFromunicode(u32 *unicode_char, u32 unicode_char_len, u8 *bitmap, u32 bitmapW, u32 bitmapH, float font_pixels, u32 *disp_w, u32 *disp_y);



int inPrinter_string(u8 *txt,u8 * outbuf,int align){
	int offset=0;
	int i,j;
	u8 txtbuf[32][48];
	u8 tmp[25];
	int len=strlen(txt);
	u8 *txt1=NULL;

	memset(tmp,0,sizeof(tmp));
	if(align == DISPLAY_CENTER){
		if(len > 24){
			txt[24]=0;
			len=24;
			strcpy(tmp,txt);
		}else{
			memset(tmp,0x20,(24-len)/2);
			strcat(tmp,txt);
		}	
	}else if(align == DISPLAY_RIGHT){
		if(len >= 24){
			txt[24]=0;
			len=24;
			strcpy(tmp,txt);
		}else{
			memset(tmp,0x20,24-len);
			strcat(tmp,txt);
		}
	}else{
		if(len >= 24){
			txt[24]=0;
			len=24;
		}
		strcpy(tmp,txt);
	}

	txt1 = tmp;
	//DBG_STR("txt=%s\n",txt1);
	memset(txtbuf,0,sizeof(txtbuf));
	while (*txt1)
	{
		for(i=0;i<32;i++){
			memcpy(&txtbuf[i][offset],DefASCFont16X32 + (*txt1 -0x20)*64 + i*2,2);
		}
		offset+=2;
		txt1 += 1;
	}	
	for(i=0;i<32;i++)
	{
		memcpy(outbuf+i*48,txtbuf[i],48);
	}
	return sizeof(txtbuf);
}

//feeding paper
int inPrinter_line(int height,u8 * outbuf){
	memset(outbuf,0x00,48*height);
	return 48*height;
}

int printer_sample(){
	
}

int printer_black(void * pxPARAM)
{
	int ret;
	u8* buf;
	int off=0;
	u8 str[100] = {0};

	PrnInit();
	ret = PrnStatus();
	PRT_STRLOG("inPrinter_getStatus=%d",ret);
	if(ret != PRN_OK){
		PRT_STRLOG("printer err status = %d", ret);
		if(ret == PRN_PAPEROUT){
			displayWarning("printer test","no paper");
		}else if(ret == PRN_TOOHEAT){
			displayWarning("printer test","over heat");
		}else{
			displayWarning("printer test","printer err");
		}
		return -1;
	}
	PrnSetGray(50);//50-500

    buf = malloc(48*1024);
    if(buf == NULL) {
        return -1;
    }

	memset(buf,0,48*1024);
	//black block
	memset(buf+off,0xFF,48*1000);
	off += (48*1000);

	while(1){
		//start
		ret = PrnStart(buf,off/(384/8));
		PRT_STRLOG("PrnStart [%d] len[%d]", ret,off);
		if(ret != 0){
			free(buf);
			return -1;
		}
		
		if(Ui_WaitKey(100)==KB_CANCEL){
			break;
		}
	}
	free(buf);
    return 0;
}

int printer_test(void * pxPARAM)
{
	int ret;
	u8* buf;
	int off=0;
	u8 str[100] = {0};
	PrnInit();
	ret = PrnStatus();
	PRT_STRLOG("inPrinter_getStatus=%d",ret);
	if(ret != PRN_OK){
		PRT_STRLOG("printer err status = %d", ret);
		if(ret == PRN_PAPEROUT){
			displayWarning("printer test","no paper");
		}else if(ret == PRN_TOOHEAT){
			displayWarning("printer test","over heat");
		}else{
			displayWarning("printer test","printer err");
		}
		return -1;
	}
	
	PrnSetGray(500);//50-500

    buf = malloc(48*1024);
    if(buf == NULL) {
        return -1;
    }

	memset(buf,0,48*1024);
	//print title
	memset(str,0x00,sizeof(str));
	sprintf(str,"A50 PRINT TEST");
	ret = inPrinter_string(str,buf+off,DISPLAY_CENTER);
	off += ret;
	//print string
	memset(str,0x00,sizeof(str));
	sprintf(str,"left test");
	ret = inPrinter_string(str,buf+off,DISPLAY_LEFT);
	off += ret;
	memset(str,0x00,sizeof(str));
	sprintf(str,"center test");
	ret = inPrinter_string(str,buf+off,DISPLAY_CENTER);
	off += ret;
	memset(str,0x00,sizeof(str));
	sprintf(str,"right test");
	ret = inPrinter_string(str,buf+off,DISPLAY_RIGHT);
	off += ret;
	
	ret = inPrinter_line(2,buf+off);
	off +=ret;

	//black block
	memset(buf+off,0xFF,48*40);
	off += (48*40);
	
	ret = inPrinter_line(2,buf+off);
	off +=ret;

	//print qrcode
	ret  = PrintQrcode("abcdefghijklmn",8,ALIGN_CENTER,0,buf+off);
	if(ret > 0)
		off += ret;
    PRT_STRLOG("Ui_PrintQRCode [%d]", ret);

	ret = inPrinter_line(100,buf+off);
	off +=ret;

	//start
	ret = PrnStart(buf,off/(384/8));
    PRT_STRLOG("PrnStart [%d] len[%d]", ret,off);
	if(ret != 0){
		free(buf);
		return -1;
	}
	
	free(buf);
    return 0;
}

int test_printer_sample_receipt(void * pxPARAM)
{
    int ret;
    PrnInit();	
	ret = PrnStatus();
	PRT_STRLOG("inPrinter_getStatus=%d",ret);
	if(ret != PRN_OK){
		PRT_STRLOG("printer err status = %d", ret);
		if(ret == PRN_PAPEROUT){
			displayWarning("printer test","no paper");
		}else if(ret == PRN_TOOHEAT){
			displayWarning("printer test","over heat");
		}else{
			displayWarning("printer test","printer err");
		}
		return -1;
	}
    ret = PrnStart((u8*)sample_receipt, sizeof(sample_receipt)/(384/8) );

    PRT_STRLOG("%d, sample receipt [%08x] len[%d]", ret,  sample_receipt, sizeof(sample_receipt));
    return 0;
}


int test_printer(void * pxPARAM)
{	
    int ret;
    const ST_MENUITEM menu[] = {
        {KB1, "1.SAMPLE RECEIPT",      test_printer_sample_receipt, (void *)NULL},
        {KB2, "2.FACTORY TEST",        printer_test, (void *)NULL},
		{KB3, "3.BLACK PRT", 	       printer_black, (void *)NULL},

    };
    return Ui_Menu("PRINTER TEST", menu, sizeof(menu)/sizeof(ST_MENUITEM), APP_UI_TIMEOUT);
}