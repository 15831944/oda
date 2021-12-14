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

#if !defined (_REGISTRYCOMMON_INCLUDED)
#define _REGISTRYCOMMON_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SMALL_STRING_SIZE 32
#define LARGE_STRING_SIZE 1024
#define MAX_PATH_LENGTH   1024

#include "stdafx.h"
#include "OdaCommon.h"
#include "OdString.h"

OdString GetRegistryAcadLocation();
OdString GetRegistryACADFromProfile();
OdString GetRegistryAVEMAPSFromProfile();
OdString GetRegistryAcadProfilesKey();
bool GetRegistryString(  HKEY key, const wchar_t *subkey, const wchar_t *name, wchar_t *value, int size);
bool GetRegistryString(  HKEY key, const char *subkey, const char *name, char *value, int size);
HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA);
HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW);

#endif
