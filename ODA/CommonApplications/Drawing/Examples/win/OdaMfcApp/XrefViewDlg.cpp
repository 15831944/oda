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

// XrefViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "XrefViewDlg.h"
#include "DbBlockTableRecord.h"
#include "FileDlgExt.h"
#include "PreviewDib.h"
#include "XRefMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXrefViewDlg dialog


CXrefViewDlg::CXrefViewDlg(OdDbDatabase* pDb, CWnd* pParent /*=NULL*/)
	: CDialog(CXrefViewDlg::IDD, pParent), m_pDb(pDb), m_pNode(0)
{
	//{{AFX_DATA_INIT(CXrefViewDlg)
	//}}AFX_DATA_INIT
}


void CXrefViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXrefViewDlg)
	DDX_Control(pDX, IDC_BROWSE, m_btnBrowse);
	DDX_Control(pDX, IDC_SAVEPATH, m_btnSavePath);
	DDX_Control(pDX, IDC_FOUND_PATH, m_foundAt);
	DDX_Control(pDX, IDC_XREFLIST, m_list);
	DDX_Control(pDX, IDC_RELOAD, m_btnReload);
	DDX_Control(pDX, IDC_UNLOAD, m_btnUnload);
	DDX_Control(pDX, IDC_XREFTREE, m_tree);
	//}}AFX_DATA_MAP
  if(pDX->m_bSaveAndValidate)
  {
    m_xrefsToReload.clear();
    m_xrefsToUnload.clear();
    m_xrefsToDetach.clear();
    int n = m_list.GetItemCount();
    for(int i=0; i<n; ++i)
    {
      CString s = m_list.GetItemText(i, 1);
      if(s == "Reload")
      {
        OdDbXrefGraphNode* pNode = (OdDbXrefGraphNode*)m_list.GetItemData(i);
        m_xrefsToReload.append(pNode->blockId());
      }
      else if(s == "Unload")
      {
        OdDbXrefGraphNode* pNode = (OdDbXrefGraphNode*)m_list.GetItemData(i);
        m_xrefsToUnload.append(pNode->blockId());
      }
      else if(s == "Detach")
      {
        OdDbXrefGraphNode* pNode = (OdDbXrefGraphNode*)m_list.GetItemData(i);
        m_xrefsToDetach.append(pNode->blockId());
      }
    }
  }
  else
  {
  }
}


BEGIN_MESSAGE_MAP(CXrefViewDlg, CDialog)
	//{{AFX_MSG_MAP(CXrefViewDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_XREFTREE, OnSelchangedXreftree)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_NOTIFY(NM_CLICK, IDC_XREFLIST, OnClickXreflist)
	ON_BN_CLICKED(IDC_RELOAD, OnReload)
	ON_BN_CLICKED(IDC_UNLOAD, OnUnload)
	//}}AFX_MSG_MAP

  ON_COMMAND(ID_XRM_LIST, OnViewList)
	ON_COMMAND(ID_XRM_TREE, OnViewTree)
  ON_BN_CLICKED(IDC_DETACH, OnBnClickedDetach)
  ON_BN_CLICKED(IDC_ATTACH, &CXrefViewDlg::OnBnClickedAttach)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXrefViewDlg message handlers

OdString getFilename(OdDbXrefGraphNode* pNode)
{
  OdString sRes;
  OdString sFileName;
  if(pNode->database())
  {
    sFileName = pNode->database()->getFilename();
  }
  else
  {
    sFileName = OdDbBlockTableRecordPtr(pNode->blockId().safeOpenObject())->pathName();
  }
  return sFileName;
}
static bool findItemData(const CTreeCtrl* tree, HTREEITEM hParent, DWORD_PTR data)
{
  if (!hParent || hParent == TVI_ROOT)
    return false;
  if (tree->GetItemData(hParent) == data)
    return true;
  return findItemData(tree, tree->GetParentItem(hParent), data);
}
void CXrefViewDlg::fillTree(OdDbXrefGraphNode* pNode, HTREEITEM hParent)
{
  OdString sFileName;
  if(pNode->owner()->rootNode()!=pNode)
  {
    sFileName = pNode->name();
  }
  else
  {
    sFileName = _T("Current");
  }
  bool cycle = findItemData(&m_tree, hParent, (DWORD_PTR)pNode);
  HTREEITEM hItem = m_tree.InsertItem(sFileName, hParent);
  m_tree.SetItemState(hItem, TVIS_EXPANDED, TVIS_EXPANDED);
  m_tree.SetItemData(hItem, (DWORD_PTR)pNode);
  if (!cycle)
  {
    for (int i = 0; i < pNode->numOut(); ++i)
    {
      fillTree((OdDbXrefGraphNode*)pNode->out(i), hItem);
    }
  }
}

