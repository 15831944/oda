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

#include "RegistryCommon.h"

bool GetRegistryString(  HKEY key, const char *subkey, const char *name, char *value, int size)
{
  wchar_t *wSubkey = NULL;
  wchar_t *wName = NULL;
  char *tmpChar = NULL;
  bool rv = false;
  AnsiToUnicode( subkey, &wSubkey);
  AnsiToUnicode( name, &wName);
  if (wSubkey && wName)
  {
    wchar_t *wValue = new wchar_t[ size];
    rv = GetRegistryString(  key, wSubkey, wName, wValue, size ) ;
    if (rv)
    {
      UnicodeToAnsi( wValue, &tmpChar);
      if(tmpChar)
      {
        strcpy( value, tmpChar);
        CoTaskMemFree( tmpChar);
      }
      else
      {
        rv = false;
      }
    }
    delete [] wValue;
    CoTaskMemFree( wName);
    CoTaskMemFree( wSubkey);
  }

  return rv;
}


//////////////////////////////////////////////////////////////////////

bool GetRegistryString(  HKEY key, const wchar_t *subkey, const wchar_t *name, wchar_t *value, int size)
{
  bool rv = false;
  HKEY hKey;
  if( RegOpenKeyExW( key, subkey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
  {
    DWORD dwSize( MAX_PATH_LENGTH);
    unsigned char data[MAX_PATH_LENGTH];
    memset (&data, 0x00, MAX_PATH_LENGTH);
    if( RegQueryValueExW( hKey, name, 0, 0, &data[0], &dwSize) == ERROR_SUCCESS)
    {
      rv = true;
    }
    else
    {
      if ( ERROR_SUCCESS == RegEnumKeyExW(hKey, 0, (LPWSTR)(unsigned short*)&data[0], &dwSize
        , NULL, NULL, NULL, NULL))
      {
        rv = true;
      }
    }

    if( size < MAX_PATH_LENGTH)
    {
      swprintf( value, L"%s\0", data);
    }
    else
    {
      wcsncpy( value, (wchar_t*)data, size - 1);
      value[ size-1] = '\0';
    }

    RegCloseKey(hKey);
  }
  return rv;
}


//////////////////////////////////////////////////////////////////////
OdString GetRegistryAcadLocation()
{
  OdString subkey = L"SOFTWARE\\Autodesk\\AutoCAD";
  TCHAR version[ SMALL_STRING_SIZE];
  TCHAR subVersion[ SMALL_STRING_SIZE];
  TCHAR searchPaths[ LARGE_STRING_SIZE];

  // get the version and concatenate onto subkey
  if( GetRegistryString( HKEY_LOCAL_MACHINE, (LPCTSTR)subkey, _T("CurVer"), version, SMALL_STRING_SIZE) == 0)
    return L"";
  subkey += L"\\";
  subkey += version;

  // get the sub-version and concatenate onto subkey
  if( GetRegistryString( HKEY_LOCAL_MACHINE, (LPCTSTR)subkey, _T("CurVer"), subVersion, SMALL_STRING_SIZE) == 0)
    return L"";
  subkey += L"\\";
  subkey += subVersion;

  // get the value for the AcadLocation entry in the registry
  if( GetRegistryString( HKEY_LOCAL_MACHINE, (LPCTSTR)subkey, _T("AcadLocation"), searchPaths, LARGE_STRING_SIZE) == 0)
    return L"";

  return OdString(searchPaths);
}

//////////////////////////////////////////////////////////////////////
OdString GetRegistryAcadProfilesKey()
{
  OdString subkey = L"SOFTWARE\\Autodesk\\AutoCAD";
  TCHAR version[ SMALL_STRING_SIZE];
  TCHAR subVersion[ SMALL_STRING_SIZE];
  TCHAR profile[ LARGE_STRING_SIZE];
  // char searchPaths[ LARGE_STRING_SIZE];

  if( GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), version, SMALL_STRING_SIZE) == 0)
    return L"";
  subkey += L"\\";
  subkey += version;

  // get the sub-version and concatenate onto subkey
  if( GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), subVersion, SMALL_STRING_SIZE) == 0)
    return L"";
  subkey += L"\\";
  subkey += subVersion;
  subkey += L"\\Profiles";

  // get the value for the (Default) entry in the registry
  if( GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, _T(""), profile, LARGE_STRING_SIZE) == 0)
    return L"";
  subkey += L"\\";
  subkey += profile;

  return subkey;
}

