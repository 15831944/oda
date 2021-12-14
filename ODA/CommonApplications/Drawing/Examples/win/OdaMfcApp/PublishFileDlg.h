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
// PublishFileDlg.h : header file
//


#ifndef PUBLISH_FILEDLG_DEFINED
#define PUBLISH_FILEDLG_DEFINED

#include "DwfExport.h"
using namespace TD_DWF_EXPORT;


/////////////////////////////////////////////////////////////////////////////
class CPublishSheets : public CDialog
{
public:
    CPublishSheets(DwExportParams* pParams);
    ~CPublishSheets();

	//{{AFX_DATA(CPublishSheets)
	enum { IDD = IDD_PUBLISH_SHEETS };
  enum 
  { 
      enVarious = -1
    , enBinary = 0
    , enAscii
  }                 m_radioFormat;

	CListCtrl	        m_listSheets;
  CString	          m_sFileName;
  CString	          m_sPassword;
	//}}AFX_DATA

  DwExportParams*       m_pParams;
  OdArray<DwfPageData>  m_arPageData;

	//{{AFX_VIRTUAL(CPublishSheets)
	protected:
	  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
	  afx_msg void OnBrowse();    
	  afx_msg void OnClickedLayoutsList(NMHDR* pNMHDR, LRESULT* pResult);
    //afx_msg void OnDblClickedLayoutsList(NMHDR* pNMHDR, LRESULT* pResult);
	  afx_msg void OnRemove();    
	  afx_msg void OnSetting();
	//}}AFX_VIRTUAL

// Implementation
protected:

	//{{AFX_MSG(CPublishSheets)
		// No message handlers
//    afx_msg void On???();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void InitPagaDate();
  void FillListSheets();
  void FillItem(UINT i);

};



class CPublishSetting : public CDialog
{
  DwfPageData&  m_pageData;

  CString       m_sLayout;
  CString       m_sSheet;
  CString       m_sDrawing;
  CString       m_sSubject;
  CString       m_sCompany;
  CString       m_sComments;
  CString       m_sAuthor;
  CString       m_sPreviewFile;
  int           m_nWidth;
  int           m_nHeight;
  int           m_nColorDepth;

public:
    CPublishSetting(DwfPageData& pageData, CString& sDrawing);
    ~CPublishSetting();

	//{{AFX_DATA(CPublishSetting)
	enum { IDD = IDD_PUBLISH_SETTING };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CPublishSetting)
	protected:
	  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //virtual void OnCancel();
	  afx_msg void OnBrowsePreview();    
	//}}AFX_VIRTUAL

// Implementation
protected:

	//{{AFX_MSG(CPublishSetting)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // PUBLISH_FILEDLG_DEFINED
