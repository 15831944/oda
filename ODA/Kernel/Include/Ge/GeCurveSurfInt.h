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

#ifndef OD_GECSINT_H
#define OD_GECSINT_H /*!DOM*/

#include "Ge/GeEntity3d.h"

#include "TD_PackPush.h"

class OdGeCurve3d;
class OdGeSurface;
class OdGePointOnCurve3d;
class OdGePointOnSurface;

/** \details
    This class represents intersections of 3d curves and surfaces.

    \remarks
    The intersection object links to curve and surface objects, but does not contain them.
    Calculation of the intersection is performed by a query function.

    If one of the objects is deleted, the intersection must be recalculated.
    If one of the objects is NULL, kXXUnknown status is set and the default value is returned.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
    
    \sa
    <link ge_OdGeIntersectEntities, Working with Intersected Entities>
*/
class GE_TOOLKIT_EXPORT OdGeCurveSurfInt : public OdGeEntity3d
{
public:
  /** \details 
    Default constructor for the OdGeCurveSurfInt class. Constructs an invalid curve-surface intersector with no intersection curve and surface and with default tolerance.
  */
  OdGeCurveSurfInt();

  /** \details
    Constructor. Creates an OdGeCurveSurfInt object with the specified curve, surface and geometric tolerance.

    \param curve [in]  Curve.
    \param surf [in]  Surface.
    \param tol [in]  Geometric tolerance.
  */
  OdGeCurveSurfInt(
    const OdGeCurve3d& curve, 
    const OdGeSurface& surf,
    const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Copy constructor. Creates an OdGeCurveSurfInt object and initializes it with parameters of the specified source object.

    \param source [in]  Object to be copied.
  */
  OdGeCurveSurfInt(
    const OdGeCurveSurfInt& source);

  /** \details
    Returns the 3d curve.
    
    \returns
    A pointer to an instance of the OdGeCurve3d class.
  */
  const OdGeCurve3d* curve() const;

  /** \details
    Returns the surface.
    
    \returns
    A pointer to an instance of the OdGeSurface class.
  */
  const OdGeSurface* surface() const;

  /** \details
    Returns the tolerance for determining intersections.

    \returns
    An instance of the OdGeTol class.
  */
  OdGeTol tolerance() const;

  /** \details
    Returns the number of intersections between the curve and the surface,
    and retrieves the status of the intersection.

    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  int numResults(
    OdGeIntersectError& status) const;

  /** \details
    Returns a dimension of the specified intersection,
    and retrieves the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection.
    \param status [out]  Receives the status of the intersection.

    \returns
    A dimension of the specified intersection as an integer number

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  int getDimension(
    int intNum,
    OdGeIntersectError& status) const;

  /** \details
    Returns the number of point intersections between the curve and the surface,
    and retrieves the status of the intersection.

    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  int numIntPoints(
    OdGeIntersectError& status) const;


  /** \details
    Returns a 3d point representing the specified intersection,
    and retrieves the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection.
    \param status [out]  Receives the status of the intersection.

    \returns
    An instance of the OdGePoint3d class

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  OdGePoint3d intPoint(
    int intNum,
    OdGeIntersectError& status) const;

  /** \details
    Retrieves the parameters of the specified intersection point with respect to the curve and surface,
    and retrieves the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param param1 [out]  Receives the parameter of the intersection point with respect to the curve.
    \param param2 [out]  Receives the parameter pair of the intersection point with respect to the surface.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  void getIntParams(
    int intNum,
    double& param1,
    OdGePoint2d& param2,
    OdGeIntersectError& status) const;

  /** \details
    Retrieves the intersection point on the curve, and retrieves the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection pooint to return.
    \param intPnt [out]  Receives the specified intersection point on the curve.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  void getPointOnCurve(
    int intNum,
    OdGePointOnCurve3d& intPnt,
    OdGeIntersectError& status) const;

  /** \details
    Retrieves the intersection point on the surface, and retrieves the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection point to return.
    \param intPnt [out]  Receives the specified intersection point on the surface.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  void getPointOnSurface(
    int intNum,
    OdGePointOnSurface& intPnt,
    OdGeIntersectError& status) const;

  /** \details
    Calculates the configurations on either side of the surface at the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param lower [out]  The configuration record of the lower intersection.
    \param higher [out]  The configuration record of the upper intersection.
    \param smallAngle [out]  true if the intersection is considered to be a small angle.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for lower and higher are as follows:

    @untitled table
    kXUnknown
    kXOut
    kXIn
    kXTanOut
    kXTanIn
    kXCoincident
    kXCoincidentUnbounded

    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
  void getIntConfigs(
    int intNum,
    OdGe::csiConfig& lower,
    OdGe::csiConfig& higher,
    bool& smallAngle,
    OdGeIntersectError& status) const;

  /** \details
    Retrieves the number of interval of overlaps between the curve and the surface,
    and retrieves the status of the intersection.

    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  int overlapCount(
    OdGeIntersectError& status) const;

  /** \details
    Retrieves the range for the curve for the specified overlap,
    and retrieves the status of the intersection.

    \param intNum [in]  The zero-based index of the overlap range to query.
    \param range [out]  Receives the range of the curve for the specified overlap.
    \param status [out]  Receives the status of the intersection.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  void getOverlapRange(
    int intNum,
    OdGeInterval& range,
    OdGeIntersectError& status) const;

  /** \details
    Returns the 2d curve representing the specified intersection
    and the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param isExternal [in]  If true, returns a pointer of OdGeExternalCurve2d (not implemented).
    \param status [out]  Receives the status of the intersection.

    \returns
    A pointer to OdGeCurve2d.

    \remarks
    If the intersection is not a 2d curve, returns NULL.
    If some intersection is queried more than once, returns NULL and sets status to kXXUnknown.

    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  OdGeCurve2d* intParamCurve(
    int intNum,
    bool isExternal,
    OdGeIntersectError& status) const;

  /** \details
    Specifies the 3d curve, surface, and tolerance for determining intersections.

    \param crv      [in]  3d curve that intersects with the specified surface.
    \param surface  [in]  A surface that intersects with the specified curve.
    \param tol      [in]  Geometric tolerance.

    \returns
    Reference to this OdGeCurveSurfInt object.
  */
  OdGeCurveSurfInt& set(
    const OdGeCurve3d& crv,
    const OdGeSurface& surface,
    const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Assignment operator for the OdGeCurveSurfInt object.
    
    \param crvSurfInt [in]  OdGeCurveSurfInt object to assign.
    
    \returns
    Reference to this OdGeCurveSurfInt object.
  */
  OdGeCurveSurfInt& operator =(
    const OdGeCurveSurfInt& crvSurfInt);

  //////////////////////////////////////////////////////////////////////////

};

#include "TD_PackPop.h"

#endif

