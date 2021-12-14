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

// PsTbEditFormViewPg.cpp : implementation file
//

#include "stdafx.h"
#include "PsTbEditFormViewPg.h"
#include "Ps/PlotStyles.h"
#include "ColorMapping.h"
#include "DynamicLinker.h"
#include "PsEditLineweightDlg.h"


#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "WindowsX.h"

void Dlg_OnClose(HWND hwnd) 
{
   DestroyWindow(hwnd);
}

void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) 
{
  switch(id)
  {
    case IDOK:
      {
        const int nMaxCount = 100;
        TCHAR sString[nMaxCount];
        HWND hOwner = ::GetWindow(hwnd, GW_OWNER);
        ::GetWindowText(::GetDlgItem(hwnd, IDC_PS_ADDPS_EDIT_PSNAME), sString, nMaxCount);
        CPropertySheet *pPsDlg = (CPropertySheet *)CWnd::FromHandle(hOwner);
        CPsTbEditFormViewPage *pPg = (CPsTbEditFormViewPage*)pPsDlg->GetActivePage();
        pPg->AddNewPlotStyle(sString);
      }
    case IDCANCEL:
      EndDialog(hwnd, id);
      break;
    case IDC_PS_ADDPS_EDIT_PSNAME:
      {
        if( codeNotify == EN_CHANGE)
        {
          const int nMaxCount = 100;
          TCHAR sString[nMaxCount];
          HWND hOwner = ::GetWindow(hwnd, GW_OWNER);
          ::GetWindowText(::GetDlgItem(hwnd, IDC_PS_ADDPS_EDIT_PSNAME), sString, nMaxCount);
          CPropertySheet *pPsDlg = (CPropertySheet *)CWnd::FromHandle(hOwner);
          CPsTbEditFormViewPage *pPg = (CPsTbEditFormViewPage*)pPsDlg->GetActivePage();
          const OdPsPlotStyleTable *pPsTab = pPg->GetPlotStyleTable();
          OdPsPlotStylePtr pPs;
          HWND hDInfo = ::GetDlgItem(hwnd, IDC_PS_ADDPS_STATIC_DINFO);
          HWND hSInfo = ::GetDlgItem(hwnd, IDC_PS_ADDPS_STATIC_SINFO);
          HWND hOkBtn = ::GetDlgItem(hwnd, IDOK);
          CString newName = sString;
          newName.MakeLower();
          if(newName.IsEmpty())
          {
            EnableWindow(hOkBtn, FALSE);
            return;
          }
          for(UINT i = 0; i < pPsTab->plotStyleSize(); i++)
          {
            pPs = pPsTab->plotStyleAt(i);
            CString name = (LPCTSTR)pPs->localizedName();
            name.MakeLower();
            if(name == newName)
            {
              CString sInfo;
              sInfo.Format(_T("A style named <%s> already exists."), sString);
              ::SetWindowText(hDInfo, sInfo);
              ShowWindow(hDInfo, SW_SHOW);
              ShowWindow(hSInfo, SW_SHOW);
              EnableWindow(hOkBtn, FALSE);
              return;
            }
          }
        ShowWindow(hDInfo, SW_HIDE);
        ShowWindow(hSInfo, SW_HIDE);
        EnableWindow(hOkBtn, TRUE);
        break;
        }
      }
  }
}

BOOL Dlg_OnInit(HWND hwnd, HWND hwndCtl, LPARAM lParam)
{
  HWND hOwner = ::GetWindow(hwnd, GW_OWNER);
  CPropertySheet *pPsDlg = (CPropertySheet *)CWnd::FromHandle(hOwner);
  CPsTbEditFormViewPage *pPg = (CPsTbEditFormViewPage*)pPsDlg->GetActivePage();
  const OdPsPlotStyleTable *pPsTab = pPg->GetPlotStyleTable();
  OdString sName;
  sName.format(OD_T("Style %d"), pPsTab->plotStyleSize());
  ::SetWindowText(::GetDlgItem(hwnd, IDC_PS_ADDPS_EDIT_PSNAME), sName);
  return TRUE;
}

int WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) 
   {
   case WM_CLOSE: 
     return SetDlgMsgResult(hwnd, uMsg, 
       HANDLE_WM_CLOSE(hwnd, wParam, lParam, Dlg_OnClose));
   case WM_COMMAND: 
     return SetDlgMsgResult(hwnd, uMsg, 
       HANDLE_WM_COMMAND(hwnd, wParam, lParam, Dlg_OnCommand));
   case WM_INITDIALOG: 
     return SetDlgMsgResult(hwnd, uMsg, 
       HANDLE_WM_INITDIALOG(hwnd, wParam, lParam, Dlg_OnInit));
   }

   return(FALSE);
}

void CBitmapColorInfo::GetBitmapSizes(CBitmap &Bmp, int &W, int &H)
{
    BITMAP bi;
    Bmp.GetBitmap(&bi);
    W = bi.bmWidth;
    H = bi.bmHeight;
}

DIBCOLOR *CBitmapColorInfo::GetBitmapPixels(CBitmap &Bmp, int &W, int &H)
{
    CDC dcMem; dcMem.CreateCompatibleDC(NULL);
    GetBitmapSizes(Bmp, W, H);
    BITMAPINFO bi;
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = W;
    bi.bmiHeader.biHeight = -H;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = H*W*4;
    bi.bmiHeader.biClrUsed = 0;
    bi.bmiHeader.biClrImportant = 0;
    void *buf = new char[H*W*4];
    GetDIBits(dcMem.m_hDC, (HBITMAP)Bmp.m_hObject, 0, H, buf, &bi, DIB_RGB_COLORS);
    return (DIBCOLOR*)buf;
}

void CBitmapColorInfo::SetBitmapPixels(CBitmap &Bmp, DIBCOLOR *pPixels)
{
    CDC dcMem; dcMem.CreateCompatibleDC(NULL);
    int W, H;
    GetBitmapSizes(Bmp, W, H);
    BITMAPINFO bi;
    bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
    bi.bmiHeader.biWidth = W;
    bi.bmiHeader.biHeight = -H;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = H*W*4;
    bi.bmiHeader.biClrUsed = 0;
    bi.bmiHeader.biClrImportant = 0;
    SetDIBits(dcMem.m_hDC, (HBITMAP)Bmp.m_hObject, 0, H, pPixels, &bi, DIB_RGB_COLORS); 
    delete pPixels;
}

