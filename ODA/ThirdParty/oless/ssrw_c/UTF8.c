/************************************************************************
* UTF8.c
*
* Contains functions for converting from UTF-8 to UCS-2 and back
*
* (c) Schema Software Inc., 2004
*
*************************************************************************

$Revision: 1.10.2.1 $
$Date: 2004/02/24 19:09:25 $
*/


#include "Types.h"
#include "Common.h"
#include "UTF8.h"

/*============================================================================
  Description:  Converts a wchar_t to a UTF-8 character
  Arguments:
   out_pm - pointer to a buffer of five or more bytes that will receive the
            UTF-8 character
   in_wch - wide character to convert to UTF-8

  Return:
   The length of the UTF-8 character, in bytes
  ==========================================================================*/
int 
wctou8(char *out_pm, wchar_t in_wch)
{
/* Unicode Table 3-5. UTF-8 Bit Distribution
Unicode                     1st Byte 2nd Byte 3rd Byte 4th Byte
00000000 0xxxxxxx           0xxxxxxx
00000yyy yyxxxxxx           110yyyyy 10xxxxxx
zzzzyyyy yyxxxxxx           1110zzzz 10yyyyyy 10xxxxxx
000uuuuu zzzzyyyy yyxxxxxx  11110uuu 10uuzzzz 10yyyyyy 10xxxxxx
*/

    if (!(in_wch & ~0x7f)) 
    { 
        out_pm[0] = in_wch & 0x7f;
        out_pm[1] = '\0';
        return 1; 
    }
    else if (!(in_wch & ~0x7ff)) 
    { 
        out_pm[0] = ((in_wch >> 6) & 0x1f) | 0xc0;
        out_pm[1] = (in_wch & 0x3f) | 0x80;
        out_pm[2] = '\0';
        return 2; 
    }
    else if(!(in_wch & ~0xffff)) 
    { 
        out_pm[0] = ((in_wch >> 12) & 0x0f) | 0xe0;
        out_pm[1] = ((in_wch >> 6)&0x3f) | 0x80;
        out_pm[2] = (in_wch & 0x3f) | 0x80;
        out_pm[3] = '\0';
        return 3; 
    }
    else if(!(in_wch & ~0x1fffff)) 
    { 
        out_pm[0] = ((in_wch >> 18) & 0x07) | 0xf0;
        out_pm[1] = ((in_wch >> 12) & 0x3f) | 0x80;
        out_pm[2] = ((in_wch >> 6 ) & 0x3f) | 0x80;
        out_pm[3] = (in_wch & 0x3f) | 0x80;
        out_pm[4] = '\0';
        return 4; 
    }
    else 
    {
        return -1;
    }
}

/*============================================================================
  Description: Returns the number of bytes required to represent a single 
               wchar_t in UTF-8
  Arguments:
   in_wch - wide character

  Return:
   The number of bytes required to represent in_wch as a UTF-8 character
  ==========================================================================*/
int 
wcu8len(const wchar_t in_wch)
{
    if(!(in_wch & ~0x7f)) 
    {
        return 1;
    }
    else if(!(in_wch & ~0x7ff)) 
    {
        return 2;
    }
    else if(!(in_wch & ~0xffff)) 
    {
        return 3;
    }
    else if(!(in_wch & ~0x1fffff)) 
    {
        return 4;
    }
    else 
    {
        /* error */ 
        return -1;
    }
}

/*============================================================================
  Description: Returns the number of bytes required to represent a wide (UCS-2)
               string in UTF-8
  Arguments:
   in_pwch - wide character string, null terminated

  Return:
   The number of bytes required to represent in_pwch as a UTF-8 string
  ==========================================================================*/
int 
wcsu8slen(const wchar_t *out_pwch)
{
    int     len = 0;
    wchar_t wch = 0;

    while ((wch = *out_pwch++) != 0)
    {
        if (!(wch & ~0x7f)) 
        {
            len += 1;
        }
        else if (!(wch & ~0x7ff)) 
        {
            len += 2;
        }
        else if (!(wch & ~0xffff)) 
        {
            len += 3;
        }
        else if (!(wch & ~0x1fffff)) 
        {
            len += 4;
        }
        else 
        {
            /* error: add width of null character entity &#x00; */
            len += 6;
        }
    }

    return len;
}

/*============================================================================
  Description: Converts a wide (UCS-2) string to UTF-8, or just returns the
               number of bytes required to store the UTF-8 string
  Arguments:
   out_pu - If NULL, wcstou8s returns the number of bytes required to store
            the UTF-8 version of the string.  If non-NULL, must point to a 
            buffer large enough to store the UTF-8 version of the string.
   in_pwch - the wide string to convert.  Must be nul-terminated.
   in_count - ignored

  Return:
   The number of bytes required to represent in_pwch as a UTF-8 string
  ==========================================================================*/
