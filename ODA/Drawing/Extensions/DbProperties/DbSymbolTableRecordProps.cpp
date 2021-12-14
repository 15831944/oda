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
#include "DbSymbolTableRecord.h"
#include "DbValueTypes.h"

/* OdDbSymbolTableRecordNameProperty */

struct OdDbSymbolTableRecordNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSymbolTableRecordNameProperty>::createObject();
    ((OdDbSymbolTableRecordNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbSymbolTableRecordNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 1));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSymbolTableRecordNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Name"));
    ((OdDbSymbolTableRecordNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the name of the layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSymbolTableRecordNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSymbolTableRecordPtr pObj = OdDbSymbolTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->getName();
  return eOk;
}

OdResult OdDbSymbolTableRecordNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSymbolTableRecordPtr pObj = OdDbSymbolTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setName(val);
  return eOk;
}

/* OdDbSymbolTableRecordIsDependentProperty */

struct OdDbSymbolTableRecordIsDependentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSymbolTableRecordIsDependentProperty>::createObject();
    ((OdDbSymbolTableRecordIsDependentProperty*)res.get())->init(__OD_T("IsDependent"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSymbolTableRecordIsDependentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSymbolTableRecordPtr pObj = OdDbSymbolTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isDependent());
  return eOk;
}

/* OdDbSymbolTableRecordIsResolvedProperty */

struct OdDbSymbolTableRecordIsResolvedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSymbolTableRecordIsResolvedProperty>::createObject();
    ((OdDbSymbolTableRecordIsResolvedProperty*)res.get())->init(__OD_T("IsResolved"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSymbolTableRecordIsResolvedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSymbolTableRecordPtr pObj = OdDbSymbolTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isResolved());
  return eOk;
}

void createOdDbSymbolTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbSymbolTableRecordNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSymbolTableRecordIsDependentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSymbolTableRecordIsResolvedProperty::createObject(b.owner()));
  b.add(properties.last());
}
