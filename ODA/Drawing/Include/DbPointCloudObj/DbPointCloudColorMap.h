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

#ifndef _OD_DB_POINTCLOUDCOLORMAP_
#define _OD_DB_POINTCLOUDCOLORMAP_

#include "DbPointCloudObjExports.h"
#include "DbObject.h"
#include "StringArray.h"

#include "TD_PackPush.h"

/** <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdPoinCloudColorSchemeCollectionReactor
{
public: 

  virtual void PoinCloudColorSchemeCollectionChanged( class OdDbPointCloudColorMap* colorMap ) = 0;

  /** \details
    Virtual destructor.
  */
  virtual ~OdPoinCloudColorSchemeCollectionReactor();
};

/** <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudColorRamp
{
  friend class OdDbPointCloudColorMap;

public:
  
  /** \details
    Default constructor.
  */
  OdDbPointCloudColorRamp();

  /** \details
    Copy constructor.

    \param source [in] OdDbPointCloudColorRamp object to be copied.
  */
  OdDbPointCloudColorRamp( const OdDbPointCloudColorRamp& source );

  /** \details
    Assignment operator. Assigns values from source to this object.

    \param source [in] OdDbPointCloudColorRamp object, values of which will be assigned to this object.
  */
  OdDbPointCloudColorRamp& operator = (const OdDbPointCloudColorRamp& source);

  /** \details
    Destructor.
  */
  ~OdDbPointCloudColorRamp();

  /** \details
    Returns the number of colors in this ramp items array.

    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  OdInt32 numColors() const;

  /** \details
    Resizes the array of ramp items.

    \param count [in] New size of the ramp item array.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  OdResult setNumColors( int count );

  /** \details
    Retrieves a color with a given index.

    \param index [in] Index for the array to pick a color.
    \returns 
      A color from an array if the index is valid, 
      or an entity color object with default properties 
      if the index is out of bounds (less than 0 and greater than size of array).
  */
  OdCmEntityColor color( int index ) const;

  /** \details
    Assigns a color to a given element in the ramp items array.

    \param index [in] Index in the array, which the color will be assigned to.
    \param color [in] Color to be set.
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  void setColor( int index, OdCmEntityColor color ) const;

  /** \details
    Checks whether this ramp item is visible or not.

    \param index [in] Index in the array.
    \returns true if the item is visible, false if the item is not visible, or if 
      index is out of bounds (less than 0 and greater than size of array).
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  bool visibility( int index ) const;

  /** \details
    Sets a visibility flag to this ramp item.

    \param index [in] Index in the array.
    \param visibility [in] New flag value to be set.
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  void setVisibility( int index, bool visibility );

  /** \details
    Copies array of ramp items and a name of a ramp from source.

    \param source [in] Pointer to an object, values of which will be assigned to this object.
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  void setFrom( const OdDbPointCloudColorRamp *source );

  /** \details
    Returns the name of this ramp.
  */
  const OdString name() const;

  /** \details
    Assigns a name to this ramp.

    \param name [in] The name to be assigned.
  */
  void setName( const OdString &name );


private:

  // File IO
  OdResult  dwgInFields( OdDbDwgFiler *pFiler );
  void  dwgOutFields( OdDbDwgFiler *pFiler ) const;
  OdResult  dxfInFields( OdDbDxfFiler *pFiler );
  void  dxfOutFields( OdDbDxfFiler *pFiler ) const;

  class OdDbPointCloudColorRampImpl *pImpl;
};

