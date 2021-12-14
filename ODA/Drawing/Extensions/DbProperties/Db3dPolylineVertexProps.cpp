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
#include "Db3dPolylineVertex.h"
#include "DbValueTypes.h"

/* OdDb3dPolylineVertexPositionProperty */

struct OdDb3dPolylineVertexPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dPolylineVertexPositionProperty>::createObject();
    ((OdDb3dPolylineVertexPositionProperty*)res.get())->init(__OD_T("Position"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDb3dPolylineVertexPositionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 11));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb3dPolylineVertexPositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position X;Position Y;Position Z"));
    ((OdDb3dPolylineVertexPositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the polyline"));
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

OdResult OdDb3dPolylineVertexPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3dPolylineVertexPtr pObj = OdDb3dPolylineVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

OdResult OdDb3dPolylineVertexPositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3dPolylineVertexPtr pObj = OdDb3dPolylineVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPosition(val);
  return eOk;
}

/* OdDb3dPolylineVertexVertexTypeProperty */

struct OdDb3dPolylineVertexVertexTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dPolylineVertexVertexTypeProperty>::createObject();
    ((OdDb3dPolylineVertexVertexTypeProperty*)res.get())->init(__OD_T("VertexType"), &OdRxValueType::Desc<OdDb::Vertex3dType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb3dPolylineVertexVertexTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3dPolylineVertexPtr pObj = OdDb3dPolylineVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::Vertex3dType>(pObj->vertexType());
  return eOk;
}

OdResult OdDb3dPolylineVertexVertexTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3dPolylineVertexPtr pObj = OdDb3dPolylineVertex::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::Vertex3dType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setVertexType(val);
  return eOk;
}

void createOdDb3dPolylineVertexProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDb3dPolylineVertexPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3dPolylineVertexVertexTypeProperty::createObject(b.owner()));
  b.add(properties.last());
}
