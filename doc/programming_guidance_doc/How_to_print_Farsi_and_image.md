## **How to print Farsi and image**


### 1. print Farsi

It takes two steps to print Farsi/Arabic text

1. call `cFontAttrSet` set font attributes, including size, bold or not bold , font color,  with or without background color, background color, etc.

   ```C
   typedef enum {FONT_ATTR_SIZE=0,FONT_ATTR_BLOD,FONT_ATTR_BACKGROUND,
   			FONT_ATTR_BACKCOLOR,FONT_ATTR_COLOR,FONT_COLOR_INVERSION,
   			FONT_ATTR_RETRACT}FONT_ATTR_TYPE;
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
   ```

2. Call `Print_Printf` to  display at different positions on the screen.    `Print_Printf` includes getting the text bitmap data and converting the bitmap data into the data needed for printing, and finally printing. The source code of `Print_Printf` has been provided in the demo code, please refer to it for details.

   ```C
   int Print_Printf(int x, UI_ALIGN_MODE align_mode,const char * fmt,...);
   ```

**Note**: currently only large/medium/small, normal/bold combinations of different fonts can be printed. The encoding format  of text printed must be utf-8.
**For complete processing, please refer to the functions `printerTest` of demo source code, it shows the printing function in different scenarios. **

```C
void printerTest()
{
    testPngFileLoad();

    cFontAttrSet(FONT_ATTR_SIZE,FONT_SIZE_MIN);
    cFontAttrSet(FONT_ATTR_BLOD,FONT_NOBLD);
    Print_Printf(0, UI_ALIGN_LEFT, "1111ABC>%d", 12);
    addRowHeight(24);
    Print_Printf(0, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");
    addRowHeight(24);
    cFontAttrSet(FONT_ATTR_BLOD,FONT_BOLD);
    Print_Printf(0, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");
    addRowHeight(24);

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

    Print_drawPicture(0, TEST_ICON);
    addRowHeight(24);
    Print_drawPicture(20, TEST_ICON);
    // Print_drawPictureBMP(0, TEST_BMP);

    Print_Printf_WithCharInterval(0,0,"ABCDEFG>%d",9);
    Print_Printf_WithCharInterval(10,0,"ABCDEFG>%d",9);
    Print_Printf_WithCharInterval(0,1," ABCDEFG>%d",9);
    Print_Printf_WithCharInterval(10,1," ABCDEFG>%d",9);

    addRowHeight(150);
}
```

### 2. print image directly with picture
You can call the `Print_drawPicture`  or `Print_drawPictureBMP` function to directly use the picture file to display the picture, just pass in the file name. The ``Print_drawPicture`  ` and `Print_drawPictureBMP` function internally implements the decoding function of picture files, and processes the decoded picture data for display.

**Note: Support print png pictures in most formats, but only support print bmp pictures in 24-bit deep format.**


```C
/*
Support printing png image in most formats.
*/
int Print_drawPicture(int x, char *filename)
{
	********
}
```

```C
/*
Only supports printing bmp pictures in 24-bit deep format.
*/
int Print_drawPictureBMP(int x, char *filename)
{
	********
}
```