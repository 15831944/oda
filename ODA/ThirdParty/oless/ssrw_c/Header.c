/************************************************************************
* Header.c
* Implements compound file Header reading, writing and access functions
*
* (c) Schema Software Inc., 2001-2004
*
*************************************************************************

$Revision: 1.13.2.2 $
$Date: 2004/02/24 19:09:25 $
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "StructuredStorage.h"   /* for error codes, */
#include "RootStorage.h"
#include "LEReadWrite.h"      /* for byte swapping functions */
#include "Header.h"

/*
 * Structured storage header is always at the beginning of the file. 
 */
struct _Header
{
    BYTE signature [8];      /* structured storage signature */
    BYTE reserved1 [16];
    UINT2 wMinVersion;       /* versions of the DLL that created this file*/
    UINT2 wMajVersion;
    UINT2 wByteOrder;        /* byte order on the machine tha created file
                              * should always be as defined by SS_BYTE_ORDER */
    UINT2 wSectorShift;      /* log(2, sectorSize) */
    UINT2 wSmallSectorShift; /* log(2, smallSectorSize) */
    BYTE reserved2 [6];
    UINT4 ulNumDirSectors;   /* For 4096-byte sector files, gives the number of
                              * sectors devoted to the directory.
                              * For 512-byte files, must be 0. */
    UINT4 ulFatSectors;      /* no of FAT sectors in the file */
    UINT4 ulDirectorySector; /* Start sector for TOC */
    UINT4 ulCommit;          /* number of Commits to this file */
    UINT4 ulSmallFileCutoff; /* Max size of the stream in Mini streams */
    UINT4 ulSmallFatSector;  /* Start sector for MiniFat */
    UINT4 ulSmallFatSize;    /* no of MiniFat sectors */
    UINT4 ulXFatSector;      /* DIF start sector */
    UINT4 ulXFatSize;        /* number of sectors in DIF */
};


/** Signature at front of header */
#define SIGNATURE "\xd0\xcf\x11\xe0\xa1\xb1\x1a\xe1"
#define SIGNATURE_LEN 8


/*============================================================================
  Description: Retrieves the sector shift field from a header
               (1 << sectorshift gives the sector size)
  Arguments: 
   in_pHeader -     non-NULL pointer to a header structure to query
  Returns: 
   the sector shift
  ==========================================================================*/
UINT2
headerGetSectorShift(const Header* in_pHeader)
{
    ASSERT (in_pHeader != NULL);
    return in_pHeader->wSectorShift;
}


/*============================================================================
  Description: Sets the sector shift field in a header structure
               (1 << sectorshift gives the sector size)
  Arguments: 
   io_pHeader -         non-NULL pointer to a header structure to modify
   in_wNewSectorShift - new sector shift value
  ==========================================================================*/
void
headerSetSectorShift(Header* io_pHeader, UINT2 in_wNewSectorShift)
{
    ASSERT (io_pHeader != NULL);
    io_pHeader->wSectorShift = in_wNewSectorShift;

    /* Make sure small sectors are actually smaller than regular sectors */
    if (io_pHeader->wSmallSectorShift >= io_pHeader->wSectorShift)
    {
        io_pHeader->wSmallSectorShift = (io_pHeader->wSectorShift / 2);
    }

    /* Files with sector shifts other than 512 always seem to have the following
     * differences:
     *    (a) major version number is 4 rather than 3
     *    (b) bits 40-43 inclusive, which used to be reserved, contain the 
     *        number of sectors occupied by the table of contents (directory
     *        entries).  In 512-byte files, this number must be 0.
     *        We set this value in tocWrite().
     */
    if (in_wNewSectorShift != 9)
    {
        headerEnsureV4(io_pHeader);
    }
}


/*============================================================================
  Description: Retrieves the small sector shift field from a header
               (1 << smallSectorshift gives the small sector size)
  Arguments: 
   in_pHeader -     an open file to read from;
  Returns: 
   the small sector shift
  ==========================================================================*/
