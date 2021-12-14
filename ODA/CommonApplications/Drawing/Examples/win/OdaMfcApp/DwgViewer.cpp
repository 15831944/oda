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

// DwgViewer.cpp : implementation file
//
#include "stdafx.h"

#include <winspool.h>
#include "OdRound.h"
#include "RxVariantValue.h"
#include "ColorMapping.h"
#include "DbDatabase.h"
#include "DbBlockTableRecord.h"
#include "DbLayout.h"
#include "DbLine.h"
#include "DbSymUtl.h"
#include "DbViewport.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "DbVisualStyle.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiContextualColors.h"
#include "Gi/GiSelectionStyle.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseVectorizer.h"
#include "Gs/GsModel.h"
#include "Gs/GsSelectionReactor.h"
#include "Ge/GeLineSeg2d.h"
#include "MainFrm.h"
#include "odamfcapp.h"
#include "OdaMfcAppDoc.h"
#include "DwgViewer.h"
#include "SnapshotDlg.h"
#include "OdPlatformStreamer.h"
#include "Ps/PlotStyles.h"
#include "dwgviewer.h"
#include "SnapSettings.h"
#include "DbSSet.h"
#include "DbIdMapping.h"
#include "SaveState.h"

#include <afxpriv.h>
#include <afxodlgs.h>
#include "FlatMemStream.h"
#include "DbOle2Frame.h"
#include "OleItemHandler.h"
#include ".\dwgviewer.h"
#include "ChildFrm.h"
#include "VectProps.h"
#include "winuser.h"
#include "DbPageController.h"
#include "DbHistoryManager.h"
#include "BranchesDialog.h"
#include "DwgView.h"
#include "MergeDialog.h"
#include "EditorObject.h"

typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;

extern COdaMfcAppApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDwgViewer

IMPLEMENT_DYNCREATE(CDwgViewer, CView)

CDwgViewer::CDwgViewer()
  : m_hWindowDC(0)
  , m_bRegenAbort(false)
  , m_mode(kQuiescent)
  , m_hCursor(0)
  , m_bInRegen(false)
  , m_paintMode(PaintMode_Regen)
  , m_bPlotGrayscale(false)
  , m_pagingCounter(0)
{
}

CDwgViewer::~CDwgViewer()
{
}


BEGIN_MESSAGE_MAP(CDwgViewer, CView)
  //{{AFX_MSG_MAP(CDwgViewer)
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
  ON_WM_SIZE()
  ON_WM_DESTROY()
  ON_WM_CREATE()
  ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
  ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
  ON_WM_RBUTTONUP()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEWHEEL()
  ON_COMMAND(ID_VIEW_ZOOMEXTENTS, OnViewZoomextents)
  ON_WM_MOUSEMOVE()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_MBUTTONDBLCLK()
  ON_COMMAND(ID_APPEND_LINE, OnAppendLine)
  ON_COMMAND(ID_APPEND_CIRCLE, OnAppendCircle)
  ON_COMMAND(ID_APPEND_TEXT, OnAppendText)
  ON_COMMAND(ID_REDO_LIST, OnRedoList)
  ON_COMMAND(ID_COMMIT_CHANGES, OnCommitChanges)
  ON_COMMAND(ID_UPDATE_CHANGES, OnUpdateChanges)
  ON_COMMAND(ID_PUSH_CHANGES, OnPushChanges)
  ON_COMMAND(ID_SWITCH_BRANCH, OnSwitchBranch)
  ON_COMMAND(ID_MERGE_BRANCH, OnMergeBranch)
  ON_UPDATE_COMMAND_UI(ID_REDO_LIST, OnUpdateRedoList)
  ON_UPDATE_COMMAND_UI(ID_COMMIT_CHANGES, OnUpdateCommitChanges)
  ON_UPDATE_COMMAND_UI(ID_UPDATE_CHANGES, OnUpdateUpdateChanges)
  ON_UPDATE_COMMAND_UI(ID_PUSH_CHANGES, OnUpdatePushChanges)
  ON_UPDATE_COMMAND_UI(ID_APPEND_LINE, OnUpdateAppendLine)
  ON_UPDATE_COMMAND_UI(ID_APPEND_CIRCLE, OnUpdateAppendCircle)
  ON_UPDATE_COMMAND_UI(ID_APPEND_TEXT, OnUpdateAppendText)
  ON_COMMAND(ID_VIEW_PAN, OnDrag)
  ON_UPDATE_COMMAND_UI(ID_VIEW_PAN, OnUpdateDrag)
  ON_COMMAND(ID_VIEWER_REGEN, OnViewerRegen)
  ON_COMMAND(ID_VIEWER_VPREGEN, OnViewerVpregen)
  ON_UPDATE_COMMAND_UI(ID_VIEWER_REGEN, OnUpdateViewerRegen)
  ON_UPDATE_COMMAND_UI(ID_2DWIREFRAME, OnUpdate2dwireframe)
  ON_UPDATE_COMMAND_UI(ID_3DWIREFRAME, OnUpdate3dwireframe)
  ON_UPDATE_COMMAND_UI(ID_HIDDEN, OnUpdateHidden)
  ON_UPDATE_COMMAND_UI(ID_FLAT_SHADED, OnUpdateFlatShaded)
  ON_UPDATE_COMMAND_UI(ID_FLAT_SHADED_WITH_EDGES, OnUpdateFlatShadedWithEdges)
  ON_UPDATE_COMMAND_UI(ID_GOURAUD_SHADED, OnUpdateGouraudShaded)
  ON_UPDATE_COMMAND_UI(ID_GOURAUD_SHADED_EDGES_ON, OnUpdateGouraudShadedEdgesOn)
  ON_WM_CHAR()
  ON_COMMAND(ID_VIEW_ZOOM_WINDOW, OnViewZoomWindow)
  ON_UPDATE_COMMAND_UI(ID_VIEWER_VPREGEN, OnUpdateViewerRegen)
  ON_WM_KEYDOWN()
  ON_COMMAND(ID_VIEW_3DORBIT, OnView3dorbit)
  ON_UPDATE_COMMAND_UI(ID_VIEW_3DORBIT, OnUpdateView3dorbit)
  ON_COMMAND_RANGE(ID_3DVIEWS_TOP, ID_3DVIEWS_BACK, On3DViews)
  ON_UPDATE_COMMAND_UI_RANGE(ID_3DVIEWS_TOP, ID_3DVIEWS_BACK, OnUpdateView3dorbit)
  ON_COMMAND_RANGE(ID_3DVIEWS_SW,  ID_3DVIEWS_NW,   On3DViews)
  ON_UPDATE_COMMAND_UI_RANGE(ID_3DVIEWS_SW, ID_3DVIEWS_NW, OnUpdateView3dorbit)
  ON_COMMAND(ID_VIEW_RENDER_VECTPROPS, OnViewRenderVectorizerProps)
  ON_COMMAND(ID_VIEW_RENDER_SNAPSHOT, OnViewRenderSnapshot)
  ON_COMMAND(ID_VIEW_RENDER_TAKESNAPSHOT, OnViewRenderTakeSnapshot)
  //}}AFX_MSG_MAP
  // Standard printing commands
  ON_COMMAND_RANGE(ID_2DWIREFRAME, ID_GOURAUD_SHADED_EDGES_ON, OnSetRenderMode)
  ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
  ON_COMMAND(ID_OPTIONS_SNAPPING, OnOptionsSnapping)
  ON_COMMAND(ID_REFRESH, OnRefresh)
  ON_REGISTERED_MESSAGE(g_nRedrawMSG, OnRedraw)
  ON_WM_KEYDOWN()
  ON_COMMAND(ID_VIEWER_REGEN_VISIBLE_ONLY, OnViewerRegenVisible)
  ON_UPDATE_COMMAND_UI(ID_VIEWER_REGEN_VISIBLE_ONLY, OnUpdateViewerRegenVisible)
  ON_COMMAND(ID_EDIT_RECOMPUTEDIMBLOCK, CDwgView::OnEditRecomputeDimBlock)
END_MESSAGE_MAP()

UINT CDwgViewer::g_nRedrawMSG = 0;

COdaMfcAppDoc* CDwgViewer::GetDocument()
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COdaMfcAppDoc)));
	return (COdaMfcAppDoc*)m_pDocument;
}

void CDwgViewer::exeCmd(const OdString& szCmdStr)
{
  GetDocument()->ExecuteCommand(szCmdStr);
  // #12410 : Recompute Regen after view modifications
  propagateActiveViewChanges(true);
}

/////////////////////////////////////////////////////////////////////////////
// CDwgViewer drawing

void CDwgViewer::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CDwgViewer diagnostics

#ifdef _DEBUG
void CDwgViewer::AssertValid() const
{
	CView::AssertValid();
}

void CDwgViewer::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CDwgViewer message handlers
void CDwgViewer::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
  COdaMfcAppDoc* pDoc = static_cast<COdaMfcAppDoc*>(GetDocument());
  OdDbDatabase* pDb = pDoc->m_pDb;

  OdGiContextForDbDatabase::setDatabase(pDb);
  m_hWindowDC = ::GetDC(m_hWnd);
  
  if(!g_nRedrawMSG)
    g_nRedrawMSG = ::RegisterWindowMessage(_T("OdaMfcApp::CDwgViewer::WM_REDRAW"));

  createDevice();
  if(m_pDevice.isNull())
  {
    GetParent()->PostMessage(WM_CLOSE);
    return;
  }
  m_editor.initialize(m_pDevice, static_cast<COdaMfcAppDoc*>(GetDocument())->cmdCtx() );

  setCursor(::LoadCursor(0, IDC_ARROW));
}

bool CDwgViewer::regenAbort() const
{
  // to prevent artifacts in preview (in bitmap mode)
  //if(isPlotGeneration())
    return false;

//   if(!m_bRegenAbort)
//   {
//     // this message processing is necessary to avoid GUI lock during regeneration process
//     MSG msg;
//     if(::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
//     {
//       switch(msg.message)
//       {
//       case WM_CLOSE:
//       case WM_QUIT:
//       case WM_COMMAND:
//       case WM_SYSCOMMAND:
// 
//       case WM_LBUTTONDOWN:
// //    case WM_LBUTTONUP:
//       case WM_LBUTTONDBLCLK:
//       case WM_RBUTTONDOWN:
// //    case WM_RBUTTONUP:
//       case WM_RBUTTONDBLCLK:
//       case WM_MBUTTONDOWN:
// //    case WM_MBUTTONUP:
//       case WM_MBUTTONDBLCLK:
//       case WM_MOUSEWHEEL:
//         // messages that abort regen
//         m_bRegenAbort = true;
//         break;
// 
//       case WM_MOUSEMOVE:
//         if(m_pTracker || m_mode==kGetPoint)
//         {
//           m_bRegenAbort = true;
//           break;
//         }
//       default:
//         if(msg.message==g_nRedrawMSG)
//         {
//           m_bRegenAbort = true;
//           break;
//         }
//         // message that might do not abort regen could be processed here
//         theApp.PumpMessage();
//         break;
//       }
//     }
//   }
//   return m_bRegenAbort;
}

afx_msg LRESULT CDwgViewer::OnRedraw(WPARAM wParam, LPARAM lParam)
{
  if (m_bInRegen)
    return 1;
  m_bInRegen = true;
  m_bRegenAbort = false;

  CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
  if(!regenAbort())
  {
    try
    {
      pMainFrame->StartTimer();
      if(m_pDevice.get())
      {
        setViewportBorderProperties();

        m_pDevice->update();
      }
      if(!regenAbort())
      {
        pMainFrame->StopTimer((m_paintMode == PaintMode_Regen) ? _T("Regen") : _T("Redraw"));
      }
    }
    catch(const OdError& e)
    {
      theApp.reportError(0, _T("Rendering aborted"), e);
      GetParent()->PostMessage(WM_CLOSE);
    }
    catch(const UserBreak&)
    {
      theApp.reportError(0, _T("Rendering aborted"), OdError(eUserBreak));
      GetParent()->PostMessage(WM_CLOSE);
    }
#ifndef _DEBUG
    catch(...)
    {
      theApp.reportError(0, _T("Rendering aborted"), OdError(_T("Unknown exception is caught...")));
      GetParent()->PostMessage(WM_CLOSE);
    }
#endif //#ifndef _DEBUG
  }

  m_bRegenAbort = false;
  m_bInRegen = false;
  m_paintMode = PaintMode_Redraw;
  return 1;
}

void CDwgViewer::OnPaint() 
{
  m_bRegenAbort = true;

  //Process WM_PAINT;
  PAINTSTRUCT paintStruct;
  BeginPaint(&paintStruct);
  EndPaint(&paintStruct);

  MSG msg;
  while(::PeekMessage(&msg, NULL, g_nRedrawMSG, g_nRedrawMSG, PM_REMOVE))
  {
    ;
  }
  PostMessage(g_nRedrawMSG);
}

BOOL CDwgViewer::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return TRUE;
}

void CDwgViewer::OnSize(UINT nType, int cx, int cy) 
{
  CView::OnSize(nType, cx, cy);
  if(cx && cy)
  {
    if(m_pDevice.get())
    {
      CRect rc;
      GetClientRect(rc);
      OdGsDCRect outputRect(OdGsDCPoint(rc.left, rc.bottom), OdGsDCPoint(rc.right, rc.top));
      m_pDevice->onSize(outputRect);
    }
  }
}

void CDwgViewer::OnDestroy() 
{
  COdaMfcAppDoc* pDoc = GetDocument();
  pDoc->OnCloseVectorizer( this );

  m_editor.uninitialize();

  destroyDevice();

  m_pPrinterDevice.release();

  ::ReleaseDC(m_hWnd, m_hWindowDC);
  m_hWindowDC = 0;
	CView::OnDestroy();
  ((CMainFrame*)theApp.GetMainWnd())->releaseViewerBar();
}

int CDwgViewer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
  m_dropTarget.Register(this);
  ((CMainFrame*)theApp.GetMainWnd())->createViewerBar();
  COdaMfcAppDoc* pDoc = GetDocument();
  pDoc->m_bLayoutSwitchable = true;
	return 0;
}

OdGsView* CDwgViewer::getActiveView()
{
  return m_pDevice->activeView();
}

const OdGsView* CDwgViewer::getActiveView() const
{
  return m_pDevice->activeView();
}

OdGsView* CDwgViewer::getActiveTopView()
{
  return m_editor.activeTopView();
}

const OdGsView* CDwgViewer::getActiveTopView() const
{
  return m_editor.activeTopView();
}

