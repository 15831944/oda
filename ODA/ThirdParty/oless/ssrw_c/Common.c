/************************************************************************
* Common.c
*
* Contains commonly-used definitions
*
* (c) Schema Software Inc., 2001
*
*************************************************************************

$Revision: 1.10.2.2 $
$Date: 2004/07/28 21:44:37 $
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "Types.h"
#include "Common.h"
#include "Debug.h"
#include "UTF8.h"
#include "StructuredStorage.h"

/* The difference between Jan 1, 1601 and Jan 1, 1970 in seconds */
#define TIME_DIFF_1601_1970 11644473600L 


/* 116444736000000000 == 27111902 * 2^32 + 3577643008 */
/*     ==    413 * 2^48 + 45534 * 2^32 + 54590 * 2^16 + 32768 */

#define MAGIC_2_0 27111902
#define MAGIC_2_1 3577643008

#define MAGIC_4_0   413
#define MAGIC_4_1   45534
#define MAGIC_4_2   54590
#define MAGIC_4_4   32768


/* NOTE: The following 2 macros won't compile on Solaris X64 unless they are on a single line. */
#define MYDIVIDE3( ulHighBits, ulMiddleBits, ulLowBits, number ) ( ulMiddleBits += (ulHighBits % number) << 16, ulHighBits /=  number, ulLowBits += (ulMiddleBits % number) << 16, ulMiddleBits /= number, ulLowBits /= number )

#define MYMULT3( ulHighBits, ulMiddleBits, ulLowBits, number ) ( ulLowBits *= number, ulMiddleBits = ulMiddleBits * number + (ulLowBits >> 16), ulHighBits = ulHighBits * number + (ulMiddleBits >> 16), ulLowBits &= 0xffff, ulMiddleBits &= 0xffff )

#if defined(SSTG_DEFINE_WCSCMP)
/*============================================================================
  Description: See documentation for standard C library wcscmp
  ==========================================================================*/
int wcscmp(const wchar_t* string1, const wchar_t* string2)
{
    const wchar_t* cur1 = string1;
    const wchar_t* cur2 = string2;

    for(;;)
    {
        if (*cur1 == 0 && *cur2 == 0)
        {
            return 0;
        }
        else if ((*cur1 == 0) || (*cur1 < *cur2))
        {
            return -1;
        }
        else if ((*cur2 == 0) || (*cur2 < *cur1))
        {
            return 1;
        }
        cur1++;
        cur2++;
    }
}
#endif /* SSTG_DEFINE_WCSCMP */

#if defined(SSTG_DEFINE_WCSICMP)
/*============================================================================
  Description: See documentation for standard C library wcsicmp
  ==========================================================================*/
int wcsicmp( const wchar_t *string1, const wchar_t *string2 )
{
    size_t  len1 =      wcslen (string1);
    size_t  len2 =      wcslen (string2);
    size_t  len =       len1 < len2 ? len1 : len2;
    wchar_t c1 =        0;
    wchar_t c2 =        0;
    size_t  i =         0;

    for (i = 0; i < len; i++)
    {
        c1 = towlower(string1[i]);
        c2 = towlower(string2[i]);

        if( c1 < c2 )
        {
            return -1;
        }
        else if( c1 > c2 )
        {
            return 1;
        }
    }

    if( len1 < len2 )
    {
        return -1;
    }
    else if( len1 > len2 )
    {
        return 1;
    }

    return 0;
}
#endif /* SSTG_DEFINE_WCSICMP */

#if defined(SSTG_DEFINE_WCSLEN)
/*============================================================================
  Description: See documentation for standard C library wcscpy
  ==========================================================================*/
size_t wcslen(const wchar_t* str)
{
    size_t len = 0;

    for (len = 0; ; len++)
    {
        if (str[len] == 0)
        {
            break;
        }
    }

    return len;
}
#endif /* SSTG_DEFINE_WCSLEN */

#if defined(SSTG_DEFINE_WCSCPY)
/*============================================================================
  Description: See documentation for standard C library wcscpy
  ==========================================================================*/
wchar_t *wcscpy (wchar_t *strDestination, const wchar_t *strSource)
{
    wchar_t*        cur1 = strDestination;
    const wchar_t*  cur2 = strSource;

    for (;*cur2 != 0; cur1++, cur2++)
    {
        *cur1 = *cur2;
    }
    *cur1 = 0;
    return strDestination;
}
#endif /* SSTG_DEFINE_WCSCPY */


