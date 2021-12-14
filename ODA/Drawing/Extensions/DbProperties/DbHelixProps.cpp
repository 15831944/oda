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
#include "DbHelix.h"
#include "DbValueTypes.h"

/* OdDbHelixTurnSlopeProperty */

struct OdDbHelixTurnSlopeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixTurnSlopeProperty>::createObject();
    ((OdDbHelixTurnSlopeProperty*)res.get())->init(__OD_T("TurnSlope"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHelixTurnSlopeProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixTurnSlopeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Turn Slope"));
    ((OdDbHelixTurnSlopeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Displays the constant incline angle for the helix path"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixTurnSlopeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 12));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHelixTurnSlopeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->turnSlope());
  return eOk;
}

/* OdDbHelixTotalLengthProperty */

struct OdDbHelixTotalLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixTotalLengthProperty>::createObject();
    ((OdDbHelixTotalLengthProperty*)res.get())->init(__OD_T("TotalLength"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHelixTotalLengthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixTotalLengthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Total length"));
    ((OdDbHelixTotalLengthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the total length of the helix"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixTotalLengthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 13));
    ((OdDbHelixTotalLengthProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbHelixTotalLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->totalLength());
  return eOk;
}

/* OdDbHelixHeightProperty */

struct OdDbHelixHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixHeightProperty>::createObject();
    ((OdDbHelixHeightProperty*)res.get())->init(__OD_T("Height"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHelixHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Height"));
    ((OdDbHelixHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the height of the helix"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 6));
    ((OdDbHelixHeightProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHelixHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->height());
  return eOk;
}

OdResult OdDbHelixHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHeight(val);
  return eOk;
}

/* OdDbHelixBaseRadiusProperty */

struct OdDbHelixBaseRadiusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixBaseRadiusProperty>::createObject();
    ((OdDbHelixBaseRadiusProperty*)res.get())->init(__OD_T("BaseRadius"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHelixBaseRadiusProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixBaseRadiusProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Base Radius"));
    ((OdDbHelixBaseRadiusProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the base radius of the helix"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixBaseRadiusProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 9));
    ((OdDbHelixBaseRadiusProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHelixBaseRadiusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->baseRadius());
  return eOk;
}

OdResult OdDbHelixBaseRadiusProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBaseRadius(val);
  return eOk;
}

/* OdDbHelixAxisVectorProperty */

struct OdDbHelixAxisVectorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixAxisVectorProperty>::createObject();
    ((OdDbHelixAxisVectorProperty*)res.get())->init(__OD_T("AxisVector"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbHelixAxisVectorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->axisVector());
  return eOk;
}

OdResult OdDbHelixAxisVectorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAxisVector(val);
  return eOk;
}

/* OdDbHelixTopRadiusProperty */

struct OdDbHelixTopRadiusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixTopRadiusProperty>::createObject();
    ((OdDbHelixTopRadiusProperty*)res.get())->init(__OD_T("TopRadius"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHelixTopRadiusProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixTopRadiusProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Top Radius"));
    ((OdDbHelixTopRadiusProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the top radius of the helix"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixTopRadiusProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 10));
    ((OdDbHelixTopRadiusProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHelixTopRadiusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->topRadius());
  return eOk;
}

OdResult OdDbHelixTopRadiusProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTopRadius(val);
  return eOk;
}

/* OdDbHelixTwistProperty */

struct OdDbHelixTwistProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixTwistProperty>::createObject();
    ((OdDbHelixTwistProperty*)res.get())->init(__OD_T("Twist"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixTwistProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Counterclockwise"));
    ((OdDbHelixTwistProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Controls the twist direction of the helix"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixTwistProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 11));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHelixTwistProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->twist());
  return eOk;
}

OdResult OdDbHelixTwistProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTwist(val);
  return eOk;
}

/* OdDbHelixPositionProperty */

struct OdDbHelixPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixPositionProperty>::createObject();
    ((OdDbHelixPositionProperty*)res.get())->init(__OD_T("Position"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixPositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position"));
    ((OdDbHelixPositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the X, Y, and Z for the center of the base of the helix"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixPositionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 4));
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

OdResult OdDbHelixPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGePoint3d>(pObj->startPoint());
  return eOk;
}

OdResult OdDbHelixPositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setStartPoint(val);
  return eOk;
}

/* OdDbHelixConstrainProperty */

struct OdDbHelixConstrainProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixConstrainProperty>::createObject();
    ((OdDbHelixConstrainProperty*)res.get())->init(__OD_T("Constrain"), &OdRxValueType::Desc<OdDbHelix::ConstrainType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixConstrainProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Constrain"));
    ((OdDbHelixConstrainProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Controls which property is constrained when editing other property values"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixConstrainProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 5));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHelixConstrainProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbHelix::ConstrainType>(pObj->constrain());
  return eOk;
}

OdResult OdDbHelixConstrainProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbHelix::ConstrainType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setConstrain(val);
  return eOk;
}

/* OdDbHelixTurnsProperty */

struct OdDbHelixTurnsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixTurnsProperty>::createObject();
    ((OdDbHelixTurnsProperty*)res.get())->init(__OD_T("Turns"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHelixTurnsProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixTurnsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Turns"));
    ((OdDbHelixTurnsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of turns for the helix"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixTurnsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 7));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHelixTurnsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->turns());
  return eOk;
}

OdResult OdDbHelixTurnsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTurns(val);
  return eOk;
}

/* OdDbHelixTurnHeightProperty */

struct OdDbHelixTurnHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbHelixTurnHeightProperty>::createObject();
    ((OdDbHelixTurnHeightProperty*)res.get())->init(__OD_T("TurnHeight"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbHelixTurnHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbHelixTurnHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Turn Height"));
    ((OdDbHelixTurnHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the height of one full turn for the helix"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbHelixTurnHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 8));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbHelixTurnHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->turnHeight());
  return eOk;
}

OdResult OdDbHelixTurnHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbHelixPtr pObj = OdDbHelix::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTurnHeight(val);
  return eOk;
}

void createOdDbHelixProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbHelixTurnSlopeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixTotalLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixBaseRadiusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixAxisVectorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixTopRadiusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixTwistProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixConstrainProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixTurnsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbHelixTurnHeightProperty::createObject(b.owner()));
  b.add(properties.last());
}
