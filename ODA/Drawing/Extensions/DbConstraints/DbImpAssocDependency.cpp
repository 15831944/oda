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
#include "DbImpAssocDependency.h"
#include "DbAssocDependency.h"
#include "DbFiler.h"
#include "DbAssocDependencyBody.h"
#include "DbAssocNetwork.h"
#include "DbImpAssocAction.h"
#include "OdDbAssocDependencyPE.h"
#include "DbAssoc2dConstraintGroup.h"
#include "DbCompoundObjectId.h"
//#include "DbCompoundObjectIdImpl.h"
#include <typeinfo>
////////////////////////////////////////////////////////////////////

OdDbImpAssocDependency::OdDbImpAssocDependency():
    m_nStatus(kIsUpToDateAssocStatus)
    ,m_nOrder(0)
    ,m_nIndex(0)
    ,m_bReadDep(true)
    ,m_bWriteDep(true)
    ,m_bObjectStateDependent(true)
    ,m_bDelegatingToOwningAction(false)
    ,m_refPath()
{
  m_prevDependencyOnObjectID = OdDbObjectId::kNull;
  m_nextDependencyOnObjectID = OdDbObjectId::kNull;
}

OdDbImpAssocDependency::~OdDbImpAssocDependency()
{
}

OdResult OdDbImpAssocDependency::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = eOk;
  OdUInt16 ver = pFiler->rdInt16();
  if (ver != 2)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  m_nStatus = (OdDbAssocStatus)pFiler->rdInt32();

  m_bReadDep = pFiler->rdBool();
  m_bWriteDep = pFiler->rdBool();
  m_bObjectStateDependent = pFiler->rdBool();
  m_bDelegatingToOwningAction = pFiler->rdBool();

  m_nOrder = pFiler->rdInt32();
  m_dependentOnObjectId = pFiler->rdSoftPointerId();

  res = m_refPath.dwgInFields(pFiler, 2);
  if ( res != eOk )
    return res;

  if(pFiler->filerType()==OdDbFiler::kFileFiler && m_dependentOnObjectId.isErased() && m_refPath.isEmpty()) {
    m_nStatus = kErasedAssocStatus;
  }
  m_prevDependencyOnObjectID = pFiler->rdSoftPointerId();
  m_nextDependencyOnObjectID = pFiler->rdSoftPointerId();
  m_dependencyBodyId = pFiler->rdHardOwnershipId();
  m_nIndex = pFiler->rdInt32();
  return res;
}

void OdDbImpAssocDependency::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  pFiler->wrInt16(2);   // Version?
  pFiler->wrInt32(m_nStatus);

  pFiler->wrBool(m_bReadDep);
  pFiler->wrBool(m_bWriteDep);
  pFiler->wrBool(m_bObjectStateDependent);
  pFiler->wrBool(m_bDelegatingToOwningAction);

  pFiler->wrInt32(m_nOrder);
  pFiler->wrSoftPointerId(m_dependentOnObjectId);
  m_refPath.dwgOutFields(pFiler, 0);
  pFiler->wrSoftPointerId(m_prevDependencyOnObjectID);
  pFiler->wrSoftPointerId(m_nextDependencyOnObjectID);
  pFiler->wrHardOwnershipId(m_dependencyBodyId);
  pFiler->wrInt32(m_nIndex);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdDbImpAssocDependency::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = eOk;

  // Check that we are at the correct subclass data
  if( !pFiler->atSubclassData(OdDbAssocDependency::desc()->name() ))
  {
    return eBadDxfSequence;
  }
  
  // Read version number (must be first)
  if( pFiler->nextItem() != 90 || pFiler->rdInt32() != 2)
    return eMakeMeProxy;

  NEXT_CODE(90)
  m_nStatus = (OdDbAssocStatus)pFiler->rdInt32();

  NEXT_CODE(290)
  m_bReadDep = pFiler->rdBool();
  NEXT_CODE(290)
  m_bWriteDep = pFiler->rdBool();
  NEXT_CODE(290)
  m_bObjectStateDependent = pFiler->rdBool();
  NEXT_CODE(290)
  m_bDelegatingToOwningAction = pFiler->rdBool();

  NEXT_CODE(90)
  m_nOrder = pFiler->rdInt32();
  NEXT_CODE(330)
  m_dependentOnObjectId = pFiler->rdObjectId();

  NEXT_CODE(290)
  res = m_refPath.dxfInFields(pFiler, 0, 0);
  if ( res != eOk )
    return res;

  if(pFiler->filerType()==OdDbFiler::kFileFiler && m_dependentOnObjectId.isErased() && m_refPath.isEmpty()) {
    m_nStatus = kErasedAssocStatus;
  }
  NEXT_CODE(330)
  m_prevDependencyOnObjectID = pFiler->rdObjectId();
  NEXT_CODE(330)
  m_nextDependencyOnObjectID = pFiler->rdObjectId();
  NEXT_CODE(360)
  m_dependencyBodyId = pFiler->rdObjectId();
  NEXT_CODE(90)
  m_nIndex = pFiler->rdInt32();
  return res;
}
#undef NEXT_CODE