#if defined (SSTG_DEFINE_TOWUPPER) || defined(SSTG_DEFINE_TOWLOWER) || defined(SSTG_DEFINE_ASCIITOWCS) || defined(SSTG_DEFINE_WCSTOASCII)

extern size_t asciitowcs(wchar_t* wcstring,
                         const char* asciistring,
                         size_t nwchars);
extern size_t wcstoascii(char* asciistring,
                         const wchar_t* wcstring,
                         size_t nbytes);

/*============================================================================
  Description: Similar to standard C mbstowcs function, except characters 
               outside the range 0-127 are not supported.  
  Arguments:
   wcstring - target buffer to which to write the wide string
   asciistring - source ASCII string
   nwchars - the number of wide characters available in the target buffer
  Return:
   The number of characters converted, excluding the null terminator if it was
   written
  ==========================================================================*/
size_t asciitowcs(wchar_t* wcstring, const char* asciistring, size_t nwchars)
{
    size_t i = 0;

    for (i = 0; i < nwchars; i++)
    {
        wcstring[i] = asciistring[i];
        if (asciistring[i] == 0)
        {
            break;
        }
    }
    return i;
}


/*============================================================================
  Description: Similar to standard C wcstombs function, except characters 
               outside the range 0-127 are not supported.  
  Arguments:
   asciistring - target buffer to which to write the ascii string
   wcstring - source wide string
   nwbytes - the number of bytes available in the target buffer
  Return:
   The number of characters converted, excluding the null terminator if it was
   written
  ==========================================================================*/
size_t wcstoascii(char* asciistring, const wchar_t* wcstring, size_t nbytes)
{
    size_t i = 0;

    for (i = 0; i < nbytes; i++)
    {
        asciistring[i] = (char)wcstring[i];
        if (wcstring[i] == 0)
        {
            break;
        }
    }
    return i;
}

#endif /* SSTG_DEFINE_TOWUPPER || SSTG_DEFINE_TOWLOWER || SSTG_DEFINE_WCSTOASCII || SSTG_DEFINE_ASCIITOWCS */


#if defined(SSTG_DEFINE_TOWUPPER)
/*============================================================================
  Description: See documentation for standard C library towupper
  ==========================================================================*/
int towupper(int c)
{
    wchar_t wszWide[2] =    {0};
    char    szAscii[10] =   {0};

    wszWide[0] = (wchar_t)c;
    wszWide[1] = (wchar_t)0;

    wcstoascii (szAscii, wszWide, sizeof(szAscii));

    szAscii[0] = (char) toupper (szAscii[0]);
    szAscii[1] = 0;

    asciitowcs (wszWide, szAscii, 1);
    return wszWide[0];
}
#endif /* SSTG_DEFINE_TOWUPPER */

#if defined(SSTG_DEFINE_TOWLOWER)
/*============================================================================
  Description: See documentation for standard C library towlower
  ==========================================================================*/
int towlower(int c)
{
    wchar_t wszWide[2] =    {0};
    char    szAscii[10] =   {0};

    wszWide[0] = (wchar_t)c;
    wszWide[1] = (wchar_t)0;

    wcstoascii (szAscii, wszWide, sizeof(szAscii));

    szAscii[0] = (char) tolower (szAscii[0]);
    szAscii[1] = 0;

    asciitowcs (wszWide, szAscii, 1);
    return wszWide[0];
}
#endif /* SSTG_DEFINE_TOWLOWER */

#ifdef SSTG_DEFINE_WINTIME
/*============================================================================
  Description:  Converts Windows FILETIME structure, wich is UTC time
                since midnight Jan 1, 1601 in hundreds of nanoseconds 
                (10 exp -7 seconds) to standard C library type time_t, which 
                is UTC time in seconds since midnight Jan 1, 1970
  Arguments: 
   in_pWinTime - const pointer to FILETIME structure to convert from
   out_ptime -   pointer to time_t to convert to
 ==========================================================================*/
