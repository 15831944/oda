/************************************************************************
* Fat.c
* Implemetnation of the Fat "object" method as it represented INSIDE
* the program, not inside the compound file
*
* (c) Schema Software Inc., 2001 - 2004
* 
*************************************************************************

$Revision: 1.11.2.6 $
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
#include "Fat.h"
#include "LEReadWrite.h"
#include "Directory.h"
#include "TOC.h"


#define FREE_SECTOR     ((UINT4)0xffffffff)
#define END_OF_CHAIN    ((UINT4)0xfffffffe)
#define USED_BY_FAT     ((UINT4)0xfffffffd)
#define USED_BY_XFAT    ((UINT4)0xfffffffc)

/* Defines a "squishy" limit for the largest sector index that may be used.
 * Metadata like the TOC, FAT, etc. may exceed this limit.
 * The number is less than 0xfffffffb because writing to a stream must fail
 * early. That way, hopefully, the FAT and TOC can still be written successfully
 * after the squishy limit is reached.  Otherwise the resulting structured 
 * storage file would obviously be corrupt.  */
#define FAT_SECT_MAX    ((UINT4)0xff000000)

/* If more special FAT values appear then MIN_FAT_SPECIAL_VALUE must be updated
 * to the lowest special FAT value. */
#define MIN_FAT_SPECIAL_VALUE USED_BY_XFAT

#ifdef DEBUG
static SINT4 assertFatFreeChainValid(Fat* io_pFat);
#endif /* DEBUG */


struct _Fat
{
    UINT4* alFatSectors;      /* sequential list of sectors is stg used by fat */
    UINT4* alFat;             /* the fat */

    UINT4 ulFatLength;        /* number of sectors addressed in fat */
    UINT4 ulFatReserved;      /* space reserved in alFat */
    UINT4 ulNumSectors;       /* number of sectors used by the Fat */
    RootStorage* stg;
    UINT4 ulFreeStartSector;  /* first sector in the "free sectors" chain */
    UINT2 wSectorSize;        /* sector size represented by this fat */
};

/** STATIC FUNCTION PROTOTYPES                                                  */
static SINT4 countChainLength (Fat* fat, UINT4 in_start, UINT4* length );

/*============================================================================
  Description:  Allocates and initializes a Fat structure
  Arguments: 
   in_root -        root storage structure
   out_fat -        pointer to pointer to fill with new Fat structure
   in_wSectorSize - size of sectors in this FAT
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatConstruct (RootStorage* in_root, Fat** out_fat, UINT2 in_wSectorSize)
{
    Fat* thisFat = NULL;

    thisFat = (Fat*)malloc (sizeof(Fat));
    if (thisFat == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( thisFat, 0, sizeof(Fat));


    thisFat->alFat = NULL;
    thisFat->alFatSectors = NULL;
    thisFat->stg = in_root;
    thisFat->ulFatLength = 0;
    thisFat->ulFatReserved = 0;
    thisFat->ulNumSectors = 0;
    thisFat->ulFreeStartSector = END_OF_CHAIN;
    thisFat->wSectorSize = in_wSectorSize;

    *out_fat = (Fat*)thisFat;
    return SSTG_OK;
}


/*============================================================================
  Description:  Frees all memory associated with a FAT structure
  Arguments: 
   io_ppFat - FAT structure to destroy
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatDestroy (Fat** io_ppFat)
{
    if ((*io_ppFat)->alFat != NULL)
    {
        free ((*io_ppFat)->alFat);
    }

    if ((*io_ppFat)->alFatSectors != NULL)
    {
        free ((*io_ppFat)->alFatSectors);
    }

    free(*io_ppFat);
    *io_ppFat = NULL;

    return SSTG_OK;
}


/*============================================================================
  Description:  "Set" accessor for the internal DIF (double-indirect FAT)
  Arguments: 
   in_pFat -       FAT for which to set the fat sectors
   in_pulSectors - FAT sector array
   in_ulCount -   number of sectors in the array
  ==========================================================================*/
void
fatSetFatSectors (Fat* in_pFat,
                  UINT4* in_pulSectors,
                  UINT4 in_ulCount)
{
    in_pFat->alFatSectors = in_pulSectors;
    in_pFat->ulNumSectors = in_ulCount;
}


/*============================================================================
  Description:  "Get" accessor for the internal DIF (double-indirect FAT)
  Arguments: 
   in_pFat -         FAT structure from which to get the fat sectors
   out_ppulSectors - on success, will point to the FAT sector array
   out_pulCount -    on success, will contain the number of sectors in the 
                     array
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatGetFatSectors (Fat* in_pFat,
                  UINT4** out_ppulSectors,
                  UINT4 *out_pulCount)
{
    if (out_ppulSectors != NULL)
    {
        *out_ppulSectors = in_pFat->alFatSectors;
    }
    if (out_pulCount != NULL)
    {
        *out_pulCount = in_pFat->ulNumSectors;
    }

    return SSTG_OK;
}


/*============================================================================
  Description:  Reads the content of the FAT from disk
  Arguments: 
   in_pFat - FAT structure to fill out.  The stg member and ulNumSectors member
             must already be set.
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatReadFat (Fat* io_pFat)
{
    UINT4*      sectorBuffer =                      NULL;
    UINT4       sectorsPerFATSector =               0;
    UINT4       iSector =                           0;
    UINT4       ulLastFreeSector =                  END_OF_CHAIN;
    UINT4       ulLastUsedSector =                  END_OF_CHAIN;
    UINT4       ulLastFreeSectorBeforeUsedSector =  END_OF_CHAIN;
    UINT4       ulSector =                          END_OF_CHAIN;
    UINT4       ulMaxReferencedSector =             0;
    UINT4       j =                                 0;
    UINT4*      pL =                                NULL;
    UINT4*      pF =                                NULL;
    UINT4*      pSector =                           NULL;
    SINT4       iRet =                              SSTG_OK;
    Header*     pHdr =                              NULL;
    UINT2       wSectorShift =                      0;

    ASSERT (io_pFat != NULL);
    ASSERT (io_pFat->stg != NULL);
    ASSERT (io_pFat->ulNumSectors > 0);

    if ( io_pFat->alFatSectors == NULL )
    {
        /* someone made a mistake */
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    pHdr = rootStorageGetHeader(io_pFat->stg);
    wSectorShift = headerGetSectorShift(pHdr);

    sectorsPerFATSector = (1 << wSectorShift) / sizeof (UINT4);
    io_pFat->ulFatLength = io_pFat->ulNumSectors * sectorsPerFATSector;

    io_pFat->alFat = (UINT4*)calloc (io_pFat->ulFatLength, sizeof (UINT4));
    if (io_pFat->alFat == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }
    io_pFat->ulFatReserved = io_pFat->ulFatLength;

    if (*(io_pFat->alFatSectors) == END_OF_CHAIN)
    {
        return iRet;
    }

    /* now read the thing in                                                */
    for (iSector = 0, pSector = io_pFat->alFatSectors, pF = io_pFat->alFat;
         iSector < io_pFat->ulNumSectors && iRet == SSTG_OK;
         iSector++, pSector++)
    {
        if (iSector == 0)
        {
            sectorBuffer = (UINT4*)rootStorageReadSector (io_pFat->stg, *pSector);
            if (sectorBuffer == NULL)
            {
                iRet = rootStorageGetStatus (io_pFat->stg);
            }
        }
        else
        {
            iRet = rootStorageGetSector (io_pFat->stg, *pSector, (CHAR*)sectorBuffer);
        }

        if (iRet == SSTG_OK)
        {
            for (j = iSector * sectorsPerFATSector, pL = sectorBuffer; 
                 j < (iSector + 1) * sectorsPerFATSector; 
                 j++, pL++)
            {
                ulSector = leReadUnsignedLong (pL);
                if (ulSector < 0xfffffff0 && ulSector > ulMaxReferencedSector)
                {
                    ulMaxReferencedSector = ulSector;
                }
                if (ulSector == FREE_SECTOR)
                {
                    if (ulLastFreeSector == END_OF_CHAIN)
                    {
                        io_pFat->ulFreeStartSector = j;
                    }
                    else
                    {
                        io_pFat->alFat[ulLastFreeSector] = j;
                    }
                    ulLastFreeSector = j;
                    io_pFat->alFat[j] = END_OF_CHAIN;
                }
                else
                {
                    io_pFat->alFat[j] = ulSector;
                    ulLastUsedSector = j;
                    ulLastFreeSectorBeforeUsedSector = ulLastFreeSector;
                }
            }
        }
    }

    ASSERT(ulMaxReferencedSector <= ulLastUsedSector);
    /* Adjust the FAT length to reflect any free sectors that may have been
     * lying around at the end of the FAT */
    ASSERT((ulLastUsedSector == END_OF_CHAIN) || 
           (ulLastUsedSector + 1) <= (io_pFat->ulFatLength));
    if (ulLastUsedSector != END_OF_CHAIN)
    {
        io_pFat->ulFatLength = ulLastUsedSector + 1;

        /* Now terminate the free chain before the new end of the FAT */
        if (ulLastFreeSectorBeforeUsedSector == END_OF_CHAIN)
        {
            io_pFat->ulFreeStartSector = END_OF_CHAIN;
        }
        else
        {
            io_pFat->alFat[ulLastFreeSectorBeforeUsedSector] = END_OF_CHAIN;
        }
    }

    return iRet;
}


