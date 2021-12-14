/************************************************************************
* LEReadWrite.h
*
* Provides platform safe calls to real little endian byte ordered
* fields from a file
*
* (c) Schema Software Inc., 2001
*
*************************************************************************

$Revision: 1.8.2.3 $
$Date: 2004/02/24 19:09:25 $
*/

#include <stdlib.h>
#include <time.h>

#include "Types.h"
#include "Common.h"
#include "Debug.h"
#include "LEReadWrite.h"


/*============================================================================
  Description: Reads a little-endian short from memory
  Arguments: 
   in_pvBuf - pointer to memory from which to read
  Return: 
   Short value
  ==========================================================================*/
SINT2 leReadShort(const void* in_pvBuf)
{
    const BYTE* pC =    (const BYTE*)in_pvBuf;
    SINT2       sVal;

    sVal = (SINT2) * pC++;
    sVal = sVal + (SINT2) (*pC << 8);
    return sVal;
}


/*============================================================================
  Description: Reads a little-endian unsigned short from memory
  Arguments: 
   in_pvBuf - pointer to memory from which to read
  Return: 
   Unsigned short value
  ==========================================================================*/
UINT2 leReadUnsignedShort(const void* in_pvBuf)
{
    const BYTE* pC =        (const BYTE*)in_pvBuf;
    UINT2       wVal;

    wVal = (UINT2)(*pC++);
    wVal = wVal + (UINT2)((*pC) << 8);
    return wVal;

}


/*============================================================================
  Description: Reads a little-endian long from memory
  Arguments: 
   in_pvBuf - pointer to memory from which to read
  Return: 
   Long value
  ==========================================================================*/
SINT4 leReadLong(const void* in_pvBuf)
{
    const BYTE* pC =    (const BYTE*)in_pvBuf;
    SINT4       lVal;

    lVal = (SINT4) * pC++;
    lVal += (SINT4) (*pC++) << 8;
    lVal += (SINT4) (*pC++) << 16;
    lVal += (SINT4) (*pC) << 24;

    return lVal;
}


/*============================================================================
  Description: Reads a little-endian unsigned long from memory
  Arguments: 
   in_pvBuf - pointer to memory from which to read
  Return: 
   Unsigned long value
  ==========================================================================*/
UINT4 leReadUnsignedLong(const void* in_pvBuf)
{
    const BYTE* pC =        (const BYTE*)in_pvBuf;
    UINT4       ulVal;

    ulVal = (UINT4) * pC++;
    ulVal += (UINT4) (*pC++) << 8;
    ulVal += (UINT4) (*pC++) << 16;
    ulVal += (UINT4) (*pC) << 24;

    return ulVal;
}


/*============================================================================
  Description: Reads a little-endian long long (64-bit integer) from memory
  Arguments: 
   in_pvBuf - pointer to memory from which to read
  Return: 
   Long long value
  ==========================================================================*/
SINT8 leReadLongLong(const void* in_pvBuf)
{
    const BYTE* pC =    (const BYTE*)in_pvBuf;
    SINT8       llVal;

    llVal = (SINT8) * pC++;
    llVal += (SINT8) (*pC++) << 8;
    llVal += (SINT8) (*pC++) << 16;
    llVal += (SINT8) (*pC++) << 24;
    llVal += (SINT8) (*pC++) << 32;
    llVal += (SINT8) (*pC++) << 40;
    llVal += (SINT8) (*pC++) << 48;
    llVal += (SINT8) (*pC) << 56;

    return llVal;
}


/*============================================================================
  Description: Reads a little-endian unsigned long long (64-bit unsigned 
               integer) from memory
  Arguments: 
   in_pvBuf - pointer to memory from which to read
  Return: 
   Long long value
  ==========================================================================*/
