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

#ifndef _ODDBPOINTCLOUDDEFEX_INCLUDED_
#define _ODDBPOINTCLOUDDEFEX_INCLUDED_


#include "DbPointCloudObjExports.h"
#include "StringArray.h"
#include "TD_PackPush.h"

#include "DbObject.h"

/** <group OdDbPointCloud_Classes>
  \details
  This class represents an interface for a point cloud extension definition
  object. Object of this class links an .rcp or .rcs file to the drawing
  database.
  OdDbPointCloudDefEx objects are stored in the point cloud extension dictionary
  named ACAD_POINTCLOUD_EX_DICT.
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudDefEx : public OdDbObject
{
public:
//DOM-IGNORE-BEGIN
  ODDB_DECLARE_MEMBERS( OdDbPointCloudDefEx );
//DOM-IGNORE-END

  /** \details
    This enumeration defines the property state indicators.
  */
  enum PropState
  {
    kNone = -1, // No scan in the point cloud has the property.
    kSome = 0,  // Some scans in the point cloud have the property, but not all of them.
    kAll = 1    // All scans in the point cloud have the property.
  };

  /** \details
    This enumeration defines the types of properties available for a point cloud.
  */
  enum Property
  {
    kColor = 1,          // The Color property.
    kIntensity = 2,      // The Intensity property.
    kClassification = 3, // The Classification property.
    kNormal = 4,         // The Normal property.
    kSegmentation = 5,   // The Segmentation property.
    kGeoGraphic = 6      // The Geo Graphic property.
  };

  /** \details
    Default constructor. Constructs an empty OdDbPointCloudDefEx object.
  */
  OdDbPointCloudDefEx();

  /** \details
    Returns the name of the external file containing the point cloud extension
    data (.rcp or .rcs) for this PointCloudEx Definition object (as it is stored
    in a file).

    \returns
      The name of the file containing the point cloud extension data.
    \remarks
      This PointCloudDefEx object must be open for reading.
  */
  OdString sourceFileName() const;

  /** \details
    Sets the name of the external file containing the point cloud extension data
    (.rcp or .rcs) for this PointCloudEx Definition object (as it is stored in a
    file).

    \param sPathName [in] The name of the file containing the point cloud
      extension data.
    \returns
      Returns eOk if the file is accessible or eInvalidInput otherwise.
  */
  OdResult setSourceFileName(const OdString& sPathName);

  /** \details
    Returns the path name of the actual point cloud file being currently used
    for this PointCloudEx Definition object.

    \returns
      The path name of the file.
    \remarks
      This PointCloudDef object must be open for reading.
  */
  const OdChar * activeFileName() const;

  /** \details
    Sets the path name of the actual point cloud file being currently used for
    this PointCloudEx Definition object.

    \param pPathName [in] The path name of the file.
    \returns
      Returns eOk if the file is accessible or eInvalidInput otherwise.
  */
  OdResult setActiveFileName( const OdChar * pPathName);

  /** \details
    Returns the type of the source file containing the point cloud data for this
    PointCloudEx Definition object (as it is stored in file).

    \returns
      The type of the file.
    \remarks
      his PointCloudDefEx object must be open for reading.
  */
  const OdChar * fileType() const;

  // Helper functions to access Point Cloud Ex Definition dictionary

  /** \details
    Returns the PointCloudEx Dictionary ID. Creates the dictionary, if no one is
    already present in the specified OdDbDatabase instance.

    \param Db [in]  The pointer to the dwg database.
    \returns
      The PointCloudEx Dictionary ID as an OdDbObjectId object.
  */
  static OdDbObjectId createPointCloudExDictionary(OdDbDatabase *pDb);

  /** \details
    Returns the ID of the PointCloudEx Dictionary in the specified OdDbDatabase
    instance.

    \param Db [in]  The pointer to the dwg database.
    \returns
      The PointCloudEx Dictionary ID as an OdDbObjectId object.
  */
  static OdDbObjectId pointCloudExDictionary(const OdDbDatabase *pDb);

  /** \details
    Returns the pointer to the PointCloudEx Dictionary that contains the
    specified OdDbPointCloudDefEx object.

    \param pDefEx [in] the OdDbPointCloudDefEx object to get the dictionary
      which it is stored in.
    \returns
      The pointer to the PointCloudEx Dictionary object.
  */
  static OdDbDictionary* getPointCloudExDictionary( OdDbPointCloudDefEx *pDefEx );

  /** \details
    Returns true if and only if the point cloud file for this PointCloudEx
    definition object is loaded.
    \returns
      True if the point cloud file is loaded; false otherwise.
  */
  bool  isLoaded() const;

  /** \details
    Loads the point cloud file for this PointCloudEx definition object.

    \returns
      eOk if the file was loaded successfully.
  */
  OdResult load();

  /** \details
    Unloads the point cloud file for this PointCloudEx definition object.
  */
  void unload();

  /** \details
    Returns the number of OdDbPointCloudEx entities in the current drawing that
    are dependent upon this OdDbPointCloudDefEx object.

    \param pbLocked [out] When not NULL, this parameter is set to true if any
      dependent entities reside on locked layers; false otherwise.
    \returns
      A number of dependent OdDbPointCloudEx entities as an integer value.
  */
  int entityCount(bool *pbLocked) const;

  /** \details
    Returns the total number of points of this OdDbPointCloudDefEx object.

    \returns
      A number of points as an OdUInt64 value.
  */
  OdUInt64 totalPointsCount() const;

  /** \details
    Returns the default height of this OdDbPointCloudDefEx object.
    
    \returns
      The default height as a double value.
  */
  double defaultHeight() const;

  /** \details
    Returns the default length of this OdDbPointCloudDefEx object.
    
    \returns
      The default length as a double value.
  */
  double defaultLength() const;

  /** \details
    Returns the default width of this OdDbPointCloudDefEx object.
    
    \returns
      The default width as a double value.
  */
  double defaultWidth() const;

  /** \details
    Calculates the extents of this OdDbPointCloudDefEx object.

    \param exts [out] Extents of this OdDbPointCloudDefEx object.
    \returns
      eOk if the extents are valid or eInvalidExtents otherwise.
  */
  OdResult extents( OdGeExtents3d &exts ) const;

  /** \details
    Returns the total number of regions for this OdDbPointCloudDefEx object.

    \returns
      A number of regions as an OdUInt32 value.
  */
  OdUInt32 totalRegionsCount() const;

  /** \details
    Returns the total number of scans for this OdDbPointCloudDefEx object.

    \returns
      A number of scans as an OdUInt32 value.
  */
  OdUInt32 totalScansCount() const;

  /** \details
    Gets the the coordinate system name of the point cloud.
    
    \returns
      The name of the coordinate system.
  */
  OdString coordinateSystemName() const;

  /** \details
    Gets the path of a specified .rcs file.
    
    \param guid [in] The GUID of the .rcs file.
    \returns
      The path of the .rcs file.
  */
  OdString getRcsFilePath( const OdString &guid) const;

  /** \details
    Gets the list of paths for all .rcs files.
    
    \param list [in/out] An array of strings to be filled with paths to the .rcs
      files.
    \returns
      The array of strings containing paths to the .rcs files.
  */
  void getAllRcsFilePaths( OdStringArray &list ) const;

  /** \details
    Checks whether a specified property is present in scans of a point cloud.
    The result will show whether this property is present in all scans, some of
    the scans, or none of the scans.

    \param prop [in] Property to be checked.
    \returns
      The state of the property.
  */
  PropState hasProperty( Property prop ) const;

  /** \details
    Creates a thumbnail of the point cloud.
    NOT INPLEMENTED.
  */
  OdResult createThumbnailBitmap( BITMAPINFO*& pBmpInfo, int width, int height ) const;

  // File IO

  /** \details
    This method is intended to read object specific data from a .dwg file.
    The method is not intended to be called directly, it is called by dwgIn().
    
    \param pFiler [in] Filer object which data is read from.
    \returns
      eOk if data was read successfully.
  */
  virtual OdResult  dwgInFields( OdDbDwgFiler* pFiler );

  /** \details
    This method is intended to write object specific data to a .dwg file.
    The method is not intended to be called directly, it is called by dwgOut().

    \param pFiler [in] Filer object which data is written to.
  */
  virtual void  dwgOutFields( OdDbDwgFiler* pFiler ) const;

  /** \details
    This method is intended to read object specific data from a .dxf file.
    The method is not intended to be called directly, it is called by dxfIn().

    \param pFiler [in] Filer object which data is read from.
    \returns
      eOk if data was read successfully.
  */
  virtual OdResult  dxfInFields( OdDbDxfFiler* pFiler );

  /** \details
    This method is intended to write object specific data to a .dxf file.
    The method is not intended to be called directly, it is called by dxfOut().

    \param pFiler [in] Filer object which data is written to.
  */
  virtual void  dxfOutFields( OdDbDxfFiler* pFiler ) const;

  /** \details
    Returns the current class version.

    \returns
    Class version as an integer value.
  */
  static int classVersion();

};

