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
#include "LevelPropManagerDlg.h"
#include "DgTable.h"
#include "DgLevelTableRecord.h"
#include "DgLevelFilterTableRecord.h"

CString StringByLineWeight(int nLineWeight, bool bLineWeightByIndex = true)
{
	CString strLineWeight = _T("");
	switch (nLineWeight)
	{
	case OdDg::kLineWeightByCell:
		strLineWeight = _T("Cell");
		break;
	case OdDg::kLineWeightByLevel:
		strLineWeight = _T("Level");
		break;
	default:
		strLineWeight.Format(_T("%d"), nLineWeight);
	}

	return strLineWeight;
}

#define DBL_NEG(x) (1. - (x))

#define DWORDPTR_TO_PTR( x ) OdIntToPtr(x)
#if (_MSC_VER >= 1300)
#define PTR_TO_DWORDPTR( x ) (DWORD_PTR)OdIntToGsMarker(x)
#else
#define PTR_TO_DWORDPTR( x ) (DWORD)OdIntToGsMarker(x)
#endif

CLevelPropManagerDlg::CLevelPropManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLevelPropManagerDlg::IDD, pParent)
{
}

CLevelPropManagerDlg::CLevelPropManagerDlg(OdDgDatabase * pDb, CWnd * pParent)
	: CDialog(CLevelPropManagerDlg::IDD, pParent)
	, m_pDb(pDb)
{
}

CLevelPropManagerDlg::~CLevelPropManagerDlg()
{
}

void CLevelPropManagerDlg::DoDataExchange(CDataExchange * pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LEVEL_FILTER_TREE, m_TreeFilters);
	DDX_Control(pDX, IDC_LIST_LEVELS, m_listLevelsList);
}

BEGIN_MESSAGE_MAP(CLevelPropManagerDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_CREATE()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_SET_ACTIVE, OnButtonSetActive)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LEVELS, OnClickListLevelsList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LEVELS, OnDblclkListLevelsList)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LEVELS, OnRclickListLevelsList)
	ON_NOTIFY(NM_CLICK, IDC_LEVEL_FILTER_TREE, OnClickLevelFilterTree)
END_MESSAGE_MAP()

int CLevelPropManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_nInitWidth = lpCreateStruct->cx;
	m_nInitHeight = lpCreateStruct->cy;
	return 0;
}

BOOL CLevelPropManagerDlg::OnInitDialog()
{
	m_pDb->startUndoRecord();

	CDialog::OnInitDialog();
	m_listLevelsList.DeleteAllItems();
	m_listLevelsList.InsertColumn(column_status, _T("Status"), LVCFMT_LEFT, 50);
	m_listLevelsList.InsertColumn(column_name, _T("Name"), LVCFMT_LEFT, 100);
	m_listLevelsList.InsertColumn(column_color, _T("Color"), LVCFMT_LEFT, 80);
	m_listLevelsList.InsertColumn(column_linestyle, _T("Style"), LVCFMT_LEFT, 50);
	m_listLevelsList.InsertColumn(column_lineweight, _T("Weight"), LVCFMT_LEFT, 50);
	columns_count = m_listLevelsList.InsertColumn(column_used, _T("Used"), LVCFMT_LEFT, 40);
	column_desc = m_listLevelsList.InsertColumn(++columns_count, _T("Description"), LVCFMT_LEFT, 100);
	columns_count++;

	OdDgModelPtr pModel = OdDgModel::createObject();
	pModel = m_pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
	OdDgLevelTablePtr pLevels = pModel->database()->getLevelTable(OdDg::kForWrite);
	pLevels->generateUsageData();

	OdDgElementIteratorPtr pIter = pLevels->createIterator();
	int i = 0;
	for (pIter->start(); !pIter->done(); pIter->step(), ++i)
	{
		m_listLevelsList.InsertItem(i, NULL);
		m_listLevelsList.SetItemData(i, PTR_TO_DWORDPTR((OdDbStub*)pIter->item()));
	}

	CString str;
	str.Format(_T("%d levels total"), m_listLevelsList.GetItemCount());
	GetDlgItem(IDC_STATIC_LEVEL_STATISTIC)->SetWindowText(str);

	UpdateActiveLevelInfoField();
	UpdateFiltersTree();

	return TRUE;
}

