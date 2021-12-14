/************************************************************************
* Debug.c
*
* Self-test stuff, debug-only stuff
*
* (c) Schema Software Inc., 2001
* 
************************************************************************

$Revision: 1.6.2.2 $
$Date: 2004/02/24 19:09:25 $
*/
#if defined(_DEBUG) && defined(_MSC_VER)
	#define _CRTDBG_MAP_ALLOC
#endif /* _DEBUG && _MSC_VER */

#include <stdlib.h>

#if defined(_DEBUG) && defined(_MSC_VER) && !defined(_WIN32_WCE)
	#include <crtdbg.h>
#endif /* _DEBUG && _MSC_VER */

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "StructuredStorage.h"
#include "RootStorage.h"
#include "Directory.h"
#include "Stream.h"
#include "Storage.h"
#include "OpenList.h"
#include "SsrwInputSource.h"
#include "Fat.h"

SINT8 s_llBytesAllocated =    0;
SINT8 s_llBytesAllocatedMax = 0;

#ifdef DEBUG
/*============================================================================
  Description: To be used in DEBUG builds only.  Runs a couple of quick self-
               tests on the structured storage library.
  Return: 
   Status code
  ==========================================================================*/
SINT4
structuredStorageSelfTest()
{
    SINT4 rc = SSTG_OK;

    rc = commonSelfTest();
    if (rc != SSTG_OK)
    {
        return rc;
    }

    rc = headerSelfTest();
    if (rc != SSTG_OK)
    {
        return rc;
    }

    rc = openListSelfTest();
    if (rc != SSTG_OK)
    {
        return rc;
    }

    rc = directorySelfTest();
    if (rc != SSTG_OK)
    {
        return rc;
    }

	rc = tocSelfTest();
    if (rc != SSTG_OK)
    {
        return rc;
    }

    rc = storageSelfTest();
    if (rc != SSTG_OK)
    {
        return rc;
    }

    rc = streamSelfTest();
    if (rc != SSTG_OK)
    {
        return rc;
    }

    rc = SSRWISSelfTest();
    if (rc != SSTG_OK)
    {
        return rc;
    }

    rc = fatSelfTest();
    if (rc != SSTG_OK)
    {
        return rc;
    }

    return SSTG_OK;
}


#endif /* DEBUG */


#ifdef DEBUG
void _AssertProc(CHAR* cond, CHAR* file, SINT4 line)
{
    fprintf(stderr, "Assert failed: \"%s\" (%s l.%d)", cond, file, (int)line);
#ifdef _MSC_VER
    /* Trip the debugger */
    __asm { __asm int 3 }
#else /* _MSC_VER */
    /* Intentionally segfault */
    *((char*)0) = 5;
#endif /* _MSC_VER */
}


/**
 * Intentionally leak a specified number of bytes.  This function is used to
 * test the leak checking mechanisms that are in place.
 *
 * @param in_ulBytes - number of bytes to leak 
 * @return Status code
 */
SINT4 
structuredStorageIntentionallyLeak(unsigned long in_ulBytes)
{
    malloc(in_ulBytes);

    return SSTG_OK;
}


#undef malloc
#undef free
#undef realloc
#undef calloc

/* This code written on the assumption that sizeof(UINT4) = 4.
 * Yes, a good assumption. */

UINT4 GuardBytes = 0xdddddddd;
BYTE TrashByte = 0xcc;

/*============================================================================
  Description: To be used in DEBUG builds only.  Allocates a buffer with 
               guard bytes and a bytecount.
               (replaces malloc)
  Arguments: 
   cb - number of bytes to allocate
  Return: 
   NULL on failure; new buffer on success
  ==========================================================================*/
