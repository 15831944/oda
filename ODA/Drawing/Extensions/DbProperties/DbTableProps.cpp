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
#include "DbTable.h"
#include "DbValueTypes.h"

/* OdDbTableBreakFlowDirectionProperty */

struct OdDbTableBreakFlowDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableBreakFlowDirectionProperty>::createObject();
    ((OdDbTableBreakFlowDirectionProperty*)res.get())->init(__OD_T("BreakFlowDirection"), &OdRxValueType::Desc<OdDb::TableBreakFlowDirection>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableBreakFlowDirectionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Direction"));
    ((OdDbTableBreakFlowDirectionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines the direction that parts of the table flow."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table Breaks"))
      OdRxCategory::createObject(L"Table Breaks", OdRxCategory::rootCategory());
    ((OdDbTableBreakFlowDirectionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table Breaks", 5));
    ((OdDbTableBreakFlowDirectionProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TableBreakFlowDirection"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableBreakFlowDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::TableBreakFlowDirection>(pObj->breakFlowDirection());
  return eOk;
}

OdResult OdDbTableBreakFlowDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::TableBreakFlowDirection val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBreakFlowDirection(val);
  return eOk;
}

/* OdDbTableBreakOptionsProperty */

struct OdDbTableBreakOptionsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableBreakOptionsProperty>::createObject();
    ((OdDbTableBreakOptionsProperty*)res.get())->init(__OD_T("BreakOptions"), &OdRxValueType::Desc<OdDb::TableBreakOption>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableBreakOptionsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  value = static_cast<OdDb::TableBreakOption>(pObj->breakOption());
  return eOk;
}

OdResult OdDbTableBreakOptionsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::TableBreakOption val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_NAME
  pObj->setBreakOption(val);
  return eOk;
}

/* OdDbTableBreakEnabledProperty */

struct OdDbTableBreakEnabledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableBreakEnabledProperty>::createObject();
    ((OdDbTableBreakEnabledProperty*)res.get())->init(__OD_T("BreakEnabled"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableBreakEnabledProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Enabled"));
    ((OdDbTableBreakEnabledProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether table breaking is in use. When enabled, the table will break automatically at the current specified height."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table Breaks"))
      OdRxCategory::createObject(L"Table Breaks", OdRxCategory::rootCategory());
    ((OdDbTableBreakEnabledProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table Breaks", 1));
    ((OdDbTableBreakEnabledProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"BreaksEnabled"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableBreakEnabledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isBreakEnabled());
  return eOk;
}

OdResult OdDbTableBreakEnabledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->enableBreak(val);
  return eOk;
}

/* OdDbTableMinimumTableWidthProperty */

struct OdDbTableMinimumTableWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableMinimumTableWidthProperty>::createObject();
    ((OdDbTableMinimumTableWidthProperty*)res.get())->init(__OD_T("MinimumTableWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTableMinimumTableWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    ((OdDbTableMinimumTableWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Gets the minimum width for the table."));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbTableMinimumTableWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->minimumTableWidth());
  return eOk;
}

/* OdDbTableMinimumTableHeightProperty */

struct OdDbTableMinimumTableHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableMinimumTableHeightProperty>::createObject();
    ((OdDbTableMinimumTableHeightProperty*)res.get())->init(__OD_T("MinimumTableHeight"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTableMinimumTableHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    ((OdDbTableMinimumTableHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Gets the minimum height for the table."));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbTableMinimumTableHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->minimumTableHeight());
  return eOk;
}

/* OdDbTableHasSubSelectionProperty */

struct OdDbTableHasSubSelectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableHasSubSelectionProperty>::createObject();
    ((OdDbTableHasSubSelectionProperty*)res.get())->init(__OD_T("HasSubSelection"), &OdRxValueType::Desc<bool>::value(), owner);
    ((OdDbTableHasSubSelectionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"HasSubSelection."));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbTableHasSubSelectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasSubSelection());
  return eOk;
}

/* OdDbTableHeightProperty */

struct OdDbTableHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableHeightProperty>::createObject();
    ((OdDbTableHeightProperty*)res.get())->init(__OD_T("Height"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTableHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Table height"));
    ((OdDbTableHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the Table height"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table"))
      OdRxCategory::createObject(L"Table", OdRxCategory::rootCategory());
    ((OdDbTableHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table", 25));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->height());
  return eOk;
}

OdResult OdDbTableHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHeight(val);
  return eOk;
}

/* OdDbTableNumRowsProperty */

struct OdDbTableNumRowsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableNumRowsProperty>::createObject();
    ((OdDbTableNumRowsProperty*)res.get())->init(__OD_T("NumRows"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableNumRowsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rows"));
    ((OdDbTableNumRowsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rows in the Table"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table"))
      OdRxCategory::createObject(L"Table", OdRxCategory::rootCategory());
    ((OdDbTableNumRowsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table", 5));
    ((OdDbTableNumRowsProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbTableNumRowsProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Rows"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbTableNumRowsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numRows());
  return eOk;
}

/* OdDbTableNumColumnsProperty */

struct OdDbTableNumColumnsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableNumColumnsProperty>::createObject();
    ((OdDbTableNumColumnsProperty*)res.get())->init(__OD_T("NumColumns"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableNumColumnsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Columns"));
    ((OdDbTableNumColumnsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the columns in the Table"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table"))
      OdRxCategory::createObject(L"Table", OdRxCategory::rootCategory());
    ((OdDbTableNumColumnsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table", 10));
    ((OdDbTableNumColumnsProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbTableNumColumnsProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Columns"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbTableNumColumnsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numColumns());
  return eOk;
}

/* OdDbTableDirectionProperty */

struct OdDbTableDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableDirectionProperty>::createObject();
    ((OdDbTableDirectionProperty*)res.get())->init(__OD_T("Direction"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbTableDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->direction());
  return eOk;
}

OdResult OdDbTableDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDirection(val);
  return eOk;
}

/* OdDbTableWidthProperty */

struct OdDbTableWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableWidthProperty>::createObject();
    ((OdDbTableWidthProperty*)res.get())->init(__OD_T("Width"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTableWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Table width"));
    ((OdDbTableWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the Table width"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table"))
      OdRxCategory::createObject(L"Table", OdRxCategory::rootCategory());
    ((OdDbTableWidthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table", 20));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->width());
  return eOk;
}

OdResult OdDbTableWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWidth(val);
  return eOk;
}

/* OdDbTableCellsProperty */

struct OdDbTableCellsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableCellsProperty>::createObject();
    ((OdDbTableCellsProperty*)res.get())->init(__OD_T("Cells"), &OdRxValueType::Desc<OdCellRange>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbTableCellsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdCellRange range(0, 0, pObj->numRows() - 1, pObj->numColumns() - 1); value = range;
  return eOk;
}

/* OdDbTableFlowDirectionProperty */

struct OdDbTableFlowDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableFlowDirectionProperty>::createObject();
    ((OdDbTableFlowDirectionProperty*)res.get())->init(__OD_T("FlowDirection"), &OdRxValueType::Desc<OdDb::FlowDirection>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableFlowDirectionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Direction"));
    ((OdDbTableFlowDirectionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the Table flow direction"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table"))
      OdRxCategory::createObject(L"Table", OdRxCategory::rootCategory());
    ((OdDbTableFlowDirectionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table", 15));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableFlowDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::FlowDirection>(pObj->flowDirection());
  return eOk;
}

OdResult OdDbTableFlowDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::FlowDirection val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFlowDirection(val);
  return eOk;
}

/* OdDbTableTableStyleProperty */

struct OdDbTableTableStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableTableStyleProperty>::createObject();
    ((OdDbTableTableStyleProperty*)res.get())->init(__OD_T("TableStyle"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbTableTableStyleProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/TableStyleDictionaryId/Items"));
    ((OdDbTableTableStyleProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableTableStyleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Table style"));
    ((OdDbTableTableStyleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the style name of the Table"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table"))
      OdRxCategory::createObject(L"Table", OdRxCategory::rootCategory());
    ((OdDbTableTableStyleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table", 1));
    ((OdDbTableTableStyleProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbTableTableStyleProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"StyleName"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableTableStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->tableStyle());
  return eOk;
}

OdResult OdDbTableTableStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTableStyle(val);
  return eOk;
}

/* OdDbTableSubSelectionProperty */

struct OdDbTableSubSelectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableSubSelectionProperty>::createObject();
    ((OdDbTableSubSelectionProperty*)res.get())->init(__OD_T("SubSelection"), &OdRxValueType::Desc<OdCellRange>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableSubSelectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdCellRange>(pObj->getSubSelection());
  return eOk;
}

OdResult OdDbTableSubSelectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCellRange val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setSubSelection(val);
  return rc;
}

/* OdDbTableRegenerateTableSuppressedProperty */

struct OdDbTableRegenerateTableSuppressedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableRegenerateTableSuppressedProperty>::createObject();
    ((OdDbTableRegenerateTableSuppressedProperty*)res.get())->init(__OD_T("RegenerateTableSuppressed"), &OdRxValueType::Desc<bool>::value(), owner);
    ((OdDbTableRegenerateTableSuppressedProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Enables or disables the regeneration of table block"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableRegenerateTableSuppressedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isRegenerateTableSuppressed());
  return eOk;
}

OdResult OdDbTableRegenerateTableSuppressedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->suppressRegenerateTable(val);
  return eOk;
}

/* OdDbTableRepeatTopLabelsProperty */

struct OdDbTableRepeatTopLabelsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableRepeatTopLabelsProperty>::createObject();
    ((OdDbTableRepeatTopLabelsProperty*)res.get())->init(__OD_T("RepeatTopLabels"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableRepeatTopLabelsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Repeat top labels"));
    ((OdDbTableRepeatTopLabelsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether the first set of label rows will be repeated at the top of each broken table part."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table Breaks"))
      OdRxCategory::createObject(L"Table Breaks", OdRxCategory::rootCategory());
    ((OdDbTableRepeatTopLabelsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table Breaks", 10));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableRepeatTopLabelsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = (pObj->breakOption() & OdDb::kTableBreakRepeatTopLabels) != 0;
  return eOk;
}

OdResult OdDbTableRepeatTopLabelsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setBreakOption(static_cast<OdDb::TableBreakOption>((val)?pObj->breakOption()|OdDb::kTableBreakRepeatTopLabels:pObj->breakOption()&~OdDb::kTableBreakRepeatTopLabels));
  return eOk;
}

/* OdDbTableRepeatBottomLabelsProperty */

struct OdDbTableRepeatBottomLabelsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableRepeatBottomLabelsProperty>::createObject();
    ((OdDbTableRepeatBottomLabelsProperty*)res.get())->init(__OD_T("RepeatBottomLabels"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableRepeatBottomLabelsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Repeat bottom labels"));
    ((OdDbTableRepeatBottomLabelsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether the bottom set of label rows is repeated at the bottom of each broken table part."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table Breaks"))
      OdRxCategory::createObject(L"Table Breaks", OdRxCategory::rootCategory());
    ((OdDbTableRepeatBottomLabelsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table Breaks", 15));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableRepeatBottomLabelsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = (pObj->breakOption() & OdDb::kTableBreakRepeatBottomLabels) != 0;
  return eOk;
}

OdResult OdDbTableRepeatBottomLabelsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setBreakOption(static_cast<OdDb::TableBreakOption>((val)?pObj->breakOption()|OdDb::kTableBreakRepeatBottomLabels:pObj->breakOption()&~OdDb::kTableBreakRepeatBottomLabels));
  return eOk;
}

/* OdDbTableAllowManualPositionsProperty */

struct OdDbTableAllowManualPositionsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableAllowManualPositionsProperty>::createObject();
    ((OdDbTableAllowManualPositionsProperty*)res.get())->init(__OD_T("AllowManualPositions"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableAllowManualPositionsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Manual positions"));
    ((OdDbTableAllowManualPositionsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"When enabled, each table part can be moved outside of the boundary. When not enabled, the table parts are contained within a rectangular boundary."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table Breaks"))
      OdRxCategory::createObject(L"Table Breaks", OdRxCategory::rootCategory());
    ((OdDbTableAllowManualPositionsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table Breaks", 20));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableAllowManualPositionsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = (pObj->breakOption() & OdDb::kTableBreakAllowManualPositions) != 0;
  return eOk;
}

OdResult OdDbTableAllowManualPositionsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setBreakOption(static_cast<OdDb::TableBreakOption>((val)?pObj->breakOption()|OdDb::kTableBreakAllowManualPositions:pObj->breakOption()&~OdDb::kTableBreakAllowManualPositions));
  return eOk;
}

/* OdDbTableAllowManualHeightsProperty */

struct OdDbTableAllowManualHeightsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableAllowManualHeightsProperty>::createObject();
    ((OdDbTableAllowManualHeightsProperty*)res.get())->init(__OD_T("AllowManualHeights"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableAllowManualHeightsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Manual heights"));
    ((OdDbTableAllowManualHeightsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Allows the heights of broken table parts to have their own breaking height. When set to No, all table parts will break at the height of the initial table part."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table Breaks"))
      OdRxCategory::createObject(L"Table Breaks", OdRxCategory::rootCategory());
    ((OdDbTableAllowManualHeightsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table Breaks", 25));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableAllowManualHeightsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = (pObj->breakOption() & OdDb::kTableBreakAllowManualHeights) != 0;
  return eOk;
}

OdResult OdDbTableAllowManualHeightsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setBreakOption(static_cast<OdDb::TableBreakOption>((val)?pObj->breakOption()|OdDb::kTableBreakAllowManualHeights:pObj->breakOption()&~OdDb::kTableBreakAllowManualHeights));
  return eOk;
}

/* OdDbTableTableBreakHeightProperty */

struct OdDbTableTableBreakHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableTableBreakHeightProperty>::createObject();
    ((OdDbTableTableBreakHeightProperty*)res.get())->init(__OD_T("TableBreakHeight"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTableTableBreakHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableTableBreakHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Break height"));
    ((OdDbTableTableBreakHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Sets the breaking height for the initial table part and any other table parts that do not have manual heights set."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table Breaks"))
      OdRxCategory::createObject(L"Table Breaks", OdRxCategory::rootCategory());
    ((OdDbTableTableBreakHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table Breaks", 30));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

//COMMENT: TODO: Needed Sub-table index! current = 0
OdResult OdDbTableTableBreakHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->breakHeight(0);
  return eOk;
}

OdResult OdDbTableTableBreakHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setBreakHeight(0, val);
  return eOk;
}

/* OdDbTableBreakSpacingProperty */

struct OdDbTableBreakSpacingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbTableBreakSpacingProperty>::createObject();
    ((OdDbTableBreakSpacingProperty*)res.get())->init(__OD_T("BreakSpacing"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbTableBreakSpacingProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbTableBreakSpacingProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Spacing"));
    ((OdDbTableBreakSpacingProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Controls the spacing between the broken table parts. Depending on the break direction this will be a horizontal or vertical spacing."));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Table Breaks"))
      OdRxCategory::createObject(L"Table Breaks", OdRxCategory::rootCategory());
    ((OdDbTableBreakSpacingProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Table Breaks", 35));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbTableBreakSpacingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->breakSpacing());
  return eOk;
}

OdResult OdDbTableBreakSpacingProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbTablePtr pObj = OdDbTable::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBreakSpacing(val);
  return eOk;
}

void createOdDbTableProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbTableBreakFlowDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableBreakOptionsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableBreakEnabledProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableMinimumTableWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableMinimumTableHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableHasSubSelectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableNumRowsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableNumColumnsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableCellsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableFlowDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableTableStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableSubSelectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableRegenerateTableSuppressedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableRepeatTopLabelsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableRepeatBottomLabelsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableAllowManualPositionsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableAllowManualHeightsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableTableBreakHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbTableBreakSpacingProperty::createObject(b.owner()));
  b.add(properties.last());
}
