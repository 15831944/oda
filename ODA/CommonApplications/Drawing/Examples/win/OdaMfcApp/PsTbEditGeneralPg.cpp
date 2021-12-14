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

// PsTbEditGeneralPg.cpp : implementation file
//

#include "stdafx.h"
#include "Shlwapi.h"
#include "PsTbEditGeneralPg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// m_Page2 property page

IMPLEMENT_DYNCREATE(CPsTbEditGeneralPage, CPropertyPage)

CPsTbEditGeneralPage::CPsTbEditGeneralPage() : CPropertyPage(CPsTbEditGeneralPage::IDD), m_pPlotStyleTable(0)
{
	//{{AFX_DATA_INIT(CDbTreeSettingPage2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPsTbEditGeneralPage::~CPsTbEditGeneralPage()
{
}

void CPsTbEditGeneralPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbTreeSettingPage2Dlg)
     	DDX_Control(pDX, IDC_PS_GENERAL_EDIT_DESCRIPTION,    m_editDescription);
     	DDX_Control(pDX, IDC_PS_GENERAL_EDIT_SCALE_FACTOR,   m_editScalefactor);
    	DDX_Control(pDX, IDC_PS_GENERAL_CHECK_SCALE_FACTOR,  m_checkScalefactor);
     	DDX_Control(pDX, IDC_PS_GENERAL_STATIC_FILEPATH,     m_staticFilepath);
     	DDX_Control(pDX, IDC_PS_GENERAL_STATIC_FILE_NAME,    m_staticFilename);
     	DDX_Control(pDX, IDC_PS_GENERAL_STATIC_BITMAP,       m_staticBitmap);
      DDX_Control(pDX, IDC_PS_GENERAL_STATIC_REGULAR,      m_staticRegular);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPsTbEditGeneralPage, CPropertyPage)
	//{{AFX_MSG_MAP(CPsTbEditGeneralPage)
		// NOTE: the ClassWizard will add message map macros here
    	ON_EN_CHANGE (IDC_PS_GENERAL_EDIT_DESCRIPTION,    OnChangeEditDescription)
    	ON_BN_CLICKED(IDC_PS_GENERAL_CHECK_SCALE_FACTOR,  OnCheckScalefactor     )
    	ON_EN_CHANGE (IDC_PS_GENERAL_EDIT_SCALE_FACTOR,   OnEditScalefactor      )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPsTbEditGeneralPage message handlers


void DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart,
short yStart, COLORREF cTransparentColor)
{
BITMAP bm;
COLORREF cColor;
HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
HDC hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
POINT ptSize;

hdcTemp = CreateCompatibleDC(hdc);
SelectObject(hdcTemp, hBitmap); 

GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bm);
ptSize.x = bm.bmWidth; 
ptSize.y = bm.bmHeight; 
DPtoLP(hdcTemp, &ptSize, 1); 

hdcBack = CreateCompatibleDC(hdc);
hdcObject = CreateCompatibleDC(hdc);
hdcMem = CreateCompatibleDC(hdc);
hdcSave = CreateCompatibleDC(hdc);

bmAndBack = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);

bmAndMem = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
bmSave = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

bmBackOld = (HBITMAP)SelectObject(hdcBack, bmAndBack);
bmObjectOld = (HBITMAP)SelectObject(hdcObject, bmAndObject);
bmMemOld = (HBITMAP)SelectObject(hdcMem, bmAndMem);
bmSaveOld = (HBITMAP)SelectObject(hdcSave, bmSave);

SetMapMode(hdcTemp, GetMapMode(hdc));

BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

cColor = SetBkColor(hdcTemp, cTransparentColor);

BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0,
SRCCOPY);

SetBkColor(hdcTemp, cColor);

BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0,
NOTSRCCOPY);

BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdc, xStart, yStart,
SRCCOPY);

BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);

BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack, 0, 0, SRCAND);

BitBlt(hdcMem, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCPAINT);

BitBlt(hdc, xStart, yStart, ptSize.x, ptSize.y, hdcMem, 0, 0,
SRCCOPY);

BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave, 0, 0, SRCCOPY);

DeleteObject(SelectObject(hdcBack, bmBackOld));
DeleteObject(SelectObject(hdcObject, bmObjectOld));
DeleteObject(SelectObject(hdcMem, bmMemOld));
DeleteObject(SelectObject(hdcSave, bmSaveOld));

DeleteDC(hdcMem);
DeleteDC(hdcBack);
DeleteDC(hdcObject);
DeleteDC(hdcSave);
DeleteDC(hdcTemp);
}

