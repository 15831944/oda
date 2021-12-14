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

#ifndef OD_GE_ELLIP_ARC_2D_H
#define OD_GE_ELLIP_ARC_2D_H /*!DOM*/

class OdGeCircArc2d;
class OdGePlanarEnt;
class OdGeEllipArc2d;
class OdGeLinearEnt2d;
class OdGeExtents2d;
#include "OdPlatform.h"
#include "Ge/GeCurve2d.h"

#include "TD_PackPush.h"

/** \details
  This class represents elliptical arcs and full ellipses in 2D space.

  \remarks
  The angle of a point on an ellipse is measured by projecting the point along a
  vector perpendicular to the major axis onto a circle whose center is the
  center of this ellipse and whose radius is the major radius of this ellipse.
  The angle between the major axis of this ellipse, and a vector from the center
  of this ellipse to the intersection point with the circle, measured
  counterclockwise, is the angle of this point on the ellipse.

  Corresponding C++ library: TD_Ge

  <group OdGe_Classes>

  \sa
  <link ge_OdGeEllipArc3d, Working with Elliptical Arcs>
*/
class GE_TOOLKIT_EXPORT OdGeEllipArc2d : public OdGeCurve2d
{
public:

  /** \details
    Default constructor for objects of the OdGeEllipArc2d class. Constructs an
    elliptical arc at coordinates origin (0.0, 0.0) with major and minor radii
    set to 1.0, major axis set to (1.0, 0.0) and minor axis set to (0.0, 1.0).
  */
  OdGeEllipArc2d();

  /** \details
    Copy constructor for objects of the OdGeEllipArc2d class.

    \param ell [in]  Input ellipse to be copied.
  */
  OdGeEllipArc2d(
    const OdGeEllipArc2d& ell);

  /** \details
    Constructor for objects of the OdGeEllipArc2d class.

    \param arc [in]  Input arc to be copied.
  */
  OdGeEllipArc2d(
    const OdGeCircArc2d& arc);

  /** \details
    Constructor for the OdGeEllipArc2d class.

    \param center      [in]  Center of this elliptical arc.
    \param majorAxis   [in]  Major axis of this elliptical arc.
    \param minorAxis   [in]  Minor axis of this elliptical arc.
    \param majorRadius [in]  Major radius of this elliptical arc.
    \param minorRadius [in]  Minor radius of this elliptical arc.
  */
  OdGeEllipArc2d(
    const OdGePoint2d& center,
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis,
    double majorRadius,
    double minorRadius);

  /** \details
    Constructor for objects of the OdGeEllipArc2d class.

    \param center      [in]  Center of this elliptical arc.
    \param majorAxis   [in]  Major axis of this elliptical arc.
    \param minorAxis   [in]  Minor axis of this elliptical arc.
    \param majorRadius [in]  Major radius of this elliptical arc.
    \param minorRadius [in]  Minor radius of this elliptical arc.
    \param startAng    [in]  Start angle of this elliptical arc.
    \param endAng      [in]  End angle of this elliptical arc.
  */
  OdGeEllipArc2d(
    const OdGePoint2d& center,
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis,
    double majorRadius,
    double minorRadius,
    double startAng,
    double endAng);

