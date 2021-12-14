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
#include "DbViewport.h"
#include "DbValueTypes.h"

/* OdDbViewportSnapIsometricProperty */

struct OdDbViewportSnapIsometricProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportSnapIsometricProperty>::createObject();
    ((OdDbViewportSnapIsometricProperty*)res.get())->init(__OD_T("SnapIsometric"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportSnapIsometricProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isSnapIsometric());
  return eOk;
}

OdResult OdDbViewportSnapIsometricProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setSnapIsometric(); else pObj->setSnapStandard();
  return eOk;
}

/* OdDbViewportSnapOnProperty */

struct OdDbViewportSnapOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportSnapOnProperty>::createObject();
    ((OdDbViewportSnapOnProperty*)res.get())->init(__OD_T("SnapOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportSnapOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isSnapOn());
  return eOk;
}

OdResult OdDbViewportSnapOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setSnapOn(); else pObj->setSnapOff();
  return eOk;
}

/* OdDbViewportSnapBasePointProperty */

struct OdDbViewportSnapBasePointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportSnapBasePointProperty>::createObject();
    ((OdDbViewportSnapBasePointProperty*)res.get())->init(__OD_T("SnapBasePoint"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
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

OdResult OdDbViewportSnapBasePointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint2d>(pObj->snapBasePoint());
  return eOk;
}

OdResult OdDbViewportSnapBasePointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSnapBasePoint(val);
  return eOk;
}

/* OdDbViewportSnapAngleProperty */

struct OdDbViewportSnapAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportSnapAngleProperty>::createObject();
    ((OdDbViewportSnapAngleProperty*)res.get())->init(__OD_T("SnapAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportSnapAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportSnapAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->snapAngle());
  return eOk;
}

OdResult OdDbViewportSnapAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSnapAngle(val);
  return eOk;
}

/* OdDbViewportUcsIconVisibleProperty */

struct OdDbViewportUcsIconVisibleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportUcsIconVisibleProperty>::createObject();
    ((OdDbViewportUcsIconVisibleProperty*)res.get())->init(__OD_T("UcsIconVisible"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportUcsIconVisibleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isUcsIconVisible());
  return eOk;
}

OdResult OdDbViewportUcsIconVisibleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setUcsIconVisible(); else pObj->setUcsIconInvisible();
  return eOk;
}

/* OdDbViewportUcsFollowModeOnProperty */

struct OdDbViewportUcsFollowModeOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportUcsFollowModeOnProperty>::createObject();
    ((OdDbViewportUcsFollowModeOnProperty*)res.get())->init(__OD_T("UcsFollowModeOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportUcsFollowModeOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isUcsFollowModeOn());
  return eOk;
}

OdResult OdDbViewportUcsFollowModeOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setUcsFollowModeOn(); else pObj->setUcsFollowModeOff();
  return eOk;
}

/* OdDbViewportCircleSidesProperty */

struct OdDbViewportCircleSidesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportCircleSidesProperty>::createObject();
    ((OdDbViewportCircleSidesProperty*)res.get())->init(__OD_T("CircleSides"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportCircleSidesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->circleSides());
  return eOk;
}

OdResult OdDbViewportCircleSidesProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCircleSides(val);
  return eOk;
}

/* OdDbViewportUcsIconAtOriginProperty */

struct OdDbViewportUcsIconAtOriginProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportUcsIconAtOriginProperty>::createObject();
    ((OdDbViewportUcsIconAtOriginProperty*)res.get())->init(__OD_T("UcsIconAtOrigin"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportUcsIconAtOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isUcsIconAtOrigin());
  return eOk;
}

OdResult OdDbViewportUcsIconAtOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setUcsIconAtOrigin(); else pObj->setUcsIconAtCorner();
  return eOk;
}

/* OdDbViewportGridBoundToLimitsProperty */

struct OdDbViewportGridBoundToLimitsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportGridBoundToLimitsProperty>::createObject();
    ((OdDbViewportGridBoundToLimitsProperty*)res.get())->init(__OD_T("GridBoundToLimits"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportGridBoundToLimitsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isGridBoundToLimits());
  return eOk;
}

OdResult OdDbViewportGridBoundToLimitsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridBoundToLimits(val);
  return eOk;
}

/* OdDbViewportHiddenLinesRemovedProperty */

struct OdDbViewportHiddenLinesRemovedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportHiddenLinesRemovedProperty>::createObject();
    ((OdDbViewportHiddenLinesRemovedProperty*)res.get())->init(__OD_T("HiddenLinesRemoved"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportHiddenLinesRemovedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hiddenLinesRemoved());
  return eOk;
}

OdResult OdDbViewportHiddenLinesRemovedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->removeHiddenLines(); else pObj->showHiddenLines();
  return eOk;
}

/* OdDbViewportHeightProperty */

struct OdDbViewportHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportHeightProperty>::createObject();
    ((OdDbViewportHeightProperty*)res.get())->init(__OD_T("Height"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Height"));
    ((OdDbViewportHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the height of the viewport"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbViewportHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->height());
  return eOk;
}

OdResult OdDbViewportHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHeight(val);
  return eOk;
}

/* OdDbViewportGridAdaptiveProperty */

struct OdDbViewportGridAdaptiveProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportGridAdaptiveProperty>::createObject();
    ((OdDbViewportGridAdaptiveProperty*)res.get())->init(__OD_T("GridAdaptive"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportGridAdaptiveProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isGridAdaptive());
  return eOk;
}

OdResult OdDbViewportGridAdaptiveProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridAdaptive(val);
  return eOk;
}

/* OdDbViewportSnapIsoPairProperty */

struct OdDbViewportSnapIsoPairProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportSnapIsoPairProperty>::createObject();
    ((OdDbViewportSnapIsoPairProperty*)res.get())->init(__OD_T("SnapIsoPair"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportSnapIsoPairProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->snapIsoPair());
  return eOk;
}

OdResult OdDbViewportSnapIsoPairProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSnapIsoPair(val);
  return eOk;
}

/* OdDbViewportSnapIncrementProperty */

struct OdDbViewportSnapIncrementProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportSnapIncrementProperty>::createObject();
    ((OdDbViewportSnapIncrementProperty*)res.get())->init(__OD_T("SnapIncrement"), &OdRxValueType::Desc<OdGeVector2d>::value(), owner);
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

OdResult OdDbViewportSnapIncrementProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector2d>(pObj->snapIncrement());
  return eOk;
}

OdResult OdDbViewportSnapIncrementProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSnapIncrement(val);
  return eOk;
}

/* OdDbViewportGridIncrementProperty */

struct OdDbViewportGridIncrementProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportGridIncrementProperty>::createObject();
    ((OdDbViewportGridIncrementProperty*)res.get())->init(__OD_T("GridIncrement"), &OdRxValueType::Desc<OdGeVector2d>::value(), owner);
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

OdResult OdDbViewportGridIncrementProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector2d>(pObj->gridIncrement());
  return eOk;
}

OdResult OdDbViewportGridIncrementProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridIncrement(val);
  return eOk;
}

/* OdDbViewportGridOnProperty */

struct OdDbViewportGridOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportGridOnProperty>::createObject();
    ((OdDbViewportGridOnProperty*)res.get())->init(__OD_T("GridOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportGridOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isGridOn());
  return eOk;
}

OdResult OdDbViewportGridOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setGridOn(); else pObj->setGridOff();
  return eOk;
}

/* OdDbViewportViewDirectionProperty */

struct OdDbViewportViewDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportViewDirectionProperty>::createObject();
    ((OdDbViewportViewDirectionProperty*)res.get())->init(__OD_T("ViewDirection"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbViewportViewDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->viewDirection());
  return eOk;
}

OdResult OdDbViewportViewDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setViewDirection(val);
  return eOk;
}

/* OdDbViewportViewTargetProperty */

struct OdDbViewportViewTargetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportViewTargetProperty>::createObject();
    ((OdDbViewportViewTargetProperty*)res.get())->init(__OD_T("ViewTarget"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbViewportViewTargetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->viewTarget());
  return eOk;
}

OdResult OdDbViewportViewTargetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setViewTarget(val);
  return eOk;
}

/* OdDbViewportViewCenterProperty */

struct OdDbViewportViewCenterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportViewCenterProperty>::createObject();
    ((OdDbViewportViewCenterProperty*)res.get())->init(__OD_T("ViewCenter"), &OdRxValueType::Desc<OdGePoint2d>::value(), owner);
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