/*============================================================================
  Description:  Given a data length and a sector size, calculates how many
                sectors it should take to store that data
  Arguments: 
   cbData -   length (in bytes) of the data
   cbSector - length (in bytes) of an individual sector
   
  Return:       
   Status code
  ==========================================================================*/
UINT4
calcReqdSectors(UINT8 in_cbData, UINT4 cbSector)
{
    return (UINT4)((in_cbData + ((UINT8)(cbSector - 1))) / ((UINT8)cbSector));
}


/*============================================================================
  Description:  Ensures that the FAT contains at least a given number of 
                entries
  Arguments: 
   io_pFat -      FAT to resize if necessary
   in_ulMinSize - new minimum size of the FAT
  Return:       
   Status code
  ==========================================================================*/
SINT4 
fatEnsureMinSize(Fat* io_pFat, UINT4 in_ulMinSize)
{
    Header* pHdr =              NULL;
    UINT4   ulSectorSizeInc =   0;
    UINT2   wSectorShift =      0;
    UINT4   ulNewSectorSize =   0;
    UINT4   ulNewArraySize =    0;
    UINT4*  pBuf =              NULL;

    if (in_ulMinSize > io_pFat->ulFatReserved)
    {
        ASSERT((io_pFat != NULL) && (io_pFat->stg != NULL) &&
               (rootStorageGetHeader(io_pFat->stg) != NULL));

        pHdr = rootStorageGetHeader(io_pFat->stg);
        wSectorShift = headerGetSectorShift(pHdr);
        ulSectorSizeInc = 1 << wSectorShift;

        ulNewArraySize = in_ulMinSize;
        ulNewSectorSize =
            ((((ulNewArraySize * sizeof(UINT4)) + (ulSectorSizeInc - 1)) /
              ulSectorSizeInc) * ulSectorSizeInc);
        pBuf = (UINT4*)realloc(io_pFat->alFat, ulNewSectorSize);
        if (pBuf == NULL)
        {
            return SSTG_ERROR_MEMORY;
        }
        io_pFat->alFat = pBuf;
        io_pFat->ulFatReserved = ulNewSectorSize / sizeof(UINT4);
    }

    return SSTG_OK;
}


/*============================================================================
  Description:  Sets a single FAT entry in memory, resizing the array if 
                necessary
  Arguments: 
   io_pFat -     FAT to modify
   in_ulIndex - index of entry to modify
   in_ulVal -   new value to insert
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatSetEntry (Fat* io_pFat, UINT4 in_ulIndex, UINT4 in_ulVal)
{
    UINT4 i =       0;
    SINT4 iRet =    SSTG_OK;

    ASSERT((io_pFat != NULL) && (io_pFat->stg != NULL) &&
           (rootStorageGetHeader(io_pFat->stg) != NULL));

    iRet = fatEnsureMinSize(io_pFat, in_ulIndex + 1);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    io_pFat->alFat[in_ulIndex] = in_ulVal;

    /* If we're setting a sector beyond the end of the current FAT, update the
     * length of the FAT and initialize the intervening (uninitialized) FAT 
     * entries to FREE_SECTOR. */
    if (in_ulIndex >= io_pFat->ulFatLength)
    {
        for (i = io_pFat->ulFatLength; i < in_ulIndex; i++)
        {
            io_pFat->alFat[i] = FREE_SECTOR;
        }
        io_pFat->ulFatLength = in_ulIndex + 1;
    }

    return SSTG_OK;
}


/*============================================================================
  Description:  Updates the FAT.  This should be called each time a new sector
                is written to disk.
  Arguments: 
   io_pFat -            FAT to modify
   in_ulLastFatIndex - FAT entry index of last sector written to disk
   in_ulNewFatIndex -  FAT entry index of new sector just written to disk 
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatUpdate (Fat* io_pFat, UINT4 in_ulLastFatIndex, UINT4 in_ulNewFatIndex)
{
    SINT4 iRet = SSTG_OK;

    /* If you're setting a special value you can just use fatSetEntry.
     * We can remove this assert if necessary but we'd like to know if this
     * function is being used in this way */
    ASSERT (in_ulNewFatIndex < MIN_FAT_SPECIAL_VALUE);

    iRet = fatSetEntry (io_pFat, in_ulLastFatIndex, in_ulNewFatIndex);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    if (in_ulNewFatIndex < MIN_FAT_SPECIAL_VALUE) /* Just in case */
    {
        /* We always rewrite the end-of-chain marker on the fly.  We could wait
         * until the end to write it, but this is safer and arguably easier */
        iRet = fatSetEntry (io_pFat, in_ulNewFatIndex, END_OF_CHAIN);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    return iRet;
}


/*============================================================================
  Description:  Retrieves a chain of FAT entries from disk and makes a 
                sequential copy of them in a new buffer.
  Arguments: 
   in_pFat -      FAT to read from
   in_start -     start sector of chain to retrieve
   io_pulLength - length of fat chain to retrieve.  If this is 0, it is calculated
                  and set on success.
   out_chain -    pointer to variable that, on success, will point to an array 
                  of sector numbers representing a single chain in the FAT
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatGetChain (Fat* in_pFat,
             UINT4 in_start,
             UINT4* io_pulLength,
             UINT4** out_chain)
{
    UINT4*  pNext =     NULL;
    UINT4*  aChain =    NULL;
    UINT4*  pC =        NULL;
    UINT4   count =     0;
    SINT4   rc =        SSTG_OK;

    ASSERT(io_pulLength != NULL);

    /* Calculate the length of the chain if necessary */
    if (*io_pulLength == (UINT4)-1)
    {
        rc = countChainLength (in_pFat, in_start, io_pulLength);
        if (rc != SSTG_OK)
            return rc;
    }

    if (*io_pulLength == 0)
    {
        return SSTG_OK;
    }

    aChain = (UINT4*)calloc (*io_pulLength, sizeof (UINT4));
    if (aChain == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }
    *out_chain = aChain;

    *aChain = in_start;
    pC = aChain + 1;

    if (in_start == END_OF_CHAIN)
    {
        return SSTG_OK;
    }

    /* Read FAT entries */
    pNext = in_pFat->alFat + in_start;
    while (*pNext != END_OF_CHAIN)
    {
        if (count >= (*io_pulLength - 1))
        {
            /* There are more sectors stored in the FAT than are necessary.
             * Stop now, because there could be an infinite loop or something.
             * Sometimes structured storage files do appear with extra sectors,
             * and they are read correctly by MS apps, so it is not desirable 
             * to generate an error here. */
            break;
        }

        if ((*pNext == FREE_SECTOR) ||
            (*pNext == USED_BY_FAT) ||
            (*pNext == USED_BY_XFAT))
        {
            ASSERT (0);
            /* Something is broken */
            free (aChain);
            *out_chain = NULL;
            return SSTG_ERROR_CORRUPTED_FAT;
        }
        if (*pNext > in_pFat->ulFatReserved) {
            return SSTG_ERROR_CORRUPTED_FAT;
        }
        *pC++ = *pNext;
        count++;
        pNext = in_pFat->alFat + *pNext;
    }
    if ((count+1) < *io_pulLength) {
      return SSTG_ERROR_CORRUPTED_FAT;
    }
    return SSTG_OK;
}


