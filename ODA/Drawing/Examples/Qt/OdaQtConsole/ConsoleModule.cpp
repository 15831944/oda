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
// ConsoleModule.cpp
//

#include <QtCore>
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui>
#else
#include <QtWidgets>
#endif

//#include "ConsoleCommands.h"
#include "ConsoleModule.h"
#include "OdqConsole.h"
#include "OdqCommandLineEdit.h" // for TCmdMap
#include "ConsoleReactors.h"
//#include "OdqAudit.h"
#include "SysVarPEImpl.h"
#include "OdqGripManager.h"
#include "OdqSnapManager.h"
#include "OdqDragTracker.h"
#include "ExtDbModule.h"

//////////////////////////////////////////////////////////////////////////

ODRX_DEFINE_DYNAMIC_MODULE(OdqConsoleModule)

static OdSharedPtr<OdqConsole> s_pConsole;
#if defined(__APPLE__)
static OdSharedPtr<OdqFixPlatform> s_pFixPlatform;
#endif

static OdEdCommandStackReactorPtr s_pCmdReactor;
static OdRxDLinkerReactorPtr s_pDynLinkerReactor;

// called at the start
// register custom objects and commands defined in the module

void OdqConsoleModule::initApp()
{
  // initialize RTTI classes

  //CommandContextImpl::rxInit(); 
  OdErrorExecuteCommandAfterCancelContext::rxInit();
  OdqGripManager::rxInit();
  OdqSnapManager::rxInit();
  OdqDragTracker::rxInit();
  //OdqAuditInfo::rxInit();

  s_pCmdReactor = OdqCommandReactor::createObject();
  ::odedRegCmds()->addReactor(s_pCmdReactor);
  ::odedRegCmds()->addReactor(OdqCommandLastReactor::getObject());

  ::initOdSysVarPE(); // "TD_DgnDb.tx" and "TD_BimDb.tx" are loading as side effect here

  s_pDynLinkerReactor = OdqDynLinkerReactor::createObject();
  ::odrxDynamicLinker()->addReactor(s_pDynLinkerReactor);

  // create single instance of console 
  ODA_ASSERT_ONCE(s_pConsole.isNull());
  s_pConsole = new OdqConsole(
                      static_cast<OdqCommandReactor*>(s_pCmdReactor.get())->getCommandMap());
  // register
  getIApp()->registerQObject(IqConsole_iid, s_pConsole);
  // and init it
  OdqConsole::getConsoleTab(NULL); // TODO // s_pConsole->init();

 #if defined(__APPLE__)
  s_pFixPlatform = new OdqFixPlatform();
  getIApp()->registerQObject(IqFixPlatform_iid, s_pFixPlatform);
 #endif

  // register commands

  OdEdCommandStackPtr pCommands = odedRegCmds();

  //pCommands->addCommand(&m_cmdOpen);
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->addCommand(&m_cmd##name);
  #include "ConsoleCommands.h"
}

// called at the end
// unregister custom objects and commands defined in the module

void OdqConsoleModule::uninitApp()
{
  // unregister commands

  OdEdCommandStackPtr pCommands = odedRegCmds();

  //pCommands->removeCmd(&m_cmdOpen);
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->removeCmd(&m_cmd##name);
  #include "ConsoleCommands.h"

  // unregister single instance of console and delete it
  ODA_ASSERT_ONCE(!s_pConsole.isNull());
  s_pConsole->saveSettings(); // TODO // s_pConsole->uninit();
  getIApp()->unregisterQObject(IqConsole_iid);
  
  ::odrxDynamicLinker()->unloadModule(OD_T("OdaQtViewSystem.dll")); // loaded via open command

  ::uninitOdSysVarPE();
  OdStringArray lst = getIApp()->getExtDbModuleNames();
  for (unsigned int idx = 0, sz = lst.size(); idx < sz; idx++)
  {
    const OdString& sModuleName = lst[idx];
    OdRxModule* pExtDb = ::odrxDynamicLinker()->getModule(sModuleName);
    if (!pExtDb)
      continue;
    if (!::odrxDynamicLinker()->unloadModule(sModuleName))
    {
      ODA_FAIL_ONCE();
    }
  }

  s_pConsole = NULL; //s_pConsole->close(); //if QPointer<OdqConsole> s_pConsole; 

 #if defined(__APPLE__)
  getIApp()->unregisterQObject(IqFixPlatform_iid, s_pFixPlatform);
 #endif

  ::odrxDynamicLinker()->removeReactor(s_pDynLinkerReactor);
  ::odedRegCmds()->removeReactor(OdqCommandLastReactor::getObject());
  ::odedRegCmds()->removeReactor(s_pCmdReactor);
  s_pDynLinkerReactor = NULL;
  s_pCmdReactor = NULL;

  //odrxDynamicLinker()->unloadUnreferenced();

  // uninitialize RTTI classes

  //OdqAuditInfo::rxUninit();
  OdqDragTracker::rxUninit();
  OdqSnapManager::rxUninit();
  OdqGripManager::rxUninit();
  OdErrorExecuteCommandAfterCancelContext::rxUninit();
  //CommandContextImpl::rxUninit(); 
}

