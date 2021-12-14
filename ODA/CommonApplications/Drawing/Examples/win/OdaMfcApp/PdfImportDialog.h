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

#if !defined(PDF_IMPORT_DIALOG_INCLUDED_)
#define PDF_IMPORT_DIALOG_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DbUnderlayDefinition.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CPdfImportDialog dialog

class CPdfImportDialog : public CDialog
{

private:
  OdDbUnderlayFilePtr m_pFile;
  OdDbDatabase        *m_pDb;
  CString             m_Path;
  CListCtrl           m_SheetList;
  CImageList          m_stateImages;
  CComboBox           m_comboRotation;
  CComboBox           m_comboHyperLink;
  int                 m_LayersUseType;
  BOOL                m_ImportVectorGeometry;
  BOOL                m_ImportSolidFills;
  BOOL                m_ImportTrueTypeText;
  BOOL                m_ImportRasterImages;
  BOOL                m_ImportGradientFills;
  BOOL                m_ImportAsBlock;
  BOOL                m_JoinLineAndArcSegments;
  BOOL                m_ConvertSolidsToHatches;
  BOOL                m_ApplyLineweight;
  BOOL                m_ImportTrueTypeTextAsGeometry;
  BOOL                m_UseGeometryOptimization;
  BOOL                m_ImportType3AsTrueTypeText;
  BOOL                m_ImportAsRgb;
  BOOL                m_ImportInvalidTTTextAsGeometry;
  BOOL                m_UseClipping;
  BOOL                m_UsePageBorderClipping;
  BOOL                m_UseImageClipping;
  BOOL                m_CombineTextObjects;
  BOOL                m_UsePdfTransparency;
  BOOL                m_UseINSUNITS;
  double              m_Scaling;
  double              m_Rotation;
  BOOL                m_UseInsertionPoint;
  double              m_SelectedPageNum;
  OdString            m_Password;
  BOOL                m_ImportWidgets;
  CString             m_Type3FontString;
  CString             m_NotFoundFontString;
  CString             m_ReplaceFontName1;
  CString             m_ReplaceFontFamily1;
  CString             m_ReplaceFontString1;
  CString             m_ReplaceFontName2;
  CString             m_ReplaceFontFamily2;
  CString             m_ReplaceFontString2;
  CString             m_ReplaceFontName3;
  CString             m_ReplaceFontFamily3;
  CString             m_ReplaceFontString3;
  BOOL                m_ExtractEmbeddedFonts;

  OdString            m_CommandString;
  int                 m_DialogHeight;//Initial height
  std::map<OdAnsiString, OdAnsiString> m_FontMap;


// Construction

public:

  CPdfImportDialog(OdDbDatabase *pDb, CWnd* pParent);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPdfImportDialog)
  enum { IDD = IDD_PDF_IMPORT_DIALOG };
	//}}AFX_DATA

  OdString GetCommandString();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPdfImportDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  afx_msg void OnSelchangeSheetList(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnPageNumChanged();
  afx_msg void OnCheckImportVectorGeometry();
  afx_msg void OnCheckImportClipping();
  //}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPdfImportDialog)
	afx_msg void OnBrowse();
  afx_msg void OnEditTextOptions();
	afx_msg void OnInsert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  bool InitProperties();
  void OnError(CString str);
  void OnError(OdResult res);
  void FillSheetList();

public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PDF_IMPORT_DIALOG_INCLUDED_
