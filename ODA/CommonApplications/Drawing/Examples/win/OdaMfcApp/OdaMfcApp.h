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

/************************************************************************/
/* Main header file for the ODAMFCAPP application                       */
/************************************************************************/
#if !defined(AFX_ODAMFCAPP_H__17232610_A99F_4E15_9A3A_18BE1F249877__INCLUDED_)
#define AFX_ODAMFCAPP_H__17232610_A99F_4E15_9A3A_18BE1F249877__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef _USE_CRYPTLIB
#ifndef _WIN32_WINNT	
#define _WIN32_WINNT 0x0500	// to enable Windows Cryptographic API
#endif						

#include ".\odamfcapp.h"
#endif

#ifndef APSTUDIO_INVOKED
#define DOUNDEF_APSTUDIO_INVOKED
#endif
#ifdef APSTUDIO_READONLY_SYMBOLS
#define DODEF_APSTUDIO_READONLY_SYMBOLS
#endif

#define APSTUDIO_INVOKED
#undef APSTUDIO_READONLY_SYMBOLS
#include "resource.h"       // main symbols

#ifdef DOUNDEF_APSTUDIO_INVOKED
#undef APSTUDIO_INVOKED
#undef DOUNDEF_APSTUDIO_INVOKED
#endif
#ifdef DODEF_APSTUDIO_READONLY_SYMBOLS
#define APSTUDIO_READONLY_SYMBOLS
#undef DODEF_APSTUDIO_READONLY_SYMBOLS
#endif

#include "OdToolKit.h"
#include "DbObjectId.h"
#include "RxObjectImpl.h"
#include "ExSystemServices.h"
#include "ExHostAppServices.h"
#include "DynamicLinker.h"
#include "DbObject.h"
#include "Gs/Gs.h"
#include "Ed/EdCommandStack.h"
#include "TaskBarWin7Ext.h"
#include "ThreadsCounter.h"
//#include "RegKey.h"

class OdTimeStamp;
class CAuditDlg;
class COdaMfcAppDoc;
//class COdaMfcAppApp;

#ifdef ENABLE_VBA_SUPPORT
using namespace MSAPC;
#include "evalkey.h"
#define LICENSEKEY    wszEvalLicKey
#endif // ENABLE_VBA_SUPPORT

#ifdef ODAMFC_EXPORT_SYMBOL
#include "ExDbCommandContext.h"
#include "OdaMfcExport.h"
#endif // ODAMFC_EXPORT_SYMBOL
#include "MainFrm.h"

extern COdaMfcAppApp theApp;

class UserBreak
{
};

/************************************************************************/
/* See OdaMfcApp.cpp for the implementation of this class               */
/************************************************************************/

class COdaMfcAppApp : 
#ifdef ENABLE_VBA_SUPPORT
	public CApcDualDyn<CApcApplication<COdaMfcAppApp>,Application,IOdaMfcApp,&IID_IOdaMfcApp>
#else
	public CWinApp
