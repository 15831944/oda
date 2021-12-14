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


#ifndef OD_GEPNT3D_H
#define OD_GEPNT3D_H /*!DOM*/

#include "Ge/GeGbl.h"

#ifdef OD_HAVE_MATH_FILE
#include <math.h>
#endif

class OdGeMatrix3d;
class OdGeLinearEnt3d;
class OdGePlane;
class OdGePlanarEnt;
class OdGeVector3d;
class OdGePoint2d;

#include "TD_PackPush.h"

/** \details
    This class represents points (locations) in 3D space. 

    \remarks
    OdGePoint3d may be viewed as an array[3] of doubles.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_BasicTypes, Working with Basic Geometry Types>
*/
class GE_TOOLKIT_EXPORT OdGePoint3d
{
public:
  
  /** \details
    Default constructor for the OdGePoint3d class.

    \remarks
    The default constructor constructs a point at the origin.
  */
  OdGePoint3d() 
    : x (0.0), y (0.0), z (0.0)
  {
  }
  
  /** \details
    Constructor for the OdGePoint3d class.

    \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.
    \param zz [in]  The Z-coordinate of this point.
    
    \remarks
    Constructs a 3D point with specified coordinates.
  */
  OdGePoint3d(
    double xx, 
    double yy, 
    double zz)
    : x (xx), y (yy), z (zz)
  {
  }
  
  /** \details
    Constructor for the OdGePoint3d class.

    \param plane [in]  Any planar entity.
    \param pnt2d [in]  2D parameter in a plane's coordinates.
    
    \remarks
    Constructs a 3D point by the formula:
    
    p + uAxis * pnt2d.x + vAxis * pnt2d.y,
    
    where p, uAxis, and vAxis are returned by the function call plane.get (p, uAxis, vAxis);
  */
  OdGePoint3d(
    const OdGePlanarEnt& plane, 
    const OdGePoint2d& pnt2d);

  GE_STATIC_EXPORT static const OdGePoint3d kOrigin; // Origin (0,0,0).

  /** \remarks
    Sets this point to the result of the matrix multiplication of matrix * point.

    \param matrix [in]  Any 3D matrix.
    \param point [in]  The point to be multiplied.
  */
  friend GE_TOOLKIT_EXPORT OdGePoint3d operator *(
    const OdGeMatrix3d& matrix, 
    const OdGePoint3d& point);
  
  /** \remarks
    Sets this point to the result of the matrix multiplication of the point and a scale factor. 

    \param scale [in]  Scale factor.
    \param point [in]  The point to be multiplied.
  */
  friend inline OdGePoint3d operator *(
    double scale, 
    const OdGePoint3d& point)
  {
    return OdGePoint3d(point.x * scale, point.y * scale, point.z * scale);
  }

  /** \details
    Sets this point to the product of xfm * point, and returns a reference to this point.
    
    \param xfm [in]  A 3D matrix with which a specified point is multiplied. 
    \param point [in]  The point to be multiplied.
  */
  OdGePoint3d& setToProduct(
    const OdGeMatrix3d& xfm, 
    const OdGePoint3d& point);

  /** \details
    Sets this point to the result of the matrix multiplication of xfm * this point.
    Returns a reference to this point.

    \param xfm [in]  A 3D matrix with which this point is multiplied.
  */
  OdGePoint3d& transformBy(
    const OdGeMatrix3d& xfm);
 
  /** \details
    Rotates this point about the specified basePoint and axis of rotation by the specified angle. 

    \param angle [in]  Rotation angle in radians.
    \param vect [in]  Axis of rotation.
    \param basePoint [in]  Basepoint relative to which rotation is computed.
  */
  OdGePoint3d& rotateBy(
    double angle, 
    const OdGeVector3d& vect, 
    const OdGePoint3d& basePoint = OdGePoint3d::kOrigin);

  /** \details
    Mirrors this point about the specified plane.

    \param plane [in]  Mirror plane.
  */
  OdGePoint3d& mirror(
    const OdGePlane& plane);

  /** \details
    Scales this point by the scale factor about the basepoint.

    \param scaleFactor [in]  Scale Factor. The scale factor must be greater than zero.
    \param basePoint [in]  Basepoint relative to which scaling is computed.
  */
  OdGePoint3d& scaleBy(
    double scaleFactor, 
    const OdGePoint3d& basePoint = OdGePoint3d::kOrigin);

  /** \details
    Returns the 2D point with the coordinates corresponding to the decomposition of the vector * this - p in the basis uAxis, 
    vAxis of the planar entity, where p, uAxis, and vAxis can be found by the function call plane.get(p, uAxis, vAxis).
    
    \param plane [in]  A plane on which this point lies.
    
    \remarks
    This 3D point is assumed to be on the specified plane.
  */
  OdGePoint2d convert2d (
    const OdGePlanarEnt& plane) const;
   
