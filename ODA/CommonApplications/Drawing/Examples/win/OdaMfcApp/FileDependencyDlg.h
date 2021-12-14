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

#if !defined(AFX_FILEDEPENDENCYDLG_H__27E84663_1128_4E52_9EC0_D92E1D0640F9__INCLUDED_)
#define AFX_FILEDEPENDENCYDLG_H__27E84663_1128_4E52_9EC0_D92E1D0640F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileDependencyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileDependencyDlg dialog

#include "VarDialog.h"

class CFileDependencyDlg : public CVarDialog
{
// Construction
public:
	CFileDependencyDlg(OdDbDatabase *pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileDependencyDlg)
	enum { IDD = IDD_FILEDEPENDENCY };
	CListCtrl	m_FileDepList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDependencyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  OdDbDatabase* m_pDb;

	// Generated message map functions
	//{{AFX_MSG(CFileDependencyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDEPENDENCYDLG_H__27E84663_1128_4E52_9EC0_D92E1D0640F9__INCLUDED_)
