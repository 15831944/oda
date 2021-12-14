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




#ifndef OD_GEVEC3D_H
#define OD_GEVEC3D_H /*!DOM*/


#include "Ge/GeGbl.h"

class OdGeMatrix3d;
class OdGePlane;
class OdGePlanarEnt;
class OdGePoint3d;
class OdGeVector2d;

#include "TD_PackPush.h"

/** \details
    This class represents vectors in 3D space. 

    \remarks
    OdGeVector3d may be viewed as an array[3] of doubles.
    
    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 

    \sa
    <link ge_BasicTypes, Working with Basic Geometry Types>
*/
class GE_TOOLKIT_EXPORT OdGeVector3d
{
public:

  /** \details 
    Default constructor for the OdGeVector3d class.
    
    \remarks
    Constructs a zero-length 3D vector.
  */
  OdGeVector3d() : x(0.0), y(0.0), z(0.0) {}

  /** \details 
    Constructor for the OdGeVector3d class.
    
    \param xx [in]  X-coordinate.
    \param yy [in]  Y-coordinate.
    \param zz [in]  Z-coordinate.
    
    \remarks
    Constructs a 3D vector with specified X, Y and Z coordinates.
  */
  OdGeVector3d(
    double xx, 
    double yy, 
    double zz) : x(xx), y(yy), z(zz) {}
  
  /** \details 
    Constructor for the OdGeVector3d class.
    
    \param plane [in]  2D plane.
    \param vector2d [in]  2D vector.
    
    \remarks
    Constructs  the 3D vector correspoponding to the 2D vector in the coordinates of the plane:
    
      uAxis * vect.x + vAxis * vect.y
    
    where uAxis and vAxis are returned by
    
      plane.get(origin, uAxis, vAxis)
  */
  OdGeVector3d(
    const OdGePlanarEnt& plane, 
    const OdGeVector2d& vector2d);

  GE_STATIC_EXPORT static const OdGeVector3d kIdentity; // Additive identity vector.
  GE_STATIC_EXPORT static const OdGeVector3d kXAxis; // X-Axis vector.
  GE_STATIC_EXPORT static const OdGeVector3d kYAxis; // Y-Axis vector.
  GE_STATIC_EXPORT static const OdGeVector3d kZAxis; // Z-Axis vector.

  /** \details
    Sets this vector to the product matrix * vect and returns
    a reference to this vector. 
    
    \param matrix [in]  Any 3D matrix. 
    \param vect [in]  Any 3D vector.
  */
  OdGeVector3d& setToProduct(
    const OdGeMatrix3d& matrix, 
    const OdGeVector3d& vect);
 
  /** \details
    Sets this vector to the product scale * vect and returns
    a reference to this vector. 
     
    \param vect [in]  Any 3D vector.
    \param scale [in]  Scale factor.
  */
 OdGeVector3d& setToProduct(
    const OdGeVector3d& vect, 
    double scale)
  {
    x = scale * vect.x;
    y = scale * vect.y;
    z = scale * vect.z;
    return *this;
  }
      

  /** \details
    Applies the 3D transformation matrix to this vector.

    \param xfm [in]  3D transformation matrix.
  */
  OdGeVector3d& transformBy(
    const OdGeMatrix3d& xfm);
    
  /** \details
    Rotates this vector the specified angle
    about the specified axis,
    and returns a reference to this vector.

    \param angle [in]  Rotation angle.
    \param axis [in]  Axis of rotation.
  */
  OdGeVector3d& rotateBy(
    double angle, 
    const OdGeVector3d& axis);
    
  /** \details
    Mirrors the entity about the plane passing through the
    origin with the specified normal, and returns
    a reference to the entity.

    \param normalToPlane [in]  Normal to Plane.
  */
  OdGeVector3d& mirror(
    const OdGeVector3d& normalToPlane);

  /** \details
    Returns the 2D vector, in the coordinate system
    of the plane, corresponding to the 3D vector.
    
    \param plane [in]  The planar entity on which the converted vector will be projected.
    
    \remarks
    The 3D vector must be parallel to the plane.
  */
  OdGeVector2d convert2d(
    const OdGePlanarEnt& plane) const;

  /** \details
    Returns the 2D vector, in the coordinate system
    of the XY plane, corresponding to the 3D vector.
    
    \remarks
    The 3D vector must be parallel to the XY plane.
  */
  OdGeVector2d convert2d() const;
  