CBitmap* CBitmapColorInfo::CloneBitmap(const CBitmap* pBmpSource, CBitmap* pBmpClone)
{
  ASSERT(pBmpClone);
  ASSERT(pBmpSource);
  ASSERT(pBmpSource != pBmpClone);
  if(!pBmpClone && !pBmpSource && (pBmpSource == pBmpClone)) return NULL;

  BITMAP bmp; 
  DWORD dw;
  BYTE *pb;
  ((CBitmap*)pBmpSource)->GetBitmap(&bmp); 

  CClientDC dc(NULL);
  CDC cdc;
  cdc.CreateCompatibleDC(&dc);
  pBmpClone->CreateCompatibleBitmap(&dc, bmp.bmWidth, bmp.bmHeight);
  
  dw = bmp.bmWidthBytes*bmp.bmHeight;
  pb = new BYTE[dw];
  dw = pBmpSource->GetBitmapBits(dw, pb); 
  pBmpClone->SetBitmapBits(dw, pb);
  delete[]pb;

  int W, H; 
  DIBCOLOR *buf = GetBitmapPixels(*(CBitmap*)pBmpSource, W, H);
  SetBitmapPixels(*pBmpClone, buf);

  return pBmpClone;
}

void CBitmapColorInfo::PaintBitmap(CBitmap &Bmp, COLORREF color)
{
  int W, H; 
  DIBCOLOR *buf = GetBitmapPixels(Bmp, W, H);

  DIBCOLOR *pColor = buf;
  for(int y = H-1; y >= 0; y--)
    for(int x = 0; x < W; x++, pColor++)
      *pColor = DIBCOLOR(color);

  SetBitmapPixels(Bmp, buf);
}

const OdCmEntityColor CBitmapColorInfo::GetColor()
{

  OdCmEntityColor color = OdCmEntityColor((OdUInt8)((m_color >> 16) & 0xFF), 
              (OdUInt8)((m_color >> 8) & 0xFF), (OdUInt8)(m_color & 0xFF));
  return color;
}

const bool CBitmapColorInfo::IsColor(COLORREF color, BYTE item)
{
  color = (item << 24) + (GetRValue(color) << 16) + (GetGValue(color) << 8) + (GetBValue(color));
  return m_color == color;
}

CBitmapColorInfo::CBitmapColorInfo(const CBitmap *pBitmap, COLORREF color, BYTE cColorItem, int colorIndex)
: m_iItem(cColorItem)
{

  m_color = (m_iItem << 24) + (GetRValue(color) << 16) + (GetGValue(color) << 8) + (GetBValue(color));
  CloneBitmap(pBitmap, &m_bitmap);
  PaintBitmap(m_bitmap, color);
  if (colorIndex <= 0)
  {
    _tcscpy(m_name, _T("Custom Color"));
  }
  else
  {
    OdString clrName;
    clrName.format(L"Color %d", colorIndex);
    _tcscpy(m_name, clrName.c_str());
  }
}

CBitmapColorInfo::CBitmapColorInfo(const CBitmap *pBitmap, COLORREF color, const TCHAR* name)
: m_iItem(0xff)
{
  m_color = (m_iItem << 24) + (GetRValue(color) << 16) + (GetGValue(color) << 8) + (GetBValue(color));
  CloneBitmap(pBitmap, &m_bitmap);
  PaintBitmap(m_bitmap, color);
  _tcsncpy(m_name, name, PS_COLOR_MAX_NAME);
}

CBitmapColorInfo::CBitmapColorInfo(LPCTSTR lpszResourceName, const TCHAR* name)
: m_iItem(0xff)
{
  HBITMAP hBmp ;
  hBmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(), 
          lpszResourceName, IMAGE_BITMAP, 13,13, LR_CREATEDIBSECTION ); 
  CBitmap *bitmap = CBitmap::FromHandle(hBmp);
  CloneBitmap(bitmap, &m_bitmap);
  _tcsncpy(m_name, name, PS_COLOR_MAX_NAME);
}

const int CPsListStyleData::getPublicArrayIndexByColor(COLORREF color)
{

  for (UINT i = 0; i < m_pPublicBitmapList->size(); i++)
  {
    OdCmEntityColor cl = OdCmEntityColor(GetRValue(color), GetGValue(color), GetBValue(color));
    if((*m_pPublicBitmapList)[i]->GetColor() == cl)
    {
      return i;
    }
  }
  return -1;

}

inline COLORREF getLutColor(OdInt16 idx)
{
  COLORREF rgb = RGB(OdCmEntityColor::mLUT[idx][0], OdCmEntityColor::mLUT[idx][1], OdCmEntityColor::mLUT[idx][2]);
  if (idx == OdCmEntityColor::kACIforeground)
    rgb = (rgb & 0xFF000000) | (~rgb & 0x00FFFFFF);
  return rgb;
}

CPsListStyleData::CPsListStyleData(OdPsPlotStyle* pPs, OdBitmapColorInfoArray* pPublicBitmapList, const char item) 
  : m_pPlotStyles(pPs), m_pPublicBitmapList(pPublicBitmapList), m_pBitmapColorInfo(NULL)
{
  if(!m_pPlotStyles && !m_pPublicBitmapList) return;
  OdPsPlotStyleData OdPsData;
  pPs->getData(OdPsData);
  OdCmEntityColor cL = OdPsData.color();

  OdUInt32 rgb;
  if (cL.isByACI())
    rgb = ::getLutColor(cL.colorIndex());
  else
    rgb = RGB(cL.red(), cL.green(), cL.blue());
  m_iActiveListIndex = getPublicArrayIndexByColor(rgb);

  if(m_iActiveListIndex < 0) 
    m_pBitmapColorInfo = new CBitmapColorInfo(&(*m_pPublicBitmapList)[m_pPublicBitmapList->size()-1]->m_bitmap, rgb, item, cL.isByACI() ? cL.colorIndex() : -1); 
}

