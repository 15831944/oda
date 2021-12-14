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
#include "DbAssocAction.h"
#include "DbAssocActionBody.h"
#include "DbAssocDependency.h"
#include "DbDictionary.h"
#include "DbAssocNetCloneCtx.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbAssocValueDependency.h"
#include "DbAssocVariable.h"
#include "DbImpAssocAction.h"
#include "DbAssocActionParam.h"
#include "DbEvalVariant.h"
#include "DbProxyObject.h"

#include "DbFiler.h"
#include "algorithm"
#include "DbAudit.h"
#include "DbHostAppServices.h"
#include <typeinfo>
#include "DbAssocNetwork.h"
#include "SaveState.h"

class findValueParamByName
{
  const OdString& m_Name;
public:
  inline findValueParamByName(const OdString& Name)
    : m_Name(Name)
  {
  }

  inline bool operator()(const OdDbImpValueParamPtr &ValueParam) const
  {
    return ValueParam->name() == m_Name;
  }
};

OdDbImpAssocAction::OdDbImpAssocAction()
  : m_status(kIsUpToDateAssocStatus)
  , m_evalPriority(0)
  , m_actionIndex(0)
  , m_maxAssocDepIdx(0)
  , m_isBase(true)
  , m_EvaluationInProgress(false)
  , m_version(2)
{}

OdDbImpAssocAction::~OdDbImpAssocAction()
{
}

OdDbImpAssocAction* OdDbImpAssocAction::getImpObject(OdDbAssocAction *pAction)
{
  return pAction->m_pImpObj;
}

OdDbObjectId OdDbImpAssocAction::actionBody() const
{
  return m_idActionBody;
}
    
OdResult OdDbImpAssocAction::setActionBody(OdDbObjectId& actionId, const OdDbObjectId& actionBodyId)
{
  m_idActionBody = actionBodyId;
  
  if ( !m_idActionBody.isNull() )
  {
    OdDbObjectPtr pObj = m_idActionBody.openObject(OdDb::kForWrite);
    pObj->setOwnerId(actionId);
  }
  return eOk;
}

OdDbAssocStatus OdDbImpAssocAction::status() const
{
  return m_status;
}


OdResult OdDbImpAssocAction::setStatus(OdDbObject* pThisAction, 
                                       OdDbAssocStatus newStatus,
                                       bool notifyOwningNetwork,
                                       bool setInOwnedActions)
{
  OdDbAssocStatus oldStatus = m_status;
  if (newStatus == oldStatus)
    return eOk;

  if ( isEvaluationRequest(newStatus) )
  {
    if ( !isToBeSkipped(oldStatus) )
    {
      if ( isEvaluationRequest(oldStatus) )
      {
        if ( evaluationRequestSeverityLevel(newStatus) > evaluationRequestSeverityLevel(oldStatus) )
          writeStatus(pThisAction, newStatus);
      }
      else
      {
        postpone_or_writeStatus(pThisAction, newStatus, setInOwnedActions);
      }
    }
  }
  else
  {
    if (newStatus == kErasedAssocStatus && !m_EvaluationInProgress)
      postpone_or_writeStatus(pThisAction, newStatus, setInOwnedActions);
    else
      writeStatus(pThisAction, newStatus);
  }
  if (notifyOwningNetwork && m_status != oldStatus)
  {
    OdDbAssocNetworkPtr pOwningNetwork = owningNetwork().openObject(OdDb::kForWrite);
    if (pOwningNetwork.get())
      return pOwningNetwork->ownedActionStatusChanged((OdDbAssocAction*)pThisAction, oldStatus);
  }
  return eOk;
}

inline void OdDbImpAssocAction::writeStatus(OdDbObject* pThisAction, OdDbAssocStatus newStatus) {
  pThisAction->assertWriteEnabled();
  m_status = newStatus;
}

bool OdDbAssocManager_saveRequestToEvaluate(const OdDbObjectId& objectId, OdDbAssocStatus newStatus, bool ownedActionsAlso);

inline void OdDbImpAssocAction::postpone_or_writeStatus(OdDbObject* pThisAction, OdDbAssocStatus newStatus, bool setInOwnedActions) {
  if (!OdDbAssocManager_saveRequestToEvaluate(pThisAction->objectId(), newStatus, setInOwnedActions))
    writeStatus(pThisAction, newStatus);
}

OdDbObjectId OdDbImpAssocAction::owningNetwork() const
{
  return m_idOwningNetwork;
}

OdResult OdDbImpAssocAction::setOwningNetwork(OdDbAssocAction *pAction, const OdDbObjectId& networkId, bool alsoSetAsDatabaseOwner)
{
  m_idOwningNetwork = networkId;
  if (alsoSetAsDatabaseOwner)
  {
    pAction->setOwnerId(networkId);
  }
  return eOk;
}
  
OdResult OdDbImpAssocAction::getDependencies(bool readDependenciesWanted,
                                             bool writeDependenciesWanted,
                                             OdDbObjectIdArray& dependencyIds) const
{
  OdDbAssocDependencyPtr pDep;
  for ( unsigned int i = 0; i < m_arrDependencies.length(); i++ )
  {
    pDep = m_arrDependencies[i].openObject();
    if ( pDep.isNull() )
      continue;

    if ( readDependenciesWanted && pDep->isReadDependency() )
    {
      dependencyIds.push_back( m_arrDependencies[i] );
      continue;
    }

    if ( writeDependenciesWanted && pDep->isWriteDependency() )
    {
      dependencyIds.push_back( m_arrDependencies[i] );
    }
  }

  return eOk;
}
OdResult OdDbImpAssocAction::addDependency(const OdDbObjectId& dependencyId, 
                       bool setThisActionAsOwningAction, OdDbAssocAction *pAction)
{
  int idx = findDependenciesArrayIndex(dependencyId);

  if ( idx >= 0 )
    return eOk;

  OdDbAssocDependencyPtr pDep = dependencyId.openObject(setThisActionAsOwningAction ?
                                                          OdDb::kForWrite : OdDb::kForRead);
  if ( pDep.isNull() )
    return eBadObjType;

  m_arrDependencies.append(dependencyId);
  m_isOwnedDependency.append(setThisActionAsOwningAction);
  if ( setThisActionAsOwningAction )
  {
    pDep->setOwningAction(pAction->objectId());
    pDep->setIsDelegatingToOwningAction(true);
  }
  
  return eOk;
}

inline void updateRemovingDependency(const OdDbObjectId& dependencyId, bool alsoEraseIt) {
  OdDbAssocDependencyPtr pDep = dependencyId.openObject(OdDb::kForWrite);
  if ( pDep.get() ) {
    pDep->setOwningAction(OdDbObjectId::kNull);
    if ( alsoEraseIt ) {
      pDep->setStatus(kErasedAssocStatus, false);
      pDep->detachFromObject();
      pDep->erase(true);
    }
  }
}

OdResult OdDbImpAssocAction::removeDependency(const OdDbObjectId& dependencyId, 
                          bool alsoEraseIt)
{
  int idx = findDependenciesArrayIndex(dependencyId);
  if ( idx == -1 )
    return eBadObjType;

  m_arrDependencies.removeAt(idx);
  m_isOwnedDependency.removeAt(idx);
  ::updateRemovingDependency(dependencyId, alsoEraseIt);
  return eOk;
}

