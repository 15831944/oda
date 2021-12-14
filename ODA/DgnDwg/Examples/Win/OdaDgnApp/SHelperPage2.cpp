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
// SHelperPAge1.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "SHelperPage2.h"

#include "Dg3DObject.h"
#include "DgCellHeader.h"
#include "DgModelerGeometry.h"
#include "CSHelperSheet.h"
#include "DgBRepEntityPE.h"
#include "MemoryStream.h"

// CSHelperPage2 dialog

IMPLEMENT_DYNAMIC(CSHelperPage2, CPropertyPage)

CSHelperPage2::CSHelperPage2(/*CSHelperSheet &pSheet*/)
	: CPropertyPage(CSHelperPage2::IDD)
  //, m_pSheet(pSheet)
{
}

  void CSHelperPage2::setSheet(CSHelperSheet* pSheet)
  {
    m_pSheet = pSheet;
  }

BOOL CSHelperPage2::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  RECT rect;
  GetWindowRect(&rect);
  double w = rect.right - rect.left;

  m_EntitiesList.InsertColumn(0, _T("handle"), 0, int(w*0.1));
  m_EntitiesList.InsertColumn(1, _T("type"), 0, int(w*0.3));
  m_EntitiesList.InsertColumn(2, _T("status"), 0, int(w*0.1));
  m_EntitiesList.InsertColumn(3, _T("stream type"), 0, int(w*0.1));
  m_EntitiesList.InsertColumn(4, _T("stream version"), 0, int(w-w*0.6));

  DWORD  dw = m_EntitiesList.GetExtendedStyle();
  m_EntitiesList.SetExtendedStyle(dw|LVS_EX_FULLROWSELECT);

  return TRUE;
}

CSHelperPage2::~CSHelperPage2()
{
}

void CSHelperPage2::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_ENTITIES_LIST, m_EntitiesList);
}

void CSHelperPage2::addEntityToList(OdDgElementIteratorPtr iterator)
{
  for( ; !iterator->done(); iterator->step())
  {
    OdRxObjectPtr object = iterator->item().openObject();
    OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::cast(object);
    OdDgSolidPtr pSolid = OdDgSolid::cast(object);
    if (!pCell.isNull() || !pSolid.isNull())
    {
      addEntityToList(OdDgElement::cast(object).get());

      if (!pCell.isNull())
        addEntityToList(pCell->createIterator());
      continue;
    }

    OdDgSharedCellReferencePtr pShared = OdDgSharedCellReference::cast( object );
    if (!pShared.isNull())
    {
      OdDgSharedCellDefinitionPtr pSharedDef = pShared->findDefinition();
      if (!pSharedDef.isNull())
      {
        addEntityToList(pSharedDef->createIterator());
      }
      continue;
    }
  }
}

void CSHelperPage2::addEntityToList(const OdDgElement *pE)
{
  LV_ITEM lvi;

  lvi.mask = LVIF_TEXT;
  lvi.iItem = 0;
  lvi.iSubItem = 0;
  lvi.pszText = 0;
  lvi.iImage = 0;
  lvi.stateMask = 0;
  lvi.state = 0;

  m_EntitiesList.InsertItem(&lvi);

  CString h = CString(LPCTSTR(pE->elementId().getHandle().ascii().c_str()));
  CString type = CString(LPCTSTR(pE->isA()->name().c_str()));
  FlagState status = CSHelperPage2::kCrash;
  CString streamType = _T("unk");
  CString streamVer = _T("unk");

  OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::cast(pE);
  if (!pCell.isNull())
  {
    OdDgBRepEntityPEPtr pBrep = OdDgBRepEntityPEPtr(pCell);

    OdStreamBufPtr binData = OdMemoryStream::createNew(); 
    OdUInt32 version = pBrep->brepVersion(pCell);
    if (pBrep->brepOutNative(pCell, *binData) == eOk && version != 0)
    {
      status = CSHelperPage2::kOk;
      if (version&OdDgModelerGeometry::kParasolidMask)
      {
        streamType = _T("PS");
        streamVer = _T("12");
      }
      else
      {
        streamType = _T("ACIS");
        switch(version&OdDgModelerGeometry::kVersionMask)
        {
          case OdDgModelerGeometry::kAS_1_5:
            streamVer = _T("1.5"); break;
          case OdDgModelerGeometry::kAS_1_6:
            streamVer = _T("1.6"); break;
          case OdDgModelerGeometry::kAS_1_7:
            streamVer = _T("1.7"); break;
          case OdDgModelerGeometry::kAS_2_1:
            streamVer = _T("2.1"); break;
          case OdDgModelerGeometry::kAS_3_0:
            streamVer = _T("3.0"); break;
          case OdDgModelerGeometry::kAS_4_0:
            streamVer = _T("4.0"); break;
          case OdDgModelerGeometry::kAS_5_0:
            streamVer = _T("5.0"); break;
          case OdDgModelerGeometry::kAS_6_0:
            streamVer = _T("6.0"); break;
          case OdDgModelerGeometry::kAS_7_0:
            streamVer = _T("7.0"); break;
          case OdDgModelerGeometry::kAS_8_0:
            streamVer = _T("8.0"); break;
        };
      }
    }
    else
    {
      streamType = _T("X");
      streamVer = _T("X");
    }
  }

  m_EntitiesList.SetItemText(0, 0, h);
  m_EntitiesList.SetItemText(0, 1, type);
  m_EntitiesList.SetItemText(0, 2, status == kCrash ? _T("X") : (status == kOk ? _T("+") : _T("-")));
  m_EntitiesList.SetItemText(0, 3, streamType);
  m_EntitiesList.SetItemText(0, 4, streamVer);

  /*if (pE->isKindOf(OdDgCellHeader3d::desc()))
  {

  }
  else if (pE->isKindOf(OdDgSolid::desc()))
  {

  }*/
}