CPsListStyleData::~CPsListStyleData()
{
  delete m_pBitmapColorInfo;
  m_pBitmapColorInfo = 0;
}

const bool CPsListStyleData::SetActiveListIndex(const int index, const bool bBmpInfo)
{
  if( !m_pPlotStyles && !m_pPublicBitmapList) return false;
  if((UINT)index >= m_pPublicBitmapList->size()-1) return false;
  if(index < 0) return false;
  m_iActiveListIndex = index;
  if(bBmpInfo) return true;
  delete m_pBitmapColorInfo;
  m_pBitmapColorInfo = 0;
  return true;
}

const bool CPsListStyleData::ReplaceBitmapColorInfo(COLORREF color, const int item)
{
  if( !m_pPlotStyles && !m_pPublicBitmapList) return false;

  delete m_pBitmapColorInfo;
  m_pBitmapColorInfo = 0;

  m_iActiveListIndex = getPublicArrayIndexByColor(color);

  if(m_iActiveListIndex < 0) 
    m_pBitmapColorInfo = new CBitmapColorInfo(&(*m_pPublicBitmapList)[m_pPublicBitmapList->size()-1]->m_bitmap, color, (BYTE)item); 

  return true;

}

const OdCmEntityColor CPsListStyleData::GetColor()
{
  if(m_iActiveListIndex < 0)
    return m_pBitmapColorInfo->GetColor();

  return (*m_pPublicBitmapList)[m_iActiveListIndex]->GetColor();

}


/////////////////////////////////////////////////////////////////////////////
// CPsTbEditFormViewPage property page

IMPLEMENT_DYNCREATE(CPsTbEditFormViewPage, CPropertyPage)

CPsTbEditFormViewPage::CPsTbEditFormViewPage() : CPropertyPage(CPsTbEditFormViewPage::IDD)
{
	//{{AFX_DATA_INIT(CPsTbEditFormViewPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

  m_pPlotStyleTable = 0;
  m_pPlotStyleActive = 0;
  m_bEditChanging = false;
}

CPsTbEditFormViewPage::~CPsTbEditFormViewPage()
{
}

void CPsTbEditFormViewPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbTreeSettingPage1Dlg)
      	DDX_Control(pDX, IDC_PS_FORMVIEW_COMBO_DITHER,        m_comboDither);
      	DDX_Control(pDX, IDC_PS_FORMVIEW_COMBO_GRAYSCALE,     m_comboGrayscale);
  	    DDX_Control(pDX, IDC_PS_FORMVIEW_COMBO_ADAPTIVE,      m_comboAdaptive);
  	    DDX_Control(pDX, IDC_PS_FORMVIEW_COMBO_LINETYPE,      m_comboLinetype);
  	    DDX_Control(pDX, IDC_PS_FORMVIEW_COMBO_LINEWEIGHT,    m_comboLineweight);
  	    DDX_Control(pDX, IDC_PS_FORMVIEW_COMBO_LINEENDSTYLE,  m_comboLineendstyle);
  	    DDX_Control(pDX, IDC_PS_FORMVIEW_COMBO_LINEJOINSTYLE, m_comboLinejoinstyle);
  	    DDX_Control(pDX, IDC_PS_FORMVIEW_COMBO_FILLSTYLE,     m_comboFillstyle);
  	    DDX_Control(pDX, IDC_PS_FORMVIEW_COMBO_COLOR,         m_comboColor);
      	DDX_Control(pDX, IDC_PS_FORMVIEW_EDIT_DESCRIPTION,    m_editDescription);
  	    DDX_Control(pDX, IDC_PS_FORMVIEW_SPIN_PEN,            m_spinPen);
      	DDX_Control(pDX, IDC_PS_FORMVIEW_EDIT_PEN,            m_editPen);
      	DDX_Control(pDX, IDC_PS_FORMVIEW_SPIN_VIRTPEN,        m_spinVirtpen);
       	DDX_Control(pDX, IDC_PS_FORMVIEW_EDIT_VIRTPEN,        m_editVirtpen);
      	DDX_Control(pDX, IDC_PS_FORMVIEW_SPIN_SCREENING,      m_spinScreening);
      	DDX_Control(pDX, IDC_PS_FORMVIEW_EDIT_SCREENING,      m_editScreening);
  	    DDX_Control(pDX, IDC_PS_FORMVIEW_LIST_STYLES,         m_listStyles);
      	DDX_Control(pDX, IDC_PS_FORMVIEW_BTN_ADDSTYLE,        m_btnAddstyle);
      	DDX_Control(pDX, IDC_PS_FORMVIEW_BTN_DELSTYLE,        m_btnDelstyle);
        DDX_Control(pDX, IDC_PS_FORMVIEW_BTN_LINEWEIGHT,      m_btnLineweight);
        DDX_Control(pDX, IDC_PS_FORMVIEW_BTN_SAVE,            m_btnSave);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
    
    m_spinPen.SetBuddy(&m_editPen);
    m_spinPen.SetRange(0, PS_SPIN_MAX_PEN);

    m_spinVirtpen.SetBuddy(&m_editVirtpen);
    m_spinVirtpen.SetRange(0, PS_SPIN_MAX_VIRTPEN);

    m_spinScreening.SetBuddy(&m_editScreening);
    m_spinScreening.SetRange(0, PS_SPIN_MAX_SCREENING);

}

void CPsTbEditFormViewPage::OnDestroy( )
{
  int f;
  for(f=0; f<m_listStyles.GetItemCount(); ++f)
  {
    CPsListStyleData* pPsListStyleData = (CPsListStyleData*)(m_listStyles.GetItemData(f));
    delete pPsListStyleData;
  }

  for(f=0; f<(int)m_bitmapList.size(); ++f)
  {
    delete m_bitmapList[f];
  }

  CPropertyPage::OnDestroy();
}

