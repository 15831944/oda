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
#include "DbLinetypeTableRecord.h"
#include "DbValueTypes.h"

/* OdDbLinetypeTableRecordPatternLengthProperty */

struct OdDbLinetypeTableRecordPatternLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLinetypeTableRecordPatternLengthProperty>::createObject();
    ((OdDbLinetypeTableRecordPatternLengthProperty*)res.get())->init(__OD_T("PatternLength"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLinetypeTableRecordPatternLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLinetypeTableRecordPtr pObj = OdDbLinetypeTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->patternLength());
  return eOk;
}

OdResult OdDbLinetypeTableRecordPatternLengthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLinetypeTableRecordPtr pObj = OdDbLinetypeTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPatternLength(val);
  return eOk;
}

/* OdDbLinetypeTableRecordCommentsProperty */

struct OdDbLinetypeTableRecordCommentsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLinetypeTableRecordCommentsProperty>::createObject();
    ((OdDbLinetypeTableRecordCommentsProperty*)res.get())->init(__OD_T("Comments"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLinetypeTableRecordCommentsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLinetypeTableRecordPtr pObj = OdDbLinetypeTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->comments());
  return eOk;
}

OdResult OdDbLinetypeTableRecordCommentsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLinetypeTableRecordPtr pObj = OdDbLinetypeTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setComments(val);
  return eOk;
}

/* OdDbLinetypeTableRecordIsScaledToFitProperty */

struct OdDbLinetypeTableRecordIsScaledToFitProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLinetypeTableRecordIsScaledToFitProperty>::createObject();
    ((OdDbLinetypeTableRecordIsScaledToFitProperty*)res.get())->init(__OD_T("IsScaledToFit"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLinetypeTableRecordIsScaledToFitProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLinetypeTableRecordPtr pObj = OdDbLinetypeTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isScaledToFit());
  return eOk;
}

OdResult OdDbLinetypeTableRecordIsScaledToFitProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLinetypeTableRecordPtr pObj = OdDbLinetypeTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsScaledToFit(val);
  return eOk;
}

/* OdDbLinetypeTableRecordNumDashesProperty */

struct OdDbLinetypeTableRecordNumDashesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLinetypeTableRecordNumDashesProperty>::createObject();
    ((OdDbLinetypeTableRecordNumDashesProperty*)res.get())->init(__OD_T("NumDashes"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLinetypeTableRecordNumDashesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLinetypeTableRecordPtr pObj = OdDbLinetypeTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numDashes());
  return eOk;
}

OdResult OdDbLinetypeTableRecordNumDashesProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLinetypeTableRecordPtr pObj = OdDbLinetypeTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNumDashes(val);
  return eOk;
}

void createOdDbLinetypeTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbLinetypeTableRecordPatternLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLinetypeTableRecordCommentsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLinetypeTableRecordIsScaledToFitProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLinetypeTableRecordNumDashesProperty::createObject(b.owner()));
  b.add(properties.last());
}
