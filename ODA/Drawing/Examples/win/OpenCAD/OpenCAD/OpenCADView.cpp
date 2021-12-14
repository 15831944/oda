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

// OpenCADView.cpp : implementation of the COpenCADView class
//

#include "stdafx.h"
#include "OpenCAD.h"

#include "OpenCADDoc.h"
#include "OpenCADView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "DbGsManager.h"
#include "RxVariantValue.h"
#include "AbstractViewPE.h"
#include "ColorMapping.h"
#include "AbstractViewPE.h"

// COpenCADView

IMPLEMENT_DYNCREATE(COpenCADView, CView)

BEGIN_MESSAGE_MAP(COpenCADView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_VIEW_RENDERMODE_2DWIREFRAME, COpenCADView::OnViewRendermode2dwireframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_2DWIREFRAME, COpenCADView::OnUpdateViewRendermode2dwireframe)
	ON_COMMAND(ID_VIEW_RENDERMODE_3DWIREFRAME, COpenCADView::OnViewRendermode3dwireframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_3DWIREFRAME, COpenCADView::OnUpdateViewRendermode3dwireframe)
	ON_COMMAND(ID_VIEW_RENDERMODE_3DHIDDEN, COpenCADView::OnViewRendermode3dhidden)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_3DHIDDEN, COpenCADView::OnUpdateViewRendermode3dhidden)
	ON_COMMAND(ID_VIEW_RENDERMODE_FLATSHADED, COpenCADView::OnViewRendermodeFlatshaded)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_FLATSHADED, COpenCADView::OnUpdateViewRendermodeFlatshaded)
	ON_COMMAND(ID_VIEW_RENDERMODE_FLATSHADEDWIREFRAME, COpenCADView::OnViewRendermodeFlatshadedwireframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_FLATSHADEDWIREFRAME, COpenCADView::OnUpdateViewRendermodeFlatshadedwireframe)
	ON_COMMAND(ID_VIEW_RENDERMODE_SMOOTHSHADED, COpenCADView::OnViewRendermodeSmoothshaded)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_SMOOTHSHADED, COpenCADView::OnUpdateViewRendermodeSmoothshaded)
	ON_COMMAND(ID_VIEW_RENDERMODE_SMOOTHSHADEDWIREFRAME, COpenCADView::OnViewRendermodeSmoothshadedwireframe)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RENDERMODE_SMOOTHSHADEDWIREFRAME, COpenCADView::OnUpdateViewRendermodeSmoothshadedwireframe)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_VIEW_ZOOMEXTENTS, COpenCADView::OnViewZoomextents)
	ON_COMMAND(ID_VIEW_ZOOMWINDOW, COpenCADView::OnViewZoomwindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMWINDOW, COpenCADView::OnUpdateViewZoomwindow)
END_MESSAGE_MAP()

// COpenCADView construction/destruction

COpenCADView::COpenCADView()
{
	// TODO: add construction code here
	m_clrBackground = RGB(0, 0, 0);
	m_iRenderMode = OdGsView::k2DOptimized;
	m_bLeftButton = FALSE;
	m_bMiddleButton = FALSE;
	m_bRightButton = FALSE;
	m_bZoomWindow = FALSE;
	m_pTracker = NULL;
}

COpenCADView::~COpenCADView()
{
}

BOOL COpenCADView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COpenCADView drawing

void COpenCADView::OnDraw(CDC* /*pDC*/)
{
	COpenCADDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// COpenCADView printing

BOOL COpenCADView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COpenCADView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COpenCADView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COpenCADView diagnostics

#ifdef _DEBUG
void COpenCADView::AssertValid() const
{
	CView::AssertValid();
}

void COpenCADView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COpenCADDoc* COpenCADView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COpenCADDoc)));
	return (COpenCADDoc*)m_pDocument;
}
#endif //_DEBUG


// COpenCADView message handlers

