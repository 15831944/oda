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

// SetActiveLayoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "RxRasterServices.h"
#include "SnapshotDlg.h"
#include "SetActiveLayoutDlg.h"
#include "OdArray.h"
#include "DbDictionary.h"
#include "DbLayout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetActiveLayoutDlg dialog


CSetActiveLayoutDlg::CSetActiveLayoutDlg(OdDbDatabase* pDb, CWnd* pParent /*=NULL*/)
	: CDialog(CSetActiveLayoutDlg::IDD, pParent)
  , m_pDb(pDb)
{
	//{{AFX_DATA_INIT(CSetActiveLayoutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetActiveLayoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetActiveLayoutDlg)
  DDX_Control(pDX, IDC_LAYOUTPREVIEW, m_Preview);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetActiveLayoutDlg, CDialog)
	//{{AFX_MSG_MAP(CSetActiveLayoutDlg)
  ON_COMMAND(IDC_ALAYOUT_CLOSE , OnLayoutDlgClose)
	ON_LBN_SELCHANGE(IDC_LAYOUTLIST, OnSelchangeLayoutlist)
	ON_LBN_DBLCLK(IDC_LAYOUTLIST, OnDblclkLayoutlist)
	ON_BN_CLICKED(IDC_RENAME, OnRename)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_COPY,   OnCopy)
	ON_BN_CLICKED(IDC_NEW,    OnNew)
	ON_BN_CLICKED(IDC_FROM_TEMPLATE, OnFromTemplate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetActiveLayoutDlg message handlers

BOOL CSetActiveLayoutDlg::OnInitDialog() 
{
  if (!m_sCaption.IsEmpty())
  {
    SetWindowText(m_sCaption);
    CWnd *pButton = GetDlgItem( IDC_RENAME );
    if ( pButton )
        pButton->EnableWindow( FALSE );
    pButton = GetDlgItem( IDC_DELETE );
    if ( pButton )
        pButton->EnableWindow( FALSE );
    pButton = GetDlgItem( IDC_COPY );
    if ( pButton )
        pButton->EnableWindow( FALSE );
    pButton = GetDlgItem( IDC_NEW );
    if ( pButton )
        pButton->EnableWindow( FALSE );
    pButton = GetDlgItem( IDC_FROM_TEMPLATE );
    if ( pButton )
        pButton->EnableWindow( FALSE );
  }
  CDialog::OnInitDialog();
  FillListBox();  
  return TRUE;
}



void CSetActiveLayoutDlg::FillListBox() 
{
  try
  {
    UINT                      n;
    OdArray<OdString>         items;
    OdDbDatabase*             pDb = m_pDb;
    OdDbDictionaryPtr         pLayoutDict = pDb->getLayoutDictionaryId().safeOpenObject();
    OdDbDictionaryIteratorPtr pIter = pLayoutDict->newIterator();
    OdDbObjectId              activeLayoutBTRId = pDb->getActiveLayoutBTRId();
    
    m_nOldActiveLayout = -1;
    while (!pIter->done())
    {
      OdDbLayoutPtr pLayout = pIter->objectId().safeOpenObject();
      n = pLayout->getTabOrder();
      if (n >= items.size())
        items.resize(n+1);
      items[n] = pIter->name();
      if (pLayout->getBlockTableRecordId() == activeLayoutBTRId)
      {
        m_nOldActiveLayout = n;
      }
      pIter->next();
    }

    CListBox* pLayouts = (CListBox*)GetDlgItem(IDC_LAYOUTLIST);
    pLayouts->ResetContent();
    for (n = 0; n < items.size(); ++n)
    {
      pLayouts->InsertString(n, items[n]);
    }
    pLayouts->SetSel(m_nOldActiveLayout);
    m_nNewActiveLayout = m_nOldActiveLayout;
    GetDlgItem(IDC_NEWNAME)->SetWindowText(items[m_nOldActiveLayout]);
    OnSelchangeLayoutlist();
  }
  catch(OdError& e)
  {
    theApp.reportError(_T("Error Selecting Layout"), e);
  }
}



void CSetActiveLayoutDlg::OnLayoutDlgClose() 
{
  EndDialog(m_nOldActiveLayout!=m_nNewActiveLayout ? IDOK : IDCANCEL);
}

void CSetActiveLayoutDlg::OnSelchangeLayoutlist() 
{
  CListBox* pLayouts = (CListBox*)GetDlgItem(IDC_LAYOUTLIST);
  m_nNewActiveLayout = pLayouts->GetCurSel();
  pLayouts->GetText(m_nNewActiveLayout, m_sNewLayoutName);

  OdGiRasterImagePtr pImg;
  OdDbObjectId id = m_pDb->findLayoutNamed(OdString(m_sNewLayoutName));
  OdDbLayoutPtr layout = id.safeOpenObject(OdDb::kForWrite);
  if ( !layout.isNull() )
  {
    pImg = layout->getPreviewImage();
  }
  m_Preview.SetImage(pImg);
  /*
  CWnd* pNewName = GetDlgItem(IDC_NEWNAME);
  if(!m_bCreateNewLayout)
  {
    GetDlgItem(IDC_RENAME)->EnableWindow(TRUE);
    GetDlgItem(IDC_DELETE)->EnableWindow(TRUE);
    pNewName->EnableWindow(TRUE);
    pNewName->SetWindowText(m_sNewLayoutName);
  }
  else
  {
    GetDlgItem(IDC_RENAME)->EnableWindow(FALSE);
    GetDlgItem(IDC_DELETE)->EnableWindow(FALSE);
    pNewName->EnableWindow(FALSE);
    pNewName->SetWindowText(_T(""));
  }
*/
}

void CSetActiveLayoutDlg::OnDblclkLayoutlist() 
{
  EndDialog(m_nOldActiveLayout!=m_nNewActiveLayout ? IDOK : IDCANCEL);
}

void CSetActiveLayoutDlg::OnRename() 
{
  CListBox* pLayouts = (CListBox*)GetDlgItem(IDC_LAYOUTLIST);
  CString oldName, newName;
  pLayouts->GetText(m_nNewActiveLayout, oldName);
  GetDlgItem(IDC_NEWNAME)->GetWindowText(newName);
  if(newName!=oldName)
  {
    try
    {
      m_pDb->renameLayout(OdString(oldName), OdString(newName));
    }
    catch(OdError& e)
    {
      theApp.reportError(_T("Error Renaming Layout"), e);
      return;
    }
    pLayouts->DeleteString(m_nNewActiveLayout);
    pLayouts->InsertString(m_nNewActiveLayout, newName);
    pLayouts->SetSel(m_nNewActiveLayout);
  }
}


void CSetActiveLayoutDlg::OnDelete() 
{
  CListBox* pLayouts = (CListBox*)GetDlgItem(IDC_LAYOUTLIST);
  CString currName;
  pLayouts->GetText(m_nNewActiveLayout, currName);
  try
  {
    m_pDb->startUndoRecord();
    m_pDb->deleteLayout(OdString(currName));
  }
  catch(OdError& e)
  {
    theApp.reportError(_T("Error Deleting Layout"), e);
    m_pDb->disableUndoRecording(true);
    m_pDb->undo();
    m_pDb->disableUndoRecording(false);
    return;
  }
  FillListBox();
}

void CSetActiveLayoutDlg::OnCopy() 
{
  CListBox* pLayouts = (CListBox*)GetDlgItem(IDC_LAYOUTLIST);
  CString strSourceName, strNewName;
  pLayouts->GetText(m_nNewActiveLayout, strSourceName);
  GetDlgItem(IDC_NEWNAME)->GetWindowText(strNewName);
  OdString strName(strSourceName);
  OdDbLayoutManagerPtr pLManager = m_pDb->appServices()->layoutManager();
  try
  {
    OdDbLayoutPtr pLayout = pLManager->findLayoutNamed(m_pDb, strName).safeOpenObject();
    pLManager->cloneLayout(m_pDb, pLayout, OdString(strNewName));
  }
  catch(OdError& e)
  {
    theApp.reportError(_T("Error Cloning Layout"), e);
    return;
  }
  FillListBox();
}


void CSetActiveLayoutDlg::OnNew() 
{
  CString strNewName;
  GetDlgItem(IDC_NEWNAME)->GetWindowText(strNewName);

  try
  {
    // Create new layout
    m_pDb->createLayout(OdString(strNewName));
  }
  catch(OdError& e)
  {
    theApp.reportError(_T("Error Creating Layout"), e);
    return;
  }
  FillListBox();
}


void CSetActiveLayoutDlg::OnFromTemplate() 
{
  CString sCaption;
  GetWindowText(sCaption);
  SetWindowText(L"Wait...");
  CString sPath = theApp.BrowseWithPreview(/*GetMainWnd()->*/GetSafeHwnd(),
    _T("DWG files (*.dwg)|*.dwg|DXF files (*.dxf)|*.dxf|Template files (*.dwt)|*.dwt|All Files (*.*)|*.*||"));
  if(sPath.GetLength() == 0)
    return;

  OdDbDatabasePtr pDb = theApp.readFile(OdString(sPath));
  if (pDb.isNull())
    return;

  OdString strToClone;
  CString strNewName;
  {
    CSetActiveLayoutDlg dlg(pDb, theApp.GetMainWnd());
    dlg.m_sCaption = L"Select Layout to Clone";
//    if(dlg.DoModal()==IDOK)
    dlg.DoModal();
    {
      strToClone = dlg.m_sNewLayoutName;
    }
  }
  OdDbLayoutManagerPtr pLManager = m_pDb->appServices()->layoutManager();
  OdDbLayoutPtr pLayout = OdDbLayout::cast(pLManager->findLayoutNamed(pDb, strToClone).openObject());
  if (pLayout.isNull())
    return;

  OdDbObjectId idClone = pLManager->cloneLayout(m_pDb, pLayout, strToClone);
  SetWindowText(sCaption);
  if (idClone.isNull())
  {
    theApp.reportError(L"Error Cloning Layout", eInvalidInput);
    return;
  }
  FillListBox();
}
