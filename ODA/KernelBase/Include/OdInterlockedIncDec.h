/////////////////////////////////////////////////////////////////////////////// 
// Copyright (C) 2002-2021, Open Design Alliance (the "Alliance"). 
// All rights reserved. 
// 
// This software and its documentation and related materials are owned by 
// the Alliance. The software may only be incorporated into application 
// programs owned by members of the Alliance, subject to a signed 
// Membership Agreement and Supplemental Software License Agreement with the
// Alliance. The structure and organization of this software are the valuable  
// trade secrets of the Alliance and its suppliers. The software is also 
// protected by copyright law and international treaty provisions. Application  
// programs incorporating this software must include the following statement 
// with their copyright notices:
//   
//   This application incorporates Open Design Alliance software pursuant to a license 
//   agreement with Open Design Alliance.
//   Open Design Alliance Copyright (C) 2002-2021 by Open Design Alliance. 
//   All rights reserved.
//
// By use of this software, its documentation or related materials, you 
// acknowledge and accept the above terms.
///////////////////////////////////////////////////////////////////////////////

#ifndef _OD_INTERLOCKED_INCREMENT_H_
#define _OD_INTERLOCKED_INCREMENT_H_

#include <cstring>
#ifdef OD_POSIX_THREADS
#include <pthread.h>
#endif
#if defined(_MSC_VER)
#include <winnt.h>
#endif

#ifndef TD_SINGLE_THREAD
#if defined(_MSC_VER) && (_M_IX86) && (_M_IX86 >= 400) && !defined(_WIN32_WCE)
// Intel486 platform with Microsoft compiler
#pragma warning(push)
#pragma warning(disable:4035)
#pragma warning(disable:4793)
inline int OdInterlockedIncrement(volatile int* dest)
{
  __asm
  {
    mov edx, dest
    mov eax, 1
    lock xadd[edx], eax
    inc eax
  }
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  __asm
  {
    mov edx, dest
    mov eax, -1
    lock xadd[edx], eax
    dec eax
  }
}

#pragma warning(pop)
#elif (defined(_WIN32) || defined(_WIN64)) && /*!defined(_WIN32_WCE) &&*/ /*!defined(_WINRT) && */!defined(ODA_WINDOWS_GCC)
// Windows platform with generic compiler - use interlocked functions from Win32 API
#define OdInterlockedIncrement(dest) InterlockedIncrement((LONG*)(dest))
#define OdInterlockedDecrement(dest) InterlockedDecrement((LONG*)(dest))
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)) && !defined(ANDROID)
// Intel platform with GCC compiler
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  int ret;
  __asm__ __volatile__(
    "lock; xaddl %0, (%1)"
    : "=r" (ret)
    : "r" (dest), "0" (incr)
    : "memory");
  return ret;
}
inline int OdInterlockedIncrement(volatile int* dest)
{
  return OdInterlockedExchangeAdd(dest, 1) + 1;
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return OdInterlockedExchangeAdd(dest, -1) - 1;
}
#elif defined(__GNUC__) && defined(__POWERPC__)
// Macintosh PowerPC platform with GCC compiler
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  // Assembler code is taken from Wine 0.9.4 sources
  // See http://cvs.winehq.org/cvsweb/wine/libs/port/interlocked.c?rev=1.7&content-type=text/x-cvsweb-markup
  int ret = 0;
  int zero = 0;
  __asm__ __volatile__(
    "0:  lwarx %0, %3, %1\n"
    "    add %0, %2, %0\n"
    "    stwcx. %0, %3, %1\n"
    "    bne- 0b\n"
    "    isync\n"
    : "=&r" (ret)
    : "r"(dest), "r"(incr), "r"(zero)
    : "cr0", "memory", "r0");
  return ret - incr;
}

inline int OdInterlockedIncrement(volatile int* dest)
{
  return OdInterlockedExchangeAdd(dest, 1) + 1;
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return OdInterlockedExchangeAdd(dest, -1) - 1;
}
#elif defined(__APPLE__)
// iOS or Mac platform
#include <libkern/OSAtomic.h>
inline int OdInterlockedIncrement(volatile int* dest)
{
  return OSAtomicIncrement32Barrier(dest);
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return OSAtomicDecrement32Barrier(dest);
}
/*#elif defined(ANDROID) && !defined(ANDROID_GOOGLE)
//TODO Android r10c NDK doesn't provide such functionality
// Android platform with GCC or MinGW compiler
#include <sys/atomics.h>
inline int OdInterlockedIncrement(volatile int* dest)
{
  return __atomic_inc(dest) + 1;
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return __atomic_dec(dest) - 1;
}
#define OdInterlockedIncrement(dest) InterlockedIncrement((LONG*)(dest))
#define OdInterlockedDecrement(dest) InterlockedDecrement((LONG*)(dest))*/
#elif defined(ANDROID) //&& defined(ANDROID_GOOGLE)
#include <atomic>
inline int OdInterlockedIncrement(volatile int* dest)
{
  return __atomic_fetch_add(dest, 1, std::memory_order_relaxed) + 1;
}
inline int OdInterlockedDecrement(volatile int* dest)
{
  return __atomic_fetch_sub(dest, 1, std::memory_order_relaxed) - 1;
}
#else
// here should go other platforms
// synchronization is disabled if no atomic functions are defined
#define TD_SINGLE_THREAD
#endif //architecture
#endif //TD_SINGLE_THREAD

#endif //_OD_INTERLOCKED_INCREMENT_H_
