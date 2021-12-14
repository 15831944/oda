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
#include "ExCreateBreps.h"

#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeEllipArc2d.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeLineSeg2d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeCylinder.h"
#include "Ge/GeEllipCylinder.h"
#include "Ge/GeCone.h"
#include "Ge/GeEllipCone.h"
#include "Ge/GeNurbSurface.h"
#include "Ge/GeTorus.h"
#include "Ge/GeSphere.h"
#include "Ge/GePlane.h"
#include "Ge/GeRevolvedSurface.h"
#include "Ge/GeSpunSurf.h"
#include "BrepBuilder/BrepBuilder.h"

#include "DbHostAppServices.h"
#include "DbBlockTableRecord.h"

#include "DbBody.h"

#include "Error_WithMessage.h"

#include "TPtr.h"


typedef TPtr< OdGeNurbCurve2d, TObjDelete<OdGeNurbCurve2d> > OdGeTempNurbCurve2dPtr;

// helper funcs
static OdGeVector3d getPlaneNormal(OdGePoint3d point1, OdGePoint3d point2, OdGePoint3d point3)
{
  OdGeVector3d XVector = (point2 - point1);
  OdGeVector3d YVector = (point3 - point1);
  ODA_ASSERT_ONCE(!OdZero(XVector.normalizeGetLength()) && !OdZero(YVector.normalizeGetLength()));
  return XVector.normal().crossProduct(YVector.normal());
}

static void setupUVCurve(OdGeNurbCurve2d& uvCurve, const OdGePoint2d& pnt1, const OdGePoint2d& pnt2, double param1, double param2)
{
  const int coedgeDegree = 1;

  const double uvKnotsVals[] = { param1, param1, param2, param2 };
  ODA_ASSUME(sizeof(uvKnotsVals) / sizeof(*uvKnotsVals) == 4);
  OdGeKnotVector uvKnots(4, uvKnotsVals);

  OdGePoint2dArray uvPoints(2);
  uvPoints.append(pnt1);
  uvPoints.append(pnt2);

  OdGeDoubleArray weights;

  uvCurve.set(coedgeDegree, uvKnots, uvPoints, weights);
}

