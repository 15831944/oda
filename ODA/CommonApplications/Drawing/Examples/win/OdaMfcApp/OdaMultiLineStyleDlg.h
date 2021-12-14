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

#if !defined(AFX_ODAMULTILINESTYLEDLG_H__8B71789E_7314_46D4_BA2F_EEFE39F6AB0B__INCLUDED_)
#define AFX_ODAMULTILINESTYLE_H__8B71789E_7314_46D4_BA2F_EEFE39F6AB0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OdaMultiLineStyle.h : header file
//

#include "OdaMfcApp.h"

/////////////////////////////////////////////////////////////////////////////
// OdaLayerFilterPropDlg dialog
//
class OdaMultiLineStyleDlg : public CDialog
{

    DECLARE_DYNCREATE(OdaMultiLineStyleDlg)

private:

    OdDbDatabase *m_pDb;

    const bool fillComboMLStyleCurrent();

public:
    OdaMultiLineStyleDlg(CWnd* pParent = NULL);   // standard constructor
    OdaMultiLineStyleDlg(OdDbDatabase *pDb, CWnd* pParent = NULL);

    BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog);

// Dialog Data
	//{{AFX_DATA(OdaLayerFilterPropDlg)
	enum { IDD = IDD_MLSTYLE };
    CComboBox	m_comboMLStyleCurrent;
    CEdit       m_editMLStyleName;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OdaLayerFilterPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OdaLayerFilterPropDlg)
    afx_msg void OnClickBtnMLStyleLoad();
    afx_msg void OnClickBtnMLStyleSave();
    afx_msg void OnSelchangeComboMLStyleCurrent();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODAMULTILINESTYLE_H__8B71789E_7314_46D4_BA2F_EEFE39F6AB0B__INCLUDED_)
