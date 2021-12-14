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
#include "DbAssocManager.h"
#include "Editor.h"
#include "Ed/EdCommandStack.h"
#include "Ed/EdCommandContext.h"


class ModuleReactor : public OdEditorReactor, public OdEdCommandStackReactor {
private:
  virtual void initialDwgFileOpenComplete(OdDbDatabase* pDb) {
    OdDbAssocManager::evaluateTopLevelNetwork(pDb);
  }
  virtual void beginSave(OdDbDatabase* pDb, const OdString& intendedName) {
    OdDbAssocManager::evaluateTopLevelNetwork(pDb);
  }
  virtual void beginDxfOut(OdDbDatabase* pDb) {
    OdDbAssocManager::evaluateTopLevelNetwork(pDb);
  }
  virtual void dxfInComplete(OdDbDatabase* pDb) {
    OdDbAssocManager::evaluateTopLevelNetwork(pDb);
  }
  virtual void commandWillStart(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx) {
    OdDbDatabasePtr pDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
    if (pDb.get())
    {
      OdDbAssocManager::evaluateTopLevelNetwork(pDb);
    }
  }
  virtual void commandEnded(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx) {
    OdDbDatabasePtr pDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
    if (pDb.get())
    {
      OdDbAssocManager::evaluateTopLevelNetwork(pDb);
    }
  }
  virtual void commandCancelled(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx) {
    OdDbDatabasePtr pDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
    if (pDb.get())
    {
      OdDbAssocManager::evaluateTopLevelNetwork(pDb);
    }
  }
  virtual void commandFailed(OdEdCommand* pCommand, OdEdCommandContext* pCmdCtx) {
    OdDbDatabasePtr pDb = OdDbDatabase::cast(pCmdCtx->baseDatabase());
    if (pDb.get())
    {
      OdDbAssocManager::evaluateTopLevelNetwork(pDb);
    }
  }
};

static ModuleReactor* g_reactor = 0;

void attachEditorReactor() {
  if (g_reactor)
    return;
  g_reactor = static_cast<ModuleReactor*>(
    OdRxObjectImpl<ModuleReactor, OdEditorReactor>::createObject().detach()
    );
  ::odedRegCmds()->addReactor(g_reactor);
  ::odrxEvent()->addReactor(g_reactor);
}

void detachEditorReactor() {
  if (!g_reactor)
    return;
  ::odrxEvent()->removeReactor(g_reactor);
  ::odedRegCmds()->removeReactor(g_reactor);
  static_cast<OdEditorReactor*>(g_reactor)->release();
  g_reactor = 0;
}

bool isEditorReactorAttached() {
  return g_reactor!=0;
}
