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
#include "DbUnderlayDefinition.h"
#include "DbValueTypes.h"

/* OdDbUnderlayDefinitionActiveFileNameProperty */

struct OdDbUnderlayDefinitionActiveFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayDefinitionActiveFileNameProperty>::createObject();
    ((OdDbUnderlayDefinitionActiveFileNameProperty*)res.get())->init(__OD_T("ActiveFileName"), &OdRxValueType::Desc<OdString>::value(), owner);
    ((OdDbUnderlayDefinitionActiveFileNameProperty*)res.get())->attributes().add(OdRxFilePathAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbUnderlayDefinitionActiveFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayDefinitionPtr pObj = OdDbUnderlayDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->getActiveFileName());
  return eOk;
}

/* OdDbUnderlayDefinitionSourceFileNameProperty */

struct OdDbUnderlayDefinitionSourceFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayDefinitionSourceFileNameProperty>::createObject();
    ((OdDbUnderlayDefinitionSourceFileNameProperty*)res.get())->init(__OD_T("SourceFileName"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayDefinitionSourceFileNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Saved Path"));
    ((OdDbUnderlayDefinitionSourceFileNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the path to the underlay file"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbUnderlayDefinitionSourceFileNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 3));
    ((OdDbUnderlayDefinitionSourceFileNameProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"File"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbUnderlayDefinitionSourceFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayDefinitionPtr pObj = OdDbUnderlayDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->getSourceFileName());
  return eOk;
}

/* OdDbUnderlayDefinitionItemNameProperty */

struct OdDbUnderlayDefinitionItemNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayDefinitionItemNameProperty>::createObject();
    ((OdDbUnderlayDefinitionItemNameProperty*)res.get())->init(__OD_T("ItemName"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayDefinitionItemNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Page number"));
    ((OdDbUnderlayDefinitionItemNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the item of the underlay file"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbUnderlayDefinitionItemNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 2));
    ((OdDbUnderlayDefinitionItemNameProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ItemName"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbUnderlayDefinitionItemNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayDefinitionPtr pObj = OdDbUnderlayDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->getItemName());
  return eOk;
}

/* OdDbUnderlayDefinitionLoadedProperty */

struct OdDbUnderlayDefinitionLoadedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayDefinitionLoadedProperty>::createObject();
    ((OdDbUnderlayDefinitionLoadedProperty*)res.get())->init(__OD_T("Loaded"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbUnderlayDefinitionLoadedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayDefinitionPtr pObj = OdDbUnderlayDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isLoaded());
  return eOk;
}

/* OdDbUnderlayDefinitionNameProperty */

struct OdDbUnderlayDefinitionNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbUnderlayDefinitionNameProperty>::createObject();
    ((OdDbUnderlayDefinitionNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbUnderlayDefinitionNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Name"));
    ((OdDbUnderlayDefinitionNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the name of the underlay file"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbUnderlayDefinitionNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 1));
    ((OdDbUnderlayDefinitionNameProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"UnderlayName"));
    ((OdDbUnderlayDefinitionNameProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbUnderlayDefinitionNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbUnderlayDefinitionPtr pObj = OdDbUnderlayDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getItemName());
  return eOk;
}

OdResult OdDbUnderlayDefinitionNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbUnderlayDefinitionPtr pObj = OdDbUnderlayDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setItemName(val);
  return eOk;
}

void createOdDbUnderlayDefinitionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbUnderlayDefinitionActiveFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayDefinitionSourceFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayDefinitionItemNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayDefinitionLoadedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbUnderlayDefinitionNameProperty::createObject(b.owner()));
  b.add(properties.last());
}
