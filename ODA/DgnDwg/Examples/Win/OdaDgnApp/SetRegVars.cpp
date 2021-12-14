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
#include "OdaDgnApp.h"
#include "SetRegVars.h"

#ifndef _TOOLKIT_IN_DLL_
#include "StaticAppSelDlg.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsSymbRsrc

CSetRegVarsMsSymbRsrc::CSetRegVarsMsSymbRsrc(CWnd* pParent /*=NULL*/, CRegVarsType uType /*=kSymbRsrc*/ )
  : CDialog(CSetRegVarsMsSymbRsrc::IDD, pParent)
{
  m_uType = uType;
  //{{AFX_DATA_INIT(CSetRegVarsMsSymbRsrc)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CSetRegVarsMsSymbRsrc::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CSetRegVarsMsSymbRsrc)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetRegVarsMsSymbRsrc, CDialog)
  //{{AFX_MSG_MAP(CSetRegVarsMsRsrc)
  ON_BN_CLICKED(IDC_ADDBUTTON, OnAddButton)
  ON_BN_CLICKED(IDC_UPBUTTON, OnMoveUpButton)
  ON_BN_CLICKED(IDC_DOWNBUTTON, OnMoveDownButton)
  ON_BN_CLICKED(IDC_DELETEBUTTON, OnDeleteButton)
  //}}AFX_MSG_MAP
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_PATHLIST, OnLvnItemchangedPathlist)
END_MESSAGE_MAP()

BOOL CSetRegVarsMsSymbRsrc::OnInitDialog() 
{
  // Get value from profile and set MS_SYMBRSRC are here: COdaDgnApp::InitInstance()
  //m_MsSymbRsrc = AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_SYMBRSRC"), _T(""));
  OdStringArray arraySymbRsrc;

  switch( m_uType )
  {
    case kSymbRsrc:
    {
      m_pHostAppServices->getMS_SYMBRSRC(arraySymbRsrc);
      SetWindowText(L"Specify symbology resource file.");
    } break;

    case kMatPalette:
    {
      m_pHostAppServices->getMS_MATPALETTE(arraySymbRsrc);
      SetWindowText(L"Specify material palette file.");
    } break;
  }

  if (arraySymbRsrc.size() > 0)
    m_MsSymbRsrcArr = arraySymbRsrc;

  CListCtrl* pPathList = (CListCtrl*)(GetDlgItem(IDC_PATHLIST));

  if( pPathList )
  {
    pPathList->SetExtendedStyle(LVS_EX_GRIDLINES);

    pPathList->InsertColumn(0,L"File Path",LVCFMT_LEFT,450);

    for( OdUInt32 i = 0; i < arraySymbRsrc.size(); i++ )
    {
      pPathList->InsertItem(i, arraySymbRsrc[i].c_str() );
    }
  }

  return   CDialog::OnInitDialog();
}

void CSetRegVarsMsSymbRsrc::updateListCtrl()
{
  CListCtrl* pPathList = (CListCtrl*)(GetDlgItem(IDC_PATHLIST));

  if( pPathList )
  {
    pPathList->DeleteAllItems();

    for( OdUInt32 i = 0; i < m_MsSymbRsrcArr.size(); i++ )
    {
      pPathList->InsertItem(i, m_MsSymbRsrcArr[i].c_str() );
    }
  }
}

void CSetRegVarsMsSymbRsrc::OnAddButton()
{
  OdString strDefaultExt = _T("pal");
  OdString strFilter     = _T("External material palette file (*.pal)|*.pal||");
  OdString strTitle      = _T("Select External Material Palettes");

  if( m_uType == kSymbRsrc )
  {
    strDefaultExt = _T("rsc");
    strFilter     = _T("Symbology Resource (*.rsc)|*.rsc||");
    strTitle      = _T("Select Symbology Resource");
  }

  CFileDialog dlg( TRUE, strDefaultExt.c_str(), _T(""), OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
    strFilter.c_str(), ::AfxGetMainWnd() );

  dlg.m_ofn.lpstrTitle = strTitle.c_str();

  if ( dlg.DoModal() == IDOK )
  {
    CString strSymbRsrc = dlg.GetPathName();

    if( !strSymbRsrc.IsEmpty() ) 
    {
      OdString sSumbRsrc( strSymbRsrc );

      bool bAddFile = true;

      for( OdUInt32 i = 0; i < m_MsSymbRsrcArr.size(); i++ )
      {
        if( m_MsSymbRsrcArr[i] == sSumbRsrc )
        {
          bAddFile = false;
          break;
        }
      }

      if( bAddFile )
        m_MsSymbRsrcArr.push_back( sSumbRsrc );
    }
  }

  updateListCtrl();
}

