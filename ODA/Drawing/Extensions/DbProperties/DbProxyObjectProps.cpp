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
#include "DbProxyObject.h"
#include "DbValueTypes.h"

/* OdDbProxyObjectOriginalClassNameProperty */

struct OdDbProxyObjectOriginalClassNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbProxyObjectOriginalClassNameProperty>::createObject();
    ((OdDbProxyObjectOriginalClassNameProperty*)res.get())->init(__OD_T("OriginalClassName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbProxyObjectOriginalClassNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbProxyObjectPtr pObj = OdDbProxyObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->originalClassName());
  return eOk;
}

/* OdDbProxyObjectProxyFlagsProperty */

struct OdDbProxyObjectProxyFlagsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbProxyObjectProxyFlagsProperty>::createObject();
    ((OdDbProxyObjectProxyFlagsProperty*)res.get())->init(__OD_T("ProxyFlags"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbProxyObjectProxyFlagsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbProxyObjectPtr pObj = OdDbProxyObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->proxyFlags());
  return eOk;
}

/* OdDbProxyObjectApplicationDescriptionProperty */

struct OdDbProxyObjectApplicationDescriptionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbProxyObjectApplicationDescriptionProperty>::createObject();
    ((OdDbProxyObjectApplicationDescriptionProperty*)res.get())->init(__OD_T("ApplicationDescription"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbProxyObjectApplicationDescriptionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbProxyObjectPtr pObj = OdDbProxyObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->applicationDescription());
  return eOk;
}

/* OdDbProxyObjectOriginalDxfNameProperty */

struct OdDbProxyObjectOriginalDxfNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbProxyObjectOriginalDxfNameProperty>::createObject();
    ((OdDbProxyObjectOriginalDxfNameProperty*)res.get())->init(__OD_T("OriginalDxfName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbProxyObjectOriginalDxfNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbProxyObjectPtr pObj = OdDbProxyObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->originalDxfName());
  return eOk;
}

void createOdDbProxyObjectProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbProxyObjectOriginalClassNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbProxyObjectProxyFlagsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbProxyObjectApplicationDescriptionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbProxyObjectOriginalDxfNameProperty::createObject(b.owner()));
  b.add(properties.last());
}
