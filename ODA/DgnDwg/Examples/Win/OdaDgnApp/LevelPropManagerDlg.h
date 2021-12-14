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

#if !defined(AFX_LEVELPROPMANAGERDLG_H__INCLUDED_)
#define AFX_LEVELPROPMANAGERDLG_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ColumnLabels
{
	column_status,
	column_name,
	column_color,
	column_linestyle,
	column_lineweight,
	column_used,
	column_descr
};

/////////////////////////////////////////////////////////////////////////////
// CLevelPropManagerDlg dialog
class CLevelPropManagerDlg :	public CDialog
{
	int m_nInitWidth;
	int m_nInitHeight;

	int m_nDeltaWidth;
	int m_nDeltaHeight;

	int column_desc;
	int columns_count;

	OdDgDatabase *m_pDb;
	bool m_bClickToColumnName;

	void UpdateFiltersTree();
	void UpdateActiveLevelInfoField();
	void OnClickSubItemLevelsList(int itemIndex, int labelsIndex);
	void DrawItem(CDC& dc, int itemID, int labelIndex, const RECT& rcItem);
	bool OpenSelectedLevel(OdSmartPtr<OdDgLevelTableRecord>& pLevel);

public:
	CLevelPropManagerDlg(CWnd* pParent = NULL);
	CLevelPropManagerDlg(OdDgDatabase* pDb, CWnd* pParent = NULL);   // standard constructor
	virtual ~CLevelPropManagerDlg();

	enum { IDD = IDD_LEVEL_PROP_MANAGER };
	
	CTreeCtrl	m_TreeFilters;
	CListCtrl	m_listLevelsList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	// Implementation
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void OnCancel();
	afx_msg void OnButtonSetActive();
	afx_msg void OnButtonNew();
	afx_msg void OnClickListLevelsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListLevelsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickListLevelsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickLevelFilterTree(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// LevelPropColorDlg dialog
class CLevelPropColorDlg : public CDialog
{
	// Construction
public:
	CLevelPropColorDlg(CWnd* pParent = NULL);   // standard constructor
	CLevelPropColorDlg(int nColor, CWnd* pParent = NULL);

	enum { IDD = IDD_LEVEL_PROPERTIES_COLOR };
	CListBox	m_listColors;

	int m_nColor;

	// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CLevelPropLineStyleDlg dialog
class CLevelPropLineStyleDlg : public CDialog
{
	OdDgDatabase *m_pDb;
	
	// Construction
public:
	CLevelPropLineStyleDlg(CWnd* pParent = NULL);   // standard constructor
	CLevelPropLineStyleDlg(OdDgDatabase *pDb, const OdUInt32& originalId, CWnd* pParent = NULL);

	enum { IDD = IDD_LEVEL_PROPERTIES_LINESTYLE };
	CListCtrl	m_listLineStyle;

	OdUInt32  m_originalId;

	// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
	void FillLineStylesList();
};


/////////////////////////////////////////////////////////////////////////////
// CLevelPropLineWeightDlg dialog
class CLevelPropLineWeightDlg : public CDialog
{
	int m_nOrigLineWeight;
	// Construction
public:
	CLevelPropLineWeightDlg(CWnd* pParent = NULL);   // standard constructor
	CLevelPropLineWeightDlg(int nOrigLineWeight, CWnd* pParent = NULL);

	enum { IDD = IDD_LEVEL_PROPERTIES_LINEWEIGHT };
	CListBox	m_listLineweight;
	OdUInt32 m_nLineWeight;

	// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_LEVELPROPMANAGERDLG_H__INCLUDED_)

