/************************************************************************
* Common.h
*
* Contains commonly-used definitions
*
* (c) Schema Software Inc., 2001-2004
* 
************************************************************************

$Revision: 1.11.2.4 $
$Date: 2004/07/28 21:44:37 $
*/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <time.h>
#include "OlessExport.h"
#include "StructuredStorage.h"
#include "SsrwInputSourceCommon.h"

#ifndef min
#define min(x,y) (((x) < (y)) ? (x) : (y))
#endif /* min */

#ifndef max
#define max(x,y) (((x) > (y)) ? (x) : (y))
#endif /* max */

/* common for all platforms */
#define SSTG_DEFINE_WINTIME

/* PalmOS */
#if defined(SSTG_PALMOS)
#define SSTG_INCLUDE_STDLIB_H
#define SSTG_INCLUDE_STRING_H
#define SSTG_INCLUDE_IO_H
#define SSTG_DEFINE_WCSLEN
#define SSTG_DEFINE_WCSCMP
#include <wstring.h>
#include <wchar.h>
#define SSTG_DEFINE_WCSICMP
#define SSTG_DEFINE_WCSCPY
#define SSTG_DEFINE_TOWLOWER
#define SSTG_DEFINE_TOWUPPER
#define SSTG_DEFINE_MBSTOWCS
#define SSTG_DEFINE_WCSTOMBS
#define SSTG_USE_TIMGETSECONDS_TIME
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T off_t
#endif

/* Mac OS X with the Apple-modified gcc compiler and Apple system headers */
#if defined(__APPLE__) && defined(__GNUC__)
#include <sys/types.h>
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_STRING_H
#define SSTG_DEFINE_WCSLEN
#define SSTG_DEFINE_WCSCMP
#define SSTG_DEFINE_WCSICMP
#define SSTG_DEFINE_WCSCPY
#define SSTG_DEFINE_TOWLOWER
#define SSTG_DEFINE_TOWUPPER
#define SSTG_USE_CLIB_TIME
#define SSTG_DEFINE_TRUNCATE64_IF_POSSIBLE
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T off_t
#endif /* defined(__APPLE__) && defined(__GNUC__) */

/* Mac OS 9 with CodeWarrior */
#if defined(SSTG_MACOS9)
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_WCHAR_H
#define SSTG_DEFINE_WCSICMP
#define SSTG_DEFINE_TOWLOWER
#define SSTG_DEFINE_TOWUPPER
#define SSTG_USE_CLIB_TIME
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T off_t
#endif /* SSTG_MACOS9 */

/* Windows with Visual C++ */
#if (defined(_MSC_VER) && (defined(WIN32) || defined(WIN64))) && !defined(_WIN32_WCE)
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_WCHAR_H
#define SSTG_INCLUDE_IO_H
#define SSTG_USE_CLIB_TIME
#define SSTG_DEFINE_FSEEK64_VCPP
#define SSTG_DEFINE_FTELL64_VCPP
#define SSTG_DEFINE_FTRUNCATE_VCPP
#define SSTG_USE_VCPP_WFOPEN
#define SSTG_USE_VCPP_WREMOVE
#define SSTG_OFF_T off_t
#endif /* defined(WIN32) && defined(_MSC_VER) || defined(WIN64) && defined(_MSC_VER) */

/* Windows with Borland C++ */
#if defined(WIN32) && defined(__BORLANDC__)
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_STRING_H
#define SSTG_DEFINE_WCSICMP
#define SSTG_USE_CLIB_TIME
#define SSTG_DEFINE_TRUNCATE64_IF_POSSIBLE
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T long
#endif /* defined(WIN32) && defined(__BORLANDC__) */

/* Windows with Cygwin/gcc */
#if (defined(WIN32) && !defined(_MSC_VER) && !defined(__BORLANDC__) || defined(WIN64) && !defined(_MSC_VER) && !defined(ANDROID))
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_WCHAR_H
#define SSTG_INCLUDE_IO_H
#define SSTG_DEFINE_WCSCPY
#define SSTG_DEFINE_WCSICMP
#define SSTG_DEFINE_WCSLEN
//#define SSTG_DEFINE_TOWLOWER
//#define SSTG_DEFINE_TOWUPPER
#define SSTG_USE_CLIB_TIME
#define SSTG_DEFINE_TRUNCATE64_IF_POSSIBLE
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T off_t
#endif /* (defined(_WIN32) && !defined(_MSC_VER)) || defined(WIN64) && !defined(_MSC_VER)) */

#if defined(_WIN32_WCE) || defined(_WINRT)
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_WCHAR_H
#if !defined(_WIN32_WCE)
#define SSTG_USE_CLIB_TIME
#endif
#define SSTG_USE_VCPP_WFOPEN
#define SSTG_USE_VCPP_WREMOVE
#define SSTG_OFF_T long
#endif /* defined(_WIN32_WCE) */

