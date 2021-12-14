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
// RemoteGeomPropsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OdaMfcApp.h"
#include "RemoteGeomPropsDlg.h"
#include "RemoteGeomViewer.h"


// CRemoteGeomPropsDlg dialog

IMPLEMENT_DYNAMIC(CRemoteGeomPropsDlg, CDialog)

CRemoteGeomPropsDlg::CRemoteGeomPropsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRemoteGeomPropsDlg::IDD, pParent)
{
  m_pWndToNotify = NULL;
}

CRemoteGeomPropsDlg::~CRemoteGeomPropsDlg()
{
}

void CRemoteGeomPropsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Control( pDX, IDC_ENABLE_OUTPUT_CHECK, m_bmEnableOutput );
  DDX_Control( pDX, IDC_BLOCKS_LIST, m_lstRemoteBlocks );  
  DDX_Control( pDX, IDC_HIDDEN_BLOCK_LIST, m_lstHiddenRemoteBlocks );  
  DDX_Control( pDX, IDC_PREVIEW_WINDOW, m_bmTogglePreviewWnd );  
}

BOOL CRemoteGeomPropsDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  SetIsEnableOutput(true);

  CRect rcWnd;
  GetWindowRect(&rcWnd);
  MoveWindow(0, 0, rcWnd.Width(), rcWnd.Height(), TRUE);

  m_preview.Create(IDD_RGV_PREVIEW, this);
  m_preview.CreateViewer();

  m_bmTogglePreviewWnd.SetCheck(BST_UNCHECKED);
  m_preview.ShowWindow(SW_HIDE);

  CWnd* pDesktopWnd = GetDesktopWindow();
  if(pDesktopWnd != NULL)
  {
    CRect rcDesktop;
    pDesktopWnd->GetWindowRect(&rcDesktop);

    CRect rcSize;
    GetWindowRect(&rcSize);

    MoveWindow(rcDesktop.right - rcSize.Width(), rcDesktop.top, rcSize.Width(), rcSize.Height());
  }
  
  return TRUE;
}

void CRemoteGeomPropsDlg::SetNotificationWindow(CWnd* pWndToNotify)
{
  m_pWndToNotify = pWndToNotify;
}

void CRemoteGeomPropsDlg::OnNewRemoteBlockAdded(OdUInt32 uNewBlockKey)
{
  AddBlockToList(m_lstRemoteBlocks, uNewBlockKey);
}

void CRemoteGeomPropsDlg::AddBlockToList(CListBox& lBox, OdUInt32 uBlockKey)
{
  OdString strNewName;
  strNewName.format(OD_T("Block %d"), uBlockKey);

  int iInsertPos = 0;
  const int iLbItemsCount = lBox.GetCount();
  while(iInsertPos < iLbItemsCount && lBox.GetItemData(iInsertPos) < uBlockKey)
    iInsertPos++;

  int iItem = lBox.InsertString(iInsertPos, strNewName);
  lBox.SetItemData(iItem, uBlockKey);
  lBox.SetCurSel(iItem);
}

void CRemoteGeomPropsDlg::AddBlockToHiddenListBox(OdUInt32 uBlockKey)
{
  AddBlockToList(m_lstHiddenRemoteBlocks, uBlockKey);
}

bool CRemoteGeomPropsDlg::IsEnableOutput()
{
  return (m_bmEnableOutput.GetCheck() == BST_CHECKED);
}

void CRemoteGeomPropsDlg::SetIsEnableOutput(bool fEnable)
{
  if(fEnable)
  {
    m_bmEnableOutput.SetCheck(BST_CHECKED);
    m_bmEnableOutput.SetWindowText(OD_T("Press to disable output"));
  }
  else
  {
    m_bmEnableOutput.SetCheck(BST_UNCHECKED);
    m_bmEnableOutput.SetWindowText(OD_T("Press to enable output"));
  }
}

void CRemoteGeomPropsDlg::SendParentMessage(UINT uMsg, WPARAM wp, LPARAM lp)
{
  if(m_pWndToNotify == NULL)
    return;

  m_pWndToNotify->SendMessage(uMsg, wp, lp);
}