inline bool requireAutoRegen(OdGsView *pView)
{
  OdGsDevice *pDevice = pView->device();
  if (!pDevice)
    return false;
  OdRxDictionaryPtr pProps = pDevice->properties();
  if (!pProps.isNull())
  {
    if (pProps->has(OD_T("RegenCoef")))
    {
      return OdRxVariantValue(pProps->getAt(OD_T("RegenCoef")))->getDouble() > 1.;
    }
  }
  return false;
}

void CDwgViewer::propagateActiveViewChanges(bool bForceAutoRegen) const
{
  // @@@ probably move this functionality into GsLayoutHelper's?
  OdGsViewPtr pView = getActiveView();
  OdGsClientViewInfo viewInfo;
  pView->clientViewInfo(viewInfo);
  OdRxObjectPtr pObj = OdDbObjectId(viewInfo.viewportObjectId).openObject(OdDb::kForWrite);
  OdAbstractViewPEPtr pVp(pObj);

  if (!pVp.isNull())
  {
    OdGePoint3d ptTarget(pView->target());
    OdGeVector3d vecDir(pView->position() - ptTarget);
    OdGeVector3d vecUp(pView->upVector());
    double dFieldWidth = pView->fieldWidth(), dFieldHeight = pView->fieldHeight();
    bool bPersp = pView->isPerspective();
    double dLensLength = pView->lensLength();
    if (vecDir.isZeroLength())
    {
      vecDir = pView->viewingMatrix().inverse().getCsZAxis();
      if (vecDir.isZeroLength())
        vecDir = OdGeVector3d::kZAxis;
      else
        vecDir.normalize();
    }
    if (!pVp->target(pObj).isEqualTo(ptTarget) ||
        !pVp->direction(pObj).isEqualTo(vecDir) ||
        !pVp->upVector(pObj).isEqualTo(vecUp) ||
        !OdEqual(pVp->fieldWidth(pObj), dFieldWidth) || !OdEqual(pVp->fieldHeight(pObj), dFieldHeight) ||
        pVp->isPerspective(pObj) != bPersp || !OdEqual(pVp->lensLength(pObj), dLensLength) || bForceAutoRegen)
    {
      OdGeVector2d viewOffset;
      if (pVp->direction(pObj).isEqualTo(vecDir) &&
          pVp->upVector(pObj).isEqualTo(vecUp) &&
          !bPersp && !pVp->isPerspective(pObj))
      {
        OdGeVector3d vecX = vecUp.crossProduct(vecDir).normal();
        viewOffset = pVp->viewOffset(pObj);
        OdGePoint3d prevTarg = pVp->target(pObj) - vecX * viewOffset.x - vecUp * viewOffset.y;
        viewOffset.x = vecX.dotProduct(ptTarget - prevTarg);
        viewOffset.y = vecUp.dotProduct(ptTarget - prevTarg);
      }
      pVp->setView(pObj, ptTarget, vecDir, vecUp, dFieldWidth, dFieldHeight, bPersp, viewOffset);
      pVp->setLensLength(pObj, dLensLength);
      // Auto regen
      if (!theApp.disableAutoRegen() && requireAutoRegen(pView))
        const_cast<CDwgViewer*>(this)->OnViewerRegen();
    }
    OdDb::RenderMode rm = OdDb::k2DOptimized;
    switch (pView->mode())
    {
      case OdGsView::kWireframe:
        rm = OdDb::kWireframe;
      break;
      case OdGsView::kHiddenLine:
        rm = OdDb::kHiddenLine;
      break;
      case OdGsView::kFlatShaded:
        rm = OdDb::kFlatShaded;
      break;
      case OdGsView::kGouraudShaded:
        rm = OdDb::kGouraudShaded;
      break;
      case OdGsView::kFlatShadedWithWireframe:
        rm = OdDb::kFlatShadedWithWireframe;
      break;
      case OdGsView::kGouraudShadedWithWireframe:
        rm = OdDb::kGouraudShadedWithWireframe;
      break;
    }
    if (pVp->renderMode(pObj) != rm)
      pVp->setRenderMode(pObj, rm);
    OdDbObjectId vs(pView->visualStyle());
    if ((pVp->visualStyle(pObj) != vs) && !vs.isNull())
      pVp->setVisualStyle(pObj, pView->visualStyle());
  }
}

void CDwgViewer::OnViewZoomWindow() 
{
  exeCmd( OD_T("zoom w ") );
}

void CDwgViewer::OnViewZoomextents() 
{
  exeCmd( OD_T("zoom e ") );
}

void CDwgViewer::OnViewZoomin() 
{
  exeCmd( OD_T("zoom 2x ") );
}

void CDwgViewer::OnViewZoomout() 
{
  exeCmd( OD_T("zoom .5x ") );
}

void CDwgViewer::OnView3dorbit() 
{
  exeCmd( OD_T("3dorbit ") );
}

void CDwgViewer::OnUpdateView3dorbit(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!m_bPsOverall);
}

void CDwgViewer::On3DViews(UINT nID)
{
  OdExEditorObject::_3DViewType type;
  if (nID <= ID_3DVIEWS_BACK)
    type = (OdExEditorObject::_3DViewType)(nID - ID_3DVIEWS_TOP + OdExEditorObject::k3DViewTop);
  else
    type = (OdExEditorObject::_3DViewType)(nID - ID_3DVIEWS_SW + OdExEditorObject::k3DViewSW);
  m_editor.set3DView(type);
  OnViewZoomextents();
}

void CDwgViewer::OnViewRenderVectorizerProps()
{
  OdGsModulePtr pGsModule = ::odrxDynamicLinker()->loadModule(theApp.recentGsDevicePath());
  CVectPropsDlg vPropsDisp(m_pDevice, pGsModule);
  vPropsDisp.DoModal();
}

void CDwgViewer::OnViewRenderSnapshot()
{
  CSnapshotDlg ssDlg(m_pDevice.get(), GetSafeHwnd());
  ssDlg.DoModal();
}

void CDwgViewer::OnViewRenderTakeSnapshot()
{
  GetDocument()->ExecuteCommand(_T("ExTakeSnapshot"));
}

void CDwgViewer::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CView::OnRButtonUp(nFlags, point);
}

void CDwgViewer::OnMouseMove(UINT nFlags, CPoint point) 
{
  CView::OnMouseMove(nFlags, point);
  ::SetCursor(m_hCursor);
  if(point!=m_oldPoint)
  {
    bool refresh = false;
    m_oldPoint = point;
    switch(m_mode)
    {
    case kQuiescent:
      m_editor.OnMouseMove(nFlags, point.x, point.y);
      break;

    case kGetPoint:
      {
        OdGePoint3d pt = m_editor.toEyeToWorld(point.x, point.y);
        if(!GETBIT(m_inpOptions, OdEd::kGptNoUCS))
          if(!m_editor.toUcsToWorld(pt))
            return;

        if(!GETBIT(m_inpOptions, OdEd::kGptNoOSnap))
          m_editor.snap(pt);
        refresh = m_editor.trackPoint(pt);
      }
      break;
    }
    if(refresh || !getActiveView()->isValid() || !getActiveTopView()->isValid())
    {
      //getActiveView()->invalidate();
      PostMessage(WM_PAINT);
    }
  }
}

void CDwgViewer::OnLButtonDown(UINT nFlags, CPoint point) 
{
  CView::OnLButtonDown(nFlags, point);
  switch(m_mode)
  {

  case kQuiescent:
    if(m_editor.OnMouseLeftButtonClick(nFlags, point.x, point.y, this))
    {
      //getActiveView()->invalidate();
      PostMessage(WM_PAINT);
    }
    break;

  case kGetPoint:
    m_response.m_point = m_editor.toEyeToWorld(point.x, point.y);
    if(!GETBIT(m_inpOptions, OdEd::kGptNoUCS))
    {
      if(!m_editor.toUcsToWorld(m_response.m_point))
        break;
    }
    m_editor.snap(m_response.m_point);
    m_response.m_type = Response::kPoint;
    break;

  }
}

void CDwgViewer::OnLButtonUp(UINT nFlags, CPoint point) 
{
  CView::OnLButtonUp(nFlags, point);
  if(m_mode==kGetPoint && GetCapture()==this)
  {
    m_response.m_point = m_editor.toEyeToWorld(point.x, point.y);
    if(!GETBIT(m_inpOptions, OdEd::kGptNoUCS))
    {
      if(!m_editor.toUcsToWorld(m_response.m_point))
        return;
    }
    m_response.m_type = Response::kPoint;
    ReleaseCapture();
  }

  m_editor.Set_Entity_centers();
}

CRect CDwgViewer::viewportRect() const
{
  CRect rc;
  GetClientRect(&rc);
  return rc;
}

CRect CDwgViewer::viewRect(OdGsView* pView)
{
  OdGePoint3d ll, ur;
  pView->getViewport((OdGePoint2d&)ll, (OdGePoint2d&)ur);
  OdGeMatrix3d x = pView->screenMatrix();
  ll.transformBy(x);
  ur.transformBy(x);
  return CRect(OdRoundToLong(ll.x), OdRoundToLong(ur.y), OdRoundToLong(ur.x), OdRoundToLong(ll.y));
}

BOOL CDwgViewer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
  ScreenToClient(&pt);
  if(m_editor.OnMouseWheel(nFlags, pt.x, pt.y, zDelta))
  {
    PostMessage(WM_PAINT);
    propagateActiveViewChanges();
  }
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

inline OdGsViewPtr overallView(OdGsDevice* pDevice) 
{
  OdGsViewPtr pOverallView;
  OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(pDevice);
  if(pPSHelper.get())
    pOverallView = pPSHelper->overallView();
  return pOverallView;
}

inline OdGsViewPtr activeView(OdGsDevice* pDevice) 
{
  OdGsViewPtr pActiveView;
  OdGsLayoutHelperPtr pLHelper = OdGsLayoutHelper::cast(pDevice);
  if(pLHelper.get())
    pActiveView = pLHelper->activeView();
  return pActiveView;
}

void CDwgViewer::setViewportBorderProperties() 
{
  OdGsViewPtr pOverallView = overallView(m_pDevice);
  OdGsViewPtr pActiveView = activeView(m_pDevice);

  int n = m_pDevice->numViews();
  if(n>1)
  {
    for(int i = 0; i<n; ++i)
    {
      OdGsViewPtr pView = m_pDevice->viewAt(i);
      if((pView==pOverallView) || (OdGsPaperLayoutHelper::cast(m_pDevice).get() && (pView!=pActiveView)))
      {
        // no border
        pView->setViewportBorderVisibility(false);
      }
      else if(pView!=pActiveView)
      {
        pView->setViewportBorderVisibility(true);
        pView->setViewportBorderProperties(theApp.curPalette()[7], 2);
      }
      else
      {
        pView->setViewportBorderVisibility(true);
        pView->setViewportBorderProperties(theApp.curPalette()[7], 2);
      }
    }
  }
}

OdGiContext::PStyleType CDwgViewer::plotStyleType() const
{
  if(isPlotGeneration() ? !m_bPlotPlotstyle : !m_bShowPlotstyle)
    return kPsNone;
  return getDatabase()->getPSTYLEMODE() ? kPsByColor : kPsByName;
}

void CDwgViewer::plotStyle(OdDbStub* psNameId, OdPsPlotStyleData& plotStyleData) const
{
  OdGiContextForDbDatabase::plotStyle(psNameId, plotStyleData);
  if (m_bPlotGrayscale) // #4203 : make grayscale preview if printer doesn't support color mode
    plotStyleData.setColorPolicy(plotStyleData.colorPolicy() | 2);
}

void CDwgViewer::preparePlotstyles(const OdDbLayout* pLayout, bool bForceReload)
{
  if(m_pPlotStyleTable.get() && !bForceReload)
    return;
  else if (bForceReload)
    m_pPlotStyleTable.release();

  OdDbDatabase* pDb = GetDocument()->m_pDb;
  OdDbLayoutPtr pCurrLayout;
  if(!pLayout)
  {
    OdDbBlockTableRecordPtr pLayoutBlock = pDb->getActiveLayoutBTRId().safeOpenObject();
    pCurrLayout = pLayoutBlock->getLayoutId().safeOpenObject();
    pLayout = pCurrLayout;
  }

  m_bPlotPlotstyle = pLayout->plotPlotStyles();
  m_bShowPlotstyle = pLayout->showPlotStyles();

  if(isPlotGeneration() ? m_bPlotPlotstyle : m_bShowPlotstyle)
  {
    OdString pssFile(pLayout->getCurrentStyleSheet());
    if(!pssFile.isEmpty())
    {
      OdString testpath = pDb->appServices()->findFile(pssFile);
      if(!testpath.isEmpty())
      {
        OdStreamBufPtr pFileBuf
          = odSystemServices()->createFile(testpath);

        if(pFileBuf.get())
        {
          loadPlotStyleTable(pFileBuf);
        }
      }
    }
  }
}

