/******************************************************************************
* RootStorage.c
*
* Functions supporting compound file connections
*
* (c) Schema Software Inc., 2001-2004
* 
*******************************************************************************

$Revision: 1.16.2.9 $
$Date: 2004/02/26 19:19:44 $
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "StructuredStorage.h"
#include "RootStorage.h"
#include "Header.h"
#include "Directory.h"
#include "Storage.h"
#include "Fat.h"
#include "TOC.h"
#include "LEReadWrite.h"
#include "DocumentProperties.h"
#include "Stream.h"

/* internal structure that contains all information for a sstg */
struct _RootStorage
{
    SSRWIS* pIS;                    /* attached compound file */
    boolean bStgOwnsFilePtr;        /* whether the root storage structure "owns"
                                     * its file pointer, i.e. whether it should
                                     * be responsible for closing it */

    Header* pHeader;                /* file header kept in memory */
    struct _Fat* fat;               /* FAT structure - completely kept in memory
                                     * for now */
    struct _Fat* smallFat;          /* MiniFat */
    TOC* pTOC;                      /* pointer to Table of content manager */
    struct _Storage* rootDir;       /* points to Storage structure representing
                                     * top-level storage */

    StgMode mode;                   /* read or write? */
    UINT4* directoryChain;          /* copy of FAT chain for directory sectors */
    UINT4 directoryChainLen;        /* length of directoryChain */

    UINT4 bufferedSector;           /* index of sector buffered in buffer */
    CHAR* buffer;                   /* buffer containing the contents of a sector
                                     * recently read */

    SINT4 statusCode;               /* contains latest error code */
    UINT4 ulLastMiniStreamSector;   /* last full-sized sector occupied by the
                                     * MiniStream */
    DocumentProperties* pProperties;/* Document properties manager */
    UINT4* pulXFatSectors;          /* The sectors used by the double-indirect
                                       FAT or "extended FAT". */
    UINT8 dirty;                    /* Whether anything has been written to the
                                     * file since opening.  Always true (1) for 
                                     * STG_WRITE, always false (0) for STG_READ,
                                     * set when something changes for STG_RW. */
};

/* forward declarations */
static SINT4 rootStorageInitWrite(RootStorage* in_pRoot);
static SINT4 rootStorageInitTOC(RootStorage* in_pRoot);
static SINT4 rootStorageFinalWrite(RootStorage* in_pRoot);

static void closeStg (RootStorage* in_pRoot);
static SINT4 rootStorageInit (RootStorage* in_pRoot,
                              StgMode mode,
                              UINT4 in_ulSectorSize);
static SINT4 readFatSectorsFromXFat(RootStorage* in_pRoot,
                                    UINT4 *out_alSectors,
                                    UINT4* out_pulXFatSectors);
static SINT4 readSector (RootStorage* in_pRoot,
                         UINT4 in_ulSector );
static SINT4 writeSector (RootStorage* in_pRoot, 
                          BYTE* in_buf, 
                          UINT4* out_pSector,
                          boolean in_isMetadata);
static SINT4 rewriteSector (RootStorage* in_pRoot, BYTE* in_buf, UINT4 in_ulSector);

static SINT4 initializeSmallFat (RootStorage* in_pRoot);
static int openStructuredStorageInternal (SSRWIS* in_pIS,
        RootStorage** out_ppRoot,
        boolean in_bStgInheritsFilePtr,
        StgMode in_accessMode);
static int createStructuredStorageInternal (SSRWIS* in_pIS,
        StgMode in_accessMode,
        RootStorage** out_ppRoot,
        boolean in_bStgInheritsFilePtr,
        UINT4 in_ulSectorSize);




/*============================================================================
  Description:  (API call) Opens a structured storage from a disk file 
  Arguments: 
   in_pFilename - The name of the file
   in_access -    Access mode.  Must be STG_READ or STG_RW.
   out_ppRoot -   pointer to the pointer to the root storage (to be allocated in 
                  the function). 
                  If file does not exist or is already open for reading, the 
                  function  will fail and this will point to a null pointer.
  Return:       
   A status code:
      SSTG_OK success
      SSTG_ERROR_...  for error condition
  ==========================================================================*/
int
openStructuredStorage (
    const char* in_pFilename,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    SINT4    iRet =          SSTG_OK;
    wchar_t* pwFilename =    NULL;
    UINT4    ulFilenameLen = 0;

    ulFilenameLen = strlen(in_pFilename);
    pwFilename = (wchar_t*)malloc((ulFilenameLen + 1) * sizeof(wchar_t));
    if (pwFilename == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }
    mbstowcs(pwFilename, in_pFilename, ulFilenameLen + 1);

    iRet = openStructuredStorageEx(pwFilename, in_accessMode, out_ppRoot);

    free(pwFilename);

    return (int)iRet;
}


/*============================================================================
  Description:  (API call) Opens a structured storage from a disk file 
  Arguments: 
   in_fileName - The name of the file
   in_access -   Access mode.  Must be STG_READ or STG_RW.
   out_ppRoot -  pointer to the pointer to the root storage (to be allocated in 
                 the function). 
                 If file does not exist or is already open for reading, the 
                 function  will fail and this will point to a null pointer.
  Return:       
   A status code:
      SSTG_OK success
      SSTG_ERROR_...  for error condition
  ==========================================================================*/