int CRemoteGeomPropsDlg::CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct) 
{
  ASSERT(lpCompareItemStruct->CtlType == ODT_LISTBOX);

  OdUInt32 uData1 = (OdUInt32)lpCompareItemStruct->itemData1;
  OdUInt32 uData2 = (OdUInt32)lpCompareItemStruct->itemData2;
  
  if( uData1 < uData2 )
    return -1;

  if( uData1 > uData2 )
    return 1;

  return 0;
}


BEGIN_MESSAGE_MAP(CRemoteGeomPropsDlg, CDialog)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
  ON_BN_CLICKED(IDC_ENABLE_OUTPUT_CHECK, OnBnClickedEnableOutputCheck)
  ON_WM_SIZE( )
  //ON_WM_SETFOCUS( )
  //ON_WM_MOUSEACTIVATE( )
  ON_BN_CLICKED(IDC_DELETE_BLOCK, OnBnClickedDeleteBlock)
  ON_BN_CLICKED(IDC_UNDO_DELETE_BLOCK, OnBnClickedUndoDeleteBlock)
  ON_BN_CLICKED(IDC_HIDE, OnBnClickedHide)
  ON_LBN_SELCHANGE( IDC_BLOCKS_LIST, OnBlocksListSelChange ) 
  ON_LBN_SELCHANGE( IDC_HIDDEN_BLOCK_LIST, OnHiddenBlocksListSelChange ) 
  ON_BN_CLICKED(IDC_HIDE_ALL_BLOCKS, OnBnClickedHideAllBlocks)
  ON_BN_CLICKED(IDC_SHOW_ALL_BLOCKS, OnBnClickedShowAllBlocks)
  ON_BN_CLICKED(IDC_DELETE_ALL_BLOCKS, OnBnClickedDeleteAllBlocks)
  ON_BN_CLICKED(IDC_PREVIEW_WINDOW, OnBnClickedPreviewWindow)
END_MESSAGE_MAP()


// CRemoteGeomPropsDlg message handlers

void CRemoteGeomPropsDlg::OnBnClickedOk()
{
}

void CRemoteGeomPropsDlg::OnBnClickedEnableOutputCheck()
{
  SendParentMessage(WM_RGV_CHANGE_OUTPUT_STATE, IsEnableOutput());  
  SetIsEnableOutput(IsEnableOutput());
  SetFocus();
}

void CRemoteGeomPropsDlg::OnSize(UINT nType, int cx, int cy) 
{
  CDialog::OnSize(nType, cx, cy);
  // Resize list to fill the whole view.
  //m_List.MoveWindow (0, 0, cx, cy);
}

//// int CRemoteGeomPropsDlg::OnMouseActivate( CWnd*, UINT, UINT )
//{
//  return MA_NOACTIVATE;
//}

void CRemoteGeomPropsDlg::OnBnClickedDeleteBlock()
{
  int iCurSel = m_lstRemoteBlocks.GetCurSel();
  if(iCurSel < 0)
    return;

  OdUInt32 uBlockKey = m_lstRemoteBlocks.GetItemData(iCurSel);
  m_lstRemoteBlocks.DeleteString(iCurSel);

  SendParentMessage(WM_RGV_PERMANENT_ERASE_BLOCK, uBlockKey, 0);
  SelectNextListBoxItem(m_lstRemoteBlocks, iCurSel);  
}

void CRemoteGeomPropsDlg::OnBnClickedUndoDeleteBlock()
{
  int iCurSel = m_lstHiddenRemoteBlocks.GetCurSel();
  if(iCurSel < 0)
    return;

  OdUInt32 uBlockKey = m_lstHiddenRemoteBlocks.GetItemData(iCurSel);
  m_lstHiddenRemoteBlocks.DeleteString(iCurSel);

  SendParentMessage(WM_RGV_UNDO_DELETE_BLOCK, uBlockKey, 0);
  SelectNextListBoxItem(m_lstHiddenRemoteBlocks, iCurSel);  
}

