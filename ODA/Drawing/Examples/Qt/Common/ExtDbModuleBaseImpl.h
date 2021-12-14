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
// ExtDbModuleBaseImpl.h
//

#ifndef __EXTDB_MODULE_BASE_IMPL_H__
#define __EXTDB_MODULE_BASE_IMPL_H__

//#include "OdaCommon.h"
#include "ExtDbModule.h"
#include "DbBaseHostAppServices.h"
#include "Ed/EdCommandContext.h" //"ExDbCommandContext.h" "GiContextForDbDatabase.h"
#include "ColorMapping.h"
#include "AbstractViewPE.h"
#include "Ge/GePlane.h"

//////////////////////////////////////////////////////////////////////////

template <class HostAppServ> 
class OdWrapServicesImpl : public HostAppServ
{
protected:
  //ODRX_USING_HEAP_OPERATORS(OdImgDbServicesImpl);

  friend class OdExtDbModuleBaseImpl;
  OdSmartPtr<OdDbBaseHostAppServices> m_pBaseHostAppServices;

  // ----- HostAppServ -----

  virtual OdString findFile(const OdString& filename, OdDbBaseDatabase* pDb = 0, 
                            OdDbBaseHostAppServices::FindFileHint hint = OdDbBaseHostAppServices::kDefault)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->findFile(filename, pDb, hint);
    return HostAppServ::findFile(filename, pDb, hint);
  }

  virtual OdDbHostAppProgressMeter* newProgressMeter()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->newProgressMeter();
    return HostAppServ::newProgressMeter();
  }

  virtual void releaseProgressMeter(OdDbHostAppProgressMeter* pProgressMeter)
  {
    if (!m_pBaseHostAppServices.isNull())
    {
      m_pBaseHostAppServices->releaseProgressMeter(pProgressMeter);
      return;
    }
    HostAppServ::releaseProgressMeter(pProgressMeter);
  }

  virtual const OdString program()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->program();
    return HostAppServ::program();
  }

  virtual const OdString product()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->product();
    return HostAppServ::product();
  }

  virtual const OdString companyName()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->companyName();
    return HostAppServ::companyName();
  }

  virtual ProdIdCode prodcode()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->prodcode();
    return HostAppServ::prodcode();
  }

  virtual const OdString releaseMajorMinorString()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->releaseMajorMinorString();
    return HostAppServ::releaseMajorMinorString();
  }

  virtual int releaseMajorVersion()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->releaseMajorVersion();
    return HostAppServ::releaseMajorVersion();
  }

  virtual int releaseMinorVersion()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->releaseMinorVersion();
    return HostAppServ::releaseMinorVersion();
  }

  virtual const OdString versionString()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->versionString();
    return HostAppServ::versionString();
  }

  virtual void warning(const char* warnVisGroup, const OdString& message)
  {
    if (!m_pBaseHostAppServices.isNull())
    {
      m_pBaseHostAppServices->warning(warnVisGroup, message);
      return;
    }
    HostAppServ::warning(warnVisGroup, message);
  }

  virtual void warning(OdWarning warningOb)
  {
    if (!m_pBaseHostAppServices.isNull())
    {
      m_pBaseHostAppServices->warning(warningOb);
      return;
    }
    HostAppServ::warning(warningOb);
  }

  virtual OdString getErrorDescription(unsigned int errorCode)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getErrorDescription(errorCode);
    return HostAppServ::getErrorDescription(errorCode);
  }

  virtual OdString formatMessage(unsigned int errorCode, va_list* argList)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->formatMessage(errorCode, argList);
    return HostAppServ::formatMessage(errorCode, argList);
  }

  virtual OdDbUndoControllerPtr newUndoController()
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->newUndoController();
    return HostAppServ::newUndoController();
  }

  virtual void auditPrintReport(OdAuditInfo * pAuditInfo, const OdString& strLine, int printDest) const
  {
    if (!m_pBaseHostAppServices.isNull())
    {
      m_pBaseHostAppServices->auditPrintReport(pAuditInfo, strLine, printDest);
      return;
    }
    HostAppServ::auditPrintReport(pAuditInfo, strLine, printDest);
  }

  virtual bool ttfFileNameByDescriptor(const OdTtfDescriptor& description, OdString& filename)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->ttfFileNameByDescriptor(description, filename);
    return HostAppServ::ttfFileNameByDescriptor(description, filename);
  }

  virtual OdString getAlternateFontName() const
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getAlternateFontName();
    return HostAppServ::getAlternateFontName();
  }

  virtual OdString getFontMapFileName() const
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getFontMapFileName();
    return HostAppServ::getFontMapFileName();
  }

  virtual OdString getPreferableFont(const OdString& fontName, OdFontType fontType)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getPreferableFont(fontName, fontType);
    return HostAppServ::getPreferableFont(fontName, fontType);
  }

  virtual OdString getSubstituteFont(const OdString& fontName, OdFontType fontType)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->getSubstituteFont(fontName, fontType);
    return HostAppServ::getSubstituteFont(fontName, fontType);
  }

  virtual OdString fileDialog(int flags,
                              const OdString& dialogCaption = OdString::kEmpty,
                              const OdString& defExt = OdString::kEmpty,
                              const OdString& defFilename = OdString::kEmpty,
                              const OdString& filter = OdString::kEmpty)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->fileDialog(flags, dialogCaption, defExt, defFilename, filter);
    return HostAppServ::fileDialog(flags, dialogCaption, defExt, defFilename, filter);
  }

  virtual OdGsDevicePtr gsBitmapDevice(OdRxObject* pViewObj = NULL,
                                       OdDbBaseDatabase* pDb = NULL,
                                       OdUInt32 flags = 0)
  {
    if (!m_pBaseHostAppServices.isNull())
      return m_pBaseHostAppServices->gsBitmapDevice(pViewObj, pDb, flags);
    return HostAppServ::gsBitmapDevice(pViewObj, pDb, flags);
  }

  // ----- -----

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

