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
// ImgDbModule.cpp
//

#include "OdaCommon.h"

#include "ExtDbModuleBaseImpl.h"
#include "DbHostAppServices.h"
#include "ExDynamicModule.h" // #include "RxDynamicModule.h" 
#include "ColorMapping.h"
#include "DbDictionary.h"
#include "RxRasterServices.h"
#include "DbOle2Frame.h"
#include "OleItemHandler.h"
#include "DbBlockTableRecord.h"

#include "AbstractViewPE.h"
#include "Gi/GiRasterWrappers.h"
#include "SysVarPE.h"

//////////////////////////////////////////////////////////////////////////

//appImgServicesImpl()->m_pBaseHostAppServices = pServices;

//static OdImgDbServicesImpl* appImgServicesImpl()
//{
//  static OdStaticRxObject<OdImgDbServicesImpl> g_services;
//  return &g_services;
//}

static OdDbHostAppServices* s_pServices = NULL;
OdDbHostAppServices* appImgServices()
{
  ODA_ASSERT_ONCE(s_pServices);
  return s_pServices;
}

//////////////////////////////////////////////////////////////////////////

class OdImgDbModuleImpl : public OdExtDbModuleBaseImpl
{
public:
  OdImgDbModuleImpl();
  virtual ~OdImgDbModuleImpl();

  //// OdRxModule overridden
  virtual void initApp();
  virtual void uninitApp();
  virtual bool isValid(); // to unload if using components are missing (false)

  //// init issues
  virtual void setBaseHostAppServices(OdDbBaseHostAppServices* pServices);

  //// load / save issues
  virtual OdRxObjectPtr readFile(const OdString& fileName, 
                                 Oda::FileShareMode shareMode = Oda::kShareDenyWrite);
  virtual OdRxObjectPtr createUninitializedDatabase();
  virtual bool loadToUninitializedDatabase(OdRxObjectPtr pRxDb, 
                                           const OdString& fileName, 
                                           Oda::FileShareMode shareMode = Oda::kShareDenyWrite);

  // render issues
  //virtual bool argsAtSetupLayoutViews(OdRxObject* pRxDatabase, bool& bDoZoomExtents, OdGsView::RenderMode& rm);

  // filters issues
  virtual bool isResponsible(OdRxObject* pRxDatabase);
  virtual OdStringArray getExtensions(enum OdEd::GetFilePathFlags flg = OdEd::kGfpForOpen, 
                                      OdRxObject* pRxDatabase = NULL,
                                      bool bWithExtSynonyms = false);
  virtual OdString getFilterByExtension(const OdString& sExt, 
                                        int index = 0,
                                        enum OdEd::GetFilePathFlags flg = OdEd::kGfpForOpen,
                                        OdStringArray* pExtSynonyms = NULL);

  OdRxRasterServicesPtr rasterServices()
  {
    ODA_ASSERT_ONCE(m_pDbModule.get());
    return OdRxRasterServices::cast(m_pDbModule);
  }

  bool setRasterServices(bool bSet = true)
  {
    ODA_ASSERT_ONCE(m_pDbModule.isNull() || !bSet);
    if (bSet)
      m_pDbModule = odrxDynamicLinker()->loadApp(RX_RASTER_SERVICES_APPNAME);
    else
      m_pDbModule = NULL;
    return !bSet == m_pDbModule.isNull();
  }

  /////// Commands ///////
  //#define ODRX_CMD_ENTRY(cmdName, name, impl) OdStaticRxObject<OdqCmd##name> m_cmd##name;
  //#include "ImgDbCommands.h"
};

// it's here to prevent inconvenient linking with OdaQtConsole
#if defined(ODA_SYS_VAR_PE_UNLINKED_RXINIT) && defined(_TOOLKIT_IN_DLL_)
ODRX_NO_CONS_DEFINE_MEMBERS(OdSysVarPE, OdRxObject);
#endif

ODRX_DEFINE_DYNAMIC_MODULE(OdImgDbModuleImpl);

OdImgDbModuleImpl::OdImgDbModuleImpl()
{
  //#define ODRX_CMD_ENTRY(cmdName, name, impl) m_cmd##name.m_pModule = this;
  //#include "ImgDbCommands.h"
}

