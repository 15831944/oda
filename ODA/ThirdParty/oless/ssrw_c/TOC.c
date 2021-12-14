/************************************************************************
* TOC.c
* Table of Contents and its managing structure
* - implementation
*
* (c) Schema Software Inc., 2001-2004
* 
*************************************************************************

$Revision: 1.7.2.7 $
$Date: 2004/02/26 18:26:00 $
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "StructuredStorage.h"
#include "RootStorage.h"
#include "Header.h"
#include "Directory.h"
#include "TOC.h"
#include "Fat.h"


/* Support for the list of free entries in the TOC */
struct _EntryList
{
    UINT4 ulIndex;
    struct _EntryList* next;
};

typedef struct _EntryList EntryList;

/*============================================================================
  Description:  Methods to handle the EntryList structure
    This structures is used only in this file and only to represent 
    the stack of empty entries in the TOC.
  Methods supported:
    push, pop, destroy
  Arguments:    
   
  Return:       
  ==========================================================================*/

static SINT4 
entryListPush(UINT4 in_index, EntryList** io_pStack)
{
    EntryList* pNewEntry = (EntryList*)malloc(sizeof(EntryList));
    EntryList* tmp = *io_pStack;
    if(pNewEntry == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    /* check whether the entry is already in the stack */
    while (tmp != NULL)
    {
        if (tmp->ulIndex == in_index)
        {
        /* We are trying to add the entry that is already
         in the stack. This is OK, we simply do not have to do
         anything */
            return SSTG_OK;
        }
        tmp = tmp->next;
    }

    pNewEntry->ulIndex = in_index;

    /* This works even when io_stack is NULL */
    pNewEntry->next = *io_pStack;
    *io_pStack = pNewEntry;

    return SSTG_OK;
}

static UINT4 
entryListPop(EntryList** io_pStack)
{
    UINT4 index = (UINT4)-1;
    EntryList* pEntry = *io_pStack;

    if (pEntry != NULL)
    {
        index = pEntry->ulIndex;
        *io_pStack = pEntry->next;
        free(pEntry);
    }

    return index;
}

static void 
entryListDestroy(EntryList** io_pStack)
{
    while (*io_pStack != NULL)
        entryListPop(io_pStack);
}

/*================================================================*/


/* Table of Contents structure */
struct _TOC
{
    DirectoryEntry* aTOC;
    EntryList* freeEntryStack;
    UINT4 cEntries;
    UINT4 ulSectorSize;
};


/*============================================================================
  Description:  Allocates and initializes a TOC object
  Arguments:    
   in_ulSectorSize - number of bytes per sector in this structured storage 
                     file
   out_ppTOC -       on success, will point to pointer to freshly created TOC
  Return:       
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
tocConstruct(UINT4 in_ulSectorSize, TOC** out_ppTOC)
{
    TOC* ptr;

    ptr = (TOC*)malloc (sizeof(TOC));
    if (ptr == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(ptr, 0, sizeof(TOC));

    ptr->ulSectorSize = in_ulSectorSize;
    ptr->freeEntryStack = NULL;
    ptr->cEntries = 0;
    ptr->aTOC = NULL;
    *out_ppTOC = ptr;

    return SSTG_OK;
}


/*============================================================================
  Description:  Frees the structures associated with a TOC object
  Arguments:    
   io_ppTOC - pointer to pointer to TOC structure to free
  Return:       
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
tocDestroy(TOC** io_ppTOC)
{
    if (io_ppTOC != NULL && *io_ppTOC != NULL)
    {
        entryListDestroy(&((*io_ppTOC)->freeEntryStack));
        if ((*io_ppTOC)->aTOC != NULL)
        {
            free(((CHAR*)(*io_ppTOC)->aTOC) - sizeOfDirectoryEntry());
        }
        free(*io_ppTOC);
        *io_ppTOC = NULL;
    }
    return SSTG_OK;
}


/*============================================================================
  Description:  Adds (allocates) an additional sector to the TOC array
  Arguments:    
   in_pTOC -     TOC to add entry to (this) 
   out_ppEntry - On success, will point to new entry
  Return:
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
tocAddEntry(TOC* in_pTOC, DirectoryEntry** out_ppEntry)
{
    UINT4 entryPerBlock = in_pTOC->ulSectorSize/DIRECTORY_BLOCKSIZE;
    void* ptr;            /* temporary variable */
    UINT4 ulNewDirIndex = (UINT4)-1;

    ASSERT(in_pTOC != NULL);

    /* First search for existing empty slots.  This implementation is O(n),
     * but since this is only over the number of directory entries, it
     * shouldn't cost much unless the number of storages and streams is
     * extremely high.  Not much of a conceern. 
    for (i = 0; (i < in_pTOC->cEntries) && (ulNewDirIndex == ((UINT4)-1)); i++)
    {
        if (directoryGetType(tocGetEntryAtIndex(in_pTOC, i)) == DIR_FREE)
        {
            ulNewDirIndex = i;
        }
    } */

    ulNewDirIndex = entryListPop(&(in_pTOC->freeEntryStack));

    /* If there were no empty slots, add the new entry to the end of the list */
    if (ulNewDirIndex == (UINT4)-1)
    {
        /* Note:
         *   For the purposes of managing the red-black trees of the directory,
         *   it is useful to have an actual "nil" node.  This node should have
         *   the index DIR_NONE, but its colour is black.  Therefore we allocate
         *   an extra array member and reset the pointer to the array so that 
         *   this extra member has index -1.  We need to remember this when 
         *   freeing the array as well.
         */
        /* allocate new block of entries if necessary. */
        if (in_pTOC->cEntries % entryPerBlock == 0)
        {
            if (in_pTOC->aTOC == NULL)
            {
                ptr = malloc((entryPerBlock + 1) * sizeOfDirectoryEntry());
                directoryConstructNilEntry((DirectoryEntry*)ptr);
            }
            else
            {
                ptr = realloc((((CHAR*)in_pTOC->aTOC) - sizeOfDirectoryEntry()),
                              (in_pTOC->cEntries + entryPerBlock + 1) *
                              sizeOfDirectoryEntry());
            }
            if (ptr == NULL)
            {
                return SSTG_ERROR_MEMORY;
            }
            in_pTOC->aTOC = (DirectoryEntry*)
                (((CHAR*)ptr) + sizeOfDirectoryEntry());

        }
        ulNewDirIndex = in_pTOC->cEntries;
        (in_pTOC->cEntries)++;
    }

    /* initialize new entry */
    *out_ppEntry = directoryGetAtIndex(in_pTOC->aTOC, ulNewDirIndex);
    directoryConstructEntry(ulNewDirIndex, *out_ppEntry);

    return SSTG_OK;
}