//time_t 
SINT4
time_tFromWindowsTime(const WinTime * in_pWinTime, time_t* out_ptime)
{
    UINT4 ulLowBits     = 0;   /* 16 bit, low    bits */
    UINT4 ulMiddleBits  = 0;   /* 16 bit, medium bits */
    UINT4 ulHighBits    = 0;   /* 32 bit, high   bits */
    UINT4 ulCarryBit    = 0;   /* carry bit for subtraction */
    SINT4 lNegative     = 0;   /* whether a represents a negative value */

    /* Shortcut time "0" to the earliest available time_t */
    if ((in_pWinTime->ulLowDateTime == 0) &&
        (in_pWinTime->ulHighDateTime == 0))
    {
        *out_ptime = 0;
        return SSTG_OK;
    }

    /* Copy the time values to ulHighBits/ulMiddleBits/ulLowBits  */
    ulLowBits =    ((UINT4)in_pWinTime->ulLowDateTime ) & 0xffff;
    ulMiddleBits = ((UINT4)in_pWinTime->ulLowDateTime ) >> 16;
    ulHighBits =   ((UINT4)in_pWinTime->ulHighDateTime );

    /* Subtract the time difference */
    if (ulLowBits >= MAGIC_4_4) 
    {
        ulLowBits -= MAGIC_4_4;
    }
    else 
    {
        ulLowBits += (1 << 16) - MAGIC_4_4;        
        ulCarryBit = 1;
    }

    if (ulMiddleBits >= MAGIC_4_2 + ulCarryBit) 
    {
        ulMiddleBits -= MAGIC_4_2 + ulCarryBit;
    }
    else
    {
        ulMiddleBits += (1 << 16) - MAGIC_4_2 - ulCarryBit;
        ulCarryBit = 1;
    }

    ulHighBits -= MAGIC_2_0 + ulCarryBit;

    /* If a is negative, replace a by (-1-a) */
    lNegative = (ulHighBits >= ((UINT4)1) << 31);
    if (lNegative)
    {
        /* Set a to -a - 1 (a is ulHighBits/ulMiddleBits/ulLowBits) */
        ulLowBits = 0xffff - ulLowBits;
        ulMiddleBits = 0xffff - ulMiddleBits;
        ulHighBits = ~ulHighBits;
    }

    /* Divide a by 10000 * 1000  */
    /* which are both less than 0xffff */
    MYDIVIDE3( ulHighBits, ulMiddleBits, ulLowBits, 10000 );
    MYDIVIDE3( ulHighBits, ulMiddleBits, ulLowBits, 1000 );

    /* If a was negative, replace a by (-1-a) */
    if (lNegative)
    {
        /* Set a to -a - 1 (a is ulHighBits/ulMiddleBits/ulLowBits) */
        ulLowBits = 0xffff - ulLowBits;
        ulMiddleBits = 0xffff - ulMiddleBits;
        ulHighBits = ~ulHighBits;
    }
 
    *out_ptime = ((((time_t)ulHighBits) << 16) << 16) + (ulMiddleBits << 16) + ulLowBits;
    return *out_ptime;
}

#endif /* SSTG_DEFINE_WINTIME */


#ifdef SSTG_DEFINE_WINTIME
/*============================================================================
  Description:  Converts standard C library type time_t, which is UTC 
                time in seconds since midnight Jan 1, 1970 to Windows FILETIME 
                structure, wich is UTC time since midnight Jan 1, 1601 in 
                hundreds of nanoseconds (10 exp -7 seconds)
  Arguments: 
   in_t - time_t from which to convert
   out_pTime - on success, the WinTime structure pointed to by this variable
               will contain the converted time in Windows format
  Return:
   Status code
 ==========================================================================*/
SINT4 
WindowsTimeFromTime_t(time_t in_t, WinTime* out_pTime)
{  
    UINT4 ulLowBits       = 0;  /* 16 bit, low    bits */
    UINT4 ulMiddleBits    = 0;  /* 16 bit, medium bits */
    UINT4 ulHighBits      = 0;  /* 32 bit, high   bits */
 
    /* Copy the standard time to ulHighBits/ulMiddleBits/ulLowBits */
    ulLowBits =  in_t & 0xffff;
    ulMiddleBits = (in_t >> 16) & 0xffff;
    ulHighBits = 0;

    /* Multiply by 10000 * 1000 */
    MYMULT3( ulHighBits, ulMiddleBits, ulLowBits, 10000 );
    MYMULT3( ulHighBits, ulMiddleBits, ulLowBits, 1000 );

    /* Add the time difference */
    ulLowBits += MAGIC_4_4;
    ulMiddleBits += MAGIC_4_2 + (ulLowBits >> 16);
    ulHighBits += MAGIC_2_0 + (ulMiddleBits >> 16);
    ulLowBits &= 0xffff;
    ulMiddleBits &= 0xffff;

    out_pTime->ulLowDateTime  = (ulMiddleBits << 16) + ulLowBits;
    out_pTime->ulHighDateTime = ulHighBits;

    return SSTG_OK;
}

