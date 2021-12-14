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
#include "DgnTreeFiller.h"
#include "ExDgnElementDumperPE.h"


HTREEITEM DgnTreeFiller::addTreeItem(  const CString& strName, bool bHasChildren, HTREEITEM hParent, HTREEITEM hInsertAfter )
{
  CString strItemName = strName;

  if( strName.IsEmpty() )
  {
    strItemName = OD_T(" ");
  }

  HTREEITEM hItem = m_pObjTree->InsertItem(strItemName, hParent, hInsertAfter);
  TVITEM item = { 0 };
  item.mask   = TVIF_HANDLE | TVIF_CHILDREN | TVIF_PARAM;
  item.hItem  = hItem;
  item.cChildren  = bHasChildren;
  item.lParam = (LPARAM)(0);

  m_pObjTree->SetItem(&item);

  return hItem;
}

HTREEITEM DgnTreeFiller::addElement(OdDgElementPtr pElm, HTREEITEM hParent, HTREEITEM hInsertAfter)
{
  if ( !pElm.isNull() )
  {
    CString sName;
    OdDgElementDumperPEPtr pElmDumper = OdDgElementDumperPE::cast( pElm );
    int hasChildren = 0;
    if ( pElmDumper.get() )
    {
      sName = (LPCTSTR)(pElmDumper->getName(pElm.get()));
      hasChildren = pElmDumper->hasSubElements(pElm);
    }
    else
    {
      sName.Format(_T("<%s>"), (LPCTSTR)(pElm->isA()->name()));
    }

    HTREEITEM hItem = m_pObjTree->InsertItem(sName, hParent, hInsertAfter);
    TVITEM item = { 0 };
    item.mask   = TVIF_HANDLE | TVIF_CHILDREN | TVIF_PARAM;
    item.hItem  = hItem;
    item.cChildren  = hasChildren;

    OdDbStub* pIdStub = (OdDbStub*)(pElm->elementId());
    item.lParam = (LPARAM)pIdStub;
    // Save NonDBRO collections in separate map
    if ( pIdStub == 0 )
    {
      m_nonDBROItems[hItem] = pElm;
    }
    m_pObjTree->SetItem(&item);
    //if ( m_sset->isMember(id) )
    //{
    //  m_pObjTree->SetItemState( hItem, TVIS_BOLD, TVIS_BOLD );
    //}
    return hItem;
  }
  return 0;
}

void DgnTreeFiller::addSubElements(OdDgElementPtr pElm, HTREEITEM hParent)
{
  OdDgElementDumperPEPtr pDumper = OdDgElementDumperPE::cast( pElm );
  if ( !pDumper.isNull() )
  {
    HTREEITEM hParentItem = hParent;

    OdDgModelPtr pModel = OdDgModel::cast( pElm );

    OdDgReferenceAttachmentHeaderPtr pXRef = OdDgReferenceAttachmentHeader::cast( pElm );
    OdDgTextStyleTablePtr pTextStyleTable = OdDgTextStyleTable::cast(pElm);

    if( !pXRef.isNull() )
    {
      OdDgLevelTablePtr pLevelTable = pXRef->getLevelTable( OdDg::kForRead);

      if( !pLevelTable.isNull() )
      {
        addElement( pLevelTable.get(), hParentItem );
      }
    }
    else if (!pTextStyleTable.isNull())
    {
      OdDgTextStyleTableRecordPtr pDefaultTextStyle = pTextStyleTable->getDefaultData();

      if (!pDefaultTextStyle.isNull())
        addElement(pDefaultTextStyle.get(), hParentItem);
    }
    else if( !pModel.isNull() )
    {
      OdDgElementIteratorPtr pGraphIt = pModel->createGraphicsElementsIterator();

      bool bHasGraphItems = (!pGraphIt.isNull() && !pGraphIt->done());

      hParentItem = addTreeItem(L"Graphics", bHasGraphItems, hParent );
    }

    OdDgElementIteratorPtr pIt = pDumper->createIterator( pElm );
    for ( ; !pIt->done(); pIt->step() )
    {
      OdDgElementId childId = pIt->item();
      addElement( childId.openObject().get(), hParentItem );
    }

    if ( !pModel.isNull() )
    {
      OdDgElementDumperPEPtr pDumper = OdDgElementDumperPE::cast( pElm );
      OdDgModelDumperPE* pModelDumper = static_cast<OdDgModelDumperPE*>(pDumper.get());
      OdDgElementIteratorPtr pIt = pModelDumper->createControlElementsIterator( pElm );
 
      bool bHasCtrlItems = (!pIt.isNull() && !pIt->done());

      hParentItem = addTreeItem(L"Control", bHasCtrlItems, hParent );

      for ( ; !pIt->done(); pIt->step() )
      {
        OdDgElementId childId = pIt->item();
        addElement( childId.openObject().get(), hParentItem );
      }
    }
  }
}

void DgnTreeFiller::explandItem(HTREEITEM hItem)
{
  OdDgElementPtr pElm;
  OdDbStub* pIdStub = (OdDbStub*)m_pObjTree->GetItemData(hItem);
  if ( pIdStub == 0 )
  {
    // Look at map of tables
    TableMap::iterator it = m_nonDBROItems.find(hItem);
    if ( it != m_nonDBROItems.end() )
    {
      pElm = OdDgElement::cast( it->second );
    }
  }
  else
  {
    pElm = OdDgElementId(pIdStub).openObject();
  }

  if ( !pElm.isNull() )
  {
    addSubElements(pElm.get(), hItem);
  }
}

OdRxObjectPtr DgnTreeFiller::getObject(TVITEM tvitem)
{
  OdDgElementId elmId = reinterpret_cast<OdDbStub*>(tvitem.lParam);
  OdDgElementPtr pElm = OdDgElement::cast( elmId.openObject() );
  if ( pElm.get() )
  {
    return pElm.get();
  }
  else
  {
    TableMap::iterator it = m_nonDBROItems.find(tvitem.hItem);
    if ( it != m_nonDBROItems.end() )
    {
      return it->second;
    }
    else if (tvitem.hItem == m_dbTreeItem)                                        // MKU 23/12/09 - ('warning' issue)
    {
      return database();
    }
  }
  return OdRxObjectPtr();
}

void DgnListFiller::dump(OdRxObjectPtr pObj)
{
  OdDgElementDumperPEPtr pDumper = OdDgElementDumperPE::cast( pObj );
  if ( !pDumper.isNull() )
  {
    pDumper->dump(pObj, this);
  }
}

void DgnListFiller::dumpFieldName( const OdString& fieldName )
{
  m_pObjList->InsertItem(m_pObjList->GetItemCount(), fieldName );
}

void DgnListFiller::dumpFieldValue( const OdString& fieldValue )
{
  m_pObjList->SetItemText(m_pObjList->GetItemCount() - 1, 1, fieldValue );
}