/*============================================================================
  Description:  Adds an entry to TOC and initialize it with the info
  Arguments:    
   io_pTOC -        TOC to modify
   in_name -        name as a wide string
   in_type -        type (storage, stream, etc.)
   in_pRoot -       pointer to the root storage structure for this file
   out_ppEntry -    On success, will point to new entry
   
  Return:
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
tocAddAndInitEntry(TOC* in_pTOC,
                    const wchar_t* in_name,
                    DirEntry in_type,
                    struct _RootStorage* in_pRoot,
                    DirectoryEntry** out_ppEntry)
{
    SINT4 iRet = SSTG_OK;

    iRet =  tocAddEntry(in_pTOC, out_ppEntry);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = directoryInitEntry (*out_ppEntry,
                               in_name,
                               in_type,
                               in_pRoot);
    if (iRet != SSTG_OK)
    {
        tocInvalidateEntry(in_pTOC, directoryGetIndex(*out_ppEntry));
        out_ppEntry = NULL;
    }

    return iRet;
}

/*============================================================================
  Description:  Removes an entry from the TOC
  Arguments:    
   io_pTOC -        TOC to modify
   in_pPathParent - parent of entry to remove, relative to the directory tree, 
                    not the red-black tree.
   in_pToRemove -   pointer to entry to remove
  Return:
   Status code, STG_OK on success
  ==========================================================================*/
