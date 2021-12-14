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
#include "DbArc.h"
#include "DbValueTypes.h"

/* OdDbArcStartAngleProperty */

struct OdDbArcStartAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcStartAngleProperty>::createObject();
    ((OdDbArcStartAngleProperty*)res.get())->init(__OD_T("StartAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbArcStartAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbArcStartAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Start angle"));
    ((OdDbArcStartAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the start angle of the arc"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbArcStartAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 11));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbArcStartAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->startAngle());
  return eOk;
}

OdResult OdDbArcStartAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStartAngle(val);
  return eOk;
}

/* OdDbArcEndAngleProperty */

struct OdDbArcEndAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcEndAngleProperty>::createObject();
    ((OdDbArcEndAngleProperty*)res.get())->init(__OD_T("EndAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbArcEndAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbArcEndAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"End angle"));
    ((OdDbArcEndAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the end angle of the arc"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbArcEndAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 12));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbArcEndAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->endAngle());
  return eOk;
}

OdResult OdDbArcEndAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEndAngle(val);
  return eOk;
}

/* OdDbArcCenterProperty */

struct OdDbArcCenterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcCenterProperty>::createObject();
    ((OdDbArcCenterProperty*)res.get())->init(__OD_T("Center"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbArcCenterProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Center X;Center Y;Center Z"));
    ((OdDbArcCenterProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the center point of the arc or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbArcCenterProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
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

OdResult OdDbArcCenterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->center());
  return eOk;
}

OdResult OdDbArcCenterProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCenter(val);
  return eOk;
}

/* OdDbArcRadiusProperty */

struct OdDbArcRadiusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcRadiusProperty>::createObject();
    ((OdDbArcRadiusProperty*)res.get())->init(__OD_T("Radius"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbArcRadiusProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbArcRadiusProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Radius"));
    ((OdDbArcRadiusProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the radius of the arc"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbArcRadiusProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 10));
    ((OdDbArcRadiusProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbArcRadiusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->radius());
  return eOk;
}

OdResult OdDbArcRadiusProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRadius(val);
  return eOk;
}

/* OdDbArcTotalAngleProperty */

struct OdDbArcTotalAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcTotalAngleProperty>::createObject();
    ((OdDbArcTotalAngleProperty*)res.get())->init(__OD_T("TotalAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbArcTotalAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbArcTotalAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Total angle"));
    ((OdDbArcTotalAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the total angle of the arc"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbArcTotalAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 13));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbArcTotalAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = static_cast<double>(fabs(pObj->endAngle() - pObj->startAngle()));
  return eOk;
}

/* OdDbArcNormalProperty */

struct OdDbArcNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcNormalProperty>::createObject();
    ((OdDbArcNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbArcNormalProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Normal X;Normal Y;Normal Z"));
    ((OdDbArcNormalProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the normal direction vector"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbArcNormalProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 55));
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

OdResult OdDbArcNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

/* OdDbArcThicknessProperty */

struct OdDbArcThicknessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcThicknessProperty>::createObject();
    ((OdDbArcThicknessProperty*)res.get())->init(__OD_T("Thickness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbArcThicknessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbArcThicknessProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Thickness"));
    ((OdDbArcThicknessProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the thickness of the arc"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbArcThicknessProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 9));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbArcThicknessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->thickness());
  return eOk;
}

OdResult OdDbArcThicknessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setThickness(val);
  return eOk;
}

/* OdDbArcLengthProperty */

struct OdDbArcLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcLengthProperty>::createObject();
    ((OdDbArcLengthProperty*)res.get())->init(__OD_T("Length"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbArcLengthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbArcLengthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Arc length"));
    ((OdDbArcLengthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the arc length of the arc"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbArcLengthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 14));
    ((OdDbArcLengthProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbArcLengthProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ArcLength"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbArcLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcPtr pObj = OdDbArc::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double endParam, length = 0.0; OdResult rc = pObj->getEndParam(endParam); if (rc == eOk) rc = pObj->getDistAtParam(endParam, length); value = length;
  return rc;
}

void createOdDbArcProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbArcStartAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcEndAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcCenterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcRadiusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcTotalAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcThicknessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcLengthProperty::createObject(b.owner()));
  b.add(properties.last());
}
