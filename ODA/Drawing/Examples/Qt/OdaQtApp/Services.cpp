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
// Services.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif
#include <QDir>
#include <QMessageBox>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QFSFileEngine>
#endif
#include <QDataStream>
#include <QHash>

#include "OdqApplication.h"
#include "OdqPropertyDialog.h"
#include "OdqAudit.h"
#include "Services.h"
#include "ExAppServicesImpl.h"
#include "PropServicesImpl.h"

#include "OdCharMapper.h"
#include "Ed/EdUserIO.h"
#include "TDVersion.h"
#include "diagnostics.h"
#include "RxDynamicModule.h"
//#include "RxDLinkerReactor.h"
#include "Gi/TtfDescriptor.h"
#include "ExUndoController.h"
#include "OdFontServices.h"
#include "ExtDbModule.h"

//////////////////////////////////////////////////////////////////////////

// was moved from ExAppServices.h because of OdDb::ProxyImage is out of Kernel now
inline OdSysVarType toRbType(const OdDb::ProxyImage&)
{
  return kRtInt32;
}

static void myTrace(const OdChar* psTrace)
{
  static QString s_qsTracePath;
  static OdSharedPtr<QFile> s_pTraceFile;
  static OdSharedPtr<QTextStream> s_pTraceStream;

  if (s_qsTracePath.isEmpty())
  {
    s_qsTracePath = QDir::toNativeSeparators(QDir::homePath() + "/ODA/trace");
    if (QDir(s_qsTracePath).exists())
    {
      s_qsTracePath = QDir::toNativeSeparators(s_qsTracePath + "/trace.txt");
      s_pTraceFile = new QFile(s_qsTracePath);
      if (!s_pTraceFile->open(QIODevice::WriteOnly | QIODevice::Text))
      {
        ODA_FAIL_ONCE();
        s_pTraceFile = NULL;
        return;
      }
      s_pTraceStream = new QTextStream(s_pTraceFile);
    }
  }

  if (s_pTraceStream.isNull())
    return;

  QString qsTrace = getIApp()->toQString(OdString(psTrace));
  (*s_pTraceStream) << qsTrace;
  (*s_pTraceStream) << endl;
  s_pTraceStream->flush();
}

//////////////////////////////////////////////////////////////////////////
//
// Unfortunately Q_ASSERT is inconvenient to debugging
// (does the abort the application - see at end of function qt_message_output)
//

inline int odq_dbg_report(const char *title, const char *msg, ...)
{
  static const int QT_BUFFER_LENGTH = 8192; // internal buffer length
  char buf[QT_BUFFER_LENGTH];
  buf[QT_BUFFER_LENGTH - 1] = '\0';
  va_list ap;
  va_start(ap, msg); // use variable arg list
  qvsnprintf(buf, QT_BUFFER_LENGTH - 1, msg, ap);
  va_end(ap);

  if (QMessageBox::Retry == QMessageBox::question(
          QApplication::activeWindow(), // qApp->activeWindow()
          QString(title), // caption
          QString(buf), // text
          QMessageBox::Retry | QMessageBox::Ignore,
          QMessageBox::Ignore))
  {
    return 1;
  }
  return 0;
}

#if defined(Q_CC_MSVC) && defined(_CRT_ERROR)

static void myAssert(const char* pExpression, const char* pFileName, int nLineNo)
{
#ifdef _DEBUG 
  OdString sFileName(pFileName), sExpression(pExpression);
  sFileName.replace('\\', '/');
  if (   sFileName.find("Drawing/Source/database/Objects/DbSortentsTable.cpp") >= 0
    //&& OdAnsiString(pExpression) == "m_handlePairs.size() == changedDraworders.size()"
      || sFileName.find("Kernel/Source/Gs/GsContainerNode.cpp") >= 0
     )
  {
    OdString sMsg;
    sMsg.format(L"ODA_ASSERT(%ls) in \"%ls\" line %d\n", sExpression.c_str(), sFileName.c_str(), nLineNo);
    myTrace(sMsg); // TODO getIConsole()->postString(sMsg);
    return;
  }

  int iReport = 0;
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  iReport = _CrtDbgReport(_CRT_ASSERT, //_CRT_ERROR
                          pFileName, nLineNo,
                          NULL, "%s\nType: %s", pExpression,
                          !qstricmp(pExpression, "Invalid Execution.")
                          ? "ODA_FAIL" : "ODA_ASSERT");
  if (iReport == 1)
    _CrtDbgBreak(); // _asm { int 3 }
#else
  iReport = odq_dbg_report("Assertion failed!",
                           "Expression: \"%s\"\nFile: \"%s\"\nLine: %d",
                           pExpression, pFileName, nLineNo);
  if (iReport == 1)
  {
    const UINT oldErrorMode = ::SetErrorMode(0);
    ::DebugBreak(); // __debugbreak();
    ::SetErrorMode(oldErrorMode);
  }
#endif
 //#if defined(ANDROID)
 // qApp->processEvents(); // hide assertion dialog
 //#endif
#endif
}

#else // elif defined(Q_OS_MAC) // defined(__GNUC__) && defined(__APPLE__)

#include <signal.h>

static void myAssert(const char* pExpression, const char* pFileName, int nLineNo)
{
  int iReport = odq_dbg_report("Assertion failed!",
                               "Expression: \"%s\"\nFile: \"%s\"\nLine: %d",
                               pExpression, pFileName, nLineNo);
  if (iReport == 1)
    ::raise(SIGTRAP);
}

#endif

static bool myCheckAssertGroup(const char* pGroup)
{
  ODA_ASSERT_VAR(if (pGroup && OdAnsiString(pGroup) == "WR" ) return false;)
  ODA_ASSERT_VAR(if (pGroup && OdAnsiString(pGroup) == "GENURBS") return false;)
  ODA_ASSERT_VAR(if (pGroup && OdAnsiString(pGroup) == "DGN") return false;)
  //ODA_ASSERT_VAR(if (pGroup && OdAnsiString(pGroup) == "TDWF") return true;)
  ODA_ASSERT_VAR(if (pGroup && OdAnsiString(pGroup) == "MLEADER") return true;)
  return !pGroup || !*pGroup; // asserts of base group are enabled only (default)
  //return true; // asserts of all groups are enabled
}

static void myGeError(OdResult res)
{
  OdDbDatabasePtr pDwgDb = OdDbDatabase::cast(getIConsole()->getCurrentDatabase());
  if (pDwgDb.isNull()) // recommended by Misha
    return;

  //odPrintConsoleString(OD_T("\n!!! Ge error: \"%s\"\n"), OdError(res).description().c_str());
  // unfortunately now implementation of odPrintConsoleString is hardcoded to system command line only

  QString qsMsg = QObject::tr("ODA Ge error: %1\n").arg(getIApp()->toQString(OdError(res).description()));

  getIConsole()->putString(qsMsg);
  getIApp()->toStatusBar(qsMsg, 400);
}

//////////////////////////////////////////////////////////////////////////

// Define an AuditInfo class that prints no errors

//class dwgAudit : public OdDbAuditInfo
//{
//  void printError(const OdString& /*strName*/,
//                  const OdString& /*strValue*/,
//                  const OdString& /*strValidation*/,
//                  const OdString& /*strDefaultValue*/)
//  {
//    //ODA_FAIL_ONCE(); // TODO
//  }
//};

