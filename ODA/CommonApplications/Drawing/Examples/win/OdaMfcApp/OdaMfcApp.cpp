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
/* Defines the class behaviors for the application.                     */
/************************************************************************/

#include "stdafx.h"
#include "OdaMfcApp.h"
//#include  <io.h>
#include <malloc.h>
#include <atlconv.h>

#include "MainFrm.h"
#include "ChildFrm.h"
#include "OdaMfcAppDoc.h"
#include "DwgView.h"
#include "resource.h"
#include "FileDlgExt.h"
#include "PreviewDib.h"
#include "AuditDlg.h"
#include "SummInfo.h"
#include "LoadApps.h"
#include "OdaTrianParamsDlg.h"
#include "ColorMapping.h"
#include "ExUndoController.h"
#include "diagnostics.h"
#include "ModelerGeometry/ModelerModule.h"
#include "PasswordDlg.h"
#include "Ed/EdCommandStack.h"
#include "RxModule.h"
#include "RxDynamicModule.h"
#include "ExPageController.h"
#include "Ps/PlotStyles.h"
#include "Ed/EdUserIO.h"
#include "DbLayout.h"
#include "DbLayoutPaperPE.h"
#include "Gi/GiMaterialItem.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeEllipArc3d.h"
#include "AppAuditInfo.h"
#include "DwgViewer.h"
#include "odaxwrap.h"
#include "RemoteGeomViewer.h"
#include "MaterialsEditor.h"
#include "VectProps.h"
#include "ShHistory/TD_3DSolidHistoryDef.h"
#ifndef ENABLE_VBA_SUPPORT
#include "RegKey.h"
#endif
#ifndef _TOOLKIT_IN_DLL_
#include "StaticAppSelDlg.h"
//#include "..\..\Architecture\include\AECAppStaticModules.h" // uncomment for Architecture objects support in statically linked version
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1600)
#include <Shlwapi.h>
#endif

#include "../Imports/DwfImport/Include/DwfImport.h"
#ifdef DGNIMPORT_ENABLE
#include "Imports/DgnImport/DgnImport.h"
#endif
#include <afxinet.h>

#include "MemFileStreamImpl.h"
#include "ExFileUndoController.h"
#include <Tf/TfRevisionControl.h>

#include "RcsFileServices/RxRcsFileServices.h"

#ifdef DGNIMPORT_ENABLE
using namespace TD_DGN_IMPORT;
#endif
using namespace TD_DWF_IMPORT;

#ifdef IFC2DWG_SUPPORT
#include "../Ifc/Include/Common/ModuleNames.h"
#include "Exports/Ifc2Dwg/Include/IfcExport.h"
using namespace TD_IFC_EXPORT;
#endif

#define MAX_PATH_LENGTH 1024


extern "C" _CRTIMP int __cdecl _set_sbh_threshold(size_t);

#ifdef _TOOLKIT_IN_DLL_

#if defined (PDFMODULEVI_PRODUCTION)
#undef PDFMODULEVI_PRODUCTION
#endif
//#define PDFMODULEVI_ENABLED
//#define PDFMODULEVI_PRODUCTION
#ifdef PDFMODULEVI_PRODUCTION
#pragma comment(lib,"TD_PdfKey_VI.lib")
#include "PdfKey/PdfKey.h"
#endif // PDFMODULEVI_PRODUCTION

//#define _OPENRL_KEY_ENABLED_
#ifdef _OPENRL_KEY_ENABLED_
#pragma comment(lib,"TD_OpenRLKey.lib")
//#include "../../../Extensions/ExRender/OpenRL/OpenRLKey/include/OpenRLKey.h"
void initOpenRLSupport();
void uninitOpenRLSupport();
#endif // _OPENRL_KEY_ENABLED_
#endif // _TOOLKIT_IN_DLL_

#ifdef MECHANICAL_MODE
  #include "TmModuleNames.h"
#endif // MECHANICAL_MODE

/////////////////////////////////////////////////////////////////////////////
// COdaMfcAppApp
#include <LoadStaticModules.h>

BEGIN_MESSAGE_MAP(COdaMfcAppApp, CWinApp)
  //{{AFX_MSG_MAP(COdaMfcAppApp)
  ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
#ifdef ENABLE_VBA_SUPPORT
  ON_COMMAND(ID_TOOLS_MACRO_VISUALBASICEDITOR, OnVbaIde)
#endif // ENABLE_VBA_SUPPORT
  ON_COMMAND(ID_VIEW_DWG, OnViewDwg)
  ON_UPDATE_COMMAND_UI(ID_VIEW_DWG, OnUpdateViewDwg)
  ON_COMMAND(ID_VIEW_DXF, OnViewDxf)
  ON_UPDATE_COMMAND_UI(ID_VIEW_DXF, OnUpdateViewDxf)
  ON_COMMAND(ID_VIEW_SCHEMA, OnViewSchema)
  ON_UPDATE_COMMAND_UI(ID_VIEW_SCHEMA, OnUpdateViewSchema)
  ON_COMMAND(ID_OPTIONS_SAVEROUNDTRIPDATA, OnOptionsSaveroundtripdata)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_SAVEROUNDTRIPDATA, OnUpdateOptionsSaveroundtripdata)
  ON_COMMAND(ID_OPTIONS_DOUBLEBUFFER, OnOptionsDoublebuffer)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DOUBLEBUFFER, OnUpdateOptionsDoublebuffer)
  ON_COMMAND(ID_OPTIONS_BLOCKSCACHE, OnOptionsBlockscache)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_BLOCKSCACHE, OnUpdateOptionsBlockscache)
  ON_COMMAND(ID_OPTIONS_GSDEVMT, OnOptionsGsdevicemultithread)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_GSDEVMT, OnUpdateOptionsGsdevicemultithread)
  ON_COMMAND(ID_OPTIONS_NUMREGENTHREADS, OnOptionsNmtregenthreads)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_NUMREGENTHREADS, OnUpdateOptionsNmtregenthreads)
  ON_COMMAND(ID_OPTIONS_PRINTPREVIEW, OnOptionsPrintPreview)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_PRINTPREVIEW, OnUpdateOptionsPrintPreview)
  ON_COMMAND( ID_OPTIONS_GDITRANSPARENCY, OnOptionsGDITransparency )
  ON_UPDATE_COMMAND_UI( ID_OPTIONS_GDITRANSPARENCY, OnUpdateOptionsGDITransparency )
  ON_COMMAND(ID_TOOLS_LOADAPPLICATIONS, OnToolsLoadapplications)
  ON_COMMAND(ID_OPTIONS_SAVEPREVIEW, OnOptionsSavePreview)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_SAVEPREVIEW, OnUpdateOptionsSavePreview)
  ON_COMMAND(ID_OPTIONS_USEGSMODELGEOMETRYCACHE, OnOptionsUseGsModel)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_USEGSMODELGEOMETRYCACHE, OnUpdateOptionsUseGsModel)
  ON_UPDATE_COMMAND_UI(ID_VECTORIZE_ADDVECTORIZERDLL, OnUpdateVectorizeAddvectorizerdll)
  ON_COMMAND(ID_VECTORIZE_ADDVECTORIZERDLL, OnVectorizeAddVectorizerDLL)
  ON_COMMAND(ID_VECTORIZE_CLEARMENU, OnVectorizeClearmenu)
  ON_UPDATE_COMMAND_UI(ID_VECTORIZE_CLEARMENU, OnUpdateVectorizeClearmenu)
  ON_UPDATE_COMMAND_UI(ID_VECTORIZE, OnUpdateVectorize)
  ON_COMMAND(ID_OPTIONS_SOFTHLR, OnOptionsSofthlr)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_SOFTHLR, OnUpdateOptionsSofthlr)
  ON_COMMAND(ID_OPTIONS_DISCARDBACKFACES, OnOptionsDiscardbackfaces)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DISCARDBACKFACES, OnUpdateOptionsDiscardbackfaces)
  ON_COMMAND(ID_OPTIONS_CONTEXTCOLORS, OnOptionsContextualcolors)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_CONTEXTCOLORS, OnUpdateOptionsContextualcolors)
  ON_COMMAND(ID_OPTIONS_TTFPOLYDRAW, OnOptionsTtfpolydrawmode)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_TTFPOLYDRAW, OnUpdateOptionsTtfpolydrawmode)
  ON_COMMAND(ID_OPTIONS_TTFTRIANGLECACHE, OnOptionsTtfTriangleCachemode)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_TTFTRIANGLECACHE, OnUpdateOptionsTtfTriangleCachemode)
  ON_COMMAND(ID_OPTIONS_TTFTEXTOUT, OnOptionsTtftextoutmode)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_TTFTEXTOUT, OnUpdateOptionsTtftextoutmode)
  ON_COMMAND(ID_OPTIONS_TTFCACHE, OnOptionsTtfcache)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_TTFCACHE, OnUpdateOptionsTtfcache)
  ON_COMMAND(ID_OPTIONS_DYNAMICSUBENTHLT, OnOptionsDynamicsubenthlt)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DYNAMICSUBENTHLT, OnUpdateOptionsDynamicsubenthlt)
  ON_COMMAND(ID_OPTIONS_GRADASBITMAP, OnOptionsGradientsasbitmaps)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_GRADASBITMAP, OnUpdateOptionsGradientsasbitmaps)
  ON_COMMAND(ID_OPTIONS_GRADASPOLYS, OnOptionsGradientsaspolys)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_GRADASPOLYS, OnUpdateOptionsGradientsaspolys)
  ON_COMMAND(ID_OPTIONS_GRADASPOLYSTHRESHOLD, OnOptionsGradientsaspolysthreshold)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_GRADASPOLYSTHRESHOLD, OnUpdateOptionsGradientsaspolysshreshold)
  ON_COMMAND(ID_VIEW_DISABLEAUTOREGEN, OnViewDisableAutoRegen)
  ON_UPDATE_COMMAND_UI(ID_VIEW_DISABLEAUTOREGEN, OnUpdateViewDisableAutoRegen)
  //}}AFX_MSG_MAP
  // Standard file based document commands
  ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
  ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
  // Standard print setup command
  ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
  ON_COMMAND(ID_OPTIONS_SAVEWITHPASSWORD, OnOptionsSaveWithPassword)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_SAVEWITHPASSWORD, OnUpdateOptionsSaveWithPassword)
  ON_COMMAND(ID_OPTIONS_FILL_TTF, OnOptionsFillTtf)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_FILL_TTF, OnUpdateOptionsFillTtf)
  ON_COMMAND(ID_DEBUG_GEOMETRY, OnRemoteGeomViewer)
  // Paging support options
  ON_COMMAND(ID_OPTIONS_USEFILEPAGING, OnOptionsUseFilePaging)                        // ET Oct 16, 2012
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_USEFILEPAGING, OnOptionsUseFilePagingUI)            
  ON_COMMAND(ID_OPTIONS_USEUNLOADPAGING, OnOptionsUseUnloadPaging)                    
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_USEUNLOADPAGING, OnOptionsUseUnloadPagingUI)        
  ON_COMMAND(ID_OPTIONS_USETEMPUNDOFILES, OnOptionsUseTempUndoFiles)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_USETEMPUNDOFILES, OnOptionsUseTempUndoFilesUI)
  ON_COMMAND(ID_OPTIONS_DWF_EXPORT_MODEL_EXT, OnOptionsDwfExportModelSpaceToExtents)
  ON_UPDATE_COMMAND_UI(ID_OPTIONS_DWF_EXPORT_MODEL_EXT, OnUpdateOptionsDwfExportModelSpaceToExtents)
END_MESSAGE_MAP()

COdaMfcAppApp::~COdaMfcAppApp()
{
  for (INT_PTR i = 0; i < m_tempFilesList.GetCount(); ++i)
  {
    ::DeleteFile(m_tempFilesList[i]);
  }
}

const ODCOLORREF* COdaMfcAppApp::curPalette() const
{
  return odcmAcadPalette(m_background);
}

/************************************************************************/
/* Specialization of CCommandLineInfo to add the following switches:    */
/*                                                                      */
/*              /s<filename>   SaveAs                                   */
/*              /exit          Exit                                     */
/************************************************************************/
class CFullCommandLineInfo : public CCommandLineInfo
{
public:
  CString m_SaveName;
  BOOL    m_Exit;
  CStringArray m_AppsToLoad;
  CStringArray m_CommandsToExecute;
  CString      m_ScriptToExecute;
  CString      m_BatToExecute;

  CFullCommandLineInfo()
    : CCommandLineInfo()
    , m_Exit(0)
  {
  }

  virtual void ParseParam (LPCTSTR lpszParam,BOOL bFlag,BOOL bLast )
  {
    BOOL is = FALSE;
    if(bFlag)
    {
      if (!_tcsnicmp(lpszParam, _T("bat:"), 4))
      {
        m_BatToExecute = &lpszParam[4];
        is = TRUE;
      }
      else if(!_tcsnicmp(lpszParam, _T("ld:"), 3))
      {
        m_AppsToLoad.Add(&lpszParam[3]);
        is = TRUE;
      }
      else if(!_tcsnicmp(lpszParam, _T("scr:"), 4))
      {
        m_ScriptToExecute = &lpszParam[4];
        is = TRUE;
      }
      else if(!_tcsnicmp(lpszParam, _T("ex:"), 3))
      {
        m_CommandsToExecute.Add(&lpszParam[3]);
        is = TRUE;
      }
      else if (!_tcsnicmp(lpszParam, _T("s:"), 2))
      {
        m_SaveName = &lpszParam[2];
        is = TRUE;
      }
      else if(!_tcsicmp(lpszParam, _T("exit")))
      {
        m_Exit = true;
        is = TRUE;
      }
    }
    if (!is || bLast)
      CCommandLineInfo::ParseParam(lpszParam,bFlag,bLast);
  }
};

