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
#include "DbSolid.h"
#include "DbValueTypes.h"

/* OdDbSolidThicknessProperty */

struct OdDbSolidThicknessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSolidThicknessProperty>::createObject();
    ((OdDbSolidThicknessProperty*)res.get())->init(__OD_T("Thickness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbSolidThicknessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSolidThicknessProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Thickness"));
    ((OdDbSolidThicknessProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the thickness of the solid"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbSolidThicknessProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 9));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSolidThicknessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSolidPtr pObj = OdDbSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->thickness());
  return eOk;
}

OdResult OdDbSolidThicknessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSolidPtr pObj = OdDbSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setThickness(val);
  return eOk;
}

/* OdDbSolidNormalProperty */

struct OdDbSolidNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSolidNormalProperty>::createObject();
    ((OdDbSolidNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbSolidNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSolidPtr pObj = OdDbSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbSolidNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSolidPtr pObj = OdDbSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDbSolidCoordinatesProperty */

struct OdDbSolidCoordinatesProperty : OdRxIndexedProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSolidCoordinatesProperty>::createObject();
    ((OdDbSolidCoordinatesProperty*)res.get())->init(__OD_T("Coordinates"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSolidCoordinatesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Vertex X;Vertex Y;Elevation"));
    ((OdDbSolidCoordinatesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the solid"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSolidCoordinatesProperty::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
  OdDbSolidPtr pObj = OdDbSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d item;
  //>>REPLACE_FNAME
  pObj->getPointAt(index, item);
  value = static_cast<OdGePoint3d>(item);
  return eOk;
}

OdResult OdDbSolidCoordinatesProperty::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDbSolidPtr pObj = OdDbSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setPointAt(index, val);
  return eOk;
}

void createOdDbSolidProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbSolidThicknessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSolidNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSolidCoordinatesProperty::createObject(b.owner()));
  b.add(properties.last());
}
