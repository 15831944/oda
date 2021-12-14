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

//
// Decrypt the buffer with a session key derived from a password. 
//
#include "stdafx.h"
#include "OdaMfcApp.h"
#include "PasswordDlg.h"

#define _WIN32_WINNT 0x0400
#if(_WIN32_WINNT < 0x0400)

bool COdaMfcAppApp::decryptData(OdBinaryData& , const OdSecurityParams*)
{
  return false;
}

bool COdaMfcAppApp::encryptData(OdBinaryData& , const OdSecurityParams*)
{
  return false;
}

#else

#include <wincrypt.h>

class OdWinDecrypt
{
  HCRYPTPROV m_hCryptProv; // the provider handle
  HCRYPTHASH m_hHash;      // the hash object
  HCRYPTKEY  m_hKey;       // the session key
public:
  OdWinDecrypt() : m_hCryptProv(0), m_hHash(0), m_hKey(0) {}
  ~OdWinDecrypt() { clear(); }

  void clear()
  {
    if(m_hHash)
      CryptDestroyHash(m_hHash);
    m_hHash = 0;
    if(m_hKey)
      CryptDestroyKey(m_hKey);
    m_hKey = 0;
    if(m_hCryptProv)
      CryptReleaseContext(m_hCryptProv, 0);
    m_hCryptProv = 0;
  }

  bool prepareKey(const OdSecurityParams* pSecurityParams)
  {
    if (!pSecurityParams)
      return false;

    clear();

    // Get a handle to the default provider. 
    if(!CryptAcquireContextW(
         &m_hCryptProv,
         NULL,
         (LPCWSTR)pSecurityParams->provName.asArrayPtr(),
         pSecurityParams->nProvType,
         CRYPT_VERIFYCONTEXT))
    {
      ODA_TRACE0("Error during CryptAcquireContext!");
      return false;
    }
 
    // Create a hash object.
    if(!CryptCreateHash(
         m_hCryptProv,
         CALG_MD5,
         0,
         0,
         &m_hHash))
    {
      ODA_TRACE0("Error during CryptCreateHash!");
      return false;
    }

    // Hash in the password data. 
    if(!CryptHashData(
         m_hHash,
         (BYTE*)pSecurityParams->password.asArrayPtr(), 
         pSecurityParams->password.size(),
         0))
    {
      ODA_TRACE0("Error during CryptHashData!");
      return false;
    }

    // Derive a session key from the hash object. 
    DWORD dwFlags = pSecurityParams->nKeyLength << 16;
    dwFlags |= CRYPT_NO_SALT;
    if(!CryptDeriveKey(
         m_hCryptProv,
         pSecurityParams->nAlgId,// CALG_RC4
         m_hHash,
         dwFlags,
         &m_hKey))
    {
      ODA_TRACE0("Error during CryptDeriveKey!\n");
      return false;
    }

    return true;
  }

  bool decryptData(OdBinaryData& buffer, const OdSecurityParams* pSecurityParams)
  {
    if (!prepareKey(pSecurityParams))
    {
      return false;
    }

    // Decrypt data. 
    DWORD dwSize = buffer.size();
    if(!CryptDecrypt(
         m_hKey,
         0,
         TRUE,
         0,
         buffer.asArrayPtr(),
         &dwSize))
    {
      ODA_TRACE0("Error during CryptDecrypt!");
      return false;
    }

    return true;
  }

  bool encryptData(OdBinaryData& buffer, const OdSecurityParams* pSecurityParams)
  {
    if (!prepareKey(pSecurityParams))
    {
      return false;
    }

    // Decrypt data. 
    DWORD dwSize = buffer.size();
    if(!CryptEncrypt(
         m_hKey,
         0,
         TRUE,
         0,
         buffer.asArrayPtr(),
         &dwSize,
         dwSize))
    {
      ODA_TRACE0("Error during CryptDecrypt!");
      return false;
    }

    return true;
  }
};

bool COdaMfcAppApp::decryptData(OdBinaryData& buffer, const OdSecurityParams* pSecurityParams)
{
  OdWinDecrypt dc;
  return dc.decryptData(buffer, pSecurityParams);
}

bool COdaMfcAppApp::encryptData(OdBinaryData& buffer, const OdSecurityParams* pSecurityParams)
{
  OdWinDecrypt dc;
  return dc.encryptData(buffer, pSecurityParams);
}

#endif // (_WIN32_WINNT < 0x0400)


// TODO: to be removed
void ToWideCharTemp(WCHAR* pTo, LPCSTR pFrom )
{
   int i;
   for(i=0;i < lstrlen( pFrom ); i++)  pTo[i] = pFrom[i];
}

bool COdaMfcAppApp::getPassword(const OdString& dwgName, bool /*isXref*/, OdBinaryData& password)
{
  CPasswordDlg pwdDlg;
  pwdDlg.m_FileName = dwgName.c_str();
  if (pwdDlg.DoModal()==IDOK && !pwdDlg.m_Password.IsEmpty())
  {
    pwdDlg.m_Password.MakeUpper();
    int sz = pwdDlg.m_Password.GetLength();
    password.resize(sz);
    ToWideCharTemp((wchar_t*)password.asArrayPtr(), (LPCTSTR)pwdDlg.m_Password);
    //if (mbtowc((wchar_t*)password.asArrayPtr(), (LPCTSTR)pwdDlg.m_Password, sz) == sz)
    return true;
  }
  return false;
}
