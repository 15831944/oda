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
#include "DgEllipseCurvePEImpl.h"
#include "DgEllipse.h"
#include "RxObjectImpl.h"
#include "DgArc.h"
#include "DgBSplineCurve.h"
#include "Ge/GePlane.h"
#include "Ge/GeLine3d.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeNurbCurve2d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeKnotVector.h"
#include "DgLine.h"

#define  STL_USING_MEMORY
#include "OdaSTL.h"

//----------------------------------------------------------
//
// OdDgEllipseElement2dCurvePE
//
//----------------------------------------------------------

OdResult OdDgEllipseElement2dCurvePE::getOffsetCurves(const OdDgCurveElement2d* pCurve, 
                                                      double offsetDist,  
                                                      OdRxObjectPtrArray& offsetCurves) const
{
  OdDgEllipse2d* pArc = (OdDgEllipse2d*)(pCurve);

  if( (pArc->getPrimaryAxis() + offsetDist <= 0) || 
    (pArc->getSecondaryAxis() + offsetDist <= 0)
    )
  {
    return eInvalidInput;
  }

  OdDgEllipse2dPtr pNewArc = pArc->clone();
  pNewArc->setPrimaryAxis( pNewArc->getPrimaryAxis() + offsetDist );
  pNewArc->setSecondaryAxis( pNewArc->getSecondaryAxis() + offsetDist );
  offsetCurves.append( pNewArc.get() );
  return eOk;
}

//========================================================================================================

OdResult OdDgEllipseElement2dCurvePE::getSplitCurves(const OdDgCurveElement2d* pCurve, 
                                                     const OdGeDoubleArray& params, 
                                                     OdRxObjectPtrArray& curveSegments) const
{
  OdDgEllipse2d* pEllipse = (OdDgEllipse2d*)(pCurve);

  double prevParam = 0;
  for( unsigned i = 0; i < params.size(); i++ )
  {
    if( params[i] < 0 || params[i] > Oda2PI ) 
      return eInvalidInput;

    if( prevParam != params[i] )
    {
      OdDgArc2dPtr pNewArc = OdDgArc2d::createObject();
      pNewArc->setPrimaryAxis( pEllipse->getPrimaryAxis() );
      pNewArc->setSecondaryAxis( pEllipse->getSecondaryAxis() );
      pNewArc->setOrigin( pEllipse->getOrigin() );
      pNewArc->setRotationAngle( pEllipse->getRotationAngle() );
      pNewArc->setPropertiesFrom( pEllipse );
      pNewArc->setStartAngle( prevParam );
      pNewArc->setSweepAngle( params[i] - prevParam );
      curveSegments.append( pNewArc.get() );
    }

    prevParam = params[i];
  }

  if( prevParam != Oda2PI )
  {
    OdDgArc2dPtr pNewArc = OdDgArc2d::createObject();
    pNewArc->setPrimaryAxis( pEllipse->getPrimaryAxis() );
    pNewArc->setSecondaryAxis( pEllipse->getSecondaryAxis() );
    pNewArc->setOrigin( pEllipse->getOrigin() );
    pNewArc->setRotationAngle( pEllipse->getRotationAngle() );
    pNewArc->setPropertiesFrom( pEllipse );
    pNewArc->setStartAngle( prevParam );
    pNewArc->setSweepAngle( Oda2PI - prevParam );
    curveSegments.append( pNewArc.get() );
  }
  return eOk;
}

//========================================================================================================

OdResult OdDgEllipseElement2dCurvePE::getSplitCurves( const OdDgCurveElement2d* pCurve, 
                                                     const OdGePoint2dArray& arrPoints, 
                                                     OdRxObjectPtrArray& curveSegments ) const
{
  OdDgEllipse2d* pArc = (OdDgEllipse2d*)(pCurve);

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

OdResult OdDgEllipseElement2dCurvePE::extend(OdDgCurveElement2d* pCurve, 
                                             double param)
{
  return eInvalidInput;
}

//========================================================================================================

OdResult OdDgEllipseElement2dCurvePE::extend( OdDgCurveElement2d* pCurve, 
                                             bool extendStart,
                                             const OdGePoint2d& toPoint )
{
  return eInvalidInput;
}

//========================================================================================================

OdResult OdDgEllipseElement2dCurvePE::getSpline (const OdDgCurveElement2d* pCurve, 
                                                 OdDgBSplineCurve2dPtr& spline) const
{
  OdDgEllipse2d* pArc = (OdDgEllipse2d*)(pCurve);
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
    knots[j] = (knots[j]-dStartKnot) / dKnotInterval;
  }

  spline = OdDgBSplineCurve2d::createObject();
  spline->setNurbsData( degree + 1, rational, false, ctl2d, knots, ww );

  return eOk;
}