/*============================================================================
  Description: Determines the last sector in a given chain
  Arguments: 
   in_pFat -      FAT to query
   in_start -     start sector of chain
   out_pulCount - on success, will contain the last sector in the chain
  Return: 
   Status code
  ==========================================================================*/
SINT4 
fatGetChainLastSector (Fat* in_pFat,
                       UINT4 in_ulStartSector,
                       UINT4* out_pulLastSector)
{
    UINT4   count = 1;
    UINT4*  pNext = NULL;
    UINT4*  pPrev = &in_ulStartSector;

    if ((in_ulStartSector == FREE_SECTOR) || 
        (in_ulStartSector == END_OF_CHAIN))
    {
        *out_pulLastSector = FREE_SECTOR;
        return SSTG_OK;
    }

    if (in_ulStartSector > in_pFat->ulFatLength)
    {
        return SSTG_ERROR_CORRUPTED_FAT;
    }

    pNext = in_pFat->alFat + in_ulStartSector;

    if (*pNext > in_pFat->ulFatLength && *pNext != END_OF_CHAIN)
    {
        return SSTG_ERROR_CORRUPTED_FAT;
    }

    while (*pNext != END_OF_CHAIN)
    {
        pPrev = pNext;

        count++;
        pNext = in_pFat->alFat + *pNext;

        if (count > in_pFat->ulFatLength)
        {
            /* infinite loop escape hatch */
            return SSTG_ERROR_CORRUPTED_FAT;
        }

        if (*pNext > in_pFat->ulFatLength && *pNext != END_OF_CHAIN)
        {
            /* SIGSEGV avoidance */
            return SSTG_ERROR_CORRUPTED_FAT;
        }

    }
    *out_pulLastSector = *pPrev;

    return SSTG_OK;
}


/*============================================================================
  Description: Counts length of chain by traversing fat.
  Arguments: 
   in_pFat -      FAT in which to count the length of a chain
   in_start -     start sector of chain
   out_pulCount - on success, will contain the number of sectors in the chain
  Return: 
   Status code
  ==========================================================================*/
static SINT4
countChainLength (Fat* in_pFat, UINT4 in_start, UINT4* out_pulCount)
{
    UINT4   count = 1;
    UINT4*  pNext;

    if (in_start > in_pFat->ulFatLength)
    {
        return SSTG_ERROR_CORRUPTED_FAT;
    }

    pNext = in_pFat->alFat + in_start;

    if (*pNext > in_pFat->ulFatLength && *pNext != END_OF_CHAIN)
    {
        return SSTG_ERROR_CORRUPTED_FAT;
    }

    while (*pNext != END_OF_CHAIN)
    {
        count++;
        pNext = in_pFat->alFat + *pNext;

        if (count > in_pFat->ulFatLength)
        {
            /* infinite loop escape hatch                           */
            return SSTG_ERROR_CORRUPTED_FAT;
        }

        if (*pNext > in_pFat->ulFatLength && *pNext != END_OF_CHAIN)
        {
            /* SIGSEGV avoidance                           */
            return SSTG_ERROR_CORRUPTED_FAT;
        }

    }
    *out_pulCount = count;

    return SSTG_OK;
}


/*============================================================================
  Description: Returns the number of sectors referenced in the FAT
  Arguments: 
   in_pFat - pointer to FAT structure
  Return: 
   number of entries in FAT
  ==========================================================================*/
UINT4
fatGetLength (Fat* in_pFat)
{
    ASSERT(in_pFat != NULL);

    return in_pFat->ulFatLength;
}


/*============================================================================
  Description: Inserts a chain terminator 
  Arguments: 
   io_pFat -          pointer to FAT structure to modify
   in_ulFinalSector - final sector of chain to terminate
  Return: 
   Status code
  ==========================================================================*/
SINT4
fatTerminateChain(Fat* io_pFat, UINT4 in_ulFinalSector)
{
    return fatSetEntry(io_pFat, in_ulFinalSector, END_OF_CHAIN);
}


/*============================================================================
  Description: Marks a specified sector as unused space
  Arguments: 
   io_pFat -          pointer to FAT structure to modify
   in_ulFinalSector - index of free sector
  Return: 
   Status code
  ==========================================================================*/
SINT4
fatSetFreeSector(Fat* io_pFat, UINT4 in_ulSector)
{
    return fatSetEntry(io_pFat, in_ulSector, FREE_SECTOR);
}

/*============================================================================
  Description:  Writes a FAT table from memory.
  Arguments: 
   in_pFat -          pointer to the FAT to write
   in_ulStartSector - index of first sector in file to write to.  Further
                      sectors will be picked sequentially after the first.
  Return:       
   Status code
  ==========================================================================*/
