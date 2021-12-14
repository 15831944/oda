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
// OdaDgnAppVectorizer.cpp : implementation file
//
#include "stdafx.h"

#include <winspool.h>
#include "OdRound.h"
#include "RxVariantValue.h"
#include "Gi/GiDrawableImpl.h"
#include "Gi/GiRasterImage.h"
#include "Gi/GiWorldDraw.h"
#include "Gs/Gs.h"
#include "Gs/GsBaseVectorizer.h"
#include "Gs/GsModel.h"
#include "Gs/GsSelectionReactor.h"
#include "Ge/GeLineSeg2d.h"
#include "MainFrm.h"
#include "OpenDGN_Viewer.h"
#include "OpenDGN_ViewerDoc.h"
#include "OpenDGN_ViewerVectorizer.h"
#include "OdPlatformStreamer.h"
#include "Ps/PlotStyles.h"
#include "OpenDGN_ViewerVectorizer.h"
#include "DynamicLinker.h"
#include "Gi/GiFastExtCalc.h"


#include <afxpriv.h>
#include <afxodlgs.h>
#include "ChildFrm.h"
#include "winuser.h"

#include "DgGsManager.h"

typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;

extern COdaDgnApp theApp;


/////////////////////////////////////////////////////////////////////////////
// COdaDgnAppVectorizerBase
IMPLEMENT_DYNCREATE(COdaDgnAppVectorizerBase, CView)

COdaDgnAppVectorizerBase::COdaDgnAppVectorizerBase()
{
}

COdaDgnAppVectorizerBase::~COdaDgnAppVectorizerBase()
{
}

BEGIN_MESSAGE_MAP(COdaDgnAppVectorizerBase, CView)
  //{{AFX_MSG_MAP(COdaDgnAppVectorizer)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void COdaDgnAppVectorizerBase::OnInitialUpdate() 
{
  CView::OnInitialUpdate();
  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  pDoc->OnVectorize(theApp.settings().getActiveGS());
}

COdaDgnAppDoc* COdaDgnAppVectorizerBase::GetDocument()
{
  ODA_ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COdaDgnAppDoc)));
  return (COdaDgnAppDoc*)m_pDocument;
}

void COdaDgnAppVectorizerBase::OnDraw(CDC* /*pDC*/)
{
}

BOOL COdaDgnAppVectorizerBase::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.style |= WS_MINIMIZEBOX; //~WS_VISIBLE;
  return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COdaDgnAppVectorizer

IMPLEMENT_DYNCREATE(COdaDgnAppVectorizer, CView)

COdaDgnAppVectorizer::COdaDgnAppVectorizer()
  : m_hWindowDC(0)
  , m_bRegenAbort(false)
  , m_mode(kQuiescent)
  , m_hCursor(0)
  , m_pBasePt(0)
{
}

COdaDgnAppVectorizer::~COdaDgnAppVectorizer()
{
}


BEGIN_MESSAGE_MAP(COdaDgnAppVectorizer, CView)
  //{{AFX_MSG_MAP(COdaDgnAppVectorizer)
  ON_WM_PAINT()
  ON_WM_ERASEBKGND()
  ON_WM_SIZE()
  ON_WM_DESTROY()
  ON_WM_CREATE()
  ON_WM_RBUTTONUP()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEWHEEL()
  ON_WM_MOUSEMOVE()
  ON_WM_RBUTTONDBLCLK()
  ON_WM_LBUTTONDBLCLK()
  ON_WM_CHAR()
  ON_WM_KEYDOWN()
  ON_WM_ACTIVATE()

  //ON_COMMAND(ID_REFRESH, OnRefresh)
  ON_COMMAND(ID_VECTORIZE, OnVectorize)
  ON_COMMAND(ID_VIEWER_REGEN, OnViewerRegen)
  ON_COMMAND(ID_VIEWER_VPREGEN, OnViewerVpregen)
  ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
  ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
  ON_COMMAND(ID_VIEW_ZOOMEXTENTS, OnViewZoomextents)
  ON_COMMAND(ID_VIEW_ZOOM_WINDOW, OnViewZoomWindow)

  ON_COMMAND(ID_VIEW_PAN, OnViewPan)
  ON_COMMAND(ID_VIEW_3DORBIT, OnView3dOrbit)
  ON_UPDATE_COMMAND_UI(ID_VIEW_PAN, OnUpdateViewPan)
  ON_UPDATE_COMMAND_UI(ID_VIEW_3DORBIT, OnUpdateView3dOrbit)

  ON_UPDATE_COMMAND_UI(ID_VIEWER_REGEN, OnUpdateViewerRegen)
  ON_UPDATE_COMMAND_UI(ID_2DWIREFRAME, OnUpdate2dwireframe)
  ON_UPDATE_COMMAND_UI(ID_3DWIREFRAME, OnUpdate3dwireframe)
  ON_UPDATE_COMMAND_UI(ID_HIDDEN, OnUpdateHidden)
  ON_UPDATE_COMMAND_UI(ID_FLAT_SHADED, OnUpdateFlatShaded)
  ON_UPDATE_COMMAND_UI(ID_GOURAUD_SHADED, OnUpdateGouraudShaded)
  ON_UPDATE_COMMAND_UI(ID_FLAT_SHADED_WITH_EDGES, OnUpdateFlatShadedWithEdges)
  ON_UPDATE_COMMAND_UI(ID_GOURAUD_SHADED_EDGES_ON, OnUpdateGouraudShadedEdgesOn)

  ON_COMMAND_RANGE(ID_2DWIREFRAME, ID_GOURAUD_SHADED_EDGES_ON, OnSetRenderMode)

  ON_UPDATE_COMMAND_UI(ID_VIEWER_REGEN, OnUpdateViewerRegen)

  ON_COMMAND( ID_VECTORIZE_WINDIRECTX, OnWinDirectX )
  ON_COMMAND( ID_VECTORIZE_WINGDI, OnWinGDI )
  ON_COMMAND( ID_VECTORIZE_WINOPENGL, OnWinOpenGL )

  ON_COMMAND_RANGE(ID_VIEW1, ID_VIEW8, OnViewNumber)
  ON_UPDATE_COMMAND_UI(ID_VIEW1, OnUpdateView1)
  ON_UPDATE_COMMAND_UI(ID_VIEW2, OnUpdateView2)
  ON_UPDATE_COMMAND_UI(ID_VIEW3, OnUpdateView3)
  ON_UPDATE_COMMAND_UI(ID_VIEW4, OnUpdateView4)
  ON_UPDATE_COMMAND_UI(ID_VIEW5, OnUpdateView5)
  ON_UPDATE_COMMAND_UI(ID_VIEW6, OnUpdateView6)
  ON_UPDATE_COMMAND_UI(ID_VIEW7, OnUpdateView7)
  ON_UPDATE_COMMAND_UI(ID_VIEW8, OnUpdateView8)
  ON_COMMAND( ID_VIEWS, OnReToolbar )
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

