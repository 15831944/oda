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

#if !defined(AFX_ODALAYERPROPMANAGERDLG_H__8B71789E_7314_46D4_BA2F_EEFE39F6AB0B__INCLUDED_)
#define AFX_ODALAYERPROPMANAGERDLG_H__8B71789E_7314_46D4_BA2F_EEFE39F6AB0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OdaLayerPropManagerDlg.h : header file
//

#include "LyLayerFilter.h"

class OdDbLayerTableRecord;

/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropManagerDlg dialog
enum ColumnLabels
{
  column_status,
  column_name,
  column_on,
  column_freeze,
  column_lock,
  column_color,
  column_linetype,
  column_lineweight,
  column_transparency,
  column_plotstyle,
  column_plot,
  column_vpfreeze,
  column_vpcolor,
  column_vplinetype,
  column_vplineweight,
  column_vptransparency,
  column_vpplotstyle,
  column_descr
};

class OdaLayerPropManagerDlg : public CDialog
{
  int m_nInitWidth;
  int m_nInitHeight;

  int m_nDeltaWidth;
  int m_nDeltaHeight;

  int column_desc;
  int columns_count;

  OdDbDatabase *m_pDb;
  OdDbObjectId  m_vp;
  bool m_bClickToColumnName;
  OdLyLayerFilterPtr m_pRootFilter;
  CImageList m_pTreeImages;
  CImageList m_stateImages;
  
  void UpdateFiltersTree();
  void UpdateCurrentLayerInfoField();
  void OnClickSubItemLayersList(int itemIndex, int labelsIndex);
  void DrawItem(CDC& dc, int itemID, int labelIndex, const RECT& rcItem);
  bool OpenSelectedLayer(OdSmartPtr<OdDbLayerTableRecord>& pLayer);

// Construction
public:
	OdaLayerPropManagerDlg(CWnd* pParent = NULL);   // standard constructor
  OdaLayerPropManagerDlg(OdDbDatabase* pDb, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(OdaLayerPropManagerDlg)
	enum { IDD = IDD_LAYER_PROPERTIES_MANAGER };
	CTreeCtrl	m_TreeFilters;
	CListCtrl	m_listLayersList;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OdaLayerPropManagerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OdaLayerPropManagerDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnButtonSetCurrent();
	afx_msg void OnClickListLayersList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnDblclkListLayersList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkLayerFilterTree(NMHDR* pNMHDR, LRESULT* pResult);
  afx_msg void OnClickLayerFilterTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonAdd();
  afx_msg void OnButtonLayerStates();
	afx_msg void OnRclickListLayersList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClearAllViewportsOverrides();
	afx_msg void OnClearAllViewportOverridesForCurrVp();
	afx_msg void OnRemoveViewportColorOverrideForCurrVp();
	afx_msg void OnRemoveViewportLinetypeOverrideForCurrVp();
	afx_msg void OnRemoveViewportLineweightOverrideForCurrVp();
	afx_msg void OnRemoveViewportPlotstyleOverrideForCurrVp();
	afx_msg void OnRemoveViewportTransparencyOverrideForCurrVp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnTvnKeydownLayerFilterTree(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropertiesLineWeightDlg dialog

class OdaLayerPropertiesLineWeightDlg : public CDialog
{
  int m_nOrigLineWeight;
// Construction
public:
	OdaLayerPropertiesLineWeightDlg(CWnd* pParent = NULL);   // standard constructor
  OdaLayerPropertiesLineWeightDlg(int nOrigLineWeight, CWnd* pParent = NULL);
  
// Dialog Data
	//{{AFX_DATA(OdaLayerPropertiesLineWeightDlg)
	enum { IDD = IDD_LAYER_PROPERTIES_LINEWEIGHT };
	CListBox	m_listLineweight;
	//}}AFX_DATA
  OdDb::LineWeight m_nLineWeight;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OdaLayerPropertiesLineWeightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OdaLayerPropertiesLineWeightDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkListLineweight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropLineTypeDlg dialog

class OdaLayerPropLineTypeDlg : public CDialog
{
// Construction
  OdDbDatabase *m_pDb;
  bool m_bHideByLayerByBlock;
public:
	OdaLayerPropLineTypeDlg(CWnd* pParent = NULL);   // standard constructor
  OdaLayerPropLineTypeDlg(OdDbDatabase *pDb, const OdDbObjectId& originalId,
    bool bHideByLayerByBlock = false, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(OdaLayerPropLineTypeDlg)
	enum { IDD = IDD_LAYER_PROPERTIES_LINETYPE };
	CListCtrl	m_listLinetype;
	//}}AFX_DATA

  OdDbObjectId  m_originalId;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OdaLayerPropLineTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog);

	// Generated message map functions
	//{{AFX_MSG(OdaLayerPropLineTypeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonLoadLinetype();
	afx_msg void OnButtonSaveLinetype();
	//}}AFX_MSG

  void FillLinetypesList();
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropColorDlg dialog

class OdaLayerPropColorDlg : public CDialog
{
// Construction
public:
	OdaLayerPropColorDlg(CWnd* pParent = NULL);   // standard constructor
  OdaLayerPropColorDlg(int nColor, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(OdaLayerPropColorDlg)
	enum { IDD = IDD_LAYER_PROPERTIES_COLOR };
	CListBox	m_listColors;
	//}}AFX_DATA
  int m_nColor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OdaLayerPropColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OdaLayerPropColorDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropTransparencyDlg dialog

class OdaLayerPropTransparencyDlg : public CDialog
{
// Construction
public:
	OdaLayerPropTransparencyDlg(CWnd* pParent = NULL);   // standard constructor
  OdaLayerPropTransparencyDlg(int nAlpha, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(OdaLayerPropTransparencyDlg)
	enum { IDD = IDD_LAYER_PROPERTIES_COLOR };
	CListBox	m_listTransps;
	//}}AFX_DATA
  int m_nAlpha;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OdaLayerPropTransparencyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OdaLayerPropTransparencyDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// OdaLayerFilterPropDlg dialog
//
class OdaLayerFilterPropDlg : public CDialog
{
// Construction
  OdLyLayerFilterPtr m_pFilter; 
public:
  OdaLayerFilterPropDlg( OdLyLayerFilter* lf, CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(OdaLayerFilterPropDlg)
	enum { IDD = IDD_LAYER_PROPERTIES_FILTER };
	CString	m_sName;
  CString m_sDef;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OdaLayerFilterPropDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OdaLayerFilterPropDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODALAYERPROPMANAGERDLG_H__8B71789E_7314_46D4_BA2F_EEFE39F6AB0B__INCLUDED_)
