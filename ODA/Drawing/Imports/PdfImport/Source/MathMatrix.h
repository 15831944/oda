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

#ifndef _IMPORT_MATH_MATRIX_INCLUDED_
#define _IMPORT_MATH_MATRIX_INCLUDED_

#include "MathVector.h"
#include "Gaussian.h"
#include <algorithm>
#include "DebugStuff.h"


// would like to be replaced by ublas::matrix

template <typename Type>
class MathMatrix
{
public:
  MathMatrix();

  MathMatrix(int num_rows, int num_cols);

  MathMatrix(int num_rows, int num_cols, int r, int c);


  void setSize(int num_rows, int num_cols, bool preserve = false);
  inline void getSize(int& num_rows, int& num_cols) const;
  inline int getNumRows() const;
  inline int getNumCols() const;
  inline int getNumElements() const;
  inline Type const& operator()(int r, int c) const;
  inline Type& operator()(int r, int c);

  void setRow(int r, MathVector<Type> const& vec);
  void setCol(int c, MathVector<Type> const& vec);
  MathVector<Type> getRow(int r) const;
  MathVector<Type> getCol(int c) const;

  inline Type const& operator[](int i) const;
  inline Type& operator[](int i);

  inline bool operator==(MathMatrix const& mat) const;
  inline bool operator!=(MathMatrix const& mat) const;
  inline bool operator< (MathMatrix const& mat) const;
  inline bool operator<=(MathMatrix const& mat) const;
  inline bool operator> (MathMatrix const& mat) const;
  inline bool operator>=(MathMatrix const& mat) const;


  void makeZero();  
  void makeUnit(int r, int c);  
  void makeIdentity();  
  static MathMatrix zero(int num_rows, int num_cols);
  static MathMatrix unit(int num_rows, int num_cols, int r, int c);
  static MathMatrix identity(int num_rows, int num_cols);

protected:
  int m_NumRows, m_NumCols;
  std::vector<Type> m_Storage;
};

// Unary operations.
template <typename Type>
MathMatrix<Type> operator+(MathMatrix<Type> const& M);

template <typename Type>
MathMatrix<Type> operator-(MathMatrix<Type> const& M);

// Linear-algebraic operations.
template <typename Type>
MathMatrix<Type> operator+(MathMatrix<Type> const& M0, MathMatrix<Type> const& M1);

template <typename Type>
MathMatrix<Type> operator-(MathMatrix<Type> const& M0, MathMatrix<Type> const& M1);

template <typename Type>
MathMatrix<Type> operator*(MathMatrix<Type> const& M, Type scalar);

template <typename Type>
MathMatrix<Type> operator*(Type scalar, MathMatrix<Type> const& M);

template <typename Type>
MathMatrix<Type> operator/(MathMatrix<Type> const& M, Type scalar);

template <typename Type>
MathMatrix<Type>& operator+=(MathMatrix<Type>& M0, MathMatrix<Type> const& M1);

template <typename Type>
MathMatrix<Type>& operator-=(MathMatrix<Type>& M0, MathMatrix<Type> const& M1);

template <typename Type>
MathMatrix<Type>& operator*=(MathMatrix<Type>& M, Type scalar);

template <typename Type>
MathMatrix<Type>& operator/=(MathMatrix<Type>& M, Type scalar);

// Geometric operations.
template <typename Type>
Type l1Norm(MathMatrix<Type> const& M);

template <typename Type>
Type l2Norm(MathMatrix<Type> const& M);

template <typename Type>
Type lInfinityNorm(MathMatrix<Type> const& M);

template <typename Type>
MathMatrix<Type> inverse(MathMatrix<Type> const& M,
  bool* reportInvertibility = nullptr);

template <typename Type>
Type determinant(MathMatrix<Type> const& M);

// M^T
template <typename Type>
MathMatrix<Type> transpose(MathMatrix<Type> const& M);

// M*V
template <typename Type>
MathVector<Type> operator*(MathMatrix<Type> const& M, MathVector<Type> const& V);

