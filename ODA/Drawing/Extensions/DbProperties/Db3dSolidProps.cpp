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
#include "Db3dSolid.h"
#include "DbValueTypes.h"

/* OdDb3dSolidAreaProperty */

struct OdDb3dSolidAreaProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dSolidAreaProperty>::createObject();
    ((OdDb3dSolidAreaProperty*)res.get())->init(__OD_T("Area"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDb3dSolidAreaProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(4)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDb3dSolidAreaProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3dSolidPtr pObj = OdDb3dSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dVal = 0.0; OdResult rc = pObj->getArea(dVal); value = dVal;
  return rc;
}

/* OdDb3dSolidIsNullProperty */

struct OdDb3dSolidIsNullProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dSolidIsNullProperty>::createObject();
    ((OdDb3dSolidIsNullProperty*)res.get())->init(__OD_T("IsNull"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDb3dSolidIsNullProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3dSolidPtr pObj = OdDb3dSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isNull());
  return eOk;
}

/* OdDb3dSolidNumChangesProperty */

struct OdDb3dSolidNumChangesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dSolidNumChangesProperty>::createObject();
    ((OdDb3dSolidNumChangesProperty*)res.get())->init(__OD_T("NumChanges"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDb3dSolidNumChangesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3dSolidPtr pObj = OdDb3dSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numChanges());
  return eOk;
}

/* OdDb3dSolidShowHistoryProperty */

struct OdDb3dSolidShowHistoryProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dSolidShowHistoryProperty>::createObject();
    ((OdDb3dSolidShowHistoryProperty*)res.get())->init(__OD_T("ShowHistory"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb3dSolidShowHistoryProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Show History"));
    ((OdDb3dSolidShowHistoryProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether to show history of the solid"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Solid History"))
      OdRxCategory::createObject(L"Solid History", OdRxCategory::rootCategory());
    ((OdDb3dSolidShowHistoryProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Solid History", 85));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb3dSolidShowHistoryProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3dSolidPtr pObj = OdDb3dSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->showHistory());
  return eOk;
}

OdResult OdDb3dSolidShowHistoryProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3dSolidPtr pObj = OdDb3dSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setShowHistory(val);
  return rc;
}

/* OdDb3dSolidRecordHistoryProperty */

struct OdDb3dSolidRecordHistoryProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDb3dSolidRecordHistoryProperty>::createObject();
    ((OdDb3dSolidRecordHistoryProperty*)res.get())->init(__OD_T("RecordHistory"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDb3dSolidRecordHistoryProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Record History"));
    ((OdDb3dSolidRecordHistoryProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether history is saved"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Solid History"))
      OdRxCategory::createObject(L"Solid History", OdRxCategory::rootCategory());
    ((OdDb3dSolidRecordHistoryProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Solid History", 80));
    ((OdDb3dSolidRecordHistoryProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"History"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDb3dSolidRecordHistoryProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDb3dSolidPtr pObj = OdDb3dSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->recordHistory());
  return eOk;
}

OdResult OdDb3dSolidRecordHistoryProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDb3dSolidPtr pObj = OdDb3dSolid::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setRecordHistory(val);
  return rc;
}

void createOdDb3dSolidProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDb3dSolidAreaProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3dSolidIsNullProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3dSolidNumChangesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3dSolidShowHistoryProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDb3dSolidRecordHistoryProperty::createObject(b.owner()));
  b.add(properties.last());
}
