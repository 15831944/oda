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
#include "DbImpAssocVariable.h"
#include "DbAssocNetwork.h"
#include "DbCompoundObjectId.h"
#include "DbAssocValueDependency.h"
#include "OdExpression.h"

#include "DbFiler.h"
#include "DbImpAssocValueDependency.h"
#include "DbIdMapping.h"
#include "DbAssocNetCloneCtx.h"
#include "DebugStuff.h"
#include "DbHostAppServices.h"
#include "DbDictionaryVar.h"

OdDbAssocVariableCallback* OdDbImpAssocVariable::m_assocVariableCallback = NULL;

OdDbImpAssocVariable::OdDbImpAssocVariable():OdDbImpAssocAction(),
                                        m_varName(),
                                        m_expression(),
                                        m_description(),
                                        m_evaluatorId(L"AcDbCalc:1.0"),
                                        m_isMergeable(false),
                                        m_mustMerge(false),
                                        m_mergeableVariableName(),
                                        m_arrValueDependenciesIdx()
{
  m_isBase = false;
  m_value = OdDbEvalVariant::init(1.0);
}

OdDbImpAssocVariable::~OdDbImpAssocVariable()
{
}

const OdString OdDbImpAssocVariable::name() const
{
  return m_varName;
}

const OdString OdDbImpAssocVariable::expression() const
{
  return m_expression;
}

const OdDbEvalVariantPtr OdDbImpAssocVariable::value() const
{
  return m_value;
}

const OdString OdDbImpAssocVariable::description() const
{
  return m_description;
}

void OdDbImpAssocVariable::evaluate(OdDbAssocAction* pAction, OdDbAssocEvaluationCallback*)
{
  if (!pAction || pAction->isA() != OdDbAssocVariable::desc()) {
    setStatus(pAction, kFailedToEvaluateAssocStatus);
    return;
  }

  if (m_expression.isEmpty()) {
    setStatus(pAction, kIsUpToDateAssocStatus);
    return;
  }

  bool failedToUpdateCache = false;
  bool hasRelevantChange = (status()==kChangedDirectlyAssocStatus);

  OdDbObjectIdArray depCandidatesToRemove = m_arrDependencies;
  for (unsigned int i = 0; i < depCandidatesToRemove.size(); i++) {
    OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(depCandidatesToRemove[i].openObject());
    if (!pDep.isNull() && pDep->status() == kErasedAssocStatus) {
      // TODO: ideally we should use m_arrValueDependenciesIdx to lookup erased
      //       variable name here, but that member is never looked up in the code
      //       except in dwgIn/dwgOut, so it is better to not take the risk at this point
      const OdDbObjectId dependentVarId = pDep->dependentOnObject();
      OdDbAssocVariablePtr pErasedVar = OdDbAssocVariable::cast(dependentVarId.openObject(OdDb::kForRead, true));
      if (!pErasedVar.isNull()) {
        OdDbEvalVariantPtr pCachedValue = OdDbEvalVariant::createObject();
        pDep->getDependentOnObjectValue(pCachedValue);

        fixVariable(pErasedVar->name(), pCachedValue);
        removeDependency(depCandidatesToRemove[i], true);
      }
      else {
        failedToUpdateCache = true;
        break;
      }
    }
    else if(!hasRelevantChange) {
      hasRelevantChange = pDep->isRelevantChange();
    }
  }

  if (failedToUpdateCache) {
    OdDbAssocVariable* pThisVariable = static_cast<OdDbAssocVariable*>(pAction);
    pThisVariable->removeAllDependencies(true);
    pThisVariable->setExpression(OdExpression::createExpression(pThisVariable->value()), OdString::kEmpty, false, false);
  }
  else if(!hasRelevantChange) {
    setStatus(pAction, kIsUpToDateAssocStatus);
    return;
  }

  OdExpression::Parser p;
  OdDbEvalVariantPtr value = OdDbEvalVariant::createObject();
  for (unsigned int i = 0; i < m_arrDependencies.size(); i++) {
    OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(m_arrDependencies[i].openObject(OdDb::kForWrite));
    if (!pDep.isNull()) {
      pDep->evaluate();
      const OdDbObjectId dependentVarId = pDep->dependentOnObject();
      OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(dependentVarId.openObject());
      ODA_ASSERT(!pVar.isNull());
      if (!pVar.isNull()) {
        pDep->getDependentOnObjectValue(value);
        p.addVariable(pVar->name(), value);
      }
    }
  }

  if (p.setExpression(m_expression) != eOk)
    return;

  value = p.evaluate();
  if (!value.isNull()) {
    setStatus(pAction, kIsUpToDateAssocStatus);
    m_value = value;
  }
  else {
    setStatus(pAction, kFailedToEvaluateAssocStatus);
  }
}

