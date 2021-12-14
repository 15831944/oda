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
#include "OdCurveFunctions.h"
#include "RxObjectImpl.h"
#include "DbLine.h"
#include "DbArc.h"
#include "DbDatabase.h"
#include "DbSpline.h"
#include "Ge/GeLineSeg3d.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeCircArc3d.h"
#include "Ge/GeMatrix3d.h"
#include "Ge/GeEllipArc3d.h"
#include <memory>

ODRX_CONS_DEFINE_MEMBERS(OdDbCurvePE_Line,OdDbCurvePE,RXIMPL_CONSTR);
ODRX_CONS_DEFINE_MEMBERS(OdDbCurvePE_Arc,OdDbCurvePE,RXIMPL_CONSTR);

/************************************************************************/
/* Creates a new curve entity that is the result of projecting          */
/* the curve along the projPlane’s normal onto the projPlane and sets   */
/* pProjCurve to point to the newly created curve. pProjCurve is        */
/* declared as type OdDbCurvePtr to allow the returned curve to be any  */
/* class derived from OdDbCurve (for example, when an OdDbArc is        */
/* projected onto a plane, the result is an OdDbEllipse).               */
/************************************************************************/
OdResult OdDbCurvePE_Line::getOrthoProjectedCurve(const OdDbCurve* curve, 
                                                  const OdGePlane& projPlane, 
                                                  OdDbCurvePtr* pProjCurve) const
{
  OdDbLinePtr line = curve;
  OdGePoint3d pp1,pp2;
  projPlane.project( line->startPoint(), pp1 );
  projPlane.project( line->endPoint(), pp2 );
  OdDbLinePtr projLine = OdDbLine::createObject();
  projLine->setStartPoint(pp1);
  projLine->setEndPoint(pp2);
  *pProjCurve = projLine;
  return eOk;
}

/************************************************************************/
/* Creates a new curve entity that is the result of projecting the      */
/* curve parallel to projDir onto the projPlane and sets pProjCurve to  */
/* point to the newly created curve. pProjCurve is declared as type     */
/* OdDbCurvePtr in order to allow the returned curve to be any class    */
/* derived from OdDbCurve. For example, when an OdDbArc is projected    */
/* onto a plane, the result is an OdDbEllipse.                          */
/************************************************************************/
OdResult OdDbCurvePE_Line::getProjectedCurve(const OdDbCurve* curve, 
                                             const OdGePlane& projPlane,
                                             const OdGeVector3d& projDir, 
                                             OdDbCurvePtr* pProjCurve) const
{
  OdDbLinePtr line = curve;
  // If projDir is perpendicular to the projPlane’s normal then eInvalidInput is returned
  if (projDir.isPerpendicularTo(projPlane.normal()))
    return eInvalidInput;
  OdGePoint3d pp1 = line->startPoint().project(projPlane, projDir),
              pp2 = line->endPoint().project(projPlane, projDir);
  OdDbLinePtr projLine = OdDbLine::createObject();
  projLine->setStartPoint(pp1);
  projLine->setEndPoint(pp2);
  *pProjCurve = projLine;
  return eOk;
}

/************************************************************************/
/* Creates one or more entities that together make up the result        */ 
/* of offsetting the curve by the distance offsetDist. For many curves, */
/* the result is a single new curve. In some cases, it might be         */
/* necessary for the offset result to be several curves. To allow for   */
/* this possibility, a dynamic array is used to hold pointers to the    */
/* resulting entity or entities. The new curves may or may not be of    */
/* the same class as the original curve. For example, offsetting an     */
/* OdDbEllipse results in an OdDbSpline, since the result of offsetting */
/* an ellipse does NOT fit the equation of an ellipse.                  */
/*                                                                      */
/* Pointers to all new curves are appended to the offsetCurves array.   */
/************************************************************************/
OdResult OdDbCurvePE_Line::getOffsetCurves(const OdDbCurve* curve, 
                                           double offsetDist,  
                                           OdRxObjectPtrArray& offsetCurves) const
{
  OdDbLinePtr line = curve;
  OdGePoint3d p1 = line->startPoint();
  OdGePoint3d p2 = line->endPoint();
  OdGeVector3d v;
  // linear entities have no natural normal, therefore this function may not be 
  // applied quite correctly.
  // If object is database resident - offset in UCS plane (if possible)
  if ( curve->database() )
    v = curve->database()->getUCSXDIR().crossProduct(curve->database()->getUCSYDIR()).crossProduct(p2-p1);
  // if line is Z directed, offset anywhere
  if ( v.isZeroLength() )
    v = line->normal();
  v.normalize();
  OdDbLinePtr l = OdDbLine::createObject();
  l->setStartPoint( p1 + v * offsetDist );
  l->setEndPoint( p2 + v * offsetDist );
  offsetCurves.append(l.get());
  return eOk;
}

