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

// DwfUnderlayDialog.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "PdfImportTextOptDialog.h"
#include <cstring>

#if _MSC_VER < 1400
#include <atlbase.h>
#include <atlwin.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdfImportTextOptDialog dialog


CPdfImportTextOptDialog::CPdfImportTextOptDialog(CWnd* pParent , std::map<OdAnsiString, OdAnsiString>& font_map)
	: CDialog(CPdfImportTextOptDialog::IDD, pParent)
  , m_FontMap(font_map)
{
	//{{AFX_DATA_INIT(CPdfImportTextOptDialog)


  m_ImportTrueTypeTextAsGeometry = FALSE;
  m_ImportType3AsTrueTypeText = FALSE;
  m_ImportInvalidTTTextAsGeometry = TRUE;
  m_CombineTextObjects = FALSE;

  m_ReplaceType3Font = FALSE;
  m_ReplaceNotFoundFont = FALSE;

  m_ReplaceFont1 = FALSE;
  m_ReplaceFont2 = FALSE;
  m_ReplaceFont3 = FALSE;

	//}}AFX_DATA_INIT
}

BOOL CPdfImportTextOptDialog::GetImportTrueTypeTextAsGeometry()
{
  return m_ImportTrueTypeTextAsGeometry;
}

void CPdfImportTextOptDialog::SetImportTrueTypeTextAsGeometry(BOOL val)
{
  m_ImportTrueTypeTextAsGeometry = val;
}

BOOL CPdfImportTextOptDialog::GetImportType3AsTrueTypeText()
{
  return m_ImportType3AsTrueTypeText;
}

void CPdfImportTextOptDialog::SetImportType3AsTrueTypeText(BOOL val)
{
  m_ImportType3AsTrueTypeText = val;
}

BOOL CPdfImportTextOptDialog::GetImportInvalidTTTextAsGeometry()
{
  return m_ImportInvalidTTTextAsGeometry;
}

void CPdfImportTextOptDialog::SetImportInvalidTTTextAsGeometry(BOOL val)
{
  m_ImportInvalidTTTextAsGeometry = val;
}

BOOL CPdfImportTextOptDialog::GetCombineTextObjects()
{
  return m_CombineTextObjects;
}

void CPdfImportTextOptDialog::SetCombineTextObjects(BOOL val)
{
  m_CombineTextObjects = val;
}

void CPdfImportTextOptDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPdfImportTextOptDialog)
  DDX_Check(pDX, IDC_TTF_AS_GEOM, m_ImportTrueTypeTextAsGeometry);
  DDX_Check(pDX, IDC_PDF_IMP_TYPE3_AS_TT, m_ImportType3AsTrueTypeText);
  DDX_Check(pDX, IDC_INVALID_TTF_AS_GEOM, m_ImportInvalidTTTextAsGeometry);
  DDX_Check(pDX, IDC_PDF_IMP_NEED_TO_COMB_TEXT, m_CombineTextObjects);
  DDX_Check(pDX, IDC_PDFIMPORT_CHANGE_TYPE3_FONT2, m_ReplaceType3Font);
  DDX_Control(pDX, IDC_PDFIMPORT_CHANGE_T3_COMBO, m_RepalceType3FontCombo);
  DDX_Text(pDX, IDC_PDFIMPORT_CHANGE_T3_COMBO, m_Type3FontString);
  DDX_Check(pDX, IDC_PDFIMPORT_CHANGE_NOTFOUND_FONT, m_ReplaceNotFoundFont);
  DDX_Control(pDX, IDC_PDFIMPORT_CHANGE_NF_COMBO, m_NotFoundFontCombo);
  DDX_Text(pDX, IDC_PDFIMPORT_CHANGE_NF_COMBO, m_NotFoundFontString);
  DDX_Check(pDX, IDC_PDFIMPORT_CHANGE_FONT1, m_ReplaceFont1);
  DDX_Control(pDX, IDC_EDIT_PDFIMPORT_FN1, m_ReplaceFontNameCombo1);
  DDX_Control(pDX, IDC_EDIT_PDFIMPORT_FF1, m_ReplaceFontFamilyCombo1);
  DDX_Text(pDX, IDC_EDIT_PDFIMPORT_FN1, m_ReplaceFontName1);
  DDX_Text(pDX, IDC_EDIT_PDFIMPORT_FF1, m_ReplaceFontFamily1);
  DDX_Control(pDX, IDC_PDFIMPORT_FCHANGE_COMBO1, m_ReplaceFontCombo1);
  DDX_Text(pDX, IDC_PDFIMPORT_FCHANGE_COMBO1, m_ReplaceFontString1);
  DDX_Check(pDX, IDC_PDFIMPORT_CHANGE_FONT2, m_ReplaceFont2);
  DDX_Control(pDX, IDC_EDIT_PDFIMPORT_FN2, m_ReplaceFontNameCombo2);
  DDX_Control(pDX, IDC_EDIT_PDFIMPORT_FF2, m_ReplaceFontFamilyCombo2);
  DDX_Text(pDX, IDC_EDIT_PDFIMPORT_FN2, m_ReplaceFontName2);
  DDX_Text(pDX, IDC_EDIT_PDFIMPORT_FF2, m_ReplaceFontFamily2);
  DDX_Control(pDX, IDC_PDFIMPORT_FCHANGE_COMBO2, m_ReplaceFontCombo2);
  DDX_Text(pDX, IDC_PDFIMPORT_FCHANGE_COMBO2, m_ReplaceFontString2);
  DDX_Check(pDX, IDC_PDFIMPORT_CHANGE_FONT3, m_ReplaceFont3);
  DDX_Control(pDX, IDC_EDIT_PDFIMPORT_FN3, m_ReplaceFontNameCombo3);
  DDX_Control(pDX, IDC_EDIT_PDFIMPORT_FF3, m_ReplaceFontFamilyCombo3);
  DDX_Text(pDX, IDC_EDIT_PDFIMPORT_FN3, m_ReplaceFontName3);
  DDX_Text(pDX, IDC_EDIT_PDFIMPORT_FF3, m_ReplaceFontFamily3);
  DDX_Control(pDX, IDC_PDFIMPORT_FCHANGE_COMBO3, m_ReplaceFontCombo3);
  DDX_Text(pDX, IDC_PDFIMPORT_FCHANGE_COMBO3, m_ReplaceFontString3);
  DDX_Check(pDX, IDC_PDF_IMPORT_EXTRACT_EMBEDD_FONTS, m_ExtractEmbeddedFonts);

  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPdfImportTextOptDialog, CDialog)
	//{{AFX_MSG_MAP(CPdfImportTextOptDialog)
  ON_BN_CLICKED(IDC_PDFIMPORT_CHANGE_TYPE3_FONT2, OnCheckReplaceType3Font)
  ON_BN_CLICKED(IDC_PDFIMPORT_CHANGE_NOTFOUND_FONT, OnCheckReplaceNotFoundFont)
  ON_BN_CLICKED(IDC_PDFIMPORT_CHANGE_FONT1, OnCheckReplaceFont1)
  ON_BN_CLICKED(IDC_PDFIMPORT_CHANGE_FONT2, OnCheckReplaceFont2)
  ON_BN_CLICKED(IDC_PDFIMPORT_CHANGE_FONT3, OnCheckReplaceFont3)
  ON_CBN_SELENDOK(IDC_EDIT_PDFIMPORT_FN1, OnComboFN1Changed)
  ON_CBN_EDITUPDATE(IDC_EDIT_PDFIMPORT_FN1, OnComboFN1Edited)
  ON_BN_CLICKED(IDOK, OnSaveSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPdfImportTextOptDialog message handlers

static std::set<CString> fonts;

int CALLBACK EnumFontFamExProc(ENUMLOGFONTEXA* lpelfe,
  NEWTEXTMETRICEX* lpntme, DWORD FontType, LPARAM lParam)
{
  if (FontType != 0x004 || strchr(lpelfe->elfLogFont.lfFaceName, '@'))
    return true;

  fonts.insert(lpelfe->elfLogFont.lfFaceName);
  return true;
}


BOOL CPdfImportTextOptDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  CenterWindow();

  if (0 == fonts.size())
  {
    HWND hwnd = m_hWnd;
    HDC hdc = ::GetDC(hwnd);
    LOGFONTA  lf;
    lf.lfFaceName[0] = '\0';
    lf.lfCharSet = DEFAULT_CHARSET;
    EnumFontFamiliesExA(hdc, &lf, (FONTENUMPROCA)EnumFontFamExProc, (LPARAM)0, (DWORD)0);
    ::ReleaseDC(hwnd, hdc);
  }

  UpdateData(true); // save and validate

  for (std::set<CString>::const_iterator iter = fonts.begin(); iter != fonts.end(); ++iter)
  {
    const int idx = (int)std::distance(static_cast<std::set<CString>::const_iterator>(fonts.begin()), iter);

    m_RepalceType3FontCombo.InsertString(idx, *iter);
    m_NotFoundFontCombo.InsertString(idx, *iter);
    m_ReplaceFontCombo1.InsertString(idx, *iter);
    m_ReplaceFontCombo2.InsertString(idx, *iter);
    m_ReplaceFontCombo3.InsertString(idx, *iter);
  }

  for (std::map<OdAnsiString, OdAnsiString>::iterator iter = m_FontMap.begin(); iter != m_FontMap.end(); ++iter)
  {
    const int idx = (int)std::distance(m_FontMap.begin(), iter);
    OdString font_name(iter->first);
    OdString font_family(iter->second);
    
    m_ReplaceFontNameCombo1.InsertString(idx, font_name.c_str());
    m_ReplaceFontFamilyCombo1.InsertString(idx, font_family.c_str());

    m_ReplaceFontNameCombo2.InsertString(idx, font_name.c_str());
    m_ReplaceFontFamilyCombo2.InsertString(idx, font_family.c_str());

    m_ReplaceFontNameCombo3.InsertString(idx, font_name.c_str());
    m_ReplaceFontFamilyCombo3.InsertString(idx, font_family.c_str());
  }

  int import_mode = AfxGetApp()->GetProfileInt(L"PdfImport", L"PDFIMPORT_TEXT_MODE", 0);
  m_ReplaceType3Font = GETBIT(import_mode, 1);
  m_ReplaceNotFoundFont = GETBIT(import_mode, 2);

  m_ReplaceFont1 = GETBIT(import_mode, 4);
  m_ReplaceFont2 = GETBIT(import_mode, 8);
  m_ReplaceFont3 = GETBIT(import_mode, 16);

  UpdateData(false);

  OnCheckReplaceType3Font();
  OnCheckReplaceNotFoundFont();
  
  OnCheckReplaceFont1();
  OnCheckReplaceFont2();
  OnCheckReplaceFont3();

  return TRUE;
}

