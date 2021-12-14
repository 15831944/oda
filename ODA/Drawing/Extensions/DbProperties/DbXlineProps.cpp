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
#include "DbXline.h"
#include "DbValueTypes.h"

/* OdDbXlineBasePointProperty */

struct OdDbXlineBasePointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbXlineBasePointProperty>::createObject();
    ((OdDbXlineBasePointProperty*)res.get())->init(__OD_T("BasePoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbXlineBasePointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Basepoint X;Basepoint Y;Basepoint Z"));
    ((OdDbXlineBasePointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for the base point of the Xline or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbXlineBasePointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 4));
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

OdResult OdDbXlineBasePointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbXlinePtr pObj = OdDbXline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->basePoint());
  return eOk;
}

OdResult OdDbXlineBasePointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbXlinePtr pObj = OdDbXline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBasePoint(val);
  return eOk;
}

/* OdDbXlineUnitDirProperty */

struct OdDbXlineUnitDirProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbXlineUnitDirProperty>::createObject();
    ((OdDbXlineUnitDirProperty*)res.get())->init(__OD_T("UnitDir"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbXlineUnitDirProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Direction vector X;Direction vector Y;Direction vector Z"));
    ((OdDbXlineUnitDirProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for the direction vector of the Xline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbXlineUnitDirProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 6));
    ((OdDbXlineUnitDirProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DirectionVector"));
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

OdResult OdDbXlineUnitDirProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbXlinePtr pObj = OdDbXline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->unitDir());
  return eOk;
}

OdResult OdDbXlineUnitDirProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbXlinePtr pObj = OdDbXline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setUnitDir(val);
  return eOk;
}

/* OdDbXlineSecondPointProperty */

struct OdDbXlineSecondPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbXlineSecondPointProperty>::createObject();
    ((OdDbXlineSecondPointProperty*)res.get())->init(__OD_T("SecondPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbXlineSecondPointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Second point X;Second point Y;Second point Z"));
    ((OdDbXlineSecondPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for the second point of the Xline or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbXlineSecondPointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 5));
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

OdResult OdDbXlineSecondPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbXlinePtr pObj = OdDbXline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->basePoint() + pObj->unitDir();
  return eOk;
}

OdResult OdDbXlineSecondPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbXlinePtr pObj = OdDbXline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setUnitDir(val - pObj->basePoint());
  return eOk;
}

void createOdDbXlineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbXlineBasePointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbXlineUnitDirProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbXlineSecondPointProperty::createObject(b.owner()));
  b.add(properties.last());
}
