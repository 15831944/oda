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

//
// PageSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "OdaCommon.h"
#include "Gs/Gs.h"

#include "PageSetupDlg.h"
#include "winspool.h"
#include "DbPlotSettingsValidator.h"
#include "DbDatabase.h"
#include "DbHostAppServices.h"
#include "OdaMfcApp.h"
#include "PsTbEditDlg.h"
#include "DbSymbolTable.h"
#include "DbViewTable.h"
#include "DbViewTableRecord.h"
#include "CustomMediaDlg.h"
#include "../Extensions/PlotSettingsValidator/PlotSettingsValidatorCustomMediaPE.h"
#include "RemoteGeomViewer.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NONE_DEVICE _T("None")

struct PlotScaleSetting
{
  OdChar *m_Str;
  double  m_dRealWorldUnits;  // numerator    142(dxf)
  double  m_dDrawingUnits;    // denominator  143(dxf)
  int     m_ScaleType;        //              75 (dxf)
  double  m_dScaleFactor;     //              147(dxf)
};

static PlotScaleSetting plotScaleSetting[] = {
  {_T("Custom"),      1,     1,    0,  1},                   //    _T("Scaled to Fit"),  //    kScaleToFit
  {_T("1/128\" = 1'"),1,     1536, 1,  0.0006510416666667},  //    _T("1/128\" = 1'"),   //    k1_128in_1ft
  {_T("1/64\" = 1'"), 1,     768,  2,  0.0013020833333333},  //    _T("1/64\" = 1'"),    //    k1_64in_1ft
  {_T("1/32\" = 1'"), 1,     384,  3,  0.0026041666666667},  //    _T("1/32\" = 1'"),    //    k1_32in_1ft
  {_T("1/16\" = 1'"), 1,     192,  4,  0.0052083333333333},  //    _T("1/16\" = 1'"),    //    k1_16in_1ft
  {_T("3/32\" = 1'"), 1,     128,  5,  0.0078125},           //    _T("3/32\" = 1'"),    //    k3_32in_1ft
  {_T("1/8\" = 1'"),  1,     96,   6,  0.0104166666666667},  //    _T("1/8\" = 1'"),     //    k1_8in_1ft,
  {_T("3/16\" = 1'"), 1,     64,   7,  0.015625},            //    _T("3/16\" = 1'"),    //    k3_16in_1ft
  {_T("1/4\" = 1'"),  1,     48,   8,  0.0208333333333333},   //   _T("1/4\" = 1'"),     //    k1_4in_1ft,
  {_T("3/8\" = 1'"),  1,     32,   9,  0.03125},             //    _T("3/8\" = 1'"),     //    k3_8in_1ft,
  {_T("1/2\" = 1'"),  1,     24,   10, 0.0416666666666667},  //    _T("1/2\" = 1'"),     //    k1_2in_1ft,
  {_T("3/4\" = 1'"),  1,     16,   11, 0.0625},              //    _T("3/4\" = 1'"),     //    k3_4in_1ft,
  {_T("1\" = 1'"),    1,     12,   12, 0.0833333333333333},  //    _T("1\" = 1'"),       //    k1in_1ft,
  {_T("3\" = 1'"),    1,     4,    13, 0.25},                //    _T("3\" = 1'"),       //    k3in_1ft,
  {_T("6\" = 1'"),    1,     2,    14, 0.5},                 //    _T("6\" = 1'"),       //    k6in_1ft,
  {_T("1' = 1'"),     1,     1,    15, 1},                   //    _T("1' = 1'"),        //    k1ft_1ft,
  {_T("1:1"),         1,     1,    16, 1},                   //    _T("1:1"),            //    k1_1,
  {_T("1:2"),         1,     2,    17, 0.5},                 //    _T("1:2"),            //    k1_2,
  {_T("1:4"),         1,     4,    18, 0.25},                //    _T("1:4"),            //    k1_4,
  {_T("1:5"),         1,     5,    19, 0.20},                //    _T("1:5"),            //    k1_5,
  {_T("1:8"),         1,     8,    20, 0.125},               //    _T("1:8"),            //    k1_8,
  {_T("1:10"),        1,     10,   21, 0.1},                 //    _T("1:10"),           //    k1_10, 
  {_T("1:16"),        1,     16,   22, 0.0625},              //    _T("1:16"),           //    k1_16, 
  {_T("1:20"),        1,     20,   23, 0.05},                //    _T("1:20"),           //    k1_20, 
  {_T("1:30"),        1,     30,   24, 0.03333333333333},    //    _T("1:30"),           //    k1_30, 
  {_T("1:40"),        1,     40,   25, 0.025},               //    _T("1:40"),           //    k1_40, 
  {_T("1:50"),        1,     50,   26, 0.2},                 //    _T("1:50"),           //    k1_50, 
  {_T("1:100"),       1,     100,  27, 0.01},                //    _T("1:100"),          //    k1_100,
  {_T("2:1"),         2,     1,    28, 2},                   //    _T("2:1"),            //    k2_1,
  {_T("4:1"),         4,     1,    29, 4},                   //    _T("4:1"),            //    k4_1,
  {_T("8:1"),         8,     1,    30, 8},                   //    _T("8:1"),            //    k8_1,
  {_T("10:1"),        10,    1,    31, 10},                  //    _T("10:1"),           //    k10_1, 
  {_T("100:1"),       100,   1,    32, 100},                 //    _T("100:1"),          //    k100_1,
  {_T("1000:1"),      1000,  1,    33, 1000},                //    _T("1000:1")          //    k1000_1
  {_T("1.5\"= 1'"),   1,     8,    34, 0.125}                //    _T("1.5\"= 1'")       //    k1and1_2in_1ft
};

struct unitsInfo
{
  double dScale;
  const TCHAR *pName1;
  const TCHAR *pName2;

  static const TCHAR *getTextByValue(double dValue, const unitsInfo &pInfo)
  {
    if (fabs(dValue) <= 1.)
    {
      return pInfo.pName1;
    }

    return pInfo.pName2;
  }
};

static unitsInfo unitsScale[] = {
  {kMmPerInch, _T("inch"), _T("inches")}, 
  {1., _T("mm"), _T("mm")} ,
  {1. , _T("pixel"), _T("pixels")},
  {1. , _T("unit"), _T("units")}
};

/////////////////////////////////////////////////////////////////////////////
// CPageSetupDlg