// V^T*M
template <typename Type>
MathVector<Type> operator*(MathVector<Type> const& V, MathMatrix<Type> const& M);

// A*B
template <typename Type>
MathMatrix<Type> operator*(MathMatrix<Type> const& A, MathMatrix<Type> const& B);

template <typename Type>
MathMatrix<Type> multiplyAB(MathMatrix<Type> const& A, MathMatrix<Type> const& B);

// A*B^T
template <typename Type>
MathMatrix<Type> multiplyABT(MathMatrix<Type> const& A, MathMatrix<Type> const& B);

// A^T*B
template <typename Type>
MathMatrix<Type> multiplyATB(MathMatrix<Type> const& A, MathMatrix<Type> const& B);

// A^T*B^T
template <typename Type>
MathMatrix<Type> multiplyATBT(MathMatrix<Type> const& A, MathMatrix<Type> const& B);

// M*D, D is square diagonal (stored as vector)
template <typename Type>
MathMatrix<Type> multiplyMD(MathMatrix<Type> const& M, MathVector<Type> const& D);

// D*M, D is square diagonal (stored as vector)
template <typename Type>
MathMatrix<Type> multiplyDM(MathVector<Type> const& D, MathMatrix<Type> const& M);

// U*V^T, U is N-by-1, V is M-by-1, result is N-by-M.
template <typename Type>
MathMatrix<Type> outerProduct(MathVector<Type> const& U, MathVector<Type> const& V);

// Initialization to a diagonal matrix whose diagonal entries are the
// components of D.
template <typename Type>
void makeDiagonal(MathVector<Type> const& D, MathMatrix<Type>& M);


template <typename Type>
MathMatrix<Type>::MathMatrix()
  :m_NumRows(0)
  ,m_NumCols(0)
{
}

template <typename Type>
MathMatrix<Type>::MathMatrix(int num_rows, int num_cols)
{
  setSize(num_rows, num_cols);
  std::fill(m_Storage.begin(), m_Storage.end(), (Type)0);
}

template <typename Type>
MathMatrix<Type>::MathMatrix(int num_rows, int num_cols, int r, int c)
{
  setSize(num_rows, num_cols);
  makeUnit(r, c);
}

template <typename Type>
void MathMatrix<Type>::setSize(int num_rows, int num_cols, bool preserve)
{
  if (num_rows > 0 && num_cols > 0)
  {
    std::vector<Type> tmp;
    if (preserve)
      std::swap(tmp, m_Storage);
    int old_mNumRows(m_NumRows), old_mNumCols(m_NumCols);
    m_NumRows = num_rows;
    m_NumCols = num_cols;
    m_Storage.resize(m_NumRows * m_NumCols);
    if (preserve)
    {
      for (int r = 0; r < std::min(old_mNumRows, m_NumRows); ++r)
        for (int c = 0; c < std::min(old_mNumCols, m_NumCols); ++c)
          operator()(r, c) = tmp[r + m_NumRows*c];
    }
  }
  else
  {
    m_NumRows = 0;
    m_NumCols = 0;
    m_Storage.clear();
  }

}

template <typename Type> inline
void MathMatrix<Type>::getSize(int& num_rows, int& num_cols) const
{
  num_rows = m_NumRows;
  num_cols = m_NumCols;
}

template <typename Type> inline
int MathMatrix<Type>::getNumRows() const
{
  return m_NumRows;
}

template <typename Type> inline
int MathMatrix<Type>::getNumCols() const
{
  return m_NumCols;
}

template <typename Type> inline
int MathMatrix<Type>::getNumElements() const
{
  return static_cast<int>(m_Storage.size());
}

template <typename Type> inline
Type const& MathMatrix<Type>::operator()(int r, int c) const
{
  ODA_ASSERT(0 <= r && r < getNumRows() && 0 <= c && c < getNumCols()); //"Invalid index."

#if defined(IS_ROW_MAJOR)
  return m_Storage[c + m_NumCols*r];
#else
  return m_Storage[r + m_NumRows*c];
#endif
}

