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

// CustomPDFPropertiesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CustomPDFPropertiesDialog.h"

CCustomPDFPropertiesDialog::CCustomPDFPropertiesDialog(unsigned int* piHatchDPI, unsigned int* piGeomDPI, unsigned int* piColorImagesDPI, unsigned int* piBWImagesDPI, 
                                                        unsigned int* piDCTQuality, unsigned int* piPdfAMode, BOOL* pbMergeLines,
                                                        BOOL* pbCropImages, BOOL* pbDCTCompression, BOOL* pbMeasuring, BOOL* pbUseGsCache, CWnd* pParent /*=NULL*/)
  : CDialog(CCustomPDFPropertiesDialog::IDD, pParent)
  , m_piHatchDPI(piHatchDPI)
  , m_piGeomDPI(piGeomDPI)
  , m_piColorImagesDPI(piColorImagesDPI)
  , m_piBWImagesDPI(piBWImagesDPI)
  , m_piDCTQuality(piDCTQuality)
  , m_piPdfAMode(piPdfAMode)
  , m_bMergeLinesCrossing(pbMergeLines)
  , m_bCropImages(pbCropImages)
  , m_bDCTCompression(pbDCTCompression)
  , m_bMeasuring(pbMeasuring)
  , m_bUseGsCache(pbUseGsCache)
{
}

CCustomPDFPropertiesDialog::~CCustomPDFPropertiesDialog()
{
}

void CCustomPDFPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text(pDX,  IDC_EDIT_HATCH_DPI, *m_piHatchDPI);
  DDX_Text(pDX,  IDC_EDIT_GEOM_DPI, *m_piGeomDPI);
  DDX_Text(pDX,  IDC_EDIT_COLOR_IMAGES_DPI, *m_piColorImagesDPI);
  DDX_Text(pDX,  IDC_EDIT_BW_IMAGES_DPI, *m_piBWImagesDPI);
  DDX_Check(pDX, IDC_CROP, *m_bCropImages);
  DDX_Check(pDX, IDC_DCT, *m_bDCTCompression);
  DDX_Check(pDX, IDC_MEASURING, *m_bMeasuring);
  DDX_Check(pDX, IDC_USE_GS_CACHE, *m_bUseGsCache);

  DDX_Control(pDX, IDC_COMBO_MERGE_CONTROL, m_comboMergeControl);
  DDX_Control(pDX, IDC_COMBO_PDF_A, m_comboPDFAmode);
}


BEGIN_MESSAGE_MAP(CCustomPDFPropertiesDialog, CDialog)
  ON_BN_CLICKED(IDOK, OnOk)
  ON_BN_CLICKED(IDC_DCT, OnCheckDCTCompression)
END_MESSAGE_MAP()

void CCustomPDFPropertiesDialog::OnCheckDCTCompression()
{
  UpdateData(TRUE);
  GetDlgItem(IDC_SLIDER_QUALITY)->EnableWindow(*m_bDCTCompression);
  GetDlgItem(IDC_STATIC_IQ)->EnableWindow(*m_bDCTCompression);
  UpdateData(FALSE); // save and validate
}

// CCustomPDFPropertiesDialog message handlers
BOOL CCustomPDFPropertiesDialog::OnInitDialog()
{
  if (!CDialog::OnInitDialog())
    return FALSE;

  CSliderCtrl* pSlider = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_QUALITY));
  pSlider->SetRange(10, 100);
  pSlider->SetPos(*m_piDCTQuality);
  pSlider->SetTicFreq(1);

  m_comboMergeControl.ResetContent();
  m_comboMergeControl.InsertString(0, _T("Lines Overwrite"));
  m_comboMergeControl.InsertString(1, _T("Lines Merge"));
  m_comboMergeControl.SetCurSel(*m_bMergeLinesCrossing);

  m_comboPDFAmode.ResetContent();
  m_comboPDFAmode.InsertString(0, _T("None"));
  m_comboPDFAmode.InsertString(1, _T("PDF/A-1b"));
  m_comboPDFAmode.InsertString(2, _T("PDF/A-2b (Recommended for archiving)"));
  m_comboPDFAmode.SetCurSel(*m_piPdfAMode <= 2 ? *m_piPdfAMode : 0);

  UpdateData(FALSE);
  return TRUE;
}

void CCustomPDFPropertiesDialog::OnOk()
{
  UpdateData(TRUE);
  bool bWrongData = false;

  if (*m_piGeomDPI < 72)
    *m_piGeomDPI = 72;
  if (*m_piGeomDPI > 40000)
    *m_piGeomDPI = 40000;

  if (*m_piHatchDPI < 72)
    *m_piHatchDPI = 72;
  if (*m_piHatchDPI > *m_piGeomDPI)
    *m_piHatchDPI = *m_piGeomDPI;

  if (*m_piColorImagesDPI < 72)
    *m_piColorImagesDPI = 72;
  if (*m_piColorImagesDPI > *m_piGeomDPI)
  {
    bWrongData = true;
    *m_piColorImagesDPI = *m_piGeomDPI;
  }

  if (*m_piBWImagesDPI < 72)
    *m_piBWImagesDPI = 72;
  if (*m_piBWImagesDPI > *m_piGeomDPI)
  {
    bWrongData = true;
    *m_piBWImagesDPI = *m_piGeomDPI;
  }

  *m_piDCTQuality = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_QUALITY))->GetPos();

  switch (m_comboMergeControl.GetCurSel())
  {
    case 1: //Merge Lines
    {
      *m_bMergeLinesCrossing = TRUE;
    }
    break;
  }

  *m_piPdfAMode = m_comboPDFAmode.GetCurSel();

  if (bWrongData)
  {
    ::MessageBox(NULL, _T("Raster resolution cannot exceed the current vector resolution settings."), _T("Resolution reset"), MB_OK);
    UpdateData(FALSE);
  }
  else
    CDialog::OnOK();
  
}