OdResult OdDbImpAssocAction::removeAllDependencies(bool alsoEraseThem)
{
  for ( unsigned int i = 0; i < m_arrDependencies.length(); i++ )
    ::updateRemovingDependency(m_arrDependencies[i], alsoEraseThem);

  m_arrDependencies.clear();
  m_isOwnedDependency.clear();
  return eOk;
}

OdResult OdDbImpAssocAction::getDependentObjects(bool readDependenciesWanted,
                                                 bool writeDependenciesWanted,
                                                 OdDbObjectIdArray& objectIds) const
{
  for (unsigned int i = 0, sz = m_arrDependencies.length(); i < sz; i++)
  {
    OdDbObjectId objId = m_arrDependencies[i];
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(objId.openObject());
    if ( pDep.isNull() )
      continue;

    if ( readDependenciesWanted && pDep->isReadDependency() )
    {
      objId = pDep->dependentOnObject();
    }
    else if ( writeDependenciesWanted && pDep->isWriteDependency() )
    {
      objId = pDep->dependentOnObject();
    }
    else
      continue;

    bool isExist = false;
    for (unsigned int j = 0, szJ = objectIds.length(); j < szJ; j++)
    {
      if ( objectIds[j] == objId )
      {
        isExist = true;
        break;
      }
    }

    if ( isExist )
      continue;
    else
      objectIds.push_back(objId);
  }
  
  return eOk;
}
/*

    bool OdDbImpAssocAction::is(const OdDbAssocDependency* pDependency) const;

    bool OdDbImpAssocAction::isExternalDependency(const OdDbAssocDependency* pDependency) const;
*/

OdResult OdDbImpAssocAction::checkGetBody(OdDbAssocActionBodyPtr& out, OdDb::OpenMode mode) const {
  OdDbObjectPtr bodyObj = actionBody().openObject(mode);
  if (bodyObj.isNull())
    return eNotImplemented;
  if (bodyObj->isAProxy() || !bodyObj->isKindOf(OdDbAssocActionBody::desc()))
    return eNotThatKindOfClass;
  out = (OdDbAssocActionBody*)bodyObj.get();
  return eOk;
}

bool OdDbImpAssocAction::doesObjectHaveActiveActions(const OdDbObject* object, const OdDbObjectId& exceptAction) {
  if(object->isKindOf(OdDbAssocAction::desc())) {
    OdDbAssocAction* action = (OdDbAssocAction*)object;
    if(isEvaluationRequest(action->status()))
      return true;
  }
  OdDbAssocDependencyPtr dependency = OdDbAssocDependency::getFirstDependencyOnObject(object).openObject();
  while (dependency.get()) {
    if (dependency->isWriteDependency() && !dependency->isReadDependency() && dependency->owningAction()!=exceptAction) {
      OdDbAssocActionPtr action = dependency->owningAction().openObject();
      if (action.get() && isEvaluationRequest(action->status()))
        return true;
    }
    dependency = dependency->nextDependencyOnObject().openObject();
  }
  return false;
}

bool OdDbImpAssocAction::isRelevantDependencyChange(const OdDbAssocDependency* pDependency) const
{
  return true;
}
/*
    bool OdDbImpAssocAction::hasDependencyCachedValue(const OdDbAssocDependency* pDependency) const;

    bool OdDbImpAssocAction::areDependenciesOnTheSameThing(const OdDbAssocDependency* pDependency1, 
                                       const OdDbAssocDependency* pDependency2) const;

    bool OdDbImpAssocAction::areDependenciesEqual(const OdDbAssocDependency* pDependency1, 
                              const OdDbAssocDependency* pDependency2) const;

*/


OdResult OdDbImpAssocAction::addMoreObjectsToDeepClone(const OdDbAssocAction* pThisAction, OdDbIdMapping& idMap, 
                                                        OdDbObjectIdArray& additionalObjectsToClone ) const
{
  return eOk;
}

OdResult OdDbImpAssocAction::postProcessAfterDeepClone(OdDbAssocAction *pAction, OdDbIdMapping& idMap)
{
  return eOk;
}

OdResult OdDbImpAssocAction::postProcessAfterDeepCloneCancel(OdDbAssocAction *pAction, OdDbIdMapping& idMap)
{
  return eOk;
}

bool OdDbImpAssocAction::isActionEvaluationInProgress() const
{
  return m_EvaluationInProgress;
}

OdDbAssocEvaluationCallback* OdDbImpAssocAction::currentEvaluationCallback() const
{
  ODA_FAIL();
  //throw OdError(eNotImplemented);
  return NULL;
}

OdResult OdDbImpAssocAction::evaluateDependencies() {
  OdDbObjectIdArray::const_iterator iter = m_arrDependencies.begin();
  OdDbObjectIdArray::const_iterator end = m_arrDependencies.end();
  while (iter != end) {
    OdDbAssocDependencyPtr dep = iter->openObject(OdDb::kForWrite);
    if(dep.get() && dep->status()!=kIsUpToDateAssocStatus)
      dep->evaluate();
    ++iter;
  }
  return eOk;
}

bool OdDbImpAssocAction::filterChanges_hasRelevantDependencyChange() {
  OdDbObjectIdArray arrDependencies = m_arrDependencies;
  OdDbObjectIdArray::const_iterator iter = arrDependencies.begin();
  OdDbObjectIdArray::const_iterator end = arrDependencies.end();
  bool hasRelevant = false;
  while (iter != end) {
    OdDbAssocDependencyPtr dep = iter->openObject(OdDb::kForWrite);
    if (dep.get()) {
      OdDbAssocStatus status = dep->status();
      if (dep->isReadDependency()) {
        if(status==kErasedAssocStatus) {
          hasRelevant = true;
        }
        else if (isEvaluationRequest(status)) {
          if (dep->isRelevantChange())
            hasRelevant = true;
          else
            dep->evaluate();
        }
      }
    }
    ++iter;
  }
  return hasRelevant;
}

void OdDbImpAssocAction::evaluateDependency(OdDbAssocDependency* pDependency)
{
  pDependency->setStatus(kIsUpToDateAssocStatus, false);
}

OdResult OdDbImpAssocAction::ownedDependencyStatusChanged(OdDbAssocAction* pThisAction, OdDbAssocDependency* pOwnedDependency, 
                                      OdDbAssocStatus      previousStatus)
{
  OdDbAssocStatus status = pOwnedDependency->status();
  if (status == kIsUpToDateAssocStatus)
    return eOk;
  return setStatus(pThisAction, pOwnedDependency->isReadDependency() ? kChangedTransitivelyAssocStatus : kChangedNoDifferenceAssocStatus);
}

OdResult OdDbImpAssocAction::transformActionBy(OdDbAssocAction* thisAction, const OdGeMatrix3d& transform) {
  return eNotImplemented;
}

/*
    bool OdDbImpAssocAction::isEqualTo(const OdDbImpAssocAction* pOtherAction) const;
*/

