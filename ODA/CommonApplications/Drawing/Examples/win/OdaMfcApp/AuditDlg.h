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

// AuditDlg.h: interface for the CAuditDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDITDLG_H__0783B553_D920_43D1_8119_CE76593959ED__INCLUDED_)
#define AFX_AUDITDLG_H__0783B553_D920_43D1_8119_CE76593959ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AuditDlg.h : header file
//
#include "VarDialog.h"

class OdDbAuditInfo;
/////////////////////////////////////////////////////////////////////////////
// CAuditDlg dialog

class CAuditDlg : public CVarDialog
{
// Construction
public:
	CAuditDlg(CWnd* pParent = NULL);   // standard constructor
  
  /**********************************************************************/
  /* Append error to AuditErrList or AuditInfoList                      */
  /**********************************************************************/
  void printReport (OdDbAuditInfo* pAuditInfo);
  void OnCancel();

// Dialog Data
	//{{AFX_DATA(CAuditDlg)
	enum { IDD = IDD_AUDITINFO };
	CListCtrl	m_AuditInfoList;
	CListCtrl	m_AuditErrList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAuditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAuditDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUDITDLG_H__0783B553_D920_43D1_8119_CE76593959ED__INCLUDED_)
