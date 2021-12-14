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

// OpenCAD.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OpenCAD.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "OpenCADDoc.h"
#include "OpenCADView.h"
#include "DbLibraryInfo.h"
#include "summinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define AppRegistryKey _T("Open Design Alliance")

// COpenCADApp

BEGIN_MESSAGE_MAP(COpenCADApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, COpenCADApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// COpenCADApp construction

COpenCADApp::COpenCADApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only COpenCADApp object

COpenCADApp theApp;


// COpenCADApp initialization

BOOL COpenCADApp::InitInstance()
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
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(AppRegistryKey);

	if(InitializeTeigha() == FALSE)
		return FALSE;

	LoadDRXModules();

	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_DrawingTYPE,
		RUNTIME_CLASS(COpenCADDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(COpenCADView));
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
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	return TRUE;
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
  OdDbLibraryInfo*  libInfo = oddbGetLibraryInfo();
  //  OdString  sCompany = libInfo->getCompanyName();
  OdString  sLib = libInfo->getBuildComments();
  OdString  sCopyright = libInfo->getCopyright();

  CDialog::OnInitDialog();

  SetDlgItemText(IDC_INF_COPYRIGHT, sCopyright);
  //  SetDlgItemText(IDC_INF_COMPANY, sCompany.c_str());
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
void COpenCADApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// COpenCADApp message handlers

BOOL COpenCADApp::InitializeTeigha()
{
	try
	{
		::odInitialize(this);
	}
	catch(OdError& /*err*/)
	{
		AfxMessageBox(_T("Teigha Initialization error"));
		return FALSE;
	}
	catch(...)
	{
		AfxMessageBox(_T("Teigha Initialization error"));
		return FALSE;
	}
	return TRUE;
}

void COpenCADApp::UninitializeTeigha()
{
	::odUninitialize();
}

int COpenCADApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	UninitializeTeigha();

	return CWinApp::ExitInstance(); // msvc6 support // __super::ExitInstance();
}

void COpenCADApp::LoadDRXModules()
{
  try
  {
	  ::odrxDynamicLinker()->loadModule(OdOpenCadTxModuleName, FALSE);
  }
  catch(OdError& e)
  {
	  AfxMessageBox((LPCTSTR)e.description());
  }
}

BOOL COpenCADApp::AllowInteraction()
{
	POSITION DocTemplatePos = AfxGetApp()->GetFirstDocTemplatePosition();
	while(DocTemplatePos != NULL)
	{
		// Get a document template
		CDocTemplate *pDocTemplate = AfxGetApp()->GetNextDocTemplate(DocTemplatePos);
 
		// Iterate through documents for the current template
		POSITION DocPos = pDocTemplate->GetFirstDocPosition();
		while(DocPos != NULL)
		{
			COpenCADDoc *pDoc = (COpenCADDoc*)pDocTemplate->GetNextDoc(DocPos);
			if(pDoc->GetChildFrame()->m_wndCommandPrompt.m_iCommandMode != CCommandPrompt::eCommandModeNone)
			{
				MessageBox(AfxGetMainWnd()->m_hWnd, _T("Unable to initiate user input. An active command in another document is awaiting user input"), _T("Warning"), MB_ICONWARNING);
				return FALSE;
			}
 		}
	}

	return TRUE;
}
