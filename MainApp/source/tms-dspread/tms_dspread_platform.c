/*
********************************************************************************
*
*   File Name:
*       tms_dspread_platform.c
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#define _TMS_DSPREAD_PLATFORM_MODULE
#define _TMS_DSPREAD_PLATFORM_C

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/
#include "dymem.h"
#include "chttp.h"

#include "tms_dspread_cfg.h"

// #include <liteos_api.h>
// #include "liteMapi.h"
// #include "type.h"


#include "app.h"
#include "ui.h"
#include "utils.h"

#include "string.h"
#include "stdio.h"

#include "cJSON.h"
#include "dsp_lib_api.h"
#include "lring.h"


/*-----------------------------------------------------------------------------
|   Macros
+----------------------------------------------------------------------------*/
#if !defined(TMS_PLATFORM_HEAP_SIZE)
#define TMS_PLATFORM_HEAP_SIZE    (1024*1024*16)
#endif
#if !defined(TMS_PATCH_BLK_SIZE)
#define TMS_PATCH_BLK_SIZE        (1024*1024)
#endif



/*-----------------------------------------------------------------------------
|   Enumerations
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Typedefs
+----------------------------------------------------------------------------*/
typedef struct _tms_heap_bgn_stru_t
{
	int             size;
} tms_heap_bgn_stru_t, *ptms_heap_bgn_stru_t;

typedef const tms_heap_bgn_stru_t* pctms_heap_bgn_stru_t;



/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/
// static char g_dymem[TMS_PLATFORM_HEAP_SIZE];
// static void* g_dymem_fp = 0;



/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Functions
+----------------------------------------------------------------------------*/
int tms_dspread_progress_cancel(void);

