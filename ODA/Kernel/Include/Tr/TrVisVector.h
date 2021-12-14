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
// Vector definition

#ifndef ODTRVISVECTOR
#define ODTRVISVECTOR

#include "TD_PackPush.h"

#include "Ge/GePoint3d.h"
#include "Ge/GeVector3d.h"

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
template <typename DataType>
struct OdTrVisMiniVec2d
{
  DataType x, y;
  typedef DataType EntryType;

  static bool eqTol(float val , float  eps = 1.e-8f) { return (val < eps) && (val > -eps); }
  static bool eqTol(double val, double eps = 1.e-10) { return (val < eps) && (val > -eps); }

  // Constructors
  OdTrVisMiniVec2d() { }
  OdTrVisMiniVec2d(DataType _x, DataType _y) : x(_x), y(_y) { }
  template <typename VecType>
  OdTrVisMiniVec2d(const VecType &vec) : x((DataType)vec.x), y((DataType)vec.y) { }

  // Setters
  OdTrVisMiniVec2d &set(DataType _x, DataType _y)
  { x = _x; y = _y; return *this; }
  template <typename VecType>
  OdTrVisMiniVec2d &set(const VecType &vec)
  { x = (DataType)vec.x; y = (DataType)vec.y; return *this; }

  // Getters
  DataType operator [](int nAxis) const
  { return (!nAxis) ? x : y; }
  DataType &operator [](int nAxis)
  { return (!nAxis) ? x : y; }

  // Comparators
  bool operator ==(const OdTrVisMiniVec2d &vec) const
  { return  eqTol(x - vec.x) &&  eqTol(y - vec.y); }
  bool operator !=(const OdTrVisMiniVec2d &vec) const
  { return !eqTol(x - vec.x) || !eqTol(y - vec.y); }

  // Converters
  operator OdGePoint2d() const { return OdGePoint2d(x, y); }
  operator OdGeVector2d() const { return OdGeVector2d(x, y); }

  // Basic math operators
  OdTrVisMiniVec2d operator +(const OdTrVisMiniVec2d &vec) const
  { return OdTrVisMiniVec2d(x + vec.x, y + vec.y); }
  OdTrVisMiniVec2d &operator +=(const OdTrVisMiniVec2d &vec)
  { x += vec.x; y += vec.y; return *this; }
  OdTrVisMiniVec2d operator -(const OdTrVisMiniVec2d &vec) const
  { return OdTrVisMiniVec2d(x - vec.x, y - vec.y); }
  OdTrVisMiniVec2d &operator -=(const OdTrVisMiniVec2d &vec)
  { x -= vec.x; y -= vec.y; return *this; }

  // Dot/cross product
  DataType operator *(const OdTrVisMiniVec2d &vec) const
  { return x * vec.x + y * vec.y; }
  DataType operator ^(const OdTrVisMiniVec2d &vec) const
  { return x * vec.y - y * vec.x; }

  // Scale operators
  friend OdTrVisMiniVec2d operator *(const OdTrVisMiniVec2d &vec, DataType val)
  { return OdTrVisMiniVec2d(vec.x * val, vec.y * val); }
  friend OdTrVisMiniVec2d operator *(DataType val, const OdTrVisMiniVec2d &vec)
  { return OdTrVisMiniVec2d(val * vec.x, val * vec.y); }
  OdTrVisMiniVec2d &operator *=(DataType val)
  { x *= val; y *= val; return *this; }
  friend OdTrVisMiniVec2d operator /(const OdTrVisMiniVec2d &vec, DataType val)
  { return OdTrVisMiniVec2d(vec.x / val, vec.y / val); }
  friend OdTrVisMiniVec2d operator /(DataType val, const OdTrVisMiniVec2d &vec)
  { return OdTrVisMiniVec2d(val / vec.x, val / vec.y); }
  OdTrVisMiniVec2d &operator /=(DataType val)
  { x /= val; y /= val; return *this; }

  // Negate operator
  OdTrVisMiniVec2d operator -() const { return OdTrVisMiniVec2d(-x, -y); }
  OdTrVisMiniVec2d &negate() { x = -x; y = -y; return *this; }

  // Component-wise operators
  OdTrVisMiniVec2d mul(const OdTrVisMiniVec2d &vec) const
  { return OdTrVisMiniVec2d(x * vec.x, y * vec.y); }
  OdTrVisMiniVec2d &setMul(const OdTrVisMiniVec2d &vec) const
  { x *= vec.x; y *= vec.y; return *this; }
  OdTrVisMiniVec2d div(const OdTrVisMiniVec2d &vec) const
  { return OdTrVisMiniVec2d(x / vec.x, y / vec.y); }
  OdTrVisMiniVec2d &setDiv(const OdTrVisMiniVec2d &vec) const
  { x /= vec.x; y /= vec.y; return *this; }

