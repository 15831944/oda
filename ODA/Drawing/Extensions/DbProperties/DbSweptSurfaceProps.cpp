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
#include "DbSweptSurface.h"
#include "DbValueTypes.h"

/* OdDbSweptSurfacePathLengthProperty */

struct OdDbSweptSurfacePathLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSweptSurfacePathLengthProperty>::createObject();
    ((OdDbSweptSurfacePathLengthProperty*)res.get())->init(__OD_T("PathLength"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbSweptSurfacePathLengthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSweptSurfacePathLengthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Length"));
    ((OdDbSweptSurfacePathLengthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the length of the sweep path"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbSweptSurfacePathLengthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 6));
    ((OdDbSweptSurfacePathLengthProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Length"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSweptSurfacePathLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSweptSurfacePtr pObj = OdDbSweptSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double len; OdResult rc = pObj->getPathLength(len); if (rc == eOk) value = len;
  return rc;
}

/* OdDbSweptSurfaceSweepOptionsProperty */

struct OdDbSweptSurfaceSweepOptionsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSweptSurfaceSweepOptionsProperty>::createObject();
    ((OdDbSweptSurfaceSweepOptionsProperty*)res.get())->init(__OD_T("SweepOptions"), &OdRxValueType::Desc<OdDbSweepOptions>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"AlignAngle", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Profile rotation"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rotation of the sweep profile"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    res = OdRxSubProperty::createObject(L"ScaleFactor", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Scale along path"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the scale factor from start to the end of the sweep path"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 3));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"scale"));
    res = OdRxSubProperty::createObject(L"TwistAngle", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Twist along path"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the amount of rotation along the entire length of the sweep path"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 4));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Twist"));
    res = OdRxSubProperty::createObject(L"Bank", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Bank along path"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether or not the profile curve twists and rotates along a 3D path"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 5));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSweptSurfaceSweepOptionsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSweptSurfacePtr pObj = OdDbSweptSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDbSweepOptions sweepOptions; pObj->getSweepOptions(sweepOptions); value = sweepOptions;
  return eOk;
}

OdResult OdDbSweptSurfaceSweepOptionsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSweptSurfacePtr pObj = OdDbSweptSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbSweepOptions val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSweepOptions(val);
  return eOk;
}

void createOdDbSweptSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbSweptSurfacePathLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSweptSurfaceSweepOptionsProperty::createObject(b.owner()));
  b.add(properties.last());
}