void CPdfImportTextOptDialog::OnCheckReplaceType3Font()
{
  UpdateData(true);
  GetDlgItem(IDC_PDFIMPORT_CHANGE_T3_COMBO)->EnableWindow(m_ReplaceType3Font);
  UpdateData(false);
}

void CPdfImportTextOptDialog::OnCheckReplaceNotFoundFont()
{
  UpdateData(true);
  GetDlgItem(IDC_PDFIMPORT_CHANGE_NF_COMBO)->EnableWindow(m_ReplaceNotFoundFont);
  UpdateData(false);
}

void CPdfImportTextOptDialog::OnCheckReplaceFont1()
{
  UpdateData(true);
  GetDlgItem(IDC_EDIT_PDFIMPORT_FN1)->EnableWindow(m_ReplaceFont1);
  GetDlgItem(IDC_EDIT_PDFIMPORT_FF1)->EnableWindow(m_ReplaceFont1);
  GetDlgItem(IDC_PDFIMPORT_FCHANGE_COMBO1)->EnableWindow(m_ReplaceFont1);
  UpdateData(false);
}

void CPdfImportTextOptDialog::OnCheckReplaceFont2()
{
  UpdateData(true);
  GetDlgItem(IDC_EDIT_PDFIMPORT_FN2)->EnableWindow(m_ReplaceFont2);
  GetDlgItem(IDC_EDIT_PDFIMPORT_FF2)->EnableWindow(m_ReplaceFont2);
  GetDlgItem(IDC_PDFIMPORT_FCHANGE_COMBO2)->EnableWindow(m_ReplaceFont2);
  UpdateData(false);
}