template <typename Type> inline
Type& MathMatrix<Type>::operator()(int r, int c)
{
  ODA_ASSERT(0 <= r && r < getNumRows() && 0 <= c && c < getNumCols()); //"Invalid index."

#if defined(IS_ROW_MAJOR)
  return m_Storage[c + m_NumCols*r];
#else
  return m_Storage[r + m_NumRows*c];
#endif
}

template <typename Type>
void MathMatrix<Type>::setRow(int r, MathVector<Type> const& vec)
{
  ODA_ASSERT(vec.getSize() == getNumCols()); // "Mismatched size."

  for (int c = 0; c < m_NumCols; ++c)
  {
    operator()(r, c) = vec[c];
  }
}

template <typename Type>
void MathMatrix<Type>::setCol(int c, MathVector<Type> const& vec)
{
  ODA_ASSERT(vec.getSize() == getNumRows()); // "Mismatched size."
  for (int r = 0; r < m_NumRows; ++r)
  {
    operator()(r, c) = vec[r];
  }
}

template <typename Type>
MathVector<Type> MathMatrix<Type>::getRow(int r) const
{
  MathVector<Type> vec(m_NumCols);
  for (int c = 0; c < m_NumCols; ++c)
  {
    vec[c] = operator()(r, c);
  }
  return vec;
}

template <typename Type>
MathVector<Type> MathMatrix<Type>::getCol(int c) const
{
  MathVector<Type> vec(m_NumRows);
  for (int r = 0; r < m_NumRows; ++r)
  {
    vec[r] = operator()(r, c);
  }
  return vec;
}

template <typename Type> inline
Type const& MathMatrix<Type>::operator[](int i) const
{
  return m_Storage[i];
}

template <typename Type> inline
Type& MathMatrix<Type>::operator[](int i)
{
  return m_Storage[i];
}

template <typename Type> inline
bool MathMatrix<Type>::operator==(MathMatrix const& mat) const
{
  return m_NumRows == mat.m_NumRows && m_NumCols == mat.m_NumCols
    && m_Storage == mat.m_Storage;
}

template <typename Type> inline
bool MathMatrix<Type>::operator!=(MathMatrix const& mat) const
{
  return !operator==(mat);
}

template <typename Type> inline
bool MathMatrix<Type>::operator<(MathMatrix const& mat) const
{
  return m_NumRows == mat.m_NumRows && m_NumCols == mat.m_NumCols
    && m_Storage < mat.m_Storage;
}

template <typename Type> inline
bool MathMatrix<Type>::operator<=(MathMatrix const& mat) const
{
  return m_NumRows == mat.m_NumRows && m_NumCols == mat.m_NumCols
    && m_Storage <= mat.m_Storage;
}

template <typename Type> inline
bool MathMatrix<Type>::operator>(MathMatrix const& mat) const
{
  return m_NumRows == mat.m_NumRows && m_NumCols == mat.m_NumCols
    && m_Storage > mat.m_Storage;
}

template <typename Type> inline
bool MathMatrix<Type>::operator>=(MathMatrix const& mat) const
{
  return m_NumRows == mat.m_NumRows && m_NumCols == mat.m_NumCols
    && m_Storage >= mat.m_Storage;
}

template <typename Type>
void MathMatrix<Type>::makeZero()
{
  std::fill(m_Storage.begin(), m_Storage.end(), (Type)0);
}

template <typename Type>
void MathMatrix<Type>::makeUnit(int r, int c)
{
  makeZero();
  if (0 <= r && r < m_NumRows && 0 <= c && c < m_NumCols)
  {
    operator()(r, c) = (Type)1;
  }
}

template <typename Type>
void MathMatrix<Type>::makeIdentity()
{
  makeZero();
  int const numDiagonal = (m_NumRows <= m_NumCols ? m_NumRows : m_NumCols);
  for (int i = 0; i < numDiagonal; ++i)
  {
    operator()(i, i) = (Type)1;
  }
}

