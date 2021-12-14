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
// OdaDgnApp.h : main header file for the OdaDgnApp application
//
#pragma once

#ifndef __AFXWIN_H__
  #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "DgDatabase.h"

/************************************************************************/
/* Required ODA Drawings API for DGN header files                                      */
/************************************************************************/
#include "ExSystemServices.h"

#ifdef TDRAWING_ENABLED
#include "ExHostAppServices.h"
#endif

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a



class COdaDgnAppUserBreak
{
};

// COdaDgnApp:
// See OdaDgnApp.cpp for the implementation of this class
//

class COdaDgnAppSettings
{
public:
  COdaDgnAppSettings()
    : m_background(0)
    , m_numGSMenuItems(0)
    , m_gsWinDirectX(false)
    , m_gsWinGDI(false)
    , m_gsWinOpenGL(false)
  {}

  void load(CWinApp* pApp)
  {
    m_background          = pApp->GetProfileInt   (_T("format"), _T("Background colour"),            0);
    m_vectorizerPath     = pApp->GetProfileString(_T("options"), _T("recent TXV"), _T(""));
    if (m_vectorizerPath.isEmpty())
    {
      m_vectorizerPath = OdWinGDIModuleName;
    }

    if ( m_vectorizerPath.iCompare( OdWinGDIModuleName ) == 0)
    {
      m_activeGS = ID_VECTORIZE_WINGDI;
    }
    else if ( m_vectorizerPath.iCompare( OdWinDirectXModuleName ) == 0)
    {
      m_activeGS = ID_VECTORIZE_WINDIRECTX;
    }
    else if ( m_vectorizerPath.iCompare( OdWinOpenGLModuleName ) == 0)
    {
      m_activeGS = ID_VECTORIZE_WINOPENGL;
    }

    //m_sRecentCmd          = GetProfileString(_T("options"), _T("Recent Command"),              _T(""));
    //int nFillTtf          = GetProfileInt   (_T("options"), _T("Fill TTF text"),               1);
  }

  void save(CWinApp* pApp)
  {
    pApp->WriteProfileInt(_T("format"), _T("Background colour"), m_background);
    pApp->WriteProfileString(_T("options"), _T("recent TXV"), m_vectorizerPath);
  }

  OdString getGsDevice() const
  { 
    OdString sVectorizer;

    if (getActiveGS() == ID_VECTORIZE_WINGDI)
    {
      sVectorizer = OdWinGDIModuleName;
    }
    else if (getActiveGS() == ID_VECTORIZE_WINDIRECTX)
    {
      sVectorizer = OdWinDirectXModuleName;
    }
    else if (getActiveGS() == ID_VECTORIZE_WINOPENGL)
    {
      sVectorizer = OdWinOpenGLModuleName;
    }

    return sVectorizer;
  }
protected:
  void setGsDevice(const OdString& vectorizer)
  { 
    m_vectorizerPath = vectorizer;
  }
public:
  const ODCOLORREF  getActiveBackground() const { return m_background; }
  void setActiveBackground(const ODCOLORREF &color) { m_background = color&0xffffff;}

  DWORD getNumGSMenuItems() { return m_numGSMenuItems; }
  void setNumGSMenuItems(DWORD val) { m_numGSMenuItems = val; }

  UINT getActiveGS() const
  { 
    return m_activeGS;
  }
  void setActiveGS(UINT activeGS)
  { 
    m_activeGS = activeGS;
    setGsDevice( getGsDevice() );
  }

  bool getWinDirectXFlag()
  {
    return m_gsWinDirectX;
  }
  void setWinDirectXFlag(bool flag)
  {
    m_gsWinDirectX = flag;
  }
  bool getWinGDIFlag()
  {
    return m_gsWinGDI;
  }
  void setWinGDIFlag(bool flag)
  {
    m_gsWinGDI = flag;
  }
  bool getWinOpenGLFlag()
  {
    return m_gsWinOpenGL;
  }
  void setWinOpenGLFlag(bool flag)
  {
    m_gsWinOpenGL = flag;
  }

private:
  OdString   m_vectorizerPath;
  ODCOLORREF m_background;
  DWORD      m_numGSMenuItems;

