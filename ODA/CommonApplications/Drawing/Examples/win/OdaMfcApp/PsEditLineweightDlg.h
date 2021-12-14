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

#if !defined(AFX_PSEDITLINEWEIGHTDLG_H__0D67E57C_0ED0_4998_A9E8_A605D65325E7__INCLUDED_)
#define AFX_PSEDITLINEWEIGHTDLG_H__0D67E57C_0ED0_4998_A9E8_A605D65325E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PsEditLineweightDlg.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CPsEditLineweight dialog


struct CPsEditLineweightIdx
{
  int m_iOldIdx;
  int m_iNewIdx;
  double value;
  CPsEditLineweightIdx() : m_iOldIdx(-1), m_iNewIdx(-1){}; 
};

class CPsEditLineweightDlg : public CDialog
{

  OdPsPlotStyleTable*      m_pPlotStyleTable;
  CImageList               m_imageList;
  UINT                     m_uintListActiveIdx;
  CPsEditLineweightIdx*    m_pPsEditLineweightIdx;

// Construction
public:
	CPsEditLineweightDlg(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CPsEditLineweight)
	enum { IDD = IDD_PS_DLG_LINEWEIGHT };
    CListCtrl            m_listLineweight;
    CButton              m_radioMillimetrs;
    CButton              m_radioInches;
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPsEditLineweight)
	protected:
	virtual BOOL DestroyWindow();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
  void initListCtrl();
  const int insertItem(int index, const CString& sLineweight, const bool isUse);
  void initImageList();

public:
  const bool SetPlotStyleTable(OdPsPlotStyleTable* pPlotStyleTable);
  void SetUnitIntoList(const bool bUnit);
  void SetListActiveIdx(const UINT index);

protected:
	// Generated message map functions
	//{{AFX_MSG(CPsTbEditFormViewPage)
		// NOTE: the ClassWizard will add member functions here
    	virtual void OnOK();
    	virtual BOOL OnInitDialog();
      afx_msg void OnRadioMillimetrs();
      afx_msg void OnRadioInches();
      afx_msg void OnButtonEditlineweight();
      afx_msg void OnButtonSortlineweight();
    	afx_msg void OnEndlabeleditListLineweight(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSEDITLINEWEIGHTDLG_H__0D67E57C_0ED0_4998_A9E8_A605D65325E7__INCLUDED_)
