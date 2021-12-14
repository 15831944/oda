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
#if !defined(AFX_LOADAPPS_H__8CD884D4_A935_478F_9494_EE0265C186AE__INCLUDED_)
#define AFX_LOADAPPS_H__8CD884D4_A935_478F_9494_EE0265C186AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadApps.h : header file
//

#include "RxDLinkerReactor.h"

/////////////////////////////////////////////////////////////////////////////
// CLoadApps dialog

class CLoadApps : public CDialog
{
public:

  class LoadedApps : public OdArray<OdString>, public OdStaticRxObject<OdRxDLinkerReactor>
  {
	  
  public:
    ODRX_HEAP_OPERATORS();
    LoadedApps() : m_pListBox(0) {}
    CListBox* m_pListBox;
    void rxAppLoaded(OdRxModule* pAppModule);
    void rxAppUnloaded(const OdString& szAppName);
  };


private:
  friend class LoadedApps;
  static LoadedApps* m_pLoadedApps;

// Construction
public:
  CLoadApps(CWnd* pParent = NULL);   // standard constructor

  static void rxInit();
  static void rxUninit();
// Dialog Data
  //{{AFX_DATA(CLoadApps)
  enum { IDD = IDD_LOAD_APPS};
  CButton  m_UnloadBtn;
  CListBox  m_AppsList;
  //}}AFX_DATA


// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CLoadApps)
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

// Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CLoadApps)
  virtual BOOL OnInitDialog();
  afx_msg void OnLoadApp();
  afx_msg void OnUnloadApp();
  afx_msg void OnAppsListEvent();
  afx_msg void OnDestroy();
 // afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDIS);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADAPPS_H__8CD884D4_A935_478F_9494_EE0265C186AE__INCLUDED_)
