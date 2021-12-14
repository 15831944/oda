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
// StaticAppSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "StaticAppSelDlg.h"
#include "RxDynamicModule.h"
#include "TDVersion.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef _TOOLKIT_IN_DLL_

/////////////////////////////////////////////////////////////////////////////
// CStaticAppSelDlg dialog


CStaticAppSelDlg::CStaticAppSelDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CStaticAppSelDlg::IDD, pParent)
{
  //{{AFX_DATA_INIT(CStaticAppSelDlg)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}


void CStaticAppSelDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CStaticAppSelDlg)
  DDX_Control(pDX, IDC_APPLIST, m_appList);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStaticAppSelDlg, CDialog)
  //{{AFX_MSG_MAP(CStaticAppSelDlg)
  ON_NOTIFY(NM_CLICK, IDC_APPLIST, OnClickApplist)
  ON_NOTIFY(NM_DBLCLK, IDC_APPLIST, OnDblclkApplist)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticAppSelDlg message handlers

extern STATIC_MODULE_DESC g_ODRX_STATIC_MODULE_MAP[];

BOOL CStaticAppSelDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();

  HICON hIcon = 0;
  if(::ExtractIconEx(_T("shell32.dll"), 72, NULL, &hIcon, 1)==1)
  {
    if(m_icons.Create(16, 16, ILC_COLOR4, 1, 1))
    {
      m_icons.SetBkColor(::GetSysColor(COLOR_WINDOW));
      m_icons.Add(hIcon);
      m_appList.SetImageList(&m_icons, LVSIL_SMALL);
    }
  }
  STATIC_MODULE_DESC* pMDesc = g_ODRX_STATIC_MODULE_MAP;
  while(pMDesc->entryPoint)
  {
    CString drxName(pMDesc->szAppName);
    if(drxName.Find('.')==-1)
      drxName += DEFAULT_MODULE_EXTENSION_W;

    m_appList.InsertItem(0, drxName, 0);
    ++pMDesc;
  }
 
  return TRUE;
}

CString CStaticAppSelDlg::GetPathName()
{
  return m_sPath;
}

CString CStaticAppSelDlg::GetFileName()
{
  return m_sPath;
}

void CStaticAppSelDlg::OnClickApplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
  int n = m_appList.GetSelectionMark();
  if(n > -1)
  {
    m_sPath = m_appList.GetItemText(n, 0);
  }
  else
  {
    m_sPath.Empty();
  }
  *pResult = 1;
}

void CStaticAppSelDlg::OnDblclkApplist(NMHDR* pNMHDR, LRESULT* pResult) 
{
  if(!m_sPath.IsEmpty())
  {
    EndModalLoop(IDOK);
  }
  *pResult = 1;
}

#endif  //#ifndef _TOOLKIT_IN_DLL_
