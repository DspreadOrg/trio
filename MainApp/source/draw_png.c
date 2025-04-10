#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "app.h"
#include "spng.h"
#include "Liteui.h"

int png_decode_size(int context, size_t *size, u32 *width, u32 *height)
{
	int ret;
    spng_ctx *ctx;
    struct spng_ihdr ihdr;

    ctx = (spng_ctx *)context;

    ret = spng_decoded_image_size(ctx, SPNG_FMT_RGB8, size);
    if (ret != SPNG_OK) {
        DBG_STR("spng_decoded_image_size ret %d", ret);
        return -1;
    }

    ret = spng_get_ihdr(ctx, &ihdr);
    if (ret != SPNG_OK) {
        DBG_STR("spng_get_ihdr ret %d", ret);
        return -1;
    }

    *width =  ihdr.width;
    *height = ihdr.height;

    return 0;
}

int png_decode(int context, u8 *out, size_t size)
{
	int ret;
    spng_ctx *ctx;

    ctx = (spng_ctx *)context;
    if (ctx == NULL) {
        return -1;
    }

    ret = spng_decode_image(ctx, out, size, SPNG_FMT_RGB8, 0);
    if (ret != SPNG_OK) {
        DBG_STR("spng_decode_image ret %d", ret);
        return -1;
    }

    return 0;
}

static int png_decode_ex(int context, u8 *out, size_t size, void *fun)
{
	int ret;
    spng_ctx *ctx;

    ctx = (spng_ctx *)context;
    if (ctx == NULL) {
        return -1;
    }

    ret = spng_decode_image_ex(ctx, out, size, SPNG_FMT_RGB8, 0, fun);
    if (ret != SPNG_OK) {
        DBG_STR("spng_decode_image ret %d", ret);
        if (ret == SPNG_IO_ERROR) {
            return -2;
        } else {
            return -1;
        }
    }

    return 0;
}

void *calloc (size_t nelem, size_t elsize)
{
  void *ptr;  
  
  if (nelem == 0 || elsize == 0)
    nelem = elsize = 1;
  
  ptr = malloc(nelem * elsize);
  if (ptr) {
    memset(ptr, 0, nelem * elsize);
  }
  
  return ptr;
}

int png_decode_init(u8 * png, u32 len)
{
    spng_ctx *ctx;
    int ret;

    ctx = spng_ctx_new(0);
    if (ctx == NULL) {
        return 0;
    }

    ret = spng_set_png_buffer(ctx, png, len);
    if (ret != SPNG_OK) {
        DBG_STR("spng_set_png_buffer ret %d", ret);
        spng_ctx_free(ctx);
        return 0;
    }

    return (int)ctx;
}

void png_decoder_uninit(int context)
{
    if (context != 0) {
        spng_ctx_free((spng_ctx *)context);
    }
}

int draw_png(int x, int y, u8 *png, u32 len)
{
    int ret;
    int result = -1;
    int png_ctx;
    int image_width;
    int image_height;
    size_t size;
    u8 *out = NULL;
    u32 start;


    png_ctx = png_decode_init(png, len);
    if (png_ctx == 0) {
        return -1;
    }

    ret = png_decode_size(png_ctx, &size, &image_width, &image_height);
    if (ret < 0) {
        goto release;
    }

    //DBG_STR("image info, size %d  width %d  height %d", size, image_width, image_height);
    out = (u8 *)malloc(size);
    if (out == NULL) {
        goto release;
    }

    start = LvosSysTick();
    ret = png_decode(png_ctx, out, size);
    if (ret < 0) {
        goto release;
    }
    //DBG_STR("deoode time %d", LvosSysTick() - start);

    CLcdDrawArea(x, y, image_width, image_height, (u16*)out);
    
    result = 0;

release:
    png_decoder_uninit(png_ctx);

    if (out != NULL) {
        free(out);
    }

    return result;
}

int draw_png_ex(int x, int y, u8 *png, u32 len, void *fun)
{
    int ret;
    int result = -1;
    int png_ctx;
    int image_width;
    int image_height;
    size_t size;
    u8 *out = NULL;
    u32 start;

    png_ctx = png_decode_init(png, len);
    if (png_ctx == 0) {
        return -1;
    }

    ret = png_decode_size(png_ctx, &size, &image_width, &image_height);
    if (ret < 0) {
        goto release;
    }

    DBG_STR("image info, size %d  width %d  height %d", size, image_width, image_height);
    out = (u8 *)malloc(size);
    if (out == NULL) {
        goto release;
    }

    start = LvosSysTick();
    ret = png_decode_ex(png_ctx, out, size, fun);
    if (ret < 0) {
        DBG_STR("decode %d, %d", ret, SPNG_IO_EOF);
        result = ret;
        goto release;
    }
    DBG_STR("deoode time %d", LvosSysTick() - start);

    CLcdDrawArea(x, y, image_width, image_height, (u16 *)out);

    result = 0;

release:
    png_decoder_uninit(png_ctx);

    if (out != NULL) {
        free(out);
    }

    return result;    
}
