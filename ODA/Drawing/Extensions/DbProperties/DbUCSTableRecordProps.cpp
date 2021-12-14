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
#include "DbUCSTableRecord.h"
#include "DbValueTypes.h"

/* OdDbUCSTableRecordOriginProperty */

struct OdDbUCSTableRecordOriginProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUCSTableRecordOriginProperty>::createObject();
    ((OdDbUCSTableRecordOriginProperty*)res.get())->init(__OD_T("Origin"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbUCSTableRecordOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUCSTableRecordPtr pObj = OdDbUCSTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->origin());
  return eOk;
}

OdResult OdDbUCSTableRecordOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUCSTableRecordPtr pObj = OdDbUCSTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setOrigin(val);
  return eOk;
}

/* OdDbUCSTableRecordXAxisProperty */

struct OdDbUCSTableRecordXAxisProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUCSTableRecordXAxisProperty>::createObject();
    ((OdDbUCSTableRecordXAxisProperty*)res.get())->init(__OD_T("XAxis"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbUCSTableRecordXAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUCSTableRecordPtr pObj = OdDbUCSTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->xAxis());
  return eOk;
}

OdResult OdDbUCSTableRecordXAxisProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUCSTableRecordPtr pObj = OdDbUCSTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXAxis(val);
  return eOk;
}

/* OdDbUCSTableRecordYAxisProperty */

struct OdDbUCSTableRecordYAxisProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUCSTableRecordYAxisProperty>::createObject();
    ((OdDbUCSTableRecordYAxisProperty*)res.get())->init(__OD_T("YAxis"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbUCSTableRecordYAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUCSTableRecordPtr pObj = OdDbUCSTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->yAxis());
  return eOk;
}

OdResult OdDbUCSTableRecordYAxisProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUCSTableRecordPtr pObj = OdDbUCSTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setYAxis(val);
  return eOk;
}

void createOdDbUCSTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbUCSTableRecordOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUCSTableRecordXAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUCSTableRecordYAxisProperty::createObject(b.owner()));
  b.add(properties.last());
}
