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
//
// BaseSelectionSet.h
//

#ifndef BASE_SSET_H_
#define BASE_SSET_H_

#include "SSet.h"
#include "SmartPtr.h"
#include "OdArray.h"
#include "RxObjectImpl.h"
#include "Gs/GsSelectionReactor.h"

#define STL_USING_SET
#define STL_USING_MAP
#include "OdaSTL.h"

template <class ObjId>
struct OdSelectionInfo
{
  // Less function for using in std::set and std::map containers.
  // Note that ObjId's are compared as pointers, and their ordering may change from run to run.
  // Keep that in mind when iterating through a container.
  struct less
  {
    bool operator()(const OdDbBaseFullSubentPath& fsp1, const OdDbBaseFullSubentPath& fsp2) const
    {
      const OdDbSubentId & sid1 = fsp1.subentId();
      const OdDbSubentId & sid2 = fsp2.subentId();
      if (sid1.index() != sid2.index())
        return sid1.index() < sid2.index();
      if (sid1.type() != sid2.type())
        return sid1.type() < sid2.type();
      const OdDbStubPtrArray& oids1 = fsp1.objectIds();
      const OdDbStubPtrArray& oids2 = fsp2.objectIds();
      const int len1 = oids1.length();
      const int len2 = oids2.length();
      if (len1 != len2)
        return len1 < len2;
      for (int i = 0; i < len1; ++i)
      {
        ObjId oid1 = oids1[i];
        ObjId oid2 = oids2[i];
        if (oid1 != oid2)
          return oid1 < oid2;
      }
      return false;
    }
  };

  OdDbSelectionMethodPtr first;
  OdDbBaseFullSubentPathArray second;
  typedef std::set<OdDbBaseFullSubentPath, typename OdSelectionInfo::less> FspSet;
  FspSet secondAsSet;

  void appendFsp(const OdDbBaseFullSubentPath & fsp)
  {
    this->secondAsSet.insert(fsp);
    if (this->second.physicalLength() >= 32)
      this->second.setGrowLength(-75);
    this->second.append(fsp);
  }

  void removeFsp(const OdDbBaseFullSubentPath & fsp)
  {
    this->secondAsSet.erase(fsp);
    this->second.remove(fsp);
  }

  bool containsFsp(const OdDbBaseFullSubentPath & fsp) const
  {
    return this->secondAsSet.find(fsp) != this->secondAsSet.end();
  }

  OdSelectionInfo() {}
  OdSelectionInfo(OdDbSelectionMethodPtr m, const OdDbBaseFullSubentPathArray& a)
  {
    this->first = m;
    this->second = a;
    for (unsigned int i = 0; i < a.length(); ++i)
      this->secondAsSet.insert(a[i]);
  }
  // OdSelectionInfo(const OdSelectionInfo& s) = default;
  // OdSelectionInfo& operator =(const OdSelectionInfo& s) = default;

  bool operator ==(const OdSelectionInfo& other) const
  {
    if (first != other.first)
      return false;
    if (second.length() != other.second.length())
      return false;
    for (OdUInt32 i = 0; i < second.size(); ++i)
      if (!(second[i] == other.second[i]))
        return false;
    return true;
  }
};

template <class> class OdBaseSelectionSet;

// TODO //#define OdConstIterator(ObjId) OdArray<ObjId, OdObjectsAllocator<ObjId> >::const_iterator
#define OdConstIterator(ObjId) const ObjId* 

//template <class ObjId> typedef std::multimap<ObjId, OdSelectionInfo<ObjId> > OdBaseSSetContainer; 
#define OdBaseSSetContainer typename std::multimap<ObjId, OdSelectionInfo<ObjId> >
// or via // template <typename T> using ... in C++ 11
//
#define OdContainerIterator(ObjId) OdBaseSSetContainer::const_iterator
////#define OdContainerIterator(ObjId) const OdSelectionInfo<ObjId> *
// std::_Tree_const_iterator<std::_Tree_val<std::_Tree_simple_types<_Ty> > >
//
//template <typename T>
//void do_something(T var)  // Also consider "const T&"
//{
//  // do something
//}
//
//template <typename TContainer>
//void func(const TContainer& container)
//{
//  for (typename TContainer::iterator it = container.begin();
//    it != container.end(); ++it)
//  {
//    do_something(it->second);
//  }
//}