  /** \details
    Multiplication operator for the OdGeVector3d class.

    \param scale [in]  Scale factor.
    
    \remarks
    Returns a vector with multiplied coordinates of this vector by the scale factor.
  */
  OdGeVector3d operator *(
    double scale) const
  { return OdGeVector3d(x * scale, y * scale, z * scale); }
  
  /** \details
    Multiplication operator for the OdGeVector3d class.

    \param scale [in]  Scale factor.
    
    \remarks
    Multiplies coordinates of this vector by the scale factor and returns reference to this vector.
  */
  OdGeVector3d& operator *=(
    double scale)
  {
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
  }
    
  /** \details
    Division operator for the OdGeVector3d class.

    \param scale [in]  Scale factor.
    
    \remarks
    Returns a vector with divided coordinates of this vector by the scale factor.
  */
  OdGeVector3d operator /(
      double scale) const
  {
    return OdGeVector3d(x / scale, y / scale, z / scale);
  }

  /** \details
    Division operator for the OdGeVector3d class.

    \param scale [in]  Scale factor.
    
    \remarks
    Divides coordinates of this vector by the scale factor and returns reference to this vector.
  */
  OdGeVector3d& operator /=(
      double scale)
  {
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
  }

  /** \details
    Addition operator for the OdGeVector3d class.

    \param vect [in]  A 3D vector to be added.
    
    \remarks
    Returns a vector that is created by adding coordinates of this vector to the coordinates of the specified vector.
  */
  OdGeVector3d operator +(
    const OdGeVector3d& vect) const
  { return OdGeVector3d(x + vect.x, y + vect.y, z + vect.z);}    
  
  /** \details
    Addition operator for the OdGeVector3d class.

    \param vect [in]   A 3D vector to be added.
    
    \remarks
    Adds coordinates of the specified vector to the coordinates of this vector and returns a reference to this vector.
  */
  OdGeVector3d operator +=(
    const OdGeVector3d& vect)
  { return OdGeVector3d(x += vect.x, y += vect.y, z += vect.z);}    

  /** \details
    Subtraction operator for the OdGeVector3d class.

    \param vect [in]  A 3D vector to be subtracted.
    
    \remarks
    Returns a vector that is created by subtracting coordinates of the passed vector from the coordinates of this vector.
  */
  OdGeVector3d operator -(
    const OdGeVector3d& vect) const
  { return OdGeVector3d(x - vect.x, y - vect.y, z - vect.z);}    
  
  /** \details
    Subtraction operator for the OdGeVector3d class.

    \param vect [in]   A 3D vector to be subtracted.
    
    \remarks
    Returns a vector that is created by subtracting coordinates of the passed vector from the coordinates of this vector.
  */
  OdGeVector3d operator -=(
    const OdGeVector3d& vect)
  { return OdGeVector3d(x -= vect.x, y -= vect.y, z -= vect.z);}    

  /** \details
  Returns this vector as a point.
  */
  const OdGePoint3d& asPoint() const;

  /** \details
  Returns this vector as a point.
  */
  OdGePoint3d& asPoint();

  /** \details
    Sets this vector to vector1 + vector1, and returns a reference to this vector.
    
    \param vector1 [in]  Any 3D vector.
    \param vector2 [in]  Any 3D vector.
  */
  OdGeVector3d& setToSum(
    const OdGeVector3d& vector1, 
    const OdGeVector3d& vector2)
  {
    x = vector1.x + vector2.x;
    y = vector1.y + vector2.y;
    z = vector1.z + vector2.z;
    return *this;
  }    

  /** \details
    Returns a vector with inverted X, Y and Z coordinates.

    \returns
    Vector opposite to this vector.
  */
  OdGeVector3d operator -() const { return OdGeVector3d(-x, -y, -z); }
  
  /** \details
    Negates this vector (-x, -y, -z), and returns a reference to this vector.
  */
  OdGeVector3d& negate()
  {
    x = -x;
    y = -y;
    z = -z;
    return *this;
  }
  

  /** \details
    Returns a vector perpendicular to this one.
    
    \remarks
    The orthogonal vector is determined by function OdGeContext::gOrthoVector()
  */
  OdGeVector3d perpVector() const;


