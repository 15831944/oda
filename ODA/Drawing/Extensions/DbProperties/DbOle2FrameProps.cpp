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
#include "DbOle2Frame.h"
#include "DbValueTypes.h"

/* OdDbOle2FrameWcsHeightProperty */

struct OdDbOle2FrameWcsHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameWcsHeightProperty>::createObject();
    ((OdDbOle2FrameWcsHeightProperty*)res.get())->init(__OD_T("WcsHeight"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbOle2FrameWcsHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOle2FrameWcsHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Height"));
    ((OdDbOle2FrameWcsHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the height of the OLE object box"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbOle2FrameWcsHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDbOle2FrameWcsHeightProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Height"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FrameWcsHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->wcsHeight());
  return eOk;
}

OdResult OdDbOle2FrameWcsHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWcsHeight(val);
  return eOk;
}

/* OdDbOle2FrameWcsWidthProperty */

struct OdDbOle2FrameWcsWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameWcsWidthProperty>::createObject();
    ((OdDbOle2FrameWcsWidthProperty*)res.get())->init(__OD_T("WcsWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbOle2FrameWcsWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOle2FrameWcsWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Width"));
    ((OdDbOle2FrameWcsWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the width of the OLE object box"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbOle2FrameWcsWidthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDbOle2FrameWcsWidthProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Width"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FrameWcsWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->wcsWidth());
  return eOk;
}

OdResult OdDbOle2FrameWcsWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWcsWidth(val);
  return eOk;
}

/* OdDbOle2FrameAutoOutputQualityProperty */

struct OdDbOle2FrameAutoOutputQualityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameAutoOutputQualityProperty>::createObject();
    ((OdDbOle2FrameAutoOutputQualityProperty*)res.get())->init(__OD_T("AutoOutputQuality"), &OdRxValueType::Desc<unsigned char>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FrameAutoOutputQualityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<unsigned char>(pObj->autoOutputQuality());
  return eOk;
}

OdResult OdDbOle2FrameAutoOutputQualityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  unsigned char val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAutoOutputQuality(val);
  return eOk;
}

/* OdDbOle2FrameScaleWidthProperty */

struct OdDbOle2FrameScaleWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameScaleWidthProperty>::createObject();
    ((OdDbOle2FrameScaleWidthProperty*)res.get())->init(__OD_T("ScaleWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbOle2FrameScaleWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOle2FrameScaleWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Scale width  "));
    ((OdDbOle2FrameScaleWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the width of the object as a percentage of original width"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbOle2FrameScaleWidthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FrameScaleWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->scaleWidth());
  return eOk;
}

OdResult OdDbOle2FrameScaleWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setScaleWidth(val);
  return eOk;
}

/* OdDbOle2FrameRotationProperty */

struct OdDbOle2FrameRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameRotationProperty>::createObject();
    ((OdDbOle2FrameRotationProperty*)res.get())->init(__OD_T("Rotation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbOle2FrameRotationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbOle2FrameRotationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOle2FrameRotationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotation"));
    ((OdDbOle2FrameRotationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rotation angle of the OLE object"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FrameRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->rotation());
  return eOk;
}

OdResult OdDbOle2FrameRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRotation(val);
  return eOk;
}

/* OdDbOle2FrameLockAspectProperty */

