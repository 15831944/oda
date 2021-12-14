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
#include "DbImpAssocNetwork.h"
#include "DbAssocNetwork.h"
#include "DbFiler.h"
#include "DbDictionary.h"
#include "DbImpAssocAction.h"
#include "DbAssocAction.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocNetCloneCtx.h"
#include "DbAssocVariable.h"
#include "DbHostAppServices.h"
#include "DbAudit.h"
#include "DbAssocManager.h"
#include "DebugStuff.h"
#include "SaveState.h"


OdDbImpAssocNetwork::OdDbImpAssocNetwork()
  : OdDbImpAssocAction()
  , m_maxChildActionIdx(0)
  , m_fixStatusConsistency(false)
{
  m_isBase = false;
}

OdDbObjectIdArray OdDbImpAssocNetwork::getActions() const
{
  return m_actions;
}

OdDbObjectIdArray OdDbImpAssocNetwork::getActionsToEvaluate() const {
  return m_actionsToEvaluate;
}

OdResult OdDbImpAssocNetwork::addAction(OdDbAssocNetwork* thisNetwork, const OdDbObjectId& actionId, bool alsoSetAsDatabaseOwner)
{
  OdDbObjectId networkId = thisNetwork->objectId();
  OdDbObjectPtr pObj = actionId.openObject(OdDb::kForWrite);
  if ( !pObj->isKindOf(OdDbAssocAction::desc()) )
    return eInvalidInput;

  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(pObj);
  if (   networkId== pAction->owningNetwork() 
      && (!alsoSetAsDatabaseOwner || networkId ==  pAction->ownerId())
      && m_actions.contains(actionId))
  {
    ODA_FAIL_ONCE(); // temp test marker
    // #11465 fix audit problem via cloning (select + drag)
    return eInvalidInput; // already added
  }

  pAction->setOwningNetwork(networkId, alsoSetAsDatabaseOwner);

  if(alsoSetAsDatabaseOwner)
    pAction->setOwnerId(networkId);

  m_actions.push_back(actionId);
  m_isOwnedAction.push_back(alsoSetAsDatabaseOwner);
  if( isEvaluationRequest(pAction->status()) ) {
    m_actionsToEvaluate.push_back(actionId);
    thisNetwork->setStatus(kChangedDirectlyAssocStatus);
  }

  m_maxChildActionIdx++;

  OdDbImpAssocAction *pImpAction = OdDbImpAssocAction::getImpObject(pAction);
  pImpAction->setActionIndex(m_maxChildActionIdx);

  return eOk;
}

OdResult OdDbImpAssocNetwork::removeAction(OdDbAssocNetwork* thisNetwork, const OdDbObjectId& actionId, bool alsoEraseIt)
{
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(actionId.openObject(OdDb::kForWrite));
  if (pAction.isNull())
    alsoEraseIt = false;
  else
    pAction->setOwningNetwork(OdDbObjectId::kNull, false);

  int i = m_actions.size();
  while (i-- > 0) {
    if (m_actions[i] == actionId) {
      m_actions.removeAt(i);
      m_isOwnedAction.removeAt(i);
      m_actionsToEvaluate.remove(actionId);
      thisNetwork->setStatus(kChangedDirectlyAssocStatus);
      break;
    }
  }

  if ( alsoEraseIt )
  {
    pAction->erase();
    pAction->downgradeOpen();
  }
  return eOk;
}

OdResult OdDbImpAssocNetwork::ownedActionStatusChanged(OdDbObject* pThisNetwork, OdDbAssocAction* pOwnedAction, OdDbAssocStatus previousStatus) {
  OdDbAssocStatus newStatus = pOwnedAction->status();
  ODA_ASSERT(newStatus != previousStatus);
  bool new_toEvaluate = isEvaluationRequest(newStatus);
  bool old_toEvaluate = isEvaluationRequest(previousStatus);
  if (m_status == kErasedAssocStatus) {
    return eOk;
  }
  else if (newStatus == kErasedAssocStatus) {
    setStatus(pThisNetwork, kChangedDirectlyAssocStatus, true, false);
  }
  else if (old_toEvaluate && !new_toEvaluate) {
    pThisNetwork->assertWriteEnabled();
    m_actionsToEvaluate.remove(pOwnedAction->objectId());
    if(m_actionsToEvaluate.isEmpty())
      setStatus(pThisNetwork, kIsUpToDateAssocStatus, true, false);
  }
  else if (!old_toEvaluate && new_toEvaluate) {
    pThisNetwork->assertWriteEnabled();
    m_actionsToEvaluate.append(pOwnedAction->objectId());
    setStatus(pThisNetwork, kChangedDirectlyAssocStatus, true, false);
  }
  return eOk;
}

void odaaEnableStatusConsistencyCheck(OdDbAssocNetwork* network, bool doIt) {
  static_cast<OdDbImpAssocNetwork*>(OdDbImpAssocAction::getImpObject(network))->m_fixStatusConsistency = doIt;
}

