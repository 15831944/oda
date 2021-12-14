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
// DwfDbModule.cpp
//

#include "OdaCommon.h"
#include "DwfDbModuleImpl.h"
#include "DynamicLinker.h"
#include "StaticRxObject.h"
#include "DbUndoController.h" // fixed compiling error on mac
#include "DbUnitsFormatterImpl.h"
#include "Ge/GePlane.h"
#include "RxInit.h"
#include "Gs/GsModel.h"
#include "Gs/GsBaseVectorizeDevice.h"
#include "Gi/GiPathNode.h"
#include "DbUserIO.h"
#include "PropServices.h"
#include "SysVarPE.h"
#include "OdBinaryData.h"
#include "ColorMapping.h"
#include "DwfDatabase.h"
#include "DwfHostAppServices.h"
#include "DwfGsLayoutHelper.h"
#include "DwfGiContext.h"
#include "DwfObjectImpl.h"

#include "DbPlotSettingsValidator.h"
#include "DbHostAppServices.h"

#include "DwfImport.h" // ../Imports/DwfImport/Include/DwfImport.h

#include "RxVariantValue.h"
#include "ColorMapping.h"
using namespace TD_DWF_IMPORT;

#include "DwfExport.h" // ../Exports/DwfExport/Include/DwfExport.h
using namespace TD_DWF_EXPORT;

//#include "DwfSSet.h"
//#include "DwfSysVarPEImpl.h"
//#include "DwfCommandContext.h"
// for OdDwfGsPath
//#include "GsMarkerArray.h"
//#include "DwfSubentId.h"

//////////////////////////////////////////////////////////////////////////

class OdDwfDbServicesImpl : public OdDwfHostAppServices
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdDwfHostAppServices);

  friend class OdDwfDbModuleImpl;
  OdSmartPtr<OdDbBaseHostAppServices> m_pBaseHostAppServices;

  // ----- OdDwfHostAppServices -----

  virtual OdString findFile(const OdString& filename, OdDbBaseDatabase* pDb = 0, FindFileHint hint = kDefault)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->findFile(filename, pDb, hint);
    return OdDwfHostAppServices::findFile(filename, pDb, hint);
  }

  virtual OdDbHostAppProgressMeter* newProgressMeter()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->newProgressMeter();
    return OdDwfHostAppServices::newProgressMeter();
  }

  virtual void releaseProgressMeter(OdDbHostAppProgressMeter* pProgressMeter)
  {
    if (!m_pBaseHostAppServices.isNull())
    {
      m_pBaseHostAppServices->releaseProgressMeter(pProgressMeter);
      return;
    }
    OdDwfHostAppServices::releaseProgressMeter(pProgressMeter);
  }

  virtual const OdString program()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->program();
    return OdDwfHostAppServices::program();
  }

  virtual const OdString product()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->product();
    return OdDwfHostAppServices::product();
  }

  virtual const OdString companyName()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->companyName();
    return OdDwfHostAppServices::companyName();
  }

  virtual ProdIdCode prodcode()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->prodcode();
    return OdDwfHostAppServices::prodcode();
  }

  virtual const OdString releaseMajorMinorString()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->releaseMajorMinorString();
    return OdDwfHostAppServices::releaseMajorMinorString();
  }

  virtual int releaseMajorVersion()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->releaseMajorVersion();
    return OdDwfHostAppServices::releaseMajorVersion();
  }

  virtual int releaseMinorVersion()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->releaseMinorVersion();
    return OdDwfHostAppServices::releaseMinorVersion();
  }

  virtual const OdString versionString()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->versionString();
    return OdDwfHostAppServices::versionString();
  }

  TD_USING(OdDwfHostAppServices::warning);

  virtual void warning(const char* warnVisGroup, const OdString& message)
  {
    if (!m_pBaseHostAppServices.isNull())
    {
      m_pBaseHostAppServices->warning(warnVisGroup, message);
      return;
    }
    OdDwfHostAppServices::warning(warnVisGroup, message);
  }

  virtual void warning(OdWarning warningOb)
  {
    if (!m_pBaseHostAppServices.isNull())
    {
      m_pBaseHostAppServices->warning(warningOb);
      return;
    }
    OdDwfHostAppServices::warning(warningOb);
  }

  virtual OdString getErrorDescription(unsigned int errorCode)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getErrorDescription(errorCode);
    return OdDwfHostAppServices::getErrorDescription(errorCode);
  }

  virtual OdString formatMessage(unsigned int errorCode, va_list* argList)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->formatMessage(errorCode, argList);
    return OdDwfHostAppServices::formatMessage(errorCode, argList);
  }

  virtual OdDbUndoControllerPtr newUndoController()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->newUndoController();
    return OdDwfHostAppServices::newUndoController();
  }

  virtual void auditPrintReport(OdAuditInfo * pAuditInfo, const OdString& strLine, int printDest) const
  {
    if (!m_pBaseHostAppServices.isNull())
    {
      m_pBaseHostAppServices->auditPrintReport(pAuditInfo, strLine, printDest);
      return;
    }
    OdDwfHostAppServices::auditPrintReport(pAuditInfo, strLine, printDest);
  }

  virtual bool ttfFileNameByDescriptor(const OdTtfDescriptor& description, OdString& filename)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->ttfFileNameByDescriptor(description, filename);
    return OdDwfHostAppServices::ttfFileNameByDescriptor(description, filename);
  }

  virtual OdString getAlternateFontName() const
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getAlternateFontName();
    return OdDwfHostAppServices::getAlternateFontName();
  }

  virtual OdString getFontMapFileName() const
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getFontMapFileName();
    return OdDwfHostAppServices::getFontMapFileName();
  }

  virtual OdString getPreferableFont(const OdString& fontName, OdFontType fontType)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getPreferableFont(fontName, fontType);
    return OdDwfHostAppServices::getPreferableFont(fontName, fontType);
  }

  virtual OdString getSubstituteFont(const OdString& fontName, OdFontType fontType)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getSubstituteFont(fontName, fontType);
    return OdDwfHostAppServices::getSubstituteFont(fontName, fontType);
  }

  virtual OdString fileDialog(int flags,
                              const OdString& dialogCaption = OdString::kEmpty,
                              const OdString& defExt = OdString::kEmpty,
                              const OdString& defFilename = OdString::kEmpty,
                              const OdString& filter = OdString::kEmpty)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->fileDialog(flags, dialogCaption, defExt, defFilename, filter);
    return OdDwfHostAppServices::fileDialog(flags, dialogCaption, defExt, defFilename, filter);
  }

  virtual OdGsDevicePtr gsBitmapDevice(OdRxObject* pViewObj = NULL,
                                       OdDbBaseDatabase* pDb = NULL,
                                       OdUInt32 flags = 0)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->gsBitmapDevice(pViewObj, pDb, flags);
    return OdDwfHostAppServices::gsBitmapDevice(pViewObj, pDb, flags);
  }

  // ----- OdDwfDbServicesImpl -----

  //void restoreRegVariables()
  //{
  //  ExAppServicesPtr pExAppServices = ExAppServices::cast(m_pBaseHostAppServices);
  //  if (pExAppServices.isNull())
  //    return;

  //  OdString sBaseKey = OD_T("vars/tg/");
  //  #define REGVAR_DEF(type, name, def_val, unused4, unused5) \
  //    if (!pExAppServices->readRegistryValue(sBaseKey + OD_T(#name), \
  //                                           toRbType(m_##name), &m_##name)) \
  //      m_##name = def_val;

  //  #define REGVAR_DEF_ARRAY(type, name, unused3, unused4, unused5) \
  //    { \
  //      m_##name.clear(); \
  //      for (int index = 0; true; index++) \
  //      { \
  //        OdString sName; \
  //        sName.format(OD_T("%ls[%d]"), OD_T(#name), index); \
  //        OdString sValue; \
  //        if (   !pExAppServices->readRegistryValue(sBaseKey + sName, \
  //                                                  OdResBuf::kRtString, &sValue) \
  //            || sValue.isEmpty()) \
  //          break; \
  //        m_##name.push_back(sValue); \
  //      } \
  //    }

  //  #include "DgRegVarDefs.h"

  //  #undef REGVAR_DEF
  //  #undef REGVAR_DEF_ARRAY
  //}

  //void saveRegVariables()
  //{
  //  ExAppServicesPtr pExAppServices = ExAppServices::cast(m_pBaseHostAppServices);
  //  if (pExAppServices.isNull())
  //    return;

  //  OdString sBaseKey = OD_T("vars/tg/");
  //  #define REGVAR_DEF(type, name, def_val, unused4, unused5) \
  //    pExAppServices->writeRegistryValue(sBaseKey + OD_T(#name), \
  //                                       toRbType(m_##name), &m_##name);
  //  #define REGVAR_DEF_ARRAY(type, name, unused3, unused4, unused5) \
  //    { \
  //      int num = m_##name.size(); \
  //      for (int index = 0; index <= num; index++) \
  //      { \
  //        OdString sName; \
  //        sName.format(OD_T("%ls[%d]"), OD_T(#name), index); \
  //        if (index >= num) \
  //        { \
  //          pExAppServices->writeRegistryValue(sBaseKey + sName, \
  //                                             OdResBuf::kRtString, &OdString::kEmpty); \
  //          continue; \
  //        } \
  //        pExAppServices->writeRegistryValue(sBaseKey + sName, \
  //                                           toRbType(m_##name.getAt(index)), &m_##name.getAt(index)); \
  //      } \
  //    }

  //  #include "DgRegVarDefs.h"

  //  #undef REGVAR_DEF
  //  #undef REGVAR_DEF_ARRAY
  //}

  //OdStringArray resetRegVariables() // set to default values
  //{
  //  OdStringArray vars;

  //  #define REGVAR_DEF(type, name, def_val, unused4, unused5) \
  //    if (m_##name != def_val) \
  //    { \
  //      m_##name = def_val; \
  //      vars.push_back(OD_T(#name)); \
  //    }
  //  // are empty by default
  //  #define REGVAR_DEF_ARRAY(type, name, unused3, unused4, unused5) \
  //    if (m_##name.size()) \
  //    { \
  //      m_##name.clear(); \
  //      vars.push_back(OD_T(#name)); \
  //    }

  //  #include "DgRegVarDefs.h"

  //  #undef REGVAR_DEF
  //  #undef REGVAR_DEF_ARRAY

  //  return vars;
  //}
};