struct OdDbOle2FrameLockAspectProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameLockAspectProperty>::createObject();
    ((OdDbOle2FrameLockAspectProperty*)res.get())->init(__OD_T("LockAspect"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOle2FrameLockAspectProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Lock aspect"));
    ((OdDbOle2FrameLockAspectProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Ensures the width and height of the object stay in proportion"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbOle2FrameLockAspectProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDbOle2FrameLockAspectProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LockAspectRatio"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FrameLockAspectProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->lockAspect());
  return eOk;
}

OdResult OdDbOle2FrameLockAspectProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLockAspect(val);
  return eOk;
}

/* OdDbOle2FrameScaleHeightProperty */

struct OdDbOle2FrameScaleHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameScaleHeightProperty>::createObject();
    ((OdDbOle2FrameScaleHeightProperty*)res.get())->init(__OD_T("ScaleHeight"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbOle2FrameScaleHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOle2FrameScaleHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Scale height "));
    ((OdDbOle2FrameScaleHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the height of the object as a percentage of original height"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbOle2FrameScaleHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FrameScaleHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->scaleHeight());
  return eOk;
}

OdResult OdDbOle2FrameScaleHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setScaleHeight(val);
  return eOk;
}

/* OdDbOle2FrameLinkNameProperty */

struct OdDbOle2FrameLinkNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameLinkNameProperty>::createObject();
    ((OdDbOle2FrameLinkNameProperty*)res.get())->init(__OD_T("LinkName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbOle2FrameLinkNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getLinkName());
  return eOk;
}

/* OdDbOle2FrameIsLinkedProperty */

struct OdDbOle2FrameIsLinkedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameIsLinkedProperty>::createObject();
    ((OdDbOle2FrameIsLinkedProperty*)res.get())->init(__OD_T("IsLinked"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbOle2FrameIsLinkedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDbOle2Frame::Type OleType = pObj->getType(); value = (OleType == OdDbOle2Frame::kLink);
  return eOk;
}

/* OdDbOle2FrameUserTypeProperty */

struct OdDbOle2FrameUserTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameUserTypeProperty>::createObject();
    ((OdDbOle2FrameUserTypeProperty*)res.get())->init(__OD_T("UserType"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbOle2FrameUserTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getUserType());
  return eOk;
}

/* OdDbOle2FrameLinkPathProperty */

struct OdDbOle2FrameLinkPathProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameLinkPathProperty>::createObject();
    ((OdDbOle2FrameLinkPathProperty*)res.get())->init(__OD_T("LinkPath"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbOle2FrameLinkPathProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getLinkPath());
  return eOk;
}

/* OdDbOle2FrameLocationProperty */

struct OdDbOle2FrameLocationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameLocationProperty>::createObject();
    ((OdDbOle2FrameLocationProperty*)res.get())->init(__OD_T("Location"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOle2FrameLocationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position X;Position Y;Position Z"));
    ((OdDbOle2FrameLocationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the origin coordinates (upper left corner) of the OLE object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbOle2FrameLocationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDbOle2FrameLocationProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsertionPoint"));
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

OdResult OdDbOle2FrameLocationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGePoint3d locPoint; pObj->getLocation(locPoint); value = locPoint;
  return eOk;
}

/* OdDbOle2FrameTypeProperty */

struct OdDbOle2FrameTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameTypeProperty>::createObject();
    ((OdDbOle2FrameTypeProperty*)res.get())->init(__OD_T("Type"), &OdRxValueType::Desc<OdDbOle2Frame::Type>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOle2FrameTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Type"));
    ((OdDbOle2FrameTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the OLE object is linked to the original pasted file when opening object for editing"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbOle2FrameTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 0));
    ((OdDbOle2FrameTypeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"OleItemType"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbOle2FrameTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbOle2Frame::Type>(pObj->getType());
  return eOk;
}

/* OdDbOle2FrameOutputQualityProperty */

struct OdDbOle2FrameOutputQualityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FrameOutputQualityProperty>::createObject();
    ((OdDbOle2FrameOutputQualityProperty*)res.get())->init(__OD_T("OutputQuality"), &OdRxValueType::Desc<OdDbOle2Frame::PlotQuality>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbOle2FrameOutputQualityProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Plot quality"));
    ((OdDbOle2FrameOutputQualityProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Controls plot quality of OLE object based on file type selected from list"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbOle2FrameOutputQualityProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 0));
    ((OdDbOle2FrameOutputQualityProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"OlePlotQuality"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FrameOutputQualityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbOle2Frame::PlotQuality>(pObj->outputQuality());
  return eOk;
}

OdResult OdDbOle2FrameOutputQualityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbOle2Frame::PlotQuality val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setOutputQuality(val);
  return eOk;
}

/* OdDbOle2FramePosition2dProperty */

struct OdDbOle2FramePosition2dProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FramePosition2dProperty>::createObject();
    ((OdDbOle2FramePosition2dProperty*)res.get())->init(__OD_T("Position2d"), &OdRxValueType::Desc<ODRECT>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FramePosition2dProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  ODRECT rect;
  OdRectangle3d pos;
  pObj->position(pos);
  rect.left = pos.upLeft.x;
  rect.top = pos.upLeft.y;
  rect.right = rect.left + (pos.lowRight - pos.lowLeft).length();
  rect.bottom = rect.top - (pos.upLeft - pos.lowLeft).length();
  value = rect;
  return eOk;
  //<<REPLACE_ALL
}

OdResult OdDbOle2FramePosition2dProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  ODRECT rect;
  if (!(value >> rect))
    return eInvalidInput;
  OdRectangle3d pos;
  pObj->position(pos);
  OdGeVector3d u(pos.upRight - pos.upLeft);
  OdGeVector3d v(pos.lowLeft - pos.upLeft);
  u.setLength(rect.right - rect.left);
  v.setLength(rect.top - rect.bottom);
  pos.upLeft.x = rect.left;
  pos.upLeft.y = rect.top;
  pos.upRight = pos.upLeft + u;
  pos.lowLeft = pos.upLeft + v;
  pos.lowRight = pos.upRight + v;
  pObj->setPosition(pos);
  return eOk;
  //<<REPLACE_ALL
}

/* OdDbOle2FramePosition3dProperty */

struct OdDbOle2FramePosition3dProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbOle2FramePosition3dProperty>::createObject();
    ((OdDbOle2FramePosition3dProperty*)res.get())->init(__OD_T("Position3d"), &OdRxValueType::Desc<OdRectangle3d>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbOle2FramePosition3dProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdRectangle3d rect3d; pObj->position(rect3d); value = rect3d;
  return eOk;
}

OdResult OdDbOle2FramePosition3dProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbOle2FramePtr pObj = OdDbOle2Frame::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdRectangle3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setPosition(val);
  return eOk;
}

void createOdDbOle2FrameProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbOle2FrameWcsHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameWcsWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameAutoOutputQualityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameScaleWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameLockAspectProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameScaleHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameLinkNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameIsLinkedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameUserTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameLinkPathProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameLocationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FrameOutputQualityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FramePosition2dProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbOle2FramePosition3dProperty::createObject(b.owner()));
  b.add(properties.last());
}
