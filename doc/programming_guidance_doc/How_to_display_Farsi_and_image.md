## **How to display Farsi and image**

### 1. display Farsi
It takes two steps to display Farsi/Arabic text

1. call `cFontAttrSet` set font attributes, including size, bold or not bold , font color,  with or without background color, background color, etc.The source code of `cFontAttrSet` has been provided in the demo code,  you can refer to it for details.

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
   
2. Call `cLcdPrintf` to  display at different positions on the screen.   `cLcdPrintf `includes the processing of getting bitmap data of  text and displaying bitmap data to LCD. The source code of `cLcdPrintf` has been provided in the demo code,  you can refer to it for details.

   ```C
   int cLcdPrintf(int x,int y,UI_ALIGN_MODE align_mode,const char * fmt,...);
   ```

**Note**: currently only large/medium/small, normal/bold combinations of different fonts can be displayed. The encoding format  of text displayed must be utf-8.

**For complete processing, please refer to the functions `FontAlignTest`, `FontSetAttr`, `FontDisplayWithXY`, `LcdBackGroundTest` of demo source code. The following is just a show of the `FontAlignTest` function:**

```C
void FontAlignTest()
{
    cLcdClear();

    cFontAttrSet(FONT_ATTR_SIZE,38);
    cFontAttrSet(FONT_ATTR_BLOD, FONT_NOBLD);

    cLcdPrintf(0, 0, UI_ALIGN_LEFT, "این متن ازمون فارسی اس");
    cLcdPrintf(0, 32, UI_ALIGN_CENTER, "این متن ازمون فارسی اس");
    cLcdPrintf(0, 64, UI_ALIGN_RIGHT, "این متن ازمون فارسی اس");

    cLcdPrintf(0, 96, UI_ALIGN_LEFT, "TestString:%d",1);
    cLcdPrintf(0, 128, UI_ALIGN_CENTER, "TestString:%d",1);
    cLcdPrintf(0, 160, UI_ALIGN_RIGHT, "TestString:%d",1);

    getkey();
}
```
### 2. display image directly with picture
You can call the `cLcddrawPicture` or `lcdDrawPictureBMP` function to directly use the picture file to display the picture, just pass in the file name.The `cLcddrawPicture` and `lcdDrawPictureBMP` function internally implements the decoding function of picture files, and processes the decoded picture data for display.

**Note: Support display png pictures in most formats, but only support display bmp pictures in 24-bit deep format.**

```C
/*
Support display png image in most formats
*/
int cLcddrawPicture(int x,int y,char *filename)
{
	/******/
}
```

```C
/*
Only supports display bmp pictures in 24-bit deep format.
*/
int lcdDrawPictureBMP(int x, int y, char *filename)
{
    /******/
}
```

### 3. display image with bitmap of picture

**The solution to display picture is as follows:**

1. Convert images to bitmap data using LVGL tools, finally, a C file will be generated, the detailed operation is as follows.

   ![](https://i.postimg.cc/15wkh9pB/20230524184544.png)

   The C file generated by the conversion is as follows

   ![](https://i.postimg.cc/PxkgqJ12/20230524185914.png)

2. Transplant the generated bitmap data into the code. When transplanting, we only need to use the bitmap data corresponding to [LV_COLOR_DEPTH == 16 && LV_COLOR_16_SWAP == 0] in the C file, other data do not need to be used. The bitmap data that needs to be used is shown in the figure below.

![](https://i.postimg.cc/ry9DMt5Z/20230524190424.png)

![](https://i.postimg.cc/hjLfJX6j/20230524190409.png)

![](https://i.postimg.cc/1XvkHNzM/20230524201755.png)

	Transplant method: use `disp_img_dsc_t` to create a new structure instance `dsp_logo_image` , replace the width, height and data of `dsp_logo_image` with the width, height and bitmap data in the C file as shown in the figure of step 2，code show as below.

```C
const char dsp_logo[]={0xc8, 0xe1, 0x87, 0xd9, 0x86, 0xd9}//Due to the data is too large, it is not complete.
const disp_img_dsc_t dsp_logo_image = {
  .header.w = 194,
  .header.h = 55,
  .data = dsp_logo,
};
```

3. Call `Ui_DispPic` to display bitmap data for show PIC.

   ```C 
   void Ui_DispPic(const char *pic,u32 w,u32 h,u32 startx,u32 starty);
   Ui_DispPic(dsp_logo_image.data,dsp_logo_image.header.w,dsp_logo_image.header.h,50,50);
   ```

   The effect of [Display Image] in Menu7 of Demo is as follows.and please refer to `Demo_DispImage` function of demo code.

   ![](https://i.postimg.cc/SNGh56FV/20230524192428.jpg)

   ```C
   int Demo_DispImage(void){
       Ui_Clear();
       Ui_DispPic(dsp_logo_image.data,dsp_logo_image.header.w,dsp_logo_image.header.h,50,50);
   
       Ui_WaitKey(100*100);
   
       return 0;
   }
   ```

   

   **Other points to note when developing:**

   - The current processing is that the bitmap data of the image is stored in memory, so using too large and too many images  may be affect the compilation or may be cause the program to fail to apply for more memory.
   - The current method only supports pictures in BMP, JPG, and PNG formats.