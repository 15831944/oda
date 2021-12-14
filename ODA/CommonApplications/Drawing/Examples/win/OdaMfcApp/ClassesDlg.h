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

//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_CLASSESDLG_H__4FE6DEFE_47F8_41CE_A163_7EE48499F05B__INCLUDED_)
#define AFX_CLASSESDLG_H__4FE6DEFE_47F8_41CE_A163_7EE48499F05B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClassesDlg.h : header file
//

#include "DbDatabase.h"

/////////////////////////////////////////////////////////////////////////////
// CClassesDlg dialog

class CClassesDlg : public CDialog
{
// Construction
  OdDbDatabase* m_pDb;
public:
	CClassesDlg(OdDbDatabase* pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CClassesDlg)
	enum { IDD = IDD_CLASSES };
	CTreeCtrl	m_ClassTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CClassesDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedBtnClassesDump();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSESDLG_H__4FE6DEFE_47F8_41CE_A163_7EE48499F05B__INCLUDED_)
