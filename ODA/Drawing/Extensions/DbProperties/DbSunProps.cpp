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
#include "DbSun.h"
#include "DbValueTypes.h"

/* OdDbSunSkyParametersProperty */

struct OdDbSunSkyParametersProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunSkyParametersProperty>::createObject();
    ((OdDbSunSkyParametersProperty*)res.get())->init(__OD_T("SkyParameters"), &OdRxValueType::Desc<OdGiSkyParameters>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"DiskIntensity", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Disk intensity"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun Disk Appearance"))
      OdRxCategory::createObject(L"Sun Disk Appearance", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun Disk Appearance", 16));
    res = OdRxSubProperty::createObject(L"ShadowSample", OdRxValueType::Desc<unsigned short>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Samples"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Rendered Shadow Details"))
      OdRxCategory::createObject(L"Rendered Shadow Details", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Rendered Shadow Details", 24));
    res = OdRxSubProperty::createObject(L"GlowIntensity", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Glow intensity"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun Disk Appearance", 15));
    res = OdRxSubProperty::createObject(L"DiskScale", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Disk scale"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun Disk Appearance", 14));
    res = OdRxSubProperty::createObject(L"VisibilityDistance", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Visibility distance"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Advanced Sky"))
      OdRxCategory::createObject(L"Advanced Sky", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Advanced Sky", 13));
    res = OdRxSubProperty::createObject(L"AerialPerspective", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Aerial perspective"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Advanced Sky", 12));
    res = OdRxSubProperty::createObject(L"NightColor", OdRxValueType::Desc<OdCmColor>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Night Color"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Advanced Sky", 11));
    res = OdRxSubProperty::createObject(L"GroundColor", OdRxValueType::Desc<OdCmColor>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Ground Color"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sky Horizon"))
      OdRxCategory::createObject(L"Sky Horizon", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sky Horizon", 10));
    res = OdRxSubProperty::createObject(L"Blur", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Blur"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sky Horizon", 9));
    res = OdRxSubProperty::createObject(L"Height", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Height"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sky Horizon", 8));
    res = OdRxSubProperty::createObject(L"Haze", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Haze"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sky", 7));
    res = OdRxSubProperty::createObject(L"IntensityFactor", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Intensity Factor"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sky", 6));
    res = OdRxSubProperty::createObject(L"DisplaySky", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Display sky"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sky", 5));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunSkyParametersProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGiSkyParameters params; pObj->skyParameters(params); value = params;
  return eOk;
}

OdResult OdDbSunSkyParametersProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiSkyParameters val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSkyParameters(val);
  return eOk;
}

/* OdDbSunShadowParametersProperty */

struct OdDbSunShadowParametersProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunShadowParametersProperty>::createObject();
    ((OdDbSunShadowParametersProperty*)res.get())->init(__OD_T("ShadowParameters"), &OdRxValueType::Desc<OdGiShadowParameters>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"ShadowSoftness", OdRxValueType::Desc<unsigned char>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Softness"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Rendered Shadow Details"))
      OdRxCategory::createObject(L"Rendered Shadow Details", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Rendered Shadow Details", 25));
    res = OdRxSubProperty::createObject(L"SunShadowType", OdRxValueType::Desc<OdGiShadowParameters::ShadowType>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Type"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Rendered Shadow Details", 23));
    res = OdRxSubProperty::createObject(L"CastsShadows", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Casts shadows"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun"))
      OdRxCategory::createObject(L"Sun", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun", 4));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunShadowParametersProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiShadowParameters>(pObj->shadowParameters());
  return eOk;
}

OdResult OdDbSunShadowParametersProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiShadowParameters val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShadowParameters(val);
  return eOk;
}

/* OdDbSunIntensityProperty */

struct OdDbSunIntensityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunIntensityProperty>::createObject();
    ((OdDbSunIntensityProperty*)res.get())->init(__OD_T("Intensity"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbSunIntensityProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSunIntensityProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Intensity Factor"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun"))
      OdRxCategory::createObject(L"Sun", OdRxCategory::rootCategory());
    ((OdDbSunIntensityProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun", 2));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunIntensityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->intensity());
  return eOk;
}

OdResult OdDbSunIntensityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIntensity(val);
  return eOk;
}

/* OdDbSunSunColorProperty */

