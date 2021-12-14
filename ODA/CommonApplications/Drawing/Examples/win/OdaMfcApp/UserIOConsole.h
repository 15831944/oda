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

#if !defined(AFX_USERIOCONSOLE_H__5310888B_FAAE_4E56_B996_FE2B1B544CC2__INCLUDED_)
#define AFX_USERIOCONSOLE_H__5310888B_FAAE_4E56_B996_FE2B1B544CC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserIOConsole.h : header file
//

#include "DbUserIO.h"
#include "RxObjectImpl.h"
#include "ExStringIO.h"

/////////////////////////////////////////////////////////////////////////////
// CUserIOConsole dialog

class CUserIOConsole : public CDialog, public OdEdBaseIO
{
  static int _cx; // width
  static int _cy; // height
// Construction
  OdUInt32              m_nRefCounter;

  using CDialog::operator new;
  using CDialog::operator delete;

  int m_nStrings;
  void addOut(const CString& str);
  void addString(const CString& str);
  CFont m_font;
protected:
	CUserIOConsole(CWnd* pParent);
  void addRef();
  void release();
  long numRefs() const;

  // Dialog Data
	//{{AFX_DATA(CUserIOConsole)
	enum { IDD = IDD_CONSOLE_DLG };
	CStatic	m_promptWnd;
	CString	m_sInput;
	CString	m_sPrompt;
	//}}AFX_DATA
	CString	m_sOutput;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserIOConsole)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
	//{{AFX_MSG(CUserIOConsole)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
  OdString getLastString();
  static OdSmartPtr<CUserIOConsole> create(CWnd* pParent);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnDestroy();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

  OdString getString(const OdString& prompt, int options, OdEdStringTracker* pTracker);
  virtual OdUInt32 getKeyState(){return 0;}
  void putString(const OdString& string);
  void echo(const OdString& szString);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERIOCONSOLE_H__5310888B_FAAE_4E56_B996_FE2B1B544CC2__INCLUDED_)
