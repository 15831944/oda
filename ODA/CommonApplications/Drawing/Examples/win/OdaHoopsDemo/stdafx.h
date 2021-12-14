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

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#ifdef _DLL
#ifndef _AFXDLL
#define _AFXDLL
#endif
#endif


#if !defined (_STDAFX_INCLUDED)
#define _STDAFX_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes
//-------------------------------------------------- DLL
#ifdef _TOOLKIT_IN_DLL_

#ifdef _DEBUG
#pragma comment(lib,"hoopsd.lib")
#pragma comment(lib,"hoops_streamd.lib")
#pragma comment(lib,"hoops_utilsstatd_md.lib")
#else
#pragma comment(lib,"hoops.lib")
#pragma comment(lib,"hoops_stream.lib")
#pragma comment(lib,"hoops_utilsstat_md.lib")
#endif
#pragma comment(lib,"TD_Alloc.lib")
#endif // _TOOLKIT_IN_DLL
//--------------------------------------------------
#ifdef _MT // MT - MD configurations

#ifndef _DLL // MT configuration	// -------   MT

#pragma comment(lib,"tinyxml.lib")
#pragma comment(lib,"oless.lib")
#pragma comment(lib,"PSToolkit.lib")

#ifdef _DEBUG
#pragma comment(lib,"hoopstatd_mt.lib")
#pragma comment(lib,"hoops_streamstatd_mt.lib")
#pragma comment(lib,"hoops_utilsstatd_mt.lib")
#else
#pragma comment(lib,"hoopstat_mt.lib")
#pragma comment(lib,"hoops_streamstat_mt.lib")
#pragma comment(lib,"hoops_utilsstat_mt.lib")
#endif

#else	// MD configuration      	// -------   MD

#pragma comment(lib,"tinyxml.lib")
#pragma comment(lib,"oless.lib")
#pragma comment(lib,"PSToolkit.lib")
#pragma comment(lib,"TD_Alloc.lib")
#ifdef _DEBUG
#pragma comment(lib,"hoopstatd_md.lib")
#pragma comment(lib,"hoops_streamstatd_md.lib")
#pragma comment(lib,"hoops_utilsstatd_md.lib")
#else
#pragma comment(lib,"hoopstat_md.lib")
#pragma comment(lib,"hoops_streamstat_md.lib")
#pragma comment(lib,"hoops_utilsstat_md.lib")
#endif

#endif

#endif // MT-MD configuration_
//--------------------------------------------------

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "OdaCommon.h"
#include "RxModule.h"
#include "TDVersion.h"
#include "OdString.h"
#include "TD_PackPush.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#endif //_STDAFX_INCLUDED
