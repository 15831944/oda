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

#include "StdAfx.h"
#include "DgLineCurvePEImpl.h"
#include "DgLine.h"
#include "RxObjectImpl.h"
#include "Ge/GeLine2d.h"
#include "Ge/GeLineSeg2d.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeLine3d.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GePlane.h"
#include "DgBSplineCurve.h"

//----------------------------------------------------------
//
// OdDgLineElement2dCurvePE
//
//----------------------------------------------------------

OdResult OdDgLineElement2dCurvePE::getOffsetCurves(const OdDgCurveElement2d* pCurve, 
                                                   double offsetDist,  
                                                   OdRxObjectPtrArray& offsetCurves) const
{
  OdDgLine2dPtr pLine = pCurve;
  OdGePoint2d ptStart = pLine->getStartPoint();
  OdGePoint2d ptEnd   = pLine->getEndPoint();
  OdGeVector2d vrOffsetDir = ptEnd - ptStart;
  vrOffsetDir.rotateBy(OdaPI2);
  vrOffsetDir.normalizeGetLength();

  OdDgLine2dPtr newLine = OdDgLine2d::createObject();
  newLine->setStartPoint( ptStart + vrOffsetDir * offsetDist );
  newLine->setEndPoint( ptEnd + vrOffsetDir * offsetDist );
  offsetCurves.append(newLine.get());
  return eOk;
}

OdResult OdDgLineElement2dCurvePE::getSplitCurves(const OdDgCurveElement2d* pCurve, 
                                                  const OdGeDoubleArray& params, 
                                                  OdRxObjectPtrArray& curveSegments) const
{
  OdDgLine2dPtr pLine = pCurve;
  OdGePoint2d ptStart = pLine->getStartPoint();
  OdGeVector2d vrDir = pLine->getEndPoint() - ptStart;

  if( vrDir.isZeroLength() )
  {
    return eDegenerateGeometry;
  }

  double dLength = vrDir.length();
  vrDir.normalize();

  double prevParam = 0;

  for( OdUInt32 i = 0; i < params.size(); i++ )
  {
    if( params[i] > dLength || params[i] < 0 )
    {
      return eInvalidInput;
    }

    if( prevParam != params[i] )
    {
      OdDgLine2dPtr newLine = OdDgLine2d::createObject();
      newLine->setStartPoint( ptStart + vrDir*prevParam );
      newLine->setEndPoint( ptStart + vrDir*params[i] );
      curveSegments.append( newLine.get() );
    }

    prevParam = params[i];
  }

  if( prevParam != dLength )
  {
    OdDgLine2dPtr newLine = OdDgLine2d::createObject();
    newLine->setStartPoint( ptStart + vrDir*prevParam );
    newLine->setEndPoint( ptStart + vrDir*dLength );
    curveSegments.append( newLine.get() );
  }

  return eOk;
}

OdResult OdDgLineElement2dCurvePE::getSplitCurves( const OdDgCurveElement2d* pCurve, 
                                                  const OdGePoint2dArray& arrPoints, 
                                                  OdRxObjectPtrArray& curveSegments ) const
{
  OdDgLine2dPtr pLine   = pCurve;
  OdGePoint2d   ptStart = pLine->getStartPoint();
  OdGePoint2d   ptEnd   = pLine->getEndPoint();
  OdGeVector2d  vrDir   = ptEnd - ptStart;

  double dLength = vrDir.length();

  if( vrDir.isZeroLength() )
  {
    return eDegenerateGeometry;
  }

  vrDir.normalize();

  double prevParam = 0;

  for( OdUInt32 i = 0; i < arrPoints.size(); i++ )
  {
    double param;

    if( pLine->getParamAtPoint(arrPoints[i],param) != eOk )
    {
      return eInvalidInput;
    }

    if( prevParam != param )
    {
      OdDgLine2dPtr newLine = OdDgLine2d::createObject();
      newLine->setStartPoint( ptStart + vrDir*prevParam );
      newLine->setEndPoint( ptStart + vrDir*param );
      curveSegments.append( newLine.get() );
    }
    prevParam = param;
  }

  if( prevParam != dLength )
  {
    OdDgLine2dPtr newLine = OdDgLine2d::createObject();
    newLine->setStartPoint( ptStart + vrDir*prevParam );
    newLine->setEndPoint( ptStart + vrDir*dLength );
    curveSegments.append( newLine.get() );
  }

  return eOk;
}