OdImgDbModuleImpl::~OdImgDbModuleImpl()
{
}

static bool s_bUnlinkedWithOdSysVarPE = false;

void OdImgDbModuleImpl::initApp()
{
  setRasterServices();
  OdExtDbModuleBaseImpl::initApp();

  OdRxDictionary* pClassDict = ::odrxClassDictionary().get();
  s_bUnlinkedWithOdSysVarPE = (!OdSysVarPE::g_pDesc);
  if (s_bUnlinkedWithOdSysVarPE)
  {
    #if !defined(ODA_SYS_VAR_PE_UNLINKED_RXINIT)
    ODA_FAIL_ONCE();
    #endif

    OdRxClass* pOdSysVarPEClass = OdRxClass::cast(pClassDict->getAt(L"OdSysVarPE")).get();
    ODA_ASSERT_ONCE(pOdSysVarPEClass);
    OdSysVarPE::g_pDesc = pOdSysVarPEClass;
  }

  // ...::rxInit();
  // register commands
  //OdEdCommandStackPtr pCommands = odedRegCmds();
  //#define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->addCommand(&m_cmd##name);
  //#include "ImgDbCommands.h"
}

void OdImgDbModuleImpl::uninitApp()
{
  // unregister commands
  //OdEdCommandStackPtr pCommands = odedRegCmds();
  //#define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->removeCmd(&m_cmd##name);
  //#include "ImgDbCommands.h"
  // ...::rxUninit();

  if (s_bUnlinkedWithOdSysVarPE)
    OdSysVarPE::g_pDesc = NULL;

  OdExtDbModuleBaseImpl::uninitApp();
  setRasterServices(false);
}

bool OdImgDbModuleImpl::isValid() // to unload if using components are missing (false)
{
  return !rasterServices().isNull();
}

// init issues
void OdImgDbModuleImpl::setBaseHostAppServices(OdDbBaseHostAppServices* pServices)
{
  ODA_ASSERT_ONCE(!s_pServices || !pServices);

  s_pServices = OdDbHostAppServices::cast(pServices).get();
}

OdRxObjectPtr OdImgDbModuleImpl::readFile(const OdString& fileName, 
                                          Oda::FileShareMode shareMode) // = Oda::kShareDenyWrite
{
  OdRxObjectPtr pRxDb = OdExtDbModuleBaseImpl::readFile(fileName, shareMode);
  return pRxDb;
}

OdRxObjectPtr OdImgDbModuleImpl::createUninitializedDatabase()
{
  OdDbDatabasePtr pDb = appImgServices()->createDatabase();
  OdDbDictionaryPtr pLayoutDict = pDb->getLayoutDictionaryId().safeOpenObject();
  OdDbObjectPtr pLayout;
  if ((pLayout = pLayoutDict->getAt(L"Layout1", OdDb::kForWrite)).get())
    pLayout->erase();
  if ((pLayout = pLayoutDict->getAt(L"Layout2", OdDb::kForWrite)).get())
    pLayout->erase();
  return OdRxObject::cast(pDb);
}

//static void setValue(OdDbOle2FramePtr pOleFrame, 
//                     const OdGePoint3d& _orig, const OdGeVector2d& _size, double _dist)
//{
//  //_dist = dist;
//  OdRectangle3d r3d;
//
//  OdGeVector2d size = _size.normal() * _dist;
//  r3d.lowLeft .set(_orig.x,          _orig.y,          _orig.z);
//  r3d.upLeft  .set(_orig.x,          _orig.y + size.y, _orig.z);
//  r3d.upRight .set(_orig.x + size.x, _orig.y + size.y, _orig.z);
//  r3d.lowRight.set(_orig.x + size.x, _orig.y,          _orig.z);
//
//  pOleFrame->setPosition(r3d);
//}

