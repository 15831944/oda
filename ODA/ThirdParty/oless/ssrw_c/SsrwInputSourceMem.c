/**
* 
*  SsrwInputSourceMem.c
*  
*  Implements the SsrwInputSource abstraction using a memory buffer rather 
*  than a file
* 
*  Copyright (C) 2002 - 2004 Schema Software, Inc. All rights reserved.
*
*  $Revision: 1.3.2.5 $
*  $Date: 2004/02/26 19:20:30 $
*  
*/

#include <string.h>
#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "SsrwInputSourceMem.h"
#include "SsrwInputSourceCommon.h"

size_t SsrwMemFread(
    void*   in_pv,
    size_t  in_size,
    size_t  in_num,
    SSRWIS* in_pIS)
{
    SINT8               llBytesRemaining =  0;
    size_t              realNum =           in_num;
    size_t              realBytes =         in_num * in_size;
    char*               pcBuf =             0;
    SsrwMemFileInfo*    pMemInfo =          (SsrwMemFileInfo*)(in_pIS->m_pSrc);

    ASSERT(in_pIS != NULL);

    /* Quit if this buffer is not open for writing */
    if ((pMemInfo->m_isReadable) == 0)
    {
        return 0;
    }

    llBytesRemaining = pMemInfo->m_llLengthAllocated -
                       pMemInfo->m_llSeekPos;
    if (llBytesRemaining < 0)
    {
        llBytesRemaining = 0;
    }
    if ((long)(realBytes) > llBytesRemaining)
    {
        realNum = (size_t)(llBytesRemaining / ((long)in_size));
        realBytes = ((size_t)realNum) * in_size;
    }

    if (realBytes > 0)
    {
        pcBuf = (char*)*(pMemInfo->m_ppBuf);
        memcpy( in_pv,
                pcBuf + pMemInfo->m_llSeekPos,
                realBytes);
        pMemInfo->m_llSeekPos += (long)realBytes;
    }

    return realNum;
}


size_t SsrwMemFwrite(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS)
{
    long                lMinLength =        0;
    long                lNewSize =          0;
    size_t              realNum =           in_num;
    void*               pNewBuf =           NULL;
    size_t              realBytes =         in_num * in_size;
    char*               pcBuf =             0;
    SINT8               llBytesRemaining =  0;
    SsrwMemFileInfo*    pMemInfo =          (SsrwMemFileInfo*)(in_pIS->m_pSrc);

    ASSERT(in_pv != NULL);
    ASSERT(in_pIS != NULL);

    /* Quit if this buffer is not open for writing */
    if ((pMemInfo->m_isWritable) == 0)
    {
        return 0;
    }

    /* Start by resizing the buffer to be at least big enough if possible */
    lNewSize = (long)(pMemInfo->m_llLengthAllocated);
    lMinLength = (long)(pMemInfo->m_llLengthUsed + in_size * in_num);
    while(lMinLength > lNewSize)
    {
        lNewSize *= 2;
        if (lNewSize < 1024)
        {
            lNewSize = 1024;
        }
    }

    if (lNewSize != pMemInfo->m_llLengthAllocated)
    {
        pNewBuf = pMemInfo->m_pRealloc(*(pMemInfo->m_ppBuf), lNewSize);
        if (pNewBuf != NULL)
        {
            *(pMemInfo->m_ppBuf) = pNewBuf;
            pMemInfo->m_llLengthAllocated = lNewSize;
        }
    }


    /* Now write however much of the data will fit in the buffer */
    llBytesRemaining = pMemInfo->m_llLengthAllocated -
                       pMemInfo->m_llSeekPos;
    if ((long)(realBytes) > llBytesRemaining)
    {
        realNum = (long)(llBytesRemaining / ((long)in_size));
        realBytes = ((size_t)realNum) * in_size;
    }

    pcBuf = (char*)*(pMemInfo->m_ppBuf);
    memcpy( pcBuf + pMemInfo->m_llSeekPos,
            in_pv,
            realBytes);
    pMemInfo->m_llSeekPos += (long)realBytes;
    if (pMemInfo->m_llSeekPos > pMemInfo->m_llLengthUsed)
    {
        pMemInfo->m_llLengthUsed = pMemInfo->m_llSeekPos;
    }

    return realNum;
}


int SsrwMemFclose(
    SSRWIS* in_pIS)
{
    SsrwMemFileInfo*    pMemInfo =      (SsrwMemFileInfo*)(in_pIS->m_pSrc);
    void*               pNewBuf =       NULL;

    ASSERT(in_pIS != NULL);
    ASSERT((pMemInfo->m_isWritable == 0) || /* read */
           (pMemInfo->m_pRealloc != NULL)); /* write */

    if (pMemInfo->m_llLengthUsed != pMemInfo->m_llLengthAllocated)
    {
        pNewBuf = pMemInfo->m_pRealloc(
                      *(pMemInfo->m_ppBuf),
                      (size_t)(pMemInfo->m_llLengthUsed));
        if (pNewBuf != NULL)
        {
            *(pMemInfo->m_ppBuf) = pNewBuf;
            pMemInfo->m_llLengthAllocated = pMemInfo->m_llLengthUsed;
        }
    }

    free(in_pIS->m_pSrc);

    return 0;
}