/************************************************************************/
/* Dispatch commands specified on the command line                      */
/************************************************************************/
BOOL COdaMfcAppApp::ProcessShellCommand(CCommandLineInfo& rCmdInfo)
{
  CFullCommandLineInfo& rCmdInfo2 = (CFullCommandLineInfo&)rCmdInfo;

  if (!rCmdInfo2.m_BatToExecute.IsEmpty())
  {
    _tsystem((LPCTSTR)rCmdInfo2.m_BatToExecute);
  }

  for (int idx = 0; idx < rCmdInfo2.m_AppsToLoad.GetCount(); ++idx)
  {
    odrxDynamicLinker()->loadModule(OdString((LPCTSTR)rCmdInfo2.m_AppsToLoad.GetAt(idx)), false);
  }

  COdaMfcAppDoc* tmpDoc = NULL;
  if (rCmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
  {
    tmpDoc = (COdaMfcAppDoc*)OpenDocumentFile(rCmdInfo.m_strFileName);
    if (!tmpDoc)
      return FALSE;
    if (!rCmdInfo2.m_ScriptToExecute.IsEmpty())
    {
      CStdioFile scrFile(rCmdInfo2.m_ScriptToExecute, CFile::modeRead);
      CString strCmd;
      while(scrFile.ReadString(strCmd))
      {
        if (!strCmd.IsEmpty() && strCmd[0] != _T('#'))
        tmpDoc->ExecuteCommand(OdString((LPCTSTR)strCmd));
      }
    }
    for (int idx = 0; idx < rCmdInfo2.m_CommandsToExecute.GetCount(); ++idx)
    {
      tmpDoc->ExecuteCommand(OdString((LPCTSTR)rCmdInfo2.m_CommandsToExecute.GetAt(idx)));
    }

  } else
    CWinApp::ProcessShellCommand(rCmdInfo);

  if (!rCmdInfo2.m_SaveName.IsEmpty())
  {
    if (!tmpDoc->OnSaveDocument(rCmdInfo2.m_SaveName))
      return FALSE;
  }
  if (rCmdInfo2.m_Exit)
  {
    PostQuitMessage(0);
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// COdaMfcAppApp construction

COdaMfcAppApp::COdaMfcAppApp()
  : m_nProgressPos(0)
  , m_nProgressLimit(100)
  , m_pAuditDlg(NULL)
  , m_bUseGsModel(TRUE)
  , m_numGSMenuItems(0)
  , m_bDiscardBackFaces(1)
  , m_bEnableHLR(0)
  , m_bContextColors(1)
  , m_bTTFPolyDraw(0)
  , m_bTTFTriangleCache(0)
  , m_bTTFTextOut(0)
  , m_bTTFCache(0)
  , m_bDynamicSubEntHlt(0)
  , m_bGDIGradientsAsBitmap(0)
  , m_bGDIGradientsAsPolys(0)
  , m_nGDIGradientsAsPolysThreshold(10)
  , m_bDisableAutoRegen(0)
  , m_bLoading(false)
  , m_bRemoteGeomViewer(false)
  , m_bSupportFileSelectionViaDialog(true)
  , m_bDwfExportModelSpaceToExtents(false)
#ifdef ENABLE_VBA_SUPPORT
  , m_pLastDoc(0)
  , m_pDocsCollection(0)
  , m_bATLInited(false)
#endif // ENABLE_VBA_SUPPORT
// ODA_MT_DBIO_BEGIN
  , m_bUseMTLoading(false)
// ODA_MT_DBIO_END
  , m_bUseTempFiles(false)
  , m_bDisableFieldUpdateOnOpen(false)
#ifdef ENABLE_PAGING_SUPPORT
  , m_pagingType(OdDb::kPage)
#else
  , m_pagingType(0)
#endif //ENABLE_PAGING_SUPPORT
{
}

void COdaMfcAppApp::setStatusText(LPCTSTR msg)
{
  ((CMainFrame*)GetMainWnd())->setStatusText(msg);
}

void COdaMfcAppApp::setStatusText(int nCol, LPCTSTR msg)
{
  ((CMainFrame*)GetMainWnd())->setStatusText(nCol, msg);
}


/////////////////////////////////////////////////////////////////////////////
// The one and only COdaMfcAppApp object

COdaMfcAppApp theApp;


/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
OdDbHostAppProgressMeter* COdaMfcAppApp::newProgressMeter()
{
  if(m_thisThreadID != ::GetCurrentThreadId()) // disable access from other threads
  {
    return 0;
  }
  return ExHostAppServices::newProgressMeter();
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void COdaMfcAppApp::start(const OdString& displayString)
{
  m_Msg = (LPCTSTR)displayString;
  m_nProgressPos = 0;
  m_nPercent = -1;
  m_tbExt.SetProgressState(::AfxGetMainWnd()->GetSafeHwnd(), CTaskBarWin7Ext::PS_Normal);
  m_tbExt.SetProgressValue(::AfxGetMainWnd()->GetSafeHwnd(), 0, 100);
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void COdaMfcAppApp::stop()
{
  m_nProgressPos = m_nProgressLimit;
  meterProgress();
  m_tbExt.SetProgressState(::AfxGetMainWnd()->GetSafeHwnd(), CTaskBarWin7Ext::PS_NoProgress);
  //m_tbExt.FlashWindow(::AfxGetMainWnd()->GetSafeHwnd());
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void COdaMfcAppApp::meterProgress()
{
  bool bUpdateProgress;
  int nPercent;
  { TD_AUTOLOCK_P_DEF(m_pMeterMutex);
    int nOldPercent = m_nPercent;
    nPercent = m_nPercent = int( ( double(m_nProgressPos++) / double(m_nProgressLimit)) * 100 );
    bUpdateProgress = (nOldPercent!=m_nPercent);
  }
  if (bUpdateProgress) {
    struct StatUpdater {
      int m_nPercent;
      CMainFrame *pWnd;
      COdaMfcAppApp *pThis;
      StatUpdater(int nPercent, CMainFrame *_pWnd, COdaMfcAppApp *_pThis)
        : m_nPercent(nPercent), pWnd(_pWnd), pThis(_pThis) { }
      static void Exec(void *pArg) {
        StatUpdater *pExec = reinterpret_cast<StatUpdater*>(pArg);
        CString str;
        str.Format(_T("%s %d"), pExec->pThis->m_Msg, pExec->m_nPercent);
        pExec->pWnd->m_wndStatusBar.SetPaneText(0, str);
        pExec->pThis->m_tbExt.SetProgressValue(::AfxGetMainWnd()->GetSafeHwnd(), (ULONG)pExec->m_nPercent, 100);
        MSG msg;
        while(::PeekMessage(&msg, pExec->pWnd->m_hWnd, WM_KEYUP, WM_KEYUP, 1))
        {
          bool bDup = false;
          if(msg.wParam == VK_ESCAPE && !bDup)
          {
            bDup = true;
            str.Format(_T("Are you sure you want to terminate\n%s ?"), pExec->pThis->m_Msg);
            pExec->pThis->m_tbExt.SetProgressState(::AfxGetMainWnd()->GetSafeHwnd(), CTaskBarWin7Ext::PS_Paused);
            if(::AfxMessageBox(str, MB_YESNO|MB_ICONQUESTION)==IDYES)
            {
              pExec->pThis->m_tbExt.SetProgressState(::AfxGetMainWnd()->GetSafeHwnd(), CTaskBarWin7Ext::PS_NoProgress);
              throw UserBreak();
            }
            else
              pExec->pThis->m_tbExt.SetProgressState(::AfxGetMainWnd()->GetSafeHwnd(), CTaskBarWin7Ext::PS_Normal);
          }
        }
      }
    } execArg(nPercent, (CMainFrame*)GetMainWnd(), this);
    odExecuteMainThreadAction(StatUpdater::Exec, &execArg);
  }
}

/************************************************************************/
/* Progress Meter override                                              */
/************************************************************************/
void COdaMfcAppApp::setLimit(int max)
{
  m_nProgressLimit = max ? max : 1;
}
/************************************************************************/
/* Returns the static OdRxClass description object associated with the  */
/* database                                                             */
/************************************************************************/
OdRxClass* COdaMfcAppApp::databaseClass() const
{
  return OdDbDatabaseDoc::desc();
}

/************************************************************************/
/* Simple trace function                                                */
/************************************************************************/
static void myTrace(const OdChar* debugString)
{
#ifndef NDEBUG
  OutputDebugStringW((LPCWSTR)debugString);
#endif
}

/************************************************************************/
/* Displays MessageBox for Assertion failure                            */
/************************************************************************/
static void myAssert(const char* expr, const char* fileName, int nLine)
{
  USES_CONVERSION;
  char sMsg[MAX_PATH*3];
  sprintf(sMsg, "expression: \"%s\"\n\nfile: \"%s\"\nline: %d",
    expr, fileName, nLine);

  switch(MessageBox(NULL, A2CT(sMsg), _T("Assertion failed!"), MB_ICONWARNING|MB_ABORTRETRYIGNORE))
  {
  case IDABORT:
    exit(0);
    break;
  case IDRETRY:
    DebugBreak();
    break;
  case IDIGNORE:
  default:
    break;
  }
  }

static bool myCheckAssertGroup(const char* pGroup)
{
  return !pGroup || !*pGroup; // asserts of base group are enabled only (default)
  //return true; // asserts of all groups are enabled
}

// Define module map for statically linked modules:


/************************************************************************/
/* Add paper-drawing protocol extension                                 */
/************************************************************************/
static void addPaperDrawingCustomization()
{
  static class OdDbLayoutPaperPEImpl : public OdStaticRxObject<OdDbLayoutPaperPE>
  {
  public:
    virtual bool drawPaper( const OdDbLayout* , OdGiWorldDraw* pWd, OdGePoint3d* points )
    {
      pWd->geometry().polygon(4, points);
      return true;
    }
    virtual bool drawBorder( const OdDbLayout* , OdGiWorldDraw* pWd, OdGePoint3d* points )
    {
      pWd->geometry().polygon(4, points);
      return true;
    }
    virtual bool drawMargins( const OdDbLayout* , OdGiWorldDraw* pWd, OdGePoint3d* points )
    {
      if(points[0]==points[1] || points[1]==points[2])
        return true;

      int nDashes = 15, i;
      OdGiGeometry& geom = pWd->geometry();
      OdGePoint3d dash1[2];
      OdGePoint3d dash2[2];
      OdGeVector3d step = (points[1] - points[0]) / (nDashes * 2 + 1);
      dash1[0] = points[0];
      dash2[0] = points[2];
      for(i=0; i<=nDashes; ++i)
      {
        dash1[1] = dash1[0] + step;
        geom.polyline(2, dash1);
        dash1[0] = dash1[1] + step;
        dash2[1] = dash2[0] - step;
        geom.polyline(2, dash2);
        dash2[0] = dash2[1] - step;
      }
      nDashes = int((points[2] - points[1]).length() / step.length() - 1) / 2;
      step = (points[2] - points[1]) / (nDashes * 2 + 1);
      dash1[0] = points[1];
      dash2[0] = points[3];
      for(i=0; i<=nDashes; ++i)
      {
        dash1[1] = dash1[0] + step;
        geom.polyline(2, dash1);
        dash1[0] = dash1[1] + step;
        dash2[1] = dash2[0] - step;
        geom.polyline(2, dash2);
        dash2[0] = dash2[1] - step;
      }
      return true;
    }
  }
  s_PaperDrawExt;

  OdDbLayout::desc()->addX(OdDbLayoutPaperPE::desc(), &s_PaperDrawExt);
}

/************************************************************************/
/* Remove paper-drawing protocol extension                                 */
/************************************************************************/
static void removePaperDrawingCustomization()
{
  OdDbLayout::desc()->delX( OdDbLayoutPaperPE::desc() );
}

/************************************************************************/
/* Add material textures loading monitor protocol extension             */
/************************************************************************/
static void addMaterialTextureLoadingMonitor()
{
  static class OdGiMaterialTextureLoadPEImpl : public OdStaticRxObject<OdGiMaterialTextureLoadPE>
  {
  public:
    void startTextureLoading(OdString &/*fileName*/, OdDbBaseDatabase* /*pDb*/)
    {
      // Material texture to be loaded. We could correct loading path here.
    }

    void textureLoaded(const OdString &fileName, OdDbBaseDatabase* /*pDb*/)
    {
      ODA_TRACE1("Material texture loaded: %s\n", fileName.c_str());
    }
    /** Description:
      Called by texture loader after file loading, only if texture loading failed.
    */
    void textureLoadingFailed(const OdString &fileName, OdDbBaseDatabase* /*pDb*/)
    {
      ODA_TRACE1("Failed to load material texture: %s\n", fileName.c_str());
    }
  }
  s_MatLoadExt;

  OdGiMaterialTextureEntry::desc()->addX(OdGiMaterialTextureLoadPE::desc(), &s_MatLoadExt);
}

/************************************************************************/
/* Remove material texture loading monitor protocol extension           */
/************************************************************************/
void removeMaterialTextureLoadingMonitor()
{
  OdGiMaterialTextureEntry::desc()->delX(OdGiMaterialTextureLoadPE::desc());
}

void rxInit_COleClientItem_handler();

///////////////////////////////////////////////////////////////////////////
// COdaMfcAppApp initialization

#define AppRegistryKey _T("Open Design Alliance")

OdStaticRxObject<Cmd_VIEW> g_Cmd_VIEW;
OdStaticRxObject<Cmd_SELECT> g_Cmd_SELECT;
OdStaticRxObject<Cmd_DISPLAY_DIFFS> g_Cmd_DISPLAY_DIFFS;
OdStaticRxObject<Cmd_SAVE> g_Cmd_SAVE;
OdStaticRxObject<Cmd_SAVEAS> g_Cmd_SAVEAS;

#if (_MSC_VER > 1200) && (_MSC_VER < 1400)
//////////////////////////////////////////////////////////////////////////////////////
// This code block is copied from MFC library, to fix memory overrun error, 
// present in MFC7.0 and 7.1 versions.
//
UINT AFXAPI AfxGetFileName(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);
UINT AFXAPI AfxGetFileTitle(LPCTSTR lpszPathName, LPTSTR lpszTitle, UINT nMax);
// this function is not exported from MFC and copied here to avoid reinventing the wheel
static void _AfxAbbreviateName(LPTSTR lpszCanon, int cchMax, BOOL bAtLeastName)
{
  int cchFullPath, cchFileName, cchVolName;
  const TCHAR* lpszCur;
  const TCHAR* lpszBase;
  const TCHAR* lpszFileName;

  lpszBase = lpszCanon;
  cchFullPath = lstrlen(lpszCanon);

  cchFileName = AfxGetFileName(lpszCanon, NULL, 0) - 1;
  lpszFileName = lpszBase + (cchFullPath-cchFileName);

  // If cchMax is more than enough to hold the full path name, we're done.
  // This is probably a pretty common case, so we'll put it first.
  if (cchMax >= cchFullPath)
    return;

  // If cchMax isn't enough to hold at least the basename, we're done
  if (cchMax < cchFileName)
  {
    lstrcpy(lpszCanon, (bAtLeastName) ? lpszFileName : _T(""));
    return;
  }

  // Calculate the length of the volume name.  Normally, this is two characters
  // (e.g., "C:", "D:", etc.), but for a UNC name, it could be more (e.g.,
  // "\\server\share").
  //
  // If cchMax isn't enough to hold at least <volume_name>\...\<base_name>, the
  // result is the base filename.

  lpszCur = lpszBase + 2;                 // Skip "C:" or leading "\\"

  if (lpszBase[0] == '\\' && lpszBase[1] == '\\') // UNC pathname
  {
    // First skip to the '\' between the server name and the share name,
    while (*lpszCur != '\\')
    {
      lpszCur = _tcsinc(lpszCur);
      ASSERT(*lpszCur != '\0');
    }
  }
  // if a UNC get the share name, if a drive get at least one directory
  ASSERT(*lpszCur == '\\');
  // make sure there is another directory, not just c:\filename.ext
  if (cchFullPath - cchFileName > 3)
  {
    lpszCur = _tcsinc(lpszCur);
    while (*lpszCur != '\\')
    {
      lpszCur = _tcsinc(lpszCur);
      ASSERT(*lpszCur != '\0');
    }
  }
  ASSERT(*lpszCur == '\\');

  cchVolName = int(lpszCur - lpszBase);
  if (cchMax < cchVolName + 5 + cchFileName)
  {
    lstrcpy(lpszCanon, lpszFileName);
    return;
  }

  // Now loop through the remaining directory components until something
  // of the form <volume_name>\...\<one_or_more_dirs>\<base_name> fits.
  //
  // Assert that the whole filename doesn't fit -- this should have been
  // handled earlier.

  ASSERT(cchVolName + (int)lstrlen(lpszCur) > cchMax);
  while (cchVolName + 4 + (int)lstrlen(lpszCur) > cchMax)
  {
    do
    {
      lpszCur = _tcsinc(lpszCur);
      ASSERT(*lpszCur != '\0');
    }
    while (*lpszCur != '\\');
  }

  // Form the resultant string and we're done.
  lpszCanon[cchVolName] = '\0';
  lstrcat(lpszCanon, _T("\\..."));
  lstrcat(lpszCanon, lpszCur);
}
class CDDRecentFileList : public CRecentFileList
{
public:
  CDDRecentFileList(UINT nStart, LPCTSTR lpszSection,
    LPCTSTR lpszEntryFormat, int nSize,
    int nMaxDispLen = AFX_ABBREV_FILENAME_LEN):CRecentFileList(nStart,lpszSection,lpszEntryFormat,nSize,nMaxDispLen){}
  virtual BOOL GetDisplayName(CString& strName, int nIndex,
    LPCTSTR lpszCurDir, int nCurDir, BOOL bAtLeastName = TRUE) const
  {
    ASSERT(lpszCurDir == NULL || AfxIsValidString(lpszCurDir, nCurDir));

    ASSERT(m_arrNames != NULL);
    ASSERT(nIndex < m_nSize);
    if (m_arrNames[nIndex].IsEmpty())
      return FALSE;

    int nLenName = m_arrNames[nIndex].GetLength();
    LPTSTR lpch = strName.GetBuffer(_MAX_PATH+1);
    if (lpch == NULL)
    {
      AfxThrowMemoryException();
    }
    lstrcpyn(lpch, m_arrNames[nIndex],nLenName + 1 );
    // nLenDir is the length of the directory part of the full path
    int nLenDir = nLenName - (AfxGetFileName(lpch, NULL, 0) - 1);
    BOOL bSameDir = FALSE;
    if (nLenDir == nCurDir)
    {
      TCHAR chSave = lpch[nLenDir];
      lpch[nCurDir] = 0;  // terminate at same location as current dir
      bSameDir = lstrcmpi(lpszCurDir, lpch) == 0;
      lpch[nLenDir] = chSave;
    }
    // copy the full path, otherwise abbreviate the name
    if (bSameDir)
    {
      // copy file name only since directories are same
      TCHAR szTemp[_MAX_PATH];
      AfxGetFileTitle(lpch+nCurDir, szTemp, _MAX_PATH);
      lstrcpyn(lpch, szTemp, _MAX_PATH);
    }
    else if (m_nMaxDisplayLength != -1)
    {
      // strip the extension if the system calls for it
      TCHAR szTemp[_MAX_PATH];
      AfxGetFileTitle(lpch+nLenDir, szTemp, _MAX_PATH);
      lstrcpyn(lpch+nLenDir, szTemp, _MAX_PATH-nLenDir);

      // abbreviate name based on what will fit in limited space
      _AfxAbbreviateName(lpch, m_nMaxDisplayLength, bAtLeastName);
    }
    strName.ReleaseBuffer();
    return TRUE;
  }
};
#endif

#ifdef ENABLE_VBA_SUPPORT
BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_Application, Application)
OBJECT_ENTRY(CLSID_Documents, Documents)
OBJECT_ENTRY(CLSID_Document, Document)
END_OBJECT_MAP()
#endif // ENABLE_VBA_SUPPORT

#if !defined(_TOOLKIT_IN_DLL_) && defined(_NATIVE_WCHAR_T_DEFINED)
namespace std
{
#pragma warning(push)
#pragma warning(disable:4231)
  extern template allocator<unsigned short>::allocator(void);
  extern template allocator<unsigned short>::allocator(class std::allocator<unsigned short> const &);
  extern template unsigned int allocator<unsigned short>::max_size(void)const;
  extern template unsigned short * __thiscall allocator<unsigned short>::allocate(unsigned int);
  extern template void __thiscall allocator<unsigned short>::deallocate(unsigned short *,unsigned int);
#pragma warning(pop)
}
static std::vector<unsigned short> shorttest;
#endif    

void rxInitExPointCloud();
void rxUninitExPointCloud();

BOOL COdaMfcAppApp::InitInstance()
{
#if _MSC_VER < 1600 // removed in VC 2010
  _set_sbh_threshold(1016);
#endif

//  setlocale( LC_NUMERIC, "C" ); // saving doubles to dxf
//  setlocale( LC_TIME, "" ); // correct strftime formatting

// Client application can set the locale
  setlocale( LC_ALL, "" );

#ifdef ENABLE_VBA_SUPPORT
  if (!ApcOleInit())
  {
    //AfxMessageBox(IDP_OLE_INIT_FAILED);
    return FALSE;
  }
  m_bATLInited = TRUE;
  
  _Module.Init(ObjectMap, AfxGetInstanceHandle());
  _Module.dwThreadID = GetCurrentThreadId();
#else
  AfxOleInit();
#endif // ENABLE_VBA_SUPPORT
  m_tbExt.StartManagement();
  
  // Change the registry key under which our settings are stored.
  SetRegistryKey(AppRegistryKey);


#if (_MSC_VER > 1200) && (_MSC_VER < 1400)
  LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)
  BOOL bNoRecentDocs = FALSE;
  GetSysPolicyValue(_AFX_SYSPOLICY_NORECENTDOCHISTORY, &bNoRecentDocs);
  if (!bNoRecentDocs )
  {
    m_pRecentFileList = new CDDRecentFileList(0, _T("Recent File List"), _T("File%d"), 10);
    m_pRecentFileList->ReadList();
  }
#else
  LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)
#endif

  m_bDiscardBackFaces   = GetProfileInt   (_T("options"), _T("Discard Back Faces"),          1);
  // m_bEnableDoubleBuffer = GetProfileInt   (_T("options"), _T("Enable Double Buffer"),        1);
  m_bEnableDoubleBuffer = 1; // m_bEnableDoubleBuffer = 0 is for debug use only
  m_bBlocksCache        = GetProfileInt   (_T("options"), _T("Enable Blocks Cache"),         0); // 1
  m_bGsDevMultithread   = GetProfileInt   (_T("options"), _T("Gs Device Multithread"),       0);
  m_nMtRegenThreads     = GetProfileInt   (_T("options"), _T("Mt Regen Threads Count"),      4);
  m_bEnablePrintPreviewViaBitmap = GetProfileInt   (_T("options"), _T("Print/Preview via bitmap device"), 1);
  m_bGDITransparency    = GetProfileInt   ( _T( "options" ), _T( "GDI Plot With Transparency" ), 0 );
  m_bUseGsModel         = GetProfileInt   (_T("options"), _T("UseGsModel"),                  TRUE);
  m_bEnableHLR          = GetProfileInt   (_T("options"), _T("Enable Software HLR"),         0);
  m_bContextColors      = GetProfileInt   (_T("options"), _T("Contextual Colors"),           1);
  m_bTTFPolyDraw        = GetProfileInt   (_T("options"), _T("TTF PolyDraw"),                0);
  m_bTTFTriangleCache   = GetProfileInt   (_T("options"), _T("TTF TriangleCache"),           0);
  m_bTTFTextOut         = GetProfileInt   (_T("options"), _T("TTF TextOut"),                 0);
  m_bTTFCache           = GetProfileInt   (_T("options"), _T("TTF Cache"),                   0);
  m_bDynamicSubEntHlt   = GetProfileInt   (_T("options"), _T("Dynamic Subentities Highlight"), 0);
  m_bGDIGradientsAsBitmap = GetProfileInt   (_T("options"), _T("GDI Gradients as Bitmaps"),  1);
  m_bGDIGradientsAsPolys = GetProfileInt   (_T("options"), _T("GDI Gradients as Polys"),     0);
  m_nGDIGradientsAsPolysThreshold = (BYTE)GetProfileInt(_T("options"), _T("GDI Gradients as Polys Threshold"), 10);
  m_bDisableAutoRegen   = GetProfileInt   (_T("options"), _T("Disable Auto-Regen"),          0);
  m_displayFields       = GetProfileInt   (_T("options"), _T("Field display format"),        0);
  m_bSaveRoundTrip      = GetProfileInt   (_T("options"), _T("Save round trip information"), 1);
  m_bSavePreview        = GetProfileInt   (_T("options"), _T("Save Preview"),                0);
  m_background          = GetProfileInt   (_T("format"), _T("Background colour"),            0);
  m_bSaveWithPassword   = GetProfileInt   (_T("options"), _T("Save DWG with password"),      0);
  m_sVectorizerPath     = GetProfileString(_T("options"), _T("recent GS"),                   _T(""));
  m_sRecentCmd          = GetProfileString(_T("options"), _T("Recent Command"),              _T(""));
  int nFillTtf          = GetProfileInt   (_T("options"), _T("Fill TTF text"),               1);
  setTEXTFILL(nFillTtf != 0);
  m_bDwfExportModelSpaceToExtents = GetProfileInt(L"options", L"Dwf Export Model Space to Extents", 0);

  // Customize diagnostics:
  ::odSetTraceFunc(myTrace);
  ::odSetAssertFunc(myAssert);
  ::odSetCheckAssertGroupFunc(myCheckAssertGroup);
  try
  {
#ifdef _DEBUG

#if 0  //!defined(PDFMODULEVI_ENABLED) // PDF conflicts with...
    // This will cause OdaMfcApp to crash if it will perform illegal FP operation
    _clear87();
    _control87( //              off on
                //_EM_INVALID       |
                _EM_DENORMAL      |
                //_EM_ZERODIVIDE    |
                //_EM_OVERFLOW      |
                _EM_UNDERFLOW     |
                _EM_INEXACT       ,
                _MCW_EM
              );
#endif
#endif
#ifndef _TOOLKIT_IN_DLL_
    ODRX_INIT_STATIC_MODULE_MAP();
#endif
    // Initialize ODA Drawings
    odInitialize(this);
#ifdef _DEBUG
    // Be sure multiple initialization/deinitialization is not broken
    odUninitialize();
    odInitialize(this);
#endif

    CLoadApps::rxInit();

#ifdef PDFMODULEVI_PRODUCTION
    initPdfSupport();
#endif

#ifdef PDFMODULEVI_ENABLED
    //::odrxDynamicLinker()->loadModule(OdPdfModuleVIModuleName); do not load PdfUnderlay module w/o check if there already Pdfium or PdfVI module loaded
#endif // _PDF_VI_ENABLED_

#ifdef _OPENRL_KEY_ENABLED_
    initOpenRLSupport();
#endif // _OPENRL_KEY_ENABLED_

#ifdef ODAMFC_EXPORT
    OdApplicationReactor::rxInit();
    OdApDocument::rxInit();
#endif // ODAMFC_EXPORT

    // Sample grip/snap points implementation
    ::odrxDynamicLinker()->loadModule(OdGripPointsModuleName);
    // DimAssoc Support
    ::odrxDynamicLinker()->loadModule(L"ExDimAssoc");
    // Additional commands support (ERASE,EXPLODE,PURGE, etc.)
    ::odrxDynamicLinker()->loadModule(OdDbCommandsModuleName);
    // Sample custom objects + debug commands
    ::odrxDynamicLinker()->loadModule(OdExCustObjsModuleName);
    ::odrxDynamicLinker()->loadModule(OdExCommandsModuleName);
    ::odrxDynamicLinker()->loadModule(OdRasterExportModuleName);
    ::odrxDynamicLinker()->loadModule(OdExDeepCloningModuleName);
    // Core dynamic blocks support
    ::odrxDynamicLinker()->loadModule(OdDynBlocksModuleName);
    // Sample commands illustrating dynamic blocks usage
    ::odrxDynamicLinker()->loadModule(OdExDynamicBlocksModuleName);
    // Support of old style materials, lights, etc. (AVE_*)
    ::odrxDynamicLinker()->loadModule(OdAveModuleName);
    // Field evaluator for "AcVar", and "AcDiesel" fields
    ::odrxDynamicLinker()->loadModule(OdExFieldEvaluatorModuleName);
    // custom OdDbPlotSettingsValidator implementation
    ::odrxDynamicLinker()->loadModule(OdPlotSettingsValidatorModuleName);
    // OdAsSurfBody object support module
    ::odrxDynamicLinker()->loadModule(OdAutoSurfServicesModuleName);
    // ODA Platform format import/export module
    ::odrxDynamicLinker()->loadModule(L"TD_Tf");
    // ODA Drawings constraints module
    ::odrxDynamicLinker()->loadModule(L"DbConstraints");

    ::odrxSysRegistry()->putAt(L"ODDB_LAYOUT_MANAGER", layoutManager()); // for AcModelDocObj 
    ::odrxDynamicLinker()->loadModule(L"AcModelDocObj"); // uses layout manager from <Rx System Registry>/ODDB_LAYOUT_MANAGER

    ::odrxDynamicLinker()->loadModule(L"ExampleRevisionControl");
    ::odrxDynamicLinker()->loadModule(SynergyObjDPWModuleName);

    // Load additional modules defined in file OdaMfcApp.plugins
    OdString sFileName = theApp.getApplicationPath() + OD_T("\\OdaMfcApp.plugins");
    if (odrxSystemServices()->accessFile(sFileName, Oda::kFileRead))
    {
      CStdioFile file;
      if (file.Open(sFileName, CFile::modeRead | CFile::typeText))
      {
        OdString sModuleName;

        CString str;
        while (file.ReadString(str))
        {
          str.Trim();
          if (!str.IsEmpty() && str.GetAt(0) != L'#')
          {
            sModuleName.format(OD_T("%s"), str.GetBuffer());
            ::odrxDynamicLinker()->loadModule(sModuleName);
          }
        }
      }
    }

    //::odrxDynamicLinker()->loadModule(OdExPdfPublishCmdsModuleName);
    //::odrxDynamicLinker()->loadModule(L"OdPrcPublish3dCmds");
    //::odrxDynamicLinker()->loadModule(L"OdPrc2PdfModule");
		//::odrxDynamicLinker()->loadModule(OdSpaModelerModuleName, false);
#ifdef ODAMFC_EXPORT
    ::odrxDynamicLinker()->loadModule(L"TD_MgdMfc.dll");
#endif // ODAMFC_EXPORT
    // Free type support - replaces built-in Windows TTF handling
#ifdef ODA_COLLECT_TTF_FONTS_IN_DRAWING_FOLDER
    ::odrxDynamicLinker()->loadModule(OdFTFontEngineModuleName);
#endif    
    // ::odrxDynamicLinker()->loadModule(OdFTFontEngineModuleName);

    OdRxSystemServices* pSs = odrxSystemServices();
    if( pSs->accessFile(L"c:\\bigfont.ini", Oda::kFileRead))
    {
    //  OdCharMapper::addBigFontWithIndex(L"DAYUXP.SHX", 2);
       OdCharMapper::addBigFonts(pSs->createFile(L"c:\\bigfont.ini"));
    }
    //OdCharMapper::addBigFontWithIndex(L"HZTXT.SHX", 5);

    //::odrxDynamicLinker()->loadModule(DbConstraintsModuleName);

    //::odrxDynamicLinker()->loadModule(L"TATestTx");

//     OdTtfDescriptor description;
//     OdString fNAme(OD_T("C:\\Windows\\Fonts\\MyriadPro-Regular.otf"));
//     ttfFileNameByDescriptor(description, fNAme);
    addPaperDrawingCustomization();
    addMaterialTextureLoadingMonitor();

    ::odrxDynamicLinker()->loadModule(DbCryptModuleName);

#ifdef MECHANICAL_MODE
    LoadTeighaMechanicalModules(false);
#endif // MECHANICAL_MODE

    OdDbDatabaseDoc::rxInit();

    ::rxInit_COleClientItem_handler();

    OdEdCommandStackPtr pCommands = odedRegCmds();
    pCommands->addCommand(&g_Cmd_VIEW);
    pCommands->addCommand(&g_Cmd_SELECT);
    pCommands->addCommand(&g_Cmd_DISPLAY_DIFFS);
    pCommands->addCommand(&g_Cmd_SAVE);
    pCommands->addCommand(&g_Cmd_SAVEAS);
    rxInitMaterialsEditorObjects();
    rxInitExPointCloud();
  }
  catch (OdError& err)
  {
    reportError(_T("Teigha Initialization error"), err);
    ExitProcess(-1);
  }
#ifndef _DEBUG
  catch (...)
  {
    reportError(_T("Teigha Initialization error"), eNotHandled);
    ExitProcess(-1);
  }
#endif

  CMainFrame* pMainFrame;
  createDocTemplate();
  if (!createMainMDIFrame(pMainFrame))
    return FALSE;

#ifndef ODA_FILE_FIXER
#ifdef ENABLE_VBA_SUPPORT
  {
    OdString strModuleName;
    CComPtr<ITypeLib> spTLib;
    HRESULT hr = E_FAIL;
    if(GetModuleFileNameW(NULL, strModuleName.getBuffer(_MAX_PATH), _MAX_PATH))
    {
      strModuleName.releaseBuffer();
      if(SUCCEEDED(hr = LoadTypeLib(::SysAllocString(strModuleName.c_str()), &spTLib)))
        hr = RegisterTypeLib(spTLib, ::SysAllocString(strModuleName.c_str()), NULL);
    }
  }
  
  if(FAILED(CComObject<Documents>::CreateInstance(&m_pDocsCollection)))
    return FALSE;
  // hold onto a reference
  m_pDocsCollection->AddRef();
#endif // ENABLE_VBA_SUPPORT

  // Parse command line for standard shell commands, DDE, file open
  CFullCommandLineInfo cmdInfo;
  ParseCommandLine(cmdInfo);
#ifdef ENABLE_VBA_SUPPORT
  // should parse command line here for automation vbalang parameter
  m_nVBALanguage = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), SORT_DEFAULT);
#endif // ENABLE_VBA_SUPPORT
  if ( cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew ) 
    cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

  // Dispatch commands specified on the command line
  if (!ProcessShellCommand(cmdInfo))
    return FALSE;
#endif

  WINDOWPLACEMENT wp;
  BOOL bRes = pMainFrame->GetWindowPlacement(&wp);

  wp.flags                  = GetProfileInt(_T("MainFrame"), _T("flags"),   wp.flags                  );
  wp.showCmd                = GetProfileInt(_T("MainFrame"), _T("showCmd"), wp.showCmd                );
  wp.rcNormalPosition.left  = GetProfileInt(_T("MainFrame"), _T("left"),    wp.rcNormalPosition.left  );
  wp.rcNormalPosition.top   = GetProfileInt(_T("MainFrame"), _T("top"),     wp.rcNormalPosition.top   );
  wp.rcNormalPosition.bottom= GetProfileInt(_T("MainFrame"), _T("bottom"),  wp.rcNormalPosition.bottom);
  wp.rcNormalPosition.right = GetProfileInt(_T("MainFrame"), _T("right"),   wp.rcNormalPosition.right );

  wp.ptMinPosition.x        = GetProfileInt(_T("MainFrame"), _T("Min.x"),   wp.ptMinPosition.x        );
  wp.ptMinPosition.y        = GetProfileInt(_T("MainFrame"), _T("Min.y"),   wp.ptMinPosition.y        );

  wp.ptMaxPosition.x        = GetProfileInt(_T("MainFrame"), _T("Max.x"),   wp.ptMaxPosition.x        );
  wp.ptMaxPosition.y        = GetProfileInt(_T("MainFrame"), _T("Max.y"),   wp.ptMaxPosition.y        );

  m_thisThreadID = ::GetCurrentThreadId();
  bRes = pMainFrame->SetWindowPlacement(&wp);
  pMainFrame->UpdateWindow();

  initPlotStyleSheetEnv();

  return TRUE;
}

const int nCommandMenuIndex = 8;

CMenu* COdaMfcAppApp::CommandMenu(CMenu** ppEditMenu)
{
  // Locate the edit submenu
  CMenu* pEditMenu = NULL;
  CMenu* pTopMenu = GetMainWnd()->GetMenu();
  int n = pTopMenu->GetMenuItemCount();
  for (int i = 0; i < n; ++i)
  {
    CMenu* pMenu = pTopMenu->GetSubMenu(i);
    if (pMenu && pMenu->GetMenuItemID(0) == ID_EDIT_UNDO)
    {
      pEditMenu = pMenu;
      break;
    }
  }
  ASSERT(pEditMenu != NULL);
  if(ppEditMenu)
  {
    *ppEditMenu = pEditMenu;
  }

  CMenu* pCommandMenu = pEditMenu->GetSubMenu(nCommandMenuIndex);

  ASSERT(pCommandMenu != NULL);

  return pCommandMenu;
}

void COdaMfcAppApp::RefreshCommandMenu()
{
  MENUITEMINFO menuItemInfo;
  menuItemInfo.cbSize = sizeof(menuItemInfo);
  menuItemInfo.fMask = MIIM_DATA;

  CMenu* pEditMenu = NULL;
  CMenu* pCommandMenu = CommandMenu(&pEditMenu);

  int i;
  for(i = pCommandMenu->GetMenuItemCount()-1; i >= 0; i--)
  {
    CMenu* pSubMenu = pCommandMenu->GetSubMenu(i);
    if(pSubMenu)
    {
      pSubMenu->DestroyMenu();
    }
    pCommandMenu->DeleteMenu(i, MF_BYPOSITION);
  }

  // Then add an item for each available command
  OdEdCommandStackPtr pCommandStack = ::odedRegCmds();
  bool bHasNoCommand = pCommandStack->newIterator()->done();
  pEditMenu->EnableMenuItem(nCommandMenuIndex, MF_BYPOSITION|(bHasNoCommand ? MF_GRAYED : MF_ENABLED));
  i = _APS_NEXT_COMMAND_VALUE + 100; // leave 100 ids for scales
  if(!bHasNoCommand)
  {
    OdRxIteratorPtr pGroups = pCommandStack->newGroupIterator();
    while(!pGroups->done())
    {
      OdRxDictionaryPtr pGroup = pGroups->object();
      CMenu groupMenu;
      groupMenu.CreateMenu();
      OdRxIteratorPtr pCommandIter = pGroup->newIterator(OdRx::kDictSorted);
      OdString groupName;
      while(!pCommandIter->done())
      {
        OdEdCommandPtr pCmd = pCommandIter->object().get();
        if (groupName.isEmpty())
        {
          groupName = pCmd->groupName();
        }
        groupMenu.AppendMenu(MF_STRING, i, OdString(pCmd->globalName()));
        menuItemInfo.dwItemData = (LPARAM)pCmd.get();
        VERIFY(::SetMenuItemInfo(groupMenu.m_hMenu, i, FALSE, &menuItemInfo));

        pCommandIter->next();
        ++i;
      }
      pCommandMenu->AppendMenu(MF_STRING|MF_POPUP, (LPARAM)groupMenu.Detach(), groupName);
      pGroups->next();
      groupName.empty();
    }
  }
  m_numCustomCommands = i - _APS_NEXT_COMMAND_VALUE - 100;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
  CAboutDlg();
  
  // Dialog Data
  //{{AFX_DATA(CAboutDlg)
  enum { IDD = IDD_ABOUTBOX };
  //}}AFX_DATA
  
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CAboutDlg)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  virtual BOOL OnInitDialog();
  //}}AFX_VIRTUAL
  
  // Implementation
protected:
  //{{AFX_MSG(CAboutDlg)
  // No message handlers
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  //{{AFX_DATA_INIT(CAboutDlg)
  //}}AFX_DATA_INIT
}

