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

#if !defined(AFX_COLORPICKERDIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
#define AFX_COLORPICKERDIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Exports from Gi. You must link Gi first.
#include "Gi/GiProceduralGenerator.h"

// CColorGradientTrack

class CColorGradientTrack : public CStatic
{
	DECLARE_DYNAMIC(CColorGradientTrack)

  OdGiGradientGenerator m_cGradient;
  long m_lPos;
  bool m_bCaptured;
public:
	CColorGradientTrack();
	virtual ~CColorGradientTrack();

  void SetGradient(ODCOLORREF baseColor, bool invalidate = false);
  void SetColor(ODCOLORREF baseColor, bool invalidate = false);
  void SetSpectrum(bool invalidate = false);
  void SetPos(long pos, bool invalidate = false);
  void SetPosRange(long pos, long minRange, long maxRange, bool invalidate = false);
  long GetPos() const;
  long GetPosRange(long minRange, long maxRange) const;
protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

// CColorPreview

class CColorPreview : public CStatic
{
	DECLARE_DYNAMIC(CColorPreview)

  ODCOLORREF m_crColor;
public:
	CColorPreview();
	virtual ~CColorPreview();

  void SetColor(ODCOLORREF crColor, bool invalidate = false);
  ODCOLORREF GetColor() const;
protected:
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnPaint();
};

// CColorPickerDlg dialog

class CColorPickerDlg : public CDialog
{
	DECLARE_DYNAMIC(CColorPickerDlg)

  CColorGradientTrack m_cRedTracker;
  CColorGradientTrack m_cGreenTracker;
  CColorGradientTrack m_cBlueTracker;
  CColorPreview m_cColorPreview;
  ODCOLORREF m_crColor;
public:
	CColorPickerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CColorPickerDlg();

// Dialog Data
	enum { IDD = IDD_BACKGROUND_COLOR };

  void SetColor(ODCOLORREF crColor, bool invalidate = false);
  ODCOLORREF GetColor() const;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();
  afx_msg LRESULT OnCGTColorChange(WPARAM wParam, LPARAM lParam);
  afx_msg void OnBnClickedGradcolorChoose();
};

#endif // AFX_COLORPICKERDIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_
