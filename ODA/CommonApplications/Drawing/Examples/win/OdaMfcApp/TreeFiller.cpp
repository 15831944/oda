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

#include "stdafx.h"
#include "OdToolKit.h"
#include "TreeFiller.h"
#include "DbSymbolTableRecord.h"
#include "DbDictionary.h"
#include "DbBlockTableRecord.h"
#include "DbBlockEnd.h"
#include "DbSequenceEnd.h"
#include "Db2dPolyline.h"
#include "Db3dPolyline.h"
#include "DbPolyFaceMesh.h"
#include "DbPolygonMesh.h"
#include "DbMInsertBlock.h"
#include "DbBlockReference.h"
#include "DbXrecord.h"
#include "DbProxyExt.h"

bool g_bTreeExpandPE_init = false;
const OdRxClass* g_pAECDbObject_class = 0;
const OdRxClass* g_pAECDbEntity_class = 0;
AECOwnershipTreeExpandPE* g_pTreeExpandPE = 0;

bool isAECObjectOrEntity( const OdRxClass* pClass ) {
  if( !g_bTreeExpandPE_init ) {
    g_pTreeExpandPE = (AECOwnershipTreeExpandPE*)odrxServiceDictionary()->
      getAt( L"AECOwnershipTreeExpandPE" ).get();
    if( g_pTreeExpandPE ) {
      g_pAECDbObject_class = (const OdRxClass*)odrxClassDictionary()->getAt( L"AECDbObject" ).get();
      g_pAECDbEntity_class = (const OdRxClass*)odrxClassDictionary()->getAt( L"AECDbEntity" ).get();
    }
    g_bTreeExpandPE_init = true;
  }
  if( g_pAECDbObject_class && g_pAECDbEntity_class ) {
    while( pClass ) {
      if( pClass == g_pAECDbObject_class ||
          pClass == g_pAECDbEntity_class ) {
          return true;
      }
      pClass = pClass->myParent();
    }
  }
  return false;
}

TreeFiller::TreeFiller(OdDbDatabase* pDb, CTreeCtrl *pObjTree, OdDbSelectionSet* ss)
  : m_pObjTree(pObjTree)
  , m_pDb(pDb)
  , m_sset(ss)
  , m_nEmbeddedSubobjectFollowState(0)
{
  m_ItemStack.push(std::pair<HTREEITEM, OdDbDictionary*>(TVI_ROOT, (OdDbDictionary*)NULL));
}

TreeFiller::~TreeFiller()
{
}

void TreeFiller::wrSoftOwnershipId(const OdDbObjectId& id)
{
  addObject2Tree(id, m_ItemStack.top().first);
}

void TreeFiller::wrHardOwnershipId(const OdDbObjectId& id)
{
  addObject2Tree(id, m_ItemStack.top().first);
}

void TreeFiller::addReference(OdDbObjectId id, OdDb::ReferenceType rt)
{
  if ( (rt & OdDb::kSoftOwnershipRef)!=0 )
  {
    addObject2Tree(id, m_ItemStack.top().first);
  }
}

void TreeFiller::wrSoftPointerId(const OdDbObjectId& )
{
}

void TreeFiller::wrHardPointerId(const OdDbObjectId& )
{
}

void TreeFiller::wrAddress(const void* val) {
  switch( m_nEmbeddedSubobjectFollowState ) {
    case 0:
      // "this" pointer used as an identifier that embedded subobject follows
      // Caller calls: pFiler->wrAddress( pFiler ) and then immediately - pFiler->wrAddress( pEmbeddedSubobject )
      if( val == this )
        m_nEmbeddedSubobjectFollowState = 1;
      break;
    case 1:
      addEmbeddedObject2Tree((const OdRxObject*)val, m_ItemStack.top().first);
      m_nEmbeddedSubobjectFollowState = 2;
      break;
    case 2:
      *((bool*)val) = true; // message to caller that filer has processed trick
      m_nEmbeddedSubobjectFollowState = 0;
      break;
  }
}

ChildrenCounter::ChildrenCounter(OdDbDatabase* pDb)
  : m_pDb(pDb)
  , m_nEmbeddedSubobjectFollowState(0)
{
}

OdDbDatabase* ChildrenCounter::database() const
{
  return m_pDb;
}

void ChildrenCounter::wrSoftOwnershipId(const OdDbObjectId& id)
{
  if(!id.isNull())
    ++m_nChildCount;
}

void ChildrenCounter::wrHardOwnershipId(const OdDbObjectId& id)
{
  if(!id.isNull())
    ++m_nChildCount;
}

void ChildrenCounter::wrSoftPointerId(const OdDbObjectId& )
{
}

void ChildrenCounter::wrHardPointerId(const OdDbObjectId& )
{
}

// D.Novikov: trick for ADT debugging
void ChildrenCounter::wrAddress(const void* val) {
  switch( m_nEmbeddedSubobjectFollowState ) {
    case 0:
      // "this" pointer used as an identifier that embedded subobject follows
      // Caller calls: pFiler->wrAddress( pFiler ) and then immediately - pFiler->wrAddress( pEmbeddedSubobject )
      if( val == this )
        m_nEmbeddedSubobjectFollowState = 1;
      break;
    case 1:
      if( val )
        ++m_nChildCount;
      m_nEmbeddedSubobjectFollowState = 2;
      break;
    case 2:
      *((bool*)val) = (g_pTreeExpandPE!=0); // message to caller that filer has processed trick
      m_nEmbeddedSubobjectFollowState = 0;
      break;
  }
}

