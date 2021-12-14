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


#if !defined(AFX_PDF_SELECT_VIEW_DIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
#define AFX_PDF_SELECT_VIEW_DIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxwin.h"
#include "resource.h"

#include "DgDatabase.h"

class CPdfExportSelectViewsDialog : public CDialog
{
	//DECLARE_DYNAMIC(CPdfExportSelectViewsDialog)

  OdDgDatabasePtr    m_pDb;
  OdArray<OdString> *m_pLayouts;
  OdArray<OdString> m_LayoutsViews;

  CListBox          m_ViewList;

public:
	CPdfExportSelectViewsDialog(OdDgDatabasePtr pDb, OdArray<OdString> *layouts, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPdfExportSelectViewsDialog();

// Dialog Data
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PDF_EXPORT_LAYOUT_VIEWS };
//#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  afx_msg void OnOk();
  afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()
};

#endif //AFX_PDF_SELECT_VIEW_DIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_
