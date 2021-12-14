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
#ifndef _PageSetupDlg_h_Included_
#define _PageSetupDlg_h_Included_

#include "DgDatabase.h"
#include "OdaDgnAppDoc.h"

// CCameraView dialog

class CPageSetupDlg : public CDialog
{
	DECLARE_DYNAMIC(CPageSetupDlg)

public:
	CPageSetupDlg(CWnd* pParent, 
                const COdaDgnAppPrintSettings& printSettings,
                OdDgElementId idModel);   // standard constructor
	virtual ~CPageSetupDlg();

  COdaDgnAppPrintSettings getPrinterSettings() const;

// Dialog Data
	enum { IDD = IDD_PAGE_SETUP_DLG };

private:

  struct Paper
  {
    const OdChar* _localeName;
    const OdChar* _canonicalName;
    double _w;
    double _h;
    bool   _inMM;

    double _xminMargin;
    double _yminMargin;
    double _xmaxMargin;
    double _ymaxMargin;
  };

private:

  void initCtrls();
  void initDataByPrinterSettings();
  void updatePrinterSettings();
  void updateCtrls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  virtual void OnCancel();

  afx_msg void OnSelchangePrinterName();
  afx_msg void OnSelchangePaperSize();
  afx_msg void OnSelchangePlotType();

	DECLARE_MESSAGE_MAP()

private:

  COdaDgnAppPrintSettings m_printSettings;
  OdDgElementId           m_idModel;
  OdArray<CString>        m_strPrinterNameArr;
  OdArray<CString>        m_strPrinterLogNameArr;
  OdArray<CString>        m_strPrinterWhereArr;
  OdArray<CString>        m_strPrinterDescriptionArr;
  OdArray<OdUInt32>       m_viewComboNums;
  bool                    m_bViewsPresent;

  int                     m_iNameIndex;
  int                     m_iPaperSize;
  int                     m_iRenderMode;
  int                     m_iPlotArea;
  int                     m_iRotation;
  CString                 m_strPlotterLocal;
  CString                 m_strPlotterWhere;
  CString                 m_strPlotterDescription;
  double                  m_dPageWidth;
  double                  m_dPageHeight;
  double                  m_dLeftMargin;
  double                  m_dRightMargin;
  double                  m_dTopMargin;
  double                  m_dBottomMargin;
  double                  m_dOffsetX;
  double                  m_dOffsetY;
  double                  m_dScale;

  static Paper            m_arrPapers[];
  static OdUInt32         m_numPapers;
public:
  afx_msg void OnBnClickedOk();
};

#endif // _PageSetupDlg_h_Included_
