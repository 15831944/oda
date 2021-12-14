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

#ifndef _ODGSBASEPERTHREADSTORAGE_H_
#define _ODGSBASEPERTHREADSTORAGE_H_

#include "ThreadsCounter.h"

// Currently we invoke simple list with minor optimization (store middle list pointer). Actually number of threads
// can't be large, so usage of std::set looks redundant. But, of course, it can be changed to use another container if need.

template <typename ObjectType>
class OdBasePerThreadStorage
{
  protected:
    struct ListEntry
    {
      unsigned m_nThreadId;
      ObjectType m_pObject;
      ListEntry *m_pNext, *m_pPrev;
    } m_defEntry;
    ListEntry *m_pFirst, *m_pLast;
    ListEntry *m_pMid;
    mutable OdMutexPtr m_mutex;
    OdUInt32 m_nEntries;
#ifdef DEBUG
    unsigned m_fakeThreadId;
#endif
  protected:
    unsigned curThreadId() const
    {
#ifdef DEBUG
      if (m_fakeThreadId)
        return m_fakeThreadId;
#endif
      return ::odGetCurrentThreadId();
    }
    ListEntry *searchSubDiv(unsigned threadId, ListEntry *pFirst, ListEntry *pLast) const
    {
      do
      {
        if (pFirst->m_nThreadId == threadId)
          return pFirst;
        if (pFirst == pLast)
          return NULL;
        if (pLast->m_nThreadId == threadId)
          return pLast;
        pFirst = pFirst->m_pNext;
        pLast = pLast->m_pPrev;
      } while (pLast->m_pNext != pFirst);
      return NULL;
    }
    ListEntry *searchByThread(unsigned threadId) const
    { if (m_nEntries)
      { if (!m_pMid)
          return searchSubDiv(threadId, m_pFirst, m_pLast);
        else
        {
          if (m_pMid->m_nThreadId == threadId)
            return m_pMid;
          if (m_pMid->m_nThreadId < threadId)
            return searchSubDiv(threadId, m_pMid, m_pLast);
          return searchSubDiv(threadId, m_pFirst, m_pMid);
        }
      }
      return NULL;
    }
    ListEntry *makeMid()
    {
      if (m_nEntries < 3)
        return NULL;
      ListEntry *pFirst = m_pFirst, *pLast = m_pLast;
      do
      {
        pFirst = pFirst->m_pNext;
        pLast = pLast->m_pPrev;
        if (pFirst == pLast)
          break;
      } while (pLast->m_pNext != pFirst);
      return pLast;
    }
    void clearImpl()
    {
      while (m_pFirst)
      {
        ListEntry *pEntryDel = m_pFirst;
        m_pFirst = m_pFirst->m_pNext;
        if (pEntryDel != &m_defEntry)
          delete pEntryDel;
      }
    }
  public:
    OdBasePerThreadStorage()
      : m_pFirst(NULL), m_pLast(NULL), m_pMid(NULL), m_nEntries(0)
    {
#ifdef DEBUG
      m_fakeThreadId = 0;
#endif
    }

    ~OdBasePerThreadStorage()
    {
      clearImpl();
    }

#ifdef DEBUG
    void setFakeThreadId(unsigned threadId)
    {
      m_fakeThreadId = threadId;
    }
#endif

    void setObject(ObjectType pObject)
    { TD_AUTOLOCK_P_DEF(m_mutex);
      const unsigned threadId = curThreadId();
      ListEntry *pEntry = searchByThread(threadId);
      if (!pEntry)
      {
        if (!m_nEntries)
          pEntry = &m_defEntry;
        else
          pEntry = new ListEntry;
        pEntry->m_nThreadId = threadId;
        pEntry->m_pNext = pEntry->m_pPrev = NULL;
        if (!m_nEntries)
          m_pFirst = m_pLast = pEntry;
        else if (m_pFirst->m_nThreadId > threadId)
          m_pFirst->m_pPrev = pEntry, pEntry->m_pNext = m_pFirst, m_pFirst = pEntry;
        else if (m_pLast->m_nThreadId < threadId)
          m_pLast->m_pNext = pEntry, pEntry->m_pPrev = m_pLast, m_pLast = pEntry;
        else
        { ListEntry *pPrev = m_pFirst;
          while (pPrev->m_pNext->m_nThreadId < threadId)
            pPrev = pPrev->m_pNext;
          pPrev->m_pNext->m_pPrev = pEntry;
          pEntry->m_pNext = pPrev->m_pNext;
          pPrev->m_pNext = pEntry;
          pEntry->m_pPrev = pPrev;
        }
        m_nEntries++;
        m_pMid = makeMid();
      }
      pEntry->m_pObject = pObject;
    }

    void resetObject()
    { TD_AUTOLOCK_P_DEF(m_mutex);
      ListEntry *pEntry = searchByThread(curThreadId());
      if (pEntry)
      {
        if (pEntry->m_pNext)
          pEntry->m_pNext->m_pPrev = pEntry->m_pPrev;
        else
          m_pLast = pEntry->m_pPrev;
        if (pEntry->m_pPrev)
          pEntry->m_pPrev->m_pNext = pEntry->m_pNext;
        else
          m_pFirst = pEntry->m_pNext;
        m_nEntries--;
        m_pMid = makeMid();
        if (pEntry != &m_defEntry)
          delete pEntry;
      }
    }

    void setObjectCmp(ObjectType pObject, ObjectType pCmpWith = NULL)
    {
      if (pObject != pCmpWith)
        setObject(pObject);
      else
        resetObject();
    }

    void clear()
    { TD_AUTOLOCK_P_DEF(m_mutex);
      clearImpl();
      m_pLast = m_pMid = NULL;
      m_nEntries = 0;
    }

    bool hasObject() const
    { TD_AUTOLOCK_P_DEF(m_mutex);
      return searchByThread(curThreadId()) != NULL;
    }

    ObjectType getObject(ObjectType pRetHasNotYet = NULL) const
    { TD_AUTOLOCK_P_DEF(m_mutex);
      ListEntry *pEntry = searchByThread(curThreadId());
      if (pEntry)
        return pEntry->m_pObject;
      return pRetHasNotYet;
    }
};

/* Test:
  int rands[10], vals[10];
  for (int n_rnd = 0; n_rnd < 10; n_rnd++)
    rands[n_rnd] = rand(), vals[n_rnd] = rand();
  OdBasePerThreadStorage<int> chk;
  for (int nAdd = 0; nAdd < 10; nAdd++)
    chk.setFakeThreadId(rands[nAdd]),
    chk.setObject(vals[nAdd]);
  for (int nGet = 0; nGet < 10; nGet++)
    chk.setFakeThreadId(rands[nGet]),
    ODA_VERIFY(chk.getObject() == vals[nGet]);
  for (int nRem = 0; nRem < 10; nRem++)
    chk.setFakeThreadId(rands[nRem]),
    chk.resetObject();
  for (int nNuGet = 0; nNuGet < 10; nNuGet++)
    chk.setFakeThreadId(rands[nNuGet]),
    ODA_VERIFY(chk.getObject() == 0);
*/

#endif //#ifndef _ODGSBASEPERTHREADSTORAGE_H_