void OdDbImpAssocVariable::evaluateDependency(OdDbAssocDependency* dependency)
{
  if (dependency && dependency->isA() == OdDbAssocValueDependency::desc()) {
    const bool wasDelegating = dependency->isDelegatingToOwningAction();
    dependency->setIsDelegatingToOwningAction(false);
    dependency->evaluate();
    dependency->setIsDelegatingToOwningAction(wasDelegating);
  }
}

OdResult OdDbImpAssocVariable::setName(OdDbAssocVariable* pVariable, const OdString newName, bool updateReferencingExpressions)
{
  if ( m_varName.compare(newName) == 0 )
    return eOk;
  if ((newName.isEmpty() || newName[0] == OdChar('*')) && // anonymous variable
    !updateReferencingExpressions) //!todo implement check for ReferencingExpressions
  {
    m_varName = newName;
    return eOk;
  }
  OdDbObjectId objId = findObjectByName(newName, OdDbAssocVariable::desc());
  if ( !objId.isNull() )
    return eBadObjType;

  OdExpression::Parser p;
  OdResult res = p.setName(newName);
  if (res != eOk)
    return res;

  if (updateReferencingExpressions) {
    OdDbObjectIdArray aReactors = pVariable->getPersistentReactors();
    for (unsigned int i = 0; i < aReactors.size(); i++) {
      OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(aReactors[i].openObject());
      if (pDep.isNull())
        continue;
      OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(pDep->owningAction().openObject());
      if (!pVar.isNull()) {
        pVar->upgradeOpen();
        pVar->assertWriteEnabled();
        res = ((OdDbImpAssocVariable*)OdDbImpAssocAction::getImpObject(pVar))->renameVariable(m_varName, newName);
      }
      else {
        pDep->upgradeOpen();
        pDep->setStatus(kChangedDirectlyAssocStatus);
      }
    }
  }
  m_varName = newName;

  return eOk;
}

OdResult OdDbImpAssocVariable::fixVariable(const OdString& name, const OdDbEvalVariant* value)
{
  OdExpression::Parser p;
  p.fixVariable(name, value);
  OdResult res = p.setExpression(m_expression);
  if (res == eOk)
    m_expression = (p.isConstExpression()) ? OdExpression::createExpression(m_value) : p.getTranslated();
  return res;
}

OdResult OdDbImpAssocVariable::renameVariable(const OdString& oldName, const OdString& newName)
{
  OdExpression::Parser p;
  p.renameVariable(oldName, newName);
  OdResult res = p.setExpression(m_expression);
  if (res == eOk)
    m_expression = p.getTranslated();
  return res;
}

OdResult OdDbImpAssocVariable::checkCyclicDependencies(OdArray<OdString>& names, OdDbObjectIdArray& reactors)
{
  OdResult res = eOk;
  for (unsigned int i = 0; i < reactors.size() && res == eOk; i++) {
    OdDbAssocValueDependencyPtr pDep = OdDbAssocValueDependency::cast(reactors[i].openObject());
    if (!pDep.isNull()) {
      OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(pDep->owningAction().openObject());
      if (!pVar.isNull()) {
        OdString name = OdString(pVar->name()).makeLower();
        for (unsigned int j = 0; j < names.size(); j++) {
          if (name == names[j])
            return eDuplicateKey;
        }
        OdDbObjectIdArray aSubReactors = pVar->getPersistentReactors();
        res = ((OdDbImpAssocVariable*)OdDbImpAssocAction::getImpObject(pVar))->checkCyclicDependencies(names, aSubReactors);
      }
    }
  }
  return res;
}