inline OdString statusString(OdDb::XrefStatus status)
{
  switch(status)
  {
  case OdDb::kXrfResolved:
    return _T("Loaded");
  case OdDb::kXrfUnloaded:
    return _T("Unloaded");
  case OdDb::kXrfUnreferenced:
    return _T("Unreferenced");
  case OdDb::kXrfFileNotFound:
    return _T("Not Found");
  case OdDb::kXrfUnresolved:
    return _T("Unresolved");
  };
  return _T("Unknown");
}

void SetItemText(CListCtrl& list, int nItem, int nSubItem, LPCTSTR lpszText)
{
  // list.SetItemText(nItem, 1, lpszText); does not work
  LVITEM lvi = {
    0, nItem, nSubItem, 0, 0, (LPTSTR)lpszText, 0, 0, 0
#if (_WIN32_IE >= 0x0300)
    , 0
#endif
  };
  lvi.iSubItem = nSubItem;
  lvi.pszText = (LPTSTR) lpszText;
  SendMessage(list.m_hWnd, LVM_SETITEMTEXT, nItem, (LPARAM)&lvi);
}

static void initList(CListCtrl& l)
{
  int n = 0;
  l.InsertColumn(n++, _T("Reference Name"),  LVCFMT_LEFT, 100);
  l.InsertColumn(n++, _T("Status"),          LVCFMT_LEFT,  50);
  l.InsertColumn(n++, _T("Size"),            LVCFMT_LEFT,  50);
  l.InsertColumn(n++, _T("Type"),            LVCFMT_LEFT,  50);
  l.InsertColumn(n++, _T("Date"),            LVCFMT_LEFT,  50);
  l.InsertColumn(n++, _T("Saved Path"),      LVCFMT_LEFT, 119);
}

void CXrefViewDlg::fillList()
{
  int n = m_graph.numNodes();
  for(int i=0; i<n; ++i)
  {
    OdDbXrefGraphNode* pNode = m_graph.xrefNode(i);
    if(pNode!=m_graph.rootNode())
    {
      int nItem = m_list.InsertItem(i, pNode->name());
      SetItemText(m_list, nItem, 1, ::statusString(pNode->xrefStatus()));
      m_list.SetItemData(nItem, (DWORD_PTR)pNode);
      
      OdDbBlockTableRecordPtr pXrefBlock = pNode->blockId().safeOpenObject();

      SetItemText(m_list, nItem, 3, pXrefBlock->isFromOverlayReference() ? _T("Overlay") : _T("Attach"));
      SetItemText(m_list, nItem, 5, pXrefBlock->pathName());
    }
  }
}
BOOL CXrefViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
  OdDbXrefGraph::getFrom(m_pDb, m_graph);
  fillTree(m_graph.xrefNode(0), TVI_ROOT);
  initList(m_list);
  fillList();
  m_tree.ShowWindow(SW_HIDE);
	return TRUE;
}

void CXrefViewDlg::OnSelchangedXreftree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

  OdDbXrefGraphNode* pNewNode = 0;
  if(pNMTreeView->itemNew.hItem)
    pNewNode = (OdDbXrefGraphNode*)m_tree.GetItemData(pNMTreeView->itemNew.hItem);

  OnSelChanged(pNewNode);
	*pResult = 0;
}

bool CXrefViewDlg::SelectInTree(DWORD dwKey, HTREEITEM hRoot)
{
  if(!hRoot)
  {
    hRoot = m_tree.GetRootItem();
  }
  HTREEITEM hItem = m_tree.GetChildItem(hRoot);
  while(hItem != NULL)
  {
    for(; hItem != NULL; hItem = m_tree.GetNextItem(hItem, TVGN_NEXT))
    {
      if(m_tree.GetItemData(hItem) == dwKey)
      {
        m_tree.SelectItem(hItem);
        return true;
      }
      TVITEM tvitem;
      tvitem.mask = TVIF_HANDLE|TVIF_CHILDREN;
      tvitem.hItem = hItem;
      tvitem.cChildren = 0;
      if(m_tree.GetItem(&tvitem) && tvitem.cChildren)
      {
        if(SelectInTree(dwKey, hItem))
          return true;
      }
    }
  }
  return false;
}

void CXrefViewDlg::SelectInList(LONG_PTR dwKey)
{
  LVFINDINFO fi = { LVFI_PARAM, 0, dwKey, {0,0}, 0 };
  m_list.SetSelectionMark(m_list.FindItem(&fi));
  m_list.UpdateWindow();
}

void CXrefViewDlg::OnClickXreflist(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
  if(m_list.GetSelectedCount())
  {
    int n = m_list.GetSelectionMark();
    DWORD_PTR data = m_list.GetItemData(n);
    OnSelChanged((OdDbXrefGraphNode*)data);
    SelectInTree(data);
  }
  else
  {
    OnSelChanged(0);
  }
}

