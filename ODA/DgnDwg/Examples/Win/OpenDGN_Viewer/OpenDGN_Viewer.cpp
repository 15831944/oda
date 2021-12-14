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
// OdaDgnApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OpenDGN_Viewer.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "resource.h"
#include "OpenDGN_ViewerDoc.h"
#include "RegKey.h"
#include "DgModelerGeometry/DgModelerModule.h"
#include "Wr/wrTriangulationParams.h"

#include "diagnostics.h"
#include "ColorMapping.h"
#include "DynamicLinker.h"
#include "RxDynamicModule.h"
#include "DbLibraryInfo.h"
#include "SetRegVars.h"
#include "OpenDGN_ViewerVectorizer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _TOOLKIT_IN_DLL_
#include "StaticAppSelDlg.h"
#endif

// COdaDgnApp

BEGIN_MESSAGE_MAP(COdaDgnApp, CWinApp)
  //{{AFX_MSG_MAP(COdaDgnApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  // Standard file based document commands
  ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
  ON_COMMAND(ID_SETREGVARS_MS_SYMBRSRC, OnToolsSetRegVarsMsSymbRsrc)
  ON_COMMAND(ID_SETREGVARS_MS_RSRC, OnToolsSetRegVarsMsRsrc)
  ON_COMMAND(ID_SETREGVARS_FAST_FONT, OnToolsSetRegVarsFastFont)
  ON_UPDATE_COMMAND_UI( ID_VECTORIZE_WINDIRECTX, OnUpdateWinDirectX )
  ON_UPDATE_COMMAND_UI( ID_VECTORIZE_WINGDI, OnUpdateWinGDI )
  ON_UPDATE_COMMAND_UI( ID_VECTORIZE_WINOPENGL, OnUpdateWinOpenGL )
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define AppRegistryKey _T("Open Design Alliance")

// Define module map for statically linked modules:

#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinGDIModule);
#if !defined(WINDIRECTX_DISABLED)
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinDirectXModule);
#endif //WINDIRECTX_DISABLED
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRasterProcessingServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);   //this library allows to read V7 files

#if defined TDRAWING_ENABLE

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDwgDbModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnLSModule);

#if defined DGNIMPORT_ENABLE
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgnImportModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgImportLineStyleModule);
#endif

#endif


ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_Db",                OdDgnModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry",   DgModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinGDIModuleName,      WinGDIModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinOpenGLModuleName,   WinOpenGLModule)
#if !defined(WINDIRECTX_DISABLED)
  ODRX_DEFINE_STATIC_APPMODULE(OdWinDirectXModuleName,  WinDirectXModule)
#endif //WINDIRECTX_DISABLED
  ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName,   BitmapModule)
  ODRX_DEFINE_STATIC_APPMODULE(RX_RASTER_SERVICES_APPNAME,     ExRasterModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdRasterProcessorModuleName,      OdRasterProcessingServicesImpl)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO",          OdDgn7IOModuleImpl)
#if defined TDRAWING_ENABLE
  ODRX_DEFINE_STATIC_APPMODULE(OdDbModuleName, OdDwgModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_DwgDb", OdDwgDbModuleImpl)
  ODRX_DEFINE_STATIC_APPMODULE(OdModelerGeometryModuleName, ModelerModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdDgnLSModuleName, OdDgnLSModule)
#if defined DGNIMPORT_ENABLE
  ODRX_DEFINE_STATIC_APPMODULE(L"TD_DgnImport", DgnImportModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"ExDgnImportLineStyle", OdDgImportLineStyleModule)
#endif

#endif
ODRX_END_STATIC_MODULE_MAP()

#endif


// COdaDgnApp construction

COdaDgnApp::COdaDgnApp()
  : m_bLoading(false)
  , m_pDbHostApp(0)
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}


// The one and only COdaDgnApp object
COdaDgnApp theApp;
/************************************************************************/
/* Create a Custom Db HostAppServices class                            */
/************************************************************************/
#ifdef TDRAWING_ENABLED
OdStaticRxObject<MyDbHostAppServices> dbSvcs;
#endif


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

// COdaDgnApp initialization

