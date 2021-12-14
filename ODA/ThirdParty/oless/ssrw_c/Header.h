/************************************************************************
* Header.h
* Describes compound file's Header structure and prototypes functions
* dealing with it
*
* (c) Schema Software Inc., 2001-2004
*
*************************************************************************

$Revision: 1.7.2.2 $
$Date: 2004/02/24 19:09:25 $
*/

#ifndef __HEADER_H__
#define __HEADER_H__

#define HEADER_SIZE 76 /* in bytes */

#define SSTG_BYTE_ORDER 0xfffe 
/* This has nothing to do with the price of bottle of vodka
  in Soviet Union. It is just the most widely used version of OLE DLL
*/
#define DEFAULT_MIN_VERSION 62
#define DEFAULT_MAX_VERSION 3

#define DEFAULT_SMALL_SECTOR_SHIFT 6
#define DEFAULT_SECTOR_SHIFT 9

#define DEFAULT_SMALL_CUTOFF 0x1000

#define HEADER_FAT_SECTORS 109
#define HEADER_BLOCK_SIZE 512

extern _OLESS_API SINT4 headerRead (SSRWIS* in_pIS, Header* out_header);
extern _OLESS_API SINT4 headerInit(Header **out_ppHeader);
extern _OLESS_API SINT4 headerWrite (SSRWIS* in_pIS, Header* in_header);
extern _OLESS_API SINT4 headerReadFatSectors(SSRWIS* in_pIS,
                                  Header* in_pHeader,
                                  UINT4* out_pSectors,
                                  UINT4* out_pCount);
extern _OLESS_API SINT4 headerWriteFatSectors(SSRWIS* in_pIS,
                                   UINT4* in_alSectors,
                                   UINT4 in_count);
extern _OLESS_API SINT4 headerWriteSectorFiller(const Header* in_pHeader,
                                     SSRWIS* in_pIS);
extern _OLESS_API UINT2 headerGetSectorShift(const Header* in_pHeader);
extern _OLESS_API void headerSetSectorShift(Header* in_pHeader, UINT2 in_newSectorShift);

extern _OLESS_API UINT2 headerGetSmallSectorShift(const Header* in_pHeader);
extern _OLESS_API void headerSetFatSize(Header* io_pHeader,
                             UINT4 in_cdwSize);
extern _OLESS_API UINT4 headerGetFatSize(const Header* io_pHeader);
extern _OLESS_API void headerSetXFatStartSector(Header* io_pHeader,
                                     UINT4 in_dwSector);
extern _OLESS_API UINT4 headerGetXFatStartSector(const Header* io_pHeader);
extern _OLESS_API void headerSetXFatSize(Header* io_pHeader,
                              UINT4 in_cdwSize);
extern _OLESS_API UINT4 headerGetXFatSize(const Header* io_pHeader);
extern _OLESS_API void headerSetSmallFatStartSector(Header* io_pHeader,
                                         UINT4 in_dwSector);
extern _OLESS_API UINT4 headerGetSmallFatStartSector(const Header* in_pHeader);
extern _OLESS_API void headerSetSmallFatSize(Header* io_pHeader,
                                  UINT4 in_cdwSize);
extern _OLESS_API UINT4 headerGetSmallFatSize(const Header* in_pHeader);
extern _OLESS_API void headerSetDirectoryStartSector(Header* io_pHeader,
                                          UINT4 in_dwSector);
extern _OLESS_API UINT4 headerGetDirectoryStartSector(const Header* in_pHeader);
extern _OLESS_API UINT4 headerGetSmallStreamCutoff(const Header* in_pHeader);

extern _OLESS_API UINT2 headerGetMajorVersion(const Header* in_pHeader);
extern _OLESS_API SINT4 headerEnsureV4(Header* io_pHeader);

extern _OLESS_API SINT4 headerSetNumDirSectors(Header* io_pHeader, UINT4 in_ulNumDirSectors);

#endif /* __HEADER_H__ */ 
