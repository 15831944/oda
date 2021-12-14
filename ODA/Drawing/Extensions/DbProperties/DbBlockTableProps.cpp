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
#include "DbBlockTable.h"
#include "DbValueTypes.h"

/* OdDbBlockTableODAUniqueIDProperty */

struct OdDbBlockTableODAUniqueIDProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableODAUniqueIDProperty>::createObject();
    ((OdDbBlockTableODAUniqueIDProperty*)res.get())->init(__OD_T("ODAUniqueID"), &OdRxValueType::Desc<OdUInt64>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableODAUniqueIDProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  if (pO == NULL)
    return eNullPtr;

  OdDbBlockTablePtr pObj = OdDbBlockTable::cast(pO);
  if (pObj.isNull())
    return eNotThatKindOfClass;

  OdUInt64 h = (OdUInt64)(pObj->objectId().getHandle());
  value = h;
  return eOk;
}

/* OdDbBlockTableNameProperty */

struct OdDbBlockTableNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableNameProperty>::createObject();
    ((OdDbBlockTableNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  if (pO == NULL)
    return eNullPtr;

  OdDbBlockTablePtr pObj = OdDbBlockTable::cast(pO);
  if (pObj.isNull())
    return eNotThatKindOfClass;

  OdString name = pObj->isA()->name();
  if (name.find(L"AcDb") == 0)
    name.deleteChars(0, 4);
  value = name;

  return eOk;
}

class FIRSTDLL_EXPORT OdRxBlockTableItemsHierarchyLevelAttribute : public OdRxHierarchyLevelAttribute
{
public:

  static OdRxAttributePtr createObject()
  {
    OdRxAttributePtr res = OdRxObjectImpl<OdRxBlockTableItemsHierarchyLevelAttribute>::createObject();
    return res;
  }

  virtual OdString value(OdRxValue& value) ODRX_OVERRIDE
  {
    const OdDbObjectId* id = rxvalue_cast<OdDbObjectId>(&value);
    if (id)
    {
      OdDbObjectPtr pObj = id->openObject();
      if (!pObj.isNull())
      {
        OdDbBlockTableRecordPtr pBTR = OdDbBlockTableRecord::cast(pObj);
        if (!pBTR.isNull())
        {
          return  OdDb::kXrfNotAnXref == pBTR->xrefStatus() ? OD_T("Block") : OD_T("UNUSED_NODE");
        }
      }
    }
    return OD_T("");
  }
};

// OdDbBlockTableItems
struct OdDbBlockTableItems : OdRxCollectionProperty
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
    OdDbBlockTablePtr t = OdDbBlockTable::cast(pO);
    if (!t.isNull())
    {
      res = OdRxObjectImpl<I>::createObject();
      ((I*)res.get())->it = t->newIterator();
    }
    return res;
  }
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE
  {
    OdDbBlockTablePtr t = OdDbBlockTable::cast(pO);
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
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableItems>::createObject();
    ((OdDbBlockTableItems*)res.get())->init(__OD_T("Items"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbBlockTableItems*)res.get())->attributes().add(OdRxBlockTableItemsHierarchyLevelAttribute::createObject());
    ((OdDbBlockTableItems*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
};

void createOdDbBlockTableProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbBlockTableODAUniqueIDProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableItems::createObject(b.owner()));
  b.add(properties.last());
}
