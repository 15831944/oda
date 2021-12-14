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
// PageSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "PageSetupDlg.h"
#include "winspool.h"

#include "DgTable.h"
#include "DgView.h"
#include "DgModel.h"

// CPageSetupDlg dialog

//---------------------------------------------------------

OdUInt32 CPageSetupDlg::m_numPapers = 40;

CPageSetupDlg::Paper CPageSetupDlg::m_arrPapers[] = 
{
  {OD_T("Letter (8.50 x 11.00 Inches)")           , OD_T("Letter_(8.50_x_11.00_Inches)")           , 8.50     , 11.00     , false , 0.2500000,  0.2500000,  0.2500002,  0.2500002 } ,
  {OD_T("Legal (8.50 x 14.0 Inches)")             , OD_T("Legal_(8.50_x_14.0_Inches)")             , 8.50     , 14.0      , false , 0.2500000,  0.2500000,  0.2500002,  0.2500002 } ,
  {OD_T("ANSI A (8.50 x 11.00 Inches)")           , OD_T("ANSI_A_(8.50_x_11.00_Inches)")           , 8.50     , 11.00     , false , 0.2500000,  0.7500000,  0.2500002,  0.7499995 } ,
  {OD_T("ANSI A (11.00 x 8.50 Inches)")           , OD_T("ANSI_A_(11.00_x_8.50_Inches)")           , 11.00    , 8.50      , false , 0.2500000,  0.7500000,  0.2500002,  0.7500001 } ,
  {OD_T("ANSI B (11.00 x 17.00 Inches)")          , OD_T("ANSI_B_(11.00_x_17.00_Inches)")          , 11.00    , 17.00     , false , 0.2500000,  0.7500000,  0.2500002,  0.7499995 } ,
  {OD_T("ANSI B (17.00 x 11.00 Inches)")          , OD_T("ANSI_B_(17.00_x_11.00_Inches)")          , 17.00    , 11.00     , false , 0.2500000,  0.7500000,  0.2500002,  0.7499995 } ,
  {OD_T("ANSI C (17.00 x 22.00 Inches)")          , OD_T("ANSI_C_(17.00_x_22.00_Inches)")          , 17.00    , 22.00     , false , 0.2500000,  0.7500000,  0.2500002,  0.7499995 } ,
  {OD_T("ANSI C (22.00 x 17.00 Inches)")          , OD_T("ANSI_C_(22.00_x_17.00_Inches)")          , 22.00    , 17.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7499995 } ,
  {OD_T("ANSI D (22.00 x 34.00 Inches)")          , OD_T("ANSI_D_(22.00_x_34.00_Inches)")          , 22.00    , 34.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7499995 } ,
  {OD_T("ANSI D (34.00 x 22.00 Inches)")          , OD_T("ANSI_D_(34.00_x_22.00_Inches)")          , 34.00    , 22.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7499995 } ,
  {OD_T("ANSI E (34.00 x 44.00 Inches)")          , OD_T("ANSI_E_(34.00_x_44.00_Inches)")          , 34.00    , 44.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7500019 } ,
  {OD_T("ANSI E (44.00 x 34.00 Inches)")          , OD_T("ANSI_E_(44.00_x_34.00_Inches)")          , 44.00    , 34.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7499995 } ,
  {OD_T("ARCH C (18.00 x 24.00 Inches)")          , OD_T("ARCH_C_(18.00_x_24.00_Inches)")          , 18.00    , 24.00     , false , 0.2500000,  0.7500000,  0.2500002,  0.7499995 } ,
  {OD_T("ARCH C (24.00 x 18.00 Inches)")          , OD_T("ARCH_C_(24.00_x_18.00_Inches)")          , 24.00    , 18.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7499995 } ,
  {OD_T("ARCH D (24.00 x 36.00 Inches)")          , OD_T("ARCH_D_(24.00_x_36.00_Inches)")          , 24.00    , 36.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7499995 } ,
  {OD_T("ARCH D (36.00 x 24.00 Inches)")          , OD_T("ARCH_D_(36.00_x_24.00_Inches)")          , 36.00    , 24.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7499995 } ,
  {OD_T("ARCH E (36.00 x 48.00 Inches)")          , OD_T("ARCH_E_(36.00_x_48.00_Inches)")          , 36.00    , 48.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7500019 } ,
  {OD_T("ARCH E (48.00 x 36.00 Inches)")          , OD_T("ARCH_E_(48.00_x_36.00_Inches)")          , 48.00    , 36.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7499995 } ,
  {OD_T("ARCH E1 (30.00 x 42.00 Inches)")         , OD_T("ARCH_E1_(30.00_x_42.00_Inches)")         , 30.00    , 42.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7500019 } ,
  {OD_T("ARCH E1 (42.00 x 30.00 Inches)")         , OD_T("ARCH_E1_(42.00_x_30.00_Inches)")         , 42.00    , 30.00     , false , 0.2500000,  0.7500000,  0.2499990,  0.7499995 } ,
  {OD_T("ISO A4 (210.00 x 297.00 MM)")            , OD_T("ISO_A4_(210.00_x_297.00_MM)")            , 210.00   , 297.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO A4 (297.00 x 210.00 MM)")            , OD_T("ISO_A4_(297.00_x_210.00_MM)")            , 297.00   , 210.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO A3 (297.00 x 420.00 MM)")            , OD_T("ISO_A3_(297.00_x_420.00_MM)")            , 297.00   , 420.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO A3 (420.00 x 297.00 MM)")            , OD_T("ISO_A3_(420.00_x_297.00_MM)")            , 420.00   , 297.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO A2 (420.00 x 594.00 MM)")            , OD_T("ISO_A2_(420.00_x_594.00_MM)")            , 420.00   , 594.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO A2 (594.00 x 420.00 MM)")            , OD_T("ISO_A2_(594.00_x_420.00_MM)")            , 594.00   , 420.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO A1 (594.00 x 841.00 MM)")            , OD_T("ISO_A1_(594.00_x_841.00_MM)")            , 594.00   , 841.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO A1 (841.00 x 594.00 MM)")            , OD_T("ISO_A1_(841.00_x_594.00_MM)")            , 841.00   , 594.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO A0 (841.00 x 1189.00 MM)")           , OD_T("ISO_A0_(841.00_x_1189.00_MM)")           , 841.00   , 1189.00   , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO A0 (1189.00 x 841.00 MM)")           , OD_T("ISO_A0_(1189.00_x_841.00_MM)")           , 1189.00  , 841.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO B5 (182.00 x 237.00 MM)")            , OD_T("ISO_B5_(182.00_x_237.00_MM)")            , 182.00   , 237.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO B5 (237.00 x 182.00 MM)")            , OD_T("ISO_B5_(237.00_x_182.00_MM)")            , 237.00   , 182.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO B4 (250.00 x 354.00 MM)")            , OD_T("ISO_B4_(250.00_x_354.00_MM)")            , 250.00   , 354.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO B4 (354.00 x 250.00 MM)")            , OD_T("ISO_B4_(354.00_x_250.00_MM)")            , 354.00   , 250.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO B2 (500.00 x 707.00 MM)")            , OD_T("ISO_B2_(500.00_x_707.00_MM)")            , 500.00   , 707.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO B2 (707.00 x 500.00 MM)")            , OD_T("ISO_B2_(707.00_x_500.00_MM)")            , 707.00   , 500.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO B1 (707.00 x 1000.00 MM)")           , OD_T("ISO_B1_(707.00_x_1000.00_MM)")           , 707.00   , 1000.00   , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO B1 (1000.00 x 707.00 MM)")           , OD_T("ISO_B1_(1000.00_x_707.00_MM)")           , 1000.00  , 707.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("ISO C5 (229.00 x 162.00 MM)")            , OD_T("ISO_C5_(229.00_x_162.00_MM)")            , 229.00   , 162.00    , true  , 7.500000, 20.000000,  7.500000, 20.000000 } ,
  {OD_T("700mm (700.00 x 1000.00 MM)")            , OD_T("700mm_(700.00_x_1000.00_MM)")            , 700.00   , 1000.00   , true  , 7.500000, 20.000000,  7.500000, 20.000000 } 
};

