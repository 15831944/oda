/************************************************************************
* Storage.c
*
* Contains code for handling Storage structures
*
* (c) Schema Software Inc., 2001-2004
*
*************************************************************************

$Revision: 1.12.2.4 $
$Date: 2004/02/24 19:09:26 $
*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "StructuredStorage.h"
#include "RootStorage.h"
#include "Directory.h"
#include "Storage.h"
#include "Stream.h"
#include "OpenList.h"
#include "TOC.h"
#include "Fat.h"
#include "Header.h"

#define CHILD_BLOCKSIZE 8

#define ROOT_STORAGE_NAME L"Root Entry"

struct _Storage
{
    RootStorage* pRoot;            /* reference to root storage */
    SINT4 ulDirIndex;              /* Index of the directory entry corresponding
                                    * to this storage in the TOC */
    Storage* pParent;              /* pointer to parent storage */
    OpenList* pOpenList;           /* list of open child streams and storages */
    StgMode mode;                  /* mode in which this storage was opened */
};

static SINT4 storageConstructFromDirEntry (DirectoryEntry* in_pEntry, 
                                           Storage** out_storage);
static SINT4 storageConstruct(Storage** out_storage);

static SINT4 openStreamInternal (Storage* in_pStorage,
                                 const wchar_t* in_wszName,
                                 Stream** out_ppChild);


/*============================================================================
  Description:  Constructs a storage from a specified directory entry
  Note:
   The caller should set the pRoot and pParent fields of the new storage.
  Arguments:    
   in_pEntry -     the directory entry
   out_ppStorage - points to a pointer that will, on success, point to the 
                   storage created
  Return:       
   Status code
  ==========================================================================*/
static SINT4
storageConstructFromDirEntry (DirectoryEntry* in_pEntry,
                              Storage** out_ppStorage)
{
    Storage*    pStorage =  NULL;
    SINT4       iRet =      SSTG_OK;
    DirEntry    dirType =   DIR_ROOT;

    dirType = directoryGetType (in_pEntry);
    if (dirType != DIR_STORAGE && dirType != DIR_ROOT)
    {
        return SSTG_ERROR_NOT_STORAGE;
    }

    iRet = storageConstruct(&pStorage);
    if (iRet != SSTG_OK || pStorage == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }


    pStorage->ulDirIndex = directoryGetIndex(in_pEntry);

    *out_ppStorage = (Storage*)pStorage;

    return iRet;
}


/*============================================================================
  Description:  Constructs a storage objects and inits it
  Arguments:    
   out_ppStorage - points to a pointer that will, on success, point to the 
                   storage created
  Return:       
   Status code
  ==========================================================================*/