static SINT4
fatWriteFat (Fat* in_pFat, UINT4 in_ulStartSector)
{
    UINT4*      pbBuf =                 NULL;
    SINT4       iRet =                  SSTG_OK;
    UINT4       cbSector =              0;
    UINT4       sectorsPerFATSector =   0;
    UINT4       i =                     0;
    UINT4       j =                     0;
    Header*     pHdr =                  NULL;
    UINT2       wSectorShift =          0;

    if (in_pFat->ulFatLength == 0)
    {
        return SSTG_OK;
    }

    pHdr = rootStorageGetHeader(in_pFat->stg);
    wSectorShift = headerGetSectorShift(pHdr);
    cbSector = (1 << wSectorShift);
    sectorsPerFATSector = cbSector / sizeof (UINT4);

    pbBuf = (UINT4*)malloc(cbSector);
    if (pbBuf == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset(pbBuf, 0, cbSector);


    /* now write the FAT itself */
    for (i = 0; i < in_pFat->ulFatLength && iRet == SSTG_OK; )
    {
        for (j = 0; j < sectorsPerFATSector && i < in_pFat->ulFatLength; j++, i++)
        {
            leWriteUnsignedLong ((in_pFat->alFat)[i], &pbBuf[j]);
        }

        /* Fill any possible remaining spaces in the FAT sector to be written
         * with FREE_SECTOR */
        for (; j < sectorsPerFATSector; j++)
        {
            leWriteUnsignedLong(FREE_SECTOR, &pbBuf[j]);
        }

        iRet = rootStorageRewriteSector (
                   in_pFat->stg, (BYTE*)pbBuf, in_ulStartSector);
        if (iRet != SSTG_OK)
        {
            free (pbBuf);
            return iRet;
        }
        in_ulStartSector++;
    }

    free(pbBuf);

    return iRet;
}


/*============================================================================
  Description:  Writes the MiniFAT from memory.
                All streams and storages must be flushed before this function
                is called.  Also, the MiniFAT passed in must have a correct 
                value in its ulFatLength field.
  Arguments: 
   in_pFat - pointer to the MiniFAT to write
  Return:
   Status code
  ==========================================================================*/
SINT4
fatWriteMiniFat (Fat* in_pFat)
{
    UINT4       cSectorsWritten =   0;
    UINT4       cMiniFATSectors =   0;
    UINT4       cbSector =          0;
    SINT4       iRet =              SSTG_OK;
    UINT4       i =                 0;
    Header*     pHdr =              NULL;
    UINT2       wSectorShift =      0;

    ASSERT (in_pFat != NULL);
    ASSERT (in_pFat->stg != NULL);
    ASSERT (in_pFat->ulFatLength == 0 || in_pFat->alFat != NULL);

    if ((in_pFat == NULL) ||
        (in_pFat->stg == NULL) ||
        (in_pFat->ulFatLength > 0 && in_pFat->alFat == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    /* Early exit if the MiniFAT has no entries */
    if (in_pFat->ulFatLength == 0)
    {
        return SSTG_OK;
    }

    /* Mark all the free sectors as being free, thereby unchaining the free 
     * sector chain */
    iRet = fatMarkFreeChain(in_pFat);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* Do some calculations to figure out where the small fat will go */
    cSectorsWritten = rootStorageGetFat(in_pFat->stg)->ulFatLength;
    pHdr = rootStorageGetHeader(in_pFat->stg);
    wSectorShift = headerGetSectorShift(pHdr);
    cbSector = (1 << wSectorShift);
    cMiniFATSectors = calcReqdSectors (in_pFat->ulFatLength * sizeof(UINT4), cbSector);

    /* Actually write the MiniFAT */
    iRet = fatWriteFat(in_pFat, cSectorsWritten);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* Write the sectors used by the MiniFAT to the regular FAT */
    for (i = 1; i < cMiniFATSectors; i++)
    {
        iRet = fatUpdate(rootStorageGetFat(in_pFat->stg),
                         cSectorsWritten + i - 1,
                         cSectorsWritten + i);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    iRet = fatTerminateChain(rootStorageGetFat(in_pFat->stg),
                             cSectorsWritten + cMiniFATSectors - 1);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* Update the header with the MiniFAT details */
    headerSetSmallFatStartSector(pHdr, cSectorsWritten);
    headerSetSmallFatSize(pHdr, cMiniFATSectors);

    return iRet;
}


/*============================================================================
  Description:  Attempts to merge any free sectors from the end of the small
                FAT into the free list of the regular FAT.
  Arguments: 
   in_pMiniFat - pointer to the mini FAT from which to recover free sectors
   in_pRegFat -  pointer to the normal FAT to which to add free sectors
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatRecoverMiniFatFreeSectors(Fat* io_pMiniFat, Fat* io_pRegFat)
{
    UINT2           unRegSectorShift =              0;
    UINT4           ulCur =                         io_pMiniFat->ulFreeStartSector;
    UINT4           ulPrev =                        (UINT4)-1;
    UINT4           ulLastUsedSector =              ulCur - 1;
    UINT4           ulLastFreeBeforeLastUsed =      (UINT4)-1;
    DirectoryEntry* pRootDir =                      NULL;
    Header*         pHdr =                          NULL;
    TOC*            pTOC =                          NULL;
    UINT2           unSmallSectorShift =            0;
    UINT4           ulSmallSectorCutoff =           0;
    UINT4           ulSmallSectorsPerRegSector =    0;
    UINT4           ulFirstMiniSectorToFree =       0;
    UINT4           ulLastMiniSectorToFree =        0;
    UINT4           ulCurRegSector =                0;
    UINT4           ulPrevRegSector =               0;
    UINT4           ulSectorIndex =                 0;
    UINT4           i =                             0;

#ifdef DEBUG
    assertFatFreeChainValid(io_pMiniFat);
    assertFatFreeChainValid(io_pRegFat);
#endif /* DEBUG */

    if (io_pMiniFat->ulFreeStartSector == END_OF_CHAIN)
    {
        /* Early exit if mini-fat is empty */
        return SSTG_OK;
    }

    pHdr = rootStorageGetHeader(io_pMiniFat->stg);
    pTOC = rootStorageGetTOC(io_pMiniFat->stg);
    pRootDir = tocGetEntryAtIndex(pTOC, 0);
    unRegSectorShift = headerGetSectorShift(pHdr);
    unSmallSectorShift = headerGetSmallSectorShift(pHdr);
    ulSmallSectorCutoff = headerGetSmallStreamCutoff(pHdr);
    ASSERT(unRegSectorShift >= unSmallSectorShift);
    ulSmallSectorsPerRegSector = (1 << (unRegSectorShift - unSmallSectorShift));

    /* Find the last used sector in the mini FAT; also, see if free sectors
     * adjoin the end of the mini FAT */
    for (;ulCur != END_OF_CHAIN;)
    {
        ulPrev = ulCur;
        ulCur = io_pMiniFat->alFat[ulCur];
        ASSERT(ulCur > ulPrev);
        if ((ulCur != END_OF_CHAIN) && (ulCur - ulPrev > 1))
        {
            ulLastFreeBeforeLastUsed = ulPrev;
            ulLastUsedSector = ulCur - 1;
        }
    }
    
    /* If free sectors adjoin the end of the mini FAT */
    if ((ulPrev == io_pMiniFat->ulFatLength - 1))
    {

        /* If the mini FAT is completely empty */
        if (ulLastUsedSector == (UINT4)-1)
        {
            rootStorageSetLastMiniStreamSector(io_pMiniFat->stg, (UINT4)-1);
            fatFreeChain(io_pRegFat, directoryGetStartSector(pRootDir));
            directorySetStartSector(pRootDir, END_OF_CHAIN);
            directorySetSize(pRootDir, 0);
            headerSetSmallFatSize(pHdr, 0);
            headerSetSmallFatStartSector(pHdr, (UINT4)-1);
            io_pMiniFat->ulFatLength = 0;
            io_pMiniFat->ulFreeStartSector = END_OF_CHAIN;
        }
        else /* The mini FAT is not completely empty, but can be trimmed */
        {
            ulFirstMiniSectorToFree = (ulLastUsedSector + 
                                       ulSmallSectorsPerRegSector) / 
                                      ulSmallSectorsPerRegSector * 
                                      ulSmallSectorsPerRegSector;
            ulLastMiniSectorToFree = ulPrev;

            /* If there isn't a full sector to merge, quit early */
            if (ulFirstMiniSectorToFree > ulLastMiniSectorToFree)
            {
                return SSTG_OK;
            }

            ulSectorIndex = (ulFirstMiniSectorToFree / 
                            ulSmallSectorsPerRegSector);
            ASSERT(ulSectorIndex > 0);

            ulPrevRegSector = END_OF_CHAIN;
            ulCurRegSector = directoryGetStartSector(pRootDir);
            for (i = 0; i < ulSectorIndex; i++)
            {
                ulPrevRegSector = ulCurRegSector;
                ulCurRegSector = io_pRegFat->alFat[ulCurRegSector];
            }
            ASSERT(ulCurRegSector != END_OF_CHAIN);

            fatTerminateChain(io_pRegFat, ulPrevRegSector);
            fatFreeChain(io_pRegFat, ulCurRegSector);
            rootStorageSetLastMiniStreamSector(io_pRegFat->stg, ulPrevRegSector);
            directorySetSize(pRootDir, ulSectorIndex * (1 << unRegSectorShift));
            io_pMiniFat->ulFatLength -= (ulLastMiniSectorToFree - 
                                         ulFirstMiniSectorToFree + 1);
            if (ulLastUsedSector == ulFirstMiniSectorToFree - 1)
            {
                if ((ulLastFreeBeforeLastUsed == END_OF_CHAIN) || 
                    (ulLastFreeBeforeLastUsed == (UINT4)-1))
                {
                    io_pMiniFat->ulFreeStartSector = END_OF_CHAIN;
                }
                else
                {
                    fatTerminateChain(io_pMiniFat, ulLastFreeBeforeLastUsed);
                }
            }
            else
            {
                fatTerminateChain(io_pMiniFat, ulFirstMiniSectorToFree - 1);
            }
        }
    }

#ifdef DEBUG
    assertFatFreeChainValid(io_pMiniFat);
    assertFatFreeChainValid(io_pRegFat);
#endif /* DEBUG */

    return SSTG_OK;
}


/*============================================================================
  Description:  Writes the entire FAT and DIF from memory.
                All streams and storages must be flushed before this function
                is called.  Also, the FAT passed in must have a correct value
                in its ulFatLength field.
  Arguments: 
   in_pFat - pointer to the FAT to write
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatWriteFatAndXFat (Fat* in_pFat)
{
    UINT4*      pbBuf =                 NULL;
    UINT4       sectorsPerFATSector =   0;
    UINT4       cbSector =              0;
    UINT4       i =                     0;
    UINT4       j =                     0;
    SINT4       iRet =                  SSTG_OK;
    UINT4       iSectorWritten =        0;
    UINT4       cFATSectors =           0;
    UINT4       cDIFSectors =           0;
    UINT4       cFATSectorDelta =       0;
    UINT4       cDIFSectorDelta =       0;
    UINT4       ulDIFSector =           0;
    UINT4       cFATSectorsInDIF =      0;
    UINT4       ulFatFinalLength =      0;
    Header*     pHdr =                  NULL;
    UINT2       wSectorShift =          0;

    /* Check arguments */
    ASSERT (in_pFat != NULL);
    ASSERT (in_pFat->stg != NULL);
    ASSERT (in_pFat->ulFatLength == 0 || in_pFat->alFat != NULL);
    if ((in_pFat == NULL) ||
            (in_pFat->stg == NULL) ||
            (in_pFat->ulFatLength > 0 && in_pFat->alFat == NULL))
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (in_pFat->ulFatLength == 0)
    {
        return SSTG_OK;
    }

    /* Calculate some basic information about this structured storage file:
     * sector sizes, etc. */
    pHdr = rootStorageGetHeader(in_pFat->stg);
    wSectorShift = headerGetSectorShift(pHdr);
    cbSector = 1 << wSectorShift;
    sectorsPerFATSector = cbSector / sizeof (UINT4);

    /* Calculate the space req'd for the FAT and DIF (XFat).  This might be
     * recursive.  Specifically: we start by calculating the size of the FAT and 
     * DIF based on a "snapshot" of what is in the file already.  When we calculate
     * the size of the DIF, we might find that this adds some extra sectors to the
     * file.  This would then imply that the FAT would have to grow.  However, 
     * growing the FAT could mean that we cross a sector boundary in the FAT, 
     * thereby increasing the size of the DIF again. So this is a fairly 
     * complicated calculation. */
    cFATSectorDelta = calcReqdSectors(
                          in_pFat->ulFatLength * sizeof(UINT4), cbSector);
    cFATSectorsInDIF = max(cFATSectors, HEADER_FAT_SECTORS) - HEADER_FAT_SECTORS;
    cDIFSectorDelta = calcReqdSectors(
                          (max(cFATSectors, HEADER_FAT_SECTORS) - HEADER_FAT_SECTORS) * sizeof(UINT4), cbSector - sizeof(UINT4));
    while(cFATSectorDelta != 0 || cDIFSectorDelta != 0)
    {
        cFATSectors += cFATSectorDelta;
        cDIFSectors += cDIFSectorDelta;
        in_pFat->ulFatLength += cFATSectorDelta;
        in_pFat->ulFatLength += cDIFSectorDelta;

        cFATSectorDelta = calcReqdSectors(
                              in_pFat->ulFatLength * sizeof(UINT4), cbSector) - cFATSectors;
        cFATSectorsInDIF = max(cFATSectors, HEADER_FAT_SECTORS) - HEADER_FAT_SECTORS;
        cDIFSectorDelta = calcReqdSectors(
                              cFATSectorsInDIF * sizeof(UINT4), (cbSector - sizeof(UINT4))) - cDIFSectors;
    }

    ulFatFinalLength = in_pFat->ulFatLength;

    /* Allocate some space for the DIF in memory */
    ASSERT(in_pFat->alFatSectors == NULL);
    in_pFat->alFatSectors = (UINT4*)malloc (cFATSectors * sizeof(UINT4));
    if (in_pFat->alFatSectors == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( in_pFat->alFatSectors, 0, cFATSectors * sizeof(UINT4));


    /* Write the sectors used by the DIF into the FAT */
    for (i = 0; i < cDIFSectors; i++)
    {
        iRet = fatSetEntry(in_pFat, ulFatFinalLength - i - 1, USED_BY_XFAT);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    /* Calculate the information that should go in the DIF, and write the sectors
     * used by the FAT in the FAT itself */
    for (i = 0; i < cFATSectors; i++)
    {
        in_pFat->alFatSectors[i] = ulFatFinalLength - cDIFSectors - cFATSectors + i;
        iRet = fatSetEntry(in_pFat, ulFatFinalLength - cDIFSectors - cFATSectors + i, USED_BY_FAT);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }
    in_pFat->ulNumSectors = cFATSectors;

    /* Write the FAT */
    iRet = fatWriteFat(in_pFat, ulFatFinalLength - cDIFSectors - cFATSectors);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* Create a temporary buffer for the DIF */
    pbBuf = (UINT4*)malloc(cbSector);
    if (pbBuf == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }

    memset( pbBuf, 0, cbSector );


    ulDIFSector = 0;
    /* Write the actual DIF */
    for (i = HEADER_FAT_SECTORS; i < cFATSectors && iRet == SSTG_OK; )
    {
        for (j = 0; (j < sectorsPerFATSector - 1) && i < cFATSectors; j++, i++)
        {
            leWriteUnsignedLong ((in_pFat->alFatSectors)[i], &pbBuf[j]);
        }
        /* Chain to the next sector */
        leWriteUnsignedLong(ulFatFinalLength - cDIFSectors + ulDIFSector + 1,
                            &pbBuf[sectorsPerFATSector - 1]);

        iSectorWritten = ulFatFinalLength - cDIFSectors + ulDIFSector;
        iRet = rootStorageRewriteSector (
                   in_pFat->stg, (BYTE*)pbBuf, iSectorWritten);
        if (iRet != SSTG_OK)
        {
            free (pbBuf);
            return iRet;
        }
        ulDIFSector++;
    }

    /* Free the buffer */
    free(pbBuf);

    /* Update the header with the final FAT, DIF information */
    headerSetFatSize(pHdr, cFATSectors);
    headerSetXFatSize(pHdr, cDIFSectors);
    if (cDIFSectors > 0)
    {
        headerSetXFatStartSector(pHdr, ulFatFinalLength - cDIFSectors);
    }
    else
    {
        headerSetXFatStartSector(pHdr, (UINT4)-1);
    }

    return iRet;
}


/*============================================================================
  Description:  Merges two chains in the FAT.  
                The first must be in sorted order, meaning that i < alFat[i] < 
                for all i in the chain.  
                The second chain need not be sorted.  
                The resulting chain will be sorted.

                This algorithm is O(n^2) in the worst case.  It is assumed that
                in the typical case, the second list will be "mostly" sorted
                and this algorithm will cost more like O(n).

  Arguments: 
   io_pFat -               pointer to the FAT
   in_ulStart1 -           index of the start sector of the first chain.  This 
                           chain must be sorted.
   in_ulStart2 -           index of the start sector of the second chain.  
                           This chain does not have to be sorted.
   out_pulNewStartSector - on success, will point to the start sector of the
                           new, merged, chain.

  Return:       
   Status code
  ==========================================================================*/
SINT4
fatOrderedMergeChain(Fat* io_pFat,
                     UINT4 in_ulStart1,
                     UINT4 in_ulStart2,
                     UINT4* out_pulNewStartSector)
{
    UINT4 ulArray1Start = in_ulStart1;
    UINT4 ulArray1Cur =   in_ulStart1;
    UINT4 ulArray1Prev =  END_OF_CHAIN;
    UINT4 ulArray2Cur =   in_ulStart2;
    UINT4 ulTmp1 =        0;
    UINT4 ulTmp2 =        0;

    ASSERT(ulArray2Cur != (UINT4)-1);

    while(ulArray2Cur != END_OF_CHAIN)
    {
        ASSERT((ulArray2Cur == END_OF_CHAIN) || 
               (ulArray2Cur < io_pFat->ulFatLength));
        ASSERT((ulArray1Cur == END_OF_CHAIN) || 
               (ulArray1Cur < io_pFat->ulFatLength));

        /* If this fails then chain 1 was not sorted */
        ASSERT((ulArray1Prev == END_OF_CHAIN) || (ulArray1Prev < ulArray1Cur));

        /* Out of order?  Then reset pointers into chain 1. */
        if ((ulArray1Prev != END_OF_CHAIN) && (ulArray2Cur < ulArray1Prev))
        {
            ulArray1Cur = ulArray1Start;
            ulArray1Prev = END_OF_CHAIN;
        }

        ASSERT((ulArray1Prev == END_OF_CHAIN) || (ulArray1Prev < ulArray2Cur));

        /* Move indices into first chain until prev1 < cur2 < cur1 */
        while (ulArray2Cur > ulArray1Cur)
        {
            ulArray1Prev = ulArray1Cur;
            ulArray1Cur = io_pFat->alFat[ulArray1Cur];
        }

        ASSERT(((ulArray1Prev == END_OF_CHAIN) ||
                (ulArray1Prev < ulArray2Cur)) &&
               (ulArray2Cur  < ulArray1Cur));

        /* Insert the sector ulArray2Cur into chain 1 */
        if (ulArray1Prev == END_OF_CHAIN)
        {
            ulArray1Start = ulArray2Cur;
        }
        else
        {
            io_pFat->alFat[ulArray1Prev] = ulArray2Cur;
        }
        ulTmp2 = io_pFat->alFat[ulArray2Cur];
        ulTmp1 = ulArray2Cur;
        io_pFat->alFat[ulArray2Cur] = ulArray1Cur;
        ulArray2Cur = ulTmp2;
        ulArray1Prev = ulTmp1;

        ASSERT((ulArray1Prev == END_OF_CHAIN) || 
               (ulArray1Prev < io_pFat->ulFatLength));
        ASSERT((ulArray1Cur == END_OF_CHAIN) || 
               (ulArray1Cur < io_pFat->ulFatLength));
        ASSERT((ulArray2Cur == END_OF_CHAIN) || 
               (ulArray2Cur < io_pFat->ulFatLength));
    }

    *out_pulNewStartSector = ulArray1Start;

    return SSTG_OK;
}


/*============================================================================
  Description:  Compares two sectors.  Required in order to use clib's qsort
                function on an array of sectors.
  Arguments: 
   in_pSector1 - pointer to UINT4 sector index
   in_pSector2 - pointer to UINT4 sector index
  Return:       
   -1, 0, or 1, as per qsort's compare function specification
  ==========================================================================*/
int fatCompareSectors(const void* in_pSector1, const void* in_pSector2)
{
    UINT4 ulSector1 = *(UINT4*)in_pSector1;
    UINT4 ulSector2 = *(UINT4*)in_pSector2;

    if (ulSector1 < ulSector2)
    {
        return -1;
    }
    else if (ulSector1 > ulSector2)
    {
        return 1;
    }
    return 0;
}


/*============================================================================
  Description:  Adds an array of sectors to the free chain.  Frees the array
                and sets its count to 0 at the end.

  Arguments: 
   io_pFat -           pointer to the FAT
   io_ppSectors -      array of sectors to add to the free chain.  This array
                       will be freed on successful return.
   io_pulSectorCount - number of sectors in the array.  Will be set to zero
                       on success.
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatAddFreeSectors(Fat* io_pFat, UINT4** io_ppSectors, UINT4* io_pulSectorCount)
{
    UINT4  ulNext1 =       0;
    UINT4  ulNext2 =       0;
    UINT4  ulNext =        0;
    UINT4* pulCur =        0;
    SINT4  iRet =          0;
    UINT4  i =             0;
    UINT4  j =             0;
    UINT4  ulMaxSector =   0;
    UINT4  ulArrayIndex  = 0;
    UINT4* pSectors =      *io_ppSectors;
    UINT4  ulSectorCount = *io_pulSectorCount;
    
    ASSERT(io_pFat != NULL);

    /* Early exit in the case of an empty list */
    if ((*io_pulSectorCount <= 0) || 
        (io_ppSectors == NULL) || 
        (*io_ppSectors == NULL))
    {
        if ((io_ppSectors != NULL) && (*io_ppSectors != NULL))
        {
            free(*io_ppSectors);
            *io_ppSectors = NULL;
        }
        *io_ppSectors = 0;
        return SSTG_OK;
    }

    /* Presort the array.  This will be faster than trying to sort while 
     * merging.*/
    qsort(*io_ppSectors, ulSectorCount, sizeof(UINT4), &fatCompareSectors);

    /* Make sure that the sectors we are about to make free exist! */
    ulMaxSector = (*io_ppSectors)[ulSectorCount - 1];
    iRet = fatEnsureMinSize(io_pFat, ulMaxSector + 1);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }
    if (io_pFat->ulFatLength < ulMaxSector + 1)
    {
        io_pFat->ulFatLength = ulMaxSector + 1;
    }

    if (io_pFat->ulFreeStartSector == END_OF_CHAIN)
    {
        /* There are currrently no free sectors.  Create a new chain. */
        io_pFat->ulFreeStartSector = pSectors[0];
        for (i = 0; i < ulSectorCount - 1; i++)
        {
            io_pFat->alFat[pSectors[i]] = pSectors[i + 1];
        }
        io_pFat->alFat[pSectors[ulSectorCount - 1]] = END_OF_CHAIN;
    }
    else
    {
        /* Merge the new array into the existing chain. */
        pulCur = &(io_pFat->ulFreeStartSector);
        ulNext1 = io_pFat->ulFreeStartSector;
        ulArrayIndex = 0;
        ulNext2 = pSectors[ulArrayIndex];
        for (;;)
        {
            if (ulNext1 < ulNext2)
            {
                ulNext = ulNext1;
                /* Continue down the chain */
                ulNext1 = io_pFat->alFat[ulNext1];
            }
            else
            {
                ulNext = ulNext2;
                ulArrayIndex++;
                if (ulArrayIndex < ulSectorCount)
                {
                    ulNext2 = pSectors[ulArrayIndex];
                }
            }

            *pulCur = ulNext;
            pulCur = &(io_pFat->alFat[ulNext]);

            /* Check for two termination conditions */
            if (ulNext1 == END_OF_CHAIN)
            {
                /* Reached the end of the existing chain.  Concatenate
                 * the remaining array members. */
                *pulCur = pSectors[ulArrayIndex];
                for (j = ulArrayIndex; j < ulSectorCount - 1; j++)
                {
                    io_pFat->alFat[pSectors[j]] = 
                        pSectors[j + 1];
                }
                io_pFat->alFat[pSectors[ulSectorCount - 1]] = END_OF_CHAIN;
                break; /* out of the outermost for loop */
            } 
            else if (ulArrayIndex >= ulSectorCount)
            {
                /* Link the new current sector to the rest of the existing free
                 * sector chain and exit */
                *pulCur = ulNext1;
                break; /* out of the outermost for loop */
            }
        }
    }

    /* Free the chain, it's no longer valid */
    free(*io_ppSectors);
    *io_ppSectors = NULL;
    *io_pulSectorCount = 0;

    return iRet;
}


/*============================================================================
  Description:  Removes a sector from the free chain and returns its index.

  Arguments: 
   io_pFat -           pointer to the FAT
   out_pulFreeSector - on success, variable pointed to by this pointer will 
                       receive the sector index removed from the chain.
  Return:       
   Status code; SSTG_ILLEGAL_CALL if the free chain is empty, SSTG_OK on 
   success
  ==========================================================================*/
SINT4
fatPopFreeSector(Fat* io_pFat, UINT4* out_pulFreeSector)
{
    SINT4 iRet =         SSTG_ERROR_ILLEGAL_CALL;
    UINT4 ulFreeSector = (UINT4)-1;

    ASSERT(io_pFat != NULL);
    if (io_pFat->ulFreeStartSector != END_OF_CHAIN)
    {
        ulFreeSector = io_pFat->ulFreeStartSector;
        io_pFat->ulFreeStartSector = io_pFat->alFat[ulFreeSector];
        ASSERT((io_pFat->ulFreeStartSector == END_OF_CHAIN) ||
               (io_pFat->ulFreeStartSector < io_pFat->ulFatLength));

        iRet = SSTG_OK;
    }

    *out_pulFreeSector = ulFreeSector;

    return iRet;
}


/*============================================================================
  Description:  Returns the next available sector in the FAT, either from the
                free chain or from the end of the FAT

  Arguments: 
   io_pFat -           pointer to the FAT
   out_pulFreeSector - on success, variable pointed to by this pointer will 
                       receive the index of the next sector available
                       in the FAT.
   in_bIsMetadata - flag specifying whether this is a "metadata" like the TOC
                    or like the FAT.
                    When writing streams, this flag should be SSRW_FALSE;
                    when writing metadata, it should be set to SSRW_TRUE.
  Return:       
   Status code
  ==========================================================================*/
SINT4 
fatNextFreeSector(Fat*    io_pFat, 
                  UINT4*  out_pulSectorIndex, 
                  boolean in_bIsMetadata)
{
    SINT4 iRet =     SSTG_OK;

    /* Determine where to write this sector: in the middle of the file, or at
     * the end */
    iRet = fatPopFreeSector(io_pFat, out_pulSectorIndex);
    if (iRet != SSTG_OK)
    {
        *out_pulSectorIndex = io_pFat->ulFatLength;
        iRet = SSTG_OK;
    }

    /* Check to make sure the file isn't going to blow past its addressing 
     * limit. 
     * If we are trying to write out metadata like the FAT or TOC, though, 
     * try anyway.  If we can't write those out then the file will be corrupt 
     * in any case.
     */
    if ((!in_bIsMetadata) && (*out_pulSectorIndex > FAT_SECT_MAX))
    {
        *out_pulSectorIndex = FREE_SECTOR;
        iRet = SSTG_ERROR_ILLEGAL_ACCESS;
    }

    return iRet;
}


/*============================================================================
  Description:  The structured storage library chains free sectors as if they
                represented a stream in the FAT.  This is so that the free 
                sectors can be efficiently managed in read/write mode.  
                However, the structured storage file format specifies that all
                free sectors be marked with the special value FREE_SECTOR.
                Therefore, before the structured storage file is closed when
                in read/write mode, this function should be called to set all
                the sectors in the free chain to FREE_SECTOR.
  Arguments: 
   io_pFat -           pointer to the FAT to chain
  Return:       
   Status code
  ==========================================================================*/
SINT4 
fatMarkFreeChain(Fat* io_pFat)
{
    UINT4 ulCur = 0;
    UINT4 ulTmp = END_OF_CHAIN;

    ASSERT(io_pFat != NULL);

    for (ulCur = io_pFat->ulFreeStartSector; 
         ulCur != END_OF_CHAIN && ulCur < io_pFat->ulFatLength; )
    {
        ulTmp = io_pFat->alFat[ulCur];
        io_pFat->alFat[ulCur] = FREE_SECTOR;
        ulCur = ulTmp;
    }

    io_pFat->ulFreeStartSector = END_OF_CHAIN;

    return SSTG_OK;
}


/*============================================================================
  Description:  Adds the sectors used by a given FAT structure to the free 
                sector chain of "this" fat.
                io_pFat is allowed to be the same as io_pFatToFree.
  Arguments: 
   io_pFat -       pointer to the FAT to which to add the free sectors
   io_pFatToFree - pointer to the fat whose sectors should be added to the 
                   free sector chain
  Return:       
   Status code
  ==========================================================================*/
SINT4 
fatAddFatToFreeSectors(Fat* io_pFat, Fat* io_pFatToFree)
{
    SINT4 iRet = SSTG_OK;

    ASSERT(io_pFat != NULL);
    ASSERT(io_pFatToFree != NULL);

    iRet = fatAddFreeSectors(io_pFat,
                             &(io_pFatToFree->alFatSectors),
                             &(io_pFatToFree->ulNumSectors));
#ifdef DEBUG
    assertFatFreeChainValid(io_pFat);
#endif /* DEBUG */

    return iRet;
}


/*============================================================================
  Description:  Adds a chain of sectors to the ordered free chain.
  Arguments: 
   io_pFat -    pointer to the FAT to update
   in_ulStart - index of the first sector in the chain to free
  Return:       
   Status code
  ==========================================================================*/
SINT4 
fatFreeChain(Fat* io_pFat, UINT4 in_ulStart)
{
    SINT4 iRet = SSTG_OK;

    if (in_ulStart == END_OF_CHAIN || in_ulStart == (UINT4)-1)
    {
        return SSTG_OK;
    }

    iRet = fatOrderedMergeChain(io_pFat,
                                io_pFat->ulFreeStartSector,
                                in_ulStart,
                                &(io_pFat->ulFreeStartSector));
#ifdef DEBUG
    assertFatFreeChainValid(io_pFat);
#endif /* DEBUG */

    return iRet;
}


/*============================================================================
  Description:  Trims any free sectors off the end of the free chain if they
                are at the end of the file.
                Returns the new length of the file, in sectors.
                The free chain must be sorted, as usual.
  Arguments: 
   io_pFat -       pointer to the FAT to adjust
   out_plNewSize - on success, this will point to the number of sectors still
                   tracked by the FAT.
  Return:       
   Status code
  ==========================================================================*/
SINT4 
fatTrimFreeSectorsAtEnd(Fat* io_pFat, UINT4* out_pulNewSize)
{
    UINT4 ulCur =                       0;
    UINT4 ulPrev =                      END_OF_CHAIN;
    UINT4 ulLastFreeBeforeLastUsed =    END_OF_CHAIN;
    UINT4 ulLastUsed =                  io_pFat->ulFreeStartSector - 1;

    ulCur = io_pFat->ulFreeStartSector;
    while(ulCur != END_OF_CHAIN)
    {
        ulPrev = ulCur;
        ulCur = io_pFat->alFat[ulCur];
        if ((ulCur != END_OF_CHAIN) && (ulCur > ulPrev + 1))
        {
            ulLastFreeBeforeLastUsed = ulPrev;
            ulLastUsed = ulCur - 1;
        }
    }

    if ((ulPrev == (io_pFat->ulFatLength - 1)) && 
        (ulLastUsed < (io_pFat->ulFatLength - 1)))
    {
        if (ulLastFreeBeforeLastUsed == END_OF_CHAIN)
        {
            io_pFat->ulFreeStartSector = END_OF_CHAIN;
        }
        else
        {
            io_pFat->alFat[ulLastFreeBeforeLastUsed] = END_OF_CHAIN;
        }
        io_pFat->ulFatLength = ulLastUsed + 1;
    }

    *out_pulNewSize = io_pFat->ulFatLength;

#ifdef DEBUG
    assertFatFreeChainValid(io_pFat);
#endif /* DEBUG */

    return SSTG_OK;
}


#ifdef DEBUG

#define FAT_BLANK_VALUE 0xfffffffb

/*============================================================================
  Description:  Blanks one chain from a FAT array, setting all sectors
                to the value FAT_BLANK_VALUE.
  Arguments:
   io_pulFat -        FAT sector array
   in_ulFatLen -      length of the FAT sector array
   in_ulStartSector - start sector of the chain to blank
  Return:       
   Status code
  ==========================================================================*/
SINT4
fatBlankChain(UINT4* io_pulFat, UINT4 in_ulFatLen, UINT4 in_ulStartSector)
{
    UINT4 ulCurSector = 0;
    UINT4 ulNextSector = in_ulStartSector;

    do
    {
        ASSERT(ulNextSector < in_ulFatLen);
        ulCurSector = ulNextSector;
        ulNextSector = io_pulFat[ulCurSector];
        ASSERT(ulNextSector != ulCurSector);
        ASSERT(ulNextSector != FAT_BLANK_VALUE);
        io_pulFat[ulCurSector] = FAT_BLANK_VALUE;
    }
    while(ulNextSector != END_OF_CHAIN);

    return SSTG_OK;
}

/*============================================================================
  Description:  Scans the FAT and small FAT for sectors that are not marked
                as being free, and are not referred to by any stream.
                ASSERTs if there are any such unaddressed sectors.
  Arguments:
   io_pFat - regular FAT to examine
   io_pSmallFat - small FAT to examine
  Return:       
   Status code
  ==========================================================================*/
SINT4
assertNoUntrackedFatSectors(Fat* io_pFat,
                            Fat* io_pSmallFat,
                            TOC* in_pTOC)
{
    SINT4  iRet =                 SSTG_OK;
    UINT4  i =                    0; 
    UINT4* pulFatCopy =           NULL;
    UINT4* pulSmallFatCopy =      NULL;
    UINT4  ulLeakedSectors =      0;
    UINT4  ulLeakedSmallSectors = 0;

    pulFatCopy = calloc(sizeof(UINT4), io_pFat->ulFatLength);
    if (pulFatCopy == NULL)
    {
        return SSTG_ERROR_MEMORY;
    }
    pulSmallFatCopy = calloc(sizeof(UINT4), io_pSmallFat->ulFatLength);
    if (pulSmallFatCopy == NULL)
    {
        free(pulFatCopy);
        return SSTG_ERROR_MEMORY;
    }

    memcpy(pulFatCopy, io_pFat->alFat, ((io_pFat->ulFatLength) * sizeof(UINT4)));
    memcpy(pulSmallFatCopy, io_pSmallFat->alFat, ((io_pSmallFat->ulFatLength) * sizeof(UINT4)));

    /* Blank the chains of all the streams in the directory */
    iRet = tocBlankUsedSectors(
            in_pTOC,
            pulFatCopy, 
            io_pFat->ulFatLength, 
            pulSmallFatCopy,
            io_pSmallFat->ulFatLength);
    if (iRet != SSTG_OK)
    {
        return iRet;
    }

    /* Blank the free sector chains as well */
    if (io_pFat->ulFreeStartSector != END_OF_CHAIN)
    {
        iRet = fatBlankChain(pulFatCopy, 
            io_pFat->ulFatLength, 
            io_pFat->ulFreeStartSector);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    if (io_pSmallFat->ulFreeStartSector != END_OF_CHAIN)
    {
        iRet = fatBlankChain(pulSmallFatCopy, 
            io_pSmallFat->ulFatLength, 
            io_pSmallFat->ulFreeStartSector);
        if (iRet != SSTG_OK)
        {
            return iRet;
        }
    }

    /* Now, count the number of sectors whose value in the FAT copy is not 
     * FAT_BLANK_VALUE.  These will be leaked sectors.  There should be none 
     * of these.
     */
    for (i = 0; i < io_pFat->ulFatLength; i++)
    {
        if(pulFatCopy[i] != FAT_BLANK_VALUE)
        {
            ulLeakedSectors++;
        }
    }
    ASSERT(ulLeakedSectors == 0);

    for (i = 0; i < io_pSmallFat->ulFatLength; i++)
    {
        if (pulSmallFatCopy[i] != FAT_BLANK_VALUE)
        {
            ulLeakedSmallSectors++;
        }
    }
    ASSERT(ulLeakedSmallSectors == 0);

    free(pulFatCopy);
    free(pulSmallFatCopy);

    return SSTG_OK;
}


/*============================================================================
  Description:  Asserts some invariants about the free sector chain.
                This function should only be run in DEBUG mode.
  Return:       
   Status code
  ==========================================================================*/
SINT4 
assertFatFreeChainValid(Fat* io_pFat)
{
    UINT4 ulCur = io_pFat->ulFreeStartSector;

    while (ulCur != END_OF_CHAIN)
    {
        ASSERT(ulCur < io_pFat->ulFatLength);
        ulCur = io_pFat->alFat[ulCur];
        ASSERT(ulCur != USED_BY_FAT);
        ASSERT(ulCur != USED_BY_XFAT);
    }
    return SSTG_OK;
}


/*============================================================================
  Description:  Runs some internal tests on the Fat abstract data type's 
                functions.
                This function should only be run in DEBUG mode.
  Return:       
   Status code
  ==========================================================================*/
SINT4 
fatSelfTest(void)
{
    SINT4 iRet =                0;
    UINT4 i =                   0;
    Fat   fat;
                         
    /* Test 1 */
    /* FAT contains two 10-sector chains, merge them. */
    UINT4 testFat1[] = {  2,  3,  4,  5,  6,  7,  8,  9,           10,           11,
                         12, 13, 14, 15, 16, 17, 18, 19, END_OF_CHAIN, END_OF_CHAIN };
    UINT4 ulNewStartSector =    0;

    /* Test 2 */
    /* FAT contains two 10-sector chains. Mark one of them as being free, then 
     * the other. */
    UINT4 testFat2[] = 
        {  2,  3,  4,  5,  6,  7,  8,  9,           10,           11,
          12, 13, 14, 15, 16, 17, 18, 19, END_OF_CHAIN, END_OF_CHAIN };
    UINT4 testFat2AfterStepA[] = 
        {  2,  3,  4,  5,  6,  7,  8,  9,           10,           11,
          12, 13, 14, 15, 16, 17, 18, 19, END_OF_CHAIN, END_OF_CHAIN };
    UINT4  sectorList2a[] = { 18, 14, 16, 12, 10,  8,  6,  4,  2, 0 };
    UINT4  sectorList2b[] = {  1,  7,  5,  3,  9, 11, 13, 15, 19, 17 };
    UINT4* sectorList2aCopy =   NULL;
    UINT4* sectorList2bCopy =   NULL;
    UINT4 sectorList2aSize =    sizeof(sectorList2a) / sizeof(UINT4);
    UINT4 sectorList2bSize =    sizeof(sectorList2b) / sizeof(UINT4);

    /* Test 3 */
    /* FAT contains two 10 sector chains, merge them.  The second one is not
     * sorted. */
    UINT4 testFat3[] = {  9, END_OF_CHAIN,  1,  2,            3,  
                          4,            5,  6,  7,            8,
                         11,           12, 13, 14,           15, 
                         16,           17, 18, 19, END_OF_CHAIN };

    /* Test 4.  "Used by FAT" sectors follow regular free chain in Fat.  Merge 
     * the FAT into the free chain. */
    UINT4 testFat4[] = 
        { 1, 2, 3, 4, 5, 6, 7, 8, 9, END_OF_CHAIN, USED_BY_FAT, USED_BY_FAT, 
          USED_BY_FAT, USED_BY_FAT, USED_BY_FAT, USED_BY_FAT, USED_BY_XFAT, 
          USED_BY_XFAT, USED_BY_XFAT, USED_BY_XFAT };
    UINT4  sectorList4[] =      { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    UINT4* sectorList4Copy =    NULL;
    UINT4 sectorList4Size =     sizeof(sectorList4) / sizeof(UINT4);

    /* Test 1: merge two chains into one.  The straighforward case of two
     * strictly increasing, interleaved chains */
    fat.alFat = testFat1;
    fat.ulFatLength = 20;
    fat.ulFatReserved = 20;
    ulNewStartSector = 5;
    iRet = fatOrderedMergeChain(&fat, 0, 1, &ulNewStartSector);
    ASSERT(iRet == SSTG_OK);
    ASSERT(ulNewStartSector == 0);
    for (i = 0; i < 19; i++)
    {
        ASSERT(fat.alFat[i] == (i + 1));
    }
    ASSERT(fat.alFat[19] == END_OF_CHAIN);

    /* Test 2a: Start by creating a new free list.*/
    fat.alFat = testFat2;
    fat.ulFatLength = 20;
    fat.ulFatReserved = 20;
    fat.ulFreeStartSector = END_OF_CHAIN;
    sectorList2aCopy = (UINT4*)malloc(sizeof(sectorList2a));
    ASSERT(sectorList2aCopy != NULL);
    memcpy(sectorList2aCopy, sectorList2a, sizeof(sectorList2a));
    iRet = fatAddFreeSectors(&fat, &sectorList2aCopy, &sectorList2aSize);
    ASSERT(iRet == SSTG_OK);
    ASSERT(sectorList2aCopy == NULL);
    ASSERT(sectorList2aSize == 0);
    for (i = 0; i < 19; i++)
    {
        ASSERT(fat.alFat[i] == testFat2AfterStepA[i]);
    }
    ASSERT(fat.ulFreeStartSector == 0);

    /* Test 2b: Now merge the other chain into the free list. */
    sectorList2bCopy = (UINT4*)malloc(sizeof(sectorList2b));
    ASSERT(sectorList2bCopy != NULL);
    memcpy(sectorList2bCopy, sectorList2b, sizeof(sectorList2b));
    iRet = fatAddFreeSectors(&fat, &sectorList2bCopy, &sectorList2bSize);
    ASSERT(sectorList2bCopy == NULL);
    ASSERT(sectorList2bSize == 0);
    for (i = 0; i < 19; i++)
    {
        ASSERT(fat.alFat[i] == (i + 1));
    }
    ASSERT(fat.alFat[19] == END_OF_CHAIN);


    /* Test 3: merge two chains into one.  The second chain precedes the
     * first in the FAT, and is mostly ordered backwards. */
    fat.alFat = testFat3;
    fat.ulFatLength = 20;
    fat.ulFatReserved = 20;
    ulNewStartSector = 5;
    iRet = fatOrderedMergeChain(&fat, 10, 0, &ulNewStartSector);
    ASSERT(iRet == SSTG_OK);
    ASSERT(ulNewStartSector == 0);
    for (i = 0; i < 19; i++)
    {
        ASSERT(fat.alFat[i] == (i + 1));
    }
    ASSERT(fat.alFat[19] == END_OF_CHAIN);


    /* Test 4: Merge "Used by FAT" and "Used by XFAT" sectors into the free list. */
    fat.alFat = testFat4;
    fat.ulFatLength = 20;
    fat.ulFatReserved = 20;
    fat.ulFreeStartSector = 0;
    sectorList4Copy = (UINT4*)malloc(sizeof(sectorList4));
    ASSERT(sectorList4Copy != NULL);
    memcpy(sectorList4Copy, sectorList4, sizeof(sectorList4));
    iRet = fatAddFreeSectors(&fat, &sectorList4Copy, &sectorList4Size);
    ASSERT(iRet == SSTG_OK);
    ASSERT(sectorList4Copy == NULL);
    ASSERT(sectorList4Size == 0);
    for (i = 0; i < 18; i++)
    {
        ASSERT(fat.alFat[i] == i + 1);
    }
    ASSERT(fat.alFat[19] == END_OF_CHAIN);
    ASSERT(fat.ulFreeStartSector == 0);

    return SSTG_OK;
}

#endif /* DEBUG */
