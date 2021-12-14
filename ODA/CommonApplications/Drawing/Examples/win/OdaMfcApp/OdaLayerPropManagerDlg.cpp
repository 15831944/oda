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

// OdaLayerPropManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "OdaLayerPropManagerDlg.h"
#include "DbSymUtl.h"
#include "OdaDefs.h"
#include "LoadLineTypes.h"
#include "VectProps.h"

#include "DbLayerTable.h"
#include "DbLayerTableRecord.h"
#include "DbLinetypeTable.h"
#include "DbLinetypeTableRecord.h"
#include "DbViewport.h"
#include "LayerStatesDialog.h"

#if _MSC_VER < 1400
#include <atlbase.h>
#include <atlwin.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PALETTE_SIZE 255


OdDb::LineWeight lineWeightByIndex(OdInt8 lw)
{
  switch(lw)
  {
  case 0:
    return OdDb::kLnWt000;
  case 1:
    return OdDb::kLnWt005;
  case 2:
    return OdDb::kLnWt009;
  case 3:
    return OdDb::kLnWt013;
  case 4:
    return OdDb::kLnWt015;
  case 5:
    return OdDb::kLnWt018;
  case 6:
    return OdDb::kLnWt020;
  case 7:
    return OdDb::kLnWt025;
  case 8:
    return OdDb::kLnWt030;
  case 9:
    return OdDb::kLnWt035;
  case 10:
    return OdDb::kLnWt040;
  case 11:
    return OdDb::kLnWt050;
  case 12:
    return OdDb::kLnWt053;
  case 13:
    return OdDb::kLnWt060;
  case 14:
    return OdDb::kLnWt070;
  case 15:
    return OdDb::kLnWt080;
  case 16:
    return OdDb::kLnWt090;
  case 17:
    return OdDb::kLnWt100;
  case 18:
    return OdDb::kLnWt106;
  case 19:
    return OdDb::kLnWt120;
  case 20:
    return OdDb::kLnWt140;
  case 21:
    return OdDb::kLnWt158;
  case 22:
    return OdDb::kLnWt200;
  case 23:
    return OdDb::kLnWt211;
  case 30:
    return OdDb::kLnWtByBlock;
  case 31:
    return OdDb::kLnWtByLwDefault;
  }
  return OdDb::kLnWtByLayer;
}

CString StringByLineWeight(int nLineWeight, bool bLineWeightByIndex = true)
{
  if (bLineWeightByIndex)
  {
    nLineWeight = lineWeightByIndex((char)nLineWeight);
  }
  CString strLineWeight = _T("");
  switch (nLineWeight)
  {
  case OdDb::kLnWtByLayer:
    strLineWeight = _T("Layer");
    break;    
  case OdDb::kLnWtByBlock:
    strLineWeight = _T("Block");
    break;
  case OdDb::kLnWtByLwDefault:
    strLineWeight = _T("Default");
    break;
  default:
    strLineWeight.Format(_T("%1.2f mm"), (float)nLineWeight / 100);
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

/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropManagerDlg dialog


OdaLayerPropManagerDlg::OdaLayerPropManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OdaLayerPropManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(OdaLayerPropManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

OdaLayerPropManagerDlg::OdaLayerPropManagerDlg(OdDbDatabase* pDb, CWnd* pParent /*=NULL*/)
	: CDialog(OdaLayerPropManagerDlg::IDD, pParent)
  , m_pDb(pDb)
{
}


void OdaLayerPropManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OdaLayerPropManagerDlg)
	DDX_Control(pDX, IDC_LAYER_FILTER_TREE, m_TreeFilters);
	DDX_Control(pDX, IDC_LIST_LAYERS_LIST, m_listLayersList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OdaLayerPropManagerDlg, CDialog)
	//{{AFX_MSG_MAP(OdaLayerPropManagerDlg)
	ON_WM_SIZE()
  ON_WM_SIZING()
	ON_WM_CREATE()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_BUTTON_SET_CURRENT, OnButtonSetCurrent)
	ON_NOTIFY(NM_CLICK, IDC_LIST_LAYERS_LIST, OnClickListLayersList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LAYERS_LIST, OnDblclkListLayersList)
	ON_NOTIFY(NM_DBLCLK, IDC_LAYER_FILTER_TREE, OnDblclkLayerFilterTree)
  ON_NOTIFY(NM_CLICK, IDC_LAYER_FILTER_TREE, OnClickLayerFilterTree)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_LAYERS_LIST, OnRclickListLayersList)
	ON_COMMAND(ID_VP_OVR_CLEAR_ALL_FOR_ALL_VP, OnClearAllViewportsOverrides)
	ON_COMMAND(ID_VP_OVR_CLEAR_ALL_FOR_CURR_VP, OnClearAllViewportOverridesForCurrVp)
	ON_COMMAND(ID_VP_OVR_REMOVE_COLOR, OnRemoveViewportColorOverrideForCurrVp)
	ON_COMMAND(ID_VP_OVR_REMOVE_LINETYPE, OnRemoveViewportLinetypeOverrideForCurrVp)
	ON_COMMAND(ID_VP_OVR_REMOVE_LINEWEIGHT, OnRemoveViewportLineweightOverrideForCurrVp)
	ON_COMMAND(ID_VP_OVR_REMOVE_PLOTSTYLE, OnRemoveViewportPlotstyleOverrideForCurrVp)
	ON_COMMAND(ID_VP_OVR_REMOVE_TRANSPARENCY, OnRemoveViewportTransparencyOverrideForCurrVp)
	//}}AFX_MSG_MAP
  ON_NOTIFY(TVN_KEYDOWN, IDC_LAYER_FILTER_TREE, OnTvnKeydownLayerFilterTree)
  ON_BN_CLICKED(IDC_BUTTON_LAYER_STATES, OnButtonLayerStates)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropManagerDlg message handlers

void OdaLayerPropManagerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
  CRect itemRect;
  CRect dlgRect;
  if (m_listLayersList.m_hWnd)
  {
    m_listLayersList.GetWindowRect(&itemRect);
    ScreenToClient(itemRect);
    GetWindowRect(&dlgRect);
    itemRect.right  += (dlgRect.Width() - m_nDeltaWidth);
    itemRect.bottom += (dlgRect.Height() - m_nDeltaHeight);
    m_listLayersList.MoveWindow(itemRect);
  }

  if (GetDlgItem(IDC_STATIC_CURRENT_LAYER))
  {
    GetDlgItem(IDC_STATIC_CURRENT_LAYER)->GetWindowRect(&itemRect);
    ScreenToClient(itemRect);
    GetWindowRect(&dlgRect);
    itemRect.right  += (dlgRect.Width() - m_nDeltaWidth);
    GetDlgItem(IDC_STATIC_CURRENT_LAYER)->MoveWindow(itemRect);
  }

  if (GetDlgItem(IDC_STATIC_LAYER_STATISTIC))
  {
    GetDlgItem(IDC_STATIC_LAYER_STATISTIC)->GetWindowRect(&itemRect);
    ScreenToClient(itemRect);
    GetWindowRect(&dlgRect);
    GetDlgItem(IDC_STATIC_LAYER_STATISTIC)->MoveWindow(itemRect.left
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


void OdaLayerPropManagerDlg::OnSizing(UINT fwSide, LPRECT pRect) 
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
        pRect->top= pRect->bottom - m_nInitHeight;
        break;
    }
  }
  
  CDialog::OnSizing(fwSide, pRect);
}

int OdaLayerPropManagerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
  m_nInitWidth  = lpCreateStruct->cx;
  m_nInitHeight = lpCreateStruct->cy;
	return 0;
}

