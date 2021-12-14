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
// DifferenceDialog.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "BranchesDialog.h"
#include "DbHistoryManager.h"
#include "Tf/TfRevisionControl.h"

// DifferenceDialog dialog

IMPLEMENT_DYNAMIC(BranchesDialog, CDialog)


BranchesDialog::BranchesDialog(CWnd* pParent /*=NULL*/)
  : CDialog(BranchesDialog::IDD, pParent)
  , m_nSelected(0)
{

}

BranchesDialog::~BranchesDialog()
{
}

void BranchesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BranchesDialog, CDialog)
  ON_LBN_SELCHANGE(IDC_LIST_BRANCHES, &BranchesDialog::OnLbnSelchangeDiffObject)
ON_BN_CLICKED(IDC_BUTTON_ADD_BRANCH, &BranchesDialog::OnBnClickedButtonAddBranch)
ON_BN_CLICKED(IDC_BUTTON_SWITCH_BRANCH, &BranchesDialog::OnBnClickedButtonSwitchBranch)
END_MESSAGE_MAP()

BOOL BranchesDialog::OnInitDialog()
{
  CDialog::OnInitDialog();
  m_pEditNewBranch = (CEdit*)GetDlgItem(IDC_EDIT_ADD_BRANCH);
  m_pNotEditCurrentBranch = (CEdit*)GetDlgItem(IDC_EDIT_CURRENT_BRANCH);
  m_pListBranches = (CListBox*)GetDlgItem(IDC_LIST_BRANCHES);
  m_pButtonAddBranch = (CButton*)GetDlgItem(IDC_BUTTON_ADD_BRANCH);
  fillBranches();
  m_pNotEditCurrentBranch->SetWindowTextW(L"Current branch: " + OdTfRevisionControl::getLocalBranch(m_pDb));
  CPoint pt;
  GetCursorPos(&pt);
  SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
  return TRUE;
}

void BranchesDialog::fillBranches()
{
  OdTfRevisionControl::ReferenceList branches;
  OdTfRevisionControl::getRepository(m_pDb)->getReferences(branches);
  OdTfRevisionControl::ReferenceList::const_iterator it = branches.begin();
  OdTfRevisionControl::ReferenceList::const_iterator itEnd = branches.end();
  size_t oldSize = m_branches.size();
  unsigned int i = 0;
  m_branches.clear();
  for (; it != itEnd; ++i, ++it)
  {
    m_pListBranches->DeleteString(i);
    m_pListBranches->InsertString(i, it->first.c_str());
    m_branches.push_back(it->first);
  }
  while (i < oldSize)
  {
    m_pListBranches->DeleteString(i++);
  }
}


void BranchesDialog::OnLbnSelchangeDiffObject()
{
  m_nSelected = m_pListBranches->GetCurSel();
}


void BranchesDialog::OnBnClickedButtonAddBranch()
{
  CString branch;
  m_pEditNewBranch->GetWindowTextW(branch);
  OdString odBranch = branch;
  OdTfDigest dig;
  OdTfRevisionControl::getLocalBranch(m_pDb, &dig);
  OdTfRevisionControl::getRepository(m_pDb)->setReference(odBranch, dig);
  OdTfRevisionControl::switchBranch(m_pDb, odBranch);
  m_pNotEditCurrentBranch->SetWindowTextW(L"Current branch: " + OdTfRevisionControl::getLocalBranch(m_pDb));
  fillBranches();
}


void BranchesDialog::OnBnClickedButtonSwitchBranch()
{
  OdString branch = m_branches[m_nSelected];
  OdTfRevisionControl::switchBranch(m_pDb, branch);
  m_pNotEditCurrentBranch->SetWindowTextW(L"Current branch: " + OdTfRevisionControl::getLocalBranch(m_pDb));
}
