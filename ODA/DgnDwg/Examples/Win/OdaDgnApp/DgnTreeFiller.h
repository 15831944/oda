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
#ifndef _ODA_DGN_APP_TREE_FILLER_
#define _ODA_DGN_APP_TREE_FILLER_

#define STL_USING_MAP
#include "OdaSTL.h"

#include "RxObjectImpl.h"
#include "OdaDgnApp.h"
#include "DgDatabase.h"
#include "StaticRxObject.h"
#include "ExDgnElementDumperPE.h"

extern COdaDgnApp theApp;

class DgnTreeFiller
{
public:
  DgnTreeFiller(OdDgDatabase* pDb, CTreeCtrl *pObjTree)
    : m_pObjTree(pObjTree)
    , m_pDb(pDb)
  {
    ODA_ASSERT(pObjTree && pDb);

    // Add root database node
    //CString sName;
    //sName.Format(_T("<%s>"), (LPCTSTR)(pDb->isA()->name()));
    //m_dbTreeItem = m_pObjTree->InsertItem(sName, TVI_ROOT, TVI_LAST);
    //TVITEM item = { 0 };
    //item.mask   = TVIF_HANDLE | TVIF_CHILDREN | TVIF_PARAM;
    //item.hItem  = m_dbTreeItem;
    //item.cChildren  = 1;
    //item.lParam = 0;
    //m_pObjTree->SetItem(&item);
  }
  ~DgnTreeFiller()
  {
  }

  HTREEITEM addElement(OdDgElementPtr pObj, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
  void addSubElements(OdDgElementPtr pObj, HTREEITEM hParent);
  void explandItem(HTREEITEM hItem);

  HTREEITEM addTreeItem( const CString& strName, bool bHasChildren = true, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST );

  HTREEITEM getDbTreeItem() { return m_dbTreeItem; }
  void setDbTreeItem(HTREEITEM item) 
  { 
    m_dbTreeItem = item; 
  }

  OdDgDatabase* database() const { return m_pDb; }

  OdRxObjectPtr getObject(TVITEM hItem);

  CTreeCtrl* getTree()
  {
    return m_pObjTree;
  }

private:
  CTreeCtrl* m_pObjTree;
  OdDgDatabase* m_pDb;

  HTREEITEM m_dbTreeItem;

  typedef std::map<HTREEITEM, OdRxObjectPtr> TableMap;
  TableMap m_nonDBROItems;
};


class DgnListFiller : public OdExDgnDumper
{
public:
  ODRX_HEAP_OPERATORS();

  DgnListFiller(OdDgDatabase* pDb, CListCtrl* pObjList)
    : m_pObjList(pObjList)
    , m_pDb(pDb)
  {
    ODA_ASSERT(pObjList && pDb);
  }
  ~DgnListFiller()
  {
  }

  // OdExDgnDumper overridden
  virtual void dumpFieldName( const OdString& fieldName );
  virtual void dumpFieldValue( const OdString& fieldValue );

  void dump(OdRxObjectPtr pObj);
private:
  CListCtrl*   m_pObjList;
  OdDgDatabase* m_pDb;
};

#endif // _ODA_DGN_APP_TREE_FILLER_
