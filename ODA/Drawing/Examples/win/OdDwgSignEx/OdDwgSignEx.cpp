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

// OdDwgSignEx.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OdDwgSignEx.h"
#include "OdDwgSignExDlg.h"

#include <malloc.h>

#include "diagnostics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COdDwgSignExApp

BEGIN_MESSAGE_MAP(COdDwgSignExApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp) 
END_MESSAGE_MAP()


// COdDwgSignExApp construction

COdDwgSignExApp::COdDwgSignExApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only COdDwgSignExApp object

COdDwgSignExApp theApp;


/************************************************************************/
/* Simple trace function                                                */
/************************************************************************/
static void myTrace(const OdChar* debugString)
{
#ifndef NDEBUG
  OutputDebugStringW((LPCWSTR)debugString);
#endif
}

/************************************************************************/
/* Displays MessageBox for Assertion failure                            */
/************************************************************************/
static void myAssert(const char* expr, const char* fileName, int nLine)
{
  USES_CONVERSION;
  char sMsg[MAX_PATH*3];
  sprintf(sMsg, "expression: \"%s\"\n\nfile: \"%s\"\nline: %d",
    expr, fileName, nLine);

  switch(MessageBox(NULL, A2CT(sMsg), _T("Assertion failed!"), MB_ICONWARNING|MB_ABORTRETRYIGNORE))
  {
  case IDABORT:
    exit(0);
    break;
  case IDRETRY:
    DebugBreak();
    break;
  case IDIGNORE:
  default:
    break;
  }
}

static bool myCheckAssertGroup(const char* pGroup)
{
  return !pGroup || !*pGroup; // asserts of base group are enabled only (default)
  //return true; // asserts of all groups are enabled
}


// Define module map for statically linked modules:
#if !defined(_TOOLKIT_IN_DLL_)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbPointCloudObjModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GeolocationObjModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdGeoDataModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDbGeoMapPEModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DbCryptModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExTimeProviderWin);

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,   ModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE  (OdWinBitmapModuleName,         BitmapModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRasterProcessorModuleName,   OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPLICATION(DbPointCloudObjModuleName,     DbPointCloudObjModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdGeolocationObjModuleName,    GeolocationObjModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdGeoDataModuleName,           OdGeoDataModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdGeoMapPEModuleName,          OdDbGeoMapPEModule)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,      ExRasterModule)
  ODRX_DEFINE_STATIC_APPMODULE(DbCryptModuleName,            DbCryptModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdTimeProviderModuleName, ExTimeProviderWin)
  ODRX_END_STATIC_MODULE_MAP()
#endif


// COdDwgSignExApp initialization

BOOL COdDwgSignExApp::InitInstance()
{
  // Customize diagnostics:
  ::odSetTraceFunc(myTrace);
  ::odSetAssertFunc(myAssert);
  ::odSetCheckAssertGroupFunc(myCheckAssertGroup);

  try
  {
#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif
    // Initialize ODA Drawings
    odInitialize(&m_signatureApp);

    //load DbCryptModule:
    ::odrxDynamicLinker()->loadModule(DbCryptModuleName);
  }
  catch (OdError& err)
  {
    reportError(_T("Teigha Initialization error"), err);
    return FALSE;
  }

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	AfxEnableControlContainer();

  COdDwgSignExDlg dlg;

  OdRxClassPtr pService = odrxServiceDictionary()->getAt(OD_T("OdCryptoServices"));
  if (!pService.isNull())
  {
    OdCryptoServicesPtr pCryptoServices = pService->create();

    //init suitable certificates list:
    OdArray<OdCertificateDescription> certDescArray;
    int certsNumber = 0;
    certsNumber = pCryptoServices->getPersonalCertsWithTrustedStatus(certDescArray);
    if (certsNumber)
    {    	
      m_pMainWnd = &dlg;

      dlg.m_pSignatureApp = &m_signatureApp;

      dlg.m_certDescArray = certDescArray;

      dlg.DoModal();
    }
    else
    {
      messageBox(L"OdDwgSignEx", L"No valid digital ID (Certificate) is available on your system", MB_OK);
    }
  }
  else
  {    
    messageBox(L"OdDwgSignEx", L"OdCryptoServices have not been initialized to process signatures!", MB_OK);
  }

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int COdDwgSignExApp::ExitInstance()
{
  try
  {
    ::odUninitialize();
  }
  catch(const OdError& e)
  {
    theApp.reportError(_T(""), e);
  }

  return CWinApp::ExitInstance();  
}

/////////////////////////////////////////////////////////////////////////////
#ifdef _MT
#ifndef _DLL

// For MT configurations only
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern "C" {

  ALLOCDLL_EXPORT void* odrxAlloc(size_t s)
  {
    return ::malloc(s);
  }

  ALLOCDLL_EXPORT void* odrxRealloc(void* p, size_t new_size, size_t /*old_size*/)
  {
    return ::realloc(p, new_size);
  }

  ALLOCDLL_EXPORT void odrxFree(void* p) 
  {
    ::free(p);
  }

} // extern "C"
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#endif
#endif
