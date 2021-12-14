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

// CommandPrompt.cpp : implementation file
//

#include "stdafx.h"
#include "OpenCAD.h"
#include "CommandPrompt.h"

#include "ChildFrm.h"

// CCommandPrompt dialog

IMPLEMENT_DYNAMIC(CCommandPrompt, CDialogBar)

CCommandPrompt::CCommandPrompt()
{
	m_iCommandMode = eCommandModeNone;
}

CCommandPrompt::~CCommandPrompt()
{
}

void CCommandPrompt::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCommandPrompt, CDialogBar)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CCommandPrompt message handlers

void CCommandPrompt::Resize(int iWidth)
{
  if (!m_hWnd)
    return;

	CEdit* pEdit;
	CRect rcWindow, rcInput, rcPrompt;
	GetWindowRect(&rcWindow);
	this->MoveWindow(0, 0, iWidth, rcWindow.Height());

	pEdit = (CEdit*)GetDlgItem(IDC_EDT_COMMAND);
	::GetWindowRect(pEdit->GetSafeHwnd(), &rcInput);
	::MoveWindow(pEdit->GetSafeHwnd(), 2, rcWindow.Height() - rcInput.Height() - 2, iWidth, rcInput.Height(), TRUE);

	pEdit = (CEdit*)GetDlgItem(IDC_EDT_PROMPT); 
	::GetWindowRect(pEdit->GetSafeHwnd(), &rcPrompt);
	::MoveWindow(pEdit->GetSafeHwnd(), 2, 2, iWidth - 2, rcWindow.Height() - rcInput.Height() - 4, TRUE);
}

void CCommandPrompt::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

//	CDialogBar::OnLButtonDown(nFlags, point);
}

void CCommandPrompt::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

//	CDialogBar::OnLButtonDblClk(nFlags, point);
}

BOOL CCommandPrompt::PutString(const CString& strText, BOOL bNewLine)
{
	CString strPrompt;

	CEdit* pwndPrompt = (CEdit*)GetDlgItem(IDC_EDT_PROMPT);
	if(pwndPrompt == NULL)
		return FALSE;

	// Get original prompt text
	pwndPrompt->GetWindowTextW(strPrompt);

	if(bNewLine == TRUE)
		strPrompt += _T("\r\n");
	strPrompt += strText;

	// Update prompt text
	pwndPrompt->SetWindowTextW(strPrompt);

	// Scroll to last line
	pwndPrompt->LineScroll(pwndPrompt->GetLineCount());

	return TRUE;
}

BOOL CCommandPrompt::GetString(const CString& strPrompt, CString& strText)
{
	if(m_iCommandMode != eCommandModeNone)
	{
		throw OdError(_T("Unable to request input. Another command is active"));
	}

	if(PutString(strPrompt) == FALSE)	// Show prompt
	{
		throw OdError(_T("Unable to output to prompt area"));
	}

	CEdit* pwndCommand = (CEdit*)GetDlgItem(IDC_EDT_COMMAND);
	if(pwndCommand == NULL)
	{
		throw OdError(_T("Invalid command prompt"));
	}

	pwndCommand->SetFocus();	// Set focus to the command edit box

	m_iCommandMode = eCommandModeGetString;	// Go into GetString mode
	m_bCancel = FALSE;		// Reset Cancel flag
	while(theApp.PumpMessage())
	{
		if(m_iCommandMode == eCommandModeNone)	// Exit GetString mode
			break;
	}

	if(m_bCancel == TRUE)	// User cancelled operation
	{
		PutString(_T(" *Cancel*"), FALSE);
		throw OdEdCancel();
	}

	// Once again get the control. This is important because in the while
	// loop above the command prompt may have been destroyed
	pwndCommand = (CEdit*)GetDlgItem(IDC_EDT_COMMAND);
	if(pwndCommand == NULL)
	{
		throw OdError(_T("Invalid command prompt"));
	}

	pwndCommand->GetWindowTextW(strText);	// Store entered text
	pwndCommand->SetWindowTextW(_T(""));	// Empty command edit box
	PutString(strText, FALSE);	// Append entered text to prompt

	return TRUE;
}

