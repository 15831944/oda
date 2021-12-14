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

#ifndef _OD_MUTEX_H_
#define _OD_MUTEX_H_

#include <cstring>
#include "DebugStuff.h"
#include "RootExport.h"
#include <OdPlatform.h>
#include "TD_PackPush.h"
#include "OdInterlockedIncDec.h"
#ifdef OD_POSIX_THREADS
#include <pthread.h>
#endif
#if defined(_MSC_VER)
#include <winnt.h>
#endif

/**
	<group Other_Classes>
*/
class FIRSTDLL_EXPORT OdNonCopyable {
protected:
  OdNonCopyable() {
  }

  ~OdNonCopyable() {
  }

private:
  OdNonCopyable(const OdNonCopyable&);
  const OdNonCopyable& operator=(const OdNonCopyable&);
};

/** \details
    This class implements Mutex objects in a ODA Platform context.
    
    \remarks
    Mutex objects are used to synchronize between threads and across processes.
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
class FIRSTDLL_EXPORT OdMutex : public OdNonCopyable
{
#ifdef OD_POSIX_THREADS
  pthread_mutex_t _mutex;
public:
  OdMutex()
  {
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); 
    pthread_mutex_init(&_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
  }
  ~OdMutex()
  {
    pthread_mutex_destroy((pthread_mutex_t*)&_mutex);
  }
  /** \details
  Locks this Mutex object.
  */
  void lock()
  {
    pthread_mutex_lock(&_mutex);
  }
  /** \details
  Unlocks this Mutex object.
  */
  void unlock()
  {
    pthread_mutex_unlock(&_mutex);
  }
#elif (defined(ODA_WINDOWS)) && !defined(_WIN32_WCE) && !defined(_WINRT)
  CRITICAL_SECTION _mutex;
public:
  OdMutex()
  {
    InitializeCriticalSection(&_mutex);
  }
  ~OdMutex()
  {
    DeleteCriticalSection(&_mutex);
  }
  /** \details
  Locks this Mutex object.
  */
  void lock()
  {
    EnterCriticalSection(&_mutex);
  }
  /** \details
  Unlocks this Mutex object.
  */
  void unlock()
  {
    LeaveCriticalSection(&_mutex);
  }
#else
public:
  OdMutex() {}
  ~OdMutex() {}
  /** \details
  Locks this Mutex object.
  */
  void lock() {}
  /** \details
  Unlocks this Mutex object.
  */
  void unlock() {}
#endif
};

/** \details
    This class implements Mutex AutoLock objects in a ODA Platform context.
    
    \remarks
    Mutex objects are used to synchronize between threads and across processes.
    
    MutexAutoLock objects automatically lock the specified OdMutex object
    when constructed and unlock it when destroyed.
    
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
class OdMutexAutoLock : public OdNonCopyable
{
public:
  /** \param mutex [in]  Mutex to be Autolocked.
  */
  OdMutexAutoLock(OdMutex& mutex) : m_Mutex(mutex)
  {
    m_Mutex.lock();
  }
  ~OdMutexAutoLock()
  { 
    m_Mutex.unlock();
  }
private:
  OdMutex& m_Mutex;
};

#ifndef TD_SINGLE_THREAD
#define TD_AUTOLOCK(Mutex) OdMutexAutoLock autoLock(Mutex);
#else
#define TD_AUTOLOCK(Mutex)
#endif


#ifndef TD_SINGLE_THREAD
#if defined(_MSC_VER) && (_M_IX86) && (_M_IX86 >= 400) && !defined(_WIN32_WCE)
// Intel486 platform with Microsoft compiler
#pragma warning(push)
#pragma warning(disable:4035)
#pragma warning(disable:4793)
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  __asm
  {
    mov edx, dest
    mov eax, val
    lock xchg [edx], eax
  }
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  __asm
  {
    mov edx, dest
    mov eax, incr
    lock xadd [edx], eax
  }
}

inline int OdInterlockedCompareExchange(volatile int* dest, int x, int compare)
{
  __asm
  {
    mov eax, compare
    mov ecx, x
    mov edx, dest
    lock cmpxchg[edx], ecx
  }
}

