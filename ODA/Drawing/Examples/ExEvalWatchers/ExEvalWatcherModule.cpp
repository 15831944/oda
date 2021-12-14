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
/* Defines entry points for the DLL                                     */
/************************************************************************/
#include "StdAfx.h"
#include "ExEvalWatcherModule.h"
#include "RxDynamicModule.h"
#include "DbHatch.h"
#include "DbDimAssoc.h"
#include "DbLeader.h"
#include "ExHatchWatcher.h"
#include "ExDimAssocWatcher.h"

ODRX_DEFINE_DYNAMIC_MODULE(OdExEvalWatcherModule);

OdExEvalWatcherModule::OdExEvalWatcherModule()
{
}

void OdExEvalWatcherModule::initApp()
{
  if (!OdDbHatch::desc())
    throw OdError(eNotInitializedYet);

  OdDbHatch::desc()->module()->addRef();

  OdDbHatch::desc()->addX(OdDbEvalWatcherPE::desc(), &_hatchWatcher);
  OdDbDimAssoc::desc()->addX(OdDbEvalWatcherPE::desc(), &_dimWatcher);

  _commandReactor._module = this;
  ::odedRegCmds()->addReactor(&_commandReactor);
  _eventReactor._module = this;
  ::odrxEvent()->addReactor(&_eventReactor);
  _transactionReactor._module = this;
}

void OdExEvalWatcherModule::uninitApp()
{
  OdDbHatch::desc()->delX(OdDbEvalWatcherPE::desc());
  OdDbDimAssoc::desc()->delX(OdDbEvalWatcherPE::desc());

  ::odedRegCmds()->removeReactor(&_commandReactor);
  ::odrxEvent()->removeReactor(&_eventReactor);

  OdDbHatch::desc()->module()->release();
}

void OdExEvalWatcherModule::open()
{
  _hatchWatcher.open();
  _dimWatcher.open();
}

void OdExEvalWatcherModule::close(bool bUpdate)
{
  _hatchWatcher.close(bUpdate);
  _dimWatcher.close(bUpdate);
}

void OdExEvalWatcherModule::OdExEvalWatchersCommandReactor::commandWillStart(OdEdCommand*, OdEdCommandContext*)
{
  _module->open();
}

void OdExEvalWatcherModule::OdExEvalWatchersCommandReactor::commandEnded(OdEdCommand*, OdEdCommandContext*)
{
  _module->close(true);
}

void OdExEvalWatcherModule::OdExEvalWatchersCommandReactor::commandCancelled(OdEdCommand*, OdEdCommandContext*)
{
  _module->close(false);
}

void OdExEvalWatcherModule::OdExEvalWatchersCommandReactor::commandFailed(OdEdCommand*, OdEdCommandContext*)
{
  _module->close(false);
}

void OdExEvalWatcherModule::OdExEvalWatchersEventReactor::databaseConstructed(OdDbDatabase* pDb)
{
  pDb->addTransactionReactor(&_module->_transactionReactor);
}

void OdExEvalWatcherModule::OdExEvalWatchersEventReactor::databaseToBeDestroyed(OdDbDatabase* pDb)
{
  pDb->removeTransactionReactor(&_module->_transactionReactor);
}

void OdExEvalWatcherModule::OdExEvalWatchersTransactionReactor::transactionStarted(OdDbDatabase* pDb)
{
  _module->open();
}

void OdExEvalWatcherModule::OdExEvalWatchersTransactionReactor::transactionEnded(OdDbDatabase* pDb)
{
  _module->close(true);
}

void OdExEvalWatcherModule::OdExEvalWatchersTransactionReactor::transactionAborted(OdDbDatabase* pDb)
{
  _module->close(false);
}
