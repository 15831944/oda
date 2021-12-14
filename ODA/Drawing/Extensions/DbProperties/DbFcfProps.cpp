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
#include "DbFcf.h"
#include "DbValueTypes.h"

/* OdDbFcfDimgapProperty */

struct OdDbFcfDimgapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfDimgapProperty>::createObject();
    ((OdDbFcfDimgapProperty*)res.get())->init(__OD_T("Dimgap"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbFcfDimgapProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFcfDimgapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimgap());
  return eOk;
}

OdResult OdDbFcfDimgapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimgap(val);
  return eOk;
}

/* OdDbFcfDimscaleProperty */

struct OdDbFcfDimscaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfDimscaleProperty>::createObject();
    ((OdDbFcfDimscaleProperty*)res.get())->init(__OD_T("Dimscale"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbFcfDimscaleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFcfDimscaleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim scale overall"));
    ((OdDbFcfDimscaleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the overall scale factor applied to properties that specify sizes, distances, or offsets"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Fit"))
      OdRxCategory::createObject(L"Fit", OdRxCategory::rootCategory());
    ((OdDbFcfDimscaleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Fit", 3));
    ((OdDbFcfDimscaleProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ScaleFactor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFcfDimscaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimscale());
  return eOk;
}

OdResult OdDbFcfDimscaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimscale(val);
  return eOk;
}

/* OdDbFcfDimclrdProperty */

struct OdDbFcfDimclrdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfDimclrdProperty>::createObject();
    ((OdDbFcfDimclrdProperty*)res.get())->init(__OD_T("Dimclrd"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFcfDimclrdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim line color"));
    ((OdDbFcfDimclrdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the color of the dimension lines"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbFcfDimclrdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 4));
    ((OdDbFcfDimclrdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DimensionLineColor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFcfDimclrdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimclrd());
  return eOk;
}

OdResult OdDbFcfDimclrdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimclrd(val);
  return eOk;
}

/* OdDbFcfDimclrtProperty */

struct OdDbFcfDimclrtProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfDimclrtProperty>::createObject();
    ((OdDbFcfDimclrtProperty*)res.get())->init(__OD_T("Dimclrt"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFcfDimclrtProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text color"));
    ((OdDbFcfDimclrtProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the color of the dimension text"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbFcfDimclrtProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 5));
    ((OdDbFcfDimclrtProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextColor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFcfDimclrtProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimclrt());
  return eOk;
}

OdResult OdDbFcfDimclrtProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimclrt(val);
  return eOk;
}

/* OdDbFcfDimtxstyProperty */

struct OdDbFcfDimtxstyProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfDimtxstyProperty>::createObject();
    ((OdDbFcfDimtxstyProperty*)res.get())->init(__OD_T("Dimtxsty"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbFcfDimtxstyProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFcfDimtxstyProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimtxsty());
  return eOk;
}

OdResult OdDbFcfDimtxstyProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtxsty(val);
  return eOk;
}

/* OdDbFcfDimtxtProperty */

struct OdDbFcfDimtxtProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfDimtxtProperty>::createObject();
    ((OdDbFcfDimtxtProperty*)res.get())->init(__OD_T("Dimtxt"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbFcfDimtxtProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFcfDimtxtProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text height"));
    ((OdDbFcfDimtxtProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text height of the tolerance"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbFcfDimtxtProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 6));
    ((OdDbFcfDimtxtProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextHeight"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFcfDimtxtProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtxt());
  return eOk;
}

OdResult OdDbFcfDimtxtProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtxt(val);
  return eOk;
}

/* OdDbFcfNormalProperty */

struct OdDbFcfNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfNormalProperty>::createObject();
    ((OdDbFcfNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbFcfNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

/* OdDbFcfDirectionProperty */

struct OdDbFcfDirectionProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfDirectionProperty>::createObject();
    ((OdDbFcfDirectionProperty*)res.get())->init(__OD_T("Direction"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbFcfDirectionProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->direction());
  return eOk;
}

/* OdDbFcfLocationProperty */

struct OdDbFcfLocationProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfLocationProperty>::createObject();
    ((OdDbFcfLocationProperty*)res.get())->init(__OD_T("Location"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFcfLocationProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Position X;Position Y;Position Z"));
    ((OdDbFcfLocationProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specify the X, Y, Z coordinate for insertion point of the tolerance or use the Pick Point button to set X, Y, Z values simultaneously"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbFcfLocationProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 1));
    ((OdDbFcfLocationProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"InsertionPoint"));
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

OdResult OdDbFcfLocationProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->location());
  return eOk;
}

OdResult OdDbFcfLocationProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setLocation(val);
  return eOk;
}

/* OdDbFcfTextProperty */

struct OdDbFcfTextProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfTextProperty>::createObject();
    ((OdDbFcfTextProperty*)res.get())->init(__OD_T("Text"), &OdRxValueType::Desc<OdString>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFcfTextProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text override"));
    ((OdDbFcfTextProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text string of the tolerance"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbFcfTextProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 9));
    ((OdDbFcfTextProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextString"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFcfTextProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdString>(pObj->text());
  return eOk;
}

OdResult OdDbFcfTextProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdString val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setText(val);
  return eOk;
}

/* OdDbFcfTextStyleIdProperty */

struct OdDbFcfTextStyleIdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfTextStyleIdProperty>::createObject();
    ((OdDbFcfTextStyleIdProperty*)res.get())->init(__OD_T("TextStyleId"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbFcfTextStyleIdProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/TextStyleTableId/Items"));
    ((OdDbFcfTextStyleIdProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFcfTextStyleIdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text style"));
    ((OdDbFcfTextStyleIdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the text style of the tolerance"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbFcfTextStyleIdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 8));
    ((OdDbFcfTextStyleIdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextStyle"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFcfTextStyleIdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_FNAME
  value = static_cast<OdDbObjectId>(pObj->dimtxsty());
  return eOk;
}

OdResult OdDbFcfTextStyleIdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setDimtxsty(val);
  return eOk;
}

/* OdDbFcfDimensionStyleProperty */

struct OdDbFcfDimensionStyleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFcfDimensionStyleProperty>::createObject();
    ((OdDbFcfDimensionStyleProperty*)res.get())->init(__OD_T("DimensionStyle"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbFcfDimensionStyleProperty*)res.get())->attributes().add(OdRxRefersToAttribute::createObject(L"/DimStyleTableId/Items"));
    ((OdDbFcfDimensionStyleProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFcfDimensionStyleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim style"));
    ((OdDbFcfDimensionStyleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the style name of the tolerance"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbFcfDimensionStyleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 2));
    ((OdDbFcfDimensionStyleProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"StyleName"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFcfDimensionStyleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>STATIC_CAST
  value = static_cast<OdDbObjectId>(pObj->dimensionStyle());
  return eOk;
}

OdResult OdDbFcfDimensionStyleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFcfPtr pObj = OdDbFcf::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimensionStyle(val);
  return eOk;
}

void createOdDbFcfProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbFcfDimgapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfDimscaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfDimclrdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfDimclrtProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfDimtxstyProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfDimtxtProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfDirectionProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfLocationProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfTextProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfTextStyleIdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFcfDimensionStyleProperty::createObject(b.owner()));
  b.add(properties.last());
}
