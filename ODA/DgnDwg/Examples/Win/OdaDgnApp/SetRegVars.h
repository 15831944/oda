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
public:

  enum CRegVarsType
  {
    kSymbRsrc   = 0,
    kMatPalette = 1
  };

private:

  OdDgHostAppServices*    m_pHostAppServices;

  OdArray<OdString>       m_MsSymbRsrcArr;

  CRegVarsType            m_uType;

// Construction
public:
  CSetRegVarsMsSymbRsrc(CWnd* pParent = NULL, CRegVarsType uType = kSymbRsrc );   // standard constructor

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

  void updateListCtrl();

  // Generated message map functions
  //{{AFX_MSG(CSetRegVarsMsSymbRsrc)
  afx_msg void OnAddButton();
  afx_msg void OnMoveUpButton();
  afx_msg void OnMoveDownButton();
  afx_msg void OnDeleteButton();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnLvnItemchangedPathlist(NMHDR *pNMHDR, LRESULT *pResult);
};


/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsSymbRsrc

class CSetRegVarsMsRsrc : public CDialog
{
public:

  enum CRegVarsType
  {
    kMsRsrc     = 0,
    kMsPenTable = 1,
    kMsDefCtbl  = 2
  };

private:
  OdDgHostAppServices*    m_pHostAppServices;

  CString                 m_MsRsrc;

  CRegVarsType            m_uType;

// Construction
public:
  CSetRegVarsMsRsrc(CWnd* pParent = NULL, CRegVarsType uType = kMsRsrc);   // standard constructor

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
// CSetRegVarsMsTexDir

class CSetRegVarsMsTexDir : public CDialog
{
public:
  enum PropertyToSet
  {
    kMaterial = 0,
    kPattern  = 1,
    kBump     = 2,
    kMtbl     = 3,
    kFontPath = 4
  };

public:
  OdDgHostAppServices*    m_pHostAppServices;

  CString                 m_MsTexDir;
  PropertyToSet           m_iProperty;       

  // Construction
public:
  CSetRegVarsMsTexDir(CWnd* pParent = NULL, PropertyToSet iProperty = kPattern);   // standard constructor

  void setHostAppServices(OdDgHostAppServices* pAppServices) 
  { 
    m_pHostAppServices = pAppServices; 
  }

  // Dialog Data
  //{{AFX_DATA(CSetRegVarsMsRsrc)
  enum { IDD = IDD_PATTERN };
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSetRegVarsMsTexDir)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  virtual BOOL OnInitDialog();
  virtual void OnOK();

  // Generated message map functions
  //{{AFX_MSG(CSetRegVarsMsRsrc)
  afx_msg void OnBnClickedMsTexDir();
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

/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsNurbs

class CSetRegVarsNurbs : public CDialog
{
  OdDgHostAppServices*    m_pHostAppServices;

  OdUInt32                m_iGridSize;

  // Construction
public:
  CSetRegVarsNurbs(CWnd* pParent = NULL);   // standard constructor

  void setHostAppServices(OdDgHostAppServices* pAppServices) 
  { 
    m_pHostAppServices = pAppServices; 
  }

  // Dialog Data
  //{{AFX_DATA(CSetRegVarsNurbs)
  enum { IDD = IDD_RENDERING_PROP_DLG };
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSetRegVarsNurbs)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  virtual BOOL OnInitDialog();
  virtual void OnOK();

  // Generated message map functions
  //{{AFX_MSG(CSetRegVarsNurbs)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsGripPoints

class CSetRegVarsGripPoints : public CDialog
{
  OdDgHostAppServices*    m_pHostAppServices;

  OdUInt32                m_uGripSize;
  OdUInt32                m_uGripObjLimit;
  int                     m_uGripColor;
  int                     m_uGripHover;
  int                     m_uGripHot;

  // Construction
public:
  CSetRegVarsGripPoints(CWnd* pParent = NULL);   // standard constructor

  void setHostAppServices(OdDgHostAppServices* pAppServices) 
  { 
    m_pHostAppServices = pAppServices; 
  }

  // Dialog Data
  //{{AFX_DATA(CSetRegVarsGripPoints)
  enum { IDD = IDD_GRIP_POINTS_PROP_DLG };
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CSetRegVarsGripPoints)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  virtual BOOL OnInitDialog();
  virtual void OnOK();

  // Generated message map functions
  //{{AFX_MSG(CSetRegVarsGripPoints)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_SETREGVARS_H__INCLUDED_)

