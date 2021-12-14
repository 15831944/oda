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

// ColorPickerDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "odamfcapp.h"
#include "ColorPickerDlg.h"

#define WM_CGT_COLORCHANGE (WM_USER + 1)

// CColorPickerDlg dialog

IMPLEMENT_DYNAMIC(CColorPickerDlg, CDialog)

CColorPickerDlg::CColorPickerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CColorPickerDlg::IDD, pParent)
{
  m_crColor = RGB(0, 0, 0);
  //m_cSpectrumTracker.SetSpectrum(); // HSL
  //m_cIntensityTracker.SetGradient(RGB(128, 128, 128)); // HSL
  m_cRedTracker.SetColor(RGB(255, 0, 0));
  m_cGreenTracker.SetColor(RGB(0, 255, 0));
  m_cBlueTracker.SetColor(RGB(0, 0, 255));
}

CColorPickerDlg::~CColorPickerDlg()
{
}

void CColorPickerDlg::SetColor(ODCOLORREF crColor, bool invalidate)
{
  m_crColor = crColor;
  m_cColorPreview.SetColor(crColor, invalidate);
}

ODCOLORREF CColorPickerDlg::GetColor() const
{
  return m_crColor;
}

void CColorPickerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_GRADCOLOR_SELECTED, m_cColorPreview);
  DDX_Control(pDX, IDC_GRADCOLOR_RED, m_cRedTracker);
  DDX_Control(pDX, IDC_GRADCOLOR_GREEN, m_cGreenTracker);
  DDX_Control(pDX, IDC_GRADCOLOR_BLUE, m_cBlueTracker);
}

BEGIN_MESSAGE_MAP(CColorPickerDlg, CDialog)
  ON_MESSAGE(WM_CGT_COLORCHANGE, OnCGTColorChange)
  ON_BN_CLICKED(IDC_GRADCOLOR_CHOOSE, OnBnClickedGradcolorChoose)
END_MESSAGE_MAP()

// CColorPickerDlg message handlers

BOOL CColorPickerDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_cRedTracker.SetPosRange(ODGETRED(m_crColor), 0, 255, false);
  m_cGreenTracker.SetPosRange(ODGETGREEN(m_crColor), 0, 255, false);
  m_cBlueTracker.SetPosRange(ODGETBLUE(m_crColor), 0, 255, false);
  //m_cIntensityTracker.SetGradient(crColor, invalidate);  

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CColorPickerDlg::OnCGTColorChange(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
  long r = m_cRedTracker.GetPosRange(0, 255),
       g = m_cGreenTracker.GetPosRange(0, 255),
       b = m_cBlueTracker.GetPosRange(0, 255);
  SetColor(ODRGB(r, g, b), true);
  return 0L;
}

void CColorPickerDlg::OnBnClickedGradcolorChoose()
{
  CColorDialog dlg;
  dlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
  dlg.m_cc.rgbResult = m_crColor;

  if (IDOK == dlg.DoModal())
  {
    ODCOLORREF color = (ODCOLORREF)dlg.GetColor();
    SetColor(color, true);
    m_cRedTracker.SetPosRange(GetRValue(m_crColor), 0, 255, true);
    SetColor(color, true);
    m_cGreenTracker.SetPosRange(GetGValue(m_crColor), 0, 255, true);
    SetColor(color, true);
    m_cBlueTracker.SetPosRange(GetBValue(m_crColor), 0, 255, true);
    SetColor(color, true);
  }
}

//////////////////////////////////////////////////////////////////////////////////////

// CColorGradientTrack

IMPLEMENT_DYNAMIC(CColorGradientTrack, CStatic)

CColorGradientTrack::CColorGradientTrack()
{
  m_lPos = 1;
  m_bCaptured = false;
}

CColorGradientTrack::~CColorGradientTrack()
{
}

void CColorGradientTrack::SetGradient(ODCOLORREF baseColor, bool invalidate)
{
  m_cGradient.createGradient(ODRGB(0, 0, 0), baseColor, ODRGB(255, 255, 255), 256);
  if (invalidate)
  {
    Invalidate(FALSE);
  }
}

