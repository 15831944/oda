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

// BrowseFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HoopsViewer.h"
#include "BrowseFileDlg.h"


// CBrowseFileDlg dialog

IMPLEMENT_DYNAMIC(CBrowseFileDlg, CDialog)

CBrowseFileDlg::CBrowseFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrowseFileDlg::IDD, pParent)
{
  m_bSkip     = false;
  m_bSkipAll  = false;
  m_strFileName.empty();
  m_strPathName.empty();
}

CBrowseFileDlg::~CBrowseFileDlg()
{
}

void CBrowseFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBrowseFileDlg, CDialog)
  ON_BN_CLICKED(IDC_BROWSE_BTN, &CBrowseFileDlg::OnBnClickedBrowseBtn)
  ON_BN_CLICKED(IDC_SKIP_BTN, &CBrowseFileDlg::OnBnClickedSkipBtn)
  ON_BN_CLICKED(IDC_SKIPALL_BTN, &CBrowseFileDlg::OnBnClickedSkipallBtn)
END_MESSAGE_MAP()

BOOL CBrowseFileDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CStatic* pStatic = (CStatic*)GetDlgItem( IDC_FNAME_STATIC);

  CString  strCaption;
  strCaption.Format(L"Can't find file \"%s\"\nPlease, browse it's location.",m_strFileName.getBuffer(m_strFileName.getLength()));
  m_strFileName.releaseBuffer();
  pStatic->SetWindowText(strCaption);

  return TRUE;
}


// CBrowseFileDlg message handlers

void CBrowseFileDlg::OnBnClickedBrowseBtn()
{
  CFileDialog dlgBrowse(true, NULL, m_strFileName,4|2,L"All files|*.*");

  if( dlgBrowse.DoModal() == IDOK )
  {
    m_strPathName = dlgBrowse.GetPathName();
    OnOK();
  }
}

void CBrowseFileDlg::OnBnClickedSkipBtn()
{
  m_bSkip = true;
  OnCancel();
}

void CBrowseFileDlg::OnBnClickedSkipallBtn()
{
  m_bSkipAll = true;
  OnCancel();
}