COdaDgnAppDoc* COdaDgnAppVectorizer::GetDocument()
{
  ODA_ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COdaDgnAppDoc)));
  return (COdaDgnAppDoc*)m_pDocument;
}

/////////////////////////////////////////////////////////////////////////////
// COdaDgnAppVectorizer drawing

void COdaDgnAppVectorizer::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// COdaDgnAppVectorizer diagnostics

#ifdef _DEBUG
void COdaDgnAppVectorizer::AssertValid() const
{
  CView::AssertValid();
}

void COdaDgnAppVectorizer::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}
#endif //_DEBUG


#define GDI_DEVICE_NAME OdWinGDIModuleName

/////////////////////////////////////////////////////////////////////////////
// COdaDgnAppVectorizer message handlers
void COdaDgnAppVectorizer::OnInitialUpdate() 
{
  CView::OnInitialUpdate();

  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  OdDgDatabase* pDb = pDoc->database();

  OdGiContextForDgDatabase::setDatabase(pDb);
  OdGiContextForDgDatabase::setView( pDoc->vectorizedViewId().openObject( OdDg::kForRead ) );
  m_hWindowDC = ::GetDC(m_hWnd);
  
  createDevice( pDoc->vectorizedModelId(), pDoc->vectorizedViewId() );
  if(m_pDevice.isNull())
  {
    GetParent()->PostMessage(WM_CLOSE);
    return;
  }

  pDoc->vectorizers().push_back( this );
  OnReToolbar();

  setCursor(::LoadCursor(0, IDC_ARROW));

  m_currentOperation.initialize( this );
}

