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
#include "DbMlineStyle.h"
#include "DbValueTypes.h"

/* OdDbMlineStyleStartRoundCapProperty */

struct OdDbMlineStyleStartRoundCapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleStartRoundCapProperty>::createObject();
    ((OdDbMlineStyleStartRoundCapProperty*)res.get())->init(__OD_T("StartRoundCap"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleStartRoundCapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->startRoundCap());
  return eOk;
}

OdResult OdDbMlineStyleStartRoundCapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStartRoundCap(val);
  return eOk;
}

/* OdDbMlineStyleStartInnerArcsProperty */

struct OdDbMlineStyleStartInnerArcsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleStartInnerArcsProperty>::createObject();
    ((OdDbMlineStyleStartInnerArcsProperty*)res.get())->init(__OD_T("StartInnerArcs"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleStartInnerArcsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->startInnerArcs());
  return eOk;
}

OdResult OdDbMlineStyleStartInnerArcsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStartInnerArcs(val);
  return eOk;
}

/* OdDbMlineStyleEndSquareCapProperty */

struct OdDbMlineStyleEndSquareCapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleEndSquareCapProperty>::createObject();
    ((OdDbMlineStyleEndSquareCapProperty*)res.get())->init(__OD_T("EndSquareCap"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleEndSquareCapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->endSquareCap());
  return eOk;
}

OdResult OdDbMlineStyleEndSquareCapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEndSquareCap(val);
  return eOk;
}

/* OdDbMlineStyleNameProperty */

struct OdDbMlineStyleNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleNameProperty>::createObject();
    ((OdDbMlineStyleNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->name());
  return eOk;
}

OdResult OdDbMlineStyleNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setName(val);
  return eOk;
}

/* OdDbMlineStyleShowMitersProperty */

struct OdDbMlineStyleShowMitersProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleShowMitersProperty>::createObject();
    ((OdDbMlineStyleShowMitersProperty*)res.get())->init(__OD_T("ShowMiters"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleShowMitersProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->showMiters());
  return eOk;
}

OdResult OdDbMlineStyleShowMitersProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShowMiters(val);
  return eOk;
}

/* OdDbMlineStyleStartSquareCapProperty */

struct OdDbMlineStyleStartSquareCapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleStartSquareCapProperty>::createObject();
    ((OdDbMlineStyleStartSquareCapProperty*)res.get())->init(__OD_T("StartSquareCap"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleStartSquareCapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->startSquareCap());
  return eOk;
}

OdResult OdDbMlineStyleStartSquareCapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStartSquareCap(val);
  return eOk;
}

/* OdDbMlineStyleDescriptionProperty */

struct OdDbMlineStyleDescriptionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleDescriptionProperty>::createObject();
    ((OdDbMlineStyleDescriptionProperty*)res.get())->init(__OD_T("Description"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleDescriptionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->description());
  return eOk;
}

OdResult OdDbMlineStyleDescriptionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDescription(val);
  return eOk;
}

/* OdDbMlineStyleStartAngleProperty */

struct OdDbMlineStyleStartAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleStartAngleProperty>::createObject();
    ((OdDbMlineStyleStartAngleProperty*)res.get())->init(__OD_T("StartAngle"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleStartAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->startAngle());
  return eOk;
}

OdResult OdDbMlineStyleStartAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setStartAngle(val);
  return eOk;
}

/* OdDbMlineStyleEndAngleProperty */

struct OdDbMlineStyleEndAngleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleEndAngleProperty>::createObject();
    ((OdDbMlineStyleEndAngleProperty*)res.get())->init(__OD_T("EndAngle"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleEndAngleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->endAngle());
  return eOk;
}

OdResult OdDbMlineStyleEndAngleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEndAngle(val);
  return eOk;
}

/* OdDbMlineStyleEndRoundCapProperty */

struct OdDbMlineStyleEndRoundCapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleEndRoundCapProperty>::createObject();
    ((OdDbMlineStyleEndRoundCapProperty*)res.get())->init(__OD_T("EndRoundCap"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleEndRoundCapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->endRoundCap());
  return eOk;
}

OdResult OdDbMlineStyleEndRoundCapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEndRoundCap(val);
  return eOk;
}

/* OdDbMlineStyleEndInnerArcsProperty */

struct OdDbMlineStyleEndInnerArcsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleEndInnerArcsProperty>::createObject();
    ((OdDbMlineStyleEndInnerArcsProperty*)res.get())->init(__OD_T("EndInnerArcs"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleEndInnerArcsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->endInnerArcs());
  return eOk;
}

OdResult OdDbMlineStyleEndInnerArcsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEndInnerArcs(val);
  return eOk;
}

/* OdDbMlineStyleFillColorProperty */

struct OdDbMlineStyleFillColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleFillColorProperty>::createObject();
    ((OdDbMlineStyleFillColorProperty*)res.get())->init(__OD_T("FillColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleFillColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->fillColor());
  return eOk;
}

OdResult OdDbMlineStyleFillColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFillColor(val);
  return eOk;
}

/* OdDbMlineStyleFilledProperty */

struct OdDbMlineStyleFilledProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMlineStyleFilledProperty>::createObject();
    ((OdDbMlineStyleFilledProperty*)res.get())->init(__OD_T("Filled"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMlineStyleFilledProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->filled());
  return eOk;
}

OdResult OdDbMlineStyleFilledProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMlineStylePtr pObj = OdDbMlineStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFilled(val);
  return eOk;
}

void createOdDbMlineStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbMlineStyleStartRoundCapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleStartInnerArcsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleEndSquareCapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleNameProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleShowMitersProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleStartSquareCapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleDescriptionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleStartAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleEndAngleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleEndRoundCapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleEndInnerArcsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleFillColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMlineStyleFilledProperty::createObject(b.owner()));
  b.add(properties.last());
}
