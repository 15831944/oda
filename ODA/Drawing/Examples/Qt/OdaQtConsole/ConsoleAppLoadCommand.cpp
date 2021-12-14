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
// ConsoleAppLoadCommand.cpp
//

#include "OdaCommon.h"
#include "Ed/EdCommandContext.h"
#include "Ed/EdUserIO.h"
#include "Ed/EdCommandStack.h" 
#include "ExStringIO.h"
#include "DbHostAppServices.h"
#include "TDVersion.h"

#include "ConsoleModule.h"
#include "PropServices.h"
#include "ExAppServices.h"

#if defined(_TOOLKIT_IN_DLL_) && defined(__GNUC__) && defined(__APPLE__)
#include <dlfcn.h>
#endif

//#include <QtCore> // for QT_VERSION only
//#if QT_VERSION >= QT_VERSION_CHECK(5,0,0) && defined(__linux__)
//#  define QFILEDIALOG_IN_LAYOUT_UNSUPPORTED // now fixed in constructor of OdqPropControlFileDialog
//#endif

//------------------------------------------------------------------------

OdDbHostAppServices* getServices(OdEdCommandContext* pCmdCtx)
{
  OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());

  OdDbHostAppServices* pServices = OdDbHostAppServices::cast(pPropServices->getBaseHostAppServices()).get();
  return pServices;
}

// return error message
static OdString loadApp(const OdString& csPath, OdEdCommandContext* pCmdCtx)
{
  OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());

  if (csPath.isEmpty())
    return pPropServices->tr("An empty application path to load.");

  if (::odrxDynamicLinker()->loadModule(csPath, true).get())
    return OdString::kEmpty; // ok

// already done in OdqServices::loadModule
//#if defined(_MSC_VER)
//  {
//    OdString sDllPath = csPath; 
//    sDllPath.replace(L'/', L'\\');
//    int pos = sDllPath.reverseFind(L'\\');
//    if (pos > 0)
//    {
//      sDllPath = sDllPath.left(pos);
//      OdString sSvPath = _wgetenv(L"PATH");
//      if (sSvPath.find(sDllPath) < 0)
//      {
//        OdString sPath = (!sSvPath.isEmpty() ? (sSvPath + L";") : L"");
//        sPath += sDllPath;
//
//        // Attempt to change path. Note that this only affects the environment  
//        // variable of the current process. The command processor's  
//        // environment is not changed.  
//        _putenv(L"PATH=" + sPath);
//        ODA_ASSERT_ONCE((sPath = _wgetenv(L"PATH")).find(sDllPath) >= 0);
//        if ((sPath = _wgetenv(L"PATH")).find(sDllPath) >= 0)
//        {
//          if (::odrxDynamicLinker()->loadModule(csPath, true).get())
//            return OdString::kEmpty; // ok
//          _putenv(L"PATH=" + sSvPath); // restore
//        }
//      }
//    }
//  }
//#endif

  ExAppServicesPtr pExAppServices = ExAppServices::cast(::odrxSysRegistry()->getAt(EX_APP_SERVICES).get());
  ODA_ASSERT_ONCE(!pExAppServices.isNull());

  OdString sPath(csPath);
  //OdString sAssemblyVersion;
  if (   !pExAppServices.isNull()
      && pExAppServices->isNetAssembly(sPath) //, &sAssemblyVersion) 
    #if defined(TD_DLL_VERSION_SUFFIX_STR)
      && OdString(sPath).makeLower().find(OdString(OD_T("td_mgd")) + TD_DLL_VERSION_SUFFIX_STR) < 0
    #endif
      )
  {
    OdEdCommandPtr pCommand = ::odedRegCmds()->lookupCmd(OD_T("NETLOAD"));
    if (pCommand.isNull())
    {
      ::odrxDynamicLinker()->loadModule(L"NETLOADCommand.tx");
      ::odrxDynamicLinker()->loadModule(L"TD_Mgd.dll");
      pCommand = ::odedRegCmds()->lookupCmd(L"NETLOAD");
      if (pCommand.isNull())
        return OD_T("TD_Mgd.dll is inaccessible.");
    }

    OdSmartPtr<ExStringIO> pStringIO = ExStringIO::create(sPath);
    pCmdCtx->setArbitraryData(OD_T("NETLOAD/PATH"), pStringIO);
    pCommand->execute(pCmdCtx);
    pCmdCtx->setArbitraryData(OD_T("NETLOAD/PATH"), NULL);
    return OdString::kEmpty; // ok
  }

  bool bMissingEntryPoint = false,
       bTryToLoadViaFixedPath = false;
