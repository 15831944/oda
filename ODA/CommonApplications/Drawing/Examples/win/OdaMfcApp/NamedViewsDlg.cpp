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

// NamedViewsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "OdaMfcAppDoc.h"
#include "NamedViewsDlg.h"
#include "DbLayout.h"
#include "DbViewport.h"

#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbBlockTableRecord.h"
#include "NewViewDlg.h"
#include "DbLayerState.h"
#include "DbUCSTableRecord.h"
#include "DbSymUtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

OdDbObjectId CNamedViewListCtrl::viewId(int nItem) const
{
  return OdDbObjectId(reinterpret_cast<OdDbStub*>(GetItemData(nItem)));
}

void CNamedViewListCtrl::setViewId(int nItem, const OdDbObjectId& id)
{
  SetItemData(nItem, reinterpret_cast<DWORD_PTR>(static_cast<OdDbStub*>(id)));
}

OdDbViewTableRecordPtr CNamedViewListCtrl::view(int nItem)
{
  return viewId(nItem).safeOpenObject(OdDb::kForWrite);
}

void CNamedViewListCtrl::setView(int nItem, const OdDbViewTableRecord* pView)
{
  setViewId(nItem, pView->objectId());
}

OdString ucsString(const OdDbObject* pViewObj)
{
  OdString res;
  OdDbAbstractViewportDataPtr pViewPE(pViewObj);
  switch (pViewPE->orthoUcs(pViewObj))
  {
  case OdDb::kTopView:
    res = _T("Top");
    break;
  case OdDb::kBottomView:
    res = _T("Bottom");
    break;
  case OdDb::kFrontView:
    res = _T("Front");
    break;
  case OdDb::kBackView:
    res = _T("Back");
    break;
  case OdDb::kLeftView:
    res = _T("Left");
    break;
  case OdDb::kRightView:
    res = _T("Right");
    break;
  default:
    {
      OdDbUCSTableRecordPtr pUCS = OdDbObjectId(pViewPE->ucsName(pViewObj)).openObject();
      if(pUCS.get())
      {
        res = pUCS->getName();
      }
      else
      {
        OdGePoint3d origin;
        OdGeVector3d xAxis, yAxis;
        pViewPE->getUcs(pViewObj, origin, xAxis, yAxis);
        if(origin==OdGePoint3d::kOrigin && xAxis==OdGeVector3d::kXAxis && yAxis==OdGeVector3d::kYAxis)
          res = _T("World");
        else
          res = _T("Unnamed");
      }
    }
    break;
  }
  return res;
}

void CNamedViewListCtrl::InsertItem(int i, const OdDbViewTableRecord* pView)
{
  CListCtrl::InsertItem(i, pView->getName());
  setView(i, pView);
  SetItemText(i, 1, pView->getCategoryName());
  SetItemText(i, 2, pView->isPaperspaceView() ? _T("Layout") : _T("Model"));
  SetItemText(i, 3, pView->isViewAssociatedToViewport() ? _T("True") : _T(""));
  SetItemText(i, 4, pView->getLayerState().isEmpty() ? _T("") : _T("Saved") );
  if (  pView->isUcsAssociatedToView()  )
  {
    SetItemText(i, 5, ucsString(pView));
  }
  SetItemText(i, 6, pView->perspectiveEnabled() ? _T("On") : _T("Off"));
}

OdDbViewTableRecordPtr CNamedViewListCtrl::selectedView()
{
  int nSelectionMark = GetSelectionMark();
  if (nSelectionMark > -1)
    return view(nSelectionMark);
  return OdDbViewTableRecordPtr();
}

/////////////////////////////////////////////////////////////////////////////
// CNamedViewsDlg dialog


CNamedViewsDlg::CNamedViewsDlg(COdaMfcAppDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CNamedViewsDlg::IDD, pParent)
{
  m_pDoc = pDoc;
	//{{AFX_DATA_INIT(CNamedViewsDlg)
	//}}AFX_DATA_INIT
}

OdDbDatabase* CNamedViewsDlg::database() { return document()->m_pDb; }

void CNamedViewsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNamedViewsDlg)
	DDX_Control(pDX, IDC_NAMEDVIEWS, m_views);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNamedViewsDlg, CDialog)
	//{{AFX_MSG_MAP(CNamedViewsDlg)
	ON_BN_CLICKED(IDC_SETCURRENT_BUTTON, OnSetcurrentButton)
	ON_NOTIFY(NM_DBLCLK, IDC_NAMEDVIEWS, OnDblclkNamedviews)
	ON_BN_CLICKED(IDC_NEW_BUTTON, OnNewButton)
	ON_BN_CLICKED(IDC_UPDATE_LAYERS_BUTTON, OnUpdateLayersButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamedViewsDlg message handlers

BOOL CNamedViewsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  OdDbDatabase  *pDb = m_pDoc->m_pDb;

  m_views.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 100);
  m_views.InsertColumn(1, _T("Category"), LVCFMT_LEFT, 60);
  m_views.InsertColumn(2, _T("Location"), LVCFMT_LEFT, 50);
  m_views.InsertColumn(3, _T("VP"), LVCFMT_LEFT, 40);
  m_views.InsertColumn(4, _T("Layers"), LVCFMT_LEFT, 50);
  m_views.InsertColumn(5, _T("UCS"), LVCFMT_LEFT, 60);
  m_views.InsertColumn(6, _T("Perspective"), LVCFMT_LEFT, 30);

  try
  {
    OdDbViewTablePtr pViewTable = pDb->getViewTableId().safeOpenObject();
    int i = 0;
    OdDbSymbolTableIteratorPtr pIter;
    for (pIter = pViewTable->newIterator(); !pIter->done(); pIter->step() )
    {
      OdDbViewTableRecordPtr pView = pIter->getRecordId().openObject();
      m_views.InsertItem(i++, pView);
    }
  }
  catch(const OdError& e)
  {
    theApp.reportError(_T("Error creating view dialog"), e);
    EndDialog(IDCANCEL);
    return FALSE;
  }
  return TRUE;
}

