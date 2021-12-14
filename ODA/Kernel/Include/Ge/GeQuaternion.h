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



#ifndef OD_GEQUATERNION_H
#define OD_GEQUATERNION_H /*!DOM*/

#include "Ge/GeExport.h"
#include "Ge/GeMatrix3d.h"


#include "TD_PackPush.h"

/** \details
    This class represents a rotation quaternion in 3D space.
    Quaternions provide a convenient mathematical notation for representing orientations and rotations of objects in three dimensions.
    Quaternions resolve the problem of "gimbal locks".
    
    The quaternion consists of 2 components:
    1. Component of rotation (WW)
    2. Components of rotation axis (XX, YY, ZZ) 
    
    To define quaternion components, use following formulas:
    
    <pre>
    XX = x*sin(angle/2)
    YY = y*sin(angle/2)
    ZZ = z*sin(angle/2)
    WW = cos(angle/2),
    </pre>
    
    where angle is angle of desired rotation specified in radians,
    x, y, z - components of a 3D unit vector that represents axis of rotation.
    For example, a quaternion that rotates 1.5 radian by the x axis (unit vector(1,0,0)), looks the following way:
    <pre>
        WW            XX             YY             ZZ
    cos(1.5/2)   1*sin(1.5/2)   0*sin(1.5/2)   0*sin(1.5/2)
    </pre>
    
    Corresponding C++ library: TD_Ge

   <group OdGe_Classes>
*/
class GE_TOOLKIT_EXPORT OdGeQuaternion
{
public:
  
  /** \details
    Default constructor for the OdGeQuaternion class.
    
    \remarks
    The default constructor constructs an identity quaternion (1., 0., 0., 0.).
  */
  OdGeQuaternion()
    : w(1.), x(0.), y(0.), z(0.)
  {}
  
  /** \details
    Constructor for the OdGeQuaternion class.
    
    \param ww [in]  The W-component of this quaternion object.
    \param xx [in]  The X-component of this quaternion object.
    \param yy [in]  The Y-component of this quaternion object.
    \param zz [in]  The Z-component of this quaternion object.
    
    \remarks
    Constructs a quaternion with specified components.
  */
  OdGeQuaternion(double ww, double xx, double yy, double zz)
    : w(ww), x(xx), y(yy), z(zz)
  {}

  /** \details
    Set components for the quaternion and returns a reference to it.
    
    \param ww [in]  The W-component of this quaternion object.
    \param xx [in]  The X-component of this quaternion object.
    \param yy [in]  The Y-component of this quaternion object.
    \param zz [in]  The Z-component of this quaternion object.
  */
  OdGeQuaternion& set (double ww, double xx, double yy, double zz)
  {
    w = ww;
    x = xx;
    y = yy;
    z = zz;
    return *this;
  }

  /** \details
    Transforms passed matrix into the quaternion.
    
    \param matrix [in]  A 3D that defines components of this quaternion.
  */
  OdGeQuaternion& set( const OdGeMatrix3d& matrix )
  {
    double trace = matrix[0][0] + matrix[1][1] + matrix[2][2];
    if ( trace > 0.0 )
    {
      w = sqrt(1.0 + trace) / 2.0;

      double w4 = (4.0 * w);
      x = (matrix[1][2] - matrix[2][1]) / w4;
      y = (matrix[2][0] - matrix[0][2]) / w4;
      z = (matrix[0][1] - matrix[1][0]) / w4;
    }
    else
    {
      OdUInt32 i = 0;
      if (matrix[1][1] > matrix[0][0]) i = 1;
      if (matrix[2][2] > matrix[i][i]) i = 2;
      OdUInt32 j = (2 == i ? 0 : i + 1);
      OdUInt32 k = (2 == j ? 0 : j + 1);

      double s = sqrt((matrix[i][i] - (matrix[j][j] + matrix[k][k])) + 1.0);
      double q[ 4 ];

      q[i] = s * 0.5;
      if (s != 0.0) s = 0.5/s;

      q[3] = (matrix[j][k] - matrix[k][j]) * s;
      q[j] = (matrix[i][j] + matrix[j][i]) * s;
      q[k] = (matrix[i][k] + matrix[k][i]) * s;

      x = q[0];
      y = q[1];
      z = q[2];
      w = q[3];
    }
    return( *this );
  }

