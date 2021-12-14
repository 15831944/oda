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
#include "DbSubDMesh.h"
#include "DbValueTypes.h"

/* OdDbSubDMeshNumberOfEdgesProperty */

struct OdDbSubDMeshNumberOfEdgesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSubDMeshNumberOfEdgesProperty>::createObject();
    ((OdDbSubDMeshNumberOfEdgesProperty*)res.get())->init(__OD_T("NumberOfEdges"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSubDMeshNumberOfEdgesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSubDMeshPtr pObj = OdDbSubDMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdInt32 nNumEdges; OdResult rc = pObj->numOfEdges(nNumEdges); value = static_cast<int>(nNumEdges);
  return rc;
}

/* OdDbSubDMeshNumberOfSubDividedVerticesProperty */

struct OdDbSubDMeshNumberOfSubDividedVerticesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSubDMeshNumberOfSubDividedVerticesProperty>::createObject();
    ((OdDbSubDMeshNumberOfSubDividedVerticesProperty*)res.get())->init(__OD_T("NumberOfSubDividedVertices"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSubDMeshNumberOfSubDividedVerticesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSubDMeshPtr pObj = OdDbSubDMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdInt32 nNumVert; OdResult rc = pObj->numOfSubDividedVertices(nNumVert); value = static_cast<int>(nNumVert);
  return rc;
}

/* OdDbSubDMeshNumberOfFacesProperty */

struct OdDbSubDMeshNumberOfFacesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSubDMeshNumberOfFacesProperty>::createObject();
    ((OdDbSubDMeshNumberOfFacesProperty*)res.get())->init(__OD_T("NumberOfFaces"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSubDMeshNumberOfFacesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Number of faces"));
    ((OdDbSubDMeshNumberOfFacesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of faces for the unsmooth mesh"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbSubDMeshNumberOfFacesProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 0));
    ((OdDbSubDMeshNumberOfFacesProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"FaceCount"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSubDMeshNumberOfFacesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSubDMeshPtr pObj = OdDbSubDMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdInt32 nNumFaces; OdResult rc = pObj->numOfFaces(nNumFaces); value = static_cast<int>(nNumFaces);
  return rc;
}

/* OdDbSubDMeshWatertightProperty */

struct OdDbSubDMeshWatertightProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSubDMeshWatertightProperty>::createObject();
    ((OdDbSubDMeshWatertightProperty*)res.get())->init(__OD_T("Watertight"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSubDMeshWatertightProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSubDMeshPtr pObj = OdDbSubDMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  bool isW; OdResult rc = pObj->isWatertight(isW); value = isW;
  return rc;
}

/* OdDbSubDMeshNumberOfVerticesProperty */

struct OdDbSubDMeshNumberOfVerticesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSubDMeshNumberOfVerticesProperty>::createObject();
    ((OdDbSubDMeshNumberOfVerticesProperty*)res.get())->init(__OD_T("NumberOfVertices"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbSubDMeshNumberOfVerticesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Number of vertices"));
    ((OdDbSubDMeshNumberOfVerticesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of vertices for the unsmooth mesh"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbSubDMeshNumberOfVerticesProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 0));
    ((OdDbSubDMeshNumberOfVerticesProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"VertexCount"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSubDMeshNumberOfVerticesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSubDMeshPtr pObj = OdDbSubDMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdInt32 nNumVert; OdResult rc = pObj->numOfVertices(nNumVert); value = static_cast<int>(nNumVert);
  return rc;
}

/* OdDbSubDMeshNumberOfSubDividedFacesProperty */

struct OdDbSubDMeshNumberOfSubDividedFacesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbSubDMeshNumberOfSubDividedFacesProperty>::createObject();
    ((OdDbSubDMeshNumberOfSubDividedFacesProperty*)res.get())->init(__OD_T("NumberOfSubDividedFaces"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbSubDMeshNumberOfSubDividedFacesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbSubDMeshPtr pObj = OdDbSubDMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  OdInt32 nNumSDF; OdResult rc = pObj->numOfSubDividedFaces(nNumSDF); value = static_cast<int>(nNumSDF);
  return rc;
}

void createOdDbSubDMeshProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbSubDMeshNumberOfEdgesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSubDMeshNumberOfSubDividedVerticesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSubDMeshNumberOfFacesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSubDMeshWatertightProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSubDMeshNumberOfVerticesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbSubDMeshNumberOfSubDividedFacesProperty::createObject(b.owner()));
  b.add(properties.last());
}
