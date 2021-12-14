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
#include "DbBlockReference.h"
#include "DbValueTypes.h"
#include "DbBlockTableRecord.h"


class FIRSTDLL_EXPORT OdRxBlockTableRecordHierarchyLevelAttribute : public OdRxHierarchyLevelAttribute
{
public:

  static OdRxAttributePtr createObject()
  {
    OdRxAttributePtr res = OdRxObjectImpl<OdRxBlockTableRecordHierarchyLevelAttribute>::createObject();
    return res;
  }

  virtual OdString value(OdRxValue& value) ODRX_OVERRIDE 
  {
    const OdDbObjectId* id = rxvalue_cast<OdDbBlockTableRecordId>(&value);
    if (id)
    {
      OdDbObjectPtr pObj = id->openObject();
      if (!pObj.isNull())
      {
        OdDbBlockTableRecordPtr pBTR = OdDbBlockTableRecord::cast(pObj);
        if (!pBTR.isNull())
        {
          return  OdDb::kXrfNotAnXref == pBTR->xrefStatus() ? OD_T("Block") : OD_T("Database");
        }
      }
    }
    return OD_T("");
  }
};


/* OdDbBlockReferenceODAUniqueIDProperty */

struct OdDbBlockReferenceODAUniqueIDProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockReferenceODAUniqueIDProperty>::createObject();
    ((OdDbBlockReferenceODAUniqueIDProperty*)res.get())->init(__OD_T("ODAUniqueID"), &OdRxValueType::Desc<OdUInt64>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockReferenceODAUniqueIDProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  if (pO == NULL)
    return eNullPtr;

  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotThatKindOfClass;

  OdUInt64 h = (OdUInt64)(pObj->objectId().getHandle());
  value = h;
  return eOk;
}

/* OdDbBlockReferenceNameProperty */

struct OdDbBlockReferenceNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockReferenceNameProperty>::createObject();
    ((OdDbBlockReferenceNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockReferenceNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  if (pO == NULL)
    return eNullPtr;

  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotThatKindOfClass;

  OdString name = pObj->isA()->name();
  if (name.find(L"AcDb") == 0)
    name.deleteChars(0, 4);
  value = name;

  return eOk;
}

/* OdDbBlockReferenceBlockTableRecordProperty */

struct OdDbBlockReferenceBlockTableRecordProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockReferenceBlockTableRecordProperty>::createObject();
    ((OdDbBlockReferenceBlockTableRecordProperty*)res.get())->init(__OD_T("BlockTableRecord"), &OdRxValueType::Desc<OdDbBlockTableRecordId>::value(), owner);
    ((OdDbBlockReferenceBlockTableRecordProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/BlockTableId/Items"));
    ((OdDbBlockReferenceBlockTableRecordProperty*)res.get())->attributes().add(OdRxBlockTableRecordHierarchyLevelAttribute::createObject());
    ((OdDbBlockReferenceBlockTableRecordProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"Name", OdRxValueType::Desc<const OdChar*>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Name"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the original block name"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 15));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    res = OdRxSubProperty::createObject(L"Units", OdRxValueType::Desc<OdString>::value(), this);
    children.append(res);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Block Unit"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies insunits saved with the block"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 40));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsUnits"));
    children.append(OdRxSubProperty::createObject(L"BlockScaling", OdRxValueType::Desc<OdDbBlockTableRecord::BlockScaling>::value(), this));
    children.append(OdRxSubProperty::createObject(L"PathName", OdRxValueType::Desc<const OdChar*>::value(), this));
    res = OdRxSubProperty::createObject(L"UnitFactor", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Unit factor"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the conversion factor between block units and drawing units"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 41));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsUnitsFactor"));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockReferenceBlockTableRecordProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbBlockTableRecordId>(pObj->blockTableRecord());
  return eOk;
}

OdResult OdDbBlockReferenceBlockTableRecordProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbBlockTableRecordId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockTableRecord(val);
  return eOk;
}

