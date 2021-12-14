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

#include "resource.h"
#include "afxcmn.h"
#include "DgElementIterator.h"
// CSHelperPage1 dialog

class CSHelperSheet;

class CSHelperPage1 : public CPropertyPage
{
	DECLARE_DYNAMIC(CSHelperPage1)

  enum FlagState
  {
    kCrash = 0,
    kOk = 1,
    kNotFound = 2
  };

  void getAttributesForSATSAB(CString fullPath, FlagState &bCrash);
  void getAttributesForXTXB(CString fullPath, FlagState &bCrash);
  void getAttributesForFile(CString fullPath, FlagState &bCrash, FlagState &bXT, FlagState &bSat, FlagState &bFeature, FlagState &bSolid);
  void getAttributesForDgn(CString fullPath, FlagState &bCrash, FlagState &bXT, FlagState &bSat, FlagState &bFeature, FlagState &bSolid);
  void proceedForDgn(OdDgElementIteratorPtr pIter, FlagState &bCrash, FlagState &bXT, FlagState &bSat, FlagState &bFeature, FlagState &bSolid);

  CListCtrl      m_FileList;
  CSHelperSheet* m_pSheet;
  CString        m_FolderPath;
  BOOL m_EnableSubfolders;
  BOOL m_dgn;
  BOOL m_xt;
  BOOL m_xb;
  BOOL m_sat;

public:
  // MKU 02/01/10 warning C4355: 'this' : used in base member initializer list
	CSHelperPage1(/*CSHelperSheet &pSheet*/);
	virtual ~CSHelperPage1();

  void setSheet(CSHelperSheet* pSheet);

// Dialog Data
	enum { IDD = IDD_SOLID_HELPER_PAGE1 };

protected:
  
  void addFileToList(CString fullPath);
  void searchProcedure(CString folder, CString ext);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedGetPath();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedSearch();
  afx_msg void OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult);
};
