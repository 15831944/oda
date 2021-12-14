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
// LinkArrtDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "LinkArrtDlg.h"
#include "DgXAttribute.h"

using namespace std;

// CLinkArrtDlg dialog

IMPLEMENT_DYNAMIC(CLinkArrtDlg, CDialog)

//CLinkArrtDlg::CLinkArrtDlg(CWnd* pParent /*=NULL*/)
//	: CDialog(IDD_LINKS_ATTRS, pParent)
//{
//
//}

CLinkArrtDlg::CLinkArrtDlg(const OdDgElementId& idCurrentElement, CWnd * pParent)
	: CDialog(IDD_LINKS_ATTRS, pParent)
{
	m_idCurrentElement = idCurrentElement;
}

CLinkArrtDlg::~CLinkArrtDlg()
{
}

BOOL CLinkArrtDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitLinkageArray(); InitXAttributesArray();

	m_buttonDelLink.EnableWindow(false);
	m_buttonUpLink.EnableWindow(false);
	m_buttonDownLink.EnableWindow(false);

	m_buttonDelAttr.EnableWindow(false);
	m_buttonDownAttr.EnableWindow(false);
	m_buttonUpAttr.EnableWindow(false);

	SwitchPage(0);

	UpdateLinksTable();	UpdateAttrsTable();

	return 0;
}

void CLinkArrtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listControl);
	DDX_Control(pDX, IDC_BUTTON_UP_LINK, m_buttonUpLink);
	DDX_Control(pDX, IDC_BUTTON_DOWN_LINK, m_buttonDownLink);
	DDX_Control(pDX, IDC_BUTTON_DELETE_LINK, m_buttonDelLink);
	DDX_Control(pDX, IDC_LIST_ATTRS, m_listControlAttr);
	DDX_Control(pDX, IDC_BUTTON3, m_buttonUpAttr);
	DDX_Control(pDX, IDC_BUTTON4, m_buttonDownAttr);
	DDX_Control(pDX, IDC_BUTTON5, m_buttonDelAttr);
	DDX_Control(pDX, IDC_BUTTON7, m_buttonLinkages);
	DDX_Control(pDX, IDC_BUTTON8, m_buttonAttributes);
}


BEGIN_MESSAGE_MAP(CLinkArrtDlg, CDialog)
	ON_BN_CLICKED(IDOK2, &CLinkArrtDlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_LINK, &CLinkArrtDlg::OnBnClickedButtonDeleteLink)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_LINK, &CLinkArrtDlg::OnBnClickedButtonDownLink)
	ON_BN_CLICKED(IDC_BUTTON_UP_LINK, &CLinkArrtDlg::OnBnClickedButtonUpLink)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CLinkArrtDlg::OnNMClickList1)
	ON_BN_CLICKED(IDC_BUTTON7, &CLinkArrtDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CLinkArrtDlg::OnBnClickedButton8)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ATTRS, &CLinkArrtDlg::OnNMClickListAttrs)
	ON_BN_CLICKED(IDC_BUTTON3, &CLinkArrtDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CLinkArrtDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CLinkArrtDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CLinkArrtDlg message handlers


void CLinkArrtDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CLinkArrtDlg::AddData(CListCtrl &ctrl, int row, int col, LPWSTR str)
{
	LVITEM lv;
	lv.iItem = row;
	lv.iSubItem = col;
	lv.pszText = str;
	lv.mask = LVIF_TEXT;
	if (col == 0)
		ctrl.InsertItem(&lv);
	else
		ctrl.SetItem(&lv);
}

OdString CLinkArrtDlg::GetAttributeClassName(const OdDgXAttributePtr &pXAttribute)
{
  if( !pXAttribute.isNull() )
  {
    return pXAttribute->isA()->name();
  }
  else
  {
    return OdString(L"Undefined Attribute");
  }
}

OdString CLinkArrtDlg::GetLinkageClassName(const OdDgAttributeLinkagePtr &pLinkage)
{
  if( !pLinkage.isNull() )
  {
    return pLinkage->isA()->name();
  }
  else
  {
    return OdString(L"Undefined Linkage");
  }
}

void CLinkArrtDlg::InitLinkageArray()
{
	//take all linkages
	if (!m_idCurrentElement.isNull())
  {
    OdDgElementPtr pElm = m_idCurrentElement.openObject(OdDg::kForRead);
		pElm->getLinkages(m_linkagesArray);
  }
}

void CLinkArrtDlg::InitXAttributesArray()
{
	//take all xattributes
  if (!m_idCurrentElement.isNull())
  {
    OdDgElementPtr pElm = m_idCurrentElement.openObject(OdDg::kForRead);
    pElm->getXAttributes(m_xAttributesArray);
  }
}

void CLinkArrtDlg::UpdateLinksTable()
{
	//Reseting table contents
	m_listControl.DeleteAllItems();
	int iColumnCount = m_listControl.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < iColumnCount; i++)
	{
		m_listControl.DeleteColumn(0);
	}

	//Setting up table headers
	m_listControl.InsertColumn(0, _T("Linkage Primary ID"), LVCFMT_LEFT, 0);
	m_listControl.InsertColumn(1, _T("Linkage Data Size"), LVCFMT_LEFT, 0);
	m_listControl.InsertColumn(2, _T("Linkage Class Name"), LVCFMT_LEFT, 0);

	for (int i = 0; i < m_listControl.GetHeaderCtrl()->GetItemCount(); ++i)
		m_listControl.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);

	m_listControl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	OdString sLinkId, sLinkDataSize;
	char text[500];
	wchar_t wtext[500];

	int iLinkagesNumber = m_linkagesArray.size();
	for (int i = 0; i < iLinkagesNumber; ++i)
	{
		const OdDgAttributeLinkagePtr pLinkage = m_linkagesArray[i];

		sprintf(text, "%x", pLinkage->getPrimaryId());
		mbstowcs(wtext, text, strlen(text) + 1); //Plus null
		LPWSTR ptrId = wtext;
		AddData(m_listControl, i, 0, ptrId);

		OdBinaryData binData;
		pLinkage->getData(binData);
		sprintf(text, "%d", binData.size());

		sprintf(text, "%d", binData.size());
		mbstowcs(wtext, text, strlen(text) + 1); //Plus null
		LPWSTR ptrSize = wtext;
		AddData(m_listControl, i, 1, ptrSize);

		const char *s = GetLinkageClassName(pLinkage);
		mbstowcs(wtext, s, strlen(s) + 1); //Plus null
		LPWSTR ptrType = wtext;
		AddData(m_listControl, i, 2, ptrType);
	}
}