  /** \details
    Returns the 2D point from this point discarding the Z-coordinate.
  */
  OdGePoint2d convert2d() const;

  /** \details
    Multiplication operator for the OdGePoint3d class.
    
    \param scale [in]  Input scale factor.
    
    \remarks
    Makes this point equal to the point scaled by a scale factor. 
  */
  OdGePoint3d operator *(
    double scale) const
  {
     return OdGePoint3d (x * scale, y * scale, z * scale);
  }
  
  /** \details
    Multiplication operator for the OdGePoint3d class.
    
    \param scale [in]  Input scale factor.
    
    \remarks
    Scales this point by a scale factor. Returns the reference to this point.
  */
  OdGePoint3d& operator *=(
    double scale)
  {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
  }
  
  /** \details
    Division operator for the OdGePoint3d class.
    
    \param scale [in]  Input scale factor.
    
    \remarks
    Returns the point equal to this point scaled by a scale factor 1/scale.  
  */
  OdGePoint3d operator /(
    double scale) const
  {
    return OdGePoint3d (x / scale, y / scale, z / scale);
  }
    
  /** \details
    Division operator for the OdGePoint3d class.
    
    \param scale [in]  Input scale factor.
    
    \remarks
    Returns a reference to a point that is the result of scaling this point with respect to the origin by a scale factor. 
  */
  OdGePoint3d& operator /=(
    double scale)
  {
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
  }
  
  /** \details
    Addition operator for the OdGePoint3d class.
    
    \param vect [in]  Input vector by which a point is translated.
    
    \remarks
    Translates this point by the specified vector.
  */
  OdGePoint3d operator +(
    const OdGeVector3d& vect) const;
  
  /** \details
    Addition operator for the OdGePoint3d class.
    
    \param vect [in]  Input vector by which a point is translated.
    
    \remarks
    Returns a reference to a point that is translated by the specified input vector.
  */
  OdGePoint3d& operator +=(
    const OdGeVector3d& vect);
  
  /** \details
    Subtraction operator for the OdGePoint3d class.
    
    \param vect [in]  Input vector by which a point is translated.
    
    \remarks
    Returns a point that is translated in the opposite direction and length of the specified input vector.
  */
  OdGePoint3d operator -(
    const OdGeVector3d& vect) const;
  
  /** \details
    Subtraction operator for the OdGePoint3d class.
    
    \param vect [in]  Input vector by which a point is translated.
    
    \remarks
    Returns a reference to the point equal to this point translated by the vector opposite to the specified vector.
  */
  OdGePoint3d& operator -=(
    const OdGeVector3d& vect);
      
  /** \details
    Sets this point to point + vect, and returns a reference to this point.
    
    \param point [in]  Any 3D point.
    \param vect [in]  Any 3D vector.
  */
  OdGePoint3d& setToSum(
    const OdGePoint3d& point, 
    const OdGeVector3d& vect);

  /** \details
    Subtraction operator for the OdGePoint3d class.
    
    \param point [in]  Input 3D point, coordinates of which are subtracted from the coordinates of this point.
    
    \remarks
    Returns a vector formed as a result of this point's coordinates minus coordinates of the specified point.
  */
  OdGeVector3d operator -(
    const OdGePoint3d& point) const;
    
  /** \details
    Returns this point as a vector.
  */
  const OdGeVector3d& asVector() const;

  /** \details
  Returns this point as a vector.
  */
  OdGeVector3d& asVector();

  /** \details
    Returns the distance from this point to the specified point.

    \param point [in]  Any 3D point to which a distance is measured.
  */
  double distanceTo(
    const OdGePoint3d& point) const
  {
    double dx = x - point.x, dy = y - point.y, dz = z - point.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
  }

  /** \details
    Returns the squared distance from this point to the specified point.

    \param point [in]  Any 3D point to which a squared distance is measured.
  */
  double distanceSqrdTo(
    const OdGePoint3d& point) const
  {
    double dx = x - point.x;
    double dy = y - point.y;
    double dz = z - point.z;
    double r = dx*dx + dy*dy + dz*dz;
    return r;
  }

  /** \details
    Projects this point onto the specified plane along the specified vector.

    \param plane [in]  Plane on which this point is to be projected
    \param vect [in]  Vector defining the projection direction.
  */
  OdGePoint3d project(
    const OdGePlane& plane,
    const OdGeVector3d& vect) const;
    
  /** \details
    Returns the 3D point that is the result of the orthogonal projection of this point into the specified plane. 

    \param plane [in]  Plane on which this point is to be projected
  */
    OdGePoint3d orthoProject(
    const OdGePlane& plane) const;

