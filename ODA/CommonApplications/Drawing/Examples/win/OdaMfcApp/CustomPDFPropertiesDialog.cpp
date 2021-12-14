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

using namespace TD_PDF_2D_EXPORT;

CCustomPDFPropertiesDialog::CCustomPDFPropertiesDialog(AuxPdfProps& props, CWnd* pParent /*=NULL*/)
  : CDialog(CCustomPDFPropertiesDialog::IDD, pParent)
  , m_piHatchDPI(props.piHatchDPI)
  , m_piGeomDPI(props.piGeomDPI)
  , m_piColorImagesDPI(props.piColorImagesDPI)
  , m_piBWImagesDPI(props.piBWImagesDPI)
  , m_piDCTQuality(props.piDCTQuality)
  , m_piPdfAMode(props.piPdfAMode)
  , m_piShadedVpMode(props.piShadedVpMode)
  , m_bMergeLinesCrossing(props.pbMergeLines)
  , m_bCropImages(props.pbCropImages)
  , m_bDCTCompression(props.pbDCTCompression)
  , m_bMeasuring(props.pbMeasuring)
  , m_bForceDisableGsDevice(props.pbForceDisableGsDevice)
  , m_bTranspBgForShadedViewports(props.pbTranspBgForShadedViewports)
  , m_bSearchTextOnShadedVp(props.pbSearchTextOnShadedVp)
  , m_bUpscaleImages(props.pbUpscaleImages)
  , m_bUseGsCache(props.pbUseGsCache)
  , m_bUsePdfBlocks(props.pbUsePdfBlocks)
  , m_Watermark(props.watermark)
  , m_bFitWm2Page(FALSE)
{
}

CCustomPDFPropertiesDialog::~CCustomPDFPropertiesDialog()
{
}

void CCustomPDFPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Text(pDX,    IDC_EDIT_HATCH_DPI, *m_piHatchDPI);
  DDX_Text(pDX,    IDC_EDIT_GEOM_DPI, *m_piGeomDPI);
  DDX_Text(pDX,    IDC_EDIT_COLOR_IMAGES_DPI, *m_piColorImagesDPI);
  DDX_Text(pDX,    IDC_EDIT_BW_IMAGES_DPI, *m_piBWImagesDPI);
  DDX_Text(pDX,    IDC_WATERMARK, m_wmText);
  DDX_Text(pDX,    IDC_EDIT_WM_FONTSIZE, m_iWMFomtSize);
  DDX_Text(pDX,    IDC_EDIT_WM_OPACITY, m_iWMOpacity);

  DDX_Check(pDX,   IDC_CROP, *m_bCropImages);
  DDX_Check(pDX,   IDC_DCT, *m_bDCTCompression);
  DDX_Check(pDX,   IDC_MEASURING, *m_bMeasuring);
  DDX_Check(pDX,   IDC_DISABLE_GS, *m_bForceDisableGsDevice);
  DDX_Check(pDX,   IDC_TRANSPARENT_BG, *m_bTranspBgForShadedViewports);
  DDX_Check(pDX,   IDC_SEARCHABLE_TXT_ON_SHADED_VP, *m_bSearchTextOnShadedVp);
  DDX_Check(pDX,   IDC_CHECK_FIT_WM_2_PAGE, m_bFitWm2Page);
  DDX_Check(pDX,   IDC_UPSCALE_IMAGES, *m_bUpscaleImages);
  DDX_Check(pDX,   IDC_USE_GS_CACHE, *m_bUseGsCache);
  DDX_Check(pDX,   IDC_USE_PDF_BLOCKS, *m_bUsePdfBlocks);

  DDX_Control(pDX, IDC_COMBO_MERGE_CONTROL, m_comboMergeControl);
  DDX_Control(pDX, IDC_COMBO_SHADED_VP_HANDLE, m_comboShadedVpMode);
  DDX_Control(pDX, IDC_COMBO_PDF_A, m_comboPDFAmode);
  DDX_Control(pDX, IDC_COMBO_WM_POSITION, m_comboWMPosition);
  DDX_Control(pDX, IDC_COMBO_WM_FONT, m_comboWMFont);
  DDX_Control(pDX, IDC_COMBO_WM_FONT_COLOR, m_comboWMColor);
}


BEGIN_MESSAGE_MAP(CCustomPDFPropertiesDialog, CDialog)
  ON_BN_CLICKED(IDOK, OnOk)
  ON_BN_CLICKED(IDC_DCT, OnCheckDCTCompression)
  ON_BN_CLICKED(IDC_USE_GS_CACHE, OnCheckGsCache)
END_MESSAGE_MAP()

void CCustomPDFPropertiesDialog::OnCheckDCTCompression()
{
  UpdateData(TRUE);
  GetDlgItem(IDC_SLIDER_QUALITY)->EnableWindow(*m_bDCTCompression);
  GetDlgItem(IDC_STATIC_IQ)->EnableWindow(*m_bDCTCompression);
  UpdateData(FALSE); // save and validate
}