void COpenCADView::ResetDevice(BOOL bZoomExtents)
{
	// Get the client rectangle
	CRect rc;
	GetClientRect(&rc);

	// Load the vectorization module
	OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);//(OdWinDirectXModuleName); 
  if (pGs.isNull())
  {
    MessageBox(_T("Can't load vectorization module " OdWinOpenGLModuleName), _T("OpenCAD - Rendering aborted"), MB_ICONERROR);
    exit(1);
  }

	// Create a new OdGsDevice object, and associate with the vectorization GsDevice
	m_pDevice = pGs->createDevice();
	if(m_pDevice.isNull())
		return;

	// Return a pointer to the dictionary entity containing the device properties
	OdRxDictionaryPtr pProperties = m_pDevice->properties();

	// Set the window handle for this GsDevice
	pProperties->putAt("WindowHWND", OdRxVariantValue((OdIntPtr)m_hWnd));

	// Define a device coordinate rectangle equal to the client rectangle
	OdGsDCRect gsRect(rc.left, rc.right, rc.bottom, rc.top);

	// Set the device background color and palette
	m_pDevice->setBackgroundColor(m_clrBackground);
	m_pDevice->setLogicalPalette(CurrentPalette(), 256);

	if(!database())
		return;
    
	// Set up the views for the active layout
	m_pDevice = OdDbGsManager::setupActiveLayoutViews(m_pDevice, this);

	// Return true if and only the current layout is a paper space layout
	BOOL bModelSpace = (GetDocument()->m_pDatabase->getTILEMODE() == 0);

	// Set the viewport border properties
	SetViewportBorderProperties(m_pDevice, !bModelSpace);

	if(bZoomExtents)
		ViewZoomExtents();
  
	// Update the client rectangle
	OnSize(0, rc.Width(),rc.Height());

	// Redraw the window
	RedrawWindow();
}

void COpenCADView::SetViewportBorderProperties(OdGsDevice* pDevice, BOOL bModel) 
{
	// If current layout is Model, and it has more then one viewport then make their borders visible.
	// If current layout is Paper, then make visible the borders of all but the overall viewport.
	int n = pDevice->numViews();
	if(n > 1)
	{
		for(int i = bModel ? 0 : 1; i < n; ++i)
		{
			// Get the viewport
			OdGsViewPtr pView = pDevice->viewAt(i);

			// Make it visible
			pView->setViewportBorderVisibility(true);
			
			// Set the color and width
			pView->setViewportBorderProperties(CurrentPalette()[7], 1);
		}
	}
}

OdGsViewPtr COpenCADView::GetView()
{
	return m_pDevice->viewAt(0);
}

void COpenCADView::ViewZoomExtents() 
{
	// Get the overall viewport
	OdGsViewPtr pView = GetView();

	// Modifies the viewport to fit the extents
	OdAbstractViewPEPtr(pView)->zoomExtents(pView);

	PostMessage(WM_PAINT);
}

const ODCOLORREF* COpenCADView::CurrentPalette()
{
	const ODCOLORREF *pColor = odcmAcadPalette(m_clrBackground);
	return pColor;
}

void COpenCADView::OnInitialUpdate()
{
	CView::OnInitialUpdate(); // msvc6 support // __super::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	OdGiContextForDbDatabase::setDatabase(GetDocument()->m_pDatabase);
	enableGsModel(true);
	ResetDevice(true);
}

void COpenCADView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy); // msvc6 support // __super::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(!m_pDevice.isNull() && cx && cy)
	{
		CRect rc;
		GetClientRect(rc);

		// Update the client rectangle
		OdGsDCRect Rect(OdGsDCPoint(rc.left, rc.bottom), OdGsDCPoint(rc.right, rc.top));
		m_pDevice->onSize(Rect);
	}
}

void COpenCADView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call __super::OnPaint() for painting messages

	// Paint the client rectangle with the GS device
	if(!m_pDevice.isNull())
  {
    try
    {
		  m_pDevice->update();
    }
    catch(const OdError& e)
    {
      MessageBox(e.description(), _T("OpenCAD - Rendering aborted"), MB_ICONERROR);
      exit(1);
    }
    catch(...)
    {
      MessageBox(_T("Unknown error"), _T("OpenCAD - Rendering aborted"), MB_ICONERROR);
      exit(1);
    }
  }
}

BOOL COpenCADView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	//return __super::OnEraseBkgnd(pDC);
	return TRUE;
}

void COpenCADView::SetRenderMode(OdGsView::RenderMode iRenderMode)
{
	// Get the view
	OdGsViewPtr pView = GetView();

	// Check if render mode needs to be changed
	if(pView->mode() == iRenderMode)
		return;

	// Set the render mode
	pView->setMode(iRenderMode);

	// Check if render mode was changed
	if(pView->mode() != iRenderMode)
	{
		MessageBox(_T("Sorry, this render mode is not supported by the current device"), _T("OpenCAD"), MB_ICONWARNING);
	}
	else
	{
		// Show the new render mode
		PostMessage(WM_PAINT);
		m_iRenderMode = iRenderMode;
	}
}
void COpenCADView::OnViewRendermode2dwireframe()
{
	// TODO: Add your command handler code here
	SetRenderMode(OdGsView::k2DOptimized);
}

void COpenCADView::OnUpdateViewRendermode2dwireframe(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_iRenderMode == OdGsView::k2DOptimized);
}

void COpenCADView::OnViewRendermode3dwireframe()
{
	// TODO: Add your command handler code here
	SetRenderMode(OdGsView::kWireframe);
}