bool COdaDgnAppVectorizer::regenAbort() const
{
  // to prevent artifacts in preview (in bitmap mode)
  if(isPlotGeneration())
    return false;

  if(!m_bRegenAbort)
  {
    // this message processing is necessary to avoid GUI lock during regeneration process
    MSG msg;
    if(::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
    {
      switch(msg.message)
      {
      case WM_CLOSE:
      case WM_QUIT:
      case WM_COMMAND:
      case WM_SYSCOMMAND:

      case WM_LBUTTONDOWN:
      case WM_LBUTTONUP:
      case WM_LBUTTONDBLCLK:
      case WM_RBUTTONDOWN:
      case WM_RBUTTONUP:
      case WM_RBUTTONDBLCLK:
      case WM_MBUTTONDOWN:
      case WM_MBUTTONUP:
      case WM_MBUTTONDBLCLK:
      case WM_MOUSEWHEEL:
        // messages that abort regen
        m_bRegenAbort = true;
        break;

      default:
        // message that might do not abort regen could be processed here
        theApp.PumpMessage();
        break;
      }
    }
  }
  return m_bRegenAbort;
}

void COdaDgnAppVectorizer::OnPaint() 
{
  CView::OnPaint();

  CMainFrame* pMainFrame = (CMainFrame*)theApp.GetMainWnd();
  m_bRegenAbort = false;

  try
  {
    pMainFrame->StartTimer();
    if ( m_pDevice.get() )
    {
      setViewportBorderProperties();
      m_pDevice->update();
    }
    pMainFrame->StopTimer(_T("Regen"));
  }
  catch(const OdError& e)
  {
    theApp.reportError(0, _T("Rendering aborted"), e);
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

BOOL COdaDgnAppVectorizer::OnEraseBkgnd(CDC* /*pDC*/) 
{
  return TRUE;
}

void COdaDgnAppVectorizer::OnSize(UINT nType, int cx, int cy) 
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

void COdaDgnAppVectorizer::OnDestroy() 
{

  destroyDevice();

  ::ReleaseDC(m_hWnd, m_hWindowDC);
  m_hWindowDC = 0;
  CView::OnDestroy();
}

int COdaDgnAppVectorizer::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
  if (CView::OnCreate(lpCreateStruct) == -1)
    return -1;
  m_dropTarget.Register(this);

  CFrameWnd* pFrameWnd = GetParentFrame();

  if (!m_toolBar.CreateEx( pFrameWnd, 
                     TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) 
                    ||
                     !m_toolBar.LoadToolBar(IDR_VECTORIZER) )
  {
    TRACE0("Failed to create vectorizer toolbar\n");
    return 0;      // fail to create
  }

  m_toolBar.EnableDocking( CBRS_ALIGN_ANY );
  pFrameWnd->EnableDocking( CBRS_ALIGN_ANY );
  pFrameWnd->DockControlBar( &m_toolBar );

  return 0;
}

OdGsView* COdaDgnAppVectorizer::getActiveView()
{
  return m_pDevice->activeView();
}

const OdGsView* COdaDgnAppVectorizer::getActiveView() const
{
  return m_pDevice->activeView();
}

void COdaDgnAppVectorizer::OnViewZoomWindow() 
{
}

void COdaDgnAppVectorizer::OnViewZoomextents() 
{
  OdGsViewPtr view = getActiveView();

  //it is not calculated yet for perspective cameras
  if( view->isPerspective() )
  {
    return;
  }

  //get active pModel
  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  OdDgDatabase* pDb = pDoc->database();
  OdDgModelPtr pModel = pDb->getActiveModelId().safeOpenObject();

  //get the extents of that pModel
  OdGeExtents3d extents;
  pModel->getGeomExtents( extents );
  if( !extents.isValidExtents() )
  {
    return;
  }

  //get all points
  OdGePoint3d realPoints[ 8 ];
  { 
    OdGePoint3d min = extents.minPoint(), max = extents.maxPoint();

    realPoints[ 0 ] = min;
    realPoints[ 1 ] = max;
    realPoints[ 2 ] = OdGePoint3d( min.x, max.y, min.z );
    realPoints[ 3 ] = OdGePoint3d( max.x, max.y, min.z );
    realPoints[ 4 ] = OdGePoint3d( max.x, min.y, min.z );
    realPoints[ 5 ] = OdGePoint3d( min.x, min.y, max.z );
    realPoints[ 6 ] = OdGePoint3d( min.x, max.y, max.z );
    realPoints[ 7 ] = OdGePoint3d( max.x, min.y, max.z );
  }

  //get 'relative' points
  OdGePoint2d   relativePoints[ 8 ];
  OdGePoint3d   position = view->position(), target = view->target();
  OdGeVector3d  n = view->upVector(), m = ( target - position ).crossProduct( n ).normal();
  {
    int         i;

    for( i = 0; i < 8; i++ )
    {
      relativePoints[ i ].x = ( realPoints[ i ] - position ).dotProduct( m );
      relativePoints[ i ].y = ( realPoints[ i ] - position ).dotProduct( n );
    }
  }

  //get characteristic points
  OdGeVector2d min, medium, max;
  {
    int i;

    max.x = relativePoints[ 0 ].x;
    max.y = relativePoints[ 0 ].y;

    min = max;

    for( i = 0; i < 8; i++ )
    {
      if( min.x > relativePoints[ i ].x ) 
      {
        min.x = relativePoints[ i ].x;
      }
      if( max.x < relativePoints[ i ].x )
      {
        max.x = relativePoints[ i ].x;
      }
      if( min.y > relativePoints[ i ].y ) 
      {
        min.y = relativePoints[ i ].y;
      }
      if( max.y < relativePoints[ i ].y )
      {
        max.y = relativePoints[ i ].y;
      }
    }

    medium = ( max + min ) / 2.;
  }

  //shift the camera (if new size is not zero; it is prohibited by Ge library)
  if( min.x < max.x || min.y < max.y )
  {
    view->setView(  position + m * medium.x + n * medium.y, target + m * medium.x + n * medium.y, n,
                    ( max.x - min.x ) * 1.1, ( max.y - min.y ) * 1.1 );

    for( int i = 0; i < m_pDevice->numViews(); i++ )
    {
      OdGsView* pXRefView = m_pDevice->viewAt(i);

      if( pXRefView == view )
      {
        continue;
      }

      OdGsView::RenderMode renderMode = pXRefView->mode();
      view->viewParameters( pXRefView );
      pXRefView->setMode( renderMode );
    }

    PostMessage( WM_PAINT );
  }
}

void COdaDgnAppVectorizer::OnViewZoomin() 
{
  zoom( 1. );
}

void COdaDgnAppVectorizer::OnViewZoomout() 
{
  zoom( -1. );
}

void COdaDgnAppVectorizer::OnView3dOrbit() 
{
  if( m_currentOperation.getType() == m_currentOperation.k3dOrbit )
  {
    m_currentOperation.setType( m_currentOperation.kNone );
  }
  else
  {
    m_currentOperation.setType( m_currentOperation.k3dOrbit );
  }
}

void COdaDgnAppVectorizer::OnUpdateView3dOrbit(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( m_currentOperation.getType() == m_currentOperation.k3dOrbit );
}

void COdaDgnAppVectorizer::OnViewPan()
{
  if( m_currentOperation.getType() == m_currentOperation.kPan )
  {
    m_currentOperation.setType( m_currentOperation.kNone );
  }
  else
  {
    m_currentOperation.setType( m_currentOperation.kPan );
  }
}

void COdaDgnAppVectorizer::OnUpdateViewPan(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck( m_currentOperation.getType() == m_currentOperation.kPan );
}

void COdaDgnAppVectorizer::OnRButtonUp(UINT nFlags, CPoint point) 
{
  CView::OnRButtonUp(nFlags, point);
}

void COdaDgnAppVectorizer::OnMouseMove(UINT nFlags, CPoint point) 
{
  CView::OnMouseMove(nFlags, point);
  
  if( nFlags & MK_LBUTTON )
  {
    m_currentOperation.drawMarker( point );
  }
  else
  {
    //they could press the left button inside, move cursor out, unhold the button and then return the cursor inside
    m_currentOperation.undoDrawnMarker();
  }
}

void COdaDgnAppVectorizer::OnLButtonDown(UINT nFlags, CPoint point) 
{
  CView::OnLButtonDown(nFlags, point);

  m_currentOperation.setStartPoint( point );
  m_currentOperation.drawMarker( point );
}

void COdaDgnAppVectorizer::OnLButtonUp(UINT nFlags, CPoint point) 
{
  CView::OnLButtonUp(nFlags, point);

  CurrentOperation::Type operation = m_currentOperation.getType();
  if( operation == CurrentOperation::kNone )
  {
    return;
  }
  m_currentOperation.undoDrawnMarker();

  OdGsView* view = getActiveView();
  OdGeVector3d upVector = view->upVector();
  OdGePoint3d position = view->position(), target = view->target();
  double width = view->fieldWidth(), height = view->fieldHeight();
  bool isPerspective = view->isPerspective();

  switch( operation )
  {
  case CurrentOperation::kPan :
    {
      OdGePoint2d s1, s2 = s1 + m_currentOperation.getResultOffset();
      OdGeVector3d shift = getWorldCoordinateByScreen( s1 ) - getWorldCoordinateByScreen( s2 );

      //update the parts
      position += shift;
      target += shift;
    }
    break;
  case CurrentOperation::k3dOrbit :
    {
      //get a matrix of transformation between coordinate systems
      OdGeMatrix3d transformationToCamera;
      OdGeMatrix3d fullMatrix;
      {
        OdGeVector3d direction = target - position;
        transformationToCamera.setCoordSystem( position, direction.crossProduct( upVector ), upVector, -direction );
      }

      //rotate
      fullMatrix = transformationToCamera * m_currentOperation.getResultRotation().transpose();

      //update the parts
      upVector = fullMatrix * OdGeVector3d::kYAxis;
      position = fullMatrix * OdGePoint3d::kOrigin;
      target = fullMatrix * OdGePoint3d( 0., 0., -1. );
    }
    break;
  }

  view->setView( position, target, upVector, width, height, isPerspective ? OdGsView::kPerspective : OdGsView::kParallel  );

  for( int i = 0; i < m_pDevice->numViews(); i++ )
  {
    OdGsView* pXRefView = m_pDevice->viewAt(i);

    if( pXRefView == view )
    {
      continue;
    }

    OdGsView::RenderMode renderMode = pXRefView->mode();
    view->viewParameters( pXRefView );
    pXRefView->setMode( renderMode );
  }

  OnPaint();
}

CRect COdaDgnAppVectorizer::viewportRect() const
{
  CRect rc;
  GetClientRect(&rc);
  return rc;
}

CRect COdaDgnAppVectorizer::viewRect(OdGsView* pView)
{
  OdGePoint3d ll, ur;
  pView->getViewport((OdGePoint2d&)ll, (OdGePoint2d&)ur);
  OdGeMatrix3d x = pView->screenMatrix();
  ll.transformBy(x);
  ur.transformBy(x);
  return CRect(OdRoundToLong(ll.x), OdRoundToLong(ur.y), OdRoundToLong(ur.x), OdRoundToLong(ll.y));
}

void COdaDgnAppVectorizer::dolly(int x, int y) 
{
  OdGsView* pView = getActiveView();
  OdGeVector3d vec(-x, -y, 0.0);
  vec.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
  pView->dolly(vec);
}

BOOL COdaDgnAppVectorizer::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
  ScreenToClient(&pt);
  OdGePoint2d correctScreenPoint( pt.x, pt.y );

  OdGePoint3d worldPoint = getWorldCoordinateByScreen( correctScreenPoint );
  zoom( zDelta / WHEEL_DELTA );
  
  //shift back
  {
    OdGsView* view = getActiveView();

    //get new screen point of the same world point
    OdGePoint3d newScreenPoint;
    {
      OdGeMatrix3d worldToDevice = view->worldToDeviceMatrix();
      newScreenPoint = worldToDevice* worldPoint;
    }

    //get world points on the focal plane
    OdGePoint3d
      movingPoint = getWorldCoordinateByScreen( OdGePoint2d( newScreenPoint.x, newScreenPoint.y ) ),
      destinationPoint = getWorldCoordinateByScreen( correctScreenPoint );  

    OdGePoint3d position = view->position(), target = view->target();
    OdGeVector3d upVector = view->upVector();
    double width = view->fieldWidth(), height = view->fieldHeight();
    bool isPerspective = view->isPerspective();

    //shift the camera so points coincide
    OdGeVector3d offset = destinationPoint - movingPoint;
    position -= offset;
    target -= offset;

    view->setView( position, target, upVector, width, height, isPerspective ? OdGsView::kPerspective : OdGsView::kParallel );

    for( int i = 0; i < m_pDevice->numViews(); i++ )
    {
      OdGsView* pXRefView = m_pDevice->viewAt(i);

      if( pXRefView == view )
      {
        continue;
      }

      OdGsView::RenderMode renderMode = pXRefView->mode();
      view->viewParameters( pXRefView );
      pXRefView->setMode( renderMode );
    }
  }

  return CView::OnMouseWheel(nFlags, zDelta, pt);
}

inline OdGsViewPtr activeView(OdGsDevice* pDevice) 
{
  OdGsViewPtr pActiveView;
  OdGsDeviceForDgModelPtr pLHelper = OdGsDeviceForDgModel::cast(pDevice);
  if(pLHelper.get())
    pActiveView = pLHelper->activeView();
  return pActiveView;
}

void COdaDgnAppVectorizer::setViewportBorderProperties() 
{
  OdGsViewPtr pActiveView = activeView(m_pDevice);
  int n = m_pDevice->numViews();
  if(n>1)
  {
    for(int i = 0; i<n; ++i)
    {
      OdGsViewPtr pView = m_pDevice->viewAt(i);
      pView->setViewportBorderVisibility(true);
    }
  }
}

OdGiContext::PStyleType COdaDgnAppVectorizer::plotStyleType() const
{
  return kPsNone;
}

void COdaDgnAppVectorizer::createDevice( const OdDgElementId& modelId, const OdDgElementId& viewId )
{
  CRect rc;
  GetClientRect(&rc);
  try
  {
    OdString str = theApp.settings().getGsDevice();
    if (str.isEmpty())
      return;

    OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(str, false);
    OdGsDevicePtr pDevice = pGs->createDevice();
    OdRxDictionaryPtr pProperties = pDevice->properties();
    if(pProperties.get())
    {
      if(pProperties->has(OD_T("WindowHWND")))
        pProperties->putAt(OD_T("WindowHWND"), OdRxVariantValue((OdIntPtr)m_hWnd)); // hWnd necessary for DirectX device
      if(pProperties->has(OD_T("WindowHDC")))
        pProperties->putAt(OD_T("WindowHDC"),  OdRxVariantValue((OdIntPtr)m_hWindowDC)); // hWindowDC necessary for Bitmap device
    }

    enableGsModel(false); // TODO: use theApp.useGsModel() setting

    OdString strGDIModuleName = OdWinGDIModuleName;
    strGDIModuleName = strGDIModuleName.left( strGDIModuleName.getLength() - 4 );

    if( str.find(strGDIModuleName) != -1 )
    {
      setLineWeightConfiguration( kPointLineWeight, 1 );
      setLineWeightConfiguration( kLineCapStyle, (OdUInt32)(OdPs::kLesRound) );
      setLineWeightConfiguration( kLineJoinStyle, (OdUInt32)(OdPs::kLjsRound) );
    }
    else
    {
      setLineWeightConfiguration( kPointLineWeight, 1 );
      setLineWeightConfiguration( kLineCapStyle, (OdUInt32)(OdPs::kLesButt) );
      setLineWeightConfiguration( kLineJoinStyle, (OdUInt32)(OdPs::kLjsDiamond) );
    }
    //

    OdDgDatabase* pDb = (OdDgDatabase*)database();
    OdDgElementId modelId = pDb->getActiveModelId();

    COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
    m_pDevice = OdGsDeviceForDgModel::setupModelView(modelId, pDoc->getVeiwId( pDoc->getActiveView() ) /*viewId*/, pDevice, this);
    m_modelId = modelId;

    const ODCOLORREF* refColors = OdDgColorTable::currentPalette( pDb );

    ODGSPALETTE pPalCpy;
    pPalCpy.insert(pPalCpy.begin(), refColors, refColors + 256);

    OdDgModelPtr pModel = m_modelId.safeOpenObject();
    COLORREF background = pModel->getBackground();
    theApp.settings().setActiveBackground( background );

    // Color with #255 always defines backround. The background of the active model must be considered in the device palette.
    pPalCpy[255] = background;
    // Note: This method should be called to resolve "white background issue" before setting device palette
    bool bCorrected = OdDgColorTable::correctPaletteForWhiteBackground(pPalCpy.asArrayPtr());

    m_pDevice->setLogicalPalette(pPalCpy.asArrayPtr(), 256);
    m_pDevice->setBackgroundColor(theApp.settings().getActiveBackground()); // ACAD's color for paper bg
    setPaletteBackground(theApp.settings().getActiveBackground());
  
    OdGsDCRect  gsRect(rc.left, rc.right, rc.bottom, rc.top);
    m_pDevice->onSize(gsRect);

  }
  catch(const OdError& e)
  {
    destroyDevice();
    theApp.reportError(_T("Graphic System Initialization Error"), e);
  }  
}

void COdaDgnAppVectorizer::destroyDevice() 
{
  m_pDevice.release();
}

HCURSOR COdaDgnAppVectorizer::cursor() const
{
  return m_hCursor;
}

void COdaDgnAppVectorizer::setCursor(HCURSOR hCursor)
{
  m_hCursor = hCursor;
  ::SetCursor(hCursor);
}

void COdaDgnAppVectorizer::OnSetRenderMode(UINT nID)
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

    for( int i = 0; i < m_pDevice->numViews(); i++ )
    {
      OdGsView* pXRefView = m_pDevice->viewAt(i);

      if( pXRefView == pView )
      {
        continue;
      }

      pXRefView->setMode( rm );
    }

    if(pView->mode() != rm)
    {
      COdaDgnApp::messageBox(m_hWnd, _T("Rendering"), _T("Mode is not supported by current device."), MB_ICONWARNING);
    }
    else
    {
      PostMessage(WM_PAINT);
    }
  }
}

