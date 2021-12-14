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


#ifndef CUSTOM_PDF_DLG_DEFINED
#define CUSTOM_PDF_DLG_DEFINED

class CCustomPDFPropertiesDialog : public CDialog
{
  unsigned int* m_piHatchDPI;
  unsigned int* m_piGeomDPI;
  unsigned int* m_piBWImagesDPI;
  unsigned int* m_piColorImagesDPI;
  unsigned int* m_piDCTQuality;
  unsigned int* m_piPdfAMode;
  BOOL*         m_bMergeLinesCrossing;
  BOOL*         m_bCropImages;
  BOOL*         m_bDCTCompression;
  BOOL*         m_bMeasuring;
  BOOL*         m_bUseGsCache;

  CComboBox     m_comboMergeControl;
  CComboBox     m_comboPDFAmode;

  afx_msg void OnCheckDCTCompression();

public:
  CCustomPDFPropertiesDialog(unsigned int* piHatchDPI, unsigned int* piGeomDPI, unsigned int* piColorImagesDPI, unsigned int* piBWImagesDPI, unsigned int* piDCTQuality, unsigned int* piPdfAMode, 
                              BOOL* pbMergeLines, BOOL* pbCropImages, BOOL* pbDCTCompression, BOOL* pbMeasuring, BOOL* pbUseGsCache, CWnd* pParent = NULL); //constructor
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