BOOL OdaLayerPropManagerDlg::OnInitDialog() 
{
  m_pDb->startUndoRecord();

  CDialog::OnInitDialog();
  m_listLayersList.DeleteAllItems();
  m_listLayersList.InsertColumn(column_status, _T("Status"), LVCFMT_LEFT, 15);
  m_listLayersList.InsertColumn(column_name, _T("Name"), LVCFMT_LEFT, 60);
  m_listLayersList.InsertColumn(column_on, _T("On"), LVCFMT_LEFT, 15);
  m_listLayersList.InsertColumn(column_freeze, _T("Freeze in all VP"), LVCFMT_LEFT, 15);
  m_listLayersList.InsertColumn(column_lock, _T("Lock"), LVCFMT_LEFT, 15);
  m_listLayersList.InsertColumn(column_color, _T("Color"), LVCFMT_LEFT, 45);
  m_listLayersList.InsertColumn(column_linetype, _T("Linetype"), LVCFMT_LEFT, 85);
  m_listLayersList.InsertColumn(column_lineweight, _T("Lineweight"), LVCFMT_LEFT, 50);
  m_listLayersList.InsertColumn(column_transparency, _T("Transparency"), LVCFMT_LEFT, 25);
  m_listLayersList.InsertColumn(column_plotstyle, _T("Plot Style"), LVCFMT_LEFT, 50);
  columns_count =
  m_listLayersList.InsertColumn(column_plot, _T("Plot"), LVCFMT_LEFT, 15);
  if(m_pDb->getTILEMODE()==0)
  {
    m_vp = m_pDb->activeViewportId();
    m_listLayersList.InsertColumn(column_vpcolor, _T("VP Freeze"), LVCFMT_LEFT, 15);
    m_listLayersList.InsertColumn(column_vpcolor, _T("VP Color"), LVCFMT_LEFT, 45);
    m_listLayersList.InsertColumn(column_vplinetype, _T("VP Linetype"), LVCFMT_LEFT, 85);
    m_listLayersList.InsertColumn(column_vplineweight, _T("VP Lineweight"), LVCFMT_LEFT, 50);
    m_listLayersList.InsertColumn(column_vptransparency, _T("VP Transparency"), LVCFMT_LEFT, 25);
    columns_count =
    m_listLayersList.InsertColumn(column_vpplotstyle, _T("Plot Style"), LVCFMT_LEFT, 50);
  }
  column_desc = m_listLayersList.InsertColumn(++columns_count, _T("Description"), LVCFMT_LEFT, 80);
  columns_count++;

  OdDbLayerTablePtr pLayers = m_pDb->getLayerTableId().safeOpenObject();
  pLayers->generateUsageData();

  OdDbSymbolTableIteratorPtr pIter = pLayers->newIterator();
  int i = 0;
  for (pIter->start(); !pIter->done(); pIter->step(), ++i)
  {
    m_listLayersList.InsertItem(i, NULL);
    m_listLayersList.SetItemData(i, PTR_TO_DWORDPTR((OdDbStub*)pIter->getRecordId()));
  }

  CString str;
  str.Format(_T("%d Total layers"), m_listLayersList.GetItemCount());
  GetDlgItem(IDC_STATIC_LAYER_STATISTIC)->SetWindowText(str);

  UpdateCurrentLayerInfoField();

  CBitmap bmp; bmp.LoadBitmap( IDB_LAYER_FILTERS );
  m_pTreeImages.Create( 16, 16, ILC_COLOR32, 0, 1 );
  m_pTreeImages.Add( &bmp, RGB( 0, 0, 0 ) );
  m_TreeFilters.SetImageList( &m_pTreeImages, TVSIL_NORMAL );
  bmp.DeleteObject();

  bmp.LoadBitmap( IDB_LAYER_STATES );
  m_stateImages.Create( 12, 12, ILC_COLOR16|ILC_MASK, 0, 1 );
  m_stateImages.Add( &bmp, RGB( 0, 0, 128 ) );

  UpdateFiltersTree();

  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// recursive filter tree filling helper
//
static void updateFilterTree( CTreeCtrl& tree, HTREEITEM hParent, const OdLyLayerFilter* pRoot, const OdLyLayerFilter* pCurrent )
{
  if ( !pRoot ) return;
  HTREEITEM h = tree.InsertItem( pRoot->name(), hParent );
  tree.SetItemData( h, PTR_TO_DWORDPTR((void*)pRoot) );
  int image = pRoot->isIdFilter() ? 2 : 1;
  tree.SetItemImage( h, image, image );
  for ( unsigned int i = 0; i < pRoot->getNestedFilters().length(); i++ ) 
    updateFilterTree( tree, h, pRoot->getNestedFilters()[i], pCurrent );
  if ( pCurrent == pRoot ) tree.SelectItem( h );
}

// Fill filters tree
//
void OdaLayerPropManagerDlg::UpdateFiltersTree()
{
  m_TreeFilters.DeleteAllItems();
  OdLyLayerFilterManagerPtr mgr = ::odlyGetLayerFilterManager( m_pDb );
	OdLyLayerFilterPtr pCurrent;
	if ( mgr->getFilters( m_pRootFilter, pCurrent ) != eOk ) return;
  updateFilterTree( m_TreeFilters, TVI_ROOT, m_pRootFilter, pCurrent );
  m_TreeFilters.SetItemImage( m_TreeFilters.GetRootItem(), 0, 0 );
}

// Show clicked filter properties
//
void OdaLayerPropManagerDlg::OnDblclkLayerFilterTree( NMHDR* /*pNMHDR*/, LRESULT* pResult ) 
{
  if ( HTREEITEM h = m_TreeFilters.GetSelectedItem() )
  {
    OdLyLayerFilter* lf = (OdLyLayerFilter*)(void*)m_TreeFilters.GetItemData( h );
    if ( !lf->dynamicallyGenerated() && !lf->isIdFilter() )
    {
      OdaLayerFilterPropDlg( lf, this ).DoModal();
    }
  }
	*pResult = 0;
}

void OdaLayerPropManagerDlg::UpdateCurrentLayerInfoField()
{
  CString str;
  str.Format(_T("Current Layer: %s"), (LPCTSTR)OdDbSymUtil::getSymbolName(m_pDb->getCLAYER()));
  GetDlgItem(IDC_STATIC_CURRENT_LAYER)->SetWindowText(str);
}

void DrawColorBox(CDC& dc, const RECT& rcItem, const OdCmColor& cmcol) 
{
  CBrush brush(RGB(cmcol.red(), cmcol.green(), cmcol.blue()));
  CGdiObject* pOld = dc.SelectObject(&brush);
  CRect rc(rcItem);
  rc.DeflateRect(1,1,1,1);
  rc.right = rc.left + rc.bottom - rc.top;
  dc.Rectangle(&rc);
  dc.SelectObject(pOld);
  rc.SetRect(rc.right + 1, rcItem.top, rcItem.right, rcItem.bottom);
  if(rc.left <= rcItem.right)
  {
    OdString sTextOut = cmcol.colorNameForDisplay();
    dc.ExtTextOut(rc.left, rc.top, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.getLength(), NULL);
  }
}

void DrawPlotStyle(CDC& dc, const RECT& rcItem, const CString& sTextOut, const OdDbDatabase* pDb) 
{
  if(pDb->getPSTYLEMODE()==1)
  {
    COLORREF old = dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
    dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
    dc.SetTextColor(old);
  }
  else
  {
    dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
  }
}

void OdaLayerPropManagerDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
  if (IDC_LIST_LAYERS_LIST == nIDCtl)
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
        if( lpDrawItemStruct->itemState & ODS_FOCUS )
        {
          dc.DrawFocusRect(rcItem);
        }
        int itemID = lpDrawItemStruct->itemID;
        if( itemID != -1 )
        {
          // The text color is stored as the item data.
          COLORREF rgbText = (lpDrawItemStruct->itemState & ODS_SELECTED) ?
                              ::GetSysColor(COLOR_HIGHLIGHTTEXT) : ::GetSysColor(COLOR_WINDOWTEXT);
          dc.SetBkColor(rgbBkgnd);
          dc.SetTextColor(rgbText);
          for (int labelIndex = 0; labelIndex < columns_count; ++labelIndex)
          {
            m_listLayersList.GetSubItemRect(itemID, labelIndex, LVIR_LABEL, rcItem);
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

void OdaLayerPropManagerDlg::OnButtonSetCurrent()
{
  OdDbLayerTableRecordPtr pLayer;
  if (OpenSelectedLayer(pLayer))
  {
    m_pDb->setCLAYER(pLayer->objectId());
    UpdateCurrentLayerInfoField();
    GetDlgItem(IDC_LIST_LAYERS_LIST)->RedrawWindow();
  }
}

bool hittestSubitem(CListCtrl& listLayersList, int columns_count, const CPoint& pt, int &index, int &labelIndex) 
{
  index = listLayersList.GetTopIndex();
  int last_visible_index = index + listLayersList.GetCountPerPage();
  if (last_visible_index > listLayersList.GetItemCount())
  {
    last_visible_index = listLayersList.GetItemCount();
  }
  
  while (index <= last_visible_index)
  {
    // Get the bounding rectangle of an item. If the mouse
    // location is within the bounding rectangle of the item,
    // you know you have found the item that was being clicked.
    CRect rectItem;
    listLayersList.GetItemRect(index, &rectItem, LVIR_BOUNDS);
    if (rectItem.PtInRect(pt))
    {
      CRect rectColumn;
      for (labelIndex = 0; labelIndex < columns_count; ++labelIndex)
      {
        listLayersList.GetSubItemRect(index, labelIndex, LVIR_LABEL, rectColumn);
        if (rectColumn.PtInRect(pt))
        {
          return true;
        }
      }
      //UINT flag = LVIS_SELECTED | LVIS_FOCUSED;
      //listLayersList.SetItemState(index, flag, flag);
      break;
    }

    // Get the next item in listview control.
    index++;
  }
  return false;
}

void OdaLayerPropManagerDlg::OnClickListLayersList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
  DWORD pos = GetMessagePos();

  CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
  m_listLayersList.ScreenToClient(&pt);

  int index, labelIndex;
  if(hittestSubitem(m_listLayersList, columns_count, pt, index, labelIndex))
    OnClickSubItemLayersList(index, labelIndex);

  *pResult = 0;
}

void OdaLayerPropManagerDlg::OnClickLayerFilterTree(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
  DWORD pos = GetMessagePos();

  CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
  m_TreeFilters.ScreenToClient(&pt);
  if (HTREEITEM h = m_TreeFilters.HitTest(pt))
  {
    m_listLayersList.DeleteAllItems();
    OdLyLayerFilter* lf = (OdLyLayerFilter*)(void*)m_TreeFilters.GetItemData( h );
    OdDbLayerTablePtr pLayers = m_pDb->getLayerTableId().safeOpenObject();
    int i = 0;
    for (OdDbSymbolTableIteratorPtr pIter = pLayers->newIterator(); !pIter->done(); pIter->step())
    {
      OdDbLayerTableRecordPtr layer = pIter->getRecord();
      if (lf->filter(layer))
      {
        m_listLayersList.InsertItem(i, NULL);
        m_listLayersList.SetItemData(i, PTR_TO_DWORDPTR((OdDbStub*)pIter->getRecordId()));
        ++i;
      }
    }
    m_listLayersList.Invalidate();
  }
  *pResult = 0;
}

void OdaLayerPropManagerDlg::OnClickSubItemLayersList(int itemIndex, int labelIndex)
{
  OdDbObjectId Id = (OdDbStub*)DWORDPTR_TO_PTR(m_listLayersList.GetItemData(itemIndex));
  OdDbLayerTableRecordPtr pLayer = Id.safeOpenObject(OdDb::kForWrite);
  m_bClickToColumnName = false;
  switch(labelIndex)
  {
    case column_name:
      m_bClickToColumnName = true;
      return;
    case column_on:
      pLayer->setIsOff(!pLayer->isOff());
      break;
    case column_freeze:
      pLayer->setIsFrozen(!pLayer->isFrozen());
      break;
    case column_lock:
      pLayer->setIsLocked(!pLayer->isLocked());
      break;
    case column_color:
      {
        OdaLayerPropColorDlg dlg(pLayer->colorIndex());
        if (IDOK == dlg.DoModal())
        {
          pLayer->setColorIndex((OdInt16)dlg.m_nColor);
        }
      }
      break;
    case column_linetype:
      {
        OdaLayerPropLineTypeDlg dlg(m_pDb, pLayer->linetypeObjectId(), true);
        if (IDOK == dlg.DoModal())
        {
          pLayer->setLinetypeObjectId(dlg.m_originalId);
        }
      }      
      break;
    case column_lineweight:
      {
        OdaLayerPropertiesLineWeightDlg dlg(pLayer->lineWeight());
        if (IDOK == dlg.DoModal())
        {
          pLayer->setLineWeight(dlg.m_nLineWeight);
        }
      }
      break;
    case column_transparency:
      {
        OdaLayerPropTransparencyDlg dlg(OdRoundToLong(DBL_NEG(pLayer->transparency().alphaPercent()) * 10));
        if (IDOK == dlg.DoModal())
        {
          pLayer->setTransparency(OdCmTransparency(DBL_NEG((double)dlg.m_nAlpha / 10)));
        }
      }
      break;
    case column_plotstyle:
      break;
    case column_plot:
      pLayer->setIsPlottable(!pLayer->isPlottable());
      break;
    case column_vpfreeze:
    case column_descr:
      if(labelIndex!=column_desc)
      {
        OdDbViewportPtr pVp = OdDbViewport::cast(pLayer->database()->
          activeViewportId().safeOpenObject(OdDb::kForWrite));
        if(pVp.get())
        {
          OdDbObjectIdArray ids(1);
          ids.append(Id);
          if(pVp->isLayerFrozenInViewport(Id))
            pVp->thawLayersInViewport(ids);
          else
            pVp->freezeLayersInViewport(ids);
        }
      }
      else
      {
        OdString descText = pLayer->description();
        if (CSimpleInputDlg().DoInput(_T("Layer Description"), _T("Enter new layer description:"), &descText))
          pLayer->setDescription(descText);
      }
      break;
    case column_vpcolor:
      {
        OdaLayerPropColorDlg dlg(pLayer->colorIndex());
        if (IDOK == dlg.DoModal())
        {
          OdCmColor col;
          col.setColorIndex((OdInt16)dlg.m_nColor);
          pLayer->setColor(col, m_vp);
        }
      }
      break;
    case column_vplinetype:
      {
        OdaLayerPropLineTypeDlg dlg(m_pDb, pLayer->linetypeObjectId(), true);
        if (IDOK == dlg.DoModal())
        {
          pLayer->setLinetypeObjectId(dlg.m_originalId, m_vp);
        }
      }      
      break;
    case column_vplineweight:
      {
        OdaLayerPropertiesLineWeightDlg dlg(pLayer->lineWeight());
        if (IDOK == dlg.DoModal())
        {
          pLayer->setLineWeight(dlg.m_nLineWeight, m_vp);
        }
      }      
      break;
    case column_vptransparency:
      {
        OdaLayerPropTransparencyDlg dlg(OdRoundToLong(DBL_NEG(pLayer->transparency().alphaPercent()) * 10));
        if (IDOK == dlg.DoModal())
        {
          pLayer->setTransparency(OdCmTransparency(DBL_NEG((double)dlg.m_nAlpha / 10)), m_vp);
        }
      }
      break;
    case column_vpplotstyle:
      break;
  }
  m_listLayersList.Invalidate();
}

void OdaLayerPropManagerDlg::DrawItem(CDC& dc, int itemID, int labelIndex, const RECT& rcItem) 
{
  CString sTextOut;
  OdDbObjectId Id = (OdDbStub*)DWORDPTR_TO_PTR(m_listLayersList.GetItemData(itemID));
  OdDbLayerTableRecordPtr pLayer = Id.safeOpenObject();
  switch (labelIndex)
  {
    case column_status:
      if(pLayer->objectId()==Id.database()->getCLAYER())
        m_stateImages.Draw(&dc, 8, ((CRect&)rcItem).TopLeft(), ILD_TRANSPARENT);
      else if(pLayer->isInUse())
        m_stateImages.Draw(&dc, 9, ((CRect&)rcItem).TopLeft(), ILD_TRANSPARENT);
      break;
    case column_name:
      sTextOut = (LPCTSTR)pLayer->getName();
      dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
      break;
    case column_on:
      m_stateImages.Draw(&dc, pLayer->isOff() ? 3 : 2, ((CRect&)rcItem).TopLeft(), ILD_TRANSPARENT);
      break;
    case column_freeze:
      m_stateImages.Draw(&dc, pLayer->isFrozen() ? 4 : 5, ((CRect&)rcItem).TopLeft(), ILD_TRANSPARENT);
      break;
    case column_lock:
      m_stateImages.Draw(&dc, pLayer->isLocked() ? 0 : 1, ((CRect&)rcItem).TopLeft(), ILD_TRANSPARENT);
      break;
    case column_color:
      ::DrawColorBox(dc, rcItem, pLayer->color());
      break;
    case column_linetype:
      sTextOut = (LPCTSTR)OdDbSymUtil::getSymbolName(pLayer->linetypeObjectId());
      dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
      break;
    case column_lineweight:
      sTextOut = StringByLineWeight(pLayer->lineWeight(), false);
      dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
      break;
    case column_transparency:
      sTextOut.Format(_T("%d"), OdRoundToLong(DBL_NEG(pLayer->transparency().alphaPercent()) * 10) * 10);
      dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
      break;
    case column_plotstyle:
      sTextOut = (LPCTSTR)pLayer->plotStyleName();
      ::DrawPlotStyle(dc, rcItem, sTextOut, m_pDb);
      break;
    case column_plot:
      m_stateImages.Draw(&dc, pLayer->isPlottable() ? 6 : 7, ((CRect&)rcItem).TopLeft(), ILD_TRANSPARENT);
      break;
    case column_descr:
    case column_vpfreeze:
      if(labelIndex!=column_desc)
      {
        OdDbViewportPtr pVp = OdDbViewport::cast(pLayer->database()->activeViewportId().safeOpenObject());
        if(pVp.get())
          m_stateImages.Draw(&dc, pVp->isLayerFrozenInViewport(Id) ? 4 : 5, ((CRect&)rcItem).TopLeft(), ILD_TRANSPARENT);
      }
      else
      {
        sTextOut = (LPCTSTR)pLayer->description();
        dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
      }
      break;
    case column_vpcolor:
      ::DrawColorBox(dc, rcItem, pLayer->color(m_vp));
      break;
    case column_vplinetype:
      sTextOut = (LPCTSTR)OdDbSymUtil::getSymbolName(pLayer->linetypeObjectId(m_vp));
      dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
      break;
    case column_vplineweight:
      sTextOut = StringByLineWeight(pLayer->lineWeight(m_vp), false);
      dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
      break;
    case column_vptransparency:
      sTextOut.Format(_T("%d"), OdRoundToLong(DBL_NEG(pLayer->transparency(m_vp).alphaPercent()) * 10) * 10);
      dc.ExtTextOut(rcItem.left+6, rcItem.top+1, ETO_CLIPPED, &rcItem, sTextOut, sTextOut.GetLength(), NULL);
      break;
    case column_vpplotstyle:
      sTextOut = (LPCTSTR)pLayer->plotStyleName(m_vp);
      ::DrawPlotStyle(dc, rcItem, sTextOut, m_pDb);
      break;
  }
}

void OdaLayerPropManagerDlg::OnCancel() 
{
	m_pDb->undo();
	CDialog::OnCancel();
}

void OdaLayerPropManagerDlg::OnDblclkListLayersList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	if (m_bClickToColumnName)
  {
    OnButtonSetCurrent();    
  }
	*pResult = 0;
}


/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropertiesLineWeightDlg dialog

OdaLayerPropertiesLineWeightDlg::OdaLayerPropertiesLineWeightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OdaLayerPropertiesLineWeightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(OdaLayerPropertiesLineWeightDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

OdaLayerPropertiesLineWeightDlg::OdaLayerPropertiesLineWeightDlg(int nOrigLineWeight, CWnd* pParent)
  : CDialog(OdaLayerPropertiesLineWeightDlg::IDD, pParent)
  , m_nOrigLineWeight(nOrigLineWeight)
  , m_nLineWeight((OdDb::LineWeight)nOrigLineWeight)
{
  
}

void OdaLayerPropertiesLineWeightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OdaLayerPropertiesLineWeightDlg)
	DDX_Control(pDX, IDC_LIST_LINEWEIGHT, m_listLineweight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OdaLayerPropertiesLineWeightDlg, CDialog)
	//{{AFX_MSG_MAP(OdaLayerPropertiesLineWeightDlg)
	ON_LBN_DBLCLK(IDC_LIST_LINEWEIGHT, OnDblclkListLineweight)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropertiesLineWeightDlg message handlers



BOOL OdaLayerPropertiesLineWeightDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  m_listLineweight.InsertString(0, StringByLineWeight(OdDb::kLnWtByLwDefault, false));
  m_listLineweight.SetItemData(0, (OdUInt32)OdDb::kLnWtByLwDefault);
  for (int index = 1; index < 25; ++index)
  {
    m_listLineweight.InsertString(index, StringByLineWeight(index - 1));
    m_listLineweight.SetItemData(index, lineWeightByIndex((char)(index - 1)));
  }
  m_listLineweight.SelectString(-1, StringByLineWeight(m_nOrigLineWeight, false));
  CString str;
  str.Format(_T("Original : %s"), StringByLineWeight(m_nOrigLineWeight, false));
  GetDlgItem(IDC_STATIC_LINEWEIGHT_ORIGINAL)->SetWindowText(str);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void OdaLayerPropertiesLineWeightDlg::OnOK() 
{
  int index = m_listLineweight.GetCurSel();
	m_nLineWeight = (OdDb::LineWeight)m_listLineweight.GetItemData(index);
	CDialog::OnOK();
}

void OdaLayerPropertiesLineWeightDlg::OnDblclkListLineweight() 
{
	// TODO: Add your control notification handler code here
  OnOK();	
}
/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropLineTypeDlg dialog


OdaLayerPropLineTypeDlg::OdaLayerPropLineTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OdaLayerPropLineTypeDlg::IDD, pParent)
  , m_pDb(NULL)
  , m_bHideByLayerByBlock(false)
{
	//{{AFX_DATA_INIT(OdaLayerPropLineTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

OdaLayerPropLineTypeDlg::OdaLayerPropLineTypeDlg(OdDbDatabase *pDb, const OdDbObjectId& originalId, bool bHideByLayerByBlock, CWnd* pParent)
  : CDialog(OdaLayerPropLineTypeDlg::IDD, pParent)
  , m_pDb(pDb)
  , m_originalId(originalId)
  , m_bHideByLayerByBlock(bHideByLayerByBlock)
{

}

void OdaLayerPropLineTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OdaLayerPropLineTypeDlg)
	DDX_Control(pDX, IDC_LIST_LINETYPE, m_listLinetype);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OdaLayerPropLineTypeDlg, CDialog)
	//{{AFX_MSG_MAP(OdaLayerPropLineTypeDlg)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LINETYPE, OnButtonLoadLinetype)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LINETYPE, OnButtonSaveLinetype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropLineTypeDlg message handlers

