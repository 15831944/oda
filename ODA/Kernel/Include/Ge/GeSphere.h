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

#ifndef OD_GESPHERE_H
#define OD_GESPHERE_H /*!DOM*/

#include "Ge/GeSurface.h"
class OdGeCircArc3d;

#include "TD_PackPush.h"

/** \details
    This class represents spheres.  

    \remarks
    A sphere is defined by its
    * radius
    * center
    * northAxis
    * refAxis

    northAxis defines the direction from the center to the north pole. 

    refAxis, a vector orthogonal to northAxis, the prime meridian.

    Latitude is defined by the U parameter, which defaults to 
    [-OdaPI/2, OdaPI/2] for closed spheres. The lower bound maps to the south
    pole, zero maps to the equator, and the upper bound maps to the north pole.

    Longitude is defined by the V parameter, which defaults to 
    [-OdaPI, OdaPI) for closed spheres. Zero corresponds to the meridian defined by
    the refAxis of this sphere. Applying the right
    hand rule along the northAxis defines the increasing direction for V.
    However, if isReverseV() is true, the increasing direction for V is defined by the left hand rule.

    The sphere is periodic in V with a period of Oda2PI.

    [umin, umax] by [vmin, vmax] defines a spherical patch with 4 sides
    bounded by 2 longitudinal arcs and 2 latitudinal arcs.  

    The following constraints apply when defining a patch.

    * umin < umax and |umin - umax| <= OdaPI.
    * vmin < vmax and |vmin - vmax| <= Oda2PI.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeSphere, Working with Spheress>
*/
class GE_TOOLKIT_EXPORT OdGeSphere : public OdGeSurface
{
public:
  
  /** \details 
    Default constructor for the OdGeSphere class.
    
    \remarks
    Constructs a default sphere that is located at the origin of world coordinates and with 1.0 radius.
  */
  OdGeSphere();
  
  /** \details 
    Constructor for the OdGeSphere class.
    \param radius [in]  The *radius* of this sphere.
    \param center [in]  The origin of the this sphere.
  */
  OdGeSphere(
    double radius, 
    const OdGePoint3d& center);
  
  /** \details 
    Constructor for the OdGeSphere class.
    \param radius [in] The *radius* of this sphere.
    \param center [in]  The origin of the this sphere.
    \param northAxis [in]  the *direction* to the north pole.
    \param refAxis [in] the *direction* to the prime meridian.
    \param startAngleU [in] Starting longitude which is defined by a U parameter. To create a closed sphere, set this parameter to -OdaPI/2 and endAngleU to OdaPI/2.
    \param endAngleU [in] Ending longitude which is defined by a U parameter. To create a closed sphere, set this parameter to OdaPI/2 and startAngleU to -OdaPI/2.
    \param startAngleV [in] Starting latitude which is defined by a V parameter. To create a closed sphere, set this parameter to -OdaPI and endAngleV to OdaPI.
    \param endAngleV [in] Ending latitude which is defined by a V parameter. To create a closed sphere, set this parameter to -OdaPI and endAngleV to OdaPI.

    \remarks
    All angles are expressed in radians.
  */
  OdGeSphere(
    double radius, 
    const OdGePoint3d& center,
    const OdGeVector3d& northAxis, 
    const OdGeVector3d& refAxis,
    double startAngleU, 
    double endAngleU, 
    double startAngleV, 
    double endAngleV);
  /** \details
    Copy constructor for the OdGeSphere class.

    \param sphere [in]  Sphere to be copied.
  */
  OdGeSphere(const OdGeSphere& sphere);

  /** \details
    Returns the radius of this sphere.

    \returns
    Radius of this sphere as a double value.
  */
  double radius() const;

  /** \details
    Returns the center of this sphere.

    \returns
    The center of this sphere as an OdGePoint3d instance.
  */
  OdGePoint3d center() const;

  /** \details
    Retrieves the start and end longitude.

    \param startAngleU [out]  Receives the start longitude.
    \param endAngleU [out]  Receives the end longitude.
  */
  void getAnglesInU(
    double& startAngleU, 
    double& endAngleU) const;

  /** \details
    Retrieves the start and end latitude.

    \param startAngleV [out]  Receives the start latitude.
    \param endAngleV [out]  Receives the end latitude.
  */
  void getAnglesInV(
    double& startAngleV, 
    double& endAngleV) const;

  /** \details
    Returns the direction to the north pole of this sphere.

    \returns
    The direction to the north pole of this sphere as an OdGeVector3d instance.
  */
  OdGeVector3d northAxis() const;

  /** \details
    Returns the reference axis of this sphere.

    \returns
    The reference axis of this sphere as an OdGeVector3d instance.
  */
  OdGeVector3d refAxis() const;

  /** \details
    Returns the location of the north pole.

    \returns
    The north pole of this sphere as an OdGePoint3d instance.
  */
  OdGePoint3d northPole() const;

