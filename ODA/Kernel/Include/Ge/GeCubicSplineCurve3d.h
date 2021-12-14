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

#ifndef OD_GECSPL3D_H
#define OD_GECSPL3D_H /*!DOM*/

#include "Ge/GeSplineEnt3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents 3D interpolation cubic spline curves.
    Currently not implemented.

    Corresponding C++ library: TD_Ge

   <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeCubicSplineCurve3d : public OdGeSplineEnt3d
{
public:

  /** \details
    Default constructor for the OdGeCubicSplineCurve3d class. Constructs an empty cubic spline curve.
  */
  OdGeCubicSplineCurve3d();

  /** \details
    Copy constructor for the OdGeCubicSplineCurve3d class. Copies the specified cubic spline curve.
    Currently not implemented.
    
    \param source [in]  Object to copy.
  */
  OdGeCubicSplineCurve3d(
    const OdGeCubicSplineCurve3d& source);

  /** \details
    Constructor for the OdGeCubicSplineCurve3d class. Constructs a
    periodic (closed) cubic spline curve. The last fit point must equal to the first one.
    Currently not implemented.

    \param fitPnts  [in]  An array of 3D fit points.
    \param tol      [in]  Geometric tolerance.
  */
  OdGeCubicSplineCurve3d(
    const OdGePoint3dArray& fitPnts,
    const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Constructor for the OdGeCubicSplineCurve3d class. Constructs a
    cubic spline curve based on fit points, starting and ending derivatives of
    the cubic spline curve.
    Currently not implemented.

    \param fitPnts      [in]  An array of 3D fit points.
    \param startDeriv   [in]  Starting derivative of the cubic spline curve.
    \param endDeriv     [in]  Ending derivative of this cubic spline curve.
    \param tol          [in]  Geometric tolerance.
  */
  OdGeCubicSplineCurve3d(
    const OdGePoint3dArray& fitPnts,
    const OdGeVector3d& startDeriv,
    const OdGeVector3d& endDeriv,
    const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Constructor for objects of the OdGeCubicSplineCurve3d class. 
    Constructs a cubic spline curve from the passed curve that is approximated according to the specified approximation precision.
    Currently not implemented.

    \param curve    [in]  A curve to be approximated by this cubic spline curve.
    \param epsilon  [in]  Approximation precision.
  */
  OdGeCubicSplineCurve3d(
    const OdGeCurve3d& curve,
    double epsilon 
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    );

  /** \details
    Constructor for objects of the OdGeCubicSplineCurve3d class.  Constructs a
    cubic spline curve based on fit points, knot vectors and first derivatives
    at the fit points and option that specifies whether cubic spline curve should be closed.

    \param fitPnts      [in]  An array of 3D fit points.
    \param knots        [in]  Knot vector.
    \param isPeriodic   [in]  True if the cubic spline curve should be periodic (closed).
    \param firstDerivs  [in]  Array of first derivatives at the fit points.

    \remarks
    Currently not implemented.
  */
  OdGeCubicSplineCurve3d(
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& fitPnts,
    const OdGeVector3dArray& firstDerivs,
    bool isPeriodic = false);

  /** \details
    Returns the number of fit points.

    \returns Number of fit points.

    \remarks
    Currently not implemented.
  */
   int numFitPoints() const;

  /** \details
    Returns the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.

    \returns Fit point at the specified index.

    \remarks
    Currently not implemented.
  */
   OdGePoint3d fitPointAt(
    int fitPointIndex) const;

  /** \details
    Sets the fit point at the specified index, and returns a reference to this curve.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Any 3D point.

    \returns Reference to this curve.

    \remarks
    Currently not implemented.
  */
   OdGeCubicSplineCurve3d& setFitPointAt(
    int fitPointIndex, 
    const OdGePoint3d& point);

  /** \details
    Returns the first derivative at the specified index.

    \param fitPointIndex [in]  Fit point index.

    \returns First derivative at the specified index.

    \remarks
    Currently not implemented.
  */
   OdGeVector3d firstDerivAt(
    int fitPointIndex) const;

  /** \details
    Sets the first derivative at the specified index, and returns a reference to this curve.

    \param fitPointIndex [in]  Fit point index.
    \param deriv [in]  The first derivative at the fit point.

    \returns Reference to this curve.

    \remarks
    Currently not implemented.
  */
   OdGeCubicSplineCurve3d& setFirstDerivAt(
    int fitPointIndex, 
    const OdGeVector3d& deriv);

  /** \details
    Assignment operator for the OdGeCubicSplineCurve3d class. 
    Assigns properties of the specified cubic spline curve.
    Currently not implemented.

    \param spline [in] Cubic spline curve to assign.
  */
  OdGeCubicSplineCurve3d&  operator =(
    const OdGeCubicSplineCurve3d& spline);
};

#include "TD_PackPop.h"

#endif