//static OdImgDbServicesImpl* appImgServicesImpl()
//{
//  static OdStaticRxObject<OdImgDbServicesImpl> g_services;
//  return &g_services;
//}

//OdDbHostAppServices* appImgServices()
//{
//  return appImgServicesImpl();
//}

//////////////////////////////////////////////////////////////////////////

class OdExtDbModuleBaseImpl : public OdExtDbModule
{
public:
  OdExtDbModuleBaseImpl();
  virtual ~OdExtDbModuleBaseImpl();

  // OdRxModule overridden
  virtual void initApp();
  virtual void uninitApp();
  virtual bool isValid(); // to unload if using components are missing (false)

  // init issues
  //virtual void setBaseHostAppServices(OdDbBaseHostAppServices* pServices);
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
  virtual OdRxObject* getWrappedDatabase(OdRxObject* pRxDb); // for TD_TvfDb (pRxDb is a wrapper)
  virtual OdString getFilename(OdRxObject* pRxDb);

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
  virtual OdGiDefaultContextPtr createGiContext(OdRxObject* pRxDb);
  virtual OdGsDevicePtr setupActiveLayoutViews(OdRxObject* pRxDb, OdGsDevice* pGsDevice, OdGiDefaultContext* pGiCtx,
                                               void* winId = NULL, long widthQWidget = 800, long heightQWidget = 600);
  virtual OdGsDevicePtr takeUnderlyingDeviceAndViewports(OdGsDevicePtr pGsDevice,
                                                         OdArray<OdGsViewPtr>& viewportsToDeleteAfterUpdate); // out
  virtual void enableGsModel(class OdGiDefaultContext* pCtx, bool bGsCache);
  virtual bool getModelBackground(OdRxObject* pRxDatabase, ODCOLORREF& clrBackground,
                                                           ODGSPALETTE* pPalette = NULL);
  virtual bool setModelBackground(OdRxObject* pRxDatabase, ODCOLORREF clrBackground);
  virtual bool argsAtSetupLayoutViews(OdRxObject* pRxDatabase, bool& bDoZoomExtents, OdGsView::RenderMode& rm);
  virtual bool isPerspective(OdRxObject* pRxDatabase, OdGsDevice* pGsDevice = NULL);
  virtual bool setPerspective(OdRxObject* pRxDatabase, bool bVal);

  // get point, zoom etc issues
  virtual OdGsView* getActiveGsView(OdGsDevice* pGsDevice);
  virtual OdGsModel* getGsModel(OdGsDevice* pGsDevice);
  virtual bool getUcsPlane(OdRxObject* pRxDatabase, OdGsView* pGsView, OdGePlane& plane,
                           const OdGeVector3d* pvHintDirToIntersect = NULL);
  virtual bool setGsViewToDb(OdRxObject* pRxDatabase, OdGsView* pGsView);

  virtual OdUnitsFormatter* getFormatter(OdRxObject* pRxDatabase);

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
  //
  virtual OdGiDrawablePtr cloneEntity(OdDbStub* id);
  virtual OdRxObjectPtr openObject(OdDbStub* id, bool isForWriteMode = false);
  // alternative to OdDbGripPointsPE methods
  // TODO redesign OdDbGripPointsPE(OdDgGripPointsPE...) to OdRxObject & use it instead next methods
  // next methods can be invoked for non-resident clone object (see also OdBaseGripManager) 
  virtual OdResult getGripPointsAtSubentPath(OdRxObject* pRxDb, OdGiDrawable* pEntity, const OdDbBaseFullSubentPath& path, OdDbGripDataPtrArray& grips, double curViewUnitSize, int gripSize, const OdGeVector3d& curViewDir, const OdUInt32 bitflags);
  virtual OdResult getGripPoints(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdDbGripDataPtrArray& grips, double curViewUnitSize, int gripSize, const OdGeVector3d& curViewDir, int bitFlags);
  virtual OdResult getGripPoints(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdGePoint3dArray& gripPoints);
  virtual OdResult moveGripPointsAtSubentPaths(OdRxObject* pRxDb, OdGiDrawable* pEntity, const OdDbBaseFullSubentPathArray& paths, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, OdUInt32 bitflags);
  virtual OdResult moveGripPointsAt(OdRxObject* pRxDb, OdGiDrawable* pEntity, const OdDbVoidPtrArray& gripAppData, const OdGeVector3d& offset, int bitFlags);
  virtual OdResult moveGripPointsAt(OdRxObject* pRxDb, OdGiDrawable* pEntity, const OdIntArray& indices, const OdGeVector3d& offset);
  virtual void subentGripStatus(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdDb::GripStat status, const OdDbBaseFullSubentPath& subentity);
  virtual void gripStatus(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdDb::GripStat st);
  virtual void dragStatus(OdRxObject* pRxDb, OdGiDrawable* pEntity, OdDb::DragStat st);
  virtual bool isModel(OdRxObject* pRxDb, OdGiDrawable* pEntity);
  //
  virtual bool isErased(OdDbStub* id);
  virtual OdDbHandle getElementHandle(OdDbStub* id);
  virtual OdString getElementClassName(OdDbStub* id);