OdResult OdDbImpAssocNetwork::dwgInFields(OdDbAssocAction *pSelf, OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocAction::dwgInFields(pSelf, pFiler);
  if ( res != eOk)
    return res;

#ifdef _DEBUG
  int val = pFiler->rdInt16();
  ODA_ASSERT(val == 0);    // Version?
#else
  pFiler->rdInt16();
#endif // _DEBUG

  m_maxChildActionIdx = pFiler->rdInt32();
  OdUInt32 nRefs = pFiler->rdInt32();
  m_actions.clear();
  m_actions.reserve(nRefs);
  m_isOwnedAction.clear();
  m_isOwnedAction.reserve(nRefs);
  OdDbObjectId id;
  bool fileFiler = pFiler->filerType()==OdDbFiler::kFileFiler;
  while (nRefs--)
  {
    bool owned = pFiler->rdBool();
    if (owned)
      id = pFiler->rdHardOwnershipId();
    else
      id = pFiler->rdSoftPointerId();

    if(!fileFiler || !id.isNull()) {
      m_isOwnedAction.append(owned);
      m_actions.append(id);
    }
  }

  // Some more references
  nRefs = pFiler->rdInt32();
  m_actionsToEvaluate.clear();
  m_actionsToEvaluate.reserve(nRefs);
  while (nRefs--)
  {
    id = pFiler->rdSoftPointerId();
    if(!fileFiler || !id.isNull())
      m_actionsToEvaluate.append(id);
  }

  if (pFiler->filerType() == OdDbFiler::kFileFiler && !m_actions.empty())
    ::odaaEnableStatusConsistencyCheck(static_cast<OdDbAssocNetwork*>(pSelf));

  return eOk;
}


void OdDbImpAssocNetwork::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dwgOutFields(pFiler, objectId);
  pFiler->wrInt16(0);   // Version?
  pFiler->wrInt32(m_maxChildActionIdx);

  OdUInt32 nRefs = m_actions.size();
  pFiler->wrInt32(nRefs);
  const OdDbObjectId* pId = m_actions.asArrayPtr();
  const bool* pOwned = m_isOwnedAction.asArrayPtr();
  while (nRefs--)
  {
    pFiler->wrBool(*pOwned);
    if (*pOwned++)
    {
      pFiler->wrHardOwnershipId(*pId++);
    }
    else
    {
      pFiler->wrSoftPointerId(*pId++);
    }
  }

  //Some more references
  nRefs = m_actionsToEvaluate.size();
  pFiler->wrInt32(nRefs);
  pId = m_actionsToEvaluate.asArrayPtr();
  while (nRefs--)
  {
    pFiler->wrSoftPointerId(*pId++);
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdDbImpAssocNetwork::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocAction::dxfInFields(pFiler, objectId);
  if (res != eOk)
    return res;

  // Check that we are at the correct subclass data
  if( !pFiler->atSubclassData( OdDbAssocNetwork::desc()->name() ))
  {
    return eBadDxfSequence;
  }

  NEXT_CODE(90)
#ifdef ODA_DIAGNOSTICS
  int val = 
#endif // ODA_DIAGNOSTICS
  pFiler->rdInt32();
  ODA_ASSERT(val == 0);  // Version?

  NEXT_CODE(90)
  m_maxChildActionIdx = pFiler->rdInt32();
  NEXT_CODE(90)
  OdUInt32 nRefs = pFiler->rdInt32();
  m_actions.clear();
  m_actions.reserve(nRefs);
  m_isOwnedAction.clear();
  m_isOwnedAction.reserve(nRefs);
  OdDbObjectId id;
  bool fileFiler = pFiler->filerType() == OdDbFiler::kFileFiler;
  bool owned;
  while (nRefs--)
  {
    switch(pFiler->nextItem())
    {
    case 330:
      owned = false;
      break;
    case 360:
      owned = true;
      break;
    default:
      ODA_FAIL();
      throw OdError(eBadDxfSequence);
    }
    id = pFiler->rdObjectId();
    if (!fileFiler || !id.isNull()) {
      m_isOwnedAction.append(owned);
      m_actions.append(id);
    }
  }

  // More references
  NEXT_CODE(90)
  nRefs = pFiler->rdInt32();
  m_actionsToEvaluate.clear();
  m_actionsToEvaluate.reserve(nRefs);
  while (nRefs--)
  {
    NEXT_CODE(330)
    id = pFiler->rdObjectId();
    if (!fileFiler || !id.isNull()) {
      m_isOwnedAction.append(owned);
      m_actions.append(id);
    }
  }

  if (pFiler->filerType() == OdDbFiler::kFileFiler && !m_actions.empty())
    ::odaaEnableStatusConsistencyCheck(OdDbAssocNetworkPtr(objectId.safeOpenObject()));

  return eOk;
}

void OdDbImpAssocNetwork::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker( OdDbAssocNetwork::desc()->name() );

  pFiler->wrInt32(90, 0);   // Version ?

  pFiler->wrInt32(90, m_maxChildActionIdx);

  OdUInt32 nRefs = m_actions.size();
  pFiler->wrInt32(90, nRefs);
  const OdDbObjectId* pId = m_actions.asArrayPtr();
  const bool* pOwned = m_isOwnedAction.asArrayPtr();
  while (nRefs--)
  {
    if (*pOwned++)
    {
      pFiler->wrObjectId(360, *pId++);
    }
    else
    {
      pFiler->wrObjectId(330, *pId++);
    }
  }

  // More references
  nRefs = m_actionsToEvaluate.size();
  pFiler->wrInt32(90, nRefs);
  pId = m_actionsToEvaluate.asArrayPtr();
  while (nRefs--)
  {
    pFiler->wrObjectId(330, *pId++);
  }
}

void OdDbImpAssocNetwork::composeForLoad(OdDbObject* pObj, OdDb::SaveType format, 
                                         OdDb::DwgVersion version, OdDbAuditInfo* pAuditInfo)
{
  OdDbImpAssocAction::composeForLoad(pObj, format, version, pAuditInfo);
  OdDbObjectId id = pObj->objectId();

  if ( format == OdDb::kDwg && OdDb::kSTMode == pObj->database()->multiThreadedMode() )
    return;

  OdDbObjectIdArray ids = getActions();
  for (OdUInt32 index = 0; index < ids.size(); index++)
  {
    OdDbObjectPtr pChild = ids[index].openObject();
    if (pChild.isNull())
      continue;
    OdDbObjectId idOwner = pChild->ownerId();
    ODA_ASSERT_VAR(OdDbObjectId idChild = pChild->objectId();)
    if (idOwner != id)
      continue;

    pChild->composeForLoad(format, version, pAuditInfo);
  }
}