BOOL OdaLayerPropLineTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if (!m_pDb)
  {
    return FALSE;
  }

  m_listLinetype.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_listLinetype.InsertColumn(0, _T("Linetype"), LVCFMT_LEFT, 140);
  m_listLinetype.InsertColumn(1, _T("Description"), LVCFMT_LEFT, 200);

  FillLinetypesList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void OdaLayerPropLineTypeDlg::OnOK() 
{
  m_listLinetype.SetFocus();
  int index = m_listLinetype.GetSelectionMark();
  if (index > -1)
  {
  	m_originalId = (OdDbStub*)DWORDPTR_TO_PTR(m_listLinetype.GetItemData(index));
  }
	CDialog::OnOK();
}

void OdaLayerPropLineTypeDlg::OnButtonLoadLinetype() 
{
  CLoadLineTypes dlg(m_pDb);
  dlg.DoModal();
  FillLinetypesList();
}

void OdaLayerPropLineTypeDlg::OnButtonSaveLinetype() 
{
    CString newName;
    bool bReplace = true;
    if (DoPromptFileName( newName,
              bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
              OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST, FALSE ))
    {
        OdStreamBufPtr pFile = 
           odSystemServices()->createFile(OdString((LPCTSTR)newName), Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
        odDbSaveLineTypeFile(*m_pDb, *pFile);
    }

}

BOOL OdaLayerPropLineTypeDlg::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog)
{

  CFileDialog dlgFile(bOpenFileDialog);

  CString title;
  VERIFY(title.LoadString(nIDSTitle));

  dlgFile.m_ofn.Flags |= lFlags;

  CString strFilter;
  CString strDefault;

  strFilter = _T("lin file format (*.lin)");
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += _T("*.lin");
  strFilter += (TCHAR)'\0';   // last string
  dlgFile.m_ofn.nMaxCustFilter++;
  dlgFile.m_ofn.nFilterIndex = 1;

  dlgFile.m_ofn.lpstrFilter = strFilter;
  dlgFile.m_ofn.lpstrTitle = title;
  dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

  int nResult = dlgFile.DoModal();
  fileName.ReleaseBuffer();

  if( fileName.IsEmpty() ) return FALSE;

  int pos = fileName.ReverseFind('.');
  if( pos >= 0 )
  {
       fileName = fileName.Left(pos);
  }

  fileName += _T(".lin");

  return nResult == IDOK;
}


