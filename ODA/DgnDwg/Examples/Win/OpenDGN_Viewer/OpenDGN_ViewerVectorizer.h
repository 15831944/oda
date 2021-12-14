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

#ifndef _AFX_ODADGNAPPVECTORIZER__INCLUDED_
#define _AFX_ODADGNAPPVECTORIZER__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Ge/GeMatrix3d.h"
#include "Gi/Gi.h"
#include "DgGsManager.h"
#include "DgGiContext.h"
#include <afxole.h>


class COdaDgnAppVectorizerBase : public CView
                               , private OdGiContextForDgDatabase
{
protected:
  using CView::operator new;
  using CView::operator delete;
private:
  HDC                 m_hWindowDC;

  COdaDgnAppVectorizerBase();           // protected constructor used by dynamic creation

  DECLARE_DYNCREATE(COdaDgnAppVectorizerBase);

// Attributes
public:


  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COdaDgnAppVectorizer)
  public:
  virtual void OnInitialUpdate();
  //}}AFX_VIRTUAL
  protected:
  virtual void OnDraw(CDC* pDC);      // overridden to draw this view

  // Implementation
  void addRef() {}
  void release() {}

  BOOL PreCreateWindow(CREATESTRUCT& cs);

  COdaDgnAppDoc* GetDocument();

protected:
  virtual ~COdaDgnAppVectorizerBase();
#ifdef _DEBUG
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(COdaDgnAppVectorizer)
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

///////////////////////////
class COdaDgnAppVectorizer : public CView
                           , private OdGiContextForDgDatabase