//////////////////////////////////////////////////////////////////////////

static OdDwfDbServicesImpl* appDwfServicesImpl()
{
  static OdStaticRxObject<OdDwfDbServicesImpl> g_services;
  return &g_services;
}

OdDwfHostAppServices* appDwfServices()
{
  return appDwfServicesImpl();
}

//////////////////////////////////////////////////////////////////////////

class OdDwfDbModuleImpl : public OdExtDbModuleBaseImpl
{
public:
  OdDwfDbModuleImpl();
  virtual ~OdDwfDbModuleImpl();

  // OdRxModule overridden
  virtual void initApp();
  virtual void uninitApp();

  // init issues
  virtual void setBaseHostAppServices(OdDbBaseHostAppServices* pServices);
  virtual void initOdExtPE();
  virtual void uninitOdExtPE();

  virtual OdStringArray resetRegVariables(); // set to default values

  virtual bool getPreviewBitmap(const OdString& fileName, 
                                OdBinaryData& dataBmp); // out

  // load / save issues
  virtual OdRxObjectPtr readFile(const OdString& fileName, 
                                 Oda::FileShareMode shareMode = Oda::kShareDenyWrite);
  virtual OdRxObjectPtr createUninitializedDatabase();
  virtual bool loadToUninitializedDatabase(OdRxObjectPtr pRxDb, 
                                           const OdString& fileName, 
                                           Oda::FileShareMode shareMode = Oda::kShareDenyWrite);
  virtual bool writeFile(const OdString& fileName, OdEdCommandContext* ctx);

  // Cloud command command context issues
  virtual OdEdBaseIO* baseIO(OdEdCommandContext* ctx);
  virtual OdEdCommandContextPtr cloneCommandContext(OdEdCommandContext* ctx,
                                                    OdEdBaseIO* pIOStream = NULL, 
                                                    OdRxObject* pRxDatabase = NULL);

  // render issues
  virtual bool getVisibleLayoutViewIds(OdRxObject*, OdDbStub*&, OdDbStubPtrArray&, OdGsDCRectArray&)
  {
    return false; // to work with standard way (via OdDbBaseDatabasePE & OdDbBaseLayoutPE)
  }
  virtual bool getLayoutViewNamesAndIds(OdRxObject*, OdStringArray&, OdDbStubPtrArray&,
                                        OdString* = NULL, bool = true, OdGsDCRectArray* = NULL)
  {
    return false; // to work with standard way (via OdDbBaseDatabasePE & OdDbBaseLayoutPE)
  }
  virtual OdGsDevicePtr createDevice(OdGsModulePtr pGs, OdRxObjectPtr pRxDatabase);
  virtual OdGsDevicePtr createBitmapDevice(OdGsModulePtr pGs, OdRxObjectPtr pRxDatabase);
  virtual OdGsDevicePtr takeUnderlyingDeviceAndViewports(OdGsDevicePtr pGsDevice,
                                                         OdArray<OdGsViewPtr>& viewportsToDeleteAfterUpdate); // out

  virtual void enableGsModel(class OdGiDefaultContext* pCtx, bool bGsCache);

  virtual bool getModelBackground(OdRxObject* pRxDatabase, ODCOLORREF& clrBackground,
                                                           ODGSPALETTE* pPalette = NULL);
  virtual bool setModelBackground(OdRxObject* pRxDatabase, ODCOLORREF clrBackground);

  // get point, zoom etc issues
  virtual OdGsView* getActiveGsView(OdGsDevice* pGsDevice);
  virtual OdGsModel* getGsModel(OdGsDevice* pGsDevice);

  virtual OdUnitsFormatter* getFormatter(OdRxObject* pRxDatabase);

  virtual bool isPerspective(OdRxObject* pRxDatabase, OdGsDevice* pGsDevice = NULL);
  virtual bool setPerspective(OdRxObject* pRxDatabase, bool bVal);

  // selection issues
  virtual OdSelectionSetPtr createSelectionSet(OdRxObject* pRxDatabase);
  virtual OdSelectionSetPtr select(OdGsView* gsView,
                                   int nPoints,
                                   const OdGePoint3d* wcsPts,
                                   OdDbVisualSelection::Mode mode = OdDbVisualSelection::kCrossing,
                                   OdDbVisualSelection::SubentSelectionMode sm = OdDbVisualSelection::kDisableSubents,
                                   const OdRxObject* pFilter = NULL);
  virtual void getVisibleAllIds(OdRxObject* pRxDatabase, OdDbStubPtrArray& ids);
  virtual OdDbStub* getVisibleLastId(OdRxObject* pRxDatabase);
  virtual void highlight(OdGsView* gsView, bool bValue,
                         OdDbStub* id, const OdDbBaseFullSubentPath* pPath);
  virtual bool getGripPoints(OdDbStub* id,
                             OdGePoint3dArray& gripPoints,
                             OdEdCommandContext* pCmdCtx = NULL);
  virtual bool getGripPoints(OdDbStub* id,
                             OdDbGripDataPtrArray& grips,
                             double curViewUnitSize,
                             int gripSize,
                             const OdGeVector3d& curViewDir,
                             int bitFlags = 0);
  virtual bool getGripPointsAtSubentPath(const OdDbBaseFullSubentPath& path, 
                                         OdDbGripDataPtrArray& grips,
                                         double curViewUnitSize, 
                                         int gripSize,
                                         const OdGeVector3d& curViewDir, 
                                         OdUInt32 bitflags = 0);
  virtual bool isErased(OdDbStub* id);
  virtual OdDbHandle getElementHandle(OdDbStub* id);
  virtual OdString getElementClassName(OdDbStub* id);

  // command issues
  virtual bool cmdErase(OdEdCommandContext* pCmdCtx);

  // render issues
  virtual bool argsAtSetupLayoutViews(OdRxObject* pRxDatabase, bool& bDoZoomExtents, OdGsView::RenderMode& rm);

  // filters issues
  virtual bool isResponsible(OdRxObject* pRxDatabase);
  virtual OdStringArray getExtensions(enum OdEd::GetFilePathFlags flg = OdEd::kGfpForOpen, 
                                      OdRxObject* pRxDatabase = NULL,
                                      bool bWithExtSynonyms = false);
  virtual OdString getFilterByExtension(const OdString& sExt, 
                                        int index = 0,
                                        enum OdEd::GetFilePathFlags flg = OdEd::kGfpForOpen,
                                        OdStringArray* pExtSynonyms = NULL);

  // import / export issues
  virtual OdStringArray getImportExportExtensions(OdRxObject* pRxDatabase,
                                                  bool isForImport = true,
                                                  bool bWithExtSynonyms = false); // TODO
  virtual OdString getImportExportFilterByExtension(const OdString& sExt,
                                                    int index = 0, // TODO for such formats as DXF (it may be binary or ascii)
                                                    bool isForImport = true,
                                                    OdStringArray* pExtSynonyms = NULL); // TODO
  virtual bool importFromFile(OdEdCommandContext* pCmdCtx, const OdString& fileName, ODCOLORREF& clrBackground);
  virtual bool exportToFile(OdEdCommandContext* pCmdCtx, const OdString& fileName, int filterIndex, ODCOLORREF clrBackground);

  /////// Commands ///////
  #define ODRX_CMD_ENTRY(cmdName, name, impl) OdStaticRxObject<OdqCmd##name> m_cmd##name;
  #include "DwfDbCommands.h"

private:
  OdRxModulePtr m_pDbModule;
  //OdStaticRxObject<OdDbUnitsFormatterImpl> m_formatter;
};

ODRX_DEFINE_DYNAMIC_MODULE(OdDwfDbModuleImpl);