BOOL CAboutDlg::OnInitDialog()
{
  OdDbLibraryInfo*  libInfo = oddbGetLibraryInfo();
//  OdString  sCompany = libInfo->getCompanyName();
  OdString  sLib = libInfo->getBuildComments();
  OdString  sCopyright = libInfo->getCopyright();

  CDialog::OnInitDialog();

  SetDlgItemText(IDC_INF_COPYRIGHT, sCopyright);
//  SetDlgItemText(IDC_INF_COMPANY, sCompany.c_str());
  SetDlgItemText(IDC_INF_LIBRARY, sLib);

  return TRUE;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
        CDialog::DoDataExchange(pDX);
        //{{AFX_DATA_MAP(CAboutDlg)
        //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
        //{{AFX_MSG_MAP(CAboutDlg)
                // No message handlers
        //}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void COdaMfcAppApp::OnAppAbout()
{
  CAboutDlg aboutDlg;
  aboutDlg.DoModal();
}

void COdaMfcAppApp::OnFileOpen()
{
  CString filter = _T("DWG files (*.dwg)|*.dwg|DXF files (*.dxf)|*.dxf|Drawings Stream Format (*.dsf)|*.dsf|");
  if (!::odrxDynamicLinker()->loadApp(OdDgnImportModuleName, true).isNull())
    filter += _T("DGN files (*.dgn)|*.dgn|");
#ifdef IFC2DWG_SUPPORT
  if (!::odrxDynamicLinker()->loadApp(OdIfc2DwgModuleName, true).isNull())
    filter += _T("IFC files (*.ifc)|*.ifc|");
#endif
  if (!::odrxDynamicLinker()->loadApp(OdDwf7ImportModuleName, true).isNull())
    filter += _T("DWF files (*.dwf)|*.dwf|XPS files (*.dwfx)|*.dwfx|");
  filter += _T("All Files (*.*)|*.*||");
  CString sPath = BrowseWithPreview(GetMainWnd()->GetSafeHwnd(), filter, true);

  if (!sPath.IsEmpty())
  {
    int cnt = 0;
    int pos = sPath.Find(L'|');
    while (pos > 0)
    {
      CString fName = sPath.Mid(0, pos);
      sPath.Delete(0, fName.GetLength() +1);
      if (OpenDocumentFile(fName))
        ++cnt;
      pos = sPath.Find(L'|');
    }
    if (OpenDocumentFile(sPath))
      ++cnt;

    if (cnt > 1 && GetMainWnd())
    {
      CMDIFrameWnd *pWnd = dynamic_cast<CMDIFrameWnd*>(GetMainWnd());
      if (pWnd)
        pWnd->MDITile(1);
    }
  }
}

#pragma warning(disable:4510 4610)
#pragma warning(push)
void COdaMfcAppApp::OnRemoteGeomViewer() 
{
  m_bRemoteGeomViewer = true;
  struct CDocTemplateEx : CDocTemplate {
    void SetViewToCreate(CRuntimeClass* pViewClass){m_pViewClass = pViewClass;}
  };
  POSITION p = GetFirstDocTemplatePosition();
  CDocTemplateEx* pTemplate = (CDocTemplateEx*)GetNextDocTemplate(p);
  ASSERT_VALID(pTemplate);

  pTemplate->SetViewToCreate(RUNTIME_CLASS(CRemoteGeomViewer));
  CWinApp::OnFileNew();
  pTemplate->SetViewToCreate(RUNTIME_CLASS(CDwgView));
}
#pragma warning(pop)

void COdaMfcAppApp::OnOptionsUseFilePaging()
{
  if (!(pagingType() & OdDb::kPage))
    setPagingType(OdDb::kPage);
  else
    setPagingType(0);
    
}

void COdaMfcAppApp::OnOptionsUseFilePagingUI(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(TRUE);
  pCmdUI->SetCheck(pagingType() & OdDb::kPage ? TRUE : FALSE);
}

void COdaMfcAppApp::OnOptionsUseUnloadPaging()
{
  if (!(pagingType() & OdDb::kUnload)) 
    setPagingType(OdDb::kUnload);
  else
    setPagingType(0);
}

void COdaMfcAppApp::OnOptionsUseUnloadPagingUI(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(TRUE);
  pCmdUI->SetCheck(pagingType() == OdDb::kUnload ? TRUE : FALSE);
}

void COdaMfcAppApp::OnOptionsUseTempUndoFiles()
{
  setUndoType(!undoType());
}

void COdaMfcAppApp::OnOptionsUseTempUndoFilesUI(CCmdUI *pCmdUI)
{
  pCmdUI->Enable(TRUE);
  pCmdUI->SetCheck(undoType() ? TRUE : FALSE);
}

void COdaMfcAppApp::OnOptionsDwfExportModelSpaceToExtents()
{
  m_bDwfExportModelSpaceToExtents = !m_bDwfExportModelSpaceToExtents;
  WriteProfileInt(L"options", L"Dwf Export Model To Extents", m_bDwfExportModelSpaceToExtents);
}

void COdaMfcAppApp::OnUpdateOptionsDwfExportModelSpaceToExtents(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bDwfExportModelSpaceToExtents);
}

