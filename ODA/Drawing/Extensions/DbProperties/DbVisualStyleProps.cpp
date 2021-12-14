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
#include "DbVisualStyle.h"
#include "DbValueTypes.h"

/* OdDbVisualStyleTypeProperty */

struct OdDbVisualStyleTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleTypeProperty>::createObject();
    ((OdDbVisualStyleTypeProperty*)res.get())->init(__OD_T("Type"), &OdRxValueType::Desc<OdGiVisualStyle::Type>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbVisualStyleTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGiVisualStyle::Type>(pObj->type());
  return eOk;
}

OdResult OdDbVisualStyleTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiVisualStyle::Type val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setType(val);
  return eOk;
}

/* OdDbVisualStyleNameProperty */

struct OdDbVisualStyleNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleNameProperty>::createObject();
    ((OdDbVisualStyleNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbVisualStyleNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdString strName; pObj->name(strName); value = strName;
  return eOk;
}

/* OdDbVisualStyleInternalUseOnlyProperty */

struct OdDbVisualStyleInternalUseOnlyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleInternalUseOnlyProperty>::createObject();
    ((OdDbVisualStyleInternalUseOnlyProperty*)res.get())->init(__OD_T("InternalUseOnly"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbVisualStyleInternalUseOnlyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isInternalUseOnly());
  return eOk;
}

OdResult OdDbVisualStyleInternalUseOnlyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setInternalUseOnly(val);
  return eOk;
}

/* OdDbVisualStyleFaceOpacityProperty */

struct OdDbVisualStyleFaceOpacityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleFaceOpacityProperty>::createObject();
    ((OdDbVisualStyleFaceOpacityProperty*)res.get())->init(__OD_T("FaceOpacity"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbVisualStyleFaceOpacityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = static_cast<int>(pObj->faceStyle().isFaceModifierFlagSet(OdGiFaceStyle::kOpacity));
  return eOk;
}

OdResult OdDbVisualStyleFaceOpacityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  const_cast<OdGiFaceStyle&>(pObj->faceStyle()).setFaceModifierFlag(OdGiFaceStyle::kOpacity, (val != 0));
  return eOk;
}

/* OdDbVisualStyleFaceColorModeProperty */

struct OdDbVisualStyleFaceColorModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleFaceColorModeProperty>::createObject();
    ((OdDbVisualStyleFaceColorModeProperty*)res.get())->init(__OD_T("FaceColorMode"), &OdRxValueType::Desc<OdGiVisualStyleProperties::FaceColorMode>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbVisualStyleFaceColorModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = static_cast<OdGiVisualStyleProperties::FaceColorMode>(pObj->faceStyle().faceColorMode());
  return eOk;
}

OdResult OdDbVisualStyleFaceColorModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiVisualStyleProperties::FaceColorMode val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  const_cast<OdGiFaceStyle&>(pObj->faceStyle()).setFaceColorMode(static_cast<OdGiFaceStyle::FaceColorMode>(val));
  return eOk;
}

/* OdDbVisualStyleMaterialModeProperty */

struct OdDbVisualStyleMaterialModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleMaterialModeProperty>::createObject();
    ((OdDbVisualStyleMaterialModeProperty*)res.get())->init(__OD_T("MaterialMode"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbVisualStyleMaterialModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = static_cast<int>(pObj->displayStyle().isDisplaySettingsFlagSet(OdGiDisplayStyle::kMaterials));
  return eOk;
}

OdResult OdDbVisualStyleMaterialModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  const_cast<OdGiDisplayStyle&>(pObj->displayStyle()).setDisplaySettingsFlag(OdGiDisplayStyle::kMaterials, (val != 0));
  return eOk;
}

/* OdDbVisualStyleEdgeModeProperty */

struct OdDbVisualStyleEdgeModeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleEdgeModeProperty>::createObject();
    ((OdDbVisualStyleEdgeModeProperty*)res.get())->init(__OD_T("EdgeMode"), &OdRxValueType::Desc<OdGiVisualStyleProperties::EdgeModel>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbVisualStyleEdgeModeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = static_cast<OdGiVisualStyleProperties::EdgeModel>(pObj->edgeStyle().edgeModel());
  return eOk;
}

OdResult OdDbVisualStyleEdgeModeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGiVisualStyleProperties::EdgeModel val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  const_cast<OdGiEdgeStyle&>(pObj->edgeStyle()).setEdgeModel(static_cast<OdGiEdgeStyle::EdgeModel>(val));
  return eOk;
}

/* OdDbVisualStyleEdgeJitterProperty */

struct OdDbVisualStyleEdgeJitterProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleEdgeJitterProperty>::createObject();
    ((OdDbVisualStyleEdgeJitterProperty*)res.get())->init(__OD_T("EdgeJitter"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbVisualStyleEdgeJitterProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = static_cast<int>(pObj->edgeStyle().jitterAmount());
  return eOk;
}

OdResult OdDbVisualStyleEdgeJitterProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  const_cast<OdGiEdgeStyle&>(pObj->edgeStyle()).setJitterAmount(static_cast<OdGiEdgeStyle::JitterAmount>(val), false);
  return eOk;
}

/* OdDbVisualStyleSilhWidthProperty */

struct OdDbVisualStyleSilhWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleSilhWidthProperty>::createObject();
    ((OdDbVisualStyleSilhWidthProperty*)res.get())->init(__OD_T("SilhWidth"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbVisualStyleSilhWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = static_cast<int>(pObj->edgeStyle().silhouetteWidth());
  return eOk;
}

OdResult OdDbVisualStyleSilhWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  const_cast<OdGiEdgeStyle&>(pObj->edgeStyle()).setSilhouetteWidth(static_cast<short>(val));
  return eOk;
}

/* OdDbVisualStyleOverHangProperty */

struct OdDbVisualStyleOverHangProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbVisualStyleOverHangProperty>::createObject();
    ((OdDbVisualStyleOverHangProperty*)res.get())->init(__OD_T("OverHang"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbVisualStyleOverHangProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->edgeStyle().overhangAmount();
  return eOk;
}

OdResult OdDbVisualStyleOverHangProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbVisualStylePtr pObj = OdDbVisualStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  const_cast<OdGiEdgeStyle&>(pObj->edgeStyle()).setOverhangAmount(val, false);
  return eOk;
}

void createOdDbVisualStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbVisualStyleTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbVisualStyleNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbVisualStyleInternalUseOnlyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbVisualStyleFaceOpacityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbVisualStyleFaceColorModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbVisualStyleMaterialModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbVisualStyleEdgeModeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbVisualStyleEdgeJitterProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbVisualStyleSilhWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbVisualStyleOverHangProperty::createObject(b.owner()));
  b.add(properties.last());
}