UINT8 leReadUnsignedLongLong(const void* in_pvBuf)
{
    const BYTE* pC = (const BYTE*)in_pvBuf;
    UINT8       val;

    val = (UINT8) * pC++;
    val += (UINT8) (*pC++) << 8;
    val += (UINT8) (*pC++) << 16;
    val += (UINT8) (*pC++) << 24;
    val += (UINT8) (*pC++) << 32;
    val += (UINT8) (*pC++) << 40;
    val += (UINT8) (*pC++) << 48;
    val += (UINT8) (*pC) << 56;

    return val;
}


/*============================================================================
  Description: Reads a little-endian float from memory
  Arguments: 
   in_pvBuf - pointer to memory from which to read
  Return: 
   Float value
  ==========================================================================*/
FLT4 leReadFloat(const void* in_pvBuf)
{
    SINT4 lVal;
    FLT4* pVal;

    lVal = leReadLong(in_pvBuf);
    pVal = (FLT4*)(void*)&lVal;

    return *pVal;
}


/*============================================================================
  Description: Reads a little-endian double from memory
  Arguments: 
   in_pvBuf - pointer to memory from which to read
  Return: 
   Double value
  ==========================================================================*/
FLT8 leReadDouble(const void* in_pvBuf)
{
    SINT8 llVal;
    FLT8* pVal;

    llVal = leReadLongLong(in_pvBuf);
    pVal = (FLT8*)(void*)&llVal;

    return *pVal;
}


/*============================================================================
  Description: Reads a little-endian FILETIME into a native-format time_t
  Arguments: 
   in_pvBuf - pointer to memory from which to read
  Return: 
   time_t value
  ==========================================================================*/
time_t
leReadTime (const void* in_pvBuf)
{
    time_t  ret;
    WinTime winTime;

    winTime.ulLowDateTime = leReadUnsignedLong(in_pvBuf);
    winTime.ulHighDateTime =
        leReadUnsignedLong((void*)((char*)in_pvBuf + sizeof(UINT4)));

    /* Discard error codes here */
    time_tFromWindowsTime(&winTime, &ret);
    return ret;
}


/*============================================================================
  Description: Writes a short from memory into a little-endian buffer
  Arguments: 
   in_sVal -       value to write
   out_pvBuf - buffer into which to write little-endian value
  ==========================================================================*/
void leWriteShort(SINT2 in_sVal, void* out_pvBuf)
{
    BYTE* pC = (BYTE*)out_pvBuf;

    *pC++ = (BYTE)(in_sVal & 0xff);
    *pC = (BYTE)((in_sVal >> 8) & 0xff);
}


/*============================================================================
  Description: Writes an unsigned short from memory into a little-endian 
               buffer
  Arguments: 
   in_wVal -   value to write
   out_pvBuf - buffer into which to write little-endian value
  ==========================================================================*/
void leWriteUnsignedShort(UINT2 in_wVal, void* out_pvBuf)
{
    BYTE* pC = (BYTE*)out_pvBuf;

    *pC++ = (BYTE)(in_wVal & 0xff);
    *pC = (BYTE)((in_wVal >> 8) & 0xff);
}


/*============================================================================
  Description: Writes a long from memory into a little-endian buffer
  Arguments: 
   in_lVal -   value to write
   out_pvBuf - buffer into which to write little-endian value
  ==========================================================================*/
void leWriteLong(SINT4 in_lVal, void* out_pvBuf)
{
    BYTE* pC = (BYTE*)out_pvBuf;

    *pC++ = (BYTE)(in_lVal & 0xff);
    *pC++ = (BYTE)((in_lVal >> 8) & 0xff);
    *pC++ = (BYTE)((in_lVal >> 16) & 0xff);
    *pC = (BYTE)((in_lVal >> 24) & 0xff);
}


/*============================================================================
  Description: Writes an unsigned long from memory into a little-endian buffer
  Arguments: 
   in_ulVal -  value to write
   out_pvBuf - buffer into which to write little-endian value
  ==========================================================================*/