inline bool OdDbAssocManager_isPostponingEvaluationRequests(OdDbDatabase* db);

OdResult OdDbImpAssocNetwork::setStatus(OdDbObject* pThisNetwork, 
                                        OdDbAssocStatus newStatus,  
                                        bool notifyOwningNetwork, 
                                        bool setInOwnedActions)
{
  OdDbImpAssocAction::setStatus(pThisNetwork, newStatus,  notifyOwningNetwork, setInOwnedActions);
  if ( setInOwnedActions && !OdDbAssocManager_isPostponingEvaluationRequests(pThisNetwork->database()) )
  {
    OdDbObjectIdArray attachedActions = getActions();

    for (unsigned int i = 0; i < attachedActions.size(); i++) {
      OdDbAssocActionPtr curAction = OdDbAssocAction::cast(attachedActions[i].openObject(OdDb::kForWrite));
      if (!curAction.isNull())
        curAction->setStatus(newStatus, false, setInOwnedActions);
    }
  }

  return eOk;
}

class AssocNetEvalGraphNode : public OdRxObject {
  OdUInt32 flags;
  OdDbObjectId id;
  OdDbAssocStatus saved;
  mutable OdDbAssocEvaluationPriority evalPriority;
public:
  static int compare(const void* v1, const void* v2) { // dna: should it take into account write dependency order here?
    return (*(AssocNetEvalGraphNode**)v1)->priority() - (*(AssocNetEvalGraphNode**)v2)->priority();
  }
# ifndef NDEBUG
  OdUInt64 dbgHandle;
# endif
  enum {
    kIsNew = 1,
    kIsAction = 2,
    kIsProcessed = 4,
  };
  AssocNetEvalGraphNode() { flags = kIsNew; evalPriority = kCannotDermineAssocEvaluationPriority; }
  ~AssocNetEvalGraphNode() {
  }
  OdDbObjectId objectId() const { return id; }
  void setObjectId(OdDbObjectId id)  { this->id = id; }
  bool isNew() const { return GETBIT(flags, kIsNew); }
  bool isAction() const { return GETBIT(flags, kIsAction); }
  void setIsNew(bool bNew) { SETBIT(flags, kIsNew, bNew); }
  void setIsAction(bool bAction) { SETBIT(flags, kIsAction, bAction); }

  bool isProcessed() const { return GETBIT(flags, kIsProcessed); }
  void setIsProcessed() { SETBIT(flags, kIsProcessed, true); }

  OdDbAssocEvaluationPriority priority() const {
    if (evalPriority == kCannotDermineAssocEvaluationPriority) {
      OdDbAssocActionPtr action = id.safeOpenObject(OdDb::kForWrite);
      evalPriority = action->evaluationPriority();
    }
    return evalPriority;
  }

  void suppressAction() {
    OdDbAssocActionPtr action = id.safeOpenObject(OdDb::kForWrite);
    saved = action->status();
    action->setStatus(kSuppressedAssocStatus, false);
  }

  void unsuppressAction() {
    OdDbAssocActionPtr action = id.safeOpenObject(OdDb::kForWrite);
    action->setStatus(kIsUpToDateAssocStatus, false);
    action->setStatus(saved, false);
  }

  void resetPriority()  {
    evalPriority = kCannotDermineAssocEvaluationPriority;
  }
};


void clearIfErased(OdDbAssocAction* action)
{
  OdDbAssocNetworkPtr net = OdDbAssocNetwork::cast(action->owningNetwork().openObject(OdDb::kForWrite));
  if (net.get())
    net->removeAction(action->objectId(), true);
  else
    action->erase(true);
}

#ifndef NDEBUG
void DBG_DUMP_OBJID(const OdDbObjectId& id) {
  OdDbObjectPtr object = id.openObject(OdDb::kForRead, true);
  OdDbAssocActionPtr action = OdDbAssocAction::cast(object);
  OdString className;
  if (action.get()) {
    OdDbObjectPtr body = action->actionBody().openObject(OdDb::kForRead, true);
    if (body.get())
      className.format(L"%ls:[%ls]", object->isA()->name().c_str(), body->isA()->name().c_str());
  }
  if (object.get() && className.isEmpty())
    className = object->isA()->name();
  ODA_TRACE3("%" PRIX64W L":%ls%ls", (OdUInt64)id.getHandle(), className.c_str(), (id.isErased() ? L" (erased)" : L""));
}
void DBG_DUMP_STATUS(OdDbAssocStatus status) {
  switch(status) {
  case kIsUpToDateAssocStatus          : ODA_TRACE0(" UpToDate");     break;
  case kChangedDirectlyAssocStatus     : ODA_TRACE0(" Directly");     break;
  case kChangedTransitivelyAssocStatus : ODA_TRACE0(" Transitively"); break;
  case kChangedNoDifferenceAssocStatus : ODA_TRACE0(" NoDifference"); break;
  case kFailedToEvaluateAssocStatus    : ODA_TRACE0(" Failed");       break;
  case kErasedAssocStatus              : ODA_TRACE0(" Erased");       break;
  case kSuppressedAssocStatus          : ODA_TRACE0(" Suppressed");   break;
  case kUnresolvedAssocStatus          : ODA_TRACE0(" Unresolved");   break;
  }
}
void DBG_DUMP_NODE(AssocNetEvalGraphNode* node) {
  OdDbAssocEvaluationPriority priority = node->priority();
  ODA_TRACE1("%+10d :  ", priority); DBG_DUMP_OBJID(node->objectId());
  OdDbAssocActionPtr action = node->objectId().safeOpenObject();
  DBG_DUMP_STATUS(action->status()); ODA_TRACE0("\n");
}
#else
#define DBG_DUMP_OBJID(id)
#define DBG_DUMP_STATUS(status)
#define DBG_DUMP_NODE(node)
#endif

