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
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#ifdef _DLL
#ifndef _AFXDLL
#define _AFXDLL
#endif
#endif

#if !defined(AFX_STDAFX_H__FE7AA89C_1E2B_41A9_89CB_68581C7491C1__INCLUDED_)
#define AFX_STDAFX_H__FE7AA89C_1E2B_41A9_89CB_68581C7491C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if _MSC_VER > 1700
#include <SDKDDKVer.h>
#include <afxcmn.h>
#endif


#if _MSC_VER >= 1300

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#if   _MSC_VER >= 1800
#define WINVER 0x0501
#else
#define WINVER 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#if _MSC_VER >= 1800
#define _WIN32_WINNT 0x0501	
#else
#define _WIN32_WINNT 0x0500		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#endif // _MSC_VER  >= 1300

#define _ATL_APARTMENT_THREADED

#include <malloc.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxole.h>
#include <afxdisp.h>        // MFC OLE automation classes

#include <shlwapi.h>        // Windows shell extensions

#ifdef ENABLE_VBA_SUPPORT
#define APC_IMPORT_MIDL	//define our import mode
#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class COdaMfcAppModule : public CComModule
{
public:
	LONG Unlock();
	LONG Lock();
	DWORD dwThreadID;
};
extern COdaMfcAppModule _Module;

#include <atlcom.h>
#define HAS_NEW
#include <ApcMfc.h>
#include <ApcDual.h>
#include <ocidl.h>	// Added by ClassView
#include <atlwin.h>

#include "OdaMfcApp_h.h"
#endif // ENABLE_VBA_SUPPORT

/*
// the picture type is defined differently depending on import mode
// this is provided to allow compilation in any mode.
#if defined(APC_IMPORT_MIDL)
#define PicturePtr CComPtr<IPictureDisp>
#elif defined(APC_IMPORT_FULL)
#define PicturePtr CComPtr<Picture>
#else
#error "No Import Mode"
#endif
*/

class COdaMfcAppApp;

extern COdaMfcAppApp theApp;

#include "OdaCommon.h"
#define STL_USING_ALL
#include "OdaSTL.h"
#include "comdef.h"
#include "RxObjectImpl.h"
#include "ResBuf.h"
#include "RxVariantValue.h"
#include "RxDictionary.h"
#include "OdString.h"
#include "OdRound.h"
#include "OdPlatformStreamer.h"
#include "OdString.h"
#include "DbSystemServices.h"
#include "RxDynamicModule.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint3d.h"
#include "Gs/Gs.h"
#include "Gs/GsModel.h"
#include "Gs/GsBaseVectorizer.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiDrawableImpl.h"

#include <AFXPRIV.H>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__FE7AA89C_1E2B_41A9_89CB_68581C7491C1__INCLUDED_)
