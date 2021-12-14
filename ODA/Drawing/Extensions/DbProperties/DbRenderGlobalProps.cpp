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

/* OdDbRenderGlobalSaveFileNameProperty */

struct OdDbRenderGlobalSaveFileNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderGlobalSaveFileNameProperty>::createObject();
    ((OdDbRenderGlobalSaveFileNameProperty*)res.get())->init(__OD_T("SaveFileName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderGlobalSaveFileNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->saveFileName());
  return eOk;
}

OdResult OdDbRenderGlobalSaveFileNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setSaveFileName(val);
  return rc;
}

/* OdDbRenderGlobalSaveEnabledProperty */

struct OdDbRenderGlobalSaveEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderGlobalSaveEnabledProperty>::createObject();
    ((OdDbRenderGlobalSaveEnabledProperty*)res.get())->init(__OD_T("SaveEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderGlobalSaveEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->saveEnabled());
  return eOk;
}

OdResult OdDbRenderGlobalSaveEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSaveEnabled(val);
  return eOk;
}

/* OdDbRenderGlobalPredefinedPresetsFirstProperty */

struct OdDbRenderGlobalPredefinedPresetsFirstProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderGlobalPredefinedPresetsFirstProperty>::createObject();
    ((OdDbRenderGlobalPredefinedPresetsFirstProperty*)res.get())->init(__OD_T("PredefinedPresetsFirst"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderGlobalPredefinedPresetsFirstProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->predefinedPresetsFirst());
  return eOk;
}

OdResult OdDbRenderGlobalPredefinedPresetsFirstProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPredefinedPresetsFirst(val);
  return eOk;
}

/* OdDbRenderGlobalHighInfoLevelProperty */

struct OdDbRenderGlobalHighInfoLevelProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderGlobalHighInfoLevelProperty>::createObject();
    ((OdDbRenderGlobalHighInfoLevelProperty*)res.get())->init(__OD_T("HighInfoLevel"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderGlobalHighInfoLevelProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->highInfoLevel());
  return eOk;
}

OdResult OdDbRenderGlobalHighInfoLevelProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHighInfoLevel(val);
  return eOk;
}

/* OdDbRenderGlobalProcedureProperty */

struct OdDbRenderGlobalProcedureProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderGlobalProcedureProperty>::createObject();
    ((OdDbRenderGlobalProcedureProperty*)res.get())->init(__OD_T("Procedure"), &OdRxValueType::Desc<OdDbRenderGlobal::Procedure>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderGlobalProcedureProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbRenderGlobal::Procedure>(pObj->procedure());
  return eOk;
}

OdResult OdDbRenderGlobalProcedureProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbRenderGlobal::Procedure val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setProcedure(val);
  return rc;
}

/* OdDbRenderGlobalDestinationProperty */

struct OdDbRenderGlobalDestinationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderGlobalDestinationProperty>::createObject();
    ((OdDbRenderGlobalDestinationProperty*)res.get())->init(__OD_T("Destination"), &OdRxValueType::Desc<OdDbRenderGlobal::Destination>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderGlobalDestinationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbRenderGlobal::Destination>(pObj->destination());
  return eOk;
}

OdResult OdDbRenderGlobalDestinationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbRenderGlobal::Destination val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setDestination(val);
  return rc;
}

/* OdDbRenderGlobalExposureTypeProperty */

struct OdDbRenderGlobalExposureTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbRenderGlobalExposureTypeProperty>::createObject();
    ((OdDbRenderGlobalExposureTypeProperty*)res.get())->init(__OD_T("ExposureType"), &OdRxValueType::Desc<OdGiMrExposureType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbRenderGlobalExposureTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiMrExposureType>(pObj->exposureType());
  return eOk;
}

OdResult OdDbRenderGlobalExposureTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbRenderGlobalPtr pObj = OdDbRenderGlobal::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiMrExposureType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setExposureType(val);
  return rc;
}

void createOdDbRenderGlobalProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbRenderGlobalSaveFileNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderGlobalSaveEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderGlobalPredefinedPresetsFirstProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderGlobalHighInfoLevelProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderGlobalProcedureProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderGlobalDestinationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbRenderGlobalExposureTypeProperty::createObject(b.owner()));
  b.add(properties.last());
}
