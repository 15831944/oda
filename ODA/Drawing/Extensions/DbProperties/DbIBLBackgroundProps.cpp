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

/* OdDbIBLBackgroundEnableProperty */

struct OdDbIBLBackgroundEnableProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbIBLBackgroundEnableProperty>::createObject();
    ((OdDbIBLBackgroundEnableProperty*)res.get())->init(__OD_T("Enable"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbIBLBackgroundEnableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enable());
  return eOk;
}

OdResult OdDbIBLBackgroundEnableProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnable(val);
  return eOk;
}

/* OdDbIBLBackgroundIBLImageNameProperty */

struct OdDbIBLBackgroundIBLImageNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbIBLBackgroundIBLImageNameProperty>::createObject();
    ((OdDbIBLBackgroundIBLImageNameProperty*)res.get())->init(__OD_T("IBLImageName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbIBLBackgroundIBLImageNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->IBLImageName());
  return eOk;
}

OdResult OdDbIBLBackgroundIBLImageNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIBLImageName(val);
  return eOk;
}

/* OdDbIBLBackgroundRotationProperty */

struct OdDbIBLBackgroundRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbIBLBackgroundRotationProperty>::createObject();
    ((OdDbIBLBackgroundRotationProperty*)res.get())->init(__OD_T("Rotation"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbIBLBackgroundRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->rotation());
  return eOk;
}

OdResult OdDbIBLBackgroundRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRotation(val);
  return eOk;
}

/* OdDbIBLBackgroundDisplayImageProperty */

struct OdDbIBLBackgroundDisplayImageProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbIBLBackgroundDisplayImageProperty>::createObject();
    ((OdDbIBLBackgroundDisplayImageProperty*)res.get())->init(__OD_T("DisplayImage"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbIBLBackgroundDisplayImageProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->displayImage());
  return eOk;
}

OdResult OdDbIBLBackgroundDisplayImageProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDisplayImage(val);
  return eOk;
}

/* OdDbIBLBackgroundSecondaryBackgroundProperty */

struct OdDbIBLBackgroundSecondaryBackgroundProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbIBLBackgroundSecondaryBackgroundProperty>::createObject();
    ((OdDbIBLBackgroundSecondaryBackgroundProperty*)res.get())->init(__OD_T("SecondaryBackground"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbIBLBackgroundSecondaryBackgroundProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbIBLBackgroundSecondaryBackgroundProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->secondaryBackground());
  return eOk;
}

OdResult OdDbIBLBackgroundSecondaryBackgroundProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbIBLBackgroundPtr pObj = OdDbIBLBackground::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSecondaryBackground(val);
  return eOk;
}

void createOdDbIBLBackgroundProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbIBLBackgroundEnableProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbIBLBackgroundIBLImageNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbIBLBackgroundRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbIBLBackgroundDisplayImageProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbIBLBackgroundSecondaryBackgroundProperty::createObject(b.owner()));
  b.add(properties.last());
}