#if !defined(_TOOLKIT_IN_DLL_)
 #if defined(__BORLANDC__)
  #define ODRX_ENTRY_POINT_NAME "_odrxCreateModuleObject"
 #else
  #define ODRX_ENTRY_POINT_NAME "odrxCreateModuleObject"
 #endif

 #if defined(_MSC_VER) || defined(__BORLANDC__) // defined(Q_OS_WIN) || defined(Q_WS_WIN)
  HMODULE hModule = LoadLibrary(sPath);
  if (hModule)
  {
    if (!::GetProcAddress(hModule, ODRX_ENTRY_POINT_NAME))
      bMissingEntryPoint = true;
    else
      bTryToLoadViaFixedPath = true;
    ::FreeLibrary(hModule);
  }
 //#elif defined(__APPLE__) // defined(Q_WS_MAC) 
 // void* hModule = LoadSharedLibrary(sPath, RTLD_LAZY | RTLD_GLOBAL); // #9011 // | RTLD_LOCAL);
 // if (hModule)
 // {
 //   if (!GetFunction(hModule, ODRX_ENTRY_POINT_NAME))
 //     bMissingEntryPoint = true;
 //   else
 //     bTryToLoadViaFixedPath = true;
 //   FreeSharedLibrary(hModule);
 // }
 #else // unix, linux and others
  void* hModule = dlopen(sPath, RTLD_LAZY | RTLD_GLOBAL); // #9011 // | RTLD_LOCAL);
  if (hModule)
  {
    if (!dlsym(hModule, ODRX_ENTRY_POINT_NAME))
      bMissingEntryPoint = true;
    else
      bTryToLoadViaFixedPath = true;
    dlclose(hModule);
  }
 #endif
#endif
  OdString sMsg;
  if (pPropServices->getBaseHostAppServices()->findFile(sPath).isEmpty())
    sMsg.format(pPropServices->tr("Application file \"%ls\" was not found").c_str(), sPath.c_str());
  else
  {
    if (   bTryToLoadViaFixedPath
        && ::odrxDynamicLinker()->loadModule(sPath, true).get())
      return OdString::kEmpty; // ok
    sMsg.format(pPropServices->tr("Loading of \"%ls\" was failed%ls.").c_str(), 
                sPath.c_str(), pPropServices->tr(bMissingEntryPoint ? " (Entry point is missing)" : "").c_str());
  }
  return sMsg;
}

//------------------------------------------------------------------------

class ContentsPropSourceImpl : public OdPropSource
{
  OdEdCommandContext* m_pCmdCtx;
  OdString m_sCurrAppName;
public:
  ContentsPropSourceImpl(OdEdCommandContext* pCmdCtx);

  static bool loadStartupSuite(OdEdCommandContext* pCmdCtx);
  static OdString getAppNameByPath(const OdString& sPath);
  // "./" - is base-app-path of folder with native apps (dll & tx)
  static OdString toAbsolutePath(const OdString& sPath, OdEdCommandContext* pCmdCtx);
  // for subpath of base-app-path("./") only
  static OdString toRelativePath(const OdString& sPath, OdEdCommandContext* pCmdCtx);
  static OdString baseAppPath(OdEdCommandContext* pCmdCtx);

  virtual bool updateXml(OdString& sXml);
  virtual bool setPropertyValue(const OdString& csPath,
                                const OdString& sValue,
                                Action& action);
  virtual bool getPropertyPossibleValues(const OdString& sPath,
                                         OdStringArray& values,
                                         OdStringArray& iconPaths);
};

class AppLoadPropSourceImpl : public OdPropSource
{
  OdEdCommandContext* m_pCmdCtx;
  OdString m_sCurrAppName;
public:
  AppLoadPropSourceImpl(OdEdCommandContext* pCmdCtx);

  static bool canBeUnloaded(const OdString& sAppName);
  static OdString supportedAppFilters();

  virtual bool updateXml(OdString& sXml);
  virtual bool setPropertyValue(const OdString& csPath,
                                const OdString& sValue,
                                Action& action);
  virtual bool getPropertyPossibleValues(const OdString& sPath,
                                         OdStringArray& values,
                                         OdStringArray& iconPaths);
};

//------------------------------------------------------------------------

FIRSTDLL_EXPORT OdStringArray odrxLoadedApps();
FIRSTDLL_EXPORT bool odrxAppIsLoaded(const OdString& appName);

ContentsPropSourceImpl::ContentsPropSourceImpl(OdEdCommandContext* pCmdCtx)
  : m_pCmdCtx(pCmdCtx)
{
}

