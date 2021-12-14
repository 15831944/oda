/************************************************************************
* Stream.c
*
* Contains code for managing open streams in a structured storage
*
* (c) Schema Software Inc., 2001-2004
*
*************************************************************************

$Revision: 1.22.2.5 $
$Date: 2004/07/28 21:44:37 $
*/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "StructuredStorage.h"
#include "RootStorage.h"
#include "Header.h"
#include "Directory.h"
#include "Fat.h"
#include "Storage.h"
#include "LEReadWrite.h"
#include "Stream.h"
#include "TOC.h"

#define CHILD_BLOCKSIZE   8

#define NO_SECTOR_WRITTEN ((UINT4)-1)

struct _StreamSharedBlock
{
    UINT8 ullSize;                   /* Size (in bytes) of this stream */

    UINT4* pulSectorChain;           /* Copy of FAT chain for this stream */
    UINT4 ulSectorChainLen;          /* Length of FAT chain for this stream */
    UINT4 ulSectorChainAlloc;        /* Number of slots allocated in the sector
                                      * chain array.  ulSectorChainLen <= 
                                      * ulSectorChainAlloc. */
    Stream* pMiniStream;             /* For small streams, points to another
                                      * Stream structure representing the
                                      * MiniStream */
    BYTE isStreamShouldBeMiniKnown;
    BYTE isStreamShouldBeMini;
    UINT4 ulRefCount;                /* Number of Streams that have pointers to
                                      * this structure */
};

struct _Stream
{
    RootStorage* pRoot;              /* pointer to root storage */
    UINT4 ulDirEntry;                /* index of directory entry */
    Storage* pParent;                /* pointer to parent storage of this
                                      * stream */

    UINT8 ullSeekPointer;            /* Current seek location inside this
                                      * stream */
    BYTE* pbSectorBuf;               /* points to buffer containing unflushed
                                      * contents of this stream */
    UINT4 cbInSectorBuf;             /* number of bytes in pbSectorBuf */
    UINT4 ulBufferedSector;          /* index of sector represented by 
                                      * pbSectorBuf */

    StgMode mode;                    /* mode in which this stream was opened */

    StreamSharedBlock* pS;           /* block of data shared by all Stream 
                                      * structs that point to the same 
                                      * physical stream */
};


/* forward declarations */
static SINT4 streamFlushToDisk(Stream* in_pStream);
static SINT4 convertToSmallStream(Stream* io_pStream);

#ifdef DEBUG
static SINT4 assertSectorChainIntegrity(Stream* in_pStream);
#endif /* DEBUG */

/*============================================================================
  Description:  Sets an entry in the sector chain.  Grows the sector chain 
                buffer if necessary.  The buffer is doubled, rather than 
                incrementally grown, so as to avoid constant reallocs.
  Arguments: 
   io_pStream - pointer to stream structure 
   in_ulIndex - index of entry to set
   in_ulValue - value to set

  Return:       
   Status code
  ==========================================================================*/
static SINT4
streamSetSectorChainEntry(Stream* io_pStream, UINT4 in_ulIndex, UINT4 in_ulValue)
{
    UINT4  ulNewSizeBytes =    0;
    UINT4* pulNewSectorChain = NULL;

    ASSERT(io_pStream->pS->ulSectorChainLen <= io_pStream->pS->ulSectorChainAlloc);

    if (in_ulIndex >= io_pStream->pS->ulSectorChainAlloc)
    {
        if (io_pStream->pS->ulSectorChainAlloc == 0)
        {
            ASSERT(io_pStream->pS->pulSectorChain == NULL);
            /* Start at 512 bytes, more or less arbitrarily */
            ulNewSizeBytes = 512;
        }
        else if (io_pStream->pS->ulSectorChainAlloc & 0x80000000)
        {
            /* If the sector chain is already very large, ensure that the size 
             * doesn't wrap past 0 */
            ulNewSizeBytes = 0xffffffff;
        }
        else
        {
            /* Double the existing size of the FAT */
            ulNewSizeBytes = (io_pStream->pS->ulSectorChainAlloc * 2) * 
                             sizeof(UINT4);
        }

        if (io_pStream->pS->pulSectorChain == NULL)
        {
            pulNewSectorChain = (UINT4*)malloc(ulNewSizeBytes);
        }
        else
        {
            pulNewSectorChain = (UINT4*)realloc(io_pStream->pS->pulSectorChain, 
                                                ulNewSizeBytes);
        }

        if (pulNewSectorChain == NULL)
        {
            return SSTG_ERROR_MEMORY;
        }

        io_pStream->pS->pulSectorChain = pulNewSectorChain;
        io_pStream->pS->ulSectorChainAlloc = ulNewSizeBytes / sizeof(UINT4);
    }

    if (in_ulIndex >= io_pStream->pS->ulSectorChainLen)
    {
        ASSERT(in_ulIndex == io_pStream->pS->ulSectorChainLen);

        io_pStream->pS->ulSectorChainLen = in_ulIndex + 1;
    }

    io_pStream->pS->pulSectorChain[in_ulIndex] = in_ulValue;

    ASSERT(io_pStream->pS->ulSectorChainLen <= 
           io_pStream->pS->ulSectorChainAlloc);

    return SSTG_OK;
}


/*============================================================================
  Description:  Determines if a given stream is chained in the MiniFAT and
                resides in the MiniStream
  Arguments: 
   in_pStream - pointer to stream structure 
  Return:       0 if the stream is not a small stream; a nonzero value 
                otherwise
  ==========================================================================*/
static SINT4
streamShouldBeMini (Stream* in_pStream)
{
    DirectoryEntry*     pDirEntry =     NULL;
    Header*             pHdr =          NULL;
    UINT4               cbCutoff =      0;
    TOC*                pTOC =          NULL;
    StreamSharedBlock*  pS =            in_pStream->pS;

    /* Shortcut if possible */
    if (pS->isStreamShouldBeMiniKnown)
    {
        return pS->isStreamShouldBeMini;
    }

    pHdr = rootStorageGetHeader(in_pStream->pRoot);
    cbCutoff = headerGetSmallStreamCutoff(pHdr);

    pTOC = rootStorageGetTOC(in_pStream->pRoot);
    pDirEntry = tocGetEntryAtIndex(pTOC, in_pStream->ulDirEntry);
    pS->isStreamShouldBeMiniKnown = 1;
    pS->isStreamShouldBeMini = (BYTE)(in_pStream->pS->ullSize < cbCutoff &&
                                      directoryGetType(pDirEntry) != DIR_ROOT);
    return pS->isStreamShouldBeMini;
}


/*============================================================================
  Description:  Constructor for stream structure.  Initializes all members.
  Arguments: 
   in_pRoot -     pointer to an initialized RootStorage representing the file
                  being read from or written to
   in_pParent -   the parent storage of this stream
   in_pDirEntry - pointer to the directory entry for this stream
   in_mode -      the access mode for this stream
   out_ppChild -  pointer to a pointer that will, on success, point to a 
                  freshly allocated and initialized stream structure
  Return:
   Status code
  ==========================================================================*/
