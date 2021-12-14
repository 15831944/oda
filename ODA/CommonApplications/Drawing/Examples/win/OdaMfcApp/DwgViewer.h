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

#if !defined(AFX_DWGVIEWER_H__E8E1831C_152A_4383_AAEF_DF47B3E97917__INCLUDED_)
#define AFX_DWGVIEWER_H__E8E1831C_152A_4383_AAEF_DF47B3E97917__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DwgViewer.h : header file
//

#include "StaticRxObject.h"

#include "Ge/GeMatrix3d.h"
#include "Gi/Gi.h"
#include "DbGsManager.h"
#include "GiContextForDbDatabase.h"
#include "DbLine.h"
#include "EditorObject.h"
#include "ExDbCommandContext.h"
#include "OdaMfcExport.h"
#include <afxole.h>

class COdaMfcAppDoc;

#include "OdLoggerBase.h"
#include <iostream>
#include <fstream>
using namespace std;
class CDwgViewerLogger : OdLoggerBase
{
public:
  void prepareLogFiles(const OdString& sLogFolder);
  void closeFile();
  virtual ~CDwgViewerLogger();

  //Simple logging
  virtual void logMessage(const OdString& msg);
  virtual void logWarning(const OdString& wrn);
  virtual void logError(const OdString& err);

private:
  ofstream m_logStream;
};

/////////////////////////////////////////////////////////////////////////////
// CDwgViewer view

class CDwgViewer : public CView
                 , public OdGiContextForDbDatabase
                 , OdEdBaseIO
                 , OdExEditorObject::OleDragCallback

{
  friend class SaveViewParams;

  void destroyDevice();
  COleDropTarget    m_dropTarget;

  OdString          m_sPrompt;
  ExEdInputParser   m_inpars;

  static UINT       g_nRedrawMSG;
  OdExEditorObject  m_editor;
  mutable bool      m_bRegenAbort;
  mutable bool      m_bInRegen; // flag to avoid reentrancy in regen, if new redraw message is received while regen is incomplete (e.g. when assert pops up)
  enum PaintMode {
    PaintMode_Redraw,
    PaintMode_Regen
  }                 m_paintMode;
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
  CDwgViewerLogger  m_logger;
  void exeCmd(const OdString& szCmdStr);
  bool beginDragCallback(const OdGePoint3d& pt);
protected:
  using CView::operator new;
  using CView::operator delete;
private:
  BOOL m_bPsOverall;

  OdDbObjectId      m_layoutId;

  bool              m_bPlotPlotstyle;
  bool              m_bShowPlotstyle;
  bool              m_bPlotGrayscale;

  OdGiContext::PStyleType plotStyleType() const;
  void plotStyle(OdDbStub* psNameId, OdPsPlotStyleData& plotStyleData) const;
protected:
#ifdef ODAMFC_EXPORT_SYMBOL
  friend OdGsLayoutHelperPtr odGetDocDevice( CDocument* pDoc );
#endif

  OdGsLayoutHelperPtr m_pDevice;
  OdGsLayoutHelperPtr m_pPrinterDevice;
  HDC                 m_hWindowDC;
  int                 m_pagingCounter;

  CRect viewportRect() const;
  static CRect viewRect(OdGsView*);

  CDwgViewer();           // protected constructor used by dynamic creation
  void preparePlotstyles(const OdDbLayout* pLayout = NULL, bool bForceReload = false);

  OdUInt32 glyphSize(GlyphType glyphType) const;
  void fillContextualColors(OdGiContextualColorsImpl *pCtxColors);

  DECLARE_DYNCREATE(CDwgViewer);

// Attributes
public:
  OdGsView* getActiveView();
  const OdGsView* getActiveView() const;
  OdGsView* getActiveTopView();
  const OdGsView* getActiveTopView() const;
  OdGsLayoutHelper *getDevice() { return m_pDevice; }
  void propagateActiveViewChanges(bool bForceAutoRegen = false) const;
  void recreateDevice() { createDevice(true); }

  void track(OdEdInputTracker* pTracker);
  void setCursor(HCURSOR hCursor);
  HCURSOR cursor() const;

  void setViewportBorderProperties();
  bool canClose() const;
  bool isGettingString() const { return m_mode!= kQuiescent; }
  const OdString& prompt() const { return m_sPrompt; }
  int inpOptions() const { return m_inpOptions; }
  void respond(const OdString& s);
  OdEdCommandPtr command(const OdString& commandName);
  OdExEditorObject &editorObject();
  const OdExEditorObject &editorObject() const;
  bool isModelSpaceView() const;

  OdIntPtr drawableFilterFunctionId(OdDbStub* viewportId) const;
  OdUInt32 drawableFilterFunction(OdIntPtr functionId, const OdGiDrawable *pDrawable, OdUInt32 nFlags);

  virtual OdUInt32 selectionStyle(OdUInt32 nStyle, OdGiSelectionStyle &selStyle) const;
  // Operations
public:
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDwgViewer)
public:
  virtual void OnInitialUpdate();
