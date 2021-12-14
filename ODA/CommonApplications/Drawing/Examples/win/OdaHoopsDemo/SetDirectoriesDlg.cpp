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

// SetDirectoriesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HoopsViewer.h"
#include "SetDirectoriesDlg.h"
#include <ShlObj.h>

// CSetDirectoriesDlg dialog

IMPLEMENT_DYNAMIC(CSetDirectoriesDlg, CDialog)

CSetDirectoriesDlg::CSetDirectoriesDlg(OdArray<OdString>* pDirArr,CWnd* pParent /*=NULL*/)
	: CDialog(CSetDirectoriesDlg::IDD, pParent)
{
  m_pDirArr = pDirArr;
}

CSetDirectoriesDlg::~CSetDirectoriesDlg()
{
}

void CSetDirectoriesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetDirectoriesDlg, CDialog)
  ON_BN_CLICKED(IDC_ADD_BUTTON, &CSetDirectoriesDlg::OnBnClickedAddButton)
  ON_BN_CLICKED(IDC_DEL_BUTTON, &CSetDirectoriesDlg::OnBnClickedDelButton)
  ON_BN_CLICKED(IDC_CLOSE_BUTTON, &CSetDirectoriesDlg::OnBnClickedCloseButton)
  ON_NOTIFY(NM_CLICK, IDC_DIRS_LIST, &CSetDirectoriesDlg::OnNMClickDirsList)
END_MESSAGE_MAP()

BOOL CSetDirectoriesDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_DIRS_LIST);

  if( pList )
  {
    pList->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    CRect listRect;
    pList->GetClientRect(&listRect);

    pList->InsertColumn(0,L"Directory",LVCFMT_LEFT,listRect.Width() - GetSystemMetrics(SM_CXVSCROLL));

    for( unsigned int i = 0; i < m_pDirArr->size(); i++ )
      pList->InsertItem(i, (*m_pDirArr)[i].c_str());
  }

  GetDlgItem(IDC_DEL_BUTTON)->EnableWindow(FALSE);

  return TRUE;
}

// CSetDirectoriesDlg message handlers

bool ChooseFolder(HWND hwnd, LPWSTR lpszTitle, LPWSTR lpBuffer) 
{
  BROWSEINFO bi;
  bool res = false;
  LPITEMIDLIST pidlBrowse;
  ZeroMemory(&bi, sizeof(BROWSEINFO));
  bi.hwndOwner = hwnd;
  bi.pszDisplayName = lpBuffer;
  bi.lpszTitle = lpszTitle;
  bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN;
  pidlBrowse = SHBrowseForFolder(&bi); 
  if( pidlBrowse != NULL ) 
  {
    if( SHGetPathFromIDList(pidlBrowse, lpBuffer) )
      res = true;
  }

  return res;
} 


void CSetDirectoriesDlg::OnBnClickedAddButton()
{
  WCHAR strPath[1024];

  CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_DIRS_LIST);

  if( ChooseFolder(m_hWnd, L"", strPath) )
  {
    OdString strDir = strPath;
    if( strDir[strDir.getLength()-1] != '\\' && strDir[strDir.getLength()-1] != '/')
      strDir += "\\";

    m_pDirArr->push_back( strDir );
    pList->InsertItem( pList->GetItemCount(), strDir.c_str() );
  }
}

void CSetDirectoriesDlg::OnBnClickedDelButton()
{
  CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_DIRS_LIST);

  int iItem = pList->GetSelectionMark();

  if( iItem >= 0 && iItem < (int)m_pDirArr->size() )
  {
    CString strMessage;
    strMessage.Format(L"%s\nDelete directory from the list?", (*m_pDirArr)[iItem].c_str());
    if( AfxMessageBox(strMessage, MB_YESNO) == IDYES )
    {
      m_pDirArr->erase( m_pDirArr->begin() + iItem, m_pDirArr->begin() + iItem + 1);
      pList->DeleteItem( iItem );

      iItem = pList->GetSelectionMark();
      GetDlgItem(IDC_DEL_BUTTON)->EnableWindow(FALSE);
    }
  }
}

void CSetDirectoriesDlg::OnBnClickedCloseButton()
{
  OnCancel();
}

void CSetDirectoriesDlg::OnNMClickDirsList(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_DIRS_LIST);

  int iItem = pList->GetSelectionMark();

  if( iItem >= 0 && iItem < (int)m_pDirArr->size() )
    GetDlgItem(IDC_DEL_BUTTON)->EnableWindow(TRUE);
  else
    GetDlgItem(IDC_DEL_BUTTON)->EnableWindow(FALSE);

  *pResult = 0;
}

void CSetDirectoriesDlg::OnOK()
{

}
