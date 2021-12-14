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
#include "DbMline.h"
#include "DbValueTypes.h"

/* OdDbMlineScaleProperty */

struct OdDbMlineScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineScaleProperty>::createObject();
    ((OdDbMlineScaleProperty*)res.get())->init(__OD_T("Scale"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMlineScaleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMlineScaleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Mline scale"));
    ((OdDbMlineScaleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the scale of the MLine"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbMlineScaleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 0));
    ((OdDbMlineScaleProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"MLineScale"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->scale());
  return eOk;
}

OdResult OdDbMlineScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setScale(val);
  return eOk;
}

/* OdDbMlineNormalProperty */

struct OdDbMlineNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineNormalProperty>::createObject();
    ((OdDbMlineNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbMlineNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbMlineNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDbMlineStyleProperty */

struct OdDbMlineStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleProperty>::createObject();
    ((OdDbMlineStyleProperty*)res.get())->init(__OD_T("Style"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMlineStyleProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/NamedObjectsDictionaryId[ACAD_MLINESTYLE]/Items"));
    ((OdDbMlineStyleProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMlineStyleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Mline style"));
    ((OdDbMlineStyleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the Mline style name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->style());
  return eOk;
}

OdResult OdDbMlineStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStyle(val);
  return eOk;
}

/* OdDbMlineJustificationProperty */

struct OdDbMlineJustificationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineJustificationProperty>::createObject();
    ((OdDbMlineJustificationProperty*)res.get())->init(__OD_T("Justification"), &OdRxValueType::Desc<Mline::MlineJustification>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMlineJustificationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Mline justification"));
    ((OdDbMlineJustificationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the justification of the MLine"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbMlineJustificationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 0));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineJustificationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<Mline::MlineJustification>(pObj->justification());
  return eOk;
}

OdResult OdDbMlineJustificationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  Mline::MlineJustification val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setJustification(val);
  return eOk;
}

/* OdDbMlineSupressEndCapsProperty */

struct OdDbMlineSupressEndCapsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineSupressEndCapsProperty>::createObject();
    ((OdDbMlineSupressEndCapsProperty*)res.get())->init(__OD_T("SupressEndCaps"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineSupressEndCapsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->supressEndCaps());
  return eOk;
}

OdResult OdDbMlineSupressEndCapsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSupressEndCaps(val);
  return eOk;
}

/* OdDbMlineNumberOfVerticesProperty */

struct OdDbMlineNumberOfVerticesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineNumberOfVerticesProperty>::createObject();
    ((OdDbMlineNumberOfVerticesProperty*)res.get())->init(__OD_T("NumberOfVertices"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbMlineNumberOfVerticesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->numVertices());
  return eOk;
}

/* OdDbMlineIsClosedProperty */

struct OdDbMlineIsClosedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineIsClosedProperty>::createObject();
    ((OdDbMlineIsClosedProperty*)res.get())->init(__OD_T("IsClosed"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineIsClosedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->closedMline());
  return eOk;
}

OdResult OdDbMlineIsClosedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setClosedMline(val);
  return eOk;
}

/* OdDbMlineSupressStartCapsProperty */

struct OdDbMlineSupressStartCapsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineSupressStartCapsProperty>::createObject();
    ((OdDbMlineSupressStartCapsProperty*)res.get())->init(__OD_T("SupressStartCaps"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineSupressStartCapsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->supressStartCaps());
  return eOk;
}

OdResult OdDbMlineSupressStartCapsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlinePtr pObj = OdDbMline::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSupressStartCaps(val);
  return eOk;
}

void createOdDbMlineProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbMlineScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineJustificationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineSupressEndCapsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineNumberOfVerticesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineIsClosedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineSupressStartCapsProperty::createObject(b.owner()));
  b.add(properties.last());
}
