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
#include "Db3PointAngularDimension.h"
#include "DbValueTypes.h"

/* OdDb3PointAngularDimensionAngularMeasurementProperty */

struct OdDb3PointAngularDimensionAngularMeasurementProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3PointAngularDimensionAngularMeasurementProperty>::createObject();
    ((OdDb3PointAngularDimensionAngularMeasurementProperty*)res.get())->init(__OD_T("AngularMeasurement"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb3PointAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb3PointAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Measurement"));
    ((OdDb3PointAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies dimension measurement value"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDb3PointAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 33));
    ((OdDb3PointAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDb3PointAngularDimensionAngularMeasurementProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3PointAngularDimensionPtr pObj = OdDb3PointAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->measurement());
  return eOk;
}

/* OdDb3PointAngularDimensionXLine1PointProperty */

struct OdDb3PointAngularDimensionXLine1PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3PointAngularDimensionXLine1PointProperty>::createObject();
    ((OdDb3PointAngularDimensionXLine1PointProperty*)res.get())->init(__OD_T("XLine1Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDb3PointAngularDimensionXLine1PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3PointAngularDimensionPtr pObj = OdDb3PointAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine1Point());
  return eOk;
}

OdResult OdDb3PointAngularDimensionXLine1PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3PointAngularDimensionPtr pObj = OdDb3PointAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine1Point(val);
  return eOk;
}

/* OdDb3PointAngularDimensionArcPointProperty */

struct OdDb3PointAngularDimensionArcPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3PointAngularDimensionArcPointProperty>::createObject();
    ((OdDb3PointAngularDimensionArcPointProperty*)res.get())->init(__OD_T("ArcPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDb3PointAngularDimensionArcPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3PointAngularDimensionPtr pObj = OdDb3PointAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->arcPoint());
  return eOk;
}

OdResult OdDb3PointAngularDimensionArcPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3PointAngularDimensionPtr pObj = OdDb3PointAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArcPoint(val);
  return eOk;
}

/* OdDb3PointAngularDimensionCenterPointProperty */

struct OdDb3PointAngularDimensionCenterPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3PointAngularDimensionCenterPointProperty>::createObject();
    ((OdDb3PointAngularDimensionCenterPointProperty*)res.get())->init(__OD_T("CenterPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDb3PointAngularDimensionCenterPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3PointAngularDimensionPtr pObj = OdDb3PointAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->centerPoint());
  return eOk;
}

OdResult OdDb3PointAngularDimensionCenterPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3PointAngularDimensionPtr pObj = OdDb3PointAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCenterPoint(val);
  return eOk;
}

/* OdDb3PointAngularDimensionXLine2PointProperty */

struct OdDb3PointAngularDimensionXLine2PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3PointAngularDimensionXLine2PointProperty>::createObject();
    ((OdDb3PointAngularDimensionXLine2PointProperty*)res.get())->init(__OD_T("XLine2Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDb3PointAngularDimensionXLine2PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3PointAngularDimensionPtr pObj = OdDb3PointAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine2Point());
  return eOk;
}

OdResult OdDb3PointAngularDimensionXLine2PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3PointAngularDimensionPtr pObj = OdDb3PointAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine2Point(val);
  return eOk;
}

void createOdDb3PointAngularDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDb3PointAngularDimensionAngularMeasurementProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3PointAngularDimensionXLine1PointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3PointAngularDimensionArcPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3PointAngularDimensionCenterPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3PointAngularDimensionXLine2PointProperty::createObject(b.owner()));
  b.add(properties.last());
}
