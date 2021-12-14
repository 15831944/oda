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

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__12602DAD_9F12_43F1_A849_2F6213BD2A7F__INCLUDED_)
#define AFX_MAINFRM_H__12602DAD_9F12_43F1_A849_2F6213BD2A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStatusBarColor : public CStatusBar
{
  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
};

class CMainFrame : public CMDIFrameWnd
{
  class TempBar : public OdRxObject, public CToolBar
  {
  public:
    ODRX_HEAP_OPERATORS();
    BOOL Create(CWnd* pParentWnd);
  };
	LARGE_INTEGER m_pc0;
	LARGE_INTEGER m_pc1;
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
  inline void StartTimer()
  {
	  QueryPerformanceCounter(&m_pc0);
  }

  inline void StopTimer(LPCTSTR pOperationName = NULL)
  {
	  QueryPerformanceCounter(&m_pc1);
	  m_pc1.QuadPart-=m_pc0.QuadPart;
	  if(QueryPerformanceFrequency(&m_pc0))
	  {
		  double loadTime = ((double)m_pc1.QuadPart) / ((double)m_pc0.QuadPart);
		  CString msg;
      msg.Format(_T("%s Time: %.6f sec.")
        , pOperationName ? pOperationName : _T("Operation")
        , loadTime);
      setStatusText(_tcscmp(_T("Redraw"), pOperationName)==0 ? 2 : 1, msg);
    }
  }

// Attributes
public:

// Operations
public:
	CStatusBarColor  m_wndStatusBar;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	void setStatusText(LPCTSTR msg) { setStatusText(1, msg); }
	void setStatusText(int nColumn, LPCTSTR msg) { m_wndStatusBar.SetPaneText(nColumn, msg); }
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

  void createViewerBar();
  void releaseViewerBar();
protected:  // control bar embedded members
	CToolBar    m_wndToolBar;
	TempBar*    m_pViewerBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnFilePartialOpen();
	afx_msg void OnFileRecover();
  afx_msg void OnFileMTOpen();
	afx_msg void OnClose();
  afx_msg void OnDropFiles(HDROP hDropInfo);
  afx_msg void OnOptionsDoNotEvaluateAssocNetwork();
  afx_msg void OnUpdateOptionsDoNotEvaluateAssocNetwork(CCmdUI* pCmdUI);
	//}}AFX_MSG
#ifdef ODAMFC_EXPORT_SYMBOL
  afx_msg void OnReservedItems( UINT nID );
#endif
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__12602DAD_9F12_43F1_A849_2F6213BD2A7F__INCLUDED_)
