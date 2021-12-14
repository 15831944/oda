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

// SummDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "SummDlg.h"
#include "SummInfo.h"
#include "DwgView.h"
#include "OdaMfcAppDoc.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSummDlg dialog


CSummDlg::CSummDlg(OdDbDatabase *pDb, CWnd* pParent /*=NULL*/)
	: CDialog(CSummDlg::IDD, pParent)
  , m_pDb(pDb)
{
	//{{AFX_DATA_INIT(CSummDlg)
	//}}AFX_DATA_INIT
}


void CSummDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSummDlg)
	DDX_Control(pDX, IDC_SUMMINFO_LIST, m_SummInfoList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSummDlg, CDialog)
	//{{AFX_MSG_MAP(CSummDlg)
	//}}AFX_MSG_MAP
  ON_NOTIFY(LVN_KEYDOWN, IDC_SUMMINFO_LIST, &CSummDlg::OnLvnKeydownSumminfoList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSummDlg message handlers

#define SUMINFO_ITEM(itemName, itemValue) \
  m_SummInfoList.InsertItem(number, _T("")); \
  m_SummInfoList.SetItemText(number, 1, itemName); \
  m_SummInfoList.SetItemText(number, 2, itemValue); \
  number++;


BOOL CSummDlg::OnInitDialog() 
{
  USES_CONVERSION;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  m_SummInfoList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 50);
  m_SummInfoList.InsertColumn(1, _T("Name"), LVCFMT_LEFT, 100);
  m_SummInfoList.InsertColumn(2, _T("Value"), LVCFMT_LEFT, 100);
  m_SummInfoList.DeleteAllItems();  

  OdDbDatabaseSummaryInfoPtr pSI = oddbGetSummaryInfo(m_pDb);
  OdString tmp;

  int number = 0;
  char out[20];
  
  SUMINFO_ITEM(_T("HyperlinkBase"), pSI->getHyperlinkBase())
  SUMINFO_ITEM(_T("Title"),         pSI->getTitle())
  SUMINFO_ITEM(_T("Subject"),       pSI->getSubject())
  SUMINFO_ITEM(_T("Author"),        pSI->getAuthor())
  SUMINFO_ITEM(_T("Comments"),      pSI->getComments())
  SUMINFO_ITEM(_T("Keywords"),      pSI->getKeywords())
  SUMINFO_ITEM(_T("LastSavedBy"),   pSI->getLastSavedBy())

  SUMINFO_ITEM(_T("Database file name"), m_pDb->getFilename())
  SUMINFO_ITEM(_T("Approximate number of objects"), A2T(itoa(m_pDb->approxNumObjects(), out, 10)))
  SUMINFO_ITEM(_T("Number of saves"), A2T((char*)itoa(m_pDb->numberOfSaves(), out, 10)))
  OdDb::MaintReleaseVer mVer;
  OdDb::DwgVersion dwgVer = m_pDb->version(&mVer);
  SUMINFO_ITEM(_T("Maintenance release version"), A2T((char*)itoa(mVer, out, 10)))
  dwgVer = m_pDb->originalFileVersion(&mVer);
  SUMINFO_ITEM(_T("Original file version"), A2T((char*)OdDb::DwgVersionToStr(dwgVer)))
  SUMINFO_ITEM(_T("Original file maintenance version"), A2T((char*)itoa(mVer, out, 10)))
  dwgVer = m_pDb->lastSavedAsVersion(&mVer);
  SUMINFO_ITEM(_T("Last saved as version"), dwgVer == OdDb::kDHL_MC0_0 ? _T("kDHL_MC0_0") : A2T((char*)OdDb::DwgVersionToStr(dwgVer)))
  SUMINFO_ITEM(_T("Last saved as maintenance version"), A2T((char*)itoa(mVer, out, 10)))
  dwgVer = m_pDb->originalFileSavedByVersion(&mVer);
  SUMINFO_ITEM(_T("Application's created this file version"), dwgVer == OdDb::kDHL_MC0_0 ? _T("kDHL_MC0_0") : A2T((char*)OdDb::DwgVersionToStr(dwgVer)))
  SUMINFO_ITEM(_T("Application's created this file maintenance version"), A2T((char*)itoa(mVer, out, 10)))
  // File size
  CString fileName = (LPCTSTR)m_pDb->getFilename();
  if (fileName.GetLength() > 0)
  {
    CFile file(fileName, CFile::modeRead | CFile::shareDenyNone);
    //  pmk 9.1.2004 : conversion from 'ULONGLONG' to 'int', possible loss of data
    SUMINFO_ITEM(_T("File size"), A2T(itoa((int) file.GetLength(), out, 10)))
    CFileStatus fileStatus;
    file.GetStatus(fileStatus);
    SUMINFO_ITEM(_T("Last modified date"), fileStatus.m_mtime.Format(_T("%A, %B %d, %Y")))
  }
  else
  {
    SUMINFO_ITEM(_T("File size"), _T("unknown"))
    SUMINFO_ITEM(_T("Last modified date"), _T("unknown"))
  }
  
  number = m_SummInfoList.GetItemCount();
  for(int f = 0; f < pSI->numCustomInfo(); ++f )
  {
    OdString key, val;
    pSI->getCustomSummaryInfo(f, key, val);
    m_SummInfoList.InsertItem(number, A2T(itoa( f, out, 10)));
    m_SummInfoList.SetItemText(number, 1, key);
    m_SummInfoList.SetItemText(number, 2, val);
    number++;
  }
	return TRUE;
}


void CSummDlg::OnLvnKeydownSumminfoList(NMHDR* pNMHDR, LRESULT* pResult)
{
  LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
  if (pLVKeyDow->wVKey = VK_DELETE)
  {
    if (POSITION p = m_SummInfoList.GetFirstSelectedItemPosition()) 
    {
      int n = m_SummInfoList.GetNextSelectedItem(p);
      if (!m_SummInfoList.GetItemText(n, 0).IsEmpty())
      {
        CString txt = m_SummInfoList.GetItemText(n, 1);
        OdDbDatabaseSummaryInfoPtr pSI = oddbGetSummaryInfo(m_pDb);
        pSI->deleteCustomSummaryInfo(OdString(txt));
        oddbPutSummaryInfo(pSI);
        m_SummInfoList.DeleteItem(n);
      }
    }
  }
  *pResult = 0;
}