bool OdImgDbModuleImpl::loadToUninitializedDatabase(OdRxObjectPtr pRxDb, 
                                                    const OdString& fileName, 
                                                    Oda::FileShareMode) // shareMode // = Oda::kShareDenyWrite
{
  OdDbDatabasePtr pDb = OdDbDatabase::cast(pRxDb);
  ODA_ASSERT_ONCE(pDb.get());
  if (pDb.isNull())
    return false;
  // see also command insertraster
  // or 
  // possible alternative way to create special db wrapper for image based of simple code
  //Java_com_opendesign_android_TeighaDWGJni_open of Drawing/Examples/Android/jni/com_opendesign_android_TeighaDWGJni.cpp

  OdGiRasterImagePtr pImage = rasterServices()->loadRasterImage(fileName);
  ODA_ASSERT_ONCE(pImage.get());
  if (pImage.isNull())
    return false; // Failed to load raster image from file

//#if defined(ODA_WINDOWS)
//  // workaround for problem with large images on Windows
//  ::odrxDynamicLinker()->loadModule(OdOleSsItemHandlerModuleName); 
//#endif

  OdUInt32 width = pImage->pixelWidth(),
           height = pImage->pixelHeight();
  OdUInt32 max = odmax(width, height);
  OdUInt32 nMaxLim = 2048; // it looks like this limit depends of hardware 
  // TODO 
  OdSysVarPEPtr pSysVarPE = ::odrxSysRegistry()->getAt(OD_SYS_VAR_PE);
  if (pSysVarPE.get())
  {
    OdString sOleMaxLim = pSysVarPE->valueToString(NULL, L"olemaxlim");
    int val = 0;
    if (!sOleMaxLim.isEmpty() && (val = odStrToInt(sOleMaxLim)) > 0)
      nMaxLim = (OdUInt32) val;
  }
  if (   max > nMaxLim
      || pImage->pixelFormat().isRGB() || pImage->pixelFormat().isRGBA()) // OLE supports BGR & BGRA only
  {
    if (max > nMaxLim)
    {
      OdUInt32 coef = OdUInt32(double(max) / nMaxLim + 1.0 - 1e-7);
      width = width / coef;
      height = height / coef;
    }
    OdSmartPtr<OdGiRasterImageDesc> pDesc = OdGiRasterImageDesc::createObject(pImage);
    pDesc->setPixelWidth(width);
    pDesc->setPixelHeight(height);
    if (pImage->pixelFormat().isRGB())
    {
      //pDesc->setColorDepth(24);
      pDesc->pixelFormat().setBGR(); 
    }
    else if (pImage->pixelFormat().isRGBA())
    {
      //pDesc->setColorDepth(32);
      pDesc->pixelFormat().setBGRA();
    }
    pDesc->setScanLinesAlignment(4);
    pImage = pImage->convert(true, 50., 50., 0.0, 0, false, false, false, pDesc);
    ODA_ASSERT_ONCE(width == pImage->pixelWidth() && height == pImage->pixelHeight());
  }

  OdDbOle2FramePtr pOleFrame = OdDbOle2Frame::createObject();

  pOleFrame->setDatabaseDefaults(pDb);
  OdOleItemHandler* pItem = pOleFrame->getItemHandler();
  if (!pItem->embedRaster(pImage, pDb))
    return false; // "embedRaster is unsupported...
  pOleFrame->setOutputQuality(OdDbOle2Frame::kHighGraphics);

  try
  {
    OdGeVector2d size(pImage->pixelWidth(), pImage->pixelHeight());

    pOleFrame->unhandled_setHimetricSize(OdUInt16(size.x), OdUInt16(size.y));

    OdGePoint3d orig;

    // Set initial OLE frame position to origin, and size in world units:
    // Convert MM_HIMETRIC (0.01 millimeter) to Space Units:
    double s = pDb->getMEASUREMENT()==OdDb::kEnglish ? 0.01 / 25.4 : 0.01;
    size.set(double(size.x) * s, double(size.y) * s);

    double dist = size.x;
    OdRectangle3d r3d;
    size = size.normal() * dist;
    r3d.lowLeft.set(orig.x, orig.y, orig.z);
    r3d.upLeft.set(orig.x, orig.y + size.y, orig.z);
    r3d.upRight.set(orig.x + size.x, orig.y + size.y, orig.z);
    r3d.lowRight.set(orig.x + size.x, orig.y, orig.z);
    pOleFrame->setPosition(r3d);

    OdDbBlockTableRecordPtr pSpace = pDb->getActiveLayoutBTRId().safeOpenObject(OdDb::kForWrite);
    pSpace->appendOdDbEntity(pOleFrame);

    OdDbObjectPtr pVpObj = pDb->activeViewportId().safeOpenObject(OdDb::kForWrite);
    OdAbstractViewPEPtr pVpPE(pVpObj);
    OdGeBoundBlock3d fullExtents;
    if (pVpPE->viewExtents(pVpObj, fullExtents))
      pVpPE->zoomExtents(pVpObj, &fullExtents);
  }
  catch (OdError&) // err
  {
    ODA_FAIL_ONCE();
    if (pOleFrame->isDBRO())
      pOleFrame->erase();
    return false; // Failed to insert OLE object from file...
  }

  return true;
}

