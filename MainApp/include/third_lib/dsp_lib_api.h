/*
********************************************************************************
*
*   File Name:
*       dsp_api.h
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _DSP_LIB_API_H
#define _DSP_LIB_API_H

#ifdef __cplusplus
extern "C"
{
#endif//__cplusplus

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/

#ifdef _DSP_LIB_API_C
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
enum
{
    SSL_VERIFY_START = 0,

    SSL_VERIFY_NONE = SSL_VERIFY_START,
    SSL_VERIFY_OPTIONAL,
    SSL_VERIFY_REQUIRED,
    SSL_VERIFY_UNSET,

    SSL_VERIFY_NUMS
};



/*-----------------------------------------------------------------------------
|   Typedefs
+----------------------------------------------------------------------------*/
typedef struct md5_context
{
    unsigned int private_total[2];          /*!< number of bytes processed  */
    unsigned int private_state[4];          /*!< intermediate digest state  */
    unsigned char private_buffer[64];   /*!< data block being processed */
}md5_context;

/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   prototypes
+----------------------------------------------------------------------------*/
/**TLS**/
GLOBAL int dsp_tls_init(void);
GLOBAL int dsp_tls_cacert(const unsigned char *pdata, int len);
GLOBAL int dsp_tls_clikey(const unsigned char* pdata, int len);
GLOBAL int dsp_tls_clicert(const unsigned char* pdata, int len);
GLOBAL int dsp_tls_vfymode(int fd, int mode);
GLOBAL int dsp_tls_timeout(int fd, int timeout);
GLOBAL int dsp_tls_hostname(int fd, const char *phostname);
GLOBAL int dsp_tls_socket(void);
GLOBAL int dsp_tls_connect(int fd, char* host_or_ip, char* port);
GLOBAL int dsp_tls_send(int fd, const void* buf, int len, int flags);
GLOBAL int dsp_tls_recv(int fd, void* buf, int len, int flags);
GLOBAL int dsp_tls_socketclose(int fd);

GLOBAL int dsp_md5_starts(md5_context *ctx);
GLOBAL int dsp_md5_update(md5_context *ctx, const unsigned char *input, int ilen);
GLOBAL int dsp_md5_finish( md5_context *ctx, unsigned char output[16] );

/*
*******************************************************************************
*   End of File
*******************************************************************************
*/

#undef GLOBAL

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif//__cplusplus

#endif  /* #ifndef _DSPAPI_TLS_H */
