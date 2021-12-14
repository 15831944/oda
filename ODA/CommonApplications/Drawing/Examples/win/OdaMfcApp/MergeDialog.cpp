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
#include "MergeDialog.h"
#include "odamfcapp.h"
#include "DbHistoryManager.h"
#include "Tf/TfRevisionControl.h"


// MergeDialog dialog

IMPLEMENT_DYNAMIC(MergeDialog, CDialog)

MergeDialog::MergeDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_MERGE, pParent), m_nSelected1(-1), m_nSelected2(-1)
{

}

MergeDialog::~MergeDialog()
{
}

void MergeDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}


BOOL MergeDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_pListBranches1 = (CListBox*)GetDlgItem(IDC_LIST_MERGE_BRANCHES_1);
  m_pListBranches2 = (CListBox*)GetDlgItem(IDC_LIST_MERGE_BRANCHES_2);
  m_pButtonAddBranch = (CButton*)GetDlgItem(ID_BUTTON_MERGE_OK);
  fillBranches();
  m_pButtonAddBranch->EnableWindow(FALSE);
  CPoint pt;
  GetCursorPos(&pt);
  SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  return TRUE;
}

BEGIN_MESSAGE_MAP(MergeDialog, CDialog)
  ON_BN_CLICKED(ID_BUTTON_MERGE_OK, &MergeDialog::OnBnClickedButtonMergeOk)
  ON_LBN_SELCHANGE(IDC_LIST_MERGE_BRANCHES_1, &MergeDialog::OnLbnSelchangeListMergeBranches1)
  ON_LBN_SELCHANGE(IDC_LIST_MERGE_BRANCHES_2, &MergeDialog::OnLbnSelchangeListMergeBranches2)
END_MESSAGE_MAP()


void MergeDialog::fillBranches()
{
  OdTfRevisionControl::ReferenceList branches;
  OdTfRevisionControl::getRepository(m_pDb)->getReferences(branches);
  std::map<OdString, OdTfDigest>::const_iterator it = branches.begin();
  std::map<OdString, OdTfDigest>::const_iterator itEnd = branches.end();
  size_t oldSize = m_branches.size();
  unsigned int i = 0;
  m_branches.clear();
  for (; it != itEnd; ++i, ++it)
  {
    m_pListBranches1->DeleteString(i);
    m_pListBranches1->InsertString(i, it->first.c_str());
    m_pListBranches2->DeleteString(i);
    m_pListBranches2->InsertString(i, it->first.c_str());
    m_branches.push_back(it->first);
  }
  while (i < oldSize)
  {
    m_pListBranches1->DeleteString(i++);
    m_pListBranches2->DeleteString(i++);
  }
}


void MergeDialog::OnBnClickedButtonMergeOk()
{
  if (m_nSelected1 == -1 || m_nSelected2 == -1)
    m_pButtonAddBranch->EnableWindow(FALSE);
  OdHandleArray hArr;
  OdString currentBranch = OdTfRevisionControl::getLocalBranch(m_pDb);
  if (currentBranch == m_branches[m_nSelected1])
  {
    OdTfRevisionControl::merge(m_pDb, m_branches[m_nSelected2], OdTfFixedMergeHandler<>());
  }
  else if (currentBranch == m_branches[m_nSelected2])
  {
    OdTfRevisionControl::merge(m_pDb, m_branches[m_nSelected1], OdTfFixedMergeHandler<>());
  }
  else
  {
    OdTfRevisionControl::switchBranch(m_pDb, m_branches[m_nSelected1]);
    OdTfRevisionControl::merge(m_pDb, m_branches[m_nSelected2], OdTfFixedMergeHandler<>());
  }
}


void MergeDialog::OnLbnSelchangeListMergeBranches1()
{
  m_nSelected1 = m_pListBranches1->GetCurSel();
  if(m_nSelected2 != -1)
    m_pButtonAddBranch->EnableWindow(true);
}


void MergeDialog::OnLbnSelchangeListMergeBranches2()
{
  m_nSelected2 = m_pListBranches2->GetCurSel();
  if(m_nSelected1 != -1)
    m_pButtonAddBranch->EnableWindow(true);
}
