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
// AppModule.cpp
//

#if !defined(_USE_CRYPTLIB) && defined(Q_CC_MSVC)
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0400 // to enable Windows Cryptographic API  
#endif

//#include "AppCommands.h"
#include "AppModule.h"
#include "DynamicLinker.h"

///////////////////////////////////////////////////////////////////////////////

#include <QObject> // for Q_CC_MSVC only



#if !defined(ODA_QT_NO_CAUSTIC_OPENRL)
  #include "OpenRLKey.h"
#endif

///////////////////////////////////////////////////////////////////////////////

// defined via ODRX_DEFINE_STATIC_MODULE(OdqAppModule) in OdqMainWindow.cpp

// called at the start (but after OdqConsoleModule::initApp())
// register custom objects and commands defined in the module

void OdqAppModule::initApp()
{
  // initialize RTTI classes
  //...::rxInit(); 

 #if !defined(_USE_CRYPTLIB) && defined(Q_CC_MSVC)
	::odrxDynamicLinker()->loadModule(DbCryptModuleName);
 #endif

 #if !defined(ODA_QT_NO_CAUSTIC_OPENRL)
  initOpenRLSupport();
 #endif

  // register commands

  OdEdCommandStackPtr pCommands = odedRegCmds();

  //pCommands->addCommand(&m_cmdOpen);
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->addCommand(&m_cmd##name);
  #include "AppCommands.h"
}

// called at the end (but before OdqConsoleModule::uninitApp())
// unregister custom objects and commands defined in the module

void OdqAppModule::uninitApp()
{
  // unregister commands

  OdEdCommandStackPtr pCommands = odedRegCmds();

  //pCommands->removeCmd(&m_cmdOpen);
  #define ODRX_CMD_ENTRY(cmdName, name, impl) pCommands->removeCmd(&m_cmd##name);
  #include "AppCommands.h"

 #if !defined(ODA_QT_NO_CAUSTIC_OPENRL)
  uninitOpenRLSupport();
 #endif

  // uninitialize RTTI classes
  // ...::rxUninit();
}
