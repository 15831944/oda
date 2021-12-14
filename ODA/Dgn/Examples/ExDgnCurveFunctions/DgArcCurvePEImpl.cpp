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
#include "DgArcCurvePEImpl.h"
#include "DgArc.h"
#include "RxObjectImpl.h"
#include "Ge/GeEllipArc2d.h"
#include "Ge/GeCircArc2d.h"
#include "Ge/GeEllipArc3d.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeLine3d.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeNurbCurve3d.h"
#include "DgBSplineCurve.h"
#include "DgLine.h"

#define  STL_USING_MEMORY
#include "OdaSTL.h"

//----------------------------------------------------------
//
// OdDgArcElement2dCurvePE
//
//----------------------------------------------------------

OdResult OdDgArcElement2dCurvePE::getOffsetCurves(const OdDgCurveElement2d* pCurve, 
                                                  double offsetDist,  
                                                  OdRxObjectPtrArray& offsetCurves) const
{
  OdDgArc2d* pArc = (OdDgArc2d*)(pCurve);

  if( (pArc->getPrimaryAxis() + offsetDist <= 0) || 
    (pArc->getSecondaryAxis() + offsetDist <= 0)
    )
  {
    return eInvalidInput;
  }

  OdDgArc2dPtr pNewArc = pArc->clone();
  pNewArc->setPrimaryAxis( pNewArc->getPrimaryAxis() + offsetDist );
  pNewArc->setSecondaryAxis( pNewArc->getSecondaryAxis() + offsetDist );
  offsetCurves.append( pNewArc.get() );
  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement2dCurvePE::getSplitCurves(const OdDgCurveElement2d* pCurve, 
                                                 const OdGeDoubleArray& params, 
                                                 OdRxObjectPtrArray& curveSegments) const
{
  OdDgArc2d* pArc = (OdDgArc2d*)(pCurve);

  double prevParam = pArc->getStartAngle();
  for( unsigned i = 0; i < params.size(); i++ )
  {
    if( pArc->getSweepAngle() < 0 )
    {
      if( params[i] > pArc->getStartAngle() || params[i] < pArc->getStartAngle() + pArc->getSweepAngle() ) 
        return eInvalidInput;
    }
    else
    {
      if( params[i] < pArc->getStartAngle() || params[i] > pArc->getStartAngle() + pArc->getSweepAngle() ) 
        return eInvalidInput;
    }

    if ( prevParam != params[i] )
    {
      OdDgArc2dPtr pNewArc = pArc->clone();
      pNewArc->setStartAngle( prevParam );
      pNewArc->setSweepAngle( params[i] - prevParam );
      curveSegments.append( pNewArc.get() );
    }

    prevParam = params[i];
  }

  if( prevParam != pArc->getStartAngle() + pArc->getSweepAngle() )
  {
    OdDgArc2dPtr pNewArc = pArc->clone();
    pNewArc->setStartAngle( prevParam );
    pNewArc->setSweepAngle( pArc->getStartAngle() + pArc->getSweepAngle() - prevParam );
    curveSegments.append( pNewArc.get() );
  }
  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement2dCurvePE::getSplitCurves( const OdDgCurveElement2d* pCurve, 
                                                 const OdGePoint2dArray& arrPoints, 
                                                 OdRxObjectPtrArray& curveSegments ) const
{
  OdDgArc2d* pArc = (OdDgArc2d*)(pCurve);

  OdGeDoubleArray arrParams;
  for ( unsigned i = 0; i < arrPoints.size(); i++ )
  {
    double param;

    if ( eOk != pArc->getParamAtPoint( arrPoints[i], param ) )
      return eInvalidInput;

    arrParams.push_back( param );
  }

  return getSplitCurves(pArc, arrParams, curveSegments);
}

//========================================================================================================

OdResult OdDgArcElement2dCurvePE::extend(OdDgCurveElement2d* pCurve, 
                                         double param)
{
  OdDgArc2d* pArc = (OdDgArc2d*)(pCurve);

  if( pArc->getSweepAngle() > 0 )
  {
    if( param < pArc->getStartAngle() )
    {
      pArc->setStartAngle( param );
    }
    else if( param > pArc->getStartAngle() + pArc->getSweepAngle() )
    {
      pArc->setSweepAngle( param - pArc->getStartAngle() );
    }
    else 
    {
      return eInvalidInput;
    }
  }
  else
  {
    if( param > pArc->getStartAngle() )
    {
      double dStartAngle = pArc->getStartAngle();
      pArc->setStartAngle( param );
      pArc->setSweepAngle(  pArc->getSweepAngle() - param + dStartAngle );
    }
    else if( param < pArc->getStartAngle() + pArc->getSweepAngle() )
    {
      pArc->setSweepAngle( param - pArc->getStartAngle() );
    }
    else 
    {
      return eInvalidInput;
    }
  }

  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement2dCurvePE::extend( OdDgCurveElement2d* pCurve, 
                                         bool extendStart,
                                         const OdGePoint2d& toPoint )
{
  OdDgArc2d* pArc = (OdDgArc2d*)(pCurve);

  OdGeVector2d v = toPoint - pArc->getOrigin();

  if( extendStart )
  {
    OdGeVector2d vrXAxis = OdGeVector2d::kXAxis;
    vrXAxis = vrXAxis.rotateBy( pArc->getRotationAngle() );
    double dNewStartAngle = vrXAxis.angleToCCW(v);
    double dEndAngle = pArc->getStartAngle() + pArc->getSweepAngle();

    pArc->setStartAngle( dNewStartAngle );
    pArc->setSweepAngle( dEndAngle - dNewStartAngle );
  }
  else
  {
    OdGeVector2d vrXAxis = OdGeVector2d::kXAxis;
    vrXAxis = vrXAxis.rotateBy( pArc->getRotationAngle() );
    double dNewEndAngle = vrXAxis.angleToCCW(v);

    if( (pArc->getSweepAngle() < 0) && ( dNewEndAngle > pArc->getStartAngle() ) )
    {
      dNewEndAngle -= Oda2PI;
    }
    else if( (pArc->getSweepAngle() > 0) && ( dNewEndAngle < pArc->getStartAngle() ) )
    {
      dNewEndAngle += Oda2PI;
    }

    pArc->setSweepAngle( dNewEndAngle - pArc->getStartAngle() );
  }

  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement2dCurvePE::getSpline (const OdDgCurveElement2d* pCurve, 
                                             OdDgBSplineCurve2dPtr& spline) const
{
  OdDgArc2d* pArc = (OdDgArc2d*)(pCurve);
  OdGeEllipArc2d geArc = pArc->getEllipArc();

  OdGeNurbCurve2d curCurve(geArc);

  OdGePoint2dArray ctl2d;
  OdGeDoubleArray ww;
  OdGeKnotVector knots;
  int degree;
  bool rational, periodic;
  curCurve.getDefinitionData( degree, rational, periodic, knots, ctl2d, ww );

  double dStartKnot = knots[0];
  double dEndKnot   = knots[knots.length()-1];
  double dKnotInterval = dEndKnot - dStartKnot;

  for( OdInt32 j = 0; j < knots.length(); j++ )
  {
    knots[j] = (knots[j] - dStartKnot) / dKnotInterval;
  }

  spline = OdDgBSplineCurve2d::createObject();
  spline->setNurbsData( degree + 1, rational, false, ctl2d, knots, ww );

  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement2dCurvePE::getClosestPointTo(const OdDgCurveElement2d* pCurve, 
                                                    const OdGePoint2d& givenPoint,
                                                    OdGePoint2d& pointOnCurve, 
                                                    bool extend ) const
{
  OdDgArc2d* pArc = (OdDgArc2d*)(pCurve);

  OdGeEllipArc2d geArc = pArc->getEllipArc();

  double param;
  const OdGeInterval ivFull(0.0, Oda2PI);
  pointOnCurve = geArc.closestPointTo( givenPoint, param, extend ? &ivFull : NULL );

  return eOk;
}

//----------------------------------------------------------
//
// OdDgArcElement3dCurvePE
//
//----------------------------------------------------------

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::getOrthoProjectedCurve( const OdDgCurveElement3d* pCurve, 
                                                         const OdGePlane& projPlane, 
                                                         OdDgCurveElement3dPtr* pProjCurve) const
{
  return getProjectedCurve( pCurve, projPlane, projPlane.normal(), pProjCurve );
}

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::getProjectedCurve(const OdDgCurveElement3d* pCurve, 
                                                    const OdGePlane& projPlane,
                                                    const OdGeVector3d& projDirection, 
                                                    OdDgCurveElement3dPtr* pProjCurve) const
{
  if( projDirection.isZeroLength() || projDirection.isPerpendicularTo(projPlane.normal()) )
    return eInvalidInput;

  OdDgArc3dPtr pArc = pCurve;
  OdGeEllipArc3d geArc = pArc->getEllipArc();

  if( projDirection.isPerpendicularTo(geArc.normal()) )
  {
    std::auto_ptr<OdGeLineSeg3d> seg((OdGeLineSeg3d*)geArc.orthoProject( projPlane ));
    OdDgLine3dPtr line = OdDgLine3d::createObject();
    line->setStartPoint( seg->startPoint() );
    line->setEndPoint( seg->endPoint() );
    *pProjCurve = line;
  }
  else
  {
    try
    {
      OdDgArc3dPtr pArcCopy = pArc->clone();
      pArcCopy->transformBy( OdGeMatrix3d::projection( projPlane, projDirection ) );
      *pProjCurve = pArcCopy;
    }
    catch(OdError* e)
    {
      return e->code();
    }
  }
  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::getOffsetCurves(const OdDgCurveElement3d* pCurve, 
                                                  double offsetDistance,  
                                                  OdRxObjectPtrArray& offsetCurves) const
{
  OdDgArc3d* pArc = (OdDgArc3d*)(pCurve);

  if( (pArc->getPrimaryAxis() + offsetDistance <= 0) || 
    (pArc->getSecondaryAxis() + offsetDistance <= 0)
    )
  {
    return eInvalidInput;
  }

  OdDgArc3dPtr pNewArc = pArc->clone();
  pNewArc->setPrimaryAxis( pNewArc->getPrimaryAxis() + offsetDistance );
  pNewArc->setSecondaryAxis( pNewArc->getSecondaryAxis() + offsetDistance );
  offsetCurves.append( pNewArc.get() );
  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::getOffsetCurvesGivenPlaneNormal(const OdDgCurveElement3d* pCurve, 
                                                                  const OdGeVector3d& normal, 
                                                                  double offsetDistance,
                                                                  OdRxObjectPtrArray& offsetCurves) const
{
  if( normal.isZeroLength() ) 
    return eInvalidInput;

  OdDgArc3dPtr pArc = pCurve;
  OdGeEllipArc3d geArc = pArc->getEllipArc();

  if( !geArc.normal().isParallelTo( normal ) ) 
    return eInvalidInput;

  return getOffsetCurves( pCurve, offsetDistance, offsetCurves );
}

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::getSplitCurves(const OdDgCurveElement3d* pCurve, 
                                                 const OdGeDoubleArray& params, 
                                                 OdRxObjectPtrArray& curveSegments) const
{
  OdDgArc3d* pArc = (OdDgArc3d*)(pCurve);

  double prevParam = pArc->getStartAngle();

  for( unsigned i = 0; i < params.size(); i++ )
  {
    if( pArc->getSweepAngle() < 0 )
    {
      if( params[i] > pArc->getStartAngle() || params[i] < pArc->getStartAngle() + pArc->getSweepAngle() ) 
        return eInvalidInput;
    }
    else
    {
      if( params[i] < pArc->getStartAngle() || params[i] > pArc->getStartAngle() + pArc->getSweepAngle() ) 
        return eInvalidInput;
    }

    if ( prevParam != params[i] )
    {
      OdDgArc3dPtr pNewArc = pArc->clone();
      pNewArc->setStartAngle( prevParam );
      pNewArc->setSweepAngle( params[i] - prevParam );
      curveSegments.append( pNewArc.get() );
    }

    prevParam = params[i];
  }

  if( prevParam != pArc->getStartAngle() + pArc->getSweepAngle() )
  {
    OdDgArc3dPtr pNewArc = pArc->clone();
    pNewArc->setStartAngle( prevParam );
    pNewArc->setSweepAngle( pArc->getStartAngle() + pArc->getSweepAngle() - prevParam );
    curveSegments.append( pNewArc.get() );
  }
  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::getSplitCurves(const OdDgCurveElement3d* pCurve, 
                                                 const OdGePoint3dArray& points, 
                                                 OdRxObjectPtrArray& curveSegments) const
{
  OdDgArc3d* pArc = (OdDgArc3d*)(pCurve);

  OdGeDoubleArray arrParams;
  for ( unsigned i = 0; i < points.size(); i++ )
  {
    double param;

    if ( eOk != pArc->getParamAtPoint( points[i], param ) )
      return eInvalidInput;

    arrParams.push_back( param );
  }

  return getSplitCurves(pArc, arrParams, curveSegments);
}

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::extend(OdDgCurveElement3d* pCurve, double param)
{
  OdDgArc3d* pArc = (OdDgArc3d*)(pCurve);

  if( pArc->getSweepAngle() > 0 )
  {
    if( param < pArc->getStartAngle() )
    {
      pArc->setStartAngle( param );
    }
    else if( param > pArc->getStartAngle() + pArc->getSweepAngle() )
    {
      pArc->setSweepAngle( param - pArc->getStartAngle() );
    }
    else 
    {
      return eInvalidInput;
    }
  }
  else
  {
    if( param > pArc->getStartAngle() )
    {
      double dStartAngle = pArc->getStartAngle();
      pArc->setStartAngle( param );
      pArc->setSweepAngle(  pArc->getSweepAngle() - param + dStartAngle );
    }
    else if( param < pArc->getStartAngle() + pArc->getSweepAngle() )
    {
      pArc->setSweepAngle( param - pArc->getStartAngle() );
    }
    else 
    {
      return eInvalidInput;
    }
  }

  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::extend(OdDgCurveElement3d* pCurve, 
                                         bool extendStart,
                                         const OdGePoint3d& toPoint)
{
  OdDgArc3d* pArc = (OdDgArc3d*)(pCurve);
  OdGeVector3d vrRef    = OdGeVector3d::kXAxis;
  OdGeVector3d vrNormal = OdGeVector3d::kZAxis;
  OdGeMatrix3d matRotation = pArc->getRotation().getMatrix();
  vrRef    = vrRef.transformBy( matRotation );
  vrNormal = vrNormal.transformBy( matRotation );
  vrRef.normalize();

  OdGePoint3d ptExtend = toPoint;

  OdGePlane arcPlane( pArc->getOrigin(), vrNormal );
  ptExtend = ptExtend.project( arcPlane, vrNormal );

  OdGeVector3d v = ptExtend - pArc->getOrigin();

  if( v.isZeroLength() )
  {
    return eInvalidInput;
  }

  v.normalize();

  if( extendStart )
  {
    double dNewStartAngle = vrRef.angleTo( v, vrNormal);
    double dEndAngle = pArc->getStartAngle() + pArc->getSweepAngle();

    pArc->setStartAngle( dNewStartAngle );
    pArc->setSweepAngle( dEndAngle - dNewStartAngle );
  }
  else
  {
    double dNewEndAngle = vrRef.angleTo( v, vrNormal);

    if( (pArc->getSweepAngle() < 0) && ( dNewEndAngle > pArc->getStartAngle() ) )
    {
      dNewEndAngle -= Oda2PI;
    }
    else if( (pArc->getSweepAngle() > 0) && ( dNewEndAngle < pArc->getStartAngle() ) )
    {
      dNewEndAngle += Oda2PI;
    }

    pArc->setSweepAngle( dNewEndAngle - pArc->getStartAngle() );
  }

  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::getSpline(const OdDgCurveElement3d* pCurve, 
                                            OdDgBSplineCurve3dPtr& spline) const
{
  OdDgArc3d* pArc = (OdDgArc3d*)(pCurve);
  OdGeEllipArc3d geArc = pArc->getEllipArc();

  OdGeNurbCurve3d curCurve(geArc);

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

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::getClosestPointTo(const OdDgCurveElement3d* pCurve, 
                                                    const OdGePoint3d& givenPoint,
                                                    OdGePoint3d& pointOnCurve, 
                                                    bool extend ) const
{
  OdDgArc3d* pArc = (OdDgArc3d*)(pCurve);
  OdGeEllipArc3d geArc = pArc->getEllipArc();

  if( extend )
  {
    geArc.setAngles(0, Oda2PI);
  }

  pointOnCurve = geArc.closestPointTo( givenPoint );

  return eOk;
}

//========================================================================================================

OdResult OdDgArcElement3dCurvePE::getClosestPointTo(const OdDgCurveElement3d* pCurve, 
                                                    const OdGePoint3d& givenPoint,
                                                    const OdGeVector3d& direction,
                                                    OdGePoint3d& pointOnCurve, 
                                                    bool extend ) const
{
  if( direction.isZeroLength() ) 
    return eInvalidInput;

  OdDgArc3d* pArc = (OdDgArc3d*)(pCurve);
  OdGeEllipArc3d geArc = pArc->getEllipArc();

  if( extend )
  {
    geArc.setAngles(0, Oda2PI);
  }

  OdGeVector3d vrNormal = OdGeVector3d::kZAxis;
  OdGeMatrix3d matRotation = pArc->getRotation().getMatrix();
  vrNormal = vrNormal.transformBy( matRotation );
  vrNormal.normalize();

  if( direction.isPerpendicularTo(vrNormal) )
  {
    return getClosestPointTo( pCurve, givenPoint, pointOnCurve, extend );
  }
  else
  {
    OdDgCurveElement3dPtr pProjCurve;
    OdResult res = getOrthoProjectedCurve( pCurve, OdGePlane(givenPoint,direction), &pProjCurve );
    if (res != eOk)
      return res;
    if(pProjCurve->isKindOf(OdDgArc3d::desc()))
      res = getClosestPointTo( pProjCurve, givenPoint, pointOnCurve, extend );
    else
      res = pProjCurve->getClosestPointTo(givenPoint, direction, pointOnCurve, extend );

    if (res != eOk)
      return res;

    OdGePlane plane(pArc->getOrigin(), vrNormal);
    pointOnCurve = pointOnCurve.project(plane, direction);
  }
  return eOk;
}

//========================================================================================================
