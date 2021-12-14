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
#include "DbArcDimension.h"
#include "DbValueTypes.h"

/* OdDbArcDimensionCenterPointProperty */

struct OdDbArcDimensionCenterPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionCenterPointProperty>::createObject();
    ((OdDbArcDimensionCenterPointProperty*)res.get())->init(__OD_T("CenterPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    ((OdDbArcDimensionCenterPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the center of the arc"));
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

OdResult OdDbArcDimensionCenterPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->centerPoint());
  return eOk;
}

OdResult OdDbArcDimensionCenterPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCenterPoint(val);
  return eOk;
}

/* OdDbArcDimensionLeader1PointProperty */

struct OdDbArcDimensionLeader1PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionLeader1PointProperty>::createObject();
    ((OdDbArcDimensionLeader1PointProperty*)res.get())->init(__OD_T("Leader1Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    ((OdDbArcDimensionLeader1PointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the origin of leader 1"));
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

OdResult OdDbArcDimensionLeader1PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->leader1Point());
  return eOk;
}

OdResult OdDbArcDimensionLeader1PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeader1Point(val);
  return eOk;
}

/* OdDbArcDimensionArcPointProperty */

struct OdDbArcDimensionArcPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionArcPointProperty>::createObject();
    ((OdDbArcDimensionArcPointProperty*)res.get())->init(__OD_T("ArcPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    ((OdDbArcDimensionArcPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies a point on the arc"));
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

OdResult OdDbArcDimensionArcPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->arcPoint());
  return eOk;
}

OdResult OdDbArcDimensionArcPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArcPoint(val);
  return eOk;
}

/* OdDbArcDimensionArcStartParamProperty */

struct OdDbArcDimensionArcStartParamProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionArcStartParamProperty>::createObject();
    ((OdDbArcDimensionArcStartParamProperty*)res.get())->init(__OD_T("ArcStartParam"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbArcDimensionArcStartParamProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    ((OdDbArcDimensionArcStartParamProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the start parameter of the arc"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbArcDimensionArcStartParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->arcStartParam());
  return eOk;
}

OdResult OdDbArcDimensionArcStartParamProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArcStartParam(val);
  return eOk;
}

/* OdDbArcDimensionArcEndParamProperty */

struct OdDbArcDimensionArcEndParamProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionArcEndParamProperty>::createObject();
    ((OdDbArcDimensionArcEndParamProperty*)res.get())->init(__OD_T("ArcEndParam"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbArcDimensionArcEndParamProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    ((OdDbArcDimensionArcEndParamProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the end parameter of the arc"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbArcDimensionArcEndParamProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->arcEndParam());
  return eOk;
}

OdResult OdDbArcDimensionArcEndParamProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArcEndParam(val);
  return eOk;
}

/* OdDbArcDimensionLeader2PointProperty */

struct OdDbArcDimensionLeader2PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionLeader2PointProperty>::createObject();
    ((OdDbArcDimensionLeader2PointProperty*)res.get())->init(__OD_T("Leader2Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    ((OdDbArcDimensionLeader2PointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the origin of leader 2"));
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

OdResult OdDbArcDimensionLeader2PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->leader2Point());
  return eOk;
}

OdResult OdDbArcDimensionLeader2PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeader2Point(val);
  return eOk;
}

/* OdDbArcDimensionIsPartialProperty */

struct OdDbArcDimensionIsPartialProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionIsPartialProperty>::createObject();
    ((OdDbArcDimensionIsPartialProperty*)res.get())->init(__OD_T("IsPartial"), &OdRxValueType::Desc<bool>::value(), owner);
    ((OdDbArcDimensionIsPartialProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the dimension is for a partial arc"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbArcDimensionIsPartialProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isPartial());
  return eOk;
}

OdResult OdDbArcDimensionIsPartialProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsPartial(val);
  return eOk;
}

/* OdDbArcDimensionHasLeaderProperty */

struct OdDbArcDimensionHasLeaderProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionHasLeaderProperty>::createObject();
    ((OdDbArcDimensionHasLeaderProperty*)res.get())->init(__OD_T("HasLeader"), &OdRxValueType::Desc<bool>::value(), owner);
    ((OdDbArcDimensionHasLeaderProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the whether the dimension has leader"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbArcDimensionHasLeaderProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasLeader());
  return eOk;
}

OdResult OdDbArcDimensionHasLeaderProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHasLeader(val);
  return eOk;
}

/* OdDbArcDimensionArcSymbolTypeProperty */

struct OdDbArcDimensionArcSymbolTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionArcSymbolTypeProperty>::createObject();
    ((OdDbArcDimensionArcSymbolTypeProperty*)res.get())->init(__OD_T("ArcSymbolType"), &OdRxValueType::Desc<OdInt16>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbArcDimensionArcSymbolTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Arc length symbol"));
    ((OdDbArcDimensionArcSymbolTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies placement of the arc length dimension symbol (DIMARCSYM system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbArcDimensionArcSymbolTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 32));
    ((OdDbArcDimensionArcSymbolTypeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"SymbolPosition"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbArcDimensionArcSymbolTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdInt16>(pObj->arcSymbolType());
  return eOk;
}

OdResult OdDbArcDimensionArcSymbolTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdInt16 val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArcSymbolType(val);
  return eOk;
}

/* OdDbArcDimensionXLine1PointProperty */

struct OdDbArcDimensionXLine1PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionXLine1PointProperty>::createObject();
    ((OdDbArcDimensionXLine1PointProperty*)res.get())->init(__OD_T("XLine1Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    ((OdDbArcDimensionXLine1PointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the origin of extension line 1"));
    ((OdDbArcDimensionXLine1PointProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ExtLine1Point"));
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

OdResult OdDbArcDimensionXLine1PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine1Point());
  return eOk;
}

OdResult OdDbArcDimensionXLine1PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine1Point(val);
  return eOk;
}

/* OdDbArcDimensionXLine2PointProperty */

struct OdDbArcDimensionXLine2PointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbArcDimensionXLine2PointProperty>::createObject();
    ((OdDbArcDimensionXLine2PointProperty*)res.get())->init(__OD_T("XLine2Point"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    ((OdDbArcDimensionXLine2PointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the origin of extension line 2"));
    ((OdDbArcDimensionXLine2PointProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ExtLine2Point"));
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

OdResult OdDbArcDimensionXLine2PointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->xLine2Point());
  return eOk;
}

OdResult OdDbArcDimensionXLine2PointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbArcDimensionPtr pObj = OdDbArcDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXLine2Point(val);
  return eOk;
}

void createOdDbArcDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbArcDimensionCenterPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionLeader1PointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionArcPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionArcStartParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionArcEndParamProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionLeader2PointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionIsPartialProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionHasLeaderProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionArcSymbolTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionXLine1PointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbArcDimensionXLine2PointProperty::createObject(b.owner()));
  b.add(properties.last());
}