BEGIN_MESSAGE_MAP(CPsTbEditFormViewPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPsTbEditFormViewPage)
		// NOTE: the ClassWizard will add message map macros here
	    ON_BN_CLICKED   (                  IDC_PS_FORMVIEW_BTN_LINEWEIGHT,      OnLineweightBtn             )
	    ON_BN_CLICKED   (                  IDC_PS_FORMVIEW_BTN_SAVE,            OnSaveBtn                   )
	    ON_BN_CLICKED   (                  IDC_PS_FORMVIEW_BTN_DELSTYLE,        OnDelBtnStyle               )
	    ON_BN_CLICKED   (                  IDC_PS_FORMVIEW_BTN_ADDSTYLE,        OnAddBtnStyle               )
    	ON_EN_UPDATE    (                  IDC_PS_FORMVIEW_EDIT_DESCRIPTION,    OnUpdateEditDescription     )
    	ON_EN_CHANGE    (                  IDC_PS_FORMVIEW_EDIT_DESCRIPTION,    OnChangeEditDescription     )
    	ON_EN_CHANGE    (                  IDC_PS_FORMVIEW_EDIT_VIRTPEN,        OnChangeEditVirtPen         )
    	ON_EN_CHANGE    (                  IDC_PS_FORMVIEW_EDIT_PEN,            OnChangeEditPen             )
    	ON_EN_CHANGE    (                  IDC_PS_FORMVIEW_EDIT_SCREENING,      OnChangeEditScreening       )
    	ON_NOTIFY       (LVN_ITEMCHANGED,  IDC_PS_FORMVIEW_LIST_STYLES,         OnItemchangedListStyles     )
	    ON_NOTIFY       (LVN_ITEMCHANGING, IDC_PS_FORMVIEW_LIST_STYLES,         OnItemchangingListStyles    )
    	ON_NOTIFY       (UDN_DELTAPOS,     IDC_PS_FORMVIEW_SPIN_PEN,            OnDeltaposSpinPen           )
    	ON_CBN_SELCHANGE(                  IDC_PS_FORMVIEW_COMBO_COLOR,         OnSelchangeComboColor       )
    	ON_CBN_SELENDOK (                  IDC_PS_FORMVIEW_COMBO_COLOR,         OnSelendokComboColor        )
      ON_CBN_SELCHANGE(                  IDC_PS_FORMVIEW_COMBO_DITHER,        OnSelendokComboDither       )
      ON_CBN_SELCHANGE(                  IDC_PS_FORMVIEW_COMBO_GRAYSCALE,     OnSelendokComboGrayScale    )
      ON_CBN_SELCHANGE(                  IDC_PS_FORMVIEW_COMBO_LINETYPE,      OnSelendokComboLineType     )
      ON_CBN_SELCHANGE(                  IDC_PS_FORMVIEW_COMBO_ADAPTIVE,      OnSelendokComboAdaptive     )
      ON_CBN_SELCHANGE(                  IDC_PS_FORMVIEW_COMBO_LINEWEIGHT,    OnSelendokComboLineWeight   )
      ON_CBN_SELCHANGE(                  IDC_PS_FORMVIEW_COMBO_LINEENDSTYLE,  OnSelendokComboLineEndStyle )
      ON_CBN_SELCHANGE(                  IDC_PS_FORMVIEW_COMBO_LINEJOINSTYLE, OnSelendokComboLineJoinStyle)
      ON_CBN_SELCHANGE(                  IDC_PS_FORMVIEW_COMBO_FILLSTYLE,     OnSelendokComboFillStyle    )
    //ON_WM_PAINT()
      ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CPsTbEditFormViewPage::initAdaptiveComboBox()
{
  m_comboAdaptive.AddString(_T("On"));
  m_comboAdaptive.AddString(_T("Off"));
  m_comboAdaptive.SelectString(-1,_T("On"));
}

void CPsTbEditFormViewPage::initGrayscaleComboBox()
{
  m_comboGrayscale.AddString(_T("On"));
  m_comboGrayscale.AddString(_T("Off"));
  m_comboGrayscale.SelectString(-1,_T("On"));
}

void CPsTbEditFormViewPage::initDitherComboBox()
{
  m_comboDither.AddString(_T("On"));
  m_comboDither.AddString(_T("Off"));
  m_comboDither.SelectString(-1,_T("On"));
}

void CPsTbEditFormViewPage::initLinetypeComboBox()
{

  for(int i = 0; i < 32; i++)
  {
    m_comboLinetype.AddString(StringLineType[i]);
  }
  m_comboLinetype.SetCurSel(31);
}

void CPsTbEditFormViewPage::initLineweightComboBox()
{
  m_comboLineweight.AddString(_T("Use object lineweight"));
  const bool bInch = m_pPlotStyleTable->isDisplayCustomLineweightUnits();
  CString sUnits = bInch ? _T("''") : _T(" mm");
  for(unsigned int i = 0; i < m_pPlotStyleTable->lineweightSize(); i++)
  {
    CString lineweight;
    lineweight.Format(_T("%.4f%s"), bInch ? MMTOINCH(m_pPlotStyleTable->getLineweightAt(i)) : m_pPlotStyleTable->getLineweightAt(i), sUnits);
    m_comboLineweight.AddString(lineweight);
  }
  m_comboLineweight.SetCurSel(0);
}

void CPsTbEditFormViewPage::initLineendstyleComboBox()
{
  for(int i = 0; i < 5; i++)
  {
    m_comboLineendstyle.AddString(StringLineEndStyle[i]);
  }
  m_comboLineendstyle.SetCurSel(0);
}

void CPsTbEditFormViewPage::initLinejoinstyleComboBox()
{
  for(int i = 0; i < 5; i++)
  {
    m_comboLinejoinstyle.AddString(StringLineJoinStyle[i]);
  }
  m_comboLinejoinstyle.SetCurSel(0);
}

void CPsTbEditFormViewPage::initFillstyleComboBox()
{
  for(int i = 0; i < 10; i++)
  {
    m_comboFillstyle.AddString(StringFillStyle[i]);
  }
  m_comboFillstyle.SetCurSel(0);
}