OdResult OdDbViewportViewCenterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint2d>(pObj->viewCenter());
  return eOk;
}

OdResult OdDbViewportViewCenterProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint2d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setViewCenter(val);
  return eOk;
}

/* OdDbViewportViewHeightProperty */

struct OdDbViewportViewHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportViewHeightProperty>::createObject();
    ((OdDbViewportViewHeightProperty*)res.get())->init(__OD_T("ViewHeight"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportViewHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportViewHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->viewHeight());
  return eOk;
}

OdResult OdDbViewportViewHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setViewHeight(val);
  return eOk;
}

/* OdDbViewportCenterPointProperty */

struct OdDbViewportCenterPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportCenterPointProperty>::createObject();
    ((OdDbViewportCenterPointProperty*)res.get())->init(__OD_T("CenterPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportCenterPointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Center X;Center Y;Center Z"));
    ((OdDbViewportCenterPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for the center of the viewport or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbViewportCenterPointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDbViewportCenterPointProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Center"));
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

OdResult OdDbViewportCenterPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->centerPoint());
  return eOk;
}

OdResult OdDbViewportCenterPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCenterPoint(val);
  return eOk;
}

/* OdDbViewportWidthProperty */

struct OdDbViewportWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportWidthProperty>::createObject();
    ((OdDbViewportWidthProperty*)res.get())->init(__OD_T("Width"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Width"));
    ((OdDbViewportWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the width of the viewport"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbViewportWidthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->width());
  return eOk;
}

OdResult OdDbViewportWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWidth(val);
  return eOk;
}

/* OdDbViewportOnProperty */

struct OdDbViewportOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportOnProperty>::createObject();
    ((OdDbViewportOnProperty*)res.get())->init(__OD_T("On"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportOnProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"On"));
    ((OdDbViewportOnProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether the viewport is On or Off"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbViewportOnProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 1));
    ((OdDbViewportOnProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ViewportOn"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isOn());
  return eOk;
}

OdResult OdDbViewportOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setOn(); else pObj->setOff();
  return eOk;
}

/* OdDbViewportNumberProperty */

struct OdDbViewportNumberProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportNumberProperty>::createObject();
    ((OdDbViewportNumberProperty*)res.get())->init(__OD_T("Number"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewportNumberProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->number());
  return eOk;
}

/* OdDbViewportFrontClipDistanceProperty */

struct OdDbViewportFrontClipDistanceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportFrontClipDistanceProperty>::createObject();
    ((OdDbViewportFrontClipDistanceProperty*)res.get())->init(__OD_T("FrontClipDistance"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportFrontClipDistanceProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportFrontClipDistanceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->frontClipDistance());
  return eOk;
}

OdResult OdDbViewportFrontClipDistanceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFrontClipDistance(val);
  return eOk;
}

/* OdDbViewportFrontClipAtEyeOnProperty */

struct OdDbViewportFrontClipAtEyeOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportFrontClipAtEyeOnProperty>::createObject();
    ((OdDbViewportFrontClipAtEyeOnProperty*)res.get())->init(__OD_T("FrontClipAtEyeOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportFrontClipAtEyeOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isFrontClipAtEyeOn());
  return eOk;
}

OdResult OdDbViewportFrontClipAtEyeOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setFrontClipAtEyeOn(); else pObj->setFrontClipAtEyeOff();
  return eOk;
}

/* OdDbViewportPerspectiveOnProperty */

struct OdDbViewportPerspectiveOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportPerspectiveOnProperty>::createObject();
    ((OdDbViewportPerspectiveOnProperty*)res.get())->init(__OD_T("PerspectiveOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportPerspectiveOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isPerspectiveOn());
  return eOk;
}

OdResult OdDbViewportPerspectiveOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setPerspectiveOn(); else pObj->setPerspectiveOff();
  return eOk;
}

/* OdDbViewportBackClipDistanceProperty */

struct OdDbViewportBackClipDistanceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportBackClipDistanceProperty>::createObject();
    ((OdDbViewportBackClipDistanceProperty*)res.get())->init(__OD_T("BackClipDistance"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportBackClipDistanceProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportBackClipDistanceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->backClipDistance());
  return eOk;
}

OdResult OdDbViewportBackClipDistanceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackClipDistance(val);
  return eOk;
}

/* OdDbViewportLensLengthProperty */

struct OdDbViewportLensLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportLensLengthProperty>::createObject();
    ((OdDbViewportLensLengthProperty*)res.get())->init(__OD_T("LensLength"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportLensLengthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportLensLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->lensLength());
  return eOk;
}

OdResult OdDbViewportLensLengthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLensLength(val);
  return eOk;
}

/* OdDbViewportTwistAngleProperty */

struct OdDbViewportTwistAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTwistAngleProperty>::createObject();
    ((OdDbViewportTwistAngleProperty*)res.get())->init(__OD_T("TwistAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportTwistAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTwistAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->twistAngle());
  return eOk;
}

OdResult OdDbViewportTwistAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTwistAngle(val);
  return eOk;
}

/* OdDbViewportBackClipOnProperty */

struct OdDbViewportBackClipOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportBackClipOnProperty>::createObject();
    ((OdDbViewportBackClipOnProperty*)res.get())->init(__OD_T("BackClipOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportBackClipOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isBackClipOn());
  return eOk;
}

OdResult OdDbViewportBackClipOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setBackClipOn(); else pObj->setBackClipOff();
  return eOk;
}

/* OdDbViewportFrontClipOnProperty */

struct OdDbViewportFrontClipOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportFrontClipOnProperty>::createObject();
    ((OdDbViewportFrontClipOnProperty*)res.get())->init(__OD_T("FrontClipOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportFrontClipOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isFrontClipOn());
  return eOk;
}

OdResult OdDbViewportFrontClipOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setFrontClipOn(); else pObj->setFrontClipOff();
  return eOk;
}

/* OdDbViewportUcsPerViewportProperty */

struct OdDbViewportUcsPerViewportProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportUcsPerViewportProperty>::createObject();
    ((OdDbViewportUcsPerViewportProperty*)res.get())->init(__OD_T("UcsPerViewport"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportUcsPerViewportProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"UCS per viewport"));
    ((OdDbViewportUcsPerViewportProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether the UCS is saved with the viewport or not"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbViewportUcsPerViewportProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 30));
    ((OdDbViewportUcsPerViewportProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"UCSPerViewport"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportUcsPerViewportProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isUcsSavedWithViewport());
  return eOk;
}

OdResult OdDbViewportUcsPerViewportProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setUcsPerViewport(val);
  return eOk;
}

/* OdDbViewportViewOrthographicProperty */

struct OdDbViewportViewOrthographicProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportViewOrthographicProperty>::createObject();
    ((OdDbViewportViewOrthographicProperty*)res.get())->init(__OD_T("ViewOrthographic"), &OdRxValueType::Desc<OdDb::OrthographicView>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewportViewOrthographicProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDb::OrthographicView eView; pObj->isUcsOrthographic(eView); value = eView;
  return eOk;
}

/* OdDbViewportShadePlotProperty */

struct OdDbViewportShadePlotProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportShadePlotProperty>::createObject();
    ((OdDbViewportShadePlotProperty*)res.get())->init(__OD_T("ShadePlot"), &OdRxValueType::Desc<OdDbViewport::ShadePlotType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportShadePlotProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Shade plot"));
    ((OdDbViewportShadePlotProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the shade plot mode of the viewport"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbViewportShadePlotProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 45));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportShadePlotProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbViewport::ShadePlotType>(pObj->shadePlot());
  return eOk;
}

OdResult OdDbViewportShadePlotProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbViewport::ShadePlotType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShadePlot(val);
  return eOk;
}

/* OdDbViewportUcsOrthographicProperty */

struct OdDbViewportUcsOrthographicProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportUcsOrthographicProperty>::createObject();
    ((OdDbViewportUcsOrthographicProperty*)res.get())->init(__OD_T("UcsOrthographic"), &OdRxValueType::Desc<OdDb::OrthographicView>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewportUcsOrthographicProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDb::OrthographicView eView; pObj->isUcsOrthographic(eView); value = eView;
  return eOk;
}

/* OdDbViewportNonRectClipEntityIdProperty */

struct OdDbViewportNonRectClipEntityIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportNonRectClipEntityIdProperty>::createObject();
    ((OdDbViewportNonRectClipEntityIdProperty*)res.get())->init(__OD_T("NonRectClipEntityId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbViewportNonRectClipEntityIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportNonRectClipEntityIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->nonRectClipEntityId());
  return eOk;
}

OdResult OdDbViewportNonRectClipEntityIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNonRectClipEntityId(val);
  return eOk;
}

/* OdDbViewportElevationProperty */

struct OdDbViewportElevationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportElevationProperty>::createObject();
    ((OdDbViewportElevationProperty*)res.get())->init(__OD_T("Elevation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportElevationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportElevationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->elevation());
  return eOk;
}

OdResult OdDbViewportElevationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setElevation(val);
  return eOk;
}

/* OdDbViewportUcsNameProperty */

struct OdDbViewportUcsNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportUcsNameProperty>::createObject();
    ((OdDbViewportUcsNameProperty*)res.get())->init(__OD_T("UcsName"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbViewportUcsNameProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewportUcsNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->ucsName());
  return eOk;
}

/* OdDbViewportToneOperatorParametersProperty */

struct OdDbViewportToneOperatorParametersProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportToneOperatorParametersProperty>::createObject();
    ((OdDbViewportToneOperatorParametersProperty*)res.get())->init(__OD_T("ToneOperatorParameters"), &OdRxValueType::Desc<OdGiToneOperatorParametersPtr>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportToneOperatorParametersProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGiToneOperatorParametersPtr params = OdGiToneOperatorParameters::createObject(); pObj->toneOperatorParameters(*params); value = params;
  return eOk;
}

OdResult OdDbViewportToneOperatorParametersProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiToneOperatorParametersPtr val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setToneOperatorParameters(*val);
  return eOk;
}

/* OdDbViewportGridSubdivisionRestrictedProperty */

struct OdDbViewportGridSubdivisionRestrictedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportGridSubdivisionRestrictedProperty>::createObject();
    ((OdDbViewportGridSubdivisionRestrictedProperty*)res.get())->init(__OD_T("GridSubdivisionRestricted"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportGridSubdivisionRestrictedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isGridSubdivisionRestricted());
  return eOk;
}

OdResult OdDbViewportGridSubdivisionRestrictedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridSubdivisionRestricted(val);
  return eOk;
}

/* OdDbViewportShadePlotIdProperty */

struct OdDbViewportShadePlotIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportShadePlotIdProperty>::createObject();
    ((OdDbViewportShadePlotIdProperty*)res.get())->init(__OD_T("ShadePlotId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbViewportShadePlotIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewportShadePlotIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->shadePlotId());
  return eOk;
}

/* OdDbViewportBackgroundProperty */

struct OdDbViewportBackgroundProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportBackgroundProperty>::createObject();
    ((OdDbViewportBackgroundProperty*)res.get())->init(__OD_T("Background"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbViewportBackgroundProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportBackgroundProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->background());
  return eOk;
}

OdResult OdDbViewportBackgroundProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackground(val);
  return eOk;
}

/* OdDbViewportPlotAsRasterProperty */

struct OdDbViewportPlotAsRasterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportPlotAsRasterProperty>::createObject();
    ((OdDbViewportPlotAsRasterProperty*)res.get())->init(__OD_T("PlotAsRaster"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewportPlotAsRasterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->plotAsRaster());
  return eOk;
}

/* OdDbViewportPlotWireframeProperty */

struct OdDbViewportPlotWireframeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportPlotWireframeProperty>::createObject();
    ((OdDbViewportPlotWireframeProperty*)res.get())->init(__OD_T("PlotWireframe"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewportPlotWireframeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->plotWireframe());
  return eOk;
}

/* OdDbViewportNonRectClipOnProperty */

struct OdDbViewportNonRectClipOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportNonRectClipOnProperty>::createObject();
    ((OdDbViewportNonRectClipOnProperty*)res.get())->init(__OD_T("NonRectClipOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportNonRectClipOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isNonRectClipOn());
  return eOk;
}

OdResult OdDbViewportNonRectClipOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setNonRectClipOn(); else pObj->setNonRectClipOff();
  return eOk;
}

/* OdDbViewportContrastProperty */

struct OdDbViewportContrastProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportContrastProperty>::createObject();
    ((OdDbViewportContrastProperty*)res.get())->init(__OD_T("Contrast"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportContrastProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportContrastProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->contrast());
  return eOk;
}

OdResult OdDbViewportContrastProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setContrast(val);
  return eOk;
}