protected:
  virtual void OnDraw(CDC* pDC);      // overridden to draw this view
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

  // Implementation
  void addRef() {}
  void release() {}

  COdaMfcAppDoc* GetDocument();

protected:
  virtual ~CDwgViewer();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CDwgViewer)
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
  afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnAppendLine();
  afx_msg void OnAppendCircle();
  afx_msg void OnAppendText();
  afx_msg void OnRedoList();
  afx_msg void OnCommitChanges();
  afx_msg void OnUpdateChanges();
  afx_msg void OnPushChanges();
  afx_msg void OnSwitchBranch();
  afx_msg void OnMergeBranch();
  afx_msg void OnUpdateRedoList(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCommitChanges(CCmdUI* pCmdUI);
  afx_msg void OnUpdateUpdateChanges(CCmdUI* pCmdUI);
  afx_msg void OnUpdatePushChanges(CCmdUI* pCmdUI);
  afx_msg void OnUpdateAppendLine(CCmdUI* pCmdUI);
  afx_msg void OnUpdateAppendCircle(CCmdUI* pCmdUI);
  afx_msg void OnUpdateAppendText(CCmdUI* pCmdUI);
  afx_msg void OnDrag();
  afx_msg void OnUpdateDrag(CCmdUI* pCmdUI);
  public:
  afx_msg void OnViewerRegen();
protected:
  afx_msg void OnViewerVpregen();
  afx_msg void OnUpdateViewerRegen(CCmdUI* pCmdUI);
  afx_msg void OnUpdate2dwireframe(CCmdUI* pCmdUI);
  afx_msg void OnUpdate3dwireframe(CCmdUI* pCmdUI);
  afx_msg void OnUpdateHidden(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFlatShaded(CCmdUI* pCmdUI);
  afx_msg void OnUpdateFlatShadedWithEdges(CCmdUI* pCmdUI);
  afx_msg void OnUpdateGouraudShaded(CCmdUI* pCmdUI);
  afx_msg void OnUpdateGouraudShadedEdgesOn(CCmdUI* pCmdUI);
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnViewZoomWindow();
  afx_msg void OnView3dorbit();
  afx_msg void OnUpdateView3dorbit(CCmdUI* pCmdUI);
  afx_msg void On3DViews(UINT nID);
  afx_msg void OnViewRenderVectorizerProps();
  afx_msg void OnViewRenderSnapshot();
  afx_msg void OnViewRenderTakeSnapshot();
  //}}AFX_MSG
  afx_msg void OnSetRenderMode(UINT nID);
  afx_msg void OnRefresh();
  afx_msg void OnOptionsSnapping();
  afx_msg LRESULT OnRedraw(WPARAM wParam, LPARAM lParam);

  afx_msg void OnViewerRegenVisible();
  afx_msg void OnUpdateViewerRegenVisible(CCmdUI* pCmdUI);

  DECLARE_MESSAGE_MAP()
  void adjustDevice(OdGsDevice* pDevice);
  void createDevice(bool bRecreate = false);
  bool regenAbort() const;
public:
  OdGePoint3d getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker);
  OdString getString(const OdString& prompt, int options, OdEdStringTracker* pTracker);
  void putString(const OdString& string);
  OdUInt32 getKeyState();

  virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
  virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnInsertOleobject();
  void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
  BOOL OnIdle(LONG lCount);

  bool UpdateStringTrackerCursor(void);

protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DWGVIEWER_H__E8E1831C_152A_4383_AAEF_DF47B3E97917__INCLUDED_)
