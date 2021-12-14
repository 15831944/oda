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


#ifndef __OD_DB_POINCLOUD_HOST__
#define __OD_DB_POINCLOUD_HOST__

#include "DbPointCloudObjExports.h"
#include "Ge/GeExtents3d.h"

class OdDbPointCloud;
class OdDbPointCloudDef;
class OdGiWorldDraw;
class OdGiViewportDraw;
class OdDbPointCloudEx;
class OdDbPointCloudDefEx;

#include "TD_PackPush.h"

#include "StringArray.h"

/** <group OdDbPointCloud_Classes>
  \details
  This class represents an interface of an OdDbPointCloud object within the ODA
  Drawings vectorization framework.
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudItem : public OdRxObject
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbPointCloudItem);
//DOM-IGNORE-END

  /** \details
    Returns the extents of the Point Cloud Object.

    \returns
    The extents as an OdGeExtents3d object.
  */
  virtual OdGeExtents3d extents() const = 0;

  /** \details
    Returns the total number of points in the Point Cloud Object.

    \returns
    Number of points as an OdInt64 value.
  */
  virtual OdInt64 pointsCount() const = 0;

  /** \details
    Creates a viewport-independent geometric representation of a Point Cloud
    Entity.

    \returns
    Returns true if and only if the geometric representation can be generated in
    a viewport-independent manner.

    \remarks
    A return value of false indicates that viewportDrawPoints() must be called
    for this object.
    Transformations and clipping boundaries are taken care of outside of this
    function by the ODA Platform.
    The 3D GS will call this function at least once, but may cache subsequent
    display updates.

    \param pEnt [in] Pointer to the OdDbPointCloud entity.
    \param pWd  [in] Pointer to the OdGiWorldDraw interface.
  */
  virtual bool worldDrawPoints(const OdDbPointCloud* pEnt, OdGiWorldDraw* pWd) const = 0;

  /** \details
    Creates a viewport-dependent geometric representation of a Point Cloud
    Entity.

    \remarks
    This function is called once per viewport.
    Transformations and clipping boundaries are taken care of outside of this
    function by the ODA Platform.

    \param pEnt [in] Pointer to the OdDbPointCloud entity.
    \param pVd  [in] Pointer to the OdGiViewportDraw interface.
  */
  virtual void viewportDrawPoints(const OdDbPointCloud* pEnt, OdGiViewportDraw* pWd) const = 0;

};

/** \details
  Smart pointer to objects of the OdDbPointCloudItem class.
*/
typedef OdSmartPtr<OdDbPointCloudItem> OdDbPointCloudItemPtr;

/** <group OdDbPointCloud_Classes>
  \details
  This class represents an interface of an OdDbPointCloudEx object within the
  ODA Drawings vectorization framework.
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudExItem : public OdRxObject
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbPointCloudExItem);
//DOM-IGNORE-END

  /** \details
    Default destructor for objects of the OdDbPointCloudExItem class.
  */
  virtual ~OdDbPointCloudExItem() {}

  /** \details
    Returns the extents of the Point Cloud Object.

    \returns
    The extents as an OdGeExtents3d object.
  */
  virtual OdGeExtents3d extents() const = 0;

  /** \details
    Returns the total number of points in the Point Cloud Object.

    \returns
    Number of points as an OdInt64 value.
  */
  virtual OdUInt64 pointsCount() const = 0;

  /** \details
    Creates a viewport-independent geometric representation of a Point Cloud
    Entity.

    \returns
    Returns true if and only if the geometric representation can be generated in
    a viewport-independent manner.

    \remarks
    A return value of false indicates that viewportDrawPoints() must be called
    for this object.
    Transformations and clipping boundaries are taken care of outside of this
    function by the ODA Platform.
    The 3D GS will call this function at least once, but may cache subsequent
    display updates.
        
    \param pWd  [in] Pointer to the OdGiWorldDraw interface.
  */
  virtual bool worldDrawPoints(OdGiWorldDraw* pWd) const = 0;

  /** \details
    Creates a viewport-dependent geometric representation of a Point Cloud
    Entity.

    \remarks
    This function is called once per viewport.
    Transformations and clipping boundaries are taken care of outside of this
    function by the ODA Platform.
        
    \param pVd  [in] Pointer to the OdGiViewportDraw interface.
  */
  virtual void viewportDrawPoints(OdGiViewportDraw* pVd) const = 0;  
};