OdString GetRegistryAcadProfilesKey(); // OdaMfcApp.cpp
static bool GetRegistryDWORD(HKEY key, const OdChar *subkey, const OdChar *name, DWORD &rval)
{
  bool rv = false;
  HKEY hKey;
  if (RegOpenKeyExW(key, subkey, 0, KEY_READ | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS)
  {
    DWORD dwSize = sizeof(DWORD);
    if (RegQueryValueExW(hKey, name, NULL, NULL, (LPBYTE)&rval, &dwSize) == ERROR_SUCCESS)
    {
      rv = true;
    }
    RegCloseKey(hKey);
  }
  return rv;
}
static bool GetAcadProfileRegistryDWORD(const OdChar *pSubkey, const OdChar *pName, DWORD &value)
{
  OdString subkey = GetRegistryAcadProfilesKey();
  if (!subkey.isEmpty())
  {
    if (pSubkey)
    {
      subkey += L"\\";
      subkey += pSubkey;
    }
    return GetRegistryDWORD(HKEY_CURRENT_USER, (LPCTSTR)subkey, pName, value);
  } 
  return false;
}
OdUInt32 CDwgViewer::glyphSize(GlyphType glyphType) const
{
  bool bProcessed = false;
  DWORD val;
  switch (glyphType)
  {
    case kLightGlyph:
      bProcessed = GetAcadProfileRegistryDWORD(OD_T("Light"), OD_T("Glyph size"), val);
    break;
    case kCameraGlyph:
      bProcessed = GetAcadProfileRegistryDWORD(OD_T("Dialogs\\AcCamera"), OD_T("GlyphSize"), val);
    break;
  }
  if (bProcessed)
    return (OdUInt32)val;
  return OdGiContextForDbDatabase::glyphSize(glyphType);
}
void CDwgViewer::fillContextualColors(OdGiContextualColorsImpl *pCtxColors)
{
  OdGiContextForDbDatabase::fillContextualColors(pCtxColors); // Fill by defaults first
  DWORD val;
#define SET_CTXCLR_ISOK(entry, subkey, name) \
  if (GetAcadProfileRegistryDWORD(OD_T(subkey), OD_T(name), val)) \
    pCtxColors->setContextualColor(OdGiContextualColorsImpl::entry, (ODCOLORREF)val);
#define SET_CTXCLRTINT_ISOK(entry, subkey, name) \
  if (GetAcadProfileRegistryDWORD(OD_T(subkey), OD_T(name), val)) \
    pCtxColors->setContextualColorTint(OdGiContextualColorsImpl::entry, val != 0);
  switch (pCtxColors->visualType())
  {
    case OdGiContextualColorsImpl::k2dModel:
      SET_CTXCLR_ISOK(kGridMajorLinesColor, "Drawing Window", "2D Model grid major lines color");
      SET_CTXCLR_ISOK(kGridMinorLinesColor, "Drawing Window", "2D Model grid minor lines color");
      SET_CTXCLR_ISOK(kGridAxisLinesColor, "Drawing Window", "2D Model grid axis lines color");
      SET_CTXCLRTINT_ISOK(kGridMajorLineTint, "Drawing Window", "2D Model grid major lines tint");
      SET_CTXCLRTINT_ISOK(kGridMinorLineTint, "Drawing Window", "2D Model grid minor lines tint");
      SET_CTXCLRTINT_ISOK(kGridAxisLineTint, "Drawing Window", "2D Model grid axis lines tint");
      SET_CTXCLR_ISOK(kLightGlyphsColor, "Light", "Model glyphs color");
      SET_CTXCLR_ISOK(kLightHotspotColor, "Light", "Model hotspot color");
      SET_CTXCLR_ISOK(kLightFalloffColor, "Light", "Model falloff color");
      SET_CTXCLR_ISOK(kLightStartLimitColor, "Light", "Model start color");
      SET_CTXCLR_ISOK(kLightEndLimitColor, "Light", "Model end color");
      SET_CTXCLR_ISOK(kLightShapeColor, "Light", "Model Web shape color");
      SET_CTXCLR_ISOK(kLightDistanceColor, "Light", "Model lux at distance color");
      SET_CTXCLR_ISOK(kWebMeshColor, "Light", "Model Web color");
      SET_CTXCLR_ISOK(kWebMeshMissingColor, "Light", "Model Web(missing file) color");
      SET_CTXCLR_ISOK(kCameraGlyphsColor, "Camera", "Model glyphs color");
      SET_CTXCLR_ISOK(kCameraFrustrumColor, "Camera", "Model frustrum color");
      SET_CTXCLR_ISOK(kCameraClippingColor, "Camera", "Model clipping color");
    break;
    case OdGiContextualColorsImpl::kLayout:
      SET_CTXCLR_ISOK(kGridMajorLinesColor, "Drawing Window", "Layout grid major lines color");
      SET_CTXCLR_ISOK(kGridMinorLinesColor, "Drawing Window", "Layout grid minor lines color");
      SET_CTXCLR_ISOK(kGridAxisLinesColor, "Drawing Window", "Layout grid axis lines color");
      SET_CTXCLRTINT_ISOK(kGridMajorLineTint, "Drawing Window", "Layout grid major lines tint");
      SET_CTXCLRTINT_ISOK(kGridMinorLineTint, "Drawing Window", "Layout grid minor lines tint");
      SET_CTXCLRTINT_ISOK(kGridAxisLineTint, "Drawing Window", "Layout grid axis lines tint");
      SET_CTXCLR_ISOK(kLightGlyphsColor, "Light", "Layout glyphs color");
      SET_CTXCLR_ISOK(kLightHotspotColor, "Light", "Layout hotspot color");
      SET_CTXCLR_ISOK(kLightFalloffColor, "Light", "Layout falloff color");
      SET_CTXCLR_ISOK(kLightStartLimitColor, "Light", "Layout start color");
      SET_CTXCLR_ISOK(kLightEndLimitColor, "Light", "Layout end color");
      SET_CTXCLR_ISOK(kLightShapeColor, "Light", "Layout Web shape color");
      SET_CTXCLR_ISOK(kLightDistanceColor, "Light", "Layout lux at distance color");
      SET_CTXCLR_ISOK(kWebMeshColor, "Light", "Layout Web color");
      SET_CTXCLR_ISOK(kWebMeshMissingColor, "Light", "Layout Web(missing file) color");
      SET_CTXCLR_ISOK(kCameraGlyphsColor, "Camera", "Layout glyphs color");
      SET_CTXCLR_ISOK(kCameraFrustrumColor, "Camera", "Layout frustrum color");
      SET_CTXCLR_ISOK(kCameraClippingColor, "Camera", "Layout clipping color");
    break;
    case OdGiContextualColorsImpl::k3dParallel:
      SET_CTXCLR_ISOK(kGridMajorLinesColor, "Drawing Window", "Parallel grid major lines color");
      SET_CTXCLR_ISOK(kGridMinorLinesColor, "Drawing Window", "Parallel grid minor lines color");
      SET_CTXCLR_ISOK(kGridAxisLinesColor, "Drawing Window", "Parallel grid axis lines color");
      SET_CTXCLRTINT_ISOK(kGridMajorLineTint, "Drawing Window", "Parallel grid major lines tint");
      SET_CTXCLRTINT_ISOK(kGridMinorLineTint, "Drawing Window", "Parallel grid minor lines tint");
      SET_CTXCLRTINT_ISOK(kGridAxisLineTint, "Drawing Window", "Parallel grid axis lines tint");
      SET_CTXCLR_ISOK(kLightGlyphsColor, "Light", "Parallel glyphs color");
      SET_CTXCLR_ISOK(kLightHotspotColor, "Light", "Parallel hotspot color");
      SET_CTXCLR_ISOK(kLightFalloffColor, "Light", "Parallel falloff color");
      SET_CTXCLR_ISOK(kLightStartLimitColor, "Light", "Parallel start color");
      SET_CTXCLR_ISOK(kLightEndLimitColor, "Light", "Parallel end color");
      SET_CTXCLR_ISOK(kLightShapeColor, "Light", "Parallel Web shape color");
      SET_CTXCLR_ISOK(kLightDistanceColor, "Light", "Parallel lux at distance color");
      SET_CTXCLR_ISOK(kWebMeshColor, "Light", "Parallel Web color");
      SET_CTXCLR_ISOK(kWebMeshMissingColor, "Light", "Parallel Web(missing file) color");
      SET_CTXCLR_ISOK(kCameraGlyphsColor, "Camera", "Parallel glyphs color");
      SET_CTXCLR_ISOK(kCameraFrustrumColor, "Camera", "Parallel frustrum color");
      SET_CTXCLR_ISOK(kCameraClippingColor, "Camera", "Parallel clipping color");
    break;
    case OdGiContextualColorsImpl::k3dPerspective:
      SET_CTXCLR_ISOK(kGridMajorLinesColor, "Drawing Window", "Perspective grid major lines color");
      SET_CTXCLR_ISOK(kGridMinorLinesColor, "Drawing Window", "Perspective grid minor lines color");
      SET_CTXCLR_ISOK(kGridAxisLinesColor, "Drawing Window", "Perspective grid axis lines color");
      SET_CTXCLRTINT_ISOK(kGridMajorLineTint, "Drawing Window", "Perspective grid major lines tint");
      SET_CTXCLRTINT_ISOK(kGridMinorLineTint, "Drawing Window", "Perspective grid minor lines tint");
      SET_CTXCLRTINT_ISOK(kGridAxisLineTint, "Drawing Window", "Perspective grid axis lines tint");
      SET_CTXCLR_ISOK(kLightGlyphsColor, "Light", "Perspective glyphs color");
      SET_CTXCLR_ISOK(kLightHotspotColor, "Light", "Perspective hotspot color");
      SET_CTXCLR_ISOK(kLightFalloffColor, "Light", "Perspective falloff color");
      SET_CTXCLR_ISOK(kLightStartLimitColor, "Light", "Perspective start color");
      SET_CTXCLR_ISOK(kLightEndLimitColor, "Light", "Perspective end color");
      SET_CTXCLR_ISOK(kLightShapeColor, "Light", "Perspective Web shape color");
      SET_CTXCLR_ISOK(kLightDistanceColor, "Light", "Perspective lux at distance color");
      SET_CTXCLR_ISOK(kWebMeshColor, "Light", "Perspective Web color");
      SET_CTXCLR_ISOK(kWebMeshMissingColor, "Light", "Perspective Web(missing file) color");
      SET_CTXCLR_ISOK(kCameraGlyphsColor, "Camera", "Perspective glyphs color");
      SET_CTXCLR_ISOK(kCameraFrustrumColor, "Camera", "Perspective frustrum color");
      SET_CTXCLR_ISOK(kCameraClippingColor, "Camera", "Perspective clipping color");
    break;
    case OdGiContextualColorsImpl::kBlock:
      SET_CTXCLR_ISOK(kGridMajorLinesColor, "Drawing Window", "BEdit grid major lines color");
      SET_CTXCLR_ISOK(kGridMinorLinesColor, "Drawing Window", "BEdit grid minor lines color");
      SET_CTXCLR_ISOK(kGridAxisLinesColor, "Drawing Window", "BEdit grid axis lines color");
      SET_CTXCLRTINT_ISOK(kGridMajorLineTint, "Drawing Window", "BEdit grid major lines tint");
      SET_CTXCLRTINT_ISOK(kGridMinorLineTint, "Drawing Window", "BEdit grid minor lines tint");
      SET_CTXCLRTINT_ISOK(kGridAxisLineTint, "Drawing Window", "BEdit grid axis lines tint");
      SET_CTXCLR_ISOK(kLightGlyphsColor, "Light", "BEdit glyphs color");
      SET_CTXCLR_ISOK(kLightHotspotColor, "Light", "BEdit hotspot color");
      SET_CTXCLR_ISOK(kLightFalloffColor, "Light", "BEdit falloff color");
      SET_CTXCLR_ISOK(kLightStartLimitColor, "Light", "BEdit start color");
      SET_CTXCLR_ISOK(kLightEndLimitColor, "Light", "BEdit end color");
      SET_CTXCLR_ISOK(kLightShapeColor, "Light", "BEdit Web shape color");
      SET_CTXCLR_ISOK(kLightDistanceColor, "Light", "BEdit lux at distance color");
      SET_CTXCLR_ISOK(kWebMeshColor, "Light", "BEdit Web color");
      SET_CTXCLR_ISOK(kWebMeshMissingColor, "Light", "BEdit Web(missing file) color");
    break;
    default: break;
  }
#undef SET_CTXCLRTINT_ISOK
#undef SET_CTXCLR_ISOK
}

void odExGLES2ConfigureDevice(OdRxDictionary *pProperties); // Defined in MaterialsEditor.cpp
const OdString &odExLoadGsStateSetting(); // Defined in MaterialsEditor.cpp

void CDwgViewer::createDevice(bool bRecreate)
{
  CRect rc;
  GetClientRect(&rc);
  try
  {
    OdArray<OdGsViewPtr> m_prevViews;
    OdGsModelPtr m_pModel;

    if (!bRecreate)
    {
      OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(theApp.recentGsDevicePath(), false);
      OdGsDevicePtr pDevice = pGs->createDevice();
      OdRxDictionaryPtr pProperties = pDevice->properties();
      if(pProperties.get())
      {
        if(pProperties->has(OD_T("WindowHWND"))) // Check if property is supported
          pProperties->putAt(OD_T("WindowHWND"), OdRxVariantValue((OdIntPtr)m_hWnd)); // hWnd necessary for DirectX device
        if(pProperties->has(OD_T("WindowHDC"))) // Check if property is supported
          pProperties->putAt(OD_T("WindowHDC"),  OdRxVariantValue((OdIntPtr)m_hWindowDC)); // hWindowDC necessary for Bitmap device
        if(pProperties->has(OD_T("DoubleBufferEnabled"))) // Check if property is supported
          pProperties->putAt(OD_T("DoubleBufferEnabled"), OdRxVariantValue(theApp.doubleBufferEnabled()));
        if(pProperties->has(OD_T("EnableSoftwareHLR"))) // Check if property is supported
          pProperties->putAt(OD_T("EnableSoftwareHLR"), OdRxVariantValue(theApp.useSoftwareHLR()));
        if(pProperties->has(OD_T("DiscardBackFaces"))) // Check if property is supported
          pProperties->putAt(OD_T("DiscardBackFaces"), OdRxVariantValue(theApp.discardBackFaces()));
        if(pProperties->has(OD_T("BlocksCache"))) // Check if property is supported
          pProperties->putAt(OD_T("BlocksCache"), OdRxVariantValue(theApp.blocksCacheEnabled()));
        if(pProperties->has(OD_T("EnableMultithread"))) // Check if property is supported
          pProperties->putAt(OD_T("EnableMultithread"), OdRxVariantValue(theApp.gsDeviceMultithreadEnabled()));
        if(pProperties->has(OD_T("MaxRegenThreads"))) // Check if property is supported
          pProperties->putAt(OD_T("MaxRegenThreads"), OdRxVariantValue((OdUInt16)theApp.mtRegenThreadsCount()));
        if(pProperties->has(OD_T("UseTextOut"))) // Check if property is supported
          pProperties->putAt(OD_T("UseTextOut"), OdRxVariantValue(theApp.enableTTFTextOut()));
        if(pProperties->has(OD_T("UseTTFCache"))) // Check if property is supported
          pProperties->putAt(OD_T("UseTTFCache"), OdRxVariantValue(theApp.enableTTFCache()));
        if(pProperties->has(OD_T("DynamicSubEntHlt"))) // Check if property is supported
          pProperties->putAt(OD_T("DynamicSubEntHlt"), OdRxVariantValue(theApp.enableDynamicSubEntHlt()));
        //SEA 3/21/2017: Gradient As Bitmap
        if( pProperties->has( OD_T("GradientsAsBitmap") ) )
        {
          pProperties->putAt(OD_T("GradientsAsBitmap"), OdRxVariantValue(theApp.enableGDIGradientsAsBitmap()));
        }
        // Uncomment for debug logging
        m_logger.prepareLogFiles(theApp.getTempPath());
        if (pProperties->has(OD_T("Logger"))) // Check if property is supported
          pProperties->putAt(OD_T("Logger"), OdRxVariantValue((OdIntPtr)&m_logger));
        ::odExGLES2ConfigureDevice(pProperties);
      }

      enableKeepPSLayoutHelperView(true);
      enableContextualColorsManagement(theApp.enableContextualColors());
      setTtfPolyDrawMode(theApp.enableTTFPolyDraw());
      setUseTtfTriangleCache(theApp.enableTTFTriangleCache());
      enableGsModel(theApp.useGsModel());

      m_pDevice = OdDbGsManager::setupActiveLayoutViews(pDevice, this);

      if (!odExLoadGsStateSetting().isEmpty())
      {
        if (m_pDevice->supportLayoutGsStateSaving())
        {
          OdStreamBufPtr pFile = ::odrxSystemServices()->createFile(odExLoadGsStateSetting(), Oda::kFileRead, Oda::kShareDenyWrite, Oda::kOpenExisting);
          if (!m_pDevice->restoreLayoutGsState(pFile))
            MessageBox(_T("Failed to load Gs State."), _T("Gs Error"), MB_ICONERROR);
        }
      }
    }
    else
    {
      OdGsLayoutHelperPtr pDeviceIn = m_pDevice;
      // Store current device views to keep cache alive
      for (int nView = 0; nView < pDeviceIn->numViews(); nView++)
        m_prevViews.append(pDeviceIn->viewAt(nView));
      // Store current device Gs model
      m_pModel = pDeviceIn->gsModel();
      // Detach views from exist device
      pDeviceIn->eraseAllViews();
      // Create new helper for exist device
      OdGsLayoutHelperPtr pDeviceOut = OdDbGsManager::setupActiveLayoutViews(pDeviceIn->underlyingDevice(), this);
      // Release exist helper device
      m_pDevice = pDeviceOut; // 1st release
      pDeviceIn.release(); // 2nd release
      m_editor.initialize(m_pDevice, static_cast<COdaMfcAppDoc*>(GetDocument())->cmdCtx()); // 3rd release
    }

    m_layoutId = m_pDevice->layoutId();

    const ODCOLORREF* palette = theApp.curPalette();
    ODGSPALETTE pPalCpy;
    pPalCpy.insert(pPalCpy.begin(), palette, palette+256);
    pPalCpy[0] = theApp.activeBackground();
    m_pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
    OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(m_pDevice);
    if(pPSHelper.isNull())
    {
      m_bPsOverall = false;
      m_pDevice->setBackgroundColor(pPalCpy[0]); // for model space
    }
    else
    {
      m_bPsOverall = (pPSHelper->overallView().get() == pPSHelper->activeView().get());
      m_pDevice->setBackgroundColor(ODRGB(173,174,173)); // ACAD's color for paper bg
    }    
    setPaletteBackground(theApp.activeBackground());
  
    setViewportBorderProperties();

    OdGsDCRect  gsRect(rc.left, rc.right, rc.bottom, rc.top);
    m_pDevice->onSize(gsRect);

    // Adding plotstyletable info
    preparePlotstyles(NULL, bRecreate);

    if (bRecreate)
    {
      // Call update to share cache from exist views
      m_pDevice->update();
      // Invalidate views for exist Gs model (i. e. remove unused drawables and mark view props as invalid)
      if (!m_pModel.isNull())
      {
        OdGsViewPtr *pViews = m_prevViews.asArrayPtr();
        OdUInt32 nViews = m_prevViews.size();
        for (OdUInt32 nView = 0; nView < nViews; nView++)
          m_pModel->invalidate(pViews[nView]);
      }
      // Release exist views to detach from Gs and keep released slots free.
      m_prevViews.clear();
    }
  }
  catch(const OdError& e)
  {
    destroyDevice();
    theApp.reportError(_T("Graphic System Initialization Error"), e);
  }  
}

void CDwgViewer::destroyDevice() 
{
  m_pDevice.release();
}

void CDwgViewer::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnBeginPrinting(pDC, pInfo);
}

