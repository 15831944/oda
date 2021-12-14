/******************************************************************************\
* 
*  SsrwInputSourceFile.c
*  
*  Passes calls from the SsrwInputSource abstraction down to the real C FILE* 
*  functions
* 
*  Copyright (C) 2002 - 2003 Schema Software, Inc. All rights reserved.
*
*  $Revision: 1.5.2.4 $
*  $Date: 2004/02/26 19:20:30 $
*  
\******************************************************************************/

#ifndef SSTG_PALMOS

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "SsrwInputSourceFile.h"
#include "SsrwInputSourceCommon.h"

size_t SsrwFileFread(
    void*   in_pv,
    size_t  in_size,
    size_t  in_num,
    SSRWIS* in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile != NULL);

    return fread(
        in_pv, 
        in_size, 
        in_num, 
        ((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile);
}


size_t SsrwFileFwrite(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile != NULL);

    return fwrite(
        in_pv, 
        in_size, 
        in_num, 
        ((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile);
}


int SsrwFileFclose(
    SSRWIS* in_pIS)
{
    int result = 0;

    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile != NULL);
    ASSERT(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_isOwnedBySSRWIS != 0);

    result = fclose(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile);

    free(in_pIS->m_pSrc);

    return result;
}


int SsrwFileFseek(
    SSRWIS* in_pIS,
    SINT8   in_llOffset,
    int     in_nWhence)
{
    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile != NULL);

    return (int)SSRW_SEEK(
        ((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile, 
        in_llOffset, 
        in_nWhence);
}


SINT8 SsrwFileFtell(
    SSRWIS* in_pIS)
{
    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile != NULL);

    return SSRW_TELL(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile);
}


long SsrwFileFtruncate(
    SSRWIS* in_pIS,
    SINT8   in_llNewSize)
{
    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile != NULL);

    return SSRW_FTRUNCATE(((SsrwRealFileInfo*)(in_pIS->m_pSrc))->m_pFile, 
                          in_llNewSize);
}


SSRWIS* SsrwOpenFile(const wchar_t* path, const wchar_t* mode)
{
    SSRWIS*     pIS = NULL;

    pIS = (SSRWIS*)malloc(sizeof(SSRWIS));
    if (pIS != NULL)
    {
        memset( pIS, 0, sizeof(SSRWIS));
        pIS->m_pSrc = malloc(sizeof(SsrwRealFileInfo));
        if (pIS->m_pSrc != NULL)
        {
            memset(pIS->m_pSrc, 0, sizeof(SsrwRealFileInfo));

            /* Initialize function pointers */
            pIS->m_pFread = &SsrwFileFread;
            pIS->m_pFwrite = &SsrwFileFwrite;
            pIS->m_pFclose = &SsrwFileFclose;
            pIS->m_pFseek = &SsrwFileFseek;
            pIS->m_pFtell = &SsrwFileFtell;
            pIS->m_pFtruncate = &SsrwFileFtruncate;

            /* Try to open the file */
            ((SsrwRealFileInfo*)(pIS->m_pSrc))->m_isOwnedBySSRWIS = 1;
            ((SsrwRealFileInfo*)(pIS->m_pSrc))->m_pFile = 
                SSRW_WFOPEN(path, mode);
            if (((SsrwRealFileInfo*)(pIS->m_pSrc))->m_pFile == NULL)
            {
                /* Error */
                free(pIS->m_pSrc);
                free(pIS);
                pIS = NULL;
            }
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


SSRWIS* SsrwConnectToExternalFile(FILE* in_pFile)
{
    SSRWIS*     pIS = NULL;

    pIS = (SSRWIS*)malloc(sizeof(SSRWIS));
    if (pIS != NULL)
    {
        memset( pIS, 0, sizeof(SSRWIS));
        pIS->m_pSrc = malloc(sizeof(SsrwRealFileInfo));
        if (pIS->m_pSrc != NULL)
        {
            memset(pIS->m_pSrc, 0, sizeof(SsrwRealFileInfo));

            /* Initialize function pointers */
            pIS->m_pFread = &SsrwFileFread;
            pIS->m_pFwrite = &SsrwFileFwrite;
            pIS->m_pFclose = &SsrwFileFclose;
            pIS->m_pFseek = &SsrwFileFseek;
            pIS->m_pFtell = &SsrwFileFtell;
            pIS->m_pFtruncate = &SsrwFileFtruncate;

            ((SsrwRealFileInfo*)(pIS->m_pSrc))->m_isOwnedBySSRWIS = 0;
            ((SsrwRealFileInfo*)(pIS->m_pSrc))->m_pFile = in_pFile;
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

#endif /* !SSTG_PALMOS */
