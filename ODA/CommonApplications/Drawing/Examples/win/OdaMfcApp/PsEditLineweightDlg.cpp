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

// PsEditLineweightDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PsEditLineweightDlg.h"
#include "PsTbEditFormViewPg.h"
#include "Ge/GeIntArray.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPsEditLineweightDlg dialog

//IMPLEMENT_DYNCREATE(CPsEditLineweight, CDialog)

 CPsEditLineweightDlg::CPsEditLineweightDlg(CWnd* pParent /*=NULL*/) 
   : CDialog(CPsEditLineweightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPsTbEditFormViewPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

   m_pPlotStyleTable = NULL;
   m_uintListActiveIdx = 0;
   m_pPsEditLineweightIdx = 0;
}

 BOOL CPsEditLineweightDlg::DestroyWindow() 
{
	delete m_pPsEditLineweightIdx;
  m_pPsEditLineweightIdx = 0;
	return CDialog::DestroyWindow();
}


void CPsEditLineweightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbTreeSettingPage1Dlg)
  	    DDX_Control(pDX, IDC_PS_EDITLINEWEIGHT_LIST_LINEWEIGHTS,   m_listLineweight);
       	DDX_Control(pDX, IDC_PS_EDITLINEWEIGHT_BUTTON_MILLIMETERS, m_radioMillimetrs);
        DDX_Control(pDX, IDC_PS_EDITLINEWEIGHT_BUTTON_INCHES,      m_radioInches); 
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
  
}

BEGIN_MESSAGE_MAP(CPsEditLineweightDlg, CDialog)
	//{{AFX_MSG_MAP(CPsEditLineweight)
		// NOTE: the ClassWizard will add message map macros here
    	ON_BN_CLICKED   (                  IDC_PS_EDITLINEWEIGHT_BUTTON_MILLIMETERS,    OnRadioMillimetrs)
      ON_BN_CLICKED   (                  IDC_PS_EDITLINEWEIGHT_BUTTON_INCHES,         OnRadioInches)
    	ON_BN_CLICKED   (                  IDC_PS_EDITLINEWEIGHT_BUTTON_EDITLINEWEIGHT, OnButtonEditlineweight)
    	ON_BN_CLICKED   (                  IDC_PS_EDITLINEWEIGHT_BUTTON_SORTLINEWEIGHT, OnButtonSortlineweight)
    	ON_NOTIFY       (LVN_ENDLABELEDIT, IDC_PS_EDITLINEWEIGHT_LIST_LINEWEIGHTS,      OnEndlabeleditListLineweight)
    //ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

typedef struct {
  CListCtrl *m_pListBox;
  int m_nSubItem;
  char m_cType;
} SORTTYPE;

static int CALLBACK SortFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  USES_CONVERSION;
  SORTTYPE*  sortType = (SORTTYPE *)lParamSort;
  CListCtrl* pListCtrl = sortType->m_pListBox;
  CPsEditLineweightIdx *pPsEditLineweightIdx1 = (CPsEditLineweightIdx*)lParam1;
  CPsEditLineweightIdx *pPsEditLineweightIdx2 = (CPsEditLineweightIdx*)lParam2;
  CString s1 = pListCtrl->GetItemText(pPsEditLineweightIdx1->m_iNewIdx, sortType->m_nSubItem);
  CString s2 = pListCtrl->GetItemText(pPsEditLineweightIdx2->m_iNewIdx, sortType->m_nSubItem);
  float value1 = (float)atof(T2A((LPTSTR)(LPCTSTR)s1));
  float value2 = (float)atof(T2A((LPTSTR)(LPCTSTR)s2));
  return value1 > value2;
}

void CPsEditLineweightDlg::OnOK() 
{
	// TODO: Add extra validation here
  OnButtonSortlineweight();
  m_pPlotStyleTable->setDisplayCustomLineweightUnits(m_radioInches.GetCheck() ? true : false);
  unsigned int uiPlotStyleSize = m_pPlotStyleTable->plotStyleSize();
  OdPsPlotStylePtr pPs;
  OdPsPlotStyleData OdPsData;
  int iLineweightQnt = m_pPlotStyleTable->lineweightSize();
  for( unsigned int i = 0; i < uiPlotStyleSize; i++ )
  {
    pPs = m_pPlotStyleTable->plotStyleAt(i);
    pPs->getData(OdPsData);
    int lineweight = (int)OdPsData.lineweight()-1;
    for( int j = 0; j < iLineweightQnt; j++)
    {
      if(m_pPsEditLineweightIdx[j].m_iOldIdx == lineweight)
      {
        if(m_pPsEditLineweightIdx[j].m_iOldIdx != m_pPsEditLineweightIdx[j].m_iNewIdx)
        {
          OdPsData.setLineweight(m_pPsEditLineweightIdx[j].m_iNewIdx+1);
          pPs->setData(OdPsData);
        }
        break;
      }
    }
  }

  OdGeDoubleArray lineweights;
  int             nIndex;
  const int nQnt = m_listLineweight.GetItemCount();
  lineweights.resize(nQnt);
  for(nIndex = 0; nIndex < nQnt;  nIndex++)
  {
    CPsEditLineweightIdx *pPsEditLineweightIdx = (CPsEditLineweightIdx *)m_listLineweight.GetItemData(nIndex);
    lineweights[nIndex] = pPsEditLineweightIdx->value;
  }
  m_pPlotStyleTable->setLineweights(lineweights);

	CDialog::OnOK();
}