#if !defined(TD_DLL_VERSION_SUFFIX_STR)
#define TD_DLL_VERSION_SUFFIX_STR L""
#endif

/*
*/
CString COdaMfcAppApp::BrowseWithPreview(HWND hWndParent, LPCTSTR caFilter, bool bMultiple)
{
  CString sFileBuff;
  HINSTANCE hinstLib = LoadLibrary(OdString(_T("FileDlgExt")) 
#ifndef _UNICODE
    + "Ansi"
#endif
    + OdString(TD_DLL_VERSION_SUFFIX_STR) + OdString(_T(".dll")));
  if (NULL != hinstLib)
  {
    ODA_OPEN_DLGPROC fpDlgProc = (ODA_OPEN_DLGPROC)GetProcAddress(hinstLib, "CreateOpenWithPreviewDlg");
    if (NULL != fpDlgProc)
    {
      CPreviewDIB statDib;
      OpenWithPreviewDlg *pDlg;
      (fpDlgProc)(&statDib, hWndParent, NULL, caFilter,
                              OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST
                              | (bMultiple ? OFN_ALLOWMULTISELECT : 0),
                              &pDlg);
      if (IDOK == pDlg->ShowModal())
      {
        if (!bMultiple)
        {
          long nSize = _MAX_PATH;
          pDlg->GetFullFileName(sFileBuff.GetBuffer(nSize), nSize);
          sFileBuff.ReleaseBuffer();
        }
        else
        {
          long nSize = 0;
          if (pDlg->GetAllSelectedFileNames(NULL, nSize))
          {
            pDlg->GetAllSelectedFileNames(sFileBuff.GetBuffer(nSize), nSize);
            sFileBuff.ReleaseBuffer();
          }
        }
      }
      pDlg->ReleaseDlg();
    }
    FreeLibrary(hinstLib);
  }
  else
  {    // use alternate method for OpenFile dialog
    OPENFILENAME  ofn;

    // replace the '|' character with '\0'
    CString filter;
    long l = (long)_tcslen(caFilter), l2 = l;
    LPTSTR buff = filter.GetBuffer(l);
    while (--l >= 0)
    {
      if (caFilter[l] == '|')
        buff[l] = '\0';
      else
        buff[l] = caFilter[l];
    }
    filter.ReleaseBuffer(l2);

    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWndParent;
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = sFileBuff.GetBuffer(_MAX_PATH);
    ofn.nMaxFile = _MAX_PATH;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST;

    GetOpenFileName(&ofn);
    sFileBuff.ReleaseBuffer();
  }
  return sFileBuff;
}

void rxUninit_COleClientItem_handler();

int COdaMfcAppApp::ExitInstance()
{
#ifdef ODAMFC_EXPORT
    OdApplicationReactor::rxUninit();
    OdApDocument::rxUninit();
#endif // ODAMFC_EXPORT

    CLoadApps::rxUninit();

  try
  {
    OdEdCommandStackPtr pCommands = odedRegCmds();

    rxUninitExPointCloud();
    rxUninitMaterialsEditorObjects();
    pCommands->removeCmd(&g_Cmd_DISPLAY_DIFFS);
    pCommands->removeCmd(&g_Cmd_SELECT);
    pCommands->removeCmd(&g_Cmd_VIEW);
    pCommands->removeCmd(&g_Cmd_SAVE);
    pCommands->removeCmd(&g_Cmd_SAVEAS);
    OdDbDatabaseDoc::rxUninit();

    ::rxUninit_COleClientItem_handler();
    
    removePaperDrawingCustomization();
    removeMaterialTextureLoadingMonitor();

#ifdef _OPENRL_KEY_ENABLED_
    uninitOpenRLSupport();
#endif // _OPENRL_KEY_ENABLED_

#ifdef PDFMODULEVI_PRODUCTION
    uninitPdfSupport();
#endif

    ::odUninitialize();
  }
  catch(const OdError& e)
  {
    theApp.reportError(_T(""), e);
  }
  m_tbExt.EndManagement();
#ifdef ENABLE_VBA_SUPPORT
  int res = CApcApplication<COdaMfcAppApp>::ExitInstance();
  if (m_bATLInited)
  {
    _Module.Term();
  }
  return res;
#else 
  return CWinApp::ExitInstance();
#endif // ENABLE_VBA_SUPPORT
}


void COdaMfcAppApp::UpdateFieldDisplay() {
  CFrameWnd* pFrame = static_cast<CFrameWnd*>(GetMainWnd());
  pFrame = pFrame->GetActiveFrame();
  CView* pView = pFrame->GetActiveView();
  if (pView && pView->IsKindOf(RUNTIME_CLASS(CDwgView)))
    static_cast<CDwgView*>(pView)->UpdateFieldDisplay();
}