void CPsTbEditFormViewPage::initColorComboBox()
{
  int item = -1;

  for (unsigned int i = 0; i < m_bitmapList.size(); i++)
  {
    if( !i )
      item = m_comboColor.AddBitmap(NULL, m_bitmapList[i]->m_name);
    else
      item = m_comboColor.AddBitmap(&m_bitmapList[i]->m_bitmap, m_bitmapList[i]->m_name);
    m_bitmapList[i]->m_iItem = (BYTE)item;
  }

  m_comboColor.SetCurSel(0);
}

void CPsTbEditFormViewPage::OnItemchangedListStyles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
  if(!pNMListView->uNewState) 
  {
  	*pResult = 0;
    return;
  }

  m_bEditChanging = true;

  CPsListStyleData *pPsListStyleData = (CPsListStyleData*)(m_listStyles.GetItemData(pNMListView->iItem));

  m_pPlotStyleActive = pPsListStyleData->GetOdPsPlotStyle();

  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);

  m_editDescription.SetWindowText(m_pPlotStyleActive->description());

  m_comboDither.SelectString(-1, OdPsData.isDitherOn() ? _T("On") : _T("Off"));

  m_comboGrayscale.SelectString(-1, OdPsData.isGrayScaleOn() ? _T("On") : _T("Off"));

  m_spinPen.SetPos(OdPsData.physicalPenNumber());

  m_spinVirtpen.SetPos(OdPsData.virtualPenNumber());

  m_spinScreening.SetPos(OdPsData.screening());

  m_comboAdaptive.SelectString(-1,OdPsData.isAdaptiveLinetype() ? _T("On") : _T("Off"));

  m_comboLinetype.SetCurSel(OdPsData.linetype());

  m_comboLineweight.SetCurSel((OdUInt32)OdPsData.lineweight());

  m_comboLineendstyle.SetCurSel(OdPsData.endStyle());

  m_comboLinejoinstyle.SetCurSel(OdPsData.joinStyle() < 5 ? OdPsData.joinStyle() : 4);

  m_comboFillstyle.SetCurSel(OdPsData.fillStyle() - 64);

  deleteCustomColor();
  m_comboColor.SetCurSel(appendCustomColor(pNMListView->iItem));

  m_bEditChanging = false;

  if(!m_pPlotStyleTable->isAciTableAvailable())
  {
    m_btnAddstyle.EnableWindow(TRUE);
    CWnd *pChildWnd = GetWindow(GW_CHILD);
    pChildWnd = pChildWnd->GetWindow(GW_HWNDFIRST);
    if(!pNMListView->iItem)
    {
      while(pChildWnd)
      {
        pChildWnd->EnableWindow(FALSE);
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
      }

      m_btnDelstyle.EnableWindow(FALSE);
      m_listStyles.EnableWindow(TRUE);
    }
    else
    {
      while(pChildWnd)
      {
        pChildWnd->EnableWindow(TRUE);
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
      }
    }

    m_btnLineweight.EnableWindow(TRUE);
    m_btnSave.EnableWindow(TRUE);
    m_btnAddstyle.EnableWindow(TRUE);
  }


	*pResult = 0;
}

void CPsTbEditFormViewPage::OnChangeEditScreening() 
{
  if(m_bEditChanging) return;
  m_bEditChanging = true;

  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  CString pVal;
  m_editScreening.GetWindowText(pVal);
  int num;
  if(pVal == _T("Automatic"))
    pVal = _T("0");
  _stscanf(pVal, _T("%d"), &num);
  if(num < 0 || num > PS_SPIN_MAX_PEN)
  {
    num = 0;
    m_spinScreening.SetPos(num);
  }
  OdPsData.setPhysicalPenNumber((OdInt16)num);
  m_pPlotStyleActive->setData(OdPsData);

  if(!m_spinScreening.GetPos())
  {
    m_editScreening.SetWindowText(_T("Automatic"));
  }
  else
  {
    CString buffer;
    buffer.Format(_T("%d"), num);
    m_editScreening.SetWindowText(buffer);
  }

  m_bEditChanging = false;
}

void CPsTbEditFormViewPage::OnChangeEditPen() 
{
  if(m_bEditChanging) return;
  m_bEditChanging = true;

  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  CString pVal;
  m_editPen.GetWindowText(pVal);
  int num;
  if(pVal == _T("Automatic"))
    pVal = _T("0");
  _stscanf(pVal, _T("%d"), &num);
  if(num < 0 || num > PS_SPIN_MAX_PEN)
  {
    num = 0;
    m_spinPen.SetPos(num);
  }
  OdPsData.setPhysicalPenNumber((OdInt16)num);
  m_pPlotStyleActive->setData(OdPsData);

  if(!m_spinPen.GetPos())
  {
    m_editPen.SetWindowText(_T("Automatic"));
  }
  else
  {
    TCHAR buffer[256];
    _itot(num, buffer, 10);
    m_editPen.SetWindowText(buffer);
  }

  m_bEditChanging = false;
}

void CPsTbEditFormViewPage::OnChangeEditVirtPen() 
{
  if(m_bEditChanging) return;
  m_bEditChanging = true;

  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  CString pVal;
  m_editVirtpen.GetWindowText(pVal);
  int num;
  if(pVal == _T("Automatic"))
    pVal = _T("0");
  _stscanf(pVal, _T("%d"), &num);
  if(num < 0 || num > PS_SPIN_MAX_VIRTPEN)
  {
    num = 0;
    m_spinVirtpen.SetPos(num);
  }
  OdPsData.setVirtualPenNumber((OdInt16)num);
  m_pPlotStyleActive->setData(OdPsData);

  if(!m_spinVirtpen.GetPos())
  {
    m_editVirtpen.SetWindowText(_T("Automatic"));
  }
  else
  {
    TCHAR buffer[256];
    _itot(num, buffer, 10);
    m_editVirtpen.SetWindowText(buffer);
  }

  m_bEditChanging = false;

}


void CPsTbEditFormViewPage::OnChangeEditDescription()
{
  if(m_bEditChanging) return;
  m_bEditChanging = true;

  int iItem = m_listStyles.GetSelectionMark();
  if(iItem < 0) 
  {
    return;
  }

  CPsListStyleData *pPsListStyleData = (CPsListStyleData*)(m_listStyles.GetItemData(iItem));
  OdPsPlotStyle* pPs = pPsListStyleData->GetOdPsPlotStyle();
  CString pVal;
  m_editDescription.GetWindowText(pVal);
#ifdef _UNICODE
  pPs->setDescription(OdString(pVal));
#else
  pPs->setDescription(OdString(pVal));
#endif

  m_bEditChanging = false;
}