void CLinkArrtDlg::UpdateAttrsTable()
{
	//Reseting table contents
	m_listControlAttr.DeleteAllItems();
	int iColumnCount = m_listControlAttr.GetHeaderCtrl()->GetItemCount();
	for (int i = 0; i < iColumnCount; i++)
	{
		m_listControlAttr.DeleteColumn(0);
	}

	//Setting up table headers
	m_listControlAttr.InsertColumn(0, _T("Handle ID"), LVCFMT_LEFT, 0);
	m_listControlAttr.InsertColumn(1, _T("XAttribute ID"), LVCFMT_LEFT, 0);
	m_listControlAttr.InsertColumn(2, _T("XAttribute Data Size"), LVCFMT_LEFT, 0);
	m_listControlAttr.InsertColumn(3, _T("XAttribute Class Name"), LVCFMT_LEFT, 0);

	for (int i = 0; i < m_listControlAttr.GetHeaderCtrl()->GetItemCount(); ++i)
		m_listControlAttr.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);

	m_listControlAttr.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	OdString sLinkId, sLinkDataSize;
	char text[500];
	wchar_t wtext[500];

	int iAttributesNumber = m_xAttributesArray.size();
	for (int i = 0; i < iAttributesNumber; ++i)
	{
		const OdDgXAttributePtr pXAttribute = m_xAttributesArray[i];
		OdBinaryData data;
		pXAttribute->writeData(data);

		OdUInt32 handlerId = pXAttribute->getHandlerId();
		OdUInt32 attrId = pXAttribute->getXAttrId();

		sprintf(text, "%x", handlerId);
		mbstowcs(wtext, text, strlen(text) + 1); //Plus null
		LPWSTR ptrHId = wtext;
		AddData(m_listControlAttr, i, 0, ptrHId);

		sprintf(text, "%x", attrId);
		mbstowcs(wtext, text, strlen(text) + 1); //Plus null
		LPWSTR ptrId = wtext;
		AddData(m_listControlAttr, i, 1, ptrId);

		sprintf(text, "%d", data.size());
		mbstowcs(wtext, text, strlen(text) + 1); //Plus null
		LPWSTR ptrData = wtext;
		AddData(m_listControlAttr, i, 2, ptrData);

		const char *s = GetAttributeClassName(pXAttribute);
		mbstowcs(wtext, s, strlen(s) + 1); //Plus null
		LPWSTR ptrClassName = wtext;
		AddData(m_listControlAttr, i, 3, ptrClassName);
	}
}