#include "../../ExRender/BmpTilesGen.h"
void generateTiles(HDC hdc, RECT& drawrc, OdGsDevice* pBmpDevice, OdUInt32 nTileWidth, OdUInt32 nTileHeight)
{
  CRect destrc = drawrc;
  destrc.NormalizeRect();
  OdGsDCRect step(0,0,0,0);
  OdGsDCRect rc(drawrc.left, drawrc.right, drawrc.bottom, drawrc.top);
  OdUInt32 nWidth = abs(rc.m_max.x-rc.m_min.x);
  rc.m_max.x -= rc.m_min.x;
  if(rc.m_max.x < 0)
  {
    rc.m_min.x = -rc.m_max.x;
    rc.m_max.x = 0;
    step.m_min.x = nTileWidth;
  }
  else
  {
    rc.m_min.x = 0;
    step.m_max.x = nTileWidth;
  }
  OdUInt32 nHeight = abs(rc.m_max.y-rc.m_min.y);
  rc.m_max.y -= rc.m_min.y;
  if(rc.m_max.y < 0)
  {
    rc.m_min.y = -rc.m_max.y;
    rc.m_max.y = 0;
    step.m_min.y = nTileHeight;
  }
  else
  {
    rc.m_min.y = 0;
    step.m_max.y = nTileHeight;
  }
  
  OdInt32 m = nWidth / nTileWidth + (nWidth % nTileWidth ? 1 : 0);
  OdInt32 n = nHeight / nTileHeight + (nHeight % nTileHeight ? 1 : 0);
  
  BmpTilesGen tilesGen(pBmpDevice, rc);
  pBmpDevice->onSize(rc);
  
  OdGiRasterImagePtr pImg;
  
  int dx = (step.m_max.x - step.m_min.x);
  int dy = (step.m_max.y - step.m_min.y);

  int dx2 = m > 1 ? dx / abs(dx) * 8 : 0;
  int dy2 = n > 1 ? dy / abs(dy) * 8 : 0;

  BITMAPINFO bmi;
  bmi.bmiHeader.biBitCount = (WORD)24;
  bmi.bmiHeader.biWidth  = nTileWidth  + abs(dx2) * 2;
  bmi.bmiHeader.biHeight = nTileHeight;
  bmi.bmiHeader.biClrImportant = 0;
  bmi.bmiHeader.biClrUsed = 0;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biSizeImage = 0;
  bmi.bmiHeader.biXPelsPerMeter = 0;
  bmi.bmiHeader.biYPelsPerMeter = 0;

  HDC bmpDC = CreateCompatibleDC(hdc);

  SetStretchBltMode( hdc, HALFTONE );

  if(bmpDC)
  {
    void* pBuf;
    HBITMAP hBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &pBuf, NULL, 0);
    if(hBmp)
    {
      HBITMAP hOld = (HBITMAP)SelectObject(bmpDC, hBmp);
      for(OdInt32 i=0; i < m; ++i)
      {
        for(OdInt32 j=0; j < n; ++j)
        {
          int minx = rc.m_min.x + i * dx;
          int maxx = minx + dx;
          int miny = rc.m_min.y + j * dy;
          int maxy = miny + dy;
          
          // render wider then a tile area to reduce gaps in lines.
          pImg = tilesGen.regenTile(OdGsDCRect(minx - dx2, maxx + dx2, miny - dy2, maxy + dy2));
          
          pImg->scanLines((OdUInt8*)pBuf, 0, nTileHeight);
          BitBlt(hdc, destrc.left + odmin(minx, maxx), destrc.top + odmin(miny, maxy),
            nTileWidth, nTileHeight, bmpDC, abs(dx2), 0, SRCCOPY);
        }
      }
      SelectObject(bmpDC, hOld);
      DeleteObject(hBmp);
    }
    DeleteDC(bmpDC);
  }
}