/************************************************************************/
/* Creates one or more entities that together make up the result        */ 
/* of offsetting the curve by the distance offset in toe plane with     */
/* normal vector normal. For many curves, the result is a single new    */
/* curve. In some cases, it might be necessary for the offset result    */
/* to be several curves. To allow for this possibility, a dynamic array */
/* is used to hold pointers to the resulting entity or entities. The    */
/* new curves may or may not be of the same class as the original       */
/* curve. For example, offsetting an OdDbEllipse results in an          */
/* OdDbSpline, since the result of offsetting an ellipse does NOT fit   */
/* the equation of an ellipse.                                          */
/*                                                                      */
/* Pointers to all new curves are appended to the offsetCurves array.   */
/************************************************************************/
OdResult OdDbCurvePE_Line::getOffsetCurvesGivenPlaneNormal(const OdDbCurve* curve, 
                                                           const OdGeVector3d& normal, 
                                                           double offsetDist,
                                                           OdRxObjectPtrArray& offsetCurves) const
{
  OdDbLinePtr line = curve;
  OdGePoint3d p1 = line->startPoint();
  OdGePoint3d p2 = line->endPoint();
  // 
  if ( normal.isZeroLength() || !normal.isPerpendicularTo(p2-p1) )
    return eInvalidInput;
  OdGeVector3d offset = normal.normal() * offsetDist;
  OdDbLinePtr l = OdDbLine::createObject();
  l->setStartPoint( p1 + offset );
  l->setEndPoint( p2 + offset );
  offsetCurves.append(l.get());
  return eOk;
}

/************************************************************************/
/* Creates one or more entities that are subcurves of the original.     */
/* curve. The params array contains the parameters of the points on the */
/* curve that will be used as the start and/or end points of the newly  */
/* created subcurves. The first subcurve starts at the original curve’s */
/* start point and ends at the first element of the params array. The   */
/* second subcurve starts at the first element of the array and ends at */
/* the third, and so in. The last subcurve starts at the last element   */
/* of the params array and ends at the original curve’s endpoint.       */
/*                                                                      */
/* If two consecutive parameters are coincident (including the original */
/* curve’s start and end point, then no subcurve will be created for    */
/* that pair.                                                           */
/*                                                                      */
/* Pointers to all new curves are appended to the curveSegments array.  */
/************************************************************************/
OdResult OdDbCurvePE_Line::getSplitCurves (const OdDbCurve* curve, 
                                           const OdGeDoubleArray& params, 
                                           OdRxObjectPtrArray& curveSegments) const
{
  OdDbLinePtr line = curve;
  OdGePoint3d p1 = line->startPoint();
  OdGeVector3d v = line->endPoint() - p1;
  if ( v.isZeroLength() ) return eDegenerateGeometry;
  double len = v.length();
  v.normalize();
  double prevParam = 0;
  for ( unsigned i = 0; i < params.size(); i++ )
  {
    if ( params[i] > len || params[i] < 0 ) return eInvalidInput;
    if ( prevParam != params[i] )
    {
      OdDbLinePtr l = OdDbLine::createObject();
      l->setStartPoint( p1 + v*prevParam );
      l->setEndPoint( p1 + v*params[i] );
      curveSegments.append( l.get() );
    }
    prevParam = params[i];
  }
  return eOk;
}

