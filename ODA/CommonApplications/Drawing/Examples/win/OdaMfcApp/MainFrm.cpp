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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OdaMfcApp.h"

#include "MainFrm.h"
#include "DwgView.h"
#include "DwgViewer.h"
#include "DbAssocActionModule.h"

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(ID_FILE_PARTIAL_OPEN, OnFilePartialOpen)
	ON_COMMAND(ID_FILE_RECOVER, OnFileRecover)
  ON_COMMAND(ID_FILE_MT_OPEN, OnFileMTOpen)
  ON_COMMAND(ID_OPTIONS_DONOTEVALUATEASSOC, OnOptionsDoNotEvaluateAssocNetwork)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DONOTEVALUATEASSOC, OnUpdateOptionsDoNotEvaluateAssocNetwork)
	ON_WM_CLOSE()
  ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
#ifdef ODAMFC_EXPORT_SYMBOL
  ON_COMMAND_RANGE( 34000, 35000, OnReservedItems )
#endif
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	0,
	0,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

#ifdef ODAMFC_EXPORT_SYMBOL
void CMainFrame::OnReservedItems( UINT nID )
{
}
#endif

CMainFrame::CMainFrame() : m_pViewerBar(0)
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

  m_wndStatusBar.SetPaneInfo(0, 0, SBPS_NORMAL, 80);             // guv
  m_wndStatusBar.SetPaneStyle(1, SBPS_OWNERDRAW | SBPS_STRETCH);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

  DragAcceptFiles(TRUE);

	return 0;
}

#define CL_SELECT RGB(255, 0, 0)

void CStatusBarColor::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
  CDC* pDc = CDC::FromHandle(lpDrawItemStruct->hDC);
  pDc->FillRect(&lpDrawItemStruct->rcItem, &CBrush(GetSysColor(COLOR_BTNFACE)));
  pDc->SetBkMode(TRANSPARENT);

  CString text((wchar_t*)lpDrawItemStruct->itemData);

  if (text.Find(L"[")>=0 && text.Find(L"]")>=0)   
    text.Replace(L"/", L"  ");                    // guv: L"/" need for OdaQtApp

  int w  = pDc->GetTextExtent(text).cx;
  int x  = (lpDrawItemStruct->rcItem.right + lpDrawItemStruct->rcItem.left - w)/2.0 + 0.5;
  int y  = 3;
  pDc->TextOutW(x, y, text);

  pDc->SetTextColor(CL_SELECT);
  bool bOut = false;
  for(int i=0; text[i]; i++)
  {
    if (text[i] == L'[') bOut = true;
    if (text[i] == L']') bOut = false;

    if (bOut && (text[i] >= L'A' && text[i] <= L'Z' || text[i] == L'?'))
    {
      w  = pDc->GetTextExtent(text.Left(i)).cx;
      pDc->TextOutW(x + w, y, text.Mid(i, 1));
    }
  }
  pDc->ReleaseOutputDC();
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

BOOL CMainFrame::TempBar::Create(CWnd* pParentWnd)
{
  UINT nStyle = theApp.GetProfileInt(_T("ViewerBar"), _T("style"),
    CBRS_TOP|CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC);
  
  if (!CreateEx(pParentWnd, TBSTYLE_FLAT, WS_CHILD|WS_VISIBLE|nStyle ) ||
      !LoadToolBar(IDR_VIEWER))
  {
    TRACE0("Failed to create toolbar\n");
    return FALSE;
  }
  EnableDocking(CBRS_ALIGN_ANY);
  return TRUE;
}

void CMainFrame::createViewerBar()
{
  if(!m_pViewerBar)
  {
    m_pViewerBar = OdRxObjectImpl<TempBar>::createObject().detach();
    m_pViewerBar->Create(this);
    RECT rc;
    rc.left   = theApp.GetProfileInt(_T("ViewerBar"), _T("left"),   310);
    rc.right  = theApp.GetProfileInt(_T("ViewerBar"), _T("right"),  517);
    rc.top    = theApp.GetProfileInt(_T("ViewerBar"), _T("top"),    0);
    rc.bottom = theApp.GetProfileInt(_T("ViewerBar"), _T("bottom"), 26);
    ClientToScreen(&rc);
    DockControlBar(m_pViewerBar, UINT(0), rc.left!=rc.right && rc.top!=rc.bottom ? &rc : 0);
  }
  else
  {
    m_pViewerBar->addRef();
  }
}