BOOL COdaDgnApp::InitInstance()
{
//TODO: call AfxInitRichEdit2() to initialize richedit2 library.
#if _MSC_VER < 1600
  _set_sbh_threshold(1016);
#endif

  setlocale( LC_NUMERIC, "C" ); // saving doubles to dxf
  setlocale( LC_TIME, "" ); // correct strftime formatting

  // Change the registry key under which our settings are stored.
  SetRegistryKey(AppRegistryKey);

  settings().load(this);

  // Customize diagnostics:
  ::odSetTraceFunc(myTrace);
  ::odSetAssertFunc(myAssert);
  try
  {
#ifdef _DEBUG
    // This will cause OdaMfcApp to crash if it will perform illegal FP operation
    //_clear87();
    //_control87(
    //  //              off on
    //  //_EM_INVALID       |
    //  //_EM_DENORMAL      |
    //  //_EM_ZERODIVIDE    |
    //  //_EM_OVERFLOW      |
    //  //_EM_UNDERFLOW     |
    //  //_EM_INEXACT       ,
    //  //_MCW_EM
    //  );
#endif

#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif
    OdStringArray arrayRscList;
    
    //OdString sRscPath("C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Symb\\font.rsc");
    OdString sRscPath( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_SYMBRSRC"), _T("")) );
    if ( !sRscPath.isEmpty() )
    {
      arrayRscList.append( sRscPath );
      setMS_SYMBRSRC( arrayRscList );
    }

    //OdString sRsrc("C:\\Program Files\\Bentley\\MicroStation\\ustation.rsc");
    OdString sRsrc( AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_RSRC"), _T("")) );
    if ( !sRsrc.isEmpty() )
    {
      setMS_RSRC( sRsrc );
    }

    OdString sFastFont = AfxGetApp()->GetProfileString(_T("RegVars"), _T("FAST_FONT"), _T(""));
    if ( !sFastFont.isEmpty() )
    {
      setFAST_FONT( sFastFont );
    }

#ifdef TDRAWING_ENABLED
    setDbHostAppServices(&dbSvcs);
    dbSvcs.setDgnHostAppServices(this);
#endif

    // Initialize Runtime Extension environment
    odrxInitialize(this);

    // Register ODA Drawings API for DGN
    ::odrxDynamicLinker()->loadModule(L"TG_Db", false);
#ifdef TDRAWING_ENABLED
    // Register ODA Drawings SDK
    ::odrxDynamicLinker()->loadModule(OdDbModuleName, false); // for instance, to draw .dwg file XRef
    ::odrxDynamicLinker()->loadModule(L"TG_DwgDb", false);
    // Dgn level table overrides for dwg reference attachments support
    ::odrxDynamicLinker()->loadModule(L"ExDgnImportLineStyle");
#endif

    // check GS modules accessibility
    //
    OdGsModulePtr pGs = ::odrxDynamicLinker()->loadModule(OdWinDirectXModuleName, true);
    if ( !pGs.isNull() )
    {
      settings().setWinDirectXFlag(true);
      ::odrxDynamicLinker()->unloadModule(OdWinDirectXModuleName);
    }

    pGs = ::odrxDynamicLinker()->loadModule(OdWinGDIModuleName, true);
    if ( !pGs.isNull() )
    {
      settings().setWinGDIFlag(true);
      ::odrxDynamicLinker()->unloadModule(OdWinGDIModuleName);
    }
    pGs = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName, true);
    if ( !pGs.isNull() )
    {
      settings().setWinOpenGLFlag(true);
      ::odrxDynamicLinker()->unloadModule(OdWinOpenGLModuleName);
    }

//    m_dumper.init();
  }
  catch (OdError& err)
  {
    reportError(_T("ODA Drawings API for DGN Initialization error"), err);
    return FALSE;
  }
#ifndef _DEBUG
  catch (...)
  {
    OdError err(eNotHandled);
    reportError(_T("ODA Drawings API for DGN Initialization error"), err);
    return FALSE;
  }
#endif


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

  // Initialize OLE libraries
  if (!AfxOleInit())
  {
    AfxMessageBox(IDP_OLE_INIT_FAILED);
    return FALSE;
  }
  AfxEnableControlContainer();
  // Standard initialization
  // If you are not using these features and wish to reduce the size
  // of your final executable, you should remove from the following
  // the specific initialization routines you do not need

  LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views
  CMultiDocTemplate* pDocTemplate;
  pDocTemplate = new CMultiDocTemplate(IDR_OPENDGN_VIEWER_TYPE,
    RUNTIME_CLASS(COdaDgnAppDoc),
    RUNTIME_CLASS(CChildFrame), // custom MDI child frame
    RUNTIME_CLASS(COdaDgnAppVectorizer));
  if (!pDocTemplate)
    return FALSE;
  AddDocTemplate(pDocTemplate);

  // create main MDI Frame window
  CMainFrame* pMainFrame = new CMainFrame;
  if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
  {
    delete pMainFrame;
    return FALSE;
  }
  m_pMainWnd = pMainFrame;
  // call DragAcceptFiles only if there's a suffix
  //  In an MDI app, this should occur immediately after setting m_pMainWnd


  // Parse command line for standard shell commands, DDE, file open
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);
  if ( cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew ) 
    cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;


  // Dispatch commands specified on the command line.  Will return FALSE if
  // app was launched with /RegServer, /Register, /Unregserver or /Unregister.
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  WINDOWPLACEMENT wp;
  BOOL bRes = pMainFrame->GetWindowPlacement(&wp);

  wp.flags                  = GetProfileInt(_T("MainFrame"), _T("flags"),   wp.flags                  );
  wp.showCmd                = GetProfileInt(_T("MainFrame"), _T("showCmd"), wp.showCmd                );
  wp.rcNormalPosition.left  = GetProfileInt(_T("MainFrame"), _T("left"),    wp.rcNormalPosition.left  );
  wp.rcNormalPosition.top   = GetProfileInt(_T("MainFrame"), _T("top"),     wp.rcNormalPosition.top   );
  wp.rcNormalPosition.bottom= GetProfileInt(_T("MainFrame"), _T("bottom"),  wp.rcNormalPosition.bottom);
  wp.rcNormalPosition.right = GetProfileInt(_T("MainFrame"), _T("right"),   wp.rcNormalPosition.right );

  wp.ptMinPosition.x        = GetProfileInt(_T("MainFrame"), _T("Min.x"),   wp.ptMinPosition.x        );
  wp.ptMinPosition.y        = GetProfileInt(_T("MainFrame"), _T("Min.y"),   wp.ptMinPosition.y        );

  wp.ptMaxPosition.x        = GetProfileInt(_T("MainFrame"), _T("Max.x"),   wp.ptMaxPosition.x        );
  wp.ptMaxPosition.y        = GetProfileInt(_T("MainFrame"), _T("Max.y"),   wp.ptMaxPosition.y        );

  bRes = pMainFrame->SetWindowPlacement(&wp);

  // The main window has been initialized, so show and update it
  pMainFrame->ShowWindow(m_nCmdShow);
  pMainFrame->UpdateWindow();

  return TRUE;
}

