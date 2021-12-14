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
// COdaDgnTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "OdaDgnTreeView.h"
#include "OdaDgnAppDoc.h"
#include "DgnTreeFiller.h"
#include "MainFrm.h"
#include "FindObjectDlg.h"
#include "LinkArrtDlg.h"
#include "FieldViewDlg.h"
#include "ItemTypeAttachDlg.h"

// COdaDgnTreeView

IMPLEMENT_DYNCREATE(COdaDgnTreeView, CFormView)

COdaDgnTreeView::COdaDgnTreeView()
	: CFormView(COdaDgnTreeView::IDD)
	, m_pTreeFiller(0)
	, m_pListFiller(0)
{
}

COdaDgnTreeView::~COdaDgnTreeView()
{
	if (m_pTreeFiller)
	{
		delete m_pTreeFiller;
		m_pTreeFiller = 0;
	}
	if (m_pListFiller)
	{
		delete m_pListFiller;
		m_pListFiller = 0;
	}
}

DgnTreeFiller* COdaDgnTreeView::getTreeFiller()
{
	if (!m_pTreeFiller)
	{
		OdDgDatabase* pDb = GetDocument()->database();
		m_pTreeFiller = new DgnTreeFiller(pDb, &m_ObjectTree);
	}
	return m_pTreeFiller;
}

DgnListFiller* COdaDgnTreeView::getListFiller()
{
	if (!m_pListFiller)
	{
		OdDgDatabase* pDb = GetDocument()->database();
		m_pListFiller = new DgnListFiller(pDb, &m_ObjectList);
	}
	return m_pListFiller;
}

BOOL COdaDgnTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_MAXIMIZE;
	return CFormView::PreCreateWindow(cs);
}

void COdaDgnTreeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DGNTREEVIEW, m_ObjectTree);
	DDX_Control(pDX, IDC_ELEMENTDUMPLIST, m_ObjectList);
}

void COdaDgnTreeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
	SetControlFont();

	m_ObjectList.SetExtendedStyle(m_ObjectList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	ResizeParentToFit();

	//m_ObjectList.InsertColumn(0, _T("Groupcode"), LVCFMT_LEFT, 80);
	//m_ObjectList.InsertColumn(1, _T("Type"), LVCFMT_LEFT, 120);
	//m_ObjectList.InsertColumn(2, _T("Value"), LVCFMT_LEFT, 231);
	//m_nTotalObjects = 0;
	m_ObjectList.InsertColumn(0, _T("Field"), LVCFMT_LEFT, 240);
	m_ObjectList.InsertColumn(1, _T("Value"), LVCFMT_LEFT, 240);
	m_nTotalObjects = 0;
}

void COdaDgnTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	fillTree();
}

void COdaDgnTreeView::SetControlFont()
{
	LOGFONT lf = { 0 };
	m_ObjectTree.GetFont()->GetLogFont(&lf);
	CString strFont = AfxGetApp()->GetProfileString(_T("options"), _T("List window font typeface"), _T("Microsoft Sans Serif"));
	_tcscpy(lf.lfFaceName, (LPCTSTR)strFont);
	m_font.CreateFontIndirect(&lf);

	// apply the font to the controls
	m_ObjectTree.SetFont(&m_font);
	m_ObjectList.SetFont(&m_font);
}

BEGIN_MESSAGE_MAP(COdaDgnTreeView, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DGNTREEVIEW, OnTvnSelchangedTree)
	ON_NOTIFY(TVN_KEYDOWN, IDC_DGNTREEVIEW, OnKeydownOwnershiptree)
	ON_NOTIFY(NM_RCLICK, IDC_DGNTREEVIEW, OnRightKeydownOwnershiptree)
	ON_NOTIFY(NM_DBLCLK, IDC_ELEMENTDUMPLIST, OnDoubleClickObjectList)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_FIND, OnEditFind)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_DGNTREEVIEW, OnTvnItemexpandingDgntreeview)
	ON_COMMAND(ID_REFRESH, OnRefresh)
	ON_COMMAND(ID_EDIT_CLEARSELECTION, OnEditClearSelection)
	ON_COMMAND(IDC_BUTTON_LINKS_ATTR_DLG, &COdaDgnTreeView::OnButtonLinksAttrDlg)
  ON_COMMAND(ID_ITEMTYPES, &COdaDgnTreeView::OnItemTypes)
  ON_UPDATE_COMMAND_UI(ID_ITEMTYPES, COdaDgnTreeView::OnUpdateItemTypes)