  /** \details
    Determines whether the input line intersects the ellipse.

    \param line    [in]  A 2D linear entity that is tested for intersection with the ellipse.
    \param numInt [out]  Number of intersection points.
    \param p1     [out]  First intersection point.
    \param p2     [out]  Second intersection point.
    \param tol     [in]  Tolerance for tangent intersections.

    \returns
    A boolean value that indicates whether an intersection occurred.
  */
  bool intersectWith(
    const OdGeLinearEnt2d& line,
    int& numInt,
    OdGePoint2d& p1,
    OdGePoint2d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines whether an ellipse is a circular arc.

    \param tol [in]  Input tolerance.

    \returns
    A boolean value that indicates whether an ellipse is a circular arc.
  */
  bool isCircular(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified point lies within the full
    ellipse.

    \param point [in]  A 2D point to be tested whether it lies on the elliptical arc.
    \param tol   [in]  Geometric tolerance.

    \returns
    A boolean value that indicates whether a given point is inside the full ellipse.
  */
  bool isInside(
    const OdGePoint2d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the center of this elliptical arc.

    \returns
    An instance of the OdGePoint2d class.
  */
  OdGePoint2d center() const;

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
    An instance of the OdGeVector2d class.
  */
  OdGeVector2d minorAxis() const;

  /** \details
    Returns the major axis of this elliptical arc as a unit vector.

    \returns
    An instance of the OdGeVector2d class.
  */
  OdGeVector2d majorAxis() const;

  /** \details
    Returns the start angle measured from the major axis of this elliptical arc.

    \returns
    A double value that represents the start angle.
    \remarks
    All angles are expressed in radians.
    \remarks
    The result of this method should not be used as a curve parameter.
    The angles and its corresponding parameter may differ.
  */
  double startAng() const;

  /** \details
    Returns the end angle measured from the major axis of this elliptical arc.

    \returns
    A double value that represents the end angle.
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
    An instance of the OdGePoint2d class.
  */
  OdGePoint2d startPoint() const;

  /** \details
    Returns the end point of this elliptical arc.

    \returns
    An instance of the OdGePoint2d class.
  */
  OdGePoint2d endPoint() const;

  /** \details
    Returns true if and only if this elliptical arc is drawn clockwise from the
    start point to the end point.
  */
  bool isClockWise() const;

  /** \details
    Sets the center of this elliptical arc.

    \param center [in]  Center point of this elliptical arc.

    \returns
    Reference to this OdGeEllipArc2d object.
  */
  OdGeEllipArc2d& setCenter(
    const OdGePoint2d& center);

  /** \details
    Sets the minor radius of this elliptical arc.

    \param rad [in]  Minor radius of this elliptical arc.

    \returns
    Reference to this OdGeEllipArc2d object.
  */
  OdGeEllipArc2d& setMinorRadius(
    double rad);

  /** \details
    Sets the major radius of this elliptical arc.

    \param rad [in]  Major radius of this elliptical arc.

    \returns
    Reference to this OdGeEllipArc2d object.
  */
  OdGeEllipArc2d& setMajorRadius(
    double rad);

  /** \details
    Sets the major and minor axes of this elliptical arc.

    \param majorAxis [in]  Major axis of this elliptical arc.
    \param minorAxis [in]  Minor axis of this elliptical arc.

    \returns
    Reference to this OdGeEllipArc2d object.
  */
  OdGeEllipArc2d& setAxes(
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis);

  /** \details
    Sets the starting and ending angles of this elliptical arc.

    \param startAng [in]  Starting angle of this elliptical arc.
    \param endAng   [in]  Ending angle of this elliptical arc.

    \returns
    Reference to this OdGeEllipArc2d object.

    \remarks
    All angles are expressed in radians.
  */
  OdGeEllipArc2d& setAngles(
    double startAng,
    double endAng);

  /** \details
    Sets the parameters for this elliptical arc according to the arguments and
    returns a reference to this elliptical arc.

    \param center      [in]  Center of this elliptical arc.
    \param majorAxis   [in]  Major axis of this elliptical arc.
    \param minorAxis   [in]  Minor axis of this elliptical arc.
    \param majorRadius [in]  Major radius of this elliptical arc.
    \param minorRadius [in]  Minor radius of this elliptical arc.

    \returns
    Reference to this OdGeEllipArc2d object.
  */
  OdGeEllipArc2d& set(
    const OdGePoint2d& center,
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis,
    double majorRadius,
    double minorRadius);

  /** \details
    Sets the parameters for this elliptical arc according to the arguments and
    returns a reference to this elliptical arc.

    \param center      [in]  Center of this elliptical arc.
    \param majorAxis   [in]  Major axis of this elliptical arc.
    \param minorAxis   [in]  Minor axis of this elliptical arc.
    \param majorRadius [in]  Major radius of this elliptical arc.
    \param minorRadius [in]  Minor radius of this elliptical arc.
    \param startAng    [in]  Starting angle of this elliptical arc.
    \param endAng      [in]  Ending angle of this elliptical arc.

    \returns
    Reference to this OdGeEllipArc2d object.

    \remarks
    All angles are expressed in radians.
  */
    OdGeEllipArc2d& set(
    const OdGePoint2d& center,
    const OdGeVector2d& majorAxis,
    const OdGeVector2d& minorAxis,
    double majorRadius,
    double minorRadius,
    double startAng,
    double endAng);

  /** \details
    Sets the parameters for this elliptical arc identical to the parameters of
    the specified circular arc and returns a reference to this elliptical arc.

    \param arc [in]  A 2D circular arc with the parameters to be copied.

    \returns
    Reference to this OdGeEllipArc2d object.
  */
  OdGeEllipArc2d& set(
    const OdGeCircArc2d& arc);

  /** \details
    Assignment operator for the OdGeEllipArc2d object.

    \param ell [in]  Elliptical arc to assign.
    \returns
    Reference to this OdGeEllipArc2d object.
  */
  OdGeEllipArc2d& operator =(
    const OdGeEllipArc2d& ell);

  //////////////////////////////////////////////////////////////////////////
  // Overrides :

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
    Returns the geometric extents of this elliptical arc.

    \param extents [out]  Receives the geometric extents.
  */
  void getGeomExtents(
    OdGeExtents2d& extents) const;

  // returns the parameter of elliptical arc points (if any) which have tangent
  // co-directional to "tan" vector. Parameters are returned in params array.
  // Note that this function doesn't clear array - only appends items.

  /** \details
    Returns the parameters of the points (if any) on the elliptical arc, whose
    tangents are parallel to the specified tangent vector.

    \param tan     [in] A vector defining the tangent direction.
    \param params [out] Receives the array of tangent point parameters.

    \remarks
    The parameters are appended to the specified array. You may wish to
    initialize the params array with clear().
  */
  void inverseTangent(
    const OdGeVector2d& tan,
    OdGeDoubleArray& params);
};

#include "TD_PackPop.h"

#endif // OD_GE_ELLIP_ARC_2D_H