static SINT4
storageConstruct (Storage** out_ppStorage)
{
    SINT4       iRet =      SSTG_OK;
    Storage*    pStorage =  NULL;

    pStorage = (Storage*) malloc (sizeof(Storage));
    if (pStorage == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(pStorage, 0, sizeof(Storage));


    pStorage->pRoot = NULL;
    pStorage->ulDirIndex = -1;
    pStorage->pParent = NULL;

    /* Construct the list of open children of this storage */
    iRet = openListConstruct(&(pStorage->pOpenList));
    if (iRet != SSTG_OK)
    {
        free (pStorage);
        return iRet;
    }

    *out_ppStorage = (Storage*)pStorage;

    return iRet;
}


/*============================================================================
  Description: Constructs the storage representing the root storage of the 
               whole structured storage
  Arguments: 
   in_pRoot -      pointer to an initialized root storage object
   out_ppStorage - points to a pointer that will, on success, point to the 
                   newly created root storage object.
  Return:       
   Status code
  ==========================================================================*/
SINT4
storageConstructRoot (RootStorage* in_pRoot, Storage** out_ppStorage)
{
    DirectoryEntry* pDirRoot =  NULL;
    SINT4           iRet =      SSTG_OK;
    TOC*            pTOC =      NULL;

    pTOC = rootStorageGetTOC(in_pRoot);
    pDirRoot = tocGetEntryAtIndex(pTOC, 0);
    if (pDirRoot == NULL)
    {
        return SSTG_ERROR_CHILD_NOT_FOUND;
    }

    _TCHECK;

    iRet = storageConstructFromDirEntry (pDirRoot, out_ppStorage);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    (*out_ppStorage)->pParent = NULL;
    (*out_ppStorage)->ulDirIndex = 0;
    (*out_ppStorage)->pRoot = in_pRoot;
    (*out_ppStorage)->mode = rootStorageGetMode(in_pRoot);

    return iRet;
}


/*============================================================================
  Description:  Destructor, destroys the passed storage
  Arguments: 
   io_ppStorage - pointer to pointer to storage to destroy
  Return:       
   A status code.  SSTG_OK is successful
  ==========================================================================*/
SINT4
storageDestroy (Storage** in_ppStorage)
{
    SINT4 iRet = SSTG_OK;

    /* Destroy the (by now empty) list of open children of this storage */
    iRet = openListDestroy (&((*in_ppStorage)->pOpenList));

    free (*in_ppStorage);
    *in_ppStorage = NULL;

    return iRet;
}


/*============================================================================
  Description:  (API Call) Opens a substorage of a given storage.  This call
                is only legal if the storage being read from is open in read-
                only mode.
  Arguments: 
   in_pStorage - pointer to initialized Storage structure representing 
                 parent storage of storage to be opened
   in_wszName -  name of substorage to open
   out_ppChild - points to pointer that will, on success, point to freshly 
                 created child storage
  Return: 
   Status code
  ==========================================================================*/
int
openStorage (Storage* in_pStorage,
             const wchar_t* in_wszName,
             Storage** out_ppChild)
{
    DirectoryEntry* pChildEntry =   NULL;
    DirectoryEntry* pParentEntry =  NULL;
    SINT4           iRet =          SSTG_OK;
    TOC*            pTOC =          NULL;

    ASSERT(in_pStorage != NULL &&
           in_pStorage->pRoot != NULL &&
           in_wszName != NULL &&
           *in_wszName != 0 &&
           out_ppChild != NULL &&
           STG_CANREAD(in_pStorage->mode));
    if (in_pStorage == NULL ||
            in_pStorage->pRoot == NULL ||
            in_wszName == NULL ||
            *in_wszName == 0 ||
            out_ppChild == NULL ||
            (!STG_CANREAD(in_pStorage->mode)))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    pTOC = rootStorageGetTOC(in_pStorage->pRoot);
    pParentEntry = tocGetEntryAtIndex(pTOC, in_pStorage->ulDirIndex);
    if (pParentEntry == NULL)
    {
        return (int)SSTG_ERROR_CHILD_NOT_FOUND;
    }

    iRet = tocFindChildEntry (pTOC, pParentEntry, in_wszName, &pChildEntry);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    _TCHECK;

    iRet = storageConstructFromDirEntry (pChildEntry, out_ppChild);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    (*out_ppChild)->pParent = in_pStorage;
    (*out_ppChild)->pRoot = in_pStorage->pRoot;
    (*out_ppChild)->mode = in_pStorage->mode;

    iRet = openListAppend (in_pStorage->pOpenList, STORAGE, *out_ppChild);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    return (int)iRet;
}



/*============================================================================
  Description:  (API call) Closes a given open storage.
  Arguments: 
   in_ppStorage - pointer to pointer to storage structure representing storage 
                  to close.  Will point to NULL on success.
  Return: 
   Status code
  ==========================================================================*/
int
closeStorage (Storage** io_ppStorage)
{
    /* Here we just want to return an error if the client attempts to close the 
     * root storage.  An assert is not justified because some clients do not
     * track which storage is the root; it should be allowed to attempt to 
     * close any storage without the possibility of causing catastrophic 
     * failure. */
    if ((*io_ppStorage)->pParent == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    return (int)closeStorageInternal(io_ppStorage);
}


/*============================================================================
  Description:  Closes a given open storage.
  Arguments: 
   in_ppStorage - pointer to pointer to storage structure representing storage 
                  to close.  Will point to NULL on success.
  Return: 
   Status code
  ==========================================================================*/
SINT4
closeStorageInternal (Storage** io_ppStorage)
{
    SINT4           iRet =          SSTG_OK;
    UINT4           i =             0;
    StgDirType      childType =     STREAM;
    void*           pChild =        NULL;
    Stream*         pChildStream =  NULL;
    Storage*        pChildStorage = NULL;
    DirectoryEntry* pDirEntry =     NULL;

    ASSERT (io_ppStorage != NULL);
    ASSERT (*io_ppStorage != NULL);
    if ((io_ppStorage == NULL) || 
        (*io_ppStorage == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    _KABOOM2;

    /* Close all children of this storage */
    for (i = openListItemCount((*io_ppStorage)->pOpenList) - 1; i != (UINT4)-1; i--)
    {
        iRet = openListGetItem ((*io_ppStorage)->pOpenList, i, &childType, &pChild);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        if (childType == STREAM)
        {
            pChildStream = (Stream*)pChild;
            iRet = closeStream(&pChildStream);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
        else if (childType == STORAGE)
        {
            pChildStorage = (Storage*)pChild;
            iRet = closeStorageInternal(&pChildStorage);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
        /* When the children are closed, the remove themselves from
         * the open list. */
    }

    /* Set the "last modification" field of the directory entry for this
     * storage */
    if (STG_CANWRITE((*io_ppStorage)->mode))
    {
        pDirEntry = tocGetEntryAtIndex(rootStorageGetTOC((*io_ppStorage)->pRoot),
                                       (*io_ppStorage)->ulDirIndex);
        if (pDirEntry == NULL)
        {
            return SSTG_ERROR_ILLEGAL_CALL;
        }

        /* TODO see if this should be set in the case of STG_RW */
        iRet = directorySetCurrentModificationTime(pDirEntry);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    /* Remove storage from its parent's open storage list */
    if ((*io_ppStorage)->pParent != NULL)
    {
        iRet = storageRemoveOpenChild ((*io_ppStorage)->pParent,
                                       STORAGE,
                                       *io_ppStorage);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    return (int)storageDestroy (io_ppStorage);
}


/*============================================================================
  Description:  (API call) Opens a given stream in a given storage for reading.
                Only valid if the parent storage is open in STG_READ or STG_RW 
                mode.
  Arguments: 
   in_pStorage - parent of stream to open
   in_wszName -  name of stream to open
   out_ppChild - on success, will point to a newly created Stream structure
  Return: 
   Status code
  ==========================================================================*/
int
openStream (Storage* in_pStorage,
            const wchar_t* in_wszName,
            Stream** out_ppChild)
{
    ASSERT(in_pStorage != NULL);
    ASSERT(in_pStorage->pRoot != NULL);
    ASSERT(in_wszName != NULL);
    ASSERT(*in_wszName != 0);
    ASSERT(out_ppChild != NULL);
    ASSERT(STG_CANREAD(in_pStorage->mode));
    if ((in_pStorage == NULL) ||
        (in_pStorage->pRoot == NULL) ||
        (in_wszName == NULL) ||
        (*in_wszName == 0) ||
        (out_ppChild == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }
    if (!STG_CANREAD(in_pStorage->mode))
    {
        return (int)SSTG_ERROR_ILLEGAL_ACCESS;
    }

    return (int)openStreamInternal(in_pStorage, in_wszName, out_ppChild);
}


/*============================================================================
  Description:  Opens a given stream in a given storage for reading.

                This is valid in all modes, although STG_WRITE is only allowed
                because it is necessary for streamResize().

  Arguments: 
   in_pStorage - parent of stream to open
   in_wszName -  name of stream to open
   out_ppChild - on success, will point to a newly created Stream structure
  Return: 
   Status code
  ==========================================================================*/
static SINT4
openStreamInternal (Storage* in_pStorage,
                    const wchar_t* in_wszName,
                    Stream** out_ppChild)
{
    DirectoryEntry* pChildEntry =   NULL;
    DirectoryEntry* pParentEntry =  NULL;
    SINT4           iRet =          SSTG_OK;
    TOC*            pTOC =          NULL;

    TCHECK1;

    ASSERT((in_pStorage != NULL) &&
           (in_pStorage->pRoot != NULL) &&
           (in_wszName != NULL) &&
           (*in_wszName != 0) &&
           (out_ppChild != NULL));
    if ((in_pStorage == NULL) ||
        (in_pStorage->pRoot == NULL) ||
        (in_wszName == NULL) ||
        (*in_wszName == 0) ||
        (out_ppChild == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    /* Get parent storage's directory entry */
    pTOC = rootStorageGetTOC(in_pStorage->pRoot);
    pParentEntry = tocGetEntryAtIndex(pTOC, in_pStorage->ulDirIndex);
    if (pParentEntry == NULL)
    {
        return SSTG_ERROR_CHILD_NOT_FOUND;
    }

    /* Find specified stream's directory entry */
    iRet = tocFindChildEntry (pTOC, pParentEntry, in_wszName, &pChildEntry);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* Construct the actual stream structure */
    iRet = streamConstruct (in_pStorage->pRoot,
                            in_pStorage,
                            pChildEntry,
                            in_pStorage->mode,
                            out_ppChild);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    KABOOM1;

    /* If the stream is writable and is a small stream, upgrade it immediately
     * to a full-size stream.  If it is still smaller than the cutoff when
     * it is closed, it will be demoted again.  This is to avoid having two
     * implementations of all the writing and rewriting code. */
    if (STG_CANWRITE(in_pStorage->mode))
    {
        /* This call returns success if the stream is already a regular stream 
         */
        iRet = streamUpgradeMiniToRegular(*out_ppChild);
        if (iRet != SSTG_OK)
        {
            streamDestroy(out_ppChild);
            return iRet;
        }
    }
    
    /* Add the newly opened stream to its parent storage's list of open
     * children */
    iRet = openListAppend (in_pStorage->pOpenList, STREAM, *out_ppChild);
    if (iRet != SSTG_OK)
    {
        streamDestroy(out_ppChild);
        return iRet;
    }

    return iRet;
}

//	===================================================================
//	Get number of children
//	===================================================================

int countChildren(Storage* in_pStorage, int* out_pCount)
{
	TOC*                pTOC =          NULL;
    DirectoryEntry*     pParentEntry =  NULL;
    UINT4               cChildren =     0;

	ASSERT (in_pStorage != NULL);
    ASSERT (out_pCount != NULL);
    if ((in_pStorage == NULL) ||
            (out_pCount == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }


    pTOC = rootStorageGetTOC(in_pStorage->pRoot);
    pParentEntry = tocGetEntryAtIndex(pTOC, in_pStorage->ulDirIndex);
    if (pParentEntry == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    cChildren = tocCountEntryChildren(pTOC, pParentEntry);

    /* Early exit if no children */
    if (cChildren == 0)
    {
        *out_pCount = 0;
        return SSTG_OK;
    }
	else
	{
		*out_pCount = cChildren;
		return SSTG_OK;
	}
}

/*============================================================================
  Description:  (API call)  Returns an array of StgInfo structures with info
                about each child of a given Storage.
  Arguments: 
   in_pStorage - pointer to storage for which to return child info
   out_paInfo -  pointer to variable that, on success, will point to an array
                 of StgInfo structures
   out_pCount -  pointer to variable that, on success, will contain the number
                 of entries in the out_paInfo array
  Return:        
   Status code
  ==========================================================================*/
int
getChildrenInfo(Storage* in_pStorage,
                StgInfo** out_paInfo,
                int* out_pCount)
{
    TOC*                pTOC =          NULL;
    DirectoryEntry*     pParentEntry =  NULL;
    UINT4               cChildren =     0;
    UINT4               i =             0;
    StgInfo*            pInfo =         NULL;

    TCHECK2;

    ASSERT (in_pStorage != NULL);
    ASSERT (out_paInfo != NULL);
    ASSERT (out_pCount != NULL);
    if ((in_pStorage == NULL) ||
            (out_paInfo == NULL) ||
            (out_pCount == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }


    pTOC = rootStorageGetTOC(in_pStorage->pRoot);
    pParentEntry = tocGetEntryAtIndex(pTOC, in_pStorage->ulDirIndex);
    if (pParentEntry == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    cChildren = tocCountEntryChildren(pTOC, pParentEntry);

    /* Early exit if no children */
    if (cChildren == 0)
    {
        *out_pCount = 0;
        return SSTG_OK;
    }

    /* Allocate an array of new StgInfos */
    pInfo = (StgInfo*)malloc(sizeof(StgInfo) * cChildren);
    if (pInfo == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }
    memset(pInfo, 0, sizeof(StgInfo) * cChildren);

    /* Fill in information for each entry */
    i = tocFillChildrenStgArray(pTOC, pParentEntry, pInfo);
    if(i != cChildren)
    {
        freeInfo(&pInfo, cChildren);
        return SSTG_ERROR_MEMORY;
    }

    KABOOM2;

    *out_paInfo = pInfo;
    *out_pCount = cChildren;

    return SSTG_OK;
}


/*============================================================================
  Description:  (API Call) Fills a structure with information about a storage
  Arguments: 
   in_pStream - pointer to storage from which to get information
   out_ppInfo - on success, will point to a newly allocated structure
                that contains information about the given storage.  This 
                structure should only be freed with freeInfo().
  Return:       Status code
  ==========================================================================*/
int
getStorageInfo(Storage* in_pStorage,
               StgInfo** out_ppInfo)
{
    SINT4           iRet =      SSTG_OK;
    DirectoryEntry* pEntry =    NULL;
    TOC*            pTOC =      NULL;

    ASSERT (in_pStorage != NULL);
    ASSERT (in_pStorage->pRoot != NULL);
    ASSERT (out_ppInfo != NULL);
    if ((in_pStorage == NULL) ||
            (in_pStorage->pRoot == NULL) ||
            (out_ppInfo == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    pTOC = rootStorageGetTOC(in_pStorage->pRoot);
    /* Get the directory entry corresponding to this storage */
    pEntry = tocGetEntryAtIndex(pTOC, in_pStorage->ulDirIndex);
    if (pEntry == NULL)
    {
        return SSTG_ERROR_CHILD_NOT_FOUND;
    }

    *out_ppInfo = (StgInfo*)malloc(sizeof(StgInfo));
    if (*out_ppInfo == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(*out_ppInfo, 0, sizeof(StgInfo));


    iRet =  directoryFillStgInfo (pEntry, *out_ppInfo);
    return iRet;
}


/*============================================================================
  Description:  Removes a child from the open list
  Arguments: 
   io_pStorage - pointer to storage from which to remove a child from
                 the open list
   in_type -     the type of child, STREAM or STORAGE
   in_pChild -   pointer to child
  Return:       
   Status code
  ==========================================================================*/
SINT4
storageRemoveOpenChild (Storage* io_pThis,
                        StgDirType in_type,
                        void* in_pChild)
{
    SINT4 iRet =    SSTG_OK;
    UINT4 index =   (UINT4)-1;

    iRet = openListFindItem(io_pThis->pOpenList, &index, in_type, in_pChild);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = openListRemoveItem(io_pThis->pOpenList, index);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    return iRet;
}


/*============================================================================
  Description:  (API Call) Creates a new stream in a given storage.  Only 
                allowed if the parent storage is open in STG_WRITE mode.
  Arguments:    
   in_pStorage - storage in which to create a new stream
   in_wszName -  name of new stream
   out_ppChild - pointer to pointer that will, on success, point to freshly 
                 created empty stream
  Return:       
   Status code
  ==========================================================================*/
int
createStream (Storage* in_pStorage,
              const wchar_t* in_wszName,
              Stream** out_ppChild)
{
    DirectoryEntry* pEntry =    NULL;
    SINT4           iRet =      SSTG_OK;
    TOC*            pTOC =      NULL;

    ASSERT(in_pStorage != NULL &&
           in_wszName != NULL &&
           *in_wszName != 0 &&
           out_ppChild != NULL &&
           STG_CANWRITE(in_pStorage->mode));
    if (in_pStorage == NULL ||
            in_wszName == NULL ||
            *in_wszName == 0 ||
            out_ppChild == NULL ||
            (!STG_CANWRITE(in_pStorage->mode)))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    *out_ppChild = NULL;
    /* Mark all metadata as being dirty so it will be rewritten later */
    iRet = rootStorageSetDirty(in_pStorage->pRoot);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    /* create a directory entry for the stream */
    pTOC = rootStorageGetTOC (in_pStorage->pRoot);

	iRet = tocAddAndInitEntry(pTOC,
							in_wszName,
                            DIR_STREAM,
                            in_pStorage->pRoot,
							&pEntry );
    if (iRet != SSTG_OK)
    {
		streamDestroy(out_ppChild);
        return iRet;
    }

    /* Make the new entry the child of its parent */
    iRet = tocInsertChild(pTOC,
                          tocGetEntryAtIndex (pTOC,
                                              in_pStorage->ulDirIndex),
                          pEntry);
    if (iRet != SSTG_OK)
    {
        tocInvalidateEntry (pTOC, directoryGetIndex(pEntry));
        return iRet;
    }

    /* Construct the stream structure itself */
    iRet = streamConstruct (in_pStorage->pRoot, 
                            in_pStorage, 
                            pEntry, 
                            in_pStorage->mode,
                            out_ppChild);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    _KABOOM3;

    /* If the stream is read/writable and is a small stream, upgrade it 
     * immediately to a full-size stream.  If it is still smaller than the 
     * cutoff when it is closed, it will be demoted again.  This is to avoid 
     * having two implementations of all the writing and rewriting code. 
     * 
     * There is a special high-performance case for write-only files, so we
     * don't have to upgrade the stream in that case. */
    if (STG_CANWRITE(in_pStorage->mode) && STG_CANREAD(in_pStorage->mode))
    {
        /* This call returns success if the stream is already a regular stream 
         */
        iRet = streamUpgradeMiniToRegular(*out_ppChild);
        if (iRet != SSTG_OK)
        {
            tocInvalidateEntry (pTOC, directoryGetIndex(pEntry));
            streamDestroy(out_ppChild);
            return iRet;
        }
    }
    
    /* Add this stream to its parent storage's list of open streams and
     * storages */
    iRet = openListAppend (in_pStorage->pOpenList, STREAM, *out_ppChild);
    if (iRet != SSTG_OK)
    {
        tocInvalidateEntry (pTOC, directoryGetIndex(pEntry));
        streamDestroy(out_ppChild);
        return iRet;
    }

    return iRet;
}


/*============================================================================
  Description: (API call) Sets the CLSID of the storage
  Arguments:   
   io_pDir - non-NULL pointer to a storage to modify
   in_pCLSID - pointer to the new CLSID for the storage.  Makes a copy.  Must 
               be non-NULL.
  Return:       
   Status code
  ==========================================================================*/
int
storageSetClass(Storage* io_pStorage,
                const SSRW_CLSID* in_pCLSID)
{
    TOC*            pTOC =      NULL;
    DirectoryEntry* pDirEntry = NULL;
    SINT4           iRet =      SSTG_OK;

    ASSERT (io_pStorage != NULL);
    ASSERT (STG_CANWRITE(io_pStorage->mode));
    if ((io_pStorage == NULL) ||
        (!STG_CANWRITE(io_pStorage->mode)))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = rootStorageSetDirty(io_pStorage->pRoot);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    pTOC = rootStorageGetTOC(io_pStorage->pRoot);
    pDirEntry = tocGetEntryAtIndex(pTOC, io_pStorage->ulDirIndex);
    return (int)directorySetClass(pDirEntry, in_pCLSID);
}


/*============================================================================
  Description: (API call) Removes a given storage or stream from the 
               structured storage file.
  Arguments:   
   io_pParent - storage containing the item to destroy
   in_pName -   name of item to destroy
  Return:       
   Status code
  ==========================================================================*/
int 
destroy( Storage* io_pParent, const wchar_t* in_pName)
{
    SINT4           iRet =         SSTG_OK;
    TOC*            pTOC =         NULL;
    DirectoryEntry* pParentEntry = NULL;
    DirectoryEntry* pChildEntry =  NULL;
    StgInfo*        pStgChildren = NULL;
    SINT4           childCount =   0;
    SINT4           i =            0;
    Storage*        pStorage =     0;
    Header*         pHeader =      NULL;
    Fat*            pRegFat =      NULL;
    Fat*            pSmallFat =    NULL;
    
    ASSERT(io_pParent != NULL);
    ASSERT(in_pName != NULL);
    ASSERT(STG_CANWRITE(io_pParent->mode));

    if ((io_pParent == NULL) || (in_pName == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    if (!STG_CANWRITE(io_pParent->mode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pRegFat = rootStorageGetFat(io_pParent->pRoot);
    pSmallFat = rootStorageGetSmallFat(io_pParent->pRoot);

    /* Get parent storage's directory entry */
    pTOC = rootStorageGetTOC(io_pParent->pRoot);
    pParentEntry = tocGetEntryAtIndex(pTOC, io_pParent->ulDirIndex);
    if (pParentEntry == NULL)
    {
        return (int)SSTG_ERROR_CHILD_NOT_FOUND;
    }

    /* Find specified stream's directory entry */
    iRet = tocFindChildEntry (pTOC,
                              pParentEntry,
                              in_pName,
                              &pChildEntry);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    /* Ensure that the deletion operation is legal */
    if ((directoryGetType(pChildEntry) != DIR_STREAM) &&
        (directoryGetType(pChildEntry) != DIR_STORAGE))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    /* From this point on, any failures will cause an incomplete state in
     * the structured storage file.  */

    /* Mark all metadata as being dirty so it will be rewritten later */
    iRet = rootStorageSetDirty(io_pParent->pRoot);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    if (directoryGetType(pChildEntry) == DIR_STREAM)
    {
        pHeader = rootStorageGetHeader(io_pParent->pRoot);
        if (directoryGetStreamLength(pChildEntry) < 
            headerGetSmallStreamCutoff(pHeader))
        {
            /* Merge the sectors of the stream into the free chain */
            iRet = fatFreeChain(pSmallFat,
                                directoryGetStartSector(pChildEntry));
            if (iRet != SSTG_OK)
            {
                return iRet;
            }

            /* Recover as many sectors as possible from the small FAT's free 
             * chain */
            iRet = fatRecoverMiniFatFreeSectors(pSmallFat, pRegFat);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
        else
        {
            /* Merge the sectors of the stream into the free chain */
            iRet = fatFreeChain(pRegFat, directoryGetStartSector(pChildEntry));
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
    }
    else if (directoryGetType(pChildEntry) == DIR_STORAGE)
    {
        /* Non-empty storages should be removed recursively */
        iRet = openStorage(io_pParent, in_pName, &pStorage);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }

        iRet = getChildrenInfo(pStorage, &pStgChildren, (int*)(void*)&childCount);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        for (i = 0; i < childCount; i++)
        {
            iRet = destroy(pStorage, pStgChildren[i].sName);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }

        if (childCount > 0)
        {
            iRet = freeInfo(&pStgChildren, childCount);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }

        iRet = closeStorage(&pStorage);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    /* Remove the corresponding entry from the TOC */
    iRet = tocRemoveEntry(pTOC, pParentEntry, pChildEntry);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

#ifdef DEBUG
    assertMiniStreamIntegrity(io_pParent->pRoot);
#endif /* DEBUG */

    return (int) iRet;
}


/*============================================================================
  Description:  (API Call) Writes an entire stream in one pass.
  Arguments: 
   io_pStorage - pointer to parent storage in which to create the new stream
   in_wszStreamName - name of new stream
   in_pvBuf - data to write into stream
   in_pcbBufSize - number of bytes in in_pvBuf
  Return:
   Status code
  ==========================================================================*/
int
flushStream(Storage* io_pStorage,
            const wchar_t* in_wszStreamName,
            const void* in_pvBuf,
            unsigned long* in_pcbBufSize)
{
    int     iRet =          SSTG_OK;
    Stream* pNewStream =    NULL;

    ASSERT ((io_pStorage != NULL) &&
            (in_wszStreamName != NULL) &&
            (*in_wszStreamName != 0) &&
            (in_pvBuf != NULL));
    if ((io_pStorage == NULL) ||
        (in_wszStreamName == NULL) ||
        (*in_wszStreamName == 0) ||
        (in_pvBuf == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    _KABOOM1;

    iRet = createStream(io_pStorage, in_wszStreamName, &pNewStream);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = streamWrite(pNewStream, in_pvBuf, in_pcbBufSize);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = closeStream(&pNewStream);

    return iRet;
}

/*============================================================================
  Description:  (API Call) Creates a new substorage in a given storage
  Arguments:    
   in_pStorage - the storage in which to create a substorage
   in_wszName -  the name of the new substorage
   out_ppChild - points to a pointer which, on success, will point to a new
                 Storage structure
  Return:       
   Status code
  ==========================================================================*/

int
createStorage (Storage* in_pStorage,
               const wchar_t* in_wszName,
               Storage** out_ppChild)
{
    Storage*        pNewStorage =   NULL;
    DirectoryEntry* pEntry =        NULL;
    SINT4           iRet =          SSTG_OK;
    TOC*            pTOC =          NULL;


    ASSERT (in_pStorage != NULL &&
            in_wszName != NULL &&
            *in_wszName != 0 &&
            out_ppChild != NULL &&
            STG_CANWRITE(in_pStorage->mode));
    if (in_pStorage == NULL ||
        in_wszName == NULL ||
        *in_wszName == 0 ||
        out_ppChild == NULL ||
        !STG_CANWRITE(in_pStorage->mode))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    /* Mark all metadata as being dirty so it will be rewritten later */
    iRet = rootStorageSetDirty(in_pStorage->pRoot);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    iRet = storageConstruct (out_ppChild);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }
    pNewStorage = *out_ppChild;

    pNewStorage->pRoot = in_pStorage->pRoot;
    pNewStorage->pParent = in_pStorage;

    /* create a directory entry for the storage */
    pTOC = rootStorageGetTOC(in_pStorage->pRoot);
	
	iRet = tocAddAndInitEntry(pTOC,
							in_wszName,
                            DIR_STORAGE,
                            in_pStorage->pRoot,
							&pEntry );

    if (iRet != SSTG_OK)
    {
        storageDestroy(out_ppChild);
        return iRet;
    }

    iRet = directorySetCurrentCreationTime(pEntry);
    if (iRet != SSTG_OK)
    {
        tocInvalidateEntry (pTOC, directoryGetIndex(pEntry));
        storageDestroy(out_ppChild);
        return iRet;
    }

    /* Make the new entry the child of its parent */
    iRet = tocInsertChild(pTOC,
                          tocGetEntryAtIndex (pTOC, in_pStorage->ulDirIndex),
                          pEntry);
    if (iRet != SSTG_OK)
    {
        tocInvalidateEntry (pTOC, directoryGetIndex(pEntry));
        storageDestroy(out_ppChild);
        return iRet;
    }

    _TCHECK;

    pNewStorage->ulDirIndex = directoryGetIndex(pEntry);
    pNewStorage->mode = in_pStorage->mode;

    iRet = openListAppend (in_pStorage->pOpenList, STORAGE, *out_ppChild);
    if (iRet != SSTG_OK)
    {
        tocInvalidateEntry (pTOC, directoryGetIndex(pEntry));
        storageDestroy(out_ppChild);
        return iRet;
    }

    return iRet;
}

/*============================================================================
  Description:  Creates a new storage as a root of the directory tree
  Arguments:    
   in_pRoot -    rootStorage to create storage for
   out_ppChild - handle to the newly created Storage structure
  Return:       
   Status code
  ==========================================================================*/

SINT4
storageCreateAtRoot (RootStorage* in_pRoot,
                     Storage** out_ppChild)
{
    Storage*            pNewStorage =   NULL;
    DirectoryEntry*     pEntry =        NULL;
    SINT4               iRet =          SSTG_OK;
    TOC*                pTOC =          NULL;

    ASSERT (in_pRoot != NULL && out_ppChild != NULL);
    if (in_pRoot == NULL || out_ppChild == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }


    iRet = storageConstruct (out_ppChild);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }
    pNewStorage = *out_ppChild;

    pNewStorage->pRoot = in_pRoot;
    pNewStorage->pParent = NULL;

    /* create a directory entry for the storage */
    pTOC = rootStorageGetTOC(in_pRoot);
    
	iRet = tocAddAndInitEntry(pTOC,
							ROOT_STORAGE_NAME,
                            DIR_ROOT,
                            in_pRoot,
							&pEntry );
    if (iRet != SSTG_OK)
    {
        storageDestroy(out_ppChild);
        return iRet;
    }

    pNewStorage->ulDirIndex = directoryGetIndex(pEntry);
    pNewStorage->mode = rootStorageGetMode(in_pRoot);

    return iRet;
}

#ifdef DEBUG

/*============================================================================
  Description: Self-test of the storage ADT.  Writes data to disk and reads
               it back in again, then compares the results.
  Return: 
   Status code
  ==========================================================================*/
#define WSZ_TEST_FILENAME L"testSSRW.bin"
#define WSZ_TEST_STREAM_1 L"TestStream1"
#define WSZ_TEST_STREAM_2 L"TestStream2"
#define WSZ_TEST_STREAM_3 L"TestStream3"
#define WSZ_TEST_STREAM_4 L"TestStream4"
#define WSZ_TEST_STORAGE_2 L"TestStorage2"
#define WSZ_TEST_STORAGE_3 L"TestStorage3"

SINT4
storageSelfTest1(StgMode in_stgModeOpen, StgMode in_stgModeCreate)
{
    SINT4           iRet =          SSTG_OK;
    RootStorage*    pRoot =         NULL;
    Storage*        pStorage1 =     NULL;
    Storage*        pStorage2 =     NULL;
    Storage*        pStorage3 =     NULL;
    Stream*         pStream1 =      NULL;
    Stream*         pStream2 =      NULL;
    Stream*         pStream3 =      NULL;
    Stream*         pStream4 =      NULL;
    int             cch =           0;
    unsigned long   cchL =          0;
    wchar_t         pwchBuf[] =     L"Stuff to write into streams";
    wchar_t         pwchBuf2[60];

    /* The main thing being tested here is the open list.  If the calling
     * application fails to close some streams and storages before closing
     * the structured storage file, the structured storage library is supposed 
     * to clean everything up automatically.
     */

    /* Create a bunch of streams and storages */
    iRet = createStructuredStorageEx (WSZ_TEST_FILENAME, in_stgModeCreate, &pRoot, 0);
    ASSERT (iRet == SSTG_OK && pRoot != NULL);

    iRet = getStorageFromRoot (pRoot, &pStorage1);
    ASSERT (iRet == SSTG_OK && pStorage1 != NULL);

    iRet = createStream (pStorage1, WSZ_TEST_STREAM_1, &pStream1);
    ASSERT (iRet == SSTG_OK && pStream1 != NULL);

    cch = 0;
    ASSERT (writeLEwstring(pStream1, pwchBuf, &cch) == SSTG_OK && cch == 28);

    iRet = createStorage (pStorage1, WSZ_TEST_STORAGE_2, &pStorage2);
    ASSERT (iRet == SSTG_OK && pStorage2 != NULL);

    iRet = createStorage (pStorage2, WSZ_TEST_STORAGE_3, &pStorage3);
    ASSERT (iRet == SSTG_OK && pStorage3 != NULL);

    iRet = createStream (pStorage2, WSZ_TEST_STREAM_2, &pStream2);
    ASSERT (iRet == SSTG_OK && pStream2 != NULL);

    cch = 0;
    ASSERT (writeLEwstring(pStream2, pwchBuf, &cch) == SSTG_OK && cch == 28);

    cchL = 28 * sizeof(wchar_t);
    iRet = flushStream(pStorage2, WSZ_TEST_STREAM_3, pwchBuf, &cchL);
    ASSERT(iRet == SSTG_OK && cchL == (28 * sizeof(wchar_t)));

    /* Test to ensure that destroying a mini stream works */
    cchL = 28 * sizeof(wchar_t);
    iRet = flushStream(pStorage2, WSZ_TEST_STREAM_4, pwchBuf, &cchL);
    ASSERT(iRet == SSTG_OK && cchL == (28 * sizeof(wchar_t)));
    iRet = destroy(pStorage2, WSZ_TEST_STREAM_4);
    ASSERT(iRet == SSTG_OK);

    /* Test to make sure duplicates are not allowed */
    iRet = createStream(pStorage2, WSZ_TEST_STREAM_3, &pStream4);
    ASSERT (iRet != SSTG_OK && pStream4 == NULL);

    /* Now close the entire compound file without closing any of the open
     * children */
    iRet = closeStructuredStorage(&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    pStorage1 = NULL;
    pStorage2 = NULL;
    pStorage3 = NULL;
    pStream1 = NULL;
    pStream2 = NULL;
    pStream3 = NULL;

    /* Now do the same thing for reading. */
    /* Open a bunch of streams and storages for reading. */
    iRet = openStructuredStorageEx (WSZ_TEST_FILENAME, in_stgModeOpen, &pRoot);
    ASSERT (iRet == SSTG_OK);

    iRet = getStorageFromRoot (pRoot, &pStorage1);
    ASSERT (iRet == SSTG_OK && pStorage1 != NULL);

    iRet = openStream (pStorage1, WSZ_TEST_STREAM_1, &pStream1);
    ASSERT (iRet == SSTG_OK && pStream1 != NULL);

    cch = 28;
    iRet = readLEwstring(pStream1, &cch, pwchBuf2);
    ASSERT (iRet == SSTG_OK && cch == 28);
    ASSERT (memcmp(pwchBuf, pwchBuf2, wcslen(pwchBuf) + 1) == 0);
    memset(pwchBuf2, 0, sizeof(pwchBuf2));

    iRet = openStorage (pStorage1, WSZ_TEST_STORAGE_2, &pStorage2);
    ASSERT (iRet == SSTG_OK && pStorage2 != NULL);

    iRet = openStorage (pStorage2, WSZ_TEST_STORAGE_3, &pStorage3);
    ASSERT (iRet == SSTG_OK && pStorage3 != NULL);

    iRet = openStream (pStorage2, WSZ_TEST_STREAM_2, &pStream2);
    ASSERT (iRet == SSTG_OK && pStream2 != NULL);

    cch = 28;
    ASSERT (readLEwstring(pStream2, &cch, pwchBuf2) == SSTG_OK && cch == 28);
    ASSERT (memcmp(pwchBuf, pwchBuf2, wcslen(pwchBuf) + 1) == 0);
    memset(pwchBuf2, 0, sizeof(pwchBuf2));

    iRet = openStream (pStorage2, WSZ_TEST_STREAM_3, &pStream3);
    ASSERT (iRet == SSTG_OK && pStream3 != NULL);

    cch = 28 * sizeof(wchar_t);
    iRet = streamRead(pStream3, pwchBuf2, &cchL);
    ASSERT (iRet == SSTG_OK && cchL == (28 * sizeof(wchar_t)));
    ASSERT (memcmp(pwchBuf, pwchBuf2, wcslen(pwchBuf) + 1) == 0);
    memset(pwchBuf2, 0, sizeof(pwchBuf2));

    /* Close the entire compound file without closing any of the open
     * children */
    iRet = closeStructuredStorage(&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Remove test file */
    SSRW_WREMOVE (WSZ_TEST_FILENAME);

    return iRet;
}


SINT4
storageSelfTest()
{
    SINT4 iRet = SSTG_OK;

    iRet = storageSelfTest1(STG_READ, STG_WRITE);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = storageSelfTest1(STG_RW, STG_RW);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    return iRet;
}

#endif /* DEBUG */