  /** \details
    Returns a 3D matrix that represents this quaternion.
  */
  OdGeMatrix3d getMatrix() const
  {
    OdGeMatrix3d matrix;

    matrix( 0, 0 ) = w * w + x * x - y * y - z * z;
    matrix( 1, 0 ) = 2. * ( x * y - w * z );
    matrix( 2, 0 ) = 2. * ( w * y + x * z );

    matrix( 0, 1 ) = 2. * ( w * z + x * y );
    matrix( 1, 1 ) = w * w - x * x + y * y - z * z;
    matrix( 2, 1 ) = 2. * ( y * z - w * x );

    matrix( 0, 2 ) = 2. * ( x * z - w * y );
    matrix( 1, 2 ) = 2. * ( w * x + y * z );
    matrix( 2, 2 ) = w * w - x * x - y * y + z * z;

    return matrix;
  }

  /** \details
    Rotates specified 3D point using this quaternion.
    
    \param sourcePoint [out]  A 3D point that is rotated.
  */
  OdGePoint3d rotate( OdGePoint3d& sourcePoint ) const
  {
    OdGeMatrix3d matrix = getMatrix();

    return sourcePoint.transformBy( matrix );
  }

  /** \details
    Rotates specified 3D vector using this quaternion.
    
    \param vector [out]  A 3D vector that is rotated.
  */
  OdGeVector3d rotate( OdGeVector3d& vector ) const
  {
    OdGeMatrix3d matrix = getMatrix();

    return vector.transformBy( matrix );
  }

  /** \details
    Returns true if and only if quat is identical to this quaternion,
    within the specified tolerance.

    \param quat [in]  Quaternion to be compared to this quaternion.
    \param tol [in]  Geometric tolerance.
  */
  bool isEqualTo(
    const OdGeQuaternion& quat,
    const OdGeTol& tol = OdGeContext::gTol) const
  {
    return fabs(x - quat.x) <= tol.equalVector() &&
           fabs(y - quat.y) <= tol.equalVector() &&
           fabs(z - quat.z) <= tol.equalVector() &&
           fabs(w - quat.w) <= tol.equalVector();
  }

  /** \details
    Equality operator. Returns true if this quaternion is equal to the input quaternion.
    
    \param quat [in] Input quaternion that is checked for equality.
  */
  bool operator == (const OdGeQuaternion& quat) const
  {
    return isEqualTo(quat);
  }
  
  /** \details
    Inequality operator. Returns true if this quaternion is not equal to the input quaternion.
    
    \param quat [in] Input quaternion that is checked for inequality.
  */
  bool operator != (const OdGeQuaternion& quat) const
  {
    return !isEqualTo(quat);
  }

  /** \details
    Multiplication operator for the OdGeQuaternion class.
    
    \param scale [in]  Input scale factor.
    
    \returns
    A new scaled quaternion.
  */
  OdGeQuaternion operator * (
    double scale) const
  {
    return OdGeQuaternion(w * scale, x * scale, y * scale, z * scale);
  }

  /** \details
    Multiplication operator for the OdGeQuaternion class.
    \param scale [in]  Input scale factor.
    
    \returns
    Reference to this OdGeQuaternion object.
    
    \remarks
    Scales this quaternion by scale factor.
  */
  OdGeQuaternion& operator *= (
    double scale)
  {
    w *= scale;
    x *= scale;
    y *= scale;
    z *= scale;
    return *this;
  }

  /** \details
    Division operator for the OdGeQuaternion class.
    
    \param scale [in]  Input scale factor.
    
    \returns
    Quaternion equal to this quaternion divided by the scale factor. 
  */
  OdGeQuaternion operator / (
    double scale) const
  {
    return OdGeQuaternion(w / scale, x / scale, y / scale, z / scale);
  }

