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
#include "DbTableStyle.h"
#include "DbValueTypes.h"

/* OdDbTableStyleTemplateProperty */

struct OdDbTableStyleTemplateProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableStyleTemplateProperty>::createObject();
    ((OdDbTableStyleTemplateProperty*)res.get())->init(__OD_T("Template"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbTableStyleTemplateProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbTableStyleTemplateProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTableStylePtr pObj = OdDbTableStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->getTemplate());
  return eOk;
}

/* OdDbTableStyleBitFlagsProperty */

struct OdDbTableStyleBitFlagsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableStyleBitFlagsProperty>::createObject();
    ((OdDbTableStyleBitFlagsProperty*)res.get())->init(__OD_T("BitFlags"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableStyleBitFlagsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTableStylePtr pObj = OdDbTableStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->bitFlags());
  return eOk;
}

OdResult OdDbTableStyleBitFlagsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTableStylePtr pObj = OdDbTableStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBitFlags(val);
  return eOk;
}

/* OdDbTableStyleDescriptionProperty */

struct OdDbTableStyleDescriptionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableStyleDescriptionProperty>::createObject();
    ((OdDbTableStyleDescriptionProperty*)res.get())->init(__OD_T("Description"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableStyleDescriptionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTableStylePtr pObj = OdDbTableStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->description());
  return eOk;
}

OdResult OdDbTableStyleDescriptionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTableStylePtr pObj = OdDbTableStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDescription(val);
  return eOk;
}

/* OdDbTableStyleFlowDirectionProperty */

struct OdDbTableStyleFlowDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableStyleFlowDirectionProperty>::createObject();
    ((OdDbTableStyleFlowDirectionProperty*)res.get())->init(__OD_T("FlowDirection"), &OdRxValueType::Desc<OdDb::FlowDirection>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableStyleFlowDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTableStylePtr pObj = OdDbTableStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::FlowDirection>(pObj->flowDirection());
  return eOk;
}

OdResult OdDbTableStyleFlowDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTableStylePtr pObj = OdDbTableStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::FlowDirection val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFlowDirection(val);
  return eOk;
}

/* OdDbTableStyleNameProperty */

struct OdDbTableStyleNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableStyleNameProperty>::createObject();
    ((OdDbTableStyleNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableStyleNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTableStylePtr pObj = OdDbTableStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getName());
  return eOk;
}

OdResult OdDbTableStyleNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTableStylePtr pObj = OdDbTableStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setName(val);
  return eOk;
}

void createOdDbTableStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbTableStyleTemplateProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableStyleBitFlagsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableStyleDescriptionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableStyleFlowDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableStyleNameProperty::createObject(b.owner()));
  b.add(properties.last());
}