/* Android
*/
#if defined(ANDROID)
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_WCHAR_H
#define SSTG_INCLUDE_WCTYPE_H
#define SSTG_DEFINE_WCSICMP
#define SSTG_USE_CLIB_TIME
#define SSTG_DEFINE_TRUNCATE64_IF_POSSIBLE
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T off_t
#endif

/* Linux.  This is a hard one because it will vary on machines with different
 * glibc versions.  For now we just support one version for testing purposes. 
 */
#if (defined(__linux__) || defined(EMCC)) && !defined(ANDROID)
#include <sys/types.h>
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_WCHAR_H
#define SSTG_INCLUDE_WCTYPE_H
#define SSTG_INCLUDE_SYS_IO_H
#define SSTG_DEFINE_WCSICMP
#define SSTG_USE_CLIB_TIME
#define SSTG_DEFINE_TRUNCATE64_IF_POSSIBLE
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T off_t
#endif /* defined(__linux__) */

/* HP */
#if defined(__hpux)
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_WCHAR_H
#define SSTG_INCLUDE_WCTYPE_H
#define SSTG_INCLUDE_SYS_IO_H
#define SSTG_DEFINE_WCSICMP
#define SSTG_USE_CLIB_TIME
#define SSTG_DEFINE_TRUNCATE64_IF_POSSIBLE
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T off_t
#endif /* defined(__hpux) */

/* Solaris */
#if defined(__sun)
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_WCHAR_H
#define SSTG_INCLUDE_WCTYPE_H
#define SSTG_DEFINE_WCSICMP
#define SSTG_USE_CLIB_TIME
#define SSTG_DEFINE_FSEEK64_O64
#define SSTG_DEFINE_FTELL64_O64
#define SSTG_DEFINE_TRUNCATE64_IF_POSSIBLE
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T off_t
#endif /* defined(__sun) */

/* SGI */
#if defined(__sgi)
#define SSTG_INCLUDE_MEMORY_H
#define SSTG_INCLUDE_WCHAR_H
#define SSTG_INCLUDE_WCTYPE_H
#define SSTG_DEFINE_WCSICMP
#define SSTG_USE_CLIB_TIME
#define SSTG_DEFINE_FSEEK64_O64
#define SSTG_DEFINE_FTELL64_O64
#define SSTG_DEFINE_TRUNCATE64_IF_POSSIBLE
#define SSTG_USE_FALSE_WFOPEN
#define SSTG_USE_FALSE_WREMOVE
#define SSTG_OFF_T off_t
#endif /* defined(__sgi) */



#ifdef SSTG_INCLUDE_WCHAR_H
#include <wchar.h>
#endif /* SSTG_INCLUDE_WCHAR_H */

#ifdef SSTG_INCLUDE_WCTYPE_H
#include <wctype.h>
#endif /* SSTG_INCLUDE_WCTYPE_H */

#ifdef SSTG_INCLUDE_STDLIB_H
#include <stdlib.h>
#endif /* SSTG_INCLUDE_STDLIB_H */

#ifdef SSTG_INCLUDE_STRING_H
#include <string.h>
#endif /* SSTG_INCLUDE_STRING_H */

#ifdef SSTG_INCLUDE_MEMORY_H
#include <memory.h>
#endif /* SSTG_INCLUDE_MEMORY_H */

#ifdef SSTG_INCLUDE_SYS_IO_H
#include <sys/io.h>
#endif /* SSTG_INCLUDE_SYS_IO_H */

#ifdef SSTG_INCLUDE_IO_H
#include <io.h>
#endif /* SSTG_INCLUDE_IO_H */

/* Now the prototypes */
#ifdef SSTG_DEFINE_WCSCMP
extern int wcscmp(const wchar_t* string1, const wchar_t* string2);
#endif /* SSTG_DEFINE_WCSCMP */

#ifdef SSTG_DEFINE_WCSICMP
extern int wcsicmp(const wchar_t *string1, const wchar_t *string2);
#endif /* SSTG_DEFINE_WCSCMP */

#ifdef SSTG_DEFINE_WCSLEN
extern size_t wcslen(const wchar_t* str);
#endif /* SSTG_DEFINE_WCSLEN */

#ifdef SSTG_DEFINE_WCSCPY
extern wchar_t *wcscpy (wchar_t *strDestination, const wchar_t *strSource);
#endif /* SSTG_DEFINE_WCSCPY */

#ifdef SSTG_DEFINE_TOWUPPER
extern int towupper(int c);
#endif /* SSTG_DEFINE_TOWUPPER */

#ifdef SSTG_DEFINE_TOWLOWER
extern int towlower(int c);
#endif /* SSTG_DEFINE_TOLOWER */

#ifdef SSTG_DEFINE_MBSTOWCS
#define mbstowcs(x,y,z) asciitowcs((x),(y),(z))
#define SSTG_DEFINE_ASCIITOWCS
#endif /* SSTG_DEFINE_MBSTOWCS */

#ifdef SSRW_USE_UTF8_AS_MBS
#define wcstombs(x,y,z) wcstou8s((x),(y),(z))
#else /* !SSRW_USE_UTF8_AS_MBS */

