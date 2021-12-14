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

#pragma once

#include "OpenCADDoc.h"

// CCommandPrompt dialog
class CChildFrame;

class CCommandPrompt : public CDialogBar
{
	DECLARE_DYNAMIC(CCommandPrompt)

public:
	enum CommandMode
	{
		eCommandModeNone		= 0,
		eCommandModeGetString	= 1,
		eCommandModeGetPoint	= 2
	};
	CommandMode m_iCommandMode;
	BOOL m_bCancel;
	CChildFrame* m_pChildFrame;

public:
	CCommandPrompt();   // standard constructor
	virtual ~CCommandPrompt();
	void Resize(int iWidth);
	BOOL PutString(const CString& strText, const BOOL bNewLine = TRUE);
	BOOL GetString(const CString& strPrompt, CString& strText);
	COpenCADDoc* GetDocument();
	BOOL GetPoint(const CString& strPrompt, OdGePoint3d& Point);
	BOOL GetPointFromString(const CString& strText, OdGePoint3d& Point);
	BOOL SetCommand(const CString& strText, BOOL bEnter);

// Dialog Data
	enum { IDD = IDD_COMMANDPROMPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