void CMainFrame::releaseViewerBar()
{
  if(m_pViewerBar)
  {
    int n = m_pViewerBar->numRefs();
    if(n==1)
    {
      RECT rc;
      m_pViewerBar->GetWindowRect(&rc);
      ScreenToClient(&rc);
      theApp.WriteProfileInt(_T("ViewerBar"), _T("left"),  rc.left   );
      theApp.WriteProfileInt(_T("ViewerBar"), _T("right"), rc.right  );
      theApp.WriteProfileInt(_T("ViewerBar"), _T("top"),   rc.top    );
      theApp.WriteProfileInt(_T("ViewerBar"), _T("bottom"),rc.bottom );
//      UINT nAlignMask = CBRS_LEFT|CBRS_TOP|CBRS_RIGHT|CBRS_BOTTOM;
      theApp.WriteProfileInt(_T("ViewerBar"), _T("style"), m_pViewerBar->GetBarStyle());
      m_pViewerBar->release();
      m_pViewerBar = 0;
    }
    else
    {
      m_pViewerBar->release();
    }
  }
}

void CMainFrame::OnClose() 
{
  POSITION pos = theApp.GetFirstDocTemplatePosition();
  CDocTemplate* pDocTempl = theApp.GetNextDocTemplate(pos);
  pos = pDocTempl->GetFirstDocPosition();
  while(pos)
  {
    CDocument* pDoc = pDocTempl->GetNextDoc(pos);
    POSITION pos = pDoc->GetFirstViewPosition();
    while (pos)
    {
      if(!pDoc->CanCloseFrame(pDoc->GetNextView(pos)->GetParentFrame()))
        return;
    }
  }

  WINDOWPLACEMENT wp;
  GetWindowPlacement(&wp);

  theApp.WriteProfileInt(_T("MainFrame"), _T("flags"), wp.flags);
  theApp.WriteProfileInt(_T("MainFrame"), _T("showCmd"), wp.showCmd);
  theApp.WriteProfileInt(_T("MainFrame"), _T("left"),  wp.rcNormalPosition.left);
  theApp.WriteProfileInt(_T("MainFrame"), _T("top"),   wp.rcNormalPosition.top);
  theApp.WriteProfileInt(_T("MainFrame"), _T("bottom"),wp.rcNormalPosition.bottom);
  theApp.WriteProfileInt(_T("MainFrame"), _T("right"), wp.rcNormalPosition.right);

  theApp.WriteProfileInt(_T("MainFrame"), _T("Min.x"), wp.ptMinPosition.x);
  theApp.WriteProfileInt(_T("MainFrame"), _T("Min.y"), wp.ptMinPosition.y);

  theApp.WriteProfileInt(_T("MainFrame"), _T("Max.x"), wp.ptMaxPosition.x);
  theApp.WriteProfileInt(_T("MainFrame"), _T("Max.y"), wp.ptMaxPosition.y);

	CMDIFrameWnd::OnClose();
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
  int fileNumber, index;
  TCHAR fileName[_MAX_PATH];

  fileNumber = DragQueryFile(hDropInfo, -1, fileName, _MAX_PATH);

  for (index = 0; index < fileNumber; index++)
  {
    DragQueryFile(hDropInfo, index, fileName, _MAX_PATH);

    AfxGetApp()->OpenDocumentFile(fileName);
  }

  // Release handle memory
  DragFinish(hDropInfo);
}

void CMainFrame::OnOptionsDoNotEvaluateAssocNetwork() {
  OdDbAssocActionModulePtr aam = ::odrxDynamicLinker()->loadModule(DbAssocActionModuleName);
  if (aam.get()) {
    aam->enableAutoEvaluation(!aam->autoEvaluationEnabled());
  }
}

void CMainFrame::OnUpdateOptionsDoNotEvaluateAssocNetwork(CCmdUI* pCmdUI) {
  OdDbAssocActionModulePtr aam = ::odrxDynamicLinker()->loadModule(DbAssocActionModuleName);
  pCmdUI->Enable(aam.get()!=0 ? 1 : 0);
  if (aam.get())
    pCmdUI->SetCheck(aam->autoEvaluationEnabled() ? 0 : 1);
}
