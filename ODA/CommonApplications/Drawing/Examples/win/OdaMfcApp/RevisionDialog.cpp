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
#include "stdafx.h"
#include "OdaMfcApp.h"
#include "RevisionDialog.h"


// CRevisionDialog dialog

IMPLEMENT_DYNAMIC(CRevisionDialog, CDialog)

CRevisionDialog::CRevisionDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_REVISIONS, pParent)
{

}

CRevisionDialog::~CRevisionDialog()
{
}

void CRevisionDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST_LOG, m_LogList);
  DDX_Control(pDX, IDC_EDIT1, m_LogMessage);
}


BEGIN_MESSAGE_MAP(CRevisionDialog, CDialog)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LOG, &CRevisionDialog::OnLvnItemchangedListLog)
END_MESSAGE_MAP()


// CRevisionDialog message handlers

static void addCommitParents(OdDbDatabase* pDb, OdArray<OdTfCommitInfo>& commitList, OdTfCommitInfo& c, std::set<OdTfDigest>& added)
{
  for (unsigned i = 0; i < c.parents.size(); ++i)
  {
    if (added.find(c.parents[i]) != added.end())
      continue;
    added.insert(c.parents[i]);
    OdTfCommitInfo cp = OdTfRevisionControl::getRepository(pDb)->getCommitInfo(c.parents[i]);
    commitList.append(cp);
    addCommitParents(pDb, commitList, cp, added);
  }
}
CString formatDate(const OdTimeStamp& ts)
{
  CTime t(ts.year(), ts.month(), ts.day(), ts.hour(), ts.minute(), ts.second());
  SYSTEMTIME st;
  t.GetAsSystemTime(st);
  TCHAR datebuf[1024] = {0};
  ::GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, 0, datebuf, 1024);
  TCHAR timebuf[1024] = { 0 };
  ::GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, 0, timebuf, 1024);
  return CString(datebuf) + _T(" ") + CString(timebuf);
}
BOOL CRevisionDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  CRect rc;
  m_LogList.GetClientRect(&rc);
  m_LogList.InsertColumn(0, _T("Message"), LVCFMT_LEFT, rc.Width() * 2 / 3);
  m_LogList.InsertColumn(1, _T("Date"), LVCFMT_LEFT, rc.Width() / 6);
  m_LogList.InsertColumn(2, _T("Author"), LVCFMT_LEFT, rc.Width() / 6);
  OdTfDigest sha;
  OdString currentBranch = OdTfRevisionControl::getLocalBranch(m_pDb, &sha);// getCurrentBranch(m_pDb);
  if (!currentBranch.isEmpty())
  {
    std::set<OdTfDigest> added;
    OdTfCommitInfo topCommit;
    topCommit = OdTfRevisionControl::getRepository(m_pDb)->getCommitInfo(sha);
    //OdTfRevisionControl::getBranchTip(m_pDb, currentBranch, sha, &topCommit);
    commitList.append(topCommit);
    addCommitParents(m_pDb, commitList, topCommit, added);
#if _MSC_VER > 1600
    std::sort(commitList.begin(), commitList.end(), [](const OdTfCommitInfo& c1, const OdTfCommitInfo& c2) -> bool { return c1.annotation.date > c2.annotation.date; });
#endif
  }
  for (unsigned i = 0; i < commitList.size(); ++i)
  {
    CString message = (LPCTSTR)commitList[i].annotation.message;
    int n = m_LogList.InsertItem(i, message); // TODO: truncate the message with ellipsis
    m_LogList.SetItemText(n, 1, formatDate(commitList[i].annotation.date));
    m_LogList.SetItemText(n, 2, (LPCTSTR)commitList[i].annotation.author);
  }
  return TRUE;
}

void CRevisionDialog::OnLvnItemchangedListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  if (pNMLV->iItem >= 0 && pNMLV->iItem < (int)commitList.size())
    m_LogMessage.SetWindowText(commitList[pNMLV->iItem].annotation.message);
  *pResult = 0;
}