CPageSetupDlg::CPageSetupDlg(OdDbPlotSettings& params, OdSmartPtr<OdDbUserIO> pIO)
	: CDialog(CPageSetupDlg::IDD, NULL)
  , m_params(params)
  , m_pIO(pIO)
{
	//{{AFX_DATA_INIT(CPageSetupDlg)
    m_DPI = 0;
    m_nCenterThePlot = 0;
    m_dOffsetX = 0.;
    m_dOffsetY = 0.;
    m_dPIOX = "0.";
    m_dPIOY = "0.";
    m_radioDrawingOrientation = 0;
    m_dPaperScaleUnit = 0.;
    m_dDrawingScaleUnit = 0.;
    m_nFitToPaper = 0;
    m_dMarginsL = "0.";
    m_dMarginsR = "0.";
    m_dMarginsT = "0.";
    m_dMarginsB = "0.";
    m_nDisplayPlotStyles = 0;
    m_nPlotUpsideDown = 0;
    m_nScaleLW = 0;
    m_nPlotObjectLW = 0;
    m_nPlotWithPlotStyles = 0;
    m_nPlotPaperspaceLast = 0;
    m_nHidePaperspaceObjects = 0;
    m_nPlotTransparencies = 0;
    m_xMin = "0.";m_yMin = "0.";m_xMax = "0.";m_yMax = "0.";

	//}}AFX_DATA_INIT
}

CPageSetupDlg::~CPageSetupDlg()
{
}

void CPageSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSetupDlg)
  DDX_Control(pDX, IDC_BUTTON_ADD_MEDIA, m_add_media_btn);
  DDX_Control(pDX, IDC_PAGESETUP_QUALITY, m_comboQuality);
  DDX_Control(pDX, IDC_PAGESETUP_SHADE_PLOT, m_comboShadePlot);
  DDX_Control(pDX, IDC_PAGESETUP_VIEWS, m_comboViews);
  DDX_Control(pDX, IDC_COMBO_MM_INCHES, m_comboMMInches);
  DDX_Control(pDX, IDC_PAGESETUP_COMBO_PLOTSTYLEFILES, m_comboPlotStyleFiles);
  DDX_Control(pDX, IDC_PAGESETUP_DEVICE, m_comboPlotDeviceName);
  DDX_Control(pDX, IDC_PAGESETUP_SIZE, m_comboPaperSize);
  DDX_Control(pDX, IDC_PAGESETUP_PLOTAREATYPE, m_comboPlotAreaType);
  DDX_Check  (pDX, IDC_CHECK_CENTERTHEPLOT, m_nCenterThePlot);
  DDX_Check  (pDX, IDC_CHECK_FIT_TO_PAPER, m_nFitToPaper);
  DDX_Check  (pDX, IDC_CHECK_SCALE_LW, m_nScaleLW);
  DDX_Check  (pDX, IDC_CHECK_UPSIDEDOWN, m_nPlotUpsideDown);
  DDX_Check  (pDX, IDC_CHECK_DISPLAY_PLOT_STYLES, m_nDisplayPlotStyles);
  DDX_Text   (pDX, IDC_PAGESETUP_OFFSET_X, m_dOffsetX);
  DDX_Text   (pDX, IDC_PAGESETUP_OFFSET_Y, m_dOffsetY);
  DDX_Text   (pDX, IDC_CANONICAL_PAPER, m_CanonicalPaper);
  DDX_Text   (pDX, IDC_PIO_XX, m_dPIOX);
  DDX_Text   (pDX, IDC_PIO_YY, m_dPIOY);
  DDX_Text   (pDX, IDC_PAGESETUP_DPI, m_DPI);
  DDX_Text   (pDX, IDC_MARGINS_L, m_dMarginsL);
  DDX_Text   (pDX, IDC_MARGINS_R, m_dMarginsR);
  DDX_Text   (pDX, IDC_MARGINS_T, m_dMarginsT);
  DDX_Text   (pDX, IDC_MARGINS_B, m_dMarginsB);
  //DDX_Text   (pDX, IDC_PAGESETUP_PAPER_UNIT_STATIC, m_PaperUnitText);
  DDX_Text   (pDX, IDC_PAGESETUP_DRAWING_UNIT_STATIC, m_DrawingUnitText);
  DDX_Text   (pDX, IDC_PAGESETUP_OFFSET_X_STATIC, m_OffsetXText);
  DDX_Text   (pDX, IDC_PAGESETUP_OFFSET_Y_STATIC, m_OffsetYText);
  DDX_Text   (pDX, IDC_WINDOW_MINX, m_xMin);
  DDX_Text   (pDX, IDC_WINDOW_MINY, m_yMin);
  DDX_Text   (pDX, IDC_WINDOW_MAXX, m_xMax);
  DDX_Text   (pDX, IDC_WINDOW_MAXY, m_yMax);
  DDX_Radio(pDX, IDC_PAGESETUP_PORTRAIT, m_radioDrawingOrientation);
  DDX_Control(pDX, IDC_PAGESETUP_SCALE, m_comboScaleValues);
	DDX_Text(pDX, IDC_PAGESETUP_PAPER_UNIT, m_dPaperScaleUnit);
	DDX_Text(pDX, IDC_PAGESETUP_DRAWING_UNIT, m_dDrawingScaleUnit);
  DDX_Check  (pDX, IDC_CHECK_PLOT_OBJECT_LW, m_nPlotObjectLW);
  DDX_Check  (pDX, IDC_CHECK_PLOT_WITH_PLOTSTYLES, m_nPlotWithPlotStyles);
  DDX_Check  (pDX, IDC_CHECK_PLOT_PAPERSPACE_LAST, m_nPlotPaperspaceLast);
  DDX_Check  (pDX, IDC_CHECK_HIDE_PAPERSPACE_OBJECTS, m_nHidePaperspaceObjects);
  DDX_Check  (pDX, IDC_CHECK_PLOT_TRANSPARENCIES, m_nPlotTransparencies);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPageSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CPageSetupDlg)
    ON_CBN_SELCHANGE(IDC_PAGESETUP_DEVICE, OnSelchangeDeviceList)
    ON_CBN_SELCHANGE(IDC_PAGESETUP_SCALE, OnSelchangeScaleValues)
    ON_CBN_SELCHANGE(IDC_PAGESETUP_SIZE, OnSelChangeMediaList)
    ON_CBN_SELCHANGE(IDC_PAGESETUP_PLOTAREATYPE, OnSelChangePlotAreaType)
    ON_CBN_SELCHANGE(IDC_PAGESETUP_QUALITY, OnSelChangeQualityList)
    ON_CBN_SELCHANGE(IDC_PAGESETUP_SHADE_PLOT, OnSelChangeShadePlotList)
    ON_CBN_SELCHANGE(IDC_PAGESETUP_COMBO_PLOTSTYLEFILES, OnSelChangePlotStyleFiles)
    ON_CBN_SELCHANGE(IDC_PAGESETUP_VIEWS, OnSelChangeViewsList)
    ON_CBN_SELCHANGE(IDC_COMBO_MM_INCHES, OnSelChangeMMInchesList)

    ON_BN_CLICKED(IDC_CHECK_CENTERTHEPLOT, OnCheckCenterThePlot)
    ON_BN_CLICKED(IDC_CHECK_FIT_TO_PAPER, OnCheckFitToPaper)
    ON_BN_CLICKED(IDC_CHECK_SCALE_LW, OnCheckScaleLW)
    ON_BN_CLICKED(IDC_PAGESETUP_PORTRAIT, OnClickPortraitLandscape)
    ON_BN_CLICKED(IDC_PAGESETUP_LANDSCAPE, OnClickPortraitLandscape)
    ON_BN_CLICKED(IDC_CHECK_UPSIDEDOWN, OnClickPortraitLandscape)
    ON_BN_CLICKED(IDC_BUTTON_WINDOW, OnClickWindowButton)
    ON_BN_CLICKED(IDC_BUTTON_ADD_MEDIA, OnClickAddMediaButton)

    ON_BN_CLICKED(IDC_PAGESETUP_BUTTON_PLOTSTYLEFILES, OnClickPlotStyleFilesBtn)
    ON_BN_CLICKED(IDC_CHECK_DISPLAY_PLOT_STYLES, OnCheckDisplayPlotStyles)

    ON_BN_CLICKED(IDC_CHECK_PLOT_OBJECT_LW, OnClickPlotStyles)
    ON_BN_CLICKED(IDC_CHECK_PLOT_WITH_PLOTSTYLES, OnClickPlotStyles)
    ON_BN_CLICKED(IDC_CHECK_PLOT_PAPERSPACE_LAST, OnClickPlotStyles)
    ON_BN_CLICKED(IDC_CHECK_HIDE_PAPERSPACE_OBJECTS, OnClickPlotStyles)
    ON_BN_CLICKED(IDC_CHECK_PLOT_TRANSPARENCIES, OnCheckPlotTransparencies)

    ON_EN_KILLFOCUS(IDC_PAGESETUP_DPI, OnChangeEditDPI)
    ON_EN_KILLFOCUS(IDC_PAGESETUP_OFFSET_X, OnChangeEditOffsetXY)
    ON_EN_KILLFOCUS(IDC_PAGESETUP_OFFSET_Y, OnChangeEditOffsetXY)
    ON_EN_KILLFOCUS(IDC_PAGESETUP_PAPER_UNIT, OnChangeEditScaleUnit)
    ON_EN_KILLFOCUS(IDC_PAGESETUP_DRAWING_UNIT,OnChangeEditScaleUnit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPageSetupDlg::setPlotCfgName2Validator(OdString &pDeviceName, OdString canonicalMediaName, bool bValidNames)
{
  OdString currentDevice = m_params.getPlotCfgName();
  OdString currentMedia = m_params.getCanonicalMediaName();

  if (bValidNames && pDeviceName == currentDevice && currentMedia == canonicalMediaName)
    return;

  double dW, dH;
  m_params.getPlotPaperSize(dW, dH);
  OdDbPlotSettings::PlotPaperUnits units = m_params.plotPaperUnits();
  if (units == OdDbPlotSettings::kInches)
  {
    dW /= unitsScale[units].dScale;
    dH /= unitsScale[units].dScale;
  }

  if (m_pValidator->setPlotCfgName(&m_params, pDeviceName, canonicalMediaName) != eOk)   // good device, but wrong paper
  {
    if (m_pValidator->setPlotCfgName(&m_params, pDeviceName) != eOk ) // wrong device
    {
      // if not - select None.
      pDeviceName = NONE_DEVICE;
      m_pValidator->setPlotCfgName(&m_params, pDeviceName, "none_user_media");
    }
    else if (bValidNames)
    {
      if (m_pValidator->setClosestMediaName(&m_params, dW, dH, units, true) != eOk)
      {
        pDeviceName = NONE_DEVICE;
        m_pValidator->setPlotCfgName(&m_params, pDeviceName, "none_user_media");
      }
    }
  }
}

void CPageSetupDlg::FillMMInches(bool )
{
  m_comboMMInches.ResetContent();

  OdDbPlotSettings::PlotPaperUnits ppu = m_params.plotPaperUnits();
  if (ppu == OdDbPlotSettings::kPixels)
  {
    m_comboMMInches.AddString(_T("pixels"));
    m_comboMMInches.EnableWindow(FALSE);
    m_comboMMInches.SetCurSel(0);
  }
  else
  {
    m_comboMMInches.AddString(_T("inches"));
    m_comboMMInches.AddString(_T("mm"));
    m_comboMMInches.EnableWindow(TRUE);
    m_comboMMInches.SetCurSel(ppu == OdDbPlotSettings::kMillimeters);
  }
}

void CPageSetupDlg::FillViewCombo(bool bFillCombo)
{
  if (bFillCombo)
  {
    m_comboViews.ResetContent();

    OdDbViewTablePtr pViewTable =  m_params.database()->getViewTableId().safeOpenObject();
    OdDbSymbolTableIteratorPtr pIt = pViewTable->newIterator();
    while(!pIt->done())
    {
      OdDbViewTableRecordPtr pView = pIt->getRecord();
      if (pView->isPaperspaceView() != isModelSpacePageSetup())
      {
        m_comboViews.AddString( pView->getName() );
      }

      pIt->step();
    }
  }

  GetDlgItem(IDC_PAGESETUP_VIEWS)->EnableWindow( m_comboViews.GetCount() );
  if (m_comboViews.GetCount())
  {
    m_comboViews.SetCurSel( m_comboViews.FindStringExact(0, m_params.getPlotViewName() ));
  }
}

void CPageSetupDlg::FillShadePlotQualityDPI(bool bFillCombo)
{
  if (bFillCombo)
  {
    m_comboQuality.ResetContent();
    m_comboQuality.AddString(_T("Draft"));
    m_comboQuality.AddString(_T("Preview"));
    m_comboQuality.AddString(_T("Normal"));
    m_comboQuality.AddString(_T("Presentation"));
    m_comboQuality.AddString(_T("Maximum"));
    m_comboQuality.AddString(_T("Custom"));

    m_comboShadePlot.ResetContent();
    m_comboShadePlot.AddString(_T("As displayed"));
    m_comboShadePlot.AddString(_T("Wireframe"));
    m_comboShadePlot.AddString(_T("Hidden"));
    m_comboShadePlot.AddString(_T("Rendered"));
  }

  OdDbPlotSettings::ShadePlotType pShadePlot = m_params.shadePlot();
  m_comboShadePlot.SetCurSel((int)pShadePlot);

  OdDbPlotSettings::ShadePlotResLevel spr = m_params.shadePlotResLevel();
  m_comboQuality.SetCurSel((int)spr);

  if (spr == OdDbPlotSettings::kCustom)
    m_DPI = m_params.shadePlotCustomDPI();

  if (isModelSpacePageSetup())
  {
    bool bEnableWindows = pShadePlot == OdDbPlotSettings::kAsDisplayed || pShadePlot == OdDbPlotSettings::kRendered;
    GetDlgItem(IDC_PAGESETUP_QUALITY)->EnableWindow( bEnableWindows );
    GetDlgItem(IDC_PAGESETUP_DPI    )->EnableWindow( spr == OdDbPlotSettings::kCustom && bEnableWindows );
  }
  else
  {
    GetDlgItem(IDC_PAGESETUP_SHADE_PLOT)->EnableWindow( FALSE );
    GetDlgItem(IDC_PAGESETUP_QUALITY)->EnableWindow( TRUE );
    GetDlgItem(IDC_PAGESETUP_DPI    )->EnableWindow( spr == OdDbPlotSettings::kCustom );
  }

  UpdateData( FALSE );
}

void CPageSetupDlg::OnClickPlotStyles()
{
  UpdateData( );

  m_params.setPrintLineweights(m_nPlotObjectLW == 1);
  m_params.setPlotPlotStyles(m_nPlotWithPlotStyles == 1);
  m_params.setDrawViewportsFirst(m_nPlotPaperspaceLast == 1);
  m_params.setPlotHidden(m_nHidePaperspaceObjects == 1);

  FillPlotStyles();
}

void CPageSetupDlg::OnCheckPlotTransparencies()
{
  UpdateData();
  m_params.setPlotTransparency(m_nPlotTransparencies == 1);
}

void CPageSetupDlg::FillPlotStyles()
{
  m_nPlotObjectLW = m_params.printLineweights();
  m_nPlotWithPlotStyles = m_params.plotPlotStyles();
  m_nPlotPaperspaceLast = m_params.drawViewportsFirst();
  m_nHidePaperspaceObjects = m_params.plotHidden();
  m_nPlotTransparencies = m_params.plotTransparency();

  GetDlgItem(IDC_CHECK_PLOT_PAPERSPACE_LAST)->EnableWindow( !isModelSpacePageSetup() );
  GetDlgItem(IDC_CHECK_HIDE_PAPERSPACE_OBJECTS)->EnableWindow( !isModelSpacePageSetup() );

  UpdateData( FALSE );
}

void CPageSetupDlg::FillPaperOrientation()
{
  OdDbPlotSettings::PlotRotation pr = m_params.plotRotation();
  
  m_radioDrawingOrientation = pr & 1;
  if (!isPaperWidthLessHeight())
    m_radioDrawingOrientation = !m_radioDrawingOrientation;
  
  m_nPlotUpsideDown = (pr & 2) / 2;

  UpdateData( FALSE );
}

void CPageSetupDlg::OnClickPortraitLandscape()
{
  UpdateData( );

  OdDbPlotSettings::PlotRotation pr;

  if (isPaperWidthLessHeight())
    pr =(OdDbPlotSettings::PlotRotation)(m_radioDrawingOrientation + m_nPlotUpsideDown*2);
  else
    pr =(OdDbPlotSettings::PlotRotation)((!m_radioDrawingOrientation) + m_nPlotUpsideDown*2);
  
  m_pValidator->setPlotRotation(&m_params, pr);

  FillPaperOrientation();
  FillScaleValues(false);
  FillPlotOffset();
}

void CPageSetupDlg::OnChangeEditScaleUnit()
{
  double dOldPaperScaleUnit = m_dPaperScaleUnit
       , dOldDrawingScaleUnit = m_dDrawingScaleUnit;
  UpdateData( );

  if (dOldPaperScaleUnit != m_dPaperScaleUnit || dOldDrawingScaleUnit != m_dDrawingScaleUnit)
  {
    //OdDbPlotSettings::PlotPaperUnits ppu = m_params.plotPaperUnits();
    ODA_VERIFY( m_pValidator->setCustomPrintScale(
      &m_params,
      m_dPaperScaleUnit,
      m_dDrawingScaleUnit) == eOk);

    FillPaperOrientation();
    FillScaleValues(false);
    FillPlotOffset();
  }
}

void CPageSetupDlg::OnCheckFitToPaper()
{
  UpdateData( );

  if (m_nFitToPaper)
  {
    m_pValidator->setStdScaleType(&m_params, OdDbPlotSettings::kScaleToFit);
  }
  else
  {
    m_pValidator->setUseStandardScale(&m_params, false);
  }

  FillPaperOrientation();
  FillScaleValues(false);
  FillPlotOffset();
}

void CPageSetupDlg::OnCheckScaleLW()
{
  UpdateData( );

  m_params.setScaleLineweights(m_nScaleLW != 0);

  FillPaperOrientation();
  FillScaleValues(false);
  FillPlotOffset();
}

void CPageSetupDlg::OnSelchangeScaleValues()
{
  UpdateData( );

  int i = m_comboScaleValues.GetCurSel();
  if (i != 0) // skip Custom
    m_pValidator->setStdScaleType(&m_params, StdScaleType(plotScaleSetting[i].m_ScaleType));

  FillPaperOrientation();
  FillScaleValues(false);
  FillPlotOffset();
}

OdDbPlotSettings::PlotPaperUnits CPageSetupDlg::getMediaNativePPU()
{ // This method uses the backdoor way to define PPU from Media.
  OdDbPlotSettings params;
  m_pValidator->setPlotCfgName(&params, m_params.getPlotCfgName(), m_params.getCanonicalMediaName());

  return params.plotPaperUnits();
}

void CPageSetupDlg::OnSelChangeMediaList()
{
  UpdateData( );

  CString newLocaleMediaName;
  int i = m_comboPaperSize.GetCurSel();
  m_comboPaperSize.GetLBText(i, newLocaleMediaName);

  OdString newCanonicalMediaName = getCanonicalByLocaleMediaName(OdString((LPCTSTR)newLocaleMediaName));

  m_pValidator->setCanonicalMediaName(&m_params, newCanonicalMediaName);
  OdDbPlotSettings::PlotPaperUnits nativeUnits = getMediaNativePPU();

  FillPaperSizes();
  m_comboPaperSize.SetCurSel(m_comboPaperSize.FindStringExact(0, newLocaleMediaName));

  // change paper orientation to dialog values
  OdDbPlotSettings::PlotRotation pr;
  if (isPaperWidthLessHeight())
    pr =(OdDbPlotSettings::PlotRotation)(m_radioDrawingOrientation + m_nPlotUpsideDown*2);
  else
    pr =(OdDbPlotSettings::PlotRotation)((!m_radioDrawingOrientation) + m_nPlotUpsideDown*2);
  m_pValidator->setPlotRotation(&m_params, pr);

  FillPaperOrientation();
  FillScaleValues(false);
  FillPlotOffset();

  // and reset units to paper native
  if (nativeUnits == OdDbPlotSettings::kInches || nativeUnits == OdDbPlotSettings::kMillimeters)
  {
    m_comboMMInches.SetCurSel(nativeUnits == OdDbPlotSettings::kMillimeters);
    OnSelChangeMMInchesList();
  }
}

OdString CPageSetupDlg::getCanonicalByLocaleMediaName(OdString localeMediaName)
{
  OdArray<const OdChar *> mediaNames;
  m_pValidator->canonicalMediaNameList(&m_params, mediaNames);

  OdArray<const OdChar *>::const_iterator pIt = mediaNames.begin();
  OdArray<const OdChar *>::const_iterator pEnd = mediaNames.end();

  while(pIt != pEnd)
  {
    if (m_pValidator->getLocaleMediaName(&m_params, pIt - mediaNames.begin()) == localeMediaName)
    {
      return *pIt;
    }
    ++pIt;
  }

  ODA_ASSERT(0);
  return mediaNames.first();
}

void CPageSetupDlg::OnSelchangeDeviceList()
{
  UpdateData( );

  CString newDeviceName;
  int i = m_comboPlotDeviceName.GetCurSel();
  m_comboPlotDeviceName.GetLBText(i, newDeviceName);

  OdString canonicalMediaName = m_params.getCanonicalMediaName();
  
  OdString strDeviceName(newDeviceName);
  setPlotCfgName2Validator(strDeviceName, canonicalMediaName, true);

  if (!strDeviceName.compare(CUSTOM_DEVICE_NAME))
    m_add_media_btn.ShowWindow(SW_SHOWNORMAL);
  else
    m_add_media_btn.ShowWindow(SW_HIDE);

  m_comboPlotDeviceName.SelectString(0, strDeviceName);

  // fill paper sizes combo box
  if (!FillPaperSizes())
  {
    return /*FALSE*/;
  }

  OdString localeMediaName = m_pValidator->getLocaleMediaName(&m_params, m_params.getCanonicalMediaName());

  // select active media
  if (m_comboPaperSize.SetCurSel(m_comboPaperSize.FindStringExact(0, localeMediaName)) == LB_ERR)
  {
    // ALEXR TODO : Autocad use paper w&h to find nearest paper or set a4 ?
    // ALEXR TODO : SelectString select by part of string. 'q' -> select 'qwe'.
    CString csLocaleMediaName;
    m_comboPaperSize.GetLBText(0, csLocaleMediaName);
    if (csLocaleMediaName == _T(""))
    {
      return /*FALSE*/;
    }

    canonicalMediaName = getCanonicalByLocaleMediaName(OdString((LPCTSTR)csLocaleMediaName));
    m_pValidator->setCanonicalMediaName(&m_params, canonicalMediaName);
    
    m_comboPaperSize.SetCurSel(m_comboPaperSize.FindStringExact(0, csLocaleMediaName));
  }

  FillPlotAreaCombo(false);
  FillPlotOffset();

  FillScaleValues(false);
  FillPaperOrientation();
  FillPlotStyles();
}

BOOL CPageSetupDlg::OnInitDialog()
{
  if (!CDialog::OnInitDialog())
    return FALSE;

  if (!m_params.database() || !m_params.database()->appServices())
    return FALSE;

  m_pValidator = m_params.database()->appServices()->plotSettingsValidator();
  if (m_pValidator.isNull())
    return FALSE;

  OdDbPlotSettingsValidatorCustomMediaPEPtr pPE = OdDbPlotSettingsValidatorPE::desc()->getX(OdDbPlotSettingsValidatorCustomMediaPE::desc());

  if (!pPE.isNull())
  {
    OdDbPlotSettingsValidatorPE::psvPaperInfo paperInfo;

    paperInfo.localeName = "ISO A4 (297.00 x 210.00 MM)";
    paperInfo.canonicalName = "ISO_A4_(297.00_x_210.00_MM)";
    paperInfo.w = 297.;
    paperInfo.h = 210.;
    paperInfo.units = OdDbPlotSettings::kMillimeters;
    paperInfo.left = 5.793749809265137;
    paperInfo.bottom = 5.793762207031250;
    paperInfo.top = 17.793746948242188;
    paperInfo.right = 17.793750762939453;
    pPE->addMedia(paperInfo);
  }

  // fill device list
  m_pValidator->refreshLists(&m_params);

  // is stored device name available in system ?
  OdString deviceName = m_params.getPlotCfgName();
  OdString mediaName  = m_params.getCanonicalMediaName();

  setPlotCfgName2Validator(deviceName, mediaName, false);

  // fill device combo
  if (!FillDeviceCombo())
    return FALSE;

  // select device in combo list.
  m_comboPlotDeviceName.SelectString(0, deviceName);
  UpdateData(FALSE);


  // select active paper from plot settings
  // au doesn't use media name stored in dxf, possible au
  // look for name by paper parameters.
  OnSelchangeDeviceList();

  FillPlotAreaCombo(true);
  FillPlotOffset();

  FillScaleValues(true);
  FillPaperOrientation();
  FillPlotStyles();
  FillShadePlotQualityDPI(true);
  FillPlotStyleCombo(true);
  FillViewCombo(true);
  FillWindowArea();

  return TRUE;
}

bool CPageSetupDlg::FillDeviceCombo()
{
  OdArray<const OdChar *> devices;
  m_pValidator->plotDeviceList(devices);

  m_comboPlotDeviceName.ResetContent();

  OdArray<const OdChar *>::const_iterator pIt = devices.begin();
  OdArray<const OdChar *>::const_iterator pEnd = devices.end();

  while(pIt != pEnd)
  {
    m_comboPlotDeviceName.AddString((LPCTSTR)OdString(*pIt));
    ++pIt;
  }

  UpdateData(FALSE);

  return true;
}

bool CPageSetupDlg::FillPaperSizes()
{
  OdArray<const OdChar *> canonicalMediaNames;
  m_pValidator->canonicalMediaNameList(&m_params, canonicalMediaNames);

  m_comboPaperSize.ResetContent();

  for(OdUInt32 f=0; f<canonicalMediaNames.size(); ++f)
  {
    m_comboPaperSize.AddString(m_pValidator->getLocaleMediaName(&m_params, f));
  }

  UpdateData(FALSE);

  return true;
}

void CPageSetupDlg::FillScaleValues(bool bFillCombo)
{
  if (bFillCombo)
  {
    m_comboScaleValues.ResetContent();

    int iSize = sizeof(plotScaleSetting)/sizeof(plotScaleSetting[0]);
    for (int i = 0; i < iSize; i++)
    {
      m_comboScaleValues.AddString(plotScaleSetting[i].m_Str);
    }
  }
  
  OdDbPlotSettings::StdScaleType sst = m_params.stdScaleType();
  if (m_params.useStandardScale() && sst != OdDbPlotSettings::kScaleToFit && sst >=0 && sst <=OdDbPlotSettings::k1and1_2in_1ft)
  {
    m_comboScaleValues.SetCurSel(m_comboScaleValues.FindStringExact(0, plotScaleSetting[sst].m_Str));
  }
  else
  {
    m_comboScaleValues.SetCurSel(m_comboScaleValues.FindStringExact(0, _T("Custom")));
  }

  bool isModel = isModelSpacePageSetup();
  bool isLayoutMode = m_params.plotType() == OdDbPlotSettings::kLayout;

  m_nFitToPaper = m_params.useStandardScale() && !isLayoutMode && (sst == OdDbPlotSettings::kScaleToFit);
  m_nScaleLW    = m_params.scaleLineweights();

  if (isLayoutMode)
  {
    m_nFitToPaper = m_nCenterThePlot = false;
  }

  GetDlgItem(IDC_CHECK_SCALE_LW    )->EnableWindow( !isModel );
  GetDlgItem(IDC_CHECK_FIT_TO_PAPER)->EnableWindow( !isLayoutMode );
  GetDlgItem(IDC_CHECK_CENTERTHEPLOT)->EnableWindow( !isLayoutMode );

  GetDlgItem(IDC_PAGESETUP_SCALE)->EnableWindow( !m_nFitToPaper );
  GetDlgItem(IDC_PAGESETUP_PAPER_UNIT)->EnableWindow( !m_nFitToPaper );
  GetDlgItem(IDC_PAGESETUP_DRAWING_UNIT)->EnableWindow( !m_nFitToPaper );

  if (m_params.useStandardScale() && !m_nFitToPaper)
  {
    m_dPaperScaleUnit   = plotScaleSetting[sst].m_dRealWorldUnits;
    m_dDrawingScaleUnit = plotScaleSetting[sst].m_dDrawingUnits;
  }
  else
  {
    m_params.getCustomPrintScale(m_dPaperScaleUnit, m_dDrawingScaleUnit);
  }

  FillMMInches(bFillCombo);
  //m_PaperUnitText   = CString(unitsInfo::getTextByValue(m_dPaperScaleUnit, unitsScale[ppu])) + _T(" =");
  m_DrawingUnitText = unitsInfo::getTextByValue(m_dDrawingScaleUnit, unitsScale[3]);

  UpdateData(FALSE);  
}

bool CPageSetupDlg::isWHSwap() const
{
  OdDbPlotSettings::PlotRotation pr = m_params.plotRotation();
  return pr == OdDbPlotSettings::k90degrees || pr == OdDbPlotSettings::k270degrees;
}

void CPageSetupDlg::OnChangeEditOffsetXY()
{
  UpdateData( );

  OdDbPlotSettings::PlotPaperUnits ppu = m_params.plotPaperUnits();
  if (ppu == OdDbPlotSettings::kInches)
  {
    m_dOffsetX *= unitsScale[ppu].dScale;
    m_dOffsetY *= unitsScale[ppu].dScale;
  }

  if (isWHSwap())
    m_pValidator->setPlotOrigin(&m_params, m_dOffsetY, m_dOffsetX);
  else
    m_pValidator->setPlotOrigin(&m_params, m_dOffsetX, m_dOffsetY);

  FillPaperOrientation();
  FillPlotOffset(); // possibly offset was changed in validator
  FillScaleValues(false);
}

void CPageSetupDlg::OnCheckCenterThePlot()
{
  UpdateData( );

  m_pValidator->setPlotCentered(&m_params, m_nCenterThePlot != 0);

  FillPaperOrientation();
  FillPlotOffset();
  FillScaleValues(false);
}

void CPageSetupDlg::OnSelChangePlotAreaType()
{
  UpdateData( );

  CString newViewType;
  int i = m_comboPlotAreaType.GetCurSel();
  m_comboPlotAreaType.GetLBText(i, newViewType);

  OdDbPlotSettings::PlotType pt = OdDbPlotSettings::kDisplay;
  if (newViewType == _T("Display"))
  {
    pt = OdDbPlotSettings::kDisplay;
  }
  else if (newViewType == _T("Limits"))
  {
    pt = OdDbPlotSettings::kLimits;
  }
  else if (newViewType == _T("View"))
  {
    pt = OdDbPlotSettings::kView;
  }
  else if (newViewType == _T("Window"))
  {
    pt = OdDbPlotSettings::kWindow;
    OnClickWindowButton();
  }
  else if (newViewType == _T("Extents"))
  {
    pt = OdDbPlotSettings::kExtents;
  }
  else if (newViewType == _T("Layout"))
  {
    pt = OdDbPlotSettings::kLayout;
  }

  m_pValidator->setPlotType(&m_params, pt);

  FillPlotAreaCombo(false);

  if (pt == OdDbPlotSettings::kLayout)
  {
    // This is differences between dialog and validator. Validator doesn't
    // change UseStandardScale to false. Example is kExtents, kFit2Paper -> kLayout ->kExtents
    // Dialog has kFit2Paper disabled, but validator don't clear kFit2Paper flag.
    // Validator also don't change PlotOrigin to 0,0, if plotsenteres was true, but it change scale to 1:1 if fittopaper was true
    
    if (m_nCenterThePlot)
    {
      m_pValidator->setPlotOrigin(&m_params, 0., 0.);
    }
    if (m_nFitToPaper)
    {
      m_pValidator->setUseStandardScale(&m_params, false);
    }
  }

  FillPaperOrientation();
  FillScaleValues(false);
  FillPlotOffset();
}

bool CPageSetupDlg::isPaperWidthLessHeight() const
{
  double paperWidth, paperHeight;
  m_params.getPlotPaperSize(paperWidth, paperHeight);
  return paperWidth < paperHeight;
}

void CPageSetupDlg::FillWindowArea()
{
  double xmin, ymin, xmax, ymax;
  m_params.getPlotWindowArea(xmin, ymin, xmax, ymax);

  m_xMin.Format(L"%.6f", xmin);
  m_yMin.Format(L"%.6f", ymin);
  m_xMax.Format(L"%.6f", xmax);
  m_yMax.Format(L"%.6f", ymax);

  UpdateData(FALSE);
}

void CPageSetupDlg::FillPlotOffset()
{
  m_nCenterThePlot = m_params.plotCentered();

  GetDlgItem(IDC_PAGESETUP_OFFSET_X)->EnableWindow( !m_nCenterThePlot );
  GetDlgItem(IDC_PAGESETUP_OFFSET_Y)->EnableWindow( !m_nCenterThePlot );

  if (isWHSwap())
    m_params.getPlotOrigin(m_dOffsetY, m_dOffsetX);
  else
    m_params.getPlotOrigin(m_dOffsetX, m_dOffsetY);

  OdDbPlotSettings::PlotPaperUnits ppu = m_params.plotPaperUnits();
  if (ppu == OdDbPlotSettings::kInches)
  {
    m_dOffsetX /= unitsScale[ppu].dScale;
    m_dOffsetY /= unitsScale[ppu].dScale;
  }

  // it doesn't changed with isWHSwap
  m_dPIOX.Format(L"%.6f", m_params.getPaperImageOrigin().x);
  m_dPIOY.Format(L"%.6f", m_params.getPaperImageOrigin().y);

  m_dMarginsL.Format(L"%.6f", m_params.getLeftMargin());
  m_dMarginsR.Format(L"%.6f", m_params.getRightMargin());
  m_dMarginsT.Format(L"%.6f", m_params.getTopMargin());
  m_dMarginsB.Format(L"%.6f", m_params.getBottomMargin());

  m_CanonicalPaper = (LPCTSTR)m_params.getCanonicalMediaName();

  m_OffsetXText = unitsInfo::getTextByValue(m_dOffsetX, unitsScale[ppu]);
  m_OffsetYText = unitsInfo::getTextByValue(m_dOffsetY, unitsScale[ppu]);

  UpdateData(FALSE);
}

bool CPageSetupDlg::viewsExists() const
{
  OdDbViewTablePtr pViewTable =  m_params.database()->getViewTableId().safeOpenObject();
  OdDbSymbolTableIteratorPtr pIt = pViewTable->newIterator();
  while(!pIt->done())
  {
    OdDbViewTableRecordPtr pView = pIt->getRecord();
    if (pView->isPaperspaceView() != isModelSpacePageSetup())
    {
      return true;
    }

    pIt->step();
  }
  return false;
}

void CPageSetupDlg::FillPlotAreaCombo(bool bFillCombo)
{
  if (bFillCombo)
  {
    m_comboPlotAreaType.ResetContent();

    if (isModelSpacePageSetup())
    {
      m_comboPlotAreaType.AddString(_T("Display"));
      m_comboPlotAreaType.AddString(_T("Extents"));// TODO : depends on entities existance
      m_comboPlotAreaType.AddString(_T("Limits"));
      
      if ( viewsExists() )
        m_comboPlotAreaType.AddString(_T("View"));
      
      m_comboPlotAreaType.AddString(_T("Window"));
    }
    else
    {
      m_comboPlotAreaType.AddString(_T("Display"));
      m_comboPlotAreaType.AddString(_T("Extents"));// TODO : depends on entities existance
      m_comboPlotAreaType.AddString(_T("Layout"));
      
      if ( viewsExists() )
        m_comboPlotAreaType.AddString(_T("View"));

      m_comboPlotAreaType.AddString(_T("Window"));
    }
  }

  OdDbPlotSettings::PlotType pt = m_params.plotType();

  GetDlgItem(IDC_PAGESETUP_VIEWS)->ShowWindow( (pt == OdDbPlotSettings::kView  ) ? SW_SHOW : SW_HIDE );
  GetDlgItem(IDC_BUTTON_WINDOW  )->ShowWindow( (pt == OdDbPlotSettings::kWindow) ? SW_SHOW : SW_HIDE );
  
  switch(pt)
  {
    case OdDbPlotSettings::kDisplay:
      {
        m_comboPlotAreaType.SelectString(0, _T("Display"));
      }
      break;
    case OdDbPlotSettings::kExtents:
      {
        m_comboPlotAreaType.SelectString(0, _T("Extents"));
      }
      break;
    case OdDbPlotSettings::kLimits:
      {
        m_comboPlotAreaType.SelectString(0, _T("Limits"));
      }
      break;
    case OdDbPlotSettings::kView:
      {
        m_comboPlotAreaType.SelectString(0, _T("View"));
      }
      break;
    case OdDbPlotSettings::kWindow:
      {
        m_comboPlotAreaType.SelectString(0, _T("Window"));
      }
      break;
    case OdDbPlotSettings::kLayout:
      {
        m_comboPlotAreaType.SelectString(0, _T("Layout"));
      }
      break;
  };

  UpdateData(FALSE);
}


void CPageSetupDlg::OnOK()
{
  UpdateData();                   // TRUE - data is being retrieved 

  CDialog::OnOK();
}

void CPageSetupDlg::OnCancel()
{
  CDialog::OnCancel();
}

bool CPageSetupDlg::isModelSpacePageSetup() const
{
  return m_params.modelType();
}


bool CPageSetupDlg::FillArrayByPatternFile(OdArray<CString> &arrFiles, const CString sPattern)
{
  WIN32_FIND_DATA lpFindFileData;
  memset( &lpFindFileData, 0, sizeof(WIN32_FIND_DATA));
  CString sFolder = sPattern.Left(sPattern.ReverseFind(_T('\\'))+1);
  HANDLE hFindFile = FindFirstFile(sPattern, &lpFindFileData);
  CString sFile;

  BOOL bFind = true;
  bool bIsFind = false;
	do{
         if( lpFindFileData.dwFileAttributes & ~FILE_ATTRIBUTE_DIRECTORY )
         {
      		sFile = sFolder + lpFindFileData.cFileName;
            arrFiles.append(sFile);
            bIsFind = true;
         }

      	 bFind = FindNextFile( hFindFile, &lpFindFileData );

	}while( bFind && bFind != ERROR_NO_MORE_FILES  );

	FindClose( hFindFile );
  return bIsFind;
}

void CPageSetupDlg::FillPlotStyleCombo(bool bFillCombo)
{
  USES_CONVERSION;
  if (bFillCombo)
  {
    OdArray<const OdChar *> PSSlist;
    m_pValidator->plotStyleSheetList(PSSlist);

	  m_comboPlotStyleFiles.AddString(_T("None"));
	  for(unsigned int i = 0; i < PSSlist.length(); i++ )
	  {
	    m_comboPlotStyleFiles.AddString(W2T((wchar_t*)(PSSlist[i])));
	  }
  }

  int indx = 0;
  OdString curSS = m_params.getCurrentStyleSheet();
  if (!curSS.isEmpty())
  {
    indx = m_comboPlotStyleFiles.FindStringExact(0, curSS);
    if (indx == -1)
      indx = 0;
  }
  m_comboPlotStyleFiles.SetCurSel(indx);
  OnSelChangePlotStyleFiles();

  GetDlgItem(IDC_CHECK_DISPLAY_PLOT_STYLES)->EnableWindow( !isModelSpacePageSetup() );
  m_nDisplayPlotStyles = m_params.showPlotStyles();

  UpdateData(FALSE);
}

void CPageSetupDlg::OnClickPlotStyleFilesBtn()
{
  int idx = m_comboPlotStyleFiles.GetCurSel();

  CString tmp;
  m_comboPlotStyleFiles.GetLBText(idx, tmp);

  OdString sPath = LPCTSTR(tmp);

  try
  {  
    OdStreamBufPtr pFileBuf;
    bool bSucc(false);
    OdDbSystemServices* pSs = odSystemServices();
    sPath = m_params.database()->appServices()->findFile(sPath);
    if (sPath.isEmpty())
      return;

    if(pSs->accessFile(sPath, Oda::kFileRead))
    {
      bSucc = true;
      pFileBuf = pSs->createFile(sPath);
    }
    if(!bSucc) return;
    OdPsPlotStyleTablePtr pPlotStyleTable;
    if(pFileBuf.get())
    {
      OdPsPlotStyleServicesPtr pPSS = odrxDynamicLinker()->loadApp(ODPS_PLOTSTYLE_SERVICES_APPNAME);
      if(pPSS.get())
      {
        pPlotStyleTable = pPSS->loadPlotStyleTable(pFileBuf);
      }
    }

    CPsTableEditorDlg PsTableEditorDlg(theApp.GetMainWnd());
    PsTableEditorDlg.SetFileBufPath(sPath);
    PsTableEditorDlg.SetPlotStyleTable(pPlotStyleTable);
    if( PsTableEditorDlg.DoModal() == IDOK)
    {
      pPlotStyleTable->copyFrom(PsTableEditorDlg.GetPlotStyleTable());
    }
  }
  
  catch(...)
  {
    return;
  }
}

void CPageSetupDlg::OnSelChangePlotStyleFiles()
{
  int idx = m_comboPlotStyleFiles.GetCurSel();
  GetDlgItem(IDC_PAGESETUP_BUTTON_PLOTSTYLEFILES)->EnableWindow( idx );
  
  if (idx)
  {
    CString tmp;
    m_comboPlotStyleFiles.GetLBText(idx, tmp);
    m_pValidator->setCurrentStyleSheet(&m_params, OdString(tmp));
  }
  else
  {
    m_pValidator->setCurrentStyleSheet(&m_params, OdString::kEmpty);
  }
}

void CPageSetupDlg::OnSelChangeQualityList()
{
  UpdateData();

  int idx = m_comboQuality.GetCurSel();
  m_params.setShadePlotResLevel( (OdDbPlotSettings::ShadePlotResLevel)idx );

  FillShadePlotQualityDPI(false);
}

void CPageSetupDlg::OnSelChangeShadePlotList()
{
  UpdateData();

  int idx = m_comboShadePlot.GetCurSel();
  
  m_params.setShadePlot( (OdDbPlotSettings::ShadePlotType)idx );

  FillShadePlotQualityDPI(false);
}


void CPageSetupDlg::OnSelChangeViewsList()
{
  UpdateData();

  CString viewName;
  int idx = m_comboViews.GetCurSel();
  m_comboViews.GetLBText(idx, viewName);

  m_pValidator->setPlotViewName( &m_params, OdString(viewName) );

  FillViewCombo(false);
}

void CPageSetupDlg::unitsConverted(OdDbPlotSettings::PlotPaperUnits prevUnits, OdDbPlotSettings::PlotPaperUnits plotPaperUnits)
{
  double dCoeff = 0;
  if (plotPaperUnits == OdDbPlotSettings::kMillimeters && prevUnits == OdDbPlotSettings::kInches)
  {
    dCoeff = 25.4;
  }
  else if (plotPaperUnits == OdDbPlotSettings::kInches && prevUnits == OdDbPlotSettings::kMillimeters)
  {
    dCoeff = 1. / 25.4;
  }
  else
  {
    return;
  }

  OdGePoint2d paperImageOrigin = m_params.getPaperImageOrigin();
  paperImageOrigin /= dCoeff;
  m_params.setPaperImageOrigin(paperImageOrigin);

  bool bStandardScale = m_params.useStandardScale();
  if (bStandardScale)
  {
    double dStandardScale;
    m_params.getStdScale(dStandardScale);
    if (dStandardScale != 0) // skip Fit to paper
      m_pValidator->setCustomPrintScale(&m_params, dStandardScale, 1. / dCoeff);
  }
  else
  {
    double numerator, denominator;
    m_params.getCustomPrintScale(numerator, denominator);
    m_pValidator->setCustomPrintScale(&m_params, numerator, denominator / dCoeff);
  }
}

void CPageSetupDlg::OnSelChangeMMInchesList()
{
  UpdateData();

  CString unitsStr;
  int idx = m_comboMMInches.GetCurSel();
  m_comboMMInches.GetLBText(idx, unitsStr);

  OdDbPlotSettings::PlotPaperUnits plotPaperUnits = OdDbPlotSettings::kPixels;
  if (unitsStr == "mm") plotPaperUnits = OdDbPlotSettings::kMillimeters;
  else if (unitsStr == "inches") plotPaperUnits = OdDbPlotSettings::kInches;

  OdDbPlotSettings::PlotPaperUnits prevUnits = m_params.plotPaperUnits();

  ODA_VERIFY(m_pValidator->setPlotPaperUnits( &m_params, plotPaperUnits ) == eOk);

  // cr 5751 fix - I didn't test kPixels way.
  unitsConverted( prevUnits, plotPaperUnits);

  FillScaleValues(false);
  FillPlotOffset();
}

void CPageSetupDlg::OnChangeEditDPI()
{
  UpdateData();

  m_params.setShadePlotCustomDPI( m_DPI );

  FillShadePlotQualityDPI(false);
}

void CPageSetupDlg::OnCheckDisplayPlotStyles()
{
  UpdateData();

  m_params.setShowPlotStyles( m_nDisplayPlotStyles != 0 );

  FillPlotStyleCombo(false);
}

void CPageSetupDlg::OnClickAddMediaButton()
{
  OdDbPlotSettingsValidatorPE::psvPaperInfo paperInfo;
  CCustomMediaDlg dlgCM(&paperInfo, this);
  if (dlgCM.DoModal() == IDOK)
  {
    OdDbPlotSettingsValidatorCustomMediaPEPtr pPE = OdDbPlotSettingsValidatorPE::desc()->getX(OdDbPlotSettingsValidatorCustomMediaPE::desc());
    if (!pPE.isNull())
    {
      pPE->addMedia(paperInfo);

      m_pValidator->setCanonicalMediaName(&m_params, "none_user_media");
      FillPaperSizes();
      m_pValidator->setCanonicalMediaName(&m_params, paperInfo.canonicalName);
      OnSelchangeDeviceList();
    }
  }
}

#include "DwgViewer.h"

void CPageSetupDlg::OnClickWindowButton()
{
  UpdateData();

  ShowWindow(SW_HIDE);
  
    CWnd *pParent = GetParent();
    pParent->EnableWindow(TRUE);
    pParent->BringWindowToTop();
    
    OdGePoint3d pt1 = m_pIO->getPoint(_T("Specify first corner:"),    OdEd::kGptNoUCS);
    OdGePoint3d pt2 = m_pIO->getPoint(_T("Specify opposite corner:"), OdEd::kGptNoUCS|OdEd::kGptRectFrame);

    // Points are returned in eye plane, transform it back to screen plane if it is possible
    // Workaround, unfortunately can't get screen plane point from IO stream.
  	CMDIChildWnd* pWnd = (((CMDIFrameWnd*)theApp.GetMainWnd())->MDIGetActive());
    CView *pView = pWnd->GetActiveView();
    if (pView->IsKindOf(RUNTIME_CLASS(CDwgViewer)) && !pView->IsKindOf(RUNTIME_CLASS(CRemoteGeomViewer)))
    {
      CDwgViewer *pDwgView = static_cast<CDwgViewer*>(pView);
      if (pDwgView->isModelSpaceView())
      {
        pt1 = pDwgView->editorObject().toScreenCoord(pt1);
        pt2 = pDwgView->editorObject().toScreenCoord(pt2);
      }
    }
    //

    m_pValidator->setPlotWindowArea(&m_params, pt1.x, pt1.y, pt2.x, pt2.y);

    pParent->EnableWindow(FALSE);

  EnableWindow(TRUE);
  ShowWindow(SW_SHOW);

  FillWindowArea();
}
