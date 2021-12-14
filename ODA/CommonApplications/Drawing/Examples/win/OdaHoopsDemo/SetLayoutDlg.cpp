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

// SetLayoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetLayoutDlg.h"
#include "HoopsViewer.h"

// CSetLayoutDlg dialog

IMPLEMENT_DYNAMIC(CSetLayoutDlg, CDialog)

CSetLayoutDlg::CSetLayoutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLayoutDlg::IDD, pParent)
{
  m_iSelItem = -1;
}

CSetLayoutDlg::~CSetLayoutDlg()
{
}

void CSetLayoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetLayoutDlg, CDialog)
  ON_BN_CLICKED(IDOK, &CSetLayoutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetLayoutDlg message handlers

BOOL CSetLayoutDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CListBox* pList = (CListBox*)GetDlgItem(IDC_LAYOUT_LIST);

  for( unsigned int i = 0; i < m_strLayoutsArr.size(); i++ )
    pList->AddString( m_strLayoutsArr[i] );

  if( m_iSelItem >= 0 )
    pList->SetCurSel( m_iSelItem );

  return TRUE;
}

void CSetLayoutDlg::OnBnClickedOk()
{
  CListBox* pList = (CListBox*)GetDlgItem(IDC_LAYOUT_LIST);

  if( pList->GetCurSel() != LB_ERR )
    m_iSelItem = pList->GetCurSel();

  OnOK();
}
