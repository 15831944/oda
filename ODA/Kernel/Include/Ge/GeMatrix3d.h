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


#ifndef OD_GE_MATRIX_3D_H
#define OD_GE_MATRIX_3D_H /*!DOM*/

#include "Ge/GeGbl.h"
#include "Ge/GeMatrix2d.h"
#include "Ge/GePoint3d.h"

class OdGeLine3d;
class OdGeVector3d;
class OdGePlane;
class OdGeTol;
class OdGeScale3d;

#include "TD_PackPush.h"

/** \details
  This class represents 3D transformation matrices that define affine
  (translation, rotation and/or scaling) transformations.

  Corresponding C++ library: TD_Ge

  \remarks
  OdGeMatrix3d may be viewed as an array[4][4] of doubles.

  An OdGeMatrix3d, M, can be expressed as a 4 x 4 matrix* in the form:

              <pre>
              a00    a01    a02    t0
              a10    a11    a12    t1
              a20    a21    a22    t2
                0      0      0     1
              </pre>

  The linear part of M is the matrix:
              
              <pre>
              a00    a01    a02
              a10    a11    a12
              a20    a21    a22
              </pre>
              
  The translational part of M is the column:
              
              <pre>
              t0
              t1
              t2
              </pre>
              
  The origin of the coordinate system of M is (t0, t1, t2).

  <group OdGe_Classes>
*/
class GE_TOOLKIT_EXPORT OdGeMatrix3d
{
public:

  /** \details
    Default constructor for objects of the OdGeMatrix3d class.

    \remarks
    Constructs a matrix for 3D transformation operations and sets it to
    identity.
  */
  OdGeMatrix3d();

  //OdGeMatrix3d(const OdGeMatrix3d& src);
  //OdGeMatrix3d& operator =(const OdGeMatrix3d& src);

  /** \details
    Multiplicative identity matrix.
  */
  GE_STATIC_EXPORT static const OdGeMatrix3d kIdentity; // Multiplicative identity matrix.

  // Reset matrix.
  //

  /** \details
    Sets this matrix to the identity matrix, and returns a reference to this
    matrix.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToIdentity();

  /** \details
    Sets any values within tol of 0 to 0.

    \param tol [in] Validation tolerance.

    \remarks
    For correcting in case that Zero is corrupted.
  */
  void validateZero(
    const OdGeTol& tol = OdGeContext::gTol);

  // Multiplication.
  //

  /** \details
    Multiplication operator. Returns the product of this matrix and another
    matrix.

    \param matrix [in] Another matrix to multiply this matrix by.

    \returns Product of this matrix and another matrix.
  */
  OdGeMatrix3d operator *(
    const OdGeMatrix3d& matrix) const;

  /** \details
    Multiplication with assignment operator. Sets this matrix to the product of
    this matrix and another matrix, and returns a reference to this matrix.

    \param matrix [in] Another matrix to multiply this matrix by.

    \returns
    A reference to this OdGeMatrix3d object.
  */
  OdGeMatrix3d& operator *=(
    const OdGeMatrix3d& matrix);

  /** \details
    Sets this matrix to the product of leftSide and this matrix, and returns a
    reference to this matrix.

    \param leftSide [in] 3D matrix that will be multiplied by this matrix.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& preMultBy(
    const OdGeMatrix3d& leftSide);

  /** \details
    Sets this matrix to the product of this matrix and rightSide, and returns a
    reference to this matrix.

    \param rightSide [in] 3D matrix that will be multiplied by this matrix.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& postMultBy(
    const OdGeMatrix3d& rightSide);

  /** \details
    Sets this matrix to the product of matrix1 and matrix2, and returns a
    reference to this matrix.

    \param matrix1 [in] First 3D matrix that is multiplied.
    \param matrix2 [in] Second 3D matrix that is multiplied.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToProduct(
    const OdGeMatrix3d& matrix1,
    const OdGeMatrix3d& matrix2);

  // Multiplicative inverse.
  //

  /** \details
    Sets this matrix to its inversion.

    \returns
    Reference to this matrix.

    \remarks
    Throws eInvalidInput if determinant of this matrix is zero.
  */
  OdGeMatrix3d& invert();

  /** \details
    Calculates inversion of this matrix.

    \returns Inversion of this matrix.

    \remarks
    Throws eInvalidInput if determinant of this matrix is zero.
  */
  OdGeMatrix3d inverse() const;
  /** \details
    Calculates inversion of this matrix.

    \param tol [in] Tolerance value.

    \returns Inverse of this matrix.

    \remarks
    Throws eInvalidInput if determinant of this matrix is less than tol.equalPoint().
  */
  OdGeMatrix3d inverse(
    const OdGeTol& tol) const;