OdDbObjectId OdDbImpAssocVariable::findObjectByName(const OdString  objectName, 
                              const OdRxClass* pObjectClass) const
{
  OdDbAssocNetworkPtr pNetwork = OdDbAssocNetwork::cast(owningNetwork().openObject());
  if (pNetwork.isNull())
    return OdDbObjectId::kNull;

  OdDbObjectIdArray arrObj = pNetwork->getActions();
  OdDbObjectPtr pObj;
  OdDbAssocVariablePtr pVar;
  for ( unsigned int i = 0; i < arrObj.length(); i++ )
  {
    pObj = arrObj[i].openObject();
    if ( pObj->isKindOf(pObjectClass) )
    {
      //Currently the only named objects are OdDbImpAssocVariables
      if ( pObj->isKindOf(OdDbAssocVariable::desc()) )
      {
        pVar = OdDbAssocVariable::cast(pObj);
        if ( pVar->name().iCompare(objectName) == 0 )
          return arrObj[i];
      }
    } 
  }

  return OdDbObjectId::kNull;
}

OdResult OdDbImpAssocVariable::validateNameAndExpression(const OdString nameToValidate, 
                                                      const OdString expressionToValidate,
                                                      OdString       errorMessage) const
{
  return eNotImplementedYet;
}

OdResult OdDbImpAssocVariable::setExpression(OdDbAssocVariable* pVariable,
                                             const OdString newExpression,
                                          const OdString evaluatorId, 
                                          bool checkForCyclicalDependencies,
                                          bool updateDependenciesOnReferencedSymbols,
                                          OdString* errorMessage,
                                          bool silentMode)
{
  if (!evaluatorId.isEmpty())
    m_evaluatorId = evaluatorId;
  if (newExpression.isEmpty()) {
    m_expression.empty();
    int i = m_arrDependencies.size();
    while (i-- > 0) {
      removeDependency(m_arrDependencies[i], true);
    }
    m_arrValueDependenciesIdx.clear();
    setStatus(pVariable, kChangedDirectlyAssocStatus);
    return eOk;
  }
  OdExpression::Parser p;
  OdResult res;
  if ((res = p.setExpression(newExpression)) != eOk)
    return res;
  OdArray<OdString> usedVars;
  p.getVariables(usedVars);
  int i = usedVars.size();
  while (i-- > 0) {
    if (m_varName.iCompare(usedVars[i]) == 0)
      return eDuplicateKey;
    OdDbAssocVariablePtr var = findObjectByName(usedVars[i], OdDbAssocVariable::desc()).openObject();
    if (var.isNull())
      return eNullObjectPointer;
    p.addVariable(var->name(), var->value());
  }
  if (checkForCyclicalDependencies && usedVars.size() > 0) {
    OdDbObjectIdArray aReactors = pVariable->getPersistentReactors();
    if ((res = checkCyclicDependencies(usedVars, aReactors)) != eOk)
      return res;
  }
  OdDbEvalVariantPtr val = p.evaluate();
  if (val.isNull())
    return eInvalidResBuf;
  if (updateDependenciesOnReferencedSymbols) {
    setExpression(pVariable, OdString::kEmpty, OdString::kEmpty, false, false, 0, true);
    OdInt16 c = 0;
    int i = usedVars.size();
    while (i-- > 0) {
      OdDbAssocVariablePtr var = findObjectByName(usedVars[i], OdDbAssocVariable::desc()).safeOpenObject();
      OdDbCompoundObjectId varCId;
      varCId.set(var->objectId());
      OdDbAssocValueDependencyPtr valDep = OdDbAssocValueDependency::createObject();
      pVariable->addDependency(var->database()->addOdDbObject(valDep));
      valDep->setIsWriteDependency(false);
      valDep->attachToObject(varCId);
      valDep->setDependentOnObjectValue(var->value());
      m_arrValueDependenciesIdx.append(c++); //!TODO check proper behavior
    }
  }
  m_value = val;
  m_expression = newExpression;
  setStatus(pVariable, kChangedDirectlyAssocStatus);
  return eOk;
}

const OdString OdDbImpAssocVariable::evaluatorId() const
{
  return m_evaluatorId;
}

OdResult OdDbImpAssocVariable::setEvaluatorId(const OdString evalId)
{
  m_evaluatorId = evalId;
  return eOk;
}