OdDwfDbModuleImpl::OdDwfDbModuleImpl()
{
  #define ODRX_CMD_ENTRY(cmdName, name, impl) m_cmd##name.m_pModule = this;
  #include "DwfDbCommands.h"
}

OdDwfDbModuleImpl::~OdDwfDbModuleImpl()
{
}

void OdDwfDbModuleImpl::initApp()
{
  m_pDbModule = odrxDynamicLinker()->loadApp(L"TDwfDb.tx");
  ODA_ASSERT_ONCE(m_pDbModule.get());

  // ...::rxInit();

  // register commands
  OdEdCommandStackPtr pCommands = odedRegCmds();
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->addCommand(&m_cmd##name);
  #include "DwfDbCommands.h"
}

void OdDwfDbModuleImpl::uninitApp()
{
  // unregister commands
  OdEdCommandStackPtr pCommands = odedRegCmds();
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->removeCmd(&m_cmd##name);
  #include "DwfDbCommands.h"

  // ...::rxUninit();
  m_pDbModule = NULL;
}

// init issues
void OdDwfDbModuleImpl::setBaseHostAppServices(OdDbBaseHostAppServices* pServices)
{
  appDwfServicesImpl()->m_pBaseHostAppServices = pServices;
}

// it's here to prevent inconvenient linking with OdaQtConsole
#if defined(ODA_SYS_VAR_PE_UNLINKED_RXINIT) && defined(_TOOLKIT_IN_DLL_)
ODRX_NO_CONS_DEFINE_MEMBERS(OdSysVarPE, OdRxObject);
#endif
static bool s_bUnlinkedWithOdSysVarPE = false;

void OdDwfDbModuleImpl::initOdExtPE()
{
  //::initOdDgSysVarPE();
  OdRxDictionary* pClassDict = ::odrxClassDictionary().get();
  s_bUnlinkedWithOdSysVarPE = (!OdSysVarPE::g_pDesc);
  if (s_bUnlinkedWithOdSysVarPE)
  {
    // We have duplicated definition of g_pDesc, desc(), isA() and queryX(...) 
    // via ODRX_NO_CONS_DEFINE_MEMBERS(OdSysVarPE, OdRxObject);
    // (to prevent inconvenient linking with OdaQtConsole)
#  if !defined(ODA_SYS_VAR_PE_UNLINKED_RXINIT)
    ODA_FAIL_ONCE();
#  endif

    // see also newOdRxClass
    OdRxClass* pOdSysVarPEClass = OdRxClass::cast(pClassDict->getAt(L"OdSysVarPE")).get();
    ODA_ASSERT_ONCE(pOdSysVarPEClass);
    OdSysVarPE::g_pDesc = pOdSysVarPEClass;
  }

  //appDwfServicesImpl()->restoreRegVariables();
}

void OdDwfDbModuleImpl::uninitOdExtPE()
{
  //appDwfServicesImpl()->saveRegVariables();

  //::uninitOdDgSysVarPE();
  if (s_bUnlinkedWithOdSysVarPE)
    OdSysVarPE::g_pDesc = NULL;
}

OdStringArray OdDwfDbModuleImpl::resetRegVariables() // set to default values
{
  return OdStringArray(); //return appDwfServicesImpl()->resetRegVariables();
}

bool OdDwfDbModuleImpl::getPreviewBitmap(const OdString& fileName, 
                                         OdBinaryData& dataBmp) // out
{
  OdDwfDatabasePtr pDwfDb = appDwfServices()->readFile(fileName);
  if (pDwfDb.isNull())
    return false;
  OdDwfObjectId id = pDwfDb->currentLayoutId();
  ODA_ASSERT_ONCE_X(TDWF, !id.isNull());
  if (id.isNull())
    return false;
  OdDwfViewPtr pView = id.openObject();
  return pView->getThumbnail(dataBmp);
}

OdRxObjectPtr OdDwfDbModuleImpl::readFile(const OdString& fileName, 
                                          Oda::FileShareMode) // shareMode // = Oda::kShareDenyWrite
{
  bool bDwf3dOrthoOnly = false;
  OdSysVarPEPtr pSysVarPE = ::odrxSysRegistry()->getAt(OD_SYS_VAR_PE); // = OdSysVarPE::cast(pCmdCtx);
  if (pSysVarPE.get())
  {
    OdString sDwf3dOrthoOnly = pSysVarPE->valueToString(NULL, //pCmdCtx,
                                                        L"dwf3dorthoonly");
    bDwf3dOrthoOnly = (sDwf3dOrthoOnly == L"true" || sDwf3dOrthoOnly == L"1");
    OdString sMarkup = pSysVarPE->valueToString(NULL, L"dwfmarkup");
    bool bMarkup = (sMarkup == L"true" || sMarkup == L"1");
    appDwfServices()->setMarkupEnabled(bMarkup);
  }

  OdDwfDatabasePtr pDwfDb = appDwfServices()->readFile(fileName, true, OdPassword(), bDwf3dOrthoOnly);
  return OdRxObject::cast(pDwfDb);
}

OdRxObjectPtr OdDwfDbModuleImpl::createUninitializedDatabase()
{
  OdDwfDatabasePtr pDwfDb = appDwfServicesImpl()->createDatabase();
  return OdRxObject::cast(pDwfDb);
}

bool OdDwfDbModuleImpl::loadToUninitializedDatabase(OdRxObjectPtr pRxDb, 
                                                    const OdString& fileName, 
                                                    Oda::FileShareMode) // shareMode // = Oda::kShareDenyWrite
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDb.get());
  ODA_ASSERT_ONCE(pDwfDb.get());
  if (pDwfDb.isNull())
    return false;

  pDwfDb->readFile(fileName);
  return true;
}

bool OdDwfDbModuleImpl::writeFile(const OdString& fileName,
                                  OdEdCommandContext* ctx)
{
  ODA_ASSERT_ONCE(ctx);
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(ctx->baseDatabase());
  if (pDwfDb.isNull())
    return false;

  ctx->userIO()->putError(L"Writing of DWF database is not supported.") ;
  //ODA_FAIL_ONCE(); // TODO //pDwfDb->writeFile(fileName);
  return true;
}

// Cloud command command context issues
// possible TODO move reset, baseIO & cloneObject into OdEdCommandContext

//class OdExDwfCmdContext : public ExDbCommandContext {
//protected:
//  OdEdCommandContext* m_pOwner; // of clone
//
//  OdExDwfCmdContext()
//    : m_pOwner(NULL)
//  {
//  }
//
//public:
//
//  static OdEdCommandContextPtr createObject(OdEdBaseIO* pIOStream, 
//                                            OdRxObject* pRxDb = NULL,
//                                            OdEdCommandContext* pOwner = NULL)
//  {
//    OdEdCommandContextPtr pRes = OdRxObjectImpl<OdExDwfCmdContext, ExDbCommandContext>::createObject();
//    OdExDwfCmdContext* pCmdCtx = static_cast<OdExDwfCmdContext*>(pRes.get());
//    pCmdCtx->m_pDb = OdDwfDatabase::cast(pRxDb).get();
//    pCmdCtx->m_pIoStream = pIOStream;
//    pCmdCtx->m_pOwner = pOwner;
//    //m_pDb->addRef();
//    return pRes;
//  }
//
//  OdEdBaseIO* baseIO()
//  {
//    return m_pIoStream.get();
//  }
//
//  virtual OdEdCommandContextPtr cloneObject(OdEdBaseIO* pIOStream = NULL,
//                                            OdRxObject* pRxDb = NULL)
//  {
//    OdEdCommandContextPtr pRes = OdRxObjectImpl<OdExDwfCmdContext, ExDbCommandContext>::createObject();
//    //OdExDwfCmdContext* pCmdCtx = static_cast<OdExDwfCmdContext*>(pRes.get());
//    return createObject(pIOStream ? pIOStream : m_pIoStream.get(), 
//                        pRxDb ? pRxDb : m_pDb,
//                        this);
//  }
//};

OdEdBaseIO* OdDwfDbModuleImpl::baseIO(OdEdCommandContext* pCtx)
{
  if (!pCtx || OdDwfDatabase::cast(pCtx->baseDatabase()).isNull())
    return NULL;
  OdExDwfCmdContext* pCmdCtx = static_cast<OdExDwfCmdContext*>(pCtx);
  return pCmdCtx->baseIO();
}

OdEdCommandContextPtr OdDwfDbModuleImpl::cloneCommandContext(OdEdCommandContext* pCtx,
                                                             OdEdBaseIO* pIOStream, // = NULL
                                                             OdRxObject* pRxDb) // = NULL
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDb);
  ODA_ASSERT_ONCE(pDwfDb.get());
  if (pDwfDb.isNull())
    return OdEdCommandContextPtr();
  
  ODA_ASSERT_ONCE(pCtx); // to check with deb next
  if (!pCtx || OdDwfDatabase::cast(pCtx->baseDatabase()).isNull())
    return OdExDwfCmdContext::createObject(pIOStream, pRxDb);

  OdExDwfCmdContext* pCmdCtx = static_cast<OdExDwfCmdContext*>(pCtx);
  return pCmdCtx->cloneObject(pIOStream, pRxDb);
}

