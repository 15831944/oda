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


#ifndef OD_GEELLIPCONE_H
#define OD_GEELLIPCONE_H /*!DOM*/

#include "Ge/GeSurface.h"
#include "Ge/GeInterval.h"
#include "OdPlatformSettings.h"

class OdGePoint3d;
class OdGeVector3d;
class OdGeCircArc3d;
class OdGeLinearEnt3d;

#include "TD_PackPush.h"

/** \details
    This class represents right elliptical cones.

    \remarks
    A elliptical cone is defined by its
    * axis of symmetry
    * height
    * sine and cosine of the half angle
    * origin (a point on the axis of symmetry)
    * major and minor radii at the origin
    * major axis
    * U parameter scale.

    The cross section containing the origin is referred to as the base.
    The radii at the base cannot be zero.
    This elliptical cone may extend on either side of the base.
    This elliptical cone extends only on one direction from the apex,
    i.e. it is not a double elliptical cone.

    The half angle is measured from the symmetric axis
    of this elliptical cone to a point on the major axis.
    If cosine of the half angle is positive, this elliptical cone is convex.
    If cosine of the half angle is negative, this elliptical cone is concave.
    If cosine and sine of the half angle have the same sign,
    the apex is on the opposite direction of the symmetric axis
    and radius increases along the symmetric axis.
    If cosine and sine of the half angle have different signs,
    the apex is on the direction of the symmetric axis
    and radius decreases along the symmetric axis.
    Both cosine and sine of the half angle can't be zeros.

    Parameter V is the angle of revolution measured from the major axis
    to the axis of symmetry. For a closed elliptical cone,
    it defaults to [-OdaPI, OdaPI). The right hand rule applied along the
    direction of the axis of symmetry defines the positive direction
    of V. The surface is periodic in V with a period of Oda2PI.

    The angle of point on an ellipse is measured by projecting
    the point on the ellipse perpendicular to major axis onto a
    circle which lies on the same plane, which center is the center
    of the ellipse and which radius is the major radius of the ellipse.

    The angle between the major axis of the ellipse, and a vector from
    the center of the ellipse to the intersection point with the circle,
    measured counterclockwise, is the angle of the point on the ellipse.

    The height interval of this elliptical cone is specified relative to its origin.
    Height increases in the direction of the symmetric axis.
    Height interval must be bounded.

    Parameter U varies along the generating line.
    U is dimensionless, measured in U parameter scale (which equals to the major radius by default),
    multiplied by the absolute value of cosine of the half angle.
    U increases in the direction of the axis of symmetry.
    However, if U parameter scale is negative, U decreases in the direction of the axis of symmetry.
    U = 0 corresponds to the base of this elliptical cone.

    If cosine and sine of the half angle have the same sign,
    U and height are less than zero at the apex.
    If cosine and sine of the half angle have different signs,
    U and height are greater than zero at the apex.

    [umin, umax] x [vmin, vmax] defines a four sided conical patch
    bounded by two straight lines (at angles vmin and vmax), and two
    circular arcs (at umin and umax). The following must be observed
    when defining an elliptical cone:

    * umin < umax
    * |vmax - vmin| <= Oda2PI.
    * majorRadius > 0.0
    * minorRadius > 0.0

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeCone, Working with Cones>
*/
class GE_TOOLKIT_EXPORT OdGeEllipCone : public OdGeSurface
{
public:

  /** \details 
    Default constructor for the OdGeEllipCone class. Constructs an elliptical cone that uses a half angle of 45°, 
    a base origin of (0.0, 0.0, 0.0), a base radii of 1.0, an axis of symmetry of (0.0, 1.0, 0.0), a major axis of (1.0, 0.0, 0.0) and minor axis of (0.0, 0.0, -1.0).
  */
  OdGeEllipCone();
  
  /** \details 
    Constructor for the OdGeEllipCone class.
    
    \param cosineAngle    [in]  The cosine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param sineAngle      [in] The sine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param origin         [in] The origin point of this elliptical cone.
    \param majorRadius    [in] The major radius of this elliptical cone.
    \param minorRadius    [in] The minor radius of this elliptical cone.
    \param axisOfSymmetry [in] Axis of symmetry (rotation).
  */
  OdGeEllipCone(
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& origin, 
    double minorRadius,
    double majorRadius, 
    const OdGeVector3d& axisOfSymmetry);
  
