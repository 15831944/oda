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
#include "DbSection.h"
#include "DbValueTypes.h"

/* OdDbSectionNormalProperty */

struct OdDbSectionNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionNormalProperty>::createObject();
    ((OdDbSectionNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSectionNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

/* OdDbSectionIndicatorTransparencyProperty */

struct OdDbSectionIndicatorTransparencyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionIndicatorTransparencyProperty>::createObject();
    ((OdDbSectionIndicatorTransparencyProperty*)res.get())->init(__OD_T("IndicatorTransparency"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSectionIndicatorTransparencyProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Plane Transparency"));
    ((OdDbSectionIndicatorTransparencyProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies transparency of section plane when shading is turned on"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Section Object"))
      OdRxCategory::createObject(L"Section Object", OdRxCategory::rootCategory());
    ((OdDbSectionIndicatorTransparencyProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Section Object", 4));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionIndicatorTransparencyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->indicatorTransparency());
  return eOk;
}

OdResult OdDbSectionIndicatorTransparencyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setIndicatorTransparency(val);
  return eOk;
}

/* OdDbSectionIndicatorFillColorProperty */

struct OdDbSectionIndicatorFillColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionIndicatorFillColorProperty>::createObject();
    ((OdDbSectionIndicatorFillColorProperty*)res.get())->init(__OD_T("IndicatorFillColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSectionIndicatorFillColorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Plane Color"));
    ((OdDbSectionIndicatorFillColorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies color of section plane when shading is turned on"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Section Object"))
      OdRxCategory::createObject(L"Section Object", OdRxCategory::rootCategory());
    ((OdDbSectionIndicatorFillColorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Section Object", 5));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionIndicatorFillColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->indicatorFillColor());
  return eOk;
}

OdResult OdDbSectionIndicatorFillColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setIndicatorFillColor(val);
  return eOk;
}

/* OdDbSectionVerticalDirectionProperty */

struct OdDbSectionVerticalDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionVerticalDirectionProperty>::createObject();
    ((OdDbSectionVerticalDirectionProperty*)res.get())->init(__OD_T("VerticalDirection"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbSectionVerticalDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->verticalDirection());
  return eOk;
}

OdResult OdDbSectionVerticalDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setVerticalDirection(val);
  return rc;
}

/* OdDbSectionStateProperty */

struct OdDbSectionStateProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionStateProperty>::createObject();
    ((OdDbSectionStateProperty*)res.get())->init(__OD_T("State"), &OdRxValueType::Desc<OdDbSection::State>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSectionStateProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Type"));
    ((OdDbSectionStateProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies section object type"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Section Object"))
      OdRxCategory::createObject(L"Section Object", OdRxCategory::rootCategory());
    ((OdDbSectionStateProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Section Object", 2));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionStateProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbSection::State>(pObj->state());
  return eOk;
}

OdResult OdDbSectionStateProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbSection::State val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setState(val);
  return eOk;
}

/* OdDbSectionNameProperty */

struct OdDbSectionNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionNameProperty>::createObject();
    ((OdDbSectionNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSectionNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Name"));
    ((OdDbSectionNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies section object name"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Section Object"))
      OdRxCategory::createObject(L"Section Object", OdRxCategory::rootCategory());
    ((OdDbSectionNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Section Object", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getName());
  return eOk;
}

OdResult OdDbSectionNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setName(val);
  return eOk;
}

/* OdDbSectionViewingDirectionProperty */

struct OdDbSectionViewingDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionViewingDirectionProperty>::createObject();
    ((OdDbSectionViewingDirectionProperty*)res.get())->init(__OD_T("ViewingDirection"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbSectionViewingDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->viewingDirection());
  return eOk;
}

OdResult OdDbSectionViewingDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setViewingDirection(val);
  return rc;
}

/* OdDbSectionNumVerticesProperty */

struct OdDbSectionNumVerticesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionNumVerticesProperty>::createObject();
    ((OdDbSectionNumVerticesProperty*)res.get())->init(__OD_T("NumVertices"), &OdRxValueType::Desc<int>::value(), owner);
    ((OdDbSectionNumVerticesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Gets the number of vertices in the section line"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSectionNumVerticesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numVertices());
  return eOk;
}

/* OdDbSectionElevationProperty */

struct OdDbSectionElevationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionElevationProperty>::createObject();
    ((OdDbSectionElevationProperty*)res.get())->init(__OD_T("Elevation"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionElevationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->elevation());
  return eOk;
}

OdResult OdDbSectionElevationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setElevation(val);
  return rc;
}

/* OdDbSectionTopPlaneProperty */

struct OdDbSectionTopPlaneProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionTopPlaneProperty>::createObject();
    ((OdDbSectionTopPlaneProperty*)res.get())->init(__OD_T("TopPlane"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbSectionTopPlaneProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSectionTopPlaneProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Top Plane"));
    ((OdDbSectionTopPlaneProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies elevation of section plane top extents relative to the object's elevation"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbSectionTopPlaneProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 10));
    ((OdDbSectionTopPlaneProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TopHeight"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionTopPlaneProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->topPlane());
  return eOk;
}

OdResult OdDbSectionTopPlaneProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setTopPlane(val);
  return rc;
}

/* OdDbSectionBottomPlaneProperty */

struct OdDbSectionBottomPlaneProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionBottomPlaneProperty>::createObject();
    ((OdDbSectionBottomPlaneProperty*)res.get())->init(__OD_T("BottomPlane"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbSectionBottomPlaneProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSectionBottomPlaneProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Bottom Plane"));
    ((OdDbSectionBottomPlaneProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies elevation of section plane bottom extents relative to the object's elevation"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbSectionBottomPlaneProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 20));
    ((OdDbSectionBottomPlaneProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"BottomHeight"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionBottomPlaneProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->bottomPlane());
  return eOk;
}

OdResult OdDbSectionBottomPlaneProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setBottomPlane(val);
  return rc;
}

/* OdDbSectionIsLiveSectionEnabledProperty */

struct OdDbSectionIsLiveSectionEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionIsLiveSectionEnabledProperty>::createObject();
    ((OdDbSectionIsLiveSectionEnabledProperty*)res.get())->init(__OD_T("IsLiveSectionEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSectionIsLiveSectionEnabledProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Live Section"));
    ((OdDbSectionIsLiveSectionEnabledProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Turns live section on or off for this section object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Section Object"))
      OdRxCategory::createObject(L"Section Object", OdRxCategory::rootCategory());
    ((OdDbSectionIsLiveSectionEnabledProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Section Object", 3));
    ((OdDbSectionIsLiveSectionEnabledProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LiveSectionEnabled"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionIsLiveSectionEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isLiveSectionEnabled());
  return eOk;
}

OdResult OdDbSectionIsLiveSectionEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->enableLiveSection(val);
  return eOk;
}

/* OdDbSectionIsSliceProperty */

struct OdDbSectionIsSliceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionIsSliceProperty>::createObject();
    ((OdDbSectionIsSliceProperty*)res.get())->init(__OD_T("IsSlice"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionIsSliceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isSlice());
  return eOk;
}

OdResult OdDbSectionIsSliceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setIsSlice(val);
  return rc;
}

/* OdDbSectionVertices */

struct OdDbSectionVertices : OdRxIndexedProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionVertices>::createObject();
    ((OdDbSectionVertices*)res.get())->init(__OD_T("Vertices"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionVertices::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d item;
  //>>REPLACE_FNAME
  pObj->getVertex(index, item);
  value = static_cast<OdGePoint3d>(item);
  return eOk;
}

OdResult OdDbSectionVertices::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  OdResult rc = pObj->setVertex(index, val);
  return rc;
}

/* OdDbSectionSettingsProperty */

struct OdDbSectionSettingsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionSettingsProperty>::createObject();
    ((OdDbSectionSettingsProperty*)res.get())->init(__OD_T("Settings"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbSectionSettingsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Gets the section settings object"));
    ((OdDbSectionSettingsProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSectionSettingsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->getSettings());
  return eOk;
}

/* OdDbSectionThicknessDepthProperty */

struct OdDbSectionThicknessDepthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionThicknessDepthProperty>::createObject();
    ((OdDbSectionThicknessDepthProperty*)res.get())->init(__OD_T("ThicknessDepth"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionThicknessDepthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->thicknessDepth());
  return eOk;
}

OdResult OdDbSectionThicknessDepthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  OdResult rc = pObj->setThicknessDepth(val);
  return rc;
}

/* OdDbSectionSectionPlaneOffsetProperty */

struct OdDbSectionSectionPlaneOffsetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSectionSectionPlaneOffsetProperty>::createObject();
    ((OdDbSectionSectionPlaneOffsetProperty*)res.get())->init(__OD_T("SectionPlaneOffset"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbSectionSectionPlaneOffsetProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSectionSectionPlaneOffsetProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Section Offset"));
    ((OdDbSectionSectionPlaneOffsetProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies off set of section plane"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbSectionSectionPlaneOffsetProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 5));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSectionSectionPlaneOffsetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->sectionPlaneOffset());
  return eOk;
}

OdResult OdDbSectionSectionPlaneOffsetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSectionPtr pObj = OdDbSection::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setSectionPlaneOffset(val);
  return rc;
}

void createOdDbSectionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbSectionNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionIndicatorTransparencyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionIndicatorFillColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionVerticalDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionStateProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionViewingDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionNumVerticesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionElevationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionTopPlaneProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionBottomPlaneProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionIsLiveSectionEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionIsSliceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionVertices::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionSettingsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionThicknessDepthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSectionSectionPlaneOffsetProperty::createObject(b.owner()));
  b.add(properties.last());
}
