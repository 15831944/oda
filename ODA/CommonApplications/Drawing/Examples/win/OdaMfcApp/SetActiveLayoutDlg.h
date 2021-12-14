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

#if !defined(AFX_SETACTIVELAYOUTDLG_H__639D4DF7_5A7E_42B6_AE11_3632C6DA030B__INCLUDED_)
#define AFX_SETACTIVELAYOUTDLG_H__639D4DF7_5A7E_42B6_AE11_3632C6DA030B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetActiveLayoutDlg.h : header file
//

#include "DbDatabase.h"
#include "Gi\GiRasterImage.h"
#include "SnapshotDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSetActiveLayoutDlg dialog

class CSetActiveLayoutDlg : public CDialog
{
// Construction
  OdDbDatabase* m_pDb;
  int m_nOldActiveLayout;
public:
  int m_nNewActiveLayout;
  bool m_bCreateNewLayout;
  CString m_sNewLayoutName;
  CString m_sCaption;
  CPreviewGiRasterImage m_Preview;

	CSetActiveLayoutDlg(OdDbDatabase* pDb, CWnd* pParent = NULL);   // standard constructor
  void FillListBox();

// Dialog Data
	//{{AFX_DATA(CSetActiveLayoutDlg)
	enum { IDD = IDD_SetActiveLayoutDlg };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetActiveLayoutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetActiveLayoutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLayoutDlgClose();
	afx_msg void OnSelchangeLayoutlist();
	afx_msg void OnDblclkLayoutlist();
	afx_msg void OnRename();
	afx_msg void OnDelete();
	afx_msg void OnCopy();
	afx_msg void OnNew();
	afx_msg void OnFromTemplate();
  //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETACTIVELAYOUTDLG_H__639D4DF7_5A7E_42B6_AE11_3632C6DA030B__INCLUDED_)