void OdaLayerPropLineTypeDlg::FillLinetypesList()
{
  m_listLinetype.DeleteAllItems();
  OdDbLinetypeTablePtr pLinetypes = m_pDb->getLinetypeTableId().safeOpenObject();
  OdDbLinetypeTableIteratorPtr pIter = pLinetypes->newIterator();
  OdDbLinetypeTableRecordPtr pLineTypeRec;
  int i = 0;
  for (pIter->start(); !pIter->done(); pIter->step())
  {
    OdDbObjectId id = pIter->getRecordId();
    if(m_bHideByLayerByBlock)
    {
      if(id==m_pDb->getLinetypeByBlockId() || id==m_pDb->getLinetypeByLayerId())
         continue;
    }
    m_listLinetype.InsertItem(i, NULL);
    m_listLinetype.SetItemData(i, PTR_TO_DWORDPTR((OdDbStub*)id));
    pLineTypeRec = id.safeOpenObject();
    m_listLinetype.SetItemText(i, 0, pLineTypeRec->getName());
    m_listLinetype.SetItemText(i, 1, pLineTypeRec->comments());
    ++i;
  }

  LVFINDINFO info;
  info.flags = LVFI_PARAM;
  info.lParam = PTR_TO_DWORDPTR((OdDbStub*)m_originalId);

  int index = m_listLinetype.FindItem(&info);
  m_listLinetype.SetItem(index, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, LVIS_SELECTED, 0);

}