SINT4 
tocRemoveEntry(TOC* io_pTOC, 
               DirectoryEntry* in_pPathParent, 
               DirectoryEntry* in_pToRemove)
{
    SINT4 iRet;
    UINT4 ulEntry = directoryGetIndex(in_pToRemove);

    iRet =  directoryRemoveEntry(io_pTOC->aTOC, 
                                in_pPathParent,
                                in_pToRemove);
/* the last entry removal should not require stack push */
    if ((iRet == SSTG_OK) && (ulEntry < io_pTOC->cEntries))
    {
        iRet = entryListPush(ulEntry, &(io_pTOC->freeEntryStack));
    }

    return iRet;
}


/*============================================================================
  Description:  Set the size of each sector in the structured storage file
  Arguments:    
   io_pTOC -         TOC to modify
   in_ulSectorSize - new sector size.  Must be a power of 2.
  Return:
   Status code, STG_OK on success
  ==========================================================================*/
SINT4 
tocSetSectorSize(TOC* io_pTOC, UINT4 in_ulSectorSize)
{
    /* Assert that the sector size is a power of 2 */
    ASSERT(((in_ulSectorSize - 1) & in_ulSectorSize) == 0);

    io_pTOC->ulSectorSize = in_ulSectorSize;

    return SSTG_OK;
}


/*============================================================================
  Description:  registers TOC in FAT and streams it down to the file 
  Arguments:    
   in_pTOC -  TOC to write (this) 
   in_pRoot - pointer to an initialized RootStorage representing the open file
  Return:
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
tocWrite(TOC* in_this, RootStorage* in_pRoot)
{
    UINT4           i =                     0;
    SINT4           iRet =                  SSTG_OK;
    Fat*            pFAT =                  NULL;
    Header*         pHdr =                  NULL;
    UINT4           cbSector =              0;
    CHAR*           sectorBuffer =          NULL;
    DirectoryEntry* pEntry =                NULL;
    SINT4           offset =                0;
    UINT4           perSector =             0;
    UINT4           ulSectorWritten =       0;
    UINT4           ulLastSectorWritten =   0;
    UINT2           wSectorShift =          0;
    boolean         fFirstSectorUpdated =   0;

    pFAT = rootStorageGetFat(in_pRoot);
    pHdr = rootStorageGetHeader(in_pRoot);
    wSectorShift = headerGetSectorShift(pHdr);
    cbSector = 1 << wSectorShift;
    perSector = cbSector / DIRECTORY_BLOCKSIZE;

    /* In 4k sector files, the structured storage header must be updated with 
     * the number of sectors in the directory.  In 512-byte sectors, this value
     * must be 0. */
    if (wSectorShift == 9)
    {
        iRet = headerSetNumDirSectors(pHdr, 0);
    }
    else
    {
        iRet = headerSetNumDirSectors(
            pHdr, 
            (in_this->cEntries + (perSector - 1)) / perSector);
    }
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* allocate sector buffer */
    sectorBuffer = (CHAR*)malloc (cbSector);
    if (sectorBuffer == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(sectorBuffer, 0, cbSector);

    /* start loop along sectors -
     * please note: we increase 'i' manually inside the loop, but only for 
     * final array padding, so it is OK */
    for (i = 0; i < in_this->cEntries; i++)
    {
        pEntry = directoryGetAtIndex (in_this->aTOC, i);
        directoryEntryWrite (pEntry, sectorBuffer, &offset);

        /* if this is the last entry, pad the array with free entries,
           since we always read full sectors */
        if ((i + 1) == in_this->cEntries)
        {
            while ((i + 1) % perSector != 0)
            {
                pEntry = directoryGetAtIndex(in_this->aTOC, i + 1);
                directoryConstructEntry(i + 1, pEntry);
                directoryEntryWrite(pEntry, sectorBuffer, &offset);
                i++;
            }
        }

        /* Flush the sector if it is full or if we finished with directory */
        if ( (i + 1) % perSector == 0 )
        {
            /* we filled up the sector. Flush it to the disk */
            iRet = rootStorageWriteSector(in_pRoot,
                                          (BYTE*)sectorBuffer,
                                          &ulSectorWritten,
                                          SSRW_TRUE);
            if (iRet != SSTG_OK)
            {
                free (sectorBuffer);
                return iRet;
            }

            if (!fFirstSectorUpdated)
            {
                /* Update the root storage's header to record the first directory
                 * sector */
                headerSetDirectoryStartSector(pHdr, ulSectorWritten);
                fFirstSectorUpdated = 1;
                iRet = fatTerminateChain(pFAT, ulSectorWritten);
                if (iRet != SSTG_OK)
                {
                    free (sectorBuffer);
                    return iRet;
                }
            }
            else
            {
                /* Add entry to the Fat */
                iRet = fatUpdate(pFAT,ulLastSectorWritten,ulSectorWritten);
                if (iRet != SSTG_OK)
                {
                    free (sectorBuffer);
                    return iRet;
                }
            }
            /* Keep the number of the last sector written to chain the fat */
            ulLastSectorWritten = ulSectorWritten;
            offset = 0;
        }
    }

    free (sectorBuffer);

    return SSTG_OK;
}