void COdaMfcAppApp::OnViewDwg()
{
  m_displayFields = kDwgFields;
  UpdateFieldDisplay();
  WriteProfileInt(_T("options"), _T("Field display format"), m_displayFields);
}

void COdaMfcAppApp::OnUpdateViewDwg(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_displayFields == kDwgFields);
}

void COdaMfcAppApp::OnViewDxf()
{
  m_displayFields = kDxfFields;
  UpdateFieldDisplay();
  WriteProfileInt(_T("options"), _T("Field display format"), m_displayFields);
}

void COdaMfcAppApp::OnUpdateViewDxf(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_displayFields == kDxfFields);
}

void COdaMfcAppApp::OnViewSchema()
{
  m_displayFields = kSchemaFields;
  UpdateFieldDisplay();
  WriteProfileInt(_T("options"), _T("Field display format"), m_displayFields);
}

void COdaMfcAppApp::OnUpdateViewSchema(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_displayFields == kSchemaFields);
}

static bool s_ignoreWarnings = false;
void COdaMfcAppApp::warning(const char* warnVisGroup, const OdString& sM)
{
  if ( // m_bLoading &&
    !s_ignoreWarnings &&
     (!warnVisGroup || !*warnVisGroup) // default implementation skips all named groups
// ODA_MT_DBIO_BEGIN
// TODO: MT: Implement correct processing of COdaMfcAppApp::warning() in MT mode
    && !m_bUseMTLoading )
// ODA_MT_DBIO_END
  {
    OdString sMsg(sM);
    sMsg += _T("\n\nDo you want to proceed ?");
#ifdef _DEBUG
    switch (MessageBox(NULL, sMsg, _T("Warning!"), MB_ICONWARNING | MB_CANCELTRYCONTINUE))
    {
    case IDCANCEL:
      throw UserBreak();
    case IDTRYAGAIN:
    case IDCONTINUE:
      s_ignoreWarnings = true;
      break;
    }
#else
    if (MessageBox(NULL, sMsg, _T("Warning!"), MB_ICONWARNING | MB_YESNO) == IDNO)
      throw UserBreak();
#endif
  }
}

void COdaMfcAppApp::auditPrintReport(OdAuditInfo* pAuditInfo, const OdString& /*strLine*/, int /*nPrintDest*/) const
{
  if (m_pAuditDlg)
    m_pAuditDlg->printReport((OdDbAuditInfo*)pAuditInfo);
}

OdDbUndoControllerPtr COdaMfcAppApp::newUndoController()
{
  if (undoType())
  {
    ExFileUndoControllerPtr pController = OdRxObjectImpl<ExFileUndoController>::createObject();
    pController->setStorage(newUndoStream());
    return pController;
  }
  
  return OdRxObjectImpl<ExUndoController>::createObject();
}

OdStreamBufPtr COdaMfcAppApp::newUndoStream()
{
  OdSmartPtr<OdMemFileStreamImpl<OdStreamBuf> > pp = OdRxObjectImpl<OdMemFileStreamImpl<OdStreamBuf> >::createObject();
  pp->setAppServices(this);
  return pp;
  // OdMemFileStreamImpl = mix of memory and file streams
}

void COdaMfcAppApp::OnOptionsSaveroundtripdata()
{
  m_bSaveRoundTrip = !m_bSaveRoundTrip;
  WriteProfileInt(_T("options"), _T("Save round trip information"), m_bSaveRoundTrip);
}

void COdaMfcAppApp::OnUpdateOptionsSaveroundtripdata(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bSaveRoundTrip);
}

void COdaMfcAppApp::OnOptionsSavePreview()
{
  m_bSavePreview = !m_bSavePreview;
  WriteProfileInt(_T("options"), _T("Save Preview"), m_bSavePreview);
}

void COdaMfcAppApp::OnUpdateOptionsSavePreview(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bSavePreview);
}

void COdaMfcAppApp::OnOptionsSaveWithPassword()
{
  m_bSaveWithPassword = !m_bSaveWithPassword;
  WriteProfileInt(_T("options"), _T("Save DWG with password"), m_bSaveWithPassword);
}

void COdaMfcAppApp::OnUpdateOptionsSaveWithPassword(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bSaveWithPassword);
}

void COdaMfcAppApp::OnOptionsFillTtf()
{
  setTEXTFILL(!getTEXTFILL());
  WriteProfileInt(_T("options"), _T("Fill TTF text"), getTEXTFILL());
}

void COdaMfcAppApp::OnUpdateOptionsFillTtf(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(getTEXTFILL());
}

void COdaMfcAppApp::OnOptionsDiscardbackfaces() 
{
  m_bDiscardBackFaces = !m_bDiscardBackFaces;
  WriteProfileInt(_T("options"), _T("Discard Back Faces"), m_bDiscardBackFaces);
}

void COdaMfcAppApp::OnUpdateOptionsDiscardbackfaces(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_bDiscardBackFaces);
}

void COdaMfcAppApp::OnOptionsDoublebuffer()
{
  m_bEnableDoubleBuffer = !m_bEnableDoubleBuffer;
  WriteProfileInt(_T("options"), _T("Enable Double Buffer"), m_bEnableDoubleBuffer);
}

void COdaMfcAppApp::OnUpdateOptionsDoublebuffer(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bEnableDoubleBuffer);
}

void COdaMfcAppApp::OnOptionsBlockscache()
{
  m_bBlocksCache = !m_bBlocksCache;
  WriteProfileInt(_T("options"), _T("Enable Blocks Cache"), m_bBlocksCache);
}

void COdaMfcAppApp::OnUpdateOptionsBlockscache(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bBlocksCache);
}

void COdaMfcAppApp::OnOptionsGsdevicemultithread()
{
  m_bGsDevMultithread = !m_bGsDevMultithread;
  WriteProfileInt(_T("options"), _T("Gs Device Multithread"), m_bGsDevMultithread);
}

void COdaMfcAppApp::OnUpdateOptionsGsdevicemultithread(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bGsDevMultithread);
}

void COdaMfcAppApp::OnOptionsNmtregenthreads()
{
  OdUInt32 rVal = (OdUInt32)m_nMtRegenThreads;
  CSimpleInputDlg valInput;
  if (valInput.DoInput(_T("Mt Regen Threads Count"), _T("Enter Mt Regen Threads Count:"), &rVal, 1, 0xFFFF) &&
      (rVal != m_nMtRegenThreads))
  {
    m_nMtRegenThreads = (UINT)rVal;
    WriteProfileInt(_T("options"), _T("Mt Regen Threads Count"), (int)m_nMtRegenThreads);
  }
}

void COdaMfcAppApp::OnUpdateOptionsNmtregenthreads(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(m_bGsDevMultithread);
}

void COdaMfcAppApp::OnOptionsPrintPreview()
{
  m_bEnablePrintPreviewViaBitmap = !m_bEnablePrintPreviewViaBitmap;
  WriteProfileInt(_T("options"), _T("Print/Preview via bitmap device"), m_bEnablePrintPreviewViaBitmap);
}

void COdaMfcAppApp::OnUpdateOptionsPrintPreview(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bEnablePrintPreviewViaBitmap);
}

void COdaMfcAppApp::OnOptionsGDITransparency()
{
  m_bGDITransparency = !m_bGDITransparency;
  WriteProfileInt( _T( "options" ), _T( "GDI Plot With Transparency" ), m_bGDITransparency );
}

void COdaMfcAppApp::OnUpdateOptionsGDITransparency( CCmdUI* pCmdUI )
{
  pCmdUI->SetCheck( m_bGDITransparency );
}

void COdaMfcAppApp::OnOptionsSofthlr()
{
  m_bEnableHLR = !m_bEnableHLR;
  WriteProfileInt(_T("options"), _T("Enable Software HLR"), m_bEnableHLR);
}

void COdaMfcAppApp::OnUpdateOptionsSofthlr(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bEnableHLR);
}

void COdaMfcAppApp::OnOptionsContextualcolors()
{
  m_bContextColors = !m_bContextColors;
  WriteProfileInt(_T("options"), _T("Contextual Colors"), m_bContextColors);
}

void COdaMfcAppApp::OnUpdateOptionsContextualcolors(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bContextColors);
}

void COdaMfcAppApp::OnOptionsTtfpolydrawmode()
{
  m_bTTFPolyDraw = !m_bTTFPolyDraw;
  WriteProfileInt(_T("options"), _T("TTF PolyDraw"), m_bTTFPolyDraw);
}

void COdaMfcAppApp::OnUpdateOptionsTtfpolydrawmode(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bTTFPolyDraw);
}

void COdaMfcAppApp::OnOptionsTtfTriangleCachemode()
{
  m_bTTFTriangleCache = !m_bTTFTriangleCache;
  WriteProfileInt(_T("options"), _T("TTF TriangleCache"), m_bTTFTriangleCache);
}

void COdaMfcAppApp::OnUpdateOptionsTtfTriangleCachemode(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bTTFTriangleCache);
}

void COdaMfcAppApp::OnOptionsTtftextoutmode()
{
  m_bTTFTextOut = !m_bTTFTextOut;
  WriteProfileInt(_T("options"), _T("TTF TextOut"), m_bTTFTextOut);
}

void COdaMfcAppApp::OnUpdateOptionsTtftextoutmode(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bTTFTextOut);
}

void COdaMfcAppApp::OnOptionsTtfcache()
{
  m_bTTFCache = !m_bTTFCache;
  WriteProfileInt(_T("options"), _T("TTF Cache"), m_bTTFCache);
}

void COdaMfcAppApp::OnUpdateOptionsTtfcache(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bTTFCache);
}

void COdaMfcAppApp::OnOptionsDynamicsubenthlt()
{
  m_bDynamicSubEntHlt = !m_bDynamicSubEntHlt;
  WriteProfileInt(_T("options"), _T("Dynamic Subentities Highlight"), m_bDynamicSubEntHlt);
}

void COdaMfcAppApp::OnUpdateOptionsDynamicsubenthlt(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bDynamicSubEntHlt);
}

void COdaMfcAppApp::OnOptionsGradientsasbitmaps()
{
  m_bGDIGradientsAsBitmap = !m_bGDIGradientsAsBitmap;
  WriteProfileInt(_T("options"), _T("GDI Gradients as Bitmaps"), m_bGDIGradientsAsBitmap);
}

void COdaMfcAppApp::OnUpdateOptionsGradientsasbitmaps(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bGDIGradientsAsBitmap);
  //SEA 3/21/2017 "Gradient As Bitmap" now available independent on "Print via Bitmap"
  //pCmdUI->Enable(!m_bEnablePrintPreviewViaBitmap);
}

void COdaMfcAppApp::OnOptionsGradientsaspolys()
{
  m_bGDIGradientsAsPolys = !m_bGDIGradientsAsPolys;
  WriteProfileInt(_T("options"), _T("GDI Gradients as Polys"), m_bGDIGradientsAsPolys);
}

void COdaMfcAppApp::OnUpdateOptionsGradientsaspolys(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bGDIGradientsAsPolys);
  pCmdUI->Enable(!m_bEnablePrintPreviewViaBitmap);
}

void COdaMfcAppApp::OnOptionsGradientsaspolysthreshold()
{
  OdUInt32 rVal = (OdUInt32)m_nGDIGradientsAsPolysThreshold;
  CSimpleInputDlg valInput;
  if (valInput.DoInput(_T("WinGDI Gradients as Polygons Threshold"), _T("Enter WinGDI Gradients as Polygons Threshold:"), &rVal, 0, 0xFF) &&
      (rVal != m_nGDIGradientsAsPolysThreshold))
  {
    m_nGDIGradientsAsPolysThreshold = (BYTE)rVal;
    WriteProfileInt(_T("options"), _T("GDI Gradients as Polys Threshold"), (int)m_nGDIGradientsAsPolysThreshold);
  }
}

void COdaMfcAppApp::OnUpdateOptionsGradientsaspolysshreshold(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(!m_bEnablePrintPreviewViaBitmap);
}

void COdaMfcAppApp::OnViewDisableAutoRegen()
{
  m_bDisableAutoRegen = !m_bDisableAutoRegen;
  WriteProfileInt(_T("options"), _T("Disable Auto-Regen"), m_bDisableAutoRegen);
}

void COdaMfcAppApp::OnUpdateViewDisableAutoRegen(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bDisableAutoRegen);
}

void COdaMfcAppApp::OnToolsLoadapplications()
{
  CLoadApps appDlg;
  appDlg.DoModal();
}

OdGsDevicePtr COdaMfcAppApp::gsBitmapDevice(OdRxObject* /*pViewObj*/,
                                            OdDbBaseDatabase* /*pDb*/,
                                            OdUInt32 flags)
{

  try
  {
    OdGsModulePtr pM;
    if (GETBIT(flags, kFor2dExportRender))
    {
      // Don't export HiddenLine viewports as bitmap in Pdf/Dwf/Svg exports.
      if (GETBIT(flags, kFor2dExportRenderHLR))
        return OdGsDevicePtr();
      // Try to export shaded viewports using OpenGL device.
      pM = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
    }
    // Use currently selected device for thumbnails and etc.
    if (pM.isNull())
      pM = ::odrxDynamicLinker()->loadModule(m_sVectorizerPath);
    if (pM.isNull())
      return OdGsDevicePtr();
    // Create device.
    return pM->createBitmapDevice();
  }
  catch(const OdError&)
  {
  }
  return OdGsDevicePtr();
}

void COdaMfcAppApp::OnOptionsUseGsModel()
{
  m_bUseGsModel = !m_bUseGsModel;
  WriteProfileInt(_T("options"), _T("UseGsModel"), m_bUseGsModel);
}

void COdaMfcAppApp::OnUpdateOptionsUseGsModel(CCmdUI* pCmdUI)
{
  pCmdUI->SetCheck(m_bUseGsModel);
}


//////////////////////////////////////////////////////////////////////
// AE 15.08.2003 - Begin
// AMark 19.05.2008 - Added GetRegistryAVEMAPSFromProfile()
#define SMALL_STRING_SIZE 32
#define LARGE_STRING_SIZE 1024
OdString GetRegistryAcadLocation();
OdString GetRegistryACADFromProfile();
OdString GetRegistryAVEMAPSFromProfile();
OdString GetRegistryAcadProfilesKey();
bool GetRegistryString(  HKEY key, const wchar_t *subkey, const wchar_t *name, wchar_t *value, int size);
bool GetRegistryString(  HKEY key, const char *subkey, const char *name, char *value, int size);
HRESULT __fastcall UnicodeToAnsi(LPCOLESTR pszW, LPSTR* ppszA);
HRESULT __fastcall AnsiToUnicode(LPCSTR pszA, LPOLESTR* ppszW);


// Find Plot Config file or Style Sheet file
//
static OdString FindConfigPath( const OdString& configType)
{
  TCHAR searchPath[ LARGE_STRING_SIZE];
  TCHAR expandedPath[ LARGE_STRING_SIZE];

  OdString subkey = GetRegistryAcadProfilesKey();
  if ( !subkey.isEmpty() )
  {
    subkey += L"\\General";
    // get the value for the ACAD entry in the registry
    if( ::GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, (LPCTSTR)configType, searchPath, LARGE_STRING_SIZE ) )
    {
      ExpandEnvironmentStrings(searchPath, expandedPath, LARGE_STRING_SIZE);
      return OdString(expandedPath);
    }
  }
  return OdString::kEmpty;
}

static OdString FindConfigFile( const OdString& configType, OdString file, OdDbSystemServices* pSs)
{
  OdString searchPath = FindConfigPath( configType );
  if (!searchPath.isEmpty())
  {
    file = searchPath + L"\\" + file;
    if ( pSs->accessFile( file, Oda::kFileRead ) ) return file;
  }
  return OdString::kEmpty;
}

