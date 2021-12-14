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
#include "DbMLeaderStyle.h"
#include "DbValueTypes.h"

/* OdDbMLeaderStyleLandingGapProperty */

struct OdDbMLeaderStyleLandingGapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleLandingGapProperty>::createObject();
    ((OdDbMLeaderStyleLandingGapProperty*)res.get())->init(__OD_T("LandingGap"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleLandingGapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->landingGap());
  return eOk;
}

OdResult OdDbMLeaderStyleLandingGapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLandingGap(val);
  return eOk;
}

/* OdDbMLeaderStyleEnableDoglegProperty */

struct OdDbMLeaderStyleEnableDoglegProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleEnableDoglegProperty>::createObject();
    ((OdDbMLeaderStyleEnableDoglegProperty*)res.get())->init(__OD_T("EnableDogleg"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleEnableDoglegProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableDogleg());
  return eOk;
}

OdResult OdDbMLeaderStyleEnableDoglegProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnableDogleg(val);
  return eOk;
}

/* OdDbMLeaderStyleLeaderLineWeightProperty */

struct OdDbMLeaderStyleLeaderLineWeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleLeaderLineWeightProperty>::createObject();
    ((OdDbMLeaderStyleLeaderLineWeightProperty*)res.get())->init(__OD_T("LeaderLineWeight"), &OdRxValueType::Desc<OdDb::LineWeight>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleLeaderLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineWeight>(pObj->leaderLineWeight());
  return eOk;
}

OdResult OdDbMLeaderStyleLeaderLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineWeight val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderLineWeight(val);
  return eOk;
}

/* OdDbMLeaderStyleEnableLandingProperty */

struct OdDbMLeaderStyleEnableLandingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleEnableLandingProperty>::createObject();
    ((OdDbMLeaderStyleEnableLandingProperty*)res.get())->init(__OD_T("EnableLanding"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleEnableLandingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableLanding());
  return eOk;
}

OdResult OdDbMLeaderStyleEnableLandingProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnableLanding(val);
  return eOk;
}

/* OdDbMLeaderStyleDoglegLengthProperty */

struct OdDbMLeaderStyleDoglegLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleDoglegLengthProperty>::createObject();
    ((OdDbMLeaderStyleDoglegLengthProperty*)res.get())->init(__OD_T("DoglegLength"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleDoglegLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->doglegLength());
  return eOk;
}

OdResult OdDbMLeaderStyleDoglegLengthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDoglegLength(val);
  return eOk;
}

/* OdDbMLeaderStyleTextStyleIdProperty */

struct OdDbMLeaderStyleTextStyleIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleTextStyleIdProperty>::createObject();
    ((OdDbMLeaderStyleTextStyleIdProperty*)res.get())->init(__OD_T("TextStyleId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMLeaderStyleTextStyleIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleTextStyleIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->textStyleId());
  return eOk;
}

OdResult OdDbMLeaderStyleTextStyleIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextStyleId(val);
  return eOk;
}

/* OdDbMLeaderStyleArrowSymbolIdProperty */

struct OdDbMLeaderStyleArrowSymbolIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleArrowSymbolIdProperty>::createObject();
    ((OdDbMLeaderStyleArrowSymbolIdProperty*)res.get())->init(__OD_T("ArrowSymbolId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMLeaderStyleArrowSymbolIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleArrowSymbolIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->arrowSymbolId());
  return eOk;
}

OdResult OdDbMLeaderStyleArrowSymbolIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArrowSymbolId(val);
  return eOk;
}

/* OdDbMLeaderStyleArrowSizeProperty */

struct OdDbMLeaderStyleArrowSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleArrowSizeProperty>::createObject();
    ((OdDbMLeaderStyleArrowSizeProperty*)res.get())->init(__OD_T("ArrowSize"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleArrowSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->arrowSize());
  return eOk;
}

OdResult OdDbMLeaderStyleArrowSizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArrowSize(val);
  return eOk;
}

/* OdDbMLeaderStyleDrawLeaderOrderTypeProperty */

struct OdDbMLeaderStyleDrawLeaderOrderTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleDrawLeaderOrderTypeProperty>::createObject();
    ((OdDbMLeaderStyleDrawLeaderOrderTypeProperty*)res.get())->init(__OD_T("DrawLeaderOrderType"), &OdRxValueType::Desc<OdDbMLeaderStyle::DrawLeaderOrderType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleDrawLeaderOrderTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::DrawLeaderOrderType>(pObj->drawLeaderOrderType());
  return eOk;
}

OdResult OdDbMLeaderStyleDrawLeaderOrderTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::DrawLeaderOrderType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDrawLeaderOrderType(val);
  return eOk;
}

/* OdDbMLeaderStyleMaxLeaderSegmentsPointsProperty */