OdResult OdDbImpAssocVariable::setValue(OdDbAssocAction* pAction, const OdDbEvalVariant& newValue)
{
  *m_value.get() = newValue;
  setStatus(pAction, kChangedDirectlyAssocStatus);

  return eOk;
}

OdResult OdDbImpAssocVariable::setDescription(const OdString newDescription)
{
  m_description = newDescription;
  return eOk;
}


bool OdDbImpAssocVariable::isMergeable() const
{
  return m_isMergeable;
}

bool OdDbImpAssocVariable::mustMerge() const
{
  return m_mustMerge;
}


OdString OdDbImpAssocVariable::mergeableVariableName() const
{
  return m_mergeableVariableName;
}

void OdDbImpAssocVariable::setIsMergeable(bool isMerg, bool mustMerg, const OdString& mergeableVariableName)
{
  m_isMergeable = isMerg;
  m_mustMerge = mustMerg;
  m_mergeableVariableName = mergeableVariableName;
}

OdResult OdDbImpAssocVariable::evaluateExpression(OdDbEvalVariant& evaluatedExpressionValue,
                                     OdString        errorMessage) const
{
  return eNotImplementedYet;
}

OdResult OdDbImpAssocVariable::evaluateExpression(OdDbObjectIdArray&        objectIds,    // in/out argument
                                               OdArray<OdDbEvalVariant>& objectValues, // in/out argument
                                               OdDbEvalVariant&          evaluatedExpressionValue,
                                               OdString                 errorMessage) const
{
  return eNotImplementedYet;
}

OdResult OdDbImpAssocVariable::dwgInFields(OdDbAssocAction *pSelf, OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocAction::dwgInFields(pSelf, pFiler);
  if (res != eOk) 
    return res;
  if (pFiler->rdInt16() != 2)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  m_varName = pFiler->rdString();
  m_expression = pFiler->rdString();
  m_evaluatorId = pFiler->rdString();
  m_description = pFiler->rdString();

  res = m_value->dwgInFields(pFiler);
  if ( res != eOk ) 
    return res;

  m_isMergeable = pFiler->rdBool();
  if (m_isMergeable)
    m_mergeableVariableName = pFiler->rdString();
  m_mustMerge = pFiler->rdBool();

  OdInt16 qty = pFiler->rdInt16();
  if ( qty > 0 )
  {
    m_arrDependencies.resize(qty);
    m_isOwnedDependency.resize(qty, true);
    m_arrValueDependenciesIdx.resize(qty);

    OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
    OdInt16 *pIdx = m_arrValueDependenciesIdx.asArrayPtr();
    while (qty--)
    {
      *pId++ = pFiler->rdHardOwnershipId();
      *pIdx++ = pFiler->rdInt16();
    }
  }
  return eOk;
}

