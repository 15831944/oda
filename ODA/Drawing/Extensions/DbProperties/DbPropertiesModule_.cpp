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
#include "RxDynamicModule.h"
#include "RxSysRegistry.h"
#include "RxObjectImpl.h"
#include "RxValueTypeUtil.h"
#include "RxAttribute.h"
#include "DbObject.h"
#include "DbAnnotativeObjectPE.h"
#include "RxCategory.h"
#include "DynamicLinker.h"
#include "DbValueTypes.h"
#include "DbSymbolTable.h"
#include "DbCurve.h"
#include "DbLine.h"

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsModifiedXData, bool);

OdResult OdDbObjectIsModifiedXDataProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isModifiedXData();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsModified, bool);

OdResult OdDbObjectIsModifiedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isModified();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsUndoing, bool);

OdResult OdDbObjectIsUndoingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isUndoing();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsNotifying, bool);

OdResult OdDbObjectIsNotifyingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isNotifying();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsNewObject, bool);

OdResult OdDbObjectIsNewObjectProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isNewObject();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsErased, bool);

OdResult OdDbObjectIsErasedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isErased();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsEraseStatusToggled, bool);

OdResult OdDbObjectIsEraseStatusToggledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isEraseStatusToggled();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, ExtensionDictionary, OdDbObjectId);

OdResult OdDbObjectExtensionDictionaryProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->extensionDictionary();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsNotifyEnabled, bool);

OdResult OdDbObjectIsNotifyEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isNotifyEnabled();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsWriteEnabled, bool);

OdResult OdDbObjectIsWriteEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isWriteEnabled();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsReadEnabled, bool);

OdResult OdDbObjectIsReadEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isReadEnabled();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsCancelling, bool);

// NOT IMPLEMENTED!//
OdResult OdDbObjectIsCancellingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  return eNotApplicable;
  //  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  //  if (pObj.isNull())
  //    return eNotApplicable;
  //  value = pObj->isCancelling();
  //  return eOk;
}

ODRX_DECLARE_RX_PROPERTY(OdDbObject, HasSaveVersionOverride, bool);

OdResult OdDbObjectHasSaveVersionOverrideProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->hasSaveVersionOverride();
  return eOk;
}

OdResult OdDbObjectHasSaveVersionOverrideProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  pObj->setHasSaveVersionOverride(*rxvalue_cast<bool>(&value));
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, ObjectId, OdDbObjectId);

OdResult OdDbObjectObjectIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->objectId();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsModifiedGraphics, bool);

OdResult OdDbObjectIsModifiedGraphicsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isModifiedGraphics();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, HasFields, bool);

OdResult OdDbObjectHasFieldsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->hasFields();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsAProxy, bool);

OdResult OdDbObjectIsAProxyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isAProxy();
  return eOk;
}

// TODO:
// realize

/*ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsTransactionResident, bool);

OdResult OdDbObjectIsTransactionResidentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isTransactionResident();
  return eOk;
}*/

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsReallyClosing, bool);

OdResult OdDbObjectIsReallyClosingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isReallyClosing();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsObjectIdsInFlux, bool);

OdResult OdDbObjectIsObjectIdsInFluxProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isOdDbObjectIdsInFlux();
  return eOk;
}

//LNK2019
//ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, Handle, OdDbHandle);

//OdResult OdDbObjectHandleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
//{
//  OdDbObjectPtr pObj = OdDbObject::cast(pO);
//  if (pObj.isNull())
//    return eNotApplicable;
//  value = pObj->handle();
//  return eOk;
//}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, OwnerId, OdDbObjectId);

OdResult OdDbObjectOwnerIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->ownerId();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, IsPersistent, bool);

OdResult OdDbObjectIsPersistentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->isPersistent();
  return eOk;
}

ODRX_DECLARE_RX_PROPERTY_RO(OdDbObject, MergeStyle, OdDb::DuplicateRecordCloning);

OdResult OdDbObjectMergeStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbObjectPtr pObj = OdDbObject::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = pObj->mergeStyle();
  return eOk;
}

struct OdDbObjectAnnotativeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbObjectAnnotativeProperty>::createObject();
    ((OdDbObjectAnnotativeProperty*)res.get())->init(L"Annotative", &OdRxValueType::Desc<bool>::value(), owner);
    //((OdDbObjectAnnotativeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Annotative"));
    ((OdDbObjectAnnotativeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies whether or not the dimension is annotative"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"AnnotationScaling"))
      OdRxCategory::createObject(L"AnnotationScaling", OdRxCategory::rootCategory());
    ((OdDbObjectAnnotativeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"AnnotationScaling", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE
  {
    OdDbObjectPtr pObj = OdDbObject::cast(pO);
    OdDbAnnotativeObjectPEPtr a = OdDbAnnotativeObjectPE::cast(pO);
    if (pObj.isNull() || a.isNull())
      return eNotApplicable;
    value = a->annotative(pObj);
    return eOk;
  }
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE
  {
    OdDbObjectPtr pObj = OdDbObject::cast(pO);
    OdDbAnnotativeObjectPEPtr a = OdDbAnnotativeObjectPE::cast(pO);
    if (pObj.isNull() || a.isNull())
      return eNotApplicable;
    a->setAnnotative(pObj, *rxvalue_cast<bool>(&value));
    return eOk;
  }
};

// example of a collection property
struct OdDbSymbolTableItems : OdRxCollectionProperty
{
  struct I : OdRxValueIterator
  {
    OdDbSymbolTableIteratorPtr it;
    virtual bool done() { return it->done(); }
    virtual bool next() 
    {
      if (it->done())
        return false;
      it->step();
      return true;
    }
    virtual OdRxValue current() const
    {
      return it->getRecordId();
    }
  };
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE
  {
    OdRxValueIteratorPtr res;
    OdDbSymbolTablePtr t = OdDbSymbolTable::cast(pO);
    if (!t.isNull())
    {
      res = OdRxObjectImpl<I>::createObject();
      ((I*)res.get())->it = t->newIterator();
    }
    return res;
  }
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE
  {
    OdDbSymbolTablePtr t = OdDbSymbolTable::cast(pO);
    if (t.isNull())
      return eNotApplicable;
    int N = 0;
    for (OdDbSymbolTableIteratorPtr it = t->newIterator(); !it->done(); it->step())
      ++N;
    count = N;
    return eOk;
  }
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSymbolTableItems>::createObject();
    ((OdDbSymbolTableItems*)res.get())->init(L"Items", &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    return res;
  }
};

ODRX_DECLARE_RX_PROPERTY_RO(OdDbCurve, MergeStyle, OdDb::DuplicateRecordCloning);

// example of a property with children
struct OdDbCurveStartPointProperty : OdRxProperty
{ 
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbCurveStartPointProperty>::createObject();
    ((OdDbCurveStartPointProperty*)res.get())->init(L"StartPoint", &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    // skip localization dependent attributes
    res->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    res->attributes().add(OdRxCOMAttribute::createObject(L"Position"));
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    // as far as I understand, subproperties are generated if value type has children and OdRxTypePromotionAttribute
    // if that is true, generic version of this code may be moved to OdRxProperty::subChildren or even straight to children()
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE
  {
    OdDbCurvePtr pObj = OdDbCurve::cast(pO);
    if (pObj.isNull())
      return eNotApplicable;
    OdGePoint3d p;
    OdResult res = pObj->getStartPoint(p);
    if (res == eOk)
      value = p;
    return res;
  }
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

struct OdDbLineNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLineNormalProperty>::createObject();
    ((OdDbLineNormalProperty*)res.get())->init(L"Normal", &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE
  {
    OdDbLinePtr pObj = OdDbLine::cast(pO);
    if (pObj.isNull())
      return eNotApplicable;
    OdGeVector3d p;
    p = pObj->normal();
    value = p;
    return eOk;
  }
  //virtual OdResult OdDbObjectAnnotativeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE

  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE
  {
    OdDbLinePtr pObj = OdDbLine::cast(pO);
    if (pObj.isNull())
      return eNotApplicable;
    pObj->setNormal(*rxvalue_cast<OdGeVector3d>(&value));
    return eOk;
  }
};

FIRSTDLL_EXPORT void odrxSetMemberConstructor(OdRxClass* pClass, OdRxMemberCollectionConstructorPtr pc, void* data = 0);

class OdDbPropertiesModule : public OdRxModule
{
  static OdArray<OdRxMemberPtr> properties;
  static void constructOdDbObjectProperties(OdRxMemberCollectionBuilder& b, void*)
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
    properties.append(OdDbObjectIsCancellingProperty::createObject(b.owner()));
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
    //properties.append(OdDbObjectIsTransactionResidentProperty::createObject(b.owner()));
    //b.add(properties.last());
    properties.append(OdDbObjectIsReallyClosingProperty::createObject(b.owner()));
    b.add(properties.last());
    properties.append(OdDbObjectIsObjectIdsInFluxProperty::createObject(b.owner()));
    b.add(properties.last());
    //properties.append(OdDbObjectHandleProperty::createObject(b.owner()));
    //b.add(properties.last());
    properties.append(OdDbObjectOwnerIdProperty::createObject(b.owner()));
    b.add(properties.last());
    properties.append(OdDbObjectIsPersistentProperty::createObject(b.owner()));
    b.add(properties.last());
    properties.append(OdDbObjectMergeStyleProperty::createObject(b.owner()));
    b.add(properties.last());
    properties.append(OdDbObjectAnnotativeProperty::createObject(b.owner()));
    b.add(properties.last());
  }
  static void constructOdDbSymbolTableProperties(OdRxMemberCollectionBuilder& b, void*)
  {
    properties.append(OdDbSymbolTableItems::createObject(b.owner()));
    b.add(properties.last());
  }
  static void constructOdDbCurveProperties(OdRxMemberCollectionBuilder& b, void*)
  {
    properties.append(OdDbCurveStartPointProperty::createObject(b.owner()));
    b.add(properties.last());
  }
  static void constructOdDbLineProperties(OdRxMemberCollectionBuilder& b, void*)
  {
    properties.append(OdDbLineNormalProperty::createObject(b.owner()));
    b.add(properties.last());
  }

public:
  void initApp()
  {
    ::odrxDynamicLinker()->loadModule(L"RxProperties");
    ::odrxSetMemberConstructor(OdDbObject::desc(), constructOdDbObjectProperties);
    ::odrxSetMemberConstructor(OdDbSymbolTable::desc(), constructOdDbSymbolTableProperties);
    ::odrxSetMemberConstructor(OdDbCurve::desc(), constructOdDbCurveProperties);
    ::odrxSetMemberConstructor(OdDbLine::desc(), constructOdDbLineProperties);
  }
  void uninitApp()
  {
    for (unsigned i = 0; i < properties.size(); ++i)
      OdRxMember::deleteMember(properties[i]);
    properties.clear();
  }
};
OdArray<OdRxMemberPtr> OdDbPropertiesModule::properties;

ODRX_DEFINE_DYNAMIC_MODULE(OdDbPropertiesModule);
DISABLE_THREAD_LIBRARY_CALLS()