int
openStructuredStorageEx (
    const wchar_t* in_pwFilename,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    SSRWIS* pIS =   NULL;
    int     iRet =  SSTG_OK;

    ASSERT (in_pwFilename != NULL && *in_pwFilename != 0);
    ASSERT (STG_CANREAD(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    if ((in_pwFilename == NULL) ||
        (*in_pwFilename == 0) ||
        (out_ppRoot == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    if (! STG_CANREAD(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    if ( in_accessMode == STG_READ )
    {
        pIS = SsrwOpenFile(in_pwFilename, L"rb");
    }
    else
    {
        pIS = SsrwOpenFile(in_pwFilename, L"r+b");
    }
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILE_NOT_FOUND;
    }

    iRet = openStructuredStorageInternal(pIS, out_ppRoot, SSRW_TRUE, in_accessMode);

    if (iRet != SSTG_OK)
    {
        SsrwFclose(pIS);
    }

    return iRet;
}

/*============================================================================
  Description:  (API call) Opens a structured storage from an external source
  Arguments: 
   in_pSSRWIS -  pointer to a client-created SSRWIS structure
   in_access -   Access mode.  Must be STG_READ or STG_RW.
   out_ppRoot -  pointer to the pointer to the root storage (to be allocated in 
                 the function). 
                 If file does not exist or is already open for reading, the 
                 function  will fail and this will point to a null pointer.
  Return:       
   A status code:
      SSTG_OK success
      SSTG_ERROR_...  for error condition
  ==========================================================================*/
int
openStructuredStorageInSource (
    SSRWIS* in_pSSRWIS,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    int     iRet =  SSTG_OK;

    ASSERT (in_pSSRWIS != NULL);
    ASSERT (in_pSSRWIS->m_pFread != NULL);
    ASSERT (in_pSSRWIS->m_pFwrite != NULL);
    ASSERT (in_pSSRWIS->m_pFclose != NULL);
    ASSERT (in_pSSRWIS->m_pFseek != NULL);
    ASSERT (in_pSSRWIS->m_pFtell != NULL);
    ASSERT (in_pSSRWIS->m_pFtruncate != NULL);
    ASSERT (STG_CANREAD(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    if ((in_pSSRWIS == NULL) ||
        (out_ppRoot == NULL) ||
        (in_pSSRWIS->m_pFread == NULL) ||
        (in_pSSRWIS->m_pFwrite == NULL) ||
        (in_pSSRWIS->m_pFclose == NULL) ||
        (in_pSSRWIS->m_pFseek == NULL) ||
        (in_pSSRWIS->m_pFtell == NULL) ||
        (in_pSSRWIS->m_pFtruncate == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    if (! STG_CANREAD(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    iRet = openStructuredStorageInternal(in_pSSRWIS, 
                                         out_ppRoot, 
                                         SSRW_TRUE, 
                                         in_accessMode);

    return iRet;
}

#ifdef SSTG_PALMOS

/*============================================================================
  Description:  (API call) Opens a structured storage from a PalmOS VFS file
                system
  Arguments: 
   in_fileName - The name of the file
   in_access -   Access mode.  Must be STG_READ or STG_RW.
   out_ppRoot -  pointer to the pointer to the root storage (to be allocated in 
                 the function). 
                 If file does not exist or is already open for reading, the 
                 function  will fail and this will point to a null pointer.
   in_volRef -   PalmOS VFS volume reference for the expansion card on which
                 the file resides
  Return:       
   A status code:
      SSTG_OK success
      SSTG_ERROR_...  for error condition
  ==========================================================================*/
int
openStructuredStorageInVFS (
    const char* in_fileName,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned short in_volRef)
{
    SSRWIS* pIS =   NULL;
    int     iRet =  SSTG_OK;

    ASSERT (in_fileName != NULL && *in_fileName != 0);
    ASSERT (STG_CANREAD(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    if ((in_fileName == NULL) ||
            (*in_fileName == 0) ||
            (out_ppRoot == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    if (! STG_CANREAD(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    if ( in_accessMode == STG_READ )
    {
        pIS = SsrwOpenVFSFile(in_fileName, "rb", in_volRef);
    }
    else
    {
        pIS = SsrwOpenVFSFile(in_fileName, "rwb", in_volRef);
    }
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILE_NOT_FOUND;
    }

    iRet = openStructuredStorageInternal(pIS, out_ppRoot, SSRW_TRUE, in_accessMode);

    if (iRet != SSTG_OK)
    {
        SsrwFclose(pIS);
    }

    return iRet;
}
#endif /* SSTG_PALMOS */

/*============================================================================
  Description:  (API call) Opens a structured storage from a disk file 
  Arguments: 
   in_ppBuf - Pointer to a pointer to a buffer containing a structured storage
              file in memory.
   in_ulBufSize - the size of the buffer, in bytes
   in_access -   Access mode.  Must be STG_READ or STG_RW.
   out_ppRoot -  pointer to the pointer to the root storage (to be allocated in 
                 the function). 
                 If file does not exist or is already open for reading, the 
                 function  will fail and this will point to a null pointer.
  Return:       
   A status code:
      SSTG_OK success
      SSTG_ERROR_...  for error condition
  ==========================================================================*/
int
openStructuredStorageInBuf (
    const void** in_ppBuf,
    unsigned long in_ulBufSize,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    SSRWIS* pIS =   NULL;
    int     iRet =  SSTG_OK;

    ASSERT (in_ppBuf != NULL);
    ASSERT (*in_ppBuf != NULL);
    ASSERT (STG_CANREAD(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    if ((in_ppBuf == NULL) ||
            (*in_ppBuf == NULL) ||
            (out_ppRoot == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    if (! STG_CANREAD(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pIS = SsrwOpenMemForReading(in_ppBuf, in_ulBufSize);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILE_NOT_FOUND;
    }

    iRet = openStructuredStorageInternal(pIS, out_ppRoot, SSRW_TRUE, in_accessMode);

    if (iRet != SSTG_OK)
    {
        SsrwFclose(pIS);
    }

    return iRet;
}

#ifdef SSTG_PALMOS

/*============================================================================
  Description:  (API call) Opens a structured storage from a Palm file handle
  Arguments: 
   in_pFile -    File pointer.  MUST be open in read-only mode.
   in_access -   Access mode.  Must be STG_READ or STG_RW.
   out_ppRoot -  pointer to the pointer to the root storage (to be allocated in 
                 the function). 
                 If file does not exist or is already open for reading, the 
                 function  will fail and this will point to a null pointer.
  Return:       
   A status code:
      SSTG_OK success
      SSTG_ERROR_...  for error condition
  ==========================================================================*/
int
openStructuredStorageInPalmStream (
    FileHand in_file,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    SSRWIS* pIS =   NULL;
    SINT4   iRet =  SSTG_OK;

    ASSERT (in_pFile != NULL);
    ASSERT (STG_CANREAD(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    if ((in_file == 0) ||
        (out_ppRoot == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    if (! STG_CANREAD(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pIS = SsrwConnectToExternalPalmFileHand(in_file);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILE_NOT_FOUND;
    }

    iRet = openStructuredStorageInternal(pIS,
                                         out_ppRoot,
                                         SSRW_FALSE,
                                         in_accessMode);
    if (iRet != SSTG_OK)
    {
        SsrwDisconnect(pIS);
    }

    return iRet;
}

/*============================================================================
  Description:  (API call) Opens a structured storage from a Palm file handle
  Arguments: 
   in_file -     File reference.  MUST be open in read-only mode.
   in_access -   Access mode.  Must be STG_READ or STG_RW.
   out_ppRoot -  pointer to the pointer to the root storage (to be allocated in 
                 the function). 
                 If file does not exist or is already open for reading, the 
                 function  will fail and this will point to a null pointer.
  Return:       
   A status code:
      SSTG_OK success
      SSTG_ERROR_...  for error condition
  ==========================================================================*/
int
openStructuredStorageInPalmVFSStream (
    FileRef in_file,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    SSRWIS* pIS =  NULL;
    SINT4   iRet = SSTG_OK;

    ASSERT (in_pFile != NULL);
    ASSERT (STG_CANREAD(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    if ((in_file == 0) ||
        (out_ppRoot == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    if (! STG_CANREAD(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pIS = SsrwConnectToExternalPalmVFSFileRef(in_file);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILE_NOT_FOUND;
    }

    iRet = openStructuredStorageInternal(pIS,
                                         out_ppRoot,
                                         SSRW_FALSE,
                                         in_accessMode);
    if (iRet != SSTG_OK)
    {
        SsrwDisconnect(pIS);
    }

    return iRet;
}

#else /* !SSTG_PALMOS */

/*============================================================================
  Description:  (API call) Opens a structured storage from a disk file
                If file does not exist or is already open for reading, the
                function  will fail and this will point to a null pointer.
  Arguments:
   in_pFile -    File pointer.  MUST be open in read-only mode.
   in_access -   access mode.  Must be STG_READ or STG_RW.
   out_ppRoot -  pointer to the pointer to the root storage (to be allocated in
                 the function).
  Return:
  A status code:
  SSTG_OK success
  SSTG_ERROR_...  for error condition
==========================================================================*/
int
openStructuredStorageInStream (
    FILE* in_pFile,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    SSRWIS* pIS =   NULL;
    SINT4   iRet =  SSTG_OK;

    ASSERT (in_pFile != NULL);
    ASSERT (STG_CANREAD(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    if ((in_pFile == NULL) ||
            (out_ppRoot == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }
    if (! STG_CANREAD(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pIS = SsrwConnectToExternalFile(in_pFile);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILE_NOT_FOUND;
    }

    iRet = openStructuredStorageInternal(pIS,
                                         out_ppRoot,
                                         SSRW_FALSE,
                                         in_accessMode);
    if (iRet != SSTG_OK)
    {
        SsrwDisconnect(pIS);
    }

    return iRet;
}

#endif /* !SSTG_PALMOS */

/*============================================================================
  Description:  Opens a structured storage from a disk file 
  Arguments: 
   in_pIS -                 Pointer to input source from which to read
   out_ppRoot -             pointer to the pointer to the root storage (to be 
                            allocated in the function). If file does not exist 
                            or is already open for reading, the function  will 
                            fail and this will point to a null pointer.
   in_bStgInheritsFilePtr - specifies whether the root storage will own the
                            passed file pointer after this call.  On failure,
                            the caller always still owns the file pointer.
  Return:       
   A status code:
      SSTG_OK success
      SSTG_ERROR_...  for error condition
  ==========================================================================*/
static int
openStructuredStorageInternal (
    SSRWIS* in_pIS,
    RootStorage** out_ppRoot,
    boolean in_bStgInheritsFilePtr,
    StgMode in_accessMode)
{
    RootStorage*    pSstg =             NULL;
    DirectoryEntry* pRootEntry =        NULL;
    SINT4           iRet =              SSTG_OK;
    UINT4*          pulFatSectors =     NULL;
    UINT4*          pulXFatSectors =    NULL;
    UINT4           cSectorsRead =      0;
    UINT2           wSectorShift =      0;
    UINT4           cFatSectors =       0;
    UINT4           cSmallFatSectors =  0;
    UINT4           cXFatSectors =      0;

    ASSERT (in_pIS != NULL);
    ASSERT (out_ppRoot != NULL);

    /* allocate the storage */
    pSstg = (RootStorage*)malloc (sizeof (RootStorage));
    if (pSstg == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( pSstg, 0, sizeof(RootStorage));


    /* Initialize the root storage structure */
    iRet = rootStorageInit (pSstg, in_accessMode, 512);
    if (iRet != SSTG_OK)
    {
        closeStg (pSstg);
        return iRet;
    }

    /* store the file pointer*/
    pSstg->pIS = in_pIS;

    /* read the header */
    iRet = headerRead (pSstg->pIS, pSstg->pHeader);
    if (iRet != SSTG_OK)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return iRet;
    }

    wSectorShift = headerGetSectorShift(pSstg->pHeader);

    tocSetSectorSize(pSstg->pTOC, (1 << wSectorShift));

    /* create the sector buffer */
    pSstg->buffer = (CHAR*)malloc (1 << wSectorShift);
    if (pSstg->buffer == NULL)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return SSTG_ERROR_MEMORY;
    }

    memset(pSstg->buffer, 0, 1 << wSectorShift);


    /* initialize the FAT */
    iRet = fatConstruct (pSstg,
                         &pSstg->fat,
                         (UINT2)(1 << wSectorShift));
    if (iRet != SSTG_OK)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return iRet;
    }

    pulFatSectors = (UINT4*)
        malloc(headerGetFatSize(pSstg->pHeader) * sizeof(UINT4));
    if (pulFatSectors == NULL)
    {
        pSstg->pIS = NULL;
        closeStg(pSstg);
        return SSTG_ERROR_MEMORY;
    }
    memset( pulFatSectors, 0, headerGetFatSize(pSstg->pHeader) * sizeof(UINT4));

    cXFatSectors = headerGetXFatSize(pSstg->pHeader);
    if (cXFatSectors > 0)
    {
        pulXFatSectors = (UINT4*)malloc(cXFatSectors * sizeof(UINT4));
        if (pulXFatSectors == NULL)
        {
            free(pulFatSectors);
            pSstg->pIS = NULL;
            closeStg(pSstg);
            return SSTG_ERROR_MEMORY;
        }
    }

    iRet = headerReadFatSectors (pSstg->pIS,
                                 pSstg->pHeader,
                                 pulFatSectors,
                                 &cSectorsRead);
    if (iRet != SSTG_OK)
    {
        free(pulXFatSectors);
        free(pulFatSectors);
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return iRet;
    }
    cFatSectors = headerGetFatSize(pSstg->pHeader);
    ASSERT (cSectorsRead == min(cFatSectors, HEADER_FAT_SECTORS));

    if (cFatSectors > HEADER_FAT_SECTORS)
    {
        iRet = readFatSectorsFromXFat (pSstg, pulFatSectors, pulXFatSectors);
        if (iRet != SSTG_OK)
        {
            pSstg->pIS = NULL;
            free (pulXFatSectors);
            pulXFatSectors = NULL;
            free (pulFatSectors);
            pulFatSectors = NULL;
            closeStg (pSstg);
            return iRet;
        }
    }

    fatSetFatSectors (pSstg->fat, pulFatSectors, cFatSectors);
    pSstg->pulXFatSectors = pulXFatSectors;

    /* Read in the whole FAT */
    fatReadFat (pSstg->fat);

    /* initialize the smallFat */
    cSmallFatSectors = headerGetSmallFatSize(pSstg->pHeader);
    if (cSmallFatSectors > 0)
    {
        iRet = initializeSmallFat (pSstg);
    }
    else
    {
        iRet = fatConstruct (
                pSstg,
                &(pSstg->smallFat),
                (UINT2)(1 << headerGetSmallSectorShift(pSstg->pHeader)));
    }
    if (iRet != SSTG_OK)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return iRet;
    }

    /* initialize the directory chain */
    iRet = fatGetChain (pSstg->fat,
                        headerGetDirectoryStartSector(pSstg->pHeader),
                        &pSstg->directoryChainLen,
                        &pSstg->directoryChain );
    if (iRet != SSTG_OK)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return iRet;
    }

    /* Read the entire directory table into memory */
    iRet = tocRead (pSstg->pTOC, pSstg);
    if (iRet != SSTG_OK)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return iRet;
    }

    /* initialize the root directory */
    iRet = storageConstructRoot (pSstg, &pSstg->rootDir);
    if (iRet != SSTG_OK)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return iRet;
    }

    /* Figure out the index of the last sector used by the small stream */
    pRootEntry = tocGetEntryAtIndex(pSstg->pTOC, 0);
    if (pRootEntry == NULL)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return SSTG_ERROR_CHILD_NOT_FOUND;
    }
    iRet = fatGetChainLastSector(pSstg->fat,
                                 directoryGetStartSector(pRootEntry),
                                 &(pSstg->ulLastMiniStreamSector));
    if (iRet != SSTG_OK)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return iRet;
    }


    pSstg->bStgOwnsFilePtr = in_bStgInheritsFilePtr;
    *out_ppRoot = pSstg;


    return (int)iRet;
}


/*============================================================================
  Description: (API call) Closes the structured storage indicated by the 
               argument.  Any open file is closed and all memory is freed.
  Arguments: 
   io_ppRoot - pointer to pointer to root storage structure to close and free.
               At the end of this call (*io_ppRoot) will be NULL.
  Return: 
   Status code
  ==========================================================================*/
int
closeStructuredStorage (RootStorage** io_ppRoot )
{
    SINT4 iRet = SSTG_OK;

    ASSERT (io_ppRoot != NULL && (*io_ppRoot) != NULL);
    if ((io_ppRoot == NULL) || ((*io_ppRoot) == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

#ifdef DEBUG
    assertMiniStreamIntegrity(*io_ppRoot);
#endif /* DEBUG */

    if ((*io_ppRoot)->dirty != 0)
    {
        iRet = rootStorageFinalWrite(*io_ppRoot);
    }

    closeStg (*io_ppRoot);
    *io_ppRoot = NULL;

    return iRet;
}


/*============================================================================
  Description:  Reads the specified sector into the storage's sector buffer
                and locks the buffer.
  Arguments:    
   in_pRoot - 
  Return:       A pointer to the internal buffer.  If NULL, an error code is
                set in status.  Call rootStorageGetStatus
  ==========================================================================*/
CHAR*
rootStorageReadSector (RootStorage* in_pRoot, UINT4 in_ulSector)
{
    if (in_pRoot->bufferedSector != in_ulSector)
    {
        in_pRoot->statusCode = readSector (in_pRoot, in_ulSector);
        if (in_pRoot->statusCode != SSTG_OK)
        {
            return NULL;
        }
    }
    return in_pRoot->buffer;
}


/*============================================================================
  Description:  Writes a sector to the structured storage file.
  Arguments:    
   in_pRoot -    root storage structure representing file to write to
   in_buf -      data to write
   out_pSector - on success, will contain the index of the sector just written
  Return:       
   Status code, SSTG_OK on success
  ==========================================================================*/
SINT4
rootStorageWriteSector (RootStorage* in_pRoot, 
                        BYTE* in_buf, 
                        UINT4* out_pSector,
                        boolean in_isMetadata)
{
    ASSERT (in_buf != NULL && out_pSector != NULL);
    return writeSector(in_pRoot, in_buf, out_pSector, in_isMetadata);
}


/*============================================================================
  Description:  rewrites a sector in the structured storage file.
  Arguments:    
   in_pRoot -    root storage structure representing file to write to
   in_buf -      data to write
   in_ulSector - index of sector to overwrite

  Return:       
   Status code, SSTG_OK on success
  ==========================================================================*/
SINT4
rootStorageRewriteSector (RootStorage* in_pRoot, BYTE* in_buf, UINT4 in_ulSector)
{
    ASSERT (in_buf != NULL && in_pRoot != NULL);
    return rewriteSector(in_pRoot, in_buf, in_ulSector);
}


/*============================================================================
  Description: Reads a sector into a buffer supplied by the calling function
  Arguments:   
   in_pRoot -    pointer to RootStorage representing file from which to get the
                 sector
   in_ulSector - index of sector to get
   in_pvBuffer - pointer to buffer that will contain the data from the 
                 sector on success
  Return:      
   Status code -- hopefully SSTG_OK 
  ==========================================================================*/
SINT4
rootStorageGetSector (RootStorage* in_pRoot, UINT4 in_ulSector, void* in_pvBuffer)
{
    UINT8 ullSeek =         0;
    SINT4 iRet =            SSTG_OK;
    SINT4 rc =              0;
    UINT2 wSectorShift =    0;

    wSectorShift = headerGetSectorShift(in_pRoot->pHeader);
    /* Count the header as a sector in this calculation */
    ullSeek = ((UINT8)(in_ulSector + 1)) << ((UINT8)wSectorShift);
    SsrwFseek (in_pRoot->pIS, ullSeek, SEEK_SET);

    rc = SsrwFread(in_pvBuffer, (1 << wSectorShift), 1, in_pRoot->pIS);
    if (rc != 1)
    {
        iRet = SSTG_ERROR_FILEIO;
    }
    in_pRoot->statusCode = iRet;

    return iRet;
}


/*============================================================================
  Description:  Accesses the Header of the RootStorage
  Arguments:    
   in_pRoot - this
  Return:       
   Pointer to the header
  ==========================================================================*/
Header*
rootStorageGetHeader (RootStorage* in_pRoot)
{
    ASSERT (in_pRoot != NULL);
    ASSERT (in_pRoot->pHeader != NULL);
    return in_pRoot->pHeader;
}


/*============================================================================
  Description:  Accesses the DocProperties of the RootStorage
  Arguments:    
   in_pRoot - this
  Return:       
   Pointer to the header
  ==========================================================================*/
DocumentProperties* rootStorageGetDocProperties(RootStorage* in_pRoot)
{
    ASSERT (in_pRoot != NULL);
    ASSERT (in_pRoot->pProperties != NULL);
    return in_pRoot->pProperties;
}


/*============================================================================
  Description:  Retrieves the Fat structure representing the normal FAT of
                a given RootStorage
  Arguments:    
   in_pRoot - this
  Return:       
   Pointer to the Fat structure
  ==========================================================================*/
Fat*
rootStorageGetFat(RootStorage* in_pRoot)
{
    ASSERT (in_pRoot != NULL);
    ASSERT (in_pRoot->fat != NULL);

    return in_pRoot->fat;
}


/*============================================================================
  Description:  Retrieves the Fat structure representing the small FAT of
                a given RootStorage
  Arguments:    
   in_pRoot - this
  Return:       
   Pointer to the small Fat structure
  ==========================================================================*/
Fat*
rootStorageGetSmallFat(RootStorage* in_pRoot)
{
    ASSERT (in_pRoot != NULL);
    ASSERT (in_pRoot->smallFat != NULL);

    return in_pRoot->smallFat;
}


/*============================================================================
  Description:  Retrieves the TOC structure representing the table of contents
                of a file represented by a given RootStorage
  Arguments:    
   in_pRoot - this
  Return:       
   Pointer to the table of contents structure
  ==========================================================================*/
TOC*
rootStorageGetTOC(RootStorage* in_pRoot)
{
    ASSERT (in_pRoot != NULL);
    ASSERT (in_pRoot->pTOC != NULL);

    return in_pRoot->pTOC;
}


/*============================================================================
  Description:  Sets the last sector in the MiniStream
  Arguments:    
   io_pRoot - this
   in_ulSector - new last sector in the MiniStream
  ==========================================================================*/
void
rootStorageSetLastMiniStreamSector(RootStorage* io_pRoot, UINT4 in_ulSector)
{
    ASSERT (io_pRoot != NULL);

    io_pRoot->ulLastMiniStreamSector = in_ulSector;
}


/*============================================================================
  Description:  Retrieves the last sector in the MiniStream
  Arguments:    
   in_pRoot - this
  Return:       
   Last sector in the MiniStream
  ==========================================================================*/
UINT4
rootStorageGetLastMiniStreamSector(RootStorage* in_pRoot)
{
    ASSERT (in_pRoot != NULL);

    return in_pRoot->ulLastMiniStreamSector;
}


/*============================================================================
  Description:  Retrieves the access mode of the structured storage file
                represented by a given root storage
  Arguments:    
   in_pRoot - this
  Return:       
   Access mode (STG_READ, etc.)
  ==========================================================================*/
StgMode
rootStorageGetMode(RootStorage* in_pRoot)
{
    ASSERT (in_pRoot != NULL);
    return in_pRoot->mode;
}


/*============================================================================
  Description:  Returns the status code of the given structured storage object
  Arguments:    
   in_pRoot - this
  Return:       
   The given structured storage object's error status code
  ==========================================================================*/
SINT4
rootStorageGetStatus (RootStorage* in_pRoot)
{
    ASSERT (in_pRoot != NULL);
    return in_pRoot->statusCode;
}


/*============================================================================
  Description:  Retrieves the FAT chain for the table of contents sectors.
  Arguments:    
   in_pRoot - this
   out_ppulDirChain - will point to the directory chain
   out_pulDirChain - will contain the number of sectors in the chain
  ==========================================================================*/
void
rootStorageGetDirChain(RootStorage* in_pRoot,
                       UINT4** out_ppulDirChain,
                       UINT4* out_pulDirChainLen)
{
    ASSERT (in_pRoot != NULL);
    ASSERT (in_pRoot->directoryChain != NULL);
    ASSERT (in_pRoot->directoryChainLen > 0);
    *out_ppulDirChain = in_pRoot->directoryChain;
    *out_pulDirChainLen = in_pRoot->directoryChainLen;
}


/*============================================================================
 Description: (API call) accessor to get the root Storage object
  Arguments: 
   in_pRoot -      initialized RootStorage from which to get the Storage
   out_ppStorage - variable that, on success, will point to the root Storage
  Return:          
   Status code
  ==========================================================================*/
int
getStorageFromRoot(RootStorage* in_pRoot,
                   Storage** out_ppStorage)
{
    SINT4 iRet = SSTG_OK;

    ASSERT (in_pRoot != NULL);
    ASSERT (out_ppStorage != NULL);
    if (in_pRoot == NULL ||
            out_ppStorage == NULL)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (in_pRoot->rootDir == NULL)
    {
        return SSTG_ERROR_CHILD_NOT_FOUND;
    }

    *out_ppStorage = in_pRoot->rootDir;

    return iRet;
}

/*============================================================================
 Description: (API call) accessor to retrieve the sector size of a structured
              storage
  Arguments: 
   in_pRoot -        initialized RootStorage from which to get the sector size
   out_pSectorSize - variable that, on success, will point to the root Storage
  Return:          
   Status code
  ==========================================================================*/
int
structuredStorageGetSectorSize(const RootStorage* in_pRoot,
                               unsigned long* out_pSectorSize)
{
    SINT4 iRet = SSTG_OK;

    ASSERT (in_pRoot != NULL);
    ASSERT (out_pSectorSize != NULL);
    if ((in_pRoot == NULL) ||
        (out_pSectorSize == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    *out_pSectorSize = (1 << headerGetSectorShift(in_pRoot->pHeader));

    return iRet;
}

/**   S T A T I C     F U N C T I O N S                                 */

/*============================================================================
  Description:  Initializes a root storage
  Arguments: 
   in_pRoot -        storage to initialize
   in_mode -         write or read?
   in_ulSectorSize - number of bytes per sector
  Return: 
  ==========================================================================*/
static SINT4
rootStorageInit (RootStorage* in_pRoot, StgMode in_mode, UINT4 in_ulSectorSize)
{
    SINT4 iRet = SSTG_OK;

    ASSERT (in_pRoot != NULL);

    /* init file objects */
    in_pRoot->pIS = NULL;
    in_pRoot->bStgOwnsFilePtr = SSRW_FALSE;

    /* init header */
    iRet = headerInit(&in_pRoot->pHeader);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    headerSetSectorShift(in_pRoot->pHeader, 
                         (UINT2)SsrwUINTLog2(in_ulSectorSize));

    /* init table of content */
    iRet = rootStorageInitTOC(in_pRoot);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    tocSetSectorSize(in_pRoot->pTOC, in_ulSectorSize);

    /* init DocumentProperties manager */
    iRet = documentPropertiesConstruct(&(in_pRoot->pProperties));
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* init remaining fields */
    in_pRoot->mode = in_mode;
    in_pRoot->buffer = NULL;
    in_pRoot->directoryChain = NULL;
    in_pRoot->directoryChainLen = (UINT4)-1;
    in_pRoot->rootDir = NULL;
    in_pRoot->bufferedSector = 0xffffffff;
    in_pRoot->statusCode = SSTG_OK;
    in_pRoot->ulLastMiniStreamSector = (UINT4)-1;
    in_pRoot->fat = NULL;
    in_pRoot->smallFat = NULL;
    in_pRoot->pulXFatSectors = NULL;
    in_pRoot->dirty = (in_mode == STG_WRITE) ? 1 : 0;

    return iRet;
}


/*============================================================================
  Description: Inits TOC, i.e allocates one block and fills it with 0xff
  Arguments: 
   in_pRoot - a storage root to have SINT4ialized TOC
  Return: 
   Status code
  ==========================================================================*/
static SINT4
rootStorageInitTOC(RootStorage* in_pRoot)
{
    SINT4 cbSectorSize = 0;

    ASSERT (in_pRoot != NULL);

    cbSectorSize = (1 << headerGetSectorShift(in_pRoot->pHeader));
    return tocConstruct (cbSectorSize, &(in_pRoot->pTOC));
}


/*============================================================================
  Description: Cleans up a storage, i.e. frees all memory associated with that
               storage including the storage structure itself.
  Arguments:   
   io_pRoot - storage to clean up.
  Return: 
  ==========================================================================*/
static void
closeStg (RootStorage* io_pRoot)
{
    ASSERT (io_pRoot != NULL);

    if (io_pRoot->rootDir != NULL)
    {
        closeStorageInternal (&(io_pRoot->rootDir));
    }

    /* close the file */
    if (io_pRoot->pIS != NULL)
    {
        if (io_pRoot->bStgOwnsFilePtr)
        {
            SsrwFclose (io_pRoot->pIS);
        }
        else
        {
            SsrwDisconnect(io_pRoot->pIS);
        }
    }

    /* Free all member pointers that point to a valid structure */
    if (io_pRoot->buffer != NULL)
    {
        free (io_pRoot->buffer);
    }

    if (io_pRoot->smallFat != NULL)
    {
        fatDestroy (&(io_pRoot->smallFat));
    }

    if (io_pRoot->directoryChain != NULL)
    {
        free (io_pRoot->directoryChain);
    }

    if (io_pRoot->fat != NULL)
    {
        fatDestroy (&(io_pRoot->fat));
    }

    if (io_pRoot->pProperties != NULL)
    {
        documentPropertiesDestroy(&(io_pRoot->pProperties));
    }

    if (io_pRoot->pHeader != NULL)
    {
        free(io_pRoot->pHeader);
    }

    if (io_pRoot->pTOC != NULL)
    {
        tocDestroy(&(io_pRoot->pTOC));
    }

    if (io_pRoot->pulXFatSectors != NULL)
    {
        free(io_pRoot->pulXFatSectors);
        io_pRoot->pulXFatSectors = NULL;
    }

    free (io_pRoot);
}


/*============================================================================
  Description: Reads the fat sectors from the extended FAT
               (Also known as the double-indirect FAT or DIF)
  Arguments: 
   in_pRoot -           pointer to RootStorage representing file to read from
   out_pulSectors -     on success, the array pointed to by this pointer will
                        contain the FAT sectors in use
   out_pulXFatSectors - on success, the array pointed to by this pointer will
                        contain the XFAT/DIF sectors in use
  Return: 
   Status code
  ==========================================================================*/
static SINT4
readFatSectorsFromXFat(RootStorage* in_pRoot,
                       UINT4* out_pulSectors,
                       UINT4* out_pulXFatSectors)
{
    CHAR* pB =                          NULL;
    UINT4 cFATSectorsPerXFATSector =    0;
    UINT4 cSectorsRead =                0;
    UINT4 ulSectorToRead =              0;
    SINT4 iRet =                        SSTG_OK;
    UINT2 wSectorShift =                0;
    UINT4 ulXFatStartSector =           0;
    UINT4 ulXFatIndex =                 0;
    UINT4 i =                           0;

    wSectorShift = headerGetSectorShift(in_pRoot->pHeader);
    cFATSectorsPerXFATSector = (1 << wSectorShift) / sizeof (UINT4);

    ulXFatStartSector = headerGetXFatStartSector(in_pRoot->pHeader);
    iRet = readSector (in_pRoot, ulXFatStartSector);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    out_pulXFatSectors[ulXFatIndex] = ulXFatStartSector;
    ulXFatIndex++;

    cSectorsRead = HEADER_FAT_SECTORS;
    pB = in_pRoot->buffer;
    for (i = 0; cSectorsRead < headerGetFatSize(in_pRoot->pHeader); i++)
    {
        if (((i + 1) % cFATSectorsPerXFATSector) == 0)
        {
            ulSectorToRead = leReadUnsignedLong (pB);
            iRet = readSector (in_pRoot, ulSectorToRead);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
            pB = in_pRoot->buffer;
            out_pulXFatSectors[ulXFatIndex] = ulSectorToRead;
            ulXFatIndex++;
        }
        else
        {
            out_pulSectors[cSectorsRead] =
                leReadUnsignedLong(pB);
            cSectorsRead++;
            pB += sizeof(UINT4);
        }
    }

    return iRet;
}


/*============================================================================
  Description: Reads an entire sector from the file
  Arguments: 
   in_pRoot -    initialized root storage object representing the file to read
                 from
   in_ulSector - index of sector to read
  Return: 
   Status code
  ==========================================================================*/
static SINT4 readSector (RootStorage* in_pRoot,
                         UINT4 in_ulSector)
{
    UINT8   ullSeek =           0;
    SINT4   iRet =              SSTG_OK;
    size_t  cbToRead =          0;
    size_t  cbRead =            0;
    UINT2   wSectorShift =      0;

    wSectorShift = headerGetSectorShift(in_pRoot->pHeader);
    /* Count the header as a sector in this calculation */
    ullSeek = ((UINT8)(in_ulSector + 1)) << ((UINT8)wSectorShift);
    SsrwFseek(in_pRoot->pIS, ullSeek, SEEK_SET);

    /* Make sure we actually got to where we wanted to be */
    if ((UINT8)SsrwFtell(in_pRoot->pIS) != ullSeek)
    {
        return SSTG_ERROR_FILEIO;
    }

    cbToRead = (size_t)(1 << wSectorShift);
    cbRead = SsrwFread(in_pRoot->buffer, 1, cbToRead, in_pRoot->pIS);
    if (cbRead != cbToRead)
    {
        /* See Splat #1015.  It turns out that there are real, working structured
         * storage files whose length is not a multiple of the sector size.  
         * Worse still, there's data in the last partial sector.  So 
         * don't return an error unless no data at all is read, indicating that
         * we're past the end of the file or something like that. 
         * The garbage at the end of the buffer should be harmlessly ignored. */
        if (cbRead == 0)
        {
            iRet = SSTG_ERROR_FILEIO;
        }
        else
        {
            iRet = SSTG_OK;
        }
    }
    else
    {
        in_pRoot->bufferedSector = in_ulSector;
    }
    return iRet;
}


/*============================================================================
  Description:  Writes an entire sector to the structured storage file.
  Arguments:    
   in_pRoot -    initialized rootStorage 
   in_buf -      data to write
   out_pSector - sector used to write data
  Return:        
   Status code, STG_OK on success
  ==========================================================================*/
static SINT4
writeSector (RootStorage* io_pRoot, 
             BYTE* in_buf, 
             UINT4* out_pSector,
             boolean in_isMetadata)
{
    UINT8 ullSeek =         0;
    SINT4 iRet =            SSTG_OK;
    SINT4 rc =              0;
    UINT2 wSectorShift =    0;
    UINT4 ulSectorIndex =   0;

    ASSERT (STG_CANWRITE(io_pRoot->mode));
    ASSERT (io_pRoot != NULL);

    wSectorShift = headerGetSectorShift(io_pRoot->pHeader);

    /* Determine where to write this sector: in the middle of the file, or at
     * the end */
    iRet = fatNextFreeSector(io_pRoot->fat, &ulSectorIndex, in_isMetadata);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* If we are writing to the same sector as the one we have buffered 
     * for reading, invalidate the buffer */
    if (ulSectorIndex == io_pRoot->bufferedSector)
    {
        io_pRoot->bufferedSector = (UINT4)-1;
    }
    
    /* Count the header as a sector in this calculation */
    ullSeek = ((UINT8)(ulSectorIndex + 1)) << ((UINT8)wSectorShift);
    if (SsrwFseek(io_pRoot->pIS, ullSeek, SEEK_SET) != 0)
    {
        iRet = SSTG_ERROR_FILEIO;
    }
    else
    {
        rc = SsrwFwrite((void*)in_buf, (1 << wSectorShift), 1, io_pRoot->pIS);
        if (rc == 1)
        {
            *out_pSector = ulSectorIndex;
        }
        else
        {
            iRet = SSTG_ERROR_FILEIO;
        }
    }
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    if (ullSeek > 0x07fff0000)
    {
        iRet = headerEnsureV4(io_pRoot->pHeader);
    }

    return iRet;
}


/*============================================================================
  Description:  rewrites an entire sector in the structured storage file.
  Arguments:    
   in_pRoot -    initialized rootStorage 
   in_buf -      data to write
   in_ulSector - index of sector to overwrite

  Return:        
   Status code, STG_OK on success
  ==========================================================================*/
static SINT4
rewriteSector (RootStorage* in_pRoot, BYTE* in_buf, UINT4 in_ulSector )
{
    UINT8 ullSeek =         0;
    SINT4 iRet =            SSTG_OK;
    SINT4 rc =              0;
    UINT2 wSectorShift =    0;

    ASSERT (STG_CANWRITE(in_pRoot->mode));
    ASSERT (in_pRoot != NULL);

    /* If we are writing to the same sector as the one we have buffered 
     * for reading, invalidate the buffer */
    if (in_ulSector == in_pRoot->bufferedSector)
    {
        in_pRoot->bufferedSector = (UINT4)-1;
    }

    wSectorShift = headerGetSectorShift(in_pRoot->pHeader);
    /* Count the header as a sector in this calculation */
    ullSeek = ((UINT8)(in_ulSector + 1)) << ((UINT8)wSectorShift);
    SsrwFseek(in_pRoot->pIS, ullSeek, SEEK_SET);

    /* Make sure we actually got to where we wanted to be */
    if ((UINT8)SsrwFtell(in_pRoot->pIS) != ullSeek)
    {
        return SSTG_ERROR_FILEIO;
    }
    else
    {
        rc = SsrwFwrite((void*)in_buf, (1 << wSectorShift), 1, in_pRoot->pIS);
        if (rc == 1)
        {
            in_pRoot->bufferedSector = 0xffffffff;
        }
        else
        {
            iRet = SSTG_ERROR_FILEIO;
        }
    }
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    if (ullSeek > 0x07fff0000)
    {
        iRet = headerEnsureV4(in_pRoot->pHeader);
    }

    return iRet;
}


/*============================================================================
  Description:  Writes an entire "mini" sector to the structured storage file.
  Arguments:    
   in_pRoot -    initialized rootStorage 
   in_buf -      data to write
   out_pSector - index of mini sector used to write data
  Return:        
   Status code, STG_OK on success
  ==========================================================================*/
SINT4
rootStorageWriteMiniSector (RootStorage* io_pRoot, 
                            BYTE* in_buf, 
                            UINT4* out_pSector)
{
    UINT8           ullSeek =                     0;
    SINT4           iRet =                        SSTG_OK;
    SINT4           rc =                          0;
    UINT2           nRegSectorShift =             0;
    UINT4           ulRegSectorSize =             0;
    UINT2           nMiniSectorShift =            0;
    UINT4           ulMiniSectorSize =            0;
    UINT4           ulRegSector =                 0;
    UINT4           ulMiniSector =                0;
    UINT4           ulMiniStreamSectorIndex =     0;
    UINT4*          pulMiniFatSectors =           NULL;
    UINT4           ulMiniFatSectorCount =        0;
    UINT4           ulMiniSectorsPerRegSector =   0;
    UINT8           ullMiniStreamSize =           0;
    UINT8           ullRoundedUpMiniStreamSize =  0;
    DirectoryEntry* pRootEntry =                  NULL;

    ASSERT (STG_CANWRITE(io_pRoot->mode));
    ASSERT (io_pRoot != NULL);

    /* Precalculate some parameters */
    nRegSectorShift = headerGetSectorShift(io_pRoot->pHeader);
    ulRegSectorSize = (1 << nRegSectorShift);
    nMiniSectorShift = headerGetSmallSectorShift(io_pRoot->pHeader);
    ulMiniSectorSize = (1 << nMiniSectorShift);
    ulMiniSectorsPerRegSector = 1 << (nRegSectorShift - nMiniSectorShift);
    pRootEntry = tocGetEntryAtIndex(io_pRoot->pTOC, 0);
    ullMiniStreamSize = directoryGetStreamLength(pRootEntry);

    /* Determine where to write this sector */
    iRet = fatNextFreeSector(io_pRoot->smallFat, &ulMiniSector, SSRW_FALSE);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    ulMiniStreamSectorIndex = ulMiniSector / ulMiniSectorsPerRegSector;
    ullRoundedUpMiniStreamSize = (ullMiniStreamSize + 
                                  (ulRegSectorSize - 1)) / 
                                  ulRegSectorSize * 
                                  ulRegSectorSize;
    if (((UINT8)ulMiniSector * (1 << (UINT8)nMiniSectorShift)) >=
        ullRoundedUpMiniStreamSize)
    {
        /* We must link another regular sector onto the end of the mini 
         * stream. */
        iRet = fatNextFreeSector(io_pRoot->fat, &ulRegSector, SSRW_FALSE);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }

        if (io_pRoot->ulLastMiniStreamSector != (UINT4)-1)
        {
            iRet = fatUpdate(io_pRoot->fat, 
                             io_pRoot->ulLastMiniStreamSector, 
                             ulRegSector);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
        else
        {
            iRet = directorySetStartSector(pRootEntry, ulRegSector);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
            iRet = fatTerminateChain(io_pRoot->fat, ulRegSector);
            if (iRet != SSTG_OK)
            {
                return iRet;
            }
        }
        io_pRoot->ulLastMiniStreamSector = ulRegSector;
        ullMiniStreamSize = (UINT8)(ulMiniStreamSectorIndex) * 
                            (UINT8)ulRegSectorSize +
                            ulMiniSectorSize;
        iRet = directorySetSize(pRootEntry, ullMiniStreamSize);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }

        /* Don't need to add remaining mini sectors in reg. sector to the
         * free list.  They are "implied" free because they are past the
         * end of the mini FAT. */
    }
    else if (ulMiniSector == fatGetLength(io_pRoot->smallFat))
    {
        /* We are still writing to the last regular sector chained into the
         * mini stream. */
        ulRegSector = io_pRoot->ulLastMiniStreamSector;

        ullMiniStreamSize = (UINT8)(ulMiniStreamSectorIndex) * 
                            (UINT8)ulRegSectorSize +
                            ((UINT8)((ulMiniSector % 
                                      ulMiniSectorsPerRegSector) + 1) *
                             (UINT8)ulMiniSectorSize);
        iRet = directorySetSize(pRootEntry, ullMiniStreamSize);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }
    else
    {
        /* We must get the FAT chain for the mini stream, figure out exactly
         * which regular sector to use, and then free the FAT chain again */
        ulMiniFatSectorCount = (UINT4)-1;
        iRet = fatGetChain(io_pRoot->fat,
                           directoryGetStartSector(pRootEntry),
                           &ulMiniFatSectorCount,
                           &pulMiniFatSectors);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        ulRegSector = pulMiniFatSectors[ulMiniStreamSectorIndex];
        free(pulMiniFatSectors);
    }

    /* Count the header as a sector in this calculation */
    ullSeek = ((UINT8)(ulRegSector + 1)) << ((UINT8)nRegSectorShift);
    ullSeek += (ulMiniSector % ulMiniSectorsPerRegSector) * 
               ulMiniSectorSize;

    /* Seek and write */
    if (SsrwFseek(io_pRoot->pIS, ullSeek, SEEK_SET) != 0)
    {
        iRet = SSTG_ERROR_FILEIO;
    }
    else
    {
        rc = SsrwFwrite((void*)in_buf, ulMiniSectorSize, 1, io_pRoot->pIS);
        if (rc == 1)
        {
            *out_pSector = ulMiniSector;
        }
        else
        {
            iRet = SSTG_ERROR_FILEIO;
        }
    }
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* If the file size is approaching the limit for V.3 structured storage files,
     * upgrade the version number */
    if (ullSeek > 0x07fff0000)
    {
        iRet = headerEnsureV4(io_pRoot->pHeader);
    }

    return iRet;
}


/*============================================================================
  Description: Initializes the small FAT
  Arguments: 
   in_pRoot - root storage for which to initialize the small FAT
  Return: 
   Status code
  ==========================================================================*/
static SINT4
initializeSmallFat (RootStorage* in_pRoot)
{
    Header* pHdr =              in_pRoot->pHeader;
    UINT4*  smallFatChain =     NULL;
    SINT4   iRet =              SSTG_OK;
    UINT2   wSmallSectorShift = 0;
    UINT4   cSmallFatSectors =  0;

    ASSERT(in_pRoot != NULL);

    wSmallSectorShift = headerGetSmallSectorShift(pHdr);
    iRet = fatConstruct (in_pRoot,
                         &in_pRoot->smallFat,
                         (UINT2)(1 << wSmallSectorShift));

    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    cSmallFatSectors = headerGetSmallFatSize(pHdr);
    iRet = fatGetChain (in_pRoot->fat,
                        headerGetSmallFatStartSector(pHdr),
                        &cSmallFatSectors,
                        &smallFatChain);

    if (iRet == SSTG_OK)
    {
        headerSetSmallFatSize(pHdr, cSmallFatSectors);
        fatSetFatSectors (in_pRoot->smallFat, smallFatChain, cSmallFatSectors);
        iRet = fatReadFat (in_pRoot->smallFat);
    }

    if (iRet != SSTG_OK)
    {
        fatDestroy (&(in_pRoot->smallFat));
        in_pRoot->smallFat = NULL;
    }

    return iRet;
}


#ifdef SSTG_PALMOS

/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another, on a PalmOS VFS expansion card.
  Arguments: 
   in_fileName -   the name of compound file to be ceated
   in_accessMode - Must be STG_WRITE or STG_RW.
   out_ppRoot -    on success, will be filled with a pointer to a new 
                   RootStorage structure representing a new structured storage
                   file.
   in_volRef -     PalmOS VFS volume reference for the expansion card the new 
                   structured storage will reside upon.
  Return: 
   Status code
  ==========================================================================*/
static int createStructuredStorageInVFS (
    const char* in_fileName,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned short in_volRef)
{
    return createStructuredStorageInVFSEx(in_fileName,
                                          in_accessMode,
                                          out_ppRoot,
                                          in_volRef,
                                          0);
}

/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another, on a PalmOS VFS expansion card.
  Arguments: 
   in_fileName -     the name of compound file to be ceated
   in_accessMode -   Must be STG_WRITE or STG_RW.
   out_ppRoot -      on success, will be filled with a pointer to a new 
                     RootStorage structure representing a new structured 
                     storage file.
   in_volRef -       PalmOS VFS volume reference for the expansion card the new 
                     structured storage will reside upon.
   in_ulSectorSize - size, in bytes, of sectors in the new file.  Passing 0 
                     will cause a default sector size to be used.  The sector 
                     size must be a power of 2 if it is not 0.
  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorageInVFSEx (
    const char* in_fileName,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned short in_volRef,
    unsigned long in_ulSectorSize)
{
    SSRWIS* pIS =   NULL;
    int     iRet =  SSTG_OK;

    ASSERT (in_fileName != NULL && *in_fileName != 0);
    ASSERT (STG_CANWRITE(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    ASSERT (((in_ulSectorSize - 1) & in_ulSectorSize) == 0);

    if ((in_fileName == NULL) ||
        (*in_fileName == 0) ||
        (out_ppRoot == NULL) ||
        (((in_ulSectorSize - 1) & in_ulSectorSize) != 0))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (!STG_CANWRITE(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }
    pIS = SsrwOpenVFSFile (in_fileName, "w+b", in_volRef);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILEIO;
    }
    iRet = createStructuredStorageInternal(pIS,
                                           in_accessMode,
                                           out_ppRoot,
                                           SSRW_TRUE,
                                           (UINT4)in_ulSectorSize);
    if (iRet != SSTG_OK)
    {
        SsrwFclose(pIS);
    }

    return iRet;
}

#endif /* SSTG_PALMOS */

/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_pFilename -  the name of compound file to be ceated
   in_accessMode - Must be STG_WRITE or STG_RW.
   out_ppRoot -    on success, will be filled with a pointer to a new 
                   RootStorage structure representing a new structured storage
                   file.

  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorage (
    const char* in_pFilename,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    SINT4    iRet =          SSTG_OK;
    wchar_t* pwFilename =    NULL;
    UINT4    ulFilenameLen = 0;

    ulFilenameLen = strlen(in_pFilename);
    pwFilename = (wchar_t*)malloc((ulFilenameLen + 1) * sizeof(wchar_t));
    if (pwFilename == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }
    mbstowcs(pwFilename, in_pFilename, ulFilenameLen + 1);

    iRet = createStructuredStorageEx(pwFilename, in_accessMode, out_ppRoot, 0);

    free(pwFilename);
    
    return iRet;
}

/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_pwFilename -   the name of compound file to be ceated
   in_accessMode -   Must be STG_WRITE or STG_RW.
   out_ppRoot -      on success, will be filled with a pointer to a new 
                     RootStorage structure representing a new structured 
                     storage file.
   in_ulSectorSize - size, in bytes, of sectors in the new file.  Passing 0 
                     will cause a default sector size to be used.  The sector 
                     size must be a power of 2 if it is not 0.

  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorageEx (
    const wchar_t* in_pwFilename,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned long in_ulSectorSize)
{
    SSRWIS* pIS =   NULL;
    int     iRet =  SSTG_OK;

    ASSERT (in_pwFilename != NULL && *in_pwFilename != 0);
    ASSERT (STG_CANWRITE(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    ASSERT (((in_ulSectorSize - 1) & in_ulSectorSize) == 0);
    if ((in_pwFilename == NULL) ||
        (*in_pwFilename == 0) ||
        (out_ppRoot == NULL) ||
        (((in_ulSectorSize - 1) & in_ulSectorSize) != 0))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (!STG_CANWRITE(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }
    pIS = SsrwOpenFile (in_pwFilename, L"w+b");
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILEIO;
    }
    iRet = createStructuredStorageInternal(pIS,
                                           in_accessMode,
                                           out_ppRoot,
                                           SSRW_TRUE,
                                           (UINT4)in_ulSectorSize);
    if (iRet != SSTG_OK)
    {
        SsrwFclose(pIS);
    }

    return iRet;
}

/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_fileName -   the name of compound file to be ceated
   in_accessMode - Must be STG_WRITE or STG_RW.
   out_ppRoot -    on success, will be filled with a pointer to a new 
                   RootStorage structure representing a new structured storage
                   file.
  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorageInBuf (
    void** in_ppBuf,
    unsigned long in_ulBufSize,
    SsrwReallocFunc in_pRealloc,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    return createStructuredStorageInBufEx(in_ppBuf,
                                          in_ulBufSize,
                                          in_pRealloc,
                                          in_accessMode,
                                          out_ppRoot,
                                          0);
}

/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_fileName -     the name of compound file to be ceated
   in_accessMode -   access mode. For now, it is always STG_WRITE (write only)
   out_ppRoot -      on success, will be filled with a pointer to a new 
                     RootStorage structure representing a new structured 
                     storage file.
   in_ulSectorSize - size, in bytes, of sectors in the new file.  Passing 0 
                     will cause a default sector size to be used.  The sector 
                     size must be a power of 2 if it is not 0.
  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorageInBufEx (
    void** in_ppBuf,
    unsigned long in_ulBufSize,
    SsrwReallocFunc in_pRealloc,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned long in_ulSectorSize)
{
    SSRWIS* pIS =   NULL;
    int     iRet =  SSTG_OK;

    ASSERT (in_ppBuf != NULL);
    ASSERT (*in_ppBuf != NULL);
    ASSERT (in_pRealloc != NULL);
    ASSERT (STG_CANWRITE(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    ASSERT (((in_ulSectorSize - 1) & in_ulSectorSize) == 0);
    if ((in_ppBuf == NULL) ||
        (*in_ppBuf == NULL) ||
        (in_pRealloc == 0) ||
        (out_ppRoot == NULL) ||
        (((in_ulSectorSize - 1) & in_ulSectorSize) != 0))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (!STG_CANWRITE(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }
    pIS = SsrwOpenMemForReadWrite (in_ppBuf, in_ulBufSize, in_pRealloc);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILEIO;
    }
    iRet = createStructuredStorageInternal(pIS,
                                           in_accessMode,
                                           out_ppRoot,
                                           SSRW_TRUE,
                                           (UINT4)in_ulSectorSize);
    if (iRet != SSTG_OK)
    {
        SsrwFclose(pIS);
    }

    return iRet;
}


#ifdef SSTG_PALMOS

/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_file -       File handle to which the structured storage library will 
                   connect.
   out_ppRoot -    on success, will be filled with a pointer to a new 
                   RootStorage structure representing a new structured storage
                   file.
  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorageInPalmStream (
    FileHand in_file,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    return createStructuredStorage(in_file,
                                   in_accessMode,
                                   out_ppRoot);
}

/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_file -         File handle to which the structured storage library will 
                     connect.
   out_ppRoot -      on success, will be filled with a pointer to a new 
                     RootStorage structure representing a new structured 
                     storage file.
   in_ulSectorSize - size, in bytes, of sectors in the new file.  Passing 0 
                     will cause a default sector size to be used.  The sector 
                     size must be a power of 2 if it is not 0.
  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorageInPalmStreamEx (
    FileHand in_file,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned long in_ulSectorSize)
{
    SSRWIS* pIS =   NULL;
    SINT4   iRet =  SSTG_OK;

    ASSERT (in_file != 0);
    ASSERT (STG_CANWRITE(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    ASSERT (((in_ulSectorSize - 1) & in_ulSectorSize) == 0);
    if ((in_file == 0) ||
        (out_ppRoot == NULL) ||
        (((in_ulSectorSize - 1) & in_ulSectorSize) != 0))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (!STG_CANWRITE(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pIS = SsrwConnectToExternalPalmFileHand(in_file);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILEIO;
    }

    iRet = createStructuredStorageInternal(pIS,
                                           in_accessMode,
                                           out_ppRoot,
                                           SSRW_FALSE,
                                           (UINT4)in_ulSectorSize);
    if (iRet != SSTG_OK)
    {
        SsrwDisconnect(pIS);
    }

    return iRet;
}


/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_file -         File handle to which the structured storage library will 
                     connect.
   out_ppRoot -      on success, will be filled with a pointer to a new 
                     RootStorage structure representing a new structured 
                     storage file.
   in_ulSectorSize - size, in bytes, of sectors in the new file.  Passing 0 
                     will cause a default sector size to be used.  The sector 
                     size must be a power of 2 if it is not 0.
  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorageInPalmVFSStream (
    FileRef in_file,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    return createStructuredStorage(in_file,
                                   in_accessMode,
                                   out_ppRoot,
                                   0);
}

/*============================================================================
  Description: (API Call) Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_file -         File handle to which the structured storage library will 
                     connect.
   out_ppRoot -      on success, will be filled with a pointer to a new 
                     RootStorage structure representing a new structured 
                     storage file.
   in_ulSectorSize - size, in bytes, of sectors in the new file.  Passing 0 
                     will cause a default sector size to be used.  The sector 
                     size must be a power of 2 if it is not 0.
  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorageInPalmVFSStreamEx (
    FileRef in_file,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned long in_ulSectorSize)
{
    SSRWIS* pIS =   NULL;
    SINT4   iRet =  SSTG_OK;

    ASSERT (in_file != 0);
    ASSERT (STG_CANWRITE(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    ASSERT (((in_ulSectorSize - 1) & in_ulSectorSize) == 0);
    if ((in_file == 0) ||
        (out_ppRoot == NULL) ||
        (((in_ulSectorSize - 1) & in_ulSectorSize) != 0))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (!STG_CANWRITE(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pIS = SsrwConnectToExternalPalmVFSFileRef(in_file);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILEIO;
    }

    iRet = createStructuredStorageInternal(pIS,
                                           in_accessMode,
                                           out_ppRoot,
                                           SSRW_FALSE,
                                           (UINT4)in_ulSectorSize);
    if (iRet != SSTG_OK)
    {
        SsrwDisconnect(pIS);
    }

    return iRet;
}

#else  /* !SSTG_PALMOS */

/*============================================================================
 Description: (API Call) Creates new structured storage and compound file and
   attaches them one to another. Note that the memory for the new
   root is allocated inside the function
 Arguments:
   in_pFile -        File to which the structured storage library will connect
   out_ppRoot -      on success, will be filled with a pointer to a new
                     RootStorage structure representing a new structured 
                     storage file.
   in_ulSectorSize - size, in bytes, of sectors in the new file.  Passing 0 
                     will cause a default sector size to be used.  The sector 
                     size must be a power of 2 if it is not 0.
 Return:
   Status code
==========================================================================*/
int createStructuredStorageInStream (
    FILE* in_pFile,
    StgMode in_accessMode,
    RootStorage** out_ppRoot)
{
    return createStructuredStorageInStreamEx(in_pFile,
            in_accessMode,
            out_ppRoot,
            0);
}

/*============================================================================
 Description: (API Call) Creates new structured storage and compound file and
   attaches them one to another. Note that the memory for the new
   root is allocated inside the function
 Arguments:
   in_pFile -        File to which the structured storage library will connect
   out_ppRoot -      on success, will be filled with a pointer to a new
                     RootStorage structure representing a new structured 
                     storage file.
   in_ulSectorSize - size, in bytes, of sectors in the new file.  Passing 0 
                     will cause a default sector size to be used.  The sector 
                     size must be a power of 2 if it is not 0.
 Return:
   Status code
==========================================================================*/
int createStructuredStorageInStreamEx (
    FILE* in_pFile,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned long in_ulSectorSize)
{
    SSRWIS* pIS =   NULL;
    SINT4   iRet =  SSTG_OK;

    ASSERT (in_pFile != NULL);
    ASSERT (STG_CANWRITE(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    ASSERT (((in_ulSectorSize - 1) & in_ulSectorSize) == 0);
    if ((in_pFile == NULL) ||
        (out_ppRoot == NULL) ||
        (((in_ulSectorSize - 1) & in_ulSectorSize) != 0))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (!STG_CANWRITE(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    pIS = SsrwConnectToExternalFile(in_pFile);
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILEIO;
    }

    iRet = createStructuredStorageInternal(pIS,
                                           in_accessMode,
                                           out_ppRoot,
                                           SSRW_FALSE,
                                           (UINT4)in_ulSectorSize);
    if (iRet != SSTG_OK)
    {
        SsrwDisconnect(pIS);
    }

    return iRet;
}

#endif /* SSTG_PALMOS */


/*============================================================================
  Description: Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_fileName -            the name of compound file to be ceated
   in_accessMode -          access mode. For now, it is always STG_WRITE 
                            (write only)
   out_ppRoot -             on success, will be filled with a pointer to a new 
                            RootStorage structure representing a new structured 
                            storage file.
   in_ulSectorSize -        size, in bytes, of sectors in the new file.  
                            Passing 0 will cause a default sector size to be 
                            used.  The sector size must be a power of 2 if it 
                            is not 0.
  Return: 
   Status code
  ==========================================================================*/
int createStructuredStorageInSource (
    SSRWIS* in_pSSRWIS,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    UINT4 in_ulSectorSize)
{
    SINT4   iRet =  SSTG_OK;

    ASSERT (in_pSSRWIS != NULL);
    ASSERT (in_pSSRWIS->m_pFread != NULL);
    ASSERT (in_pSSRWIS->m_pFwrite != NULL);
    ASSERT (in_pSSRWIS->m_pFclose != NULL);
    ASSERT (in_pSSRWIS->m_pFseek != NULL);
    ASSERT (in_pSSRWIS->m_pFtell != NULL);
    ASSERT (in_pSSRWIS->m_pFtruncate != NULL);
    ASSERT (STG_CANWRITE(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    ASSERT (((in_ulSectorSize - 1) & in_ulSectorSize) == 0);
    if ((in_pSSRWIS == NULL) ||
        (out_ppRoot == NULL) ||
        (((in_ulSectorSize - 1) & in_ulSectorSize) != 0) ||
        (in_pSSRWIS->m_pFread == NULL) ||
        (in_pSSRWIS->m_pFwrite == NULL) ||
        (in_pSSRWIS->m_pFclose == NULL) ||
        (in_pSSRWIS->m_pFseek == NULL) ||
        (in_pSSRWIS->m_pFtell == NULL) ||
        (in_pSSRWIS->m_pFtruncate == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (!STG_CANWRITE(in_accessMode))
    {
        return SSTG_ERROR_ILLEGAL_ACCESS;
    }

    iRet = createStructuredStorageInternal(in_pSSRWIS,
                                           in_accessMode,
                                           out_ppRoot,
                                           SSRW_TRUE,
                                           (UINT4)in_ulSectorSize);
    return iRet;
}


/*============================================================================
  Description: Creates new structured storage and compound file and 
               attaches them one to another. Note that the memory for the new 
               root is allocated inside the function
  Arguments: 
   in_fileName -            the name of compound file to be ceated
   in_accessMode -          access mode. For now, it is always STG_WRITE 
                            (write only)
   out_ppRoot -             on success, will be filled with a pointer to a new 
                            RootStorage structure representing a new structured 
                            storage file.
   in_bStgInheritsFilePtr - specifies whether the root storage will own the
                            passed file pointer after this call.  On failure,
                            the caller always still owns the file pointer.
   in_ulSectorSize -        size, in bytes, of sectors in the new file.  
                            Passing 0 will cause a default sector size to be 
                            used.  The sector size must be a power of 2 if it 
                            is not 0.
  Return: 
   Status code
  ==========================================================================*/
static int createStructuredStorageInternal (
    SSRWIS* in_pIS,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    boolean in_bStgInheritsFilePtr,
    UINT4 in_ulSectorSize)
{
    RootStorage* pSstg =        NULL;
    SINT4        iRet =         SSTG_OK;
    UINT2        wSectorShift = 0;

    ASSERT (in_pIS != NULL);
    ASSERT (STG_CANWRITE(in_accessMode));
    ASSERT (out_ppRoot != NULL);
    ASSERT (((in_ulSectorSize - 1) & in_ulSectorSize) == 0);

    /* allocate and initialize the storage */
    pSstg = (RootStorage*)malloc (sizeof (RootStorage));
    if (pSstg == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(pSstg, 0, sizeof(RootStorage));

    /* Calculate the appropriate sector size */
    if (in_ulSectorSize == 0)
    {
        in_ulSectorSize = 512;
    }

    rootStorageInit (pSstg, STG_WRITE, in_ulSectorSize);

    /* create the sector buffer */
    wSectorShift = headerGetSectorShift(pSstg->pHeader);
    pSstg->buffer = (CHAR*)malloc (1 << wSectorShift);
    if (pSstg->buffer == NULL)
    {
        closeStg (pSstg);
        return SSTG_ERROR_MEMORY;
    }
    memset(pSstg->buffer, 0, 1 << wSectorShift);

    /* Initialize the FAT */
    iRet = fatConstruct (pSstg,
                         &pSstg->fat,
                         (UINT2)(1 << headerGetSectorShift(pSstg->pHeader)));
    if (iRet != SSTG_OK)
    {
        closeStg (pSstg);
        return iRet;
    }

    /* Store the file pointer in the root storage struct */
    pSstg->pIS = in_pIS;
    pSstg->mode = in_accessMode;

    /* initialize the smallFat */
    iRet = fatConstruct (pSstg,
                         &(pSstg->smallFat),
                         (UINT2)(1 << headerGetSmallSectorShift(pSstg->pHeader)));
    if (iRet != SSTG_OK)
    {
        pSstg->pIS = NULL;
        closeStg (pSstg);
        return iRet;
    }

    /* create rootDir -a root Storage object */
    storageCreateAtRoot(pSstg, &(pSstg->rootDir));

    /* do initial writing and test it */
    iRet = rootStorageInitWrite(pSstg);
    if (iRet != SSTG_OK)
    {
        pSstg->pIS = NULL;
        closeStg(pSstg);
        return iRet;
    }

    pSstg->bStgOwnsFilePtr = in_bStgInheritsFilePtr;
    *out_ppRoot = pSstg;

    return (int)iRet;
}


/*============================================================================
  Description: Writes down initial fixed length portions of the file:
               the header and the header FAT sectors.
  Arguments: 
   in_pRoot -     initialized rootStorage to be written down
  Return: 
   Status code
  ==========================================================================*/
static SINT4
rootStorageInitWrite(RootStorage* in_pRoot)
{
    SINT4   iRet =      SSTG_OK;
    BYTE    dummy[4] =  {0xff,0xff,0xff, 0xff};
    SINT4   i =         0;
    SINT4   rc =        0;

    /* write down header */
    iRet = headerWrite(in_pRoot->pIS, in_pRoot->pHeader);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    rc = SsrwFseek(in_pRoot->pIS, HEADER_SIZE, SEEK_SET);
    if (rc != 0)
    {
        return SSTG_ERROR_FILEIO;
    }

    /* write down initial (empty) FAT sectors */
    for (i = 0; i < HEADER_FAT_SECTORS; i++)
    {
        rc = SsrwFwrite(dummy, sizeof(dummy), 1, in_pRoot->pIS);
        if (rc != 1)
        {
            return SSTG_ERROR_FILEIO;
        }
    }

    return iRet;
}


/*============================================================================
  Description: Helper for closeStructuredStorage function in STG_WRITE
               mode.  Does "final writing" of the file": directory, fat, 
               minifat, XFAT
  Arguments: 
   in_pRoot - root storage structure for which to do the final write
  Return: 
   Status code
  ==========================================================================*/
static SINT4
rootStorageFinalWrite(RootStorage* in_pRoot)
{
    SINT4         iRet =          SSTG_OK;
    UINT4*        pFatSectors =   NULL;
    UINT4         cFatSectors =   0;
    SINT8         llFinalSize =   0;
    unsigned long ulSectorSize =  0;
    UINT4         ulNewFatSize =  0;


    ASSERT(in_pRoot->fat != NULL);

    /* Write down Document properties if any */
    iRet = documentPropertiesWrite(in_pRoot->pProperties);

    /* make sure everything is closed */
    if (in_pRoot->rootDir != NULL)
    {
        iRet = closeStorageInternal (&(in_pRoot->rootDir));
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

#ifdef DEBUG
    /* Check for sectors that are not used by any streams and are not in any 
     * free sector list.  These "leaked" sectors will unnecessarily increase the 
     * size of a structured storage file. */
    iRet = assertNoLeakedSectors(in_pRoot);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
#endif /* DEBUG */

    /* write directory here */
    iRet = tocWrite(in_pRoot->pTOC,in_pRoot);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* Update the "next free sector" counter to use whatever remaining
     * contiguous stretch of free space is available at the end of the file */
    iRet = fatTrimFreeSectorsAtEnd(in_pRoot->fat, &ulNewFatSize);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* At this point, the FAT may have a chain of sectors that represents a 
     * sorted list of free sectors.  Unlink that chain and mark each sector 
     * with the FREE_SECTOR constant, as per the structured storage file 
     * format. */
    iRet = fatMarkFreeChain(in_pRoot->fat);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* write MiniFAT if necessary */
    iRet = fatWriteMiniFat(in_pRoot->smallFat);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* write FAT */
    iRet = fatWriteFatAndXFat(in_pRoot->fat);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* finally, write header and the DIF entries that go in the header "sector" */
    iRet = headerWrite(in_pRoot->pIS, in_pRoot->pHeader);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = fatGetFatSectors(in_pRoot->fat, &pFatSectors, &cFatSectors);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = headerWriteFatSectors(in_pRoot->pIS,
                                 pFatSectors,
                                 cFatSectors);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    iRet = headerWriteSectorFiller(in_pRoot->pHeader, in_pRoot->pIS);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* If there are unused sectors at the end of the file, then the file should
     * be truncated.  Some operating systems do not support a file truncation 
     * operation, so any errors are ignored here. */
    iRet = structuredStorageGetSectorSize(in_pRoot, &ulSectorSize);
    llFinalSize = (fatGetLength(in_pRoot->fat) + 1) * 
                  ((SINT8)ulSectorSize);
    SsrwFtruncate(in_pRoot->pIS, llFinalSize);

    return iRet;
}


/*============================================================================
  Description: Tells structured storage that the FAT, XFAT, MiniFAT, TOC will
               all have to be re-written because something in the structured
               storage file has changed.
  Arguments: 
   in_pRoot - root storage structure for which to set the dirty flag
  Return: 
   Status code
  ==========================================================================*/
SINT4 
rootStorageSetDirty(RootStorage* io_pRoot)
{
    SINT4 iRet =    SSTG_OK;
    UINT4 count =   0;

    ASSERT(io_pRoot != NULL);
    ASSERT(STG_CANWRITE(io_pRoot->mode));

    if (io_pRoot->dirty == 0)
    {
        ASSERT(io_pRoot->mode != STG_WRITE);

        /* We're in read/write mode and a change was made.  Now we need to do a 
         * few things:
         *     (a) Mark all space in the FAT that was devoted to the TOC, FAT,
         *         DIF/XFAT, MiniFAT as being free
         *     (b) Add those sectors to the free list
         *
         * Start with the XFAT/DIF.
         */
        count = headerGetXFatSize(io_pRoot->pHeader);
        iRet = fatAddFreeSectors(io_pRoot->fat,
                                 &(io_pRoot->pulXFatSectors), 
                                 &count);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
        
        /* Mark FAT sectors free */
        iRet = fatAddFatToFreeSectors(io_pRoot->fat, io_pRoot->fat);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }

        /* Mark small FAT sectors free, if the small FAT exists */
        if (io_pRoot->smallFat != NULL)
        {
            iRet = fatAddFatToFreeSectors(io_pRoot->fat,
                                          io_pRoot->smallFat);
        }

        /* Mark directory sectors free */
        iRet = fatAddFreeSectors(io_pRoot->fat,
                                 &(io_pRoot->directoryChain),
                                 &(io_pRoot->directoryChainLen));
        if (iRet != SSTG_OK)
        {
            return iRet;
        }

        io_pRoot->dirty = 1;
    }

    return iRet;
}

#ifdef DEBUG

SINT4
assertNoLeakedSectors(RootStorage* in_pRoot)
{
    return assertNoUntrackedFatSectors(in_pRoot->fat, in_pRoot->smallFat, in_pRoot->pTOC);
}

#endif /* DEBUG */

