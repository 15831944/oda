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
#include "ExDbCurveIntersectionPE.h"
#include "DbCurve.h"
#include "Ge/GeCurveCurveInt3d.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeEllipArc3d.h"


OdResult ExDbCurveIntersectionPE::intersectWith(
    const OdDbEntity* pThisEnt, const OdDbEntity* pEnt,
    OdDb::Intersect intType, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker, OdGsMarker otherGsMarker) const
{
  const double tol = 1e-8;
  const double tol2 = tol * tol;
  OdDbCurvePtr pEnt1 = pThisEnt;
  if (thisGsMarker)
  {
    pEnt1 = pThisEnt->subentPtr(OdDbFullSubentPath(OdDb::kEdgeSubentType, thisGsMarker));
  }
  if (pEnt1.isNull())
    return eDegenerateGeometry;

  OdDbCurvePtr pEnt2 = pEnt;
  if (otherGsMarker)
  {
    pEnt2 = pEnt->subentPtr(OdDbFullSubentPath(OdDb::kEdgeSubentType, otherGsMarker));
  }
  if (pEnt2.isNull())
    return eDegenerateGeometry;

  OdGeCurve3d* pGeCurve[2] = { NULL, NULL };
  for (int i = 0; i < 2; ++i) {
    OdDbCurvePtr pEntity = i == 0 ? pEnt1 : pEnt2;
    OdResult res = pEntity->getOdGeCurve(pGeCurve[i]);
    if (res != eOk) {
      delete pGeCurve[i];
      if (i > 0) {
        delete pGeCurve[i - 1];
      }
      return res;
    }
  }

  for (int i = 0; i < 2; ++i) {
    bool needToExtend = intType == OdDb::kExtendBoth || intType == (i == 0 ? OdDb::kExtendThis : OdDb::kExtendArg);
    if (needToExtend) {
      OdGe::EntityId type = pGeCurve[i]->type();
      if (type == OdGe::kLineSeg3d) {
        OdGeLineSeg3d* pLineSeg = static_cast<OdGeLineSeg3d*>(pGeCurve[0]);
        OdGeLine3d* pLine = new OdGeLine3d();
        pLineSeg->getLine(*pLine);
        delete pLineSeg;
        pGeCurve[i] = pLine;
      }
      else if (type == OdGe::kCircArc3d) {
        OdGeCircArc3d* pArc = static_cast<OdGeCircArc3d*>(pGeCurve[0]);
        pArc->setAngles(0, Oda2PI);
      }
      else if (type == OdGe::kEllipArc3d) {
        OdGeEllipArc3d* pArc = static_cast<OdGeEllipArc3d*>(pGeCurve[0]);
        pArc->setAngles(0, Oda2PI);
      }
    }
  }

  OdGeCurveCurveInt3d intersection(*pGeCurve[0], *pGeCurve[1], OdGeVector3d::kIdentity, tol);

  int nIntersections = intersection.numIntPoints();
  intersection.orderWrt2();
  for (int i = 0; i < nIntersections; ++i) {
    OdGePoint3d pt = intersection.intPoint(i);
    bool good = true;
    for (OdUInt32 j = 0; j < points.size(); ++j) {
      double dist2 = points[j].distanceSqrdTo(pt);
      if (dist2 <= tol2) {
        good = false;
        break;
      }
    }
    if (good) {
      points.push_back(intersection.intPoint(i));
    }
  }

  for (int i = 0; i < 2; ++i) {
    delete pGeCurve[i];
  }

  return eOk;
}

OdResult ExDbCurveIntersectionPE::intersectWith(
    const OdDbEntity* pThisEnt, const OdDbEntity* pEnt,
    OdDb::Intersect intType, const OdGePlane& projPlane, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker, OdGsMarker otherGsMarker) const
{
  return eNotImplementedYet;
}

OdResult ExDbCurveIntersectionPE::boundingBoxIntersectWith(
    const OdDbEntity* pThisEnt, const OdDbEntity* pEnt,
    OdDb::Intersect intType, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker, OdGsMarker otherGsMarker) const
{
  return eNotImplementedYet;
}

OdResult ExDbCurveIntersectionPE::boundingBoxIntersectWith(
    const OdDbEntity* pThisEnt, const OdDbEntity* pEnt,
    OdDb::Intersect intType, const OdGePlane& projPlane, OdGePoint3dArray& points,
    OdGsMarker thisGsMarker, OdGsMarker otherGsMarker) const
{
  return eNotImplementedYet;
}