void COpenCADView::OnUpdateViewRendermode3dwireframe(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_iRenderMode == OdGsView::kWireframe);
}

void COpenCADView::OnViewRendermode3dhidden()
{
	// TODO: Add your command handler code here
	SetRenderMode(OdGsView::kHiddenLine);
}

void COpenCADView::OnUpdateViewRendermode3dhidden(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_iRenderMode == OdGsView::kHiddenLine);
}

void COpenCADView::OnViewRendermodeFlatshaded()
{
	// TODO: Add your command handler code here
	SetRenderMode(OdGsView::kFlatShaded);
}

void COpenCADView::OnUpdateViewRendermodeFlatshaded(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_iRenderMode == OdGsView::kFlatShaded);
}

void COpenCADView::OnViewRendermodeFlatshadedwireframe()
{
	// TODO: Add your command handler code here
	SetRenderMode(OdGsView::kFlatShadedWithWireframe);
}

void COpenCADView::OnUpdateViewRendermodeFlatshadedwireframe(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_iRenderMode == OdGsView::kFlatShadedWithWireframe);
}

void COpenCADView::OnViewRendermodeSmoothshaded()
{
	// TODO: Add your command handler code here
	SetRenderMode(OdGsView::kGouraudShaded);
}

void COpenCADView::OnUpdateViewRendermodeSmoothshaded(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_iRenderMode == OdGsView::kGouraudShaded);
}

void COpenCADView::OnViewRendermodeSmoothshadedwireframe()
{
	// TODO: Add your command handler code here
	SetRenderMode(OdGsView::kGouraudShadedWithWireframe);
}

void COpenCADView::OnUpdateViewRendermodeSmoothshadedwireframe(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_iRenderMode == OdGsView::kGouraudShadedWithWireframe);
}

void COpenCADView::Dolly(int x, int y) 
{
	// Get the view
	OdGsViewPtr pView = GetView();

	// Set up the dolly vector
	OdGeVector3d Vector(-x, -y, 0.0);
	Vector.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());

	// Perform the dolly
	pView->dolly(Vector);
}

BOOL COpenCADView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	OdGsViewPtr pView = GetView();

	OdGePoint3d Position(pView->position());
	Position.transformBy(pView->worldToDeviceMatrix());
	int x, y;
	x = (int)OdRound(Position.x);
	y = (int)OdRound(Position.y);
	x = pt.x - x;
	y = pt.y - y;

	Dolly(-x, -y);
	pView->zoom(zDelta > 0 ? 1.0 / 0.9 : 0.9);
	Dolly(x, y);

	PostMessage(WM_PAINT);

	return CView::OnMouseWheel(nFlags, zDelta, pt); // msvc6 support // __super::OnMouseWheel(nFlags, zDelta, pt);
}

void COpenCADView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLeftButton = TRUE;
	m_MousePosition = point;
	m_MouseClick = point;

	if(m_bZoomWindow == TRUE)
	{
		m_Points.clear(); // Empty point array
		m_Points.append(GetWorldCoordinates(point)); // Record first point
	}
	else if(GetChildFrame()->m_wndCommandPrompt.m_iCommandMode == CCommandPrompt::eCommandModeGetPoint)
	{
		OdGePoint3d Point = GetWorldCoordinates(point);
		CString strCoordinates;
		strCoordinates.Format(_T("%f,%f,%f"), Point.x, Point.y, Point.z);
		GetChildFrame()->m_wndCommandPrompt.SetCommand(strCoordinates, TRUE);
	}
	CView::OnLButtonDown(nFlags, point); // msvc6 support // __super::OnLButtonDown(nFlags, point);
}

void COpenCADView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLeftButton = FALSE;

	if(m_bZoomWindow == TRUE)
	{
		m_Points.append(GetWorldCoordinates(point)); // Record second point
		if(m_Points.length() == 2)	// Zoom rectangle has been completely defined
		{
			ZoomWindow(m_Points[0], m_Points[1]);
			m_bZoomWindow = FALSE; // Turn off zoom window mode
			m_Points.clear(); // Empty point array
		}
	}

	CView::OnLButtonUp(nFlags, point); // msvc6 support // __super::OnLButtonUp(nFlags, point);
}

void COpenCADView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMiddleButton = TRUE;
	m_MousePosition = point;
	CView::OnMButtonDown(nFlags, point); // msvc6 support // __super::OnMButtonDown(nFlags, point);
}

void COpenCADView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bMiddleButton = FALSE;
	CView::OnMButtonUp(nFlags, point); // msvc6 support // __super::OnMButtonUp(nFlags, point);
}

void COpenCADView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bRightButton = TRUE;
	m_MousePosition = point;
	CView::OnRButtonDown(nFlags, point); // msvc6 support // __super::OnRButtonDown(nFlags, point);
}