//////////////////////////////////////////////////////////////////////
OdString COdaMfcAppApp::findFile(const OdString& file, OdDbBaseDatabase* pDb,
                                 OdDbBaseHostAppServices::FindFileHint hint)
{
  OdString sFile = ExHostAppServices::findFile(file, pDb, hint);
  if( !sFile.isEmpty() )
    return sFile;

  OdDbSystemServices* pSs = odSystemServices();

  OdString sFileName = file;
  OdString ext = sFileName.right(4);
  ext.makeUpper();

  if ( ext == L".PC3" )
    return FindConfigFile( L"PrinterConfigDir", file, pSs );

  if ( ext == L".STB" || ext == L".CTB")
    return FindConfigFile( L"PrinterStyleSheetDir", file, pSs );

  if ( ext == L".PMP" )
    return FindConfigFile( L"PrinterDescDir", file, pSs );

  switch (hint)
  {
  case kFontFile:
  case kCompiledShapeFile:
  case kTrueTypeFontFile:
  case kPatternFile:
  case kFontMapFile:
  case kTextureMapFile:
    break;
  case kEmbeddedImageFile:
    if (sFileName.left(5).iCompare(L"http:") == 0 || sFileName.left(6).iCompare(L"https:") == 0)
    {
      try
      {
        CInternetSession iss;
        if (CStdioFile* f = iss.OpenURL(sFileName))
        {
          ULONGLONG len = f->GetLength();
          OdString tmpPath = getTempPath();
          TCHAR temp[MAX_PATH_LENGTH + 1] = { _T('\0') };
          ::GetTempFileName(tmpPath.c_str(), _T(""), 0, temp);
          CStdioFile tempFile(temp, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
          char buf[10000];
          do
          {
            UINT N = 10000;
            if (N > len)
              N = (UINT)len;
            f->Read(buf, N);
            tempFile.Write(buf, N);
            len -= N;
          } while (len != 0);
          tempFile.Close();
          f->Close();
          delete f;
          m_tempFilesList.Add(temp);
          sFile = temp;
        }
      }
      catch (const CException&)
      {
      }
    }
    // fall through
  default:
    return sFile;
  }

  if (hint != kTextureMapFile && ext !=L".SHX" && ext != L".PAT" && ext != L".TTF" && ext != L".TTC" && ext != L".OTF" && ext != L".LIN")
  {
    sFileName += L".shx";
  }
  else if (hint == kTextureMapFile)
  {
    sFileName.replace(L'/', L'\\');
    sFileName.deleteChars(0, sFileName.reverseFind(L'\\')+1);
  }

  sFile = (hint != kTextureMapFile) ? GetRegistryACADFromProfile() : GetRegistryAVEMAPSFromProfile();
  while (!sFile.isEmpty())
  {
    int nFindStr = sFile.find(L";");
    OdString sPath;
    if (-1 == nFindStr)
    {
      sPath = sFile;
      sFile.empty();
    }
    else
    {
      sPath = sFile.left(nFindStr);
      sPath += L"\\";
      sPath += sFileName;
      if (pSs->accessFile(sPath, Oda::kFileRead))
      {
        return sPath;
      }
      sFile = sFile.right(sFile.getLength() - nFindStr - 1);
    }
  }

  if (hint == kTextureMapFile)
  {
    return sFile;
  }

  if (sFile.isEmpty())
  {
    OdString sAcadLocation(GetRegistryAcadLocation());
    if (!sAcadLocation.isEmpty())
    {
      sFile = sAcadLocation;
      sFile += L"\\Fonts\\";
      sFile += sFileName;
      if (!pSs->accessFile(sFile, Oda::kFileRead))
      {
        sFile = sAcadLocation;
        sFile += L"\\Support\\";
        sFile += sFileName;
        if (!pSs->accessFile(sFile, Oda::kFileRead))
        {
          sFile = sAcadLocation;
          sFile += L"\\UserDataCache\\Support\\";
          sFile += sFileName;
          if (!pSs->accessFile(sFile, Oda::kFileRead))
          {
            sFile = L"";
          }
        }
      }
    }
  }
  return sFile;
}

const OdString COdaMfcAppApp::product()
{
  return L"OdaMfcApp sample Application";
}

//////////////////////////////////////////////////////////////////////
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
  if( RegOpenKeyExW( key, subkey, 0, KEY_READ | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS)
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
      swprintf( value, L"%ls\0", (const wchar_t*)data);
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
 if( GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), version, SMALL_STRING_SIZE) == 0)
  return L"";
 subkey += L"\\";
 subkey += version;
 
 // get the sub-version and concatenate onto subkey
 if( GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), subVersion, SMALL_STRING_SIZE) == 0)
  return L"";
 subkey += L"\\";
 subkey += subVersion;
 
 // get the value for the AcadLocation entry in the registry
 if( GetRegistryString(HKEY_LOCAL_MACHINE, (LPCTSTR)subkey, _T("AcadLocation"), searchPaths, LARGE_STRING_SIZE) == 0)
  return L"";

 return OdString(searchPaths);
}

//////////////////////////////////////////////////////////////////////
OdString GetRegistryAcadProfilesKey()
{
        OdString subkey = L"SOFTWARE\\Autodesk\\AutoCAD";
  TCHAR version[SMALL_STRING_SIZE];
  TCHAR subVersion[SMALL_STRING_SIZE];
  TCHAR profile[LARGE_STRING_SIZE];
        // char searchPaths[ LARGE_STRING_SIZE];

  if (GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), version, SMALL_STRING_SIZE) == 0)
                return L"";
        subkey += L"\\";
        subkey += version;


        // get the sub-version and concatenate onto subkey
  if (GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), subVersion, SMALL_STRING_SIZE) == 0)
                return L"";
        subkey += L"\\";
        subkey += subVersion;
        subkey += L"\\Profiles";


        // get the value for the (Default) entry in the registry
  if (GetRegistryString(HKEY_CURRENT_USER, (LPCTSTR)subkey, _T(""), profile, LARGE_STRING_SIZE) == 0)
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
  TCHAR expandedPath[ LARGE_STRING_SIZE];

  subkey = GetRegistryAcadProfilesKey();
  if ( !subkey.isEmpty() )
  {
    subkey += L"\\General";
    // get the value for the ACAD entry in the registry
    if( GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("ACAD"), searchPaths, LARGE_STRING_SIZE) == 0)
      return L"";

    ExpandEnvironmentStrings(searchPaths, expandedPath, LARGE_STRING_SIZE);
    return OdString(expandedPath);
  } 
  else
    return L"";
}

OdString GetRegistryAVEMAPSFromProfile()
{
  OdString subkey; // = "SOFTWARE\\Autodesk\\AutoCAD";
  TCHAR searchPaths[ LARGE_STRING_SIZE];
  TCHAR expandedPath[ LARGE_STRING_SIZE];

  subkey = GetRegistryAcadProfilesKey();
  if ( !subkey.isEmpty() )
  {
    subkey += L"\\General";
    // get the value for the ACAD entry in the registry
    if( GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("AVEMAPS"), searchPaths, LARGE_STRING_SIZE) == 0)
      return L"";

    ExpandEnvironmentStrings(searchPaths, expandedPath, LARGE_STRING_SIZE);
    return OdString(expandedPath);
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
  // If input is null then just return the same.
  if (NULL == pszA)
  {
      *ppszW = NULL;
      return NOERROR;
  }

  // Determine number of wide characters to be allocated for the
  // Unicode string.
  int cCharacters =  int(strlen(pszA) + 1);

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
      DWORD dwError = GetLastError();
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
  // If input is null then just return the same.
  if (pszW == NULL)
  {
      *ppszA = NULL;
      return NOERROR;
  }

  int cCharacters = int(wcslen(pszW) + 1);
  // Determine number of bytes to be allocated for ANSI string. An
  // ANSI string can have at most 2 bytes per character (for Double
  // Byte Character Strings.)
  int cbAnsi = cCharacters*2;

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
      DWORD dwError = GetLastError();
      CoTaskMemFree(*ppszA);
      *ppszA = NULL;
      return HRESULT_FROM_WIN32(dwError);
  }
  return NOERROR;

}

// AE - End

OdString COdaMfcAppApp::getSubstituteFont(const OdString& fontName, OdFontType fontType)
{
  return OdString(L"simplex.shx");
}

OdString COdaMfcAppApp::getFontMapFileName() const
{
  // return L"c:\\acad.fmp";
  OdString subkey;
  TCHAR fontMapFile[ LARGE_STRING_SIZE];
  TCHAR expandedPath[ LARGE_STRING_SIZE];
  subkey = GetRegistryAcadProfilesKey();
  if ( ! subkey.isEmpty() )
  {
    subkey += L"\\Editor Configuration";
    // get the value for the ACAD entry in the registry
    if( GetRegistryString( HKEY_CURRENT_USER, subkey, _T("FontMappingFile"), fontMapFile, LARGE_STRING_SIZE) == 0)
      return L"";

    ExpandEnvironmentStrings(fontMapFile, expandedPath, LARGE_STRING_SIZE);
    return OdString(expandedPath);
  }
  else
    return L"c:\\acad.fmp";
}

OdString COdaMfcAppApp::getTempPath() const
{
  OdString subkey;
  TCHAR tempPath[MAX_PATH];

  subkey = GetRegistryAcadProfilesKey();
  if ( !subkey.isEmpty() )
  {
    subkey += L"\\General Configuration";
    // get the value for the ACAD entry in the registry
    if( GetRegistryString( HKEY_CURRENT_USER, subkey, _T("TempDirectory"), tempPath, MAX_PATH) == 0)
      return OdDbHostAppServices::getTempPath();

    if (_waccess(tempPath, 0))//we use _waccess instead of accessFile because of accessFile doesn't work with directories
      return OdDbHostAppServices::getTempPath();
    else
    {
      OdString ret(tempPath, (int)odStrLen(tempPath));
      if (ret.getAt(ret.getLength()-1) != OdChar('\\'))
        ret += OdChar('\\');
      return ret;
    }
  }
  else
    return OdDbHostAppServices::getTempPath();
}

OdString GetWinSubstituteFont(OdString fontName)
{
  OSVERSIONINFO           os;
  int result;
  HKEY    fontkey;
  int value_len = 255;
  int data_len = 1023;
  TCHAR value[256];
  unsigned char data[1024];
  DWORD type;
  OdString retStr;
  
  os.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
  GetVersionEx(&os);
  if (os.dwPlatformId & VER_PLATFORM_WIN32_NT)
  {
    result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
      _T("Software\\Microsoft\\Windows nt\\CurrentVersion\\FontSubstitutes"),
      0,
      KEY_READ,
      &fontkey);
  } 
  else
  {
    result=RegOpenKeyEx(HKEY_LOCAL_MACHINE,
      _T("Software\\Microsoft\\Windows\\CurrentVersion\\FontSubstitutes"),
      0,
      KEY_READ,
      &fontkey);
  }
  
  for(int n = 0; ; n++)
  {
    value_len = data_len = 255;
    result = RegEnumValue(fontkey,
      n,                        // index of value to query
      value,                  // address of buffer for value string
      (unsigned long *)&value_len, // address for size of value buffer
      0,                        // reserved
      &type,                        // address of buffer for type code
      data,                   // address of buffer for value data
      (unsigned long *)&data_len); // address for size of data buffer);
    if (result != ERROR_SUCCESS)
    {
      break;
    }
    
    if ( fontName.iCompare(OdString(value)) == 0 )
    {
      OdString Data((const TCHAR*)data);
      int n = Data.find(L',');
      if ( n > 0 )
        retStr = Data.left(n);
      else
        retStr = Data;
      break;
    }
  }
  
  if (fontkey)
  {
    RegCloseKey(fontkey);
  }
  return retStr;
}

OdString GetAcadRegSubstituteFont(OdString fontName)
{
  int           result,n;
  HKEY          fontkey;
  int           value_len=255;
  int           data_len=255;
  TCHAR        value[256];
  unsigned char data[256];
  DWORD         type;
  OdString      retStr;
  
  OdString subkey = L"SOFTWARE\\Autodesk\\AutoCAD";
  TCHAR version[ SMALL_STRING_SIZE];
  TCHAR subVersion[ SMALL_STRING_SIZE];
  
  if( GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), version, SMALL_STRING_SIZE) == 0)
    return L"";
  subkey += L"\\";
  subkey += version;
  
  
  // get the sub-version and concatenate onto subkey
  if( GetRegistryString( HKEY_CURRENT_USER, (LPCTSTR)subkey, _T("CurVer"), subVersion, SMALL_STRING_SIZE) == 0)
    return L"";

  subkey += L"\\";
  subkey += subVersion;

  subkey += L"\\AltFontNames";

  result=RegOpenKeyExW(HKEY_CURRENT_USER,
                      subkey,
                      0,
                      KEY_READ | KEY_WOW64_64KEY,
                      &fontkey);
  if ( result != ERROR_SUCCESS )
    return L"";

  for(n = 0; ; n++)
  {
    value_len = data_len = 255;
    result = RegEnumValue(fontkey,
                          n,                          // index of value to query
                          value,                      // address of buffer for value string
                          (unsigned long *)&value_len,// address for size of value buffer
                          0,                          // reserved
                          &type,                      // address of buffer for type code
                          data,                       // address of buffer for value data
                          (unsigned long *)&data_len);// address for size of data buffer);
    if (result != ERROR_SUCCESS)
    {
      break;
    }
    
    if ( fontName.iCompare(OdString(value)) == 0 )
    {
      retStr = (const TCHAR*)data;
      break;
    }
  }
  
  if (fontkey)
  {
    RegCloseKey(fontkey);
  }
  return retStr;
}

bool COdaMfcAppApp::getPassword(const OdString& dwgName, bool /*isXref*/, OdPassword& password)
{
  CPasswordDlg pwdDlg;
  pwdDlg.m_sFileName = (LPCTSTR)dwgName;
  if (pwdDlg.DoModal()==IDOK)
  {
    password = pwdDlg.m_password;
    if (dwgName.right(4) == L".dwg")
    {
      password.makeUpper();
    }
    return true;
  }
  return false;
}

OdDbPageControllerPtr COdaMfcAppApp::newPageController()
{
  switch (m_pagingType & 0x0f)
  {
  case 1: //OdDb::kUnload
    // Simple unloading of objects for partially loaded database.
    return OdRxObjectImpl<ExUnloadController>::createObject();
  case 2: //OdDb::kPage
  case 3: //OdDb::kUnload | OdDb::kPage
    // Unloading of objects for partially loaded database and paging of objects thru ExPageController.
    return OdRxObjectImpl<ExPageController>::createObject();
  }
  // Paging is not used.
  return (OdDbPageController*)0;
}

int COdaMfcAppApp::setPagingType(int pagingType)
{ 
  int oldType = m_pagingType;
  m_pagingType = pagingType;
  return oldType;
}

bool COdaMfcAppApp::setUndoType(bool bUseTempFiles)
{ 
  bool oldType = m_bUseTempFiles;
  m_bUseTempFiles = bUseTempFiles;
  return oldType;
}

OdString COdaMfcAppApp::recentGsDevicePath() const
{
  return m_sVectorizerPath;
}

void COdaMfcAppApp::setRecentGsDevicePath(const OdString& sVectorizerPath)
{
  WriteProfileString(_T("options"), _T("recent GS"), sVectorizerPath);
  m_sVectorizerPath = sVectorizerPath;
}

