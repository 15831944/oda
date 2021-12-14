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
#include "DbMText.h"
#include "DbValueTypes.h"

/* OdDbMTextAttachmentProperty */

struct OdDbMTextAttachmentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextAttachmentProperty>::createObject();
    ((OdDbMTextAttachmentProperty*)res.get())->init(__OD_T("Attachment"), &OdRxValueType::Desc<OdDbMText::AttachmentPoint>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextAttachmentProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Justify"));
    ((OdDbMTextAttachmentProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies both text height and text orientation by designating the endpoints of the baseline"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextAttachmentProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 10));
    ((OdDbMTextAttachmentProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"AttachmentPoint"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextAttachmentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMText::AttachmentPoint>(pObj->attachment());
  return eOk;
}

OdResult OdDbMTextAttachmentProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMText::AttachmentPoint val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAttachment(val);
  return eOk;
}

/* OdDbMTextTextHeightProperty */

struct OdDbMTextTextHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextTextHeightProperty>::createObject();
    ((OdDbMTextTextHeightProperty*)res.get())->init(__OD_T("TextHeight"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextTextHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextTextHeightProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text height"));
    ((OdDbMTextTextHeightProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text height of the Mtext"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextTextHeightProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 61));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextTextHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->textHeight());
  return eOk;
}

OdResult OdDbMTextTextHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setTextHeight(val);
  return eOk;
}

/* OdDbMTextFlowDirectionProperty */

struct OdDbMTextFlowDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextFlowDirectionProperty>::createObject();
    ((OdDbMTextFlowDirectionProperty*)res.get())->init(__OD_T("FlowDirection"), &OdRxValueType::Desc<OdDbMText::FlowDirection>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextFlowDirectionProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Direction"));
    ((OdDbMTextFlowDirectionProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the drawing direction of the Mtext"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextFlowDirectionProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 15));
    ((OdDbMTextFlowDirectionProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DrawingDirection"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextFlowDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbMText::FlowDirection>(pObj->flowDirection());
  return eOk;
}

OdResult OdDbMTextFlowDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMText::FlowDirection val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setFlowDirection(val);
  return eOk;
}

/* OdDbMTextActualHeightProperty */

struct OdDbMTextActualHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextActualHeightProperty>::createObject();
    ((OdDbMTextActualHeightProperty*)res.get())->init(__OD_T("ActualHeight"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextActualHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbMTextActualHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->actualHeight());
  return eOk;
}

/* OdDbMTextTextProperty */

struct OdDbMTextTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextTextProperty>::createObject();
    ((OdDbMTextTextProperty*)res.get())->init(__OD_T("Text"), &OdRxValueType::Desc<OdString>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbMTextTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->text());
  return eOk;
}

/* OdDbMTextContentsProperty */

struct OdDbMTextContentsProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextContentsProperty>::createObject();
    ((OdDbMTextContentsProperty*)res.get())->init(__OD_T("Contents"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextContentsProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Contents"));
    ((OdDbMTextContentsProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text string of the Mtext"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextContentsProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 1));
    ((OdDbMTextContentsProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbMTextContentsProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextString"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextContentsProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->contents());
  return eOk;
}

OdResult OdDbMTextContentsProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setContents(val);
  return eOk;
}

/* OdDbMTextTextStyleIdProperty */

struct OdDbMTextTextStyleIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextTextStyleIdProperty>::createObject();
    ((OdDbMTextTextStyleIdProperty*)res.get())->init(__OD_T("TextStyleId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbMTextTextStyleIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/TextStyleTableId/Items"));
    ((OdDbMTextTextStyleIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextTextStyleIdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Style"));
    ((OdDbMTextTextStyleIdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the style name of the Mtext"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextTextStyleIdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 5));
    ((OdDbMTextTextStyleIdProperty*)res.get())->attributes().add(OdRxLMVAttribute::createObject());
    ((OdDbMTextTextStyleIdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"StyleName"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextTextStyleIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->textStyle());
  return eOk;
}

OdResult OdDbMTextTextStyleIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setTextStyle(val);
  return eOk;
}

/* OdDbMTextDirectionProperty */

struct OdDbMTextDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextDirectionProperty>::createObject();
    ((OdDbMTextDirectionProperty*)res.get())->init(__OD_T("Direction"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbMTextDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->direction());
  return eOk;
}

OdResult OdDbMTextDirectionProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDirection(val);
  return eOk;
}

/* OdDbMTextNormalProperty */

struct OdDbMTextNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextNormalProperty>::createObject();
    ((OdDbMTextNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbMTextNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

OdResult OdDbMTextNormalProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNormal(val);
  return eOk;
}

/* OdDbMTextLocationProperty */

struct OdDbMTextLocationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextLocationProperty>::createObject();
    ((OdDbMTextLocationProperty*)res.get())->init(__OD_T("Location"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextLocationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position X;Position Y;Position Z"));
    ((OdDbMTextLocationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for the insertion point of the Mtext or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbMTextLocationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 35));
    ((OdDbMTextLocationProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsertionPoint"));
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

OdResult OdDbMTextLocationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->location());
  return eOk;
}

OdResult OdDbMTextLocationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLocation(val);
  return eOk;
}

/* OdDbMTextRotationProperty */

struct OdDbMTextRotationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextRotationProperty>::createObject();
    ((OdDbMTextRotationProperty*)res.get())->init(__OD_T("Rotation"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextRotationProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(2)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextRotationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Rotation"));
    ((OdDbMTextRotationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the rotation angle of the Mtext"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextRotationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 30));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextRotationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->rotation());
  return eOk;
}

OdResult OdDbMTextRotationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setRotation(val);
  return eOk;
}

/* OdDbMTextDescentProperty */

struct OdDbMTextDescentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextDescentProperty>::createObject();
    ((OdDbMTextDescentProperty*)res.get())->init(__OD_T("Descent"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextDescentProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbMTextDescentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->descent());
  return eOk;
}

/* OdDbMTextAscentProperty */

struct OdDbMTextAscentProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextAscentProperty>::createObject();
    ((OdDbMTextAscentProperty*)res.get())->init(__OD_T("Ascent"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextAscentProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbMTextAscentProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->ascent());
  return eOk;
}

/* OdDbMTextWidthProperty */

struct OdDbMTextWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextWidthProperty>::createObject();
    ((OdDbMTextWidthProperty*)res.get())->init(__OD_T("Width"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextWidthProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Defined width"));
    ((OdDbMTextWidthProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the defined width of the Mtext"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextWidthProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 60));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->width());
  return eOk;
}

OdResult OdDbMTextWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setWidth(val);
  return eOk;
}

/* OdDbMTextActualWidthProperty */

struct OdDbMTextActualWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextActualWidthProperty>::createObject();
    ((OdDbMTextActualWidthProperty*)res.get())->init(__OD_T("ActualWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextActualWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbMTextActualWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->actualWidth());
  return eOk;
}

/* OdDbMTextColumnAutoHeightProperty */

struct OdDbMTextColumnAutoHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextColumnAutoHeightProperty>::createObject();
    ((OdDbMTextColumnAutoHeightProperty*)res.get())->init(__OD_T("ColumnAutoHeight"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextColumnAutoHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->getColumnAutoHeight());
  return eOk;
}

OdResult OdDbMTextColumnAutoHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColumnAutoHeight(val);
  return eOk;
}

/* OdDbMTextColumnCountProperty */

struct OdDbMTextColumnCountProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextColumnCountProperty>::createObject();
    ((OdDbMTextColumnCountProperty*)res.get())->init(__OD_T("ColumnCount"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextColumnCountProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<int>(pObj->getColumnCount());
  return eOk;
}

OdResult OdDbMTextColumnCountProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColumnCount(val);
  return eOk;
}

/* OdDbMTextColumnTypeProperty */

struct OdDbMTextColumnTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextColumnTypeProperty>::createObject();
    ((OdDbMTextColumnTypeProperty*)res.get())->init(__OD_T("ColumnType"), &OdRxValueType::Desc<OdDbMText::ColumnType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextColumnTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Columns"));
    ((OdDbMTextColumnTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the column type of the MText"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextColumnTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 95));
    ((OdDbMTextColumnTypeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Columns"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextColumnTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbMText::ColumnType>(pObj->getColumnType());
  return eOk;
}

OdResult OdDbMTextColumnTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbMText::ColumnType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColumnType(val);
  return eOk;
}

/* OdDbMTextColumnWidthProperty */

struct OdDbMTextColumnWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextColumnWidthProperty>::createObject();
    ((OdDbMTextColumnWidthProperty*)res.get())->init(__OD_T("ColumnWidth"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextColumnWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->getColumnWidth());
  return eOk;
}

OdResult OdDbMTextColumnWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColumnWidth(val);
  return eOk;
}

/* OdDbMTextHeightProperty */

struct OdDbMTextHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextHeightProperty>::createObject();
    ((OdDbMTextHeightProperty*)res.get())->init(__OD_T("Height"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->height());
  return eOk;
}

OdResult OdDbMTextHeightProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setHeight(val);
  return eOk;
}

/* OdDbMTextColumnFlowReversedProperty */

struct OdDbMTextColumnFlowReversedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextColumnFlowReversedProperty>::createObject();
    ((OdDbMTextColumnFlowReversedProperty*)res.get())->init(__OD_T("ColumnFlowReversed"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextColumnFlowReversedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->getColumnFlowReversed());
  return eOk;
}

OdResult OdDbMTextColumnFlowReversedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColumnFlowReversed(val);
  return eOk;
}

/* OdDbMTextColumnGutterWidthProperty */

struct OdDbMTextColumnGutterWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextColumnGutterWidthProperty>::createObject();
    ((OdDbMTextColumnGutterWidthProperty*)res.get())->init(__OD_T("ColumnGutterWidth"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextColumnGutterWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->getColumnGutterWidth());
  return eOk;
}

OdResult OdDbMTextColumnGutterWidthProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setColumnGutterWidth(val);
  return eOk;
}

/* OdDbMTextBackgroundFillProperty */

struct OdDbMTextBackgroundFillProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextBackgroundFillProperty>::createObject();
    ((OdDbMTextBackgroundFillProperty*)res.get())->init(__OD_T("BackgroundFill"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextBackgroundFillProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Background mask"));
    ((OdDbMTextBackgroundFillProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies use Background mask"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextBackgroundFillProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 55));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextBackgroundFillProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->backgroundFillOn());
  return eOk;
}

OdResult OdDbMTextBackgroundFillProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackgroundFill(val);
  return eOk;
}

/* OdDbMTextLineSpacingFactorProperty */

struct OdDbMTextLineSpacingFactorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextLineSpacingFactorProperty>::createObject();
    ((OdDbMTextLineSpacingFactorProperty*)res.get())->init(__OD_T("LineSpacingFactor"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextLineSpacingFactorProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextLineSpacingFactorProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Line space factor"));
    ((OdDbMTextLineSpacingFactorProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the line spacing factor of the Mtext"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextLineSpacingFactorProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 40));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextLineSpacingFactorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->lineSpacingFactor());
  return eOk;
}

OdResult OdDbMTextLineSpacingFactorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLineSpacingFactor(val);
  return eOk;
}

/* OdDbMTextLineSpacingStyleProperty */

struct OdDbMTextLineSpacingStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextLineSpacingStyleProperty>::createObject();
    ((OdDbMTextLineSpacingStyleProperty*)res.get())->init(__OD_T("LineSpacingStyle"), &OdRxValueType::Desc<OdDb::LineSpacingStyle>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextLineSpacingStyleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Line space style"));
    ((OdDbMTextLineSpacingStyleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the line spacing style of the Mtext"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextLineSpacingStyleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 50));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextLineSpacingStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineSpacingStyle>(pObj->lineSpacingStyle());
  return eOk;
}

OdResult OdDbMTextLineSpacingStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineSpacingStyle val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLineSpacingStyle(val);
  return eOk;
}

/* OdDbMTextBackgroundFillColorProperty */

struct OdDbMTextBackgroundFillColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextBackgroundFillColorProperty>::createObject();
    ((OdDbMTextBackgroundFillColorProperty*)res.get())->init(__OD_T("BackgroundFillColor"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextBackgroundFillColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdCmColor>(pObj->getBackgroundFillColor());
  return eOk;
}

OdResult OdDbMTextBackgroundFillColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackgroundFillColor(val);
  return eOk;
}

/* OdDbMTextUseBackgroundColorProperty */

struct OdDbMTextUseBackgroundColorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextUseBackgroundColorProperty>::createObject();
    ((OdDbMTextUseBackgroundColorProperty*)res.get())->init(__OD_T("UseBackgroundColor"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextUseBackgroundColorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<bool>(pObj->useBackgroundColorOn());
  return eOk;
}

OdResult OdDbMTextUseBackgroundColorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setUseBackgroundColor(val);
  return eOk;
}

/* OdDbMTextBackgroundTransparencyProperty */

struct OdDbMTextBackgroundTransparencyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextBackgroundTransparencyProperty>::createObject();
    ((OdDbMTextBackgroundTransparencyProperty*)res.get())->init(__OD_T("BackgroundTransparency"), &OdRxValueType::Desc<OdCmTransparency>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextBackgroundTransparencyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdCmTransparency>(pObj->getBackgroundTransparency());
  return eOk;
}

OdResult OdDbMTextBackgroundTransparencyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmTransparency val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackgroundTransparency(val);
  return eOk;
}

/* OdDbMTextBackgroundScaleFactorProperty */

struct OdDbMTextBackgroundScaleFactorProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextBackgroundScaleFactorProperty>::createObject();
    ((OdDbMTextBackgroundScaleFactorProperty*)res.get())->init(__OD_T("BackgroundScaleFactor"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbMTextBackgroundScaleFactorProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextBackgroundScaleFactorProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<double>(pObj->getBackgroundScaleFactor());
  return eOk;
}

OdResult OdDbMTextBackgroundScaleFactorProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setBackgroundScaleFactor(val);
  return eOk;
}

/* OdDbMTextShowBordersProperty */

struct OdDbMTextShowBordersProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbMTextShowBordersProperty>::createObject();
    ((OdDbMTextShowBordersProperty*)res.get())->init(__OD_T("ShowBorders"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbMTextShowBordersProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text frame"));
    ((OdDbMTextShowBordersProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Display/hide text border of the MText"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbMTextShowBordersProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 100));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbMTextShowBordersProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->showBorders());
  return eOk;
}

OdResult OdDbMTextShowBordersProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbMTextPtr pObj = OdDbMText::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setShowBorders(val);
  return eOk;
}

void createOdDbMTextProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbMTextAttachmentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextTextHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextFlowDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextActualHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextTextProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextContentsProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextTextStyleIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextLocationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextRotationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextDescentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextAscentProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextActualWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextColumnAutoHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextColumnCountProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextColumnTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextColumnWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextColumnFlowReversedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextColumnGutterWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextBackgroundFillProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextLineSpacingFactorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextLineSpacingStyleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextBackgroundFillColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextUseBackgroundColorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextBackgroundTransparencyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextBackgroundScaleFactorProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbMTextShowBordersProperty::createObject(b.owner()));
  b.add(properties.last());
}
