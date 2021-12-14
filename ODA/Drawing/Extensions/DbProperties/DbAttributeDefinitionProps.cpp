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
#include "DbAttributeDefinition.h"
#include "DbValueTypes.h"

/* OdDbAttributeDefinitionConstantProperty */

struct OdDbAttributeDefinitionConstantProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionConstantProperty>::createObject();
    ((OdDbAttributeDefinitionConstantProperty*)res.get())->init(__OD_T("Constant"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionConstantProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Constant"));
    ((OdDbAttributeDefinitionConstantProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the constant mode of the attribute"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionConstantProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 53));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionConstantProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isConstant());
  return eOk;
}

OdResult OdDbAttributeDefinitionConstantProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setConstant(val);
  return eOk;
}

/* OdDbAttributeDefinitionVerifiableProperty */

struct OdDbAttributeDefinitionVerifiableProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionVerifiableProperty>::createObject();
    ((OdDbAttributeDefinitionVerifiableProperty*)res.get())->init(__OD_T("Verifiable"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionVerifiableProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Verify"));
    ((OdDbAttributeDefinitionVerifiableProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the verify mode of the attribute"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionVerifiableProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 54));
    ((OdDbAttributeDefinitionVerifiableProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Verify"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionVerifiableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isVerifiable());
  return eOk;
}

OdResult OdDbAttributeDefinitionVerifiableProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setVerifiable(val);
  return eOk;
}

/* OdDbAttributeDefinitionInvisibleProperty */

struct OdDbAttributeDefinitionInvisibleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionInvisibleProperty>::createObject();
    ((OdDbAttributeDefinitionInvisibleProperty*)res.get())->init(__OD_T("Invisible"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionInvisibleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Invisible"));
    ((OdDbAttributeDefinitionInvisibleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the invisible mode of the attribute"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionInvisibleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 52));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionInvisibleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isInvisible());
  return eOk;
}

OdResult OdDbAttributeDefinitionInvisibleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setInvisible(val);
  return eOk;
}

/* OdDbAttributeDefinitionPromptProperty */

struct OdDbAttributeDefinitionPromptProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionPromptProperty>::createObject();
    ((OdDbAttributeDefinitionPromptProperty*)res.get())->init(__OD_T("Prompt"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionPromptProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Prompt"));
    ((OdDbAttributeDefinitionPromptProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the prompt string of the attribute"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionPromptProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 5));
    ((OdDbAttributeDefinitionPromptProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbAttributeDefinitionPromptProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"PromptString"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionPromptProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->prompt());
  return eOk;
}

OdResult OdDbAttributeDefinitionPromptProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPrompt(val);
  return eOk;
}

/* OdDbAttributeDefinitionTagProperty */

struct OdDbAttributeDefinitionTagProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionTagProperty>::createObject();
    ((OdDbAttributeDefinitionTagProperty*)res.get())->init(__OD_T("Tag"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionTagProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Tag"));
    ((OdDbAttributeDefinitionTagProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the tag string of the attribute"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionTagProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 1));
    ((OdDbAttributeDefinitionTagProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbAttributeDefinitionTagProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TagString"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionTagProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->tag());
  return eOk;
}

OdResult OdDbAttributeDefinitionTagProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTag(val);
  return eOk;
}

/* OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty */

struct OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty>::createObject();
    ((OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty*)res.get())->init(__OD_T("IsMTextAttributeDefinition"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Multiple lines"));
    ((OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether if the attribute is multiline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 56));
    ((OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"MTextAttribute"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isMTextAttributeDefinition());
  return eOk;
}

OdResult OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->convertIntoMTextAttributeDefinition(val);
  return eOk;
}

/* OdDbAttributeDefinitionValueProperty */

struct OdDbAttributeDefinitionValueProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionValueProperty>::createObject();
    ((OdDbAttributeDefinitionValueProperty*)res.get())->init(__OD_T("Value"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionValueProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Value"));
    ((OdDbAttributeDefinitionValueProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text string of the attribute"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionValueProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 1));
    ((OdDbAttributeDefinitionValueProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionValueProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->textString());
  return eOk;
}

OdResult OdDbAttributeDefinitionValueProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setTextString(val);
  return eOk;
}

/* OdDbAttributeDefinitionWidthProperty */

struct OdDbAttributeDefinitionWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionWidthProperty>::createObject();
    ((OdDbAttributeDefinitionWidthProperty*)res.get())->init(__OD_T("Width"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbAttributeDefinitionWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Multiline text width"));
    ((OdDbAttributeDefinitionWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Gets the width of text boundary of the Mtext"));
    ((OdDbAttributeDefinitionWidthProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"MTextBoundaryWidth"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionWidthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 41));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbAttributeDefinitionWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->widthFactor());
  return eOk;
}

/* OdDbAttributeDefinitionLockPositionInBlockProperty */

struct OdDbAttributeDefinitionLockPositionInBlockProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionLockPositionInBlockProperty>::createObject();
    ((OdDbAttributeDefinitionLockPositionInBlockProperty*)res.get())->init(__OD_T("LockPositionInBlock"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionLockPositionInBlockProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Lock position"));
    ((OdDbAttributeDefinitionLockPositionInBlockProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the attribute may be moved relative to the geometry in the block"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionLockPositionInBlockProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 57));
    ((OdDbAttributeDefinitionLockPositionInBlockProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LockPosition"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionLockPositionInBlockProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->lockPositionInBlock());
  return eOk;
}

OdResult OdDbAttributeDefinitionLockPositionInBlockProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLockPositionInBlock(val);
  return eOk;
}

/* OdDbAttributeDefinitionPresetProperty */

struct OdDbAttributeDefinitionPresetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionPresetProperty>::createObject();
    ((OdDbAttributeDefinitionPresetProperty*)res.get())->init(__OD_T("Preset"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeDefinitionPresetProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Preset"));
    ((OdDbAttributeDefinitionPresetProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the preset mode of the attribute"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbAttributeDefinitionPresetProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 55));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionPresetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isPreset());
  return eOk;
}

OdResult OdDbAttributeDefinitionPresetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPreset(val);
  return eOk;
}

/* OdDbAttributeDefinitionFieldLengthProperty */

struct OdDbAttributeDefinitionFieldLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeDefinitionFieldLengthProperty>::createObject();
    ((OdDbAttributeDefinitionFieldLengthProperty*)res.get())->init(__OD_T("FieldLength"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeDefinitionFieldLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>STATIC_CAST
  value = static_cast<int>(pObj->fieldLength());
  return eOk;
}

OdResult OdDbAttributeDefinitionFieldLengthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributeDefinitionPtr pObj = OdDbAttributeDefinition::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFieldLength(val);
  return eOk;
}

void createOdDbAttributeDefinitionProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbAttributeDefinitionConstantProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionVerifiableProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionInvisibleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionPromptProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionTagProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionIsMTextAttributeDefinitionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionValueProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionLockPositionInBlockProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionPresetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeDefinitionFieldLengthProperty::createObject(b.owner()));
  b.add(properties.last());
}