BOOL CCommandPrompt::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_iCommandMode != eCommandModeNone && (wParam == IDCANCEL || wParam == IDOK))
	{
		m_iCommandMode = eCommandModeNone;
		if(wParam == IDCANCEL)
			m_bCancel = TRUE;
	}
	else if(m_iCommandMode == eCommandModeNone && wParam == IDOK)
	{
		CEdit* pwndCommand = (CEdit*)GetDlgItem(IDC_EDT_COMMAND);
		if(pwndCommand != NULL)
		{
			CString strCommand;
			pwndCommand->GetWindowTextW(strCommand);
			pwndCommand->SetWindowTextW(_T(""));
			GetDocument()->ExecuteCommand(OdString(strCommand));
			return FALSE;
		}
	}

	return CDialogBar::OnCommand(wParam, lParam);
}

COpenCADDoc* CCommandPrompt::GetDocument()
{
	return (COpenCADDoc*)m_pChildFrame->GetActiveDocument();
}

BOOL CCommandPrompt::GetPoint(const CString& strPrompt, OdGePoint3d& Point)
{
	if(m_iCommandMode != eCommandModeNone)
	{
		throw OdError(_T("Unable to request input. Another command is active"));
	}

Repeat:
	if(PutString(strPrompt) == FALSE)	// Show prompt
	{
		throw OdError(_T("Unable to output to prompt area"));
	}

	CEdit* pwndCommand = (CEdit*)GetDlgItem(IDC_EDT_COMMAND);
	if(pwndCommand == NULL)
	{
		throw OdError(_T("Invalid command prompt"));
	}

	pwndCommand->SetFocus();	// Set focus to the command edit box

	m_iCommandMode = eCommandModeGetPoint;	// Go into GetPoint mode
	m_bCancel = FALSE;		// Reset Cancel flag
	while(theApp.PumpMessage())
	{
		if(m_iCommandMode == eCommandModeNone)	// Exit GetPoint mode
			break;
	}

	if(m_bCancel == TRUE)	// User cancelled operation
	{
		PutString(_T(" *Cancel*"), FALSE);
		throw OdEdCancel();
	}

	// Once again get the control. This is important because in the while
	// loop above the command prompt may have been destroyed
	pwndCommand = (CEdit*)GetDlgItem(IDC_EDT_COMMAND);
	if(pwndCommand == NULL)
	{
		throw OdError(_T("Invalid command prompt"));
	}

	CString strText;
	pwndCommand->GetWindowTextW(strText);	// Store entered text
	if(strText == _T(""))
	{
		throw OdEdCancel();
	}
	if(GetPointFromString(strText, Point) == FALSE)
	{
		PutString(strText, FALSE);	// Append entered text to prompt
		PutString(_T("Invalid point"));
		goto Repeat;
	}

	pwndCommand->SetWindowTextW(_T(""));	// Empty command edit box
	PutString(strText, FALSE);	// Append entered text to prompt

	return TRUE;
}

BOOL CCommandPrompt::GetPointFromString(const CString& strText, OdGePoint3d& Point)
{
	TCHAR strLine[255];
	_tcscpy(strLine, strText);
	for(unsigned int i = 0; i < _tcslen(strLine); i++)
	{
		if(strLine[i] == ',')
			strLine[i] = ' ';
	}

	double x, y, z = 0.0;
	if(_stscanf(strLine, _T("%lf %lf %lf"), &x, &y, &z) < 2)
		return FALSE;

	Point.set(x, y, z);
	return TRUE;
}

BOOL CCommandPrompt::SetCommand(const CString& strText, BOOL bEnter)
{
	CEdit* pwndCommand = (CEdit*)GetDlgItem(IDC_EDT_COMMAND);
	if(pwndCommand == NULL)
		return FALSE;

	pwndCommand->SetWindowText(strText);
	if(bEnter)
		PostMessage(WM_COMMAND, IDOK);

	return TRUE;
}