void CDwgViewer::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  OdDbDatabase* pDb = getDatabase();
  // copy info from gsView to active.
  OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
  OdDbAbstractViewportDataPtr pAVD(pVpObj);
  const OdGsView* pView = getActiveView();
  if (pView)
  {
    pAVD->setView(pVpObj, pView);
  }

  setPlotGeneration(true);
  struct KeepPrevGiContextParams
  {
    OdGiContextForDbDatabase *m_pGiCtx;
    bool m_bPrevGsModelState;
    ODCOLORREF m_crPrevBkgndColor;

    KeepPrevGiContextParams(OdGiContextForDbDatabase *pGiCtx) : m_pGiCtx(pGiCtx)
    {
      m_bPrevGsModelState = pGiCtx->useGsModel();
      m_crPrevBkgndColor = pGiCtx->paletteBackground();
    }
    ~KeepPrevGiContextParams()
    {
      m_pGiCtx->enableGsModel(m_bPrevGsModelState);
      m_pGiCtx->setPaletteBackground(m_crPrevBkgndColor);
    }
  } prevGiContextParams(this);

  // To get paper size of selected printer
  //  and to get properties (scale, offset) from PlotSettings
  //  to set using them OverAll View;
  //  Note: if we want to get the same Plot View for Paper Layout as AutoCAD
  //        than pIter needs to create pseudo DC having the requisite settings & properties.
  //        Look at OnPreparePrinting() where we try to set required printer device.
  //        Otherwise CPreviewView uses settings and properties of current Printer/plotter 
  //        (see CPreviewView::OnDraw()) to draw empty page on screen.

  bool bPlotViaBMP = AfxGetApp()->GetProfileInt(_T("options"), _T("Print/Preview via bitmap device"), 1) != 0;
  bool bGDITransparency = AfxGetApp()->GetProfileInt( _T( "options" ), _T( "GDI Plot With Transparency" ), 1 ) != 0;

  if(m_pPrinterDevice.isNull())
  {
    OdGsModulePtr pGsModule = ::odrxDynamicLinker()->loadModule(theApp.recentGsDevicePath());
    if (!bPlotViaBMP && pGsModule.isNull())
      pGsModule = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
    OdGsDevicePtr pPrinterDevice;
    if(bPlotViaBMP && pGsModule.get())
    {
      pPrinterDevice = pGsModule->createBitmapDevice();
    }
    else
    {
      bPlotViaBMP = false;
      pGsModule = ::odrxDynamicLinker()->loadModule(OdWinGDIModuleName);
      if(pGsModule.get())
        pPrinterDevice = pGsModule->createDevice();
    }

    if (pPrinterDevice.get())
    {
      enableGsModel(true);
      if (!pPrinterDevice->properties().isNull())
      {
        if (pPrinterDevice->properties()->has(OD_T("EnableSoftwareHLR")))
          pPrinterDevice->properties()->putAt(OD_T("EnableSoftwareHLR"), OdRxVariantValue(theApp.useSoftwareHLR()));
        if (/*bPlotViaBMP &&*/ pPrinterDevice->properties()->has(OD_T("DPI"))) // #9633 (1)
          pPrinterDevice->properties()->putAt(OD_T("DPI"), OdRxVariantValue((OdUInt32)odmin(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY))));
        if (pPrinterDevice->properties()->has(OD_T("UseTextOut")))
          pPrinterDevice->properties()->putAt(OD_T("UseTextOut"), OdRxVariantValue(theApp.enableTTFTextOut()));
        if (pPrinterDevice->properties()->has(OD_T("UseTTFCache")))
          pPrinterDevice->properties()->putAt(OD_T("UseTTFCache"), OdRxVariantValue(theApp.enableTTFCache()));
        if (pPrinterDevice->properties()->has(OD_T("GradientsAsBitmap")))
          pPrinterDevice->properties()->putAt(OD_T("GradientsAsBitmap"), OdRxVariantValue(theApp.enableGDIGradientsAsBitmap()));
        if (pPrinterDevice->properties()->has(OD_T("GradientsAsPolys")))
          pPrinterDevice->properties()->putAt(OD_T("GradientsAsPolys"), OdRxVariantValue(theApp.enableGDIGradientsAsPolys()));
        if (pPrinterDevice->properties()->has(OD_T("GradientsAsPolysThreshold")))
          pPrinterDevice->properties()->putAt(OD_T("GradientsAsPolysThreshold"), OdRxVariantValue((OdUInt8)theApp.gdiGradientsAsPolysThreshold()));
        if( pPrinterDevice->properties()->has(OD_T("GDITransparencyMode")))
          pPrinterDevice->properties()->putAt(OD_T("GDITransparencyMode"), OdRxVariantValue((OdUInt8)(bGDITransparency ? 1 : 0)));
        if( pPrinterDevice->properties()->has(OD_T("BlocksCache"))) // Check if property is supported
          pPrinterDevice->properties()->putAt(OD_T("BlocksCache"), OdRxVariantValue(theApp.blocksCacheEnabled()));
      }
      m_pPrinterDevice = OdDbGsManager::setupActiveLayoutViews(pPrinterDevice, this);
      preparePlotstyles();

      m_pPrinterDevice->setLogicalPalette(odcmAcadLightPalette(), 256);
      m_pPrinterDevice->setBackgroundColor(ODRGB(255,255,255));
      setPaletteBackground(m_pPrinterDevice->getBackgroundColor());
    }
  }
  else
  {
    bPlotViaBMP = m_pPrinterDevice->properties()->has(OD_T("RasterImage"));
    setPaletteBackground(m_pPrinterDevice->getBackgroundColor());
  }

  if (m_pPrinterDevice.get())
  {
    // #13023 : modify 255th color on printing
    if (pInfo->m_bPreview)
      m_pPrinterDevice->setLogicalPalette(odcmAcadLightPalette(), 256);
    else
    {
      OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > printPalette;
      printPalette.insert(printPalette.end(), odcmAcadLightPalette(), odcmAcadLightPalette() + 256);
      printPalette[255] = ODRGB(250, 250, 250);
      m_pPrinterDevice->setLogicalPalette(printPalette.getPtr(), 256);
    }
    //

    bool bPrint90 = false;
    bool bPrint0 = false;
    bool bPrint180 = false;
    bool bPrint270 = false;

    // Get printer paper info
    double dPrinterWidth        = pDC->GetDeviceCaps(PHYSICALWIDTH);
    if (pInfo->m_bPreview){
      dPrinterWidth -= 2;
    }
    double dPrinterHeight       = pDC->GetDeviceCaps(PHYSICALHEIGHT);
    double dPrinterLeftMargin   = pDC->GetDeviceCaps(PHYSICALOFFSETX);
    double dPrinterTopMargin    = pDC->GetDeviceCaps(PHYSICALOFFSETY);
    double dPrinterMarginWidth  = pDC->GetDeviceCaps(HORZRES);
    double dPrinterMarginHeight = pDC->GetDeviceCaps(VERTRES);
    double dLogPixelX           = pDC->GetDeviceCaps(LOGPIXELSX);
    double dLogPixelY           = pDC->GetDeviceCaps(LOGPIXELSY);
//    double dPrinterRightMargin  = dPrinterWidth  - dPrinterMarginWidth  - dPrinterLeftMargin;
    double dPrinterBottomMargin = dPrinterHeight - dPrinterMarginHeight - dPrinterTopMargin;
    double koeffX               = dLogPixelX / kMmPerInch;
    double koeffY               = dLogPixelY / kMmPerInch;

    bool bModel = m_pPrinterDevice->isKindOf(OdGsModelLayoutHelper::desc());

    // Get Layout info
    OdDbLayoutPtr pLayout = m_pPrinterDevice->layoutId().safeOpenObject();

    bool bScaledToFit =  pLayout->useStandardScale() && (OdDbPlotSettings::kScaleToFit == pLayout->stdScaleType());
    bool bCentered    =  pLayout->plotCentered();
    bool bMetric      = (pLayout->plotPaperUnits() != OdDbPlotSettings::kInches) ? true : false;
    bool bPrintLW     =  pLayout->printLineweights() || pLayout->showPlotStyles();

    double offsetX, offsetY;
    pLayout->getPlotOrigin(offsetX, offsetY);         // in mm
    OdGePoint2d pio = pLayout->getPaperImageOrigin(); // in mm

    double dLeftMargin   = pLayout->getLeftMargin();  // in mm
    double dRightMargin  = pLayout->getRightMargin(); // in mm
    double dTopMargin    = pLayout->getTopMargin();   // in mm
    double dBottomMargin = pLayout->getBottomMargin();// in mm
    
    OdDbPlotSettings::PlotType plotType = pLayout->plotType();

    // set LineWeight scale factor for model space
    if (bPrintLW && bModel)
    {
      OdGsView* pTo = m_pPrinterDevice->viewAt(0);
      pTo->setLineweightToDcScale(odmax(dLogPixelX, dLogPixelY) / kMmPerInch * 0.01 );
    }

    // Apply paper rotation to paper parameters
    if (pInfo->m_bPreview)
    {
      double dTmp;
      switch (pLayout->plotRotation())
      {
        case OdDbPlotSettings::k90degrees:
          dTmp          = dTopMargin;
          dTopMargin    = dRightMargin;
          dRightMargin  = dBottomMargin;
          dBottomMargin = dLeftMargin;
          dLeftMargin   = dTmp;
          //std::swap(pio.x, pio.y);
          std::swap(offsetX, offsetY);
          break;
        case OdDbPlotSettings::k180degrees:
          std::swap(dBottomMargin, dTopMargin );
          std::swap(dRightMargin , dLeftMargin);
          break;
        case OdDbPlotSettings::k270degrees:
            dTmp          = dTopMargin;
            dTopMargin    = dLeftMargin;
            dLeftMargin   = dBottomMargin;
            dBottomMargin = dRightMargin;
            dRightMargin  = dTmp;
            //std::swap(pio.x, pio.y);
            std::swap(offsetX, offsetY);
          break;
      };
    }
    else // printing
    {
      OdDbPlotSettings::PlotRotation plotRotation = pLayout->plotRotation();

      if (plotRotation == OdDbPlotSettings::k90degrees || plotRotation == OdDbPlotSettings::k270degrees)
      {
        // 3962 fixed
        DWORD landOrientation = DeviceCapabilities(pInfo->m_pPD->GetDeviceName(), pInfo->m_pPD->GetPortName(), DC_ORIENTATION, 0, 0);
        if (landOrientation == 270)
          plotRotation = (plotRotation == OdDbPlotSettings::k90degrees) ? OdDbPlotSettings::k270degrees : OdDbPlotSettings::k90degrees;
      }

      switch (plotRotation)
      {
        case OdDbPlotSettings::k0degrees:
          bPrint0 = true;
        break;
        case OdDbPlotSettings::k90degrees:
          bPrint90 = true;
          std::swap(dTopMargin, dRightMargin);
          std::swap(dBottomMargin, dLeftMargin);
          std::swap(dBottomMargin, dTopMargin);
          //std::swap(pio.x, pio.y);
          std::swap(offsetX, offsetY);
          offsetY = -offsetY;
          offsetX = -offsetX;
          break;
        case OdDbPlotSettings::k180degrees:
          bPrint180 = true;
          std::swap(dRightMargin , dLeftMargin);
          offsetY = -offsetY;
          offsetX = -offsetX;
          break;
        case OdDbPlotSettings::k270degrees:
          bPrint270 = true;
          std::swap(dTopMargin, dRightMargin);
          std::swap(dBottomMargin, dLeftMargin);
          std::swap(offsetX, offsetY);
          break;
      }
    }

    // Get scale factor
    double factor;
    if (pLayout->useStandardScale())
    {
      pLayout->getStdScale(factor);
    }
    else
    {
      double numerator, denominator;
      pLayout->getCustomPrintScale(numerator, denominator);
      factor = numerator / denominator;
    }

//     if (dLeftMargin < dPrinterLeftMargin / koeffX)
//       dLeftMargin = dPrinterLeftMargin / koeffX;
//     if (dTopMargin < dPrinterTopMargin  / koeffY)
//       dTopMargin = dPrinterTopMargin  / koeffY;

    // Calculate paper drawable area using margins from layout (in mm).
    double drx1 = (- dPrinterLeftMargin / koeffX + dLeftMargin);                // in mm
    double drx2 = (drx1 + dPrinterWidth / koeffX - dLeftMargin - dRightMargin); // in mm
    double dry1 = (- dPrinterTopMargin  / koeffY + dTopMargin );                // in mm
    double dry2 = (dry1 + dPrinterHeight/ koeffY - dTopMargin - dBottomMargin); // in mm

    // Margin clip box calculation
    dTopMargin    *= koeffY ; /// in printer units
    dRightMargin  *= koeffX ;
    dBottomMargin *= koeffY ;
    dLeftMargin   *= koeffX ;

    CRgn newClipRgn; newClipRgn.CreateRectRgn(0,0,1,1);
    CRect MarginsClipBox;

    int ret = GetClipRgn(pDC->m_hDC, newClipRgn);
    bool bNullMarginsClipBox = !ret || ret && (GetLastError() != ERROR_SUCCESS);

    double dScreenFactorH, dScreenFactorW;
    if (bNullMarginsClipBox)
    { // printing way
      double x = dLeftMargin    - dPrinterLeftMargin;
      double y = dTopMargin     - dPrinterTopMargin;
      MarginsClipBox.SetRect(int(x), int(y)
                          , int(x + dPrinterWidth - dLeftMargin - dRightMargin)
                          , int(y + dPrinterHeight - dTopMargin - dBottomMargin));

      dScreenFactorH = dScreenFactorW = 1.;
    }
    else
    {
      newClipRgn.GetRgnBox(&MarginsClipBox);
      dScreenFactorH = double(MarginsClipBox.Height()) / dPrinterMarginHeight;
      dScreenFactorW = double(MarginsClipBox.Width())  / dPrinterMarginWidth;

      MarginsClipBox.left += LONG(( dLeftMargin - dPrinterLeftMargin) * dScreenFactorW);
      MarginsClipBox.bottom -= LONG(( dBottomMargin  - dPrinterBottomMargin ) * dScreenFactorH);

      MarginsClipBox.top = MarginsClipBox.bottom  - LONG(( dPrinterHeight - dTopMargin - dBottomMargin) * dScreenFactorH);
      MarginsClipBox.right  = MarginsClipBox.left + LONG(( dPrinterWidth - dLeftMargin - dRightMargin ) * dScreenFactorW);
    }

    // MarginsClipBox is calculated
    CRect ClipBox(MarginsClipBox);
  
    // Get view and viewport position, direction ...
    OdGsViewPtr pOverallView;

    OdGePoint3d viewportCenter;
    OdGePoint3d viewTarget, oldTarget;
    OdGeVector3d upV, viewDir;
    bool isPerspective;
    double viewportH, viewportW;
    OdGeMatrix3d eyeToWorld, WorldToeye;
    bool SkipClipping = false;

    OdAbstractViewPEPtr pAbstractViewPE;
    OdRxObjectPtr pVObject;

    pOverallView = bModel ? OdGsModelLayoutHelperPtr(m_pPrinterDevice)->activeView() 
                          : OdGsPaperLayoutHelperPtr(m_pPrinterDevice)->overallView();

    if (plotType == OdDbPlotSettings::kView)
    {
      OdString sPlotViewName = pLayout->getPlotViewName();
      OdDbViewTableRecordPtr pVtr =
        ((OdDbViewTablePtr)(pDb->getViewTableId().safeOpenObject()))->getAt(sPlotViewName).safeOpenObject();

      viewTarget = pVtr->target();     // in plotPaperUnits
      pAbstractViewPE = OdAbstractViewPEPtr(pVObject = pVtr);
    }
    else if (bModel)
    {
      OdDbViewportTablePtr pVPT = pDb->getViewportTableId().safeOpenObject();
      OdDbViewportTableRecordPtr pActiveVP = pVPT->getActiveViewportId().safeOpenObject();

      viewTarget = pActiveVP->target();     // in plotPaperUnits
      pAbstractViewPE = OdAbstractViewPEPtr(pVObject = pActiveVP);
    }
    else
    {
      OdDbObjectId overallVpId = pLayout->overallVportId();
      OdDbViewportPtr pActiveVP = overallVpId.safeOpenObject();

      viewTarget = pActiveVP->viewTarget();   // in plotPaperUnits
      pAbstractViewPE = OdAbstractViewPEPtr(pVObject = pActiveVP);
    }

    // get info from view, viewport .... etc
    viewportCenter = pAbstractViewPE->target(pVObject);       // in plotPaperUnits
    isPerspective  = pAbstractViewPE->isPerspective(pVObject);
    viewportH      = pAbstractViewPE->fieldHeight(pVObject);  // in plotPaperUnits
    viewportW      = pAbstractViewPE->fieldWidth(pVObject);   // in plotPaperUnits
    viewDir        = pAbstractViewPE->direction(pVObject);    // in plotPaperUnits
    upV            = pAbstractViewPE->upVector(pVObject);     // in plotPaperUnits
    eyeToWorld = pAbstractViewPE->eyeToWorld(pVObject);
    WorldToeye = pAbstractViewPE->worldToEye(pVObject);

    bool isPlanView = /*viewTarget.isEqualTo(OdGePoint3d(0,0,0)) &&*/ viewDir.normal().isEqualTo(OdGeVector3d::kZAxis);

    oldTarget = viewTarget;

    // To set OverAll View using default settings

    // get rect of drawing to view (in plotPaperUnits)
    double fieldWidth(viewportW), fieldHeight(viewportH);
    if (plotType == OdDbPlotSettings::kWindow || (plotType == OdDbPlotSettings::kLimits && isPlanView))
    {
      double xmin, ymin, xmax, ymax;
      if (plotType == OdDbPlotSettings::kWindow)
      {
        pLayout->getPlotWindowArea(xmin, ymin, xmax, ymax); // in plotPaperUnits
      }
      else
      {
        xmin = pDb->getLIMMIN().x; ymin = pDb->getLIMMIN().y;
        xmax = pDb->getLIMMAX().x; ymax = pDb->getLIMMAX().y;
      }

      fieldWidth  = xmax - xmin;
      fieldHeight = ymax - ymin;

      OdGeVector3d tmp = viewportCenter - viewTarget;
      viewTarget.set((xmin + xmax) / 2., (ymin + ymax) / 2., 0);
      //if (!bModel) // incorrect
      viewTarget.transformBy(eyeToWorld);
      viewTarget -= tmp;
    }
    else if (plotType == OdDbPlotSettings::kDisplay)
    {
      viewTarget = viewportCenter;
      fieldWidth  = viewportW;
      fieldHeight = viewportH;
    }
    else if (plotType == OdDbPlotSettings::kExtents || (plotType == OdDbPlotSettings::kLimits && !isPlanView))
    {
      OdGsDCRect rect(0, dPrinterWidth, 0, dPrinterHeight);
      m_pPrinterDevice->onSize(rect);
      OdGeBoundBlock3d extents;
      if (pAbstractViewPE->plotExtents(pVObject, extents)) // pIter also skip 'off layers'
      {
        extents.transformBy(eyeToWorld);
        viewTarget = (extents.minPoint() + extents.maxPoint().asVector()) / 2.;
        extents.transformBy(WorldToeye);
    
        fieldWidth  = fabs(extents.maxPoint().x - extents.minPoint().x);
        fieldHeight = fabs(extents.maxPoint().y - extents.minPoint().y);
      }
    }
    else if (plotType == OdDbPlotSettings::kView)
    {
      viewTarget = viewportCenter;

      fieldWidth  = viewportW;
      fieldHeight = viewportH;
    }
    else if (plotType == OdDbPlotSettings::kLayout)
    {
      SkipClipping = true; // used full paper drawing area.

      fieldWidth  = (drx2-drx1) / factor; // drx in mm -> fieldWidth in mm
      fieldHeight = (dry2-dry1) / factor;

      viewTarget.set(fieldWidth / 2. - pio.x - offsetX / factor, fieldHeight / 2. - pio.y - offsetY / factor, 0); // in mm
      if (!bMetric)
      {
        viewTarget /= kMmPerInch; // must be in plotpaper units
        fieldWidth /= kMmPerInch; 
        fieldHeight/= kMmPerInch;
      }

      offsetX = offsetY = pio.x = pio.y = 0.; // pIter was applied to viewTarget, reset pIter.
      bCentered = bScaledToFit = false;       // kLayout doesn't support pIter.
    }

    if (plotType != OdDbPlotSettings::kView) // AlexR : 3952
      viewTarget = viewTarget.orthoProject(OdGePlane(oldTarget, viewDir));

    // in plotpaper units
    pOverallView->setView(viewTarget + viewDir, viewTarget, upV, fieldWidth, fieldHeight, isPerspective ? OdGsView::kPerspective : OdGsView::kParallel);

    if (!bMetric)
    {
      fieldWidth *= kMmPerInch;
      fieldHeight *= kMmPerInch;
    }

    if (bScaledToFit) // Scale factor can be stored in layout, but preview recalculate pIter if bScaledToFit = true.
    {                 // Some times stored factor isn't correct, due to autocad isn't update pIter before saving.
      factor = odmin( (drx2 - drx1) / fieldWidth, (dry2 - dry1) / fieldHeight );
    }

    if (bCentered)    // Offset also can be incorectly saved.
    {
      offsetX = ( (drx2 - drx1) - fieldWidth  * factor ) / 2.;
      offsetY = ( (dry2 - dry1) - fieldHeight * factor ) / 2.;

      if (bPrint90 || bPrint180)
      {
        offsetY = -offsetY;
        offsetX = -offsetX;
      }
    }

    if (bPrint180 || bPrint90)
    {
      drx1 = drx2 - fieldWidth  * factor;
      dry2 = dry1 + fieldHeight * factor;
    }
    else if (bPrint0 || bPrint270)
    {
      drx2 = drx1 + fieldWidth  * factor;
      dry1 = dry2 - fieldHeight * factor;
    }
    else // preview
    {
      drx2 = drx1 + fieldWidth  * factor;
      dry1 = dry2 - fieldHeight * factor;
    }

    if (!SkipClipping)
    {
      if (bPrint180 || bPrint90)
      {
        ClipBox.left   = LONG(ClipBox.right   - (drx2-drx1)* koeffX * dScreenFactorW);
        ClipBox.bottom = LONG(ClipBox.top + (dry2-dry1)* koeffY * dScreenFactorH);
      }
      else if (bPrint0 || bPrint270)
      {
        ClipBox.right  = LONG(ClipBox.left   + (drx2-drx1)* koeffX * dScreenFactorW);
        ClipBox.top    = LONG(ClipBox.bottom - (dry2-dry1)* koeffY * dScreenFactorH);
      }
      else // preview
      {
        ClipBox.right  = LONG(ClipBox.left   + (drx2-drx1)* koeffX * dScreenFactorW);
        ClipBox.top    = LONG(ClipBox.bottom - (dry2-dry1)* koeffY * dScreenFactorH);
      }
      ClipBox.OffsetRect(int(offsetX * koeffX * dScreenFactorW), int(-offsetY * koeffY * dScreenFactorH));
    }

    pOverallView->setViewport(OdGePoint2d(0,0), OdGePoint2d(1,1));

    CRect ResultClipBox;
    ResultClipBox.IntersectRect(&MarginsClipBox, &ClipBox);

    // Apply clip region to screen
    CRgn newClip;
    newClip.CreateRectRgnIndirect(&ResultClipBox);
    pDC->SelectClipRgn(&newClip);

    // Calculate viewport rect in printer units
    long x1 = long( ( offsetX + drx1) * koeffX );
    long x2 = long( ( offsetX + drx2) * koeffX );
    long y1 = long( (-offsetY + dry1) * koeffY );
    long y2 = long( (-offsetY + dry2) * koeffY );

    OdGsDCRect viewportRect;
    if (bPrint180 || bPrint90)
    {
      //viewportRect = OdGsDCRect( x1, x2, y1, y2 );
      viewportRect = OdGsDCRect( x2, x1, y1, y2 );
    }
    else if (bPrint0 || bPrint270)
    {
      //viewportRect = OdGsDCRect( x2, x1, y2, y1 );
      viewportRect = OdGsDCRect( x1, x2, y2, y1 );
    }
    else // preview
    {
      //viewportRect = OdGsDCRect( x2, x1, y2, y1 );
      viewportRect = OdGsDCRect( x1, x2, y2, y1 );
    }

    if(!bPlotViaBMP)
    {
    m_pPrinterDevice->onSize(viewportRect);

    m_pPrinterDevice->properties()->putAt(_T("WindowHDC"), OdRxVariantValue((OdIntPtr)pDC->m_hDC));

    m_pPrinterDevice->update(0);
    }
    else
    {
      CRect rc(viewportRect.m_min.x, viewportRect.m_max.y, viewportRect.m_max.x, viewportRect.m_min.y);
      generateTiles( pDC->m_hDC, rc, m_pPrinterDevice, 1000, 1000 );
    }
  }
  else
  {
    AfxMessageBox(_T("Can't initialize GS for printing..."));
  }
  
  setPlotGeneration(false);
  CView::OnPrint(pDC, pInfo);
}

