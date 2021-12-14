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
#include "DbPolygonMesh.h"
#include "DbValueTypes.h"

/* OdDbPolygonMeshNSizeProperty */

struct OdDbPolygonMeshNSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolygonMeshNSizeProperty>::createObject();
    ((OdDbPolygonMeshNSizeProperty*)res.get())->init(__OD_T("NSize"), &OdRxValueType::Desc<short>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolygonMeshNSizeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"N vertex count"));
    ((OdDbPolygonMeshNSizeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the N Vertex number of the polygonmesh"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbPolygonMeshNSizeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 5));
    ((OdDbPolygonMeshNSizeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"NVertexCount"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPolygonMeshNSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->nSize());
  return eOk;
}

/* OdDbPolygonMeshMSizeProperty */

struct OdDbPolygonMeshMSizeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolygonMeshMSizeProperty>::createObject();
    ((OdDbPolygonMeshMSizeProperty*)res.get())->init(__OD_T("MSize"), &OdRxValueType::Desc<short>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolygonMeshMSizeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"M vertex count"));
    ((OdDbPolygonMeshMSizeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Returns the M Vertex number of the polygonmesh"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbPolygonMeshMSizeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 4));
    ((OdDbPolygonMeshMSizeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"MVertexCount"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbPolygonMeshMSizeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->mSize());
  return eOk;
}

/* OdDbPolygonMeshPolyMeshTypeProperty */

struct OdDbPolygonMeshPolyMeshTypeProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolygonMeshPolyMeshTypeProperty>::createObject();
    ((OdDbPolygonMeshPolyMeshTypeProperty*)res.get())->init(__OD_T("PolyMeshType"), &OdRxValueType::Desc<OdDb::PolyMeshType>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolygonMeshPolyMeshTypeProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Fit/Smooth"));
    ((OdDbPolygonMeshPolyMeshTypeProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the type of the polygonmesh"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Misc"))
      OdRxCategory::createObject(L"Misc", OdRxCategory::rootCategory());
    ((OdDbPolygonMeshPolyMeshTypeProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Misc", 0));
    ((OdDbPolygonMeshPolyMeshTypeProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"Type"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolygonMeshPolyMeshTypeProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<OdDb::PolyMeshType>(pObj->polyMeshType());
  return eOk;
}

OdResult OdDbPolygonMeshPolyMeshTypeProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  OdDb::PolyMeshType val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setPolyMeshType(val);
  return eOk;
}

/* OdDbPolygonMeshIsMClosedProperty */

struct OdDbPolygonMeshIsMClosedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolygonMeshIsMClosedProperty>::createObject();
    ((OdDbPolygonMeshIsMClosedProperty*)res.get())->init(__OD_T("IsMClosed"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolygonMeshIsMClosedProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"M closed"));
    ((OdDbPolygonMeshIsMClosedProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies mesh style for M direction, Open or Closed"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbPolygonMeshIsMClosedProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 0));
    ((OdDbPolygonMeshIsMClosedProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"MClose"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolygonMeshIsMClosedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isMClosed());
  return eOk;
}

OdResult OdDbPolygonMeshIsMClosedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->makeMClosed(); else pObj->makeMOpen();
  return eOk;
}

/* OdDbPolygonMeshNSurfaceDensityProperty */

struct OdDbPolygonMeshNSurfaceDensityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolygonMeshNSurfaceDensityProperty>::createObject();
    ((OdDbPolygonMeshNSurfaceDensityProperty*)res.get())->init(__OD_T("NSurfaceDensity"), &OdRxValueType::Desc<short>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolygonMeshNSurfaceDensityProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"N density"));
    ((OdDbPolygonMeshNSurfaceDensityProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies N density value of the polygonmesh; valid values 3-201"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbPolygonMeshNSurfaceDensityProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 3));
    ((OdDbPolygonMeshNSurfaceDensityProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"NDensity"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolygonMeshNSurfaceDensityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->nSurfaceDensity());
  return eOk;
}

OdResult OdDbPolygonMeshNSurfaceDensityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setNSurfaceDensity(val);
  return eOk;
}

/* OdDbPolygonMeshMSurfaceDensityProperty */

struct OdDbPolygonMeshMSurfaceDensityProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolygonMeshMSurfaceDensityProperty>::createObject();
    ((OdDbPolygonMeshMSurfaceDensityProperty*)res.get())->init(__OD_T("MSurfaceDensity"), &OdRxValueType::Desc<short>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolygonMeshMSurfaceDensityProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"M density"));
    ((OdDbPolygonMeshMSurfaceDensityProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies M density value of the polygonmesh; valid values 3-201"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbPolygonMeshMSurfaceDensityProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 2));
    ((OdDbPolygonMeshMSurfaceDensityProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"MDensity"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolygonMeshMSurfaceDensityProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<short>(pObj->mSurfaceDensity());
  return eOk;
}

OdResult OdDbPolygonMeshMSurfaceDensityProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  short val;
  if (!(value >> val))
    return eInvalidInput;
  pObj->setMSurfaceDensity(val);
  return eOk;
}

/* OdDbPolygonMeshIsNClosedProperty */

struct OdDbPolygonMeshIsNClosedProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolygonMeshIsNClosedProperty>::createObject();
    ((OdDbPolygonMeshIsNClosedProperty*)res.get())->init(__OD_T("IsNClosed"), &OdRxValueType::Desc<bool>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolygonMeshIsNClosedProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"N closed"));
    ((OdDbPolygonMeshIsNClosedProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies mesh style for N direction, Open or Closed"));
    if (!OdRxCategory::rootCategory()->findDescendant(L"Mesh"))
      OdRxCategory::createObject(L"Mesh", OdRxCategory::rootCategory());
    ((OdDbPolygonMeshIsNClosedProperty*)res.get())->attributes().add(OdRxUiPlacementAttribute::createObject(L"Mesh", 1));
    ((OdDbPolygonMeshIsNClosedProperty*)res.get())->attributes().add(OdRxCOMAttribute::createObject(L"NClose"));
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual OdResult subSetValue(OdRxObject* pO, const OdRxValue& value) const ODRX_OVERRIDE;
};

OdResult OdDbPolygonMeshIsNClosedProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  value = static_cast<bool>(pObj->isNClosed());
  return eOk;
}

OdResult OdDbPolygonMeshIsNClosedProperty::subSetValue(OdRxObject* pO, const OdRxValue& value) const
{
  OdDbPolygonMeshPtr pObj = OdDbPolygonMesh::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  bool val;
  if (!(value >> val))
    return eInvalidInput;
  //>>REPLACE_LINE
  if (val) pObj->makeNClosed(); else pObj->makeNOpen();
  return eOk;
}

// OdDbPolygonMeshCoordinatesProperty
struct OdDbPolygonMeshCoordinatesProperty : OdRxCollectionProperty
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
    OdDbPolygonMeshPtr t = OdDbPolygonMesh::cast(pO);
    if (!t.isNull())
    {
      res = OdRxObjectImpl<I>::createObject();
      ((I*)res.get())->it = t->vertexIterator();
    }
    return res;
  }
  virtual OdResult subTryGetCount(const OdRxObject* pO, int& count) const ODRX_OVERRIDE
  {
    OdDbPolygonMeshPtr t = OdDbPolygonMesh::cast(pO);
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
    OdRxMemberPtr res = OdRxObjectImpl<OdDbPolygonMeshCoordinatesProperty>::createObject();
    ((OdDbPolygonMeshCoordinatesProperty*)res.get())->init(__OD_T("Coordinates"), &OdRxValueType::Desc<OdGePoint3d>::value(), owner);
//  TODO: OdRxLocalizedNameAttribute is temporarily down
//    ((OdDbPolygonMeshCoordinatesProperty*)res.get())->attributes().add(OdRxLocalizedNameAttribute::createObject(L"Vertex"));
    ((OdDbPolygonMeshCoordinatesProperty*)res.get())->attributes().add(OdRxDescriptionAttribute::createObject(L"Specifies the coordinates of the polygonmesh"));
    return res;
  }
};

void createOdDbPolygonMeshProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbPolygonMeshNSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolygonMeshMSizeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolygonMeshPolyMeshTypeProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolygonMeshIsMClosedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolygonMeshNSurfaceDensityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolygonMeshMSurfaceDensityProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolygonMeshIsNClosedProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbPolygonMeshCoordinatesProperty::createObject(b.owner()));
  b.add(properties.last());
}
