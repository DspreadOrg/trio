/*
********************************************************************************
*
*   File Name:
*       lring.c
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#define _LRING_MODULE
#define _LRING_C

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/
#include "lring.h"



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
|   Functions
+----------------------------------------------------------------------------*/
/*--------------------------------------
|   Function Name:
|       lring_item_copy
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
static void lring_item_copy(pring_t pring, unsigned char* source, unsigned char* dest)
{
    for (int i = 0; i < pring->width; i++)
    {
        dest[i] = source[i];
    }
}

/*--------------------------------------
|	Function Name:
|		lring_item_included
|	Description:
|	Parameters:
|	Returns:
+-------------------------------------*/
static int lring_item_included(pring_t pring, const void* pitem)
{
    if (pitem == 0)
    {
        return 0;
    }
    if((unsigned int)pitem < (unsigned int)pring->buffer \
        || (unsigned int)pitem > (unsigned int)pring->buffer + (unsigned int)pring->width * (pring->numbers - 1) \
    )
    {
        return 0;
    }
    if((unsigned int)((unsigned int)pitem - (unsigned int)pring->buffer) % pring->width)
    {
        return 0;
    }
    return 1;
}

/*--------------------------------------
|	Function Name:
|		lring_counter_included
|	Description:
|	Parameters:
|	Returns:
+-------------------------------------*/
static int lring_counter_included(pring_t pring, unsigned int uiCounter)
{
    if(pring->front < pring->rear)
    {
        if(     (uiCounter % pring->numbers) < pring->front \
            ||  (uiCounter % pring->numbers) > pring->rear \
        )
        {
            return 0;
        }
    }
    else
    {
        if(     (uiCounter % pring->numbers) > pring->rear \
            &&  (uiCounter % pring->numbers) < pring->front \
        )
        {
            return 0;
        }
    }
    return 1;
}