void CPdfImportTextOptDialog::OnCheckReplaceFont3()
{
  UpdateData(true);
  GetDlgItem(IDC_EDIT_PDFIMPORT_FN3)->EnableWindow(m_ReplaceFont3);
  GetDlgItem(IDC_EDIT_PDFIMPORT_FF3)->EnableWindow(m_ReplaceFont3);
  GetDlgItem(IDC_PDFIMPORT_FCHANGE_COMBO3)->EnableWindow(m_ReplaceFont3);
  UpdateData(false);
}

void CPdfImportTextOptDialog::SelectFontFamily(CString& font_name, CComboBox& family_combo)
{
  OdString tmp2(font_name);
  OdAnsiString name(tmp2);
  std::map<OdAnsiString, OdAnsiString>::iterator iter = m_FontMap.find(name);
  if (m_FontMap.end() != iter)
  {
    family_combo.SetCurSel(std::distance(m_FontMap.begin(), iter));
  }
}

void CPdfImportTextOptDialog::OnComboFN1Changed()
{
  CString tmp;
  m_ReplaceFontNameCombo1.GetLBText(m_ReplaceFontNameCombo1.GetCurSel(), tmp);
  SelectFontFamily(tmp, m_ReplaceFontFamilyCombo1);
}

void CPdfImportTextOptDialog::OnComboFN1Edited()
{
  UpdateData();
  CString tmp;
  GetDlgItem(IDC_EDIT_PDFIMPORT_FN1)->GetWindowText(tmp);
  SelectFontFamily(tmp, m_ReplaceFontFamilyCombo1);
}

void CPdfImportTextOptDialog::OnComboFN2Changed()
{
  CString tmp;
  m_ReplaceFontNameCombo2.GetLBText(m_ReplaceFontNameCombo2.GetCurSel(), tmp);
  SelectFontFamily(tmp, m_ReplaceFontFamilyCombo2);
}

void CPdfImportTextOptDialog::OnComboFN2Edited()
{
  UpdateData();
  CString tmp;
  GetDlgItem(IDC_EDIT_PDFIMPORT_FN2)->GetWindowText(tmp);
  SelectFontFamily(tmp, m_ReplaceFontFamilyCombo2);
}

void CPdfImportTextOptDialog::OnComboFN3Changed()
{
  CString tmp;
  m_ReplaceFontNameCombo3.GetLBText(m_ReplaceFontNameCombo3.GetCurSel(), tmp);
  SelectFontFamily(tmp, m_ReplaceFontFamilyCombo3);
}

void CPdfImportTextOptDialog::OnComboFN3Edited()
{
  UpdateData();
  CString tmp;
  GetDlgItem(IDC_EDIT_PDFIMPORT_FN3)->GetWindowText(tmp);
  SelectFontFamily(tmp, m_ReplaceFontFamilyCombo3);
}

