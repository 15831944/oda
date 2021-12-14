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

#include "DgCurveElementProperties.h"
#include "DgMaterialTableRecord.h"
#include "DgColorTable.h"
#include "DgBRepEntityPE.h"
#include "DgDetailingSymbol.h"

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgCurveElement2d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleScale
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dLineStyleScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getLineStyleScale();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dLineStyleScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineStyleScale(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleModifiers
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dLineStyleModifiersProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getLineStyleModifiers();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dLineStyleModifiersProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubProperty::createObject(L"DashScaleDefined", OdRxValueType::Desc<bool>::value(), this));
	children.append(OdRxSubProperty::createObject(L"DashScale", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"GapScaleDefined", OdRxValueType::Desc<bool>::value(), this));
	children.append(OdRxSubProperty::createObject(L"GapScale", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"WidthDataDefined", OdRxValueType::Desc<bool>::value(), this));
	children.append(OdRxSubProperty::createObject(L"WidthMode", OdRxValueType::Desc<OdGiDgLinetypeModifiers::WidthMode>::value(), this));
	children.append(OdRxSubProperty::createObject(L"StartWidth", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"EndWidth", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"TrueWidth", OdRxValueType::Desc<bool>::value(), this));
	children.append(OdRxSubProperty::createObject(L"ShiftMode", OdRxValueType::Desc<OdGiDgLinetypeModifiers::ShiftMode>::value(), this));
	children.append(OdRxSubProperty::createObject(L"ShiftDistance", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"ShiftFraction", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"CornersMode", OdRxValueType::Desc<OdGiDgLinetypeModifiers::CornersMode>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dLineStyleModifiersProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineStyleModifiers(*rxvalue_cast<OdGiDgLinetypeModifiers>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Closed
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dClosedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->isClosed();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Periodic
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dPeriodicProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->isPeriodic();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: StartParam
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dStartParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  double dValue = 0.0;
  OdResult retVal = pObj->getStartParam(dValue);

  value = dValue;

  return retVal;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: EndParam
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dEndParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  double dValue = 0.0;
  OdResult retVal = pObj->getEndParam(dValue);

  value = dValue;

  return retVal;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: StartPoint
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dStartPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdGePoint2d ptStart;
  OdResult retVal = pObj->getStartPoint(ptStart);

  value = ptStart;

  return retVal;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: EndPoint
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement2dEndPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement2dPtr pObj = OdDgCurveElement2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdGePoint2d ptEnd;
  OdResult retVal = pObj->getEndPoint(ptEnd);

  value = ptEnd;

  return retVal;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgCurveElement3d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleScale
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dLineStyleScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getLineStyleScale();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dLineStyleScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineStyleScale(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyleModifiers
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dLineStyleModifiersProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getLineStyleModifiers();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dLineStyleModifiersProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubProperty::createObject(L"DashScaleDefined", OdRxValueType::Desc<bool>::value(), this));
	children.append(OdRxSubProperty::createObject(L"DashScale", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"GapScaleDefined", OdRxValueType::Desc<bool>::value(), this));
	children.append(OdRxSubProperty::createObject(L"GapScale", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"WidthDataDefined", OdRxValueType::Desc<bool>::value(), this));
	children.append(OdRxSubProperty::createObject(L"WidthMode", OdRxValueType::Desc<OdGiDgLinetypeModifiers::WidthMode>::value(), this));
	children.append(OdRxSubProperty::createObject(L"StartWidth", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"EndWidth", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"TrueWidth", OdRxValueType::Desc<bool>::value(), this));
	children.append(OdRxSubProperty::createObject(L"ShiftMode", OdRxValueType::Desc<OdGiDgLinetypeModifiers::ShiftMode>::value(), this));
	children.append(OdRxSubProperty::createObject(L"ShiftDistance", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"ShiftFraction", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"CornersMode", OdRxValueType::Desc<OdGiDgLinetypeModifiers::CornersMode>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dLineStyleModifiersProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineStyleModifiers(*rxvalue_cast<OdGiDgLinetypeModifiers>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Closed
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dClosedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->isClosed();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Periodic
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dPeriodicProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->isPeriodic();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: StartParam
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dStartParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  double dValue = 0.0;
  OdResult retVal = pObj->getStartParam(dValue);

  value = dValue;

  return retVal;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: EndParam
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dEndParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  double dValue = 0.0;
  OdResult retVal = pObj->getEndParam(dValue);

  value = dValue;

  return retVal;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: StartPoint
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dStartPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdGePoint3d ptStart;
  OdResult retVal = pObj->getStartPoint(ptStart);

  value = ptStart;

  return retVal;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: EndPoint
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCurveElement3dEndPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCurveElement3dPtr pObj = OdDgCurveElement3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdGePoint3d ptEnd;
  OdResult retVal = pObj->getEndPoint(ptEnd);

  value = ptEnd;

  return retVal;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgLine2d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: StartPoint
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLine2dStartPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLine2dPtr pObj = OdDgLine2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getStartPoint();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLine2dStartPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLine2dPtr pObj = OdDgLine2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setStartPoint(*rxvalue_cast<OdGePoint2d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: EndPoint
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLine2dEndPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLine2dPtr pObj = OdDgLine2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getEndPoint();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLine2dEndPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLine2dPtr pObj = OdDgLine2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setEndPoint(*rxvalue_cast<OdGePoint2d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgLine3d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: StartPoint
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLine3dStartPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLine3dPtr pObj = OdDgLine3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getStartPoint();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLine3dStartPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLine3dPtr pObj = OdDgLine3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setStartPoint(*rxvalue_cast<OdGePoint3d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: EndPoint
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLine3dEndPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgLine3dPtr pObj = OdDgLine3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getEndPoint();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgLine3dEndPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgLine3dPtr pObj = OdDgLine3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setEndPoint(*rxvalue_cast<OdGePoint3d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgArc2d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: PrimaryAxis
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dPrimaryAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getPrimaryAxis();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dPrimaryAxisProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setPrimaryAxis(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: SecondaryAxis
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dSecondaryAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSecondaryAxis();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dSecondaryAxisProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setSecondaryAxis(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: RotationAngle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dRotationAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getRotationAngle();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dRotationAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setRotationAngle(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Origin
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getOrigin();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dOriginProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setOrigin(*rxvalue_cast<OdGePoint2d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: StartAngle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dStartAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getStartAngle();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dStartAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setStartAngle(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: SweepAngle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dSweepAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSweepAngle();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc2dSweepAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc2dPtr pObj = OdDgArc2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setSweepAngle(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgArc3d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: PrimaryAxis
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dPrimaryAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getPrimaryAxis();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dPrimaryAxisProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setPrimaryAxis(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: SecondaryAxis
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dSecondaryAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSecondaryAxis();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dSecondaryAxisProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setSecondaryAxis(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Origin
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getOrigin();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dOriginProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setOrigin(*rxvalue_cast<OdGePoint3d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: StartAngle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dStartAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getStartAngle();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dStartAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setStartAngle(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: SweepAngle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dSweepAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getSweepAngle();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dSweepAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setSweepAngle(*rxvalue_cast<double>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Rotation
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getRotation();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dRotationProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubProperty::createObject(L"RotationX", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"RotationY", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"RotationZ", OdRxValueType::Desc<double>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdGeQuaternion* pValue = rxvalue_cast<OdGeQuaternion>(&value);

  if( !pValue )
    return eNotThatKindOfClass;

  pObj->setRotation(*pValue);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Normal
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgArc3dPtr pObj = OdDgArc3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdGeQuaternion rotation = pObj->getRotation();
  OdGeVector3d   vrZAxis  = OdGeVector3d::kZAxis;
  vrZAxis = rotation.rotate(vrZAxis);

  value = vrZAxis;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgArc3dNormalProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgCellHeader2d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Name
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getOrigin();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setName(*rxvalue_cast<OdString>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdDgGraphicsElementPtr getFirstCellHeaderItem( OdDgElementIteratorPtr& pIter )
{
  OdDgGraphicsElementPtr pRet;

  if( pIter.isNull() )
    return pRet;

  for(; !pIter->done(); pIter->step())
  {
    OdDgElementPtr pItem = pIter->item().openObject(OdDg::kForRead);

    if (!pItem.isNull() && pItem->isKindOf(OdDgGraphicsElement::desc()))
    {
      pRet = pItem;
      break;
    }
  }

  return pRet;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Color
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if( pObj.isNull() )
    return eNotApplicable;

  OdDgElementIteratorPtr pIter = pObj->createIterator();
  OdDgGraphicsElementPtr pFirstItem;

  if (pIter.isNull())
    pFirstItem = pObj;
  else
    pFirstItem = getFirstCellHeaderItem(pIter);

  if( pFirstItem.isNull() )
    return eNotApplicable;

  OdCmEntityColor cmColor;

  OdUInt32 uIndex = pFirstItem->getColorIndex();

  if (uIndex == OdDg::kColorByLevel)
    cmColor.setColorMethod(OdCmEntityColor::kByLayer);
  else if (uIndex == OdDg::kColorByCell)
    cmColor.setColorMethod(OdCmEntityColor::kByBlock);
  else if (uIndex < 256)
  {
    cmColor.setColorMethod(OdCmEntityColor::kByDgnIndex);
    cmColor.setColorIndex(uIndex);
  }
  else
  {
    ODCOLORREF rgbColor = pObj->getColor();
    cmColor.setColorMethod(OdCmEntityColor::kByColor);
    cmColor.setRed(ODGETRED(rgbColor));
    cmColor.setGreen(ODGETGREEN(rgbColor));
    cmColor.setBlue(ODGETBLUE(rgbColor));
  }

  value = cmColor;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdCmEntityColor* pColor = rxvalue_cast<OdCmEntityColor>(&value);

  switch (pColor->colorMethod())
  {
    case OdCmEntityColor::kByLayer:
      pObj->setColorIndex(OdDg::kColorByLevel, true); break;
    case OdCmEntityColor::kByBlock:
      pObj->setColorIndex(OdDg::kColorByCell, true); break;
    case OdCmEntityColor::kByACI:
      pObj->setColorIndex(pColor->colorIndex(), true); break; //???
    case OdCmEntityColor::kByDgnIndex:
      pObj->setColorIndex(pColor->colorIndex(), true); break;
    case OdCmEntityColor::kByPen:
      pObj->setColorIndex(pColor->colorIndex(), true); break; //???
    case OdCmEntityColor::kByColor:
    {
      ODCOLORREF curColor = ODRGB(pColor->red(), pColor->green(), pColor->blue());
      pObj->setColor(curColor, true);
    }
  }

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Level
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dLevelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementIteratorPtr pIter = pObj->createIterator();
  OdDgGraphicsElementPtr pFirstItem;

  if (pIter.isNull())
    pFirstItem = pObj;
  else
    pFirstItem = getFirstCellHeaderItem(pIter);

  if (pFirstItem.isNull())
    return eNotApplicable;

  value = pFirstItem->getLevelId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dLevelProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLevelId(*rxvalue_cast<OdDgElementId>(&value),true);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dLineStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementIteratorPtr pIter = pObj->createIterator();
  OdDgGraphicsElementPtr pFirstItem;

  if (pIter.isNull())
    pFirstItem = pObj;
  else
    pFirstItem = getFirstCellHeaderItem(pIter);

  if (pFirstItem.isNull())
    return eNotApplicable;

  value = pFirstItem->getLineStyleId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dLineStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineStyleId(*rxvalue_cast<OdDgElementId>(&value), true);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineWeight
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementIteratorPtr pIter = pObj->createIterator();
  OdDgGraphicsElementPtr pFirstItem;

  if (pIter.isNull())
    pFirstItem = pObj;
  else
    pFirstItem = getFirstCellHeaderItem(pIter);

  if (pFirstItem.isNull())
    return eNotApplicable;

  value = (OdDb::LineWeight)(pFirstItem->getLineWeight());

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineWeight((OdUInt32)(*rxvalue_cast<OdDb::LineWeight>(&value)), true);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Origin
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getName();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dOriginProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setOrigin(*rxvalue_cast<OdGePoint2d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Transformation
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dTransformationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getTransformation();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dTransformationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setTransformation(*rxvalue_cast<OdGeMatrix2d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Elements
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgCellHeader2dElementsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if( !pObj.isNull() )
  {
    if(!pObj->isKindOf(OdDgDetailingSymbolCellHeader2d::desc()) )
    {
      res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
      ((OdDgElementIdValueIterator*)res.get())->m_pIter = pObj->createIterator();
      ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = false;
    }
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dElementsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if( pObj.isNull() )
    return eNotApplicable;

  int N = 0;

  if( !pObj->isKindOf(OdDgDetailingSymbolCellHeader2d::desc()) )
  {
    for (OdDgElementIteratorPtr it = pObj->createIterator(); !it->done(); it->step())
      ++N;
  }
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: GroupedHole
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader2dGroupedHoleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader2dPtr pObj = OdDgCellHeader2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->isPolygonWithHoles();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgCellHeader3d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Name
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getName();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setName(*rxvalue_cast<OdString>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Color
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementIteratorPtr pIter = pObj->createIterator();
  OdDgGraphicsElementPtr pFirstItem;
  
  if( pIter.isNull() )
    pFirstItem = pObj;
  else
    pFirstItem = getFirstCellHeaderItem(pIter);

  if (pFirstItem.isNull())
    return eNotApplicable;

  OdCmEntityColor cmColor;

  OdUInt32 uIndex = pFirstItem->getColorIndex();

  if (uIndex == OdDg::kColorByLevel)
    cmColor.setColorMethod(OdCmEntityColor::kByLayer);
  else if (uIndex == OdDg::kColorByCell)
    cmColor.setColorMethod(OdCmEntityColor::kByBlock);
  else if (uIndex < 256)
  {
    cmColor.setColorMethod(OdCmEntityColor::kByDgnIndex);
    cmColor.setColorIndex(uIndex);
  }
  else
  {
    ODCOLORREF rgbColor = pObj->getColor();
    cmColor.setColorMethod(OdCmEntityColor::kByColor);
    cmColor.setRed(ODGETRED(rgbColor));
    cmColor.setGreen(ODGETGREEN(rgbColor));
    cmColor.setBlue(ODGETBLUE(rgbColor));
  }

  value = cmColor;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdCmEntityColor* pColor = rxvalue_cast<OdCmEntityColor>(&value);

  switch (pColor->colorMethod())
  {
  case OdCmEntityColor::kByLayer:
    pObj->setColorIndex(OdDg::kColorByLevel, true); break;
  case OdCmEntityColor::kByBlock:
    pObj->setColorIndex(OdDg::kColorByCell, true); break;
  case OdCmEntityColor::kByACI:
    pObj->setColorIndex(pColor->colorIndex(), true); break; //???
  case OdCmEntityColor::kByDgnIndex:
    pObj->setColorIndex(pColor->colorIndex(), true); break;
  case OdCmEntityColor::kByPen:
    pObj->setColorIndex(pColor->colorIndex(), true); break; //???
  case OdCmEntityColor::kByColor:
  {
    ODCOLORREF curColor = ODRGB(pColor->red(), pColor->green(), pColor->blue());
    pObj->setColor(curColor, true);
  }
  }

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Level
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dLevelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementIteratorPtr pIter = pObj->createIterator();
  OdDgGraphicsElementPtr pFirstItem;

  if (pIter.isNull())
    pFirstItem = pObj;
  else
    pFirstItem = getFirstCellHeaderItem(pIter);

  if (pFirstItem.isNull())
    return eNotApplicable;

  value = pFirstItem->getLevelId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dLevelProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLevelId(*rxvalue_cast<OdDgElementId>(&value), true);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineStyle
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dLineStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementIteratorPtr pIter = pObj->createIterator();
  OdDgGraphicsElementPtr pFirstItem;

  if (pIter.isNull())
    pFirstItem = pObj;
  else
    pFirstItem = getFirstCellHeaderItem(pIter);

  if (pFirstItem.isNull())
    return eNotApplicable;

  value = pFirstItem->getLineStyleId();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dLineStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineStyleId(*rxvalue_cast<OdDgElementId>(&value), true);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: LineWeight
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementIteratorPtr pIter = pObj->createIterator();
  OdDgGraphicsElementPtr pFirstItem;

  if (pIter.isNull())
    pFirstItem = pObj;
  else
    pFirstItem = getFirstCellHeaderItem(pIter);

  if (pFirstItem.isNull())
    return eNotApplicable;

  value = (OdDb::LineWeight)(pFirstItem->getLineWeight());

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setLineWeight((OdUInt32)(*rxvalue_cast<OdDb::LineWeight>(&value)), true);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Origin
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getOrigin();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dOriginProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  pObj->setOrigin(*rxvalue_cast<OdGePoint3d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Transformation
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dTransformationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getTransformation();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dTransformationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdGeMatrix3d matTransform = *rxvalue_cast<OdGeMatrix3d>(&value);
  pObj->setTransformation( matTransform );

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Elements
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgCellHeader3dElementsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if( !pObj.isNull() )
  {
    if( !pObj->isKindOf(OdDgDetailingSymbolCellHeader3d::desc()) && 
        (OdDgBRepEntityPEPtr(pObj)->type(pObj) == OdDgBRepEntityPE::eNotBRepEntity) 
      )
    {
      res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
      ((OdDgElementIdValueIterator*)res.get())->m_pIter = pObj->createIterator();
      ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = false;
    }
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dElementsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if( pObj.isNull() )
    return eNotApplicable;

  int N = 0;

  if( !pObj->isKindOf(OdDgDetailingSymbolCellHeader3d::desc()) &&
      (OdDgBRepEntityPEPtr(pObj)->type(pObj) == OdDgBRepEntityPE::eNotBRepEntity)
    )
  {
    for (OdDgElementIteratorPtr it = pObj->createIterator(); !it->done(); it->step())
      ++N;
  }
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: GroupedHole
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgCellHeader3dGroupedHoleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgCellHeader3dPtr pObj = OdDgCellHeader3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->isPolygonWithHoles();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgShape2d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Vertices
//-----------------------------------------------------------------------------------------------------------------------

ODDG_DEFINE_RX_VALUE_ARRAY_ITERATOR(OdGePoint2d);

OdRxValueIteratorPtr OdDgShape2dVerticesProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  res = OdRxObjectImpl<OdRxOdGePoint2dValueIterator>::createObject();

  OdDgShape2dPtr pShape = OdDgShape2d::cast(pO);

  if( !pShape.isNull() )
  {
    OdArray<OdGePoint2d> arrVertices;

    for( OdUInt32 i = 0; i < pShape->getVerticesCount(); i++ )
      arrVertices.push_back( pShape->getVertexAt(i) );

    ((OdRxOdGePoint2dValueIterator*)res.get())->init( arrVertices, 0);
  }
    
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dVerticesProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdDgShape2dPtr pShape = OdDgShape2d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  count = pShape->getVerticesCount();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dVerticesProperty::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
  OdDgShape2dPtr pShape = OdDgShape2d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pShape->getVerticesCount() )
    return eInvalidIndex;

  value = pShape->getVertexAt(index);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dVerticesProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubPropertyOfIndexedProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubPropertyOfIndexedProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dVerticesProperty::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDgShape2dPtr pShape = OdDgShape2d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pShape->getVerticesCount() )
    return eInvalidIndex;

  pShape->setVertexAt(index, *rxvalue_cast<OdGePoint2d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dVerticesProperty::subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDgShape2dPtr pShape = OdDgShape2d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pShape->getVerticesCount() )
    return eInvalidIndex;

  pShape->insertVertex(index, *rxvalue_cast<OdGePoint2d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dVerticesProperty::subRemoveValue(OdRxObject* pO, int index) const
{
  OdDgShape2dPtr pShape = OdDgShape2d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pShape->getVerticesCount() )
    return eInvalidIndex;

  pShape->removeVertexAt(index);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Material
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dMaterialProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgShape2dPtr pObj = OdDgShape2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getMaterial();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dMaterialProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgShape2dPtr pObj = OdDgShape2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdDgElementId* pId = rxvalue_cast<OdDgElementId>(&value);

  pObj->setMaterial(*pId);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: MaterialName
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgShape2dMaterialNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgShape2dPtr pObj = OdDgShape2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdString strName = OdString::kEmpty;
  OdDgElementId idMaterial = pObj->getMaterial();

  if( !idMaterial.isNull() )
  {
    OdDgMaterialTableRecordPtr pMat = idMaterial.openObject(OdDg::kForRead);
    strName = pMat->getName();
  }

  value = strName;

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgShape2dMaterialNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgShape2dPtr pObj = OdDgShape2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdString* pValue = rxvalue_cast<OdString>(&value);

  if( pValue && pValue->isEmpty() )
    pObj->setMaterial(OdString::kEmpty);
  else
    pObj->setMaterial(*pValue);

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Fill
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dFillProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgShape2dPtr pObj = OdDgShape2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  switch( pObj->getFillType() )
  {
    case OdDg::kFillColor:
    {
      value = pObj->getFillColorIndex();
    } break;

    case OdDg::kGradientFill:
    {
      value = pObj->getGradientFill();
    } break;

    case OdDg::kNoFill:
    {
      return eEmptySet;
    } break;
  }

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dFillProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgShape2dPtr pObj = OdDgShape2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdUInt32*         pColorIndex = rxvalue_cast<OdUInt32>(&value);
  const OdDgGradientFill* pGradient   = rxvalue_cast<OdDgGradientFill>(&value);

  if( pColorIndex )
    pObj->setFillColor( *pColorIndex );
  else if( pGradient )
    pObj->setGradientFill(*pGradient);
  else
    pObj->setFillType(OdDg::kNoFill);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: HatchPattern
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dHatchPatternProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgShape2dPtr pObj = OdDgShape2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( pObj->getHatchPattern().isNull() )
    return eEmptySet;

  OdDgHatchPatternPtr pHatch = pObj->getHatchPattern();
  OdRxObjectPtr       pHatchClone = pHatch;

  value = pHatchClone;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape2dHatchPatternProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgShape2dPtr pObj = OdDgShape2d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdRxObjectPtr* pHatch = rxvalue_cast<OdRxObjectPtr>(&value);

  if( (*pHatch).isNull() || !(*pHatch)->isKindOf( OdDgHatchPattern::desc()) )
    pObj->setHatchPattern( OdDgHatchPatternPtr() );
  else
  {
    OdDgHatchPatternPtr pHatchData = *pHatch;
    pObj->setHatchPattern(pHatchData);
  }

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgShape3d
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Vertices
//-----------------------------------------------------------------------------------------------------------------------

ODDG_DEFINE_RX_VALUE_ARRAY_ITERATOR(OdGePoint3d);

OdRxValueIteratorPtr OdDgShape3dVerticesProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  res = OdRxObjectImpl<OdRxOdGePoint3dValueIterator>::createObject();

  OdDgShape3dPtr pShape = OdDgShape3d::cast(pO);

  if( !pShape.isNull() )
  {
    OdArray<OdGePoint3d> arrVertices;

    for( OdUInt32 i = 0; i < pShape->getVerticesCount(); i++ )
      arrVertices.push_back( pShape->getVertexAt(i) );

    ((OdRxOdGePoint3dValueIterator*)res.get())->init( arrVertices, 0);
  }
    
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dVerticesProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdDgShape3dPtr pShape = OdDgShape3d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  count = pShape->getVerticesCount();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dVerticesProperty::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
  OdDgShape3dPtr pShape = OdDgShape3d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pShape->getVerticesCount() )
    return eInvalidIndex;

  value = pShape->getVertexAt(index);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dVerticesProperty::subChildren(OdArray<OdRxMemberPtr>& children) const
{
	// as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
	// if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
	children.append(OdRxSubPropertyOfIndexedProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubPropertyOfIndexedProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
	children.append(OdRxSubPropertyOfIndexedProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
	return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dVerticesProperty::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDgShape3dPtr pShape = OdDgShape3d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pShape->getVerticesCount() )
    return eInvalidIndex;

  pShape->setVertexAt(index, *rxvalue_cast<OdGePoint3d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dVerticesProperty::subInsertValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDgShape3dPtr pShape = OdDgShape3d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pShape->getVerticesCount() )
    return eInvalidIndex;

  pShape->insertVertex(index, *rxvalue_cast<OdGePoint3d>(&value));

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dVerticesProperty::subRemoveValue(OdRxObject* pO, int index) const
{
  OdDgShape3dPtr pShape = OdDgShape3d::cast(pO);

  if (pShape.isNull())
    return eNotApplicable;

  if( index < 0 || index >= (int)pShape->getVerticesCount() )
    return eInvalidIndex;

  pShape->removeVertexAt(index);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Material
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dMaterialProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgShape3dPtr pObj = OdDgShape3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  value = pObj->getMaterial();

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dMaterialProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgShape3dPtr pObj = OdDgShape3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdDgElementId* pId = rxvalue_cast<OdDgElementId>(&value);

  pObj->setMaterial(*pId);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: MaterialName
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgShape3dMaterialNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgShape3dPtr pObj = OdDgShape3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdString strName = OdString::kEmpty;
  OdDgElementId idMaterial = pObj->getMaterial();

  if( !idMaterial.isNull() )
  {
    OdDgMaterialTableRecordPtr pMat = idMaterial.openObject(OdDg::kForRead);
    strName = pMat->getName();
  }

  value = strName;

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
/*
OdResult OdDgShape3dMaterialNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgShape3dPtr pObj = OdDgShape3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdString* pValue = rxvalue_cast<OdString>(&value);

  if( pValue && pValue->isEmpty() )
    pObj->setMaterial(OdString::kEmpty);
  else
    pObj->setMaterial(*pValue);

  return eOk;
}
*/
//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Fill
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dFillProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgShape3dPtr pObj = OdDgShape3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  switch( pObj->getFillType() )
  {
    case OdDg::kFillColor:
    {
      OdCmEntityColor clrRet;
      OdUInt32 uIndex = pObj->getFillColorIndex();

      if( uIndex == OdDg::kColorByLevel )
        clrRet.setColorMethod( OdCmEntityColor::kByLayer );
      else if( uIndex == OdDg::kColorByCell )
        clrRet.setColorMethod( OdCmEntityColor::kByBlock );
      else if( uIndex < 256 )
      {
        clrRet.setColorMethod( OdCmEntityColor::kByDgnIndex );
        clrRet.setColorIndex( uIndex );
      }
      else
      {
        ODCOLORREF clrData = OdDgColorTable::lookupRGB( pObj->database(), uIndex );
        clrRet.setRGB( ODGETRED(clrData), ODGETGREEN(clrData), ODGETBLUE(clrData) );
      }

      value = clrRet;

    } break;

    case OdDg::kGradientFill:
    {
      value = pObj->getGradientFill();
    } break;

    case OdDg::kNoFill:
    {
      return eEmptySet;
    } break;
  }

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dFillProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgShape3dPtr pObj = OdDgShape3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdCmEntityColor*  pColor      = rxvalue_cast<OdCmEntityColor>(&value);
  const OdDgGradientFill* pGradient   = rxvalue_cast<OdDgGradientFill>(&value);

  if( pColor )
  {
    OdUInt32 uColorIndex = 0;

    switch( pColor->colorMethod() )
    {
      case OdCmEntityColor::kByLayer:
        uColorIndex = OdDg::kColorByLevel; break;
      case OdCmEntityColor::kByBlock:
        uColorIndex = OdDg::kColorByCell; break;
      case OdCmEntityColor::kByColor:
      {
        uColorIndex = OdDgColorTable::getColorIndexByRGB( pObj->database(), ODRGB( pColor->red(), pColor->green(), pColor->blue() ));
      } break;
      case OdCmEntityColor::kByDgnIndex:
      case OdCmEntityColor::kByACI:
      case OdCmEntityColor::kByPen:
      {
        uColorIndex = pColor->colorIndex();
      } break;
    }

    pObj->setFillColor(uColorIndex);
  }
  else if( pGradient )
    pObj->setGradientFill(*pGradient);
  else
    pObj->setFillType(OdDg::kNoFill);

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: HatchPattern
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dHatchPatternProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgShape3dPtr pObj = OdDgShape3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  if( pObj->getHatchPattern().isNull() )
    return eEmptySet;

  OdDgHatchPatternPtr pHatch = pObj->getHatchPattern();
  OdRxObjectPtr       pHatchClone = pHatch;

  value = pHatchClone;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgShape3dHatchPatternProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgShape3dPtr pObj = OdDgShape3d::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdRxObjectPtr* pHatch = rxvalue_cast<OdRxObjectPtr>(&value);

	if( !pHatch || (*pHatch).isNull() || !(*pHatch)->isKindOf( OdDgHatchPattern::desc()) )
    pObj->setHatchPattern( OdDgHatchPatternPtr() );
  else
  {
    OdDgHatchPatternPtr pHatchData = *pHatch;
		OdDgHatchPatternPtr pNewPattern = pHatchData->copy();
    pObj->setHatchPattern(pNewPattern);
  }

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgComplexCurve
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: Elements
//-----------------------------------------------------------------------------------------------------------------------

OdRxValueIteratorPtr OdDgComplexCurveElementsProperty::subNewValueIterator(const OdRxObject* pO) const
{
  OdRxValueIteratorPtr res;
  OdDgComplexCurvePtr pObj = OdDgComplexCurve::cast(pO);

  if (!pObj.isNull())
  {
    res = OdRxObjectImpl<OdDgElementIdValueIterator>::createObject();
    ((OdDgElementIdValueIterator*)res.get())->m_pIter = pObj->createIterator();
    ((OdDgElementIdValueIterator*)res.get())->m_bAllowNull = false;
  }
  return res;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgComplexCurveElementsProperty::subTryGetCount(const OdRxObject* pO, int& count) const
{
  OdRxValueIteratorPtr res;
  const OdDgComplexCurvePtr pObj = OdDgComplexCurve::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  int N = 0;

  for (OdDgElementIteratorPtr it = pObj->createIterator(); !it->done(); it->step())
    ++N;
  count = N;
  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgSharedCellReference
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: BlockDefinition
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSharedCellReferenceBlockDefinitionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgSharedCellReferencePtr pObj = OdDgSharedCellReference::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementId idRet;
  OdDgSharedCellDefinitionPtr pDef = pObj->findDefinition();

  if (!pDef.isNull())
    idRet = pDef->elementId();

  value = idRet;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgSharedCellReferenceBlockDefinitionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDgSharedCellReferencePtr pObj = OdDgSharedCellReference::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  const OdDgElementId* pId = rxvalue_cast<OdDgElementId>(&value);

  if (!pId)
    return eNotThatKindOfClass;

  OdDgElementPtr pElm = pId->openObject(OdDg::kForRead, true);

  if( !pElm.isNull() && pElm->isKindOf(OdDgSharedCellDefinition::desc()) )
  {
    OdDgSharedCellDefinitionPtr pDef = pElm;
    pObj->setDefinitionName(pDef->getName());
    return eOk;
  }

  return eNotThatKindOfClass;
}

//-----------------------------------------------------------------------------------------------------------------------
//                                          class: OdDgReferenceAttachmentHeader
//-----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------
//                                          property: ReferencedModel
//-----------------------------------------------------------------------------------------------------------------------

OdResult OdDgReferenceAttachmentHeaderReferencedModelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDgReferenceAttachmentHeaderPtr pObj = OdDgReferenceAttachmentHeader::cast(pO);

  if (pObj.isNull())
    return eNotApplicable;

  OdDgElementId idRet;

  OdDgReferenceAttachCDAPE* pXRefCDAPE = OdDgReferenceAttachCDAPE::cast(pObj);

  if (pXRefCDAPE)
    idRet = pXRefCDAPE->getModelIdForPropertyCollections(pObj);

  value = idRet;

  return eOk;
}

//-----------------------------------------------------------------------------------------------------------------------




