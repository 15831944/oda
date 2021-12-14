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

// CustomMediaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CustomMediaDlg.h"

CCustomMediaDlg::CCustomMediaDlg(OdDbPlotSettingsValidatorPE::psvPaperInfo* paperInfo, CWnd* pParent /*=NULL*/)
	: CDialog(CCustomMediaDlg::IDD, NULL)
{
  m_dMarginsL = 0.;
  m_dMarginsR = 0.;
  m_dMarginsT = 0.;
  m_dMarginsB = 0.;
  m_dHeight   = 0.;
  m_dWidth    = 0.;
  m_Name      = "";
  m_paperInfo = paperInfo;
}

CCustomMediaDlg::~CCustomMediaDlg()
{
}

void CCustomMediaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMBO_MUNITS, m_comboMUnits);
  DDX_Text(pDX, IDC_EDIT_LMARGIN, m_dMarginsL);
  DDX_Text(pDX, IDC_EDIT_RMARGIN, m_dMarginsR);
  DDX_Text(pDX, IDC_EDIT_TMARGIN, m_dMarginsT);
  DDX_Text(pDX, IDC_EDIT_BMARGIN, m_dMarginsB);
  DDX_Text(pDX, IDC_EDIT_NAME,    m_Name);
  DDX_Text(pDX, IDC_EDIT_HEIGHT,  m_dHeight);
  DDX_Text(pDX, IDC_EDIT_WIDTH,   m_dWidth);
}


BEGIN_MESSAGE_MAP(CCustomMediaDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_COMBO_MUNITS, OnSelChangeMUnits)
END_MESSAGE_MAP()

void CCustomMediaDlg::OnSelChangeMUnits()
{
  UpdateData();
}

BOOL CCustomMediaDlg::OnInitDialog()
{
  if (!CDialog::OnInitDialog())
    return FALSE;

  if (!m_paperInfo)
    return FALSE;

  m_comboMUnits.ResetContent();

  m_comboMUnits.InsertString(0, L"Inches");      //OdDbPlotSettings::PlotPaperUnits::kInches
  m_comboMUnits.InsertString(1, L"Millimeters"); //OdDbPlotSettings::PlotPaperUnits::kMillimeters
  m_comboMUnits.InsertString(2, L"Pixels");      //OdDbPlotSettings::PlotPaperUnits::kPixels
  m_comboMUnits.SetCurSel(1);

  UpdateData(FALSE);
  return TRUE;
}

void CCustomMediaDlg::OnOK()
{
  UpdateData();                   // TRUE - data is being retrieved

  m_paperInfo->w = m_dWidth;
  m_paperInfo->h = m_dHeight;
  m_paperInfo->bottom = m_dMarginsB;
  m_paperInfo->top = m_dMarginsT;
  m_paperInfo->left = m_dMarginsL;
  m_paperInfo->right = m_dMarginsR;

  m_Name.Trim();
  while (m_Name.Replace(L"  ", L" "));
  m_paperInfo->localeName = m_Name;
  m_Name.Replace(L" ", L"_");
  m_paperInfo->canonicalName = m_Name;

  m_paperInfo->units = (OdDbPlotSettings::PlotPaperUnits)m_comboMUnits.GetCurSel();

  CDialog::OnOK();
}

void CCustomMediaDlg::OnCancel()
{
  *m_paperInfo = OdDbPlotSettingsValidatorPE::psvPaperInfo();
  CDialog::OnCancel();
}


// CCustomMediaDlg message handlers
