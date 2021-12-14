/************************************************************************
* Fat.h
* Header describing the strcutures hte Fat "object" as it represented 
* INSIDE the program, not inside the compound file
*
* (c) Schema Software Inc., 2001-2004
* 
*************************************************************************

$Revision: 1.4.2.1 $
$Date: 2004/02/26 18:26:00 $
*/
#ifndef _Fat_h
#define _Fat_h

/* forward declarations */
extern _OLESS_API SINT4 fatConstruct (RootStorage* root, Fat** fat, UINT2 in_wSectorSize);
extern _OLESS_API SINT4 fatDestroy (Fat** io_ppFat);
extern _OLESS_API void fatSetFatSectors (Fat* fat, UINT4* sectors,
                              UINT4 count);
extern _OLESS_API SINT4 fatGetFatSectors (Fat* in_fat, UINT4** out_ppSectors,
                               UINT4 *out_pCount);
extern _OLESS_API SINT4 fatReadFat (Fat* in_fat);
extern _OLESS_API SINT4 fatWriteFatAndXFat (Fat* in_fat);
extern _OLESS_API SINT4 fatWriteMiniFat (Fat* in_fat);
extern _OLESS_API SINT4 fatRecoverMiniFatFreeSectors(Fat* io_pMiniFat, Fat* io_pRegFat);
extern _OLESS_API SINT4 fatSetEntry (Fat* io_fat, UINT4 index, UINT4 val);
extern _OLESS_API SINT4 fatUpdate (Fat* io_fat, UINT4 in_dwLastFatIndex, UINT4 in_dwNewFatIndex);
extern _OLESS_API SINT4 fatTerminateChain(Fat* io_pFat, UINT4 in_dwFinalSector);
extern _OLESS_API SINT4 fatSetFreeSector(Fat* io_pFat, UINT4 in_dwSector);
extern _OLESS_API SINT4 fatGetChain (Fat* fat,
                          UINT4 start,
                          UINT4* length,
                          UINT4** out_chain);
extern _OLESS_API SINT4 fatGetChainLastSector (Fat* in_pFat,
                                    UINT4 in_ulStartSector,
                                    UINT4* out_pulLastSector);
extern _OLESS_API UINT4 fatGetLength (Fat* in_pFat);
extern _OLESS_API UINT4 calcReqdSectors(UINT8 cbData, UINT4 cbSector);
extern _OLESS_API SINT4 fatOrderedMergeChain(Fat* io_pFat,
                                  UINT4 in_ulStart1,
                                  UINT4 in_ulStart2,
                                  UINT4* out_pulNewStartSector);
extern _OLESS_API SINT4 fatAddFreeSectors(Fat* io_pFat, 
                               UINT4** io_ppSectors, 
                               UINT4* io_pulSectorCount);
extern _OLESS_API SINT4 fatPopFreeSector(Fat* io_pFat, UINT4* out_pulFreeSector);
extern _OLESS_API SINT4 fatNextFreeSector(Fat* io_pFat, 
                               UINT4* out_pulSectorIndex,
                               boolean in_bIsMetadata);
extern _OLESS_API SINT4 fatMarkFreeChain(Fat* io_pFat);
extern _OLESS_API SINT4 fatAddFatToFreeSectors(Fat* io_pFat, Fat* io_pFatToFree);
extern _OLESS_API SINT4 fatFreeChain(Fat* io_pFat, UINT4 in_ulStart);
extern _OLESS_API SINT4 fatTrimFreeSectorsAtEnd(Fat* io_pFat, UINT4* out_plNewSize);

#ifdef DEBUG
extern _OLESS_API SINT4 fatBlankChain(UINT4* io_pulFat,
                           UINT4 in_ulFatLen,
                           UINT4 in_ulStartSector);
extern _OLESS_API SINT4 assertNoUntrackedFatSectors(Fat* io_pFat,
                                         Fat* io_pSmallFat,
                                         TOC* in_pTOC);
extern _OLESS_API SINT4 fatSelfTest(void);
#endif /* DEBUG */

#endif /* _Fat_h */