/** \details
  Smart pointer to objects of the OdDbPointCloudExItem class.
*/
typedef OdSmartPtr<OdDbPointCloudExItem> OdDbPointCloudExItemPtr;

/** <group OdDbPointCloud_Classes>
  \details
  This class represents an interface of an OdDbPointCloudEx object within the
  ODA Drawings vectorization framework. The class is designed to work with a
  separate scan. This kind of an OdDbPointCloudEx object links a .rcs file.
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudExScanItem : public OdDbPointCloudExItem
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbPointCloudExScanItem);
//DOM-IGNORE-END

  /** \details
    Default destructor for objects of the OdDbPointCloudExScanItem class.
  */
  virtual ~OdDbPointCloudExScanItem() {}
  
  /** \details
    Returns the value of the hasRGB flag which indicates whether the linked scan
    has the kColor property.

    \returns
    The hasRGB flag as a boolean value.
  */
  virtual bool hasRGB() const = 0;
  
  /** \details
    Returns the value of the hasNormals flag which indicates whether the linked
    scan has the kNormal property.

    \returns
    The hasNormals flag as a boolean value.
  */
  virtual bool hasNormals() const = 0;
  
  /** \details
    Returns the value of the hasIntensity flag which indicates whether the
    linked scan has the kIntensity property.

    \returns
    The hasIntensity flag as a boolean value.
  */
  virtual bool hasIntensity() const = 0;

  /** \details
    Returns the ID string of the linked scan.

    \returns
    The scan ID as the OdString value.
  */
  virtual OdString getScanId() const = 0;
};

/** \details
  Smart pointer to objects of the OdDbPointCloudExScanItem class.
*/
typedef OdSmartPtr<OdDbPointCloudExScanItem> OdDbPointCloudExScanItemPtr;


/** <group OdDbPointCloud_Classes>
  \details
  This class represents an interface of an OdDbPointCloudEx object within the
  ODA Drawings vectorization framework. The class is designed to work with a
  project that combines multiple scans. This kind of an OdDbPointCloudEx object
  links a .rcp file.
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudExProjItem : public OdDbPointCloudExItem
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbPointCloudExProjItem);
//DOM-IGNORE-END

  /** \details
    Default destructor for objects of the OdDbPointCloudExProjItem class.
  */
  virtual ~OdDbPointCloudExProjItem() {}

  /** \details
    Gets the list of paths for all .rcs files of the project.
    
    \param list [in/out] An array of strings to be filled with paths to the .rcs
                         files.
  */
  virtual void getAllRcsFilePaths( OdStringArray &list ) const = 0;

  /** \details
    Gets the list of relative paths for all .rcs files of the project.
    
    \param list [in/out] An array of strings to be filled with relative paths to
                         the .rcs files.
  */
  virtual void getAllRcsRelativeFilePaths( OdStringArray &list ) const = 0;

  /** \details
    Gets the total amount of scan items within the project item.

    \returns
    Number of scan items as an OdUInt64 value.
  */
  virtual OdUInt64 getTotalAmountOfScanItems() const = 0;
  
  /** \details
    Gets the total amount of regions in the project.
    
    \returns
    Number of regions as OdUInt32 value.
  */
  virtual OdUInt32 getTotalRegionsCount() const = 0;

  /** \details
    Gets the total amount of scans (.rcs files) linked to the project.
    
    \returns
    Number of scans as an OdUInt32 value.
  */
  virtual OdUInt32 getTotalScansCount() const = 0;

  /** \details
    Gets the the coordinate system name of the point cloud project item.
    
    \returns
    The name of the coordinate system.
  */
  virtual OdString getCoordinateSystemName() const = 0;

  /** \details
    Returns the value of the hasRGB flag which indicates whether the contained
    scan items have the kColor property.

    \returns
    1 if all scan items of the project item have kColor property.
    0 if at least one scan item of the project item has kColor property.
    -1 if no scan items of the project item have kColor property.
  */
  virtual OdInt8 hasRGB() const = 0;  

  /** \details
    Returns the value of the hasNormal flag which indicates whether the
    contained scan items have the kNormal property.

    \returns
    1 if all scan items of the project item have kNormal property.
    0 if at least one .scan item of the project item has kNormal property.
    -1 if no scan items of the project item have kNormal property.
  */
  virtual OdInt8 hasNormals() const = 0;  

  /** \details
    Returns the value of the hasIntensity flag which indicates whether the
    contained scan items have the kIntensity property.

    \returns
    1 if all scan items of the project item have kIntensity property.
    0 if at least one .scan item of the project item has kIntensity property.
    -1 if no scan items of the project item have kIntensity property.
  */
  virtual OdInt8 hasIntensity() const = 0;

  /** \details
    Gets the path for .rcs file with given ID.
    
    \param guid [in] ID of the file.

    \returns
    The string containing the path to the .rcs file.
  */
  virtual OdString getRcsFilePath(const OdString &guid) const = 0;

  /** \details
    Gets the relative path for .rcs file with given ID.
    
    \param guid [in] ID of the file.

    \returns
    The string containing the relative path to the .rcs file.
  */
  virtual OdString getRcsRelativeFilePath(const OdString &guid) const = 0;
};