#endif // ENABLE_VBA_SUPPORT
  , public ExSystemServices
  , public ExHostAppServices
{
protected:
#ifdef ENABLE_VBA_SUPPORT
  using CApcDualDyn<CApcApplication<COdaMfcAppApp>,Application,IOdaMfcApp,&IID_IOdaMfcApp>::operator new;
  using CApcDualDyn<CApcApplication<COdaMfcAppApp>,Application,IOdaMfcApp,&IID_IOdaMfcApp>::operator delete;
#else
	using CWinApp::operator new;
	using CWinApp::operator delete;
#endif // ENABLE_VBA_SUPPORT
private:
  int                   m_nProgressLimit;
  int                   m_nProgressPos;
  int                   m_nPercent;
  CString               m_Msg;
  BOOL                  m_bDiscardBackFaces;
  BOOL                  m_bEnableDoubleBuffer;
  BOOL                  m_bBlocksCache;
  BOOL                  m_bGsDevMultithread;
  UINT                  m_nMtRegenThreads;
  BOOL                  m_bEnablePrintPreviewViaBitmap;
  BOOL                  m_bGDITransparency;
  BOOL                  m_bUseGsModel;
  BOOL                  m_bEnableHLR;
  BOOL                  m_bContextColors;
  BOOL                  m_bTTFPolyDraw;
  BOOL                  m_bTTFTriangleCache;
  BOOL                  m_bTTFTextOut;
  BOOL                  m_bTTFCache;
  BOOL                  m_bDynamicSubEntHlt;
  BOOL                  m_bGDIGradientsAsBitmap;
  BOOL                  m_bGDIGradientsAsPolys;
  BYTE                  m_nGDIGradientsAsPolysThreshold;
  BOOL                  m_bDisableAutoRegen;
  ODCOLORREF            m_background;
  DWORD                 m_thisThreadID;
  UINT                  m_numCustomCommands;
  DWORD                 m_numGSMenuItems;
  OdString              m_sVectorizerPath;
  OdString              m_sRecentCmd;
  bool                  m_bPartial;
  bool                  m_bRecover;
  bool                  m_bLoading;
// ODA_MT_DBIO_BEGIN
  bool                  m_bUseMTLoading;
// ODA_MT_DBIO_END
  bool                  m_bRemoteGeomViewer;
  int                   m_pagingType;
  bool                  m_bUseTempFiles;
  CStringArray          m_tempFilesList;
  bool                  m_bSupportFileSelectionViaDialog;
  bool                  m_bDwfExportModelSpaceToExtents;
#ifdef ENABLE_VBA_SUPPORT
  COdaMfcAppDoc*        m_pLastDoc;
  CComObject<Documents>* m_pDocsCollection;
  bool m_bATLInited;
  int m_nVBALanguage;
#endif // ENABLE_VBA_SUPPORT

#ifdef ODAMFC_EXPORT_SYMBOL
  void UpdateFieldDisplay();
public:
  void AddReactor( OdApplicationReactor* pReactor );
  void RemoveReactor( OdApplicationReactor* pReactor );
  std::vector< OdSmartPtr< OdApplicationReactor > > m_aAppReactors;
#endif // ODAMFC_EXPORT_SYMBOL

public:
  OdDbDatabasePtr openFile(LPCTSTR lpszPathName);
  void setPartialOption(bool val) { m_bPartial = val; }
  void setRecoverOption(bool val) { m_bRecover = val; }
// ODA_MT_DBIO_BEGIN
  void setMTLoadingOption(bool val) { m_bUseMTLoading = val; }
// ODA_MT_DBIO_END

public:
  OdGsMarker getGSMenuItemMarker() const { return (OdGsMarker)this; }
  CMenu* CommandMenu(CMenu** ppEditMenu=0);
  void RefreshCommandMenu();
  UINT numCustomCommands() const { return m_numCustomCommands; }
  static CString BrowseWithPreview(HWND hWndParent, LPCTSTR caFilter, bool bMultiple = false);

  bool printingViaBitmap() const { return m_bEnablePrintPreviewViaBitmap != 0; }
  bool gdiTransparency() const { return m_bGDITransparency != 0; }
  bool doubleBufferEnabled() const { return m_bEnableDoubleBuffer!=0; }
  bool blocksCacheEnabled() const { return m_bBlocksCache!=0; }
  bool gsDeviceMultithreadEnabled() const { return m_bGsDevMultithread!=0; }
  UINT mtRegenThreadsCount() const { return m_nMtRegenThreads; }
  bool useGsModel() const { return m_bUseGsModel!=0; }
  bool useSoftwareHLR() const { return m_bEnableHLR!=0; }
  bool enableContextualColors() const { return m_bContextColors!=0; }
  bool enableTTFPolyDraw() const { return m_bTTFPolyDraw!=0; }
  bool enableTTFTriangleCache() const { return m_bTTFTriangleCache != 0; }
  bool enableTTFTextOut() const { return m_bTTFTextOut!=0; }
  bool enableTTFCache() const { return m_bTTFCache!=0; }
  bool enableDynamicSubEntHlt() const { return m_bDynamicSubEntHlt!=0; }
  bool enableGDIGradientsAsBitmap() const { return m_bGDIGradientsAsBitmap!=0; }
  bool enableGDIGradientsAsPolys() const { return m_bGDIGradientsAsPolys!=0; }
  BYTE gdiGradientsAsPolysThreshold() const { return m_nGDIGradientsAsPolysThreshold; }
  bool disableAutoRegen() const { return m_bDisableAutoRegen!=0; }
  bool discardBackFaces() const { return m_bDiscardBackFaces!=0; }
  enum DisplayFields {
    kSchemaFields,
    kDxfFields,
    kDwgFields
  };
  int  m_displayFields;
  BOOL m_bSaveRoundTrip;
  BOOL m_bSavePreview;
  BOOL m_bSaveWithPassword;
  bool m_bDisableFieldUpdateOnOpen;

  CAuditDlg* m_pAuditDlg;
  CTaskBarWin7Ext m_tbExt;
  OdMutexPtr m_pMeterMutex;

	COdaMfcAppApp();
  OdString recentGsDevicePath() const;
  void setRecentGsDevicePath(const OdString& sVectorizerPath);

  void setStatusText(LPCTSTR msg);
  void setStatusText(int nCol, LPCTSTR msg);

  void addRef() {}
  void release() {}

  OdDbHostAppProgressMeter* newProgressMeter();
  void start(const OdString& displayString = OdString::kEmpty);
  void stop();
  void meterProgress();
  void setLimit(int max);
  void warning(const char* warnVisGroup, const OdString& sMsg);
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
  void reportError(LPCTSTR szContextMsg, unsigned int eCode)
  {
    messageBox(szContextMsg, (LPCTSTR)getErrorDescription(eCode), MB_OK|MB_ICONERROR);
  }

  OdRxClass* databaseClass() const;

  OdString findFile(const OdString& pcFilename,
										OdDbBaseDatabase* pDb = NULL,
										OdDbBaseHostAppServices::FindFileHint hint = kDefault);
  OdString getFontMapFileName() const;
  OdString getSubstituteFont(const OdString& fontName, OdFontType fontType);

  const OdString product();

  /** Description:
    Returns the path to the temporary files folder.
  */
  virtual OdString getTempPath() const;

	BOOL ProcessShellCommand(CCommandLineInfo& rCmdInfo);

  void initPlotStyleSheetEnv();
  virtual void createDocTemplate();
  virtual BOOL createMainMDIFrame(CMainFrame*& pMainFrame);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdaMfcAppApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
  virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(COdaMfcAppApp)
	afx_msg void OnAppAbout();
  afx_msg void OnViewDwg();
  afx_msg void OnUpdateViewDwg(CCmdUI* pCmdUI);
  afx_msg void OnViewDxf();
  afx_msg void OnUpdateViewDxf(CCmdUI* pCmdUI);
  afx_msg void OnViewSchema();
  afx_msg void OnUpdateViewSchema(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnOptionsSaveroundtripdata();
	afx_msg void OnUpdateOptionsSaveroundtripdata(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDoublebuffer();
	afx_msg void OnUpdateOptionsDoublebuffer(CCmdUI* pCmdUI);
	afx_msg void OnOptionsBlockscache();
	afx_msg void OnUpdateOptionsBlockscache(CCmdUI* pCmdUI);
	afx_msg void OnOptionsGsdevicemultithread();
	afx_msg void OnUpdateOptionsGsdevicemultithread(CCmdUI* pCmdUI);
	afx_msg void OnOptionsNmtregenthreads();
	afx_msg void OnUpdateOptionsNmtregenthreads(CCmdUI* pCmdUI);
	afx_msg void OnWindowRender();
	afx_msg void OnToolsLoadapplications();
	afx_msg void OnOptionsSavePreview();
	afx_msg void OnUpdateOptionsSavePreview(CCmdUI* pCmdUI);
	afx_msg void OnOptionsUseGsModel();
	afx_msg void OnUpdateOptionsUseGsModel(CCmdUI* pCmdUI);
  afx_msg void OnOptionsSaveWithPassword();
  afx_msg void OnUpdateOptionsSaveWithPassword(CCmdUI* pCmdUI);
  afx_msg void OnOptionsFillTtf();
  afx_msg void OnUpdateOptionsFillTtf(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVectorizeAddvectorizerdll(CCmdUI* pCmdUI);
	afx_msg void OnVectorizeAddVectorizerDLL();
	afx_msg void OnVectorizeClearmenu();
	afx_msg void OnUpdateVectorizeClearmenu(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVectorize(CCmdUI* pCmdUI);
	afx_msg void OnOptionsSofthlr();
	afx_msg void OnUpdateOptionsSofthlr(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDiscardbackfaces();
	afx_msg void OnUpdateOptionsDiscardbackfaces(CCmdUI* pCmdUI);
	afx_msg void OnOptionsContextualcolors();
	afx_msg void OnUpdateOptionsContextualcolors(CCmdUI* pCmdUI);
	afx_msg void OnOptionsTtfpolydrawmode();
	afx_msg void OnUpdateOptionsTtfpolydrawmode(CCmdUI* pCmdUI);
  afx_msg void OnOptionsTtfTriangleCachemode();
  afx_msg void OnUpdateOptionsTtfTriangleCachemode(CCmdUI* pCmdUI);
  afx_msg void OnOptionsTtftextoutmode();
	afx_msg void OnUpdateOptionsTtftextoutmode(CCmdUI* pCmdUI);
	afx_msg void OnOptionsTtfcache();
	afx_msg void OnUpdateOptionsTtfcache(CCmdUI* pCmdUI);
	afx_msg void OnOptionsDynamicsubenthlt();
	afx_msg void OnUpdateOptionsDynamicsubenthlt(CCmdUI* pCmdUI);
	afx_msg void OnOptionsGradientsasbitmaps();
	afx_msg void OnUpdateOptionsGradientsasbitmaps(CCmdUI* pCmdUI);
	afx_msg void OnOptionsGradientsaspolys();
	afx_msg void OnUpdateOptionsGradientsaspolys(CCmdUI* pCmdUI);
	afx_msg void OnOptionsGradientsaspolysthreshold();
	afx_msg void OnUpdateOptionsGradientsaspolysshreshold(CCmdUI* pCmdUI);
  afx_msg void OnViewDisableAutoRegen();
  afx_msg void OnUpdateViewDisableAutoRegen(CCmdUI* pCmdUI);
  afx_msg void OnOptionsPrintPreview();
  afx_msg void OnUpdateOptionsPrintPreview(CCmdUI* pCmdUI);
  afx_msg void OnOptionsGDITransparency();
  afx_msg void OnUpdateOptionsGDITransparency( CCmdUI* pCmdUI );
  afx_msg void OnOptionsUseFilePaging();
  afx_msg void OnOptionsUseFilePagingUI(CCmdUI *pCmdUI);
  afx_msg void OnOptionsUseUnloadPaging();
  afx_msg void OnOptionsUseUnloadPagingUI(CCmdUI *pCmdUI);
  afx_msg void OnOptionsUseTempUndoFiles();
  afx_msg void OnOptionsUseTempUndoFilesUI(CCmdUI *pCmdUI);
  afx_msg void OnOptionsDwfExportModelSpaceToExtents();
  afx_msg void OnUpdateOptionsDwfExportModelSpaceToExtents(CCmdUI* pCmdUI);
  afx_msg void OnRemoteGeomViewer();
  //}}AFX_MSGzz
	DECLARE_MESSAGE_MAP()

  // To control option:
  bool getSAVEROUNDTRIP() const { return (m_bSaveRoundTrip!=0); }

  void auditPrintReport(OdAuditInfo* pAuditInfo, const OdString& strLine, int nPrintDest) const;

  OdDbUndoControllerPtr newUndoController();

  virtual OdStreamBufPtr newUndoStream();

public:
  void OnOptionsRenderingdeviceVectorize();

  bool getSavePreview()
  {
    return (m_bSavePreview != 0);
  }

  bool getSaveWithPassword()
  {
    return (m_bSaveWithPassword != 0);
  }

  void setRecentCmd(const OdString& cmd);
  const OdString& getRecentCmd() { return m_sRecentCmd; }

  static inline OdString objectIdAndClassName(OdDbObjectId id)
  {
    return objectIdAndClassName(id.openObject());
  }
  static inline OdString objectIdAndClassName(const OdDbObject* pObj)
  {
    if(pObj)
    {
      return OdString().format(_T("%02I64X : <%ls>"),
        OdUInt64(pObj->objectId().getHandle()),
        pObj->isA()->name().c_str()
      );
    }
    return OdString(_T("00 : < >"));
  }

  const ODCOLORREF  activeBackground() const                      { return m_background; }
  void              setActiveBackground(const ODCOLORREF &color)  
  { m_background = color&0xffffff;}
  const ODCOLORREF* curPalette() const;


  OdGsDevicePtr     gsBitmapDevice(OdRxObject* pViewObj = NULL,
                                   OdDbBaseDatabase* pDb = NULL,
                                   OdUInt32 flags = 0);

  bool encryptData(OdBinaryData& buffer, const OdSecurityParams* pSecurityParams);
  bool decryptData(OdBinaryData& buffer, const OdSecurityParams* pSecurityParams);
  bool getPassword(const OdString& dwgName, bool isXref, OdPassword& password);

  OdDbPageControllerPtr newPageController();
  int setPagingType(int pagingType);
  int pagingType() const { return m_pagingType & 0x0f; }

  bool setUndoType(bool bUseTempFiles);
  bool undoType() const { return m_bUseTempFiles; }

  OdString fileDialog(int nFlags,
    const OdString& prompt = OdString::kEmpty, 
    const OdString& defExt = OdString::kEmpty,
    const OdString& fileName = OdString::kEmpty,
    const OdString& filter = OdString::kEmpty);
#ifdef ENABLE_VBA_SUPPORT
  HRESULT AutomationFileNew(IOdaMfcDoc** ppDoc);
  HRESULT GetCollection(IOdaMfcDocs** pColl);
  HRESULT CreateAPCHost();
  HRESULT EnsureAPCObjectsCreated();
  CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
  HRESULT AddToCollection(COdaMfcAppDoc *pDoc);
  HRESULT RemoveFromCollection(COdaMfcAppDoc *pDoc);
  afx_msg void OnVbaIde();
#endif // ENABLE_VBA_SUPPORT
  virtual BOOL PreTranslateMessage(MSG* pMsg);

  bool remoteGeomViewer() const { return m_bRemoteGeomViewer; }
  void setRemoteGeomViewer() { m_bRemoteGeomViewer=true; }

  bool supportFileSelectionViaDialog() const { return m_bSupportFileSelectionViaDialog; }
  void setSupportFileSelectionViaDialog(bool b) { m_bSupportFileSelectionViaDialog = b; }

  bool dwfExportModelSpaceToExtents() const { return m_bDwfExportModelSpaceToExtents; }

  static CString getApplicationPath();
  ~COdaMfcAppApp();
};

#ifdef ENABLE_VBA_SUPPORT
inline COdaMfcAppApp * GetApp() { return (COdaMfcAppApp*)AfxGetApp(); }

/////////////////////////////////////////////////////////////////////////////
// Application

class Application : 
	public IDispatchImpl<IOdaMfcApp, &IID_IOdaMfcApp, &LIBID_OdaMfcAppLIB>, 
	public ISupportErrorInfo,
	public CComObjectRoot,
	public CComCoClass<Application,&CLSID_Application>,
	public IProvideClassInfo2Impl<&CLSID_Application, NULL, &LIBID_OdaMfcAppLIB>,
	public IApcDualImpl<COdaMfcAppApp>
{
public:
	Application() {}
BEGIN_COM_MAP(Application)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IOdaMfcApp)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(IApcDual)
END_COM_MAP()
//DECLARE_NOT_AGGREGATABLE(Application) 
// Remove the comment from the line above if you don't want your object to 
// support aggregation. 

// This typedef makes it so that this object does not lock the module.
// We do this becuase VBA will hold a pointer to this object that will not be released
// until ApcHost.Destroy is called in the base class implementation of ExitInstance, and if
// the module is locked, ExitInstance will not be called.
typedef CComCreator< CComObjectNoLock< Application > > _CreatorClass;

DECLARE_REGISTRY(Application, _T("OdaMfc.Application.1"), _T("OdaMfc.Application"), IDS_APPLICATION_DESC, THREADFLAGS_BOTH)
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IOdaMfcApp
public:
	STDMETHOD(Quit)(/*[in]*/ MpSaveOptions);
	STDMETHOD(Activate)();
	STDMETHOD(get_VBE)(/*[out, retval]*/ IDispatch* *pVal);
	STDMETHOD(get_Parent)(/*[out, retval]*/ IOdaMfcApp* *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Documents)(/*[out, retval]*/ IOdaMfcDocs* *pVal);
	STDMETHOD(get_ActiveDocument)(/*[out, retval]*/ IOdaMfcDoc* *pVal);
	STDMETHOD(get_Application)(/*[out, retval]*/ IOdaMfcApp* *pVal);
};


  class EnumDocs;
  class Documents : 
  public IDispatchImpl<IOdaMfcDocs, &IID_IOdaMfcDocs, &LIBID_OdaMfcAppLIB>, 
    public ISupportErrorInfo,
    public CComObjectRoot,
    public CComCoClass<Documents,&CLSID_Documents>
  {
    friend class EnumDocs;
  public:
    Documents() {}
    BEGIN_COM_MAP(Documents)
      COM_INTERFACE_ENTRY(IDispatch)
      COM_INTERFACE_ENTRY(IOdaMfcDocs)
      COM_INTERFACE_ENTRY(ISupportErrorInfo)
      END_COM_MAP()
      //DECLARE_NOT_AGGREGATABLE(Documents) 
      // Remove the comment from the line above if you don't want your object to 
      // support aggregation. 
      
      DECLARE_REGISTRY(Documents, _T("OdaMfc.Documents.1"), _T("OdaMfc.Documents"), IDS_DOCUMENTS_DESC, THREADFLAGS_BOTH)
      // ISupportsErrorInfo
      STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
    
    // IOdaMfcDocs
  public:	
    STDMETHOD(Add)(/*[out retval]*/IOdaMfcDoc** ppVal);
    STDMETHOD(Open)(/*[in]*/ BSTR FileName, /*[out, retval]*/ IOdaMfcDoc** ppVal);
    STDMETHOD(Item)(/*[in]*/ VARIANT Index, /*[out, retval]*/ IOdaMfcDoc** ppVal);
    STDMETHOD(get__NewEnum)(/*[out, retval]*/ IUnknown* *pVal);
    STDMETHOD(get_Count)(/*[out, retval]*/ long *pVal);
    STDMETHOD(get_Parent)(/*[out, retval]*/ IOdaMfcApp* *pVal);
    STDMETHOD(get_Application)(/*[out, retval]*/ IOdaMfcApp* *pVal);
    
    HRESULT ClearCollection();
    HRESULT RemoveFromCollection(CString strName);
    HRESULT AddToCollection(IOdaMfcDoc* pDoc);
  private:
    
    CPtrList m_listDocs;
  };
  
  class EnumDocs : public IEnumVARIANT
  {
  public:
    EnumDocs(Documents* pDocs); //main constructor
    EnumDocs(EnumDocs* pEnum); //clone constructor
    
    //IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, LPVOID* ppvObj);
    STDMETHOD_(ULONG, AddRef)() { return ++m_cRef; }
    STDMETHOD_(ULONG, Release)() { if (--m_cRef == 0) { delete this; return 0;} return m_cRef; }
    
    //IEnumVARIANT methods
    STDMETHOD(Next)(ULONG celt, VARIANT* rgvar, ULONG* celtFetched);
    STDMETHOD(Skip)(ULONG celt);
    STDMETHOD(Reset)();
    STDMETHOD(Clone)(IEnumVARIANT** ppenum);
    
  private:
    ULONG m_cRef;
    CPtrList* m_plistDocs;
    POSITION m_posCur;
  };

  void SetErrorInfo(UINT uiStringID);
#endif // ENABLE_VBA_SUPPORT
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODAMFCAPP_H__17232610_A99F_4E15_9A3A_18BE1F249877__INCLUDED_)