END_MESSAGE_MAP()

//BEGIN_MESSAGE_MAP(COdaDgnTreeView, CTreeCtrl)
//	//<AngularNoBind>{{</AngularNoBind>AFX_MSG_MAP(CMyTreeCtrl)
//	ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
//	ON_WM_CONTEXTMENU()
//	//<AngularNoBind>}}</AngularNoBind>AFX_MSG_MAP
//END_MESSAGE_MAP()

// COdaDgnTreeView diagnostics

#ifdef _DEBUG
void COdaDgnTreeView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void COdaDgnTreeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

COdaDgnAppDoc* COdaDgnTreeView::GetDocument() const // non-debug version is inline
{
	ODA_ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COdaDgnAppDoc)));
	return (COdaDgnAppDoc*)m_pDocument;
}
#endif //_DEBUG


// COdaDgnTreeView message handlers

void COdaDgnTreeView::OnDestroy()
{
	// Close vectorizer views
	COdaDgnAppDoc* pDoc = GetDocument();
	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = pDoc->GetNextView(pos);
		if (pView && pView != this)
		{
			pView->GetParent()->PostMessage(WM_CLOSE);
		}
	}

	CFormView::OnDestroy();
}

BOOL COdaDgnTreeView::Find(HTREEITEM item, OdDgElementId id)
{
	for (; item != NULL; item = m_ObjectTree.GetNextItem(item, TVGN_NEXT))
	{
		LPARAM data = m_ObjectTree.GetItemData(item);

		OdDbHandle h1 = id.getHandle();
		OdDbHandle h2 = OdDgElementId((OdDbStub*)data).getHandle();
		if (h1 == h2)
		{
			m_ObjectTree.SelectItem(item);
			return TRUE;
		}

		if (m_ObjectTree.ItemHasChildren(item))
		{
			m_ObjectTree.Expand(item, TVE_EXPAND);
			// MKU 05\28\20010 fixed #7005 (OdaDgnApp: Find By Handle doesn't work as some expected)
			//  Note: GetChildItem() returns a child item only for expanded Tree. What's why the line above was added
			HTREEITEM subItem = m_ObjectTree.GetChildItem(item);
			if (subItem && Find(subItem, id))
			{
				m_ObjectTree.Expand(item, TVE_EXPAND);
				return TRUE;
			}
			m_ObjectTree.Expand(item, TVE_COLLAPSE);
		}
	}

	return FALSE;
}

void COdaDgnTreeView::Find(const OdDgElementId& targetId)
{
	// Building parentship queue
	OdArray<OdDgElementId> parents;

	// Opening...
	HTREEITEM item = m_ObjectTree.GetChildItem(m_ObjectTree.GetRootItem());
	Find(item, targetId);
}

void COdaDgnTreeView::OnEditFind()
{
	CFindObjectDlg fd;
	if (fd.DoModal() == IDOK)
	{
		// Calculating handle
		OdDbHandle han((LPCTSTR)fd.m_objHandle);

		// Requesting ObjectId
		OdDgElementId targetId;
		OdDgDatabase* pDb = GetDocument()->database();
		targetId = pDb->getElementId(han);
		if (targetId.isNull())
		{
			MessageBox(_T("Incorrect handle"), _T("Error"), MB_OK + MB_ICONSTOP);
			return;
		}
		COdaDgnTreeView::Find(targetId);
	}
}