/*--------------------------------------
|   Function Name:
|       lring_init
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int lring_init(pring_t pring)
{
    if(     pring->buffer == 0 \
        ||  pring->width == 0 \
        ||  pring->numbers == 0 \
        )
    {
        return 0;
    }
	pring->front = 0;
	pring->rear = 0;
	pring->traversal = 0;
    return 1;
}

/*--------------------------------------
|   Function Name:
|       lring_put
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int lring_put(pring_t pring, const void* pitem)
{
	unsigned char* source = 0;
	unsigned char* dest = 0;

	if(!lring_is_full(pring))
	{
		source = (unsigned char*)pitem;
		dest = (unsigned char*)pring->buffer + pring->width*pring->rear;
		lring_item_copy(pring, source, dest);
		pring->rear = (pring->rear + 1) % pring->numbers;
		return 1;
	}
	else
	{
		return 0;
	}
}

/*--------------------------------------
|   Function Name:
|       lring_get
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void* lring_get(pring_t pring, void* pitem)
{
    unsigned char* source = 0;
    unsigned char* dest = 0;

    if(pitem == 0)
    {
        return 0;
    }

    if(!lring_is_empty(pring))
    {
        source = (unsigned char*)pring->buffer + pring->width*pring->front;
        dest = pitem;
        lring_item_copy(pring, source, dest);
        pring->front = (pring->front + 1) % pring->numbers;
        return pitem;
    }
    else
    {
        return 0;
    }
}

/*--------------------------------------
|   Function Name:
|       lring_is_full
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int lring_is_full(pring_t pring)
{
    if(     pring->buffer == 0 \
        ||  pring->width == 0 \
        ||  pring->numbers == 0 \
        )
    {
        return 0;
    }

    if((pring->rear + 1) % pring->numbers == pring->front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*--------------------------------------
|   Function Name:
|       lring_is_empty
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int lring_is_empty(pring_t pring)
{
    if(     pring->buffer == 0 \
        ||  pring->width == 0 \
        ||  pring->numbers == 0 \
        )
    {
        return 0;
    }

    if(pring->front == pring->rear)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*--------------------------------------
|   Function Name:
|       lring_first
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void* lring_first(pring_t pring)
{
    unsigned char* source = 0;

    if(!lring_is_empty(pring))
    {
        pring->traversal = pring->front;
        source = (unsigned char*)pring->buffer + pring->width*pring->traversal;
        pring->traversal = (pring->traversal + 1) % pring->numbers;
        return source;
    }
    else
    {
        return 0;
    }
}

/*--------------------------------------
|   Function Name:
|       lring_next
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
void* lring_next(pring_t pring)
{
    unsigned char* source = 0;

    if(!lring_is_empty(pring) && pring->traversal != pring->rear)
    {
        source = (unsigned char*)pring->buffer + pring->width*pring->traversal;
        pring->traversal = (pring->traversal + 1) % pring->numbers;
        return source;
    }
    else
    {
        return 0;
    }
}

/*--------------------------------------
|   Function Name:
|       lring_delete
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int lring_delete(pring_t pring, const void* pitem)
{
	unsigned char* source = 0;
	unsigned char* dest = 0;
	unsigned int counter = 0;

	if(lring_is_empty(pring))
	{
		return 0;
	}
	if(!lring_item_included(pring, pitem))
	{
		return 0;
	}
	counter = (unsigned int)((unsigned int)pitem - (unsigned int)pring->buffer)/pring->width;
	if(!lring_counter_included(pring, counter))
	{
		return 0;
	}
	while(counter != pring->rear)
	{
		source = (unsigned char*)pring->buffer + pring->width*((counter + 1) % pring->numbers);
		dest = (unsigned char*)pring->buffer + pring->width * (counter);
		lring_item_copy(pring, source, dest);
		counter = (counter + 1) % pring->numbers;
	}
	pring->rear = (pring->rear - 1) % pring->numbers;
	pring->traversal = (pring->traversal - 1) % pring->numbers;
	return 1;
}

/*--------------------------------------
|   Function Name:
|       lring_insert_last
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int lring_insert_last(pring_t pring, const void* pbase, const void* pitem)
{
	unsigned char* source = 0;
	unsigned char* dest = 0;
	unsigned int counter = 0;
	unsigned int backcounter = 0;

	if(lring_is_empty(pring))
	{
		return 0;
	}
	if(!lring_item_included(pring, pbase))
	{
		return 0;
	}
	counter = (unsigned int)((unsigned int)pbase - (unsigned int)pring->buffer)/pring->width;
	if(!lring_counter_included(pring, counter))
	{
		return 0;
	}
	if(lring_put(pring, pitem))
	{
		return 0;
	}
	backcounter = pring->rear;
	while(counter != backcounter)
	{
		source = (unsigned char*)pring->buffer + pring->width * ((backcounter - 1) % pring->numbers);
		dest = (unsigned char*)pring->buffer + pring->width*(backcounter);
		lring_item_copy(pring, source, dest);
		backcounter = (backcounter - 1) % pring->numbers;
	}
	source = (unsigned char*)pitem;
	dest = (unsigned char*)pring->buffer + pring->width*(backcounter);
	lring_item_copy(pring, source, dest);
	return 1;
}

/*--------------------------------------
|   Function Name:
|       lring_insert_next
|   Description:
|   Parameters:
|   Returns:
+-------------------------------------*/
int lring_insert_next(pring_t pring, const void* pbase, const void* pitem)
{
	unsigned int counter = 0;

	if(lring_is_empty(pring))
	{
		return 0;
	}
	if(!lring_item_included(pring, pbase))
	{
		return 0;
	}
	counter = (unsigned int)((unsigned int)pbase - (unsigned int)pring->buffer) / pring->width;
	counter = (counter + 1) % pring->numbers;
	return lring_insert_last(pring, (unsigned char*)pring->buffer + counter*pring->width, pitem);
}
