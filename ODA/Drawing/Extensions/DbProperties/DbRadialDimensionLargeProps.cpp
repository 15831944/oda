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
#include "DbRadialDimensionLarge.h"
#include "DbValueTypes.h"

/* OdDbRadialDimensionLargeCenterProperty */

struct OdDbRadialDimensionLargeCenterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRadialDimensionLargeCenterProperty>::createObject();
    ((OdDbRadialDimensionLargeCenterProperty*)res.get())->init(__OD_T("Center"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRadialDimensionLargeCenterProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Center"));
    ((OdDbRadialDimensionLargeCenterProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the center of the arc"));
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

OdResult OdDbRadialDimensionLargeCenterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->center());
  return eOk;
}

OdResult OdDbRadialDimensionLargeCenterProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCenter(val);
  return eOk;
}

/* OdDbRadialDimensionLargeJogAngleProperty */

struct OdDbRadialDimensionLargeJogAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRadialDimensionLargeJogAngleProperty>::createObject();
    ((OdDbRadialDimensionLargeJogAngleProperty*)res.get())->init(__OD_T("JogAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbRadialDimensionLargeJogAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRadialDimensionLargeJogAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Jog angle"));
    ((OdDbRadialDimensionLargeJogAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the jog angle (DIMJOGANG system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbRadialDimensionLargeJogAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 14));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRadialDimensionLargeJogAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->jogAngle());
  return eOk;
}

OdResult OdDbRadialDimensionLargeJogAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setJogAngle(val);
  return eOk;
}

/* OdDbRadialDimensionLargeChordPointProperty */

struct OdDbRadialDimensionLargeChordPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRadialDimensionLargeChordPointProperty>::createObject();
    ((OdDbRadialDimensionLargeChordPointProperty*)res.get())->init(__OD_T("ChordPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    ((OdDbRadialDimensionLargeChordPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the chord point for the arc"));
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

OdResult OdDbRadialDimensionLargeChordPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->chordPoint());
  return eOk;
}

OdResult OdDbRadialDimensionLargeChordPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setChordPoint(val);
  return eOk;
}

/* OdDbRadialDimensionLargeOverrideCenterProperty */

struct OdDbRadialDimensionLargeOverrideCenterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRadialDimensionLargeOverrideCenterProperty>::createObject();
    ((OdDbRadialDimensionLargeOverrideCenterProperty*)res.get())->init(__OD_T("OverrideCenter"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRadialDimensionLargeOverrideCenterProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Center location override X;Center location override Y"));
    ((OdDbRadialDimensionLargeOverrideCenterProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the override center location or pick point"));
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 12));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"BaseOffsetX"));
    res = OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 12));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"BaseOffsetY"));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRadialDimensionLargeOverrideCenterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->overrideCenter());
  return eOk;
}

OdResult OdDbRadialDimensionLargeOverrideCenterProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setOverrideCenter(val);
  return eOk;
}

/* OdDbRadialDimensionLargeJogPointProperty */

struct OdDbRadialDimensionLargeJogPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRadialDimensionLargeJogPointProperty>::createObject();
    ((OdDbRadialDimensionLargeJogPointProperty*)res.get())->init(__OD_T("JogPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRadialDimensionLargeJogPointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Jog location X;Jog location Y"));
    ((OdDbRadialDimensionLargeJogPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the jog location or pick point"));
    ((OdDbRadialDimensionLargeJogPointProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"JogLocation"));
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 13));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"BaseOffsetX"));
    res = OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 13));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"BaseOffsetY"));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRadialDimensionLargeJogPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->jogPoint());
  return eOk;
}

OdResult OdDbRadialDimensionLargeJogPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRadialDimensionLargePtr pObj = OdDbRadialDimensionLarge::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setJogPoint(val);
  return eOk;
}

void createOdDbRadialDimensionLargeProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRadialDimensionLargeCenterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRadialDimensionLargeJogAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRadialDimensionLargeChordPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRadialDimensionLargeOverrideCenterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRadialDimensionLargeJogPointProperty::createObject(b.owner()));
  b.add(properties.last());
}