void OdDbImpAssocDependency::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  pFiler->wrSubclassMarker(OdDbAssocDependency::desc()->name());
  pFiler->wrInt32(90, 2);   // Version?
  pFiler->wrInt32(90, m_nStatus);

  pFiler->wrBool(290, m_bReadDep);
  pFiler->wrBool(290, m_bWriteDep);
  pFiler->wrBool(290, m_bObjectStateDependent);
  pFiler->wrBool(290, m_bDelegatingToOwningAction);

  pFiler->wrInt32(90, m_nOrder);
  pFiler->wrObjectId(330, m_dependentOnObjectId);

  m_refPath.dxfOutFields(pFiler, 0);

  pFiler->wrObjectId(330, m_prevDependencyOnObjectID);
  pFiler->wrObjectId(330, m_nextDependencyOnObjectID);
  pFiler->wrObjectId(360, m_dependencyBodyId);
  pFiler->wrInt32(90, m_nIndex);
}

OdDbObjectId OdDbImpAssocDependency::dependencyBody() const
{
  return m_dependencyBodyId;
}
    
OdResult OdDbImpAssocDependency::setDependencyBody(OdDbObjectId dependencyBodyId, OdDbObjectId dependencyId)
{
  m_dependencyBodyId = dependencyBodyId;
  OdDbObjectPtr pObj = m_dependencyBodyId.openObject(OdDb::kForWrite);
  pObj->setOwnerId(dependencyId);
  return eOk;
}

OdDbAssocStatus OdDbImpAssocDependency::status() const
{
  return (OdDbAssocStatus)m_nStatus;
}

inline void flushAllTransactedChanges(const OdDbObject* obj) {
  OdDbDatabase* db = obj->database();
  if (db)
    db->flushAllTransactedChanges();
}

OdResult OdDbImpAssocDependency::setStatus(OdDbAssocDependency* pDep, OdDbAssocStatus newStatus, bool notifyOwningAction)
{
  OdDbAssocStatus oldStatus = status();
  if (newStatus == oldStatus)
    return eOk;

  if ( newStatus == kIsUpToDateAssocStatus )
    ::flushAllTransactedChanges(pDep->database());
  ODA_ASSERT(oldStatus == status()); // side effect: status was changed

  if ( isEvaluationRequest(newStatus) )
  {
    if ( !isToBeSkipped(oldStatus) )
    {
      if ( isEvaluationRequest(oldStatus) )
      {
        if ( evaluationRequestSeverityLevel(newStatus) > evaluationRequestSeverityLevel(oldStatus) )
          writeStatus(pDep, newStatus);
      }
      else
      {
        writeStatus(pDep, newStatus);
      }
    }
  }
  else
  {
    writeStatus(pDep, newStatus);
  }
  if (notifyOwningAction && oldStatus!=status()) {
    OdDbAssocActionPtr action = OdDbAssocAction::cast(pDep->owningAction().openObject(OdDb::kForWrite));
    if (action.get())
      action->ownedDependencyStatusChanged(pDep, oldStatus);
  }
  return eOk;
}