UINT2
headerGetSmallSectorShift(const Header* in_pHeader)
{
    ASSERT (in_pHeader != NULL);
    return in_pHeader->wSmallSectorShift;
}


/*============================================================================
  Description: Sets the length of the FAT (in sectors)
  Arguments: 
   io_pHeader - header to modify
   in_ulSize - new size
  ==========================================================================*/
void
headerSetFatSize(Header* io_pHeader, UINT4 in_ulSize)
{
    ASSERT (io_pHeader != NULL);
    io_pHeader->ulFatSectors = in_ulSize;
}


/*============================================================================
  Description: Retrieves the length of the FAT (in sectors)
  Arguments: 
   io_pHeader - header to query
  Returns:
   number of sectors taken up by the FAT
  ==========================================================================*/
UINT4
headerGetFatSize(const Header* in_pHeader)
{
    ASSERT (in_pHeader != NULL);
    return in_pHeader->ulFatSectors;
}


/*============================================================================
  Description: Sets the start sector of the DIF (double-indirect FAT) or XFat
  Arguments: 
   io_pHeader -  header to modify
   in_ulSector - new start sector
  ==========================================================================*/
void
headerSetXFatStartSector(Header* io_pHeader, UINT4 in_ulSector)
{
    ASSERT (io_pHeader != NULL);
    io_pHeader->ulXFatSector = in_ulSector;
}


/*============================================================================
  Description: Retrieves the start sector of the DIF (double-indirect FAT) or 
               XFat
  Arguments: 
   io_pHeader -  header to query
  Returns:
   start sector of XFat
  ==========================================================================*/
UINT4
headerGetXFatStartSector(const Header* in_pHeader)
{
    ASSERT (in_pHeader != NULL);
    return in_pHeader->ulXFatSector;
}


/*============================================================================
  Description: Sets the length of the DIF or XFat (in sectors)
  Arguments: 
   io_pHeader - header to modify
   in_ulSize - new size
  ==========================================================================*/
void
headerSetXFatSize(Header* io_pHeader, UINT4 in_ulSize)
{
    ASSERT (io_pHeader != NULL);
    io_pHeader->ulXFatSize = in_ulSize;
}


/*============================================================================
  Description: Gets the length of the DIF or XFat (in sectors)
  Arguments: 
   io_pHeader - header to query
  Returns:
   the number of sectors devoted to the XFAT/DIF
  ==========================================================================*/
UINT4
headerGetXFatSize(const Header* io_pHeader)
{
    ASSERT (io_pHeader != NULL);
    return io_pHeader->ulXFatSize;
}


/*============================================================================
  Description: Sets the start sector of the small FAT
  Arguments: 
   io_pHeader -  header to modify
   in_ulSector - new start sector
  ==========================================================================*/
void
headerSetSmallFatStartSector(Header* io_pHeader, UINT4 in_ulSector)
{
    ASSERT (io_pHeader != NULL);
    io_pHeader->ulSmallFatSector = in_ulSector;
}


/*============================================================================
  Description: Retrieves the small FAT start sector
  Arguments: 
   in_pHeader - header to query
  Returns: 
   the first sector of the small FAT
  ==========================================================================*/
UINT4
headerGetSmallFatStartSector(const Header* in_pHeader)
{
    ASSERT (in_pHeader != NULL);
    return in_pHeader->ulSmallFatSector;
}


/*============================================================================
  Description: Sets the length of the small FAT (in sectors)
  Arguments: 
   io_pHeader - header to modify
   in_ulSize - new size
  ==========================================================================*/
void
headerSetSmallFatSize(Header* io_pHeader,
                      UINT4 in_ulSize)
{
    ASSERT (io_pHeader != NULL);
    io_pHeader->ulSmallFatSize = in_ulSize;
}


/*============================================================================
  Description: Retrieves the length of the small FAT (in sectors)
  Arguments: 
   io_pHeader - header to query
  Returns:
   number of sectors taken up by the small FAT
  ==========================================================================*/
UINT4
headerGetSmallFatSize(const Header* in_pHeader)
{
    ASSERT (in_pHeader != NULL);
    return in_pHeader->ulSmallFatSize;
}


