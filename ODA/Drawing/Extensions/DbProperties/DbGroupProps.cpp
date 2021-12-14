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
#include "DbGroup.h"
#include "DbValueTypes.h"

/* OdDbGroupSelectableProperty */

struct OdDbGroupSelectableProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGroupSelectableProperty>::createObject();
    ((OdDbGroupSelectableProperty*)res.get())->init(__OD_T("Selectable"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbGroupSelectableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGroupPtr pObj = OdDbGroup::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isSelectable());
  return eOk;
}

OdResult OdDbGroupSelectableProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbGroupPtr pObj = OdDbGroup::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSelectable(val);
  return eOk;
}

/* OdDbGroupDescriptionProperty */

struct OdDbGroupDescriptionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGroupDescriptionProperty>::createObject();
    ((OdDbGroupDescriptionProperty*)res.get())->init(__OD_T("Description"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbGroupDescriptionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGroupPtr pObj = OdDbGroup::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->description());
  return eOk;
}

OdResult OdDbGroupDescriptionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbGroupPtr pObj = OdDbGroup::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDescription(val);
  return eOk;
}

/* OdDbGroupNumEntitiesProperty */

struct OdDbGroupNumEntitiesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGroupNumEntitiesProperty>::createObject();
    ((OdDbGroupNumEntitiesProperty*)res.get())->init(__OD_T("NumEntities"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbGroupNumEntitiesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGroupPtr pObj = OdDbGroup::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numEntities());
  return eOk;
}

/* OdDbGroupIsAnonymousProperty */

struct OdDbGroupIsAnonymousProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGroupIsAnonymousProperty>::createObject();
    ((OdDbGroupIsAnonymousProperty*)res.get())->init(__OD_T("IsAnonymous"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbGroupIsAnonymousProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGroupPtr pObj = OdDbGroup::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isAnonymous());
  return eOk;
}

/* OdDbGroupIsNotAccessibleProperty */

struct OdDbGroupIsNotAccessibleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGroupIsNotAccessibleProperty>::createObject();
    ((OdDbGroupIsNotAccessibleProperty*)res.get())->init(__OD_T("IsNotAccessible"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbGroupIsNotAccessibleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGroupPtr pObj = OdDbGroup::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isNotAccessible());
  return eOk;
}

void createOdDbGroupProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbGroupSelectableProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbGroupDescriptionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbGroupNumEntitiesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbGroupIsAnonymousProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbGroupIsNotAccessibleProperty::createObject(b.owner()));
  b.add(properties.last());
}
