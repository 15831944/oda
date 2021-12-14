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

#if !defined(AFX_DWFUNDERLAYDIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
#define AFX_DWFUNDERLAYDIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DwfUnderlayDialog.h : header file
//

#include "DbUnderlayHost.h"
#include "DbUnderlayDefinition.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CDwfUnderlayDialog dialog

class CDwfUnderlayDialog : public CDialog
{
public:
  enum UnderlayType { eDgn = 0, eDwf, ePdf };

private:
  OdDbUnderlayFilePtr m_pFile;
  OdArray<bool>       m_Visibility;
  OdDbDatabase        *m_pDb;
  UnderlayType        m_Type;
  CString             m_Path;
  CString             m_FileName;
  CListBox            m_SheetList;
  CListCtrl           m_LayerList;
  CImageList          m_stateImages;
  CStatic             m_Preview;
  int                 m_PathType;
  OdString            m_RelativePath;

  int                 m_DialogHeight;//Initial height

  OdDbUnderlayDefinitionPtr getDef() const;
// Construction

public:

	CDwfUnderlayDialog(OdDbDatabase *pDb, CWnd* pParent, UnderlayType type);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDwfUnderlayDialog)
	enum { IDD = IDD_DwfUnderlay };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDwfUnderlayDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  afx_msg void OnCheckLayers();
  virtual BOOL OnInitDialog();
  afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  afx_msg void OnSelchangeSheetList();
  afx_msg void OnClickListLayersList(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnClickPathType();
  //}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDwfUnderlayDialog)
	afx_msg void OnBrowse();
	afx_msg void OnInsert();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  void updatePreview();
  bool InitDwfProperties();
  void OnError(CString str);
  void OnError(OdResult res);
  void FillSheetList();
  void FillLayerList();
  void DrawItem(CDC& dc, int itemID, int labelIndex, const RECT& rcItem);
  bool hittestSubitem(CListCtrl& listLayersList, int columns_count, const CPoint& pt, int &index, int &labelIndex) ;

public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DWFUNDERLAYDIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
