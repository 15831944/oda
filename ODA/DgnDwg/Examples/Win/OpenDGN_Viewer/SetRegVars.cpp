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
// SetRegVars.cpp : implementation file
//

#include "stdafx.h"
#include "OpenDGN_Viewer.h"
#include "SetRegVars.h"

#ifndef _TOOLKIT_IN_DLL_
#include "StaticAppSelDlg.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsSymbRsrc

CSetRegVarsMsSymbRsrc::CSetRegVarsMsSymbRsrc(CWnd* pParent /*=NULL*/)
  : CDialog(CSetRegVarsMsSymbRsrc::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSetRegVarsMsSymbRsrc)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CSetRegVarsMsSymbRsrc::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CSetRegVarsMsSymbRsrc)
  DDX_Text(pDX, IDC_PATH, m_MsSymbRsrc);
  DDV_MaxChars(pDX, m_MsSymbRsrc, 255);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetRegVarsMsSymbRsrc, CDialog)
  //{{AFX_MSG_MAP(CSetRegVarsMsRsrc)
  ON_BN_CLICKED(IDC_GET_PATH, OnBnClickedMsSymbRsrc)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSetRegVarsMsSymbRsrc::OnInitDialog() 
{
  // Get value from profile and set MS_SYMBRSRC are here: COdaDgnApp::InitInstance()
  //m_MsSymbRsrc = AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_SYMBRSRC"), _T(""));
  OdStringArray arraySymbRsrc;
  m_pHostAppServices->getMS_SYMBRSRC(arraySymbRsrc);
  if (arraySymbRsrc.size() > 0)
  {
    m_MsSymbRsrc = arraySymbRsrc.at(0).c_str();
  }

  return   CDialog::OnInitDialog();
}

void CSetRegVarsMsSymbRsrc::OnBnClickedMsSymbRsrc()
{
  CFileDialog dlg( TRUE, _T("rsc"), _T(""), OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
    _T("Symbology Resource (*.rsc)|*.rsc||"), ::AfxGetMainWnd() );

  dlg.m_ofn.lpstrTitle = _T("Select Symbology Resource");

  if ( dlg.DoModal() == IDOK )
  {
    m_MsSymbRsrc = dlg.GetPathName();

    UpdateData(FALSE);
  }
}
void CSetRegVarsMsSymbRsrc::OnOK() 
{
  UpdateData();

  // ser "MS_SYMBRSRC" RegVar
  OdStringArray arraySumbRsrc;
  OdString sSumbRsrc( m_MsSymbRsrc );
  arraySumbRsrc.append( sSumbRsrc );
  m_pHostAppServices->setMS_SYMBRSRC(arraySumbRsrc);

  AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_SYMBRSRC"), m_MsSymbRsrc);

	CDialog::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsRsrc

CSetRegVarsMsRsrc::CSetRegVarsMsRsrc(CWnd* pParent /*=NULL*/)
  : CDialog(CSetRegVarsMsRsrc::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSetRegVarsMsRsrc)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CSetRegVarsMsRsrc::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CSetRegVarsMsRsrc)
  DDX_Text(pDX, IDC_PATH, m_MsRsrc);
  DDV_MaxChars(pDX, m_MsRsrc, 255);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetRegVarsMsRsrc, CDialog)
  //{{AFX_MSG_MAP(CSetRegVarsMsRsrc)
  ON_BN_CLICKED(IDC_GET_PATH, OnBnClickedMsRsrc)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSetRegVarsMsRsrc::OnInitDialog() 
{
  // Get value from profile and set MS_RSRC are here: COdaDgnApp::InitInstance()
  //m_MsRsrc = AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_RSRC"), _T(""));
  m_MsRsrc = m_pHostAppServices->getMS_RSRC().c_str();

  return   CDialog::OnInitDialog();
}

void CSetRegVarsMsRsrc::OnBnClickedMsRsrc()
{
#ifdef _TOOLKIT_IN_DLL_
  CFileDialog dlg( TRUE, _T("rsc"), _T(""), OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
    _T("Symbology Resource (*.rsc)|*.rsc||"), ::AfxGetMainWnd() );

  dlg.m_ofn.lpstrTitle = _T("Select Symbology Resource");
#else
  CStaticAppSelDlg dlg(::AfxGetMainWnd());
#endif //#ifdef _TOOLKIT_IN_DLL_

  if ( dlg.DoModal() == IDOK )
  {
    m_MsRsrc = dlg.GetPathName();

    UpdateData(FALSE);
  }
}
void CSetRegVarsMsRsrc::OnOK() 
{
  UpdateData();

  // ser "MS_SYMBRSRC" RegVar
  OdString sRsrc( m_MsRsrc );
  m_pHostAppServices->setMS_RSRC(sRsrc);

  AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_RSRC"), m_MsRsrc);

	CDialog::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsRsrc

CSetRegVarsFastFont::CSetRegVarsFastFont(CWnd* pParent /*=NULL*/)
  : CDialog(CSetRegVarsFastFont::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSetRegVarsMsRsrc)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CSetRegVarsFastFont::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CSetRegVarsFastFont)
  DDX_Text(pDX, IDC_PATH, m_FastFont);
  DDV_MaxChars(pDX, m_FastFont, 255);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetRegVarsFastFont, CDialog)
  //{{AFX_MSG_MAP(CSetRegVarsFastFont)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSetRegVarsFastFont::OnInitDialog() 
{
  // Get value from profile and set FAST_FONT (if it's required) are here: COdaDgnApp::InitInstance()
  m_FastFont = m_pHostAppServices->getFAST_FONT().c_str();

  return   CDialog::OnInitDialog();
}

void CSetRegVarsFastFont::OnOK() 
{
  UpdateData();

  // ser "MS_SYMBRSRC" RegVar
  OdString sRsrc( m_FastFont );
  m_pHostAppServices->setFAST_FONT(sRsrc);

  AfxGetApp()->WriteProfileString(_T("RegVars"), _T("FAST_FONT"), m_FastFont);

	CDialog::OnOK();
}