void CColorGradientTrack::SetColor(ODCOLORREF baseColor, bool invalidate)
{
  m_cGradient.createGradient(ODRGB(0, 0, 0), baseColor, 256);
  // @@@ Some tests
  //m_cGradient.createGradient(ODRGB(0, 0, 0), baseColor, 256, OdGiGradientGenerator::kExpInterpolation);
  //m_cGradient.createGradient(ODRGB(0, 0, 0), baseColor, 256, OdGiGradientGenerator::kInvExpInterpolation);
  //m_cGradient.createGradient(ODRGB(0, 0, 0), baseColor, 256, OdGiGradientGenerator::kCosInterpolation);
  //SetSpectrum(invalidate);
  //m_cGradient.createGradient(ODRGB(200, 150, 0), ODRGB(120, 200, 75), ODRGB(200, 150, 0), 256,
  //                           OdGiGradientGenerator::kExpInterpolation, OdGiGradientGenerator::kInvExpInterpolation);
  // @@@
  if (invalidate)
  {
    Invalidate(FALSE);
  }
}

void CColorGradientTrack::SetSpectrum(bool invalidate)
{
  m_cGradient = OdGiGradientGenerator::createSpectrumGradient(256);
  if (invalidate)
  {
    Invalidate(FALSE);
  }
}

void CColorGradientTrack::SetPos(long pos, bool invalidate)
{
  RECT Rect;
  GetClientRect(&Rect);
  if (pos < Rect.left + 1)
    pos = Rect.left + 1;
  else if (pos > Rect.right - 1)
    pos = Rect.right - 1;
  if (pos != m_lPos)
  {
    RECT Rect2;
    Rect2.left = m_lPos - 6;
    Rect2.right = m_lPos + 6;
    Rect2.top = Rect.bottom - 10;
    Rect2.bottom = Rect.bottom;
    if (pos - 6 < Rect2.left)
      Rect2.left = pos - 6;
    if (pos + 6 > Rect2.right)
      Rect2.right = pos + 6;
    m_lPos = pos;
    if (invalidate)
    {
      InvalidateRect(&Rect, FALSE);
      GetParent()->SendMessage(WM_CGT_COLORCHANGE, 0, (LPARAM)m_lPos);
    }
  }
}

void CColorGradientTrack::SetPosRange(long pos, long minRange, long maxRange, bool invalidate)
{
  RECT Rect;
  GetClientRect(&Rect);
  SetPos(1 + long((double(pos - minRange) / (maxRange - minRange)) * (Rect.right - 2)), invalidate);
}

long CColorGradientTrack::GetPos() const
{
  return m_lPos;
}

long CColorGradientTrack::GetPosRange(long minRange, long maxRange) const
{
  RECT Rect;
  GetClientRect(&Rect);
  return minRange + long((double(m_lPos - 1) / (Rect.right - 2)) * (maxRange - minRange));
}

BEGIN_MESSAGE_MAP(CColorGradientTrack, CStatic)
  ON_WM_PAINT()
  ON_WM_LBUTTONDOWN()
  ON_WM_LBUTTONUP()
  ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CColorGradientTrack message handlers