void CRemoteGeomPropsDlg::SelectNextListBoxItem(CListBox& lBox, int iItem)
{
  if(lBox.GetCount() > 0)
  {
    while(iItem >= lBox.GetCount())
      iItem--;

    lBox.SetCurSel(iItem);
  }
}

void CRemoteGeomPropsDlg::OnBnClickedHide()
{
  int iCurSel = m_lstRemoteBlocks.GetCurSel();
  if(iCurSel < 0)
    return;

  OdUInt32 uBlockKey = m_lstRemoteBlocks.GetItemData(iCurSel);
  m_lstRemoteBlocks.DeleteString(iCurSel);

  SendParentMessage(WM_RGV_DELETE_BLOCK, uBlockKey, 0);
  SelectNextListBoxItem(m_lstRemoteBlocks, iCurSel);

  AddBlockToList(m_lstHiddenRemoteBlocks, uBlockKey);
}

void CRemoteGeomPropsDlg::OnBlocksListSelChange()
{
  CRemoteGeomViewer* pViewer = dynamic_cast<CRemoteGeomViewer*>(m_pWndToNotify);
  if(pViewer == NULL)
    return;

  int iCurSel = m_lstRemoteBlocks.GetCurSel();
  if(iCurSel < 0)
    return;

  OdUInt32 uBlockKey = m_lstRemoteBlocks.GetItemData(iCurSel);

  PreviewBlockData rData;
  if(!pViewer->GetPreviewBlockData(uBlockKey, rData))
    return;

  m_preview.SetPreview(rData.pDb, rData.idBlock);
}

void CRemoteGeomPropsDlg::OnHiddenBlocksListSelChange()
{

}
void CRemoteGeomPropsDlg::OnBnClickedHideAllBlocks()
{
  for(int listBlockNum = 0; listBlockNum < m_lstRemoteBlocks.GetCount(); listBlockNum++)
  {
    OdUInt32 uBlockKey = m_lstRemoteBlocks.GetItemData(listBlockNum);
    SendParentMessage(WM_RGV_DELETE_BLOCK, uBlockKey, 0);
    AddBlockToHiddenListBox( uBlockKey );
  }

  m_lstRemoteBlocks.ResetContent();  
}

void CRemoteGeomPropsDlg::OnBnClickedShowAllBlocks()
{
  for(int listBlockNum = 0; listBlockNum < m_lstHiddenRemoteBlocks.GetCount(); listBlockNum++)
  {
    OdUInt32 uBlockKey = m_lstHiddenRemoteBlocks.GetItemData(listBlockNum);
    SendParentMessage(WM_RGV_UNDO_DELETE_BLOCK, uBlockKey, 0);
  }

  m_lstHiddenRemoteBlocks.ResetContent();
}

void CRemoteGeomPropsDlg::OnBnClickedDeleteAllBlocks()
{
  for(int listBlockNum = 0; listBlockNum < m_lstRemoteBlocks.GetCount(); listBlockNum++)
  {
    OdUInt32 uBlockKey = m_lstRemoteBlocks.GetItemData(listBlockNum);
    SendParentMessage(WM_RGV_PERMANENT_ERASE_BLOCK, uBlockKey, 0);
  }

  m_lstRemoteBlocks.ResetContent();
}

void CRemoteGeomPropsDlg::AlignPreviewWindowToPropsDlg()
{
  WINDOWPLACEMENT wp;
  ZeroMemory(&wp, sizeof(wp));
  wp.length = sizeof(wp);
  
  GetWindowPlacement(&wp);

  CRect rcPreview;
  m_preview.GetWindowRect(&rcPreview);
  m_preview.MoveWindow(wp.rcNormalPosition.left, wp.rcNormalPosition.bottom, rcPreview.Width(), rcPreview.Height(), TRUE);
}

void CRemoteGeomPropsDlg::TogglePreviewWindow()
{
  bool fShowPreview = (m_bmTogglePreviewWnd.GetCheck() == BST_CHECKED);
  m_preview.ShowWindow(fShowPreview ? SW_SHOW : SW_HIDE);
}

void CRemoteGeomPropsDlg::OnBnClickedPreviewWindow()
{
  TogglePreviewWindow();
  AlignPreviewWindowToPropsDlg();
}
