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
#include "DbNurbSurface.h"
#include "DbValueTypes.h"

/* OdDbNurbSurfaceNumberOfKnotsInVProperty */

struct OdDbNurbSurfaceNumberOfKnotsInVProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceNumberOfKnotsInVProperty>::createObject();
    ((OdDbNurbSurfaceNumberOfKnotsInVProperty*)res.get())->init(__OD_T("NumberOfKnotsInV"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceNumberOfKnotsInVProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  int iCount; OdResult rc = pObj->getNumberOfKnotsInV(iCount); if (rc == eOk) value = iCount;
  return rc;
}

/* OdDbNurbSurfaceNumberOfKnotsInUProperty */

struct OdDbNurbSurfaceNumberOfKnotsInUProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceNumberOfKnotsInUProperty>::createObject();
    ((OdDbNurbSurfaceNumberOfKnotsInUProperty*)res.get())->init(__OD_T("NumberOfKnotsInU"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceNumberOfKnotsInUProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  int iCount; OdResult rc = pObj->getNumberOfKnotsInU(iCount); if (rc == eOk) value = iCount;
  return rc;
}

/* OdDbNurbSurfaceDegreeInVProperty */

struct OdDbNurbSurfaceDegreeInVProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceDegreeInVProperty>::createObject();
    ((OdDbNurbSurfaceDegreeInVProperty*)res.get())->init(__OD_T("DegreeInV"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceDegreeInVProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  int iDegree; OdResult rc = pObj->getDegreeInV(iDegree); if (rc == eOk) value = iDegree;
  return rc;
}

/* OdDbNurbSurfaceDegreeInUProperty */

struct OdDbNurbSurfaceDegreeInUProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceDegreeInUProperty>::createObject();
    ((OdDbNurbSurfaceDegreeInUProperty*)res.get())->init(__OD_T("DegreeInU"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceDegreeInUProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  int iDegree; OdResult rc = pObj->getDegreeInU(iDegree); if (rc == eOk) value = iDegree;
  return rc;
}

/* OdDbNurbSurfaceNumberOfControlPointsInUProperty */

struct OdDbNurbSurfaceNumberOfControlPointsInUProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceNumberOfControlPointsInUProperty>::createObject();
    ((OdDbNurbSurfaceNumberOfControlPointsInUProperty*)res.get())->init(__OD_T("NumberOfControlPointsInU"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceNumberOfControlPointsInUProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  int iCount; OdResult rc = pObj->getNumberOfControlPointsInU(iCount); if (rc == eOk) value = iCount;
  return rc;
}

/* OdDbNurbSurfaceNumberOfControlPointsInVProperty */

struct OdDbNurbSurfaceNumberOfControlPointsInVProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceNumberOfControlPointsInVProperty>::createObject();
    ((OdDbNurbSurfaceNumberOfControlPointsInVProperty*)res.get())->init(__OD_T("NumberOfControlPointsInV"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceNumberOfControlPointsInVProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  int iCount; OdResult rc = pObj->getNumberOfControlPointsInV(iCount); if (rc == eOk) value = iCount;
  return rc;
}

/* OdDbNurbSurfaceIsRationalProperty */

struct OdDbNurbSurfaceIsRationalProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceIsRationalProperty>::createObject();
    ((OdDbNurbSurfaceIsRationalProperty*)res.get())->init(__OD_T("IsRational"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceIsRationalProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  bool bIsRational; OdResult rc = pObj->isRational(bIsRational); if (rc == eOk) value = bIsRational;
  return rc;
}

/* OdDbNurbSurfacePeriodInVProperty */

struct OdDbNurbSurfacePeriodInVProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfacePeriodInVProperty>::createObject();
    ((OdDbNurbSurfacePeriodInVProperty*)res.get())->init(__OD_T("PeriodInV"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfacePeriodInVProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dPeriod; OdResult rc = pObj->getPeriodInV(dPeriod); if (rc == eOk) value = dPeriod;
  return rc;
}

/* OdDbNurbSurfaceNumberOfSpansInVProperty */

struct OdDbNurbSurfaceNumberOfSpansInVProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceNumberOfSpansInVProperty>::createObject();
    ((OdDbNurbSurfaceNumberOfSpansInVProperty*)res.get())->init(__OD_T("NumberOfSpansInV"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceNumberOfSpansInVProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  int iSpan; OdResult rc = pObj->getNumberOfSpansInV(iSpan); if (rc == eOk) value = iSpan;
  return rc;
}

/* OdDbNurbSurfaceNumberOfSpansInUProperty */

struct OdDbNurbSurfaceNumberOfSpansInUProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceNumberOfSpansInUProperty>::createObject();
    ((OdDbNurbSurfaceNumberOfSpansInUProperty*)res.get())->init(__OD_T("NumberOfSpansInU"), &OdRxValueType::Desc<int>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceNumberOfSpansInUProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  int iSpan; OdResult rc = pObj->getNumberOfSpansInU(iSpan); if (rc == eOk) value = iSpan;
  return rc;
}

/* OdDbNurbSurfacePeriodInUProperty */

struct OdDbNurbSurfacePeriodInUProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfacePeriodInUProperty>::createObject();
    ((OdDbNurbSurfacePeriodInUProperty*)res.get())->init(__OD_T("PeriodInU"), &OdRxValueType::Desc<double>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfacePeriodInUProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  double dPeriod; OdResult rc = pObj->getPeriodInU(dPeriod); if (rc == eOk) value = dPeriod;
  return rc;
}

/* OdDbNurbSurfaceIsClosedInVProperty */

struct OdDbNurbSurfaceIsClosedInVProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceIsClosedInVProperty>::createObject();
    ((OdDbNurbSurfaceIsClosedInVProperty*)res.get())->init(__OD_T("IsClosedInV"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceIsClosedInVProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  bool bIsClosed; OdResult rc = pObj->isClosedInV(bIsClosed); if (rc == eOk) value = bIsClosed;
  return rc;
}

/* OdDbNurbSurfaceIsClosedInUProperty */

struct OdDbNurbSurfaceIsClosedInUProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceIsClosedInUProperty>::createObject();
    ((OdDbNurbSurfaceIsClosedInUProperty*)res.get())->init(__OD_T("IsClosedInU"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceIsClosedInUProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  bool bIsClosed; OdResult rc = pObj->isClosedInU(bIsClosed); if (rc == eOk) value = bIsClosed;
  return rc;
}

/* OdDbNurbSurfaceIsPeriodicInVProperty */

struct OdDbNurbSurfaceIsPeriodicInVProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceIsPeriodicInVProperty>::createObject();
    ((OdDbNurbSurfaceIsPeriodicInVProperty*)res.get())->init(__OD_T("IsPeriodicInV"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceIsPeriodicInVProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  bool bIsPeriodic; OdResult rc = pObj->isPeriodicInV(bIsPeriodic); if (rc == eOk) value = bIsPeriodic;
  return rc;
}

/* OdDbNurbSurfaceIsPeriodicInUProperty */

struct OdDbNurbSurfaceIsPeriodicInUProperty : OdRxProperty
{
  static OdRxMemberPtr createObject(const OdRxClass* owner)
  {
    OdRxMemberPtr res = OdRxObjectImpl<OdDbNurbSurfaceIsPeriodicInUProperty>::createObject();
    ((OdDbNurbSurfaceIsPeriodicInUProperty*)res.get())->init(__OD_T("IsPeriodicInU"), &OdRxValueType::Desc<bool>::value(), owner);
    return res;
  }
  virtual OdResult subGetValue(const OdRxObject* pO, OdRxValue& value) const ODRX_OVERRIDE;
  virtual bool isReadOnly(const OdRxObject*) const ODRX_OVERRIDE { return true; }
};

OdResult OdDbNurbSurfaceIsPeriodicInUProperty::subGetValue(const OdRxObject* pO, OdRxValue& value) const
{
  OdDbNurbSurfacePtr pObj = OdDbNurbSurface::cast(pO);
  if (pObj.isNull())
    return eNotApplicable;
  //>>REPLACE_LINE
  bool bIsPeriodic; OdResult rc = pObj->isPeriodicInU(bIsPeriodic); if (rc == eOk) value = bIsPeriodic;
  return rc;
}

void createOdDbNurbSurfaceProperties(OdArray<OdRxMemberPtr>& properties, OdRxMemberCollectionBuilder& b, void*)
{
  properties.append(OdDbNurbSurfaceNumberOfKnotsInVProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceNumberOfKnotsInUProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceDegreeInVProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceDegreeInUProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceNumberOfControlPointsInUProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceNumberOfControlPointsInVProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceIsRationalProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfacePeriodInVProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceNumberOfSpansInVProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceNumberOfSpansInUProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfacePeriodInUProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceIsClosedInVProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceIsClosedInUProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceIsPeriodicInVProperty::createObject(b.owner()));
  b.add(properties.last());
  properties.append(OdDbNurbSurfaceIsPeriodicInUProperty::createObject(b.owner()));
  b.add(properties.last());
}