void COdaDgnAppVectorizer::OnViewerRegen() 
{
  m_pDevice->invalidate();
  if(m_pDevice->gsModel())
    m_pDevice->gsModel()->invalidate(OdGsModel::kInvalidateAll);
  PostMessage(WM_PAINT);
}

void COdaDgnAppVectorizer::OnViewerVpregen() 
{
  m_pDevice->invalidate();
  if(m_pDevice->gsModel())
    m_pDevice->gsModel()->invalidate(getActiveView());
  PostMessage(WM_PAINT);
}

void COdaDgnAppVectorizer::OnUpdateViewerRegen(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_pDevice->gsModel()!=0);
}

bool COdaDgnAppVectorizer::canClose() const
{
  if(m_mode != kQuiescent)
  {
    AfxMessageBox(_T("Can not exit while command is active."), MB_OK|MB_ICONEXCLAMATION);
    return false;
  }
  return true;
}

void COdaDgnAppVectorizer::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  switch( nChar ) 
  {
  case VK_ESCAPE:
    break;

  case VK_F5:
    PostMessage(WM_PAINT);
    break;

  case VK_DELETE:
    PostMessage(WM_PAINT);
    break;
  }

  CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL COdaDgnAppVectorizer::PreCreateWindow(CREATESTRUCT& cs)
{
  HCURSOR cursor = LoadCursor( NULL, IDC_CROSS );
  cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW|CS_OWNDC, cursor );
  return CView::PreCreateWindow(cs);
}