struct OdDbMLeaderStyleMaxLeaderSegmentsPointsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleMaxLeaderSegmentsPointsProperty>::createObject();
    ((OdDbMLeaderStyleMaxLeaderSegmentsPointsProperty*)res.get())->init(__OD_T("MaxLeaderSegmentsPoints"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleMaxLeaderSegmentsPointsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->maxLeaderSegmentsPoints());
  return eOk;
}

OdResult OdDbMLeaderStyleMaxLeaderSegmentsPointsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setMaxLeaderSegmentsPoints(val);
  return eOk;
}

/* OdDbMLeaderStyleContentTypeProperty */

struct OdDbMLeaderStyleContentTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleContentTypeProperty>::createObject();
    ((OdDbMLeaderStyleContentTypeProperty*)res.get())->init(__OD_T("ContentType"), &OdRxValueType::Desc<OdDbMLeaderStyle::ContentType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleContentTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::ContentType>(pObj->contentType());
  return eOk;
}

OdResult OdDbMLeaderStyleContentTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::ContentType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setContentType(val);
  return eOk;
}

/* OdDbMLeaderStyleDrawMLeaderOrderTypeProperty */

struct OdDbMLeaderStyleDrawMLeaderOrderTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleDrawMLeaderOrderTypeProperty>::createObject();
    ((OdDbMLeaderStyleDrawMLeaderOrderTypeProperty*)res.get())->init(__OD_T("DrawMLeaderOrderType"), &OdRxValueType::Desc<OdDbMLeaderStyle::DrawMLeaderOrderType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleDrawMLeaderOrderTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::DrawMLeaderOrderType>(pObj->drawMLeaderOrderType());
  return eOk;
}

OdResult OdDbMLeaderStyleDrawMLeaderOrderTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::DrawMLeaderOrderType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDrawMLeaderOrderType(val);
  return eOk;
}

/* OdDbMLeaderStyleFirstSegmentAngleConstraintProperty */

struct OdDbMLeaderStyleFirstSegmentAngleConstraintProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleFirstSegmentAngleConstraintProperty>::createObject();
    ((OdDbMLeaderStyleFirstSegmentAngleConstraintProperty*)res.get())->init(__OD_T("FirstSegmentAngleConstraint"), &OdRxValueType::Desc<OdDbMLeaderStyle::SegmentAngleType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleFirstSegmentAngleConstraintProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::SegmentAngleType>(pObj->firstSegmentAngleConstraint());
  return eOk;
}

OdResult OdDbMLeaderStyleFirstSegmentAngleConstraintProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::SegmentAngleType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFirstSegmentAngleConstraint(val);
  return eOk;
}

/* OdDbMLeaderStyleLeaderLineColorProperty */

struct OdDbMLeaderStyleLeaderLineColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleLeaderLineColorProperty>::createObject();
    ((OdDbMLeaderStyleLeaderLineColorProperty*)res.get())->init(__OD_T("LeaderLineColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleLeaderLineColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->leaderLineColor());
  return eOk;
}

OdResult OdDbMLeaderStyleLeaderLineColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderLineColor(val);
  return eOk;
}

/* OdDbMLeaderStyleLeaderLineTypeIdProperty */

struct OdDbMLeaderStyleLeaderLineTypeIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleLeaderLineTypeIdProperty>::createObject();
    ((OdDbMLeaderStyleLeaderLineTypeIdProperty*)res.get())->init(__OD_T("LeaderLineTypeId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMLeaderStyleLeaderLineTypeIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleLeaderLineTypeIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->leaderLineTypeId());
  return eOk;
}

OdResult OdDbMLeaderStyleLeaderLineTypeIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderLineTypeId(val);
  return eOk;
}

/* OdDbMLeaderStyleSecondSegmentAngleConstraintProperty */

struct OdDbMLeaderStyleSecondSegmentAngleConstraintProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleSecondSegmentAngleConstraintProperty>::createObject();
    ((OdDbMLeaderStyleSecondSegmentAngleConstraintProperty*)res.get())->init(__OD_T("SecondSegmentAngleConstraint"), &OdRxValueType::Desc<OdDbMLeaderStyle::SegmentAngleType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleSecondSegmentAngleConstraintProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::SegmentAngleType>(pObj->secondSegmentAngleConstraint());
  return eOk;
}

OdResult OdDbMLeaderStyleSecondSegmentAngleConstraintProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::SegmentAngleType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setSecondSegmentAngleConstraint(val);
  return eOk;
}

/* OdDbMLeaderStyleLeaderLineTypeProperty */

struct OdDbMLeaderStyleLeaderLineTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleLeaderLineTypeProperty>::createObject();
    ((OdDbMLeaderStyleLeaderLineTypeProperty*)res.get())->init(__OD_T("LeaderLineType"), &OdRxValueType::Desc<OdDbMLeaderStyle::LeaderType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleLeaderLineTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::LeaderType>(pObj->leaderLineType());
  return eOk;
}