class AssocNetEvalGraph;
typedef AssocNetEvalGraph RequestedToEvaluate;

class AssocNetEvalGraph : public OdDbActionsToEvaluateCallback, public OdStaticRxObject<OdDbObjectReactor> {
  typedef AssocNetEvalGraphNode EvalGrapNode;
  typedef OdSmartPtr<EvalGrapNode> EvalGrapNodePtr;
  typedef OdHashMap<OdDbStub*, EvalGrapNodePtr, OdHashSet_PtrHasher<OdDbStub> > Id2NodeHash;
  Id2NodeHash ids;
  typedef OdArray<EvalGrapNodePtr> EvalGrapNodePtrArray;
  EvalGrapNodePtrArray actionsToEvaluate;

  EvalGrapNode* lookupNode(const OdDbObjectId& objectId) {
    Id2NodeHash::iterator it = ids.find(objectId);
    if(it!=ids.end())
      return it->second.get();
    return 0;
  }
  EvalGrapNode* newNode(const OdDbObjectId& objectId) {
    OdSmartPtr<EvalGrapNode> node = OdRxObjectImpl<EvalGrapNode>::createObject();
    ids.insert(std::pair<OdDbStub*, EvalGrapNode*>(objectId, node));
    node->setObjectId(objectId);
#   ifndef NDEBUG
    node->dbgHandle = objectId.getHandle();
#   endif
    return node;
  }
  EvalGrapNode* mapNode(const OdDbObjectId& objectId) {
    EvalGrapNode* node = lookupNode(objectId);
    if (!node)
      node = newNode(objectId);
    return node;
  }
  OdDbDatabase* db;
  bool isEvaluationInProgress;
public:
  AssocNetEvalGraph(OdDbDatabase* db) {
    isEvaluationInProgress = false;
    addToDb(this->db = db);
  }
  ~AssocNetEvalGraph() {
    removeFromDb(db);
    applyNewStates(); // keeps states consistent in case of exception
  }

  virtual void needsToEvaluate(const OdDbObjectId &objectId, OdDbAssocStatus newStatus, bool ownedActionsAlso) {
    OdDbObjectPtr object = objectId.openObject(OdDb::kForWrite);
    if (object.get()) {
      if (object->isKindOf(OdDbAssocAction::desc())) {
        actionNeedsToEvaluate(static_cast<OdDbAssocAction*>(object.get()), newStatus);
        if (object->isKindOf(OdDbAssocNetwork::desc()) && ownedActionsAlso) {
          OdDbObjectIdArray actions = static_cast<OdDbAssocNetwork*>(object.get())->getActions();
          for (OdDbObjectIdArray::const_iterator it = actions.begin(), end = actions.end(); it != end; ++it)
            needsToEvaluate(*it, newStatus, ownedActionsAlso);
        }
      }
      else if (object->isKindOf(OdDbAssocDependency::desc()))
        dependencyNeedsToEvaluate(static_cast<OdDbAssocDependency*>(object.get()), newStatus);
      else
        objectNeedsToEvaluate(object.get(), newStatus);
    }
  }

  virtual void dependencyNeedsToEvaluate(OdDbAssocDependency* dep, OdDbAssocStatus newStatus) {
    if (!dep)
      return;

    ODA_TRACE0("dependency :  "); DBG_DUMP_OBJID(dep->objectId()); DBG_DUMP_STATUS(newStatus); ODA_TRACE0("\n");
    propagateWriteDependencyChange(dep);
    propagateReadDependencyChange(dep);
  }

  void propagateWriteDependencyChange(OdDbAssocDependency* dep) {
    if (dep->isWriteDependency()) {
      OdDbObjectId dependentOnObjectId = dep->dependentOnObject();
      OdDbObjectPtr obj = dependentOnObjectId.openObject(OdDb::kForWrite);
      if(obj.get() && mapNode(dependentOnObjectId)->isNew())
        objectNeedsToEvaluate(obj, kChangedTransitivelyAssocStatus);
    }
  }

  virtual void actionNeedsToEvaluate(OdDbAssocAction* action, OdDbAssocStatus newStatus) {
    if (!action)
      return;

    if (action->status() == kErasedAssocStatus) {
      ODA_TRACE0("remove erased action :  "); DBG_DUMP_OBJID(action->objectId()); DBG_DUMP_STATUS(newStatus); ODA_TRACE0("\n");
      removeErasedAction(action);
    }
    else if (isEvaluationRequest(newStatus)) {
      action->setStatus(newStatus);
      EvalGrapNode* node = mapNode(action->objectId());
      if (node->isNew()) {
        ODA_TRACE0("action     :  "); DBG_DUMP_OBJID(action->objectId()); DBG_DUMP_STATUS(newStatus); ODA_TRACE0("\n");
        node->setIsAction(true);
        node->setIsNew(false);
        action->getDependentActionsToEvaluate(this);
        actionsToEvaluate.append(node);
        propagateDependenciesOnObject(action);
      }
    }
  }

  virtual void objectNeedsToEvaluate(const OdDbObject* object, OdDbAssocStatus newStatus) {
    if(!object)
      return;

    if (object->isA()->isDerivedFrom(OdDbAssocAction::desc())) {
      actionNeedsToEvaluate((OdDbAssocAction*)object, newStatus);
    }
    else if (isEvaluationRequest(newStatus)) {
      EvalGrapNode* node = mapNode(object->objectId());
      if (node->isNew()) {
        ODA_TRACE0("object     :  "); DBG_DUMP_OBJID(object->objectId()); DBG_DUMP_STATUS(newStatus); ODA_TRACE0("\n");
        node->setIsNew(false);
        propagateDependenciesOnObject(object);
      }
    }
  }

