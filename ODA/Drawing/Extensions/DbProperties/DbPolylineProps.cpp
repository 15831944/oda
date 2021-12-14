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
#include "DbPolyline.h"
#include "DbValueTypes.h"
#include "Db3dPolylineVertex.h"

/* OdDbPolylineThicknessProperty */

struct OdDbPolylineThicknessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylineThicknessProperty>::createObject();
    ((OdDbPolylineThicknessProperty*)res.get())->init(__OD_T("Thickness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbPolylineThicknessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolylineThicknessProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Thickness"));
    ((OdDbPolylineThicknessProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the thickness of the lightweight polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbPolylineThicknessProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 9));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolylineThicknessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->thickness());
  return eOk;
}

OdResult OdDbPolylineThicknessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setThickness(val);
  return eOk;
}

/* OdDbPolylineConstantWidthProperty */

struct OdDbPolylineConstantWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylineConstantWidthProperty>::createObject();
    ((OdDbPolylineConstantWidthProperty*)res.get())->init(__OD_T("ConstantWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbPolylineConstantWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolylineConstantWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Global width"));
    ((OdDbPolylineConstantWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the constant width for the polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbPolylineConstantWidthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 14));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolylineConstantWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->getConstantWidth());
  return eOk;
}

OdResult OdDbPolylineConstantWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setConstantWidth(val);
  return eOk;
}

/* OdDbPolylinePlinegenProperty */

struct OdDbPolylinePlinegenProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylinePlinegenProperty>::createObject();
    ((OdDbPolylinePlinegenProperty*)res.get())->init(__OD_T("Plinegen"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolylinePlinegenProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Linetype generation"));
    ((OdDbPolylinePlinegenProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether linetype generation is Enabled or Disabled for the polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbPolylinePlinegenProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 20));
    ((OdDbPolylinePlinegenProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LinetypeGeneration"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolylinePlinegenProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->hasPlinegen());
  return eOk;
}

OdResult OdDbPolylinePlinegenProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPlinegen(val);
  return eOk;
}

/* OdDbPolylineElevationProperty */

struct OdDbPolylineElevationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylineElevationProperty>::createObject();
    ((OdDbPolylineElevationProperty*)res.get())->init(__OD_T("Elevation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbPolylineElevationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolylineElevationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Elevation"));
    ((OdDbPolylineElevationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the elevation of the polyline relative to the Z axis of the objects' coordinate system (Z coordinate of current vertex)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbPolylineElevationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 15));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolylineElevationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->elevation());
  return eOk;
}

OdResult OdDbPolylineElevationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setElevation(val);
  return eOk;
}

/* OdDbPolylineVertices */

struct OdDbPolylineVertices : OdRxIndexedProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylineVertices>::createObject();
    ((OdDbPolylineVertices*)res.get())->init(__OD_T("Vertices"), &OdRxValueType::Desc<OdDb3dPolylineVertexPtr>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolylineVertices*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Vertex"));
    ((OdDbPolylineVertices*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the lightweight polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbPolylineVertices*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDbPolylineVertices*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Coordinates"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolylineVertices::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb3dPolylineVertexPtr item = OdDb3dPolylineVertex::createObject();
  OdGePoint2d point2d;
  OdGePoint3d point3d;
  pObj->getPointAt(index, point2d);
  point3d.x = point2d.x;
  point3d.y = point2d.y;
  point3d.z = pObj->elevation();
  item->setPosition(point3d);
  value = item;
  return eOk;
  //<<REPLACE_ALL
}

OdResult OdDbPolylineVertices::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d pt;
  OdDb3dPolylineVertexPtr item;
  if (!(value >> item))
    return eInvalidInput;
  OdGePoint3d position = item->position();
  pt.x = position.x;
  pt.y = position.y;
  pObj->setPointAt(index, pt);
  return eOk;
  //<<REPLACE_ALL
}

/* OdDbPolylineIsOnlyLinesProperty */

struct OdDbPolylineIsOnlyLinesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylineIsOnlyLinesProperty>::createObject();
    ((OdDbPolylineIsOnlyLinesProperty*)res.get())->init(__OD_T("IsOnlyLines"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPolylineIsOnlyLinesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isOnlyLines());
  return eOk;
}

/* OdDbPolylineNormalProperty */

struct OdDbPolylineNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylineNormalProperty>::createObject();
    ((OdDbPolylineNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbPolylineNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbPolylineNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDbPolylineHasWidthProperty */

struct OdDbPolylineHasWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylineHasWidthProperty>::createObject();
    ((OdDbPolylineHasWidthProperty*)res.get())->init(__OD_T("HasWidth"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPolylineHasWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasWidth());
  return eOk;
}

/* OdDbPolylineHasBulgesProperty */

struct OdDbPolylineHasBulgesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylineHasBulgesProperty>::createObject();
    ((OdDbPolylineHasBulgesProperty*)res.get())->init(__OD_T("HasBulges"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPolylineHasBulgesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasBulges());
  return eOk;
}

/* OdDbPolylineLengthProperty */

struct OdDbPolylineLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolylineLengthProperty>::createObject();
    ((OdDbPolylineLengthProperty*)res.get())->init(__OD_T("Length"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbPolylineLengthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolylineLengthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Length"));
    ((OdDbPolylineLengthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the length of the lightweight polyline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbPolylineLengthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 17));
    ((OdDbPolylineLengthProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPolylineLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbPolylinePtr pObj = OdDbPolyline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double endParam;
  OdResult res = pObj->getEndParam(endParam);
  if (res != eOk)
    return res;
  double len;
  res = pObj->getDistAtParam(endParam, len);
  if (res != eOk)
    return res;
  value = len;
  return eOk;
  //<<REPLACE_ALL
}

void createOdDbPolylineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbPolylineThicknessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolylineConstantWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolylinePlinegenProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolylineElevationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolylineVertices::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolylineIsOnlyLinesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolylineNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolylineHasWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolylineHasBulgesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolylineLengthProperty::createObject(b.owner()));
  b.add(properties.last());
}
