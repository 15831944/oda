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
// RegKey.h
//

#if !defined(_CREGKEY_H_)
#define _CREGKEY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace dgn
{
/////////////////////////////////////////////////////////////////////////////
// CRegKey

class CRegKey
{
public:
  CRegKey();
  ~CRegKey();

// Attributes
public:
  operator HKEY() const;
  HKEY m_hKey;

// Operations
public:
  LONG SetValue(DWORD dwValue, LPCTSTR lpszValueName);
  LONG QueryValue(DWORD& dwValue, LPCTSTR lpszValueName);
  LONG QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount);
  LONG SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);

  LONG SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);
  static LONG WINAPI SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName,
    LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL);

  LONG Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
    LPTSTR lpszClass = REG_NONE, DWORD dwOptions = REG_OPTION_NON_VOLATILE,
    REGSAM samDesired = KEY_ALL_ACCESS,
    LPSECURITY_ATTRIBUTES lpSecAttr = NULL,
    LPDWORD lpdwDisposition = NULL);
  LONG Open(HKEY hKeyParent, LPCTSTR lpszKeyName,
    REGSAM samDesired = KEY_ALL_ACCESS);
  LONG Close();
  HKEY Detach();
  void Attach(HKEY hKey);
  LONG DeleteSubKey(LPCTSTR lpszSubKey);
  LONG RecurseDeleteKey(LPCTSTR lpszKey);
  LONG DeleteValue(LPCTSTR lpszValue);
};

inline CRegKey::CRegKey()
{m_hKey = NULL;}

inline CRegKey::~CRegKey()
{Close();}

inline CRegKey::operator HKEY() const
{return m_hKey;}

inline HKEY CRegKey::Detach()
{
  HKEY hKey = m_hKey;
  m_hKey = NULL;
  return hKey;
}

inline void CRegKey::Attach(HKEY hKey)
{
  ODA_ASSERT(m_hKey == NULL);
  m_hKey = hKey;
}

inline LONG CRegKey::DeleteSubKey(LPCTSTR lpszSubKey)
{
  ODA_ASSERT(m_hKey != NULL);
  return RegDeleteKey(m_hKey, lpszSubKey);
}

inline LONG CRegKey::DeleteValue(LPCTSTR lpszValue)
{
  ODA_ASSERT(m_hKey != NULL);
  return RegDeleteValue(m_hKey, (LPTSTR)lpszValue);
}

inline LONG CRegKey::Close()
{
  LONG lRes = eOk;
  if (m_hKey != NULL)
  {
    lRes = RegCloseKey(m_hKey);
    m_hKey = NULL;
  }
  return lRes;
}

inline LONG CRegKey::Create(HKEY hKeyParent, LPCTSTR lpszKeyName,
  LPTSTR lpszClass, DWORD dwOptions, REGSAM samDesired,
  LPSECURITY_ATTRIBUTES lpSecAttr, LPDWORD lpdwDisposition)
{
  ODA_ASSERT(hKeyParent != NULL);
  DWORD dw;
  HKEY hKey = NULL;
  LONG lRes = RegCreateKeyEx(hKeyParent, lpszKeyName, 0,
    lpszClass, dwOptions, samDesired, lpSecAttr, &hKey, &dw);
  if (lpdwDisposition != NULL)
    *lpdwDisposition = dw;
  if (lRes == eOk)
  {
    lRes = Close();
    m_hKey = hKey;
  }
  return lRes;
}

inline LONG CRegKey::Open(HKEY hKeyParent, LPCTSTR lpszKeyName, REGSAM samDesired)
{
  ODA_ASSERT(hKeyParent != NULL);
  HKEY hKey = NULL;
  LONG lRes = RegOpenKeyEx(hKeyParent, lpszKeyName, 0, samDesired, &hKey);
  if (lRes == eOk)
  {
    lRes = Close();
    ODA_ASSERT(lRes == eOk);
    m_hKey = hKey;
  }
  return lRes;
}

inline LONG CRegKey::QueryValue(DWORD& dwValue, LPCTSTR lpszValueName)
{
  DWORD dwType = NULL;
  DWORD dwCount = sizeof(DWORD);
  LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
    (LPBYTE)&dwValue, &dwCount);
  ODA_ASSERT((lRes!=eOk) || (dwType == REG_DWORD));
  ODA_ASSERT((lRes!=eOk) || (dwCount == sizeof(DWORD)));
  return lRes;
}

inline LONG CRegKey::QueryValue(LPTSTR szValue, LPCTSTR lpszValueName, DWORD* pdwCount)
{
  ODA_ASSERT(pdwCount != NULL);
  DWORD dwType = NULL;
  LONG lRes = RegQueryValueEx(m_hKey, (LPTSTR)lpszValueName, NULL, &dwType,
    (LPBYTE)szValue, pdwCount);
  ODA_ASSERT((lRes!=eOk) || (dwType == REG_SZ) ||
       (dwType == REG_MULTI_SZ) || (dwType == REG_EXPAND_SZ));
  return lRes;
}

inline LONG WINAPI CRegKey::SetValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
  ODA_ASSERT(lpszValue != NULL);
  CRegKey key;
  LONG lRes = key.Create(hKeyParent, lpszKeyName);
  if (lRes == eOk)
    lRes = key.SetValue(lpszValue, lpszValueName);
  return lRes;
}

inline LONG CRegKey::SetKeyValue(LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
  ODA_ASSERT(lpszValue != NULL);
  CRegKey key;
  LONG lRes = key.Create(m_hKey, lpszKeyName);
  if (lRes == eOk)
    lRes = key.SetValue(lpszValue, lpszValueName);
  return lRes;
}

inline LONG CRegKey::SetValue(DWORD dwValue, LPCTSTR lpszValueName)
{
  ODA_ASSERT(m_hKey != NULL);
  return RegSetValueEx(m_hKey, lpszValueName, NULL, REG_DWORD,
    (BYTE * const)&dwValue, sizeof(DWORD));
}

inline LONG CRegKey::SetValue(LPCTSTR lpszValue, LPCTSTR lpszValueName)
{
  ODA_ASSERT(lpszValue != NULL);
  ODA_ASSERT(m_hKey != NULL);
  return RegSetValueEx(m_hKey, lpszValueName, NULL, REG_SZ,
    (BYTE * const)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
}

inline LONG CRegKey::RecurseDeleteKey(LPCTSTR lpszKey)
{
  CRegKey key;
  LONG lRes = key.Open(m_hKey, lpszKey, KEY_READ | KEY_WRITE);
  if (lRes != eOk)
    return lRes;
  FILETIME time;
  DWORD dwSize = 256;
  TCHAR szBuffer[256];
  while (RegEnumKeyEx(key.m_hKey, 0, szBuffer, &dwSize, NULL, NULL, NULL,
    &time)==eOk)
  {
    lRes = key.RecurseDeleteKey(szBuffer);
    if (lRes != eOk)
      return lRes;
    dwSize = 256;
  }
  key.Close();
  return DeleteSubKey(lpszKey);
}

};

#endif // !defined(_CREGKEY_H_)