  void propagateDependenciesOnObject(const OdDbObject* object) {
    OdDbAssocDependencyPtr dep = OdDbAssocDependency::getFirstDependencyOnObject(object).openObject(OdDb::kForWrite);
    while (dep.get()) {
      propagateReadDependencyChange(dep);
      dep = dep->nextDependencyOnObject().openObject(OdDb::kForWrite);
    }
  }

  void propagateReadDependencyChange(OdDbAssocDependency* dep)   {
    if (dep->isReadDependency()) {
      OdDbObjectId actionId = dep->owningAction();
      OdDbAssocActionPtr action = actionId.openObject(OdDb::kForWrite);
      if (action.get() && mapNode(actionId)->isNew()) {
        dep->setStatus(kChangedTransitivelyAssocStatus);
        actionNeedsToEvaluate(action, kChangedTransitivelyAssocStatus);
      }
    }
  }

  OdDbAssocNetwork* topAssocNetwork;
  EvalGrapNodePtrArray notEvaluated;

  void evaluateActions(OdDbAssocNetwork* topAssocNetwork) {
    if (actionsToEvaluate.empty())
      return;
    this->topAssocNetwork = topAssocNetwork;
    ODA_TRACE0("************ Evaluating network of actions ************\n");
    isEvaluationInProgress = true;

    ODA_TRACE0("* Sorting actions to evaluate... \n");
    sortNodesByPriority();
    int evaluated = 0;
    do {
      EvalGrapNode* node = actionsToEvaluate.last(); // highest priority node
      if (node->priority() > 0 || actionsToEvaluate.size()==1) {
        ODA_TRACE0("* Evaluating...\n");
        DBG_DUMP_NODE(node);
        evaluateActionNode(node);
        ++evaluated;
        actionsToEvaluate.removeLast();
      }
      else if (evaluated) {
        evaluated = 0;
        ODA_TRACE0("* Updating priorities for:\n");
#ifndef NDEBUG
        for (int dd = actionsToEvaluate.size() - 1; dd >= 0; --dd) {
          DBG_DUMP_NODE(actionsToEvaluate[dd]);
        }
#endif
        ODA_TRACE0("* Resorting actions to evaluate...\n");
        resortNodesByPriority();
      }
      else {
        ODA_TRACE0("* Breaking deadlock...\n");

        node = actionsToEvaluate.first(); // least priority node
        OdDbAssocActionPtr action = node->objectId().safeOpenObject(OdDb::kForWrite);
        OdDbAssocManager::requestToEvaluate(action->objectId(), kChangedDirectlyAssocStatus, false);
        action->setStatus(kIsUpToDateAssocStatus);
        actionsToEvaluate.removeFirst();
        resortNodesByPriority();
      }
    }
    while (!actionsToEvaluate.empty());
    isEvaluationInProgress = false;
  }

  void unsupressNotEvaluatedActions() {
    if (notEvaluated.size()) {
      ODA_TRACE0("*************** Not evaluated actions *****************\n");
      do {
        notEvaluated.last()->unsuppressAction();
        DBG_DUMP_NODE(notEvaluated.last());
        notEvaluated.removeLast();
      }
      while (notEvaluated.size());
    }
    ODA_TRACE0("******************* Evaluation done *******************\n\n");
  }

  void sortNodesByPriority() {
    ::qsort(actionsToEvaluate.asArrayPtr(), actionsToEvaluate.size(),
      sizeof(EvalGrapNode*), &EvalGrapNode::compare);
  }

  static void removeErasedAction(OdDbAssocAction* action) {
    action->erase(true);
  }

  void evaluateActionNode(EvalGrapNode* actionNode) {
    OdDbObjectId id = actionNode->objectId();
    OdDbAssocActionPtr action = id.openObject(OdDb::kForWrite);
    if (action.get()) {
      if (action->status() == kErasedAssocStatus)
        removeErasedAction(action);
      else if(isEvaluationRequest(action->status())) {
        action->evaluate(0);
        OdDbAssocStatus status = action->status();
        if (status == kErasedAssocStatus)
          removeErasedAction(action);
        else if (isEvaluationRequest(status)) {
          actionNode->suppressAction();
          notEvaluated.append(actionNode);
          ODA_TRACE0("           :  was not evaluated\n");
        }
      }
    }
  }

  static void reset_priority(EvalGrapNode* actionNode) {
    actionNode->resetPriority();
  }

  void resortNodesByPriority() {
    std::for_each(actionsToEvaluate.begin(), actionsToEvaluate.end(), &reset_priority);
    sortNodesByPriority();
  }

  void assembly(OdDbAssocNetwork* topAssocNetwork) {
    ODA_TRACE0("****** Assembling Assoc.Network Evaluation Graph ******\n");
    ODA_TRACE(db->numActiveTransactions()>0 ? L"* Database transaction is in progress; will flush all changes\n" : L"");
    topAssocNetwork->getDependentActionsToEvaluate(this);
  }

  typedef OdHashMap<OdDbStub*, std::pair<OdDbAssocStatus, bool>, OdHashSet_PtrHasher<OdDbStub> > Id2Request;
private:
  Id2Request buf;
  typedef OdHashSet<OdDbStub*, OdHashSet_PtrHasher<OdDbStub> > IdHash;
  IdHash allActionsEvaluated;