int SsrwMemFseek(
    SSRWIS* in_pIS,
    SINT8   in_llOffset,
    int     in_whence)
{
    SsrwMemFileInfo*    pMemInfo =      (SsrwMemFileInfo*)(in_pIS->m_pSrc);
    SINT8               llNewSeekPos =  pMemInfo->m_llSeekPos;

    ASSERT(in_pIS != NULL);
    ASSERT((in_whence == SEEK_SET) ||
           (in_whence == SEEK_CUR) ||
           (in_whence == SEEK_END));

    switch(in_whence)
    {
    case SEEK_SET:
        llNewSeekPos = in_llOffset;
        break;
    case SEEK_CUR:
        llNewSeekPos = pMemInfo->m_llSeekPos + in_llOffset;
        break;
    case SEEK_END:
        llNewSeekPos = pMemInfo->m_llLengthUsed + in_llOffset;
        break;
    }

    pMemInfo->m_llSeekPos = llNewSeekPos;

    return 0;
}


SINT8 SsrwMemFtell(
    SSRWIS* in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwMemFileInfo*)(in_pIS->m_pSrc))->m_ppBuf != NULL);

    return (SINT8)((SsrwMemFileInfo*)(in_pIS->m_pSrc))->m_llSeekPos;
}


long SsrwMemFtruncate(
    SSRWIS* in_pIS,
    SINT8   in_llOffset)
{
    SsrwMemFileInfo*    pMemInfo =      (SsrwMemFileInfo*)(in_pIS->m_pSrc);

    pMemInfo->m_llLengthUsed = in_llOffset;

    return 0;
}


SSRWIS* SsrwOpenMemForReading(
    const void**    in_ppvBuf,
    unsigned long   in_ulLength)
{
    SSRWIS* pIS = NULL;

    pIS = (SSRWIS*)malloc(sizeof(SSRWIS));

    if (pIS != NULL)
    {
        memset( pIS, 0, sizeof(SSRWIS));
        pIS->m_pSrc = malloc(sizeof(SsrwMemFileInfo));
        if (pIS->m_pSrc != NULL)
        {
            memset(pIS->m_pSrc, 0, sizeof(SsrwMemFileInfo));

            /* Initialize function pointers */
            pIS->m_pFread = &SsrwMemFread;
            pIS->m_pFwrite = &SsrwMemFwrite;
            pIS->m_pFclose = &SsrwMemFclose;
            pIS->m_pFseek = &SsrwMemFseek;
            pIS->m_pFtell = &SsrwMemFtell;
            pIS->m_pFtruncate = &SsrwMemFtruncate;

            /* Initialize in-memory file emulation structures */
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_ppBuf = (void**)in_ppvBuf;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_llLengthUsed = in_ulLength;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_llLengthAllocated = in_ulLength;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_llSeekPos = 0;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_pRealloc = NULL;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_isReadable = 1;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_isWritable = 0;
        }
        else
        {
            /* Error */
            free(pIS);
            pIS = NULL;
        }
    }

    return pIS;
}


SSRWIS* SsrwOpenMemForWriting(
    void**                  in_ppvBuf,
    unsigned long           in_ulLength,
    Ssrw_Func_Realloc       in_pRealloc)
{
    SSRWIS* pIS = NULL;

    pIS = (SSRWIS*)malloc(sizeof(SSRWIS));

    if (pIS != NULL)
    {
        memset( pIS, 0, sizeof(SSRWIS));
        pIS->m_pSrc = malloc(sizeof(SsrwMemFileInfo));
        if (pIS->m_pSrc != NULL)
        {
            memset(pIS->m_pSrc, 0, sizeof(SsrwMemFileInfo));

            /* Initialize function pointers */
            pIS->m_pFread = &SsrwMemFread;
            pIS->m_pFwrite = &SsrwMemFwrite;
            pIS->m_pFclose = &SsrwMemFclose;
            pIS->m_pFseek = &SsrwMemFseek;
            pIS->m_pFtell = &SsrwMemFtell;
            pIS->m_pFtruncate = &SsrwMemFtruncate;

            /* Initialize in-memory file emulation structures */
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_ppBuf = in_ppvBuf;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_llLengthUsed = 0;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_llLengthAllocated = in_ulLength;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_llSeekPos = 0;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_pRealloc = in_pRealloc;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_isReadable = 0;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_isWritable = 1;
        }
        else
        {
            /* Error */
            free(pIS);
            pIS = NULL;
        }
    }

    return pIS;
}


SSRWIS* SsrwOpenMemForReadWrite(
    void**                  in_ppvBuf,
    unsigned long           in_ulLength,
    Ssrw_Func_Realloc       in_pRealloc)
{
    SSRWIS* pIS = NULL;

    pIS = (SSRWIS*)malloc(sizeof(SSRWIS));

    if (pIS != NULL)
    {
        memset( pIS, 0, sizeof(SSRWIS));
        pIS->m_pSrc = malloc(sizeof(SsrwMemFileInfo));
        if (pIS->m_pSrc != NULL)
        {
            memset(pIS->m_pSrc, 0, sizeof(SsrwMemFileInfo));

            /* Initialize function pointers */
            pIS->m_pFread = &SsrwMemFread;
            pIS->m_pFwrite = &SsrwMemFwrite;
            pIS->m_pFclose = &SsrwMemFclose;
            pIS->m_pFseek = &SsrwMemFseek;
            pIS->m_pFtell = &SsrwMemFtell;
            pIS->m_pFtruncate = &SsrwMemFtruncate;

            /* Initialize in-memory file emulation structures */
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_ppBuf = in_ppvBuf;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_llLengthUsed = 0;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_llLengthAllocated = in_ulLength;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_llSeekPos = 0;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_pRealloc = in_pRealloc;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_isReadable = 1;
            ((SsrwMemFileInfo*)(pIS->m_pSrc))->m_isWritable = 1;
        }
        else
        {
            /* Error */
            free(pIS);
            pIS = NULL;
        }
    }

    return pIS;
}


