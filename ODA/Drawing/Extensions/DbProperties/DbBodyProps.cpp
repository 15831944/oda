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
#include "DbBody.h"
#include "DbValueTypes.h"

/* OdDbBodyIsNullProperty */

struct OdDbBodyIsNullProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBodyIsNullProperty>::createObject();
    ((OdDbBodyIsNullProperty*)res.get())->init(__OD_T("IsNull"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBodyIsNullProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBodyPtr pObj = OdDbBody::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isNull());
  return eOk;
}

/* OdDbBodyNumChangesProperty */

struct OdDbBodyNumChangesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBodyNumChangesProperty>::createObject();
    ((OdDbBodyNumChangesProperty*)res.get())->init(__OD_T("NumChanges"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBodyNumChangesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBodyPtr pObj = OdDbBody::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numChanges());
  return eOk;
}

void createOdDbBodyProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbBodyIsNullProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBodyNumChangesProperty::createObject(b.owner()));
  b.add(properties.last());
}
