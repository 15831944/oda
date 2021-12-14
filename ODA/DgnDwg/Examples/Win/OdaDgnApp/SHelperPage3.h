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
#include "afxwin.h"
#include "OdBinaryData.h"
#include "DgModelerGeometry.h"

// CSHelperPage3 dialog

class CSHelperPage3 : public CPropertyPage
{
	DECLARE_DYNAMIC(CSHelperPage3)

  mutable OdStreamBufPtr m_data;
  BOOL m_bSat;
  OdDgModelerGeometry::OdDgModelerVersion getDataVer() const;

  struct PSComboVers
  {
    const char *pText;
    OdDgModelerGeometry::OdDgModelerVersion ver;
  };

  static PSComboVers m_ComboFill[];
  static PSComboVers m_ComboFillSat[];

  void UpdateEditBox();
  OdDgModelerGeometry::OdDgModelerVersion getCurrVer() const;
  OdDgModelerGeometryPtr getModeler() const;
  void SaveOriginalTo(CString path);
  void SaveModifiedTo(CString path);

public:
	CSHelperPage3();
	virtual ~CSHelperPage3();

  void fill(OdStreamBuf &data);

// Dialog Data
	enum { IDD = IDD_SOLID_HELPER_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButtonSave();
  afx_msg void OnBnClickedButtonOriginal();
  afx_msg void OnSelchangeVer();
private:
  CEdit m_StreamEdit;
  CComboBox m_Vers;
public:
  BOOL m_bDebugMode;
  afx_msg void OnBnClickedButton3();
  afx_msg void OnBnClickedButtonSave2();
  afx_msg void OnBnClickedButtonCompare();
};