  /** \details
    Equality operator. Returns true if all the coordinates of this point are equal to the corresponding coordinates
    of the specified point.
    
    \param point [in] A 3D point to be checked for equality with this point.
  */
  bool operator ==(
    const OdGePoint3d& point) const
  {
    return isEqualTo (point);
  }
  
  /** \details
    Inequality operator. Returns true if any of the coordinates of this point are not equal to the corresponding coordinates
    of the specified point.
    
    \param point [in] A 3D point to be checked for inequality with this point.
  */
  bool operator !=(
    const OdGePoint3d& point) const
  {
    return !isEqualTo (point);
  }
    
  /** \details
    Returns true if and only if the distance from this point to point is within the 
    specified tolerance.

    \param point [in]  Any 3D point.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;
  
  /** 
    \details
    Returns ith coordinate of this point.
    
    \param i [in]  Index of coordinate.
    
    \remarks
    * 0 returns the X-coordinate.
    * 1 returns the Y-coordinate.
    * 2 returns the Z-coordinate.
  */
  double operator [](
    unsigned int i) const
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
    * 2 references the Z-coordinate.
  */
  double& operator [](
    unsigned int i)
  {
    return * (&x+i);
  }
  
  /** \details
    Sets the parameters for this point according to the arguments
    
    \param xx [in]  The X-coordinate of this point.
    \param yy [in]  The Y-coordinate of this point.
    \param zz [in]  The Z-coordinate of this point.
  */
  OdGePoint3d& set(
    double xx, 
    double yy, 
    double zz)
  {
    x = xx;
    y = yy;
    z = zz;
    return *this;
  }

  /** \details
    Sets the parameters for this point according to the arguments.
    
    \param xy [in]  A 2D point that provides the X and Y coordinates for this point.
    \param zz [in]  The Z-coordinate of this point.
  */
  OdGePoint3d& set(
    const OdGePoint2d& xy,
    double zz = 0.0);

 /** \details
    Sets the parameters for this point according to the arguments
    
    \param plane [in]  Any plane.
    \param point [in]  Any 2D point.
    
    \remarks
    Returns a reference to this point.
    \remarks
    plane and pnt2d define a point as follows:
            
            p + uAxis * point.x + vAxis * point.y,
            
    where p, uAxis, and vAxis are returned by the function call plane.get (p, uAxis, vAxis);
  */  
  OdGePoint3d& set(
    const OdGePlanarEnt& plane, 
    const OdGePoint2d& point);

  double x; // X-coordinate
  double y; // Y-coordinate
  double z; // Z-coordinate

  /** \details
    Swaps coordinates of this point and the specified point.
    
    \param point [in]  A 3D point's coordinates which are swapped with coordinates of this point.
  */
  OdGePoint3d& swapWithPoint(
    OdGePoint3d& point);
};

#include "TD_PackPop.h"


#include "Ge/GeVector3d.h"
//DOM-IGNORE-BEGIN
inline OdGePoint3d OdGePoint3d::operator +(
  const OdGeVector3d& vect) const
{
  return OdGePoint3d(x + vect.x, y + vect.y, z + vect.z);
}

inline OdGePoint3d& OdGePoint3d::operator +=(
  const OdGeVector3d& vect)
{
  x += vect.x;
  y += vect.y;
  z += vect.z;
  return *this;
}

inline OdGePoint3d OdGePoint3d::operator -(
  const OdGeVector3d& vect) const
{
  return OdGePoint3d(x - vect.x, y - vect.y, z - vect.z);
}

inline OdGePoint3d& OdGePoint3d::operator -=(
  const OdGeVector3d& vect)
{
  x -= vect.x;
  y -= vect.y;
  z -= vect.z;
  return *this;
}

inline OdGeVector3d OdGePoint3d::operator -(
  const OdGePoint3d& point) const
{
  return OdGeVector3d(x - point.x, y - point.y, z - point.z);
}

inline const OdGeVector3d& OdGePoint3d::asVector() const
{
  ODA_ASSUME(sizeof(OdGePoint3d) == 3 * sizeof(double));
  ODA_ASSUME(sizeof(OdGeVector3d) == 3 * sizeof(double));
  return *reinterpret_cast<const OdGeVector3d*>(this);
}

inline OdGeVector3d& OdGePoint3d::asVector()
{
  ODA_ASSUME(sizeof(OdGePoint3d) == 3 * sizeof(double));
  ODA_ASSUME(sizeof(OdGeVector3d) == 3 * sizeof(double));
  return *reinterpret_cast<OdGeVector3d*>(this);
}
//DOM-IGNORE-END
#endif