template <class ObjId>
class OdBaseSelectionSetIterator : public OdRxObjectImpl<OdSelectionSetIterator> //public OdSelectionSetIterator
{
private:
  OdBaseSelectionSetIterator(); // disable
protected:
  OdSmartPtr<OdBaseSelectionSet<ObjId> >  m_pParent;
  OdConstIterator(ObjId) m_pItem; // const ObjId* m_pItem;
  OdConstIterator(ObjId) m_pEndItem; // const ObjId* m_pEndItem;

public:
  OdBaseSelectionSetIterator(const OdBaseSelectionSet<ObjId>* pParent);

  static OdSmartPtr<OdBaseSelectionSetIterator> createObject(const OdBaseSelectionSet<ObjId>* pParent);
  virtual OdDbSelectionMethodPtr method() const;

  virtual bool done() const 
  { 
    return m_pItem == m_pEndItem; 
  }

  virtual bool next()
  {
    if (done())
      return false;
    ++m_pItem;
    return !done();
  }

  virtual OdUInt32 subentCount();

  ObjId objectId() const
  {
    ObjId id(*m_pItem);
    return id;
  }

  virtual bool getSubentity(OdUInt32 i, OdDbBaseFullSubentPath& path);

  virtual OdDbStub* id() const 
  { 
    return *m_pItem; 
  }
};

//static
template <class ObjId>
OdSmartPtr<OdBaseSelectionSetIterator<ObjId> > 
  OdBaseSelectionSetIterator<ObjId>::createObject(const OdBaseSelectionSet<ObjId>* pParent)
{
  return OdSmartPtr<OdBaseSelectionSetIterator<ObjId> >(
           new OdBaseSelectionSetIterator<ObjId>(pParent), kOdRxObjAttach);
}

template <class ObjId>
OdBaseSelectionSetIterator<ObjId>::OdBaseSelectionSetIterator(const OdBaseSelectionSet<ObjId>* pParent)
  : m_pParent(pParent)
  , m_pItem(pParent->m_idArray.begin())
  , m_pEndItem(pParent->m_idArray.end())
{
}

typedef bool(*TCheckToSelectCallback)(const OdRxObject* pRxDb,
                                      OdDbStub*& id, // in / out
                                      OdDbStub* parentId,
                                      void* pData );

template <class ObjId>
class OdBaseSelectionSet : public OdSelectionSet
{
  friend class OdBaseSelectionSetIterator<ObjId>;

  const OdRxObject* m_pRxDb;
  OdBaseSSetContainer m_map;
  OdArray<ObjId> m_idArray;

protected:
  OdBaseSelectionSet();
public:
  //OdBaseSelectionSet();
  virtual ~OdBaseSelectionSet();
  bool operator ==(const OdBaseSelectionSet& other) const
  {
    return    m_pRxDb == other.m_pRxDb
      && m_idArray == other.m_idArray; // && m_map == other.m_map;
  }

  static OdSmartPtr<OdBaseSelectionSet> createObject(
                                          const OdRxObject* pRxDb);
  static OdSmartPtr<OdBaseSelectionSet<ObjId> > select(
                                                  OdRxObject* pRxDb, OdGsView* gsView,
                                                  int nPoints, const OdGePoint3d* wcsPts,
                                                  OdDbVisualSelection::Mode mode = OdDbVisualSelection::kCrossing,
                                                  OdDbVisualSelection::SubentSelectionMode sm = OdDbVisualSelection::kDisableSubents,
                                                  TCheckToSelectCallback checkToSelectCallback = NULL,
                                                  void* checkToSelectData = NULL,
                                                  OdUInt32 pickBoxSize = 3);

  // ----- OdSelectionSet : -----

