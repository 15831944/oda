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

// Export2PDFDialog.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "PdfNotification.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdfNotification dialog used for App About


CPdfNotification::CPdfNotification() : CDialog(CPdfNotification::IDD)
{
  //{{AFX_DATA_INIT(CPdfNotification)
  //}}AFX_DATA_INIT
}

BOOL CPdfNotification::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_Url.SetURL(L"http://www.opendesign.com/the_oda_platform/thirdpartyintegrations/pdfintegration");
  m_Url.SetFont(GetFont());
  m_Url2.SetURL(L"http://www.visual-integrity.com");
  m_Url2.SetFont(GetFont());
  m_Url3.SetURL(L"http://www.visual-integrity.com/contact-us/");
  m_Url3.SetFont(GetFont());

  return TRUE;
}

void CPdfNotification::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CPdfNotification)
  DDX_Control(pDX, IDC_URL, m_Url);
  DDX_Control(pDX, IDC_URL2, m_Url3);
  DDX_Control(pDX, IDC_URL3, m_Url2);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPdfNotification, CDialog)
  //{{AFX_MSG_MAP(CPdfNotification)
  // No message handlers
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperlink


/***********************************************************************/
/// <b>Function: CHyperlink</b>
///
/// \param   NONE
///
/// \remarks    Constructor
///
/***********************************************************************/
CHyperlink::CHyperlink()
{
  m_HypCursor = NULL;
}


/***********************************************************************/
/// <b>Function: ~CHyperlink</b>
///
/// \param   NONE
///
/// \remarks    Destructor
///
/***********************************************************************/
CHyperlink::~CHyperlink()
{
  ::DeleteObject(m_HypCursor);
  ::DeleteObject(m_HypFont);
}


BEGIN_MESSAGE_MAP(CHyperlink, CStatic)
  //{{AFX_MSG_MAP(CHyperlink)
  ON_WM_PAINT()
  ON_WM_NCHITTEST()
  ON_WM_LBUTTONUP()
  ON_WM_SETCURSOR()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHyperlink message handlers


/***********************************************************************/
/// <b>Function: OnPaint</b>
///
/// \param   NONE
///
/// \return void 
///
/// \remarks    WM_PAINT HANDLER
///
/***********************************************************************/
void CHyperlink::OnPaint() 
{
  // TODO: Add your message handler code here

  CPaintDC        dc (this);    // device context for painting
  RECT            rectWnd;        // window rectangle
  CString   strText;  // window text

  dc.SetBkMode(TRANSPARENT);

  GetWindowText(strText);
  GetClientRect(&rectWnd);

  dc.SelectObject(m_HypFont);

  dc.SetTextColor( RGB(0, 0, 255) );

  /// Draw text
  ::DrawTextEx(dc.m_hDC, strText.GetBuffer(0), strText.GetLength(), &rectWnd, DT_LEFT, NULL);
  strText.ReleaseBuffer();

  // Do not call CStatic::OnPaint() for painting messages
}


/***********************************************************************/
/// <b>Function: PreSubclassWindow</b>
///
/// \param   NONE
///
/// \return void 
///
/// \remarks    Initialize variables specially the hand 
///    cursor before the window is created.
///    "Default hand cursor" from Paul DiLascia's Jan 1998 
///    MSJ article.
///
/***********************************************************************/
void CHyperlink::PreSubclassWindow() 
{
  // TODO: Add your specialized code here and/or call the base class

  if (m_HypCursor == NULL)                // No cursor handle - load our own
  {
    m_HypCursor  = ::LoadCursor(NULL, IDC_HAND);
  }

  CStatic::PreSubclassWindow();
}


/***********************************************************************/
/// <b>Function: OnNcHitTest</b>
///
/// \param  point (in\out)
///
/// \return UINT 
///
/// \remarks    Override WM_NCHITTEST message handler to handle 
///    the Mouse events to be handled by the Client and not 
///    the parent window.
///
/***********************************************************************/
#if _MSC_VER > 1310 // > VS 7.1
LRESULT CHyperlink::OnNcHitTest(CPoint point) 
#else
UINT CHyperlink::OnNcHitTest(CPoint point) 
#endif
{
  return HTCLIENT  ;
}


/***********************************************************************/
/// <b>Function: OnLButtonUp</b>
///
/// \param  nFlags    (in\out)
///
/// \param  point (in\out)
///
/// \return void 
///
/// \remarks    Launch the web browser to display the URL.
///
/***********************************************************************/
void CHyperlink::OnLButtonUp(UINT nFlags, CPoint point) 
{
  // TODO: Add your message handler code here and/or call default
  HINSTANCE result = ShellExecute(NULL, _T("open"), m_URL, NULL,NULL, SW_SHOW);

  CStatic::OnLButtonUp(nFlags, point);
}


/***********************************************************************/
/// <b>Function: OnSetCursor</b>
///
/// \param  pWnd  (in\out)
///
/// \param  nHitTest  (in\out)
///
/// \param  message   (in\out)
///
/// \return BOOL 
///
/// \remarks    Change the cursor so that the cursor is always hand cursor
///
/***********************************************************************/
BOOL CHyperlink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
  // TODO: Add your message handler code here and/or call default
  if (m_HypCursor)
  {
    ::SetCursor(m_HypCursor);
    return TRUE;
  }
  return FALSE;
}
