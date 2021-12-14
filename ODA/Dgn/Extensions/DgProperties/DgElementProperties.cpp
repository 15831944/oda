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

#include "DgElementProperties.h"

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: ElementId
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementElementIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->elementId();
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: ODAUniqueID
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementODAUniqueIDProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = (OdUInt64)(pObj->elementId().getHandle());
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: ElementType
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementElementTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->getElementType();
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: ElementSubType
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementElementSubTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->getElementSubType();
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: ModificationTime
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementModificationTimeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdTimeStamp tmp;
  tmp.setPackedValue( (OdUInt64)(pObj->getLastModifiedTime())/1000);
  value = tmp;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: New
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementNewProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isNewObject();
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Modified
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementModifiedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isModifiedGraphics();
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Locked
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementLockedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->getLockedFlag();
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementLockedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;

  const bool* pValue = rxvalue_cast<bool>(&value);

  if( pValue )
  {
    pObj->setLockedFlag(*pValue);
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Name
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgElementNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgElementPtr pObj = OdDgElement::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;

  OdString strName = pObj->isA()->name().c_str();
  value = strName;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------



