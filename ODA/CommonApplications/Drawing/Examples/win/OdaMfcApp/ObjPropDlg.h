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
#if !defined(AFX_OBJPROPDLG_H__8D92EBB0_ABBF_442D_A749_D86F05454BCE__INCLUDED_)
#define AFX_OBJPROPDLG_H__8D92EBB0_ABBF_442D_A749_D86F05454BCE__INCLUDED_
#if defined(_TOOLKIT_IN_DLL_) && (_MSC_VER > 1310)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ObjPropDlg.h : header file
//

#define _INC_MALLOC
#include "comdef.h"

/////////////////////////////////////////////////////////////////////////////
// CObjPropDlg dialog

class CObjPropDlg : public CDialog, protected IPropertyNotifySink
{
  ULONG STDMETHODCALLTYPE AddRef() { return 1; }
  ULONG STDMETHODCALLTYPE Release() { return 1; }
  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
  HRESULT STDMETHODCALLTYPE OnChanged(DISPID dispID);
  HRESULT STDMETHODCALLTYPE OnRequestEdit(DISPID dispID);
// Construction
  IUnknownPtr m_pObjWrapper;
  int m_nCurItem;
  DWORD m_nCookie;
public:
	CObjPropDlg(const OdDbObjectId &id, CWnd* pParent = NULL);   // standard constructor
	virtual ~CObjPropDlg();

// Dialog Data
	//{{AFX_DATA(CObjPropDlg)
	enum { IDD = IDD_PROPERTIES };
	CButton	m_doset;
	CListCtrl	m_propList;
	CString	m_sValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void fill();

	// Generated message map functions
	//{{AFX_MSG(CObjPropDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusValue();
	afx_msg void OnButton();
	afx_msg void OnClickProplist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownProplist(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //#ifdef _TOOLKIT_IN_DLL_
#endif // !defined(AFX_OBJPROPDLG_H__8D92EBB0_ABBF_442D_A749_D86F05454BCE__INCLUDED_)