OdGsDevicePtr OdDwfDbModuleImpl::createDevice(OdGsModulePtr pGs,
                                              OdRxObjectPtr pRxDatabase)
{
  OdGsDevicePtr pGsDevice;
  if (pGs.isNull() || pRxDatabase.isNull())
  {
    ODA_FAIL_ONCE();
    return pGsDevice;
  }

  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  if (pDwfDb.isNull()) // valid way to prevent linking with DGN
    return pGsDevice;

  pGsDevice = pGs->createDevice();
  
  // the next will be done via OdqView::resetDevice()

  // Prepare database context for device
  //OdGiContextForDwfDatabasePtr pDgnContext = OdGiContextForDwfDatabase::createObject();
  //pDgnContext->setDatabase(pDwfDb);

  // Prepare the device to render the active layout in this database.
  //OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDatabase);
  //ODA_ASSERT_ONCE(!pDbPE.isNull());
  //pGsDevice = pDbPE->setupActiveLayoutViews(pGsDevice, pDgnContext);

  // TODO
  //pDwfDb->getActiveViewGroupId() ... pDwfDb->recommendActiveViewGroupId()

  return pGsDevice;
}

OdGsDevicePtr OdDwfDbModuleImpl::takeUnderlyingDeviceAndViewports(OdGsDevicePtr pGsDevice,
                                                                  OdArray<OdGsViewPtr>& viewportsToDeleteAfterUpdate) // out
{
  ODA_FAIL_ONCE(); // TODO
  return OdGsDevicePtr();
//  OdDwfGsLayoutHelperPtr pLayoutHelperPrev = OdDwfGsLayoutHelper::cast(pGsDevice);
//  ODA_ASSERT_ONCE(pLayoutHelperPrev.get() && !viewportsToDeleteAfterUpdate.size());
//  if (pLayoutHelperPrev.isNull())
//    return OdGsDevicePtr();
//  OdGsDevicePtr pUnderlying = pLayoutHelperPrev->underlyingDevice();
//  ODA_ASSERT_ONCE(pUnderlying.get())
//
//  int numViewportsToDelete = pLayoutHelperPrev->numViews();
//  if (numViewportsToDelete && pUnderlying.get())
//  {
//    for (int idx = 0; idx < numViewportsToDelete; idx++)
//    {
//      OdGsViewPtr pView = pLayoutHelperPrev->viewAt(idx);
//      ODA_ASSERT_ONCE(pView.get());
//      viewportsToDeleteAfterUpdate.push_back(pView); // store to delete after update
//    }
//  }
//  pLayoutHelperPrev->eraseAllViews();
//
//  return pUnderlying;
}

OdGsDevicePtr OdDwfDbModuleImpl::createBitmapDevice(OdGsModulePtr pGs, 
                                                    OdRxObjectPtr pRxDatabase)
{
  OdGsDevicePtr pGsDevice;
  if (pGs.isNull() || pRxDatabase.isNull())
  {
    ODA_FAIL_ONCE();
    return pGsDevice;
  }
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  if (pDwfDb.isNull()) // valid way to prevent linking with DGN
    return pGsDevice;

  pGsDevice = pGs->createBitmapDevice();
  return pGsDevice;
}

void OdDwfDbModuleImpl::enableGsModel(OdGiDefaultContext* pCtx,
                                      bool bGsCache)
{
  ODA_ASSERT_ONCE(pCtx);
  if (!pCtx)
    return;
  OdGiContextForDwfDatabasePtr pDwfCtx = OdGiContextForDwfDatabase::cast(pCtx); 
  if (pDwfCtx.isNull())
    return;
  pDwfCtx->enableGsModel(bGsCache);
}

//inline bool isDarkPalette(ODCOLORREF bgColor)
//{
//  return (ODGETRED(bgColor) < 140) && (ODGETGREEN(bgColor) < 140) && (ODGETBLUE(bgColor) < 140);
//}

bool OdDwfDbModuleImpl::getModelBackground(OdRxObject* pRxDatabase,
                                           ODCOLORREF& clrBackground,
                                           ODGSPALETTE* pPalette) // = NULL
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  if (pDwfDb.isNull())
    return false;
  
  //OdDwfModelPtr pModel = pDwfDb->getActiveModelId().safeOpenObject();
  //clrBackground = pModel->getBackground();

  if (pPalette)
  {
    ODGSPALETTE& palette = *pPalette;
    ODA_ASSERT_ONCE(!palette.size());
    //const ODCOLORREF* refColors = OdDgColorTable::currentPalette(pDwfDb);
    const ODCOLORREF* refColors = isDarkPalette(clrBackground) ? ::odcmAcadDarkPalette()
                                                               : ::odcmAcadLightPalette();

    palette.insert(palette.begin(), refColors, refColors + 256);
    // Color with #255 always defines background. 
    // The background of the active model must be considered in the device palette.
    palette[255] = clrBackground;
    // Note: This method should be called to resolve "white background issue" before setting device palette
    //OdDgColorTable::correctPaletteForWhiteBackground(palette.asArrayPtr());
  }

  return true;
}

bool OdDwfDbModuleImpl::setModelBackground(OdRxObject* pRxDatabase, 
                                           ODCOLORREF clrBackground)
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  if (pDwfDb.isNull())
    return false;

  ODA_FAIL_ONCE(); // TODO
  return false;
  //OdDwfModelPtr pModel = pDwfDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);
  //if (clrBackground == pModel->getBackground())
  //  return true;
  //pModel->setUseBackgroundColorFlag(true);
  //pModel->setBackground(clrBackground);
  //return true;
}

OdGsView* OdDwfDbModuleImpl::getActiveGsView(OdGsDevice* pGsDevice)
{
  OdDwfGsLayoutHelperPtr pLayoutHelper = OdDwfGsLayoutHelper::cast(pGsDevice);
  ODA_ASSERT_ONCE(pLayoutHelper.get());
  if (pLayoutHelper.isNull())
    return NULL;
  OdGsView* pGsView = pLayoutHelper->activeView().get();
  ODA_ASSERT_ONCE(pGsView);

  return pGsView;
}

OdGsModel* OdDwfDbModuleImpl::getGsModel(OdGsDevice* pGsDevice)
{
  OdDwfGsLayoutHelperPtr pLayoutHelper = OdDwfGsLayoutHelper::cast(pGsDevice);
  ODA_ASSERT_ONCE(pLayoutHelper.get());
  OdGsModel* pGsModel = pLayoutHelper->gsModel();
  // it is NULL if GS cache off //ODA_ASSERT_ONCE(pGsModel);
  return pGsModel;
}

OdUnitsFormatter* OdDwfDbModuleImpl::getFormatter(OdRxObject* pRxDatabase)
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  if (pDwfDb.isNull())
    return NULL;

  return &pDwfDb->formatter(); // return &m_formatter; // TODO special for each model in view (via active model)
}

bool OdDwfDbModuleImpl::isPerspective(OdRxObject* pRxDatabase, 
                                      OdGsDevice* pGsDevice) // = NULL
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  if (pDwfDb.get())
  {
    OdDwfViewPtr pView = OdDwfView::cast(pDwfDb->currentLayoutId().openObject());
    OdDwfModelViewPtr pModelView = OdDwfModelView::cast(pView);
    if (pModelView.get())
      return pModelView->isPerspective();
    return false;
  }
  return OdExtDbModuleBaseImpl::isPerspective(pRxDatabase, pGsDevice);
}

bool OdDwfDbModuleImpl::setPerspective(OdRxObject* pRxDatabase, bool bVal)
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  if (pDwfDb.get())
  {
    OdDwfViewPtr pView = OdDwfView::cast(pDwfDb->currentLayoutId().openObject());
    OdDwfModelViewPtr pModelView = OdDwfModelView::cast(pView);
    if (pModelView.get())
      return pModelView->setPerspective(bVal);
    if (!bVal)
      return true;
  }
  return OdExtDbModuleBaseImpl::setPerspective(pRxDatabase, bVal);
}

// selection issues
OdSelectionSetPtr OdDwfDbModuleImpl::createSelectionSet(OdRxObject* pRxDatabase)
{
  return OdSelectionSetPtr(); // TODO

  //OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  //if (pDwfDb.isNull())
  //  return OdSelectionSetPtr();
  //return OdDwfSelectionSet::createObject(pDwfDb);
}

OdSelectionSetPtr OdDwfDbModuleImpl::select(OdGsView* gsView,
                                            int nPoints,
                                            const OdGePoint3d* wcsPts,
                                            OdDbVisualSelection::Mode mode, // = OdDbVisualSelection::kCrossing
                                            OdDbVisualSelection::SubentSelectionMode sm, // = OdDbVisualSelection::kDisableSubents
                                            const OdRxObject* pFilter) // = NULL
{
  ODA_FAIL_ONCE(); // TODO
  return OdSelectionSetPtr();

  //if (!gsView || !gsView->userGiContext())
  //{
  //  ODA_FAIL_ONCE(); // test
  //  return OdSelectionSetPtr();
  //}
  //OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(gsView->userGiContext()->database());
  //if (pDwfDb.isNull())
  //  return OdSelectionSetPtr();
  //return OdDwfSelectionSet::select(pDwfDb, gsView, nPoints, wcsPts, mode, sm, pFilter);
}