  static void processRequest(const Id2Request::value_type& req) {
    ODA_TRACE0("           :  "); DBG_DUMP_OBJID(req.first); DBG_DUMP_STATUS(req.second.first); ODA_TRACE0("\n");
    doRequest(req.first, req.second.first, req.second.second);
  }

  template <class T>
  static void setStatus(OdDbAssocStatus newStatus, T *object) {
    if (newStatus == kErasedAssocStatus) {
      object->setStatus(kChangedDirectlyAssocStatus, true);
      object->setStatus(newStatus, false);
    }
    else
      object->setStatus(newStatus);
  }
  static void doRequest(const OdDbObjectId &objectId, OdDbAssocStatus newStatus, bool ownedActionsAlso) {
    OdDbObjectPtr object = objectId.openObject(OdDb::kForWrite);
    if (object.get()) {
      if (object->isKindOf(OdDbAssocAction::desc())) {
        setStatus(newStatus,  static_cast<OdDbAssocAction*>(object.get()));
        if (object->isKindOf(OdDbAssocNetwork::desc()) && ownedActionsAlso) {
          OdDbObjectIdArray actions = static_cast<OdDbAssocNetwork*>(object.get())->getActions();
          for (OdDbObjectIdArray::const_iterator it = actions.begin(), end = actions.end(); it != end; ++it)
            doRequest(*it, newStatus, ownedActionsAlso);
        }
      }
      else if (object->isKindOf(OdDbAssocDependency::desc()))
        static_cast<OdDbAssocDependency*>(object.get())->setStatus(newStatus);
      else {
        OdDbAssocDependencyPtr dep = static_cast<OdDbAssocDependency*>(object.get());
        while (dep.get()) {
          doRequest(dep->objectId(), newStatus, ownedActionsAlso);
          dep = dep->nextDependencyOnObject().openObject(OdDb::kForWrite);
        }
      }
    }
  }

  static RequestedToEvaluate* getFromDb(OdDbDatabase* db) {
    return static_cast<RequestedToEvaluate*>(
      OdDbObjectReactor::findReactor(db, RequestedToEvaluate::desc()));
  }
  void addToDb(OdDbDatabase* db) {
    db->OdDbObject::addReactor(this);
  }
  static void removeFromDb(OdDbDatabase* db) {
    RequestedToEvaluate* r = getFromDb(db);
    if (r)
      db->OdDbObject::removeReactor(r);
  }

public:
  ODRX_DECLARE_MEMBERS(RequestedToEvaluate);

  static OdResult requestToEvaluate(const OdDbObjectId& objectId, OdDbAssocStatus newStatus, bool ownedActionsAlso) {
    if(!isEvaluationRequest(newStatus) && newStatus!=kErasedAssocStatus)
      return eInvalidInput;
    OdDbDatabase* db = objectId.database();
    if (!db)
      return eNoDatabase;
    RequestedToEvaluate* rte = getFromDb(db);
    if (rte) {
      if (!rte->saveRequestToEvaluate(objectId, newStatus, ownedActionsAlso))
        rte->needsToEvaluate(objectId, newStatus, ownedActionsAlso);
      return eOk;
    }
    else {
      doRequest(objectId, newStatus, ownedActionsAlso);
    }
    return eOk;
  }
  bool checkInfiniteLoop() {
    size_t oldsize = allActionsEvaluated.size();
    for (Id2Request::const_iterator it = buf.begin(), end = buf.end(); it != end; ++it)
      allActionsEvaluated.insert(it->first);
    return oldsize == allActionsEvaluated.size(); // no new actions involved, so that's it
  }
  bool applyPostponedRequests() {
    if (buf.size()) {
      actionsToEvaluate.clear();
      ids.clear();
      bool infiniteLoop = checkInfiniteLoop();
      if(!infiniteLoop) {
        ODA_TRACE0("************* Applying Postponed Requests *************\n");
      }
      else {
        ODA_ASSERT_ONCE(!("Prevented infinite loop in assoc.network evaluation. See debug output for detailes."));
        ODA_TRACE0("* Prevented infinite loop: the following postponed requests were ignored:\n");
      }
      applyNewStates();
      return !infiniteLoop;
    }
    return false;
  }
  void applyNewStates() {
    std::for_each(buf.begin(), buf.end(), &processRequest);
    buf.clear();
  }
  Id2Request postponedRequests() const { 
    return buf;
  }
  static bool saveRequestToEvaluate(const OdDbObjectId& objectId, OdDbAssocStatus newStatus, bool ownedActionsAlso) {
    OdDbDatabase* db = objectId.database();
    if (db) {
      RequestedToEvaluate* rte = AssocNetEvalGraph::getFromDb(db);
      if (rte && rte->isEvaluationInProgress) {
        ODA_TRACE0("postponed  :  "); DBG_DUMP_OBJID(objectId); DBG_DUMP_STATUS(newStatus); ODA_TRACE(ownedActionsAlso ? L" ownedActionsAlso" : L""); ODA_TRACE0("\n");
        rte->buf.insert(std::pair<OdDbStub*, std::pair<OdDbAssocStatus, bool> >(
          objectId, std::pair<OdDbAssocStatus, bool>(newStatus, ownedActionsAlso)
          )
        );
        return true;
      }
    }
    return false;
  }

  inline static bool isPostponingEvaluationRequest(OdDbDatabase* db) {
    if (db) {
      RequestedToEvaluate* rte = AssocNetEvalGraph::getFromDb(db);
      if (rte && rte->isEvaluationInProgress)
        return rte->isEvaluationInProgress;
    }
    return false;
  }
};

void RequestedToEvaluate_rxInit() {
  RequestedToEvaluate::rxInit();
}

void RequestedToEvaluate_rxUninit() {
  RequestedToEvaluate::rxUninit();
}