void COdaDgnAppVectorizer::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
  CView::OnUpdate(pSender, lHint, pHint);
  //if ( !m_pDevice.isNull() )
    //OnViewerRegen();
}

void COdaDgnAppVectorizer::OnUpdate2dwireframe(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::k2DOptimized);
}

/////////////////
bool checkAcceptability( UINT nID, OdGsView* pView )
{
  bool bEnable = true;
  OdGsView::RenderMode rm = OdGsView::RenderMode(OdGsView::k2DOptimized + (nID - ID_2DWIREFRAME));
  OdGsView::RenderMode rmActual = pView->mode();
  pView->setMode( rm );
  if (pView->mode() != rm)
  {
    bEnable = false;
  }
  pView->setMode( rmActual ); // restore

  return bEnable;
}

void COdaDgnAppVectorizer::OnUpdate3dwireframe(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kWireframe);
}

void COdaDgnAppVectorizer::OnUpdateHidden(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kHiddenLine);
}

void COdaDgnAppVectorizer::OnUpdateFlatShaded(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kFlatShaded);
}

void COdaDgnAppVectorizer::OnUpdateFlatShadedWithEdges(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kFlatShadedWithWireframe);
}

void COdaDgnAppVectorizer::OnUpdateGouraudShaded(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kGouraudShaded);
}