// Fill filters tree
//
void CLevelPropManagerDlg::UpdateFiltersTree()
{
	m_TreeFilters.DeleteAllItems();

	OdDgLevelFilterTablePtr filterTable = m_pDb->getLevelFilterTable(OdDg::kForWrite);

	OdDgElementIteratorPtr pIter = filterTable->createIterator();
	OdDgLevelFilterTableRecordPtr filter;
	int i = 0;
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		OdDgElementId id = pIter->item();
		filter = id.safeOpenObject(OdDg::kForWrite);
		HTREEITEM treeItem =
			m_TreeFilters.InsertItem(filter->getName(), TVI_ROOT, TVI_LAST);
		m_TreeFilters.SetItemData(treeItem, PTR_TO_DWORDPTR((OdDbStub*)id));
		m_TreeFilters.SetItemText(treeItem, filter->getName());
		++i;
	}
}

void CLevelPropManagerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect itemRect;
	CRect dlgRect;
	if (m_listLevelsList.m_hWnd)
	{
		m_listLevelsList.GetWindowRect(&itemRect);
		ScreenToClient(itemRect);
		GetWindowRect(&dlgRect);
		itemRect.right += (dlgRect.Width() - m_nDeltaWidth);
		itemRect.bottom += (dlgRect.Height() - m_nDeltaHeight);
		m_listLevelsList.MoveWindow(itemRect);
	}

	if (GetDlgItem(IDC_STATIC_ACTIVE_LEVEL))
	{
		GetDlgItem(IDC_STATIC_ACTIVE_LEVEL)->GetWindowRect(&itemRect);
		ScreenToClient(itemRect);
		GetWindowRect(&dlgRect);
		itemRect.right += (dlgRect.Width() - m_nDeltaWidth);
		GetDlgItem(IDC_STATIC_ACTIVE_LEVEL)->MoveWindow(itemRect);
	}

	if (GetDlgItem(IDC_STATIC_LEVEL_STATISTIC))
	{
		GetDlgItem(IDC_STATIC_LEVEL_STATISTIC)->GetWindowRect(&itemRect);
		ScreenToClient(itemRect);
		GetWindowRect(&dlgRect);
		GetDlgItem(IDC_STATIC_LEVEL_STATISTIC)->MoveWindow(itemRect.left
			, itemRect.top + (dlgRect.Height() - m_nDeltaHeight)
			, itemRect.Width() + (dlgRect.Width() - m_nDeltaWidth)
			, itemRect.Height());
	}


	if (GetDlgItem(IDCANCEL))
	{
		GetDlgItem(IDCANCEL)->GetWindowRect(&itemRect);
		ScreenToClient(itemRect);
		GetWindowRect(&dlgRect);
		GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDCANCEL)->MoveWindow(itemRect.left + (dlgRect.Width() - m_nDeltaWidth)
			, itemRect.top + (dlgRect.Height() - m_nDeltaHeight)
			, itemRect.Width(), itemRect.Height());
		GetDlgItem(IDCANCEL)->ShowWindow(SW_SHOW);

	}
	if (GetDlgItem(IDOK))
	{
		GetDlgItem(IDOK)->GetWindowRect(&itemRect);
		ScreenToClient(itemRect);
		GetWindowRect(&dlgRect);
		GetDlgItem(IDOK)->ShowWindow(SW_HIDE);
		GetDlgItem(IDOK)->MoveWindow(itemRect.left + (dlgRect.Width() - m_nDeltaWidth)
			, itemRect.top + (dlgRect.Height() - m_nDeltaHeight)
			, itemRect.Width(), itemRect.Height());
		GetDlgItem(IDOK)->ShowWindow(SW_SHOW);
	}

}

void CLevelPropManagerDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CRect rct;
	rct = *pRect;
	CRect dlgRect;
	GetWindowRect(&dlgRect);
	m_nDeltaWidth = dlgRect.Width();
	m_nDeltaHeight = dlgRect.Height();
	if (rct.Width() < m_nInitWidth)
	{
		switch (fwSide)
		{
		case WMSZ_LEFT:
		case WMSZ_BOTTOMLEFT:
		case WMSZ_TOPLEFT:
			pRect->left = pRect->right - m_nInitWidth;
			break;
		case WMSZ_RIGHT:
		case WMSZ_BOTTOMRIGHT:
		case WMSZ_TOPRIGHT:
			pRect->right = pRect->left + m_nInitWidth;
			break;
		}
	}

	if (rct.Height() < m_nInitHeight)
	{
		switch (fwSide)
		{
		case WMSZ_BOTTOM:
		case WMSZ_BOTTOMLEFT:
		case WMSZ_BOTTOMRIGHT:
			pRect->bottom = pRect->top + m_nInitHeight;
			break;
		case WMSZ_TOP:
		case WMSZ_TOPLEFT:
		case WMSZ_TOPRIGHT:
			pRect->top = pRect->bottom - m_nInitHeight;
			break;
		}
	}

	CDialog::OnSizing(fwSide, pRect);
}

void CLevelPropManagerDlg::UpdateActiveLevelInfoField()
{
	CString str;
	for (int i = 0; i < m_listLevelsList.GetItemCount(); i++) {
		OdDgElementId Id = (OdDbStub*)DWORDPTR_TO_PTR(m_listLevelsList.GetItemData(i));
		OdDgLevelTableRecordPtr pLevel = Id.safeOpenObject();
		if (pLevel->getEntryId() == Id.database()->getActiveLevelEntryId())
			str.Format(_T("Active Level: %s"), ((LPCTSTR)pLevel->getName()));
	}
	GetDlgItem(IDC_STATIC_ACTIVE_LEVEL)->SetWindowText(str);
}

void DrawColorBox(CDC& dc, const RECT& rcItem, const OdCmEntityColor& cmcol)
{
	CBrush brush(RGB(cmcol.red(), cmcol.green(), cmcol.blue()));
	CGdiObject* pOld = dc.SelectObject(&brush);
	CRect rc(rcItem);
	rc.DeflateRect(1, 1, 1, 1);
	rc.right = rc.left + rc.bottom - rc.top;
	dc.Rectangle(&rc);
	dc.SelectObject(pOld);
	rc.SetRect(rc.right + 1, rcItem.top, rcItem.right, rcItem.bottom);
	if (rc.left <= rcItem.right)
	{
		OdString sTextOut;
		sTextOut.format(OD_T("%d,%d,%d"), cmcol.red(), cmcol.green(), cmcol.blue());
		dc.ExtTextOut(rc.left, rc.top, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.getLength(), NULL);
	}
}

void CLevelPropManagerDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (IDC_LIST_LEVELS == nIDCtl)
	{
		CRect rcItem;
		switch (lpDrawItemStruct->itemAction)
		{
		case ODA_DRAWENTIRE:
		{
			//clear item
			rcItem = lpDrawItemStruct->rcItem;
			CDC dc;
			COLORREF rgbBkgnd = ::GetSysColor((lpDrawItemStruct->itemState & ODS_SELECTED) ?
				COLOR_HIGHLIGHT : COLOR_WINDOW);
			dc.Attach(lpDrawItemStruct->hDC);
			CBrush br(rgbBkgnd);
			dc.FillRect(rcItem, &br);
			if (lpDrawItemStruct->itemState & ODS_FOCUS)
			{
				dc.DrawFocusRect(rcItem);
			}
			int itemID = lpDrawItemStruct->itemID;
			if (itemID != -1)
			{
				// The text color is stored as the item data.
				COLORREF rgbText = (lpDrawItemStruct->itemState & ODS_SELECTED) ?
					::GetSysColor(COLOR_HIGHLIGHTTEXT) : ::GetSysColor(COLOR_WINDOWTEXT);
				dc.SetBkColor(rgbBkgnd);
				dc.SetTextColor(rgbText);
				for (int labelIndex = 0; labelIndex < columns_count; ++labelIndex)
				{
					m_listLevelsList.GetSubItemRect(itemID, labelIndex, LVIR_LABEL, rcItem);
					DrawItem(dc, itemID, labelIndex, rcItem);
				}
			}
			dc.Detach();
		}
		break;
		case ODA_SELECT:
			::InvertRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
			break;
		case ODA_FOCUS:
			//::DrawFocusRect(lpDrawItemStruct->hDC, &(lpDrawItemStruct->rcItem));
			break;
		}

		return;
	}
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CLevelPropManagerDlg::OnButtonSetActive()
{
	OdDgLevelTableRecordPtr pLevel;
	if (OpenSelectedLevel(pLevel))
	{
		m_pDb->setActiveLevelEntryId(pLevel->getEntryId());
		UpdateActiveLevelInfoField();
		GetDlgItem(IDC_LIST_LEVELS)->RedrawWindow();
	}
}

bool hittestSubitem(CListCtrl& ListLevelsList, int columns_count, const CPoint& pt, int &index, int &labelIndex)
{
	index = ListLevelsList.GetTopIndex();
	int last_visible_index = index + ListLevelsList.GetCountPerPage();
	if (last_visible_index > ListLevelsList.GetItemCount())
	{
		last_visible_index = ListLevelsList.GetItemCount();
	}

	while (index <= last_visible_index)
	{
		// Get the bounding rectangle of an item. If the mouse
		// location is within the bounding rectangle of the item,
		// you know you have found the item that was being clicked.
		CRect rectItem;
		ListLevelsList.GetItemRect(index, &rectItem, LVIR_BOUNDS);
		if (rectItem.PtInRect(pt))
		{
			CRect rectColumn;
			for (labelIndex = 0; labelIndex < columns_count; ++labelIndex)
			{
				ListLevelsList.GetSubItemRect(index, labelIndex, LVIR_LABEL, rectColumn);
				if (rectColumn.PtInRect(pt))
				{
					return true;
				}
			}
			break;
		}

		// Get the next item in listview control.
		index++;
	}
	return false;
}

void CLevelPropManagerDlg::OnClickListLevelsList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	DWORD pos = GetMessagePos();

	CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
	m_listLevelsList.ScreenToClient(&pt);

	int index, labelIndex;
	if (hittestSubitem(m_listLevelsList, columns_count, pt, index, labelIndex))
		OnClickSubItemLevelsList(index, labelIndex);

	*pResult = 0;
}

void CLevelPropManagerDlg::OnClickLevelFilterTree(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	DWORD pos = GetMessagePos();

	CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
	m_TreeFilters.ScreenToClient(&pt);
	if (HTREEITEM h = m_TreeFilters.HitTest(pt))
	{
		m_listLevelsList.DeleteAllItems();
		OdDgElementId Id = (OdDbStub*)DWORDPTR_TO_PTR(m_TreeFilters.GetItemData(h));
		OdDgLevelFilterTableRecordPtr lf = Id.safeOpenObject(OdDg::kForWrite);

		OdDgLevelTablePtr pLevels = m_pDb->getLevelTable(OdDg::kForWrite);
		int i = 0;
		for (OdDgElementIteratorPtr pIter = pLevels->createIterator(); !pIter->done(); pIter->step())
		{
			OdDgLevelTableRecordPtr level = pIter->item().safeOpenObject(OdDg::kForWrite);
			if (lf->filter(level))
			{
				m_listLevelsList.InsertItem(i, NULL);
				m_listLevelsList.SetItemData(i, PTR_TO_DWORDPTR((OdDbStub*)pIter->item()));
				++i;
			}
		}
		m_listLevelsList.Invalidate();
	}
	*pResult = 0;
}