  // undo/redo issues
  virtual bool startUndoRecord(OdRxObject* pRxDb);
  virtual bool blockUndoRecording(OdRxObject* pRxDb, bool bBegin);
  virtual bool isUndoBlockStarted(OdRxObject* pRxDb);
  virtual bool hasUndo(OdRxObject* pRxDb);
  virtual void undo(OdRxObject* pRxDb);
  virtual bool hasRedo(OdRxObject* pRxDb);
  virtual void redo(OdRxObject* pRxDb);

  // command issues
  virtual bool cmdErase(OdEdCommandContext* pCmdCtx);

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
  virtual bool importFromFile(OdRxObject* pRxDatabase, const OdString& fileName, ODCOLORREF& clrBackground);
  virtual bool importFromFile(OdEdCommandContext* pCmdCtx, const OdString& fileName, ODCOLORREF& clrBackground);
  virtual bool exportToFile(OdRxObject* pRxDatabase, const OdString& fileName, int filterIndex, ODCOLORREF clrBackground);
  virtual bool exportToFile(OdEdCommandContext* pCmdCtx, const OdString& fileName, int filterIndex, ODCOLORREF clrBackground);

protected:
  OdRxModulePtr m_pDbModule;
};

inline OdExtDbModuleBaseImpl::OdExtDbModuleBaseImpl()
{
}

inline OdExtDbModuleBaseImpl::~OdExtDbModuleBaseImpl()
{
}

inline void OdExtDbModuleBaseImpl::initApp()
{
  ODA_ASSERT_ONCE(m_pDbModule.get());
}

inline void OdExtDbModuleBaseImpl::uninitApp()
{
  ODA_ASSERT_ONCE(m_pDbModule.get());
  m_pDbModule = NULL;
}

inline bool OdExtDbModuleBaseImpl::isValid() // to unload if using components are missing (false)
{
  return true;
}

// init issues
//inline void OdExtDbModuleBaseImpl::setBaseHostAppServices(OdDbBaseHostAppServices* pServices)
//{
//  appImgServicesImpl()->m_pBaseHostAppServices = pServices;
//}

inline void OdExtDbModuleBaseImpl::initOdExtPE()
{
  //  ::initOdDgSysVarPE();
  //  appImgServicesImpl()->restoreRegVariables();
}

inline void OdExtDbModuleBaseImpl::uninitOdExtPE()
{
  //  appImgServicesImpl()->saveRegVariables();
  //  ::uninitOdDgSysVarPE();
}

inline OdStringArray OdExtDbModuleBaseImpl::resetRegVariables() // set to default values
{
  return OdStringArray(); //return appImgServicesImpl()->resetRegVariables();
}

inline bool OdExtDbModuleBaseImpl::getPreviewBitmap(const OdString&, //fileName, 
                                                    OdBinaryData&) // dataBmp) // out
{
  return false; // is used in cloud only for fixed set of bridge modules
}

inline OdRxObjectPtr OdExtDbModuleBaseImpl::readFile(const OdString& fileName, 
                                                     Oda::FileShareMode shareMode) // = Oda::kShareDenyWrite
{
  OdRxObjectPtr pRxDb = createUninitializedDatabase();
  if (!loadToUninitializedDatabase(pRxDb, fileName, shareMode))
    pRxDb = NULL;
  return pRxDb;
}

inline OdRxObjectPtr OdExtDbModuleBaseImpl::createUninitializedDatabase()
{
  ODA_FAIL_ONCE(); // should be overridden
  throw OdError(eNotImplementedYet);
}

inline bool OdExtDbModuleBaseImpl::loadToUninitializedDatabase(OdRxObjectPtr, // pRxDb, 
                                                               const OdString&, // fileName, 
                                                               Oda::FileShareMode) // shareMode // = Oda::kShareDenyWrite
{
  ODA_FAIL_ONCE(); // should be overridden
  throw OdError(eNotImplementedYet);
}

inline bool OdExtDbModuleBaseImpl::writeFile(const OdString&, // fileName,
                                             OdEdCommandContext* ctx)
{
  ODA_ASSERT_ONCE(ctx && ctx->baseDatabase());
  if (!isResponsible(ctx->baseDatabase()))
    return false;

  ctx->userIO()->putError(L"This format is not supported to write.") ;
  return true;
}