struct OdDbSunSunColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunSunColorProperty>::createObject();
    ((OdDbSunSunColorProperty*)res.get())->init(__OD_T("SunColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSunSunColorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Color"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun"))
      OdRxCategory::createObject(L"Sun", OdRxCategory::rootCategory());
    ((OdDbSunSunColorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun", 3));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunSunColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->sunColor());
  return eOk;
}

OdResult OdDbSunSunColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSunColor(val);
  return eOk;
}

/* OdDbSunDisplaySunLightProperty */

struct OdDbSunDisplaySunLightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunDisplaySunLightProperty>::createObject();
    ((OdDbSunDisplaySunLightProperty*)res.get())->init(__OD_T("DisplaySunLight"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSunDisplaySunLightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Display sun light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun"))
      OdRxCategory::createObject(L"Sun", OdRxCategory::rootCategory());
    ((OdDbSunDisplaySunLightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunDisplaySunLightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isOn());
  return eOk;
}

OdResult OdDbSunDisplaySunLightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setOn(val);
  return eOk;
}

/* OdDbSunDateTimeProperty */

struct OdDbSunDateTimeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunDateTimeProperty>::createObject();
    ((OdDbSunDateTimeProperty*)res.get())->init(__OD_T("DateTime"), &OdRxValueType::Desc<OdDbDate>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSunDateTimeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Date and Time"));
    ((OdDbSunDateTimeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Displays the current date setting."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun Angle"))
      OdRxCategory::createObject(L"Sun Angle", OdRxCategory::rootCategory());
    ((OdDbSunDateTimeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun Angle", 30));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunDateTimeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbDate>(pObj->dateTime());
  return eOk;
}

OdResult OdDbSunDateTimeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbDate val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDateTime(val);
  return eOk;
}

/* OdDbSunIsDaylightSavingsOnProperty */

struct OdDbSunIsDaylightSavingsOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunIsDaylightSavingsOnProperty>::createObject();
    ((OdDbSunIsDaylightSavingsOnProperty*)res.get())->init(__OD_T("IsDaylightSavingsOn"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSunIsDaylightSavingsOnProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Daylight Saving"));
    ((OdDbSunIsDaylightSavingsOnProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Displays the current setting for daylight saving time."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun Angle"))
      OdRxCategory::createObject(L"Sun Angle", OdRxCategory::rootCategory());
    ((OdDbSunIsDaylightSavingsOnProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun Angle", 34));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunIsDaylightSavingsOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isDayLightSavingsOn());
  return eOk;
}

OdResult OdDbSunIsDaylightSavingsOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setDayLightSavingsOn(val);
  return eOk;
}

/* OdDbSunAzimuthProperty */

struct OdDbSunAzimuthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunAzimuthProperty>::createObject();
    ((OdDbSunAzimuthProperty*)res.get())->init(__OD_T("Azimuth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbSunAzimuthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSunAzimuthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Azimuth"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun Angle"))
      OdRxCategory::createObject(L"Sun Angle", OdRxCategory::rootCategory());
    ((OdDbSunAzimuthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun Angle", 35));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunAzimuthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->azimuth());
  return eOk;
}

OdResult OdDbSunAzimuthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setAzimuth(val);
  return rc;
}

/* OdDbSunAltitudeProperty */

struct OdDbSunAltitudeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunAltitudeProperty>::createObject();
    ((OdDbSunAltitudeProperty*)res.get())->init(__OD_T("Altitude"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbSunAltitudeProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSunAltitudeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Altitude"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun Angle"))
      OdRxCategory::createObject(L"Sun Angle", OdRxCategory::rootCategory());
    ((OdDbSunAltitudeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun Angle", 36));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunAltitudeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->altitude());
  return eOk;
}

OdResult OdDbSunAltitudeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setAltitude(val);
  return rc;
}

/* OdDbSunSunDirectionProperty */

struct OdDbSunSunDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSunSunDirectionProperty>::createObject();
    ((OdDbSunSunDirectionProperty*)res.get())->init(__OD_T("SunDirection"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSunSunDirectionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Source Vector X;Source Vector Y;Source Vector Z"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Sun Angle"))
      OdRxCategory::createObject(L"Sun Angle", OdRxCategory::rootCategory());
    ((OdDbSunSunDirectionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Sun Angle", 37));
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbSunSunDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->sunDirection());
  return eOk;
}

OdResult OdDbSunSunDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbSunPtr pObj = OdDbSun::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSunDirection(val);
  return eOk;
}

void createOdDbSunProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbSunSkyParametersProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSunShadowParametersProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSunIntensityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSunSunColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSunDisplaySunLightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSunDateTimeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSunIsDaylightSavingsOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSunAzimuthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSunAltitudeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSunSunDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
}
