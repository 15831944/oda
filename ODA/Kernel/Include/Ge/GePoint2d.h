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


#ifndef OD_GEPNT2D_H
#define OD_GEPNT2D_H /*!DOM*/

#include "Ge/GeGbl.h"

#ifdef OD_HAVE_MATH_FILE
#include <math.h>
#endif

#ifdef OD_HAVE_COMPLEX_FILE
#if __cplusplus >= 201103L
#include <sstream>
#else
#include <complex>
#endif
#endif

class OdGeMatrix2d;
class OdGeVector2d;
class OdGeLinearEnt2d;
class OdGeLine2d;

#include "TD_PackPush.h"

/** \details
    This class represents points (locations) in 2D space. 

    \remarks
    OdGePoint2d may be viewed as an array[2] of doubles.
    
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>
    
    \sa
    <link ge_BasicTypes, Working with Basic Geometry Types>
*/
class GE_TOOLKIT_EXPORT OdGePoint2d
{
public:
  
  /** \details
    Default constructor for the OdGePoint2d class.

    \remarks
    The default constructor constructs a point at the origin.
  */
  OdGePoint2d()
    : x (0.0), y (0.0)
  {
  }
  
  /** \details
    Constructor for the OdGePoint2d class.

    \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.
    
    \remarks
    Constructs a 2D point with specified coordinates.
  */
  OdGePoint2d(
    double xx, 
    double yy) 
    : x (xx), y (yy)
  {
  }

  GE_STATIC_EXPORT static const OdGePoint2d kOrigin; // Origin (0,0).

  /** \remarks
    Sets this point to the result of the matrix multiplication of matrix * point.

    \param matrix [in]  Any 2D matrix.
    \param point [in]  The point to be multiplied.
  */
  friend GE_TOOLKIT_EXPORT OdGePoint2d operator *(
    const OdGeMatrix2d& matrix, 
    const OdGePoint2d& point);

  /** \remarks
    Sets this point to the result of the matrix multiplication of the point and a scale factor. 

    \param scale [in]  Scale factor.
    \param point [in]  The point to be multiplied.
  */  
  friend inline OdGePoint2d operator *(
    double scale, 
    const OdGePoint2d& point)
  {
    return OdGePoint2d(point.x * scale, point.y * scale);
  }

  /** \details
    Sets this point to the result of the matrix multiplication (matrix * point).
    Returns a reference to this point.

    \param matrix [in]  A 2D matrix with which a specified point is multiplied.
    \param point [in]  The point to be multiplied.
  */
  OdGePoint2d& setToProduct(
    const OdGeMatrix2d& matrix, 
    const OdGePoint2d& point);

  /** \details
    Sets this point to the result of the matrix multiplication of xfm * this point.
    Returns a reference to this point.

    \param xfm [in]  A 2D matrix with which this point is multiplied.
  */
  OdGePoint2d& transformBy(
    const OdGeMatrix2d& xfm);

  /** \details
    Rotates this point about the specified basePoint by the specified angle. 

    \param angle [in]  Rotation angle in radians.
    \param basePoint [in]  Basepoint relative to which rotation is computed.
  */
  OdGePoint2d& rotateBy(
    double angle, 
    const OdGePoint2d& basePoint = kOrigin);

  /** \details
    Mirrors this point about the specified 2D line.

    \param line [in]  Mirror Line.
  */
  OdGePoint2d& mirror(
    const OdGeLine2d& line);

  /** \details
    Scales this point by the scale factor about the basepoint.

    \param scaleFactor [in]  Scale Factor. The scale factor must be greater than zero.
    \param basePoint [in]  Basepoint relative to which scaling is computed.
  */
  OdGePoint2d& scaleBy(
    double scaleFactor, 
    const OdGePoint2d& basePoint = kOrigin);

  /** \details
    Multiplication operator for the OdGePoint2d class.
    
    \param scale [in]  Input scale factor.
    
    \remarks
    Makes this point equal to the point scaled by a scale factor. 
  */
  OdGePoint2d operator *(
    double scale) const
  {
    return OdGePoint2d (x*scale, y*scale);
  }

  /** \details
    Multiplication operator for the OdGePoint2d class.
    
    \param scale [in]  Input scale factor.
    
    \remarks
    Scales this point by a scale factor. Returns the reference to this point.
  */
  OdGePoint2d& operator *=(
    double scale)
  {
    x *= scale;
    y *= scale;
    return *this;
  }

  /** \details
    Division operator for the OdGePoint2d class.
    
    \param scale [in]  Input scale factor.
    
    \remarks
    Returns the point equal to this point scaled by a scale factor 1/scale.  
  */
  OdGePoint2d operator /(
    double scale) const
  {
    return OdGePoint2d (x/scale, y/scale);
  }

  /** \details
    Division operator for the OdGePoint2d class.
    
    \param scale [in]  Input scale factor.
    
    \remarks
    Returns a reference to a point that is the result of scaling this point with respect to the origin by a scale factor. 
  */
  OdGePoint2d& operator /=(
    double scale)
  {
    x /= scale;
    y /= scale;
    return *this;
  }

  /** \details
    Addition operator for the OdGePoint2d class.
    
    \param vect [in]  Input vector by which a point is translated.
    
    \remarks
    Translates this point by the specified vector.
  */
  OdGePoint2d operator +(
    const OdGeVector2d& vect) const;

  /** \details
    Addition operator for the OdGePoint2d class.
    
    \param vect [in]  Input vector by which a point is translated.
    
    \remarks
    Returns a 2D point that is translated by the specified input vector.
  */
  OdGePoint2d& operator +=(
    const OdGeVector2d& vect);

