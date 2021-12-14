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

#if !defined(OD_POINTCLOUDSCANITERATOR_H__D198EF43_FEB9_4FB0_B0F8_061B760CE491__INCLUDED)
#define OD_POINTCLOUDSCANITERATOR_H__D198EF43_FEB9_4FB0_B0F8_061B760CE491__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "RootExport.h"
#include "SharedPtr.h"

#include "OdPointCloudScanDatabase.h"

/** <group OdRcs_Classes>
  \details
  This class is an interface class for the points cloud scan iterator that
  enables getting scans from a project.

  \remarks
  Corresponding C++ library: RcsFileServices.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdPointCloudScanIterator
{
public:

  /** \details
    Default destructor for objects of the OdPointCloudScanIterator class.
  */
  virtual ~OdPointCloudScanIterator(){}

  /** \details
    Moves the current position to the beginning of the scan data
    within the current point cloud project database.
  */
  virtual void start() = 0;

  /** \details
    Moves forward the current position to the next record of the
    scan data within the current point cloud project database.
  */
  virtual void step() = 0;
 
 /** \details
    Returns true if and only if the traversal by this iterator object is
    complete.

    \returns
    True if the traversal by this iterator object is complete; false otherwise.
  */
  virtual bool done() const = 0;

  /** \details
    Returns the pointer to the point cloud scan database that is currently
    pointed at by the iterator.

    \returns
    The pointer to the point cloud scan database.
  */
  virtual OdPointCloudScanDatabasePtr getScanDb() = 0;

  /** \details
    Gets the title of the scan, currently pointed by the iterator.

    \returns
    The scan title string.
  */
  virtual OdString getScanTitle() const = 0;

  /** \details
    Gets the value of the IsVisible flag of the scan, currently pointed by the
    iterator. The flag indicates whether the scan is visible.

    \returns
    True if the scan is visible; false otherwise.
  */
  virtual bool getScanIsVisible() const = 0;

  /** \details
    Returns the complete transformation matrix specified in the project data for
    the current scan pointed by the iterator.

    \returns
    Transformation matrix.
  */
  virtual OdGeMatrix3d getScanTransform() const = 0;
};

/** \details
  Shared pointer to objects of the OdPointCloudScanIterator class.
*/
typedef OdSharedPtr<OdPointCloudScanIterator> OdPointCloudScanIteratorPtr;

#include "TD_PackPop.h"

#endif