  virtual const OdRxObject* baseDatabase() const;
  virtual OdSelectionSetIteratorPtr newIterator() const;
  virtual OdUInt32 numEntities() const;
  virtual void append(OdDbStub* entityId, OdDbSelectionMethod* pMethod = 0);
  virtual void append(const OdDbBaseFullSubentPath& subent, OdDbSelectionMethod* pMethod = 0);
  virtual void append(const OdSelectionSet* pSSet);
  virtual void remove(OdDbStub* entityId);
  virtual void remove(const OdDbBaseFullSubentPath& subent);
  virtual void remove(const OdSelectionSet* pSSet);
  virtual bool isMember(OdDbStub* entityId) const;
  virtual bool isMember(const OdDbBaseFullSubentPath& subent) const;
  virtual OdDbSelectionMethodPtr method(OdDbStub* entityId) const;
  virtual void clear();

  // ----- OdBaseSelectionSet methods: -----

  void append(const ObjId& objId, OdDbSelectionMethod* method);
  void remove(const ObjId& objId);
  OdDbSelectionMethodPtr method(const ObjId& objId) const;
  OdDbSelectionMethodPtr method(const OdDbBaseFullSubentPath& subent) const;
  bool isMember(const ObjId& objId) const;
  OdUInt32 subentCount(const ObjId& objId) const;
  bool getSubentity(const ObjId& objId, OdUInt32 i, OdDbBaseFullSubentPath& path) const;

  OdArray<ObjId> objectIdArray() const;
  void append(const OdArray<ObjId>& entityIds, OdDbSelectionMethod* pMethod = 0);
  void remove(const OdArray<ObjId>& entityIds);
};

template <class ObjId>
OdBaseSelectionSet<ObjId>::OdBaseSelectionSet()
  : m_pRxDb(NULL)
{
}

template <class ObjId>
void OdBaseSelectionSet<ObjId>::append(const OdArray<ObjId>& entityIds, OdDbSelectionMethod* pMethod)
{
  OdConstIterator(ObjId) pId = entityIds.begin();
  OdConstIterator(ObjId) pEndId = entityIds.end();
  while (pId != pEndId)
  {
    append(*pId, pMethod);
    ++pId;
  }
}

template <class ObjId>
void OdBaseSelectionSet<ObjId>::append(const OdSelectionSet* pSSet)
{
  OdSmartPtr<OdBaseSelectionSetIterator<ObjId> > pIter = pSSet->newIterator();
  //const OdBaseSelectionSet<ObjId>* pDbSSet = OdBaseSelectionSet<ObjId>::cast(pSSet).get();
  const OdBaseSelectionSet<ObjId>* pDbSSet = reinterpret_cast<const OdBaseSelectionSet<ObjId>* >(pSSet);
  if (!pDbSSet)
    return;

  while (!pIter->done())
  {
    ObjId objId = pIter->objectId();
    append(objId, pIter->method());
    // Copy subentities
    for (OdUInt32 se = 0; se < pDbSSet->subentCount(objId); se++)
    {
      OdDbBaseFullSubentPath subEntPath;
      if (pDbSSet->getSubentity(objId, se, subEntPath))
      {
        append(subEntPath, pDbSSet->method(subEntPath));
      }
    }
    //
    pIter->next();
  }
}

template <class ObjId>
void OdBaseSelectionSet<ObjId>::remove(const OdArray<ObjId>& entityIds)
{
  OdConstIterator(ObjId) pId = entityIds.begin();
  OdConstIterator(ObjId) pEndId = entityIds.end();
  while (pId != pEndId)
  {
    remove(*pId);
    ++pId;
  }
}

template <class ObjId>
void OdBaseSelectionSet<ObjId>::remove(const OdSelectionSet* pSSet)
{
  OdSmartPtr<OdBaseSelectionSetIterator<ObjId> > pIter = pSSet->newIterator();
  //const OdBaseSelectionSet<ObjId>* pDbSSet = OdBaseSelectionSet<ObjId>::cast(pSSet).get();
  const OdBaseSelectionSet<ObjId>* pDbSSet = reinterpret_cast<const OdBaseSelectionSet<ObjId>*>(pSSet);
  if (!pDbSSet)
    return;

  while (!pIter->done())
  {
    // Remove subentities first
    ObjId objId = pIter->objectId();
    for (OdUInt32 se = 0; se < pDbSSet->subentCount(objId); se++)
    {
      OdDbBaseFullSubentPath subEntPath;
      if (pDbSSet->getSubentity(objId, se, subEntPath))
        remove(subEntPath);
    }
    // Remove object only if all subentities cleared
    //remove(pIter->objectId());
    {
      OdSmartPtr<OdBaseSelectionSetIterator<ObjId> > pIterInt = newIterator();
      while (!pIterInt->done())
      {
        if (pIterInt->objectId() == pIter->objectId())
        {
          if (pIterInt->subentCount() == 0)
          {
            remove(pIter->objectId());
            break;
          }
        }
        pIterInt->next();
      }
    }
    pIter->next();
  }
}