#endif /* SSTG_DEFINE_WINTIME */


/*============================================================================
  Description:  Works the same as the C library time() function.  time() is not
                supported on PalmOS.  In the Metrowerks standard C library for
                Palm there is just a stub for this function that causes link
                errors.
  Arguments: 
   in_pTime - if non-NULL, the value pointed to by this pointer will also
              receive the return value
  Return:
   The number of seconds between now and 12:00am January 1, 1970
 ==========================================================================*/
time_t SsrwTime(time_t* in_pTime)
{
    time_t result = 0;

#if defined(SSTG_USE_CLIB_TIME)
    result = time(NULL);
#elif defined (SSTG_USE_TIMGETSECONDS_TIME)
    /* This constant arrived at experimentally on the PalmOS emulator.  It's
    * the number of seconds between 00:00:00 January 1, 1970 and 00:00:00
    * January 1, 1904. */
    result = TimGetSeconds() - 2082844800UL;
#endif /* SSTG_USE_CLIB_TIME */

    if (in_pTime != NULL)
    {
        *in_pTime = result;
    }
    return result;
}


/*============================================================================
  Description:  Determines the log (base 2) of a positive integer
  Arguments: 
   in_ulNum - Non-zero unsigned integer
  Return:
   the log (base 2) of the argument
 ==========================================================================*/
UINT4 SsrwUINTLog2(UINT4 in_num)
{
    UINT4 i = 0;

    ASSERT(in_num != 0);

    for (i = 0; in_num > 1; i++)
    {
        in_num = in_num >> 1;
    }

    return i;
}

#ifdef SSTG_DEFINE_FSEEK64_VCPP
/*============================================================================
  Description:  Performs a seek operation on a FILE* using the Visual C++
                C library functions

                CAUTION CAUTION CAUTION
                This is a hack.  For some reason there is no 64-bit version of
                fseek in the Visual C++ libraries.
                We use fpos_t here with the knowledge that it is defined to be
                equivalent to __int64, although it should be considered an
                opaque type.  This code is not portable.
                Furthermore, for SEEK_CUR and SEEK_END, we perform extra 
                operations, which is inefficient.  
  Arguments: 
   io_pFile -  non-NULL file pointer to seek
   in_offset - offset to seek
   in_whence - SEEK_SET, SEEK_CUR, or SEEK_END as used in fseek()
  Return:
   0 on success, nonzero value on failure
 ==========================================================================*/
extern SINT8 SsrwSeekVCPP(FILE* io_pFile, SINT8 in_offset, int in_whence)
{
    fpos_t fp;

    switch(in_whence)
    {
    case SEEK_SET:
        fp = in_offset;
        break;
    case SEEK_END:
        fseek(io_pFile, 0, SEEK_END);
        /* Intentional fall through */
    case SEEK_CUR:
        fp = SSRW_TELL(io_pFile) + in_offset;
        break;
    }

    return fsetpos(io_pFile, &fp);
}
#endif /* SSTG_DEFINE_FSEEK64_VCPP */


#ifdef SSTG_DEFINE_FTELL64_VCPP
/*============================================================================
  Description:  Performs a tell operation on a FILE* using the Visual C++
                C library functions

                CAUTION CAUTION CAUTION
                This is a hack.  For some reason there is no 64-bit version of
                ftell in the Visual C++ libraries.
                We use fpos_t here with the knowledge that it is defined to be
                equivalent to __int64, although it should be considered an
                opaque type.  This code is not portable.
  Arguments: 
   in_pFile -  non-NULL file pointer from which to get the seek location

  Return:
   -1 on failure, otherwise the current seek location of this stream
 ==========================================================================*/
extern SINT8 SsrwTellVCPP(FILE* in_pFile)
{
    fpos_t fp;
    return (fgetpos(in_pFile, &fp) == 0) ? fp : -1;
}
#endif /* SSTG_DEFINE_FSEEK64_VCPP */

#if defined(SSTG_DEFINE_FTRUNCATE_VCPP) /* Visual C++ has its own way of 
                                         * truncating files.  It can't truncate
                                         * files to sizes larger than 4GB.  */
SINT4 SsrwFTruncate64VCPP(FILE* io_pFile, SINT8 in_llNewLength)
{
    if (in_llNewLength <= 0x07fffffff)
    {
        fflush(io_pFile);
        return _chsize(_fileno(io_pFile), (SINT4)in_llNewLength);
    }
    return -1; /* There is no way to truncate a file to a size larger than
                * 2GB using the VC++ standard library */
}
#elif defined(SSTG_DEFINE_FTRUNCATE64)  /* An extension available on some 
                                         * UNIX-like OSes */