void CNamedViewsDlg::OnSetcurrentButton() 
{
  OdDbViewTableRecordPtr pNamedView = m_views.selectedView();
  if(pNamedView.get())
  {
    OdDbDatabase* pDb = m_pDoc->m_pDb;
    OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
    OdDbAbstractViewportDataPtr pVpPE(pVpObj);
    pVpPE->setView(pVpObj, pNamedView);
    pVpPE->setUcs(pVpObj, pNamedView);
    // pVpPE->setProps(pVpObj, pNamedView); // #CORE-12249 : don't copy additional properties because DbViewTableRecord haven't them.
    OdString sLSName = pNamedView->getLayerState();
    if(!sLSName.isEmpty())
    {
      OdDbLayerState::restore(pDb, sLSName,
        OdDbLayerStateManager::kUndefDoNothing, OdDbLayerStateManager::kOn| OdDbLayerStateManager::kFrozen);
    }
  }
}

void CNamedViewsDlg::OnDblclkNamedviews(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/) 
{
  OnSetcurrentButton();
}

void deleteLayerState(OdDbViewTableRecord* pNamedView) 
{
  OdString sLSName = pNamedView->getLayerState();
  if(!sLSName.isEmpty())
  {
    OdDbLayerState::remove(pNamedView->database(), sLSName);
    pNamedView->setLayerState(_T(""));
  }
}

void updateLayerState(OdDbViewTableRecord* pNamedView) 
{
  OdString sLSName = pNamedView->getLayerState();
  OdDbDatabase* pDb = pNamedView->database();
  if(sLSName.isEmpty())
  {
    OdString name;
    name.format(OD_T("ACAD_VIEWS_%s"), pNamedView->getName().c_str());
    sLSName = name;
    int i=1;
    while(OdDbLayerState::has(pDb, sLSName))
    {
      sLSName.format(OD_T("%s(%d)"), name.c_str(), ++i);
    }
    pNamedView->setLayerState(sLSName);
  }
  OdDbLayerState::save(pDb, sLSName, OdDbLayerStateManager::kStateIsHidden|OdDbLayerStateManager::kCurrentViewport);
}

void CNamedViewsDlg::OnNewButton() 
{
  CNewViewDlg newDlg(this);
  OdDbViewTableRecordPtr pNamedView;
  OdDbDatabase* pDb = m_pDoc->m_pDb;
  while(newDlg.DoModal()==IDOK)
  {
    LVFINDINFO lvfi = {LVFI_STRING, newDlg.m_sViewName, 0, {0,0}, 0};
    int i = m_views.FindItem(&lvfi);
    if(i >= 0)
    {
      if(AfxMessageBox(newDlg.m_sViewName + _T(" already exists.\nDo you want to replace it?"), MB_YESNOCANCEL)!=IDYES)
        continue;
      pNamedView = m_views.view(i);
      m_views.DeleteItem(i);
    }
    else
    {
      OdDbViewTablePtr pViewTable = pDb->getViewTableId().safeOpenObject(OdDb::kForWrite);
      pNamedView = OdDbViewTableRecord::createObject();
      pNamedView->setName(OdString(newDlg.m_sViewName));
      pViewTable->add(pNamedView);

      i = m_views.GetItemCount();
    }

    OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject();
    OdDbAbstractViewportDataPtr pViewPE(pNamedView);
    pViewPE->setView(pNamedView, pVpObj);

    if(newDlg.m_bSaveUCS)
    {
      if(newDlg.m_sUcsName==_T("Unnamed"))
        pViewPE->setUcs(pNamedView, pVpObj);
      else if(newDlg.m_sUcsName==_T("World"))
        pNamedView->setUcsToWorld();
      else
        pNamedView->setUcs(OdDbSymUtil::getUCSId(OdString(newDlg.m_sUcsName), pDb));
    }
    else
      pNamedView->disassociateUcsFromView();

    pViewPE->setProps(pNamedView, pVpObj);

    pNamedView->setCategoryName(OdString(newDlg.m_sViewCategory));

    if(newDlg.m_bStoreLS)
      updateLayerState(pNamedView);
    else
      deleteLayerState(pNamedView);

    m_views.InsertItem(i, pNamedView);
    break;
  }
}

void CNamedViewsDlg::OnUpdateLayersButton() 
{
  int nSelectionMark = m_views.GetSelectionMark();
  if (nSelectionMark > -1)
  {
    updateLayerState(m_views.selectedView());
    m_views.SetItemText(nSelectionMark, 4, _T("Saved"));
  }
}

void CNamedViewsDlg::OnDeleteButton() 
{
  int nSelectionMark = m_views.GetSelectionMark();
  if (nSelectionMark > -1)
  {
    m_views.view(nSelectionMark)->erase();
    m_views.DeleteItem(nSelectionMark);
  }
}