#pragma warning(pop)
#elif (defined(_WIN32) || defined(_WIN64)) && /*!defined(_WIN32_WCE) &&*/ /*!defined(_WINRT) && */!defined(ODA_WINDOWS_GCC)
// Windows platform with generic compiler - use interlocked functions from Win32 API
#define OdInterlockedExchange(dest, val) InterlockedExchange((LONG*)(dest), val)
#define OdInterlockedExchangeAdd(dest, incr) InterlockedExchangeAdd((LONG*)(dest), incr)
#define OdInterlockedCompareExchange(dest, x, compare) InterlockedCompareExchange((LONG*)(dest), x, compare)
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)) && !defined(ANDROID) && !defined(_APPLE_)
// Intel platform with GCC compiler
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  int ret;
  __asm__ __volatile__ (
    "lock; xchgl %0, (%1)"
    : "=r" (ret)
    : "r" (dest), "0" (val)
    : "memory");
  return ret;
}
inline int OdInterlockedCompareExchange(int volatile *dest, int xchg, int compare)
{
  int ret;
  __asm__ __volatile__("lock; cmpxchgl %2,(%1)"
    : "=a" (ret) : "r" (dest), "r" (xchg), "0" (compare) : "memory");
  return ret;
}
#elif defined(__GNUC__) && defined(__POWERPC__)
// Macintosh PowerPC platform with GCC compiler
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  // Assembler code is taken from Wine 0.9.4 sources
  // See http://cvs.winehq.org/cvsweb/wine/libs/port/interlocked.c?rev=1.7&content-type=text/x-cvsweb-markup
  int ret = 0;
  __asm__ __volatile__ (
    "0:  lwarx %0, 0, %1\n"
    "    stwcx. %2, 0, %1\n"
    "    bne- 0b\n"
    "    isync\n"
    : "=&r"(ret)
    : "r"(dest), "r"(val)
    : "cr0", "memory", "r0");
  return ret;
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  // Assembler code is taken from Wine 0.9.4 sources
  // See http://cvs.winehq.org/cvsweb/wine/libs/port/interlocked.c?rev=1.7&content-type=text/x-cvsweb-markup
  int ret = 0;
  int zero = 0;
  __asm__ __volatile__ (
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

inline int OdInterlockedCompareExchange(volatile int *dest, int xchg, int compare)
{
  int ret = 0;
  int scratch;
  __asm__ __volatile__(
    "0:    lwarx %0,0,%2\n"
    "      xor. %1,%4,%0\n"
    "      bne 1f\n"
    "      stwcx. %3,0,%2\n"
    "      bne- 0b\n"
    "      isync\n"
    "1:    "
    : "=&r"(ret), "=&r"(scratch)
    : "r"(dest), "r"(xchg), "r"(compare)
    : "cr0", "memory", "r0");
  return ret;
}

#elif defined(__APPLE__)
// iOS or Mac platform
#include <libkern/OSAtomic.h>
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  int oldValue = *dest;
  while (!OSAtomicCompareAndSwapIntBarrier(oldValue, val, dest))
    oldValue = *dest;
  return oldValue;
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  return OSAtomicAdd32Barrier(incr, dest) - incr;
}
inline int OdInterlockedCompareExchange(volatile int *dest, int xchg, int compare)
{
  // This is not an exact equivalent of InterlockedCompareExchange but it is enough for atomic arithmetics
  return OSAtomicCompareAndSwapIntBarrier(compare, xchg, dest) ? compare : xchg;
}
/*#elif defined(ANDROID) && !defined(ANDROID_GOOGLE)
//TODO Android r10c NDK doesn't provide such functionality
// Android platform with GCC or MinGW compiler
#include <sys/atomics.h>
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  return __atomic_swap(val, dest);
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  // There is no atomic instruction to add. This implementation isn't completely safe.
  return __atomic_swap(*dest + incr, dest);
}
inline int OdInterlockedCompareExchange(volatile int *dest, int xchg, int compare)
{
  return __atomic_cmpxchg(dest, xchg, compare);
}
#define OdInterlockedExchange(dest, val) InterlockedExchange((LONG*)(dest), val)
#define OdInterlockedExchangeAdd(dest, incr) InterlockedExchangeAdd((LONG*)(dest), incr)
#define OdInterlockedCompareExchange(dest, x, compare) InterlockedCompareExchange((LONG*)(dest), x, compare)
struct OdVolatile
{
  typedef int VolatileType;
  volatile VolatileType _val;
  OdVolatile& operator = (VolatileType n) { _val = 0; OdInterlockedExchange(&_val, n); return *this; }
  operator VolatileType () const { return OdInterlockedExchangeAdd(const_cast<VolatileType*>(&_val), 0); }
  VolatileType operator|=(VolatileType n) { return OdInterlockedExchange(&_val, _val | n); }
  VolatileType operator&=(VolatileType n) { return OdInterlockedExchange(&_val, _val&n); }
  OdVolatile() : _val(0) {}
  OdVolatile(int n) : _val(n) {}
};*/
/* 
#elif defined(__sun)
// sparc on Sun Studio compiler, solaris
#include <atomic.h>
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  return (int)atomic_swap_uint((volatile uint_t*)dest, (uint_t)val);
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  return (int)atomic_add_int_nv((volatile uint_t*)dest, incr) - incr;
}
#elif defined (__hpux)
// HPUX (require libatomic: https://h20392.www2.hp.com/portal/swdepot/displayProductInfo.do?productNumber=Atomic)
#include <atomic.h>
inline int OdInterlockedExchange(volatile int* dest, int val)
{
  return (int)atomic_swap_32((volatile uint32_t*)dest, (uint32_t)val);
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
  // There is no atomic instruction to add. This implementation isn't completely safe.
  return (int)atomic_swap_32((volatile uint32_t*)dest, (uint32_t)(*desc + incr));
}*/
#elif defined(ANDROID) //&& defined(ANDROID_GOOGLE)

