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
#include "DbCurve.h"
#include "DbValueTypes.h"

/* OdDbCurveEndParamProperty */

struct OdDbCurveEndParamProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCurveEndParamProperty>::createObject();
    ((OdDbCurveEndParamProperty*)res.get())->init(__OD_T("EndParam"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbCurveEndParamProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbCurveEndParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCurvePtr pObj = OdDbCurve::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double endParam = 0.0; OdResult rc = pObj->getEndParam(endParam); value = endParam;
  return rc;
}

/* OdDbCurveStartParamProperty */

struct OdDbCurveStartParamProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCurveStartParamProperty>::createObject();
    ((OdDbCurveStartParamProperty*)res.get())->init(__OD_T("StartParam"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbCurveStartParamProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbCurveStartParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCurvePtr pObj = OdDbCurve::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double startParam = 0.0; OdResult rc = pObj->getStartParam(startParam); value = startParam;
  return rc;
}

/* OdDbCurveIsPeriodicProperty */

struct OdDbCurveIsPeriodicProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCurveIsPeriodicProperty>::createObject();
    ((OdDbCurveIsPeriodicProperty*)res.get())->init(__OD_T("IsPeriodic"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbCurveIsPeriodicProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCurvePtr pObj = OdDbCurve::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isPeriodic());
  return eOk;
}

/* OdDbCurveClosedProperty */

//COMMENT: TODO: Maybe read-only property?
struct OdDbCurveClosedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCurveClosedProperty>::createObject();
    ((OdDbCurveClosedProperty*)res.get())->init(__OD_T("Closed"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCurveClosedProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Closed"));
    ((OdDbCurveClosedProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether polyline is Open or Closed. Closed draws a line segment from current position to starting point of the polyline."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbCurveClosedProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 19));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbCurveClosedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCurvePtr pObj = OdDbCurve::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->isClosed());
  return eOk;
}

//COMMENT: TODO: Maybe read-only property?
OdResult OdDbCurveClosedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>ENOTAPPLICABLE
  return eNotApplicable;
}

/* OdDbCurveAreaProperty */

struct OdDbCurveAreaProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCurveAreaProperty>::createObject();
    ((OdDbCurveAreaProperty*)res.get())->init(__OD_T("Area"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbCurveAreaProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(4)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCurveAreaProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Area"));
    ((OdDbCurveAreaProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the area"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbCurveAreaProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 16));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbCurveAreaProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCurvePtr pObj = OdDbCurve::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double area = 0.0; OdResult rc = pObj->getArea(area); value = area;
  return rc;
}

/* OdDbCurveStartPointProperty */

struct OdDbCurveStartPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCurveStartPointProperty>::createObject();
    ((OdDbCurveStartPointProperty*)res.get())->init(__OD_T("StartPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCurveStartPointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Start X;Start Y;Start Z"));
    ((OdDbCurveStartPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the start point of the ellipse"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbCurveStartPointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    ((OdDbCurveStartPointProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Position"));
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

OdResult OdDbCurveStartPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCurvePtr pObj = OdDbCurve::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGePoint3d startPoint; OdResult rc = pObj->getStartPoint(startPoint); value = startPoint;
  return rc;
}

//COMMENT: TODO: setStartPoint in children
OdResult OdDbCurveStartPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>ENOTAPPLICABLE
  return eNotApplicable;
}

/* OdDbCurveEndPointProperty */

//COMMENT: TODO: setEndPoint in children
struct OdDbCurveEndPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCurveEndPointProperty>::createObject();
    ((OdDbCurveEndPointProperty*)res.get())->init(__OD_T("EndPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbCurveEndPointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"End X;End Y;End Z"));
    ((OdDbCurveEndPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the end point of the ellipse"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbCurveEndPointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 3));
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

OdResult OdDbCurveEndPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbCurvePtr pObj = OdDbCurve::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGePoint3d endPoint; OdResult rc = pObj->getEndPoint(endPoint); value = endPoint;
  return rc;
}

//COMMENT: TODO: setEndPoint in children
OdResult OdDbCurveEndPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>ENOTAPPLICABLE
  return eNotApplicable;
}

void createOdDbCurveProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbCurveEndParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCurveStartParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCurveIsPeriodicProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCurveClosedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCurveAreaProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCurveStartPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbCurveEndPointProperty::createObject(b.owner()));
  b.add(properties.last());
}
