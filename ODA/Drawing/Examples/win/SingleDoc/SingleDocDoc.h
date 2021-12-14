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

/************************************************************************/
/* Interface of the CSingleDocDoc class                                 */
/************************************************************************/
#if !defined(AFX_SINGLEDOCDOC_H__CF9D0BA4_03B5_4B04_A391_FCB5577CF9A9__INCLUDED_)
#define AFX_SINGLEDOCDOC_H__CF9D0BA4_03B5_4B04_A391_FCB5577CF9A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DbDatabase.h"

class CSingleDocDoc : public CDocument
{
protected: // create from serialization only
  CSingleDocDoc(){}
	DECLARE_DYNCREATE(CSingleDocDoc)

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSingleDocDoc)
	public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(CSingleDocDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
  void ClearGsModel();

public:
  OdDbDatabasePtr m_pDb;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGLEDOCDOC_H__CF9D0BA4_03B5_4B04_A391_FCB5577CF9A9__INCLUDED_)