{
  void destroyDevice();
  COleDropTarget m_dropTarget;
  mutable bool      m_bRegenAbort;
  CPoint            m_oldPoint;
  HCURSOR           m_hCursor;

  enum Mode
  {
    kQuiescent,
    kGetPoint,
    kGetString,
    kDragDrop
  }
  m_mode;

  class CurrentOperation
  {
  public:
    enum Type
    {
      k3dOrbit,
      kPan,
      kNone
    };

    void initialize( CWnd* window );

    void setStartPoint( const CPoint& startPoint );
    void drawMarker( const CPoint& currentPoint );
    void undoDrawnMarker();
    void setType( Type type );
    Type getType() const;

    CPoint toCPoint( const OdGePoint2d& original ) const;
    POINT toPOINT( const OdGePoint2d& original ) const;

    const OdGeMatrix3d& getResultRotation() const;
    const OdGeVector2d& getResultOffset() const;

  private:
    struct DrawnMarker
    {
      DrawnMarker();

      Type m_type;
      CPoint m_point;
    };

    void drawMarkerFor3dOrbit( const CPoint& currentPoint );
    void drawMarkerForPan( const CPoint& currentPoint );

    Type m_type;
    CPoint m_startPoint;
    CWnd* m_window;
    DrawnMarker m_drawnMarker;

    OdGeMatrix3d m_resultRotation;
    OdGeVector2d m_resultOffset;
  } m_currentOperation;

  struct Response
  {
  public:
    enum Type
    {
      kNone,
      kPoint,
      kString,
      kCancel
    };
    Type            m_type;
    OdGePoint3d     m_point;
    OdString        m_string;
  };
  Response          m_response;

  int               m_inpOptions;
  bool              m_bTrackerHasDrawables;
  OdGePoint3d       m_basePt;
  const OdGePoint3d*m_pBasePt;

  bool beginDragCallback(const OdGePoint3d& pt);
protected:
  using CView::operator new;
  using CView::operator delete;

  CToolBar  m_toolBar;
private:
  OdDgElementId      m_modelId;

  bool              m_bPlotPlotstyle;
  bool              m_bShowPlotstyle;

  void dolly(int x, int y);
  OdGiContext::PStyleType plotStyleType() const;
protected:
  OdGsDeviceForDgModelPtr m_pDevice;
  HDC                 m_hWindowDC;

  CRect viewportRect() const;
  static CRect viewRect(OdGsView*);

  COdaDgnAppVectorizer();           // protected constructor used by dynamic creation

  DECLARE_DYNCREATE(COdaDgnAppVectorizer);

// Attributes
public:
  OdGsView* getActiveView();
  const OdGsView* getActiveView() const;

  void setCursor(HCURSOR hCursor);
  HCURSOR cursor() const;

  void setViewportBorderProperties();
  bool canClose() const;
  bool isGettingString() const { return m_mode!= kQuiescent; }
  int inpOptions() const { return m_inpOptions; }
  void respond(const OdString& s);

  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(COdaDgnAppVectorizer)
  public:
  virtual void OnInitialUpdate();
  protected:
  virtual void OnDraw(CDC* pDC);      // overridden to draw this view
  //}}AFX_VIRTUAL

  // Implementation
  void addRef() {}
  void release() {}

  COdaDgnAppDoc* GetDocument();

protected:
  virtual ~COdaDgnAppVectorizer();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(COdaDgnAppVectorizer)
  afx_msg void OnVectorize();
  afx_msg void OnPaint();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnDestroy();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnViewZoomin();
  afx_msg void OnViewZoomout();
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  afx_msg void OnViewZoomextents();
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnActivateFrame(UINT nState, CFrameWnd* pFrameWnd);

  afx_msg void OnAppendLine();
  afx_msg void OnUpdateAppendLine(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDrag(CCmdUI* pCmdUI);
  afx_msg void OnViewerRegen();
  afx_msg void OnViewerVpregen();
  afx_msg void OnUpdateViewerRegen(CCmdUI* pCmdUI);
  afx_msg void OnUpdate2dwireframe(CCmdUI* pCmdUI);
  afx_msg void OnUpdate3dwireframe(CCmdUI* pCmdUI);
  afx_msg void OnUpdateHidden(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFlatShaded(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFlatShadedWithEdges(CCmdUI* pCmdUI);
  afx_msg void OnUpdateGouraudShaded(CCmdUI* pCmdUI);
  afx_msg void OnUpdateGouraudShadedEdgesOn(CCmdUI* pCmdUI);
  afx_msg void OnViewZoomWindow();

  afx_msg void OnView3dOrbit();
  afx_msg void OnUpdateView3dOrbit(CCmdUI* pCmdUI);
  afx_msg void OnViewPan();
  afx_msg void OnUpdateViewPan(CCmdUI* pCmdUI);

  //}}AFX_MSG
  afx_msg void OnSetRenderMode(UINT nID);

  afx_msg void OnWinDirectX();
  afx_msg void OnWinGDI();
  afx_msg void OnWinOpenGL();

  afx_msg void OnViewNumber(UINT nID);
  afx_msg void OnReToolbar();
  afx_msg void OnUpdateView1(CCmdUI* pCmdUI);
  afx_msg void OnUpdateView2(CCmdUI* pCmdUI);
  afx_msg void OnUpdateView3(CCmdUI* pCmdUI);
  afx_msg void OnUpdateView4(CCmdUI* pCmdUI);
  afx_msg void OnUpdateView5(CCmdUI* pCmdUI);
  afx_msg void OnUpdateView6(CCmdUI* pCmdUI);
  afx_msg void OnUpdateView7(CCmdUI* pCmdUI);
  afx_msg void OnUpdateView8(CCmdUI* pCmdUI);

  DECLARE_MESSAGE_MAP()
  void adjustDevice(OdGsDevice* pDevice);
  void createDevice(const OdDgElementId& modelId, const OdDgElementId& viewId );
  bool regenAbort() const;
public:
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);

  //I believe this functionality exists somewhere within TD, but could not find that.
  //This method returns a point on the focal plane (for perspective cameras) or on the target plane (orthogonal projections)
  OdGePoint3d getWorldCoordinateByScreen( const OdGePoint2d& screenPoint ) const;

protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //zoom in (steps > 0), zoom out (steps < 0)
  void zoom( double steps );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _AFX_ODADGNAPPVECTORIZER__INCLUDED_
