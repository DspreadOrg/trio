#include "Farsi_api.h"

#define ERROR_MEMORY 	-1001
#define ERROR_FILE 		-1002
#define ERROR_PNGFORMAT	-1003
#define ERROR_STRFORMAT	-1004
#define ERROR_PARAM		-1005

#define PRINTER_PRINTF_SUCCEED		0
#define PRINTER_MULTILINE_NOENOUGH	1001
#define PRINTER_COMPLEX_NOENOUGH	1002

typedef struct {     
	int x;
	int y;
	int width; 
	int height;
} UI_AREA_ST;

typedef enum {UI_ALIGN_LEFT=0,UI_ALIGN_CENTER,UI_ALIGN_RIGHT,UI_ALIGN_NO}UI_ALIGN_MODE;
typedef enum {UI_ATTR_FCOLOR=0,UI_ATTR_BCOLOR}UI_ATTR_TYPE;

typedef enum {FONT_SIZE_MAX=48,FONT_SIZE_MEDIUM=32,FONT_SIZE_MIN=24}FONT_SIZE;
typedef enum {FONT_ATTR_SIZE=0,FONT_ATTR_BLOD,FONT_ATTR_BACKGROUND,
			FONT_ATTR_BACKCOLOR,FONT_ATTR_COLOR,FONT_COLOR_INVERSION,
			FONT_ATTR_RETRACT}FONT_ATTR_TYPE;

typedef enum {PRINTER_ATTR_MULTILINE, PRINTER_ATTR_REVERSECOLOR, PRINTER_ATTR_COMPLEXLINE, 
			PRINTER_ATTR_STRING_DIRECTIONREVERSE, PRINTER_ATTR_CHARINTERVAL}PRINTER_ATTR_TYPE;

/*************************************
FONT_ATTR_SIZE:	use to set font size,value is enum FONT_SIZE
FONT_ATTR_BLOD:	use to set blod, 1 is blod, 0 is no blod
FONT_ATTR_BACKGROUND:	use to set if font background visable,1 is visable,0 is invisable
FONT_ATTR_BACKCOLOR:	use to set font background color, value is RGB565
FONT_ATTR_COLOR:		use to set font color, value is RGB565
FONT_COLOR_INVERSION:	use to inversion the font color and background color,value is nonuse
FONT_ATTR_RETRACT:		use to set string retract, value is 0~320
*************************************/
int cFontAttrSet(FONT_ATTR_TYPE fontAttr, int value);

void cLcdClear();
int cLcdPrintf(int x,int y,UI_ALIGN_MODE align_mode,const char * fmt,...);
int cLcddrawPicture(int x,int y,char *filename);
int cLcdSetAttr(UI_AREA_ST area,UI_ATTR_TYPE type, int value);

int Print_SetAttr(PRINTER_ATTR_TYPE printerAttr, int value);
int Print_Printf(int x, UI_ALIGN_MODE align_mode,const char * fmt,...);
int Print_drawPicture(int x, char *filename);
