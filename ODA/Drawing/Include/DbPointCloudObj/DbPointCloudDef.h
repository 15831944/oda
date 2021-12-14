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

#ifndef _ODDBPOINTCLOUDDEF_INCLUDED_
#define _ODDBPOINTCLOUDDEF_INCLUDED_ /* { Secret} */

#include "DbPointCloudObjExports.h"
#include "TD_PackPush.h"

#include "DbObject.h"

/** \details
    This class defines Point Cloud Definition objects in an OdDbDatabase instance.

    \sa
    AcDbPointCloudObj

    \remarks
    Point Cloud Definitions (OdDbPointCloudDef objects) work with Point Cloud (OdDbPointCloud) entities
    in much the same way that Raster Image Definitions (OdDbRasterImageDef objects) work with Image References
    (OdDbRasterImage entities).

    <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudDef : public OdDbObject
{
public:
  //DOM-IGNORE-BEGIN
  ODDB_DECLARE_MEMBERS(OdDbPointCloudDef);
  //DOM-IGNORE-END

  /** \details
    Default constructor. Constructs an empty OdDbPointCloudDef object.
  */
  OdDbPointCloudDef();

  // Properties

  /** \details
    Returns the name of the external file containing the point cloud data (.pcg or .isd) for this PointCloud Definition object (as it is stored in file).
    
    \remarks
    This PointCloudDef object must be open for reading.
  */
  OdString sourceFileName() const;

  /** \details
    Sets the name of the external file containing the point cloud data (.pcg or .isd) for this PointCloud Definition object (as it is stored in file).
    
    \returns
    Returns eOk if the file is accessible or eInvalidInput otherwise.
  */
  OdResult setSourceFileName(const OdString&);

  /** \details
    Returns the type of the source file containing the point cloud data for this PointCloud Definition object (as it is stored in file).

    \remarks
    This PointCloudDef object must be open for reading.
  */
  OdString  fileType(void) const;


  /** \details
    Returns the path name of the actual point cloud file being currently used for this PointCloud Definition object.

    \remarks
    This PointCloudDef object must be open for reading.
  */
  OdString activeFileName() const;


  /** \details
    Sets the path name of the actual point cloud file being currently used for this PointCloud Definition object.

    \param name [in] New active file name to be set.
    \returns
      Returns eOk if the file is accessible or eInvalidInput otherwise.
  */
  OdResult setActiveFileName(const OdString& name);

  
  /** \details
    Returns true if and only if the point cloud file for this PointCloud definition object is loaded.
  */
  bool isLoaded() const;


  /** \details
    Currently not implemented.
  */
  OdResult load(bool bModifyDatabase);

  /** \details
    Currently not implemented.
  */
  void unload(bool bModifyDatabase);

  /** \details
    Returns the number of OdDbPointCloud entities in the current drawing that are dependent upon this object.
    
    \param pbLocked [in]  If it's non-zero, then this function will set the true value at that pointer if any dependent entities reside on locked layers, or set false otherwise.
  */
  int entityCount(bool *pbLocked) const;

  /** \details
    Returns the total number of points of this OdDbPointCloudDef object.
  */
  OdUInt64 totalPointsCount() const;

  /** \details
    Returns the default height of this OdDbPointCloudDef object.
  */
  double defaultHeight() const;

  /** \details
    Returns the default length of this OdDbPointCloudDef object.
  */
  double defaultLength() const;

  /** \details
    Returns the default width of this OdDbPointCloudDef object.
  */
  double defaultWidth() const;


  /** \details
    Calculates the extents of this OdDbPointCloudDef object.

    \param exts [in]  Extents to be set.
    \returns
      Returns eOk if the extents are valid or eInvalidExtents otherwise.
  */
  OdResult  getNativeCloudExtent(OdGeExtents3d &exts) const;

  /** \details
    Sets the extents for this OdDbPointCloudDef object.

    \param exts [in]  Extents to be set.
  */
  void setExtents(const OdGeExtents3d &exts);

  // File IO

  /** \details
    Reads the .dwg file data of this object.

    \param pFiler [in]  Filer object from which data are read.
    \returns
      Returns the filer status.
      This function is called by dwgIn() to allow the object to read its data.
      When overriding this function:
        1)  Call assertWriteEnabled().
        2)  Call the parent class's dwgInFields(pFiler).
        3)  If it returns eOK, continue; otherwise return whatever the parent's dwgInFields(pFiler) returned.
        4)  Call the OdDbDwgFiler(pFiler) methods to read each of the object's data items in the order they were written.
        5)  Return pFiler->filerStatus().
  */
  virtual OdResult  dwgInFields(OdDbDwgFiler *pFiler);

  /** \details
    Writes the .dwg file data of this object.

    \param pFiler [in]  Pointer to the filer to which data are written.
    \remarks
      This function is called by dwgIn() to allow the object to write its data.
      When overriding this function:
        1)  Call assertReadEnabled().
        2)  Call the parent class's dwgOutFields(pFiler).
        3)  Call the OdDbDwgFiler(pFiler) methods to write each of the object's data items in the order they were written.
  */
  virtual void  dwgOutFields(OdDbDwgFiler *pFiler) const;

  /** \details
    Reads the DXF data of this object.

    \param pFiler [in]  Pointer to the filer from which data are read.
    \returns
      Returns the filer status.
      This function is called by dxfIn() to allow the object to read its data.
      When overriding this function:
        1)  Call assertWriteEnabled().
        2)  Call the parent class's dwgInFields(pFiler).
        3)  If it returns eOK, continue; otherwise return whatever the parent's dxfInFields(pFiler) returned.
        4)  Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the order they were written.
        5)  Return pFiler->filerStatus().
  */
  virtual OdResult  dxfInFields(OdDbDxfFiler *pFiler);

  /** \details
    Writes the DXF data of this object.

    \param pFiler [in]  Pointer to the filer to which data are to be written.
    \remarks
      This function is called by dxfOut() to allow the object to write its data.
      When overriding this function:
        1)  Call assertReadEnabled().
        2)  Call the parent class's dxfOutFields(pFiler).
        4)  Use pFiler to call the OdDbDxfFiler methods to write each of the object's data items in the order they were written.
  */
  virtual void  dxfOutFields(OdDbDxfFiler *pFiler) const;

  static int classVersion();

  // Helper functions to access Point Cloud Definition dictionary

  /** \details
    Returns Pointcloud Dictionary ID. Creates the dictionary, if one is not already present, in the specified OdDbDatabase instance.

    \param Db [in]  The database.
  */
  static OdDbObjectId createPointCloudDictionary(OdDbDatabase& Db);

  /** \details
    Returns the Object ID of the point cloud dictionary in the specified OdDbDatabase instance.

    \param Db [in]  The database.  
  */
  static OdDbObjectId pointCloudDictionary(const OdDbDatabase& Db);

  //DOM-IGNORE-BEGIN
