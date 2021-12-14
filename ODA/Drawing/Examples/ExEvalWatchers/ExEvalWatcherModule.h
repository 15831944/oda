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
/* Main header file for the DLL                                         */
/*                                                                      */
/* Implements notification functions for the evaluation of OdDbHatch    */
/* and OdDbDimension entities                                           */
/************************************************************************/
#ifndef _ODEX_EVALWATCHERMODULE_INCLUDED_
#define _ODEX_EVALWATCHERMODULE_INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RxModule.h"
#include "StaticRxObject.h"
#include "ExHatchWatcher.h"
#include "ExDimAssocWatcher.h"
#include "Ed/EdCommandStack.h"
#include "RxEvent.h"
#include "DbTransactionReactor.h"

class OdExEvalWatcherModule : public OdRxModule
{
public:
  void init();
  void uninit();
  void open();
  void close(bool bUpdate);
protected:
  OdExEvalWatcherModule();
  void initApp();
  void uninitApp();
private:
  OdStaticRxObject<OdExHatchWatcherPE>    _hatchWatcher;
  OdStaticRxObject<OdExDimAssocWatcherPE> _dimWatcher;
  struct OdExEvalWatchersCommandReactor : OdEdCommandStackReactor
  {
    OdExEvalWatcherModule* _module;
    virtual void commandWillStart(OdEdCommand* pCmd, OdEdCommandContext* pCmdCtx) ODRX_OVERRIDE;
    virtual void commandEnded(OdEdCommand* pCmd, OdEdCommandContext* pCmdCtx) ODRX_OVERRIDE;
    virtual void commandCancelled(OdEdCommand* pCmd, OdEdCommandContext* pCmdCtx) ODRX_OVERRIDE;
    virtual void commandFailed(OdEdCommand* pCmd, OdEdCommandContext* pCmdCtx) ODRX_OVERRIDE;
  };
  OdStaticRxObject<OdExEvalWatchersCommandReactor> _commandReactor;
  struct OdExEvalWatchersEventReactor : OdRxEventReactor
  {
    OdExEvalWatcherModule* _module;
    virtual void databaseConstructed(OdDbDatabase* pDb) ODRX_OVERRIDE;
    virtual void databaseToBeDestroyed(OdDbDatabase* pDb) ODRX_OVERRIDE;
  };
  OdStaticRxObject<OdExEvalWatchersEventReactor> _eventReactor;
  struct OdExEvalWatchersTransactionReactor : OdDbTransactionReactor
  {
    OdExEvalWatcherModule* _module;
    virtual void transactionStarted(OdDbDatabase* pDb) ODRX_OVERRIDE;
    virtual void transactionEnded(OdDbDatabase* pDb) ODRX_OVERRIDE;
    virtual void transactionAborted(OdDbDatabase* pDb) ODRX_OVERRIDE;
  };
  OdStaticRxObject<OdExEvalWatchersTransactionReactor> _transactionReactor;
  friend struct OdExEvalWatchersCommandReactor;
  friend struct OdExEvalWatchersEventReactor;
  friend struct OdExEvalWatchersTransactionReactor;
};

#endif // _ODEX_EVALWATCHERMODULE_INCLUDED_
