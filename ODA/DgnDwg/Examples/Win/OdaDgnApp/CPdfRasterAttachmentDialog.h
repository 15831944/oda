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


#if !defined(AFX_PDF_ATTACH_DIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
#define AFX_PDF_ATTACH_DIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxwin.h"
#include "resource.h"

#include "DgDatabase.h"
#include "RxPdfToRasterServices.h"


// CPdfRasterAttachmentDialog dialog

class CPdfRasterAttachmentDialog : public CDialog
{
	//DECLARE_DYNAMIC(CPdfRasterAttachmentDialog)

public:
	CPdfRasterAttachmentDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPdfRasterAttachmentDialog();

  void setData(CString path, CString fName, OdDgDatabasePtr pDb);

// Dialog Data
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PDF_ATTACH_DIALOG };
//#endif

private:
  OdDgDatabasePtr     m_pDb;
  CString             m_Path;
  CString             m_FileName;
  CListBox            m_SheetList;
  CListCtrl           m_LayerList;
  CImageList          m_stateImages;
  CStatic             m_Preview;
  OdPdfLayerArray     m_layers;

  OdPdf2ImageConverterPtr m_pConverter;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  virtual BOOL OnInitDialog();
  afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  afx_msg void OnSelchangeSheetList();
  afx_msg void OnClickListLayersList(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnInsert();

  void DrawItem(CDC& dc, int itemID, int labelIndex, const RECT& rcItem);
  void FillSheetList();
  void FillLayerList();
  void updatePreview();
  bool hittestSubitem(CListCtrl& listLayersList, int columns_count, const CPoint& pt, int &index, int &labelIndex);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PDF_ATTACH_DIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