  /** \details 
    Constructor for the OdGeEllipCone class.
    
    \param cosineAngle [in]  The cosine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param sineAngle [in] The sine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param baseOrigin [in] The origin of this elliptical cone.
    \param minorRadius [in] The minor radius of this elliptical cone.
    \param majorRadius [in] The major radius of this elliptical cone.
    \param axisOfSymmetry [in] Axis of symmetry (rotation).
    \param majorAxis [in] The major axis of this elliptical cone.
    \param height [in] Height interval of this elliptical cone.
    \param startAng [in] Start angle of this elliptical cone.
    \param endAng [in] End angle of this elliptical cone.
    
    \remarks
    All angles are expressed in radians.
  */
  OdGeEllipCone(
    double cosineAngle, 
    double sineAngle,
    const OdGePoint3d& baseOrigin, 
    double minorRadius,
    double majorRadius,
    const OdGeVector3d& axisOfSymmetry,
    const OdGeVector3d& majorAxis, 
    const OdGeInterval& height,
    double startAng, 
    double endAng);

  /** \details
    Copy constructor for the OdGeEllipCone class.

    \param cone [in]  Cone to be copied.
  */
  OdGeEllipCone(const OdGeEllipCone& cone);

  /** \details
    Returns the ratio of the minor to the major radius of this elliptical cone.

    \returns
    The ratio of radii of this cone as a double value.
  */
  double radiusRatio() const;
  
  /** \details
    Returns the minor radius of this elliptical cone.

    \returns
    The minor radius of this cone as a double value.
  */
  double minorRadius() const;

  /** \details
    Returns the major radius of this elliptical cone.

    \returns
    The major radius of this cone as a double value.
  */
  double majorRadius() const;

  /** \details
    Returns the center of the base.

    \returns
    The center of the base of this cone as an OdGePoint3d instance.
  */
  OdGePoint3d baseCenter() const;

  /** \details
    Retrieves the starting and ending angles of this elliptical cone.

    \param startAng [out]  Receives the start angle.
    \param endAng [out]  Receives the end angle.

    \remarks
    All angles are expressed in radians.
  */
  void getAngles(
    double& startAng, 
    double& endAng) const;

  /** \details
    Returns the angle between the generating line passing through the major axis, and the axis of symmetry.

    \returns
    The half angle of this cone as a double value.
  */
  double halfAngle() const;

  /** \details
    Retrieves the angle between the generating line passing through the major axis, and the axis of symmetry.

    \remarks
    The angle is returned "as is", the signs are preserved.

    \param cosineAngle [out]  Receives the cosine of the angle.
    \param sineAngle [out]  Receives the sine of the angle.
  */
  void getHalfAngle(
    double& cosineAngle, 
    double& sineAngle) const;

  /** \details
    Retrieves the interval of the axis of symmetry.

    \param height [out]  Receives the interval of the axis of symmetry.
  */
  void getHeight(
    OdGeInterval& height) const;

  /** \details
    Returns the height of this elliptical cone corresponding to
    the specified position on the U-axis.

    \remarks
    Parameter U varies along the generating line. U is dimensionless,
    measured in radius of the origin, and increases
    in the direction of the axis of symmetry.

    \param u [in]  Position on the U-axis of this elliptical cone.

    \returns
    The height as a double value.
  */
  double heightAt(
    double u) const;

  /** \details
    Returns the axis of symmetry of this elliptical cone.

    \returns
    The axis of symmetry of this cone as an OdGeVector3d instance.
  */
  OdGeVector3d axisOfSymmetry() const;

  /** \details
    Returns the major axis of this elliptical cone.

    \returns
    The major axis of this cone as an OdGeVector3d instance.
  */
  OdGeVector3d majorAxis() const;

  /** \details
    Returns the minor axis of this elliptical cone.

    \returns
    The minor axis of this cone as an OdGeVector3d instance.
  */
  OdGeVector3d minorAxis() const;

  /** \details
    Returns the apex of this elliptical cone.

    \returns
    The apex of this cone as an OdGePoint3d instance.
  */
  OdGePoint3d apex() const;