void CSetRegVarsMsSymbRsrc::OnMoveUpButton()
{
  CListCtrl* pPathList = (CListCtrl*)(GetDlgItem(IDC_PATHLIST));

  POSITION pPos = pPathList->GetFirstSelectedItemPosition();

  if( pPos )
  {
    int nSelItem = pPathList->GetNextSelectedItem( pPos );

    if( nSelItem > 0 && nSelItem < (int)(m_MsSymbRsrcArr.size()) )
    {
      OdString strOld = m_MsSymbRsrcArr[nSelItem];
      m_MsSymbRsrcArr[nSelItem] = m_MsSymbRsrcArr[nSelItem - 1];
      m_MsSymbRsrcArr[nSelItem - 1] = strOld;

      updateListCtrl();

      pPathList->SetItemState( nSelItem - 1, LVIS_SELECTED, LVIF_TEXT);
      pPathList->SetFocus();
      UpdateData(FALSE);
    }
  }
}

void CSetRegVarsMsSymbRsrc::OnMoveDownButton()
{
  CListCtrl* pPathList = (CListCtrl*)(GetDlgItem(IDC_PATHLIST));

  POSITION pPos = pPathList->GetFirstSelectedItemPosition();

  if( pPos )
  {
    int nSelItem = pPathList->GetNextSelectedItem( pPos );

    if( nSelItem >= 0 && nSelItem < (int)(m_MsSymbRsrcArr.size() - 1) )
    {
      OdString strOld = m_MsSymbRsrcArr[nSelItem];
      m_MsSymbRsrcArr[nSelItem] = m_MsSymbRsrcArr[nSelItem + 1];
      m_MsSymbRsrcArr[nSelItem + 1] = strOld;

      updateListCtrl();
    }
  }
}

void CSetRegVarsMsSymbRsrc::OnDeleteButton()
{
  CListCtrl* pPathList = (CListCtrl*)(GetDlgItem(IDC_PATHLIST));

  POSITION pPos = pPathList->GetFirstSelectedItemPosition();

  if( pPos )
  {
    int nSelItem = pPathList->GetNextSelectedItem( pPos );

    if( nSelItem < (int)(m_MsSymbRsrcArr.size()) && nSelItem >= 0 )
    {
      m_MsSymbRsrcArr.removeAt( nSelItem );

      updateListCtrl();
    }
  }
}

void CSetRegVarsMsSymbRsrc::OnLvnItemchangedPathlist(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  // TODO: Add your control notification handler code here
  *pResult = 0;
}

void CSetRegVarsMsSymbRsrc::OnOK() 
{
  UpdateData();

  // ser "MS_SYMBRSRC" RegVar
  CString arraySumbRsrcStr;

  for( OdUInt32 i = 0; i < m_MsSymbRsrcArr.size(); i++ )
  {
    CString itemStr = m_MsSymbRsrcArr[i].c_str();

    arraySumbRsrcStr = arraySumbRsrcStr + itemStr;

    if( i < m_MsSymbRsrcArr.size() - 1 )
    {
      arraySumbRsrcStr += L";";
    }
  }

  switch( m_uType )
  {
    case kSymbRsrc:
    {
      m_pHostAppServices->setMS_SYMBRSRC(m_MsSymbRsrcArr);
      AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_SYMBRSRC"), arraySumbRsrcStr);
    } break;

    case kMatPalette:
    {
      m_pHostAppServices->setMS_MATPALETTE(m_MsSymbRsrcArr);
      AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_MATPALETTE"), arraySumbRsrcStr);
    } break;
  }

	CDialog::OnOK();
}


/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsRsrc

CSetRegVarsMsRsrc::CSetRegVarsMsRsrc(CWnd* pParent /*=NULL*/, CRegVarsType uType )
  : CDialog(CSetRegVarsMsRsrc::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSetRegVarsMsRsrc)
    // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT

  m_uType = uType;
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

  if( m_uType == kMsRsrc )
  {
    m_MsRsrc = m_pHostAppServices->getMS_RSRC().c_str();
    SetWindowText(L"Specify the main default resource file.");
  }
  else if( m_uType == kMsPenTable)
  {
    m_MsRsrc = m_pHostAppServices->getMS_PENTABLE().c_str();
    SetWindowText(L"Specify the pen table ( dgn plot style ) file.");
  }
  else if( m_uType == kMsDefCtbl)
  {
    m_MsRsrc = m_pHostAppServices->getMS_DEFCTBL().c_str();
    SetWindowText(L"Specify the default color table file.");
  }

  return   CDialog::OnInitDialog();
}

void CSetRegVarsMsRsrc::OnBnClickedMsRsrc()
{
#ifdef _TOOLKIT_IN_DLL_

  CString defaultExt;
  CString defaultFilter;
  CString strTitle;

  if( m_uType == kMsRsrc )
  {
    defaultExt    = _T("rsc"); 
    defaultFilter = _T("Symbology Resource (*.rsc)|*.rsc||");
    strTitle      = _T("Select Symbology Resource");
  }
  else if( m_uType == kMsPenTable)
  {
    defaultExt = _T("tbl");
    defaultFilter = _T("Pen Table (*.tbl)|*.tbl||");
    strTitle = _T("Select Pen Table File");
  }
  else if( m_uType == kMsDefCtbl)
  {
    defaultExt = _T("rsc");
    defaultFilter = _T("Color Table (*.tbl)|*.tbl||");
    strTitle = _T("Select Default Color Table File");
  }

  CFileDialog dlg( TRUE, defaultExt, _T(""), OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
    defaultFilter, ::AfxGetMainWnd() );
  dlg.m_ofn.lpstrTitle = strTitle;
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

  if( m_uType == kMsRsrc )
  {
    m_pHostAppServices->setMS_RSRC(sRsrc);
    AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_RSRC"), m_MsRsrc);
  }
  else if( m_uType == kMsPenTable)
  {
    m_pHostAppServices->setMS_PENTABLE(sRsrc);
    AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_PENTABLE"), m_MsRsrc);
  }
  else if( m_uType == kMsDefCtbl)
  {
    m_pHostAppServices->setMS_DEFCTBL(sRsrc);
    AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_DEFCTBL"), m_MsRsrc);
  }

	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsMsTexDir

CSetRegVarsMsTexDir::CSetRegVarsMsTexDir(CWnd* pParent /*=NULL*/, PropertyToSet iProperty )
: CDialog(CSetRegVarsMsTexDir::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSetRegVarsMsTexDir)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT

  m_iProperty = iProperty;
}

void CSetRegVarsMsTexDir::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CSetRegVarsMsRsrc)
  DDX_Text(pDX, IDC_PATH, m_MsTexDir);
  DDV_MaxChars(pDX, m_MsTexDir, 255);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetRegVarsMsTexDir, CDialog)
  //{{AFX_MSG_MAP(CSetRegVarsMsTexDir)
  ON_BN_CLICKED(IDC_GET_DIR, OnBnClickedMsTexDir)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSetRegVarsMsTexDir::OnInitDialog() 
{
  // Get value from profile and set MS_RSRC are here: COdaDgnApp::InitInstance()
  //m_MsRsrc = AfxGetApp()->GetProfileString(_T("RegVars"), _T("MS_RSRC"), _T(""));
  OdStringArray arraySymbRsrc;

  switch( m_iProperty )
  {
    case kMaterial:
    {
      m_pHostAppServices->getMS_MATERIAL(arraySymbRsrc);
      SetWindowText(L"Specify default directory for material texture file.");
    } break;

    case kPattern:
    {
      m_pHostAppServices->getMS_PATTERN(arraySymbRsrc);
      SetWindowText(L"Specify default directory for pattern image file.");
    } break;

    case kBump:
    {
      m_pHostAppServices->getMS_BUMP(arraySymbRsrc);
      SetWindowText(L"Specify default directory for bump map image file.");
    } break;

    case kMtbl:
    {
      m_pHostAppServices->getMS_MTBL(arraySymbRsrc);
      SetWindowText(L"Specify default directory for material table file.");
    } break;

    case kFontPath:
    {
      m_pHostAppServices->getMS_FONTPATH(arraySymbRsrc);
      SetWindowText(L"Specify default directory for shx font file.");
    } break;
  }

  if (arraySymbRsrc.size() > 0)
  {
    m_MsTexDir = arraySymbRsrc.at(0).c_str();
  }

  return   CDialog::OnInitDialog();
}

