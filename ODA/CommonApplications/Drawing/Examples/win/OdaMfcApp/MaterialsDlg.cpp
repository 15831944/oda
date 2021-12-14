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

// MaterialsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "MaterialsDlg.h"

#include "Ve/AveMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaterialsDlg dialog


CMaterialsDlg::CMaterialsDlg(OdDbDatabase* pDb, CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialsDlg::IDD, pParent)
  , m_pDb(pDb)
{
	//{{AFX_DATA_INIT(CMaterialsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMaterialsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaterialsDlg)
	DDX_Control(pDX, IDC_MATERIALS_LIST, m_MaterialsList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaterialsDlg, CDialog)
	//{{AFX_MSG_MAP(CMaterialsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaterialsDlg message handlers

BOOL CMaterialsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  if (m_pDb)
  {
    RECT rect;
    m_MaterialsList.GetClientRect(&rect);
    m_MaterialsList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, (rect.right-rect.left)/2 );
    m_MaterialsList.InsertColumn(1, _T("Type"), LVCFMT_LEFT, (rect.right-rect.left)/2 );

    OdRxIteratorPtr pIt = oddbGetMaterials(m_pDb);
    for (; !pIt->done(); pIt->next())
    {
      OdAveMaterialPtr pMat = pIt->object();
      if (!pMat.isNull())
      {
        OdString sType;
        switch (pMat->type())
        {
          case OdAve::kStandard:
            sType = _T("Standard");
            break;
          case OdAve::kGranite:
            sType = _T("Granite");
            break;
          case OdAve::kMarble:
            sType = _T("Marble");
            break;
          case OdAve::kWood:
            sType = _T("Wood");
            break;
          default:
            ODA_FAIL();
        }
        int nItem = m_MaterialsList.GetItemCount();
        m_MaterialsList.InsertItem(nItem, pMat->name());
        m_MaterialsList.SetItemText (nItem, 1, sType);
      }
    }
  }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
