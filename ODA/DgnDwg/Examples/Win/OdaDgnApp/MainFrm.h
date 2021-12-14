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


#pragma once

class CMainFrame : public CMDIFrameWnd
{
  DECLARE_DYNAMIC(CMainFrame)
public:
  CMainFrame();
  virtual ~CMainFrame();

// Attributes
public:

// Operations
public:
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
      setStatusText(_tcscmp(_T("Regen"), pOperationName)==0 ? 2 : 1, msg);
    }
  }

// Overrides
public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  void setStatusText(LPCTSTR msg) { setStatusText(1, msg); }
  void setStatusText(int nColumn, LPCTSTR msg) { m_wndStatusBar.SetPaneText(nColumn, msg); }

protected:  // control bar embedded members
  CStatusBar  m_wndStatusBar;
  CToolBar    m_wndToolBar;
  CToolBar    m_vectorizerToolBar;

private:
  LARGE_INTEGER m_pc0;
  LARGE_INTEGER m_pc1;

// Generated message map functions
protected:
  //{{AFX_MSG(CMainFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnClose();
  afx_msg void OnDropFiles(HDROP hDropInfo);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};


