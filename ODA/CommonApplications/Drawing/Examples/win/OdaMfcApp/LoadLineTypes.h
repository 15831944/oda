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

#if !defined(AFX_LOADLINETYPES_H__65E0B3DA_8CCB_48A5_B6ED_ECF8F7CA5C62__INCLUDED_)
#define AFX_LOADLINETYPES_H__65E0B3DA_8CCB_48A5_B6ED_ECF8F7CA5C62__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadLineTypes.h : header file
//

#include "DbDatabase.h"

/////////////////////////////////////////////////////////////////////////////
// CLoadLineTypes dialog

class CLoadLineTypes : public CDialog
{
// Construction
  OdDbDatabasePtr m_pDb;
  BOOL m_SomethingDoing;
  int  m_DescIndx;
public:
	CLoadLineTypes(OdDbDatabasePtr pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLoadLineTypes)
	enum { IDD = IDD_DIALOG_LOAD_LINETYPE };
	CListCtrl	m_ListCtrl;
  CString   m_FileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadLineTypes)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
// Implementation
  BOOL isSomethingDone() { return m_SomethingDoing; }

protected:
	BOOL ReadLinetypeFileLines(const TCHAR *fileName, CStringArray &ret);
protected:

	// Generated message map functions
	//{{AFX_MSG(CLoadLineTypes)
	afx_msg void OnOpenLinetypefile();
	afx_msg void OnLoadSelection();
	afx_msg void OnButtonLoadAll();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADLINETYPES_H__65E0B3DA_8CCB_48A5_B6ED_ECF8F7CA5C62__INCLUDED_)
