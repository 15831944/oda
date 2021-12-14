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

//
// CustomMediaDlg.h : header file
//
#include "resource.h"
#include "DbPlotSettingsValidatorPE.h"

#ifndef CUSTOM_MEDIA_DLG_DEFINED
#define CUSTOM_MEDIA_DLG_DEFINED

class CCustomMediaDlg : public CDialog
{
public:
  CCustomMediaDlg(OdDbPlotSettingsValidatorPE::psvPaperInfo* paperInfo, CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomMediaDlg();

// Dialog Data
	enum { IDD = IDD_CUSTOM_MEDIA };

private:
  CComboBox m_comboMUnits;

  CString   m_Name;

  double    m_dMarginsL;
  double    m_dMarginsR;
  double    m_dMarginsT;
  double    m_dMarginsB;
  double    m_dHeight;
  double    m_dWidth;

  OdDbPlotSettingsValidatorPE::psvPaperInfo* m_paperInfo;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  virtual BOOL OnInitDialog();
  virtual void OnOK();
  virtual void OnCancel();

  afx_msg void OnSelChangeMUnits();

	DECLARE_MESSAGE_MAP()
};

#endif // CUSTOM_MEDIA_DLG_DEFINED