void OdDbImpAssocVariable::dwgOutFields(OdDbDwgFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dwgOutFields(pFiler, objectId);

  pFiler->wrInt16(2);

  pFiler->wrString(m_varName);
  pFiler->wrString(m_expression);
  pFiler->wrString(m_evaluatorId);
  pFiler->wrString(m_description);

  m_value->dwgOutFields(pFiler);

  pFiler->wrBool(m_isMergeable);
  if (m_isMergeable)
    pFiler->wrString(m_mergeableVariableName);
  pFiler->wrBool(m_mustMerge);

  unsigned int qty = m_arrDependencies.size();
  pFiler->wrInt16((OdInt16)qty);
  if ( qty > 0 )
  {
    const OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
    const OdInt16 *pIdx = m_arrValueDependenciesIdx.asArrayPtr();
    while (qty--)
    {
      pFiler->wrHardOwnershipId(*pId++);
      pFiler->wrInt16(*pIdx++);
    }
  }
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{ \
  ODA_FAIL(); \
  return eMakeMeProxy; \
}

OdResult OdDbImpAssocVariable::dxfInFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId)
{
  OdResult res = OdDbImpAssocAction::dxfInFields(pFiler, objectId);
  if ( res != eOk ) 
    return res;

  if( !pFiler->atSubclassData( OdDbAssocVariable::desc()->name() ))
  {
    return eBadDxfSequence;
  }

  NEXT_CODE(90)
  if (pFiler->rdUInt32() != 2)
  {
    ODA_FAIL_ONCE();
    return eMakeMeProxy;
  }

  NEXT_CODE(1)
  pFiler->rdString(m_varName);

  NEXT_CODE(1)
  pFiler->rdString(m_expression);

  NEXT_CODE(1)
  pFiler->rdString(m_evaluatorId);

  NEXT_CODE(1)
  pFiler->rdString(m_description);

  res = m_value->dxfInFields(pFiler);
  if (res != eOk) 
    return res;

  NEXT_CODE(290)
  m_isMergeable = pFiler->rdBool();
  if (m_isMergeable) {
    NEXT_CODE(1)
    pFiler->rdString(m_mergeableVariableName);
  }
  NEXT_CODE(290)
  m_mustMerge = pFiler->rdBool();

  NEXT_CODE(90)
  OdUInt32 qty = pFiler->rdInt32();
  if ( qty > 0 )
  { 
    m_arrDependencies.resize(qty);
    m_isOwnedDependency.resize(qty, true);
    m_arrValueDependenciesIdx.resize(qty);
    OdDbObjectId *pId = m_arrDependencies.asArrayPtr();
    OdInt16 *pIdx = m_arrValueDependenciesIdx.asArrayPtr();
    while (qty--)
    {
      NEXT_CODE(360)
      *pId++ = pFiler->rdObjectId();

      NEXT_CODE(90)
      *pIdx++ = (OdInt16)pFiler->rdInt32();
    }
  }

  return res;
}

void OdDbImpAssocVariable::dxfOutFields(OdDbDxfFiler* pFiler, OdDbObjectId objectId) const
{
  OdDbImpAssocAction::dxfOutFields(pFiler, objectId);

  pFiler->wrSubclassMarker( OdDbAssocVariable::desc()->name() );
  pFiler->wrInt32(90, 2);
  pFiler->wrString(1, m_varName);
  pFiler->wrString(1, m_expression);
  pFiler->wrString(1, m_evaluatorId);
  pFiler->wrString(1, m_description);

  m_value->dxfOutFields(pFiler);

  pFiler->wrBool(290, m_isMergeable);
  if (m_isMergeable)
    pFiler->wrString(1, m_mergeableVariableName);
  pFiler->wrBool(290, m_mustMerge);

  int qty = m_arrDependencies.length();
  pFiler->wrInt32(90, qty);
  if ( qty > 0 )
  {
    const OdDbObjectId* pId = m_arrDependencies.asArrayPtr();
    const OdInt16* pIdx = m_arrValueDependenciesIdx.asArrayPtr();
    while ( qty-- )
    {
      pFiler->wrObjectId(360, *pId++);
      pFiler->wrInt32(90, *pIdx++);
    }
  }
}

void OdDbAssocVariable::appendToOwner(OdDbIdPair& idPair, OdDbObject* owner, OdDbIdMapping& idMap) {
  if (!isAnonymous()) {
    OdDbAssocNetworkPtr network = OdDbAssocNetwork::cast(owner);
    if (network.isNull() && owner->isKindOf(OdDbBlockTableRecord::desc()))
      network = OdDbAssocNetwork::getInstanceFromObject(owner->objectId(), true).safeOpenObject(OdDb::kForWrite);

    OdDbAssocNetCloneCtxPtr ctx = OdDbAssocNetCloneCtx::internal_fromDb(idMap.destDb());
    if (ctx.isNull())
      throw OdError(eInvalidContext);

    OdDbAssocVarCloneCtx& assocVarCtx = ctx->varCloneContext(network);

    const OdString oldVarName = name();
    OdString varName = oldVarName;
    const OdDbAssocVarCloneCtx::DataEntry* clonedEntry = assocVarCtx.getEntryBySourceName(varName);
    if ( clonedEntry && clonedEntry->m_isCloned ) {
      varName = clonedEntry->m_dstName;
    }

    if (assocVarCtx.getEntryByDestName(varName)) {
      OdString prefix = varName;
      prefix.trimRight(OD_T("0123456789"));
      OdString suffix = varName.mid(prefix.getLength());
      int i = int(assocVarCtx.destVariableCount()); i = (i > 99) ? i : 0;

      if (suffix.getLength() > 7)
        prefix = varName;
      else if (i == 0 && !suffix.isEmpty())
        i = OdUnitsFormatterTool::toInt(suffix);

      do {
        varName.format(OD_T("%ls%d"), prefix.c_str(), ++i);
      }
      while (assocVarCtx.getEntryByDestName(varName) || assocVarCtx.getEntryBySourceName(varName));

      if (setName(varName, true) != eOk)
        return;
    }

    OdDbAssocAction::appendToOwner(idPair, network, idMap);
    if(idPair.isOwnerXlated())
      assocVarCtx.recordVariableClone(oldVarName, varName);
  }
  else
    OdDbAssocAction::appendToOwner(idPair, owner, idMap);
}