void CSHelperPage2::addFileToList(CString fullPath)
{
  try
  {
    OdDgDatabasePtr pDb = theApp.readFile(OdString(fullPath));

    if (!pDb.isNull())
    {

      OdDgModelTablePtr pModelTable = pDb->getModelTable();
      OdDgElementIteratorPtr pIter = pModelTable->createIterator();
      for ( ; !pIter->done(); pIter->step() )
      {
        OdDgModelPtr pModel = OdDgModel::cast( pIter->item().openObject() );
        if ( pModel.get() )
        {
          int i = 0;
          while(i < 2)
          {
            OdDgElementIteratorPtr iterator = i++==0 ? pModel->createGraphicsElementsIterator()
              : pModel->createControlElementsIterator();
            addEntityToList(iterator);
          }
        }
      }
    }
  }
  catch (...)
  {
  }
}

void CSHelperPage2::fill (CString fileName)
{
  m_EntitiesList.DeleteAllItems();
  addFileToList(fileName);
  m_Path = fileName;
}

BEGIN_MESSAGE_MAP(CSHelperPage2, CPropertyPage)
  ON_NOTIFY(NM_DBLCLK, IDC_ENTITIES_LIST, OnNMDblclkEntitiesList)
  ON_BN_CLICKED(IDC_BUTTON_LOADFILE, CSHelperPage2::OnBnClickedButtonLoadfile)
END_MESSAGE_MAP()


// CSHelperPage2 message handlers

void CSHelperPage2::OnNMDblclkEntitiesList(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO: Add your control notification handler code here
  *pResult = 0;

  LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;

  UINT pFlags = 0;
  int selectedItem = m_EntitiesList.HitTest(lpnmitem->ptAction, &pFlags);
  if (selectedItem != -1)
  {
    CString s = m_EntitiesList.GetItemText(selectedItem,0);
    OdDgDatabasePtr pDb = theApp.readFile(OdString(m_Path));
    if (!pDb.isNull())
    {
      OdDgElementId id = pDb->getElementId(OdDbHandle(OdString(s)));
      OdDgCellHeader3dPtr pCell = OdDgCellHeader3d::cast(id.safeOpenObject());
      if (!pCell.isNull())
      {
        OdStreamBufPtr binData = OdMemoryStream::createNew(); 
        if (OdDgBRepEntityPEPtr(pCell)->brepOutNative(pCell, *binData) == eOk)
        {
          binData->rewind();
          m_pSheet->fillAndSelect3Page(*binData);
        }
      }
    }
  }
}

void CSHelperPage2::OnBnClickedButtonLoadfile()
{
  CFileDialog loadFile(true, _T("dgn"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Dgn files (*.dgn)|*.dgn||"), this);
  if (loadFile.DoModal() == IDOK)
  {
    fill(loadFile.GetPathName());
  }
}