void CSetRegVarsMsTexDir::OnBnClickedMsTexDir()
{
  BROWSEINFO              bi;
  OdChar                  szDisplayName[MAX_PATH]; 
  LPITEMIDLIST            pidl; 
  ZeroMemory(&bi, sizeof(bi));      
  bi.hwndOwner = NULL; 
  bi.pszDisplayName = szDisplayName; 
  bi.lpszTitle = OD_T("Select default texture directory"); 
  bi.ulFlags = BIF_RETURNONLYFSDIRS;  
  pidl = SHBrowseForFolder(&bi); 

  if( pidl )
  {
    SHGetPathFromIDList(pidl, szDisplayName);
    m_MsTexDir = szDisplayName;
    UpdateData(FALSE);
  }
}

void CSetRegVarsMsTexDir::OnOK() 
{
  UpdateData();

  // set "MS_TEXDIFDIR" RegVar
  OdStringArray arrayTexDirs;
  OdString sDirRsrc( m_MsTexDir );
  arrayTexDirs.append( sDirRsrc );

  switch( m_iProperty )
  {
    case kMaterial:
    {
      m_pHostAppServices->setMS_MATERIAL(arrayTexDirs);
      AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_MATERIAL"), m_MsTexDir);
    } break;

    case kPattern:
    {
      m_pHostAppServices->setMS_PATTERN(arrayTexDirs);
      AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_PATTERN"), m_MsTexDir);
    } break;

    case kBump:
    {
      m_pHostAppServices->setMS_BUMP(arrayTexDirs);
      AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_BUMP"), m_MsTexDir);
    } break;

    case kMtbl:
    {
      m_pHostAppServices->setMS_MTBL(arrayTexDirs);
      AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_MTBL"), m_MsTexDir);
    } break;    
    
    case kFontPath:
    {
      m_pHostAppServices->setMS_FONTPATH(arrayTexDirs);
      AfxGetApp()->WriteProfileString(_T("RegVars"), _T("MS_FONTPATH"), m_MsTexDir);
    } break;
  }

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
  //m_MsRsrc = AfxGetApp()->GetProfileString(_T("RegVars"), _T("FAST_FONT"), _T(""));
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

/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsNurbs

CSetRegVarsNurbs::CSetRegVarsNurbs(CWnd* pParent /*=NULL*/)
: CDialog(CSetRegVarsNurbs::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSetRegVarsNurbs)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  m_iGridSize  = 10;
}

void CSetRegVarsNurbs::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CSetRegVarsFastFont)
  DDX_Text(pDX, IDC_NURNS_EDIT, m_iGridSize);
  DDV_MinMaxInt(pDX, m_iGridSize, 0, 1000 );
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetRegVarsNurbs, CDialog)
  //{{AFX_MSG_MAP(CSetRegVarsFastFont)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSetRegVarsNurbs::OnInitDialog() 
{
  // Get value from profile and set TG_NURBS_GRID (if it's required) are here: COdaDgnApp::InitInstance()
  //m_MsRsrc = AfxGetApp()->GetProfileString(_T("RegVars"), _T("TG_NURBS_GRID"), _T(""));
  m_iGridSize = m_pHostAppServices->getTG_NURBS_GRID();

  return   CDialog::OnInitDialog();
}