void convertToConstant(OdDbAssocVariable* variable) {
  variable->removeAllDependencies(true);
  variable->setExpression(OdExpression::createExpression(variable->value()), OdString::kEmpty, false, false);
}

void merge_UsingCloned(OdDbAssocVariable* original, OdDbAssocVariable* cloned) {
  OdDbAssocNetCloneCtxPtr ctx = OdDbAssocNetCloneCtx::fromDatabase(cloned->database());
  if (ctx.isNull())
    return;

  OdDbAssocNetworkPtr destNetwork = OdDbAssocNetwork::cast(cloned->owningNetwork().openObject());
  if (destNetwork.isNull())
    return;

  const OdDbAssocVarCloneCtx& varCtx = ctx->varCloneContext(destNetwork);

  OdStringArray aVarNames;
  {
    OdExpression::Parser expressionParser;
    expressionParser.setExpression(cloned->expression());
    expressionParser.getVariables(aVarNames);
  }

  OdExpression::Parser newExpressionParser;

  OdStringArray::iterator varIt = aVarNames.begin();
  OdStringArray::iterator varEnd = aVarNames.end();
  for (; varIt != varEnd; ++varIt) {
    const OdDbAssocVarCloneCtx::DataEntry* destVarEntry = varCtx.getEntryByDestName(*varIt);
    if (destVarEntry && destVarEntry->m_isCloned)
      continue;

    OdDbObjectId nonClonedVarId = original->findObjectByName(*varIt, OdDbAssocVariable::desc());
    OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(nonClonedVarId.openObject());
    if (pVar.isNull()) {
      ODA_ASSERT(false && "Original dependent variable could not be found");
      convertToConstant(cloned);
      return;
    }

    newExpressionParser.fixVariable(*varIt, pVar->value());
  }

  newExpressionParser.setExpression(cloned->expression());
  if (newExpressionParser.isConstExpression()) {
    convertToConstant(cloned);
  } else {
    cloned->setExpression(newExpressionParser.getTranslated(), cloned->evaluatorId(), true, true);
  }
}

bool allVariablesAvailable(const OdDbAssocVariable* original, OdDbAssocVariable* clone) {
  OdExpression::Parser expressionParser;
  expressionParser.setExpression(clone->expression());
  OdStringArray variables;
  expressionParser.getVariables(variables);
  OdStringArray::size_type n = variables.size();
  for(OdStringArray::size_type i=0; i<n; ++i) {
    const OdString& varName = variables[i];
    OdDbAssocVariablePtr existingVar = clone->findObjectByName(varName, OdDbAssocVariable::desc()).openObject();
    if (existingVar.isNull())
      return false;
    OdDbAssocVariablePtr originalVar = original->findObjectByName(varName, OdDbAssocVariable::desc()).openObject();
    if (originalVar.get() && *existingVar->value() != *originalVar->value())
      return false;
  }
  return true;
}

