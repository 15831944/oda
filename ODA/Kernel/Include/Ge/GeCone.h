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

#ifndef OD_GECONE_H
#define OD_GECONE_H /*!DOM*/

#include "OdPlatformSettings.h"
#include "Ge/GeSurface.h"
#include "Ge/GeInterval.h"

class OdGePoint3d;
class OdGeVector3d;
class OdGeCircArc3d;
class OdGeLinearEnt3d;

#include "TD_PackPush.h"

/** \details
    This class represents right circular cones.

    \remarks
    A right circular cone is defined by its 

    * axis of symmetry
    * height
    * sine and cosine of the half angle
    * origin (a point on the axis of symmetry)
    * radius at the origin
    * U parameter scale.

    The cross section containing the origin is referred to as the base.
    The radius at the base cannot be zero.  
    This cone may extend on either side of the base.
    This cone extends only on one direction from the apex,
    i.e. it is not a double cone.

    The half angle is measured between the symmetric axis
    and the generating line of this cone.
    If cosine of the half angle is positive, this cone is convex.
    If cosine of the half angle is negative, this cone is concave.
    If cosine and sine of the half angle have the same sign,
    the apex is on the opposite direction of the symmetric axis
    and radius increases along the symmetric axis.
    If cosine and sine of the half angle have different signs,
    the apex is on the direction of the symmetric axis
    and radius decreases along the symmetric axis.
    Both cosine and sine of the half angle can't be zeros.

    The height interval of this cone is specified relative to its origin.
    Height increases in the direction of the symmetric axis.
    Height interval must be bounded.

    Parameter V is the angle of revolution measured from the refAxis
    (an axis perpendicular to the axis of symmetry). For a closed cone,
    it defaults to [-OdaPI, OdaPI). The right hand rule applied along the
    direction of the axis of symmetry defines the positive direction
    of V. The surface is periodic in V with a period of Oda2PI.

    Parameter U varies along the generating line.
    U is dimensionless, measured in U parameter scale (which equals to the radius by default),
    multiplied by the absolute value of cosine of the half angle.
    U increases in the direction of the axis of symmetry.
    However, if U parameter scale is negative, U decreases in the direction of the axis of symmetry.
    U = 0 corresponds to the base of this cone.

    If cosine and sine of the half angle have the same sign,
    U and height are less than zero at the apex.
    If cosine and sine of the half angle have different signs,
    U and height are greater than zero at the apex.

    [umin, umax] x [vmin, vmax] defines a four sided conical patch
    bounded by two straight lines (at angles vmin and vmax), and two
    circular arcs (at umin and umax). The following must be observed
    when defining a cone:

    * umin < umax
    * |vmax - vmin| <= Oda2PI.
    * baseRadius > 0.0
    
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_OdGeCone, Working with Cones>
*/
class GE_TOOLKIT_EXPORT OdGeCone : public OdGeSurface
{
public:
  /** \details
    Default constructor for the OdGeCone class.

    \remarks
    The default constructor uses a half angle of 45°, a base origin of (0.0, 0.0, 0.0),
    a base radius of 2.0, an axis of symmetry of (0.0, 1.0, 0.0), and a reference axis of (1.0, 0.0, 0.0).
  */
  OdGeCone();

  /** \details
    Constructor for the OdGeCone class.

    \param cosineAngle [in]  The cosine of the angle between the generating line and the axis of symmetry.
    \param sineAngle [in]  The sine of the angle between the generating line and the axis of symmetry.
    \param baseOrigin [in]  Center of the base.
    \param baseRadius [in]  Radius of the base.
    \param axisOfSymmetry [in]  Axis of symmetry (rotation).
  */
  OdGeCone(
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& baseOrigin, 
    double baseRadius,
    const  OdGeVector3d& axisOfSymmetry);

