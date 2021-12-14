/************************************************************************
* StructuredStorage.c
*
* Implementation for StgInfo "object" and various utility functions
*
* (c) Schema Software Inc., 2001-2003
*
*************************************************************************

$Revision: 1.5 $
$Date: 2003/03/26 21:00:04 $
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Types.h"
#include "Common.h"
#include "Debug.h"
#include "StructuredStorage.h"
#include "RootStorage.h"
#include "Fat.h"
#include "LEReadWrite.h"

/*============================================================================
  Description: Frees memory allocated to StgInfo object(s)
               either a single object or an array. 
               In fact, since it takes "pointer to pointer" input,
               it treats a single structure as an array of length 1.
  Arguments: 
   in_ppInfo - handle to an array to free
   in_count -  array length. Pass 1 for single element.
  Return: 
   Status code
==============================================================================*/
int
freeInfo(StgInfo** io_paInfo,
         int in_count)
{
    SINT4 i = 0;

    ASSERT(in_count >= 0 && io_paInfo != NULL && *io_paInfo != NULL);
    if (io_paInfo == NULL || *io_paInfo == NULL || in_count < 0)
    {
        return SSTG_ERROR_ILLEGAL_CALL;
    }

    if (*io_paInfo != NULL)
    {
        ASSERT(in_count > 0);
        for (i = 0; i < in_count; i++)
        {
            if (((*io_paInfo)[i]).sName != NULL)
            {
                free (((*io_paInfo)[i]).sName);
            }
        }

        free (*io_paInfo);
        *io_paInfo = NULL;
    }

    return SSTG_OK;
}