void CLevelPropManagerDlg::OnClickSubItemLevelsList(int itemIndex, int labelIndex)
{
	OdDgElementId Id = (OdDbStub*)DWORDPTR_TO_PTR(m_listLevelsList.GetItemData(itemIndex));
	OdDgLevelTableRecordPtr pLevel = Id.safeOpenObject(OdDg::kForWrite);
	m_bClickToColumnName = false;
	switch (labelIndex)
	{
	case column_name:
		m_bClickToColumnName = true;
		return;
	case column_used:
		pLevel->isInUse();
		break;
	case column_color:
	{
		CLevelPropColorDlg dlg(pLevel->getElementColorIndex());
		if (IDOK == dlg.DoModal())
		{
			pLevel->setElementColorIndex((OdInt16)dlg.m_nColor);
		}
	}
	break;
	case column_linestyle:
	{
		CLevelPropLineStyleDlg dlg(m_pDb, pLevel->getElementLineStyleEntryId());
		if (IDOK == dlg.DoModal())
		{
			pLevel->setElementLineStyleEntryId(dlg.m_originalId);
		}
	}
	break;
	case column_lineweight:
	{
		CLevelPropLineWeightDlg dlg(pLevel->getElementLineWeight());
		if (IDOK == dlg.DoModal())
		{
			pLevel->setElementLineWeight(dlg.m_nLineWeight);
		}
	}
	break;
	}
	m_listLevelsList.Invalidate();
}

