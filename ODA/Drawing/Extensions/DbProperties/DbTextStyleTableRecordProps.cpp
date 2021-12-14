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
#include "DbTextStyleTableRecord.h"
#include "DbValueTypes.h"

/* OdDbTextStyleTableRecordXScaleProperty */

struct OdDbTextStyleTableRecordXScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextStyleTableRecordXScaleProperty>::createObject();
    ((OdDbTextStyleTableRecordXScaleProperty*)res.get())->init(__OD_T("XScale"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTextStyleTableRecordXScaleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextStyleTableRecordXScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->xScale());
  return eOk;
}

OdResult OdDbTextStyleTableRecordXScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setXScale(val);
  return eOk;
}

/* OdDbTextStyleTableRecordObliquingAngleProperty */

struct OdDbTextStyleTableRecordObliquingAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextStyleTableRecordObliquingAngleProperty>::createObject();
    ((OdDbTextStyleTableRecordObliquingAngleProperty*)res.get())->init(__OD_T("ObliquingAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTextStyleTableRecordObliquingAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextStyleTableRecordObliquingAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->obliquingAngle());
  return eOk;
}

OdResult OdDbTextStyleTableRecordObliquingAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setObliquingAngle(val);
  return eOk;
}

/* OdDbTextStyleTableRecordIsVerticalProperty */

struct OdDbTextStyleTableRecordIsVerticalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextStyleTableRecordIsVerticalProperty>::createObject();
    ((OdDbTextStyleTableRecordIsVerticalProperty*)res.get())->init(__OD_T("IsVertical"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextStyleTableRecordIsVerticalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isVertical());
  return eOk;
}

OdResult OdDbTextStyleTableRecordIsVerticalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsVertical(val);
  return eOk;
}

/* OdDbTextStyleTableRecordTextSizeProperty */

struct OdDbTextStyleTableRecordTextSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextStyleTableRecordTextSizeProperty>::createObject();
    ((OdDbTextStyleTableRecordTextSizeProperty*)res.get())->init(__OD_T("TextSize"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTextStyleTableRecordTextSizeProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextStyleTableRecordTextSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->textSize());
  return eOk;
}

OdResult OdDbTextStyleTableRecordTextSizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextSize(val);
  return eOk;
}

/* OdDbTextStyleTableRecordIsShapeFileProperty */

struct OdDbTextStyleTableRecordIsShapeFileProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextStyleTableRecordIsShapeFileProperty>::createObject();
    ((OdDbTextStyleTableRecordIsShapeFileProperty*)res.get())->init(__OD_T("IsShapeFile"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextStyleTableRecordIsShapeFileProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isShapeFile());
  return eOk;
}

OdResult OdDbTextStyleTableRecordIsShapeFileProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsShapeFile(val);
  return eOk;
}

/* OdDbTextStyleTableRecordBigFontFileNameProperty */

struct OdDbTextStyleTableRecordBigFontFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextStyleTableRecordBigFontFileNameProperty>::createObject();
    ((OdDbTextStyleTableRecordBigFontFileNameProperty*)res.get())->init(__OD_T("BigFontFileName"), &OdRxValueType::Desc<OdString>::value(), owner);
    ((OdDbTextStyleTableRecordBigFontFileNameProperty*)res.get())->attributes().add(OdRxFilePathAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextStyleTableRecordBigFontFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->bigFontFileName());
  return eOk;
}

OdResult OdDbTextStyleTableRecordBigFontFileNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBigFontFileName(val);
  return eOk;
}

/* OdDbTextStyleTableRecordPriorSizeProperty */

struct OdDbTextStyleTableRecordPriorSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextStyleTableRecordPriorSizeProperty>::createObject();
    ((OdDbTextStyleTableRecordPriorSizeProperty*)res.get())->init(__OD_T("PriorSize"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTextStyleTableRecordPriorSizeProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextStyleTableRecordPriorSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->priorSize());
  return eOk;
}

OdResult OdDbTextStyleTableRecordPriorSizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPriorSize(val);
  return eOk;
}

/* OdDbTextStyleTableRecordFileNameProperty */

struct OdDbTextStyleTableRecordFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTextStyleTableRecordFileNameProperty>::createObject();
    ((OdDbTextStyleTableRecordFileNameProperty*)res.get())->init(__OD_T("FileName"), &OdRxValueType::Desc<OdString>::value(), owner);
    ((OdDbTextStyleTableRecordFileNameProperty*)res.get())->attributes().add(OdRxFilePathAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTextStyleTableRecordFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->fileName());
  return eOk;
}

OdResult OdDbTextStyleTableRecordFileNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTextStyleTableRecordPtr pObj = OdDbTextStyleTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFileName(val);
  return eOk;
}

void createOdDbTextStyleTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbTextStyleTableRecordXScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextStyleTableRecordObliquingAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextStyleTableRecordIsVerticalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextStyleTableRecordTextSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextStyleTableRecordIsShapeFileProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextStyleTableRecordBigFontFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextStyleTableRecordPriorSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTextStyleTableRecordFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
}
