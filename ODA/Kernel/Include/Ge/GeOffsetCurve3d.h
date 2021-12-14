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

#ifndef OD_GEOFFC3D_H
#define OD_GEOFFC3D_H /*!DOM*/

#include "Ge/GeCurve3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents 3D curves that are exact offsets of other curves.

    \remarks
    Every GeOffsetCurve3d contains a pointer to the curve from which it is offset.
    Modifying the base curve modifies the offset curve. Modifying the offset curve
    does not modify the base curve. setInterval() for an offset curve result create a curve that
    is an offset of the specified interval in the base curve.

    This curve may be self-intersecting, even if the base curve is not. To create offset
    curves that are not self-intersecting, use OdGeCurve3d::getTrimmedOffset().

    The base curve must be planar, and a normal to the plane must be specified.
    Positive offset distances at any point on the base curve are defined as 90° counterclockwise
    from the tangent of the base curve at that point.

    An offset curve with a 0.0 offset distance is exact replica of the base curve. An offset curve with
    a non-zero 0.0 offset distance has a continuity of one less than that of the base curve. 
    To insure that the offset curve is a valid curve, the base curve must have a continuity
    of at least 1.

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeOffsetCurve3d : public OdGeCurve3d
{
public:

  /** \details
    Default constructor for the OdGeOffsetCurve3d class.
    Sets the base curve pointer to NULL, the offset distance to 0.0 and the
    normal to the plane is initialized by its default with coordinates (0, 0, 0).
  */
  OdGeOffsetCurve3d();

  /** \details
    Sets the base curve pointer to baseCurve, the plain normal pointer to
    planeNormal, and the offset distance to offsetDistance.

    \param baseCurve      [in] Any 3D curve to be set as the base curve.
    \param planeNormal    [in] A 3D vector to be set as the normal to the plane of the base curve.
    \param offsetDistance [in] Offset distance.
    \param makeCopy       [in] If true, makes a copy of the base curve specified by the baseCurve parameter.

    \remarks
    If makeCopy is true, the constructor makes a copy of the 3D curve entity
    specified by the baseCurve parameter. The OdGeOffsetCurve3d object that is
    being created becomes the owner of the newly made copy of the given 3D
    curve.
    The direction of the offset is determined by the sign of the offsetDistance parameter 
    and by the direction of planeNormal. The base curve must be planar and planeNormal
    must be perpendicular to the plane of the base curve. 

    There are two exceptions to this rule: 

    * If the offset distance is 0, then planeNormal may be any vector, 
    including the zero vector (it will not be used if the offset distance is 0).
    * If the base curve is linear, then planeNormal must be perpendicular to the line of the curve.
  */
  OdGeOffsetCurve3d(
    const OdGeCurve3d& baseCurve, 
    const OdGeVector3d& planeNormal,
    double offsetDistance,
    bool makeCopy = false);

  /** \details
    Creates a copy of the specified OdGeOffsetCurve3d object.

    \param source [in] Object to be copied.
  */
  OdGeOffsetCurve3d(
    const OdGeOffsetCurve3d& source);

  /** \details
    Returns a pointer to the base curve. 
  */
   const OdGeCurve3d* curve() const;

  /** \details
    Returns the normal to the base curve as a unit vector. 
  */
   OdGeVector3d normal() const; 

  /** \details
    Returns the offset distance of this curve.

    \remarks
    Offset distance is a distance by which this curve is offset from the base curve.
    Positive offset distances at any point on the base curve are defined as 90° counterclockwise
    from the tangent of the base curve at that point.
  */
   double offsetDistance() const;

  /** \details
    Returns true if and only if this curve has the same parameter direction as the base curve.

    \remarks
    Currently not implemented.
  */
   bool paramDirection() const;

  /** \details
    Returns the concatenation of the transformation matrices applied to this curve with transformBy(). 

    \remarks
    If no transformation matrices have been applied to this curve, returns the identity matrix.

    \remarks
    Currently not implemented.
  */
   OdGeMatrix3d transformation() const;

  /** \details
    Sets the base curve for this OdGeOffsetCurve3d object.

    \param baseCurve [in] Any 3D curve to be set as the base curve.
    \param makeCopy  [in] If true, makes a copy of the base curve specified by the baseCurve parameter.

    \remarks
      If makeCopy is true, the method makes a copy of the 3D curve entity
      specified by the baseCurve parameter. This OdGeOffsetCurve3d object
      becomes the owner of the newly made copy of the given 3D curve.
  */
  OdGeOffsetCurve3d& setCurve(
    const OdGeCurve3d& baseCurve,
    bool makeCopy = false);

  /** \details
    Sets the normal to the plane of the base curve.

    \param planeNormal [in]  A 3D normal vector to the plane on which base curve lies.
  */
   OdGeOffsetCurve3d& setNormal(
    const OdGeVector3d& planeNormal);

  /** \details
    Sets the offset distance for this curve.

    \param offsetDistance [in]  Distance by which this curve is offset from the base curve.

    \remarks
    Positive offset distances at any point on the base curve are defined as 90° counterclockwise
    from the tangent of the base curve at that point.
    The direction of offset is determined by the sign of the distance parameter.

    An offset curve with a 0.0 offset distance is exact replica of the base curve.
  */
   OdGeOffsetCurve3d& setOffsetDistance(
    double offsetDistance);

  /** \details
    Assignment operator for the OdGeOffsetCurve3d object.

    \param offsetCurve [in]  Offset curve to assign.
    
    \returns
    Reference to this OdGeOffsetCurve3d object.
  */
  OdGeOffsetCurve3d&  operator =(
    const OdGeOffsetCurve3d& offsetCurve);
};

#include "TD_PackPop.h"

#endif // OD_GEOFFC3D_H