void merge_UsingExisting_Or_ConvertToConstant(OdDbAssocVariablePtr original, OdDbAssocVariablePtr clone) {
  if (allVariablesAvailable(original, clone))
    clone->setExpression(clone->expression(), OdString::kEmpty, true, true, 0, true);
  else
    convertToConstant(clone);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dna: this code below do very similar things to what generic clone operations do                          //
//      except cloning referenced objects ('owned' or 'hardly' referenced), so maybe it makes sense
//      to generalize this implementation. Probably, these dependent variables should be cloned
//      along with other objects in a common manner.

//      This is part of OdDbObject::deepClone/wblockClone():
//
OdDbAssocVariablePtr OdDbImpAssocVariable::createVariableCopy(const OdDbAssocVariable* originalVar)
{
  OdDbAssocVariablePtr newVar = originalVar->isA()->create();

  OdDbImpAssocVariable* newVarImp = (OdDbImpAssocVariable*)getImpObject(newVar.get());
  newVarImp->m_varName = originalVar->name();
  newVarImp->m_expression = originalVar->expression();
  newVarImp->m_description = originalVar->description();
  newVarImp->m_evaluatorId = originalVar->evaluatorId();
  newVarImp->m_isMergeable = originalVar->isMergeable();
  newVarImp->m_mustMerge = originalVar->mustMerge();
  newVarImp->m_mergeableVariableName = originalVar->mergeableVariableName();
  newVarImp->m_value->copyFrom(originalVar->value());
  newVar->setXData(originalVar->xData());

  return newVar;
}

TOOLKIT_EXPORT void oddbTranslateXData(OdDbObject* object, OdDbIdMapping& pMap);

//      This is part of common id translation and owner xlation during generic deep/wblock cloning:
//
OdDbAssocVariablePtr addVarCopy(OdDbIdMapping& idMap, const OdDbAssocVariable* originalVar, OdDbAssocNetwork* destNetwork) {
  OdDbAssocVariablePtr newVar = OdDbImpAssocVariable::createVariableCopy(originalVar);
  if (idMap.destDb() != idMap.origDb()) // wblockClone
    oddbTranslateXData(newVar, idMap); // translate ids from different database

  OdDbIdPair idPair(originalVar->objectId(), destNetwork->database()->addOdDbObject(newVar), false, false);
  newVar->appendToOwner(idPair, destNetwork, idMap); // adds variable and rename if name conflicted

  return newVar;
}

//                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////


enum MergeMode {
  kParameterCopyMode_3,
  kParameterCopyMode_4,
  kRenameAllParameters,
};

inline bool canBeRenamed(MergeMode mode) {
  return (mode == kParameterCopyMode_4 || mode == kRenameAllParameters);
}
inline bool wasRenamed(const OdDbAssocVariable* newVar, const OdDbAssocVariable* originalVar) {
  return newVar->name().iCompare(originalVar->name()) != 0;
}

void merge_UsingExisting_CreateMissing_RenameCollided(
  OdDbIdMapping& idMap, const OdDbAssocVariable* original,
  OdDbAssocVariable* clone, MergeMode mode)
{
  OdExpression::Parser expressionParser;
  OdString origExpression = clone->expression();
  expressionParser.setExpression(origExpression);
  OdStringArray variables;
  expressionParser.getVariables(variables);
  OdStringArray::size_type n = variables.size();
  if (n) {
    OdDbAssocNetworkPtr destNetwork = clone->owningNetwork().openObject(OdDb::kForWrite);
    if (destNetwork.get()) {
      for (OdStringArray::size_type i = 0; i < n; ++i) {
        const OdString& varName = variables[i];
        OdDbAssocVariablePtr originalVar, existingDestVar;

        originalVar = original->findObjectByName(varName, OdDbAssocVariable::desc()).openObject();
        if (originalVar.get()) {
          OdString clonedVarName;
          OdDbAssocNetCloneCtxPtr ctx = OdDbAssocNetCloneCtx::internal_fromDb(destNetwork->database());
          if (!ctx.get())
            break;

          OdDbAssocVarCloneCtx& assocVarCtx = ctx->varCloneContext(destNetwork);
          const OdDbAssocVarCloneCtx::DataEntry* clonedEntry = assocVarCtx.getEntryBySourceName(varName);
          if (clonedEntry && clonedEntry->m_isCloned) {
            if (clonedEntry->m_dstName != clonedEntry->m_srcName)
              expressionParser.renameVariable(clonedEntry->m_srcName, clonedEntry->m_dstName);
            continue;
          }

          if (const OdDbAssocVarCloneCtx::DataEntry* destEntry = assocVarCtx.getEntryByDestName(varName)) {
            if (destEntry->m_isCloned)
              continue;

            if (mode != kRenameAllParameters)
              existingDestVar = clone->findObjectByName(varName, OdDbAssocVariable::desc()).openObject();
          }

          if (existingDestVar.isNull() || mode==kParameterCopyMode_4 && *existingDestVar->value() != *originalVar->value()) {
            OdDbAssocVariablePtr newVar = addVarCopy(idMap, originalVar, destNetwork);
            if (wasRenamed(newVar, originalVar))
              expressionParser.renameVariable(originalVar->name(), newVar->name());

            merge_UsingExisting_CreateMissing_RenameCollided(idMap, originalVar, newVar, mode);
          }
        }
      }
      expressionParser.setExpression(origExpression); // translate renamed variables
      clone->setExpression(expressionParser.getTranslated(), OdString::kEmpty, true, true, 0, true);
    }
  }
}

bool entireNetworkCloned(const OdDbAssocVariable* thisAction, const OdDbIdMapping& idMap) {
  OdDbIdPair idPair(thisAction->owningNetwork());
  return idMap.compute(idPair) && idPair.isCloned();
}

OdResult OdDbImpAssocVariable::postProcessAfterDeepClone(OdDbAssocAction *action, OdDbIdMapping& idMap) {
  OdDbAssocVariablePtr clone = OdDbAssocVariable::cast(
    ::oddbTranslate(action->objectId(), idMap).openObject(OdDb::kForWrite));
  if (clone.get()) {
    OdDbAssocVariablePtr thisVar(action);
    if(::entireNetworkCloned(thisVar, idMap))
      return eOk;

    OdDbAssocVariablePtr original(action);
    int PARAMETERCOPYMODE = idMap.origDb()->appServices()->getPARAMETERCOPYMODE();
    switch (idMap.deepCloneContext())
    {
    case OdDb::kDcCopy:
      switch (PARAMETERCOPYMODE) {
      case 0:
      case 1:
      case 2:
        ::merge_UsingExisting_Or_ConvertToConstant(original, clone);
        break;
      case 3:
        ::merge_UsingExisting_CreateMissing_RenameCollided(idMap, original, clone, kParameterCopyMode_3);
        break;
      case 4:
        ::merge_UsingExisting_CreateMissing_RenameCollided(idMap, original, clone, kParameterCopyMode_4);
        break;
      default:
        ODA_FAIL_ONCE();
        break;
      }
      break;

    case OdDb::kDcExplode:
      ::merge_UsingExisting_CreateMissing_RenameCollided(idMap, original, clone, kParameterCopyMode_4);
      break;

    case OdDb::kDcWblock:
    case OdDb::kDcBlock:
      switch (PARAMETERCOPYMODE) {
      case 1:
        ::merge_UsingCloned(original, clone);
        break;
      case 2:
      case 3:
      case 4:
        ::merge_UsingExisting_CreateMissing_RenameCollided(idMap, original, clone, kParameterCopyMode_4);
        break;
      default:
        ODA_FAIL_ONCE();
        break;
      }
      break;

    default:
      switch (PARAMETERCOPYMODE) {
      case 1:
        ::merge_UsingCloned(original, clone);
        break;
      case 2:
        ::merge_UsingExisting_Or_ConvertToConstant(original, clone);
        break;
      case 3:
        ::merge_UsingExisting_CreateMissing_RenameCollided(idMap, original, clone, kParameterCopyMode_3);
        break;
      case 4:
        ::merge_UsingExisting_CreateMissing_RenameCollided(idMap, original, clone, kParameterCopyMode_4);
        break;
      default:
        ODA_FAIL_ONCE();
        break;
      }
      break;
    }
  }
  return eOk;
}

bool OdDbImpAssocVariable::isRelevantDependencyChange(const OdDbAssocDependency* pDependency) const
{
  if (pDependency && pDependency->isA() == OdDbAssocValueDependency::desc())
  {
    const OdDbAssocValueDependency* pValDep = static_cast<const OdDbAssocValueDependency*>(pDependency);
    OdDbEvalVariantPtr pActual = OdDbEvalVariant::createObject(),
                       pCached = OdDbEvalVariant::createObject();
    if (pValDep->getDependentOnObjectValue(pActual) == eOk &&
        pValDep->getDependentOnObjectCachedValue(pCached) == eOk &&
        pActual.get() && pCached.get())
    {
      return *pActual != *pCached;
    }
  }
  return true;
}
