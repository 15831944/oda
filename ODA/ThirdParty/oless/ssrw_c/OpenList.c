/************************************************************************
* OpenList.c
*
* Contains list of open streams and storages
*
* (c) Schema Software Inc., 2001
* 
*************************************************************************

$Revision: 1.6.2.1 $
$Date: 2004/02/24 19:09:25 $
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "StructuredStorage.h"
#include "OpenList.h"

struct _OpenItem
{
    StgDirType type;
    union
    {
        struct _Stream* pStream;
        struct _Storage* pStorage;
    } data;
    struct _OpenItem *pNext;
};

struct _OpenList
{
    OpenItem* pHead;
    UINT4 nItems;
};

/*============================================================================
  Description: Creates a new list of open streams and storages
  Arguments: 
   out_ppList - pointer to variable that will point to new list structure
                on success
  Return: 
   status code
  ==========================================================================*/
SINT4
openListConstruct(OpenList** out_ppList)
{
    OpenList* pNewList = NULL;

    ASSERT(out_ppList != NULL);
    if (out_ppList == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    pNewList = (OpenList*)malloc(sizeof(OpenList));
    if (pNewList == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( pNewList, 0, sizeof(OpenList) );


    pNewList->nItems = 0;
    pNewList->pHead = NULL;
    *out_ppList = pNewList;

    return SSTG_OK;
}


/*============================================================================
  Description: Destroys a list of open storages and streams
  Arguments: 
   out_ppList - pointer to variable that points to existing list
  Return: 
   status code
  ==========================================================================*/
SINT4
openListDestroy(OpenList** out_ppList)
{
    OpenItem* pCurItem = NULL;
    OpenItem* pLastItem = NULL;

    ASSERT(out_ppList != NULL && *out_ppList != NULL);
    if (out_ppList == NULL || *out_ppList == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    pCurItem = ((*out_ppList)->pHead);
    while (pCurItem != NULL)
    {
        pLastItem = pCurItem;
        pCurItem = pCurItem->pNext;
        free (pLastItem);
    }

    free(*out_ppList);
    *out_ppList = NULL;

    return SSTG_OK;
}


/*============================================================================
  Description: Appends a new item to a list of open storages and streams
  Arguments: 
   io_pList - pointer to existing list structure
   in_type - type of new item
   in_ptr - pointer to storage or stream structure to add
  Return: 
   status code
  ==========================================================================*/
SINT4
openListAppend(OpenList* io_pList, StgDirType in_type, void* in_ptr)
{
    OpenItem** ppCurItem = NULL;
    OpenItem* pNewItem = NULL;

    ASSERT(io_pList != NULL);
    if (io_pList == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    pNewItem = (OpenItem*)malloc(sizeof(OpenItem));
    if (pNewItem == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(pNewItem, 0, sizeof(OpenItem));


    pNewItem->type = in_type;
    if (pNewItem->type == STREAM)
    {
        pNewItem->data.pStream = (Stream*)in_ptr;
    }
    else
    {
        pNewItem->data.pStorage = (Storage*)in_ptr;
    }
    pNewItem->pNext = NULL;

    ppCurItem = &(io_pList->pHead);
    while(*ppCurItem != NULL)
    {
        ppCurItem = &((*ppCurItem)->pNext);
    }

    *ppCurItem = pNewItem;
    io_pList->nItems++;

    return SSTG_OK;
}


/*============================================================================
  Description: Removes an existing item from the list
  Arguments: 
   io_pList - pointer to existing list structure
   in_ulIndex - index of item to remove
  Return: 
   status code
  ==========================================================================*/
SINT4
openListRemoveItem(OpenList* io_pList, UINT4 in_ulIndex)
{
    OpenItem** ppCurItem = NULL;
    OpenItem* pTmp = NULL;
    UINT4 i = 0;

    ASSERT(in_ulIndex < io_pList->nItems);
    if (in_ulIndex >= io_pList->nItems)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    ppCurItem = &(io_pList->pHead);
    for (i = 0; i < in_ulIndex; i++)
    {
        ppCurItem = &((*ppCurItem)->pNext);
        ASSERT(*ppCurItem != NULL);
    }
    pTmp = *ppCurItem;
    *ppCurItem = (*ppCurItem)->pNext;
    free(pTmp);
    io_pList->nItems--;

    return SSTG_OK;
}


/*============================================================================
  Description: Retrieves an item from the list
  Arguments: 
   io_pList - pointer to existing list structure
   in_ulIndex - index of item to retrieve
   out_pType - type of item on success
   out_ptr - will point to open stream or storage structure on success
  Return: 
   status code
  ==========================================================================*/
SINT4
openListGetItem(OpenList* in_pList,
                UINT4 ulIndex,
                StgDirType* out_pType,
                void** out_ptr)
{
    OpenItem* pCurItem = NULL;
    UINT4 i = 0;

    ASSERT(ulIndex < in_pList->nItems);
    if (ulIndex >= in_pList->nItems)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    pCurItem = in_pList->pHead;
    for (i = 0; i < ulIndex; i++)
    {
        pCurItem = pCurItem->pNext;
        ASSERT(pCurItem != NULL);
    }

    *out_pType = pCurItem->type;
    if (pCurItem->type == STREAM)
    {
        *out_ptr = pCurItem->data.pStream;
    }
    else
    {
        *out_ptr = pCurItem->data.pStorage;
    }

    return SSTG_OK;
}


/*============================================================================
  Description: Gets the number of items already in the list
  Arguments: 
   in_pList - pointer to existing list structure
  Return: 
   status code
  ==========================================================================*/
SINT4
openListItemCount(OpenList* in_pList)
{
    return in_pList->nItems;
}


/*============================================================================
  Description: Returns the index of a piece of data in the list
  Arguments: 
   in_pList - pointer to existing list structure
   out_pulIndex - on success, will contain the index of the item
   in_type - type of the item to search for
   in_ptr - pointer to item to search for
  Return: 
   status code
  ==========================================================================*/
SINT4
openListFindItem(OpenList* in_pList,
                 UINT4* out_pulIndex,
                 StgDirType in_type,
                 void* in_ptr)
{
    UINT4 i = 0;
    OpenItem* pCur = in_pList->pHead;

    for (i = 0; pCur != NULL; i++)
    {
        if (pCur->type == in_type &&
                in_type == STREAM &&
                in_ptr == pCur->data.pStream)
        {
            *out_pulIndex = i;
            return SSTG_OK;
        }
        else if (pCur->type == in_type &&
                 in_type == STORAGE &&
                 in_ptr == pCur->data.pStorage)
        {
            *out_pulIndex = i;
            return SSTG_OK;
        }
        pCur = pCur->pNext;
    }

    return SSTG_ERROR_CHILD_NOT_FOUND;
}


#ifdef DEBUG

/*============================================================================
  Description: Self-test of the open list ADT.  Should only be compiled into
               the DEBUG build.
  Return: 
   Status code
  ==========================================================================*/
SINT4
openListSelfTest(void)
{
    OpenList* pList = NULL;
    SINT4 nItems = -1;
    SINT4 iRet = SSTG_OK;
    StgDirType type = STREAM;
    void* ptr = NULL;
    SINT4 i = 0;
    UINT4 index = 0;

    iRet = openListConstruct(&pList);
    ASSERT(iRet == SSTG_OK && pList != NULL);

    for (i = 0; i < 20; i++)
    {
        iRet = openListAppend(pList, (i%2)?STREAM:STORAGE, (void*)i);
        ASSERT (iRet == SSTG_OK);
        nItems = openListItemCount(pList);
        ASSERT (nItems == i + 1);
    }

    for (i = 0; i < 20; i++)
    {
        iRet = openListGetItem(pList, i, &type, &ptr);
        ASSERT(iRet == SSTG_OK);
        ASSERT(type == (i%2)?STREAM:STORAGE);
        ASSERT((int)ptr == i);
    }
    iRet = openListRemoveItem(pList, 0);
    ASSERT(iRet == SSTG_OK);

    iRet = openListGetItem(pList, 0, &type, &ptr);
    ASSERT(iRet == SSTG_OK && type == STREAM && (((int)ptr) == 1));
    iRet = openListFindItem(pList, &index, STREAM, (void*)1);
    ASSERT(iRet == SSTG_OK && index == 0);

    iRet = openListGetItem(pList, 18, &type, &ptr);
    ASSERT(iRet == SSTG_OK && type == STREAM && (((int)ptr) == 19));
    iRet = openListFindItem(pList, &index, STREAM, (void*)19);
    ASSERT(iRet == SSTG_OK && index == 18);

    iRet = openListRemoveItem(pList, 2);
    ASSERT(iRet == SSTG_OK);

    iRet = openListGetItem(pList, 1, &type, &ptr);
    ASSERT(iRet == SSTG_OK && type == STORAGE && (((int)ptr) == 2));
    iRet = openListFindItem(pList, &index, STORAGE, (void*)2);
    ASSERT(iRet == SSTG_OK && index == 1);

    iRet = openListGetItem(pList, 2, &type, &ptr);
    ASSERT(iRet == SSTG_OK && type == STORAGE && (((int)ptr) == 4));
    iRet = openListFindItem(pList, &index, STORAGE, (void*)4);
    ASSERT(iRet == SSTG_OK && index == 2);

    iRet = openListDestroy(&pList);
    ASSERT(iRet == SSTG_OK && pList == NULL);

    return SSTG_OK;
}

#endif /* DEBUG */