#if 0
  OdResult  createThumbnailBitmap(struct tagBITMAPINFO * &,int,int) const;
  bool  getPointCloudFileVersion(int &,int &) const;
  bool  hasProperty(const OdString&,bool &) const;
  bool  saveToVersion(const OdString&,int);
#endif
  //DOM-IGNORE-END

  // OdDbObject overridden methods

  /** \details
    Called as the first operation as this object is being erased or unerased.

    \param erasing [in]  A copy of the erasing argument passed to erase().
    \remarks
      This function is notified just before the current object is to be erased, giving this function
      the ability to cancel the erase.
    \returns
      Returns eOk if and only if erase() is to continue.
      When overriding this function:
        1)  If the OdDbObject's state is incorrect, return
          something other than eOk.
        2)  If the parent class's subErase() returns anything
          other than eOk, immediately return it.
        3)  If other actions are required before erase, do them.
        4)  Return eOk.
      If you must make changes to this object's state, either make them after
      step 2, or roll them back if step 2 returns other than eOk.
      The default implementation of this function does nothing but return eOk.  This function can be
      overridden in custom classes.
  */
  virtual OdResult subErase(bool erasing) ODRX_OVERRIDE;

  /** \details
    Called as the first operation as this object is being closed, for
    database -resident objects only.

    \remarks
      This function is notified just before the current open operation is to be closed, giving this function
      the ability to perform necessary operatons.
      When overriding this function:
        1)  If the OdDbObject's state is incorrect, throw exception.
        2)  Call parent class's subClose().
        3)  If other actions are required before close, do them.
      The default implementation of this function does nothing.
      This function can be overridden in custom classes.
  */
  virtual void subClose() ODRX_OVERRIDE;

  /** \details
    Called as the first operation of the handOverTo function.

    \param pNewObject [in]  Pointer to the object with which to replace this object in the database.
    \remarks
      This function allows custom classes to populate the new object.
    \remarks
      Overriding this function in a child class allows a child instance to be notified each time an
      object is handed over.
      This function is notified just before an object is to be handed over; giving this function
      the ability to cancel the handover.
      When overriding this function:
        1)  If the OdDbObject's state is incorrect, throw exception.
        2)  Call parent class's subHandover().
        3)  If other actions are required before handover, do them.
      The default implementation of this function does nothing.  This function can be
      overridden in custom classes.
  */
  virtual void subHandOverTo(OdDbObject* pNewObject) ODRX_OVERRIDE;

  virtual OdDbObjectPtr subWblockClone(OdDbIdMapping& idMap, OdDbObject* owner, bool bPrimary) const ODRX_OVERRIDE;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbPointCloudDef object pointers.
