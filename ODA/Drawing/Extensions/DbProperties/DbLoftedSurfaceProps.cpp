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
#include "DbLoftedSurface.h"
#include "DbValueTypes.h"

/* OdDbLoftedSurfaceNumberOfCrossSectionsProperty */

struct OdDbLoftedSurfaceNumberOfCrossSectionsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLoftedSurfaceNumberOfCrossSectionsProperty>::createObject();
    ((OdDbLoftedSurfaceNumberOfCrossSectionsProperty*)res.get())->init(__OD_T("NumberOfCrossSections"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLoftedSurfaceNumberOfCrossSectionsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Number of Cross sections"));
    ((OdDbLoftedSurfaceNumberOfCrossSectionsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Displays the number of cross-section curves used"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbLoftedSurfaceNumberOfCrossSectionsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    ((OdDbLoftedSurfaceNumberOfCrossSectionsProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"NumCrossSections"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLoftedSurfaceNumberOfCrossSectionsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLoftedSurfacePtr pObj = OdDbLoftedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->numCrossSections());
  return eOk;
}

/* OdDbLoftedSurfaceNumberOfGuideCurvesProperty */

struct OdDbLoftedSurfaceNumberOfGuideCurvesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLoftedSurfaceNumberOfGuideCurvesProperty>::createObject();
    ((OdDbLoftedSurfaceNumberOfGuideCurvesProperty*)res.get())->init(__OD_T("NumberOfGuideCurves"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLoftedSurfaceNumberOfGuideCurvesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Number of Guide curves"));
    ((OdDbLoftedSurfaceNumberOfGuideCurvesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Displays the number of guide paths used"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbLoftedSurfaceNumberOfGuideCurvesProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 3));
    ((OdDbLoftedSurfaceNumberOfGuideCurvesProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"NumGuidePaths"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLoftedSurfaceNumberOfGuideCurvesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLoftedSurfacePtr pObj = OdDbLoftedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->numGuideCurves());
  return eOk;
}

/* OdDbLoftedSurfaceHasPathEntityProperty */

struct OdDbLoftedSurfaceHasPathEntityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLoftedSurfaceHasPathEntityProperty>::createObject();
    ((OdDbLoftedSurfaceHasPathEntityProperty*)res.get())->init(__OD_T("HasPathEntity"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLoftedSurfaceHasPathEntityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLoftedSurfacePtr pObj = OdDbLoftedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = !pObj->getPathEntity().isNull();
  return eOk;
}

/* OdDbLoftedSurfaceHasGuideCurvesProperty */

struct OdDbLoftedSurfaceHasGuideCurvesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLoftedSurfaceHasGuideCurvesProperty>::createObject();
    ((OdDbLoftedSurfaceHasGuideCurvesProperty*)res.get())->init(__OD_T("HasGuideCurves"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLoftedSurfaceHasGuideCurvesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLoftedSurfacePtr pObj = OdDbLoftedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->numGuideCurves() > 0;
  return eOk;
}

/* OdDbLoftedSurfaceLoftOptionsProperty */

struct OdDbLoftedSurfaceLoftOptionsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLoftedSurfaceLoftOptionsProperty>::createObject();
    ((OdDbLoftedSurfaceLoftOptionsProperty*)res.get())->init(__OD_T("LoftOptions"), &OdRxValueType::Desc<OdDbLoftOptions>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"StartDraftAngle", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Start draft angle"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the draft angle of the surface for the first cross section"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 5));
    res = OdRxSubProperty::createObject(L"EndDraftAngle", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"End draft angle"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the draft angle of the surface for the last cross section"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 6));
    res = OdRxSubProperty::createObject(L"StartDraftMagnitude", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Start magnitude"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the draft magnitude of the surface for the first cross section"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 7));
    res = OdRxSubProperty::createObject(L"EndDraftMagnitude", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"End magnitude"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the draft magnitude of the surface for the last cross section"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 8));
    res = OdRxSubProperty::createObject(L"Closed", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Closed"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specfies whether loft object is open or closed"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 22));
    res = OdRxSubProperty::createObject(L"Periodic", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Periodic"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specfies whether loft object is periodic."));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 23));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLoftedSurfaceLoftOptionsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLoftedSurfacePtr pObj = OdDbLoftedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDbLoftOptions loftOptions; pObj->getLoftOptions(loftOptions); value = loftOptions;
  return eOk;
}

OdResult OdDbLoftedSurfaceLoftOptionsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLoftedSurfacePtr pObj = OdDbLoftedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbLoftOptions val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setLoftOptions(val);
  return rc;
}

/* OdDbLoftedSurfaceSurfaceNormalsProperty */

struct OdDbLoftedSurfaceSurfaceNormalsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLoftedSurfaceSurfaceNormalsProperty>::createObject();
    ((OdDbLoftedSurfaceSurfaceNormalsProperty*)res.get())->init(__OD_T("SurfaceNormals"), &OdRxValueType::Desc<OdDbLoftOptions::NormalOption>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLoftedSurfaceSurfaceNormalsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Surface Normals"));
    ((OdDbLoftedSurfaceSurfaceNormalsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies for which cross-section curves the surface is normal to the cross section"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbLoftedSurfaceSurfaceNormalsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 4));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLoftedSurfaceSurfaceNormalsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLoftedSurfacePtr pObj = OdDbLoftedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDbLoftOptions loftOptions; pObj->getLoftOptions(loftOptions); value = loftOptions.normal();
  return eOk;
}

OdResult OdDbLoftedSurfaceSurfaceNormalsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLoftedSurfacePtr pObj = OdDbLoftedSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbLoftOptions::NormalOption val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdDbLoftOptions loftOptions; pObj->getLoftOptions(loftOptions); loftOptions.setNormal(val); pObj->setLoftOptions(loftOptions);
  return eOk;
}

void createOdDbLoftedSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbLoftedSurfaceNumberOfCrossSectionsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLoftedSurfaceNumberOfGuideCurvesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLoftedSurfaceHasPathEntityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLoftedSurfaceHasGuideCurvesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLoftedSurfaceLoftOptionsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLoftedSurfaceSurfaceNormalsProperty::createObject(b.owner()));
  b.add(properties.last());
}
