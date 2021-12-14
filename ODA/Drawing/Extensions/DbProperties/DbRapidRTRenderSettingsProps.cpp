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

/* OdDbRapidRTRenderSettingsRenderTargetProperty */

struct OdDbRapidRTRenderSettingsRenderTargetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRapidRTRenderSettingsRenderTargetProperty>::createObject();
    ((OdDbRapidRTRenderSettingsRenderTargetProperty*)res.get())->init(__OD_T("RenderTarget"), &OdRxValueType::Desc<OdDbRapidRTRenderSettings::RenderTarget>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRapidRTRenderSettingsRenderTargetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbRapidRTRenderSettings::RenderTarget>(pObj->renderTarget());
  return eOk;
}

OdResult OdDbRapidRTRenderSettingsRenderTargetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbRapidRTRenderSettings::RenderTarget val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setRenderTarget(val);
  return rc;
}

/* OdDbRapidRTRenderSettingsRenderLevelProperty */

struct OdDbRapidRTRenderSettingsRenderLevelProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRapidRTRenderSettingsRenderLevelProperty>::createObject();
    ((OdDbRapidRTRenderSettingsRenderLevelProperty*)res.get())->init(__OD_T("RenderLevel"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRapidRTRenderSettingsRenderLevelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->renderLevel());
  return eOk;
}

OdResult OdDbRapidRTRenderSettingsRenderLevelProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setRenderLevel(val);
  return rc;
}

/* OdDbRapidRTRenderSettingsRenderTimeProperty */

struct OdDbRapidRTRenderSettingsRenderTimeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRapidRTRenderSettingsRenderTimeProperty>::createObject();
    ((OdDbRapidRTRenderSettingsRenderTimeProperty*)res.get())->init(__OD_T("RenderTime"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRapidRTRenderSettingsRenderTimeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->renderTime());
  return eOk;
}

OdResult OdDbRapidRTRenderSettingsRenderTimeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setRenderTime(val);
  return rc;
}

/* OdDbRapidRTRenderSettingsLightingModelProperty */

struct OdDbRapidRTRenderSettingsLightingModelProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRapidRTRenderSettingsLightingModelProperty>::createObject();
    ((OdDbRapidRTRenderSettingsLightingModelProperty*)res.get())->init(__OD_T("LightingModel"), &OdRxValueType::Desc<OdGiLightingMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRapidRTRenderSettingsLightingModelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiLightingMode>(pObj->lightingModel());
  return eOk;
}

OdResult OdDbRapidRTRenderSettingsLightingModelProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiLightingMode val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setLightingModel(val);
  return rc;
}

/* OdDbRapidRTRenderSettingsFilterTypeProperty */

struct OdDbRapidRTRenderSettingsFilterTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRapidRTRenderSettingsFilterTypeProperty>::createObject();
    ((OdDbRapidRTRenderSettingsFilterTypeProperty*)res.get())->init(__OD_T("FilterType"), &OdRxValueType::Desc<OdGiFilterType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRapidRTRenderSettingsFilterTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiFilterType>(pObj->filterType());
  return eOk;
}

OdResult OdDbRapidRTRenderSettingsFilterTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiFilterType val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setFilterType(val);
  return rc;
}

/* OdDbRapidRTRenderSettingsFilterWidthProperty */

struct OdDbRapidRTRenderSettingsFilterWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRapidRTRenderSettingsFilterWidthProperty>::createObject();
    ((OdDbRapidRTRenderSettingsFilterWidthProperty*)res.get())->init(__OD_T("FilterWidth"), &OdRxValueType::Desc<float>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRapidRTRenderSettingsFilterWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<float>(pObj->filterWidth());
  return eOk;
}

OdResult OdDbRapidRTRenderSettingsFilterWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  float val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setFilterWidth(val);
  return rc;
}

/* OdDbRapidRTRenderSettingsFilterHeightProperty */

struct OdDbRapidRTRenderSettingsFilterHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRapidRTRenderSettingsFilterHeightProperty>::createObject();
    ((OdDbRapidRTRenderSettingsFilterHeightProperty*)res.get())->init(__OD_T("FilterHeight"), &OdRxValueType::Desc<float>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRapidRTRenderSettingsFilterHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<float>(pObj->filterHeight());
  return eOk;
}

OdResult OdDbRapidRTRenderSettingsFilterHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRapidRTRenderSettingsPtr pObj = OdDbRapidRTRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  float val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setFilterHeight(val);
  return rc;
}

void createOdDbRapidRTRenderSettingsProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRapidRTRenderSettingsRenderTargetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRapidRTRenderSettingsRenderLevelProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRapidRTRenderSettingsRenderTimeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRapidRTRenderSettingsLightingModelProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRapidRTRenderSettingsFilterTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRapidRTRenderSettingsFilterWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRapidRTRenderSettingsFilterHeightProperty::createObject(b.owner()));
  b.add(properties.last());
}
