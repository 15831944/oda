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
#include "DbHatch.h"
#include "DbValueTypes.h"

/* OdDbHatchGradientNameProperty */

struct OdDbHatchGradientNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchGradientNameProperty>::createObject();
    ((OdDbHatchGradientNameProperty*)res.get())->init(__OD_T("GradientName"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchGradientNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Gradient name"));
    ((OdDbHatchGradientNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the pattern name of the gradient."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchGradientNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 4));
    ((OdDbHatchGradientNameProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchGradientNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->gradientName());
  return eOk;
}

OdResult OdDbHatchGradientNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setGradient(OdDbHatch::kPreDefinedGradient, val);
  return eOk;
}

/* OdDbHatchGradientTypeProperty */

struct OdDbHatchGradientTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchGradientTypeProperty>::createObject();
    ((OdDbHatchGradientTypeProperty*)res.get())->init(__OD_T("GradientType"), &OdRxValueType::Desc<OdDbHatch::GradientPatternType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchGradientTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbHatch::GradientPatternType>(pObj->gradientType());
  return eOk;
}

/* OdDbHatchGradientAngleProperty */

struct OdDbHatchGradientAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchGradientAngleProperty>::createObject();
    ((OdDbHatchGradientAngleProperty*)res.get())->init(__OD_T("GradientAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHatchGradientAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchGradientAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Gradient Angle"));
    ((OdDbHatchGradientAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the gradient angle"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchGradientAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 17));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchGradientAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->gradientAngle());
  return eOk;
}

OdResult OdDbHatchGradientAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGradientAngle(val);
  return eOk;
}

/* OdDbHatchElevationProperty */

struct OdDbHatchElevationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchElevationProperty>::createObject();
    ((OdDbHatchElevationProperty*)res.get())->init(__OD_T("Elevation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHatchElevationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchElevationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Elevation"));
    ((OdDbHatchElevationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the elevation of the hatch relative to the Z axis of the objects' coordinate system"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHatchElevationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchElevationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->elevation());
  return eOk;
}

OdResult OdDbHatchElevationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setElevation(val);
  return eOk;
}

/* OdDbHatchGradientOneColorModeProperty */

struct OdDbHatchGradientOneColorModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchGradientOneColorModeProperty>::createObject();
    ((OdDbHatchGradientOneColorModeProperty*)res.get())->init(__OD_T("GradientOneColorMode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchGradientOneColorModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->getGradientOneColorMode());
  return eOk;
}

OdResult OdDbHatchGradientOneColorModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGradientOneColorMode(val);
  return eOk;
}

/* OdDbHatchIsSolidFillProperty */

struct OdDbHatchIsSolidFillProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchIsSolidFillProperty>::createObject();
    ((OdDbHatchIsSolidFillProperty*)res.get())->init(__OD_T("IsSolidFill"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchIsSolidFillProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isSolidFill());
  return eOk;
}

/* OdDbHatchBackgroundColorProperty */

struct OdDbHatchBackgroundColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchBackgroundColorProperty>::createObject();
    ((OdDbHatchBackgroundColorProperty*)res.get())->init(__OD_T("BackgroundColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchBackgroundColorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Background color"));
    ((OdDbHatchBackgroundColorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the background color of the hatch."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchBackgroundColorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 19));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchBackgroundColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->backgroundColor());
  return eOk;
}

OdResult OdDbHatchBackgroundColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackgroundColor(val);
  return eOk;
}

/* OdDbHatchNormalProperty */

struct OdDbHatchNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchNormalProperty>::createObject();
    ((OdDbHatchNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbHatchNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbHatchNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDbHatchNumberOfLoopsProperty */

struct OdDbHatchNumberOfLoopsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchNumberOfLoopsProperty>::createObject();
    ((OdDbHatchNumberOfLoopsProperty*)res.get())->init(__OD_T("NumberOfLoops"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchNumberOfLoopsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->numLoops());
  return eOk;
}

/* OdDbHatchHatchObjectTypeProperty */

struct OdDbHatchHatchObjectTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchHatchObjectTypeProperty>::createObject();
    ((OdDbHatchHatchObjectTypeProperty*)res.get())->init(__OD_T("HatchObjectType"), &OdRxValueType::Desc<OdDbHatch::HatchObjectType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchHatchObjectTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbHatch::HatchObjectType>(pObj->hatchObjectType());
  return eOk;
}

OdResult OdDbHatchHatchObjectTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbHatch::HatchObjectType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHatchObjectType(val);
  return eOk;
}

/* OdDbHatchOriginProperty */

struct OdDbHatchOriginProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchOriginProperty>::createObject();
    ((OdDbHatchOriginProperty*)res.get())->init(__OD_T("Origin"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchOriginProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Origin X;Origin Y"));
    ((OdDbHatchOriginProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the origin coordinates for the pattern of the hatch entity"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchOriginProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 9));
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGePoint2d>(pObj->originPoint());
  return eOk;
}

OdResult OdDbHatchOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setOriginPoint(val);
  return eOk;
}

/* OdDbHatchPatternDoubleProperty */

struct OdDbHatchPatternDoubleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchPatternDoubleProperty>::createObject();
    ((OdDbHatchPatternDoubleProperty*)res.get())->init(__OD_T("PatternDouble"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchPatternDoubleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Double"));
    ((OdDbHatchPatternDoubleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether the hatch pattern is double or not"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchPatternDoubleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 12));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchPatternDoubleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->patternDouble());
  return eOk;
}

/* OdDbHatchAreaProperty */

struct OdDbHatchAreaProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchAreaProperty>::createObject();
    ((OdDbHatchAreaProperty*)res.get())->init(__OD_T("Area"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHatchAreaProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(4)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchAreaProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Area"));
    ((OdDbHatchAreaProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the area of the hatch entity"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHatchAreaProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    ((OdDbHatchAreaProperty*)res.get())->attributes().add(OdRxCumulativeAttribute::createObject(L"Area"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchAreaProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double area = 0.0; OdResult rc = pObj->getArea(area); value = area;
  return rc;
}

/* OdDbHatchHatchStyleProperty */

struct OdDbHatchHatchStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchHatchStyleProperty>::createObject();
    ((OdDbHatchHatchStyleProperty*)res.get())->init(__OD_T("HatchStyle"), &OdRxValueType::Desc<OdDbHatch::HatchStyle>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchHatchStyleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Island detection style"));
    ((OdDbHatchHatchStyleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Sets the island display style of the hatch"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchHatchStyleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 14));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchHatchStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbHatch::HatchStyle>(pObj->hatchStyle());
  return eOk;
}

OdResult OdDbHatchHatchStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbHatch::HatchStyle val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHatchStyle(val);
  return eOk;
}

/* OdDbHatchNumberOfPatternDefinitionsProperty */

struct OdDbHatchNumberOfPatternDefinitionsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchNumberOfPatternDefinitionsProperty>::createObject();
    ((OdDbHatchNumberOfPatternDefinitionsProperty*)res.get())->init(__OD_T("NumberOfPatternDefinitions"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchNumberOfPatternDefinitionsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->numPatternDefinitions());
  return eOk;
}

/* OdDbHatchNumberOfHatchLinesProperty */

struct OdDbHatchNumberOfHatchLinesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchNumberOfHatchLinesProperty>::createObject();
    ((OdDbHatchNumberOfHatchLinesProperty*)res.get())->init(__OD_T("NumberOfHatchLines"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchNumberOfHatchLinesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->numHatchLines());
  return eOk;
}

/* OdDbHatchPatternTypeProperty */

struct OdDbHatchPatternTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchPatternTypeProperty>::createObject();
    ((OdDbHatchPatternTypeProperty*)res.get())->init(__OD_T("PatternType"), &OdRxValueType::Desc<OdDbHatch::HatchPatternType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchPatternTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Type"));
    ((OdDbHatchPatternTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the pattern type of the hatch"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchPatternTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbHatch::HatchPatternType>(pObj->patternType());
  return eOk;
}

/* OdDbHatchPatternScaleProperty */

struct OdDbHatchPatternScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchPatternScaleProperty>::createObject();
    ((OdDbHatchPatternScaleProperty*)res.get())->init(__OD_T("PatternScale"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHatchPatternScaleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchPatternScaleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Scale"));
    ((OdDbHatchPatternScaleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the pattern scale of the hatch"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchPatternScaleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 8));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchPatternScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->patternScale());
  return eOk;
}

OdResult OdDbHatchPatternScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPatternScale(val);
  return eOk;
}

/* OdDbHatchPatternNameProperty */

struct OdDbHatchPatternNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchPatternNameProperty>::createObject();
    ((OdDbHatchPatternNameProperty*)res.get())->init(__OD_T("PatternName"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchPatternNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Pattern name"));
    ((OdDbHatchPatternNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the pattern name of the hatch"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchPatternNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 3));
    ((OdDbHatchPatternNameProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchPatternNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->patternName());
  return eOk;
}

/* OdDbHatchPatternSpaceProperty */

struct OdDbHatchPatternSpaceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchPatternSpaceProperty>::createObject();
    ((OdDbHatchPatternSpaceProperty*)res.get())->init(__OD_T("PatternSpace"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHatchPatternSpaceProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchPatternSpaceProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Spacing"));
    ((OdDbHatchPatternSpaceProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the pattern space of the hatch"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchPatternSpaceProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 10));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchPatternSpaceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->patternSpace());
  return eOk;
}

OdResult OdDbHatchPatternSpaceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPatternSpace(val);
  return eOk;
}

/* OdDbHatchPatternAngleProperty */

struct OdDbHatchPatternAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchPatternAngleProperty>::createObject();
    ((OdDbHatchPatternAngleProperty*)res.get())->init(__OD_T("PatternAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHatchPatternAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchPatternAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Angle"));
    ((OdDbHatchPatternAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the pattern angle of the hatch"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchPatternAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 7));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchPatternAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->patternAngle());
  return eOk;
}

OdResult OdDbHatchPatternAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPatternAngle(val);
  return eOk;
}

/* OdDbHatchGradientShiftProperty */

struct OdDbHatchGradientShiftProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchGradientShiftProperty>::createObject();
    ((OdDbHatchGradientShiftProperty*)res.get())->init(__OD_T("GradientShift"), &OdRxValueType::Desc<float>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchGradientShiftProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<float>(pObj->gradientShift());
  return eOk;
}

OdResult OdDbHatchGradientShiftProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  float val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGradientShift(val);
  return eOk;
}

/* OdDbHatchShadeTintValueProperty */

struct OdDbHatchShadeTintValueProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchShadeTintValueProperty>::createObject();
    ((OdDbHatchShadeTintValueProperty*)res.get())->init(__OD_T("ShadeTintValue"), &OdRxValueType::Desc<float>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchShadeTintValueProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<float>(pObj->getShadeTintValue());
  return eOk;
}

OdResult OdDbHatchShadeTintValueProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  float val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShadeTintValue(val);
  return eOk;
}

/* OdDbHatchIsGradientProperty */

struct OdDbHatchIsGradientProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchIsGradientProperty>::createObject();
    ((OdDbHatchIsGradientProperty*)res.get())->init(__OD_T("IsGradient"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchIsGradientProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isGradient());
  return eOk;
}

/* OdDbHatchIsHatchProperty */

struct OdDbHatchIsHatchProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchIsHatchProperty>::createObject();
    ((OdDbHatchIsHatchProperty*)res.get())->init(__OD_T("IsHatch"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHatchIsHatchProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isHatch());
  return eOk;
}

/* OdDbHatchAssociativeProperty */

struct OdDbHatchAssociativeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchAssociativeProperty>::createObject();
    ((OdDbHatchAssociativeProperty*)res.get())->init(__OD_T("Associative"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchAssociativeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Associative"));
    ((OdDbHatchAssociativeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether the hatch is associative or not"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchAssociativeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 13));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchAssociativeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->associative());
  return eOk;
}

OdResult OdDbHatchAssociativeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAssociative(val);
  return eOk;
}

/* OdDbHatchGradientColor1Property */

struct OdDbHatchGradientColor1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchGradientColor1Property>::createObject();
    ((OdDbHatchGradientColor1Property*)res.get())->init(__OD_T("GradientColor1"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchGradientColor1Property*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Color 1"));
    ((OdDbHatchGradientColor1Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the gradient start color."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchGradientColor1Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 15));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchGradientColor1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColorArray colors;
  OdGeDoubleArray values;
  pObj->getGradientColors(colors, values);
  if (colors.size() == 0)
    return eNotApplicable;
  value = colors[0];
  return eOk;
  //<<REPLACE_ALL
}

OdResult OdDbHatchGradientColor1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  OdCmColorArray colors;
  OdGeDoubleArray values;
  pObj->getGradientColors(colors, values);
  if (colors.size() != 2)
    return eNotApplicable;
  colors[0] = val;
  pObj->setGradientColors(colors.size(), colors.asArrayPtr(), values.asArrayPtr());
  return eOk;
  //<<REPLACE_ALL
}

/* OdDbHatchGradientColor2Property */

struct OdDbHatchGradientColor2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHatchGradientColor2Property>::createObject();
    ((OdDbHatchGradientColor2Property*)res.get())->init(__OD_T("GradientColor2"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHatchGradientColor2Property*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Color 2"));
    ((OdDbHatchGradientColor2Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the gradient end color."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Pattern"))
      OdRxCategory::createObject(L"Pattern", OdRxCategory::rootCategory());
    ((OdDbHatchGradientColor2Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Pattern", 16));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHatchGradientColor2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColorArray colors;
  OdGeDoubleArray values;
  pObj->getGradientColors(colors, values);
  if (colors.size() < 2)
    return eNotApplicable;
  value = colors[1];
  return eOk;
  //<<REPLACE_ALL
}

OdResult OdDbHatchGradientColor2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbHatchPtr pObj = OdDbHatch::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  OdCmColorArray colors;
  OdGeDoubleArray values;
  pObj->getGradientColors(colors, values);
  if (colors.size() != 2)
    return eNotApplicable;
  colors[1] = val;
  pObj->setGradientColors(colors.size(), colors.asArrayPtr(), values.asArrayPtr());
  return eOk;
  //<<REPLACE_ALL
}

void createOdDbHatchProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbHatchGradientNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchGradientTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchGradientAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchElevationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchGradientOneColorModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchIsSolidFillProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchBackgroundColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchNumberOfLoopsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchHatchObjectTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchPatternDoubleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchAreaProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchHatchStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchNumberOfPatternDefinitionsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchNumberOfHatchLinesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchPatternTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchPatternScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchPatternNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchPatternSpaceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchPatternAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchGradientShiftProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchShadeTintValueProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchIsGradientProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchIsHatchProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchAssociativeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchGradientColor1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHatchGradientColor2Property::createObject(b.owner()));
  b.add(properties.last());
}
