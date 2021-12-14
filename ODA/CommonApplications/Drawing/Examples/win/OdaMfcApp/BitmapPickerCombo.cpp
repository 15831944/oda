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
#include "BitmapPickerCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
                   
//----------------------------------------------------------------------------
// Function DrawBitmap
// @func    draws bitmap at specified point in specified device context
// @rdesc   nothing
// @parm    const CBitmap | *bitmap | bitmap to draw
// @parm    const CDC     | *pDC    | device context to draw in
// @parm    const CPoint  | &point  | top left point of bitmap
//----------------------------------------------------------------------------
static void DrawBitmap(const CBitmap *bitmap, const CDC *pDC, const CPoint &point)
{
  BITMAP bm; ((CBitmap*)bitmap)->GetBitmap(&bm);
  int w = bm.bmWidth; 
  int h = bm.bmHeight;
  CDC memDC; memDC.CreateCompatibleDC((CDC*)pDC);
  CBitmap *pBmp = memDC.SelectObject((CBitmap*)bitmap);
  ((CDC*)pDC)->BitBlt(point.x, point.y, w, h, &memDC, 0, 0, SRCCOPY);
  memDC.SelectObject(pBmp);
}

//----------------------------------------------------------------------------
// Function DrawBitmap
// @func    draws bitmap centered in specified rectangle in specified device context
// @rdesc   nothing
// @parm    const CBitmap | *bitmap | bitmap to draw
// @parm    const CDC     | *pDC    | device context to draw in
// @parm    const CRect   | &rect   | rectangle to center in
//----------------------------------------------------------------------------
static void DrawBitmap(const CBitmap *bitmap, const CDC *pDC, const CRect &rect)
{
  BITMAP bm; ((CBitmap*)bitmap)->GetBitmap(&bm);
  int w = bm.bmWidth; 
  int h = bm.bmHeight;
  CPoint point;
  point.x = rect.left + ((rect.right - rect.left) / 2) - (w / 2);
  point.y = rect.top + ((rect.bottom - rect.top) / 2) - (h / 2);
  DrawBitmap(bitmap, pDC, point);
}

CBitmapPickerCombo::CBitmapPickerCombo():
  CComboBox(),
    m_nItemWidth(0),
    m_nItemHeight(0)
{
}

int CBitmapPickerCombo::AddBitmap(const CBitmap *bitmap, LPCTSTR string)
{
  return InsertBitmap(GetCount(), bitmap, string);
}

int CBitmapPickerCombo::InsertBitmap(int nIndex, const CBitmap *bitmap, LPCTSTR string)
{
  int n = CComboBox::InsertString(nIndex, string ? string : _T(""));
  if(!bitmap) return n;

  if (n != CB_ERR && n != CB_ERRSPACE) {
      SetItemData(n, (DWORD_PTR)bitmap);
      BITMAP bm; ((CBitmap*)bitmap)->GetBitmap(&bm);
      SetSize(bm.bmWidth, bm.bmHeight);
  }
  return n;
}

void CBitmapPickerCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{ 
  lpMIS->itemWidth = (m_nItemWidth + 2);
  lpMIS->itemHeight = (m_nItemHeight + 2);
}

void CBitmapPickerCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
  CDC* pDC = CDC::FromHandle(lpDIS->hDC);

  if (!IsWindowEnabled()) {
    CBrush brDisabled(RGB(192,192,192)); // light gray
    CBrush* pOldBrush = pDC->SelectObject(&brDisabled);
    CPen penDisabled(PS_SOLID, 1, RGB(192,192,192));
    CPen* pOldPen = pDC->SelectObject(&penDisabled);
    OutputBitmap(lpDIS, false);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
    return;
  }

  // Selected
  if ((lpDIS->itemState & ODS_SELECTED) 
    && (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
    CBrush brHighlight(::GetSysColor(COLOR_HIGHLIGHT)); 
    CBrush* pOldBrush = pDC->SelectObject(&brHighlight);
    CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT));
    CPen* pOldPen = pDC->SelectObject(&penHighlight);
    pDC->Rectangle(&lpDIS->rcItem);
    pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
    pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    OutputBitmap(lpDIS, true);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
  }

  // De-Selected
  if (!(lpDIS->itemState & ODS_SELECTED) 
    && (lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE))) {
    CBrush brWindow(::GetSysColor(COLOR_WINDOW)); 
    CBrush* pOldBrush = pDC->SelectObject(&brWindow);
    CPen penHighlight(PS_SOLID, 1, ::GetSysColor(COLOR_WINDOW));
    CPen* pOldPen = pDC->SelectObject(&penHighlight);
    pDC->Rectangle(&lpDIS->rcItem);
    pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
    pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
    OutputBitmap(lpDIS, false);
    pDC->SelectObject(pOldBrush);
    pDC->SelectObject(pOldPen);
  }

  // Focus
  if (lpDIS->itemAction & ODA_FOCUS) 
    pDC->DrawFocusRect(&lpDIS->rcItem);
}

void CBitmapPickerCombo::OutputBitmap(LPDRAWITEMSTRUCT lpDIS, bool selected)
{
  const CBitmap *bitmap = (const CBitmap*)(lpDIS->itemData);
  if (bitmap && bitmap != (const CBitmap *)(OdIntToPtr(-1))) {
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);
    CString string; 
    if (lpDIS->itemID != -1) 
      GetLBText(lpDIS->itemID, string); 
    if (string.IsEmpty()) 
      DrawBitmap(bitmap, pDC, lpDIS->rcItem);
    else {
      CPoint point;
      point.x = lpDIS->rcItem.left + 2;
      point.y = lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2); 
      DrawBitmap(bitmap, pDC, point);
      CRect rcText(lpDIS->rcItem); 
      rcText.DeflateRect(m_nItemWidth + 4, 0, 0, 0);
      pDC->DrawText(string, rcText, DT_SINGLELINE |DT_VCENTER ); 
    }
  }
  if(!bitmap)
  {
    CDC* pDC = CDC::FromHandle(lpDIS->hDC);
    CString string; 
    if (lpDIS->itemID != -1) 
      GetLBText(lpDIS->itemID, string); 
      CPoint point;
      point.x = lpDIS->rcItem.left + 2;
      point.y = lpDIS->rcItem.top + ((lpDIS->rcItem.bottom - lpDIS->rcItem.top) / 2) - (m_nItemHeight / 2); 
      CRect rcText(lpDIS->rcItem); 
      pDC->DrawText(string, rcText, DT_SINGLELINE |DT_VCENTER ); 
  }
}               

void CBitmapPickerCombo::SetSize(int width, int height)
{
  if (width > m_nItemWidth)
    m_nItemWidth = width;
  if (height > m_nItemHeight)
    m_nItemHeight = height;
  for (int i = -1; i < GetCount(); i++) 
    SetItemHeight(i, m_nItemHeight + 6);
}

/*void CBitmapPickerCombo::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	//Let the parent do its thing
	CComboBox::DeleteItem(lpDeleteItemStruct);
}*/


#ifdef _DEBUG
void CBitmapPickerCombo::PreSubclassWindow() 
{
  CComboBox::PreSubclassWindow();

  // ensure some styles are set
  // modifying style here has NO effect!?!
  ASSERT(GetStyle() & CBS_DROPDOWNLIST);
  ASSERT(GetStyle() & CBS_OWNERDRAWVARIABLE);
  ASSERT(GetStyle() & CBS_HASSTRINGS);
}
#endif

//----------------------------------------------------------------------------
