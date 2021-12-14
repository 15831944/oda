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
#include "OpenDGN_Viewer.h"
#include "SummDlg.h"
#include "DgSummaryInfo.h"
#include "OpenDGN_ViewerDoc.h"
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSummDlg dialog


CSummDlg::CSummDlg(OdDgDatabase *pDb, CWnd* pParent /*=NULL*/)
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

  OdDgDocumentSummaryInformationPtr pDocSI = oddgGetDocumentSummaryInformation(m_pDb);
  OdDgSummaryInformationPtr pSI = oddgGetSummaryInformation(m_pDb);
  OdString tmp;

  int number = 0;
  char out[20];
  
  SUMINFO_ITEM(_T("Title"),         pSI->getTitle())
  SUMINFO_ITEM(_T("Subject"),       pSI->getSubject())
  SUMINFO_ITEM(_T("Author"),        pSI->getAuthor())
  SUMINFO_ITEM(_T("Keywords"),      pSI->getKeywords())
  SUMINFO_ITEM(_T("Comments"),      pSI->getComments())
  SUMINFO_ITEM(_T("Last Saved By"),   pSI->getLastSavedBy())
  SUMINFO_ITEM(_T("Revision Number"),   pSI->getRevisionNumber())
  SUMINFO_ITEM(_T("Application Name"),   pSI->getApplicationName())

  SUMINFO_ITEM(_T("Database file name"), m_pDb->getFilename())

  SUMINFO_ITEM(_T( "Original version"), A2T(itoa(m_pDb->getOriginalFileVersion(), out, 10)))
  SUMINFO_ITEM(_T( "Major version"), A2T(itoa(m_pDb->getMajorVersion(), out, 10)))
  SUMINFO_ITEM(_T( "Minor version"), A2T(itoa(m_pDb->getMinorVersion(), out, 10)))

	return TRUE;
}
