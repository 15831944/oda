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

#ifndef OD_GEAPLN3D_H
#define OD_GEAPLN3D_H  /*!DOM*/

#include "Ge/GePolyline3d.h"
#include "Ge/GePoint3d.h"
#include "Ge/GeVector3dArray.h"
#include "Ge/GeKnotVector.h"

#include "TD_PackPush.h"

/** \details
    This class represents mathematical entities used to support various types of spline curves in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeAugPolyline3d : public OdGePolyline3d
{
public:


  /** \details
    Default constructor for objects of the OdGeAugPolyline3d class. Constructs an empty polyline and sets the default approximation tolerance for evaluating point equality.
  */
  OdGeAugPolyline3d();

  /** \details
    Constructor for objects of the OdGeAugPolyline3d class. Creates a clone of
    an existing object of the same class.

    \param apline [in]  Object to be cloned.
  */
  OdGeAugPolyline3d(
    const OdGeAugPolyline3d& apline);

  /** \details
    Constructor for objects of the OdGeAugPolyline3d class. Uses knots, control
    points and bundle vectors to create a spline curve.

    \param knots [in]  Knot vector.
    \param controlPoints [in]  Array of control points.
    \param vecBundle [in]  Array of bundle vectors.
  */
  OdGeAugPolyline3d(
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& controlPoints,
    const OdGeVector3dArray& vecBundle);

  /** \details
    Constructor for objects of the OdGeAugPolyline3d class. Uses control points
    and bundle vectors to create a spline curve.

    \param controlPoints [in]  Control point array.
    \param vecBundle [in]  Vector array. 
  */
  OdGeAugPolyline3d(
    const OdGePoint3dArray& controlPoints,
    const OdGeVector3dArray& vecBundle);

  /** \details
    Constructor for objects of the OdGeAugPolyline3d class. Uses a reference
    curve to create a spline curve. 

    \param curve [in]  Any 3D curve.
    \param fromParam [in]  Starting parameter value.
    \param toParam [in]  Ending parameter value.
    \param approxEps [in]  Approximate spacing along a curve.
  */
  OdGeAugPolyline3d(
    const OdGeCurve3d& curve,
    double fromParam, 
    double toParam, 
    double approxEps);

  /** \details
    This is the assignment operator for objects of the OdGeAugPolyline3d class.
    Assigns the value of the second operand to the first one.
    
    \param apline [in] OdGeAugPolyline3d object to be assigned.
  */
  OdGeAugPolyline3d& operator =(
    const OdGeAugPolyline3d& apline);

  /** \details
    Returns the control point at the specified index.

    \param index [in]  Control point index.

    \returns Control point at the specified index.
  */
  OdGePoint3d getPoint(
    int index) const;

  /** \details
    Sets the control point at the specified index, and returns a reference to this curve.

    \param controlpointIndex [in]  Control point index.
    \param point [in]  Any 3D point.
    
    \returns A reference to this curve.
  */
  OdGeAugPolyline3d& setPoint(
    int controlpointIndex, 
    const OdGePoint3d& point);

  /** \details
    Returns the array of control points.

    \param controlPoints [out]  Receives an array of control points.
  */
  void getPoints(
    OdGePoint3dArray& controlPoints) const;

  /** \details
    Returns the vector at the specified index.

    \param vectorIndex [in]  Vector index.

    \returns The vector at the specified index.

    \remarks
    Currently not implemented.
  */
  OdGeVector3d getVector(
    int vectorIndex) const;

  /** \details
    Sets the vector at the specified index, and returns a reference to this curve.

    \param vectorIndex [in]  Vector index.
    \param vect [in]  Any 3D vector.

    \returns A reference to this curve.

    \remarks
    Currently not implemented.
  */
  OdGeAugPolyline3d& setVector(
    int vectorIndex, 
    const OdGeVector3d& vect);

  /** \details
    Returns an array of the tangents (first derivative vectors) to the curve at each control point.

    \param tangents [out]  Receives an array of tangents.

    \remarks
    Currently not implemented.
  */
  void getD1Vectors(
    OdGeVector3dArray& tangents) const;

  /** \details
    Returns the second derivative vector at the specified index.

    \param vectorIndex [in]  Vector index.

    \returns The second derivative vector at the specified index.

    \remarks
    Currently not implemented.
  */
  OdGeVector3d getD2Vector(
    int vectorIndex) const;

  /** \details
    Sets the second derivative vector at the specified index, and returns a reference to this curve.

    \param vectorIndex [in]  Vector index.
    \param vect [in]  Second derivative vector.

    \returns A reference to this curve.

    \remarks
    Currently not implemented.
  */
  OdGeAugPolyline3d& setD2Vector(
    int vectorIndex, 
    const OdGeVector3d& vect);

  /** \details
    Returns an array of the second derivative vectors to the curve at each control point.

    \param d2Vectors [out] An array of second derivative cectors.

    \remarks
    Currently not implemented.
  */
  void getD2Vectors(
    OdGeVector3dArray& d2Vectors) const;

  /** \details
    Returns the approximate tolerance that was used to construct the polyline.
    
    \returns The approximate tolerance value.
  */
  double approxTol() const;

  /** \details
    Sets the approximate tolerance to be used to construct the polyline, and returns
    a reference to this polyline.
    
    \param approxTol [in] Approximate tolerance.

    \returns A reference to this polyline.

    \remarks
    This method recomputes the polyline.
  */
  OdGeAugPolyline3d& setApproxTol(
    double approxTol);
};

#include "TD_PackPop.h"

#endif // OD_GEAPLN3D_H

