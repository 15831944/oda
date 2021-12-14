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
#include "Db2dVertex.h"
#include "DbValueTypes.h"

/* OdDb2dVertexStartWidthProperty */

struct OdDb2dVertexStartWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dVertexStartWidthProperty>::createObject();
    ((OdDb2dVertexStartWidthProperty*)res.get())->init(__OD_T("StartWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb2dVertexStartWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2dVertexStartWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Start segment width"));
    ((OdDb2dVertexStartWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDb2dVertexStartWidthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 12));
    ((OdDb2dVertexStartWidthProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"StartSegmentWidth"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dVertexStartWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->startWidth());
  return eOk;
}

OdResult OdDb2dVertexStartWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStartWidth(val);
  return eOk;
}

/* OdDb2dVertexPositionProperty */

struct OdDb2dVertexPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dVertexPositionProperty>::createObject();
    ((OdDb2dVertexPositionProperty*)res.get())->init(__OD_T("Position"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2dVertexPositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Vertex X;Vertex Y"));
    ((OdDb2dVertexPositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the polyline"));
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 10));
    res = OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the polyline"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 11));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dVertexPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

OdResult OdDb2dVertexPositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPosition(val);
  return eOk;
}

/* OdDb2dVertexVertexTypeProperty */

struct OdDb2dVertexVertexTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dVertexVertexTypeProperty>::createObject();
    ((OdDb2dVertexVertexTypeProperty*)res.get())->init(__OD_T("VertexType"), &OdRxValueType::Desc<OdDb::Vertex2dType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDb2dVertexVertexTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::Vertex2dType>(pObj->vertexType());
  return eOk;
}

/* OdDb2dVertexEndWidthProperty */

struct OdDb2dVertexEndWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dVertexEndWidthProperty>::createObject();
    ((OdDb2dVertexEndWidthProperty*)res.get())->init(__OD_T("EndWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb2dVertexEndWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2dVertexEndWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"End segment width"));
    ((OdDb2dVertexEndWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDb2dVertexEndWidthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 13));
    ((OdDb2dVertexEndWidthProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"EndSegmentWidth"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dVertexEndWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->endWidth());
  return eOk;
}

OdResult OdDb2dVertexEndWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEndWidth(val);
  return eOk;
}

/* OdDb2dVertexTangentProperty */

struct OdDb2dVertexTangentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dVertexTangentProperty>::createObject();
    ((OdDb2dVertexTangentProperty*)res.get())->init(__OD_T("Tangent"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb2dVertexTangentProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dVertexTangentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->tangent());
  return eOk;
}

OdResult OdDb2dVertexTangentProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTangent(val);
  return eOk;
}

/* OdDb2dVertexTangentUsedProperty */

struct OdDb2dVertexTangentUsedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dVertexTangentUsedProperty>::createObject();
    ((OdDb2dVertexTangentUsedProperty*)res.get())->init(__OD_T("TangentUsed"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dVertexTangentUsedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isTangentUsed());
  return eOk;
}

OdResult OdDb2dVertexTangentUsedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->useTangent(); else pObj->ignoreTangent();
  return eOk;
}

/* OdDb2dVertexBulgeProperty */

struct OdDb2dVertexBulgeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dVertexBulgeProperty>::createObject();
    ((OdDb2dVertexBulgeProperty*)res.get())->init(__OD_T("Bulge"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb2dVertexBulgeProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dVertexBulgeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->bulge());
  return eOk;
}

OdResult OdDb2dVertexBulgeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dVertexPtr pObj = OdDb2dVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBulge(val);
  return eOk;
}

void createOdDb2dVertexProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDb2dVertexStartWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dVertexPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dVertexVertexTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dVertexEndWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dVertexTangentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dVertexTangentUsedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dVertexBulgeProperty::createObject(b.owner()));
  b.add(properties.last());
}
