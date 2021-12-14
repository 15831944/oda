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

class OdDgElement;
class CSHelperSheet;

// CSHelperPage2 dialog

class CSHelperPage2 : public CPropertyPage
{
	DECLARE_DYNAMIC(CSHelperPage2)

  CSHelperSheet* m_pSheet;

  enum FlagState
  {
    kCrash = 0,
    kOk = 1,
    kNotFound = 2
  };

  void addEntityToList(OdDgElementIteratorPtr pIter);
  void addEntityToList(const OdDgElement *pE);
  void addFileToList(CString fullPath);
public:
  // MKU 02/01/10 warning C4355: 'this' : used in base member initializer list
	CSHelperPage2(/*CSHelperSheet &pSheet*/);
	virtual ~CSHelperPage2();

  void setSheet(CSHelperSheet* pSheet);

  void fill (CString fileName);
  BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_SOLID_HELPER_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
  CListCtrl m_EntitiesList;
  CString m_Path;
public:
  afx_msg void OnNMDblclkEntitiesList(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedButtonLoadfile();
};