void CPsEditLineweightDlg::OnButtonSortlineweight()
{

   SORTTYPE sortType;
   sortType.m_cType = 0; // sort type
   sortType.m_pListBox = &m_listLineweight;
   sortType.m_nSubItem = 0; //nSubItem;
   m_listLineweight.SortItems(SortFunc, (LPARAM) &sortType);
   int nQnt = m_listLineweight.GetItemCount();
   for( int i = 0; i < nQnt; i++)
   {
     CPsEditLineweightIdx *pPsEditLineweightIdx = (CPsEditLineweightIdx *)m_listLineweight.GetItemData(i);
     pPsEditLineweightIdx->m_iNewIdx = i; 
   }

}

void CPsEditLineweightDlg::OnEndlabeleditListLineweight(NMHDR* pNMHDR, LRESULT* pResult) 
{
  USES_CONVERSION;
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
  LV_ITEM* pItem = &((LV_DISPINFO*)pDispInfo)->item;
  if(pItem->mask & LVIF_TEXT)
  {
    CString str;
    str.Format(_T("%.4f"), atof(T2A(pItem->pszText)));
    m_listLineweight.SetItemText(pItem->iItem,  pItem->iSubItem, str);
    CPsEditLineweightIdx *pPsEditLineweightIdx = (CPsEditLineweightIdx *)m_listLineweight.GetItemData(pItem->iItem);
    if(m_radioInches.GetCheck())
    {
      pPsEditLineweightIdx->value = INCHTOMM(atof(T2A(pItem->pszText)));
    }
    else
    {
      pPsEditLineweightIdx->value = atof(T2A(pItem->pszText));
    }
  }
	
	*pResult = 0;
}

void CPsEditLineweightDlg::OnButtonEditlineweight()
{
	CRect   rect;
	m_listLineweight.GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_listLineweight.ModifyStyle(0, LVS_EDITLABELS);
	InvalidateRect(rect);
	UpdateData(FALSE);  // send information back *

  m_listLineweight.SetFocus();
  m_listLineweight.EditLabel(m_listLineweight.GetSelectionMark());
} 

void CPsEditLineweightDlg::SetListActiveIdx(const UINT index)
{
  m_uintListActiveIdx = !index ? index : index-1;
}

void CPsEditLineweightDlg::SetUnitIntoList(const bool bUnit)
{
  CString lineweight;
  int nQnt = m_listLineweight.GetItemCount();
  for( int i = 0; i < nQnt; i++)
  {

    CPsEditLineweightIdx *pPsEditLineweightIdx = (CPsEditLineweightIdx *)m_listLineweight.GetItemData(i);
    float fUnit = (float)pPsEditLineweightIdx->value;
    lineweight.Format(_T("%.4f"), bUnit ? MMTOINCH(fUnit) : fUnit);

    LV_ITEM lvi;
  	lvi.mask = LVIF_TEXT;
  	lvi.iItem = i;
  	lvi.iSubItem = 0;
  	lvi.pszText = (LPTSTR)(LPCTSTR)lineweight;
  	lvi.cchTextMax = lineweight.GetLength();
    m_listLineweight.SetItem(&lvi);
  }
}

void CPsEditLineweightDlg::OnRadioMillimetrs()
{
  SetUnitIntoList(false);
}

void CPsEditLineweightDlg::OnRadioInches()
{
  SetUnitIntoList(true);
}

const bool CPsEditLineweightDlg::SetPlotStyleTable(OdPsPlotStyleTable* pPlotStyleTable)
{
  if(!pPlotStyleTable) return false;
  m_pPlotStyleTable = pPlotStyleTable;
  return true;
}

