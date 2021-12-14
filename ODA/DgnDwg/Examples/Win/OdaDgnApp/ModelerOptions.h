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
#if !defined(AFX_MODELEROPTIONDLG_H__0E260D91_32BD_4A16_8BDF_A2BE25414A44__INCLUDED_)
#define AFX_MODELEROPTIONDLG_H__0E260D91_32BD_4A16_8BDF_A2BE25414A44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// CModelerOptions dialog

class CModelerOptions : public CDialog
{
public:
	CModelerOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModelerOptions();

  // Dialog Data
  //{{AFX_DATA(CFindObjectDlg)
  enum { IDD = IOD_TRIANG_DIALOG };
  double m_dDegree;
  unsigned int m_nKnots;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CFindObjectDlg)
public:
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CFindObjectDlg)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_MODELEROPTIONDLG_H__0E260D91_32BD_4A16_8BDF_A2BE25414A44__INCLUDED_)