/*============================================================================
  Description: Sets the start sector of the table of contents
  Arguments: 
   io_pHeader -  header to modify
   in_ulSector - new start sector for the table of contents
  ==========================================================================*/
void
headerSetDirectoryStartSector(Header* io_pHeader, UINT4 in_ulSector)
{
    ASSERT (io_pHeader != NULL);
    io_pHeader->ulDirectorySector = in_ulSector;
}


/*============================================================================
  Description: Gets the number of the first sector of the table of contents
  Arguments: 
   io_pHeader - header to query
  Returns:
   start sector of table of contents
  ==========================================================================*/
UINT4
headerGetDirectoryStartSector(const Header* in_pHeader)
{
    ASSERT (in_pHeader != NULL);
    return in_pHeader->ulDirectorySector;
}


/*============================================================================
  Description: Gets the small stream size cutoff.  All streams smaller than
               this cutoff will be stored in the MiniStream and tracked in the 
               MiniFAT.
  Arguments: 
   io_pHeader - header to query
  Returns:
   small stream cutoff
  ==========================================================================*/
UINT4
headerGetSmallStreamCutoff(const Header* in_pHeader)
{
    ASSERT (in_pHeader != NULL);
    return in_pHeader->ulSmallFileCutoff;
}


/*============================================================================
  Description: 
   Reads in the structured storage header from the file and tores it in the 
  Arguments: 
   in_pIS -     an open input source to read from;
   out_pHeader - an allocated Header structure to fill    
  Returns: 
   Status code
  ==========================================================================*/

#define ALL_ZEROS(ARR) (is_empty((ARR), sizeof(ARR) / sizeof((ARR)[0])))

int is_empty(const char * buf, size_t size)
{
    size_t i = 0;
    for(; i < size; i++) {
        if(buf[i] != 0) return 0;
    }
    return 1;
}

