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
#ifndef _GE_UV_BOX_H_
#define _GE_UV_BOX_H_

#include "Ge/GeInterval.h"
#include "Ge/GeExport.h"
#include "Ge/GePoint2d.h"

class OdGeExtents2d;

/** \details
  Represents a box in UV-space of a surface. 

  Corresponding C++ library: TD_Ge
  
  <group !!RECORDS_tkernel_apiref>
*/
struct GE_TOOLKIT_EXPORT OdGeUvBox
{
  /** \details
    U and V intervals.
  */
  OdGeInterval intervals[2];

  /** \details 
    Default constructor for the OdGeUvBox structure.
  */
  OdGeUvBox() {}
  
  /** \details 
    Constructor for the OdGeUvBox structure.
    
    \param iIntervalU [in]  Parametric interval that defines bounds for U parameter.
    \param iIntervalV [in]  Parametric interval that defines bounds for V parameter.
    
    \remarks
    Constructs UV box represented by the specified parametric intervals.
  */
  OdGeUvBox(const OdGeInterval& iIntervalU, const OdGeInterval& iIntervalV)
  {
    intervals[0] = iIntervalU;
    intervals[1] = iIntervalV;
  }

  /** \details 
    Returns a constant reference to an interval that represents bounds of the U parameter.
  */
  const OdGeInterval& u() const { return intervals[0]; }
  
  /** \details 
    Returns a constant reference to an interval that represents bounds of the V parameter.
  */
  const OdGeInterval& v() const { return intervals[1]; }
  
  /** \details 
    Returns an interval that represents bounds of the U parameter.
  */
  OdGeInterval& u() { return intervals[0]; }
  
  /** \details 
    Returns an interval that represents bounds of the V parameter.
  */
  OdGeInterval& v() { return intervals[1]; }

  /** \details 
    Returns a constant reference to an interval either for U or V parameter.
    
    \param idx [in] index that determines which interval (U or V) should be returned.
  */
  const OdGeInterval& operator[](int idx) const { return intervals[idx]; }
  
  /** \details 
    Returns an interval either for U or V parameter.
    
    \param idx [in] index that determines which interval (U or V) should be returned.
  */
  OdGeInterval& operator[](int idx) { return intervals[idx]; }

  /** \details 
    Sets the U and V intervals to have upperBound = lowerBound = 0 and returns a reference to this UV box.
    
    \remarks
    UV box has unbounded intervals.
  */
  OdGeUvBox& set()
  {
    intervals[0].set();
    intervals[1].set();
    return *this;
  }
  
  /** \details 
    Sets the U and V intervals for this UV box.
    
    \param iIntervalU [in] Interval for U parameter to be set for this UV box.
    \param iIntervalV [in] Interval for V parameter to be set for this UV box.
  */
  OdGeUvBox& set(const OdGeInterval& iIntervalU, const OdGeInterval& iIntervalV)
  {
    intervals[0] = iIntervalU;
    intervals[1] = iIntervalV;
    return *this;
  }
  
  /** \details 
    Sets the U and V intervals for this UV box according to the passed 2D points.
    
    \param iLowerBound [in] 2D point that defines the lower bound for U and V parameter of this UV box.
    \param iUpperBound [in] 2D point that defines the upper bound for U and V parameter of this UV box.
    
    \remarks
    * Lower U bound = iLowerBound.x
    * Lower V bound = iLowerBound.y
    * Upper U bound = iUpperBound.x
    * Upper V bound = iUpperBound.y
  */
  OdGeUvBox& set(const OdGePoint2d& iLowerBound, const OdGePoint2d& iUpperBound);

  /** \details 
    Determines whether passed parameter values are inside corresponding intervals.
    
    \param uparam [in] A parameter value that is presumably inside the U interval of this UV box.
    \param vparam [in] A parameter value that is presumably inside the V interval of this UV box.
    
    \remarks
    Returns true only if both passed values lie inside corresponding intervals.
  */
  bool contains(double uparam, double vparam) const;
  
  /** \details 
    Determines whether coordinates of passed 2D point are inside corresponding intervals.
    
    \param uvpoint [in] A 2D point, coordinates of which are tested to be inside corresponding intervals of this UV box.
    
    \remarks
    Returns true only if: (uvpoint.x lies between u.lowerBound and u.upperBound) && (uvpoint.y lies between v.lowerBound and v.upperBound).
  */
  bool contains(const OdGePoint2d& uvpoint) const;

  /** \details 
    Determines if intervals of this UV box are bounded.
    
    \remarks
    Returns true only if both intervals (U and V) are bounded (below and above).
  */
  bool isBounded() const;

  /** \details
    Find finite sub-envelope belonging both to the specified range and this one
    
    \returns
    true if and only if the specified uv-box intersects with this one, 
    and the resulting uv-box is bounded.

    \param range [in]  The uv-box to be intersected.
    \param result [out]  Receives the intersected uv-box.
  */
  bool finiteIntersectWith(const OdGeUvBox& range, OdGeUvBox& result) const;

  /** \details 
    Returns lower bound of both intervals as 2D point.
    
    \remarks
    * pnt.x = u.lowerBound
    * pnt.y = v.lowerBound
  */
  OdGePoint2d lowerBound() const
  {
    return OdGePoint2d(u().lowerBound(), v().lowerBound());
  }
  
  /** \details 
    Returns upper bound of both intervals as 2D point.
    
    \remarks
    * pnt.x = u.upperBound
    * pnt.y = v.upperBound
  */
  OdGePoint2d upperBound() const
  {
    return OdGePoint2d(u().upperBound(), v().upperBound());
  }

  /** \details 
    Compares input UV box to this UV box.
    
    \param uvbox [in]  Input UV box that is compared.
    
    \remarks
    Returns true only if compared UV box has the same parameter intervals for U and V (with equal corresponding bounds).
  */
  bool isEqualTo(const OdGeUvBox& uvbox) const;

  /** \details 
    This operator constructs OdGeExtents2d object from this UV box structure.
    
    \remarks
    Resulting Extents2d properties:
    * min_pnt.x = u.lowerBound
    * min_pnt.y = v.lowerBound
    * max_pnt.x = u.upperBound
    * max_pnt.y = v.upperBound
  */
  operator OdGeExtents2d() const;

  /** \details 
    Evaluates parameter value by the interval ratio for U and V parameters and returns a 2D point with corresponding coordinates.
    
    \param ratioU [in]  A ratio for evaluation the U parameter from the interval.
    \param ratioV [in]  A ratio for evaluation the V parameter from the interval.
    
    \remarks
    Use this method only if parameter intervals are bounded.
    Ratio values range from 0 to 1, where 0 represents the lower bound of interval and 1 represents the upper bound of interval.
  */
  OdGePoint2d eval(double ratioU, double ratioV) const
  {
    OdGePoint2d res(u().eval(ratioU), v().eval(ratioV));
    return res;
  }

  /** \details 
    Returns UV-point inside this UV-box, which is closest to the specified one.
    
    \param point [in]  UV point.
  */
  OdGePoint2d clamp(const OdGePoint2d &point) const
  {
    OdGePoint2d res(u().clamp(point.x), v().clamp(point.y));
    return res;
  }
};

#endif //_GE_UV_BOX_H_
