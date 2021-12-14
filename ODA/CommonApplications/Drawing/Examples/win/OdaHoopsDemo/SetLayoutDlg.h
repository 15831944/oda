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

#if !defined( _SETLAYOUTDLG_INCLUDED )
#define _SETLAYOUTDLG_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OdaCommon.h"
#include "resource.h"
#include "OdArray.h"

// CSetLayoutDlg dialog

class CSetLayoutDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetLayoutDlg)

public:
	CSetLayoutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetLayoutDlg();

// Dialog Data
	enum { IDD = IDD_SETLAYOUT_DLG };
  OdArray<CString>     m_strLayoutsArr;
  int                  m_iSelItem;

  virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedOk();
};

#endif
