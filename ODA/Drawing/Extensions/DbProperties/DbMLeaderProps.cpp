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
#include "DbMLeader.h"
#include "DbValueTypes.h"

/* OdDbMLeaderDoglegLengthProperty */

struct OdDbMLeaderDoglegLengthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderDoglegLengthProperty>::createObject();
    ((OdDbMLeaderDoglegLengthProperty*)res.get())->init(__OD_T("DoglegLength"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMLeaderDoglegLengthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderDoglegLengthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Landing distance"));
    ((OdDbMLeaderDoglegLengthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the landing distance"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Leaders"))
      OdRxCategory::createObject(L"Leaders", OdRxCategory::rootCategory());
    ((OdDbMLeaderDoglegLengthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Leaders", 8));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderDoglegLengthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->doglegLength());
  return eOk;
}

OdResult OdDbMLeaderDoglegLengthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setDoglegLength(val);
  return rc;
}

/* OdDbMLeaderArrowSymbolIdProperty */

struct OdDbMLeaderArrowSymbolIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderArrowSymbolIdProperty>::createObject();
    ((OdDbMLeaderArrowSymbolIdProperty*)res.get())->init(__OD_T("ArrowSymbolId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMLeaderArrowSymbolIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderArrowSymbolIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->arrowSymbolId());
  return eOk;
}

OdResult OdDbMLeaderArrowSymbolIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArrowSymbolId(val);
  return eOk;
}

/* OdDbMLeaderArrowheadTypeProperty */

//COMMENT: TODO: needed realized get/set ArrowheadType
struct OdDbMLeaderArrowheadTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderArrowheadTypeProperty>::createObject();
    ((OdDbMLeaderArrowheadTypeProperty*)res.get())->init(__OD_T("ArrowheadType"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderArrowheadTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Arrowhead"));
    ((OdDbMLeaderArrowheadTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the type of leader arrowhead"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Leaders"))
      OdRxCategory::createObject(L"Leaders", OdRxCategory::rootCategory());
    ((OdDbMLeaderArrowheadTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Leaders", 5));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderArrowheadTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  //>>ENOTAPPLICABLE
  return eNotApplicable;
}

OdResult OdDbMLeaderArrowheadTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  //>>ENOTAPPLICABLE
  return eNotApplicable;
}

/* OdDbMLeaderEnableLandingProperty */

struct OdDbMLeaderEnableLandingProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderEnableLandingProperty>::createObject();
    ((OdDbMLeaderEnableLandingProperty*)res.get())->init(__OD_T("EnableLanding"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderEnableLandingProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableLanding());
  return eOk;
}

OdResult OdDbMLeaderEnableLandingProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnableLanding(val);
  return eOk;
}

/* OdDbMLeaderEnableDoglegProperty */

struct OdDbMLeaderEnableDoglegProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderEnableDoglegProperty>::createObject();
    ((OdDbMLeaderEnableDoglegProperty*)res.get())->init(__OD_T("EnableDogleg"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderEnableDoglegProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Horizontal Landing"));
    ((OdDbMLeaderEnableDoglegProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Enable/Disable horizontal landing of multileader"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Leaders"))
      OdRxCategory::createObject(L"Leaders", OdRxCategory::rootCategory());
    ((OdDbMLeaderEnableDoglegProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Leaders", 7));
    ((OdDbMLeaderEnableDoglegProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DogLegged"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderEnableDoglegProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableDogleg());
  return eOk;
}

OdResult OdDbMLeaderEnableDoglegProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnableDogleg(val);
  return eOk;
}

/* OdDbMLeaderTextStyleIdProperty */

struct OdDbMLeaderTextStyleIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextStyleIdProperty>::createObject();
    ((OdDbMLeaderTextStyleIdProperty*)res.get())->init(__OD_T("TextStyleId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMLeaderTextStyleIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderTextStyleIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->textStyleId());
  return eOk;
}

OdResult OdDbMLeaderTextStyleIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextStyleId(val);
  return eOk;
}

/* OdDbMLeaderArrowSizeProperty */

struct OdDbMLeaderArrowSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderArrowSizeProperty>::createObject();
    ((OdDbMLeaderArrowSizeProperty*)res.get())->init(__OD_T("ArrowSize"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMLeaderArrowSizeProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderArrowSizeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Arrowhead Size"));
    ((OdDbMLeaderArrowSizeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the size of leader arrowhead"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Leaders"))
      OdRxCategory::createObject(L"Leaders", OdRxCategory::rootCategory());
    ((OdDbMLeaderArrowSizeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Leaders", 6));
    ((OdDbMLeaderArrowSizeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ArrowheadSize"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderArrowSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->arrowSize());
  return eOk;
}

OdResult OdDbMLeaderArrowSizeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setArrowSize(val);
  return eOk;
}

/* OdDbMLeaderContentTypeProperty */

struct OdDbMLeaderContentTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderContentTypeProperty>::createObject();
    ((OdDbMLeaderContentTypeProperty*)res.get())->init(__OD_T("ContentType"), &OdRxValueType::Desc<OdDbMLeaderStyle::ContentType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderContentTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::ContentType>(pObj->contentType());
  return eOk;
}

OdResult OdDbMLeaderContentTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::ContentType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setContentType(val);
  return eOk;
}

/* OdDbMLeaderLeaderLineCountProperty */

struct OdDbMLeaderLeaderLineCountProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderLeaderLineCountProperty>::createObject();
    ((OdDbMLeaderLeaderLineCountProperty*)res.get())->init(__OD_T("LeaderLineCount"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbMLeaderLeaderLineCountProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdIntArray leaderLineIndexes; pObj->getLeaderLineIndexes(leaderLineIndexes); value = static_cast<int>(leaderLineIndexes.size());
  return eOk;
}

/* OdDbMLeaderLeaderLineTypeProperty */

struct OdDbMLeaderLeaderLineTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderLeaderLineTypeProperty>::createObject();
    ((OdDbMLeaderLeaderLineTypeProperty*)res.get())->init(__OD_T("LeaderLineType"), &OdRxValueType::Desc<OdDbMLeaderStyle::LeaderType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderLeaderLineTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Leader type"));
    ((OdDbMLeaderLeaderLineTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the leader type"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Leaders"))
      OdRxCategory::createObject(L"Leaders", OdRxCategory::rootCategory());
    ((OdDbMLeaderLeaderLineTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Leaders", 1));
    ((OdDbMLeaderLeaderLineTypeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LeaderType"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderLeaderLineTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::LeaderType>(pObj->leaderLineType());
  return eOk;
}

OdResult OdDbMLeaderLeaderLineTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::LeaderType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderLineType(val);
  return eOk;
}

/* OdDbMLeaderNormalProperty */

struct OdDbMLeaderNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderNormalProperty>::createObject();
    ((OdDbMLeaderNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbMLeaderNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

/* OdDbMLeaderLeaderCountProperty */

struct OdDbMLeaderLeaderCountProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderLeaderCountProperty>::createObject();
    ((OdDbMLeaderLeaderCountProperty*)res.get())->init(__OD_T("LeaderCount"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbMLeaderLeaderCountProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdIntArray leaderIndexes; pObj->getLeaderIndexes(leaderIndexes); value = static_cast<int>(leaderIndexes.size());
  return eOk;
}

/* OdDbMLeaderLeaderLineWeightProperty */

struct OdDbMLeaderLeaderLineWeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderLeaderLineWeightProperty>::createObject();
    ((OdDbMLeaderLeaderLineWeightProperty*)res.get())->init(__OD_T("LeaderLineWeight"), &OdRxValueType::Desc<OdDb::LineWeight>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderLeaderLineWeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Leader lineweight"));
    ((OdDbMLeaderLeaderLineWeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the line weight of leader lines"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Leaders"))
      OdRxCategory::createObject(L"Leaders", OdRxCategory::rootCategory());
    ((OdDbMLeaderLeaderLineWeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Leaders", 4));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderLeaderLineWeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineWeight>(pObj->leaderLineWeight());
  return eOk;
}

OdResult OdDbMLeaderLeaderLineWeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineWeight val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderLineWeight(val);
  return eOk;
}

/* OdDbMLeaderLandingGapProperty */

struct OdDbMLeaderLandingGapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderLandingGapProperty>::createObject();
    ((OdDbMLeaderLandingGapProperty*)res.get())->init(__OD_T("LandingGap"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMLeaderLandingGapProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderLandingGapProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Landing gap"));
    ((OdDbMLeaderLandingGapProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the text landing gap"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMLeaderLandingGapProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 85));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderLandingGapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kMTextContent)
    return eNotApplicable;
  value = static_cast<double>(pObj->landingGap());
  return eOk;
}

OdResult OdDbMLeaderLandingGapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLandingGap(val);
  return eOk;
}

/* OdDbMLeaderLeaderLineColorProperty */

struct OdDbMLeaderLeaderLineColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderLeaderLineColorProperty>::createObject();
    ((OdDbMLeaderLeaderLineColorProperty*)res.get())->init(__OD_T("LeaderLineColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderLeaderLineColorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Leader color"));
    ((OdDbMLeaderLeaderLineColorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the color of the leader lines"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Leaders"))
      OdRxCategory::createObject(L"Leaders", OdRxCategory::rootCategory());
    ((OdDbMLeaderLeaderLineColorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Leaders", 2));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderLeaderLineColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->leaderLineColor());
  return eOk;
}

OdResult OdDbMLeaderLeaderLineColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderLineColor(val);
  return eOk;
}

/* OdDbMLeaderLeaderLineTypeIdProperty */

struct OdDbMLeaderLeaderLineTypeIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderLeaderLineTypeIdProperty>::createObject();
    ((OdDbMLeaderLeaderLineTypeIdProperty*)res.get())->init(__OD_T("LeaderLineTypeId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMLeaderLeaderLineTypeIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/LinetypeTableId/Items"));
    ((OdDbMLeaderLeaderLineTypeIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderLeaderLineTypeIdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Leader linetype"));
    ((OdDbMLeaderLeaderLineTypeIdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the linetype of leader lines"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Leaders"))
      OdRxCategory::createObject(L"Leaders", OdRxCategory::rootCategory());
    ((OdDbMLeaderLeaderLineTypeIdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Leaders", 3));
    ((OdDbMLeaderLeaderLineTypeIdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LeaderLinetype"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderLeaderLineTypeIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->leaderLineTypeId());
  return eOk;
}

OdResult OdDbMLeaderLeaderLineTypeIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLeaderLineTypeId(val);
  return eOk;
}

/* OdDbMLeaderTextAngleTypeProperty */

struct OdDbMLeaderTextAngleTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextAngleTypeProperty>::createObject();
    ((OdDbMLeaderTextAngleTypeProperty*)res.get())->init(__OD_T("TextAngleType"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAngleType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderTextAngleTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::TextAngleType>(pObj->textAngleType());
  return eOk;
}

OdResult OdDbMLeaderTextAngleTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAngleType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextAngleType(val);
  return eOk;
}

/* OdDbMLeaderMLeaderStyleProperty */

struct OdDbMLeaderMLeaderStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderMLeaderStyleProperty>::createObject();
    ((OdDbMLeaderMLeaderStyleProperty*)res.get())->init(__OD_T("MLeaderStyle"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMLeaderMLeaderStyleProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/MLeaderStyleDictionaryId/Items"));
    ((OdDbMLeaderMLeaderStyleProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderMLeaderStyleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Multileader style"));
    ((OdDbMLeaderMLeaderStyleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the style name of this multileader object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbMLeaderMLeaderStyleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 2));
    ((OdDbMLeaderMLeaderStyleProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderMLeaderStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->MLeaderStyle());
  return eOk;
}

OdResult OdDbMLeaderMLeaderStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setMLeaderStyle(val);
  return eOk;
}

/* OdDbMLeaderBlockPositionProperty */

struct OdDbMLeaderBlockPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderBlockPositionProperty>::createObject();
    ((OdDbMLeaderBlockPositionProperty*)res.get())->init(__OD_T("BlockPosition"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbMLeaderBlockPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  if (pObj->contentType() != OdDbMLeaderStyle::kBlockContent) return eNotApplicable; else { OdGePoint3d position; pObj->getBlockPosition(position); value = position; }
  return eOk;
}

OdResult OdDbMLeaderBlockPositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockPosition(val);
  return eOk;
}

/* OdDbMLeaderBlockConnectionTypeProperty */

struct OdDbMLeaderBlockConnectionTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderBlockConnectionTypeProperty>::createObject();
    ((OdDbMLeaderBlockConnectionTypeProperty*)res.get())->init(__OD_T("BlockConnectionType"), &OdRxValueType::Desc<OdDbMLeaderStyle::BlockConnectionType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderBlockConnectionTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Attachment"));
    ((OdDbMLeaderBlockConnectionTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify how leaders connect with content block"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Block"))
      OdRxCategory::createObject(L"Block", OdRxCategory::rootCategory());
    ((OdDbMLeaderBlockConnectionTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Block", 0));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderBlockConnectionTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kBlockContent)
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::BlockConnectionType>(pObj->blockConnectionType());
  return eOk;
}

OdResult OdDbMLeaderBlockConnectionTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::BlockConnectionType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockConnectionType(val);
  return eOk;
}

/* OdDbMLeaderEnableAnnotationScaleProperty */

struct OdDbMLeaderEnableAnnotationScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderEnableAnnotationScaleProperty>::createObject();
    ((OdDbMLeaderEnableAnnotationScaleProperty*)res.get())->init(__OD_T("EnableAnnotationScale"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderEnableAnnotationScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableAnnotationScale());
  return eOk;
}

OdResult OdDbMLeaderEnableAnnotationScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnableAnnotationScale(val);
  return eOk;
}

/* OdDbMLeaderTextAttachmentDirectionProperty */

struct OdDbMLeaderTextAttachmentDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextAttachmentDirectionProperty>::createObject();
    ((OdDbMLeaderTextAttachmentDirectionProperty*)res.get())->init(__OD_T("TextAttachmentDirection"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAttachmentDirection>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderTextAttachmentDirectionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Attachment type"));
    ((OdDbMLeaderTextAttachmentDirectionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies if leaders connect with the text horizontally or vertically"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMLeaderTextAttachmentDirectionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 62));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderTextAttachmentDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kMTextContent)
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::TextAttachmentDirection>(pObj->textAttachmentDirection());
  return eOk;
}

OdResult OdDbMLeaderTextAttachmentDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAttachmentDirection val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextAttachmentDirection(val);
  return eOk;
}

/* OdDbMLeaderTextLocationProperty */

struct OdDbMLeaderTextLocationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextLocationProperty>::createObject();
    ((OdDbMLeaderTextLocationProperty*)res.get())->init(__OD_T("TextLocation"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbMLeaderTextLocationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kMTextContent || !pObj->hasContent())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGePoint3d location; pObj->getTextLocation(location); value = location;
  return eOk;
}

OdResult OdDbMLeaderTextLocationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextLocation(val);
  return eOk;
}

/* OdDbMLeaderToleranceLocationProperty */

struct OdDbMLeaderToleranceLocationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderToleranceLocationProperty>::createObject();
    ((OdDbMLeaderToleranceLocationProperty*)res.get())->init(__OD_T("ToleranceLocation"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbMLeaderToleranceLocationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdGePoint3d location; pObj->getToleranceLocation(location); value = location;
  return eOk;
}

OdResult OdDbMLeaderToleranceLocationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setToleranceLocation(val);
  return rc;
}

/* OdDbMLeaderScaledTextHeightProperty */

struct OdDbMLeaderScaledTextHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderScaledTextHeightProperty>::createObject();
    ((OdDbMLeaderScaledTextHeightProperty*)res.get())->init(__OD_T("ScaledTextHeight"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

//COMMENT: TODO: Check formula
OdResult OdDbMLeaderScaledTextHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->textHeight() * pObj->scale();
  return eOk;
}

//COMMENT: TODO: Check formula
OdResult OdDbMLeaderScaledTextHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setTextHeight(val / pObj->scale());
  return eOk;
}

/* OdDbMLeaderEnableFrameTextProperty */

struct OdDbMLeaderEnableFrameTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderEnableFrameTextProperty>::createObject();
    ((OdDbMLeaderEnableFrameTextProperty*)res.get())->init(__OD_T("EnableFrameText"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderEnableFrameTextProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text frame"));
    ((OdDbMLeaderEnableFrameTextProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Display/hide text frame of multileader content"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMLeaderEnableFrameTextProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 90));
    ((OdDbMLeaderEnableFrameTextProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextFrameDisplay"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderEnableFrameTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kMTextContent)
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableFrameText());
  return eOk;
}

OdResult OdDbMLeaderEnableFrameTextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setEnableFrameText(val);
  return eOk;
}

/* OdDbMLeaderTextAlignmentTypeProperty */

struct OdDbMLeaderTextAlignmentTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextAlignmentTypeProperty>::createObject();
    ((OdDbMLeaderTextAlignmentTypeProperty*)res.get())->init(__OD_T("TextAlignmentType"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAlignmentType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderTextAlignmentTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Justify"));
    ((OdDbMLeaderTextAlignmentTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies both text height and text orientation by designating the endpoints of the baseline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMLeaderTextAlignmentTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 10));
    ((OdDbMLeaderTextAlignmentTypeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextJustify"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderTextAlignmentTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kMTextContent)
    return eNotApplicable;
  value = static_cast<OdDbMLeaderStyle::TextAlignmentType>(pObj->textAlignmentType());
  return eOk;
}

OdResult OdDbMLeaderTextAlignmentTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAlignmentType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextAlignmentType(val);
  return eOk;
}

/* OdDbMLeaderTextColorProperty */

struct OdDbMLeaderTextColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextColorProperty>::createObject();
    ((OdDbMLeaderTextColorProperty*)res.get())->init(__OD_T("TextColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderTextColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->textColor());
  return eOk;
}

OdResult OdDbMLeaderTextColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextColor(val);
  return eOk;
}

/* OdDbMLeaderBlockScaleProperty */

struct OdDbMLeaderBlockScaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderBlockScaleProperty>::createObject();
    ((OdDbMLeaderBlockScaleProperty*)res.get())->init(__OD_T("BlockScale"), &OdRxValueType::Desc<OdGeScale3d>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"UniformScale", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Scale"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify how leaders connect with content block"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Block"))
      OdRxCategory::createObject(L"Block", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Block", 0));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderBlockScaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeScale3d>(pObj->blockScale());
  return eOk;
}

OdResult OdDbMLeaderBlockScaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeScale3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockScale(val);
  return eOk;
}

/* OdDbMLeaderBlockRotationProperty */

struct OdDbMLeaderBlockRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderBlockRotationProperty>::createObject();
    ((OdDbMLeaderBlockRotationProperty*)res.get())->init(__OD_T("BlockRotation"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderBlockRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->blockRotation());
  return eOk;
}

OdResult OdDbMLeaderBlockRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockRotation(val);
  return eOk;
}

/* OdDbMLeaderBlockContentIdProperty */

struct OdDbMLeaderBlockContentIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderBlockContentIdProperty>::createObject();
    ((OdDbMLeaderBlockContentIdProperty*)res.get())->init(__OD_T("BlockContentId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMLeaderBlockContentIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderBlockContentIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->blockContentId());
  return eOk;
}

OdResult OdDbMLeaderBlockContentIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockContentId(val);
  return eOk;
}

/* OdDbMLeaderBlockColorProperty */

struct OdDbMLeaderBlockColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderBlockColorProperty>::createObject();
    ((OdDbMLeaderBlockColorProperty*)res.get())->init(__OD_T("BlockColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderBlockColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->blockColor());
  return eOk;
}

OdResult OdDbMLeaderBlockColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBlockColor(val);
  return eOk;
}

/* OdDbMLeaderScaleFactorProperty */

struct OdDbMLeaderScaleFactorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderScaleFactorProperty>::createObject();
    ((OdDbMLeaderScaleFactorProperty*)res.get())->init(__OD_T("ScaleFactor"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMLeaderScaleFactorProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderScaleFactorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Overall scale"));
    ((OdDbMLeaderScaleFactorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the overall scale factor of this multileader object"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbMLeaderScaleFactorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 1));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderScaleFactorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->scale());
  return eOk;
}

OdResult OdDbMLeaderScaleFactorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  OdResult rc = pObj->setScale(val);
  return rc;
}

/* OdDbMLeaderMTextProperty */

struct OdDbMLeaderMTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderMTextProperty>::createObject();
    ((OdDbMLeaderMTextProperty*)res.get())->init(__OD_T("MText"), &OdRxValueType::Desc<OdDbMTextPtr>::value(), owner);
    return res;
  }
  virtual OdResult subChildren(OdArray<OdRxMemberPtr>& children) const
  {
    OdRxMemberPtr res;
    res = OdRxSubProperty::createObject(L"Contents", OdRxValueType::Desc<OdString>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text string of the Mtext"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 1));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Contents"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextString"));
    res = OdRxSubProperty::createObject(L"TextStyleId", OdRxValueType::Desc<OdDbObjectId>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/TextStyleTableId/Items"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the style name of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 5));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text style"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextStyleName"));
    res = OdRxSubProperty::createObject(L"FlowDirection", OdRxValueType::Desc<OdDbMText::FlowDirection>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the drawing direction of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 15));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Direction"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextDirection"));
    res = OdRxSubProperty::createObject(L"Width", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the defined width of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 60));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Width"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextWidth"));
    res = OdRxSubProperty::createObject(L"TextHeight", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text height of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 61));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Height"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextHeight"));
    res = OdRxSubProperty::createObject(L"Rotation", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rotation angle of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 30));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotation"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextRotation"));
    res = OdRxSubProperty::createObject(L"LineSpacingFactor", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the line spacing factor of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 40));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Line space factor"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextLineSpacingFactor"));
    res = OdRxSubProperty::createObject(L"LineSpaceDistance", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the line spacing distance of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 45));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Line space distance"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextLineSpacingDistance"));
    res = OdRxSubProperty::createObject(L"LineSpacingStyle", OdRxValueType::Desc<OdDb::LineSpacingStyle>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the line spacing style of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 50));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Line space style"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextLineSpacingStyle"));
    res = OdRxSubProperty::createObject(L"BackgroundFill", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies use Background mask"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 55));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Background mask"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextBackgroundFill"));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderMTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_NAME
  OdDbMTextPtr pText = static_cast<OdDbMTextPtr>(pObj->mtext());
  if (pText.isNull())
    return eNullObjectId; // possible if pObj->contentType() != OdDbMLeaderStyle::kMTextContent
  value = pText;
  return eOk;
}

OdResult OdDbMLeaderMTextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMTextPtr val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setMText(val);
  return eOk;
}

/* OdDbMLeaderTextLeftAttachmentTypeProperty */

//COMMENT: TODO: Check formula
struct OdDbMLeaderTextLeftAttachmentTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextLeftAttachmentTypeProperty>::createObject();
    ((OdDbMLeaderTextLeftAttachmentTypeProperty*)res.get())->init(__OD_T("TextLeftAttachmentType"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAttachmentType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderTextLeftAttachmentTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Left Attachment"));
    ((OdDbMLeaderTextLeftAttachmentTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify how leaders on the left side connect with the text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMLeaderTextLeftAttachmentTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 65));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderTextLeftAttachmentTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kMTextContent)
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->textAttachmentType(OdDbMLeaderStyle::kLeftLeader);
  return eOk;
}

OdResult OdDbMLeaderTextLeftAttachmentTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAttachmentType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setTextAttachmentType(val, OdDbMLeaderStyle::kLeftLeader);
  return eOk;
}

/* OdDbMLeaderTextRightAttachmentTypeProperty */

struct OdDbMLeaderTextRightAttachmentTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextRightAttachmentTypeProperty>::createObject();
    ((OdDbMLeaderTextRightAttachmentTypeProperty*)res.get())->init(__OD_T("TextRightAttachmentType"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAttachmentType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderTextRightAttachmentTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Right Attachment"));
    ((OdDbMLeaderTextRightAttachmentTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Gets the dog leg direction of the specific leader"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMLeaderTextRightAttachmentTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 70));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderTextRightAttachmentTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kMTextContent)
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->textAttachmentType(OdDbMLeaderStyle::kRightLeader);
  return eOk;
}

OdResult OdDbMLeaderTextRightAttachmentTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAttachmentType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setTextAttachmentType(val, OdDbMLeaderStyle::kRightLeader);
  return eOk;
}

/* OdDbMLeaderTextTopAttachmentTypeProperty */

struct OdDbMLeaderTextTopAttachmentTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextTopAttachmentTypeProperty>::createObject();
    ((OdDbMLeaderTextTopAttachmentTypeProperty*)res.get())->init(__OD_T("TextTopAttachmentType"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAttachmentType>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMLeaderTextTopAttachmentTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 75));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderTextTopAttachmentTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Top Attachment"));
    ((OdDbMLeaderTextTopAttachmentTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies how leaders on the top connect with the text"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderTextTopAttachmentTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kMTextContent)
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->textAttachmentType(OdDbMLeaderStyle::kTopLeader);
  return eOk;
}

OdResult OdDbMLeaderTextTopAttachmentTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAttachmentType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setTextAttachmentType(val, OdDbMLeaderStyle::kTopLeader);
  return eOk;
}

/* OdDbMLeaderTextBottomAttachmentTypeProperty */

struct OdDbMLeaderTextBottomAttachmentTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderTextBottomAttachmentTypeProperty>::createObject();
    ((OdDbMLeaderTextBottomAttachmentTypeProperty*)res.get())->init(__OD_T("TextBottomAttachmentType"), &OdRxValueType::Desc<OdDbMLeaderStyle::TextAttachmentType>::value(), owner);
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMLeaderTextBottomAttachmentTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 80));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderTextBottomAttachmentTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Bottom Attachment"));
    ((OdDbMLeaderTextBottomAttachmentTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies how leaders on the bottom connect with the text"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderTextBottomAttachmentTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull() || pObj->contentType() != OdDbMLeaderStyle::kMTextContent)
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->textAttachmentType(OdDbMLeaderStyle::kBottomLeader);
  return eOk;
}

OdResult OdDbMLeaderTextBottomAttachmentTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMLeaderStyle::TextAttachmentType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  pObj->setTextAttachmentType(val, OdDbMLeaderStyle::kBottomLeader);
  return eOk;
}

/* OdDbMLeaderExtendLeaderToTextProperty */

struct OdDbMLeaderExtendLeaderToTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMLeaderExtendLeaderToTextProperty>::createObject();
    ((OdDbMLeaderExtendLeaderToTextProperty*)res.get())->init(__OD_T("ExtendLeaderToText"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMLeaderExtendLeaderToTextProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Leader extension"));
    ((OdDbMLeaderExtendLeaderToTextProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Extends the horizontal multiline leader to the multiline text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Leaders"))
      OdRxCategory::createObject(L"Leaders", OdRxCategory::rootCategory());
    ((OdDbMLeaderExtendLeaderToTextProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Leaders", 0));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMLeaderExtendLeaderToTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->extendLeaderToText());
  return eOk;
}

OdResult OdDbMLeaderExtendLeaderToTextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMLeaderPtr pObj = OdDbMLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setExtendLeaderToText(val);
  return eOk;
}

void createOdDbMLeaderProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbMLeaderDoglegLengthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderArrowSymbolIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderArrowheadTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderEnableLandingProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderEnableDoglegProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextStyleIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderArrowSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderContentTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderLeaderLineCountProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderLeaderLineTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderLeaderCountProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderLeaderLineWeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderLandingGapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderLeaderLineColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderLeaderLineTypeIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextAngleTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderMLeaderStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderBlockPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderBlockConnectionTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderEnableAnnotationScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextAttachmentDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextLocationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderToleranceLocationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderScaledTextHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderEnableFrameTextProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextAlignmentTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderBlockScaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderBlockRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderBlockContentIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderBlockColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderScaleFactorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderMTextProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextLeftAttachmentTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextRightAttachmentTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextTopAttachmentTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderTextBottomAttachmentTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMLeaderExtendLeaderToTextProperty::createObject(b.owner()));
  b.add(properties.last());
}
