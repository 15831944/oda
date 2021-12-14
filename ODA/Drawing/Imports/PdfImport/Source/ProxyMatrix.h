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

#ifndef _IMPORT_PROXY_MATRIX_INCLUDED_
#define _IMPORT_PROXY_MATRIX_INCLUDED_


template <bool RowMajor, typename Type>
class ProxyMatrix
{
public:
  inline ProxyMatrix(int num_rows, int num_cols, Type* matrix);

  inline int getNumRows() const;
  inline int getNumCols() const;
  inline Type& operator()(int r, int c);
  inline Type const& operator()(int r, int c) const;

private:
  int m_NumRows, m_NumCols;
  Type* m_Matrix;
};

template <bool RowMajor, typename Type> inline
ProxyMatrix<RowMajor, Type>::ProxyMatrix(int num_rows, int num_cols, Type* matrix)
  :
  m_NumRows(num_rows),
  m_NumCols(num_cols),
  m_Matrix(matrix)
{
}

template <bool RowMajor, typename Type> inline
int ProxyMatrix<RowMajor, Type>::getNumRows() const
{
  return m_NumRows;
}

template <bool RowMajor, typename Type> inline
int ProxyMatrix<RowMajor, Type>::getNumCols() const
{
  return m_NumCols;
}

template <bool RowMajor, typename Type> inline
Type& ProxyMatrix<RowMajor, Type>::operator()(int r, int c)
{
  if(RowMajor)
    return m_Matrix[c + m_NumCols*r];
  else
    return m_Matrix[r + m_NumRows*c];
}

template <bool RowMajor, typename Type> inline
Type const& ProxyMatrix<RowMajor, Type>::operator()(int r, int c) const
{
  if(RowMajor)
    return m_Matrix[c + m_NumCols*r];
  else
    return m_Matrix[r + m_NumRows*c];
}

#endif //_IMPORT_PROXY_MATRIX_INCLUDED_ 

