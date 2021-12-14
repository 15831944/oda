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

#ifndef _IMPORT_GAUSSIAN_INCLUDED_
#define _IMPORT_GAUSSIAN_INCLUDED_

#include "ProxyMatrix.h"
#include <cmath>
#include <vector>

template <typename Type>
class Gaussian
{
public:
  bool operator()(int num_rows,
    Type const* M, Type* inverseM, Type& determinant,
    Type const* B, Type* X,
    Type const* C, int num_cols, Type* Y) const;

private:
  void set(int num_elements, Type const* source, Type* target) const;
};


template <typename Type>
bool Gaussian<Type>::operator()(int num_rows, Type const* M,
  Type* inverseM, Type& determinant, Type const* B, Type* X, Type const* C,
  int num_cols, Type* Y) const
{
  if (num_rows <= 0 || !M
    || ((B != nullptr) != (X != nullptr))
    || ((C != nullptr) != (Y != nullptr))
    || (C != nullptr && num_cols < 1))
  {
    ODA_ASSERT(false);
    return false;
  }

  int num_elements = num_rows * num_rows;
  bool wantInverse = (inverseM != nullptr);
  std::vector<Type> localInverseM;
  if (!wantInverse)
  {
    localInverseM.resize(num_elements);
    inverseM = localInverseM.data();
  }
  set(num_elements, M, inverseM);

  if (B)
  {
    set(num_rows, B, X);
  }

  if (C)
  {
    set(num_rows * num_cols, C, Y);
  }

#if defined(IS_ROW_MAJOR)
  ProxyMatrix<true, Type> matInvM(num_rows, num_rows, inverseM);
  ProxyMatrix<true, Type> matY(num_rows, num_cols, Y);
#else
  ProxyMatrix<false, Type> matInvM(num_rows, num_rows, inverseM);
  ProxyMatrix<false, Type> matY(num_rows, num_cols, Y);
#endif

  std::vector<int> colIndex(num_rows), rowIndex(num_rows), pivoted(num_rows);
  std::fill(pivoted.begin(), pivoted.end(), 0);

  Type const zero = (Type)0;
  Type const one = (Type)1;
  bool odd = false;
  determinant = one;

  int i1, i2, row = 0, col = 0;
  for (int i0 = 0; i0 < num_rows; ++i0)
  {
    Type max_value = zero;
    for (i1 = 0; i1 < num_rows; ++i1)
    {
      if (!pivoted[i1])
      {
        for (i2 = 0; i2 < num_rows; ++i2)
        {
          if (!pivoted[i2])
          {
            Type abs_value = std::abs(matInvM(i1, i2));
            if (abs_value > max_value)
            {
              max_value = abs_value;
              row = i1;
              col = i2;
            }
          }
        }
      }
    }

    if (max_value == zero)
    {
      if (wantInverse)
      {
        set(num_elements, nullptr, inverseM);
      }
      determinant = zero;

      if (B)
      {
        set(num_rows, nullptr, X);
      }

      if (C)
      {
        set(num_rows * num_cols, nullptr, Y);
      }
      return false;
    }

    pivoted[col] = true;

    if (row != col)
    {
      odd = !odd;
      for (int i = 0; i < num_rows; ++i)
      {
        std::swap(matInvM(row, i), matInvM(col, i));
      }

      if (B)
      {
        std::swap(X[row], X[col]);
      }

      if (C)
      {
        for (int i = 0; i < num_cols; ++i)
        {
          std::swap(matY(row, i), matY(col, i));
        }
      }
    }

    rowIndex[i0] = row;
    colIndex[i0] = col;

    Type diagonal = matInvM(col, col);
    determinant *= diagonal;
    Type inv = one / diagonal;
    matInvM(col, col) = one;
    for (i2 = 0; i2 < num_rows; ++i2)
    {
      matInvM(col, i2) *= inv;
    }

    if (B)
    {
      X[col] *= inv;
    }

    if (C)
    {
      for (i2 = 0; i2 < num_cols; ++i2)
      {
        matY(col, i2) *= inv;
      }
    }

    for (i1 = 0; i1 < num_rows; ++i1)
    {
      if (i1 != col)
      {
        Type save = matInvM(i1, col);
        matInvM(i1, col) = zero;
        for (i2 = 0; i2 < num_rows; ++i2)
        {
          matInvM(i1, i2) -= matInvM(col, i2) * save;
        }

        if (B)
        {
          X[i1] -= X[col] * save;
        }

        if (C)
        {
          for (i2 = 0; i2 < num_cols; ++i2)
          {
            matY(i1, i2) -= matY(col, i2) * save;
          }
        }
      }
    }
  }

  if (wantInverse)
  {
    for (i1 = num_rows - 1; i1 >= 0; --i1)
    {
      if (rowIndex[i1] != colIndex[i1])
      {
        for (i2 = 0; i2 < num_rows; ++i2)
        {
          std::swap(matInvM(i2, rowIndex[i1]),
            matInvM(i2, colIndex[i1]));
        }
      }
    }
  }

  if (odd)
  {
    determinant = -determinant;
  }

  return true;
}

template <typename Type>
void Gaussian<Type>::set(int num_elements, Type const* source,
  Type* target) const
{
  if (std::is_floating_point<Type>() == std::true_type())
  {
    // native floating-point
    size_t numBytes = num_elements * sizeof(Type);
    if (source)
    {
      memcpy(target, source, numBytes);
    }
    else
    {
      memset(target, 0, numBytes);
    }
  }
  else
  {
    if (source)
    {
      for (int i = 0; i < num_elements; ++i)
      {
        target[i] = source[i];
      }
    }
    else
    {
      Type const zero = (Type)0;
      for (int i = 0; i < num_elements; ++i)
      {
        target[i] = zero;
      }
    }
  }
}

#endif // _IMPORT_GAUSSIAN_INCLUDED_

