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
#include "DbRadialDimension.h"
#include "DbValueTypes.h"

/* OdDbRadialDimensionLeaderLengthProperty */

struct OdDbRadialDimensionLeaderLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRadialDimensionLeaderLengthProperty>::createObject();
    ((OdDbRadialDimensionLeaderLengthProperty*)res.get())->init(__OD_T("LeaderLength"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbRadialDimensionLeaderLengthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRadialDimensionLeaderLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRadialDimensionPtr pObj = OdDbRadialDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->leaderLength());
  return eOk;
}

OdResult OdDbRadialDimensionLeaderLengthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRadialDimensionPtr pObj = OdDbRadialDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderLength(val);
  return eOk;
}

/* OdDbRadialDimensionCenterProperty */

struct OdDbRadialDimensionCenterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRadialDimensionCenterProperty>::createObject();
    ((OdDbRadialDimensionCenterProperty*)res.get())->init(__OD_T("Center"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRadialDimensionCenterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRadialDimensionPtr pObj = OdDbRadialDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->center());
  return eOk;
}

OdResult OdDbRadialDimensionCenterProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRadialDimensionPtr pObj = OdDbRadialDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCenter(val);
  return eOk;
}

/* OdDbRadialDimensionChordPointProperty */

struct OdDbRadialDimensionChordPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRadialDimensionChordPointProperty>::createObject();
    ((OdDbRadialDimensionChordPointProperty*)res.get())->init(__OD_T("ChordPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRadialDimensionChordPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRadialDimensionPtr pObj = OdDbRadialDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->chordPoint());
  return eOk;
}

OdResult OdDbRadialDimensionChordPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRadialDimensionPtr pObj = OdDbRadialDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setChordPoint(val);
  return eOk;
}

void createOdDbRadialDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRadialDimensionLeaderLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRadialDimensionCenterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRadialDimensionChordPointProperty::createObject(b.owner()));
  b.add(properties.last());
}