//static bool isElementLocked(OdDwfObjectPtr pElem)
//{
//  if (pElem.isNull())
//    return false;
//
//  if (pElem->getLockedFlag())
//    return true;
//
//  OdDgGraphicsElementPtr pItem = OdDgGraphicsElement::cast(pElem);
//  if (pItem.isNull())
//    return false;
//
//  if (!pItem->getLevelId().isNull())
//  {
//    OdDgLevelTableRecordPtr pLevel = pItem->getLevelId().safeOpenObject();
//    if (pLevel->getIsFrozenFlag() || pLevel->getIsReadOnlyFlag())
//      return true;
//  }
//
//  OdDwfObjectIteratorPtr pItr;
//  {
//    OdDgCellHeader2dPtr pHeader2d = OdDgCellHeader2d::cast(pItem);
//    OdDgCellHeader3dPtr pHeader3d = OdDgCellHeader3d::cast(pItem);
//    if (!pHeader2d.isNull())
//      pItr = pHeader2d->createIterator();
//    if (pItr.isNull() && !(pHeader3d = OdDgCellHeader3d::cast(pItem)).isNull())
//      pItr = pHeader3d->createIterator();
//  }
//  if (pItr.isNull())
//    return false;
//  
//  for (; !pItr->done(); pItr->step())
//  {
//    pElem = OdDwfObject::cast(pItr->item().safeOpenObject());
//    if (pElem.isNull() || !isElementLocked(pElem))
//      continue;
//    return true;
//  }
//
//  return false;
//}

void OdDwfDbModuleImpl::getVisibleAllIds(OdRxObject* pRxDatabase, OdDbStubPtrArray& ids)
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  if (pDwfDb.isNull())
    return;

  ODA_FAIL_ONCE(); // TODO
  //OdDwfModelPtr pModel = OdDwfModel::cast(pDwfDb->getActiveModelId().openObject());
  //ODA_ASSERT_ONCE(!pModel.isNull());
  //if (pModel.isNull())
  //  return;
  //int idx = 0;
  //for (OdDwfObjectIteratorPtr pItr = pModel->createGraphicsElementsIterator();
  //     idx < 2; pItr = pModel->createControlElementsIterator(), idx++)
  //{
  //  for (; !pItr->done(); pItr->step())
  //  {
  //    OdDwfObjectId idElem = pItr->item();
  //    ODA_ASSERT_ONCE(!idElem.isNull());
  //    if (idElem.isNull())
  //      continue;

  //    OdDwfObjectPtr pElem = OdDwfObject::cast(idElem.openObject());
  //    ODA_ASSERT_ONCE(!pElem.isNull());
  //    if (pElem.isNull() || isElementLocked(pElem))
  //      continue;
  //    if (!pElem->gsNode()) // Cache
  //      continue;

  //    ids.push_back(idElem);
  //  }
  //}
}

OdDbStub* OdDwfDbModuleImpl::getVisibleLastId(OdRxObject* pRxDatabase)
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  if (pDwfDb.isNull())
    return NULL;

  ODA_FAIL_ONCE(); // TODO
  //OdDwfModelPtr pModel = OdDwfModel::cast(pDwfDb->getActiveModelId().openObject());
  //ODA_ASSERT_ONCE(!pModel.isNull());
  //if (pModel.isNull())
  //  return NULL;
  //int idx = 0;
  //for (OdDwfObjectIteratorPtr pItr = pModel->createGraphicsElementsIterator(false);
  //     idx < 2; pItr = pModel->createControlElementsIterator(false), idx++)
  //{
  //  for (; !pItr->done(); pItr->step())
  //  {
  //    OdDwfObjectId idElem = pItr->item();
  //    ODA_ASSERT_ONCE(!idElem.isNull());
  //    if (idElem.isNull())
  //      continue;

  //    OdDwfObjectPtr pElem = OdDwfObject::cast(idElem.openObject());
  //    ODA_ASSERT_ONCE(!pElem.isNull());
  //    if (pElem.isNull() || isElementLocked(pElem))
  //      continue;
  //    if (!pElem->gsNode()) // Cache
  //      continue;

  //    return idElem;
  //  }
  //}

  return NULL;
}

//class OdDwfGsPath
//{
//  struct Node : OdGiPathNode
//  {
//    const Node*     m_pParent;
//    OdDbStub*       m_pId;
//    OdGiDrawablePtr m_pDrawable;
//    OdGsMarker      m_gsMarker;
//
//    const OdGiPathNode* parent() const 
//    { 
//      return m_pParent; 
//    }
//    OdDbStub* persistentDrawableId() const 
//    { 
//      return m_pId; 
//    }
//    const OdGiDrawable* transientDrawable() const 
//    { 
//      return m_pDrawable; 
//    }
//    OdGsMarker selectionMarker() const 
//    { 
//      return m_gsMarker; 
//    }
//  };
//  const Node* m_pLeaf;
//
//  void add(const OdGiDrawable* pDrawable, const OdDwfObjectId& drawableId, OdGsMarker gsMarker = -1)
//  {
//    Node* pNode = new Node();
//    pNode->m_pParent    = m_pLeaf;
//    m_pLeaf = pNode;
//
//    pNode->m_pDrawable  = pDrawable;
//    pNode->m_pId        = drawableId;
//    pNode->m_gsMarker   = gsMarker;
//  }
//
//  //void addNode(OdDwfObjectIdArray::const_iterator& iter)
//  //{
//  //  OdDwfObjectPtr pObj = iter->safeOpenObject();
//  //  addNode(pObj);
//  //  OdDgBlockReferencePtr pInsert = OdDgBlockReference::cast(pObj);
//  //  if (pInsert.get())
//  //    addNode(pInsert->blockTableRecord());
//  //  ++iter;
//  //}
//public:
//  OdDwfGsPath()
//    : m_pLeaf(0)
//  {
//  }
//  ~OdDwfGsPath()
//  {
//    clear();
//  }
//  OdDwfGsPath(const OdDgFullSubentPath& path)
//    : m_pLeaf(0)
//  {
//    set(path);
//  }
//
//  void clear()
//  {
//    while(m_pLeaf)
//    {
//      const Node* pNode = m_pLeaf;
//      m_pLeaf = pNode->m_pParent;
//      delete pNode;
//    }
//    m_pLeaf = NULL;
//  }
//
//  void set(const OdDgFullSubentPath& path)
//  {
//    set(path, kNullSubentIndex);
//  }
//  void set(const OdDgFullSubentPath& path, OdGsMarker gsMarker)
//  {
//    clear();
//    const OdDwfObjectIdArray& ids = path.objectIds();
//
//    OdDwfObjectIdArray::const_iterator iter = ids.begin();
//    if (iter==ids.end())
//      throw OdError(eInvalidInput);
//
//    OdDwfObjectPtr pObj = iter->safeOpenObject();
//    if (!pObj->ownerId().isNull())
//      addNode(pObj->ownerId());
//    for (; iter != ids.end()-1; ++iter)
//      addNode(*iter);
//
//    addNode(*iter, gsMarker);
//  }
//
//  void addNode(const OdDwfObjectId& drawableId, OdGsMarker gsMarker = kNullSubentIndex)
//  {
//    add(0, drawableId, gsMarker);
//  }
//  void addNode(const OdGiDrawable* pDrawable, OdGsMarker gsMarker = kNullSubentIndex)
//  {
//    add(pDrawable->isPersistent() ? 0 : pDrawable, pDrawable->id(), gsMarker);
//  }
//
//  operator const OdGiPathNode&() const
//  {
//    return *m_pLeaf;
//  }
//};

void OdDwfDbModuleImpl::highlight(OdGsView* gsView, bool bValue,
                                  OdDbStub* id, const OdDbBaseFullSubentPath* pPath)
{
  OdRxObject* pRxDatabase = baseDatabaseBy(id);
  ODA_ASSERT_ONCE(pRxDatabase && id);
  if (!pRxDatabase || !id)
    return;

  ODA_FAIL_ONCE(); // TODO
  //OdDwfObjectId idEnt(id);
  //OdDwfObjectPtr pEnt = idEnt.openObject();
  //if (pEnt.isNull())
  //  return; // erased
  //
  //// TODO via pEnt->highlight(bValue, &pPath);
  //// see OdDbEntity::subHighlight
  ////
  //OdGsCache* pCache = pEnt->gsNode();
  //if (!pCache)
  //{
  //  pEnt->downgradeOpen();
  //  pCache = pEnt->gsNode();
  //  ODA_ASSERT_ONCE(pCache);
  //  if (!pCache)
  //    return;
  //}

  //if (!pPath)
  //{
  //  //if (pEnt->ownerId().isNull())
  //  //  return;
  //  OdDwfGsPath gsPath; //OdDbGsPath gsPath;
  //  if (!pEnt->ownerId().isNull())
  //    gsPath.addNode(pEnt->ownerId());
  // #ifdef _DEBUG
  //  else
  //    gsPath = gsPath; // brk !!!!
  // #endif
  //  gsPath.addNode(pEnt);
  //  pCache->model()->highlight(gsPath, bValue);
  //  return;
  //}

  //ODA_FAIL_ONCE(); // TODO
  ////OdGsMarkerArray gsMarkers;
  ////pEnt->getGsMarkersAtSubentPath(*pPath, gsMarkers);
  ////for (OdGsMarkerArray::iterator sm = gsMarkers.begin(); sm != gsMarkers.end(); ++sm)
  ////{
  ////  OdDwfGsPath gsPath; //OdDbGsPath gsPath;
  ////  gsPath.set(*pPath, *sm);
  ////  pCache->model()->highlight(gsPath, bValue);
  ////}
}