inline OdRxObject* OdExtDbModuleBaseImpl::getWrappedDatabase(OdRxObject* pRxDb)
{
  return pRxDb; // pRxDb is a wrapper for for TD_TvfDb only
}

inline OdString OdExtDbModuleBaseImpl::getFilename(OdRxObject* pRxDb)
{
  OdDbBaseDatabasePEPtr pDbPe = OdDbBaseDatabasePE::cast(pRxDb);
  if (pDbPe.get())
    return pDbPe->getFilename(pRxDb);
  ODA_FAIL_ONCE(); // should be overridden
  throw OdError(eNotImplementedYet);
}

inline OdEdBaseIO* OdExtDbModuleBaseImpl::baseIO(OdEdCommandContext* pCtx)
{
  if (!pCtx || !isResponsible(pCtx->baseDatabase()))
    return NULL;
  //ExDbCommandContext* pCmdCtx = static_cast<ExDbCommandContext*>(pCtx);
  //return pCmdCtx->baseIO();
  return pCtx->baseIO();
}

inline OdEdCommandContextPtr OdExtDbModuleBaseImpl::cloneCommandContext(OdEdCommandContext* pCtx,
                                                                        OdEdBaseIO* pIOStream, // = NULL
                                                                        OdRxObject* pRxDb) // = NULL
{
  ODA_ASSERT_ONCE(pCtx && isResponsible(pRxDb));
  if (!pCtx || !isResponsible(pRxDb))
    return OdEdCommandContextPtr();
  //ExDbCommandContext* pCmdCtx = static_cast<ExDbCommandContext*>(pCtx);
  //return pCmdCtx->cloneObject(pIOStream, pRxDb);
  return pCtx->cloneObject(pIOStream, pRxDb);
}

inline OdGsDevicePtr OdExtDbModuleBaseImpl::createDevice(OdGsModulePtr pGs,
                                                         OdRxObjectPtr pRxDatabase)
{
  OdGsDevicePtr pGsDevice;
  ODA_ASSERT_ONCE(pGs.get() && pRxDatabase.get());

  if (pGs.isNull() || pRxDatabase.isNull() || !isResponsible(pRxDatabase))
    return pGsDevice;

  pGsDevice = pGs->createDevice();
  return pGsDevice;
}

inline OdGsDevicePtr OdExtDbModuleBaseImpl::takeUnderlyingDeviceAndViewports(OdGsDevicePtr pGsDevice,
                                                                             OdArray<OdGsViewPtr>& viewportsToDeleteAfterUpdate) // out
{
  ODA_FAIL_ONCE(); // TODO
  return OdGsDevicePtr();
}

inline OdGsDevicePtr OdExtDbModuleBaseImpl::createBitmapDevice(OdGsModulePtr pGs, 
                                                               OdRxObjectPtr pRxDatabase)
{
  OdGsDevicePtr pGsDevice;
  ODA_ASSERT_ONCE(pGs.get() && pRxDatabase.get());
  if (pGs.isNull() || pRxDatabase.isNull() || !isResponsible(pRxDatabase))
    return pGsDevice;

  pGsDevice = pGs->createBitmapDevice();
  return pGsDevice;
}

inline OdGiDefaultContextPtr OdExtDbModuleBaseImpl::createGiContext(OdRxObject* pRxDb)
{
  OdGiDefaultContextPtr pGiCtx;
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  if (pDbPE.get())
    pGiCtx = pDbPE->createGiContext(pRxDb);
  ODA_ASSERT_ONCE(pGiCtx.get()); // TODO
  return pGiCtx;
}

inline OdGsDevicePtr OdExtDbModuleBaseImpl::setupActiveLayoutViews(OdRxObject* pRxDb, 
                                                                   OdGsDevice* pGsDevice, OdGiDefaultContext* pGiCtx,
                                                                   void*, // winId = NULL
                                                                   long, // widthQWidget = 800
                                                                   long) // heightQWidget = 600
{
  OdDbBaseDatabasePEPtr pDbPE = OdDbBaseDatabasePE::cast(pRxDb);
  if (pDbPE.get())
    return pDbPE->setupActiveLayoutViews(pGsDevice, pGiCtx);
  ODA_FAIL_ONCE(); //should be overrode
  return OdGsDevicePtr(); // TODO
}

inline void OdExtDbModuleBaseImpl::enableGsModel(OdGiDefaultContext* pCtx, bool bGsCache)
{
  ODA_ASSERT_ONCE(pCtx);
  if (!pCtx)
    return;
  pCtx->enableGsModel(bGsCache);
}

inline bool isDarkPalette(ODCOLORREF bgColor)
{
  return (ODGETRED(bgColor) < 140) && (ODGETGREEN(bgColor) < 140) && (ODGETBLUE(bgColor) < 140);
}

