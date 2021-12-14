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
#include "DbLeader.h"
#include "DbValueTypes.h"

/* OdDbLeaderAnnoHeightProperty */

struct OdDbLeaderAnnoHeightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderAnnoHeightProperty>::createObject();
    ((OdDbLeaderAnnoHeightProperty*)res.get())->init(__OD_T("AnnoHeight"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLeaderAnnoHeightProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLeaderAnnoHeightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->annoHeight());
  return eOk;
}

/* OdDbLeaderAnnoTypeProperty */

struct OdDbLeaderAnnoTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderAnnoTypeProperty>::createObject();
    ((OdDbLeaderAnnoTypeProperty*)res.get())->init(__OD_T("AnnoType"), &OdRxValueType::Desc<OdDbLeader::AnnoType>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLeaderAnnoTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbLeader::AnnoType>(pObj->annoType());
  return eOk;
}

/* OdDbLeaderAnnoWidthProperty */

struct OdDbLeaderAnnoWidthProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderAnnoWidthProperty>::createObject();
    ((OdDbLeaderAnnoWidthProperty*)res.get())->init(__OD_T("AnnoWidth"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLeaderAnnoWidthProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLeaderAnnoWidthProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->annoWidth());
  return eOk;
}

/* OdDbLeaderNormalProperty */

struct OdDbLeaderNormalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderNormalProperty>::createObject();
    ((OdDbLeaderNormalProperty*)res.get())->init(__OD_T("Normal"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbLeaderNormalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->normal());
  return eOk;
}

/* OdDbLeaderHasHookLineProperty */

struct OdDbLeaderHasHookLineProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderHasHookLineProperty>::createObject();
    ((OdDbLeaderHasHookLineProperty*)res.get())->init(__OD_T("HasHookLine"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLeaderHasHookLineProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasHookLine());
  return eOk;
}

/* OdDbLeaderNumVerticesProperty */

struct OdDbLeaderNumVerticesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderNumVerticesProperty>::createObject();
    ((OdDbLeaderNumVerticesProperty*)res.get())->init(__OD_T("NumVertices"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbLeaderNumVerticesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numVertices());
  return eOk;
}

/* OdDbLeaderFirstVertexProperty */

struct OdDbLeaderFirstVertexProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderFirstVertexProperty>::createObject();
    ((OdDbLeaderFirstVertexProperty*)res.get())->init(__OD_T("FirstVertex"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbLeaderFirstVertexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->firstVertex());
  return eOk;
}

/* OdDbLeaderLastVertexProperty */

struct OdDbLeaderLastVertexProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderLastVertexProperty>::createObject();
    ((OdDbLeaderLastVertexProperty*)res.get())->init(__OD_T("LastVertex"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
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

OdResult OdDbLeaderLastVertexProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGePoint3d>(pObj->lastVertex());
  return eOk;
}

/* OdDbLeaderDimscaleProperty */

struct OdDbLeaderDimscaleProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderDimscaleProperty>::createObject();
    ((OdDbLeaderDimscaleProperty*)res.get())->init(__OD_T("Dimscale"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLeaderDimscaleProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(0)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLeaderDimscaleProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim scale overall"));
    ((OdDbLeaderDimscaleProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the overall scale factor applied to properties that specify sizes, distances, or offsets"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Fit"))
      OdRxCategory::createObject(L"Fit", OdRxCategory::rootCategory());
    ((OdDbLeaderDimscaleProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Fit", 4));
    ((OdDbLeaderDimscaleProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ScaleFactor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderDimscaleProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimscale());
  return eOk;
}

OdResult OdDbLeaderDimscaleProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimscale(val);
  return eOk;
}

/* OdDbLeaderHasArrowHeadProperty */

struct OdDbLeaderHasArrowHeadProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderHasArrowHeadProperty>::createObject();
    ((OdDbLeaderHasArrowHeadProperty*)res.get())->init(__OD_T("HasArrowHead"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderHasArrowHeadProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->hasArrowHead());
  return eOk;
}

OdResult OdDbLeaderHasArrowHeadProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->enableArrowHead(); else pObj->disableArrowHead();
  return eOk;
}

/* OdDbLeaderIsSplinedProperty */

struct OdDbLeaderIsSplinedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderIsSplinedProperty>::createObject();
    ((OdDbLeaderIsSplinedProperty*)res.get())->init(__OD_T("IsSplined"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderIsSplinedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isSplined());
  return eOk;
}

OdResult OdDbLeaderIsSplinedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->setToSplineLeader(); else pObj->setToStraightLeader();
  return eOk;
}

/* OdDbLeaderDimtxtProperty */

struct OdDbLeaderDimtxtProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderDimtxtProperty>::createObject();
    ((OdDbLeaderDimtxtProperty*)res.get())->init(__OD_T("Dimtxt"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLeaderDimtxtProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderDimtxtProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimtxt());
  return eOk;
}

OdResult OdDbLeaderDimtxtProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtxt(val);
  return eOk;
}

/* OdDbLeaderDimtadProperty */

struct OdDbLeaderDimtadProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderDimtadProperty>::createObject();
    ((OdDbLeaderDimtadProperty*)res.get())->init(__OD_T("Dimtad"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLeaderDimtadProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text pos vert"));
    ((OdDbLeaderDimtadProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the vertical dimension text position"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbLeaderDimtadProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 10));
    ((OdDbLeaderDimtadProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"VerticalTextPosition"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderDimtadProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->dimtad());
  return eOk;
}

OdResult OdDbLeaderDimtadProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  int val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimtad(val);
  return eOk;
}

/* OdDbLeaderDimaszProperty */

struct OdDbLeaderDimaszProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderDimaszProperty>::createObject();
    ((OdDbLeaderDimaszProperty*)res.get())->init(__OD_T("Dimasz"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLeaderDimaszProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLeaderDimaszProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Arrow size"));
    ((OdDbLeaderDimaszProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the size of the leader arrowhead"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbLeaderDimaszProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 6));
    ((OdDbLeaderDimaszProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"ArrowheadSize"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderDimaszProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimasz());
  return eOk;
}

OdResult OdDbLeaderDimaszProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimasz(val);
  return eOk;
}

/* OdDbLeaderDimlwdProperty */

struct OdDbLeaderDimlwdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderDimlwdProperty>::createObject();
    ((OdDbLeaderDimlwdProperty*)res.get())->init(__OD_T("Dimlwd"), &OdRxValueType::Desc<OdDb::LineWeight>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLeaderDimlwdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim line lineweight"));
    ((OdDbLeaderDimlwdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the lineweight of the leader line"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbLeaderDimlwdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 7));
    ((OdDbLeaderDimlwdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DimensionLineWeight"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderDimlwdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::LineWeight>(pObj->dimlwd());
  return eOk;
}

OdResult OdDbLeaderDimlwdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::LineWeight val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimlwd(val);
  return eOk;
}

/* OdDbLeaderDimgapProperty */

struct OdDbLeaderDimgapProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderDimgapProperty>::createObject();
    ((OdDbLeaderDimgapProperty*)res.get())->init(__OD_T("Dimgap"), &OdRxValueType::Desc<double>::value(), owner);
    ((OdDbLeaderDimgapProperty*)res.get())->attributes().add(OdRxUnitTypeAttribute::createObject(static_cast<OdRxUnitTypeAttribute::UnitType>(1)));
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLeaderDimgapProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Text offset"));
    ((OdDbLeaderDimgapProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the distance around the dimension text that the dimension line is broken"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Text"))
      OdRxCategory::createObject(L"Text", OdRxCategory::rootCategory());
    ((OdDbLeaderDimgapProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Text", 9));
    ((OdDbLeaderDimgapProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"TextGap"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderDimgapProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<double>(pObj->dimgap());
  return eOk;
}

OdResult OdDbLeaderDimgapProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  double val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimgap(val);
  return eOk;
}

/* OdDbLeaderDimclrdProperty */

struct OdDbLeaderDimclrdProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderDimclrdProperty>::createObject();
    ((OdDbLeaderDimclrdProperty*)res.get())->init(__OD_T("Dimclrd"), &OdRxValueType::Desc<OdCmColor>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbLeaderDimclrdProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Dim line color"));
    ((OdDbLeaderDimclrdProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the color of the leader lines"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Lines & Arrows"))
      OdRxCategory::createObject(L"Lines & Arrows", OdRxCategory::rootCategory());
    ((OdDbLeaderDimclrdProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Lines & Arrows", 8));
    ((OdDbLeaderDimclrdProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"DimensionLineColor"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderDimclrdProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdCmColor>(pObj->dimclrd());
  return eOk;
}

OdResult OdDbLeaderDimclrdProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdCmColor val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimclrd(val);
  return eOk;
}

/* OdDbLeaderAnnotationOffsetProperty */

struct OdDbLeaderAnnotationOffsetProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderAnnotationOffsetProperty>::createObject();
    ((OdDbLeaderAnnotationOffsetProperty*)res.get())->init(__OD_T("AnnotationOffset"), &OdRxValueType::Desc<OdGeVector3d>::value(), owner);
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

OdResult OdDbLeaderAnnotationOffsetProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdGeVector3d>(pObj->annotationOffset());
  return eOk;
}

OdResult OdDbLeaderAnnotationOffsetProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGeVector3d val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setAnnotationOffset(val);
  return eOk;
}

/* OdDbLeaderDimldrblkProperty */

struct OdDbLeaderDimldrblkProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderDimldrblkProperty>::createObject();
    ((OdDbLeaderDimldrblkProperty*)res.get())->init(__OD_T("Dimldrblk"), &OdRxValueType::Desc<OdDbObjectId>::value(), owner);
    ((OdDbLeaderDimldrblkProperty*)res.get())->attributes().add(OdRxDisplayAsAttribute::createObject(L"Name"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderDimldrblkProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDbObjectId>(pObj->dimldrblk());
  return eOk;
}

OdResult OdDbLeaderDimldrblkProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDbObjectId val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimldrblk(val);
  return eOk;
}

/* OdDbLeaderDimsahProperty */

struct OdDbLeaderDimsahProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbLeaderDimsahProperty>::createObject();
    ((OdDbLeaderDimsahProperty*)res.get())->init(__OD_T("Dimsah"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbLeaderDimsahProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->dimsah());
  return eOk;
}

OdResult OdDbLeaderDimsahProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbLeaderPtr pObj = OdDbLeader::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setDimsah(val);
  return eOk;
}

void createOdDbLeaderProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbLeaderAnnoHeightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderAnnoTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderAnnoWidthProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderNormalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderHasHookLineProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderNumVerticesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderFirstVertexProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderLastVertexProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderDimscaleProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderHasArrowHeadProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderIsSplinedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderDimtxtProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderDimtadProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderDimaszProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderDimlwdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderDimgapProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderDimclrdProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderAnnotationOffsetProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderDimldrblkProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbLeaderDimsahProperty::createObject(b.owner()));
  b.add(properties.last());
}