bool addGsMenuItem(CMenu* pVectorizeMenu, DWORD& nIndex, LPCTSTR szPath) 
{
  if(ID_VECTORIZER_FIRST + nIndex <= ID_VECTORIZER_LAST)
  {
    MENUITEMINFO menuItemInfo;
    menuItemInfo.cbSize = sizeof(menuItemInfo);
    menuItemInfo.fMask = MIIM_DATA;
    
    pVectorizeMenu->InsertMenu(nIndex, MF_BYPOSITION, ID_VECTORIZER_FIRST + nIndex, szPath);
    
    menuItemInfo.dwItemData = theApp.getGSMenuItemMarker();
    VERIFY(::SetMenuItemInfo(pVectorizeMenu->m_hMenu, nIndex, TRUE, &menuItemInfo));
    
    if(theApp.recentGsDevicePath().iCompare(OdString(szPath))==0)
    {
      pVectorizeMenu->CheckMenuItem(nIndex, MF_BYPOSITION|MF_CHECKED);
    }
    ++nIndex;
    return true;
  }
  return false;
}

CString COdaMfcAppApp::getApplicationPath()
{
  TCHAR path[MAX_PATH];
  ::GetModuleFileName(::GetModuleHandle(0), path, MAX_PATH);
  TCHAR path_canon[MAX_PATH];
  ::PathCanonicalize(path_canon, path);
  CString s_path(path_canon);
  int nLen = s_path.ReverseFind('\\');
  s_path = s_path.Left(nLen);
  return s_path;
}

void COdaMfcAppApp::OnVectorizeAddVectorizerDLL() 
{
  CMenu* pVectorizeMenu = GetMainWnd()->GetMenu()->GetSubMenu(4);

#ifdef _TOOLKIT_IN_DLL_
  CFileDialog dlg( TRUE, _T(VECTORIZATION_MODULE_EXTENSION_S), _T(""),
    OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST,
#ifdef _UNICODE
    L"Graphic System DLL (*."  VECTORIZATION_MODULE_EXTENSION_W  L")|*." VECTORIZATION_MODULE_EXTENSION_W  L"|Windows DLL (*.dll)|*.dll||",
#else
    "Graphic System DLL (*."  VECTORIZATION_MODULE_EXTENSION_S  ")|*." VECTORIZATION_MODULE_EXTENSION_S  "|Windows DLL (*.dll)|*.dll||",
#endif
    ::AfxGetMainWnd() );
  dlg.m_ofn.lpstrTitle = _T("Select Graphic System DLL");
  CString s_path = getApplicationPath();
  dlg.m_ofn.lpstrInitialDir = s_path.GetBuffer(s_path.GetLength());
#else
  CStaticAppSelDlg dlg(::AfxGetMainWnd());
#endif //#ifdef _TOOLKIT_IN_DLL_

  if ( dlg.DoModal() == IDOK )
  {
    m_sVectorizerPath = dlg.GetFileName();
#ifdef _TOOLKIT_IN_DLL_
    m_sVectorizerPath.replace(TD_DLL_VERSION_SUFFIX_STR, "");
#endif
    ::addGsMenuItem(pVectorizeMenu, m_numGSMenuItems, m_sVectorizerPath) ;
    WriteProfileString(_T("options\\vectorizers"), m_sVectorizerPath, _T(""));
    GetMainWnd()->SendMessage(WM_COMMAND, ID_VECTORIZE);
  }
}

void COdaMfcAppApp::OnUpdateVectorizeAddvectorizerdll(CCmdUI* /*pCmdUI*/) 
{
  if(m_numGSMenuItems == 0)
  {
    CMenu* pVectorizeMenu = GetMainWnd()->GetMenu()->GetSubMenu(4);
    CRegKey rk;
    rk.Create(HKEY_CURRENT_USER, _T("Software\\") AppRegistryKey _T("\\OdaMfcApp\\options\\vectorizers"));

    CString path;
    DWORD pathSize;
    for(;;)
    {
      pathSize = _MAX_FNAME + _MAX_EXT;
      DWORD err = ::RegEnumValue(rk, m_numGSMenuItems,
                        path.GetBuffer(pathSize), &pathSize, NULL, NULL, NULL, NULL);
      path.ReleaseBuffer();
      if(err == ERROR_SUCCESS)
      {
        if(!::addGsMenuItem(pVectorizeMenu, m_numGSMenuItems, path))
          break;
      }
      else
        break;
    }
  }
}

void COdaMfcAppApp::OnVectorizeClearmenu() 
{
  CMenu* pVectorizeMenu = GetMainWnd()->GetMenu()->GetSubMenu(4);
  while(pVectorizeMenu->GetMenuItemCount()>3)
  {
    pVectorizeMenu->RemoveMenu(0, MF_BYPOSITION);
  }
  CRegKey rk;
  rk.Create(HKEY_CURRENT_USER, _T("Software\\") AppRegistryKey _T("\\OdaMfcApp\\options"));
  rk.RecurseDeleteKey(_T("vectorizers"));
  setRecentGsDevicePath(OdString::kEmpty);
  m_numGSMenuItems = 0;
}

void COdaMfcAppApp::OnUpdateVectorizeClearmenu(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_numGSMenuItems > 0);
}

void COdaMfcAppApp::OnUpdateVectorize(CCmdUI* pCmdUI) 
{
  //pCmdUI->Enable(!m_sVectorizerPath.isEmpty());
  pCmdUI->Enable(FALSE); // Always false, because only OdaMfcAppDoc can start vectorization
}

void COdaMfcAppApp::setRecentCmd(const OdString& cmd)
{
  if ( !cmd.isEmpty() && cmd != m_sRecentCmd )
  {
    m_sRecentCmd = cmd;
    WriteProfileString(_T("options"), _T("Recent Command"), m_sRecentCmd);
  }
}

OdString COdaMfcAppApp::fileDialog(int nFlags, const OdString& prompt, 
                                   const OdString& defExt, const OdString& fileName, const OdString& filter)
{
  if (!supportFileSelectionViaDialog())
  {
    return OdString(OD_T("*unsupported*"));
  }

  DWORD dlgFlags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST;
  if (GETBIT(nFlags, OdEd::kGfpOverwritePrompt))
    dlgFlags |= OFN_OVERWRITEPROMPT;

  CFileDialog dlg( !GETBIT(nFlags, OdEd::kGfpForSave), defExt, fileName, dlgFlags, filter, ::AfxGetMainWnd() );

  dlg.m_ofn.lpstrTitle = prompt;
  if (!defExt.isEmpty() && !filter.isEmpty())
  { // Set filter index else default extension does not work
    int nIndx = 1;
    OdString strFilter(filter);
    OdString strExtensions;
    for (;;)
    {
      int n = strFilter.find(L'|');
      if (n <= 0)
        break;
      strFilter = strFilter.mid(n + 1);
      n = strFilter.find(L'|');
      if (n <= 0)
        break;
      strExtensions = strFilter.left(n);
      if (strExtensions.find(defExt) >= 0)
        break;
      strFilter = strFilter.mid(n + 1);
      ++nIndx;
    }
    dlg.m_ofn.nFilterIndex = nIndx;
  }
  if ( dlg.DoModal() == IDOK )
    return OdString((LPCTSTR)dlg.GetPathName());

  throw OdEdCancel();
}

OdDbDatabasePtr COdaMfcAppApp::openFile(LPCTSTR lpszPathName)
{
  CMainFrame* pMFrame = (CMainFrame*)GetMainWnd();
  OdDbDatabasePtr pDb;

// ODA_MT_DBIO_BEGIN
  OdInt16 nMode = getMtMode();
  SETBIT(nMode, 1, m_bUseMTLoading);
  setMtMode(nMode);
// ODA_MT_DBIO_END

  // open an existing document
  pMFrame->StartTimer();
  try
  {  
    CWaitCursor   wait;
    OdString path(lpszPathName);
    OdString shxPath;

    OdString sFile = GetRegistryACADFromProfile();

    while( !sFile.isEmpty() )
    {
      int nFindStr = sFile.find(L";");
      OdString sPath;

      if (-1 == nFindStr)
      {
        sPath = sFile;
        sFile.empty();
      }
      else
      {
        sPath = sFile.left(nFindStr);
        sPath += L"\\";

        OdString sPathU = sPath;
        sPathU.makeUpper();

        if( sPathU.find(L"FONTS") != -1 )
        {
          shxPath = sPath;
          break;
        }
       
        sFile = sFile.right(sFile.getLength() - nFindStr - 1);
      }
    }

    if (path.right(4).iCompare(L".dwf") == 0 || path.right(5).iCompare(L".dwfx") == 0)
    {
      OdDwfImportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDwf7ImportModuleName);
      OdDwfImportPtr importer = pModule->create();
      OdRxDictionaryPtr pProps = importer->properties();
      pDb = createDatabase();
      pProps->putAt(OD_T("Database"), pDb);
      pProps->putAt(OD_T("DwfPath"), OdRxVariantValue(path));
      pProps->putAt(OD_T("PreserveColorIndices"), OdRxVariantValue(false));
      pProps->putAt(OD_T("DwgPalette"), OdRxVariantValue(OdIntPtr(curPalette())));
      pDb->disableUndoRecording(true); // CORE-13721
      OdDwfImport::ImportResult res = importer->import();
      pDb->disableUndoRecording(false);
      switch (res)
      {
      case OdDwfImport::success:
        break;
      case OdDwfImport::bad_password:
        messageBox(_T("DWF import"), _T("The file is encrypted"), MB_OK | MB_ICONERROR);
        break;
      default:
        messageBox(_T("DWF import"), _T("Import error"), MB_OK | MB_ICONERROR);
        break;
      }
      OdRxVariantValue backGround = (OdRxObject*)pProps->getAt(OD_T("Background"));
      setActiveBackground((ODCOLORREF)backGround->getInt32());
      pMFrame->StopTimer(_T("Loading"));
    }
#ifdef DGNIMPORT_ENABLE
    else if (path.right(4).iCompare(L".dgn") == 0)
    {
//       OdStringArray arrRsrc;
//       arrRsrc.push_back( L"D:/font.rsc" );
      OdDgnImportModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdDgnImportModuleName, false);
      OdDgnImportPtr importer = pModule->create();
      importer->properties()->putAt( L"Services", static_cast<ExHostAppServices*>(this) );
      importer->properties()->putAt( L"DgnPath", OdRxVariantValue(path) );
//      importer->properties()->putAt( L"MS_SYMBRSRC", OdRxVariantValue(arrRsrc) );
      importer->properties()->putAt( L"XRefImportMode", OdRxVariantValue(OdUInt8(2)) );
      importer->properties()->putAt( L"ImportActiveModelToModelSpace", OdRxVariantValue(true) );
      importer->properties()->putAt( L"ImportPaperSpaceModels", OdRxVariantValue(true) );
      importer->properties()->putAt( L"RecomputeDimensionsAfterImport", OdRxVariantValue(false) );
      importer->properties()->putAt( L"ImportViewIndex", OdRxVariantValue(OdUInt8(0)) );
      importer->properties()->putAt( L"3dShapeImportMode", OdRxVariantValue(OdUInt8(1)) );
      importer->properties()->putAt( L"shxFontsPath", OdRxVariantValue(shxPath));
      //importer->properties()->putAt( L"3dObjectImportMode", OdRxVariantValue(OdUInt8(1)) );

      OdDgnImport::ImportResult res = importer->import();

      if (res == OdDgnImport::success)
        pDb = importer->properties()->getAt(L"Database");
      else
      {
        switch(res)
        {
        case OdDgnImport::bad_database:
          messageBox(_T("DGN import"), _T("Bad database"), MB_OK|MB_ICONERROR);
          break;
        case OdDgnImport::bad_file:
          messageBox(_T("DGN import"), _T("Bad file"), MB_OK|MB_ICONERROR);
          break;
        case OdDgnImport::encrypted_file:
        case OdDgnImport::bad_password:
          messageBox(_T("DGN import"), _T("The file is encrypted"), MB_OK|MB_ICONERROR);
          break;
        case OdDgnImport::fail:
          messageBox(_T("DGN import"), _T("Unknown import error"), MB_OK|MB_ICONERROR);
          break;
        }
      }
      pMFrame->StopTimer(_T("Loading"));
    }
#endif
#ifdef IFC2DWG_SUPPORT
    else if (path.right(4).iCompare(L".ifc") == 0)
    {
      OdIfc2DwgModulePtr pModule = ::odrxDynamicLinker()->loadApp(OdIfc2DwgModuleName, false);
      OdIfcExportPtr Exporter = pModule->create();
      Exporter->properties()->putAt(L"OdDbServices", static_cast<ExHostAppServices*>(this));
      Exporter->properties()->putAt(L"IfcFilePath", OdRxVariantValue(path));
      Exporter->properties()->putAt(L"ExportBuildingElementProxy", OdRxVariantValue(true));
      Exporter->properties()->putAt(L"ZoomExtents", OdRxVariantValue(true));
      Exporter->properties()->putAt(L"ExportMode", OdRxVariantValue(OdInt16(TD_IFC_EXPORT::kAsPolyFaceMesh)));

      OdIfcExport::ExportResult res = Exporter->exportIfc();

      if (res == OdIfcExport::success)
      {
        pDb = Exporter->properties()->getAt(L"Database");
        OdRxObjectPtr pFile = Exporter->properties()->getAt(L"IfcFile");
        OdIfcConnectionMapPtr pMap = Exporter->properties()->getAt(L"IfcConnectionMap");
        OdRxObjectPtr pMapAssignedFile = pMap->getIfcFile();
        ODA_ASSERT(pFile.get() == pMapAssignedFile.get());
      }
      else
      {
        switch (res)
        {
        case OdIfcExport::bad_database:
          messageBox(_T("IFC import"), _T("Bad database"), MB_OK | MB_ICONERROR);
          break;
        case OdIfcExport::bad_file:
          messageBox(_T("IFC import"), _T("Bad file"), MB_OK | MB_ICONERROR);
          break;
        case OdIfcExport::fail:
          messageBox(_T("IFC import"), _T("Unknown import error"), MB_OK | MB_ICONERROR);
          break;
        }
      }
      pMFrame->StopTimer(_T("Loading"));
    }
#endif
    else if (path.right(4).iCompare(L".dsf") == 0)
    {
      ::odrxDynamicLinker()->loadModule(L"TD_Tf", false);
      ::odTfInitialize(this);
      OdTfRepositoryPtr pRep = OdTfRepository::createObject(createFile(path, Oda::FileAccessMode(Oda::kFileWrite | Oda::kFileRead), Oda::kShareDenyWrite, Oda::kOpenExisting), this);
      pDb = pRep->checkout(OdString(L"master"), OdString(L"master"), m_bPartial);
      pMFrame->StopTimer(_T("Loading"));
    }
    else if (m_bRecover)
    {
      ODA_ASSERT(!m_pAuditDlg);
      m_pAuditDlg = new CAuditDlg();
      ODA_ASSERT(m_pAuditDlg);
      ODA_VERIFY(m_pAuditDlg->Create(IDD_AUDITINFO));

      CAppAuditInfo aiInfo;
      aiInfo.setHostAppServices(&theApp);

      pDb = recoverFile(createFile(OdString(lpszPathName)), &aiInfo);
      pMFrame->StopTimer(_T("Recovering"));

      // Show recover result
      CString strTitle = _T("Recover info ");
      strTitle += lpszPathName;
      m_pAuditDlg->SetWindowText (strTitle);
      m_pAuditDlg->ShowWindow (SW_SHOW);

      // Dialog will be destroyed on exit
      m_pAuditDlg = NULL;
    }
    else
    {
      m_bLoading = true;
        pDb = readFile(OdString(lpszPathName), false, m_bPartial);
      pMFrame->StopTimer(_T("Loading"));
      m_bLoading = false;
    }
  }
  catch(const OdError& e)
  {
    pDb = 0;
    pMFrame->m_wndStatusBar.SetPaneText(0, _T(""));
    reportError(_T("Loading Error..."), e);
  }
  catch(const UserBreak&)
  {
    pDb = 0;
    pMFrame->m_wndStatusBar.SetPaneText(0, _T(""));
    setStatusText(_T("Operation was canceled by user."));
  }
  catch(std::bad_alloc&)
  {
    pDb = 0;
    pMFrame->m_wndStatusBar.SetPaneText(0, _T(""));
    setStatusText(_T("Memory Allocation Error..."));
  }

  // Destroy audit dialog if recover failed
  if (m_pAuditDlg)
  {
    delete m_pAuditDlg;
    m_pAuditDlg = 0;
  }

  return pDb;
}

