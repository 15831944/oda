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
#include "DbViewportTableRecord.h"
#include "DbValueTypes.h"

/* OdDbViewportTableRecordGridIncrementsProperty */

struct OdDbViewportTableRecordGridIncrementsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordGridIncrementsProperty>::createObject();
    ((OdDbViewportTableRecordGridIncrementsProperty*)res.get())->init(__OD_T("GridIncrements"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordGridIncrementsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint2d>(pObj->gridIncrements());
  return eOk;
}

OdResult OdDbViewportTableRecordGridIncrementsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridIncrements(val);
  return eOk;
}

/* OdDbViewportTableRecordGridEnabledProperty */

struct OdDbViewportTableRecordGridEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordGridEnabledProperty>::createObject();
    ((OdDbViewportTableRecordGridEnabledProperty*)res.get())->init(__OD_T("GridEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordGridEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->gridEnabled());
  return eOk;
}

OdResult OdDbViewportTableRecordGridEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridEnabled(val);
  return eOk;
}

/* OdDbViewportTableRecordSnapEnabledProperty */

struct OdDbViewportTableRecordSnapEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordSnapEnabledProperty>::createObject();
    ((OdDbViewportTableRecordSnapEnabledProperty*)res.get())->init(__OD_T("SnapEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordSnapEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->snapEnabled());
  return eOk;
}

OdResult OdDbViewportTableRecordSnapEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSnapEnabled(val);
  return eOk;
}

/* OdDbViewportTableRecordLowerLeftCornerProperty */

struct OdDbViewportTableRecordLowerLeftCornerProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordLowerLeftCornerProperty>::createObject();
    ((OdDbViewportTableRecordLowerLeftCornerProperty*)res.get())->init(__OD_T("LowerLeftCorner"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordLowerLeftCornerProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint2d>(pObj->lowerLeftCorner());
  return eOk;
}

OdResult OdDbViewportTableRecordLowerLeftCornerProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLowerLeftCorner(val);
  return eOk;
}

/* OdDbViewportTableRecordIsometricSnapEnabledProperty */

struct OdDbViewportTableRecordIsometricSnapEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordIsometricSnapEnabledProperty>::createObject();
    ((OdDbViewportTableRecordIsometricSnapEnabledProperty*)res.get())->init(__OD_T("IsometricSnapEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordIsometricSnapEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isometricSnapEnabled());
  return eOk;
}

OdResult OdDbViewportTableRecordIsometricSnapEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsometricSnapEnabled(val);
  return eOk;
}

/* OdDbViewportTableRecordUcsFollowModeProperty */

struct OdDbViewportTableRecordUcsFollowModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordUcsFollowModeProperty>::createObject();
    ((OdDbViewportTableRecordUcsFollowModeProperty*)res.get())->init(__OD_T("UcsFollowMode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordUcsFollowModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->ucsFollowMode());
  return eOk;
}

OdResult OdDbViewportTableRecordUcsFollowModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setUcsFollowMode(val);
  return eOk;
}

/* OdDbViewportTableRecordUpperRightCornerProperty */

struct OdDbViewportTableRecordUpperRightCornerProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordUpperRightCornerProperty>::createObject();
    ((OdDbViewportTableRecordUpperRightCornerProperty*)res.get())->init(__OD_T("UpperRightCorner"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordUpperRightCornerProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint2d>(pObj->upperRightCorner());
  return eOk;
}

OdResult OdDbViewportTableRecordUpperRightCornerProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setUpperRightCorner(val);
  return eOk;
}

/* OdDbViewportTableRecordCircleSidesProperty */

struct OdDbViewportTableRecordCircleSidesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordCircleSidesProperty>::createObject();
    ((OdDbViewportTableRecordCircleSidesProperty*)res.get())->init(__OD_T("CircleSides"), &OdRxValueType::Desc<short>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordCircleSidesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->circleSides());
  return eOk;
}

OdResult OdDbViewportTableRecordCircleSidesProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCircleSides(val);
  return eOk;
}

/* OdDbViewportTableRecordIconAtOriginProperty */

struct OdDbViewportTableRecordIconAtOriginProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordIconAtOriginProperty>::createObject();
    ((OdDbViewportTableRecordIconAtOriginProperty*)res.get())->init(__OD_T("IconAtOrigin"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordIconAtOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->iconAtOrigin());
  return eOk;
}

OdResult OdDbViewportTableRecordIconAtOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIconAtOrigin(val);
  return eOk;
}

/* OdDbViewportTableRecordIconEnabledProperty */

struct OdDbViewportTableRecordIconEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordIconEnabledProperty>::createObject();
    ((OdDbViewportTableRecordIconEnabledProperty*)res.get())->init(__OD_T("IconEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordIconEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->iconEnabled());
  return eOk;
}

OdResult OdDbViewportTableRecordIconEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIconEnabled(val);
  return eOk;
}

/* OdDbViewportTableRecordSnapAngleProperty */

struct OdDbViewportTableRecordSnapAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordSnapAngleProperty>::createObject();
    ((OdDbViewportTableRecordSnapAngleProperty*)res.get())->init(__OD_T("SnapAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportTableRecordSnapAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordSnapAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->snapAngle());
  return eOk;
}

OdResult OdDbViewportTableRecordSnapAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSnapAngle(val);
  return eOk;
}

/* OdDbViewportTableRecordGridFollowProperty */

struct OdDbViewportTableRecordGridFollowProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordGridFollowProperty>::createObject();
    ((OdDbViewportTableRecordGridFollowProperty*)res.get())->init(__OD_T("GridFollow"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordGridFollowProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isGridFollow());
  return eOk;
}

OdResult OdDbViewportTableRecordGridFollowProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridFollow(val);
  return eOk;
}

/* OdDbViewportTableRecordGridSubdivisionRestrictedProperty */

struct OdDbViewportTableRecordGridSubdivisionRestrictedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordGridSubdivisionRestrictedProperty>::createObject();
    ((OdDbViewportTableRecordGridSubdivisionRestrictedProperty*)res.get())->init(__OD_T("GridSubdivisionRestricted"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordGridSubdivisionRestrictedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isGridSubdivisionRestricted());
  return eOk;
}

OdResult OdDbViewportTableRecordGridSubdivisionRestrictedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridSubdivisionRestricted(val);
  return eOk;
}

/* OdDbViewportTableRecordGridMajorProperty */

struct OdDbViewportTableRecordGridMajorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordGridMajorProperty>::createObject();
    ((OdDbViewportTableRecordGridMajorProperty*)res.get())->init(__OD_T("GridMajor"), &OdRxValueType::Desc<short>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordGridMajorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->gridMajor());
  return eOk;
}

OdResult OdDbViewportTableRecordGridMajorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridMajor(val);
  return eOk;
}

/* OdDbViewportTableRecordSnapPairProperty */

struct OdDbViewportTableRecordSnapPairProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordSnapPairProperty>::createObject();
    ((OdDbViewportTableRecordSnapPairProperty*)res.get())->init(__OD_T("SnapPair"), &OdRxValueType::Desc<short>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordSnapPairProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->snapPair());
  return eOk;
}

OdResult OdDbViewportTableRecordSnapPairProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSnapPair(val);
  return eOk;
}

/* OdDbViewportTableRecordUcsSavedWithViewportProperty */

struct OdDbViewportTableRecordUcsSavedWithViewportProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordUcsSavedWithViewportProperty>::createObject();
    ((OdDbViewportTableRecordUcsSavedWithViewportProperty*)res.get())->init(__OD_T("UcsSavedWithViewport"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordUcsSavedWithViewportProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isUcsSavedWithViewport());
  return eOk;
}

OdResult OdDbViewportTableRecordUcsSavedWithViewportProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>ENOTAPPLICABLE
  return eNotApplicable;
}

/* OdDbViewportTableRecordSnapIncrementsProperty */

struct OdDbViewportTableRecordSnapIncrementsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordSnapIncrementsProperty>::createObject();
    ((OdDbViewportTableRecordSnapIncrementsProperty*)res.get())->init(__OD_T("SnapIncrements"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordSnapIncrementsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint2d>(pObj->snapIncrements());
  return eOk;
}

OdResult OdDbViewportTableRecordSnapIncrementsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSnapIncrements(val);
  return eOk;
}

/* OdDbViewportTableRecordSnapBaseProperty */

struct OdDbViewportTableRecordSnapBaseProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordSnapBaseProperty>::createObject();
    ((OdDbViewportTableRecordSnapBaseProperty*)res.get())->init(__OD_T("SnapBase"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordSnapBaseProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint2d>(pObj->snapBase());
  return eOk;
}

OdResult OdDbViewportTableRecordSnapBaseProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSnapBase(val);
  return eOk;
}

/* OdDbViewportTableRecordGridAdaptiveProperty */

struct OdDbViewportTableRecordGridAdaptiveProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordGridAdaptiveProperty>::createObject();
    ((OdDbViewportTableRecordGridAdaptiveProperty*)res.get())->init(__OD_T("GridAdaptive"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordGridAdaptiveProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isGridAdaptive());
  return eOk;
}

OdResult OdDbViewportTableRecordGridAdaptiveProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridAdaptive(val);
  return eOk;
}

/* OdDbViewportTableRecordGridBoundToLimitsProperty */

struct OdDbViewportTableRecordGridBoundToLimitsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordGridBoundToLimitsProperty>::createObject();
    ((OdDbViewportTableRecordGridBoundToLimitsProperty*)res.get())->init(__OD_T("GridBoundToLimits"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordGridBoundToLimitsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isGridBoundToLimits());
  return eOk;
}

OdResult OdDbViewportTableRecordGridBoundToLimitsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridBoundToLimits(val);
  return eOk;
}

/* OdDbViewportTableRecordFastZoomsEnabledProperty */

struct OdDbViewportTableRecordFastZoomsEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTableRecordFastZoomsEnabledProperty>::createObject();
    ((OdDbViewportTableRecordFastZoomsEnabledProperty*)res.get())->init(__OD_T("FastZoomsEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTableRecordFastZoomsEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->fastZoomsEnabled());
  return eOk;
}

OdResult OdDbViewportTableRecordFastZoomsEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportTableRecordPtr pObj = OdDbViewportTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFastZoomsEnabled(val);
  return eOk;
}

void createOdDbViewportTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbViewportTableRecordGridIncrementsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordGridEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordSnapEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordLowerLeftCornerProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordIsometricSnapEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordUcsFollowModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordUpperRightCornerProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordCircleSidesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordIconAtOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordIconEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordSnapAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordGridFollowProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordGridSubdivisionRestrictedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordGridMajorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordSnapPairProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordUcsSavedWithViewportProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordSnapIncrementsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordSnapBaseProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordGridAdaptiveProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordGridBoundToLimitsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTableRecordFastZoomsEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
}
