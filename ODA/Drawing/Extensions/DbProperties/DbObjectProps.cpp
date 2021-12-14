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
#include "DbObject.h"
#include "DbValueTypes.h"
#include "DbAnnotativeObjectPE.h"

/* OdDbObjectIsModifiedXDataProperty */

struct OdDbObjectIsModifiedXDataProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsModifiedXDataProperty>::createObject();
    ((OdDbObjectIsModifiedXDataProperty*)res.get())->init(__OD_T("IsModifiedXData"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsModifiedXDataProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isModifiedXData());
  return eOk;
}

/* OdDbObjectIsModifiedProperty */

struct OdDbObjectIsModifiedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsModifiedProperty>::createObject();
    ((OdDbObjectIsModifiedProperty*)res.get())->init(__OD_T("IsModified"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsModifiedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isModified());
  return eOk;
}

/* OdDbObjectIsUndoingProperty */

struct OdDbObjectIsUndoingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsUndoingProperty>::createObject();
    ((OdDbObjectIsUndoingProperty*)res.get())->init(__OD_T("IsUndoing"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsUndoingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isUndoing());
  return eOk;
}

/* OdDbObjectIsNotifyingProperty */

struct OdDbObjectIsNotifyingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsNotifyingProperty>::createObject();
    ((OdDbObjectIsNotifyingProperty*)res.get())->init(__OD_T("IsNotifying"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsNotifyingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isNotifying());
  return eOk;
}

/* OdDbObjectIsNewObjectProperty */

struct OdDbObjectIsNewObjectProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsNewObjectProperty>::createObject();
    ((OdDbObjectIsNewObjectProperty*)res.get())->init(__OD_T("IsNewObject"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsNewObjectProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isNewObject());
  return eOk;
}

/* OdDbObjectIsErasedProperty */

struct OdDbObjectIsErasedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsErasedProperty>::createObject();
    ((OdDbObjectIsErasedProperty*)res.get())->init(__OD_T("IsErased"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsErasedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isErased());
  return eOk;
}

/* OdDbObjectIsEraseStatusToggledProperty */

struct OdDbObjectIsEraseStatusToggledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsEraseStatusToggledProperty>::createObject();
    ((OdDbObjectIsEraseStatusToggledProperty*)res.get())->init(__OD_T("IsEraseStatusToggled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsEraseStatusToggledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isEraseStatusToggled());
  return eOk;
}

/* OdDbObjectExtensionDictionaryProperty */

struct OdDbObjectExtensionDictionaryProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectExtensionDictionaryProperty>::createObject();
    ((OdDbObjectExtensionDictionaryProperty*)res.get())->init(__OD_T("ExtensionDictionary"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbObjectExtensionDictionaryProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectExtensionDictionaryProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->extensionDictionary());
  return eOk;
}

/* OdDbObjectIsNotifyEnabledProperty */

struct OdDbObjectIsNotifyEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsNotifyEnabledProperty>::createObject();
    ((OdDbObjectIsNotifyEnabledProperty*)res.get())->init(__OD_T("IsNotifyEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsNotifyEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isNotifyEnabled());
  return eOk;
}

/* OdDbObjectIsWriteEnabledProperty */

struct OdDbObjectIsWriteEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsWriteEnabledProperty>::createObject();
    ((OdDbObjectIsWriteEnabledProperty*)res.get())->init(__OD_T("IsWriteEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsWriteEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isWriteEnabled());
  return eOk;
}

/* OdDbObjectIsReadEnabledProperty */

struct OdDbObjectIsReadEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsReadEnabledProperty>::createObject();
    ((OdDbObjectIsReadEnabledProperty*)res.get())->init(__OD_T("IsReadEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsReadEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isReadEnabled());
  return eOk;
}

/* OdDbObjectHasSaveVersionOverrideProperty */

struct OdDbObjectHasSaveVersionOverrideProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectHasSaveVersionOverrideProperty>::createObject();
    ((OdDbObjectHasSaveVersionOverrideProperty*)res.get())->init(__OD_T("HasSaveVersionOverride"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbObjectHasSaveVersionOverrideProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasSaveVersionOverride());
  return eOk;
}

OdResult OdDbObjectHasSaveVersionOverrideProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setHasSaveVersionOverride(val);
  return eOk;
}

/* OdDbObjectObjectIdProperty */

struct OdDbObjectObjectIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectObjectIdProperty>::createObject();
    ((OdDbObjectObjectIdProperty*)res.get())->init(__OD_T("ObjectId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbObjectObjectIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectObjectIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->objectId());
  return eOk;
}

/* OdDbObjectIsModifiedGraphicsProperty */

struct OdDbObjectIsModifiedGraphicsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsModifiedGraphicsProperty>::createObject();
    ((OdDbObjectIsModifiedGraphicsProperty*)res.get())->init(__OD_T("IsModifiedGraphics"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsModifiedGraphicsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isModifiedGraphics());
  return eOk;
}

/* OdDbObjectHasFieldsProperty */

struct OdDbObjectHasFieldsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectHasFieldsProperty>::createObject();
    ((OdDbObjectHasFieldsProperty*)res.get())->init(__OD_T("HasFields"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectHasFieldsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasFields());
  return eOk;
}

/* OdDbObjectIsAProxyProperty */

struct OdDbObjectIsAProxyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsAProxyProperty>::createObject();
    ((OdDbObjectIsAProxyProperty*)res.get())->init(__OD_T("IsAProxy"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsAProxyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isAProxy());
  return eOk;
}

/* OdDbObjectIsReallyClosingProperty */

struct OdDbObjectIsReallyClosingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsReallyClosingProperty>::createObject();
    ((OdDbObjectIsReallyClosingProperty*)res.get())->init(__OD_T("IsReallyClosing"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsReallyClosingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isReallyClosing());
  return eOk;
}

/* OdDbObjectIsObjectIdsInFluxProperty */

struct OdDbObjectIsObjectIdsInFluxProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsObjectIdsInFluxProperty>::createObject();
    ((OdDbObjectIsObjectIdsInFluxProperty*)res.get())->init(__OD_T("IsObjectIdsInFlux"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsObjectIdsInFluxProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<bool>(pObj->isOdDbObjectIdsInFlux());
  return eOk;
}

/* OdDbObjectHandleProperty */

struct OdDbObjectHandleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectHandleProperty>::createObject();
    ((OdDbObjectHandleProperty*)res.get())->init(__OD_T("Handle"), &OdRxValueType::Desc<OdDbHandle>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectHandleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbHandle>(pObj->handle());
  return eOk;
}

/* OdDbObjectOwnerIdProperty */

struct OdDbObjectOwnerIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectOwnerIdProperty>::createObject();
    ((OdDbObjectOwnerIdProperty*)res.get())->init(__OD_T("OwnerId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbObjectOwnerIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectOwnerIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->ownerId());
  return eOk;
}

/* OdDbObjectIsPersistentProperty */

struct OdDbObjectIsPersistentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectIsPersistentProperty>::createObject();
    ((OdDbObjectIsPersistentProperty*)res.get())->init(__OD_T("IsPersistent"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectIsPersistentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isPersistent());
  return eOk;
}

/* OdDbObjectMergeStyleProperty */

struct OdDbObjectMergeStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectMergeStyleProperty>::createObject();
    ((OdDbObjectMergeStyleProperty*)res.get())->init(__OD_T("MergeStyle"), &OdRxValueType::Desc<OdDb::DuplicateRecordCloning>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbObjectMergeStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::DuplicateRecordCloning>(pObj->mergeStyle());
  return eOk;
}

/* OdDbObjectAnnotativeProperty */

struct OdDbObjectAnnotativeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectAnnotativeProperty>::createObject();
    ((OdDbObjectAnnotativeProperty*)res.get())->init(__OD_T("Annotative"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbObjectAnnotativeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Annotative"));
    ((OdDbObjectAnnotativeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether or not the dimension is annotative"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"AnnotationScaling"))
      OdRxCategory::createObject(L"AnnotationScaling", OdRxCategory::rootCategory());
    ((OdDbObjectAnnotativeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"AnnotationScaling", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbObjectAnnotativeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  OdDbAnnotativeObjectPEPtr a = OdDbAnnotativeObjectPE::cast(pO);
  if (pObj.isNull() || a.isNull())
    return eNotApplicable;
  value = a->annotative(pObj);
  return eOk;
  //<<REPLACE_ALL
}

OdResult OdDbObjectAnnotativeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>REPLACE_ALL
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  OdDbAnnotativeObjectPEPtr a = OdDbAnnotativeObjectPE::cast(pO);
  if (pObj.isNull() || a.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  a->setAnnotative(pObj, val);
  return eOk;
  //<<REPLACE_ALL
}

void createOdDbObjectProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbObjectIsModifiedXDataProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsModifiedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsUndoingProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsNotifyingProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsNewObjectProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsErasedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsEraseStatusToggledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectExtensionDictionaryProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsNotifyEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsWriteEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsReadEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectHasSaveVersionOverrideProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectObjectIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsModifiedGraphicsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectHasFieldsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsAProxyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsReallyClosingProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsObjectIdsInFluxProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectHandleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectOwnerIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectIsPersistentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectMergeStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbObjectAnnotativeProperty::createObject(b.owner()));
  b.add(properties.last());
}
