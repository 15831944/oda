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
// SetRegVars.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "StlExportDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CStlExportDlg

CStlExportDlg::CStlExportDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CStlExportDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CStlExportDlg)
    // NOTE: the ClassWizard will add member initialization here
  m_Binary = TRUE;
  //}}AFX_DATA_INIT
}

void CStlExportDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CStlExportDlg)
  DDX_Text(pDX, IDC_PATH, m_Path);
  DDV_MaxChars(pDX, m_Path, 255);
  DDX_Text(pDX, IDC_HANDLE, m_Handle);
  DDV_MaxChars(pDX, m_Handle, 10);
  DDX_Check(pDX, IDC_CHECK_BINARY, m_Binary);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CStlExportDlg, CDialog)
  //{{AFX_MSG_MAP(CStlExportDlg)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CStlExportDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  m_Path = theApp.GetProfileString(_T("options"), _T("STL export"), _T(""));
  m_Handle = theApp.GetProfileString(_T("options"), _T("STL handle"), _T(""));
  m_Binary = theApp.GetProfileString(_T("options"), _T("STL binary"), _T("Y")) == _T("Y");
  UpdateData(FALSE);

  return TRUE;
}

void CStlExportDlg::OnOK() 
{
  UpdateData();

  theApp.WriteProfileString(_T("options"), _T("STL export"), m_Path);
  theApp.WriteProfileString(_T("options"), _T("STL handle"), m_Handle);
  theApp.WriteProfileString(_T("options"), _T("STL binary"), m_Binary ? _T("Y") : _T("N"));

	CDialog::OnOK();
}

