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
#include "OdaMfcApp.h"
#include "BBValidationErrDlg.h"


// CBBValidationErrDlg dialog

IMPLEMENT_DYNAMIC(CBBValidationErrDlg, CDialog)

CBBValidationErrDlg::CBBValidationErrDlg(const CString &strErr, CWnd* pParent /*=NULL*/)
  : CDialog(IDD_DIALOG_BBERRORS, pParent)
{
  m_strErr = strErr;
}

CBBValidationErrDlg::~CBBValidationErrDlg()
{
}

void CBBValidationErrDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT_BBERRORS, m_ValidationErrorsCtrl);
}

BOOL CBBValidationErrDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_ValidationErrorsCtrl.SetWindowText(m_strErr);
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(CBBValidationErrDlg, CDialog)
END_MESSAGE_MAP()


// CBBValidationErrDlg message handlers
