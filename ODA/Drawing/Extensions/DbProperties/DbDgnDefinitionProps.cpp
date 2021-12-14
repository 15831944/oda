///////////////////////////////////////////////////////////////////////////////
// THIS FILE IS MACHINE GENERATED!!!!                                        //
///////////////////////////////////////////////////////////////////////////////

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
#include "RxProperty.h"
#include "RxObjectImpl.h"
#include "RxAttribute.h"
#include "RxCategory.h"
#include "RxValueTypeUtil.h"
#include "DbValueTypes.h"
#include "DbUnderlayDefinition.h"

/* OdDbDgnDefinitionXrefDepthProperty */

struct OdDbDgnDefinitionXrefDepthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDgnDefinitionXrefDepthProperty>::createObject();
    ((OdDbDgnDefinitionXrefDepthProperty*)res.get())->init(__OD_T("XrefDepth"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDgnDefinitionXrefDepthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDgnDefinitionPtr pObj = OdDbDgnDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->xrefDepth());
  return eOk;
}

OdResult OdDbDgnDefinitionXrefDepthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDgnDefinitionPtr pObj = OdDbDgnDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXrefDepth(val);
  return eOk;
}

/* OdDbDgnDefinitionUseMasterUnitsProperty */

struct OdDbDgnDefinitionUseMasterUnitsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDgnDefinitionUseMasterUnitsProperty>::createObject();
    ((OdDbDgnDefinitionUseMasterUnitsProperty*)res.get())->init(__OD_T("UseMasterUnits"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDgnDefinitionUseMasterUnitsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDgnDefinitionPtr pObj = OdDbDgnDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->useMasterUnits());
  return eOk;
}

OdResult OdDbDgnDefinitionUseMasterUnitsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDgnDefinitionPtr pObj = OdDbDgnDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setUseMasterUnits(val);
  return eOk;
}

/* OdDbDgnDefinitionSetShowRasterRefProperty */

struct OdDbDgnDefinitionSetShowRasterRefProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDgnDefinitionSetShowRasterRefProperty>::createObject();
    ((OdDbDgnDefinitionSetShowRasterRefProperty*)res.get())->init(__OD_T("SetShowRasterRef"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDgnDefinitionSetShowRasterRefProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDgnDefinitionPtr pObj = OdDbDgnDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->showRasterRef());
  return eOk;
}

OdResult OdDbDgnDefinitionSetShowRasterRefProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDgnDefinitionPtr pObj = OdDbDgnDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setShowRasterRef(val);
  return eOk;
}

/* OdDbDgnDefinitionShowRasterRefProperty */

struct OdDbDgnDefinitionShowRasterRefProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDgnDefinitionShowRasterRefProperty>::createObject();
    ((OdDbDgnDefinitionShowRasterRefProperty*)res.get())->init(__OD_T("ShowRasterRef"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDgnDefinitionShowRasterRefProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDgnDefinitionPtr pObj = OdDbDgnDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->showRasterRef());
  return eOk;
}

void createOdDbDgnDefinitionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbDgnDefinitionXrefDepthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDgnDefinitionUseMasterUnitsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDgnDefinitionSetShowRasterRefProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDgnDefinitionShowRasterRefProperty::createObject(b.owner()));
  b.add(properties.last());
}