OdResult OdDgLineElement2dCurvePE::extend( OdDgCurveElement2d* pCurve, double param )
{
  OdDgLine2dPtr pLine   = pCurve;
  OdGePoint2d   ptStart = pLine->getStartPoint();
  OdGePoint2d   ptEnd   = pLine->getEndPoint();
  OdGeVector2d  vrDir   = ptEnd - ptStart;

  double dLength = vrDir.length();

  if( vrDir.isZeroLength() )
  {
    return eDegenerateGeometry;
  }

  if( param < 0 )
    pLine->setStartPoint( ptStart + vrDir.normal() * param );
  else if ( param > vrDir.length() )
    pLine->setEndPoint( ptStart + vrDir.normal() * param );
  else 
    return eInvalidInput;

  return eOk;
}

OdResult OdDgLineElement2dCurvePE::extend( OdDgCurveElement2d* pCurve, 
                                          bool extendStart,
                                          const OdGePoint2d& toPoint )
{
  OdDgLine2dPtr pLine   = pCurve;
  OdGePoint2d   ptStart = pLine->getStartPoint();
  OdGePoint2d   ptEnd   = pLine->getEndPoint();

  if( ptStart.isEqualTo(ptEnd) )
  {
    return eDegenerateGeometry;
  }

  OdGeLine2d curLine(ptStart,ptEnd);

  if( !curLine.isOn(toPoint) )
  {
    return eInvalidInput;
  }

  double param = curLine.paramOf(toPoint);

  if( extendStart )
  {
    if( param > 0.0 )
    {
      return eInvalidInput;
    }

    pLine->setStartPoint( toPoint );
  }
  else
  {
    if( param < 1.0 )
    {
      return eInvalidInput;
    }

    pLine->setEndPoint( toPoint );
  }
  return eOk;
}

OdResult OdDgLineElement2dCurvePE::getSpline (const OdDgCurveElement2d* pCurve, 
                                              OdDgBSplineCurve2dPtr& spline) const
{
  OdDgLine2dPtr pLine   = pCurve;
  OdGePoint2d   ptStart = pLine->getStartPoint();
  OdGePoint2d   ptEnd   = pLine->getEndPoint();

  OdGePoint3d   ptStart3d( ptStart.x, ptStart.y, 0.0);
  OdGePoint3d   ptEnd3d( ptEnd.x, ptEnd.y, 0.0);

  OdGeNurbCurve3d curCurve(OdGeLineSeg3d(ptStart3d,ptEnd3d));
  OdGePoint3dArray ctl;
  OdGeDoubleArray ww;
  OdGeKnotVector knots;
  int degree;
  bool rational, periodic;
  curCurve.getDefinitionData( degree, rational, periodic, knots, ctl, ww );

  OdGePoint2dArray ctl2d;
  for( OdUInt32 i = 0; i < ctl.size(); i++ )
  {
    ctl2d.push_back( ctl[i].convert2d() );
  }

  double dStartKnot = knots[0];
  double dEndKnot   = knots[knots.length()-1];
  double dKnotInterval = dEndKnot - dStartKnot;

  for( OdInt32 j = 0; j < knots.length(); j++ )
  {
    knots[j] = (knots[j]-dStartKnot)/dKnotInterval;
  }

  spline = OdDgBSplineCurve2d::createObject();
  spline->setNurbsData( degree + 1, rational, false, ctl2d, knots, ww );

  return eOk;
}

OdResult OdDgLineElement2dCurvePE::getClosestPointTo(const OdDgCurveElement2d* pCurve, 
                                                     const OdGePoint2d& givenPoint,
                                                     OdGePoint2d& pointOnCurve, 
                                                     bool extend ) const
{
  OdDgLine2dPtr pLine   = pCurve;
  OdGePoint2d   ptStart = pLine->getStartPoint();
  OdGePoint2d   ptEnd   = pLine->getEndPoint();

  if( ptStart.isEqualTo(ptEnd) )
  {
    pointOnCurve = ptStart;
  }
  else if ( extend )
  {
    OdGePoint3d ptStart3d( ptStart.x, ptStart.y, 0.0 );
    OdGePoint3d ptEnd3d( ptEnd.x, ptEnd.y, 0.0 );
    OdGePoint3d ptGiven3d( givenPoint.x, givenPoint.y, 0.0 );

    pointOnCurve = OdGeLine3d(ptStart3d,ptEnd3d).closestPointTo(ptGiven3d).convert2d();
  }
  else
  {
    OdGePoint3d ptStart3d( ptStart.x, ptStart.y, 0.0 );
    OdGePoint3d ptEnd3d( ptEnd.x, ptEnd.y, 0.0 );
    OdGePoint3d ptGiven3d( givenPoint.x, givenPoint.y, 0.0 );

    pointOnCurve = OdGeLineSeg3d(ptStart3d,ptEnd3d).closestPointTo(ptGiven3d).convert2d();
  }

  return eOk;
}

