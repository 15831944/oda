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
#include "DbRevolvedSurface.h"
#include "DbValueTypes.h"

/* OdDbRevolvedSurfaceAxisPointProperty */

struct OdDbRevolvedSurfaceAxisPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRevolvedSurfaceAxisPointProperty>::createObject();
    ((OdDbRevolvedSurfaceAxisPointProperty*)res.get())->init(__OD_T("AxisPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRevolvedSurfaceAxisPointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Axis position X;Axis position Y;Axis position Z"));
    ((OdDbRevolvedSurfaceAxisPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the start point of the axis of revolution"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbRevolvedSurfaceAxisPointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 3));
    ((OdDbRevolvedSurfaceAxisPointProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"AxisPosition"));
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

OdResult OdDbRevolvedSurfaceAxisPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRevolvedSurfacePtr pObj = OdDbRevolvedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGePoint3d>(pObj->getAxisPnt());
  return eOk;
}

OdResult OdDbRevolvedSurfaceAxisPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRevolvedSurfacePtr pObj = OdDbRevolvedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setAxisPnt(val);
  return eOk;
}

/* OdDbRevolvedSurfaceAxisDirectionProperty */

struct OdDbRevolvedSurfaceAxisDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRevolvedSurfaceAxisDirectionProperty>::createObject();
    ((OdDbRevolvedSurfaceAxisDirectionProperty*)res.get())->init(__OD_T("AxisDirection"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRevolvedSurfaceAxisDirectionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Axis direction X;Axis direction Y;Axis direction Z"));
    ((OdDbRevolvedSurfaceAxisDirectionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Displays the direction of the axis of revolution"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbRevolvedSurfaceAxisDirectionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 4));
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

OdResult OdDbRevolvedSurfaceAxisDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRevolvedSurfacePtr pObj = OdDbRevolvedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGeVector3d>(pObj->getAxisVec());
  return eOk;
}

OdResult OdDbRevolvedSurfaceAxisDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRevolvedSurfacePtr pObj = OdDbRevolvedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setAxisVec(val);
  return eOk;
}

/* OdDbRevolvedSurfaceStartAngleProperty */

struct OdDbRevolvedSurfaceStartAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRevolvedSurfaceStartAngleProperty>::createObject();
    ((OdDbRevolvedSurfaceStartAngleProperty*)res.get())->init(__OD_T("StartAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbRevolvedSurfaceStartAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbRevolvedSurfaceStartAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRevolvedSurfacePtr pObj = OdDbRevolvedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->getStartAngle());
  return eOk;
}

/* OdDbRevolvedSurfaceRevolveAngleProperty */

struct OdDbRevolvedSurfaceRevolveAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRevolvedSurfaceRevolveAngleProperty>::createObject();
    ((OdDbRevolvedSurfaceRevolveAngleProperty*)res.get())->init(__OD_T("RevolveAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbRevolvedSurfaceRevolveAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRevolvedSurfaceRevolveAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Angle of revolution"));
    ((OdDbRevolvedSurfaceRevolveAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the angle of revolution"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbRevolvedSurfaceRevolveAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    ((OdDbRevolvedSurfaceRevolveAngleProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"RevolutionAngle"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRevolvedSurfaceRevolveAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRevolvedSurfacePtr pObj = OdDbRevolvedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->getRevolveAngle());
  return eOk;
}

OdResult OdDbRevolvedSurfaceRevolveAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRevolvedSurfacePtr pObj = OdDbRevolvedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRevolveAngle(val);
  return eOk;
}

void createOdDbRevolvedSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRevolvedSurfaceAxisPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRevolvedSurfaceAxisDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRevolvedSurfaceStartAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRevolvedSurfaceRevolveAngleProperty::createObject(b.owner()));
  b.add(properties.last());
}
