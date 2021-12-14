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

// DwgView.h : interface of the CDwgView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DWGVIEW_H__65AE4922_0D08_4C35_8940_8419CE77C84C__INCLUDED_)
#define AFX_DWGVIEW_H__65AE4922_0D08_4C35_8940_8419CE77C84C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STL_USING_STACK
#include "OdaSTL.h"

#include "DbDatabase.h"
#include "DbDictionary.h"
#include "DbFiler.h"

class COdaMfcAppDoc;

extern COdaMfcAppApp theApp;

typedef OdArray<const OdRxObject*, OdMemoryAllocator<const OdRxObject*> > OdRxObjectRawPtrArray;

class CDwgView : public CFormView
{
protected: // create from serialization only
	CDwgView();
	DECLARE_DYNCREATE(CDwgView)
public:
	//{{AFX_DATA(CDwgView)
	enum { IDD = IDD_ODAMFCAPP_FORM };
	CTreeCtrl	m_ObjectTree;
	CListCtrl	m_ObjectList;
  NM_TREEVIEW m_savedLast;
  OdDbObjectIdArray m_found;
  OdRxObjectRawPtrArray m_foundSub;
  OdDbObjectIdArray::size_type m_nCurFound;
	//}}AFX_DATA

// Attributes
public:
	COdaMfcAppDoc* GetDocument();

// Operations
public:
  void fillTree();

  OdDbObjectId curObjectId();

  void UpdateFieldDisplay();
  // Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDwgView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~CDwgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

  class Condition {
  public:
    virtual bool check( const OdDbObject* pObj, const OdRxObject* pSubObj ) const = 0;
  };
protected:
	int m_nTotalObjects;
  inline void setStatusText(LPCTSTR msg) { theApp.setStatusText(msg); }
  CFont m_font;
  void SetControlFont();
public:
  void Find( const OdDbObjectId& targetId, const OdRxObject* pSubObj = 0 );
protected:
  void Find( const CDwgView::Condition& condition );
  bool Find( HTREEITEM hRoot, const CString& sub );

// Generated message map functions
protected:
	//{{AFX_MSG(CDwgView)
	afx_msg void OnSelchangingOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewClassesinfo();
	afx_msg void OnProperties();
	afx_msg void OnEditFind();
	afx_msg void OnRefresh();
	afx_msg void OnViewSystemvariables();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnItemexpandingOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewRenderMaterials();
	afx_msg void OnViewRenderLight();
	afx_msg void OnEditAcisConversion();
	afx_msg void OnViewFiledependency();
	afx_msg void OnEditAcisSatSave();
	afx_msg void OnEditReduce();
	afx_msg void OnEditClearSelection();
	afx_msg void OnUpdateExplode(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEditRecomputeDimBlock();
  afx_msg void OnEditEntget();
  afx_msg void OnNMClickOwnershiptree(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnEditFindnextselected();
};

#ifndef _DEBUG  // debug version in DwgView.cpp
inline COdaMfcAppDoc* CDwgView::GetDocument()
   { return (COdaMfcAppDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DWGVIEW_H__65AE4922_0D08_4C35_8940_8419CE77C84C__INCLUDED_)
