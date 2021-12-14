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

// HoopsViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "HoopsViewer.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "HoopsViewerDoc.h"
#include "HoopsViewerView.h"
#include "SetDirectoriesDlg.h"
#include "RegistryCommon.h"

#include "hoops_3dgs/source/visualize_license.h"

#define AppRegistryKey _T("Open Design Alliance")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHoopsViewerApp

BEGIN_MESSAGE_MAP(CHoopsViewerApp, CWinApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
  ON_COMMAND(ID_DIRECTORIES, OnSetDirectories)
  ON_COMMAND(ID_SETBGCOLOR, OnBgColor)
  ON_COMMAND(ID_TOOLS_IMPORT_CUR, OnLoadActiveLayout)
  ON_UPDATE_COMMAND_UI(ID_TOOLS_IMPORT_CUR, OnUpdateLoadActiveLayout)
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

// CHoopsViewerApp construction

CHoopsViewerApp::CHoopsViewerApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
  m_iCounter = 0;

  char    szTemp[256];

  //determine the driver set in the environment
  HC_Show_Pathname_Expansion ("?Picture", szTemp);
  HC_Parse_String (szTemp, "/", 2, m_sDriverType);
  HC_Delete_Segment ("?Picture");
  HC_UnDefine_Alias ("?Picture");
}


// The one and only CHoopsViewerApp object

CHoopsViewerApp theApp;


// CHoopsViewerApp initialization

BOOL CHoopsViewerApp::InitInstance()
{
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

  SetRegistryKey(AppRegistryKey);

  //LoadStdProfileSettings();  // Load standard INI file options (including MRU)

  // Register the application's document templates.  Document templates
  //  serve as the connection between documents, frame windows and views.

  m_clrBgColor    = RGB(220,220,220);
  m_bFirstRun     = true;
  m_bShowAxis     = true;
  m_bImportAllLayouts = false;

  InitSettingsFromRegistry();

  if( m_bFirstRun )
    AddACADDefaultPaths();

  if( m_bFirstRun && m_strFontDirArr.size() )
  {
    if( AfxMessageBox(L"\nDo you want to use AutoCAD directories for searching fonts and textures?", MB_YESNO) == IDNO )
      m_strFontDirArr.clear();
  }

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_SIMPLETYPE,
		RUNTIME_CLASS(CHoopsViewerDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CHoopsViewerView));
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


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

  HC_Define_System_Options("license = `" VISUALIZE_LICENSE "`");

	return TRUE;
}

