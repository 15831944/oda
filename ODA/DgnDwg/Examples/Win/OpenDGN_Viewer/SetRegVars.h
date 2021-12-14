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
#if !defined(_SETREGVARS_H__INCLUDED_)
#define _SETREGVARS_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRegVars.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsSymbRsrc

class CSetRegVarsMsSymbRsrc : public CDialog
{
  OdDgHostAppServices*    m_pHostAppServices;

  CString                 m_MsSymbRsrc;

// Construction
public:
  CSetRegVarsMsSymbRsrc(CWnd* pParent = NULL);   // standard constructor

  void setHostAppServices(OdDgHostAppServices* pAppServices) 
  { 
    m_pHostAppServices = pAppServices; 
  }

// Dialog Data
  //{{AFX_DATA(CSetRegVarsMsSymbRsrc)
  enum { IDD = IDD_SYMBRSRC };
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSetRegVarsMsSymbRsrc)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  virtual BOOL OnInitDialog();
  virtual void OnOK();

  // Generated message map functions
  //{{AFX_MSG(CSetRegVarsMsSymbRsrc)
  afx_msg void OnBnClickedMsSymbRsrc();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedCancel();
};


/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsSymbRsrc

class CSetRegVarsMsRsrc : public CDialog
{
  OdDgHostAppServices*    m_pHostAppServices;

  CString                 m_MsRsrc;

// Construction
public:
  CSetRegVarsMsRsrc(CWnd* pParent = NULL);   // standard constructor

  void setHostAppServices(OdDgHostAppServices* pAppServices) 
  { 
    m_pHostAppServices = pAppServices; 
  }

// Dialog Data
  //{{AFX_DATA(CSetRegVarsMsRsrc)
  enum { IDD = IDD_RSRC };
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSetRegVarsMsRsrc)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  virtual BOOL OnInitDialog();
  virtual void OnOK();

  // Generated message map functions
  //{{AFX_MSG(CSetRegVarsMsRsrc)
  afx_msg void OnBnClickedMsRsrc();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsSymbRsrc

class CSetRegVarsFastFont : public CDialog
{
  OdDgHostAppServices*    m_pHostAppServices;

  CString                 m_FastFont;

// Construction
public:
  CSetRegVarsFastFont(CWnd* pParent = NULL);   // standard constructor

  void setHostAppServices(OdDgHostAppServices* pAppServices) 
  { 
    m_pHostAppServices = pAppServices; 
  }

// Dialog Data
  //{{AFX_DATA(CSetRegVarsFastFont)
  enum { IDD = IDD_FASTFONT };
  //}}AFX_DATA

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSetRegVarsFastFont)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  virtual BOOL OnInitDialog();
  virtual void OnOK();

  // Generated message map functions
  //{{AFX_MSG(CSetRegVarsFastFont)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_SETREGVARS_H__INCLUDED_)

