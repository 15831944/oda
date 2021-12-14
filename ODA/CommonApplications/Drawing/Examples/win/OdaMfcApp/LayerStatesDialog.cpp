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

#include "StdAfx.h"
#include "OdaMfcApp.h"
#include "DbDatabase.h"
#include "LayerStatesDialog.h"
#include "DbLayerState.h"
#include "NewLayerStateDialog.h"

IMPLEMENT_DYNAMIC(LayerStatesDialog, CDialog)

LayerStatesDialog::LayerStatesDialog(OdDbDatabase* pDb, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_LAYER_STATES, pParent), m_pDb(pDb)
{
  if (!m_pDb)
    throw OdError(L"Database cannot be empty");
}

BOOL LayerStatesDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  CRect rect;
  m_List.GetClientRect(&rect);
  m_List.InsertColumn(0, L"Name", LVCFMT_LEFT, rect.Width() / 4);
  m_List.InsertColumn(1, L"Space", LVCFMT_LEFT, rect.Width() / 4, 1);
  m_List.InsertColumn(2, L"Description", LVCFMT_LEFT, rect.Width() / 2, 2);
  OdStringArray sa;
  m_pDb->getLayerStateManager()->getLayerStateNames(sa);
  for (unsigned int i = 0; i < sa.size(); ++i)
  {
    m_List.InsertItem(i, (const TCHAR*)sa[i]);
    OdString desc;
    if (eOk == m_pDb->getLayerStateManager()->getLayerStateDescription(sa[i], desc))
      m_List.SetItemText(i, 2, (const TCHAR*)desc);
    m_List.SetItemText(i, 1, m_pDb->getLayerStateManager()->layerStateHasViewportData(sa[i]) ? L"Layout" : L"Model");
  }
  return TRUE;
}

LayerStatesDialog::~LayerStatesDialog()
{
}

void LayerStatesDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(LayerStatesDialog, CDialog)
  ON_BN_CLICKED(IDC_BUTTON1, &LayerStatesDialog::OnBnClickedNew)
  ON_BN_CLICKED(IDC_BUTTON3, &LayerStatesDialog::OnBnClickedDelete)
  ON_BN_CLICKED(IDC_BUTTON4, &LayerStatesDialog::OnBnClickedUpdate)
  ON_BN_CLICKED(IDC_BUTTON6, &LayerStatesDialog::OnBnClickedExport)
  ON_BN_CLICKED(IDC_BUTTON7, &LayerStatesDialog::OnBnClickedImport)
  ON_BN_CLICKED(IDOK, &LayerStatesDialog::OnBnClickedRestore)
  ON_BN_CLICKED(IDCANCEL, &LayerStatesDialog::OnBnClickedClose)
END_MESSAGE_MAP()


void LayerStatesDialog::OnBnClickedNew()
{
  NewLayerStateDialog nlsd;
  if (nlsd.DoModal() == IDOK && !nlsd.m_sName.IsEmpty() && !m_pDb->getLayerStateManager()->hasLayerState((const TCHAR*)nlsd.m_sName))
  {
    m_pDb->getLayerStateManager()->saveLayerState((const TCHAR*)nlsd.m_sName, OdDbLayerStateManager::kAll, m_pDb->getTILEMODE() == 0 ? m_pDb->activeViewportId() : OdDbObjectId::kNull);
    m_List.InsertItem(0, nlsd.m_sName);
    m_List.SetItemText(0, 2, nlsd.m_sDescription);
    m_List.SetItemText(0, 1, m_pDb->getTILEMODE() == 0 ? L"Layout" : L"Model");
  }
}


void LayerStatesDialog::OnBnClickedDelete()
{
  int sel = m_List.GetSelectionMark();
  if (sel != -1)
  {
    m_pDb->getLayerStateManager()->deleteLayerState((const TCHAR*)m_List.GetItemText(sel, 0));
    m_List.DeleteItem(sel);
  }
}


void LayerStatesDialog::OnBnClickedUpdate()
{
  int sel = m_List.GetSelectionMark();
  if (sel != -1)
  {
    m_pDb->getLayerStateManager()->saveLayerState((const TCHAR*)m_List.GetItemText(sel, 0),
      OdDbLayerStateManager::kAll, m_pDb->getTILEMODE() == 0 ? m_pDb->activeViewportId() : OdDbObjectId::kNull);
  }
}


void LayerStatesDialog::OnBnClickedExport()
{
  int sel = m_List.GetSelectionMark();
  if (sel != -1)
  {
    CFileDialog fd(false, L"las", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"*.las|*.las|");
    if (fd.DoModal() == IDOK)
    {
      OdStreamBufPtr s = odrxSystemServices()->createFile((const TCHAR*)fd.GetPathName(), Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
      m_pDb->getLayerStateManager()->exportLayerState((const TCHAR*)m_List.GetItemText(sel, 0), s);
    }
  }
}


void LayerStatesDialog::OnBnClickedImport()
{
  CFileDialog fd(true, L"las", 0, 0, L"*.las|*.las|");
  if (fd.DoModal() == IDOK)
  {
    OdStreamBufPtr s = odrxSystemServices()->createFile((const TCHAR*)fd.GetPathName(), Oda::kFileRead, Oda::kShareDenyNo, Oda::kOpenExisting);
    OdString name;
    if (eOk == m_pDb->getLayerStateManager()->importLayerState(s, name))
    {
      m_List.InsertItem(0, (const TCHAR*)name);
      OdString desc;
      if (eOk == m_pDb->getLayerStateManager()->getLayerStateDescription(name, desc))
        m_List.SetItemText(0, 2, (const TCHAR*)desc);
      m_List.SetItemText(0, 1, m_pDb->getLayerStateManager()->layerStateHasViewportData(name) ? L"Layout" : L"Model");
    }
  }
}


void LayerStatesDialog::OnBnClickedRestore()
{
  int sel = m_List.GetSelectionMark();
  if (sel != -1)
  {
    m_pDb->getLayerStateManager()->restoreLayerState((const TCHAR*)m_List.GetItemText(sel, 0), 
      m_pDb->getTILEMODE() == 0 ? m_pDb->activeViewportId() : OdDbObjectId::kNull, OdDbLayerStateManager::kAll);
  }
  CDialog::OnOK();
}


void LayerStatesDialog::OnBnClickedClose()
{
  CDialog::OnCancel();
}