/** <group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudClassificationColorRamp
{
  friend class OdDbPointCloudColorMap;

public:

  /** \details
    Default constructor.
  */
  OdDbPointCloudClassificationColorRamp();

  /** \details
    Copy constructor.

    \param source [in] OdDbPointCloudClassificationColorRamp object to be copied.
  */
  OdDbPointCloudClassificationColorRamp( const OdDbPointCloudClassificationColorRamp& source );

  /** \details
    Assignment operator. Assigns values from source to this object.

    \param source [in] OdDbPointCloudClassificationColorRamp object, values of which will be assigned to this object.
  */
  OdDbPointCloudClassificationColorRamp& operator = (const OdDbPointCloudClassificationColorRamp& source);

  /** \details
    Destructor.
  */
  ~OdDbPointCloudClassificationColorRamp();

  /** \details
    Returns the number of colors in this ramp items array.

    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  OdInt32 numColors() const;

  /** \details
    Resizes the array of ramp items.

    \param count [in] New size of the ramp item array.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  OdResult setNumColors( int count );

  /** \details
    Retrieves a color with a given index.

    \param index [in] Index for the array to pick a color.
    \returns
      A color from an array if the index is valid,
      or an entity color object with default properties
      if the index is out of bounds (less than 0 and greater than size of array).
  */
  OdCmEntityColor color( int index ) const;

  /** \details
    Assigns a color to a given element in the ramp items array.

    \param index [in] Index in the array, which the color will be assigned to.
    \param color [in] Color to be set.
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  void setColor( int index, OdCmEntityColor color ) const;

  /** \details
    Checks whether this ramp item is visible or not.

    \param index [in] Index in the array.
    \returns true if the item is visible, false if the item is not visible, or if
      index is out of bounds (less than 0 and greater than size of array).
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  bool visibility( int index ) const;

  /** \details
    Sets a visibility flag to this ramp item.

    \param index [in] Index in the array.
    \param visibility [in] New flag value to be set.
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  void setVisibility( int index, bool visibility );

  /** \details
    Copies array of ramp items and a name of a ramp from source.

    \param source [in] Pointer to an object, values of which will be assigned to this object.
    \remarks
      Ramp item is a struct that has fields representing color and visibility.
  */
  void setFrom( const OdDbPointCloudClassificationColorRamp *source );


  /** \details
    Returns the name of this ramp.
  */
  const OdString name() const;

  /** \details
    Assigns a name to this ramp.

    \param name [in] The name to be assigned.
  */
  void setName( const OdString &name );

private:

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
  OdResult  dwgInFields( OdDbDwgFiler *pFiler );
  
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
  void  dwgOutFields( OdDbDwgFiler *pFiler ) const;
  
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
  OdResult  dxfInFields( OdDbDxfFiler *pFiler );
  
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
  void  dxfOutFields( OdDbDxfFiler *pFiler ) const;

  class OdDbPointCloudClassificationColorRampImpl *pImpl;
};

/** \details
This class represents PointCloudColorMap objects in an OdDbDatabase instance.

Corresponding C++ library: AcDbPointCloudObj

<group OdDbPointCloud_Classes>
*/
class DBPOINTCLOUDOBJ_EXPORT OdDbPointCloudColorMap : public OdDbObject
{
public:

  ODDB_DECLARE_MEMBERS( OdDbPointCloudColorMap );

  /** \details
    Default constructor.
  */
  OdDbPointCloudColorMap();

  /** \details
    Virtual destructor.
  */
  virtual ~OdDbPointCloudColorMap(){};

  /** \details
    Retrieves a color map if it exists, or creates a new one in case it doesn't.

    \param id [out] Id of a retrieved color map.
    \param pDb [in] Database, a color map is associated with.
    \param bCreateIfNone [in] Flag that states whether to create a new color map if it doesn't exist.
    \returns eOk if the operation completed successfully, or an appropriate error code otherwise.
  */
  static OdResult getColorMap( OdDbObjectId& id, OdDbDatabase *pDb, bool bCreateIfNone = true );

  /** \details
    Adds a color scheme collection reactor.

    \param pReactor [in] A color scheme collection reactor to be added.
  */
  void addColorSchemeCollectionReactor( OdPoinCloudColorSchemeCollectionReactor *pReactor );

  /** \details
    Removes a color scheme collection reactor.

    \param pReactor [in] A color scheme collection reactor to be removed.
  */
  void removeColorSchemeCollectionReactor( OdPoinCloudColorSchemeCollectionReactor *pReactor );

  void notifyColorSchemeCollectionReactor();

  /** \details
    Returns the default intensity of a color scheme.
  */
  const OdString defaultIntensityColorScheme() const;

  /** \details
    Sets the default intensity of a color scheme.
    
    \param guid [in] New default intensity.
  */
  void setDefaultIntensityColorScheme( const OdString guid );

  /** \details
    Returns the default elevation of a color scheme.
  */
  const OdString defaultElevationColorScheme() const;