namespace ExCreateBreps
{
  void addPlane(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, bool useReverse, const OdGeVector3d& offset, OdDbBodyPtr* pBodyStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    OdGePoint3d center(0., 0., 0.);
    OdGeVector3d norm(0., 0., 1.);

    OdGePlane pln(center, norm);
    if (useReverse)
    {
      pln.reverseNormal();
    }

    OdGeLineSeg3d l1(OdGePoint3d(0, 2, 0), OdGePoint3d(1, -2, 0));
    OdGeLineSeg3d l2(OdGePoint3d(1, -2, 0), OdGePoint3d(-3, -2, 0));
    OdGeLineSeg3d l3(OdGePoint3d(-3, -2, 0), OdGePoint3d(0, 2, 0));


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kOpenShell);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId faceId = brepBuilder.addFace(&/*cylinder*/pln, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId loopId;
    BRepBuilderGeometryId edgeId;

    loopId = brepBuilder.addLoop(faceId);
    edgeId = brepBuilder.addEdge(&l1);
    brepBuilder.addCoedge(loopId, edgeId, !useReverse ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward);

    edgeId = !useReverse ? brepBuilder.addEdge(&l3) : brepBuilder.addEdge(&l2);
    brepBuilder.addCoedge(loopId, edgeId, !useReverse ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward);

    edgeId = !useReverse ? brepBuilder.addEdge(&l2) : brepBuilder.addEdge(&l3);
    brepBuilder.addCoedge(loopId, edgeId, !useReverse ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward);

    brepBuilder.finishLoop(loopId);

    brepBuilder.finishFace(faceId);
    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDbBodyPtr pBody = OdDbBody::createObject();
    pBody->setBody(pModel);
    pBody->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pBody);
    }

    if (pBodyStore)
    {
      *pBodyStore = pBody;
    }
  }

  void addRegionWithHole(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDbRegionPtr* pRegionStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    OdGePoint3d center(OdGePoint3d::kOrigin);
    OdGeVector3d norm(OdGeVector3d::kZAxis);

    OdGePlane pln(center, norm);

    const OdGePoint3d pointsOuter[] = {
      OdGePoint3d(0., 0., 0.), OdGePoint3d(2., 0., 0), OdGePoint3d(2., 2., 0), OdGePoint3d(0., 2., 0)
    };
    const OdGePoint3d pointsInner[] = {
      OdGePoint3d(0.5, 0.5, 0.), OdGePoint3d(0.5, 1.5, 0), OdGePoint3d(1.5, 0.5, 0)
    };

    OdGeLineSeg3d lineOuter0(pointsOuter[0], pointsOuter[1]);
    OdGeLineSeg3d lineOuter1(pointsOuter[1], pointsOuter[2]);
    OdGeLineSeg3d lineOuter2(pointsOuter[2], pointsOuter[3]);
    OdGeLineSeg3d lineOuter3(pointsOuter[3], pointsOuter[0]);

    OdGeLineSeg3d lineInner0(pointsInner[0], pointsInner[1]);
    OdGeLineSeg3d lineInner1(pointsInner[1], pointsInner[2]);
    OdGeLineSeg3d lineInner2(pointsInner[2], pointsInner[0]);


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kOpenShell);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId faceId = brepBuilder.addFace(&pln, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId loopId;
    BRepBuilderGeometryId edgeId;

    {
      loopId = brepBuilder.addLoop(faceId);

      edgeId = brepBuilder.addEdge(&lineOuter0);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);

      edgeId = brepBuilder.addEdge(&lineOuter1);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);

      edgeId = brepBuilder.addEdge(&lineOuter2);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);

      edgeId = brepBuilder.addEdge(&lineOuter3);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);

      brepBuilder.finishLoop(loopId);
    }
    {
      loopId = brepBuilder.addLoop(faceId);

      edgeId = brepBuilder.addEdge(&lineInner0);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);

      edgeId = brepBuilder.addEdge(&lineInner1);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);

      edgeId = brepBuilder.addEdge(&lineInner2);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);

      brepBuilder.finishLoop(loopId);
    }

    brepBuilder.finishFace(faceId);
    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDbRegionPtr pRegion = OdDbRegion::createObject();
    pRegion->setBody(pModel);
    pRegion->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pRegion);
    }

    if (pRegionStore)
    {
      *pRegionStore = pRegion;
    }
  }

  //
  void addCylinder(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, bool useReverse, const OdGeVector3d& offset, OdDbBodyPtr* pBodyStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    OdGePoint3d center(0., 0., 0.);
    OdGeVector3d axis(0., 0., 1.);
    double radius = 3.15;
    double height = 4.2;

    OdGeCylinder cylinder(radius, center, axis);

    OdGeCircArc3d arc1(center, axis, radius);
    OdGeCircArc3d arc2(center + axis * height, axis, radius);

    if (useReverse) arc2.reverseParam();// !

                                        // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kOpenShell);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId faceId = brepBuilder.addFace(&cylinder, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId loopId;
    BRepBuilderGeometryId edgeId;

    loopId = brepBuilder.addLoop(faceId);
    edgeId = brepBuilder.addEdge(&arc1);
    brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);
    brepBuilder.finishLoop(loopId);

    loopId = brepBuilder.addLoop(faceId);
    edgeId = brepBuilder.addEdge(&arc2);
    brepBuilder.addCoedge(loopId, edgeId, useReverse ? OdBrepBuilder::kForward : OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId);

    brepBuilder.finishFace(faceId);
    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDbBodyPtr pBody = OdDbBody::createObject();
    pBody->setBody(pModel);
    pBody->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pBody);
    }

    if (pBodyStore)
    {
      *pBodyStore = pBody;
    }
  }

  //
  void addNurbSurface(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, bool useReverse, const OdGeVector3d& offset, OdDbBodyPtr* pBodyStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    const int degreeU = 3;
    const int degreeV = 3;
    const int propsU = 17;
    const int propsV = 17;
    const int nCptsU = 5;
    const int nCptsV = 5;
    const int pntsCount = nCptsU * nCptsV;
    const int knotCountU = nCptsU + degreeU + 1;
    const int knotCountV = nCptsV + degreeV + 1;
    const double uKnots[knotCountU] = { 0, 0, 0, 0, 0.5, 1, 1, 1, 1 };
    const double vKnots[knotCountV] = { 0, 0, 0, 0, 0.5, 1, 1, 1, 1 };
    const double coedgeKnots[4] = { 0., 0., 1., 1. };

    const OdGePoint3d pnts[pntsCount] =
    {
      OdGePoint3d(6.0, 0.0, 0.0), OdGePoint3d(5.0, 0.0, 1.5), OdGePoint3d(3.0, 0.0, 4.5), OdGePoint3d(1.0, 0.0, 1.5), OdGePoint3d(0.0, 0.0, 0.0),
      OdGePoint3d(6.0, 1.0, 1.5), OdGePoint3d(5.0, 1.0, 3.0), OdGePoint3d(3.0, 1.0, 6.0), OdGePoint3d(1.0, 1.0, 3.0), OdGePoint3d(0.0, 1.0, 1.5),
      OdGePoint3d(6.0, 3.0, 4.5), OdGePoint3d(5.0, 3.0, 6.0), OdGePoint3d(3.0, 3.0, 9.0), OdGePoint3d(1.0, 3.0, 6.0), OdGePoint3d(0.0, 3.0, 4.5),
      OdGePoint3d(6.0, 5.0, 1.5), OdGePoint3d(5.0, 5.0, 3.0), OdGePoint3d(3.0, 5.0, 6.0), OdGePoint3d(1.0, 5.0, 3.0), OdGePoint3d(0.0, 5.0, 1.5),
      OdGePoint3d(6.0, 6.0, 0.0), OdGePoint3d(5.0, 6.0, 1.5), OdGePoint3d(3.0, 6.0, 4.5), OdGePoint3d(1.0, 6.0, 1.5), OdGePoint3d(0.0, 6.0, 0.0)
    };

    const OdGePoint2d pntsCoedgeU1[2] = { OdGePoint2d(0.0, 0.0), OdGePoint2d(1.0, 0.0) };
    const OdGePoint2d pntsCoedgeV1[2] = { OdGePoint2d(1.0, 0.0), OdGePoint2d(1.0, 1.0) };
    const OdGePoint2d pntsCoedgeU2[2] = { OdGePoint2d(0.0, 1.0), OdGePoint2d(1.0, 1.0) };
    const OdGePoint2d pntsCoedgeV2[2] = { OdGePoint2d(0.0, 0.0), OdGePoint2d(0.0, 1.0) };

    // surf
    OdGeDoubleArray arrWeights;
    OdGePoint3dArray arrSurfPnts(pntsCount);
    arrSurfPnts.assign(pnts, pnts + pntsCount);
    OdGeKnotVector arrUKnots(knotCountU, uKnots);
    OdGeKnotVector arrVKnots(knotCountV, uKnots);
    // curve 2d
    OdGePoint2dArray arrU1CoedgePnts(2);
    arrU1CoedgePnts.assign(pntsCoedgeU1, pntsCoedgeU1 + 2);
    OdGePoint2dArray arrV1CoedgePnts(2);
    arrV1CoedgePnts.assign(pntsCoedgeV1, pntsCoedgeV1 + 2);
    OdGePoint2dArray arrU2CoedgePnts(2);
    arrU2CoedgePnts.assign(pntsCoedgeU2, pntsCoedgeU2 + 2);
    OdGePoint2dArray arrV2CoedgePnts(2);
    arrV2CoedgePnts.assign(pntsCoedgeV2, pntsCoedgeV2 + 2);
    OdGeKnotVector arrCoedgeKnots(4, coedgeKnots);

    //
    OdGeNurbSurface nurbSurf(degreeU, degreeV, propsU, propsV, nCptsU, nCptsV, arrSurfPnts, arrWeights, arrUKnots, arrVKnots);
    OdGeInterval intervalU, intervalV;
    nurbSurf.getEnvelope(intervalU, intervalV);
    ODA_ASSERT_ONCE(OdEqual(intervalU.lowerBound(), 0.));
    ODA_ASSERT_ONCE(OdEqual(intervalV.lowerBound(), 0.));
    ODA_ASSERT_ONCE(OdEqual(intervalU.upperBound(), 1.));
    ODA_ASSERT_ONCE(OdEqual(intervalV.upperBound(), 1.));
    OdGeNurbCurve3d edgeCrvU1;
    OdGeNurbCurve3d edgeCrvV1;
    OdGeNurbCurve3d edgeCrvU2;
    OdGeNurbCurve3d edgeCrvV2;
    nurbSurf.computeVIsoLine(0., edgeCrvU1);
    nurbSurf.computeUIsoLine(1., edgeCrvV1);
    nurbSurf.computeVIsoLine(1., edgeCrvU2);
    nurbSurf.computeUIsoLine(0., edgeCrvV2);

    OdGeNurbCurve2d coedgeCrvU1(1, arrCoedgeKnots, arrU1CoedgePnts);
    OdGeNurbCurve2d coedgeCrvV1(1, arrCoedgeKnots, arrV1CoedgePnts);
    OdGeNurbCurve2d coedgeCrvU2(1, arrCoedgeKnots, arrU2CoedgePnts);
    OdGeNurbCurve2d coedgeCrvV2(1, arrCoedgeKnots, arrV2CoedgePnts);

    if (useReverse) nurbSurf.reverseNormal();// !

                                             // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kOpenShell);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId faceId = brepBuilder.addFace(&nurbSurf, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId loopId;
    BRepBuilderGeometryId edgeId;

    loopId = brepBuilder.addLoop(faceId);

    if (!useReverse)
    {
      edgeId = brepBuilder.addEdge(&edgeCrvU1);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward, &coedgeCrvU1);
      edgeId = brepBuilder.addEdge(&edgeCrvV1);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward, &coedgeCrvV1);
      edgeId = brepBuilder.addEdge(&edgeCrvU2);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kReversed, &coedgeCrvU2);
      edgeId = brepBuilder.addEdge(&edgeCrvV2);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kReversed, &coedgeCrvV2);
    }
    else
    {
      edgeId = brepBuilder.addEdge(&edgeCrvU1);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kReversed, &coedgeCrvU1);
      edgeId = brepBuilder.addEdge(&edgeCrvV2);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward, &coedgeCrvV2);
      edgeId = brepBuilder.addEdge(&edgeCrvU2);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward, &coedgeCrvU2);
      edgeId = brepBuilder.addEdge(&edgeCrvV1);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kReversed, &coedgeCrvV1);
    }

    brepBuilder.finishLoop(loopId);

    brepBuilder.finishFace(faceId);
    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDbBodyPtr pBody = OdDbBody::createObject();
    pBody->setBody(pModel);
    pBody->transformBy(OdGeMatrix3d().setTranslation(offset - OdGeVector3d(3., 3., 0)));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pBody);
    }

    if (pBodyStore)
    {
      *pBodyStore = pBody;
    }
  }

  //
  void addCone(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, bool cutBeforeApex, bool useReverse, const OdGeVector3d& offset, OdDbBodyPtr* pBodyStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    double angleCone = OdaPI / 6.;
    OdGePoint3d center(0., 0., 0.);
    OdGeVector3d axis(0., 0, -1.0);// TODO check
    double radius = 3.15;
    double coneCos = cos(angleCone);
    double coneSin = sin(angleCone);
    double heightRatio = 0.5;// height offset from center of cone to cut cone

    ODA_ASSERT_ONCE(coneCos > 0);

    OdGeCone cone(coneCos, coneSin, center, radius, axis);
    if (useReverse) cone.reverseNormal(); // !
    OdGePoint3d apexPnt(cone.apex());

    OdGeCircArc3d arc1(center, axis, radius);// loop 0
    if (useReverse)//we reversed cone surface. Now it hole and will not checking by acis. 
    {
      arc1.reverseParam();
    }
    OdGeLineSeg3d line2(apexPnt, apexPnt);// loop 1: apex
    OdGeCircArc3d arc2(center + (apexPnt - center) * heightRatio, axis, radius * (1. - heightRatio));// or loop 1: arc
    if (useReverse)//we reversed cone surface. Now it hole and will not checking by acis. 
    {
      arc2.reverseParam();
    }
    OdGeCurve3d* pCurveLoop1 = cutBeforeApex ? static_cast<OdGeCurve3d*>(&arc2) : static_cast<OdGeCurve3d*>(&line2);

    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kOpenShell);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId faceId = brepBuilder.addFace(&cone, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId loopId;
    BRepBuilderGeometryId edgeId;

    loopId = brepBuilder.addLoop(faceId);
    edgeId = brepBuilder.addEdge(&arc1);
    brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId);

    loopId = brepBuilder.addLoop(faceId);
    edgeId = brepBuilder.addEdge(pCurveLoop1);
    brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);
    brepBuilder.finishLoop(loopId);

    brepBuilder.finishFace(faceId);
    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDbBodyPtr pBody = OdDbBody::createObject();
    pBody->setBody(pModel);
    pBody->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pBody);
    }

    if (pBodyStore)
    {
      *pBodyStore = pBody;
    }
  }

  //
  void addTorus(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, TorusType torusType, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    double majorRadius;
    double minorRadius;
    switch (torusType)
    {
    case ttDoughnut: majorRadius = 3.5; minorRadius = 1.0; break;
    case ttApple: majorRadius = 2.; minorRadius = 2.4; break;
    case ttVortex: majorRadius = 2.4; minorRadius = 2.4; break;
    case ttLemon: majorRadius = -2.; minorRadius = 4.; break;
    }
    OdGePoint3d center(0., 0., 0.);
    OdGeVector3d axis(0., 0, 1.0);
    // Define apex for case !isMajorGreateMinor:
    OdGeVector3d apexOffset = ttDoughnut == torusType ? OdGeVector3d()
      : axis * sqrt(minorRadius * minorRadius - majorRadius * majorRadius);
    OdGePoint3d apexPoint1(center + apexOffset);
    OdGePoint3d apexPoint2(center - apexOffset);

    OdGeTorus torus(majorRadius, minorRadius, center, axis);

    OdGeLineSeg3d apex1(apexPoint1, apexPoint1);// loop 1: apex
    OdGeLineSeg3d apex2(apexPoint2, apexPoint2);// loop 2: apex

                                                // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId faceId = brepBuilder.addFace(&torus, OdBrepBuilder::kForward, shellId);

    if (ttDoughnut != torusType)
    {
      BRepBuilderGeometryId loopId;
      BRepBuilderGeometryId edgeId;

      loopId = brepBuilder.addLoop(faceId);
      edgeId = brepBuilder.addEdge(&apex1);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);

      loopId = brepBuilder.addLoop(faceId);
      edgeId = brepBuilder.addEdge(&apex2);
      brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
    }

    brepBuilder.finishFace(faceId);
    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  //
  void addTorusQuarter(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    const OdGePoint3d centerTorus(0.0, 0.0, 0.0);
    const OdGeVector3d axisOfSymmetryTorus(0.0, 1.0, 0.0);
    const OdGeVector3d refAxisTorus(1.0, 0.0, 0.0);
    const double startAngleUTorus = 0.0;
    const double endAngleUTorus = Oda2PI;
    const double startAngleVTorus = -OdaPI;
    const double endAngleVTorus = OdaPI;
    const double majorRadiusTorus = 5.0;
    const double minorRadiusTorus = 5.0;
    const bool revTorus = false;

    // plane 1
    const OdGePoint3d centerPlane1(5.0, 0.0, 0.0);
    const OdGeVector3d uPlane1(1.0, 0.0, 0.0);
    const OdGeVector3d vPlane1(0.0, 1.0, 0.0);
    const bool revPlane1 = false;
    const OdGeVector3d majorAxisPlane1 = -uPlane1;
    const OdGeVector3d minorAxisPlane1 = -vPlane1;
    const double majorRadiusPlane1 = 5.0;
    const double minorRadiusPlane1 = 5.0;
    const double startParamPlane1 = 0.0;
    const double includedParamPlane1 = Oda2PI;
    const bool revEdgePlane1 = false;

    // plane 2
    const OdGePoint3d centerPlane2(0.0, 0.0, -5.0);
    const OdGeVector3d uPlane2(0.0, 0.0, -1.0);
    const OdGeVector3d vPlane2(0.0, 1.0, 0.0);
    const bool revPlane2 = true;
    const OdGeVector3d majorAxisPlane2 = -uPlane2;
    const OdGeVector3d minorAxisPlane2 = -vPlane2;
    const double majorRadiusPlane2 = 5.0;
    const double minorRadiusPlane2 = 5.0;
    const double startParamPlane2 = 0.0;
    const double includedParamPlane2 = Oda2PI;
    const bool revEdgePlane2 = true;

    //
    OdGeTorus torus(majorRadiusTorus, minorRadiusTorus, centerTorus, axisOfSymmetryTorus, refAxisTorus, startAngleUTorus, endAngleUTorus, startAngleVTorus, endAngleVTorus);
    OdGePlane plane1(centerPlane1, uPlane1, vPlane1);
    OdGeEllipArc3d arc1(centerPlane1, majorAxisPlane1, minorAxisPlane1, majorRadiusPlane1, minorRadiusPlane1, startParamPlane1, includedParamPlane1);
    OdGePlane plane2(centerPlane2, uPlane2, vPlane2);
    OdGeEllipArc3d arc2(centerPlane2, majorAxisPlane2, minorAxisPlane2, majorRadiusPlane2, minorRadiusPlane2, startParamPlane2, includedParamPlane2);

    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId edgeArc1 = brepBuilder.addEdge(&arc1);
    BRepBuilderGeometryId edgeArc2 = brepBuilder.addEdge(&arc2);

    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&plane1, revPlane1 ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeArc1, revEdgePlane1 ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&plane2, revPlane2 ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeArc2, revEdgePlane2 ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&torus, revTorus ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeArc1, !revEdgePlane1 ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward);
      brepBuilder.addCoedge(loopId, edgeArc2, !revEdgePlane2 ? OdBrepBuilder::kReversed : OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  //
  void addSphere(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    double radius = 3;
    OdGePoint3d center(0., 0., 0.);

    OdGeSphere sphere(radius, center);

    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId faceId = brepBuilder.addFace(&sphere, OdBrepBuilder::kForward, shellId);

    brepBuilder.finishFace(faceId);
    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  //
  void addHemiSphere(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    double radius = 3;
    OdGePoint3d center(0., 0., 0.);
    OdGeVector3d normal(OdGeVector3d::kZAxis);

    OdGeVector3d northAxis = OdGeVector3d::kZAxis;
    OdGeVector3d refAxis = OdGeVector3d::kXAxis;
    double startAngleU = -OdaPI2;
    double endAngleU = OdaPI2;
    double startAngleV = 0.;
    double endAngleV = Oda2PI;
    OdGeSphere sphere(radius, center, northAxis, refAxis,
      startAngleU, endAngleU, startAngleV, endAngleV);
    OdGePlane plane(center, -OdGeVector3d::kZAxis);

    OdGeCircArc3d arc1(center, normal, refAxis, radius, startAngleV, endAngleV);

    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId faceId;
    BRepBuilderGeometryId loopId;
    BRepBuilderGeometryId edgeId = brepBuilder.addEdge(&arc1);

    faceId = brepBuilder.addFace(&sphere, OdBrepBuilder::kForward, shellId);
    loopId = brepBuilder.addLoop(faceId);
    brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kForward);
    brepBuilder.finishLoop(loopId);
    brepBuilder.finishFace(faceId);

    faceId = brepBuilder.addFace(&plane, OdBrepBuilder::kForward, shellId);
    loopId = brepBuilder.addLoop(faceId);
    brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId);
    brepBuilder.finishFace(faceId);

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  //
  void addPyramid(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    // Set the points of the pyramid
    OdGePoint3d pnt_1(0., 2., 0.);
    OdGePoint3d pnt_2(3., -3., 0.);
    OdGePoint3d pnt_3(-3., -3., 0.);
    OdGePoint3d pnt_4((pnt_1 + pnt_2.asVector() + pnt_3.asVector())*(1. / 3) + OdGeVector3d::kZAxis * 5.);

    // Create Planes
    OdGePlane plane4(pnt_1, getPlaneNormal(pnt_1, pnt_2, pnt_3));
    OdGePlane plane12(pnt_4, getPlaneNormal(pnt_4, pnt_2, pnt_1));
    OdGePlane plane23(pnt_4, getPlaneNormal(pnt_4, pnt_3, pnt_2));
    OdGePlane plane31(pnt_4, getPlaneNormal(pnt_4, pnt_1, pnt_3));

    // Bottom edges
    OdGeLineSeg3d edge12(pnt_1, pnt_2);
    OdGeLineSeg3d edge23(pnt_2, pnt_3);
    OdGeLineSeg3d edge31(pnt_3, pnt_1);

    // Side edges
    OdGeLineSeg3d edge14(pnt_1, pnt_4);
    OdGeLineSeg3d edge24(pnt_2, pnt_4);
    OdGeLineSeg3d edge34(pnt_3, pnt_4);

    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    // Add Faces
    BRepBuilderGeometryId faceId4 = brepBuilder.addFace(&plane4, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId faceId12 = brepBuilder.addFace(&plane12, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId faceId23 = brepBuilder.addFace(&plane23, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId faceId31 = brepBuilder.addFace(&plane31, OdBrepBuilder::kForward, shellId);

    // Add Edges
    // Bottom edges
    BRepBuilderGeometryId edgeId12 = brepBuilder.addEdge(&edge12);
    BRepBuilderGeometryId edgeId23 = brepBuilder.addEdge(&edge23);
    BRepBuilderGeometryId edgeId31 = brepBuilder.addEdge(&edge31);

    // Side edges
    BRepBuilderGeometryId edgeId14 = brepBuilder.addEdge(&edge14);
    BRepBuilderGeometryId edgeId24 = brepBuilder.addEdge(&edge24);
    BRepBuilderGeometryId edgeId34 = brepBuilder.addEdge(&edge34);

    // Edge color
    OdCmEntityColor color;
    color.setRGB(0xff, 0xa5, 0x00);// use this color for all edges
    const int edgeCount = 6;
    BRepBuilderGeometryId allEdges[edgeCount] = {
      edgeId12, edgeId23, edgeId31, edgeId14, edgeId24, edgeId34
    };
    for (int i = 0; i < 6; ++i)
    {
      res = brepBuilder.setEdgeColor(allEdges[i], color);
      OD_CHECK_THROW(eOk == res, res, "Fail setup color");
    }

    // Add Loops
    BRepBuilderGeometryId loopId4 = brepBuilder.addLoop(faceId4);
    BRepBuilderGeometryId loopId12 = brepBuilder.addLoop(faceId12);
    BRepBuilderGeometryId loopId23 = brepBuilder.addLoop(faceId23);
    BRepBuilderGeometryId loopId31 = brepBuilder.addLoop(faceId31);

    // Add Coedge
    // Bottom face
    brepBuilder.addCoedge(loopId4, edgeId12, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId4, edgeId23, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId4, edgeId31, OdBrepBuilder::kForward);
    brepBuilder.finishLoop(loopId4);
    color.setRGB(0xff, 0xff, 0x00);
    res = brepBuilder.setFaceColor(faceId4, color);
    OD_CHECK_THROW(eOk == res, res, "Fail setup color");
    brepBuilder.finishFace(faceId4);

    //
    brepBuilder.addCoedge(loopId12, edgeId12, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId12, edgeId14, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId12, edgeId24, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId12);
    color.setRGB(0xff, 0x00, 0x00);
    res = brepBuilder.setFaceColor(faceId12, color);
    OD_CHECK_THROW(eOk == res, res, "Fail setup color");
    brepBuilder.finishFace(faceId12);

    //
    brepBuilder.addCoedge(loopId23, edgeId23, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId23, edgeId24, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId23, edgeId34, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId23);
    color.setRGB(0x00, 0xff, 0x00);
    res = brepBuilder.setFaceColor(faceId23, color);
    OD_CHECK_THROW(eOk == res, res, "Fail setup color");
    brepBuilder.finishFace(faceId23);

    //
    brepBuilder.addCoedge(loopId31, edgeId31, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId31, edgeId34, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId31, edgeId14, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId31);
    color.setRGB(0x00, 0x00, 0xff);
    res = brepBuilder.setFaceColor(faceId31, color);
    OD_CHECK_THROW(eOk == res, res, "Fail setup color");
    brepBuilder.finishFace(faceId31);

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  //
  void addBox(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    // Set the points of the box. First 4 bottom CCW order, then 4 top CCW order
    double boxSide = 6.;
    OdGePoint3d bottomCenter(0., 0., 0.);
    OdGePoint3d pnt_1(bottomCenter + OdGeVector3d(-boxSide*0.5, -boxSide*0.5, 0.));
    OdGePoint3d pnt_2(bottomCenter + OdGeVector3d(+boxSide*0.5, -boxSide*0.5, 0.));
    OdGePoint3d pnt_3(bottomCenter + OdGeVector3d(+boxSide*0.5, +boxSide*0.5, 0.));
    OdGePoint3d pnt_4(bottomCenter + OdGeVector3d(-boxSide*0.5, +boxSide*0.5, 0.));
    OdGePoint3d pnt_5(pnt_1 + OdGeVector3d::kZAxis * boxSide);
    OdGePoint3d pnt_6(pnt_2 + OdGeVector3d::kZAxis * boxSide);
    OdGePoint3d pnt_7(pnt_3 + OdGeVector3d::kZAxis * boxSide);
    OdGePoint3d pnt_8(pnt_4 + OdGeVector3d::kZAxis * boxSide);

    // Create Planes
    OdGePlane plane1234(pnt_1, getPlaneNormal(pnt_2, pnt_3, pnt_1));
    OdGePlane plane5678(pnt_5, getPlaneNormal(pnt_6, pnt_7, pnt_5));
    OdGePlane plane1265(pnt_1, getPlaneNormal(pnt_2, pnt_6, pnt_1));
    OdGePlane plane2376(pnt_2, getPlaneNormal(pnt_3, pnt_7, pnt_2));
    OdGePlane plane3487(pnt_3, getPlaneNormal(pnt_4, pnt_8, pnt_3));
    OdGePlane plane4158(pnt_4, getPlaneNormal(pnt_1, pnt_5, pnt_4));

    // Bottom edges
    OdGeLineSeg3d edge12(pnt_1, pnt_2);
    OdGeLineSeg3d edge23(pnt_2, pnt_3);
    OdGeLineSeg3d edge34(pnt_3, pnt_4);
    OdGeLineSeg3d edge41(pnt_4, pnt_1);
    // Top edges
    OdGeLineSeg3d edge56(pnt_5, pnt_6);
    OdGeLineSeg3d edge67(pnt_6, pnt_7);
    OdGeLineSeg3d edge78(pnt_7, pnt_8);
    OdGeLineSeg3d edge85(pnt_8, pnt_5);
    // Side edges
    OdGeLineSeg3d edge15(pnt_1, pnt_5);
    OdGeLineSeg3d edge26(pnt_2, pnt_6);
    OdGeLineSeg3d edge37(pnt_3, pnt_7);
    OdGeLineSeg3d edge48(pnt_4, pnt_8);

    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    // Add Faces
    BRepBuilderGeometryId faceId1234 = brepBuilder.addFace(&plane1234, OdBrepBuilder::kReversed, shellId);
    BRepBuilderGeometryId faceId5678 = brepBuilder.addFace(&plane5678, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId faceId1265 = brepBuilder.addFace(&plane1265, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId faceId2376 = brepBuilder.addFace(&plane2376, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId faceId3487 = brepBuilder.addFace(&plane3487, OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId faceId4158 = brepBuilder.addFace(&plane4158, OdBrepBuilder::kForward, shellId);

    // Add Edges
    // Bottom edges
    BRepBuilderGeometryId edgeId12 = brepBuilder.addEdge(&edge12);
    BRepBuilderGeometryId edgeId23 = brepBuilder.addEdge(&edge23);
    BRepBuilderGeometryId edgeId34 = brepBuilder.addEdge(&edge34);
    BRepBuilderGeometryId edgeId41 = brepBuilder.addEdge(&edge41);
    // Top edges
    BRepBuilderGeometryId edgeId56 = brepBuilder.addEdge(&edge56);
    BRepBuilderGeometryId edgeId67 = brepBuilder.addEdge(&edge67);
    BRepBuilderGeometryId edgeId78 = brepBuilder.addEdge(&edge78);
    BRepBuilderGeometryId edgeId85 = brepBuilder.addEdge(&edge85);
    // Side edges
    BRepBuilderGeometryId edgeId15 = brepBuilder.addEdge(&edge15);
    BRepBuilderGeometryId edgeId26 = brepBuilder.addEdge(&edge26);
    BRepBuilderGeometryId edgeId37 = brepBuilder.addEdge(&edge37);
    BRepBuilderGeometryId edgeId48 = brepBuilder.addEdge(&edge48);

    // Add Loops
    BRepBuilderGeometryId loopId1234 = brepBuilder.addLoop(faceId1234);
    BRepBuilderGeometryId loopId5678 = brepBuilder.addLoop(faceId5678);
    BRepBuilderGeometryId loopId1265 = brepBuilder.addLoop(faceId1265);
    BRepBuilderGeometryId loopId2376 = brepBuilder.addLoop(faceId2376);
    BRepBuilderGeometryId loopId3487 = brepBuilder.addLoop(faceId3487);
    BRepBuilderGeometryId loopId4158 = brepBuilder.addLoop(faceId4158);

    // Add Coedge
    // Bottom face
    brepBuilder.addCoedge(loopId1234, edgeId41, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId1234, edgeId34, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId1234, edgeId23, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId1234, edgeId12, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId1234);
    brepBuilder.finishFace(faceId1234);
    // Top face
    brepBuilder.addCoedge(loopId5678, edgeId56, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId5678, edgeId67, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId5678, edgeId78, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId5678, edgeId85, OdBrepBuilder::kForward);
    brepBuilder.finishLoop(loopId5678);
    brepBuilder.finishFace(faceId5678);
    // Side face 1265
    brepBuilder.addCoedge(loopId1265, edgeId12, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId1265, edgeId26, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId1265, edgeId56, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId1265, edgeId15, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId1265);
    brepBuilder.finishFace(faceId1265);
    // Side face 2376
    brepBuilder.addCoedge(loopId2376, edgeId23, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId2376, edgeId37, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId2376, edgeId67, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId2376, edgeId26, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId2376);
    brepBuilder.finishFace(faceId2376);
    // Side face 3487
    brepBuilder.addCoedge(loopId3487, edgeId34, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId3487, edgeId48, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId3487, edgeId78, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId3487, edgeId37, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId3487);
    brepBuilder.finishFace(faceId3487);
    // Side face 4158
    brepBuilder.addCoedge(loopId4158, edgeId41, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId4158, edgeId15, OdBrepBuilder::kForward);
    brepBuilder.addCoedge(loopId4158, edgeId85, OdBrepBuilder::kReversed);
    brepBuilder.addCoedge(loopId4158, edgeId48, OdBrepBuilder::kReversed);
    brepBuilder.finishLoop(loopId4158);
    brepBuilder.finishFace(faceId4158);

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  //
  void addSeamedCylinder(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdBrepBuilder bbuilder;
    pDb->appServices()->brepBuilder(bbuilder, kSolid);

    // Create complex
    BRepBuilderGeometryId complexId = bbuilder.addComplex();

    // Create shell
    BRepBuilderGeometryId shellId = bbuilder.addShell(complexId);

    double radius = 3.15;
    OdGeInterval intrvlV(-5., 5.);
    OdSharedPtr <OdGeCylinder> cylSurf(new OdGeCylinder(radius, OdGePoint3d(0, 0, 5), OdGeVector3d(0, 0, 1), OdGeVector3d(0, 1, 0), intrvlV, 0, Oda2PI));

    OdSharedPtr <OdGePlane> top = new OdGePlane(OdGePoint3d(0, 0, 10), OdGeVector3d(1, 0, 0), OdGeVector3d(0, 1, 0));
    OdSharedPtr <OdGePlane> bottom = new OdGePlane(OdGePoint3d(0, 0, 0), OdGeVector3d(1, 0, 0), OdGeVector3d(0, 1, 0));

    // Add Faces
    BRepBuilderGeometryId cylFaceId = bbuilder.addFace(cylSurf.get(), OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId topFaceId = bbuilder.addFace(top.get(), OdBrepBuilder::kForward, shellId);
    BRepBuilderGeometryId bottomFaceId = bbuilder.addFace(bottom.get(), OdBrepBuilder::kReversed, shellId);

    OdSharedPtr <OdGeEllipArc3d> edgeBottom = new OdGeEllipArc3d(OdGePoint3d(0, 0, 0), OdGeVector3d::kXAxis, -OdGeVector3d::kYAxis, radius, radius, 0, Oda2PI);
    OdSharedPtr <OdGeEllipArc3d> edgeTop = new OdGeEllipArc3d(OdGePoint3d(0, 0, 10), OdGeVector3d::kXAxis, OdGeVector3d::kYAxis, radius, radius, 0, Oda2PI);
    OdSharedPtr <OdGeLineSeg3d> edgeSeam = new OdGeLineSeg3d(edgeBottom->evalPoint(0.0), edgeTop->evalPoint(0.0));

    BRepBuilderGeometryId edgeBottomId = bbuilder.addEdge(edgeBottom.get());
    BRepBuilderGeometryId edgeTopId = bbuilder.addEdge(edgeTop.get());
    BRepBuilderGeometryId edgeSeamId = bbuilder.addEdge(edgeSeam.get());

    BRepBuilderGeometryId loopId_cylinder = bbuilder.addLoop(cylFaceId);
    BRepBuilderGeometryId loopId_Top = bbuilder.addLoop(topFaceId);
    BRepBuilderGeometryId loopId_Bottom = bbuilder.addLoop(bottomFaceId);
    bbuilder.finishFace(cylFaceId);
    bbuilder.finishFace(topFaceId);
    bbuilder.finishFace(bottomFaceId);

    bbuilder.addCoedge(loopId_cylinder, edgeTopId, OdBrepBuilder::kReversed);
    bbuilder.addCoedge(loopId_cylinder, edgeSeamId, OdBrepBuilder::kReversed);
    bbuilder.addCoedge(loopId_cylinder, edgeBottomId, OdBrepBuilder::kReversed);
    bbuilder.addCoedge(loopId_cylinder, edgeSeamId, OdBrepBuilder::kForward);
    bbuilder.addCoedge(loopId_Top, edgeTopId, OdBrepBuilder::kForward);
    bbuilder.addCoedge(loopId_Bottom, edgeBottomId, OdBrepBuilder::kForward);
    bbuilder.finishLoop(loopId_cylinder);
    bbuilder.finishLoop(loopId_Top);
    bbuilder.finishLoop(loopId_Bottom);

    bbuilder.finishShell(shellId);
    bbuilder.finishComplex(complexId);

    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(bbuilder.finish());
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    OdDbBlockTableRecordPtr pMS = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
    pMS->appendOdDbEntity(pSolid);

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }


  //
  void addRotSurface1(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    const double radiusBottom = 4;
    const double radiusTop = 3;
    const double height = 2;
    const OdGeVector3d rotAxis(OdGeVector3d::kZAxis);
    const OdGeVector3d refAxis(OdGeVector3d::kXAxis);
    const OdGeVector3d yAxis(rotAxis.crossProduct(refAxis).normalize());
    const OdGePoint3d centerBottom(OdGePoint3d::kOrigin);
    const OdGePoint3d centerTop(centerBottom + rotAxis * height);

    // bottom -> top
    const OdGeLineSeg3d line(centerBottom + refAxis * radiusBottom, centerTop + refAxis * radiusTop);
    const OdGeLineSeg3d line2(centerBottom - refAxis * radiusBottom, centerTop - refAxis * radiusTop);
    // right -> left
    const OdGeCircArc3d arcBottom(centerBottom, rotAxis, refAxis, radiusBottom, 0, OdaPI);
    const OdGeCircArc3d arcTop(centerTop, rotAxis, refAxis, radiusTop, 0, OdaPI);
    // left -> right
    const OdGeLineSeg3d lineBottom(centerBottom - refAxis * radiusBottom, centerBottom + refAxis * radiusBottom);
    const OdGeLineSeg3d lineTop(centerTop - refAxis * radiusTop, centerTop + refAxis * radiusTop);

    const OdGePlane planeBottom(centerBottom, -rotAxis);
    const OdGePlane planeTop(centerTop, rotAxis);
    const OdGePlane planeSide(centerBottom, -yAxis);

    OdGeMatrix3d surfCoord;
    surfCoord.setCoordSystem(centerBottom, refAxis, yAxis, rotAxis).invert();
    OdGeLineSeg3d genLine(line);

    OdGeRevolvedSurface rotSurf;
    OdGeSpunSurf spunSurf;
    if (bUseSpunSurf)
    {
      spunSurf = OdGeSpunSurf(genLine, centerBottom, rotAxis);
      spunSurf.setAngles(0, OdaPI);
    }
    else
    {
      genLine.transformBy(surfCoord);
      rotSurf = OdGeRevolvedSurface(genLine, centerBottom, rotAxis, refAxis);
      rotSurf.setAngles(0, OdaPI);
    }
    // u --- curve param, v --- rot param
    const int coedgeDegree = 1;

    OdGePoint2dArray pointsArcBottom(2);
    pointsArcBottom.append(OdGePoint2d(0., 0.));
    pointsArcBottom.append(OdGePoint2d(0., OdaPI));
    OdGePoint2dArray pointsLine(2);
    pointsLine.append(OdGePoint2d(0., 0.));
    pointsLine.append(OdGePoint2d(1., 0.));
    OdGePoint2dArray pointsLine2(2);
    pointsLine2.append(OdGePoint2d(0., OdaPI));
    pointsLine2.append(OdGePoint2d(1., OdaPI));
    OdGePoint2dArray pointsArcTop(2);
    pointsArcTop.append(OdGePoint2d(1., 0.));
    pointsArcTop.append(OdGePoint2d(1., OdaPI));

    const double coedgeArrKnotsArcBottom[] = { 0., 0., OdaPI, OdaPI };
    const double coedgeArrKnotsLine[] = { 0., 0., 1., 1. };
    const double coedgeArrKnotsLine2[] = { 0., 0., 1., 1. };
    const double coedgeArrKnotsArcTop[] = { 0., 0., OdaPI, OdaPI };
    ODA_ASSUME(sizeof(coedgeArrKnotsArcBottom) / sizeof(*coedgeArrKnotsArcBottom) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsLine) / sizeof(*coedgeArrKnotsLine) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsLine2) / sizeof(*coedgeArrKnotsLine2) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsArcTop) / sizeof(*coedgeArrKnotsArcTop) == 4);

    OdGeKnotVector coedgeKnotsArcBottom(4, coedgeArrKnotsArcBottom);
    OdGeKnotVector coedgeKnotsLine(4, coedgeArrKnotsLine);
    OdGeKnotVector coedgeKnotsLine2(4, coedgeArrKnotsLine2);
    OdGeKnotVector coedgeKnotsArcTop(4, coedgeArrKnotsArcTop);

    OdGeNurbCurve2d coedgeArcBottom(coedgeDegree, coedgeKnotsArcBottom, pointsArcBottom);
    OdGeNurbCurve2d coedgeLine(coedgeDegree, coedgeKnotsLine, pointsLine);
    OdGeNurbCurve2d coedgeLine2(coedgeDegree, coedgeKnotsLine2, pointsLine2);
    OdGeNurbCurve2d coedgeArcTop(coedgeDegree, coedgeKnotsArcTop, pointsArcTop);


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId edgeIdLine = brepBuilder.addEdge(&line);
    BRepBuilderGeometryId edgeIdLine2 = brepBuilder.addEdge(&line2);
    BRepBuilderGeometryId edgeIdArcBottom = brepBuilder.addEdge(&arcBottom);
    BRepBuilderGeometryId edgeIdArcTop = brepBuilder.addEdge(&arcTop);
    BRepBuilderGeometryId edgeIdLineBottom = brepBuilder.addEdge(&lineBottom);
    BRepBuilderGeometryId edgeIdLineTop = brepBuilder.addEdge(&lineTop);

    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeBottom, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kReversed);
      brepBuilder.addCoedge(loopId, edgeIdLineBottom, OdBrepBuilder::kReversed);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeTop, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder::kForward);
      brepBuilder.addCoedge(loopId, edgeIdLineTop, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeSide, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdLineTop, OdBrepBuilder::kReversed);
      brepBuilder.addCoedge(loopId, edgeIdLine2, OdBrepBuilder::kReversed);
      brepBuilder.addCoedge(loopId, edgeIdLineBottom, OdBrepBuilder::kForward);
      brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = bUseSpunSurf ? brepBuilder.addFace(&spunSurf, OdBrepBuilder::kReversed, shellId) : brepBuilder.addFace(&rotSurf, OdBrepBuilder::kForward, shellId);;
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kForward, &coedgeArcBottom);
      brepBuilder.addCoedge(loopId, edgeIdLine2, OdBrepBuilder::kForward, &coedgeLine2);
      brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder::kReversed, &coedgeArcTop);
      brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder::kReversed, &coedgeLine);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }
  void addRotSurface2(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    const double cospi4 = 0.5 * sqrt(2.);
    const double radiusBottom = 4;
    const double height = 2;
    const OdGeVector3d rotAxis(OdGeVector3d::kYAxis);
    const OdGeVector3d refAxis(cospi4, 0., cospi4);
    const OdGeVector3d yAxis(rotAxis.crossProduct(refAxis).normalize());
    const OdGePoint3d centerBottom(0., 0., -2.);
    const OdGePoint3d centerTop(centerBottom + rotAxis * height);

    // bottom -> top
    const OdGeLineSeg3d line(centerBottom + refAxis * radiusBottom, centerTop);
    const OdGeLineSeg3d line2(centerBottom - refAxis * radiusBottom, centerTop);
    // right -> left
    const OdGeCircArc3d arcBottom(centerBottom, rotAxis, refAxis, radiusBottom, 0, OdaPI);
    // left -> right
    const OdGeLineSeg3d lineBottom(centerBottom - refAxis * radiusBottom, centerBottom + refAxis * radiusBottom);

    const OdGePlane planeBottom(centerBottom, -rotAxis);
    const OdGePlane planeSide(centerBottom, -yAxis);

    OdGeMatrix3d surfCoord;
    surfCoord.setCoordSystem(centerBottom, refAxis, yAxis, rotAxis).invert();
    OdGeLineSeg3d genLine(line);
    OdGeRevolvedSurface rotSurf;
    OdGeSpunSurf spunSurf;
    if (bUseSpunSurf)
    {
      spunSurf = OdGeSpunSurf(genLine, centerBottom, rotAxis);
      spunSurf.setAngles(0, OdaPI);
    }
    else
    {
      genLine.transformBy(surfCoord);
      rotSurf = OdGeRevolvedSurface(genLine, centerBottom, rotAxis, refAxis);
      rotSurf.setAngles(0, OdaPI);
    }

    // u --- curve param, v --- rot param
    const int coedgeDegree = 1;

    OdGePoint2dArray pointsArcBottom(2);
    pointsArcBottom.append(OdGePoint2d(0., 0.));
    pointsArcBottom.append(OdGePoint2d(0., OdaPI));
    OdGePoint2dArray pointsLine(2);
    pointsLine.append(OdGePoint2d(0., 0.));
    pointsLine.append(OdGePoint2d(1., 0.));
    OdGePoint2dArray pointsLine2(2);
    pointsLine2.append(OdGePoint2d(0., OdaPI));
    pointsLine2.append(OdGePoint2d(1., OdaPI));

    const double coedgeArrKnotsArcBottom[] = { 0., 0., OdaPI, OdaPI };
    const double coedgeArrKnotsLine[] = { 0., 0., 1., 1. };
    const double coedgeArrKnotsLine2[] = { 0., 0., 1., 1. };
    ODA_ASSUME(sizeof(coedgeArrKnotsArcBottom) / sizeof(*coedgeArrKnotsArcBottom) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsLine) / sizeof(*coedgeArrKnotsLine) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsLine2) / sizeof(*coedgeArrKnotsLine2) == 4);

    OdGeKnotVector coedgeKnotsArcBottom(4, coedgeArrKnotsArcBottom);
    OdGeKnotVector coedgeKnotsLine(4, coedgeArrKnotsLine);
    OdGeKnotVector coedgeKnotsLine2(4, coedgeArrKnotsLine2);

    OdGeNurbCurve2d coedgeArcBottom(coedgeDegree, coedgeKnotsArcBottom, pointsArcBottom);
    OdGeNurbCurve2d coedgeLine(coedgeDegree, coedgeKnotsLine, pointsLine);
    OdGeNurbCurve2d coedgeLine2(coedgeDegree, coedgeKnotsLine2, pointsLine2);


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId edgeIdLine = brepBuilder.addEdge(&line);
    BRepBuilderGeometryId edgeIdLine2 = brepBuilder.addEdge(&line2);
    BRepBuilderGeometryId edgeIdArcBottom = brepBuilder.addEdge(&arcBottom);
    BRepBuilderGeometryId edgeIdLineBottom = brepBuilder.addEdge(&lineBottom);

    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeBottom, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kReversed);
      brepBuilder.addCoedge(loopId, edgeIdLineBottom, OdBrepBuilder::kReversed);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeSide, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdLine2, OdBrepBuilder::kReversed);
      brepBuilder.addCoedge(loopId, edgeIdLineBottom, OdBrepBuilder::kForward);
      brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = bUseSpunSurf ? brepBuilder.addFace(&spunSurf, OdBrepBuilder::kReversed, shellId) : brepBuilder.addFace(&rotSurf, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kForward, &coedgeArcBottom);
      brepBuilder.addCoedge(loopId, edgeIdLine2, OdBrepBuilder::kForward, &coedgeLine2);
      brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder::kReversed, &coedgeLine);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }


    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  void addRotSurface3(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    const double majorRadius = 10.;
    const double minorRadius = 4.;
    const double rotSurfRadius = 2;
    const double arcStartAngle = OdaPI2;
    const double arcEndAngle = OdaPI2 + OdaPI / 3;
    const OdGeVector3d rotAxis(OdGeVector3d::kZAxis);
    const OdGeVector3d refAxis(OdGeVector3d::kXAxis);
    const OdGeVector3d yAxis(rotAxis.crossProduct(refAxis).normalize());
    const OdGePoint3d center(OdGePoint3d::kOrigin);
    const OdGePoint3d arcCenter(center - refAxis * rotSurfRadius);

    // bottom -> top
    const OdGeEllipArc3d arc(arcCenter, -OdGeVector3d::kZAxis, OdGeVector3d::kXAxis, majorRadius, minorRadius, arcStartAngle, arcEndAngle);

    const OdGeCircArc3d arcBottom(center, rotAxis, refAxis, rotSurfRadius, 0, Oda2PI);

    const OdGePlane planeBottom(center, -rotAxis);

    OdGeMatrix3d surfCoord;
    surfCoord.setCoordSystem(center, refAxis, yAxis, rotAxis).invert();
    OdGeEllipArc3d genArc(arc);

    OdGeRevolvedSurface rotSurf;
    OdGeSpunSurf spunSurf;
    if (bUseSpunSurf)
    {
      spunSurf = OdGeSpunSurf(genArc, center, rotAxis);
      spunSurf.setAngles(0, Oda2PI);
    }
    else
    {
      genArc.transformBy(surfCoord);
      rotSurf = OdGeRevolvedSurface(genArc, center, rotAxis, refAxis);
      rotSurf.setAngles(0, Oda2PI);
    }

    // u --- curve param, v --- rot param
    const int coedgeDegree = 1;

    OdGePoint2dArray pointsArc(2);
    pointsArc.append(OdGePoint2d(arcStartAngle, 0.));
    pointsArc.append(OdGePoint2d(arcEndAngle, 0.));
    OdGePoint2dArray pointsArcBottom(2);
    pointsArcBottom.append(OdGePoint2d(arcStartAngle, 0.));
    pointsArcBottom.append(OdGePoint2d(arcStartAngle, Oda2PI));

    const double coedgeArrKnotsArc[] = { arcStartAngle, arcStartAngle, arcEndAngle, arcEndAngle };
    const double coedgeArrKnotsArcBottom[] = { 0., 0., Oda2PI, Oda2PI };
    ODA_ASSUME(sizeof(coedgeArrKnotsArc) / sizeof(*coedgeArrKnotsArc) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsArcBottom) / sizeof(*coedgeArrKnotsArcBottom) == 4);

    OdGeKnotVector coedgeKnotsArc(4, coedgeArrKnotsArc);
    OdGeKnotVector coedgeKnotsArcBottom(4, coedgeArrKnotsArcBottom);

    OdGeNurbCurve2d coedgeArc(coedgeDegree, coedgeKnotsArc, pointsArc);
    OdGeNurbCurve2d coedgeArcBottom(coedgeDegree, coedgeKnotsArcBottom, pointsArcBottom);


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);


    BRepBuilderGeometryId edgeIdArc = brepBuilder.addEdge(&arc);
    BRepBuilderGeometryId edgeIdArcBottom = brepBuilder.addEdge(&arcBottom);

    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeBottom, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kReversed);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = bUseSpunSurf ? brepBuilder.addFace(&spunSurf, OdBrepBuilder::kReversed, shellId) : brepBuilder.addFace(&rotSurf, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder::kReversed, &coedgeArc);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kForward, &coedgeArcBottom);
      brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder::kForward, &coedgeArc);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }
  void addRotSurface4(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    const double majorRadius = 10.;
    const double minorRadius = 4.;
    const double rotSurfRadius = 2;
    const double arcStartAngle = OdaPI2;
    const double arcEndAngle = OdaPI2 + OdaPI / 3;
    const OdGeVector3d rotAxis(OdGeVector3d::kZAxis);
    const OdGeVector3d refAxis(OdGeVector3d::kXAxis);
    const OdGeVector3d yAxis(rotAxis.crossProduct(refAxis).normalize());
    const OdGePoint3d center(OdGePoint3d::kOrigin);
    const OdGePoint3d arcCenter(OdGePoint3d::kOrigin);
    const OdGePoint3d centerTop(center - rotAxis * majorRadius * cos(arcEndAngle));

    // bottom -> top
    const OdGeEllipArc3d arc(arcCenter, -OdGeVector3d::kZAxis, OdGeVector3d::kXAxis, majorRadius, minorRadius, arcStartAngle, arcEndAngle);

    const OdGeCircArc3d arcBottom(center, rotAxis, refAxis, minorRadius, 0, Oda2PI);
    const OdGeCircArc3d arcTop(centerTop, rotAxis, refAxis, rotSurfRadius, 0, Oda2PI);

    const OdGePlane planeBottom(center, -rotAxis);
    const OdGePlane planeTop(centerTop, rotAxis);

    OdGeMatrix3d surfCoord;
    surfCoord.setCoordSystem(center, refAxis, yAxis, rotAxis).invert();
    OdGeEllipArc3d genArc(arc);

    OdGeRevolvedSurface rotSurf;
    OdGeSpunSurf spunSurf;
    if (bUseSpunSurf)
    {
      spunSurf = OdGeSpunSurf(genArc, center, rotAxis);
      spunSurf.setAngles(0, Oda2PI);
    }
    else
    {
      genArc.transformBy(surfCoord);
      rotSurf = OdGeRevolvedSurface(genArc, center, rotAxis, refAxis);
      rotSurf.setAngles(0, Oda2PI);
    }

    // u --- curve param, v --- rot param
    const int coedgeDegree = 1;

    OdGePoint2dArray pointsArc(2);
    pointsArc.append(OdGePoint2d(arcStartAngle, 0.));
    pointsArc.append(OdGePoint2d(arcEndAngle, 0.));
    OdGePoint2dArray pointsArcBottom(2);
    pointsArcBottom.append(OdGePoint2d(arcStartAngle, 0.));
    pointsArcBottom.append(OdGePoint2d(arcStartAngle, Oda2PI));
    OdGePoint2dArray pointsArcTop(2);
    pointsArcTop.append(OdGePoint2d(arcEndAngle, 0.));
    pointsArcTop.append(OdGePoint2d(arcEndAngle, Oda2PI));

    const double coedgeArrKnotsArc[] = { arcStartAngle, arcStartAngle, arcEndAngle, arcEndAngle };
    const double coedgeArrKnotsArcBottom[] = { 0., 0., Oda2PI, Oda2PI };
    const double coedgeArrKnotsArcTop[] = { 0., 0., Oda2PI, Oda2PI };
    ODA_ASSUME(sizeof(coedgeArrKnotsArc) / sizeof(*coedgeArrKnotsArc) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsArcBottom) / sizeof(*coedgeArrKnotsArcBottom) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsArcTop) / sizeof(*coedgeArrKnotsArcTop) == 4);

    OdGeKnotVector coedgeKnotsArc(4, coedgeArrKnotsArc);
    OdGeKnotVector coedgeKnotsArcBottom(4, coedgeArrKnotsArcBottom);
    OdGeKnotVector coedgeKnotsArcTop(4, coedgeArrKnotsArcTop);

    OdGeNurbCurve2d coedgeArc(coedgeDegree, coedgeKnotsArc, pointsArc);
    OdGeNurbCurve2d coedgeArcBottom(coedgeDegree, coedgeKnotsArcBottom, pointsArcBottom);
    OdGeNurbCurve2d coedgeArcTop(coedgeDegree, coedgeKnotsArcTop, pointsArcTop);


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);


    BRepBuilderGeometryId edgeIdArc = brepBuilder.addEdge(&arc);
    BRepBuilderGeometryId edgeIdArcBottom = brepBuilder.addEdge(&arcBottom);
    BRepBuilderGeometryId edgeIdArcTop = brepBuilder.addEdge(&arcTop);

    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeBottom, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kReversed);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeTop, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = bUseSpunSurf ? brepBuilder.addFace(&spunSurf, OdBrepBuilder::kReversed, shellId) : brepBuilder.addFace(&rotSurf, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder::kReversed, &coedgeArc);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kForward, &coedgeArcBottom);
      brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder::kForward, &coedgeArc);
      brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder::kReversed, &coedgeArcTop);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }


  void addRotSurface5(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, bool bUseSpunSurf, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    const double radius = 4.;
    const OdGePoint3d center(0., 0., 0.);
    const double arcStartAngle = 0.;
    const double arcEndAngle = OdaPI2;
    const OdGeVector3d rotAxis(OdGeVector3d::kZAxis);
    const OdGeVector3d refAxis(OdGeVector3d::kXAxis);
    const OdGeVector3d yAxis(rotAxis.crossProduct(refAxis).normalize());

    // top -> bottom
    const OdGeCircArc3d arc(center, OdGeVector3d::kYAxis, rotAxis, radius, arcStartAngle, arcEndAngle);
    // bottom -> top
    const OdGeLineSeg3d line(center + refAxis * radius, center + rotAxis * radius * 0.5);

    const OdGeCircArc3d arcBottom(center, rotAxis, refAxis, radius, 0, Oda2PI);

    OdGeMatrix3d surfCoord;
    surfCoord.setCoordSystem(center, refAxis, yAxis, rotAxis).invert();
    OdGeCircArc3d genArc(arc);

    OdGeRevolvedSurface rotSurf1;
    OdGeSpunSurf spunSurf1;
    if (bUseSpunSurf)
    {
      spunSurf1 = OdGeSpunSurf(genArc, center, rotAxis);
      spunSurf1.setAngles(0, Oda2PI);
    }
    else
    {
      genArc.transformBy(surfCoord);
      rotSurf1 = OdGeRevolvedSurface(genArc, center, rotAxis, refAxis);
      rotSurf1.setAngles(0, Oda2PI);
    }

    OdGeLineSeg3d genLine(line);
    OdGeRevolvedSurface rotSurf2;
    OdGeSpunSurf spunSurf2;
    if (bUseSpunSurf)
    {
      spunSurf2 = OdGeSpunSurf(genLine, center, rotAxis);
      spunSurf2.setAngles(0, Oda2PI);
      spunSurf2.reverseNormal();
    }
    else
    {
      genLine.transformBy(surfCoord);
      rotSurf2 = OdGeRevolvedSurface(genLine, center, rotAxis, refAxis);
      rotSurf2.setAngles(0, Oda2PI);
      rotSurf2.reverseNormal();
    }

    // u --- curve param, v --- rot param
    const int coedgeDegree = 1;

    // rotSurf1
    OdGePoint2dArray pointsArc(2);
    pointsArc.append(OdGePoint2d(arcStartAngle, 0.));
    pointsArc.append(OdGePoint2d(arcEndAngle, 0.));
    OdGePoint2dArray pointsArcBottom1(2);
    pointsArcBottom1.append(OdGePoint2d(arcEndAngle, 0.));
    pointsArcBottom1.append(OdGePoint2d(arcEndAngle, Oda2PI));

    const double coedgeArrKnotsArc[] = { arcStartAngle, arcStartAngle, arcEndAngle, arcEndAngle };
    const double coedgeArrKnotsArcBottom1[] = { 0., 0., Oda2PI, Oda2PI };
    ODA_ASSUME(sizeof(coedgeArrKnotsArc) / sizeof(*coedgeArrKnotsArc) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsArcBottom1) / sizeof(*coedgeArrKnotsArcBottom1) == 4);

    OdGeKnotVector coedgeKnotsArc(4, coedgeArrKnotsArc);
    OdGeKnotVector coedgeKnotsArcBottom1(4, coedgeArrKnotsArcBottom1);

    // rotSurf2
    OdGePoint2dArray pointsLine(2);
    pointsLine.append(OdGePoint2d(0., 0.));
    pointsLine.append(OdGePoint2d(1., 0.));
    OdGePoint2dArray pointsArcBottom2(2);
    pointsArcBottom2.append(OdGePoint2d(0., 0.));
    pointsArcBottom2.append(OdGePoint2d(0., Oda2PI));

    const double coedgeArrKnotsArcBottom2[] = { 0., 0., Oda2PI, Oda2PI };
    const double coedgeArrKnotsLine[] = { 0., 0., 1., 1. };
    ODA_ASSUME(sizeof(coedgeArrKnotsArcBottom2) / sizeof(*coedgeArrKnotsArcBottom2) == 4);
    ODA_ASSUME(sizeof(coedgeArrKnotsLine) / sizeof(*coedgeArrKnotsLine) == 4);

    OdGeKnotVector coedgeKnotsArcBottom2(4, coedgeArrKnotsArcBottom2);
    OdGeKnotVector coedgeKnotsLine(4, coedgeArrKnotsLine);

    //
    OdGeNurbCurve2d coedgeArc(coedgeDegree, coedgeKnotsArc, pointsArc);
    OdGeNurbCurve2d coedgeArcBottom1(coedgeDegree, coedgeKnotsArcBottom1, pointsArcBottom1);
    OdGeNurbCurve2d coedgeLine(coedgeDegree, coedgeKnotsLine, pointsLine);
    OdGeNurbCurve2d coedgeArcBottom2(coedgeDegree, coedgeKnotsArcBottom2, pointsArcBottom2);


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);


    BRepBuilderGeometryId edgeIdArc = brepBuilder.addEdge(&arc);
    BRepBuilderGeometryId edgeIdLine = brepBuilder.addEdge(&line);
    BRepBuilderGeometryId edgeIdArcBottom = brepBuilder.addEdge(&arcBottom);

    {
      BRepBuilderGeometryId faceId = bUseSpunSurf ? brepBuilder.addFace(&spunSurf1, OdBrepBuilder::kForward, shellId) : brepBuilder.addFace(&rotSurf1, OdBrepBuilder::kReversed, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder::kForward, &coedgeArc);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kForward, &coedgeArcBottom1);
      brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder::kReversed, &coedgeArc);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = bUseSpunSurf ? brepBuilder.addFace(&spunSurf2, OdBrepBuilder::kReversed, shellId) : brepBuilder.addFace(&rotSurf2, OdBrepBuilder::kForward, shellId);;
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder::kReversed, &coedgeLine);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kReversed, &coedgeArcBottom2);
      brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder::kForward, &coedgeLine);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  //
  void addEllipCylinder(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    const double majorRadius = 4.;
    const double minorRadius = 2.;
    const double height = 5.;
    const OdGeInterval heightInterval(0., height);
    const double startAngle = 0.;
    const double endAngle = Oda2PI;
    const OdGeVector3d majorAxis(OdGeVector3d::kXAxis);
    const OdGeVector3d minorAxis(OdGeVector3d::kYAxis);
    const OdGeVector3d normal(majorAxis.crossProduct(minorAxis));
    const OdGePoint3d centerBottom(OdGePoint3d::kOrigin);
    const OdGePoint3d centerTop(centerBottom + normal * height);

    //
    const OdGeEllipArc3d arcBottom(centerBottom, majorAxis, minorAxis, majorRadius, minorRadius, startAngle, endAngle);
    const OdGeEllipArc3d arcTop(centerTop, majorAxis, minorAxis, majorRadius, minorRadius, startAngle, endAngle);

    const OdGePlane planeBottom(centerBottom, majorAxis, minorAxis);
    const OdGePlane planeTop(centerTop, majorAxis, minorAxis);
    const OdGeEllipCylinder cylinder(minorRadius, majorRadius, centerBottom, normal, majorAxis, heightInterval, startAngle, endAngle);


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);


    BRepBuilderGeometryId edgeIdArcBottom = brepBuilder.addEdge(&arcBottom);
    BRepBuilderGeometryId edgeIdArcTop = brepBuilder.addEdge(&arcTop);

    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeBottom, OdBrepBuilder::kReversed, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kReversed);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeTop, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&cylinder, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder::kReversed);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  void addEllipCone(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/)
  {
    OdResult res = eOk;

    // Ge
    const double majorRadius = 4.;
    const double minorRadius = 2.;
    const double height = 5.;
    const double coneCos = height / sqrt(height * height + majorRadius * majorRadius);
    const double coneSin = majorRadius / sqrt(height * height + majorRadius * majorRadius);
    const OdGeInterval heightInterval(-height, 0.);
    const double startAngle = 0.;
    const double endAngle = Oda2PI;
    const OdGeVector3d majorAxis(OdGeVector3d::kXAxis);
    const OdGeVector3d minorAxis(OdGeVector3d::kYAxis);
    const OdGeVector3d normal(majorAxis.crossProduct(minorAxis));
    const OdGePoint3d centerBottom(OdGePoint3d::kOrigin);
    const OdGePoint3d apex(centerBottom + normal * height);

    //
    const OdGeEllipArc3d arcBottom(centerBottom, majorAxis, minorAxis, majorRadius, minorRadius, startAngle, endAngle);
    const OdGeLineSeg3d apexLine(apex, apex);

    const OdGePlane planeBottom(centerBottom, majorAxis, minorAxis);
    const OdGeEllipCone cone(coneCos, coneSin, centerBottom, minorRadius, majorRadius, -normal, majorAxis, heightInterval, startAngle, endAngle);


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);


    BRepBuilderGeometryId edgeIdArcBottom = brepBuilder.addEdge(&arcBottom);
    BRepBuilderGeometryId edgeIdApexLine = brepBuilder.addEdge(&apexLine);

    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeBottom, OdBrepBuilder::kReversed, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kReversed);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&cone, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdApexLine, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  // Tolerant model
  void addTCoedgeEllipCylinder(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/, double deltaRadius /*= 1e-3*/)
  {
    OdResult res = eOk;

    // Ge
    const double majorRadius = 4.;
    const double minorRadius = 2.;
    const double height = 5.;
    const OdGeInterval heightInterval(0., height);
    const double startAngle = 0.;
    const double endAngle = Oda2PI;
    const OdGeVector3d majorAxis(OdGeVector3d::kXAxis);
    const OdGeVector3d minorAxis(OdGeVector3d::kYAxis);
    const OdGeVector3d normal(majorAxis.crossProduct(minorAxis));
    const OdGePoint3d centerBottom(OdGePoint3d::kOrigin);
    const OdGePoint3d centerTop(centerBottom + normal * height);

    //
    const double newMajorRadius = majorRadius + deltaRadius;
    const double newMinorRadius = minorRadius;
    const OdGeEllipArc3d arcBottom(centerBottom, majorAxis, minorAxis, newMajorRadius, newMinorRadius, startAngle, endAngle);
    const OdGeEllipArc3d arcTop(centerTop, majorAxis, minorAxis, majorRadius, minorRadius, startAngle, endAngle);

    const OdGeEllipArc2d arcBottom2d(centerBottom.convert2d(), majorAxis.convert2d(), minorAxis.convert2d(), majorRadius, minorRadius, startAngle, endAngle);

    const OdGePlane planeBottom(centerBottom, majorAxis, minorAxis);
    const OdGePlane planeTop(centerTop, majorAxis, minorAxis);
    const OdGeEllipCylinder cylinder(minorRadius, majorRadius, centerBottom, normal, majorAxis, heightInterval, startAngle, endAngle);

    //
    OdGeNurbCurve2d coedgeArcBottom;
    setupUVCurve(coedgeArcBottom, OdGePoint2d(0.0, startAngle), OdGePoint2d(0.0, endAngle), startAngle, endAngle);
    OdGeTempNurbCurve2dPtr pCoedgeArcBottomPlane(OdGeNurbCurve2d::convertFrom(&arcBottom2d));

    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);


    BRepBuilderGeometryId edgeIdArcBottom = brepBuilder.addEdge(&arcBottom);
    BRepBuilderGeometryId edgeIdArcTop = brepBuilder.addEdge(&arcTop);

    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeBottom, OdBrepBuilder::kReversed, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kReversed, pCoedgeArcBottomPlane);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&planeTop, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder::kForward);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }
    {
      BRepBuilderGeometryId faceId = brepBuilder.addFace(&cylinder, OdBrepBuilder::kForward, shellId);
      BRepBuilderGeometryId loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder::kForward, &coedgeArcBottom);
      brepBuilder.finishLoop(loopId);
      loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder::kReversed);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

  //
  void addTCoedgeFourthSphere(OdDbHostAppServices* pHostApp, OdDbDatabasePtr pDb, const OdGeVector3d& offset, OdDb3dSolidPtr* pSolidStore /*= NULL*/, double deltaRadius /*= 1e-3*/)
  {
    OdResult res = eOk;

    // Ge
    double radius = 3;
    OdGePoint3d center(0., 0., 0.);
    OdGeVector3d normal(OdGeVector3d::kZAxis);

    OdGeVector3d northAxis = OdGeVector3d::kZAxis;
    OdGeVector3d refAxis = OdGeVector3d::kXAxis;
    double startAngleU = -OdaPI2;
    double endAngleU = OdaPI2;
    double startAngleV = 0.;
    double endAngleV = Oda2PI;
    double cutAngleV = OdaPI;
    // all face normal is outer
    OdGeSphere sphere(radius, center, northAxis, refAxis,
      startAngleU, endAngleU, startAngleV, endAngleV);
    OdGePlane planeBottom(center, -OdGeVector3d::kXAxis, OdGeVector3d::kYAxis);// normal = -OdGeVector3d::kZAxis
    OdGePlane planeSide(center, OdGeVector3d::kXAxis, OdGeVector3d::kZAxis);// normal = -OdGeVector3d::kYAxis

    OdGeCircArc3d arcBottom(center, -normal, -refAxis, radius + deltaRadius, startAngleV, cutAngleV);
    OdGeLineSeg3d lineBottom(center - refAxis * (radius + deltaRadius), center + refAxis * (radius + deltaRadius));
    OdGeCircArc3d arcTop1(center, -OdGeVector3d::kYAxis, refAxis, radius + deltaRadius, 0., OdaPI2);
    OdGeCircArc3d arcTop2(center, -OdGeVector3d::kYAxis, refAxis, radius + deltaRadius, OdaPI2, OdaPI);

    //
    OdGeNurbCurve2d coedgeArcBottomSphere;
    setupUVCurve(coedgeArcBottomSphere, OdGePoint2d(0.0, OdaPI), OdGePoint2d(0.0, 0.0), 0.0, OdaPI);

    OdGeCircArc2d arcBottomPlaneBottom(center.convert2d(), radius, 0.0, OdaPI, OdGeVector2d::kXAxis);
    OdGeTempNurbCurve2dPtr pCoedgeArcBottomPlaneBottom(OdGeNurbCurve2d::convertFrom(&arcBottomPlaneBottom));

    OdGeNurbCurve2d coedgeLineBottomPlaneBottom;
    setupUVCurve(coedgeLineBottomPlaneBottom, OdGePoint2d(radius, 0.0), OdGePoint2d(-radius, 0.0), 0.0, 1.0);
    OdGeNurbCurve2d coedgeLineBottomPlaneSide;
    setupUVCurve(coedgeLineBottomPlaneSide, OdGePoint2d(-radius, 0.0), OdGePoint2d(radius, 0.0), 0.0, 1.0);

    OdGeNurbCurve2d coedgeArcTop1Sphere;
    setupUVCurve(coedgeArcTop1Sphere, OdGePoint2d(0.0, 0.0), OdGePoint2d(OdaPI2, 0.0), 0.0, OdaPI2);
    OdGeNurbCurve2d coedgeArcTop2Sphere;
    setupUVCurve(coedgeArcTop2Sphere, OdGePoint2d(OdaPI2, OdaPI), OdGePoint2d(0.0, OdaPI), OdaPI2, OdaPI);

    OdGeCircArc2d arcTop1PlaneSide(center.convert2d(), radius, 0.0, OdaPI2, OdGeVector2d::kXAxis);
    OdGeCircArc2d arcTop2PlaneSide(center.convert2d(), radius, OdaPI2, OdaPI, OdGeVector2d::kXAxis);
    OdGeTempNurbCurve2dPtr pCoedgeArcTop1PlaneSide(OdGeNurbCurve2d::convertFrom(&arcTop1PlaneSide));
    OdGeTempNurbCurve2dPtr pCoedgeArcTop2PlaneSide(OdGeNurbCurve2d::convertFrom(&arcTop2PlaneSide));


    // Prepare
    OdBrepBuilder brepBuilder;
    res = pHostApp->brepBuilder(brepBuilder, kSolid);
    OD_CHECK_THROW(eOk == res, res, "Fail to init BrepBuilder");

    // Construct
    BRepBuilderGeometryId complexId = brepBuilder.addComplex();
    BRepBuilderGeometryId shellId = brepBuilder.addShell(complexId);

    BRepBuilderGeometryId faceId;
    BRepBuilderGeometryId loopId;
    BRepBuilderGeometryId arcBottomEdgeId = brepBuilder.addEdge(&arcBottom);
    BRepBuilderGeometryId lineBottomEdgeId = brepBuilder.addEdge(&lineBottom);
    BRepBuilderGeometryId arcTop1EdgeId = brepBuilder.addEdge(&arcTop1);
    BRepBuilderGeometryId arcTop2EdgeId = brepBuilder.addEdge(&arcTop2);

    {
      faceId = brepBuilder.addFace(&sphere, OdBrepBuilder::kForward, shellId);
      loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, arcBottomEdgeId, OdBrepBuilder::kReversed, &coedgeArcBottomSphere);
      brepBuilder.addCoedge(loopId, arcTop2EdgeId, OdBrepBuilder::kReversed, &coedgeArcTop2Sphere);
      brepBuilder.addCoedge(loopId, arcTop1EdgeId, OdBrepBuilder::kReversed, &coedgeArcTop1Sphere);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    {
      faceId = brepBuilder.addFace(&planeBottom, OdBrepBuilder::kForward, shellId);
      loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, arcBottomEdgeId, OdBrepBuilder::kForward, pCoedgeArcBottomPlaneBottom);
      brepBuilder.addCoedge(loopId, lineBottomEdgeId, OdBrepBuilder::kReversed, &coedgeLineBottomPlaneBottom);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }

    {
      faceId = brepBuilder.addFace(&planeSide, OdBrepBuilder::kForward, shellId);
      loopId = brepBuilder.addLoop(faceId);
      brepBuilder.addCoedge(loopId, lineBottomEdgeId, OdBrepBuilder::kForward, &coedgeLineBottomPlaneSide);
      brepBuilder.addCoedge(loopId, arcTop1EdgeId, OdBrepBuilder::kForward, pCoedgeArcTop1PlaneSide);
      brepBuilder.addCoedge(loopId, arcTop2EdgeId, OdBrepBuilder::kForward, pCoedgeArcTop2PlaneSide);
      brepBuilder.finishLoop(loopId);
      brepBuilder.finishFace(faceId);
    }


    brepBuilder.finishShell(shellId);
    brepBuilder.finishComplex(complexId);

    //
    OdRxObjectPtr pModel = brepBuilder.finish();
    OD_CHECK_THROW(!pModel.isNull(), eNullPtr, "BrepBuilder fail to create entity");
    OdDb3dSolidPtr pSolid = OdDb3dSolid::createObject();
    pSolid->setBody(pModel);
    pSolid->transformBy(OdGeMatrix3d().setTranslation(offset));

    {
      OdDbBlockTableRecordPtr pMs = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);
      pMs->appendOdDbEntity(pSolid);
    }

    if (pSolidStore)
    {
      *pSolidStore = pSolid;
    }
  }

}