bool OdDbAssocManager_saveRequestToEvaluate(const OdDbObjectId& objectId, OdDbAssocStatus newStatus, bool ownedActionsAlso) {
  return AssocNetEvalGraph::saveRequestToEvaluate(objectId, newStatus, ownedActionsAlso);
}

inline bool OdDbAssocManager_isPostponingEvaluationRequests(OdDbDatabase* db) {
  return AssocNetEvalGraph::isPostponingEvaluationRequest(db);
}

OdResult OdDbAssocManager::requestToEvaluate(const OdDbObjectId& objectId, OdDbAssocStatus newStatus, bool ownedActionsAlso) {
  return RequestedToEvaluate::requestToEvaluate(objectId, newStatus, ownedActionsAlso);
}

ODRX_NO_CONS_DEFINE_MEMBERS(RequestedToEvaluate, OdDbObjectReactor);

void OdDbImpAssocNetwork::evaluate(OdDbAssocAction *pThisNetwork, OdDbAssocEvaluationCallback* pEvaluationCallback)
{
  OdDbAssocNetworkPtr owningNet = OdDbAssocNetwork::cast(owningNetwork().openObject());
  if (owningNet.isNull() || !owningNet->isActionEvaluationInProgress()) { // top level network
    AssocNetEvalGraph evalGraph(pThisNetwork->database());
    do {
      evalGraph.assembly(static_cast<OdDbAssocNetwork*>(pThisNetwork));
      evalGraph.evaluateActions(static_cast<OdDbAssocNetwork*>(pThisNetwork));
    }
    while (evalGraph.applyPostponedRequests());
    // some actions may stay not evaluated due to evaluating code (library) absence
    // so keep them marked 'changed' to be evaluated once after evaluating code is available
    evalGraph.unsupressNotEvaluatedActions(); 
  }
  if (m_actionsToEvaluate.empty())
    setStatus(pThisNetwork, m_actions.empty() ? kErasedAssocStatus : kIsUpToDateAssocStatus, true, false);
}

unsigned int removeAll(const OdDbObjectId& id, OdDbObjectIdArray &from, unsigned int start = 0) {
  unsigned int found, removed = 0;
  while (start < from.size() && from.find(id, found, start)) {
    from.removeAt(found);
    start = found;
    ++removed;
  }
  return removed;
}

inline unsigned int removeNulls(OdDbObjectIdArray &ids) {
  return removeAll(OdDbObjectId::kNull, ids);
}

void OdDbImpAssocNetwork::audit(OdDbObject* pObj, OdDbAuditInfo* pAuditInfo)
{
  OdDbImpAssocAction::audit(pObj, pAuditInfo);

  auditEvaluationQueue(pObj, pAuditInfo);

  OdDb::OpenMode mode = pAuditInfo->fixErrors() ? OdDb::kForWrite : OdDb::kForRead;
  int i = m_actions.size();
  while (i-- > 0)
  {
    OdDbObjectId id = m_actions[i];
    OdDbObjectPtr pObj = id.openObject(mode);
    bool bRemove;
    if (pObj.isNull())
    {
      bRemove = true;
    }
    else
    {
      pObj->audit(pAuditInfo);
      // For permanently erased object with Null objectId pObj->isErased() returns false
      bRemove = pObj->objectId().isErased();
    }
    if (bRemove)
    {
      m_actions.removeAt(i);
      m_isOwnedAction.removeAt(i);
      m_actionsToEvaluate.remove(id);
    }
  }
}

static OdDbObjectId getOrCreateExtensionDict(OdDbObject& obj)
{
  OdDbObjectId extDictId = obj.extensionDictionary();
  if (extDictId.isNull())
  {
    obj.createExtensionDictionary();
    extDictId = obj.extensionDictionary();
  }
  return extDictId;
}

static OdDbObjectId getOrCreateAssocNetworkDict(OdDbDictionary& extDict)
{
  const OdString ACAD_ASSOCNETWORK = OD_T("ACAD_ASSOCNETWORK");
  OdDbObjectId netDictId = extDict.getAt(ACAD_ASSOCNETWORK);
  if (netDictId.isNull())
  {
    OdDbDictionaryPtr pNetDict = OdDbDictionary::createObject();
    netDictId = extDict.setAt(ACAD_ASSOCNETWORK, pNetDict);
  }
  return netDictId;
}

void OdDbImpAssocNetwork::appendToOwner(OdDbAssocAction* pThisAction, OdDbIdPair& idPair, OdDbObject* owner, OdDbIdMapping& idMap)
{
  ODA_ASSERT(!idPair.isOwnerXlated());
  if (owner && owner->isA()->isDerivedFrom(OdDbBlockTableRecord::desc()))
  {
    const OdDbObjectId extDictId = getOrCreateExtensionDict(*owner);
    OdDbDictionaryPtr pExtDict = OdDbDictionary::cast(extDictId.openObject(OdDb::kForWrite));
    if (pExtDict.get())
    {
      const OdDbObjectId netDictId = getOrCreateAssocNetworkDict(*pExtDict);
      OdDbDictionaryPtr pNetDict = OdDbDictionary::cast(netDictId.openObject(OdDb::kForWrite));
      if (pNetDict.get())
      {
        const OdString ACAD_ASSOCNETWORK = OD_T("ACAD_ASSOCNETWORK");
        OdDbObjectId existingNetId = pNetDict->getAt(ACAD_ASSOCNETWORK);
        if (!existingNetId.isNull())
        {
          ODA_ASSERT(false && "Cannot append assoc network to a dictionary already containing one");
          return;
        }
        pNetDict->setAt(ACAD_ASSOCNETWORK, pThisAction);
        idPair.setOwnerXlated(true);
        idMap.assign(idPair);

        OdDbObjectPtr original = idPair.key().openObject();
        if (original.get()) {
          OdDbIdPair ownerMapping(original->ownerId());
          if (idMap.compute(ownerMapping) && !ownerMapping.isCloned()) {
            ownerMapping.setValue(netDictId);
            idMap.assign(ownerMapping);
          }
        }
      }
    }
  } else {
    pThisAction->OdDbObject::appendToOwner(idPair, owner, idMap);
  }
}

