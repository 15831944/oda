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
#include "OdaDgnApp.h"
#include "FieldViewDlg.h"
#include "OdaDgnAppDoc.h"

CFieldViewDlg::CFieldViewDlg(CListCtrl &pObjList, int pCurElement, CWnd * pParent)
	: CDialog(IDD_FIELD_VIEW, pParent)
{
	m_sName = pObjList.GetItemText(pCurElement, 0);
	m_sValue = pObjList.GetItemText(pCurElement, 1);
}

CFieldViewDlg::~CFieldViewDlg()
{
}

BOOL CFieldViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return 0;
}

void CFieldViewDlg::DoDataExchange(CDataExchange * pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITFIELD, m_sValue);
	DDX_Text(pDX, IDC_FIELD_NAME, m_sName);
}