//========================================================================================================

OdResult OdDgEllipseElement2dCurvePE::getClosestPointTo(const OdDgCurveElement2d* pCurve, 
                                                        const OdGePoint2d& givenPoint,
                                                        OdGePoint2d& pointOnCurve, 
                                                        bool extend ) const
{
  OdDgEllipse2d* pArc = (OdDgEllipse2d*)(pCurve);

  OdGeEllipArc2d geArc = pArc->getEllipArc();

  double param;
  const OdGeInterval ivFull(0.0, Oda2PI);
  pointOnCurve = geArc.closestPointTo( givenPoint, param, extend ? &ivFull : NULL );

  return eOk;
}

//----------------------------------------------------------
//
// OdDgEllipseElement3dCurvePE
//
//----------------------------------------------------------

OdResult OdDgEllipseElement3dCurvePE::getOrthoProjectedCurve( const OdDgCurveElement3d* pCurve, 
                                                             const OdGePlane& projPlane, 
                                                             OdDgCurveElement3dPtr* pProjCurve) const
{
  return getProjectedCurve( pCurve, projPlane, projPlane.normal(), pProjCurve );
}

//========================================================================================================

OdResult OdDgEllipseElement3dCurvePE::getProjectedCurve(const OdDgCurveElement3d* pCurve, 
                                                        const OdGePlane& projPlane,
                                                        const OdGeVector3d& projDirection, 
                                                        OdDgCurveElement3dPtr* pProjCurve) const
{
  if( projDirection.isZeroLength() || projDirection.isPerpendicularTo(projPlane.normal()) )
    return eInvalidInput;

  OdDgEllipse3dPtr pEllipse = pCurve;
  OdGeEllipArc3d geArc = pEllipse->getEllipArc();

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
      OdDgArc3dPtr pNewArc = OdDgArc3d::createObject();
      pNewArc->setPrimaryAxis( pEllipse->getPrimaryAxis() );
      pNewArc->setSecondaryAxis( pEllipse->getSecondaryAxis() );
      pNewArc->setOrigin( pEllipse->origin() );
      pNewArc->setRotation( pEllipse->getRotation() );
      pNewArc->setPropertiesFrom( pEllipse );
      pNewArc->transformBy( OdGeMatrix3d::projection( projPlane, projDirection ) );
      *pProjCurve = pNewArc;
    }
    catch(OdError* e)
    {
      return e->code();
    }
  }
  return eOk;
}

//========================================================================================================

OdResult OdDgEllipseElement3dCurvePE::getOffsetCurves(const OdDgCurveElement3d* pCurve, 
                                                      double offsetDistance,  
                                                      OdRxObjectPtrArray& offsetCurves) const
{
  OdDgEllipse3d* pArc = (OdDgEllipse3d*)(pCurve);

  if( (pArc->getPrimaryAxis() + offsetDistance <= 0) || 
    (pArc->getSecondaryAxis() + offsetDistance <= 0)
    )
  {
    return eInvalidInput;
  }

  OdDgEllipse3dPtr pNewArc = pArc->clone();
  pNewArc->setPrimaryAxis( pNewArc->getPrimaryAxis() + offsetDistance );
  pNewArc->setSecondaryAxis( pNewArc->getSecondaryAxis() + offsetDistance );
  offsetCurves.append( pNewArc.get() );
  return eOk;
}

//========================================================================================================

OdResult OdDgEllipseElement3dCurvePE::getOffsetCurvesGivenPlaneNormal(const OdDgCurveElement3d* pCurve, 
                                                                      const OdGeVector3d& normal, 
                                                                      double offsetDistance,
                                                                      OdRxObjectPtrArray& offsetCurves) const
{
  if( normal.isZeroLength() ) 
    return eInvalidInput;

  OdDgEllipse3dPtr pArc = pCurve;
  OdGeEllipArc3d geArc = pArc->getEllipArc();

  if( !geArc.normal().isParallelTo( normal ) ) 
    return eInvalidInput;

  return getOffsetCurves( pCurve, offsetDistance, offsetCurves );
}

//========================================================================================================

