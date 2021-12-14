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
#include "DbRotatedDimension.h"
#include "DbValueTypes.h"

/* OdDbRotatedDimensionXLine2PointProperty */

struct OdDbRotatedDimensionXLine2PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRotatedDimensionXLine2PointProperty>::createObject();
    ((OdDbRotatedDimensionXLine2PointProperty*)res.get())->init(__OD_T("XLine2Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbRotatedDimensionXLine2PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine2Point());
  return eOk;
}

OdResult OdDbRotatedDimensionXLine2PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine2Point(val);
  return eOk;
}

/* OdDbRotatedDimensionXLine1PointProperty */

struct OdDbRotatedDimensionXLine1PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRotatedDimensionXLine1PointProperty>::createObject();
    ((OdDbRotatedDimensionXLine1PointProperty*)res.get())->init(__OD_T("XLine1Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbRotatedDimensionXLine1PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine1Point());
  return eOk;
}

OdResult OdDbRotatedDimensionXLine1PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine1Point(val);
  return eOk;
}

/* OdDbRotatedDimensionDimLinePointProperty */

struct OdDbRotatedDimensionDimLinePointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRotatedDimensionDimLinePointProperty>::createObject();
    ((OdDbRotatedDimensionDimLinePointProperty*)res.get())->init(__OD_T("DimLinePoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbRotatedDimensionDimLinePointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->dimLinePoint());
  return eOk;
}

OdResult OdDbRotatedDimensionDimLinePointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimLinePoint(val);
  return eOk;
}

/* OdDbRotatedDimensionRotationProperty */

struct OdDbRotatedDimensionRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRotatedDimensionRotationProperty>::createObject();
    ((OdDbRotatedDimensionRotationProperty*)res.get())->init(__OD_T("Rotation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbRotatedDimensionRotationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRotatedDimensionRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->rotation());
  return eOk;
}

OdResult OdDbRotatedDimensionRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRotation(val);
  return eOk;
}

/* OdDbRotatedDimensionObliqueProperty */

struct OdDbRotatedDimensionObliqueProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRotatedDimensionObliqueProperty>::createObject();
    ((OdDbRotatedDimensionObliqueProperty*)res.get())->init(__OD_T("Oblique"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbRotatedDimensionObliqueProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRotatedDimensionObliqueProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->oblique());
  return eOk;
}

OdResult OdDbRotatedDimensionObliqueProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRotatedDimensionPtr pObj = OdDbRotatedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setOblique(val);
  return eOk;
}

void createOdDbRotatedDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRotatedDimensionXLine2PointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRotatedDimensionXLine1PointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRotatedDimensionDimLinePointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRotatedDimensionRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRotatedDimensionObliqueProperty::createObject(b.owner()));
  b.add(properties.last());
}
