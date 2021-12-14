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
// CSHelperSheet.cpp : implementation file
//

#include "stdafx.h"
#include "CSHelperSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSHelperSheet

IMPLEMENT_DYNAMIC(CSHelperSheet, CPropertySheet)

CSHelperSheet::CSHelperSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
: CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
//, m_page1(*this)
//, m_page2(*this)
{
  
  AddControlPages();
  // TODO :: Add the pages for the rest of the controls here.
}

CSHelperSheet::CSHelperSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
//, m_page1(*this)
//, m_page2(*this)
{
  setHelperPages();
  AddControlPages();
}

CSHelperSheet::~CSHelperSheet()
{
}

void CSHelperSheet::AddControlPages()
{
  AddPage(&m_page1);
  AddPage(&m_page2);
  AddPage(&m_page3);
  AddPage(&m_page4);
}

BEGIN_MESSAGE_MAP(CSHelperSheet, CPropertySheet)
  //{{AFX_MSG_MAP(CSHelperSheet)
  ON_WM_SYSCOMMAND()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSHelperSheet message handlers

BOOL CSHelperSheet::OnInitDialog()
{
  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.

  return CPropertySheet::OnInitDialog();
}

void CSHelperSheet::OnSysCommand(UINT nID, LPARAM lParam)
{
  CPropertySheet::OnSysCommand(nID, lParam);
}

void CSHelperSheet::fillAndSelect2Page(CString file)
{
  SetActivePage(1);
  m_page2.fill(file);
}

void CSHelperSheet::fillAndSelect3Page(OdStreamBuf &data)
{
  SetActivePage(2);
  m_page3.fill(data);
}
