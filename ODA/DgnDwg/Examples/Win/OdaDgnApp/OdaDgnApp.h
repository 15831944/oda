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

#define APSTUDIO_INVOKED
#undef APSTUDIO_READONLY_SYMBOLS
#include "resource.h"       // main symbols

#include "ExDgnServices.h"
#include "ExDgnHostAppServices.h"
#include "DgDatabase.h"
#include "ExDgnElementDumperPE.h"

/************************************************************************/
/* Required ODA Drawings SDK header files                                      */
/************************************************************************/
#include "ExSystemServices.h"

#ifdef TDRAWING_ENABLED
#include "ExHostAppServices.h"
#endif

#include "ExPrintConsole.h"

#include "ExDgnDumperModule.h"

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
  {}

  void load(CWinApp* pApp)
  {
    //m_bDiscardBackFaces   = GetProfileInt   (_T("options"), _T("Discard Back Faces"),          1);
    //// m_bEnableDoubleBuffer = GetProfileInt   (_T("options"), _T("Enable Double Buffer"),        1);
    //m_bEnableDoubleBuffer = 1; // m_bEnableDoubleBuffer = 0 is for debug use only
    //m_bEnablePrintPreviewViaBitmap = GetProfileInt   (_T("options"), _T("Print/Preview via bitmap device"), 1);
    //m_bUseGsModel         = GetProfileInt   (_T("options"), _T("UseGsModel"),                  0);
    //m_bEnableHLR          = GetProfileInt   (_T("options"), _T("Enable Software HLR"),         0);
    //m_bSaveRoundTrip      = GetProfileInt   (_T("options"), _T("Save round trip information"), 1);
    //m_bSavePreview        = GetProfileInt   (_T("options"), _T("Save Preview"),                0);
    m_background          = pApp->GetProfileInt   (_T("format"), _T("Background colour"),            0);
    //m_bSaveWithPassword   = GetProfileInt   (_T("options"), _T("Save file with password"),      0);
    m_vectorizerPath     = pApp->GetProfileString(_T("options"), _T("recent GS"), _T(""));
    m_sRecentCmd          = pApp->GetProfileString(_T("options"), _T("Recent Command"),              _T(""));
    //int nFillTtf          = GetProfileInt   (_T("options"), _T("Fill TTF text"),               1);
  }

  void save(CWinApp* pApp)
  {
    pApp->WriteProfileInt(_T("format"), _T("Background colour"), m_background);
    pApp->WriteProfileString(_T("options"), _T("recent GS"), m_vectorizerPath);
  }

  OdString getGsDevicePath() const
  { 
    return m_vectorizerPath;
  }
  void setGsDevicePath(const OdString& vectorizerPath)
  { 
    m_vectorizerPath = vectorizerPath;
  }

  const ODCOLORREF  getActiveBackground() const { return m_background; }
  void setActiveBackground(const ODCOLORREF &color) { m_background = color&0xffffff;}

  DWORD getNumGSMenuItems() { return m_numGSMenuItems; }
  void setNumGSMenuItems(DWORD val) { m_numGSMenuItems = val; }

  void setRecentCmd(const OdString& cmd) 
  { 
    m_sRecentCmd = cmd;
    AfxGetApp()->WriteProfileString(_T("options"), _T("Recent Command"), m_sRecentCmd);
  }
  const OdString& getRecentCmd() { return m_sRecentCmd; }

private:
  OdString   m_vectorizerPath;
  ODCOLORREF m_background;
  DWORD      m_numGSMenuItems;
  OdString   m_sRecentCmd;
};


