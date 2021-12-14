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

// OdDwgSignEx.h : main header file for the ODDWGSIGNEX application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


#include "OdToolKit.h"
//#include "DbObjectId.h"
//#include "RxObjectImpl.h"

#include "ExDrawingSignatureApp.h"

// COdDwgSignExApp:
// See OdDwgSignEx.cpp for the implementation of this class
//

class COdDwgSignExApp : public CWinApp
{
protected:
  using CWinApp::operator new;
  using CWinApp::operator delete;

public:
	COdDwgSignExApp();

  ExDrawingSignatureApp m_signatureApp;

public:  
  static int messageBox(HWND hParent, LPCTSTR lpCaption, LPCTSTR lpText, UINT uType)
  {
    return ::MessageBox(hParent, lpText, lpCaption, uType);
  }
  int messageBox(LPCTSTR lpCaption, LPCTSTR lpText, UINT uType)
  {
    CWnd* pMainWnd = GetMainWnd();
    HWND hWnd = 0;
    if(pMainWnd)
      hWnd = pMainWnd->m_hWnd;
    return messageBox(hWnd, lpCaption, lpText, uType);
  }  
  void reportError(LPCTSTR szContextMsg, const OdError& e)
  {
    messageBox(szContextMsg, (LPCTSTR)e.description(), MB_OK|MB_ICONERROR);
  }

	virtual BOOL InitInstance();
  virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

extern COdDwgSignExApp theApp;