//static 
bool ContentsPropSourceImpl::loadStartupSuite(OdEdCommandContext* pCmdCtx)
{
  ExAppServicesPtr pExAppServices = ExAppServices::cast(::odrxSysRegistry()->getAt(EX_APP_SERVICES).get());
  ODA_ASSERT_ONCE(!pExAppServices.isNull());
  if (pExAppServices.isNull())
    return false;

  OdString sBaseKey = OD_T("startup/");
  OdInt16 count = 0;
  if (!pExAppServices->readRegistryValue(sBaseKey + OD_T("count"), toRbType(count), &count))
    count = 0;
  if (!count)
    return true; // Startup Suite is empty

  OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());
  if (!pPropServices.isNull())
    pCmdCtx->userIO()->putString(pPropServices->tr("Loading Startup Suite :"));

  for (OdInt16 idx = 0; idx < count; idx++)
  {
    OdString sKey;
    sKey.format(OD_T("startup/path_%d"), idx);

    OdString sPath;
    if (!pExAppServices->readRegistryValue(sKey, toRbType(sPath), &sPath))
    {
      ODA_FAIL_ONCE();
      break;
    }

    OdString sAppName = getAppNameByPath(sPath);
    sPath = toAbsolutePath(sPath, pCmdCtx);

    OdString sMsg = loadApp(sPath, pCmdCtx);
    if (!sMsg.isEmpty())
    {
      OdDbHostAppServices* pServices = getServices(pCmdCtx);
      if (pServices)
        pServices->warning(sMsg);
      return false;
    }

    if (!pPropServices.isNull())
    {
      sMsg.format(pPropServices->tr("  %ls was loaded.").c_str(), sAppName.c_str());
      pCmdCtx->userIO()->putString(sMsg);
    }

  } // end for

  return true;
}

//static 
OdString ContentsPropSourceImpl::getAppNameByPath(const OdString& sPath)
{
  OdString sAppName = sPath;
  sAppName.replace(__OD_T('\\'), __OD_T('/'));
  int pos = sAppName.reverseFind(__OD_T('/'));
  if (pos >= 0)
    sAppName = sAppName.mid(++pos);
 #if defined(TD_DLL_VERSION_SUFFIX_STR)
  sAppName.replace(TD_DLL_VERSION_SUFFIX_STR, OD_T(""));
 #endif
  return sAppName;
}

//static 
OdString ContentsPropSourceImpl::baseAppPath(OdEdCommandContext* pCmdCtx)
{
  OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());
  OdDbBaseHostAppServices* pServices = pPropServices->getBaseHostAppServices();
#if !defined(_TOOLKIT_IN_DLL_)
    OdString moduleName(OD_T("OdaQtConsole"));
#else
    OdString moduleName(OD_T("OdaQtConsole.dll"));
#endif
  OdString sBasePath = pServices->findFile(moduleName, NULL, OdDbBaseHostAppServices::kTXApplication);
  sBasePath.replace(__OD_T('\\'), __OD_T('/'));
  int pos = sBasePath.reverseFind(__OD_T('/'));
  ODA_ASSERT_ONCE(pos > 0);
  sBasePath = sBasePath.left(++pos);
  ODA_ASSERT_ONCE(!sBasePath.isEmpty());

  return sBasePath;
}

// "./" - is base-path of folder with native apps (dll & tx)
//static 
OdString ContentsPropSourceImpl::toAbsolutePath(const OdString& csPath, OdEdCommandContext* pCmdCtx)
{
  OdString sPath = csPath;
  sPath.replace(__OD_T('\\'), __OD_T('/'));

  if (!sPath.find(OD_T("./")))
    sPath = baseAppPath(pCmdCtx) + sPath.mid(2);

 #if defined(_MSC_VER)|| defined(__BORLANDC__) // defined(Q_OS_WIN) || defined(Q_WS_WIN)
  sPath.replace(__OD_T('/'), __OD_T('\\'));
 #endif
  return sPath;
}

// for subpath of base-path("./") only
//static
OdString ContentsPropSourceImpl::toRelativePath(const OdString& csPath, OdEdCommandContext* pCmdCtx)
{
  OdString sPath = csPath;
  sPath.replace(__OD_T('\\'), __OD_T('/'));

  OdString sBaseAppPath = baseAppPath(pCmdCtx);
 #if !defined(__linux__)
  if (!OdString(sPath).makeLower().find(OdString(sBaseAppPath).makeLower()))
 #else
  if (!sPath.find(sBaseAppPath))
 #endif
    sPath = OD_T("./") + sPath.mid(sBaseAppPath.getLength());

  return sPath;
}

