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
#include "DbGeoPositionMarker.h"
#include "DbValueTypes.h"

/* OdDbGeoPositionMarkerPositionProperty */

struct OdDbGeoPositionMarkerPositionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGeoPositionMarkerPositionProperty>::createObject();
    ((OdDbGeoPositionMarkerPositionProperty*)res.get())->init(__OD_T("Position"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbGeoPositionMarkerPositionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position X;Position Y;Position Z"));
    ((OdDbGeoPositionMarkerPositionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Position of the marker"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbGeoPositionMarkerPositionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    ((OdDbGeoPositionMarkerPositionProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Position"));
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

OdResult OdDbGeoPositionMarkerPositionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->position());
  return eOk;
}

OdResult OdDbGeoPositionMarkerPositionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setPosition(val);
  return rc;
}

/* OdDbGeoPositionMarkerRadiusProperty */

struct OdDbGeoPositionMarkerRadiusProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGeoPositionMarkerRadiusProperty>::createObject();
    ((OdDbGeoPositionMarkerRadiusProperty*)res.get())->init(__OD_T("Radius"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbGeoPositionMarkerRadiusProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbGeoPositionMarkerRadiusProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Size"));
    ((OdDbGeoPositionMarkerRadiusProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Radius of the marker"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbGeoPositionMarkerRadiusProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    ((OdDbGeoPositionMarkerRadiusProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Radius"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbGeoPositionMarkerRadiusProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->radius());
  return eOk;
}

OdResult OdDbGeoPositionMarkerRadiusProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setRadius(val);
  return rc;
}

/* OdDbGeoPositionMarkerLandingGapProperty */

struct OdDbGeoPositionMarkerLandingGapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGeoPositionMarkerLandingGapProperty>::createObject();
    ((OdDbGeoPositionMarkerLandingGapProperty*)res.get())->init(__OD_T("LandingGap"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbGeoPositionMarkerLandingGapProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbGeoPositionMarkerLandingGapProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Landing gap"));
    ((OdDbGeoPositionMarkerLandingGapProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the text landing gap"));
    ((OdDbGeoPositionMarkerLandingGapProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LandingGap"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbGeoPositionMarkerLandingGapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>STATIC_CAST
  value = static_cast<double>(pObj->landingGap());
  return eOk;
}

OdResult OdDbGeoPositionMarkerLandingGapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setLandingGap(val);
  return rc;
}

/* OdDbGeoPositionMarkerMTextProperty */

struct OdDbGeoPositionMarkerMTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGeoPositionMarkerMTextProperty>::createObject();
    ((OdDbGeoPositionMarkerMTextProperty*)res.get())->init(__OD_T("MText"), &OdRxValueType::Desc<OdDbMTextPtr>::value(), owner);
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
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LineSpacingFactor"));
    res = OdRxSubProperty::createObject(L"LineSpaceDistance", OdRxValueType::Desc<double>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the line spacing distance of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 45));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Line space distance"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LineSpacingDistance"));
    res = OdRxSubProperty::createObject(L"LineSpacingStyle", OdRxValueType::Desc<OdDb::LineSpacingStyle>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the line spacing style of the Mtext"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 50));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Line space style"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"LineSpacingStyle"));
    res = OdRxSubProperty::createObject(L"BackgroundFill", OdRxValueType::Desc<bool>::value(), this);
    children.append(res);
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies use Background mask"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 55));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdRxSubProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Background mask"));
    ((OdRxSubProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"BackgroundFill"));
    return eOk;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbGeoPositionMarkerMTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbMTextPtr>(pObj->mtext());
  return eOk;
}

OdResult OdDbGeoPositionMarkerMTextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMTextPtr val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setMText(val);
  return rc;
}

/* OdDbGeoPositionMarkerAttachmentProperty */

//COMMENT: TODO: Changed type TextAlignmentType -> OdTextAlignmentType
struct OdDbGeoPositionMarkerAttachmentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGeoPositionMarkerAttachmentProperty>::createObject();
    ((OdDbGeoPositionMarkerAttachmentProperty*)res.get())->init(__OD_T("Attachment"), &OdRxValueType::Desc<OdDbGeoPositionMarker::OdTextAlignmentType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbGeoPositionMarkerAttachmentProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Justify"));
    ((OdDbGeoPositionMarkerAttachmentProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies both text height and text orientation by designating the endpoints of the baseline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbGeoPositionMarkerAttachmentProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 10));
    ((OdDbGeoPositionMarkerAttachmentProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextJustify"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbGeoPositionMarkerAttachmentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbGeoPositionMarker::OdTextAlignmentType>(pObj->textAlignmentType());
  return eOk;
}

OdResult OdDbGeoPositionMarkerAttachmentProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbGeoPositionMarker::OdTextAlignmentType val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  OdResult rc = pObj->setTextAlignmentType(val);
  return rc;
}

/* OdDbGeoPositionMarkerEnableFrameTextProperty */

struct OdDbGeoPositionMarkerEnableFrameTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbGeoPositionMarkerEnableFrameTextProperty>::createObject();
    ((OdDbGeoPositionMarkerEnableFrameTextProperty*)res.get())->init(__OD_T("EnableFrameText"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbGeoPositionMarkerEnableFrameTextProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text frame"));
    ((OdDbGeoPositionMarkerEnableFrameTextProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Display/hide text frame of multileader content"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbGeoPositionMarkerEnableFrameTextProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 90));
    ((OdDbGeoPositionMarkerEnableFrameTextProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextFrameDisplay"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbGeoPositionMarkerEnableFrameTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->enableFrameText());
  return eOk;
}

OdResult OdDbGeoPositionMarkerEnableFrameTextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbGeoPositionMarkerPtr pObj = OdDbGeoPositionMarker::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  OdResult rc = pObj->setEnableFrameText(val);
  return rc;
}

void createOdDbGeoPositionMarkerProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbGeoPositionMarkerPositionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbGeoPositionMarkerRadiusProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbGeoPositionMarkerLandingGapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbGeoPositionMarkerMTextProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbGeoPositionMarkerAttachmentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbGeoPositionMarkerEnableFrameTextProperty::createObject(b.owner()));
  b.add(properties.last());
}
