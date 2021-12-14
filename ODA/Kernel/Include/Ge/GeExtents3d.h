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

#ifndef _ODGEEXTENTS3D_INCLUDED_
#define _ODGEEXTENTS3D_INCLUDED_ /*!DOM*/

#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"
#include "Ge/GeMatrix3d.h"

#include "TD_PackPush.h"

class OdGeExtents2d;

#define INVALIDEXTENTS 1.0e20

/** \details
    This class represents 3D bounding boxes as minimum and maximum 3d points.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeExtents3d
{
public:

  /** \details
    Default constructor for the OdGeExtents3d class.
  */
  OdGeExtents3d();

  /** \details
    Constructor for the OdGeExtents3d class.
    \param min [in]  Minimum point.
    \param max [in]  Maximum point.
  */
  OdGeExtents3d(
    const OdGePoint3d& min, 
    const OdGePoint3d& max);

  /** \details
    The invalid extents.
  */
  GE_STATIC_EXPORT static const OdGeExtents3d kInvalid;

  /** \details
    Returns the minimum point of this OdGeExtents3d object.
    
    \returns
    Reference to minimum point of these extents.
  */
  const OdGePoint3d& minPoint() const;

  /** \details
    Returns the maximum point of this OdGeExtents3d object.
    
    \returns
    Reference to maximum point of these extents.
  */
  const OdGePoint3d& maxPoint() const;

  /** \details
    Returns the vertex of the box depending on index.
    
    \param iIndex [in]  Index of the vertex to return.
    \returns
    Minimum vertex when iIndex = 0, maximum vertex otherwise.
  */
  const OdGePoint3d& operator[] (int iIndex) const {
    return iIndex == 0 ? m_min : m_max;
  }
  
  /** \details
  Returns the reference to the vertex of the box depending on index.

  \param iIndex [in]  Index of the vertex to return.
  
  \returns
  Reference to the minimum vertex when iIndex = 0, reference to the maximum vertex otherwise.
  */
  OdGePoint3d& operator[] (int iIndex) {
    return iIndex == 0 ? m_min : m_max;
  }

  /** \details
    Sets the minimum and maximum points for this OdGeExtents3d object.
    
    \param min [in]  Minimum point.
    \param max [in]  Maximum point.
  */
  void set(
    const OdGePoint3d& min, 
    const OdGePoint3d& max);

  /** \details
    Sets the minimum and maximum points for this OdGeExtents3d object to
    that of the box defined by pt1 and pt2.
    
    \param pt1 [in]  First point that defines the extents.
    \param pt2 [in]  Second point that defines the extents.

    \remarks
    The passed first and second points are only needed to define a box. The points
    themselves do not necessarily need to be minimum and maximum points. The
    coordinates of the passed points are compared and then set to real minimum and maximum points.
  */
  void comparingSet(
    const OdGePoint3d& pt1, 
    const OdGePoint3d& pt2);

  /** \details
    Updates the extents of this OdGeExtents3d object with the specified point.

    \param point [in]  Any 3D point.
  */
  OdGeExtents3d& addPoint(
    const OdGePoint3d& point);

  /** \details
    Updates the extents of this OdGeExtents3d object with the specified point.

    \param points [in]  Array of 3D points that are added to extents.
    
    \returns
    Reference to this OdGeExtents3d object.
  */
  OdGeExtents3d& addPoints(
    const OdGePoint3dArray& points);

  /** \details
    Updates the extents of this OdGeExtents3d object with the specified array of points.

    \param extents [in]  Array of 3D points that are added to these extents.
    
    \returns
    Reference to this OdGeExtents3d object.
  */
  OdGeExtents3d& addExt(
    const OdGeExtents3d& extents);

  /** \details
    Checks whether this OdGeExtents3d object contains valid extents.
    
    \remarks
    Extents are valid if and only if each member of the minimum extents 
    is less than or equal to the corresponding member of maximum extents.
    
    \returns
    true if and only if this OdGeExtents3d object contains valid extents.
  */
  inline bool isValidExtents() const
  {
    return ( (m_max.x >= m_min.x) && (m_max.y >= m_min.y) && (m_max.z >= m_min.z));
  }

  /** \details
    Updates the extents of this OdGeExtents3d object by the specified vector.
    
    \param vect [in]  A 3D vector that defines where new points are located.
  */
    void expandBy(
    const OdGeVector3d& vect);

  /** \details
    Applies the 3D transformation matrix to the extents.

    \param xfm [in]  3D transformation matrix.
  */
  void transformBy(
    const OdGeMatrix3d& xfm);
    
  /** \details
    Checks whether this OdGeExtents3d object contains the specified point.
    
    \param point [in]  A 3D point that is presumably contained inside these extents.
    \param tol [in]  Geometric tolerance.
    
    \returns
    true if and only if this OdGeExtents3d object contains valid extents.
  */
  bool contains(
    const OdGePoint3d& point, const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Checks whether if this OdGeExtents3d object contains the specified extents.
    
    \param extents [in]  3D extents that are tested to be within these extents.
    \param tol [in]  Geometric tolerance.
    
    \returns
    true if and only if this OdGeExtents3d object contains valid extents.
  */
  bool contains(
    const OdGeExtents3d& extents, const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Checks whether the specified extents do not intersect these extents.

    \param extents [in]  Any 3D Extents object.
    \param tol     [in]  Geometric tolerance.
    
    \returns
    true if and only if specified OdGeExtents3d object does not intersect this one.
  */
  bool isDisjoint(
    const OdGeExtents3d& extents, const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Checks whether the specified extents do not intersect these extents.
    
    \param extents [in]  Any 3D Extents object.
    \param tol     [in]  Geometric tolerance.
    
    \returns
    true if and only if specified OdGeExtents3d object does not intersect this one.
    
    \remarks
    This method is the same as isDisjoint, but checks precisely whether minimal Euclidean distance is greater than tolerance.
  */
  bool isDisjointEuclidean(
    const OdGeExtents3d& extents, const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Calculates distance to the specified point.
    
    \param iPoint [in]  Point to find distance to.
    
    \returns
    0.0 if point is inside the box or euclidean distance otherwise.
  */
  double distanceTo(const OdGePoint3d& iPoint) const;

  /** \details
  Types of intersection results.
  */
  enum IntersectionStatus
  {
    /** Either or both extents are invalid.*/
    kIntersectUnknown,
    /** Extents are not intersecting.*/
    kIntersectNot,
    /** Operand is completely within these extents.*/
    kIntersectOpIn,
    /** These extents are completely within operand.*/
    kIntersectOpOut,
    /** Extents are intersecting, result is returned.*/
    kIntersectOk
  };

  /** \details
    Determines the intersection of the specified OdGeExtents3d object with this one,
    and returns the resulting intersection box.
    
    \param extents [in]  Any 3D Extents object.
    \param pResult [out]  Receives extents of the intersection.
    
    \remarks
    pResult object should be created by the caller. 
    
    \returns
    Possible return values are as follows:
    
    @untitled table
    kIntersectUnknown   Either or both extents boxes are invalid
    kIntersectNot       The extents boxes are not intersecting
    kIntersectOpIn      The specified Extents object is completely within this one
    kIntersectOpOut     This OdGeExtents3d object is completely within the specified one
    kIntersectOk        The extents boxes are intersecting, and a result is returned
    
  */
  IntersectionStatus intersectWith(const OdGeExtents3d& extents, OdGeExtents3d* pResult = 0) const;
  
  /** \details
   Checks whether the box intersects the sphere with specified center and radius.
   
   \param pt [in]  A 3D point that represents the sphere center.
   \param radius [in]  A radius of the sphere.
   
   \returns
   true if and only if the box intersects the sphere with specified center and radius.
  */
  bool isWithinRange(const OdGePoint3d &pt, double radius);

  /** \details
    Returns a center point of this OdGeExtents3d object.
    
    \returns
    Center point as OdGePoint3d.
  */
  OdGePoint3d center() const
  {
    return m_min + (m_max - m_min) * 0.5;
  }
  
  /** \details
    Returns the vector from minimum to maximum point.
    
    \returns
    Vector from minimal to maximal point.
  */
  OdGeVector3d diagonal() const
  {
    return m_max - m_min;
  }

  /** \details
    Types of 2D planes.
  */
  enum Convert2dPlane
  {
    /**XY plane.*/
    kConvert2dPlaneXY = 0x04,
    /**XZ plane.*/
    kConvert2dPlaneXZ = 0x08,
    /**YX plane.*/
    kConvert2dPlaneYX = 0x01,
    /**YZ plane.*/
    kConvert2dPlaneYZ = 0x09,
    /**ZX plane.*/
    kConvert2dPlaneZX = 0x02,
    /**ZY plane.*/
    kConvert2dPlaneZY = 0x06
  };
  
  /** \details
    Converts this OdGeExtents3d object to a OdGeExtents2d object.
    
    \param extents [out]  Resulting converted 2D extents object.
    \param plane [in]  Plane according to which conversion is calculated.
    
    \remarks
    This method sets minimum and maximum 2D points of this OdGeExtents3d object, eliminating one of the axes.
  */
  void convert2d(OdGeExtents2d &extents, Convert2dPlane plane = kConvert2dPlaneXY) const;
  
  /** \details
    Sets minimum and maximum points for this OdGeExtents3d object from a passed OdGeExtents2d object.
    
    \param extents [in]  2D extents from which conversion is calculated.
    \param plane [in]  Plane on which the OdGeExtents2d object lies.
    
    \remarks
    This method sets 0 to the coordinate on the axis that is absent in the passed OdGeExtents2d object.
  */
  void setFrom2d(const OdGeExtents2d &extents, Convert2dPlane plane = kConvert2dPlaneXY);

  /** \details
    Checks if this OdGeExtents3d object is equal to the passed extents.
    
    \param extents [in]  3D extents to be compared for equality.
    \param tol [in]  Geometric tolerance.
    
    \remarks
    This method compares this object to the passed object by testing the equivalence of their definition points. 
    
    \returns
    true if the points match within the range specified by tolerance. Otherwise, it returns false.
  */
  bool isEqualTo(const OdGeExtents3d& extents, const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Comparison operator for the OdGeExtents3d object.
    
    \param extents [in]  Other extents to compare.
    
    \returns
    A boolean value that indicates whether the input OdGeExtents3d object is identical to these extents object.
  */
  bool operator ==(const OdGeExtents3d& extents) const
  {
    return isEqualTo(extents);
  }
  
  /** \details
    Comparison operator for the OdGeExtents3d object.
    
    \param extents [in]  Other extents to compare.
    
    \returns
    A boolean value that indicates whether the input OdGeExtents3d object is not identical to these extents object.
  */
  bool operator !=(const OdGeExtents3d& extents) const
  {
    return !isEqualTo(extents);
  }
//DOM-IGNORE-BEGIN
protected:
  OdGePoint3d m_min;
  OdGePoint3d m_max;
//DOM-IGNORE-END
};

// Inline implementations

inline OdGeExtents3d::OdGeExtents3d()
  : m_min(INVALIDEXTENTS,  INVALIDEXTENTS,  INVALIDEXTENTS)
  , m_max(-INVALIDEXTENTS, -INVALIDEXTENTS, -INVALIDEXTENTS)
{
}

inline OdGeExtents3d::OdGeExtents3d(const OdGePoint3d& min, const OdGePoint3d& max)
  : m_min(min)
  , m_max(max)
{
}

inline const OdGePoint3d& OdGeExtents3d::minPoint() const
{
  return m_min;
}

inline const OdGePoint3d& OdGeExtents3d::maxPoint() const
{
  return m_max;
}

inline void OdGeExtents3d::set(const OdGePoint3d& min, const OdGePoint3d& max)
{
  //ODA_ASSERT_ONCE(min.x <= max.x);
  //ODA_ASSERT_ONCE(min.y <= max.y);
  //ODA_ASSERT_ONCE(min.z <= max.z);
  m_min = min;
  m_max = max;
}

inline void OdGeExtents3d::comparingSet(const OdGePoint3d& pt1, const OdGePoint3d& pt2)
{
  m_min.x = odmin(pt1.x, pt2.x);
  m_max.x = odmax(pt1.x, pt2.x);
  m_min.y = odmin(pt1.y, pt2.y);
  m_max.y = odmax(pt1.y, pt2.y);
  m_min.z = odmin(pt1.z, pt2.z);
  m_max.z = odmax(pt1.z, pt2.z);
}

inline OdGeExtents3d& OdGeExtents3d::addPoint(const OdGePoint3d& point)
{
  if ((m_max.x < m_min.x) || (m_max.y < m_min.y) || (m_max.z < m_min.z))
  {
    //TODO: it's better to remove this case for better performance!
    m_max = m_min = point;
  }
  else
  {
    m_min.x = odmin(m_min.x, point.x);
    m_max.x = odmax(m_max.x, point.x);
    m_min.y = odmin(m_min.y, point.y);
    m_max.y = odmax(m_max.y, point.y);
    m_min.z = odmin(m_min.z, point.z);
    m_max.z = odmax(m_max.z, point.z);
  }
  return *this;
}

inline OdGeExtents3d& OdGeExtents3d::addExt(const OdGeExtents3d& extents)
{
  ODA_ASSERT(extents.isValidExtents());

#if 0
  addPoint(extents.minPoint());
  addPoint(extents.maxPoint());
#else
  if ((m_max.x < m_min.x) || (m_max.y < m_min.y) || (m_max.z < m_min.z))
  {
    //TODO: it's better to remove this case for better performance!
    set(extents.minPoint(), extents.maxPoint());
  }
  else
  {
    m_min.x = odmin(m_min.x, extents.m_min.x);
    m_max.x = odmax(m_max.x, extents.m_max.x);
    m_min.y = odmin(m_min.y, extents.m_min.y);
    m_max.y = odmax(m_max.y, extents.m_max.y);
    m_min.z = odmin(m_min.z, extents.m_min.z);
    m_max.z = odmax(m_max.z, extents.m_max.z);
  }
#endif
  return *this;
}

inline void OdGeExtents3d::expandBy(const OdGeVector3d& vect)
{
  ODA_ASSERT(isValidExtents());

  OdGePoint3d p1 = m_min, p2 = m_max;
  addPoint(p1 + vect);
  addPoint(p2 + vect);
}    

inline bool OdGeExtents3d::contains(const OdGePoint3d& point, const OdGeTol& tol) const
{
  //ODA_ASSERT(isValidExtents());   //note: empty/invalid box does NOT contain any point

  return ( (point.x + tol.equalPoint()) >= m_min.x
    && (point.y + tol.equalPoint()) >= m_min.y
    && (point.z + tol.equalPoint()) >= m_min.z
    && (point.x - tol.equalPoint()) <= m_max.x
    && (point.y - tol.equalPoint()) <= m_max.y
    && (point.z - tol.equalPoint()) <= m_max.z );
}

inline bool OdGeExtents3d::contains(const OdGeExtents3d& extents, const OdGeTol& tol) const
{
  ODA_ASSERT(isValidExtents());

  return ((extents.m_min.x + tol.equalPoint()) >= m_min.x 
    && (extents.m_min.y + tol.equalPoint()) >= m_min.y 
    && (extents.m_min.z + tol.equalPoint()) >= m_min.z 
    && m_max.x >= (extents.m_max.x - tol.equalPoint()) 
    && m_max.y >= (extents.m_max.y - tol.equalPoint()) 
    && m_max.z >= (extents.m_max.z - tol.equalPoint()));
}    

inline bool OdGeExtents3d::isDisjoint(const OdGeExtents3d& extents, const OdGeTol& tol) const
{
  //ODA_ASSERT(isValidExtents());   //note: empty/invalid box is disjoint with everything

  return ((extents.m_min.x - tol.equalPoint()) > m_max.x 
    || (extents.m_min.y - tol.equalPoint()) > m_max.y 
    || (extents.m_min.z - tol.equalPoint()) > m_max.z
    || m_min.x > (extents.m_max.x + tol.equalPoint())
    || m_min.y > (extents.m_max.y + tol.equalPoint())
    || m_min.z > (extents.m_max.z + tol.equalPoint()));
}    

#undef INVALIDEXTENTS

#include "TD_PackPop.h"

#endif //_ODGEEXTENTS3D_INCLUDED_
