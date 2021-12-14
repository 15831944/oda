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

// HoopsViewerView.h : interface of the CHoopsViewerView class
//

#if !defined (_HOOPSVIEWERVIEW_INCLUDED)
#define _HOOPSVIEWERVIEW_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// used for the copy to clipboard code
#define WMF             0
#define EMF             1

// different rendermode
#define SHADED		0
#define WIREFRAME	1
#define HIDDENLINE	2


// msw printer driver specific driver_option debug flags
#define DEBUG_PRINT_NOT_FRAMEBUFFER_MODE	0x00020000
#define DEBUG_PRINT_NOT_RASTER_REDUCTION  	0x00200000


// msw driver options debug
#define DEBUG_NO_WINDOWS_HOOK		0x00000040
#define DEBUG_FORCE_FULL_COLOR		0x00000010
#define DEBUG_SFB_COPY_TO_CLIPBOARD	0x00004000
#define DEBUG_NO_RASTERS_IN_PRINTING	0x00020000
#define DEBUG_PRINTING_MODE         0x00040000
#define DEBUG_CLIPBOARD_MODE        0x00080000

// rendering options debug
#define DEBUG_NO_PAINTERS_PANELLING	0x00000008

enum ViewMode
{
  kOrbit = 0,
  kZoom  = 1,
  kPan   = 2
};

class CHoopsViewerView : public CView
{
private:
  bool	m_bOperatorStarted;
  bool	m_bFastPrint;
  bool	m_bClipboardPrinting;
  bool	m_bClipboardTruecolor;
  int		m_MetafileType;
  HC_KEY	m_lDriverKey;
  HC_KEY	m_lSceneKey;
  HC_KEY	m_lLightKey;
  HC_KEY  m_lRectangleKey;
  HC_KEY  m_lMarkerKey;
  HC_KEY  m_AxisTriadKey;
  char	m_sDriverSegment[1024];
  CPalette	*m_pViewPalette;
  HPoint	m_ptWorldFirst;
  HPoint  m_ptRealPoint;
  HPoint  m_ptFirstReal;
  HPoint  m_ptRealOld;
  // For zoom
  HPoint	m_ptDist, m_ptCamera;
  float	  m_Width, m_Height, m_fLength;
  // For zoom window
  HPoint	m_ptRectangle[2];
  HPoint  m_ptFirst;
  char    m_pSavedVisibility[1024];
  char    m_pSavedColor[1024];
  bool	  m_bUseCenterMarker;	
  bool    m_bResetColor;
  bool    m_bResetVisibility;

  ViewMode m_iViewMode;
  bool     m_bZoomWindowOn;

  int			m_AxisWindowSize;
protected: // create from serialization only
	CHoopsViewerView();
	DECLARE_DYNCREATE(CHoopsViewerView)

// Attributes
public:
	CHoopsViewerDoc* GetDocument() const;
  CPalette	*GetPalette();
  void	InitializePalette();
  void	FitWorld();
  int		GetRenderMode();

private:
  void OnOrbitMouseMove( HPoint& point, HPoint& real_new );
  void OnZoomMouseMove( HPoint point );
  void OnPanMouseMove( HPoint point );
  void OnZoomWindowMouseMove( HPoint point );
  void ZoomWindow();

// Operations
public:

// Overrides
public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual void OnInitialUpdate();
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
// Implementation
public:
	virtual ~CHoopsViewerView();
  void SetAxis();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
  void UpdateLamp();
  void InsertArrow(float stick_length, float tip_length, float stick_radius, float arrow_radius, const char *text, const char *stick_color, const char *tip_color, int num_sides = 6, float tip_falloff = 0.05, bool reverse = true);
  bool GeneratePointsOnCircle( HPoint* points, HPoint const & center,float radius,int numpoints);
  void AdjustAxisWindow();
  void RotateAxisOnUpdate();


protected:

// Generated message map functions
protected:
  //{{AFX_MSG(CDWGViewerView)
  afx_msg void OnPaint();
  afx_msg void OnEditCopy();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
  afx_msg void OnViewWireframe();
  afx_msg void OnUpdateViewWireframe(CCmdUI* pCmdUI);
  afx_msg void OnViewShaded();
  afx_msg void OnUpdateViewShaded(CCmdUI* pCmdUI);
  afx_msg void OnViewHiddenline();
  afx_msg void OnUpdateViewHiddenline(CCmdUI* pCmdUI);
  afx_msg void OnOrbit();
  afx_msg void OnZoom();
  afx_msg void OnPan();
  afx_msg void OnZoomWindow();
  afx_msg void OnZoomToExtents();
  afx_msg void OnUpdateOrbit(CCmdUI* pCmdUI);
  afx_msg void OnUpdateZoom(CCmdUI* pCmdUI);
  afx_msg void OnUpdatePan(CCmdUI* pCmdUI);
  afx_msg void OnUpdateZoomToExtents(CCmdUI* pCmdUI);
  afx_msg void OnUpdateZoomWindow(CCmdUI* pCmdUI);
  afx_msg void OnXYPlane();
  afx_msg void OnBottomView();
  afx_msg void OnXZPlane();
  afx_msg void OnRightView();
  afx_msg void OnYZPlane();
  afx_msg void OnBackView();
  afx_msg void OnSWIsometric();
  afx_msg void OnSEIsometric();
  afx_msg void OnNEIsometric();
  afx_msg void OnNWIsometric();
  afx_msg void OnAxisOnOff();
  afx_msg void OnUpdateAxisOnOff(CCmdUI* pCmdUI);
  afx_msg void ChangeBgColor();
  afx_msg void OnSetActiveLayout();
  afx_msg void OnUpdateSetActiveLayout(CCmdUI* pCmdUI);
  afx_msg void OnShowText();
  afx_msg void OnUpdateShowText(CCmdUI* pCmdUI);
  afx_msg void OnShowLines();
  afx_msg void OnUpdateShowLines(CCmdUI* pCmdUI);
  //}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HoopsViewerView.cpp
inline CHoopsViewerDoc* CHoopsViewerView::GetDocument() const
   { return reinterpret_cast<CHoopsViewerDoc*>(m_pDocument); }
#endif

#endif
