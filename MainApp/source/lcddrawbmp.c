#include "app.h"
#include "ui.h"

static int bmp24to16(u8 *data, int len, u8 *buf)
{
    int i;
    int j = 0;
    u16 col;

    for (i = 0; i < len; i += 3) {
        col = 0;
        col |= ((data[i + 2] >> 3) & 0x1F) << 11;
        col |= ((data[i + 1] >> 2) & 0x3F) << 5;
        col |= ((data[i + 0] >> 3) & 0x1F);

        buf[j++] = col & 0xFF;
        buf[j++] = (col >> 8) & 0xFF;
    }

    return j;
}

static void s_LcdDrawLineEx(u32 x1, u32 y1, u32 x2, u32 y2, u8 *data)
{ 
	CLcdDrawArea(x1, y1, x2 - x1 + 1, y2 - y1 + 1, data);
    return;
}


static void display_bmp(u32 startx, u32 starty, u32 width, u32 height, const char * data)
{
    BMP_HEAD *bmp;
    u32 head_buf[14];
    //u8 *pbuf = data;
    u8 tmpbuf[1024];
    int i;
    int bytes_per_pix;
    u8 buf[640];
	u32 offset, curwidth;
	
	memcpy(head_buf, (void*)data, sizeof(BMP_HEAD));
    bmp = (BMP_HEAD*)head_buf;    
    if(bmp->bfType != 0x4d42) {
        // usb_debug("not BM flag");
        return;
    }
    
    if(bmp->biBitCount != 24) {
        // usb_debug("BitsPerPixel %d", bmp->biBitCount);
        return;
    }

    bytes_per_pix = bmp->biBitCount / 8;
	if(bmp->biWidth > width) curwidth = width;
	else curwidth = bmp->biWidth;
	
    offset = bmp->bfOffBits;
	//usb_debug("bmp height:%d, bmp width:%d\r\n", bmp->biHeight, bmp->biWidth);
	//usb_debug("height:%d, width:%d\r\n", height, width);
    for (i = 0; i < bmp->biHeight; i++) {
		memcpy(tmpbuf, (void*)(data + offset), bmp->biWidth * bytes_per_pix);
		if(bmp->biHeight -i < height)
		{
        	bmp24to16(tmpbuf, bmp->biWidth * bytes_per_pix, buf);
        	//s_LcdDrawLine(0, bmp->biHeight - i -1, bmp->biWidth - 1, buf);
        	s_LcdDrawLineEx(startx, bmp->biHeight -i -1 + starty, curwidth - 1 + startx, bmp->biHeight -i - 1 + starty, buf);
		}

        offset += bmp->biWidth * bytes_per_pix;
    }
}

int Ui_DispBMP(u32 hpixel,u32 vpixel, u32 width, u32 height, const char * bmpdata)
{
	int iRet;

	//DBG_STR("hpixel=%d, vpixel=%d, width=%d, height=%d, value_addr=%08x", hpixel, vpixel, width, height, value_addr);
	display_bmp(hpixel, vpixel, width, height, bmpdata);

	return 0;
}

void bmpGetRGB565Data(u32 width, u32 height, const char * data, char *outData)
{
    BMP_HEAD *bmp;
    u32 head_buf[14];
    u8 tmpbuf[1024];
    int i,ret, outOffSet;
    int bytes_per_pix;
    u8 buf[640];
	u32 offset, curwidth;

	memcpy(head_buf, (void*)data, sizeof(BMP_HEAD));
    bmp = (BMP_HEAD*)head_buf;    
    if(bmp->bfType != 0x4d42) {
        //DBG_STR("not BM flag");
        return;
    }
    
    if(bmp->biBitCount != 24) {
        //DBG_STR("BitsPerPixel %d", bmp->biBitCount);
        return;
    }

    bytes_per_pix = bmp->biBitCount / 8;
	if(bmp->biWidth > width) curwidth = width;
	else curwidth = bmp->biWidth;
	
    offset = bmp->bfOffBits;
    outOffSet = 0;
    for (i = 0; i < bmp->biHeight; i++) {
		memcpy(tmpbuf, (void*)(data + offset), bmp->biWidth * bytes_per_pix);
		if(bmp->biHeight -i < height)
		{
        	ret = bmp24to16(tmpbuf, bmp->biWidth * bytes_per_pix, buf);
            memcpy(outData+outOffSet, buf, ret);
            //DBG_STR("outOffSet:%d,size:%d", outOffSet, ret);
            outOffSet+=ret;
		}
        offset += bmp->biWidth * bytes_per_pix;
    }
}
