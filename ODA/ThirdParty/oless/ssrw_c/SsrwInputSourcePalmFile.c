/******************************************************************************\
* 
*  SsrwInputSourcePalmFile.c
*  
*  Passes calls from the SsrwInputSource abstraction down to the PalmOS file 
*  I/O functions
* 
*  Copyright (C) 2002 - 2004 Schema Software, Inc. All rights reserved.
*
*  $Revision: 1.4.2.3 $
*  $Date: 2004/02/26 19:20:30 $
*  
\******************************************************************************/

#ifdef SSTG_PALMOS

#include <string.h>
#include <PalmOS.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "SsrwInputSourcePalmFile.h"
#include "SsrwInputSourceCommon.h"

size_t SsrwPalmFileFread(
    void*   in_pv,
    size_t  in_size,
    size_t  in_num,
    SSRWIS* in_pIS)
{
    Err errP = 0;

    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file != NULL);

    return FileRead(
        ((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file, 
        in_pv, 
        in_size, 
        in_num, 
        &errP);
}


size_t SsrwPalmFileFwrite(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS)
{
    Err errP = 0;

    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file != 0);

    return FileWrite(
        ((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file, 
        in_pv, i
        n_size, 
        in_num, 
        &errP);
}


int SsrwPalmFileFclose(
    SSRWIS* in_pIS)
{
    unsigned short unResult = 0;

    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file != 0);
    ASSERT(((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_isOwnedBySSRWIS != 0);

    unResult = FileClose(
        ((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file);

    free(in_pIS->m_pSrc);

    return unResult;
}


int SsrwPalmFileFseek(
    SSRWIS* in_pIS,
    SINT8   in_llOffset,
    int     in_nWhence)
{
    FileOriginEnum palmOrigin = fileOriginBeginning;

    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file != NULL);

    switch(in_nWhence)
    {
    case SSRWIS_SEEK_SET:
        palmOrigin = fileOriginBeginning;
        break;
    case SSRWIS_SEEK_CUR:
        palmOrigin = fileOriginCurrent;
        break;
    case SSRWIS_SEEK_END:
        palmOrigin = fileOriginEnd;
        break;
    default:
        ASSERT(0);
        break;
    }

    return FileSeek(
        ((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file,
        (long)in_llOffset,
        palmOrigin);
}


SINT8 SsrwPalmFileFtell(
    SSRWIS* in_pIS)
{
    Int32 fileSizeP = 0; /* unused */
    Err   errP =      0;

    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file != NULL);

    return FileTell(
        ((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file, 
        &fileSizeP, 
        &errP);
}


SINT4 SsrwPalmFileFtruncate(
    SSRWIS* in_pIS,
    SINT8   in_llNewSize)
{
    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file != NULL);

    return FileTruncate(
        ((SsrwPalmFileInfo*)(in_pIS->m_pSrc))->m_file, 
        (Int32)in_llNewSize);
}


SSRWIS* SsrwOpenFile(const char* in_pPath, const char* in_pMode)
{
    SSRWIS*     pIS =       NULL;
    Err         errP =      0;
    UInt32      palmMode =  0;
    FileHand    file =      0;

    /* Convert the file mode */
    if ((strcmp(in_pMode, "w+") == 0)||
            (strcmp(in_pMode, "w") == 0) ||
            (strcmp(in_pMode, "wb") == 0))
    {
        palmMode = fileModeReadWrite;
    }
    else if ((strcmp(in_pMode, "r") == 0) ||
             (strcmp(in_pMode, "rb") == 0))
    {
        palmMode = fileModeReadOnly;
    }
    else
    {
        ASSERT(0);
        return NULL;
    }

    pIS = malloc(sizeof(SSRWIS));
    if (pIS != NULL)
    {
        memset( pIS, 0, sizeof(SSRWIS));
        pIS->m_pSrc = malloc(sizeof(SsrwPalmFileInfo));
        if (pIS->m_pSrc != NULL)
        {
            memset(pIS->m_pSrc, 0, sizeof(SsrwPalmFileInfo));

            /* Initialize function pointers */
            pIS->m_pFread = &SsrwPalmFileFread;
            pIS->m_pFwrite = &SsrwPalmFileFwrite;
            pIS->m_pFclose = &SsrwPalmFileFclose;
            pIS->m_pFseek = &SsrwPalmFileFseek;
            pIS->m_pFtell = &SsrwPalmFileFtell;

            /* Try to open the file */
            ((SsrwPalmFileInfo*)(pIS->m_pSrc))->m_isOwnedBySSRWIS = 1;
            ((SsrwPalmFileInfo*)(pIS->m_pSrc))->m_file = 
                FileOpen(0, in_pPath, 0, 0, palmMode, &errP);
            if (((SsrwPalmFileInfo*)(pIS->m_pSrc))->m_file == NULL)
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


SSRWIS* SsrwConnectToExternalPalmFileHand(FileHand in_file)
{
    SSRWIS*     pIS = NULL;

    pIS = malloc(sizeof(SSRWIS));
    if (pIS != NULL)
    {
        memset( pIS, 0, sizeof(SSRWIS));
        
        pIS->m_pSrc = malloc(sizeof(SsrwPalmFileInfo));
        if (pIS->m_pSrc != NULL)
        {
            memset(pIS->m_pSrc, 0, sizeof(SsrwPalmFileInfo));

            /* Initialize function pointers */
            pIS->m_pFread = &SsrwPalmFileFread;
            pIS->m_pFwrite = &SsrwPalmFileFwrite;
            pIS->m_pFclose = &SsrwPalmFileFclose;
            pIS->m_pFseek = &SsrwPalmFileFseek;
            pIS->m_pFtell = &SsrwPalmFileFtell;

            /* Initialize data members */
            ((SsrwPalmFileInfo*)(pIS->m_pSrc))->m_isOwnedBySSRWIS = 0;
            ((SsrwPalmFileInfo*)(pIS->m_pSrc))->m_file = in_file;
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

#endif /* SSTG_PALMOS */