//////////////////////////////////////////////////////////////////////
OdString GetRegistryACADFromProfile()
{
  OdString subkey; // = "SOFTWARE\\Autodesk\\AutoCAD";
  TCHAR searchPaths[ LARGE_STRING_SIZE];

  subkey = GetRegistryAcadProfilesKey();
  if ( !subkey.isEmpty() )
  {
    subkey += L"\\General";
    // get the value for the ACAD entry in the registry
    if( GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("ACAD"), searchPaths, LARGE_STRING_SIZE) == 0)
      return L"";
    return OdString(searchPaths);
  } 
  else
    return L"";
}

OdString GetRegistryAVEMAPSFromProfile()
{
  OdString subkey; // = "SOFTWARE\\Autodesk\\AutoCAD";
  TCHAR searchPaths[ LARGE_STRING_SIZE];

  subkey = GetRegistryAcadProfilesKey();
  if ( !subkey.isEmpty() )
  {
    subkey += L"\\General";
    // get the value for the ACAD entry in the registry
    if( GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("AVEMAPS"), searchPaths, LARGE_STRING_SIZE) == 0)
      return L"";
    return OdString(searchPaths);
  } 
  else
    return L"";
}

/*
* AnsiToUnicode converts the ANSI string pszA to a Unicode string
* and returns the Unicode string through ppszW. Space for the
* the converted string is allocated by AnsiToUnicode.
*/
HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW)
{

  ULONG cCharacters;
  DWORD dwError;

  // If input is null then just return the same.
  if (NULL == pszA)
  {
    *ppszW = NULL;
    return NOERROR;
  }

  // Determine number of wide characters to be allocated for the
  // Unicode string.
  cCharacters =  strlen(pszA)+1;

  // Use of the OLE allocator is required if the resultant Unicode
  // string will be passed to another COM component and if that
  // component will free it. Otherwise you can use your own allocator.
  *ppszW = (LPOLESTR) CoTaskMemAlloc(cCharacters*2);
  if (NULL == *ppszW)
    return E_OUTOFMEMORY;

  // Covert to Unicode.
  if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
    *ppszW, cCharacters))
  {
    dwError = GetLastError();
    CoTaskMemFree(*ppszW);
    *ppszW = NULL;
    return HRESULT_FROM_WIN32(dwError);
  }

  return NOERROR;
}
/*
* UnicodeToAnsi converts the Unicode string pszW to an ANSI string
* and returns the ANSI string through ppszA. Space for the
* the converted string is allocated by UnicodeToAnsi.
*/

HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA)
{
  ULONG cbAnsi, cCharacters;
  DWORD dwError;

  // If input is null then just return the same.
  if (pszW == NULL)
  {
    *ppszA = NULL;
    return NOERROR;
  }

  cCharacters = wcslen(pszW)+1;
  // Determine number of bytes to be allocated for ANSI string. An
  // ANSI string can have at most 2 bytes per character (for Double
  // Byte Character Strings.)
  cbAnsi = cCharacters*2;

  // Use of the OLE allocator is not required because the resultant
  // ANSI  string will never be passed to another COM component. You
  // can use your own allocator.
  *ppszA = (LPSTR) CoTaskMemAlloc(cbAnsi);
  if (NULL == *ppszA)
    return E_OUTOFMEMORY;

  // Convert to ANSI.
  if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, *ppszA,
    cbAnsi, NULL, NULL))
  {
    dwError = GetLastError();
    CoTaskMemFree(*ppszA);
    *ppszA = NULL;
    return HRESULT_FROM_WIN32(dwError);
  }
  return NOERROR;

}