/************************************************************************/
/* Creates one or more entities that are subcurves of the original.     */
/* curve. The points array contains the points on the curve that        */
/* will be used as the start and/or end points of the newly created     */
/* subcurves. The first subcurve starts at the original curve’s start   */
/* point and ends at the first element of the params array. The second  */
/* subcurve starts at the first element of the array and ends at the    */
/* second. The third subcurve starts at the second element and ends at  */
/* the third, and so in. The last subcurve starts at the last element   */
/* of the params array and ends at the original curve’s endpoint.       */
/*                                                                      */
/* If two consecutive parameters are coincident (including the original */
/* curve’s start and end point, then no subcurve will be created for    */
/* that pair.                                                           */
/*                                                                      */
/* Pointers to all new curves are appended to the curveSegments array.  */
/************************************************************************/
OdResult OdDbCurvePE_Line::getSplitCurves (const OdDbCurve* curve, 
                                           const OdGePoint3dArray& points, 
                                           OdRxObjectPtrArray& curveSegments) const
{
  OdDbLinePtr line = curve;
  OdGePoint3d p1 = line->startPoint();
  OdGePoint3d p2 = line->endPoint();
  OdGeVector3d v = p2 - p1;
  if ( v.isZeroLength() ) return eDegenerateGeometry;
  v.normalize();
  double prevParam = 0;
  for ( unsigned i = 0; i < points.size(); i++ )
  {
    double param; 
    if ( line->getParamAtPoint(points[i],param) != eOk ) return eInvalidInput;
    if ( prevParam != param )
    {
      OdDbLinePtr l = OdDbLine::createObject();
      l->setStartPoint( p1 + v*prevParam );
      l->setEndPoint( p1 + v*param );
      curveSegments.append( l.get() );
    }
    prevParam = param;
  }
  return eOk;
}

/************************************************************************/
/* Extends the beginning or end of the curve to the new point           */
/* determined by the param value.                                       */
/*                                                                      */
/* If param is less than the curve’s start parameter, the curve is      */
/* set to start at param. If param is greater than the curve’s          */
/* end parameter, then the curve is reset to end param.                 */    
/************************************************************************/
OdResult OdDbCurvePE_Line::extend(OdDbCurve* curve, double param)
{
  OdDbLinePtr line = curve;
  OdGePoint3d p1 = line->startPoint();
  OdGePoint3d p2 = line->endPoint();
  OdGeVector3d v = p2 - p1;
  if ( v.isZeroLength() ) return eDegenerateGeometry;
  if ( param < 0 )
    line->setStartPoint( p1 + v.normal() * param );
  else if ( param > v.length() )
    line->setEndPoint( p1 + v.normal() * param );
  else 
    return eInvalidInput;
  return eOk;
}
/************************************************************************/
/* Extends the curve to the WCS point toPoint.                          */
/*                                                                      */
/* If extendStart == true, then the curve’s beginning is extended. If   */
/* extendStart == false, then the curve’s end is extended.              */
/*                                                                      */                                                  /* eInvalidInput is returned if toPoint is not on the path of the       */
/* curve.                                                               */
/************************************************************************/
OdResult OdDbCurvePE_Line::extend(OdDbCurve* curve, bool extendStart,
                                  const OdGePoint3d& toPoint)
{
  OdDbLinePtr line = curve;
  OdGePoint3d p1 = line->startPoint();
  OdGePoint3d p2 = line->endPoint();
  if ( p1 == p2 ) return eDegenerateGeometry;
  OdGeLine3d l(p1,p2);
  if ( !l.isOn(toPoint) ) return eInvalidInput;
  double param = l.paramOf(toPoint);
  if ( extendStart )
  {
    if ( param > 0 ) return eInvalidInput;
    line->setStartPoint( toPoint );
  }
  else
  {
    if ( param < 1 ) return eInvalidInput;
    line->setEndPoint( toPoint );
  }
  return eOk;
}

/************************************************************************/
/* Helper function to convert OdGeNurbCurve3d to OdDbSpline             */
/************************************************************************/
static OdDbSplinePtr createSplineFromNurbs( const OdGeNurbCurve3d& nurbs )
{
  OdGePoint3dArray ctl;
  OdGeDoubleArray ww;
  OdGeKnotVector knots;
  int degree;
  bool rational, periodic;
  nurbs.getDefinitionData(
    degree, rational, periodic, knots, ctl, ww );
  OdDbSplinePtr spline = OdDbSpline::createObject();
  spline->setNurbsData( degree, rational, false, false, ctl, knots, ww, 
    OdGeContext::gTol.equalPoint() );
  return spline;
}

/************************************************************************/
/* Returns an OdDbSpline approximation of the specified curve.          */
/************************************************************************/
OdResult OdDbCurvePE_Line::getSpline (const OdDbCurve* curve, 
                                      OdDbSplinePtr* spline) const
{
  OdDbLinePtr line = curve;
  OdGePoint3d p1 = line->startPoint();
  OdGePoint3d p2 = line->endPoint();
  *spline = createSplineFromNurbs(OdGeNurbCurve3d(OdGeLineSeg3d(p1,p2)));
  return eOk;
}

