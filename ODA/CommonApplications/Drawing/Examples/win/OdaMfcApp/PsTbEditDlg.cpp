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

// Sheet.cpp : implementation file
//

#include "stdafx.h"
#include "PsTbEditDlg.h"
#include "PsTbEditFormViewPg.h"
#include "PsTbEditGeneralPg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Sheet

IMPLEMENT_DYNAMIC(CPsTableEditorDlg, CPropertySheet)

CPsTableEditorDlg::CPsTableEditorDlg(CWnd *pParent)
	: CPropertySheet(_T("Plot Style Table Editor"), pParent)
{

	m_psh.dwFlags |= PSH_NOAPPLYNOW;

	AddPage(&m_page1);
	AddPage(&m_page2);
}

CPsTableEditorDlg::~CPsTableEditorDlg()
{
}


BEGIN_MESSAGE_MAP(CPsTableEditorDlg, CPropertySheet)
	//{{AFX_MSG_MAP(CSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CPsTableEditorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Sheet message handlers

BOOL CPsTableEditorDlg::OnInitDialog() 
{
	CPropertySheet::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


const bool CPsTableEditorDlg::SetPlotStyleTable(OdPsPlotStyleTable* pPlotStyleTable)
{
  if( !pPlotStyleTable ) return false;

  m_pPlotStyleTable = pPlotStyleTable;

  m_pPsTabForPropertyPg = pPlotStyleTable->clone();
  OdPsPlotStyleTable *pPsTab = m_pPsTabForPropertyPg.get();

  return m_page1.SetPlotStyleTable(pPsTab) 
      && m_page2.SetPlotStyleTable(pPsTab);
}

void CPsTableEditorDlg::SetFileBufPath(const OdString sFilePath)
{
  m_page1.SetFileBufPath(sFilePath);
  m_page2.SetFileBufPath(sFilePath);
}

OdPsPlotStyleTablePtr CPsTableEditorDlg::GetPlotStyleTable() const
{
  return m_pPsTabForPropertyPg;
}
