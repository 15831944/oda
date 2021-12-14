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
/* Main header file for the SINGLEDOC application                       */
/************************************************************************/
#if !defined(AFX_SINGLEDOC_H__976866CE_482B_4A11_A79F_ED62B988BA4A__INCLUDED_)
#define AFX_SINGLEDOC_H__976866CE_482B_4A11_A79F_ED62B988BA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "SingleDocDoc.h"
#include "Mainfrm.h"
#include "OdaCommon.h"
#include "OdToolKit.h"
#include "DbObjectId.h"
#include "RxObjectImpl.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "DynamicLinker.h"
#include "DbObject.h"
#include "Gs/Gs.h"
#include "DbSystemServices.h"
//#include "GsDeviceManager.h"
#include "DbGsManager.h"

class UserBreak
{
};


/************************************************************************/
/* See SingleDoc.cpp for implementation of CSingleDocApp                */
/************************************************************************/
class CSingleDocApp : public CWinApp
  , public ExSystemServices
  , public ExHostAppServices
{
protected:
  using CWinApp::operator new;
  using CWinApp::operator delete;
public:
	CSingleDocApp();

  ODCOLORREF            m_background;
  int                   m_rendererID;
  BOOL m_bSavePreview;
  int                   m_nProgressLimit;
  int                   m_nProgressPos;
  int                   m_nPercent;
  CString               m_Msg;

  void addRef() {}
  void release() {}

 	//BOOL ProcessShellCommand(CCommandLineInfo& rCmdInfo);

  virtual void start(const char* displayString = NULL);
  virtual void stop();
  virtual void meterProgress();
  virtual void setLimit(int max);
  virtual void warning(const char* warnVisGroup, const OdString& sMsg);

  void OnOptionsTriangulationParams() ;

  const ODCOLORREF activeBackground() const { return m_background; }
  const ODCOLORREF* curPalette() const;

  CSingleDocDoc *GetActiveDocument(){
    if( !((CSingleDocDoc*)((CMainFrame*)m_pMainWnd) ))
      return NULL;
    return (CSingleDocDoc*)((CMainFrame*)m_pMainWnd)->GetActiveDocument();
  }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSingleDocApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSingleDocApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGLEDOC_H__976866CE_482B_4A11_A79F_ED62B988BA4A__INCLUDED_)
