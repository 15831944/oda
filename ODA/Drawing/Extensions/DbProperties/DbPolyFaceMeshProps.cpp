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
#include "DbPolyFaceMesh.h"
#include "DbValueTypes.h"

/* OdDbPolyFaceMeshNumVerticesProperty */

struct OdDbPolyFaceMeshNumVerticesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolyFaceMeshNumVerticesProperty>::createObject();
    ((OdDbPolyFaceMeshNumVerticesProperty*)res.get())->init(__OD_T("NumVertices"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolyFaceMeshNumVerticesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Number of vertices"));
    ((OdDbPolyFaceMeshNumVerticesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of vertices in the mesh"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbPolyFaceMeshNumVerticesProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 1));
    ((OdDbPolyFaceMeshNumVerticesProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"NumberOfVertices"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPolyFaceMeshNumVerticesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolyFaceMeshPtr pObj = OdDbPolyFaceMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numVertices());
  return eOk;
}

/* OdDbPolyFaceMeshNumFacesProperty */

struct OdDbPolyFaceMeshNumFacesProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolyFaceMeshNumFacesProperty>::createObject();
    ((OdDbPolyFaceMeshNumFacesProperty*)res.get())->init(__OD_T("NumFaces"), &OdRxValueType::Desc<int>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolyFaceMeshNumFacesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Number of faces"));
    ((OdDbPolyFaceMeshNumFacesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the number of faces in the mesh"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbPolyFaceMeshNumFacesProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 2));
    ((OdDbPolyFaceMeshNumFacesProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"NumberOfFaces"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPolyFaceMeshNumFacesProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolyFaceMeshPtr pObj = OdDbPolyFaceMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<int>(pObj->numFaces());
  return eOk;
}

// OdDbPolyFaceMeshCoordinatesProperty
struct OdDbPolyFaceMeshCoordinatesProperty : OdRxCollectionProperty
{
  struct I : OdRxValueIterator
  {
    OdDbObjectIteratorPtr it;
    virtual bool done() { return it->done(); }
    virtual bool next()
    {
      if (it->done())
        return false;
      it->step();
      return true;
    }
    virtual OdRxValue current() const
    {
      return it->objectId();
    }
  };
  virtual OdRxValueIteratorPtr subNewValueIterator(const OdRxObject* pO) const ODRX_OVERRIDE
  {
    OdRxValueIteratorPtr res;
    OdDbPolyFaceMeshPtr t = OdDbPolyFaceMesh::cast(pO);
    if (!t.isNull())
    {
      res = OdRxObjectImpl<I>::createObject();
      ((I*)res.get())->it = t->vertexIterator();
    }
    return res;
  }
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE
  {
    OdDbPolyFaceMeshPtr t = OdDbPolyFaceMesh::cast(pO);
    if (t.isNull())
     return eNotApplicable;
    int N = 0;
    for (OdDbObjectIteratorPtr it = t->vertexIterator(); !it->done(); it->step())
      ++N;
    count = N;
    return eOk;
  }
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolyFaceMeshCoordinatesProperty>::createObject();
    ((OdDbPolyFaceMeshCoordinatesProperty*)res.get())->init(__OD_T("Coordinates"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolyFaceMeshCoordinatesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Vertex"));
    ((OdDbPolyFaceMeshCoordinatesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the vertices of the mesh"));
    return res;
  }
};

void createOdDbPolyFaceMeshProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbPolyFaceMeshNumVerticesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolyFaceMeshNumFacesProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolyFaceMeshCoordinatesProperty::createObject(b.owner()));
  b.add(properties.last());
}
