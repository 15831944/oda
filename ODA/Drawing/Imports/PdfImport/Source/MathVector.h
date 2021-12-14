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

#ifndef _IMPORT_MATH_VECTOR_INCLUDED_
#define _IMPORT_MATH_VECTOR_INCLUDED_

#include <cmath>
#include <vector>
#include "DebugStuff.h"


// would like to be replaced by ublas::vector

template <typename Type>
class MathVector
{
public:

  MathVector();

  MathVector(int size);

  MathVector(int size, int d);

  void setSize(int size);
  inline int getSize() const;
  inline Type const& operator[](int i) const;
  inline Type& operator[](int i);

  inline bool operator==(MathVector const& vec) const;
  inline bool operator!=(MathVector const& vec) const;
  inline bool operator< (MathVector const& vec) const;
  inline bool operator<=(MathVector const& vec) const;
  inline bool operator> (MathVector const& vec) const;
  inline bool operator>=(MathVector const& vec) const;

  void makeZero();  
  void makeUnit(int d); 
  static MathVector zero(int size);
  static MathVector unit(int size, int d);

protected:
  std::vector<Type> m_Storage;
};

// Unary operations.
template <typename Type>
MathVector<Type> operator+(MathVector<Type> const& v);

template <typename Type>
MathVector<Type> operator-(MathVector<Type> const& v);

// Linear-algebraic operations.
template <typename Type>
MathVector<Type> operator+(MathVector<Type> const& v0, MathVector<Type> const& v1);

template <typename Type>
MathVector<Type> operator-(MathVector<Type> const& v0, MathVector<Type> const& v1);

template <typename Type>
MathVector<Type> operator*(MathVector<Type> const& v, Type scalar);

template <typename Type>
MathVector<Type> operator*(Type scalar, MathVector<Type> const& v);

template <typename Type>
MathVector<Type> operator/(MathVector<Type> const& v, Type scalar);

template <typename Type>
MathVector<Type>& operator+=(MathVector<Type>& v0, MathVector<Type> const& v1);

template <typename Type>
MathVector<Type>& operator-=(MathVector<Type>& v0, MathVector<Type> const& v1);

template <typename Type>
MathVector<Type>& operator*=(MathVector<Type>& v, Type scalar);

template <typename Type>
MathVector<Type>& operator/=(MathVector<Type>& v, Type scalar);


template <typename Type>
Type dot(MathVector<Type> const& v0, MathVector<Type> const& v1);

template <typename Type>
Type length(MathVector<Type> const& v, bool robust = false);

template <typename Type>
Type normalize(MathVector<Type>& v, bool robust = false);

template <typename Type>
MathVector<Type>::MathVector()
{
}

template <typename Type>
MathVector<Type>::MathVector(int size)
{
  setSize(size);
}

template <typename Type>
MathVector<Type>::MathVector(int size, int d)
{
  setSize(size);
  makeUnit(d);
}

template <typename Type>
void MathVector<Type>::setSize(int size)
{
  ODA_ASSERT(size >= 0); //"Mismatched size."

  if (size > 0)
  {
    m_Storage.resize(size);
  }
}

template <typename Type> inline
int MathVector<Type>::getSize() const
{
  return static_cast<int>(m_Storage.size());
}

template <typename Type> inline
Type const& MathVector<Type>::operator[](int i) const
{
  return m_Storage[i];
}

template <typename Type> inline
Type& MathVector<Type>::operator[](int i)
{
  return m_Storage[i];
}

template <typename Type> inline
bool MathVector<Type>::operator==(MathVector const& vec) const
{
  return m_Storage == vec.m_Storage;
}

template <typename Type> inline
bool MathVector<Type>::operator!=(MathVector const& vec) const
{
  return m_Storage != vec.m_Storage;
}

template <typename Type> inline
bool MathVector<Type>::operator<(const MathVector& vec) const
{
  return m_Storage < vec.m_Storage;
}

template <typename Type> inline
bool MathVector<Type>::operator<=(const MathVector& vec) const
{
  return m_Storage <= vec.m_Storage;
}

template <typename Type> inline
bool MathVector<Type>::operator>(const MathVector& vec) const
{
  return m_Storage > vec.m_Storage;
}

template <typename Type> inline
bool MathVector<Type>::operator>=(const MathVector& vec) const
{
  return m_Storage >= vec.m_Storage;
}

template <typename Type>
void MathVector<Type>::makeZero()
{
  std::fill(m_Storage.begin(), m_Storage.end(), (Type)0);
}

template <typename Type>
void MathVector<Type>::makeUnit(int d)
{
  std::fill(m_Storage.begin(), m_Storage.end(), (Type)0);
  if (0 <= d && d < (int)m_Storage.size())
  {
    m_Storage[d] = (Type)1;
  }
}

template <typename Type>
MathVector<Type> MathVector<Type>::zero(int size)
{
  MathVector<Type> v(size);
  v.makeZero();
  return v;
}

template <typename Type>
MathVector<Type> MathVector<Type>::unit(int size, int d)
{
  MathVector<Type> v(size);
  v.makeUnit(d);
  return v;
}



template <typename Type>
MathVector<Type> operator+(MathVector<Type> const& v)
{
  return v;
}

template <typename Type>
MathVector<Type> operator-(MathVector<Type> const& v)
{
  MathVector<Type> result(v.getSize());
  for (int i = 0; i < v.getSize(); ++i)
  {
    result[i] = -v[i];
  }
  return result;
}