  UINT  m_activeGS;
  bool m_gsWinDirectX;
  bool m_gsWinGDI;
  bool m_gsWinOpenGL;
};


class COdaDgnApp : public CWinApp
                 , public OdRxObjectImpl<OdExDgnSystemServices>
                 , public OdRxObjectImpl<OdExDgnHostAppServices>
{
protected:
  using CWinApp::operator new;
  using CWinApp::operator delete;
public:
  COdaDgnApp();

  OdDgDatabasePtr openFile(LPCTSTR lpszPathName);

  COdaDgnAppSettings& settings() { return m_settings; }
  const COdaDgnAppSettings& settings() const { return m_settings; }

  OdGsMarker getGSMenuItemMarker() const { return (OdGsMarker)this; }

  // OdRxSystemServices overridden
  virtual void warning(const OdString& message);

  virtual OdDbBaseHostAppServices* getDbHostAppServices() const
  {
    return m_pDbHostApp;
  }
  void setDbHostAppServices(OdDbBaseHostAppServices* pHostApp)
  {
    m_pDbHostApp = pHostApp;
  }

private:
  CString BrowseWithPreview(HWND hWndParent, LPCTSTR caFilter);

  //OdDgDumper m_dumper;

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
  void reportError(HWND hParent, LPCTSTR szContextMsg, const OdError& e)
  {
    messageBox(hParent, szContextMsg, (LPCTSTR)e.description(), MB_OK|MB_ICONERROR);
  }
  void reportError(LPCTSTR szContextMsg, const OdError& e)
  {
    messageBox(szContextMsg, (LPCTSTR)e.description(), MB_OK|MB_ICONERROR);
  }
  //void reportError(LPCTSTR szContextMsg, unsigned int eCode)
  //{
  //  messageBox(szContextMsg, (LPCTSTR)getErrorDescription(eCode), MB_OK|MB_ICONERROR);
  //}

  const ODCOLORREF* curPalette() const;

  void setStatusText(LPCTSTR msg);
  void setStatusText(int nCol, LPCTSTR msg);

private:
  COdaDgnAppSettings m_settings;
  bool               m_bLoading;

  OdDbBaseHostAppServices* m_pDbHostApp;

// Overrides
public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();

  //{{AFX_MSG(COdaDgnApp)
  afx_msg void OnAppAbout();
  afx_msg void OnFileOpen();
  afx_msg void OnToolsSetRegVarsMsSymbRsrc();
  afx_msg void OnToolsSetRegVarsMsRsrc();
  afx_msg void OnToolsSetRegVarsFastFont();

  afx_msg void OnUpdateWinDirectX(CCmdUI* pCmdUI);
  afx_msg void OnUpdateWinGDI(CCmdUI* pCmdUI);
  afx_msg void OnUpdateWinOpenGL(CCmdUI* pCmdUI);

  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

#ifdef TDRAWING_ENABLED
/************************************************************************/
/* Define a Custom Db HostAppServices class                            */
/************************************************************************/
class MyDbHostAppServices : public ExHostAppServices
{
#ifdef _MSC_VER
public:
  /**********************************************************************/
  /* Define a console interface for password support.                   */
  /**********************************************************************/
  bool getPassword(const OdString& sName, bool /*isXref*/, OdPassword& password)
  {
    odPrintConsoleString(L"Enter password to open drawing: %ls\n", sName.c_str());
    wchar_t pwdBuff[_MAX_PATH] = {0};
    STD(wcin).get(pwdBuff, L'\n').get();
    ::CharUpperW(pwdBuff);
    password = pwdBuff;
    return !password.isEmpty();
  }
#endif

public:
  virtual OdDbBaseHostAppServices* getDgnHostAppServices() const
  {
    return m_pDgnHostApp;
  }

public:
  MyDbHostAppServices()
    : m_pDgnHostApp(0)
  {}

  void setDgnHostAppServices(OdDbBaseHostAppServices* pHostApp)
  {
    m_pDgnHostApp = pHostApp;
  }

private:
  OdDbBaseHostAppServices* m_pDgnHostApp;
};

#endif

extern COdaDgnApp theApp;
