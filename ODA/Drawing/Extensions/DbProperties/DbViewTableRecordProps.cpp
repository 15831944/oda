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
#include "DbViewTableRecord.h"
#include "DbValueTypes.h"

/* OdDbViewTableRecordLayerStateProperty */

struct OdDbViewTableRecordLayerStateProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewTableRecordLayerStateProperty>::createObject();
    ((OdDbViewTableRecordLayerStateProperty*)res.get())->init(__OD_T("LayerState"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewTableRecordLayerStateProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->getLayerState());
  return eOk;
}

OdResult OdDbViewTableRecordLayerStateProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLayerState(val);
  return eOk;
}

/* OdDbViewTableRecordIsPaperspaceViewProperty */

struct OdDbViewTableRecordIsPaperspaceViewProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewTableRecordIsPaperspaceViewProperty>::createObject();
    ((OdDbViewTableRecordIsPaperspaceViewProperty*)res.get())->init(__OD_T("IsPaperspaceView"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewTableRecordIsPaperspaceViewProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isPaperspaceView());
  return eOk;
}

OdResult OdDbViewTableRecordIsPaperspaceViewProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setIsPaperspaceView(val);
  return eOk;
}

/* OdDbViewTableRecordIsUcsAssociatedToViewProperty */

struct OdDbViewTableRecordIsUcsAssociatedToViewProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewTableRecordIsUcsAssociatedToViewProperty>::createObject();
    ((OdDbViewTableRecordIsUcsAssociatedToViewProperty*)res.get())->init(__OD_T("IsUcsAssociatedToView"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbViewTableRecordIsUcsAssociatedToViewProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isUcsAssociatedToView());
  return eOk;
}

/* OdDbViewTableRecordCategoryNameProperty */

struct OdDbViewTableRecordCategoryNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewTableRecordCategoryNameProperty>::createObject();
    ((OdDbViewTableRecordCategoryNameProperty*)res.get())->init(__OD_T("CategoryName"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewTableRecordCategoryNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->getCategoryName());
  return eOk;
}

OdResult OdDbViewTableRecordCategoryNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setCategoryName(val);
  return eOk;
}

/* OdDbViewTableRecordLayoutProperty */

struct OdDbViewTableRecordLayoutProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewTableRecordLayoutProperty>::createObject();
    ((OdDbViewTableRecordLayoutProperty*)res.get())->init(__OD_T("Layout"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbViewTableRecordLayoutProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewTableRecordLayoutProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->getLayout());
  return eOk;
}

OdResult OdDbViewTableRecordLayoutProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLayout(val);
  return eOk;
}

/* OdDbViewTableRecordViewAssociatedToViewportProperty */

struct OdDbViewTableRecordViewAssociatedToViewportProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewTableRecordViewAssociatedToViewportProperty>::createObject();
    ((OdDbViewTableRecordViewAssociatedToViewportProperty*)res.get())->init(__OD_T("ViewAssociatedToViewport"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewTableRecordViewAssociatedToViewportProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->isViewAssociatedToViewport());
  return eOk;
}

OdResult OdDbViewTableRecordViewAssociatedToViewportProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setViewAssociatedToViewport(val);
  return eOk;
}

/* OdDbViewTableRecordLiveSectionProperty */

struct OdDbViewTableRecordLiveSectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbViewTableRecordLiveSectionProperty>::createObject();
    ((OdDbViewTableRecordLiveSectionProperty*)res.get())->init(__OD_T("LiveSection"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbViewTableRecordLiveSectionProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbViewTableRecordLiveSectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->liveSection());
  return eOk;
}

OdResult OdDbViewTableRecordLiveSectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbViewTableRecordPtr pObj = OdDbViewTableRecord::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLiveSection(val);
  return eOk;
}

void createOdDbViewTableRecordProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbViewTableRecordLayerStateProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewTableRecordIsPaperspaceViewProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewTableRecordIsUcsAssociatedToViewProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewTableRecordCategoryNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewTableRecordLayoutProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewTableRecordViewAssociatedToViewportProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbViewTableRecordLiveSectionProperty::createObject(b.owner()));
  b.add(properties.last());
}