bool ContentsPropSourceImpl::updateXml(OdString& sXml)
{
  sXml += OD_T("<Property Path=\"StartupSuite\" Name=\"Applications\" Type=\"grid\" />\n");

  OdInt16 count = 0;
  OdString sRecord;
  ExAppServicesPtr pExAppServices = ExAppServices::cast(::odrxSysRegistry()->getAt(EX_APP_SERVICES).get());
  if (!pExAppServices.isNull())
  {
    OdString sBaseKey = OD_T("startup/");
    if (!pExAppServices->readRegistryValue(sBaseKey + OD_T("count"), toRbType(count), &count))
      count = 0;
  
    for (OdInt16 idx = 0; idx < count; idx++)
    {
      OdString sKey;
      sKey.format(OD_T("startup/path_%d"), idx);

      OdString sPath;
      if (!pExAppServices->readRegistryValue(sKey, toRbType(sPath), &sPath))
      {
        ODA_FAIL_ONCE();
        break;
      }

      OdString sAppName = getAppNameByPath(sPath);
      sRecord.format(OD_T("<Property Path=\"StartupSuite/Applications/%ls\" Name=\"File\" Type=\"string\" Value=\"%ls\" ReadOnly=\"true\" />\n"),
                     sAppName.c_str(), sAppName.c_str());
      sXml += sRecord;

      sPath = toAbsolutePath(sPath, m_pCmdCtx);
      sRecord.format(OD_T("<Property Path=\"StartupSuite/Applications/%ls\" Name=\"Path\" Type=\"string\" Value=\"%ls\" ReadOnly=\"true\" />\n"),
                     sAppName.c_str(), sPath.c_str());
      sXml += sRecord;

      if (!idx && m_sCurrAppName.isEmpty())
        m_sCurrAppName = sAppName;
    } // end for
    if (!count)
    {
      // add empty row to create header fiels in grid
      sXml += OD_T("<Property Path=\"StartupSuite/Applications/empty\" Name=\"File\" Type=\"string\" Value=\"\" ReadOnly=\"true\" />\n");
      sXml += OD_T("<Property Path=\"StartupSuite/Applications/empty\" Name=\"Path\" Type=\"string\" Value=\"\" ReadOnly=\"true\" />\n");
    }

  } // end if

  sXml += OD_T("<Property Path=\"StartupSuite\" Name=\"ButtonsLayout\" Type=\"category\" Layout=\"hor\" Alignment=\"right\" />\n");
  sXml += OD_T("<Property Path=\"StartupSuite/ButtonsLayout\" Name=\"Add\" Type=\"button\" />\n");
  sRecord.format(OD_T("<Property Path=\"StartupSuite/ButtonsLayout\" Name=\"Remove\" Type=\"button\" Enabled=\"%ls\" />\n"),
                 (count && !m_sCurrAppName.isEmpty()) ? OD_T("true") : OD_T("false"));
  sXml += sRecord;
  sXml += OD_T("<Property Path=\"StartupSuite/ButtonsLayout\" Name=\"Close\" Type=\"button\" />\n");

  return true;
}

bool ContentsPropSourceImpl::setPropertyValue(const OdString& csPath,
                                              const OdString& sValue,
                                              Action& action)
{
  OdString sPath = csPath;
  OdString sName = extractFirst(sPath);
  if (sName != OD_T("StartupSuite"))
    return false;

  sName = extractFirst(sPath);

  if (   sName == OD_T("Applications")
      && sValue == OD_T("<setCurrent>"))
  {
    ODA_ASSERT_ONCE(!sPath.isEmpty());

    if (sPath == OD_T("empty"))
      sPath.empty();
    bool isEmptyPrev = m_sCurrAppName.isEmpty();
    
    m_sCurrAppName = sPath;
    if (m_sCurrAppName.isEmpty() == isEmptyPrev)
      return false;
    action = enAction_RefreshAllSources;
    return true;
  }

  if (sName == OD_T("ButtonsLayout"))
  {
    if (sPath == OD_T("Add"))
    {
      try 
      {
        sPath = m_pCmdCtx->userIO()->getFilePath(OD_T("Enter name of application"), 
                                                 OdEd::kGfpForOpen, 
                                                 OD_T("Select Application File"), // dialog caption 
                                                 OD_T("tx"), // default extension
                                                 OdString::kEmpty, // file name
                                                 AppLoadPropSourceImpl::supportedAppFilters());
      }
      catch (const OdEdCancel&)
      {
        sPath.empty(); 
      }
      if (sPath.isEmpty())
        return false;

      OdString sMsg = loadApp(sPath, m_pCmdCtx);
      if (!sMsg.isEmpty())
      {
        OdDbHostAppServices* pServices = getServices(m_pCmdCtx);
        if (pServices)
          pServices->warning(sMsg);
        return false;
      }
      sPath = toRelativePath(sPath, m_pCmdCtx);

      ExAppServicesPtr pExAppServices = ExAppServices::cast(::odrxSysRegistry()->getAt(EX_APP_SERVICES).get());
      if (pExAppServices.isNull())
        return false;

      OdString sBaseKey = OD_T("startup/");
      OdInt16 count = 0;
      if (!pExAppServices->readRegistryValue(sBaseKey + OD_T("count"), toRbType(count), &count))
        count = 0;

      OdString sKey;
      for (OdInt16 idx = 0; idx < count; idx++)
      {
        sKey.format(OD_T("startup/path_%d"), idx);

        OdString sPath_;
        if (!pExAppServices->readRegistryValue(sKey, toRbType(sPath_), &sPath_))
        {
          ODA_FAIL_ONCE();
          break;
        }
        sPath_ = toRelativePath(sPath_, m_pCmdCtx);

        if (sPath == sPath_)
        {
          OdPropServicesPtr pPropServices = OdPropServices::cast(m_pCmdCtx);
          ODA_ASSERT_ONCE(!pPropServices.isNull());

          sMsg.format(OD_T("The following application is already present in the Startup Suite:\n %ls"), 
                      toAbsolutePath(sPath, m_pCmdCtx).c_str());
          pPropServices->messageBox(OD_T("%ls Error"), sMsg, OD_T("~OK"), NULL); // or //pPropServices->messageBox(OD_T("%ls Error"), sMsg);
          return false;
        }
      } // end for

      sKey.format(OD_T("startup/path_%d"), count++);
      pExAppServices->writeRegistryValue(sKey, toRbType(sPath), &sPath);
      pExAppServices->writeRegistryValue(sBaseKey + OD_T("count"), toRbType(count), &count);

      action = (count == 1) ? enAction_RefreshSourceWidgets : enAction_RefreshSource;
      return true;
    }
    if (sPath == OD_T("Remove"))
    {
      ODA_ASSERT_ONCE(!m_sCurrAppName.isEmpty())
      if (m_sCurrAppName.isEmpty())
        return false;

      ExAppServicesPtr pExAppServices = ExAppServices::cast(::odrxSysRegistry()->getAt(EX_APP_SERVICES).get());
      if (pExAppServices.isNull())
        return false;

      OdString sBaseKey = OD_T("startup/");
      OdInt16 count = 0;
      if (!pExAppServices->readRegistryValue(sBaseKey + OD_T("count"), toRbType(count), &count))
        count = 0;

      OdString sKey;
      for (OdInt16 idx = 0; idx < count; idx++)
      {
        sKey.format(OD_T("startup/path_%d"), idx);

        OdString sPath_;
        if (!pExAppServices->readRegistryValue(sKey, toRbType(sPath_), &sPath_))
        {
          ODA_FAIL_ONCE();
          break;
        }

        OdString sAppName = getAppNameByPath(sPath_);
        if (sAppName != m_sCurrAppName)
          continue;

        for (; idx < (count - 1); idx++)
        {
          sKey.format(OD_T("startup/path_%d"), idx + 1);
          if (!pExAppServices->readRegistryValue(sKey, toRbType(sPath_), &sPath_))
          {
            ODA_FAIL_ONCE();
            break;
          }
          sKey.format(OD_T("startup/path_%d"), idx);
          pExAppServices->writeRegistryValue(sKey, toRbType(sPath_), &sPath_);
        }
        count--;
        pExAppServices->writeRegistryValue(sBaseKey + OD_T("count"), toRbType(count), &count);
        m_sCurrAppName.empty();
        action = !count ? enAction_RefreshSourceWidgets : enAction_RefreshSource;
        return true;

      } // end for        {

      ODA_FAIL_ONCE(); // was not found
      m_sCurrAppName.empty();
      return false;
    }
    if (sPath == OD_T("Close"))
    {
      action = enAction_Close; // close dialog
      return true;
    }
  
    ODA_FAIL_ONCE(); // TODO
    return false;
  }

  return false;
}

