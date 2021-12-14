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
#include "DbText.h"
#include "DbValueTypes.h"

/* OdDbTextRotationProperty */

struct OdDbTextRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextRotationProperty>::createObject();
    ((OdDbTextRotationProperty*)res.get())->init(__OD_T("Rotation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTextRotationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextRotationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotation"));
    ((OdDbTextRotationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rotation angle of the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbTextRotationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 30));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->rotation());
  return eOk;
}

OdResult OdDbTextRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRotation(val);
  return eOk;
}

/* OdDbTextObliqueProperty */

struct OdDbTextObliqueProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextObliqueProperty>::createObject();
    ((OdDbTextObliqueProperty*)res.get())->init(__OD_T("Oblique"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTextObliqueProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextObliqueProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Obliquing"));
    ((OdDbTextObliqueProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the oblique angle of the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbTextObliqueProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 40));
    ((OdDbTextObliqueProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ObliqueAngle"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextObliqueProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->oblique());
  return eOk;
}

OdResult OdDbTextObliqueProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setOblique(val);
  return eOk;
}

/* OdDbTextPositionProperty */

struct OdDbTextPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextPositionProperty>::createObject();
    ((OdDbTextPositionProperty*)res.get())->init(__OD_T("Position"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextPositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position X;Position Y;Position Z"));
    ((OdDbTextPositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the insertion point of the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbTextPositionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 50));
    ((OdDbTextPositionProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsertionPoint"));
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

OdResult OdDbTextPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

/* OdDbTextHeightProperty */

struct OdDbTextHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextHeightProperty>::createObject();
    ((OdDbTextHeightProperty*)res.get())->init(__OD_T("Height"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTextHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Height"));
    ((OdDbTextHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the height of the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbTextHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 25));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->height());
  return eOk;
}

OdResult OdDbTextHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHeight(val);
  return eOk;
}

/* OdDbTextIsDefaultAlignmentProperty */

struct OdDbTextIsDefaultAlignmentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextIsDefaultAlignmentProperty>::createObject();
    ((OdDbTextIsDefaultAlignmentProperty*)res.get())->init(__OD_T("IsDefaultAlignment"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbTextIsDefaultAlignmentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isDefaultAlignment());
  return eOk;
}

/* OdDbTextAlignmentPointProperty */

struct OdDbTextAlignmentPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextAlignmentPointProperty>::createObject();
    ((OdDbTextAlignmentPointProperty*)res.get())->init(__OD_T("AlignmentPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextAlignmentPointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text alignment X;Text alignment Y;Text alignment Z"));
    ((OdDbTextAlignmentPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate of the alignment point of the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbTextAlignmentPointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 45));
    ((OdDbTextAlignmentPointProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextAlignmentPoint"));
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

OdResult OdDbTextAlignmentPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->alignmentPoint());
  return eOk;
}

OdResult OdDbTextAlignmentPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAlignmentPoint(val);
  return eOk;
}

/* OdDbTextThicknessProperty */

struct OdDbTextThicknessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextThicknessProperty>::createObject();
    ((OdDbTextThicknessProperty*)res.get())->init(__OD_T("Thickness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTextThicknessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextThicknessProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Thickness"));
    ((OdDbTextThicknessProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the thickness of the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbTextThicknessProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 9));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextThicknessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->thickness());
  return eOk;
}

OdResult OdDbTextThicknessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setThickness(val);
  return eOk;
}

/* OdDbTextNormalProperty */

struct OdDbTextNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextNormalProperty>::createObject();
    ((OdDbTextNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbTextNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbTextNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDbTextHorizontalModeProperty */

struct OdDbTextHorizontalModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextHorizontalModeProperty>::createObject();
    ((OdDbTextHorizontalModeProperty*)res.get())->init(__OD_T("HorizontalMode"), &OdRxValueType::Desc<OdDb::TextHorzMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextHorizontalModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::TextHorzMode>(pObj->horizontalMode());
  return eOk;
}

OdResult OdDbTextHorizontalModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::TextHorzMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHorizontalMode(val);
  return eOk;
}

/* OdDbTextIsMirroredInYProperty */

struct OdDbTextIsMirroredInYProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextIsMirroredInYProperty>::createObject();
    ((OdDbTextIsMirroredInYProperty*)res.get())->init(__OD_T("IsMirroredInY"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextIsMirroredInYProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Upside down"));
    ((OdDbTextIsMirroredInYProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether the text is upside down or not"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbTextIsMirroredInYProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 55));
    ((OdDbTextIsMirroredInYProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"UpsideDown"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextIsMirroredInYProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isMirroredInY());
  return eOk;
}

OdResult OdDbTextIsMirroredInYProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->mirrorInY(val);
  return eOk;
}

/* OdDbTextWidthFactorProperty */

struct OdDbTextWidthFactorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextWidthFactorProperty>::createObject();
    ((OdDbTextWidthFactorProperty*)res.get())->init(__OD_T("WidthFactor"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTextWidthFactorProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextWidthFactorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Width factor"));
    ((OdDbTextWidthFactorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the width scale factor of the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbTextWidthFactorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 35));
    ((OdDbTextWidthFactorProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ScaleFactor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextWidthFactorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->widthFactor());
  return eOk;
}

OdResult OdDbTextWidthFactorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWidthFactor(val);
  return eOk;
}

/* OdDbTextVerticalModeProperty */

struct OdDbTextVerticalModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextVerticalModeProperty>::createObject();
    ((OdDbTextVerticalModeProperty*)res.get())->init(__OD_T("VerticalMode"), &OdRxValueType::Desc<OdDb::TextVertMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextVerticalModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::TextVertMode>(pObj->verticalMode());
  return eOk;
}

OdResult OdDbTextVerticalModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::TextVertMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setVerticalMode(val);
  return eOk;
}

/* OdDbTextTextStringProperty */

struct OdDbTextTextStringProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextTextStringProperty>::createObject();
    ((OdDbTextTextStringProperty*)res.get())->init(__OD_T("TextString"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextTextStringProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Contents"));
    ((OdDbTextTextStringProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text string of the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbTextTextStringProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 1));
    ((OdDbTextTextStringProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextTextStringProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->textString());
  return eOk;
}

OdResult OdDbTextTextStringProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextString(val);
  return eOk;
}

/* OdDbTextIsMirroredInXProperty */

struct OdDbTextIsMirroredInXProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextIsMirroredInXProperty>::createObject();
    ((OdDbTextIsMirroredInXProperty*)res.get())->init(__OD_T("IsMirroredInX"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextIsMirroredInXProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Backward"));
    ((OdDbTextIsMirroredInXProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether the text is backward or not"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbTextIsMirroredInXProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 60));
    ((OdDbTextIsMirroredInXProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Backward"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextIsMirroredInXProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isMirroredInX());
  return eOk;
}

OdResult OdDbTextIsMirroredInXProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->mirrorInX(val);
  return eOk;
}

/* OdDbTextTextStyleIdProperty */

struct OdDbTextTextStyleIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextTextStyleIdProperty>::createObject();
    ((OdDbTextTextStyleIdProperty*)res.get())->init(__OD_T("TextStyleId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbTextTextStyleIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/TextStyleTableId/Items"));
    ((OdDbTextTextStyleIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTextTextStyleIdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Style"));
    ((OdDbTextTextStyleIdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the style name of the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbTextTextStyleIdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 5));
    ((OdDbTextTextStyleIdProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbTextTextStyleIdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"StyleName"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextTextStyleIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->textStyle());
  return eOk;
}

OdResult OdDbTextTextStyleIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextPtr pObj = OdDbText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setTextStyle(val);
  return eOk;
}

void createOdDbTextProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbTextRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextObliqueProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextIsDefaultAlignmentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextAlignmentPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextThicknessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextHorizontalModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextIsMirroredInYProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextWidthFactorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextVerticalModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextTextStringProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextIsMirroredInXProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextTextStyleIdProperty::createObject(b.owner()));
  b.add(properties.last());
}
