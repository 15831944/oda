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
#include "CPdfExportSelectViewsDialog.h"
//#include "afxdialogex.h"


// CPdfExportSelectViewsDialog dialog

//IMPLEMENT_DYNAMIC(CPdfExportSelectViewsDialog, CDialogEx)

CPdfExportSelectViewsDialog::CPdfExportSelectViewsDialog(OdDgDatabasePtr pDb, OdArray<OdString> *layouts, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PDF_EXPORT_LAYOUT_VIEWS, pParent)
{
  m_pDb = pDb;
  m_pLayouts = layouts;
}

CPdfExportSelectViewsDialog::~CPdfExportSelectViewsDialog()
{
}

void CPdfExportSelectViewsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_LIST_VIEWS, m_ViewList);
}


BEGIN_MESSAGE_MAP(CPdfExportSelectViewsDialog, CDialog)
  ON_BN_CLICKED(IDOK, OnOk)
  ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()


// CPdfExportSelectViewsDialog message handlers
BOOL CPdfExportSelectViewsDialog::OnInitDialog()
{
  if (!CDialog::OnInitDialog())
    return FALSE;

  int counter = 0;
  m_ViewList.ResetContent();
  //////////////////////////////////////////////////////////////////////////
  OdDgModelTablePtr pModelTable = m_pDb->getModelTable();
  if (!pModelTable.isNull())
  {
    OdDgElementIteratorPtr pIter = pModelTable->createIterator();
    for (; !pIter->done(); pIter->step())
    {
      OdDgModelPtr pModel = OdDgModel::cast(pIter->item().openObject());
      if (pModel.get())
      {
        OdString layoutName = pModel->getName();
        OdDgElementId idViewGroup = pModel->getViewGroup(true);

        if (!idViewGroup.isNull())
        {
          OdDgViewGroupPtr pViewGroup = idViewGroup.openObject(OdDg::kForRead);

          if (!pViewGroup.isNull())
          {
            OdDgElementIteratorPtr pViewIter = pViewGroup->createIterator();

            for (; !pViewIter->done(); pViewIter->step())
            {
              OdDgViewPtr pView = pViewIter->item().openObject(OdDg::kForRead);

              if (!pView.isNull() && pView->getVisibleFlag())
              {
                OdString viewIndex;
                viewIndex.format(L"%d", pView->getIndex());

                OdString strLayoutAndView;
                strLayoutAndView = layoutName + L"$" + viewIndex;

                OdString strDisplay = L"Layout - \"" + layoutName + L"\", view - \"" + viewIndex + L"\"";
                m_ViewList.InsertString(counter, (LPCTSTR)strDisplay);
                m_LayoutsViews.insertAt(counter++, strLayoutAndView);
              }
            }
          }
        }
      }
    }
  }
  //////////////////////////////////////////////////////////////////////////

  if (!counter)
    GetDlgItem(IDOK)->EnableWindow(FALSE);

  UpdateData(FALSE);
  return TRUE;
}

void CPdfExportSelectViewsDialog::OnOk()
{
  UpdateData(TRUE);
  m_pLayouts->clear();

  int nCount = m_ViewList.GetSelCount();
  CArray<int, int> itemsArray;

  itemsArray.SetSize(nCount);
  m_ViewList.GetSelItems(nCount, itemsArray.GetData());

  for (int i = 0; i < itemsArray.GetCount(); i++)
  {
    int index = itemsArray.GetAt(i);
    m_pLayouts->push_back(m_LayoutsViews.at(index));
  }

  CDialog::OnOK();
}

void CPdfExportSelectViewsDialog::OnCancel()
{
  if (m_pLayouts)
    m_pLayouts->clear();

  CDialog::OnCancel();
}