void CXrefViewDlg::OnSelChanged(OdDbXrefGraphNode* pNewNode) 
{
  if(m_pNode == pNewNode)
    return;

  if(pNewNode && !pNewNode->blockId().isNull())
  {
    m_btnUnload.EnableWindow(pNewNode->xrefStatus()!=OdDb::kXrfUnloaded);
    m_btnReload.EnableWindow(TRUE);
  
    OdDbBlockTableRecordPtr pXrefBlock = pNewNode->blockId().safeOpenObject();

    m_foundAt.EnableWindow(TRUE);
    m_foundAt.SetWindowText(m_pDb->appServices()->findFile(pXrefBlock->pathName(), m_pDb, OdDbHostAppServices::kXRefDrawing));
    m_btnBrowse.EnableWindow(TRUE);
    m_btnSavePath.EnableWindow(TRUE);
  }
  else
  {
    m_btnUnload.EnableWindow(FALSE);
    m_btnReload.EnableWindow(FALSE);

    m_foundAt.EnableWindow(FALSE);
    m_foundAt.SetWindowText(_T(""));
    m_btnBrowse.EnableWindow(FALSE);
    m_btnSavePath.EnableWindow(FALSE);
  }
  m_pNode = pNewNode;
}

int CXrefViewDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(WS_CHILD|WS_VISIBLE|CCS_NOPARENTALIGN|CCS_NODIVIDER, CRect(11,10,55,18), this, IDR_XREFMAN_BAR))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
  m_wndToolBar.AddBitmap(2, IDB_XREFBAR);
  TBBUTTON btn1 = { 0, ID_XRM_LIST, TBSTATE_CHECKED|TBSTATE_ENABLED, 0, 0, NULL };
  m_wndToolBar.InsertButton(0, &btn1);
  TBBUTTON btn2 = { 1, ID_XRM_TREE, TBSTATE_ENABLED, 0, 0, NULL };
  m_wndToolBar.InsertButton(1, &btn2);

	return 0;
}

void CXrefViewDlg::OnViewList()
{
  m_wndToolBar.SetState(ID_XRM_LIST, TBSTATE_CHECKED|TBSTATE_ENABLED);
  m_wndToolBar.SetState(ID_XRM_TREE, TBSTATE_ENABLED);

  m_list.ShowWindow(SW_SHOW);
  m_list.EnableWindow(TRUE);

  m_tree.ShowWindow(SW_HIDE);
  m_tree.EnableWindow(FALSE);

  SelectInList((LONG_PTR)m_pNode);
}

void CXrefViewDlg::OnViewTree()
{
  m_wndToolBar.SetState(ID_XRM_LIST, TBSTATE_ENABLED);
  m_wndToolBar.SetState(ID_XRM_TREE, TBSTATE_CHECKED|TBSTATE_ENABLED);

  m_list.ShowWindow(SW_HIDE);
  m_list.EnableWindow(FALSE);

  m_tree.ShowWindow(SW_SHOW);
  m_tree.EnableWindow(TRUE);
}

void CXrefViewDlg::OnBrowse() 
{
  CString sPath = COdaMfcAppApp::BrowseWithPreview(AfxGetMainWnd()->GetSafeHwnd(), _T("DWG files (*.dwg)|*.dwg||"));
  if(sPath.GetLength())
  {
    m_foundAt.SetWindowText(sPath);
  }
}

void CXrefViewDlg::OnReload() 
{
  int nSel = m_list.GetSelectionMark();
  m_list.SetItemText(nSel, 1, _T("Reload"));
}

void CXrefViewDlg::OnUnload() 
{
  int nSel = m_list.GetSelectionMark();
  m_list.SetItemText(nSel, 1, _T("Unload"));
}

void CXrefViewDlg::OnBnClickedDetach()
{
  int nSel = m_list.GetSelectionMark();
  m_list.SetItemText(nSel, 1, _T("Detach"));
  
}

void CXrefViewDlg::OnBnClickedAttach()
{
  OdString sPath = COdaMfcAppApp::BrowseWithPreview(AfxGetMainWnd()->GetSafeHwnd(), _T("DWG files (*.dwg)|*.dwg||"));
  if (!sPath.isEmpty())
  {
    OdString name = sPath.mid(sPath.reverseFind('\\') + 1);
    name = name.left(name.getLength()-4);
    m_pDb->startUndoRecord();
    OdDbBlockTableRecordPtr pRec = OdDbXRefManExt::addNewXRefDefBlock(m_pDb, sPath, name, false);
    OdDbXRefMan::load(pRec);
    m_pNode = 0;
    OdDbXrefGraph::getFrom(m_pDb, m_graph);
    m_tree.DeleteAllItems();
    fillTree(m_graph.xrefNode(0), TVI_ROOT);
    m_list.DeleteAllItems();
    fillList();
  }
}
