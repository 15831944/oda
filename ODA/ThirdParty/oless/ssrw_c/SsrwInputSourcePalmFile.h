/******************************************************************************\
* 
*  SsrwInputSourcePalmFile.h
*  
*  Passes calls from the SsrwInputSource abstraction down to Palm C-like 
*  file I/O functions
* 
*  Copyright (C) 2002 - 2004 Schema Software, Inc. All rights reserved.
*
*  $Revision: 1.2.2.1 $
*  $Date: 2004/02/24 03:06:20 $
*  
\******************************************************************************/

#ifndef SSRWINPUTSOURCEPALMFILE_H_INCLUDED
#define SSRWINPUTSOURCEPALMFILE_H_INCLUDED

#include "OlessExport.h"

#ifdef SSTG_PALMOS

#include "SsrwInputSource.h"

/**
 * Works just like fread, but accepts an SSRWIS* instead of a FILE*.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param out_pv -  buffer into which to read data
 * @param in_size - size, in bytes, of each item to read
 * @param in_num -  number of items to read
 * @param in_pIS -  pointer to a palm file input source to read from 
 * @return number of items read
 */
_OLESS_API size_t SsrwPalmFileFread(
    void*   in_pv,
    size_t  in_size,
    size_t  in_num,
    SSRWIS* in_pIS);

/**
 * Works just like fwrite, but accepts an SSRWIS* instead of a FILE*.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param in_pv -   buffer of data to write
 * @param in_size - size, in bytes, of each item to write
 * @param in_num -  number of items to write
 * @param in_pIS -  pointer to a palm file input source to write to
 * @return number of items write
 */
_OLESS_API size_t SsrwPalmFileFwrite(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS);

/**
 * Works just like fclose, but accepts an SSRWIS* instead of a FILE*.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param in_pIS -  pointer to a palm file input source to close
 * @return 0 on success, nonzero on failure
 */
_OLESS_API int SsrwPalmFileFclose(
    SSRWIS* in_pv);

/**
 * Works just like fseek, but accepts an SSRWIS* instead of a FILE*.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param in_pIS -      pointer to a palm file input stream whose seek location 
 *                      should be moved
 * @param in_llOffset - offset to which to move pointer
 * @param in_whence -   SEEK_SET, etc. origin from which to move pointer
 * @return 0 on success, nonzero on failure
 */
_OLESS_API int SsrwPalmFileFseek(
    SSRWIS* in_pIS,
    SINT8   in_llOffset,
    int     in_whence);

/**
 * Works just like ftell, but accepts an SSRWIS* instead of a FILE*.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param in_pIS - palm file input stream from which to get seek location
 * @return offset from beginning of input source, in bytes
 */
_OLESS_API SINT8 SsrwPalmFileFtell(
    SSRWIS* in_pIS);

/**
 * Truncates a stream to a given size.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param in_pIS -      pointer to a palm file input stream that should be 
 *                      truncated
 * @param in_llOffset - new size of the stream
 * @return 0 on success, nonzero on failure
 */
_OLESS_API SINT4 SsrwPalmFileFtruncate(
    SSRWIS* in_pIS,
    SINT8   in_llOffset);

#endif /* SSTG_PALMOS */

#endif /* SSRWINPUTSOURCEPALMFILE_H_INCLUDED */