void CPsTbEditFormViewPage::OnUpdateEditDescription()
{

}


void CPsTbEditFormViewPage::OnDeltaposSpinPen(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CPsTbEditFormViewPage::OnItemchangingListStyles(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
  if(!pNMListView->uNewState) 
  {
  	*pResult = 0;
    return;
  }

  int iLastItem = m_listStyles.GetSelectionMark();
  if(iLastItem < 0) *pResult = 0;

  int iItem = m_listStyles.GetSelectionMark();
  if(iItem < 0) 
  {
    *pResult = 0;
    return;
  }

  CPsListStyleData *pPsListStyleData = (CPsListStyleData*)(m_listStyles.GetItemData(iItem));
  pPsListStyleData->SetActiveListIndex(m_comboColor.GetCurSel());

	*pResult = 0;
}

const bool CPsTbEditFormViewPage::SetPlotStyleTable(OdPsPlotStyleTable* pPlotStyleTable)
{
  if(!pPlotStyleTable) return false;
  m_pPlotStyleTable = pPlotStyleTable;
  return true;
}

HICON CPsTbEditFormViewPage::initColorIcon(int width,int height, COLORREF color)
{
	ICONINFO ii;
	ii.fIcon=TRUE;
	HDC hScreenDC=::GetDC(NULL);
	HDC hIconDC=CreateCompatibleDC(hScreenDC);
	HDC hMaskDC=CreateCompatibleDC(hScreenDC);

	ii.xHotspot=0;
	ii.yHotspot=0;	
	ii.hbmColor=CreateCompatibleBitmap(hScreenDC,width,height);	
	ii.hbmMask=CreateCompatibleBitmap(hMaskDC,width,height);
	
  ::ReleaseDC(NULL,hScreenDC);
		
	HGDIOBJ hOldIconDC=::SelectObject(hIconDC,ii.hbmColor);
	HGDIOBJ hOldMaskDC=::SelectObject(hMaskDC,ii.hbmMask);	
	
	BitBlt(hIconDC,0,0,width,height,NULL,0,0,WHITENESS);
	BitBlt(hMaskDC,0,0,width,height,NULL,0,0,BLACKNESS);	
	
	RECT r={0,0,width,height};	
	HBRUSH hBR=CreateSolidBrush(color);
	FillRect(hIconDC,&r,hBR);
	DeleteObject(hBR);
	
	SelectObject(hIconDC,hOldIconDC);
	SelectObject(hMaskDC,hOldMaskDC);

	HICON hIcon=CreateIconIndirect(&ii);

	//Cleanup
	DeleteObject(ii.hbmColor);
	DeleteObject(ii.hbmMask);
	DeleteDC(hMaskDC);
	DeleteDC(hIconDC);

	return hIcon;
}

void CPsTbEditFormViewPage::initImageList()
{
  m_imageList.Create(16, 16, ILC_COLORDDB/*ILC_COLOR32*/, 0, 0);
  
  unsigned int uiPlotStyleSize = m_pPlotStyleTable->plotStyleSize();
  const ODCOLORREF *pPal = odcmAcadLightPalette();
  for( unsigned int j = 0; j < uiPlotStyleSize; j++ )
  {
    m_imageList.Add(initColorIcon(16, 16, ::getLutColor((OdInt16)j + 1)));
  }
  
  if(m_pPlotStyleTable->isAciTableAvailable())
    m_listStyles.SetImageList(&m_imageList, LVSIL_SMALL);
  
}

void CPsTbEditFormViewPage::initListCtrl()
{

  LV_COLUMN lvColumn;
  memset(&lvColumn, 0, sizeof(LV_COLUMN));
  lvColumn.mask = LVCF_FMT|LVCF_TEXT;
  lvColumn.fmt = LVCFMT_CENTER;
  m_listStyles.InsertColumn(1, &lvColumn);
    
  unsigned int uiPlotStyleSize = m_pPlotStyleTable->plotStyleSize();
  for( unsigned int j = 0; j < uiPlotStyleSize; j++ )
  {
     insertItem(j);
  }
}

const int CPsTbEditFormViewPage::insertItem(int index)
{
  m_listStyles.LockWindowUpdate();	// ***** lock window updates while filling list *****

  OdPsPlotStyle *pPs = (m_pPlotStyleTable->plotStyleAt(index)).get();

  LV_ITEM lvItem;
  memset(&lvItem, 0, sizeof(LV_ITEM));
  lvItem.mask = m_pPlotStyleTable->isAciTableAvailable() 
    ? LVIF_TEXT|LVIF_IMAGE|LVIF_STATE
    : LVIF_TEXT|LVIF_STATE;
  lvItem.state = 0;
  lvItem.stateMask = 0;
  if(m_pPlotStyleTable->isAciTableAvailable())
    lvItem.iImage = index;
  lvItem.iItem = index;
  lvItem.iSubItem = 0;

  OdString str = pPs->localizedName();
  lvItem.pszText = (LPTSTR)(LPCTSTR)str;

  int nItem = m_listStyles.InsertItem(&lvItem);

  CPsListStyleData *pPsListStyleData =
    new CPsListStyleData(pPs, &m_bitmapList, (char)nItem);

  m_listStyles.SetItemData(nItem, (LPARAM)pPsListStyleData);

 	m_listStyles.UnlockWindowUpdate();	// ***** unlock window updates *****

  return nItem;
}

void CPsTbEditFormViewPage::initBitmapList()
{

  CBitmapColorInfo *pBitmapColorInfo = new CBitmapColorInfo(MAKEINTRESOURCE(IDB_SELECT_TRUE_COLOR), _T("Select true color..."));
  CBitmap *bitmapSrc = &(pBitmapColorInfo->m_bitmap); 

  m_bitmapList.push_back(
     new CBitmapColorInfo(bitmapSrc, RGB(255,255,255), _T("Use object color")));

  m_bitmapList.push_back(
     new CBitmapColorInfo(bitmapSrc, RGB(255,0,0), _T("Red")));

  m_bitmapList.push_back(
     new CBitmapColorInfo(bitmapSrc, RGB(255,255,0), _T("Yellow")));

  m_bitmapList.push_back(
     new CBitmapColorInfo(bitmapSrc, RGB(0,255,0), _T("Green")));

  m_bitmapList.push_back(
     new CBitmapColorInfo(bitmapSrc, RGB(0,255,255), _T("Cyan")));

  m_bitmapList.push_back(
     new CBitmapColorInfo(bitmapSrc, RGB(0,0,255), _T("Blue")));

  m_bitmapList.push_back(
     new CBitmapColorInfo(bitmapSrc, RGB(255,0,255), _T("Magenta")));

  m_bitmapList.push_back(
     new CBitmapColorInfo(bitmapSrc, RGB(0,0,0), _T("Black")));

  m_bitmapList.push_back(pBitmapColorInfo);
}

BOOL CPsTbEditFormViewPage::OnInitDialog() 
{

	CPropertyPage::OnInitDialog();

  if(!m_pPlotStyleTable) return FALSE;
	
  initBitmapList();
  initImageList();
  initListCtrl();
  initGrayscaleComboBox();
  initDitherComboBox();
  initAdaptiveComboBox();
  initLinetypeComboBox();
  initLineweightComboBox();
  initLineendstyleComboBox();
  initLinejoinstyleComboBox();
  initFillstyleComboBox();
  initColorComboBox();

  SetWindowLong(m_listStyles.m_hWnd, GWL_STYLE, WS_VISIBLE|WS_CHILD|WS_BORDER|LVS_SMALLICON|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_AUTOARRANGE);
	
  ListView_SetItemState(m_listStyles.m_hWnd, 0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);


  if(m_pPlotStyleTable->isAciTableAvailable())
  {
    m_btnAddstyle.EnableWindow(FALSE);
    m_btnDelstyle.EnableWindow(FALSE);
  }
  else
  {
    m_btnAddstyle.EnableWindow(TRUE);
    m_btnDelstyle.EnableWindow(FALSE);
  }


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPsTbEditFormViewPage::OnSelchangeComboColor() 
{
	// TODO: Add your control notification handler code here
  OdInt16 intColorPolicy = 1;
	int index = m_comboColor.GetCurSel();
  int cListStylesItem = m_listStyles.GetSelectionMark();
  CPsListStyleData *pPsListStyleData = (CPsListStyleData*)(m_listStyles.GetItemData(cListStylesItem));

  if(index == m_comboColor.GetCount()-1)
  {
    CColorDialog dlgColor;
    if(dlgColor.DoModal() == IDOK)
    {
      deleteCustomColor();

      COLORREF color = dlgColor.GetColor();
      m_comboColor.SetCurSel(replaceCustomColor(color, cListStylesItem));
      intColorPolicy = 3;
    }
  }
  else
  {
      pPsListStyleData->SetActiveListIndex(index);
      if(index) intColorPolicy = 5;
  }

  OdCmEntityColor color = pPsListStyleData->GetColor();
  // m_pPlotStyleActive->setColorPolicy(intColorPolicy);


  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);

  OdPsData.setColor(color);
  m_pPlotStyleActive->setData(OdPsData);

}

void CPsTbEditFormViewPage::OnSelendokComboColor() 
{
}

void CPsTbEditFormViewPage::OnSelendokComboDither()
{
	int index = m_comboDither.GetCurSel();
  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  OdPsData.setDitherOn(index == 0 ? true : false);
  m_pPlotStyleActive->setData(OdPsData);
}

void CPsTbEditFormViewPage::OnSelendokComboGrayScale()
{
	int index = m_comboGrayscale.GetCurSel();
  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  OdPsData.setGrayScaleOn(index == 0 ? true : false);
  m_pPlotStyleActive->setData(OdPsData);
}

void CPsTbEditFormViewPage::OnSelendokComboAdaptive()
{
	int index = m_comboAdaptive.GetCurSel();
  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  OdPsData.setAdaptiveLinetype(index == 0 ? true : false);
  m_pPlotStyleActive->setData(OdPsData);
}

void CPsTbEditFormViewPage::OnSelendokComboLineWeight()
{
	int index = m_comboLineweight.GetCurSel();
  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  OdPsData.setLineweight(index);
  m_pPlotStyleActive->setData(OdPsData);
}

void CPsTbEditFormViewPage::OnSelendokComboLineEndStyle()
{
	int index = m_comboLineendstyle.GetCurSel();
  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  OdPsData.setEndStyle(OdPs::LineEndStyle(index));
  m_pPlotStyleActive->setData(OdPsData);
}

void CPsTbEditFormViewPage::OnSelendokComboFillStyle()
{
	int index = m_comboFillstyle.GetCurSel();
  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  OdPsData.setFillStyle(OdPs::FillStyle(index+64));
  m_pPlotStyleActive->setData(OdPsData);
}

void CPsTbEditFormViewPage::OnSelendokComboLineJoinStyle()
{
	int index = m_comboLinejoinstyle.GetCurSel();
  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  OdPsData.setJoinStyle(OdPs::LineJoinStyle(index));
  m_pPlotStyleActive->setData(OdPsData);
}

void CPsTbEditFormViewPage::OnSelendokComboLineType()
{
	int index = m_comboLinetype.GetCurSel();
  OdPsPlotStyleData OdPsData;
  m_pPlotStyleActive->getData(OdPsData);
  OdPsData.setLinetype(OdPs::LineType(index));
  m_pPlotStyleActive->setData(OdPsData);
}

void CPsTbEditFormViewPage::OnDelBtnStyle() 
{
	// TODO: Add your control notification handler code here
  int iItem = m_listStyles.GetSelectionMark();

  m_pPlotStyleActive = m_pPlotStyleTable->delPlotStyle(m_pPlotStyleActive);
  CPsListStyleData* pPsListStyleData = (CPsListStyleData*)(m_listStyles.GetItemData(iItem));
  m_listStyles.DeleteItem(iItem);
  delete pPsListStyleData;
  m_listStyles.SetItemState(iItem-1, LVIS_SELECTED, LVIS_SELECTED);
  m_listStyles.SetSelectionMark(iItem-1);
  m_listStyles.SetFocus();
}

void CPsTbEditFormViewPage::AddNewPlotStyle(LPCTSTR lpStyleName)
{
  m_pPlotStyleActive = m_pPlotStyleTable->addNewPlotStyle(lpStyleName);
  int iItem = m_pPlotStyleTable->plotStyleSize()-1; 
  insertItem(iItem);
  m_listStyles.SetItemState(iItem, LVIS_SELECTED, LVIS_SELECTED);
  m_listStyles.SetSelectionMark(iItem);
}

void CPsTbEditFormViewPage::OnAddBtnStyle()
{
#ifdef _WIN64
  DialogBox(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDD_PS_DLG_ADDPS), m_hWnd, (DLGPROC)Dlg_Proc); 
#else
  DialogBox(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDD_PS_DLG_ADDPS), m_hWnd, Dlg_Proc); 
