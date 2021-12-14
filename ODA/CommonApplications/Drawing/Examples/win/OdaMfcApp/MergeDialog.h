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

#if !defined(AFX_MERGE_H__INCLUDED_)
#define AFX_MERGE_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetActiveLayoutDlg.h : header file
//
#include "OdaCommon.h"
#include "DbDatabase.h"
#define STL_USING_MAP
#define STL_USING_VECTOR
#include "OdaSTL.h"


//#include <afxdialogex.h>


// MergeDialog dialog

class MergeDialog : public CDialog
{
	DECLARE_DYNAMIC(MergeDialog)

  int     m_nSelected1;
  int     m_nSelected2;
  OdDbDatabase* m_pDb;
  CListBox* m_pListBranches1;
  CListBox* m_pListBranches2;
  CButton* m_pButtonAddBranch;
  std::vector<OdString> m_branches;
public:
	MergeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~MergeDialog();
  void setDatabase(OdDbDatabase* pDb)
  {
    m_pDb = pDb;
  }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MERGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  void fillBranches();

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButtonMergeOk();
  afx_msg void OnLbnSelchangeListMergeBranches1();
  afx_msg void OnLbnSelchangeListMergeBranches2();
};

#endif
