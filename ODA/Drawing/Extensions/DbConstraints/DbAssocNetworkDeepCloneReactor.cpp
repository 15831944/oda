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

#include "DbAssocNetworkDeepCloneReactor.h"
#include "DbAssocNetCloneCtx.h"
#include "DbAssocNetworkLongTransactionReactor.h"
#include "DbHostAppServices.h"
#include "ApLongTransactions.h"
#include "DbAssocNetwork.h"
#include "DbBlockTableRecord.h"
#include "DbIdMapping.h"
#include "DbDictionary.h"
#include "DbObjectOverrule.h"

#define STL_USING_ALGORITHM
#define STL_USING_LIST
#include "OdaSTL.h"
#include "DbIdBuffer.h"
#include "DbImpAssocAction.h"
#include "DbImpAssocDependency.h"
#include "DbAssocManager.h"
#include "RxOverrule.h"

ODRX_NO_CONS_DEFINE_MEMBERS(OdDbAssocNetCloneOverrule, OdDbObjectOverrule);

OdDbAssocNetworkDeepCloneReactor::OdDbAssocNetworkDeepCloneReactor()
{
  m_Counter = 0;
}
OdDbAssocNetworkDeepCloneReactor::~OdDbAssocNetworkDeepCloneReactor()
{
  m_Counter = 0;
}

void OdDbImpAssocAction::dependentObjectCloned(const OdDbAssocDependency* dep,
  const OdDbObject* object, const OdDbObject* clone)
{
  if (dep && object && object->objectId() && clone && clone->objectId()) {
    OdDbAssocNetCloneCtxPtr ctx = OdDbAssocNetCloneCtx::internal_fromDb(clone->database());
    if (ctx.get()) {
      ctx->onDependentObjectCloned(*object);
    }
  }
}

void OdDbAssocNetworkDeepCloneReactor::processEvent(OdDbAssocNetCloneCtx& ctx, EventHandler eventHandler)
{
  OdDbObjectIdLinkedArray& actions = ctx.clonedActions();
  if (actions.size()==0) // no assoc action was cloned
    return;

  OdDbObjectIdLinkedArray::const_iterator it = actions.begin();
  OdDbObjectIdLinkedArray::const_iterator end = actions.end();
  while (it!=end) {
    OdDbAssocActionPtr pSourceAction = OdDbObjectId(*it).openObject();
    if (pSourceAction.get())
      (*eventHandler)(pSourceAction, ctx);
    ++it;
  }
}

static void aaDcCancel(OdDbAssocAction* source, OdDbAssocNetCloneCtx& ctx) { source->postProcessAfterDeepCloneCancel(ctx.idMapping()); }

void OdDbAssocNetworkDeepCloneReactor::abortDeepClone(OdDbIdMapping& idMap)
{
  OdDbAssocNetCloneCtxPtr ctx = getProcessContext(idMap);
  ContextTerminator terminator = endProcessDeferred(idMap.destDb());
  if (ctx.get())
    processEvent(*ctx, &aaDcCancel);
}

static void aaDcEnd(OdDbAssocAction* source, OdDbAssocNetCloneCtx& ctx) {
  if (ctx.doApplyTransform()) {
    OdDbIdPair idPair(source->objectId());
    if (ctx.idMapping().compute(idPair) && idPair.isCloned()) {
      OdDbAssocActionPtr clone = OdDbAssocAction::cast(idPair.value().openObject(OdDb::kForWrite));
      if (clone.get())
        clone->transformActionBy(ctx.actionTransform());
    }
  }
  source->postProcessAfterDeepClone(ctx.idMapping());
}

struct UpdateDependencyStatus {
  void operator() (const OdDbObjectId& id) {
    OdDbAssocDependencyPtr clonedDependency = OdDbAssocDependency::cast(id.openObject(OdDb::kForWrite));
    if (clonedDependency.get()) {
      OdDbObjectPtr object = clonedDependency->dependentOnObject().openObject();
      clonedDependency->setStatus(object.isNull() ? kErasedAssocStatus : kChangedNoDifferenceAssocStatus);
    }
  }
};

