/************************************************************************
* Directory.h
*
* Declares the DirectoryEntry type
*
* (c) Schema Software Inc., 2001-2004
* 
************************************************************************

$Revision: 1.6.2.2 $
$Date: 2004/02/26 18:26:00 $
*/

#ifndef __DIRECTORY_H__
#define __DIRECTORY_H__


#define NAME_SIZE           32
#define CLSID_SIZE          16

#define DIRECTORY_BLOCKSIZE 128

struct _RootStorage;
struct _DirectoryEntry;
typedef struct _DirectoryEntry DirectoryEntry;
struct _StreamSharedBlock;
typedef struct _StreamSharedBlock StreamSharedBlock;

typedef enum
{
    DIR_FREE = 0,
    DIR_STORAGE = 1,
    DIR_STREAM = 2,
    DIR_ROOT = 5
} DirEntry;

extern _OLESS_API SINT4 directoryConstructEntry(UINT4 in_sid, DirectoryEntry* in_pEntry);
extern _OLESS_API SINT4 directoryConstructNilEntry(DirectoryEntry* out_pEntry);
extern _OLESS_API SINT4 sizeOfDirectoryEntry();
extern _OLESS_API UINT4 directoryGetIndex(DirectoryEntry* in_pDirEntry);
extern _OLESS_API DirectoryEntry*  directoryGetAtIndex(DirectoryEntry* in_aDir, UINT4 i);
extern _OLESS_API SINT4 directoryInitEntry (DirectoryEntry* in_pEntry, 
                                 const wchar_t* in_name, 
                                 DirEntry in_type, 
                                 struct _RootStorage* in_pRoot);
extern _OLESS_API SINT4 directoryEntryWrite(DirectoryEntry* in_pEntry, CHAR* in_buffer, SINT4* io_poffset);
extern _OLESS_API UINT4 directoryCountChildren(DirectoryEntry* in_pEntry, UINT4 n, DirectoryEntry* aDir);

extern _OLESS_API SINT4 directoryConstructAllDirs (struct _RootStorage* in_root,
                                        struct _DirectoryEntry** out_paDirs, UINT4* out_pcDirs);
extern _OLESS_API SINT4 directoryFindChild (DirectoryEntry* in_pDir,
                                 const wchar_t* in_wszName,
                                 DirectoryEntry* aDir,
                                 DirectoryEntry** out_ppChild);
extern _OLESS_API DirEntry directoryGetType (DirectoryEntry* in_dir);
extern _OLESS_API UINT4 directoryGetStartSector (DirectoryEntry* in_dir);
extern _OLESS_API UINT8 directoryGetStreamLength (DirectoryEntry* in_dir);
extern _OLESS_API SINT4 directoryInsertChild (DirectoryEntry* in_pRoot,
                                   DirectoryEntry* in_pChild,
                                   DirectoryEntry* aDir);
extern _OLESS_API SINT4 directoryRemoveEntry(DirectoryEntry*  io_aDir,
                                  DirectoryEntry*  io_pPathParent,
                                  DirectoryEntry*  io_pToRemove);
extern _OLESS_API DirectoryEntry* directoryGetFirstChild (DirectoryEntry* in_aDir, DirectoryEntry* in_dir);
extern _OLESS_API DirectoryEntry* directoryGetRightSibling (DirectoryEntry* in_aDir, DirectoryEntry* in_dir);
extern _OLESS_API UINT4 directoryFillStgArray(DirectoryEntry* in_pDir, StgInfo* out_aStgInfo, UINT4 i,
                                   DirectoryEntry* aDir);
extern _OLESS_API SINT4 directoryFillStgInfo(DirectoryEntry* in_pDir, StgInfo* out_pStgInfo);
extern _OLESS_API SINT4 directorySetSize(DirectoryEntry* io_pDir, UINT8 in_llNewSize);
extern _OLESS_API SINT4 directorySetStartSector(DirectoryEntry* io_pDir, UINT4 in_lNewStartSector);
extern _OLESS_API SINT4 directorySetCurrentCreationTime(DirectoryEntry* io_pDir);
extern _OLESS_API SINT4 directorySetCurrentModificationTime(DirectoryEntry* io_pDir);
extern _OLESS_API SINT4 directorySetClass(DirectoryEntry* io_pDir, const SSRW_CLSID* in_pCLSID);
extern _OLESS_API SINT4 directoryInvalidate(DirectoryEntry* io_pDir);
extern _OLESS_API SINT4 directoryQueryStreamSharedBlock(DirectoryEntry* io_pDir, 
                                             StreamSharedBlock** out_ppSharedBlock);
extern _OLESS_API SINT4 directoryReleaseStreamSharedBlock(DirectoryEntry* io_pDir);

#ifdef DEBUG

extern _OLESS_API UINT4 directoryBlankFatSectors(DirectoryEntry* in_aDir, 
                                      UINT4 in_ulCurIndex, 
                                      UINT4* io_pulFat, 
                                      UINT4 in_ulFatLength,
                                      UINT4* io_pulSmallFat, 
                                      UINT4 in_ulSmallFatLength);
extern _OLESS_API SINT4 directorySelfTest(void);

#endif /* DEBUG */

#endif /* __DIRECTORY_H__ */