  /** \details
    Returns the angle to the specified vector.
    
    \param vect [in]  Any 3D vector.
    
    \returns
    angle to the specified vector in radians as a double value.
    
    \remarks
    * Returns a value in the range [0.0 .. OdaPI].
    * This function is commutative.
  */
  double angleTo(
    const OdGeVector3d& vect) const;
  
  /** \details
    Returns the angle to the specified vector.
    
    \param vect         [in]  Any 3D vector.
    \param refVector    [in]  Reference vector.
    
    \returns
    angle to the specified vector in radians as a double value.
    
    \remarks
    * Returns a value in the range [0.0 .. Oda2PI].
    * If (refVector.dotProduct(crossProduct(vect)) >= 0.0, the return value is angleTo(vect).
    * If (refVector.dotProduct(crossProduct(vect)) < 0.0, the return value is Oda2PI - angleTo(vect)
  */
  double angleTo(
    const OdGeVector3d& vect,
    const OdGeVector3d& refVector) const;

  /** \details
    Returns the angle of this vector projected onto
    the specified plane
    
    \param plane [in]  Any 3D plane.
    
    \remarks
    This vector is projected orthogonally onto the 
    plane through its origin, and is measured with
    respect to axis1 as returned by
    
            plane.getCoordSystem(origin, axis1, axis2)
  */
  double angleOnPlane(
    const OdGePlanarEnt& plane) const;