SINT4
headerRead (SSRWIS* in_pIS, Header* out_pHeader)
{
    CHAR *buffer;

    buffer = (CHAR*)malloc (HEADER_SIZE);
    if (buffer == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( buffer, 0, HEADER_SIZE );


    SsrwFseek (in_pIS, 0, SEEK_SET);
    if (SsrwFread (buffer, HEADER_SIZE, 1, in_pIS) != 1)
    {
        free (buffer);
        return SSTG_ERROR_FILEIO;
    }


    if (memcmp (buffer, SIGNATURE, SIGNATURE_LEN) != 0)
    {
        free (buffer);
        return SSTG_ERROR_FILE_NOT_SSTG;
    }

    /* We don't know how to read the reserved fields in the correct endianness,
     * but we assume here that it's better to read something than nothing. */
    memcpy(out_pHeader->reserved1, buffer + 8, sizeof(out_pHeader->reserved1));
    if (!ALL_ZEROS(out_pHeader->reserved1)) {
        return SSTG_ERROR_FILEIO;
    }
    out_pHeader->wMinVersion = leReadUnsignedShort (buffer + 24);
    out_pHeader->wMajVersion = leReadUnsignedShort (buffer + 26);
    out_pHeader->wByteOrder = leReadUnsignedShort (buffer + 28);

    if (out_pHeader->wByteOrder != SSTG_BYTE_ORDER)
    {
        free (buffer);
        return SSTG_ERROR_FILE_NOT_SSTG;
    }

    out_pHeader->wSectorShift = leReadUnsignedShort (buffer + 30);
    out_pHeader->wSmallSectorShift = leReadUnsignedShort (buffer + 32);
    memcpy(out_pHeader->reserved2, buffer + 34, sizeof(out_pHeader->reserved2));

    if (!ALL_ZEROS(out_pHeader->reserved2)) {
        return SSTG_ERROR_FILEIO;
    }

    out_pHeader->ulNumDirSectors = leReadUnsignedLong(buffer + 40);
    out_pHeader->ulFatSectors = leReadUnsignedLong (buffer + 44);
    out_pHeader->ulDirectorySector = leReadUnsignedLong (buffer + 48);
    out_pHeader->ulCommit = leReadUnsignedLong (buffer + 52);
    out_pHeader->ulSmallFileCutoff = leReadUnsignedLong (buffer + 56);
    out_pHeader->ulSmallFatSector = leReadUnsignedLong (buffer + 60);
    out_pHeader->ulSmallFatSize = leReadUnsignedLong (buffer + 64);
    out_pHeader->ulXFatSector = leReadUnsignedLong (buffer + 68);
    out_pHeader->ulXFatSize = leReadUnsignedLong (buffer + 72);

    free (buffer);
    return SSTG_OK;
}


/*============================================================================
  Description: 
   Allocates a header structure and fills it with default values
  Arguments: 
   out_ppHeader - on success, will point to a freshly allocated header structure
  Returns: 
   Status code
  ==========================================================================*/
SINT4
headerInit(Header **out_ppHeader)
{
    Header* pHeader = NULL;

    pHeader = (Header*)malloc(sizeof(Header));
    if (pHeader == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( pHeader, 0, sizeof(Header));


    memcpy(pHeader->signature, SIGNATURE, SIGNATURE_LEN);
    memset(pHeader->reserved1, 0, sizeof(pHeader->reserved1));
    pHeader->wMinVersion = DEFAULT_MIN_VERSION;
    pHeader->wMajVersion = DEFAULT_MAX_VERSION;
    pHeader->wByteOrder = SSTG_BYTE_ORDER;

    pHeader->wSectorShift = DEFAULT_SECTOR_SHIFT;
    pHeader->wSmallSectorShift = DEFAULT_SMALL_SECTOR_SHIFT;
    memset(pHeader->reserved2, 0, sizeof(pHeader->reserved2));
    pHeader->ulNumDirSectors = 0;
    pHeader->ulFatSectors = 0;      /* no of FAT sectors in the file */
    pHeader->ulDirectorySector = (UINT4)-1; /* Start sector for TOC */
    pHeader->ulCommit = 1;          /* number of Commits to this file */
    pHeader->ulSmallFileCutoff = DEFAULT_SMALL_CUTOFF;
    pHeader->ulSmallFatSector = (UINT4)-1; /* Start sector for MiniFat */
    pHeader->ulSmallFatSize = 0;    /* no of MiniFat sectors */
    pHeader->ulXFatSector = (UINT4)-1; /* DIF start ector */
    pHeader->ulXFatSize = 0;        /* no of sectors in DIF */

    *out_ppHeader = pHeader;

    return SSTG_OK;
}
/*============================================================================
  Description: Reads the first 109 (or less) fat sectors that are within the continuation
               of the header 
  Arguments: 
   in_pIS -      open pIS to read from
   in_pHeader -   pointer to a filled Header structure for the file
   out_pSectors - pre-allocated buffer of FAT sectors
   out_pCount -   number of FAT sectors read in (and the size of an allocated array)
  Return: 
   Status code
  ==========================================================================*/
SINT4
headerReadFatSectors(SSRWIS* in_pIS,
                     Header* in_pHeader,
                     UINT4* out_pSectors,
                     UINT4* out_pCount)
{
    UINT4   i;
    UINT4*  pSector;
    CHAR*   buffer;
    CHAR*   pB;
    SINT4   iRet =      SSTG_OK;
    UINT4   nSectors;

    nSectors = min(HEADER_FAT_SECTORS, in_pHeader->ulFatSectors);

    /* speedy return on empty file */
    if (nSectors == 0)
    {
        *out_pCount = 0;
        return iRet;
    }

    /* allocate buffer to read in the header FAT sectors */
    buffer = (CHAR*)malloc (nSectors * sizeof (UINT4));
    if (buffer == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(buffer, 0, nSectors * sizeof (UINT4));


    /* set up input source reading pointer */
    if (SsrwFseek (in_pIS, HEADER_SIZE, SEEK_SET) != 0)
    {
        free(buffer);
        return SSTG_ERROR_FILEIO;
    }

    /* read into the buffer */
    if (SsrwFread (buffer, nSectors * sizeof (UINT4), 1, in_pIS) != 1)
    {
        free(buffer);
        return SSTG_ERROR_FILEIO;
    }

    pSector = out_pSectors;

    /* Copy sector numbers from buffer to the array with necessary endian conversion */
    pB = buffer;
    for (i = 0; i < nSectors; i++)
    {
        *pSector = leReadUnsignedLong (pB);
        pSector++;
        pB += sizeof(UINT4);
    }

    /* assignment and cleanup */
    free (buffer);
    *out_pCount = nSectors;

    return iRet;
}


/*============================================================================
  Description: Retrieves the major version number of the structured storage 
               file
  Arguments: 
   in_pHeader -   pointer to a filled Header structure for the file
  Return: 
   Major version number.  Microsoft uses 3 for older files, 4 for newer files 
   with 4096-byte sectors.
  ==========================================================================*/
UINT2
headerGetMajorVersion(const Header* in_pHeader)
{
    return in_pHeader->wMajVersion;
}


/*============================================================================
  Description: Ensures that the structured storage file is versioned to
               properly to allow streams >2GB in size.
  Arguments: 
   io_pHeader - pointer to a header to update
  Return: 
   Status code
  ==========================================================================*/
SINT4 
headerEnsureV4(Header* io_pHeader)
{
    io_pHeader->wMajVersion = 4;

    return SSTG_OK;
}


/*============================================================================
  Description: Sets the number of sectors occupied by the table of contents.
               This value is ignored and must be zero for 512-byte sector 
               files, but must give a correct count for 4096-byte sector 
               files.
              
  Arguments: 
   io_pHeader -         pointer to a header to update
   in_ulNumDirSectors - the number of sectors containing directory entries
  Return: 
   Status code
  ==========================================================================*/
SINT4 
headerSetNumDirSectors(Header* io_pHeader, UINT4 in_ulNumDirSectors)
{
    io_pHeader->ulNumDirSectors = in_ulNumDirSectors;

    return SSTG_OK;
}


/*============================================================================
  Description: Writes the header to an input source.
  Arguments:  
   in_file -   an open file to write to
   in_header - a pointer to a filled header structure to write down
  Return: 
   Status code SSTG_OK for success, SSTG_ERROR_... for error condition
  ==========================================================================*/
SINT4
headerWrite (SSRWIS* in_pIS, Header* in_header)
{
    CHAR *buffer;

    /* allocate and fill in the buffer doing endian conversion */
    buffer = (CHAR*)malloc (HEADER_SIZE);
    if (buffer == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }


    memcpy (buffer, SIGNATURE, SIGNATURE_LEN);
    /* We don't know how to write the reserved fields in the correct endianness,
     * but we assume here that it's better to write something than nothing 
     * (uninitialized memory). */
    memcpy (buffer + 8, in_header->reserved1, sizeof(in_header->reserved1));
    leWriteUnsignedShort (in_header->wMinVersion, buffer + 24);
    leWriteUnsignedShort (in_header->wMajVersion, buffer + 26);
    leWriteUnsignedShort (in_header->wByteOrder, buffer + 28);
    leWriteUnsignedShort (in_header->wSectorShift, buffer + 30);
    leWriteUnsignedShort (in_header->wSmallSectorShift, buffer + 32);
    memcpy (buffer + 34, in_header->reserved2, sizeof(in_header->reserved2));
    leWriteUnsignedLong (in_header->ulNumDirSectors, buffer + 40);
    leWriteUnsignedLong (in_header->ulFatSectors, buffer + 44);
    leWriteUnsignedLong (in_header->ulDirectorySector, buffer + 48);
    leWriteUnsignedLong (in_header->ulCommit, buffer + 52);
    leWriteUnsignedLong (in_header->ulSmallFileCutoff, buffer + 56);
    leWriteUnsignedLong (in_header->ulSmallFatSector, buffer + 60);
    leWriteUnsignedLong (in_header->ulSmallFatSize, buffer + 64);
    leWriteUnsignedLong (in_header->ulXFatSector, buffer + 68);
    leWriteUnsignedLong (in_header->ulXFatSize, buffer + 72);


    /* set up file writing pointer */
    if (SsrwFseek(in_pIS, 0, SEEK_SET) != 0)
    {
        free(buffer);
        return SSTG_ERROR_FILEIO;
    }

    /* write the buffer down */
    if (SsrwFwrite (buffer, HEADER_SIZE, 1, in_pIS) != 1)
    {
        free (buffer);
        return SSTG_ERROR_FILEIO;
    }

    free (buffer);
    return SSTG_OK;
}


/*============================================================================
  Description: Fills the "header block" of the file, which is the same size as
               a sector, with appropriate "blank" data.  The portion often 
               taken up by the 109 DIF sectors is filled with 0xffffffff, and
               everything everything after that is taken up with 0x00000000.
  Arguments: 
   in_pHeader -   pointer to a filled Header structure for the file
   in_pIS -       open input source to write to
  Return: 
   Status code
  ==========================================================================*/
SINT4
headerWriteSectorFiller(const Header* in_pHeader,
                        SSRWIS* in_pIS)
{
    UINT4       i =                     0;
    UINT4       ulSectorSize =          0;
    UINT4       ulStartPos =            0;
    UINT4       ulSectors =             0;
    UINT4       ulFatSectorListEnd =    0;
    CHAR        aZeroBuffer[4] =        { 0, 0, 0, 0 };
    CHAR        aFFFFBuffer[4] =        { -1, -1, -1, -1 };

    /* The header structure itself is exactly 76 bytes, when serialised.
     * That structure is followed by up to 109 4-byte FAT sectors.  The filler
     * starts after that. */
    ulSectors = min(HEADER_FAT_SECTORS, in_pHeader->ulFatSectors);
    ulStartPos = 76 + sizeof(UINT4) * ulSectors;
    ulSectorSize = 1 << in_pHeader->wSectorShift;
    ulFatSectorListEnd = min(ulSectorSize, 512);

    for (i = ulStartPos; i < ulFatSectorListEnd; i += sizeof(aFFFFBuffer))
    {
        /* Note that 0xffffffff is the same regardless of platform endianness */
        if (SsrwFwrite (aFFFFBuffer, sizeof(aFFFFBuffer), 1, in_pIS) != 1)
        {
            return SSTG_ERROR_FILEIO;
        }
    }

    for (i = ulFatSectorListEnd; i < ulSectorSize; i += sizeof(aZeroBuffer))
    {
        /* Note that 0x00000000 is the same regardless of platform endianness */
        if (SsrwFwrite (aZeroBuffer, sizeof(aZeroBuffer), 1, in_pIS) != 1)
        {
            return SSTG_ERROR_FILEIO;
        }
    }

    return SSTG_OK;
}


/*============================================================================
  Description: Writes the first 109 (or less) fat sectors into the continuation
               of the header 
  Arguments: 
   in_pHeader -   pointer to a filled Header structure for the file
   in_pIS -       open input source to write to
   in_alSectors - array of FAT sectors to write
   in_count -     number of FAT sectors to write (and the size of in_alSectors)
  Return: 
   Status code
  ==========================================================================*/
SINT4
headerWriteFatSectors(SSRWIS* in_pIS,
                      UINT4* in_alSectors,
                      UINT4 in_count)
{
    UINT4       i =         0;
    UINT4*      pSector =   NULL;
    CHAR*       buffer =    NULL;
    CHAR*       pB =        NULL;
    SINT4       iRet =      SSTG_OK;
    UINT4       nSectors =  0;

    nSectors = min(HEADER_FAT_SECTORS, in_count);

    /* speedy return on empty file */
    if (nSectors == 0)
    {
        return iRet;
    }

    /* allocate buffer to write in the header FAT sectors */
    buffer = (CHAR*)malloc (nSectors * sizeof (UINT4));
    if (buffer == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(buffer, 0, nSectors * sizeof (UINT4));


    /* Copy sector numbers from the array to buffer with necessary endian conversion */
    pB = buffer;
    pSector = &in_alSectors[0];
    for (i = 0; i < nSectors; i++)
    {
        leWriteUnsignedLong (*pSector, pB);
        pSector++;
        pB += sizeof(UINT4);
    }

    /* set up input source writing pointer */
    if (SsrwFseek (in_pIS, HEADER_SIZE, SEEK_SET) != 0)
    {
        free(buffer);
        return SSTG_ERROR_FILEIO;
    }

    /* write the buffer */
    if (SsrwFwrite (buffer, nSectors * sizeof (UINT4), 1, in_pIS) != 1)
    {
        free(buffer);
        return SSTG_ERROR_FILEIO;
    }

    /* cleanup */
    free (buffer);

    return iRet;
}


#ifdef DEBUG
/*============================================================================
  Description: Self-test of the header ADT.  Writes a header to disk and reads
               it back in again, then compares the results.
  Return: 
   Status code
  ==========================================================================*/
SINT4
headerSelfTest()
{
    const wchar_t*  fn =                    L"testSSRW.bin";
    SSRWIS*         pIS =                   NULL;
    SINT4           rc =                    SSTG_OK;
    int             i = 0;
    Header*         test1;
    Header*         test2;
    UINT4           test1FatStart[109];
    UINT4           test2FatStart[109];
    UINT4           test2FatStartCount =    0;

    rc = headerInit(&test1);
    if (rc != SSTG_OK)
    {
        return rc;
    }

    rc = headerInit(&test2);
    if (rc != SSTG_OK)
    {
        return rc;
    }

    /* We don't write funny values into the reserved fields because it is not
     * clear how they should be written and/or read in the appropriate 
     * endianness.  So we just don't test the reserved field reading/writing
     * code. */
    memcpy(test1->signature, SIGNATURE, SIGNATURE_LEN);
    test1->wMinVersion = 13;
    test1->wMajVersion = 17;
    test1->wByteOrder = SSTG_BYTE_ORDER;
    test1->wSectorShift = 9;
    test1->wSmallSectorShift = 6;
    test1->ulFatSectors = 109;
    test1->ulDirectorySector = 2;
    test1->ulCommit = 13;
    test1->ulSmallFileCutoff = 1000;
    test1->ulSmallFatSector = 100;
    test1->ulSmallFatSize = 7;
    test1->ulXFatSector = 101;
    test1->ulXFatSize = 999;


    for (i = 0; i < 109; i++)
    {
        test1FatStart[i] = rand();
    }

    pIS = SsrwOpenFile(fn, L"wb");
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILEIO;
    }

    rc = headerWrite(pIS, test1);
    if (rc != SSTG_OK)
    {
        SsrwFclose(pIS);
        pIS = NULL;
        return rc;
    }

    rc = headerWriteFatSectors(pIS, test1FatStart, 109);
    if (rc != SSTG_OK)
    {
        SsrwFclose(pIS);
        pIS = NULL;
        return rc;
    }

    /* Close the file */
    SsrwFclose(pIS);

    /* ...and reopen for reading */
    pIS = SsrwOpenFile(fn, L"rb");
    if (pIS == NULL)
    {
        return SSTG_ERROR_FILEIO;
    }

    rc = headerRead(pIS, test2);
    if (rc != SSTG_OK)
    {
        SsrwFclose(pIS);
        return rc;
    }

    rc = memcmp((void*)test1, (void*)test2, sizeof(Header));
    if (rc != 0)
    {
        return SSTG_ERROR_FILE_NOT_SSTG;
    }

    rc = headerReadFatSectors(pIS, test2, test2FatStart, &test2FatStartCount);
    if (rc != 0)
    {
        return rc;
    }

    SsrwFclose(pIS);

    SSRW_WREMOVE(fn);

    rc = memcmp((void*)test1FatStart,(void*)test2FatStart, 109 * sizeof(UINT4));
    if (test2FatStartCount != 109 || rc != 0)
    {
        return SSTG_ERROR_FILE_NOT_SSTG;
    }

    free(test1);
    free(test2);

    return SSTG_OK;
}
#endif /* DEBUG */