/* OdDbViewportBrightnessProperty */

struct OdDbViewportBrightnessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportBrightnessProperty>::createObject();
    ((OdDbViewportBrightnessProperty*)res.get())->init(__OD_T("Brightness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportBrightnessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportBrightnessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->brightness());
  return eOk;
}

OdResult OdDbViewportBrightnessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBrightness(val);
  return eOk;
}

/* OdDbViewportSunIdProperty */

struct OdDbViewportSunIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportSunIdProperty>::createObject();
    ((OdDbViewportSunIdProperty*)res.get())->init(__OD_T("SunId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbViewportSunIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewportSunIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->sunId());
  return eOk;
}

/* OdDbViewportAmbientLightColorProperty */

struct OdDbViewportAmbientLightColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportAmbientLightColorProperty>::createObject();
    ((OdDbViewportAmbientLightColorProperty*)res.get())->init(__OD_T("AmbientLightColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportAmbientLightColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->ambientLightColor());
  return eOk;
}

OdResult OdDbViewportAmbientLightColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAmbientLightColor(val);
  return eOk;
}

/* OdDbViewportGridMajorProperty */

struct OdDbViewportGridMajorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportGridMajorProperty>::createObject();
    ((OdDbViewportGridMajorProperty*)res.get())->init(__OD_T("GridMajor"), &OdRxValueType::Desc<short>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportGridMajorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->gridMajor());
  return eOk;
}