/*============================================================================
  Description:  for given storage or stream, finds and returns DirEntry 
  Arguments:    
   in_pTOC -    TOC to search in (this) 
   in_pParent - parent object (should be storage)
   in_name -    name of the child we are looking for
   out_ppDir -  handle for found entry (NULL if it has not been found)
  Return:
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
tocFindChildEntry (TOC* in_pTOC,
                   DirectoryEntry* in_pParent,
                   const wchar_t* in_name,
                   DirectoryEntry** out_ppDir)
{
    SINT4 iRet = directoryFindChild (in_pParent,
                                     in_name,
                                     in_pTOC->aTOC,
                                     out_ppDir);
    return iRet;
}


/*============================================================================
  Description:  Returns the directory entry at a given index in the table of
                contents
  Arguments:    
   in_pTOC -    pointer to table of contents struct
   in_ulIndex - index of directory entry to retrieve
  Return:
   NULL on failure, pointer to directory entry on success
  ==========================================================================*/
DirectoryEntry*
tocGetEntryAtIndex(TOC* in_pTOC, UINT4 in_ulIndex)
{
    return directoryGetAtIndex(in_pTOC->aTOC, in_ulIndex);
}


/*============================================================================
  Description:  Counts children for the given entry
  Arguments:    
   in_pTOC -   pointer to table of contents struct
   in_pEntry - directory entry for which to retrieve a sibling pointer
  Return:
   NULL if it has no more siblings, pointer to directory entry on success
  ==========================================================================*/
UINT4
tocCountEntryChildren(TOC* in_pTOC, DirectoryEntry* in_pEntry)
{
    UINT4           n =      0;
    DirectoryEntry* pChild = directoryGetFirstChild(in_pTOC->aTOC, in_pEntry);

    if (pChild == NULL)
    {
        return 0;
    }

    n = directoryCountChildren(pChild, 0, in_pTOC->aTOC);

    return n;
}


/*============================================================================
  Description:  Counts children for the given entry
  Arguments:    
   in_pTOC -   pointer to table of contents struct
   in_pEntry - directory entry for which to retrieve a sibling pointer
   out_aInfo - allocated array to be filled
  Return:
   number of filled elements
  ==========================================================================*/
