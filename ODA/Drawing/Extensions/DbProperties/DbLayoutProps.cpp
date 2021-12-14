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
#include "DbLayout.h"
#include "DbValueTypes.h"

/* OdDbLayoutAnnoAllVisibleProperty */

struct OdDbLayoutAnnoAllVisibleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayoutAnnoAllVisibleProperty>::createObject();
    ((OdDbLayoutAnnoAllVisibleProperty*)res.get())->init(__OD_T("AnnoAllVisible"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayoutAnnoAllVisibleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->annoAllVisible());
  return eOk;
}

OdResult OdDbLayoutAnnoAllVisibleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAnnoAllVisible(val);
  return eOk;
}

/* OdDbLayoutLayoutNameProperty */

struct OdDbLayoutLayoutNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayoutLayoutNameProperty>::createObject();
    ((OdDbLayoutLayoutNameProperty*)res.get())->init(__OD_T("LayoutName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayoutLayoutNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->getLayoutName());
  return eOk;
}

OdResult OdDbLayoutLayoutNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLayoutName(val);
  return eOk;
}

/* OdDbLayoutTabOrderProperty */

struct OdDbLayoutTabOrderProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayoutTabOrderProperty>::createObject();
    ((OdDbLayoutTabOrderProperty*)res.get())->init(__OD_T("TabOrder"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayoutTabOrderProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->getTabOrder());
  return eOk;
}

OdResult OdDbLayoutTabOrderProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTabOrder(val);
  return eOk;
}

/* OdDbLayoutTabSelectedProperty */

struct OdDbLayoutTabSelectedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayoutTabSelectedProperty>::createObject();
    ((OdDbLayoutTabSelectedProperty*)res.get())->init(__OD_T("TabSelected"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayoutTabSelectedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->getTabSelected());
  return eOk;
}

OdResult OdDbLayoutTabSelectedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTabSelected(val);
  return eOk;
}

/* OdDbLayoutBlockTableRecordIdProperty */

struct OdDbLayoutBlockTableRecordIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLayoutBlockTableRecordIdProperty>::createObject();
    ((OdDbLayoutBlockTableRecordIdProperty*)res.get())->init(__OD_T("BlockTableRecordId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbLayoutBlockTableRecordIdProperty*)res.get())->attributes().add(OdRxHierarchyLevelAttribute::createObject(L"Block"));
    ((OdDbLayoutBlockTableRecordIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLayoutBlockTableRecordIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getBlockTableRecordId());
  return eOk;
}

OdResult OdDbLayoutBlockTableRecordIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLayoutPtr pObj = OdDbLayout::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockTableRecordId(val);
  return eOk;
}

void createOdDbLayoutProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbLayoutAnnoAllVisibleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayoutLayoutNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayoutTabOrderProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayoutTabSelectedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLayoutBlockTableRecordIdProperty::createObject(b.owner()));
  b.add(properties.last());
}
