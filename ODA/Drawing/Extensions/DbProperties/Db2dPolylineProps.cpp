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
#include "Db2dPolyline.h"
#include "DbValueTypes.h"

/* OdDb2dPolyline2dPolylineClosedProperty */

struct OdDb2dPolyline2dPolylineClosedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dPolyline2dPolylineClosedProperty>::createObject();
    ((OdDb2dPolyline2dPolylineClosedProperty*)res.get())->init(__OD_T("2dPolylineClosed"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2dPolyline2dPolylineClosedProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Closed"));
    ((OdDb2dPolyline2dPolylineClosedProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether polyline is Open or Closed. Closed draws a line segment from current position to starting point of the polyline."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDb2dPolyline2dPolylineClosedProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 19));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dPolyline2dPolylineClosedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isClosed());
  return eOk;
}

OdResult OdDb2dPolyline2dPolylineClosedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->makeClosed(); else pObj->makeOpen();
  return eOk;
}

/* OdDb2dPolylineDefaultEndWidthProperty */

struct OdDb2dPolylineDefaultEndWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dPolylineDefaultEndWidthProperty>::createObject();
    ((OdDb2dPolylineDefaultEndWidthProperty*)res.get())->init(__OD_T("DefaultEndWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb2dPolylineDefaultEndWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dPolylineDefaultEndWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->defaultEndWidth());
  return eOk;
}

OdResult OdDb2dPolylineDefaultEndWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDefaultEndWidth(val);
  return eOk;
}

/* OdDb2dPolylineThicknessProperty */

struct OdDb2dPolylineThicknessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dPolylineThicknessProperty>::createObject();
    ((OdDb2dPolylineThicknessProperty*)res.get())->init(__OD_T("Thickness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb2dPolylineThicknessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2dPolylineThicknessProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Thickness"));
    ((OdDb2dPolylineThicknessProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the thickness of the polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDb2dPolylineThicknessProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 9));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dPolylineThicknessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->thickness());
  return eOk;
}

OdResult OdDb2dPolylineThicknessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setThickness(val);
  return eOk;
}

/* OdDb2dPolylinePoly2dTypeProperty */

struct OdDb2dPolylinePoly2dTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dPolylinePoly2dTypeProperty>::createObject();
    ((OdDb2dPolylinePoly2dTypeProperty*)res.get())->init(__OD_T("Poly2dType"), &OdRxValueType::Desc<OdDb::Poly2dType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2dPolylinePoly2dTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Fit/Smooth"));
    ((OdDb2dPolylinePoly2dTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Applies a fit curve or spline type to a 2D polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDb2dPolylinePoly2dTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 18));
    ((OdDb2dPolylinePoly2dTypeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Type"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dPolylinePoly2dTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDb::Poly2dType>(pObj->polyType());
  return eOk;
}

OdResult OdDb2dPolylinePoly2dTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::Poly2dType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setPolyType(val);
  return eOk;
}

/* OdDb2dPolylineDefaultStartWidthProperty */

struct OdDb2dPolylineDefaultStartWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dPolylineDefaultStartWidthProperty>::createObject();
    ((OdDb2dPolylineDefaultStartWidthProperty*)res.get())->init(__OD_T("DefaultStartWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb2dPolylineDefaultStartWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dPolylineDefaultStartWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->defaultStartWidth());
  return eOk;
}

OdResult OdDb2dPolylineDefaultStartWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDefaultStartWidth(val);
  return eOk;
}

/* OdDb2dPolylineNormalProperty */

struct OdDb2dPolylineNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dPolylineNormalProperty>::createObject();
    ((OdDb2dPolylineNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDb2dPolylineNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDb2dPolylineNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDb2dPolylineElevationProperty */

struct OdDb2dPolylineElevationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dPolylineElevationProperty>::createObject();
    ((OdDb2dPolylineElevationProperty*)res.get())->init(__OD_T("Elevation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb2dPolylineElevationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2dPolylineElevationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Elevation"));
    ((OdDb2dPolylineElevationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the elevation of the polyline relative to the Z axis of the objects' coordinate system"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDb2dPolylineElevationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 15));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dPolylineElevationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->elevation());
  return eOk;
}

OdResult OdDb2dPolylineElevationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setElevation(val);
  return eOk;
}

/* OdDb2dPolylineLinetypeGenerationOnProperty */

struct OdDb2dPolylineLinetypeGenerationOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dPolylineLinetypeGenerationOnProperty>::createObject();
    ((OdDb2dPolylineLinetypeGenerationOnProperty*)res.get())->init(__OD_T("LinetypeGenerationOn"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2dPolylineLinetypeGenerationOnProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Linetype generation"));
    ((OdDb2dPolylineLinetypeGenerationOnProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Generates linetype in a continuous pattern through the vertices of the polyline. When turned off, linetype is generated starting and ending with a dash at each vertex."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDb2dPolylineLinetypeGenerationOnProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 20));
    ((OdDb2dPolylineLinetypeGenerationOnProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LinetypeGeneration"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb2dPolylineLinetypeGenerationOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isLinetypeGenerationOn());
  return eOk;
}

OdResult OdDb2dPolylineLinetypeGenerationOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb2dPolylinePtr pObj = OdDb2dPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setLinetypeGenerationOn(); else pObj->setLinetypeGenerationOff();
  return eOk;
}

// OdDb2dPolylineVertices
struct OdDb2dPolylineVertices : OdRxCollectionProperty
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
    OdDb2dPolylinePtr t = OdDb2dPolyline::cast(pO);
    if (!t.isNull())
    {
      res = OdRxObjectImpl<I>::createObject();
      ((I*)res.get())->it = t->vertexIterator();
    }
    return res;
  }
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE
  {
    OdDb2dPolylinePtr t = OdDb2dPolyline::cast(pO);
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
    OdRxMemberPtr res = OdRxObjectImpl<OdDb2dPolylineVertices>::createObject();
    ((OdDb2dPolylineVertices*)res.get())->init(__OD_T("Vertices"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb2dPolylineVertices*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Vertex"));
    ((OdDb2dPolylineVertices*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDb2dPolylineVertices*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDb2dPolylineVertices*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Coordinates"));
    ((OdDb2dPolylineVertices*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
};

void createOdDb2dPolylineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDb2dPolyline2dPolylineClosedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dPolylineDefaultEndWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dPolylineThicknessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dPolylinePoly2dTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dPolylineDefaultStartWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dPolylineNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dPolylineElevationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dPolylineLinetypeGenerationOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb2dPolylineVertices::createObject(b.owner()));
  b.add(properties.last());
}
