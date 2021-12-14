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

#if !defined(AFX_LWSETTINGSDLG_H__A75E2AC2_03B6_4E5B_9D8D_587AD09F378B__INCLUDED_)
#define AFX_LWSETTINGSDLG_H__A75E2AC2_03B6_4E5B_9D8D_587AD09F378B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LwSettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLwSettingsDlg dialog

class CLwSettingsDlg : public CDialog
{
// Construction
  void fillList();
  BOOL m_bListMM;
  int  m_defaultIndex;
  int  m_lwIndex;
public:
  OdDb::LineWeight  m_defaultLw;
  OdDb::LineWeight  m_currentLw;
  bool              m_bDisplayLw;
  int               m_lwScaleIndex;

	CLwSettingsDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLwSettingsDlg)
	enum { IDD = IDD_LINEWEIGHT_SETTINGS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLwSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLwSettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioMm();
	afx_msg void OnRadioIn();
	afx_msg void OnSelchangeComboDefault();
	afx_msg void OnSelchangeLineweights();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LWSETTINGSDLG_H__A75E2AC2_03B6_4E5B_9D8D_587AD09F378B__INCLUDED_)
