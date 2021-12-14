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

#if !defined(AFX_PSTBEDITGENERALPG_H__EB8BA5BC_F0C4_45D4_AB3B_C22C4FC877C3__INCLUDED_)
#define AFX_PSTBEDITGENERALPG_H__EB8BA5BC_F0C4_45D4_AB3B_C22C4FC877C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PsTbEditGeneralPg.h : header file
//

#include "resource.h"


#define  PS_EDIT_MAX_SCALEFACTOR        10

/////////////////////////////////////////////////////////////////////////////
// CPsTbEditGeneralPage dialog

class CPsTbEditGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CPsTbEditGeneralPage)


  OdPsPlotStyleTable* m_pPlotStyleTable;
  OdString m_sFileBufPath;

// Construction
public:
	CPsTbEditGeneralPage();
	~CPsTbEditGeneralPage();

// Dialog Data
	//{{AFX_DATA(CPsTbEditGeneralPage)
	enum { IDD = IDD_PS_DLG_GENEGAL };
		// NOTE - ClassWizard will add data members here.
    CEdit	          m_editDescription;
    CButton         m_checkScalefactor;
    CEdit           m_editScalefactor;
  	CStatic       	m_staticFilepath;
    CStatic       	m_staticFilename;
    CStatic         m_staticBitmap;
    CStatic         m_staticRegular;
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPsTbEditGeneralPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
public:
  const bool SetPlotStyleTable(OdPsPlotStyleTable* pPlotStyleTable);
  void SetFileBufPath(const OdString sFilePath);

protected:
	// Generated message map functions
	//{{AFX_MSG(CPsTbEditGeneralPage)
		// NOTE: the ClassWizard will add member functions here
      	virtual BOOL OnInitDialog();
        afx_msg void OnChangeEditDescription();
        afx_msg void OnCheckScalefactor();
        afx_msg void OnEditScalefactor();
       	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSTBEDITGENERALPG_H__EB8BA5BC_F0C4_45D4_AB3B_C22C4FC877C3__INCLUDED_)
