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

#if !defined(AFX_PDFNOTIFICATIONDIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
#define AFX_PDFNOTIFICATIONDIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CHyperlink window

class CHyperlink : public CStatic
{
  // Construction
public:
  CHyperlink();

  // Dialog Data
  //{{AFX_DATA(CPdfNotification)
  enum { IDD = IDC_URL };
  //}}AFX_DATA

  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CHyperlink)
public:
protected:
  virtual void PreSubclassWindow();
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CHyperlink();

  void SetURL(CString text) {
    m_URL = text;
  }

  void SetFont(CFont *hypFont) {
    LOGFONT pLogFont;
    GetFont()->GetLogFont(&pLogFont);
    pLogFont.lfUnderline = TRUE;
    m_HypFont.CreateFontIndirect(&pLogFont);
  }

  // Generated message map functions
protected:
  HCURSOR m_HypCursor;
  CFont  m_HypFont;
  CString m_URL;
  //{{AFX_MSG(CHyperlink)
  afx_msg void OnPaint();
#if _MSC_VER > 1310 // > VS 7.1
  afx_msg LRESULT OnNcHitTest(CPoint point);
#else
  afx_msg UINT OnNcHitTest(CPoint point);
#endif
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
};

class CPdfNotification : public CDialog
{
public:
  CPdfNotification();

  // Dialog Data
  //{{AFX_DATA(CPdfNotification)
  enum { IDD = IDD_PDFNOTIFICATION };
  CHyperlink m_Url;
  CHyperlink m_Url2;
  CHyperlink m_Url3;
  //}}AFX_DATA

  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CPdfNotification)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  //}}AFX_VIRTUAL

  // Implementation
protected:
  //{{AFX_MSG(CPdfNotification)
  // No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDFNOTIFICATIONDIALOG_H__4AF80B03_7FB7_42D3_9281_E8A5F4FB9A36__INCLUDED_)