void CPsEditLineweightDlg::initImageList()
{
	VERIFY(m_imageList.Create(IDB_PS_BITMAP_WHITE, 16, 3, RGB(255, 255, 255)));
  HBITMAP hBmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(), 
          MAKEINTRESOURCE(IDB_PS_BITMAP_CHECK), IMAGE_BITMAP, 16, 16, LR_CREATEDIBSECTION ); 
  CBitmap *bitmap = CBitmap::FromHandle(hBmp);
  m_imageList.Add(bitmap, RGB( 255, 255, 255));
  m_listLineweight.SetImageList(&m_imageList, LVSIL_SMALL);
}

void CPsEditLineweightDlg::initListCtrl()
{

  delete m_pPsEditLineweightIdx;
  m_pPsEditLineweightIdx = 0;

  LV_COLUMN lvColumn;
  lvColumn.mask = LVCF_FMT|LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM|LVCF_IMAGE;
  lvColumn.fmt = LVCFMT_LEFT;
  lvColumn.cx = 75;

  lvColumn.iSubItem = 0;
  lvColumn.pszText = _T("Value");
  m_listLineweight.InsertColumn(0, &lvColumn);


  lvColumn.iSubItem = 1;
  lvColumn.pszText = _T("In Use");
  m_listLineweight.InsertColumn(1, &lvColumn);


  OdGeIntArray useLineWeightIndex;
  OdPsPlotStylePtr pPs;
  OdPsPlotStyleData OdPsData;
  unsigned int uiPlotStyleSize = m_pPlotStyleTable->plotStyleSize();
  for(unsigned int j = 0; j < uiPlotStyleSize; j++)
  {

    pPs = m_pPlotStyleTable->plotStyleAt(j);
    pPs->getData(OdPsData);
    unsigned int value, nIndex;
    value = (unsigned int)OdPsData.lineweight()-1;
    if( !useLineWeightIndex.find(value, nIndex))
      useLineWeightIndex.push_back(value);
  }

  m_pPsEditLineweightIdx = new CPsEditLineweightIdx[m_pPlotStyleTable->lineweightSize()];
  const bool bInch = m_pPlotStyleTable->isDisplayCustomLineweightUnits();
  for(unsigned int i = 0; i < m_pPlotStyleTable->lineweightSize(); i++)
  {

    m_pPsEditLineweightIdx[i].m_iOldIdx = i;
    m_pPsEditLineweightIdx[i].m_iNewIdx = i;
    m_pPsEditLineweightIdx[i].value = m_pPlotStyleTable->getLineweightAt(i);

    CString lineweight;
    lineweight.Format(_T("%.4f"), bInch ? MMTOINCH(m_pPsEditLineweightIdx[i].value) : m_pPsEditLineweightIdx[i].value);
    bool isUse = false;
    unsigned int nIndex;

    if(useLineWeightIndex.find(i, nIndex))
          isUse = true;

    unsigned int item = insertItem(i, lineweight, isUse );
    m_listLineweight.SetItemData(item, (DWORD_PTR)&m_pPsEditLineweightIdx[i]);
  }

  m_listLineweight.SetItemState(m_uintListActiveIdx, LVIS_SELECTED, LVIS_SELECTED);

}

const int CPsEditLineweightDlg::insertItem(int index, const CString& sLineweight, const bool isUse)
{
  m_listLineweight.LockWindowUpdate();	


  LV_ITEM lvItem;
  lvItem.mask = LVIF_TEXT|LVIF_STATE;
  lvItem.state = 0;
  lvItem.stateMask = 0;
  lvItem.iItem = index;
  lvItem.iSubItem = 0;

  lvItem.pszText = (LPTSTR)(LPCTSTR)sLineweight;
  int nItem = m_listLineweight.InsertItem(&lvItem);

  LV_ITEM lvItem1;
  lvItem1.mask = LVIF_STATE|LVIF_IMAGE;
  lvItem1.state = 0;
  lvItem1.stateMask = 0;
  lvItem1.iImage = isUse ? 1 : 0;
  lvItem1.iItem = index;
  lvItem1.iSubItem = 1;
  m_listLineweight.SetItem(&lvItem1);

 	m_listLineweight.UnlockWindowUpdate();	

  return nItem;
}

BOOL CPsEditLineweightDlg::OnInitDialog() 
{

	CDialog::OnInitDialog();

  if(!m_pPlotStyleTable) return FALSE;

  if(m_pPlotStyleTable->isDisplayCustomLineweightUnits())
    m_radioInches.SetCheck(1);
  else
    m_radioMillimetrs.SetCheck(1);

  initImageList();
  m_listLineweight.SetExtendedStyle(LVS_EX_SUBITEMIMAGES);
  initListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