void COdaDgnAppVectorizer::OnUpdateGouraudShadedEdgesOn(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( checkAcceptability( pCmdUI->m_nID, getActiveView() ));
  pCmdUI->SetCheck(getActiveView()->mode() == OdGsView::kGouraudShadedWithWireframe);
}

void COdaDgnAppVectorizer::respond(const OdString& s)
{
  m_response.m_type = Response::kString;
  m_response.m_string = s;
}

void COdaDgnAppVectorizer::zoom( double steps )
{
  OdGsView* view = getActiveView();
  
  if( view->isPerspective() )
  {
    OdGePoint3d position = view->position(), target = view->target();
    OdGeVector3d direction = target - position;
    OdGeVector3d upVector = view->upVector();
    double width = view->fieldWidth(), height = view->fieldHeight();

    //calculate an appropriate offset using real size of the frame
    if( width < height )
    {
      steps *= width / 2.;
    }
    else
    {
      steps *= height / 2.;
    }
    direction *= steps;

    position += direction;
    target += direction;

    view->setView( position, target, upVector, width, height, OdGsView::kPerspective );

    for( int i = 0; i < m_pDevice->numViews(); i++ )
    {
      OdGsView* pXRefView = m_pDevice->viewAt(i);

      if( pXRefView == view )
      {
        continue;
      }

      OdGsView::RenderMode renderMode = pXRefView->mode();
      view->viewParameters( pXRefView );
      pXRefView->setMode( renderMode );
    }
  }
  else
  {
    view->zoom( pow( 1.11, double( steps ) ) );

    for( int i = 0; i < m_pDevice->numViews(); i++ )
    {
      OdGsView* pXRefView = m_pDevice->viewAt(i);

      if( pXRefView == view )
      {
        continue;
      }

      OdGsView::RenderMode renderMode = pXRefView->mode();
      view->viewParameters( pXRefView );
      pXRefView->setMode( renderMode );
    }
  }

  PostMessage( WM_PAINT );
}

void COdaDgnAppVectorizer::CurrentOperation::initialize( CWnd* window )
{
  m_type = kNone;
  m_window = window;
}

void COdaDgnAppVectorizer::CurrentOperation::drawMarker( const CPoint& currentPoint )
{
  undoDrawnMarker();

  switch( m_type )
  {
  case k3dOrbit : drawMarkerFor3dOrbit( currentPoint ); break;
  case kPan : drawMarkerForPan( currentPoint ); break;
  }

  m_drawnMarker.m_point = currentPoint;
  m_drawnMarker.m_type = m_type;
}

void COdaDgnAppVectorizer::CurrentOperation::setStartPoint( const CPoint& startPoint )
{
  m_startPoint = startPoint;
}

void COdaDgnAppVectorizer::CurrentOperation::drawMarkerFor3dOrbit( const CPoint& currentPoint )
{
  RECT extent;
  m_window->GetClientRect( &extent );

  double sizeFactor;
  {
    double x = ( extent.right - extent.left ) / 10., y = (extent.bottom - extent.top ) / 10.;
    sizeFactor = x < y ? x : y;

    //zero value can insult the further calculations
    if( !sizeFactor )
    {
      return;
    }
  }
  OdGePoint2d startPoint( m_startPoint.x, m_startPoint.y ), _currentPoint( currentPoint.x, currentPoint.y );

  CClientDC context( m_window );
  context.SetROP2( R2_NOT );

  //determine a rotation
  {
    OdGeVector2d line = _currentPoint - startPoint;
    m_resultRotation.setToRotation( atan( line.length() / sizeFactor / 10. ) * 4., OdGeVector3d( line.y, line.x, 0. ) );
  }

  //draw a cube
  {
    OdGePoint3d center3d( ( extent.right + extent.left ) / 2., ( extent.top + extent.bottom ) / 2., 0. ), point3d;
    OdGeVector3d directions[ 10 ];
    OdGePoint2d points[ 10 ];

    directions[ 0 ].set( -sizeFactor, -sizeFactor, sizeFactor );
    directions[ 1 ].set( -sizeFactor, sizeFactor, sizeFactor );
    directions[ 2 ].set( sizeFactor, sizeFactor, sizeFactor );
    directions[ 3 ].set( sizeFactor, -sizeFactor, sizeFactor );
    directions[ 4 ].set( -sizeFactor, -sizeFactor, -sizeFactor );
    directions[ 5 ].set( -sizeFactor, sizeFactor, -sizeFactor );
    directions[ 6 ].set( sizeFactor, sizeFactor, -sizeFactor );
    directions[ 7 ].set( sizeFactor, -sizeFactor, -sizeFactor );
    directions[ 8 ].set( 0., sizeFactor * .3, sizeFactor );
    directions[ 9 ].set( 0., -sizeFactor, sizeFactor );
    
    {
      int i;
      OdGeMatrix3d invertY;
      
      invertY[ 1 ][ 1 ] = -1.;
      invertY = invertY * m_resultRotation;
      for( i = 0; i < 10; i++ )
      {
        point3d = center3d + invertY * directions[ i ];
        points[ i ].set( point3d.x, point3d.y );
      }
    }

    context.MoveTo( toPOINT( points[ 0 ] ) );
    context.LineTo( toPOINT( points[ 1 ] ) );
    context.LineTo( toPOINT( points[ 2 ] ) );
    context.LineTo( toPOINT( points[ 6 ] ) );
    context.LineTo( toPOINT( points[ 7 ] ) );
    context.LineTo( toPOINT( points[ 4 ] ) );
    context.LineTo( toPOINT( points[ 5 ] ) );
    context.LineTo( toPOINT( points[ 1 ] ) );
    context.MoveTo( toPOINT( points[ 2 ] ) );
    context.LineTo( toPOINT( points[ 3 ] ) );
    context.LineTo( toPOINT( points[ 0 ] ) );
    context.LineTo( toPOINT( points[ 4 ] ) );
    context.MoveTo( toPOINT( points[ 5 ] ) );
    context.LineTo( toPOINT( points[ 6 ] ) );
    context.MoveTo( toPOINT( points[ 7 ] ) );
    context.LineTo( toPOINT( points[ 3 ] ) );
    context.LineTo( toPOINT( points[ 8 ] ) );
    context.LineTo( toPOINT( points[ 9 ] ) );
  }
}