int ChildrenCounter::countChildrenEmbedded(const OdRxObject* pObj)
{
  m_nChildCount = 0;
  if( g_pTreeExpandPE ) {
    g_pTreeExpandPE->outFields( pObj, this );
  }
  return m_nChildCount;
}

HTREEITEM TreeFiller::addObject2Tree(OdDbObjectId id, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
  CString sName;
  OdDbObjectPtr pObj = id.openObject();
  if(!pObj.isNull())
  {
    CString str;
    CString dictStr;
    CString prx("");
    OdDbSymbolTableRecordPtr pRec = OdDbSymbolTableRecord::cast(pObj);
    if(!pRec.isNull())
    {
      str = (LPCTSTR)pRec->getName();
    }
    else if(!m_ItemStack.top().second.isNull())
    {
      OdDbDictionaryIterator* pIter = m_ItemStack.top().second;
      if(pIter->setPosition(id))
      {
        dictStr.Format(_T("%s  "), (LPCTSTR)pIter->name());
      }
    }
    if(str.IsEmpty())
    {
      OdDbProxyExtPtr pProxyExt = OdDbProxyExt::cast(pObj);
      if(pProxyExt.get())
      {
        prx = _T(" <Proxy> : ");
        str = (LPCTSTR)pProxyExt->originalClassName(pObj);
      }
      else
      {
        if (pObj->isKindOf(OdDbDatabase::desc()) && !((OdDbDatabase*)pObj.get())->xrefBlockId().isNull())
        {
          dictStr = _T("XREF:");
          prx = _T("");
          str = (LPCTSTR)pObj->isA()->name();
        }
        else
        {
          prx = _T("");
          str = (LPCTSTR)pObj->isA()->name();
        }
      }
    }
    OdString handleStr = id.getHandle().ascii();
    sName.Format(_T("%s : %s<%s%s>"), (LPCTSTR)handleStr, (LPCTSTR)dictStr, (LPCTSTR)prx, (LPCTSTR)str);

    HTREEITEM hItem = m_pObjTree->InsertItem(sName, hParent, hInsertAfter);
    TVITEM item	  = { 0 };
    item.mask     = TVIF_HANDLE | TVIF_CHILDREN | TVIF_PARAM;
    item.hItem	  = hItem;
    if(ChildrenCounter(m_pDb).countChildren(pObj))
    {
      item.cChildren	= 1;
    }
    else
    {
      item.cChildren	= 0;
    }
    item.lParam = (LPARAM)(OdDbStub*)id;
    m_pObjTree->SetItem(&item);
    if ( m_sset->isMember(id) )
      m_pObjTree->SetItemState( hItem, TVIS_BOLD, TVIS_BOLD );
    return hItem;
  }
  return 0;
}

HTREEITEM TreeFiller::addEmbeddedObject2Tree( const OdRxObject* pObj, HTREEITEM hParent )
{
  if( pObj ) {
    CString sName;
    sName.Format(_T("{0x%08X <%s>}"), pObj, pObj->isA()->name().c_str());

    HTREEITEM hItem = m_pObjTree->InsertItem(sName, hParent, TVI_LAST);
    TVITEM item	  = { 0 };
    item.mask     = TVIF_HANDLE | TVIF_CHILDREN | TVIF_PARAM;
    item.hItem	  = hItem;
    if( ChildrenCounter(m_pDb).countChildrenEmbedded(pObj) ) {
      item.cChildren	= 1;
    }
    else {
      item.cChildren	= 0;
    }
    if( g_pTreeExpandPE ) {
      item.lParam = (LPARAM)(OdDbStub*)g_pTreeExpandPE->dbOwner( pObj );
    }
    else {
      item.lParam = m_pObjTree->GetItemData( hParent );
    }
    m_pObjTree->SetItem(&item);
    return hItem;
  }
  return 0;
}

void TreeFiller::outSubObjects2Tree(const OdDbObject* pObj, HTREEITEM hParent)
{
  OdDbDictionaryIteratorPtr pIter;
  OdDbDictionaryPtr pDict = OdDbDictionary::cast(pObj);
  if(!pDict.isNull())
    pIter = pDict->newIterator(OdRx::kDictCollated);
  m_ItemStack.push(std::pair<HTREEITEM, OdDbDictionaryIteratorPtr>(hParent, pIter));

  AECOwnershipTreeExpandPE* pExpandPE;
  if( ::isAECObjectOrEntity( pObj->isA() ) ) {
    pExpandPE = g_pTreeExpandPE;
  }
  else {
    pExpandPE = 0;
  }

  if( pExpandPE ) {
    pExpandPE->outFields( pObj, this );
  }
  else {
    pObj->dwgOutFields(this);
  }
  if (pObj->isKindOf(OdDbBlockTableRecord::desc()) && OdDbBlockTableRecordPtr(pObj)->xrefDatabase())
  {
    addObject2Tree(OdDbBlockTableRecordPtr(pObj)->xrefDatabase()->objectId(), hParent, TVI_LAST);
  }
  m_ItemStack.pop();
}

void TreeFiller::outEmbeddedSubObjects2Tree( const OdRxObject* pObj, HTREEITEM hParent )
{
  m_ItemStack.push(std::pair<HTREEITEM, OdDbDictionaryIteratorPtr>(hParent, OdDbDictionaryIteratorPtr()));

  g_pTreeExpandPE->outFields( pObj, this );

  m_ItemStack.pop();
}