void CSetRegVarsNurbs::OnOK() 
{
  UpdateData();

  // set "TG_NURBS_GRID" RegVar
  m_pHostAppServices->setTG_NURBS_GRID(m_iGridSize);

  AfxGetApp()->WriteProfileInt(_T("RegVars"), _T("TG_NURBS_GRID"), m_iGridSize);

  CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CSetRegVarsGripPoints

CSetRegVarsGripPoints::CSetRegVarsGripPoints(CWnd* pParent /*=NULL*/)
: CDialog(CSetRegVarsGripPoints::IDD, pParent)
{
  //{{AFX_DATA_INIT(CSetRegVarsGripPoints)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
  m_uGripSize     = 5;
  m_uGripObjLimit = 100;
  m_uGripColor    = 119;
  m_uGripHover    = 4;
  m_uGripHot      = 2;
}

void CSetRegVarsGripPoints::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(CSetRegVarsFastFont)
  DDX_Text(pDX, IDC_GRIPSIZE_EDIT, m_uGripSize);
  DDV_MinMaxInt(pDX, m_uGripSize, 0, 255 );
  DDX_Text(pDX, IDC_GRIPOBJLIMIT_EDIT, m_uGripObjLimit);
  DDV_MinMaxInt(pDX, m_uGripSize, 0, 32000 );
  DDX_CBIndex( pDX, IDC_BASE_COMBO, m_uGripColor );
  DDX_CBIndex( pDX, IDC_HOVER_COMBO, m_uGripHover );
  DDX_CBIndex( pDX, IDC_HOT_COMBO, m_uGripHot );
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSetRegVarsGripPoints, CDialog)
  //{{AFX_MSG_MAP(CSetRegVarsGripPoints)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSetRegVarsGripPoints::OnInitDialog() 
{
  m_uGripSize     = m_pHostAppServices->getDG_GRIPSIZE();
  m_uGripObjLimit = m_pHostAppServices->getDG_GRIPOBJLIMIT();
  m_uGripColor    = m_pHostAppServices->getDG_GRIPCOLOR();
  m_uGripHover    = m_pHostAppServices->getDG_GRIPHOVER();
  m_uGripHot      = m_pHostAppServices->getDG_GRIPHOT();

  CComboBox* pCBColor = (CComboBox*)(GetDlgItem( IDC_BASE_COMBO ));
  CComboBox* pCBHover = (CComboBox*)(GetDlgItem( IDC_HOVER_COMBO ));
  CComboBox* pCBHot   = (CComboBox*)(GetDlgItem( IDC_HOT_COMBO ));

  pCBColor->Clear();
  pCBHover->Clear();
  pCBHot->Clear();

  for( OdUInt32 i = 0; i < 256; i++ )
  {
    CString strData;
    strData.Format(L"%d", i);
    pCBColor->AddString( strData );
    pCBHover->AddString( strData );
    pCBHot->AddString( strData );
  }

  UpdateData( false );

  return   CDialog::OnInitDialog();
}

void CSetRegVarsGripPoints::OnOK() 
{
  UpdateData( true );

  if( m_uGripColor < 0 )
  {
    m_uGripColor = 0;
  }

  if( m_uGripHover < 0 )
  {
    m_uGripHover = 0;
  }

  if( m_uGripHot < 0 )
  {
    m_uGripHot = 0;
  }

  m_pHostAppServices->setDG_GRIPSIZE( m_uGripSize );
  m_pHostAppServices->setDG_GRIPOBJLIMIT( m_uGripObjLimit );
  m_pHostAppServices->setDG_GRIPCOLOR( m_uGripColor );
  m_pHostAppServices->setDG_GRIPHOVER( m_uGripHover );
  m_pHostAppServices->setDG_GRIPHOT( m_uGripHot );

  AfxGetApp()->WriteProfileInt(_T("RegVars"), _T("DG_GRIPSIZE"), m_uGripSize);
  AfxGetApp()->WriteProfileInt(_T("RegVars"), _T("DG_GRIPOBJLIMIT"), m_uGripObjLimit);
  AfxGetApp()->WriteProfileInt(_T("RegVars"), _T("DG_GRIPCOLOR"), m_uGripColor);
  AfxGetApp()->WriteProfileInt(_T("RegVars"), _T("DG_GRIPHOVER"), m_uGripHover);
  AfxGetApp()->WriteProfileInt(_T("RegVars"), _T("DG_GRIPHOT"), m_uGripHot);

  CDialog::OnOK();
}