void COdaDgnAppVectorizer::CurrentOperation::drawMarkerForPan( const CPoint& currentPoint )
{
  RECT extent;
  m_window->GetClientRect( &extent );

  //sizeFactor determines the size of arrow (its width, its elements)
  double sizeFactor;
  {
    double x = ( extent.right - extent.left ) / 100., y = (extent.bottom - extent.top ) / 100.;
    sizeFactor = x < y ? x : y;

    //zero value can insult the further calculations
    if( !sizeFactor )
    {
      return;
    }
  }
  OdGePoint2d startPoint( m_startPoint.x, m_startPoint.y ), _currentPoint( currentPoint.x, currentPoint.y );

  CClientDC context( m_window );
  context.SetROP2( R2_NOT );

  OdGeVector2d direction, perpedicular;
  double length;
  {
    direction = _currentPoint - startPoint;
    length = direction.length();
    if( length < sizeFactor )
    {
      m_resultOffset.set( 0., 0. );
      return;
    }

    //scale the arrow to fit to the length
    if( length < sizeFactor * 5. )
    {
      sizeFactor = length / 5.;
    }

    //store the result
    {
      m_resultOffset.set( direction.x, direction.y );
    }

    direction.normalize();
    perpedicular.set( -direction.y, direction.x );
  }

  //arrow to the current position
  {
    POINT points[ 8 ];

    points[ 0 ] = toPOINT( startPoint + perpedicular * sizeFactor );
    points[ 1 ] = toPOINT( _currentPoint + perpedicular * sizeFactor - direction * sizeFactor * 5. );
    points[ 2 ] = toPOINT( _currentPoint + perpedicular * sizeFactor * 2. - direction * sizeFactor * 5. );
    points[ 3 ] = toPOINT( _currentPoint );
    points[ 4 ] = toPOINT( _currentPoint - perpedicular * sizeFactor * 2. - direction * sizeFactor * 5. );
    points[ 5 ] = toPOINT( _currentPoint - perpedicular * sizeFactor - direction * sizeFactor * 5. );
    points[ 6 ] = toPOINT( startPoint - perpedicular * sizeFactor );
    points[ 7 ] = toPOINT( startPoint + perpedicular * sizeFactor );

    context.Polyline( points, 8 );
  }
}

void COdaDgnAppVectorizer::CurrentOperation::setType( Type type )
{
  m_type = type;
}

COdaDgnAppVectorizer::CurrentOperation::Type COdaDgnAppVectorizer::CurrentOperation::getType() const
{
  return m_type;
}

CPoint COdaDgnAppVectorizer::CurrentOperation::toCPoint( const OdGePoint2d& original ) const
{
  return CPoint( int( original.x ), int( original.y ) );
}

void COdaDgnAppVectorizer::CurrentOperation::undoDrawnMarker()
{
  if( m_drawnMarker.m_type == kNone )
  {
    return;
  }

  switch( m_drawnMarker.m_type )
  {
  case k3dOrbit : drawMarkerFor3dOrbit( m_drawnMarker.m_point ); break;
  case kPan : drawMarkerForPan( m_drawnMarker.m_point ); break;
  }

  m_drawnMarker.m_type = kNone;
}

POINT COdaDgnAppVectorizer::CurrentOperation::toPOINT( const OdGePoint2d& original ) const
{
  POINT a = { long( original.x ), long( original.y ) };
  return a;
}

COdaDgnAppVectorizer::CurrentOperation::DrawnMarker::DrawnMarker()
: m_type( kNone )
{
}

const OdGeMatrix3d& COdaDgnAppVectorizer::CurrentOperation::getResultRotation() const
{
  return m_resultRotation;
}

const OdGeVector2d& COdaDgnAppVectorizer::CurrentOperation::getResultOffset() const
{
  return m_resultOffset;
}

OdGePoint3d COdaDgnAppVectorizer::getWorldCoordinateByScreen( const OdGePoint2d& screenPoint ) const
{
  OdGsViewPtr view = getActiveView();

  //directions, origin, etc
  OdGePoint3d target = view->target();
  OdGeVector3d xVector, yVector;
  {
    OdGeVector3d direction = target - view->position();
    yVector = view->upVector();
    xVector = direction.crossProduct( yVector ).normal();
  }

  //the simplest way I found is to make back calculations...
  OdGePoint3d center, stepX, stepY;
  {
    center = target;
    stepX = target + xVector;
    stepY = target + yVector;

    OdGeMatrix3d matrix = view->worldToDeviceMatrix();
    stepX.transformBy( matrix );
    stepY.transformBy( matrix );
    center.transformBy( matrix );
  }

  //make the calculation
  OdGePoint3d result;
  {
    double x, y;
    x = ( screenPoint.x - center.x ) / ( stepX.x - center.x );
    y = ( screenPoint.y - center.y ) / ( stepY.y - center.y );

    result = target + xVector * x + yVector * y;
  }

  return result;
}

void COdaDgnAppVectorizer::OnVectorize() 
{
  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  pDoc->OnVectorize(theApp.settings().getActiveGS() /*.getGsDevice()*/);
}