OdResult OdDbImpAssocNetwork::postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap) {
  OdDbAssocActionPtr clonedAction = oddbTranslate(pAction->objectId(),idMap).openObject(OdDb::kForWrite);
  if(clonedAction.get()) {
    if(OdDbBlockTableRecord::cast(objectThatOwnsNetworkInstance(clonedAction).openObject()).isNull()) { // orphaned block record
      clonedAction->setStatus(kErasedAssocStatus);
      OdDbAssocNetworkPtr net = clonedAction->owningNetwork().openObject(OdDb::kForWrite);
      if(net.get())
        net->removeAction(clonedAction->objectId(), true);
    }
  }
  return eOk;
}

unsigned int removeDuplicates(OdDbObjectIdArray &ids) {
  unsigned int removed = 0;
  for (unsigned int i = 0; i < ids.size(); ++i)
    removed += removeAll(ids[i], ids, i + 1);
  return removed;
}

void reportAuditError(OdDbAuditInfo* ctx, OdDbObject* obj,
  OdUInt32 sidValueName, OdUInt32 sidValidation, OdUInt32 sidRepair, OdUInt32 errFound = 1, OdUInt32 errFixed = 0);

void OdDbImpAssocNetwork::auditEvaluationQueue(OdDbObject* obj, OdDbAuditInfo* ctx) {
  if(m_actionsToEvaluate.isEmpty())
    return;
  bool fix = ctx->fixErrors();
  unsigned int errors = 0;
  if (fix) {
    errors = removeDuplicates(m_actionsToEvaluate)
      + removeNulls(m_actionsToEvaluate);
  }
  else {
    for (unsigned int i = 0, found; i < m_actionsToEvaluate.size()-1; ++i) {
      if (m_actionsToEvaluate.find(m_actionsToEvaluate[i], found, i + 1)) {
        ++errors;
        break;
      }
    }
    if (m_actionsToEvaluate.contains(OdDbObjectId::kNull))
      ++errors;
  }
  if(!m_actionsToEvaluate.empty()) {
    OdDbAssocActionPtr net = obj;
    if (!isEvaluationRequest(net->status())) {
      ++errors;
      if (fix)
        net->setStatus(kChangedDirectlyAssocStatus);
    }
  }
  if (errors)
    reportAuditError(ctx, obj, sidEvaluationQueue, sidVarValidInvalid, sidVarDefRepair, 1, fix ? 1 : 0);
}

void OdDbImpAssocNetwork::fixStatusConsistency(OdDbAssocAction* self) {
  if (m_fixStatusConsistency) {
    bool hasDirtyActions = false;
    for (unsigned int i = 0; i < m_actionsToEvaluate.length(); i++) {
      OdDbAssocActionPtr action = OdDbAssocAction::cast(m_actionsToEvaluate[i].openObject());
      if (action.get() && action->status() == kIsUpToDateAssocStatus) {
        action->upgradeOpen();
        action->setStatus(kChangedDirectlyAssocStatus);
        ODA_TRACE(L"Status consistency fix: 'changed directly' : "); DBG_DUMP_OBJID(action->objectId()); ODA_TRACE(L"\n");
        hasDirtyActions = true;
      }
    }
    for (unsigned int i = 0; i < m_actions.length(); i++) {
      OdDbAssocActionPtr action = OdDbAssocAction::cast( m_actions[i].openObject() );
      if (action.isNull())
        continue;

      if (action.get() && action->status() == kIsUpToDateAssocStatus) {
        ::odaaFixStatusConsistency(action);
        if(!hasDirtyActions)
          hasDirtyActions = isEvaluationRequest(action->status());
      }
    }
    if(hasDirtyActions) {
      self->upgradeOpen();
      self->setStatus(kChangedDirectlyAssocStatus);
      ODA_TRACE(L"Status consistency fix: 'changed directly' : "); DBG_DUMP_OBJID(self->objectId()); ODA_TRACE(L"\n");
    }
    m_fixStatusConsistency = false;
  }
}

void OdDbImpAssocNetwork::getDependentActionsToEvaluate(const OdDbAssocAction* pThisNetwork, OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const {
  if (isEvaluationRequest(status())) {
    OdDbImpAssocAction::getDependentActionsToEvaluate(pThisNetwork, pActionsToEvaluateCallback);
    OdDbObjectIdArray actionsToEvaluate = m_actionsToEvaluate;
    OdDbObjectIdArray::const_iterator iter = actionsToEvaluate.begin();
    OdDbObjectIdArray::const_iterator end = actionsToEvaluate.end();
    while (iter != end) {
      OdDbAssocActionPtr action = OdDbAssocAction::cast(iter->openObject(OdDb::kForWrite));
      if (action.get())
        pActionsToEvaluateCallback->actionNeedsToEvaluate(action, action->status());

      ++iter;
    }
  }
}

OdDbAssocEvaluationPriority OdDbImpAssocNetwork::evaluationPriority() const {
  if (m_actionsToEvaluate.empty())
    return OdDbAssocEvaluationPriority(kCanBeEvaluatedAssocEvaluationPriority / 2 + evaluationRequestSeverityLevel(status()));
  return OdDbAssocEvaluationPriority(kCannotBeEvaluatedAssocEvaluationPriority / 2 - m_actionsToEvaluate.size());
}
