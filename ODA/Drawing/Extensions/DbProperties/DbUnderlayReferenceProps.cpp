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
#include "DbUnderlayReference.h"
#include "DbValueTypes.h"

/* OdDbUnderlayReferenceIsClippedProperty */

struct OdDbUnderlayReferenceIsClippedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceIsClippedProperty>::createObject();
    ((OdDbUnderlayReferenceIsClippedProperty*)res.get())->init(__OD_T("IsClipped"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayReferenceIsClippedProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Show clipped"));
    ((OdDbUnderlayReferenceIsClippedProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Enables or disables the clipping boundary of the underlay"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbUnderlayReferenceIsClippedProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 6));
    ((OdDbUnderlayReferenceIsClippedProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ClippingEnabled"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceIsClippedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isClipped());
  return eOk;
}

OdResult OdDbUnderlayReferenceIsClippedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsClipped(val);
  return eOk;
}

/* OdDbUnderlayReferenceTransformProperty */

struct OdDbUnderlayReferenceTransformProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceTransformProperty>::createObject();
    ((OdDbUnderlayReferenceTransformProperty*)res.get())->init(__OD_T("Transform"), &OdRxValueType::Desc<OdGeMatrix3d>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceTransformProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeMatrix3d>(pObj->transform());
  return eOk;
}

OdResult OdDbUnderlayReferenceTransformProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeMatrix3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTransform(val);
  return eOk;
}

/* OdDbUnderlayReferenceContrastProperty */

struct OdDbUnderlayReferenceContrastProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceContrastProperty>::createObject();
    ((OdDbUnderlayReferenceContrastProperty*)res.get())->init(__OD_T("Contrast"), &OdRxValueType::Desc<unsigned char>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayReferenceContrastProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Contrast"));
    ((OdDbUnderlayReferenceContrastProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current contrast value of the underlay"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Underlay Adjust"))
      OdRxCategory::createObject(L"Underlay Adjust", OdRxCategory::rootCategory());
    ((OdDbUnderlayReferenceContrastProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Underlay Adjust", 0));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceContrastProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<unsigned char>(pObj->contrast());
  return eOk;
}

OdResult OdDbUnderlayReferenceContrastProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  unsigned char val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setContrast(val);
  return eOk;
}

/* OdDbUnderlayReferenceIsOnProperty */

struct OdDbUnderlayReferenceIsOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceIsOnProperty>::createObject();
    ((OdDbUnderlayReferenceIsOnProperty*)res.get())->init(__OD_T("IsOn"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayReferenceIsOnProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Show underlay"));
    ((OdDbUnderlayReferenceIsOnProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether underlay is visible or not"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbUnderlayReferenceIsOnProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 5));
    ((OdDbUnderlayReferenceIsOnProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"UnderlayVisibility"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceIsOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isOn());
  return eOk;
}

OdResult OdDbUnderlayReferenceIsOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsOn(val);
  return eOk;
}

/* OdDbUnderlayReferenceScaleFactorsProperty */

struct OdDbUnderlayReferenceScaleFactorsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceScaleFactorsProperty>::createObject();
    ((OdDbUnderlayReferenceScaleFactorsProperty*)res.get())->init(__OD_T("ScaleFactors"), &OdRxValueType::Desc<OdGeScale3d>::value(), owner);
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

OdResult OdDbUnderlayReferenceScaleFactorsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeScale3d>(pObj->scaleFactors());
  return eOk;
}

OdResult OdDbUnderlayReferenceScaleFactorsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeScale3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setScaleFactors(val);
  return eOk;
}

/* OdDbUnderlayReferencePositionProperty */

struct OdDbUnderlayReferencePositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferencePositionProperty>::createObject();
    ((OdDbUnderlayReferencePositionProperty*)res.get())->init(__OD_T("Position"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayReferencePositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position X;Position Y;Position Z"));
    ((OdDbUnderlayReferencePositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the origin coordinates (lower left corner) of the underlay"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbUnderlayReferencePositionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
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

OdResult OdDbUnderlayReferencePositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

OdResult OdDbUnderlayReferencePositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPosition(val);
  return eOk;
}

/* OdDbUnderlayReferenceNormalProperty */

struct OdDbUnderlayReferenceNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceNormalProperty>::createObject();
    ((OdDbUnderlayReferenceNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbUnderlayReferenceNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbUnderlayReferenceNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDbUnderlayReferenceRotationProperty */

struct OdDbUnderlayReferenceRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceRotationProperty>::createObject();
    ((OdDbUnderlayReferenceRotationProperty*)res.get())->init(__OD_T("Rotation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbUnderlayReferenceRotationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayReferenceRotationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotation"));
    ((OdDbUnderlayReferenceRotationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rotation angle of the underlay"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbUnderlayReferenceRotationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->rotation());
  return eOk;
}

OdResult OdDbUnderlayReferenceRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRotation(val);
  return eOk;
}

/* OdDbUnderlayReferenceFadeProperty */

struct OdDbUnderlayReferenceFadeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceFadeProperty>::createObject();
    ((OdDbUnderlayReferenceFadeProperty*)res.get())->init(__OD_T("Fade"), &OdRxValueType::Desc<unsigned char>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayReferenceFadeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Fade"));
    ((OdDbUnderlayReferenceFadeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current fade value of the underlay"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Underlay Adjust"))
      OdRxCategory::createObject(L"Underlay Adjust", OdRxCategory::rootCategory());
    ((OdDbUnderlayReferenceFadeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Underlay Adjust", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceFadeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<unsigned char>(pObj->fade());
  return eOk;
}

OdResult OdDbUnderlayReferenceFadeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  unsigned char val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFade(val);
  return eOk;
}

/* OdDbUnderlayReferenceIsClipInvertedProperty */

struct OdDbUnderlayReferenceIsClipInvertedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceIsClipInvertedProperty>::createObject();
    ((OdDbUnderlayReferenceIsClipInvertedProperty*)res.get())->init(__OD_T("IsClipInverted"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceIsClipInvertedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isClipInverted());
  return eOk;
}

OdResult OdDbUnderlayReferenceIsClipInvertedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setClipInverted(val);
  return eOk;
}

/* OdDbUnderlayReferenceDefinitionIdProperty */

struct OdDbUnderlayReferenceDefinitionIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceDefinitionIdProperty>::createObject();
    ((OdDbUnderlayReferenceDefinitionIdProperty*)res.get())->init(__OD_T("DefinitionId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbUnderlayReferenceDefinitionIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"$/OwnerId/Items"));
    ((OdDbUnderlayReferenceDefinitionIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"SourceFileName", OdRxValueType::Desc<const OdChar*>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Saved Path"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the path to the underlay file"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 3));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"File"));
    res = OdRxSubProperty::createObject(L"ItemName", OdRxValueType::Desc<const OdChar*>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Page number"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the item of the underlay file"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 2));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ItemName"));
    res = OdRxSubProperty::createObject(L"Name", OdRxValueType::Desc<OdString>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Name"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the name of the underlay file"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 1));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"UnderlayName"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceDefinitionIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->definitionId());
  return eOk;
}

OdResult OdDbUnderlayReferenceDefinitionIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDefinitionId(val);
  return eOk;
}

/* OdDbUnderlayReferenceAdjustColorForBackgroundProperty */

struct OdDbUnderlayReferenceAdjustColorForBackgroundProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceAdjustColorForBackgroundProperty>::createObject();
    ((OdDbUnderlayReferenceAdjustColorForBackgroundProperty*)res.get())->init(__OD_T("AdjustColorForBackground"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayReferenceAdjustColorForBackgroundProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Adjust colors for background"));
    ((OdDbUnderlayReferenceAdjustColorForBackgroundProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether the underlay colors are adjusted for the current background color"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Underlay Adjust"))
      OdRxCategory::createObject(L"Underlay Adjust", OdRxCategory::rootCategory());
    ((OdDbUnderlayReferenceAdjustColorForBackgroundProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Underlay Adjust", 3));
    ((OdDbUnderlayReferenceAdjustColorForBackgroundProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"AdjustForBackground"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceAdjustColorForBackgroundProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isAdjustedForBackground());
  return eOk;
}

OdResult OdDbUnderlayReferenceAdjustColorForBackgroundProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setIsAdjustedForBackground(val);
  return eOk;
}

/* OdDbUnderlayReferenceMonochromeProperty */

struct OdDbUnderlayReferenceMonochromeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayReferenceMonochromeProperty>::createObject();
    ((OdDbUnderlayReferenceMonochromeProperty*)res.get())->init(__OD_T("Monochrome"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayReferenceMonochromeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Monochrome"));
    ((OdDbUnderlayReferenceMonochromeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether underlay is monochrome or not"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Underlay Adjust"))
      OdRxCategory::createObject(L"Underlay Adjust", OdRxCategory::rootCategory());
    ((OdDbUnderlayReferenceMonochromeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Underlay Adjust", 2));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayReferenceMonochromeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isMonochrome());
  return eOk;
}

OdResult OdDbUnderlayReferenceMonochromeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayReferencePtr pObj = OdDbUnderlayReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setIsMonochrome(val);
  return eOk;
}

void createOdDbUnderlayReferenceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbUnderlayReferenceIsClippedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceTransformProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceContrastProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceIsOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceScaleFactorsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferencePositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceFadeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceIsClipInvertedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceDefinitionIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceAdjustColorForBackgroundProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayReferenceMonochromeProperty::createObject(b.owner()));
  b.add(properties.last());
}