void COpenCADView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bRightButton = FALSE;
	CView::OnRButtonUp(nFlags, point); // msvc6 support // __super::OnRButtonUp(nFlags, point);
}

void COpenCADView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(m_bLeftButton == TRUE)
	{
		if(m_bZoomWindow == TRUE)
		{
			CClientDC dc(this);	// Get a client DC which accesses the client area of the view
			CRect rcZoom, rcZoomOld;
			
			// Get the old zoom window
			rcZoomOld.SetRect(m_MouseClick.x, m_MouseClick.y, m_MousePosition.x, m_MousePosition.y);
			rcZoomOld.NormalizeRect();
			rcZoomOld.InflateRect(1, 1);

			// Redraw the old zoom window
			RedrawWindow(&rcZoomOld);

			// Create the new zoom window
			rcZoom.SetRect(m_MouseClick.x, m_MouseClick.y, point.x, point.y);
			rcZoom.NormalizeRect();

			// Draw a focus rectangle for the new zoom window
			dc.DrawFocusRect(&rcZoom);

			m_MousePosition = point;
		}
	}
	else if(m_bMiddleButton == TRUE)
	{
		OdGeVector3d Vector(m_MousePosition.x - point.x, m_MousePosition.y - point.y, 0.0);
		// Screen to Eye
		Vector.transformBy((GetView()->screenMatrix() * GetView()->projectionMatrix()).inverse());
		GetView()->dolly(Vector);
		m_MousePosition = point;
		PostMessage(WM_PAINT);
	}
	else if(m_bRightButton == TRUE)
	{
		GetView()->orbit((m_MousePosition.y - point.y) / 100.0, (m_MousePosition.x - point.x) / 100.0);
		m_MousePosition = point;
		PostMessage(WM_PAINT);
	}
	else if(GetChildFrame()->m_wndCommandPrompt.m_iCommandMode == CCommandPrompt::eCommandModeGetPoint)
	{
		if(m_pTracker)
		{
			static_cast<OdEdPointTracker*>(m_pTracker)->setValue(GetWorldCoordinates(point));
			if(!GetView()->isValid())
				PostMessage(WM_PAINT);
		}
	}

	CView::OnMouseMove(nFlags, point); // msvc6 support // __super::OnMouseMove(nFlags, point);
}

void COpenCADView::OnViewZoomextents()
{
	// TODO: Add your command handler code here
	ViewZoomExtents();
}

OdGePoint3d COpenCADView::GetWorldCoordinates(CPoint Point)
{
	OdGsViewPtr pView = GetView();
	OdGePoint3d WCSPoint(Point.x, Point.y, 0.0);
	
	WCSPoint.transformBy((pView->screenMatrix() * pView->projectionMatrix()).inverse());
	WCSPoint.z = 0.0;
	WCSPoint.transformBy(OdAbstractViewPEPtr(pView)->eyeToWorld(pView));

	return WCSPoint;
}

void COpenCADView::ZoomWindow(OdGePoint3d Point1, OdGePoint3d Point2)
{
	OdGsViewPtr pView = GetView();
	OdGeMatrix3d WorldToEye = OdAbstractViewPEPtr(pView)->worldToEye(pView);
	
	Point1.transformBy(WorldToEye);
	Point2.transformBy(WorldToEye);
	OdGeVector3d Vector = Point2 - Point1;
	
	if(OdNonZero(Vector.x) && OdNonZero(Vector.y))
	{
		OdGePoint3d NewPosition = Point1 + Vector / 2.0;
    
		Vector.x = fabs(Vector.x);
		Vector.y = fabs(Vector.y);
    
		pView->dolly(NewPosition.asVector());
    
		double wf = pView->fieldWidth() / Vector.x;
		double hf = pView->fieldHeight() / Vector.y;
    
		pView->zoom(odmin(wf, hf));
	
		PostMessage(WM_PAINT);
	}
}

void COpenCADView::OnViewZoomwindow()
{
	// TODO: Add your command handler code here
	m_Points.clear();
	m_bZoomWindow = !m_bZoomWindow;
}

void COpenCADView::OnUpdateViewZoomwindow(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bZoomWindow);
}

void COpenCADView::Track(OdEdInputTracker* pTracker)
{
	if(m_pTracker)
		m_pTracker->removeDrawables(GetView());

	m_pTracker = pTracker;

	if(m_pTracker)
		m_pTracker->addDrawables(GetView());
}

void COpenCADView::Refresh()
{
	// Clean out the tracker
	if(m_pTracker)
		m_pTracker->removeDrawables(GetView());
	m_pTracker = NULL;

	// Refresh the drawing view
	m_pDevice->invalidate();
	PostMessage(WM_PAINT);
}
