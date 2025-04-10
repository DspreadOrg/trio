/*
********************************************************************************
*
*   File Name:
*       lring.h
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#ifndef _LRING_H
#define _LRING_H

#ifdef __cplusplus
extern "C"
{
#endif//__cplusplus

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/

#ifdef _LRING_C
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
typedef struct _ring_t
{
	void*        buffer;
	unsigned int width;
	unsigned int numbers;
	unsigned int front;
	unsigned int rear;
	unsigned int traversal;
}ring_t, *pring_t;

typedef const ring_t     *pcring_t;



/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   prototypes
+----------------------------------------------------------------------------*/
GLOBAL int lring_init(pring_t pring);
GLOBAL int lring_put(pring_t pring, const void* pitem);
GLOBAL void* lring_get(pring_t pring, void* pitem);
GLOBAL int lring_is_full(pring_t pring);
GLOBAL int lring_is_empty(pring_t pring);
GLOBAL void* lring_first(pring_t pring);
GLOBAL void* lring_next(pring_t pring);
GLOBAL int lring_delete(pring_t pring, const void* pitem);
GLOBAL int lring_insert_last(pring_t pring, const void* pbase, const void* pitem);
GLOBAL int lring_insert_next(pring_t pring, const void* pbase, const void* pitem);

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

#endif  /* #ifndef _UTIL_LRING_H */