/************************************************************************/
/* Returns the point on the specified curve closest to the given point  */
/* The curve is extended along its path to find the closest point if    */
/* and only if extend == true.                                          */
/************************************************************************/
OdResult OdDbCurvePE_Line::getClosestPointTo(const OdDbCurve* curve, 
                                             const OdGePoint3d& givenPoint,
                                             OdGePoint3d& pointOnCurve, 
                                             bool extend ) const
{
  OdDbLinePtr line = curve;
  OdGePoint3d p1 = line->startPoint();
  OdGePoint3d p2 = line->endPoint();
  if ( p1.isEqualTo(p2) )
    pointOnCurve = p1;
  else if ( extend )
    pointOnCurve = OdGeLine3d(p1,p2).closestPointTo(givenPoint);
  else
    pointOnCurve = OdGeLineSeg3d(p1,p2).closestPointTo(givenPoint);
  return eOk;
}

/************************************************************************/
/* This function projects curve onto the plane defined by givenPoint    */
/* and normal, finds the closest point on the projected curve to        */
/* givenPoint, projects said closest point back onto the plane of the   */
/* curve. This point is returned.                                       */
/*                                                                      */
/* The curve is extended along its path to find the closest point if    */
/* and only if extend == true.                                          */
/************************************************************************/
OdResult OdDbCurvePE_Line::getClosestPointTo(const OdDbCurve* curve, 
                                             const OdGePoint3d& givenPoint,
                                             const OdGeVector3d& normal,
                                             OdGePoint3d& pointOnCurve, 
                                             bool extend ) const
{
  OdDbCurvePtr pProjCurve;
  getOrthoProjectedCurve( curve, OdGePlane(givenPoint,normal), &pProjCurve );
  getClosestPointTo( pProjCurve, givenPoint, pointOnCurve, extend );
  OdDbLinePtr line = curve;
  OdGePoint3d p1 = line->startPoint();
  OdGePoint3d p2 = line->endPoint();
  if ( !OdGeLine3d(p1,p2).intersectWith( OdGeLine3d(pointOnCurve,normal),pointOnCurve) )
  { // normal parallel to line
    if ( !extend ) // if extend => already set
      pointOnCurve = OdGeLineSeg3d(p1,p2).closestPointTo(pointOnCurve);
  }
  return eOk;
}
/************************************************************************/
/* Returns the startpoint, midpoint and endpoint of the specified arc.  */                                                 /************************************************************************/
static void getPointsFromArc(OdDbArc* arc, OdGePoint3d& startPoint, OdGePoint3d& midPoint, OdGePoint3d& endPoint )
{
  arc->getStartPoint( startPoint );
  arc->getEndPoint( endPoint );
  double start,end;
  arc->getStartParam( start );
  arc->getEndParam ( end );
  arc->getPointAtParam( (start + end)/2, midPoint );
}