template <typename Type>
MathMatrix<Type> MathMatrix<Type>::zero(int num_rows, int num_cols)
{
  MathMatrix<Type> M(num_rows, num_cols);
  M.makeZero();
  return M;
}

template <typename Type>
MathMatrix<Type> MathMatrix<Type>::unit(int num_rows, int num_cols, int r, int c)
{
  MathMatrix<Type> M(num_rows, num_cols);
  M.makeUnit(r, c);
  return M;
}

template <typename Type>
MathMatrix<Type> MathMatrix<Type>::identity(int num_rows, int num_cols)
{
  MathMatrix<Type> M(num_rows, num_cols);
  M.makeIdentity();
  return M;
}



template <typename Type>
MathMatrix<Type> operator+(MathMatrix<Type> const& M)
{
  return M;
}

template <typename Type>
MathMatrix<Type> operator-(MathMatrix<Type> const& M)
{
  MathMatrix<Type> result(M.getNumRows(), M.getNumCols());
  for (int i = 0; i < M.getNumElements(); ++i)
  {
    result[i] = -M[i];
  }
  return result;
}

template <typename Type>
MathMatrix<Type> operator+(MathMatrix<Type> const& M0, MathMatrix<Type> const& M1)
{
  MathMatrix<Type> result = M0;
  return result += M1;
}

template <typename Type>
MathMatrix<Type> operator-(MathMatrix<Type> const& M0, MathMatrix<Type> const& M1)
{
  MathMatrix<Type> result = M0;
  return result -= M1;
}

template <typename Type>
MathMatrix<Type> operator*(MathMatrix<Type> const& M, Type scalar)
{
  MathMatrix<Type> result = M;
  return result *= scalar;
}

template <typename Type>
MathMatrix<Type> operator*(Type scalar, MathMatrix<Type> const& M)
{
  MathMatrix<Type> result = M;
  return result *= scalar;
}

template <typename Type>
MathMatrix<Type> operator/(MathMatrix<Type> const& M, Type scalar)
{
  MathMatrix<Type> result = M;
  return result /= scalar;
}

template <typename Type>
MathMatrix<Type>& operator+=(MathMatrix<Type>& M0, MathMatrix<Type> const& M1)
{
  ODA_ASSERT(M0.getNumRows() == M1.getNumRows() &&
    M0.getNumCols() == M1.getNumCols()); //"Mismatched size."

  for (int i = 0; i < M0.getNumElements(); ++i)
  {
    M0[i] += M1[i];
  }
  return M0;
}

template <typename Type>
MathMatrix<Type>& operator-=(MathMatrix<Type>& M0, MathMatrix<Type> const& M1)
{
  ODA_ASSERT(M0.getNumRows() == M1.getNumRows() &&
    M0.getNumCols() == M1.getNumCols()); //"Mismatched size."

  for (int i = 0; i < M0.getNumElements(); ++i)
  {
    M0[i] -= M1[i];
  }
  return M0;
}

template <typename Type>
MathMatrix<Type>& operator*=(MathMatrix<Type>& M, Type scalar)
{
  for (int i = 0; i < M.getNumElements(); ++i)
  {
    M[i] *= scalar;
  }
  return M;
}

template <typename Type>
MathMatrix<Type>& operator/=(MathMatrix<Type>& M, Type scalar)
{
  if (scalar != (Type)0)
  {
    Type invScalar = ((Type)1) / scalar;
    for (int i = 0; i < M.getNumElements(); ++i)
    {
      M[i] *= invScalar;
    }
  }
  else
  {
    for (int i = 0; i < M.getNumElements(); ++i)
    {
      M[i] = (Type)0;
    }
  }
  return M;
}

template <typename Type>
Type l1Norm(MathMatrix<Type> const& M)
{
  Type sum = std::abs(M[0]);
  for (int i = 1; i < M.getNumElements(); ++i)
  {
    sum += std::abs(M[i]);
  }
  return sum;
}

template <typename Type>
Type l2Norm(MathMatrix<Type> const& M)
{
  Type sum = M[0] * M[0];
  for (int i = 1; i < M.getNumElements(); ++i)
  {
    sum += M[i] * M[i];
  }
  return std::sqrt(sum);
}