#endif
  m_listStyles.SetFocus();
}


BOOL CPsTbEditFormViewPage::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags/*, BOOL bOpenFileDialog*/)
{
  CString ext = fileName.Right(3);
  bool isCtb = m_pPlotStyleTable->isAciTableAvailable();

  CFileDialog dlgFile(FALSE);

  CString title = _T("Save As");

  dlgFile.m_ofn.Flags |= lFlags;

  CString strFilter;
  CString strDefault;

  strFilter = isCtb 
            ? _T("Color-Dependent Style Table Files (*.ctb)")
            : _T("Style Table Files (*.stb)");
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += isCtb ? _T("*.ctb") : _T("*.stb");
  strFilter += (TCHAR)'\0';   // last string
  dlgFile.m_ofn.nMaxCustFilter++;
  dlgFile.m_ofn.nFilterIndex = 1;

  if (fileName.ReverseFind('.') != -1)
  {
    fileName = fileName.Left(fileName.ReverseFind('.'));
  }

  dlgFile.m_ofn.lpstrFilter = strFilter;
  dlgFile.m_ofn.lpstrTitle = title;
  dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

  LPARAM nResult = dlgFile.DoModal();
  if(nResult == IDOK )
  {
    fileName = dlgFile.GetPathName();
    if (fileName.ReverseFind('.') == -1)
    {
      fileName += isCtb ? _T(".ctb") : _T(".stb");
    }

  }

  return nResult == IDOK;
}