void CLinkArrtDlg::ApplyChangesToLinkages()
{
  OdRxObjectPtrArray arrLinkages;
  OdDgElementPtr pElm = m_idCurrentElement.openObject(OdDg::kForWrite);

  pElm->getLinkages(arrLinkages);

  OdUInt32 i;

	for (i = 0; i < arrLinkages.size(); i++)
	{
		OdDgAttributeLinkagePtr pLinkage = arrLinkages[i];
		pElm->removeLinkages(pLinkage->getPrimaryId(), false);
	}

	for (i = 0; i < m_linkagesArray.size(); i++)
	{
		OdDgAttributeLinkagePtr pLinkage = m_linkagesArray[i];
		pElm->addLinkage(pLinkage->getPrimaryId(), pLinkage);
	}
}

void CLinkArrtDlg::ApplyChangesToAttributes()
{
  OdRxObjectPtrArray arrXAttrs;
  OdDgElementPtr pElm = m_idCurrentElement.openObject(OdDg::kForWrite);

  pElm->getXAttributes(arrXAttrs);

  OdUInt32 i;

	for (i = 0; i < arrXAttrs.size(); i++)
	{
		OdDgXAttributePtr pXAttribute = arrXAttrs[i];
		pElm->removeXAttributes(pXAttribute->getHandlerId());
	}

	for (i = 0; i < m_xAttributesArray.size(); i++)
	{
		OdDgXAttributePtr pXAttribute = m_xAttributesArray[i];
		pElm->addXAttribute(pXAttribute->getHandlerId(), pXAttribute);
	}
}

void CLinkArrtDlg::EraseSelectedLinkage()
{
	int index = m_listControl.GetNextItem(-1, LVNI_SELECTED);
	int size = m_linkagesArray.size();
	if ((size > index) && (index != -1))
	{
		m_linkagesArray.erase(m_linkagesArray.begin() + index);
	}
}

void CLinkArrtDlg::EraseSelectedAttribute()
{
	int index = m_listControlAttr.GetNextItem(-1, LVNI_SELECTED);
	int size = m_xAttributesArray.size();
	if ((size > index) && (index != -1))
	{
		m_xAttributesArray.erase(m_xAttributesArray.begin() + index);
	}
}

//Shifts selected linkage, while iDirection == -1 - up, +1 - down
void CLinkArrtDlg::ShiftSelectedLinkage(int iDirection)
{
	int index = m_listControl.GetNextItem(-1, LVNI_SELECTED);
	int size = m_linkagesArray.size();

	if ((size > index) && (index != -1) && (index + iDirection != -1) && (index + iDirection < size))
	{
		OdRxObjectPtr pLinkage = m_linkagesArray.at(index);
		m_linkagesArray.erase(m_linkagesArray.begin() + index);
		m_linkagesArray.insertAt(index + iDirection, pLinkage);
	}
}

//Shifts selected attribute, while iDirection == -1 - up, +1 - down
void CLinkArrtDlg::ShiftSelectedAttribute(int iDirection)
{
	int index = m_listControlAttr.GetNextItem(-1, LVNI_SELECTED);
	int size = m_xAttributesArray.size();

	if( (size > index) && (index != -1) && (index + iDirection != -1) && (index + iDirection < size) )
	{
		OdDgXAttributePtr pXAttribute = m_xAttributesArray.at(index);
		m_xAttributesArray.erase(m_xAttributesArray.begin() + index);
		m_xAttributesArray.insertAt(index + iDirection, pXAttribute);
	}
}

//Switches to tab with iTabIndex number
void CLinkArrtDlg::SwitchPage(unsigned int iTabIndex)
{
	if (iTabIndex == 0)
	{
		m_listControlAttr.ShowWindow(SW_HIDE);
		m_buttonUpAttr.ShowWindow(SW_HIDE);
		m_buttonDownAttr.ShowWindow(SW_HIDE);
		m_buttonDelAttr.ShowWindow(SW_HIDE);

		m_buttonUpLink.ShowWindow(SW_SHOW);
		m_buttonDownLink.ShowWindow(SW_SHOW);
		m_buttonDelLink.ShowWindow(SW_SHOW);
		m_listControl.ShowWindow(SW_SHOW);

		m_buttonLinkages.EnableWindow(false);
		m_buttonAttributes.EnableWindow(true);

		return;
	}
	if (iTabIndex == 1)
	{
		m_listControlAttr.ShowWindow(SW_SHOW);
		m_buttonUpAttr.ShowWindow(SW_SHOW);
		m_buttonDownAttr.ShowWindow(SW_SHOW);
		m_buttonDelAttr.ShowWindow(SW_SHOW);

		m_buttonUpLink.ShowWindow(SW_HIDE);
		m_buttonDownLink.ShowWindow(SW_HIDE);
		m_buttonDelLink.ShowWindow(SW_HIDE);
		m_listControl.ShowWindow(SW_HIDE);

		m_buttonLinkages.EnableWindow(true);
		m_buttonAttributes.EnableWindow(false);
		return;
	}
}

