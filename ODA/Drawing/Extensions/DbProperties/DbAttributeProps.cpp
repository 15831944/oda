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
#include "DbAttribute.h"
#include "DbValueTypes.h"

/* OdDbAttributeIsVerifiableProperty */

struct OdDbAttributeIsVerifiableProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeIsVerifiableProperty>::createObject();
    ((OdDbAttributeIsVerifiableProperty*)res.get())->init(__OD_T("IsVerifiable"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbAttributeIsVerifiableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isVerifiable());
  return eOk;
}

/* OdDbAttributeTagProperty */

struct OdDbAttributeTagProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeTagProperty>::createObject();
    ((OdDbAttributeTagProperty*)res.get())->init(__OD_T("Tag"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeTagProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Tag"));
    ((OdDbAttributeTagProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the tag string of the attribute reference"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbAttributeTagProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 10));
    ((OdDbAttributeTagProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TagString"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeTagProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->tag());
  return eOk;
}

OdResult OdDbAttributeTagProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTag(val);
  return eOk;
}

/* OdDbAttributeInvisibleProperty */

struct OdDbAttributeInvisibleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeInvisibleProperty>::createObject();
    ((OdDbAttributeInvisibleProperty*)res.get())->init(__OD_T("Invisible"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeInvisibleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Invisible"));
    ((OdDbAttributeInvisibleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the invisible mode of the attribute reference"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbAttributeInvisibleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 0));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeInvisibleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isInvisible());
  return eOk;
}

OdResult OdDbAttributeInvisibleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setInvisible(val);
  return eOk;
}

/* OdDbAttributeIsPresetProperty */

struct OdDbAttributeIsPresetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeIsPresetProperty>::createObject();
    ((OdDbAttributeIsPresetProperty*)res.get())->init(__OD_T("IsPreset"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbAttributeIsPresetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isPreset());
  return eOk;
}

/* OdDbAttributeIsMTextAttributeProperty */

struct OdDbAttributeIsMTextAttributeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeIsMTextAttributeProperty>::createObject();
    ((OdDbAttributeIsMTextAttributeProperty*)res.get())->init(__OD_T("IsMTextAttribute"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeIsMTextAttributeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Multiple lines"));
    ((OdDbAttributeIsMTextAttributeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether if the attribute reference is multiline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbAttributeIsMTextAttributeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 80));
    ((OdDbAttributeIsMTextAttributeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"MTextAttribute"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbAttributeIsMTextAttributeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isMTextAttribute());
  return eOk;
}

/* OdDbAttributeValueProperty */

struct OdDbAttributeValueProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeValueProperty>::createObject();
    ((OdDbAttributeValueProperty*)res.get())->init(__OD_T("Value"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeValueProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Value"));
    ((OdDbAttributeValueProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text string of the attribute reference"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbAttributeValueProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeValueProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->textString());
  return eOk;
}

OdResult OdDbAttributeValueProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setTextString(val);
  return eOk;
}

/* OdDbAttributeWidthProperty */

struct OdDbAttributeWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeWidthProperty>::createObject();
    ((OdDbAttributeWidthProperty*)res.get())->init(__OD_T("Width"), &OdRxValueType::Desc<double>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Multiline text width"));
    ((OdDbAttributeWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Gets the width of text boundary of the Mtext"));
    ((OdDbAttributeWidthProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"MTextBoundaryWidth"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->widthFactor());
  return eOk;
}

OdResult OdDbAttributeWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setWidthFactor(val);
  return eOk;
}

/* OdDbAttributeFieldLengthProperty */

struct OdDbAttributeFieldLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeFieldLengthProperty>::createObject();
    ((OdDbAttributeFieldLengthProperty*)res.get())->init(__OD_T("FieldLength"), &OdRxValueType::Desc<int>::value(), owner);
    ((OdDbAttributeFieldLengthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the field length of the attribute reference"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeFieldLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->fieldLength());
  return eOk;
}

OdResult OdDbAttributeFieldLengthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFieldLength(val);
  return eOk;
}

/* OdDbAttributeLockPositionInBlockProperty */

struct OdDbAttributeLockPositionInBlockProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbAttributeLockPositionInBlockProperty>::createObject();
    ((OdDbAttributeLockPositionInBlockProperty*)res.get())->init(__OD_T("LockPositionInBlock"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbAttributeLockPositionInBlockProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Lock position"));
    ((OdDbAttributeLockPositionInBlockProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether the attribute may be moved relative to the geometry in the block"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbAttributeLockPositionInBlockProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 85));
    ((OdDbAttributeLockPositionInBlockProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LockPosition"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbAttributeLockPositionInBlockProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->lockPositionInBlock());
  return eOk;
}

OdResult OdDbAttributeLockPositionInBlockProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbAttributePtr pObj = OdDbAttribute::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLockPositionInBlock(val);
  return eOk;
}

void createOdDbAttributeProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbAttributeIsVerifiableProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeTagProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeInvisibleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeIsPresetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeIsMTextAttributeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeValueProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeFieldLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbAttributeLockPositionInBlockProperty::createObject(b.owner()));
  b.add(properties.last());
}