void CLevelPropManagerDlg::DrawItem(CDC& dc, int itemID, int labelIndex, const RECT& rcItem)
{
	CString sTextOut;

	OdDgElementId Id = (OdDbStub*)DWORDPTR_TO_PTR(m_listLevelsList.GetItemData(itemID));
	//open mode
	OdDgLevelTableRecordPtr pLevel = Id.safeOpenObject();

	OdCmEntityColor color;

	switch (labelIndex)
	{
	case column_status:
		if (pLevel->getEntryId() == Id.database()->getActiveLevelEntryId())
		{
			sTextOut = L"active";
			dc.ExtTextOut(rcItem.left + 6, rcItem.top + 1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
		}
		else if (pLevel->isInUse())
		{
			sTextOut = L"in use";
			dc.ExtTextOut(rcItem.left + 6, rcItem.top + 1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
		}
		break;
	case column_name:
		sTextOut = (LPCTSTR)pLevel->getName();
		dc.ExtTextOut(rcItem.left + 6, rcItem.top + 1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
		break;
	case column_used:
		if (pLevel->isInUse())
		{
			sTextOut = L"+";
			dc.ExtTextOut(rcItem.left + 6, rcItem.top + 1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
		}
		break;
	case column_color:
		color = OdDgColorTable::createObject()->lookupTrueColor(pLevel->database(), pLevel->getElementColorIndex());
		dc.ExtTextOut(rcItem.left + 6, rcItem.top + 1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
		::DrawColorBox(dc, rcItem, color);
		break;
	case column_linestyle:
		if (pLevel->getElementLineStyleEntryId() >= 0xFFFFFFFF)
			sTextOut = L"Continuous";
		else
			sTextOut.Format(_T("%d "), pLevel->getElementLineStyleEntryId());
		dc.ExtTextOut(rcItem.left + 6, rcItem.top + 1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
		break;
	case column_lineweight:
		sTextOut = StringByLineWeight(pLevel->getElementLineWeight(), false);
		dc.ExtTextOut(rcItem.left + 6, rcItem.top + 1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
		break;
	case column_descr:
		break;
	}
}

void CLevelPropManagerDlg::OnCancel()
{
	CDialog::OnCancel();
}

void CLevelPropManagerDlg::OnDblclkListLevelsList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	if (m_bClickToColumnName)
	{
		OnButtonSetActive();
	}
	*pResult = 0;
}

void CLevelPropManagerDlg::OnButtonNew()
{
	OdDgLevelTablePtr pLevels = m_pDb->getLevelTable();
	OdDgLevelTableRecordPtr pLevel = OdDgLevelTableRecord::createObject();

	OdString str;
	int n = 1;
	do
	{
		str.format(OD_T("Level%d"), n++);
	} while (!pLevels->getAt(str).isNull());
	pLevel->setName(str);
	pLevels->add(pLevel);

	int i = m_listLevelsList.GetItemCount();
	m_listLevelsList.InsertItem(i, NULL);
	m_listLevelsList.SetItemData(i, PTR_TO_DWORDPTR((OdDbStub*)pLevel->elementId()));
}

void CLevelPropManagerDlg::OnRclickListLevelsList(NMHDR* pNMHDR, LRESULT* pResult)
{
	DWORD pos = GetMessagePos();

	CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
	m_listLevelsList.ScreenToClient(&pt);

	int index, labelIndex;
	if (hittestSubitem(m_listLevelsList, columns_count, pt, index, labelIndex))
	{
	}
	CMenu menu;
	menu.LoadMenu(IDR_LEVEL_POPUP_MENU);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, GET_X_LPARAM(pos), GET_Y_LPARAM(pos), this);

	*pResult = 0;
}

bool CLevelPropManagerDlg::OpenSelectedLevel(OdSmartPtr<OdDgLevelTableRecord>& pLevel)
{
	int nSelectionMark = m_listLevelsList.GetSelectionMark();
	if (nSelectionMark > -1)
	{
		OdDgElementId id = (OdDbStub*)DWORDPTR_TO_PTR(m_listLevelsList.GetItemData(nSelectionMark));
		pLevel = id.openObject(OdDg::kForWrite);
		return !pLevel.isNull();
	}
	return false;
}


/////////////////////////////////////////////////////////////////////////////
// CLevelPropColorDlg dialog

CLevelPropColorDlg::CLevelPropColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLevelPropColorDlg::IDD, pParent)
	, m_nColor(7)
{
}

CLevelPropColorDlg::CLevelPropColorDlg(int nColor, CWnd* pParent /*=NULL*/)
	: CDialog(CLevelPropColorDlg::IDD, pParent)
	, m_nColor(nColor)
{
}

void CLevelPropColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COLORS, m_listColors);
}

BEGIN_MESSAGE_MAP(CLevelPropColorDlg, CDialog)
	ON_LBN_DBLCLK(IDC_LIST_COLORS, OnOK)
END_MESSAGE_MAP()

BOOL CLevelPropColorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;
	for (int index = 1; index <= 255; ++index)
	{
		str.Empty();
		str.Format(_T("Color_%d"), index);
		m_listColors.InsertString(index - 1, str);
	}

	str.Format(_T("Color_%d"), m_nColor);
	m_listColors.SelectString(-1, str);
	return TRUE;
}

void CLevelPropColorDlg::OnOK()
{
	m_nColor = m_listColors.GetCurSel() + 1;
	CDialog::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// CLevelPropLineStyleDlg dialog

CLevelPropLineStyleDlg::CLevelPropLineStyleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLevelPropLineStyleDlg::IDD, pParent)
	, m_pDb(NULL)
{
}

CLevelPropLineStyleDlg::CLevelPropLineStyleDlg(OdDgDatabase *pDb, const OdUInt32& originalId, CWnd* pParent)
	: CDialog(CLevelPropLineStyleDlg::IDD, pParent)
	, m_pDb(pDb)
	, m_originalId(originalId)
{
}

void CLevelPropLineStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LINESTYLE, m_listLineStyle);
}

BEGIN_MESSAGE_MAP(CLevelPropLineStyleDlg, CDialog)
	ON_LBN_DBLCLK(IDC_LIST_LINESTYLE, OnOK)
