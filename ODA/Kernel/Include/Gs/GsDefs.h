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

#ifndef __GSDEFS_H_INCLUDED_
#define __GSDEFS_H_INCLUDED_

#include "TD_PackPush.h"
#include "OdRound.h"
#include "Ge/GePoint2d.h"

//****************************************************************************
// Helper classes
//****************************************************************************

// copied(with modification) from limits.h to avoid extra #includes

#define SCALAR_MIN    (-2147483647 - 1)  // minimum(signed) int32 value
#define SCALAR_MAX      2147483647      // maximum(signed) int32 value

/** \details
    This class represents 2D Device Coordinate points.
    Corresponding C++ library: TD_Gs

    \remarks
    Device coordinates are measured in pixels from the lower-left corner
    of the device.
    
    \sa
    Coordinate Systems.
    
    <group OdGs_Classes> 
*/
class OdGsDCPoint
{
public:
  /** Maximum values for point coordinates. */
  enum MaxFlag { Maximum };
  /** Minimum values for point coordinates. */
  enum MinFlag { Minimum };
  
  /** \details
    Default constructor for the OdGsDCPoint class. Creates an instance with 0 for x and y coordinates.
  */
  OdGsDCPoint() : x(0), y(0) { }
  
  /** \details
    Constructor for the OdGsDCPoint class.
    
    \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.    
  */
  OdGsDCPoint(long xx, long yy) : x(xx), y(yy) { }
  
  /** \details
    Constructor for the OdGsDCPoint class. Builds an instance of the object with the following parameters:
    OdGsDCPoint(Maximum) sets x = SCALAR_MAX, y = SCALAR_MAX
    
    \param MaxFlag [in]  Sets maximum allowed values for x and y axes.    
  */
  OdGsDCPoint(MaxFlag) { x = SCALAR_MAX; y = SCALAR_MAX; }
  
  /** \details
    Constructor for the OdGsDCPoint class. Builds an instance of the object with the following parameters:
    OdGsDCPoint(Minimum) sets x = SCALAR_MIN, y = SCALAR_MIN
    
    \param MinFlag [in]  Sets minimum allowed values for x and y axes.    
  */
  OdGsDCPoint(MinFlag) { x = SCALAR_MIN; y = SCALAR_MIN; }
  
  /** \remarks
    Assignment operator for the OdGsDCPoint class. Assigns the maximum allowed values for x and y coordinates for this object.

    \param MaxFlag [in]  Sets maximum allowed values for x and y axes. 
  */
  void operator=(MaxFlag) { x = SCALAR_MAX; y = SCALAR_MAX; }
  
  /** \remarks
    Assignment operator for the OdGsDCPoint class. Assigns the minimum allowed values for x and y coordinates for this object.

    \param MinFlag [in]  Sets minimum allowed values for x and y axes. 
  */
  void operator=(MinFlag) { x = SCALAR_MIN; y = SCALAR_MIN; }
  
  /** \remarks
    Assignment operator for the OdGsDCPoint class.

    \param dcPoint [in]  2D point in device coordinates to assign. 
  */
  void operator=(const OdGsDCPoint& dcPoint) { x = dcPoint.x; y = dcPoint.y; }
  
  /** \remarks
    Equality operator for the OdGsDCPoint class.
    Points are considered equal if:
    * the x coordinate of this point equals to the x coordinate of the passed point
    * the y coordinate of this point equals to the y coordinate of the passed point

    \param dcPoint [in]  2D point in device coordinates to compare. 
  */
  bool operator==(const OdGsDCPoint& dcPoint) const { return x == dcPoint.x && y == dcPoint.y; }
  
  /** \remarks
    Inequality operator for the OdGsDCPoint class.
    Points are considered not equal if at least one of the following conditions fails:
    * the x coordinate of this point equals to the x coordinate of the passed point
    * the y coordinate of this point equals to the y coordinate of the passed point

    \param dcPoint [in]  2D point in device coordinates to compare. 
  */
  bool operator!=(const OdGsDCPoint& dcPoint) const { return x != dcPoint.x || y != dcPoint.y; }
  