  /** \details
    Checks if the base of this elliptical cone
    is a full ellipse within the specified tolerance.

    \param tol [in]  Geometric tolerance.

    \returns
    true if and only if the base of this elliptical cone
    is a full ellipse within the specified tolerance.
  */
  bool isClosed(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Checks if the normal to this surface
    is pointing outward.

    \returns
    true if and only if the normal to this surface
    is pointing outward.
  */
  bool isOuterNormal() const;

  /** \details
    Sets minor radius of this elliptical cone.

    \param minorRadius [in]  The minor radius of this elliptical cone.

    \returns
    A reference to this OdGeEllipCone object.
  */
  OdGeEllipCone& setMinorRadius(
    double minorRadius);

  /** \details
    Sets major radius of this elliptical cone.

    \param majorRadius [in]  The major radius of this elliptical cone.

    \returns
    A reference to this OdGeEllipCone object.
  */
  OdGeEllipCone& setMajorRadius(
    double majorRadius);


  /** \details
    Sets the starting and ending angles of this elliptical cone.

    \param startAng [in]  Start angle in radians.
    \param endAng [in]  End angle in radians.

    \returns
    A reference to this OdGeEllipCone object.
  */
  OdGeEllipCone& setAngles(
    double startAng, 
    double endAng);

  /** \details
    Sets the height interval of this elliptical cone.

    \param height [in]  Height interval of this elliptical cone.

    \returns
    A reference to this OdGeEllipCone object.
  */
  OdGeEllipCone& setHeight(
    const OdGeInterval& height);

  /** \details
    Returns U parameter at the pole

    \returns
    U parameter at the pole of this cone as a double value.
  */
  double getPoleParam() const;

  /** \details
    Checks if the cone intersects with a line entity,
    and returns the number of intersections and the points of intersection.

    \param linEnt [in]  Linear entity.
    \param numInt [out]  Receives the number of intersections.
    \param p1 [out]  Receives the first intersection point.
    \param p2 [out]  Receives the second intersection point.
    \param tol [in]  Geometric tolerance.

    \remarks
    * p1 is valid only if numInt > 0.
    * p2 is valid only if numInt > 1.

    \returns
    true if and only if this cylinder intersects with
    a line entity.
  */
  bool intersectWith(
    const OdGeLinearEnt3d& linEnt,
    int& numInt,
    OdGePoint3d& p1,
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Sets the parameters for this elliptical cone according to the arguments, 
    and returns a reference to this elliptical cone.

    \param cosineAngle [in]  The cosine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param sineAngle [in] The sine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param majorRadius [in] The major radius of this elliptical cone.
    \param minorRadius [in] The minor radius of this elliptical cone.
    \param center [in] The origin of this elliptical cone.
    \param axisOfSymmetry [in] Axis of symmetry (rotation).
        
    \remarks
    All angles are expressed in radians.

    \returns
    A reference to this OdGeEllipCone object.
  */
  OdGeEllipCone& set(
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& center,
    double minorRadius, 
    double majorRadius,
    const  OdGeVector3d& axisOfSymmetry);
  
  /** \details
    Sets the parameters for this elliptical cone according to the arguments, 
    and returns a reference to this elliptical cone.

    \param cosineAngle [in]  The cosine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param sineAngle [in] The sine of the angle between the generating line passing
    through the major axis, and the axis of symmetry.
    \param majorRadius [in] The major radius of this elliptical cone.
    \param minorRadius [in] The minor radius of this elliptical cone.
    \param center [in] The origin of this elliptical cone.
    \param axisOfSymmetry [in] Axis of symmetry (rotation).
    \param majorAxis [in] The major axis of this elliptical cone.
    \param height [in] Height interval of this elliptical cone.
    \param startAng [in] Starting angle of this elliptical cone.
    \param endAng [in] Ending angle of this elliptical cone in.

    \returns
    A reference to this OdGeEllipCone object.
    
    \remarks
    All angles are expressed in radians.
  */
  OdGeEllipCone& set(
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& center,
    double minorRadius, 
    double majorRadius,
    const  OdGeVector3d& axisOfSymmetry,
    const  OdGeVector3d& majorAxis,
    const  OdGeInterval& height,
    double startAng, 
    double endAng);

  /** \details
    Assignment operator for the OdGeEllipCone object.

    \param cone [in]  Elliptical cone to be assigned.

    \returns
    A reference to this OdGeEllipCone object.
  */
  OdGeEllipCone& operator =(const OdGeEllipCone& cone);

  /** \details
    Retrieves U parameter scale.

    \returns
    U parameter scale of this cone as a value of the double type.
  */
  double getUParamScale() const;

  /** \details
    Sets U parameter scale according to the parameter.

    \param uScale [in]  U parameter scale.

    \remarks
    U parameter scale can be negative.
    If uScale is zero, U parameter scale is reset to the major radius of this elliptical cone.
  */
  void setUParamScale(
    double uScale = 0.0);
};

#include "TD_PackPop.h"

#endif // OD_GEELLIPCONE_H