bool ContentsPropSourceImpl::getPropertyPossibleValues(const OdString&, // sPath
                                                       OdStringArray&, // values
                                                       OdStringArray&) // iconPaths
{
  return false;
}

AppLoadPropSourceImpl::AppLoadPropSourceImpl(OdEdCommandContext* pCmdCtx)
  : m_pCmdCtx(pCmdCtx)
{
}

//static 
bool AppLoadPropSourceImpl::canBeUnloaded(const OdString& sAppName)
{
  if (sAppName.isEmpty() || !::odrxAppIsLoaded(sAppName))
    return false;
  // TODO via real references
  if (   sAppName.find(OD_T("OdaQtConsole.")) >= 0 // >0 for non-Windows platforms because of prefix "lib"
      || sAppName.find(OD_T("OdaQtViewSystem.")) >= 0
      || sAppName.find(OD_T("TD_DgnDb.")) >= 0
      || sAppName.find(OD_T("TD_Db.")) >= 0
      || sAppName.find(OD_T("TG_Db.")) >= 0)
    return false;

  ODA_ASSERT_ONCE(::odrxAppIsLoaded(sAppName));
  OdRxModule* pModule = ::odrxDynamicLinker()->loadModule(sAppName, true).get();
  ODA_ASSERT_ONCE(pModule);
  OdString sPath = pModule->moduleName();
  bool bCanBeUnloaded = (pModule->sysData() && !pModule->numRefs());
  return bCanBeUnloaded;
}

//static 
OdString AppLoadPropSourceImpl::supportedAppFilters()
{
  OdString sFilters = OD_T("Apps (*.tx)|*.tx||");
 #if defined(__linux__)
  sFilters += OD_T("Dlls  (*.so)|*.so|");
  sFilters += OD_T("All Apps (*.tx *.txv *.so)|*.tx *.txv *.so|");
 #elif defined(__APPLE__) // MacOS X
  sFilters += OD_T("Dlls  (*.dylib)|*.dylib|");
  sFilters += OD_T("All Apps (*.tx *.txv *.dylib)|*.tx *.txv *.dylib|");
 #else
  sFilters += OD_T("Dlls  (*.dll)|*.dll|");
  sFilters += OD_T("All Apps (*.tx *.txv *.dll)|*.tx *.txv *.dll|");
 #endif
  if (!sFilters.isEmpty())
    sFilters += OD_T("|"); // or // sFilter += OD_T("All files (*.*)|*.*||");

  return sFilters;
}