inline int OdInterlockedExchange(volatile int* dest, int val)
{
  int oldVal = *dest;
  __sync_val_compare_and_swap(dest, oldVal, val);
  return oldVal;
}
inline int OdInterlockedExchangeAdd(volatile int* dest, int incr)
{
	int oldVal = *dest;
  __sync_val_compare_and_swap(dest, oldVal, incr + oldVal);
  return oldVal;
}
inline int OdInterlockedCompareExchange(volatile int *dest, int xchg, int compare)
{
  bool bRes = __sync_bool_compare_and_swap(dest, compare, xchg);
  return bRes ? xchg : compare;
}
/**
  <group !!RECORDS_tkernel_apiref>
*/
struct OdRefCounter
{
  typedef int RefCounterType;
  volatile RefCounterType _ref_count;
  OdRefCounter& operator = (RefCounterType n) { _ref_count = 0; OdInterlockedExchange(&_ref_count, n); return *this; }
  operator RefCounterType () const { return OdInterlockedExchangeAdd(const_cast<RefCounterType*>(&_ref_count), 0); }
  RefCounterType operator ++ () { return OdInterlockedIncrement(&_ref_count); }
  RefCounterType operator -- () { return OdInterlockedDecrement(&_ref_count); }
  // 1 as default is not correct for all classes 
  // (see for example OdArrayBuffer, CAllocator)
  OdRefCounter() : _ref_count(-1) {} // undefined
  OdRefCounter(int n) : _ref_count(n) {}
};

/**
  <group !!RECORDS_tkernel_apiref>
*/
struct OdVolatile
{
  typedef int VolatileType;
  volatile VolatileType _val;
  OdVolatile& operator = (VolatileType n) { _val = 0; OdInterlockedExchange(&_val, n); return *this; }
  operator VolatileType () const { return OdInterlockedExchangeAdd(const_cast<VolatileType*>(&_val), 0); }
  VolatileType operator|=(VolatileType n) { return OdInterlockedExchange(&_val, _val | n); }
  VolatileType operator&=(VolatileType n) { return OdInterlockedExchange(&_val, _val&n); }
  OdVolatile() : _val(0) {}
  OdVolatile(int n) : _val(n) {}
};
#else
// here should go other platforms
// synchronization is disabled if no atomic functions are defined
#define TD_SINGLE_THREAD
#endif //architecture
#endif //TD_SINGLE_THREAD

/** \details
    This class implements reference counter with interlocked operations for objects.
    
    Corresponding C++ library: TD_Root
    <group Other_Classes>
*/
#if !(defined(ANDROID)) //&& defined(ANDROID_GOOGLE))
#ifndef TD_SINGLE_THREAD
#ifdef _MANAGED
#pragma managed(push, off)
#endif
/** 
  <group !!RECORDS_tkernel_apiref>
*/
  struct OdRefCounter
{
  typedef int RefCounterType;
  volatile RefCounterType _ref_count;
  OdRefCounter& operator = (RefCounterType n) { _ref_count = 0; OdInterlockedExchange(&_ref_count, n); return *this; }
  operator RefCounterType () const { return OdInterlockedExchangeAdd(const_cast<RefCounterType*>(&_ref_count), 0); }
  RefCounterType operator ++ () { return OdInterlockedIncrement(&_ref_count); }
  RefCounterType operator -- () { return OdInterlockedDecrement(&_ref_count); }
  // 1 as default is not correct for all classes 
  // (see for example OdArrayBuffer, CAllocator)
  OdRefCounter() : _ref_count(-1) {} // undefined
  OdRefCounter(int n) : _ref_count(n) {}
};

/** 
  <group !!RECORDS_tkernel_apiref>
*/
  struct OdVolatile
{
  typedef int VolatileType;
  volatile VolatileType _val;
  OdVolatile& operator = (VolatileType n) { _val = 0; OdInterlockedExchange(&_val, n); return *this; }
  operator VolatileType () const { return OdInterlockedExchangeAdd(const_cast<VolatileType*>(&_val), 0); }
  VolatileType operator|=(VolatileType n) { return OdInterlockedExchange(&_val, _val|n); }
  VolatileType operator&=(VolatileType n) { return OdInterlockedExchange(&_val, _val&n); }
  OdVolatile() : _val(0) {}
  OdVolatile(int n): _val(n) {}
};

#ifdef _MANAGED
#pragma managed(pop)
#endif

#else
typedef int OdRefCounter;
typedef int OdVolatile;
#endif
#endif //!(defined(ANDROID)) //&& defined(ANDROID_GOOGLE))
#include "TD_PackPop.h"
#endif
