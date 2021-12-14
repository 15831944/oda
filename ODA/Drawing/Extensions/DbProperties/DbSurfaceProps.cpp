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
#include "DbSurface.h"
#include "DbValueTypes.h"

/* OdDbSurfacePerimeterProperty */

struct OdDbSurfacePerimeterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSurfacePerimeterProperty>::createObject();
    ((OdDbSurfacePerimeterProperty*)res.get())->init(__OD_T("Perimeter"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSurfacePerimeterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSurfacePtr pObj = OdDbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dRegionPerimeter; OdResult rc = pObj->getPerimeter(dRegionPerimeter); if (rc == eOk) value = dRegionPerimeter;
  return rc;
}

/* OdDbSurfaceVIsoLineDensityProperty */

struct OdDbSurfaceVIsoLineDensityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSurfaceVIsoLineDensityProperty>::createObject();
    ((OdDbSurfaceVIsoLineDensityProperty*)res.get())->init(__OD_T("VIsoLineDensity"), &OdRxValueType::Desc<short>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSurfaceVIsoLineDensityProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"V isolines"));
    ((OdDbSurfaceVIsoLineDensityProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of V isolines that are displayed"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbSurfaceVIsoLineDensityProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 16));
    ((OdDbSurfaceVIsoLineDensityProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"VIsolineDensity"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSurfaceVIsoLineDensityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSurfacePtr pObj = OdDbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<short>(pObj->vIsolineDensity());
  return eOk;
}

OdResult OdDbSurfaceVIsoLineDensityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSurfacePtr pObj = OdDbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setVIsolineDensity(val);
  return eOk;
}

/* OdDbSurfaceUIsoLineDensityProperty */

struct OdDbSurfaceUIsoLineDensityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSurfaceUIsoLineDensityProperty>::createObject();
    ((OdDbSurfaceUIsoLineDensityProperty*)res.get())->init(__OD_T("UIsoLineDensity"), &OdRxValueType::Desc<short>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSurfaceUIsoLineDensityProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"U isolines"));
    ((OdDbSurfaceUIsoLineDensityProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of U isolines that are displayed"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbSurfaceUIsoLineDensityProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 15));
    ((OdDbSurfaceUIsoLineDensityProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"UIsolineDensity"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSurfaceUIsoLineDensityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSurfacePtr pObj = OdDbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<short>(pObj->uIsolineDensity());
  return eOk;
}

OdResult OdDbSurfaceUIsoLineDensityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSurfacePtr pObj = OdDbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setUIsolineDensity(val);
  return eOk;
}

void createOdDbSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbSurfacePerimeterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSurfaceVIsoLineDensityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSurfaceUIsoLineDensityProperty::createObject(b.owner()));
  b.add(properties.last());
}