  /** \details
    Returns the unit vector codirectional with this vector.
    
    \param tol [in]  Geometric tolerance.
        
    \remarks
    If the length() <= tol, this vector is returned.
  */
  OdGeVector3d normal(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Sets this vector to the unit vector codirectional with this vector,
    and returns a reference to this vector
    
    \param tol [in]  Geometric tolerance.

    \remarks
    If this.length() <= tol, this vector is unchanged.    
  */
  OdGeVector3d& normalize(
    const OdGeTol& tol = OdGeContext::gTol);
  
  /** \details
    Sets this vector to the unit vector codirectional with this vector,
    and returns a reference to this vector
    
    \param tol [in]  Geometric tolerance.
    \param status [out]  Receives the status of normalization.

    \remarks
    If this.length() <= tol, this vector is unchanged, and k0This is returned in status.

    Possible values for status are as follows:

    @untitled table
    kOk
    k0This
  */
  OdGeVector3d& normalize(
    const OdGeTol& tol, 
    OdGe::ErrorCondition& status);

  /** \details
    Sets this vector to the unit vector codirectional with this vector,
    and returns the length prior to normalization.
    
    \param tol [in]  Tolerance.
    
    \returns
    Vector length prior to normalization. If this.length() <= tol, this vector is unchanged and 0.0 is returned.
  */
  double normalizeGetLength(double tol = 1.e-300);

  /** \details
    Returns the length of this vector.
  */
  double length() const;

  /** \details
    Sets the length of this vector.

    \param length [in]  Vector length.
  */
  void setLength(double length);

  /** \details
    Returns the square of the length of this vector.
  */
  double lengthSqrd() const { return x*x + y*y + z*z; }

  /** \details
    Returns true if and only if the length of this vector is 1.0 within the specified tolerance.

    \param tol [in]  Geometric tolerance.
  */
  bool isUnitLength(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the length of this vector is 0.0 within the specified tolerance.

    \param tol [in]  Geometric tolerance.
  */
  bool isZeroLength(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified vector is parallel to this vector within the specified tolerance.

    \param vect [in]  Any 3D vector.
    \param tol [in]  Geometric tolerance.   
  */
  bool isParallelTo(
    const OdGeVector3d& vect,
    const OdGeTol& tol = OdGeContext::gTol) const;
  
  /** \details
    Returns true if and only if the specified vector is parallel to this vector within the specified tolerance.

    \param vect [in]  Any 3D vector.
    \param tol [in]  Geometric tolerance.
    \param status [out]  Receives the status of test.

    \remarks
    If the length of either vector is < tol, k0This is returned in status.

    Possible values for status are as follows:

    @untitled table
    kOk
    k0This
    k0Arg1
  */
  bool isParallelTo(
    const OdGeVector3d& vect,
    const OdGeTol& tol, 
    OdGeError& status) const;

  /** \details
    Returns true if and only if the specified vector is codirectional to this vector within the specified tolerance.

    \param vect [in]  Any 3D vector.
    \param tol [in]  Geometric tolerance. 
  */
  bool isCodirectionalTo(
    const OdGeVector3d& vect,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the specified vector is codirectional to this vector within the specified tolerance.

    \param vect [in]  Any 3D vector.
    \param tol [in]  Geometric tolerance.
    \param status [out]  Receives the status of test.

    \remarks
    If the length of either vector is < tol, k0This is returned in status.

    Possible values for status are as follows:

    @untitled table
    kOk
    k0This
    k0Arg1
  */
  bool isCodirectionalTo(
    const OdGeVector3d& vect,
    const OdGeTol& tol,
    OdGeError& status) const;
    

  /** \details
    Returns true if and only if the specified vector is perpendicular to this vector within the specified tolerance.

    \param vect [in]  Any 3D vector.
    \param tol [in]  Geometric tolerance.   
  */
  bool isPerpendicularTo(
    const OdGeVector3d& vect,
    const OdGeTol& tol = OdGeContext::gTol) const;
  
  /** \details
    Returns true if and only if the specified vector is perpendicular to this vector within the specified tolerance.

    \param vect [in]  Any 3D vector.
    \param tol [in]  Geometric tolerance.
    \param status [out]  Receives the status of test.

    \remarks
    If the length of either vector is < tol, k0This is returned in status.

    Possible values for status are as follows:

    @untitled table
    kOk
    k0This
    k0Arg1
  */
  bool isPerpendicularTo(
    const OdGeVector3d& vect,
    const OdGeTol& tol, 
    OdGeError& status) const;
    
  /** \details
    Returns the dot product of this vector and the specified vector.
    
    \param vect [in]  Any 3D vector.
  */
  double dotProduct(
    const OdGeVector3d& vect) const
  { return x * vect.x + y * vect.y + z * vect.z; }    
    
  /** \details
    Returns the cross product of this vector and the specified vector.
    
    \param vect [in]  Any 3D vector.
  */
  OdGeVector3d crossProduct(
    const OdGeVector3d& vect) const
  {
    return OdGeVector3d(y * vect.z - z * vect.y, z * vect.x - x * vect.z, x * vect.y - y * vect.x);
  }
    
  // OdGeMatrix3d rotateTo(const OdGeVector3d& vector, const OdGeVector3d& axis
  // = OdGeVector3d::kIdentity) const;

  /** \details
    Returns a 3D vector which is the result of the projection of this vector in given projectDirection into the plane with normal planeNormal passing through the origin.

    \param planeNormal [in]  A 3D vector which is a normal vector of a plane on which this vector is projected.
    \param projectDirection [in]  A projection direction.
  */
  OdGeVector3d project(const OdGeVector3d& planeNormal,
    const OdGeVector3d& projectDirection) const;

  /** \details
    Returns a 3D vector which is the result of the projection of this vector in given projectDirection into the plane with normal planeNormal passing through the origin.

    \param planeNormal [in]  A 3D vector which is a normal vector of a plane on which this vector is projected.
    \param projectDirection [in]  A projection direction.
    \param tol [in]  Geometric tolerance.
    \param flag [in]  A flag that displays if method executed successfully.

    \remarks
    Possible values for flag are as follows:

    @untitled table
    kOk
    k0Arg1
    k0Arg2
    kPerpendicularArg1Arg2
  */
  OdGeVector3d project(const OdGeVector3d& planeNormal,
    const OdGeVector3d& projectDirection, 
    const OdGeTol& tol, OdGeError& flag) const;
  
  /** \details
    Returns a 3D vector which is the result of the orthogonal projection of this vector into the plane with normal planeNormal passing through the origin.
    
    \param planeNormal [in]  A 3D vector which is a normal vector of a plane on which this vector is projected.
  */
  OdGeVector3d orthoProject(const OdGeVector3d& planeNormal) const;
  
  /** \details
    Returns a 3D vector which is the result of the orthogonal projection of this vector into the plane with normal planeNormal passing through the origin.
    
    \param planeNormal [in]  A 3D vector which is a normal vector of a plane on which this vector is projected.
    \param tol [in]  Geometric tolerance.
    \param flag [in]  A flag that displays if method executed successfully.
    
    \remarks
    The flag is set to kOk value if method executed successfully, otherwise it is set to k0Arg1 value.
  */
  OdGeVector3d orthoProject(const OdGeVector3d& planeNormal, 
   const OdGeTol& tol, OdGeError& flag) const;

  /** \details
    Equality operator for the OdGeVector3d class.

    \param vect [in]   A 3D vector to be compared.
    
    \remarks
    Returns true if passed vector is equal to this vector.
  */
  bool operator ==(
    const OdGeVector3d& vect) const;
  
  /** \details
    Inequality operator for the OdGeVector3d class.

    \param vect [in]   A 3D vector to be compared.
    
    \remarks
    Returns true if passed vector is not equal to this vector.
  */
  bool operator !=(
    const OdGeVector3d& vect) const;
    
  /** \details
    Returns true if and only if vect is identical to this vector,
    within the specified tolerance.

    \param vect [in]  Any 3D vector.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo(
    const OdGeVector3d& vect,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details 
    Returns the ith coordinate of this vector.
    
    \param i [in]  Index of coordinate.
    
    \remarks
    * 0 returns the X-coordinate.
    * 1 returns the Y-coordinate.
    * 2 returns the Z-coordinate.
  */
  double operator [](
    unsigned int i) const {return *(&x + i); }
  
  /** \details 
    References the ith coordinate of this vector.
    
    \param i [in]  Index of coordinate.
    
    \remarks
    * 0 references the X-coordinate.
    * 1 references the Y-coordinate.
    * 2 references the Z-coordinate.
  */
  double& operator [](
    unsigned int i) {return *(&x + i); }
  /** \details
    Returns the index of the largest absolute coordinate of this vector.
  */
    unsigned int largestElement() const;
  
  /** \details
    Sets this vector to the specified arguments, 
    and returns a reference to this vector.

    \param xx [in]  X-coordinate.
    \param yy [in]  Y-coordinate.
    \param zz [in]  Z-coordinate.
  */
  OdGeVector3d& set(
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
    Sets this vector to the specified arguments, 
    and returns a reference to this vector.

    \param vect [in]  Any 2D vector.
    \param plane [in]  Any plane.
    
    \remarks
    When called with plane and vector, constructs 
    the 3D vector correspoponding to the 2D vector
    in the coordinates of the plane:
    
            uAxis * vect.x + vAxis * vect.y
    
    where uAxis and vAxis are returned by
    
            plane.get(origin, uAxis, vAxis)
            
    The 3D vector will be parallel to the 2D vector.
  */  
  OdGeVector3d& set(
    const OdGePlanarEnt& plane, 
    const OdGeVector2d& vect);

  /** \details
    Returns the equivalent 3D tranformation matrix.
  */
  operator OdGeMatrix3d() const;
  
/*
  static OdGeVector3d givePerp(
    const OdGeVector2d& vector1, 
    const OdGeVector2d& vector2);
*/

  double x; //  X-coordinate.
  double y; //  Y-coordinate.
  double z; //  Z-coordinate.
};

/**
    \details
    Returns the product of matrix * vect.  
    
    \param matrix [in] Any 3D *matrix* to be multiplied with vector.
    \param vect [in] Any 3D vector to be multiplied with matrix.
*/
GE_TOOLKIT_EXPORT OdGeVector3d operator *(
    const OdGeMatrix3d& matrix, 
    const OdGeVector3d& vect);

/**
    \details
    Returns the product of scale * vect. 
    
    \param scale [in] Scale factor to be multiplied with vector.
    \param vect [in] Any 3D vector to be multiplied with scale factor.
*/
static inline OdGeVector3d operator *(
    double scale, 
    const OdGeVector3d& vect)
{
    return OdGeVector3d(vect.x * scale, vect.y * scale, vect.z * scale);
}


#include "TD_PackPop.h"


#include "GePoint3d.h"
//DOM-IGNORE-BEGIN
inline const OdGePoint3d& OdGeVector3d::asPoint() const
{
  ODA_ASSUME(sizeof(OdGePoint3d) == 3 * sizeof(double));
  ODA_ASSUME(sizeof(OdGeVector3d) == 3 * sizeof(double));
  return *reinterpret_cast<const OdGePoint3d*>(this);
}

inline OdGePoint3d& OdGeVector3d::asPoint()
{
  ODA_ASSUME(sizeof(OdGePoint3d) == 3 * sizeof(double));
  ODA_ASSUME(sizeof(OdGeVector3d) == 3 * sizeof(double));
  return *reinterpret_cast<OdGePoint3d*>(this);
}
//DOM-IGNORE-END
#endif

