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

#ifndef OD_GESSINT_H
#define OD_GESSINT_H /*!DOM*/

#include "Ge/GeEntity3d.h"

class OdGeCurve3d;
class OdGeCurve2d;
class OdGeSurface;

#include "TD_PackPush.h"

/** \details
    This class holds the intersection data of two surfaces.

    \remarks
    This object contains references to surface objects but does not own them.
    Calculation of the intersection is performed by a query function.

    Any output geometry from an intersection object is owned by the caller.
    The const base objects returned by surface1() and surface2() are not considered output objects.

    If one of the objects is deleted, the intersection must be recalculated.
    If one of the object is NULL, kXXUnknown status is set and zero value is returned.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
    
    \sa
    <link ge_OdGeIntersectEntities, Working with Intersected Entities>
*/
class GE_TOOLKIT_EXPORT OdGeSurfSurfInt  : public OdGeEntity3d
{
public:
  
  /** \details 
    Default constructor for the OdGeSurfSurfInt class. Constructs an invalid surface-surface intersector with no intersection surfaces and default tolerance.
  */
  OdGeSurfSurfInt();

  /** \details 
    Constructor. Creates an OdGeSurfSurfInt object with specified two surfaces and geometric tolerance.
    
    \param srf1 [in]  First surface that intersects with the second surface.
    \param srf2 [in]  Second surface that intersects with the first surface.
    \param tol [in]  Geometric tolerance.
    
    \remarks
    Constructs the objects with the specified parameters.
  */
  OdGeSurfSurfInt(
    const OdGeSurface& srf1,
    const OdGeSurface& srf2,
    const OdGeTol& tol = OdGeContext::gTol);
  
  /** \details 
    Copy constructor. Creates an OdGeSurfSurf object and initializes it with parameters of the specified source object.
    
    \param source [in]  A surface intersection object to be copied.
  */
  OdGeSurfSurfInt(
    const OdGeSurfSurfInt& source);

  /** \details
    Returns a constant pointer to the first surface of intersection.

    \returns
    An pointer to OdGeSurface
  */
  const OdGeSurface* surface1() const;

  /** \details
    Returns a constant pointer to the second surface of intersection.

    \returns
    An pointer to OdGeSurface
  */
  const OdGeSurface* surface2() const;

  /** \details
    Returns the tolerance for determining intersections.

    \returns
    An instance of OdGeTol
  */
  OdGeTol tolerance() const;

  /** \details
    Returns the number of intersections between the two surfaces,
    and the status of the intersections.

    \param status [out]  Receives the status of the intersection.

    \returns
    The number of intersections as an integer value.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXUnknown
  */
  int numResults(
    OdGeIntersectError& status) const;

  /** \details
    Returns the number of intersections between the surfaces.

    \param status [out]  Receives the status of the intersection.

    \returns
    The number of intersections as an integer value.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXUnknown
  */
  int numIntPoints(
    OdGeIntersectError& status) const;