  /** \remarks
    Returns i-th coordinate of this point.
    
    \param i [in]  i-th coordinate of this point where 0 is x coordinate and 1 is y coordinate. 
  */
  inline long operator[](unsigned int i) const { return *(&x + i); }

  long x;  // X-coordinate.
  long y;  // Y-coordinate.
};

/** \details
    This class represents 2D device coordinate rectangles.
    Corresponding C++ library: TD_Gs

    \remarks
    Device coordinates are measured in pixels from the lower-left corner
    of the device.
    
    \sa
    Coordinate Systems.
    
    <group OdGs_Classes>
*/
class OdGsDCRect
{
public:
  enum NullFlag { Null };
  /** \details
    Default constructor for the OdGsDCRect class.
  */
  OdGsDCRect() { }
  
  /** \details
    Constructor for the OdGsDCRect class.
    
    \param minPoint [in]  Lower-left corner.
    \param maxPoint [in]  Upper-right corner.
  */
  OdGsDCRect(const OdGsDCPoint& minPoint, const OdGsDCPoint& maxPoint) : m_min(minPoint), m_max(maxPoint) { }
  
  /** \details
    Constructor for the OdGsDCRect class.
    
    \param xMin [in]  Minimum X-coordinate.
    \param xMax [in]  Maximum X-coordinate.
    \param yMin [in]  Minimum Y-coordinate.
    \param yMax [in]  Maximum Y-coordinate.
  */
  OdGsDCRect(long xMin, long xMax, long yMin, long yMax) : m_min(xMin,yMin), m_max(xMax,yMax) { }
  
  /** \details
    Constructor for the OdGsDCRect class. Creates a null rectangle.
  */
  OdGsDCRect(NullFlag) { set_null(); }
  
  /** \details
    Assignment operator for the OdGsDCRect class. Assigns the passed rectangle.
    
    \param dcRect [in]  Minimum X-coordinate.
  */
  OdGsDCRect & operator=(const OdGsDCRect& dcRect)
  {
    m_min = dcRect.m_min;
    m_max = dcRect.m_max;
    return*this;
  }
  /** \details
    Assignment by bitwise OR operator for the OdGsDCRect class. 
    The minor values for coordinates are assigned to the minimum point while the greater values are assigned to the maximum point.
    As result, rectangle is extended.
    
    \param dcRect [in]  Rectangle, points of which are assigned, if necessary.
  */
  void operator|=(const OdGsDCRect& dcRect)
  {
    if(m_min.x > dcRect.m_min.x) 
      m_min.x = dcRect.m_min.x; 
    if(m_max.x < dcRect.m_max.x) 
      m_max.x = dcRect.m_max.x;
    
    if(m_min.y > dcRect.m_min.y)
      m_min.y = dcRect.m_min.y;
    if(m_max.y < dcRect.m_max.y)
      m_max.y = dcRect.m_max.y;
  }
  
  /** \details
    Assignment by bitwise AND operator for the OdGsDCRect class. 
    The greater values for coordinates are assigned to the minimum point while the minor values are assigned to the maximum point.
    New rectangle is result of intersection between this rectangle and the passed rectangle.
    
    \param dcRect [in]  Rectangle, points of which are assigned, if necessary.
  */
  void operator&=(const OdGsDCRect& dcRect)
  {
    intersectWith(dcRect);
  }
  
  /** \remarks
    Equality operator for the OdGsDCRect class.
    Rectangles are considered equal if:
    * the minimum point of this rectangle equals to the minimum point of the passed rectangle
    * the maximum point of this rectangle equals to the maximum point of the passed rectangle

    \param dcPoint [in]  2D rectangle to compare. 
  */
  bool operator==(const OdGsDCRect& dcRect) const
  {
    return m_min == dcRect.m_min && m_max == dcRect.m_max; 
  }
  
  /** \remarks
    Inequality operator for the OdGsDCRect class.
    Rectangles are considered not equal if at least one of the following conditions fails:
    * the minimum point of this rectangle equals to the minimum point of the passed rectangle
    * the maximum point of this rectangle equals to the maximum point of the passed rectangle

    \param dcRect [in]  2D rectangle to compare.
  */
  bool operator!=(const OdGsDCRect& dcRect) const
  {
    return !(*this == dcRect);
  }
  /** \details
    Sets this rectangle object to the null rectangle.
  */
  void set_null()
  { 
    m_min = OdGsDCPoint::Maximum; 
    m_max = OdGsDCPoint::Minimum; 
  } 
  
