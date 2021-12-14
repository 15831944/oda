/******************************************************************************\
* 
*  SsrwInputSource.c
*  
*  Provides an abstraction from the C file I/O functions
* 
*  Copyright (C) 2002 - 2003 Schema Software, Inc. All rights reserved.
*
*  $Revision: 1.4.2.4 $
*  $Date: 2004/02/26 19:20:30 $
*  
\******************************************************************************/

#include <string.h>
#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "OlessExport.h"
#include "StructuredStorage.h"
#include "SsrwInputSourceCommon.h"
#include "SsrwInputSourceMem.h"
#include "SsrwInputSourceFile.h"

size_t SsrwFread(
    void*   in_pv,
    size_t  in_size,
    size_t  in_num,
    SSRWIS* in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(in_pIS->m_pFread != NULL);

    return in_pIS->m_pFread(in_pv, in_size, in_num, in_pIS);
}


size_t SsrwFwrite(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(in_pIS->m_pFwrite != NULL);

    return in_pIS->m_pFwrite(in_pv, in_size, in_num, in_pIS);
}


int SsrwFclose(
    SSRWIS* in_pIS)
{
    int result = 0;

    ASSERT(in_pIS != NULL);
    ASSERT(in_pIS->m_pSrc != NULL);
    ASSERT(in_pIS->m_pFclose != NULL);

    result = in_pIS->m_pFclose(in_pIS);

    free(in_pIS);
    in_pIS = NULL;

    return result;
}


int SsrwFseek(
    SSRWIS* in_pIS,
    SINT8   in_llOffset,
    int     in_nWhence)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pIS->m_pFseek != NULL);

    return in_pIS->m_pFseek(in_pIS, in_llOffset, in_nWhence);
}


SINT8 SsrwFtell(
    SSRWIS* in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pIS->m_pFtell != NULL);

    return in_pIS->m_pFtell(in_pIS);
}


long SsrwFtruncate(
    SSRWIS* in_pIS,
    SINT8   in_llNewSize)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pIS->m_pFtruncate != NULL);

    return in_pIS->m_pFtruncate(in_pIS, in_llNewSize);
}


int SsrwDisconnect(SSRWIS* in_pIS)
{
    ASSERT(in_pIS != NULL);

    if (in_pIS->m_pSrc != NULL)
    {
        free(in_pIS->m_pSrc);
    }
    free(in_pIS);
    in_pIS = NULL;

    return 0;
}

