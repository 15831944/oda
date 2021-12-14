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
#include "DbShape.h"
#include "DbValueTypes.h"

/* OdDbShapeWidthFactorProperty */

struct OdDbShapeWidthFactorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapeWidthFactorProperty>::createObject();
    ((OdDbShapeWidthFactorProperty*)res.get())->init(__OD_T("WidthFactor"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbShapeWidthFactorProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbShapeWidthFactorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Width factor"));
    ((OdDbShapeWidthFactorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the width scale factor of the shape"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbShapeWidthFactorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 4));
    ((OdDbShapeWidthFactorProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ScaleFactor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbShapeWidthFactorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->widthFactor());
  return eOk;
}

OdResult OdDbShapeWidthFactorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWidthFactor(val);
  return eOk;
}

/* OdDbShapePositionProperty */

struct OdDbShapePositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapePositionProperty>::createObject();
    ((OdDbShapePositionProperty*)res.get())->init(__OD_T("Position"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbShapePositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position X;Position Y;Position Z"));
    ((OdDbShapePositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify X, Y, Z coordinate for the insertion point of the shape or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbShapePositionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    ((OdDbShapePositionProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsertionPoint"));
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

OdResult OdDbShapePositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

OdResult OdDbShapePositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPosition(val);
  return eOk;
}

/* OdDbShapeRotationProperty */

struct OdDbShapeRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapeRotationProperty>::createObject();
    ((OdDbShapeRotationProperty*)res.get())->init(__OD_T("Rotation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbShapeRotationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbShapeRotationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotation"));
    ((OdDbShapeRotationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rotation angle of the shape"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbShapeRotationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 3));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbShapeRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->rotation());
  return eOk;
}

OdResult OdDbShapeRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRotation(val);
  return eOk;
}

/* OdDbShapeSizeProperty */

struct OdDbShapeSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapeSizeProperty>::createObject();
    ((OdDbShapeSizeProperty*)res.get())->init(__OD_T("Size"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbShapeSizeProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbShapeSizeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Size"));
    ((OdDbShapeSizeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the height of the shape"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbShapeSizeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 2));
    ((OdDbShapeSizeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Height"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbShapeSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->size());
  return eOk;
}

OdResult OdDbShapeSizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSize(val);
  return eOk;
}

/* OdDbShapeNameProperty */

struct OdDbShapeNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapeNameProperty>::createObject();
    ((OdDbShapeNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbShapeNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Name"));
    ((OdDbShapeNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the name of the shape"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbShapeNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbShapeNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->name());
  return eOk;
}

/* OdDbShapeStyleIdProperty */

struct OdDbShapeStyleIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapeStyleIdProperty>::createObject();
    ((OdDbShapeStyleIdProperty*)res.get())->init(__OD_T("StyleId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbShapeStyleIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbShapeStyleIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->styleId());
  return eOk;
}

OdResult OdDbShapeStyleIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setStyleId(val);
  return rc;
}

/* OdDbShapeObliqueProperty */

struct OdDbShapeObliqueProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapeObliqueProperty>::createObject();
    ((OdDbShapeObliqueProperty*)res.get())->init(__OD_T("Oblique"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbShapeObliqueProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbShapeObliqueProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Obliquing"));
    ((OdDbShapeObliqueProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the oblique angle of the shape"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbShapeObliqueProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 5));
    ((OdDbShapeObliqueProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ObliqueAngle"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbShapeObliqueProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->oblique());
  return eOk;
}

OdResult OdDbShapeObliqueProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setOblique(val);
  return eOk;
}

/* OdDbShapeShapeNumberProperty */

struct OdDbShapeShapeNumberProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapeShapeNumberProperty>::createObject();
    ((OdDbShapeShapeNumberProperty*)res.get())->init(__OD_T("ShapeNumber"), &OdRxValueType::Desc<short>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbShapeShapeNumberProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->shapeNumber());
  return eOk;
}

OdResult OdDbShapeShapeNumberProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShapeNumber(val);
  return eOk;
}

/* OdDbShapeThicknessProperty */

struct OdDbShapeThicknessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapeThicknessProperty>::createObject();
    ((OdDbShapeThicknessProperty*)res.get())->init(__OD_T("Thickness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbShapeThicknessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbShapeThicknessProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Thickness"));
    ((OdDbShapeThicknessProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the thickness of the shape"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbShapeThicknessProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 9));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbShapeThicknessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->thickness());
  return eOk;
}

OdResult OdDbShapeThicknessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setThickness(val);
  return eOk;
}

/* OdDbShapeNormalProperty */

struct OdDbShapeNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbShapeNormalProperty>::createObject();
    ((OdDbShapeNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbShapeNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbShapeNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbShapePtr pObj = OdDbShape::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

void createOdDbShapeProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbShapeWidthFactorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbShapePositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbShapeRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbShapeSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbShapeNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbShapeStyleIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbShapeObliqueProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbShapeShapeNumberProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbShapeThicknessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbShapeNormalProperty::createObject(b.owner()));
  b.add(properties.last());
}