//bool OdImgDbModuleImpl::argsAtSetupLayoutViews(OdRxObject* pRxDatabase, bool& bDoZoomExtents, OdGsView::RenderMode& rm)
//{
//  OdExtDbModuleBaseImpl::argsAtSetupLayoutViews(pRxDatabase, bDoZoomExtents, rm); // set for false case
//  //bDoZoomExtents = true;
//  ////rm = OdGsView::kGouraudShaded;
//  return true;
//}

bool OdImgDbModuleImpl::isResponsible(OdRxObject* pRxDatabase)
{
  ODA_ASSERT_ONCE(pRxDatabase); // test
  return false;
}

OdStringArray OdImgDbModuleImpl::getExtensions(enum OdEd::GetFilePathFlags flg, // = OdEd::kGfpForOpen
                                               OdRxObject* pRxDatabase, // = NULL
                                               bool bWithExtSynonyms) // = false
{
  OdStringArray lst;
  if (flg == OdEd::kGfpForOpen)
  {
    OdUInt32Array types = rasterServices()->getRasterImageTypes();
    for (unsigned int idx = 0, sz = types.size(); idx < sz; idx++)
    {
      OdUInt32 tp = types[idx];
      OdString sExt = rasterServices()->mapTypeToExtension(tp, NULL);
      if (!sExt.makeLower().find(L'.'))
        sExt = sExt.mid(1);
      if (sExt.isEmpty())
        continue;
      OdStringArray synonyms;
      if (getFilterByExtension(sExt, 0, OdEd::kGfpForOpen, bWithExtSynonyms ? &synonyms : NULL).isEmpty())
        continue;

      lst.push_back(sExt);
      if (bWithExtSynonyms && synonyms.size())
        lst.append(synonyms);
    }
  }
  return lst;
}

OdString OdImgDbModuleImpl::getFilterByExtension(const OdString& csExt, 
                                                 int index, // = 0
                                                 enum OdEd::GetFilePathFlags flg, // = OdEd::kGfpForOpen
                                                 OdStringArray* pExtSynonyms) // = NULL
{
  if (flg != OdEd::kGfpForOpen || index)
    return OdString::kEmpty;
  OdString sExt = csExt;
  if (sExt.makeLower().find(L'.'))
    sExt = L"." + csExt, sExt.makeLower();

  OdUInt32 tp = rasterServices()->mapExtensionToType(sExt);
  if (!rasterServices()->isRasterImageTypeSupported(tp))
    return OdString::kEmpty;

  OdString sFilter; 
  if (sExt != rasterServices()->mapTypeToExtension(tp, &sFilter) || sFilter.isEmpty())
  {
    ODA_FAIL_ONCE();
    return OdString::kEmpty;
  }

  if (pExtSynonyms)
  {
    pExtSynonyms->clear();
    int pos = 0, len = 0;
    while ((pos = sFilter.find(L"*.", pos)) > 0)
    {
      pos += 2;
      if ((len = sFilter.mid(pos).findOneOf(L" ,;)|")) < 0)
        break;
      OdString sSyn = sFilter.mid(pos, len).trimLeft().trimRight().makeLower();
      if (!sSyn.isEmpty() && sSyn != OdString(csExt).makeLower() && !pExtSynonyms->contains(sSyn))
        pExtSynonyms->push_back(sSyn);
      pos += len + 1;
    }
  }

  return sFilter;
}
