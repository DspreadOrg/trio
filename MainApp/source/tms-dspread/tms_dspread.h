/*
********************************************************************************
*
*   File Name:
*       tms_dspread.h
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _TMS_DSPREAD_H
#define _TMS_DSPREAD_H

#ifdef __cplusplus
extern "C"
{
#endif//__cplusplus

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/

#ifdef _TMS_DSPREAD_C
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
enum TMS_DSPREAD_ERR {
    TMS_DSPREAD_ERR_OK        =  0,
    TMS_DSPREAD_ERR_PARAM     = -1,
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
GLOBAL int tms_dspread_dl(const char* id, const char* custcode, const char* heartbeatpath, const char* dir);
GLOBAL int tms_http_dl(const char* url, const char* filpath);

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

#endif  /* #ifndef _TMS_DSPREAD_H */
