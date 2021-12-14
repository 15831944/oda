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

#if !defined(AFX_PSTBEDIT_H__2D8ADF65_CA57_4F6A_AD85_504D283EEF56__INCLUDED_)
#define AFX_PSTBEDIT_H__2D8ADF65_CA57_4F6A_AD85_504D283EEF56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//

/////////////////////////////////////////////////////////////////////////////
// Sheet
#include "PsTbEditFormViewPg.h"
#include "PsTbEditGeneralPg.h"

class OdPsPlotStyleTable;

class CPsTableEditorDlg : public CPropertySheet
{
    DECLARE_DYNCREATE(CPsTableEditorDlg)

    OdPsPlotStyleTable *m_pPlotStyleTable;
    OdPsPlotStyleTablePtr m_pPsTabForPropertyPg;

// Construction
public:
	CPsTableEditorDlg(CWnd* pParentWnd = NULL);
  const bool SetPlotStyleTable(OdPsPlotStyleTable* pPlotStyleTable);
  void SetFileBufPath(const OdString sFilePath);
  OdPsPlotStyleTablePtr GetPlotStyleTable() const;

// Attributes
public:
  CPsTbEditGeneralPage m_page1;
	CPsTbEditFormViewPage m_page2;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPsTableEditorDlg)
		enum { IDD = IDD_PS_DLG };
// Implementation
public:
	virtual ~ CPsTableEditorDlg();
	// Generated message map functions
protected:
	//{{AFX_MSG(Sheet)
		// NOTE - the ClassWizard will add and remove member functions here.
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSTBEDIT_H__2D8ADF65_CA57_4F6A_AD85_504D283EEF56__INCLUDED_)