OdResult OdDbMLeaderStyleLeaderLineTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::LeaderType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderLineType(val);
  return eOk;
}

/* OdDbMLeaderStyleBlockRotationProperty */

struct OdDbMLeaderStyleBlockRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleBlockRotationProperty>::createObject();
    ((OdDbMLeaderStyleBlockRotationProperty*)res.get())->init(__OD_T("BlockRotation"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleBlockRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->blockRotation());
  return eOk;
}

OdResult OdDbMLeaderStyleBlockRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockRotation(val);
  return eOk;
}

/* OdDbMLeaderStyleEnableBlockRotationProperty */

struct OdDbMLeaderStyleEnableBlockRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleEnableBlockRotationProperty>::createObject();
    ((OdDbMLeaderStyleEnableBlockRotationProperty*)res.get())->init(__OD_T("EnableBlockRotation"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleEnableBlockRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableBlockRotation());
  return eOk;
}

OdResult OdDbMLeaderStyleEnableBlockRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnableBlockRotation(val);
  return eOk;
}

/* OdDbMLeaderStyleBlockScaleProperty */

struct OdDbMLeaderStyleBlockScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleBlockScaleProperty>::createObject();
    ((OdDbMLeaderStyleBlockScaleProperty*)res.get())->init(__OD_T("BlockScale"), &OdRxValueType::Desc<OdGeScale3d>::value(), owner);
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

OdResult OdDbMLeaderStyleBlockScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeScale3d>(pObj->blockScale());
  return eOk;
}

OdResult OdDbMLeaderStyleBlockScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeScale3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockScale(val);
  return eOk;
}

/* OdDbMLeaderStyleEnableBlockScaleProperty */

struct OdDbMLeaderStyleEnableBlockScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleEnableBlockScaleProperty>::createObject();
    ((OdDbMLeaderStyleEnableBlockScaleProperty*)res.get())->init(__OD_T("EnableBlockScale"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleEnableBlockScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableBlockScale());
  return eOk;
}

OdResult OdDbMLeaderStyleEnableBlockScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnableBlockScale(val);
  return eOk;
}

/* OdDbMLeaderStyleBreakSizeProperty */

struct OdDbMLeaderStyleBreakSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleBreakSizeProperty>::createObject();
    ((OdDbMLeaderStyleBreakSizeProperty*)res.get())->init(__OD_T("BreakSize"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleBreakSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->breakSize());
  return eOk;
}

OdResult OdDbMLeaderStyleBreakSizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBreakSize(val);
  return eOk;
}

/* OdDbMLeaderStyleTextAttachmentDirectionProperty */

struct OdDbMLeaderStyleTextAttachmentDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleTextAttachmentDirectionProperty>::createObject();
    ((OdDbMLeaderStyleTextAttachmentDirectionProperty*)res.get())->init(__OD_T("TextAttachmentDirection"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAttachmentDirection>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleTextAttachmentDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::TextAttachmentDirection>(pObj->textAttachmentDirection());
  return eOk;
}

OdResult OdDbMLeaderStyleTextAttachmentDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAttachmentDirection val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextAttachmentDirection(val);
  return eOk;
}

/* OdDbMLeaderStyleBlockConnectionTypeProperty */

struct OdDbMLeaderStyleBlockConnectionTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleBlockConnectionTypeProperty>::createObject();
    ((OdDbMLeaderStyleBlockConnectionTypeProperty*)res.get())->init(__OD_T("BlockConnectionType"), &OdRxValueType::Desc<OdDbMLeaderStyle::BlockConnectionType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleBlockConnectionTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::BlockConnectionType>(pObj->blockConnectionType());
  return eOk;
}

OdResult OdDbMLeaderStyleBlockConnectionTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::BlockConnectionType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockConnectionType(val);
  return eOk;
}

/* OdDbMLeaderStyleScaleProperty */

struct OdDbMLeaderStyleScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleScaleProperty>::createObject();
    ((OdDbMLeaderStyleScaleProperty*)res.get())->init(__OD_T("Scale"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->scale());
  return eOk;
}

OdResult OdDbMLeaderStyleScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setScale(val);
  return eOk;
}

/* OdDbMLeaderStyleBlockColorProperty */

struct OdDbMLeaderStyleBlockColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleBlockColorProperty>::createObject();
    ((OdDbMLeaderStyleBlockColorProperty*)res.get())->init(__OD_T("BlockColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleBlockColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->blockColor());
  return eOk;
}

OdResult OdDbMLeaderStyleBlockColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockColor(val);
  return eOk;
}