//----------------------------------------------------------
//
// OdDgLineElement3dCurvePE
//
//----------------------------------------------------------

OdResult OdDgLineElement3dCurvePE::getOrthoProjectedCurve(const OdDgCurveElement3d* pCurve, 
                                                          const OdGePlane& projPlane, 
                                                          OdDgCurveElement3dPtr* pProjCurve) const
{
  OdDgLine3dPtr pLine = pCurve;
  OdGePoint3d pp1,pp2;
  projPlane.project( pLine->getStartPoint(), pp1 );
  projPlane.project( pLine->getEndPoint(), pp2 );
  OdDgLine3dPtr projLine = OdDgLine3d::createObject();
  projLine->setStartPoint(pp1);
  projLine->setEndPoint(pp2);
  *pProjCurve = projLine;
  return eOk;
}

OdResult OdDgLineElement3dCurvePE::getProjectedCurve(const OdDgCurveElement3d* pCurve, 
                                                     const OdGePlane& projPlane,
                                                     const OdGeVector3d& projDirection, 
                                                     OdDgCurveElement3dPtr* pProjCurve) const
{
  OdDgLine3dPtr pLine = pCurve;
  // If projDir is perpendicular to the projPlane’s normal then eInvalidInput is returned
  if( projDirection.isPerpendicularTo(projPlane.normal()) )
  {
    return eInvalidInput;
  }

  OdGePoint3d pp1 = pLine->getStartPoint().project(projPlane, projDirection);
  OdGePoint3d pp2 = pLine->getEndPoint().project(projPlane, projDirection);
  OdDgLine3dPtr projLine = OdDgLine3d::createObject();
  projLine->setStartPoint(pp1);
  projLine->setEndPoint(pp2);
  *pProjCurve = projLine;
  return eOk;
}

OdResult OdDgLineElement3dCurvePE::getOffsetCurves(const OdDgCurveElement3d* pCurve, 
                                                   double offsetDistance,  
                                                   OdRxObjectPtrArray& offsetCurves) const
{
  OdDgLine3dPtr pLine = pCurve;
  OdGePoint3d p1 = pLine->getStartPoint();
  OdGePoint3d p2 = pLine->getEndPoint();
  OdGeVector3d vrDir = p2 - p1;

  if( vrDir.isZeroLength() )
  {
    return eDegenerateGeometry;
  }

  vrDir.normalize();

  OdGeVector3d vrNormal = OdGeVector3d::kZAxis;

  if( vrNormal.isParallelTo( vrDir ) )
  {
    vrDir = OdGeVector3d::kYAxis;
  }

  OdGeVector3d vrOffsetDir = -vrDir.crossProduct( vrNormal );
  vrOffsetDir.normalize();

  OdDgLine3dPtr pOffsetLine = OdDgLine3d::createObject();
  pOffsetLine->setStartPoint( p1 + vrOffsetDir * offsetDistance );
  pOffsetLine->setEndPoint( p2 + vrOffsetDir * offsetDistance );
  offsetCurves.append(pOffsetLine.get());
  return eOk;
}