#ifndef _TOOLKIT_IN_DLL_

ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(DgModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgn7IOModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnDbModuleImpl);
//
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(BitmapModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(TD_IdViewObjModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPsPlotStyleServicesImpl);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdPsPlotStyleServicesImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdqPlotSettingsValidatorModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(RasterExportModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdGripPointsModule);
//ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdqPropSystemModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(GLES2Module);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdqViewSystemModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdqConsoleModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdDgnDbModuleImpl);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ModelerModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(OdRecomputeDimBlockModule);
ODRX_DECLARE_STATIC_MODULE_ENTRY_POINT(ExFieldEvaluatorModule);

#if !defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
#include "AECAppStaticModules.h"
ODRX_DECLARE_ARCH_STATIC_MODULES_ENTRY_POINTS()
#endif

ODRX_BEGIN_STATIC_MODULE_MAP()
  ODRX_DEFINE_STATIC_APPMODULE(L"TG_ModelerGeometry", DgModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Dgn7IO", OdDgn7IOModuleImpl)
  ODRX_DEFINE_STATIC_APPLICATION(L"TG_Db", OdDgnModule)
  ODRX_DEFINE_STATIC_APPLICATION(L"TD_DgnDb", OdDgnDbModuleImpl)
  //
  //ODRX_DEFINE_STATIC_APPMODULE(OdWinBitmapModuleName, BitmapModule)
  //ODRX_DEFINE_STATIC_APPLICATION(OdAcIdViewObjModuleName, TD_IdViewObjModule)
  //ODRX_DEFINE_STATIC_APPLICATION(ODPS_PLOTSTYLE_SERVICES_APPNAME, OdPsPlotStyleServicesImpl)
  ODRX_DEFINE_STATIC_APPMODULE(L"OdaQtPlotSettingsValidator.dll", OdqPlotSettingsValidatorModule)
  //ODRX_DEFINE_STATIC_APPLICATION(OdRasterExportModuleName, RasterExportModule)
  //ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  //ODRX_DEFINE_STATIC_APPLICATION(OdGripPointsModuleName, OdGripPointsModule)
  //ODRX_DEFINE_STATIC_APPMODULE(L"OdaQtPropSystem.dll", OdqPropSystemModule)
  ODRX_DEFINE_STATIC_APPMODULE(OdQtGLES2ModuleName, GLES2Module)
  ODRX_DEFINE_STATIC_APPMODULE(L"OdaQtViewSystem.dll", OdqViewSystemModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"OdaQtConsole.dll", OdqConsoleModule)
  ODRX_DEFINE_STATIC_APPMODULE(L"TD_DgnDb.tx", OdDgnDbModuleImpl)
  ODRX_DEFINE_STATIC_APPLICATION(OdModelerGeometryModuleName, ModelerModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdRecomputeDimBlockModuleName, OdRecomputeDimBlockModule)
  ODRX_DEFINE_STATIC_APPLICATION(OdExFieldEvaluatorModuleName, ExFieldEvaluatorModule)
#if !defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
  ODRX_DEFINE_ARCH_STATIC_APPMODULES()
#endif
ODRX_END_STATIC_MODULE_MAP()

#endif

OdqServices::OdqServices()
  : m_bInitialized(false)
  //, m_bBusy(true)
  //, m_bStopRequest(false)
  //, m_bSilent(false)
  //, m_bAudit(false)
  //, m_typeSaveAs(OdDb::kDwg)
  //, m_verSaveAs(OdDb::vAC24)
{
}

OdStringArray OdqServices::resetRegVariables() // Restore default values of registry variables
{
  OdStringArray vars;
  
  #define REGVAR_DEF(type, name, def_val, unused4, unused5) \
    if (m_##name != def_val) \
    { \
      m_##name = def_val; \
      vars.push_back(OD_T(#name)); \
    }
  
  #include "SysVarDefs.h"
  
  #undef REGVAR_DEF

  return vars;
}

OdqServices::~OdqServices() 
{
  uninit();
}

//class OdqDynLinkerBridgeReactor : public OdRxObjectImpl<OdRxDLinkerReactor>
//{
//  OdqDynLinkerBridgeReactor()
//  {
//  }
//public:
//  virtual ~OdqDynLinkerBridgeReactor()
//  {
//  }
//  // Creates an instance
//  static OdSmartPtr<OdRxDLinkerReactor> createObject();
//
//  // TODO : update m_sModuleName via next overloads
//  //virtual void rxAppWillBeLoaded(const OdString &sDrxModulePathName);
//
//  virtual void rxAppLoaded(OdRxModule* pModule);
//  //virtual void rxAppUnloaded(const OdString& sModuleName);
//};
//// Creates an instance
//// static
//OdSmartPtr<OdRxDLinkerReactor> OdqDynLinkerBridgeReactor::createObject()
//{
//  OdSmartPtr<OdRxDLinkerReactor> pReactor;
//  pReactor.attach(new OdqDynLinkerBridgeReactor()); // without addRef
//  return pReactor;
//}
//void OdqDynLinkerBridgeReactor::rxAppLoaded(OdRxModule* pModule)
//{
//  static_cast<OdqServices*>(getIApp()->getBaseHostAppServices())->updateExtDbModuleNames(pModule);
//}
//static OdSmartPtr<OdRxDLinkerReactor> s_pDynLinkerBridgeReactor;

///

//#define _TEST_ODINITIALIZE // test for double initializing
#ifdef _TEST_ODINITIALIZE
#include "GiContextForDbDatabase.h"
#include "ColorMapping.h"
#include "Gs/GsBaseVectorizer.h"
#endif

void OdqServices::init()
{
  ::odSetAssertFunc(myAssert);
  ::odSetCheckAssertGroupFunc(myCheckAssertGroup);
  OdGeContext::gErrorFunc = myGeError;
  ::odSetTraceFunc(myTrace);

  ODA_ASSERT(!m_bInitialized);

#ifndef _TOOLKIT_IN_DLL_
  ODRX_INIT_STATIC_MODULE_MAP();
#endif
  // Initialize the Drawings SDK

  if (systemCodePage() == CP_UNDEFINED)
    setSystemCodePage(CP_ANSI_1251); // Linux way to block useless assert in odInitializeDbCore(...)
  ::odInitialize(this);

#if !defined(Q_OS_WIN) && !defined(Q_WS_WIN) // !defined(_MSC_VER)
  /********************************************************************/
  /* Find the data file and and initialize the character mapper       */
  /********************************************************************/
  OdString iniFile = findFile(OD_T("adinit.dat"));
  if (!iniFile.isEmpty())
    OdCharMapper::initialize(iniFile);
#endif

#ifdef _TEST_ODINITIALIZE
  {
    OdDbDatabasePtr pDb;
    OdString sTstFile = findFile(OD_T("/_Oda/data/_bugs/jap/08.dwg"));
    if (!sTstFile.isEmpty())
      pDb = readFile(sTstFile, true, false, Oda::kShareDenyNo); 
    if (!pDb.isNull())
    {
      OdGsModulePtr pGsModule = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName); // very strange name for Linux but it works
      if (pGsModule.get())
      {
        OdGsDevicePtr pGsDevice = pGsModule->createBitmapDevice();

        OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pDb);
        OdGiDefaultContextPtr pGiCtx = pDbPE->createGiContext(pDb);

        OdGiContextForDbDatabasePtr pDbCtx = OdGiContextForDbDatabase::cast(pGiCtx); 
        pDbCtx->enableGsModel(false);

        ODCOLORREF clrBackground = ODRGB(255, 255, 255);
        ODGSPALETTE palette;
        const ODCOLORREF* pColors = odcmAcadPalette(clrBackground); // odcmAcadDarkPalette() or odcmAcadLightPalette();
        palette.resize(256);
        for (int index = 0; index < 256; index++)
          palette.setAt(index, pColors[index]);
        palette[0] = clrBackground;
        pGsDevice->setLogicalPalette(palette.asArrayPtr(), 256);

        pGsDevice = pDbPE->setupActiveLayoutViews(pGsDevice, pGiCtx);

        OdGsDCRect viewportRect(OdGsDCPoint(0, 0), OdGsDCPoint(1000, 1000));
        pGsDevice->onSize(viewportRect);
        pGsDevice->update();
      }
    }
  }
  ::odUninitialize();
  ::odInitialize(this);
 #if !defined(Q_OS_WIN) && !defined(Q_WS_WIN) // !defined(_MSC_VER)
  if (!iniFile.isEmpty())
    OdCharMapper::initialize(iniFile);
 #endif
#endif // _TEST_ODINITIALIZE

  initExAppServices();

  // ----- restore registry variables -----
  ExAppServicesPtr pExAppServices = ExAppServices::cast(getIApp()->getBaseHostAppServices());
  ODA_ASSERT_ONCE(pExAppServices.get());
  OdString sBaseKey = OD_T("vars/td/");

  #define REGVAR_DEF(type, name, def_val, unused4, unused5) \
    if (!pExAppServices->readRegistryValue(sBaseKey + OD_T(#name), \
                                           toRbType(m_##name), &m_##name)) \
      m_##name = def_val;
  #include "SysVarDefs.h"
  #undef REGVAR_DEF
  // -----

  initOdPropServices();

  //s_pDynLinkerBridgeReactor = OdqDynLinkerBridgeReactor::createObject();
  //::odrxDynamicLinker()->addReactor(s_pDynLinkerBridgeReactor);

  m_bInitialized = true;
  //m_bBusy = false;
}

void OdqServices::uninit()
{
  if (!m_bInitialized)
    return;
  m_bInitialized = false;

  //::odrxDynamicLinker()->removeReactor(s_pDynLinkerBridgeReactor);
  //s_pDynLinkerBridgeReactor = NULL;

  uninitOdPropServices();

  {
    // ----- save registry variables -----
    ExAppServicesPtr pExAppServices = ExAppServices::cast(getIApp()->getBaseHostAppServices());
    ODA_ASSERT_ONCE(pExAppServices.get());
    OdString sBaseKey = OD_T("vars/td/");

    #define REGVAR_DEF(type, name, def_val, unused4, unused5) \
      pExAppServices->writeRegistryValue(sBaseKey + OD_T(#name), \
                                         toRbType(m_##name), &m_##name);
    #include "SysVarDefs.h"
    #undef REGVAR_DEF
    // -----
  }
  uninitExAppServices();

  // Uninitialize the Drawings SDK
  try
  {
    ::odUninitialize();
  }
  catch (OdError& e)
  {
    ODA_FAIL_ONCE();
    OdString sMsg = e.description();
//    sMsg += OD_T(": ");
//    sMsg += e.description();
    ODA_TRACE(sMsg.c_str());
  }
}

static QString s_qsAppTxPath;
  // support folders other than the current folder (text fonts, menus, linetypes, and hatch patterns)
static QStringList s_pathsSupportFolders;

OdRxModule* OdqServices::loadModule(const OdString & sModuleFileName, bool bSilent)
{
  OdString sModulePathName(sModuleFileName);

  if (s_qsAppTxPath.isEmpty())
  {
    s_qsAppTxPath = QDir::toNativeSeparators(QDir(qApp->applicationDirPath()).absolutePath());

    // support command-line switches (/s)
    bool isSupportParam = false;
    foreach (QString qsArg, qApp->arguments())
    {
      QString qsArgLow = qsArg.toLower();
      if (!isSupportParam && (qsArgLow.toLower() == "/s" || qsArgLow.toLower() == "-s"))
      {
        isSupportParam = true;
        continue;
      }
      if (!isSupportParam)
        continue;
      isSupportParam = false;

      if (qsArg.size() >= 2 && qsArg.startsWith('\"') && qsArg.endsWith('\"'))
        qsArg = qsArg.mid(1, qsArg.size() - 2);
      foreach (QString qsPath, qsArg.split(chPathSeparator, QString::SkipEmptyParts))
      {
        if (qsPath.startsWith('\"'))
        {
          if (qsPath.size() >= 2 && qsPath.endsWith('\"'))
            qsPath = qsPath.mid(1, qsPath.size() - 2);
          else
          {
            ODA_FAIL_ONCE();
          }
        }
        s_pathsSupportFolders << qsPath;
      }
    }

   #if defined(__APPLE__) && !defined(__IPHONE_OS_VERSION_MIN_REQUIRED)// GNU compiler MacOS X

    // redirect to destination folder (if application is executed under xCode)
    QString qsXCodeRedirectDebug = "/Examples/Qt/build/Debug",
            qsXCodeRedirectRelease = "/Examples/Qt/build/Release";
    int pos = s_qsAppTxPath.indexOf(qsXCodeRedirectDebug);
    if (pos >= 0)
    {
      s_qsAppTxPath.replace(qsXCodeRedirectDebug, "");
    }
    else if ((pos = s_qsAppTxPath.indexOf(qsXCodeRedirectRelease)) >= 0)
    {
      s_qsAppTxPath.replace(qsXCodeRedirectRelease, "");
    }

    QDir dirAppTx(s_qsAppTxPath);
    if (dirAppTx.dirName() == "MacOS") 
    {
      s_qsAppTxPath.clear();
      dirAppTx.cdUp();
      ODA_ASSERT_ONCE(dirAppTx.dirName() == "Contents");
      // find possible sub-folder in Contents folder (DEF_MODULE_EXTENSION is useless here)
      foreach (QString qsDrx, QStringList() << "tx" << "Tx" << "TX" << "drx" << "Drx" << "DRX")
      {
        if (dirAppTx.exists(qsDrx))
        {
          dirAppTx.cd(qsDrx);
          s_qsAppTxPath = dirAppTx.absolutePath();
          break;
        }
      }

      if (s_qsAppTxPath.isEmpty())
      {
        dirAppTx.cdUp();
        dirAppTx.cdUp();
        // at OdaFileConverter.app folder
        s_qsAppTxPath = dirAppTx.absolutePath();
      }

      //QMessageBox::question(
      //  QApplication::activeWindow(), 
      //  QObject::tr("OdaFileConverter"), // caption
      //  QObject::tr("qsAppDrxPath = \"%1\"").arg(s_qsAppTxPath), // text
      //  QMessageBox::Yes,
      //  QMessageBox::Yes);
    }
    else
      ODA_FAIL_ONCE();

   #endif
  }

  QString qsModulePathName = getIApp()->toQString(sModuleFileName);
 #if defined(_TOOLKIT_IN_DLL_) // !defined(ANDROID)
  if (   !s_qsAppTxPath.isEmpty() 
      && qsModulePathName.indexOf("/") < 0
      && qsModulePathName.indexOf("\\") < 0)
  {
    QDir dir(s_qsAppTxPath);
    ODA_ASSERT_ONCE(dir.exists());

    qsModulePathName = QDir::fromNativeSeparators(dir.absoluteFilePath(qsModulePathName));
    sModulePathName = getIApp()->toOdString(qsModulePathName);
  }
 #endif

  static QStringList s_pathsTxAppPath;
  QFileInfo infoModule(getIApp()->toQString(sModulePathName));
  QString qsCurrentTxAppPath = QDir::toNativeSeparators(infoModule.absolutePath());
  if (qsCurrentTxAppPath.toLower() == s_qsAppTxPath.toLower())
    qsCurrentTxAppPath.clear();

  OdRxModule* pModule = NULL;
  try
  {
#  if defined(Q_OS_WIN) || defined(Q_WS_WIN) // !defined(_MSC_VER)
    if (qsCurrentTxAppPath.isEmpty())
#  endif
      pModule = OdDbSystemServices::loadModule(sModulePathName, bSilent);
  }
  catch (OdError& err)
  {
    // it is possible if initApp contains non-silent loading of other modules
    if (!bSilent && s_pathsTxAppPath.isEmpty())
      throw err;
  }
#if defined(Q_OS_WIN) || defined(Q_WS_WIN) // !defined(_MSC_VER)
  if (!pModule)
  {  
    // For loading dependent tx-es from non-standard directories.
    // For example : loading ExDeepCloning.tx from TX SDK from separate folder
    //               (can be loaded by user to separate folder)
    //               requires to load ExCustObjs.tx from the same folder.
    if (!qsCurrentTxAppPath.isEmpty())
    {
      for (QStringList::Iterator pItr = s_pathsTxAppPath.begin(); pItr != s_pathsTxAppPath.end(); pItr++)
      {
        if (qsCurrentTxAppPath.toLower() != pItr->toLower())
          continue;

        s_pathsTxAppPath.erase(pItr);
        break;
      }
      s_pathsTxAppPath.prepend(qsCurrentTxAppPath);
    }

    foreach (QString qsTxAppPath, s_pathsTxAppPath)
    {
      bool bOnlyEnv = (qsCurrentTxAppPath.toLower() == qsTxAppPath.toLower());
      if (bOnlyEnv && !infoModule.exists())
        continue;
      QString qsModulePathName;
      if (bOnlyEnv) 
        qsModulePathName = QDir::toNativeSeparators(infoModule.absoluteFilePath());
      else
      {
        qsModulePathName = QDir::toNativeSeparators(QDir(qsTxAppPath).absoluteFilePath(infoModule.fileName()));
        if (!QFile::exists(qsModulePathName))
          continue;
      }

      qsTxAppPath = ";" + qsTxAppPath;

      QString qsPath = "PATH=";
      qsPath += getenv("PATH");

      int pos = qsPath.lastIndexOf(qsTxAppPath);
      if (pos < 0 || pos != (qsPath.size() - qsTxAppPath.size()))
      {
        QString qsPathEx = qsPath + qsTxAppPath;
#      if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        if (putenv(qsPathEx.toAscii().data()))
#      else
        if (putenv(qsPathEx.toLatin1().data()))
#      endif
        {
          ODA_FAIL_ONCE();
        }

        try
        {
          pModule = OdDbSystemServices::loadModule(getIApp()->toOdString(qsModulePathName), bSilent);
        }
        catch (OdError& err)
        {
          ODA_ASSERT_ONCE(bSilent); // it is possible if initApp contains non-silent loading of other modules
          if (!bSilent)
            throw err;
        }

#      if QT_VERSION < QT_VERSION_CHECK(5,0,0)
        if (putenv(qsPath.toAscii().data())) // restore
#      else
        if (putenv(qsPath.toLatin1().data())) // restore
#      endif
        {
          ODA_FAIL_ONCE();
        }
        if (pModule)
          break;
      }
    } // end foreach
  }
#endif
//#if defined(Q_OS_LINUX) // for Q_WS_MAC this problem was fixed via relinking
//  // local solution for renamed TG_Db.tx (to libTG_Db.so) on Linux
//  // (for Q_WS_MAC this problem was fixed via relinking)
//  // (TODO remove it after renaming TG_Db<suffix>.tx to TG_Db<suffix>.dll in v3.4.1 or later)
//  if (!pModule && qsModulePathName.indexOf("TG_Db.tx") >= 0)
//  {
//    qsModulePathName.replace("TG_Db.tx", "libTG_Db.so");
//    sModulePathName = getIApp()->toOdString(qsModulePathName);
//    pModule = OdDbSystemServices::loadModule(sModulePathName, bSilent);
//  }
//#endif
#if !defined(Q_OS_WIN) && !defined(Q_WS_WIN) // !defined(_MSC_VER)
  if (   !pModule
      && QFileInfo(qsModulePathName).fileName().indexOf("OdaQt") >= 0 
      && QFileInfo(qsModulePathName).fileName().indexOf("OdaQtGLES") < 0
      && QFileInfo(qsModulePathName).fileName().indexOf("OdaQtOpenRL") < 0)
    QMessageBox::question(
      QApplication::activeWindow(), 
      QObject::tr("OdaQtApp Warning "),
      QObject::tr("Loading of \"%1\" was failed").arg(qsModulePathName),
      QMessageBox::Yes, QMessageBox::Yes);
#endif

  if (pModule)
    updateExtDbModuleNames(pModule);

  return pModule;
}

// Notification function called to specify the maximum number of times
// the ProgressMeter object will be incremented
void OdqServices::setLimit(int max)
{
  getIApp()->setMeterLimit((unsigned int) max);
}

// Notification function called to initialize the progress meter
void OdqServices::start(const OdString& sDisplayString) // = OdString::kEmpty
{
  QString qsText = getIApp()->toQString(sDisplayString);
  getIApp()->startMeter(qsText); // show progress bar (zero level)
}

// Notification function called to reset the progress meter
void OdqServices::stop()
{
  getIApp()->stopMeter(500); // hide progress meter over ... ms
}

// Notification function called to increment the progress meter
void OdqServices::meterProgress()
{
  getIApp()->stepMeter(); // increment the progress meter
}

// Notification function called to display a warning message
void OdqServices::warning(const char* warnVisGroup, const OdString& sMsg)
{
  if (warnVisGroup && *warnVisGroup) // default implementation skips all named groups
  {
    // Known warning visibility groups :
    //   "FNTSUBST" - via font substitution functionality
    //   "SPAAUX"
    return;
  }

  QString qsMsg = QObject::tr("ODA Warning: %1\n").arg(getIApp()->toQString(sMsg));
  
  getIConsole()->putString(qsMsg);
  getIApp()->toStatusBar(qsMsg, 400);
}

bool OdqServices::getPassword(const OdString& sDwgPathName,
                              bool isXref,
                              OdPassword& password)
{
  QMainWindow* pMainWindow = getIApp()->getMainWindow();

  OdqPropertyDialog dlg(pMainWindow);
  dlg.setWindowTitle(QObject::tr("Password"));
  dlg.addWidget(new QLabel(QObject::tr("Enter password to open drawing:\n%1")
                            .arg(getIApp()->toQString(sDwgPathName))));
  QPointer<QLineEdit> pPassword = qobject_cast<QLineEdit*>(dlg.addWidget(new QLineEdit()));
  pPassword->setEchoMode(QLineEdit::Password); //QLineEdit::PasswordEchoOnEdit
  dlg.setButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  if (dlg.exec() != QDialog::Accepted || pPassword->text().isEmpty())
  {
    QMessageBox::warning(pMainWindow, 
                         QObject::tr("%1 Message").arg(getIApp()->getAppName()), // caption
                         QObject::tr("%1\nUnable to open this drawing.\n"
                                     "It is encrypted and requires a password.")
                            .arg(getIApp()->toQString(sDwgPathName)),
                         QMessageBox::Ok, QMessageBox::Ok);
    return false;
  }

  password = getIApp()->toOdString(pPassword->text().toUpper());
  // TODO //password = getIApp()->toOdString(pPassword->text());
  return true;
}

// used to find TTF font at DWG file when it's not loaded yet
//   (getIConsole()->getCurrentDatabase() is not correct here)
static OdString s_sFilePathNameInLoading, s_sFilePathNameInImporting;

inline QString getPathOfCurrentDrawing(OdDbBaseDatabase* pRxDb)
{
  if (!s_sFilePathNameInImporting.isEmpty())
    return QDir::toNativeSeparators(
                  QFileInfo(getIApp()->toQString(s_sFilePathNameInImporting))
                    .absolutePath());

  if (pRxDb)
  {
    QString qsFilePathName = getIApp()->toQString(getIConsole()->getFilePathName(pRxDb));
    OdDbBaseDatabasePEPtr pPE;
    if (qsFilePathName.isEmpty() && (pPE = OdDbBaseDatabasePE::cast(pRxDb)).get())
      qsFilePathName = getIApp()->toQString(pPE->getFilename(pRxDb)); // for .pal & .mat of DGN file
    if (!qsFilePathName.isEmpty())
      return QDir::toNativeSeparators(QFileInfo(qsFilePathName).absolutePath());
  }

  if (!s_sFilePathNameInLoading.isEmpty())
    return QDir::toNativeSeparators(
                  QFileInfo(getIApp()->toQString(s_sFilePathNameInLoading))
                    .absolutePath());
  return "";
}

inline void addPathsOfSystemTtfFonts(QStringList& lstPaths)
{
  OdStringArray aDirs;
  if (getIApp()->getBaseHostAppServices()->getSystemFontFolders(aDirs) && aDirs.size())
  {
    for (int idx = 0, sz = (int) aDirs.size(); idx < sz; idx++)
    {
      QString qsPath = getIApp()->toQString(aDirs[idx]);
      if (!qsPath.isEmpty())
        lstPaths << qsPath;
    }
    return;
  }

 #if defined(Q_OS_MAC) // Q_WS_MAC 
  lstPaths << "/Library/Fonts" << "/System/Library/Fonts";
 #endif
 #if defined(Q_OS_LINUX)
  QDir dirLnxFonts("/usr/share/fonts/truetype");
  lstPaths << dirLnxFonts.absolutePath();
  // to support ubuntu :
  foreach(QString qsSub, dirLnxFonts.entryList(  QDir::Dirs | QDir::Readable 
                                               | QDir::NoSymLinks | QDir::NoDotAndDotDot))
    lstPaths << QDir::toNativeSeparators(dirLnxFonts.absoluteFilePath(qsSub));
 #endif
 #if defined(ANDROID)
  lstPaths << "/system/fonts";
 #endif
}

OdDbDatabasePtr OdqServices::readFile(const OdString& sFileName,
                                      bool allowCPConversion, // = false
                                      bool partialLoad, // = false
                                      Oda::FileShareMode shareMode, // = Oda::kShareDenyNo
                                      const OdPassword& password) // = OdPassword()
{
  OdString sSvFilePathNameInLoading = s_sFilePathNameInLoading;
  s_sFilePathNameInLoading = sFileName;

  OdDbDatabasePtr pDwgDb;
  try
  {
    pDwgDb = ExHostAppServices::readFile(sFileName, allowCPConversion,
                                         partialLoad, shareMode, password);
  }
  catch (OdError& err)
  {
    s_sFilePathNameInLoading = sSvFilePathNameInLoading; // restore
    throw err;
  }

  s_sFilePathNameInLoading = sSvFilePathNameInLoading; // restore
  return pDwgDb;
}

// is needed to add path for findFile set
bool OdqServices::importFromFile(OdExtDbModule* pExtDb,
                                 OdEdCommandContext* pCmdCtx,
                                 const OdString& sFileName,
                                 ODCOLORREF& clrBackground)
{
  ODA_ASSERT_ONCE(pExtDb);
  if (!pExtDb)
    return false;
  OdString sSvFilePathNameInImporting = s_sFilePathNameInImporting;
  s_sFilePathNameInImporting = sFileName;
  if (OdDbDatabase::cast(pCmdCtx->baseDatabase()).get())
    s_sFilePathNameInImporting += L".dwg";
  bool bRes = false;
  try
  {
    bRes = pExtDb->importFromFile(pCmdCtx, sFileName, clrBackground);
  }
  catch (OdError& err)
  {
    s_sFilePathNameInImporting = sSvFilePathNameInImporting; // restore
    throw err;
  }
  s_sFilePathNameInImporting = sSvFilePathNameInImporting; // restore
  return bRes;
}

OdString OdqServices::findFile(const OdString& sFileName,
                               OdDbBaseDatabase* pDb, // = NULL
                               FindFileHint hint) // = kDefault
{
  if (sFileName.isEmpty())
    return sFileName;
  ODA_ASSERT_VAR(if (hint == kTrueTypeFontFile || hint == kFontFile || hint == kCompiledShapeFile))
    ODA_ASSERT(true); //brk

  OdString sPath = ExHostAppServices::findFile(sFileName, pDb, hint);
  if (!sPath.isEmpty())
  {
    if (sPath.find(L'/') < 0 && sPath.find(L'\\') < 0 && hint == kTXApplication)
    {
      // PROD-124 // problem with command: new "i" appload "xnet.dll" addsometext
      sPath = getIApp()->toOdString(QDir::current().absoluteFilePath(getIApp()->toQString(sPath)));
      sPath.replace(L'\\', L'/');
    }
    return sPath;
  }

  QStringList lstPrefixes,
              lstSuffixes;
  // Validate hint for kTXApplication
  QString qsFileName = getIApp()->toQString(sFileName);
  if (hint == kDefault || hint == kTXApplication)
  {
    int pos;
   #if defined(TD_DLL_VERSION_SUFFIX_STR) 
    if ((pos = sFileName.find(TD_DLL_VERSION_SUFFIX_STR)) > 0)
    {
      pos += OdString(TD_DLL_VERSION_SUFFIX_STR).getLength();
      if (pos == sFileName.getLength())
      {
        qsFileName += ".";
        if (hint == kDefault)
          hint = kTXApplication;
      }
    }
    else
   #endif
    {
      foreach (QString qsSuffix, QStringList() << ".dll" << ".dylib" << ".so")
      {
        if (   (pos = qsFileName.toLower().lastIndexOf(qsSuffix)) <= 0
            || (pos + qsSuffix.length()) != qsFileName.length())
          continue;
        qsFileName = qsFileName.left(++pos); // remove suffix
        if (qsSuffix != ".dll")
        {
          QString qsTst = qsFileName;
          qsTst.replace("\\", "/");
          if ((pos = qsTst.lastIndexOf("/lib")) > 0 && pos == qsTst.lastIndexOf("/"))
            qsFileName = qsFileName.left(++pos) + qsFileName.mid(pos + 3); // remove prefix
        }
        if (hint == kDefault)
          hint = kTXApplication;
        break;
      }
    }
  }

  QFileInfo info(qsFileName);
  if (!info.suffix().isEmpty())
    lstSuffixes << QString(".%1").arg(info.suffix().toLower());
  else
  {
    switch (hint)
    {
    case kTXApplication:
      lstSuffixes << ".tx";
     #if defined(TD_DLL_VERSION_SUFFIX_STR)
      lstSuffixes << (getIApp()->toQString(TD_DLL_VERSION_SUFFIX_STR) + ".tx");
      lstSuffixes << (getIApp()->toQString(TD_DLL_VERSION_SUFFIX_STR) + ".dll");
     #endif
     #if defined(Q_OS_WIN) || defined(Q_WS_WIN) // Q_OS_WIN32 _MSC_VER
      lstSuffixes << ".dll";
     #elif defined(Q_OS_MAC) // Q_WS_MAC __APPLE__
      lstSuffixes << ".dylib";
      lstPrefixes << "lib";
     #elif defined(Q_OS_LINUX)
      lstSuffixes << ".so";
      lstPrefixes << "lib";
     #endif
      break;
    case kXRefDrawing:
      lstSuffixes << ".dwg";
      break;
    case kTrueTypeFontFile:
      lstSuffixes << ".ttf" << ".ttc" << ".otf";
      break;
    case kFontFile:
    case kCompiledShapeFile:
      lstSuffixes << ".shx";
      break;
    case kPatternFile:
      lstSuffixes << ".pat";
      break;
    }
  }

  QStringList lstPaths;

  QString qsDrawingPath = getPathOfCurrentDrawing(pDb);
  if (!qsDrawingPath.isEmpty()) 
    lstPaths << qsDrawingPath;
  if (!s_qsAppTxPath.isEmpty())
    lstPaths << s_qsAppTxPath;
  if (hint != kTXApplication && !s_pathsSupportFolders.isEmpty())
    lstPaths << s_pathsSupportFolders;
  if (hint == kTrueTypeFontFile)
    addPathsOfSystemTtfFonts(lstPaths);
  if (hint == kFontFile || hint == kTrueTypeFontFile || hint == kCompiledShapeFile || hint == kPatternFile)
  {
    OdString sAcadPath = getIAppProps()->toString(OD_T("Vars/acad"), 
                                                  getIApp()->toOdString(getenv("ACAD")));
    if (!sAcadPath.isEmpty())
    {
      QStringList lst = getIApp()->toQString(sAcadPath).split(chPathSeparator, QString::SkipEmptyParts);
      if (!lst.isEmpty())
        lstPaths << lst;
    }
  }
//#if defined(Q_OS_WIN32) // && defined(_DEBUG) // !defined(__APPLE__) && !defined(Q_OS_LINUX)
  if (hint == kFontFile || hint == kCompiledShapeFile || hint == kTextureMapFile)
  {
    foreach(QString qsKey, getIApp()->getAcadRegProfileGeneralKeys())
    {
      QSettings settings(qsKey, QSettings::NativeFormat);
      QString qsPaths = settings.value((hint != kTextureMapFile) ? "ACAD" : "AVEMAPS",
                                       "").toString();
      getIApp()->substituteEnvVariables(qsPaths);
      QStringList lst = qsPaths.split(chPathSeparator, QString::SkipEmptyParts);
      if (!lst.isEmpty())
        lstPaths << lst;
    }
  }
//#endif
  if (hint == kDefault && (lstSuffixes.contains(".ctb") || lstSuffixes.contains(".stb")))
  {
    // plot style
    OdString sPathPS = getIAppProps()->toString(L"Vars/ddplotstylepaths");
    if (sPathPS.isEmpty())
        getEnvVar(L"DDPLOTSTYLEPATHS", sPathPS);
    if (sPathPS.isEmpty())
        ::odrxSystemServices()->getEnvVar(L"DDPLOTSTYLEPATHS", sPathPS); // getenv("DDPLOTSTYLEPATHS") //getenv() is a standard ISO C function from stdlib.h
    QString qsPathPS = getIApp()->toQString(sPathPS);
   //#if defined(Q_OS_WIN32) // && defined(_DEBUG) // !defined(__APPLE__) && !defined(Q_OS_LINUX)
    if (qsPathPS.isEmpty())
    {
      // try to get it via the installed ACAD on computer
      foreach(QString qsKey, getIApp()->getAcadRegProfileGeneralKeys())
      {
        QSettings settings(qsKey, QSettings::NativeFormat);
        qsPathPS = settings.value("PrinterStyleSheetDir", "").toString();
        getIApp()->substituteEnvVariables(qsPathPS); // resolve for %USERPROFILE% - critical for Windows 10
        if (!qsPathPS.isEmpty())
          break;
      }
    }
   //#endif
    QStringList lst = qsPathPS.split(chPathSeparator, QString::SkipEmptyParts);
    if (!lst.isEmpty())
      lstPaths << lst;
  }

  foreach(QString qsPath, lstPaths)
  {
    QDir dir(qsPath);
    QString qsFilePath = QDir::toNativeSeparators(dir.absoluteFilePath(info.fileName()));
    if (!QFile(qsFilePath).exists())
    {
      foreach (QString qsSuffix, lstSuffixes)
      {
        qsFilePath = QDir::toNativeSeparators(dir.absoluteFilePath(
                                                      info.completeBaseName() + qsSuffix));
        if (QFile(qsFilePath).exists())
          break;
        if (lstPrefixes.isEmpty())
          continue;

        foreach (QString qsPrefix, lstPrefixes)
        {
          qsFilePath = QDir::toNativeSeparators(dir.absoluteFilePath(
                                                        qsPrefix + info.completeBaseName() + qsSuffix));
          if (QFile(qsFilePath).exists())
            break;
        }
        if (QFile(qsFilePath).exists())
          break;
      }
    }
    if (!QFile(qsFilePath).exists())
    {
     #if defined(Q_OS_LINUX) && (QT_VERSION < QT_VERSION_CHECK(5,0,0))// !defined(Q_WS_WIN) && !defined(Q_OS_MAC)
      // try to find as CaseInsensitive on Linux
      QFSFileEngine fe(qsFilePath);
      if (fe.caseSensitive())
      {
        foreach(QString qsName, dir.entryList())
        {
          if (qsName.toLower() != info.fileName().toLower())
            continue;
          qsFilePath = QDir::toNativeSeparators(dir.absoluteFilePath(qsName));
          break;
        }
        if (!QFile(qsFilePath).exists())
          continue;
      }
      else
     #endif
        continue;
    }

    sPath = getIApp()->toOdString(qsFilePath);
    break;
  }

  if (   sPath.isEmpty()
      && (pDb || !s_sFilePathNameInLoading.isEmpty())
      && (hint == kFontFile || hint == kCompiledShapeFile)
      && getIApp()->getAcadRegProfileGeneralKeys().isEmpty()
      && info.suffix().toLower() == "shx" && info.completeBaseName().toLower() != "simplex")
  {
    //QMessageBox::warning(getIApp()->getMainWindow(),
    //                     QObject::tr("%1 Warning").arg(getIApp()->getAppName()), // caption
    //                     QObject::tr("via font : %1").arg(getIApp()->toQString(sFileName)),
    //                     QMessageBox::Ok, QMessageBox::Ok);

    OdString sAcadPath;
    if (eOk == getEnv(OD_T("ACAD"), sAcadPath) && sAcadPath.isEmpty())
    {
      sAcadPath = getIAppProps()->toString(OD_T("Vars/acad"), OD_T("<undefined>"));
      if (sAcadPath == OD_T("<undefined>"))
      {
#if 0
        getIAppProps()->set(OD_T("Vars/acad"), OdString::kEmpty, OD_T("folder"));
        ODA_ASSERT_ONCE(getIAppProps()->toString(OD_T("Vars/acad"), OD_T("<undefined>")).isEmpty());
        QMessageBox::warning(getIApp()->getMainWindow(),
                             QObject::tr("%1 Warning").arg(getIApp()->getAppName()), // caption
                             QObject::tr("You should define environment variable ACAD with paths to your SHX fonts."
                                         "\nYou can type \"ACAD DIALOG\" in console for it."),
                             QMessageBox::Ok, QMessageBox::Ok);
     
#endif
      }
    }
  }

  return sPath;
}

//////////////////////////////////////////////////////////////////////////

inline QString getSysFontKey(const QString& qsTypeface, bool isBold, bool isItalic)
{
  ODA_ASSERT_ONCE(!qsTypeface.isEmpty());
  QString qsKey = qsTypeface.toLower();
  if (!isBold && !isItalic && !qsTypeface.isEmpty())
    qsKey = QString("<%1%2>/%3").arg(isBold ? "b" : "")
                                .arg(isItalic ? "i" : "")
                                .arg(qsKey);
  return qsKey;
}

bool OdqServices::ttfFileNameByDescriptor(const OdTtfDescriptor& description, 
                                          OdString& sFileName)
{
  bool bRes = false;
//#if defined(_DEBUG)
//  if (description.typeface().isEmpty()) // see ODA_COLLECT_TTF_FONTS_IN_DRAWING_FOLDER 
//    bRes = bRes; // brk
//#endif
  bRes = ExHostAppServices::ttfFileNameByDescriptor(description, sFileName);

// CORE-11366 //#if defined(Q_OS_WIN) || defined(Q_WS_WIN) // is obsolete for Q_OS_MAC and Q_OS_LINUX
#if !defined(Q_OS_WIN) && !defined(Q_WS_WIN) 
  if (s_sFilePathNameInLoading.isEmpty() && s_sFilePathNameInImporting.isEmpty())
    return bRes; // is obsolete for Q_OS_MAC and Q_OS_LINUX except import case (via findFile arguments)
#endif

  if (!bRes && sFileName.isEmpty())
  {
    OdString sPath = description.fileName();
    OdDbBaseDatabase* pRxDb = NULL;
    if (s_sFilePathNameInLoading.isEmpty() && s_sFilePathNameInImporting.isEmpty() && getIConsole())
      pRxDb = getIConsole()->getCurrentDatabase();

    if (!sPath.isEmpty())
    {
      sPath = findFile(sPath, pRxDb, kTrueTypeFontFile);
      if (!sPath.isEmpty())
      {
        sFileName = sPath;
        return true;
      }

      sPath = description.fileName();
      if (sPath.replace(OD_T(" "), OD_T("")))
      {
        sPath = findFile(sPath, pRxDb, kTrueTypeFontFile);
        if (!sPath.isEmpty())
        {
          sFileName = sPath;
          return true;
        }
      }
    }

    QString qsTypeface = getIApp()->toQString(description.typeface());
    if (!qsTypeface.isEmpty())
    {
      static QHash<QString, QString> s_mapCacheSysFonts; // <by [bi/]qsTypeface, FilePathName>
      static QString s_qsLastDrawingFolder;

      QStringList lstPaths;
      if (s_qsLastDrawingFolder.isEmpty())
        addPathsOfSystemTtfFonts(lstPaths);

      QString qsDrawingPath = getPathOfCurrentDrawing(pRxDb);
      if (!qsDrawingPath.isEmpty())
      {
        s_qsLastDrawingFolder = qsDrawingPath;
        lstPaths << qsDrawingPath;
      }

      OdFontServicesPtr pFontServices = ::odrxSysRegistry()->getAt(ODDB_FONT_SERVICES);
      if (!pFontServices.get())
        return false;

      //int cntDirs = -1;
      foreach(QString qsPath, lstPaths)
      {
        //cntDirs++;
        QDir dir(qsPath);

        foreach(QString qsTtfFileName, dir.entryList(  QDir::Files | QDir::Readable 
                                                     | QDir::NoSymLinks | QDir::NoDotAndDotDot))
        {
          qsTtfFileName = QDir::toNativeSeparators(dir.absoluteFilePath(qsTtfFileName));
          QString qsExt = QFileInfo(qsTtfFileName).suffix().toLower();
          if (qsExt != "ttf" && qsExt != "ttc" && qsExt != "otf")
            continue;

          OdString sTtfFileName = getIApp()->toOdString(qsTtfFileName);

          OdTtfDescriptor descr;
          // see also getTrueTypeFontParams
          if (!pFontServices->getTTFParamFromFile(sTtfFileName, descr))
          {
            //ODA_FAIL_ONCE();
            continue;
          }
          QStringList lstTypefaces = getIApp()->toQString(descr.typeface()).split(" & ", QString::SkipEmptyParts);
          ODA_ASSERT_ONCE(lstTypefaces.size());

          bool bBold = (lstTypefaces.size() == 1) ? descr.isBold() : false,
               bItalic = (lstTypefaces.size() == 1) ? descr.isItalic() : false;

          foreach(QString qsKey, lstTypefaces)
          {
            //qsKey = qsKey.toLower();
            //if (cntDirs)
            //{
              // add to cache
              qsKey = getSysFontKey(qsKey, bBold, bItalic);
              s_mapCacheSysFonts[qsKey] = qsTtfFileName;
            //  continue;
            //}
            //if (qsKey != qsTypeface.toLower())
            //  continue;
            //sFileName = getIApp()->toOdString(qsTtfFileName);
            //return true;
          }
        }
      }
      if (!s_mapCacheSysFonts.isEmpty())
      {
        QStringList qsKeysToCheck;
        qsKeysToCheck << getSysFontKey(qsTypeface, description.isBold(), description.isItalic());
        qsKeysToCheck << getSysFontKey(qsTypeface, description.isBold(), !description.isItalic());
        qsKeysToCheck << getSysFontKey(qsTypeface, !description.isBold(), description.isItalic());
        qsKeysToCheck << getSysFontKey(qsTypeface, !description.isBold(), !description.isItalic());
        foreach (QString qsKey, qsKeysToCheck)
        {
          if (qsKey.isEmpty())
            continue;
          QString qsFileName = s_mapCacheSysFonts[qsKey];
          if (qsFileName.isEmpty())
            continue;

          sFileName = getIApp()->toOdString(qsFileName);
          return true;
        }
      }
    }
  }
//#endif // defined(Q_OS_WIN) || defined(Q_WS_WIN) // is obsolete for Q_OS_MAC and Q_OS_LINUX

  return bRes;
}

//OdCodePageId OdqServices::systemCodePage() const
//{
//  OdCodePageId page = ExSystemServices::systemCodePage();
////#ifdef __APPLE__
////  // test
////  if (page == CP_UNDEFINED)
////    page = CP_MACINTOSH;
////#endif
//  return page;
//}

// Show dialog to get file path from user
//
// flags must be one of the following:
//   kGfpForOpen   0       For Open.
//   kGfpForSave   1       For Save.
//
// default implementation of base class OdDbHostAppServices return "*unsupported*"

OdString OdqServices::fileDialog(int flags,
                                 const OdString& sDialogCaption, // = OdString::kEmpty
                                 const OdString& sDefExt, // = OdString::kEmpty
                                 const OdString& sDefFileName, // = OdString::kEmpty
                                 const OdString& sFilter) // = OdString::kEmpty
{
  QFileDialog dlg(getIApp()->getMainWindow());

  bool bForSave = ((flags & OdEd::kGfpForSave) != 0);
  if (bForSave)
  {
    dlg.setFileMode(QFileDialog::AnyFile); 
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setConfirmOverwrite(true);
  }
  else
  {
    // TODO via PreviewFileDialog
    ODA_ASSERT_ONCE(flags == OdEd::kGfpForOpen);
    dlg.setFileMode(QFileDialog::ExistingFile); 
  }

#ifdef ANDROID
  if (QApplication::activeWindow())
  {
    QSize baseSize = QApplication::activeWindow()->size();
    if (baseSize.width() > 0 && baseSize.height() > 0)
    {
      dlg.resize(baseSize.width(), baseSize.height());
    }
  }
#endif
  QString qsDialogCaption = getIApp()->toQString(sDialogCaption);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  dlg.setWindowTitle(QObject::tr(qsDialogCaption.toAscii().data())); // "Select File"
#else
  dlg.setWindowTitle(QObject::tr(qsDialogCaption.toLatin1().data())); // "Select File"
#endif

  QSettings settings("ODA", 
                     QFileInfo(QCoreApplication::applicationFilePath()).baseName());
  settings.beginGroup(TD_SHORT_STRING_VER_S "/recent");

  QString qsSelectedFilter;
  bool bDefExtAndReturnAsCurrFilter = false;
  if (bForSave)
  {
    int pos = sDefExt.find(OD_T("|"));
    if (pos >= 0)
    {
      ODA_ASSERT_ONCE(pos > 0);
      bDefExtAndReturnAsCurrFilter = true;
      qsSelectedFilter = getIApp()->toQString(sDefExt);
      qsSelectedFilter = qsSelectedFilter.left(pos);
    }
  }

  QString qsFilters = getIApp()->toQString(sFilter);
  qsFilters.replace(',', ' '); // problem with CreateLinkedTable & others
  if (qsFilters.isEmpty()) // support for NETLOAD
    qsFilters = QObject::tr("All files (*.*)|*.*||"); // default filter

  QStringList flts = QString(qsFilters)
                         .replace("||", "").split("|", QString::SkipEmptyParts),
              filters;
  int index = 0;
  QString qssDefExtAsFlt;
  if (bForSave)
    qssDefExtAsFlt = QString("*.%1").arg(getIApp()->toQString(sDefExt));
  foreach (QString qsFlt, flts)
  {
    if (index++ % 2)
    {
      ODA_ASSERT_ONCE(flts.at(index - 1).contains(qsFlt));
      continue;
    }
    if (bForSave && qsFlt.count(';'))
    {
      // QFileDialog (differ to mfc dialog) take last extension from (*.first;...;*.last)
      // reverse the order of extensions as the simplest workaround for this problem
      int pos = qsFlt.indexOf("(*");
      if (pos > 0)
      {
        pos++;
        QString qsPref = qsFlt.left(pos),
                qsExtensions = qsFlt.mid(pos).remove(' ');
        pos = qsExtensions.lastIndexOf(")");
        if (pos == (qsExtensions.length() - 1))
        {
          QStringList exts = qsExtensions.left(pos).split(';', QString::SkipEmptyParts);
          qsExtensions.clear();
          QStringList::iterator itr = exts.end();
          do 
          {
            QString qsExtn = *--itr;
            if (!qsExtensions.isEmpty())
              qsExtensions += ';';
            qsExtensions += qsExtn;
          }
          while (itr != exts.begin());
          qsFlt = qsPref + qsExtensions + ")";
        }
        else
        {
          ODA_FAIL_ONCE();
        }
      }
    }
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    qsFlt = QObject::tr(qsFlt.toAscii().data());
#else
    qsFlt = QObject::tr(qsFlt.toLatin1().data());
#endif
    filters << qsFlt;

    if (   qsSelectedFilter.isEmpty() && !qssDefExtAsFlt.isEmpty() 
        && qsFlt.indexOf(qssDefExtAsFlt) > 0)
      qsSelectedFilter = qsFlt;
  }
  if (!filters.count())
  {
    ODA_FAIL_ONCE();
   #ifdef __APPLE__ // MacOS X
    return OdString::kEmpty;
   #else
    FIX_MAC_APP_TROW(OdError(eNotApplicable));// without real filter
   #endif
  }

  dlg.setNameFilters(filters); // dlg.setFilters(filters);

  QString qsFltKey = "filter+" + qsDialogCaption;
  qsFltKey.remove(' ').remove(':').remove(':')
          .remove('[').remove(']').remove('<').remove('>')
          .remove('\"').remove('\'').remove('\\').remove('/');

  bool wasFltKeyRead = true;
  if (qsSelectedFilter.isEmpty() || !filters.contains(qsSelectedFilter))
  {
    qsSelectedFilter = settings.value(qsFltKey).toString();
    if (qsSelectedFilter.isEmpty())
    {
      wasFltKeyRead = false;
      qsSelectedFilter = bForSave ? filters.first() : filters.last();
    }
  }
  dlg.selectNameFilter(qsSelectedFilter); // dlg.selectFilter(qsSelectedFilter);

  QString qsSelectedExt = getIApp()->getExtensionByFilter(qsSelectedFilter); //  = getIApp()->toQString(sDefExt);
  dlg.setDefaultSuffix(qsSelectedExt);
  qsSelectedExt = getIApp()->getExtensionByFilter(qsSelectedFilter, "+"); 
  if (qsSelectedExt.isEmpty())
    qsSelectedExt = "all";
  
  QString qsRecentPath = settings.value(QString("%1/path").arg(qsSelectedExt)).toString(),
          qsLastPath = settings.value("lastpath", getIApp()->getInitialDocumentsPath()).toString();
  if (qsRecentPath.isEmpty() && bForSave && !wasFltKeyRead)
  {
    QString qsFltForOpen = settings.value(qsFltKey, filters.last()).toString();
    QString qsExtForOpen = getIApp()->getExtensionByFilter(qsFltForOpen, "+");
    qsRecentPath = settings.value(QString("%1/path").arg(qsExtForOpen)).toString();
  }
  if (qsRecentPath.isEmpty())
    qsRecentPath = qsLastPath;
  dlg.setDirectory(qsRecentPath);

  QString qsDefFileName = getIApp()->toQString(sDefFileName);
  if (!qsDefFileName.isEmpty())
  {
    qsDefFileName.replace("\\", "/");
    if (qsDefFileName.indexOf("/") >= 0)
      dlg.setDirectory(QFileInfo(qsDefFileName).path());
    dlg.selectFile(qsDefFileName);
  }

  QDialog::DialogCode code = (QDialog::DialogCode) dlg.exec(); // execute as modal
  QStringList files = dlg.selectedFiles();
  QString qsFilePathName;
  if (   code != QDialog::Accepted
      || files.size() != 1 || (qsFilePathName = files.first()).isEmpty())
  {
    ODA_ASSERT_ONCE(code != QDialog::Accepted);
   #ifdef __APPLE__ // MacOS X
    return OdString::kEmpty;
   #else
    FIX_MAC_APP_TROW(OdEdCancel()); // like COdaMfcAppApp::fileDialog etc // return OD_T("*canceled*");
   #endif
  }

  // save current extension and path via QSettings
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
  QString selFilter = dlg.selectedFilter();
#else
  QString selFilter = dlg.selectedNameFilter();
#endif
  if (!wasFltKeyRead || qsSelectedFilter != selFilter)
    settings.setValue(qsFltKey, selFilter);

  QFileInfo info(qsFilePathName);
  if (   qsSelectedFilter != selFilter
      || qsRecentPath != info.path())
  {
    qsSelectedExt = getIApp()->getExtensionByFilter(selFilter, "+"); 
    if (qsSelectedExt.isEmpty())
      qsSelectedExt = "all";

    settings.setValue(QString("%1/path").arg(qsSelectedExt), info.path());
  }
  if (info.path() != qsLastPath)
    settings.setValue("lastpath", info.path());
  
  OdString sFilePathName = getIApp()->toOdString(QDir::toNativeSeparators(info.canonicalFilePath()));
  if (bForSave && sFilePathName.isEmpty()) // info.canonicalFilePath() return empty if file does not exist
    sFilePathName = getIApp()->toOdString(info.absoluteFilePath());

  if (bDefExtAndReturnAsCurrFilter)
  {
    ODA_ASSERT_ONCE(bForSave && !dlg.selectedNameFilter().isEmpty());

    qsSelectedFilter.clear();
    index = 0;
    foreach (QString qsFlt, flts)
    {
      if (index++ % 2)
      {
        ODA_ASSERT_ONCE(flts.at(index - 1).contains(qsFlt));
        if (qsSelectedFilter.isEmpty())
          continue;

        qsSelectedFilter += "|";
        qsSelectedFilter += qsFlt;
        qsSelectedFilter += "|";
        break;
      }
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
      if (dlg.selectedNameFilter() == QObject::tr(qsFlt.toAscii().data()))
#else
      if (dlg.selectedNameFilter() == QObject::tr(qsFlt.toLatin1().data()))
#endif
        qsSelectedFilter = qsFlt;
    }

    sFilePathName += OD_T("|");
    sFilePathName += getIApp()->toOdString(qsSelectedFilter);
  }

  return sFilePathName;
}

OdGsDevicePtr OdqServices::gsBitmapDevice(OdRxObject* pViewObj, // = NULL
                                          OdDbBaseDatabase* pRxDb, // = NULL
                                          OdUInt32 flags) // = 0
{
  OdGsModulePtr pGsModule;
  // possible TODO via OdQtGLES2ModuleName, OdQtOpenGLModuleName 
  if (getIAppProps()->toBool(OD_T("Vars/gles"), true))
    pGsModule = ::odrxDynamicLinker()->loadModule(OdWinGLES2ModuleName);
  if (pGsModule.isNull())
    pGsModule = ::odrxDynamicLinker()->loadModule(OdWinOpenGLModuleName);
  if (pGsModule.isNull())
    pGsModule = ::odrxDynamicLinker()->loadModule(OdWinBitmapModuleName);
  OdGsDevicePtr pBmpDev;
  if (pGsModule.get())
    pBmpDev = pGsModule->createBitmapDevice();
  if (pBmpDev.isNull())
    pBmpDev = ExHostAppServices::gsBitmapDevice(pViewObj, pRxDb, flags);
  return pBmpDev;
}

OdDbUndoControllerPtr OdqServices::newUndoController()
{
  return OdRxObjectImpl<ExUndoController>::createObject();
}

void OdqServices::auditPrintReport(OdAuditInfo* pAuditInfo, 
                                   const OdString& sLine, 
                                   int nPrintDest) const
{
  // TODO via inheritance from OdRxObject
  //OdqAuditInfoPtr pAudit = OdqAuditInfo::cast(pAuditInfo);
  //if (pAudit.isNull()) return;
  OdqAuditInfo* pAudit = (OdqAuditInfo *) pAuditInfo;
  OdRxObject* pRxDatabase = pAudit->baseDatabase();
  ODA_ASSERT_ONCE(pRxDatabase);

  OdString sMsg;
  sMsg = OD_T("  ");
  sMsg += pAuditInfo->getLastInfo().strName;
  if (pAuditInfo->getLastInfo().bIsError)
  {
    int nErr = pAudit->numErrors();
    if (!nErr)
    {
      // Title
      OdString sTitle = OD_T("  Name\t\t\tValue\t\tValidation\tDefault value");
      getIConsole()->putString(sTitle, pRxDatabase);
    }

    sMsg += OD_T("\t");
    sMsg += pAuditInfo->getLastInfo().strValue;
    sMsg += OD_T("\t");
    sMsg += pAuditInfo->getLastInfo().strValidation;
    sMsg += OD_T("\t");
    sMsg += pAuditInfo->getLastInfo().strDefaultValue;
  }
  getIConsole()->putString(sMsg, pRxDatabase);
}

OdDbPlotSettingsValidator* OdqServices::plotSettingsValidator()
{
  if (m_pValidator.isNull())  
  {
    OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(
                                OD_T("OdaQtPlotSettingsValidator.dll"));
   #if defined(Q_OS_WIN32) // !defined(__APPLE__) && !defined(Q_OS_LINUX)
    // try to use non-portable version
    if (pModule.isNull())
      pModule = ::odrxDynamicLinker()->loadModule(OdPlotSettingsValidatorModuleName);
   #endif
    if (pModule.isNull())
    {
      static bool bFirst = true;
      if (!bFirst)
        return NULL;
      bFirst = false;

      ODA_FAIL_ONCE();
      getIConsole()->putString(OD_T("OdaQtPlotSettingsValidator was not found"));
    }
  }

  return ExHostAppServices::plotSettingsValidator();
}

#if defined(_TOOLKIT_IN_DLL_)

static bool isBridgeModule(const QString& qsFileName, // in
                           OdString& sName, OdString& sFileName) // out
{
  QString qsVersionSuffix;
#ifdef TD_DLL_VERSION_SUFFIX_STR
  qsVersionSuffix = getIApp()->toQString(TD_DLL_VERSION_SUFFIX_STR);
#endif
  QString qsWildcard = "TD_*Db" + qsVersionSuffix;
  QRegExp rxExp(qsWildcard, Qt::CaseSensitive, QRegExp::Wildcard);

  QFileInfo info(qsFileName);
  if (info.suffix().toLower() != "tx")
    return false;
  sFileName = getIApp()->toOdString(qsFileName);
  QString qsName = info.completeBaseName();
  if (!rxExp.exactMatch(qsName))
    return false;
  int pos = qsName.lastIndexOf("Db" + qsVersionSuffix);
  if (pos <= 3)
    return false;
  if (!qsVersionSuffix.isEmpty())
  {
    if ((pos = qsName.indexOf('_', pos)) <= 0)
      return false;
    qsName = qsName.left(pos);
  }
  qsName += ".tx";
  sName = getIApp()->toOdString(qsName);

  if (sName.isEmpty() || sFileName.isEmpty())
  {
    ODA_FAIL_ONCE();
    return false;
  }
  return true;
}

OdStringArray OdqServices::getExtDbModuleNames(OdRxObject*, // pRxDb // = NULL
                                               bool bNamesOnly) // = true
{
  if (m_namesExtDb.isEmpty())
  {
    m_pathsExtDb.clear();

    QStringList lstPaths;
    if (!s_qsAppTxPath.isEmpty())
      lstPaths << s_qsAppTxPath;
    if (!s_pathsSupportFolders.isEmpty())
      lstPaths << s_pathsSupportFolders;
    // usually is useless adn cannot be cached
    //QString qsDrawingPath = getPathOfCurrentDrawing(pRxDb);
    //if (!qsDrawingPath.isEmpty()) 
    //  lstPaths << qsDrawingPath;

    foreach(QString qsPath, lstPaths)
    {
      QDir dir(qsPath);
      foreach(QString qsFileName, dir.entryList(  QDir::Files | QDir::Readable 
                                                | QDir::NoSymLinks | QDir::NoDotAndDotDot))
      {
        qsFileName = QDir::toNativeSeparators(dir.absoluteFilePath(qsFileName));

        OdString sName, sFileName;
        if (!isBridgeModule(qsFileName, sName, sFileName))
          continue;
        if (m_namesExtDb.contains(sName))
        {
          ODA_FAIL_ONCE(); // it's strange
          continue;
        }
        OdRxModulePtr pModule = ::odrxDynamicLinker()->loadModule(sFileName);
        if (pModule.isNull())
          continue;
        if (!OdExtDbModule_cast(pModule)->isValid())
        {
          pModule = NULL;
          if (!::odrxDynamicLinker()->unloadModule(sFileName))
          {
            ODA_FAIL_ONCE();
          }
          continue;
        }

        if (!m_namesExtDb.contains(sName))
          m_namesExtDb.push_back(sName);
        if (!m_pathsExtDb.contains(sFileName))
          m_pathsExtDb.push_back(sFileName);

      } // foreach
    } // foreach
  } // if
  return bNamesOnly ? m_namesExtDb : m_pathsExtDb;
}

void OdqServices::updateExtDbModuleNames(OdRxModule* pModule)
{
  OdString sModulePath = pModule->moduleName();
  QString qsFileName = getIApp()->toQString(sModulePath);
  qsFileName = QDir::toNativeSeparators(qsFileName);

  OdString sName, sFileName;
  if (!isBridgeModule(qsFileName, sName, sFileName))
    return;
  if (m_namesExtDb.contains(sName))
    return;
  ODA_ASSERT_ONCE(!m_pathsExtDb.contains(sFileName));

  m_namesExtDb.push_back(sName);
  m_pathsExtDb.push_back(sFileName);
}

#else

OdStringArray OdqServices::getExtDbModuleNames(OdRxObject*, bool) // = true
{
  if (m_namesExtDb.isEmpty())
  {
    m_namesExtDb.push_back(L"TD_DgnDb.tx");
    m_namesExtDb.push_back(L"TD_DwfDb.tx");
    m_namesExtDb.push_back(L"TD_ImgDb.tx");
  }
  return m_namesExtDb;
}

void OdqServices::updateExtDbModuleNames(OdRxModule*)
{
}

#endif

