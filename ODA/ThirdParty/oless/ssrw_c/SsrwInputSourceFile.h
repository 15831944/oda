/******************************************************************************\
* 
*  SsrwInputSource.h
*  
*  Passes calls from the SsrwInputSource abstraction down to the real C FILE* 
*  functions
* 
*  Copyright (C) 2002 - 2003 Schema Software, Inc. All rights reserved.
*
*  $Revision: 1.3 $
*  $Date: 2003/03/26 21:00:04 $
*  
\******************************************************************************/

#ifndef SSRWINPUTSOURCEFILE_H_INCLUDED
#define SSRWINPUTSOURCEFILE_H_INCLUDED

#ifndef SSTG_PALMOS

#include "OlessExport.h"
#include "SsrwInputSource.h"

/**
 * Works just like fread, but accepts an SSRWIS* instead of a FILE*.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param out_pv -  buffer into which to read data
 * @param in_size - size, in bytes, of each item to read
 * @param in_num -  number of items to read
 * @param in_pIS -  pointer to a file input source to read from 
 * @return number of items read
 */
_OLESS_API size_t SsrwFileFread(
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
 * @param in_pIS -  pointer to a file input source to write to
 * @return number of items write
 */
_OLESS_API size_t SsrwFileFwrite(
    const void* in_pv,
    size_t      in_size,
    size_t      in_num,
    SSRWIS*     in_pIS);

/**
 * Works just like fclose, but accepts an SSRWIS* instead of a FILE*.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param in_pIS -  pointer to a file input source to close
 * @return 0 on success, nonzero on failure
 */
_OLESS_API int SsrwFileFclose(
    SSRWIS* in_pv);

/**
 * Works just like fseek, but accepts an SSRWIS* instead of a FILE*.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param in_pIS -      pointer to a file input stream whose seek location 
 *                      should be moved
 * @param in_llOffset - offset to which to move pointer
 * @param in_whence -   SEEK_SET, etc. origin from which to move pointer
 * @return 0 on success, nonzero on failure
 */
_OLESS_API int SsrwFileFseek(
    SSRWIS* in_pIS,
    SINT8   in_llOffset,
    int     in_whence);

/**
 * Works just like ftell, but accepts an SSRWIS* instead of a FILE*.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param in_pIS - file input stream from which to get seek location
 * @return offset from beginning of input source, in bytes
 */
_OLESS_API SINT8 SsrwFileFtell(
    SSRWIS* in_pIS);

/**
 * Truncates a SSRWIS to a given size, if possible.
 * THIS FUNCTION SHOULD NOT BE USED BY EXTERNAL CALLERS.
 *
 * @param in_pIS -       file input stream to truncate
 * @param in_llNewSize - new size of stream
 * @return 0 on success, nonzero on failure
 */
_OLESS_API long  SsrwFileFtruncate(
    SSRWIS* in_pIS,
    SINT8   in_llNewSize);

#endif /* !SSTG_PALMOS */

#endif /* SSRWINPUTSOURCEFILE_H_INCLUDED */
