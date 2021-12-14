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

// LwSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "LwSettingsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLwSettingsDlg dialog


CLwSettingsDlg::CLwSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLwSettingsDlg::IDD, pParent)
  , m_bListMM(TRUE)
  , m_defaultIndex(0)
  , m_lwIndex(0)
  , m_defaultLw(OdDb::kLnWt000)
  , m_currentLw(OdDb::kLnWtByLayer)
  , m_bDisplayLw(true)
  , m_lwScaleIndex(5)
{
	//{{AFX_DATA_INIT(CLwSettingsDlg)
	//}}AFX_DATA_INIT
}


inline int lw2Index(OdDb::LineWeight lw, const CListBox* pLws) 
{
  int i = 0;
  while(OdDb::LineWeight(pLws->GetItemData(i)) != lw)
  {
    ++i;
  }
  return i;
}

inline OdDb::LineWeight index2Lw(int n, const CListBox* pLws) 
{
  return OdDb::LineWeight(pLws->GetItemData(n));
}

void CLwSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLwSettingsDlg)
	//}}AFX_DATA_MAP

  if(pDX->m_bSaveAndValidate)
  {
    CListBox* pLws = (CListBox*)GetDlgItem(IDC_LINEWEIGHTS);
    m_currentLw = index2Lw(m_lwIndex, pLws);
    m_defaultLw = index2Lw(m_defaultIndex + 3, pLws);
    m_bDisplayLw = (((CButton*)GetDlgItem(IDC_DISPLAY_LINEWEIGHT))->GetCheck()!=0);
    m_lwScaleIndex = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCALE))->GetPos();
  }
}


BEGIN_MESSAGE_MAP(CLwSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CLwSettingsDlg)
	ON_BN_CLICKED(IDC_RADIO_MM, OnRadioMm)
	ON_BN_CLICKED(IDC_RADIO_IN, OnRadioIn)
	ON_CBN_SELCHANGE(IDC_COMBO_DEFAULT, OnSelchangeComboDefault)
	ON_LBN_SELCHANGE(IDC_LINEWEIGHTS, OnSelchangeLineweights)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLwSettingsDlg message handlers

BOOL CLwSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
  ((CButton*)GetDlgItem(IDC_RADIO_MM))->SetCheck(TRUE);
  ((CButton*)GetDlgItem(IDC_DISPLAY_LINEWEIGHT))->SetCheck(m_bDisplayLw ? TRUE : FALSE);

  CSliderCtrl* pSlider = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_SCALE));
  pSlider->SetRange(0, 18);
  pSlider->SetPos(m_lwScaleIndex);
  pSlider->SetTicFreq(2);
  fillList();

  CListBox* pLws = (CListBox*)GetDlgItem(IDC_LINEWEIGHTS);
  m_lwIndex = lw2Index(m_currentLw, pLws);
  pLws->SetCurSel(m_lwIndex);
  m_defaultIndex = lw2Index(m_defaultLw, pLws) - 3;
  ((CComboBox*)GetDlgItem(IDC_COMBO_DEFAULT))->SetCurSel(m_defaultIndex);
  OnSelchangeLineweights();
	return TRUE;
}

