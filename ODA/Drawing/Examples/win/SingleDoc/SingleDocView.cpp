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
/* Implementation of the CSingleDocView class                           */
/************************************************************************/
#include "stdafx.h"
#include "SingleDoc.h"

#include "SingleDocDoc.h"
#include "SingleDocView.h"

#include "GiContextForDbDatabase.h"
#include "DbGsManager.h"
#include "RxVariantValue.h"
#include "DbBlockTableRecord.h"
#include "AbstractViewPE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSingleDocApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSingleDocView

IMPLEMENT_DYNCREATE(CSingleDocView, CView)

BEGIN_MESSAGE_MAP(CSingleDocView, CView)
	//{{AFX_MSG_MAP(CSingleDocView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSingleDocView construction/destruction

CSingleDocView::CSingleDocView()
  : m_interactionMode(CSingleDocView::kNone)
  , m_hSavedCur(0)
{
	// TODO: add construction code here

}

CSingleDocView::~CSingleDocView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSingleDocView drawing

void CSingleDocView::OnDraw(CDC*)
{
}

/////////////////////////////////////////////////////////////////////////////
// CSingleDocView printing

BOOL CSingleDocView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSingleDocView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSingleDocView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSingleDocView diagnostics

#ifdef _DEBUG
void CSingleDocView::AssertValid() const
{
	CView::AssertValid();
}

void CSingleDocView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSingleDocDoc* CSingleDocView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSingleDocDoc)));
	return (CSingleDocDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSingleDocView message handlers

void CSingleDocView::ViewZoomextents() 
{
  /**********************************************************************/
  /* Get the overall viewport                                           */
  /**********************************************************************/
  OdGsView* pView = m_pDevice->viewAt(0);
  
  /**********************************************************************/
  /* Modifies the viewport to fit the extents.                          */
  /**********************************************************************/
  OdAbstractViewPEPtr(pView)->zoomExtents(pView);
}

#define AUTOMATICALLY_ZOOM_TOEXTENTS_WHEN_OPENINGDRAWING


void CSingleDocView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	/**********************************************************************/
	/* Set the database to be vectorized                                  */
	/**********************************************************************/
  OdGiContextForDbDatabase::setDatabase( GetDocument()->m_pDb );
  
	/**********************************************************************/
	/* Set the GS Model flag                                              */
	/**********************************************************************/
  enableGsModel(true) ;
  resetDevice( true )	;
}

void CSingleDocView::resetDevice(bool zoomExtents)
{
  /**********************************************************************/
  /* Get the client rectangle                                           */
  /**********************************************************************/
  CRect rc;
  GetClientRect(&rc);
  /**********************************************************************/
  /* Load the WinGDI module                                             */
  /**********************************************************************/
  OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(OdWinGDIModuleName); 
  /**********************************************************************/
  /* Create a new OdGsDevice object, and associate with the WinGDI      */
  /* GsDevice                                                           */
  /**********************************************************************/
  m_pDevice = pGs->createDevice();
  if(m_pDevice.isNull())
    return;
  /**********************************************************************/
  /* Return a pointer to the dictionary entity containing the device    */
  /* properties                                                         */
  /**********************************************************************/  
  OdRxDictionaryPtr pProperties = m_pDevice->properties();
  /**********************************************************************/
  /* Set the window handle for this GsDevice                            */
  /**********************************************************************/
  pProperties->putAt("WindowHWND", OdRxVariantValue((OdIntPtr)m_hWnd));

  /**********************************************************************/
  /* Define a device coordinate rectangle equal to the client rectangle.*/
  /**********************************************************************/
  OdGsDCRect  gsRect(rc.left, rc.right, rc.bottom, rc.top);

  /************************************************************************/
  /* Set the device background color and palette from the current app.   */
  /************************************************************************/
  m_pDevice->setBackgroundColor(theApp.activeBackground());
  m_pDevice->setLogicalPalette(theApp.curPalette(), 256);

  if( !database() )
    return;
    
  /**********************************************************************/
  /* Set up the views for the active layout                             */
  /**********************************************************************/
  m_pDevice = OdDbGsManager::setupActiveLayoutViews(m_pDevice, this);
  
  /**********************************************************************/
  /* Return true if and only the current layout is a paper space layout.*/
  /**********************************************************************/
  m_bPsOverall = (GetDocument()->m_pDb->getTILEMODE()==0);
  
  /**********************************************************************/
  /* Set the viewport border properties.                                 */
  /**********************************************************************/
  CSingleDocView::setViewportBorderProperties(m_pDevice, !m_bPsOverall);

#ifdef AUTOMATICALLY_ZOOM_TOEXTENTS_WHEN_OPENINGDRAWING
  if(zoomExtents)
    ViewZoomextents();
#endif
  
  /**********************************************************************/
  /* Update the client rectangle.                                       */
  /**********************************************************************/
  OnSize(0, rc.Width(),rc.Height());
  /**********************************************************************/
  /* Redraw the window.                                                 */
  /**********************************************************************/
  RedrawWindow();
}

void CSingleDocView::setViewportBorderProperties(OdGsDevice* pDevice, bool bModel) 
{
  /**********************************************************************/
  /* If current layout is Model, and it has more then one viewport then */
  /* make their borders visible.                                        */
  /* If current layout is Paper, then make visible the borders of all   */
  /* but the overall viewport.                                          */                                              
  /***********************************************************************/
  int n = pDevice->numViews();
  if(n>1)
  {
    for(int i = bModel ? 0 : 1; i<n; ++i)
    {
      /******************************************************************/
      /* Get the viewport                                               */
      /******************************************************************/
      OdGsViewPtr pView = pDevice->viewAt(i);
      /******************************************************************/
      /* Make it visible                                                */
      /******************************************************************/
      pView->setViewportBorderVisibility(true);
      /******************************************************************/
      /* Set the color and width                                        */
      /******************************************************************/
      pView->setViewportBorderProperties(theApp.curPalette()[7], 1);
    }
  }
}
void CSingleDocView::OnPaint() 
{
  CView::OnPaint();
  /**********************************************************************/
  /* Paint the client rectangle with the GS device                      */
  /**********************************************************************/
  if(!m_pDevice.isNull())
    m_pDevice->update();
	
	// Do not call CView::OnPaint() for painting messages
}

void CSingleDocView::OnSize(UINT nType, int cx, int cy) 
{
  if(!m_pDevice.isNull() && cx && cy)
  {
    CRect rc;
    GetClientRect(rc);
    /********************************************************************/
    /* Update the client rectangle                                      */
    /********************************************************************/
    OdGsDCRect outputRect( OdGsDCPoint(rc.left, rc.bottom), OdGsDCPoint(rc.right, rc.top) );
    m_pDevice->onSize(outputRect);
  }
	
}

BOOL CSingleDocView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