void CLinkArrtDlg::OnBnClickedOk2()
{
	CDialog::EndDialog(0);
}

void CLinkArrtDlg::OnBnClickedButtonDeleteLink()
{
	EraseSelectedLinkage();
	ApplyChangesToLinkages();
	UpdateLinksTable();
}

void CLinkArrtDlg::OnBnClickedButtonDownLink()
{
	ShiftSelectedLinkage(1);
	ApplyChangesToLinkages();
	UpdateLinksTable();
}

void CLinkArrtDlg::OnBnClickedButtonUpLink()
{
	ShiftSelectedLinkage(-1);
	ApplyChangesToLinkages();
	UpdateLinksTable();
}

void CLinkArrtDlg::SetLinksButtonsState()
{
	int index = m_listControl.GetNextItem(-1, LVNI_SELECTED);
	int size = m_linkagesArray.size();
	if ((index < 0) || (index > size))
	{
		m_buttonDelLink.EnableWindow(false);
		m_buttonUpLink.EnableWindow(false);
		m_buttonDownLink.EnableWindow(false);
	}
	else {
		m_buttonDelLink.EnableWindow(true);
		m_buttonUpLink.EnableWindow(true);
		m_buttonDownLink.EnableWindow(true);

		if ((index == 0))
		{
			m_buttonUpLink.EnableWindow(false);
		}
		else
		{
			OdDgAttributeLinkagePtr pLinkage0 = m_linkagesArray[index - 1];
			OdDgAttributeLinkagePtr pLinkage1 = m_linkagesArray[index];
			if (pLinkage0->getPrimaryId() == pLinkage1->getPrimaryId())
			{
				m_buttonUpLink.EnableWindow(true);
			}
			else
			{
				m_buttonUpLink.EnableWindow(false);
			}
		}

		if (index == (m_linkagesArray.size() - 1))
		{
			m_buttonDownLink.EnableWindow(false);
		}
		else
		{
			OdDgAttributeLinkagePtr pLinkage0 = m_linkagesArray[index + 1];
			OdDgAttributeLinkagePtr pLinkage1 = m_linkagesArray[index];
			if (pLinkage0->getPrimaryId() == pLinkage1->getPrimaryId())
			{
				m_buttonDownLink.EnableWindow(true);
			}
			else
			{
				m_buttonDownLink.EnableWindow(false);
			}
		}
	}
}

void CLinkArrtDlg::SetAttrsButtonsState()
{
	int index = m_listControlAttr.GetNextItem(-1, LVNI_SELECTED);
	int size = m_xAttributesArray.size();
	if ((index < 0) || (index > size))
	{
		m_buttonDelAttr.EnableWindow(false);
		m_buttonUpAttr.EnableWindow(false);
		m_buttonDownAttr.EnableWindow(false);
	}
	else {
		m_buttonDelAttr.EnableWindow(true);
		m_buttonUpAttr.EnableWindow(true);
		m_buttonDownAttr.EnableWindow(true);

		if ((index == 0))
		{
			m_buttonUpAttr.EnableWindow(false);
		}

		if (index == (m_xAttributesArray.size() - 1))
		{
			m_buttonDownAttr.EnableWindow(false);
		}
	}
}

void CLinkArrtDlg::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	SetLinksButtonsState();

	*pResult = 0;
}

void CLinkArrtDlg::OnNMClickListAttrs(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	SetAttrsButtonsState();

	*pResult = 0;
}

void CLinkArrtDlg::OnBnClickedButton7()
{
	SwitchPage(0);
}

void CLinkArrtDlg::OnBnClickedButton8()
{
	SwitchPage(1);
}

void CLinkArrtDlg::OnBnClickedButton3()
{
	ShiftSelectedAttribute(-1);
	ApplyChangesToAttributes();
	UpdateAttrsTable();
}

void CLinkArrtDlg::OnBnClickedButton4()
{
	ShiftSelectedAttribute(1);
	ApplyChangesToAttributes();
	UpdateAttrsTable();
}

void CLinkArrtDlg::OnBnClickedButton5()
{
	EraseSelectedAttribute();
	ApplyChangesToAttributes();
	UpdateAttrsTable();
}
