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
// TigDbModule.cpp
//

#include "OdaCommon.h"
#include "OdqInterfaces.h"
#include "OdqRevSystem.h"

#include "TigDbModuleImpl.h"
#include "RevisionController.h"

//////////////////////////////////////////////////////////////////////////

//appTigServicesImpl()->m_pBaseHostAppServices = pServices;

//static OdTigDbServicesImpl* appTigServicesImpl()
//{
//  static OdStaticRxObject<OdTigDbServicesImpl> g_services;
//  return &g_services;
//}

static OdDbHostAppServices* s_pServices = NULL;
OdDbHostAppServices* appTigServices()
{
  ODA_ASSERT_ONCE(s_pServices);
  return s_pServices;
}

//////////////////////////////////////////////////////////////////////////

class OdTigDbModuleImpl : public OdExtDbModuleBaseImpl
{
public:
  OdTigDbModuleImpl();
  virtual ~OdTigDbModuleImpl();

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

  inline OdStringArray getImportExportExtensions(OdRxObject* pRxDatabase,
    bool isForImport = true,
    bool bWithExtSynonyms = false);

  OdString getImportExportFilterByExtension(const OdString& sExt,
    int index = 0,
    bool isForImport = true,
    OdStringArray* pExtSynonyms = NULL);

  bool exportToFile(OdRxObject* pRxDatabase, const OdString& fileName, int filterIndex, ODCOLORREF clrBackground);

  bool exportToFile(OdEdCommandContext* pCmdCtx, const OdString& fileName, int filterIndex, ODCOLORREF clrBackground);

  /////// Commands ///////
  #define ODRX_CMD_ENTRY(cmdName, name, impl) OdStaticRxObject<OdqCmd##name> m_cmd##name;
  #include "TigDbCommands.h"
};

ODRX_DEFINE_DYNAMIC_MODULE(OdTigDbModuleImpl);

OdTigDbModuleImpl::OdTigDbModuleImpl()
{
  #define ODRX_CMD_ENTRY(cmdName, name, impl) m_cmd##name.m_pModule = this;
  #include "TigDbCommands.h"
}

OdTigDbModuleImpl::~OdTigDbModuleImpl()
{
}

static OdSharedPtr<OdqRevSystem> s_pRevSys;

