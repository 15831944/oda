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
#include "DbDimension.h"
#include "DbValueTypes.h"

/* OdDbDimensionConstraintObjectProperty */

struct OdDbDimensionConstraintObjectProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionConstraintObjectProperty>::createObject();
    ((OdDbDimensionConstraintObjectProperty*)res.get())->init(__OD_T("ConstraintObject"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDimensionConstraintObjectProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isConstraintObject());
  return eOk;
}

/* OdDbDimensionHorizontalRotationProperty */

struct OdDbDimensionHorizontalRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionHorizontalRotationProperty>::createObject();
    ((OdDbDimensionHorizontalRotationProperty*)res.get())->init(__OD_T("HorizontalRotation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionHorizontalRotationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionHorizontalRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->horizontalRotation());
  return eOk;
}

OdResult OdDbDimensionHorizontalRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHorizontalRotation(val);
  return eOk;
}

/* OdDbDimensionDimBlockIdProperty */

struct OdDbDimensionDimBlockIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimBlockIdProperty>::createObject();
    ((OdDbDimensionDimBlockIdProperty*)res.get())->init(__OD_T("DimBlockId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimensionDimBlockIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimBlockIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimBlockId());
  return eOk;
}

OdResult OdDbDimensionDimBlockIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimBlockId(val);
  return eOk;
}

/* OdDbDimensionTextLineSpacingStyleProperty */

struct OdDbDimensionTextLineSpacingStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionTextLineSpacingStyleProperty>::createObject();
    ((OdDbDimensionTextLineSpacingStyleProperty*)res.get())->init(__OD_T("TextLineSpacingStyle"), &OdRxValueType::Desc<OdDb::LineSpacingStyle>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionTextLineSpacingStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineSpacingStyle>(pObj->textLineSpacingStyle());
  return eOk;
}

OdResult OdDbDimensionTextLineSpacingStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineSpacingStyle val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextLineSpacingStyle(val);
  return eOk;
}

/* OdDbDimensionTextLineSpacingFactorProperty */

struct OdDbDimensionTextLineSpacingFactorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionTextLineSpacingFactorProperty>::createObject();
    ((OdDbDimensionTextLineSpacingFactorProperty*)res.get())->init(__OD_T("TextLineSpacingFactor"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionTextLineSpacingFactorProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionTextLineSpacingFactorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->textLineSpacingFactor());
  return eOk;
}

OdResult OdDbDimensionTextLineSpacingFactorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextLineSpacingFactor(val);
  return eOk;
}

/* OdDbDimensionDynamicDimensionProperty */

struct OdDbDimensionDynamicDimensionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDynamicDimensionProperty>::createObject();
    ((OdDbDimensionDynamicDimensionProperty*)res.get())->init(__OD_T("DynamicDimension"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDynamicDimensionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isDynamicDimension());
  return eOk;
}

OdResult OdDbDimensionDynamicDimensionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDynamicDimension(val);
  return eOk;
}

/* OdDbDimensionDimBlockPositionProperty */

struct OdDbDimensionDimBlockPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimBlockPositionProperty>::createObject();
    ((OdDbDimensionDimBlockPositionProperty*)res.get())->init(__OD_T("DimBlockPosition"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbDimensionDimBlockPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->dimBlockPosition());
  return eOk;
}

/* OdDbDimensionMeasurementProperty */

struct OdDbDimensionMeasurementProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionMeasurementProperty>::createObject();
    ((OdDbDimensionMeasurementProperty*)res.get())->init(__OD_T("Measurement"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionMeasurementProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionMeasurementProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Measurement"));
    ((OdDbDimensionMeasurementProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies dimension measurement value"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionMeasurementProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 33));
    ((OdDbDimensionMeasurementProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDimensionMeasurementProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->measurement());
  return eOk;
}

/* OdDbDimensionTextAttachmentProperty */

struct OdDbDimensionTextAttachmentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionTextAttachmentProperty>::createObject();
    ((OdDbDimensionTextAttachmentProperty*)res.get())->init(__OD_T("TextAttachment"), &OdRxValueType::Desc<OdDbMText::AttachmentPoint>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionTextAttachmentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMText::AttachmentPoint>(pObj->textAttachment());
  return eOk;
}

OdResult OdDbDimensionTextAttachmentProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMText::AttachmentPoint val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextAttachment(val);
  return eOk;
}

/* OdDbDimensionNormalProperty */

struct OdDbDimensionNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionNormalProperty>::createObject();
    ((OdDbDimensionNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbDimensionNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbDimensionNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDbDimensionUsingDefaultTextPositionProperty */

struct OdDbDimensionUsingDefaultTextPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionUsingDefaultTextPositionProperty>::createObject();
    ((OdDbDimensionUsingDefaultTextPositionProperty*)res.get())->init(__OD_T("UsingDefaultTextPosition"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionUsingDefaultTextPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isUsingDefaultTextPosition());
  return eOk;
}

OdResult OdDbDimensionUsingDefaultTextPositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->useDefaultTextPosition(); else pObj->useSetTextPosition();
  return eOk;
}

/* OdDbDimensionTextPositionProperty */

struct OdDbDimensionTextPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionTextPositionProperty>::createObject();
    ((OdDbDimensionTextPositionProperty*)res.get())->init(__OD_T("TextPosition"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionTextPositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text position X;Text position Y"));
    ((OdDbDimensionTextPositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the dimension text position or pick point"));
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the dimension text position or pick point"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 30));
    res = OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the dimension text position or pick point"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 30));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionTextPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->textPosition());
  return eOk;
}

OdResult OdDbDimensionTextPositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextPosition(val);
  return eOk;
}

/* OdDbDimensionElevationProperty */

struct OdDbDimensionElevationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionElevationProperty>::createObject();
    ((OdDbDimensionElevationProperty*)res.get())->init(__OD_T("Elevation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionElevationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionElevationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->elevation());
  return eOk;
}

OdResult OdDbDimensionElevationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setElevation(val);
  return eOk;
}

/* OdDbDimensionDimensionStyleProperty */

struct OdDbDimensionDimensionStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimensionStyleProperty>::createObject();
    ((OdDbDimensionDimensionStyleProperty*)res.get())->init(__OD_T("DimensionStyle"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimensionDimensionStyleProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/DimStyleTableId/Items"));
    ((OdDbDimensionDimensionStyleProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimensionStyleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim style"));
    ((OdDbDimensionDimensionStyleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current dimension style by name (for DIMSTYLE system variable use SETVAR)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbDimensionDimensionStyleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 1));
    ((OdDbDimensionDimensionStyleProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimensionStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimensionStyle());
  return eOk;
}

OdResult OdDbDimensionDimensionStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimensionStyle(val);
  return eOk;
}

/* OdDbDimensionTextRotationProperty */

struct OdDbDimensionTextRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionTextRotationProperty>::createObject();
    ((OdDbDimensionTextRotationProperty*)res.get())->init(__OD_T("TextRotation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionTextRotationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionTextRotationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text rotation"));
    ((OdDbDimensionTextRotationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rotation angle of the dimension text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionTextRotationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 31));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionTextRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->textRotation());
  return eOk;
}

OdResult OdDbDimensionTextRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextRotation(val);
  return eOk;
}

/* OdDbDimensionDimensionTextProperty */

struct OdDbDimensionDimensionTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimensionTextProperty>::createObject();
    ((OdDbDimensionDimensionTextProperty*)res.get())->init(__OD_T("DimensionText"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimensionTextProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text override"));
    ((OdDbDimensionDimensionTextProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text string of the dimension (overrides Measurement string)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionDimensionTextProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 34));
    ((OdDbDimensionDimensionTextProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbDimensionDimensionTextProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextOverride"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimensionTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->dimensionText());
  return eOk;
}

OdResult OdDbDimensionDimensionTextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimensionText(val);
  return eOk;
}

/* OdDbDimensionDimgapProperty */

struct OdDbDimensionDimgapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimgapProperty>::createObject();
    ((OdDbDimensionDimgapProperty*)res.get())->init(__OD_T("Dimgap"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimgapProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimgapProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text offset"));
    ((OdDbDimensionDimgapProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies distance around dimension text when dimension line breaks for dimension text (DIMGAP system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionDimgapProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 24));
    ((OdDbDimensionDimgapProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextGap"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimgapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimgap());
  return eOk;
}

OdResult OdDbDimensionDimgapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimgap(val);
  return eOk;
}

/* OdDbDimensionDimldrblkProperty */

struct OdDbDimensionDimldrblkProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimldrblkProperty>::createObject();
    ((OdDbDimensionDimldrblkProperty*)res.get())->init(__OD_T("Dimldrblk"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimensionDimldrblkProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimldrblkProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimldrblk());
  return eOk;
}

OdResult OdDbDimensionDimldrblkProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimldrblk(val);
  return eOk;
}

/* OdDbDimensionDimexoProperty */

struct OdDbDimensionDimexoProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimexoProperty>::createObject();
    ((OdDbDimensionDimexoProperty*)res.get())->init(__OD_T("Dimexo"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimexoProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimexoProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Ext line offset"));
    ((OdDbDimensionDimexoProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies offset of extension lines from the origin points (DIMEXO system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbDimensionDimexoProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 24));
    ((OdDbDimensionDimexoProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ExtensionLineOffset"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimexoProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimexo());
  return eOk;
}

OdResult OdDbDimensionDimexoProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimexo(val);
  return eOk;
}

/* OdDbDimensionDimlweProperty */

struct OdDbDimensionDimlweProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimlweProperty>::createObject();
    ((OdDbDimensionDimlweProperty*)res.get())->init(__OD_T("Dimlwe"), &OdRxValueType::Desc<OdDb::LineWeight>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimlweProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Ext line lineweight"));
    ((OdDbDimensionDimlweProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies lineweight for extension lines (DIMLWE system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbDimensionDimlweProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 8));
    ((OdDbDimensionDimlweProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ExtensionLineWeight"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimlweProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineWeight>(pObj->dimlwe());
  return eOk;
}

OdResult OdDbDimensionDimlweProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineWeight val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimlwe(val);
  return eOk;
}

/* OdDbDimensionDimlimProperty */

struct OdDbDimensionDimlimProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimlimProperty>::createObject();
    ((OdDbDimensionDimlimProperty*)res.get())->init(__OD_T("Dimlim"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimlimProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimlim());
  return eOk;
}

OdResult OdDbDimensionDimlimProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimlim(val);
  return eOk;
}

/* OdDbDimensionDimdsepProperty */

struct OdDbDimensionDimdsepProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimdsepProperty>::createObject();
    ((OdDbDimensionDimdsepProperty*)res.get())->init(__OD_T("Dimdsep"), &OdRxValueType::Desc<OdInt16>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimdsepProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Decimal separator"));
    ((OdDbDimensionDimdsepProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the decimal separator for metric dimensions (DIMDSEP system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Primary Units"))
      OdRxCategory::createObject(L"Primary Units", OdRxCategory::rootCategory());
    ((OdDbDimensionDimdsepProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Primary Units", 41));
    ((OdDbDimensionDimdsepProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DecimalSeparator"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimdsepProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdInt16>(pObj->dimdsep());
  return eOk;
}

OdResult OdDbDimensionDimdsepProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdInt16 val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimdsep(val);
  return eOk;
}

/* OdDbDimensionDimalttzProperty */

struct OdDbDimensionDimalttzProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimalttzProperty>::createObject();
    ((OdDbDimensionDimalttzProperty*)res.get())->init(__OD_T("Dimalttz"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimalttzProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimalttz());
  return eOk;
}

OdResult OdDbDimensionDimalttzProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimalttz(val);
  return eOk;
}

/* OdDbDimensionDimarcsymProperty */

struct OdDbDimensionDimarcsymProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimarcsymProperty>::createObject();
    ((OdDbDimensionDimarcsymProperty*)res.get())->init(__OD_T("Dimarcsym"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimarcsymProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimarcsym());
  return eOk;
}

OdResult OdDbDimensionDimarcsymProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimarcsym(val);
  return eOk;
}

/* OdDbDimensionDimaltzProperty */

struct OdDbDimensionDimaltzProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimaltzProperty>::createObject();
    ((OdDbDimensionDimaltzProperty*)res.get())->init(__OD_T("Dimaltz"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimaltzProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimaltz());
  return eOk;
}

OdResult OdDbDimensionDimaltzProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimaltz(val);
  return eOk;
}

/* OdDbDimensionDimclrtProperty */

struct OdDbDimensionDimclrtProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimclrtProperty>::createObject();
    ((OdDbDimensionDimclrtProperty*)res.get())->init(__OD_T("Dimclrt"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimclrtProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text color"));
    ((OdDbDimensionDimclrtProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the color of the dimension text (DIMCLRT system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionDimclrtProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 22));
    ((OdDbDimensionDimclrtProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextColor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimclrtProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimclrt());
  return eOk;
}

OdResult OdDbDimensionDimclrtProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimclrt(val);
  return eOk;
}

/* OdDbDimensionDimclreProperty */

struct OdDbDimensionDimclreProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimclreProperty>::createObject();
    ((OdDbDimensionDimclreProperty*)res.get())->init(__OD_T("Dimclre"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimclreProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Ext line color"));
    ((OdDbDimensionDimclreProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies color of the extension line (DIMCLRE system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbDimensionDimclreProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 22));
    ((OdDbDimensionDimclreProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ExtensionLineColor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimclreProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimclre());
  return eOk;
}

OdResult OdDbDimensionDimclreProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimclre(val);
  return eOk;
}

/* OdDbDimensionDimdliProperty */

struct OdDbDimensionDimdliProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimdliProperty>::createObject();
    ((OdDbDimensionDimdliProperty*)res.get())->init(__OD_T("Dimdli"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimdliProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimdliProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimdli());
  return eOk;
}

OdResult OdDbDimensionDimdliProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimdli(val);
  return eOk;
}

/* OdDbDimensionDimaszProperty */

struct OdDbDimensionDimaszProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimaszProperty>::createObject();
    ((OdDbDimensionDimaszProperty*)res.get())->init(__OD_T("Dimasz"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimaszProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimaszProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Arrow size"));
    ((OdDbDimensionDimaszProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies size of the dimension arrowhead (DIMASZ system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbDimensionDimaszProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 4));
    ((OdDbDimensionDimaszProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ArrowheadSize"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimaszProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimasz());
  return eOk;
}

OdResult OdDbDimensionDimaszProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimasz(val);
  return eOk;
}

/* OdDbDimensionDimcenProperty */

struct OdDbDimensionDimcenProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimcenProperty>::createObject();
    ((OdDbDimensionDimcenProperty*)res.get())->init(__OD_T("Dimcen"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimcenProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimcenProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimcen());
  return eOk;
}

OdResult OdDbDimensionDimcenProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimcen(val);
  return eOk;
}

/* OdDbDimensionDimazinProperty */

struct OdDbDimensionDimazinProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimazinProperty>::createObject();
    ((OdDbDimensionDimazinProperty*)res.get())->init(__OD_T("Dimazin"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimazinProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimazin());
  return eOk;
}

OdResult OdDbDimensionDimazinProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimazin(val);
  return eOk;
}

/* OdDbDimensionDimtolProperty */

struct OdDbDimensionDimtolProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtolProperty>::createObject();
    ((OdDbDimensionDimtolProperty*)res.get())->init(__OD_T("Dimtol"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtolProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimtol());
  return eOk;
}

OdResult OdDbDimensionDimtolProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtol(val);
  return eOk;
}

/* OdDbDimensionDimtohProperty */

struct OdDbDimensionDimtohProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtohProperty>::createObject();
    ((OdDbDimensionDimtohProperty*)res.get())->init(__OD_T("Dimtoh"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtohProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text outside align"));
    ((OdDbDimensionDimtohProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Sets positioning of dimension text outside extension lines On or Off (DIMTOH system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtohProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 25));
    ((OdDbDimensionDimtohProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextOutsideAlign"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtohProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimtoh());
  return eOk;
}

OdResult OdDbDimensionDimtohProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtoh(val);
  return eOk;
}

/* OdDbDimensionDimtoljProperty */

struct OdDbDimensionDimtoljProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtoljProperty>::createObject();
    ((OdDbDimensionDimtoljProperty*)res.get())->init(__OD_T("Dimtolj"), &OdRxValueType::Desc<OdUInt8>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtoljProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Tolerance pos vert"));
    ((OdDbDimensionDimtoljProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies vertical justification for tolerance values relative to nominal dimension text (DIMTOLJ system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Tolerances"))
      OdRxCategory::createObject(L"Tolerances", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtoljProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Tolerances", 72));
    ((OdDbDimensionDimtoljProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ToleranceJustification"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtoljProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdUInt8>(pObj->dimtolj());
  return eOk;
}

OdResult OdDbDimensionDimtoljProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdUInt8 val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtolj(val);
  return eOk;
}

/* OdDbDimensionDimtvpProperty */

struct OdDbDimensionDimtvpProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtvpProperty>::createObject();
    ((OdDbDimensionDimtvpProperty*)res.get())->init(__OD_T("Dimtvp"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimtvpProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtvpProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtvp());
  return eOk;
}

OdResult OdDbDimensionDimtvpProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtvp(val);
  return eOk;
}

/* OdDbDimensionDimtszProperty */

struct OdDbDimensionDimtszProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtszProperty>::createObject();
    ((OdDbDimensionDimtszProperty*)res.get())->init(__OD_T("Dimtsz"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimtszProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtszProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtsz());
  return eOk;
}

OdResult OdDbDimensionDimtszProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtsz(val);
  return eOk;
}

/* OdDbDimensionDimtpProperty */

struct OdDbDimensionDimtpProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtpProperty>::createObject();
    ((OdDbDimensionDimtpProperty*)res.get())->init(__OD_T("Dimtp"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimtpProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtpProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Tolerance limit upper"));
    ((OdDbDimensionDimtpProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the maximum (or upper) tolerance limit for dimension text when DIMTOL or DIMLIM is on (DIMTP sysem variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Tolerances"))
      OdRxCategory::createObject(L"Tolerances", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtpProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Tolerances", 71));
    ((OdDbDimensionDimtpProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ToleranceUpperLimit"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDimensionDimtpProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtp());
  return eOk;
}

/* OdDbDimensionDimtfillclrProperty */

struct OdDbDimensionDimtfillclrProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtfillclrProperty>::createObject();
    ((OdDbDimensionDimtfillclrProperty*)res.get())->init(__OD_T("Dimtfillclr"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    ((OdDbDimensionDimtfillclrProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Sets text fill color (DIMTFILLCLR system variable)"));
    ((OdDbDimensionDimtfillclrProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextFillColor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtfillclrProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimtfillclr());
  return eOk;
}

OdResult OdDbDimensionDimtfillclrProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtfillclr(val);
  return eOk;
}

/* OdDbDimensionDimtfillProperty */

struct OdDbDimensionDimtfillProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtfillProperty>::createObject();
    ((OdDbDimensionDimtfillProperty*)res.get())->init(__OD_T("Dimtfill"), &OdRxValueType::Desc<int>::value(), owner);
    ((OdDbDimensionDimtfillProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Sets fill color On or Off (DIMTFILL system variable)"));
    ((OdDbDimensionDimtfillProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextFill"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtfillProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtfill());
  return eOk;
}

OdResult OdDbDimensionDimtfillProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtfill(val);
  return eOk;
}

/* OdDbDimensionDimtfacProperty */

struct OdDbDimensionDimtfacProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtfacProperty>::createObject();
    ((OdDbDimensionDimtfacProperty*)res.get())->init(__OD_T("Dimtfac"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimtfacProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtfacProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Tolerance text height"));
    ((OdDbDimensionDimtfacProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies scale factor for text height of tolerance values relative to dimension text height as set by DIMTXT (DIMTFAC system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Tolerances"))
      OdRxCategory::createObject(L"Tolerances", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtfacProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Tolerances", 78));
    ((OdDbDimensionDimtfacProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ToleranceHeightScale"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDimensionDimtfacProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtfac());
  return eOk;
}

/* OdDbDimensionDimtihProperty */

struct OdDbDimensionDimtihProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtihProperty>::createObject();
    ((OdDbDimensionDimtihProperty*)res.get())->init(__OD_T("Dimtih"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtihProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text inside align"));
    ((OdDbDimensionDimtihProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Sets position of dimension text inside extension lines On or Off (DIMTIH system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtihProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 29));
    ((OdDbDimensionDimtihProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextInsideAlign"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtihProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimtih());
  return eOk;
}

OdResult OdDbDimensionDimtihProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtih(val);
  return eOk;
}

/* OdDbDimensionDimtmoveProperty */

struct OdDbDimensionDimtmoveProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtmoveProperty>::createObject();
    ((OdDbDimensionDimtmoveProperty*)res.get())->init(__OD_T("Dimtmove"), &OdRxValueType::Desc<OdInt16>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtmoveProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text movement"));
    ((OdDbDimensionDimtmoveProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies position of text when it's moved, either manually or automatically (DIMTMOVE system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Fit"))
      OdRxCategory::createObject(L"Fit", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtmoveProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Fit", 40));
    ((OdDbDimensionDimtmoveProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextMovement"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtmoveProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdInt16>(pObj->dimtmove());
  return eOk;
}

OdResult OdDbDimensionDimtmoveProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdInt16 val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtmove(val);
  return eOk;
}

/* OdDbDimensionDimtmProperty */

struct OdDbDimensionDimtmProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtmProperty>::createObject();
    ((OdDbDimensionDimtmProperty*)res.get())->init(__OD_T("Dimtm"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimtmProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtmProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Tolerance limit lower"));
    ((OdDbDimensionDimtmProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies minimum (or lower) tolerance limit for dimension text when DIMTOL or DIMLIM is on (DIMTM system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Tolerances"))
      OdRxCategory::createObject(L"Tolerances", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtmProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Tolerances", 70));
    ((OdDbDimensionDimtmProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ToleranceLowerLimit"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDimensionDimtmProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtm());
  return eOk;
}

/* OdDbDimensionDimblk1Property */

struct OdDbDimensionDimblk1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimblk1Property>::createObject();
    ((OdDbDimensionDimblk1Property*)res.get())->init(__OD_T("Dimblk1"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimensionDimblk1Property*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimblk1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimblk1());
  return eOk;
}

OdResult OdDbDimensionDimblk1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimblk1(val);
  return eOk;
}

/* OdDbDimensionDimblkProperty */

struct OdDbDimensionDimblkProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimblkProperty>::createObject();
    ((OdDbDimensionDimblkProperty*)res.get())->init(__OD_T("Dimblk"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimensionDimblkProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimblkProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimblk());
  return eOk;
}

OdResult OdDbDimensionDimblkProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimblk(val);
  return eOk;
}

/* OdDbDimensionDimpostProperty */

struct OdDbDimensionDimpostProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimpostProperty>::createObject();
    ((OdDbDimensionDimpostProperty*)res.get())->init(__OD_T("Dimpost"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimpostProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->dimpost());
  return eOk;
}

OdResult OdDbDimensionDimpostProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimpost(val);
  return eOk;
}

/* OdDbDimensionDimblk2Property */

struct OdDbDimensionDimblk2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimblk2Property>::createObject();
    ((OdDbDimensionDimblk2Property*)res.get())->init(__OD_T("Dimblk2"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDimensionDimblk2Property*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimblk2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimblk2());
  return eOk;
}

OdResult OdDbDimensionDimblk2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimblk2(val);
  return eOk;
}

/* OdDbDimensionDimaltmzsProperty */

struct OdDbDimensionDimaltmzsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimaltmzsProperty>::createObject();
    ((OdDbDimensionDimaltmzsProperty*)res.get())->init(__OD_T("Dimaltmzs"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimaltmzsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->dimaltmzs());
  return eOk;
}

OdResult OdDbDimensionDimaltmzsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimaltmzs(val);
  return eOk;
}

/* OdDbDimensionDimmzsProperty */

struct OdDbDimensionDimmzsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimmzsProperty>::createObject();
    ((OdDbDimensionDimmzsProperty*)res.get())->init(__OD_T("Dimmzs"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimmzsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->dimmzs());
  return eOk;
}

OdResult OdDbDimensionDimmzsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimmzs(val);
  return eOk;
}

/* OdDbDimensionDimtxtProperty */

struct OdDbDimensionDimtxtProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtxtProperty>::createObject();
    ((OdDbDimensionDimtxtProperty*)res.get())->init(__OD_T("Dimtxt"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimtxtProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtxtProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text height"));
    ((OdDbDimensionDimtxtProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies text height of the dimension (DIMTXT system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtxtProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 23));
    ((OdDbDimensionDimtxtProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextHeight"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtxtProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtxt());
  return eOk;
}

OdResult OdDbDimensionDimtxtProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtxt(val);
  return eOk;
}

/* OdDbDimensionDimtzinProperty */

struct OdDbDimensionDimtzinProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtzinProperty>::createObject();
    ((OdDbDimensionDimtzinProperty*)res.get())->init(__OD_T("Dimtzin"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtzinProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtzin());
  return eOk;
}

OdResult OdDbDimensionDimtzinProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtzin(val);
  return eOk;
}

/* OdDbDimensionDimapostProperty */

struct OdDbDimensionDimapostProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimapostProperty>::createObject();
    ((OdDbDimensionDimapostProperty*)res.get())->init(__OD_T("Dimapost"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimapostProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->dimapost());
  return eOk;
}

OdResult OdDbDimensionDimapostProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimapost(val);
  return eOk;
}

/* OdDbDimensionDimzinProperty */

struct OdDbDimensionDimzinProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimzinProperty>::createObject();
    ((OdDbDimensionDimzinProperty*)res.get())->init(__OD_T("Dimzin"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimzinProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimzin());
  return eOk;
}

OdResult OdDbDimensionDimzinProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimzin(val);
  return eOk;
}

/* OdDbDimensionDimuptProperty */

struct OdDbDimensionDimuptProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimuptProperty>::createObject();
    ((OdDbDimensionDimuptProperty*)res.get())->init(__OD_T("Dimupt"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimuptProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimupt());
  return eOk;
}

OdResult OdDbDimensionDimuptProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimupt(val);
  return eOk;
}

/* OdDbDimensionDimtdecProperty */

struct OdDbDimensionDimtdecProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtdecProperty>::createObject();
    ((OdDbDimensionDimtdecProperty*)res.get())->init(__OD_T("Dimtdec"), &OdRxValueType::Desc<OdInt16>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtdecProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Tolerance precision"));
    ((OdDbDimensionDimtdecProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies number of decimal places for tolerance values of a dimension (DIMTDEC system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Tolerances"))
      OdRxCategory::createObject(L"Tolerances", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtdecProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Tolerances", 73));
    ((OdDbDimensionDimtdecProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TolerancePrecision"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDimensionDimtdecProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdInt16>(pObj->dimtdec());
  return eOk;
}

/* OdDbDimensionDimscaleProperty */

struct OdDbDimensionDimscaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimscaleProperty>::createObject();
    ((OdDbDimensionDimscaleProperty*)res.get())->init(__OD_T("Dimscale"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbDimensionDimscaleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimscaleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim scale overall"));
    ((OdDbDimensionDimscaleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the overall scale factor applied to properties that specify sizes, distances, or offsets (DIMSCALE system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Fit"))
      OdRxCategory::createObject(L"Fit", OdRxCategory::rootCategory());
    ((OdDbDimensionDimscaleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Fit", 37));
    ((OdDbDimensionDimscaleProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ScaleFactor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimscaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimscale());
  return eOk;
}

OdResult OdDbDimensionDimscaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimscale(val);
  return eOk;
}

/* OdDbDimensionDimtadProperty */

struct OdDbDimensionDimtadProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimtadProperty>::createObject();
    ((OdDbDimensionDimtadProperty*)res.get())->init(__OD_T("Dimtad"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionDimtadProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text pos vert"));
    ((OdDbDimensionDimtadProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the vertical dimension text position relative to dimension line (DIMTAD system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionDimtadProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 27));
    ((OdDbDimensionDimtadProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"VerticalTextPosition"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimtadProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtad());
  return eOk;
}

OdResult OdDbDimensionDimtadProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtad(val);
  return eOk;
}

/* OdDbDimensionDimsahProperty */

struct OdDbDimensionDimsahProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionDimsahProperty>::createObject();
    ((OdDbDimensionDimsahProperty*)res.get())->init(__OD_T("Dimsah"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionDimsahProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimsah());
  return eOk;
}

OdResult OdDbDimensionDimsahProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimsah(val);
  return eOk;
}

/* OdDbDimensionPrefixProperty */

struct OdDbDimensionPrefixProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionPrefixProperty>::createObject();
    ((OdDbDimensionPrefixProperty*)res.get())->init(__OD_T("Prefix"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionPrefixProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim prefix"));
    ((OdDbDimensionPrefixProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text prefix for the dimension (DIMPOST system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Primary Units"))
      OdRxCategory::createObject(L"Primary Units", OdRxCategory::rootCategory());
    ((OdDbDimensionPrefixProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Primary Units", 42));
    ((OdDbDimensionPrefixProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextPrefix"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionPrefixProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString res = pObj->dimpost();
  OdString pref;
  int nDiv = res.find(OD_T("\\<\\>"));
  if (nDiv >= 0)
    pref = res.left(nDiv);
  value = pref;
  return eOk;
  //<<REPLACE_ALL
}

OdResult OdDbDimensionPrefixProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  OdString suffix = pObj->dimpost();
  int nDiv = suffix.find(OD_T("\\<\\>"));
  if (nDiv >= 0) suffix.deleteChars(0, nDiv + 4);
  else suffix.empty();
  if (!val.isEmpty() || !suffix.isEmpty())
    pObj->setDimpost(val + OD_T("\\<\\>") + suffix);
  else
    pObj->setDimpost(OdString::kEmpty);
  return eOk;
  //<<REPLACE_ALL
}

/* OdDbDimensionSuffixProperty */

struct OdDbDimensionSuffixProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionSuffixProperty>::createObject();
    ((OdDbDimensionSuffixProperty*)res.get())->init(__OD_T("Suffix"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionSuffixProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim suffix"));
    ((OdDbDimensionSuffixProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text suffix for the dimension (DIMPOST system variable)"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Primary Units"))
      OdRxCategory::createObject(L"Primary Units", OdRxCategory::rootCategory());
    ((OdDbDimensionSuffixProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Primary Units", 43));
    ((OdDbDimensionSuffixProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextSuffix"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionSuffixProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString res = pObj->dimpost();
  int nDiv = res.find(OD_T("\\<\\>"));
  if (nDiv >= 0) res.deleteChars(0, nDiv + 4);
  else res.empty();
  value = res;
  return eOk;
  //<<REPLACE_ALL
}

OdResult OdDbDimensionSuffixProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  OdString prefix = pObj->dimpost();
  int nDiv = prefix.find(OD_T("\\<\\>"));
  if (nDiv > 0) prefix = prefix.left(nDiv);
  else prefix.empty();
  if (val.getLength() || prefix.getLength())
    pObj->setDimpost(prefix + OD_T("\\<\\>") + val);
  else
    pObj->setDimpost(OdString::kEmpty);
  return eOk;
  //<<REPLACE_ALL
}

/* OdDbDimensionInspectionShapeProperty */

struct OdDbDimensionInspectionShapeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionInspectionShapeProperty>::createObject();
    ((OdDbDimensionInspectionShapeProperty*)res.get())->init(__OD_T("InspectionShape"), &OdRxValueType::Desc<int>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbDimensionInspectionShapeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 2));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionInspectionShapeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Inspection shape"));
    ((OdDbDimensionInspectionShapeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies inspection frame style"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionInspectionShapeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->inspectionFrame());
  return eOk;
}

OdResult OdDbDimensionInspectionShapeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setInspectionFrame(val);
  return eOk;
}

/* OdDbDimensionInspectionProperty */

struct OdDbDimensionInspectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionInspectionProperty>::createObject();
    ((OdDbDimensionInspectionProperty*)res.get())->init(__OD_T("Inspection"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionInspectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->inspection());
  return eOk;
}

OdResult OdDbDimensionInspectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setInspection(val);
  return eOk;
}

/* OdDbDimensionInspectionLabelProperty */

struct OdDbDimensionInspectionLabelProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionInspectionLabelProperty>::createObject();
    ((OdDbDimensionInspectionLabelProperty*)res.get())->init(__OD_T("InspectionLabel"), &OdRxValueType::Desc<OdString>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbDimensionInspectionLabelProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 3));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionInspectionLabelProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Inspection label"));
    ((OdDbDimensionInspectionLabelProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies inspection label"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDimensionInspectionLabelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->inspectionLabel());
  return eOk;
}

/* OdDbDimensionInspectionRateProperty */

struct OdDbDimensionInspectionRateProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionInspectionRateProperty>::createObject();
    ((OdDbDimensionInspectionRateProperty*)res.get())->init(__OD_T("InspectionRate"), &OdRxValueType::Desc<OdString>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbDimensionInspectionRateProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 4));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionInspectionRateProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Inspection rate"));
    ((OdDbDimensionInspectionRateProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies inspection percentage rate"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDimensionInspectionRateProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->inspectionRate());
  return eOk;
}

/* OdDbDimensionTextViewDirectionProperty */

struct OdDbDimensionTextViewDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDimensionTextViewDirectionProperty>::createObject();
    ((OdDbDimensionTextViewDirectionProperty*)res.get())->init(__OD_T("TextViewDirection"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbDimensionTextViewDirectionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text view direction"));
    ((OdDbDimensionTextViewDirectionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text direction"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbDimensionTextViewDirectionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 32));
    ((OdDbDimensionTextViewDirectionProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DimTxtDirection"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDimensionTextViewDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->dimtxtdirection());
  return eOk;
}

OdResult OdDbDimensionTextViewDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDimensionPtr pObj = OdDbDimension::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setDimtxtdirection(val);
  return eOk;
}

void createOdDbDimensionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbDimensionConstraintObjectProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionHorizontalRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimBlockIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionTextLineSpacingStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionTextLineSpacingFactorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDynamicDimensionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimBlockPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionMeasurementProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionTextAttachmentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionUsingDefaultTextPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionTextPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionElevationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimensionStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionTextRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimensionTextProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimgapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimldrblkProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimexoProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimlweProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimlimProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimdsepProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimalttzProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimarcsymProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimaltzProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimclrtProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimclreProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimdliProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimaszProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimcenProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimazinProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtolProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtohProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtoljProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtvpProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtszProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtpProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtfillclrProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtfillProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtfacProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtihProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtmoveProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtmProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimblk1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimblkProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimpostProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimblk2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimaltmzsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimmzsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtxtProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtzinProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimapostProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimzinProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimuptProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtdecProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimscaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimtadProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionDimsahProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionPrefixProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionSuffixProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionInspectionShapeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionInspectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionInspectionLabelProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionInspectionRateProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDimensionTextViewDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
}
