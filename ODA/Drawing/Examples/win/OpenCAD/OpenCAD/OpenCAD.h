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

// OpenCAD.h : main header file for the OpenCAD application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "OdaCommon.h"
#include "OdToolKit.h"
#include "../Extensions/ExServices/ExHostAppServices.h"
#include "../Extensions/ExServices/ExSystemServices.h"

// COpenCADApp:
// See OpenCAD.cpp for the implementation of this class
//

class COpenCADApp : public CWinApp, public ExSystemServices, public ExHostAppServices
{
protected:
	using CWinApp::operator new;
	using CWinApp::operator delete;
	void addRef() {}
	void release() {}
public:
	BOOL InitializeTeigha();
	void UninitializeTeigha();
	void LoadDRXModules();
	BOOL AllowInteraction();

public:
	COpenCADApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	virtual int ExitInstance();
};

extern COpenCADApp theApp;
