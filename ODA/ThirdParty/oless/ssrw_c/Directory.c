/************************************************************************
* Directory.c
*
* Defines the DirectoryEntry type
*
* (c) Schema Software Inc., 2001-2004
* 
************************************************************************

$Revision: 1.13.2.9 $
$Date: 2004/05/28 18:14:08 $
*/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>

#include "Types.h"
#include "Common.h"
#include "Debug.h"
#include "StructuredStorage.h"
#include "RootStorage.h"
#include "Header.h"
#include "Directory.h"
#include "Stream.h"
#include "LEReadWrite.h"

/* Tree node colors */
#define DE_RED 0x0
#define DE_BLACK 0x1

/* marking the absence of a sibling and/or child */
#define DIR_NONE -1

#define RED_BLACK 1

struct _DirectoryEntry
{
    RootStorage* pRoot;              /* Pointer to initialized RootStorage
                                      * structure */

    /* added to make Red Black algorithms easy */
    SINT4 parent;                    /* index of red-black tree parent in the
                                      * TOC */
    SINT4 sid;                       /* index in the TOC array */

    /* Corresponds to the Directory entry on the disk */
    wchar_t name[NAME_SIZE];         /* Name of the stream or storage */
    UINT2 nameLength;                /* Byte length of the name (NOTE: ONLY
                                      * valid where sizeof(wchar_t) == 2) */
    DirEntry type;                   /* DIR_ROOT, DIR_STREAM, DIR_STORAGE, or
                                      * DIR_FREE */
    BYTE BFlags;                     /* red or black */
    SINT4 left;                      /* left child (here child means child in
                                      * the red-black tree, NOT child in the 
                                      * directory tree) */
    SINT4 right;                     /* right child */
    SINT4 child;
    SINT4 userFlags;
    SSRW_CLSID clsid;
    time_t creationTime;             /* time this storage was created (only
                                      * valid for storages) */
    time_t lastModification;         /* time this storage was last modified
                                      * (only valid for storages */
    UINT4 startSector;               /* first sector of this stream, indicates
                                      * the start of the FAT chain */
    UINT8 streamLength;              /* number of bytes in the stream, not
                                      * necessarily sector-aligned. 
                                      * Newer structured storage files can have
                                      * individual streams > 4GB in size.
                                      */
    StreamSharedBlock* pSharedBlock; /* For streams, points to a chunk of data
                                      * that is shared by all open Stream 
                                      * objects that refer to the same physical
                                      * stream. */
    UINT2 bRightEntry;               // To mark corrupted directory entry
    UINT2 bEntryWasUsed;             // Not to allow hung up during constructing of tree
};

static SINT4 readDirectory (DirectoryEntry* io_pDir, SINT4 in_index);
static SINT4 compareNames (const wchar_t* in_name0, const wchar_t *in_name1);

#ifdef RED_BLACK
static SINT4 balanceRedBlackTree(DirectoryEntry* in_pNew, DirectoryEntry* in_pRoot, DirectoryEntry* aDir);
static SINT4 directoryRemoveFixup(DirectoryEntry*  io_aDir,
                                  DirectoryEntry** io_pRootEntry,
                                  DirectoryEntry*  in_pCurEntry);
static DirectoryEntry* nodeLeftRotate(DirectoryEntry* in_pNode,
                                      DirectoryEntry* in_pRoot,
                                      DirectoryEntry* aDir);
static DirectoryEntry* nodeRightRotate(DirectoryEntry* in_pNode,
                                       DirectoryEntry* in_pRoot,
                                       DirectoryEntry* aDir);
#endif /* RED_BLACK */

#ifdef DEBUG
static SINT4 directoryAssertEntriesConsistent(const DirectoryEntry* in_aDir, 
                                              SINT4 in_lArraySize);
static SINT4 directoryAssertBSTInvariants(const DirectoryEntry* in_aDir,
                                          const DirectoryEntry* in_pCurEntry,
                                          const wchar_t* in_pLeftBoundingName,
                                          const wchar_t* in_pRightBoundingName);
#ifdef RED_BLACK
static SINT4 directoryAssertRedBlackInvariants(const DirectoryEntry* in_aDir,
                                               const DirectoryEntry* in_pCurEntry);
#endif /* RED_BLACK */

#endif /* DEBUG */


/* offsets of fields of directory entries on disk */
#define FIELD_OFFSETS {0, 64, 66, 67, 68, 72, 76, 80, 84, 86, 88, 96, 100, 108, 116, 120}

/*============================================================================
  Description: Returns the directory entry at index i in the Table of Contents
               (TOC)
  Arguments: 
   in_aDir - pointer to the start of the table of contents array
   i -       index of directory entry to return
  Return: 
   Status code
  ==========================================================================*/
DirectoryEntry*
directoryGetAtIndex(DirectoryEntry* in_aDir, UINT4 i)
{
    return &(in_aDir[i]);
}


/*============================================================================
  Description: Returns the size, in bytes of a single directory entry 
               structure instance
  Arguments: 
  Return: 
   Size in bytes
  ==========================================================================*/
SINT4
sizeOfDirectoryEntry()
{
    return sizeof(DirectoryEntry);
}


/*============================================================================
  Description: Returns the index of a directory entry in the TOC
               (Note, this is equivalent to the SID of the directory entry)
  Arguments: 
   in_pDirEntry - pointer to directory entry whose index to retrieve
  Return: 
   The index of the directory entry
  ==========================================================================*/
UINT4
directoryGetIndex(DirectoryEntry* in_pDirEntry)
{
    ASSERT(in_pDirEntry != NULL);
    return in_pDirEntry->sid;
}


/*============================================================================
  Description: Returns the type (DIR_STREAM, DIR_STORAGE, etc.) of a 
               directory entry
  Arguments: 
   in_pDirEntry - pointer to directory entry whose type to retrieve
  Return: 
   The type of the directory entry
  ==========================================================================*/
DirEntry
directoryGetType (DirectoryEntry* in_pDirEntry)
{
    return in_pDirEntry->type;
}


/*============================================================================
  Description: Returns the start sector of a stream corresponding to a given
               directory entry.
  Arguments: 
   in_pDirEntry - pointer to directory entry whose start sector to retrieve
  Return: 
   The index of the directory entry
  ==========================================================================*/
UINT4
directoryGetStartSector (DirectoryEntry* in_pDirEntry)
{
    return in_pDirEntry->startSector;
}


/*============================================================================
  Description: Returns the size, in bytes, of a stream corresponding to a 
               given directory entry.
  Arguments: 
   in_pDirEntry - pointer to directory entry whose start sector to retrieve
  Return: 
   The index of the directory entry
  ==========================================================================*/
UINT8
directoryGetStreamLength (DirectoryEntry* in_pDirEntry)
{
    return in_pDirEntry->streamLength;
}


/*============================================================================
  Description: Returns the topmost or "root" child of a directory entry in the 
               TOC.  (There are two types of trees here, the directory tree and 
               many red-black trees.  This returns the root of a red-black 
               tree.)
  Arguments: 
   in_aDir -      pointer to the start of the table of contents array
   in_pDirEntry - pointer to directory entry whose start sector to retrieve
  Return: 
   The index of the directory entry
  ==========================================================================*/
DirectoryEntry*
directoryGetFirstChild (DirectoryEntry* in_aDir, DirectoryEntry* in_pDirEntry)
{
    if (in_pDirEntry->child == DIR_NONE)
    {
        return NULL;
    }
    return directoryGetAtIndex(in_aDir, in_pDirEntry->child);
}

DirectoryEntry*
directoryGetRightSibling (DirectoryEntry* in_aDir, DirectoryEntry* in_pDirEntry)
{
    if (in_pDirEntry->right == DIR_NONE)
    {
        return NULL;
    }
    return directoryGetAtIndex(in_aDir, in_pDirEntry->right);
}

/*============================================================================
  Description: Pre-initialization of a directory entry.
  Arguments: 
   in_sid -     index of the new entry in the directory array (0-based)
   out_pEntry - pointer that will, on success, point to a newly pre-initialized 
                entry
  Return: 
   Status code
  Note: There is no "constructor" (memory allocator) for DirEntries. TOC does
   all this work serving as a factory.
  ==========================================================================*/
SINT4
directoryConstructEntry(UINT4 in_sid, DirectoryEntry* out_pEntry)
{
    out_pEntry->sid = in_sid;
    out_pEntry->name[0] = 0;
    out_pEntry->nameLength = 0;
    out_pEntry->parent = DIR_NONE;
    out_pEntry->type = DIR_FREE;
    out_pEntry->BFlags = DE_RED;
    out_pEntry->left = DIR_NONE;
    out_pEntry->right = DIR_NONE;
    out_pEntry->child = DIR_NONE;
    out_pEntry->userFlags = 0;
    memset(&(out_pEntry->clsid), 0, CLSID_SIZE);
    out_pEntry->creationTime = 0;
    out_pEntry->lastModification = 0;
    out_pEntry->startSector = (UINT4)-1;
    out_pEntry->streamLength = 0;
    out_pEntry->pSharedBlock = NULL;

    return SSTG_OK;
}