  /** \details
    Constructor for the OdGeCone class.

    \param cosineAngle [in]  The cosine of the angle between the generating line and the axis of symmetry.
    \param sineAngle [in]  The sine of the angle between the generating line and the axis of symmetry.
    \param baseOrigin [in]  Center of the base.
    \param baseRadius [in]  Radius of the base.
    \param axisOfSymmetry [in]  Axis of symmetry (rotation).
    \param refAxis [in]  Defines angle 0 of circular base.
    \param height [in]  Height interval of the cone.
    \param startAng [in]  Start angle.
    \param endAng [in]  End angle.

    \remarks
    All angles are expressed in radians.
  */
  OdGeCone(
    double cosineAngle, 
    double sineAngle,
    const  OdGePoint3d& baseOrigin, 
    double baseRadius,
    const  OdGeVector3d& axisOfSymmetry,
    const  OdGeVector3d& refAxis, 
    const  OdGeInterval& height,
    double startAng, 
    double endAng);

  /** \details
    Copy constructor for the OdGeCone class.

    \param cone [in]  Cone to be copied.
  */
  OdGeCone(const OdGeCone& cone);

  /** \details
    Returns the base radius of this cone.

    \returns
    The base radius of this cone as a double value.
  */
  double baseRadius() const;

  /** \details
    Returns the center of the base.

    \returns
    The center of the base of this cone as an OdGePoint3d instance.
  */
  OdGePoint3d baseCenter() const;

  /** \details
    Retrieves the starting and ending angles of this cone.

    \param startAng [out]  Receives the start angle.
    \param endAng [out]  Receives the end angle.

    \remarks
    All angles are expressed in radians.
  */
  void getAngles(
    double& startAng, 
    double& endAng) const;

  /** \details
    Returns the angle between the generating line and the axis of symmetry.

    \returns
    The half angle of this cone as a double value.

    \remarks
    Absolute value is returned (i.e. any signs are dropped).
    The angle is expressed in radians.
  */
  double halfAngle() const;

  /** \details
    Retrieves the cosine and the sine of the angle between the generating line and the axis of symmetry.

    \remarks
    Absolute value is returned for both functions (i.e. any signs are dropped).

    \param cosineAngle [out]  Receives the cosine of the angle.
    \param sineAngle [out]  Receives the sine of the angle.
  */
  void getHalfAngle(
    double& cosineAngle, 
    double& sineAngle) const;

  /** \details
    Retrieves the cosine and the sine of the angle between the generating line and the axis of symmetry.

    \remarks
    The angle is returned "as is", the signs are preserved.

    \param cosineAngle [out]  Receives the cosine of the angle.
    \param sineAngle [out]  Receives the sine of the angle.
  */
  void getHalfAngleSigned(
    double& cosineAngle, 
    double& sineAngle) const;

  /** \details
    Retrieves the interval of the axis of symmetry.

    \param height [out]  Receives the interval.
  */
  void getHeight(
    OdGeInterval& height) const;

  /** \details
    Returns the height of this cone corresponding to
    the specified position on the U-axis.

    \remarks
    Parameter U varies along the generating line. U is dimensionless,
    measured in radius of the origin, and increases
    in the direction of the axis of symmetry. 

    \param u [in]  Position on the U-axis of this cone.

    \returns
    The height as a double value.
  */
  double heightAt(
    double u) const;

  /** \details
    Returns the axis of symmetry of this cone.

    \returns
    The axis of symmetry of this cone as an OdGeVector3d instance.
  */
  OdGeVector3d axisOfSymmetry() const;

  /** \details
    Returns the reference axis of this cone.

    \returns
    The reference axis of this cone as an OdGeVector3d instance.
  */
  OdGeVector3d refAxis() const;

  /** \details
    Returns the apex of this cone.

    \returns
    The apex of this cone as an OdGePoint3d instance.
  */
  OdGePoint3d apex() const;