inline void OdDbImpAssocDependency::writeStatus(OdDbAssocDependency* pDep, OdDbAssocStatus newStatus) {
  pDep->assertWriteEnabled();
  m_nStatus = newStatus;
}

bool OdDbImpAssocDependency::isReadDependency() const
{
  return m_bReadDep;
}

bool OdDbImpAssocDependency::isWriteDependency() const
{
  return m_bWriteDep;
}

void OdDbImpAssocDependency::setIsReadDependency(bool bRead)
{
  m_bReadDep = bRead;
}

void OdDbImpAssocDependency::setIsWriteDependency(bool bWrite)
{
  m_bWriteDep = bWrite;
}

bool OdDbImpAssocDependency::isObjectStateDependent() const
{
  return m_bObjectStateDependent;
}

void OdDbImpAssocDependency::setIsObjectStateDependent(bool bDependent)
{
  m_bObjectStateDependent = bDependent;
}

int OdDbImpAssocDependency::order() const
{
  return m_nOrder;
}

void OdDbImpAssocDependency::setOrder(int newOrder)
{
  m_nOrder = (OdInt32)newOrder;
}


OdResult OdDbImpAssocDependency::setOwningAction(OdDbObjectId actionId, OdDbAssocDependency *pDep)
{
  pDep->setOwnerId(actionId);
  if ( actionId.isNull() )
    return eOk;

  OdDbAssocActionPtr pAction = actionId.openObject();
  OdDbImpAssocAction *pImpActtion = OdDbImpAssocAction::getImpObject(pAction);
  
  m_nIndex = pImpActtion->getMaximumAssocDepIndex() + 1;
  pImpActtion->setMaximumAssocDepIndex(m_nIndex);

  return eOk;
}


OdDbObjectId OdDbImpAssocDependency::dependentOnObject() const
{
  return m_dependentOnObjectId;
}

bool OdDbImpAssocDependency::isDependentOnCompoundObject() const
{
  return !m_refPath.isEmpty();
}

OdDbObjectId OdDbImpAssocDependency::prevDependencyOnObject() const
{
  return m_prevDependencyOnObjectID;
}

 
OdDbObjectId OdDbImpAssocDependency::nextDependencyOnObject() const
{
  return m_nextDependencyOnObjectID;
}

OdResult OdDbImpAssocDependency::attachToObject(const OdDbCompoundObjectId& compoundId, OdDbAssocDependency *pDep)
{
  ODA_ASSERT(pDep != NULL);

  if (isAttachedToObject() || !isReadyToAttach())
    return eBadObjType;

  OdDbObjectIdArray arrObjId;
  if (compoundId.getFullPath(arrObjId) != eOk)
    return eBadObjType;

  //OdDbObjectPtr pObj = pCompoundId->leafId().openObject(); // AlexeyTyurin 11062017 : CORE-12281 If the length of path of pCompoundId > 1
  // the top usually is OdDbObjectRef () but the leaf usually is Entity (circle, line, etc.) that
  // hasn't dependencies
  OdDbObjectPtr pObj = compoundId.topId().openObject();
  ODA_ASSERT(!pObj.isNull());

  if (pObj.isNull())
    return eInvalidObjectId;

  OdSmartPtr<OdDbAssocDependencyPE> pPE = pObj->isA()->getX(OdDbAssocDependencyPE::desc());
  if (!pPE.isNull() && !pPE->allowsDependencies(pObj, isNeedWriteDependency()))
  {
    return eVetoed; // according to ObjectARX man
  }

  if (arrObjId.length() > 1)
    m_refPath.set(compoundId, pObj->database());

  const OdDbObjectId depId = pDep->objectId();
  OdDbObjectId lastDepId = OdDbAssocDependency::getFirstDependencyOnObject(pObj);
  OdDbAssocDependencyPtr pLastDep = OdDbAssocDependency::cast(lastDepId.openObject());

  for (unsigned int i = 0; i < arrObjId.length(); i++)
  {
    OdDbObjectPtr pObjTemp = arrObjId[i].openObject(OdDb::kForWrite);
    pObjTemp->addPersistentReactor(depId);
  }

  for (; !pLastDep.isNull() && !pLastDep->nextDependencyOnObject().isNull(); )
  {
    lastDepId = pLastDep->nextDependencyOnObject();
    pLastDep = OdDbAssocDependency::cast(lastDepId.openObject());
  }

  if (!pLastDep.isNull())
  {
    pLastDep->upgradeOpen();
    pLastDep->assertWriteEnabled();

    if (lastDepId != depId)
      pLastDep->m_pImpObj->m_nextDependencyOnObjectID = depId;
  }

  if (depId != lastDepId)
    m_prevDependencyOnObjectID = lastDepId;

  m_dependentOnObjectId = pObj->objectId();

  return eOk;
}

