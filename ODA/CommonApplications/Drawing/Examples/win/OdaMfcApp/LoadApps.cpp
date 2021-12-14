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

// LoadApps.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "LoadApps.h"
#include "RxModule.h"

#ifndef _TOOLKIT_IN_DLL_
#include "StaticAppSelDlg.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CLoadApps dialog

CLoadApps::LoadedApps* CLoadApps::m_pLoadedApps = 0;

void CLoadApps::rxInit()
{
  m_pLoadedApps = new LoadedApps;
  odrxDynamicLinker()->addReactor(m_pLoadedApps);
}

void CLoadApps::rxUninit()
{
  odrxDynamicLinker()->removeReactor(m_pLoadedApps);
  delete m_pLoadedApps;
}

CLoadApps::CLoadApps(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadApps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadApps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CLoadApps::LoadedApps::rxAppLoaded(OdRxModule* pAppModule)
{
  append(pAppModule->moduleName());
  if(m_pListBox)
  {
    m_pListBox->SetCurSel(m_pListBox->AddString(pAppModule->moduleName()));
  }
}

void CLoadApps::LoadedApps::rxAppUnloaded(const OdString& szAppName)
{
  remove(szAppName);
  if(m_pListBox)
  {
    int n = m_pListBox->FindString(0, OdString(szAppName));
    if(n != LB_ERR )
    {
      m_pListBox->DeleteString(n);
    }
  }
}

void CLoadApps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadApps)
	DDX_Control(pDX, IDC_UNLOAD_APP, m_UnloadBtn);
	DDX_Control(pDX, IDC_APPS_LIST, m_AppsList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoadApps, CDialog)
	//{{AFX_MSG_MAP(CLoadApps)
	ON_BN_CLICKED(IDC_LOAD_APP, OnLoadApp)
	ON_BN_CLICKED(IDC_UNLOAD_APP, OnUnloadApp)
	ON_LBN_SETFOCUS(IDC_APPS_LIST, OnAppsListEvent)
	ON_LBN_SELCHANGE(IDC_APPS_LIST, OnAppsListEvent)
	ON_LBN_KILLFOCUS(IDC_APPS_LIST, OnAppsListEvent)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadApps message handlers

BOOL CLoadApps::OnInitDialog() 
{
  CDialog::OnInitDialog();
	
  m_AppsList.ResetContent();

  m_pLoadedApps->m_pListBox = &m_AppsList;

  for(OdUInt32 i=0; i<m_pLoadedApps->size(); ++i)
  {
    int n = m_AppsList.AddString(m_pLoadedApps->at(i));
    OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(m_pLoadedApps->at(i));
    m_AppsList.SetItemData(n, (LPARAM)pModule.get());
  }

  OnAppsListEvent();
	
  return TRUE;
}

void CLoadApps::OnLoadApp() 
{
#ifdef _TOOLKIT_IN_DLL_
  CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
    _T("Teigha Run-time Extention (*.dll,*.tx)|*.dll;*.tx|Any file (*.*)|*.*||"), this);

  dlg.m_ofn.lpstrTitle = _T("Load application");
  CString s_path = COdaMfcAppApp::getApplicationPath();
  dlg.m_ofn.lpstrInitialDir = s_path.GetBuffer(s_path.GetLength());
#else
  CStaticAppSelDlg dlg(this);
#endif //#ifdef _TOOLKIT_IN_DLL_
  if ( dlg.DoModal() == IDOK)
  {
    try
    {
      ::odrxDynamicLinker()->loadModule(OdString((LPCTSTR)dlg.GetPathName()), false);
    }
    catch(const OdError& e)
    {
      theApp.reportError(m_hWnd, _T("Error"), e);
    }
  }

  OnAppsListEvent();
}

void CLoadApps::OnUnloadApp() 
{
  int nIndex = m_AppsList.GetCurSel();
  if (nIndex != LB_ERR)
  {
    CString s;
    m_AppsList.GetText(nIndex, s);
    if(::odrxDynamicLinker()->unloadModule(OdString((LPCTSTR)s)))
    {
      //m_AppsList.DeleteString(nIndex);
      if(m_AppsList.GetCount()<=nIndex)
      {
        nIndex = m_AppsList.GetCount()-1;
      }
      m_AppsList.SetCurSel(nIndex);
    }
    else
    {
      theApp.messageBox(_T("Failed"),_T("Module is referenced."), MB_OK);
    }
  }
  OnAppsListEvent();
}

void CLoadApps::OnAppsListEvent()
{
  m_UnloadBtn.EnableWindow(m_AppsList.GetCurSel() != LB_ERR);
}

void CLoadApps::OnDestroy() 
{
  m_pLoadedApps->m_pListBox = 0;
	CDialog::OnDestroy();
}