  /** \details
    Retrieves the inverse of this matrix.
    
    \param inverseMatrix [out] Receives the inverse matrix.
    \param tol            [in] Tolerance value. 

    \returns true if this matrix has an inverse; false otherwise.

    \remarks
    Matrix has an inverse if determinant of this matrix is not zero within tol.
  */
  bool inverse(
    OdGeMatrix3d& inverseMatrix,
    double tol) const;

  /** \details
    Checks whether this matrix is singular.

    \param tol [in]  Geometric tolerance.
    \returns
    true if this matrix is singular, false otherwise.
    \remarks
    * A matrix is singular if and only if its determinant is zero within the specified tolerance.
    * A singular matrix cannot be inverted.
  */
  bool isSingular(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Sets this matrix to its transpose, and returns a reference to this matrix.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& transposeIt();

  /** \details
    Returns the transpose of this matrix.

    \returns Transpose of this matrix.
  */
  OdGeMatrix3d transpose() const;

  /** \details
    Comparison operator.

    \param matrix [in] Matrix to compare with this matrix.

    \returns true if compared matrices are equal; false otherwise.
  */
  bool operator ==(
    const OdGeMatrix3d& matrix) const;

  /** \details
    Negated comparison operator.

    \param matrix [in] Matrix to compare with this matrix.

    \returns true if compared matrices are not equal; false otherwise.
  */
  bool operator !=(
    const OdGeMatrix3d& matrix) const;

  /** \details
    Returns true if and only if another matrix is identical to this one within
    the specified tolerance.

    \param matrix [in] Matrix to compare with this matrix.
    \param tol    [in] Geomentric tolerance.

    \returns true if this matrix is identical to another matrix; false
    otherwise.
  */
  bool isEqualTo(
    const OdGeMatrix3d& matrix,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the columns vectors of the linear part of this
    matrix are of equal length and mutually perpendicular within the specified
    tolerance.

    \param tol [in] Geomentric tolerance.

    \returns true if the columns vectors of the linear part of this matrix are
    of equal length and mutually perpendicular; false otherwise.
  */
  bool isUniScaledOrtho(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Checks whether the column vectors of the linear part of this
    matrix are mutually perpendicular within the specified tolerance.

    \param tol [in] Geomentric tolerance.

    \returns true if the columns vectors of the linear part of this matrix are
    mutually perpendicular; false otherwise.
  */
  bool isScaledOrtho(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Indicates whether the matrix can be used for perspective projections.

    \param tol [in] Geomentric tolerance.

    \returns true if the matrix can be used for perspective projections; false
    otherwise.

    \remarks
    Returns true if [3][0] or [3][1] or [3][2] elements have non-zero values
    considering passed tolerance.
  */
  bool isPerspective(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the determinant of this matrix.

    \returns Determinant value.
  */
  double det() const;

  /** \details
    Sets the translation part of the matrix to the specified vector.

    \param vect [in] Translation vector.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setTranslation(
    const OdGeVector3d& vect);

  /** \details
    Sets this matrix to the matrix which maps the WCS to the coordinate system
    defined by origin, X-axis, Y-axis and Z-axis, and returns a reference to
    this matrix.

    \param origin [in] Origin of the coordinate system.
    \param xAxis  [in] X-axis of the coordinate system.
    \param yAxis  [in] Y-axis of the coordinate system.
    \param zAxis  [in] Z-axis of the coordinate system.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setCoordSystem(
    const OdGePoint3d& origin,
    const OdGeVector3d& xAxis,
    const OdGeVector3d& yAxis,
    const OdGeVector3d& zAxis);

  /** \details
    Returns the origin, X-axis, Y-axis and Z-axis of the coordinate system to
    which this matrix maps the WCS.

    \param origin [out] Receives the origin of the coordinate system.
    \param xAxis  [out] Receives the X-axis of the coordinate system.
    \param yAxis  [out] Receives the Y-axis of the coordinate system.
    \param zAxis  [out] Receives the Z-axis of the coordinate system.
  */
  void getCoordSystem(
    OdGePoint3d& origin,
    OdGeVector3d& xAxis,
    OdGeVector3d& yAxis,
    OdGeVector3d& zAxis) const;

  /** \details
    Returns the origin of the coordinate system to which this matrix maps the
    WCS.

    \returns Origin of the coordinate system.
  */
  OdGePoint3d getCsOrigin() const;

  /** \details
    Returns the X-axis of the coordinate system to which this matrix maps the
    WCS.

    \returns X-axis of the coordinate system.
  */
  OdGeVector3d getCsXAxis() const;

  /** \details
    Returns the Y-axis of the coordinate system to which this matrix maps the
    WCS.

    \returns Y-axis of the coordinate system.
  */
  OdGeVector3d getCsYAxis() const;

  /** \details
    Returns the Z-axis of the coordinate system to which this matrix maps the
    WCS.

    \returns Z-axis of the coordinate system.
  */
  OdGeVector3d getCsZAxis() const;

  /** \details
    Sets this matrix to the matrix which is translated by a translation vector
    and returns a reference to this matrix.

    \param vect [in] Translation vector.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToTranslation(
    const OdGeVector3d& vect);

  /** \details
    Sets this matrix to the matrix which is rotated by a specified angle about
    an axis passing through the center, and returns a reference to this matrix.

    \param angle  [in] Rotation angle.
    \param axis   [in] Rotation axis.
    \param center [in] Center of rotation.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToRotation(
    double angle,
    const OdGeVector3d& axis,
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Sets this matrix to the matrix which is scaled by a scale factor about the
    center of scaling, and returns a reference to this matrix.

    \param scale  [in] Scale factor.
    \param center [in] Center of scaling.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToScaling(
    double scale,
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Sets this matrix to the matrix which is scaled by a scale factor about the
    center of scaling, and returns a reference to this matrix.

    \param scale  [in] Scale factor.
    \param center [in] Center of scaling.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToScaling(
    const OdGeScale3d& scale,
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Sets this matrix to the matrix which is mirrored about the specified object
    and returns a reference to this matrix.

    \param mirrorPlane [in] Mirror plane.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToMirroring(
    const OdGePlane& mirrorPlane);

  /** \details
    Sets this matrix to the matrix which is mirrored about the specified object
    and returns a reference to this matrix.

    \param mirrorPoint [in] Mirror point.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToMirroring(
    const OdGePoint3d& mirrorPoint);

  /** \details
    Sets this matrix to the matrix which is mirrored about the specified object
    and returns a reference to this matrix.

    \param mirrorLine [in] Mirror line entity.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToMirroring(
    const OdGeLine3d& mirrorLine);
    
  /** \details
    Sets this matrix to the matrix which is parallely projected onto the
    specified plane in the specified direction and returns a reference to this
    matrix.

    \param projectionPlane [in] Projection plane.
    \param projectDir      [in] Projection direction.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToProjection(
    const OdGePlane& projectionPlane,
    const OdGeVector3d& projectDir);

  /** \details
    Sets this matrix to the matrix which maps the initial coordinate system
    defined by fromOrigin, fromXAxis, fromYAxis and fromZAxis, to the
    destination coordinate system defined by toOrigin, toXAxis, toYAxis and
    toZAxis, and returns a reference to this matrix.

    \param fromOrigin [in] Origin of the initial coordinate system.
    \param fromXAxis  [in] X-axis of the initial coordinate system.
    \param fromYAxis  [in] Y-axis of the initial coordinate system.
    \param fromZAxis  [in] Z-axis of the initial coordinate system.
    \param toOrigin   [in] Origin of the destination coordinate system.
    \param toXAxis    [in] X-axis of the destination coordinate system.
    \param toYAxis    [in] Y-axis of the destination coordinate system.
    \param toZAxis    [in] Z-axis of the destination coordinate system.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToAlignCoordSys(
    const OdGePoint3d& fromOrigin,
    const OdGeVector3d& fromXAxis,
    const OdGeVector3d& fromYAxis,
    const OdGeVector3d& fromZAxis,
    const OdGePoint3d& toOrigin,
    const OdGeVector3d& toXAxis,
    const OdGeVector3d& toYAxis,
    const OdGeVector3d& toZAxis);


  /** \details
    Sets this matrix to the matrix which maps the WCS to the plane coordinate
    system, and returns a reference to this matrix.

    \param normal [in] Normal to the plane.

    \returns Reference to this matrix.

    \remarks
    If the normal is specified, the origin of the plane is (0,0,0).
  */
  OdGeMatrix3d& setToWorldToPlane(
    const OdGeVector3d& normal);

  /** \details
    Sets this matrix to the matrix which maps the WCS to the plane coordinate
    system, and returns a reference to this matrix.

    \param plane [in] Plane.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToWorldToPlane(
    const OdGePlane& plane);

  /** \details
    Sets this matrix to the matrix which maps the plane coordinate system to the
    WCS, and returns a reference to this matrix.

    \param normal [in] Normal to the plane.

    \returns Reference to this matrix.

    \remarks
    If the normal is specified, the origin of the plane is (0,0,0).
  */
  OdGeMatrix3d& setToPlaneToWorld(
    const OdGeVector3d& normal);

  /** \details
    Sets this matrix to the matrix which maps the plane coordinate system to the
    WCS, and returns a reference to this matrix.

    \param plane [in] Plane.

    \returns Reference to this matrix.
  */
  OdGeMatrix3d& setToPlaneToWorld(
    const OdGePlane& plane);

  /** \details
    Returns the matrix of the translation by a vector.

    \param vect [in] Translation vector.

    \returns Matrix of the translation by the vector.
  */
  static OdGeMatrix3d translation(
    const OdGeVector3d& vect);

  /** \details
    Returns the translation vector of this matrix.

    \returns Translation vector.
  */
  OdGeVector3d translation() const;

  /** \details
    Returns the matrix that rotates entities by an angle about an axis passing
    through the center.

    \param angle  [in] Rotation angle.
    \param axis   [in] Rotation axis.
    \param center [in] Center of rotation.

    \returns Rotation matrix.
  */
  static OdGeMatrix3d rotation(
    double angle,
    const OdGeVector3d& axis,
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Returns the matrix that scales entities by a scale factor relatively to the
    center.

    \param scale  [in] Scale factor.
    \param center [in] Center of scaling.

    \returns Scaling matrix.
  */
  static OdGeMatrix3d scaling(
    double scale,
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Returns the matrix that scales entities by a scale factor relatively to the
    center.

    \param scale  [in] Scale factor.
    \param center [in] Center of scaling.

    \returns Scaling matrix.
  */
  static OdGeMatrix3d scaling(
    const OdGeScale3d& scale, 
    const OdGePoint3d& center = OdGePoint3d::kOrigin);

  /** \details
    Returns the matrix that mirrors entities across the specified plane.

    \param mirrorPlane [in] Mirror plane.

    \returns Mirroring matrix.
  */
  static OdGeMatrix3d mirroring(
    const OdGePlane& mirrorPlane);

  /** \details
    Returns the matrix taht mirrors entities across the specified point.

    \param mirrorPoint [in] Mirror point.

    \returns Mirroring matrix.
  */  
  static OdGeMatrix3d mirroring(
    const OdGePoint3d& mirrorPoint);

  /** \details
    Returns the matrix that mirrors entities across the specified line.

    \param mirrorLine [in] Mirror line entity.

    \returns Mirroring matrix.
  */
  static OdGeMatrix3d mirroring(
    const OdGeLine3d& mirrorLine);

  /** \details
    Returns the matrix that parallely projects entities onto the specified plane
    in the specified direction.

    \param projectionPlane [in] Projection plane.
    \param projectDir      [in] Projection direction.

    \returns Projection matrix.
  */
  static OdGeMatrix3d projection(
    const OdGePlane& projectionPlane,
    const OdGeVector3d& projectDir);

  /** \details
    Returns the matrix which maps the initial coordinate system defined by
    fromOrigin, fromXAxis, fromYAxis and fromZAxis, to the destination
    coordinate system defined by toOrigin, toXAxis, toYAxis and toZAxis.

    \param fromOrigin [in] Origin of the initial coordinate system.
    \param fromXAxis  [in] X-axis of the initial coordinate system.
    \param fromYAxis  [in] Y-axis of the initial coordinate system.
    \param fromZAxis  [in] Z-axis of the initial coordinate system.
    \param toOrigin   [in] Origin of the destination coordinate system.
    \param toXAxis    [in] X-axis of the destination coordinate system.
    \param toYAxis    [in] Y-axis of the destination coordinate system.
    \param toZAxis    [in] Z-axis of the destination coordinate system.

    \returns Transformation matrix.
  */
  static OdGeMatrix3d alignCoordSys(
    const OdGePoint3d& fromOrigin,
    const OdGeVector3d& fromXAxis,
    const OdGeVector3d& fromYAxis,
    const OdGeVector3d& fromZAxis,
    const OdGePoint3d& toOrigin,
    const OdGeVector3d& toXAxis,
    const OdGeVector3d& toYAxis,
    const OdGeVector3d& toZAxis);

  /** \details
    Returns the matrix which maps the WCS to the plane coordinate system.

    \param normal [in] Normal to the plane.

    \returns Transformation matrix.

    \remarks
    If normal is specified, the origin of the plane is (0,0,0).
  */
  static OdGeMatrix3d worldToPlane(
    const OdGeVector3d& normal);

  /** \details
    Returns the matrix which maps the WCS to the plane coordinate system.

    \param plane [in] Plane.

    \returns Transformation matrix.
  */
  static OdGeMatrix3d worldToPlane(
    const OdGePlane& plane);

  /** \details
    Returns the matrix which maps the plane coordinate system to the WCS.

    \param normal [in] Normal to the plane.

    \returns Transformation matrix.

    \remarks
    If normal is specified, the origin of the plane is (0,0,0).
  */
  static OdGeMatrix3d planeToWorld(
    const OdGeVector3d& normal);

  /** \details
    Returns the matrix which maps the plane coordinate system to the WCS.

    \param plane [in] Plane.

    \returns Transformation matrix.
  */
  static OdGeMatrix3d planeToWorld(
    const OdGePlane& plane);

  /** \details
    Returns the scale factor of this matrix.

    \returns Scale factor.

    \remarks
    The scale factor is the square root of the longest column vector of the
    linear part of this matrix.
  */
  double scale() const;

  /** \details
    Returns the largest absolute value of the linear part of this matrix.

    \returns The largest absolute value of the linear part of this matrix.
  */
  double norm() const;

  /** \details
    Returns the matrix which transforms the plane specified by a normal and an
    elevation.

    \param normal    [in/out] Normal to the plane.
    \param elevation [in/out] Elevation of the plane.

    \returns Transformation matrix.

    \remarks
    Returns the normal and elevation of the transformed plane, as well as a 2D
    transformation matrix for transforming points in the original plane
    coordinates to the transformed plane coordinates.
  */
  OdGeMatrix2d convertToLocal(
    OdGeVector3d& normal,
    double& elevation) const;

  // For convenient access to the data.
  //

  /** \details
    Returns matrix.entry[row] as matrix[row].

    \param row [in] Row.

    \returns Pointer to a constant matrix row.
  */
  const double* operator [](
    int row) const;

  /** \details
    References matrix.entry[row] as matrix[row].

    \param row [in] Row.

    \returns Pointer to the matrix row.
  */
  double* operator [](
    int row);

  /** \details
    Returns matrix.entry[row][column] as matrix(row,column).

    \param row    [in] Row.
    \param column [in] Column.

    \returns Matrix element.
  */
  double operator ()(
    int row,
    int column) const;

  /** \details
    References matrix.entry[row][column] as matrix(row,column).

    \param row    [in] Row.
    \param column [in] Column.

    \returns Reference to the matrix element.
  */
  double& operator ()(
    int row,
    int column);

  /** \details
    Matrix data by rows.
  */
  double entry[4][4];
  //DOM-IGNORE-END
};

// these operations really decrease rendering performance in non-inline case

/** \details
  Returns a pointer to a constant matrix row.

  \param row [in] Row of a matrix. Must be in range from 0 to 3.

  \returns Pointer to a constant matrix row.
*/
inline const double* OdGeMatrix3d::operator [](int row) const
{
  return entry[row];
}

/** \details
  Returns a pointer to a matrix row.

  \param row [in] Row of a matrix. Must be in range from 0 to 3.

  \returns Pointer to the matrix row.
*/
inline double* OdGeMatrix3d::operator [](int row)
{
  return entry[row];
}

/** \details
  Returns a matrix element specified by the row and column.

  \param row    [in] Row of a matrix. Must be in range from 0 to 3.
  \param column [in] Column of a matrix. Must be in range from 0 to 3.

  \returns Matrix element.
*/
inline double OdGeMatrix3d::operator ()(int row, int column) const
{
  return entry[row][column];
}

/** \details
  Returns a reference to a matrix element specified by the row and column.

  \param row    [in] Row of a matrix. Must be in range from 0 to 3.
  \param column [in] Column of a matrix. Must be in range from 0 to 3.

  \returns Reference to the matrix element.
*/
inline double& OdGeMatrix3d::operator ()(int row, int column)
{
  return entry[row][column];
}

#include "TD_PackPop.h"

#endif // OD_GE_MATRIX_3D_H
