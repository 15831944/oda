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

#ifndef OD_GEELL3D_H
#define OD_GEELL3D_H /*!DOM*/

class OdGeEllipArc2d;
class OdGeCircArc3d;
class OdGeLineEnt3d;
class OdGePlanarEnt;
class OdGeExtents3d;

#include "Ge/GeCurve3d.h"
#include "OdPlatformSettings.h"

#include "TD_PackPush.h"

/** \details
  This class represents 3D elliptical arcs and full ellipses in 3D space.

  \remarks
  The angle of a point on an ellipse is measured by projecting the point along a
  vector perpendicular to the major axis onto a circle whose center is the
  center of this ellipse and whose radius is the major radius of this ellipse.
  The angle between the major axis of the ellipse and a vector from the center
  of the ellipse to the intersection point with the circle, measured
  counterclockwise about the crossproduct of the major and minor axes, is the
  angle of the point on the ellipse.

  Corresponding C++ library: TD_Ge

  <group OdGe_Classes>

  \sa
  <link ge_OdGeEllipArc3d, Working with Elliptical Arcs>
*/
class GE_TOOLKIT_EXPORT OdGeEllipArc3d : public OdGeCurve3d
{
public:

  /** \details
    Default constructor for objects of the OdGeEllipArc3d class. Constructs an
    elliptical arc in the XY plane (normal vector (0.0, 0.0, 1.0)) with center
    (0.0, 0.0, 0.0), major and minor radii set to 1.0, major axis set to
    (1.0, 0.0, 0.0) and minor axis set to (0.0, 1.0, 0.0).
  */
  OdGeEllipArc3d();

  /** \details
    Copy constructor for objects of the OdGeEllipArc3d class.

    \param ell [in]  Input elliptical arc to be copied.
  */
  OdGeEllipArc3d(
    const OdGeEllipArc3d& ell);

  /** \details
    Constructor for objects of the OdGeEllipArc3d class.

    \param source [in]  Input circular arc to be copied.
  */
  OdGeEllipArc3d(
    const OdGeCircArc3d& source);

  /** \details
    Constructor for the OdGeEllipArc3d class.

    \param center      [in]  Center of the elliptical arc.
    \param majorAxis   [in]  Major axis of the elliptical arc.
    \param minorAxis   [in]  Minor axis of the elliptical arc.
    \param majorRadius [in]  Major radius of the elliptical arc.
    \param minorRadius [in]  Minor radius of the elliptical arc.
  */
  OdGeEllipArc3d(
    const OdGePoint3d& center,
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis,
    double majorRadius,
    double minorRadius);

  /** \details
    Constructor for objects of the OdGeEllipArc3d class.

    \param center      [in]  Center of the elliptical arc.
    \param majorAxis   [in]  Major axis of the elliptical arc.
    \param minorAxis   [in]  Minor axis of the elliptical arc.
    \param majorRadius [in]  Major radius of the elliptical arc.
    \param minorRadius [in]  Minor radius of the elliptical arc.
    \param startAng    [in]  Start angle of the elliptical arc.
    \param endAng      [in]  End angle of the elliptical arc.
  */
  OdGeEllipArc3d(
    const OdGePoint3d& center,
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis,
    double majorRadius,
    double minorRadius,
    double startAng,
    double endAng);