void OdDbImpAssocDependency::relinkDependenciesOn(OdDbObject* pObj)
{
  if (!pObj)
    return;
  const OdDbObjectIdArray aReactors = pObj->getPersistentReactors();
  OdDbObjectIdArray aDeps;
  aDeps.push_back(OdDbObjectId());
  int i = aReactors.size();
  while (i-- > 0)
  {
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(aReactors[i].openObject());
    if (!pDep.isNull())
    {
      if (pDep->dependentOnObject() != pObj->objectId())
      {
        pObj->upgradeOpen();
        pObj->removePersistentReactor(aReactors[i]);
      }
      else
        aDeps.push_back(aReactors[i]);
    }
  }
  aDeps.push_back(OdDbObjectId());
  i = aDeps.size();
  while (i-- > 0)
  {
    OdDbAssocDependencyPtr pDep = OdDbAssocDependency::cast(aDeps[i].openObject());
    if (!pDep.isNull())
    {
      if (pDep->m_pImpObj->m_prevDependencyOnObjectID != aDeps[i - 1] ||
        pDep->m_pImpObj->m_nextDependencyOnObjectID != aDeps[i + 1])
      {
        pDep->upgradeOpen();
        pDep->assertWriteEnabled();
        pDep->m_pImpObj->m_prevDependencyOnObjectID = aDeps[i - 1];
        pDep->m_pImpObj->m_nextDependencyOnObjectID = aDeps[i + 1];
      }
    }
  }
}

OdResult OdDbImpAssocDependency::dependentOnObjectStatus() const
{
  if ( isAttachedToObject() )
    return eOk;
  else
    return eBadObjType;
}

OdResult OdDbImpAssocDependency::detachFromObject(OdDbAssocDependency *pDep)
{
  if ( !isAttachedToObject() )
    return eBadObjType;

  OdDbObjectIdArray arrObjId;
  if ( m_refPath.getFullPath(arrObjId) != eOk )
    return eBadObjType;

  OdDbObjectPtr pObj;
  OdDbObjectId depId = pDep->objectId();
  for ( unsigned int i = 0; i < arrObjId.length(); i++ )
  {
    pObj = arrObjId[i].openObject(OdDb::kForWrite);
    if (!pObj.isNull())
      pObj->removePersistentReactor(depId);
  }

  pObj = m_dependentOnObjectId.openObject(OdDb::kForWrite);
  if (!pObj.isNull())
    pObj->removePersistentReactor(depId);
  if (!m_prevDependencyOnObjectID.isNull()) {
    OdDbAssocDependencyPtr pPrevDep = m_prevDependencyOnObjectID.openObject(OdDb::kForWrite);
    if (!pPrevDep.isNull()) {
      pPrevDep->assertWriteEnabled();
      pPrevDep->m_pImpObj->m_nextDependencyOnObjectID = m_nextDependencyOnObjectID;
    }
  }
  if (!m_nextDependencyOnObjectID.isNull()) {
    OdDbAssocDependencyPtr pNextDep = m_nextDependencyOnObjectID.openObject(OdDb::kForWrite);
    if (!pNextDep.isNull()) {
      pNextDep->assertWriteEnabled();
      pNextDep->m_pImpObj->m_prevDependencyOnObjectID = m_prevDependencyOnObjectID;
    }
  }
  m_prevDependencyOnObjectID.setNull();
  m_nextDependencyOnObjectID.setNull();

  m_dependentOnObjectId = OdDbObjectId::kNull;

  return eOk;
}