OdDbAssocEvaluationPriority OdDbImpAssocAction::evaluationPriority() const {
  OdDbAssocStatus stat = status();
  int numActiveDeps = 0;
  OdDbObjectIdArray::const_iterator iter = m_arrDependencies.begin();
  OdDbObjectIdArray::const_iterator end = m_arrDependencies.end();
  while (iter != end) {
    OdDbAssocDependencyPtr dep = iter->openObject();
    if (dep.get() && dep->isReadDependency()) {
      OdDbObjectPtr object = dep->dependentOnObject().openObject();
      if(object.get() && OdDbImpAssocAction::doesObjectHaveActiveActions(object, dep->owningAction()))
        ++numActiveDeps;
    }
    ++iter;
  }

  if (numActiveDeps)
    return OdDbAssocEvaluationPriority(kCannotBeEvaluatedAssocEvaluationPriority - numActiveDeps);
  return OdDbAssocEvaluationPriority(kCanBeEvaluatedAssocEvaluationPriority + evaluationRequestSeverityLevel(status()));
}

void OdDbImpAssocAction::getDependentActionsToEvaluate(const OdDbAssocAction* pThisAction, OdDbActionsToEvaluateCallback* pActionsToEvaluateCallback) const
{
  if (isEvaluationRequest(status())) {
    OdDbObjectIdArray arrDependencies = m_arrDependencies;
    OdDbObjectIdArray::const_iterator iter = arrDependencies.begin();
    OdDbObjectIdArray::const_iterator end = arrDependencies.end();
    while(iter!=end) {
      OdDbAssocDependencyPtr dep = iter->openObject(OdDb::kForWrite);
      if (dep.get() && dep->isWriteDependency())
        pActionsToEvaluateCallback->dependencyNeedsToEvaluate(dep, kChangedTransitivelyAssocStatus);
      ++iter;
    }
  }
}

void OdDbImpAssocAction::evaluate(OdDbAssocAction *pAction, OdDbAssocEvaluationCallback* pEvaluationCallback)
{
  ODA_ASSERT_ONCE(("OdDbImpAssocAction::evaluate() is not implemented and should not be called",0));
}

OdDbObjectId OdDbImpAssocAction::objectThatOwnsNetworkInstance(const OdDbAssocAction *pThisAction) const
{
  OdDbObjectPtr obj(pThisAction->ownerId().openObject());

  while (obj.get() && obj->isKindOf(OdDbAssocNetwork::desc()))
    obj = obj->ownerId().openObject();

  while (obj.get() && obj->isKindOf(OdDbDictionary::desc()))
    obj = obj->ownerId().openObject();

  return obj.get() ? obj->objectId() : OdDbObjectId::kNull;
}

//void OdDbImpAssocAction::dragStatus(const OdDb::DragStat status);

OdResult OdDbImpAssocAction::dwgInFields(OdDbAssocAction *pSelf, OdDbDwgFiler* pFiler)
{
  m_version = pFiler->rdInt16();
  if (!m_version || m_version > 2)
  {
    ODA_FAIL(); // TODO
    return eMakeMeProxy;
  }

  m_isOwnedDependency.clear();
  m_arrDependencies.clear();
  m_arrParams.clear();
  m_arrValueParams.clear();

  m_status = OdDbAssocStatus(pFiler->rdInt32());

  m_idOwningNetwork = pFiler->rdSoftPointerId();
  m_idActionBody = pFiler->rdHardOwnershipId();

  m_actionIndex = pFiler->rdInt32();

  m_maxAssocDepIdx = pFiler->rdInt32();

  OdInt32 n = pFiler->rdInt32();
  if (n > 0) {
    OdDb::MaintReleaseVer maintReleaseVer;
    OdDb::DwgVersion ver = pFiler->dwgVersion(&maintReleaseVer);
    bool isOwned = true;
    while (n-- > 0) {
      try
      {
        isOwned = pFiler->rdBool();
      }
      catch (OdError&)
      {
        // CORE-14184 Crash on evaluating constraints with assoc actions as proxies
        // It is strange but for files xref1.dwg & xref2.dwg all ids of m_arrDependencies should be read as rdHardOwnershipId.
        // And it is eof of stream (next isOwned = pFiler->rdBool() generate exception via it)
        // But we have correct reading for same handles if it is resaved as DWG/DXF 2000 or 2010 in ACAD.
        ODA_ASSERT_ONCE_X(ASSOC,   !n && !isOwned
                                 && m_version == 1 && ver == OdDb::kDHL_1021 && maintReleaseVer == OdDb::kMRelease25);
        return eMakeMeProxy;
      }
      m_isOwnedDependency.push_back(isOwned);
      if (isOwned)
        m_arrDependencies.push_back(pFiler->rdHardOwnershipId());
      else if (m_version == 1 && ver == OdDb::kDHL_1021 && maintReleaseVer == OdDb::kMRelease25)
        m_arrDependencies.push_back(pFiler->rdHardOwnershipId());
      else
        m_arrDependencies.push_back(pFiler->rdSoftPointerId());
    }
  }

  if (m_version > 1) 
  {
    // AC27
    if (pFiler->rdInt16())
    { // TODO AC2013
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }

    // Param Ids
    n = pFiler->rdInt32();
    while (n-- > 0) {
      m_arrParams.push_back(pFiler->rdHardOwnershipId());
    }
    
    if (pFiler->rdInt16())
    { // TODO AC2013
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }

    n = pFiler->rdInt32();
    while (n-- > 0) 
    {
      OdDbImpValueParamPtr pValueParam = new OdDbImpValueParam(); 
      OdResult res = pValueParam->dwgInFields(pFiler);
      if (res != eOk)
        return res;
      m_arrValueParams.push_back(pValueParam);
    }
  }
  return eOk;
}

void OdDbImpAssocAction::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  int ver = (pFiler->dwgVersion() >= OdDb::vAC27) ? 2 : 1;
  pFiler->wrInt16(ver);

  pFiler->wrInt32(m_status);

  pFiler->wrSoftPointerId(m_idOwningNetwork);
  pFiler->wrHardOwnershipId(m_idActionBody);

  pFiler->wrInt32(m_actionIndex);
  pFiler->wrInt32(m_maxAssocDepIdx);
  OdInt32 n = (m_isBase) ? m_arrDependencies.size() : 0;
  pFiler->wrInt32(n);
  OdInt32 index = 0;
  for (; index < n; index++) 
  {
    bool isOwned = m_isOwnedDependency[index];
    pFiler->wrBool(isOwned);
    if (isOwned)
      pFiler->wrHardOwnershipId(m_arrDependencies[index]);
    else
      pFiler->wrSoftPointerId(m_arrDependencies[index]);
  }

  if (ver > 1) 
  {
    // AC27
    pFiler->wrInt16(0);
  
    // Param Ids
    n = m_arrParams.size();
    pFiler->wrInt32(n); // Param Ids
    for (index = 0; index < n; index++)
      pFiler->wrHardOwnershipId(m_arrParams[index]);

    pFiler->wrInt16(0);

    n = m_arrValueParams.size();
    pFiler->wrInt32(n);
    for (index = 0; index < n; index++) 
    {
      OdDbImpValueParamPtr pValueParam = m_arrValueParams[index]; 
      pValueParam->dwgOutFields(pFiler);
    }
  }
}