OdResult OdDgEllipseElement3dCurvePE::getSplitCurves(const OdDgCurveElement3d* pCurve, 
                                                     const OdGeDoubleArray& params, 
                                                     OdRxObjectPtrArray& curveSegments) const
{
  OdDgEllipse3d* pEllipse = (OdDgEllipse3d*)(pCurve);

  double prevParam = 0;

  for( unsigned i = 0; i < params.size(); i++ )
  {
    if( params[i] < 0 || params[i] > Oda2PI ) 
      return eInvalidInput;

    if ( prevParam != params[i] )
    {
      OdDgArc3dPtr pNewArc = OdDgArc3d::createObject();
      pNewArc->setPrimaryAxis( pEllipse->getPrimaryAxis() );
      pNewArc->setSecondaryAxis( pEllipse->getSecondaryAxis() );
      pNewArc->setOrigin( pEllipse->origin() );
      pNewArc->setRotation( pEllipse->getRotation() );
      pNewArc->setPropertiesFrom( pEllipse );
      pNewArc->setStartAngle( prevParam );
      pNewArc->setSweepAngle( params[i] - prevParam );
      curveSegments.append( pNewArc.get() );
    }

    prevParam = params[i];
  }

  if( prevParam != Oda2PI )
  {
    OdDgArc3dPtr pNewArc = OdDgArc3d::createObject();
    pNewArc->setPrimaryAxis( pEllipse->getPrimaryAxis() );
    pNewArc->setSecondaryAxis( pEllipse->getSecondaryAxis() );
    pNewArc->setOrigin( pEllipse->origin() );
    pNewArc->setRotation( pEllipse->getRotation() );
    pNewArc->setPropertiesFrom( pEllipse );
    pNewArc->setStartAngle( prevParam );
    pNewArc->setSweepAngle( Oda2PI - prevParam );
    curveSegments.append( pNewArc.get() );
  }
  return eOk;
}

//========================================================================================================

OdResult OdDgEllipseElement3dCurvePE::getSplitCurves(const OdDgCurveElement3d* pCurve, 
                                                     const OdGePoint3dArray& points, 
                                                     OdRxObjectPtrArray& curveSegments) const
{
  OdDgEllipse3d* pArc = (OdDgEllipse3d*)(pCurve);

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

OdResult OdDgEllipseElement3dCurvePE::extend(OdDgCurveElement3d* pCurve, double param)
{
  return eInvalidInput;
}

//========================================================================================================

OdResult OdDgEllipseElement3dCurvePE::extend(OdDgCurveElement3d* pCurve, 
                                             bool extendStart,
                                             const OdGePoint3d& toPoint)
{
  return eInvalidInput;
}

//========================================================================================================

OdResult OdDgEllipseElement3dCurvePE::getSpline(const OdDgCurveElement3d* pCurve, 
                                                OdDgBSplineCurve3dPtr& spline) const
{
  OdDgEllipse3d* pArc = (OdDgEllipse3d*)(pCurve);
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

OdResult OdDgEllipseElement3dCurvePE::getClosestPointTo(const OdDgCurveElement3d* pCurve, 
                                                        const OdGePoint3d& givenPoint,
                                                        OdGePoint3d& pointOnCurve, 
                                                        bool extend ) const
{
  OdDgEllipse3d* pArc = (OdDgEllipse3d*)(pCurve);
  OdGeEllipArc3d geArc = pArc->getEllipArc();

  double param;
  OdGeInterval ivFull(0.0, Oda2PI);
  pointOnCurve = geArc.closestPointTo( givenPoint, param, extend ? &ivFull : NULL );

  return eOk;
}

//========================================================================================================

OdResult OdDgEllipseElement3dCurvePE::getClosestPointTo(const OdDgCurveElement3d* pCurve, 
                                                        const OdGePoint3d& givenPoint,
                                                        const OdGeVector3d& direction,
                                                        OdGePoint3d& pointOnCurve, 
                                                        bool extend ) const
{
  if( direction.isZeroLength() ) 
    return eInvalidInput;

  OdDgEllipse3d* pArc = (OdDgEllipse3d*)(pCurve);
  OdGeEllipArc3d geArc = pArc->getEllipArc();

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
    if(pProjCurve->isKindOf(OdDgEllipse3d::desc()))
      res = getClosestPointTo( pProjCurve, givenPoint, pointOnCurve, extend );
    else
      res = pProjCurve->getClosestPointTo(givenPoint, direction, pointOnCurve, extend );

    if (res != eOk)
      return res;

    OdGePlane plane(pArc->origin(), vrNormal);
    pointOnCurve = pointOnCurve.project(plane, direction);
  }
  return eOk;
}

//========================================================================================================


