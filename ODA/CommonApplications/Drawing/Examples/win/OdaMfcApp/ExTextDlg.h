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
#include "afxwin.h"


enum Justify_Mode
{
  Justify_Left = 0,
  Justify_Center,
  Justify_Right,
  Justify_Align,
  Justify_Midle,
  Justify_Fit,
  Justify_Top_left,
  Justify_Top_center,
  Justify_Top_right,
  Justify_Midle_left,
  Justify_Midle_center,
  Justify_Midle_right,
  Justify_Bottom_left,
  Justify_Bottom_center,
  Justify_Bottom_right
};

class CExTextDlg : public CDialog
{
	 DECLARE_DYNAMIC(CExTextDlg)

  HICON m_hIcon;
  
  CString      sCurrent;
  OdDbObjectId idStyle;

  OdDbTextPtr&     pText;
  OdDbDatabasePtr& pDb;

public:
	 CExTextDlg(OdDbDatabasePtr& pDb, OdDbTextPtr& pText, CWnd* pParent = NULL);

	 enum { IDD = IDD_EXTEXT_DLG };

 protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

 public:
  afx_msg void OnBnClickedOk();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void OnCbnSelchangeJustify();
  afx_msg void OnCbnSelchangeStyle();

  CComboBox m_Justify;
  CString   m_Text;
  CString   m_Height;
  CString   m_Angle;
  CComboBox m_Style;
};
