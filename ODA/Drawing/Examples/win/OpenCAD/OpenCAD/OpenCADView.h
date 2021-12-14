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

// OpenCADView.h : interface of the COpenCADView class
//


#pragma once

#include "GiContextForDbDatabase.h"
#include "Gs/Gs.h"
#include "ChildFrm.h"

class COpenCADView : public CView, OdGiContextForDbDatabase
{
protected:
	using CView::operator new;
	using CView::operator delete;
	void addRef() {}
	void release() {}

protected: // create from serialization only
	COpenCADView();
	DECLARE_DYNCREATE(COpenCADView)

// Attributes
public:
	OdGsDevicePtr m_pDevice;			// Vectorizer device
	ODCOLORREF m_clrBackground;			// Drawing background color
	OdGsView::RenderMode m_iRenderMode;	// Render mode
	BOOL m_bLeftButton;					// Flag for left mouse button press
	BOOL m_bMiddleButton;				// Flag for middle mouse button press
	BOOL m_bRightButton;				// Flag for right mouse button press
	CPoint m_MousePosition;				// Position of mouse pointer
	BOOL m_bZoomWindow;					// Flag for zoom window mode
	OdGePoint3dArray m_Points;			// Mouse clicks
	CPoint m_MouseClick;				// Location of mouse click
	OdEdInputTracker* m_pTracker;		// Input tracker

// Operations
public:
	COpenCADDoc* GetDocument() const;
	void ResetDevice(BOOL bZoomExtents = FALSE);
	void SetViewportBorderProperties(OdGsDevice* pDevice, BOOL bModel);
	OdGsViewPtr GetView();
	void ViewZoomExtents();
	const ODCOLORREF* CurrentPalette();
	void SetRenderMode(OdGsView::RenderMode iRenderMode);
	void Dolly(int x, int y);
	OdGePoint3d GetWorldCoordinates(CPoint Point);
	void ZoomWindow(OdGePoint3d Point1, OdGePoint3d Point2);
	CChildFrame* GetChildFrame() { return (CChildFrame*)GetParentFrame(); };
	void Track(OdEdInputTracker* pTracker);
	void Refresh();

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~COpenCADView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnPaint();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnViewRendermode2dwireframe();
public:
	afx_msg void OnUpdateViewRendermode2dwireframe(CCmdUI *pCmdUI);
public:
	afx_msg void OnViewRendermode3dwireframe();
public:
	afx_msg void OnUpdateViewRendermode3dwireframe(CCmdUI *pCmdUI);
public:
	afx_msg void OnViewRendermode3dhidden();
public:
	afx_msg void OnUpdateViewRendermode3dhidden(CCmdUI *pCmdUI);
public:
	afx_msg void OnViewRendermodeFlatshaded();
public:
	afx_msg void OnUpdateViewRendermodeFlatshaded(CCmdUI *pCmdUI);
public:
	afx_msg void OnViewRendermodeFlatshadedwireframe();
public:
	afx_msg void OnUpdateViewRendermodeFlatshadedwireframe(CCmdUI *pCmdUI);
public:
	afx_msg void OnViewRendermodeSmoothshaded();
public:
	afx_msg void OnUpdateViewRendermodeSmoothshaded(CCmdUI *pCmdUI);
public:
	afx_msg void OnViewRendermodeSmoothshadedwireframe();
public:
	afx_msg void OnUpdateViewRendermodeSmoothshadedwireframe(CCmdUI *pCmdUI);
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnViewZoomextents();
public:
	afx_msg void OnViewZoomwindow();
public:
	afx_msg void OnUpdateViewZoomwindow(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in OpenCADView.cpp
inline COpenCADDoc* COpenCADView::GetDocument() const
   { return reinterpret_cast<COpenCADDoc*>(m_pDocument); }
#endif
