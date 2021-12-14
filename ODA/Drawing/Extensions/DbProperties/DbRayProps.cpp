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
#include "DbRay.h"
#include "DbValueTypes.h"

/* OdDbRayBasePointProperty */

struct OdDbRayBasePointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRayBasePointProperty>::createObject();
    ((OdDbRayBasePointProperty*)res.get())->init(__OD_T("BasePoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRayBasePointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Basepoint X;Basepoint Y;Basepoint Z"));
    ((OdDbRayBasePointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the basepoint of the ray or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbRayBasePointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 4));
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

OdResult OdDbRayBasePointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRayPtr pObj = OdDbRay::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->basePoint());
  return eOk;
}

OdResult OdDbRayBasePointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRayPtr pObj = OdDbRay::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBasePoint(val);
  return eOk;
}

/* OdDbRayUnitDirProperty */

struct OdDbRayUnitDirProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRayUnitDirProperty>::createObject();
    ((OdDbRayUnitDirProperty*)res.get())->init(__OD_T("UnitDir"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRayUnitDirProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Direction vector X;Direction vector Y;Direction vector Z"));
    ((OdDbRayUnitDirProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z direction vectors of the ray"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbRayUnitDirProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 6));
    ((OdDbRayUnitDirProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DirectionVector"));
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

OdResult OdDbRayUnitDirProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRayPtr pObj = OdDbRay::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->unitDir());
  return eOk;
}

OdResult OdDbRayUnitDirProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRayPtr pObj = OdDbRay::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setUnitDir(val);
  return eOk;
}

void createOdDbRayProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRayBasePointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRayUnitDirProperty::createObject(b.owner()));
  b.add(properties.last());
}