  /** \details
    Returns the 3d curve representing the specified intersection
    and the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param isExternal [in]  If true, returns a pointer of OdGeExternalCurve3d (not implemented).
    \param status [out]  Receives the status of the intersection.

    \returns
    A pointer to OdGeCurve3d.

    \remarks
    If the intersection is not a 3d curve, returns NULL.
    If some intersection is queried more than once, returns NULL and sets status to kXXUnknown.

    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  OdGeCurve3d* intCurve(
    int intNum, 
    bool isExternal, 
    OdGeIntersectError& status) const; 

  /** \details
    Returns the 2d curve representing the specified intersection
    and the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param isExternal [in]  If true, returns a pointer of OdGeExternalCurve2d (not implemented).
    \param isFirst [in]  If true, returns the curve on the first surface, otherwise it the curve on the second surface.
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
    bool isFirst, 
    OdGeIntersectError& status) const;

  /** \details
    Returns the 3d point representing the specified intersection,
    and the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param status [out]  Receives the status of the intersection.

    \returns
    An instance of OdGePoint3d.

    \remarks
    If the intersection is not a 3d point, returns (0.0, 0.0, 0.0) point.

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
    Retrieves the parameter pairs for the specified intersection point with respect to each surface,
    and the status of the intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param param1 [out]  Receives the parameter pair of the intersection point with respect to the first curve.
    \param param2 [out]  Receives the parameter pair of the intersection point with respect to the second curve.
    \param status [out]  Receives the status of the intersection.

    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  void getIntPointParams(
    int intNum,
    OdGePoint2d& param1, 
    OdGePoint2d& param2, 
    OdGeIntersectError& status) const;

  /** \details
    Retrieves the configurations on either side of the intersection each surface.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param surf1Left [out]  Receives the configuration for surface 1 on the left side of the intersection.
    \param surf1Right [out]  Receives the configuration for surface 1 on the right side of the intersection.
    \param surf2Left [out]  Receives the configuration for surface 2 on the left side of the intersection.
    \param surf2Right [out]  Receives the configuration for surface 2 on the right side of the intersection.
    \param intType [out]  Receives the intersection type.
    \param dim [out]  Receives the dimension of the intersection.
    \param status [out]  Receives the status of the intersection.
    
    \remarks
    Possible values for surf1Left, surf1Right, surf2Left, and surf2Right are as follows:

    @untitled table
    kSSIUnknown
    kSSIOut            Neighborhood is outside this surface.
    kSSIIn             Neighborhood is inside this surface.
    kSSICoincident     Non-zero area intersection.

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
    OdGe::ssiConfig& surf1Left,  
    OdGe::ssiConfig& surf1Right,
    OdGe::ssiConfig& surf2Left,  
    OdGe::ssiConfig& surf2Right,  
    OdGe::ssiType& intType, 
    int& dim, 
    OdGeIntersectError& status) const;

  /** \details
    Returns the number of intersection curves for the two surfaces

    \param status [out]  Receives the status of the intersection.

    \returns
    The number of intervals as an integer value.

    \remarks
    Possible values for status are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  int numIntCurves(
    OdGeIntersectError& status) const;

  /** \details
    Returns the dimension of the intersection point by its index.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param status [out]  Receives the status of the intersection.

    \returns
    A dimension of the intersection as an integer number.

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
    Returns the type of the specified intersection.

    \param intNum [in]  The zero-based index of the intersection curve to return.
    \param status [out]  Receives the status of the intersection.

    \returns
    An instance of OdGe::ssiType.

    \remarks
    Possible return values for the configuratons are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown

    \remarks
    Currently not implemented.
  */
  OdGe::ssiType getType(
    int intNum, 
    OdGeIntersectError& status) const;

  /** \details
    Checks if the surfaces have an overlap.

    \param status [out]  Receives the status of the intersection.

    \returns
    true if and only if the surfaces have an overlap, false otherwise.

    \remarks
    Possible return values for the configuratons are as follows:

    @untitled table
    kXXOk
    kXXIndexOutOfRange
    kXXWrongDimensionAtIndex
    kXXUnknown
  */
  bool haveOverlap(
    OdGeIntersectError& status) const;

  /** \details
    Sets the surfaces and tolerances to be used for determining intersection data.

    \returns
    Reference to this OdGeSurfSurfInt object.

    \param srf1 [in]  First surface to be intersected.
    \param srf2 [in]  Second surface to be intersected.
    \param tol [in]  Geometric tolerance.
  */
  OdGeSurfSurfInt& set(
    const OdGeSurface& srf1,
    const OdGeSurface& srf2,
    const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Assignment operator for the OdGeSurfSurfInt object.
    
    \param surfSurfInt [in]  OdGeSurfSurfInt object to assign.
    \returns
    Reference to this OdGeSurfSurfInt object.
  */
  OdGeSurfSurfInt& operator =(
    const OdGeSurfSurfInt& surfSurfInt);

  //////////////////////////////////////////////////////////////////////////

};

#include "TD_PackPop.h"

#endif // OD_GESSINT_H
