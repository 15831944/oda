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
// PageSetupDlg.h : header file
//


#ifndef PAGE_SETUP_DLG_DEFINED
#define PAGE_SETUP_DLG_DEFINED

#include "DbPlotSettings.h"
#include "DbPlotSettingsValidator.h"
#include "DbUserIO.h"

/////////////////////////////////////////////////////////////////////////////
class CPageSetupDlg : public CDialog
{
private:
    OdDbPlotSettings            &m_params;
    OdDbPlotSettingsValidatorPtr m_pValidator;
    OdSmartPtr<OdDbUserIO>       m_pIO;

    OdString getCanonicalByLocaleMediaName(OdString localeMediaName);
    void setPlotCfgName2Validator(OdString &pDeviceName, OdString canonicalMediaName, bool bValidNames);
    bool isWHSwap() const;
    bool isPaperWidthLessHeight() const;
    void unitsConverted(OdDbPlotSettings::PlotPaperUnits prevUnits, OdDbPlotSettings::PlotPaperUnits plotPaperUnits);
    OdDbPlotSettings::PlotPaperUnits getMediaNativePPU();
public:
    CPageSetupDlg(OdDbPlotSettings& params, OdSmartPtr<OdDbUserIO> pIO);
    ~CPageSetupDlg();

private:
  //{{AFX_DATA(CPageSetupDlg)
  enum { IDD = IDD_PAGE_SETUP2 };
  CComboBox         m_comboPlotStyleFiles;
  CComboBox         m_comboPlotDeviceName;
  CComboBox         m_comboPaperSize;
  CComboBox         m_comboPlotAreaType;
  CComboBox         m_comboScaleValues;
  CComboBox         m_comboQuality;
  CComboBox         m_comboShadePlot;
  CComboBox         m_comboViews;
  CComboBox         m_comboMMInches;
  int               m_nCenterThePlot;
  int               m_nDisplayPlotStyles;
  double            m_dOffsetX;
  double            m_dOffsetY;
  int               m_radioDrawingOrientation;
  int               m_nPlotUpsideDown;
  double            m_dPaperScaleUnit;
  double            m_dDrawingScaleUnit;
  int               m_nFitToPaper;
  int               m_nScaleLW;
  int               m_nPlotObjectLW;
  int               m_nPlotWithPlotStyles;
  int               m_nPlotPaperspaceLast;
  int               m_nHidePaperspaceObjects;
  int               m_nPlotTransparencies;

  short             m_DPI;
  //CString           m_PaperUnitText;
  CString           m_DrawingUnitText;
  CString           m_OffsetXText;
  CString           m_OffsetYText;

  CString           m_CanonicalPaper;

  CString           m_dPIOX;
  CString           m_dPIOY;

  CString           m_dMarginsL;
  CString           m_dMarginsR;
  CString           m_dMarginsT;
  CString           m_dMarginsB;

  CString           m_xMin, m_yMin, m_xMax, m_yMax;

  CButton	          m_add_media_btn;

  //}}AFX_DATA


	//{{AFX_VIRTUAL(CPageSetupDlg)
	protected:

	  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnSelchangeScaleValues();
    afx_msg void OnSelchangeDeviceList();
    afx_msg void OnSelChangeMediaList();
    afx_msg void OnSelChangeQualityList();
    afx_msg void OnSelChangeShadePlotList();
    afx_msg void OnSelChangeViewsList();
    afx_msg void OnSelChangeMMInchesList();
    afx_msg void OnSelChangePlotAreaType();
    afx_msg void OnCheckCenterThePlot();
    afx_msg void OnCheckDisplayPlotStyles();
    afx_msg void OnCheckFitToPaper();
    afx_msg void OnCheckScaleLW();
    afx_msg void OnChangeEditOffsetXY();
    afx_msg void OnChangeEditDPI();
    afx_msg void OnChangeEditScaleUnit();
    afx_msg void OnClickPortraitLandscape();
    afx_msg void OnClickPlotStyles();
  	afx_msg void OnClickPlotStyleFilesBtn();
	  afx_msg void OnSelChangePlotStyleFiles();
    afx_msg void OnClickWindowButton();
    afx_msg void OnClickAddMediaButton();
    afx_msg void OnCheckPlotTransparencies();

	//}}AFX_VIRTUAL

// Implementation
protected:

	//{{AFX_MSG(CPageSetupDlg)
		// No message handlers
//    afx_msg void On???();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  bool FillDeviceCombo();
  bool FillPaperSizes();
  void FillShadePlotQualityDPI(bool bFillCombo);
  void FillScaleValues(bool bFillCombo);
  void FillPlotAreaCombo(bool bFillCombo);
  void FillPlotOffset();
  void FillPaperOrientation();
  void FillPlotStyles();
  void FillPlotStyleCombo(bool bFillCombo);
  void FillViewCombo(bool bFillCombo);
  void FillMMInches(bool bFillCombo);
  bool FillArrayByPatternFile(OdArray<CString> &arrFiles, const CString sPattern);
  void FillWindowArea();

  bool viewsExists( ) const;
  bool isModelSpacePageSetup() const;
};

#endif // PAGE_SETUP_DLG_DEFINED