/** \details
  Smart pointer to objects of the OdDbPointCloudExProjItem class.
*/
typedef OdSmartPtr<OdDbPointCloudExProjItem> OdDbPointCloudExProjItemPtr;

/** <group OdDbPointCloud_Classes>
  \details
  This protocol extension, attached to the OdDbPointCloudDef class,
  returns a pointer to an object of the class implementing the
  OdDbPointCloudItem interface given a file path to a Point Cloud file.
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudHostPE : public OdRxObject
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbPointCloudHostPE);
//DOM-IGNORE-END

  /** \details
    Default constructor for objects of the OdDbPointCloudHostPE class.
  */
  OdDbPointCloudHostPE() {}

  /** \details
    Returns a pointer to an object of the class implementing the
    OdDbPointCloudItem interface given a file path to a Point Cloud file.

    \param strPath [in] File path.
    \param item [out] Pointer to the object of the class implementing the
                      OdDbPointCloudItem interface for a specified file.

    \returns
    Status of the operation as the OdResult value.

    \remarks
    Not implemented. If called, returns eNotImplemented.
  */
  virtual OdResult load(const OdString& strPath, OdDbPointCloudItemPtr& item) = 0;
};

/** \details
  Smart pointer to objects of the OdDbPointCloudHostPE class.
*/
typedef OdSmartPtr<OdDbPointCloudHostPE> OdDbPointCloudHostPEPtr;

/** <group OdDbPointCloud_Classes>
  \details
  This protocol extension, attached to the OdDbPointCloudDefEx class, returns
  a pointer to an object of the class implementing the OdDbPointCloudItem
  interface given a file path to a Point Cloud file.
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudExHostPE : public OdRxObject
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbPointCloudExHostPE);
//DOM-IGNORE-END

  /** \details
    Default constructor for objects of the OdDbPointCloudExHostPE class.
  */
  OdDbPointCloudExHostPE() {}

  /** \details
    Returns a pointer to an object of the class implementing the
    OdDbPointCloudItem interface given the path to a Point Cloud scan file in
    .rcs format.

    \param strPath [in] File path.
    \param item   [out] Pointer to the object of the class implementing the
                        OdDbPointCloudExItem interface for the specified file.

    \returns
    Status of the operation as the OdResult value.

    \remarks
    Not implemented. If called, returns eNotImplemented.
  */
  virtual OdResult loadRcs(const OdString& strPath, OdDbPointCloudExItemPtr& item) = 0;

  /** \details
    Returns a pointer to an object of the class implementing the
    OdDbPointCloudItem interface given the path to a Point Cloud project file in
    .rcs format.

    \param strPath [in] File path.
    \param item   [out] Pointer to the object of the class implementing the
                        OdDbPointCloudExItem interface for the specified file.

    \returns
    Status of the operation as the OdResult value.

    \remarks
    Not implemented. If called, returns eNotImplemented.
  */
  virtual OdResult loadRcp(const OdString& strPath, OdDbPointCloudExItemPtr& item) = 0;
};

/** \details
  Smart pointer to objects of the OdDbPointCloudExHostPE class.
*/
typedef OdSmartPtr<OdDbPointCloudExHostPE> OdDbPointCloudExHostPEPtr;

//DOM-IGNORE-BEGIN
static const OdChar* szPointCloudHostModuleName = OD_T("PointCloudHost");
//DOM-IGNORE-END

#include "TD_PackPop.h"

#endif // __OD_DB_POINCLOUD_HOST__