void CColorGradientTrack::OnPaint()
{
  CPaintDC dc(this);
  CBrush backBrush(GetSysColor(COLOR_BTNFACE));
  RECT allRect;
  GetClientRect(&allRect);
  RECT btRect = allRect;
  btRect.top = btRect.bottom - 8;
  dc.FillRect(&btRect, &backBrush);
  RECT gradRect = allRect;
  gradRect.bottom -= 8;
#if (_WIN32_WINNT >= 0x0500) && (_MFC_VER >= 0x0700)
  /*COLORREF dcPenPrev =*/ dc.SetDCPenColor(RGB(0, 0, 0));
  HGDIOBJ prevDcPen = dc.SelectObject(GetStockObject(DC_PEN));
  CPen *prevPen, blackPen(PS_SOLID, 1, RGB(0, 0, 0));
#else
  CPen *prevPen, blackPen(PS_SOLID, 1, RGB(0, 0, 0));
  prevPen = dc.SelectObject(&blackPen);
#endif  
  dc.MoveTo(gradRect.left, gradRect.top);
  dc.LineTo(gradRect.right, gradRect.top);
  dc.LineTo(gradRect.right, gradRect.bottom);
  dc.LineTo(gradRect.left, gradRect.bottom);
  dc.LineTo(gradRect.left, gradRect.top);
#if !((_WIN32_WINNT >= 0x0500) && (_MFC_VER >= 0x0700))
  dc.SelectObject(prevPen);
#endif
  for (long i = 0; i <= gradRect.right - 2; i++)
  {
#if (_WIN32_WINNT >= 0x0500) && (_MFC_VER >= 0x0700)
    dc.SetDCPenColor((COLORREF)m_cGradient.colorAt(double(i) / (gradRect.right - 2)));
#else
    CPen gradPen(PS_SOLID, 1, (COLORREF)m_cGradient.colorAt(double(i) / (gradRect.right - 2)));
    prevPen = dc.SelectObject(&gradPen);
#endif
    dc.MoveTo(i + 1, gradRect.top + 1);
    dc.LineTo(i + 1, gradRect.bottom);
#if !((_WIN32_WINNT >= 0x0500) && (_MFC_VER >= 0x0700))
    dc.SelectObject(prevPen);
#endif
  }
#if (_WIN32_WINNT >= 0x0500) && (_MFC_VER >= 0x0700)
  dc.SelectObject(prevDcPen);
#endif
  // Paint marker
  POINT points[3];
  points[0].x = m_lPos; // 1 - 1
  points[0].y = gradRect.bottom;
  points[1].x = points[0].x - 5;
  if (points[1].x < 1)
    points[1].x = 1;
  points[1].y = allRect.bottom - 1;
  points[2].x = points[0].x + 5;
  if (points[2].x > gradRect.right - 1)
    points[2].x = gradRect.right - 1;
  points[2].y = allRect.bottom - 1;
  CBrush seekBrush(RGB(0, 0, 128)), *prevBrush;
  prevBrush = dc.SelectObject(&seekBrush);
  prevPen = dc.SelectObject(&blackPen);
  dc.Polygon(points, 3);
  dc.SelectObject(prevBrush);
  dc.SelectObject(prevPen);
}

void CColorGradientTrack::OnLButtonDown(UINT nFlags, CPoint point)
{
  SetPos(point.x, true);
  m_bCaptured = true;
  SetCapture();

  CStatic::OnLButtonDown(nFlags, point);
}

void CColorGradientTrack::OnLButtonUp(UINT nFlags, CPoint point)
{
  if (m_bCaptured)
  {
    ReleaseCapture();
    m_bCaptured = false;
  }

  CStatic::OnLButtonUp(nFlags, point);
}

void CColorGradientTrack::OnMouseMove(UINT nFlags, CPoint point)
{
  if (m_bCaptured)
  {
    SetPos(point.x, true);
  }

  CStatic::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////////////////

// CColorPreview

IMPLEMENT_DYNAMIC(CColorPreview, CStatic)

CColorPreview::CColorPreview()
{
  m_crColor = RGB(0, 0, 0);
}

CColorPreview::~CColorPreview()
{
}

void CColorPreview::SetColor(ODCOLORREF crColor, bool invalidate)
{
  m_crColor = crColor;
  if (invalidate)
  {
    Invalidate(FALSE);
  }
}

ODCOLORREF CColorPreview::GetColor() const
{
  return m_crColor;
}

BEGIN_MESSAGE_MAP(CColorPreview, CStatic)
  ON_WM_PAINT()
END_MESSAGE_MAP()

// CColorPreview message handlers

void CColorPreview::OnPaint()
{
  CPaintDC dc(this);
  CBrush brush((COLORREF)m_crColor);
  RECT Rect;
  GetClientRect(&Rect);
  dc.FillRect(&Rect, &brush);
}

//
