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

// SingleDocDoc.cpp : implementation of the CSingleDocDoc class
//

#include "stdafx.h"
#include "SingleDoc.h"

#include "SingleDocDoc.h"
#include "SingleDocView.h"

/////////////////////////////////////////////////////////////////////////////
// CSingleDocDoc

IMPLEMENT_DYNCREATE(CSingleDocDoc, CDocument)

BEGIN_MESSAGE_MAP(CSingleDocDoc, CDocument)
	//{{AFX_MSG_MAP(CSingleDocDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

extern CSingleDocApp theApp;

void CSingleDocDoc::ClearGsModel()
{
  if (!m_pDb.isNull()) // clear gs model associated with the previous database
  {
    POSITION pos = GetFirstViewPosition();
    while (pos)
    {
      CSingleDocView* view = (CSingleDocView*)GetNextView(pos);
      view->m_pDevice = 0;
      view->m_pPrinterDevice = 0;
    }
  }
}

BOOL CSingleDocDoc::OnNewDocument() 
{
  ClearGsModel();

  /**********************************************************************/
  /* Create a new default English database                              */
  /**********************************************************************/
  OdDb::MeasurementValue measure = OdDb::kEnglish;
  m_pDb = theApp.createDatabase(true, measure);
	
	return CDocument::OnNewDocument();
}

BOOL CSingleDocDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  if (!CDocument::OnOpenDocument(lpszPathName))
    return FALSE;
  /**********************************************************************/
  /* Load the specified DWG file into the database, allow code page     */
  /* conversion disallow partial load.                                  */
  /**********************************************************************/
  try
  {
    ClearGsModel(); // clear gs model associated with the previous database

    m_pDb =  theApp.readFile(lpszPathName, true, false);
  }
  catch(OdError& /*e*/)
  {
  }
  catch(UserBreak)
  {
    return FALSE;
  }
  return TRUE;
}
