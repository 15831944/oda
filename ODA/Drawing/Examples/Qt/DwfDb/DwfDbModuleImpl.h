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
// DwfDbModuleImpl.h
//

#ifndef __DWFDB_MODULE_IMPL__H__
#define __DWFDB_MODULE_IMPL__H__

#include "ExtDbModuleBaseImpl.h"
#include "DwfHostAppServices.h"
#include "ExDynamicModule.h" // #include "RxDynamicModule.h" 
#include "Ed/EdCommandStack.h"

#include "ExDbCommandContext.h"
typedef ExDbCommandContext OdExDwfCmdContext;

//////////////////////////////////////////////////////////////////////////

#define ODRX_MODULE_NAME_W L"TD_DwfDb" // critical be same for Cloud

#define ODRX_CMD_ENTRY_BASE_CLASS OdEdCommand

#define ODRX_CMD_ENTRY(cmdName, name, impl) \
  ODRX_DECLARE_COMMAND(cmdName, OdqCmd##name, ODRX_CMD_ENTRY_BASE_CLASS, impl);
#include "DwfDbCommands.h"

//////////////////////////////////////////////////////////////////////////

OdDwfHostAppServices* appDwfServices();

#endif // __DWFDB_MODULE_IMPL__H__