*/
typedef OdSmartPtr<OdDbPointCloudDef> OdDbPointCloudDefPtr;


/** \details
  This class represents the point cloud definition reactor.
 
  Corresponding C++ library: AcDbPointCloudObj

  <group OdDbPointCloud_Classes>
*/
class  DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudDefReactor : public OdDbObject
{
public:
  //DOM-IGNORE-BEGIN
  ODDB_DECLARE_MEMBERS(OdDbPointCloudDefReactor);
  //DOM-IGNORE-END

  /** \details
    Default constructor. Creates an empty OdDbPointCloudDefRefactor object.
  */
  OdDbPointCloudDefReactor();

  static int classVersion();

  // File IO

  /** \details
    Reads the .dwg file data of this object.

    \param pFiler [in]  Filer object from which data are read.
    \returns
      Returns the filer status.
      This function is called by dwgIn() to allow the object to read its data.
      When overriding this function:
        1)  Call assertWriteEnabled().
        2)  Call the parent class's dwgInFields(pFiler).
        3)  If it returns eOK, continue; otherwise return whatever the parent's dwgInFields(pFiler) returned.
        4)  Call the OdDbDwgFiler(pFiler) methods to read each of the object's data items in the order they were written.
        5)  Return pFiler->filerStatus().
  */
  virtual OdResult dwgInFields(OdDbDwgFiler* pFiler);

  /** \details
    Writes the .dwg file data of this object.

    \param pFiler [in]  Pointer to the filer to which data are written.
    \remarks
      This function is called by dwgIn() to allow the object to write its data.
      When overriding this function:
        1)  Call assertReadEnabled().
        2)  Call the parent class's dwgOutFields(pFiler).
        3)  Call the OdDbDwgFiler(pFiler) methods to write each of the object's data items in the order they were written.
  */
  virtual void dwgOutFields(OdDbDwgFiler* pFiler) const;

  /** \details
    Reads the DXF data of this object.

    \param pFiler [in]  Pointer to the filer from which data are read.
    \returns
      Returns the filer status.
      This function is called by dxfIn() to allow the object to read its data.
      When overriding this function:
        1)  Call assertWriteEnabled().
        2)  Call the parent class's dwgInFields(pFiler).
        3)  If it returns eOK, continue; otherwise return whatever the parent's dxfInFields(pFiler) returned.
        4)  Call the OdDbDxfFiler(pFiler) methods to read each of the object's data items in the order they were written.
        5)  Return pFiler->filerStatus().
  */
  virtual OdResult  dxfInFields(OdDbDxfFiler* pFiler);

  /** \details
    Writes the DXF data of this object.

    \param pFiler [in]  Pointer to the filer to which data are to be written.
    \remarks
      This function is called by dxfOut() to allow the object to write its data.
      When overriding this function:
        1)  Call assertReadEnabled().
        2)  Call the parent class's dxfOutFields(pFiler).
        4)  Use pFiler to call the OdDbDxfFiler methods to write each of the object's data items in the order they were written.
  */
  virtual void  dxfOutFields(OdDbDxfFiler* pFiler) const;

  // Notifications
  //DOM-IGNORE-BEGIN
  virtual void erased(OdDbObject const * , bool) ;   // not implemented
  //DOM-IGNORE-END

  /** \details
    This method is called whenever a reactor is modified.

    \param obj [in] Pointer to a reactor as OdDbObject.
  */
  virtual void modified(OdDbObject const *obj) ;

  /** \details
    Checks if this element is enabled.

    \returns true if this reactor is enabled.
  */
  static bool isEnabled();

  /** \details
    Enables or disables this reactor.

    \param val [in] Flag whether to enable or disable this element.
  */
  static void setEnable(bool val);
private:
  static bool m_bEnabled;
};
typedef OdSmartPtr<OdDbPointCloudDefReactor> OdDbPointCloudDefReactorPtr;


#include "TD_PackPop.h"

#endif
