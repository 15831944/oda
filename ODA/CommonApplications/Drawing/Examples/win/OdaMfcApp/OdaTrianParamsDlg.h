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

#if !defined(AFX_ODATRIANPARAMSDLG_H__F495BBB8_424C_4927_961B_80652F1C4F34__INCLUDED_)
#define AFX_ODATRIANPARAMSDLG_H__F495BBB8_424C_4927_961B_80652F1C4F34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "afxwin.h"
#include "Wr/wrTriangulationParams.h"
// OdaTrianParamsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OdaTrianParamsDlg dialog

class OdaTrianParamsDlg : public CDialog
{
public:
	OdaTrianParamsDlg(CWnd* pParent = NULL, wrTriangulationParams *params = NULL, double *facetRes = NULL);   // standard constructor
  virtual ~OdaTrianParamsDlg();

	enum { IDD = IDD_TRIANGULATIONPARAMS };
  wrTriangulationParams *m_params;
  double *m_dFacetRes;

protected:
  void fillTriangulationParametersFields();
  bool validateField(int hDlg, double minVal, double maxVal, double &value, const OdString name, bool isInteger = false);
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedTpUsenewtessellation();
  afx_msg void OnBnClickedRecalcSurfTol();
  afx_msg void OnBnClickedButtonDefault();
};

#endif // !defined(AFX_ODATRIANPARAMSDLG_H__F495BBB8_424C_4927_961B_80652F1C4F34__INCLUDED_)
