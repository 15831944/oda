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

#if !defined(PDF_IMPORT_TEXT_OPT_DIALOG_INCLUDED_)
#define PDF_IMPORT_TEXT_OPT_DIALOG_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CPdfImportTextOptDialog dialog

class CPdfImportTextOptDialog : public CDialog
{

private:
  BOOL                m_ImportTrueTypeTextAsGeometry;
  BOOL                m_ImportType3AsTrueTypeText;
  BOOL                m_ImportInvalidTTTextAsGeometry;
  BOOL                m_CombineTextObjects;
  BOOL                m_ReplaceType3Font;
  CString             m_Type3FontString;
  CComboBox           m_RepalceType3FontCombo;
  BOOL                m_ReplaceNotFoundFont;
  CString             m_NotFoundFontString;
  CComboBox           m_NotFoundFontCombo;
  BOOL                m_ReplaceFont1;
  CComboBox           m_ReplaceFontNameCombo1;
  CComboBox           m_ReplaceFontFamilyCombo1;
  CString             m_ReplaceFontName1;
  CString             m_ReplaceFontFamily1;
  CString             m_ReplaceFontString1;
  CComboBox           m_ReplaceFontCombo1;
  BOOL                m_ReplaceFont2;
  CComboBox           m_ReplaceFontNameCombo2;
  CComboBox           m_ReplaceFontFamilyCombo2;
  CString             m_ReplaceFontName2;
  CString             m_ReplaceFontFamily2;
  CString             m_ReplaceFontString2;
  CComboBox           m_ReplaceFontCombo2;
  BOOL                m_ReplaceFont3;
  CComboBox           m_ReplaceFontNameCombo3;
  CComboBox           m_ReplaceFontFamilyCombo3;
  CString             m_ReplaceFontName3;
  CString             m_ReplaceFontFamily3;
  CString             m_ReplaceFontString3;
  CComboBox           m_ReplaceFontCombo3;
  BOOL                m_ExtractEmbeddedFonts;


  std::map<OdAnsiString, OdAnsiString>& m_FontMap;

// Construction

public:

  CPdfImportTextOptDialog(CWnd* pParent, std::map<OdAnsiString, OdAnsiString>& font_map);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPdfImportTextOptDialog)
  enum { IDD = IDD_PDF_IMPORT_TEXT_OPT_DIALOG };
	//}}AFX_DATA

  BOOL GetImportTrueTypeTextAsGeometry();
  void SetImportTrueTypeTextAsGeometry(BOOL val);
  BOOL GetImportType3AsTrueTypeText();
  void SetImportType3AsTrueTypeText(BOOL val);
  BOOL GetImportInvalidTTTextAsGeometry();
  void SetImportInvalidTTTextAsGeometry(BOOL val);
  BOOL GetCombineTextObjects();
  void SetCombineTextObjects(BOOL val);

  CString GetType3FontString();
  void SetType3FontString(CString& val);

  CString GetNotFoundFontString();
  void SetNotFoundFontString(CString& val);

  CString GetReplaceFontName1();
  void SetReplaceFontName1(CString& val);
  CString GetReplaceFontFamily1();
  void SetReplaceFontFamily1(CString& val);
  CString GetReplaceFontString1();
  void SetReplaceFontString1(CString& val);

  CString GetReplaceFontName2();
  void SetReplaceFontName2(CString& val);
  CString GetReplaceFontFamily2();
  void SetReplaceFontFamily2(CString& val);
  CString GetReplaceFontString2();
  void SetReplaceFontString2(CString& val);

  CString GetReplaceFontName3();
  void SetReplaceFontName3(CString& val);
  CString GetReplaceFontFamily3();
  void SetReplaceFontFamily3(CString& val);
  CString GetReplaceFontString3();
  void SetReplaceFontString3(CString& val);

  BOOL GetExtractEmbeddedFonts();
  void SetExtractEmbeddedFonts(BOOL val);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPdfImportTextOptDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  afx_msg void OnCheckReplaceType3Font();
  afx_msg void OnCheckReplaceNotFoundFont();

  afx_msg void OnCheckReplaceFont1();
  afx_msg void OnCheckReplaceFont2();
  afx_msg void OnCheckReplaceFont3();

  afx_msg void OnComboFN1Changed();
  afx_msg void OnComboFN1Edited();

  afx_msg void OnComboFN2Changed();
  afx_msg void OnComboFN2Edited();

  afx_msg void OnComboFN3Changed();
  afx_msg void OnComboFN3Edited();
  //}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPdfImportTextOptDialog)
	afx_msg void OnSaveSettings();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void OnError(CString str);
  void OnError(OdResult res);

  void SelectFontFamily(CString& font_name, CComboBox& family_combo);

public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PDF_IMPORT_TEXT_OPT_DIALOG_INCLUDED_
