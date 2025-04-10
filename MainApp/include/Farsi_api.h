#ifndef __FARSI_API_H__
#define __FARSI_API_H__

#define MAX_FARSI_HEIGHT     (48)
#define LCD_MAX_XPIXEL       (320)
#define ONE_FARSI_MAX_AREA   ((MAX_FARSI_HEIGHT * LCD_MAX_XPIXEL)/8)  
typedef struct {       
	int width; 
	int height;
	unsigned char bitmap[ONE_FARSI_MAX_AREA];       
} Farsi_Dot_t;

/**For customer API**/
int LiteMGetFarsiDot(int fontWidth, int fontHight, int isBlod, unsigned short *farsi_str, Farsi_Dot_t* dot);

#endif