template <class ObjId>
OdDbSelectionMethodPtr OdBaseSelectionSetIterator<ObjId>::method() const
{
  return m_pParent->method(*m_pItem);
}

template <class ObjId>
OdDbSelectionMethodPtr OdBaseSelectionSet<ObjId>::method(const ObjId& id) const
{
  OdContainerIterator(ObjId) i = m_map.find(id);
  if (i == m_map.end())
    return OdDbSelectionMethodPtr();
  return i->second.first;
}

template <class ObjId>
OdDbSelectionMethodPtr OdBaseSelectionSet<ObjId>::method(OdDbStub* id) const
{
  return method(ObjId(id));
}

template <class ObjId>
OdBaseSelectionSet<ObjId>::~OdBaseSelectionSet()
{
  clear();
}

template <class ObjId>
OdSelectionSetIteratorPtr OdBaseSelectionSet<ObjId>::newIterator() const
{
  OdSmartPtr<OdBaseSelectionSetIterator<ObjId> > pItr
    = OdBaseSelectionSetIterator<ObjId>::createObject(this);
  return pItr;
}

template <class ObjId>
OdArray<ObjId> OdBaseSelectionSet<ObjId>::objectIdArray() const
{
  return m_idArray;
}

template <class ObjId>
const OdRxObject* OdBaseSelectionSet<ObjId>::baseDatabase() const
{
  return m_pRxDb;
}

// operations on the selection set 

template <class ObjId>
OdUInt32 OdBaseSelectionSet<ObjId>::numEntities()  const
{
  return (OdUInt32)m_idArray.size();
}

template <class ObjId>
void  OdBaseSelectionSet<ObjId>::append(const ObjId& id, OdDbSelectionMethod* method)
{
  if (!isMember(id))
  {
    m_map.insert(OdBaseSSetContainer::value_type(id, OdSelectionInfo<ObjId>(method, OdDbBaseFullSubentPathArray())));
    m_idArray.append(id);
  }
}

template <class ObjId>
void OdBaseSelectionSet<ObjId>::append(OdDbStub* id, OdDbSelectionMethod* method)
{
  append(ObjId(id), method);
}

template <class ObjId>
void OdBaseSelectionSet<ObjId>::append(const OdDbBaseFullSubentPath& subent, OdDbSelectionMethod* pMethod)
{
  if (subent.objectIds().size() == 0)
    return; // invalid subent path
  ObjId parent = subent.objectIds().first();
  OdBaseSSetContainer::iterator it = m_map.find(parent);
  if (it == m_map.end())
  {
    OdDbBaseFullSubentPathArray a;
    a.append(subent);
    m_map.insert(OdBaseSSetContainer::value_type(parent, OdSelectionInfo<ObjId>(pMethod, a)));
    m_idArray.append(parent);
  }
  else
  {
    OdBaseSSetContainer::iterator saved_start = it;
    // check for duplicates
    while (it != m_map.end() && it->first == parent)
    {
      if (it->second.containsFsp(subent))
        return;
      ++it;
    }
    it = saved_start;
    // check for existing methods
    while (it != m_map.end() && it->first == parent)
    {
      if (it->second.first == OdDbSelectionMethodPtr(pMethod))
      {
        it->second.appendFsp(subent);
        return;
      }
      ++it;
    }
    // new subentity & selection method
    OdDbBaseFullSubentPathArray a;
    a.append(subent);
    m_map.insert(OdBaseSSetContainer::value_type(parent, OdSelectionInfo<ObjId>(pMethod, a)));
  }
}

template <class ObjId>
OdDbSelectionMethodPtr OdBaseSelectionSet<ObjId>::method(const OdDbBaseFullSubentPath& subent) const
{
  if (subent.objectIds().size() == 0)
    return OdDbSelectionMethodPtr(); // invalid subent path
  ObjId parent = subent.objectIds().first();
  OdContainerIterator(ObjId) it = m_map.find(parent);
  while (it != m_map.end() && it->first == parent)
  {
    if (it->second.containsFsp(subent))
      return it->second.first;
    ++it;
  }
  return OdDbSelectionMethodPtr();
}

