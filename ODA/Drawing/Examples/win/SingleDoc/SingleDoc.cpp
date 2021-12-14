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
/* This SDI application allows you to open, view and save DWG files.    */
/* The drawing window is resized with the application window.           */
/************************************************************************/

/************************************************************************/
/* Defines the class behaviors for the application.                     */
/************************************************************************/
#include "stdafx.h"
#include "SingleDoc.h"

#include "MainFrm.h"
#include "SingleDocDoc.h"
#include "SingleDocView.h"
#include "ColorMapping.h"
#include "RxDynamicModule.h"
#include "summinfo.h"

#ifdef ENABLE_ACIS_SUPPORT
#include "ModelerModule.h"
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#if defined(UNICODE) || defined (_UNICODE) || !defined(_MBCS)
#error "This example must be MBCS"
#endif

#define AppRegistryKey _T("Open Design Alliance")

/////////////////////////////////////////////////////////////////////////////
// CSingleDocApp
#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinGDIModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(WinOpenGLModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExRasterModule);
#ifdef ENABLE_ACIS_SUPPORT
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
#endif

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPMODULE(OdWinGDIModuleName,     WinGDIModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
//  ODRX_DEFINE_STATIC_APPLICATION(OdWinOpenGLModuleName,        WinOpenGLModule)
//  ODRX_DEFINE_STATIC_APPLICATION(RX_RASTER_SERVICES_APPNAME, ExRasterModule)
#ifdef ENABLE_ACIS_SUPPORT
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName,  ModelerModule)
#endif
ODRX_END_STATIC_MODULE_MAP()

#endif


BEGIN_MESSAGE_MAP(CSingleDocApp, CWinApp)
  //{{AFX_MSG_MAP(CSingleDocApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
  //}}AFX_MSG_MAP
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSingleDocApp construction

CSingleDocApp::CSingleDocApp()
{
  // TODO: add construction code here,
  // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSingleDocApp object

CSingleDocApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSingleDocApp initialization

BOOL CSingleDocApp::InitInstance()
{
  AfxEnableControlContainer();

  // Standard initialization
  // If you are not using these features and wish to reduce the size
  //  of your final executable, you should remove from the following
  //  the specific initialization routines you do not need.

#ifdef _AFXDLL
  Enable3dControls();     // Call this when using MFC in a shared DLL
#else
  Enable3dControlsStatic(); // Call this when linking to MFC statically
#endif

  // Change the registry key under which our settings are stored.
  // TODO: You should modify this string to be something appropriate
  // such as the name of your company or organization.
  SetRegistryKey(AppRegistryKey);

  LoadStdProfileSettings();  // Load standard INI file options (including MRU)

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif

  /**********************************************************************/
  /* Initialize the Drawings SDK                                        */
  /**********************************************************************/
  try 
  {
    odInitialize(this);
  }
  catch (OdError& /*err*/)
  {
    ::AfxMessageBox( _T("Teigha Initialization error") );//, err);
    return FALSE;
  }
  catch (...)
  {
    ::AfxMessageBox( _T("Teigha Initialization error") );//, eNotHandled);
    return FALSE;
  }

  /**********************************************************************/
  /* Register the application's document templates.  Document templates */
  /* serve as the connection between documents, frame windows and view  */
  /**********************************************************************/
  CSingleDocTemplate* pDocTemplate;
  pDocTemplate = new CSingleDocTemplate(
    IDR_MAINFRAME,
    RUNTIME_CLASS(CSingleDocDoc),
    RUNTIME_CLASS(CMainFrame),       // main SDI frame window
    RUNTIME_CLASS(CSingleDocView));
  AddDocTemplate(pDocTemplate);

  EnableShellOpen();
  RegisterShellFileTypes(TRUE);
  
  /**********************************************************************/
  /* Parse command line for standard shell commands, DDE, file open     */
  /**********************************************************************/
  CCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);

  /**********************************************************************/
  /* Dispatch commands specified on the command line                    */
  /**********************************************************************/
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;

  /**********************************************************************/
  /* The only window has been initialized, so show and update it.       */
  /**********************************************************************/
  m_pMainWnd->ShowWindow(SW_SHOW);
  m_pMainWnd->UpdateWindow();

  return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();

// Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();  
  //}}AFX_VIRTUAL

// Implementation
protected:
  //{{AFX_MSG(CAboutDlg)
    // No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CAboutDlg)
  //}}AFX_DATA_MAP
}

