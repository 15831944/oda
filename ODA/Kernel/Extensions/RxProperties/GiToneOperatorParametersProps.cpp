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
#include "DbRootValueTypes.h"
#include "Gi/GiViewportTraits.h"

/* OdGiToneOperatorParametersIsActiveProperty */

struct OdGiToneOperatorParametersIsActiveProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiToneOperatorParametersIsActiveProperty>::createObject();
    ((OdGiToneOperatorParametersIsActiveProperty*)res.get())->init(__OD_T("IsActive"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiToneOperatorParametersIsActiveProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isActive());
  return eOk;
}

OdResult OdGiToneOperatorParametersIsActiveProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsActive(val);
  return eOk;
}

/* OdGiToneOperatorParametersChromaticAdaptationProperty */

struct OdGiToneOperatorParametersChromaticAdaptationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiToneOperatorParametersChromaticAdaptationProperty>::createObject();
    ((OdGiToneOperatorParametersChromaticAdaptationProperty*)res.get())->init(__OD_T("ChromaticAdaptation"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiToneOperatorParametersChromaticAdaptationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->chromaticAdaptation());
  return eOk;
}

OdResult OdGiToneOperatorParametersChromaticAdaptationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setChromaticAdaptation(val);
  return eOk;
}

/* OdGiToneOperatorParametersColorDifferentiationProperty */

struct OdGiToneOperatorParametersColorDifferentiationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiToneOperatorParametersColorDifferentiationProperty>::createObject();
    ((OdGiToneOperatorParametersColorDifferentiationProperty*)res.get())->init(__OD_T("ColorDifferentiation"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiToneOperatorParametersColorDifferentiationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->colorDifferentiation());
  return eOk;
}

OdResult OdGiToneOperatorParametersColorDifferentiationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColorDifferentiation(val);
  return eOk;
}

/* OdGiToneOperatorParametersWhiteColorProperty */

struct OdGiToneOperatorParametersWhiteColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiToneOperatorParametersWhiteColorProperty>::createObject();
    ((OdGiToneOperatorParametersWhiteColorProperty*)res.get())->init(__OD_T("WhiteColor"), &OdRxValueType::Desc<OdCmEntityColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiToneOperatorParametersWhiteColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmEntityColor>(pObj->whiteColor());
  return eOk;
}

OdResult OdGiToneOperatorParametersWhiteColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmEntityColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWhiteColor(val);
  return eOk;
}

/* OdGiToneOperatorParametersProcessBackgroundProperty */

struct OdGiToneOperatorParametersProcessBackgroundProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiToneOperatorParametersProcessBackgroundProperty>::createObject();
    ((OdGiToneOperatorParametersProcessBackgroundProperty*)res.get())->init(__OD_T("ProcessBackground"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiToneOperatorParametersProcessBackgroundProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->processBackground());
  return eOk;
}

OdResult OdGiToneOperatorParametersProcessBackgroundProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setProcessBackground(val);
  return eOk;
}

/* OdGiToneOperatorParametersBrightnessProperty */

struct OdGiToneOperatorParametersBrightnessProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiToneOperatorParametersBrightnessProperty>::createObject();
    ((OdGiToneOperatorParametersBrightnessProperty*)res.get())->init(__OD_T("Brightness"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdGiToneOperatorParametersBrightnessProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdGiToneOperatorParametersBrightnessProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Brightness"));
    ((OdGiToneOperatorParametersBrightnessProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Adjust the brightness of the converted colors."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Render"))
      OdRxCategory::createObject(L"Render", OdRxCategory::rootCategory());
    ((OdGiToneOperatorParametersBrightnessProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Render", 3));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiToneOperatorParametersBrightnessProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->brightness());
  return eOk;
}

OdResult OdGiToneOperatorParametersBrightnessProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBrightness(val);
  return eOk;
}

/* OdGiToneOperatorParametersContrastProperty */

struct OdGiToneOperatorParametersContrastProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiToneOperatorParametersContrastProperty>::createObject();
    ((OdGiToneOperatorParametersContrastProperty*)res.get())->init(__OD_T("Contrast"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdGiToneOperatorParametersContrastProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdGiToneOperatorParametersContrastProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Contrast"));
    ((OdGiToneOperatorParametersContrastProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Adjust the contrast of the converted colors."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Render"))
      OdRxCategory::createObject(L"Render", OdRxCategory::rootCategory());
    ((OdGiToneOperatorParametersContrastProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Render", 4));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiToneOperatorParametersContrastProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->contrast());
  return eOk;
}

OdResult OdGiToneOperatorParametersContrastProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setContrast(val);
  return eOk;
}

/* OdGiToneOperatorParametersMidTonesProperty */

struct OdGiToneOperatorParametersMidTonesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiToneOperatorParametersMidTonesProperty>::createObject();
    ((OdGiToneOperatorParametersMidTonesProperty*)res.get())->init(__OD_T("MidTones"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdGiToneOperatorParametersMidTonesProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdGiToneOperatorParametersMidTonesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Mid tones"));
    ((OdGiToneOperatorParametersMidTonesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Adjust the mid-tone values of the converted colors."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Render"))
      OdRxCategory::createObject(L"Render", OdRxCategory::rootCategory());
    ((OdGiToneOperatorParametersMidTonesProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Render", 5));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiToneOperatorParametersMidTonesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->midTones());
  return eOk;
}

OdResult OdGiToneOperatorParametersMidTonesProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setMidTones(val);
  return eOk;
}

/* OdGiToneOperatorParametersExteriorDaylightProperty */

struct OdGiToneOperatorParametersExteriorDaylightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdGiToneOperatorParametersExteriorDaylightProperty>::createObject();
    ((OdGiToneOperatorParametersExteriorDaylightProperty*)res.get())->init(__OD_T("ExteriorDaylight"), &OdRxValueType::Desc<OdGiToneOperatorParameters::ExteriorDaylightMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdGiToneOperatorParametersExteriorDaylightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiToneOperatorParameters::ExteriorDaylightMode>(pObj->exteriorDaylight());
  return eOk;
}

OdResult OdGiToneOperatorParametersExteriorDaylightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdGiToneOperatorParametersPtr pObj = OdGiToneOperatorParameters::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiToneOperatorParameters::ExteriorDaylightMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setExteriorDaylight(val);
  return eOk;
}

void createOdGiToneOperatorParametersProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdGiToneOperatorParametersIsActiveProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdGiToneOperatorParametersChromaticAdaptationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdGiToneOperatorParametersColorDifferentiationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdGiToneOperatorParametersWhiteColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdGiToneOperatorParametersProcessBackgroundProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdGiToneOperatorParametersBrightnessProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdGiToneOperatorParametersContrastProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdGiToneOperatorParametersMidTonesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdGiToneOperatorParametersExteriorDaylightProperty::createObject(b.owner()));
  b.add(properties.last());
}