#define NEXT_CODE(code)         \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL_ONCE();              \
  return eMakeMeProxy;          \
}

OdResult OdDbImpAssocAction::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  // Check that we are at the correct subclass data
  if (!pFiler->atSubclassData(OdDbAssocAction::desc()->name()))
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  m_isOwnedDependency.clear();
  m_arrDependencies.clear();
  m_arrParams.clear();
  m_arrValueParams.clear();

  // Read version number (must be first)
  NEXT_CODE(90)
  m_version = pFiler->rdUInt32();
  if (m_version > 2)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(90)
  m_status = OdDbAssocStatus(pFiler->rdUInt32());

  NEXT_CODE(330)
  m_idOwningNetwork = pFiler->rdObjectId();
  NEXT_CODE(360)
  m_idActionBody = pFiler->rdObjectId();

  NEXT_CODE(90)
  m_actionIndex = pFiler->rdUInt32();

  NEXT_CODE(90)
  m_maxAssocDepIdx = pFiler->rdUInt32();

  NEXT_CODE(90)
  OdUInt32 n = pFiler->rdUInt32();
  while (n-- > 0) {
    switch(pFiler->nextItem()) {
    case 330:
      m_isOwnedDependency.append(false);
      break;
    case 360:
      m_isOwnedDependency.append(true);
      break;
    default:
      return eMakeMeProxy;
    }
    m_arrDependencies.push_back(pFiler->rdObjectId());
  }
  if (m_version > 1)
  {
    NEXT_CODE(90)
    if (pFiler->rdUInt32() > 0)
    {  // TODO AC2013
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }

    NEXT_CODE(90)
    // Param Ids
    n = pFiler->rdUInt32();
    ODA_ASSERT_ONCE(!n || !m_idActionBody.isNull());
    m_arrParams.reserve(n);
    while (n-- > 0) {
      NEXT_CODE(360)
      m_arrParams.append(pFiler->rdObjectId());
    }

    NEXT_CODE(90)
    if (pFiler->rdUInt32() > 0)
    {  // TODO AC2013
      ODA_FAIL_ONCE();
      return eMakeMeProxy;
    }

    NEXT_CODE(90)
    n = pFiler->rdUInt32();
    ODA_ASSERT_ONCE(!n || !m_idActionBody.isNull());
    m_arrValueParams.clear();
    // see OdDbImpAssocParamBasedActionBody::dxfInFields R24 :

    while (n-- > 0) 
    {
      OdDbImpValueParamPtr pValueParam = new OdDbImpValueParam(); 
      OdResult res = pValueParam->dxfInFields(pFiler);
      if (res != eOk)
        return res;
      m_arrValueParams.push_back(pValueParam);
    }
    ODA_ASSERT_ONCE(   pFiler->atEOF()  // TODO
                       // OdDbImpAssoc2dConstraintGroup::dxfInFields does not contain subclass name reading
                    || OdDbAssoc2dConstraintGroup::cast(objectId.openObject().get()).get());
  }

  return eOk;
}

void OdDbImpAssocAction::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  pFiler->wrSubclassMarker(OdDbAssocAction::desc()->name());

  int nVersion = (pFiler->dwgVersion() <= OdDb::vAC24) ? 1 : 2;
  pFiler->wrInt32(90, nVersion);

  pFiler->wrUInt32(90, m_status);

  pFiler->wrObjectId(330, m_idOwningNetwork);
  pFiler->wrObjectId(360, m_idActionBody);

  pFiler->wrUInt32(90, m_actionIndex);
  pFiler->wrUInt32(90, m_maxAssocDepIdx);
  OdUInt32 n = (m_isBase) ? m_arrDependencies.size() : 0;
  pFiler->wrUInt32(90, n);
  OdUInt32 i = 0;
  for (; i < n; i++)
  if (m_isOwnedDependency[i]) {
   pFiler->wrObjectId(360, m_arrDependencies[i]);
  }
  else {
    pFiler->wrObjectId(330, m_arrDependencies[i]);
  }

  if (nVersion > 1)
  {
    // AC2013
    pFiler->wrUInt32(90, 0);  

    n = m_arrParams.size();
    ODA_ASSERT_ONCE(!n || !m_idActionBody.isNull());
    pFiler->wrUInt32(90, n);
    for (i = 0; i < n; i++) {
      pFiler->wrObjectId(360, m_arrParams[i]);
    }
   
    pFiler->wrUInt32(90, 0);

    n = m_arrValueParams.size();
    ODA_ASSERT_ONCE(!n || !m_idActionBody.isNull());
    pFiler->wrUInt32(90, n);
    for (i = 0; i < n; i++)
      m_arrValueParams[i]->dxfOutFields(pFiler);
  }
}