OdResult OdDgLineElement3dCurvePE::getOffsetCurvesGivenPlaneNormal(const OdDgCurveElement3d* pCurve, 
                                                                   const OdGeVector3d& normal, 
                                                                   double offsetDistance,
                                                                   OdRxObjectPtrArray& offsetCurves) const
{
  OdDgLine3dPtr pLine = pCurve;
  OdGePoint3d p1 = pLine->getStartPoint();
  OdGePoint3d p2 = pLine->getEndPoint();

  OdGeVector3d vrDir = p2 - p1;

  if( vrDir.isZeroLength() )
  {
    return eDegenerateGeometry;
  }

  if( normal.isZeroLength() || !normal.isPerpendicularTo(p2-p1) )
  {
    return eInvalidInput;
  }

  OdGeVector3d vrLineDir = p2 - p1;

  if( vrLineDir.isZeroLength() )
  {
    vrLineDir = OdGeVector3d::kXAxis;
  }
  else
  {
    vrLineDir.normalize();
  }

  vrDir.normalize();

  if( normal.isParallelTo( vrDir ) )
  {
    vrDir = OdGeVector3d::kYAxis;
  }

  OdGeVector3d vrOffsetDir = -vrDir.crossProduct( normal.normal() );
  vrOffsetDir.normalize();

  OdDgLine3dPtr pOffsetLine = OdDgLine3d::createObject();
  pOffsetLine->setStartPoint( p1 + vrOffsetDir*offsetDistance );
  pOffsetLine->setEndPoint( p2 + vrOffsetDir*offsetDistance );
  offsetCurves.append(pOffsetLine.get());
  return eOk;
}

OdResult OdDgLineElement3dCurvePE::getSplitCurves (const OdDgCurveElement3d* pCurve, 
                                                   const OdGeDoubleArray& params, 
                                                   OdRxObjectPtrArray& curveSegments) const
{
  OdDgLine3dPtr pLine = pCurve;
  OdGePoint3d p1 = pLine->getStartPoint();
  OdGeVector3d v = pLine->getEndPoint() - p1;

  if( v.isZeroLength() ) 
    return eDegenerateGeometry;

  double len = v.length();
  v.normalize();
  double prevParam = 0;
  for ( unsigned i = 0; i < params.size(); i++ )
  {
    if ( params[i] > len || params[i] < 0 ) return eInvalidInput;
    if ( prevParam != params[i] )
    {
      OdDgLine3dPtr pSegmentLine = OdDgLine3d::createObject();
      pSegmentLine->setStartPoint( p1 + v*prevParam );
      pSegmentLine->setEndPoint( p1 + v*params[i] );
      curveSegments.append( pSegmentLine.get() );
    }
    prevParam = params[i];
  }

  if( prevParam != len )
  {
    OdDgLine3dPtr pSegmentLine = OdDgLine3d::createObject();
    pSegmentLine->setStartPoint( p1 + v*prevParam );
    pSegmentLine->setEndPoint( p1 + v*len );
    curveSegments.append( pSegmentLine.get() );
  }

  return eOk;
}

OdResult OdDgLineElement3dCurvePE::getSplitCurves (const OdDgCurveElement3d* pCurve, 
                                                   const OdGePoint3dArray& points, 
                                                   OdRxObjectPtrArray& curveSegments) const
{
  OdDgLine3dPtr pLine = pCurve;
  OdGePoint3d p1 = pLine->getStartPoint();
  OdGePoint3d p2 = pLine->getEndPoint();
  OdGeVector3d v = p2 - p1;

  if( v.isZeroLength() ) 
    return eDegenerateGeometry;

  double len = v.length();

  v.normalize();

  double prevParam = 0;

  for( unsigned i = 0; i < points.size(); i++ )
  {
    double param; 

    if( pLine->getParamAtPoint(points[i],param) != eOk ) 
      return eInvalidInput;

    if( prevParam != param )
    {
      OdDgLine3dPtr pSegmentLine = OdDgLine3d::createObject();
      pSegmentLine->setStartPoint( p1 + v*prevParam );
      pSegmentLine->setEndPoint( p1 + v*param );
      curveSegments.append( pSegmentLine.get() );
    }
    prevParam = param;
  }

  if( prevParam != len )
  {
    OdDgLine3dPtr pSegmentLine = OdDgLine3d::createObject();
    pSegmentLine->setStartPoint( p1 + v*prevParam );
    pSegmentLine->setEndPoint( p1 + v*len );
    curveSegments.append( pSegmentLine.get() );
  }

  return eOk;
}

OdResult OdDgLineElement3dCurvePE::extend(OdDgCurveElement3d* pCurve, double param)
{
  OdDgLine3dPtr pLine = pCurve;
  OdGePoint3d p1 = pLine->getStartPoint();
  OdGePoint3d p2 = pLine->getEndPoint();
  OdGeVector3d v = p2 - p1;

  if( v.isZeroLength() ) 
    return eDegenerateGeometry;

  if( param < 0 )
    pLine->setStartPoint( p1 + v.normal() * param );
  else if ( param > v.length() )
    pLine->setEndPoint( p1 + v.normal() * param );
  else 
    return eInvalidInput;

  return eOk;
}