  // Null operator
  OdTrVisMiniVec2d &setNull()
  { x = y = 0.0f; return *this; }
  bool isNull(DataType eps = 1.e-8f) const
  { return eqTol(x, eps) && eqTol(y, eps); }

  // Length operator
  DataType length2() const
  { return x * x + y * y; }
  DataType length() const
  { return (DataType)sqrt(length2()); }

  // Normalization
  OdTrVisMiniVec2d operator ~() const
  { return OdTrVisMiniVec2d(*this).normalize(); }
  OdTrVisMiniVec2d &normalize(DataType eps = 1.e-8f)
  { DataType rd = length(); 
    if (eqTol(rd, eps))
      setNull();
    else
      rd = 1.0f / rd,
      *this *= rd;
    return *this;
  }

  // Min/max operators
  DataType minimum() const
  { return odmin(x, y); }
  DataType maximum() const
  { return odmax(x, y); }
};

/** \details
  Library: Source code provided.
  <group ExRender_Windows_Classes>
*/
template <typename DataType>
struct OdTrVisMiniVec3d : public OdTrVisMiniVec2d<DataType>
{
  DataType z;

  typedef OdTrVisMiniVec2d<DataType> BaseVec;
  // Constructors
  OdTrVisMiniVec3d() { }
  OdTrVisMiniVec3d(DataType _x, DataType _y, DataType _z) : OdTrVisMiniVec2d<DataType>(_x, _y), z(_z) { }
  OdTrVisMiniVec3d(const BaseVec &xy, DataType _z) : OdTrVisMiniVec2d<DataType>(xy), z(_z) { }
  template <typename VecType>
  OdTrVisMiniVec3d(const VecType &vec) : OdTrVisMiniVec2d<DataType>((DataType)vec.x, (DataType)vec.y), z(vec.z) { }

  // Setters
  OdTrVisMiniVec3d &set(DataType _x, DataType _y, DataType _z)
  { BaseVec::x = _x; BaseVec::y = _y; z = _z; return *this; }
  OdTrVisMiniVec3d &set(const BaseVec &xy, DataType _z)
  { BaseVec::x = xy.x; BaseVec::y = xy.y; z = _z; return *this; }
  template <typename VecType>
  OdTrVisMiniVec3d &set(const VecType &vec)
  { BaseVec::x = (DataType)vec.x; BaseVec::y = (DataType)vec.y; z = (DataType)vec.z; return *this; }

  // Getters
  DataType operator [](int nAxis) const
  { return (!nAxis) ? BaseVec::x : ((nAxis > 1) ? z : BaseVec::y); }
  DataType &operator [](int nAxis)
  { return (!nAxis) ? BaseVec::x : ((nAxis > 1) ? z : BaseVec::y); }

  // Comparators
  bool operator ==(const OdTrVisMiniVec3d &vec) const
  { return  BaseVec::eqTol(BaseVec::x - vec.x) &&  BaseVec::eqTol(BaseVec::y - vec.y) &&  BaseVec::eqTol(z - vec.z); }
  bool operator !=(const OdTrVisMiniVec3d &vec) const
  { return !BaseVec::eqTol(BaseVec::x - vec.x) || !BaseVec::eqTol(BaseVec::y - vec.y) || !BaseVec::eqTol(z - vec.z); }

  // Converters
  operator OdGePoint3d() const { return OdGePoint3d(BaseVec::x, BaseVec::y, z); }
  operator OdGeVector3d() const { return OdGeVector3d(BaseVec::x, BaseVec::y, z); }

  // Basic math operators
  OdTrVisMiniVec3d operator +(const OdTrVisMiniVec3d &vec) const
  { return OdTrVisMiniVec3d(BaseVec::x + vec.x, BaseVec::y + vec.y, z + vec.z); }
  OdTrVisMiniVec3d &operator +=(const OdTrVisMiniVec3d &vec)
  { BaseVec::x += vec.x; BaseVec::y += vec.y; z += vec.z; return *this; }
  OdTrVisMiniVec3d operator -(const OdTrVisMiniVec3d &vec) const
  { return OdTrVisMiniVec3d(BaseVec::x - vec.x, BaseVec::y - vec.y, z - vec.z); }
  OdTrVisMiniVec3d &operator -=(const OdTrVisMiniVec3d &vec)
  { BaseVec::x -= vec.x; BaseVec::y -= vec.y; z -= vec.z; return *this; }

  // Dot/cross product
  DataType operator *(const OdTrVisMiniVec3d &vec) const
  { return BaseVec::x * vec.x + BaseVec::y * vec.y + z * vec.z; }
  OdTrVisMiniVec3d operator ^(const OdTrVisMiniVec3d &vec) const
  { return OdTrVisMiniVec3d(BaseVec::y * vec.z - z * vec.y, z * vec.x - BaseVec::x * vec.z, BaseVec::x * vec.y - BaseVec::y * vec.x); }

