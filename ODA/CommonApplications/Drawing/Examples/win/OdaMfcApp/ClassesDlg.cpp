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

// ClassesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "ClassesDlg.h"
#include "RxDictionary.h"
#include "Ed/EdCommandStack.h"


TOOLKIT_EXPORT OdRxDictionaryIteratorPtr odDbGetDrawingClasses(OdDbDatabase& database);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClassesDlg dialog


CClassesDlg::CClassesDlg(OdDbDatabase* pDb, CWnd* pParent /*=NULL*/)
	: CDialog(CClassesDlg::IDD, pParent), m_pDb(pDb)
{
	//{{AFX_DATA_INIT(CClassesDlg)
	//}}AFX_DATA_INIT
}


void CClassesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClassesDlg)
	DDX_Control(pDX, IDC_TREE1, m_ClassTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CClassesDlg, CDialog)
	//{{AFX_MSG_MAP(CClassesDlg)
	//}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_BTN_CLASSES_DUMP, &CClassesDlg::OnBnClickedBtnClassesDump)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassesDlg message handlers

void addDictionary(CTreeCtrl& tree, HTREEITEM hRoot, OdRxDictionary* pDict);

HTREEITEM appendClass(CTreeCtrl& tree, HTREEITEM hRoot, const OdRxClass* pClass)
{
  const OdRxClass* pParent = pClass->myParent();
  HTREEITEM hParent = hRoot;
  if(pParent)
  {
    hParent = appendClass(tree, hRoot, pParent);
  }
  HTREEITEM hExisting = tree.GetNextItem( hParent, TVGN_CHILD );
  CString sClassName = (LPCTSTR)pClass->name();
  while(hExisting)
  {
    if(tree.GetItemText(hExisting)==sClassName)
      return hExisting;
    hExisting = tree.GetNextItem( hExisting, TVGN_NEXT );
  }
  return tree.InsertItem(sClassName, hParent);
}

void addClasses(CTreeCtrl& tree, HTREEITEM hRoot, OdRxIterator* pIter)
{
  HTREEITEM hItem;
  try
  {
    while(!pIter->done())
    {
      hItem = appendClass(tree, hRoot, OdRxClassPtr(pIter->object()));
      pIter->next();
    }
  }
  catch(...)
  {
    tree.InsertItem(_T("Error..."), hRoot);
  }
}

void addSet(CTreeCtrl& tree, HTREEITEM hRoot, OdRxIterator* pIter)
{
  OdRxDictionaryIteratorPtr pDictIter = OdRxDictionaryIterator::cast(pIter);
  HTREEITEM hItem;
  try
  {
    while(!pIter->done())
    {
      OdRxObjectPtr pObj = pIter->object();
      hItem = tree.InsertItem(pDictIter.get() ? pDictIter->getKey() : pObj->isA()->name(), hRoot);

      OdRxDictionaryPtr pDict = OdRxDictionary::cast(pObj);
      if(pDict.get())
      {
        if(pDict==::odrxClassDictionary())
          addClasses(tree, hItem, pDict->newIterator());
        else
          addDictionary(tree, hItem, pDict);
      }
      else
      {
        OdEdCommandStackPtr pCommands = OdEdCommandStack::cast(pObj);
        if(pCommands.get())
        {
          HTREEITEM hCmdRoot = tree.InsertItem(
            TVIF_TEXT|TVIF_STATE,
            _T("Commands"),
            0,0,
            TVIS_BOLD|TVIS_EXPANDED, TVIS_BOLD|TVIS_EXPANDED,
            0, hItem, TVI_LAST);
          
          try
          {
            addSet(tree, hCmdRoot, pCommands->newIterator());
          }
          catch(...)
          {
            tree.InsertItem(_T("Error..."), hCmdRoot);
          }
          
          hCmdRoot = tree.InsertItem(
            TVIF_TEXT|TVIF_STATE,
            _T("Groups"),
            0,0,
            TVIS_BOLD|TVIS_EXPANDED, TVIS_BOLD|TVIS_EXPANDED,
            0, hItem, TVI_LAST);
          
          try
          {
            addSet(tree, hCmdRoot, pCommands->newGroupIterator());
          }
          catch(...)
          {
            tree.InsertItem(_T("Error..."), hCmdRoot);
          }
        }
      }
      pIter->next();
    }
  }
  catch(...)
  {
    tree.InsertItem(_T("Error..."), hRoot);
  }
}

void addDictionary(CTreeCtrl& tree, HTREEITEM hRoot, OdRxDictionary* pDict)
{
  addSet(tree, hRoot, pDict->newIterator(OdRx::kDictSorted));
}

BOOL CClassesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  HTREEITEM hRoot;
  
  hRoot = m_ClassTree.InsertItem(
    TVIF_TEXT|TVIF_STATE,
    _T("System Registry"),
    0,0,
    TVIS_BOLD|TVIS_EXPANDED, TVIS_BOLD|TVIS_EXPANDED,
    0, TVI_ROOT, TVI_LAST);

  ::addDictionary(m_ClassTree, hRoot, ::odrxSysRegistry());

  hRoot = m_ClassTree.InsertItem(
    TVIF_TEXT|TVIF_STATE,
    _T("Drawing Classes"),
    0,0,
    TVIS_BOLD|TVIS_EXPANDED, TVIS_BOLD|TVIS_EXPANDED,
    0, TVI_ROOT, TVI_LAST);

  OdRxDictionaryPtr pClassDict = ::odrxClassDictionary();
  OdRxDictionaryIteratorPtr pIter = odDbGetDrawingClasses(*m_pDb);
  CString str;
  while(!pIter->done())
  {
    str = (LPCTSTR)pIter->getKey();
    OdString strApp;
    if(!pClassDict->has(pIter->getKey()))
    {
      str.Format(_T("[%s]"), (LPCTSTR)pIter->getKey());
    }
    OdRxClass* pClass = OdRxClass::cast(pIter->object());
    if (pClass)
      strApp = pClass->appName();
    if (!strApp.isEmpty())
    {
      str += L"   App:  ";
      str += (LPCSTR)strApp;
    }

    /*HTREEITEM hItem = */ m_ClassTree.InsertItem(str, hRoot);
    pIter->next();
  }

	return TRUE;
}

void recursiveDumpToFile(CTreeCtrl* pControl, HTREEITEM hItemParent, unsigned int level, std::wofstream& file)
{
  pControl->SortChildren(hItemParent);

  for (unsigned int i = 0; i < level; i++)
  {
    file << _T("  ");
  }

  file << pControl->GetItemText(hItemParent).GetString() << std::endl;

  HTREEITEM childItem;
  if (pControl->ItemHasChildren(hItemParent))
  {
    for (childItem = pControl->GetChildItem(hItemParent); childItem != NULL; childItem = pControl->GetNextItem(childItem, TVGN_NEXT))
    {
      recursiveDumpToFile(pControl, childItem, level + 1, file);
    }
  }
}

void CClassesDlg::OnBnClickedBtnClassesDump()
{
  CFileDialog FileDlg(FALSE, 
    CString(".txt"), 
    CString("oda_classes_list_dump.txt"), 
    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
    );

  if (FileDlg.DoModal() == IDOK)
  {
    std::wofstream file;
    file.open(FileDlg.GetPathName().GetString());
    recursiveDumpToFile(&m_ClassTree, m_ClassTree.GetRootItem(), 0, file);
  }
}