IMPLEMENT_DYNAMIC(CPageSetupDlg, CDialog)

CPageSetupDlg::CPageSetupDlg(CWnd* pParent, 
                             const COdaDgnAppPrintSettings& printSettings,
                             OdDgElementId idModel )
	: CDialog(CPageSetupDlg::IDD, pParent)
{
  m_printSettings = printSettings;
  m_idModel       = idModel;
  m_bViewsPresent = true;

  m_iNameIndex  = 0;
  m_iPaperSize  = 0;
  m_iRenderMode = 0;
  m_iPlotArea   = 0;
  m_iRotation   = 0;
  m_strPlotterLocal.Empty();
  m_strPlotterWhere.Empty();
  m_strPlotterDescription.Empty();
  m_dPageWidth  = 297;
  m_dPageHeight = 210;
  m_dLeftMargin = 7.5;
  m_dRightMargin = 7.5;
  m_dTopMargin  = 7.5;
  m_dBottomMargin = 7.5;
  m_dOffsetX = 0;
  m_dOffsetY = 0;
  m_dScale   = 1.0;
}

//---------------------------------------------------------

CPageSetupDlg::~CPageSetupDlg()
{
}

//---------------------------------------------------------

COdaDgnAppPrintSettings CPageSetupDlg::getPrinterSettings() const
{
  return m_printSettings;
}