  /** \details
    Checks whether this rectangle object is the null rectangle.
    \returns 
    true if this rectangle object is the null rectangle, false otherwise.
  */
  bool is_null() const
  {
    ODA_ASSERT((m_min.x <= m_max.x && m_min.y <= m_max.y) ||
     (m_min == OdGsDCPoint::Maximum && m_max == OdGsDCPoint::Minimum));
    return m_min.x > m_max.x;
  }
  
  /** \details
    Checks whether this rectangle object is inside the specified rectangle object.
    
    \param dcRect [in]  Any display coordinate rectangle.
    \returns
    true if this rectangle object is inside the specified rectangle object.
    \remarks
    A coincident edge is considered inside.
  */
  bool within(const OdGsDCRect& dcRect) const
  {
    ODA_ASSERT(!is_null()); 
    return m_min.x >= dcRect.m_min.x && 
      m_min.y >= dcRect.m_min.y && 
      m_max.x <= dcRect.m_max.x &&
      m_max.y <= dcRect.m_max.y;
  }

  /** \details
    Moves this rectangle in the provided direction.
    \param x [in]  Number of pixels to move by x-axis. Negative value moves rectangle in the opposite direction of the positive x-axis.
    \param y [in]  Number of pixels to move by y-axis. Negative value moves rectangle in the opposite direction of the positive y-axis.
  */
  void offset(long x, long y)
  {
    m_min.x += x;
    m_max.x += x;
    m_min.y += y;
    m_max.y += y;
  }

  /** \details
    Makes a new rectangle as a result of intersection between this rectangle and the passed rectangle.
    \param dcRect     [in]  Rectangle to intersect with.
    \param bValidate  [in]  Validates the new rectangle. If set to true, the new rectangle is guarantied to be valid or null.
  */
  void intersectWith(const OdGsDCRect& dcRect, bool bValidate = true)
  {
    if(m_min.x < dcRect.m_min.x)
      m_min.x = dcRect.m_min.x; 
    if(m_max.x > dcRect.m_max.x) 
      m_max.x = dcRect.m_max.x;

    if(m_min.y < dcRect.m_min.y)
      m_min.y = dcRect.m_min.y;
    if(m_max.y > dcRect.m_max.y)
      m_max.y = dcRect.m_max.y;

    if(bValidate && (m_min.x > m_max.x || m_min.y > m_max.y))
      *this = Null;
  }

  /** \details
    Normalizes this rectangle by setting valid values to minimum and maximum points.
  */
  void normalize()
  {
    long tmp;
    if (m_max.x < m_min.x)
      tmp = m_max.x, m_max.x = m_min.x, m_min.x = tmp;
    if (m_max.y < m_min.y)
      tmp = m_max.y, m_max.y = m_min.y, m_min.y = tmp;
  }

  /** \details
    Checks whether the specified rectangle object does not intersect this rectangle.

    \param r [in]  Rectangle to check.
    \returns
    true if the specified rectangle object does not intersect
    this rectangle, false otherwise.
  */
  bool isDisjoint(const OdGsDCRect& r) const
  {
    return (r.m_min.x > m_max.x || r.m_min.y > m_max.y ||
            m_min.x > r.m_max.x || m_min.y > r.m_max.y);
  }

  /** Minimum point. */
  OdGsDCPoint m_min;
  /** Maximum point. */
  OdGsDCPoint m_max;
};

/** \details
    This class represents 2D device coordinate rectangles.
    Corresponding C++ library: TD_Gs

    \remarks
    Device coordinates are measured in pixels from the lower-left corner
    of the device.
    
    \sa
    Coordinate Systems.
    
    <group OdGs_Classes>
*/
class OdGsDCRectDouble
{
public:
  /** \details
    Default constructor for the OdGsDCRectDouble class.
  */
  OdGsDCRectDouble() { }

