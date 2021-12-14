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

// LightsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "odamfcapp.h"
#include "LightsDlg.h"

#include "Ve/AveLight.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLightsDlg dialog

CLightsDlg::CLightsDlg(OdDbDatabase* pDb, CWnd* pParent /*=NULL*/)
	: CDialog(CLightsDlg::IDD, pParent)
  , m_pDb(pDb)
{
	//{{AFX_DATA_INIT(CLightsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLightsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLightsDlg)
	DDX_Control(pDX, IDC_LIGHTS_LIST, m_LightsList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLightsDlg, CDialog)
	//{{AFX_MSG_MAP(CLightsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLightsDlg message handlers

BOOL CLightsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  if (m_pDb)
  {
    RECT rect;
    m_LightsList.GetClientRect(&rect);
    m_LightsList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, (rect.right-rect.left)/2 );
    m_LightsList.InsertColumn(1, _T("Type"), LVCFMT_LEFT, (rect.right-rect.left)/2 );

    OdRxIteratorPtr pIt = oddbGetLights(m_pDb);
    for (; pIt.get() && !pIt->done(); pIt->next())
    {
      OdAveLightPtr pLt = pIt->object();
      if (!pLt.isNull())
      {
        OdString sType;
        switch (pLt->type())
        {
          case OdGsLight::kPointLight:
            sType = _T("Point light");
            break;
          case OdGsLight::kDistantLight:
            sType = _T("Distant light");
            break;
          case OdGsLight::kSpotlight:
            sType = _T("Spotlight");
            break;
          default:
            ODA_FAIL();
        }
        int nItem = m_LightsList.GetItemCount();
        m_LightsList.InsertItem(nItem, pLt->name());
        m_LightsList.SetItemText (nItem, 1, sType);
      }
    }
  }
   
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