void OdTigDbModuleImpl::initApp()
{
  //OdExtDbModuleBaseImpl::initApp();
  // ...::rxInit();

  // create single instance of Revision system (dispatcher of mdi windows)
  ODA_ASSERT_ONCE(s_pRevSys.isNull());
  s_pRevSys = new OdqRevSystem();
  // register
  getIApp()->registerQObject(IqRevSystem_iid
                             "|<event-refresh:database_closed>"
                             "|<event-refresh:database_changed>"
                             "|<event-refresh:revision_changed>", s_pRevSys);
  //OdRxObject* pCurDatabase = getIConsole()->getCurrentDatabase();
  //OdqRevSystem::getRevTab(pCurDatabase, true);
  //if (pCurDatabase)
  //{
  //  QList<OdRxObject*> lst = getIConsole()->getOpenDatabases(); // NULL is skipped in list
  //  lst.push_front(NULL);
  //  foreach(OdRxObject* pRxDb, lst)
  //  {
  //    if (pCurDatabase == pRxDb)
  //      continue;
  //    OdqRevSystem::getRevTab(pRxDb, false, true);
  //  }
  //}
  OdqRevSystem::getRevTab(NULL, true);

  // register commands
  OdEdCommandStackPtr pCommands = odedRegCmds();
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->addCommand(&m_cmd##name);
  #include "TigDbCommands.h"
}

void OdTigDbModuleImpl::uninitApp()
{
  // unregister commands
  OdEdCommandStackPtr pCommands = odedRegCmds();
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->removeCmd(&m_cmd##name);
  #include "TigDbCommands.h"

  // unregister single instance of Revision System and delete it
  ODA_ASSERT_ONCE(!s_pRevSys.isNull());
  s_pRevSys->saveSettings(); // TODO // s_pRevSys->uninit();
  getIApp()->unregisterQObject(IqRevSystem_iid
                               "|<event-refresh:database_closed>"
                               "|<event-refresh:database_changed>"
                               "|<event-refresh:revision_changed>" IqRevSystem_iid, s_pRevSys);
  s_pRevSys = NULL;

  // ...::rxUninit();
  //OdExtDbModuleBaseImpl::uninitApp();
}

bool OdTigDbModuleImpl::isValid() // to unload if using components are missing (false)
{
  return s_pRevSys->controller() != NULL;
}

// init issues
void OdTigDbModuleImpl::setBaseHostAppServices(OdDbBaseHostAppServices* pServices)
{
  ODA_ASSERT_ONCE(!s_pServices || !pServices);

  s_pServices = OdDbHostAppServices::cast(pServices).get();
  if (pServices && s_pRevSys->controller()->isValid())
    odTfInitialize(OdDbHostAppServices::cast(pServices));
}

OdRxObjectPtr OdTigDbModuleImpl::readFile(const OdString& fileName,
                                          Oda::FileShareMode shareMode) // = Oda::kShareDenyWrite
{
  //return OdExtDbModuleBaseImpl::readFile(fileName, shareMode);
  ODA_ASSERT_ONCE(s_pRevSys->controller());

  //if (::odrxGetSchemaManager()->getSchemaPath().isEmpty())
  //{
  //  bool usePrefixes = getIAppProps()->toInt(L"Vars/tigprefix", false);
  //  s_pRevSys->controller()->setPrefixDelimiter(usePrefixes ? L'\\' : L'\0');
  //}

  return s_pRevSys->controller()->open(appTigServices(), fileName);
}

OdRxObjectPtr OdTigDbModuleImpl::createUninitializedDatabase()
{
  OdDbDatabasePtr pDb = appTigServices()->createDatabase(true);
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

bool OdTigDbModuleImpl::loadToUninitializedDatabase(OdRxObjectPtr pRxDb,
                                                    const OdString& fileName, 
                                                    Oda::FileShareMode) // shareMode // = Oda::kShareDenyWrite
{
  ODA_ASSERT_ONCE(pRxDb.get());
  if (pRxDb.isNull())
    return false;

  ODA_ASSERT_ONCE(s_pRevSys->controller());
  bool bRes = s_pRevSys->controller()->loadDatabase(pRxDb, fileName);
  return bRes;
}

//bool OdTigDbModuleImpl::argsAtSetupLayoutViews(OdRxObject* pRxDatabase, bool& bDoZoomExtents, OdGsView::RenderMode& rm)
//{
//  OdExtDbModuleBaseImpl::argsAtSetupLayoutViews(pRxDatabase, bDoZoomExtents, rm); // set for false case
//  //bDoZoomExtents = true;
//  ////rm = OdGsView::kGouraudShaded;
//  return true;
//}

bool OdTigDbModuleImpl::isResponsible(OdRxObject* pRxDatabase)
{
  return s_pRevSys->controller() && s_pRevSys->controller()->isResponsible(pRxDatabase);
}

OdStringArray OdTigDbModuleImpl::getExtensions(enum OdEd::GetFilePathFlags flg, // = OdEd::kGfpForOpen
                                               OdRxObject* pRxDatabase, // = NULL
                                               bool bWithExtSynonyms) // = false
{
  OdStringArray lst;
  if (flg == OdEd::kGfpForOpen && (!pRxDatabase || isResponsible(pRxDatabase)))
    lst.push_back(L"teg"), lst.push_back(L"dsf"), lst.push_back(L"tig"); // lst.push_back(L"tig")
  return lst;
}

OdString OdTigDbModuleImpl::getFilterByExtension(const OdString& sExt,
                                                 int index, // = 0
                                                 enum OdEd::GetFilePathFlags flg, // = OdEd::kGfpForOpen
                                                 OdStringArray* pExtSynonyms) // = NULL
{
  if (index)
    return OdString::kEmpty;
  //if (sExt == L"teg")
  //  return "ODA SDK Format (*.teg)|*.teg|";
  if (sExt == L"tig")
    return "ODA SDK Format (*.tig)|*.tig|";
  if (sExt == L"dsf")
    return "Drawings Stream Format (*.dsf)|*.dsf|";
  return OdString::kEmpty;
}

inline OdStringArray OdTigDbModuleImpl::getImportExportExtensions(OdRxObject* pRxDatabase,
  bool isForImport,
  bool bWithExtSynonyms)
{
  if (isForImport)
    return OdStringArray();
  OdStringArray lst;
  if (pRxDatabase
    && isResponsible(pRxDatabase))
    lst.push_back(L"dwg");
  return lst;
}

OdString OdTigDbModuleImpl::getImportExportFilterByExtension(const OdString& sExt,
  int index,
  bool isForImport,
  OdStringArray* pExtSynonyms)
{
  if (isForImport == false && sExt == L"dwg")
  {
    switch (index)
    {
    case 0:
      return L"2018 DWG (*.dwg)|*.dwg|";
    case 1:
      return L"2013 DWG (*.dwg)|*.dwg|";
    case 2:
      return L"2010 DWG (*.dwg)|*.dwg|";
    case 3:
      return L"2007 DWG (*.dwg)|*.dwg|";
    case 4:
      return L"2004 DWG (*.dwg)|*.dwg|";
    case 5:
      return L"2000 DWG (*.dwg)|*.dwg|";
    }
    
  }
  return OdString::kEmpty;
}

bool OdTigDbModuleImpl::exportToFile(OdRxObject* pRxDatabase, const OdString& fileName, int filterIndex, ODCOLORREF clrBackground)
{
  if (pRxDatabase
    && isResponsible(pRxDatabase))
  {
    OdDbDatabase *pDb = OdDbDatabase::cast(pRxDatabase);
    if (pDb)
    {
      OdStreamBufPtr pFileBuf = odrxSystemServices()->createFile(fileName, Oda::kFileWrite, Oda::kShareDenyWrite, Oda::kCreateAlways);
      if (!pFileBuf.isNull())
      {
        switch (filterIndex)
        {
        case 0:
          pDb->writeFile(pFileBuf, OdDb::kDwg, OdDb::vAC32);
          break;
        case 1:
          pDb->writeFile(pFileBuf, OdDb::kDwg, OdDb::vAC27);
          break;
        case 2:
          pDb->writeFile(pFileBuf, OdDb::kDwg, OdDb::vAC24); 
          break;
        case 3:
          pDb->writeFile(pFileBuf, OdDb::kDwg, OdDb::vAC21); 
          break;
        case 4:
          pDb->writeFile(pFileBuf, OdDb::kDwg, OdDb::vAC18); 
          break;
        case 5:
          pDb->writeFile(pFileBuf, OdDb::kDwg, OdDb::vAC15); 
          break;
        }
        return true;
      }
    }
  }
  return false;
}

bool OdTigDbModuleImpl::exportToFile(OdEdCommandContext* pCmdCtx, const OdString& fileName, int filterIndex, ODCOLORREF clrBackground)
{
  OdRxObject *pRxDb = pCmdCtx->baseDatabase();
  return exportToFile(pRxDb, fileName, filterIndex, clrBackground);
}
