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

#if !defined(AFX_ACISSAVEDIALOG_H__75543719_5B21_4528_8260_1CDE2EB4293E__INCLUDED_)
#define AFX_ACISSAVEDIALOG_H__75543719_5B21_4528_8260_1CDE2EB4293E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AcisSaveDialog.h : header file
//

#include "ModelerGeometry.h"

/////////////////////////////////////////////////////////////////////////////
// CAcisSaveDialog dialog

class CAcisSaveDialog : public CDialog
{
  OdDbObjectPtr                 m_pObject;
  OdDbObjectId                  m_id;
  OdArray<OdModelerGeometryPtr> m_Bodies;
  bool                          m_bReadOnly;

  OdModelerGeometryPtr selectedBody();
  void UpdateBodyList();
  void UpdateEditBox();
  void InitBaseVersion();
  AfTypeVer getSelectedByUserVersion(bool bSab = false) const;

  void createTypedEntity(OdModelerGeometryPtr pModeller, OdDbBlockTableRecordPtr pBlock, OdSmartPtr<OdStreamBuf> pOut);

// Construction
public:
	CAcisSaveDialog(const OdDbObjectId &id, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAcisSaveDialog)
	enum { IDD = IDD_ACIS_SAVE_TO };
	CListBox	m_BodyList;
	CEdit	    m_AcisEditCtrl;
	CComboBox	m_AcisVerCombo;
	CString	  m_ACIS_base_ver;
	BOOL	    m_bAddIndexes;
	BOOL	    m_bStandardSave;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcisSaveDialog)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAcisSaveDialog)
	afx_msg void OnCopy();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckIndexes();
	afx_msg void OnCheckStandardSave();
	afx_msg void OnSelchangeAcisVer();
	afx_msg void OnSave();
  afx_msg void OnSaveSab();
	afx_msg void OnLoadfrom();
	afx_msg void OnCreatenew();
	afx_msg void OnSettoentity();
	afx_msg void OnSelchangeBodyList();
	afx_msg void OnCreatenewall();
  afx_msg void OnBBCheck();
  afx_msg void OnAcisCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACISSAVEDIALOG_H__75543719_5B21_4528_8260_1CDE2EB4293E__INCLUDED_)