//---------------------------------------------------------

void CPageSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Text   (pDX, IDC_PLOTTER_STATIC, m_strPlotterLocal);
  DDX_Text   (pDX, IDC_WHERE_STATIC, m_strPlotterWhere);
  DDX_Text   (pDX, IDC_DESCRIPTION_STATIC, m_strPlotterDescription);
  DDX_Text   (pDX, IDC_PAPER_W_EDIT, m_dPageWidth);
  DDX_Text   (pDX, IDC_PAPER_H_EDIT, m_dPageHeight);
  DDX_Text   (pDX, IDC_MARGIN_L_EDIT, m_dLeftMargin);
  DDX_Text   (pDX, IDC_MARGIN_R_EDIT, m_dRightMargin);
  DDX_Text   (pDX, IDC_MARGIN_T_EDIT, m_dTopMargin);
  DDX_Text   (pDX, IDC_MARGIN_B_EDIT, m_dBottomMargin);
  DDX_Text   (pDX, IDC_OFFSET_X_EDIT, m_dOffsetX);
  DDX_Text   (pDX, IDC_OFFSET_Y_EDIT, m_dOffsetY);
  DDX_Text   (pDX, IDC_SCALE_EDIT, m_dScale);
  DDX_CBIndex( pDX,IDC_PRINTER_NAME_COMBO,m_iNameIndex );
  DDX_CBIndex( pDX,IDC_PAPER_COMBO,m_iPaperSize );
  DDX_CBIndex( pDX,IDC_RENDER_COMBO,m_iRenderMode );
  DDX_CBIndex( pDX,IDC_PLOT_MODE_COMBO,m_iPlotArea );
  DDX_CBIndex( pDX,IDC_ROTATION_COMBO,m_iRotation );
}

//---------------------------------------------------------

BEGIN_MESSAGE_MAP(CPageSetupDlg, CDialog)
  ON_CBN_SELCHANGE(IDC_PRINTER_NAME_COMBO, OnSelchangePrinterName)
  ON_CBN_SELCHANGE(IDC_PAPER_COMBO, OnSelchangePaperSize)
  ON_CBN_SELCHANGE(IDC_PLOT_MODE_COMBO, OnSelchangePlotType)
  ON_BN_CLICKED(IDOK, &CPageSetupDlg::OnBnClickedOk)
END_MESSAGE_MAP()

//---------------------------------------------------------

void CPageSetupDlg::OnSelchangePrinterName()
{
  UpdateData(TRUE);
  updatePrinterSettings();
  updateCtrls();
}

//---------------------------------------------------------