void* 
dbgMalloc(size_t cb)
{
    size_t  cbRealSize =    0;
    BYTE*   pb =            NULL;

    cbRealSize = (((cb + 3) / 4) * 4) + 12;
    pb = (BYTE*)malloc(cbRealSize);
    if (pb == NULL)
    {
        return NULL;
    }
    memset(pb, TrashByte, cbRealSize);
    *(UINT4*)pb = (UINT4)cb;
    *(((UINT4*)pb) + 1) = GuardBytes;
    *(((UINT4*)pb) + (cbRealSize / 4) - 1) = GuardBytes;

    s_llBytesAllocated += (SINT8)cb;
    if (s_llBytesAllocated > s_llBytesAllocatedMax)
    {
        s_llBytesAllocatedMax = s_llBytesAllocated;
    }

    return (void*)(pb + (2 * sizeof(UINT4)));
}


/*============================================================================
  Description: To be used in DEBUG builds only.  Frees a buffer that has been
               allocated with guard bytes and a bytecount.
               (replaces free)
  Arguments: 
   pb - pointer to buffer to free
  ==========================================================================*/
void 
dbgFree(void* pb)
{
    size_t  cb =            0;
    UINT4   cbRealSize =    0;
    BYTE*   pbReal =        NULL;
    UINT4   ulGB1 =         0;
    UINT4   ulGB2 =         0;

    cb = *(((UINT4*)pb) - 2);
    cbRealSize = (((cb + 3) / 4) * 4) + 12;

    ulGB1 = *(((UINT4*)pb) - 1);
    ulGB2 = *(((UINT4*)pb) + (cbRealSize / 4) - 3);
    ASSERT((ulGB1 == GuardBytes) && (ulGB2 == GuardBytes));

    pbReal = (((BYTE*)pb) - 8);
    memset(pbReal, TrashByte, cbRealSize);
    free(pbReal);

    s_llBytesAllocated -= (SINT8)cb;
}


/*============================================================================
  Description: To be used in DEBUG builds only.  Resizes a buffer with 
               guard bytes and a bytecount.
               (replaces realloc)
  Arguments: 
   pbOrig - original buffer to reallocate
   cb -     new number of bytes to allocate
  Return: 
   NULL on failure; new buffer on success
  ==========================================================================*/
void* 
dbgRealloc(void* pbOrig, size_t cb)
{
    UINT4 cbToCopy =    0;
    UINT4 cbOrig =      0;
    BYTE* pbNew =       NULL;

    pbNew = (BYTE*)dbgMalloc(cb);
    if (pbNew != NULL && pbOrig != NULL)
    {
        cbOrig = *(((UINT4*)pbOrig) - 2);
        cbToCopy = min(cb, cbOrig);
        memcpy(pbNew, pbOrig, cbToCopy);
        dbgFree(pbOrig);
    }

    return pbNew;
}


/*============================================================================
  Description: To be used in DEBUG builds only.  Allocates a buffer with
               guard bytes and a bytecount and initializes it to 0's.
               (replaces calloc)
  Arguments: 
   c - number of items to allocate
   b - size of each item
  Return: 
   NULL on failure; new buffer on success
  ==========================================================================*/
void* 
dbgCalloc(size_t c, size_t b)
{
    void* pb = NULL;

    pb = dbgMalloc(b * c);
    if (pb != NULL)
    {
        memset(pb, 0, b * c);
    }

    return pb;
}


/*============================================================================
  Description: To be used in DEBUG builds only.  Returns some statistics
               about debug heap usage to the caller.
  Arguments: 
   out_pllBytesAllocated - number of bytes currently allocated by the debug
                           heap functions
   out_pllBytesAllocatedMax - the peak value of out_pllBytesAllocated, when
                              the greatest number of bytes were allocated.
  Return: 
   Status code
  ==========================================================================*/
SINT4 
dbgMemStats(SINT8* out_pllBytesAllocated,
            SINT8* out_pllBytesAllocatedMax)
{
    *out_pllBytesAllocated = s_llBytesAllocated;
    *out_pllBytesAllocatedMax = s_llBytesAllocatedMax;

    return SSTG_OK;
}

#endif /* DEBUG */