void COdaDgnTreeView::fillTree()
{
	m_ObjectTree.DeleteAllItems();

	OdDgDatabase* pDb = GetDocument()->database();
	DgnTreeFiller* pTreeFiller = getTreeFiller();

	// Add root database node
	CString sName;
	sName.Format(_T("<%s>"), (LPCTSTR)(pDb->isA()->name()));
	pTreeFiller->setDbTreeItem(pTreeFiller->getTree()->InsertItem(sName, TVI_ROOT, TVI_LAST));
	TVITEM item = { 0 };
	item.mask = TVIF_HANDLE | TVIF_CHILDREN | TVIF_PARAM;
	item.hItem = pTreeFiller->getDbTreeItem();
	item.cChildren = 1;
	item.lParam = 0;

	try
	{

		HTREEITEM hDbItem = pTreeFiller->getDbTreeItem();

		// Tables
		HTREEITEM hItem;
		hItem = pTreeFiller->addElement(pDb->getLevelTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getLevelFilterTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getFontTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getTextStyleTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getDimStyleTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getMaterialTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getMultilineStyleTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getLineStyleTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getLineStyleDefTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getRegAppTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getViewGroupTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getNamedViewTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getModelTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getSharedCellDefinitionTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getTagDefinitionSetTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getColorTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getElementTemplateTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getEnvironmentMapTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getDisplayStyleTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getLinkSetTable().get(), hDbItem);
    hItem = pTreeFiller->addElement(pDb->getPrintStyleTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getColorBookTable().get(), hDbItem);
    hItem = pTreeFiller->addElement(pDb->getPrototypeElementTable().get(), hDbItem);
    hItem = pTreeFiller->addElement(pDb->getDictionaryTable().get(), hDbItem);
		hItem = pTreeFiller->addElement(pDb->getNonModelElementCollection().get(), hDbItem);

		m_ObjectTree.Expand(hDbItem, TVE_EXPAND);
	}
	catch (OdError& e)
	{
		theApp.reportError(_T("Error reading DB..."), e);
	}
	//if (oldObjId.isValid())
	//{
	//  Find(oldObjId);
	//}
}

OdDgElementId COdaDgnTreeView::curElementId()
{
	return OdDgElementId::kNull;
}

void COdaDgnTreeView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (cx > 10 && cy > 10 && m_ObjectTree.m_hWnd)
	{
		CRect crc;
		GetClientRect(&crc);
		crc.DeflateRect(2, 2, 2, 2);
		m_ObjectTree.SetWindowPos(NULL, crc.left, crc.top, crc.Width() / 2 - 1, crc.Height(), SWP_NOZORDER);
		m_ObjectList.SetWindowPos(NULL, crc.left + crc.Width() / 2 + 1, crc.top, crc.Width() / 2 - 1, crc.Height(), SWP_NOZORDER);
	}
}

void COdaDgnTreeView::OnRefresh()
{
	m_ObjectTree.SelectItem(0);
	m_ObjectList.DeleteAllItems();
	fillTree();
}

void COdaDgnTreeView::OnTvnItemexpandingDgntreeview(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	// handle of selected item as tree member
	HTREEITEM hChild = m_ObjectTree.GetChildItem(pNMTreeView->itemNew.hItem);
	if (!hChild)
	{
		CMainFrame* pWnd = (CMainFrame*)theApp.GetMainWnd();
		pWnd->setStatusText(0, _T("Filling tree..."));

		HCURSOR cursor = GetCursor();
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		try
		{
			m_pTreeFiller->explandItem(pNMTreeView->itemNew.hItem);
		}
		catch (const OdError& e)
		{
			theApp.reportError(_T("Filling tree..."), e);
		}
		catch (...)
		{
			theApp.reportError(_T("Filling tree..."), OdError(eInvalidInput));
		}
		SetCursor(cursor);
		pWnd->setStatusText(0, _T("Ready"));
	}

	*pResult = 0;
}

OdDgElementId COdaDgnTreeView::GetSelectedElementId()
{
	if (!m_idCurrentElement.isNull())
		return m_idCurrentElement;

	return OdDgElementId::kNull;
}

void COdaDgnTreeView::OnKeydownOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult)
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

#ifndef ODA_FILE_FIXER
	switch (pTVKeyDown->wVKey)
	{
	case VK_DELETE:
	{
		GetDocument()->DeleteSelection();
	} break;
	}
#endif
	*pResult = 0;
}

void COdaDgnTreeView::SetCurrentSelectedItem()
{
	DWORD pos = GetMessagePos();
	CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
	m_ObjectTree.ScreenToClient(&pt);
	if (HTREEITEM hItem = m_ObjectTree.HitTest(pt))
		m_ObjectTree.SelectItem(hItem);
	HTREEITEM hselectedItem = m_ObjectTree.GetSelectedItem();
	TVITEM tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = hselectedItem;
	m_ObjectTree.GetItem(&tvItem);
	OdRxObjectPtr pRxObject = getTreeFiller()->getObject(tvItem);
	OdDgElementPtr pElement = OdDgElement::cast(pRxObject);
	if (!pElement.isNull())
	{
		m_idCurrentElement = pElement->elementId();
	}
}

