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
#include "DbBlockTableRecord.h"
#include "DbValueTypes.h"
#include "OdUtilAds.h"
#include "DbAnnotativeObjectPE.h"

/* OdRxItemsHierarchyLevelAttribute */

class FIRSTDLL_EXPORT OdRxItemsHierarchyLevelAttribute : public OdRxHierarchyLevelAttribute
{
public:

  static OdRxAttributePtr createObject()
  {
    OdRxAttributePtr res = OdRxObjectImpl<OdRxItemsHierarchyLevelAttribute>::createObject();
    return res;
  }

  virtual OdString value(OdRxValue& value) ODRX_OVERRIDE {
    OdDbObjectId *id = rxvalue_cast<OdDbObjectId>(&value);
    if (id)
    {
      OdDbObjectPtr ptr = id->openObject();
      if (!ptr.isNull())
        return (ptr->isKindOf(OdDbBlockReference::desc())) ? OD_T("BlockReference") : OD_T("Entity");
    }
    return OD_T("");
  }
};

/* OdDbBlockTableRecordODAUniqueIDProperty */

struct OdDbBlockTableRecordODAUniqueIDProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordODAUniqueIDProperty>::createObject();
    ((OdDbBlockTableRecordODAUniqueIDProperty*)res.get())->init(__OD_T("ODAUniqueID"), &OdRxValueType::Desc<OdUInt64>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordODAUniqueIDProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  if (pO == NULL)
    return eNullPtr;

  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotThatKindOfClass;

  OdUInt64 h = (OdUInt64)(pObj->objectId().getHandle());
  value = h;
  return eOk;
}

/* OdDbBlockTableRecordIsAnonymousProperty */

struct OdDbBlockTableRecordIsAnonymousProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordIsAnonymousProperty>::createObject();
    ((OdDbBlockTableRecordIsAnonymousProperty*)res.get())->init(__OD_T("IsAnonymous"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordIsAnonymousProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isAnonymous());
  return eOk;
}

/* OdDbBlockTableRecordHasPreviewIconProperty */

struct OdDbBlockTableRecordHasPreviewIconProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordHasPreviewIconProperty>::createObject();
    ((OdDbBlockTableRecordHasPreviewIconProperty*)res.get())->init(__OD_T("HasPreviewIcon"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordHasPreviewIconProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasPreviewIcon());
  return eOk;
}

/* OdDbBlockTableRecordCommentsProperty */

struct OdDbBlockTableRecordCommentsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordCommentsProperty>::createObject();
    ((OdDbBlockTableRecordCommentsProperty*)res.get())->init(__OD_T("Comments"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockTableRecordCommentsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->comments());
  return eOk;
}

OdResult OdDbBlockTableRecordCommentsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setComments(val);
  return eOk;
}

/* OdDbBlockTableRecordIsFromExternalReferenceProperty */

struct OdDbBlockTableRecordIsFromExternalReferenceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordIsFromExternalReferenceProperty>::createObject();
    ((OdDbBlockTableRecordIsFromExternalReferenceProperty*)res.get())->init(__OD_T("IsFromExternalReference"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordIsFromExternalReferenceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isFromExternalReference());
  return eOk;
}

/* OdDbBlockTableRecordHasAttributeDefinitionsProperty */

struct OdDbBlockTableRecordHasAttributeDefinitionsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordHasAttributeDefinitionsProperty>::createObject();
    ((OdDbBlockTableRecordHasAttributeDefinitionsProperty*)res.get())->init(__OD_T("HasAttributeDefinitions"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordHasAttributeDefinitionsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasAttributeDefinitions());
  return eOk;
}

/* OdDbBlockTableRecordOriginProperty */

struct OdDbBlockTableRecordOriginProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordOriginProperty>::createObject();
    ((OdDbBlockTableRecordOriginProperty*)res.get())->init(__OD_T("Origin"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    children.append(OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this));
    children.append(OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockTableRecordOriginProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->origin());
  return eOk;
}

OdResult OdDbBlockTableRecordOriginProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setOrigin(val);
  return eOk;
}

/* OdDbBlockTableRecordPathNameProperty */

struct OdDbBlockTableRecordPathNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordPathNameProperty>::createObject();
    ((OdDbBlockTableRecordPathNameProperty*)res.get())->init(__OD_T("PathName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockTableRecordPathNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->pathName());
  return eOk;
}

OdResult OdDbBlockTableRecordPathNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPathName(val);
  return eOk;
}

/* OdDbBlockTableRecordIsLayoutProperty */

struct OdDbBlockTableRecordIsLayoutProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordIsLayoutProperty>::createObject();
    ((OdDbBlockTableRecordIsLayoutProperty*)res.get())->init(__OD_T("IsLayout"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordIsLayoutProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isLayout());
  return eOk;
}

/* OdDbBlockTableRecordBlockScalingProperty */

struct OdDbBlockTableRecordBlockScalingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordBlockScalingProperty>::createObject();
    ((OdDbBlockTableRecordBlockScalingProperty*)res.get())->init(__OD_T("BlockScaling"), &OdRxValueType::Desc<OdDbBlockTableRecord::BlockScaling>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockTableRecordBlockScalingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbBlockTableRecord::BlockScaling>(pObj->blockScaling());
  return eOk;
}

OdResult OdDbBlockTableRecordBlockScalingProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbBlockTableRecord::BlockScaling val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockScaling(val);
  return eOk;
}

/* OdDbBlockTableRecordExplodableProperty */

struct OdDbBlockTableRecordExplodableProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordExplodableProperty>::createObject();
    ((OdDbBlockTableRecordExplodableProperty*)res.get())->init(__OD_T("Explodable"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockTableRecordExplodableProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->explodable());
  return eOk;
}

OdResult OdDbBlockTableRecordExplodableProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setExplodable(val);
  return eOk;
}

/* OdDbBlockTableRecordIsFromOverlayReferenceProperty */

struct OdDbBlockTableRecordIsFromOverlayReferenceProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordIsFromOverlayReferenceProperty>::createObject();
    ((OdDbBlockTableRecordIsFromOverlayReferenceProperty*)res.get())->init(__OD_T("IsFromOverlayReference"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordIsFromOverlayReferenceProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isFromOverlayReference());
  return eOk;
}

/* OdDbBlockTableRecordUnitsProperty */

struct OdDbBlockTableRecordUnitsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordUnitsProperty>::createObject();
    ((OdDbBlockTableRecordUnitsProperty*)res.get())->init(__OD_T("Units"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbBlockTableRecordUnitsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Block Unit"));
    ((OdDbBlockTableRecordUnitsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies insunits saved with the block"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbBlockTableRecordUnitsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 40));
    ((OdDbBlockTableRecordUnitsProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsUnits"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordUnitsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDb::UnitsValue unitsValue = pObj->blockInsertUnits(); value = OdString(oddbGetUnitsName(unitsValue));
  return eOk;
}

/* OdDbBlockTableRecordUnitFactorProperty */

struct OdDbBlockTableRecordUnitFactorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordUnitFactorProperty>::createObject();
    ((OdDbBlockTableRecordUnitFactorProperty*)res.get())->init(__OD_T("UnitFactor"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordUnitFactorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdDb::UnitsValue unitsValue = pObj->blockInsertUnits(); value = oddbGetUnitsConversion(unitsValue, OdDb::kUnitsUndefined);
  return eOk;
}

/* OdDbBlockTableRecordLayoutIdProperty */

struct OdDbBlockTableRecordLayoutIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordLayoutIdProperty>::createObject();
    ((OdDbBlockTableRecordLayoutIdProperty*)res.get())->init(__OD_T("LayoutId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbBlockTableRecordLayoutIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockTableRecordLayoutIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->getLayoutId());
  return eOk;
}

OdResult OdDbBlockTableRecordLayoutIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLayoutId(val);
  return eOk;
}

/* OdDbBlockTableRecordIsUnloadedProperty */

struct OdDbBlockTableRecordIsUnloadedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordIsUnloadedProperty>::createObject();
    ((OdDbBlockTableRecordIsUnloadedProperty*)res.get())->init(__OD_T("IsUnloaded"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockTableRecordIsUnloadedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isUnloaded());
  return eOk;
}

OdResult OdDbBlockTableRecordIsUnloadedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsUnloaded(val);
  return eOk;
}

/* OdDbBlockTableRecordXrefStatusProperty */

struct OdDbBlockTableRecordXrefStatusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordXrefStatusProperty>::createObject();
    ((OdDbBlockTableRecordXrefStatusProperty*)res.get())->init(__OD_T("XrefStatus"), &OdRxValueType::Desc<OdDb::XrefStatus>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordXrefStatusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockTableRecordPtr pObj = OdDbBlockTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::XrefStatus>(pObj->xrefStatus());
  return eOk;
}

// OdDbBlockTableRecordItems
struct OdDbBlockTableRecordItems : OdRxCollectionProperty
{
  struct I : OdRxValueIterator
  {
    OdDbObjectIteratorPtr it;
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
      return it->objectId();
    }
  };
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE
  {
    OdRxValueIteratorPtr res;
    OdDbBlockTableRecordPtr t = OdDbBlockTableRecord::cast(pO);
    if (!t.isNull())
    {
      res = OdRxObjectImpl<I>::createObject();
      ((I*)res.get())->it = t->newIterator();
    }
    return res;
  }
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE
  {
    OdDbBlockTableRecordPtr t = OdDbBlockTableRecord::cast(pO);
    if (t.isNull())
     return eNotApplicable;
    int N = 0;
    for (OdDbObjectIteratorPtr it = t->newIterator(); !it->done(); it->step())
      ++N;
    count = N;
    return eOk;
  }
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordItems>::createObject();
    ((OdDbBlockTableRecordItems*)res.get())->init(__OD_T("Items"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbBlockTableRecordItems*)res.get())->attributes().add(OdRxItemsHierarchyLevelAttribute::createObject());
    ((OdDbBlockTableRecordItems*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
};

/* OdDbBlockTableRecordIsAnnotativeProperty */

struct OdDbBlockTableRecordIsAnnotativeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockTableRecordIsAnnotativeProperty>::createObject();
    ((OdDbBlockTableRecordIsAnnotativeProperty*)res.get())->init(__OD_T("IsAnnotative"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockTableRecordIsAnnotativeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
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

void createOdDbBlockTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbBlockTableRecordODAUniqueIDProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordIsAnonymousProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordHasPreviewIconProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordCommentsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordIsFromExternalReferenceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordHasAttributeDefinitionsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordOriginProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordPathNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordIsLayoutProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordBlockScalingProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordExplodableProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordIsFromOverlayReferenceProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordUnitsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordUnitFactorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordLayoutIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordIsUnloadedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordXrefStatusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordItems::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockTableRecordIsAnnotativeProperty::createObject(b.owner()));
  b.add(properties.last());
}
