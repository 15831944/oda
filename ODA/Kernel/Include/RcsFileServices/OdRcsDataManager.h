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

#if !defined(OD_RCSDATAMANAGER_H__54E14989_A680_453D_A748_75444A09BF00__INCLUDED)
#define OD_RCSDATAMANAGER_H__54E14989_A680_453D_A748_75444A09BF00__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "RootExport.h"
#include "SharedPtr.h"

#include "Gi/GiViewport.h"
#include "Gi/GiPointCloud.h"

#include "OdPointCloudProjectDatabase.h"

#include "OdRcsVisibleVoxelsIterator.h"

/** <group OdRcs_Classes>
  \details
  This is an interface class to calculate view-dependent LOD and to process
  visible voxels.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdRcsDataManager
{
public:

  /** \details
    Default destructor for objects of the OdRcsDataManager class.
  */
  virtual ~OdRcsDataManager() {}


  /** \details
    Gets the pointer to scan database.

    \returns
    Pointer to scan database.
 */
  virtual OdPointCloudScanDatabasePtr getScanDb() const = 0;


  /** \details
    Calculates the total number of points in all voxels.

    \returns
    Total number of points as an OdUInt64 value.
 */
  virtual OdUInt64 pointsCount() const = 0;

  /** \details
    Updates the list of visible voxels within a viewport given the parameters.

    \param viewport [in] The viewport.
    \param pointSize [in] The point size that is considered visible.    
  */
  virtual void updateListOfVisibleVoxels(const OdGiViewport& viewport, OdUInt16 pointSize) = 0;

  /** \details
    Loads the visible points of all visible voxels for this rcs data manager.
  */
  virtual void loadPointsForVisibleVoxels() = 0;

  /** \details
    Creates a new visible voxels iterator.

    \returns
    Shared pointer to a OdRcsVisibleVoxelsIterator object.
  */
  virtual OdRcsVisibleVoxelsIteratorPtr newVisibleVoxelsIterator() const = 0;

  /** \details
    Creates a new instance of OdGiPointCloud.

    \param pointSize [in] Point size value.
  */
  virtual OdGiPointCloudPtr newGiPointCloud(OdUInt16 pointSize) = 0;
};

/** \details
  Shared pointer to objects of the OdRcsDataManager class.
*/
typedef OdSharedPtr<OdRcsDataManager> OdRcsDataManagerPtr;

#include "TD_PackPop.h"

#endif