/* OdDbBlockReferenceRotationProperty */

struct OdDbBlockReferenceRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockReferenceRotationProperty>::createObject();
    ((OdDbBlockReferenceRotationProperty*)res.get())->init(__OD_T("Rotation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbBlockReferenceRotationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbBlockReferenceRotationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotation"));
    ((OdDbBlockReferenceRotationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rotation angle of the block"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbBlockReferenceRotationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 20));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockReferenceRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->rotation());
  return eOk;
}

OdResult OdDbBlockReferenceRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRotation(val);
  return eOk;
}

/* OdDbBlockReferenceNormalProperty */

struct OdDbBlockReferenceNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockReferenceNormalProperty>::createObject();
    ((OdDbBlockReferenceNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbBlockReferenceNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbBlockReferenceNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDbBlockReferenceBlockTransformProperty */

struct OdDbBlockReferenceBlockTransformProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockReferenceBlockTransformProperty>::createObject();
    ((OdDbBlockReferenceBlockTransformProperty*)res.get())->init(__OD_T("BlockTransform"), &OdRxValueType::Desc<OdGeMatrix3d>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockReferenceBlockTransformProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeMatrix3d>(pObj->blockTransform());
  return eOk;
}

OdResult OdDbBlockReferenceBlockTransformProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeMatrix3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setBlockTransform(val);
  return rc;
}

/* OdDbBlockReferenceScaleFactorsProperty */

struct OdDbBlockReferenceScaleFactorsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockReferenceScaleFactorsProperty>::createObject();
    ((OdDbBlockReferenceScaleFactorsProperty*)res.get())->init(__OD_T("ScaleFactors"), &OdRxValueType::Desc<OdGeScale3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"X", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Scale X"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the effective XScale factor of the block"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"XEffectiveScaleFactor"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    res = OdRxSubProperty::createObject(L"Y", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Scale Y"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the effective Yscale factor of the block"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"YEffectiveScaleFactor"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    res = OdRxSubProperty::createObject(L"Z", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Scale Z"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the effective ZScale factor of the block"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ZEffectiveScaleFactor"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 3));
    res = OdRxSubProperty::createObject(L"UniformScale", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Scale"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbBlockReferenceScaleFactorsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeScale3d>(pObj->scaleFactors());
  return eOk;
}

OdResult OdDbBlockReferenceScaleFactorsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeScale3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setScaleFactors(val);
  return eOk;
}

/* OdDbBlockReferencePositionProperty */

struct OdDbBlockReferencePositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockReferencePositionProperty>::createObject();
    ((OdDbBlockReferencePositionProperty*)res.get())->init(__OD_T("Position"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbBlockReferencePositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Insertion point X;Insertion point Y;Insertion point Z"));
    ((OdDbBlockReferencePositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for insertion point of the block or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbBlockReferencePositionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDbBlockReferencePositionProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsertionPoint"));
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

OdResult OdDbBlockReferencePositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

OdResult OdDbBlockReferencePositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPosition(val);
  return eOk;
}

/* OdDbBlockReferenceTreatAsBlockRefForExplodeProperty */

struct OdDbBlockReferenceTreatAsBlockRefForExplodeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbBlockReferenceTreatAsBlockRefForExplodeProperty>::createObject();
    ((OdDbBlockReferenceTreatAsBlockRefForExplodeProperty*)res.get())->init(__OD_T("TreatAsBlockRefForExplode"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbBlockReferenceTreatAsBlockRefForExplodeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbBlockReferencePtr pObj = OdDbBlockReference::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->treatAsOdDbBlockRefForExplode());
  return eOk;
}

void createOdDbBlockReferenceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbBlockReferenceODAUniqueIDProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockReferenceNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockReferenceBlockTableRecordProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockReferenceRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockReferenceNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockReferenceBlockTransformProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockReferenceScaleFactorsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockReferencePositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbBlockReferenceTreatAsBlockRefForExplodeProperty::createObject(b.owner()));
  b.add(properties.last());
}