/************************************************************************/
/* Creates a curve entity that is the result of projecting the          */
/* curve parallel to the normal of the specified specified projection   */
/* onto the plane.                                                      */
/* The projected curve may or may not be of the same class as the       */
/* curve. For example, projecting an OdDbArc results in an              */
/* OdDbEllipse.                                                         */
/************************************************************************/
OdResult OdDbCurvePE_Arc::getOrthoProjectedCurve(const OdDbCurve* curve, 
                                                  const OdGePlane& plane, 
                                                  OdDbCurvePtr* projCrv) const
{
  return getProjectedCurve( curve, plane, plane.normal(), projCrv );
}
/************************************************************************/
/* Creates a curve entity that is the result of projecting the          */
/* curve parallel to projDir onto the specified projection plane.       */
/* The projected curve may or may not be of the same class as the       */
/* curve. For example, projecting an OdDbArc results in an              */
/* OdDbEllipse.                                                         */
/************************************************************************/
OdResult OdDbCurvePE_Arc::getProjectedCurve(const OdDbCurve* curve, 
                                             const OdGePlane& plane,
                                             const OdGeVector3d& projDir, 
                                             OdDbCurvePtr* projCrv) const
{
  if ( projDir.isZeroLength() || projDir.isPerpendicularTo(plane.normal()) )
    return eInvalidInput;
  OdDbArcPtr arc = curve;
  if ( projDir.isPerpendicularTo(arc->normal()) )
  {
    OdGePoint3d p1,p2,p3;
    getPointsFromArc( OdDbArcPtr(curve), p1, p2, p3 );
    OdGeEllipArc3d ea(OdGeCircArc3d(p1,p2,p3));
    std::unique_ptr<OdGeLineSeg3d> seg((OdGeLineSeg3d*)ea.orthoProject( plane ));
    OdDbLinePtr line = OdDbLine::createObject();
    line->setNormal( plane.normal() );
    line->setStartPoint( seg->startPoint() );
    line->setEndPoint( seg->endPoint() );
    *projCrv = line;
  }
  else
  {
    OdDbEntityPtr ent;
    arc->getTransformedCopy( OdGeMatrix3d::projection( plane, projDir ), ent );
    *projCrv = ent;
  }
  return eOk;
}
/************************************************************************/
/* Creates one or more entities that together make up the result        */ 
/* of offsetting the curve by the distance offsetDist. For many curves, */
/* the result is a single new curve. In some cases, it might be         */
/* necessary for the offset result to be several curves. To allow for   */
/* this possibility, a dynamic array is used to hold pointers to the    */
/* resulting entity or entities. The new curves may or may not be of    */
/* the same class as the original curve. For example, offsetting an     */
/* OdDbEllipse results in an OdDbSpline, since the result of offsetting */
/* an ellipse does NOT fit the equation of an ellipse.                  */
/*                                                                      */
/* Pointers to all new curves are appended to the offsetCurves array.   */
/************************************************************************/
OdResult OdDbCurvePE_Arc::getOffsetCurves(const OdDbCurve* curve, 
                                           double offsetDist,
                                           OdRxObjectPtrArray& offsetCurves) const
{
  if ( OdDbArcPtr(curve)->radius() + offsetDist <= 0 ) return eInvalidInput;
  OdDbArcPtr arc = curve->clone();
  arc->setRadius( arc->radius() + offsetDist );
  offsetCurves.append( arc.get() );
  return eOk;
}

/************************************************************************/
/* Creates one or more entities that together make up the result        */ 
/* of offsetting the curve by the distance offset in toe plane with     */
/* normal vector normal. For many curves, the result is a single new    */
/* curve. In some cases, it might be necessary for the offset result    */
/* to be several curves. To allow for this possibility, a dynamic array */
/* is used to hold pointers to the resulting entity or entities. The    */
/* new curves may or may not be of the same class as the original       */
/* curve. For example, offsetting an OdDbEllipse results in an          */
/* OdDbSpline, since the result of offsetting an ellipse does NOT fit   */
/* the equation of an ellipse.                                          */
/*                                                                      */
/* Pointers to all new curves are appended to the offsetCurves array.   */
/************************************************************************/
OdResult OdDbCurvePE_Arc::getOffsetCurvesGivenPlaneNormal(const OdDbCurve* curve, 
                                                           const OdGeVector3d& normal, 
                                                           double offsetDist,
                                                           OdRxObjectPtrArray& offsetCurves) const
{
  if ( normal.isZeroLength() ) return eInvalidInput;
  if ( !OdDbArcPtr(curve)->normal().isParallelTo( normal ) ) return eInvalidInput;
  return getOffsetCurves( curve, offsetDist, offsetCurves );
}

/************************************************************************/
/* Creates one or more entities that are subcurves of the original.     */
/* curve. The params array contains the parameters of the points on the */
/* curve that will be used as the start and/or end points of the newly  */
/* created subcurves. The first subcurve starts at the original curve’s */
/* start point and ends at the first element of the params array. The   */
/* second subcurve starts at the first element of the array and ends at */
/* the third, and so in. The last subcurve starts at the last element   */
/* of the params array and ends at the original curve’s endpoint.       */
/*                                                                      */
/* If two consecutive parameters are coincident (including the original */
/* curve’s start and end point, then no subcurve will be created for    */
/* that pair.                                                           */
/*                                                                      */
/* Pointers to all new curves are appended to the curveSegments array.  */
/************************************************************************/
OdResult OdDbCurvePE_Arc::getSplitCurves (const OdDbCurve* curve, 
                                           const OdGeDoubleArray& params, 
                                           OdRxObjectPtrArray& curveSegments) const
{
  OdDbArcPtr arc = curve;
  double prevParam = 0;
  for ( unsigned i = 0; i < params.size(); i++ )
  {
    if ( params[i] > arc->endAngle() || params[i] < arc->startAngle() ) return eInvalidInput;
    if ( prevParam != params[i] )
    {
      OdDbArcPtr clone = arc->clone();
      clone->setStartAngle( prevParam );
      clone->setEndAngle( params[i] );
      curveSegments.append( clone.get() );
    }
    prevParam = params[i];
  }
  return eOk;
}

