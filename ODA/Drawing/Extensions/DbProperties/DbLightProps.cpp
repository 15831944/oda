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
#include "DbLight.h"
#include "DbValueTypes.h"

/* OdDbLightLampColorTypeProperty */

struct OdDbLightLampColorTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightLampColorTypeProperty>::createObject();
    ((OdDbLightLampColorTypeProperty*)res.get())->init(__OD_T("LampColorType"), &OdRxValueType::Desc<OdDbLight::LampColorType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightLampColorTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbLight::LampColorType>(pObj->lampColorType());
  return eOk;
}

OdResult OdDbLightLampColorTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbLight::LampColorType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLampColorType(val);
  return eOk;
}

/* OdDbLightPhysicalIntensityProperty */

struct OdDbLightPhysicalIntensityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightPhysicalIntensityProperty>::createObject();
    ((OdDbLightPhysicalIntensityProperty*)res.get())->init(__OD_T("PhysicalIntensity"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightPhysicalIntensityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->physicalIntensity());
  return eOk;
}

OdResult OdDbLightPhysicalIntensityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPhysicalIntensity(val);
  return eOk;
}

/* OdDbLightWebFileProperty */

struct OdDbLightWebFileProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightWebFileProperty>::createObject();
    ((OdDbLightWebFileProperty*)res.get())->init(__OD_T("WebFile"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightWebFileProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Web file"));
    ((OdDbLightWebFileProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the IES web file path"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"PhotometricWeb"))
      OdRxCategory::createObject(L"PhotometricWeb", OdRxCategory::rootCategory());
    ((OdDbLightWebFileProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricWeb", 95));
    ((OdDbLightWebFileProperty*)res.get())->attributes().add(OdRxFilePathAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightWebFileProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->webFile());
  return eOk;
}

OdResult OdDbLightWebFileProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWebFile(val);
  return eOk;
}

/* OdDbLightHasTargetProperty */

struct OdDbLightHasTargetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightHasTargetProperty>::createObject();
    ((OdDbLightHasTargetProperty*)res.get())->init(__OD_T("HasTarget"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightHasTargetProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Targeted"));
    ((OdDbLightHasTargetProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the light displays a target grip for orienting the light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbLightHasTargetProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 14));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightHasTargetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasTarget());
  return eOk;
}

OdResult OdDbLightHasTargetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHasTarget(val);
  return eOk;
}

/* OdDbLightLampColorTempProperty */

struct OdDbLightLampColorTempProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightLampColorTempProperty>::createObject();
    ((OdDbLightLampColorTempProperty*)res.get())->init(__OD_T("LampColorTemp"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLightLampColorTempProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightLampColorTempProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Lamp color"));
    ((OdDbLightLampColorTempProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the color of the lamp"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"PhotometricProp"))
      OdRxCategory::createObject(L"PhotometricProp", OdRxCategory::rootCategory());
    ((OdDbLightLampColorTempProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricProp", 50));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightLampColorTempProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->lampColorTemp());
  return eOk;
}

OdResult OdDbLightLampColorTempProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLampColorTemp(val);
  return eOk;
}

/* OdDbLightLampColorPresetProperty */

struct OdDbLightLampColorPresetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightLampColorPresetProperty>::createObject();
    ((OdDbLightLampColorPresetProperty*)res.get())->init(__OD_T("LampColorPreset"), &OdRxValueType::Desc<OdDbLight::LampColorPreset>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightLampColorPresetProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Lamp color"));
    ((OdDbLightLampColorPresetProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the color of the lamp"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"PhotometricProp"))
      OdRxCategory::createObject(L"PhotometricProp", OdRxCategory::rootCategory());
    ((OdDbLightLampColorPresetProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricProp", 51));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightLampColorPresetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbLight::LampColorPreset>(pObj->lampColorPreset());
  return eOk;
}

OdResult OdDbLightLampColorPresetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbLight::LampColorPreset val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLampColorPreset(val);
  return eOk;
}

/* OdDbLightLampColorRGBProperty */

struct OdDbLightLampColorRGBProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightLampColorRGBProperty>::createObject();
    ((OdDbLightLampColorRGBProperty*)res.get())->init(__OD_T("LampColorRGB"), &OdRxValueType::Desc<OdGiColorRGB>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightLampColorRGBProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiColorRGB>(pObj->lampColorRGB());
  return eOk;
}

OdResult OdDbLightLampColorRGBProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiColorRGB val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLampColorRGB(val);
  return eOk;
}

/* OdDbLightResultingColorProperty */

struct OdDbLightResultingColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightResultingColorProperty>::createObject();
    ((OdDbLightResultingColorProperty*)res.get())->init(__OD_T("ResultingColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightResultingColorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Resulting color"));
    ((OdDbLightResultingColorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the resulting color"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"PhotometricProp"))
      OdRxCategory::createObject(L"PhotometricProp", OdRxCategory::rootCategory());
    ((OdDbLightResultingColorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricProp", 52));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLightResultingColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->resultingColor());
  return eOk;
}

/* OdDbLightFalloffAngleProperty */

struct OdDbLightFalloffAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightFalloffAngleProperty>::createObject();
    ((OdDbLightFalloffAngleProperty*)res.get())->init(__OD_T("FalloffAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLightFalloffAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightFalloffAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Falloff angle"));
    ((OdDbLightFalloffAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the angle that defines the full cone of light, also known as the field angle"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLightFalloffAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 25));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightFalloffAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->falloffAngle());
  return eOk;
}

OdResult OdDbLightFalloffAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  double hotspot = pObj->hotspotAngle(); pObj->setHotspotAndFalloff(hotspot, val);
  return eOk;
}

/* OdDbLightWebRotationProperty */

struct OdDbLightWebRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightWebRotationProperty>::createObject();
    ((OdDbLightWebRotationProperty*)res.get())->init(__OD_T("WebRotation"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotate X"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the x axis of the web rotation"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"PhotometricWebOff"))
      OdRxCategory::createObject(L"PhotometricWebOff", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricWebOff", 100));
    res = OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotate Y"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the y axis of the web rotation"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricWebOff", 101));
    res = OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotate Z"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the z axis of the web rotation"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricWebOff", 102));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightWebRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->webRotation());
  return eOk;
}

OdResult OdDbLightWebRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWebRotation(val);
  return eOk;
}

/* OdDbLightHotspotAngleProperty */

struct OdDbLightHotspotAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightHotspotAngleProperty>::createObject();
    ((OdDbLightHotspotAngleProperty*)res.get())->init(__OD_T("HotspotAngle"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLightHotspotAngleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(65536)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightHotspotAngleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Hotspot angle"));
    ((OdDbLightHotspotAngleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the angle that defines the brightest cone of light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLightHotspotAngleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 20));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightHotspotAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->hotspotAngle());
  return eOk;
}

OdResult OdDbLightHotspotAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  double falloffAngle = pObj->falloffAngle(); pObj->setHotspotAndFalloff(val, falloffAngle);
  return eOk;
}

/* OdDbLightPositionProperty */

struct OdDbLightPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightPositionProperty>::createObject();
    ((OdDbLightPositionProperty*)res.get())->init(__OD_T("Position"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightPositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position X;Position Y;Position Z"));
    ((OdDbLightPositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the X, Y, Z coordinate of the position of the light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbLightPositionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
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

OdResult OdDbLightPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

OdResult OdDbLightPositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPosition(val);
  return eOk;
}

/* OdDbLightLightTypeProperty */

struct OdDbLightLightTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightLightTypeProperty>::createObject();
    ((OdDbLightLightTypeProperty*)res.get())->init(__OD_T("LightType"), &OdRxValueType::Desc<OdGiDrawable::DrawableType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLightLightTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiDrawable::DrawableType>(pObj->lightType());
  return eOk;
}

/* OdDbLightIsPlottableProperty */

struct OdDbLightIsPlottableProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightIsPlottableProperty>::createObject();
    ((OdDbLightIsPlottableProperty*)res.get())->init(__OD_T("IsPlottable"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightIsPlottableProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Plot glyph"));
    ((OdDbLightIsPlottableProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the light glyph should be plotted"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLightIsPlottableProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 40));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightIsPlottableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isPlottable());
  return eOk;
}

OdResult OdDbLightIsPlottableProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsPlottable(val);
  return eOk;
}

/* OdDbLightIsOnProperty */

struct OdDbLightIsOnProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightIsOnProperty>::createObject();
    ((OdDbLightIsOnProperty*)res.get())->init(__OD_T("IsOn"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightIsOnProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"On/Off Status"));
    ((OdDbLightIsOnProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the light is turned on or off in the model"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLightIsOnProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 10));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightIsOnProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isOn());
  return eOk;
}

OdResult OdDbLightIsOnProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setOn(val);
  return eOk;
}

/* OdDbLightLightColorProperty */

struct OdDbLightLightColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightLightColorProperty>::createObject();
    ((OdDbLightLightColorProperty*)res.get())->init(__OD_T("LightColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightLightColorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Filter color"));
    ((OdDbLightLightColorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the color of the light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLightLightColorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 35));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightLightColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->lightColor());
  return eOk;
}

OdResult OdDbLightLightColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLightColor(val);
  return eOk;
}

/* OdDbLightGlyphDisplayProperty */

struct OdDbLightGlyphDisplayProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightGlyphDisplayProperty>::createObject();
    ((OdDbLightGlyphDisplayProperty*)res.get())->init(__OD_T("GlyphDisplay"), &OdRxValueType::Desc<OdDbLight::GlyphDisplayType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightGlyphDisplayProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Glyph display"));
    ((OdDbLightGlyphDisplayProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the light glyph should display in the viewport"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLightGlyphDisplayProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 45));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightGlyphDisplayProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbLight::GlyphDisplayType>(pObj->glyphDisplay());
  return eOk;
}

OdResult OdDbLightGlyphDisplayProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbLight::GlyphDisplayType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGlyphDisplay(val);
  return eOk;
}

/* OdDbLightShadowProperty */

struct OdDbLightShadowProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightShadowProperty>::createObject();
    ((OdDbLightShadowProperty*)res.get())->init(__OD_T("Shadow"), &OdRxValueType::Desc<OdGiShadowParameters>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"CastsShadows", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Shadows"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the light casts shadows"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 15));
    res = OdRxSubProperty::createObject(L"LightShadowType", OdRxValueType::Desc<OdGiShadowParameters::ShadowType>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Type"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the type of shadow cast by the light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"ShadowDetail"))
      OdRxCategory::createObject(L"ShadowDetail", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"ShadowDetail", 1));
    res = OdRxSubProperty::createObject(L"ShadowMapSize", OdRxValueType::Desc<OdUInt16>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Map size"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the size of the shadow map"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"ShadowDetail", 5));
    res = OdRxSubProperty::createObject(L"ShadowSoftness", OdRxValueType::Desc<unsigned char>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Softness"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the softness or fuzziness of shadow-mapped shadows"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"ShadowDetail", 10));
    res = OdRxSubProperty::createObject(L"ShadowSamples", OdRxValueType::Desc<unsigned short>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Samples"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of shadow rays to shoot for the light"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"ShadowDetail", 15));
    res = OdRxSubProperty::createObject(L"ShadowVisible", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Visible in render"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the light shape is actually rendered"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"ShadowDetail", 19));
    res = OdRxSubProperty::createObject(L"ShadowShape", OdRxValueType::Desc<OdGiShadowParameters::ExtendedLightShape>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Shape"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the shape of the lamp bulb"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"ShadowDetail", 20));
    res = OdRxSubProperty::createObject(L"ShadowLength", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Length"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies spatial dimensions of shape"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"ShadowDetail", 25));
    res = OdRxSubProperty::createObject(L"ShadowWidth", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Width"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies spatial dimensions of shape"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"ShadowDetail", 30));
    res = OdRxSubProperty::createObject(L"ShadowRadius", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Radius"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies spatial dimensions of shape"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"ShadowDetail", 35));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightShadowProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGiShadowParameters>(pObj->shadowParameters());
  return eOk;
}

OdResult OdDbLightShadowProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiShadowParameters val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setShadowParameters(val);
  return eOk;
}

/* OdDbLightTargetLocationProperty */

struct OdDbLightTargetLocationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightTargetLocationProperty>::createObject();
    ((OdDbLightTargetLocationProperty*)res.get())->init(__OD_T("TargetLocation"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightTargetLocationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Target X;Target Y;Target Z"));
    ((OdDbLightTargetLocationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the X, Y, Z coordinate of the target position of the light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbLightTargetLocationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 5));
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

OdResult OdDbLightTargetLocationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->targetLocation());
  return eOk;
}

OdResult OdDbLightTargetLocationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTargetLocation(val);
  return eOk;
}

/* OdDbLightAttenuationProperty */

struct OdDbLightAttenuationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightAttenuationProperty>::createObject();
    ((OdDbLightAttenuationProperty*)res.get())->init(__OD_T("Attenuation"), &OdRxValueType::Desc<OdGiLightAttenuation>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"UseLimits", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Use limits"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether use limits"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Attenuation"))
      OdRxCategory::createObject(L"Attenuation", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Attenuation", 5));
    res = OdRxSubProperty::createObject(L"AttenuationType", OdRxValueType::Desc<OdGiLightAttenuation::AttenuationType>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Type"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the type of attenuation for the light"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Attenuation", 1));
    res = OdRxSubProperty::createObject(L"StartLimit", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Start limit offset"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the point where light begins to affect the model; enter a distance from the light"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Attenuation", 10));
    res = OdRxSubProperty::createObject(L"EndLimit", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"End limit offset"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the point where light no longer affects the model"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Attenuation", 15));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightAttenuationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGiLightAttenuation>(pObj->lightAttenuation());
  return eOk;
}

OdResult OdDbLightAttenuationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiLightAttenuation val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setLightAttenuation(val);
  return eOk;
}

/* OdDbLightNameProperty */

struct OdDbLightNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightNameProperty>::createObject();
    ((OdDbLightNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Name"));
    ((OdDbLightNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the light name"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLightNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 1));
    ((OdDbLightNameProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->name());
  return eOk;
}

OdResult OdDbLightNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setName(val);
  return eOk;
}

/* OdDbLightDirectionProperty */

struct OdDbLightDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightDirectionProperty>::createObject();
    ((OdDbLightDirectionProperty*)res.get())->init(__OD_T("Direction"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbLightDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGeVector3d>(pObj->lightDirection());
  return eOk;
}

OdResult OdDbLightDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setLightDirection(val);
  return eOk;
}

/* OdDbLightPhysicalIntensityMethodProperty */

struct OdDbLightPhysicalIntensityMethodProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightPhysicalIntensityMethodProperty>::createObject();
    ((OdDbLightPhysicalIntensityMethodProperty*)res.get())->init(__OD_T("PhysicalIntensityMethod"), &OdRxValueType::Desc<OdDbLight::PhysicalIntensityMethod>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightPhysicalIntensityMethodProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Intensity method"));
    ((OdDbLightPhysicalIntensityMethodProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the intensity type of the light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"PhotometricProp"))
      OdRxCategory::createObject(L"PhotometricProp", OdRxCategory::rootCategory());
    ((OdDbLightPhysicalIntensityMethodProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricProp", 60));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightPhysicalIntensityMethodProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbLight::PhysicalIntensityMethod>(pObj->physicalIntensityMethod());
  return eOk;
}

OdResult OdDbLightPhysicalIntensityMethodProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbLight::PhysicalIntensityMethod val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPhysicalIntensityMethod(val);
  return eOk;
}

/* OdDbLightLampIntensityProperty */

struct OdDbLightLampIntensityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightLampIntensityProperty>::createObject();
    ((OdDbLightLampIntensityProperty*)res.get())->init(__OD_T("LampIntensity"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLightLampIntensityProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightLampIntensityProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Lamp intensity"));
    ((OdDbLightLampIntensityProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the intensity of the lamp"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"PhotometricProp"))
      OdRxCategory::createObject(L"PhotometricProp", OdRxCategory::rootCategory());
    ((OdDbLightLampIntensityProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricProp", 65));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightLampIntensityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->physicalIntensity());
  return eOk;
}

OdResult OdDbLightLampIntensityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setPhysicalIntensity(val);
  return eOk;
}

/* OdDbLightIlluminanceDistanceProperty */

struct OdDbLightIlluminanceDistanceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightIlluminanceDistanceProperty>::createObject();
    ((OdDbLightIlluminanceDistanceProperty*)res.get())->init(__OD_T("IlluminanceDistance"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLightIlluminanceDistanceProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightIlluminanceDistanceProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Distance"));
    ((OdDbLightIlluminanceDistanceProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the illuminance distance of the light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"PhotometricProp"))
      OdRxCategory::createObject(L"PhotometricProp", OdRxCategory::rootCategory());
    ((OdDbLightIlluminanceDistanceProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricProp", 70));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightIlluminanceDistanceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->illuminanceDistance());
  return eOk;
}

OdResult OdDbLightIlluminanceDistanceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIlluminanceDistance(val);
  return eOk;
}

/* OdDbLightIntensityProperty */

struct OdDbLightIntensityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightIntensityProperty>::createObject();
    ((OdDbLightIntensityProperty*)res.get())->init(__OD_T("Intensity"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLightIntensityProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightIntensityProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Intensity factor"));
    ((OdDbLightIntensityProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the intensity factor for the light"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"PhotometricProp"))
      OdRxCategory::createObject(L"PhotometricProp", OdRxCategory::rootCategory());
    ((OdDbLightIntensityProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"PhotometricProp", 75));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightIntensityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->intensity());
  return eOk;
}

OdResult OdDbLightIntensityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIntensity(val);
  return eOk;
}

/* OdDbLightFromVectorProperty */

struct OdDbLightFromVectorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightFromVectorProperty>::createObject();
    ((OdDbLightFromVectorProperty*)res.get())->init(__OD_T("FromVector"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightFromVectorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"From Vector X;From Vector Y;From Vector Z"));
    ((OdDbLightFromVectorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the light FROM vector; Enter coordinate values for X,Y,Z, or use the pointing device."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbLightFromVectorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 6));
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

OdResult OdDbLightFromVectorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

OdResult OdDbLightFromVectorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setPosition(val);
  return eOk;
}

/* OdDbLightToVectorProperty */

struct OdDbLightToVectorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightToVectorProperty>::createObject();
    ((OdDbLightToVectorProperty*)res.get())->init(__OD_T("ToVector"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLightToVectorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"To Vector X;To Vector Y;To Vector Z"));
    ((OdDbLightToVectorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the light to vector; Enter coordinate values for X,Y,Z, or use the pointing device."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbLightToVectorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 10));
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

OdResult OdDbLightToVectorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGePoint3d>(pObj->targetLocation());
  return eOk;
}

OdResult OdDbLightToVectorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setTargetLocation(val);
  return eOk;
}

/* OdDbLightSourceVectorProperty */

struct OdDbLightSourceVectorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLightSourceVectorProperty>::createObject();
    ((OdDbLightSourceVectorProperty*)res.get())->init(__OD_T("SourceVector"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Source Vector X"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the light source vector that results from the light position you set using Azimuth and Altitude; enter coordinate values for X, or use the calculator."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 15));
    res = OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Source Vector Y"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the light source vector that results from the light position you set use Azimuth and Altitude; enter coordinate values for Y, or use the calculator."));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 16));
    res = OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Source Vector Z"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the light source vector that results from the light position you set use Azimuth and Altitude; enter coordinate values for Z, or use the calculator."));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 17));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLightSourceVectorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGeVector3d>(pObj->lightDirection());
  return eOk;
}

OdResult OdDbLightSourceVectorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLightPtr pObj = OdDbLight::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setLightDirection(val);
  return eOk;
}

void createOdDbLightProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbLightLampColorTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightPhysicalIntensityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightWebFileProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightHasTargetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightLampColorTempProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightLampColorPresetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightLampColorRGBProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightResultingColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightFalloffAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightWebRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightHotspotAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightLightTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightIsPlottableProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightIsOnProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightLightColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightGlyphDisplayProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightShadowProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightTargetLocationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightAttenuationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightPhysicalIntensityMethodProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightLampIntensityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightIlluminanceDistanceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightIntensityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightFromVectorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightToVectorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLightSourceVectorProperty::createObject(b.owner()));
  b.add(properties.last());
}
