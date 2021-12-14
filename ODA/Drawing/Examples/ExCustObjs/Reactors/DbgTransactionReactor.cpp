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

#include "OdaCommon.h"
#include "DbDatabase.h"
#include "DbgTransactionReactor.h"

void OdDbgTransactionReactor::transactionAboutToStart(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[TRANSACT_REACTOR] : Database <0x%08X> transaction #%d about to start."), pDb, pDb->numActiveTransactions()+1);
  printMessage(msg);
}

void OdDbgTransactionReactor::transactionStarted(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[TRANSACT_REACTOR] : Database <0x%08X> transaction #%d started."), pDb, pDb->numActiveTransactions());
  printMessage(msg);
}

void OdDbgTransactionReactor::transactionAboutToEnd(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[TRANSACT_REACTOR] : Database <0x%08X> transaction #%d about to end."), pDb, pDb->numActiveTransactions());
  printMessage(msg);
}

void OdDbgTransactionReactor::transactionEnded(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[TRANSACT_REACTOR] : Database <0x%08X> transaction #%d ended."), pDb, pDb->numActiveTransactions()+1);
  printMessage(msg);
}

void OdDbgTransactionReactor::transactionAboutToAbort(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[TRANSACT_REACTOR] : Database <0x%08X> transaction #%d about to abort."), pDb, pDb->numActiveTransactions());
  printMessage(msg);
}

void OdDbgTransactionReactor::transactionAborted(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[TRANSACT_REACTOR] : Database <0x%08X> transaction #%d aborted."), pDb, pDb->numActiveTransactions()+1);
  printMessage(msg);
}

void OdDbgTransactionReactor::endCalledOnOutermostTransaction(OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[TRANSACT_REACTOR] : End called on outermost transaction."));
  printMessage(msg);
}

void OdDbgTransactionReactor::objectIdSwapped(const OdDbObject* pTransObj, const OdDbObject* pOtherObj, OdDbDatabase* pDb)
{
  OdString msg;
  msg.format(OD_T("[TRANSACT_REACTOR] : Transaction-resident object %ls<%ls> is swapped with transaction-resident object %ls<%ls>."), pTransObj->isA()->name().c_str(), pTransObj->objectId().getHandle().ascii().c_str(), pOtherObj->isA()->name().c_str(), pOtherObj->objectId().getHandle().ascii().c_str());
  printMessage(msg);
}