bool OdDwfDbModuleImpl::getGripPoints(OdDbStub* id,
                                      OdGePoint3dArray& gripPoints,
                                      OdEdCommandContext* pCmdCtx) // = NULL
{
  ODA_ASSERT_ONCE(id);
  if (!id)
    return false;

  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(baseDatabaseBy(id));
  if (pDwfDb.isNull())
    return false;

  ODA_FAIL_ONCE(); // TODO
  OdResult res = eNotImplementedYet;
  //OdDwfObjectPtr pElem = OdDwfObject::cast(OdDwfObjectId(id).openObject());
  //ODA_ASSERT_ONCE(!pElem.isNull());
  //if (pElem.isNull())
  //  return false;

  //// TODO
  ////res = pElem->getGripPoints(gripPoints);
  ////
  //// TEST for DGN grips
  //bool bTestDgnGrips = false;
  //OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pCmdCtx);
  //if (!pSysVarPE.isNull())
  //{
  //  bool bExist = false;
  //  OdString sValue = pSysVarPE->valueToString(pCmdCtx, OD_T("DWFGRIPS"), NULL, &bExist);
  //  ODA_ASSERT_ONCE(   sValue == OD_T("0") || sValue == OD_T("1") // DWG
  //                  || sValue == OD_T("false") || sValue == OD_T("true")); // DGN
  //  bTestDgnGrips = !(sValue.isEmpty() || sValue == OD_T("0") || sValue == OD_T("false"));
  //}
  //if (bTestDgnGrips)
  //{
  //  OdGeExtents3d extents;
  //  res = pElem->getGeomExtents(extents);
  //  if (res == eOk)
  //  {
  //    if (extents.isValidExtents())
  //    {
  //      OdGePoint3d ptCenter = extents.center();
  //      gripPoints.push_back(ptCenter);
  //    }
  //    else
  //      res = eNotImplementedYet;
  //  }
  //}

  //ODA_ASSERT_ONCE(res == eOk || res == eNotImplementedYet);
  return res == eOk;
}

bool OdDwfDbModuleImpl::getGripPoints(OdDbStub* id,
                                      OdDbGripDataPtrArray&, // grips
                                      double, // curViewUnitSize
                                      int, // gripSize
                                      const OdGeVector3d&, // curViewDir
                                      int) // bitFlags // = 0
{
  ODA_ASSERT_ONCE(id);
  if (!id)
    return false;

  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(baseDatabaseBy(id));
  if (pDwfDb.isNull())
    return false;

  ODA_FAIL_ONCE(); // TODO
  OdResult res = eNotImplementedYet;
  //OdDwfObjectPtr pElem = OdDwfObject::cast(OdDwfObjectId(id).openObject());
  //ODA_ASSERT_ONCE(!pElem.isNull());
  //if (pElem.isNull())
  //  return false;

  //// TODO
  ////res = pElem->getGripPoints(aPts, 
  ////                           grips,
  ////                           curViewUnitSize,
  ////                           gripSize,
  ////                           curViewDir,
  ////                           bitFlags);

  //ODA_ASSERT_ONCE(res == eOk || res == eNotImplementedYet);
  return res == eOk;
}

bool OdDwfDbModuleImpl::getGripPointsAtSubentPath(const OdDbBaseFullSubentPath& path, 
                                                  OdDbGripDataPtrArray&, // grips
                                                  double, // curViewUnitSize
                                                  int, // gripSize
                                                  const OdGeVector3d&, // curViewDir
                                                  OdUInt32) // bitflags // = 0
{
  if (!path.objectIds().size())
    return false;

  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(baseDatabaseBy(path.objectIds().first()));
  if (pDwfDb.isNull())
    return false;

  ODA_FAIL_ONCE(); // TODO
  OdResult res = eNotImplementedYet;
  //OdDwfObjectPtr pElem = OdDwfObject::cast(OdDwfObjectId(path.objectIds().first()).openObject());
  //ODA_ASSERT_ONCE(!pElem.isNull());
  //if (pElem.isNull())
  //  return false;

  //// TODO
  ////res = pElem->getGripPointsAtSubentPath(path
  ////                                       grips,
  ////                                       curViewUnitSize, 
  ////                                       gripSize,
  ////                                       curViewDir, 
  ////                                       bitflags);
  //ODA_ASSERT_ONCE(res == eOk || res == eNotImplementedYet);
  return res == eOk;
}

bool OdDwfDbModuleImpl::isErased(OdDbStub* id)
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(baseDatabaseBy(id));
  ODA_ASSERT_ONCE(id && !pDwfDb.isNull());
  if (pDwfDb.isNull())
    return false;
  ODA_FAIL_ONCE(); // TODO
  return false;
  //OdDwfObjectId idElem(id);
  //return idElem.isErased();
}

OdDbHandle OdDwfDbModuleImpl::getElementHandle(OdDbStub* id)
{
  ODA_FAIL_ONCE(); // TODO
  return OdDbHandle();
  //return OdDwfObjectId(id).getHandle();
}

OdString OdDwfDbModuleImpl::getElementClassName(OdDbStub* id)
{
  if (!id)
    return OdDwfDatabase::desc()->name();
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(baseDatabaseBy(id));
  ODA_ASSERT_ONCE(id && !pDwfDb.isNull());
  if (pDwfDb.isNull())
    return OdString::kEmpty;

  ODA_FAIL_ONCE(); // TODO
  return OdString::kEmpty;
  //OdDwfObjectId idElem(id);
  //OdDwfObjectPtr pElem = idElem.openObject();
  //ODA_ASSERT_ONCE(!pElem.isNull());
  //if (pElem.isNull())
  //  return OdString::kEmpty;
  //OdString sName = pElem->isA()->name();
  //return sName;
}

bool OdDwfDbModuleImpl::cmdErase(OdEdCommandContext* pCmdCtx)
{
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pCmdCtx->baseDatabase());
  if (pDwfDb.isNull())
    return false;

  ODA_FAIL_ONCE(); // TODO
  return false;
  //OdDbUserIOPtr pIO = pCmdCtx->userIO();
  //OdSelectionSetPtr pSSet = pIO->select();

  //int nLockedLayer = 0,
  //    nCount = 0;
  //for (OdDwfSelectionSetIteratorPtr pItr = pSSet->newIterator();
  //     !pItr->done(); pItr->next())
  //{
  //  OdDwfObjectId idElem = pItr->objectId();
  //  OdDwfObjectPtr pElem = OdDwfObject::cast(idElem.safeOpenObject(OdDg::kForWrite));
  //  ODA_ASSERT_ONCE(!pElem.isNull());
  //  if (pElem.isNull())
  //    continue;

  //  nCount++;
  //  if (isElementLocked(pElem))
  //  {
  //    nLockedLayer++;
  //    continue;
  //  }
  //  // already contained via OdDwfSelectionSet
  //  //OdGsCache* pCsh = pElem->gsNode();
  //  //if (pCsh)
  //  //  pCsh->model()->onErased(pElem, pElem->ownerId());
  //  
  //  pElem->erase();
  //}
  //if (!nCount)
  //  return false;

  //ODA_ASSERT_ONCE(OdPropServices::cast(pCmdCtx).get());
  //OdString sMsg;
  //sMsg.format(OdPropServices::cast(pCmdCtx)->tr("  %d found.").c_str(),
  //  nCount);
  //pIO->putString(sMsg);

  //if (nLockedLayer)
  //{
  //  sMsg.format(OdPropServices::cast(pCmdCtx)->tr( // msgNWasWereOnALockedLayer
  //              (nLockedLayer > 1) ? "  %d were on a locked layer."
  //                                 : "  %d was on a locked layer.").c_str(), 
  //              nLockedLayer);
  //  pIO->putString(sMsg);
  //}

  //return true;
}

bool OdDwfDbModuleImpl::argsAtSetupLayoutViews(OdRxObject* pRxDatabase, bool& bDoZoomExtents, OdGsView::RenderMode& rm)
{
  return OdExtDbModuleBaseImpl::argsAtSetupLayoutViews(pRxDatabase, bDoZoomExtents, rm); // set for false case
  //bDoZoomExtents = false; 
  //rm = OdGsView::k2DOptimized; // only it is correct for GLES of 2D DWF (like text_pos_problem.dwfx) //rm = OdGsView::kGouraudShaded;
  //return true;
}

