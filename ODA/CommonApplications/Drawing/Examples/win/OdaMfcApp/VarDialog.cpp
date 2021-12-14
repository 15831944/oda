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

// FileDependencyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VarDialog.h"
//#include "odamfcapp.h"
//#include "DwgView.h"
//#include "OdaMfcAppDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVarDialog::CVarDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd /*= NULL*/)
	: CDialog(lpszTemplateName, pParentWnd)
{
	//{{AFX_DATA_INIT(CVarDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
   m_bInitialized = FALSE;
}

CVarDialog::CVarDialog(UINT nIDTemplate, CWnd* pParentWnd /*=NULL*/)
	: CDialog(nIDTemplate, pParentWnd)
{
	//{{AFX_DATA_INIT(CVarDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
   m_bInitialized = FALSE;
}

BEGIN_MESSAGE_MAP(CVarDialog, CDialog)
	//{{AFX_MSG_MAP(CVarDialog)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CVarDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
  m_bInitialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVarDialog::initResizeHelper()
{
  ///////////////////////////////////////////////////////////////////////////
  //////////// Initialize the resize help data member ///////////////////////
  ///////////////////////////////////////////////////////////////////////////
  CRect rect;
  GetWindowRect(rect);
  m_origSize = CPoint(rect.Width(), rect.Height());
  
  // initialize resize helper
  m_resizeHelper.Init(m_hWnd);

  ///////////////////////////////////////////////////////////////////////////
  //////////////// Create and setup the gripper for resizing ////////////////
  ///////////////////////////////////////////////////////////////////////////
  makeGripper();
}

void CVarDialog::makeGripper()
{
   // Create a resize gripper
   CRect initRect;
   GetClientRect(initRect);
   initRect.left = initRect.right - GetSystemMetrics(SM_CXHSCROLL);
   initRect.top  = initRect.bottom - GetSystemMetrics(SM_CYVSCROLL);
   
   DWORD dwStyle = WS_CHILD | 
      SBS_SIZEBOX | 
      SBS_SIZEBOXBOTTOMRIGHTALIGN | 
      SBS_SIZEGRIP | 
      WS_VISIBLE;
   
   m_Grip.Create(dwStyle,initRect, this, AFX_IDW_SIZE_BOX);
}

void CVarDialog::OnSize(UINT nType, int cx, int cy) 
{
   CDialog::OnSize(nType, cx, cy);
   m_resizeHelper.OnSize();
   if (m_bInitialized)
      SetupGripper();
}

void CVarDialog::SetupGripper()
{
    CRect tempRect;
    GetClientRect(&tempRect);

    WINDOWPLACEMENT wp;
    GetWindowPlacement(&wp);
    BOOL bMaximized = (wp.showCmd == SW_MAXIMIZE);
    // Do not show the sizing gripper
    if (bMaximized)
        m_Grip.ShowWindow(SW_HIDE);
    else
        m_Grip.ShowWindow(SW_SHOW);

    // Setup the gripper
    if (!bMaximized)
    {
        CWnd *resizeWnd = GetDlgItem(AFX_IDW_SIZE_BOX);
	    if (resizeWnd) 
        {
	        CRect rect, rect2;
	        resizeWnd->GetWindowRect(&rect);
	        GetClientRect(rect2);
	        rect2.left = rect2.right - rect.Width();
	        rect2.top  = rect2.bottom - rect.Height();
	        resizeWnd->MoveWindow(&rect2);
	    }
    }
}