UINT4
tocFillChildrenStgArray(TOC* in_pTOC,
                        DirectoryEntry* in_pEntry,
                        StgInfo* out_aInfo)
{
    UINT4           n =      0;
    DirectoryEntry* pChild = directoryGetFirstChild(in_pTOC->aTOC, in_pEntry);

    if (pChild == NULL)
    {
        return 0;
    }

    n = directoryFillStgArray(pChild, out_aInfo, 0, in_pTOC->aTOC);

    return n;
}


/*============================================================================
  Description:  Reads an entire table of contents from the current, open 
                structured storage file
  Arguments:    
   io_pTOC -   TOC to read into - actually, redundant 
   in_pRoot -  open structured storage
   out_ppDir - handle for found entry (NULL if it has not been found)
  Return:       
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
tocRead (TOC* io_pTOC, RootStorage* in_pRoot)
{
    SINT4 iRet = SSTG_OK;
    UINT4 i;

    ASSERT(in_pRoot != NULL);
    ASSERT(io_pTOC != NULL);
    ASSERT(io_pTOC->aTOC == NULL);
    ASSERT(io_pTOC->cEntries == 0);
    ASSERT(io_pTOC->freeEntryStack == NULL);

    iRet = directoryConstructAllDirs (in_pRoot,
                                      &(io_pTOC->aTOC),
                                      &(io_pTOC->cEntries));

    /* Search for existing empty slots and put them into
     * free entry stack 
     */
    for (i = 0; i < io_pTOC->cEntries; i++)
    {
        if (directoryGetType(tocGetEntryAtIndex(io_pTOC, i)) == DIR_FREE)
        {
            entryListPush(i, &(io_pTOC->freeEntryStack));
        }

        if (iRet != SSTG_OK) break;
    } 

    return iRet;
}


