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

#if !defined(OD_RCSVISIBLEVOXELSITER_H__C8858A6D_D0D1_4DE6_B20E_260671F5523D__INCLUDED)
#define OD_RCSVISIBLEVOXELSITER_H__C8858A6D_D0D1_4DE6_B20E_260671F5523D__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "RootExport.h"
#include "SharedPtr.h"

#include "Ge/GePoint3dArray.h"
#include "CmEntityColorArray.h"

/** <group OdRcs_Classes>
  \details
  This is an interface class to iterate through the voxels visible in the
  current viewport.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRcsVisibleVoxelsIterator
{
public:

  /** \details
    Default destructor for objects of the OdRcsVisibleVoxelsIterator class.
  */
  virtual ~OdRcsVisibleVoxelsIterator() {}

  /** \details
    Moves the current iterator position to the visible voxel in the current
    viewport that will be iterated first.
  */
  virtual void start() = 0;

  /** \details
    Pteps the current iterator position to the next visible voxel in the current
    viewport.
  */
  virtual void step() = 0;

  /** details
    Indicates whether the traversal by this iterator object is complete.

    \returns
    True if the traversal by this iterator object is complete; false otherwise.
  */
  virtual bool done() const = 0;

  /** \details
    Loads the voxel data from the voxel that is currently pointed at by the
    visible voxel data iterator.

    \param coords [out] List of loaded points coordinates for the currenly pointed voxel.
    \param colors [out] List of loaded points colors for the currenly pointed voxel.
    \param numberOfPointsToDraw [out] Total number of visible points to be drawn.
    \param voxelIndex [out] Index of the voxel in scan database.

    \returns
    The total number of loaded points.
  */
  virtual OdUInt32 getVoxelBuffer(OdGePoint3dArray& coords, OdCmEntityColorArray& colors,
    OdUInt32& numberOfPointsToDraw, OdUInt64& voxelIndex) = 0;

  /** \details
    Returns the number of points by current LOD calculated for the visible voxel.

    \returns
    The number of points.
  */
  virtual OdUInt32 getNumberOfPointsByCurrentLOD() = 0;
};

/** \details
  Shared pointer to objects of the OdRcsVisibleVoxelsIterator class.
*/
typedef OdSharedPtr<OdRcsVisibleVoxelsIterator> OdRcsVisibleVoxelsIteratorPtr;

#include "TD_PackPop.h"

#endif