  /** \details
    Checks if the base of this cone is a full circle
    within the specified tolerance.

    \returns
    true if and only if the base of this cone is a full circle
    within the specified tolerance, false otherwise.

    \param tol [in]  Geometric tolerance.
  */
  bool isClosed(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Checks if the normal to this cone
    is pointing outward.

    \returns
    true if and only if the normal to this cone
    is pointing outward, false otherwise.
  */
  bool isOuterNormal() const;

  /** \details
    Sets the base radius of this cone.

    \returns
    A reference to this OdGeCone object.

    \param baseRadius [in]  Radius of the base. 
  */
  OdGeCone& setBaseRadius(
    double baseRadius);

  /** \details
    Sets the starting and ending angles of this cone.

    \param startAng [in]  Start angle.
    \param endAng [in]  End angle.

    \returns
    A reference to this OdGeCone object.

    \remarks
    All angles are expressed in radians.
  */
  OdGeCone& setAngles(
    double startAng, 
    double endAng);

  /** \details
    Sets the height interval of this cone.

    \param height [in]  Height interval of this cone.

    \returns
    A reference to this OdGeCone object.
  */
  OdGeCone& setHeight(
    const OdGeInterval& height);

  /** \details
    Returns U parameter at the pole

    \returns
    U parameter at the pole of this cone as a double value.
  */
  double getPoleParam() const;

  /** \details
    Sets the parameters for this cone according to the arguments, 
    and returns a reference to this cone.

    \param cosineAngle [in]  The cosine of the angle between the generating line and the axis of symmetry.
    \param sineAngle [in]  The sine of the angle between the generating line and the axis of symmetry.
    \param baseCenter [in]  Center of the base.
    \param baseRadius [in]  Radius of the base. 
    \param axisOfSymmetry [in]  Axis of symmetry (rotation).

    \returns
    A reference to this OdGeCone object.
  
    \remarks
    All angles are expressed in radians.
  */
  OdGeCone& set(
    double cosineAngle, 
    double sineAngle,
    const OdGePoint3d& baseCenter,
    double baseRadius,
    const OdGeVector3d& axisOfSymmetry);
 
  /** \details
    Sets the parameters for this cone according to the arguments, 
    and returns a reference to this cone.

    \param cosineAngle [in]  The cosine of the angle between the generating line and the axis of symmetry.
    \param sineAngle [in]  The sine of the angle between the generating line and the axis of symmetry.
    \param baseCenter [in]  Center of the base.
    \param baseRadius [in]  Radius of the base. 
    \param axisOfSymmetry [in]  Axis of symmetry (rotation).
    \param refAxis [in]  Defines angle 0 of circular base.
    \param height [in]  Height interval of this cone.
    \param startAng [in]  Start angle.
    \param endAng [in]  End angle.

    \returns
    A reference to this OdGeCone object.
    
    \remarks
    All angles are expressed in radians.
  */
  OdGeCone& set(
    double cosineAngle, 
    double sineAngle,
    const OdGePoint3d& baseCenter,
    double baseRadius,
    const OdGeVector3d& axisOfSymmetry,
    const OdGeVector3d& refAxis,
    const OdGeInterval& height,
    double startAng, 
    double endAng);

  /** \details
    Assignment operator for the OdGeCone object.

    \param cone [in]  Cone to be assigned.

    \returns
    A reference to this OdGeCone object.
  */
  OdGeCone& operator =(const OdGeCone& cone);

  /** \details
    Checks if this cone intersects with
    a linear entity, and returns the number of intersections and the
    points of intersection.

    \param linEnt [in]  Linear entity.
    \param numInt [out]  Receives the number of intersections.
    \param p1 [out]  Receives the first intersection point.
    \param p2 [out]  Receives the second intersection point.
    \param tol [in]  Geometric tolerance.
    
    \returns
    true if and only if this cone intersects with
    a line entity, false otherwise.

    \remarks
    * p1 is valid if and only if numInt >= 1.
    * p2 is valid if and only if numInt = 2.
  */
  bool intersectWith(
    const OdGeLinearEnt3d& linEnt, 
    int& numInt,
    OdGePoint3d& p1,
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Retrieves U parameter scale.

    \returns
    U parameter scale of this cone as a double value.
  */
  double getUParamScale() const;

  /** \details
    Sets U parameter scale according to the parameter.

    \param uScale [in]  U parameter scale.

    \remarks
    U parameter scale can be negative.
    If uScale is zero, U parameter scale will be set equal to radius.
  */
  void setUParamScale(
    double uScale = 0.0);
};

#include "TD_PackPop.h"

#endif // OD_GECONE_H

