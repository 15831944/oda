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
#include "DbAssocValueDependency.h"
#include "DbImpAssocValueDependency.h"
#include "DbAssocVariable.h"
#include "DbImpAssocVariable.h"
#include "DbAssocNetwork.h"

#include "DbFiler.h"

OdDbImpAssocValueDependency::OdDbImpAssocValueDependency() : OdDbImpAssocDependency(),
                                                       m_valueName()
{
  m_cashedValue = OdDbEvalVariant::createObject();
}

const OdString& OdDbImpAssocValueDependency::valueName() const
{
  return m_valueName;
}

OdResult OdDbImpAssocValueDependency::setValueName(const OdString& newValueName)
{
  return eNotImplementedYet;
}

bool OdDbImpAssocValueDependency::hasCachedValue() const {
  return m_cashedValue.get() && m_cashedValue->restype()!=OdResBuf::kRtNone;
}

OdResult OdDbImpAssocValueDependency::getDependentOnObjectValue(OdDbEvalVariantPtr& objectValue) const
{
  if (objectValue.isNull())
    objectValue = OdDbEvalVariant::createObject();

  OdDbObjectPtr obj;
  OdResult res = dependentOnObject().openObject(obj);
  OdDbAssocVariablePtr var = obj;
  if(var.get())
    objectValue->copyFrom(var->value());
  else if (hasCachedValue()) {
    objectValue->copyFrom(m_cashedValue);
    res = eOk;
  }
  return res;
}

OdResult OdDbImpAssocValueDependency::setDependentOnObjectValue(const OdDbEvalVariant* newObjectValue)
{
  m_cashedValue->copyFrom(newObjectValue);
  return eOk;
}

void OdDbImpAssocValueDependency::evaluate(OdDbAssocDependency* thisDependency) {
  OdDbAssocVariablePtr pVar = OdDbAssocVariable::cast(thisDependency->dependentOnObject().openObject());
  if (pVar.get())
    m_cashedValue->copyFrom(pVar->value());
  OdDbImpAssocDependency::evaluate(thisDependency);
}

OdResult OdDbImpAssocValueDependency::dwgInFields(OdDbDwgFiler* pFiler)
{
  OdResult res = OdDbImpAssocDependency::dwgInFields(pFiler);
  if ( res != eOk )   
    return res;

  if ( pFiler->rdInt16() != 0 )
    return eMakeMeProxy;

  m_valueName = pFiler->rdString();
  res = m_cashedValue->dwgInFields(pFiler);
  if (res == eBadDxfSequence)
    res = m_cashedValue->setValue(OdResBuf::kDxfReal, 0.0);

  return res;
}

void OdDbImpAssocValueDependency::dwgOutFields(OdDbDwgFiler* pFiler) const
{
  OdDbImpAssocDependency::dwgOutFields(pFiler);

  pFiler->wrInt16(0);

  pFiler->wrString(m_valueName);

  m_cashedValue->dwgOutFields(pFiler);
}

#define NEXT_CODE(code) \
if (pFiler->nextItem() != code) \
{                               \
  ODA_FAIL();                   \
  return eBadDxfSequence;       \
}

OdResult OdDbImpAssocValueDependency::dxfInFields(OdDbDxfFiler* pFiler)
{
  OdResult res = OdDbImpAssocDependency::dxfInFields(pFiler);
  if ( res != eOk )   
    return res;

  if( !pFiler->atSubclassData( OdDbAssocValueDependency::desc()->name() ))
  {
    return eBadDxfSequence;
  }

  if ( pFiler->nextItem() != 90 || pFiler->rdUInt32() != 0)
    return eMakeMeProxy;

  NEXT_CODE(1)
  pFiler->rdString(m_valueName);

  res = m_cashedValue->dxfInFields(pFiler);
  if ( res != eOk )   
    return res;

  return res;
}

void OdDbImpAssocValueDependency::dxfOutFields(OdDbDxfFiler* pFiler) const
{
  OdDbImpAssocDependency::dxfOutFields(pFiler);

  pFiler->wrSubclassMarker(OdDbAssocValueDependency::desc()->name());
  pFiler->wrInt32(90, 0);
  pFiler->wrString(1, m_valueName);
  m_cashedValue->dxfOutFields(pFiler);
}

bool OdDbImpAssocValueDependency::isRelevantChange(const OdDbAssocDependency *pThisDep) const
{
  OdDbAssocVariablePtr pVar = pThisDep->dependentOnObject().openObject();
  if (pVar.isNull() || *pVar->value() == *m_cashedValue)
    return false;
  return true;
}

OdResult OdDbImpAssocValueDependency::updateDependentOnObject(OdDbAssocDependency * pDep)
{
  return eOk;
}

OdResult OdDbImpAssocValueDependency::getDependentOnObjectCachedValue(OdDbEvalVariantPtr& cachedValue) const
{
  if (!hasCachedValue())
    return eNotApplicable;

  if (cachedValue.isNull())
    cachedValue = OdDbEvalVariant::createObject();

  cachedValue->copyFrom(m_cashedValue);
  return eOk;
}
