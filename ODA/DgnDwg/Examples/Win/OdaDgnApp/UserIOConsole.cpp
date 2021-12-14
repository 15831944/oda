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

// UserIOConsole.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "DgCommandContext.h"
#include "DgSSet.h"
#include "ExDgCommandContext.h"
#include "UserIOConsole.h"
#include "Ge/GeExtents2d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserIOConsole dialog


CUserIOConsole::CUserIOConsole(CWnd* pParent)
	: CDialog(CUserIOConsole::IDD, pParent)
  , m_nRefCounter(1)
  , m_nStrings(0)
{
	//{{AFX_DATA_INIT(CUserIOConsole)
	m_sInput = _T("");
	m_sPrompt = _T("");
	//}}AFX_DATA_INIT
}


OdString CUserIOConsole::getLastString()
{
  int ch = m_sOutput.ReverseFind('\r');
  if ( ch == -1 ) return (LPCTSTR)m_sOutput;
  return (LPCTSTR)m_sOutput.Mid( ch + 2 );
}

void CUserIOConsole::addRef()
{
  ++m_nRefCounter;
}

void CUserIOConsole::release()
{
  ODA_ASSERT((m_nRefCounter > 0));
  if (!(--m_nRefCounter))
  {
    delete this;
  }
}

long CUserIOConsole::numRefs() const { return m_nRefCounter; }

void CUserIOConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserIOConsole)
	DDX_Control(pDX, IDC_PROMPT, m_promptWnd);
	DDX_Text(pDX, IDC_INPUT, m_sInput);
	DDX_Text(pDX, IDC_PROMPT, m_sPrompt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserIOConsole, CDialog)
	//{{AFX_MSG_MAP(CUserIOConsole)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
  ON_WM_SIZE()
  ON_WM_DESTROY()
  ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


OdSmartPtr<CUserIOConsole> CUserIOConsole::create(CWnd* pParent)
{
  return OdSmartPtr<CUserIOConsole>(new CUserIOConsole(pParent), kOdRxObjAttach);
}

void CUserIOConsole::echo(const OdString& string)
{
  m_sOutput += _T(' ');
  m_sOutput += (LPCTSTR)string;
}

OdString CUserIOConsole::getString(const OdString& prompt, int options, OdEdStringTracker* pTracker)
{
  putString(prompt);
  m_sInput.Empty();
  m_sPrompt = m_sOutput;

  if(DoModal()==IDCANCEL)
  {
    m_sOutput += " *Cancel*";
    throw OdEdCancel();
  }

  echo(OdString(m_sInput));
  m_nStrings = 0;
  return (LPCTSTR)m_sInput;
}

const int nMaxStringLength = 128;

void CUserIOConsole::addString(const CString& str2)
{
  CString& str1 = m_sOutput;
  if(str2.GetLength() <= nMaxStringLength)
  {
    ++m_nStrings;
    str1 += "\r\n";
    str1 += str2;
  }
  else // break long string
  {
    CString longString(str2);
    CString sMax;
    while(longString.GetLength() > nMaxStringLength)
    {
      sMax = longString.Left(nMaxStringLength);
      int n = sMax.ReverseFind(' ');
      if(n > -1)
      {
        addString(sMax.Left(n));
        longString = longString.Right(longString.GetLength() - n - 1);
      }
      else
      {
        addString(sMax);
        longString = longString.Right(longString.GetLength() - nMaxStringLength);
      }
    }
    ASSERT(!longString.IsEmpty());
    addString(longString);
  }
}

void CUserIOConsole::addOut(const CString& str2)
{
  int n = 0, n0 = 0;
  while((n = str2.Find('\n', n0)) > -1)
  {
    addString(str2.Mid(n0, n-n0));
    n0 = ++n;
  }
  n = str2.GetLength();
  addString(str2.Mid(n0, n-n0));
}

void CUserIOConsole::putString(const OdString& string)
{
  addOut((LPCTSTR)string);
}

/////////////////////////////////////////////////////////////////////////////
// CUserIOConsole message handlers

BOOL CUserIOConsole::OnInitDialog() 
{
	CDialog::OnInitDialog();
  
  if(!m_font.m_hObject)
  {
    VERIFY(m_font.CreateFont(
      10,                   // nHeight
      0,                    // nWidth
      0,                    // nEscapement
      0,                    // nOrientation
      FW_NORMAL,            // nWeight
      FALSE,                // bItalic
      FALSE,                // bUnderline
      0,                    // cStrikeOut
      DEFAULT_CHARSET,      // nCharSet
      OUT_DEFAULT_PRECIS,   // nOutPrecision
      CLIP_DEFAULT_PRECIS,  // nClipPrecision
      DEFAULT_QUALITY,      // nQuality
      DEFAULT_PITCH,        // nPitchAndFamily
      _T("Courier")));          // lpszFacename
  }
  return TRUE;
}

void CUserIOConsole::OnPaint() 
{
	CPaintDC dc(this);
  m_promptWnd.SetFont(&m_font);
}

void CUserIOConsole::OnSize(UINT nType, int cx, int cy)
{
  CRect r1,r2;
  GetDlgItem(IDC_PROMPT)->GetWindowRect(&r1);
  GetDlgItem(IDC_INPUT)->GetWindowRect(&r2);
  ScreenToClient(r1);
  ScreenToClient(r2);
  int border = r1.left;
  int editHeight = r2.Height();
  CDialog::OnSize(nType, cx, cy);
  r1.right = cx - border;
  r1.bottom = cy - border - editHeight;
  GetDlgItem(IDC_PROMPT)->MoveWindow(r1);
  r2.top = r1.bottom;
  r2.bottom = cy - border;
  r2.right = cx - border;
  GetDlgItem(IDC_INPUT)->MoveWindow(r2);
}

int CUserIOConsole::_cx = 660;
int CUserIOConsole::_cy = 200;

void CUserIOConsole::OnDestroy()
{
  CRect r;GetWindowRect(r);
  _cx = r.Width();
  _cy = r.Height();
  CDialog::OnDestroy();
}

void CUserIOConsole::OnShowWindow(BOOL bShow, UINT nStatus)
{
  CDialog::OnShowWindow(bShow, nStatus);
  CRect r;GetWindowRect(&r);
  SetWindowPos(0,r.left,r.top,_cx,_cy, SWP_SHOWWINDOW | SWP_NOZORDER);
  CEdit* e = (CEdit*)GetDlgItem(IDC_PROMPT);
  e->LineScroll(e->GetLineCount());
}
