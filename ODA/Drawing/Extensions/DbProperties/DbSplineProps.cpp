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
#include "DbSpline.h"
#include "DbValueTypes.h"
#include "Ge/Ge.h"
#include "Ge/GeKnotVector.h"

/* OdDbSplineNumControlPointsProperty */

struct OdDbSplineNumControlPointsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineNumControlPointsProperty>::createObject();
    ((OdDbSplineNumControlPointsProperty*)res.get())->init(__OD_T("NumControlPoints"), &OdRxValueType::Desc<int>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Data Points"))
      OdRxCategory::createObject(L"Data Points", OdRxCategory::rootCategory());
    ((OdDbSplineNumControlPointsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Data Points", 1));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineNumControlPointsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Number of control points"));
    ((OdDbSplineNumControlPointsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of control points of the spline"));
    ((OdDbSplineNumControlPointsProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"NumberOfControlPoints"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSplineNumControlPointsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numControlPoints());
  return eOk;
}

/* OdDbSplineNumFitPointsProperty */

struct OdDbSplineNumFitPointsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineNumFitPointsProperty>::createObject();
    ((OdDbSplineNumFitPointsProperty*)res.get())->init(__OD_T("NumFitPoints"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineNumFitPointsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Number of fit points"));
    ((OdDbSplineNumFitPointsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of fit points of the spline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Data Points"))
      OdRxCategory::createObject(L"Data Points", OdRxCategory::rootCategory());
    ((OdDbSplineNumFitPointsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Data Points", 1));
    ((OdDbSplineNumFitPointsProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"NumberOfFitPoints"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSplineNumFitPointsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numFitPoints());
  return eOk;
}

/* OdDbSplineControlPoints */

struct OdDbSplineControlPoints : OdRxIndexedProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineControlPoints>::createObject();
    ((OdDbSplineControlPoints*)res.get())->init(__OD_T("ControlPoints"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Data Points"))
      OdRxCategory::createObject(L"Data Points", OdRxCategory::rootCategory());
    ((OdDbSplineControlPoints*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Data Points", 2));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineControlPoints*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Current Control point"));
    ((OdDbSplineControlPoints*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current control point of the spline"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSplineControlPoints::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d item;
  //>>REPLACE_FNAME
  pObj->getControlPointAt(index, item);
  value = static_cast<OdGePoint3d>(item);
  return eOk;
}

OdResult OdDbSplineControlPoints::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setControlPointAt(index, val);
  return eOk;
}

/* OdDbSplineFitPoints */

struct OdDbSplineFitPoints : OdRxIndexedProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineFitPoints>::createObject();
    ((OdDbSplineFitPoints*)res.get())->init(__OD_T("FitPoints"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineFitPoints*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Current Fit point"));
    ((OdDbSplineFitPoints*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current fit point of the spline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Data Points"))
      OdRxCategory::createObject(L"Data Points", OdRxCategory::rootCategory());
    ((OdDbSplineFitPoints*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Data Points", 2));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSplineFitPoints::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d item;
  //>>REPLACE_FNAME
  pObj->getFitPointAt(index, item);
  value = static_cast<OdGePoint3d>(item);
  return eOk;
}

OdResult OdDbSplineFitPoints::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setFitPointAt(index, val);
  return eOk;
}

/* OdDbSplineFitToleranceProperty */

struct OdDbSplineFitToleranceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineFitToleranceProperty>::createObject();
    ((OdDbSplineFitToleranceProperty*)res.get())->init(__OD_T("FitTolerance"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbSplineFitToleranceProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineFitToleranceProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Fit tolerance"));
    ((OdDbSplineFitToleranceProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the fit tolerance of the spline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbSplineFitToleranceProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 13));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSplineFitToleranceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->fitTolerance());
  return eOk;
}

OdResult OdDbSplineFitToleranceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setFitTol(val);
  return eOk;
}

/* OdDbSplineIsNullProperty */

struct OdDbSplineIsNullProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineIsNullProperty>::createObject();
    ((OdDbSplineIsNullProperty*)res.get())->init(__OD_T("IsNull"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSplineIsNullProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isNull());
  return eOk;
}

/* OdDbSplineIsRationalProperty */

struct OdDbSplineIsRationalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineIsRationalProperty>::createObject();
    ((OdDbSplineIsRationalProperty*)res.get())->init(__OD_T("IsRational"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSplineIsRationalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isRational());
  return eOk;
}

/* OdDbSplineDegreeProperty */

struct OdDbSplineDegreeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineDegreeProperty>::createObject();
    ((OdDbSplineDegreeProperty*)res.get())->init(__OD_T("Degree"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineDegreeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Degree"));
    ((OdDbSplineDegreeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the degree of the spline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbSplineDegreeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 8));
    ((OdDbSplineDegreeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Degree2"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSplineDegreeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->degree());
  return eOk;
}

/* OdDbSplineTypeProperty */

struct OdDbSplineTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineTypeProperty>::createObject();
    ((OdDbSplineTypeProperty*)res.get())->init(__OD_T("Type"), &OdRxValueType::Desc<OdDbSpline::SplineType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Method"));
    ((OdDbSplineTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether fit points or CV's are displayed when selected"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbSplineTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 7));
    ((OdDbSplineTypeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"SplineMethod"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSplineTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbSpline::SplineType>(pObj->type());
  return eOk;
}

OdResult OdDbSplineTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbSpline::SplineType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setType(val);
  return rc;
}

/* OdDbSplineHasFitDataProperty */

struct OdDbSplineHasFitDataProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineHasFitDataProperty>::createObject();
    ((OdDbSplineHasFitDataProperty*)res.get())->init(__OD_T("HasFitData"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSplineHasFitDataProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasFitData());
  return eOk;
}

/* OdDbSplineIsPlanarProperty */

struct OdDbSplineIsPlanarProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineIsPlanarProperty>::createObject();
    ((OdDbSplineIsPlanarProperty*)res.get())->init(__OD_T("IsPlanar"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineIsPlanarProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Planar"));
    ((OdDbSplineIsPlanarProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines the whether the spline is planar"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbSplineIsPlanarProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 10));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSplineIsPlanarProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isPlanar());
  return eOk;
}

/* OdDbSplineStartFitTangentProperty */

struct OdDbSplineStartFitTangentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineStartFitTangentProperty>::createObject();
    ((OdDbSplineStartFitTangentProperty*)res.get())->init(__OD_T("StartFitTangent"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineStartFitTangentProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Start X tangent vector;Start Y tangent vector;Start Z tangent vector"));
    ((OdDbSplineStartFitTangentProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for start tangent of the spline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbSplineStartFitTangentProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 11));
    ((OdDbSplineStartFitTangentProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"StartTangent"));
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

OdResult OdDbSplineStartFitTangentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGeVector3d startTangent, endTangent; OdResult rc = pObj->getFitTangents(startTangent, endTangent); value = startTangent;
  return rc;
}

OdResult OdDbSplineStartFitTangentProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdGeVector3d startTangent, endTangent; OdResult rc = pObj->getFitTangents(startTangent, endTangent); if (rc == eOk) pObj->setFitTangents(val, endTangent);
  return rc;
}

/* OdDbSplineEndFitTangentProperty */

struct OdDbSplineEndFitTangentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineEndFitTangentProperty>::createObject();
    ((OdDbSplineEndFitTangentProperty*)res.get())->init(__OD_T("EndFitTangent"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineEndFitTangentProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"End X tangent vector;End Y tangent vector;End Z tangent vector"));
    ((OdDbSplineEndFitTangentProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for end tangent of the spline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbSplineEndFitTangentProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 12));
    ((OdDbSplineEndFitTangentProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"EndTangent"));
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

OdResult OdDbSplineEndFitTangentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGeVector3d startTangent; OdGeVector3d endTangent; OdResult rc = pObj->getFitTangents(startTangent, endTangent); value = endTangent;
  return rc;
}

OdResult OdDbSplineEndFitTangentProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdGeVector3d startTangent; OdGeVector3d endTangent; OdResult rc = pObj->getFitTangents(startTangent, endTangent); if (rc == eOk) pObj->setFitTangents(startTangent, val);
  return rc;
}

/* OdDbSplineKnotParameterizationProperty */

struct OdDbSplineKnotParameterizationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineKnotParameterizationProperty>::createObject();
    ((OdDbSplineKnotParameterizationProperty*)res.get())->init(__OD_T("KnotParameterization"), &OdRxValueType::Desc<OdGe::OdGeKnotParameterization>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineKnotParameterizationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Knot parameterization"));
    ((OdDbSplineKnotParameterizationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies knot spacing when spline was created"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Data Points"))
      OdRxCategory::createObject(L"Data Points", OdRxCategory::rootCategory());
    ((OdDbSplineKnotParameterizationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Data Points", 15));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSplineKnotParameterizationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3dArray fitPoints;
  int degree;
  double fitTolerance;
  bool tangentsExist;
  OdGeVector3d startTangent, endTangent;
  OdGe::OdGeKnotParameterization knotParam;
  OdResult ret = pObj->getFitData(fitPoints, degree, fitTolerance, tangentsExist, startTangent, endTangent, knotParam);
  value = knotParam;
  return ret;
  //<<REPLACE_ALL
}

OdResult OdDbSplineKnotParameterizationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGe::OdGeKnotParameterization val;
  if (!(value >> val))
    return eInvalidInput;
  OdGePoint3dArray fitPoints;
  int degree;
  double fitTolerance;
  bool tangentsExist;
  OdGeVector3d startTangent, endTangent;
  OdGe::OdGeKnotParameterization knotParam;
  OdResult ret = pObj->getFitData(fitPoints, degree, fitTolerance, tangentsExist, startTangent, endTangent, knotParam);
  if (ret == eOk)
  {
    pObj->setFitData(fitPoints, degree, fitTolerance, startTangent, endTangent, val);
  }
  return ret;
  //<<REPLACE_ALL
}

/* OdDbSplineSplineFrameProperty */

struct OdDbSplineSplineFrameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineSplineFrameProperty>::createObject();
    ((OdDbSplineSplineFrameProperty*)res.get())->init(__OD_T("SplineFrame"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineSplineFrameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Display CV frame"));
    ((OdDbSplineSplineFrameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether displaying the CV Hull for spline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Data Points"))
      OdRxCategory::createObject(L"Data Points", OdRxCategory::rootCategory());
    ((OdDbSplineSplineFrameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Data Points", 16));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSplineSplineFrameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->cvFrame());
  return eOk;
}

OdResult OdDbSplineSplineFrameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setCvFrame(val);
  return eOk;
}

/* OdDbSplineSplineIsPeriodicProperty */

struct OdDbSplineSplineIsPeriodicProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSplineSplineIsPeriodicProperty>::createObject();
    ((OdDbSplineSplineIsPeriodicProperty*)res.get())->init(__OD_T("SplineIsPeriodic"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSplineSplineIsPeriodicProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Periodic"));
    ((OdDbSplineSplineIsPeriodicProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines if the given spline is periodic"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbSplineSplineIsPeriodicProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 16));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSplineSplineIsPeriodicProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbSplinePtr pObj = OdDbSpline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int degree;
  bool rational, closed, periodic;
  OdGePoint3dArray controlPoints;
  OdGeKnotVector knots;
  OdGeDoubleArray weights;
  double controlPtTol;
  pObj->getNurbsData(degree, rational, closed, periodic, controlPoints, knots, weights, controlPtTol);
  value = periodic;
  return eOk;
  //<<REPLACE_ALL
}

void createOdDbSplineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbSplineNumControlPointsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineNumFitPointsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineControlPoints::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineFitPoints::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineFitToleranceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineIsNullProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineIsRationalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineDegreeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineHasFitDataProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineIsPlanarProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineStartFitTangentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineEndFitTangentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineKnotParameterizationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineSplineFrameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSplineSplineIsPeriodicProperty::createObject(b.owner()));
  b.add(properties.last());
}
