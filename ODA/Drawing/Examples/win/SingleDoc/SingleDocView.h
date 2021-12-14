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

/************************************************************************/
/* Interface of the CSingleDocView class                                */
/************************************************************************/
#if !defined(AFX_SINGLEDOCVIEW_H__5167F433_4817_4BB7_A231_76FD45420A19__INCLUDED_)
#define AFX_SINGLEDOCVIEW_H__5167F433_4817_4BB7_A231_76FD45420A19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GiContextForDbDatabase.h"


class CSingleDocView : public CView , OdGiContextForDbDatabase
{
  BOOL m_bPsOverall;
protected:
  using CView::operator new;
  using CView::operator delete;
protected: // create from serialization only
	CSingleDocView();
	DECLARE_DYNCREATE(CSingleDocView)
  void addRef() {}
  void release() {}

  enum InteractionMode
  {
    kNone,
    kDolly,
    kRoll,
    kZoomWindow,
    kRotateX,
    kRotateY,
    kRotateZ
  };
  
  InteractionMode m_interactionMode;
  HCURSOR         m_hSavedCur;
  CPoint          m_startPoint;
  CPoint          m_endPoint;
  void setCursor(LPCTSTR szCurName);
  void resetCursor();

// Attributes
public:
	CSingleDocDoc* GetDocument();

// Operations
public:
  void ViewZoomextents();
  void resetDevice(bool zoomeExtents = false );
  void setViewportBorderProperties(OdGsDevice* pDevice, bool bModel) ;
  OdGsDevicePtr m_pDevice, m_pPrinterDevice;
  void OnOptionsRenderingdeviceChanged(); 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSingleDocView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSingleDocView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSingleDocView)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SingleDocView.cpp
inline CSingleDocDoc* CSingleDocView::GetDocument()
   { return (CSingleDocDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGLEDOCVIEW_H__5167F433_4817_4BB7_A231_76FD45420A19__INCLUDED_)
