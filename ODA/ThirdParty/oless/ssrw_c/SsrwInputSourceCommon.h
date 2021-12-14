/**
* 
*  SsrwInputSourceCommon.h
*  
*  Provides shared definitions for the abstractions from the C file I/O 
*  functions
* 
*  Copyright (C) 2002 - 2004 Schema Software, Inc. All rights reserved.
*
*  $Revision: 1.1.2.2 $
*  $Date: 2004/02/24 21:53:56 $
*  
*/

#ifndef SSRWINPUTSOURCECOMMON_H_INCLUDED
#define SSRWINPUTSOURCECOMMON_H_INCLUDED

#include <stdio.h>
#ifdef SSTG_PALMOS
#include <PalmOS.h>
#include <VFSMgr.h>
#endif /* SSTG_PALMOS */

#include "OlessExport.h"
#include "SsrwInputSource.h"

/**
 * A realloc-like function is needed by the memory-file interface in order to
 * resize the buffer on write
 */
typedef void* (*Ssrw_Func_Realloc)(
    void*       in_ptr,
    size_t      in_size);

/**
 * Contains information needed to simulate a file in main memory
 */
typedef struct
{
    void**              m_ppBuf;
    SINT8               m_llLengthUsed;
    SINT8               m_llLengthAllocated;
    SINT8               m_llSeekPos;
    Ssrw_Func_Realloc   m_pRealloc;
    int                 m_isReadable : 1;
    int                 m_isWritable : 1;
} SsrwMemFileInfo;

#ifdef SSTG_PALMOS

typedef struct
{
    FileHand            m_file;
    int                 m_isOwnedBySSRWIS : 1;
    int                 m_isReadable : 1;
    int                 m_isWritable : 1;
} SsrwPalmFileInfo;

typedef struct
{
    FileRef             m_file;
    int                 m_isOwnedBySSRWIS : 1;
    int                 m_isReadable : 1;
    int                 m_isWritable : 1;
} SsrwPalmExpFileInfo;

#else /* !SSTG_PALMOS */

typedef struct
{
    FILE*               m_pFile;
int                     m_isOwnedBySSRWIS : 1;
} SsrwRealFileInfo;

#endif /* !SSTG_PALMOS */

/**
 * Creates a new SSRWIS* for reading, using an in-memory buffer as a source.
 *
 * @param in_ppvBuf - pointer to a pointer to a buffer from which to read
 * @param in_ulLength - number of bytes in buffer
 * @return NULL on failure, new SSRWIS* input source on success
 */
_OLESS_API SSRWIS* SsrwOpenMemForReading(
    const void**    in_ppvBuf,
    unsigned long   in_ulLength);

/**
 * Creates a new SSRWIS* for writing, using an in-memory buffer as a target.
 * The buffer will be resized if it is not sufficiently large to hold all the
 * data in the structured storage file.
 *
 * When the SSRWIS* is closed, the buffer will be resized to the exact size of
 * the structured storage data.
 *
 * @param in_ppvBuf -   pointer to a pointer to an in-memory target buffer 
 * @param in_ulLength - the current length of the buffer, in bytes
 * @param in_pRealloc - pointer to a function conforming to the same prototype
 *                      as the C library realloc function.  This function will
 *                      be called each time the buffer needs to be enlarged.
 * @return NULL on failure, new writable SSRWIS* input source on success
 */
_OLESS_API SSRWIS* SsrwOpenMemForWriting(
    void**                  in_ppvBuf,
    unsigned long           in_ulLength,
    Ssrw_Func_Realloc       in_pRealloc);

/**
 * Creates a new SSRWIS* for reading and writing, using an in-memory buffer as 
 * a target.  The buffer will be resized if it is not sufficiently large to 
 * hold all the data in the structured storage file.
 *
 * When the SSRWIS* is closed, the buffer will be resized to the exact size of
 * the structured storage data.
 *
 * @param in_ppvBuf -   pointer to a pointer to an in-memory target buffer 
 * @param in_ulLength - the current length of the buffer, in bytes
 * @param in_pRealloc - pointer to a function conforming to the same prototype
 *                      as the C library realloc function.  This function will
 *                      be called each time the buffer needs to be enlarged.
 * @return NULL on failure, new writable SSRWIS* input source on success
 */
_OLESS_API SSRWIS* SsrwOpenMemForReadWrite(
    void**                  in_ppvBuf,
    unsigned long           in_ulLength,
    Ssrw_Func_Realloc       in_pRealloc);

#ifdef SSTG_PALMOS

/**
 * Creates a new SSRWIS based on a Palm-style file, using the same arguments as 
 * fopen.
 *
 * @param in_pPath - file to open
 * @param in_pMode - mode ("wb", "w+", "r", etc.) in which to open file
 * @return NULL on failure, pointer to a new SSRWIS input source on success
 */
_OLESS_API SSRWIS* SsrwOpenFile(const char* in_pPath, const char* in_pMode);

/**
 * Creates a new SSRWIS* based on an existing, open Palm FileHand.
 * When the SSRWIS* is disconnected, the underlying FileHand will not be
 * FileClose()d.
 * This SSRWIS should not be SsrwFclose()d.
 *
 * @param in_pFile - non-zero, open FILE*
 * @return NULL on failure, new SSRWIS* on success
 */
_OLESS_API SSRWIS* SsrwConnectToExternalPalmFileHand(FileHand in_pFile);

/**
 * Creates a new SSRWIS based on a Palm-style VFS file, using mostly the same 
 * arguments as fopen.
 *
 * @param in_pPath - file to open
 * @param in_pMode - mode ("wb", "w+", "r", etc.) in which to open file
 * @param in_unVolRef - PalmOS VFS volume reference
 *
 * @return NULL on failure, pointer to a new SSRWIS input source on success
 */
_OLESS_API SSRWIS* SsrwOpenVFSFile(const char* in_pPath, const char* in_pMode, UInt16 in_unVolRef);

/**
 * Creates a new SSRWIS* based on an existing, open Palm FileHand.
 * When the SSRWIS* is disconnected, the underlying FileHand will not be
 * FileClose()d.
 * This SSRWIS should not be SsrwFclose()d.
 *
 * @param in_pFile - non-zero, open FILE*
 * @return NULL on failure, new SSRWIS* on success
 */
_OLESS_API SSRWIS* SsrwConnectToExternalPalmVFSFileRef(FileRef in_pFile);

#else /* !SSTG_PALMOS */

/**
* Creates a new SSRWIS based on a file, using the same arguments as fopen.
*
* @param in_pPath - file to open
* @param in_pMode - mode ("wb", "w+", "r", etc.) in which to open file
* @return NULL on failure, pointer to a new SSRWIS input source on success
*/
_OLESS_API SSRWIS* SsrwOpenFile(const wchar_t* in_pPath, const wchar_t* in_pMode);

/**
* Creates a new SSRWIS* based on an existing, open FILE* pointer.
* When the SSRWIS* is disconnected, the underlying FILE* will not be
* fclose()d.
* This SSRWIS should not be SsrwFclose()d.
*
* @param in_pFile - non-zero, open FILE*
* @return NULL on failure, new SSRWIS* on success
*/
_OLESS_API SSRWIS* SsrwConnectToExternalFile(FILE* in_pFile);

#endif /* !SSTG_PALMOS */

#ifdef DEBUG
_OLESS_API SINT4 SSRWISSelfTest(void);
#endif /* DEBUG */

#endif /* SSRWINPUTSOURCECOMMON_H_INCLUDED */