const bool CPsTbEditGeneralPage::SetPlotStyleTable(OdPsPlotStyleTable* pPlotStyleTable)
{
  if(!pPlotStyleTable) return false;
  m_pPlotStyleTable = pPlotStyleTable;
  return true;
}

void WinPathToDos(TCHAR *str)
{
  CString pStr = str;
  CString sNewStr;
  int pos = 0;
  while(pos >= 0)
  {

    pos = pStr.Find('\\');
    CString s;
    if(pos < 0)
    {
      s = pStr;
    }
    else
    {
      s = pStr.Left(pos);
      if(s.GetLength() > 8)
      {
        s = s.Left(6);
        s.MakeUpper();
        s += _T("~1");
//        s += "\\";
      }
       s += _T("\\");
       pStr = pStr.Right(pStr.GetLength() - pos - 1);
    }

    sNewStr += s;
  }

  _tcscpy(str, sNewStr.GetBuffer(sNewStr.GetLength()));
}

BOOL CPsTbEditGeneralPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

  if(!m_pPlotStyleTable) return FALSE;

  OdString description = m_pPlotStyleTable->description();
  m_editDescription.SetWindowText(description);

  bool check = m_pPlotStyleTable->isApplyScaleFactor();
  m_checkScalefactor.SetCheck(check);
  m_editScalefactor.EnableWindow(check);
  OdString sScaleFactor;
  sScaleFactor.format(OD_T("%.1f"), m_pPlotStyleTable->scaleFactor());
  m_editScalefactor.SetWindowText(sScaleFactor);


  HDC editDC = ::GetDC(m_staticFilepath.m_hWnd);
//  CRect rect;
//  m_staticFilepath.GetClientRect(&rect);
  TCHAR buffer[MAX_PATH];
  _tcscpy(buffer, m_sFileBufPath);
  WinPathToDos(buffer);
  TCHAR *lpStr = buffer;
  PathCompactPath(editDC, lpStr, 630/*rect.right*/);
  m_staticFilepath.SetWindowText(lpStr);

  OdString sFileName = m_sFileBufPath.right( m_sFileBufPath.getLength() - m_sFileBufPath.reverseFind('\\') - 1);
  m_staticFilename.SetWindowText(sFileName);

  if(m_pPlotStyleTable->isAciTableAvailable())
    m_staticRegular.SetWindowText(_T("Legacy (can be used to import old DWGs)"));

  HBITMAP hBmp = (HBITMAP)::LoadImage( AfxGetInstanceHandle(), 
    MAKEINTRESOURCE(m_pPlotStyleTable->isAciTableAvailable() ? IDB_PS_BITMAP_GENERAL_CTB : IDB_PS_BITMAP_GENERAL_STB), 
      IMAGE_BITMAP, 32, 32, LR_LOADTRANSPARENT | LR_LOADMAP3DCOLORS ); 

  CClientDC dc(&m_staticBitmap);
  DrawTransparentBitmap(dc.m_hDC, hBmp, 0, 0, 0x00FFFFFF);
  m_staticBitmap.SetBitmap(hBmp);

  return TRUE;

}



void CPsTbEditGeneralPage::SetFileBufPath(const OdString sFilePath)
{
  m_sFileBufPath = sFilePath;
}

void CPsTbEditGeneralPage::OnChangeEditDescription()
{

  CString pVal;
  m_editDescription.GetWindowText(pVal);
#ifdef _UNICODE
  m_pPlotStyleTable->setDescription(OdString(pVal));
#else
  m_pPlotStyleTable->setDescription(OdString(pVal));
#endif
}

void CPsTbEditGeneralPage::OnCheckScalefactor()
{
  int check = m_checkScalefactor.GetCheck();
  m_pPlotStyleTable->setApplyScaleFactor(check ? true : false);
  m_editScalefactor.EnableWindow(check);
}

void CPsTbEditGeneralPage::OnEditScalefactor()
{
  CString pVal;
  m_editScalefactor.GetWindowText(pVal);
  double scaleFactor;
  _stscanf(pVal, _T("%lf"), &scaleFactor);
  if(scaleFactor <= 0 || scaleFactor > PS_EDIT_MAX_SCALEFACTOR)
  {
    scaleFactor = 0.01;
    m_editScalefactor.SetWindowText(_T("0.01"));
  }

/*  char buffer[15];
  _gcvt(scaleFactor, 10, buffer);
  m_editScalefactor.SetWindowText(buffer);*/
  m_pPlotStyleTable->setScaleFactor(scaleFactor);
}


void CPsTbEditGeneralPage::OnOK()
{
	CPropertyPage::OnOK();
}