template <class ObjId>
void OdBaseSelectionSet<ObjId>::remove(const OdDbBaseFullSubentPath& subent)
{
  if (subent.objectIds().size() == 0)
    return; // invalid subent path
  ObjId parent = subent.objectIds().first();
  OdBaseSSetContainer::iterator it = m_map.find(parent);
  while (it != m_map.end() && it->first == parent)
  {
    if (it->second.containsFsp(subent))
    {
      it->second.removeFsp(subent);
      if (it->second.second.length() == 0)  //no more subentities for this parent
      {
        OdBaseSSetContainer::iterator it2 = it;
        ++it2;
        m_map.erase(it);
        it = it2;
        continue;
      }
    }
    ++it;
  }
  if (m_map.find(parent) == m_map.end())
    m_idArray.remove(parent);
}

template <class ObjId>
void OdBaseSelectionSet<ObjId>::remove(OdDbStub* id)
{
  remove(ObjId(id));
}

template <class ObjId>
bool OdBaseSelectionSet<ObjId>::isMember(const OdDbBaseFullSubentPath& subent) const
{
  if (subent.objectIds().size() == 0)
    return false; // invalid subent path
  ObjId parent = subent.objectIds().first();
  OdContainerIterator(ObjId) it = m_map.find(parent);
  while (it != m_map.end() && it->first == parent)
  {
    if (it->second.containsFsp(subent))
      return true;
    ++it;
  }
  return false;
}

template <class ObjId>
void OdBaseSelectionSet<ObjId>::remove(const ObjId& id)
{
  for (OdBaseSSetContainer::iterator it = m_map.find(id); it != m_map.end();)
  {
    if (it->first != id)
      break;
    m_map.erase(it++);
  }
  m_idArray.remove(id);
}

template <class ObjId>
bool OdBaseSelectionSet<ObjId>::isMember(const ObjId& id) const
{
  return m_map.find(id) != m_map.end();
}

template <class ObjId>
bool OdBaseSelectionSet<ObjId>::isMember(OdDbStub* id) const
{
  return isMember(ObjId(id));
}

template <class ObjId>
void  OdBaseSelectionSet<ObjId>::clear()
{
  m_map.clear();
  m_idArray.clear();
}

template <class ObjId>
OdUInt32 OdBaseSelectionSetIterator<ObjId>::subentCount()
{
  if (m_pItem == m_pEndItem)
    return 0;
  return m_pParent->subentCount(*m_pItem);
}

template <class ObjId>
OdUInt32 OdBaseSelectionSet<ObjId>::subentCount(const ObjId& entityId) const
{
  OdContainerIterator(ObjId) it = m_map.find(entityId);
  if (it == m_map.end())
    return 0;
  OdUInt32 count = 0;
  while (it != m_map.end() && it->first == entityId)
  {
    count += it->second.second.length();
    ++it;
  }
  return count;
}

template <class ObjId>
bool OdBaseSelectionSetIterator<ObjId>::getSubentity(OdUInt32 i, OdDbBaseFullSubentPath& path)
{
  if (m_pItem == m_pEndItem)
    return false;
  return m_pParent->getSubentity(objectId(), i, path);
}

template <class ObjId>
bool OdBaseSelectionSet<ObjId>::getSubentity(const ObjId& entityId, OdUInt32 i,
                                             OdDbBaseFullSubentPath& path) const
{
  OdContainerIterator(ObjId) it = m_map.find(entityId);
  if (it == m_map.end())
    return false;
  for (; it != m_map.end() && it->first == entityId; ++it)
  {
    if (i < it->second.second.length())
    {
      path = it->second.second[i];
      return true;
    }
    i -= it->second.second.length();
  }
  return false;
}

