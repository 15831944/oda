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

// NewViewDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "NewViewDlg.h"
#include "NamedViewsDlg.h"
#include "DbSymbolTable.h"
#include "DbViewTableRecord.h"
#include "DbUCSTableRecord.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewViewDlg dialog


CNewViewDlg::CNewViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewViewDlg)
	m_sViewName = _T("");
	m_sViewCategory = _T("");
	m_bStoreLS = TRUE;
	m_bSaveUCS = TRUE;
	m_sUcsName = _T("");
	//}}AFX_DATA_INIT
}


void CNewViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewViewDlg)
	DDX_Control(pDX, IDC_COMBO_UCSNAME, m_UCSs);
	DDX_Control(pDX, IDC_COMBO_VIEWCATEGORY, m_categories);
	DDX_Text(pDX, IDC_EDIT_VIEWNAME, m_sViewName);
	DDX_CBString(pDX, IDC_COMBO_VIEWCATEGORY, m_sViewCategory);
	DDX_Check(pDX, IDC_CHECK_STORE_LS, m_bStoreLS);
	DDX_Check(pDX, IDC_CHECK_SAVEUCS, m_bSaveUCS);
	DDX_CBString(pDX, IDC_COMBO_UCSNAME, m_sUcsName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewViewDlg, CDialog)
	//{{AFX_MSG_MAP(CNewViewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewViewDlg message handlers

OdString orthoTypeString(OdDb::OrthographicView type);
OdString ucsString(const OdDbObject* pViewObj);

BOOL CNewViewDlg::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  CNamedViewsDlg* pParent = static_cast<CNamedViewsDlg*>(GetParent());
  OdDbDatabase* pDb = pParent->database();
  OdDbSymbolTablePtr pTable;
  OdDbSymbolTableIteratorPtr pIter;

  pTable = pDb->getViewTableId().safeOpenObject();
  for (pIter = pTable->newIterator(); !pIter->done(); pIter->step() )
  {
    OdDbViewTableRecordPtr pView = pIter->getRecordId().openObject();
    OdString sCategory = pView->getCategoryName();
    if(!sCategory.isEmpty())
    {
      if(m_categories.FindString(-1, sCategory)==-1)
        m_categories.AddString(sCategory);
    }
  }

  pTable = pDb->getUCSTableId().safeOpenObject();
  m_UCSs.AddString(_T("World"));

  m_sUcsName = (LPCTSTR)ucsString(pDb->activeViewportId().safeOpenObject());
  if(m_sUcsName==_T("Unnamed"))
    m_UCSs.AddString(m_sUcsName);

  for (pIter = pTable->newIterator(); !pIter->done(); pIter->step() )
  {
    OdDbUCSTableRecordPtr pUCS = pIter->getRecordId().openObject();
    m_UCSs.AddString(pUCS->getName());
  }

  UpdateData(FALSE);
  return TRUE;
}
