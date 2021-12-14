/************************************************************************
* OpenList.h
*
* Contains list of open streams and storages
*
* (c) Schema Software Inc., 2001
* 
*************************************************************************

$Revision: 1.4 $
$Date: 2003/03/26 21:00:04 $
*/

#ifndef __OPENLIST_H__
#define __OPENLIST_H__

struct _OpenItem;
struct _OpenList;
typedef struct _OpenItem OpenItem;
typedef struct _OpenList OpenList;

extern _OLESS_API SINT4 openListConstruct(OpenList** out_ppList);

extern _OLESS_API SINT4 openListDestroy(OpenList** out_ppList);

extern _OLESS_API SINT4 openListAppend(OpenList* io_pList, StgDirType in_type, void* in_ptr);

extern _OLESS_API SINT4 openListRemoveItem(OpenList* io_pList, UINT4 dwIndex);

extern _OLESS_API SINT4 openListGetItem(OpenList* in_pList,
                             UINT4 dwIndex,
                             StgDirType* out_pType,
                             void** out_ptr);

extern _OLESS_API SINT4 openListFindItem(OpenList* in_pList,
                              UINT4* out_dwIndex,
                              StgDirType in_type,
                              void* in_ptr);

extern _OLESS_API SINT4 openListItemCount(OpenList* io_pList);

#ifdef DEBUG
extern _OLESS_API SINT4 openListSelfTest(void);
#endif /* DEBUG */

#endif /* __OPENLIST_H__ */