void COdaDgnAppVectorizer::OnActivateFrame(
   UINT nState,
   CFrameWnd* pFrameWnd 
)
{
}
afx_msg void COdaDgnAppVectorizer::OnWinDirectX()
{
  theApp.settings().setActiveGS(ID_VECTORIZE_WINDIRECTX);
  destroyDevice();
  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  createDevice( pDoc->vectorizedModelId(), pDoc->vectorizedViewId() );
  OnReToolbar();
  PostMessage( WM_PAINT );
}
afx_msg void COdaDgnAppVectorizer::OnWinGDI()
{
  theApp.settings().setActiveGS(ID_VECTORIZE_WINGDI);
  destroyDevice();
  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  createDevice( pDoc->vectorizedModelId(), pDoc->vectorizedViewId() );
  OnReToolbar();
  PostMessage( WM_PAINT );
}
afx_msg void COdaDgnAppVectorizer::OnWinOpenGL()
{
  theApp.settings().setActiveGS(ID_VECTORIZE_WINOPENGL);
  destroyDevice();
  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  createDevice( pDoc->vectorizedModelId(), pDoc->vectorizedViewId() );
  OnReToolbar();
  PostMessage( WM_PAINT );
}

//========
afx_msg void COdaDgnAppVectorizer::OnViewNumber(UINT nID)
{
  OdUInt16 item = OdUInt16( nID - ID_VIEW1 + 1 );

  destroyDevice();
  COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
  pDoc->setActiveView( item );
  createDevice( pDoc->vectorizedModelId(), pDoc->getVeiwId( item ) );
  OnReToolbar();
  PostMessage( WM_PAINT );
}
void COdaDgnAppVectorizer::OnUpdateView1(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true );
  pCmdUI->SetCheck( 1 == (static_cast<COdaDgnAppDoc*>(GetDocument()))->getActiveView());
}
void COdaDgnAppVectorizer::OnUpdateView2(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true );
  pCmdUI->SetCheck( 2 == (static_cast<COdaDgnAppDoc*>(GetDocument()))->getActiveView());
}
void COdaDgnAppVectorizer::OnUpdateView3(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true );
  pCmdUI->SetCheck( 3 == (static_cast<COdaDgnAppDoc*>(GetDocument()))->getActiveView());
}
void COdaDgnAppVectorizer::OnUpdateView4(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true );
  pCmdUI->SetCheck( 4 == (static_cast<COdaDgnAppDoc*>(GetDocument()))->getActiveView());
}
void COdaDgnAppVectorizer::OnUpdateView5(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true );
  pCmdUI->SetCheck( 5 == (static_cast<COdaDgnAppDoc*>(GetDocument()))->getActiveView());
}
void COdaDgnAppVectorizer::OnUpdateView6(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true );
  pCmdUI->SetCheck( 6 == (static_cast<COdaDgnAppDoc*>(GetDocument()))->getActiveView());
}
void COdaDgnAppVectorizer::OnUpdateView7(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true );
  pCmdUI->SetCheck( 7 == (static_cast<COdaDgnAppDoc*>(GetDocument()))->getActiveView());
}
void COdaDgnAppVectorizer::OnUpdateView8(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( true );
  pCmdUI->SetCheck( 8 == (static_cast<COdaDgnAppDoc*>(GetDocument()))->getActiveView());
}
afx_msg void COdaDgnAppVectorizer::OnReToolbar()
{
      COdaDgnAppDoc* pDoc = static_cast<COdaDgnAppDoc*>(GetDocument());
      int nSize = pDoc->getViews();

	    // Set the toolbar to show only partial commmand list
	    m_toolBar.SetButtons(NULL, 15 + nSize);
	    //m_toolBar.SetButtons(NULL, 14);
	    m_toolBar.SetButtonInfo(0, ID_VIEW_ZOOM_WINDOW,TBBS_BUTTON, 0);
	    m_toolBar.SetButtonInfo(1, ID_VIEW_ZOOMIN,TBBS_BUTTON, 1);
	    m_toolBar.SetButtonInfo(2, ID_VIEW_ZOOMOUT,TBBS_BUTTON, 2);
	    m_toolBar.SetButtonInfo(3, ID_VIEW_ZOOMEXTENTS,TBBS_BUTTON, 3);
	    m_toolBar.SetButtonInfo(4, ID_VIEW_PAN,TBBS_BUTTON, 4);
	    m_toolBar.SetButtonInfo(5, ID_VIEW_3DORBIT,TBBS_BUTTON, 5);

	    m_toolBar.SetButtonInfo(6, ID_SEPARATOR,TBBS_SEPARATOR, 21);

	    m_toolBar.SetButtonInfo(7, ID_2DWIREFRAME,TBBS_BUTTON, 6);
	    m_toolBar.SetButtonInfo(8, ID_3DWIREFRAME,TBBS_BUTTON, 7);
	    m_toolBar.SetButtonInfo(9, ID_HIDDEN,TBBS_BUTTON, 8);
	    m_toolBar.SetButtonInfo(10, ID_FLAT_SHADED,TBBS_BUTTON, 9);
	    m_toolBar.SetButtonInfo(11, ID_GOURAUD_SHADED,TBBS_BUTTON, 10);
	    m_toolBar.SetButtonInfo(12, ID_FLAT_SHADED_WITH_EDGES,TBBS_BUTTON, 11);
	    m_toolBar.SetButtonInfo(13, ID_GOURAUD_SHADED_EDGES_ON,TBBS_BUTTON, 12);

	    m_toolBar.SetButtonInfo(14, ID_SEPARATOR,TBBS_SEPARATOR, 21);

      for (int i = 0; i < nSize; i++)
      {
	      m_toolBar.SetButtonInfo(15 + i, ID_VIEW1 + i, TBBS_BUTTON, 13 + i);
        m_toolBar.SetButtonStyle(15 + i, TBBS_CHECKGROUP);

      }
	m_toolBar.Invalidate();
	AfxGetApp()->OnIdle(-1);
}