void COdaMfcAppApp::initPlotStyleSheetEnv()
{
  OdString styleSheetFiles = FindConfigPath( L"PrinterStyleSheetDir" );
  setEnv(OD_T("DDPLOTSTYLEPATHS"), styleSheetFiles);
}

#ifdef MECHANICAL_MODE
#include "CMechMainFrm.h"
#include "CMechDoc.h"
#endif

void COdaMfcAppApp::createDocTemplate()
{
  // Register document templates
  CMultiDocTemplate* pDocTemplate;
  pDocTemplate = new CMultiDocTemplate(
    IDR_ODAMFCTYPE,
#ifdef MECHANICAL_MODE
    RUNTIME_CLASS(CMechDoc),
#else
    RUNTIME_CLASS(COdaMfcAppDoc),
#endif
    RUNTIME_CLASS(CChildFrame),
    RUNTIME_CLASS(CDwgView)
  );

  AddDocTemplate(pDocTemplate);
}

BOOL COdaMfcAppApp::createMainMDIFrame(CMainFrame*& pMainFrame)
{
  // create main MDI Frame window
#ifdef MECHANICAL_MODE
  pMainFrame = new CMechMainFrm;
#else
  pMainFrame = new CMainFrame;
#endif
  if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
    return FALSE;
  m_pMainWnd = pMainFrame;
  return TRUE;
}

BOOL COdaMfcAppApp::OnIdle(LONG lCount) 
{
#ifdef ODAMFC_EXPORT_SYMBOL
  for ( OdUInt32 i = 0; i < m_aAppReactors.size(); ++i )
    m_aAppReactors[ i ]->OnIdle( lCount );
#endif // ODAMFC_EXPORT_SYMBOL

  return CWinApp::OnIdle(lCount);
#if 0
  CFrameWnd* pFrame = static_cast<CFrameWnd*>(GetMainWnd());
  pFrame = pFrame->GetActiveFrame();
  CView* pView = pFrame->GetActiveView();
  if(pView && pView->IsKindOf(RUNTIME_CLASS( CDwgViewer )))
  {
    static_cast<CDwgViewer*>(pView)->OnIdle(lCount);
  }
  return CWinApp::OnIdle(lCount);
#endif
}

BOOL COdaMfcAppApp::PreTranslateMessage(MSG* pMsg)
{
#ifdef ODAMFC_EXPORT_SYMBOL
  for ( OdUInt32 i = 0; i < m_aAppReactors.size(); ++i )
    m_aAppReactors[ i ]->OnPreTranslateMessage( pMsg );
#endif // ODAMFC_EXPORT_SYMBOL
  return CWinApp::PreTranslateMessage(pMsg);
}

#ifdef ODAMFC_EXPORT_SYMBOL
  void COdaMfcAppApp::AddReactor( OdApplicationReactor* pReactor )
  {
    if ( m_aAppReactors.end() == std::find( m_aAppReactors.begin(), m_aAppReactors.end(), OdApplicationReactorPtr( pReactor ) ) )
      m_aAppReactors.push_back( pReactor );
  }

  void COdaMfcAppApp::RemoveReactor( OdApplicationReactor* pReactor )
  {
    m_aAppReactors.erase( 
      std::remove( m_aAppReactors.begin(), m_aAppReactors.end(), OdApplicationReactorPtr( pReactor ) ),
      m_aAppReactors.end() );
  }
#endif // ODAMFC_EXPORT_SYMBOL

#ifdef ENABLE_VBA_SUPPORT
void SetErrorInfo(UINT uiStringID)
{
  CComPtr<ICreateErrorInfo> spCEI;
  CComQIPtr<IErrorInfo, &IID_IErrorInfo> spEI;
  CString strError;
  
  strError.LoadString(uiStringID);
  if(!strError.IsEmpty())
  {
    CreateErrorInfo(&spCEI);
    spCEI->SetDescription(strError.AllocSysString());
    spEI = spCEI;
    SetErrorInfo(0, spEI);
  }
}

HRESULT COdaMfcAppApp::GetCollection(IOdaMfcDocs **ppColl)
{
  return m_pDocsCollection->QueryInterface(IID_IOdaMfcDocs, (LPVOID*)ppColl);
}

HRESULT COdaMfcAppApp::AutomationFileNew(IOdaMfcDoc **ppDoc)
{
  OnFileNew();
  ASSERT(m_pLastDoc);
  return m_pLastDoc->ApcGetPrimary(ppDoc);
}

STDMETHODIMP Application::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IOdaMfcApp,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP Application::get_Application(IOdaMfcApp **pVal)
{
	return ((IDispatch*)this)->QueryInterface(IID_IOdaMfcApp, (LPVOID*)pVal);
}

STDMETHODIMP Application::get_ActiveDocument(IOdaMfcDoc **pVal)
{
	APC_DUAL_PROLOGUE
	*pVal = NULL;
	COdaMfcAppDoc* pDoc;
	CMDIChildWnd* pWnd;
	pWnd = (((CMDIFrameWnd*)pThis->m_pMainWnd)->MDIGetActive());
	if(pWnd)
	{
		pDoc = (COdaMfcAppDoc*)pWnd->GetActiveDocument();
		if(pDoc)
			pDoc->ApcGetPrimary(pVal);
		else
		{
			SetErrorInfo(IDS_ERROR_NOACTIVEDOC);
			return E_FAIL;
		}
		(*pVal)->AddRef();
	}
	else
	{
		SetErrorInfo(IDS_ERROR_NOACTIVEDOC);
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP Application::get_Documents(IOdaMfcDocs **pVal)
{
	APC_DUAL_PROLOGUE
	return pThis->GetCollection(pVal);
}

STDMETHODIMP Application::get_Name(BSTR *pVal)
{
	CString strName = AfxGetAppName();
	*pVal = strName.AllocSysString();

	return S_OK;
}

STDMETHODIMP Application::get_Parent(IOdaMfcApp **pVal)
{
	return ((IDispatch*)this)->QueryInterface(IID_IOdaMfcApp, (LPVOID*)pVal);
}

STDMETHODIMP Application::get_VBE(IDispatch **pVal)
{
	APC_DUAL_PROLOGUE
	VBIDE::VBE* pVBE = NULL;
	HRESULT hr = NOERROR;
	if(SUCCEEDED(hr = pThis->ApcHost->APC_GET(VBE)(&pVBE)))
	{
		hr = pVBE->QueryInterface(IID_IDispatch, (LPVOID*)pVal);
		pVBE->Release();
	}

	return hr;
}

STDMETHODIMP Application::Activate()
{
	APC_DUAL_PROLOGUE

	((CFrameWnd*)pThis->m_pMainWnd)->ActivateFrame();
	return S_OK;
}

STDMETHODIMP Application::Quit(MpSaveOptions Option)
{
	APC_DUAL_PROLOGUE
	HRESULT hr = NOERROR;
	long count = 0, i = 0;
	CComPtr<IOdaMfcDocs> spDocs;

	get_Documents(&spDocs);
	if(SUCCEEDED(hr = spDocs->get_Count(&count)))
	{
		//the index is always 1 because
		//close will remove it.
		VARIANT var;
		VariantInit(&var);
		V_VT(&var) = VT_I4;
		V_I4(&var) = 1;

		for(i; i < count; i++)
		{
		
			CComPtr<IOdaMfcDoc> spDoc;
			
			if(SUCCEEDED(hr = spDocs->Item(var, &spDoc)))
			{
				if(NOERROR != (hr = spDoc->Close(Option)))
					//if close return S_FALSE we shouldn't shut down
					return hr;
			}
			else
				return hr;
		}

		pThis->m_pMainWnd->PostMessage(WM_CLOSE, 0, 0);
	}
	
	return hr;
}

STDMETHODIMP Document::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IOdaMfcDoc,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP Document::get_Application(IOdaMfcApp **pVal)
{
	return theApp.ApcGetPrimary(pVal);
}

STDMETHODIMP Document::get_Name(BSTR *pVal)
{
	APC_DUAL_PROLOGUE
	CString strName = pThis->GetTitle();
	*pVal = strName.AllocSysString();
	return S_OK;
}

STDMETHODIMP Document::get_Parent(IOdaMfcApp **pVal)
{
	return theApp.ApcGetPrimary(pVal);
}

STDMETHODIMP Document::get_Collection(IOdaMfcDocs **pVal)
{
	return theApp.GetCollection(pVal);
}

STDMETHODIMP Document::get_Database(IAcadDatabase** pDatabase)
{
	APC_DUAL_PROLOGUE
  CComPtr<IOdaMfcApp> pVal; theApp.ApcGetPrimary(&pVal);
  IUnknownPtr pUnk = ::OdOxGetIUnknownOfDatabase(pThis->m_pDb, pVal);
  return pUnk->QueryInterface(IID_IAcadDatabase, (void**)pDatabase);
}

STDMETHODIMP Document::get_Saved(VARIANT_BOOL *pVal)
{
	APC_DUAL_PROLOGUE
	if(pThis->IsModified())
	{
		*pVal = VARIANT_FALSE;
	}
	else if(pThis->GetPathName() == CString(""))
	{
		*pVal = VARIANT_FALSE;
	}
	else
	{
		*pVal = VARIANT_TRUE;
	}

	return S_OK;
}

STDMETHODIMP Document::put_Saved(VARIANT_BOOL newVal)
{
	APC_DUAL_PROLOGUE
	pThis->SetModifiedFlag(newVal == VARIANT_TRUE ? FALSE : TRUE);

	return S_OK;
}

STDMETHODIMP Document::Close(MpSaveOptions Option)
{
	APC_DUAL_PROLOGUE
	if(Option == mpDoNotSaveChanges)
	{
		pThis->OnCloseDocument();
		return NOERROR;
	}
	else if(Option == mpSaveChanges)
	{
		//no prompt, just save
		if(pThis->IsModified() && !pThis->DoFileSave())
			return S_FALSE;
	}

	pThis->SaveModified();

	pThis->OnCloseDocument();

	return S_OK;
}

STDMETHODIMP Document::Activate()
{
	APC_DUAL_PROLOGUE
	POSITION pos;
	CView* pView = NULL;;
	pos = pThis->GetFirstViewPosition();
	if(pos)
	{
		pView = pThis->GetNextView(pos);
		if(pView)
		{
			CMDIChildWnd* pFrame = NULL;
			pFrame = (CMDIChildWnd*)pView->GetParent();
			if(pFrame)
			{
				pFrame->MDIActivate();
				return NOERROR;
			}
		}
	}
	
	return E_FAIL;
}

STDMETHODIMP Document::Save()
{
	APC_DUAL_PROLOGUE
	pThis->DoFileSave();

	return S_OK;
}

COdaMfcAppModule _Module;

LONG COdaMfcAppModule::Unlock()
{
  AfxOleUnlockApp();
  return 0;
}

LONG COdaMfcAppModule::Lock()
{
  AfxOleLockApp();
  return 1;
}

HRESULT COdaMfcAppApp::CreateAPCHost()
{
  HRESULT hr = NOERROR;
  // Create VBA.  APC will call ApcHost.Destroy() on app exit.
  CString strAppName, strLicKey(LICENSEKEY);
  strAppName = m_pszAppName;
  CComPtr<IOdaMfcApp> spApp;
  ApcGetPrimary(&spApp);
  
  hr = ApcHost.Create(  GetMainWnd()->m_hWnd, strAppName.AllocSysString(),
    spApp,
    strLicKey.AllocSysString(),
    m_nVBALanguage);
  if(FAILED(hr))
  {
    AfxMessageBox(L"Error initializing VBA.");
    return hr;
  }
  return hr;
}

HRESULT COdaMfcAppApp::EnsureAPCObjectsCreated()
{
  CDocTemplate* pDocTemplate = NULL;
  POSITION	pos = NULL;
  COdaMfcAppDoc * pDoc = NULL;
  
  HRESULT hr = NOERROR;
  if(!ApcHost)
  {
    if(FAILED(hr = CreateAPCHost()))
      goto Error;
  }
  
  // get DocTemplate for iteration through documents	
  pos=GetFirstDocTemplatePosition();
  ASSERT(pos);
  
  pDocTemplate=GetNextDocTemplate(pos);
  ASSERT(pDocTemplate);
  
  //iterate through documents
  
  pos=pDocTemplate->GetFirstDocPosition();
  
  while (pos!=NULL) 
  {
    pDoc=reinterpret_cast<COdaMfcAppDoc*>(pDocTemplate->GetNextDoc(pos));
    if(!(pDoc->ApcProject))
    {
      hr = pDoc->ApcProject.Create(ApcHost, axProjectNormal, NULL);
      if (FAILED(hr))
      {
        goto Error;
      }
	  {
		  SCODE sc = ::StgCreateDocfile(L"C:\\temp\\stg",
			  STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_DENY_WRITE|STGM_CREATE,
			  0, &pDoc->getStorage());
	  }
      hr = pDoc->ApcProject.InitNew(pDoc->getStorage());
      if (FAILED(hr))
      {
        goto Error;
      }		
      
      // set the display name for the project 
      CString str(pDoc->GetTitle());
      pDoc->ApcProject->put_DisplayName(str.AllocSysString());
    }
  } 
  
  return hr;
Error:
  // Close all Apc projects created.
  if (pDocTemplate) 
  {
    pos=pDocTemplate->GetFirstDocPosition();
    while (pos!=NULL)
    {
      pDoc=reinterpret_cast<COdaMfcAppDoc*>(pDocTemplate->GetNextDoc(pos));
      if(pDoc->ApcProject)
        pDoc->ApcProject.Close();
    }
  }
  
  if(ApcHost)
    ApcHost.Destroy();
  
  AfxMessageBox(L"VBA Initialization Failed.");
  
  return hr;
}

CDocument* COdaMfcAppApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
  if(!(GetApp()->ApcHost) && FAILED(GetApp()->CreateAPCHost()))
    return FALSE;
  
  CDocument* pRet = CWinApp::OpenDocumentFile(lpszFileName);
  
  if(pRet)
    AddToCollection((COdaMfcAppDoc*)pRet);
  
  return pRet;
}

HRESULT COdaMfcAppApp::AddToCollection(COdaMfcAppDoc *pDoc)
{
  IOdaMfcDoc* pIDoc = NULL;
  pDoc->ApcGetPrimary(&pIDoc);
  m_pLastDoc = pDoc;
  return m_pDocsCollection->AddToCollection(pIDoc);
}

HRESULT COdaMfcAppApp::RemoveFromCollection(COdaMfcAppDoc *pDoc)
{
  CString strName = pDoc->GetTitle();
  if(pDoc == m_pLastDoc)
    m_pLastDoc = NULL;
  return  m_pDocsCollection->RemoveFromCollection(strName);
}

void COdaMfcAppApp::OnVbaIde() 
{
  CMDIChildWnd* pWnd = (((CMDIFrameWnd*)m_pMainWnd)->MDIGetActive());
  if (SUCCEEDED(EnsureAPCObjectsCreated()))
    CApcApplication<COdaMfcAppApp>::OnVbaIde();
}

#include "OdaX_i.c" 

#endif

/////////////////////////////////////////////////////////////////////////////
#ifdef _MT
#ifndef _DLL

// For MT configurations only
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
extern "C" {

  ALLOCDLL_EXPORT void* odrxAlloc(size_t s)
  {
    return ::malloc(s);
  }

  ALLOCDLL_EXPORT void* odrxRealloc(void* p, size_t new_size, size_t /*old_size*/)
  {
    return ::realloc(p, new_size);
  }

  ALLOCDLL_EXPORT void odrxFree(void* p) 
  {
    ::free(p);
  }

} // extern "C"
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#endif
#endif
