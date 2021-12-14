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
#include "DbExtrudedSurface.h"
#include "DbValueTypes.h"

/* OdDbExtrudedSurfaceSweepVecProperty */

struct OdDbExtrudedSurfaceSweepVecProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbExtrudedSurfaceSweepVecProperty>::createObject();
    ((OdDbExtrudedSurfaceSweepVecProperty*)res.get())->init(__OD_T("SweepVec"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbExtrudedSurfaceSweepVecProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbExtrudedSurfacePtr pObj = OdDbExtrudedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->getSweepVec());
  return eOk;
}

OdResult OdDbExtrudedSurfaceSweepVecProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbExtrudedSurfacePtr pObj = OdDbExtrudedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSweepVec(val);
  return eOk;
}

/* OdDbExtrudedSurfaceSweepOptionsProperty */

struct OdDbExtrudedSurfaceSweepOptionsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbExtrudedSurfaceSweepOptionsProperty>::createObject();
    ((OdDbExtrudedSurfaceSweepOptionsProperty*)res.get())->init(__OD_T("SweepOptions"), &OdRxValueType::Desc<OdDbSweepOptions>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"DraftAngle", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Taper angle"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the taper angle of the extrusion"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 3));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbExtrudedSurfaceSweepOptionsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbExtrudedSurfacePtr pObj = OdDbExtrudedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDbSweepOptions sweepOptions; pObj->getSweepOptions(sweepOptions); value = sweepOptions;
  return eOk;
}

OdResult OdDbExtrudedSurfaceSweepOptionsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbExtrudedSurfacePtr pObj = OdDbExtrudedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbSweepOptions val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSweepOptions(val);
  return eOk;
}

/* OdDbExtrudedSurfaceHeightProperty */

struct OdDbExtrudedSurfaceHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbExtrudedSurfaceHeightProperty>::createObject();
    ((OdDbExtrudedSurfaceHeightProperty*)res.get())->init(__OD_T("Height"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbExtrudedSurfaceHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbExtrudedSurfaceHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Height"));
    ((OdDbExtrudedSurfaceHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the height of the extrusion"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbExtrudedSurfaceHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbExtrudedSurfaceHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbExtrudedSurfacePtr pObj = OdDbExtrudedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->getHeight());
  return eOk;
}

OdResult OdDbExtrudedSurfaceHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbExtrudedSurfacePtr pObj = OdDbExtrudedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHeight(val);
  return eOk;
}

void createOdDbExtrudedSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbExtrudedSurfaceSweepVecProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbExtrudedSurfaceSweepOptionsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbExtrudedSurfaceHeightProperty::createObject(b.owner()));
  b.add(properties.last());
}
