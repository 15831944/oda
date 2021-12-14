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

#if !defined(OD_POINTCLOUDPROJECTDATABASE_H__D38E354C_DA17_4C07_917D_8A97547D877F__INCLUDED)
#define OD_POINTCLOUDPROJECTDATABASE_H__D38E354C_DA17_4C07_917D_8A97547D877F__INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TD_PackPush.h"

#include "RootExport.h"
#include "SharedPtr.h"

#include "Ge/GeExtents3d.h"
#include "StringArray.h"
#include "Ge/GeMatrix3d.h"

#include "OdPointCloudScanIterator.h"

/** <group OdRx_Classes>
  \details
  This is an interface class for the point cloud project database.

  \remarks
  Corresponding C++ library: RcsFileServices.
*/
class ODRX_ABSTRACT FIRSTDLL_EXPORT OdPointCloudProjectDatabase
{
public:

  /** \details
    Default destructor for objects of the OdPointCloudProjectDatabase class.
  */
  virtual ~OdPointCloudProjectDatabase(){}

  /** \details
    Gets the path to an .rcp file associated with the database.

    \returns
    Path to the .rcp file.
  */
  virtual OdString getProjectDatabaseFilePath() const = 0;

  /** \details
    Creates a new point cloud scan iterator and returns a shared pointer to it.

    \returns
    Shared pointer to the created scan iterator.
  */
  virtual OdPointCloudScanIteratorPtr getScanIterator() const = 0;

  /** \details
    Gets the list of paths for all .rcs files associated with the .rcp file.
    
    \param list [in/out] An array of strings to be filled with paths to the .rcs
                         files.
  */
  virtual void getAllRcsFilePaths(OdStringArray &list) const = 0;

  /** \details
    Gets the list of relative paths for all .rcs files associated with the .rcp
    file.
    
    \param list [in/out] An array of strings to be filled with relative paths to
                         the .rcs files.
  */
  virtual void getAllRcsRelativeFilePaths( OdStringArray &list ) const = 0;

  /** \details
    Gets the global transformation matrix.

    \returns
    Transformation matrix.
  */
  virtual OdGeMatrix3d getGlobalTransformation() const = 0;

  /** \details
    Gets the scan transformation matrix.

    \param guid [in] ID of the scan (.rcs file) to get the transformation matrix
                     for.

    \returns
    Transformation matrix.
  */
  virtual OdGeMatrix3d getScanTransform(const OdString &guid) const = 0;

  /** \details
    Gets the total regions count.

    \returns
    The number of regions as an OdUInt32 value.
  */
  virtual OdUInt32 getTotalRegionsCount() const = 0;

  /** \details
    Gets the total scans count.

    \returns
    The number of scans as an OdUInt32 value.
  */
  virtual OdUInt32 getTotalScansCount() const = 0;

  /** \details
    Gets the coordinate system name.

    \returns
    The coordinate system name.
  */
  virtual OdString getCoordinateSystemName() const = 0;

  /** \details
    Indicates whether the associated .rcs files contain colors.

    \returns
    1 if all .rcs files associated with the currant .rcp file contains colors.
    0 if at least one .rcs file associated with the currant .rcp file contains colors.
    -1 if no .rcs file associated with the currant .rcp file contain colors.
  */
  virtual OdInt8 hasRGB() const = 0;

  /** \details
    Indicates whether the associated .rcs files contain normals.

    \returns
    1 if all .rcs files associated with the currant .rcp file contains normals.
    0 if at least one .rcs file associated with the currant .rcp file contains normals.
    -1 if no .rcs file associated with the currant .rcp file contain normals.
  */
  virtual OdInt8 hasNormals() const = 0;

  /** \details
    Indicates whether the associated .rcs files contain intensity.

    \returns
    1 if all .rcs files associated with the currant .rcp file contains intensity.
    0 if at least one .rcs file associated with the currant .rcp file contains intensity.
    -1 if no .rcs file associated with the currant .rcp file contain intensity.
  */
  virtual OdInt8 hasIntensity() const = 0;

  /** \details
    Gets the path to an .rcs file specified by a guid.

    \param guid [in] guid of the .rcs file.

    \returns
    Path to the .rcs file.
  */
  virtual OdString getRcsFilePath(const OdString &guid) const = 0;

  /** \details
    Gets the relative path to an .rcs file specified by a guid.

    \param guid [in] guid of the .rcs file.

    \returns
    Relative path to the .rcs file.
  */
  virtual OdString getRcsRelativeFilePath(const OdString &guid) const = 0;

  /** \details
    Gets the total number of points for the point cloud project.

    \returns
    The total number of points as an OdUInt64 value.
  */
  virtual OdUInt64 getTotalAmountOfPoints() const = 0;

  /** \details
    Gets the full extents of the point cloud project.

    \returns
    The extents of the point cloud project.
  */
  virtual OdGeExtents3d getExtents() const = 0;


  /** \details
    Writes all point cloud project XML data to a specified stream.

    \param s [in/out] A stream to write data to.
  */
  virtual void writeAllXmlDataToStream(OdStreamBuf* s) = 0;
};

/** \details
  Shared pointer to objects of the
  <link OdPointCloudProjectDatabase, OdPointCloudProjectDatabase> class.
*/
typedef OdSharedPtr<OdPointCloudProjectDatabase> OdPointCloudProjectDatabasePtr;

#include "TD_PackPop.h"

#endif