inline bool OdExtDbModuleBaseImpl::getModelBackground(OdRxObject* pRxDatabase,
                                                      ODCOLORREF& clrBackground,
                                                      ODGSPALETTE* pPalette) // = NULL
{
  if (!isResponsible(pRxDatabase))
    return false;

  //OdImgModelPtr pModel = pImgDb->getActiveModelId().safeOpenObject();
  //clrBackground = pModel->getBackground();

  if (pPalette)
  {
    ODGSPALETTE& palette = *pPalette;
    ODA_ASSERT_ONCE(!palette.size());
    //const ODCOLORREF* refColors = OdDgColorTable::currentPalette(pImgDb);
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

inline bool OdExtDbModuleBaseImpl::setModelBackground(OdRxObject* pRxDatabase, 
                                                      ODCOLORREF clrBackground)
{
  ODA_FAIL_ONCE(); // ODA_ASSERT_ONCE(!isResponsible(pRxDatabase)); // TODO
  return false;
}

inline bool OdExtDbModuleBaseImpl::argsAtSetupLayoutViews(OdRxObject*, bool& bDoZoomExtents, OdGsView::RenderMode& rm)
{
  bDoZoomExtents = false;
  rm = OdGsView::kNone; // undef
  return false;
}

inline bool OdExtDbModuleBaseImpl::isPerspective(OdRxObject* pRxDatabase,
                                                 OdGsDevice* pGsDevice) // = NULL
{
  ODA_ASSERT_ONCE(isResponsible(pRxDatabase));
  if (pGsDevice)
  {
    if (OdGsView* pView = getActiveGsView(pGsDevice))
      return pView->isPerspective();
  }
  ODA_FAIL_ONCE(); // TODO
  return false;
}

inline bool OdExtDbModuleBaseImpl::setPerspective(OdRxObject* pRxDatabase, bool bVal)
{
  ODA_ASSERT_ONCE(isResponsible(pRxDatabase));
  ODA_FAIL_ONCE(); // TODO
  return false;
}

inline OdGsView* OdExtDbModuleBaseImpl::getActiveGsView(OdGsDevice* pGsDevice)
{
  ODA_FAIL_ONCE(); // TODO
  return NULL;
  //OdImgGsLayoutHelperPtr pLayoutHelper = OdImgGsLayoutHelper::cast(pGsDevice);
  //ODA_ASSERT_ONCE(pLayoutHelper.get());
  //if (pLayoutHelper.isNull())
  //  return NULL;
  //OdGsView* pGsView = pLayoutHelper->activeView().get();
  //ODA_ASSERT_ONCE(pGsView);
  //return pGsView;
}

inline OdGsModel* OdExtDbModuleBaseImpl::getGsModel(OdGsDevice* pGsDevice)
{
  ODA_FAIL_ONCE(); // TODO
  return NULL;
  //OdImgGsLayoutHelperPtr pLayoutHelper = OdImgGsLayoutHelper::cast(pGsDevice);
  //ODA_ASSERT_ONCE(pLayoutHelper.get());
  //OdGsModel* pGsModel = pLayoutHelper->gsModel();
  //// it is NULL if GS cache off //ODA_ASSERT_ONCE(pGsModel);
  //return pGsModel;
}

inline OdUnitsFormatter* OdExtDbModuleBaseImpl::getFormatter(OdRxObject* pRxDatabase)
{
  ODA_FAIL_ONCE(); // ODA_ASSERT_ONCE(isResponsible(pRxDatabase)); // TODO
  return NULL;
  //return &pImgDb->formatter(); // return &m_formatter; // TODO special for each model in view (via active model)
}

inline bool OdExtDbModuleBaseImpl::getUcsPlane(OdRxObject* pRxDatabase, 
                                               OdGsView* pGsView, OdGePlane& plane,
                                               const OdGeVector3d* pvHintDirToIntersect) // = NULL
{
  ODA_ASSERT_ONCE(pRxDatabase && pGsView);
  if (!isResponsible(pRxDatabase))
    return false;

  OdGsClientViewInfo cliInfo;
  pGsView->clientViewInfo(cliInfo);
  if (cliInfo.viewportObjectId)
  {
    OdRxObjectPtr pVpObj = openObject(cliInfo.viewportObjectId);
    OdAbstractViewPEPtr pAVD = OdAbstractViewPE::cast(pVpObj);
    if (pAVD.get())
    {
      OdGePoint3d ucsOrigin;
      OdGeVector3d ucsXAxis, ucsYAxis;
      pAVD->getUcs(pVpObj, ucsOrigin, ucsXAxis, ucsYAxis);
      plane.set(ucsOrigin, ucsXAxis, ucsYAxis);
      if (pvHintDirToIntersect && pvHintDirToIntersect->isPerpendicularTo(plane.normal()))
      {
        // to solve zoom & orbit problems for some view of Bim files
        OdGeVector3d vDelta = 1e-5 * plane.normal();
        (ucsXAxis -= vDelta).normalize();
        (ucsYAxis -= vDelta).normalize();
        plane.set(ucsOrigin, ucsXAxis, ucsYAxis);
        ODA_ASSERT_VAR(if (pvHintDirToIntersect->isPerpendicularTo(plane.normal())))
          ODA_ASSERT_VAR(vDelta = vDelta;) // brl
      }
      return true;
    }
  }

  plane.set(OdGePoint3d::kOrigin, OdGeVector3d::kZAxis);
  if (pvHintDirToIntersect && pvHintDirToIntersect->isPerpendicularTo(plane.normal()))
  {
    double dpX = pvHintDirToIntersect->dotProduct(OdGeVector3d::kXAxis);
    if (dpX < 0.)
      dpX = pvHintDirToIntersect->dotProduct(- OdGeVector3d::kXAxis);
    double dpY = pvHintDirToIntersect->dotProduct(OdGeVector3d::kYAxis);
    if (dpY < 0.)
      dpY = pvHintDirToIntersect->dotProduct(- OdGeVector3d::kYAxis);
    OdGeVector3d axis = (dpX > dpY) ? OdGeVector3d::kXAxis : OdGeVector3d::kYAxis;

    plane.set(OdGePoint3d::kOrigin, axis);
  }
  return true;
}

inline bool OdExtDbModuleBaseImpl::setGsViewToDb(OdRxObject* pRxDatabase, OdGsView* pGsView)
{
  ODA_ASSERT_ONCE(isResponsible(pRxDatabase));

  OdGsClientViewInfo cliInfo;
  pGsView->clientViewInfo(cliInfo);
  if (cliInfo.viewportObjectId)
  {
    OdRxObjectPtr pVpObj = openObject(cliInfo.viewportObjectId, true);
    OdAbstractViewPEPtr pAVD = OdAbstractViewPE::cast(pVpObj);
    if (pAVD.get())
    {
      pAVD->setView(pVpObj, pGsView);
      return true;
    }
  }

  return false;
}

// selection issues
inline OdSelectionSetPtr OdExtDbModuleBaseImpl::createSelectionSet(OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(isResponsible(pRxDatabase));
  return OdSelectionSetPtr();
}

inline OdSelectionSetPtr OdExtDbModuleBaseImpl::select(OdGsView* gsView,
                                                       int nPoints,
                                                       const OdGePoint3d* wcsPts,
                                                       OdDbVisualSelection::Mode mode, // = OdDbVisualSelection::kCrossing
                                                       OdDbVisualSelection::SubentSelectionMode sm, // = OdDbVisualSelection::kDisableSubents
                                                       const OdRxObject* pFilter) // = NULL
{
  ODA_FAIL_ONCE(); // TODO
  return OdSelectionSetPtr();
}

inline void OdExtDbModuleBaseImpl::getVisibleAllIds(OdRxObject* pRxDatabase, 
                                                    OdDbStubPtrArray& ids)
{
  ODA_FAIL_ONCE(); // ODA_ASSERT_ONCE(!isResponsible(pRxDatabase)); // TODO
  ids.clear();
}

inline OdDbStub* OdExtDbModuleBaseImpl::getVisibleLastId(OdRxObject* pRxDatabase)
{
  ODA_FAIL_ONCE(); // ODA_ASSERT_ONCE(!isResponsible(pRxDatabase)); // TODO
  return NULL;
}

inline void OdExtDbModuleBaseImpl::highlight(OdGsView* gsView, bool bValue,
                                             OdDbStub* id, const OdDbBaseFullSubentPath* pPath)
{
  ODA_ASSERT_ONCE(id);
  if (!id)
    return;
  OdRxObject* pRxDatabase = baseDatabaseBy(id);
  if (!isResponsible(pRxDatabase))
    return;

  ODA_FAIL_ONCE(); // TODO
}

inline OdGiDrawablePtr OdExtDbModuleBaseImpl::cloneEntity(OdDbStub* id)
{
  return openObject(id, true);
}

inline OdRxObjectPtr OdExtDbModuleBaseImpl::openObject(OdDbStub* id, 
                                                       bool isForWriteMode) // = false
{
  OdRxObjectPtr pRes;
  ODA_ASSERT_ONCE(id);
  if (!id || isForWriteMode)
    return pRes;
  OdRxObject* pRxDatabase = baseDatabaseBy(id);
  if (!isResponsible(pRxDatabase))
    return pRes;
  OdDbBaseDatabasePEPtr pDbPe = OdDbBaseDatabasePE::cast(pRxDatabase);
  ODA_ASSERT_ONCE(pDbPe.get());
  if (pDbPe.get())
    pRes = pDbPe->openObject(id);
  return pRes;
}

// alternative to OdDbGripPointsPE methods
// TODO redesign OdDbGripPointsPE(OdDgGripPointsPE...) to OdRxObject & use it instead next methods
// next methods can be invoked for non-resident clone object (see also OdBaseGripManager) 
inline OdResult OdExtDbModuleBaseImpl::getGripPointsAtSubentPath(OdRxObject* pRxDb, 
                                                                 OdGiDrawable*, //pEntity 
                                                                 const OdDbBaseFullSubentPath&, //path
                                                                 OdDbGripDataPtrArray&, //grips 
                                                                 double, //curViewUnitSize 
                                                                 int, //gripSize 
                                                                 const OdGeVector3d&, //curViewDir, 
                                                                 const OdUInt32) //bitflags
{
  if (!isResponsible(pRxDb))
    return eNotApplicable;
  //ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

inline OdResult OdExtDbModuleBaseImpl::getGripPoints(OdRxObject* pRxDb, 
                                                     OdGiDrawable*, //pEntity
                                                     OdDbGripDataPtrArray&, //grips
                                                     double, //curViewUnitSize
                                                     int, //gripSize
                                                     const OdGeVector3d&, //curViewDir
                                                     int) //bitFlags
{
  if (!isResponsible(pRxDb))
    return eNotApplicable;
  //ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

inline OdResult OdExtDbModuleBaseImpl::getGripPoints(OdRxObject* pRxDb,
                                                     OdGiDrawable*, //pEntity
                                                     OdGePoint3dArray&) //gripPoints
{
  if (!isResponsible(pRxDb))
    return eNotApplicable;
  //ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

inline OdResult OdExtDbModuleBaseImpl::moveGripPointsAtSubentPaths(OdRxObject* pRxDb, 
                                                                   OdGiDrawable*, //pEntity
                                                                   const OdDbBaseFullSubentPathArray&, //paths
                                                                   const OdDbVoidPtrArray&, //gripAppData
                                                                   const OdGeVector3d&, //offset
                                                                   OdUInt32) //bitflags
{
  if (!isResponsible(pRxDb))
    return eNotApplicable;
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

inline OdResult OdExtDbModuleBaseImpl::moveGripPointsAt(OdRxObject* pRxDb, 
                                                        OdGiDrawable*, //pEntity
                                                        const OdDbVoidPtrArray&, //gripAppData
                                                        const OdGeVector3d&, //offset
                                                        int) //bitFlags
{
  if (!isResponsible(pRxDb))
    return eNotApplicable;
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

inline OdResult OdExtDbModuleBaseImpl::moveGripPointsAt(OdRxObject* pRxDb, 
                                                        OdGiDrawable*, //pEntity
                                                        const OdIntArray&, //indices
                                                        const OdGeVector3d&) //offset
{
  if (!isResponsible(pRxDb))
    return eNotApplicable;
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

inline void OdExtDbModuleBaseImpl::subentGripStatus(OdRxObject* pRxDb, 
                                                    OdGiDrawable*, //pEntity
                                                    OdDb::GripStat, //status
                                                    const OdDbBaseFullSubentPath&) //subentity
{
  if (!isResponsible(pRxDb))
  {
    ODA_FAIL_ONCE(); // test
  }
  return;
}

inline void OdExtDbModuleBaseImpl::gripStatus(OdRxObject* pRxDb, 
                                              OdGiDrawable*, //pEntity
                                              OdDb::GripStat) //st
{
  if (!isResponsible(pRxDb))
  {
    ODA_FAIL_ONCE(); // test
  }
  return;
}

inline void OdExtDbModuleBaseImpl::dragStatus(OdRxObject* pRxDb, 
                                              OdGiDrawable*, //pEntity
                                              OdDb::DragStat) //st
{
  if (!isResponsible(pRxDb))
  {
    ODA_FAIL_ONCE(); // test
  }
  return;
}

inline bool OdExtDbModuleBaseImpl::isModel(OdRxObject* pRxDb, 
                                           OdGiDrawable*) //pEntity
{
  if (!isResponsible(pRxDb))
  {
    ODA_FAIL_ONCE(); // test
  }
  return true;
}

//inline bool OdExtDbModuleBaseImpl::getGripPoints(OdDbStub* id,
//                                                 OdGePoint3dArray& gripPoints,
//                                                 OdEdCommandContext* pCmdCtx) // = NULL
//{
//  ODA_ASSERT_ONCE(id);
//  if (!id)
//    return false;
//  OdRxObject* pRxDatabase = baseDatabaseBy(id);
//  if (!isResponsible(pRxDatabase))
//    return false;
//  ODA_FAIL_ONCE(); // TODO
//  return false;
//}
//
//inline bool OdExtDbModuleBaseImpl::getGripPoints(OdDbStub* id,
//                                                 OdDbGripDataPtrArray&, // grips
//                                                 double, // curViewUnitSize
//                                                 int, // gripSize
//                                                 const OdGeVector3d&, // curViewDir
//                                                 int) // bitFlags // = 0
//{
//  ODA_ASSERT_ONCE(id);
//  if (!id)
//    return false;
//  OdRxObject* pRxDatabase = baseDatabaseBy(id);
//  if (!isResponsible(pRxDatabase))
//    return false;
//  ODA_FAIL_ONCE(); // TODO
//  return false;
//}
//
//inline bool OdExtDbModuleBaseImpl::getGripPointsAtSubentPath(const OdDbBaseFullSubentPath& path, 
//                                                             OdDbGripDataPtrArray&, // grips
//                                                             double, // curViewUnitSize
//                                                             int, // gripSize
//                                                             const OdGeVector3d&, // curViewDir
//                                                             OdUInt32) // bitflags // = 0
//{
//  if (!path.objectIds().size())
//    return false;
//  OdRxObject* pRxDatabase = baseDatabaseBy(path.objectIds().first());
//  if (!isResponsible(pRxDatabase))
//    return false;
//  ODA_FAIL_ONCE(); // TODO
//  return false;
//}

inline bool OdExtDbModuleBaseImpl::isErased(OdDbStub* id)
{
  ODA_ASSERT_ONCE(id);
  if (!id)
    return false;
  OdRxObject* pRxDatabase = baseDatabaseBy(id);
  if (!isResponsible(pRxDatabase))
    return false;
  ODA_FAIL_ONCE(); // TODO
  return false;
}

inline OdDbHandle OdExtDbModuleBaseImpl::getElementHandle(OdDbStub* id)
{
  ODA_FAIL_ONCE(); // TODO
  return OdDbHandle();
}

inline OdString OdExtDbModuleBaseImpl::getElementClassName(OdDbStub* id)
{
  ODA_ASSERT_ONCE(id);
  if (!id)
    return OdString::kEmpty;
  OdRxObject* pRxDatabase = baseDatabaseBy(id);
  if (!isResponsible(pRxDatabase))
    return OdString::kEmpty;
  ODA_FAIL_ONCE(); // TODO
  return OdString::kEmpty;
}

inline bool OdExtDbModuleBaseImpl::startUndoRecord(OdRxObject*) //pRxDb
{
  return false;
}

inline bool OdExtDbModuleBaseImpl::blockUndoRecording(OdRxObject*, //pRxDb
                                               bool) //bBegin
{
  return false;
}

inline bool OdExtDbModuleBaseImpl::isUndoBlockStarted(OdRxObject*) //pRxDb
{
  return false;
}

inline bool OdExtDbModuleBaseImpl::hasUndo(OdRxObject*) //pRxDb
{
  return false;
}

inline void OdExtDbModuleBaseImpl::undo(OdRxObject*) //pRxDb
{
}

inline bool OdExtDbModuleBaseImpl::hasRedo(OdRxObject*) //pRxDb
{
  return false;
}

inline void OdExtDbModuleBaseImpl::redo(OdRxObject*) //pRxDb
{
}

inline bool OdExtDbModuleBaseImpl::cmdErase(OdEdCommandContext* pCmdCtx)
{
  if (!isResponsible(pCmdCtx->baseDatabase()))
    return false;
  ODA_FAIL_ONCE(); // TODO
  return false;
}

inline bool OdExtDbModuleBaseImpl::isResponsible(OdRxObject*) // pRxDatabase
{
  return false; // usable if import/export only
}

inline OdStringArray OdExtDbModuleBaseImpl::getExtensions(enum OdEd::GetFilePathFlags, // flg // = OdEd::kGfpForOpen
                                                          OdRxObject*, // pRxDatabase // = NULL
                                                          bool) // bWithExtSynonyms //= false
{
  return OdStringArray();
}

inline OdString OdExtDbModuleBaseImpl::getFilterByExtension(const OdString&, // sExt,
                                                            int, // index // = 0
                                                            enum OdEd::GetFilePathFlags, // flg // = OdEd::kGfpForOpen
                                                            OdStringArray*) // pExtSynonyms // = NULL
{
  return OdString::kEmpty;
}

// import / export issues

inline OdStringArray OdExtDbModuleBaseImpl::getImportExportExtensions(OdRxObject*, // pRxDatabase
                                                                      bool, // isForImport // = true
                                                                      bool) // bWithExtSynonyms // = false
{
  return OdStringArray();
}

inline OdString OdExtDbModuleBaseImpl::getImportExportFilterByExtension(const OdString&, // sExt
                                                                        int, // index // = 0
                                                                        bool, // isForImport // = true
                                                                        OdStringArray*) // pExtSynonyms // = NULL
{
  return OdString::kEmpty;
}

inline bool OdExtDbModuleBaseImpl::importFromFile(OdRxObject*, // pRxDatabase
                                                  const OdString&, // fileName
                                                  ODCOLORREF&) //  clrBackground // in/out
{
  ODA_FAIL_ONCE();
  return false;
}

inline bool OdExtDbModuleBaseImpl::importFromFile(OdEdCommandContext* pCmdCtx, 
                                                  const OdString& fileName, 
                                                  ODCOLORREF& clrBackground)
{
  ODA_ASSERT_ONCE(pCmdCtx);
  return importFromFile(pCmdCtx->baseDatabase(), fileName, clrBackground);
}

inline bool OdExtDbModuleBaseImpl::exportToFile(OdRxObject*, // pRxDatabase
                                                const OdString&, // fileName
                                                int, // filterIndex
                                                ODCOLORREF) // clrBackground
{
  ODA_FAIL_ONCE();
  return false;
}

inline bool OdExtDbModuleBaseImpl::exportToFile(OdEdCommandContext* pCmdCtx, 
                                                const OdString& fileName, 
                                                int filterIndex, 
                                                ODCOLORREF clrBackground)
{
  ODA_ASSERT_ONCE(pCmdCtx);
  return exportToFile(pCmdCtx->baseDatabase(), fileName, filterIndex, clrBackground);
}

#endif // __EXTDB_MODULE_BASE_IMPL_H__
