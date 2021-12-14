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
// CustomPDFPropertiesDialog.h : header file
//
#include "resource.h"

#include "PdfExportParams.h"


#ifndef CUSTOM_PDF_DLG_DEFINED
#define CUSTOM_PDF_DLG_DEFINED

struct AuxPdfProps 
{
  unsigned int* piHatchDPI;
  unsigned int* piGeomDPI;
  unsigned int* piColorImagesDPI;
  unsigned int* piBWImagesDPI;
  unsigned int* piDCTQuality;
  unsigned int* piPdfAMode;
  unsigned int* piShadedVpMode;
  BOOL* pbMergeLines;
  BOOL* pbCropImages;
  BOOL* pbDCTCompression;
  BOOL* pbMeasuring;
  BOOL* pbForceDisableGsDevice;
  BOOL* pbTranspBgForShadedViewports;
  BOOL* pbSearchTextOnShadedVp;
  BOOL* pbUpscaleImages;
  BOOL* pbUseGsCache;
  BOOL* pbUsePdfBlocks;
  TD_PDF_2D_EXPORT::Watermark* watermark;

  AuxPdfProps()
  {
    piHatchDPI = 0;
    piGeomDPI = 0;
    piColorImagesDPI = 0;
    piBWImagesDPI = 0;
    piDCTQuality = 0;
    piPdfAMode = 0;
    piShadedVpMode = 0;
    pbMergeLines = 0;
    pbCropImages = 0;
    pbDCTCompression = 0;
    pbMeasuring = 0;
    pbForceDisableGsDevice = 0;
    pbTranspBgForShadedViewports = 0;
    pbSearchTextOnShadedVp = 0;
    pbUpscaleImages = 0;
    pbUseGsCache = 0;
    pbUsePdfBlocks = 0;
    watermark = 0;
  }
};

class CCustomPDFPropertiesDialog : public CDialog
{
  unsigned int* m_piHatchDPI;
  unsigned int* m_piGeomDPI;
  unsigned int* m_piBWImagesDPI;
  unsigned int* m_piColorImagesDPI;
  unsigned int* m_piDCTQuality;
  unsigned int* m_piPdfAMode;
  unsigned int* m_piShadedVpMode;
 
  BOOL*         m_bMergeLinesCrossing;
  BOOL*         m_bCropImages;
  BOOL*         m_bDCTCompression;
  BOOL*         m_bMeasuring;
  BOOL*         m_bForceDisableGsDevice;
  BOOL*         m_bTranspBgForShadedViewports;
  BOOL*         m_bSearchTextOnShadedVp;
  BOOL*         m_bUpscaleImages;
  BOOL*         m_bUseGsCache;
  BOOL*         m_bUsePdfBlocks;
  BOOL          m_bFitWm2Page;

  CString       m_wmText;
  CComboBox     m_comboMergeControl;
  CComboBox     m_comboPDFAmode;
  CComboBox     m_comboWMPosition;
  CComboBox     m_comboWMFont;
  CComboBox     m_comboWMColor;
  CComboBox     m_comboShadedVpMode;
  unsigned int  m_iWMOpacity;
  unsigned int  m_iWMFomtSize;

  TD_PDF_2D_EXPORT::Watermark*    m_Watermark;

  afx_msg void OnCheckDCTCompression();
  afx_msg void OnCheckGsCache();

public:
  CCustomPDFPropertiesDialog(AuxPdfProps& props, CWnd* pParent = NULL); //constructor
	virtual ~CCustomPDFPropertiesDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_CUSTOM_PROPERTIES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  afx_msg void OnOk();

	DECLARE_MESSAGE_MAP()
};

#endif //CUSTOM_PDF_DLG_DEFINED