class COdaDgnApp : public CWinApp
                 , public OdRxObjectImpl<OdExDgnSystemServices>
                 , public OdRxObjectImpl<OdExDgnHostAppServices>
{
protected:
  using CWinApp::operator new;
  using CWinApp::operator delete;
#if (TD_VERSION < 31000 )
public:
  OdString            m_strPenTable;
#endif
public:
  COdaDgnApp();

  OdDgDatabasePtr openFile(LPCTSTR lpszPathName);

  COdaDgnAppSettings& settings() { return m_settings; }
  const COdaDgnAppSettings& settings() const { return m_settings; }

  OdGsMarker getGSMenuItemMarker() const { return (OdGsMarker)this; }

  bool useGsModel() const 
  { 
    return m_bUseGsModel!=0; 
  }

  bool useBlockCahce() const
  {
    return m_bBlocksCache;
  }

  bool useSingleViewMode() const
  {
	  return m_bUseSingleViewMode;
  }

  bool applyGlobalOriginFlag() const
  {
    return m_bApplyGlobalOrigin;
  }

  void setApplyGlobalOriginFlag( bool bSet )
  {
    m_bApplyGlobalOrigin = bSet;
    WriteProfileInt(_T("options"), _T("ApplyGlobalOrigin"), m_bApplyGlobalOrigin);
  }

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

  CMenu* CommandMenu(CMenu** ppEditMenu=0);
  void RefreshCommandMenu();
  UINT numCustomCommands() const { return m_numCustomCommands; }
  void setRecentCmd(const OdString& cmd);
  const OdString& getRecentCmd() { return m_settings.getRecentCmd(); }

private:
  CString BrowseWithPreview(HWND hWndParent, LPCTSTR caFilter);

  UINT                m_numCustomCommands;

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

  OdString fileDialog(int nFlags,
    const OdString& prompt = OdString::kEmpty, 
    const OdString& defExt = OdString::kEmpty,
    const OdString& fileName = OdString::kEmpty,
    const OdString& filter = OdString::kEmpty);

  OdRxClass* databaseClass() const;

  static CString getApplicationPath();

  OdDbUndoControllerPtr newUndoController();
  bool setUndoType(bool bUseTempFiles);
  bool undoType() const { return m_bUseTempFiles; }
  virtual OdStreamBufPtr newUndoStream();

private:
  COdaDgnAppSettings m_settings;
  bool               m_bLoading;
  bool               m_bUseGsModel;
  bool				       m_bUseSingleViewMode;
  bool               m_bApplyGlobalOrigin;
  bool               m_bBlocksCache;
#ifdef _DEBUG
  bool               m_bDisableAsserts;
#endif
  bool                m_bUseTempFiles;

  OdDbBaseHostAppServices* m_pDbHostApp;

// Overrides
public:
  virtual BOOL InitInstance();
  virtual int ExitInstance();

  //{{AFX_MSG(COdaDgnApp)
  afx_msg void OnAppAbout();
  afx_msg void OnFileOpen();
  afx_msg void OnToolsSolidHelper();
  afx_msg void OnToolsLoadapplications();
  afx_msg void OnToolsModelerOptions();
  afx_msg void OnVectorizeAddvectorizerdll();
  afx_msg void OnUpdateVectorizeAddvectorizerdll(CCmdUI *pCmdUI);
  afx_msg void OnOptionsUseGsModel();
  afx_msg void OnOptionsUseBlockCache();
  afx_msg void OnUpdateOptionsUseGsModel(CCmdUI* pCmdUI);  
  afx_msg void OnUpdateOptionsUseBlockCache(CCmdUI* pCmdUI);  
  afx_msg void OnOptionsMTLoading();
  afx_msg void OnUpdateOptionsMTLoading(CCmdUI* pCmdUI);  
  afx_msg void OnOptionsMTRegen();
  afx_msg void OnUpdateOptionsMTRegen(CCmdUI* pCmdUI);  
  afx_msg void OnOptionsTransformLSScale();
  afx_msg void OnUpdateOptionsTransformLSScale(CCmdUI* pCmdUI);
  afx_msg void OnOptionsActiveLevelAlwaysVisible();
  afx_msg void OnUpdateOptionsActiveLevelAlwaysVisible(CCmdUI* pCmdUI);    
  afx_msg void OnOptionsExactExtents();
  afx_msg void OnUpdateOptionsExactExtents(CCmdUI* pCmdUI);  
  afx_msg void OnOptionsProcessTagAssociation();
  afx_msg void OnUpdateOptionsProcessTagAssociation(CCmdUI* pCmdUI);
  afx_msg void OnOptionsProcessDimAssociation();
  afx_msg void OnUpdateOptionsProcessDimAssociation(CCmdUI* pCmdUI);
  afx_msg void OnOptionsProcessMLineAssociation();
  afx_msg void OnUpdateOptionsProcessMLineAssociation(CCmdUI* pCmdUI);
  void OnOptionsUseSingleViewMode();
  void OnUpdateOptionsUseSingleViewMode(CCmdUI * pCmdUI);
  afx_msg void OnOptions3DObjWireframeDrawIsolines();
  afx_msg void OnUpdateOptions3DObjWireframeDrawIsolines(CCmdUI* pCmdUI);
  afx_msg void OnOptionsUseXRefColorTable();
  afx_msg void OnUpdateOptionsUseXRefColorTable(CCmdUI* pCmdUI);
  afx_msg void OnOptionsRemapColorIndexes();
  afx_msg void OnUpdateOptionsRemapColorIndexes(CCmdUI* pCmdUI);
#ifdef _DEBUG
  afx_msg void OnDisableAsserts();
  afx_msg void OnUpdateDisableAsserts(CCmdUI* pCmdUI);
#endif
  afx_msg void OnUpdateGsDevices(CCmdUI *pCmdUI);
  afx_msg void OnVectorizeClearmenu();
  afx_msg void OnUpdateVectorizeClearmenu(CCmdUI *pCmdUI);
  afx_msg void OnUpdateVectorize(CCmdUI* pCmdUI);
  afx_msg void OnToolsSetRegVarsMsSymbRsrc();
  afx_msg void OnToolsSetRegVarsMsRsrc();
  afx_msg void OnToolsSetRegVarsFastFont();
  afx_msg void OnToolsSetRegVarsNurbs();
  afx_msg void OnToolsSetRegVarsGripPoints();
  afx_msg void OnToolsSetRegVarsMsPattern();
  afx_msg void OnToolsSetRegVarsMsBump();
  afx_msg void OnToolsSetRegVarsMsMaterial();
  afx_msg void OnToolsSetRegVarsMsMtbl();
  afx_msg void OnToolsSetRegVarsMsMatPalette();
  afx_msg void OnToolsSetRegVarsMsPenTable();
  afx_msg void OnToolsSetRegVarsMsDefCtbl();
  afx_msg void OnToolsSetRegVarsMsFontPath();
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
  bool getPassword(const OdString& dbName, bool /*isXref*/, OdPassword& password)
  {
    odPrintConsoleString(L"Enter password to open drawing: %ls\n", dbName.c_str());
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
