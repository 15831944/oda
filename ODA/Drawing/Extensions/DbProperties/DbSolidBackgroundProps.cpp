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
#include "DbValueTypes.h"
#include "DbBackground.h"

/* OdDbSolidBackgroundColorProperty */

struct OdDbSolidBackgroundColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSolidBackgroundColorProperty>::createObject();
    ((OdDbSolidBackgroundColorProperty*)res.get())->init(__OD_T("Color"), &OdRxValueType::Desc<OdCmEntityColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSolidBackgroundColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSolidBackgroundPtr pObj = OdDbSolidBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdCmEntityColor>(pObj->colorSolid());
  return eOk;
}

OdResult OdDbSolidBackgroundColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSolidBackgroundPtr pObj = OdDbSolidBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmEntityColor val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setColorSolid(val);
  return eOk;
}

void createOdDbSolidBackgroundProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbSolidBackgroundColorProperty::createObject(b.owner()));
  b.add(properties.last());
}