/** \details
  Smart pointer to objects of the OdDbPointCloudDefEx class.
*/
typedef OdSmartPtr<OdDbPointCloudDefEx> OdDbPointCloudDefExPtr;

/** <group OdDbPointCloud_Classes>
  \details
  This class implements point cloud extension definition reactor objects in an
  OdDbDatabase instance.
  
  \remarks
  Point cloud extension definition reactor (OdDbPointCloudDefReactorEx) objects
  are used to notify point cloud extension (OdDbPointCloudEx) objects of changes
  to their associated point cloud extension definition (OdDbPointCloudDefEx)
  objects. Modifications of point cloud definition objects redraw their
  dependent cloud point entities.
*/
class  DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudDefReactorEx : public OdDbObject
{
public:
//DOM-IGNORE-BEGIN
  ODDB_DECLARE_MEMBERS( OdDbPointCloudDefReactorEx );
//DOM-IGNORE-END

  /** \details
    Default constructor. Constructs an empty OdDbPointCloudDefReactorEx object.
  */
  OdDbPointCloudDefReactorEx( );

  /** \details
    Returns the current class version.

    \returns
      Class version as an integer value.
  */
  static int classVersion();

  // File IO

  /** \details
    This method is intended to read object specific data from a .dwg file.
    The method is not intended to be called directly, it is called by dwgIn().
    
    \param pFiler [in] Filer object which data is read from.
    \returns
      eOk if data was read successfully.
  */
  virtual OdResult  dwgInFields( OdDbDwgFiler* pFiler);