  /** \details
    Division operator for the OdGeQuaternion class.
    
    \param scale [in]  Input scale factor.
    
    \returns
    Reference to a quaternion that is divided by the scale factor. 
  */
  OdGeQuaternion& operator /= (
    double scale)
  {
    w /= scale;
    x /= scale;
    y /= scale;
    z /= scale;
    return *this;
  }

  /** \details
    Addition operator for the OdGeQuaternion class.
    
    \param quat [in]  Input quaternion, component values of which are added to component values of this quaternion.
    
    \returns
    A quaternion with summed components.
  */
  OdGeQuaternion operator + (
    const OdGeQuaternion& quat) const
  {
    return OdGeQuaternion(w + quat.w, x + quat.x, y + quat.y, z + quat.z);
  }

  /** \details
    Addition operator for the OdGeQuaternion class.
    
    \param quat [in]  A reference to a quaternion, component values of which are added to component values of this quaternion.
    
    \returns
    Reference to a quaternion with summed components.
  */
  OdGeQuaternion& operator += (
    const OdGeQuaternion& quat)
  {
    w += quat.w;
    x += quat.x,
    y += quat.y;
    z += quat.z;
    return *this;
  }

  /** \details
    Subtraction operator for the OdGeQuaternion class.
    
    \param quat [in]  Input quaternion, component values of which are subtracted from component values of this quaternion.
    
    \returns
    A quaternion with subtracted components.
  */
  OdGeQuaternion operator - (
    const OdGeQuaternion& quat) const
  {
    return OdGeQuaternion(w - quat.w, x - quat.x, y - quat.y, z - quat.z);
  }

  /** \details
    Subtraction operator for the OdGeQuaternion class.
    
    \param quat [in]  A reference to a quaternion, component values of which are subtracted from component values of this quaternion.
    
    \returns
    Reference to a quaternion with subtracted components.
  */
  OdGeQuaternion& operator -= (
    const OdGeQuaternion& quat)
  {
    w -= quat.w;
    x -= quat.x,
    y -= quat.y;
    z -= quat.z;
    return *this;
  }


  /**
    \details
    Inverse operator.
    
    \remarks
    Returns a quaternion with inverse component values according to this quaternion.
  */
  OdGeQuaternion operator - () const { return OdGeQuaternion(-w, -x, -y, -z); }

  /** \details
    Returns the square of the norm of this quaternion.
  */
  double normSqrd() const { return w*w + x*x + y*y + z*z; }

  /** \details
    Returns the norm of this quaternion.
  */
  double norm() const;

  /** \details
    Sets this quaternion to the unit quaternion and returns a reference to this quaternion

    \param tol [in]  Geometric tolerance.

    \remarks
    If this.norm() <= tol, this quaternion is unchanged.
  */
  OdGeQuaternion& normalize(const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Returns the dot product of this quaternion and the specified quaternion.

    \param quat [in]  Any quaternion to be multiplied with this quaternion.
  */
  double dotProduct(const OdGeQuaternion& quat) const
  {
    return w * quat.w + x * quat.x + y * quat.y + z * quat.z;
  }

  /** \details
    Performs spherical linear interpolation, introduced by Ken Shoemake in the context of quaternion interpolation.
    It refers to constant-speed motion along a unit-radius great circle arc, given the ends and an interpolation parameter between 0 and 1
    
    \param                q [in]  second quaternion to be interpolated between (first is this quaternion)
    \param                t [in]  a scalar between 0.0 (at p) and 1.0 (at q)
    \param bUseShortestPath [in]  boolean that indicates whether to compute quaternions that constitute the shortest possible arc on a four-dimensional unit sphere.
  */
  OdGeQuaternion slerp(OdGeQuaternion& q, double t, bool bUseShortestPath);

  /** \details
    Scalar part.
  */
  double w;
  /** \details
    First vector part.
  */
  double x;
  /** \details
    Second vector part.
  */
  double y;
  /** \details
    Third vector part.
  */
  double z;

  /** \details
    Identity quaternion (1, 0, 0, 0), i.e. a quaternion that doesn't cause any rotation.
  */
  GE_STATIC_EXPORT static const OdGeQuaternion kIdentity;

};

#include "TD_PackPop.h"


#endif // OD_GEQUATERNION_H