void CPageSetupDlg::OnSelchangePlotType()
{
  UpdateData(TRUE);

  if( m_iPlotArea == 2 ) // Sheet
  {
    m_iPlotArea = 0;

    if( !m_idModel.isNull() )
    {
      OdDgModelPtr pModel = m_idModel.openObject(OdDg::kForRead);

      if( !pModel.isNull() && pModel->getType() == OdDgModel::kSheetModel )
      {
        OdDgSheetModelPtr pSheetModel = pModel;

        if( !pSheetModel.isNull() )
        {
          m_iPlotArea     = 2;

          OdDgModel::UnitDescription masterUnits;
          pSheetModel->getMasterUnit( masterUnits );

          double dScaleFactor = masterUnits.m_denominator / masterUnits.m_numerator * 1000;

          m_dPageWidth    = pSheetModel->getSheetWidth()*dScaleFactor;
          m_dPageHeight   = pSheetModel->getSheetHeight()*dScaleFactor;
          m_dLeftMargin   = pSheetModel->getSheetLeftMargin()*dScaleFactor;
          m_dRightMargin  = pSheetModel->getSheetRightMargin()*dScaleFactor;
          m_dTopMargin    = pSheetModel->getSheetTopMargin()*dScaleFactor;
          m_dBottomMargin = pSheetModel->getSheetBottomMargin()*dScaleFactor;

          m_iPaperSize = 0;

          for( OdUInt32 i = 0; i < m_numPapers; i++  )
          {
            double dPaperWidth  = m_arrPapers[i]._w;
            double dPaperHeight = m_arrPapers[i]._h;

            if( !m_arrPapers[i]._inMM )
            {
              dPaperWidth  *= 25.4;
              dPaperHeight *= 25.4;
            }

            if( OdZero(dPaperWidth - m_dPageWidth) &&
              OdZero(dPaperHeight - m_dPageHeight)
              )
            {
              m_iPaperSize = i+1;
              break;
            }
          }
        }
      }
    }

    UpdateData(FALSE);
  }

  updatePrinterSettings();
  updateCtrls();
}

//---------------------------------------------------------

void CPageSetupDlg::OnSelchangePaperSize()
{
  UpdateData(TRUE);

  if( m_iPaperSize > 0 )
  {
    m_dPageWidth    = m_arrPapers[m_iPaperSize - 1]._w;
    m_dPageHeight   = m_arrPapers[m_iPaperSize - 1]._h;
    m_dLeftMargin   = m_arrPapers[m_iPaperSize - 1]._xminMargin;
    m_dRightMargin  = m_arrPapers[m_iPaperSize - 1]._xmaxMargin;
    m_dTopMargin    = m_arrPapers[m_iPaperSize - 1]._yminMargin;
    m_dBottomMargin = m_arrPapers[m_iPaperSize - 1]._ymaxMargin;

    if( !m_arrPapers[m_iPaperSize - 1]._inMM )
    {
      m_dPageWidth    *= 25.4;
      m_dPageHeight   *= 25.4;
      m_dLeftMargin   *= 25.4;
      m_dRightMargin  *= 25.4;
      m_dTopMargin    *= 25.4;
      m_dBottomMargin *= 25.4;
    }
  }

  updatePrinterSettings();
  updateCtrls();
}

//---------------------------------------------------------

BOOL CPageSetupDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  initCtrls();
  initDataByPrinterSettings();
  updateCtrls();

  return TRUE;
}

//---------------------------------------------------------

void CPageSetupDlg::OnOK()
{
  UpdateData( TRUE );
  updatePrinterSettings();

  CDialog::OnOK();
}

//---------------------------------------------------------

void CPageSetupDlg::OnCancel()
{
  CDialog::OnCancel();
}

//---------------------------------------------------------

void CPageSetupDlg::updateCtrls()
{
  if( m_iNameIndex > 0 )
  {
    m_strPlotterDescription = m_strPrinterDescriptionArr[m_iNameIndex - 1];
    m_strPlotterLocal       = m_strPrinterLogNameArr[m_iNameIndex  - 1];
    m_strPlotterWhere       = m_strPrinterWhereArr[m_iNameIndex  - 1];

    if( m_strPlotterDescription.IsEmpty() )
    {
      m_strPlotterDescription = L"...";
    }

    if( m_strPlotterLocal.IsEmpty() )
    {
      m_strPlotterLocal = L"...";
    }

    if( m_strPlotterWhere.IsEmpty() )
    {
      m_strPlotterWhere = L"...";
    }
  }
  else
  {
    m_strPlotterDescription = L"...";
    m_strPlotterLocal       = L"...";
    m_strPlotterWhere       = L"...";
  }

  CEdit* pWidthEdit  = (CEdit*)(GetDlgItem(IDC_PAPER_W_EDIT));
  CEdit* pHeightEdit = (CEdit*)(GetDlgItem(IDC_PAPER_H_EDIT));

  pWidthEdit->EnableWindow( m_iPaperSize == 0 );
  pHeightEdit->EnableWindow( m_iPaperSize == 0 );

  CEdit* pScaleEdit = (CEdit*)(GetDlgItem(IDC_SCALE_EDIT));

  pScaleEdit->EnableWindow( m_iPlotArea == 0 );

  UpdateData(FALSE);
}

//---------------------------------------------------------

