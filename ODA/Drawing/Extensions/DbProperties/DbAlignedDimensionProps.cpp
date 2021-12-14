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
#include "DbAlignedDimension.h"
#include "DbValueTypes.h"

/* OdDbAlignedDimensionXLine2PointProperty */

struct OdDbAlignedDimensionXLine2PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAlignedDimensionXLine2PointProperty>::createObject();
    ((OdDbAlignedDimensionXLine2PointProperty*)res.get())->init(__OD_T("XLine2Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbAlignedDimensionXLine2PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAlignedDimensionPtr pObj = OdDbAlignedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine2Point());
  return eOk;
}

OdResult OdDbAlignedDimensionXLine2PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAlignedDimensionPtr pObj = OdDbAlignedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine2Point(val);
  return eOk;
}

/* OdDbAlignedDimensionXLine1PointProperty */

struct OdDbAlignedDimensionXLine1PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAlignedDimensionXLine1PointProperty>::createObject();
    ((OdDbAlignedDimensionXLine1PointProperty*)res.get())->init(__OD_T("XLine1Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbAlignedDimensionXLine1PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAlignedDimensionPtr pObj = OdDbAlignedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine1Point());
  return eOk;
}

OdResult OdDbAlignedDimensionXLine1PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAlignedDimensionPtr pObj = OdDbAlignedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine1Point(val);
  return eOk;
}

/* OdDbAlignedDimensionObliqueProperty */

struct OdDbAlignedDimensionObliqueProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAlignedDimensionObliqueProperty>::createObject();
    ((OdDbAlignedDimensionObliqueProperty*)res.get())->init(__OD_T("Oblique"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAlignedDimensionObliqueProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAlignedDimensionObliqueProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAlignedDimensionPtr pObj = OdDbAlignedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->oblique());
  return eOk;
}

OdResult OdDbAlignedDimensionObliqueProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAlignedDimensionPtr pObj = OdDbAlignedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setOblique(val);
  return eOk;
}

/* OdDbAlignedDimensionDimLinePointProperty */

struct OdDbAlignedDimensionDimLinePointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAlignedDimensionDimLinePointProperty>::createObject();
    ((OdDbAlignedDimensionDimLinePointProperty*)res.get())->init(__OD_T("DimLinePoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbAlignedDimensionDimLinePointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAlignedDimensionPtr pObj = OdDbAlignedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->dimLinePoint());
  return eOk;
}

OdResult OdDbAlignedDimensionDimLinePointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAlignedDimensionPtr pObj = OdDbAlignedDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimLinePoint(val);
  return eOk;
}

void createOdDbAlignedDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbAlignedDimensionXLine2PointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAlignedDimensionXLine1PointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAlignedDimensionObliqueProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAlignedDimensionDimLinePointProperty::createObject(b.owner()));
  b.add(properties.last());
}
