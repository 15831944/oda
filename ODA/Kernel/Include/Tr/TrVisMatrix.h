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
// Matrix definition

#ifndef ODTRVISMATRIX
#define ODTRVISMATRIX

#include "TD_PackPush.h"

#include "TrVisVector.h"

#include "Ge/GeMatrix3d.h"
#include "OdArray.h"

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
template <typename DataType, typename VectorType>
struct OdTrVisMatDefVecTypeAdaptor
{
  const VectorType &_vec;
  OdTrVisMatDefVecTypeAdaptor(const VectorType &vec) : _vec(vec) {}
  DataType operator [](int nAxis) const { return (DataType)((!nAxis) ? _vec.x : ((nAxis == 1) ? _vec.y : _vec.z)); }
};

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
template <typename DataType>
struct OdTrVisMatrix3x3Impl
{
  // Represent data as rows and columns.
  DataType entry[3][3];
  // We doesn't provide constructors and copy operators. Initially data willn't be initialized.
  // Instead of constructors we provide setters which return reference to our object.
  typedef DataType EntryType;
  struct Copier
  { static inline void copy(DataType *pTo, const DataType *pFrom)
    { pTo[0] = pFrom[0]; pTo[1] = pFrom[1]; pTo[2] = pFrom[2]; }
  };
  // Set from linear array.
  OdTrVisMatrix3x3Impl &set(const DataType *pVals)
  {
    Copier::copy(entry[0], pVals);
    Copier::copy(entry[1], pVals + 3);
    Copier::copy(entry[2], pVals + 6);
    return *this;
  }
  // Set from function arguments.
  OdTrVisMatrix3x3Impl &set(DataType e00 = 1.0f, DataType e01 = 0.0f, DataType e02 = 0.0f,
                            DataType e10 = 0.0f, DataType e11 = 1.0f, DataType e12 = 0.0f,
                            DataType e20 = 0.0f, DataType e21 = 0.0f, DataType e22 = 1.0f)
  {
    entry[0][0] = e00; entry[0][1] = e01; entry[0][2] = e02;
    entry[1][0] = e10; entry[1][1] = e11; entry[1][2] = e12;
    entry[2][0] = e20; entry[2][1] = e21; entry[2][2] = e22;
    return *this;
  }
  // Set to identity.
  OdTrVisMatrix3x3Impl &setIdentity()
  {
    entry[0][0] = 1.0f; entry[0][1] = 0.0f; entry[0][2] = 0.0f;
    entry[1][0] = 0.0f; entry[1][1] = 1.0f; entry[1][2] = 0.0f;
    entry[2][0] = 0.0f; entry[2][1] = 0.0f; entry[2][2] = 1.0f;
    return *this;
  }
  // Getters.
  const DataType *getArray(int nRow = 0) const { return entry[nRow]; }
  DataType *getArray(int nRow = 0) { return entry[nRow]; }
  DataType operator()(int i, int j) const { return entry[i][j]; }
  // Rows/columns accessor.
  OdTrVisMiniVec3d<DataType> row(int n) const
  { return OdTrVisMiniVec3d<DataType>(entry[n][0], entry[n][1], entry[n][2]); }
  OdTrVisMiniVec3d<DataType> column(int n) const
  { return OdTrVisMiniVec3d<DataType>(entry[0][n], entry[1][n], entry[2][n]); }
  // Matrix multiplication.
  OdTrVisMatrix3x3Impl operator *(const OdTrVisMatrix3x3Impl &m) const
  { OdTrVisMatrix3x3Impl rv;
    for (int i = 0; i < 3; i++)
    { for (int j = 0; j < 3; j++)
        rv.entry[i][j] = entry[i][0] * m.entry[0][j] + entry[i][1] * m.entry[1][j] + entry[i][2] * m.entry[2][j];
    }
    return rv;
  }
  OdTrVisMatrix3x3Impl &operator *=(const OdTrVisMatrix3x3Impl &m)
  { return *this = *this * m; }
  // To be completely correct we must have inverse multiplication.
  OdTrVisMatrix3x3Impl operator ^(const OdTrVisMatrix3x3Impl &m) const
  { OdTrVisMatrix3x3Impl rv;
    for (int i = 0; i < 3; i++)
    { for (int j = 0; j < 3; j++)
        rv.entry[i][j] = entry[0][i] * m.entry[0][j] + entry[1][i] * m.entry[1][j] + entry[2][i] * m.entry[2][j];
    }
    return rv;
  }
  OdTrVisMatrix3x3Impl &operator ^=(const OdTrVisMatrix3x3Impl &m)
  { return *this = *this ^ m; }
  // Inverse operator.
  OdTrVisMatrix3x3Impl &invert(const DataType eps = 1.e-8f)
  { const DataType det = entry[0][0] * (entry[1][1] * entry[2][2] - entry[2][1] * entry[1][2]) -
                         entry[0][1] * (entry[1][0] * entry[2][2] - entry[1][2] * entry[2][0]) +
                         entry[0][2] * (entry[1][0] * entry[2][1] - entry[1][1] * entry[2][0]);
    if (det < eps)
      transpose();
    else
    { const OdTrVisMatrix3x3Impl src(*this);
      const DataType invdet = 1.0f / det;
      entry[0][0] = (src.entry[1][1] * src.entry[2][2] - src.entry[2][1] * src.entry[1][2]) * invdet;
      entry[0][1] = (src.entry[0][2] * src.entry[2][1] - src.entry[0][1] * src.entry[2][2]) * invdet;
      entry[0][2] = (src.entry[0][1] * src.entry[1][2] - src.entry[0][2] * src.entry[1][1]) * invdet;
      entry[1][0] = (src.entry[1][2] * src.entry[2][0] - src.entry[1][0] * src.entry[2][2]) * invdet;
      entry[1][1] = (src.entry[0][0] * src.entry[2][2] - src.entry[0][2] * src.entry[2][0]) * invdet;
      entry[1][2] = (src.entry[1][0] * src.entry[0][2] - src.entry[0][0] * src.entry[1][2]) * invdet;
      entry[2][0] = (src.entry[1][0] * src.entry[2][1] - src.entry[2][0] * src.entry[1][1]) * invdet;
      entry[2][1] = (src.entry[2][0] * src.entry[0][1] - src.entry[0][0] * src.entry[2][1]) * invdet;
      entry[2][2] = (src.entry[0][0] * src.entry[1][1] - src.entry[1][0] * src.entry[0][1]) * invdet;
    }
    return *this;
  }
  OdTrVisMatrix3x3Impl operator ~() const
  { return OdTrVisMatrix3x3Impl(*this).invert(); }
  // Transpose operator.
  OdTrVisMatrix3x3Impl &transpose()
  { DataType *src = getArray();
    std::swap(src[1], src[3]);
    std::swap(src[2], src[6]);
    std::swap(src[5], src[7]);
    return *this;
  }
  OdTrVisMatrix3x3Impl operator -() const
  { return OdTrVisMatrix3x3Impl(*this).transpose(); }
  // Scale operators.
  template <typename VectorType>
  static OdTrVisMatrix3x3Impl &scaleMatrix(OdTrVisMatrix3x3Impl &m, const VectorType &v, bool bInverse = false)
  { m.setIdentity();
    if (!bInverse)
      m.entry[0][0] = v[0], m.entry[1][1] = v[1], m.entry[2][2] = v[2];
    else
      m.entry[0][0] = 1.0f / v[0], m.entry[1][1] = 1.0f / v[1], m.entry[2][2] = 1.0f / v[2];
    return m;
  }
  template <typename VectorType>
  OdTrVisMatrix3x3Impl scale(const VectorType &v) const
  { OdTrVisMatrix3x3Impl s;
    return *this * scaleMatrix(s, v);
  }
  template <typename VectorType>
  OdTrVisMatrix3x3Impl &setScale(const VectorType &v)
  { OdTrVisMatrix3x3Impl s;
    return *this * scaleMatrix(s, v);
  }
  template <typename VectorType>
  OdTrVisMatrix3x3Impl recipScale(const VectorType &v) const
  { OdTrVisMatrix3x3Impl s;
    return *this * scaleMatrix(s, v, true);
  }
  template <typename VectorType>
  OdTrVisMatrix3x3Impl &setRecipScale(const VectorType &v)
  { OdTrVisMatrix3x3Impl s;
    return *this * scaleMatrix(s, v, true);
  }
  // Orthogonal scale.
  static OdTrVisMatrix3x3Impl &scaleMatrix(OdTrVisMatrix3x3Impl &m, DataType s, bool bInverse = false)
  { m.setIdentity();
    if (!bInverse)
      m.entry[0][0] = m.entry[1][1] = m.entry[2][2] = s;
    else
      m.entry[0][0] = m.entry[1][1] = m.entry[2][2] = 1.0f / s;
    return m;
  }
  friend OdTrVisMatrix3x3Impl operator *(const OdTrVisMatrix3x3Impl &m, DataType n)
  { OdTrVisMatrix3x3Impl s;
    return m * scaleMatrix(s, n);
  }
  friend OdTrVisMatrix3x3Impl operator *(DataType n, const OdTrVisMatrix3x3Impl &m)
  { OdTrVisMatrix3x3Impl s;
    return scaleMatrix(s, n) * m;
  }
  OdTrVisMatrix3x3Impl &operator *=(DataType n)
  { OdTrVisMatrix3x3Impl s;
    return *this *= scaleMatrix(s, n);
  }
  friend OdTrVisMatrix3x3Impl operator /(const OdTrVisMatrix3x3Impl &m, DataType n)
  { OdTrVisMatrix3x3Impl s;
    return m * scaleMatrix(s, n, true);
  }
  friend OdTrVisMatrix3x3Impl operator /(DataType n, const OdTrVisMatrix3x3Impl &m)
  { OdTrVisMatrix3x3Impl s;
    return scaleMatrix(s, n, true) * m;
  }
  OdTrVisMatrix3x3Impl &operator /=(DataType n)
  { OdTrVisMatrix3x3Impl s;
    return *this *= scaleMatrix(s, n, true);
  }
  // Rotation operators.
  template <typename VectorType>
  static OdTrVisMatrix3x3Impl &localRotationMatrix(OdTrVisMatrix3x3Impl &m, const VectorType &angles)
  { // Performs rotation in local coordinates system.
    const DataType cos_b = (DataType)cos(angles[0]), sin_b = (DataType)sin(angles[0]);
    const DataType cos_c = (DataType)cos(angles[1]), sin_c = (DataType)sin(angles[1]);
    const DataType cos_a = (DataType)cos(angles[2]), sin_a = (DataType)sin(angles[2]);
    // First matrix row
    m.entry[0][0] = cos_a * cos_c - sin_a * sin_b * sin_c;
    m.entry[0][1] = sin_a * cos_c + cos_a * sin_b * sin_c;
    m.entry[0][2] = cos_b * sin_c;
    // Second matrix row
    m.entry[1][0] = -sin_a * cos_b;
    m.entry[1][1] = cos_a * cos_b;
    m.entry[1][2] = -sin_b;
    // Third matrix row
    m.entry[2][0] = -cos_a * sin_c - sin_a * sin_b * cos_c;
    m.entry[2][1] = -sin_a * sin_c + cos_a * sin_b * cos_c;
    m.entry[2][2] = cos_b * cos_c;
    return m;
  }
  template <typename VectorType>
  OdTrVisMatrix3x3Impl &localRotate(const VectorType &angles)
  { OdTrVisMatrix3x3Impl r;
    return *this *= localRotationMatrix(r, angles); }
  // This is OpenGL-compatible version of rotation.
  template <typename VectorType>
  static OdTrVisMatrix3x3Impl &rotationMatrix(OdTrVisMatrix3x3Impl &m, DataType angle, const VectorType &v, const DataType eps = 1.e-8f)
  { DataType xx, yy, zz, xy, yz, zx, xs, ys, zs;
    const DataType x = v[0], y = v[1], z = v[2];
    const DataType s = (DataType)sin(-angle), c = (DataType)cos(-angle);
    const DataType mag = (DataType)sqrt(x * x + y * y + z * z);
    if (mag < eps)
      return m.setIdentity();
    x /= mag; y /= mag; z /= mag;

    xx = x * x; yy = y * y; zz = z * z;
    xy = x * y; yz = y * z; zx = z * x;
    xs = x * s; ys = y * s; zs = z * s;

    const DataType one_c = 1.0f - c;
    DataType *pMat = m.getArray();

    pMat[0] = (one_c * xx) + c;
    pMat[1] = (one_c * xy) - zs;
    pMat[2] = (one_c * zx) + ys;

    pMat[3] = (one_c * xy) + zs;
    pMat[4] = (one_c * yy) + c;
    pMat[5] = (one_c * yz) - xs;

    pMat[6] = (one_c * zx) - ys;
    pMat[7] = (one_c * yz) + xs;
    pMat[8] = (one_c * zz) + c;

    return m;
  }
  template <typename VectorType>
  OdTrVisMatrix3x3Impl &rotate(DataType angle, const VectorType &v, const DataType eps = 1.e-8f)
  { OdTrVisMatrix3x3Impl r;
    return *this *= rotationMatrix(r, angle, v, eps); }
  // Additional scaling operators.
  void removeScale()
  {
    const DataType x = fabs(entry[0][0]) + fabs(entry[0][1]) + fabs(entry[0][2]);
    const DataType y = fabs(entry[1][0]) + fabs(entry[1][1]) + fabs(entry[1][2]);
    const DataType z = fabs(entry[2][0]) + fabs(entry[2][1]) + fabs(entry[2][2]);
    entry[0][0] /= x; entry[0][1] /= x; entry[0][2] /= x;
    entry[1][0] /= y; entry[1][1] /= y; entry[1][2] /= y;
    entry[2][0] /= z; entry[2][1] /= z; entry[2][2] /= z;
  }
  OdTrVisMiniVec3d<DataType> getScale() const
  { return OdTrVisMiniVec3d<DataType>((DataType)(fabs(entry[0][0]) + fabs(entry[0][1]) + fabs(entry[0][2])),
                                      (DataType)(fabs(entry[1][0]) + fabs(entry[1][1]) + fabs(entry[1][2])),
                                      (DataType)(fabs(entry[2][0]) + fabs(entry[2][1]) + fabs(entry[2][2]))); }
  // Vector transformation operators
  template <typename VectorType>
  VectorType &transform(VectorType &vec, DataType /*w*/ = 0.0f) const
  { return vec.set(entry[0][0] * vec.x + entry[1][0] * vec.y + entry[2][0] * vec.z,
                   entry[0][1] * vec.x + entry[1][1] * vec.y + entry[2][1] * vec.z,
                   entry[0][2] * vec.x + entry[1][2] * vec.y + entry[2][2] * vec.z); }
  template <typename VectorType>
  VectorType transformClone(const VectorType &vec, DataType /*w*/ = 0.0f) const
  { VectorType tmp(vec); return transform(tmp); }
  template <typename VectorType>
  friend VectorType operator *(const OdTrVisMatrix3x3Impl m, const VectorType &v)
  { VectorType vec(v); return m.transform(vec); }
  template <typename VectorType>
  friend VectorType operator *(const VectorType &v, const OdTrVisMatrix3x3Impl m)
  { VectorType vec(v); return m.transform(vec); }
  // Inverse transformation
  template <typename VectorType>
  VectorType &untransform(VectorType &vec, DataType /*w*/ = 0.0f) const
  { return vec.set(entry[0][0] * vec.x + entry[0][1] * vec.y + entry[0][2] * vec.z,
                   entry[1][0] * vec.x + entry[1][1] * vec.y + entry[1][2] * vec.z,
                   entry[2][0] * vec.x + entry[2][1] * vec.y + entry[2][2] * vec.z); }
  template <typename VectorType>
  VectorType untransformClone(const VectorType &vec, DataType /*w*/ = 0.0f) const
  { VectorType tmp(vec); return untransform(tmp); }
  template <typename VectorType>
  friend VectorType operator ^(const OdTrVisMatrix3x3Impl m, const VectorType &v)
  { VectorType vec(v); return m.untransform(vec); }
  template <typename VectorType>
  friend VectorType operator ^(const VectorType &v, const OdTrVisMatrix3x3Impl m)
  { VectorType vec(v); return m.untransform(vec); }
};

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
template <typename DataType>
struct OdTrVisMatrix4x4Impl
{
  // Represent data as rows and columns.
  DataType entry[4][4];
  // We doesn't provide constructors and copy operators. Initially data willn't be initialized.
  // Instead of constructors we provide setters which return reference to our object.
  typedef DataType EntryType;
  struct Copier
  { static void copy(DataType *pTo, const DataType *pFrom)
    { pTo[0] = pFrom[0]; pTo[1] = pFrom[1]; pTo[2] = pFrom[2]; pTo[3] = pFrom[3]; }
  };
  // Set from linear array.
  OdTrVisMatrix4x4Impl &set(const DataType *pVals)
  {
    Copier::copy(entry[0], pVals);
    Copier::copy(entry[1], pVals + 4);
    Copier::copy(entry[2], pVals + 8);
    Copier::copy(entry[3], pVals + 12);
    return *this;
  }
  // Set from Ge matrix.
  OdTrVisMatrix4x4Impl &set(const OdGeMatrix3d &xMat, bool bTransposed = true)
  {
    if (!bTransposed)
    {
      entry[0][0] = (DataType)xMat.entry[0][0]; entry[0][1] = (DataType)xMat.entry[0][1];
      entry[0][2] = (DataType)xMat.entry[0][2]; entry[0][3] = (DataType)xMat.entry[0][3];
      entry[1][0] = (DataType)xMat.entry[1][0]; entry[1][1] = (DataType)xMat.entry[1][1];
      entry[1][2] = (DataType)xMat.entry[1][2]; entry[1][3] = (DataType)xMat.entry[1][3];
      entry[2][0] = (DataType)xMat.entry[2][0]; entry[2][1] = (DataType)xMat.entry[2][1];
      entry[2][2] = (DataType)xMat.entry[2][2]; entry[2][3] = (DataType)xMat.entry[2][3];
      entry[3][0] = (DataType)xMat.entry[3][0]; entry[3][1] = (DataType)xMat.entry[3][1];
      entry[3][2] = (DataType)xMat.entry[3][2]; entry[3][3] = (DataType)xMat.entry[3][3];
    }
    else
    {
      entry[0][0] = (DataType)xMat.entry[0][0]; entry[1][0] = (DataType)xMat.entry[0][1];
      entry[2][0] = (DataType)xMat.entry[0][2]; entry[3][0] = (DataType)xMat.entry[0][3];
      entry[0][1] = (DataType)xMat.entry[1][0]; entry[1][1] = (DataType)xMat.entry[1][1];
      entry[2][1] = (DataType)xMat.entry[1][2]; entry[3][1] = (DataType)xMat.entry[1][3];
      entry[0][2] = (DataType)xMat.entry[2][0]; entry[1][2] = (DataType)xMat.entry[2][1];
      entry[2][2] = (DataType)xMat.entry[2][2]; entry[3][2] = (DataType)xMat.entry[2][3];
      entry[0][3] = (DataType)xMat.entry[3][0]; entry[1][3] = (DataType)xMat.entry[3][1];
      entry[2][3] = (DataType)xMat.entry[3][2]; entry[3][3] = (DataType)xMat.entry[3][3];
    }
    return *this;
  }
  // Set from 3x3 matrix.
  OdTrVisMatrix4x4Impl &set(const OdTrVisMatrix3x3Impl<DataType> &xMat, bool bExtend = false)
  {
    entry[0][0] = xMat.entry[0][0]; entry[0][1] = xMat.entry[0][1]; entry[0][2] = xMat.entry[0][2];
    entry[1][0] = xMat.entry[1][0]; entry[1][1] = xMat.entry[1][1]; entry[1][2] = xMat.entry[1][2];
    entry[2][0] = xMat.entry[2][0]; entry[2][1] = xMat.entry[2][1]; entry[2][2] = xMat.entry[2][2];
    if (bExtend)
      entry[0][3] = entry[1][3] = entry[2][3] = 0.0f,
      entry[3][0] = entry[3][1] = entry[3][2] = 0.0f,
      entry[3][3] = 1.0f;
    return *this;
  }
  // Set from function arguments.
  OdTrVisMatrix4x4Impl &set(DataType e00 = 1.0f, DataType e01 = 0.0f, DataType e02 = 0.0f, DataType e03 = 0.0f,
                            DataType e10 = 0.0f, DataType e11 = 1.0f, DataType e12 = 0.0f, DataType e13 = 0.0f,
                            DataType e20 = 0.0f, DataType e21 = 0.0f, DataType e22 = 1.0f, DataType e23 = 0.0f,
                            DataType e30 = 0.0f, DataType e31 = 0.0f, DataType e32 = 0.0f, DataType e33 = 1.0f)
  {
    entry[0][0] = e00; entry[0][1] = e01; entry[0][2] = e02; entry[0][3] = e03;
    entry[1][0] = e10; entry[1][1] = e11; entry[1][2] = e12; entry[1][3] = e13;
    entry[2][0] = e20; entry[2][1] = e21; entry[2][2] = e22; entry[2][3] = e23;
    entry[3][0] = e30; entry[3][1] = e31; entry[3][2] = e32; entry[3][3] = e33;
    return *this;
  }
  // Set to identity.
  OdTrVisMatrix4x4Impl &setIdentity()
  {
    entry[0][0] = 1.0f; entry[0][1] = 0.0f; entry[0][2] = 0.0f; entry[0][3] = 0.0f;
    entry[1][0] = 0.0f; entry[1][1] = 1.0f; entry[1][2] = 0.0f; entry[1][3] = 0.0f;
    entry[2][0] = 0.0f; entry[2][1] = 0.0f; entry[2][2] = 1.0f; entry[2][3] = 0.0f;
    entry[3][0] = 0.0f; entry[3][1] = 0.0f; entry[3][2] = 0.0f; entry[3][3] = 1.0f;
    return *this;
  }
  // Get as Ge matrix
  OdGeMatrix3d &get(OdGeMatrix3d &xMat, bool bTransposed = true) const
  {
    if (!bTransposed)
    {
      xMat.entry[0][0] = entry[0][0]; xMat.entry[0][1] = entry[0][1]; xMat.entry[0][2] = entry[0][2]; xMat.entry[0][3] = entry[0][3];
      xMat.entry[1][0] = entry[1][0]; xMat.entry[1][1] = entry[1][1]; xMat.entry[1][2] = entry[1][2]; xMat.entry[1][3] = entry[1][3];
      xMat.entry[2][0] = entry[2][0]; xMat.entry[2][1] = entry[2][1]; xMat.entry[2][2] = entry[2][2]; xMat.entry[2][3] = entry[2][3];
      xMat.entry[3][0] = entry[3][0]; xMat.entry[3][1] = entry[3][1]; xMat.entry[3][2] = entry[3][2]; xMat.entry[3][3] = entry[3][3];
    }
    else
    {
      xMat.entry[0][0] = entry[0][0]; xMat.entry[1][0] = entry[0][1]; xMat.entry[2][0] = entry[0][2]; xMat.entry[3][0] = entry[0][3];
      xMat.entry[0][1] = entry[1][0]; xMat.entry[1][1] = entry[1][1]; xMat.entry[2][1] = entry[1][2]; xMat.entry[3][1] = entry[1][3];
      xMat.entry[0][2] = entry[2][0]; xMat.entry[1][2] = entry[2][1]; xMat.entry[2][2] = entry[2][2]; xMat.entry[3][2] = entry[2][3];
      xMat.entry[0][3] = entry[3][0]; xMat.entry[1][3] = entry[3][1]; xMat.entry[2][3] = entry[3][2]; xMat.entry[3][3] = entry[3][3];
    }
    return xMat;
  }
  // Get as 3x3 matrix
  OdTrVisMatrix3x3Impl<DataType> &getSubMatrix(OdTrVisMatrix3x3Impl<DataType> &xMat) const
  {
    xMat.entry[0][0] = entry[0][0]; xMat.entry[0][1] = entry[0][1]; xMat.entry[0][2] = entry[0][2];
    xMat.entry[1][0] = entry[1][0]; xMat.entry[1][1] = entry[1][1]; xMat.entry[1][2] = entry[1][2];
    xMat.entry[2][0] = entry[2][0]; xMat.entry[2][1] = entry[2][1]; xMat.entry[2][2] = entry[2][2];
    return xMat;
  }
  OdTrVisMatrix3x3Impl<DataType> getSubMatrix() const
  { OdTrVisMatrix3x3Impl<DataType> xMat;
    return getSubMatrix(xMat);
  }
  // Getters.
  const DataType *getArray(int nRow = 0) const { return entry[nRow]; }
  DataType *getArray(int nRow = 0) { return entry[nRow]; }
  DataType operator()(int i, int j) const { return entry[i][j]; }
  // Matrix multiplication.
  OdTrVisMatrix4x4Impl operator *(const OdTrVisMatrix4x4Impl &m) const
  { OdTrVisMatrix4x4Impl rv;
    // First row.
    rv.entry[0][0] = entry[0][0] * m.entry[0][0] + entry[1][0] * m.entry[0][1] + entry[2][0] * m.entry[0][2] + entry[3][0] * m.entry[0][3];
    rv.entry[0][1] = entry[0][1] * m.entry[0][0] + entry[1][1] * m.entry[0][1] + entry[2][1] * m.entry[0][2] + entry[3][1] * m.entry[0][3];
    rv.entry[0][2] = entry[0][2] * m.entry[0][0] + entry[1][2] * m.entry[0][1] + entry[2][2] * m.entry[0][2] + entry[3][2] * m.entry[0][3];
    rv.entry[0][3] = entry[0][3] * m.entry[0][0] + entry[1][3] * m.entry[0][1] + entry[2][3] * m.entry[0][2] + entry[3][3] * m.entry[0][3];
    // Second row.
    rv.entry[1][0] = entry[0][0] * m.entry[1][0] + entry[1][0] * m.entry[1][1] + entry[2][0] * m.entry[1][2] + entry[3][0] * m.entry[1][3];
    rv.entry[1][1] = entry[0][1] * m.entry[1][0] + entry[1][1] * m.entry[1][1] + entry[2][1] * m.entry[1][2] + entry[3][1] * m.entry[1][3];
    rv.entry[1][2] = entry[0][2] * m.entry[1][0] + entry[1][2] * m.entry[1][1] + entry[2][2] * m.entry[1][2] + entry[3][2] * m.entry[1][3];
    rv.entry[1][3] = entry[0][3] * m.entry[1][0] + entry[1][3] * m.entry[1][1] + entry[2][3] * m.entry[1][2] + entry[3][3] * m.entry[1][3];
    // Third row.
    rv.entry[2][0] = entry[0][0] * m.entry[2][0] + entry[1][0] * m.entry[2][1] + entry[2][0] * m.entry[2][2] + entry[3][0] * m.entry[2][3];
    rv.entry[2][1] = entry[0][1] * m.entry[2][0] + entry[1][1] * m.entry[2][1] + entry[2][1] * m.entry[2][2] + entry[3][1] * m.entry[2][3];
    rv.entry[2][2] = entry[0][2] * m.entry[2][0] + entry[1][2] * m.entry[2][1] + entry[2][2] * m.entry[2][2] + entry[3][2] * m.entry[2][3];
    rv.entry[2][3] = entry[0][3] * m.entry[2][0] + entry[1][3] * m.entry[2][1] + entry[2][3] * m.entry[2][2] + entry[3][3] * m.entry[2][3];
    // Fourth row.
    rv.entry[3][0] = entry[0][0] * m.entry[3][0] + entry[1][0] * m.entry[3][1] + entry[2][0] * m.entry[3][2] + entry[3][0] * m.entry[3][3];
    rv.entry[3][1] = entry[0][1] * m.entry[3][0] + entry[1][1] * m.entry[3][1] + entry[2][1] * m.entry[3][2] + entry[3][1] * m.entry[3][3];
    rv.entry[3][2] = entry[0][2] * m.entry[3][0] + entry[1][2] * m.entry[3][1] + entry[2][2] * m.entry[3][2] + entry[3][2] * m.entry[3][3];
    rv.entry[3][3] = entry[0][3] * m.entry[3][0] + entry[1][3] * m.entry[3][1] + entry[2][3] * m.entry[3][2] + entry[3][3] * m.entry[3][3];
    // Return result matrix.
    return rv;
  }
  OdTrVisMatrix4x4Impl &operator *=(const OdTrVisMatrix4x4Impl &m)
  { return *this = *this * m; }
  // To be completely correct we must have inverse multiplication.
  OdTrVisMatrix4x4Impl operator ^(const OdTrVisMatrix4x4Impl &m) const
  { return *this * OdTrVisMatrix4x4Impl(m).invert(); }
  OdTrVisMatrix4x4Impl &operator ^=(const OdTrVisMatrix4x4Impl &m)
  { return *this = *this * OdTrVisMatrix4x4Impl(m).invert(); }
  // Inverse operator.
  OdTrVisMatrix4x4Impl &invert(DataType eps = 1.e-8f)
  { // Premultiplied coefficients.
    const DataType a2323 = entry[2][2] * entry[3][3] - entry[2][3] * entry[3][2];
    const DataType a1323 = entry[2][1] * entry[3][3] - entry[2][3] * entry[3][1];
    const DataType a1223 = entry[2][1] * entry[3][2] - entry[2][2] * entry[3][1];
    const DataType a0323 = entry[2][0] * entry[3][3] - entry[2][3] * entry[3][0];
    const DataType a0223 = entry[2][0] * entry[3][2] - entry[2][2] * entry[3][0];
    const DataType a0123 = entry[2][0] * entry[3][1] - entry[2][1] * entry[3][0];
    const DataType a2313 = entry[1][2] * entry[3][3] - entry[1][3] * entry[3][2];
    const DataType a1313 = entry[1][1] * entry[3][3] - entry[1][3] * entry[3][1];
    const DataType a1213 = entry[1][1] * entry[3][2] - entry[1][2] * entry[3][1];
    const DataType a2312 = entry[1][2] * entry[2][3] - entry[1][3] * entry[2][2];
    const DataType a1312 = entry[1][1] * entry[2][3] - entry[1][3] * entry[2][1];
    const DataType a1212 = entry[1][1] * entry[2][2] - entry[1][2] * entry[2][1];
    const DataType a0313 = entry[1][0] * entry[3][3] - entry[1][3] * entry[3][0];
    const DataType a0213 = entry[1][0] * entry[3][2] - entry[1][2] * entry[3][0];
    const DataType a0312 = entry[1][0] * entry[2][3] - entry[1][3] * entry[2][0];
    const DataType a0212 = entry[1][0] * entry[2][2] - entry[1][2] * entry[2][0];
    const DataType a0113 = entry[1][0] * entry[3][1] - entry[1][1] * entry[3][0];
    const DataType a0112 = entry[1][0] * entry[2][1] - entry[1][1] * entry[2][0];
    // Compute determinant.
    const DataType det = entry[0][0] * (entry[1][1] * a2323 - entry[1][2] * a1323 + entry[1][3] * a1223)
                       - entry[0][1] * (entry[1][0] * a2323 - entry[1][2] * a0323 + entry[1][3] * a0223)
                       + entry[0][2] * (entry[1][0] * a1323 - entry[1][1] * a0323 + entry[1][3] * a0123)
                       - entry[0][3] * (entry[1][0] * a1223 - entry[1][1] * a0223 + entry[1][2] * a0123);
    if (det < eps)
      transpose();
    else
    { const OdTrVisMatrix4x4Impl src(*this);
      const DataType invdet = 1.0f / det;
      entry[0][0] = invdet *  (src.entry[1][1] * a2323 - src.entry[1][2] * a1323 + src.entry[1][3] * a1223);
      entry[0][1] = invdet * -(src.entry[0][1] * a2323 - src.entry[0][2] * a1323 + src.entry[0][3] * a1223);
      entry[0][2] = invdet *  (src.entry[0][1] * a2313 - src.entry[0][2] * a1313 + src.entry[0][3] * a1213);
      entry[0][3] = invdet * -(src.entry[0][1] * a2312 - src.entry[0][2] * a1312 + src.entry[0][3] * a1212);
      entry[1][0] = invdet * -(src.entry[1][0] * a2323 - src.entry[1][2] * a0323 + src.entry[1][3] * a0223);
      entry[1][1] = invdet *  (src.entry[0][0] * a2323 - src.entry[0][2] * a0323 + src.entry[0][3] * a0223);
      entry[1][2] = invdet * -(src.entry[0][0] * a2313 - src.entry[0][2] * a0313 + src.entry[0][3] * a0213);
      entry[1][3] = invdet *  (src.entry[0][0] * a2312 - src.entry[0][2] * a0312 + src.entry[0][3] * a0212);
      entry[2][0] = invdet *  (src.entry[1][0] * a1323 - src.entry[1][1] * a0323 + src.entry[1][3] * a0123);
      entry[2][1] = invdet * -(src.entry[0][0] * a1323 - src.entry[0][1] * a0323 + src.entry[0][3] * a0123);
      entry[2][2] = invdet *  (src.entry[0][0] * a1313 - src.entry[0][1] * a0313 + src.entry[0][3] * a0113);
      entry[2][3] = invdet * -(src.entry[0][0] * a1312 - src.entry[0][1] * a0312 + src.entry[0][3] * a0112);
      entry[3][0] = invdet * -(src.entry[1][0] * a1223 - src.entry[1][1] * a0223 + src.entry[1][2] * a0123);
      entry[3][1] = invdet *  (src.entry[0][0] * a1223 - src.entry[0][1] * a0223 + src.entry[0][2] * a0123);
      entry[3][2] = invdet * -(src.entry[0][0] * a1213 - src.entry[0][1] * a0213 + src.entry[0][2] * a0113);
      entry[3][3] = invdet *  (src.entry[0][0] * a1212 - src.entry[0][1] * a0212 + src.entry[0][2] * a0112);
    }
    return *this;
  }
  OdTrVisMatrix4x4Impl operator ~() const
  { return OdTrVisMatrix4x4Impl(*this).invert(); }
  // Transpose operator.
  OdTrVisMatrix4x4Impl &transpose()
  { const OdTrVisMatrix4x4Impl src(*this);
    entry[0][1] = src.entry[1][0];
    entry[0][2] = src.entry[2][0];
    entry[0][3] = src.entry[3][0];
    entry[1][2] = src.entry[2][1];
    entry[1][3] = src.entry[3][1];
    entry[2][3] = src.entry[3][2];
    // Second part.
    entry[1][0] = src.entry[0][1];
    entry[2][0] = src.entry[0][2];
    entry[3][0] = src.entry[0][3];
    entry[2][1] = src.entry[1][2];
    entry[3][1] = src.entry[1][3];
    entry[3][2] = src.entry[2][3];
    return *this;
  }
  OdTrVisMatrix4x4Impl operator -() const
  { return OdTrVisMatrix4x4Impl(*this).transpose(); }
  // Translation operators.
  template <typename VectorType>
  static OdTrVisMatrix4x4Impl &translationMatrix(OdTrVisMatrix4x4Impl &m, const VectorType &v, bool bNegate = false)
  { DataType *pMat = m.setIdentity().getArray();
    pMat[12] = v[0], pMat[13] = v[1], pMat[14] = v[2];
    if (bNegate)
      pMat[12] = -pMat[12], pMat[13] = -pMat[13], pMat[14] = -pMat[14];
    return m;
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl operator +(const VectorType &v) const
  { OdTrVisMatrix4x4Impl t;
    return *this * translationMatrix(t, v);
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl &operator +=(const VectorType &v)
  { OdTrVisMatrix4x4Impl t;
    return *this *= translationMatrix(t, v);
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl operator -(const VectorType &v) const
  { OdTrVisMatrix4x4Impl t;
    return *this * translationMatrix(t, v, true);
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl &operator -=(const VectorType &v)
  { OdTrVisMatrix4x4Impl t;
    return *this *= translationMatrix(t, v, true);
  }
  // Translation getters.
  OdTrVisMiniVec3d<DataType> getTranslation() const
  { return OdTrVisMiniVec3d<DataType>(entry[3][0], entry[3][1], entry[3][2]); }
  // Scale operators.
  template <typename VectorType>
  static OdTrVisMatrix4x4Impl &scaleMatrix(OdTrVisMatrix4x4Impl &m, const VectorType &v, bool bInverse = false)
  { OdTrVisMatrix3x3Impl<DataType> mx;
    return OdTrVisMatrix4x4Impl().set(OdTrVisMatrix3x3Impl<DataType>::scaleMatrix(mx, v, bInverse), true);
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl scale(const VectorType &v) const
  { OdTrVisMatrix4x4Impl s;
    return *this * scaleMatrix(s, v);
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl &setScale(const VectorType &v)
  { OdTrVisMatrix4x4Impl s;
    return *this * scaleMatrix(s, v);
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl recipScale(const VectorType &v) const
  { OdTrVisMatrix4x4Impl s;
    return *this * scaleMatrix(s, v, true);
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl &setRecipScale(const VectorType &v)
  { OdTrVisMatrix4x4Impl s;
    return *this * scaleMatrix(s, v, true);
  }
  // Orthogonal scale.
  static OdTrVisMatrix4x4Impl &scaleMatrix(OdTrVisMatrix4x4Impl &m, DataType s, bool bInverse = false)
  { OdTrVisMatrix3x3Impl<DataType> mx;
    return OdTrVisMatrix4x4Impl().set(OdTrVisMatrix3x3Impl<DataType>::scaleMatrix(mx, s, bInverse), true);
  }
  friend OdTrVisMatrix4x4Impl operator *(const OdTrVisMatrix4x4Impl &m, DataType n)
  { OdTrVisMatrix4x4Impl s;
    return m * scaleMatrix(s, n);
  }
  friend OdTrVisMatrix4x4Impl operator *(DataType n, const OdTrVisMatrix4x4Impl &m)
  { OdTrVisMatrix4x4Impl s;
    return scaleMatrix(s, n) * m;
  }
  OdTrVisMatrix4x4Impl &operator *=(DataType n)
  { OdTrVisMatrix4x4Impl s;
    return *this *= scaleMatrix(s, n);
  }
  friend OdTrVisMatrix4x4Impl operator /(const OdTrVisMatrix4x4Impl &m, DataType n)
  { OdTrVisMatrix4x4Impl s;
    return m * scaleMatrix(s, n, true);
  }
  friend OdTrVisMatrix4x4Impl operator /(DataType n, const OdTrVisMatrix4x4Impl &m)
  { OdTrVisMatrix4x4Impl s;
    return scaleMatrix(s, n, true) * m;
  }
  OdTrVisMatrix4x4Impl &operator /=(DataType n)
  { OdTrVisMatrix4x4Impl s;
    return *this *= scaleMatrix(s, n, true);
  }
  // Rotation operators.
  template <typename VectorType>
  static OdTrVisMatrix4x4Impl &localRotationMatrix(OdTrVisMatrix4x4Impl &m, const VectorType &angles)
  { OdTrVisMatrix3x3Impl<DataType> mx;
    return OdTrVisMatrix4x4Impl().set(OdTrVisMatrix3x3Impl<DataType>::localRotationMatrix(mx, angles), true);
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl &localRotate(const VectorType &angles)
  { OdTrVisMatrix4x4Impl r;
    return *this *= rotationMatrix(r, angles); }
  // This is more OpenGL-compatible version of rotation.
  template <typename VectorType>
  static OdTrVisMatrix4x4Impl &rotationMatrix(OdTrVisMatrix4x4Impl &m, DataType angle, const VectorType &v, DataType eps = 1.e-8f)
  { OdTrVisMatrix3x3Impl<DataType> mx;
    return OdTrVisMatrix4x4Impl().set(OdTrVisMatrix3x3Impl<DataType>::rotationMatrix(mx, angle, v, eps), true);
  }
  template <typename VectorType>
  OdTrVisMatrix4x4Impl &rotate(DataType angle, const VectorType &v, DataType eps = 1.e-8f)
  { OdTrVisMatrix4x4Impl r;
    return *this *= rotationMatrix(r, angle, v, eps); }
  // Rotation getters.
  OdTrVisMiniVec3d<DataType> getRotationXAxis() const
  { return OdTrVisMiniVec3d<DataType>(entry[0][0], entry[1][0], entry[2][0]); }
  OdTrVisMiniVec3d<DataType> getRotationYAxis() const
  { return OdTrVisMiniVec3d<DataType>(entry[0][1], entry[1][1], entry[2][1]); }
  OdTrVisMiniVec3d<DataType> getRotationZAxis() const
  { return OdTrVisMiniVec3d<DataType>(entry[0][2], entry[1][2], entry[2][2]); }
  // Vector transformation operators
  template <typename VectorType>
  VectorType &transform(VectorType &vec, DataType w = 1.0f) const
  { return vec.set(entry[0][0] * vec.x + entry[1][0] * vec.y + entry[2][0] * vec.z + entry[3][0] * w,
                   entry[0][1] * vec.x + entry[1][1] * vec.y + entry[2][1] * vec.z + entry[3][1] * w,
                   entry[0][2] * vec.x + entry[1][2] * vec.y + entry[2][2] * vec.z + entry[3][2] * w); }
  template <typename VectorType>
  VectorType transformClone(const VectorType &vec, DataType w = 1.0f) const
  { VectorType tmp(vec); return transform(tmp, w); }
  template <typename VectorType>
  friend VectorType operator *(const OdTrVisMatrix4x4Impl m, const VectorType &v)
  { VectorType vec(v); return m.transform(vec); }
  template <typename VectorType>
  friend VectorType operator *(const VectorType &v, const OdTrVisMatrix4x4Impl m)
  { VectorType vec(v); return m.transform(vec); }
  // Inverse transformation
  template <typename VectorType>
  VectorType &untransform(VectorType &vec, DataType w = 1.0f) const
  { return vec.set(entry[0][0] * vec.x + entry[0][1] * vec.y + entry[0][2] * vec.z + entry[0][3] * w,
                   entry[1][0] * vec.x + entry[1][1] * vec.y + entry[1][2] * vec.z + entry[1][3] * w,
                   entry[2][0] * vec.x + entry[2][1] * vec.y + entry[2][2] * vec.z + entry[2][3] * w); }
  template <typename VectorType>
  VectorType untransformClone(const VectorType &vec, DataType w = 1.0f) const
  { VectorType tmp(vec); return untransform(tmp, w); }
  template <typename VectorType>
  friend VectorType operator ^(const OdTrVisMatrix4x4Impl m, const VectorType &v)
  { VectorType vec(v); return m.untransform(vec); }
  template <typename VectorType>
  friend VectorType operator ^(const VectorType &v, const OdTrVisMatrix4x4Impl m)
  { VectorType vec(v); return m.untransform(vec); }
  // Setup matrix to look at specified point.
  template <typename VectorType, typename VectorType2>
  static OdTrVisMatrix4x4Impl &lookAtMatrix(OdTrVisMatrix4x4Impl &l, const VectorType &eye, const VectorType &center, const VectorType2 &up, DataType eps = 1.e-8f)
  { OdTrVisMiniVec3d<DataType> Xx, Yy, Zz, Ee((DataType)eye[0], (DataType)eye[1], (DataType)eye[2]);
    // Compute necessary vectors.
    Zz = -(Ee - OdTrVisMiniVec3d<DataType>((DataType)center[0], (DataType)center[1], (DataType)center[2]));
    Yy.set((DataType)up[0], (DataType)up[1], (DataType)up[2]);
    Xx = Yy ^ Zz;
    Yy = Zz ^ Xx;
    Xx = Xx.normalize(eps);
    Yy = Yy.normalize(eps);
    Zz = Zz.normalize(eps);
    // Result rotation.
    l.entry[0][0] = -Xx.x; l.entry[0][1] = -Yy.x; l.entry[0][2] = -Zz.x; l.entry[0][3] = 0.0f;
    l.entry[1][0] = -Xx.y; l.entry[1][1] = -Yy.y; l.entry[1][2] = -Zz.y; l.entry[1][3] = 0.0f;
    l.entry[2][0] = -Xx.z; l.entry[2][1] = -Yy.z; l.entry[2][2] = -Zz.z; l.entry[2][3] = 0.0f;
    // Result translation.
    l.entry[3][0] = Ee * Xx; l.entry[3][1] = Ee * Yy; l.entry[3][2] = Ee * Zz; l.entry[3][3] = 1.0f;
    return l;
  }
  template <typename VectorType, typename VectorType2>
  OdTrVisMatrix4x4Impl &lookAt(const VectorType &eye, const VectorType &center, const VectorType2 &up, DataType eps = 1.e-8f)
  { OdTrVisMatrix4x4Impl l;
    return *this *= lookAtMatrix(l, eye, center, up, eps);
  }
  // Orthographic projection.
  static OdTrVisMatrix4x4Impl &orthoMatrix(OdTrVisMatrix4x4Impl &m, DataType left, DataType right, DataType bottom, DataType top, DataType near_, DataType far_)
  {
    m.entry[0][0] = 2.0f / (right - left); m.entry[0][1] = 0.0f; m.entry[0][2] = 0.0f; m.entry[0][3] = (right + left) / (right - left);
    m.entry[1][0] = 0.0f; m.entry[1][1] = 2.0f / (top - bottom); m.entry[1][2] = 0.0f; m.entry[1][3] = (top + bottom) / (top - bottom);
    m.entry[2][0] = 0.0f; m.entry[2][1] = 0.0f; m.entry[2][2] = -2.0f / (far_ - near_); m.entry[2][3] = -(far_ + near_) / (far_ - near_);
    m.entry[3][0] = 0.0f; m.entry[3][1] = 0.0f; m.entry[3][2] = 0.0f; m.entry[3][3] = 1.0f;
    return m;
  }
  OdTrVisMatrix4x4Impl &ortho(DataType left, DataType right, DataType bottom, DataType top, DataType near_, DataType far_)
  { OdTrVisMatrix4x4Impl o;
    return *this *= orthoMatrix(o, left, right, bottom, top, near_, far_); }
  OdTrVisMatrix4x4Impl &ortho2d(DataType left, DataType right, DataType bottom, DataType top)
  { OdTrVisMatrix4x4Impl o;
    return *this *= orthoMatrix(o, left, right, bottom, top, -1.0f, 1.0f); }
  // Perspective projection (frustum).
  static OdTrVisMatrix4x4Impl &frustumMatrix(OdTrVisMatrix4x4Impl &m, DataType left, DataType right, DataType bottom, DataType top, DataType near_, DataType far_)
  { DataType *v = m.getArray();
    if (near_ <= 0.0f || far_ <= 0.0f)
      return m.setIdentity();
    const DataType x = (2.0f * near_) / (right - left);
    const DataType y = (2.0f * near_) / (top - bottom);
    const DataType a = (right + left) / (right - left);
    const DataType b = (top + bottom) / (top - bottom);
    const DataType c = -(far_ + near_) / (far_ - near_);
    const DataType d = -(2.0f * far_ * near_) / (far_ - near_);
    v[0] = x; v[1] = 0.0f; v[2] = 0.0f; v[3] = 0.0f;
    v[4] = 0.0f; v[5] = y; v[6] = 0.0f; v[7] = 0.0f;
    v[8] = a; v[9] = b; v[10] = c; v[11] = -1.0f;
    v[12] = 0.0f; v[13] = 0.0f; v[14] = d; v[15] = 0.0f;
    return m;
  }
  OdTrVisMatrix4x4Impl &frustum(DataType left, DataType right, DataType bottom, DataType top, DataType near_, DataType far_)
  { OdTrVisMatrix4x4Impl f;
    return *this *= frustumMatrix(f, left, right, bottom, top, near_, far_); }
  // Perspective projection.
  static OdTrVisMatrix4x4Impl &perspectiveMatrix(OdTrVisMatrix4x4Impl &m, DataType fov, DataType aspect, DataType near_, DataType far_)
  {
    if (fabs(near_ - far_) < 0.001f)
      return m.setIdentity();
    if (fabs(sin(fov / 2.0f)) < 0.001f)
      return m.setIdentity();
    const DataType h = cos(fov / 2.0f) / sin(fov / 2.0f);
    const DataType w = aspect * h;
    const DataType Q = far_ / (far_ - near_);
    m.entry[0][0] = w; m.entry[0][1] = 0.0f; m.entry[0][2] = 0.0f; m.entry[0][3] = 0.0f;
    m.entry[1][0] = 0.0f; m.entry[1][1] = h; m.entry[1][2] = 0.0f; m.entry[1][3] = 0.0f;
    m.entry[2][0] = 0.0f; m.entry[2][1] = 0.0f; m.entry[2][2] = Q; m.entry[2][3] = 1.0f;
    m.entry[3][0] = 0.0f; m.entry[3][1] = 0.0f; m.entry[3][2] = -Q * near_; m.entry[3][3] = 0.0f;
    return m;
  }
  OdTrVisMatrix4x4Impl &perspective(DataType fov, DataType aspect, DataType near_, DataType far_)
  { OdTrVisMatrix4x4Impl p;
    return *this *= perspectiveMatrix(p, fov, aspect, near_, far_); }
  OdTrVisMatrix4x4Impl &perspective2a(DataType fovX, DataType fovY, DataType near_, DataType far_)
  { OdTrVisMatrix4x4Impl p;
    return *this *= perspectiveMatrix(p, fovY, fovX / fovY, near_, far_); }
};

// Type definitions.

typedef OdTrVisMatrix3x3Impl<float> OdTrVisMatrix3x3f;
typedef OdTrVisMatrix3x3Impl<double> OdTrVisMatrix3x3d;

typedef OdTrVisMatrix4x4Impl<float> OdTrVisMatrix4x4f;
typedef OdTrVisMatrix4x4Impl<double> OdTrVisMatrix4x4d;

// Current main matrix definition.

typedef OdTrVisMatrix4x4f OdTrVisMatrix;
typedef OdTrVisMatrix3x3f OdTrVisSubMat;

typedef OdArray<OdTrVisMatrix, OdMemoryAllocator<OdTrVisMatrix> > OdTrVisMatrixArray;

#include "TD_PackPop.h"

#endif // ODTRVISMATRIX
