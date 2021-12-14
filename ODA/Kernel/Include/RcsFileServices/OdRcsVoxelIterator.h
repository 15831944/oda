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

#if !defined(OD_RCSVOXELITERATOR_H__4168B690_512C_46A3_BB52_EAF7E896BC01__INCLUDED)
#define OD_RCSVOXELITERATOR_H__4168B690_512C_46A3_BB52_EAF7E896BC01__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "RootExport.h"
#include "SharedPtr.h"

#include "OdRcsVoxel.h"

/** <group OdRcs_Classes>
  \details
  This is an interface class for the voxel data iterator that enables getting
  voxels from a scan database.

  \remarks
  Corresponding C++ library: RcsFileServices.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRcsVoxelIterator
{
public:

  /** \details
    Default destructor for objects of the OdRcsVoxelIterator class.
  */
  virtual ~OdRcsVoxelIterator(){}

  /** \details
    Moves the current position to the beginning of the voxel data.
  */
  virtual void start() = 0;

  /** \details
    Moves the current position to the next record of the voxel data.
  */
  virtual void step() = 0;

  /** details
    Indicates whether the traversal by this iterator object is complete.

    \returns
    True if the traversal by this iterator object is complete; false otherwise.
  */
  virtual bool done() const = 0;

  /** \details
    Returns the pointer to the voxel that is currently pointed at by the voxel
    data iterator.

    \returns
    The pointer to the current voxel.
  */
  virtual OdRcsVoxelPtr getVoxel() = 0;
};

/** \details
  Shared pointer to objects of the OdRcsVoxelIterator class.
*/
typedef OdSharedPtr<OdRcsVoxelIterator> OdRcsVoxelIteratorPtr;

#include "TD_PackPop.h"

#endif