/*============================================================================
  Description:  Makes a given directory entry the child of another
  Arguments:    
   io_pTOC -   TOC to modify
   in_pRoot -  pointer to entry to make the parent
   in_pChild - pointer to entry to make the child
  Return:       
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
tocInsertChild (TOC* in_pTOC,
                DirectoryEntry* in_pParent,
                DirectoryEntry* in_pChild)
{
    return directoryInsertChild (in_pParent, in_pChild, in_pTOC->aTOC);
}


/*============================================================================
  Description:  Occasionally it is possible for a call to createStorage() or
                some similar call to fail due to memory failure or something
                like that.  In that case we need to invalidate any freshly
                created directory entries.
                This function does that--but it assumes that the entry is
                the most recently created entry and has no children.
  Arguments:    
   io_pTOC -    TOC to modify
   in_ulIndex - index of the entry to invalidate (should be the last entry
                in the TOC)
  Return:
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
tocInvalidateEntry(TOC* io_pTOC,
                   UINT4 in_ulIndex)
{
    SINT4 iRet = SSTG_OK;
    DirectoryEntry* pEntry = NULL;

    ASSERT (io_pTOC != NULL &&
            io_pTOC->aTOC != NULL &&
            in_ulIndex < io_pTOC->cEntries);

    pEntry = directoryGetAtIndex(io_pTOC->aTOC, in_ulIndex);
    if (pEntry == NULL)
    {
        return SSTG_ERROR_CHILD_NOT_FOUND;
    }

    iRet = directoryInvalidate (pEntry);
    if (iRet != SSTG_OK) return iRet;

    if (in_ulIndex == (io_pTOC->cEntries - 1))
    {
        io_pTOC->cEntries--;
    }
    else
    {
        iRet = entryListPush(in_ulIndex, &(io_pTOC->freeEntryStack));
    }

    return iRet;
}


#ifdef DEBUG

SINT4
tocSelfTest(void)
{
    wchar_t* testNames[] = { 
        L"Mary", 
        L"had", 
        L"a", 
        L"little", 
        L"lamb",
        L"its",
        L"fleece",
        L"was",
        L"white",
        L"as", 
        L"snow",
        L"and",
        L"everywhere",
        L"that",
        L"went", 
        L"the",
        L"sure",
        L"to", 
        L"go" };

    SINT4           testNameCount = sizeof(testNames) / sizeof(char*);
    struct _TOC* pTOC =                NULL;
    DirectoryEntry* aTOC =          NULL;
    DirectoryEntry* pTOCTmp =       NULL;
    SINT4           iRet =          0;
    SINT4           i =             0;
    UINT4            index =            (UINT4)-1;
    SINT4           removeOrder[] =     { 19,  1, 11, 12, 10, 13,  9, 18,  2,  3, 
                                      4,   5, 17, 16,  6, 15,  7,  8, 14 };
                               

    /* Setup a fake table of contents. */
    iRet = tocConstruct(512, &pTOC);
    ASSERT(iRet == SSTG_OK);
    aTOC = pTOC->aTOC;
    ASSERT(aTOC == NULL);

    iRet = tocAddAndInitEntry(pTOC,
                            L"Root Entry",
                            DIR_ROOT,
                            NULL,
                            &pTOCTmp);

    ASSERT(iRet == SSTG_OK);

     /* Add directory entries with the names individually */
    for(i = 0; i < testNameCount; i++)
    {       
        iRet = tocAddAndInitEntry(pTOC, 
                                  testNames[i], 
                                  DIR_STREAM,
                                  NULL,
                                  &pTOCTmp);
        ASSERT(iRet == SSTG_OK);
    }

    for (i = 0; i < testNameCount/2; i++)
    {
        index = removeOrder[i];
        iRet = tocRemoveEntry(pTOC, tocGetEntryAtIndex(pTOC,0),
                                    tocGetEntryAtIndex(pTOC,index) );
            
        ASSERT(iRet == SSTG_OK);
    /* It SHOULD assert when we are deleting the last entry */
        ASSERT(pTOC->freeEntryStack->ulIndex == index);
    }

     /* Add directory entries with the names individually */
    for(i = testNameCount/2 + 1; i < testNameCount; i++)
    {      
        index = pTOC->freeEntryStack->ulIndex;

        iRet = tocAddAndInitEntry(pTOC, 
                                  testNames[i], 
                                  DIR_STREAM,
                                  NULL,
                                  &pTOCTmp);
        ASSERT(iRet == SSTG_OK);
        ASSERT(directoryGetIndex(pTOCTmp) == index);
    }
    iRet = tocDestroy(&pTOC);
    ASSERT(iRet == SSTG_OK);

    return iRet;
}


/*============================================================================
  Description:  (DEBUG only) Recurses through all of the directory entries of 
                the TOC, blanking sectors in two FAT arrays that are used by 
                streams in the TOC.
                This functionality is used in a debug-only self-check to ensure
                that sectors are not "leaked" by the structured storage library
                when writing files.
  Arguments:    
   in_pTOC -             TOC to recurse through
   io_pulFat -           regular FAT array to blank when encountering 
                         regular-sized streams
   in_ulFatLength -      length of the regular FAT array
   io_pulSmallFat -      small FAT array to blank when encountering 
                         small streams
   in_ulSmallFatLength - length of the small FAT array
  Return:
   Status code, STG_OK on success
  ==========================================================================*/
SINT4 
tocBlankUsedSectors(TOC* in_pTOC,
                    UINT4* io_pulFat,
                    UINT4 in_ulFatLength,
                    UINT4* io_pulSmallFat,
                    UINT4 in_ulSmallFatLength)
{
    return directoryBlankFatSectors(
        in_pTOC->aTOC,
        0,
        io_pulFat,
        in_ulFatLength,
        io_pulSmallFat,
        in_ulSmallFatLength);
                
}

#endif /* DEBUG */