void CLwSettingsDlg::fillList() 
{
  static TCHAR* strings[2][24] =
  {
    {
      _T(" 0.000\""),
      _T(" 0.002\""),
      _T(" 0.004\""),
      _T(" 0.005\""),
      _T(" 0.006\""),
      _T(" 0.007\""),
      _T(" 0.008\""),
      _T(" 0.010\""),
      _T(" 0.012\""),
      _T(" 0.014\""),
      _T(" 0.016\""),
      _T(" 0.020\""),
      _T(" 0.021\""),
      _T(" 0.024\""),
      _T(" 0.028\""),
      _T(" 0.031\""),
      _T(" 0.035\""),
      _T(" 0.039\""),
      _T(" 0.042\""),
      _T(" 0.047\""),
      _T(" 0.055\""),
      _T(" 0.062\""),
      _T(" 0.079\""),
      _T(" 0.083\"")
    },
    {
      _T(" 0.00 mm"),
      _T(" 0.05 mm"),
      _T(" 0.09 mm"),
      _T(" 0.13 mm"),
      _T(" 0.15 mm"),
      _T(" 0.18 mm"),
      _T(" 0.20 mm"),
      _T(" 0.25 mm"),
      _T(" 0.30 mm"),
      _T(" 0.35 mm"),
      _T(" 0.40 mm"),
      _T(" 0.50 mm"),
      _T(" 0.53 mm"),
      _T(" 0.60 mm"),
      _T(" 0.70 mm"),
      _T(" 0.80 mm"),
      _T(" 0.90 mm"),
      _T(" 1.00 mm"),
      _T(" 1.06 mm"),
      _T(" 1.20 mm"),
      _T(" 1.40 mm"),
      _T(" 1.58 mm"),
      _T(" 2.00 mm"),
      _T(" 2.11 mm")
    }
  };

	CListBox* pLws = (CListBox*)GetDlgItem(IDC_LINEWEIGHTS);

  pLws->ResetContent();

  pLws->SetItemData(pLws->AddString(_T("ByLayer")), UINT(OdDb::kLnWtByLayer));
  pLws->SetItemData(pLws->AddString(_T("ByBlock")), UINT(OdDb::kLnWtByBlock));
  pLws->SetItemData(pLws->AddString(_T("Default")), UINT(OdDb::kLnWtByLwDefault));

  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 0]), OdDb::kLnWt000);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 1]), OdDb::kLnWt005);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 2]), OdDb::kLnWt009);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 3]), OdDb::kLnWt013);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 4]), OdDb::kLnWt015);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 5]), OdDb::kLnWt018);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 6]), OdDb::kLnWt020);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 7]), OdDb::kLnWt025);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 8]), OdDb::kLnWt030);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][ 9]), OdDb::kLnWt035);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][10]), OdDb::kLnWt040);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][11]), OdDb::kLnWt050);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][12]), OdDb::kLnWt053);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][13]), OdDb::kLnWt060);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][14]), OdDb::kLnWt070);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][15]), OdDb::kLnWt080);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][16]), OdDb::kLnWt090);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][17]), OdDb::kLnWt100);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][18]), OdDb::kLnWt106);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][19]), OdDb::kLnWt120);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][20]), OdDb::kLnWt140);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][21]), OdDb::kLnWt158);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][22]), OdDb::kLnWt200);
  pLws->SetItemData(pLws->AddString(strings[m_bListMM][23]), OdDb::kLnWt211);
  pLws->SetCurSel(m_lwIndex);

  CComboBox* pDef = (CComboBox*)GetDlgItem(IDC_COMBO_DEFAULT);
  pDef->ResetContent();
  for(int i=0; i<24; ++i)
  {
    pDef->AddString(strings[m_bListMM][i]);
  }
  pDef->SetCurSel(m_defaultIndex);
}

void CLwSettingsDlg::OnRadioMm() 
{
  if(!m_bListMM)
  {
    m_bListMM = TRUE;
    fillList();
  }
}

void CLwSettingsDlg::OnRadioIn() 
{
  if(m_bListMM)
  {
    m_bListMM = FALSE;
    fillList();
  }
}

void CLwSettingsDlg::OnSelchangeComboDefault() 
{
  CComboBox* pDef = (CComboBox*)GetDlgItem(IDC_COMBO_DEFAULT);
  m_defaultIndex = pDef->GetCurSel();
}

void CLwSettingsDlg::OnSelchangeLineweights() 
{
	CListBox* pLws = (CListBox*)GetDlgItem(IDC_LINEWEIGHTS);
  m_lwIndex = pLws->GetCurSel();
  CString text;
  pLws->GetText(pLws->GetCurSel(), text);
  GetDlgItem(IDC_STATIC_CURLW)->SetWindowText(CString(_T("Current Lineweight: ")) + text);
}
