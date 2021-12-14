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

#ifndef OD_GECIRCARC3D_H
#define OD_GECIRCARC3D_H /*!DOM*/

#include "Ge/GeCurve3d.h"
#include "Ge/GePlane.h"

#include "TD_PackPush.h"

class OdGeLine3d;
class OdGeCircArc2d;
class OdGePlanarEnt;
class OdGeExtents3d;

/** \details
  A mathematical entity used to represent a circular arc in 3D space.

  Corresponding C++ library: TD_Ge

  <group OdGe_Classes>

  \sa
  <link ge_OdGeCircArc3d, Working with Circular Arcs>
*/
class GE_TOOLKIT_EXPORT OdGeCircArc3d : public OdGeCurve3d
{
public:

  /** \details 
    Default constructor for objects of the OdGeCircArc3d class. Constructs a
    full circle in the XY plane (normal vector (0.0, 0.0, 1.0)) with center
    (0.0, 0.0, 0.0), radius of 1.0, and reference axis (1.0, 0.0, 0.0).
  */
  OdGeCircArc3d();

  /** \details
    Copy constructor for objects of the OdGeCircArc3d class.
    \param source      [in]  3D arc to be copied.

    \remarks
    The constructor creates an arc that is a copy of an input arc.
  */
  OdGeCircArc3d(
    const OdGeCircArc3d& source);

  /** \details
    Constructor for objects of the OdGeCircArc3d class.
    \param center      [in]  Center of a circle.
    \param normal      [in]  Normal vector of a 3D circle.
    \param radius      [in]  Radius of a circle.

    \remarks
    The constructor creates a circle with the specified parameters.
  */
  OdGeCircArc3d(
    const OdGePoint3d& center,
    const OdGeVector3d& normal,
    double radius);

  /** \details
    Constructor for objects of the OdGeCircArc3d class.
    \param center      [in]  Center of an arc.
    \param normal      [in]  Normal vector of a 3D circle.
    \param refVec      [in]  Reference vector defining arc angle 0.
    \param radius      [in]  Radius of the circular arc.
    \param startAng    [in]  Starting angle of an arc.
    \param endAng      [in]  Ending angle of an arc.

    \remarks
    The constructor creates a circular arc with the specified parameters.
    \remarks
    Angles are measured by drawing a vector between a point on the arc and the
    center point and taking the angle between this vector and refVec.
    \remarks
    The angle is measured in a counterclockwise direction about the normal
    vector.
    \remarks
    The normal vector must be perpendicular to refVec, and endAngle must be
    greater than startAngle.
    All angles are expressed in radians.
    \remarks
    To construct a full circle, specify the endAngle and startAngle, so the
    difference between them is 2 x Pi.
    In common case parameter (paramOf, evalPoint, evaluate) interval is NOT
    equal to [startAng; endAng]! But its length is equal to endAng - startAng.
  */
  OdGeCircArc3d(
    const OdGePoint3d& center,
    const OdGeVector3d& normal,
    const OdGeVector3d& refVec,
    double radius, double startAng = 0,
    double endAng = Oda2PI);

  /** \details
    Constructor for objects of the OdGeCircArc3d class.
    \param startPoint   [in]  Startpoint of an arc.
    \param secondPoint  [in]  Second point on a 3-point arc.
    \param endPoint     [in]  Endpoint of an arc.

    \remarks
    This constructor always constructs a bounded arc and cannot be used to
    construct a full circle.
  */
  OdGeCircArc3d(
    const OdGePoint3d& startPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& endPoint);

