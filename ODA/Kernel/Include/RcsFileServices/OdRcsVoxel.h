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

#if !defined(OD_RCSVOXEL_H__160BB746_FAC9_4495_8A7D_8CF201D6D3A0__INCLUDED)
#define OD_RCSVOXEL_H__160BB746_FAC9_4495_8A7D_8CF201D6D3A0__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "RootExport.h"
#include "SharedPtr.h"

#include "OdRcsPointDataIterator.h"
#include "Ge/GeExtents3d.h"

/** <group OdRcs_Classes>
  \details
  This is an interface class for the voxel cube containing points.

  \remarks
  Corresponding C++ library: RcsFileServices.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRcsVoxel
{
public:

  /** \details
    Default destructor for objects of the OdRcsVoxel class.
  */
  virtual ~OdRcsVoxel(){}

  /** \details
    Gets the number of points within the currant voxel.

    \returns
    Number of points.
  */
  virtual OdUInt32 getTotalNumberOfPoints() const = 0;

  /** \details
    Creates a new point data iterator.

    \returns
    The shared pointer to the object of the OdRcsPointDataIterator class.
  */
  virtual OdRcsPointDataIteratorPtr getPointDataIterator() const = 0;

  /** \details
    Returns the extents of the voxel.
    
    \returns
    The extents of the voxel.
  */
  virtual OdGeExtents3d getExtents() const = 0;
};

/** \details
  Shared pointer to objects of the OdRcsVoxel class.
*/
typedef OdSharedPtr<OdRcsVoxel> OdRcsVoxelPtr;

#include "TD_PackPop.h"

#endif
