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

//
// PublishFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

#include "OdaCommon.h"
#include "Gs/Gs.h"

#include "PublishFileDlg.h"

#include "DbDatabase.h"
#include "DbLayout.h"
#include "DbDictionary.h"
#include "DbBlockTableRecord.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CPublishSheets

CPublishSheets::CPublishSheets(DwExportParams* pParams)
	: CDialog(CPublishSheets::IDD, NULL)
{
	//{{AFX_DATA_INIT(CPublishSheets)
  m_radioFormat = enAscii;
  m_pParams = pParams;

  //m_sFileName = m_pParams->sSourceFileName;
  //m_sFileName = (const TCHAR*)m_pParams->pDb->getFilename(); // before 09.01.2012
  OdDbBaseDatabasePEPtr pDbPE(m_pParams->database());
  m_sFileName = (const TCHAR*)pDbPE->getFilename(m_pParams->database()); // dgn dwf export
  if (m_sFileName.Find('.') != (-1))
  {
    m_sFileName = m_sFileName.Left( m_sFileName.Find('.') );
  }
  m_sFileName += ".dwf";

  m_sPassword = _T("");
	//}}AFX_DATA_INIT
}

CPublishSheets::~CPublishSheets()
{
}

void CPublishSheets::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPublishSheets)
	DDX_Control(pDX, IDC_PUBLISH_LAYOUTS_LIST, m_listSheets);
	DDX_Radio(pDX, IDC_PUBLISH_BINARY_RADIO, (*(int *) & m_radioFormat));
	DDX_Text(pDX, IDC_PUBLISH_FILE_NAME_EDIT, m_sFileName);
	DDX_Text(pDX, IDC_PUBLISH_PASSWORD_EDIT, m_sPassword);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPublishSheets, CDialog)
	//{{AFX_MSG_MAP(CPublishSheets)
	ON_BN_CLICKED(IDC_PUBLISH_BROWSE_BUTTON, OnBrowse)
  ON_NOTIFY(NM_CLICK, IDC_PUBLISH_LAYOUTS_LIST, OnClickedLayoutsList)
  //ON_NOTIFY(NM_DBLCLK, IDC_PUBLISH_LAYOUTS_LIST, OnDblClickedLayoutsList)
	ON_BN_CLICKED(IDC_PUBLISH_REMOVE_BUTTON, OnRemove)
	ON_BN_CLICKED(IDC_PUBLIS_SETTING_BUTTON, OnSetting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CPublishSheets::OnInitDialog()
{
  CDialog::OnInitDialog();

	m_listSheets.SetExtendedStyle(m_listSheets.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	m_listSheets.InsertColumn(0, _T("Layout Name"), LVCFMT_LEFT, 100);
	m_listSheets.InsertColumn(1, _T("Sheet Name"), LVCFMT_LEFT, 120);
	m_listSheets.InsertColumn(2, _T("Drawing Name"), LVCFMT_LEFT, 230);
	m_listSheets.InsertColumn(3, _T("Author"), LVCFMT_LEFT, 80);
	m_listSheets.InsertColumn(4, _T("Subject"), LVCFMT_LEFT, 80);
	m_listSheets.InsertColumn(5, _T("Preview Name"), LVCFMT_LEFT, 230);
	m_listSheets.InsertColumn(6, _T("Comments"), LVCFMT_LEFT, 80);

  InitPagaDate();
  FillListSheets();

  GetDlgItem(IDC_PUBLISH_REMOVE_BUTTON)->EnableWindow((m_listSheets.GetSelectedCount() > 0) ? true : false );
  GetDlgItem(IDC_PUBLIS_SETTING_BUTTON)->EnableWindow((m_listSheets.GetSelectedCount() > 0) ? true : false );

  GetDlgItem(IDC_PUBLISH_PASSWORD_EDIT)->EnableWindow( false );

  UpdateData(FALSE);

  return TRUE;
}


void CPublishSheets::OnOK()
{
  UpdateData();                   // TRUE - data is being retrieved 

  // process getting data

  if (m_sFileName.Find(_T(".dwf")) == -1)
  {
    m_sFileName += _T(".dwf");
  }

  m_pParams->setDwfFileName(OdString(m_sFileName));

  switch (m_radioFormat)
  {
  case enBinary:
    m_pParams->setFormat(DW_UNCOMPRESSED_BINARY);
    m_pParams->setVersion(nDwf_v60);
    break;

  case enAscii:
    m_pParams->setFormat(DW_ASCII);
    m_pParams->setVersion(nDwf_v60);
    break;

  default:
    ODA_ASSERT(false);
  }

  //m_pParams->sPassword = m_sPassword;

  // to add page stuff if there is...
  for (UINT i = 0; i < m_arPageData.size(); i++)
  {
    m_pParams->pageData().append(m_arPageData[i]);
  }
/*
  DwfPageData pageData;
 
    pageData.pPageAuthor = "Michael";
    pageData.pPageTitle = "the_2d_layout";
    pageData.pPageCompany = "SoftDev";
    pageData.pPageComments = " this is a my test";
    pageData.pPageReviewers = "it's me";
    pageData.pPageKeywords;
    pageData.pPageDescription = " look at a result";
    pageData.pPageCopyright = "today";

    CString thumbnailFileName("d:/tmp/dwf/1/thum.jpg");
    pageData.thumbnail.pFileName = thumbnailFileName;
    pageData.thumbnail.ColorDepth = 256;       // the number of colors used in the image
    pageData.thumbnail.Width = 1020;            // in orger to set the extents of the 
    pageData.thumbnail.Height = 610;           //  image, in pixels

    CString previewFileName("d:/tmp/dwf/1/prev.jpg");
    pageData.preview.pFileName = previewFileName;
    pageData.preview.ColorDepth = 256;       // the number of colors used in the image
    pageData.preview.Width = 1020;            // in orger to set the extents of the 
    pageData.preview.Height = 610;           //  image, in pixels

    DwfPageData::FontResource font;

    font.pFileName      = "d:/tmp/dwf/1/embeddedFont.tmp";
    font.request        = DwfPageData::FontResource::Raw | DwfPageData::FontResource::Compressed; 
    font.privilege      = DwfPageData::FontResource::Editable;
    font.characterCode  = DwfPageData::FontResource::Unicode;
    font.pCanonicalName = "Swiss 721 Bold Condensed Outline BT"; 
    font.pLogfontName   = "Swis721 BdCnOul BT";

    pageData.arFonts.append(font);
 
    params.arPageData.append(pageData);

    DwfPageData pageData_1;
    params.arPageData.append(pageData_1);
*/

  CDialog::OnOK();
}

void CPublishSheets::OnCancel()
{
  CDialog::OnCancel();
}

void CPublishSheets::OnBrowse()
{
  DWORD lFlags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST;

  CFileDialog dlgFile(false);

  dlgFile.m_ofn.Flags |= lFlags;

  CString strFilter;

  strFilter = "Deisgn Web Format (*.dwf)";
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += _T("*.dwf");
  strFilter += (TCHAR)'\0';   // last string

  dlgFile.m_ofn.lpstrDefExt = _T("dwf");
  dlgFile.m_ofn.lpstrFilter = strFilter;
  dlgFile.m_ofn.lpstrTitle = _T("Select DWF file");

  CString strFileName;
  GetDlgItem(IDC_PUBLISH_FILE_NAME_EDIT)->GetWindowText(strFileName);
  dlgFile.m_ofn.lpstrFile = strFileName.GetBuffer(_MAX_PATH);

  LPARAM nRet = dlgFile.DoModal();
  strFileName.ReleaseBuffer();
  if (nRet == IDOK)
  {
    CString strFileName = dlgFile.GetPathName();
    if (strFileName.Find('.') == -1)
    {
      strFileName += ".dwf";
    }

	  GetDlgItem(IDC_PUBLISH_FILE_NAME_EDIT)->SetWindowText(strFileName);
//	  GetDlgItem(IDC_PUBLISH_FILE_NAME_EDIT)->SendMessage(EM_SETMODIFY, 0, 0);

    UpdateData();                   // TRUE - data is being retrieved 
  }
}

void CPublishSheets::InitPagaDate()
{
  //CString strSeed = m_pParams->sSourceFileName;
  //CString strSeed = (LPCTSTR)m_pParams->pDb->getFilename(); // before 09.01.2012

  OdDbBaseDatabase *pDb = m_pParams->database();
  OdDbBaseDatabasePEPtr pDbPE(pDb);
  CString strSeed = (LPCTSTR)pDbPE->getFilename(pDb);
  if (strSeed.ReverseFind('\\') != (-1))
  {
    strSeed = strSeed.Right( strSeed.GetLength() - strSeed.ReverseFind('\\') - 1 );
  }
  if (strSeed.Find('.') != (-1))
  {
    strSeed = strSeed.Left( strSeed.Find('.') );
  }


  // To set Model Space the first
  //
  //OdDbObjectId idMsLayout = OdDbBlockTableRecordPtr(m_pParams->pDb->getModelSpaceId().safeOpenObject())->getLayoutId(); // before 09.01.2012

  //OdDbLayoutPtr pMsLayout = idMsLayout.safeOpenObject();
  OdRxObjectPtr idMsLayout = pDbPE->getFirstLayout(pDb);
  OdDbBaseLayoutPEPtr pMsLayout(idMsLayout);

  {
    DwfPageData pageData;
    CString str;

    CString strLayoutName = (LPCTSTR)pMsLayout->name(idMsLayout.get());//(pMsLayout.get());//->getLayoutName();
    pageData.sLayout = strLayoutName;

    // init default sheet name
    CString strSheetName = strSeed + '-' + strLayoutName;
    pageData.sPageTitle = strSheetName;

    m_arPageData.append(pageData);
  }

  // To get the access paper layouts in this database.
  //OdDbDictionaryPtr         pLayoutDict = m_pParams->pDb->getLayoutDictionaryId().safeOpenObject();
  //OdDbDictionaryIteratorPtr pLayoutIter = pLayoutDict->newIterator();
  OdRxIteratorPtr pLayoutIter = pDbPE->layouts(pDb);

  for (; !pLayoutIter->done(); pLayoutIter->next())
  {
    OdDbBaseLayoutPEPtr pLayout(pLayoutIter->object());
    //OdDbObjectId idCur = pLayoutIter->objectId();
    OdDbStub* idCur = pDbPE->getId(pLayoutIter->object());
    OdString nameCur = pLayout->name(pLayoutIter->object());
    if (idCur == pDbPE->getId(idMsLayout.get()))
    {
      continue;
    }

    DwfPageData pageData;
    CString str;

    //OdDbLayoutPtr pLayout = pLayoutIter->objectId().safeOpenObject();
    CString strLayoutName = (LPCTSTR)pLayout->name(pLayoutIter->object());

    pageData.sLayout = strLayoutName;

    // init default sheet name
    CString strSheetName = strSeed + '-' + strLayoutName;
    pageData.sPageTitle = strSheetName;

    m_arPageData.append(pageData);
  }
}

void CPublishSheets::FillItem(UINT i)
{
  DwfPageData pageData = m_arPageData[i];

  OdDbBaseDatabasePEPtr pDbPE(m_pParams->database());

  m_listSheets.SetItemText( i, 0, pageData.sLayout );           // Layout Name
  m_listSheets.SetItemText( i, 1, pageData.sPageTitle );        // Sheet Name
  //m_listSheets.SetItemText( i, 2, m_pParams->pDb->getFilename() ); // Drawning Name
  m_listSheets.SetItemText( i, 2, pDbPE->getFilename(m_pParams->database()) );
  m_listSheets.SetItemText( i, 3, pageData.sPageAuthor);        // Author
  m_listSheets.SetItemText( i, 4, pageData.sPageSubject);       // Subject
  m_listSheets.SetItemText( i, 5, pageData.preview.sFileName);  // Preview
  m_listSheets.SetItemText( i, 6, pageData.sPageComments);      // Comments
}

void CPublishSheets::FillListSheets()
{
  m_listSheets.DeleteAllItems();

  for (UINT i = 0; i < m_arPageData.size(); i++)
  {
    //DwfPageData pageData = m_arPageData[i];

    CString str;
    m_listSheets.InsertItem(i, str);   // insert the item in the list control

    FillItem(i);
    /*
    m_listSheets.SetItemText( i, 0, pageData.sLayout );     // Layout Name
    m_listSheets.SetItemText( i, 1, pageData.sPageTitle );  // Sheet Name
    m_listSheets.SetItemText( i, 2, m_pParams->sSourceFileName );       // Drawning Name
    */
  }

  //int nColumnCount = pmyListCtrl->GetHeaderCtrl()->GetItemCount();

}

void CPublishSheets::OnClickedLayoutsList(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
  *pResult = 0;
  GetDlgItem(IDC_PUBLISH_REMOVE_BUTTON)->EnableWindow((m_listSheets.GetSelectedCount() > 0) ? true : false );
  GetDlgItem(IDC_PUBLIS_SETTING_BUTTON)->EnableWindow((m_listSheets.GetSelectedCount() > 0) ? true : false );
}

/*
void CPublishSheets::OnDblClickedLayoutsList()
{
  // OnSettings...
  // SendMessage
}
*/

void CPublishSheets::OnRemove()
{
  if (m_listSheets.GetSelectedCount() > 0)
  {
    int nItem = m_listSheets.GetNextItem(-1, LVNI_SELECTED);

    ODA_ASSERT(nItem != -1);

    m_listSheets.DeleteItem( nItem );
    m_arPageData.removeAt( nItem );
  }

  GetDlgItem(IDC_PUBLISH_REMOVE_BUTTON)->EnableWindow((m_listSheets.GetSelectedCount() > 0) ? true : false );
  GetDlgItem(IDC_PUBLIS_SETTING_BUTTON)->EnableWindow((m_listSheets.GetSelectedCount() > 0) ? true : false );
}

void CPublishSheets::OnSetting()
{
  if (m_listSheets.GetSelectedCount() > 0)
  {
    int nItem = m_listSheets.GetNextItem(-1, LVNI_SELECTED);

    ODA_ASSERT(nItem != -1);

    DwfPageData pageData = m_arPageData[nItem];
    //CString strDrawingName = (LPCTSTR)m_pParams->pDb->getFilename();
    OdDbBaseDatabasePEPtr pDbPE(m_pParams->database());
    CString strDrawingName = (LPCTSTR)pDbPE->getFilename(m_pParams->database());

    CPublishSetting  dlgSetting(pageData, strDrawingName);
    if (dlgSetting.DoModal() == IDOK)
    {
      m_arPageData[nItem] = pageData;

      FillItem(nItem);

      m_listSheets.Update(nItem);

//      UpdateData(FALSE);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////

CPublishSetting::CPublishSetting(DwfPageData& pageData, CString& sDrawing)
	: CDialog(CPublishSetting::IDD, NULL)
  , m_pageData(pageData)
{
  m_sLayout       = (LPCTSTR)pageData.sLayout;
  m_sSheet        = (LPCTSTR)pageData.sPageTitle;
  m_sDrawing      = sDrawing;
  m_sSubject      = (LPCTSTR)pageData.sPageSubject;
  m_sCompany      = (LPCTSTR)pageData.sPageCompany;
  m_sComments     = (LPCTSTR)pageData.sPageComments;
  m_sAuthor       = (LPCTSTR)pageData.sPageAuthor;
  m_sPreviewFile  = (LPCTSTR)pageData.preview.sFileName;
  m_nWidth        = pageData.preview.Width;
  m_nHeight       = pageData.preview.Height;
  m_nColorDepth   = pageData.preview.ColorDepth;
}

CPublishSetting::~CPublishSetting()
{
}

void CPublishSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPublishSetting)
	DDX_Text(pDX, IDC_PUBLISH_SET_LAYOUT,       m_sLayout);
	DDX_Text(pDX, IDC_PUBLISH_SET_SHEET,        m_sSheet);
	DDX_Text(pDX, IDC_PUBLISH_SET_DRAWING,      m_sDrawing);
	DDX_Text(pDX, IDC_PUBLISH_SET_SUBJECT,      m_sSubject);
	DDX_Text(pDX, IDC_PUBLISH_SET_COMPANY,      m_sCompany);
	DDX_Text(pDX, IDC_PUBLISH_SET_COMMENTS,     m_sComments);
	DDX_Text(pDX, IDC_PUBLISH_SET_AUTHOR,       m_sAuthor);
	DDX_Text(pDX, IDC_PUBLISH_SET_PREVIEW_FILE, m_sPreviewFile);
	DDX_Text(pDX, IDC_PUBLISH_SET_WIDTH,        m_nWidth);
	DDX_Text(pDX, IDC_PUBLISH_SET_HEIGHT,       m_nHeight);
	DDX_Text(pDX, IDC_PUBLISH_SET_COLOR_DEPTH,  m_nColorDepth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPublishSetting, CDialog)
	//{{AFX_MSG_MAP(CPublishSetting)
	ON_BN_CLICKED(IDC_PUBLISH_SET_BROWSE_PREVIEW, OnBrowsePreview)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CPublishSetting::OnInitDialog()
{
  CDialog::OnInitDialog();

  GetDlgItem(IDC_PUBLISH_SET_DRAWING)->EnableWindow( false );
  GetDlgItem(IDC_PUBLISH_SET_LAYOUT)->EnableWindow( false );

  UpdateData(FALSE);

  return TRUE;
}

void CPublishSetting::OnOK()
{
  UpdateData();                   // TRUE - data is being retrieved 

  m_pageData.sPageTitle         = m_sSheet;
  m_pageData.sPageSubject       = m_sSubject;
  m_pageData.sPageCompany       = m_sCompany;
  m_pageData.sPageComments      = m_sComments;
  m_pageData.sPageAuthor        = m_sAuthor;
  m_pageData.preview.sFileName  = m_sPreviewFile;
  m_pageData.preview.Width      = m_nWidth;
  m_pageData.preview.Height     = m_nHeight;
  m_pageData.preview.ColorDepth = m_nColorDepth ;

  CDialog::OnOK();
}

void CPublishSetting::OnBrowsePreview()
{
  DWORD lFlags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_READONLY;

  CFileDialog dlgFile(true);

  dlgFile.m_ofn.Flags |= lFlags;

  CString strFilter;

  strFilter = "JPEG Files (*.jpg;*.jpeg)";
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += _T("*.jpg");
  strFilter += (TCHAR)'\0';   // last string

  strFilter += "PNG Files (*.png)";
  strFilter += (TCHAR)'\0';   // next string please
  strFilter += _T("*.png");
  strFilter += (TCHAR)'\0';   // last string

  dlgFile.m_ofn.lpstrFilter = strFilter;
  dlgFile.m_ofn.lpstrTitle = _T("Select Preview file");

  CString strFileName;
  GetDlgItem(IDC_PUBLISH_SET_PREVIEW_FILE)->GetWindowText(strFileName);
  dlgFile.m_ofn.lpstrFile = strFileName.GetBuffer(_MAX_PATH);

  LPARAM nRet = dlgFile.DoModal();
  strFileName.ReleaseBuffer();
  if (nRet == IDOK)
  {
    CString strPreview = dlgFile.GetPathName();

    switch(dlgFile.m_ofn.nFilterIndex)
    {
      case 1:
        if (strPreview.Find('.') == -1)
        {
          strPreview += ".jpg";
        }
        break;
      case 2:
        if (strPreview.Find('.') == -1)
        {
          strPreview += ".png";
        }
        break;
      default:
        ODA_ASSERT(false);
        return;
    }

	  GetDlgItem(IDC_PUBLISH_SET_PREVIEW_FILE)->SetWindowText(strPreview);

    UpdateData();                   // TRUE - data is being retrieved 
  }
}