END_MESSAGE_MAP()

BOOL CLevelPropLineStyleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	if (!m_pDb)
	{
		return FALSE;
	}
	m_listLineStyle.DeleteAllItems();
	m_listLineStyle.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listLineStyle.InsertColumn(0, _T("LineStyle"), LVCFMT_LEFT, 160);
	//displays entry id. could be any description
	m_listLineStyle.InsertColumn(1, _T("Description"), LVCFMT_LEFT, 160);

	FillLineStylesList();
	return TRUE;
}

void CLevelPropLineStyleDlg::OnOK()
{
	m_listLineStyle.SetFocus();
	int index = m_listLineStyle.GetSelectionMark();
	if (index > -1)
	{
		int id = m_listLineStyle.GetItemData(index);
		m_originalId = id;
	}
	CDialog::OnOK();
}

void CLevelPropLineStyleDlg::FillLineStylesList()
{
	m_listLineStyle.DeleteAllItems();
	OdDgLineStyleTablePtr pLineStyles = m_pDb->getLineStyleTable(OdDg::kForRead);
	OdDgElementIteratorPtr pIter = pLineStyles->createIterator();
	OdDgLineStyleTableRecordPtr pLineStyleRec;
	int i = 0;
	for (pIter->start(); !pIter->done(); pIter->step())
	{
		OdDgElementId id = pIter->item();
		m_listLineStyle.InsertItem(i, NULL);
		pLineStyleRec = id.safeOpenObject();
		m_listLineStyle.SetItemData(i, (OdInt32)pLineStyleRec->getEntryId());
		m_listLineStyle.SetItemText(i, 0, pLineStyleRec->getName());
		OdString str;
		str.format(OD_T("Entry Id: %d"), (OdInt32)pLineStyleRec->getEntryId());
		m_listLineStyle.SetItemText(i, 1, str);
		++i;
	}

	for (int j = 0; j <= 7; j++) {
		m_listLineStyle.InsertItem(i + j, NULL);
		m_listLineStyle.SetItemData(i + j, j);
		OdString str;
		str.format(OD_T("Entry Id: %d"), j);
		m_listLineStyle.SetItemText(i + j, 0, str);
		m_listLineStyle.SetItemText(i + j, 1, str);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CLevelPropLineWeightDlg dialog

CLevelPropLineWeightDlg::CLevelPropLineWeightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLevelPropLineWeightDlg::IDD, pParent)
{
}

CLevelPropLineWeightDlg::CLevelPropLineWeightDlg(int nOrigLineWeight, CWnd* pParent)
	: CDialog(CLevelPropLineWeightDlg::IDD, pParent)
	, m_nOrigLineWeight(nOrigLineWeight)
	, m_nLineWeight(nOrigLineWeight)
{
}

void CLevelPropLineWeightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LINEWEIGHT, m_listLineweight);
}

BEGIN_MESSAGE_MAP(CLevelPropLineWeightDlg, CDialog)
	ON_LBN_DBLCLK(IDC_LIST_LINEWEIGHT, OnOK)
END_MESSAGE_MAP()

BOOL CLevelPropLineWeightDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int index = 0; index < 32; ++index)
	{
		m_listLineweight.InsertString(index, StringByLineWeight(index));
		m_listLineweight.SetItemData(index, (index));
	}
	m_listLineweight.SelectString(-1, StringByLineWeight(m_nOrigLineWeight, false));
	CString str;
	str.Format(_T("Original : %s"), StringByLineWeight(m_nOrigLineWeight, false));
	GetDlgItem(IDC_STATIC_LINEWEIGHT_ORIGINAL)->SetWindowText(str);
	return TRUE;
}

void CLevelPropLineWeightDlg::OnOK()
{
	int index = m_listLineweight.GetCurSel();
	m_nLineWeight = (OdUInt32)m_listLineweight.GetItemData(index);
	CDialog::OnOK();
}
