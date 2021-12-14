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

/* OdDbMentalRayRenderSettingsShadowModeProperty */

struct OdDbMentalRayRenderSettingsShadowModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsShadowModeProperty>::createObject();
    ((OdDbMentalRayRenderSettingsShadowModeProperty*)res.get())->init(__OD_T("ShadowMode"), &OdRxValueType::Desc<OdGiMrShadowMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsShadowModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMrShadowMode>(pObj->shadowMode());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsShadowModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMrShadowMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShadowMode(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsShadowMapsEnabledProperty */

struct OdDbMentalRayRenderSettingsShadowMapsEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsShadowMapsEnabledProperty>::createObject();
    ((OdDbMentalRayRenderSettingsShadowMapsEnabledProperty*)res.get())->init(__OD_T("ShadowMapsEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsShadowMapsEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->shadowMapsEnabled());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsShadowMapsEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShadowMapsEnabled(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsRayTracingEnabledProperty */

struct OdDbMentalRayRenderSettingsRayTracingEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsRayTracingEnabledProperty>::createObject();
    ((OdDbMentalRayRenderSettingsRayTracingEnabledProperty*)res.get())->init(__OD_T("RayTracingEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsRayTracingEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->rayTracingEnabled());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsRayTracingEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRayTracingEnabled(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsGlobalIlluminationEnabledProperty */

struct OdDbMentalRayRenderSettingsGlobalIlluminationEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsGlobalIlluminationEnabledProperty>::createObject();
    ((OdDbMentalRayRenderSettingsGlobalIlluminationEnabledProperty*)res.get())->init(__OD_T("GlobalIlluminationEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsGlobalIlluminationEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->globalIlluminationEnabled());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsGlobalIlluminationEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGlobalIlluminationEnabled(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsGiSampleCountProperty */

struct OdDbMentalRayRenderSettingsGiSampleCountProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsGiSampleCountProperty>::createObject();
    ((OdDbMentalRayRenderSettingsGiSampleCountProperty*)res.get())->init(__OD_T("GiSampleCount"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsGiSampleCountProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->giSampleCount());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsGiSampleCountProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setGISampleCount(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsGiSampleRadiusEnabledProperty */

struct OdDbMentalRayRenderSettingsGiSampleRadiusEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsGiSampleRadiusEnabledProperty>::createObject();
    ((OdDbMentalRayRenderSettingsGiSampleRadiusEnabledProperty*)res.get())->init(__OD_T("GiSampleRadiusEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsGiSampleRadiusEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->giSampleRadiusEnabled());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsGiSampleRadiusEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setGISampleRadiusEnabled(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsGiSampleRadiusProperty */

struct OdDbMentalRayRenderSettingsGiSampleRadiusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsGiSampleRadiusProperty>::createObject();
    ((OdDbMentalRayRenderSettingsGiSampleRadiusProperty*)res.get())->init(__OD_T("GiSampleRadius"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsGiSampleRadiusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->giSampleRadius());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsGiSampleRadiusProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setGISampleRadius(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsGiPhotonsPerLightProperty */

struct OdDbMentalRayRenderSettingsGiPhotonsPerLightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsGiPhotonsPerLightProperty>::createObject();
    ((OdDbMentalRayRenderSettingsGiPhotonsPerLightProperty*)res.get())->init(__OD_T("GiPhotonsPerLight"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsGiPhotonsPerLightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->giPhotonsPerLight());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsGiPhotonsPerLightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setGIPhotonsPerLight(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsFinalGatheringEnabledProperty */

struct OdDbMentalRayRenderSettingsFinalGatheringEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsFinalGatheringEnabledProperty>::createObject();
    ((OdDbMentalRayRenderSettingsFinalGatheringEnabledProperty*)res.get())->init(__OD_T("FinalGatheringEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsFinalGatheringEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->finalGatheringEnabled());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsFinalGatheringEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFinalGatheringEnabled(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsFinalGatheringModeProperty */

struct OdDbMentalRayRenderSettingsFinalGatheringModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsFinalGatheringModeProperty>::createObject();
    ((OdDbMentalRayRenderSettingsFinalGatheringModeProperty*)res.get())->init(__OD_T("FinalGatheringMode"), &OdRxValueType::Desc<OdGiMrFinalGatheringMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsFinalGatheringModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMrFinalGatheringMode>(pObj->finalGatheringMode());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsFinalGatheringModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMrFinalGatheringMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFinalGatheringMode(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsFgRayCountProperty */

struct OdDbMentalRayRenderSettingsFgRayCountProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsFgRayCountProperty>::createObject();
    ((OdDbMentalRayRenderSettingsFgRayCountProperty*)res.get())->init(__OD_T("FgRayCount"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsFgRayCountProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->fgRayCount());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsFgRayCountProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setFGRayCount(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsFgSampleRadiusProperty */

struct OdDbMentalRayRenderSettingsFgSampleRadiusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsFgSampleRadiusProperty>::createObject();
    ((OdDbMentalRayRenderSettingsFgSampleRadiusProperty*)res.get())->init(__OD_T("FgSampleRadius"), &OdRxValueType::Desc<OdGeVector2d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsFgSampleRadiusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dMin = 0, dMax = 0; OdGeVector2d vRadius; pObj->fgSampleRadius(dMin, dMax); vRadius.set(dMin, dMax); value = vRadius;
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsFgSampleRadiusProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector2d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setFGSampleRadius((val).x, (val).y);
  return rc;
}

/* OdDbMentalRayRenderSettingsLightLuminanceScaleProperty */

struct OdDbMentalRayRenderSettingsLightLuminanceScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsLightLuminanceScaleProperty>::createObject();
    ((OdDbMentalRayRenderSettingsLightLuminanceScaleProperty*)res.get())->init(__OD_T("LightLuminanceScale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsLightLuminanceScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->lightLuminanceScale());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsLightLuminanceScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setLightLuminanceScale(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsEnergyMultiplierProperty */

struct OdDbMentalRayRenderSettingsEnergyMultiplierProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsEnergyMultiplierProperty>::createObject();
    ((OdDbMentalRayRenderSettingsEnergyMultiplierProperty*)res.get())->init(__OD_T("EnergyMultiplier"), &OdRxValueType::Desc<float>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsEnergyMultiplierProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<float>(pObj->energyMultiplier());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsEnergyMultiplierProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  float val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setEnergyMultiplier(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsDiagnosticModeProperty */

struct OdDbMentalRayRenderSettingsDiagnosticModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsDiagnosticModeProperty>::createObject();
    ((OdDbMentalRayRenderSettingsDiagnosticModeProperty*)res.get())->init(__OD_T("DiagnosticMode"), &OdRxValueType::Desc<OdGiMrDiagnosticMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsDiagnosticModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMrDiagnosticMode>(pObj->diagnosticMode());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsDiagnosticModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMrDiagnosticMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDiagnosticMode(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsDiagnosticPhotonModeProperty */

struct OdDbMentalRayRenderSettingsDiagnosticPhotonModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsDiagnosticPhotonModeProperty>::createObject();
    ((OdDbMentalRayRenderSettingsDiagnosticPhotonModeProperty*)res.get())->init(__OD_T("DiagnosticPhotonMode"), &OdRxValueType::Desc<OdGiMrDiagnosticPhotonMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsDiagnosticPhotonModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMrDiagnosticPhotonMode>(pObj->diagnosticPhotonMode());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsDiagnosticPhotonModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMrDiagnosticPhotonMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDiagnosticPhotonMode(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsDiagnosticSamplesModeProperty */

struct OdDbMentalRayRenderSettingsDiagnosticSamplesModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsDiagnosticSamplesModeProperty>::createObject();
    ((OdDbMentalRayRenderSettingsDiagnosticSamplesModeProperty*)res.get())->init(__OD_T("DiagnosticSamplesMode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsDiagnosticSamplesModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->diagnosticSamplesMode());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsDiagnosticSamplesModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDiagnosticSamplesMode(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsDiagnosticBSPModeProperty */

struct OdDbMentalRayRenderSettingsDiagnosticBSPModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsDiagnosticBSPModeProperty>::createObject();
    ((OdDbMentalRayRenderSettingsDiagnosticBSPModeProperty*)res.get())->init(__OD_T("DiagnosticBSPMode"), &OdRxValueType::Desc<OdGiMrDiagnosticBSPMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsDiagnosticBSPModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMrDiagnosticBSPMode>(pObj->diagnosticBSPMode());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsDiagnosticBSPModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMrDiagnosticBSPMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDiagnosticBSPMode(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsExportMIEnabledProperty */

struct OdDbMentalRayRenderSettingsExportMIEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsExportMIEnabledProperty>::createObject();
    ((OdDbMentalRayRenderSettingsExportMIEnabledProperty*)res.get())->init(__OD_T("ExportMIEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsExportMIEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->exportMIEnabled());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsExportMIEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setExportMIEnabled(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsExportMIModeProperty */

struct OdDbMentalRayRenderSettingsExportMIModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsExportMIModeProperty>::createObject();
    ((OdDbMentalRayRenderSettingsExportMIModeProperty*)res.get())->init(__OD_T("ExportMIMode"), &OdRxValueType::Desc<OdGiMrExportMIMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsExportMIModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMrExportMIMode>(pObj->exportMIMode());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsExportMIModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMrExportMIMode val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setExportMIMode(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsExportMIFileNameProperty */

struct OdDbMentalRayRenderSettingsExportMIFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsExportMIFileNameProperty>::createObject();
    ((OdDbMentalRayRenderSettingsExportMIFileNameProperty*)res.get())->init(__OD_T("ExportMIFileName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsExportMIFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->exportMIFileName());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsExportMIFileNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setExportMIFileName(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsTileSizeProperty */

struct OdDbMentalRayRenderSettingsTileSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsTileSizeProperty>::createObject();
    ((OdDbMentalRayRenderSettingsTileSizeProperty*)res.get())->init(__OD_T("TileSize"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsTileSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->tileSize());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsTileSizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setTileSize(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsTileOrderProperty */

struct OdDbMentalRayRenderSettingsTileOrderProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsTileOrderProperty>::createObject();
    ((OdDbMentalRayRenderSettingsTileOrderProperty*)res.get())->init(__OD_T("TileOrder"), &OdRxValueType::Desc<OdGiMrTileOrder>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsTileOrderProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMrTileOrder>(pObj->tileOrder());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsTileOrderProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMrTileOrder val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTileOrder(val);
  return eOk;
}

/* OdDbMentalRayRenderSettingsMemoryLimitProperty */

struct OdDbMentalRayRenderSettingsMemoryLimitProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsMemoryLimitProperty>::createObject();
    ((OdDbMentalRayRenderSettingsMemoryLimitProperty*)res.get())->init(__OD_T("MemoryLimit"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsMemoryLimitProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->memoryLimit());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsMemoryLimitProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setMemoryLimit(val);
  return rc;
}

/* OdDbMentalRayRenderSettingsShadowSamplingMultiplierProperty */

struct OdDbMentalRayRenderSettingsShadowSamplingMultiplierProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMentalRayRenderSettingsShadowSamplingMultiplierProperty>::createObject();
    ((OdDbMentalRayRenderSettingsShadowSamplingMultiplierProperty*)res.get())->init(__OD_T("ShadowSamplingMultiplier"), &OdRxValueType::Desc<OdDbMentalRayRenderSettings::ShadowSamplingMultiplier>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMentalRayRenderSettingsShadowSamplingMultiplierProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMentalRayRenderSettings::ShadowSamplingMultiplier>(pObj->shadowSamplingMultiplier());
  return eOk;
}

OdResult OdDbMentalRayRenderSettingsShadowSamplingMultiplierProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMentalRayRenderSettingsPtr pObj = OdDbMentalRayRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMentalRayRenderSettings::ShadowSamplingMultiplier val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setShadowSamplingMultiplier(val);
  return rc;
}

void createOdDbMentalRayRenderSettingsProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbMentalRayRenderSettingsShadowModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsShadowMapsEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsRayTracingEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsGlobalIlluminationEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsGiSampleCountProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsGiSampleRadiusEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsGiSampleRadiusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsGiPhotonsPerLightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsFinalGatheringEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsFinalGatheringModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsFgRayCountProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsFgSampleRadiusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsLightLuminanceScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsEnergyMultiplierProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsDiagnosticModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsDiagnosticPhotonModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsDiagnosticSamplesModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsDiagnosticBSPModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsExportMIEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsExportMIModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsExportMIFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsTileSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsTileOrderProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsMemoryLimitProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMentalRayRenderSettingsShadowSamplingMultiplierProperty::createObject(b.owner()));
  b.add(properties.last());
}
