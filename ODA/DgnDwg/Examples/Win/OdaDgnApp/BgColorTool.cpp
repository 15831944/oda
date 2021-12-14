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
#include "stdafx.h"
#include "OdaDgnApp.h"
#include "BgColorTool.h"
#include "RxModule.h"
#include "DynamicLinker.h"

#include "RxDLinkerReactor.h"

#ifndef _TOOLKIT_IN_DLL_
#include "StaticAppSelDlg.h"
#endif


CBgDialog::CBgDialog(OdDgColorTablePtr pColorTable, const OdDgDatabasePtr& pDb, CWnd * pParent)
	: CDialog(CBgDialog::IDD, pParent)
{
	m_pColorTable = pColorTable;
  m_pDb = pDb;
}

OdArray<ODCOLORREF> CBgDialog::GetColorPaletteArray()
{
	return m_colorPaletteArray;
}

void CBgDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST2, m_listCtrl);
}

/////////////////////////////////////////////////////////////////////////////
// CBgColor message handlers

BOOL CBgDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!m_pColorTable.isNull())
	{
		m_pColorPalette = GetColorPaletteFromTable();
	}
	else
	{
		m_pColorPalette = GetDefaultPalette();
	}
	InitColorListCtrl(m_pColorPalette);
	SetOdArrayPalette(m_pColorPalette);

	return TRUE;
}

const ODCOLORREF * CBgDialog::GetDefaultPalette()
{
  return OdDgColorTable::currentPalette(m_pDb);
}

void CBgDialog::InitColorListCtrl(const ODCOLORREF *pColorPalette)
{
  m_listCtrl.DeleteAllItems();

	RECT ctrlRect;
	m_listCtrl.GetClientRect(&ctrlRect);
	int iColumnWidth = ctrlRect.right - ctrlRect.left - GetSystemMetrics(SM_CXVSCROLL);
	m_listCtrl.InsertColumn(0, _T("Color Table"), LVCFMT_LEFT, iColumnWidth);
	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

		//Color list filling
		OdString itemStr;
		OdString itemNum;
		char s[50];
		for (int i = 0; i < 255; i++)
		{
			sprintf(s, "%d", i);
			itemNum = s;
			itemStr = "Color# " + itemNum;
			m_listCtrl.InsertItem(i, itemStr);
			m_listCtrl.SetRowColor(pColorPalette[i], i);
		}
		m_listCtrl.InsertItem(255, L"Background Color");
		m_listCtrl.SetRowColor(pColorPalette[255], 255);
	
}

const ODCOLORREF*  CBgDialog::GetColorPaletteFromTable()
{
	return m_pColorTable->palette();
}

 void CBgDialog::SetPaletteColor(int colorNum, ODCOLORREF color)
 {
	 m_colorPaletteArray.setAt(colorNum, color);
 }

void CBgDialog::SetOdArrayPalette(const ODCOLORREF * pColorRef)
{
  m_colorPaletteArray.clear();

	for (int i = 0; i < 256; i++)
	{
		m_colorPaletteArray.append(pColorRef[i]);
	}
}

BEGIN_MESSAGE_MAP(CBgDialog, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CBgDialog::OnNMDblclkList2)
	ON_BN_CLICKED(IDC_CANCEL, &CBgDialog::OnBnClickedCancel)
  ON_BN_CLICKED(ID_LOAD_COLORS, &CBgDialog::OnBnClickedLoadColors)
  ON_BN_CLICKED(ID_SAVE_COLORS, &CBgDialog::OnBnClickedSaveColors)
END_MESSAGE_MAP()

void CBgDialog::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int itemNum = pNMItemActivate->iItem;

	if (itemNum != -1)
	{
		CColorDialog colorDlg;

		if (colorDlg.DoModal() == IDOK)
		{
			COLORREF color = colorDlg.GetColor();
			SetPaletteColor(itemNum, color);
			m_listCtrl.SetRowColor(color, itemNum);
		}
	}
	*pResult = 0;
}

void CBgDialog::OnBnClickedCancel()
{
	EndDialog(0);
}

void CBgDialog::OnBnClickedLoadColors()
{
  CFileDialog loadFile(true, _T("Load from tbl file"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Color files (*.tbl)|*.tbl|All Files (*.*)|*.*||"), this);

  if (loadFile.DoModal() == IDOK)
  {
    OdDgColorTablePtr pColorTable = m_pDb->getColorTable(OdDg::kForWrite);

    if( pColorTable->loadFromFile((LPCTSTR)loadFile.GetPathName()) )
    {
      m_pColorTable = pColorTable;
      m_pColorPalette = GetColorPaletteFromTable();
      InitColorListCtrl(m_pColorPalette);
      SetOdArrayPalette(m_pColorPalette);
    }
  }
}

void CBgDialog::OnBnClickedSaveColors()
{
  CFileDialog saveFile(false, _T("Save to tbl file"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Color files (*.tbl)|*.tbl||"), this);

  if (saveFile.DoModal() == IDOK)
  {
    OdDgColorTablePtr pColorTable = m_pDb->getColorTable(OdDg::kForWrite);
    pColorTable->saveToFile((LPCTSTR)saveFile.GetPathName());
  }
}