#ifdef SSTG_DEFINE_WCSTOMBS
#define wcstombs(x,y,z) wcstoascii((x),(y),(z))
#define SSTG_DEFINE_WCSTOASCII
#endif /* SSTG_DEFINE_WCSTOMBS */

#endif /* SSRW_USE_UTF8_AS_MBS */

extern time_t SsrwTime(time_t* in_pTime);
extern UINT4 SsrwUINTLog2(UINT4 in_num);

/* Define a standard 64-bit seek function. */
#if defined(SSTG_DEFINE_FSEEK64_VCPP)
    extern SINT8 SsrwSeekVCPP(FILE* io_pFile, SINT8 in_offset, int in_whence);
    #define SSRW_SEEK(x, y, z) SsrwSeekVCPP((x), (y), (z))
#elif defined(SSTG_DEFINE_FSEEK64_O64)
    #define SSRW_SEEK(x, y, z) fseeko64((x), (y), (z))
#else /* No special 64-bit seek available, use regular fseek */
    #define SSRW_SEEK(x, y, z) fseek((x), ((SSTG_OFF_T)(y)), (z))
#endif

/* Define a standard 64-bit tell function. */
#if defined(SSTG_DEFINE_FTELL64_VCPP)
    extern SINT8 SsrwTellVCPP(FILE* io_pFile);
    #define SSRW_TELL(x) SsrwTellVCPP((x))
#elif defined(SSTG_DEFINE_FTELL64_O64)
    #define SSRW_TELL(x) ftello64((x))
#else /* No 64-bit tell avialable, use 32-bit ftell */
    #define SSRW_TELL(x) ftell(x)
#endif

/* Define a standard 64-bit function for truncating a file. */
#if defined(SSTG_DEFINE_FTRUNCATE_VCPP)
    extern SINT4 SsrwFTruncate64VCPP(FILE* io_pFile, SINT8 in_llNewLength);
    #define SSRW_FTRUNCATE(x, y) SsrwFTruncate64VCPP((x), (y))
#elif defined(SSTG_DEFINE_FTRUNCATE64)
    extern SINT4 SsrwFTruncate64(FILE* io_pFile, SINT8 in_llNewLength);
    #define SSRW_FTRUNCATE(x, y) SsrwFTruncate64((x), (y))
#elif defined(SSTG_DEFINE_FTRUNCATE64_IF_POSSIBLE)
    extern SINT4 SsrwFTruncate64IfPossible(FILE* io_pFile, in_llNewLength);
    #define SSRW_FTRUNCATE(x, y) SsrwFTruncate64IfPossible((x), (y))
#else /* Just return an error; no facility for file truncation available */
    #define SSRW_FTRUNCATE(x, y) (-1)
#endif

/* Define a standard wide-character fopen function. */
#if defined(SSTG_USE_VCPP_WFOPEN) /* Visual C++ version */
    #define SSRW_WFOPEN(x,y) _wfopen((x),(y))
#elif defined(SSTG_USE_POSIX_WFOPEN) /* POSIX version */
    #define SSRW_WFOPEN(x,y) wfopen((x),(y))
#elif defined SSTG_USE_FALSE_WFOPEN /* No wide-character fopen available, 
                                        * use a fake one */
    extern FILE* SsrwFalseWFopen(const wchar_t* in_pFilename, 
                                 const wchar_t* in_pMode);
    #define SSRW_WFOPEN(x,y) SsrwFalseWFopen((x),(y))
#endif

/* Define a standard wide-character remove function. */
#if defined(SSTG_USE_VCPP_WREMOVE) /* Visual C++ version */
    #define SSRW_WREMOVE(x) _wremove((x))
#elif defined(SSTG_USE_POSIX_WREMOVE) /* POSIX version */
    #define SSRW_WREMOVE(x) wremove((x))
#elif defined SSTG_USE_FALSE_WREMOVE /* No wide-character remove available, 
                                         * use a fake one */
    extern int SsrwFalseWRemove(const wchar_t* in_pFilename);
    #define SSRW_WREMOVE(x) SsrwFalseWRemove((x))
#endif

/* This structure is the exact copy of Windows' FILETIME structure
 * we are typdef'ing it with a different name to avoid name collisions */
#ifdef SSTG_DEFINE_WINTIME
typedef struct tagWinTime
{
    UINT4 ulLowDateTime;
    UINT4 ulHighDateTime;
}
WinTime;

/* Functions to handle Windows-style FILETIME structures */
extern SINT4 time_tFromWindowsTime(const WinTime * in_pWinTime, 
                                   time_t* out_ptime);
extern SINT4 WindowsTimeFromTime_t(time_t t, WinTime* out_pTime);

#endif /* SSTG_DEFINE_WINTIME */

#ifdef DEBUG
extern SINT4 commonSelfTest(void);
#endif /* DEBUG */

#endif /* __COMMON_H__ */

