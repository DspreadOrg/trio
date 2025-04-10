/*
********************************************************************************
*
*   File Name:
*       tms_dspread.c
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#define _TMS_DSPREAD_MODULE
#define _TMS_DSPREAD_C

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "tms_dspread_cfg.h"
#include "tms_dspread.h"
#include "tms_dspread_platform.h"


#define OTA_FILE ("ota.asc")

unsigned char g_fname_[128] ={0};

/*-----------------------------------------------------------------------------
|   Macros
+----------------------------------------------------------------------------*/
#if !defined(TMS_SERVER_TIMEOUT)
#define TMS_SERVER_TIMEOUT        (10000)
#endif
#if !defined(TMS_PATCH_BLK_SIZE)
#define TMS_PATCH_BLK_SIZE        (1024*1024)
#endif
#if !defined(TMS_HEART_DLY_DEFAULT)
#define TMS_HEART_DLY_DEFAULT     (1440)
#endif
#if !defined(TMS_UPDATE_DLY_DEFAULT)
#define TMS_UPDATE_DLY_DEFAULT    (800)
#endif
#if !defined(TMS_PATCH_BLK_RETRY)
#define TMS_PATCH_BLK_RETRY       (3)
#endif



/*-----------------------------------------------------------------------------
|   Enumerations
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Typedefs
+----------------------------------------------------------------------------*/
typedef struct _tms_path_t
{
    char type[6];
    int num;
    char path[256];
    int force;
    struct _tms_path_t* next;
}tms_path_t, *ptms_path_t;

typedef const tms_path_t     *pctms_path_t;

typedef struct _tms_priv_t
{
    char auto_dl;
    char auto_up;
    void (*dl_hook)(void* param);
    void (*up_hook)(void* param);
    int heart_dly_seconds;
    int update_dly_seconds;
    char terminal[32];
    ptms_path_t require;
}tms_priv_t, * ptms_priv_t;

typedef const tms_priv_t* pctms_priv_t;




/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/
static tms_priv_t g_tms_priv = {
    .auto_dl = 0,
    .auto_up = 0,
    .dl_hook = 0,
    .up_hook = 0,
    .heart_dly_seconds = TMS_HEART_DLY_DEFAULT / 1000,
    .update_dly_seconds = TMS_UPDATE_DLY_DEFAULT / 1000,
    .require = 0,
};



/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/
static const char* g_ppatch_type_list[] =
{
    "rom",
    "firmware",
    "other",

    "",
};



