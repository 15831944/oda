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
#include "DbRenderSettings.h"

/* OdDbRenderEnvironmentFogEnabledProperty */

struct OdDbRenderEnvironmentFogEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderEnvironmentFogEnabledProperty>::createObject();
    ((OdDbRenderEnvironmentFogEnabledProperty*)res.get())->init(__OD_T("FogEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderEnvironmentFogEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->fogEnabled());
  return eOk;
}

OdResult OdDbRenderEnvironmentFogEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFogEnabled(val);
  return eOk;
}

/* OdDbRenderEnvironmentFogColorProperty */

struct OdDbRenderEnvironmentFogColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderEnvironmentFogColorProperty>::createObject();
    ((OdDbRenderEnvironmentFogColorProperty*)res.get())->init(__OD_T("FogColor"), &OdRxValueType::Desc<OdCmEntityColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderEnvironmentFogColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmEntityColor>(pObj->fogColor());
  return eOk;
}

OdResult OdDbRenderEnvironmentFogColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmEntityColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFogColor(val);
  return eOk;
}

/* OdDbRenderEnvironmentFogBackgroundEnabledProperty */

struct OdDbRenderEnvironmentFogBackgroundEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderEnvironmentFogBackgroundEnabledProperty>::createObject();
    ((OdDbRenderEnvironmentFogBackgroundEnabledProperty*)res.get())->init(__OD_T("FogBackgroundEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderEnvironmentFogBackgroundEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->fogBackgroundEnabled());
  return eOk;
}

OdResult OdDbRenderEnvironmentFogBackgroundEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFogBackgroundEnabled(val);
  return eOk;
}

/* OdDbRenderEnvironmentEnvironmentImageFileNameProperty */

struct OdDbRenderEnvironmentEnvironmentImageFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderEnvironmentEnvironmentImageFileNameProperty>::createObject();
    ((OdDbRenderEnvironmentEnvironmentImageFileNameProperty*)res.get())->init(__OD_T("EnvironmentImageFileName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderEnvironmentEnvironmentImageFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->environmentImageFileName());
  return eOk;
}

OdResult OdDbRenderEnvironmentEnvironmentImageFileNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setEnvironmentImageFileName(val);
  return rc;
}

/* OdDbRenderEnvironmentEnvironmentImageEnabledProperty */

struct OdDbRenderEnvironmentEnvironmentImageEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderEnvironmentEnvironmentImageEnabledProperty>::createObject();
    ((OdDbRenderEnvironmentEnvironmentImageEnabledProperty*)res.get())->init(__OD_T("EnvironmentImageEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderEnvironmentEnvironmentImageEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->environmentImageEnabled());
  return eOk;
}

OdResult OdDbRenderEnvironmentEnvironmentImageEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderEnvironmentPtr pObj = OdDbRenderEnvironment::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnvironmentImageEnabled(val);
  return eOk;
}

void createOdDbRenderEnvironmentProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRenderEnvironmentFogEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderEnvironmentFogColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderEnvironmentFogBackgroundEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderEnvironmentEnvironmentImageFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderEnvironmentEnvironmentImageEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
}