template <typename Type>
Type lInfinityNorm(MathMatrix<Type> const& M)
{
  Type maxAbsElement = M[0];
  for (int i = 1; i < M.getNumElements(); ++i)
  {
    Type absElement = std::abs(M[i]);
    if (absElement > maxAbsElement)
    {
      maxAbsElement = absElement;
    }
  }
  return maxAbsElement;
}

template <typename Type>
MathMatrix<Type> inverse(MathMatrix<Type> const& M, bool* reportInvertibility)
{
  MathMatrix<Type> invM(M.getNumRows(), M.getNumCols());
  if (M.getNumRows() == M.getNumCols())
  {
    Type determinant;
    bool invertible = Gaussian<Type>()(M.getNumRows(), &M[0],
      &invM[0], determinant, nullptr, nullptr, nullptr, 0, nullptr);
    if (reportInvertibility)
    {
      *reportInvertibility = invertible;
    }
  }
  else
  {
    ODA_ASSERT(false);// ("Matrix must be square.");

    invM.makeZero();
    if (reportInvertibility)
    {
      *reportInvertibility = false;
    }
  }
  return invM;
}

template <typename Type>
Type determinant(MathMatrix<Type> const& M)
{
  Type determinant;
  if (M.getNumRows() == M.getNumCols())
  {
    Gaussian<Type>()(M.getNumRows(), &M[0], nullptr,
      determinant, nullptr, nullptr, nullptr, 0, nullptr);
  }
  else
  {
    ODA_FAIL_M_ONCE("Matrix must be square.");

    determinant = (Type)0;
  }
  return determinant;
}

template <typename Type>
MathMatrix<Type> transpose(MathMatrix<Type> const& M)
{
  MathMatrix<Type> result(M.getNumCols(), M.getNumRows());
  for (int r = 0; r < M.getNumRows(); ++r)
  {
    for (int c = 0; c < M.getNumCols(); ++c)
    {
      result(c, r) = M(r, c);
    }
  }
  return result;
}

template <typename Type>
MathVector<Type> operator*(MathMatrix<Type> const& M, MathVector<Type> const& V)
{
  ODA_ASSERT(V.getSize() == M.getNumRows()); //"Mismatched size."
  MathVector<Type> result(M.getNumRows());
  for (int r = 0; r < M.getNumRows(); ++r)
  {
    result[r] = (Type)0;
    for (int c = 0; c < M.getNumCols(); ++c)
    {
      result[r] += M(r, c) * V[c];
    }
  }
  return result;
}

template <typename Type>
MathVector<Type> operator*(MathVector<Type> const& V, MathMatrix<Type> const& M)
{
  ODA_ASSERT(V.getSize() == M.getNumCols()); //"Mismatched size."

  MathVector<Type> result(M.getNumCols());
  for (int c = 0; c < M.getNumCols(); ++c)
  {
    result[c] = (Type)0;
    for (int r = 0; r < M.getNumRows(); ++r)
    {
      result[c] += V[r] * M(r, c);
    }
  }
  return result;
}

template <typename Type>
MathMatrix<Type> operator*(MathMatrix<Type> const& A, MathMatrix<Type> const& B)
{
  return multiplyAB(A, B);
}

template <typename Type>
MathMatrix<Type> multiplyAB(MathMatrix<Type> const& A, MathMatrix<Type> const& B)
{
  ODA_ASSERT(A.getNumCols() == B.getNumRows()); //"Mismatched size."

  int const numCommon = A.getNumCols();
  MathMatrix<Type> result(A.getNumRows(), B.getNumCols());
  for (int r = 0; r < result.getNumRows(); ++r)
  {
    for (int c = 0; c < result.getNumCols(); ++c)
    {
      result(r, c) = (Type)0;
      for (int i = 0; i < numCommon; ++i)
      {
        result(r, c) += A(r, i) * B(i, c);
      }
    }
  }
  return result;
}

