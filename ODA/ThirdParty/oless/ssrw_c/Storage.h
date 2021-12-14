/************************************************************************
* Storage.h
*
* Contains code for handling Storage structures
*
* (c) Schema Software Inc., 2001-2003
*
*************************************************************************

$Revision: 1.3 $
$Date: 2003/03/26 21:00:04 $
*/

#ifndef _Storage_h
#define _Storage_h

struct _RootStorage;
struct _DirectoryEntry;
struct _Stream;

extern _OLESS_API SINT4 storageConstructRoot (RootStorage* in_root, Storage** out_storage);
extern _OLESS_API SINT4 storageDestroy (Storage** in_ppStorage);
extern _OLESS_API SINT4 storageCreateAtRoot (RootStorage* in_pRoot, Storage** out_ppChild);
extern _OLESS_API SINT4 storageRemoveOpenChild (Storage* io_pThis,
                                         StgDirType in_type,
                                         void* in_pChild);
extern _OLESS_API SINT4 closeStorageInternal (Storage** io_ppStorage);

#ifdef DEBUG

extern _OLESS_API SINT4 storageSelfTest();

#endif /* DEBUG */

#endif

