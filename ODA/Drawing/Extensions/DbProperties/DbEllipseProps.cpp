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
#include "DbEllipse.h"
#include "DbValueTypes.h"

/* OdDbEllipseMinorAxisProperty */

struct OdDbEllipseMinorAxisProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseMinorAxisProperty>::createObject();
    ((OdDbEllipseMinorAxisProperty*)res.get())->init(__OD_T("MinorAxis"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEllipseMinorAxisProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Minor axis vector X;Minor axis vector Y;Minor axis vector Z"));
    ((OdDbEllipseMinorAxisProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the minor axis of the ellipse"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbEllipseMinorAxisProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 10));
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

OdResult OdDbEllipseMinorAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->minorAxis());
  return eOk;
}

/* OdDbEllipseRadiusRatioProperty */

struct OdDbEllipseRadiusRatioProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseRadiusRatioProperty>::createObject();
    ((OdDbEllipseRadiusRatioProperty*)res.get())->init(__OD_T("RadiusRatio"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbEllipseRadiusRatioProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEllipseRadiusRatioProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Radius ratio"));
    ((OdDbEllipseRadiusRatioProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the radius ratio of the ellipse"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbEllipseRadiusRatioProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 6));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEllipseRadiusRatioProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->radiusRatio());
  return eOk;
}

OdResult OdDbEllipseRadiusRatioProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRadiusRatio(val);
  return eOk;
}

/* OdDbEllipseStartAngleProperty */

struct OdDbEllipseStartAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseStartAngleProperty>::createObject();
    ((OdDbEllipseStartAngleProperty*)res.get())->init(__OD_T("StartAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbEllipseStartAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEllipseStartAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Start angle"));
    ((OdDbEllipseStartAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the start angle of the ellipse"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbEllipseStartAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 7));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEllipseStartAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->startAngle());
  return eOk;
}

OdResult OdDbEllipseStartAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStartAngle(val);
  return eOk;
}

/* OdDbEllipseCenterProperty */

struct OdDbEllipseCenterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseCenterProperty>::createObject();
    ((OdDbEllipseCenterProperty*)res.get())->init(__OD_T("Center"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEllipseCenterProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Center X;Center Y;Center Z"));
    ((OdDbEllipseCenterProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the center point of the ellipse or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbEllipseCenterProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
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

OdResult OdDbEllipseCenterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->center());
  return eOk;
}

OdResult OdDbEllipseCenterProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCenter(val);
  return eOk;
}

/* OdDbEllipseNormalProperty */

struct OdDbEllipseNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseNormalProperty>::createObject();
    ((OdDbEllipseNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbEllipseNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

/* OdDbEllipseMajorAxisProperty */

struct OdDbEllipseMajorAxisProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseMajorAxisProperty>::createObject();
    ((OdDbEllipseMajorAxisProperty*)res.get())->init(__OD_T("MajorAxis"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEllipseMajorAxisProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Major axis vector X;Major axis vector Y;Major axis vector Z"));
    ((OdDbEllipseMajorAxisProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the major axis of the ellipse"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbEllipseMajorAxisProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 9));
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

OdResult OdDbEllipseMajorAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->majorAxis());
  return eOk;
}

/* OdDbEllipseIsNullProperty */

//COMMENT: TODO: Unknown property isNull
struct OdDbEllipseIsNullProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseIsNullProperty>::createObject();
    ((OdDbEllipseIsNullProperty*)res.get())->init(__OD_T("IsNull"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

//COMMENT: TODO: Unknown member isNull
OdResult OdDbEllipseIsNullProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>ENOTAPPLICABLE
  return eNotApplicable;
}

/* OdDbEllipseMajorRadiusProperty */

struct OdDbEllipseMajorRadiusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseMajorRadiusProperty>::createObject();
    ((OdDbEllipseMajorRadiusProperty*)res.get())->init(__OD_T("MajorRadius"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbEllipseMajorRadiusProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEllipseMajorRadiusProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Major radius"));
    ((OdDbEllipseMajorRadiusProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the major radius of the ellipse"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbEllipseMajorRadiusProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 4));
    ((OdDbEllipseMajorRadiusProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEllipseMajorRadiusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->majorAxis().length();
  return eOk;
}

OdResult OdDbEllipseMajorRadiusProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  OdGePoint3d center;
  OdGeVector3d unitNormal, majorAxis;
  double radiusRatio, startAngle, endAngle;
  pObj->get(center, unitNormal, majorAxis, radiusRatio, startAngle, endAngle);
  majorAxis.setLength(val);
  pObj->set(center, unitNormal, majorAxis, radiusRatio, startAngle, endAngle);
  return eOk;
  //<<REPLACE_ALL
}

/* OdDbEllipseMinorRadiusProperty */

struct OdDbEllipseMinorRadiusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseMinorRadiusProperty>::createObject();
    ((OdDbEllipseMinorRadiusProperty*)res.get())->init(__OD_T("MinorRadius"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbEllipseMinorRadiusProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEllipseMinorRadiusProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Minor radius"));
    ((OdDbEllipseMinorRadiusProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the minor radius of the ellipse"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbEllipseMinorRadiusProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 5));
    ((OdDbEllipseMinorRadiusProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEllipseMinorRadiusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->minorAxis().length();
  return eOk;
}

OdResult OdDbEllipseMinorRadiusProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setRadiusRatio(val / pObj->majorAxis().length());
  return eOk;
}

/* OdDbEllipseEndAngleProperty */

struct OdDbEllipseEndAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseEndAngleProperty>::createObject();
    ((OdDbEllipseEndAngleProperty*)res.get())->init(__OD_T("EndAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbEllipseEndAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbEllipseEndAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"End angle"));
    ((OdDbEllipseEndAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the end angle of the ellipse"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbEllipseEndAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 8));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEllipseEndAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->endAngle());
  return eOk;
}

OdResult OdDbEllipseEndAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEndAngle(val);
  return eOk;
}

/* OdDbEllipseEndParamProperty */

struct OdDbEllipseEndParamProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseEndParamProperty>::createObject();
    ((OdDbEllipseEndParamProperty*)res.get())->init(__OD_T("EndParam"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbEllipseEndParamProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEllipseEndParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double endParam; OdResult ret = pObj->getEndParam(endParam); value = endParam;
  return eOk;
}

OdResult OdDbEllipseEndParamProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEndParam(val);
  return eOk;
}

/* OdDbEllipseStartParamProperty */

struct OdDbEllipseStartParamProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbEllipseStartParamProperty>::createObject();
    ((OdDbEllipseStartParamProperty*)res.get())->init(__OD_T("StartParam"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbEllipseStartParamProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbEllipseStartParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double startParam; OdResult ret = pObj->getStartParam(startParam); value = startParam;
  return eOk;
}

OdResult OdDbEllipseStartParamProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbEllipsePtr pObj = OdDbEllipse::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStartParam(val);
  return eOk;
}

void createOdDbEllipseProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbEllipseMinorAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseRadiusRatioProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseStartAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseCenterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseMajorAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseIsNullProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseMajorRadiusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseMinorRadiusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseEndAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseEndParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbEllipseStartParamProperty::createObject(b.owner()));
  b.add(properties.last());
}
