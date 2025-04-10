#ifndef __UI_H__
#define __UI_H__

#define UI_FONT_SMALL         0
#define UI_FONT_NORMAL        1
#define UI_FONT_BIG           2


#ifndef RGB // R5|G6|B5 16 bit color
#define RGB(r, g, b) ((uint)(((byte)(r)) >> 3 << 11) | (uint)(((byte)(g)) >> 2 << 5) | (uint)(((byte)(b)) >> 3) )
#endif

#define  APP_BASE_ADDR 		   0x14000000
#define  ARAB_FONT_DATA_ADDR   0xFE000
#define  FONT_BITMAP_W         (320-Ui_GetLinePadding()-2)
#define  FONT_PIX_48           (46.0)
#define  FONT_BITMAP_H         ((int)FONT_PIX_48)


#define  FONT_RBG_COLOR_RED    0xF800
#define  FONT_RBG_COLOR_GREEN  0x07E0
#define  FONT_RBG_COLOR_BLUE   0x001F
#define  FONT_RBG_COLOR_BLUE   0x001F
#define  FONT_RBG_COLOR_BLACK  0x0000

//#define  FONT_BITMAP_H         (48)



#define UI_RET_BASE          -1000
#define UI_RET_SUCCESS       0
#define UI_RET_FAIL          (UI_RET_BASE)
#define UI_RET_TIMEOUT       (UI_RET_BASE - 1)
#define UI_RET_ABORT         (UI_RET_BASE - 2)
#define UI_RET_PARAM         (UI_RET_BASE - 3)


#define DISPLAY_WIDTH       320
#define DISPLAY_HEIGHT      240
#define DISPLAY_LINE_SIZE   30

#define DISPLAY_CHAR_WIDTH   12  // px
#define DISPLAY_CHAR_HEIGHT  24  // px
#define DISPLAY_LINE_PADDING 4   // 320 
//20(16x),26(12x),40(8x)
#define DISPLAY_LINE_NUM    10  // 240/24 = 10
#define DISPLAY_LINE_HZ_NUM 13  // 320/24 = 13
#define DISPLAY_LINE_EN_NUM 26  // 320/12 = 26
#define DISPLAY_LINE_BIG_NUM 17  // 320/18 = 17
#if 0
#define DISPLAY_COLOR_BLACK  RGB(0, 0, 0)        // #000000
#define DISPLAY_COLOR_WHITE  RGB(255, 255, 255)  // #FFFFFF
#define DISPLAY_COLOR_GRAY   RGB(221, 221, 221)  // #DDDDDD
#define DISPLAY_COLOR_RED    RGB(255, 0, 0)    // #FF0000
#define DISPALY_COLOR_GREEN  RGB(0, 255, 0)    // #00FF00
#else
#define DISPLAY_COLOR_BLACK  0x00000000
#define DISPLAY_COLOR_WHITE  0x00ffffff
#define DISPLAY_COLOR_GRAY   0x00DDDDDD
#define DISPLAY_COLOR_RED    0x00ff0000
#define DISPALY_COLOR_GREEN  0x0000ff00
#endif
#define DISPLAY_FONT_COLOR   DISPLAY_COLOR_BLACK
#define DISPLAY_BG_COLOR     DISPLAY_COLOR_WHITE


#define INPUT_MAX_SIZE       100

#define KB0	        '0'
#define KB1	        '1'
#define KB2	        '2'
#define KB3	        '3'
#define KB4	        '4'
#define KB5	        '5'
#define KB6	        '6'
#define KB7	        '7'
#define KB8	        '8'
#define KB9	        '9'

#define KB_DOT      '.'
#define KB_FN       0x01
#define KB_ALPHA    0x02
#define KB_CLEAR    0x03
#define KB_UP       0x05
#define KB_DOWN     0x06
#define KB_ENTER    0x0d
#define KB_CANCEL   0x1b
#define KB_NONE     0x00

typedef enum {
  DISPLAY_CENTER = 1,
  DISPLAY_LEFT,
  DISPLAY_RIGHT
}E_DISPALIGN;

typedef enum {
  MENU_1 = 1,
  MENU_2,
  MENU_3
}E_MENUTYPE;
  
typedef enum {
	  INPUT_NUM = 1,
	  INPUT_STR,
	  INPUT_AMT,
	  INPUT_HEX,
	  INPUT_IP,
	  INPUT_PWD
}E_INPUTTYPE;
	  
