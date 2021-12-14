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

#if !defined(AFX_VECTPROPS_H__F10E7051_5C9B_43B8_BAC8_C31A8A7921AA__INCLUDED_)
#define AFX_VECTPROPS_H__F10E7051_5C9B_43B8_BAC8_C31A8A7921AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VectProps.h : header file
//

#include "resource.h"
#include <limits.h>

/////////////////////////////////////////////////////////////////////////////
// CSimpleInputDlg dialog

class CSimpleInputDlg : protected CDialog
{
// Construction
public:
	CSimpleInputDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSimpleInputDlg)
	enum { IDD = IDD_SIMPLE_INPUT };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimpleInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
  bool DoInput(LPCTSTR pCaption, LPCTSTR pDesc, bool *pBool);
  bool DoInput(LPCTSTR pCaption, LPCTSTR pDesc, OdInt32 *pInt, OdInt32 rangeFrom = INT_MIN, OdInt32 rangeTo = INT_MAX);
  bool DoInput(LPCTSTR pCaption, LPCTSTR pDesc, OdUInt32 *pUInt, OdUInt32 rangeFrom = 0, OdUInt32 rangeTo = UINT_MAX);
  bool DoInput(LPCTSTR pCaption, LPCTSTR pDesc, double *pDouble, double rangeFrom = -DBL_MAX, double rangeTo = DBL_MAX);
  bool DoInput(LPCTSTR pCaption, LPCTSTR pDesc, OdString *pStr);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSimpleInputDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  enum IpType
  {
    kIpNone, kIpBool, kIpInt, kIpUInt, kIpDouble, kIpString
  } m_IpType;
  void *m_pIp;
  union IpLim
  {
    OdInt32 m_int[2];
    OdUInt32 m_uint[2];
    double m_double[2];
  } m_IpLim;
  CString m_IpCap;
  CString m_IpDesc;

  void OnOK();
};

class OdGsDevice;
class OdGsModule;

/////////////////////////////////////////////////////////////////////////////
// CVectPropsDlg dialog

class CVectPropsDlg : public CDialog
{
// Construction
public:
	CVectPropsDlg(OdGsDevice* pDevice, OdGsModule *pModule = NULL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVectPropsDlg)
	enum { IDD = IDD_RENDER_VECTPROPS };
	CListCtrl m_PropsList;
  CButton   m_BitmapDevice;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVectPropsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVectPropsDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  OdGsDevice* m_pDevice;
  OdGsModule* m_pModule;
protected:
  afx_msg void OnClickListPropsList(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnBitmapDeviceButtonClicked();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECTPROPS_H__F10E7051_5C9B_43B8_BAC8_C31A8A7921AA__INCLUDED_)
