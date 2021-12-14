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

// AuditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "AuditDlg.h"
#include "DbAudit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAuditDlg dialog


CAuditDlg::CAuditDlg(CWnd* pParent /*=NULL*/)
	: CVarDialog(CAuditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAuditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAuditDlg::DoDataExchange(CDataExchange* pDX)
{
	CVarDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAuditDlg)
	DDX_Control(pDX, IDC_AUDITSUM_LIST, m_AuditInfoList);
	DDX_Control(pDX, IDC_AUDITINFO_LIST, m_AuditErrList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAuditDlg, CVarDialog)
	//{{AFX_MSG_MAP(CAuditDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAuditDlg message handlers

BOOL CAuditDlg::OnInitDialog() 
{
	CVarDialog::OnInitDialog();

  m_AuditInfoList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 400);
  m_AuditInfoList.DeleteAllItems();  

  m_AuditErrList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 100);
  m_AuditErrList.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 100);
  m_AuditErrList.InsertColumn(2, _T("Validation"), LVCFMT_LEFT, 100);
  m_AuditErrList.InsertColumn(3, _T("Default value"), LVCFMT_LEFT, 100);
  m_AuditErrList.DeleteAllItems();  

  ///////////////////////////////////////////////////////////////////////////
  //////////// Initialize the resize help data member ///////////////////////
  ///////////////////////////////////////////////////////////////////////////
  initResizeHelper();
  m_resizeHelper.Fix(IDC_AUDITSUM_LIST,  DlgResizeHelper::kLeftRight, DlgResizeHelper::kNoVFix);
  m_resizeHelper.Fix(IDC_AUDITINFO_LIST,  DlgResizeHelper::kLeftRight, DlgResizeHelper::kNoVFix);
  m_resizeHelper.Fix(IDC_ST_AUDITERR,  DlgResizeHelper::kLeft, DlgResizeHelper::kNoVFix);
  m_resizeHelper.Fix(IDC_ST_AUDITSUM,  DlgResizeHelper::kLeft, DlgResizeHelper::kNoVFix);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
/************************************************************************/
/* Append error to AuditErrList or AuditInfoList                        */
/************************************************************************/
void CAuditDlg::printReport (OdDbAuditInfo* pAuditInfo)
{
  if (pAuditInfo->getLastInfo().bIsError)
  {
    int nItem = m_AuditErrList.GetItemCount();
    m_AuditErrList.InsertItem (nItem, pAuditInfo->getLastInfo().strName);
    m_AuditErrList.SetItemText (nItem, 1, pAuditInfo->getLastInfo().strValue);
    m_AuditErrList.SetItemText (nItem, 2, pAuditInfo->getLastInfo().strValidation);
    m_AuditErrList.SetItemText (nItem, 3, pAuditInfo->getLastInfo().strDefaultValue);
  }
  else
  {
    int nItem = m_AuditInfoList.GetItemCount();
    m_AuditInfoList.InsertItem (nItem, pAuditInfo->getLastInfo().strName);
  }
}

void CAuditDlg::OnCancel() 
{
  DestroyWindow ();
  delete this;
}
