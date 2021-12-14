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
#if !defined(AFX_BGCOLORTOOL_H__8CD884D4_A935_478F_9494_EE0265C186AE__INCLUDED_)
#define AFX_BGCOLORTOOL_H__8CD884D4_A935_478F_9494_EE0265C186AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CBgColor dialog


#include "stdafx.h"
#include "ColorListCtrl.h"
#include "OdPlatform.h"
#include "RxDLinkerReactor.h"

class CBgDialog : public CDialog
{

private:
	OdDgColorTablePtr m_pColorTable;
  OdDgDatabasePtr   m_pDb;
	OdArray<ODCOLORREF> m_colorPaletteArray;
	const ODCOLORREF* m_pColorPalette;
	CColorListCtrl m_listCtrl;

public:
	CBgDialog(OdDgColorTablePtr pColorTable, const OdDgDatabasePtr& pDb, CWnd* pParent = NULL);
	OdArray<ODCOLORREF> GetColorPaletteArray();
	enum { IDD = IDD_COLOR_BUTTONS };

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedLoadColors();
  afx_msg void OnBnClickedSaveColors();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	virtual BOOL OnInitDialog();
	const ODCOLORREF * GetDefaultPalette();
	void InitColorListCtrl(const ODCOLORREF *);
	const ODCOLORREF* GetColorPaletteFromTable();
	void SetPaletteColor(int colorNum, ODCOLORREF color);
	void SetOdArrayPalette(const ODCOLORREF*);
};

#endif