  /** \details
    Returns the point on this circle that is the closest to the specified plane,
    and the point on the plane that is the closest to this circle.

    \param plane         [in]  Any plane.
    \param pointOnPlane [out]  Receives the closest point on plane.
    \param tol           [in]  Geometric tolerance.
  */
  OdGePoint3d closestPointToPlane(
    const OdGePlanarEnt& plane,
    OdGePoint3d& pointOnPlane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Gets the intersections with other objects.

    \param line    [in]  Any 3D linear entity.
    \param numInt [out]  Receives the number of intersections.
    \param p1     [out]  Receives the first intersection point.
    \param p2     [out]  Receives the second intersection point.
    \param tol     [in]  Geometric tolerance.
    \returns True if the specified line entity intersects this arc entity.
  */
  bool intersectWith(
    const OdGeLinearEnt3d& line,
    int& numInt,
    OdGePoint3d& p1,
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Gets the intersections with other arc objects.

    \param arc      [in]  Any 3D arc entity.
    \param numInt   [out]  Receives the number of intersections.
    \param p1       [out]  Receives the first intersection point on the arc.
    \param p2       [out]  Receives the second intersection point on the arc.
    \param tol      [in]  Geometric tolerance.
    \returns
    true if the specified arc intersects this arc entity, false otherwise.
    \remarks
    * p1 has meaning only if numInt > 0.
    * p2 has meaning only if numInt > 1.
  */
  bool intersectWith(
    const OdGeCircArc3d& arc,
    int& numInt,
    OdGePoint3d& p1,
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Gets the number of intersections and points of intersection.

    \param plane   [in]  Any plane entity.
    \param numInt [out]  Receives the number of intersections.
    \param p1     [out]  Receives the first intersection point on the arc.
    \param p2     [out]  Receives the second intersection point on the arc.
    \param tol     [in]  Geometric tolerance.
    \returns True if the specified plane entity intersects this arc entity.

    \remarks
    * p1 has meaning only if numInt > 0.
    * p2 has meaning only if numInt > 1.
  */
  bool intersectWith(
    const OdGePlanarEnt& plane,
    int& numInt,
    OdGePoint3d& p1,
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns True if the projected points of the arc intersect with the
    specified linear entity, and returns the number of intersections
    and points of intersection.

    \param line        [in]  Any 3D linear entity.
    \param projDir     [in]  Projection direction.
    \param numInt     [out]  Receives the number of intersections.
    \param pntOnArc1  [out]  Receives the first intersection point on the arc.
    \param pntOnArc2  [out]  Receives the second intersection point on the arc.
    \param pntOnLine1 [out]  Receives the first intersection point on the line.
    \param pntOnLine2 [out]  Receives the second intersection point on the line.
    \param tol         [in]  Geometric tolerance.
  */
  bool projIntersectWith(
    const OdGeLinearEnt3d& line,
    const OdGeVector3d& projDir,
    int& numInt,
    OdGePoint3d& pntOnArc1,
    OdGePoint3d& pntOnArc2,
    OdGePoint3d& pntOnLine1,
    OdGePoint3d& pntOnLine2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified point is on the full circle of
    this arc, the tangent at that point.

    \param point [in]  Point on the full circle.
    \param line [out]  Receives the tangent line at that point.
    \param tol   [in]  Geometric tolerance.
  */
  bool tangent(
    const OdGePoint3d& point,
    OdGeLine3d& line,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified point is on the full circle of
    this arc, the tangent at that point, and the status of the query.

    \param point   [in]  Point on the full circle.
    \param line   [out]  Receives the tangent line at that point.
    \param tol     [in]  Geometric tolerance.
    \param status [out]  Receives the status of the query.

    \remarks
    Possible status values:

    * kArg1TooBig
    * kArg1InsideThis
    * kArg1OnThis
  */
  bool tangent(
    const OdGePoint3d& point,
    OdGeLine3d& line,
    const OdGeTol& tol,
    OdGeError& status) const;

  /** \details
    Returns the plane of the arc.

    \param plane [out]  Receives the plane of the arc.
  */
  void getPlane(
    OdGePlane& plane) const;

  /** \details
    Returns true if and only if the specified point lies inside the full circle
    of this arc, and is on the same plane as this arc.

    \param point [in]  Any 3D point.
    \param tol   [in]  Geometric tolerance.
  */
  bool isInside(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the center of this arc.
  */
  OdGePoint3d center() const;

  /** \details
    Returns the vector normal to the plane of this arc.
  */
  OdGeVector3d normal() const;

  /** \details
    Returns the reference vector as a unit vector.
  */
  OdGeVector3d refVec() const;

  /** \details
    Returns the radius of this arc.
  */
  double radius() const;

  /** \details
    Gets the starting angle measured from the reference vector in the arc
    direction.

    \returns The starting angle value.
    \remarks
    All angles are expressed in radians.
    \remarks
    The result of this method should not be used as a curve parameter.
    The angles and its corresponding parameter may differ.
  */
  double startAng() const;

  /** \details
    Gets the ending angle measured from the reference vector in the arc
    direction.

    \returns The ending angle value.
    \remarks
    All angles are expressed in radians.
    \remarks
    The result of this method should not be used as a curve parameter.
    The angles and its corresponding parameter may differ.
  */
  double endAng() const;

  /** \details
    Returns the start point of this arc.
  */
  OdGePoint3d startPoint() const;

  /** \details
    Returns the end point of this arc.
  */
  OdGePoint3d endPoint() const;

  /** \details
    Sets the center of this arc, and returns a reference to this arc.

    \param center [in]  Center of arc.
  */
  OdGeCircArc3d& setCenter(
    const OdGePoint3d& center);

  /** \details
    Sets the normal and reference vectors for this arc. Returns a reference
    to this arc.

    \param normal [in]  A vector normal to the plane of the arc.
    \param refVec [in]  The reference vector defining arc angle 0.
  */
  OdGeCircArc3d& setAxes(
    const OdGeVector3d& normal,
    const OdGeVector3d& refVec);

  /** \details
    Sets the radius of this arc, and returns a reference to this arc.

    \param radius [in]  Radius of arc.
  */
  OdGeCircArc3d& setRadius(
    double radius);

  /** \details
    Sets the starting and ending angles of this arc, and returns a reference to
    this arc.

    \param startAng [in]  Starting angle of arc.
    \param endAng   [in]  Ending angle of arc.
    \remarks
    All angles are expressed in radians.
    In common case parameter (paramOf, evalPoint, evaluate) interval is NOT
    equal to [startAng; endAng]! But its length is equal to endAng - startAng.
  */
  OdGeCircArc3d& setAngles(
    double startAng,
    double endAng);

  /** \details
    Sets the parameters for this arc according to the arguments, and returns a
    reference to this arc.

    \param center [in]  Center of the arc.
    \param normal [in]  Vector normal to the plane of the arc.
    \param radius [in]  Radius of the arc.
  */
  OdGeCircArc3d& set(
    const OdGePoint3d& center,
    const OdGeVector3d& normal,
    double radius);

  /** \details
    Sets the parameters for this arc according to the arguments, and returns a
    reference to this arc.

    \param center   [in]  Center of the arc.
    \param normal   [in]  Vector normal to the plane of the arc.
    \param radius   [in]  Radius of the arc.
    \param startAng [in]  Starting angle of the arc.
    \param endAng   [in]  Ending angle of the arc.
    \param refVec   [in]  Reference vector defining arc angle 0.

    \remarks
    To construct a circle, set endAng = startAng + Oda2PI.

    \remarks
    All angles are expressed in radians.
    In common case parameter (paramOf, evalPoint, evaluate) interval is NOT
    equal to [startAng; endAng]! But its length is equal to endAng - startAng.
    startAng must be less than endAng.
  */
  OdGeCircArc3d& set(
    const OdGePoint3d& center,
    const OdGeVector3d& normal,
    const OdGeVector3d& refVec,
    double radius,
    double startAng,
    double endAng);

  /** \details
    Sets the parameters for this arc according to the arguments, and returns a
    reference to this arc.

    \param startPoint  [in]  Startpoint of the arc.
    \param secondPoint [in]  Second point on a 3-point arc.
    \param endPoint    [in]  Endpoint of the arc.
  */
  OdGeCircArc3d& set(
    const OdGePoint3d& startPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& endPoint);

  /** \details
    Sets the parameters for this arc according to the arguments, and returns a
    reference to this arc.

    \param startPoint  [in]  Startpoint of the arc.
    \param secondPoint [in]  Second point on a 3-point arc.
    \param endPoint    [in]  Endpoint of the arc.
    \param status     [out]  Receives status of set().
  */
  OdGeCircArc3d& set(
    const OdGePoint3d& startPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& endPoint,
    OdGeError& status);

  /** \details
    Sets the parameters for this arc according to the arguments, and returns a
    reference to this arc.

    \param radius   [in]  Radius of the arc.
    \param curve1   [in]  First curve to define a tangent arc.
    \param curve2   [in]  Second curve to define a tangent arc.
    \param param1   [in]  Parameter corresponding tangency point on curve1.
    \param param2   [in]  Parameter corresponding tangency point on curve2.
    \param success [out]  Receives true if and only if the tan-tan-radius or
                          tan-tan-tan curve could be constructed. If false,
                          this arc is unmodified.
  */
  OdGeCircArc3d& set(
    const OdGeCurve3d& curve1,
    const OdGeCurve3d& curve2,
    double radius,
    double& param1,
    double& param2,
    bool& success);

  /** \details
    Sets the parameters for this arc according to the arguments, and returns a
    reference to this arc.

    \param curve1   [in]  First curve to define a tangent arc.
    \param curve2   [in]  Second curve to define a tangent arc.
    \param curve3   [in]  Third curve to define a tangent arc.
    \param param1   [in]  Parameter corresponding tangency point on curve1.
    \param param2   [in]  Parameter corresponding tangency point on curve2.
    \param param3   [in]  Parameter corresponding tangency point on curve3.
    \param success [out]  Receives true if and only if the tan-tan-radius or
                          tan-tan-tan curve could be constructed. If false,
                          this arc is unmodified.
  */
  OdGeCircArc3d& set(
    const OdGeCurve3d& curve1,
    const OdGeCurve3d& curve2,
    const OdGeCurve3d& curve3,
    double& param1,
    double& param2,
    double& param3,
    bool& success);

  /** \details
    Assignment operator for objects of the OdGeCircArc3d class.
    \param arc [in]  Input 3D arc.

    \remarks
    Assigns input arc to this arc.
  */
  OdGeCircArc3d& operator =(
    const OdGeCircArc3d& arc);

  //////////////////////////////////////////////////////////////////////////

  /** \details
    Returns the geometric extents of this arc.

    \param extents [out]  Receives the geometric extents.
  */
  void getGeomExtents(
    OdGeExtents3d& extents) const;

  /** \details
    Attaches the specified curve to this arc.

    \param curve      [in] Circle arc to join.
    \param iTolerance [in] Tolerance for comparisons.
    \returns Reference to the recomputed circular arc.

    \remarks
    Curves should not overlap. The circles should have the same center, radius
    and co-directional normals. the starting point of the join curve should
    coincide with the ending point of this curve. Otherwise it will raise
    eInvalidInput.
    Angle between the corresponding axes can be arbitrary.
    If the end of the passed arc coincides with the beginning of this arc, the
    result will be a closed curve.
  */
  OdGeCircArc3d& joinWith(
    const OdGeCircArc3d& curve,
    const OdGeTol &iTolerance = OdGeContext::gTol);
};

#include "TD_PackPop.h"

#endif
