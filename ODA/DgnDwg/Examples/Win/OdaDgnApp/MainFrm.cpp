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
#include "OdaDgnApp.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
  ON_WM_CREATE()
  ON_WM_CLOSE()
  ON_WM_DROPFILES()
  ON_WM_QUERYDRAGICON()
  ON_WM_SYSCOMMAND()
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


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  // TODO: add member initialization code here
  m_pc0.QuadPart = 0;
  m_pc0.QuadPart = 0;
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

  // TODO: Delete these three lines if you don't want the toolbar to be dockable
  m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
  EnableDocking(CBRS_ALIGN_ANY);
  DockControlBar(&m_wndToolBar);

  if (!m_wndStatusBar.Create(this) ||
    !m_wndStatusBar.SetIndicators(indicators,
      sizeof(indicators)/sizeof(UINT)))
  {
    TRACE0("Failed to create status bar\n");
    return -1;      // fail to create
  }

  DragAcceptFiles(TRUE);

  return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
  if( !CMDIFrameWnd::PreCreateWindow(cs) )
    return FALSE;
  // TODO: Modify the Window class or styles here by modifying
  //  the CREATESTRUCT cs

  return TRUE;
}


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

void CMainFrame::OnClose() 
{
  WINDOWPLACEMENT wp;
  GetWindowPlacement(&wp);

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

// CMainFrame message handlers