    /** \details
      Subtraction operator for the OdGePoint2d class.

      \param vect [in]  Input vector by which a point is translated.

      \remarks
      Returns a point that is translated in the opposite direction and length of the specified input vector.
    */
    OdGePoint2d operator -(
      const OdGeVector2d& vect) const;

  /** \details
    Subtraction operator for the OdGePoint2d class.
    
    \param vect [in]  Input vector by which a point is translated.
    
    \remarks
    Returns a reference to the point equal to this point translated by the vector opposite to the specified vector.
  */
  OdGePoint2d& operator -=(
    const OdGeVector2d& vect);

  /** \details
    Sets this point to point + vect, and returns a reference to this point.

    \param point [in]  Any 2D point.
    \param vect [in]  A 2D vector.
  */
  OdGePoint2d& setToSum(
    const OdGePoint2d& point,
    const OdGeVector2d& vect);

  /** \details
    Subtraction operator for the OdGePoint2d class.
    
    \param point [in]  Input 2D point, coordinates of which are subtracted from the coordinates of this point.
    
    \remarks
    Returns a vector formed as a result of this point's coordinates minus coordinates of the specified point.
  */
  OdGeVector2d operator -(
    const OdGePoint2d& point) const;

  /** \details
    Returns this point as a vector.
  */
  const OdGeVector2d& asVector() const;

  /** \details
    Returns this point as a vector.
  */
  OdGeVector2d& asVector();

  /** \details
    Returns the distance from this point to the specified point.

    \param point [in]  Any 2D point to which a distance is measured.
  */
  double distanceTo(
    const OdGePoint2d& point) const
  {
    return sqrt((x-point.x) * (x-point.x) + (y-point.y) * (y-point.y));
  }

  /** \details
  Returns the squared distance from this point to the specified point.

  \param point [in]  Any 2D point to which a squared distance is measured.
  */
  double distanceSqrdTo(
    const OdGePoint2d& point) const
  {
    double dx = x - point.x;
    double dy = y - point.y;
    double r = dx*dx + dy*dy;
    return r;
  }

  /** \details
    Equality operator. Returns true if all the coordinates of this point are equal to the corresponding coordinates
    of the specified point.
    
    \param point [in] A 2D point to be checked for equality with this point.
  */
  bool operator ==(
    const OdGePoint2d& point) const
  {
    return isEqualTo (point);
  }
  
  /** \details
    Inequality operator. Returns true if any of the coordinates of this point are not equal to the corresponding coordinates
    of the specified point.
    
    \param point [in] A 2D point to be checked for inequality with this point.
  */
  bool operator !=(
    const OdGePoint2d& point) const
  {
    return !isEqualTo (point);
  }

  /** \details
    Returns true if and only if the distance from this point to point is within the 
    specified tolerance.

    \param point [in]  Any 2D point.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo(
    const OdGePoint2d& point, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** 
    \details
    Returns ith coordinate of this point.
    
    \param i [in]  Index of coordinate.
    
    \remarks
    * 0 returns the X-coordinate.
    * 1 returns the Y-coordinate.
  */
  double& operator [](
    unsigned int i)
  {
    return * (&x+i);
  }
  
  /** 
    \details
    References ith coordinate of this point.
    
    \param i [in]  Index of coordinate.
    
    \remarks
    * 0 references the X-coordinate.
    * 1 references the Y-coordinate.
  */
  double operator [](
    unsigned int i) const
  {
    return * (&x+i);
  }

  /** \details
    Sets this point to the specified X and Y coordinates. 
    Returns a reference to this point.

    \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.
  */
  OdGePoint2d& set(
    double xx, 
    double yy)
  {
    x = xx;
    y = yy;
    return *this;
  }

  double x; // X-coordinate
  double y; // Y-coordinate
};

#include "TD_PackPop.h"


#include "Ge/GeVector2d.h"
//DOM-IGNORE-BEGIN
inline OdGePoint2d OdGePoint2d::operator +(
  const OdGeVector2d& vect) const
{
  return OdGePoint2d(x + vect.x, y + vect.y);
}

inline OdGePoint2d& OdGePoint2d::operator +=(
  const OdGeVector2d& vect)
{
  x += vect.x;
  y += vect.y;
  return *this;
}

inline OdGePoint2d OdGePoint2d::operator -(
  const OdGeVector2d& vect) const
{
  return OdGePoint2d(x - vect.x, y - vect.y);
}

inline OdGePoint2d& OdGePoint2d::operator -=(
  const OdGeVector2d& vect)
{
  x -= vect.x;
  y -= vect.y;
  return *this;
}

inline OdGeVector2d OdGePoint2d::operator -(
  const OdGePoint2d& point) const
{
  return OdGeVector2d(x - point.x, y - point.y);
}

inline const OdGeVector2d& OdGePoint2d::asVector() const
{
  ODA_ASSUME(sizeof(OdGePoint2d) == 2 * sizeof(double));
  ODA_ASSUME(sizeof(OdGeVector2d) == 2 * sizeof(double));
  return *reinterpret_cast<const OdGeVector2d*>(this);
}

inline OdGeVector2d& OdGePoint2d::asVector()
{
  ODA_ASSUME(sizeof(OdGePoint2d) == 2 * sizeof(double));
  ODA_ASSUME(sizeof(OdGeVector2d) == 2 * sizeof(double));
  return *reinterpret_cast<OdGeVector2d*>(this);
}
//DOM-IGNORE-END
#endif

