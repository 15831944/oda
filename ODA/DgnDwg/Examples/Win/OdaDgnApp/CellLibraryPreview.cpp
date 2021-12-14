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
// CameraView.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "CellLibraryPreview.h"

// CCellLibraryPreviewDlg dialog

IMPLEMENT_DYNAMIC(CCellLibraryPreviewDlg, CDialog)

CCellLibraryPreviewDlg::CCellLibraryPreviewDlg(CWnd* pParent /*=NULL*/ )
	: CDialog(CCellLibraryPreviewDlg::IDD, pParent)
{
  m_iModelType  = 0;
  m_iCellType   = 0;
  m_strFileName = L"";
}

CCellLibraryPreviewDlg::~CCellLibraryPreviewDlg()
{
}

void CCellLibraryPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Text(pDX, IDC_FILENAME_EDIT, m_strFileName);
  DDX_Radio(pDX, IDC_MODEL2D_RADIO, m_iModelType );
  DDX_Radio(pDX, IDC_CELLHEADER_RADIO, m_iCellType );
}


BEGIN_MESSAGE_MAP(CCellLibraryPreviewDlg, CDialog)
  ON_BN_CLICKED(IDC_FILENAME_BUTTON, CCellLibraryPreviewDlg::OnBnFileNameClick)
  ON_EN_KILLFOCUS(IDC_FILENAME_EDIT, CCellLibraryPreviewDlg::OnEnKillfocusFileNameEdit)
  ON_EN_SETFOCUS(IDC_FILENAME_EDIT, CCellLibraryPreviewDlg::OnEnSetfocusFileNameEdit)
END_MESSAGE_MAP()

BOOL CCellLibraryPreviewDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  GetDlgItem( IDOK )->EnableWindow(FALSE);

  return TRUE;
}

void CCellLibraryPreviewDlg::OnBnFileNameClick()
{
  UpdateData(TRUE);

  CFileDialog fileDlg( TRUE, _T("cel"), _T(""),OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
                       _T("dgn files (*.dgn)|*.dgn|dgnlib files (*.dgnlib)|*.dgnlib|cell library files (*.cel)|*.cel|Redline files (*.rdl)|*.rdl|"),this
                     );

  if( fileDlg.DoModal() == IDOK )
  {
    m_strFileName = fileDlg.GetPathName();
    UpdateData(FALSE);

    GetDlgItem( IDOK )->EnableWindow( !m_strFileName.IsEmpty() );
  }
}

void CCellLibraryPreviewDlg::OnEnKillfocusFileNameEdit()
{
  UpdateData(FALSE);
  GetDlgItem( IDOK )->EnableWindow( !m_strFileName.IsEmpty() );
}

void CCellLibraryPreviewDlg::OnEnSetfocusFileNameEdit()
{
  UpdateData(TRUE);
}