void CDwgViewer::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
  CView::OnEndPrinting(pDC, pInfo);

  m_pPrinterDevice.release();

  if (!m_pDevice.isNull()) // Restore gsView pointers assigned to database viewports
    m_pDevice->restoreGsViewDbLinkState();
}

BOOL CDwgViewer::OnPreparePrinting(CPrintInfo* pInfo) 
{
	//if (pInfo->m_bPreview)
	{
    // Get PlotInfo for active layout (Printer/Plotter name, Paper size...)
    OdDbDatabase* pDb = getDatabase();
    OdDbObjectId idLayout = OdDbBlockTableRecordPtr(pDb->getActiveLayoutBTRId().safeOpenObject())->getLayoutId();
    //OdDbObjectId idMsLayout = OdDbBlockTableRecordPtr(pDb->getModelSpaceId().safeOpenObject())->getLayoutId();

	  CWinApp* pApp = AfxGetApp();

    bool bPrinterWasSet(false);

    OdString strPrinterName(_T("Model layout"));

    //if (idLayout != idMsLayout)
    {
      OdDbLayoutPtr pLayout = idLayout.safeOpenObject();
      strPrinterName = pLayout->getPlotCfgName();

      double dPaperWidth, dPaperHeight;
      pLayout->getPlotPaperSize( dPaperWidth, dPaperHeight );
      
      OdDbPlotSettings::PlotRotation rotation = pLayout->plotRotation();

      if (rotation == OdDbPlotSettings::k90degrees || rotation == OdDbPlotSettings::k270degrees)
        std::swap(dPaperWidth, dPaperHeight);

      HANDLE hPrinter = NULL;
      HGLOBAL hDevnames = NULL;
      HGLOBAL hDevmode = NULL;
//    PRINTER_DEFAULTS printerDefault; 
                       // pointer to printer defaults structure

      // Try to open the same Printer/Plotter as was set for active layout
      if (OpenPrinter( (LPTSTR)(LPCTSTR)strPrinterName,           // pointer to printer or server name
                       &hPrinter,                                 // pointer to printer or server handle
                       NULL //&printerDefault                     // pointer to printer defaults structure
                     ))
      {
        // OK! Printer was found!

        DWORD dLevel = 2;     // A PRINTER_INFO_2 structure containing detailed information about the printer
        DWORD cbNeeded;

        // Get count of bytes retrieved for required printer info data structure
        GetPrinter( hPrinter,             // handle to printer of interest
                    dLevel,               // version of printer info data structure
                    NULL,                 // pointer to array of bytes that receives printer info structure
                    0,                    // size, in bytes, of the pPrinter buffer
                    &cbNeeded             // pointer to variable with count of bytes retrieved (or required)
                  );

        LPBYTE pBuf = new BYTE [cbNeeded];

        if (pBuf)
        {
          // Get required printer info data structure
          DWORD cbBuf = cbNeeded;
          if (GetPrinter( hPrinter,       // handle to printer of interest
                          dLevel,         // version of printer info data structure
                          pBuf,           // pointer to array of bytes that receives printer info structure
                          cbBuf,          // size, in bytes, of the pPrinter buffer
                          &cbNeeded       // pointer to variable with count of bytes retrieved (or required)
                         ))
          {
            PRINTER_INFO_2  printerInfo;
            memcpy(&printerInfo, pBuf, sizeof(PRINTER_INFO_2));

#if(WINVER >= 0x0500)
            m_bPlotGrayscale = ::DeviceCapabilities((LPTSTR)(LPCTSTR)strPrinterName, NULL, DC_COLORDEVICE, NULL, NULL) == 0;
#endif

            // To prepare DEVNAMES structure that identifies 
            //  the driver, device, and output port names of a specific printer.

            WORD wBytes = sizeof(DEVNAMES);           // number of bytes to allocate

            OdString strDriver(OD_T("WINSPOOL"));
            OdString strDevice(printerInfo.pPrinterName);
            OdString strOutput(printerInfo.pPortName);

            WORD wDriverOffset = wBytes;
            wBytes = WORD(wBytes + strDriver.getLength() + 1) * sizeof(OdChar);

            WORD wDeviceOffset = wBytes;
            wBytes = WORD(wBytes + strDevice.getLength() + 1) * sizeof(OdChar);
  
            WORD wOutputOffset = wBytes;
            wBytes = WORD(wBytes + strOutput.getLength() + 1) * sizeof(OdChar);

            hDevnames = ::GlobalAlloc(GHND, wBytes);
            LPDEVNAMES lpDevnames = (LPDEVNAMES)::GlobalLock(hDevnames);
            //LPDEVNAMES lpDevnames = (LPDEVNAMES)hDevnames;

            lpDevnames->wDriverOffset = wDriverOffset;
            lpDevnames->wDeviceOffset = wDeviceOffset;
            lpDevnames->wOutputOffset = wOutputOffset;
	          lpDevnames->wDefault = 0; // &= ~DN_DEFAULTPRN; // Without this, print dialog will reload from Defaults...

            _tcscpy( (LPTSTR)lpDevnames + lpDevnames->wDriverOffset, (LPCTSTR)strDriver );
            _tcscpy( (LPTSTR)lpDevnames + lpDevnames->wDeviceOffset, (LPCTSTR)strDevice );
            _tcscpy( (LPTSTR)lpDevnames + lpDevnames->wOutputOffset, (LPCTSTR)strOutput );

            // To prepare DEVMODE structure that specifies information 
            //  about the device initialization and environment of a printer.
            
            LONG            
              nResult = DocumentProperties(
              NULL,                 // handle of window that displays dialog box
              hPrinter,	            // handle of printer object
              (LPTSTR)(LPCTSTR)strDevice,	// address of device name
              //(LPSTR)lpDevnames + lpDevnames->wDeviceOffset
              0,	          // address of modified device mode structure
              0,
              0); 	// mode flag

            hDevmode = ::GlobalAlloc(GHND, nResult);
            LPDEVMODE lpDevmode = (LPDEVMODE)::GlobalLock(hDevmode);
            
            nResult = DocumentProperties(
              NULL,                 // handle of window that displays dialog box
              hPrinter,	            // handle of printer object
              (LPTSTR)(LPCTSTR)strDevice,	// address of device name
              //(LPSTR)lpDevnames + lpDevnames->wDeviceOffset
              lpDevmode,	          // address of modified device mode structure
              0,
              DM_OUT_BUFFER); 	// mode flag

            //*lpDevmode = *printerInfo.pDevMode;

            // To change Printer settings

            bool bPageLandscape = (dPaperWidth > dPaperHeight) ? true : false;
				    lpDevmode->dmFields |= DM_ORIENTATION;
				    lpDevmode->dmOrientation = (short) (bPageLandscape ? DMORIENT_LANDSCAPE : DMORIENT_PORTRAIT);

				    //if (m_nPaperSize != 0 && m_nPaperSize != DMPAPER_USER)
				    //{
					  //  lpDevmode->dmFields |= DM_PAPERSIZE;
					  //  lpDevmode->dmPaperSize = (WORD)m_nPaperSize;
				    //}
				    //else

					  lpDevmode->dmFields |= DM_PAPERLENGTH | DM_PAPERWIDTH | DM_PAPERSIZE;
            
            // in tenths of a millimeter
					  lpDevmode->dmPaperLength = (short) ((bPageLandscape ? dPaperWidth  : dPaperHeight) * 10.);
					  lpDevmode->dmPaperWidth =  (short) ((bPageLandscape ? dPaperHeight : dPaperWidth ) * 10.);
            lpDevmode->dmPaperSize = DMPAPER_USER;

				    nResult = DocumentProperties(
			    	                  NULL,                 // handle of window that displays dialog box
    				                  hPrinter,	            // handle of printer object
    				                  (LPTSTR)(LPCTSTR)strDevice,	// address of device name
                              //(LPSTR)lpDevnames + lpDevnames->wDeviceOffset
                              lpDevmode,	          // address of modified device mode structure
    				                  lpDevmode,
    				                  DM_IN_BUFFER|DM_OUT_BUFFER); 	// mode flag

					  //bool bSuccess = (nResult > 0);

            // To select a specific printer, and release the printer that was previously selected
            pApp->SelectPrinter(hDevnames, hDevmode);

            pInfo->m_pPD->m_pd.hDevNames = hDevnames;
            pInfo->m_pPD->m_pd.hDevMode = hDevmode;

				    pInfo->m_pPD->CreatePrinterDC();
            ODA_ASSERT(pInfo->m_pPD->m_pd.hDC != NULL);

            bPrinterWasSet = true;
          }

          if (pBuf)
          {
            delete [] pBuf;
          }
        }

        if (hDevnames)
        {
	        ::GlobalUnlock(hDevnames);
        }
        if (hDevmode)
        {
	        ::GlobalUnlock(hDevmode);
        }
        
        if (hPrinter)
        {
          ClosePrinter(hPrinter);
        }
      }
    }

    if (!bPrinterWasSet)
    {
      // WARNING MESSAGE
      CString str;
      if (strPrinterName == _T("None"))
      {
        str = _T("No plotter is assigned. Use Page Setup Dialog to assign a plotter to the current Layout.");
      }
      else
      {
        str.Format(_T("%s"), strPrinterName.c_str());
        str += _T(": This printer configuraion cannot be used. Use Page Setup Dialog to assign a plotter to the current Layout.");
      }
      ::AfxMessageBox(str);
      
      return false; // cancel printing
    }

/*    if (!bPrinterWasSet)
    {
      // WARNING MESSAGE
      CString str;
      str.Format(_T("%s"), strPrinterName.c_str());
      str += _T(": This printer configuraion cannot be used. The Default print device has been substituted.");
      ::AfxMessageBox(str);

      // To get PrinterDC
      if (pInfo->m_pPD->m_pd.hDC == NULL)
	  	{
		   // if no printer set then, get default printer DC and create DC without calling
		  //   print dialog.
  			if (!pApp->GetPrinterDeviceDefaults(&pInfo->m_pPD->m_pd))
			  {
  				// bring up dialog to alert the user they need to install a printer.
				  if (!pInfo->m_bDocObject || (pInfo->m_dwFlags & PRINTFLAG_MAYBOTHERUSER))
  					if (pApp->DoPrintDialog(pInfo->m_pPD) != IDOK)
						  return FALSE;
			  }

			  if (pInfo->m_pPD->m_pd.hDC == NULL)
			  {
  				// call CreatePrinterDC if DC was not created by above
				  if (pInfo->m_pPD->CreatePrinterDC() == NULL)
// DeleteDC
  					return FALSE;
			  }
		  }
    }
*/  

  }


	// TODO: call DoPreparePrinting to invoke the Print dialog box
	DoPreparePrinting(pInfo);
	return CView::OnPreparePrinting(pInfo);
}