size_t 
wcstou8s(char *out_pu, const wchar_t *in_pwch, size_t in_count)
{
    SINT4   iU8ByteIndex = 0;
    wchar_t wch =          0;
    SINT4   iU8Len =       0;
    SINT4   iU8CharLen =   0;
    
    iU8Len = wcsu8slen(in_pwch);

    if (NULL == out_pu)
    {
        return (size_t)iU8Len;
    }

    while((wch = *in_pwch++) != 0)
    {
        iU8CharLen = (SINT4)wcu8len(wch);
        
        if(iU8CharLen >= 0)
        {
            if((iU8ByteIndex + wcu8len(wch)) <= (SINT4)in_count)
            { 
                iU8ByteIndex += wctou8(out_pu, wch); 
                out_pu += iU8CharLen; 
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return (size_t)iU8ByteIndex;
}

/*============================================================================
  Description: Determines the number of bytes used by a UTF-8 character
  Arguments:
   in_pu -    Pointer to a UTF-8 character
   in_count - number of characters pointed to by in_pu

  Return:
   The number of bytes used by the first character pointed to by in_pu
  ==========================================================================*/
int 
u8len(const char *in_pu, size_t in_count)
{
    if(0 == in_count) 
    {
        return 0;
    }
    else if (NULL == in_pu) 
    {
        return 0;
    }
    else if (0 == *in_pu)
    {
        return 0;
    }
    else if (!(*in_pu & ~0x7f)) 
    {
        return 1;
    }
    else if ((*in_pu & 0xe0) == 0xc0)
    {
        return 2;
    }
    else if ((*in_pu & 0xf0) == 0xe0)
    {
        return 3;
    }
    else if ((*in_pu & 0xf8) == 0xf0)
    {
        return 4;
    }
    else
    {
        /* error */ 
        return -1;
    }
}

/*============================================================================
  Description: Converts a UTF-8 character to a wide (UCS-2) character
  Arguments:
   in_pu -    Pointer to a UTF-8 character
   in_count - number of characters pointed to by in_pu

  Return:
   The number of bytes used by the first character pointed to by in_pu
  ==========================================================================*/
int 
u8towc(wchar_t *out_pwch, const char *in_pu, size_t in_count)
{
    SINT4 iU8Length = 0;

    if(NULL == out_pwch)
    {
        return -1;
    }

    iU8Length = u8len( in_pu,1 );

    if(iU8Length < 1)
    {
        return iU8Length;
    }
    else if (1 == iU8Length)
    { 
        out_pwch[0] = in_pu[0] & 0x7f; 

        return 1; 
    }
    else if (2 == iU8Length) 
    { 
        if((in_pu[1] & 0xc0) != 0x80) 
        {
            /* error */ 
            return -1;
        }

        out_pwch[0] = ((in_pu[0] &0x1f) << 6) | (in_pu[1] & 0x3f);

        return 2; 
    }
    else if (3 == iU8Length) 
    { 
        if (((in_pu[1] & 0xc0) != 0x80) ||
            ((in_pu[2] & 0xc0) != 0x80))
        {
            /* error */ 
            return -1;
        }

        out_pwch[0] = ((in_pu[0] & 0x0f) << 12) | 
                      ((in_pu[1] & 0x3f) << 6) | 
                       (in_pu[2] & 0x3f);

        return 3; 
    }
    else if (4 == iU8Length) 
    { 
        if (((in_pu[1] & 0xc0) != 0x80) ||
            ((in_pu[2] & 0xc0) != 0x80) ||
            ((in_pu[3] & 0xc0) != 0x80))
        {
            /* error */ 
            return -1;
        }

        out_pwch[0] = 
               ((in_pu[0] & 0x07) << 18) | 
               ((in_pu[1] & 0x3f) << 12) | 
               ((in_pu[2] & 0x3f) << 6) |
               ((in_pu[3] & 0x3f));

        return 4; 
    }
    else
    {
        /* error */ 
        return -1;
    }
}

/*============================================================================
  Description: Returns the number of wide characters required to represent 
               a UTF-8 string
  Arguments:
   in_pu -    Pointer to a UTF-8 string

  Return:
   The number of characters required to represent in_pu
  ==========================================================================*/
int 
u8swcslen(const char* in_pu)
{
    int  iU8Length = 0;
    char c =         0;

    while((c = *in_pu) != 0)
    {
        if (!(c & 0x80))
        { 
            iU8Length++; 
            in_pu += 1; 
        }
        else if ((c & 0xe0) == 0xc0) 
        { 
            iU8Length++; 
            in_pu += 2; 
        }
        else if ((c & 0xf0) == 0xe0) 
        { 
            iU8Length++; 
            in_pu += 3; 
        }
        else if ((c & 0xf8) == 0xf0) 
        { 
            iU8Length++; 
            in_pu += 4; 
        }
        else
        { 
            /* error: add width of single byte character entity &#xFF; */ 
            iU8Length += 6; 
            in_pu += 1; 
        }
    }

    return iU8Length;
}

/*============================================================================
  Description: Converts a UTF-8 string to a wide (UCS-2) string
  Arguments:
   out_pwch - If NULL, u8stowcs just returns the number of characters required 
              to represent in_pu as a wchar_t string.  If non-NULL, must 
              point to a buffer with enough characters to contain the converted 
              string
   in_pu -    Pointer to a UTF-8 string

  Return:
   The number of characters required to represent in_pu
  ==========================================================================*/
size_t 
u8stowcs(wchar_t *out_pwch, const char *in_pu, size_t in_count)
{
    SINT4 iWideChars = 0;
    SINT4 iU8CharLen = 0;

    if (NULL == out_pwch)
    {
        return u8swcslen(in_pu);
    }

    while ((*in_pu != 0) && (iWideChars < (SINT4)in_count))
    {
        iU8CharLen = u8towc(&out_pwch[iWideChars], in_pu, 1);
        if(iU8CharLen < 0) 
        {
            return -1;
        }
        else 
        {
            iWideChars++; 
            in_pu += iU8CharLen; 
        }
    }

    /* Add a nul terminator only if there is enough space */
    if (('\0' == *in_pu) && (iWideChars < (SINT4)in_count)) 
    {
        out_pwch[iWideChars++] = L'\0';
    }

    return iWideChars;
}
