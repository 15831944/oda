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

// CreateDlg.h: interface for the CCreateDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CREATEDLG_H__FC75BFA1_066C_41F1_A1DF_4FDD0D290634__INCLUDED_)
#define AFX_CREATEDLG_H__FC75BFA1_066C_41F1_A1DF_4FDD0D290634__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CCreateDlg : public CDialog
{
public:
  CCreateDlg();

  // Dialog Data
  CButton m_English, m_Metric;
  BOOL m_bEngl;
  //{{AFX_DATA(CCreateDlg)
  enum { IDD = IDD_CREATEDRAWING };
  //}}AFX_DATA

  BOOL IsEnglishChoice() { return m_bEngl; }
	//{{AFX_VIRTUAL(CCreateDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
protected:
    //{{AFX_MSG(CCreateDlg)
        // No message handlers
    //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CREATEDLG_H__FC75BFA1_066C_41F1_A1DF_4FDD0D290634__INCLUDED_)