/*============================================================================
  Description: Pre-initialization of the "nil" directory entry that resides
               at index DIR_NONE (-1) of the table of contents array.
  Arguments: 
   out_ppEntry - pointer that will, on success, point to the newly initialized 
                 nil entry
  Return: 
   Status code
  ==========================================================================*/
SINT4
directoryConstructNilEntry(DirectoryEntry* out_pEntry)
{
    SINT4 result = SSTG_OK;

    result = directoryConstructEntry((UINT4)DIR_NONE, out_pEntry);
    out_pEntry->BFlags = DE_BLACK;

    return result;
}


/*============================================================================
  Description:  Puts initial values into allocated directory entry
  Arguments:    
   in_pEntry (this) -  entry to initialize
   in_name -           name as a wide string
   in_type -           type (storage, stream, etc.)
   in_pRoot -          pointer to the root storage structure for this file
                
  Return:       
   Status code - always SSTG_OK
  ==========================================================================*/
SINT4
directoryInitEntry (DirectoryEntry* in_pEntry,
                    const wchar_t* in_name,
                    DirEntry in_type,
                    struct _RootStorage* in_pRoot)
{
    UINT4 ulNameLen = 0;

    ulNameLen = wcslen(in_name) + 1;

    if (ulNameLen > NAME_SIZE)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    in_pEntry->nameLength = (UINT2)(ulNameLen * sizeof(CHAR2));

    wcscpy(in_pEntry->name, in_name);
    in_pEntry->type = in_type;
    in_pEntry->pRoot = in_pRoot;

    return SSTG_OK;
}


/*============================================================================
  Description: Performs a post-reading normalization pass on the directory
               after it is read from a file.
               This function is recursive.
  Arguments: 
   io_aDir -    the base address of the table of contents array 
   io_pRoot -   the root node of the current subtree
  Return: 
   Status code
  ==========================================================================*/
SINT4 directoryNormalize(DirectoryEntry* io_aDir, DirectoryEntry* io_pRoot)
{
    SINT4 iRet = SSTG_OK;

    ASSERT((io_pRoot->type == DIR_FREE)
           || (io_pRoot->type == DIR_STORAGE)
           || (io_pRoot->type == DIR_FREE)
           || (io_pRoot->type == DIR_STREAM));

    // MKU 03\20\14 - fixed #13648 (The attached dgn file crashes the library)
    if ((io_pRoot->type != DIR_STORAGE) 
        && (io_pRoot->type != DIR_ROOT)
        && (io_pRoot->type != DIR_FREE)
        && (io_pRoot->type != DIR_STREAM))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    io_pRoot->bEntryWasUsed = 1;

    /* Normalize child subtree (of the directory path tree) */
    if (io_pRoot->child != DIR_NONE)
    {
      if( io_aDir[io_pRoot->child].bEntryWasUsed > 0 )
        io_pRoot->child = DIR_NONE;
      else
      {
        iRet = directoryNormalize(io_aDir, &io_aDir[io_pRoot->child]);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        io_aDir[io_pRoot->child].parent = DIR_NONE;
      }
    }

    /* Normalize left subtree (of the red-black tree) */
    if (io_pRoot->left != DIR_NONE)
    {
      if( io_aDir[io_pRoot->left].bEntryWasUsed > 0 )
        io_pRoot->left = DIR_NONE;
      else
      {
        iRet = directoryNormalize(io_aDir, &io_aDir[io_pRoot->left]);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        io_aDir[io_pRoot->left].parent = io_pRoot->sid;
      }
    }

    /* Normalize right subtree (of the red-black tree) */
    if (io_pRoot->right != DIR_NONE)
    {
      if( io_aDir[io_pRoot->right].bEntryWasUsed > 0 )
        io_pRoot->right = DIR_NONE;
      else
      {
        iRet = directoryNormalize(io_aDir, &io_aDir[io_pRoot->right]);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        io_aDir[io_pRoot->right].parent = io_pRoot->sid;
      }
    }

    io_pRoot->bEntryWasUsed = 0;

    return SSTG_OK;
}


/*============================================================================
  Description: Creates the entire array of DirectoryEntries.
  Arguments: 
   in_pRoot -   the existing, initialized RootStorage
   out_paDirs - on success, will point to freshly allocated and filled
                array of DirectoryEntries
   out_pcDirs - on success, will contian the number of DirectoryEntries
  Return: 
   Status code
  ==========================================================================*/
