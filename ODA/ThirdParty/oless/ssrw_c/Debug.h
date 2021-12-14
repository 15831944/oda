/************************************************************************
* Debug.h
*
* Contains debug-only and assert-related stuff
*
* (c) Schema Software Inc., 2001
* 
************************************************************************

$Revision: 1.6.2.1 $
$Date: 2004/02/19 01:36:00 $
*/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#if defined(DEBUG) && defined(_MSC_VER)
    #ifndef _DEBUG
        #define _DEBUG
    #endif /* _DEBUG */
    #define _CRTDBG_MAP_ALLOC
#endif /* DEBUG */

#include <stdlib.h>

#if defined(_CRTDBG_MAP_ALLOC) && !defined(_WIN32_WCE)
#include <crtdbg.h>
#endif /* _CRTDBG_MAP_ALLOC */

#ifdef DEBUG
extern _OLESS_API SINT4 headerSelfTest();
extern _OLESS_API SINT4 structuredStorageSelfTest();
extern _OLESS_API SINT4 tocSelfTest();
extern _OLESS_API SINT4 structuredStorageIntentionallyLeak(unsigned long in_ulBytes);
#endif /* DEBUG */

#ifdef DEBUG
#define ASSERT(x) if ((x)) {} else { _AssertProc((CHAR*)#x, (CHAR*)__FILE__, __LINE__); }
extern _OLESS_API void _AssertProc(CHAR* cond, CHAR* file, SINT4 line);
#else /* DEBUG */
#ifndef ASSERT
#define ASSERT(x)
#endif //ASSERT
#endif /* DEBUG */

#if defined(DEBUG) && !defined(_CRTDBG_MAP_ALLOC)
#undef malloc
#undef free
#undef realloc
#undef calloc
#define malloc dbgMalloc
#define free dbgFree
#define realloc dbgRealloc
#define calloc dbgCalloc
#endif /* DEBUG && !_CRTDBG_MAP_ALLOC */

#ifdef DEBUG
extern void* dbgMalloc(size_t);
extern void dbgFree(void*);
extern void* dbgRealloc(void*, size_t);
extern void* dbgCalloc(size_t, size_t);
extern SINT4 dbgMemStats(SINT8* out_pllBytesAllocated,
                         SINT8* out_pllBytesAllocatedMax);
#endif /* DEBUG */

#endif /* __DEBUG_H__ */
