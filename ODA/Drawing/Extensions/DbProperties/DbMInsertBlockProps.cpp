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
#include "DbMInsertBlock.h"
#include "DbValueTypes.h"

/* OdDbMInsertBlockInsertPointProperty */

struct OdDbMInsertBlockInsertPointProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMInsertBlockInsertPointProperty>::createObject();
    ((OdDbMInsertBlockInsertPointProperty*)res.get())->init(__OD_T("InsertPoint"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMInsertBlockInsertPointProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Insertion point X;Insertion point Y;Insertion point Z"));
    ((OdDbMInsertBlockInsertPointProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for insertion point of the block or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbMInsertBlockInsertPointProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 0));
    ((OdDbMInsertBlockInsertPointProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsertionPoint"));
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

OdResult OdDbMInsertBlockInsertPointProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

OdResult OdDbMInsertBlockInsertPointProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setPosition(val);
  return eOk;
}

/* OdDbMInsertBlockRowsProperty */

struct OdDbMInsertBlockRowsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMInsertBlockRowsProperty>::createObject();
    ((OdDbMInsertBlockRowsProperty*)res.get())->init(__OD_T("Rows"), &OdRxValueType::Desc<short>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMInsertBlockRowsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rows"));
    ((OdDbMInsertBlockRowsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines the number of columns in the block array"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbMInsertBlockRowsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 32));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMInsertBlockRowsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->rows());
  return eOk;
}

OdResult OdDbMInsertBlockRowsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRows(val);
  return eOk;
}

/* OdDbMInsertBlockColumnsProperty */

struct OdDbMInsertBlockColumnsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMInsertBlockColumnsProperty>::createObject();
    ((OdDbMInsertBlockColumnsProperty*)res.get())->init(__OD_T("Columns"), &OdRxValueType::Desc<short>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMInsertBlockColumnsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Columns"));
    ((OdDbMInsertBlockColumnsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of columns in the block array"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbMInsertBlockColumnsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 30));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMInsertBlockColumnsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->columns());
  return eOk;
}

OdResult OdDbMInsertBlockColumnsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColumns(val);
  return eOk;
}

/* OdDbMInsertBlockRowSpacingProperty */

struct OdDbMInsertBlockRowSpacingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMInsertBlockRowSpacingProperty>::createObject();
    ((OdDbMInsertBlockRowSpacingProperty*)res.get())->init(__OD_T("RowSpacing"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMInsertBlockRowSpacingProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMInsertBlockRowSpacingProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Row spacing"));
    ((OdDbMInsertBlockRowSpacingProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the row spacing in the block array"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbMInsertBlockRowSpacingProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 33));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMInsertBlockRowSpacingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->rowSpacing());
  return eOk;
}

OdResult OdDbMInsertBlockRowSpacingProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRowSpacing(val);
  return eOk;
}

/* OdDbMInsertBlockColumnSpacingProperty */

struct OdDbMInsertBlockColumnSpacingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMInsertBlockColumnSpacingProperty>::createObject();
    ((OdDbMInsertBlockColumnSpacingProperty*)res.get())->init(__OD_T("ColumnSpacing"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMInsertBlockColumnSpacingProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMInsertBlockColumnSpacingProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Column spacing"));
    ((OdDbMInsertBlockColumnSpacingProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the column spacing in the block array"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbMInsertBlockColumnSpacingProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 31));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMInsertBlockColumnSpacingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->columnSpacing());
  return eOk;
}

OdResult OdDbMInsertBlockColumnSpacingProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMInsertBlockPtr pObj = OdDbMInsertBlock::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColumnSpacing(val);
  return eOk;
}

void createOdDbMInsertBlockProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbMInsertBlockInsertPointProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMInsertBlockRowsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMInsertBlockColumnsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMInsertBlockRowSpacingProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMInsertBlockColumnSpacingProperty::createObject(b.owner()));
  b.add(properties.last());
}