OdResult OdDbViewportGridMajorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridMajor(val);
  return eOk;
}

/* OdDbViewportGridFollowProperty */

struct OdDbViewportGridFollowProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportGridFollowProperty>::createObject();
    ((OdDbViewportGridFollowProperty*)res.get())->init(__OD_T("GridFollow"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportGridFollowProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isGridFollow());
  return eOk;
}

OdResult OdDbViewportGridFollowProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGridFollow(val);
  return eOk;
}

/* OdDbViewportDefaultLightingTypeProperty */

struct OdDbViewportDefaultLightingTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportDefaultLightingTypeProperty>::createObject();
    ((OdDbViewportDefaultLightingTypeProperty*)res.get())->init(__OD_T("DefaultLightingType"), &OdRxValueType::Desc<OdGiViewportTraits::DefaultLightingType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportDefaultLightingTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiViewportTraits::DefaultLightingType>(pObj->defaultLightingType());
  return eOk;
}

OdResult OdDbViewportDefaultLightingTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiViewportTraits::DefaultLightingType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDefaultLightingType(val);
  return eOk;
}

/* OdDbViewportDefaultLightingOnProperty */

struct OdDbViewportDefaultLightingOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportDefaultLightingOnProperty>::createObject();
    ((OdDbViewportDefaultLightingOnProperty*)res.get())->init(__OD_T("DefaultLightingOn"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportDefaultLightingOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isDefaultLightingOn());
  return eOk;
}

OdResult OdDbViewportDefaultLightingOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDefaultLightingOn(val);
  return eOk;
}

/* OdDbViewportStandardScaleProperty */

struct OdDbViewportStandardScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportStandardScaleProperty>::createObject();
    ((OdDbViewportStandardScaleProperty*)res.get())->init(__OD_T("StandardScale"), &OdRxValueType::Desc<OdDbViewport::StandardScaleType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportStandardScaleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Standard scale"));
    ((OdDbViewportStandardScaleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the standard scale for the viewport"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbViewportStandardScaleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 16));
    ((OdDbViewportStandardScaleProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"StandardScale2"));
    ((OdDbViewportStandardScaleProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportStandardScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbViewport::StandardScaleType>(pObj->standardScale());
  return eOk;
}

OdResult OdDbViewportStandardScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbViewport::StandardScaleType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStandardScale(val);
  return eOk;
}

/* OdDbViewportPlotStyleSheetProperty */

struct OdDbViewportPlotStyleSheetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportPlotStyleSheetProperty>::createObject();
    ((OdDbViewportPlotStyleSheetProperty*)res.get())->init(__OD_T("PlotStyleSheet"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportPlotStyleSheetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->plotStyleSheet());
  return eOk;
}

OdResult OdDbViewportPlotStyleSheetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPlotStyleSheet(val);
  return eOk;
}

/* OdDbViewportEffectivePlotStyleSheetProperty */

struct OdDbViewportEffectivePlotStyleSheetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportEffectivePlotStyleSheetProperty>::createObject();
    ((OdDbViewportEffectivePlotStyleSheetProperty*)res.get())->init(__OD_T("EffectivePlotStyleSheet"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewportEffectivePlotStyleSheetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->effectivePlotStyleSheet());
  return eOk;
}

/* OdDbViewportCustomScaleProperty */

struct OdDbViewportCustomScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportCustomScaleProperty>::createObject();
    ((OdDbViewportCustomScaleProperty*)res.get())->init(__OD_T("CustomScale"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbViewportCustomScaleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportCustomScaleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Custom scale"));
    ((OdDbViewportCustomScaleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the custom scale for the viewport"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbViewportCustomScaleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 20));
    ((OdDbViewportCustomScaleProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportCustomScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->customScale());
  return eOk;
}

OdResult OdDbViewportCustomScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCustomScale(val);
  return eOk;
}

/* OdDbViewportLockedProperty */

struct OdDbViewportLockedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportLockedProperty>::createObject();
    ((OdDbViewportLockedProperty*)res.get())->init(__OD_T("Locked"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportLockedProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Display locked"));
    ((OdDbViewportLockedProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether viewport is in locked state or not"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbViewportLockedProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 10));
    ((OdDbViewportLockedProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DisplayLocked"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportLockedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isLocked());
  return eOk;
}

OdResult OdDbViewportLockedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setLocked(); else pObj->setUnlocked();
  return eOk;
}

/* OdDbViewportVisualStyleIdProperty */

struct OdDbViewportVisualStyleIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportVisualStyleIdProperty>::createObject();
    ((OdDbViewportVisualStyleIdProperty*)res.get())->init(__OD_T("VisualStyleId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbViewportVisualStyleIdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Visual style"));
    ((OdDbViewportVisualStyleIdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the visual style of the viewport"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbViewportVisualStyleIdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 40));
    ((OdDbViewportVisualStyleIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/VisualStyleDictionaryId/Items"));
    ((OdDbViewportVisualStyleIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportVisualStyleIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->visualStyle());
  return eOk;
}

OdResult OdDbViewportVisualStyleIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setVisualStyle(val);
  return eOk;
}

/* OdDbViewportTransparentProperty */

struct OdDbViewportTransparentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewportTransparentProperty>::createObject();
    ((OdDbViewportTransparentProperty*)res.get())->init(__OD_T("Transparent"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewportTransparentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isTransparent());
  return eOk;
}

OdResult OdDbViewportTransparentProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewportPtr pObj = OdDbViewport::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setTransparent(); else pObj->setOpaque();
  return eOk;
}

void createOdDbViewportProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbViewportSnapIsometricProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportSnapOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportSnapBasePointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportSnapAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportUcsIconVisibleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportUcsFollowModeOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportCircleSidesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportUcsIconAtOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportGridBoundToLimitsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportHiddenLinesRemovedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportGridAdaptiveProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportSnapIsoPairProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportSnapIncrementProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportGridIncrementProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportGridOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportViewDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportViewTargetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportViewCenterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportViewHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportCenterPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportNumberProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportFrontClipDistanceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportFrontClipAtEyeOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportPerspectiveOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportBackClipDistanceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportLensLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTwistAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportBackClipOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportFrontClipOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportUcsPerViewportProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportViewOrthographicProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportShadePlotProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportUcsOrthographicProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportNonRectClipEntityIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportElevationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportUcsNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportToneOperatorParametersProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportGridSubdivisionRestrictedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportShadePlotIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportBackgroundProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportPlotAsRasterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportPlotWireframeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportNonRectClipOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportContrastProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportBrightnessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportSunIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportAmbientLightColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportGridMajorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportGridFollowProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportDefaultLightingTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportDefaultLightingOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportStandardScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportPlotStyleSheetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportEffectivePlotStyleSheetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportCustomScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportLockedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportVisualStyleIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewportTransparentProperty::createObject(b.owner()));
  b.add(properties.last());
}