void CPdfImportTextOptDialog::OnSaveSettings()
{
  int import_mode(0);
  SETBIT(import_mode, 1, m_ReplaceType3Font);
  SETBIT(import_mode, 2, m_ReplaceNotFoundFont);
  SETBIT(import_mode, 4, m_ReplaceFont1);
  SETBIT(import_mode, 8, m_ReplaceFont2);
  SETBIT(import_mode, 16, m_ReplaceFont3);
  AfxGetApp()->WriteProfileInt(L"PdfImport", L"PDFIMPORT_TEXT_MODE", import_mode);

  CPdfImportTextOptDialog::OnOK();
}

CString CPdfImportTextOptDialog::GetType3FontString()
{
  if (m_ReplaceType3Font)
    return m_Type3FontString;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetType3FontString(CString& val)
{
  m_Type3FontString = val;
}

CString CPdfImportTextOptDialog::GetNotFoundFontString()
{
  if (m_ReplaceNotFoundFont)
    return m_NotFoundFontString;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetNotFoundFontString(CString& val)
{
  m_NotFoundFontString = val;
}

CString CPdfImportTextOptDialog::GetReplaceFontName1()
{
  if (m_ReplaceFont1)
    return m_ReplaceFontName1;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetReplaceFontName1(CString& val)
{
  m_ReplaceFontName1 = val;
}

CString CPdfImportTextOptDialog::GetReplaceFontFamily1()
{
  if (m_ReplaceFont1)
    return m_ReplaceFontFamily1;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetReplaceFontFamily1(CString& val)
{
  m_ReplaceFontFamily1 = val;
}

CString CPdfImportTextOptDialog::GetReplaceFontString1()
{
  if (m_ReplaceFont1)
    return m_ReplaceFontString1;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetReplaceFontString1(CString& val)
{
  m_ReplaceFontString1 = val;
}

CString CPdfImportTextOptDialog::GetReplaceFontName2()
{
  if (m_ReplaceFont2)
    return m_ReplaceFontName2;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetReplaceFontName2(CString& val)
{
  m_ReplaceFontName2 = val;
}

CString CPdfImportTextOptDialog::GetReplaceFontFamily2()
{
  if (m_ReplaceFont2)
    return m_ReplaceFontFamily2;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetReplaceFontFamily2(CString& val)
{
  m_ReplaceFontFamily2 = val;
}

CString CPdfImportTextOptDialog::GetReplaceFontString2()
{
  if (m_ReplaceFont2)
    return m_ReplaceFontString2;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetReplaceFontString2(CString& val)
{
  m_ReplaceFontString2 = val;
}

CString CPdfImportTextOptDialog::GetReplaceFontName3()
{
  if (m_ReplaceFont3)
    return m_ReplaceFontName3;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetReplaceFontName3(CString& val)
{
  m_ReplaceFontName3 = val;
}

CString CPdfImportTextOptDialog::GetReplaceFontFamily3()
{
  if (m_ReplaceFont3)
    return m_ReplaceFontFamily3;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetReplaceFontFamily3(CString& val)
{
  m_ReplaceFontFamily3 = val;
}

CString CPdfImportTextOptDialog::GetReplaceFontString3()
{
  if (m_ReplaceFont3)
    return m_ReplaceFontString3;
  else
    return CString();
}

void CPdfImportTextOptDialog::SetReplaceFontString3(CString& val)
{
  m_ReplaceFontString3 = val;
}

BOOL CPdfImportTextOptDialog::GetExtractEmbeddedFonts()
{
  return m_ExtractEmbeddedFonts;
}

void CPdfImportTextOptDialog::SetExtractEmbeddedFonts(BOOL val)
{
  m_ExtractEmbeddedFonts = val;
}

void CPdfImportTextOptDialog::OnError(CString str)
{
  MessageBox(str, CString("Error"));
}

void CPdfImportTextOptDialog::OnError(OdResult res)
{
  OdError err(res);
  CString str((LPCTSTR)err.description());
  OnError(str);
}