bool OdDwfDbModuleImpl::isResponsible(OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase); // test
  OdDwfDatabasePtr pDwfDb = OdDwfDatabase::cast(pRxDatabase);
  return !pDwfDb.isNull();
}

OdStringArray OdDwfDbModuleImpl::getExtensions(enum OdEd::GetFilePathFlags flg, // = OdEd::kGfpForOpen
                                               OdRxObject* pRxDatabase, // = NULL
                                               bool) // bWithExtSynonyms = false
{
  OdStringArray lst;
  if (flg == OdEd::kGfpForOpen && (!pRxDatabase || isResponsible(pRxDatabase)))
    lst.push_back(L"dwf"), lst.push_back(L"dwfx");
  return lst;
}

OdString OdDwfDbModuleImpl::getFilterByExtension(const OdString& sExt, 
                                                 int index, // = 0
                                                 enum OdEd::GetFilePathFlags flg, // = OdEd::kGfpForOpen
                                                 OdStringArray*) // pExtSynonyms = NULL
{
  if (flg != OdEd::kGfpForOpen || index)
    return OdString::kEmpty;
  if (sExt == L"dwf")
    return L"DWF files (*.dwf)|*.dwf|";
  if (sExt == L"dwfx")
    return L"DWFX (XPS) files (*.dwfx)|*.dwfx|";

  return OdString::kEmpty;
}

// import / export issues

OdStringArray OdDwfDbModuleImpl::getImportExportExtensions(OdRxObject* pRxDatabase,
                                                           bool isForImport, // = true
                                                           bool) // bWithExtSynonyms // = false
{
  OdStringArray lst;
  if (   pRxDatabase
      && (!isForImport || !isResponsible(pRxDatabase))) // better avoid linking with TD_DB here // OdDbDatabase::cast(pRxDatabase))
  {
    if (!isForImport && isResponsible(pRxDatabase)) // INT-8872 disabled via invalid result for ACAD Design Review
    {
      ODA_ASSERT_ONCE(!"Uncomment next for an ability to export opened DWF back into file.");
      return lst;
    }
    lst.push_back(L"dwf");
    lst.push_back(L"dwfx");
  }
  return lst;
}

OdString OdDwfDbModuleImpl::getImportExportFilterByExtension(const OdString& sExt,
                                                             int index, // = 0
                                                             bool isForImport, // = true
                                                             OdStringArray*) // pExtSynonyms // = NULL
{
  if (sExt == L"dwf")
  {
    if (isForImport)
    {
      if (!index)
        return L"DWF files (*.dwf)|*.dwf|";
    }
    else 
    {
      switch (index)
      {
        // "3D DWF Publish" (3dPublish) 3d in OdaMfcApp :
      case 0:
        return L"3D DWF (Desgin Web Format) v6.01 (*.dwf)|*.dwf|";
        // "Export to DWF" in OdaMfcApp :
      case 1:
        return L"Binary DWF (Desgin Web Format) v6.0 (*.dwf)|*.dwf|";
      case 2:
        return L"Zipped Ascii Encoded 2D Stream DWF (Design Web Format) v6.0 (*.dwf)|*.dwf|";
      case 3:
        return L"Compressed DWF (Design Web Format) v5.5 (*.dwf)|*.dwf|";
      case 4:
        return L"Binary DWF (Design Web Format) v5.5 (*.dwf)|*.dwf|";
      case 5:
        return L"Ascii DWF (Design Web Format) v5.5 (*.dwf)|*.dwf|";
      case 6:
        return L"Compressed DWF (Design Web Format) v4.2 (*.dwf)|*.dwf|";
      case 7:
        return L"Binary DWF (Design Web Format) v4.2 (*.dwf)|*.dwf|";
      case 8:
        return L"Ascii DWF (Design Web Format) v4.2 (*.dwf)|*.dwf|";
      }
    }
  }
  if (sExt == L"dwfx")
  {
    if (isForImport)
    {
      if (!index)
        return L"XPS files (*.dwfx)|*.dwfx|";
    }
    else
    {
      switch (index)
      {
        // "3D DWF Publish" (3dPublish) 3d in OdaMfcApp :
      case 0:
        return L"3D DWFX (XPS) (*.dwfx)|*.dwfx|";
        // "Export to DWF" in OdaMfcApp :
      case 1:
        return L"XPS (*.dwfx)|*.dwfx|";
      }
    }
  }
  return OdString::kEmpty;
}

bool OdDwfDbModuleImpl::importFromFile(OdEdCommandContext* pCmdCtx, const OdString& sFileName,
                                       ODCOLORREF& clrBackground) // in /out
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  OdDwfImportModulePtr pDwfImportModule = ::odrxDynamicLinker()->loadApp(OdDwf7ImportModuleName);
  if (pDwfImportModule.isNull())
  {
    appDwfServicesImpl()->warning(OdDwf7ImportModuleName L".tx was not found");
    return false;
  }
  OdDwfImportPtr importer = pDwfImportModule->create();
  if (importer.isNull())
  {
    appDwfServicesImpl()->warning(L"Could not create dwf importer");
    return false;
  }
  OdRxDictionaryPtr pProps = importer->properties();
  //pDb = createDatabase();
  pProps->putAt(L"Database", pRxDatabase);
  pProps->putAt(L"DwfPath", OdRxVariantValue(sFileName));
  pProps->putAt(L"PreserveColorIndices", OdRxVariantValue(false));

  bool bModelToLayout = false;
  OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pCmdCtx);
  if (pSysVarPE.get())
  {
    OdString sModelToLayout = pSysVarPE->valueToString(pCmdCtx, L"dwfmodeltolayout");
    if (sModelToLayout == L"true" || sModelToLayout == L"1")
      bModelToLayout = true;
    OdString sAll = pSysVarPE->valueToString(pCmdCtx, L"dwfimportall");
    bool bAll = (sAll == L"true" || sAll == L"1");
    if (!bAll)
      pProps->putAt(L"LayoutNumber", OdRxVariantValue(OdInt32(1))); // Select which layout to import(-1, for all)
    OdString sMarkup = pSysVarPE->valueToString(pCmdCtx, L"dwfmarkup");
    bool bMarkup = (sMarkup == L"true" || sMarkup == L"1");
    pProps->putAt(L"MarkupFrozen", OdRxVariantValue(!bMarkup)); // CORE-16898
  }
  if (bModelToLayout)
    pProps->putAt(L"ModelToLayout", OdRxVariantValue(true));

  const ODCOLORREF* pCurPalette = odcmAcadPalette(clrBackground);

  pProps->putAt(L"DwgPalette", OdRxVariantValue(OdIntPtr(pCurPalette)));
  pProps->putAt(L"Background", OdRxVariantValue(OdInt32(clrBackground)));
  OdString sWarnings;
  OdDwfImport::ImportResult res = importer->import(&sWarnings);
  if (res != OdDwfImport::success && !sWarnings.isEmpty())
    sWarnings += L"\n";
  switch (res)
  {
  case OdDwfImport::success: 
    break;
  case OdDwfImport::bad_password:
    sWarnings += L"The file is encrypted"; break;
  default:
    sWarnings += L"Import error"; break;
  }
  if (!sWarnings.isEmpty())
    appDwfServicesImpl()->warning(L"DWF import: " + sWarnings);
  if (res != OdDwfImport::success)
    return false;

  OdRxVariantValue backGround = (OdRxObject*) pProps->getAt(OD_T("Background"));
  clrBackground = (ODCOLORREF) backGround->getInt32();
  return true;
}

#define DWF_DEVICE_NAME L"DWF6 ePlot.pc3"