  // Scale operators
  friend OdTrVisMiniVec3d operator *(const OdTrVisMiniVec3d &vec, DataType val)
  { return OdTrVisMiniVec3d(vec.x * val, vec.y * val, vec.z * val); }
  friend OdTrVisMiniVec3d operator *(DataType val, const OdTrVisMiniVec3d &vec)
  { return OdTrVisMiniVec3d(val * vec.x, val * vec.y, val * vec.z); }
  OdTrVisMiniVec3d &operator *=(DataType val)
  { BaseVec::x *= val; BaseVec::y *= val; z *= val; return *this; }
  friend OdTrVisMiniVec3d operator /(const OdTrVisMiniVec3d &vec, DataType val)
  { return OdTrVisMiniVec3d(vec.x / val, vec.y / val, vec.z / val); }
  friend OdTrVisMiniVec3d operator /(DataType val, const OdTrVisMiniVec3d &vec)
  { return OdTrVisMiniVec3d(val / vec.x, val / vec.y, val / vec.z); }
  OdTrVisMiniVec3d &operator /=(DataType val)
  { BaseVec::x /= val; BaseVec::y /= val; z /= val; return *this; }

  // Negate operator
  OdTrVisMiniVec3d operator -() const { return OdTrVisMiniVec3d(-BaseVec::x, -BaseVec::y, -z); }
  OdTrVisMiniVec3d &negate() { BaseVec::x = -BaseVec::x; BaseVec::y = -BaseVec::y; z = -z; return *this; }

  // Component-wise operators
  OdTrVisMiniVec3d mul(const OdTrVisMiniVec3d &vec) const
  { return OdTrVisMiniVec3d(BaseVec::x * vec.x, BaseVec::y * vec.y, z * vec.z); }
  OdTrVisMiniVec3d &setMul(const OdTrVisMiniVec3d &vec) const
  { BaseVec::x *= vec.x; BaseVec::y *= vec.y; z *= vec.z; return *this; }
  OdTrVisMiniVec3d div(const OdTrVisMiniVec3d &vec) const
  { return OdTrVisMiniVec3d(BaseVec::x / vec.x, BaseVec::y / vec.y, z / vec.z); }
  OdTrVisMiniVec3d &setDiv(const OdTrVisMiniVec3d &vec) const
  { BaseVec::x /= vec.x; BaseVec::y /= vec.y; z /= vec.z; return *this; }

  // Null operator
  OdTrVisMiniVec3d &setNull()
  { BaseVec::x = BaseVec::y = z = 0.0f; return *this; }
  bool isNull(DataType eps = 1.e-8f) const
  { return BaseVec::eqTol(BaseVec::x, eps) && BaseVec::eqTol(BaseVec::y, eps) && BaseVec::eqTol(z, eps); }

  // Length operator
  DataType length2() const
  { return BaseVec::x * BaseVec::x + BaseVec::y * BaseVec::y + z * z; }
  DataType length() const
  { return (DataType)sqrt(length2()); }

  // Normalization
  OdTrVisMiniVec3d operator ~() const
  { return OdTrVisMiniVec3d(*this).normalize(); }
  OdTrVisMiniVec3d &normalize(DataType eps = 1.e-8f)
  { DataType rd = length(); 
    if (BaseVec::eqTol(rd, eps))
      setNull();
    else
      rd = 1.0f / rd,
      *this *= rd;
    return *this;
  }

  // Min/max operators
  DataType minimum() const
  { return odmin(BaseVec::x, odmin(BaseVec::y, z)); }
  DataType maximum() const
  { return odmax(BaseVec::x, odmax(BaseVec::y, z)); }

  // Reflection vector
  OdTrVisMiniVec3d &reflect(const OdTrVisMiniVec3d &n)
  { return *this = (*this * (-2.0f * (*this * n))).mul(n) + *this; }

  // Refraction vector
  OdTrVisMiniVec3d &refract(const OdTrVisMiniVec3d &n, DataType eta, DataType eps = 1.e-8f)
  { const DataType idot = *this * n;
    const DataType k = 1.0f - eta * eta * (1.0f - idot * idot);
    if (BaseVec::eqTol(k, eps))
      return setNull();
    else
      return *this = *this * eta - (n * (eta * idot + (DataType)sqrt(k)));
  }
};

// Type definitions.

typedef OdTrVisMiniVec2d<float> OdTrVisVector2df;
typedef OdTrVisMiniVec2d<double> OdTrVisVector2dd;

typedef OdTrVisMiniVec3d<float> OdTrVisVector3df;
typedef OdTrVisMiniVec3d<double> OdTrVisVector3dd;

// Current main matrix definition.

typedef OdTrVisVector2df OdTrVisVector2d;
typedef OdTrVisVector3df OdTrVisVector;

#include "TD_PackPop.h"

#endif // ODTRVISVECTOR