//static
template <class ObjId>
OdSmartPtr<OdBaseSelectionSet<ObjId> > OdBaseSelectionSet<ObjId>::createObject(const OdRxObject* pRxDb)
{
  if (!pRxDb)
    throw OdError(eNoDatabase);
  OdSmartPtr<OdBaseSelectionSet<ObjId> > pRes
    = OdRxObjectImpl<OdBaseSelectionSet<ObjId> >::createObject();
  pRes->m_pRxDb = pRxDb;
  return pRes;
}

//static
template <class ObjId>
OdSmartPtr<OdBaseSelectionSet<ObjId> > OdBaseSelectionSet<ObjId>::select(
                                            OdRxObject* pRxDb, OdGsView* gsView,
                                            int nPoints, const OdGePoint3d* wcsPts,
                                            OdDbVisualSelection::Mode mode, // = OdDbVisualSelection::kCrossing
                                            OdDbVisualSelection::SubentSelectionMode sm, // = OdDbVisualSelection::kDisableSubents
                                            TCheckToSelectCallback checkToSelectCallback, // = NULL
                                            void* checkToSelectData, // = NULL
                                            OdUInt32 pickBoxSize) // = 3
{
  if (!pRxDb || !gsView || !gsView->userGiContext())
    return OdSelectionSetPtr();
  ODA_ASSERT_ONCE(pRxDb == gsView->userGiContext()->database());

  OdGeMatrix3d xWorldToScreen = gsView->worldToDeviceMatrix();

  OdGsView::SelectionMode gsSelMode = OdGsView::kCrossing;
  switch (mode)
  {
  case OdDbVisualSelection::kPoint:
    gsSelMode = OdGsView::kPoint;
    break;
  case OdDbVisualSelection::kBox:
    if (nPoints != 2)
      throw OdError(eInvalidInput);
    gsSelMode = (gsView->viewingMatrix() * wcsPts[0]).x < (gsView->viewingMatrix() * wcsPts[1]).x
      ? OdGsView::kWindow : OdGsView::kCrossing;
    break;
  case OdDbVisualSelection::kWindow:
    gsSelMode = OdGsView::kWindow;
    break;
  case OdDbVisualSelection::kCrossing:
    gsSelMode = OdGsView::kCrossing;
    break;
  case OdDbVisualSelection::kFence:
    gsSelMode = OdGsView::kFence;
    break;
  case OdDbVisualSelection::kWPoly:
    gsSelMode = OdGsView::kWPoly;
    break;
  case OdDbVisualSelection::kCPoly:
    gsSelMode = OdGsView::kCPoly;
    break;
  default:
    ODA_FAIL_ONCE();
    break;
  }

  OdGePoint3d p;
  OdGsDCPointArray dcpts;
  if (nPoints > 2 || mode >= OdDbVisualSelection::kFence)
  {
    dcpts.resize(nPoints);
    for (int i = 0; i < nPoints; ++i)
    {
      p = xWorldToScreen * wcsPts[i];
      dcpts[i].x = OdRoundToLong(p.x);
      dcpts[i].y = OdRoundToLong(p.y);
    }
  }
  else if (nPoints == 1)
  {
    dcpts.resize(2);
    p = xWorldToScreen * *wcsPts;
    {
      dcpts[0].x = OdRoundToLong(p.x) - pickBoxSize;
      dcpts[0].y = OdRoundToLong(p.y) - pickBoxSize;
      dcpts[1].x = OdRoundToLong(p.x) + pickBoxSize;
      dcpts[1].y = OdRoundToLong(p.y) + pickBoxSize;
    }
  }
  else if (nPoints == 2)
  {
    dcpts.resize(2);
    p = xWorldToScreen * wcsPts[0];
    dcpts[0].x = OdRoundToLong(p.x);
    dcpts[0].y = OdRoundToLong(p.y);
    p = xWorldToScreen * wcsPts[1];
    dcpts[1].x = OdRoundToLong(p.x);
    dcpts[1].y = OdRoundToLong(p.y);
  }

  OdSmartPtr<OdBaseSelectionSet<OdDAIObjectId> > pRes = OdBaseSelectionSet<OdDAIObjectId>::createObject(pRxDb);

  if (mode >= OdDbVisualSelection::kFence && nPoints < 2 || mode > OdDbVisualSelection::kFence && nPoints < 3)
    return pRes; // PICKBOX sysvar not taked into account in this modes

  //const OdRxObject* pFilter = NULL;
  //OdDgSelectionFilterPtr pSFilter = ::selectionFilter(pDb, pFilter);
  //OdDgElementId idActiveModel = pDb->getActiveModelId();
  //OdDgElementId idActive = gsView->rootDrawableIdAt(0);
  //if (idActive.isNull())
  //  idActive = idActiveModel;
  //std::map<OdDgElementId, OdDgElementId> nrcViewports;

  struct OdBaseSelectionReactor : OdGsSelectionReactor
  {
    TCheckToSelectCallback m_checkToSelectCallback;
    void* m_pCheckToSelectData;
    //OdDgSelectionFilter*                     m_pFilter;
    //OdDgElementId                            m_spaceId;
    OdBaseSelectionSet<OdDAIObjectId>* m_pSSet;
    OdDbVisualSelection::Mode                m_selMode;
    OdDbVisualSelection::SubentSelectionMode m_ssm;
    OdGePoint3d                              m_Pt;
    //std::map<OdDgElementId, OdDgElementId>  *m_nrcViewports;
    //bool                                     m_bCheckNrcViewports;

    OdBaseSelectionReactor(
        OdBaseSelectionSet<OdDAIObjectId>* pSSet,
        TCheckToSelectCallback checkToSelectCallback,
        void* pCheckToSelectData,
        //OdDgElementId spaceId,
        //OdDgSelectionFilter* pFilter,
        //std::map<OdDgElementId, OdDgElementId> *nrcViewports,
        OdDbVisualSelection::Mode selMode,
        OdDbVisualSelection::SubentSelectionMode ssm,
        OdGePoint3d pt)
      : m_pSSet(pSSet)
      , m_checkToSelectCallback(checkToSelectCallback)
      , m_pCheckToSelectData(pCheckToSelectData)
    //, m_pFilter(pFilter)
    //, m_spaceId(spaceId)
      , m_selMode(selMode)
      , m_ssm(ssm)
      , m_Pt(pt)
    //, m_nrcViewports(nrcViewports)
    {
      ODA_ASSERT_ONCE(m_pSSet && m_pSSet->baseDatabase());
      //m_bCheckNrcViewports = nrcViewports != NULL && !nrcViewports->empty();
    }
    virtual OdUInt32 selected(const OdGiPathNode& pathNode, const OdGiViewport& viewInfo)
    {
      return (OdUInt32)OdGsSelectionReactor::kNotImplemented;
    }
    bool selected(const OdGiDrawableDesc& drawableDesc)
    {
      const OdGiDrawableDesc* pDesc = &drawableDesc;
      OdDbStub* id = pDesc->persistId;
      if (pDesc->pParent)
      {
        while (pDesc->pParent->pParent)
          pDesc = pDesc->pParent;

        if (pDesc->pParent && pDesc->persistId) // && pDesc->pParent->persistId == m_spaceId)
        {
          id = pDesc->persistId;

          pDesc->markToSkip(true); // regen abort for selected drawable
          if (m_selMode == OdDbVisualSelection::kPoint)
            pDesc->markToBreak(true); // disable other drawables selection

          OdDbStub* parentId = pDesc->pParent->persistId;
          if (   !pDesc->pParent || !m_checkToSelectCallback
              || m_checkToSelectCallback(m_pSSet->baseDatabase(), id, parentId, m_pCheckToSelectData))
          {
            m_pSSet->append(id);
            return true;
          }
        }
      }
      else //if (id == m_spaceId)
      {
        pDesc->markToSkip(true); // regen abort for selected drawable
        if (m_selMode == OdDbVisualSelection::kPoint)
          pDesc->markToBreak(true); // disable other drawables selection

        if (   !m_checkToSelectCallback
            || m_checkToSelectCallback(m_pSSet->baseDatabase(), id, NULL, m_pCheckToSelectData))
        {
          m_pSSet->append(id);
          return true; // ?TODO
        }
      }
      return false;
    }
  }
  selectionReactor(pRes, 
                   checkToSelectCallback, checkToSelectData, //idActive, pSFilter.get(), &nrcViewports
                   mode, sm, p);

  gsView->select(dcpts.getPtr(), dcpts.size(), &selectionReactor, gsSelMode);
  return pRes;
}

#endif // BASE_SSET_H_