#ifdef DEBUG
SINT4 SSRWISSelfTest(void)
{
    SSRWIS*     pIS[2] =            { NULL, NULL };
    FILE*       pFile =             NULL;
    const char* pDataBuf =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    char        pTmpBuf[2][63];
    char*       pWriteBuf =         NULL;
    int         rc =                0;
    size_t      sTmp =              0;
    SINT8       llTmp =             0;
    long        i =                 0;


    pFile = fopen("RemoveMe.txt", "wb");
    ASSERT(pFile != NULL);
    ASSERT(fwrite(pDataBuf, strlen(pDataBuf), 1, pFile) == 1);
    fclose(pFile);

    /* Test opening for reading */
    pIS[0] = SsrwOpenFile(L"RemoveMe.txt", L"rb");
    pIS[1] = SsrwOpenMemForReading((const void**)&pDataBuf, strlen(pDataBuf));
    ASSERT((pIS[0] != NULL) &&
           (pIS[1] != NULL));

    /* Read-only tests */
    for (i = 0; i < 2; i++)
    {
        memset(pTmpBuf[i], 0, 63);
        sTmp = SsrwFread(pTmpBuf[i], 5, 1, pIS[i]);
        ASSERT(sTmp == 1);
        ASSERT(memcmp(pTmpBuf[i], pDataBuf, 5) == 0);

        rc = SsrwFseek(pIS[i], 0, SEEK_SET);
        ASSERT(rc == 0);
        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == 0);

        memset(pTmpBuf[i], 0, 63);
        sTmp = SsrwFread(pTmpBuf[i], 7, 1, pIS[i]);
        ASSERT(sTmp == 1);
        ASSERT(memcmp(pTmpBuf[i], pDataBuf, 7) == 0);

        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == 7);

        memset(pTmpBuf[i], 0, 63);
        sTmp = SsrwFread(pTmpBuf[i], 3, 1, pIS[i]);
        ASSERT(sTmp == 1);
        ASSERT(memcmp(pTmpBuf[i], &pDataBuf[7], 3) == 0);

        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == 10);

        rc = SsrwFseek(pIS[i], 3, SEEK_CUR);
        ASSERT(rc == 0);
        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == 13);

        memset(pTmpBuf[i], 0, 63);
        sTmp = SsrwFread(pTmpBuf[i], 3, 1, pIS[i]);
        ASSERT(sTmp == 1);
        ASSERT(memcmp(pTmpBuf[i], &pDataBuf[13], 3) == 0);

        rc = SsrwFseek(pIS[i], -5, SEEK_END);
        ASSERT(rc == 0);
        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == 57);

        sTmp = SsrwFread(pTmpBuf[i], 1, 6, pIS[i]);
        ASSERT(sTmp == 5);
        ASSERT(memcmp(pTmpBuf[i], &pDataBuf[57], 5) == 0);

        rc = SsrwFclose(pIS[i]);
        ASSERT(rc == 0);
    }

    /* Get rid of the temporary file */
    remove("RemoveMe.txt");

    /* Test opening for writing */
    pIS[0] = SsrwOpenFile(L"RemoveMe.txt", L"wb");
    pWriteBuf = (char*)malloc(100);
    memset(pWriteBuf, 0, 100);
    pIS[1] = SsrwOpenMemForWriting((void**)&pWriteBuf, 100, &realloc);
    ASSERT((pIS[0] != NULL) &&
           (pIS[1] != NULL));


    /* Write-only tests */
    for (i = 0; i < 2; i++)
    {
        rc = SsrwFseek(pIS[i], 0, SEEK_END);
        ASSERT(rc == 0);
        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == 0);

        sTmp = SsrwFwrite(pDataBuf, strlen(pDataBuf), 1, pIS[i]);
        ASSERT(sTmp == 1);
        sTmp = SsrwFwrite(pDataBuf, strlen(pDataBuf), 1, pIS[i]);
        ASSERT(sTmp == 1);

        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == (long)(2 * strlen(pDataBuf)));

        rc = SsrwFseek(pIS[i], 0, SEEK_SET);
        ASSERT(rc == 0);
        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == 0);

        memset(pTmpBuf[i], 0, 63);
        sTmp = SsrwFread(pTmpBuf[i], 3, 1, pIS[i]);
        ASSERT(sTmp == 0);
        ASSERT(pTmpBuf[i][0] == 0);
        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == 0);

        rc = SsrwFseek(pIS[i], 3, SEEK_CUR);
        ASSERT(rc == 0);
        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == 3);

        sTmp = SsrwFwrite(pDataBuf, strlen(pDataBuf), 1, pIS[i]);
        ASSERT(sTmp == 1);
        sTmp = SsrwFwrite(pDataBuf, strlen(pDataBuf), 1, pIS[i]);
        ASSERT(sTmp == 1);

        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == (long)(3 + 2 * strlen(pDataBuf)));

        rc = SsrwFseek(pIS[i], -5, SEEK_END);
        ASSERT(rc == 0);
        llTmp = SsrwFtell(pIS[i]);
        ASSERT(llTmp == (long)(2 * strlen(pDataBuf) - 2));

        sTmp = SsrwFwrite(pDataBuf, 10, 1, pIS[i]);
        ASSERT(sTmp == 1);

        rc = SsrwFclose(pIS[i]);
        ASSERT(rc == 0);

    }

    /* Remove test output file */
    remove("RemoveMe.txt");

    /* Test the write buffer for correctness, then free it */
    ASSERT(memcmp(pWriteBuf, "ABCABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234ABCDEFGHIJ", 132) == 0);
    free(pWriteBuf);
    pWriteBuf = NULL;

    return SSTG_OK;
}

#endif /* DEBUG */