/*--------------------------------------
|   Function Name:
|       url_scheme
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static unsigned int crc32bzip2(unsigned int crc, void const* mem, int len)
{
    unsigned char const* data = mem;
    unsigned k;
    if (data == NULL) return 0;
    crc = ~crc;
    while (len--) {
        crc ^= (unsigned)(*data++) << 24;
        for (k = 0; k < 8; k++)
            crc = crc & 0x80000000 ? (crc << 1) ^ 0x4c11db7 : crc << 1;
    }
    crc = ~crc;
    return crc;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_alloc
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void* tms_dspread_alloc(int size)
{
    // if (g_dymem_fp == 0)
    // {
    //     g_dymem_fp = dymem_generate(g_dymem, sizeof(g_dymem));
    // }
    // return dymem_alloc(g_dymem_fp, size);    
    return malloc(size);
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_free
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void tms_dspread_free(void* addr)
{
    // if (g_dymem_fp == 0)
    // {
    //     g_dymem_fp = dymem_generate(g_dymem, sizeof(g_dymem));
    // }
    // dymem_free(g_dymem_fp, addr);

    free(addr);
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_realloc
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void* tms_dspread_realloc(void* addr, int size)
{
    // if (g_dymem_fp == 0)
    // {
    //     g_dymem_fp = dymem_generate(g_dymem, sizeof(g_dymem));
    // }
    // return dymem_realloc(g_dymem_fp, addr, size);

    return realloc(addr, size);
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_clearline
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void tms_dspread_clearline(int line, uint color)
{
    Ui_ClearLine(line,line); 
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_display
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void tms_dspread_display(int line, const char *text)
{
    Ui_ClearLine(line,line);
    Ui_DispTextLineAlign(line, DISPLAY_CENTER, text, 0);   
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_net_connect
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_net_connect(void** pctx, const char* host, const char* port)
{
    int ret = 0;
    int socket_id = 0;
    struct net_sockaddr addr;
    u8 tm = 1;

    cJSON_Hooks hook;

    hook.malloc_fn = (void* (CJSON_CDECL*)(size_t))tms_dspread_alloc;
    hook.free_fn = (void (CJSON_CDECL*)(void*))tms_dspread_free;
    cJSON_InitHooks(&hook);

    chttp_init_hooks(tms_dspread_alloc, tms_dspread_free, tms_dspread_realloc);

    ret = NetSocket(NET_AF_INET, NET_SOCK_STREAM, 0);
    if(ret < 0)
    {
        TMS_PRINTF("%s,%d ret = %d\r\n", __FUNCTION__, __LINE__, ret);
        return ret;
    }
    socket_id = ret;
    TimerSet(tm, TMS_SERVER_TIMEOUT/100);
    while(1){
        ret = Netioctl(socket_id, CMD_TO_SET, 0);
        if(ret & SOCK_EVENT_ACCEPT)
        {
            break;
        }
        if(0 == TimerCheck(tm) || tms_dspread_progress_cancel())
        {
            TMS_PRINTF("socket timeout\r\n");
            return -1;
        }
    }

    //SockAddrSet(&addr, "211.157.135.52", 9090);
    //SockAddrSet(&addr, "54.232.124.205", 9997);
    //strcpy(addr.host, "www.dspreadserv.net");
    
    SockAddrSet(&addr, (char*)host, atoi(port));
    ret = NetConnect(socket_id, &addr, sizeof(struct net_sockaddr));
    if(ret < 0)
    {
        TMS_PRINTF("%s,%d ret = %d\r\n", __FUNCTION__, __LINE__, ret);
        return ret;
    }
    TimerSet(tm, TMS_SERVER_TIMEOUT/100);
    while(1){
        ret = Netioctl(socket_id, CMD_TO_SET, 0);
        if(ret & SOCK_EVENT_CONN)
        {
            break;
        }
        if(0 == TimerCheck(tm)  || tms_dspread_progress_cancel())
        {
            TMS_PRINTF("socket connect timeout\r\n");
            return -1;
        }
    }
    *pctx = (void*)socket_id;
    return 0;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_net_disconnect
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_net_disconnect(void* pctx)
{
    int ret = 0;
    int socket_id = (int)pctx;

    ret = NetCloseSocket(socket_id);

    return ret;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_net_send
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_net_send(void* pctx, const unsigned char *buf, int len)
{
    int ret = 0;
    int socket_id = (int)pctx;
    int cnt = 0;

    while(len > cnt)
    {
        ret = NetSend(socket_id, (char*)buf + cnt, len - cnt, 0);
        if(ret < 0)
        {
            TMS_PRINTF("%s,%d ret = %d\r\n", __FUNCTION__, __LINE__, ret);
            return ret;
        }
        cnt += ret;
    }
    return cnt;
}


unsigned char* g_ring_rev_buf = NULL;
ring_t g_ring_net;
#define NET_REV_MAX (1024*8)
int tms_dspread_net_recv_ring(void* pctx, unsigned char *buf, int len, int timeout_ms)
{
    int ret = 0;
    int socket_id = (int)pctx;
    int cnt = 0, rev_times=0;      

    if(!g_ring_rev_buf)    
    {
        g_ring_rev_buf = malloc(NET_REV_MAX+10);
        g_ring_net.buffer = g_ring_rev_buf;
        g_ring_net.width = 1;
        g_ring_net.numbers = NET_REV_MAX;
        lring_init(&g_ring_net);
        
    }

    if(g_ring_rev_buf == NULL)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return -1;
    }
        
    if(!lring_is_empty(&g_ring_net))
    {
        for(cnt=0;cnt<len;cnt++)
        {
            if(lring_is_empty(&g_ring_net))
                break;                
            lring_get(&g_ring_net, &buf[cnt]);
        }
        //TMS_PRINTF("%s,%d cnt = %d c = %02x\r\n", __FUNCTION__, __LINE__, cnt, buf[0]);
        return cnt;
    }

    unsigned char* rev_buf = tms_dspread_alloc(NET_REV_MAX+10);
    if(rev_buf == NULL)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return -1;
    }
    
    cnt = 0;
    while(NET_REV_MAX > cnt)
    {
        ret = NetRecv(socket_id, &rev_buf[cnt], NET_REV_MAX-cnt, 0);
        if(ret < 0)
        {
            TMS_PRINTF("%s,%d ret = %d\r\n", __FUNCTION__, __LINE__, ret);
            if(rev_buf)    tms_dspread_free(rev_buf);
            return ret;
        }
        //TMS_PRINTF("%s,%d ret = %d, len = %d, cnt = %d\r\n", __FUNCTION__, __LINE__, ret, len, cnt);
        cnt += ret;
        if((rev_times++)>timeout_ms/10)
            break;

        DelayMs(10);
    }

    if(cnt>0)
    {    
        if(len>=cnt)
        {
            memcpy(buf, rev_buf, cnt);
        }
        else
        {
            int i = 0;
            memcpy(buf, rev_buf, len);
            i = len;
            //DBG_HEX("rev_buf :\r\n", rev_buf, cnt);
            for(;i<cnt;i++)
            {
                lring_put(&g_ring_net, &rev_buf[i]);
            }
            cnt = len;
        }


    }

    if(rev_buf)   tms_dspread_free(rev_buf);

    //TMS_PRINTF("%s,%d cnt = %d\r\n", __FUNCTION__, __LINE__, cnt);
    return cnt;
}
/*--------------------------------------
|   Function Name:
|       tms_dspread_net_recv
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_net_recv(void* pctx, unsigned char *buf, int len, int timeout_ms)
{   
    u8 tm = 1;
    int cnt = 0, ret = 0;

    TimerSet(tm, timeout_ms/100); 
    while(cnt<len)
    {
        ret = tms_dspread_net_recv_ring(pctx, &buf[cnt], len-cnt, 20);
        if(ret < 0)
        {
            return ret;
        }

        cnt += ret;

        if(TimerCheck(tm)==0)
        {
            break;
        }
    } 

    return cnt;
}
/*--------------------------------------
|   Function Name:
|       tms_dspread_fopen
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_fopen(void** fp, const char* path, const char* mode)
{
    #if 1
    int ret = 0;
    int fd = 0;
    unsigned char m = 0;

    if(strcmp(mode, "wb+") == 0)
    {
       m = O_CREATE|O_RDWR;
    }
    else
    {
       m = O_RDWR;
    }

    ret = open((char*)path, m);
    if (ret < 0)
    {
    }

    fd = ret;
    seek(fd, 0, SEEK_SET);
    *fp = (void*)fd;
    #endif

    return 0;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_fclose
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_fclose(void* fp)
{
    #if 1
    int fd = (int)fp;
    close(fd);
    #endif

    return 0;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_fwrite
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_fwrite(void* fp, const unsigned char* buf, int len)
{
    
    int ret = 0;
    #if 1
    int fd = (int)fp;

    ret = write(fd, (byte*)buf, len);

    return ret;
    #else
    return len;
    #endif
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_fread
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_fread(void* fp, unsigned char* buf, int len)
{    
    int ret = 0;    
    int fd = (int)fp;

    ret = read(fd, (byte*)buf, len);
    
    return ret;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_fread
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_fremove(const char* path)
{    
    int ret = 0;
    
    ret = remove(path);

    return ret;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_fmd5
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
#if TMS_VFY_MD5
int tms_dspread_fmd5(void* fp, unsigned char* buf)
{
    int res = 0;
    int fd = (int)fp;
    unsigned int total = 0;
    char* pbody = 0;
    unsigned int cnt = 0;
    md5_context ctx;

    dsp_md5_starts(&ctx);

    seek(fd, 0L, SEEK_END);
    total = ftell(fd);

    seek(fd, 0, SEEK_SET);

    pbody = tms_dspread_alloc(TMS_PATCH_BLK_SIZE);
    for (cnt = 0; cnt + TMS_PATCH_BLK_SIZE <= total; cnt += TMS_PATCH_BLK_SIZE)
    {
        res = tms_dspread_fread(fp, pbody, TMS_PATCH_BLK_SIZE);
        if (res != TMS_PATCH_BLK_SIZE)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            res = -1;
            break;
        }
        res = dsp_md5_update(&ctx, pbody, res);
        if (res != 0)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            res = -1;
            break;
        }
    }
    do
    {
        if (cnt != total)
        {
            res = tms_dspread_fread(fp, pbody, total - cnt);
            if (res != total - cnt)
            {
                TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
                res = -1;
                break;
            }
            res = dsp_md5_update(&ctx, pbody, res);
            if (res != 0)
            {
                TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
                res = -1;
                break;
            }
        }
    } while (0);

    res = dsp_md5_finish(&ctx, buf);

    return res;
}
#endif

/*--------------------------------------
|   Function Name:
|       tms_dspread_fcrc32
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
#if TMS_VFY_CRC
int tms_dspread_fcrc32(void* fp, unsigned int* crc)
{
    int res = 0;
    int fd = (int)fp;
    unsigned int total = 0;
    char* pbody = 0;
    unsigned int cnt = 0;
    unsigned int icrc = 0;

    seek(fd, 0L, SEEK_END);
    total = ftell(fd);

    seek(fd, 0, SEEK_SET);

    pbody = tms_dspread_alloc(TMS_PATCH_BLK_SIZE);
    for (cnt = 0; cnt + TMS_PATCH_BLK_SIZE <= total; cnt += TMS_PATCH_BLK_SIZE)
    {
        res = tms_dspread_fread(fp, pbody, TMS_PATCH_BLK_SIZE);
        if (res != TMS_PATCH_BLK_SIZE)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            res = -1;
            break;
        }
        icrc = crc32bzip2(icrc, pbody, res);
        res = 0;
    }
    do
    {
        if (cnt != total)
        {
            res = tms_dspread_fread(fp, pbody, total - cnt);
            if (res != total - cnt)
            {
                TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
                res = -1;
                break;
            }
            icrc = crc32bzip2(icrc, pbody, res);
            res = 0;
        }
    } while (0);

    *crc = icrc;

    return res;
}
#endif

/*--------------------------------------
|   Function Name:
|       tms_dspread_progress
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void tms_dspread_progress(char* name, unsigned int range_bgn, unsigned int range_end, unsigned int total)
{
    u8 disp_buf[128] = {0};
    u8 timeBuf[32]={0};

    float p = ((float)range_end+1)/(float)total;

    //int percent1 = (range_end + 1) * 100 / total, percent2 = (unsigned int)((unsigned long long)((range_end + 1) * 100*100)  / total % 100);
    int percent1 = (range_end + 1) * 100 / total, percent2 = (int)(p*100*100)%100;

    GetTime(timeBuf);

    extern int BCDtoint(int bcd);
    usb_debug("%02d:%02d:%02d %s:%08d-%08d/%08d:%d.%02d\r\n", BCDtoint(timeBuf[3]), BCDtoint(timeBuf[4]), BCDtoint(timeBuf[5]), \
        name, range_bgn, range_end, total, percent1, percent2);  
    //usb_debug("percent = %02d.%02d\r\n", percent1, percent2);  
    percent2 = (percent1 == 100?0:percent2);

    sprintf(disp_buf, "%02d.%02d", percent1, percent2);

    tms_dspread_clearline(5,5);
    tms_dspread_display(5,  disp_buf);
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_progress_cancel
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_progress_cancel(void)
{
    //TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
    if(kbhit() != 0x00)
		return 0;

    //TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
	if(getkey()==KB_CANCEL)
        return 1;

    //TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
    return 0;
}