void CPsTbEditFormViewPage::SetFileBufPath(const OdString sFilePath)
{
  m_sFileBufPath = sFilePath;
}

void CPsTbEditFormViewPage::OnSaveBtn()
{
  CString sPath = (LPCTSTR)m_sFileBufPath;

  if (!DoPromptFileName(sPath,
            AFX_IDS_SAVEFILE, OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST))
                  return;       // don't even attempt to save


  OdStreamBufPtr pFileBuf;
  OdDbSystemServices* pSs = odSystemServices();
  try{

    pFileBuf = pSs->createFile(OdString((LPCTSTR)sPath), Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kOpenAlways/*Oda::kCreateAlways*/);
    if(pFileBuf.get())
    {
      OdPsPlotStyleServicesPtr pPSS = odrxDynamicLinker()->loadApp(ODPS_PLOTSTYLE_SERVICES_APPNAME);
      if(pPSS.get())
      {
        pPSS->savePlotStyleTable(pFileBuf, m_pPlotStyleTable);
      }
    }

   }
  catch(...)
   {
     return;
   }
}

void CPsTbEditFormViewPage::OnLineweightBtn()
{
  CPsEditLineweightDlg PsEditLineweightDlg;
  PsEditLineweightDlg.SetPlotStyleTable(m_pPlotStyleTable);
  OdPsPlotStyleData OdPsData;
  int idx = m_comboLineweight.GetCurSel();
  if(idx == CB_ERR)
  {
    m_pPlotStyleActive->getData(OdPsData);
    idx = (int)OdPsData.lineweight();
  }
  PsEditLineweightDlg.SetListActiveIdx(m_comboLineweight.GetCurSel());
  if( PsEditLineweightDlg.DoModal() == IDOK)
  {
    m_comboLineweight.ResetContent();
    initLineweightComboBox();
    m_pPlotStyleActive->getData(OdPsData);
    m_comboLineweight.SetCurSel((OdUInt32)OdPsData.lineweight());
  }
}

const int CPsTbEditFormViewPage::deleteCustomColor()
{

  if(m_comboColor.GetCount() > PS_COMBO_COLOR_POSITION+1)
     m_comboColor.DeleteString(PS_COMBO_COLOR_POSITION);

  return 0;

}

const int CPsTbEditFormViewPage::appendCustomColor(const int item)
{

  CPsListStyleData* pPsListStyleData = (CPsListStyleData*)(m_listStyles.GetItemData(item));
  CBitmapColorInfo* pBitmapColorInfo = pPsListStyleData->GetBitmapColorInfo();
  if(!pBitmapColorInfo) 
    return pPsListStyleData->GetActiveListIndex();

  return m_comboColor.InsertBitmap(PS_COMBO_COLOR_POSITION, &pBitmapColorInfo->m_bitmap, pBitmapColorInfo->m_name);

}

const int CPsTbEditFormViewPage::replaceCustomColor(COLORREF color, const int item)
{

  CPsListStyleData* pPsListStyleData = (CPsListStyleData*)(m_listStyles.GetItemData(item));
  pPsListStyleData->ReplaceBitmapColorInfo(color, item);

  CBitmapColorInfo* pBitmapColorInfo = pPsListStyleData->GetBitmapColorInfo();
  if(!pBitmapColorInfo) 
    return pPsListStyleData->GetActiveListIndex();

  return m_comboColor.InsertBitmap(PS_COMBO_COLOR_POSITION, &pBitmapColorInfo->m_bitmap, pBitmapColorInfo->m_name);

}

