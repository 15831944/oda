/************************************************************************
* Stream.h
*
* Contains code for managing open streams in a structured storage
*
* (c) Schema Software Inc., 2001-2004
* 
*************************************************************************

$Revision: 1.3.2.1 $
$Date: 2004/02/24 19:09:26 $
*/

#ifndef _STREAM_H
#define _STREAM_H

struct _StreamSharedBlock;

extern _OLESS_API SINT4 streamConstruct (RootStorage* in_root,
                              Storage* in_parent,
                              DirectoryEntry* in_dir,
                              StgMode in_mode,
                              Stream** out_child);
extern _OLESS_API SINT4 streamDestroy (Stream** io_ppStream);
extern _OLESS_API SINT4 streamUpgradeMiniToRegular(Stream* io_pStream);

extern _OLESS_API SINT4 streamDestroySharedBlock(StreamSharedBlock** io_ppSharedBlock);
extern _OLESS_API SINT4 streamConstructSharedBlock(StreamSharedBlock** out_pSharedBlock);
extern _OLESS_API UINT4 streamSharedBlockGetRefCount(
                     const StreamSharedBlock* in_pSharedBlock);
extern _OLESS_API void  streamSharedBlockSetRefCount(StreamSharedBlock* io_pSharedBlock, 
                                          UINT4 in_ulRefCount);

#ifdef DEBUG

extern _OLESS_API SINT4 streamSelfTest();
extern _OLESS_API SINT4 assertMiniStreamIntegrity(RootStorage* in_pRoot);

#endif /* DEBUG */

#endif /* _STREAM_H */

