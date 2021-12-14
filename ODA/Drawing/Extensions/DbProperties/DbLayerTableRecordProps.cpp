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
#include "DbLayerTableRecord.h"
#include "DbValueTypes.h"

/* OdDbLayerTableRecordColorProperty */

struct OdDbLayerTableRecordColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordColorProperty>::createObject();
    ((OdDbLayerTableRecordColorProperty*)res.get())->init(__OD_T("Color"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordColorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 5));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordColorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Color"));
    ((OdDbLayerTableRecordColorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the color of an entity or layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->color());
  return eOk;
}

OdResult OdDbLayerTableRecordColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColor(val);
  return eOk;
}

/* OdDbLayerTableRecordLinetypeObjectIdProperty */

struct OdDbLayerTableRecordLinetypeObjectIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordLinetypeObjectIdProperty>::createObject();
    ((OdDbLayerTableRecordLinetypeObjectIdProperty*)res.get())->init(__OD_T("LinetypeObjectId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbLayerTableRecordLinetypeObjectIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/LinetypeTableId/Items"));
    ((OdDbLayerTableRecordLinetypeObjectIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordLinetypeObjectIdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 6));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordLinetypeObjectIdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Linetype"));
    ((OdDbLayerTableRecordLinetypeObjectIdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the linetype of the layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordLinetypeObjectIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->linetypeObjectId());
  return eOk;
}

OdResult OdDbLayerTableRecordLinetypeObjectIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLinetypeObjectId(val);
  return eOk;
}

/* OdDbLayerTableRecordTransparencyProperty */

struct OdDbLayerTableRecordTransparencyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordTransparencyProperty>::createObject();
    ((OdDbLayerTableRecordTransparencyProperty*)res.get())->init(__OD_T("Transparency"), &OdRxValueType::Desc<OdCmTransparency>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordTransparencyProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 8));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordTransparencyProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Transparency"));
    ((OdDbLayerTableRecordTransparencyProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the transparency of the layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordTransparencyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmTransparency>(pObj->transparency());
  return eOk;
}

OdResult OdDbLayerTableRecordTransparencyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmTransparency val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTransparency(val);
  return eOk;
}

/* OdDbLayerTableRecordIsLockedProperty */

struct OdDbLayerTableRecordIsLockedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordIsLockedProperty>::createObject();
    ((OdDbLayerTableRecordIsLockedProperty*)res.get())->init(__OD_T("IsLocked"), &OdRxValueType::Desc<bool>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordIsLockedProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 4));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordIsLockedProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Lock"));
    ((OdDbLayerTableRecordIsLockedProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Locks or unlocks a layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordIsLockedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isLocked());
  return eOk;
}

OdResult OdDbLayerTableRecordIsLockedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsLocked(val);
  return eOk;
}

/* OdDbLayerTableRecordIsFrozenProperty */

struct OdDbLayerTableRecordIsFrozenProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordIsFrozenProperty>::createObject();
    ((OdDbLayerTableRecordIsFrozenProperty*)res.get())->init(__OD_T("IsFrozen"), &OdRxValueType::Desc<bool>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordIsFrozenProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 3));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordIsFrozenProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Freeze"));
    ((OdDbLayerTableRecordIsFrozenProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the freeze status of a layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordIsFrozenProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isFrozen());
  return eOk;
}

OdResult OdDbLayerTableRecordIsFrozenProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsFrozen(val);
  return eOk;
}

/* OdDbLayerTableRecordIsUsedProperty */

struct OdDbLayerTableRecordIsUsedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordIsUsedProperty>::createObject();
    ((OdDbLayerTableRecordIsUsedProperty*)res.get())->init(__OD_T("IsUsed"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLayerTableRecordIsUsedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isInUse());
  return eOk;
}

/* OdDbLayerTableRecordViewportVisibilityDefaultProperty */

struct OdDbLayerTableRecordViewportVisibilityDefaultProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordViewportVisibilityDefaultProperty>::createObject();
    ((OdDbLayerTableRecordViewportVisibilityDefaultProperty*)res.get())->init(__OD_T("ViewportVisibilityDefault"), &OdRxValueType::Desc<bool>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordViewportVisibilityDefaultProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 11));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordViewportVisibilityDefaultProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Freeze in New Viewports"));
    ((OdDbLayerTableRecordViewportVisibilityDefaultProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies if the layer is to be frozen in new viewports"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordViewportVisibilityDefaultProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->VPDFLT());
  return eOk;
}

OdResult OdDbLayerTableRecordViewportVisibilityDefaultProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setVPDFLT(val);
  return eOk;
}

/* OdDbLayerTableRecordIsOffProperty */

struct OdDbLayerTableRecordIsOffProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordIsOffProperty>::createObject();
    ((OdDbLayerTableRecordIsOffProperty*)res.get())->init(__OD_T("IsOff"), &OdRxValueType::Desc<bool>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordIsOffProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 2));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordIsOffProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"On"));
    ((OdDbLayerTableRecordIsOffProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the state of a layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordIsOffProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isOff());
  return eOk;
}

OdResult OdDbLayerTableRecordIsOffProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsOff(val);
  return eOk;
}