bool AppLoadPropSourceImpl::updateXml(OdString& sXml)
{
  //sXml.empty();
  //sXml += OD_T("<?xml version=\"1.0\" encoding=\"windows-1252\"?>\n");
  //sXml += OD_T("<Properties>\n");

  OdString sRecord;
  sRecord.format(__OD_T("<Property Path=\"AppLoad\" Name=\"LoadSubDialog\" Type=\"filedialog\"")
                 __OD_T(" Filters=\"%ls\"")
                 __OD_T(" AcceptName=\"Load\" />\n"),
                 supportedAppFilters().c_str());
  sXml += sRecord;

  sXml += OD_T("<Property Path=\"AppLoad\" Name=\"AppsLayout\" Type=\"category\" Layout=\"hor\" />\n");
  sXml += OD_T("<Property Path=\"AppLoad/AppsLayout\" Name=\"Applications\" Type=\"grid\" />\n");

  OdStringArray arrApps = ::odrxLoadedApps();
  OdStringArray::size_type sz = arrApps.size();
  bool bCanBeUnloaded = false;
  for (OdStringArray::size_type index = 0; index < sz; index++)
  {
    OdString sAppName = arrApps[index];
    ODA_ASSERT_ONCE(::odrxAppIsLoaded(sAppName));
    OdRxModule* pModule = ::odrxDynamicLinker()->loadModule(sAppName, true).get();
    ODA_ASSERT_ONCE(pModule || sAppName == L"OdDbGeoMapPE.tx");
    if (!pModule)
      continue;
    OdString sPath = pModule->moduleName();
    bCanBeUnloaded = canBeUnloaded(sAppName);

    sRecord.format(OD_T("<Property Path=\"AppLoad/AppsLayout/Applications/%ls\" Name=\"File\" Type=\"string\" Value=\"%ls\"%ls />\n"),
                   sAppName.c_str(), sAppName.c_str(), bCanBeUnloaded ? OD_T(" ReadOnly=\"true\"") // is not gray but inedited
                                                                      : OD_T(" Enabled=\"false\""));
    sXml += sRecord;

    sRecord.format(OD_T("<Property Path=\"AppLoad/AppsLayout/Applications/%ls\" Name=\"Path\" Type=\"string\" Value=\"%ls\"%ls />\n"),
                   sAppName.c_str(), sPath.c_str(), bCanBeUnloaded ? OD_T(" ReadOnly=\"true\"") // is not gray but inedited
                                                                   : OD_T(" Enabled=\"false\""));
    sXml += sRecord;
  }

  sXml += OD_T("<Property Path=\"AppLoad/AppsLayout\" Name=\"AppButtonsLayout\" Type=\"category\" Layout=\"ver\" />\n");

#if defined(QFILEDIALOG_IN_LAYOUT_UNSUPPORTED)
  sRecord.format(OD_T("<Property Path=\"AppLoad/AppsLayout/AppButtonsLayout\" Name=\"Load\" Type=\"button\" />\n"));
  sXml += sRecord;
#endif

  bCanBeUnloaded = canBeUnloaded(m_sCurrAppName);
  sRecord.format(OD_T("<Property Path=\"AppLoad/AppsLayout/AppButtonsLayout\" Name=\"Unload\" Type=\"button\"%ls />\n"),
                 bCanBeUnloaded ? OD_T("") : OD_T(" Enabled=\"false\""));
  sXml += sRecord;
  sXml += __OD_T("<Property Path=\"AppLoad/AppsLayout/AppButtonsLayout\" Name=\"Contents\" VisibleName=\"Startup Suite\" Type=\"button\"")
        //__OD_T(" Image=\":/images/suite.png\"") // TODO
          __OD_T(" />\n");

  sXml += OD_T("<Property Path=\"AppLoad\" Name=\"StandardButtons\" Type=\"category\" Layout=\"hor\" Alignment=\"right\" />\n");
  sXml += OD_T("<Property Path=\"AppLoad/StandardButtons\" Name=\"Close\" Type=\"button\" />\n");

  //sXml += OD_T("</Properties>\n");
  return true;
}

