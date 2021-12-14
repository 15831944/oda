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
// SHelperPage4.cpp : implementation file
//

#include "stdafx.h"
#include "OdaDgnApp.h"
#include "SHelperPage4.h"

#include "DgModelerGeometryCreator.h"


// CSHelperPage4 dialog

IMPLEMENT_DYNAMIC(CSHelperPage4, CPropertyPage)

CSHelperPage4::CSHelperPage4()
	: CPropertyPage(CSHelperPage4::IDD)
{

}

BOOL CSHelperPage4::OnInitDialog()
{
  CPropertyPage::OnInitDialog();

  CString text;
  
  OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule("TG_ModelerGeometry");
  if (pModule.isNull())
    text += _T("Error : TG_ModelerGeometry.txv not found!");
  else
  {
    text += _T("TG_ModelerGeometry.txv loaded.\r\n");
    OdRxClassPtr pService( odrxServiceDictionary()->getAt(OD_T("OdDgModelerGeometryCreator")) );
    if (pService.isNull())
      text += _T("Error : OdDgModelerGeometryCreator service not found!");
    else
    {
      text += _T("OdDgModelerGeometryCreator service available.\r\n");
      OdDgModelerGeometryCreatorPtr pCreator = pService->create();
      if (pCreator.isNull())
        text += _T("Error : OdDgModelerGeometryCreator cannot be created.\r\n");
      else
      {
        text += _T("OdDgModelerGeometryCreator created.\r\n");
      }
    }
  }

  m_Info.FmtLines(TRUE);
  m_Info.SetWindowText(text);

  return TRUE;
}

CSHelperPage4::~CSHelperPage4()
{
}

void CSHelperPage4::DoDataExchange(CDataExchange* pDX)
{
  CPropertyPage::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_EDIT1, m_Info);
}


BEGIN_MESSAGE_MAP(CSHelperPage4, CPropertyPage)
END_MESSAGE_MAP()


// CSHelperPage4 message handlers
