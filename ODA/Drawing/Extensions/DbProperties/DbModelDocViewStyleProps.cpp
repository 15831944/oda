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
#include "DbModelDocViewStyle.h"
#include "DbValueTypes.h"

/* OdDbModelDocViewStyleNameProperty */

struct OdDbModelDocViewStyleNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbModelDocViewStyleNameProperty>::createObject();
    ((OdDbModelDocViewStyleNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbModelDocViewStyleNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbModelDocViewStylePtr pObj = OdDbModelDocViewStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->getName());
  return eOk;
}

OdResult OdDbModelDocViewStyleNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbModelDocViewStylePtr pObj = OdDbModelDocViewStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setName(val);
  return eOk;
}

/* OdDbModelDocViewStyleIsRenamableProperty */

struct OdDbModelDocViewStyleIsRenamableProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbModelDocViewStyleIsRenamableProperty>::createObject();
    ((OdDbModelDocViewStyleIsRenamableProperty*)res.get())->init(__OD_T("IsRenamable"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbModelDocViewStyleIsRenamableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbModelDocViewStylePtr pObj = OdDbModelDocViewStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->canRename());
  return eOk;
}

void createOdDbModelDocViewStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbModelDocViewStyleNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbModelDocViewStyleIsRenamableProperty::createObject(b.owner()));
  b.add(properties.last());
}