void OdDbAssocNetworkDeepCloneReactor::endDeepClone(OdDbIdMapping& idMap)
{
  OdDbAssocNetCloneCtxPtr ctx = getProcessContext(idMap);
  if (ctx.get()) {
    ContextTerminator terminator = endProcessDeferred(idMap.destDb());

    ctx->relinkDependenciesOnObjects();
    OdDbObjectIdLinkedArray& actions = ctx->clonedActions();
    if (actions.size() == 0) // no assoc action was cloned
      return;

    OdDbObjectIdLinkedArray::const_iterator it = actions.begin();
    OdDbObjectIdLinkedArray::const_iterator end = actions.end();
    while (it != end) { // update dependency status
      OdDbAssocActionPtr pSourceAction = OdDbObjectId(*it).openObject();
      if (pSourceAction.get()) {
        OdDbIdPair idPair(*it);
        if (idMap.compute(idPair) && idPair.isCloned()) {
          OdDbAssocActionPtr clone = OdDbAssocAction::cast(idPair.value().openObject(OdDb::kForWrite));
          if (clone.get()) {
            OdDbObjectIdArray clonedDependencies; clone->getDependencies(true, true, clonedDependencies);
            std::for_each(clonedDependencies.begin(), clonedDependencies.end(), UpdateDependencyStatus());
          }
        }
      }
      ++it;
    }

    ctx->appendActionsToOwners();

    processEvent(*ctx, &aaDcEnd);
  }
}

void OdDbAssocNetworkDeepCloneReactor::beginDeepClone(OdDbDatabase* pToDb, OdDbIdMapping& idMap) {
  startProcess(idMap);
}

void OdDbAssocNetworkDeepCloneReactor::databaseToBeDestroyed(OdDbDatabase* pDb) {
  endProcessDeferred(pDb);
}

void OdDbAssocNetworkDeepCloneReactor::beginDeepCloneXlation(OdDbIdMapping& idMap) {
  OdDbAssocNetCloneCtxPtr ctx = OdDbAssocNetCloneCtx::internal_fromDb(idMap.destDb());
  if (ctx.get()) {
    ctx->addMoreObjectsToDeepClone();
  }
}

void OdDbAssocNetworkDeepCloneReactor::startProcess(OdDbIdMapping& idMap)
{
  OdDbAssocNetCloneCtx::attach(idMap);
}

OdDbAssocNetworkDeepCloneReactor::ContextTerminator
OdDbAssocNetworkDeepCloneReactor::endProcessDeferred(OdDbDatabase* pDb)
{
  return ContextTerminator(*this, pDb);
}

OdSmartPtr<OdDbAssocNetCloneCtx>
OdDbAssocNetworkDeepCloneReactor::getProcessContext(OdDbIdMapping& idMap)
{
  return OdDbAssocNetCloneCtx::internal_fromDb(idMap.destDb());
}

OdDbAssocNetworkDeepCloneReactor::ContextTerminator::ContextTerminator(OdDbAssocNetworkDeepCloneReactor& reactor,
                                                                       OdDbDatabase* pContextDb)
  : m_reactor(reactor),
    m_pContextDb(pContextDb)
{}

OdDbAssocNetworkDeepCloneReactor::ContextTerminator::~ContextTerminator()
{
  OdDbAssocNetCloneCtx::detach(m_pContextDb);
}

#ifdef DISABLE_NEW_CLONING_CODE
void OdDbAssocNetworkDeepCloneReactor::addRefReactor()
{
}

void OdDbAssocNetworkDeepCloneReactor::releaseReactor()
{
}
#else
void OdDbAssocNetworkDeepCloneReactor::addRefReactor()
{
  if (m_Counter++ == 0)
  {
    odrxEvent()->addReactor(this);
    m_Overrule.addOverruleClient();
  }
}

void OdDbAssocNetworkDeepCloneReactor::releaseReactor()
{
  if (--m_Counter == 0)
  {
    m_Overrule.removeOverruleClient();
    odrxEvent()->removeReactor(this);
  }
}
#endif
