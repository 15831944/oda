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
// PropSystemModule.cpp
//

#include "OdaCommon.h"
#include "RxDictionary.h"

#include "PropSystemModule.h"
#include "OdqPropSystem.h"
#include "ResBufPropValueConvertors.h"

#include "SysVarPE.h"
#include "PropServices.h"
#include "ExAppServices.h"
#include "DbXmlPropSources.h"

ODRX_DEFINE_DYNAMIC_MODULE(OdqPropSystemModule)

#if defined(ODA_SYS_VAR_PE_UNLINKED_RXINIT) && defined(_TOOLKIT_IN_DLL_)
ODRX_NO_CONS_DEFINE_MEMBERS(OdSysVarPE, OdRxObject);
#endif
#if defined(ODA_PROP_SERVICES_UNLINKED_RXINIT) && defined(_TOOLKIT_IN_DLL_)
ODRX_NO_CONS_DEFINE_MEMBERS(OdPropServices, OdRxObject);
#endif
#if defined(ODA_EX_APP_SERVICES_UNLINKED_RXINIT) && defined(_TOOLKIT_IN_DLL_)
ODRX_NO_CONS_DEFINE_MEMBERS(ExAppServices, OdRxObject);
#endif

static bool s_bUnlinkedWithOdSysVarPE = false;
static bool s_bUnlinkedWithPropServices = false;
static bool s_bUnlinkedWithExAppServices = false;

//-----------------------------------------------------------------------

// paper-drawing protocol extension

static OdSharedPtr<OdqPropSystem> s_pPropSys;

// called at the start
// register custom objects and commands defined in the module

void OdqPropSystemModule::initApp()
{
  // initialize RTTI classes

  //...::rxInit(); 

  OdRxDictionary* pClassDict = ::odrxClassDictionary().get();

  s_bUnlinkedWithOdSysVarPE = (!OdSysVarPE::g_pDesc);
  if (s_bUnlinkedWithOdSysVarPE)
  {
   #if !defined(ODA_SYS_VAR_PE_UNLINKED_RXINIT)
    ODA_FAIL_ONCE();
   #endif
    OdRxClass* pOdSysVarPEClass = OdRxClass::cast(pClassDict->getAt(OD_T("OdSysVarPE"))).get();
    ODA_ASSERT_ONCE(pOdSysVarPEClass);
    OdSysVarPE::g_pDesc = pOdSysVarPEClass;
  }
  s_bUnlinkedWithPropServices = (!OdPropServices::g_pDesc);
  if (s_bUnlinkedWithPropServices)
  {
   #if !defined(ODA_PROP_SERVICES_UNLINKED_RXINIT)
    ODA_FAIL_ONCE();
   #endif
    OdRxClass* pOdPropServicesClass = OdRxClass::cast(pClassDict->getAt(OD_T("OdPropServices"))).get();
    ODA_ASSERT_ONCE(pOdPropServicesClass);
    OdPropServices::g_pDesc = pOdPropServicesClass;
  }
  s_bUnlinkedWithExAppServices = (!ExAppServices::g_pDesc);
  if (s_bUnlinkedWithExAppServices)
  {
   #if !defined(ODA_EX_APP_SERVICES_UNLINKED_RXINIT)
    ODA_FAIL_ONCE();
   #endif
    OdRxClass* pExAppServicesClass = OdRxClass::cast(pClassDict->getAt(OD_T("ExAppServices"))).get();
    ODA_ASSERT_ONCE(pExAppServicesClass);
    ExAppServices::g_pDesc = pExAppServicesClass;
  }

  OdResBufPropControllerImpl::init();

  // create single instance of Property system (dispatcher of mdi windows)
  ODA_ASSERT_ONCE(s_pPropSys.isNull());
  s_pPropSys = new OdqPropSystem(); 
  OdRxObject* pCurDatabase = getIConsole()->getCurrentDatabase();
  if (pCurDatabase)
  {
    OdSelectionSet* pSelSet = getIConsole()->workingSelectionSet(pCurDatabase);
    // clear selection set if it is not empty
    if (pSelSet && pSelSet->numEntities())
      getIConsole()->clearSelectionSet(pCurDatabase);
  }
  // register
  getIApp()->registerQObject(IqPropSystem_iid
                             "|<event-refresh:database_closed>"
                             "|<event-refresh:database_changed>"
                             "|<event-refresh:selection_changed>", s_pPropSys);
  OdqPropSystem::getPropTab(pCurDatabase, true);
  if (pCurDatabase)
  {
    QList<OdRxObject*> lst = getIConsole()->getOpenDatabases(); // NULL is skipped in list
    lst.push_front(NULL); 
    foreach (OdRxObject* pRxDb, lst)
    {
      if (pCurDatabase == pRxDb)
        continue;
      OdqPropSystem::getPropTab(pRxDb, false, true);
    }
  }

  OdPropServicesPtr pPropServices = OdPropServices::cast(getIConsole()->getCommandContext());
  if (pPropServices.get())
    registerOdDbXmlPropSources(pPropServices);

  // register commands
  OdEdCommandStackPtr pCommands = odedRegCmds();

  //pCommands->addCommand(&m_cmdOpen);
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->addCommand(&m_cmd##name);
  #include "PropSystemCommands.h"
}

// called at the end
// unregister custom objects and commands defined in the module

void OdqPropSystemModule::uninitApp()
{
  // unregister commands

  OdEdCommandStackPtr pCommands = odedRegCmds();

  //pCommands->removeCmd(&m_cmdOpen);
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->removeCmd(&m_cmd##name);
  #include "PropSystemCommands.h"

  OdPropServices* pPropServices = OdPropServices::cast(getIConsole()->getCommandContext()).get();
  if (pPropServices)
  {
    unregisterOdDbXmlPropSources(pPropServices);
    //#include "PropServicesImpl.h"
    //((OdPropServicesImpl*)pPropServices)->saveCatWeights(); // autoloaded by 1st use
  }

  // unregister single instance of Property System and delete it
  ODA_ASSERT_ONCE(!s_pPropSys.isNull());
  s_pPropSys->saveSettings(); // TODO // s_pPropSys->uninit();
  getIApp()->unregisterQObject(IqPropSystem_iid
                               "|<event-refresh:database_closed>"
                               "|<event-refresh:database_changed>"
                               "|<event-refresh:selection_changed>" IqPropSystem_iid, s_pPropSys);
  s_pPropSys = NULL;

  OdResBufPropControllerImpl::uninit();

  // uninitialize RTTI classes

  //...::rxUninit(); 

  if (s_bUnlinkedWithExAppServices)
    ExAppServices::g_pDesc = NULL;
  if (s_bUnlinkedWithPropServices)
    OdPropServices::g_pDesc = NULL;
  if (s_bUnlinkedWithOdSysVarPE)
    OdSysVarPE::g_pDesc = NULL;
}