void leWriteUnsignedLong(UINT4 in_ulVal, void* out_pvBuf)
{
    BYTE* pC = (BYTE*)out_pvBuf;

    *pC++ = (BYTE)(in_ulVal & 0xff);
    *pC++ = (BYTE)((in_ulVal >> 8) & 0xff);
    *pC++ = (BYTE)((in_ulVal >> 16) & 0xff);
    *pC = (BYTE)((in_ulVal >> 24) & 0xff);
}


/*============================================================================
  Description: Writes a long long (64-bit signed integer) from memory into a 
               little-endian buffer
  Arguments: 
   in_llVal -  value to write
   out_pvBuf - buffer into which to write little-endian value
  ==========================================================================*/
void leWriteLongLong(SINT8 in_llVal, void* out_pvBuf)
{
    BYTE* pC = (BYTE*)out_pvBuf;

    *pC++ = (BYTE)(in_llVal & 0xff);
    *pC++ = (BYTE)((in_llVal >> 8) & 0xff);
    *pC++ = (BYTE)((in_llVal >> 16) & 0xff);
    *pC++ = (BYTE)((in_llVal >> 24) & 0xff);
    *pC++ = (BYTE)((in_llVal >> 32) & 0xff);
    *pC++ = (BYTE)((in_llVal >> 40) & 0xff);
    *pC++ = (BYTE)((in_llVal >> 48) & 0xff);
    *pC = (BYTE)((in_llVal >> 56) & 0xff);
}


/*============================================================================
  Description: Writes an unsigned long long (64-bit unsigned integer) from 
               memory into a little-endian buffer
  Arguments: 
   in_ullVal -  value to write
   out_pvBuf - buffer into which to write little-endian value
  ==========================================================================*/
void leWriteUnsignedLongLong(UINT8 in_ullVal, void* out_pvBuf)
{
    BYTE* pC = (BYTE*)out_pvBuf;

    *pC++ = (BYTE)(in_ullVal & 0xff);
    *pC++ = (BYTE)((in_ullVal >> 8) & 0xff);
    *pC++ = (BYTE)((in_ullVal >> 16) & 0xff);
    *pC++ = (BYTE)((in_ullVal >> 24) & 0xff);
    *pC++ = (BYTE)((in_ullVal >> 32) & 0xff);
    *pC++ = (BYTE)((in_ullVal >> 40) & 0xff);
    *pC++ = (BYTE)((in_ullVal >> 48) & 0xff);
    *pC = (BYTE)((in_ullVal >> 56) & 0xff);
}


/*============================================================================
  Description: Writes a float from memory into a little-endian buffer
  Arguments: 
   in_fVal -   value to write
   out_pvBuf - buffer into which to write little-endian value
  ==========================================================================*/
void leWriteFloat(FLT4 in_fVal, void* out_pvBuf)
{
    SINT4 lVal;

    lVal = *((SINT4*)(void*)&in_fVal);
    leWriteLong (lVal, out_pvBuf);
}


/*============================================================================
  Description: Writes a double from memory into a little-endian buffer
  Arguments: 
   in_lfVal -  value to write
   out_pvBuf - buffer into which to write little-endian value
  ==========================================================================*/
void leWriteDouble(FLT8 in_lfVal, void* out_pvBuf)
{
    SINT8 llVal;

    llVal = *((SINT8*)(void*)&in_lfVal);
    leWriteLongLong (llVal, out_pvBuf);
}


/*============================================================================
  Description: Writes a time_t from memory as a little-endian FILETIME in a 
  Arguments: 
   in_val -    value to write
   out_pvBuf - buffer into which to write little-endian FILETIME
  ==========================================================================*/
void leWriteTime(time_t in_val, void* out_pvBuf)
{
    WinTime winTime;
    UINT4*  pulBuf = (UINT4*)out_pvBuf;

    winTime.ulLowDateTime = 0;
    winTime.ulHighDateTime = 0;

    /* Discard error codes here */
    WindowsTimeFromTime_t(in_val, &winTime);

    leWriteUnsignedLong(winTime.ulLowDateTime, pulBuf++);
    leWriteUnsignedLong(winTime.ulHighDateTime, pulBuf++);
}

