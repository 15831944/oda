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

#if !defined(OD_POINTCLOUDSCANDATABASE_H__594EAF5C_A75B_4BA6_A439_251788E15302__INCLUDED)
#define OD_POINTCLOUDSCANDATABASE_H__594EAF5C_A75B_4BA6_A439_251788E15302__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "RootExport.h"
#include "SharedPtr.h"

#include "OdRcsVoxelIterator.h"

#include "Ge/GeVector3d.h"
#include "Ge/GeMatrix3d.h"

/** <group OdRx_Classes>
 \details
  This is an interface class for the point cloud scan database.

  \remarks
  Corresponding C++ library: RcsFileServices.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdPointCloudScanDatabase
{
public:

  /** \details
    Default destructor for objects of the OdPointCloudScanDatabase class.
  */
  virtual ~OdPointCloudScanDatabase(){}

  /** \details
    Gets the path to an .rcs file associated with the database.

    \returns
    Path to the .rcs file.
  */
  virtual OdString getScanDatabaseFilePath() const = 0;

  /** \details
    Returns the total number of points in all voxels.

    \returns
    Total number of points as an OdUInt64 value.
  */
  virtual OdUInt64 getTotalAmountOfPoints() const = 0;

  /** \details
    Gets the scan ID string.

    \returns
    The scan ID string.
  */
  virtual OdString getScanId() const = 0;

  /** \details
    Indicates whether the file contains LIDAR data.

    \returns
    True if the file contains LIDAR data; false otherwise.
  */
  virtual bool isLidarData() const = 0;

  /** \details
    Creates a new voxel iterator and returns a shared pointer to it.

    \returns
    A shared pointer to the created voxel iterator.
  */
  virtual OdRcsVoxelIteratorPtr getVoxelIterator() const = 0;

  /** \details
    Gets the amount of voxels in the database.

    \returns
    The amount of voxels as an OdUInt64 value.
  */
  virtual OdUInt64 getAmountOfVoxels() const = 0;

  /** \details
    Gets the translation vector from the database.

    \returns
    Translation vector.
  */
  virtual OdGeVector3d getTranslation() const = 0;

  /** \details
    Gets the rotation vector from the database. Each of this vector's
    coordinates is a rotation angle around the corresponding axis.

    \returns
    Rotation vector.
  */
  virtual OdGeVector3d getRotation() const = 0;

  /** \details
    Gets the scale vector from the database.

    \returns
    Scale vector.
  */
  virtual OdGeVector3d getScale() const = 0;

  /** \details
    Forms the complete transformation matrix which integrates translation,
    rotation and scale.

    \returns
    Transformation matrix.
  */
  virtual OdGeMatrix3d getTransformMatrix() const = 0;

  /** \details
    Gets the hasRGB flag value from the database.

    \returns
    The value of the hasRGB flag.
  */
  virtual bool hasRGB() const = 0;

  /** \details
    Gets the hasNormals flag value from the database.

    \returns
    The value of the hasNormals flag.
  */
  virtual bool hasNormals() const = 0;

  /** \details
    Gets the hasIntensity flag value from the database.

    \returns
    The value of the hasIntensity flag.
  */
  virtual bool hasIntensity() const = 0;

  /** \details
    Gets the extents of the point cloud from the database.

    \returns
    The extents.
  */
  virtual OdGeExtents3d getExtents() const = 0;

  /** \details
    Gets the extents of the point cloud and transforms it by the transformation
    matrix.

    \returns
    The transformed extents.
  */
  virtual OdGeExtents3d getTransformedExtents() const = 0;
  
  /** \details
    Gets a flag indicating whether normalization is used for intensity values.

    \returns
    Value of the flag as bool.
  */
  virtual bool getNormalizeIntensity() const = 0;

  /** \details
    Returns the maximum intensity of all scan points from the current scan
    database.

    \returns
    Maximum intensity of the scan database.
  */
  virtual float getMaxIntensity() const = 0;

  /** \details
    Returns the minimum intensity of all scan points from the current scan
    database.

    \returns
    Minimum intensity of the scan database.
  */
  virtual float getMinIntensity() const = 0;

  /** \details
    Returns the range image width.
    
    \returns
    Range image width.
  */
  virtual OdUInt32 getRangeImageWidth() const = 0;

  /** \details
    Returns the range image heigth.

    \returns
    Range image heigth.
  */
  virtual OdUInt32 getRangeImageHeight() const = 0;
};

/** \details
  Shared pointer to objects of the OdPointCloudScanDatabase class.
*/
typedef OdSharedPtr<OdPointCloudScanDatabase> OdPointCloudScanDatabasePtr;

#include "TD_PackPop.h"

#endif