SINT4
streamConstruct (RootStorage* in_pRoot, 
                 Storage* in_pParent,
                 DirectoryEntry* in_pDirEntry, 
                 StgMode in_mode,
                 Stream** out_ppChild)
{
    SINT4               iRet =              SSTG_OK;
    DirEntry            dirType =           DIR_FREE;
    Stream*             pStream =           NULL;
    UINT4               cbSectorSize =      0;
    Fat*                pFat =              NULL;
    DirectoryEntry*     pRootEntry =        NULL;
    Header*             pHdr =              NULL;
    UINT2               wSectorShift =      0;
    UINT2               wSmallSectorShift = 0;
    TOC*                pTOC =              NULL;

    pHdr = rootStorageGetHeader(in_pRoot);
    pTOC = rootStorageGetTOC(in_pRoot);

    dirType = directoryGetType (in_pDirEntry);
    if (dirType != DIR_STREAM && dirType != DIR_ROOT)
    {
        return SSTG_ERROR_NOT_STREAM;
    }

    pStream = (Stream*)malloc (sizeof (Stream));
    if (pStream == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(pStream, 0, sizeof(Stream));

    pStream->pRoot = in_pRoot;
    pStream->pParent = in_pParent;
    pStream->ulDirEntry = directoryGetIndex(in_pDirEntry);

    wSectorShift = headerGetSectorShift(pHdr);
    wSmallSectorShift = headerGetSmallSectorShift(pHdr);
    iRet = directoryQueryStreamSharedBlock(in_pDirEntry, 
                                           &(pStream->pS));
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    /* Only set the size if it hasn't already been set by another Stream object 
     */
    if (pStream->pS->ulSectorChainAlloc == 0)
    {
        pStream->pS->ullSize = directoryGetStreamLength (in_pDirEntry);
    }


    pStream->pS->isStreamShouldBeMiniKnown = 0;
    pStream->pS->isStreamShouldBeMini = 0;

    if (STG_CANWRITE(rootStorageGetMode(in_pRoot)))
    {
        pStream->pbSectorBuf = (BYTE*)malloc(1 << wSectorShift);
        if (pStream->pbSectorBuf == NULL)
        {
            free (pStream);
            return SSTG_ERROR_MEMORY;
        }
        memset( pStream->pbSectorBuf, 0, 1 << wSectorShift);
    }
    else
    {
        pStream->pbSectorBuf = NULL;
    }

    if (STG_CANREAD(in_mode))
    {

        if (streamShouldBeMini(pStream))
        {
            cbSectorSize = (1 << wSmallSectorShift);
            pFat = rootStorageGetSmallFat(in_pRoot);
        }
        else
        {
            cbSectorSize = (1 << wSectorShift);
            pFat = rootStorageGetFat(in_pRoot);
        }

#ifdef DEBUG
        assertMiniStreamIntegrity(in_pRoot);
#endif /* DEBUG */
        if (pStream->pS->ulSectorChainAlloc == 0)
        {
            pStream->pS->ulSectorChainAlloc = calcReqdSectors(pStream->pS->ullSize, 
                                                              cbSectorSize);
            iRet = fatGetChain (pFat,
                                directoryGetStartSector (in_pDirEntry),
                                &(pStream->pS->ulSectorChainAlloc),
                                &pStream->pS->pulSectorChain);
            if (iRet != SSTG_OK)
            {
                free (pStream);
                return iRet;
            }
            pStream->pS->ulSectorChainLen = pStream->pS->ulSectorChainAlloc;
        }

        if (streamShouldBeMini(pStream) && 
            (pStream->pS->pMiniStream == NULL) &&
            (pStream->pS->ulRefCount == 1))
        {
            pRootEntry = tocGetEntryAtIndex(pTOC, 0);
            iRet = streamConstruct(in_pRoot,
                                   in_pParent,
                                   pRootEntry,
                                   in_mode,
                                   &(pStream->pS->pMiniStream));
            if (iRet != SSTG_OK)
            {
                free (pStream);
                return iRet;
            }
        }
    }
    pStream->ullSeekPointer = 0;
    pStream->cbInSectorBuf = 0;
    pStream->ulBufferedSector = (UINT4)-1;
    pStream->mode = in_mode;

    *out_ppChild = pStream;

    return iRet;
}


/*============================================================================
  Description:  Allocates a stream shared block structure and initialises its
                data members.
  Arguments: 
   io_ppSharedBlock - pointer to a pointer that will, on success, point to
                      the new shared block.
  Return:
   Status code
  ==========================================================================*/
SINT4 
streamConstructSharedBlock(StreamSharedBlock** out_ppSharedBlock)
{
    StreamSharedBlock* pSharedBlock = NULL;

    ASSERT (out_ppSharedBlock != NULL);

    pSharedBlock = (StreamSharedBlock*)malloc(sizeof(StreamSharedBlock));
    if (pSharedBlock == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }
    pSharedBlock->ullSize = 0;
    pSharedBlock->pulSectorChain = NULL;
    pSharedBlock->ulSectorChainLen = 0;
    pSharedBlock->ulSectorChainAlloc = 0;
    pSharedBlock->pMiniStream = NULL;
    pSharedBlock->isStreamShouldBeMiniKnown = 0;
    pSharedBlock->isStreamShouldBeMini = 0;
    pSharedBlock->ulRefCount = 0;

    *out_ppSharedBlock = pSharedBlock;

    return SSTG_OK;
}


/*============================================================================
  Description:  Retrieves the reference count of this shared block
  Arguments: 
   in_pSharedBlock - pointer to a stream shared block.
  Return:
   number of Streams that have a pointer to this shared block
  ==========================================================================*/
UINT4 
streamSharedBlockGetRefCount(const StreamSharedBlock* in_pSharedBlock)
{
    return in_pSharedBlock->ulRefCount;
}


/*============================================================================
  Description:  Sets the reference count of this shared block
  Arguments: 
   io_pSharedBlock - pointer to a stream shared block.
   in_ulRefCount - new reference count of this shared block
  Return:
   number of Streams that have a pointer to this shared block
  ==========================================================================*/
void 
streamSharedBlockSetRefCount(StreamSharedBlock* io_pSharedBlock, 
                             UINT4 in_ulRefCount)
{
    io_pSharedBlock->ulRefCount = in_ulRefCount;
}


/*============================================================================
  Description:  Frees a stream shared block structure and its associated 
                data.
  Arguments: 
   io_ppSharedBlock - handle to block to free.  The pointer referred to by
                      the handle will be set to NULL.
  Return:
   Status code
  ==========================================================================*/
SINT4
streamDestroySharedBlock(StreamSharedBlock** io_ppSharedBlock)
{
    StreamSharedBlock* pSharedBlock = *io_ppSharedBlock;

    ASSERT ((io_ppSharedBlock != NULL) && (*io_ppSharedBlock != NULL));

    if (pSharedBlock->pulSectorChain != NULL)
    {
        free(pSharedBlock->pulSectorChain);
    }

    if (pSharedBlock->pMiniStream != NULL)
    {
        streamDestroy(&(pSharedBlock->pMiniStream));
    }

    free(pSharedBlock);
    *io_ppSharedBlock = NULL;

    return SSTG_OK;
}


#ifdef DEBUG
void dumpData(CHAR* in_pbData, UINT4 in_cbCount, UINT4 offset)
{
    UINT4   i =     0;
    CHAR    ch =    0;

    for (i = 0; i < in_cbCount; i++)
    {
        ch = in_pbData[i];
        printf("%c", isprint(ch) ? ch : '.');
        if (((i + offset + 1) % 16) == 0)
        {
            printf("\n");
        }
    }
}

#endif /* DEBUG */


/*============================================================================
  Description:  Converts a mini-stream to a full-size stream.  This is useful
                for writable modes, since it is easier to deal with just one
                type of stream than to implement everything twice.
                Returns SSTG_OK immediately if the stream is already a regular 
                stream.
  Arguments: 
   io_pStream -   stream to promote
  Return:
   Status code
  ==========================================================================*/
SINT4
streamUpgradeMiniToRegular(Stream* io_pStream)
{
    BYTE* pBuf =                NULL;
    UINT4 cb =                  0;
    UINT4 ulOldSectorChainLen = 0;
    UINT4 ulOldStartSector =    (UINT4)-1;
    UINT8 ullOldSeekPointer =   0;
    UINT8 ullSize =             0;
    SINT4 iRet =                SSTG_OK;

    /* Early exit if the stream isn't mini, or if there are already Stream
     * structures open that would have done the upgrade earlier */
    if (!streamShouldBeMini(io_pStream) || io_pStream->pS->ulRefCount > 1)
    {
        ASSERT(io_pStream->pS->pMiniStream == NULL);
        return SSTG_OK;
    }

    ullOldSeekPointer = io_pStream->ullSeekPointer;

	//BORLAND requires non zero size
    pBuf = (BYTE*)malloc((UINT4)(io_pStream->pS->ullSize+1));
    if (pBuf == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    ullSize = io_pStream->pS->ullSize;
    cb = (UINT4)ullSize;
    iRet = streamRead(io_pStream, pBuf, &cb);
    if ((iRet != SSTG_OK) || (cb != ((UINT4)ullSize)))
    {
        if (iRet == SSTG_OK)
        {
            iRet = SSTG_ERROR_END_OF_STREAM;
        }
        free(pBuf);
        return iRet;
    }

    io_pStream->pS->isStreamShouldBeMiniKnown = 1;
    io_pStream->pS->isStreamShouldBeMini = 0;
    ulOldSectorChainLen = io_pStream->pS->ulSectorChainLen;
    if (ulOldSectorChainLen > 0)
    {
        ulOldStartSector = io_pStream->pS->pulSectorChain[0];
    }
    io_pStream->pS->ulSectorChainLen = 0;
    io_pStream->pS->ullSize = 0;
    io_pStream->ullSeekPointer = 0;

    iRet = streamWrite(io_pStream, pBuf, &cb);
    free(pBuf);
    if ((iRet != SSTG_OK) || (cb != ((UINT4)ullSize)))
    {
        io_pStream->pS->isStreamShouldBeMiniKnown = 0;
        io_pStream->pS->ulSectorChainLen = ulOldSectorChainLen;
        io_pStream->pS->ullSize = ullSize;
        io_pStream->ullSeekPointer = ullOldSeekPointer;
        return iRet;
    }
    iRet = streamFlushToDisk(io_pStream);
    if (iRet != SSTG_OK)
    {
        io_pStream->pS->isStreamShouldBeMiniKnown = 0;
        io_pStream->pS->ulSectorChainLen = ulOldSectorChainLen;
        io_pStream->pS->ullSize = ullSize;
        io_pStream->ullSeekPointer = ullOldSeekPointer;
        return iRet;
    }

    /* Ignore errors at this call, they just mean leaked sectors */
    if (ulOldStartSector != (UINT4)-1)
    {
        fatFreeChain(rootStorageGetSmallFat(io_pStream->pRoot), ulOldStartSector);
    }

    streamDestroy(&(io_pStream->pS->pMiniStream));

    io_pStream->ullSeekPointer = ullOldSeekPointer;

    /* Recover as many sectors as possible from the small FAT's free 
     * chain */
    iRet = fatRecoverMiniFatFreeSectors(
        rootStorageGetSmallFat(io_pStream->pRoot), 
        rootStorageGetFat(io_pStream->pRoot));
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

#ifdef DEBUG
    assertSectorChainIntegrity(io_pStream);
    assertMiniStreamIntegrity(io_pStream->pRoot);
#endif /* DEBUG */

    return SSTG_OK;
}


/*============================================================================
  Description:  (API Call) Reads data from a stream
  Arguments: 
   in_pStream -  pointer to stream structure representing stream to read from
   out_pbData -  buffer to read into
   io_pcbCount - number of bytes to read.  Contains number of bytes actually
                 read on return.
  Return:       
   Status code
  ==========================================================================*/
int
streamRead(Stream* in_pStream,
           void* out_pbData,
           unsigned int* io_pcbCount)
{
    UINT4       cbInSector =            0;
    UINT4       cbRead =                0;
    UINT4       cbToCopy =              0;
    UINT4       cbReadFromMiniStream =  0;
    UINT4       ulSectorIndex =         0;
    UINT4       ulSector =              0;
    UINT4       ulOffsetInSector =      0;
    SINT4       iRet =                  SSTG_OK;
    CHAR*       pBuf =                  NULL;
    Header*     pHdr =                  NULL;
    UINT2       wSectorShift =          0;
    UINT2       wSmallSectorShift =     0;

    ASSERT(in_pStream != NULL && in_pStream->pRoot != NULL);
    ASSERT(out_pbData != NULL);
    ASSERT(io_pcbCount != NULL);
    ASSERT (STG_CANREAD(in_pStream->mode));

    if ((in_pStream == NULL) || 
        (in_pStream->pRoot == NULL) ||
        (out_pbData == NULL) || 
        (io_pcbCount == NULL) ||
        (!STG_CANREAD(in_pStream->mode)))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    /* Early exit if 0 bytes are to be read */
    if (*io_pcbCount == 0)
    {
        return SSTG_OK;
    }

    /* Early exit if the seek pointer is past the end of the stream */
    if (in_pStream->ullSeekPointer >= in_pStream->pS->ullSize)
    {
        *io_pcbCount = 0;
        return SSTG_ERROR_END_OF_STREAM;
    }

    pHdr = rootStorageGetHeader(in_pStream->pRoot);
    wSectorShift = headerGetSectorShift(pHdr);
    wSmallSectorShift = headerGetSmallSectorShift(pHdr);
    if (in_pStream->pS->pMiniStream != NULL)
    {
        cbInSector = 1 << wSmallSectorShift;
        pBuf = (CHAR*)malloc (cbInSector);
        if (pBuf == NULL)
        {
            return SSTG_ERROR_MEMORY;
        }

        memset(pBuf, 0, cbInSector );

    }
    else
    {
        cbInSector = 1 << wSectorShift;
    }

    *io_pcbCount = (UINT4)min(*io_pcbCount,
                              (in_pStream->pS->ullSize - 
                               in_pStream->ullSeekPointer));
    ulSectorIndex = (UINT4)(in_pStream->ullSeekPointer / cbInSector);
    ulOffsetInSector = (UINT4)(in_pStream->ullSeekPointer -
                               (((UINT8)ulSectorIndex) * ((UINT8)cbInSector)));

    for (cbRead = 0; cbRead < *io_pcbCount; )
    {
        ulSector = in_pStream->pS->pulSectorChain[ulSectorIndex];
        if (in_pStream->pS->pMiniStream != NULL)
        {
            iRet = streamSeek64(in_pStream->pS->pMiniStream,
                                ((SINT8)cbInSector) * ((SINT8)ulSector),
                                STG_START);
            if (iRet != SSTG_OK)
            {
                break;
            }
            cbReadFromMiniStream = cbInSector;
            iRet = streamRead (in_pStream->pS->pMiniStream, pBuf, &cbReadFromMiniStream);
            if (iRet != SSTG_OK)
            {
                break;
            }
        }
        else
        {
            pBuf = rootStorageReadSector(in_pStream->pRoot, ulSector);
            if (pBuf == NULL)
            {
                iRet = rootStorageGetStatus(in_pStream->pRoot);
                break;
            }
        }
        cbToCopy = min(cbInSector - ulOffsetInSector, *io_pcbCount - cbRead);
        memcpy(&(((BYTE*)out_pbData)[cbRead]),
               &pBuf[ulOffsetInSector],
               cbToCopy);
        ulSectorIndex++;
        cbRead += cbToCopy;
        ulOffsetInSector = 0;
    }

    if ((in_pStream->pS->pMiniStream != NULL) && (pBuf != NULL))
    {
        free (pBuf);
    }

    *io_pcbCount = cbRead;
    in_pStream->ullSeekPointer += cbRead;

    return iRet;
}


/*============================================================================
  Description:  (API Call) Sets the seek pointer inside a given stream
                For now, the stream must be open for reading, since seeking
                while writing is unimplemented.
  Arguments: 
   io_pStream - stream to seek
   in_offset -  index to seek to
   in_pos -     place from which to count index (STG_START, STG_CUR, or 
                STG_END)
  Return:
   Status code
  ==========================================================================*/
int
streamSeek(Stream* io_pStream, long in_offset, SeekPos in_pos)
{
    return streamSeek64(io_pStream, in_offset, in_pos);
}


/*============================================================================
  Description:  (API Call) Sets the seek pointer inside a given stream
                For now, the stream must be open for reading, since seeking
                while writing is unimplemented.
  Arguments: 
   io_pStream - stream to seek
   in_offset -  64-bit index to seek to
   in_pos -     place from which to count index (STG_START, STG_CUR, or 
                STG_END)
  Return:
   Status code
  ==========================================================================*/
int
streamSeek64(Stream* io_pStream, SSRW_INT64 in_offset, SeekPos in_pos)
{
    SINT8 llAbsOffset = 0;
    SINT4 iRet =        SSTG_OK;

    ASSERT (io_pStream != NULL);
    /* Can't seek before the beginning of the file */
    ASSERT (in_pos != STG_START || (in_offset >= 0));
    ASSERT (in_pos != STG_CUR || ((-1 * in_offset) <= ((SINT8)(io_pStream->ullSeekPointer))));
    ASSERT (in_pos != STG_END || ((-1 * in_offset) <= ((SINT8)io_pStream->pS->ullSize)));
    if ((io_pStream == NULL) ||
        (in_pos == STG_START && (in_offset < 0)) ||
        (in_pos == STG_CUR && ((-1 * in_offset) > ((SINT8)(io_pStream->ullSeekPointer)))) ||
        (in_pos == STG_END && ((-1 * in_offset) > ((SINT8)(io_pStream->pS->ullSize)))))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    switch(in_pos)
    {
    case STG_START:
        llAbsOffset = (SINT8)in_offset;
        break;
    case STG_END:
        llAbsOffset = (SINT8)(io_pStream->pS->ullSize + in_offset);
        break;
    case STG_CUR:
        llAbsOffset = (SINT8)(io_pStream->ullSeekPointer + in_offset);
        break;
    default:
        ASSERT(0);
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    /* If we're about to seek while writing, flush first */
    if (STG_CANWRITE(io_pStream->mode) &&
        (io_pStream->ulBufferedSector != (UINT4)-1))
    {
        iRet = streamFlushToDisk(io_pStream);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    /* Note that llAbsOffset can't be less than 0, but it can be past the end
     * of the stream. */
    io_pStream->ullSeekPointer = llAbsOffset;

    return iRet;
}


/*============================================================================
  Description:  (API Call) Retrieves the current seek position in a stream.  
                Could be past the end of the stream.   Just like ftell().
  Arguments: 
   in_pStream - pointer to stream from which to get the seek position
   out_pos      - retrieved position in the stream
  Return:
   Status code
  ==========================================================================*/
int
streamGetPos(Stream* in_pStream, unsigned long* out_pos)
{
    ASSERT (in_pStream != NULL);

    if (in_pStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    *out_pos =  (unsigned long)in_pStream->ullSeekPointer;
    return SSTG_OK;
}


/*============================================================================
  Description:  (API Call) Retrieves the current seek position in a stream.  
                Could be past the end of the stream.   Just like ftell().
  Arguments: 
   in_pStream - pointer to stream from which to get the seek position
   out_pos    - retrieved 64-bit position in the stream
  Return:
   Status code
  ==========================================================================*/
int
streamGetPos64(Stream* in_pStream, unsigned SSRW_INT64* out_pos)
{
    ASSERT (in_pStream != NULL);

    if (in_pStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    *out_pos =  in_pStream->ullSeekPointer;

    return SSTG_OK;
}


/*============================================================================
  Description:  When a stream is open for reading, returns the number of bytes
                that are still available for reading from the current seek
                position.
  Arguments: 
   in_pStream - pointer to stream, which must be open for reading
   out_pos      - retrieved count of bytes
  Return:
   Status code
  ==========================================================================*/
extern int streamGetBytesAvailableToRead(
        Stream* in_pStream,
        unsigned long* out_pOffsetFromEnd)
{
    ASSERT(in_pStream != NULL);
    ASSERT(out_pOffsetFromEnd != NULL);
    ASSERT (STG_CANREAD(in_pStream->mode));
    if ((in_pStream == NULL) || (out_pOffsetFromEnd == NULL) ||
            (! STG_CANREAD(in_pStream->mode)))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    *out_pOffsetFromEnd =
        (unsigned long)(in_pStream->pS->ullSize - in_pStream->ullSeekPointer);

    return SSTG_OK;
}


/*============================================================================
  Description:  (API Call) When a stream is open for reading, returns the number 
                of bytes that are still available for reading from the current 
                seek position.
  Arguments: 
   in_pStream - pointer to stream, which must be open for reading
   out_pos    - retrieved 64-bit count of bytes
  Return:
   Status code
  ==========================================================================*/
extern int streamGetBytesAvailableToRead64(
        Stream* in_pStream,
        unsigned SSRW_INT64* out_pOffsetFromEnd)
{
    ASSERT(in_pStream != NULL);
    ASSERT(out_pOffsetFromEnd != NULL);
    ASSERT (STG_CANREAD(in_pStream->mode));
    if ((in_pStream == NULL) || (out_pOffsetFromEnd == NULL) ||
            (! STG_CANREAD(in_pStream->mode)))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    *out_pOffsetFromEnd =
        (in_pStream->pS->ullSize - in_pStream->ullSeekPointer);

    return SSTG_OK;
}


/*============================================================================
  Description:  (API Call) Flushes and closes a stream.  Also frees all memry 
                associated with that stream.
  Arguments: 
   in_ppStream - pointer to pointer to stream structure representing stream to 
                 close.  On success, will point to NULL.
  Return:       
   Status code
  ==========================================================================*/
int
closeStream (Stream** io_ppStream)
{
    SINT4           iRet =      SSTG_OK;
    DirectoryEntry* pDirEntry = NULL;
    TOC*            pTOC =      NULL;

    if (io_ppStream == NULL || *io_ppStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }


    if (STG_CANWRITE((*io_ppStream)->mode))
    {
        iRet = streamFlushToDisk(*io_ppStream);
        /* Ignore errors here.  If we can't flush, we still want to be able to
         * avoid losing the existing data in the stream if possible. */

#ifdef SSRW_RESIZE_STREAM_ON_CLOSE
        /* Set the stream to end at the current seek pointer. */
        iRet = streamResize((*io_ppStream), (*io_ppStream)->ullSeekPointer);
#endif /* SSRW_RESIZE_STREAM_ON_CLOSE */

        /* Ignore errors here.  Again, we still want to be able to
         * avoid losing the existing data in the stream if possible. */

#ifdef DEBUG
        assertSectorChainIntegrity(*io_ppStream);
#endif /* DEBUG */
    }

    pTOC = rootStorageGetTOC((*io_ppStream)->pRoot);

    pDirEntry = tocGetEntryAtIndex(pTOC, (*io_ppStream)->ulDirEntry);
    if (STG_CANWRITE((*io_ppStream)->mode))
    {
        iRet = directorySetSize(pDirEntry, (*io_ppStream)->pS->ullSize);
        if (iRet != SSTG_OK)
        {
            streamDestroy(io_ppStream);
            return (int)iRet;
        }

        iRet = directorySetCurrentCreationTime(pDirEntry);
        if (iRet != SSTG_OK)
        {
            streamDestroy(io_ppStream);
            return (int)iRet;
        }

        iRet = directorySetCurrentModificationTime(pDirEntry);
        if (iRet != SSTG_OK)
        {
            streamDestroy(io_ppStream);
            return (int)iRet;
        }

        /* If this stream should be a "small stream", and no other Stream 
         * objects are using this stream, then turn it into a small stream */
        (*io_ppStream)->pS->isStreamShouldBeMiniKnown = 0;
        if (streamShouldBeMini(*io_ppStream) && (*io_ppStream)->pS->ulRefCount == 1)
        {
            iRet = convertToSmallStream(*io_ppStream);
            if (iRet != SSTG_OK)
            {
                streamDestroy(io_ppStream);
                return (int)iRet;
            }
        }
    }

    /* Remove stream from parent's open storage list */
    iRet = storageRemoveOpenChild ((*io_ppStream)->pParent,
                                   STREAM,
                                   (*io_ppStream));
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

    return (int)streamDestroy (io_ppStream);
}


/*============================================================================
  Description:  Destructor, destroys the passed stream
  Arguments: 
   io_ppStream - pointer to stream to free
  Return:       
   A status code.  SSTG_OK is successful
  ==========================================================================*/
SINT4
streamDestroy (Stream** io_ppStream)
{
    DirectoryEntry* pDirEntry = NULL;

    if ((*io_ppStream)->pS != NULL)
    {
        ASSERT(((*io_ppStream)->pRoot) != NULL);
        pDirEntry = tocGetEntryAtIndex(rootStorageGetTOC((*io_ppStream)->pRoot),
                                       (*io_ppStream)->ulDirEntry);
        directoryReleaseStreamSharedBlock(pDirEntry);
        /* Ignore errors here--we want to clean things down as well as 
         * possible */
    }

    if ((*io_ppStream)->pbSectorBuf != NULL)
    {
        free ((*io_ppStream)->pbSectorBuf);
    }

    free (*io_ppStream);
    *io_ppStream = NULL;

    return SSTG_OK;
}


/*============================================================================
  Description:  (API Call) Fills a structure with information about a stream
  Arguments: 
   in_pStream - pointer to stream from which to get information
   out_ppInfo - on success, will point to a newly allocated structure
                that contains information about the given stream.  This 
                structure should only be freed with freeInfo().
  Return:       Status code
  ==========================================================================*/
int
getStreamInfo(Stream* in_pStream,
              StgInfo** out_ppInfo)
{
    SINT4           iRet =      SSTG_OK;
    DirectoryEntry* pDirEntry = NULL;

    ASSERT (in_pStream != NULL);
    ASSERT (out_ppInfo != NULL);
    if ((in_pStream == NULL) ||
            (out_ppInfo == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }
    *out_ppInfo = (StgInfo*)malloc(sizeof(StgInfo));
    if (*out_ppInfo == NULL)
    {
        return (int)SSTG_ERROR_MEMORY;
    }

    memset(*out_ppInfo, 0, sizeof(StgInfo));

    pDirEntry = tocGetEntryAtIndex(rootStorageGetTOC(in_pStream->pRoot),
                                   in_pStream->ulDirEntry);
    iRet =  directoryFillStgInfo (pDirEntry, *out_ppInfo);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Reads a short from a stream
  Arguments: 
   in_pStream - pointer to stream from which to read
   out_psVal - pointer to value to fill out
  Return:
   Status code
  ==========================================================================*/
int
readLEshort(Stream* in_pStream,
            short* out_psVal)
{
    SINT4           iRet =                  SSTG_OK;
    UINT4   cb =                    sizeof(SINT2);
    BYTE            abBuf[sizeof(SINT2)];

    ASSERT (in_pStream != NULL);
    ASSERT (out_psVal != NULL);
    if ((in_pStream == NULL) ||
            (out_psVal == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = streamRead (in_pStream, abBuf, &cb);
    *out_psVal = (short)leReadShort(abBuf);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Reads a word (16-bit unsigned integer) from a 
                stream
  Arguments: 
   in_pStream - pointer to stream from which to read
   out_pwVal - pointer to value to fill out
  Return:
   Status code
  ==========================================================================*/
int
readLEword(Stream* in_pStream,
           unsigned short* out_pwVal)
{
    SINT4           iRet =                  SSTG_OK;
    UINT4   cb =                    sizeof(UINT2);
    BYTE            abBuf[sizeof(UINT2)];

    ASSERT (in_pStream != NULL);
    ASSERT (out_pwVal != NULL);
    if ((in_pStream == NULL) ||
            (out_pwVal == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = streamRead (in_pStream, abBuf, &cb);
    *out_pwVal = (unsigned short)leReadUnsignedShort(abBuf);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Reads a long from a stream
                NOTE: retrieves a platform-specific long, but ALWAYS READS 
                EXACTLY FOUR BYTES FROM THE STREAM.  THE STREAM POINTER WILL
                BE INCREMENTED BY FOUR BYTES ON EVERY PLATFORM.
  Arguments: 
   in_pStream - pointer to stream from which to read
   out_plVal - pointer to value to fill out
  Return:
   Status code
  ==========================================================================*/
int
readLElong(Stream* in_pStream,
           int* out_plVal)
{
    SINT4           iRet =                  SSTG_OK;
    UINT4   cb =                    sizeof(SINT4);
    BYTE            abBuf[sizeof(SINT4)];

    ASSERT (in_pStream != NULL);
    ASSERT (out_plVal != NULL);
    if ((in_pStream == NULL) ||
            (out_plVal == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = streamRead (in_pStream, abBuf, &cb);
    *out_plVal = (long)leReadLong(abBuf);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Reads a dword (32-bit unsigned integer) from a 
                stream
                NOTE: retrieves a platform-specific unsigned long, but ALWAYS 
                READS EXACTLY FOUR BYTES FROM THE STREAM.  THE STREAM POINTER 
                WILL BE INCREMENTED BY FOUR BYTES ON EVERY PLATFORM.
  Arguments: 
   in_pStream - pointer to stream from which to read
   out_pulVal - pointer to value to fill out
  Return:
   Status code
  ==========================================================================*/
int
readLEdword(Stream* in_pStream,
            unsigned int* out_pulVal)
{
    SINT4           iRet =                  SSTG_OK;
    UINT4   cb =                    sizeof(UINT4);
    BYTE            abBuf[sizeof(UINT4)];

    ASSERT (in_pStream != NULL);
    ASSERT (out_pulVal != NULL);
    if ((in_pStream == NULL) ||
        (out_pulVal == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = streamRead (in_pStream, abBuf, &cb);
    *out_pulVal = (unsigned long)leReadUnsignedLong(abBuf);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Reads a float from a stream
  Arguments: 
   in_pStream - pointer to stream from which to read
   out_pfVal - pointer to value to fill out
  Return:
   Status code
  ==========================================================================*/
int
readLEfloat(Stream* in_pStream,
            float* out_pfVal)
{
    SINT4           iRet =                  SSTG_OK;
    UINT4  cb =                    sizeof(float);
    BYTE            abBuf[sizeof(float)];

    ASSERT (in_pStream != NULL);
    ASSERT (out_pfVal != NULL);
    if ((in_pStream == NULL) ||
            (out_pfVal == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = streamRead (in_pStream, abBuf, &cb);
    *out_pfVal = leReadFloat(abBuf);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Reads a double from a stream
  Arguments: 
   in_pStream - pointer to stream from which to read
   out_plfVal - pointer to value to fill out
  Return:
   Status code
  ==========================================================================*/
int
readLEdouble(Stream* in_pStream,
             double* out_plfVal)
{
    SINT4           iRet =                  SSTG_OK;
    UINT4   cb =                    sizeof(double);
    BYTE            abBuf[sizeof(double)];

    ASSERT (in_pStream != NULL);
    ASSERT (out_plfVal != NULL);
    if ((in_pStream == NULL) ||
            (out_plfVal == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = streamRead (in_pStream, abBuf, &cb);
    *out_plfVal = leReadDouble(abBuf);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Reads a wide character from a stream
                NOTE: on systems where a wchar_t is 4 bytes, this method will
                simply read 2 bytes from the stream and promote the raw value 
                to a 4-byte character.
  Arguments: 
   in_pStream - pointer to stream from which to read
   out_pwcVal - pointer to value to fill out
  Return:
   Status code
  ==========================================================================*/
int
readLEwchar(Stream* in_pStream,
            wchar_t* out_pwcVal)
{
    SINT4           iRet =                  SSTG_OK;
    UINT4   cb =                    sizeof(CHAR2);
    BYTE            abBuf[sizeof(CHAR2)];

    ASSERT (in_pStream != NULL);
    ASSERT (out_pwcVal != NULL);
    if ((in_pStream == NULL) ||
            (out_pwcVal == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = streamRead (in_pStream, abBuf, &cb);
    *out_pwcVal = (wchar_t)leReadUnsignedShort(abBuf);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Reads a wide string from a stream.  Stops until
                *io_piCount characters have been read or a nullchar is 
                encountered.  If the buffer is filled, will not null-terminate
                that buffer.
  Arguments: 
   in_pStream -    pointer to stream from which to read
   io_piCount -    maximum number of characters to read (including nullchar
                   terminator)
   out_strString - pointer to value to fill out
  Return:
   Status code
  ==========================================================================*/
int readLEwstring(Stream* in_pStream,
                  int* io_piCount,
                  wchar_t* out_strString)
{
    SINT4           iRet =  SSTG_OK;
    SINT4           i =     0;
    wchar_t*        pCur =  NULL;

    ASSERT (in_pStream != NULL);
    ASSERT (io_piCount != NULL);
    ASSERT (out_strString != NULL);
    if ((in_pStream == NULL) ||
        (io_piCount == NULL) ||
        (out_strString == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    pCur = out_strString - 1;
    for (i = 0; i < *io_piCount && iRet == SSTG_OK; i++)
    {
        pCur++;
        iRet = readLEwchar(in_pStream, pCur);
        if (*pCur == 0)
        {
            i++;
            break;
        }
    }
    *io_piCount = i;

    return iRet;
}

/*============================================================================
  Description: (API call) Sets the CLSID of the stream
  Arguments:   
   io_pDir - non-NULL pointer to a stream to modify
   in_pCLSID - pointer to the new CLSID for the stream.  Makes a copy.  Must 
               be non-NULL.
  Return:       
   Status code
  ==========================================================================*/
int
streamSetClass(Stream* io_pStream,
               const SSRW_CLSID* in_pCLSID)
{
    TOC*                pTOC =          NULL;
    DirectoryEntry*     pDirEntry =     NULL;
    SINT4               iRet =          SSTG_OK;

    ASSERT (io_pStream != NULL);
    ASSERT (STG_CANWRITE((io_pStream->mode)));
    if ((io_pStream == NULL) ||
        (!STG_CANWRITE(io_pStream->mode)))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    iRet = rootStorageSetDirty(io_pStream->pRoot);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    pTOC = rootStorageGetTOC(io_pStream->pRoot);
    pDirEntry = tocGetEntryAtIndex(pTOC, io_pStream->ulDirEntry);
    return (int)directorySetClass(pDirEntry, in_pCLSID);
}

/*============================================================================
  Description:  (API Call) Writes data to a stream
  Arguments: 
   io_pStream -  pointer to stream structure representing stream to write to
   in_pbData -   data to write
   io_pcbCount - number of bytes to write.  Contains number of bytes actually
                 written on return.
  Return:
   Status code
  ==========================================================================*/
int
streamWrite(Stream* io_pStream,
            const void* in_pbData,
            unsigned int* io_pcbCount)
{
    UINT4       cbWritten =         0;
    UINT4       cbToWrite =         0;
    UINT4       cbInSector =        0;
    UINT4       cbLeftInSector =    0;
    SINT4       iRet =              SSTG_OK;
    Header*     pHdr =              NULL;
    UINT2       wSectorShift =      0;
    UINT4       ulSectorIndex =     0;
    UINT4       ulSector =          0;
    CHAR*       pBuf =              NULL;

    ASSERT (io_pStream != NULL);
    ASSERT (in_pbData != NULL);
    ASSERT (io_pcbCount != NULL);
    ASSERT (STG_CANWRITE(io_pStream->mode));
    if ((io_pStream == NULL) ||
        (in_pbData == NULL) ||
        (io_pcbCount == NULL) ||
        (!STG_CANWRITE(io_pStream->mode)))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if ((io_pStream->ullSeekPointer < io_pStream->pS->ullSize)
        && (io_pStream->pS->pMiniStream != NULL))
    {
        /* This should never happen anymore because small streams should always
         * be upgraded to regular streams when opened in writable modes */
        ASSERT(0);
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    /* Ensure that the root storage's "dirty" flag is set */
    iRet = rootStorageSetDirty(io_pStream->pRoot);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* If the seek pointer is past the end of the stream, then grow the stream
     * up to the seek pointer */
    if (io_pStream->ullSeekPointer > io_pStream->pS->ullSize)
    {
        iRet = streamResize(io_pStream, io_pStream->ullSeekPointer);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    pHdr = rootStorageGetHeader(io_pStream->pRoot);

    wSectorShift = headerGetSectorShift(pHdr);
    cbInSector = 1 << wSectorShift;
    io_pStream->cbInSectorBuf = 
            (UINT4)(io_pStream->ullSeekPointer % cbInSector);
    cbLeftInSector = cbInSector - io_pStream->cbInSectorBuf;
    cbToWrite = min(cbLeftInSector, *io_pcbCount);

    while (cbToWrite > 0)
    {
        ulSectorIndex = (UINT4)(io_pStream->ullSeekPointer / (UINT8)cbInSector);
        if ((ulSectorIndex != io_pStream->ulBufferedSector) && 
            (ulSectorIndex < io_pStream->pS->ulSectorChainLen))
        {
            ulSector = io_pStream->pS->pulSectorChain[ulSectorIndex];

            pBuf = rootStorageReadSector(io_pStream->pRoot, ulSector);
            if (pBuf == NULL)
            {
                iRet = rootStorageGetStatus(io_pStream->pRoot);
                return iRet;
            }
            memcpy(io_pStream->pbSectorBuf, pBuf, cbInSector);
            io_pStream->cbInSectorBuf = 
                (UINT4)(io_pStream->ullSeekPointer % cbInSector);
        }
        io_pStream->ulBufferedSector = ulSectorIndex;

        memcpy(&(io_pStream->pbSectorBuf[io_pStream->cbInSectorBuf]),
               &((BYTE*)in_pbData)[cbWritten], cbToWrite);
        io_pStream->cbInSectorBuf += cbToWrite;
        cbLeftInSector -= cbToWrite;

        ASSERT(io_pStream->cbInSectorBuf <= cbInSector);
        if (io_pStream->cbInSectorBuf == cbInSector)
        {
            /* Flush sector buffer */
            iRet = streamFlushToDisk(io_pStream);
            if (iRet != SSTG_OK)
            {
                *io_pcbCount = cbWritten;
                io_pStream->pS->ullSize = max( io_pStream->pS->ullSize, io_pStream->ullSeekPointer);
                return (int)iRet;
            }
            cbLeftInSector = cbInSector;
        }
        cbWritten += cbToWrite;
        io_pStream->ullSeekPointer += cbToWrite;
        io_pStream->pS->ullSize = max( io_pStream->pS->ullSize, io_pStream->ullSeekPointer);
        cbToWrite = min(cbLeftInSector, *io_pcbCount - cbWritten);
    }

    *io_pcbCount = cbWritten;

    return (int)iRet;
}


/*============================================================================
  Description:  Flushes a stream.  On success, the buffer for this stream
                will be empty.
                This function operates like fflush; streamFlush does not.
  Arguments: 
   in_pStream - pointer to stream structure representing stream to flush
  Return:
   Status code
  ==========================================================================*/
static SINT4
streamFlushToDisk(Stream* in_pStream)
{
    UINT2 wSectorShift        = 0;
    UINT4 ulSectorWritten     = 0;
    UINT4 ulSectorIndex       = 0;
    UINT4 ulLastSector        = 0;
    UINT4 ulSector            = 0;
    UINT4 cbInSector          = 0;
    SINT4 iRet                = SSTG_OK;
    DirectoryEntry* pDirEntry = NULL;
    TOC* pTOC                 = NULL;
    Fat* pFAT                 = NULL;

    ASSERT (in_pStream != NULL);
    if (in_pStream == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    /* Early exit if there's nothing in the buffer */
    if (in_pStream->cbInSectorBuf == 0)
    {
        return SSTG_OK;
    }

    wSectorShift = headerGetSectorShift( rootStorageGetHeader(in_pStream->pRoot) );
    cbInSector = 1 << wSectorShift;

    ulSectorIndex = (UINT4)(in_pStream->ullSeekPointer /
                            ((UINT8)cbInSector));

    /* Are we overwriting an existing sector? */
    if (ulSectorIndex < in_pStream->pS->ulSectorChainLen)
    {
        ulSector = in_pStream->pS->pulSectorChain[ulSectorIndex];

        iRet = rootStorageRewriteSector(in_pStream->pRoot,
                                        in_pStream->pbSectorBuf,
                                        ulSector);
        ulSectorWritten = ulSectorIndex;

        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }
    else
    {
        iRet = rootStorageWriteSector(in_pStream->pRoot,
                                      in_pStream->pbSectorBuf,
                                      &ulSectorWritten,
                                      SSRW_FALSE);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }

        pTOC = rootStorageGetTOC(in_pStream->pRoot);
        pDirEntry = tocGetEntryAtIndex(pTOC, in_pStream->ulDirEntry);
        pFAT = rootStorageGetFat(in_pStream->pRoot);
        if (in_pStream->pS->ulSectorChainLen == 0)
        {
            iRet = directorySetStartSector(pDirEntry, ulSectorWritten);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
            iRet = fatTerminateChain(pFAT, ulSectorWritten);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
        else
        {
            ulLastSector = in_pStream->pS->pulSectorChain[
                    in_pStream->pS->ulSectorChainLen - 1];
            iRet = fatUpdate(pFAT, ulLastSector, ulSectorWritten);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }

        iRet = streamSetSectorChainEntry(in_pStream, ulSectorIndex, ulSectorWritten);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    in_pStream->ulBufferedSector = (UINT4)-1;
    in_pStream->cbInSectorBuf = 0;

    return SSTG_OK;
}


/*============================================================================
  Description:  Converts a big stream into a small stream
                (a stream that resides in the MiniStream and is referenced by
                 the MiniFAT)
  Arguments: 
   io_pStream - pointer to stream to modify
  Return:
   Status code
  ==========================================================================*/
static SINT4
convertToSmallStream(Stream* io_pStream)
{
    SINT4           iRet =                      SSTG_OK;
    UINT4           cEntries =                  0;
    UINT4           cSmallFatSectors =          0;
    UINT8           ullStreamLength =           0;
    UINT4           cbSmallSector =             0;
    UINT4           cbBigSector =               0;
    UINT4           cSmallSectorsPerBigSector = 0;
    UINT4           ulStartSector =             0;
    UINT4           ulOldStartSector =          0;
    UINT4           ulLastMiniSector =          (UINT4)-1;
    UINT4           i =                         0;
    DirectoryEntry* pDirEntry =                 NULL;
    TOC*            pTOC =                      NULL;
    Fat*            pFAT =                      NULL;
    Fat*            pSmallFAT =                 NULL;
    Header*         pHdr =                      NULL;
    UINT4           ulMiniSector =              0;
    UINT4           cb =                        0;
    UINT8           llBytesToTransfer =         0;
    CHAR*           pToCopy =                   NULL;
    CHAR*           pBuf =                      NULL;
    UINT4           ulRegSector =               0;

    /* None of this is necessary for empty streams, I think */
    if (io_pStream->pS->ullSize == 0)
    {
        return SSTG_OK;
    }

    pTOC = rootStorageGetTOC(io_pStream->pRoot);
    pFAT = rootStorageGetFat(io_pStream->pRoot);
    pSmallFAT = rootStorageGetSmallFat(io_pStream->pRoot);
    pHdr = rootStorageGetHeader(io_pStream->pRoot);

    pDirEntry = tocGetEntryAtIndex(pTOC, io_pStream->ulDirEntry);
    ulStartSector = directoryGetStartSector(pDirEntry);
    cEntries = fatGetLength (pSmallFAT);
    ullStreamLength = directoryGetStreamLength(pDirEntry);
    cbBigSector = (1 << headerGetSectorShift(pHdr));
    cbSmallSector = (1 << headerGetSmallSectorShift(pHdr));
    cSmallFatSectors = calcReqdSectors(ullStreamLength,
                                       cbSmallSector);
    cSmallSectorsPerBigSector = cbBigSector / cbSmallSector;
    ulOldStartSector = directoryGetStartSector(pDirEntry);

    /* Step 1: Copy the stream's data to the next available mini-sectors */
    llBytesToTransfer = (SINT8)io_pStream->pS->ullSize;
    for(i = 0; llBytesToTransfer > 0; i++)
    {
        /* Read a mini sector worth of data from the stream */
        cb = cbSmallSector;
        if (cb > llBytesToTransfer)
        {
            cb = (UINT4)llBytesToTransfer;
        }
        if ((i % cSmallSectorsPerBigSector) == 0)
        {
            ulRegSector = io_pStream->pS->pulSectorChain[
                    i / cSmallSectorsPerBigSector];
            pBuf = rootStorageReadSector(io_pStream->pRoot, ulRegSector);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }

        pToCopy = pBuf + (i % cSmallSectorsPerBigSector) * cbSmallSector;

        /* Write to the mini sector */
        iRet = rootStorageWriteMiniSector(io_pStream->pRoot, 
                                          (BYTE*)pToCopy, 
                                          &ulMiniSector);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }

        /* Update mini-FAT */
        if (ulLastMiniSector == (UINT4)-1)
        {
            iRet = directorySetStartSector(pDirEntry, ulMiniSector);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }

            iRet = fatTerminateChain(pSmallFAT, ulMiniSector);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
        else
        {
            iRet = fatUpdate(pSmallFAT, ulLastMiniSector, ulMiniSector);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }

        ulLastMiniSector = ulMiniSector;
        llBytesToTransfer -= cb;
    }

    /* After this point, the sector chain array will be invalid.  That's okay, 
     * because there should not be anyone referencing this stream after this. */
    iRet = fatFreeChain(pFAT, ulOldStartSector);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

#ifdef DEBUG
    assertMiniStreamIntegrity(io_pStream->pRoot);
#endif /* DEBUG */

    return iRet;
}


/*============================================================================
  Description:  (API call) Resizes an open stream to a given size.  Manages 
                all the changes to the FAT, directory entry, etc.
  Arguments: 
   io_pStream -    pointer to stream to modify
   in_ullNewSize - new size of the stream, in bytes
  Return:
   Status code
  ==========================================================================*/
int 
streamResize(Stream* io_pStream, SSRW_INT64 in_llNewSize)
{
    SINT4           iRet =                      SSTG_OK;
    UINT4           ulSectorsReqdNow =          0;
    Fat*            pFAT =                      NULL;
    DirectoryEntry* pDirEntry =                 NULL;
    TOC*            pTOC =                      NULL;
    UINT4           cbSectorSize =              0;
    Header*         pHdr =                      0;
    UINT4           i =                         0;
    UINT4           ulBackupSectorChainLen =    0;
    UINT8           ullSeekPointer =            0;

    ASSERT(io_pStream != NULL);
    ASSERT(STG_CANWRITE(rootStorageGetMode(io_pStream->pRoot)));

    /* Ensure that the stream is synchronised with the file */
    iRet = streamFlushToDisk(io_pStream);
    if (iRet != SSTG_OK)
    {
        return (int)iRet;
    }

#ifdef DEBUG
    assertSectorChainIntegrity(io_pStream);
#endif /* DEBUG */

    if (((UINT8)in_llNewSize) == io_pStream->pS->ullSize)
    {
        /* Early exit */
        return (int)iRet; 
    }

    /* Ensure that the file will be updated on close */
    iRet = rootStorageSetDirty(io_pStream->pRoot);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    pHdr = rootStorageGetHeader(io_pStream->pRoot);
    pTOC = rootStorageGetTOC(io_pStream->pRoot);
    pDirEntry = tocGetEntryAtIndex(pTOC, io_pStream->ulDirEntry);
    pFAT = rootStorageGetFat(io_pStream->pRoot);
    cbSectorSize = (1 << headerGetSectorShift(pHdr));
    ulSectorsReqdNow = calcReqdSectors(((UINT8)in_llNewSize), cbSectorSize);
    ullSeekPointer = io_pStream->ullSeekPointer;

    if (((UINT8)in_llNewSize) > io_pStream->pS->ullSize)
    {
        /* We must grow the stream. */
        memset(io_pStream->pbSectorBuf, 0, cbSectorSize);

        ulBackupSectorChainLen = io_pStream->pS->ulSectorChainLen;
        if (io_pStream->pS->ulSectorChainLen == 0)
        {
            ulBackupSectorChainLen = 0;
        }
        for(i = io_pStream->pS->ulSectorChainLen; i < ulSectorsReqdNow; i++)
        {
            io_pStream->cbInSectorBuf = cbSectorSize;
            io_pStream->ullSeekPointer = ((SINT8)i) * ((SINT8)cbSectorSize);
            iRet = streamFlushToDisk(io_pStream);
            if (iRet != SSTG_OK)
            {
                if (ulBackupSectorChainLen > 0)
                {
                    fatTerminateChain(pFAT, io_pStream->pS->pulSectorChain[
                        ulBackupSectorChainLen - 1]);
                }
                fatFreeChain(pFAT, io_pStream->pS->pulSectorChain[
                    ulBackupSectorChainLen]);
                io_pStream->pS->ulSectorChainLen = ulBackupSectorChainLen;
                io_pStream->cbInSectorBuf = 0;
                io_pStream->ullSeekPointer = ulBackupSectorChainLen * 
                                             cbSectorSize;
                io_pStream->ullSeekPointer = ullSeekPointer;
                return (int)iRet;
            }
        }
    }
    else /* in_llNewSize < io_pStream->pS->ullSize */
    {
        /* We must shrink the stream. */
        if (ulSectorsReqdNow < io_pStream->pS->ulSectorChainLen)
        {
            if (ulSectorsReqdNow == 0)
            {
                iRet = directorySetStartSector(pDirEntry, (UINT4)-1);
            }
            else
            {
                iRet = fatTerminateChain(
                        pFAT, io_pStream->pS->pulSectorChain[
                                ulSectorsReqdNow - 1]);
            }
            if (iRet != SSTG_OK)
            {
                return (int)iRet;
            }

            iRet = fatFreeChain(
                    pFAT, io_pStream->pS->pulSectorChain[ulSectorsReqdNow]);
            if (iRet != SSTG_OK)
            {
                return (int)iRet;
            }

        }
    }

    io_pStream->pS->ulSectorChainLen = ulSectorsReqdNow;
    io_pStream->pS->ullSize = (UINT8)in_llNewSize;
    io_pStream->ullSeekPointer = ullSeekPointer;

#ifdef DEBUG
    assertSectorChainIntegrity(io_pStream);
#endif /* DEBUG */

    return (int)iRet;
}


/*============================================================================
  Description:  (API Call) Writes a short to a stream
  Arguments: 
   in_pStream - pointer to stream to write to
   in_sVal -    value to write
  Return:
   Status code
  ==========================================================================*/
int
writeLEshort(Stream* in_pStream,
             short in_sVal)
{
    int             iRet =                  SSTG_OK;
    UINT4   cb =                    sizeof(short);
    BYTE            abBuf[sizeof(short)];

    ASSERT (in_pStream != NULL);
    if (in_pStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    leWriteShort(in_sVal, abBuf);
    iRet =  streamWrite(in_pStream, abBuf, &cb);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Writes a word (unsigned 16-bit value) to a stream
  Arguments: 
   in_pStream - pointer to stream to write to
   in_wVal -    value to write
  Return:
   Status code
  ==========================================================================*/
int
writeLEword(Stream* in_pStream,
            unsigned short in_Val)
{
    int             iRet =                          SSTG_OK;
    UINT4   cb =                            sizeof(unsigned short);
    BYTE            abBuf[sizeof(unsigned short)];

    ASSERT (in_pStream != NULL);
    if (in_pStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    leWriteUnsignedShort(in_Val, abBuf);
    iRet =  streamWrite(in_pStream, abBuf, &cb);
    return iRet;
}

/*============================================================================
  Description:  (API Call) Writes a long to a stream
  Arguments: 
   in_pStream - pointer to stream to write to
   in_lVal -    value to write
  Return:
   Status code
  ==========================================================================*/
int
writeLElong(Stream* in_pStream,
            int in_lVal)
{
    int             iRet =                  SSTG_OK;
    UINT4   cb =                    sizeof(long);
    BYTE            abBuf[sizeof(long)];

    ASSERT (in_pStream != NULL);
    if (in_pStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    leWriteLong(in_lVal, abBuf);
    iRet =  streamWrite(in_pStream, abBuf, &cb);
    return iRet;
}

/*============================================================================
  Description:  (API Call) Writes a DWORD (unsigned 32-bit value) to a stream
  Arguments: 
   in_pStream - pointer to stream to write to
   in_ulVal -   value to write
  Return:
   Status code
  ==========================================================================*/
int writeLEdword(Stream* in_pStream,
                 unsigned int in_ulVal)
{
    int             iRet =                          SSTG_OK;
    UINT4  cb =                            sizeof(unsigned long);
    BYTE            abBuf[sizeof(unsigned long)];

    ASSERT (in_pStream != NULL);
    if (in_pStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    leWriteUnsignedLong(in_ulVal, abBuf);
    iRet =  streamWrite(in_pStream, abBuf, &cb);
    return iRet;
}

/*============================================================================
  Description:  (API Call) Writes a float to a stream
  Arguments: 
   in_pStream - pointer to stream to write to
   in_fVal -    value to write
  Return:
   Status code
  ==========================================================================*/
int
writeLEfloat(Stream* in_pStream,
             float in_fVal)
{
    int             iRet =                      SSTG_OK;
    UINT4   cb =                        sizeof(float);
    BYTE            abBuf[sizeof(float)];

    ASSERT (in_pStream != NULL);
    if (in_pStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    leWriteFloat(in_fVal, abBuf);
    iRet =  streamWrite(in_pStream, abBuf, &cb);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Writes a double to a stream
  Arguments: 
   in_pStream - pointer to stream to write to
   in_lfVal -   value to write
  Return:
   Status code
  ==========================================================================*/
int writeLEdouble(Stream* in_pStream,
                  double in_lfVal)
{
    int             iRet =                  SSTG_OK;
    UINT4   cb =                    sizeof(double);
    BYTE            abBuf[sizeof(double)];

    ASSERT (in_pStream != NULL);
    if (in_pStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    leWriteDouble(in_lfVal, abBuf);
    iRet =  streamWrite(in_pStream, abBuf, &cb);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Writes a wide character to a stream
                NOTE: on systems where a wchar_t is 4 bytes, this method will
                demote the 4-byte value to a 2-byte wide character and write 
                only the 2-byte value to the file.
  Arguments: 
   in_pStream - pointer to stream to write to
   in_wchVal -  value to write
  Return:
   Status code
  ==========================================================================*/
int writeLEwchar(Stream* in_pStream,
                 wchar_t in_wchVal)
{
    int                 iRet =                  SSTG_OK;
    UINT4       cb =                    sizeof(UINT2);
    BYTE                abBuf[sizeof(UINT2)];

    ASSERT (in_pStream != NULL);
    if (in_pStream == NULL)
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    leWriteUnsignedShort((UINT2)in_wchVal, abBuf);
    iRet =  streamWrite(in_pStream, abBuf, &cb);
    return iRet;
}


/*============================================================================
  Description:  (API Call) Writes a wide string to a stream
                NOTE: on systems where a wchar_t is 4 bytes, this method will
                demote the 4-byte value to a 2-byte wide character and write 
                only the 2-byte value to the file.
  Arguments: 
   in_pStream -   pointer to stream to write to
   in_wszString - string to write
   io_piCount -   number of characters to write.  If 
  Return:
   Status code
  ==========================================================================*/
int writeLEwstring(Stream* in_pStream,
                   const wchar_t* in_wszString,
                   int* io_piCount)
{
    UINT4 cch =         0;
    UINT4 i =           0;
    SINT4 iRet =        SSTG_OK;

    ASSERT (in_pStream != NULL);
    ASSERT (in_wszString != NULL);
    ASSERT (io_piCount !=  NULL);
    if ((in_pStream == NULL) ||
            (in_wszString == NULL) ||
            (io_piCount == NULL))
    {
        return (int)SSTG_ERROR_ILLEGAL_CALL;
    }

    /* Figure out the number of characters we're supposed to write */
    if (*io_piCount != 0)
    {
        cch = *io_piCount;
    }
    else
    {
        cch = wcslen (in_wszString) + 1;
    }

    /* Write characters one-by-one */
    for (i = 0; i < cch; i++)
    {
        iRet = writeLEwchar(in_pStream, in_wszString[i]);
        if (iRet != SSTG_OK)
        {
            break;
        }
    }

    *io_piCount = i;

    return iRet;
}

#ifdef DEBUG

/*============================================================================
  Description:  (Debug only) Asserts that the stream's sector chain array 
                properly matches with the FAT representation of the sector 
                chain
  Arguments: 
   in_pStream -   pointer to stream to validate
  Return:
   Status code
  ==========================================================================*/
SINT4
assertSectorChainIntegrity(Stream* in_pStream)
{
    RootStorage*    pRoot =         in_pStream->pRoot;
    Fat*            pFAT =          NULL;
    TOC*            pTOC =          NULL;
    DirectoryEntry* pDirEntry =     NULL;
    UINT4           ulStartSect =   0xfffffffe;
    UINT4           ulFatChainLen = (UINT4)-1;
    UINT4*          pulFatChain =   NULL;
    SINT4           iRet =          SSTG_OK;
    UINT4           i =             0;

    pFAT = rootStorageGetFat(pRoot);
    pTOC = rootStorageGetTOC(in_pStream->pRoot);
    pDirEntry = tocGetEntryAtIndex(pTOC, in_pStream->ulDirEntry);

    ulStartSect = directoryGetStartSector(pDirEntry);
    ASSERT((in_pStream->pS->ulSectorChainLen == 0) || 
           (ulStartSect == in_pStream->pS->pulSectorChain[0]));
    ASSERT((in_pStream->pS->ulSectorChainLen != 0) || 
           (ulStartSect == (UINT4)-1));

    if (ulStartSect != (UINT4)-1)
    {
        iRet = fatGetChain(pFAT, ulStartSect, &ulFatChainLen, &pulFatChain);
        ASSERT(iRet == SSTG_OK);
        ASSERT(ulFatChainLen == in_pStream->pS->ulSectorChainLen);

        for (i = 0; i < ulFatChainLen; i++)
        {
            ASSERT(pulFatChain[i] == in_pStream->pS->pulSectorChain[i]);
        }

        free(pulFatChain);
    }

    return SSTG_OK;
}


/*============================================================================
  Description:  (Debug only) Asserts that the mini stream's length and FAT
                chain agree
  Arguments: 
   in_pRoot -   root storage
  Return:
   Status code
  ==========================================================================*/
SINT4 
assertMiniStreamIntegrity(RootStorage* in_pRoot)
{
    Fat*            pFAT =          NULL;
    TOC*            pTOC =          NULL;
    DirectoryEntry* pDirEntry =     NULL;
    Header*         pHdr =          NULL;
    UINT8           ullLength =     0;
    UINT4           ulStartSector = 0;
    UINT4*          pulChain =      0;
    UINT4           ulChainLen =    (UINT4)-1;
    SINT4           iRet =          SSTG_OK;
    UINT2           wSectorShift =  0;
    UINT8           ullCalcdLength= 0;
    

    pFAT = rootStorageGetFat(in_pRoot);
    pHdr = rootStorageGetHeader(in_pRoot);
    pTOC = rootStorageGetTOC(in_pRoot);
    pDirEntry = tocGetEntryAtIndex(pTOC, 0);
    ullLength = directoryGetStreamLength (pDirEntry);
    ulStartSector = directoryGetStartSector(pDirEntry);
    wSectorShift = headerGetSectorShift(pHdr);
    if (ulStartSector == ((UINT4)-1) || ulStartSector == ((UINT4)-2))
    {
        ASSERT(ullLength == 0);
    }
    else
    {
        iRet = fatGetChain(pFAT, ulStartSector, &ulChainLen, &pulChain);

        ullCalcdLength = (ulChainLen * (1 << wSectorShift));

        ASSERT(ullLength <= ullCalcdLength);
        ASSERT(ullLength > (ullCalcdLength - (1 << wSectorShift)));
        ASSERT(rootStorageGetLastMiniStreamSector(in_pRoot) == 
               pulChain[ulChainLen - 1]);

        if (pulChain != NULL)
        {
            free(pulChain);
        }
    }

    return iRet;
}


/*============================================================================
  Description: Self-test of the stream ADT.  Writes data to disk and reads
               it back in again, then compares the results.
  Return: 
   Status code
  ==========================================================================*/
#define WSZ_TEST_FILENAME L"testSSRW.bin"
#define WSZ_TEST_STREAM L"TestStream"

SINT4
streamSelfTest1()
{
    SINT4           iRet =                  SSTG_OK;
    RootStorage*    pRoot =                 NULL;
    Storage*        pStorage =              NULL;
    Stream*         pStream =               NULL;
    StgInfo*        pStgInfo =              NULL;
    short           sVal =                  0;
    unsigned short  wVal =                  0;
    long            lVal =                  0;
    unsigned long   ulVal =                 0;
    float           fVal =                  0.0f;
    double          lfVal =                 0.0;
    wchar_t         wchVal =                L'A';
    wchar_t         pwchBuf[] =             L"Header";
    unsigned long   pos =                   0;
    int             cch =                   0;
    wchar_t         pwchBuf2[10];

    /* Test writing a bunch of data to a stream */
    iRet = createStructuredStorageEx (WSZ_TEST_FILENAME, STG_WRITE, &pRoot, 0);
    ASSERT (iRet == SSTG_OK);

    iRet = getStorageFromRoot (pRoot, &pStorage);
    ASSERT (iRet == SSTG_OK);

    iRet = createStream (pStorage, WSZ_TEST_STREAM, &pStream);
    ASSERT (iRet == SSTG_OK);

    cch = 0;
    ASSERT (writeLEwstring(pStream, pwchBuf, &cch) == SSTG_OK && cch == 7);
    cch = 6;
    ASSERT (writeLEwstring(pStream, pwchBuf, &cch) == SSTG_OK && cch == 6);
    ASSERT (writeLEshort(pStream, (short)2) == SSTG_OK);
    ASSERT (writeLEword(pStream, (unsigned short)3) == SSTG_OK);
    ASSERT (writeLElong(pStream, (long)4) == SSTG_OK);
    ASSERT (writeLEdword(pStream, (unsigned long)5) == SSTG_OK);
    ASSERT (writeLEfloat(pStream, 6.0f) == SSTG_OK);
    ASSERT (writeLEdouble(pStream, 7.0) == SSTG_OK);
    ASSERT (writeLEwchar(pStream, (wchar_t)'A') == SSTG_OK);

    /* Test metadata retrieval from written streams */
    iRet = getStreamInfo(pStream, &pStgInfo);
    ASSERT((iRet == SSTG_OK) && (pStgInfo != NULL));
    ASSERT(wcscmp(pStgInfo->sName, WSZ_TEST_STREAM) == 0);
    ASSERT(pStgInfo->type == STREAM);
    ASSERT(pStgInfo->accessMode == STG_WRITE);
    iRet = freeInfo(&pStgInfo, 1);
    ASSERT(iRet == SSTG_OK && pStgInfo == NULL);

    iRet = closeStream (&pStream);
    ASSERT (iRet == SSTG_OK && pStream == NULL);

    iRet = closeStructuredStorage (&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Test reading the data back and comparing */
    iRet = openStructuredStorageEx (WSZ_TEST_FILENAME, STG_READ, &pRoot);
    ASSERT (iRet == SSTG_OK && pRoot != NULL);

    iRet = getStorageFromRoot (pRoot, &pStorage);
    ASSERT (iRet == SSTG_OK);

    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pStream);
    ASSERT (iRet == SSTG_OK);

    cch = 7;
    memset(pwchBuf2, 0xcc, sizeof(pwchBuf2));
    ASSERT (readLEwstring(pStream, &cch, pwchBuf2) == SSTG_OK && cch == 7);
    ASSERT (memcmp(pwchBuf, pwchBuf2, 7 * sizeof(wchar_t)) == 0);
    cch = 6;
    memset(pwchBuf2, 0xcc, sizeof(pwchBuf2));
    ASSERT (readLEwstring(pStream, &cch, pwchBuf2) == SSTG_OK && cch == 6);
    ASSERT (memcmp(pwchBuf, pwchBuf2, 6 * sizeof(wchar_t)) == 0);
    ASSERT (readLEshort(pStream, &sVal) == SSTG_OK && sVal == (short)2);
    ASSERT (readLEword(pStream, &wVal) == SSTG_OK && wVal == (unsigned short)3);
    ASSERT (readLElong(pStream, &lVal) == SSTG_OK && lVal == (long)4);
    ASSERT (readLEdword(pStream, &ulVal) == SSTG_OK && ulVal == (unsigned long)5);
    ASSERT (readLEfloat(pStream, &fVal) == SSTG_OK && fVal == 6.0f);
    ASSERT (readLEdouble(pStream, &lfVal) == SSTG_OK && lfVal == 7.0);
    ASSERT (readLEwchar(pStream, &wchVal) == SSTG_OK && wchVal == L'A');

    /* Now test seeking and getting the stream position.  The size of the stream
     * should be 52 bytes. */
    ASSERT (streamSeek (pStream, 0, STG_START) == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 0);
    ASSERT (streamSeek (pStream, 2, STG_START) == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 2);
    ASSERT (readLEwchar(pStream, &wchVal) == SSTG_OK && wchVal == L'e');
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 4);
    ASSERT (streamSeek (pStream, 2, STG_CUR) == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 6);
    ASSERT (streamSeek (pStream, -6, STG_CUR) == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 0);
    ASSERT (streamSeek (pStream, 54, STG_START) == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 54);
    ASSERT (readLEwchar(pStream, &wchVal) == SSTG_ERROR_END_OF_STREAM);
    ASSERT (streamSeek (pStream, 52, STG_START) == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 52);
    ASSERT (readLEwchar(pStream, &wchVal) == SSTG_ERROR_END_OF_STREAM);
    ASSERT (streamSeek (pStream, -2, STG_END) == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 50);
    ASSERT (readLEwchar(pStream, &wchVal) == SSTG_OK && wchVal == L'A');
    ASSERT (streamSeek (pStream, -52, STG_END) == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 0);
    ASSERT (readLEwchar(pStream, &wchVal) == SSTG_OK && wchVal == L'H');
    ASSERT (streamSeek (pStream, 2, STG_END) == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 54);
    ASSERT (readLEwchar(pStream, &wchVal) == SSTG_ERROR_END_OF_STREAM);

    /* Test metadata retrieval */
    iRet = getStreamInfo(pStream, &pStgInfo);
    ASSERT(iRet == SSTG_OK && (pStgInfo != NULL));
    ASSERT(wcscmp(pStgInfo->sName, WSZ_TEST_STREAM) == 0);
    ASSERT(pStgInfo->type == STREAM);
    ASSERT(pStgInfo->size == 52);
    ASSERT(pStgInfo->accessMode == STG_READ);
    iRet = freeInfo(&pStgInfo, 1);
    ASSERT(iRet == SSTG_OK && pStgInfo == NULL);

    iRet = closeStream (&pStream);
    ASSERT (iRet == SSTG_OK && pStream == NULL);

    iRet = closeStructuredStorage (&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Remove test file */
    SSRW_WREMOVE (WSZ_TEST_FILENAME);

    return SSTG_OK;
}


SINT4
streamSelfTestResizing()
{
    SINT4           iRet =          SSTG_OK;
    RootStorage*    pRoot =         NULL;
    Storage*        pStorage =      NULL;
    Stream*         pStream =       NULL;
    unsigned long   pos =           0;
    char            ch =            0;
    unsigned long   cb =            0;
    SINT4           i =             0;

    /* Test resizing a stream in write-only mode */
    iRet = createStructuredStorageEx (WSZ_TEST_FILENAME, STG_WRITE, &pRoot, 0);
    ASSERT (iRet == SSTG_OK);

    iRet = getStorageFromRoot (pRoot, &pStorage);
    ASSERT (iRet == SSTG_OK);

    iRet = createStream (pStorage, WSZ_TEST_STREAM, &pStream);
    ASSERT (iRet == SSTG_OK);

    for (i = 0; i < 256; i++)
    {
        ch = (CHAR)(i % 256);
        cb = 1;
        iRet = streamWrite(pStream, &ch, &cb);
        ASSERT ((iRet == SSTG_OK) && (cb == 1));
    }

    iRet = streamResize(pStream, 732);
    ASSERT (iRet == SSTG_OK);

    iRet = streamResize(pStream, 512);
    ASSERT (iRet == SSTG_OK);

    iRet = streamResize(pStream, 256);
    ASSERT (iRet == SSTG_OK);

    iRet = streamResize(pStream, 8000);
    ASSERT (iRet == SSTG_OK);

    iRet = streamResize(pStream, 248);
    ASSERT (iRet == SSTG_OK);

    iRet = streamSeek(pStream, 247, STG_START);
    ASSERT(iRet == SSTG_OK);


    iRet = closeStream(&pStream);
    ASSERT (iRet == SSTG_OK);

    iRet = closeStructuredStorage (&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Test resizing a stream in read/write mode */
    iRet = openStructuredStorageEx (WSZ_TEST_FILENAME, STG_RW, &pRoot);
    ASSERT (iRet == SSTG_OK && pRoot != NULL);

    iRet = getStorageFromRoot (pRoot, &pStorage);
    ASSERT (iRet == SSTG_OK);

    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pStream);
    ASSERT (iRet == SSTG_OK);

#ifdef DEBUG
    assertSectorChainIntegrity(pStream);
#endif /* DEBUG */

    iRet = streamSeek(pStream, 0, STG_END);
    ASSERT (iRet == SSTG_OK);

#ifndef SSRW_RESIZE_STREAM_ON_CLOSE
    ASSERT ((streamGetPos(pStream, &pos) == SSTG_OK) && (pos == 248));
	iRet = streamResize(pStream, 247);
	ASSERT(iRet == SSTG_OK);
    iRet = streamSeek(pStream, 0, STG_END);
    ASSERT (iRet == SSTG_OK);
#endif /* SSRW_RESIZE_STREAM_ON_CLOSE */

    ASSERT ((streamGetPos(pStream, &pos) == SSTG_OK) && (pos == 247));
    ASSERT (iRet == SSTG_OK);

    iRet = streamSeek(pStream, 0, STG_START);
    ASSERT (iRet == SSTG_OK);

    /* First check that the data from the STG_WRITE test is all there and 
     * correct */
    for (i = 0; i < 247; i++)
    {
        cb = 1;
        iRet = streamRead(pStream, &ch, &cb);
        ASSERT ((iRet == SSTG_OK) && (cb == 1) && (((BYTE)ch) == i));
    }


    /* Make sure we fail correctly when at end-of-stream */
    cb = 1;
    ch = 13;
    iRet = streamRead(pStream, &ch, &cb);
    ASSERT((iRet == SSTG_ERROR_END_OF_STREAM) && (cb == 0) && (ch == 13));

#ifdef DEBUG
    assertSectorChainIntegrity(pStream);
#endif /* DEBUG */

    /* Write in some additional data */
    for(i = 247; i < 256; i++)
    {
        ch = (CHAR)(i % 256);
        cb = 1;
        iRet = streamWrite(pStream, &ch, &cb);
        ASSERT ((iRet == SSTG_OK) && (cb == 1));
    }

    iRet = streamResize(pStream, 8000);
    ASSERT (iRet == SSTG_OK);

    iRet = streamSeek(pStream, 8000, STG_START);
    ASSERT (iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 8000);

    /* And write some data again */
    for(i = 0; i < 256; i++)
    {
        ch = (CHAR)(i % 256);
        cb = 1;
        iRet = streamWrite(pStream, &ch, &cb);
        ASSERT ((iRet == SSTG_OK) && (cb == 1));
    }

    iRet = streamSeek(pStream, 256, STG_START);
    ASSERT (iRet == SSTG_OK);

    /* And again */
    for(i = 0; i < 256; i++)
    {
        ch = (CHAR)(i % 256);
        cb = 1;
        iRet = streamWrite(pStream, &ch, &cb);
        ASSERT ((iRet == SSTG_OK) && (cb == 1));
    }

    iRet = streamResize(pStream, 8128);
    ASSERT (iRet == SSTG_OK);

    iRet = streamSeek(pStream, 0, STG_END);
    ASSERT(iRet == SSTG_OK);

    iRet = closeStream(&pStream);
    ASSERT (iRet == SSTG_OK);

    /* Open the stream one more time to verify its content */
    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pStream);
    ASSERT (iRet == SSTG_OK);

    iRet = streamSeek(pStream, 0, STG_END);
    ASSERT (iRet == SSTG_OK);

    ASSERT ((streamGetPos(pStream, &pos) == SSTG_OK) && (pos == 8128));
    ASSERT (iRet == SSTG_OK);

    iRet = streamSeek(pStream, 0, STG_START);
    ASSERT (iRet == SSTG_OK);

    /* Check that the data is all there and correct */
    for (i = 0; i < 512; i++)
    {
        cb = 1;
        iRet = streamRead(pStream, &ch, &cb);
        ASSERT ((iRet == SSTG_OK) && (cb == 1) && (((BYTE)ch) == i % 256));
    }

    iRet = streamSeek(pStream, 8000, STG_START);
    ASSERT (iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 8000);

    /* And check the data at the end */
    for (i = 0; i < 128; i++)
    {
        cb = 1;
        iRet = streamRead(pStream, &ch, &cb);
        ASSERT ((iRet == SSTG_OK) && (cb == 1) && (((BYTE)ch) == i % 256));
    }

    /* Make sure we fail correctly when at end-of-stream */
    cb = 1;
    ch = 13;
    iRet = streamRead(pStream, &ch, &cb);
    ASSERT((iRet == SSTG_ERROR_END_OF_STREAM) && (cb == 0) && (ch == 13));

    iRet = closeStream(&pStream);
    ASSERT (iRet == SSTG_OK);

    iRet = closeStructuredStorage (&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Remove test file */
    SSRW_WREMOVE (WSZ_TEST_FILENAME);
    return iRet;
}


SINT4
streamSelfTestSeeking()
{
    SINT4               iRet =          SSTG_OK;
    RootStorage*        pRoot =         NULL;
    Storage*            pStorage =      NULL;
    Stream*             pStream =       NULL;
    unsigned long       pos =           0;
    unsigned long       cb =            0;
    char*               pOne =          "one       ";
    char*               pTwo =          "two       ";
    char*               pThree =        "three     ";
    char*               pFour =         "four      ";
    char*               pSeven =        "seven     ";
    char*               pEight =        "eight     ";
    char*               pNine =         "nine      ";
    char*               pTen =          "ten       ";
    char                buf[11];

    buf[10] = 0;

    /* Test seeking inside a stream in write-only mode */
    iRet = createStructuredStorageEx (WSZ_TEST_FILENAME, STG_WRITE, &pRoot, 0);
    ASSERT (iRet == SSTG_OK);

    iRet = getStorageFromRoot (pRoot, &pStorage);
    ASSERT (iRet == SSTG_OK);

    iRet = createStream (pStorage, WSZ_TEST_STREAM, &pStream);
    ASSERT (iRet == SSTG_OK);

    /* Write data in random order */
    cb = 10;
    iRet = streamWrite(pStream, pOne, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    iRet = streamSeek(pStream, 30, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 30);
    iRet = streamWrite(pStream, pFour, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    iRet = streamSeek(pStream, 20, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 20);
    iRet = streamWrite(pStream, pTen, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    iRet = streamSeek(pStream, 90, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 90);
    iRet = streamWrite(pStream, pTen, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    iRet = streamSeek(pStream, 10097, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 10097);
    iRet = streamWrite(pStream, pEight, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    iRet = streamSeek(pStream, 20, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 20);
    iRet = streamWrite(pStream, pThree, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    /* Set the seek pointer to the end of the file or data will be lost */
    iRet = streamSeek(pStream, 10107, STG_START);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 10107);
    ASSERT(iRet == SSTG_OK);

    /* Clean up */
    iRet = closeStream(&pStream);
    ASSERT (iRet == SSTG_OK);

    iRet = closeStructuredStorage (&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Test seeking inside a stream in read-write mode */
    iRet = openStructuredStorageEx (WSZ_TEST_FILENAME, STG_RW, &pRoot);
    ASSERT (iRet == SSTG_OK);

    iRet = getStorageFromRoot (pRoot, &pStorage);
    ASSERT (iRet == SSTG_OK);

    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pStream);
    ASSERT (iRet == SSTG_OK);

    /* Write data in random order */
    cb = 10;
    iRet = streamSeek(pStream, 10, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 10);
    iRet = streamWrite(pStream, pTwo, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    iRet = streamSeek(pStream, 0, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 0);
    iRet = streamWrite(pStream, pSeven, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    iRet = streamSeek(pStream, 80, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 80);
    iRet = streamWrite(pStream, pNine, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    iRet = streamSeek(pStream, 20097, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 20097);
    iRet = streamWrite(pStream, pSeven, &cb);
    ASSERT(iRet == SSTG_OK && cb == 10);

    /* Clean up */
    iRet = closeStream(&pStream);
    ASSERT (iRet == SSTG_OK);

    iRet = closeStructuredStorage (&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Test seeking inside a read-only structured storage file */
    iRet = openStructuredStorageEx (WSZ_TEST_FILENAME, STG_READ, &pRoot);
    ASSERT (iRet == SSTG_OK);

    iRet = getStorageFromRoot (pRoot, &pStorage);
    ASSERT (iRet == SSTG_OK);

    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pStream);
    ASSERT (iRet == SSTG_OK);

    /* Test seeking and reading beyond the bounds of the file */
    cb = 10;
    iRet = streamSeek(pStream, 30097, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 30097);
    iRet = streamRead(pStream, buf, &cb);
    ASSERT(iRet == SSTG_ERROR_END_OF_STREAM && cb == 0);

    /* Verify data in reverse order */
    cb = 10;
    iRet = streamSeek(pStream, 20097, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 20097);
    iRet = streamRead(pStream, buf, &cb);
    ASSERT((iRet == SSTG_OK) && (cb == 10) && (strcmp(pSeven, buf) == 0));

    iRet = streamSeek(pStream, 10097, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 10097);
    iRet = streamRead(pStream, buf, &cb);
    ASSERT((iRet == SSTG_OK) && (cb == 10) && (strcmp(pEight, buf) == 0));

    iRet = streamSeek(pStream, 90, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 90);
    iRet = streamRead(pStream, buf, &cb);
    ASSERT((iRet == SSTG_OK) && (cb == 10) && (strcmp(pTen, buf) == 0));

    iRet = streamSeek(pStream, 80, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 80);
    iRet = streamRead(pStream, buf, &cb);
    ASSERT((iRet == SSTG_OK) && (cb == 10) && (strcmp(pNine, buf) == 0));

    iRet = streamSeek(pStream, 30, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 30);
    iRet = streamRead(pStream, buf, &cb);
    ASSERT((iRet == SSTG_OK) && (cb == 10) && (strcmp(pFour, buf) == 0));

    iRet = streamSeek(pStream, 20, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 20);
    iRet = streamRead(pStream, buf, &cb);
    ASSERT((iRet == SSTG_OK) && (cb == 10) && (strcmp(pThree, buf) == 0));

    iRet = streamSeek(pStream, 10, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 10);
    iRet = streamRead(pStream, buf, &cb);
    ASSERT((iRet == SSTG_OK) && (cb == 10) && (strcmp(pTwo, buf) == 0));

    iRet = streamSeek(pStream, 0, STG_START);
    ASSERT(iRet == SSTG_OK);
    ASSERT (streamGetPos(pStream, &pos) == SSTG_OK && pos == 0);
    iRet = streamRead(pStream, buf, &cb);
    ASSERT((iRet == SSTG_OK) && (cb == 10) && (strcmp(pSeven, buf) == 0));

    /* Clean up */
    iRet = closeStream(&pStream);
    ASSERT (iRet == SSTG_OK);

    iRet = closeStructuredStorage (&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Remove test file */
    SSRW_WREMOVE (WSZ_TEST_FILENAME);
    return iRet;
}


SINT4
streamSelfTestInterleavedRW()
{
    SINT4               iRet =          SSTG_OK;
    RootStorage*        pRoot =         NULL;
    Storage*            pStorage =      NULL;
    Stream*             pWrite1Stream = NULL;
    Stream*             pWrite2Stream = NULL;
    Stream*             pRead1Stream =  NULL;
    Stream*             pRead2Stream =  NULL;
    unsigned long       cb =            0;
    char*               pNumbers[] =  { "one       ",
                                        "two       ",
                                        "three     ",
                                        "four      ",
                                        "five      ",
                                        "six       ",
                                        "seven     ",
                                        "eight     ",
                                        "nine      ",
                                        "ten       " };
    char                buf1[11];
    char                buf2[11];
    SINT4               i =             0;

    buf1[10] = 0;
    buf2[10] = 0;

    /* 1.  Test creating a stream, writing it while simultaneously reading & 
     * verifying data */
    iRet = createStructuredStorageEx (WSZ_TEST_FILENAME, STG_RW, &pRoot, 0);
    ASSERT (iRet == SSTG_OK);

    iRet = getStorageFromRoot (pRoot, &pStorage);
    ASSERT (iRet == SSTG_OK);

    iRet = createStream (pStorage, WSZ_TEST_STREAM, &pWrite1Stream);
    ASSERT (iRet == SSTG_OK);

    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pRead1Stream);
    ASSERT (iRet == SSTG_OK);

    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pRead2Stream);
    ASSERT (iRet == SSTG_OK);

    for (i = 0; i < 1000; i++)
    {
        cb = 10;
        iRet = streamWrite(pWrite1Stream, pNumbers[i % 10], &cb);
        ASSERT((iRet == SSTG_OK) && (cb == 10));

        iRet = streamFlushToDisk(pWrite1Stream);
        ASSERT(iRet == SSTG_OK);

        iRet = streamRead(pRead1Stream, buf1, &cb);
        ASSERT(iRet == SSTG_OK && cb == 10);
        ASSERT(strcmp(pNumbers[i % 10], buf1) == 0);

        iRet = streamRead(pRead2Stream, buf2, &cb);
        ASSERT(iRet == SSTG_OK && cb == 10);
        ASSERT(strcmp(pNumbers[i % 10], buf2) == 0);
    }
    iRet = closeStream(&pWrite1Stream);
    ASSERT((iRet == SSTG_OK) && (pWrite1Stream == NULL));
    iRet = closeStream(&pRead1Stream);
    ASSERT((iRet == SSTG_OK) && (pRead1Stream == NULL));
    iRet = closeStream(&pRead2Stream);
    ASSERT((iRet == SSTG_OK) && (pRead2Stream == NULL));

    /* 2.  Test opening a mini-stream, then opening it for writing and turning
     * it into a regular stream, all the while maintaining data integrity */

    /* First resize the last stream so that it becomes a small stream */
    iRet = openStream(pStorage, WSZ_TEST_STREAM, &pWrite1Stream);
    ASSERT(iRet == SSTG_OK);
    iRet = streamResize(pWrite1Stream, 2048);
    ASSERT(iRet == SSTG_OK);
    iRet = streamSeek(pWrite1Stream, 0, STG_END);
    ASSERT(iRet == SSTG_OK);
    iRet = closeStream(&pWrite1Stream);
    ASSERT(iRet == SSTG_OK);

    /* Now open it and verify the first 2000 bytes */
    iRet = openStream(pStorage, WSZ_TEST_STREAM, &pRead1Stream);
    ASSERT(iRet == SSTG_OK);
    for (i = 0; i < 200; i++)
    {
        cb = 10;

        iRet = streamRead(pRead1Stream, buf1, &cb);
        ASSERT(iRet == SSTG_OK && cb == 10);
        ASSERT(strcmp(pNumbers[i % 10], buf1) == 0);
    }

    /* Write 6000 bytes of data on top of what was there before.
     * This should cause the stream to turn into a regular stream. */
    iRet = openStream(pStorage, WSZ_TEST_STREAM, &pWrite1Stream);
    ASSERT(iRet == SSTG_OK);
    for (i = 0; i < 600; i++)
    {
        cb = 10;
        iRet = streamWrite(pWrite1Stream, pNumbers[9 - (i % 10)], &cb);
        ASSERT((iRet == SSTG_OK) && (cb == 10));

    }
    iRet = closeStream(&pWrite1Stream);
    ASSERT(iRet == SSTG_OK);

    /* Now verify 4000 more bytes with the read stream, including the
     * data just written. */
    for (i = 0; i < 400; i++)
    {
        cb = 10;

        iRet = streamRead(pRead1Stream, buf1, &cb);
        ASSERT(iRet == SSTG_OK && cb == 10);
        ASSERT(strcmp(pNumbers[9 - (i % 10)], buf1) == 0);
    }
    iRet = closeStream(&pRead1Stream);
    ASSERT(iRet == SSTG_OK);

    /* 3.  Two writers, two readers.  Each writer is teamed up with a reader.
     * One team writes and verifies forward from the beginning of the file.
     * The other team writes and verifies backward.  They pass each other in 
     * the middle.   Finally, the whole works is verified together. */
    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pWrite1Stream);
    ASSERT (iRet == SSTG_OK);

    iRet = streamResize(pWrite1Stream, 0);
    ASSERT (iRet == SSTG_OK);

    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pWrite2Stream);
    ASSERT (iRet == SSTG_OK);

    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pRead1Stream);
    ASSERT (iRet == SSTG_OK);

    iRet = openStream (pStorage, WSZ_TEST_STREAM, &pRead2Stream);
    ASSERT (iRet == SSTG_OK);

    for (i = 0; i < 30; i++)
    {
        cb = 10;

        /* (a) Write to write stream 1; verify with read stream 1 */
        iRet = streamSeek(pWrite1Stream, 20 * i, STG_START);
        ASSERT((iRet == SSTG_OK));

        iRet = streamWrite(pWrite1Stream, pNumbers[(i * 2) % 10], &cb);
        ASSERT((iRet == SSTG_OK) && (cb == 10));

        iRet = streamFlushToDisk(pWrite1Stream);
        ASSERT(iRet == SSTG_OK);

        iRet = streamSeek(pRead1Stream, 20 * i, STG_START);
        ASSERT((iRet == SSTG_OK));

        iRet = streamRead(pRead1Stream, buf1, &cb);
        ASSERT(iRet == SSTG_OK && cb == 10);

        ASSERT(strcmp(pNumbers[(i * 2) % 10], buf1) == 0);

        /* (b) Write backwards to write stream 2; verify with read stream 2 */
        iRet = streamSeek(pWrite2Stream, 590 - 20 * i, STG_START);
        ASSERT((iRet == SSTG_OK));

        iRet = streamWrite(pWrite2Stream, pNumbers[9 - ((i * 2) % 10)], &cb);
        ASSERT((iRet == SSTG_OK) && (cb == 10));

        iRet = streamFlushToDisk(pWrite2Stream);
        ASSERT(iRet == SSTG_OK);

        iRet = streamSeek(pRead2Stream, 590 - 20 * i, STG_START);
        ASSERT((iRet == SSTG_OK));

        iRet = streamRead(pRead2Stream, buf2, &cb);
        ASSERT(iRet == SSTG_OK && cb == 10);

        ASSERT(strcmp(pNumbers[9 - ((i * 2) % 10)], buf2) == 0);
    }
    iRet = streamSeek(pWrite1Stream, 0, STG_END);
    ASSERT((iRet == SSTG_OK));
    iRet = streamSeek(pWrite2Stream, 0, STG_END);
    ASSERT((iRet == SSTG_OK));
    iRet = streamSeek(pRead2Stream, 0, STG_END);
    ASSERT((iRet == SSTG_OK));
    iRet = closeStream(&pWrite1Stream);
    ASSERT((iRet == SSTG_OK) && (pWrite1Stream == NULL));
    iRet = closeStream(&pWrite2Stream);
    ASSERT((iRet == SSTG_OK) && (pWrite2Stream == NULL));
    iRet = closeStream(&pRead2Stream);
    ASSERT((iRet == SSTG_OK) && (pRead2Stream == NULL));

    iRet = streamSeek(pRead1Stream, 0, STG_START);
    ASSERT((iRet == SSTG_OK));

    /* Now verify the 600 bytes just written one more time */
    for (i = 0; i < 60; i++)
    {
        cb = 10;

        iRet = streamRead(pRead1Stream, buf1, &cb);
        ASSERT(iRet == SSTG_OK && cb == 10);
        ASSERT(strcmp(pNumbers[i % 10], buf1) == 0);
    }

    iRet = closeStream(&pRead1Stream);
    ASSERT((iRet == SSTG_OK) && (pRead1Stream == NULL));

    iRet = closeStructuredStorage (&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Remove test file */
    SSRW_WREMOVE (WSZ_TEST_FILENAME);

    return SSTG_OK;
}


/*============================================================================
   Description: This tests the ability of the structured storage file to 
                recover free small sectors into the regular FAT free list 
                without failing
   Return:
    Status code
  ==========================================================================*/
SINT4
streamSelfTestSmallSectorRecovery()
{
    SINT4               iRet =          SSTG_OK;
    RootStorage*        pRoot =         NULL;
    Storage*            pStorage =      NULL;
    UINT4               cb =            0;

    iRet = createStructuredStorageEx (WSZ_TEST_FILENAME, STG_RW, &pRoot, 0);
    ASSERT (iRet == SSTG_OK);

    iRet = getStorageFromRoot (pRoot, &pStorage);
    ASSERT (iRet == SSTG_OK);

    cb = 5;
    flushStream(pStorage, L"A", "AAAAA", &cb);
    ASSERT(iRet == SSTG_OK && cb == 5);
    flushStream(pStorage, L"B", "BBBBB", &cb);
    ASSERT(iRet == SSTG_OK && cb == 5);
    flushStream(pStorage, L"C", "CCCCC", &cb);
    ASSERT(iRet == SSTG_OK && cb == 5);

    iRet = destroy(pStorage, L"C");
    ASSERT(iRet == SSTG_OK);
    iRet = destroy(pStorage, L"A");
    ASSERT(iRet == SSTG_OK);
    iRet = destroy(pStorage, L"B");
    ASSERT(iRet == SSTG_OK);

    iRet = closeStructuredStorage (&pRoot);
    ASSERT (iRet == SSTG_OK && pRoot == NULL);

    /* Remove test file */
    SSRW_WREMOVE (WSZ_TEST_FILENAME);

    return SSTG_OK;
}

SINT4
streamSelfTest()
{
    SINT4 iRet = SSTG_OK;

    iRet = streamSelfTestSmallSectorRecovery();
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = streamSelfTest1();
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = streamSelfTestResizing();
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = streamSelfTestSeeking();
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = streamSelfTestInterleavedRW();
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    return SSTG_OK;
}

#endif /* DEBUG */