void CCustomPDFPropertiesDialog::OnCheckGsCache()
{
  UpdateData(TRUE);
  //TODO: unhide it when pdf blocks are implemented
//   if(!*m_bUseGsCache)
//     *m_bUsePdfBlocks = FALSE;
//   GetDlgItem(IDC_USE_PDF_BLOCKS)->EnableWindow(*m_bUseGsCache);
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

  m_comboWMColor.ResetContent();
  m_comboWMColor.InsertString(0, _T("Red"));
  m_comboWMColor.InsertString(1, _T("Green"));
  m_comboWMColor.InsertString(2, _T("Blue"));
  m_comboWMColor.SetCurSel(0);

  m_comboWMPosition.ResetContent();
  m_comboWMPosition.InsertString(0, _T("Left to right in page's center"));
  m_comboWMPosition.InsertString(1, _T("Upper left corner to lower right corner"));
  m_comboWMPosition.InsertString(2, _T("Lower left corner to upper right corner"));
  m_comboWMPosition.InsertString(3, _T("Upper left corner"));
  m_comboWMPosition.InsertString(4, _T("Upper right corner"));
  m_comboWMPosition.InsertString(5, _T("Lower right corner"));
  m_comboWMPosition.InsertString(6, _T("Lower left corner"));
  m_comboWMPosition.SetCurSel(0);

  m_comboWMFont.ResetContent();
  m_comboWMFont.InsertString(0, _T("Times Roman"));
  m_comboWMFont.InsertString(1, _T("Helvetica"));
  m_comboWMFont.InsertString(2, _T("Courier"));
  m_comboWMFont.InsertString(3, _T("Symbol"));
  m_comboWMFont.InsertString(4, _T("Times Bold"));
  m_comboWMFont.InsertString(5, _T("Helvetica Bold"));
  m_comboWMFont.InsertString(6, _T("Courier Bold"));
  m_comboWMFont.InsertString(7, _T("ZapfDingbats"));
  m_comboWMFont.InsertString(8, _T("Times Italic"));
  m_comboWMFont.InsertString(9, _T("Helvetica Oblique"));
  m_comboWMFont.InsertString(10, _T("Courier Oblique"));
  m_comboWMFont.InsertString(11, _T("Times Bold Italic"));
  m_comboWMFont.InsertString(12, _T("Helvetica Bold Oblique"));
  m_comboWMFont.InsertString(13, _T("Courier Bold Oblique"));
  m_comboWMFont.SetCurSel(0);

  m_comboShadedVpMode.ResetContent();
  m_comboShadedVpMode.InsertString(0, _T("One-piece picture"));
  m_comboShadedVpMode.InsertString(1, _T("Tiled picture (hi-res)"));
  m_comboShadedVpMode.InsertString(2, _T("Striped picture (hi-res)"));
  m_comboShadedVpMode.SetCurSel(*m_piShadedVpMode);

  m_iWMFomtSize = 48;
  m_iWMOpacity = 50;

  GetDlgItem(IDC_CHECK_FIT_WM_2_PAGE)->EnableWindow(TRUE);
  GetDlgItem(IDC_CHECK_FIT_WM_2_PAGE)->ShowWindow(SW_SHOW);

  GetDlgItem(IDC_USE_PDF_BLOCKS)->ShowWindow(SW_HIDE); //TODO: unhide it when pdf blocks are implemented
  //GetDlgItem(IDC_USE_PDF_BLOCKS)->EnableWindow(*m_bUseGsCache);

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
  case 0:
    *m_bMergeLinesCrossing = FALSE;
    break;  
  case 1: //Merge Lines
    *m_bMergeLinesCrossing = TRUE;
    break;
  }

  *m_piPdfAMode = m_comboPDFAmode.GetCurSel();
  *m_piShadedVpMode = m_comboShadedVpMode.GetCurSel();

  if (m_iWMOpacity > 100)
    m_iWMOpacity = 100;
  else if (m_iWMOpacity < 0)
    m_iWMOpacity = 0;
  m_Watermark->opacity = m_iWMOpacity;

  if (m_iWMFomtSize > 72)
    m_iWMFomtSize = 72;
  else if (m_iWMFomtSize < 0)
    m_iWMFomtSize = 0;
  m_Watermark->fontSize = m_iWMFomtSize;

  if (m_wmText.GetLength() > 15)
    m_wmText.Truncate(15);

  m_Watermark->text = m_wmText;
  switch (m_comboWMColor.GetCurSel())
  {
  case 0:
    m_Watermark->color = ODRGB(255, 0, 0);
    break;
  case 1:
    m_Watermark->color = ODRGB(0, 255, 0);
    break;
  case 2:
    m_Watermark->color = ODRGB(0, 0, 255);
    break;
  default:
    break;
  }
  m_Watermark->font = (Watermark::WatermarkFonts)m_comboWMFont.GetCurSel();
  m_Watermark->position = (Watermark::WatermarkPosition)m_comboWMPosition.GetCurSel();
  m_Watermark->scaleToPage = m_bFitWm2Page;

  if (bWrongData)
  {
    ::MessageBox(NULL, _T("Raster resolution cannot exceed the current vector resolution settings."), _T("Resolution reset"), MB_OK);
    UpdateData(FALSE);
  }
  else
    CDialog::OnOK();
  
}