typedef enum {
	QR_TYPE_NUL = -1,   ///< Terminator (NUL character). Internal use only
	QR_TYPE_NUM = 0,    ///< Numeric mode
	QR_TYPE_AN,         ///< Alphabet-numeric mode
	QR_TYPE_8,          ///< 8-bit data mode
	QR_TYPE_KANJI,      ///< Kanji (shift-jis) mode
	QR_TYPE_STRUCTURE,  ///< Internal use only
	QR_TYPE_ECI,        ///< ECI mode
	QR_TYPE_FNC1FIRST,  ///< FNC1, first position
	QR_TYPE_FNC1SECOND, ///< FNC1, second position
} E_QRTYPE;

typedef struct 
{
    u16 bfType;
    u32 bfSize;
    u16 bfReserved1;
    u16 bfReserved2;
    u32 bfOffBits;

    u32 biSize;
    int biWidth;
    int biHeight;
    u16 biPlanes;
    u16 biBitCount;
    u32 biCompression;
    u32 biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    u32 biClrUsed;
    u32 biClrImportant;
} __attribute__ ((packed)) BMP_HEAD;

typedef int (*MenuCallback)(void * pxParam);

typedef struct {
  int          kb;
  const char * text;
  MenuCallback func;
  void *       param;
}ST_MENUITEM;

typedef struct {
    u32 w; /*Width of the image map*/
    u32 h; /*Height of the image map*/
} disp_img_header_t;


/** Image header it is compatible with
 * the result from image converter utility*/
typedef struct {
    disp_img_header_t header; /**< A header describing the basics of the image*/
    const u8 * data;   /**< Pointer to the data of the image*/
} disp_img_dsc_t;

int Ui_GetLcdWidth();
int Ui_GetLcdHeight();
int Ui_GetCharWidth();
int Ui_GetCharHeight();
int Ui_GetLineEnNum();
int Ui_GetLineHzNum();
int Ui_GetLinePadding();
int Ui_GetLineNum();

void Ui_SetKBHandle(int handle);
void Ui_SetDisplayHandle(int handle);
void Ui_ClearColor(uint color);
void Ui_Clear();
void Ui_ClearBlockColor(int x, int y, int width, int height, uint color);
void Ui_ClearBlock(int x, int y, int width, int height);
void Ui_ClearLineColor(int line, uint color);
void Ui_ClearLine(int start, int end);
void Ui_Disp(int x, int y, const char *text, uint fontColor, uint bgColor, uint size, uint rever);
void Ui_DispTextColor(int x, int y, const char *text, uint color);
void Ui_DispText(int x, int y, const char *text, int isReverse);
void Ui_DispTextAlign(int y, int align, const char *text, int isReverse);
void Ui_DispTextLineAlign(int line, int align, const char *text, int isReverse);
void Ui_DispTextLineAlignColor(int line, int align, const char *text, uint fontColor, uint bgColor);
void Ui_DispBigNumStr(int y, int align, uint color, const char *text);

int Ui_DispBMP(u32 hpixel,u32 vpixel, u32 width, u32 height, const char * bmpdata);
void Ui_DispTitle(const char * title);

void Ui_ClearKey();
byte Ui_GetKey();
byte Ui_WaitKey(uint timeMS);

int Ui_Dailog(const char * title, const char * text, int yesNo, uint timeMS);
int Ui_Menu(const char *title, const ST_MENUITEM * menu, int num, uint timeMS);
int Ui_Select(const char *title, const char *info, const ST_MENUITEM * menu, int num, int mode, uint timeMS);

int Ui_Input(int start, const char *title, const char *text, char *value, int mode, uint max, uint min, uint timeMS);
int Ui_InputStrSelectMode(int start, const char *title, const char *text, char *value, int mode, uint max, uint min, uint timeMS);

int Ui_InputStr(const char * title, const char * text, char * value, uint max, uint min, uint timeMS);
int Ui_InputStrEx(const char * title, const char * text, char * value, uint max, uint min, uint timeMS);
int Ui_InputNum(const char * title, const char * text, char * value, uint max, uint min, uint timeMS);
int Ui_InputPwd(const char * title, const char * text, char * value, uint max, uint min, uint timeMS);
int Ui_InputHex(const char * title, const char * text, char * value, uint max, uint min, uint timeMS);
int Ui_InputIp(const char * title, const char * text, char * value, uint max, uint min, uint timeMS);
int Ui_InputAmt(const char * title, const char * text, char * value, uint max, uint min, uint timeMS);
char Ui_GetKeyMask(char curKey, uint *times);
char Ui_GetRealKey(char curKey, uint times, int mode);
void Ui_Print(u32 row, u32 mode, const char *str, ...);
void Ui_DispArabCommon(int x, int y, E_DISPALIGN align, const char *text, uint fontColor, uint bgColor, uint size, uint rever);
#endif