bool OdDwfDbModuleImpl::exportToFile(OdEdCommandContext* pCmdCtx, const OdString& sFileName,
                                     int filterIndex, ODCOLORREF clrBackground)
{
  OdRxObject* pRxDatabase = pCmdCtx->baseDatabase();
  OdDwfExportModulePtr pDwfExportModule = ::odrxDynamicLinker()->loadApp(OdDwf7ExportModuleName);
  if (pDwfExportModule.isNull())
  {
    appDwfServicesImpl()->warning(OdDwf7ExportModuleName L"." DEFAULT_MODULE_EXTENSION_W L" is missing");
    return false;
  }

  OdDwfExportPtr exporter = pDwfExportModule->create();
  if (exporter.isNull())
  {
    appDwfServicesImpl()->warning(L"Could not create dwf exporter");
    return false;
  }

  OdString sExt;
  int pos = sFileName.reverseFind(L'.');
  if (pos > 0)
    sExt = sFileName.mid(++pos);
  sExt.makeLower();
  if (sExt != L"dwf" && sExt != L"dwfx")
  {
    appDwfServicesImpl()->warning(L"Unsupport file extension to export");
    return false;
  }

  if (filterIndex < 1)
  {
    // "3D DWF Publish" (3dPublish) 3d in OdaMfcApp
    //return L"3D DWF (Desgin Web Format) v6.01 (*.dwf)|*.dwf|";
    //return L"3D DWFX (XPS) (*.dwfx)|*.dwfx|";

    // publish 3d
    Dwf3dExportParams params;
    params.setDatabase(pRxDatabase);
    params.setBackground(clrBackground);
    //params.pPalette = theApp.curPalette();

    params.setDwfFileName(sFileName);

    exporter->export3dDwf(params);
    return true;
  }

  // "Export to DWF" in OdaMfcApp :

  DwExportParams params;
  params.setDatabase(pRxDatabase);
  params.setBackground(clrBackground);

  ODGSPALETTE palette;
  OdExtDbModule* pExtDb = getExtDbModule(pRxDatabase);
  if (pExtDb)
  {
    ODCOLORREF background;
    if (pExtDb->getModelBackground(pRxDatabase, background, &palette))
    {
      ODA_ASSERT_ONCE(palette.size() == 256);
      if (background != clrBackground)
      {
        ODA_ASSERT_ONCE(background == palette[255]);
        palette[255] = clrBackground;
        for (int idx = 0; idx < 255; idx++)
        {
          if (palette[idx] == clrBackground)
            palette[idx] = background;
        }
      }
      params.setPalette(palette.getPtr()); // PROD-127
    }
  }
  //else
  //  params.setPalette(odcmAcadLightPalette()); // default

  // ?TODO 
  //if (false) // Publish 2d via CPublishSheets dialog
  //{
  //  DwExportParams params;
  //  params.setDatabase(pRxDatabase);
  //  params.setBackground(clrBackground);
  //
  //  //params.sSourceFileName = m_strPathName;
  //  params.setInkedArea(false);                                                        // MKU 1/21/2004
  //  params.setColorMapOptimize(false);                                                 // MKU 1/21/2004
  //
  //  // choose output files + layout & other params
  //  //CPublishSheets  dlgPublishSheets(&params);
  //  //if (dlgPublishSheets.DoModal() == IDOK)
  //  // exporter->exportDwf(params);
  //}

  params.setDwfFileName(sFileName);
  //params.pPalette = theApp.curPalette();
  //params.sSourceFileName = m_strPathName;

  params.setInkedArea(false);                                                        // MKU 1/21/2004
  params.setColorMapOptimize(false);                                                 // MKU 1/21/2004
  params.setExportInvisibleText(true);
  params.setLineweightScale(0.5);

  OdString sCurrentLayout;
  OdRxObjectPtr pRxLayout;
  OdDbBaseLayoutPEPtr pLayoutPe;
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDatabase);
  ODA_ASSERT_ONCE(pDbPE.get());
  if (pDbPE.get())
  {
    pRxLayout = pDbPE->currentLayout(pRxDatabase);
    pLayoutPe = OdDbBaseLayoutPE::cast(pRxLayout);
    ODA_ASSERT_ONCE(pLayoutPe.get());
    if (pLayoutPe.get())
      sCurrentLayout = pLayoutPe->name(pRxLayout);
  }

  filterIndex--;
  DwfPageData pageData;

  OdSysVarPEPtr pSysVarPE = OdSysVarPE::cast(pCmdCtx);
  ODA_ASSERT_ONCE_X(TDWF, pSysVarPE.get());
  if (pSysVarPE.get())
  {
    OdString sInkedArea = pSysVarPE->valueToString(pCmdCtx, L"dwfinkedarea");
    if (!sInkedArea.isEmpty() && !(sInkedArea == L"0" || sInkedArea == L"false"))
      params.setInkedArea(true); // CORE-14287

    OdString sMaxRes = pSysVarPE->valueToString(pCmdCtx, L"dwfrasterres");
    long lVal = 0L;
    if (   !sMaxRes.isEmpty() && sMaxRes != L"0" // 0 means default value  => 2000 (800 before CORE-13428)
        && (lVal = odStrToInt(sMaxRes.c_str())) > 0L)
      params.setMaxRasterResolution((OdUInt16) lVal);

    // CORE-14030
    OdString sModelToExtents = pSysVarPE->valueToString(pCmdCtx, L"dwfmodelext");
    if (sModelToExtents.makeLower() == L"true" || sModelToExtents == "1")
      params.setForceInitialViewToExtents(true); // CORE-14030
    else if (   OdDbDatabase::cast(pRxDatabase).get()
             && pLayoutPe.get() && pLayoutPe->isModelLayout(pRxLayout))
    {
      OdDbPlotSettings* pPlotSettings = OdDbPlotSettings::cast(pRxLayout).get();
      ODA_ASSERT_ONCE_X(TDWF, pPlotSettings);

      OdResult res = eOk;
      OdString sDeviceName = pPlotSettings->getPlotCfgName(),
               sMediaName = pPlotSettings->getCanonicalMediaName(),
               sDwfDeviceName = DWF_DEVICE_NAME; // L"DWF6 ePlot.pc3"
      double paperWidth = 0.0, paperHeight = 0.0;
      pPlotSettings->getPlotPaperSize(paperWidth, paperHeight);
      OdDbPlotSettings::PlotPaperUnits units = pPlotSettings->plotPaperUnits();
      if (units == OdDbPlotSettings::kInches)
        paperWidth /= kMmPerInch, paperHeight /= kMmPerInch;
      // TODO there is not a way to get plot orientation from plot setting
      // The next is missing for None device of DwfExport_PNGsLowResolution_CORE13428_YM 1
      // Therefore orientation is portraint instead landscape in result.
      OdDbPlotSettings::PlotRotation rot = pPlotSettings->plotRotation();
      bool bSwap = (rot == OdDbPlotSettings::k90degrees || rot == OdDbPlotSettings::k180degrees);
      if (bSwap)
        std::swap(paperWidth, paperHeight);
      //OdString sStyleSheet = pPlotSettings->getCurrentStyleSheet();
      OdDbPlotSettingsValidator* pValidator = OdDbDatabase::cast(pRxDatabase)->appServices()->plotSettingsValidator();
      ODA_ASSERT_ONCE_X(TDWF, pValidator);
      pPlotSettings->upgradeOpen();
      res = pValidator->setPlotCfgName(pPlotSettings, sDeviceName, sMediaName); // is valid
      if (res != eOk)
      {
        res = pValidator->setPlotCfgName(pPlotSettings, sDwfDeviceName, sMediaName);
        if (res != eOk)
        {
          res = pValidator->setPlotCfgName(pPlotSettings, sDwfDeviceName);
          if (res == eOk)
            res = pValidator->setClosestMediaName(pPlotSettings, paperWidth, paperHeight, units, true);
        }
        ODA_ASSERT_ONCE_X(TDWF, res == eOk);
        ODA_ASSERT_VAR(if (res == eOk))
        {
          ODA_ASSERT_ONCE_X(TDWF, sDwfDeviceName == pPlotSettings->getPlotCfgName());
          ODA_ASSERT_VAR(OdString sDwfMediaName = pPlotSettings->getCanonicalMediaName();)
          ODA_ASSERT(true); // brk// CORE-16434 //ODA_ASSERT_ONCE_X(TDWF, sDwfMediaName.find(L"full_bleed") >= 0);
        }
      }
    }
  }

  if (sExt == L"dwf")
  {
    switch (filterIndex)
    {
    case 0:
      params.setFormat(DW_UNCOMPRESSED_BINARY);
      params.setVersion(nDwf_v60);
      pageData.sLayout = sCurrentLayout; // = OdDbDatabase::cast(pRxDatabase)->findActiveLayout(true);
      params.pageData().append(pageData);
      break;
    case 1:
      params.setFormat(DW_ASCII);
      params.setVersion(nDwf_v60);
      pageData.sLayout = sCurrentLayout; // = OdDbDatabase::cast(pRxDatabase)->findActiveLayout(true);
      params.pageData().append(pageData);
      break;
    case 2:
      params.setFormat(DW_COMPRESSED_BINARY);
      params.setVersion(nDwf_v55);
      break;
    case 3:
      params.setFormat(DW_UNCOMPRESSED_BINARY);
      params.setVersion(nDwf_v55);
      break;
    case 4:
      params.setFormat(DW_ASCII);
      params.setVersion(nDwf_v55);
      break;
    case 5:
      params.setFormat(DW_COMPRESSED_BINARY);
      params.setVersion(nDwf_v42);
      break;
    case 6:
      params.setFormat(DW_UNCOMPRESSED_BINARY);
      params.setVersion(nDwf_v42);
      break;
    case 7:
      params.setFormat(DW_ASCII);
      params.setVersion(nDwf_v42);
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      return false;
    }
  }
  else if (sExt == L"dwfx")
  {
    switch (filterIndex)
    {
    case 0:
      params.setFormat(DW_XPS);
      params.setVersion(nDwf_v6_unknown);
      pageData.sLayout = sCurrentLayout; // = OdDbDatabase::cast(pRxDatabase)->findActiveLayout(true);
      params.pageData().append(pageData);
      break;
    default:
      ODA_FAIL_ONCE(); // TODO
      return false;
    }
  }

  exporter->exportDwf(params);

  return true;
}
