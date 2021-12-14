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
#include "Db3dPolyline.h"
#include "DbValueTypes.h"

/* OdDb3dPolyline3dPolylineClosedProperty */

struct OdDb3dPolyline3dPolylineClosedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dPolyline3dPolylineClosedProperty>::createObject();
    ((OdDb3dPolyline3dPolylineClosedProperty*)res.get())->init(__OD_T("3dPolylineClosed"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb3dPolyline3dPolylineClosedProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Closed"));
    ((OdDb3dPolyline3dPolylineClosedProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether polyline is Open or Closed. Closed draws a line segment from current position to starting point of the polyline."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDb3dPolyline3dPolylineClosedProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 19));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb3dPolyline3dPolylineClosedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3dPolylinePtr pObj = OdDb3dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isClosed());
  return eOk;
}

OdResult OdDb3dPolyline3dPolylineClosedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3dPolylinePtr pObj = OdDb3dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->makeClosed(); else pObj->makeOpen();
  return eOk;
}

/* OdDb3dPolylinePoly3dTypeProperty */

struct OdDb3dPolylinePoly3dTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dPolylinePoly3dTypeProperty>::createObject();
    ((OdDb3dPolylinePoly3dTypeProperty*)res.get())->init(__OD_T("Poly3dType"), &OdRxValueType::Desc<OdDb::Poly3dType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb3dPolylinePoly3dTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Fit/Smooth"));
    ((OdDb3dPolylinePoly3dTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the type of line or surface curve fitting"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDb3dPolylinePoly3dTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 18));
    ((OdDb3dPolylinePoly3dTypeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Type"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb3dPolylinePoly3dTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3dPolylinePtr pObj = OdDb3dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDb::Poly3dType>(pObj->polyType());
  return eOk;
}

OdResult OdDb3dPolylinePoly3dTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3dPolylinePtr pObj = OdDb3dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::Poly3dType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setPolyType(val);
  return eOk;
}

// OdDb3dPolylineVerticesProperty
struct OdDb3dPolylineVerticesProperty : OdRxCollectionProperty
{
  struct I : OdRxValueIterator
  {
    OdDbObjectIteratorPtr it;
    virtual bool done() { return it->done(); }
    virtual bool next()
    {
      if (it->done())
        return false;
      it->step();
      return true;
    }
    virtual OdRxValue current() const
    {
      return it->objectId();
    }
  };
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE
  {
    OdRxValueIteratorPtr res;
    OdDb3dPolylinePtr t = OdDb3dPolyline::cast(pO);
    if (!t.isNull())
    {
      res = OdRxObjectImpl<I>::createObject();
      ((I*)res.get())->it = t->vertexIterator();
    }
    return res;
  }
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE
  {
    OdDb3dPolylinePtr t = OdDb3dPolyline::cast(pO);
    if (t.isNull())
     return eNotApplicable;
    int N = 0;
    for (OdDbObjectIteratorPtr it = t->vertexIterator(); !it->done(); it->step())
      ++N;
    count = N;
    return eOk;
  }
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dPolylineVerticesProperty>::createObject();
    ((OdDb3dPolylineVerticesProperty*)res.get())->init(__OD_T("Vertices"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb3dPolylineVerticesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Vertex"));
    ((OdDb3dPolylineVerticesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the 3D Polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDb3dPolylineVerticesProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDb3dPolylineVerticesProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
};

void createOdDb3dPolylineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDb3dPolyline3dPolylineClosedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3dPolylinePoly3dTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3dPolylineVerticesProperty::createObject(b.owner()));
  b.add(properties.last());
}