BOOL CAboutDlg::OnInitDialog()
{
  OdDbLibraryInfo*  libInfo = oddbGetLibraryInfo();
  OdString  sCopyright = libInfo->getCopyright();

  CDialog::OnInitDialog();

  SetDlgItemText(IDC_INF_COPYRIGHT, sCopyright);

  return TRUE;
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSingleDocApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

/************************************************************************/
/* CSingleDocApp message handlers                                       */
/************************************************************************/

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void CSingleDocApp::start(const char* displayString)
{
  m_Msg = displayString;
  m_nProgressPos = 0;
  m_nPercent = -1;
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void CSingleDocApp::stop()
{
  m_nProgressPos = m_nProgressLimit;
  meterProgress();
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void CSingleDocApp::meterProgress()
{
#ifdef INCOMPLETED
  int nOldPercent = m_nPercent;
  m_nPercent = (m_nProgressPos++) * 100 / m_nProgressLimit;
  if(nOldPercent!=m_nPercent)
  {
    CString str;
    str.Format(IDS_MSGSTRING,"%s %d %%", m_Msg, m_nPercent);
    CMainFrame* pWnd = static_cast<CMainFrame*>(GetMainWnd());
    pWnd->m_wndStatusBar.SetPaneText(0, str);
    MSG msg;
    while(::PeekMessage(&msg, pWnd->m_hWnd, WM_KEYUP, WM_KEYUP, 1))
    {
      bool bDup = false;
      if(msg.wParam == VK_ESCAPE && !bDup)
      {
        bDup = true;
        str.Format("Are you sure you want to terminate\n%s ?", m_Msg);
        if(::AfxMessageBox(str, MB_YESNO|MB_ICONQUESTION)==IDYES)
        {
          throw UserBreak();
        }
      }
    }
  }
#endif
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void CSingleDocApp::setLimit(int max)
{
  m_nProgressLimit = max ? max : 1;
}

#include "tchar.h"


#ifdef _UNICODE
#pragma message("Unicode in use")
#endif

#define MAX_MESSAGE_LEN 300

//#ifdef NOMORE
void CSingleDocApp::warning(const char* warnVisGroup, const OdString& sMsgin)
{
  if (warnVisGroup && *warnVisGroup) // default implementation skips all named groups
    return;
  if (MessageBox(NULL, sMsgin + _T("\n\nDo you want to proceed ?"), _T("Warning!"), MB_ICONWARNING|MB_YESNO)==IDNO)
    throw UserBreak();
}
//#endif

const ODCOLORREF* CSingleDocApp::curPalette() const
{
  const ODCOLORREF *refcolor = odcmAcadPalette(m_background);
  return refcolor;
}
/************************************************************************/
/* Specialization of CCommandLineInfo to add the following switches:    */
/*                                                                      */
/*              /s<filename>   SaveAs                                   */
/*              /exit          Exit                                     */
/************************************************************************/
class CFullCommandLineInfo : public CCommandLineInfo
{
public:
  CString m_SaveName;
  BOOL    m_Exit;
  CFullCommandLineInfo()
    : CCommandLineInfo()
    , m_Exit(0)
  {
  }
  virtual void ParseParam (LPCSTR lpszParam,BOOL bFlag,BOOL bLast )
  {
    BOOL is = FALSE;
    if(bFlag && !_strnicmp(lpszParam, "s", 1))
    {
      m_SaveName = &lpszParam[1];
      is = TRUE;
    }
    else if(bFlag && !_stricmp(lpszParam, "exit"))
    {
      m_Exit = true;
      is = TRUE;
    }

    if (!is || bLast)
      CCommandLineInfo::ParseParam(lpszParam,bFlag,bLast);
  }
};

int CSingleDocApp::ExitInstance() 
{
  /**********************************************************************/
  /* Uninitialize the Drawings SDK                                                */
  /**********************************************************************/
  ::odUninitialize();

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
