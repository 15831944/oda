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
#include "DbMaterial.h"
#include "DbValueTypes.h"

/* OdDbMaterialTransmittanceScaleProperty */

struct OdDbMaterialTransmittanceScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialTransmittanceScaleProperty>::createObject();
    ((OdDbMaterialTransmittanceScaleProperty*)res.get())->init(__OD_T("TransmittanceScale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialTransmittanceScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->transmittanceScale());
  return eOk;
}

OdResult OdDbMaterialTransmittanceScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTransmittanceScale(val);
  return eOk;
}

/* OdDbMaterialColorBleedScaleProperty */

struct OdDbMaterialColorBleedScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialColorBleedScaleProperty>::createObject();
    ((OdDbMaterialColorBleedScaleProperty*)res.get())->init(__OD_T("ColorBleedScale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialColorBleedScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->colorBleedScale());
  return eOk;
}

OdResult OdDbMaterialColorBleedScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColorBleedScale(val);
  return eOk;
}

/* OdDbMaterialIndirectBumpScaleProperty */

struct OdDbMaterialIndirectBumpScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialIndirectBumpScaleProperty>::createObject();
    ((OdDbMaterialIndirectBumpScaleProperty*)res.get())->init(__OD_T("IndirectBumpScale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialIndirectBumpScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->indirectBumpScale());
  return eOk;
}

OdResult OdDbMaterialIndirectBumpScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIndirectBumpScale(val);
  return eOk;
}

/* OdDbMaterialReflectanceScaleProperty */

struct OdDbMaterialReflectanceScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialReflectanceScaleProperty>::createObject();
    ((OdDbMaterialReflectanceScaleProperty*)res.get())->init(__OD_T("ReflectanceScale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialReflectanceScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->reflectanceScale());
  return eOk;
}

OdResult OdDbMaterialReflectanceScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setReflectanceScale(val);
  return eOk;
}

/* OdDbMaterialLuminanceProperty */

struct OdDbMaterialLuminanceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialLuminanceProperty>::createObject();
    ((OdDbMaterialLuminanceProperty*)res.get())->init(__OD_T("Luminance"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialLuminanceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->luminance());
  return eOk;
}

OdResult OdDbMaterialLuminanceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLuminance(val);
  return eOk;
}

/* OdDbMaterialTwoSidedProperty */

struct OdDbMaterialTwoSidedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialTwoSidedProperty>::createObject();
    ((OdDbMaterialTwoSidedProperty*)res.get())->init(__OD_T("TwoSided"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialTwoSidedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->twoSided());
  return eOk;
}

OdResult OdDbMaterialTwoSidedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTwoSided(val);
  return eOk;
}

/* OdDbMaterialAnonymousProperty */

struct OdDbMaterialAnonymousProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialAnonymousProperty>::createObject();
    ((OdDbMaterialAnonymousProperty*)res.get())->init(__OD_T("Anonymous"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialAnonymousProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isAnonymous());
  return eOk;
}

OdResult OdDbMaterialAnonymousProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAnonymous(val);
  return eOk;
}

/* OdDbMaterialTranslucenceProperty */

struct OdDbMaterialTranslucenceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialTranslucenceProperty>::createObject();
    ((OdDbMaterialTranslucenceProperty*)res.get())->init(__OD_T("Translucence"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMaterialTranslucenceProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialTranslucenceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->translucence());
  return eOk;
}

OdResult OdDbMaterialTranslucenceProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTranslucence(val);
  return eOk;
}

/* OdDbMaterialReflectivityProperty */

struct OdDbMaterialReflectivityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialReflectivityProperty>::createObject();
    ((OdDbMaterialReflectivityProperty*)res.get())->init(__OD_T("Reflectivity"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMaterialReflectivityProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialReflectivityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->reflectivity());
  return eOk;
}

OdResult OdDbMaterialReflectivityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setReflectivity(val);
  return eOk;
}

/* OdDbMaterialSelfIlluminationProperty */

struct OdDbMaterialSelfIlluminationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialSelfIlluminationProperty>::createObject();
    ((OdDbMaterialSelfIlluminationProperty*)res.get())->init(__OD_T("SelfIllumination"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMaterialSelfIlluminationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialSelfIlluminationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->selfIllumination());
  return eOk;
}

OdResult OdDbMaterialSelfIlluminationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSelfIllumination(val);
  return eOk;
}

/* OdDbMaterialAmbientProperty */

struct OdDbMaterialAmbientProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialAmbientProperty>::createObject();
    ((OdDbMaterialAmbientProperty*)res.get())->init(__OD_T("Ambient"), &OdRxValueType::Desc<OdGiMaterialColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialAmbientProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGiMaterialColor materialColor; pObj->ambient(materialColor); value = materialColor;
  return eOk;
}

OdResult OdDbMaterialAmbientProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMaterialColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAmbient(val);
  return eOk;
}

/* OdDbMaterialDescriptionProperty */

struct OdDbMaterialDescriptionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialDescriptionProperty>::createObject();
    ((OdDbMaterialDescriptionProperty*)res.get())->init(__OD_T("Description"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialDescriptionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->description());
  return eOk;
}

OdResult OdDbMaterialDescriptionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDescription(val);
  return eOk;
}

/* OdDbMaterialLuminanceModeProperty */

struct OdDbMaterialLuminanceModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialLuminanceModeProperty>::createObject();
    ((OdDbMaterialLuminanceModeProperty*)res.get())->init(__OD_T("LuminanceMode"), &OdRxValueType::Desc<OdGiMaterialTraits::LuminanceMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialLuminanceModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMaterialTraits::LuminanceMode>(pObj->luminanceMode());
  return eOk;
}

OdResult OdDbMaterialLuminanceModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMaterialTraits::LuminanceMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLuminanceMode(val);
  return eOk;
}

/* OdDbMaterialNameProperty */

struct OdDbMaterialNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialNameProperty>::createObject();
    ((OdDbMaterialNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->name());
  return eOk;
}

OdResult OdDbMaterialNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setName(val);
  return eOk;
}

/* OdDbMaterialGlobalIlluminationProperty */

struct OdDbMaterialGlobalIlluminationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialGlobalIlluminationProperty>::createObject();
    ((OdDbMaterialGlobalIlluminationProperty*)res.get())->init(__OD_T("GlobalIllumination"), &OdRxValueType::Desc<OdGiMaterialTraits::GlobalIlluminationMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialGlobalIlluminationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMaterialTraits::GlobalIlluminationMode>(pObj->globalIllumination());
  return eOk;
}

OdResult OdDbMaterialGlobalIlluminationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMaterialTraits::GlobalIlluminationMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setGlobalIllumination(val);
  return eOk;
}

/* OdDbMaterialFinalGatherProperty */

struct OdDbMaterialFinalGatherProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialFinalGatherProperty>::createObject();
    ((OdDbMaterialFinalGatherProperty*)res.get())->init(__OD_T("FinalGather"), &OdRxValueType::Desc<OdGiMaterialTraits::FinalGatherMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialFinalGatherProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMaterialTraits::FinalGatherMode>(pObj->finalGather());
  return eOk;
}

OdResult OdDbMaterialFinalGatherProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMaterialTraits::FinalGatherMode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFinalGather(val);
  return eOk;
}

/* OdDbMaterialChannelFlagsProperty */

struct OdDbMaterialChannelFlagsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialChannelFlagsProperty>::createObject();
    ((OdDbMaterialChannelFlagsProperty*)res.get())->init(__OD_T("ChannelFlags"), &OdRxValueType::Desc<OdGiMaterialTraits::ChannelFlags>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialChannelFlagsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMaterialTraits::ChannelFlags>(pObj->channelFlags());
  return eOk;
}

OdResult OdDbMaterialChannelFlagsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMaterialTraits::ChannelFlags val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setChannelFlags(val);
  return eOk;
}

/* OdDbMaterialModeProperty */

struct OdDbMaterialModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialModeProperty>::createObject();
    ((OdDbMaterialModeProperty*)res.get())->init(__OD_T("Mode"), &OdRxValueType::Desc<OdGiMaterialTraits::Mode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMaterialTraits::Mode>(pObj->mode());
  return eOk;
}

OdResult OdDbMaterialModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMaterialTraits::Mode val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setMode(val);
  return eOk;
}

/* OdDbMaterialIlluminationModelProperty */

struct OdDbMaterialIlluminationModelProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMaterialIlluminationModelProperty>::createObject();
    ((OdDbMaterialIlluminationModelProperty*)res.get())->init(__OD_T("IlluminationModel"), &OdRxValueType::Desc<OdGiMaterialTraits::IlluminationModel>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMaterialIlluminationModelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMaterialTraits::IlluminationModel>(pObj->illuminationModel());
  return eOk;
}

OdResult OdDbMaterialIlluminationModelProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMaterialPtr pObj = OdDbMaterial::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMaterialTraits::IlluminationModel val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIlluminationModel(val);
  return eOk;
}

void createOdDbMaterialProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbMaterialTransmittanceScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialColorBleedScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialIndirectBumpScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialReflectanceScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialLuminanceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialTwoSidedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialAnonymousProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialTranslucenceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialReflectivityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialSelfIlluminationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialAmbientProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialDescriptionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialLuminanceModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialGlobalIlluminationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialFinalGatherProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialChannelFlagsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMaterialIlluminationModelProperty::createObject(b.owner()));
  b.add(properties.last());
}
