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

#ifndef _ODA_MFC_APP_TREE_FILLER_
#define _ODA_MFC_APP_TREE_FILLER_

#define STL_USING_STACK
#include "OdaSTL.h"

#include "RxObjectImpl.h"
#include "DbFiler.h"
#include "DbDictionary.h"
#include "OdaMfcApp.h"
#include "DbDatabase.h"
#include "StaticRxObject.h"
#include "DbSSet.h"

class ListFiller;

extern COdaMfcAppApp theApp;

class AECOwnershipTreeExpandPE : public OdRxObject {
public:
  virtual void outFields( const OdRxObject* pObj, OdDbFiler* pFiler ) = 0;
  virtual OdDbObjectId dbOwner( const OdRxObject* pObj ) = 0;
};

class ChildrenCounter : protected OdStaticRxObject<OdIdFiler>
{
  int m_nChildCount;
  OdDbDatabase* m_pDb;
  bool m_bEmbeddedSubobjectFollows;
  int m_nEmbeddedSubobjectFollowState;
public:
  ChildrenCounter(OdDbDatabase* pDb);
  int countChildren(const OdDbObject* pObj)
  {
    m_nChildCount = 0;
    pObj->dwgOutFields(this);
    return m_nChildCount;
  }

  int countChildrenEmbedded(const OdRxObject* pObj);

  OdDbDatabase* database() const;
  void wrSoftOwnershipId(const OdDbObjectId& id);
  void wrHardOwnershipId(const OdDbObjectId& id);
  void wrSoftPointerId(const OdDbObjectId& id);
  void wrHardPointerId(const OdDbObjectId& id);
  void wrAddress(const void* val);
  void addReference(OdDbObjectId id, OdDb::ReferenceType rt)
  {
    if ( (rt & OdDb::kSoftOwnershipRef)!=0 )
    {
      wrSoftOwnershipId(id);
    }
  }
};


class TreeFiller : protected OdRxObjectImpl<OdIdFiler>
{
  CTreeCtrl	* m_pObjTree;
  ListFiller* m_pListFiller;
  OdDbSelectionSet* m_sset;

  OdDbDatabase* m_pDb;
  int m_nEmbeddedSubobjectFollowState;
  std::stack<std::pair<HTREEITEM, OdDbDictionaryIteratorPtr> > m_ItemStack;
public:
  HTREEITEM addObject2Tree(OdDbObjectId id, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
  HTREEITEM addEmbeddedObject2Tree(const OdRxObject* pObj, HTREEITEM hParent = TVI_ROOT);
  TreeFiller(OdDbDatabase* pDb, CTreeCtrl *pObjTree, OdDbSelectionSet* ss);
  virtual ~TreeFiller();

  void outEmbeddedSubObjects2Tree( const OdRxObject* pObj, HTREEITEM hParent );
  void outSubObjects2Tree(const OdDbObject* pObj, HTREEITEM hParent);
  OdDbDatabase* database() const { return m_pDb; }
  void wrSoftOwnershipId(const OdDbObjectId& id);
  void wrHardOwnershipId(const OdDbObjectId& id);
  void wrSoftPointerId(const OdDbObjectId& id);
  void wrHardPointerId(const OdDbObjectId& id);
  void wrAddress(const void* val);
  void addReference(OdDbObjectId id, OdDb::ReferenceType rt);
};

#endif