/* OdDbMLeaderStyleTextAlignmentTypeProperty */

struct OdDbMLeaderStyleTextAlignmentTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleTextAlignmentTypeProperty>::createObject();
    ((OdDbMLeaderStyleTextAlignmentTypeProperty*)res.get())->init(__OD_T("TextAlignmentType"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAlignmentType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleTextAlignmentTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::TextAlignmentType>(pObj->textAlignmentType());
  return eOk;
}

OdResult OdDbMLeaderStyleTextAlignmentTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAlignmentType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextAlignmentType(val);
  return eOk;
}

/* OdDbMLeaderStyleTextAlignAlwaysLeftProperty */

struct OdDbMLeaderStyleTextAlignAlwaysLeftProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleTextAlignAlwaysLeftProperty>::createObject();
    ((OdDbMLeaderStyleTextAlignAlwaysLeftProperty*)res.get())->init(__OD_T("TextAlignAlwaysLeft"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleTextAlignAlwaysLeftProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->textAlignAlwaysLeft());
  return eOk;
}

OdResult OdDbMLeaderStyleTextAlignAlwaysLeftProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextAlignAlwaysLeft(val);
  return eOk;
}

/* OdDbMLeaderStyleTextAngleTypeProperty */

struct OdDbMLeaderStyleTextAngleTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleTextAngleTypeProperty>::createObject();
    ((OdDbMLeaderStyleTextAngleTypeProperty*)res.get())->init(__OD_T("TextAngleType"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAngleType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleTextAngleTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::TextAngleType>(pObj->textAngleType());
  return eOk;
}

OdResult OdDbMLeaderStyleTextAngleTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAngleType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextAngleType(val);
  return eOk;
}

/* OdDbMLeaderStyleEnableFrameTextProperty */

struct OdDbMLeaderStyleEnableFrameTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleEnableFrameTextProperty>::createObject();
    ((OdDbMLeaderStyleEnableFrameTextProperty*)res.get())->init(__OD_T("EnableFrameText"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleEnableFrameTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableFrameText());
  return eOk;
}

OdResult OdDbMLeaderStyleEnableFrameTextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnableFrameText(val);
  return eOk;
}

/* OdDbMLeaderStyleBlockIdProperty */

struct OdDbMLeaderStyleBlockIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleBlockIdProperty>::createObject();
    ((OdDbMLeaderStyleBlockIdProperty*)res.get())->init(__OD_T("BlockId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMLeaderStyleBlockIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleBlockIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->blockId());
  return eOk;
}

OdResult OdDbMLeaderStyleBlockIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockId(val);
  return eOk;
}

/* OdDbMLeaderStyleTextColorProperty */

struct OdDbMLeaderStyleTextColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleTextColorProperty>::createObject();
    ((OdDbMLeaderStyleTextColorProperty*)res.get())->init(__OD_T("TextColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleTextColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->textColor());
  return eOk;
}

OdResult OdDbMLeaderStyleTextColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextColor(val);
  return eOk;
}

/* OdDbMLeaderStyleTextHeightProperty */

struct OdDbMLeaderStyleTextHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleTextHeightProperty>::createObject();
    ((OdDbMLeaderStyleTextHeightProperty*)res.get())->init(__OD_T("TextHeight"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleTextHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->textHeight());
  return eOk;
}

OdResult OdDbMLeaderStyleTextHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextHeight(val);
  return eOk;
}

/* OdDbMLeaderStyleNameProperty */

struct OdDbMLeaderStyleNameProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderStyleNameProperty>::createObject();
    ((OdDbMLeaderStyleNameProperty*)res.get())->init(__OD_T("Name"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderStyleNameProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdString>(pObj->getName());
  return eOk;
}

OdResult OdDbMLeaderStyleNameProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderStylePtr pObj = OdDbMLeaderStyle::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setName(val);
  return eOk;
}

void createOdDbMLeaderStyleProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbMLeaderStyleLandingGapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleEnableDoglegProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleLeaderLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleEnableLandingProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleDoglegLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleTextStyleIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleArrowSymbolIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleArrowSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleDrawLeaderOrderTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleMaxLeaderSegmentsPointsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleContentTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleDrawMLeaderOrderTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleFirstSegmentAngleConstraintProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleLeaderLineColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleLeaderLineTypeIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleSecondSegmentAngleConstraintProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleLeaderLineTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleBlockRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleEnableBlockRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleBlockScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleEnableBlockScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleBreakSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleTextAttachmentDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleBlockConnectionTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleBlockColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleTextAlignmentTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleTextAlignAlwaysLeftProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleTextAngleTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleEnableFrameTextProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleBlockIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleTextColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleTextHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderStyleNameProperty::createObject(b.owner()));
  b.add(properties.last());
}