void CPageSetupDlg::updatePrinterSettings()
{
  if( m_iNameIndex <= 0 )
  {
    m_printSettings.m_strPrinterDriver.empty();
  }
  else
  {
    m_printSettings.m_strPrinterDriver = m_strPrinterNameArr[ m_iNameIndex - 1];
  }

  m_printSettings.m_dPaperWidth   = m_dPageWidth;
  m_printSettings.m_dPaperHeight  = m_dPageHeight;
  m_printSettings.m_dLeftMargin   = m_dLeftMargin;
  m_printSettings.m_dRightMargin  = m_dRightMargin;
  m_printSettings.m_dTopMargin    = m_dTopMargin;
  m_printSettings.m_dBottomMargin = m_dBottomMargin;
  m_printSettings.m_dPlotOffsetX  = m_dOffsetX;
  m_printSettings.m_dPlotOffsetY  = m_dOffsetY;
  m_printSettings.m_dScale        = m_dScale;
  m_printSettings.m_uPlotRotation = (COdaDgnAppPrintSettings::OdDgPlotRotation)( m_iRotation );
  m_printSettings.m_uPlotType     = (COdaDgnAppPrintSettings::OdDgPlotType)( m_iPlotArea );
  m_printSettings.m_uRenderMode   = (OdGsView::RenderMode)( m_iRenderMode );

//   if( m_iRenderMode > 0 )
//   {
//     m_printSettings.m_bPlotToBMP = true;
//   }
//   else
//   {
//     m_printSettings.m_bPlotToBMP = false;
//   }

  m_printSettings.m_bPlotToBMP = true;
}

//---------------------------------------------------------

void CPageSetupDlg::initDataByPrinterSettings()
{
  OdUInt32 i;

  m_iNameIndex = 0;

  CString strPrinter(m_printSettings.m_strPrinterDriver.c_str());

  for( i = 0; i < m_strPrinterNameArr.size(); i++ )
  {
    if( m_strPrinterNameArr[i] == strPrinter )
    {
      m_iNameIndex = i + 1;
      break;
    }
  }

  m_iPaperSize = 0;

  for( i = 0; i < m_numPapers; i++  )
  {
    double dPaperWidth  = m_arrPapers[i]._w;
    double dPaperHeight = m_arrPapers[i]._h;

    if( !m_arrPapers[i]._inMM )
    {
      dPaperWidth  *= 25.4;
      dPaperHeight *= 25.4;
    }

    if( OdZero(dPaperWidth - m_printSettings.m_dPaperWidth) &&
        OdZero(dPaperHeight - m_printSettings.m_dPaperHeight)
      )
    {
      m_iPaperSize = i+1;
      break;
    }
  }

  m_dPageWidth  = m_printSettings.m_dPaperWidth;
  m_dPageHeight = m_printSettings.m_dPaperHeight;

  m_dLeftMargin = m_printSettings.m_dLeftMargin;
  m_dRightMargin = m_printSettings.m_dRightMargin;
  m_dTopMargin = m_printSettings.m_dTopMargin;
  m_dBottomMargin = m_printSettings.m_dBottomMargin;

  m_iRenderMode = (int)(m_printSettings.m_uRenderMode);

  m_iPlotArea = m_printSettings.m_uPlotType;

  CComboBox* pTypeCombo = (CComboBox*)GetDlgItem(IDC_PLOT_MODE_COMBO);

  if( pTypeCombo->GetCount() < m_printSettings.m_uPlotType )
  {
    m_iPlotArea = m_printSettings.m_uPlotType;
  }

  m_iRotation = (int)(m_printSettings.m_uPlotRotation);

  m_dOffsetX = m_printSettings.m_dPlotOffsetX;
  m_dOffsetY = m_printSettings.m_dPlotOffsetY;
  m_dScale   = m_printSettings.m_dScale;

  UpdateData(FALSE);

  updatePrinterSettings();
}

//---------------------------------------------------------