bool AppLoadPropSourceImpl::setPropertyValue(const OdString& csPath,
                                             const OdString& sValue,
                                             Action& action)
{
  OdString sPath = csPath;
  if (extractFirst(sPath) != OD_T("AppLoad"))
    return false;

  if (sPath == OD_T("StandardButtons/Close"))
  {
    action = enAction_Close; // close dialog
    return true;
  }

  OdString sName = extractFirst(sPath);

  if (sName == OD_T("LoadSubDialog"))
  {
    ODA_ASSERT_ONCE(sPath.isEmpty() && m_pCmdCtx);

    // load application (sValue is path name)
    OdString sMsg = loadApp(sValue, m_pCmdCtx);
    if (!sMsg.isEmpty())
    {
      OdDbHostAppServices* pServices = getServices(m_pCmdCtx);
      if (pServices)
        pServices->warning(sMsg);
      return false;
    }
    action = enAction_RefreshAllSources;
    return true;
  }

  if (sName == OD_T("AppsLayout"))
  {
    sName = extractFirst(sPath);
    
    if (   sName == OD_T("Applications")
        && sValue == OD_T("<setCurrent>"))
    {
      ODA_ASSERT_ONCE(!sPath.isEmpty());

      bool bCanBeUnloadedPrev = canBeUnloaded(m_sCurrAppName);
      m_sCurrAppName = sPath;
      bool bCanBeUnloaded = canBeUnloaded(m_sCurrAppName);
      if (bCanBeUnloaded == bCanBeUnloadedPrev)
        return false; // nothing to refresh

      action = enAction_RefreshAllSources;
      return true;
    }

    if (sName == OD_T("AppButtonsLayout"))
    {
      sName = extractFirst(sPath);
      ODA_ASSERT_ONCE(sPath.isEmpty());

#    if defined(QFILEDIALOG_IN_LAYOUT_UNSUPPORTED)
      if (sName == OD_T("Load"))
      {
        // possible way for Linux via problem with QFileDialog in QVLayout
        try 
        {
          sPath = m_pCmdCtx->userIO()->getFilePath(OD_T("Enter name of application"), 
                                                   OdEd::kGfpForOpen, 
                                                   OD_T("Select Application File"), // dialog caption 
                                                   OD_T("tx"), // default extension
                                                   OdString::kEmpty, // file name
                                                   AppLoadPropSourceImpl::supportedAppFilters());
        }
        catch (const OdEdCancel&)
        {
          sPath.empty(); 
        }
        if (sPath.isEmpty())
          return false;

        OdString sMsg = loadApp(sPath, m_pCmdCtx);
        if (!sMsg.isEmpty())
        {
          OdDbHostAppServices* pServices = getServices(m_pCmdCtx);
          if (pServices)
            pServices->warning(sMsg);
          return false;
        }

        m_sCurrAppName = sPath;
        action = enAction_RefreshAllSourcesWidgets;
        return true;
      }
#    endif //defined(QFILEDIALOG_IN_LAYOUT_UNSUPPORTED)

      if (sName == OD_T("Unload"))
      {
        bool bCanBeUnloaded = canBeUnloaded(m_sCurrAppName);
        ODA_ASSERT_ONCE(bCanBeUnloaded);
        if (!bCanBeUnloaded)
          return false;
        if (!::odrxDynamicLinker()->unloadModule(m_sCurrAppName))
        {
          ODA_FAIL_ONCE(); // TODO fix in canBeUnloaded
          return false;
        }
        m_sCurrAppName.empty();
        action = enAction_RefreshAllSourcesWidgets;
        return true;
      }

      if (sName == OD_T("Contents"))
      {
        OdPropServicesPtr pPropServices = OdPropServices::cast(m_pCmdCtx);
        ODA_ASSERT_ONCE(!pPropServices.isNull());
        
        OdString sDlgKey = pPropServices->openDialog(OD_T("%ls - Startup Suite"));
        if (!sDlgKey.isEmpty())
        {
          ContentsPropSourceImpl source(m_pCmdCtx);
       //#if 0 // TODO (unsupported now)
       // pPropServices->addSource(sDlgKey, &source, OD_T("StartupSuite"));
       //#else
          // via the set of xml fragments as sources
          pPropServices->addSource(sDlgKey, &source, OD_T("StartupSuite/Applications"));
          pPropServices->pushLayout(sDlgKey);
          pPropServices->addSource(sDlgKey, &source, OD_T("StartupSuite/ButtonsLayout"));
       //#endif
          pPropServices->executeDialog(sDlgKey);
          pPropServices->closeDialog(sDlgKey);
          action = enAction_RefreshSource;
          return true;
        }
        return false;
      }
    }

    ODA_FAIL_ONCE(); // TODO
    return false;
  }

  return false;
}

bool AppLoadPropSourceImpl::getPropertyPossibleValues(const OdString&, // sPath
                                                       OdStringArray&, // values
                                                       OdStringArray&) // iconPaths
{
  return false;
}

//------------------------------------------------------------------------