int COdaDgnApp::ExitInstance()
{
  try
  {
//    m_dumper.uninit();

    settings().save(this);

    ::odrxUninitialize();
  }
  catch(const OdError& e)
  {
    theApp.reportError(_T(""), e);
  }
  return CWinApp::ExitInstance();
}

OdDgDatabasePtr COdaDgnApp::openFile(LPCTSTR lpszPathName)
{
  // Open an existing document
  OdDgDatabasePtr pDb;
  CMainFrame* pMFrame = (CMainFrame*)GetMainWnd();
  pMFrame->StartTimer();
  try
  {
    CWaitCursor wait;
    m_bLoading = true;
    pDb = readFile(OdString(lpszPathName));
    m_bLoading = false;
    pMFrame->StopTimer(_T("Loading"));
  }
  catch(const OdError& e)
  {
    setStatusText(0, _T(""));
    pDb = 0;
    reportError(_T("Loading Error..."), e);
  }
  catch(const COdaDgnAppUserBreak&)
  {
    setStatusText(0, _T(""));
    setStatusText(_T("Operation was canceled by user."));
  }
  m_bLoading = false;

  return pDb;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

// Dialog Data
  enum { IDD = IDD_ABOUTBOX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

// Implementation
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

BOOL CAboutDlg::OnInitDialog()
{
  OdDbLibraryInfo*  libInfo = oddgGetLibraryInfo();
  OdString  sLib = libInfo->getBuildComments();
  OdString  sCopyright = libInfo->getCopyright();

  CDialog::OnInitDialog();

  SetDlgItemText(IDC_INF_COPYRIGHT, sCopyright);
  SetDlgItemText(IDC_INF_LIBRARY, sLib);

  return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void COdaDgnApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}


// COdaDgnApp message handlers


void COdaDgnApp::OnFileOpen()
{
  CString sPath = BrowseWithPreview(GetMainWnd()->GetSafeHwnd(),
    _T("DGN files (*.dgn)|*.dgn|All Files (*.*)|*.*||"));
  if ( sPath.GetLength() )
  {
    OpenDocumentFile(sPath);
//    GetMainWnd()->SendMessage(WM_COMMAND, ID_VECTORIZE);
  }
}

CString COdaDgnApp::BrowseWithPreview(HWND hWndParent, LPCTSTR caFilter)
{
  CString sFileBuff;
  {    // use alternate method for OpenFile dialog
    OPENFILENAME  ofn;

    // replace the '|' character with '\0'
    CString filter;
    long l = (long)_tcslen(caFilter), l2 = l;
    LPTSTR buff = filter.GetBuffer(l);
    while (--l >= 0)
    {
      if (caFilter[l] == '|')
        buff[l] = '\0';
      else
        buff[l] = caFilter[l];
    }
    filter.ReleaseBuffer(l2);

    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWndParent;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = sFileBuff.GetBuffer(_MAX_PATH);
    ofn.nMaxFile = _MAX_PATH;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR |OFN_PATHMUSTEXIST;

    GetOpenFileName(&ofn);
    sFileBuff.ReleaseBuffer();
  }
  return sFileBuff;
}


void COdaDgnApp::OnToolsSetRegVarsMsSymbRsrc()
{
  CSetRegVarsMsSymbRsrc appDlg;
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsMsRsrc()
{
  CSetRegVarsMsRsrc appDlg;
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}

void COdaDgnApp::OnToolsSetRegVarsFastFont()
{
  CSetRegVarsFastFont appDlg;
  appDlg.setHostAppServices(this);
  appDlg.DoModal();
}


int FindMenuItem(CMenu* Menu, CString& MenuString)
{
   ASSERT(Menu);
   ASSERT(::IsMenu(Menu->GetSafeHmenu()));

   int count = Menu->GetMenuItemCount();
   for (int i = 0; i < count; i++)
   {
      CString strMenuItem;
      if (Menu->GetMenuString(i, strMenuItem, MF_BYPOSITION) &&
          (strMenuItem.Compare( MenuString )== 0))
      {
         return i;
      }
   }

   return -1;
}

CMenu* getVectorizeMenu2(CMenu* pMainMenu) 
{
  // Look for "Vectorize" pop-up menu item.
  CString sPopUpMenuItem("Vectori&ze");
  int pos = FindMenuItem(pMainMenu, sPopUpMenuItem );
  if (pos == -1)
    return NULL;
  return pMainMenu->GetSubMenu(pos);
}

const ODCOLORREF* COdaDgnApp::curPalette() const
{
  return odcmAcadPalette(settings().getActiveBackground());
}

void COdaDgnApp::setStatusText(LPCTSTR msg)
{
  ((CMainFrame*)GetMainWnd())->setStatusText(msg);
}

void COdaDgnApp::setStatusText(int nCol, LPCTSTR msg)
{
  ((CMainFrame*)GetMainWnd())->setStatusText(nCol, msg);
}

void COdaDgnApp::warning(const OdString& sm)
{
  if ( m_bLoading )
  {
    OdString sMsg(sm);
    sMsg += _T("\n\nDo you want to proceed ?");
    if (MessageBox (NULL, sMsg, _T("Warning!"), MB_ICONWARNING|MB_YESNO) == IDNO )
      throw COdaDgnAppUserBreak();
  }
}

afx_msg void COdaDgnApp::OnUpdateWinDirectX(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( settings().getWinDirectXFlag() );

  if (pCmdUI->m_pMenu)
  {
    pCmdUI->m_pMenu->CheckMenuItem( pCmdUI->m_nIndex, 
                                    (pCmdUI->m_nID == settings().getActiveGS()) ? MF_BYPOSITION|MF_CHECKED : MF_BYPOSITION|MF_UNCHECKED 
                                   );
  }
}
afx_msg void COdaDgnApp::OnUpdateWinGDI(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( settings().getWinGDIFlag() );

  if (pCmdUI->m_pMenu)
  {
    pCmdUI->m_pMenu->CheckMenuItem( pCmdUI->m_nIndex, 
                                    (pCmdUI->m_nID == settings().getActiveGS()) ? MF_BYPOSITION|MF_CHECKED : MF_BYPOSITION|MF_UNCHECKED 
                                   );
  }
}
afx_msg void COdaDgnApp::OnUpdateWinOpenGL(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable( settings().getWinOpenGLFlag() );

  if (pCmdUI->m_pMenu)
  {
    pCmdUI->m_pMenu->CheckMenuItem( pCmdUI->m_nIndex, 
                                    (pCmdUI->m_nID == settings().getActiveGS()) ? MF_BYPOSITION|MF_CHECKED : MF_BYPOSITION|MF_UNCHECKED 
                                   );
  }
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