  /** \details
    This method is intended to write object specific data to a .dwg file.
    The method is not intended to be called directly, it is called by dwgOut().

    \param pFiler [in] Filer object which data is written to.
  */
  virtual void  dwgOutFields( OdDbDwgFiler* pFiler) const;

  /** \details
    This method is intended to read object specific data from a .dxf file.
    The method is not intended to be called directly, it is called by dxfIn().

    \param pFiler [in] Filer object which data is read from.
    \returns
      eOk if data was read successfully.
  */
  virtual OdResult  dxfInFields( OdDbDxfFiler* pFiler);

  /** \details
    This method is intended to write object specific data to a .dxf file.
    The method is not intended to be called directly, it is called by dxfOut().

    \param pFiler [in] Filer object which data is written to.
  */
  virtual void  dxfOutFields( OdDbDxfFiler* pFiler) const;

  // Notifications

  /** \details
    Notification function called whenever an object has been erased or unerased.

    \remarks
      Not implemented.
  */
  virtual void erased( OdDbObject const *, bool);   // not implemented

  /** \details
    Notification function called whenever an object was opened OdDb::kForWrite,
    a function has been called that could modify the content of this object,
    and this object is now being closed.

    \param pObj [in] Pointer to the object that is being closed after being
      modified.
    \remarks
      The default implementation of this function does nothing. This function can
      be overridden in custom classes.
  */
  virtual void modified( const OdDbObject* pObj);

  /** \details
    Returns notification status for OdDbPointCloudDefEx object events.

    \returns
      True if notifications are enabled; false otherwise.
  */
  static bool isEnabled();

  /** \details
    Controls notifications of OdDbPointCloudDefEx object events.

    \param bEnable [in] True to enable notifications; false to disable notifications.
  */
  static void setEnable( bool bEnable);

private:
//DOM-IGNORE-BEGIN
  static bool m_bEnabled;
//DOM-IGNORE-END
};

/** \details
  Smart pointer to objects of the OdDbPointCloudDefReactorEx class.
*/
typedef OdSmartPtr<OdDbPointCloudDefReactorEx> OdDbPointCloudDefReactorExPtr;

#include "TD_PackPop.h"

#endif //_ODDBPOINTCLOUDDEFEX_INCLUDED_
