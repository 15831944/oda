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
#include "DbRegion.h"
#include "DbValueTypes.h"

/* OdDbRegionAreaProperty */

struct OdDbRegionAreaProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRegionAreaProperty>::createObject();
    ((OdDbRegionAreaProperty*)res.get())->init(__OD_T("Area"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbRegionAreaProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(4)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRegionAreaProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Area"));
    ((OdDbRegionAreaProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the area of the region"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbRegionAreaProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    ((OdDbRegionAreaProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbRegionAreaProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRegionPtr pObj = OdDbRegion::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dArea; OdResult res = pObj->getArea(dArea); value = dArea;
  return eOk;
}

/* OdDbRegionPerimeterProperty */

struct OdDbRegionPerimeterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRegionPerimeterProperty>::createObject();
    ((OdDbRegionPerimeterProperty*)res.get())->init(__OD_T("Perimeter"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbRegionPerimeterProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbRegionPerimeterProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Perimeter"));
    ((OdDbRegionPerimeterProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the perimeter of the region"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbRegionPerimeterProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    ((OdDbRegionPerimeterProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbRegionPerimeterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRegionPtr pObj = OdDbRegion::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dPerim; OdResult rc = pObj->getPerimeter(dPerim); value = dPerim;
  return rc;
}

/* OdDbRegionIsNullProperty */

struct OdDbRegionIsNullProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRegionIsNullProperty>::createObject();
    ((OdDbRegionIsNullProperty*)res.get())->init(__OD_T("IsNull"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbRegionIsNullProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRegionPtr pObj = OdDbRegion::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isNull());
  return eOk;
}

/* OdDbRegionNumChangesProperty */

struct OdDbRegionNumChangesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRegionNumChangesProperty>::createObject();
    ((OdDbRegionNumChangesProperty*)res.get())->init(__OD_T("NumChanges"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbRegionNumChangesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRegionPtr pObj = OdDbRegion::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numChanges());
  return eOk;
}

/* OdDbRegionNormalProperty */

struct OdDbRegionNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRegionNormalProperty>::createObject();
    ((OdDbRegionNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbRegionNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRegionPtr pObj = OdDbRegion::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGeVector3d vNormal; OdResult rc = pObj->getNormal(vNormal); value = vNormal;
  return rc;
}

void createOdDbRegionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRegionAreaProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRegionPerimeterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRegionIsNullProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRegionNumChangesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRegionNormalProperty::createObject(b.owner()));
  b.add(properties.last());
}
