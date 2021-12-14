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
#include "DbDictionary.h"
#include "DbValueTypes.h"
#include "DbLayout.h"

/* OdRxDictItemsHierarchyLevelAttribute */

class FIRSTDLL_EXPORT OdRxDictItemsHierarchyLevelAttribute : public OdRxHierarchyLevelAttribute
{
public:

  static OdRxAttributePtr createObject()
  {
    OdRxAttributePtr res = OdRxObjectImpl<OdRxDictItemsHierarchyLevelAttribute>::createObject();
    return res;
  }

  virtual OdString value(OdRxValue& value) ODRX_OVERRIDE {
    OdDbObjectId *id = rxvalue_cast<OdDbObjectId>(&value);
    if (id)
    {
      OdDbObjectPtr ptr = id->openObject();
      if (!ptr.isNull())
        return (ptr->isKindOf(OdDbLayout::desc())) ? OD_T("Model") : OD_T("");
    }
    return OD_T("");
  }
};

/* OdDbDictionaryODAUniqueIDProperty */

struct OdDbDictionaryODAUniqueIDProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDictionaryODAUniqueIDProperty>::createObject();
    ((OdDbDictionaryODAUniqueIDProperty*)res.get())->init(__OD_T("ODAUniqueID"), &OdRxValueType::Desc<OdUInt64>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbDictionaryODAUniqueIDProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  if (pO == NULL)
    return eNullPtr;

  OdDbDictionaryPtr pObj = OdDbDictionary::cast(pO);
  if (pObj.isNull())
    return eNotThatKindOfClass;

  OdUInt64 h = (OdUInt64)(pObj->objectId().getHandle());
  value = h;
  return eOk;
}

/* OdDbDictionaryTreatElementsAsHardProperty */

struct OdDbDictionaryTreatElementsAsHardProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDictionaryTreatElementsAsHardProperty>::createObject();
    ((OdDbDictionaryTreatElementsAsHardProperty*)res.get())->init(__OD_T("TreatElementsAsHard"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbDictionaryTreatElementsAsHardProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbDictionaryPtr pObj = OdDbDictionary::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isTreatElementsAsHard());
  return eOk;
}

OdResult OdDbDictionaryTreatElementsAsHardProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbDictionaryPtr pObj = OdDbDictionary::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTreatElementsAsHard(val);
  return eOk;
}

// OdDbDictionaryItems
struct OdDbDictionaryItems : OdRxDictionaryProperty
{
  struct I : OdRxValueIterator
  {
    OdDbDictionaryIteratorPtr it;
    virtual bool done() { return it->done(); }
    virtual bool next()
    {
      if (it->done())
        return false;
      it->next();
      return true;
    }
    virtual OdRxValue current() const
    {
      return it->objectId();
    }
  };
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE
  {
    OdRxValueIteratorPtr res;
    OdDbDictionaryPtr t = OdDbDictionary::cast(pO);
    if (!t.isNull())
    {
      res = OdRxObjectImpl<I>::createObject();
      ((I*)res.get())->it = t->newIterator();
    }
    return res;
  }
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE
  {
    OdDbDictionaryPtr t = OdDbDictionary::cast(pO);
    if (t.isNull())
     return eNotApplicable;
    int N = 0;
    for (OdDbDictionaryIteratorPtr it = t->newIterator(); !it->done(); it->next())
      ++N;
    count = N;
    return eOk;
  }
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbDictionaryItems>::createObject();
    ((OdDbDictionaryItems*)res.get())->init(__OD_T("Items"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbDictionaryItems*)res.get())->attributes().add(OdRxDictItemsHierarchyLevelAttribute::createObject());
    ((OdDbDictionaryItems*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"LayoutName"));
    return res;
  }
};

void createOdDbDictionaryProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbDictionaryODAUniqueIDProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDictionaryTreatElementsAsHardProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbDictionaryItems::createObject(b.owner()));
  b.add(properties.last());
}
