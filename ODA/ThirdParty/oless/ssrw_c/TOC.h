/************************************************************************
* TOC.h
* Header describing the table of contents and its managing structure
*
* (c) Schema Software Inc., 2001-2004
*
*************************************************************************

$Revision: 1.6.2.3 $
$Date: 2004/02/26 18:26:00 $
*/
#ifndef __TOC_H__
#define __TOC_H__

_OLESS_API SINT4 tocConstruct(UINT4 in_sectorSize, TOC** out_ppToc);
_OLESS_API SINT4 tocDestroy(TOC** io_ppTOC);
_OLESS_API SINT4 tocSetSectorSize(TOC* io_pTOC, UINT4 in_ulSectorSize);
_OLESS_API SINT4 tocAddAndInitEntry(TOC* in_pTOC,
					const wchar_t* in_name,
					DirEntry in_type,
					struct _RootStorage* in_pRoot,
					DirectoryEntry** out_ppEntry);
_OLESS_API SINT4 tocRemoveEntry(TOC* io_pTOC, 
                     DirectoryEntry* in_pPathParent, 
                     DirectoryEntry* in_pToRemove);
_OLESS_API SINT4 tocWrite(TOC* in_this, struct _RootStorage* in_pRoot);
_OLESS_API SINT4 tocRead (TOC* io_pTOC, struct _RootStorage* in_pRoot);
_OLESS_API UINT4 tocCountEntryChildren(TOC* in_pTOC, DirectoryEntry* in_dir);
_OLESS_API UINT4 tocFillChildrenStgArray(TOC* in_pTOC, DirectoryEntry* in_dir, StgInfo* out_aInfo);
_OLESS_API SINT4 tocFindChildEntry (TOC* in_pToc,
                         DirectoryEntry* in_pParent,
                         const wchar_t* in_name,
                         DirectoryEntry** out_ppDir);
_OLESS_API SINT4 tocInsertChild (TOC* in_pTOC,
                      DirectoryEntry* in_pParent,
                      DirectoryEntry* in_pChild);
_OLESS_API SINT4 tocInvalidateEntry(TOC* io_pTOC, UINT4 in_ulIndex);
_OLESS_API DirectoryEntry* tocGetEntryAtIndex(TOC* in_pToc, UINT4 in_ulIndex);

#ifdef DEBUG
_OLESS_API SINT4 
tocBlankUsedSectors(TOC* in_pTOC,
                    UINT4* io_pulFat,
                    UINT4 in_ulFatLength,
                    UINT4* io_pulSmallFat,
                    UINT4 in_ulSmallFatLength);
#endif /* DEBUG */

#endif /* __TOC_H__ */

