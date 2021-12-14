/******************************************************************************\
* 
*  SsrwInputSourcePalmVFSFile.c
*  
*  Passes calls from the SsrwInputSource abstraction down to the PalmOS 
*  expansion card file I/O functions
* 
*  Copyright (C) 2002 - 2004 Schema Software, Inc. All rights reserved.
*
*  $Revision: 1.3.2.3 $
*  $Date: 2004/02/26 19:20:30 $
*  
\******************************************************************************/

#ifdef SSTG_PALMOS

#include <string.h>
#include <PalmOS.h>
#include <VFSMgr.h>

#include "Types.h"
#include "Debug.h"
#include "Common.h"
#include "SsrwInputSourcePalmVFSFile.h"
#include "SsrwInputSourceCommon.h"

size_t SsrwPalmVFSFileFread(
    void*   in_pv,
    size_t  in_size,
    size_t  in_num,
    SSRWIS* in_pIS)
{
    Err    err =          0;
    UInt32 numBytesRead = 0;

    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file != NULL);

    err = VFSFileRead(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file, 
                      in_size * in_num, 
                      in_pv, 
                      &numBytesRead);

    return numBytesRead / in_size;
}


size_t SsrwPalmVFSFileFwrite(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS)
{
    Err    err =             0;
    UInt32 numBytesWritten = 0;

    ASSERT(in_pIS != NULL);
    ASSERT(in_pv != NULL);
    ASSERT(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file != 0);

    err = VFSFileWrite(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file, in_size * in_num, in_pv, &numBytesWritten);

    return numBytesWritten / in_size;
}


int SsrwPalmVFSFileFclose(
    SSRWIS* in_pIS)
{
    unsigned short unResult = 0;

    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file != 0);
    ASSERT(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_isOwnedBySSRWIS != 0);

    unResult = VFSFileClose(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file);

    free(in_pIS->m_pSrc);

    return unResult;
}


int SsrwPalmVFSFileFseek(
    SSRWIS* in_pIS,
    SINT8   in_llOffset,
    int     in_nWhence)
{
    FileOrigin palmOrigin = vfsOriginBeginning;

    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file != NULL);

    switch(in_nWhence)
    {
    case SSRWIS_SEEK_SET:
        palmOrigin = vfsOriginBeginning;
        break;
    case SSRWIS_SEEK_CUR:
        palmOrigin = vfsOriginCurrent;
        break;
    case SSRWIS_SEEK_END:
        palmOrigin = vfsOriginEnd;
        break;
    default:
        ASSERT(0);
        break;
    }

    return VFSFileSeek(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file,
                       palmOrigin,
                       (long)in_llOffset);
}


SINT8 SsrwPalmVFSFileFtell(
    SSRWIS* in_pIS)
{
    UInt32 filePos =   0;
    Err    err =       0;

    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file != NULL);

    err = VFSFileTell(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file, &filePos);

    return (SINT8)filePos;
}


SINT4 SsrwPalmVFSFileFtruncate(
    SSRWIS* in_pIS,
    SINT8   in_llNewSize)
{
    ASSERT(in_pIS != NULL);
    ASSERT(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file != NULL);

    return VFSFileResize(((SsrwPalmExpFileInfo*)(in_pIS->m_pSrc))->m_file, (UInt32)in_llNewSize);
}


SSRWIS* SsrwOpenVFSFile(const char* in_pPath, const char* in_pMode, UInt16 in_unVolRef)
{
    SSRWIS*     pIS =       NULL;
    Err         errP =      0;
    UInt16      vfsMode =   0;
    FileHand    file =      0;

    /* Convert the file mode */
    if ((strcmp(in_pMode, "w+") == 0)||
        (strcmp(in_pMode, "w") == 0) ||
        (strcmp(in_pMode, "wb") == 0))
    {
        vfsMode = vfsModeReadWrite;
    }
    else if ((strcmp(in_pMode, "r") == 0) ||
             (strcmp(in_pMode, "rb") == 0))
    {
        vfsMode = vfsModeRead;
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
        pIS->m_pSrc = malloc(sizeof(SsrwPalmExpFileInfo));
        if (pIS->m_pSrc != NULL)
        {
            memset( pIS->m_pSrc, 0, sizeof(SsrwPalmExpFileInfo));

            /* Initialize function pointers */
            pIS->m_pFread = &SsrwPalmVFSFileFread;
            pIS->m_pFwrite = &SsrwPalmVFSFileFwrite;
            pIS->m_pFclose = &SsrwPalmVFSFileFclose;
            pIS->m_pFseek = &SsrwPalmVFSFileFseek;
            pIS->m_pFtell = &SsrwPalmVFSFileFtell;
            pIS->m_pFtruncate = &SsrwPalmVFSFileFtruncate;

            /* Try to open the file */
            ((SsrwPalmExpFileInfo*)(pIS->m_pSrc))->m_isOwnedBySSRWIS = 1;
            VFSFileOpen(
                in_unVolRef, 
                in_pPath, 
                vfsMode, 
                &(((SsrwPalmExpFileInfo*)(pIS->m_pSrc))->m_file));
            if (((SsrwPalmExpFileInfo*)(pIS->m_pSrc))->m_file == NULL)
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


SSRWIS* SsrwConnectToExternalPalmVFSFileRef(FileRef in_file)
{
    SSRWIS*     pIS = NULL;

    pIS = malloc(sizeof(SSRWIS));
    if (pIS != NULL)
    {
        memset( pIS, 0, sizeof(SSRWIS));
        pIS->m_pSrc = malloc(sizeof(SsrwPalmExpFileInfo));
        if (pIS->m_pSrc != NULL)
        {
            memset( pIS->m_pSrc, 0, sizeof(SsrwPalmExpFileInfo));

            /* Initialize file pointers */
            pIS->m_pFread = &SsrwPalmVFSFileFread;
            pIS->m_pFwrite = &SsrwPalmVFSFileFwrite;
            pIS->m_pFclose = &SsrwPalmVFSFileFclose;
            pIS->m_pFseek = &SsrwPalmVFSFileFseek;
            pIS->m_pFtell = &SsrwPalmVFSFileFtell;
            pIS->m_pFtruncate = &SsrwPalmVFSFileFtruncate;

            /* Initialize the data members */
            ((SsrwPalmExpFileInfo*)(pIS->m_pSrc))->m_isOwnedBySSRWIS = 0;
            ((SsrwPalmExpFileInfo*)(pIS->m_pSrc))->m_file = in_file;
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
