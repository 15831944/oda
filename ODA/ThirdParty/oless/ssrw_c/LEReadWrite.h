/************************************************************************
* LEReadWrite.h
*
* Provides platform safe calls to real little endian byte ordered
* fields from a file
*
* (c) Schema Software Inc., 2001
*
*************************************************************************

$Revision: 1.6 $
$Date: 2003/04/01 19:41:37 $
*/

#ifndef _LEREADWRITE_H
#define _LEREADWRITE_H

extern _OLESS_API SINT2 leReadShort(const void* val);
extern _OLESS_API UINT2 leReadUnsignedShort(const void* val);
extern _OLESS_API SINT4 leReadLong(const void* val);
extern _OLESS_API UINT4 leReadUnsignedLong(const void* val);
extern _OLESS_API SINT8 leReadLongLong(const void* val);
extern _OLESS_API UINT8 leReadUnsignedLongLong(const void* val);
extern _OLESS_API FLT4 leReadFloat(const void* val);
extern _OLESS_API FLT8 leReadDouble(const void* val);
extern _OLESS_API time_t leReadTime (const void* val);


extern void leWriteShort(SINT2 val, void* buffer);
extern void leWriteUnsignedShort(UINT2 val, void* buffer);
extern void leWriteLong(SINT4 val, void* buffer);
extern void leWriteUnsignedLong(UINT4 val, void* buffer);
extern void leWriteLongLong(SINT8 val, void* buffer);
extern void leWriteUnsignedLongLong(UINT8 val, void* buffer);
extern void leWriteFloat(FLT4 val, void* buffer);
extern void leWriteDouble(FLT8 val, void* buffer);
extern void leWriteTime (time_t val, void* buffer);

#endif /* _LEREADWRITE_H */

