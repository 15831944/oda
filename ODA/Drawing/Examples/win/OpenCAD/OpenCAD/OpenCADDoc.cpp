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

// OpenCADDoc.cpp : implementation of the COpenCADDoc class
//

#include "stdafx.h"

#include "OpenCAD.h"

#include "OpenCADDoc.h"
#include "Ed/EdCommandStack.h"
#include "OpenCADView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COpenCADDoc

IMPLEMENT_DYNCREATE(COpenCADDoc, CDocument)

BEGIN_MESSAGE_MAP(COpenCADDoc, CDocument)
	ON_COMMAND(ID_FILE_SEND_MAIL, COpenCADDoc::OnFileSendMail)
	ON_UPDATE_COMMAND_UI(ID_FILE_SEND_MAIL, COpenCADDoc::OnUpdateFileSendMail)
	ON_COMMAND(ID_FILE_IMPORT, COpenCADDoc::OnFileImport)
	ON_COMMAND(ID_DRAW_LINE, COpenCADDoc::OnDrawLine)
END_MESSAGE_MAP()


// COpenCADDoc construction/destruction

COpenCADDoc::COpenCADDoc()
{
	// TODO: add one-time construction code here
	m_strLastCommand = _T("");
	m_bCommandActive = FALSE;
}

COpenCADDoc::~COpenCADDoc()
{
}

BOOL COpenCADDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_pDatabase = theApp.createDatabase();
	return TRUE;
}

// COpenCADDoc serialization

void COpenCADDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// COpenCADDoc diagnostics

#ifdef _DEBUG
void COpenCADDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COpenCADDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COpenCADDoc commands

BOOL COpenCADDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	try
	{
		m_pDatabase =  theApp.readFile(lpszPathName, true, false);
	}
	catch(OdError& /*err*/)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd, _T("Unable to read file"), _T("File Read Error"), MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}

OdString COpenCADDoc::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
	if(theApp.AllowInteraction() == FALSE)
		return OdString(_T(""));

	GetView()->Track(pTracker);

	CString strText;
	GetChildFrame()->m_wndCommandPrompt.GetString((LPCTSTR)prompt, strText);
	return OdString(strText);
}

void COpenCADDoc::putString(const OdString& string)
{
	GetChildFrame()->m_wndCommandPrompt.PutString((LPCTSTR)string);
}

OdEdBaseIO* COpenCADDoc::GetIO()
{
	return this;
}

OdDbCommandContextPtr COpenCADDoc::GetCommandContext()
{
	if(m_pCommandContext.isNull())
		m_pCommandContext = ExDbCommandContext::createObject(GetIO(), m_pDatabase);

	return m_pCommandContext;
}

BOOL COpenCADDoc::ExecuteCommand(const OdString& strCommand, BOOL bEcho) 
{
	if(m_bCommandActive == TRUE)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd, _T("Cannot execute command while another command is active"), _T("Warning"), MB_ICONWARNING);
		return FALSE;
	}

	OdDbCommandContextPtr pCmdCtx = GetCommandContext();

try
{
	OdEdCommandStackPtr pCommands = ::odedRegCmds();
	OdString sCommand = strCommand.spanExcluding(L" \t\r\n");

	if(sCommand.isEmpty()) // User did not enter a command
	{
		if(m_strLastCommand.isEmpty()) // There is no last saved command
		{
			putString(_T("Type a command name at the prompt and press 'Enter'"));
			return FALSE;
		}
		sCommand = m_strLastCommand; // Use last command
	}

	if(bEcho)
		putString(_T("Command: ") + sCommand);

	OdEdCommand* pCommand = pCommands->lookupCmd(sCommand); // Check if command is valid
	if(pCommand == NULL)
		putString(_T("Unknown command '") + sCommand + _T("'"));
	else
	{
		m_bCommandActive = TRUE;
		pCommands->executeCommand(sCommand, pCmdCtx); // Execute the command
		m_strLastCommand = sCommand; // Save as last command
		GetView()->Refresh();
		m_bCommandActive = FALSE;
	}
}
catch(const OdError& e)
{
	m_bCommandActive = FALSE;
	putString(_T("Error: ") + e.description());
	GetView()->Refresh();
	return FALSE;
}
	return TRUE;
}

void COpenCADDoc::OnFileImport()
{
	// TODO: Add your command handler code here
	ExecuteCommand(_T("Import"));
}

COpenCADView* COpenCADDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	return((COpenCADView*)GetNextView(pos));
}

CChildFrame* COpenCADDoc::GetChildFrame()
{
	return (CChildFrame*)(GetView()->GetParentFrame());
}

BOOL COpenCADDoc::CanCloseFrame(CFrameWnd* pFrame)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_bCommandActive == TRUE)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd, _T("Cannot close window while a command is active"), _T("Warning"), MB_ICONWARNING);
		return FALSE;
	}

	return CDocument::CanCloseFrame(pFrame); // msvc6 support // __super::CanCloseFrame(pFrame);
}

OdGePoint3d COpenCADDoc::getPoint(const OdString& prompt, int options, OdEdPointTracker* pTracker)
{
	OdGePoint3d Point;
	if(theApp.AllowInteraction() == FALSE)
		return Point;

	GetView()->Track(pTracker);

	CString strText;
	GetChildFrame()->m_wndCommandPrompt.GetPoint((LPCTSTR)prompt, Point);
	return Point;
}

void COpenCADDoc::OnDrawLine()
{
	// TODO: Add your command handler code here
	ExecuteCommand(_T("Line"));
}

OdUInt32 COpenCADDoc::getKeyState()
{
  OdUInt32 res = 0;
  if (::GetKeyState(VK_CONTROL) != 0)
    res |= MK_CONTROL;
  if (::GetKeyState(VK_SHIFT) != 0)
    res |= MK_SHIFT;
  return res;
}

// TODO

BOOL COpenCADDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
  return CDocument::OnSaveDocument(lpszPathName);
}

void COpenCADDoc::OnCloseDocument()
{
  CDocument::OnCloseDocument();
}

