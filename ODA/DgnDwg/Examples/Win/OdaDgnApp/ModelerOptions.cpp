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
// ModelerOptions.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "ModelerOptions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CModelerOptions dialog

CModelerOptions::CModelerOptions(CWnd* pParent /*=NULL*/)
	: CDialog(CModelerOptions::IDD, pParent)
{
  //{{AFX_DATA_INIT(CFindObjectDlg)
  m_dDegree = 0;
  m_nKnots = 0;
  //}}AFX_DATA_INIT
}

CModelerOptions::~CModelerOptions()
{
}

void CModelerOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindObjectDlg)
	DDX_Text(pDX, IDC_EDIT_DEGREE, m_dDegree);
  DDV_MinMaxDouble(pDX, m_dDegree, 0, 359);
  DDX_Text(pDX, IDC_EDIT_KNOTS, m_nKnots);
  DDV_MinMaxUInt(pDX, m_nKnots, 0, 20);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CModelerOptions, CDialog)
  //{{AFX_MSG_MAP(CModelerOptions)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CModelerOptions message handlers