/************************************************************************/
/* Creates one or more entities that are subcurves of the original.     */
/* curve. The points array contains the points on the curve that        */
/* will be used as the start and/or end points of the newly created     */
/* subcurves. The first subcurve starts at the original curve’s start   */
/* point and ends at the first element of the params array. The second  */
/* subcurve starts at the first element of the array and ends at the    */
/* second. The third subcurve starts at the second element and ends at  */
/* the third, and so in. The last subcurve starts at the last element   */
/* of the params array and ends at the original curve’s endpoint.       */
/*                                                                      */
/* If two consecutive parameters are coincident (including the original */
/* curve’s start and end point, then no subcurve will be created for    */
/* that pair.                                                           */
/*                                                                      */
/* Pointers to all new curves are appended to the curveSegments array.  */
/************************************************************************/
OdResult OdDbCurvePE_Arc::getSplitCurves (const OdDbCurve* curve, 
                                           const OdGePoint3dArray& points, 
                                           OdRxObjectPtrArray& curveSegments) const
{
  OdDbArcPtr arc = curve;
  double prevParam = 0;
  for ( unsigned i = 0; i < points.size(); i++ )
  {
    double param;
    if ( eOk != arc->getParamAtPoint( points[i], param )) return eInvalidInput;
    if ( prevParam != param )
    {
      OdDbArcPtr clone = arc->clone();
      clone->setStartAngle( prevParam );
      clone->setEndAngle( param );
      curveSegments.append( clone.get() );
    }
    prevParam = param;
  }
  return eOk;
}

// Extends the beginning or end of the curve to the new point 
// determined by the newParam value.
// If newParam is less than the curve’s start parameter, then the curve is 
// reset to start at newParam. If newParam is greater than the curve’s end 
// parameter, then the curve is reset to end at newParam.
//

/************************************************************************/
/* Extends the beginning or end of the curve to the new point           */
/* determined by the param value.                                       */
/*                                                                      */
/* If param is less than the curve’s start parameter, the curve is      */
/* set to start at param. If param is greater than the curve’s          */
/* end parameter, then the curve is reset to end param.                 */    
/************************************************************************/
OdResult OdDbCurvePE_Arc::extend(OdDbCurve* curve, double param)
{
  OdDbArcPtr arc = curve;
  if ( param < arc->startAngle() )
    arc->setStartAngle( param );
  else if ( param > arc->endAngle() ) 
    arc->setEndAngle( param );
  else 
    return eInvalidInput;
  return eOk;
}

/************************************************************************/
/* Extends the curve to the WCS point toPoint.                          */
/*                                                                      */
/* If extendStart == true, then the curve’s beginning is extended. If   */
/* extendStart == false, then the curve’s end is extended.              */
/*                                                                      */                                                  /* eInvalidInput is returned if toPoint is not on the path of the       */
/* curve.                                                               */
/************************************************************************/
OdResult OdDbCurvePE_Arc::extend(OdDbCurve* curve, bool extendStart,
                                  const OdGePoint3d& toPoint)
{
  OdDbArcPtr arc = curve;
  OdGeVector3d v = toPoint - arc->center();
  if ( !OdEqual( v.length(), arc->radius(), OdGeContext::gTol.equalPoint() ) ) return eInvalidInput;
  double param;
  if ( arc->getParamAtPoint( toPoint, param ) == eOk )return eInvalidInput;
  if ( extendStart )
  {
    OdGePoint3d p; arc->getStartPoint( p );
    arc->setStartAngle( arc->startAngle() - 
      v.angleTo( p-arc->center(), arc->normal() ) );
  }
  else
  {
    OdGePoint3d p; arc->getEndPoint( p );
    arc->setEndAngle( arc->endAngle() + 
      (p - arc->center()).angleTo( v, arc->normal() ) );
  }
  return eOk;
}

