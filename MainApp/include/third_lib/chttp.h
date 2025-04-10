/*
********************************************************************************
*
*   File Name:
*       chttp.h
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _CHTTP_H
#define _CHTTP_H

#ifdef __cplusplus
extern "C"
{
#endif//__cplusplus

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/

#ifdef _CHTTP_C
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
enum CHTTP_ERR {
    CHTTP_ERR_OK        =  0,
    CHTTP_ERR_PARAM     = -1,
    CHTTP_ERR_ALLOC     = -2,
    CHTTP_ERR_NOT_FOUND = -3,
    CHTTP_ERR_FAIL      = -4,
};



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
GLOBAL int chttp_init_hooks(void* (*alloc)(int ), void (*free)(void*), void* (*realloc)(void*, int));
GLOBAL int chttp_new(void** ppctl);
GLOBAL int chttp_del(void* pctl);
GLOBAL int chttp_line_set(void* pctl, const char* pval, int len);
GLOBAL int chttp_line_get(void* pctl, char** ppval, int* plen);
GLOBAL int chttp_header_clr(void* pctl);
GLOBAL int chttp_header_add(void* pctl, const char* pval, int len);
GLOBAL int chttp_header_first(void* pctl, char** ppval, int* plen);
GLOBAL int chttp_header_next(void* pctl, char** ppval, int* plen);
GLOBAL int chttp_header_find(void* pctl, const char* pkey, char** ppval, int* plen);
GLOBAL int chttp_body_set(void* pctl, const char* pval, int len);
GLOBAL int chttp_body_append(void* pctl, const char* pval, int len);
GLOBAL int chttp_body_get(void* pctl, char** ppval, int* plen);
GLOBAL int chttp_parse(void* pctl, const char* pval, int len);
GLOBAL int chttp_len(void* pctl, int* plen);
GLOBAL int chttp_format(void* pctl, char** ppval, int* plen);

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

#endif  /* #ifndef _CHTTP_H */