void COdaDgnTreeView::SetCurrentSelectedListItem()
{
	DWORD pos = GetMessagePos();
	CPoint pt(GET_X_LPARAM(pos), GET_Y_LPARAM(pos));
	m_ObjectList.ScreenToClient(&pt);

	int hItem = m_ObjectList.HitTest(pt);
	LVITEM lvItem;
	lvItem.mask = LVIF_PARAM;
	lvItem.iItem = hItem;
	if (m_ObjectList.GetItem(&lvItem)) {
		m_CurrentListItem = lvItem.iItem;
	}
}

void COdaDgnTreeView::OnRightKeydownOwnershiptree(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetCurrentSelectedItem();

	NMTOOLBAR* pnmHDR = reinterpret_cast<NMTOOLBAR*>(pNMHDR);

	// load and display popup menu
	CMenu menu;
	menu.LoadMenu(IDR_TREEVIEW_MENU);
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup);

	CPoint point(GetMessagePos());

	int popupMenuXPos = point.x;
	int popupMenuYPos = point.y;

	CRect rc;

	pPopup->TrackPopupMenu(
		TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
		popupMenuXPos, popupMenuYPos, this, &rc);
}

void COdaDgnTreeView::OnDoubleClickObjectList(NMHDR* pNMHDR, LRESULT* pResult)
{
	m_CurrentListItem = -1;
	SetCurrentSelectedListItem(); 

	if (m_CurrentListItem == -1)
		return;
	CFieldViewDlg Dlg(m_ObjectList, m_CurrentListItem);
	Dlg.DoModal();
}

void COdaDgnTreeView::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	//Show element id handle in status bar
	SetCurrentSelectedItem();

	CMainFrame* pWnd = (CMainFrame*)theApp.GetMainWnd();

	OdDgElementId selectedElementId = GetSelectedElementId();
	if (!selectedElementId.isNull())
	{
		UINT uSelectedElementId = (OdUInt64)selectedElementId.getHandle();

		char text[500];
		sprintf(text, "Selected element id: %x", uSelectedElementId);
		OdString sSelectedElementId(text);

		pWnd->setStatusText(0, sSelectedElementId);
	}
	else
	{
		pWnd->setStatusText(0, OD_T("Ready"));
	}

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	m_ObjectList.DeleteAllItems();

	OdDgElementId oldObjId = reinterpret_cast<OdDbStub*>(pNMTreeView->itemOld.lParam);
	OdDgElementId newObjId = reinterpret_cast<OdDbStub*>(pNMTreeView->itemNew.lParam);

	OdDgSelectionSetPtr pSSet = GetDocument()->selectionSet();
	HTREEITEM h = pNMTreeView->itemOld.hItem;
	if (h && (m_ObjectTree.GetItemState(h, TVIF_STATE) & TVIS_BOLD) == 0)
	{
		pSSet->remove(oldObjId);
	}

	try
	{
		OdRxObjectPtr pObj = getTreeFiller()->getObject(pNMTreeView->itemNew);
		if (pObj.get())
		{
			DgnListFiller* pListFiller = getListFiller();
			pListFiller->dump(pObj);
		}
		pSSet->append(newObjId);
		//OdUInt32 nums = pSSet->numEntities();
		pSSet.release();
	}
	catch (const OdError& e)
	{
		int n = m_ObjectList.InsertItem(m_ObjectList.GetItemCount(), _T("OdError:"));
		m_ObjectList.SetItemText(n, 1, e.description());
	}
	catch (...)
	{
		int n = m_ObjectList.InsertItem(m_ObjectList.GetItemCount(), _T("ERROR:"));
		m_ObjectList.SetItemText(n, 1, _T("..."));
	}

	*pResult = 0;
}

void COdaDgnTreeView::OnEditClearSelection()
{
	GetDocument()->OnEditClearselection();
	OnRefresh();
}

void COdaDgnTreeView::OnButtonLinksAttrDlg()
{
	CLinkArrtDlg Dlg(m_idCurrentElement);
	Dlg.DoModal();
}

void COdaDgnTreeView::OnUpdateItemTypes(CCmdUI* pCmdUI)
{
  bool bEnableButton = false;

  if( !m_idCurrentElement.isNull() )
  {
    OdDgElementPtr pElm = m_idCurrentElement.openObject(OdDg::kForRead);

    if( !pElm.isNull() && pElm->isKindOf(OdDgGraphicsElement::desc()) )
      bEnableButton = true;
  }

  pCmdUI->Enable(bEnableButton);
}

void COdaDgnTreeView::OnItemTypes()
{
  CItemTypeAttachDlg Dlg(NULL, GetDocument()->database(), m_idCurrentElement);
  Dlg.DoModal();
}
