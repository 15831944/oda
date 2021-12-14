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

#ifndef OD_GE_OFFSET_CURVE_2D_H
#define OD_GE_OFFSET_CURVE_2D_H /*!DOM*/

#include "Ge/GeCurve2d.h"

#include "TD_PackPush.h"

/** \details
    This class represents 2D curves that are exact offsets of other curves.

    \remarks
    Every GeOffsetCurve2d contains a pointer to the curve from which it is offset.
    Modifying the base curve modifies the offset curve. Modifying the offset curve
    does not modify the base curve. setInterval() for an offset curve result create a curve that
    is an offset of the specified interval in the base curve.

    This curve may be self-intersecting, even if the base curve is not.

    Positive offset distances at any point on the base curve are defined as 90° counterclockwise
    from the tangent of the base curve at that point.

    An offset curve with a 0.0 offset distance is exact replica of the base curve. An offset curve with
    a non-zero 0.0 offset distance has a continuity of one less than that of the base curve. 
    To insure that the offset curve is a valid curve, the base curve must have a continuity
    of at least 1.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeOffsetCurve3d, Working with Offset Curves>
*/
class GE_TOOLKIT_EXPORT OdGeOffsetCurve2d : public OdGeCurve2d
{
public:

  /** \details
    Default constructor for the OdGeOffsetCurve2d class.
    Sets the base curve pointer to NULL, and the offset distance to 0.0.
  */
  OdGeOffsetCurve2d();

  /** \details
    Sets the base curve pointer to baseCurve, and the offset distance to offsetDistance.

    \param baseCurve      [in] Any 2D curve to be set as the base curve.
    \param offsetDistance [in] Offset distance.
    \param makeCopy       [in] If true, makes a copy of the base curve specified by the baseCurve parameter.

    \remarks
      If makeCopy is true, the constructor makes a copy of the 2D curve entity
      specified by the baseCurve parameter. The OdGeOffsetCurve2d object that is
      being created becomes the owner of the newly made copy of the given 2D
      curve.
      The direction of the offset is determined by the sign of the offsetDistance parameter.
  */
  OdGeOffsetCurve2d(
    const OdGeCurve2d& baseCurve, 
    double offsetDistance,
    bool makeCopy =  false);

  /** \details
    Creates a copy of the specified OdGeOffsetCurve2d object.

    \param source [in] Object to be copied.
  */
  OdGeOffsetCurve2d(
    const OdGeOffsetCurve2d& source);

  /** \details
    Returns the pointer to the base curve. 
  */
  const OdGeCurve2d* curve() const;

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
  OdGeMatrix2d transformation() const;

  /** \details
    Sets the base curve for this OdGeOffsetCurve2d object.

    \param baseCurve [in] Any 2D curve to be set as the base curve.
    \param makeCopy  [in] If true, makes a copy of the base curve specified by the baseCurve parameter.

    \remarks
      If makeCopy is true, the method makes a copy of the 2D curve entity
      specified by the baseCurve parameter. This OdGeOffsetCurve2d object
      becomes the owner of the newly made copy of the given 2D curve.
  */
  OdGeOffsetCurve2d& setCurve(
    const OdGeCurve2d& baseCurve,
    bool makeCopy =  false);

  /** \details
    Sets the offset distance for this curve.

    \param distance [in]  Distance by which this curve is offset from the base curve.

    \remarks
    Positive offset distances at any point on the base curve are defined as 90° counterclockwise
    from the tangent of the base curve at that point.
    The direction of offset is determined by the sign of the distance parameter.

    An offset curve with a 0.0 offset distance is exact replica of the base curve.
  */
  OdGeOffsetCurve2d& setOffsetDistance(
    double distance);

  /** \details
    Assignment operator for the OdGeOffsetCurve2d object.

    \param offsetCurve [in]  Offset curve to assign.
    
    \returns
    Reference to this OdGeOffsetCurve2d object.
  */
  OdGeOffsetCurve2d& operator = (
    const OdGeOffsetCurve2d& offsetCurve);

 
  //////////////////////////////////////////////////////////////////////////
  // TD Special overrides :
};

#include "TD_PackPop.h"

#endif // OD_GE_OFFSET_CURVE_2D_H
