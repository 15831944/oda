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

#if !defined(_BROWSEFILEDLG_INCLUDED)
#define _BROWSEFILEDLG_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OdaCommon.h"

// CBrowseFileDlg dialog

class CBrowseFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CBrowseFileDlg)

public:
	CBrowseFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBrowseFileDlg();

// Dialog Data
	enum { IDD = IDD_FIND_DIALOG };
  bool    m_bSkip;
  bool    m_bSkipAll;
  OdString m_strFileName;
  OdString m_strPathName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedBrowseBtn();
  afx_msg void OnBnClickedSkipBtn();
  afx_msg void OnBnClickedSkipallBtn();
};

#endif