  /** \details
    Returns the point on the elliptical arc that is closest to the specified
    plane and the point on the specified plane that is closest to the elliptical
    arc.

    \param plane         [in]  A plane to which a closest point is calculated.
    \param pointOnPlane [out]  Receives the closest point on the plane.
    \param tol           [in]  Geometric tolerance.

    \returns
    An instance of the OdGePoint3d class.
  */
  OdGePoint3d closestPointToPlane(
    const OdGePlanarEnt& plane,
    OdGePoint3d& pointOnPlane,
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Returns true if the specified linear entity intersects the arc entity and
    returns the number of intersections and points of the intersection.

    \param line    [in]  A 3D linear entity with which the intersection is calculated.
    \param numInt [out]  Receives the number of intersections.
    \param p1     [out]  Receives the first intersection point of the elliptical arc.
    \param p2     [out]  Receives the second intersection point of the elliptical arc.
    \param tol     [in]  Geometric tolerance.

    \returns
    A boolean value that indicates whether a line intersects with the arc.

    \remarks
    * p1 is valid only if numInt > 0.
    * p2 is valid only if numInt > 1.
  */
  bool intersectWith(
    const OdGeLinearEnt3d& line,
    int& numInt,
    OdGePoint3d& p1,
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Returns true if the specifed plane or linear entity intersects the
    elliptical arc entity and returns the number of intersections and points of
    intersection.

    \param plane   [in]  A plane entity with which the intersection is calculated.
    \param numInt [out]  Receives the number of intersections.
    \param p1     [out]  Receives the first intersection point on the elliptical arc.
    \param p2     [out]  Receives the second intersection point on the elliptical arc.
    \param tol     [in]  Geometric tolerance.

    \returns
    A boolean value that indicates whether a plane intersects with an arc.

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
    Returns true if the projected points of the elliptical arc intersect with
    the specified linear entity and returns the number of intersections and
    points of intersection.

    \param line           [in]  A 3D linear entity with which the intersection is calculated.
    \param projDir        [in]  Projection direction.
    \param numInt        [out]  Receives the number of intersections.
    \param pntOnEllipse1 [out]  Receives the first intersection point on the elliptical arc.
    \param pntOnEllipse2 [out]  Receives the second intersection point on the elliptical arc.
    \param pntOnLine1    [out]  Receives the first intersection point on the line.
    \param pntOnLine2    [out]  Receives the second intersection point on the line.
    \param tol            [in]  Geometric tolerance.

    \returns
    A boolean value that indicates whether projected points of the elliptical
    arc intersect with a given line.
  */
  bool projIntersectWith(
    const OdGeLinearEnt3d& line,
    const OdGeVector3d& projDir,
    int& numInt,
    OdGePoint3d& pntOnEllipse1,
    OdGePoint3d& pntOnEllipse2,
    OdGePoint3d& pntOnLine1,
    OdGePoint3d& pntOnLine2,
    const OdGeTol& tol = OdGeContext::gTol) const ;

  /** \details
    Receives the plane of the ellipse.

    \param plane [out]  A plane on which elliptical arc resides.
  */
  void getPlane(
    OdGePlane& plane) const;

  /** \details
    Returns true if and only if the major and minor radii of the ellipse are the
    same within the specified tolerance.

    \param tol [in]  Geometric tolerance.

    \returns
    A boolean value that indicates whether a given arc is circular.
  */
  bool isCircular(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified point lies within the full
    ellipse, and on the plane of the ellipse.

    \param point [in]  A 3D point to be tested whether it lies on the elliptical arc.
    \param tol   [in]  Geometric tolerance.

    \returns
    A boolean value that indicates whether a given point is inside the full
    ellipse.
  */
  bool isInside(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the center of this elliptical arc.

    \returns
    An instance of the OdGePoint3d class.
  */
  OdGePoint3d center() const;

  /** \details
    Returns the minor radius of this elliptical arc.

    \returns
    A double value that represents a minor radius.
  */
  double minorRadius() const;

  /** \details
    Returns the major radius of this elliptical arc.

    \returns
    A double value that represents a major radius.
  */
  double majorRadius() const;

  /** \details
    Returns the minor axis of this elliptical arc as a unit vector.

    \returns
    An instance of the OdGeVector3d class.
  */
  OdGeVector3d minorAxis() const;

  /** \details
    Returns the major axis of this elliptical arc as a unit vector.

    \returns
    An instance of the OdGeVector3d class.
  */
  OdGeVector3d majorAxis() const;

  /** \details
    Returns the normal to this elliptical arc as a unit vector.

    \returns
    An instance of the OdGeVector3d class.

    \remarks
    Positive angles are always drawn counterclockwise about this vector.
  */
  OdGeVector3d normal() const;

  /** \details
    Returns the start angle measured from the major axis of this elliptical arc
    in the plane of the arc.

    \returns
    A double value that represents a start angle.
    \remarks
    All angles are expressed in radians.
    \remarks
    The result of this method should not be used as a curve parameter.
    The angles and its corresponding parameter may differ.
  */
  double startAng() const;

  /** \details
    Returns the end angle measured from the major axis of this elliptical arc in
    the plane of the arc.

    \returns
    A double value that represents an end angle.
    \remarks
    All angles are expressed in radians.
    \remarks
    The result of this method should not be used as a curve parameter.
    The angles and its corresponding parameter may differ.
  */
  double endAng() const;

  /** \details
    Returns the start point of this elliptical arc.

    \returns
    An instance of the OdGePoint3d class.
  */
  OdGePoint3d startPoint() const;

  /** \details
    Returns the end point of this elliptical arc.

    \returns
    An instance of the OdGePoint3d class.
  */
  OdGePoint3d endPoint() const;

  /** \details
    Sets the center of the elliptical arc.

    \param center [in]  Center of the elliptical arc.

    \returns
    Reference to this OdGeEllipArc3d object.
  */
  OdGeEllipArc3d& setCenter(
    const OdGePoint3d& center);

  /** \details
    Sets the minor radius of the elliptical arc.

    \param rad [in]  Minor radius of the elliptical arc.

    \returns
    Reference to this OdGeEllipArc3d object.
  */
  OdGeEllipArc3d& setMinorRadius(
    double rad);

  /** \details
    Sets the major radius of the elliptical arc.

    \param rad [in]  Major radius of the elliptical arc.

    \returns
    Reference to this OdGeEllipArc3d object.
  */
  OdGeEllipArc3d& setMajorRadius(
    double rad);

  /** \details
    Sets the major and minor axes of the elliptical arc.

    \param majorAxis [in]  Major axis of the elliptical arc.
    \param minorAxis [in]  Minor axis of the elliptical arc.

    \returns
    Reference to this OdGeEllipArc3d object.
  */
  OdGeEllipArc3d& setAxes(
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis);

  /** \details
    Sets the starting and ending angles of the elliptical arc.

    \param startAng [in]  Starting angle of the elliptical arc.
    \param endAng   [in]  Ending angle of the elliptical arc.

    \returns
    Reference to this OdGeEllipArc3d object.

    \remarks
    All angles are expressed in radians.
  */
  OdGeEllipArc3d& setAngles(
    double startAng,
    double endAng);

  /** \details
    Sets the parameters for this elliptical arc according to the arguments and
    returns a reference to this elliptical arc.

    \param center      [in]  Center of the elliptical arc.
    \param majorAxis   [in]  Major axis of the elliptical arc.
    \param minorAxis   [in]  Minor axis of the elliptical arc.
    \param majorRadius [in]  Major radius of the elliptical arc.
    \param minorRadius [in]  Minor radius of the elliptical arc.

    \returns
    Reference to this OdGeEllipArc3d object.
  */
  OdGeEllipArc3d& set(
    const OdGePoint3d& center,
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis,
    double majorRadius,
    double minorRadius);

  /** \details
    Sets the parameters for this elliptical arc according to the arguments and
    returns a reference to this elliptical arc.

    \param center      [in]  Center of the elliptical arc.
    \param majorAxis   [in]  Major axis of the elliptical arc.
    \param minorAxis   [in]  Minor axis of the elliptical arc.
    \param majorRadius [in]  Major radius of the elliptical arc.
    \param minorRadius [in]  Minor radius of the elliptical arc.
    \param startAng    [in]  Starting angle of the elliptical arc.
    \param endAng      [in]  Ending angle of the elliptical arc.

    \returns
    Reference to this OdGeEllipArc3d object.

    \remarks
    All angles are expressed in radians.
  */
  OdGeEllipArc3d& set(
    const OdGePoint3d& center,
    const OdGeVector3d& majorAxis,
    const OdGeVector3d& minorAxis,
    double majorRadius,
    double minorRadius,
    double startAng,
    double endAng);

  /** \details
    Sets the parameters for this elliptical arc according to the arguments and
    returns a reference to this elliptical arc.

    \param arc [in]  A 3D circular arc with the parameters to be copied.

    \returns
    Reference to this OdGeEllipArc3d object.
  */
  OdGeEllipArc3d& set(
    const OdGeCircArc3d& arc);

  /** \details
    Assignment operator for the OdGeEllipArc3d object.

    \param ell [in]  Elliptical arc to assign.
    \returns
    Reference to this OdGeEllipArc3d object.
  */
  OdGeEllipArc3d& operator =(
    const OdGeEllipArc3d& ell);

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  // functions making ellipse's axis orthogonal
  // this one requires error flag

  /** \details
    Makes the minor axis orthogonal to the major axis.

    \param tol   [in]  Geometric tolerance.
    \param flag [out]  Receives the status of computation.

    \remarks
    Possible values for flag are as follows:

    * kOk
    * kDegenerateGeometry
    
    If flag is not specified, the following exception may be thrown:

    Throws:
    <table>
    Exception
    eDegenerateGeometry
    </table>
  */
  void orthogonolizeAxes(
    const OdGeTol& tol,
    OdGe::ErrorCondition& flag);

  /** \details
    Makes the minor axis orthogonal to the major axis.

    \param tol [in]  Geometric tolerance.

    \remarks
    The following exception may be thrown:

    Throws:
    <table>
    Exception
    eDegenerateGeometry
    </table>
  */
  void orthogonolizeAxes(
    const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Returns the tangent vector at the point specified by param.

    \param param [in]  Parameter corresponding to the point at which to compute the tangent.
  */
  OdGeVector3d tangentAt(
    double param) const;

  /** \details
    Returns the geometric extents of this elliptical arc.

    \param extents [out]  Receives the geometric extents.
  */
  void getGeomExtents(
    OdGeExtents3d& extents) const;

  /** \details
    Returns the parameters of the points (if any) on the elliptical arc, whose
    tangents are parallel to the specified tangent vector.

    \param tan     [in] A vector defining the tangent direction.
    \param params [out] Receives the array of tangent point parameters.

    \remarks
    The parameters are appended to the specified array. You may wish to
    initialize the params array with clear().
  */
  OdResult inverseTangent(
    const OdGeVector3d& tan,
    OdGeDoubleArray& params) const;

  /** \details
    Returns the parameters of the points (if any) on the elliptical arc, whose
    tangents are parallel to the specified reference plane.

    \param refPlane [in] A reference plane.
    \param params  [out] Receives the array of tangent point parameters.

    \remarks
    The parameters are appended to the specified array. You may wish to
    initialize the params array with clear().
  */
  OdResult inverseTangentPlane(
    const OdGePlane& refPlane,
    OdGeDoubleArray& params) const;

  /** \details
    Attaches the specified curve to this elliptical arc.

    \param curve      [in] Elliptical arc to join.
    \param iTolerance [in] Tolerance for comparisons.
    \returns Reference to the recomputed elliptical arc.

    \remarks
    Curves should not overlap. Elliptical arcs should have the same center,
    co-directional normals and the corresponding radii equal. The starting point
    of the join curve should coincide with the end point of this curve.
    If these conditions are not met, this method raises the eInvalidInput error.
    \remarks
    If the ellipses are circles (isCircular(tol)), then the angle between the
    corresponding axes can be arbitrary.
    In general case, only the n * OdaPI angle is allowed.
    If the end of the passed arc coincides with the beginning of this arc,
    the result will be a closed curve.
  */
  OdGeEllipArc3d& joinWith(
    const OdGeEllipArc3d& curve,
    const OdGeTol &iTolerance = OdGeContext::gTol);
};

#include "TD_PackPop.h"

#endif // OD_GEELL3D_H