OdStringArray OdDbImpAssocAction::compareWith(const OdDbImpAssocAction *actionIDToCompare, OdDb::DwgVersion filerVersion) const
{ 
	OdStringArray retArr;  
	
	if (m_status != actionIDToCompare->m_status)
		retArr.append(CS_NOT_CRITICAL);

	if (!m_idOwningNetwork.isNull() && !actionIDToCompare->m_idOwningNetwork.isNull())
	{
		if (typeid(*(m_idOwningNetwork).openObject()) != typeid(*(actionIDToCompare->m_idOwningNetwork.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (!m_idActionBody.isNull() && !actionIDToCompare->m_idActionBody.isNull())
	{
		if (typeid(*(m_idActionBody).openObject()) != typeid(*(actionIDToCompare->m_idActionBody.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (m_actionIndex != actionIDToCompare->m_actionIndex)
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

	if (m_maxAssocDepIdx != actionIDToCompare->m_maxAssocDepIdx)
    retArr.append(CS_NOT_CRITICAL);
		//retArr.append(CS_DIFFERENT_PARAM_CRITICAL);

	if (m_arrDependencies.size() != actionIDToCompare->m_arrDependencies.size())
	{
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
		return retArr;
	}
	
	OdUInt32 n = m_arrDependencies.size();
	
	OdUInt32 i = 0;
	
	for (i = 0; i < n; i++)
	{
		if (!m_arrDependencies[i].isNull() && !actionIDToCompare->m_arrDependencies[i].isNull())
		{
			if (typeid(*(m_arrDependencies[i]).openObject()) != typeid(*(actionIDToCompare->m_arrDependencies[i].openObject())))
				retArr.append(CS_DIFFERENT_OBJECTS);
		}
	}
	
	if (m_arrParams.size() != actionIDToCompare->m_arrParams.size())
	{
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
		return retArr;
	}

	n = m_arrParams.size();

	for (i = 0; i < n; i++)
	{
		if (!m_arrParams[i].isNull() && !actionIDToCompare->m_arrParams[i].isNull())
		{
			if (typeid(*(m_arrParams[i]).openObject()) != typeid(*(actionIDToCompare->m_arrParams[i].openObject())))
				retArr.append(CS_DIFFERENT_OBJECTS);
		}
	}

	if (m_arrValueParams.size() != actionIDToCompare->m_arrValueParams.size())
	{
		retArr.append(CS_DIFFERENT_PARAM_CRITICAL);
		return retArr;
	}

	n = m_arrValueParams.size();

	for (i = 0; i < n; i++)
	{
		OdStringArray retArrV = m_arrValueParams[i]->compareWith(actionIDToCompare->m_arrValueParams[i], filerVersion);
		retArr.append(retArrV);
	}


	return retArr;
}

void OdDbImpAssocAction::composeForLoad( OdDbObject* pObj, 
                                         OdDb::SaveType format,
                                         OdDb::DwgVersion version,
                                         OdDbAuditInfo* pAuditInfo )
{
  OdInt16 versionOnLoad = m_version;
  m_version = 2;

  OdDbObjectId id = pObj->objectId();

  OdDbAssocActionBodyPtr pActionBody = OdDbAssocActionBody::cast(m_idActionBody.openObject());
  if ( !pActionBody.isNull() && (format != OdDb::kDwg || (pObj->database()->multiThreadedMode() != OdDb::kSTMode )))
  {
    pActionBody->composeForLoad( format, version, pAuditInfo );

    OdUInt32 cnt = m_arrParams.size();
    for (OdUInt32 idx = 0; idx < cnt; idx++)
    {
      OdDbObjectPtr pParam = m_arrParams[idx].openObject(OdDb::kForWrite);
      if (pParam.isNull())
        continue;
      OdDbObjectId idOwner = pParam->ownerId();
      if (idOwner != id)
        continue;
      pParam->composeForLoad(format, version, pAuditInfo);
    }
  }

  if (versionOnLoad > 1 || m_idActionBody.isNull() || pActionBody.isNull())
    return;

  //OdDbImpAssocParamBasedActionBody* pParamBaseImpl = static_cast<OdDbImpAssocParamBasedActionBody*>(pActionBody->m_pImpObj);
  OdDbImpAssocActionBody* pParamBaseImpl = static_cast<OdDbImpAssocActionBody*>(pActionBody->m_pImpObj);

  bool bUpdateOwner = (m_arrParams.isEmpty());

  m_arrParams = pParamBaseImpl->m_arrParams;
  if (bUpdateOwner)
  {
    OdUInt32 cnt = m_arrParams.size();
    for (OdUInt32 idx = 0; idx < cnt; idx++)
    {
      OdDbObjectId idChild = m_arrParams[idx];
      OdDbObjectPtr pChild = idChild.openObject(OdDb::kForWrite);
      if (pChild.isNull())
        continue;

      OdDbObjectId idOwner = pChild->ownerId();
      // fix owner of child to fix audit problem (for soubor_2010.dxf etc)
      if (idOwner == id)
      {
        ODA_ASSERT_ONCE(!idOwner.isNull() && id == idOwner.openObject()->ownerId());
        pChild->setOwnerId(id);
      }
    }
  }

  m_arrValueParams = pParamBaseImpl->m_arrValueParams;
}

void OdDbImpAssocAction::decomposeForSave(OdDbObject* pObj,
                                          OdDb::SaveType format, 
                                          OdDb::DwgVersion version)
{
  if (version > OdDb::vAC24 || m_idActionBody.isNull())
    return;
  OdDbAssocActionBodyPtr pActionBody = OdDbAssocActionBody::cast(m_idActionBody.openObject());
  if (pActionBody.isNull() || !pActionBody->m_pImpObj)
  {
    //ODA_ASSERT_ONCE(OdDbProxyObject::cast(m_idActionBody.openObject()).get());
    return;
  }

  OdDbImpAssocParamBasedActionBody* pParamBaseImpl = static_cast<OdDbImpAssocParamBasedActionBody*>(pActionBody->m_pImpObj);

  pParamBaseImpl->m_arrParams = m_arrParams;
  pParamBaseImpl->m_arrValueParams = m_arrValueParams;
}

OdResult OdDbImpAssocAction::setActionIndex(int newIndex)
{
  if ( newIndex <= 0 )
    return eInvalidInput;

  m_actionIndex = newIndex;
  return eOk;
}

int OdDbImpAssocAction::getMaximumAssocDepIndex()
{
  return m_maxAssocDepIdx;
}

void OdDbImpAssocAction::setMaximumAssocDepIndex(int index)
{
  m_maxAssocDepIdx = index;
}

int OdDbImpAssocAction::findDependenciesArrayIndex(OdDbObjectId objectId)
{
  for ( unsigned int i = 0; i < m_arrDependencies.length(); i ++ )
    if ( m_arrDependencies[i] == objectId )
      return i;

  return -1;
}

#ifndef NDEBUG
void DBG_DUMP_OBJID(const OdDbObjectId& id);
#else
#define DBG_DUMP_OBJID(id)
#endif

void OdDbImpAssocAction::fixStatusConsistency(OdDbAssocAction* self) {
  if (status() == kIsUpToDateAssocStatus) {
    for (unsigned int i = 0; i < m_arrDependencies.length(); i++) {
      OdDbAssocDependencyPtr dependeny = OdDbAssocDependency::cast(m_arrDependencies[i].openObject());
      if (dependeny.get() && dependeny->status() != kIsUpToDateAssocStatus) {
        self->upgradeOpen();
        self->setStatus(kChangedDirectlyAssocStatus);
        ODA_TRACE(L"Status consistency fix: 'changed directly' : "); DBG_DUMP_OBJID(self->objectId()); ODA_TRACE(L"\n");
        break;
      }
    }
  }
}

// AC2013
// OdDbAssocActionParams stuff
OdResult OdDbImpAssocAction::removeAllParams(bool bEraseThem)
{
  if (bEraseThem)
  {
    for (int i = m_arrParams.size(); i > 0;)
    {
      OdDbObjectPtr pParam = m_arrParams[--i].openObject(OdDb::kForWrite);
      if (!pParam.isNull())
        pParam->erase();
    }
  }
  m_arrParams.clear();
  return eOk;
}


int OdDbImpAssocAction::paramCount() const
{
  return m_arrParams.size();
}


const OdDbObjectIdArray& OdDbImpAssocAction::ownedParams() const
{
  return m_arrParams;
}


OdResult OdDbImpAssocAction::addParam(const OdDbObjectId& paramId, int& paramIndex)
{
  for (int i = m_arrParams.size(); i > 0;)
  {
    if (m_arrParams[--i] == paramId)
    {
      paramIndex = i;
      return eOk;
    }
  }
  paramIndex = m_arrParams.append(paramId);
  return eOk;
}

OdResult OdDbImpAssocAction::removeParam(const OdDbObjectId& paramId, bool alsoEraseIt)
{
  for (int i = m_arrParams.size(); i > 0;)
  {
    if (m_arrParams[--i] == paramId)
    {
      if (alsoEraseIt)
      {
        OdDbObjectPtr pParam = m_arrParams[i].openObject(OdDb::kForWrite);
        if (!pParam.isNull())
          pParam->erase();
      }
      m_arrParams.removeAt(i);
      i--;
      return eOk;
    }
  }
  return eInvalidInput;
}

OdDbObjectId OdDbImpAssocAction::paramAtIndex(int paramIndex) const
{
  return m_arrParams[paramIndex];
}

//#if defined(DB_ASSOC_ACTION_PARAM_METHOGS_DEFINED)

OdResult OdDbImpAssocAction::addParam(const OdString& paramName, 
                                      OdRxClass* pParamClass, 
                                      OdDbObjectId& paramId, 
                                      int& paramIndex,
                                      OdDbAssocAction *AssocAction)
{
  ODA_ASSERT_ONCE(!paramName.isEmpty() && pParamClass);
  OdDbObjectIdArray idPars = paramsAtName(paramName);
  if (paramIndex < (int)idPars.size())
  {
    paramId = idPars[paramIndex];
    return eAlreadyInDb;
  }

  OdDbObjectPtr pObj = OdDbObject::cast(pParamClass->create());
  if (pObj.isNull())
    return eBadObjType;

  OdDbAssocActionParamPtr pParam = OdDbAssocActionParam::cast(pObj.get()); 
  ODA_ASSERT_ONCE(!pParam.isNull());
  if (pParam.isNull())
    return eBadObjType;
  
  AssocAction->database()->addOdDbObject( pParam );

  pParam->setOwnerId( AssocAction->objectId() );
  pParam->setName(paramName);

  paramId = pParam->objectId();


  m_arrParams.push_back(paramId);

  return eOk;
}

OdDbObjectIdArray OdDbImpAssocAction::paramsAtName(const OdString& paramName) const
{
  ODA_ASSERT_ONCE(!paramName.isEmpty());

  OdDbObjectIdArray idsRes;
  OdUInt32 sz = m_arrParams.size(),
           idx = 0;
  for (; idx < sz; idx++)
  {
    OdDbObjectPtr pObj = m_arrParams[idx].openObject();
    ODA_ASSERT_ONCE(!pObj.isNull());
    if (pObj.isNull())
      continue;

    OdDbAssocActionParamPtr pParam = OdDbAssocActionParam::cast(pObj.get()); 
    ODA_ASSERT_ONCE(!pParam.isNull());
    if (pParam.isNull())
      continue;
    
    OdString sName = pParam->name();
    ODA_ASSERT_ONCE(!sName.isEmpty());
    if (paramName != sName)
      continue;

    idsRes.push_back(m_arrParams[idx]);
  }

  return idsRes;
}
  
OdDbObjectId OdDbImpAssocAction::paramAtName(const OdString& paramName,
                                             int index) const // = 0
{
  OdDbObjectIdArray ids = paramsAtName(paramName);
  ODA_ASSERT_ONCE(index >= 0);
  if (index < 0 || index >= (int) ids.size())
    return OdDbObjectId();
  return ids[index];
}

void OdDbImpAssocAction::ownedValueParamNames(OdStringArray& paramNames) const
{
  paramNames.clear();

  OdArray<const OdDbImpValueParamPtr>::iterator it; 

  for (it = m_arrValueParams.begin(); it != m_arrValueParams.end(); it++)
  {
    paramNames.push_back( (*it)->name() );
  }
}

OdResult OdDbImpAssocAction::getValueParamArray(const OdString& paramName,
                                                OdArray<OdDbEvalVariantPtr>& values,
                                                OdStringArray& expressions,
                                                OdStringArray& evaluatorIds) const
{
  values.clear();
  expressions.clear();
  evaluatorIds.clear();

  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if(m_arrValueParams.begin(), m_arrValueParams.end(), findValueParamByName(paramName));

  if (m_arrValueParams.end() == it)
    return eNotInGroup;

  OdDbObjectIdArray inputVariablesIds = (*it)->inputVariablesIds();
  int nEvalVars = inputVariablesIds.length();

  OdDbEvalVariantPtr pEvalVar = (*it)->value();

  for (int i = 0; i < nEvalVars; ++i)
  {
    values.append(pEvalVar);
    expressions.append(OdString());//TODO
    evaluatorIds.append(OdString());
    pEvalVar = pEvalVar->next();
  }

  return eOk;
}

OdResult OdDbImpAssocAction::getValueParam(const OdString& paramName,
                                           OdDbEvalVariant& value,
                                           OdString& expression,
                                           OdString& evaluatorId,
                                           int valueIndex) const // = 0
{
  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if(m_arrValueParams.begin(), m_arrValueParams.end(), findValueParamByName(paramName));

  if (m_arrValueParams.end() == it)
    return eNotInGroup;

  if (0 == valueIndex)
  {
    value = *(*it)->value();
    return eOk;
  }

  OdDbObjectIdArray inputVariablesIds = (*it)->inputVariablesIds();
  int nEvalVars = inputVariablesIds.length();

  ODA_ASSERT(valueIndex < nEvalVars);

  if (valueIndex >= nEvalVars)
  {
    return eNotInGroup;
  }
  
  OdDbEvalVariantPtr pEvalVar = (*it)->value();

  int counter = 0;
  for (int i = 0; i < nEvalVars; ++i)
  {
    if (valueIndex == counter)
    {
      break;
    }

    pEvalVar = pEvalVar->next();
    counter++;
  }

  value = *pEvalVar;
  return eOk;
}

OdResult OdDbImpAssocAction::setValueParamArray(const OdString& paramName,
                                                const OdArray<OdDbEvalVariantPtr>& values,
                                                const OdStringArray& expressions,
                                                const OdStringArray& evaluatorIds,
                                                OdStringArray& errorMessages,
                                                bool silentMode)
{
  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if(m_arrValueParams.begin(), m_arrValueParams.end(), findValueParamByName(paramName));
  OdDbImpValueParamPtr pValueParam;

  if (m_arrValueParams.end() == it)
  {
    pValueParam = new OdDbImpValueParam;
    pValueParam->setName(paramName);
    m_arrValueParams.push_back(pValueParam);
  }
  else
  {
    pValueParam = (*it);
  }

  OdDbObjectIdArray inputVariablesIds = pValueParam->inputVariablesIds();
  OdUInt16 nEvalVarsStart = inputVariablesIds.length();

  if (values.length() != nEvalVarsStart)
  {
    inputVariablesIds.resize(values.length());
    pValueParam->setInputVariablesIds(inputVariablesIds);
  }

  OdUInt16 nEvalVars = inputVariablesIds.length();

  pValueParam->unitType();
  OdDbEvalVariantPtr pLast = pValueParam->value();
  // add EvalVariants
  for (int i = 0; i < nEvalVars; ++i)
  {
    if (i < nEvalVarsStart - 1)
    {
      pLast = pLast->next();
    }

    if (i >= nEvalVarsStart)
    {
      OdDbEvalVariantPtr pNewEvVar = OdDbEvalVariant::createObject();
      pLast->setNext(pNewEvVar);
      pLast = pNewEvVar;
    }
  }

  OdDbEvalVariantPtr pEvalVar = pValueParam->value( );
  OdDbEvalVariantPtr pEvalVarPrev = pValueParam->value();
///////////////////////////////////////////////////////////
  // delete all spare EvalVariants
  if (values.length() < nEvalVarsStart)
  {
    for (OdUInt16 i = 0; i < nEvalVarsStart; i++)
    {
      pEvalVar = pEvalVarPrev->next();

      OdDbEvalVariantPtr  pEvalVarPrev1 = pEvalVarPrev->next();
      if (values.length() - 1 == i)
      {
        pEvalVarPrev->setNext(NULL);
      }

      if (i >= values.length())
      {
        pEvalVarPrev = NULL;//TODO
      }

      pEvalVarPrev = pEvalVar;
    }
  }
  ////////////////////////////////////////////////////
  pEvalVar = pValueParam->value();
  for (int i = 0; i < nEvalVars; i++)
  {
    *pEvalVar = *(values[i]);
    pEvalVar = pEvalVar->next();
  }

  return eOk;
}


OdResult OdDbImpAssocAction::setValueParam(const OdString& paramName,
                                           const OdDbEvalVariant& value,
                                           const OdString& expression,
                                           const OdString& evaluatorId,
                                           OdString& errorMessage,
                                           bool silentMode,
                                           int valueIndex) // = 0
{
  OdArray<OdDbImpValueParamPtr>::iterator it = std::find_if(m_arrValueParams.begin(),
    m_arrValueParams.end(),
    findValueParamByName(paramName));

  OdDbImpValueParamPtr pValueParam;// = new OdDbImpValueParam;

  if (m_arrValueParams.end() == it)
  {
    pValueParam = new OdDbImpValueParam;
    pValueParam->setName(paramName);
    m_arrValueParams.push_back(pValueParam);
  }
  else
  {
    pValueParam = (*it);
  }

  OdDbObjectIdArray inputVariablesIds = pValueParam->inputVariablesIds();
  int nEvalVarsStart = inputVariablesIds.length();

  if (0 == valueIndex)
  {
    OdDbEvalVariantPtr pEvalVar = pValueParam->value();
    OdDbEvalVariantPtr pEvalVarNext = pEvalVar->next();

    pValueParam->setValue(value);

    if (nEvalVarsStart > 1)
    {
      pEvalVar = pValueParam->value();
      pEvalVar->setNext(pEvalVarNext);
      inputVariablesIds.resize(nEvalVarsStart);
      pValueParam->setInputVariablesIds(inputVariablesIds);
    }

    return eOk;
  }

  if (valueIndex >= nEvalVarsStart)
  {
    inputVariablesIds.resize(valueIndex + 1);
    pValueParam->setInputVariablesIds(inputVariablesIds);
  }

  inputVariablesIds = pValueParam->inputVariablesIds();
  int nEvalVars = inputVariablesIds.length();

  OdDbEvalVariantPtr pLast = pValueParam->value();

  for (int i = 0; i < nEvalVars; ++i)
  {
    if (i < nEvalVarsStart - 1)
    {
      pLast = pLast->next();
    }
    
    if (i >= nEvalVarsStart)
    {
      OdDbEvalVariantPtr pNewEvVar = OdDbEvalVariant::createObject();
      pLast->setNext(pNewEvVar);
      pLast = pNewEvVar;
    }
  }

  OdDbEvalVariantPtr pEvalVar = pValueParam->value();

  int counter = 0;
  for (int i = 0; i < nEvalVars; ++i)
  {
    if (valueIndex == counter)
    {
      break;
    }

    pEvalVar = pEvalVar->next();
    counter++;
  }

  *pEvalVar = value;
  return eOk;
}

OdValue::UnitType OdDbImpAssocAction::valueParamUnitType(const OdString& paramName) const
{
  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                                   m_arrValueParams.end(), 
                                                                   findValueParamByName(paramName) );

  if ( m_arrValueParams.end() == it )
  {
    return OdValue::kUnitless;
  }
  else
  {
    return (*it)->unitType();
  }
}

OdResult OdDbImpAssocAction::setValueParamUnitType(const OdString& paramName, 
                                                   OdValue::UnitType unitType)
{
  OdArray<OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                             m_arrValueParams.end(), 
                                                             findValueParamByName(paramName) );

  if ( m_arrValueParams.end() == it )
  {
    return eNotInGroup;
  }

  (*it)->setUnitType (unitType);
  return eOk;
}

struct ValueParamInfo {
  OdDbObjectId varId;
  OdDbObjectId depId;
};

OdResult OdDbImpAssocAction::removeValueParam(const OdString& paramName)
{
  OdArray<OdDbImpValueParamPtr>::iterator it = std::find_if(m_arrValueParams.begin(),
                                                            m_arrValueParams.end(),
                                                            findValueParamByName(paramName));
  if (it == m_arrValueParams.end() || it->isNull())
    return eInvalidInput;

  OdDbImpValueParamPtr pParam = *it;

  OdDbObjectId commonNetId;

  OdArray<ValueParamInfo> aParamsToRemove;

  OdDbObjectIdArray aInputDepIds = pParam->inputVariablesIds();
  for (unsigned int i = 0; i < aInputDepIds.size(); ++i)
  {
    const OdDbObjectId depId = aInputDepIds[i];
    OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(depId.openObject(OdDb::kForWrite));
    if (!pDep.isNull())
    {
      OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(pDep->dependentOnObject().openObject());
      if (!pVar.isNull() && pVar->isAnonymous())
      {
        const OdDbObjectId varNetId = pVar->owningNetwork();
        if (commonNetId.isNull())
          commonNetId = varNetId;

        ODA_ASSERT(varNetId == commonNetId);

        ValueParamInfo info;
        info.depId = pDep->objectId();
        info.varId = pVar->objectId();
        aParamsToRemove.push_back(info);
      }
    }
  }

  OdDbAssocNetworkPtr pCommonNet = OdDbAssocNetwork::cast(commonNetId.openObject(OdDb::kForWrite));
  if (!pCommonNet.isNull())
  {
    for (unsigned int i = 0; i < aParamsToRemove.size(); ++i)
    {
      removeDependency(aParamsToRemove[i].depId, true);
      pCommonNet->removeAction(aParamsToRemove[i].varId, true);
    }
  }

  pParam = NULL;
  m_arrValueParams.erase(it);
  return eOk;
}

OdResult OdDbImpAssocAction::valueParamInputVariables(const OdString& paramName,
                                                      OdDbObjectIdArray& variableIds) const
{
  OdArray<const OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                                   m_arrValueParams.end(), 
                                                                   findValueParamByName(paramName) );
  if ( m_arrValueParams.end() == it )
  {
    return eInvalidInput;
  }
  else
  {
    variableIds = (*it)->inputVariablesIds();
  }
  return eOk;
}

OdResult OdDbImpAssocAction::setValueParamControlledObjectDep(const OdString& paramName, 
                                                              const OdDbObjectId& controlledObjectDepId)
{
  OdArray<OdDbImpValueParamPtr>::iterator it = std::find_if( m_arrValueParams.begin(), 
                                                                   m_arrValueParams.end(), 
                                                                   findValueParamByName(paramName) );

  if ( m_arrValueParams.end() == it )
  {
    return eInvalidInput;
  }
  else
  {
    (*it)->setControlledObjectDepId(controlledObjectDepId);
  }
  return eOk;
}

OdResult OdDbImpAssocAction::updateValueParamControlledObject(const OdString& paramName) const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::updateValueParamFromControlledObject(const OdString& paramName)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::updateAllObjectsControlledByValueParams() const
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::transformAllConstantGeometryParams(const OdGeMatrix3d& transform)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::scaleAllDistanceValueParams(double scaleFactor)
{
  ODA_FAIL_ONCE(); // TODO
  return eNotImplementedYet;
}

OdResult OdDbImpAssocAction::updateValueParams(OdDbAssocAction* pAction)
{
  bool assertWriteEnabledCalled = false;
  for (unsigned int i = 0; i < m_arrValueParams.size(); i++)
  {
    OdDbImpValueParamPtr pValueParam = m_arrValueParams[i];
    OdDbObjectIdArray aDeps = pValueParam->inputVariablesIds();
    OdDbEvalVariantPtr pEvalVar = pValueParam->value();
    for (unsigned int j = 0; j < aDeps.size(); j++)
    {
      if (!assertWriteEnabledCalled)
      {
        pAction->assertWriteEnabled();
        assertWriteEnabledCalled = true;
      }

      ODA_ASSERT(!pEvalVar.isNull());
      OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(aDeps[j].openObject(OdDb::kForWrite));
      if (!pDep.isNull())
      {
        pDep->getDependentOnObjectValue(pEvalVar);
        if (pValueParam->unitType() == OdValue::kAngle) {
          double degreeAngle = 0.;
          if (pEvalVar->getValue(degreeAngle) != eOk) {
            int degreeAngleInt = 0;
            pEvalVar->getValue(degreeAngleInt);
            degreeAngle = degreeAngleInt;
          }
          pEvalVar->setValue(OdResBuf::kDxfReal, OdaToRadian(degreeAngle));
        }
        pDep->evaluate();
      }
      pEvalVar = pEvalVar->next();
    }
  }
  return eOk;
}

class addDependentObject {
  OdDbObjectIdArray& m_objects;
public:
  addDependentObject(OdDbObjectIdArray& objects) : m_objects(objects) {}
  void operator()(const OdDbObjectId& depId) {
    if (depId.objectClass()->isDerivedFrom(OdDbAssocDependency::desc())) {
      OdDbObjectPtr obj = depId.openObject();
      m_objects.append(static_cast<const OdDbAssocDependency*>(obj.get())->dependentOnObject());
    }
  }
};

void OdDbImpAssocAction::addValueParamsToDeepClone(const OdDbAssocAction*,
                                                   OdDbObjectIdArray& additionalObjectsToClone) const {
  for (unsigned int i = 0; i < m_arrValueParams.size(); i++)
  {
    OdDbImpValueParamPtr pValueParam = m_arrValueParams[i];
    OdDbObjectIdArray aDeps = pValueParam->inputVariablesIds();
    std::for_each(aDeps.begin(), aDeps.end(), addDependentObject(additionalObjectsToClone));
  }
}

//#endif

unsigned int removeAll(const OdDbObjectId& id, OdDbObjectIdArray &from, unsigned int start = 0);

inline unsigned int removeNulls(OdDbObjectIdArray &ids) {
  return removeAll(OdDbObjectId::kNull, ids);
}

void reportAuditError(OdDbAuditInfo* ctx, OdDbObject* obj,
  OdUInt32 strIdValueName, OdUInt32 strIdValidation, OdUInt32 strIdRepair,
  OdUInt32 errFound = 1, OdUInt32 errFixed = 0)
{
  ODA_ASSERT(ctx && obj);
  if (ctx && obj) {
    OdDbHostAppServices* app = obj->database()->appServices();
    ctx->printError(obj,
      app->formatMessage(strIdValueName),
      app->formatMessage(strIdValidation),
      app->formatMessage(strIdRepair)
    );
    ctx->errorsFound(errFound);
    if (errFixed)
      ctx->errorsFixed(errFixed);
  }
}

void OdDbImpAssocAction::audit(OdDbObject* pObj, OdDbAuditInfo* pAuditInfo)
{
  OdDbAssocActionPtr pAction = pObj;
  OdDbAssocNetworkPtr pOwningNet = pAction->owningNetwork().openObject();
  bool bFixErrors = pAuditInfo->fixErrors();
  if(pOwningNet.get())
  {
    if ( !isEvaluationRequest(pAction->status())
        && pOwningNet->getActionsToEvaluate().contains(pAction->objectId()))
    {
      if (bFixErrors)
        pAction->setStatus(kChangedDirectlyAssocStatus, false);

      reportAuditError( pAuditInfo, pObj,
        sidActionStatus, sidVarValidInvalid, sidkChangedDirectly,
        1, bFixErrors ? 1 : 0
      );
    }
  }

  {
    int errors = 0;
    if (bFixErrors)
      errors = removeNulls(m_arrParams);
    else if (m_arrParams.contains(OdDbObjectId::kNull))
      ++errors;

    if (errors)
      reportAuditError( pAuditInfo, pObj,
        sidkParameterArray, sidkZeroIdsFound, sidVarDefRemove,
        1, bFixErrors ? 1 : 0
      );
  }

  if (pObj->isA() == OdDbAssocAction::desc())
  { // Pure OdDbAssocAction must have body
    OdDbObjectPtr pActionBody = actionBody().openObject();
    if (pActionBody.isNull()
      || (!pActionBody->isKindOf(OdDbAssocActionBody::desc()) && pActionBody->isA() != OdDbProxyObject::desc()))
    {
      reportAuditError( pAuditInfo, pObj,
        sidAssocActionBodyIsInvalid, sidVarValidInvalid, sidVarDefRemove );

      if (bFixErrors)
      {
        ((OdDbAssocAction*)pObj)->setOwningNetwork(OdDbObjectId::kNull, false); // suppress removing from this owning network in subErase()
        pObj->erase();
        pAuditInfo->errorsFixed(1);
      }
    }
  }
}

void OdDbImpAssocAction::appendToOwner(OdDbAssocAction* pThisAction,
                                       OdDbIdPair& idPair,
                                       OdDbObject* owner,
                                       OdDbIdMapping& idMap)
{
  ODA_ASSERT(!idPair.isOwnerXlated());
  OdDbAssocNetworkPtr pAssocNetwork = OdDbAssocNetwork::cast(owner);
  if (pAssocNetwork.isNull() && owner->isKindOf(OdDbBlockTableRecord::desc()))
    pAssocNetwork = OdDbAssocNetwork::getInstanceFromObject(owner->objectId(), true)
    .safeOpenObject(OdDb::kForWrite);

  if (pAssocNetwork.get()) {
    pAssocNetwork->addAction(pThisAction->objectId(), true);
    idPair.setOwnerXlated(true);
    idMap.assign(idPair);

    OdDbObjectPtr original = idPair.key().openObject();
    if (original.get()) {
      OdDbIdPair ownerMapping(original->ownerId());
      if (idMap.compute(ownerMapping) && !ownerMapping.isCloned()) {
        ownerMapping.setValue(pAssocNetwork->objectId());
        idMap.assign(ownerMapping);
      }
    }
  }
  else {
    pThisAction->OdDbObject::appendToOwner(idPair, owner, idMap);
  }
}