OdResult OdDbImpAssocDependency::checkGetBody(OdDbAssocDependencyBodyPtr& outBody, OdDb::OpenMode mode) const {
  OdDbObjectPtr bodyObj = dependencyBody().openObject(mode);
  if (bodyObj.isNull())
    return eNotImplemented;
  if (bodyObj->isAProxy() || !bodyObj->isKindOf(OdDbAssocDependencyBody::desc()))
    return eNotThatKindOfClass;
  outBody = (OdDbAssocDependencyBody*)bodyObj.get();
  return eOk;
}

OdResult OdDbImpAssocDependency::updateDependentOnObject(OdDbAssocDependency *pDep)
{
  return eOk;
}

void OdDbImpAssocDependency::setDependentOnObject(const OdDbCompoundObjectId& compoundId, OdDbAssocDependency *pDep)
{
  m_refPath.set(compoundId, pDep->database());
  m_dependentOnObjectId = m_refPath.leafId();
}

bool OdDbImpAssocDependency::isDelegatingToOwningAction() const
{
  return m_bDelegatingToOwningAction;
}


void OdDbImpAssocDependency::setIsDelegatingToOwningAction(bool bDelegating)
{
  m_bDelegatingToOwningAction = bDelegating;
}

bool OdDbImpAssocDependency::hasCachedValue() const { return false; }

bool OdDbImpAssocDependency::isRelevantChange(const OdDbAssocDependency *pDep) const
{
  return true;
}
/*
    bool OdDbImpAssocDependency::isDependentOnTheSameThingAs(const OdDbAssocDependency* pOtherDependency) const;

    bool OdDbImpAssocDependency::isDependentOnObjectReadOnly() const;

    bool OdDbImpAssocDependency::isEqualTo(const OdDbAssocDependency* pOtherDependency) const;
*/
bool OdDbImpAssocDependency::isActionEvaluationInProgress( const OdDbAssocDependency *pDependency ) const
{
  OdDbObjectId idAction = pDependency->owningAction();
  OdDbAssocActionPtr pAction = OdDbAssocAction::cast(idAction.openObject());
  if (pAction.isNull())
   return false;

 return pAction->isActionEvaluationInProgress();
}

OdDbAssocEvaluationCallback* OdDbImpAssocDependency::currentEvaluationCallback() const
{
  ODA_FAIL_ONCE(); // TODO : Implement me!
  return NULL;
}

void OdDbImpAssocDependency::evaluate(OdDbAssocDependency* thisDependency) {
  thisDependency->setStatus(kIsUpToDateAssocStatus);
}


void OdDbImpAssocDependency::erased(const OdDbObject* dbObj, OdDbAssocDependency *pDep, bool isErasing)
{
  pDep->setStatus(isErasing ? kErasedAssocStatus : kChangedDirectlyAssocStatus);
}

void OdDbImpAssocDependency::modified(const OdDbObject* sender, OdDbAssocDependency *self)
{
  if (status() != kErasedAssocStatus && isThisDependencyOn(sender))
    setStatus(self, kChangedDirectlyAssocStatus, true);
}

bool OdDbImpAssocDependency::isThisDependencyOn(const OdDbObject* object) {
  if (!object)
    return false;
  OdDbObjectId objectId = object->objectId();
  if (isDependentOnCompoundObject()) {
    OdDbObjectIdArray ids;
    return (eOk == m_refPath.getFullPath(ids) && ids.contains(objectId));
  }
  //else
  return objectId == dependentOnObject();
}

void OdDbImpAssocDependency::copied(const OdDbObject* pDbObj, const OdDbObject* pNewObj, OdDbAssocDependency *pDep)
{
  OdDbAssocActionPtr action = pDep->owningAction().openObject(OdDb::kForWrite);
  if (!action.isNull())
    action->dependentObjectCloned(pDep, pDbObj, pNewObj);
}

