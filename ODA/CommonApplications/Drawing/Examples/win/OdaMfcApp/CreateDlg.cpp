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

// DwgDocTemplate.cpp: implementation of the CDwgDocTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "odamfcapp.h"
#include "CreateDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CCreateDlg, CDialog)
  //{{AFX_MSG_MAP(CCreateDlg)
  // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CCreateDlg::CCreateDlg() : CDialog(CCreateDlg::IDD)
{
}

BOOL CCreateDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_English.SetCheck(1);
  m_Metric.SetCheck(0);
  m_bEngl = TRUE;
  return TRUE;
}

void CCreateDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_RADIO_ENGL, m_English);
  DDX_Control(pDX, IDC_RADIO_METRIC, m_Metric);
  if ( m_English.GetCheck() )
    m_bEngl = TRUE;
  else
    m_bEngl = FALSE;
}