void CDwgViewer::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
  CView::OnRButtonDblClk(nFlags, point);
  if(m_editor.OnMouseRightButtonDoubleClick(nFlags, point.x, point.y))
  {
    PostMessage(WM_PAINT);
    OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(m_pDevice);  
    if(pPSHelper.isNull())
      m_bPsOverall = false;
    else
      m_bPsOverall = (pPSHelper->overallView().get() == pPSHelper->activeView().get());
    propagateActiveViewChanges();
  }
}

void CDwgViewer::OnMButtonDblClk(UINT nFlags, CPoint point)
{
  if (m_mode == kQuiescent)
  {
    if (m_editor.OnMouseLeftButtonDoubleClick(nFlags, point.x, point.y))
    {
      m_pDevice->invalidate();
      PostMessage(WM_PAINT);
      OdGsPaperLayoutHelperPtr pPSHelper = OdGsPaperLayoutHelper::cast(m_pDevice);  
      if(pPSHelper.isNull())
        m_bPsOverall = false;
      else
        m_bPsOverall = (pPSHelper->overallView().get() == pPSHelper->activeView().get());
    }
  }
}

void CDwgViewer::OnAppendLine() 
{
  GetDocument()->ExecuteCommand(_T("Line"));
}

void CDwgViewer::OnAppendCircle() 
{
  GetDocument()->ExecuteCommand(_T("Circle"));
}

void CDwgViewer::OnAppendText() 
{
  GetDocument()->ExecuteCommand(_T("Text"));
}

void CDwgViewer::OnRedoList()
{
  OdDbDatabase* pDb = getDatabase();
  if (pDb->hasRedo())
    pDb->redo();
}

void CDwgViewer::OnCommitChanges()
{
  GetDocument()->ExecuteCommand(_T("COMMIT"));
}

void CDwgViewer::OnPushChanges()
{
  GetDocument()->ExecuteCommand(_T("PUSH"));
}

void CDwgViewer::OnSwitchBranch()
{
  BranchesDialog dlg(NULL);
  dlg.setDatabase(getDatabase());
  dlg.DoModal();
}

void CDwgViewer::OnMergeBranch()
{
  MergeDialog dlg(NULL);
  dlg.setDatabase(getDatabase());
  dlg.DoModal();
}

void CDwgViewer::OnUpdateChanges()
{
  GetDocument()->ExecuteCommand(_T("Pull"));
}

void CDwgViewer::OnUpdateRedoList(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(getDatabase()->hasRedo());
}

void CDwgViewer::OnUpdateCommitChanges(CCmdUI* pCmdUI)
{
  OdDbDatabase* pDb = getDatabase();
  if (!::odedRegCmds()->lookupCmd(_T("COMMIT")).isNull() ) 
  {
    pCmdUI->Enable(true);
  }
  else
  {
    pCmdUI->Enable(false);
  }
}

void CDwgViewer::OnUpdateUpdateChanges(CCmdUI* pCmdUI)
{
  OdDbDatabase* pDb = getDatabase();
  if (!::odedRegCmds()->lookupCmd(_T("PULL")).isNull())
  {
    pCmdUI->Enable(true);
  }
  else
  {
    pCmdUI->Enable(false);
  }
}

void CDwgViewer::OnUpdatePushChanges(CCmdUI* pCmdUI)
{
  OdDbDatabase* pDb = getDatabase();
  if (!::odedRegCmds()->lookupCmd(_T("PUSH")).isNull())
  {
    pCmdUI->Enable(true);
  }
  else
  {
    pCmdUI->Enable(false);
  }
}

void CDwgViewer::OnUpdateAppendLine(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(odedRegCmds()->lookupCmd(_T("Line")).get()!=0);
}
  
void CDwgViewer::OnUpdateAppendCircle(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(odedRegCmds()->lookupCmd(_T("Circle")).get()!=0);
}
  
void CDwgViewer::OnUpdateAppendText(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(odedRegCmds()->lookupCmd(_T("Text")).get()!=0);
}

void CDwgViewer::OnDrag() 
{
  exeCmd( OD_T("dolly ") );
}

void CDwgViewer::OnUpdateDrag(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_mode == kQuiescent);
}


/*
void CDwgViewer::....() 
{
  //  Sample code:
  //  setting param of current view to DB

  OdGsView* pView = getActiveView();
  OdDbDatabase* pDb = database();

  OdDbObjectId LayoutID = OdDbBlockTableRecordPtr(pDb->getActiveLayoutBTRId().safeOpenObject())->getLayoutId();
  OdDbLayoutPtr pLayout = LayoutID.safeOpenObject();
  if(pDb->getTILEMODE())
  {
    OdDbViewportTablePtr vpTabl = pDb->getViewportTableId().safeOpenObject();
    OdDbViewportTableRecordPtr tabRec = vpTabl->getActiveViewportId().safeOpenObject(OdDb::kForWrite);

    OdDbAbstractViewportDataPtr pDbViewport = OdDbAbstractViewportData::cast(tabRec);
    pDbViewport->adjustByGsView(pView, tabRec);

  }
  else
  {
    OdDbObjectId overallVpId = pLayout->overallVportId();
    OdDbObjectId activeVpId = pLayout->activeViewportId();

    if ( overallVpId != activeVpId )
      pView = m_pDevice->viewAt(1);
    OdDbViewportPtr pViewport = activeVpId.safeOpenObject(OdDb::kForWrite);
    OdDbAbstractViewportDataPtr pDbViewport = OdDbAbstractViewportData::cast(pViewport);
    pDbViewport->adjustByGsView(pView,pViewport);
  }
}

void CDwgViewer::....() 
{
  //  Sample code:
  //  Creating new Named View from current view

  OdGsView* pView = getActiveView();
  OdDbDatabase* pDb = database();

  OdDbViewTablePtr TablPtr = m_pDb->getViewTableId().safeOpenObject(OdDb::kForWrite);
  OdDbViewTableRecordPtr newView = OdDbViewTableRecord::createObject();
  newView->setName(OdString("New View"));
  TablPtr->add(newView);

  OdDbObjectId LayoutID = OdDbBlockTableRecordPtr(pDb->getActiveLayoutBTRId().safeOpenObject())->getLayoutId();
  OdDbLayoutPtr pLayout = LayoutID.safeOpenObject();
  if(pDb->getTILEMODE())
  {
    OdDbAbstractViewportDataPtr pDbViewport = OdDbAbstractViewportData::cast(newView);
    pDbViewport->adjustByGsView(pView, newView);
    newView->setIsPaperspaceView(false);
  }
  else
  {
    OdDbAbstractViewportDataPtr pDbViewport = OdDbAbstractViewportData::cast(newView);
    pDbViewport->adjustByGsView(pView,newView);
    newView->setIsPaperspaceView(true);
  }
}
*/

static const OdChar *renderModeToVisualStyle(OdGsView::RenderMode rm)
{
  // @@@TODO: create ODA-specific "FlatShaded" and "FlatShadedWithEdges" visual styles based onto "Shaded" and "ShadedWithEdges" visual styles.
  switch (rm)
  {
    case OdGsView::kWireframe: return OdDb::kszVS3DWireframe;
    case OdGsView::kHiddenLine: return OdDb::kszVSHidden;
    case OdGsView::kFlatShaded: return OdDb::kszVSShaded; // unfortunately there is no direct comparison with any correctly configured VS's. // OdDb::kszVSFlat;
    case OdGsView::kGouraudShaded: return OdDb::kszVSShaded;
    case OdGsView::kFlatShadedWithWireframe: return OdDb::kszVSShadedWithEdges; // unfortunately there is no direct comparison with any correctly configured VS's. // OdDb::kszVSFlatWithEdges;
    case OdGsView::kGouraudShadedWithWireframe: return OdDb::kszVSShadedWithEdges;
    default: return OdDb::kszVS2DWireframe;
  }
}

void CDwgViewer::OnSetRenderMode(UINT nID)
{
  ODA_ASSUME((ID_2DWIREFRAME             -ID_2DWIREFRAME) == OdGsView::k2DOptimized &&
             (ID_3DWIREFRAME             -ID_2DWIREFRAME) == OdGsView::kWireframe &&
             (ID_HIDDEN                  -ID_2DWIREFRAME) == OdGsView::kHiddenLine &&
             (ID_FLAT_SHADED             -ID_2DWIREFRAME) == OdGsView::kFlatShaded &&
             (ID_GOURAUD_SHADED          -ID_2DWIREFRAME) == OdGsView::kGouraudShaded &&
             (ID_FLAT_SHADED_WITH_EDGES  -ID_2DWIREFRAME) == OdGsView::kFlatShadedWithWireframe &&
             (ID_GOURAUD_SHADED_EDGES_ON -ID_2DWIREFRAME) == OdGsView::kGouraudShadedWithWireframe);

  OdGsView::RenderMode rm = OdGsView::RenderMode(OdGsView::k2DOptimized + (nID - ID_2DWIREFRAME));
  OdGsView* pView = getActiveView();
  if (pView->mode() != rm)
  {
    pView->setMode(rm);
    if(pView->mode() != rm)
    {
      theApp.messageBox(m_hWnd, _T("Rendering"), _T("Mode is not supported by current device."), MB_ICONWARNING);
    }
    else
    {
      if (!m_pDevice->properties().isNull() && m_pDevice->properties()->has(OD_T("UseVisualStyles")) &&
          OdRxVariantValue(m_pDevice->properties()->getAt(OD_T("UseVisualStyles")))->getBool())
        pView->setVisualStyle(OdDbDictionaryPtr(OdDbDatabasePtr(database())->getVisualStyleDictionaryId().safeOpenObject())->getAt(::renderModeToVisualStyle(rm)));
      m_pDevice->invalidate();
      PostMessage(WM_PAINT);
    }
    propagateActiveViewChanges();
  }
}

void CDwgViewer::OnViewerRegen() 
{
  m_pDevice->invalidate();
  if(m_pDevice->gsModel())
    m_pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);
  m_paintMode = PaintMode_Regen;
  PostMessage(WM_PAINT);
}

void CDwgViewer::OnViewerVpregen() 
{
  m_pDevice->invalidate();
  if(m_pDevice->gsModel())
  {
    m_pDevice->gsModel()->invalidate(getActiveView());
  }
  m_paintMode = PaintMode_Regen;
  PostMessage(WM_PAINT);
}


void CDwgViewer::OnViewerRegenVisible()
{  
  m_pDevice->invalidate();
  if(m_pDevice->gsModel())
  {
    m_pDevice->gsModel()->invalidateVisible( m_pDevice );
  }
  m_paintMode = PaintMode_Regen;
  PostMessage(WM_PAINT);
}


void CDwgViewer::OnUpdateViewerRegenVisible(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_pDevice->gsModel()!=0);
}


void CDwgViewer::OnUpdateViewerRegen(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_pDevice->gsModel()!=0);
}

void CDwgViewer::OnOptionsSnapping()
{
  CSnapSettings dlg;
  dlg.m_bSnapOn = m_editor.isSnapOn();
  dlg.m_dwSnapModes = m_editor.getSnapModes();
  if ( dlg.DoModal() == IDOK )
    m_editor.setSnapModes( dlg.m_bSnapOn != 0, dlg.m_dwSnapModes );
}

bool CDwgViewer::canClose() const
{
  if(m_mode != kQuiescent)
  {
    AfxMessageBox(_T("Can not exit while command is active."), MB_OK|MB_ICONEXCLAMATION);
    return false;
  }
  return true;
}

void CDwgViewer::putString(const OdString& string)
{
  m_sPrompt = (LPCTSTR)string;
  int n = string.reverseFind('\n');
  LPCTSTR txt = (LPCTSTR)string;
  if(n >= 0)
  {
    txt = txt + n + 1;
  }
  theApp.setStatusText(txt);
}

OdUInt32 CDwgViewer::getKeyState()
{
  return GetDocument()->getKeyState();
}

void CDwgViewer::track(OdEdInputTracker* pTracker)
{
  m_editor.setTracker(pTracker);
}

class SaveViewParams
{
protected:
  CDwgViewer*   m_pView;
  HCURSOR       m_hOldCursor;
public:
  SaveViewParams(CDwgViewer* pView, OdEdInputTracker* pTracker, HCURSOR hCursor, bool bSnap)
    : m_pView(pView)
    , m_hOldCursor(pView->cursor())
  {
    pView->track(pTracker);
    pView->setCursor(hCursor);
    if(bSnap)
      pView->m_editor.initSnapping(pView->getActiveTopView(), pTracker);
  }
  ~SaveViewParams()
  {
    m_pView->track(0);
    m_pView->setCursor(m_hOldCursor);
    m_pView->m_editor.uninitSnapping(m_pView->getActiveTopView());
  }
};

#define BLINK_CURSOR_TIMER 888
#define BLINK_CURSOR_RATE  GetCaretBlinkTime()

void CALLBACK StringTrackerTimer(HWND hWnd, UINT  nMsg, UINT  nIDTimer, DWORD dwTime);

class SaveViewParams2 : public SaveViewParams {
  bool m_bTimerSet;
public:
  SaveViewParams2(CDwgViewer* pView, OdEdStringTracker* pTracker, HCURSOR hCursor)
    : SaveViewParams(pView, pTracker, hCursor, false)
  {
    if(pTracker) {
     pTracker->setCursor(true);
     ::SetTimer(m_pView->m_hWnd, BLINK_CURSOR_TIMER, BLINK_CURSOR_RATE, (TIMERPROC)StringTrackerTimer);
      m_bTimerSet = true;
    } else
      m_bTimerSet = false;
  }
  ~SaveViewParams2() {
    if(m_bTimerSet)
      ::KillTimer(m_pView->m_hWnd, BLINK_CURSOR_TIMER);
  }
};

HCURSOR CDwgViewer::cursor() const
{
  return m_hCursor;
}

void CDwgViewer::setCursor(HCURSOR hCursor)
{
  m_hCursor = hCursor;
  ::SetCursor(hCursor);
}