  /** \details
    Sets the default elevation of a color scheme.

    \param guid [in] New default elevation.
  */
  void setDefaultElevationColorScheme( const OdString guid );

  /** \details
    Returns the default classification of a color scheme.
  */
  const OdString defaultClassificationColorScheme() const;

  /** \details
    Sets the default classification of a color scheme.

    \param guid [in] New default classification.
  */
  void setDefaultClassificationColorScheme( const OdString guid );

  /** \details
    Returns the GUIDs of color ramps.
  */
  OdStringArray colorSchemeGUIDs() const;

  /** \details
    Retrieves a color scheme of item specified by GUID.
    
    \param GUID [in] Item, which color scheme will be retrieved.
    \param target [out] Output color scheme.
    \returns 
      true if the operation completed successfully, 
      false if item doesn't exits or doesn't have a color scheme.
  */
  bool colorScheme( const OdChar *GUID, OdDbPointCloudColorRamp& target ) const;

  /** \details
    Checks if the items specified by GUID exists and has a color scheme.
    
    \param GUID [in] GUID of an item to check.
    \returns true if an item exists and has a color scheme, false - otherwise.
  */
  bool hasColorScheme( const OdChar *GUID ) const;

  /** \details
    Sets a color scheme to an item specified by GUID.

    \param GUID [in] GUID of an item, color scheme of which will be set.
    \param target [in] Color scheme to set to an item.
    \returns true if an operation completed successfully.
  */
  bool setColorScheme( const OdChar *GUID, const OdDbPointCloudColorRamp& target );

  /** \details
    Removes a color scheme from an item specified by GUID.

    \param GUID [in] GUID of an item, which color scheme will be removed.
    \returns true if the operation completed successfully.
  */
  bool deleteColorScheme( const OdChar *GUID );

  /** \details
    Collects GUIDs of classification color ramps.

    \returns Returns the collected GUIDs as an array.
  */
  OdStringArray classificationSchemeGUIDs() const;

  /** \details
    Retrieves a classification scheme of item specified by GUID.

    \param GUID [in] Item, which classification scheme will be retrieved.
    \param target [out] Output classification scheme.
    \returns
      true if the operation completed successfully,
      false if item doesn't exits or doesn't have a classification scheme.
  */
  bool classificationScheme( const OdChar *GUID, OdDbPointCloudClassificationColorRamp& target ) const;

  /** \details
    Checks if the items specified by GUID exists and has a classification scheme.

    \param GUID [in] GUID of an item to check.
    \returns true if an item exists and has a classification scheme, false - otherwise.
  */
  bool hasClassificationScheme( const OdChar *GUID ) const;

  /** \details
    Sets a classification scheme to an item specified by GUID.

    \param GUID [in] GUID of an item, classification scheme of which will be set.
    \param target [in] classification scheme to set to an item.
    \returns true if an operation completed successfully.
  */
  bool setClassificationScheme( const OdChar *GUID, const OdDbPointCloudClassificationColorRamp& target );

  /** \details
    Removes a classification scheme from an item specified by GUID.

    \param GUID [in] GUID of an item, which classification scheme will be removed.
    \returns true if the operation completed successfully.
  */
  bool deleteClassificationScheme( const OdChar *GUID );

  // DOM-IGNORE-BEGIN
  /** \details
  NOT IMPLEMENTED
  */
  void getColorSchemeInUse( OdStringArray &GUIDs );

  /** \details
  NOT IMPLEMENTED
  */
  void getClassificationColorSchemeInUse( OdStringArray &GUIDs );
  // DOM-IGNORE-END

  
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
  virtual OdResult dwgInFields(OdDbDwgFiler *pFiler);

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
  virtual void dwgOutFields(OdDbDwgFiler *pFiler) const;

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
  virtual OdResult dxfInFields(OdDbDxfFiler *pFiler);

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
  virtual void dxfOutFields(OdDbDxfFiler *pFiler) const;
};


typedef OdSmartPtr<OdDbPointCloudColorMap> OdDbPointCloudColorMapPtr;
#include "TD_PackPop.h"

#endif //_OD_DB_POINTCLOUDCOLORMAP_
