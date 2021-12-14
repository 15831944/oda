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
#include "DbOrdinateDimension.h"
#include "DbValueTypes.h"

/* OdDbOrdinateDimensionUsingXAxisProperty */

struct OdDbOrdinateDimensionUsingXAxisProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOrdinateDimensionUsingXAxisProperty>::createObject();
    ((OdDbOrdinateDimensionUsingXAxisProperty*)res.get())->init(__OD_T("UsingXAxis"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOrdinateDimensionUsingXAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isUsingXAxis());
  return eOk;
}

OdResult OdDbOrdinateDimensionUsingXAxisProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  (val) ? pObj->useXAxis() : pObj->useYAxis();
  return eOk;
}

/* OdDbOrdinateDimensionUsingYAxisProperty */

struct OdDbOrdinateDimensionUsingYAxisProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOrdinateDimensionUsingYAxisProperty>::createObject();
    ((OdDbOrdinateDimensionUsingYAxisProperty*)res.get())->init(__OD_T("UsingYAxis"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOrdinateDimensionUsingYAxisProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isUsingYAxis());
  return eOk;
}

OdResult OdDbOrdinateDimensionUsingYAxisProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  (val) ? pObj->useYAxis() : pObj->useXAxis();
  return eOk;
}

/* OdDbOrdinateDimensionOriginProperty */

struct OdDbOrdinateDimensionOriginProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOrdinateDimensionOriginProperty>::createObject();
    ((OdDbOrdinateDimensionOriginProperty*)res.get())->init(__OD_T("Origin"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbOrdinateDimensionOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->origin());
  return eOk;
}

OdResult OdDbOrdinateDimensionOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setOrigin(val);
  return eOk;
}

/* OdDbOrdinateDimensionLeaderEndPointProperty */

struct OdDbOrdinateDimensionLeaderEndPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOrdinateDimensionLeaderEndPointProperty>::createObject();
    ((OdDbOrdinateDimensionLeaderEndPointProperty*)res.get())->init(__OD_T("LeaderEndPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbOrdinateDimensionLeaderEndPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->leaderEndPoint());
  return eOk;
}

OdResult OdDbOrdinateDimensionLeaderEndPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderEndPoint(val);
  return eOk;
}

/* OdDbOrdinateDimensionDefiningPointProperty */

struct OdDbOrdinateDimensionDefiningPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOrdinateDimensionDefiningPointProperty>::createObject();
    ((OdDbOrdinateDimensionDefiningPointProperty*)res.get())->init(__OD_T("DefiningPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbOrdinateDimensionDefiningPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->definingPoint());
  return eOk;
}

OdResult OdDbOrdinateDimensionDefiningPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDefiningPoint(val);
  return eOk;
}

/* OdDbOrdinateDimensionExtLineLinetypeProperty */

struct OdDbOrdinateDimensionExtLineLinetypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOrdinateDimensionExtLineLinetypeProperty>::createObject();
    ((OdDbOrdinateDimensionExtLineLinetypeProperty*)res.get())->init(__OD_T("ExtLineLinetype"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbOrdinateDimensionExtLineLinetypeProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/LinetypeTableId/Items"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOrdinateDimensionExtLineLinetypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Ext line linetype"));
    ((OdDbOrdinateDimensionExtLineLinetypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies linetype for the extension line"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbOrdinateDimensionExtLineLinetypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 15));
    ((OdDbOrdinateDimensionExtLineLinetypeProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOrdinateDimensionExtLineLinetypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->getDimExt1Linetype());
  return eOk;
}

OdResult OdDbOrdinateDimensionExtLineLinetypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOrdinateDimensionPtr pObj = OdDbOrdinateDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setDimExt1Linetype(val);
  return eOk;
}

void createOdDbOrdinateDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbOrdinateDimensionUsingXAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOrdinateDimensionUsingYAxisProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOrdinateDimensionOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOrdinateDimensionLeaderEndPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOrdinateDimensionDefiningPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOrdinateDimensionExtLineLinetypeProperty::createObject(b.owner()));
  b.add(properties.last());
}