OdResult OdDgLineElement3dCurvePE::extend(OdDgCurveElement3d* pCurve, 
                                          bool extendStart,
                                          const OdGePoint3d& toPoint )
{
  OdDgLine3dPtr pLine = pCurve;
  OdGePoint3d p1 = pLine->getStartPoint();
  OdGePoint3d p2 = pLine->getEndPoint();

  if( p1.isEqualTo(p2) )
    return eDegenerateGeometry;

  OdGeLine3d l(p1,p2);

  if( !l.isOn(toPoint) ) 
    return eInvalidInput;

  double param = l.paramOf(toPoint);

  if( extendStart )
  {
    if( param > 0 ) 
      return eInvalidInput;

    pLine->setStartPoint( toPoint );
  }
  else
  {
    if( param < 1 ) 
      return eInvalidInput;

    pLine->setEndPoint( toPoint );
  }
  return eOk;
}

OdResult OdDgLineElement3dCurvePE::getSpline(const OdDgCurveElement3d* pCurve, 
                                             OdDgBSplineCurve3dPtr& spline) const
{
  OdDgLine3dPtr pLine   = pCurve;
  OdGePoint3d   ptStart = pLine->getStartPoint();
  OdGePoint3d   ptEnd   = pLine->getEndPoint();

  OdGeNurbCurve3d curCurve(OdGeLineSeg3d(ptStart,ptEnd));

  OdGePoint3dArray ctl;
  OdGeDoubleArray ww;
  OdGeKnotVector knots;
  int degree;
  bool rational, periodic;
  curCurve.getDefinitionData( degree, rational, periodic, knots, ctl, ww );

  double dStartKnot = knots[0];
  double dEndKnot   = knots[knots.length()-1];
  double dKnotInterval = dEndKnot - dStartKnot;

  for( OdInt32 j = 0; j < knots.length(); j++ )
  {
    knots[j] = (knots[j]-dStartKnot) / dKnotInterval;
  }

  spline = OdDgBSplineCurve3d::createObject();
  spline->setNurbsData( degree + 1, rational, false, ctl, knots, ww );

  return eOk;
}

OdResult OdDgLineElement3dCurvePE::getClosestPointTo(const OdDgCurveElement3d* pCurve, 
                                                     const OdGePoint3d& givenPoint,
                                                     OdGePoint3d& pointOnCurve, 
                                                     bool extend ) const
{
  OdDgLine3dPtr pLine   = pCurve;
  OdGePoint3d   ptStart = pLine->getStartPoint();
  OdGePoint3d   ptEnd   = pLine->getEndPoint();

  if( ptStart.isEqualTo(ptEnd) )
  {
    pointOnCurve = ptStart;
  }
  else if ( extend )
    pointOnCurve = OdGeLine3d(ptStart,ptEnd).closestPointTo(givenPoint);
  else
    pointOnCurve = OdGeLineSeg3d(ptStart,ptEnd).closestPointTo(givenPoint);

  return eOk;
}

OdResult OdDgLineElement3dCurvePE::getClosestPointTo(const OdDgCurveElement3d* pCurve, 
                                                     const OdGePoint3d& givenPoint,
                                                     const OdGeVector3d& normal,
                                                     OdGePoint3d& pointOnCurve, 
                                                     bool extend ) const
{
  OdDgCurveElement3dPtr pProjCurve;
  getOrthoProjectedCurve( pCurve, OdGePlane(givenPoint,normal), &pProjCurve );
  getClosestPointTo( pProjCurve, givenPoint, pointOnCurve, extend );

  OdDgLine3dPtr pLine = pCurve;

  OdGePoint3d p1 = pLine->getStartPoint();
  OdGePoint3d p2 = pLine->getEndPoint();

  if( !OdGeLine3d(p1,p2).intersectWith( OdGeLine3d(pointOnCurve,normal),pointOnCurve) )
  { // normal parallel to line

    if( !extend ) // if extend => already set
    {
      pointOnCurve = OdGeLineSeg3d(p1,p2).closestPointTo(pointOnCurve);
    }
  }
  return eOk;
}