/* OdDbLayerTableRecordIsHiddenProperty */

struct OdDbLayerTableRecordIsHiddenProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordIsHiddenProperty>::createObject();
    ((OdDbLayerTableRecordIsHiddenProperty*)res.get())->init(__OD_T("IsHidden"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordIsHiddenProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isHidden());
  return eOk;
}

OdResult OdDbLayerTableRecordIsHiddenProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsHidden(val);
  return eOk;
}

/* OdDbLayerTableRecordDescriptionProperty */

struct OdDbLayerTableRecordDescriptionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordDescriptionProperty>::createObject();
    ((OdDbLayerTableRecordDescriptionProperty*)res.get())->init(__OD_T("Description"), &OdRxValueType::Desc<OdString>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordDescriptionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 12));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordDescriptionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Description"));
    ((OdDbLayerTableRecordDescriptionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the description of the layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordDescriptionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->description());
  return eOk;
}

OdResult OdDbLayerTableRecordDescriptionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDescription(val);
  return eOk;
}

/* OdDbLayerTableRecordIsReconciledProperty */

struct OdDbLayerTableRecordIsReconciledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordIsReconciledProperty>::createObject();
    ((OdDbLayerTableRecordIsReconciledProperty*)res.get())->init(__OD_T("IsReconciled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordIsReconciledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isReconciled());
  return eOk;
}

OdResult OdDbLayerTableRecordIsReconciledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsReconciled(val);
  return eOk;
}

/* OdDbLayerTableRecordPlotStyleNameIdProperty */

struct OdDbLayerTableRecordPlotStyleNameIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordPlotStyleNameIdProperty>::createObject();
    ((OdDbLayerTableRecordPlotStyleNameIdProperty*)res.get())->init(__OD_T("PlotStyleNameId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbLayerTableRecordPlotStyleNameIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordPlotStyleNameIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->plotStyleNameId());
  return eOk;
}

OdResult OdDbLayerTableRecordPlotStyleNameIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setPlotStyleName(val);
  return eOk;
}

/* OdDbLayerTableRecordIsPlottableProperty */

struct OdDbLayerTableRecordIsPlottableProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordIsPlottableProperty>::createObject();
    ((OdDbLayerTableRecordIsPlottableProperty*)res.get())->init(__OD_T("IsPlottable"), &OdRxValueType::Desc<bool>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordIsPlottableProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 9));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordIsPlottableProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Plot"));
    ((OdDbLayerTableRecordIsPlottableProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies wether the layer is plottable."));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordIsPlottableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isPlottable());
  return eOk;
}

OdResult OdDbLayerTableRecordIsPlottableProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsPlottable(val);
  return eOk;
}

/* OdDbLayerTableRecordMaterialIdProperty */

struct OdDbLayerTableRecordMaterialIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordMaterialIdProperty>::createObject();
    ((OdDbLayerTableRecordMaterialIdProperty*)res.get())->init(__OD_T("MaterialId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbLayerTableRecordMaterialIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordMaterialIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->materialId());
  return eOk;
}

OdResult OdDbLayerTableRecordMaterialIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setMaterialId(val);
  return eOk;
}

/* OdDbLayerTableRecordPlotStyleNameProperty */

struct OdDbLayerTableRecordPlotStyleNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordPlotStyleNameProperty>::createObject();
    ((OdDbLayerTableRecordPlotStyleNameProperty*)res.get())->init(__OD_T("PlotStyleName"), &OdRxValueType::Desc<OdString>::value(), owner);
    ((OdDbLayerTableRecordPlotStyleNameProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/PlotStyleNameDictionaryId/Items"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordPlotStyleNameProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 10));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordPlotStyleNameProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Plot Style"));
    ((OdDbLayerTableRecordPlotStyleNameProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the plotstyle name for the layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordPlotStyleNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->plotStyleName());
  return eOk;
}

OdResult OdDbLayerTableRecordPlotStyleNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPlotStyleName(val);
  return eOk;
}

/* OdDbLayerTableRecordLineWeightProperty */

struct OdDbLayerTableRecordLineWeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayerTableRecordLineWeightProperty>::createObject();
    ((OdDbLayerTableRecordLineWeightProperty*)res.get())->init(__OD_T("LineWeight"), &OdRxValueType::Desc<OdDb::LineWeight>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"General"))
      OdRxCategory::createObject(L"General", OdRxCategory::rootCategory());
    ((OdDbLayerTableRecordLineWeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"General", 7));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLayerTableRecordLineWeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Lineweight"));
    ((OdDbLayerTableRecordLineWeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the lineweight for the layer"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayerTableRecordLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineWeight>(pObj->lineWeight());
  return eOk;
}

OdResult OdDbLayerTableRecordLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayerTableRecordPtr pObj = OdDbLayerTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineWeight val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLineWeight(val);
  return eOk;
}

void createOdDbLayerTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbLayerTableRecordColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordLinetypeObjectIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordTransparencyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordIsLockedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordIsFrozenProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordIsUsedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordViewportVisibilityDefaultProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordIsOffProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordIsHiddenProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordDescriptionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordIsReconciledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordPlotStyleNameIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordIsPlottableProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordMaterialIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordPlotStyleNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayerTableRecordLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
}