/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropColorDlg dialog


OdaLayerPropColorDlg::OdaLayerPropColorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OdaLayerPropColorDlg::IDD, pParent)
  , m_nColor(7)
{
	//{{AFX_DATA_INIT(OdaLayerPropColorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

OdaLayerPropColorDlg::OdaLayerPropColorDlg(int nColor, CWnd* pParent /*=NULL*/)
	: CDialog(OdaLayerPropColorDlg::IDD, pParent)
  , m_nColor(nColor)
{
}

void OdaLayerPropColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OdaLayerPropColorDlg)
	DDX_Control(pDX, IDC_LIST_COLORS, m_listColors);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OdaLayerPropColorDlg, CDialog)
	//{{AFX_MSG_MAP(OdaLayerPropColorDlg)
	ON_LBN_DBLCLK(IDC_LIST_COLORS, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropColorDlg message handlers

BOOL OdaLayerPropColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  CString str;
  for (int index = 1; index <= PALETTE_SIZE; ++index)
  {
    str.Empty();
    str.Format(_T("Color_%d"), index);
    m_listColors.InsertString(index - 1, str);
  }

  str.Format(_T("Color_%d"), m_nColor);
  m_listColors.SelectString(-1, str);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void OdaLayerPropColorDlg::OnOK() 
{
  m_nColor = m_listColors.GetCurSel() + 1;
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropTransparencyDlg dialog

OdaLayerPropTransparencyDlg::OdaLayerPropTransparencyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OdaLayerPropTransparencyDlg::IDD, pParent)
  , m_nAlpha(0)
{
	//{{AFX_DATA_INIT(OdaLayerPropTransparencyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

OdaLayerPropTransparencyDlg::OdaLayerPropTransparencyDlg(int nAlpha, CWnd* pParent /*=NULL*/)
	: CDialog(OdaLayerPropTransparencyDlg::IDD, pParent)
  , m_nAlpha(nAlpha)
{
}

void OdaLayerPropTransparencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OdaLayerPropTransparencyDlg)
	DDX_Control(pDX, IDC_LIST_COLORS, m_listTransps);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(OdaLayerPropTransparencyDlg, CDialog)
	//{{AFX_MSG_MAP(OdaLayerPropTransparencyDlg)
	ON_LBN_DBLCLK(IDC_LIST_COLORS, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OdaLayerPropTransparencyDlg message handlers

BOOL OdaLayerPropTransparencyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  CString str;
  for (int index = 0; index < 10; index++)
  {
    str.Empty();
    str.Format(_T("%d"), index * 10);
    m_listTransps.InsertString(index, str);
  }

  str.Format(_T("%d"), m_nAlpha * 10);
  m_listTransps.SelectString(-1, str);

  SetWindowText(_T("Layer Transparency"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void OdaLayerPropTransparencyDlg::OnOK() 
{
  m_nAlpha = m_listTransps.GetCurSel();
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// OdaLayerFilterPropDlg dialog
//

OdaLayerFilterPropDlg::OdaLayerFilterPropDlg( OdLyLayerFilter* lf, CWnd* pParent )
  : CDialog( IDD, pParent ), m_pFilter( lf ) 
{
}

BEGIN_MESSAGE_MAP(OdaLayerFilterPropDlg, CDialog)
	//{{AFX_MSG_MAP(OdaLayerPropColorDlg)
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDOK, &OdaLayerFilterPropDlg::OnOK)
END_MESSAGE_MAP()

void OdaLayerFilterPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OdaLayerPropColorDlg)
  DDX_Text( pDX, IDC_EDIT_LF_NAME, m_sName );
  DDX_Text( pDX, IDC_EDIT_LF_DEFINITION, m_sDef );
	//}}AFX_DATA_MAP
}

BOOL OdaLayerFilterPropDlg::OnInitDialog()
{
  m_sName = (LPCTSTR)m_pFilter->name();
  m_sDef = (LPCTSTR)m_pFilter->filterExpression();
  UpdateData( FALSE );
  if ( !m_pFilter->allowRename() )
    static_cast<CEdit*>( GetDlgItem( IDC_EDIT_LF_NAME ) )->SetReadOnly();
	return TRUE;  
}

void OdaLayerFilterPropDlg::OnOK()
{
  UpdateData( TRUE );

  if ( m_pFilter->allowRename() && m_sName != (LPCTSTR)m_pFilter->name())
    m_pFilter->setName( (LPCTSTR)m_sName );
  
  if ( m_sDef != (LPCTSTR)m_pFilter->filterExpression() )
    m_pFilter->setFilterExpression( (LPCTSTR)m_sDef );
  EndDialog(IDOK);
}

void OdaLayerPropManagerDlg::OnTvnKeydownLayerFilterTree(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
  if ( pTVKeyDown->wVKey == VK_DELETE )
  {
    if ( HTREEITEM h = m_TreeFilters.GetSelectedItem() )
    {
      OdLyLayerFilter* lf = (OdLyLayerFilter*)(void*)m_TreeFilters.GetItemData( h );
      if ( lf->dynamicallyGenerated() ) return;
      if ( AfxMessageBox(_T("Delete this filter?"), MB_YESNO) != IDYES ) return;
      lf->parent()->removeNested( lf );
      m_TreeFilters.DeleteItem( h );
      // save changes
      OdLyLayerFilter* root = (OdLyLayerFilter*)(void*)m_TreeFilters.GetItemData( m_TreeFilters.GetRootItem() );
      ::odlyGetLayerFilterManager( m_pDb )->setFilters( root, root );
    }
  }
  *pResult = 0;
}

void OdaLayerPropManagerDlg::OnButtonLayerStates()
{
  LayerStatesDialog(m_pDb, this).DoModal();
}

void OdaLayerPropManagerDlg::OnButtonAdd() 
{
  OdDbLayerTablePtr pLayers = m_pDb->getLayerTableId().safeOpenObject(OdDb::kForWrite);
  OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::createObject();

  OdString str;
  int n = 1;
  do
  {
    str.format(OD_T("Layer%d"), n++);
  }
  while(pLayers->has(str));
  pLayer->setName(str);
  pLayers->add(pLayer);

  int i = m_listLayersList.GetItemCount();
  m_listLayersList.InsertItem(i, NULL);
  m_listLayersList.SetItemData(i, PTR_TO_DWORDPTR((OdDbStub*)pLayer->objectId()));
}

void OdaLayerPropManagerDlg::OnRclickListLayersList(NMHDR* pNMHDR, LRESULT* pResult) 
{
  DWORD pos = GetMessagePos();

  CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
  m_listLayersList.ScreenToClient(&pt);

  int index, labelIndex;
  if(hittestSubitem(m_listLayersList, columns_count, pt, index, labelIndex))
  {
  }
  CMenu menu;
  menu.LoadMenu(IDR_LAYER_POPUP_MENU);
  menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, GET_X_LPARAM(pos), GET_Y_LPARAM(pos), this);

  *pResult = 0;
}

bool OdaLayerPropManagerDlg::OpenSelectedLayer(OdSmartPtr<OdDbLayerTableRecord>& pLayer) 
{
  int nSelectionMark = m_listLayersList.GetSelectionMark();
  if (nSelectionMark > -1)
  {
    OdDbObjectId id = (OdDbStub*)DWORDPTR_TO_PTR(m_listLayersList.GetItemData(nSelectionMark));
    pLayer = id.openObject(OdDb::kForWrite);
    return !pLayer.isNull();
  }
  return false;
}

void OdaLayerPropManagerDlg::OnClearAllViewportsOverrides() 
{
  OdDbLayerTableRecordPtr pLayer;
  if (OpenSelectedLayer(pLayer))
  {
    pLayer->removeAllOverrides();
    GetDlgItem(IDC_LIST_LAYERS_LIST)->RedrawWindow();
  }
}

void OdaLayerPropManagerDlg::OnClearAllViewportOverridesForCurrVp() 
{
  OdDbLayerTableRecordPtr pLayer;
  if (OpenSelectedLayer(pLayer))
  {
    pLayer->removeViewportOverrides(m_vp);
    GetDlgItem(IDC_LIST_LAYERS_LIST)->RedrawWindow();
  }
}

void OdaLayerPropManagerDlg::OnRemoveViewportColorOverrideForCurrVp() 
{
  OdDbLayerTableRecordPtr pLayer;
  if (OpenSelectedLayer(pLayer))
  {
    pLayer->removeColorOverride(m_vp);
    GetDlgItem(IDC_LIST_LAYERS_LIST)->RedrawWindow();
  }
}

void OdaLayerPropManagerDlg::OnRemoveViewportLinetypeOverrideForCurrVp() 
{
  OdDbLayerTableRecordPtr pLayer;
  if (OpenSelectedLayer(pLayer))
  {
    pLayer->removeLinetypeOverride(m_vp);
    GetDlgItem(IDC_LIST_LAYERS_LIST)->RedrawWindow();
  }
}

void OdaLayerPropManagerDlg::OnRemoveViewportLineweightOverrideForCurrVp() 
{
  OdDbLayerTableRecordPtr pLayer;
  if (OpenSelectedLayer(pLayer))
  {
    pLayer->removeLineWeightOverride(m_vp);
    GetDlgItem(IDC_LIST_LAYERS_LIST)->RedrawWindow();
  }
}

void OdaLayerPropManagerDlg::OnRemoveViewportPlotstyleOverrideForCurrVp() 
{
  OdDbLayerTableRecordPtr pLayer;
  if (OpenSelectedLayer(pLayer))
  {
    pLayer->removePlotStyleOverride(m_vp);
    GetDlgItem(IDC_LIST_LAYERS_LIST)->RedrawWindow();
  }
}

void OdaLayerPropManagerDlg::OnRemoveViewportTransparencyOverrideForCurrVp() 
{
  OdDbLayerTableRecordPtr pLayer;
  if (OpenSelectedLayer(pLayer))
  {
    pLayer->removeTransparencyOverride(m_vp);
    GetDlgItem(IDC_LIST_LAYERS_LIST)->RedrawWindow();
  }
}
