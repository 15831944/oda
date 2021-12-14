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

/* OdDbRenderSettingsDisplayIndexProperty */

struct OdDbRenderSettingsDisplayIndexProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsDisplayIndexProperty>::createObject();
    ((OdDbRenderSettingsDisplayIndexProperty*)res.get())->init(__OD_T("DisplayIndex"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsDisplayIndexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->displayIndex());
  return eOk;
}

OdResult OdDbRenderSettingsDisplayIndexProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDisplayIndex(val);
  return eOk;
}

/* OdDbRenderSettingsDiagnosticBackgroundEnabledProperty */

struct OdDbRenderSettingsDiagnosticBackgroundEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsDiagnosticBackgroundEnabledProperty>::createObject();
    ((OdDbRenderSettingsDiagnosticBackgroundEnabledProperty*)res.get())->init(__OD_T("DiagnosticBackgroundEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsDiagnosticBackgroundEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->diagnosticBackgroundEnabled());
  return eOk;
}

OdResult OdDbRenderSettingsDiagnosticBackgroundEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDiagnosticBackgroundEnabled(val);
  return eOk;
}

/* OdDbRenderSettingsNameProperty */

struct OdDbRenderSettingsNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsNameProperty>::createObject();
    ((OdDbRenderSettingsNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->name());
  return eOk;
}

OdResult OdDbRenderSettingsNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setName(val);
  return rc;
}

/* OdDbRenderSettingsDescriptionProperty */

struct OdDbRenderSettingsDescriptionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsDescriptionProperty>::createObject();
    ((OdDbRenderSettingsDescriptionProperty*)res.get())->init(__OD_T("Description"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsDescriptionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->description());
  return eOk;
}

OdResult OdDbRenderSettingsDescriptionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDescription(val);
  return eOk;
}

/* OdDbRenderSettingsMaterialsEnabledProperty */

struct OdDbRenderSettingsMaterialsEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsMaterialsEnabledProperty>::createObject();
    ((OdDbRenderSettingsMaterialsEnabledProperty*)res.get())->init(__OD_T("MaterialsEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsMaterialsEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->materialsEnabled());
  return eOk;
}

OdResult OdDbRenderSettingsMaterialsEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setMaterialsEnabled(val);
  return eOk;
}

/* OdDbRenderSettingsShadowsEnabledProperty */

struct OdDbRenderSettingsShadowsEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsShadowsEnabledProperty>::createObject();
    ((OdDbRenderSettingsShadowsEnabledProperty*)res.get())->init(__OD_T("ShadowsEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsShadowsEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->shadowsEnabled());
  return eOk;
}

OdResult OdDbRenderSettingsShadowsEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShadowsEnabled(val);
  return eOk;
}

/* OdDbRenderSettingsPreviewImageFileNameProperty */

struct OdDbRenderSettingsPreviewImageFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsPreviewImageFileNameProperty>::createObject();
    ((OdDbRenderSettingsPreviewImageFileNameProperty*)res.get())->init(__OD_T("PreviewImageFileName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsPreviewImageFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->previewImageFileName());
  return eOk;
}

OdResult OdDbRenderSettingsPreviewImageFileNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setPreviewImageFileName(val);
  return rc;
}

/* OdDbRenderSettingsTextureSamplingProperty */

struct OdDbRenderSettingsTextureSamplingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsTextureSamplingProperty>::createObject();
    ((OdDbRenderSettingsTextureSamplingProperty*)res.get())->init(__OD_T("TextureSampling"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsTextureSamplingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->textureSampling());
  return eOk;
}

OdResult OdDbRenderSettingsTextureSamplingProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextureSampling(val);
  return eOk;
}

/* OdDbRenderSettingsBackFacesEnabledProperty */

struct OdDbRenderSettingsBackFacesEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsBackFacesEnabledProperty>::createObject();
    ((OdDbRenderSettingsBackFacesEnabledProperty*)res.get())->init(__OD_T("BackFacesEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsBackFacesEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->backFacesEnabled());
  return eOk;
}

OdResult OdDbRenderSettingsBackFacesEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackFacesEnabled(val);
  return eOk;
}

/* OdDbRenderSettingsIsPredefinedProperty */

struct OdDbRenderSettingsIsPredefinedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderSettingsIsPredefinedProperty>::createObject();
    ((OdDbRenderSettingsIsPredefinedProperty*)res.get())->init(__OD_T("IsPredefined"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderSettingsIsPredefinedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isPredefined());
  return eOk;
}

OdResult OdDbRenderSettingsIsPredefinedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderSettingsPtr pObj = OdDbRenderSettings::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setIsPredefined(val);
  return rc;
}

void createOdDbRenderSettingsProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRenderSettingsDisplayIndexProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderSettingsDiagnosticBackgroundEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderSettingsNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderSettingsDescriptionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderSettingsMaterialsEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderSettingsShadowsEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderSettingsPreviewImageFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderSettingsTextureSamplingProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderSettingsBackFacesEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderSettingsIsPredefinedProperty::createObject(b.owner()));
  b.add(properties.last());
}