SINT4
directoryConstructAllDirs (RootStorage* in_pRoot,
                           DirectoryEntry** out_paDirs,
                           UINT4* out_pcDirs)
{
    UINT4           i =                         0;
    SINT4           iRet =                      SSTG_OK;
    UINT4           cDirEntriesNominal =        0;
    UINT4*          pulDirChain =               NULL;
    UINT4           ulDirChainLen =             0;
    Header*         pHdr =                      NULL;
    UINT2           wSectorShift =              0;
    DirectoryEntry* pTmp =                      NULL;

    ASSERT(in_pRoot != NULL);
    ASSERT(out_paDirs != NULL && *out_paDirs == NULL);
    ASSERT(out_pcDirs != NULL && *out_pcDirs == 0);

    rootStorageGetDirChain(in_pRoot, &pulDirChain, &ulDirChainLen);
    pHdr = rootStorageGetHeader(in_pRoot);
    wSectorShift = headerGetSectorShift(pHdr);

    /* (MarkA) I'm not sure there's any way to figure out the exact number of
     * directory entries other than to multiply the number of sectors by the
     * number of entries per sector (usually 4). */
    cDirEntriesNominal = ((1 << wSectorShift) *
                          ulDirChainLen) / DIRECTORY_BLOCKSIZE;

    /* Here again, we allocate an extra entry for the DIR_NONE entry of the 
     * array, which resides at index DIR_NONE (-1).  See TOC.c for another
     * comment on why we do this. */
    pTmp = (DirectoryEntry*)malloc ((cDirEntriesNominal + 1) *
                                    sizeof(DirectoryEntry));
    if (pTmp == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( pTmp, 0, cDirEntriesNominal * sizeof(DirectoryEntry) );
    directoryConstructNilEntry(pTmp);

    *out_paDirs = pTmp + 1;

    for (i = 0; i < cDirEntriesNominal; i++)
    {
        (*out_paDirs)[i].bEntryWasUsed = 0;
        (*out_paDirs)[i].pRoot = in_pRoot;
        iRet = readDirectory (&(*out_paDirs)[i], i);

        if (iRet != SSTG_OK)
        {
          (*out_paDirs)[i].left  = DIR_NONE;
          (*out_paDirs)[i].right = DIR_NONE;
          (*out_paDirs)[i].child = DIR_NONE;
          (*out_paDirs)[i].type  = DIR_STREAM;
          (*out_paDirs)[i].streamLength = 0;
          (*out_paDirs)[i].bRightEntry = 0;
        }
        else
        {
          if( (*out_paDirs)[i].left >= (SINT4)(cDirEntriesNominal) )
          {
            (*out_paDirs)[i].left = DIR_NONE;
          }

          if( (*out_paDirs)[i].right >= (SINT4)(cDirEntriesNominal) )
          {
            (*out_paDirs)[i].right = DIR_NONE;
          }

          if( (*out_paDirs)[i].child >= (SINT4)(cDirEntriesNominal) )
          {
            (*out_paDirs)[i].child = DIR_NONE;
          }

          if( (*out_paDirs)[i].type == DIR_FREE )
          {
            (*out_paDirs)[i].left  = DIR_NONE;
            (*out_paDirs)[i].right = DIR_NONE;
            (*out_paDirs)[i].child = DIR_NONE;
          }

          (*out_paDirs)[i].bRightEntry = 1;
        }
    }

    iRet = directoryNormalize(*out_paDirs, *out_paDirs);
    *out_pcDirs = cDirEntriesNominal;

    return iRet;
}


/*============================================================================
  Description:  Counts number of children for the node in the Red-Black
                tree. 
  Note:     This is tree walking recursive function that needs to be called
   on the storage's child - it is the root of the sibling tree.
  Arguments: 
   in_pEntry   - this node
   n        - number of nodes found so far
   aDir     - array describing the tree
                
  Return:       
   Number of children
  ==========================================================================*/
UINT4
directoryCountChildren(DirectoryEntry* in_pEntry, UINT4 n, DirectoryEntry* aDir)
{
    ASSERT (in_pEntry != NULL);
    n++;
    if (in_pEntry->left != DIR_NONE)
    {
       n = directoryCountChildren( &aDir[in_pEntry->left] , n, aDir);
    }
    if (in_pEntry->right != DIR_NONE)
    {
       n = directoryCountChildren( &aDir[in_pEntry->right], n, aDir);
    }
    return n;
}


/*============================================================================
  Description:  Finds a child entry from a passed name.
  Arguments:    
   in_pDir:      this
   in_wszName:   name of child to be retrieved
   aDir -        the directory array
   out_ppChild - on success, will point to child entry
  Return:       
   Status code.  If not found, returns SSTG_ERROR_CHILD_NOT_FOUND.
  ==========================================================================*/
SINT4
directoryFindChild (DirectoryEntry* in_pDir,
                    const wchar_t* in_wszName,
                    DirectoryEntry* aDir,
                    DirectoryEntry** out_ppChild)
{
    DirectoryEntry* pCurrent = NULL;

    SINT4 iCurrent = in_pDir->child;
    SINT4 iCmp = 0;
    SINT4 iParent;

    ASSERT(in_pDir != NULL);
    ASSERT(in_wszName != NULL);
    ASSERT(out_ppChild != NULL);

    if ((in_pDir->type != DIR_STORAGE) &&
        (in_pDir->type != DIR_ROOT))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    *out_ppChild = NULL;

    if  (iCurrent == DIR_NONE)
    {
        return SSTG_ERROR_CHILD_NOT_FOUND;
    }

    /* Search in the tree */
    while (iCurrent != DIR_NONE)
    {
        pCurrent = &aDir[iCurrent];
        iParent = iCurrent;
        iCmp = compareNames(pCurrent->name, in_wszName);
        if (iCmp == 0)
        {
            *out_ppChild = pCurrent;
            break;
        }
        iCurrent = (iCmp > 0) ? pCurrent->left : pCurrent->right;
    }

    return (*out_ppChild == NULL)? SSTG_ERROR_CHILD_NOT_FOUND : SSTG_OK;
}


/*============================================================================
  Description:  Fills a StgInfo structure for i-th child of the given
                entry - used for recursively filling the StgInfo array
  Arguments:    
   in_pDir -      directory entry to read information from
   out_pStgInfo - pointer to StgInfo structure to fill
  Return:       
   Status code
  ==========================================================================*/
UINT4
directoryFillStgArray(DirectoryEntry* in_pDir, StgInfo* out_aStgInfo, UINT4 i, DirectoryEntry* aDir)
{
    directoryFillStgInfo(in_pDir, out_aStgInfo + i);
    i++;
    if (in_pDir->left != DIR_NONE)
        i = directoryFillStgArray(&aDir[in_pDir->left], out_aStgInfo, i, aDir);
    if (in_pDir->right != DIR_NONE)
        i = directoryFillStgArray(&aDir[in_pDir->right], out_aStgInfo, i, aDir);
    return i;
}

/*============================================================================
  Description:  Fills a StgInfo structure with information about a given 
                stream or storage
  Arguments:    
   in_pDir -      directory entry to read information from
   out_pStgInfo - pointer to StgInfo structure to fill
  Return:       
   Status code
  ==========================================================================*/
SINT4
directoryFillStgInfo(DirectoryEntry* in_pDir, StgInfo* out_pStgInfo)
{
    SINT4 iRet = SSTG_OK;

    ASSERT (out_pStgInfo != NULL && in_pDir != NULL);
    if (out_pStgInfo == NULL || in_pDir == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    out_pStgInfo->sName = 
        (wchar_t*)malloc((wcslen(in_pDir->name) + 1) * sizeof(wchar_t));
    if (out_pStgInfo->sName == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(  out_pStgInfo->sName, 0, (wcslen(in_pDir->name) + 1) * sizeof(wchar_t));

    memcpy (out_pStgInfo->sName,
            in_pDir->name,
            wcslen(in_pDir->name) * sizeof(wchar_t));
    out_pStgInfo->sName[wcslen(in_pDir->name)] = 0;
    if (in_pDir->type == DIR_STREAM)
    {
        out_pStgInfo->type = STREAM;
    }
    else if ((in_pDir->type == DIR_STORAGE) ||
             (in_pDir->type == DIR_ROOT))
    {
        out_pStgInfo->type = STORAGE;
    }
    else
    {
        /* This definitely shouldn't happen */
        ASSERT(0);
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    out_pStgInfo->userFlags = in_pDir->userFlags;
    out_pStgInfo->size = in_pDir->streamLength;
    out_pStgInfo->creationTime = in_pDir->creationTime;
    out_pStgInfo->modificationTime = in_pDir->lastModification;
    out_pStgInfo->accessMode = rootStorageGetMode(in_pDir->pRoot);
    memcpy (&(out_pStgInfo->clsid), &(in_pDir->clsid), CLSID_SIZE);

    return iRet;
}


/*============================================================================
  Description:  Reads a directory structure from disk
  Arguments:    
   io_pDir -   The uninitialized directory structure
   in_index - The index of the directory structure
  Return:       
   Status code
  ==========================================================================*/
static SINT4
readDirectory (DirectoryEntry* io_pDir, SINT4 in_index)
{
    CHAR*           directoryBuffer;
    SINT4           entriesPerSector;
    SINT4           iChar;
    CHAR*           pB;
    SINT4           offsetArray[] =         FIELD_OFFSETS;
    SINT4*          pOffset =               offsetArray;
    UINT4*          pulDirChain =           NULL;
    UINT4           ulDirChainLen =         0;
    Header*         pHdr =                  NULL;
    UINT2           wSectorShift =          0;
    UINT2           wMajVer =               0;

    rootStorageGetDirChain(io_pDir->pRoot, &pulDirChain, &ulDirChainLen);
    pHdr = rootStorageGetHeader(io_pDir->pRoot);
    wSectorShift = headerGetSectorShift(pHdr);

    entriesPerSector = (1 << wSectorShift) / DIRECTORY_BLOCKSIZE;

    directoryBuffer = rootStorageReadSector (io_pDir->pRoot,
                      *(pulDirChain + in_index / entriesPerSector));
    if (directoryBuffer == NULL)
    {
        return rootStorageGetStatus (io_pDir->pRoot);
    }

    io_pDir->sid = in_index;

    /* now read the thing                                               */
    pB = directoryBuffer + (in_index % entriesPerSector) * DIRECTORY_BLOCKSIZE;
    for (iChar = 0; iChar < NAME_SIZE; iChar++, pB )
    {
        io_pDir->name[iChar] = (wchar_t)leReadUnsignedShort (pB + iChar * sizeof (CHAR2));
    }
    pOffset++;

    io_pDir->nameLength = leReadUnsignedShort (pB + *pOffset++);

    io_pDir->type = (DirEntry)*(pB + *pOffset++);
    io_pDir->BFlags= *(pB + *pOffset++);

    io_pDir->left = leReadLong (pB + *pOffset++);

    io_pDir->right = leReadLong (pB + *pOffset++);

    io_pDir->child = leReadLong (pB + *pOffset++);

    io_pDir->clsid.data1 = leReadUnsignedLong (pB + *pOffset++);
    io_pDir->clsid.data2 = leReadUnsignedShort (pB + *pOffset++);
    io_pDir->clsid.data3 = leReadUnsignedShort (pB + *pOffset++);
    memcpy (io_pDir->clsid.data4, pB + *pOffset++, sizeof(io_pDir->clsid.data4));
    io_pDir->userFlags = leReadLong (pB + *pOffset++);

    io_pDir->creationTime = leReadTime (pB + *pOffset++);

    io_pDir->lastModification = leReadTime (pB + *pOffset++);

    io_pDir->startSector = leReadUnsignedLong (pB + *pOffset++);

    /* For newer files with version >= 4, Microsoft uses 64 bits for the stream
     * length.  In older files, an unused "PropType" field and padding are
     * stored in the higher bits, and they could contain any data. */
    wMajVer = headerGetMajorVersion(rootStorageGetHeader(io_pDir->pRoot));
    if (wMajVer >= 4)
    {
        io_pDir->streamLength = leReadUnsignedLongLong (pB + *pOffset);
    }
    else
    {
        io_pDir->streamLength = leReadUnsignedLong (pB + *pOffset);
    }
    io_pDir->pSharedBlock = NULL;

    /* Detect a strange condition in which directory entries have their data
     * mangled. */
    if ((io_pDir->left < DIR_NONE) || 
        (io_pDir->right < DIR_NONE) ||
        (io_pDir->child < DIR_NONE))
    {
        return SSTG_ERROR_CORRUPTED_FAT;
    }

    return SSTG_OK;
}


/*============================================================================
  Description: Performs the lexigraphical name comparison of two directory 
               names.
  Arguments: 
   in_name0 - first name to compare
   in_name1 - second name to compare
  Return: 
   -1 if in_name0 < in_name1;
   0 if in_name0 == in_name1;
   1 if in_name0 > in_name1
  ==========================================================================*/
static SINT4
compareNames (const wchar_t* in_name0, const wchar_t *in_name1)
{
    SINT4   length0 =   wcslen (in_name0);
    SINT4   length1 =   wcslen (in_name1);
    wchar_t c0 =        0;
    wchar_t c1 =        0;
    SINT4   i =         0;

    if (length0 == length1)
    {
        for (i = 0; i < length0; i++)
        {
            c0 = towupper(in_name0[i]);
            c1 = towupper(in_name1[i]);

            if (c0 < c1)
            {
                return -1;
            }
            else if (c0 > c1)
            {
                return 1;
            }
        }
        return 0;
    }
    else if (length0 < length1)
    {
        return -1;
    }
    else /* length0 > length1 */
    {
        return 1;
    }
}


/*============================================================================
  Description:  Puts a new child entry into its proper place in the Red-Black
                graph of siblings; changes graph pointers.
  Arguments:    
   in_pParent - (this) parent of the new child
   in_pChild -  child to be inserted.
   aDir -       the directory array describing the sibling graph
  Return:       
   Status code
  ==========================================================================*/
SINT4
directoryInsertChild (DirectoryEntry* in_pParent, 
                      DirectoryEntry* in_pChild, 
                      DirectoryEntry* aDir)
{
    DirectoryEntry*     pCurrent =  NULL;
    SINT4               iCurrent =  in_pParent->child;
    SINT4               iCmp =      0;
    SINT4               iParent;

    if (in_pParent->child == DIR_NONE)
    {
        /* first child - lucky we are! */
        in_pParent->child = in_pChild->sid;
        in_pChild->BFlags = DE_BLACK; /* In the tree with one node, that node
                                       * should be black--this would normally
                                       * be done by balanceRedBlackTree() */
        return SSTG_OK;
    }

    /* Non-balanced insertion */
    while (iCurrent != DIR_NONE)
    {
        pCurrent = &aDir[iCurrent];
        iParent = iCurrent;
        iCmp = compareNames(pCurrent->name, in_pChild->name);
        if (iCmp > 0)
        {
            iCurrent = pCurrent->left;
        }
        else if (iCmp < 0)
        {
            iCurrent = pCurrent->right;
        }
        else /* iCmp == 0, a duplicate insertion */
        {
            return SSTG_ERROR_ILLEGAL_ACCESS;
        }
    }
    in_pChild->parent = pCurrent->sid;
    if (iCmp > 0)
    {
        pCurrent->left = in_pChild->sid;
    }
    else
    {
        pCurrent->right = in_pChild->sid;
    }

    /* Make the newly added node red and balance the tree */
    in_pChild->BFlags = DE_RED;
#ifdef RED_BLACK
    iParent = balanceRedBlackTree(in_pChild,
                                  &aDir[in_pParent->child],
                                  aDir);
    in_pParent->child = iParent;
#endif /* RED_BLACK */

    return SSTG_OK;
}


/*----------------RedBlack tree algorithms -------------------------*/
#ifdef RED_BLACK

/*============================================================================
  Description:  Rebalances a red-black tree after a node deletion operation.
                This function should only be called by directoryRemoveEntry.
  Arguments:    
   io_aDir -    the directory array 
   in_pParent - root entry of the red-black tree
   in_pChild -  node that replaces the one removed 
  Return:       
   Status code
  ==========================================================================*/
static SINT4 
directoryRemoveFixup(DirectoryEntry*  io_aDir,
                     DirectoryEntry** io_ppRootEntry,
                     DirectoryEntry*  in_pCurEntry)
{
    DirectoryEntry* pCur =               in_pCurEntry;
    DirectoryEntry* pParent =            NULL;
    DirectoryEntry* pSibling =           NULL;
    DirectoryEntry* pSiblingLeftChild =  NULL;
    DirectoryEntry* pSiblingRightChild = NULL;

    while (pCur != *io_ppRootEntry && pCur->BFlags == DE_BLACK)
    {
        pParent = &(io_aDir[pCur->parent]);
        if (pCur->sid == pParent->left)
        {
            pSibling = &(io_aDir[pParent->right]);
            if (pSibling->BFlags == DE_RED)
            {
                pSibling->BFlags = DE_BLACK;
                pParent->BFlags = DE_RED;
                *io_ppRootEntry = nodeLeftRotate(pParent, 
                                                 *io_ppRootEntry, 
                                                 io_aDir);
                pParent = &(io_aDir[pCur->parent]);
                pSibling = &(io_aDir[pParent->right]);
            }
            pSiblingLeftChild = &(io_aDir[pSibling->left]);
            pSiblingRightChild = &(io_aDir[pSibling->right]);
            if ((pSiblingLeftChild->BFlags == DE_BLACK) &&
                (pSiblingRightChild->BFlags == DE_BLACK))
            {
                pSibling->BFlags = DE_RED;
                pCur = pParent;
            }
            else
            {
                if (pSiblingRightChild->BFlags == DE_BLACK)
                {
                    pSiblingLeftChild->BFlags = DE_BLACK;
                    pSibling->BFlags = DE_RED;
                    *io_ppRootEntry = nodeRightRotate(pSibling, 
                                                      *io_ppRootEntry, 
                                                      io_aDir);
                    pParent = &(io_aDir[pCur->parent]);
                    pSibling = &(io_aDir[pParent->right]);
                    pSiblingRightChild = &(io_aDir[pSibling->right]);
                }
                pSibling->BFlags = pParent->BFlags;
                pParent->BFlags = DE_BLACK;
                pSiblingRightChild->BFlags = DE_BLACK;
                *io_ppRootEntry = nodeLeftRotate(pParent, 
                                                 *io_ppRootEntry, 
                                                 io_aDir);
                pCur = *io_ppRootEntry;
            }
        }
        else
        {
            pSibling = &(io_aDir[pParent->left]);
            if (pSibling->BFlags == DE_RED)
            {
                pSibling->BFlags = DE_BLACK;
                pParent->BFlags = DE_RED;
                *io_ppRootEntry = nodeRightRotate(pParent, 
                                                  *io_ppRootEntry, 
                                                  io_aDir);
                pParent = &(io_aDir[pCur->parent]);
                pSibling = &(io_aDir[pParent->left]);
            }
            pSiblingLeftChild = &(io_aDir[pSibling->left]);
            pSiblingRightChild = &(io_aDir[pSibling->right]);
            if ((pSiblingLeftChild->BFlags == DE_BLACK) &&
                (pSiblingRightChild->BFlags == DE_BLACK))
            {
                pSibling->BFlags = DE_RED;
                pCur = pParent;
            }
            else
            {
                if (pSiblingLeftChild->BFlags == DE_BLACK)
                {
                    pSiblingRightChild->BFlags = DE_BLACK;
                    pSibling->BFlags = DE_RED;
                    *io_ppRootEntry = nodeLeftRotate(pSibling,
                                                      *io_ppRootEntry, 
                                                      io_aDir);
                    pParent = &(io_aDir[pCur->parent]);
                    pSibling = &(io_aDir[pParent->left]);
                    pSiblingLeftChild = &(io_aDir[pSibling->left]);
                }
                pSibling->BFlags = pParent->BFlags;
                pParent->BFlags = DE_BLACK;
                pSiblingLeftChild->BFlags = DE_BLACK;
                *io_ppRootEntry = nodeRightRotate(pParent,
                                                 *io_ppRootEntry, 
                                                 io_aDir);
                pCur = *io_ppRootEntry;
            }
        }
    }

    pCur->BFlags = DE_BLACK;

    return SSTG_OK;
}
#endif /* RED_BLACK */


/*============================================================================
  Description:  Performs an operation equivalent to "copying" one node's key
                and record information to another.
                In reality, the links in the red-black tree are simply 
                readjusted to make the "source" node occupy the appropriate
                place in the tree.
  Arguments:    
   io_aDir -   the directory array 
   in_lDest -  the index of the destination node
   in_lSrc -   the index of the source node
   io_plRoot - pointer to the index of the root of the red-black tree
  Return:       
   Status code
  ==========================================================================*/
SINT4 
directoryCopyNode(DirectoryEntry*  io_aDir,
                  SINT4            in_lDest,
                  SINT4            in_lSrc,
                  DirectoryEntry** io_ppRootEntry)
{
    io_aDir[in_lSrc].left = io_aDir[in_lDest].left;
    io_aDir[in_lSrc].right = io_aDir[in_lDest].right;
    io_aDir[in_lSrc].parent = io_aDir[in_lDest].parent;
    io_aDir[in_lSrc].BFlags = io_aDir[in_lDest].BFlags;
    io_aDir[io_aDir[in_lDest].left].parent = in_lSrc;
    io_aDir[io_aDir[in_lDest].right].parent = in_lSrc;
    if (io_aDir[in_lDest].parent != DIR_NONE)
    {
        if (io_aDir[io_aDir[in_lDest].parent].left == in_lDest)
        {
            io_aDir[io_aDir[in_lDest].parent].left = in_lSrc;
        }
        else if (io_aDir[io_aDir[in_lDest].parent].right == in_lDest)
        {
            io_aDir[io_aDir[in_lDest].parent].right = in_lSrc;
        }
        else
        {
            /* Child points to parent, but parent doesn't point to child */
            ASSERT(0);
        }
    }
    if ((*io_ppRootEntry)->sid == in_lDest)
    {
        *io_ppRootEntry = &io_aDir[in_lSrc];
    }

    return SSTG_OK;
}
                          

/*============================================================================
  Description:  Removes a directory entry from the list.  The spot occupied 
                by this entry is simply marked as being free.  The remaining
                entries are not moved to occupy the "freed" space.
  Arguments:    
   in_pParent -     pointer to the base address of the directory array
   io_pPathParent - pointer to the "path parent" (i.e. storage containing
                    this object) of this node
   io_pToRemove -   node to remove from the directory
  Return:       
   Status code
  ==========================================================================*/
SINT4 
directoryRemoveEntry(DirectoryEntry*  io_aDir,
                     DirectoryEntry*  io_pPathParent,
                     DirectoryEntry*  io_pToRemove)
{
    DirectoryEntry* pToRemove =    io_pToRemove;
    DirectoryEntry* pToFree =      NULL;
    DirectoryEntry* pReplacement = NULL;
    DirectoryEntry* pRoot =        &io_aDir[io_pPathParent->child];
    SINT4           iRet =         SSTG_OK;
    BYTE            tmpFlags;


    ASSERT((io_pToRemove->type != DIR_NONE) &&
           (io_pToRemove->type != DIR_FREE));

    /* Figure out which node's space in the array will actually be freed. */
    if ((pToRemove->left == DIR_NONE) || (pToRemove->right == DIR_NONE))
    {
        pToFree = pToRemove;
    }
    else
    {
        /* Find a tree successor that has at most one child */
        pToFree = &(io_aDir[pToRemove->right]);
        while(pToFree->left != DIR_NONE)
        {
            pToFree = &(io_aDir[pToFree->left]);
        }
    }

    if (pToFree->left != DIR_NONE)
    {
        pReplacement = &(io_aDir[pToFree->left]);
    }
    else
    {
        pReplacement = &(io_aDir[pToFree->right]);
    } 

    pReplacement->parent = pToFree->parent;

    /* Remove pToFree from the parent chain.
     * The following test is to see if pToFree is not the root of the 
     * red-black tree. */
    if (pToFree->parent != DIR_NONE)
    {
        if (pToFree->sid == io_aDir[pToFree->parent].left)
        {
            io_aDir[pToFree->parent].left = pReplacement->sid;
        }
        else
        {
            ASSERT(pToFree->sid == io_aDir[pToFree->parent].right);
            io_aDir[pToFree->parent].right = pReplacement->sid;
        }
    }
    else
    {
        pRoot = pReplacement;
    }

    /* If pToFree is not the same as pToRemove, "move" pToFree's data into
     * the space previously occupied by pToRemove. */
    tmpFlags = pToFree->BFlags;
    if (pToRemove != pToFree)
    {
        iRet = directoryCopyNode(io_aDir, 
                                 pToRemove->sid, 
                                 pToFree->sid, 
                                 &pRoot);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

#ifdef RED_BLACK
    /* Rebalance the tree if necessary */
    if (tmpFlags == DE_BLACK)
    {
        iRet = directoryRemoveFixup(io_aDir, &pRoot, pReplacement);
    }
#endif /* RED_BLACK */

    directoryConstructEntry(pToRemove->sid, pToRemove);
    pToRemove->type = DIR_FREE;
    
    io_pPathParent->child = pRoot->sid;

#ifdef DEBUG  
    directoryAssertBSTInvariants(io_aDir, pRoot, NULL, NULL);
#endif /* DEBUG */

    return iRet;
}


#ifdef RED_BLACK
/*============================================================================
  Description:  Balances the tree which might loose its Red-Black properties
                after addition of a new node. Should be called only from
                directoryInsertChild.
  Arguments:    
   in_pNew -  new node added
   in_pRoot - root of the curret Red-Black tree.  Can change during this 
              operation
   aDir -     the directory array describing the sibling graph
  Return:  
   SID for the new root of the tree
  ==========================================================================*/
static SINT4
balanceRedBlackTree(DirectoryEntry* in_pNew, DirectoryEntry* in_pRoot, DirectoryEntry* aDir)
{
    DirectoryEntry* pChild =    in_pNew;
    DirectoryEntry* pCurrent =  &aDir[pChild->parent];
    DirectoryEntry* pParent =   NULL;
    DirectoryEntry* py =        NULL;

    while ((pChild != in_pRoot) &&
           (pCurrent->BFlags == DE_RED))
    {
        pParent = &aDir[pCurrent->parent];

        /* When we are working with the left child... */
        if (pCurrent->sid == pParent->left)
        {
            py = &aDir[pParent->right];
            if (py->BFlags == DE_RED)
            {
                /* case 1: Node's uncle is red */
                pCurrent->BFlags = DE_BLACK;
                py->BFlags = DE_BLACK;
                pParent->BFlags = DE_RED;
                pCurrent = &aDir[pParent->parent];
                pChild = pParent;
            }
            else
            {
                /* case 2 and 3 */
                if (pChild->sid == pCurrent->right)
                {
                    /* Case 2: we inserted the child at right.
                    Rotate it so we'll get case 3 */
                    pChild = pCurrent;
                    in_pRoot = nodeLeftRotate (pCurrent, in_pRoot, aDir);
                    pCurrent = &aDir[pChild->parent];
                    pParent = &aDir[pCurrent->parent];
                }

                /* case 3 */
                pCurrent->BFlags = DE_BLACK;
                if (pParent->sid != DIR_NONE)
                {
                    pParent->BFlags = DE_RED;
                    in_pRoot = nodeRightRotate(pParent, in_pRoot, aDir);
                }

            }
        }
        /* Symmetrical case - now we are attaching a new node to a right child */
        else
        {
            py = &aDir[pParent->left];
            if (py->BFlags == DE_RED)
            {
                /* case 1: Node's uncle is red */
                pCurrent->BFlags = DE_BLACK;
                py->BFlags = DE_BLACK;
                pParent->BFlags = DE_RED;
                pCurrent = &aDir[pParent->parent];
                pChild = pParent;
            }
            else
            {
                /* case 2 and 3 */
                if (pChild->sid == pCurrent->left)
                {
                    /* Case 2: we inserted the child at left.
                    Rotate it so we'll get case 3 */
                    pChild = pCurrent;
                    in_pRoot = nodeRightRotate(pCurrent, in_pRoot, aDir);
                    pCurrent = &aDir[pChild->parent];
                    pParent = &aDir[pCurrent->parent];
                }
                /* case 3 */
                pCurrent->BFlags = DE_BLACK;
                if (pParent->sid != DIR_NONE)
                {
                    pParent->BFlags = DE_RED;
                    in_pRoot = nodeLeftRotate(pParent, in_pRoot, aDir);
                }
            }
        }
    }

    in_pRoot->BFlags = DE_BLACK;

    return in_pRoot->sid;
}


/*============================================================================
  Description: Left rotation: node's right child takes original node
               position, the original node becomes its left child. Node's 
               right-left grandchild becomes its right child. Necessary changes 
               in parent, left, and right slots are made.
  Note:
   The node MUST have a right child.
  Arguments: 
   in_pNode - node to rotate
   in_pRoot - the root of the tree on which we operate (it might be changed!)
   aDir     - array of DirectoryEntries describing the tree
  Return: 
   pRet - the pointer to the curent tree root (could change due to rotation)
==============================================================================*/
static DirectoryEntry*
nodeLeftRotate(DirectoryEntry* in_pNode,
               DirectoryEntry* in_pRoot,
               DirectoryEntry* aDir)
{
    DirectoryEntry* pRet = in_pRoot;
    DirectoryEntry* py;        /* node's right child */
    DirectoryEntry* pParent;   /* node's parent */
    DirectoryEntry* pRight;    /* right child's left child - to become node's
                                * new right child */

    ASSERT (in_pRoot != NULL);
    ASSERT(in_pNode->right != DIR_NONE);

    py = &(aDir[in_pNode->right]);

    in_pNode->right = py->left;
    if (in_pNode->right != DIR_NONE)
    {
        pRight = &(aDir[in_pNode->right]);
        if (pRight->sid != DIR_NONE)
            pRight->parent = in_pNode->sid;
    }

    py->parent = in_pNode->parent;
    if (in_pNode->parent != DIR_NONE)
    {
        pParent = &(aDir[in_pNode->parent]);
        if (in_pNode->sid == pParent->left)
        {
            pParent->left = py->sid;
        }
        else
        {
            pParent->right = py->sid;
        }
    }
    else
    {
        pRet = py;
    }
    py->left = in_pNode->sid;
    in_pNode->parent = py->sid;

    return pRet;
}


/*============================================================================
  Description: Right rotation: node's left child takes original node
               position, the original node becomes its right child. Node's 
               left-right grandchild becomes its left child. 
               Necessary changes in parent, left, and right slots are made.
  Arguments: 
   in_pNode - node to rotate
   in_pRoot - the root of the tree on which we operate (it might be changed!)
   aDir     - array of DirectoryEntries describing the tree
  Note:
   The node MUST have a left child.
  Return: 
   ret - the pointer to the node that now serves as a root for the tree
==============================================================================*/
static DirectoryEntry*
nodeRightRotate(DirectoryEntry* in_pNode,
                DirectoryEntry* in_pRoot,
                DirectoryEntry* aDir)
{
    DirectoryEntry* pRet = in_pRoot;
    DirectoryEntry* px;        /* node's left child */
    DirectoryEntry* pParent;   /* node's parent */
    DirectoryEntry* pLeft;     /* left child's right child - to become node's
                                * new left child */

    ASSERT (in_pRoot != NULL);
    ASSERT(in_pNode->left != DIR_NONE);

    px = &(aDir[in_pNode->left]);

    in_pNode->left = px->right;
    pLeft = &(aDir[in_pNode->left]);
    if (pLeft->sid != DIR_NONE)
        pLeft->parent = in_pNode->sid;

    px->parent = in_pNode->parent;
    if (in_pNode->parent != DIR_NONE)
    {
        pParent = &(aDir[in_pNode->parent]);
        if (in_pNode->sid == pParent->left)
        {
            pParent->left = px->sid;
        }
        else
        {
            pParent->right = px->sid;
        }
    }
    else
    {
        pRet = px;
    }
    px->right = in_pNode->sid;
    in_pNode->parent = px->sid;

    return pRet;
}
#endif /* RED_BLACK */


/*============================================================================
  Description: Sets the size of the stream represented by a directory entry
  Arguments: 
   io_pDir - directory entry to modify
   in_lNewSize - the new size of the stream
  Return: 
   Status code
==============================================================================*/
SINT4
directorySetSize(DirectoryEntry* io_pDir, UINT8 in_llNewSize)
{
    io_pDir->streamLength = in_llNewSize;
    return SSTG_OK;
}


/*============================================================================
  Description: Sets the start sector of the stream represented by a directory
               entry
  Arguments: 
   io_pDir - directory entry to modify
   in_lNewSize - the new start sector of the stream
  Return: 
   Status code
==============================================================================*/
SINT4
directorySetStartSector(DirectoryEntry* io_pDir, UINT4 in_lNewStartSector)
{
    io_pDir->startSector = in_lNewStartSector;
    return SSTG_OK;
}


/*============================================================================
  Description: Sets the creation time of a directory entry to be NOW
  Arguments: 
   io_pDir - directory entry to modify
  Return: 
   Status code
==============================================================================*/
SINT4
directorySetCurrentCreationTime(DirectoryEntry* io_pDir)
{
    SsrwTime (&(io_pDir->creationTime));
    return SSTG_OK;
}


/*============================================================================
  Description: Sets the "last modification" time of a directory entry to be NOW
  Arguments: 
   io_pDir - directory entry to modify
  Return: 
   Status code
==============================================================================*/
SINT4
directorySetCurrentModificationTime(DirectoryEntry* io_pDir)
{
    SsrwTime (&(io_pDir->lastModification));
    return SSTG_OK;
}


/*============================================================================
  Description: Invalidates a directory entry that has already been created.
  Arguments: 
   io_pDir - directory entry to invalidate
  Return: 
   Status code
==============================================================================*/
SINT4
directoryInvalidate(DirectoryEntry* io_pDir)
{
    io_pDir->sid = DIR_NONE;
    io_pDir->parent = DIR_NONE;
    io_pDir->left = DIR_NONE;
    io_pDir->right = DIR_NONE;
    io_pDir->child = DIR_NONE;
    io_pDir->type = DIR_FREE;

    return SSTG_OK;
}


/*============================================================================
  Description:  Gets a pointer to a stream's shared block and increments the
                internal reference count.  If a shared block does not already 
                exist, creates a new one and sets the reference count to 1.
  Arguments: 
   io_pDir - directory entry to query for the shared block
   out_ppSharedBlock - on success, will point to a pointer to the new shared
                       block
  Return: 
   Status code
==============================================================================*/
SINT4 
directoryQueryStreamSharedBlock(DirectoryEntry* io_pDir, 
                                StreamSharedBlock** out_ppSharedBlock)
{
    SINT4 iRet =        SSTG_OK;
    UINT4 ulRefCount =  0;
        
    ASSERT(io_pDir != NULL);
    ASSERT(out_ppSharedBlock != NULL);
    ASSERT((io_pDir->type == DIR_STREAM) || (io_pDir->type == DIR_ROOT));
    ASSERT((io_pDir->pSharedBlock == NULL) ||
           (streamSharedBlockGetRefCount(io_pDir->pSharedBlock) > 0));

    /* If the shared block already exists, increment the refcount */
    if (io_pDir->pSharedBlock != NULL)
    {
        ulRefCount = streamSharedBlockGetRefCount(io_pDir->pSharedBlock);
        ulRefCount++;
        streamSharedBlockSetRefCount(io_pDir->pSharedBlock, ulRefCount);
        *out_ppSharedBlock = io_pDir->pSharedBlock;
        return SSTG_OK;
    }

    /* Construct a new one */
    iRet = streamConstructSharedBlock(&(io_pDir->pSharedBlock));
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    *out_ppSharedBlock = io_pDir->pSharedBlock;
    streamSharedBlockSetRefCount(io_pDir->pSharedBlock, 1);

    return SSTG_OK;
}


/*============================================================================
  Description:  Decrements the reference count of a stream shared block.
                If the reference count reaches 0, the shared block is freed.
  Arguments: 
   io_pDir - directory entry to update
  Return: 
   Status code
==============================================================================*/
SINT4 
directoryReleaseStreamSharedBlock(DirectoryEntry* io_pDir)
{
    SINT4 iRet =        SSTG_OK;
    UINT4 ulRefCount =  0;

    ASSERT(io_pDir != NULL);
    ASSERT((io_pDir->type == DIR_STREAM) || (io_pDir->type == DIR_ROOT));
    ASSERT((io_pDir->pSharedBlock == NULL) ||
           (streamSharedBlockGetRefCount(io_pDir->pSharedBlock) > 0));

    ulRefCount = streamSharedBlockGetRefCount(io_pDir->pSharedBlock);
    ulRefCount--;
    if (ulRefCount == 0)
    {
        iRet = streamDestroySharedBlock(&(io_pDir->pSharedBlock));
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }
    else
    {
        streamSharedBlockSetRefCount(io_pDir->pSharedBlock, ulRefCount);
    }

    return SSTG_OK;
}


#ifndef SSRW_READ_ONLY

/*============================================================================
  Description: Sets the CLSID of the storage or stream represented by a 
               directory entry
  Arguments:   
   io_pDir - directory entry to modify
   in_pCLSID - pointer to the new CLSID for the storage or stream.  
               Makes a copy.  Must be non-NULL.
  Return:       
   Status code
  ==========================================================================*/
SINT4
directorySetClass(DirectoryEntry* io_pDir, const SSRW_CLSID* in_pCLSID)
{
    memcpy(&(io_pDir->clsid), in_pCLSID, CLSID_SIZE);
    return SSTG_OK;
}


/*============================================================================
  Description:  Writes a Directory Entry strcuture into a buffer at certain
                access. The buffer is ready to be flushed into a file.
  Arguments:   
   in_pEntry - an entry to write
   in_buffer - a buffer to write to
   io_offset - pointer to a buffer to start writing. Increases by 128 at the end
  Return:       
   Status code
  ==========================================================================*/
SINT4
directoryEntryWrite(DirectoryEntry* in_pEntry,
                    CHAR* in_buffer,
                    SINT4* io_poffset)
{
    SINT4       iChar;
    CHAR*       pB =            in_buffer + *io_poffset;
    SINT4       offsetArray[] = FIELD_OFFSETS;
    SINT4*      pOffset =       offsetArray;

    for (iChar = 0; iChar < NAME_SIZE; iChar++, pB )
    {
        leWriteUnsignedShort((UINT2)in_pEntry->name[iChar],(pB + iChar * sizeof (CHAR2)));
    }
    pOffset++;

    leWriteUnsignedShort(in_pEntry->nameLength,(pB + *pOffset++) );

    *(pB + *pOffset++) = (CHAR)in_pEntry->type;
    *(pB + *pOffset++) = in_pEntry->BFlags;

    leWriteLong(in_pEntry->left, (pB + *pOffset++));
    leWriteLong(in_pEntry->right, (pB + *pOffset++));
    leWriteLong(in_pEntry->child, (pB + *pOffset++));

    leWriteUnsignedLong(in_pEntry->clsid.data1, (pB + *pOffset++));
    leWriteUnsignedShort(in_pEntry->clsid.data2, (pB + *pOffset++));
    leWriteUnsignedShort(in_pEntry->clsid.data3, (pB + *pOffset++));
    memcpy ( pB + *pOffset++,
             in_pEntry->clsid.data4,
             sizeof(in_pEntry->clsid.data4));

    leWriteLong(in_pEntry->userFlags, (pB + *pOffset++));
    leWriteTime(in_pEntry->creationTime, (pB + *pOffset++));
    leWriteTime(in_pEntry->lastModification, (pB + *pOffset++));
    leWriteUnsignedLong(in_pEntry->startSector, (pB + *pOffset++));

    leWriteUnsignedLongLong(in_pEntry->streamLength, (pB + *pOffset));

    /* Note older documentation describes an "unsupported" PropType field at
     * offset 124, but it appears that newer files use the last four bytes as
     * the high dword of a 64-bit stream length field.  We always write a
     * 64-bit field, assuming that older readers will ignore the PropType and 
     * padding safely.
     */

    /*rootStorageReleaseLock (in_pEntry->pRoot);*/
    (*io_poffset) += DIRECTORY_BLOCKSIZE;
    return SSTG_OK;
}


#ifdef DEBUG

#include "Fat.h"

/*============================================================================
  Description:  (DEBUG only) Recurses through the directory tree, blanking FAT 
                sectors for all streams and the root directory.  This is for
                a special debug-only self-check.
  Arguments:    
   in_aDir -             start entry of directory array
   in_curIndex -         index of directory entry currently being considered
   io_pulFat -           array of regular FAT sectors
   io_ulFatlength -      length of io_pulFat
   io_pulSmallFat -      array of small FAT sectors
   io_ulSmallFatLength - length of io_pulSmallFat
  Return:       
   Status code
  ==========================================================================*/
UINT4
directoryBlankFatSectors(DirectoryEntry* in_aDir, 
                         UINT4 in_ulCurIndex, 
                         UINT4* io_pulFat, 
                         UINT4 in_ulFatLength,
                         UINT4* io_pulSmallFat, 
                         UINT4 in_ulSmallFatLength)
{
    boolean isStillOpen = SSRW_FALSE;
    UINT4   cbCutoff =    0;
    UINT4*  pulFat =      NULL;
    UINT4   ulFatLen =    0;
    SINT4   iRet =        SSTG_OK;

    cbCutoff = headerGetSmallStreamCutoff(
        rootStorageGetHeader(in_aDir[0].pRoot));

    /* Storages don't take up space in the FAT until the table of contents
     * is written when the structured storage file is closed */
    if ((in_aDir[in_ulCurIndex].type == DIR_STREAM) ||
        (in_aDir[in_ulCurIndex].type == DIR_ROOT))
    {
        /* It is permitted to have 0-byte streams; skip these */
        if (in_aDir[in_ulCurIndex].streamLength > 0)
        {
            /* First, try to detect if the stream is still open */
            if ((in_aDir[in_ulCurIndex].type == DIR_STREAM) &&
                (in_aDir[in_ulCurIndex].pSharedBlock != NULL))
            {
                isStillOpen = SSRW_TRUE;
            }

            if ((in_aDir[in_ulCurIndex].streamLength >= cbCutoff) || 
                (in_aDir[in_ulCurIndex].type == DIR_ROOT) ||
                isStillOpen)
            {
                /* Blank the regular FAT */
                pulFat = io_pulFat;
                ulFatLen = in_ulFatLength;
            }
            else
            {
                /* Blank the mini FAT */
                pulFat = io_pulSmallFat;
                ulFatLen = in_ulSmallFatLength;
            }

            iRet = fatBlankChain(pulFat, 
                                 ulFatLen, 
                                 in_aDir[in_ulCurIndex].startSector);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
    }

    if (in_aDir[in_ulCurIndex].left != DIR_NONE)
    {
        iRet = directoryBlankFatSectors(in_aDir, 
            in_aDir[in_ulCurIndex].left, 
            io_pulFat,
            in_ulFatLength,
            io_pulSmallFat,
            in_ulSmallFatLength);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    if (in_aDir[in_ulCurIndex].right != DIR_NONE)
    {
        iRet = directoryBlankFatSectors(in_aDir, 
            in_aDir[in_ulCurIndex].right, 
            io_pulFat,
            in_ulFatLength,
            io_pulSmallFat,
            in_ulSmallFatLength);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    if (in_aDir[in_ulCurIndex].child != DIR_NONE)
    {
        iRet = directoryBlankFatSectors(in_aDir, 
            in_aDir[in_ulCurIndex].child, 
            io_pulFat,
            in_ulFatLength,
            io_pulSmallFat,
            in_ulSmallFatLength);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    return SSTG_OK;
}


/*============================================================================
  Description:  (DEBUG only) Calculates the "black height" of a red-black tree,
                which is the number of black nodes on any simple path from any 
                leaf to the root node.
                "Nil" nodes are counted as black; conceptually the tree has
                pointers to nil nodes instead of NULL pointers.
                This function is recursive.
  Arguments:   
   in_aDir -      the base pointer to the directory array.  The "nil" node 
                  actually directly precedes this pointer in memory.
   in_pCurEntry - the root entry of the tree to analyse
  Return:       
   Status code
  ==========================================================================*/
SINT4
directoryCalcBlackHeight(const DirectoryEntry* in_aDir, 
                         const DirectoryEntry* in_pCurEntry)
{
    SINT4                 lLeftBlackHeight =  0;
    SINT4                 lBlackHeight =      0;
    const DirectoryEntry* pLeftChild =        NULL;

    /* Base case.  An entry that has the id DIR_NONE is a "nil" entry,
     * a leaf.  It is coloured black, so return 1. */
    if (in_pCurEntry->sid == DIR_NONE)
    {
        return 1;
    }

    /* Calculate the left child's black height.  Here we explicitly assume that
     * the right child and left child have the same black height. */
    pLeftChild = &in_aDir[in_pCurEntry->left];
    lLeftBlackHeight = directoryCalcBlackHeight(in_aDir, pLeftChild);

    /* Calculate this node's black height */
    lBlackHeight = lLeftBlackHeight;
    if (in_pCurEntry->BFlags == DE_BLACK)
    {
        lBlackHeight++;
    }

    return lBlackHeight;
}


/*============================================================================
  Description:  (DEBUG only) Traverses a binary search tree, ASSERTing 
                everywhere that the tree is a binary search tree.  
                Loosely, this means that each node in the subtree rooted at the 
                left child of a node must have lesser names than the original 
                node, while each node in the subtree rooted at the right child 
                must have names that are greater.
                This function is recursive.
  Arguments:   
   in_aDir -      the base pointer to the directory array.  The "nil" node 
                  actually directly precedes this pointer in memory.
   in_pCurEntry - the root entry of the tree to analyse
  Return:       
   Status code
  ==========================================================================*/
SINT4
directoryAssertBSTInvariants(const DirectoryEntry* in_aDir,
                             const DirectoryEntry* in_pCurEntry,
                             const wchar_t* in_pLeftBoundingName,
                             const wchar_t* in_pRightBoundingName)
{
    SINT4 iRet = SSTG_OK;

    /* The nil node trivially meets the definition of a binary search tree */
    if (in_pCurEntry->sid == DIR_NONE)
    {
        return 0;
    }

    /* Test the current node's name to see if it falls outside the bounding
     * names.  If so, assert and return an error. */
    if ((in_pLeftBoundingName != NULL) &&
        (compareNames(in_pCurEntry->name, in_pLeftBoundingName) < 0))
    {
        ASSERT(0);
        return 1;
    }
    if ((in_pRightBoundingName != NULL) &&
        (compareNames(in_pCurEntry->name, in_pRightBoundingName) > 0))
    {
        ASSERT(0);
        return 1;
    }

    /* Recurse to each child, using this node's name as a new bound on one 
     * side. */
    iRet = directoryAssertBSTInvariants(in_aDir, 
                                        &in_aDir[in_pCurEntry->left], 
                                        in_pLeftBoundingName,
                                        in_pCurEntry->name);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    iRet = directoryAssertBSTInvariants(in_aDir, 
                                        &in_aDir[in_pCurEntry->right], 
                                        in_pCurEntry->name,
                                        in_pRightBoundingName);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    return SSTG_OK;
}


/*============================================================================
  Description:  (DEBUG only) Traverses a red-black tree, ASSERTing everywhere
                that the tree is both a binary search tree and a valid red-
                black tree.  This function is recursive.
  Arguments:   
   in_aDir -      the base pointer to the directory array.  The "nil" node 
                  actually directly precedes this pointer in memory.
   in_pCurEntry - the root entry of the tree to analyse
  Return:       
   Status code
  ==========================================================================*/
SINT4
directoryAssertRedBlackInvariants(const DirectoryEntry* in_aDir,
                                  const DirectoryEntry* in_pCurEntry)
{
#ifdef RED_BLACK
    SINT4                 lLeftBlackHeight =  0;
    SINT4                 lRightBlackHeight = 0;
#endif /* RED_BLACK */
    const DirectoryEntry* pLeftChild =        NULL;
    const DirectoryEntry* pRightChild =       NULL;
    SINT4                 iRet =              SSTG_OK;

    /* If this is the degenerate case, return OK */
    if (in_pCurEntry->sid == DIR_NONE)
    {
        return SSTG_OK;
    }

    /* Get pointers to the child nodes */
    pLeftChild = &in_aDir[in_pCurEntry->left];
    pRightChild = &in_aDir[in_pCurEntry->right];

    /* Assert the red-black invariants on each child */
    iRet = directoryAssertRedBlackInvariants(in_aDir, pLeftChild);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    directoryAssertRedBlackInvariants(in_aDir, pRightChild);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

#ifdef RED_BLACK
    /* Calculate the right child's black height */
    lLeftBlackHeight = directoryCalcBlackHeight(in_aDir, pLeftChild);

    /* Calculate the right child's black height */
    lRightBlackHeight = directoryCalcBlackHeight(in_aDir, pRightChild);

    /* Assert that the black heights are the same */
    ASSERT(lLeftBlackHeight == lRightBlackHeight);

    /* Assert that if this node is red, then both its children are black */
    ASSERT((in_pCurEntry->BFlags != DE_RED) || 
            (pLeftChild->BFlags == DE_BLACK));
    ASSERT((in_pCurEntry->BFlags != DE_RED) || 
            (pRightChild->BFlags == DE_BLACK));
#endif /* RED_BLACK */

    return iRet;
}


/*============================================================================
  Description:  (DEBUG only) Asserts that directory entries remain consistent
                internally
  Arguments:   
   in_aDir -       the base pointer to the directory array.  The "nil" node 
                   actually directly precedes this pointer in memory.
   in_lArraySize - the number of items in the array
  Return:       
   Status code
  ==========================================================================*/
SINT4
directoryAssertEntriesConsistent(const DirectoryEntry* in_aDir, SINT4 in_lArraySize)
{
    SINT4 i = 0;

    /* Make assertions about the special "DIR_NONE" entry at index -1 */
    ASSERT(in_aDir[DIR_NONE].type == DIR_FREE);
    ASSERT(in_aDir[DIR_NONE].BFlags == DE_BLACK);
    ASSERT(in_aDir[DIR_NONE].left == DIR_NONE);
    ASSERT(in_aDir[DIR_NONE].right == DIR_NONE);
    ASSERT(in_aDir[DIR_NONE].child == DIR_NONE);

    /* Make assertions about other directory entries */
    for (i = 0; i < in_lArraySize; i++)
    {
        /* Free entries are exempt from checks */
        if ((in_aDir[i].type == DIR_NONE) ||
            (in_aDir[i].type == DIR_FREE))
        {
            continue;
        }

        /* Assert that the index of the entry is consistent */
        ASSERT(in_aDir[i].sid == i);

        /* Make sure all child pointers are either DIR_NONE or
         * point to proper storages or streams */
        ASSERT((in_aDir[i].left == DIR_NONE) ||
               ((in_aDir[in_aDir[i].left].type == DIR_STORAGE) ||
                (in_aDir[in_aDir[i].left].type == DIR_STREAM)))
        ASSERT((in_aDir[i].right == DIR_NONE) ||
               ((in_aDir[in_aDir[i].right].type == DIR_STORAGE) ||
                (in_aDir[in_aDir[i].right].type == DIR_STREAM)))
    }
    return SSTG_OK;
}


/*============================================================================
  Description:  (DEBUG only) Runs a series of self-tests on the directory 
                insertion and deletion code, ensuring that the properties
                of a red-black binary search tree are preserved after each
                operation.
  Return:       
   Status code
  ==========================================================================*/
SINT4
directorySelfTest(void)
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
    DirectoryEntry* pTOC =          NULL;
    DirectoryEntry* pTOCTmp =       NULL;
    SINT4           iRet =          0;
    SINT4           i =             0;
    SINT4           removeOrder[] =     { 19,  1, 11, 12, 10, 13,  9, 18,  2,  3, 
                                      4,   5, 17, 16,  6, 15,  7,  8, 14 };
                               

    /* Setup a fake table of contents. */
    pTOCTmp = (DirectoryEntry*)malloc(
        sizeof(DirectoryEntry) * (testNameCount + 2)); 
    pTOC = pTOCTmp + 1;
    iRet = directoryConstructNilEntry(pTOCTmp);
    ASSERT(iRet == SSTG_OK);
    iRet = directoryConstructEntry(i, &pTOC[0]);
    ASSERT(iRet == SSTG_OK);
    iRet = directoryInitEntry (&pTOC[0],
                               L"Root Entry",
                               DIR_ROOT,
                               NULL);
    ASSERT(iRet == SSTG_OK);

    /* Add directory entries with the names individually */
    for(i = 1; i < testNameCount + 1; i++)
    {
        iRet = directoryConstructEntry(i, &pTOC[i]);
        ASSERT(iRet == SSTG_OK);

        iRet = directoryInitEntry(&pTOC[i], 
                                  testNames[i - 1], 
                                  DIR_STREAM, 
                                  NULL);
        ASSERT(iRet == SSTG_OK);
    }

    /* Insert all streams into the root storage, one by one */
    for (i = 1; i < testNameCount + 1; i++)
    {
        iRet = directoryInsertChild(&pTOC[0], &pTOC[i], pTOC);
        ASSERT(iRet == SSTG_OK);

        /* Assert that the directory entries remain internally consistent */
        iRet = directoryAssertEntriesConsistent(pTOC, testNameCount + 1);
        ASSERT(iRet == SSTG_OK);

        /* Assert that the tree remains a binary search tree */
        iRet = directoryAssertBSTInvariants(pTOC, &pTOC[pTOC[0].child], NULL, NULL);
        ASSERT(iRet == SSTG_OK);

        /* Assert that the tree's red-black properties remain */
        iRet = directoryAssertRedBlackInvariants(pTOC, &pTOC[pTOC[0].child]);
        ASSERT(iRet == SSTG_OK);
    }

    /* Remove the entries again in a predetermined order */
    for (i = 0; i < testNameCount; i++)
    {
         iRet = directoryRemoveEntry(pTOC, &pTOC[0], &pTOC[removeOrder[i]]);
        ASSERT(iRet == SSTG_OK);

        /* Assert that the directory entries remain internally consistent */
        iRet = directoryAssertEntriesConsistent(pTOC, testNameCount + 1);
        ASSERT(iRet == SSTG_OK);

        /* Assert that the tree remains a binary search tree */
        iRet = directoryAssertBSTInvariants(pTOC, &pTOC[pTOC[0].child], NULL, NULL);
        ASSERT(iRet == SSTG_OK);

        /* Assert that the tree's red-black properties remain */
        directoryAssertRedBlackInvariants(pTOC, &pTOC[pTOC[0].child]);
        ASSERT(iRet == SSTG_OK);
    }
                             
    /* A specific test: make sure the root is replaced correctly after removal */
    iRet = directoryConstructEntry(1, &pTOC[1]);
    ASSERT(iRet == SSTG_OK);
    iRet = directoryInitEntry(&pTOC[1], L"B", DIR_STREAM, NULL);
    ASSERT(iRet == SSTG_OK);
    iRet = directoryConstructEntry(2, &pTOC[2]);
    ASSERT(iRet == SSTG_OK);
    iRet = directoryInitEntry(&pTOC[2], L"A", DIR_STREAM, NULL);
    ASSERT(iRet == SSTG_OK);
    iRet = directoryConstructEntry(3, &pTOC[3]);
    ASSERT(iRet == SSTG_OK);
    iRet = directoryInitEntry(&pTOC[3], L"C", DIR_STREAM, NULL);
    ASSERT(iRet == SSTG_OK);
    for (i= 1; i < 4; i++)
    {
        iRet = directoryInsertChild(&pTOC[0], &pTOC[i], pTOC);
        ASSERT(iRet == 0);
    }
    ASSERT(pTOC[0].child == 1);
    ASSERT(wcscmp(pTOC[pTOC[0].child].name, L"B") == 0);
    ASSERT(pTOC[pTOC[0].child].left == 2);
    ASSERT(wcscmp(pTOC[pTOC[pTOC[0].child].left].name, L"A") == 0);
    ASSERT(pTOC[pTOC[0].child].right == 3);
    ASSERT(wcscmp(pTOC[pTOC[pTOC[0].child].right].name, L"C") == 0);
    iRet = directoryRemoveEntry(pTOC, &pTOC[0], &pTOC[1]);
    ASSERT(iRet == SSTG_OK);
    ASSERT((pTOC[0].child == 2) || (pTOC[0].child == 3));
    iRet = directoryRemoveEntry(pTOC, &pTOC[0], &pTOC[2]);
    ASSERT(iRet == SSTG_OK);
    ASSERT(pTOC[0].child == 3);
    iRet = directoryRemoveEntry(pTOC, &pTOC[0], &pTOC[3]);
    ASSERT(iRet == SSTG_OK);
    ASSERT(pTOC[0].child == DIR_NONE);

    /* Cleanup */
    free(pTOCTmp);
    return SSTG_OK;
}

#endif /* DEBUG */

#endif /* !SSRW_READ_ONLY */