  /** \details
    Returns the location of the south pole.

    \returns
    The south pole of this sphere as an OdGePoint3d instance.
  */
  OdGePoint3d southPole() const;

  /** \details
    Checks if the normal to this surface
    is pointing outward.

    \returns
    true if and only if the normal to this surface
    is pointing outward.
  */
  bool isOuterNormal() const;

  /** \details
    Checks if the equator is a full circle.

    \param tol [in]  Geometric tolerance.

    \returns
    true if and only if the equator is full circle.
  */
  bool isClosed(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Sets the radius of this sphere.

    \param radius [in]  The radius of this sphere.

    \returns
    A reference to this OdGeSphere object.
  */
  OdGeSphere& setRadius(
    double radius);

  /** \details
    Sets the starting and ending longitudes.

    \param startAngleU [in] Starting longitude which is defined by a U parameter.
    To create a closed sphere, set this parameter to -OdaPI/2 and endAngleU to OdaPI/2.
    \param endAngleU [in] Ending longitude which is defined by a U parameter.
    To create a closed sphere, set this parameter to OdaPI/2 and startAngleU to -OdaPI/2.

    \returns
    A reference to this OdGeSphere object.
  */
  OdGeSphere& setAnglesInU(
    double startAngleU, 
    double endAngleU);

  /** \details
    Sets the starting and ending latitudes.

    \param startAngleV [in] Starting latitude which is defined by a V parameter.
    To create a closed sphere, set this parameter to -OdaPI and endAngleV to OdaPI.
    \param endAngleV [in] Ending latitude which is defined by V a parameter.
    To create a closed sphere, set this parameter to -OdaPI and endAngleV to OdaPI.

    \returns
    A reference to this OdGeSphere object.
  */
  OdGeSphere& setAnglesInV(
    double startAngleV, 
    double endAngleV);

  /** \details
    Sets the parameters for this sphere according to the arguments.

    \param radius [in]  The radius of this sphere.
    \param center [in]  The origin of the this sphere.

    \returns
    A reference to this OdGeSphere object.
  */
  OdGeSphere& set(
    double radius, 
    const OdGePoint3d& center);

  /** \details
    Sets the parameters for this sphere according to the arguments.

    \param radius [in]  The radius of this sphere.
    \param center [in]  The origin of the this sphere.
    \param northAxis [in]  the direction to the north pole.
    \param refAxis [in]  the direction to the prime meridian.
    \param startAngleU [in] Starting longitude which is defined by a U parameter.
    To create a closed sphere, set this parameter to -OdaPI/2 and endAngleU to OdaPI/2.
    \param endAngleU [in] Ending longitude which is defined by a U parameter.
    To create a closed sphere, set this parameter to OdaPI/2 and startAngleU to -OdaPI/2.
    \param startAngleV [in] Starting latitude which is defined by a V parameter.
    To create a closed sphere, set this parameter to -OdaPI and endAngleV to OdaPI.
    \param endAngleV [in] Ending latitude which is defined by a V parameter.
    To create a closed sphere, set this parameter to -OdaPI and endAngleV to OdaPI.

    \returns
    A reference to this OdGeSphere object.
  */
  OdGeSphere& set(
    double radius, 
    const OdGePoint3d& center,
    const OdGeVector3d& northAxis,
    const OdGeVector3d& refAxis,
    double startAngleU,
    double endAngleU,
    double startAngleV,
    double endAngleV);

  /** \details
    Assignment operator for the OdGeSphere class.
    
    \param sphere [in]  Sphere to assign.
    \returns
    A reference to this OdGeSphere object.
  */
  OdGeSphere& operator =(const OdGeSphere& sphere);

  /** \details
    Checks if this sphere intersects with
    a line entity, and returns the number of intersections and the
    points of intersection.

    \param lineEnt [in]  Any 3D line entity that is tested for intersection with this sphere.
    \param numInt [out]  Receives the number of intersections.
    \param p1 [out]  Receives the first intersection point.
    \param p2 [out]  Receives the second intersection point.
    \param tol [in]  Geometric tolerance.

    \remarks
    * p1 is valid if and only if numInt >= 1.
    * p2 is valid if and only if numInt = 2.

    \returns
    true if and only if this sphere intersects with
    a line entity.
  */
  bool intersectWith(
    const OdGeLinearEnt3d& lineEnt, 
    int& numInt,
    OdGePoint3d& p1, 
    OdGePoint3d& p2,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Checks whether V parameter is increasing in clockwise direction around northAxis.

    \returns
    true if V parameter is increasing in clockwise direction around northAxis, false otherwise.
  */
  bool isReverseV() const;

  /** \details
    Specifies whether the V parameter should be measured
    clockwise around northAxis, rather than counterclockwise.

    \param isReverseV [in]  Boolean flag that indicates if the V parameter is reversed.
  */
  void setReverseV(
    bool isReverseV);

  //////////////////////////////////////////////////////////////////////////

};

#include "TD_PackPop.h"

#endif // OD_GESPHERE_H

