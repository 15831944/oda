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
#include "DbCircle.h"
#include "DbValueTypes.h"

/* OdDbCircleThicknessProperty */

struct OdDbCircleThicknessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCircleThicknessProperty>::createObject();
    ((OdDbCircleThicknessProperty*)res.get())->init(__OD_T("Thickness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbCircleThicknessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCircleThicknessProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Thickness"));
    ((OdDbCircleThicknessProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the thickness of the circle"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbCircleThicknessProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 9));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbCircleThicknessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->thickness());
  return eOk;
}

OdResult OdDbCircleThicknessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setThickness(val);
  return eOk;
}

/* OdDbCircleRadiusProperty */

struct OdDbCircleRadiusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCircleRadiusProperty>::createObject();
    ((OdDbCircleRadiusProperty*)res.get())->init(__OD_T("Radius"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbCircleRadiusProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCircleRadiusProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Radius"));
    ((OdDbCircleRadiusProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the radius of the circle"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbCircleRadiusProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 3));
    ((OdDbCircleRadiusProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbCircleRadiusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->radius());
  return eOk;
}

OdResult OdDbCircleRadiusProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRadius(val);
  return eOk;
}

/* OdDbCircleCenterProperty */

struct OdDbCircleCenterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCircleCenterProperty>::createObject();
    ((OdDbCircleCenterProperty*)res.get())->init(__OD_T("Center"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCircleCenterProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Center X;Center Y;Center Z"));
    ((OdDbCircleCenterProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the center of the circle or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbCircleCenterProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
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

OdResult OdDbCircleCenterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->center());
  return eOk;
}

OdResult OdDbCircleCenterProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCenter(val);
  return eOk;
}

/* OdDbCircleDiameterProperty */

struct OdDbCircleDiameterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCircleDiameterProperty>::createObject();
    ((OdDbCircleDiameterProperty*)res.get())->init(__OD_T("Diameter"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbCircleDiameterProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCircleDiameterProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Diameter"));
    ((OdDbCircleDiameterProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the diameter of the circle"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbCircleDiameterProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 4));
    ((OdDbCircleDiameterProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbCircleDiameterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->radius() * 2.0;
  return eOk;
}

OdResult OdDbCircleDiameterProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setRadius(val / 2.0);
  return eOk;
}

/* OdDbCircleCircumferenceProperty */

struct OdDbCircleCircumferenceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCircleCircumferenceProperty>::createObject();
    ((OdDbCircleCircumferenceProperty*)res.get())->init(__OD_T("Circumference"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbCircleCircumferenceProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCircleCircumferenceProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Circumference"));
    ((OdDbCircleCircumferenceProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the circumference of the circle"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbCircleCircumferenceProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 5));
    ((OdDbCircleCircumferenceProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbCircleCircumferenceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = Oda2PI * pObj->radius();
  return eOk;
}

OdResult OdDbCircleCircumferenceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setRadius(val / Oda2PI);
  return eOk;
}

/* OdDbCircleNormalProperty */

struct OdDbCircleNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCircleNormalProperty>::createObject();
    ((OdDbCircleNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCircleNormalProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Normal X;Normal Y;Normal Z"));
    ((OdDbCircleNormalProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the normal direction vector"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbCircleNormalProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 17));
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

OdResult OdDbCircleNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCirclePtr pObj = OdDbCircle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

void createOdDbCircleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbCircleThicknessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCircleRadiusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCircleCenterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCircleDiameterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCircleCircumferenceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCircleNormalProperty::createObject(b.owner()));
  b.add(properties.last());
}
