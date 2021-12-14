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
#include "Db2LineAngularDimension.h"
#include "DbValueTypes.h"

/* OdDb2LineAngularDimensionAngularMeasurementProperty */

struct OdDb2LineAngularDimensionAngularMeasurementProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2LineAngularDimensionAngularMeasurementProperty>::createObject();
    ((OdDb2LineAngularDimensionAngularMeasurementProperty*)res.get())->init(__OD_T("AngularMeasurement"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb2LineAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2LineAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Measurement"));
    ((OdDb2LineAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies dimension measurement value"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDb2LineAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 33));
    ((OdDb2LineAngularDimensionAngularMeasurementProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDb2LineAngularDimensionAngularMeasurementProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->measurement());
  return eOk;
}

/* OdDb2LineAngularDimensionXLine1StartProperty */

struct OdDb2LineAngularDimensionXLine1StartProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2LineAngularDimensionXLine1StartProperty>::createObject();
    ((OdDb2LineAngularDimensionXLine1StartProperty*)res.get())->init(__OD_T("XLine1Start"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDb2LineAngularDimensionXLine1StartProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine1Start());
  return eOk;
}

OdResult OdDb2LineAngularDimensionXLine1StartProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine1Start(val);
  return eOk;
}

/* OdDb2LineAngularDimensionArcPointProperty */

struct OdDb2LineAngularDimensionArcPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2LineAngularDimensionArcPointProperty>::createObject();
    ((OdDb2LineAngularDimensionArcPointProperty*)res.get())->init(__OD_T("ArcPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDb2LineAngularDimensionArcPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->arcPoint());
  return eOk;
}

OdResult OdDb2LineAngularDimensionArcPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArcPoint(val);
  return eOk;
}

/* OdDb2LineAngularDimensionXLine1EndProperty */

struct OdDb2LineAngularDimensionXLine1EndProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2LineAngularDimensionXLine1EndProperty>::createObject();
    ((OdDb2LineAngularDimensionXLine1EndProperty*)res.get())->init(__OD_T("XLine1End"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDb2LineAngularDimensionXLine1EndProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine1End());
  return eOk;
}

OdResult OdDb2LineAngularDimensionXLine1EndProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine1End(val);
  return eOk;
}

/* OdDb2LineAngularDimensionXLine2EndProperty */

struct OdDb2LineAngularDimensionXLine2EndProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2LineAngularDimensionXLine2EndProperty>::createObject();
    ((OdDb2LineAngularDimensionXLine2EndProperty*)res.get())->init(__OD_T("XLine2End"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDb2LineAngularDimensionXLine2EndProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine2End());
  return eOk;
}

OdResult OdDb2LineAngularDimensionXLine2EndProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine2End(val);
  return eOk;
}

/* OdDb2LineAngularDimensionXLine2StartProperty */

struct OdDb2LineAngularDimensionXLine2StartProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2LineAngularDimensionXLine2StartProperty>::createObject();
    ((OdDb2LineAngularDimensionXLine2StartProperty*)res.get())->init(__OD_T("XLine2Start"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDb2LineAngularDimensionXLine2StartProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine2Start());
  return eOk;
}

OdResult OdDb2LineAngularDimensionXLine2StartProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2LineAngularDimensionPtr pObj = OdDb2LineAngularDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine2Start(val);
  return eOk;
}

void createOdDb2LineAngularDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDb2LineAngularDimensionAngularMeasurementProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2LineAngularDimensionXLine1StartProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2LineAngularDimensionArcPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2LineAngularDimensionXLine1EndProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2LineAngularDimensionXLine2EndProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2LineAngularDimensionXLine2StartProperty::createObject(b.owner()));
  b.add(properties.last());
}
