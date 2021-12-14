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
#pragma once

class DgnTreeFiller;
class DgnListFiller;
class COdaDgnAppDoc;

// COdaDgnTreeView form view

class COdaDgnTreeView : public CFormView
{
	DECLARE_DYNCREATE(COdaDgnTreeView)

protected:
	COdaDgnTreeView();           // protected constructor used by dynamic creation
	virtual ~COdaDgnTreeView();

	// Attributes
public:
	COdaDgnAppDoc* GetDocument() const;

	OdDgElementId curElementId();
	OdDgElementId GetSelectedElementId();

protected:
	int m_nTotalObjects;
	int m_CurrentListItem;
	inline void setStatusText(LPCTSTR msg) { theApp.setStatusText(msg); }
	CFont m_font;

	DgnTreeFiller* m_pTreeFiller;
	DgnListFiller* m_pListFiller;

	OdDgElementId m_idCurrentElement;

	DgnTreeFiller* getTreeFiller();
	DgnListFiller* getListFiller();

	void SetControlFont();
	void fillTree();
	void SetCurrentSelectedItem();
	void SetCurrentSelectedListItem();

public:
	enum { IDD = IDD_ODADGNTREEVIEW };
	CTreeCtrl  m_ObjectTree;
	CListCtrl  m_ObjectList;
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);


	BOOL Find(HTREEITEM item, OdDgElementId id);
	void Find(const OdDgElementId& targetId);

	//{{AFX_MSG(COdaDgnTreeView)
	afx_msg void OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeydownOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightKeydownOwnershiptree(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnDoubleClickObjectList(NMHDR * pNMHDR, LRESULT * pResult);
	//  afx_msg void OnRClick(NMHDR * pNMHDR, LRESULT * pResult);
	// afx_msg void OnContextMenu(CWnd * pWnd, CPoint ptMousePos);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnItemexpandingDgntreeview(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditFind();
	afx_msg void OnRefresh();
	afx_msg void OnEditClearSelection();
  afx_msg void OnUpdateItemTypes(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonLinksAttrDlg();
  afx_msg void OnItemTypes();
};


#ifndef _DEBUG  // debug version in OdaDgnAppView.cpp
inline COdaDgnAppDoc* COdaDgnTreeView::GetDocument() const
{
	return reinterpret_cast<COdaDgnAppDoc*>(m_pDocument);
}
#endif

