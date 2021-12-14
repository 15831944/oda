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
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "DgXAttribute.h"


// CLinkArrtDlg dialog

class CLinkArrtDlg : public CDialog
{
	DECLARE_DYNAMIC(CLinkArrtDlg)

public:
	//CLinkArrtDlg(CWnd* pParent = NULL);   // standard constructor
	CLinkArrtDlg( const OdDgElementId& idCurrentElement, CWnd* pParent = NULL);
	virtual ~CLinkArrtDlg();

	BOOL OnInitDialog();



// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINKS_ATTRS };
#endif

protected:
	OdDgElementId      m_idCurrentElement;
	OdRxObjectPtrArray m_linkagesArray;
	OdRxObjectPtrArray m_xAttributesArray;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void ApplyChangesToAttributes();
	void ApplyChangesToLinkages();
	void EraseSelectedLinkage();
	void EraseSelectedAttribute();
	void ShiftSelectedLinkage(int iDirection);
	void ShiftSelectedAttribute(int iDirection);
	void InitLinkageArray();
	void InitXAttributesArray();
	void UpdateLinksTable();
	void UpdateAttrsTable();
	void SetLinksButtonsState();
	void SetAttrsButtonsState();
	void SwitchPage(unsigned int Page);
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	void AddData(CListCtrl & ctrl, int row, int col, LPWSTR str);

	OdString GetAttributeClassName(const OdDgXAttributePtr & pXAttribute);



	OdString GetLinkageClassName(const OdDgAttributeLinkagePtr &pLinkage);
	
	CButton m_buttonUpLink;
	CButton m_buttonDownLink;
	CButton m_buttonDelLink;
	CListCtrl m_listControl;

	CListCtrl m_listControlAttr;
	CButton m_buttonUpAttr;
	CButton m_buttonDownAttr;
	CButton m_buttonDelAttr;

	CButton m_buttonLinkages;
	CButton m_buttonAttributes;
	
	afx_msg	void OnBnClickedOk2();
	afx_msg void OnBnClickedButtonDeleteLink();
	afx_msg void OnBnClickedButtonDownLink();
	afx_msg void OnBnClickedButtonUpLink();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnNMClickListAttrs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
};

std::string GetLinkageClassName(OdDgAttributeLinkagePtr pLinkage);