SINT4 SsrwFTruncate64(FILE* io_pFile, SINT8 in_llNewLength)
{
    fflush(io_pFile);
    return ftruncate64(fileno(io_pFile), (off64_t)in_llNewLength);
}
#elif defined (SSTG_DEFINE_FTRUNCATE64_IF_POSSIBLE)                                  
                                        /* Uses a more common UNIX-like OS 
                                         * extension */
SINT4 SsrwFTruncate64IfPossible(FILE* io_pFile, SINT8 in_llNewLength)
{
    if (in_llNewLength < 0x07fffffff)
    {
        fflush(io_pFile);
        return ftruncate(fileno(io_pFile), (SINT4)in_llNewLength);
    }
    return -1; /* There is no way to truncate a file to a size larger than
                * 2GB using this extension */
}
#endif /* SSTG_DEFINE_FTRUNCATE64_IF_POSSIBLE */

#ifdef SSTG_USE_FALSE_WFOPEN
/*============================================================================
  Description:  Opens a file using a wide filename string.  This function is
                just like wfopen(), which is not universally available.

  Arguments: 
   io_pwFilename -  name of file to open
   in_pwMode -      fopen-style file access mode string, as a wchar_t*
  Return:
   NULL on failure, otherwise a pointer to a newly-created FILE structure
 ==========================================================================*/
FILE* 
SsrwFalseWFopen(const wchar_t* in_pwFilename, 
                const wchar_t* in_pwMode)
{
    UINT4 ulFilenameLen =       0;
    UINT4 ulModeLen =           0;
    char* pFilename =           NULL;
    char* pMode =               NULL;
    FILE* pResult =             NULL;

    /* First convert the filename and mode to narrow strings using the best
     * available method */
    ulFilenameLen = wcslen(in_pwFilename)*sizeof(wchar_t);
    ulModeLen = wcslen(in_pwMode);
    pFilename = (char*)malloc(ulFilenameLen + 1);
    if (pFilename == NULL)
    {
        return NULL;
    }
    pMode = (char*)malloc(ulFilenameLen + 1);
    if (pMode == NULL)
    {
        free(pFilename);
        return NULL;
    }
    wcstombs(pFilename, in_pwFilename, ulFilenameLen + 1);
    wcstombs(pMode, in_pwMode, ulModeLen + 1);

    /* Then attempt to open the file */
    pResult = fopen(pFilename, pMode);

    /* Finally, clean up */
    free(pFilename);
    free(pMode);

    return pResult;
}
#endif /* SSTG_USE_FALSE_WFOPEN */

#ifdef SSTG_USE_FALSE_WREMOVE
/*============================================================================
  Description:  Removes (deletes, unlinks) a file using a wide filename string.  
                This function is just like wremove(), which is not universally 
                available.

  Arguments: 
   io_pwFilename -  name of file to delete
  Return:
   0 on success, nonzero value on failure
 ==========================================================================*/
int
SsrwFalseWRemove(const wchar_t* in_pwFilename)
{
    UINT4 ulFilenameLen =       0;
    char* pFilename =           NULL;
    SINT4 lResult =             0;

    /* First convert the filename to a narrow string using the best
     * available method */
    ulFilenameLen = wcslen(in_pwFilename);
    pFilename = (char*)malloc(ulFilenameLen + 1);
    if (pFilename == NULL)
    {
        return -1;
    }
    wcstombs(pFilename, in_pwFilename, ulFilenameLen + 1);

    /* Then attempt to open the file */
    lResult = (SINT4)remove(pFilename);

    /* Finally, clean up */
    free(pFilename);

    return (int)lResult;
}
#endif /* SSTG_USE_FALSE_WREMOVE */

#ifdef DEBUG
SINT4 
commonSelfTest(void)
{
#ifdef SSTG_DEFINE_WINTIME
    {
        time_t  tt1 = 0x3e6543c6; /* March 4, 2003, around 4:20 PST */
        time_t  tt2 = 0;
        WinTime tW;

        WindowsTimeFromTime_t(tt1, &tW);
        time_tFromWindowsTime(&tW, &tt2);
        ASSERT(tt1 == tt2);
    }
#endif /* SSTG_DEFINE_WINTIME */
    return SSTG_OK;
}
#endif /* DEBUG */

