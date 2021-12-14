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
#include "DbAbstractViewTableRecord.h"
#include "DbValueTypes.h"

/* OdDbAbstractViewTableRecordPerspectiveEnabledProperty */

struct OdDbAbstractViewTableRecordPerspectiveEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordPerspectiveEnabledProperty>::createObject();
    ((OdDbAbstractViewTableRecordPerspectiveEnabledProperty*)res.get())->init(__OD_T("PerspectiveEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordPerspectiveEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->perspectiveEnabled());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordPerspectiveEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPerspectiveEnabled(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordBackClipDistanceProperty */

struct OdDbAbstractViewTableRecordBackClipDistanceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordBackClipDistanceProperty>::createObject();
    ((OdDbAbstractViewTableRecordBackClipDistanceProperty*)res.get())->init(__OD_T("BackClipDistance"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAbstractViewTableRecordBackClipDistanceProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordBackClipDistanceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->backClipDistance());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordBackClipDistanceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackClipDistance(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordFrontClipDistanceProperty */

struct OdDbAbstractViewTableRecordFrontClipDistanceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordFrontClipDistanceProperty>::createObject();
    ((OdDbAbstractViewTableRecordFrontClipDistanceProperty*)res.get())->init(__OD_T("FrontClipDistance"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAbstractViewTableRecordFrontClipDistanceProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordFrontClipDistanceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->frontClipDistance());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordFrontClipDistanceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFrontClipDistance(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordFrontClipEnabledProperty */

struct OdDbAbstractViewTableRecordFrontClipEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordFrontClipEnabledProperty>::createObject();
    ((OdDbAbstractViewTableRecordFrontClipEnabledProperty*)res.get())->init(__OD_T("FrontClipEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordFrontClipEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->frontClipEnabled());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordFrontClipEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFrontClipEnabled(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordFrontClipAtEyeProperty */

struct OdDbAbstractViewTableRecordFrontClipAtEyeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordFrontClipAtEyeProperty>::createObject();
    ((OdDbAbstractViewTableRecordFrontClipAtEyeProperty*)res.get())->init(__OD_T("FrontClipAtEye"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordFrontClipAtEyeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->frontClipAtEye());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordFrontClipAtEyeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFrontClipAtEye(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordBackClipEnabledProperty */

struct OdDbAbstractViewTableRecordBackClipEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordBackClipEnabledProperty>::createObject();
    ((OdDbAbstractViewTableRecordBackClipEnabledProperty*)res.get())->init(__OD_T("BackClipEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordBackClipEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->backClipEnabled());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordBackClipEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackClipEnabled(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordWidthProperty */

struct OdDbAbstractViewTableRecordWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordWidthProperty>::createObject();
    ((OdDbAbstractViewTableRecordWidthProperty*)res.get())->init(__OD_T("Width"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAbstractViewTableRecordWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->width());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWidth(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordHeightProperty */

struct OdDbAbstractViewTableRecordHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordHeightProperty>::createObject();
    ((OdDbAbstractViewTableRecordHeightProperty*)res.get())->init(__OD_T("Height"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAbstractViewTableRecordHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->height());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHeight(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordCenterPointProperty */

struct OdDbAbstractViewTableRecordCenterPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordCenterPointProperty>::createObject();
    ((OdDbAbstractViewTableRecordCenterPointProperty*)res.get())->init(__OD_T("CenterPoint"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
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

OdResult OdDbAbstractViewTableRecordCenterPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint2d>(pObj->centerPoint());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordCenterPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCenterPoint(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordTargetProperty */

struct OdDbAbstractViewTableRecordTargetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordTargetProperty>::createObject();
    ((OdDbAbstractViewTableRecordTargetProperty*)res.get())->init(__OD_T("Target"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbAbstractViewTableRecordTargetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->target());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordTargetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTarget(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordLensLengthProperty */

struct OdDbAbstractViewTableRecordLensLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordLensLengthProperty>::createObject();
    ((OdDbAbstractViewTableRecordLensLengthProperty*)res.get())->init(__OD_T("LensLength"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAbstractViewTableRecordLensLengthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordLensLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->lensLength());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordLensLengthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLensLength(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordViewTwistProperty */

struct OdDbAbstractViewTableRecordViewTwistProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordViewTwistProperty>::createObject();
    ((OdDbAbstractViewTableRecordViewTwistProperty*)res.get())->init(__OD_T("ViewTwist"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAbstractViewTableRecordViewTwistProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordViewTwistProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->viewTwist());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordViewTwistProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setViewTwist(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordViewDirectionProperty */

struct OdDbAbstractViewTableRecordViewDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordViewDirectionProperty>::createObject();
    ((OdDbAbstractViewTableRecordViewDirectionProperty*)res.get())->init(__OD_T("ViewDirection"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbAbstractViewTableRecordViewDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->viewDirection());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordViewDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setViewDirection(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordElevationProperty */

struct OdDbAbstractViewTableRecordElevationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordElevationProperty>::createObject();
    ((OdDbAbstractViewTableRecordElevationProperty*)res.get())->init(__OD_T("Elevation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAbstractViewTableRecordElevationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordElevationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->elevation());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordElevationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setElevation(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordUcsNameProperty */

struct OdDbAbstractViewTableRecordUcsNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordUcsNameProperty>::createObject();
    ((OdDbAbstractViewTableRecordUcsNameProperty*)res.get())->init(__OD_T("UcsName"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbAbstractViewTableRecordUcsNameProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbAbstractViewTableRecordUcsNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->ucsName());
  return eOk;
}

/* OdDbAbstractViewTableRecordVisualStyleIdProperty */

struct OdDbAbstractViewTableRecordVisualStyleIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordVisualStyleIdProperty>::createObject();
    ((OdDbAbstractViewTableRecordVisualStyleIdProperty*)res.get())->init(__OD_T("VisualStyleId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbAbstractViewTableRecordVisualStyleIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordVisualStyleIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->visualStyle());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordVisualStyleIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setVisualStyle(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordBackgroundProperty */

struct OdDbAbstractViewTableRecordBackgroundProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordBackgroundProperty>::createObject();
    ((OdDbAbstractViewTableRecordBackgroundProperty*)res.get())->init(__OD_T("Background"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbAbstractViewTableRecordBackgroundProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordBackgroundProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->background());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordBackgroundProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackground(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordBrightnessProperty */

struct OdDbAbstractViewTableRecordBrightnessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordBrightnessProperty>::createObject();
    ((OdDbAbstractViewTableRecordBrightnessProperty*)res.get())->init(__OD_T("Brightness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAbstractViewTableRecordBrightnessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordBrightnessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->brightness());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordBrightnessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBrightness(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordDefaultLightingTypeProperty */

struct OdDbAbstractViewTableRecordDefaultLightingTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordDefaultLightingTypeProperty>::createObject();
    ((OdDbAbstractViewTableRecordDefaultLightingTypeProperty*)res.get())->init(__OD_T("DefaultLightingType"), &OdRxValueType::Desc<OdGiViewportTraits::DefaultLightingType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordDefaultLightingTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiViewportTraits::DefaultLightingType>(pObj->defaultLightingType());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordDefaultLightingTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiViewportTraits::DefaultLightingType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDefaultLightingType(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordDefaultLightingOnProperty */

struct OdDbAbstractViewTableRecordDefaultLightingOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordDefaultLightingOnProperty>::createObject();
    ((OdDbAbstractViewTableRecordDefaultLightingOnProperty*)res.get())->init(__OD_T("DefaultLightingOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordDefaultLightingOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isDefaultLightingOn());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordDefaultLightingOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDefaultLightingOn(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordSunIdProperty */

struct OdDbAbstractViewTableRecordSunIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordSunIdProperty>::createObject();
    ((OdDbAbstractViewTableRecordSunIdProperty*)res.get())->init(__OD_T("SunId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbAbstractViewTableRecordSunIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbAbstractViewTableRecordSunIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->sunId());
  return eOk;
}

/* OdDbAbstractViewTableRecordAmbientLightColorProperty */

struct OdDbAbstractViewTableRecordAmbientLightColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordAmbientLightColorProperty>::createObject();
    ((OdDbAbstractViewTableRecordAmbientLightColorProperty*)res.get())->init(__OD_T("AmbientLightColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordAmbientLightColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->ambientLightColor());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordAmbientLightColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAmbientLightColor(val);
  return eOk;
}

/* OdDbAbstractViewTableRecordContrastProperty */

struct OdDbAbstractViewTableRecordContrastProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAbstractViewTableRecordContrastProperty>::createObject();
    ((OdDbAbstractViewTableRecordContrastProperty*)res.get())->init(__OD_T("Contrast"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAbstractViewTableRecordContrastProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAbstractViewTableRecordContrastProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->contrast());
  return eOk;
}

OdResult OdDbAbstractViewTableRecordContrastProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAbstractViewTableRecordPtr pObj = OdDbAbstractViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setContrast(val);
  return eOk;
}

void createOdDbAbstractViewTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbAbstractViewTableRecordPerspectiveEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordBackClipDistanceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordFrontClipDistanceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordFrontClipEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordFrontClipAtEyeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordBackClipEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordCenterPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordTargetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordLensLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordViewTwistProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordViewDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordElevationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordUcsNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordVisualStyleIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordBackgroundProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordBrightnessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordDefaultLightingTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordDefaultLightingOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordSunIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordAmbientLightColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAbstractViewTableRecordContrastProperty::createObject(b.owner()));
  b.add(properties.last());
}
