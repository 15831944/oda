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

// OdaMultiLineStyleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OdaMultiLineStyleDlg.h"
#include "DbDictionary.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// OdaMultiLineStyleDlg dialog
//

IMPLEMENT_DYNCREATE(OdaMultiLineStyleDlg, CDialog)

OdaMultiLineStyleDlg::OdaMultiLineStyleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OdaMultiLineStyleDlg::IDD, pParent), m_pDb(NULL)
{
	//{{AFX_DATA_INIT(OdaLayerPropLineTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

OdaMultiLineStyleDlg::OdaMultiLineStyleDlg(OdDbDatabase *pDb, CWnd* pParent /*=NULL*/)
	: CDialog(OdaMultiLineStyleDlg::IDD, pParent), m_pDb(pDb)
{
	//{{AFX_DATA_INIT(OdaLayerPropLineTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(OdaMultiLineStyleDlg, CDialog)
	//{{AFX_MSG_MAP(OdaMultiLineStyleDlg)
	    ON_WM_SIZE()
        ON_CBN_SELCHANGE(IDC_COMBO_MLSTYLE_CURRENT, OnSelchangeComboMLStyleCurrent)
    	ON_BN_CLICKED(IDC_BUTTON_MLSTYLE_LOAD, OnClickBtnMLStyleLoad)
    	ON_BN_CLICKED(IDC_BUTTON_MLSTYLE_SAVE, OnClickBtnMLStyleSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void OdaMultiLineStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OdaLayerPropColorDlg)
	DDX_Control(pDX, IDC_COMBO_MLSTYLE_CURRENT, m_comboMLStyleCurrent);
	DDX_Control(pDX, IDC_EDIT_MLSTYLE_NAME, m_editMLStyleName);
	//}}AFX_DATA_MAP
}

BOOL OdaMultiLineStyleDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    if( !m_pDb ) return FALSE;

    if( fillComboMLStyleCurrent() )
    {
        m_pDb->startUndoRecord();
    	return TRUE;  
    }

	return FALSE;  
}


void OdaMultiLineStyleDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
}

void OdaMultiLineStyleDlg::OnOK()
{

    CDialog::OnOK();

}

void OdaMultiLineStyleDlg::OnCancel()
{

    m_pDb->undo();
    CDialog::OnCancel();

}


void OdaMultiLineStyleDlg::OnSelchangeComboMLStyleCurrent() 
{
    int idx = m_comboMLStyleCurrent.GetCurSel();
    CString string;
    m_comboMLStyleCurrent.GetLBText(idx, string);
    m_editMLStyleName.SetWindowText(string);

}

void OdaMultiLineStyleDlg::OnClickBtnMLStyleLoad()
{

  OdString sPath = COdaMfcAppApp::BrowseWithPreview( m_hWnd,
    _T("MLN files (*.mln)|*.mln|All Files (*.*)|*.*||"));
  if(sPath.getLength())
  {
        OdStreamBufPtr pFile = 
          odSystemServices()->createFile(sPath, Oda::kFileRead, Oda::kShareDenyRead, Oda::kOpenExisting);
        odDbLoadMlineStyleFile( *m_pDb, OD_T("*"), *pFile, OdDb::kDltReplace );
  }

  fillComboMLStyleCurrent();
}

const bool OdaMultiLineStyleDlg::fillComboMLStyleCurrent()
{
   if( !m_pDb ) return false;

   m_comboMLStyleCurrent.ResetContent();

   OdDbDictionaryPtr pDic = m_pDb->database()->getMLStyleDictionaryId().safeOpenObject();
   OdDbDictionaryIteratorPtr pIt = pDic->newIterator();
   while(!pIt->done())
   {
     OdString sMLName = pIt->name();
     int index = m_comboMLStyleCurrent.AddString(sMLName);
     m_comboMLStyleCurrent.SetCurSel(index);
     pIt->next();
   }

   return true;
}

void OdaMultiLineStyleDlg::OnClickBtnMLStyleSave()
{

    CString newName;
    bool bReplace = true;
    if (DoPromptFileName( newName,
              bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
              OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST, FALSE ))
    {
        OdStreamBufPtr pFile = 
           odSystemServices()->createFile(OdString((LPCTSTR)newName), Oda::kFileWrite, Oda::kShareDenyNo, Oda::kCreateAlways);
        odDbSaveMlineStyleFile( *m_pDb, *pFile );
    }
}


BOOL OdaMultiLineStyleDlg::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog)
{

  CFileDialog dlgFile(bOpenFileDialog);

  CString title;
  VERIFY(title.LoadString(nIDSTitle));

  dlgFile.m_ofn.Flags |= lFlags;

  CString strFilter;
  CString strDefault;

  strFilter = _T("mln file format (*.mln)");
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += _T("*.mln");
  strFilter += (TCHAR)'\0';   // last string
  dlgFile.m_ofn.nMaxCustFilter++;
  dlgFile.m_ofn.nFilterIndex = 1;

  dlgFile.m_ofn.lpstrFilter = strFilter;
  dlgFile.m_ofn.lpstrTitle = title;
  dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

  int nResult = dlgFile.DoModal();
  fileName.ReleaseBuffer();

  if( fileName.IsEmpty() ) return FALSE;

  int pos = fileName.ReverseFind('.');
  if( pos >= 0 )
  {
       fileName = fileName.Left(pos);
  }

  fileName += _T(".mln");

  return nResult == IDOK;
}