template <typename Type>
MathVector<Type> operator+(MathVector<Type> const& v0, MathVector<Type> const& v1)
{
  MathVector<Type> result = v0;
  return result += v1;
}

template <typename Type>
MathVector<Type> operator-(MathVector<Type> const& v0, MathVector<Type> const& v1)
{
  MathVector<Type> result = v0;
  return result -= v1;
}

template <typename Type>
MathVector<Type> operator*(MathVector<Type> const& v, Type scalar)
{
  MathVector<Type> result = v;
  return result *= scalar;
}

template <typename Type>
MathVector<Type> operator*(Type scalar, MathVector<Type> const& v)
{
  MathVector<Type> result = v;
  return result *= scalar;
}

template <typename Type>
MathVector<Type> operator/(MathVector<Type> const& v, Type scalar)
{
  MathVector<Type> result = v;
  return result /= scalar;
}

template <typename Type>
MathVector<Type>& operator+=(MathVector<Type>& v0, MathVector<Type> const& v1)
{
  if (v0.getSize() == v1.getSize())
  {
    for (int i = 0; i < v0.getSize(); ++i)
    {
      v0[i] += v1[i];
    }
  }
  else
  {
    ODA_FAIL_M_ONCE("Mismatched size.");
  }
  return v0;
}

template <typename Type>
MathVector<Type>& operator-=(MathVector<Type>& v0, MathVector<Type> const& v1)
{
  if (v0.getSize() == v1.getSize())
  {
    for (int i = 0; i < v0.getSize(); ++i)
    {
      v0[i] -= v1[i];
    }
  }
  else
  {
    ODA_FAIL_M_ONCE("Mismatched size.");
  }
  return v0;
}

template <typename Type>
MathVector<Type>& operator*=(MathVector<Type>& v, Type scalar)
{
  for (int i = 0; i < v.getSize(); ++i)
  {
    v[i] *= scalar;
  }
  return v;
}

template <typename Type>
MathVector<Type>& operator/=(MathVector<Type>& v, Type scalar)
{
  if (scalar != (Type)0)
  {
    Type invScalar = ((Type)1) / scalar;
    for (int i = 0; i < v.getSize(); ++i)
    {
      v[i] *= invScalar;
    }
  }
  else
  {
    for (int i = 0; i < v.getSize(); ++i)
    {
      v[i] = (Type)0;
    }
  }
  return v;
}

template <typename Type>
Type dot(MathVector<Type> const& v0, MathVector<Type> const& v1)
{
  if (v0.getSize() == v1.getSize())
  {
    Type dot = v0[0] * v1[0];
    for (int i = 1; i < v0.getSize(); ++i)
    {
      dot += v0[i] * v1[i];
    }
    return dot;
  }
  else
  {
    ODA_FAIL_M_ONCE("Mismatched size.");
    return (Type)0;
  }
}

template <typename Type>
Type length(MathVector<Type> const& v, bool robust)
{
  if (robust)
  {
    Type maxAbsComp = std::abs(v[0]);
    for (int i = 1; i < v.getSize(); ++i)
    {
      Type absComp = std::abs(v[i]);
      if (absComp > maxAbsComp)
      {
        maxAbsComp = absComp;
      }
    }

    Type length;
    if (maxAbsComp > (Type)0)
    {
      MathVector<Type> scaled = v / maxAbsComp;
      length = maxAbsComp * std::sqrt(dot(scaled, scaled));
    }
    else
    {
      length = (Type)0;
    }
    return length;
  }
  else
  {
    return std::sqrt(dot(v, v));
  }
}

template <typename Type>
Type normalize(MathVector<Type>& v, bool robust)
{
  if (robust)
  {
    Type maxAbsComp = std::abs(v[0]);
    for (int i = 1; i < v.getSize(); ++i)
    {
      Type absComp = std::abs(v[i]);
      if (absComp > maxAbsComp)
      {
        maxAbsComp = absComp;
      }
    }

    Type length;
    if (maxAbsComp > (Type)0)
    {
      v /= maxAbsComp;
      length = std::sqrt(dot(v, v));
      v /= length;
      length *= maxAbsComp;
    }
    else
    {
      length = (Type)0;
      for (int i = 0; i < v.getSize(); ++i)
      {
        v[i] = (Type)0;
      }
    }
    return length;
  }
  else
  {
    Type length = std::sqrt(dot(v, v));
    if (length > (Type)0)
    {
      v /= length;
    }
    else
    {
      for (int i = 0; i < v.getSize(); ++i)
      {
        v[i] = (Type)0;
      }
    }
    return length;
  }
}

template <typename Type>
Type Orthonormalize(int numInputs, MathVector<Type>* v, bool robust)
{
  if (v && 1 <= numInputs && numInputs <= v[0].GetSize())
  {
    Type minLength = normalize(v[0], robust);
    for (int i = 1; i < numInputs; ++i)
    {
      for (int j = 0; j < i; ++j)
      {
        Type dot = dot(v[i], v[j]);
        v[i] -= v[j] * dot;
      }
      Type length = normalize(v[i], robust);
      if (length < minLength)
      {
        minLength = length;
      }
    }
    return minLength;
  }

  ODA_FAIL_M_ONCE("Invalid input.");
  return (Type)0;
}

#endif //_IMPORT_MATH_VECTOR_INCLUDED_ 

