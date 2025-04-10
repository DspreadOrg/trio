/*
********************************************************************************
*
*   File Name:
*       tms_dspread_platform.h
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _TMS_DSPREAD_PLATFORM_H
#define _TMS_DSPREAD_PLATFORM_H

#ifdef __cplusplus
extern "C"
{
#endif//__cplusplus

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/

#ifdef _TMS_DSPREAD_PLATFORM_C
#define GLOBAL
#else
#define GLOBAL  extern
#endif

/*-----------------------------------------------------------------------------
|   Macros
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Enumerations
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Typedefs
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   prototypes
+----------------------------------------------------------------------------*/
GLOBAL void* tms_dspread_alloc(int size);
GLOBAL void tms_dspread_free(void* addr);
GLOBAL void* tms_dspread_realloc(void* addr, int size);
GLOBAL void tms_dspread_clearline(int line, uint color);
GLOBAL void tms_dspread_display(int line, const char *text);
GLOBAL int tms_dspread_net_connect(void** pctx, const char* host, const char* port);
GLOBAL int tms_dspread_net_disconnect(void* pctx);
GLOBAL int tms_dspread_net_send(void* pctx, const unsigned char *buf, int len);
GLOBAL int tms_dspread_net_recv(void* pctx, unsigned char *buf, int len, int timeout_ms);

GLOBAL int tms_dspread_fremove(const char* path);
GLOBAL int tms_dspread_fopen(void** fp, const char* path, const char* mode);
GLOBAL int tms_dspread_fclose(void* fp);
GLOBAL int tms_dspread_fwrite(void* fp, const unsigned char* buf, int len);
GLOBAL int tms_dspread_fread(void* fp, unsigned char* buf, int len);
GLOBAL int tms_dspread_fmd5(void* fp, unsigned char* buf);
GLOBAL int tms_dspread_fcrc32(void* fp, unsigned int* crc);

GLOBAL void tms_dspread_progress(char* name, unsigned int range_bgn, unsigned int range_end, unsigned int total);
GLOBAL int tms_dspread_progress_cancel(void);
/*
*******************************************************************************
*   End of File
*******************************************************************************
*/

#undef GLOBAL

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/
#include "chttp.h"
#include "cJSON.h"

#ifdef __cplusplus
}
#endif//__cplusplus

#endif  /* #ifndef _TMS_DSPREAD_PLATFORM_H */