OdString CDwgViewer::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{

  m_sPrompt.empty();
  OdSaveState<OdString> savePrompt(m_sPrompt);
  putString(prompt);

  OdSaveState<Mode>     saved_m_mode(m_mode, kGetString);

  m_response.m_type = Response::kNone;

  if (pTracker) {
    m_inpars.reset(true);
  }

  m_inpOptions = options;

  SaveViewParams2 svp(this, pTracker, ::LoadCursor(0, IDC_IBEAM));

  while ( theApp.PumpMessage() )
  {
    switch(m_response.m_type)
    {
    case Response::kString:
      return m_response.m_string;

    case Response::kCancel:
      throw OdEdCancel();
      break;
    }
    // Mfc idle loop for toolbars and menus process
    LONG lIdle = 0;
    while (theApp.OnIdle(lIdle++)) ;  
  }

  throw OdEdCancel();
}

OdGePoint3d CDwgViewer::getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker)
{
  m_sPrompt.empty();
  OdSaveState<OdString> savePrompt(m_sPrompt);
  putString(prompt);

  OdSaveState<Mode>     saved_m_mode(m_mode, kGetPoint);

  m_response.m_type = Response::kNone;

  m_inpOptions = options;

  SaveViewParams svp(this, pTracker, ::LoadCursor(0, IDC_CROSS), !GETBIT(options, OdEd::kGptNoOSnap));

  while ( theApp.PumpMessage() )
  {
    switch(m_response.m_type)
    {
    case Response::kPoint:
      if(GETBIT(m_inpOptions, OdEd::kGptBeginDrag))
        SetCapture();
      return m_response.m_point;

    case Response::kString:
      if(m_response.m_string.isEmpty())
        throw OdEdEmptyInput();
      throw OdEdOtherInput(m_response.m_string);

    case Response::kCancel:
      throw OdEdCancel();
    }
    // Mfc idle loop for toolbars and menus process
    LONG lIdle = 0;
    while (theApp.OnIdle(lIdle++)) ;  
  }

  throw OdEdCancel();
}

void CDwgViewer::OnRefresh()
{
  PostMessage(WM_PAINT);
}

bool CDwgViewer::beginDragCallback(const OdGePoint3d& pt)
{
  OdSaveState<Mode> saved_m_mode(m_mode, kDragDrop);
  GetDocument()->startDrag(pt);
  return true;
}

struct ReactorSort
{
  bool operator()(OdDbObjectId id1, OdDbObjectId id2)
  {
    OdDbObjectPtr o2 = id2.openObject();
    if (o2.isNull())
      return false;
    OdDbObjectIdArray r2 = o2->getPersistentReactors();
    if (r2.contains(id1))
      return true;
    return false;
  }
};

void transform_object_set(OdDbObjectIdArray& objs, OdGeMatrix3d& xform)
{
  std::sort(objs.begin(), objs.end(), ReactorSort());
  if (objs.size())
  {
    OdDbDatabase* pDb = objs[0].database();
    pDb->startTransaction();
    for (unsigned int i = 0; i < objs.size(); ++i)
    {
      OdDbEntityPtr pEnt = objs[i].safeOpenObject(OdDb::kForWrite);
      pEnt->transformBy(xform);
    }
    pDb->endTransaction();
  }
}

BOOL CDwgViewer::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
  OdSharedPtr<COdaMfcAppDoc::ClipboardData> pData = COdaMfcAppDoc::ClipboardData::get(pDataObject);
  if ( pData )
  {
    COdaMfcAppDoc* pDoc = GetDocument();
    OdDbDatabase* pDb = pDoc->m_pDb;
    pDb->startUndoRecord();

    OdGeMatrix3d xform = OdGeMatrix3d::translation(
        m_editor.toEyeToWorld(point.x, point.y) - pData->pickPoint()
      );
    
    if (m_mode == kDragDrop)
    {
      OdDbSelectionSetPtr pSSet = pDoc->selectionSet();
      OdDbEntityPtr pEnt;
      OdDbObjectIdArray objs = pSSet->objectIdArray();
      if ( ::GetKeyState(VK_CONTROL) & 0xff00 )
      {
        OdDbIdMappingPtr pIdMapping = OdDbIdMapping::createObject();
        OdDbDatabase* pHostDb = pDb;
        pHostDb->deepCloneObjects( objs, pHostDb->getActiveLayoutBTRId(), *pIdMapping );
        
        for (unsigned int i=0; i<objs.size(); ++i)
        {
          OdDbIdPair idPair(objs[i]);
          pIdMapping->compute(idPair);
          objs[i] = idPair.value();
        }
      }
      transform_object_set(objs, xform);
    }
    else
    {
      try
      {
        OdDbDatabasePtr pTmpDb = theApp.readFile( pData->tempFileName(), true, false, Oda::kShareDenyNo ); 
        pDb->insert( xform, pTmpDb );
      }
      catch( const OdError& ex)
      {
        AfxMessageBox(ex.description());
        return FALSE;
      }
    }
    return TRUE;
  }
  return CView::OnDrop(pDataObject, dropEffect, point);
}

DROPEFFECT CDwgViewer::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
  if ( m_mode == kQuiescent || m_mode == kDragDrop )
  {
    if ( COdaMfcAppDoc::ClipboardData::isAcadDataAvailable(pDataObject) )
    {
      return (::GetKeyState(VK_CONTROL)& 0xff00) ? DROPEFFECT_COPY : DROPEFFECT_MOVE;
    }
  }
  return CView::OnDragOver(pDataObject, dwKeyState, point);
}

void CDwgViewer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  switch( nChar ) 
  {
  case VK_ESCAPE:
    break;

  case VK_F5:
    PostMessage(WM_PAINT);
    break;

  case VK_DELETE:
    ((COdaMfcAppDoc*)GetDocument())->DeleteSelection(false);
    PostMessage(WM_PAINT);
    break;
  }

  CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CDwgViewer::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW|CS_OWNDC);
  return CView::PreCreateWindow(cs);
}

void CDwgViewer::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
  CView::OnUpdate(pSender, lHint, pHint);
  //if ( !m_pDevice.isNull() )
    //OnViewerRegen();
}

void CDwgViewer::OnLButtonDblClk(UINT nFlags, CPoint point)
{
  CView::OnLButtonDblClk(nFlags, point);
  /*
  if(!m_bInGetPoint && m_editor.OnMouseLeftButtonDoubleClick(nFlags, point.x, point.y))
    PostMessage(WM_PAINT);
    */
}

void CDwgViewer::OnUpdate2dwireframe(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::k2DOptimized);
}

void CDwgViewer::OnUpdate3dwireframe(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_bPsOverall);	
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kWireframe);
}

void CDwgViewer::OnUpdateHidden(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_bPsOverall);	
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kHiddenLine);
}

void CDwgViewer::OnUpdateFlatShaded(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_bPsOverall);	
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kFlatShaded);
}

void CDwgViewer::OnUpdateFlatShadedWithEdges(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_bPsOverall);	
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kFlatShadedWithWireframe);
}

void CDwgViewer::OnUpdateGouraudShaded(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_bPsOverall);	
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kGouraudShaded);
}

void CDwgViewer::OnUpdateGouraudShadedEdgesOn(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_bPsOverall);	
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kGouraudShadedWithWireframe);
}

void CDwgViewer::respond(const OdString& s)
{
  m_response.m_type = Response::kString;
  m_response.m_string = s;
}

void CDwgViewer::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  CView::OnChar(nChar, nRepCnt, nFlags);
  m_response.m_string = m_inpars.result();
  switch(nChar)
  {
  case VK_BACK:
    while(nRepCnt--)
      m_inpars.eraseChar();
    break;

  case VK_ESCAPE:
    m_response.m_type = Response::kCancel;
    m_inpars.reset(false);

    switch(m_mode)
    {
    case kQuiescent:
      if(m_editor.unselect())
        PostMessage(WM_PAINT);
      break;

    case kGetPoint:
      // no op
      break;
    }

    break;

  default:
    while(nRepCnt--)
    {
      if(!m_inpars.addChar((OdChar)nChar))
      {
        m_inpars.reset(false);
        switch(m_mode)
        {
        case kQuiescent:
          if(m_response.m_string.isEmpty())
            GetDocument()->ExecuteCommand(GetDocument()->recentCmdName());
          else
            GetDocument()->ExecuteCommand(m_response.m_string);
          return;

        case kGetPoint:
        case kGetString:
          m_response.m_type = Response::kString;
          break;
        }
      }
    }
    break;
  }

  if(m_mode==kGetString &&
     m_response.m_type != Response::kString &&
     m_inpars.result() != m_response.m_string)
  {
    if(m_editor.trackString(m_inpars.result()))
    {
      getActiveTopView()->invalidate();
      PostMessage(WM_PAINT);
    }
  }

  if(m_sPrompt.isEmpty())
    m_sPrompt = _T("command: ");
  else if(m_inpars.result().isEmpty())
    theApp.setStatusText(m_sPrompt);
  else
    theApp.setStatusText(m_inpars.result());
}

struct OdExRegenCmd : OdEdCommand
{
  OdGsLayoutHelper* m_pDevice;
  CDwgViewer* m_pView;
  const OdString groupName() const { return L"REGEN"; }
  const OdString globalName() const { return L"REGEN"; }
  virtual OdInt32 flags() const
  {
    return OdEdCommand::flags() | OdEdCommand::kNoUndoMarker;
  }
  void execute(OdEdCommandContext* pCmdCtx)
  {
    m_pView->OnViewerRegen();
  }
};

OdEdCommandPtr CDwgViewer::command(const OdString& commandName)
{
  if (commandName.iCompare(L"REGEN") == 0)
  {
    OdSmartPtr<OdExRegenCmd> c = OdRxObjectImpl<OdExRegenCmd>::createObject();
    c->m_pView = this;
    c->m_pDevice = m_pDevice;
    return c;
  }
  else
  return m_editor.command(commandName);
}

OdExEditorObject &CDwgViewer::editorObject()
{
  return m_editor;
}

const OdExEditorObject &CDwgViewer::editorObject() const
{
  return m_editor;
}

bool CDwgViewer::isModelSpaceView() const
{
  return getDatabase()->getTILEMODE();
  //return m_bPsOverall;
}

OdIntPtr CDwgViewer::drawableFilterFunctionId(OdDbStub* viewportId) const
{
  if (theApp.pagingType() == OdDb::kPage || theApp.pagingType() == OdDb::kUnload)
    return OdGiContextForDbDatabase::drawableFilterFunctionId(viewportId) | kDrawableFilterAppRangeStart;
  return OdGiContextForDbDatabase::drawableFilterFunctionId(viewportId);
}

OdUInt32 CDwgViewer::drawableFilterFunction(OdIntPtr functionId, const OdGiDrawable *pDrawable, OdUInt32 nFlags)
{
  if (theApp.pagingType() == OdDb::kPage || theApp.pagingType() == OdDb::kUnload)
  {
    //++m_pagingCounter;
    //if (m_pagingCounter > 99)
    {
      //m_pagingCounter = 0;
      getDatabase()->pageObjects();
    }
  }
  return OdGiContextForDbDatabase::drawableFilterFunction(functionId & ~kDrawableFilterAppRangeMask, pDrawable, nFlags);
}

OdUInt32 CDwgViewer::selectionStyle(OdUInt32 nStyle, OdGiSelectionStyle &selStyle) const
{
  if (nStyle > 2)
    OdGiDefaultContext::selectionStyle(0, selStyle);
  else if (nStyle > 0) // 250 115 48 | 115 250 48
  { OdCmEntityColor entColor((nStyle == 1) ? 250 : 115, (nStyle == 1) ? 115 : 250, 48);
    selStyle.setForColorMasking();
    for (OdUInt16 nBranch = 0; nBranch < 4; nBranch++)
    {
      OdGiSelectionStyle::StyleEntry &styleBranch = selStyle.styleFor(GETBIT(nBranch, 1), GETBIT(nBranch, 2));
      if (styleBranch.edgeStyle().isColorMaskingEnabled())
        styleBranch.edgeStyle().colorMask().setColor(entColor);
      if (styleBranch.faceStyle().isColorMaskingEnabled())
        styleBranch.faceStyle().colorMask().setColor(entColor);
    }
  }
  else
    OdGiContextForDbDatabase::selectionStyle(0, selStyle);
  return 4;
}

BOOL CDwgViewer::OnIdle(LONG lCount)
{
  if(!m_pDevice->isValid())
    PostMessage(WM_PAINT);
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDwgViewerLogger

void CDwgViewerLogger::prepareLogFiles(const OdString& sLogFolder)
{
  if (m_logStream.is_open())
    m_logStream.close();

  m_logStream.open((sLogFolder + L"Log.txt").c_str());
}

void CDwgViewerLogger::closeFile()
{
  if (m_logStream.is_open())
    m_logStream.close();
}

CDwgViewerLogger::~CDwgViewerLogger()
{
  closeFile();
}

//Simple logging
void CDwgViewerLogger::logMessage(const OdString& msg)
{
  if (m_logStream.is_open())
  {
    m_logStream << OdString("Message: ") << msg + OD_T("\n");
    m_logStream.flush();
  }
  OutputDebugString(OD_T("Message: ") + msg + OD_T("\n"));
}

void CDwgViewerLogger::logWarning(const OdString& wrn)
{
  if (m_logStream.is_open())
  {
    m_logStream << OdString("Warning: ") << wrn + OD_T("\n");
    m_logStream.flush();
  }
  OutputDebugString(OD_T("Warning: ") + wrn + OD_T("\n"));
}

void CDwgViewerLogger::logError(const OdString& err)
{
  if (m_logStream.is_open())
  {
    m_logStream << OdString("Error: ") << err + OD_T("\n");
    m_logStream.flush();
  }
  OutputDebugString(OD_T("Error: ") + err + OD_T("\n"));
}

//-------------------------------------------------------------------------------------------------------------------------------------------
// Blink cursor timer

bool CDwgViewer::UpdateStringTrackerCursor(void)
{
  if (m_mode==kGetString &&
      m_response.m_type != Response::kString)
  {

    if(m_editor.trackString(m_inpars.result()))
    {
      getActiveTopView()->invalidate();
      PostMessage(WM_PAINT);
      return true;
    }
  }
  return false;
}

void CALLBACK StringTrackerTimer(HWND hWnd, UINT nMsg, UINT nIDTimer, DWORD dwTime)
{
  try
  {
    CDwgViewer* pViewer = (CDwgViewer*)CWnd::FromHandle(hWnd);

    if (!pViewer->UpdateStringTrackerCursor())
      KillTimer(hWnd, nIDTimer);
  }
  catch(...)
  {
    KillTimer(hWnd, nIDTimer);
  }
}