void OdqCmd_AppLoad::execute(OdEdCommandContext* pCmdCtx)
{
  OdPropServicesPtr pPropServices = OdPropServices::cast(pCmdCtx);
  ODA_ASSERT_ONCE(!pPropServices.isNull());
  if (pPropServices.isNull())
    throw OdError(eNotApplicable);

  OdString sPath;
  OdSmartPtr<ExStringIO> pStartUpSiteIO = pCmdCtx->arbitraryData(OD_T("APPLOAD/SUBCOMMAND"));
  if (pStartUpSiteIO.get())
  {
    OdString sSubCmd = pStartUpSiteIO->getString(OD_T(""));
    OdString sSubCmdUpper = sSubCmd;
    sSubCmdUpper.makeUpper();
    if (   sSubCmdUpper.makeUpper() == OD_T("LOADCONTENTS")
        || sSubCmdUpper == OD_T("LOADSTARTUPSUITE"))
    {
      ContentsPropSourceImpl::loadStartupSuite(pCmdCtx);
      return;
    }

    while (!(sSubCmd = sSubCmd.trimLeft().trimRight()).isEmpty())
    {
      OdString sPath = sSubCmd;
      int pos = sSubCmd.find(__OD_T('\"'));
      if (!pos)
      {
        sSubCmd = sSubCmd.mid(1);
        pos = sSubCmd.find(__OD_T('\"'));
        if (pos >= 0)
        {
          sPath = sSubCmd.left(pos);
          sSubCmd = sSubCmd.mid(++pos);
        }
        else
        {
          ODA_FAIL_ONCE();
          sPath = sSubCmd;
          sSubCmd.empty();
        }
      }
      else
      {
        int pos = sSubCmd.find(__OD_T(' '));
        if (pos > 0)
        {
          sPath = sSubCmd.left(pos);
          sSubCmd = sSubCmd.mid(++pos);
        }
        else
        {
          sPath = sSubCmd;
          sSubCmd.empty();
        }
      }
      if (sPath.isEmpty())
        continue;

      OdString sAppName = ContentsPropSourceImpl::getAppNameByPath(sPath);
      sPath = ContentsPropSourceImpl::toAbsolutePath(sPath, pCmdCtx);

      OdString sMsg = loadApp(sPath, pCmdCtx);
      if (!sMsg.isEmpty())
      {
        OdDbHostAppServices* pServices = getServices(pCmdCtx);
        if (pServices)
          pServices->warning(sMsg);
        continue;
      }

      sMsg.format(pPropServices->tr("  %ls was loaded.").c_str(), sAppName.c_str());
      pCmdCtx->userIO()->putString(sMsg);

    } // end while
    return;
  }

  OdDbHostAppServices* pServices = getServices(pCmdCtx);
  if (   (pServices && !pServices->getFILEDIA())
      || pPropServices->isInputStringReady(pCmdCtx))
  {
    sPath = pCmdCtx->userIO()->getString(OD_T("Enter name of application to load"));
    OdString sMsg = loadApp(sPath, pCmdCtx);
    if (!sMsg.isEmpty())
      pServices->warning(sMsg);
    return;
  }

  OdString sDlgKey = pPropServices->openDialog(OD_T("%ls - Load / Unload Applications"));
  if (!sDlgKey.isEmpty())
  {
    AppLoadPropSourceImpl source(pCmdCtx);

 //#if 0 // TODO (unsupported now)
 // // via common xml (use attribute Layout of category nodes) // TODO
 // pPropServices->addSource(sDlgKey, &source, OD_T("AppLoad"), OD_T("Properties"));
 //#else
    // via the set of xlm fragments as sources

#  if !defined(QFILEDIALOG_IN_LAYOUT_UNSUPPORTED)
    pPropServices->addSource(sDlgKey, &source, 
                             OD_T("AppLoad/LoadSubDialog"), OD_T("Properties"));
#  endif // !defined(QFILEDIALOG_IN_LAYOUT_UNSUPPORTED)

    pPropServices->pushLayout(sDlgKey);
    pPropServices->addSource(sDlgKey, &source,
                             OD_T("AppLoad/AppsLayout/Applications"), OD_T("Properties"), 
                             OD_T("title=Loaded Applications"));
    pPropServices->addSource(sDlgKey, &source, 
                             OD_T("AppLoad/AppsLayout/AppButtonsLayout"), OD_T("Properties"));
    pPropServices->popLayout(sDlgKey);
    // default close buttons is not accessible for modal dialogs on Mac
    pPropServices->addSource(sDlgKey, &source, 
                             OD_T("AppLoad/StandardButtons"), OD_T("Properties"));
 //#endif

    pPropServices->executeDialog(sDlgKey);
    pPropServices->closeDialog(sDlgKey);
    return;
  }

  // simple old version

  OdString sFilters = AppLoadPropSourceImpl::supportedAppFilters();

  sPath = pCmdCtx->userIO()->getFilePath(OD_T("Enter name of application to load"), 
                                         OdEd::kGfpForOpen, 
                                         OD_T("Select Application File"), // dialog caption 
                                         OD_T("tx"), // default extension
                                         OdString::kEmpty, // file name
                                         sFilters);
  if (sPath.isEmpty())
    return; // without message "Cancel by user" // throw OdEdCancel();

  OdString sMsg = loadApp(sPath, pCmdCtx);
  if (!sMsg.isEmpty())
    pServices->warning(sMsg);
}
