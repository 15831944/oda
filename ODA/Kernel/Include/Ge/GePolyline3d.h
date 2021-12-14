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

#ifndef OD_GEPLIN3D_H
#define OD_GEPLIN3D_H /*!DOM*/

#include "OdPlatform.h"
#include "Ge/GeSplineEnt3d.h"

#include "TD_PackPush.h"

/** \details
    This class represents piecewise linear splines in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGePolyline3d : public OdGeSplineEnt3d
{
public:

  /** \details
    Default constructor for the OdGePolyline3d class.
  */
  OdGePolyline3d();
  
  /** \details
    Copy constructor for the OdGePolyline3d class.
    
    \param source [in]  Object to be copied.
  */
  OdGePolyline3d(const OdGePolyline3d& source);
  
  /** \details
    Constructor for the OdGePolyline3d class.
    
    \param points [in]  Array of fit points with which a polyline is constructed.

    \remarks
    The constructor constructs a polyline by specified fit points.
  */
  OdGePolyline3d(const OdGePoint3dArray& points);
  
  /** \details
    Constructor for the OdGePolyline3d class.
    
    \param knots [in]  Knot vector.
    \param controlPoints [in]  Array of 3D points with which a polyline is constructed.

    \remarks
    Constructs a polyline with the knots and point values established.
  */
  OdGePolyline3d(const OdGeKnotVector& knots, 
                 const OdGePoint3dArray& controlPoints);
  
  /** \details
    Constructor for the OdGePolyline3d class.
    
    \param crv [in]  Input curve to be approximated by polyline.
    \param approxEps [in]  Input approximation tolerance.

    \remarks
    Constructs a polyline approximating a curve within the specified tolerance.
  */
  OdGePolyline3d(const OdGeCurve3d& crv, 
                 double approxEps);
  // TD Special
  
  /** \details
    Constructor for the OdGePolyline3d class.
    
    \param numPoints [in]  Number of points in the array.
    \param pPoints [in]  A pointer to an array of points with which a polyline is constructed.

    \remarks
    Constructs a polyline with the given points.
  */
  OdGePolyline3d(OdInt32 numPoints, 
                 const OdGePoint3d* pPoints);

  /** \details
    Returns the number of fit points.
  */
   int numFitPoints() const;

  /** \details
    Returns the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
  */
   OdGePoint3d fitPointAt(int fitPointIndex) const;

   /** \details
    Sets the fit point at the specified index, and returns a reference to this curve.

    \param index [in]  An index by which a fit point is set.
    \param point [in]  Any 3D point to be set.
  */
   OdGeSplineEnt3d& setFitPointAt(int index, const OdGePoint3d& point);

  /** \details
    Assignment operator for the OdGePolyline3d object.

    \param polyline [in]  Polyline to assign.
    
    \returns
    Reference to this OdGePolyline3d object.
  */
  OdGePolyline3d& operator =(const OdGePolyline3d& polyline);

  ///////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
  Returns the point on the specified segment corresponding to the specified parameter value.

  \param param [in]  Parameter on specified segment.
  \param numSeg [in]  The segment to be queried.
  */
   OdGePoint3d evalPointSeg(
    double param, 
    int& numSeg) const;

};

#include "TD_PackPop.h"

#endif // OD_GEPLIN3D_H
