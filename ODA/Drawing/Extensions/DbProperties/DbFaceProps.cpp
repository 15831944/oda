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
#include "DbFace.h"
#include "DbValueTypes.h"

/* OdDbFaceCoordinatesProperty */

struct OdDbFaceCoordinatesProperty : OdRxIndexedProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFaceCoordinatesProperty>::createObject();
    ((OdDbFaceCoordinatesProperty*)res.get())->init(__OD_T("Coordinates"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFaceCoordinatesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Current Vertex"));
    ((OdDbFaceCoordinatesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the current vertex of the 3D face"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFaceCoordinatesProperty::subGetValue(const OdRxObject* pO, int index, OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d item;
  //>>REPLACE_FNAME
  pObj->getVertexAt(index, item);
  value = static_cast<OdGePoint3d>(item);
  return eOk;
}

OdResult OdDbFaceCoordinatesProperty::subSetValue(OdRxObject* pO, int index, const OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdGePoint3d val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_FNAME
  pObj->setVertexAt(index, val);
  return eOk;
}

/* OdDbFaceVisibilityEdge1Property */

struct OdDbFaceVisibilityEdge1Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFaceVisibilityEdge1Property>::createObject();
    ((OdDbFaceVisibilityEdge1Property*)res.get())->init(__OD_T("VisibilityEdge1"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFaceVisibilityEdge1Property*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Display Edge 1"));
    ((OdDbFaceVisibilityEdge1Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether 3DFace Edge 1 is visible or hidden"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbFaceVisibilityEdge1Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 2));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFaceVisibilityEdge1Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->isEdgeVisibleAt(0);
  return eOk;
}

OdResult OdDbFaceVisibilityEdge1Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->makeEdgeVisibleAt(0); else pObj->makeEdgeInvisibleAt(0);
  return eOk;
}

/* OdDbFaceVisibilityEdge2Property */

struct OdDbFaceVisibilityEdge2Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFaceVisibilityEdge2Property>::createObject();
    ((OdDbFaceVisibilityEdge2Property*)res.get())->init(__OD_T("VisibilityEdge2"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFaceVisibilityEdge2Property*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Display Edge 2"));
    ((OdDbFaceVisibilityEdge2Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether 3DFace Edge 2 is visible or hidden"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbFaceVisibilityEdge2Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 3));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFaceVisibilityEdge2Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->isEdgeVisibleAt(1);
  return eOk;
}

OdResult OdDbFaceVisibilityEdge2Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->makeEdgeVisibleAt(1); else pObj->makeEdgeInvisibleAt(1);
  return eOk;
}

/* OdDbFaceVisibilityEdge3Property */

struct OdDbFaceVisibilityEdge3Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFaceVisibilityEdge3Property>::createObject();
    ((OdDbFaceVisibilityEdge3Property*)res.get())->init(__OD_T("VisibilityEdge3"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFaceVisibilityEdge3Property*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Display Edge 3"));
    ((OdDbFaceVisibilityEdge3Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether 3DFace Edge 3 is visible or hidden"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbFaceVisibilityEdge3Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 4));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFaceVisibilityEdge3Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->isEdgeVisibleAt(2);
  return eOk;
}

OdResult OdDbFaceVisibilityEdge3Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->makeEdgeVisibleAt(2); else pObj->makeEdgeInvisibleAt(2);
  return eOk;
}

/* OdDbFaceVisibilityEdge4Property */

struct OdDbFaceVisibilityEdge4Property : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbFaceVisibilityEdge4Property>::createObject();
    ((OdDbFaceVisibilityEdge4Property*)res.get())->init(__OD_T("VisibilityEdge4"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbFaceVisibilityEdge4Property*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Display Edge 4"));
    ((OdDbFaceVisibilityEdge4Property*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Determines whether 3DFace Edge 4 is visible or hidden"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Geometry"))
      OdRxCategory::createObject(L"Geometry", OdRxCategory::rootCategory());
    ((OdDbFaceVisibilityEdge4Property*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Geometry", 5));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbFaceVisibilityEdge4Property::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  value = pObj->isEdgeVisibleAt(3);
  return eOk;
}

OdResult OdDbFaceVisibilityEdge4Property::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbFacePtr pObj = OdDbFace::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->makeEdgeVisibleAt(3); else pObj->makeEdgeInvisibleAt(3);
  return eOk;
}

void createOdDbFaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbFaceCoordinatesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFaceVisibilityEdge1Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFaceVisibilityEdge2Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFaceVisibilityEdge3Property::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbFaceVisibilityEdge4Property::createObject(b.owner()));
  b.add(properties.last());
}