template <typename Type>
MathMatrix<Type> multiplyABT(MathMatrix<Type> const& A, MathMatrix<Type> const& B)
{
  ODA_ASSERT(A.getNumCols() == B.getNumCols()); //"Mismatched size."

  int const numCommon = A.getNumCols();
  MathMatrix<Type> result(A.getNumRows(), B.getNumRows());
  for (int r = 0; r < result.getNumRows(); ++r)
  {
    for (int c = 0; c < result.getNumCols(); ++c)
    {
      result(r, c) = (Type)0;
      for (int i = 0; i < numCommon; ++i)
      {
        result(r, c) += A(r, i) * B(c, i);
      }
    }
  }
  return result;
}

template <typename Type>
MathMatrix<Type> multiplyATB(MathMatrix<Type> const& A, MathMatrix<Type> const& B)
{
  ODA_ASSERT(A.getNumRows() == B.getNumRows()); //"Mismatched size."

  int const numCommon = A.getNumRows();
  MathMatrix<Type> result(A.getNumCols(), B.getNumCols());
  for (int r = 0; r < result.getNumRows(); ++r)
  {
    for (int c = 0; c < result.getNumCols(); ++c)
    {
      result(r, c) = (Type)0;
      for (int i = 0; i < numCommon; ++i)
      {
        result(r, c) += A(i, r) * B(i, c);
      }
    }
  }
  return result;
}

template <typename Type>
MathMatrix<Type> multiplyATBT(MathMatrix<Type> const& A, MathMatrix<Type> const& B)
{
  ODA_ASSERT(A.getNumRows() == B.getNumCols()); //"Mismatched size."

  int const numCommon = A.getNumRows();
  MathMatrix<Type> result(A.getNumCols(), B.getNumRows());
  for (int r = 0; r < result.getNumRows(); ++r)
  {
    for (int c = 0; c < result.getNumCols(); ++c)
    {
      result(r, c) = (Type)0;
      for (int i = 0; i < numCommon; ++i)
      {
        result(r, c) += A(i, r) * B(c, i);
      }
    }
  }
  return result;
}

template <typename Type>
MathMatrix<Type> multiplyMD(MathMatrix<Type> const& M, MathVector<Type> const& D)
{
  ODA_ASSERT(D.getSize() == M.getNumCols()); //"Mismatched size."

  MathMatrix<Type> result(M.getNumRows(), M.getNumCols());
  for (int r = 0; r < result.getNumRows(); ++r)
  {
    for (int c = 0; c < result.getNumCols(); ++c)
    {
      result(r, c) = M(r, c) * D[c];
    }
  }
  return result;
}

template <typename Type>
MathMatrix<Type> multiplyDM(MathVector<Type> const& D, MathMatrix<Type> const& M)
{
  ODA_ASSERT(D.getSize() == M.getNumRows()); //"Mismatched size."

  MathMatrix<Type> result(M.getNumRows(), M.getNumCols());
  for (int r = 0; r < result.getNumRows(); ++r)
  {
    for (int c = 0; c < result.getNumCols(); ++c)
    {
      result(r, c) = D[r] * M(r, c);
    }
  }
  return result;
}

template <typename Type>
MathMatrix<Type> outerProduct(MathVector<Type> const& U, MathVector<Type> const& V)
{
  MathMatrix<Type> result(U.getSize(), V.getSize());
  for (int r = 0; r < result.getNumRows(); ++r)
  {
    for (int c = 0; c < result.getNumCols(); ++c)
    {
      result(r, c) = U[r] * V[c];
    }
  }
  return result;
}

template <typename Type>
void makeDiagonal(MathVector<Type> const& D, MathMatrix<Type>& M)
{
  ODA_ASSERT(M.getNumRows() == M.getNumCols()); //"Mismatched size."

    int const N = M.getNumRows();
  for (int i = 0; i < N*N; ++i)
  {
    M[i] = (Type)0;
  }

  for (int i = 0; i < N; ++i)
  {
    M(i, i) = D[i];
  }
}

#endif //_IMPORT_MATH_MATRIX_INCLUDED_