  /** \details
    Constructor for the OdGsDCRectDouble class.
    
    \param minPoint [in]  Lower-left corner.
    \param maxPoint [in]  Upper-right corner.
  */
  OdGsDCRectDouble(const OdGePoint2d& minPoint, const OdGePoint2d& maxPoint)
    : m_min(minPoint), m_max(maxPoint) { }

  /** \details
    Constructor for the OdGsDCRectDouble class.
    
    \param xMin [in]  Minimum X-coordinate.
    \param xMax [in]  Maximum X-coordinate.
    \param yMin [in]  Minimum Y-coordinate.
    \param yMax [in]  Maximum Y-coordinate.
  */
  OdGsDCRectDouble(double xMin, double xMax, double yMin, double yMax)
    : m_min(xMin,yMin), m_max(xMax,yMax) { }

  /** \details
    Constructor for the OdGsDCRectDouble class. Creates an instance base on the passed rectangle.
    
    \param rc [in]  Rectangle to copy.
  */
  explicit OdGsDCRectDouble(const OdGsDCRect& rc)
    : m_min(rc.m_min.x, rc.m_min.y), m_max(rc.m_max.x,rc.m_max.y) { }

  /** \details
    Assignment operator for the OdGsDCRectDouble class. Assigns the passed rectangle.
    
    \param dcRect [in]  Rectangle to assign.
  */
  OdGsDCRectDouble& operator=(const OdGsDCRect& dcRect)
  {
    m_min.x = dcRect.m_min.x;
    m_min.y = dcRect.m_min.y;
    m_max.x = dcRect.m_max.x;
    m_max.y = dcRect.m_max.y;
    return *this;
  }

  /** \remarks
    Equality operator for the OdGsDCRectDouble class.
    Rectangles are considered equal if:
    * the minimum point of this rectangle equals to the minimum point of the passed rectangle
    * the maximum point of this rectangle equals to the maximum point of the passed rectangle

    \param dcPoint [in]  2D rectangle to compare. 
  */
  bool operator==(const OdGsDCRectDouble& dcRect) const
  {
    return m_min == dcRect.m_min && m_max == dcRect.m_max; 
  }
  
  /** \remarks
    Inequality operator for the OdGsDCRect class.
    Rectangles are considered not equal if at least one of the following conditions fails:
    * the minimum point of this rectangle equals to the minimum point of the passed rectangle
    * the maximum point of this rectangle equals to the maximum point of the passed rectangle

    \param dcRect [in]  2D rectangle to compare.
  */
  bool operator!=(const OdGsDCRectDouble& dcRect) const
  {
    return !(*this == dcRect);
  }

  /** \remarks
    Rounds coordinate values of the minimum and maximum points.
  */
  OdGsDCRect round() const
  {
    return OdGsDCRect(OdGsDCRect(
        OdTruncateToLong(m_min.x),
        OdTruncateToLong(m_max.x),
        OdTruncateToLong(m_min.y),
        OdTruncateToLong(m_max.y)));
  }
  /** Minimum point. */
  OdGePoint2d m_min;
  /** Maximum point. */
  OdGePoint2d m_max;
};

/** \details
    This data type represents a window handle, analogue of Windows window -- HWND
*/
typedef void* OdGsWindowingSystemID; 

/** \details
    Represents metafile play mode.
*/
enum EMetafilePlayMode
{
  /** Play for visualization. Default, valid for all metafile types. */
  kMfDisplay,
  /** Play as geometry for selection. Valid only if metafile can be played as OdGiConveyorGeometry ( if useMetafileAsGeometry() returns true). */
  kMfSelect,
  /** Play data of nested metafiles only. Valid only if metafile can be played as OdGiConveyorGeometry ( if useMetafileAsGeometry() returns true). */
  kMfNested,
  /** Play as geometry for extents calculation. Valid only if metafile can be played as OdGiConveyorGeometry ( if useMetafileAsGeometry() returns true). */
  kMfExtents
};

// Negative GETBIT
#ifndef GETBITNEG
#define GETBITNEG(flags, bit) (((flags) & (bit)) != (bit))
#endif

#include "TD_PackPop.h"

#endif // __GSDEFS_H_INCLUDED_
