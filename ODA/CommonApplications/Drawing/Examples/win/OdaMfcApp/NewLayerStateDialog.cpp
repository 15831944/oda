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

// NamedViewsDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "OdaMfcApp.h"
#include "NewLayerStateDialog.h"

IMPLEMENT_DYNAMIC(NewLayerStateDialog, CDialog)

NewLayerStateDialog::NewLayerStateDialog(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_NEW_LAYER_STATE, pParent)
  , m_sDescription(_T(""))
  , m_sName(_T(""))
{

}

NewLayerStateDialog::~NewLayerStateDialog() {}

void NewLayerStateDialog::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIT2, m_sDescription);
  DDX_Text(pDX, IDC_EDIT1, m_sName);
}

BEGIN_MESSAGE_MAP(NewLayerStateDialog, CDialog)
END_MESSAGE_MAP()
