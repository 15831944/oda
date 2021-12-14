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

#if !defined(AFX_XREFVIEWDLG_H__95F479A9_8912_4F93_B995_35D1A02DB992__INCLUDED_)
#define AFX_XREFVIEWDLG_H__95F479A9_8912_4F93_B995_35D1A02DB992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XrefViewDlg.h : header file
//

#include "DbXrefGraph.h"

/////////////////////////////////////////////////////////////////////////////
// CXrefViewDlg dialog

class CXrefViewDlg : public CDialog
{
// Construction
  OdDbDatabase*       m_pDb;
  OdDbXrefGraph       m_graph;
	CToolBarCtrl        m_wndToolBar;
  OdDbXrefGraphNode*  m_pNode;

  int                 m_nListItem;

  void OnSelChanged(OdDbXrefGraphNode* pNewNode);
  bool SelectInTree(DWORD dwKey, HTREEITEM hRoot = 0);
  void SelectInList(LONG_PTR dwKey);
public:
	CXrefViewDlg(OdDbDatabase* pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CXrefViewDlg)
	enum { IDD = IDD_XREFVIEW };
	CButton	m_btnBrowse;
	CButton	m_btnSavePath;
	CEdit	m_foundAt;
	CListCtrl	m_list;
	CButton	m_btnReload;
	CButton	m_btnUnload;
	CTreeCtrl	m_tree;
	//}}AFX_DATA

  OdDbObjectIdArray m_xrefsToReload;
  OdDbObjectIdArray m_xrefsToUnload;
  OdDbObjectIdArray m_xrefsToDetach;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXrefViewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

  void fillTree(OdDbXrefGraphNode* pNode, HTREEITEM hParent);
  void fillList();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CXrefViewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedXreftree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBrowse();
	afx_msg void OnClickXreflist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReload();
	afx_msg void OnUnload();
	//}}AFX_MSG

	afx_msg void OnViewList();
	afx_msg void OnViewTree();

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedDetach();
  afx_msg void OnBnClickedAttach();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XREFVIEWDLG_H__95F479A9_8912_4F93_B995_35D1A02DB992__INCLUDED_)