OdResult OdDbImpAssocDependency::getDependentOnCompoundObject(OdDbCompoundObjectId& res) const
{
  if ( isDependentOnCompoundObject() )
    res = m_refPath;
  else
    res.set(dependentOnObject());

  return eOk;
}

OdStringArray OdDbImpAssocDependency::compareWith(const OdDbImpAssocDependency *depIDToCompare, OdDb::DwgVersion filerVersion) const
{ 
	OdStringArray retArr;

	if (m_nStatus != depIDToCompare->m_nStatus)
		retArr.append(CS_UNDEFINED_ANNOTATION_ACTIONBODY);

	if (m_bReadDep != depIDToCompare->m_bReadDep)
		retArr.append(CS_READ_DEP);

	if (m_bWriteDep != depIDToCompare->m_bWriteDep)
		retArr.append(CS_WRITE_DEP);

	if (m_bObjectStateDependent != depIDToCompare->m_bObjectStateDependent)
		retArr.append(CS_OBJECT_STATE_DEPENDENT);

	if (m_bDelegatingToOwningAction != depIDToCompare->m_bDelegatingToOwningAction)
		retArr.append(CS_DELEGATING_TO_OWNING_ACTION);

	if (m_nOrder != depIDToCompare->m_nOrder)
		retArr.append(CS_ORDER);

	if ((!m_dependentOnObjectId.isNull() && depIDToCompare->m_dependentOnObjectId.isNull()) ||
		(m_dependentOnObjectId.isNull() && !depIDToCompare->m_dependentOnObjectId.isNull()))
	{
		retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (!m_dependentOnObjectId.isNull() && !depIDToCompare->m_dependentOnObjectId.isNull())
	{
		if (typeid(*(m_dependentOnObjectId).openObject()) != typeid(*(depIDToCompare->m_dependentOnObjectId.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}
	
	//m_refPath.dxfOutFields(pFiler, 0);
	if ((!m_prevDependencyOnObjectID.isNull() && depIDToCompare->m_prevDependencyOnObjectID.isNull()) ||
		(m_prevDependencyOnObjectID.isNull() && !depIDToCompare->m_prevDependencyOnObjectID.isNull()))
	{
		retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (!m_prevDependencyOnObjectID.isNull() && !depIDToCompare->m_prevDependencyOnObjectID.isNull())
	{
		if (typeid(*(m_prevDependencyOnObjectID).openObject()) != typeid(*(depIDToCompare->m_prevDependencyOnObjectID.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if ((!m_nextDependencyOnObjectID.isNull() && depIDToCompare->m_nextDependencyOnObjectID.isNull()) ||
		 (m_nextDependencyOnObjectID.isNull() && !depIDToCompare->m_nextDependencyOnObjectID.isNull()))
	{
		retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (!m_nextDependencyOnObjectID.isNull() && !depIDToCompare->m_nextDependencyOnObjectID.isNull())
	{
		if (typeid(*(m_nextDependencyOnObjectID).openObject()) != typeid(*(depIDToCompare->m_nextDependencyOnObjectID.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}
	
	if ((!m_dependencyBodyId.isNull() && !depIDToCompare->m_dependencyBodyId.isNull()) ||
		(!m_dependencyBodyId.isNull() && !depIDToCompare->m_dependencyBodyId.isNull()))
	{
		retArr.append(CS_DIFFERENT_OBJECTS);
	}
	if (!m_dependencyBodyId.isNull() && !depIDToCompare->m_dependencyBodyId.isNull())
	{
		if (typeid(*(m_dependencyBodyId).openObject()) != typeid(*(depIDToCompare->m_dependencyBodyId.openObject())))
			retArr.append(CS_DIFFERENT_OBJECTS);
	}

	if (m_nIndex != depIDToCompare->m_nIndex)
		retArr.append(CS_INDEX);

	return retArr;
}
