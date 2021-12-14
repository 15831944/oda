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

#if !defined(AFX_NAMEDVIEWSDLG_H__80FC7665_6075_437F_8058_126BDE124213__INCLUDED_)
#define AFX_NAMEDVIEWSDLG_H__80FC7665_6075_437F_8058_126BDE124213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NamedViewsDlg.h : header file
//

class COdaMfcAppDoc;
class OdDbViewTableRecord;
typedef OdSmartPtr<OdDbViewTableRecord> OdDbViewTableRecordPtr;

/////////////////////////////////////////////////////////////////////////////
// CNamedViewsDlg dialog

class CNamedViewListCtrl : public CListCtrl
{
  void setViewId(int nItem, const OdDbObjectId& id);
  void setView(int nItem, const OdDbViewTableRecord* pView);
public:
  OdDbObjectId viewId(int n) const;
  OdDbViewTableRecordPtr view(int n);
  OdDbViewTableRecordPtr selectedView();
  void InsertItem(int i, const OdDbViewTableRecord* pView);
};

class CNamedViewsDlg : public CDialog
{
  COdaMfcAppDoc *m_pDoc;
public:
  // Construction
	CNamedViewsDlg(COdaMfcAppDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

  enum
  {
    kUnchangedItem  = 0,
    kNewItem        = 1,
    kReplace        = 2
  };

  COdaMfcAppDoc* document() { return m_pDoc; }
  OdDbDatabase* database();

  // Dialog Data
	//{{AFX_DATA(CNamedViewsDlg)
	enum { IDD = IDD_DIALOG_NAMED_VIEWS };
	CNamedViewListCtrl m_views;
	//}}AFX_DATA


  // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamedViewsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

  // Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CNamedViewsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetcurrentButton();
	afx_msg void OnDblclkNamedviews(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNewButton();
	afx_msg void OnUpdateLayersButton();
	afx_msg void OnDeleteButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEDVIEWSDLG_H__80FC7665_6075_437F_8058_126BDE124213__INCLUDED_)
