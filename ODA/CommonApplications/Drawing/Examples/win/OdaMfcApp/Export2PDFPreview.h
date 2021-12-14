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

#if !defined(AFX_Export2PDFPreview_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
#define AFX_Export2PDFPreview_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Export2PDFPreview.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExport2PDFPreview dialog

class CExport2PDFPreview : public CDialog
{
  HBITMAP m_hBitmap;
// Construction
public:
	CExport2PDFPreview(CWnd* pParent = NULL);   // standard constructor
  ~CExport2PDFPreview();
// Dialog Data
	//{{AFX_DATA(CExport2PDFPreview)
	enum { IDD = IDD_PDFEXPORT_PREVIEW };
	CComboBox         m_Layouts;
  CStatic           m_PdfPreview;
  BOOL              m_bSlowMode;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExport2PDFPreview)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

  virtual BOOL OnInitDialog();
  HBITMAP CreateBitmapFromPdf(RECT &rc);
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExport2PDFPreview)
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnPaint();
  afx_msg void OnExitSizeMove();
  afx_msg void OnEnterSizeMove();
  afx_msg void OnSelChangeLayout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
  OdDbHostAppServices *m_pHost;
  OdString             m_pdfFilePath;
  int m_nPaperW;
  int m_nPaperH;
  bool m_BoolSizing;

public:
  void fillLayouts(OdArray<OdString> layouts, OdRxObjectPtrArray databases);
  void Init(OdString pdfFilePath, OdDbHostAppServices *pHost);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_Export2PDFPreview_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