/************************************************************************/
/* Returns an OdDbSpline approximation of the specified curve.          */
/************************************************************************/
OdResult OdDbCurvePE_Arc::getSpline (const OdDbCurve* curve, 
                                      OdDbSplinePtr* spline) const
{
  OdGePoint3d p1,p2,p3;
  getPointsFromArc( OdDbArcPtr(curve), p1, p2, p3 );
  *spline = createSplineFromNurbs(OdGeNurbCurve3d(OdGeEllipArc3d(OdGeCircArc3d(p1,p2,p3))));
  return eOk;
}

/************************************************************************/
/* Returns the point on the specified curve closest to the given point  */
/* The curve is extended along its path to find the closest point if    */
/* and only if extend == true.                                          */
/************************************************************************/
OdResult OdDbCurvePE_Arc::getClosestPointTo(const OdDbCurve* curve, 
                                             const OdGePoint3d& givenPoint,
                                             OdGePoint3d& pointOnCurve, 
                                             bool extend ) const
{
  if ( extend )
  {
    OdDbArcPtr arc = curve;
    OdGeCircArc3d geArc(arc->center(),arc->normal(),arc->radius());
    pointOnCurve = geArc.closestPointTo( givenPoint );
  }
  else
  {
    OdGePoint3d p1,p2,p3;
    getPointsFromArc( OdDbArcPtr(curve), p1, p2, p3 );
    pointOnCurve = OdGeCircArc3d(p1,p2,p3).closestPointTo( givenPoint );
  }
  return eOk;
}
/************************************************************************/
/* This function projects curve onto the plane defined by givenPoint    */
/* and normal, finds the closest point on the projected curve to        */
/* givenPoint, projects said closest point back onto the plane of the   */
/* curve. This point is returned.                                       */
/*                                                                      */
/* The curve is extended along its path to find the closest point if    */
/* and only if extend == true.                                          */
/************************************************************************/
OdResult OdDbCurvePE_Arc::getClosestPointTo(const OdDbCurve* curve, 
                                             const OdGePoint3d& givenPoint,
                                             const OdGeVector3d& direction,
                                             OdGePoint3d& pointOnCurve, 
                                             bool extend ) const
{
  if ( direction.isZeroLength() ) return eInvalidInput;
  OdDbArcPtr arc = curve;
  if ( direction.isPerpendicularTo(arc->normal()) )
  {
    OdGePlane plane(givenPoint,direction);
    if ( extend )
    {
      OdGePoint3d c;
      plane.project( arc->center(), c );
      OdGeVector3d v(direction.crossProduct(arc->normal()));
      v.normalize();
      v*=arc->radius();
      OdGePoint3d p = OdGeLineSeg3d( c - v, c + v ).closestPointTo( givenPoint );
      OdGeVector3d x = p - c;
      OdGeVector3d y = direction * sqrt(arc->radius()*arc->radius() - x.lengthSqrd());
      pointOnCurve = arc->center() + x + y;
    }
    else
    {
      OdGePoint3d p1,p2,p3;
      getPointsFromArc( OdDbArcPtr(curve), p1, p2, p3 );
      OdGeEllipArc3d ea(OdGeCircArc3d(p1,p2,p3));
      std::unique_ptr<OdGeLineSeg3d> seg((OdGeLineSeg3d*)ea.orthoProject( plane ));
      OdGePoint3d p = seg->closestPointTo( givenPoint );
      int intn;
      OdGePoint3d ip1,ip2;
      if (!ea.intersectWith( OdGeLine3d( p, direction ), intn, ip1, ip2 )) return eInvalidInput;
      pointOnCurve = ip1.distanceTo(p) > ip2.distanceTo(p) ? ip2 : ip1;
    }
  }
  else
  {
    OdDbCurvePtr pProjCurve;
    OdResult res = getOrthoProjectedCurve( curve, OdGePlane(givenPoint,direction), &pProjCurve );
    if (res != eOk)
      return res;
    if (pProjCurve->isKindOf(OdDbArc::desc())) // AMark : causes infinite loop if arc projected to arc
      res = getClosestPointTo( pProjCurve, givenPoint, pointOnCurve, extend );
    else
      res = pProjCurve->getClosestPointTo(givenPoint, direction, pointOnCurve, extend ); //#4922
    if (res != eOk)
      return res;
    OdGePlane plane(arc->center(), arc->normal());
    pointOnCurve = pointOnCurve.project(plane, direction);
  }
  return eOk;
}