void CPageSetupDlg::initCtrls()
{
  // Get printer info for available printers.

  m_strPrinterNameArr.clear();
  m_strPrinterLogNameArr.clear();
  m_strPrinterWhereArr.clear();
  m_strPrinterDescriptionArr.clear();

  DWORD Flags = PRINTER_ENUM_FAVORITE | PRINTER_ENUM_LOCAL; //local printers

  DWORD cbBuf;
  DWORD pcReturned;
  DWORD Level = 2;
  TCHAR Name[500];
  LPPRINTER_INFO_2 pPrinterEnum2 = NULL;

  memset( Name, 0, sizeof( TCHAR) * 500);

  ::EnumPrinters( Flags, NULL, Level, NULL, 0, &cbBuf, &pcReturned);

  pPrinterEnum2 = ( LPPRINTER_INFO_2) LocalAlloc( LPTR, cbBuf + 4);

  if( pPrinterEnum2 )
  {
    
    if( EnumPrinters(   Flags,                   // DWORD Flags, printer object types 
                        Name,                    // LPTSTR Name, name of printer object 
                        Level,                   // DWORD Level, information level 
                        (LPBYTE)pPrinterEnum2,   // LPBYTE pPrinterEnum, printer information buffer 
                        cbBuf,                   // DWORD cbBuf, size of printer information buffer
                        &cbBuf,                  // LPDWORD pcbNeeded, bytes received or required 
                        &pcReturned))            // LPDWORD pcReturned number of printers enumerated 
    {
      if( pcReturned > 0) 
      {
        for( DWORD index = 0; index < pcReturned; index++) 
        {
          if( pPrinterEnum2 )
          {
            CString strShareName   = ( pPrinterEnum2 + index)->pShareName;
            CString strPrinterName = ( pPrinterEnum2 + index)->pPrinterName;
            CString strWhere       = ( pPrinterEnum2 + index)->pLocation;
            CString strDescription = ( pPrinterEnum2 + index)->pComment;

            m_strPrinterNameArr.push_back( strPrinterName );
            m_strPrinterLogNameArr.push_back( strShareName );
            m_strPrinterWhereArr.push_back( strWhere );
            m_strPrinterDescriptionArr.push_back( strDescription );
          }
        }
      }
    }
  }

  if( pPrinterEnum2 )
  {
    LocalFree( LocalHandle( pPrinterEnum2));
  }

  // Init printer combo box.

  OdUInt32 i;

  CComboBox* pPrinterCombo = (CComboBox*)GetDlgItem(IDC_PRINTER_NAME_COMBO);

  pPrinterCombo->Clear();

  pPrinterCombo->AddString(L"<none>");

  for( i = 0; i < m_strPrinterNameArr.size(); i++ )
  {
    pPrinterCombo->AddString( m_strPrinterNameArr[i] );
  }

  // Init paper size combo box.

  CComboBox* pPaperCombo = (CComboBox*)GetDlgItem(IDC_PAPER_COMBO);

  pPaperCombo->Clear();

  pPaperCombo->AddString(L"Custom...");

  for( i = 0; i < m_numPapers; i++ )
  {
    pPaperCombo->AddString( m_arrPapers[i]._localeName );
  }

  // Initialize render mode combo

  CComboBox* pRenderCombo = (CComboBox*)GetDlgItem(IDC_RENDER_COMBO);
  pRenderCombo->Clear();

  pRenderCombo->AddString(L"2D Optimized");
  pRenderCombo->AddString(L"Wireframe");
  pRenderCombo->AddString(L"Hidden Line");
  pRenderCombo->AddString(L"Flat Shaded");
  pRenderCombo->AddString(L"Smooth Shaded");
  pRenderCombo->AddString(L"Flat Shaded With Wireframe");
  pRenderCombo->AddString(L"Smooth Shaded With Wireframe");

  // Initialize plot type combo

  CComboBox* pTypeCombo = (CComboBox*)GetDlgItem(IDC_PLOT_MODE_COMBO);
  pTypeCombo->Clear();

  pTypeCombo->AddString(L"View");
  pTypeCombo->AddString(L"Extents");

  if( !m_idModel.isNull() )
  {
    OdDgModelPtr pModel = m_idModel.openObject(OdDg::kForRead);

    if( !pModel.isNull() && pModel->getType() == OdDgModel::kSheetModel )
    {
      pTypeCombo->AddString(L"Sheet");
    }
  }

  // Initialize rotation combo

  CComboBox* pRotationCombo = (CComboBox*)GetDlgItem(IDC_ROTATION_COMBO);
  pRotationCombo->Clear();

  pRotationCombo->AddString(L"<none>");
  pRotationCombo->AddString(L"90");
  pRotationCombo->AddString(L"180");
  pRotationCombo->AddString(L"270");
}

//---------------------------------------------------------
void CPageSetupDlg::OnBnClickedOk()
{
  // TODO: Add your control notification handler code here
  OnOK();
}