int CHoopsViewerApp::ExitInstance() 
{
  HC_Reset_System(); 
  SaveSettingsToRegistry();
  return CWinApp::ExitInstance();
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

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CHoopsViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CHoopsViewerApp message handlers

void CHoopsViewerApp::InitSettingsFromRegistry()
{
  m_bFirstRun = GetProfileInt(_T("Settings"), _T("FirstRun"), 1) ? true : false;
  m_bShowAxis = GetProfileInt(_T("Settings"), _T("ShowAxis"), 1) ? true : false;
  m_bImportAllLayouts = GetProfileInt(_T("Settings"), _T("LoadAllLayouts"), 0) ? true : false;

  if( !m_bFirstRun )
  {
    int R = GetProfileInt(_T("Settings"), _T("BackgroundR"), 220);
    int G = GetProfileInt(_T("Settings"), _T("BackgroundG"), 220);
    int B = GetProfileInt(_T("Settings"), _T("BackgroundB"), 220);
    m_clrBgColor = RGB(R,G,B);

    OdString strPath;

    for( unsigned int i = 0; i < 100; i++ )
    {
      OdString strDir;
      strDir.format(L"FileDirectory%d", i);
      strPath = GetProfileString(_T("Settings"),strDir, L"");

      if( !strPath.isEmpty() )
        m_strFontDirArr.push_back(strPath);
      else
        break;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////

void CHoopsViewerApp::SaveSettingsToRegistry()
{
  WriteProfileInt(_T("Settings"), _T("FirstRun"), 0);
  WriteProfileInt(_T("Settings"), _T("ShowAxis"), m_bShowAxis ? 1 : 0);
  WriteProfileInt(_T("Settings"), _T("LoadAllLayouts"), m_bImportAllLayouts ? 1 : 0);
  WriteProfileInt(_T("Settings"), _T("BackgroundR"), GetRValue(m_clrBgColor));
  WriteProfileInt(_T("Settings"), _T("BackgroundG"), GetGValue(m_clrBgColor));
  WriteProfileInt(_T("Settings"), _T("BackgroundB"), GetBValue(m_clrBgColor));

  OdString strDir;

  for( unsigned int i = 0; i < m_strFontDirArr.size(); i++ )
  {
    strDir.format(L"FileDirectory%d", i);
    WriteProfileString(_T("Settings"),strDir, m_strFontDirArr[i]);
  }

  strDir.format(L"FileDirectory%d", m_strFontDirArr.size());
  WriteProfileString(_T("Settings"),strDir, L"");
}

/////////////////////////////////////////////////////////////////////////////

void CHoopsViewerApp::AddACADDefaultPaths()
{
  OdString sFile = GetRegistryACADFromProfile();
  OdString sPath;

  while( !sFile.isEmpty() )
  {
    int nFindStr = sFile.find(L";");

    if( -1 == nFindStr )
    {
      sPath = sFile;
      sFile.empty();
    }
    else
    {
      sPath = sFile.left(nFindStr);
      sPath += L"\\";

      m_strFontDirArr.push_back( sPath );

      sFile = sFile.right(sFile.getLength() - nFindStr - 1);
    }
  }

  sFile = GetRegistryAVEMAPSFromProfile();

  while( !sFile.isEmpty() )
  {
    int nFindStr = sFile.find(L";");

    if( -1 == nFindStr )
    {
      sPath = sFile;
      sFile.empty();
    }
    else
    {
      sPath = sFile.left(nFindStr);
      sPath += L"\\";

      m_strFontDirArr.push_back( sPath );

      sFile = sFile.right(sFile.getLength() - nFindStr - 1);
    }
  }

  OdString sAcadLocation(GetRegistryAcadLocation());

  if( !sAcadLocation.isEmpty() )
  {
    sFile = sAcadLocation;
    sFile += L"\\Fonts\\";

    m_strFontDirArr.push_back( sFile );

    sFile = sAcadLocation;
    sFile += L"\\Support\\";

    m_strFontDirArr.push_back( sFile );
  }
}

/////////////////////////////////////////////////////////////////////////////

void CHoopsViewerApp::OnSetDirectories()
{
  CSetDirectoriesDlg dlgDirs( &(((CHoopsViewerApp*)AfxGetApp())->m_strFontDirArr) );
  dlgDirs.DoModal();
}

/////////////////////////////////////////////////////////////////////////////

void CHoopsViewerApp::OnBgColor()
{
  CColorDialog dlgBgColor( m_clrBgColor, 0, m_pMainWnd);
  COLORREF     clrBgColor = m_clrBgColor;

  if( dlgBgColor.DoModal() == IDOK )
  {
    clrBgColor = dlgBgColor.GetColor();
    m_clrBgColor = clrBgColor;

    if( m_pMainWnd )
      m_pMainWnd->PostMessage(WM_COMMAND,ID_CHANGE_BG_COLOR,0);
  }
}

/////////////////////////////////////////////////////////////////////////////

void CHoopsViewerApp::OnLoadActiveLayout()
{
  m_bImportAllLayouts = !m_bImportAllLayouts;
}

/////////////////////////////////////////////////////////////////////////////

void CHoopsViewerApp::OnUpdateLoadActiveLayout(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(!m_bImportAllLayouts);
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
