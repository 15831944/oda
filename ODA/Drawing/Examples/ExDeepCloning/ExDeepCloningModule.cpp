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
/* ExDeepCloningModule.cpp: Defines the entry points for the TX         */
/************************************************************************/
#include "OdaCommon.h"
#include "RxSysRegistry.h"
#include "RxDynamicModule.h"
#include "ExDeepCloningModule.h"
#include "ExDcObject.h"
#include "ExDcEntity.h"

//
// Define methods for custom commands
//
#define CMD_DEF(CmdName, GroupName) \
  extern void _##CmdName##_func(OdEdCommandContext* pCmdCtx);\
  const OdString _##CmdName##_cmd::globalName() const { return OD_T(#CmdName); }\
  const OdString _##CmdName##_cmd::groupName() const { return GroupName; }\
  void _##CmdName##_cmd::execute(OdEdCommandContext* pCmdCtx) { _##CmdName##_func(pCmdCtx); }
#include "ExDcCmdDefs.h"
#undef CMD_DEF


ODRX_DEFINE_DYNAMIC_MODULE(ExDeepCloningModule);


void ExDeepCloningModule::initApp()
{
  // Lock ExCustObjs module
  m_pExCustObjsModule = ::odrxDynamicLinker()->loadModule(OdExCustObjsModuleName);

  OdEdCommandStackPtr pCommands = odedRegCmds();

#define CMD_DEF(CmdName, GroupName) \
  pCommands->addCommand(&m_##CmdName##_cmd);
#include "ExDcCmdDefs.h"
#undef CMD_DEF

  ExDcObject::rxInit();
  ExDcEntity::rxInit();
}

void ExDeepCloningModule::uninitApp()
{
  OdEdCommandStackPtr pCommands = odedRegCmds();

#define CMD_DEF(CmdName, GroupName) \
  pCommands->removeCmd(GroupName, OD_T(#CmdName));
#include "ExDcCmdDefs.h"
#undef CMD_DEF

  ExDcEntity::rxUninit();
  ExDcObject::rxUninit();

  // Unlock ExCustObjs module
  m_pExCustObjsModule = 0;
}
