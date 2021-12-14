/************************************************************************
* RootStorage.h
* Header describing the strcutures attached to the Root storage and
* compound file
*
* (c) Schema Software Inc., 2001-2004
* 
*************************************************************************

$Revision: 1.4.2.4 $
$Date: 2004/02/26 18:26:00 $
*/
#ifndef __ROOTSTORAGE_H__
#define __ROOTSTORAGE_H__

#include "Version.h"

struct _Directory;
struct _DocumentProperties;
struct _Fat;
struct _Header;
struct _TOC;
typedef struct _Directory Directory;
typedef struct _DocumentProperties DocumentProperties;
typedef struct _Fat Fat;
typedef struct _Header Header;
typedef struct _TOC TOC;

extern _OLESS_API SINT4 rootStorageGetStatus (RootStorage* in_root);
extern _OLESS_API CHAR* rootStorageReadSector (RootStorage* root, UINT4 sector);
extern _OLESS_API SINT4 rootStorageWriteSector (RootStorage* in_stg,
                                     BYTE* in_buf,
                                     UINT4* out_pSector,
                                     boolean in_isMetadata);
extern _OLESS_API SINT4 rootStorageRewriteSector (RootStorage* in_pRoot,
                                       BYTE* in_buf,
                                       UINT4 in_dwSector);
extern _OLESS_API SINT4 rootStorageWriteMiniSector (RootStorage* io_pRoot, 
                                         BYTE* in_buf, 
                                         UINT4* out_pSector);

extern _OLESS_API SINT4 rootStorageGetSector (RootStorage* root,
                                   UINT4 sector,
                                   void* buffer);
extern _OLESS_API Header* rootStorageGetHeader (RootStorage* root);
extern _OLESS_API SINT4 rootStorageGetStatus (RootStorage* root);
extern _OLESS_API void rootStorageGetDirChain(RootStorage* in_pRoot,
                                   UINT4** out_ppdwDirChain,
                                   UINT4* out_pcdwDirChain);
extern _OLESS_API StgMode rootStorageGetMode(RootStorage* in_pRoot);
extern _OLESS_API DocumentProperties* rootStorageGetDocProperties(RootStorage* root);
extern _OLESS_API Fat* rootStorageGetFat(RootStorage* in_pRoot);
extern _OLESS_API Fat* rootStorageGetSmallFat(RootStorage* in_pRoot);
extern _OLESS_API TOC* rootStorageGetTOC(RootStorage* in_pRoot);
extern _OLESS_API void rootStorageSetLastMiniStreamSector(RootStorage* io_pRoot, UINT4 in_dwSector);
extern _OLESS_API UINT4 rootStorageGetLastMiniStreamSector(RootStorage* in_pRoot);
extern _OLESS_API SINT4 rootStorageSetDirty(RootStorage* io_pRoot);

#ifdef DEBUG
extern _OLESS_API SINT4 assertNoLeakedSectors(RootStorage* in_pRoot);
#endif /* DEBUG */

#endif