/*-----------------------------------------------------------------------------
|   Functions
+----------------------------------------------------------------------------*/
#if TMS_DBG
/*--------------------------------------
|   Function Name:
|       tms_dspread_private_trace_ascii
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static void tms_dspread_private_trace_ascii(const char* dat, int len)
{
    while (len-- != 0)
    {
        TMS_PRINTF("%c", *(dat++));
    }
    TMS_PRINTF("\r\n");
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_trace_dump
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void tms_dspread_trace_dump(const unsigned char* dat, int len, int rts)
{
	int times = 0;
	int i = 0;
	int j = 0;
	char temp[16];

	for (i = 0; i < len; i += 16)
	{
		TMS_PRINTF("%08X", i);

		times = rts;
		while (times--)
		{
			TMS_PRINTF("|   ");
		}

		TMS_PRINTF("  ");

		memset(temp, 0x00, 16);

		if ((len - i) < 16)
		{
			for (j = 0; j < (len - i) % 16; j++)
			{
				if ((j % 4) == 0)
				{
					TMS_PRINTF(" ");
				}
				TMS_PRINTF("%02X ", dat[i + j]);

				if (dat[i + j] < ' ' || dat[i + j] > '~')
				{
					temp[j] = '.';
				}
				else
				{
					temp[j] = dat[i + j];
				}
			}

			for (; j < 16; j++)
			{
				if ((j % 4) == 0)
				{
					TMS_PRINTF(" ");
				}
				TMS_PRINTF("   ");
			}
		}
		else
		{
			for (j = 0; j < 16; j++)
			{
				if ((j % 4) == 0)
				{
					TMS_PRINTF(" ");
				}
				TMS_PRINTF("%02X ", dat[i + j]);

				if (dat[i + j] < ' ' || dat[i + j] > '~')
				{
					temp[j] = '.';
				}
				else
				{
					temp[j] = dat[i + j];
				}
			}
		}

		TMS_PRINTF(" - ");

		if ((len - i) < 16)
		{
			tms_dspread_private_trace_ascii(temp, (len - i) % 16);
		}
		else
		{
			tms_dspread_private_trace_ascii(temp, 16);
		}
	}
}
#endif

/*--------------------------------------
|   Function Name:
|       url_scheme
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static unsigned int url_scheme(const char* url, char** scheme)
{
    char* bgn = 0;
    char* end = 0;

    if (url == NULL || scheme == NULL)
    {
        return 0;
    }
    *scheme = NULL;
    bgn = (char*)url;
    end = bgn;
    while (*end != ':' && *end != '\0')
    {
        end++;
    }
    *scheme = bgn;
    return end - bgn;
}

/*--------------------------------------
|   Function Name:
|       url_host
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static unsigned int url_host(const char* url, char** host)
{
    unsigned int cnt = 0;
    char* bgn = 0;
    char* end = 0;

    if (url == NULL || host == NULL)
    {
        return 0;
    }
    *host = NULL;
    bgn = strstr(url, "//");
    if (bgn == NULL)
    {
        return 0;
    }
    bgn += 2;
    end = bgn;
    while (*end != '/' && *end != ':' && *end != '\0')
    {
        end++;
    }
    *host = bgn;
    return end - bgn;
}

/*--------------------------------------
|   Function Name:
|       url_port
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static unsigned int url_port(const char* url, char** port)
{
    char* bgn = 0;
    char* end = 0;

    if (url == NULL || port == NULL)
    {
        return 0;
    }
    *port = NULL;
    bgn = strstr(url, "//");
    if (bgn == NULL)
    {
        return 0;
    }
    bgn += 2;
    if (strstr(bgn, ":") == NULL || (unsigned int)strstr(bgn, ":") > (unsigned int)strstr(bgn, "/"))
    {
        return 0;
    }
    bgn = strstr(bgn, ":");
    bgn += 1;
    end = bgn;
    while (*end != '/' && *end != '\0')
    {
        end++;
    }
    *port = bgn;
    return end - bgn;
}

/*--------------------------------------
|   Function Name:
|       url_path
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static unsigned int url_path(const char* url, char** path)
{
    char* bgn = 0;
    char* end = 0;

    if (url == NULL || path == NULL)
    {
        return 0;
    }
    *path = NULL;
    bgn = strstr(url, "//");
    if (bgn == NULL)
    {
        return 0;
    }
    bgn += 2;
    while (*bgn != '/' && *bgn != '\0')
    {
        bgn++;
    }
    if (*bgn == '\0')
    {
        return 0;
    }
    end = bgn;
    while (*end != '\0')
    {
        end++;
    }
    *path = bgn;
    return end - bgn;
}

static unsigned int crc32bzip2(unsigned int crc, void const *mem, int len)
{
    unsigned char const *data = mem;
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

static int asc_2_bcd(const char* src, int len, char* dest)
{
	int i;

	if (len % 2) {
		return 0;
	}
	else {}
	for (i = 0; i<len / 2; i++) {
		if (src[i * 2] >= '0' && src[i * 2] <= '9') {
			dest[i] = src[i * 2] - '0';
		}
		else if (src[i * 2] >= 'A' && src[i * 2] <= 'F') {
			dest[i] = src[i * 2] - 'A' + 0x0A;
		}
		else if (src[i * 2] >= 'a' && src[i * 2] <= 'f') {
			dest[i] = src[i * 2] - 'a' + 0x0A;
		}
		else {
			return 0;
		}
		dest[i] *= 16;
		if (src[i * 2 + 1] >= '0' && src[i * 2 + 1] <= '9') {
			dest[i] += src[i * 2 + 1] - '0';
		}
		else if (src[i * 2 + 1] >= 'A' && src[i * 2 + 1] <= 'F') {
			dest[i] += src[i * 2 + 1] - 'A' + 0x0A;
		}
		else if (src[i * 2 + 1] >= 'a' && src[i * 2 + 1] <= 'f') {
			dest[i] += src[i * 2 + 1] - 'a' + 0x0A;
		}
		else {
			return 0;
		}
	}
	return i;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_body_length
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_body_length(void* phttp, int* plen)
{
    int err = 0;
    char* pdata = 0;
    int cnt = 0;
    char* bgn = 0;
    char* end = 0;
    char buffer[16];

    *plen = 0;
    err = chttp_header_find(phttp, "Content-Length", &pdata, &cnt);
    if(err != CHTTP_ERR_OK)
    {
        return err;
    }
    bgn = strstr(pdata, ": ");
    bgn += 2;
    end = pdata + cnt;
    memset(buffer, 0x00, sizeof(buffer));
    memcpy(buffer, bgn, end - bgn);
    *plen = atoi(buffer);
    return CHTTP_ERR_OK;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_read_line
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_read_line(void* pctx, void* phttp)
{
    int ret = 0;
    int cnt = 0;
    char* line = 0;

    line = tms_dspread_alloc(1024);
    memset(line, 0x00, 1024);
    do
    {
        ret = tms_dspread_net_recv(pctx, &line[cnt], 1, TMS_SERVER_TIMEOUT);
        if(ret != 1)
        {
            break;
        }
    }while(line[cnt++] != '\n');

    if(ret == 1)
    {
        ret = chttp_line_set(phttp, line, strlen(line) - 2);
    }

    tms_dspread_free(line);

    return ret;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_read_header
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_read_header(void* pctx, void* phttp)
{
    int ret = 0;
    int cnt = 0;
    char* header = 0;
    int loop = 1;

    header = tms_dspread_alloc(1024);
    do
    {
        cnt = 0;
        memset(header, 0x00, 1024);
        do
        {
            ret = tms_dspread_net_recv(pctx, &header[cnt], 1, TMS_SERVER_TIMEOUT);
            if(ret != 1)
            {
                break;
            }
        }while(header[cnt++] != '\n');

        if(memcmp(header, "\r\n", 2) == 0)
        {
            ret = CHTTP_ERR_OK;
            break;
        }
        else if(ret == 1)
        {
            ret = chttp_header_add(phttp, header, strlen(header) - 2);
        }
    }while(loop);

    tms_dspread_free(header);

    return ret;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_read_body
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_read_body(void* pctx, void* phttp)
{
    int err = 0;
    int len = 0;
    char* pbody = 0;
    
    err = tms_dspread_body_length(phttp, &len);
    if(err != CHTTP_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return err;
    }
    if(len == 0)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return CHTTP_ERR_OK;
    }
    pbody = tms_dspread_alloc(len);
    if(pbody == 0)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return CHTTP_ERR_ALLOC;
    }
    do
    {
        err = tms_dspread_net_recv(pctx, pbody, len, TMS_SERVER_TIMEOUT);
        if(err != len)
        {
            TMS_PRINTF("%s,%d len = %d, err = %d\r\n", __FUNCTION__, __LINE__, len, err);
            err = CHTTP_ERR_PARAM;
            break;
        }
        err = chttp_body_set(phttp, pbody, len);
        if(err != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            break;
        }
    }while(0);
    tms_dspread_free(pbody);
    return err;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_read_http
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_read_http(void* pctx, void* phttp)
{
    int ret = 0;

    ret = tms_dspread_read_line(pctx, phttp);
    if(ret != CHTTP_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return ret;
    }
    ret = tms_dspread_read_header(pctx, phttp);
    if(ret != CHTTP_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return ret;
    }
    ret = tms_dspread_read_body(pctx, phttp);
    if(ret != CHTTP_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return ret;
    }
    return ret;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_heart_path_parser
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_heart_path_parser(ptms_path_t* proot, const char* type, const char* path, const char* firmware)
{
    char pathdir[256];
    ptms_path_t pparent = 0;
    ptms_path_t p = 0;
    int len = 0;
    int num = 0;
    char* firmbgn = 0;
    char* firmend = 0;

    p = *proot;
    while(p != 0)
    {
        pparent = p;
        p = p->next;
        tms_dspread_free(pparent);
    }
    pparent = 0;
    memset(pathdir, 0x00, sizeof(pathdir));
    strcpy(pathdir, path);
    strcat(pathdir, "/");

    firmbgn = (char*)firmware;
    
    while(*firmbgn != '\0')
    {
        p = tms_dspread_alloc(sizeof(tms_path_t));
        if (p == 0) 
        {
            return TMS_DSPREAD_ERR_PARAM;
        }
        memset(p, 0x00, sizeof(tms_path_t));
        len = 0;
        memcpy(&p->path[len], pathdir, strlen(pathdir));
        len += strlen(pathdir);

        firmbgn = strchr(firmbgn, '|');
        firmbgn += 1;
        firmend = strchr(firmbgn, '|');
        memcpy(&p->path[len], firmbgn, firmend - firmbgn);
        len += firmend - firmbgn;
        firmbgn = firmend;
        firmbgn += 1;
        p->force = atoi(firmbgn);
        firmbgn += 1;
        p->next = 0;
        memset(p->type, 0x00, sizeof(p->type));
        memcpy(p->type, type, (strlen(type) < sizeof(p->type) - 1) ? strlen(type) : sizeof(p->type) - 1);
        p->num = num++;
        TMS_PRINTF("p->type: %s\r\n", p->type);
        TMS_PRINTF("p->num: %d\r\n", p->num);
        TMS_PRINTF("p->path: %s\r\n", p->path);
        TMS_PRINTF("p->force: %d\r\n", p->force);
        if(pparent == 0)
        {
            *proot = p;
            pparent = p;
        }
        else
        {
            pparent->next = p;
            pparent = p;
        }
        firmbgn = strchr(firmbgn, ',');
        if (firmbgn == 0) 
        {
            break;
        }
        firmbgn += 1;
    }
    return TMS_DSPREAD_ERR_OK;
}


/*--------------------------------------
|   Function Name:
|       tms_dspread_http_get_block
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_http_get_block( \
    void* pctx, const char* path, const char* host, const char* port, int blkbgn, int blkend, \
    int* total, char* etag, int* etag_len, char* blk, int* blk_len \
)
{
    int ret = 0;
    void* phttpw = 0;
    void* phttpr = 0;
    char header[256];
    char* pmsg = 0;
    char* bgn = 0;
    char* end = 0;
    int len = 0;
    char tempData[64]={0};

    ret = chttp_new(&phttpw);
    if (ret != CHTTP_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return TMS_DSPREAD_ERR_PARAM;
    }
    do
    {
        ret = chttp_header_clr(phttpw);
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        sprintf(header, "GET %s HTTP/1.1\0", path);
        ret = chttp_line_set(phttpw, header, strlen(header));
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        sprintf(header, "Host: %s:%s\0", host, port);
        ret = chttp_header_add(phttpw, header, strlen(header));
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        sprintf(header, "Range: bytes=%d-%d\0", blkbgn, blkend);
        ret = chttp_header_add(phttpw, header, strlen(header));
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        sprintf(header, "Connection: Keep-Alive\0");
        ret = chttp_header_add(phttpw, header, strlen(header));
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        ret = chttp_format(phttpw, &pmsg, &len);
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        //TMS_PRINTF("http send(%d):\r\n", len);
        //TMS_DUMP(pmsg, len, 0);
        ret = tms_dspread_net_send(pctx, (const unsigned char*)pmsg, len);
        if (ret != len)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        else
        {
            ret = TMS_DSPREAD_ERR_OK;
        }
    } while (0);
    chttp_del(phttpw);
    if (ret != TMS_DSPREAD_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return ret;
    }
    ret = chttp_new(&phttpr);
    if (ret != CHTTP_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return TMS_DSPREAD_ERR_PARAM;
    }
    do
    {
        ret = tms_dspread_read_http(pctx, phttpr);
        if (ret != TMS_DSPREAD_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            break;
        }
        //ret = chttp_format(phttpr, &pmsg, &len);
        //if (ret != CHTTP_ERR_OK)
        //{
        //    TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        //    ret = TMS_DSPREAD_ERR_PARAM;
        //    break;
        //}
        //TMS_PRINTF("http recv(%d):\r\n", len);
        //TMS_DUMP(pmsg, len, 0);
        ret = chttp_header_find(phttpr, "Content-Range", &bgn, &len);
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        memset(tempData,0,sizeof(tempData));
        memcpy(tempData,bgn,len);
        bgn = strchr(tempData, '/');
        bgn += 1;
        *total = atoi(bgn);
        ret = chttp_header_find(phttpr, "ETag", &bgn, &len);
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        bgn = strchr(bgn, ':');
        bgn += 2;
        len -= strlen("ETag: ");
        memcpy(etag, bgn, len);
        *etag_len = len;
        len = 0;
        ret = chttp_body_get(phttpr, &bgn, &len);
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        memcpy(blk, bgn, len);
        *blk_len = len;
    } while (0);
    chttp_del(phttpr);
    return ret;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_heart_beat
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_heart_beat(const char* id, const char* custcode, const char* heartbeatpath, const char* dir)
{
    void *pctx = 0;
    int ret = 0;
    char host[128];
    char port[16];
    char path[256];
    void* phttpw = 0;
    void* phttpr = 0;
    char header[256];
    char* pmsg = 0;
    int len = 0;
    int i = 0;
    char* pbody = 0;
    cJSON* jsonbody = NULL;
    cJSON* jsonitem = NULL;
    cJSON* jsonpath = NULL;
    cJSON* jsonfirm = NULL;
    TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);

    len = url_host(heartbeatpath, &pmsg);
    memset(host, 0x00, sizeof(host));
    memcpy(host, pmsg, len);
    len = url_port(heartbeatpath, &pmsg);
    memset(port, 0x00, sizeof(port));
    memcpy(port, pmsg, len);
    len = url_path(heartbeatpath, &pmsg);
    memset(path, 0x00, sizeof(path));
    memcpy(path, pmsg, len);

    if(strlen(port) == 0)
    {
        strcpy(port, "80");
    }

    ret = tms_dspread_net_connect(&pctx, host, port);
    if (ret != TMS_DSPREAD_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return ret;
    }

    ret = chttp_new(&phttpw);
    if(ret != CHTTP_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        tms_dspread_net_disconnect(pctx);
        return TMS_DSPREAD_ERR_PARAM;
    }
    do
    {
        ret = chttp_header_clr(phttpw);
        if(ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        sprintf(header, "GET %s?sn=%s&heart=%d&update=%d&revisions=&custCode=%s HTTP/1.1\0", \
            path, id, TMS_HEART_DLY_DEFAULT, TMS_UPDATE_DLY_DEFAULT, custcode);
        ret = chttp_line_set(phttpw, header, strlen(header));
        if(ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        sprintf(header, "Host: %s:%s\0", host, port);
        ret = chttp_header_add(phttpw, header, strlen(header));
        if(ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        sprintf(header, "Connection: Keep-Alive\0");
        ret = chttp_header_add(phttpw, header, strlen(header));
        if (ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        ret = chttp_format(phttpw, &pmsg, &len);
        if(ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        TMS_PRINTF("socket send(%d):\r\n", len);
        TMS_DUMP(pmsg, len, 0);
        ret = tms_dspread_net_send(pctx, (const unsigned char* )pmsg, len);
        if(ret != len)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        else 
        {
            ret = TMS_DSPREAD_ERR_OK;
        }
    }while(0);
    chttp_del(phttpw);
    if(ret != TMS_DSPREAD_ERR_OK)
    {
        tms_dspread_net_disconnect(pctx);
        return ret;
    }
    ret = chttp_new(&phttpr);
    if(ret != CHTTP_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        ret = TMS_DSPREAD_ERR_PARAM;
        tms_dspread_net_disconnect(pctx);
        return ret;
    }
    do
    {
        ret = tms_dspread_read_http(pctx, phttpr);
        if(ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        ret = chttp_format(phttpr, &pmsg, &len);
        if(ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        TMS_PRINTF("socket recv(%d):\r\n", len);
        TMS_DUMP(pmsg, len, 0);
        ret = chttp_body_get(phttpr, &pbody, &len);
        if(ret != CHTTP_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        jsonbody = cJSON_ParseWithLength((const char*)pbody, len);
        if(jsonbody == 0)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
    }while(0);
    chttp_del(phttpr);
    if(ret != TMS_DSPREAD_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        tms_dspread_net_disconnect(pctx);
        return ret;
    }
    do
    {
        jsonitem = cJSON_GetObjectItem(jsonbody, "code");
        if(jsonitem == 0)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        TMS_PRINTF("code = %d\r\n", jsonitem->valueint);
        jsonitem = cJSON_GetObjectItem(jsonbody, "merchant");
        if(jsonitem == 0)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        TMS_PRINTF("merchant = %s\r\n", jsonitem->valuestring);
        jsonitem = cJSON_GetObjectItem(jsonbody, "terminal");
        if(jsonitem == 0)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        TMS_PRINTF("terminal = %s\r\n", jsonitem->valuestring);
        memset(g_tms_priv.terminal, 0x00, sizeof(g_tms_priv.terminal));
        strcpy(g_tms_priv.terminal, jsonitem->valuestring);
        jsonitem = cJSON_GetObjectItem(jsonbody, "path");
        if(jsonitem == 0)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        TMS_PRINTF("path = %s\r\n", jsonitem->valuestring);
        jsonpath = jsonitem;
        jsonitem = cJSON_GetObjectItem(jsonbody, "heart");
        if(jsonitem == 0)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        TMS_PRINTF("heart = %d\r\n", jsonitem->valueint);
        g_tms_priv.heart_dly_seconds = jsonitem->valueint;
        jsonitem = cJSON_GetObjectItem(jsonbody, "update");
        if(jsonitem == 0)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        TMS_PRINTF("update = %d\r\n", jsonitem->valueint);
        g_tms_priv.update_dly_seconds = jsonitem->valueint;
        i = 0;
        while (strlen(g_ppatch_type_list[i]) > 0)
        {
            jsonitem = cJSON_GetObjectItem(jsonbody, g_ppatch_type_list[i]);
            if (jsonitem == 0)
            {
                i++;
                continue;
            }
            TMS_PRINTF("firmware = %s\r\n", jsonitem->valuestring);
            jsonfirm = jsonitem;
            ret = tms_dspread_heart_path_parser(&g_tms_priv.require, g_ppatch_type_list[i], jsonpath->valuestring, jsonfirm->valuestring);
            if (ret != TMS_DSPREAD_ERR_OK)
            {
                TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
                break;
            }
            i++;
        }
    }while(0);
    cJSON_Delete(jsonbody);
    tms_dspread_net_disconnect(pctx);
    return ret;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_download_patch
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_download_patch(const char* id, const char* custcode, ptms_path_t p, const char* dir)
{
    char host[128];
    char port[16];
    char path[256];
    char fname[256];
    int ret = 0;
    void* pctx = 0;
    void* fp = 0;
    char* bgn = 0;
    char etag[64];
    int etag_len = 0;
    char header[256];
    int total = 0;
    int cnt = 0;
    int len = 0;
    int blksize = 0;
    char* blk = 0;
    int blk_len = 0;
    int retry = 0;
#if TMS_VFY_CRC
    unsigned int etagcrc = 0;
    unsigned int filcrc = 0;
#endif
    TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);

    len = url_host(p->path, &bgn);
    memset(host, 0x00, sizeof(host));
    memcpy(host, bgn, len);
    len = url_port(p->path, &bgn);
    memset(port, 0x00, sizeof(port));
    memcpy(port, bgn, len);
    len = url_path(p->path, &bgn);
    memset(path, 0x00, sizeof(path));
    memcpy(path, bgn, len);

    if(strlen(port) == 0)
    {
        strcpy(port, "80");
    }
    
    bgn = p->path;
    bgn = strrchr(bgn, '/');
    bgn += 1;
    bgn += strlen(bgn) - 8;
    sprintf(fname, "%s%s%d-%d\0", dir, p->type, p->num, p->force);//for test
    memset(g_fname_, 0, sizeof(g_fname_));
    strcpy(g_fname_, fname);
    tms_dspread_fremove((const char*)fname);//delete the old file each time
    ret = tms_dspread_fopen(&fp, fname, "wb+");
    if(ret != 0)
    {
        TMS_PRINTF("%s,%d ret = %d\r\n", __FUNCTION__, __LINE__, ret);
        ret = TMS_DSPREAD_ERR_PARAM;
        return ret;
    }
    blk = tms_dspread_alloc(TMS_PATCH_BLK_SIZE);
    total = TMS_PATCH_BLK_SIZE;
    while(cnt < total)
    {
        if(tms_dspread_progress_cancel())
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            tms_dspread_free(blk);
            tms_dspread_net_disconnect(pctx);
            tms_dspread_fclose(fp);
            return -1;
        } 

        if (cnt + TMS_PATCH_BLK_SIZE < total)
        {
            blksize = TMS_PATCH_BLK_SIZE;
        }
        else
        {
            blksize = total - cnt;
        }
        memset(header, 0x00, sizeof(header));
        sprintf(header, "%s&cut=1&dev=%s&custCode=%s\0", path, id, custcode);
        if (pctx == 0) 
        {
            ret = tms_dspread_net_connect(&pctx, host, port);
            if (ret != TMS_DSPREAD_ERR_OK)
            {
                TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
                break;
            }
        }
        ret = tms_dspread_http_get_block(pctx, header, host, port, cnt, cnt + blksize - 1, \
            &total, etag, &etag_len, blk, &blk_len);
        if (ret != TMS_DSPREAD_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            if (retry < TMS_PATCH_BLK_RETRY)
            {
                tms_dspread_net_disconnect(pctx);
                pctx = 0;
                retry++;
                continue;
            }
            break;
        }
        retry = 0;
        ret = tms_dspread_fwrite(fp, blk, blk_len);
        if (ret != blk_len)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        else
        {
            ret = TMS_DSPREAD_ERR_OK;
        }
#if TMS_VFY_CRC
        memset(header, 0x00, sizeof(header));
        memcpy(header, etag, 8);
        etagcrc = strtoul(header, NULL, 16);
#endif
        tms_dspread_progress(fname, cnt, cnt + blk_len - 1, total);
        cnt += blk_len;
    }
    tms_dspread_free(blk);
    tms_dspread_net_disconnect(pctx);
    tms_dspread_fclose(fp);
    if (ret != TMS_DSPREAD_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return ret;
    }
    
#if TMS_VFY_CRC
    ret = tms_dspread_fopen(&fp, fname, "rb+");
    if (ret != 0)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        ret = TMS_DSPREAD_ERR_PARAM;
        return ret;
    }
    ret = tms_dspread_fcrc32(fp, &filcrc);
    if (ret != 0)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        tms_dspread_fclose(fp);
        ret = TMS_DSPREAD_ERR_PARAM;
        return ret;
    }
    TMS_PRINTF("etagcrc: %08X\r\n", etagcrc);
    TMS_PRINTF("filcrc: %08X\r\n", filcrc);
    if (etagcrc != filcrc)
    {
        ret = TMS_DSPREAD_ERR_PARAM;
        return ret;
    }
#endif
    return TMS_DSPREAD_ERR_OK;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_download
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static int tms_dspread_download(const char* id, const char* custcode, const char* dir)
{
    int err = 0;
    ptms_path_t p = 0;
    TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);

    p = g_tms_priv.require;

    tms_dspread_clearline(1, 9);
    tms_dspread_display(3, "File DownLoading...");
    while(p != NULL)
    {
        //Ui_DispTextLineAlign(3, DISPLAY_CENTER, "File DownLoading...", 0);
        err = tms_dspread_download_patch(g_tms_priv.terminal, custcode, p, dir);
        if(err != TMS_DSPREAD_ERR_OK)
        {
            break;
        }
        p = p->next;
    }
    return err;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_dl
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_dspread_dl(const char* id, const char* custcode, const char* heartbeatpath, const char* dir)
{
    int err = 0;
    TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);

    err = tms_dspread_heart_beat(id, custcode, heartbeatpath, dir);
    if (err != TMS_DSPREAD_ERR_OK)
    {
        return err;
    }
    err = tms_dspread_download(id, custcode, dir);
    if (err != TMS_DSPREAD_ERR_OK)
    {
        return err;
    }
    return err;
}

/*--------------------------------------
|   Function Name:
|       tms_http_dl
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int tms_http_dl(const char* url, const char* filpath)
{
    char host[128];
    char port[16];
    char path[256];
    int ret = 0;
    void* pctx = 0;
    void* fp = 0;
    char* bgn = 0;
    char etag[64];
    int etag_len = 0;
    int total = 0;
    int cnt = 0;
    int len = 0;
    int blksize = 0;
    char* blk = 0;
    int blk_len = 0;
    int retry = 0;
#if TMS_VFY_MD5
    char md5[16];
    char vfy_md5[16];
#endif
    TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);

    len = url_host(url, &bgn);
    memset(host, 0x00, sizeof(host));
    memcpy(host, bgn, len);
    len = url_port(url, &bgn);
    memset(port, 0x00, sizeof(port));
    memcpy(port, bgn, len);
    len = url_path(url, &bgn);
    memset(path, 0x00, sizeof(path));
    memcpy(path, bgn, len);

    if(strlen(port) == 0)
    {
        strcpy(port, "80");
    }

    ret = tms_dspread_fopen(&fp, filpath, "wb+");
    if (ret != 0)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        ret = TMS_DSPREAD_ERR_PARAM;
        return ret;
    }

    blk = tms_dspread_alloc(TMS_PATCH_BLK_SIZE);
    total = TMS_PATCH_BLK_SIZE;
    while (cnt < total)
    {
        if(tms_dspread_progress_cancel())
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            break;
        } 

        if (cnt + TMS_PATCH_BLK_SIZE < total)
        {
            blksize = TMS_PATCH_BLK_SIZE;
        }
        else
        {
            blksize = total - cnt;
        }
        if (pctx == 0)
        {
            ret = tms_dspread_net_connect(&pctx, host, port);
            if (ret != TMS_DSPREAD_ERR_OK)
            {
                TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
                break;
            }
        }
        ret = tms_dspread_http_get_block(pctx, path, host, port, cnt, cnt + blksize - 1, \
            & total, etag, &etag_len, blk, &blk_len);
        if (ret != TMS_DSPREAD_ERR_OK)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            if (retry < TMS_PATCH_BLK_RETRY) 
            {
                tms_dspread_net_disconnect(pctx);
                pctx = 0;
                retry++;
                continue;
            }
            break;
        }
        retry = 0;
        ret = tms_dspread_fwrite(fp, blk, blk_len);
        if (ret != blk_len)
        {
            TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
            ret = TMS_DSPREAD_ERR_PARAM;
            break;
        }
        else
        {
            ret = TMS_DSPREAD_ERR_OK;
        }
#if TMS_VFY_MD5
        asc_2_bcd(etag + 1, 32, vfy_md5);
#endif
        tms_dspread_progress((char* )filpath, cnt, cnt + blk_len - 1, total);
        cnt += blk_len;
    }
    tms_dspread_free(blk);
    tms_dspread_net_disconnect(pctx);
    tms_dspread_fclose(fp);
    if (ret != TMS_DSPREAD_ERR_OK)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        return ret;
    }
#if TMS_VFY_MD5
    ret = tms_dspread_fopen(&fp, filpath, "rb+");
    if (ret != 0)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        ret = TMS_DSPREAD_ERR_PARAM;
        return ret;
    }
    memset(md5, 0x00, sizeof(md5));
    ret = tms_dspread_fmd5(fp, md5);
    if (ret != 0)
    {
        TMS_PRINTF("%s,%d\r\n", __FUNCTION__, __LINE__);
        tms_dspread_fclose(fp);
        ret = TMS_DSPREAD_ERR_PARAM;
        return ret;
    }
    TMS_PRINTF("md5: \r\n");
    TMS_DUMP(md5, 16, 0);
    TMS_PRINTF("vfy_md5: \r\n");
    TMS_DUMP(vfy_md5, 16, 0);
    if(memcmp(md5, vfy_md5, 16) != 0)
    {
        ret = TMS_DSPREAD_ERR_PARAM;
        return ret;
    }
#endif
    return ret;
}

/*--------------------------------------
|   Function Name:
|       tms_dspread_getfilename_dl
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void tms_dspread_getfilename_dl(char* filename, int filenamemaxLen)
{
    int len = filenamemaxLen>sizeof(g_fname_)?sizeof(g_fname_):filenamemaxLen;
    strcpy(filename, g_fname_);
}