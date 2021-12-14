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

// FileDependencyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "FileDependencyDlg.h"
#include "OdFileDepMgr.h"
#include "DwgView.h"
#include "OdaMfcAppDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDependencyDlg dialog


CFileDependencyDlg::CFileDependencyDlg(OdDbDatabase *pDb, CWnd* pParent /*=NULL*/)
	: CVarDialog(CFileDependencyDlg::IDD, pParent)
  , m_pDb(pDb)
{
	//{{AFX_DATA_INIT(CFileDependencyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFileDependencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileDependencyDlg)
	DDX_Control(pDX, IDC_FILEDEP_LIST, m_FileDepList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileDependencyDlg, CVarDialog)
	//{{AFX_MSG_MAP(CFileDependencyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileDependencyDlg message handlers

BOOL CFileDependencyDlg::OnInitDialog() 
{
	CVarDialog::OnInitDialog();
	
  m_FileDepList.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 30);
  m_FileDepList.InsertColumn(1, _T("Feature"), LVCFMT_LEFT, 100);
  m_FileDepList.InsertColumn(2, _T("File Name"), LVCFMT_LEFT, 100);
  m_FileDepList.InsertColumn(3, _T("Found Path"), LVCFMT_LEFT, 150);
  m_FileDepList.InsertColumn(4, _T("Full File Name"), LVCFMT_LEFT, 150);
  m_FileDepList.InsertColumn(5, _T("Fingerprint Guid"), LVCFMT_LEFT, 100);
  m_FileDepList.InsertColumn(6, _T("Version Guid"), LVCFMT_LEFT, 100);
  m_FileDepList.InsertColumn(7, _T("Time Stamp"), LVCFMT_LEFT, 100);
  m_FileDepList.InsertColumn(8, _T("File Size"), LVCFMT_LEFT, 100);
  m_FileDepList.InsertColumn(9, _T("Affects Graphics"), LVCFMT_LEFT, 50);
  m_FileDepList.InsertColumn(10, _T("Reference Count"), LVCFMT_LEFT, 50);
  m_FileDepList.DeleteAllItems();  

  ODA_ASSERT(m_pDb);
  OdFileDependencyManagerPtr pFileDepMgr = m_pDb->fileDependencyManager();

  if (!pFileDepMgr.isNull())
  {
    pFileDepMgr->iteratorInitialize(OdString::kEmpty, false, false, true);
    OdString str;
    unsigned infoId,
             number = 0;
    while ((infoId = pFileDepMgr->iteratorNext()) != 0)
    {
      OdFileDependencyInfoPtr pDepInfo;
      if ( pFileDepMgr->getEntry(infoId, pDepInfo ) != eOk ) continue;

      m_FileDepList.InsertItem(number, _T(""));
      str.format(OD_T("%d"), pDepInfo->m_nIndex);
      m_FileDepList.SetItemText(number, 0, str);
      m_FileDepList.SetItemText(number, 1, pDepInfo->m_Feature);
      m_FileDepList.SetItemText(number, 2, pDepInfo->m_FileName);
      m_FileDepList.SetItemText(number, 3, pDepInfo->m_FoundPath);
      m_FileDepList.SetItemText(number, 4, pDepInfo->m_FullFileName);
      m_FileDepList.SetItemText(number, 5, pDepInfo->m_FingerprintGuid);
      m_FileDepList.SetItemText(number, 6, pDepInfo->m_VersionGuid);
      str.format(OD_T("%d"), pDepInfo->m_nTimeStamp);
      m_FileDepList.SetItemText(number, 7, str);
      str.format(OD_T("%d"), pDepInfo->m_nFileSize);
      m_FileDepList.SetItemText(number, 8, str);
      str.format(OD_T("%d"), pDepInfo->m_bAffectsGraphics ? 1 : 0);
      m_FileDepList.SetItemText(number, 9, str);
      str.format(OD_T("%d"), pDepInfo->m_nReferenceCount);
      m_FileDepList.SetItemText(number, 10, str);

      number++;
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  //////////// Initialize the resize help data member ///////////////////////
  ///////////////////////////////////////////////////////////////////////////
  initResizeHelper();
  m_resizeHelper.Fix(IDC_FILEDEP_LIST,  DlgResizeHelper::kLeftRight, DlgResizeHelper::kNoVFix);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
