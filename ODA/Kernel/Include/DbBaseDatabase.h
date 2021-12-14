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

#ifndef _ODDB_BASEDATABASE_INCLUDED_
#define _ODDB_BASEDATABASE_INCLUDED_

#include "RxObject.h"
#include "DbRootExport.h"
#include "OdFont.h"
#include "GiDefaultContext.h"
#include "Gs/Gs.h"
#include "Gi/GiLinetype.h"
#include "Ge/GeCurve2d.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeBoundBlock3d.h"
#include "OdTimeStamp.h" // dgn dwf export
#include "OdUnitsFormatter.h"
#include "StringArray.h"
#include "DbHandle.h"

class OdDbBaseHostAppServices;
class OdGiAnnoScaleSet;
class OdGiMaterialTraits;

typedef OdRxObject OdDbBaseDatabase;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBaseDatabase object pointers.
*/
typedef OdRxObjectPtr OdDbBaseDatabasePtr;

/** \details
    This class is the base Protocol Extension class for database classes.

    \sa
    TD_DbRoot
    <group OdDbRoot_Classes> 
*/
class DBROOT_EXPORT OdDbBaseDatabasePE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseDatabasePE);
  //DOM-IGNORE-END
  
  /** \details 
    Determines extents parameter values.
  */
  enum ExtentsFlags
  {
    kZeroFlags       = 0, // No parameter flags are used.
    kExactExtents    = 1, // Zoom to extents exact to device/paper size without any margins if this flag is set.
    kUseViewExtents  = 2, // Use view extents instead of plot extents (default) for calculation of drawing extents if this flag is set.
    kUseGivenExtents = 4, // Zoom to given extents
    kUseGivenView    = 8 // Use view from GsDevice to calculate extents
  };

  /** \details
      Returns the OdDbBaseHostAppServices object associated with this database object.
  */
  virtual OdDbBaseHostAppServices* appServices(OdDbBaseDatabase* pDb) const = 0;

  /** \details
      Returns the name of the file associated with this database object.
  */
  virtual OdString getFilename(OdDbBaseDatabase* pDb) const = 0;

  /** \details
    Starts a database transaction.
    
    \param pDb [in] A pointer to a database where the transaction should be started.
  */
  virtual void startTransaction(OdDbBaseDatabase* pDb) = 0;
  
  /** \details
    Aborts (rolls back) the current database transaction.
    
    \param pDb [in] A pointer to a database where the transaction should be aborted.
  */
  virtual void abortTransaction(OdDbBaseDatabase* pDb) = 0;

  /** \details
      Starts undo recording of this database object.
  */
  virtual OdResult startUndoRecord(OdDbBaseDatabase* pDb) = 0;

  /** \details
    Evaluates fields in database.
    \param pDb [in]  Pointer to the database for which fields are to be evaluated
    \param nContext [in]  Input context in which the field is evaluated, which can be a predefined *OdDbField::EvalContext* enum flag or a user-defined context flag; this context flag is passed to the evaluator
  */
  virtual OdResult evaluateFields(OdDbBaseDatabase* pDb, int nContext) = 0;

  /** \details
  Performs an undo operation on this database object.

  \remarks
  All operations performed since the last call to startUndoRecording will be undone.
  At least 1 undo step is always supported for internal needs.
  */
  virtual OdResult undo(OdDbBaseDatabase* pDb) = 0;

  /** \details
    Creates a Gi context object for a specified database.
    
    \param pDb [in] A pointer to a database to create a context object for.
  */
  virtual OdGiDefaultContextPtr createGiContext(OdDbBaseDatabase* pDb) const = 0;

  /** \details 
    Retrieves information about viewports for a specified database and layout. 
    
    \params pDb       [in] A pointer to a database.
    \params layoutId  [in] A layout identifier.
    \params layoutId  [out] An array of viewport names. Filled and returned by the method.
    \params layoutId  [out] An array of viewport points. Filled and returned by the method.
  */
  virtual void putNamedViewInfo(OdDbBaseDatabase* pDb, OdDbStub* layoutId, OdStringArray& names, OdGePoint3dArray& points) = 0;
  
  /** \details
  Creates a TextIterator object for the specified data.

  \param textString [in]  Text string to be parsed.
  \param length     [in]  Length of string in bytes.
  \param raw        [in]  If and only if true, character sequences in the form of %%c will be treated as raw text.
  \param codePageId [in]  Object ID of the code page for the text.
  \param pTextStyle [in]  Pointer to the TextStyle object associated with the specified text.

  \returns
  Returns a SmartPointer to the new OdDbTextIterator object.
  */
  virtual OdBaseTextIteratorPtr createTextIterator(OdDbBaseDatabase* db, const OdChar* textString, int length, bool raw, const OdGiTextStyle* pTextStyle) const = 0;
  
  /** \details 
    Sets up Gs views for a specified device, according to the active database layout data.
    
    \param pDevice  [in] A pointer to the device.
    \param pGiCtx   [in] A pointer to the Gi context object with active layout data. 
    
    \returns A smart pointer to an OdGsDevice object with views set up. 
  */
  virtual OdGsDevicePtr setupActiveLayoutViews(OdGsDevice* pDevice, OdGiDefaultContext* pGiCtx) = 0;

  /** \details 
    Sets up Gs views for a specified device according to the specified database layout data.
    
    \param pDevice  [in] A pointer to a device.
    \param pGiCtx   [in] A pointer to a Gi context object with active layout data. 
    \param pGiCtx   [in] A pointer to a database layout identifier. 
    
    \returns A smart pointer to an OdGsDevice object with view set up. 
  */
  virtual OdGsDevicePtr setupLayoutView(OdGsDevice* pDevice, OdGiDefaultContext* pGiCtx, OdDbStub* layoutId) = 0;
  
  /** \details 
    Sets up a palette associated with a specified device layout. 
    
    \param device     [in] A pointer to a device. 
    \param giContext  [in] A pointer to a Gi context.
    \param pGiCtx     [in] A pointer to a database layout identifier. 
    \param palBg      [in] A background color of the palette.
    
    \remarks 
    To overwrite the default DB-palette color, use a non-zero value for the alpha component of RGBA color.
  */
  virtual void setupPalette(OdGsDevice* device, OdGiDefaultContext* giContext, OdDbStub* layoutId = 0, ODCOLORREF palBg = ODRGBA(0,0,0,0)) = 0;

  /** \details 
    Gets the next view associated with the active layout. 
  
    \param pGiCtx [in] A pointer to a default Gi context object.
    \remarks 
    This functionality currently is not implemented.  
  */
  virtual OdDbStub* getNextViewForActiveLayout(OdGiDefaultContext* pGiCtx, OdDbStub* /*objectId*/) = 0;
  
  /** \details 
    Sets current layout plot settings (such as paper size, rotation, scale, etc.) to a specified Gs device. 
    
    \param clipBox      [out] A visible rectangle to be returned.
    \param pDevice      [in]  A pointer to a device to apply plot settings to.
    \param db           [in]  A pointer to a drawing database.
    \param extentsFlags [in]  Extents flags.
    \param dpi          [in]  Resolution value.
  */  
  virtual void applyLayoutSettings(OdGsDCRect& clipBox, OdGsDevice* pDevice, OdDbBaseDatabase* db, OdUInt32 extentsFlags = kZeroFlags, OdUInt32 dpi = 72) = 0;

  /** \details
    Makes the specified Gs device render the current layout zoomed to extents.
    \param outputRect   [in]     A rectangle, which determines the printable area in device units.
    \param pDevice      [in]     A pointer to a device to render the current layout with.
    \param db           [in]     A pointer to a drawing database.
    \param plotExtents  [in/out] Eye coordinate system plotting extents of the device viewport.
    \param extentsFlags [in]     Extents flags.
    \param objectId     [in]     Object identifier.
    
    \remarks 
    If no viewport is found, the method throws an exception. 
    plotExtents will be used to zoomExtents if kUseGivenExtents flag is set, otherwise they will be calculated
  */
  virtual void zoomToExtents(const OdGsDCRect& outputRect, OdGsDevice* pDevice, OdDbBaseDatabase* db, OdGeBoundBlock3d& plotExtents,
                             OdUInt32 extentsFlags = kZeroFlags, OdDbStub* objectId = NULL) = 0;
  
  /** \details 
    Loads plot styles of the active layout to a specified Gi context object. 
    
    \param pDwgContext [in] A pointer to a Gi context object. Plot styles will be loaded into it. 
    \param db          [in] A pointer to a drawing database. 
  */
  virtual void loadPlotstyleTableForActiveLayout(OdGiDefaultContext* pDwgContext, OdDbBaseDatabase* db) = 0;

  /** \details 
    Returns a smart pointer to the iterator of the specified database's layers.
    
    \param db [in] A pointer to a drawing database, from which the method retrieves information about layers. 
  */
  virtual OdRxIteratorPtr layers(OdDbBaseDatabase* db) const = 0;
  
  /** \details 
    Returns a smart pointer to the iterator of the specified database's visual styles.
    
    \param db [in] A pointer to a drawing database, from which the method retrieves information about visual styles. 
  */  
  virtual OdRxIteratorPtr visualStyles(OdDbBaseDatabase* db) const = 0;

  /** \details
    Returns an object identifier of a visual style with a specified name from a specified database.
    \param db   [in] A pointer to a database, from which the visual style data will be retrieved.
    \param name [in] A visual style name.
    
    \returns Returns the visual style object identifier if it was found or OdDbObjectId::kNull value in the other case.
  */
  virtual OdDbStub* getVisualStyleId(OdDbBaseDatabase* db, const OdString& name) const = 0;

  /** \details 
    Returns the current layout for the specified database.
    \param pDb [in] A pointer to a database.
    
    \remarks 
    In this case the term "layout" means an abstraction that corresponds to a "sheet" in a sheet set manager, "page" in a .pdf or .dwf file, and "layout"+"block table record" in a .dwg file.
  */
  inline OdRxObjectPtr currentLayout(OdDbBaseDatabase* pDb)
  {
    return openObject(currentLayoutId(pDb));
  }

  /** \details 
    Sets the current layout for the specified database.
    
    \param db   [in] A pointer to a database.
    \param name [in] A name of a layout to be set as current.
  */
  virtual void setCurrentLayout(OdDbBaseDatabase* db, const OdString& name) = 0;

  /** \details 
    Returns layouts for the specified database.
    
    \param db   [in] A pointer to a database.
    
    \returns Returns an iterator for accessing the layout objects dictionary.
  */
  virtual OdRxIteratorPtr layouts(OdDbBaseDatabase* db) const = 0;

  /** \details 
    Returns a layout with a specified name from the specified database.
    \param pDb    [in] A pointer to a database.
    \param name   [in] A name of a layout to be retrieved.
    \remarks 
    If layout with the specified name is not found, NULL pointer is returned.
  */
  virtual OdRxObjectPtr getLayout(OdDbBaseDatabase* pDb, const OdString& name)
  {
    return openObject(findLayoutNamed(pDb, name));
  }

  /** \details 
    Returns a layout related with a specified viewport from the specified database.
    \param db           [in] A pointer to a database.
    \param pViewportId  [in] A viewport identifier.
    \remarks 
    If a layout with the specified viewport is not found, a NULL pointer is returned.
  */
  inline OdRxObjectPtr findLayoutByViewport(OdDbBaseDatabase* db, OdDbStub* pViewportId)
  {
    return openObject(findLayoutIdByViewport(db, pViewportId));
  }

  /** \details 
    Returns a pointer to an identifier of a layout related to a specified viewport from the specified database.
    \param db           [in] A pointer to a database.
    \param pViewportId  [in] A viewport identifier.
    \remarks 
    If a layout related to the specified viewport is not found, a NULL pointer is returned.
  */
  virtual OdDbStub* findLayoutIdByViewport(OdDbBaseDatabase* db, OdDbStub* pViewportId) = 0;

  /** \details 
    Returns a pointer to an identifier of a layout with a specified name from the specified database.
    \param db   [in] A pointer to a database.
    \param name [in] A name of a layout.
    \remarks 
    If a layout with the specified name is not found, a NULL pointer is returned.
  */
  virtual OdDbStub* findLayoutNamed(OdDbBaseDatabase* db, const OdString& name) = 0; //DOM-IGNORE-BEGIN// dgn dwf export//DOM-IGNORE-END

  /** \details 
    Returns a layout that is related to model space of the specified database.
    \param db   [in] A pointer to a database.
  */
  inline OdRxObjectPtr getFirstLayout(OdDbBaseDatabase* db)
  {
    return openObject(getFirstLayoutId(db));
  }
  
  /** \details 
    Returns an identifier of a layout related to model space of the specified database.
    \param db   [in] A pointer to a database.
  */
  virtual OdDbStub* getFirstLayoutId(OdDbBaseDatabase* db) = 0;   //DOM-IGNORE-BEGIN// dgn dwf export//DOM-IGNORE-END

  /** \details 
    Returns the specified database's creation date and time.
    \param db   [in] A pointer to a database.
  */
  virtual OdTimeStamp getCreationTime(OdDbBaseDatabase* db) = 0;  //DOM-IGNORE-BEGIN// dgn dwf export//DOM-IGNORE-END
  
  /** \details 
    Returns the specified database's last update date and time.
    \param db   [in] A pointer to a database.
  */
  virtual OdTimeStamp getUpdateTime(OdDbBaseDatabase* db) = 0;    //DOM-IGNORE-BEGIN// dgn dwf export//DOM-IGNORE-END
  
  /** \details 
    Returns the specified database's fingerprint GUID. 
    The fingerprint GUID uniquely identifies the database. It is created when the database is created.
    \param db   [in] A pointer to a database.
  */
  virtual OdString getFingerPrintGuid(OdDbBaseDatabase* db) = 0;  //DOM-IGNORE-BEGIN// dgn dwf export//DOM-IGNORE-END
  
  /** \details 
    Returns the specified database's version GUID. 
    The version GUID uniquely identifies the database version. It is updated each time database changes are saved.
    \param db   [in] A pointer to a database.
  */
  virtual OdString getVersionGuid(OdDbBaseDatabase* db) = 0;      //DOM-IGNORE-BEGIN// dgn dwf export//DOM-IGNORE-END

  /** \details 
    Returns the drawing units that are used for automatic scaling of blocks, xrefs and images that are inserted into or attached to the specified database. 
    \param db   [in] A pointer to a database.
    \remarks 
    See OdDb::UnitsValue enumeration for additional information about units.
  */  
  virtual int getUnits(OdDbBaseDatabase* db) = 0; //DOM-IGNORE-BEGIN// dgn dwf export//DOM-IGNORE-END
  
  /** \details 
    Returns the current units formatter for the specified database. 
    \param db   [in] A pointer to a database.
  */
  virtual OdUnitsFormatter* baseFormatter(OdDbBaseDatabase* db) = 0; //DOM-IGNORE-BEGIN// dgn dwg common formatter base class//DOM-IGNORE-END
  
  /** \details 
    Returns the measurement system that is used for linetypes and hatches for the specified database. 
    \param db   [in] A pointer to a database.
    \remarks 
    See OdDb::MeasurementValue enumeration for additional information about valid measurement values. 
  */
  virtual int getMeasurement(OdDbBaseDatabase* db) = 0;

  /** \details 
    Searches for a linetype object by its identifier in the specified database. 
    \param db       [in]  A pointer to a database.
    \param pLTypeId [in]  A linetype identifier to be searched for.
    \param LType    [out] A reference to an OdGiLinetype object to store found linetype information.

    \returns Returns true if the linetype object was found with the specified identifier, otherwise returns false. 
  */
  virtual bool getLineTypeById( OdDbBaseDatabase* db, OdDbStub* pLTypeId, OdGiLinetype& LType ) = 0;

  /** \details 
    Searches for a text style object by its identifier in the specified database. 
    \param db       [in]  A pointer to a database.
    \param pLTypeId [in]  A text style identifier to be searched for.
    \param LType    [out] A reference to an OdGiTextStyle object to store found text style information.

    \returns Returns true if the text style object was found with the specified identifier, otherwise returns false. 
  */
  virtual bool getTextStyleById( OdDbBaseDatabase* db, OdDbStub* idStyle, OdGiTextStyle& shapeInfo ) = 0;   

  /** \details 
    Returns an object's identifier.
    \param obj [in]  A pointer to an object in a database.
  */
  virtual OdDbStub* getId(const OdRxObject* obj) const = 0;

  /** \details 
    Returns an identifier of a specified database object using its 64-bit handle.
    \param db       [in]  A pointer to a database.
    \param handle [in]  A database object handle.
  */
  virtual OdDbStub* getObject(OdDbBaseDatabase* db, OdUInt64 handle) = 0;
  
  /** \details 
    Returns a handle of an object using its identifier.
    \param id [in]  A database object identifier.
  */
  virtual OdDbHandle getHandle(OdDbStub* id);
  
  /** \details 
    Returns a pointer to a database that a specified object belongs to.
    \param id [in]  A database object identifier.
  */
  virtual OdDbBaseDatabase* getDatabase(OdDbStub *id);
  
  /** \details 
    Returns an identifier of an owner object for a specified database object.
    \param id [in]  A database object identifier.
  */
  virtual OdDbStub* getOwner(OdDbStub *id);

  /** \details 
    Returns the code page identifier for a specified database.
    \param db [in]  A pointer to a database.
  */
  virtual OdCodePageId getCodePage(OdDbBaseDatabase* db) = 0;

  /** \details 
    Returns the identifier of the model space block for a specified database.
    \param db [in]  A pointer to a database.
  */
  virtual OdDbStub* getModelBlockId(OdDbBaseDatabase* pDb) = 0;
  
  /** \details 
    Returns the identifier of the paper space block for a specified database.
    \param db [in]  A pointer to a database.
  */
  virtual OdDbStub* getPaperBlockId(OdDbBaseDatabase* pDb) = 0;
  
  /** \details 
    Returns the identifier of the current layout for a specified database.
    \param db [in]  A pointer to a database.
  */
  virtual OdDbStub* currentLayoutId(OdDbBaseDatabase* pDb) = 0;
  
  /** \details 
    Returns the identifier of the XRef block for a specified database.
    \param db [in]  A pointer to a database.
  */
  virtual OdDbStub* xrefBlockId(OdDbBaseDatabase* pDb) = 0;

  /** \details 
    Switches multi-threaded rendering on or off for a specified database.
    \param db   [in]  A pointer to a database.
    \param bOn  [in]  A value to be set (true if you want to switch multi-threaded rendering on or false in the other case).
  */
  virtual void setMultiThreadedRender(OdDbBaseDatabase* pDb, bool bOn) = 0;

  /** \details 
    Determines whether a specified object is a proxy object.
    \returns Returns true if the object is a proxy one, otherwise returns false.
  */
  virtual bool isAProxy(OdRxObject* pDrw) = 0;
  
  /** \details 
    Returns a smart pointer to the object determined with its identifier.
    \param pId [in]  A pointer to an object's identifier.
  */
  virtual OdRxObjectPtr openObject(OdDbStub* pId) = 0;
  virtual OdRxObjectPtr openObject(OdDbStub* pId, bool bForWrite) = 0;
  virtual bool upgradeOpen(OdRxObject* pObj) = 0;
  virtual void downgradeOpen(OdRxObject* pObj) = 0;

  virtual bool getAnnoScaleSet(OdDbStub* drawableId, OdGiAnnoScaleSet& res) = 0;
  
  /** \details 
    Returns an identifier of the current long transaction in a specified database.
    \param pId [in]  A pointer to a database.
  */
  virtual OdDbStub *getCurrentLongTransation(const OdDbBaseDatabase *pDb) = 0;

  /** \details 
    Class implements a reaction mechanism (reactor) for a database unload event.
  */
  class DatabaseUnloadReactor
  {
    public:
      /** \details 
        Destroys the database unload reactor object.
      */
      virtual ~DatabaseUnloadReactor() { }
      
      /** \details 
        Implements reactions for a database unload event.
        This method should be reimplemented to provide a custom reaction for a database unload event.
        \param pDb [in] A pointer to a database object.
      */
      virtual void goodbye(const OdDbBaseDatabase* pDb) = 0;
  };
  
  /** \details 
    Adds a new database unload reactor object to a specified database.
    \param pDb              [in] A pointer to a database object.
    \param pPrevReactor     [in] A pointer to a previous reactor object. If equal to NULL, then it is a new reactor.
    \param pReactorRedirect [in] A pointer to a database object.
    \remarks 
    There can be more than one database unload reactor. When a database unload event occurs, reactors run according to the order they have been added. 
  */  
  virtual OdRxObjectPtr addDatabaseUnloadReactor(OdDbBaseDatabase* pDb, OdRxObject *pPrevReactor, DatabaseUnloadReactor *pReactorRedirect) = 0;
  
  /** \details 
    Removes a database unload reactor from a specified database.
    This method should be reimplemented to provide a custom reaction for a database unload event.
    \param pDb      [in] A pointer to a database object.
    \param pReactor [in] A pointer to a reactor object to be removed.
  */
  virtual void removeDatabaseUnloadReactor(OdDbBaseDatabase* pDb, OdRxObject *pReactor) = 0;

  /** \details 
    Only for internal usage!
  */
  virtual OdResult faceConversionHelper(OdDbStub*& material, OdDbBaseDatabase* pSourceDb, OdDbBaseDatabase* pDestinationDb,
    const OdGiMaterialTraits* pMaterialTraits, const OdGiMapper* pMaterialMapper = 0, const OdCmEntityColor* pMaterialColor = 0) = 0;

  /** \details
    Returns database Partial Viewing Mode
  */
  virtual bool getDatabasePartialViewingMode( OdDbBaseDatabase* pDb ) const { return false; }

};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdDbBaseDatabasePE object pointers.
*/
typedef OdSmartPtr<OdDbBaseDatabasePE> OdDbBaseDatabasePEPtr;


/** \details
  This class is the protocol extension to be used as a layer abstraction, independent of the underlying database (OdDbDatabase or OdDgDatabase).

  \sa
  TD_DbRoot

  <group OdDbRoot_Classes> 
*/
class DBROOT_EXPORT OdDbBaseLayerPE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseLayerPE);
  //DOM-IGNORE-END

  /** \details
    Returns the name of a specified layer object.

    \param obj [in]  A pointer to a layer object.
  */
  virtual OdString name(const OdRxObject* obj) const = 0;

  /** \details
    Determines whether a specified layer object is off.
    
    \param obj [in]  A pointer to a layer object.
    
    \returns Returns true if the layer is off, otherwise returns false.
  */
  virtual bool isOff(const OdRxObject* obj) const = 0;
  
  /** \details
    Determines whether a specified layer is frozen. 
    
    \param obj [in]  A pointer to a layer object.
    
    \returns Returns true if the layer is frozen, otherwise returns false.
  */
  virtual bool isFrozen(const OdRxObject* obj) const = 0;

  /** \details
    Sets the on / off status of this Layer.

    \param obj [in]  A pointer to a layer object.
    \param off [in]  A new on / off status value (true for off, false for on).
  */
  virtual void setIsOff(OdRxObject* obj, bool off) const = 0;
  
  /** \details
    Sets the frozen status of this Layer.

    \param obj    [in]  A pointer to a layer object.
    \param frozen [in]  A new frozen status value (true to freeze, false to thaw).
  */
  virtual void setIsFrozen(OdRxObject* obj, bool frozen) const = 0;

  /** \details
    Returns the current linetype for the specified layer.
    
    \param obj    [in]  A pointer to a layer object.
    \param LType  [out] An object of the OdGiLinetype class to store and return the information about layer's linetype.
    
    \returns Returns true if the linetype has been successfully retrieved from the layer object, otherwise returns false.
  */
  virtual bool getLineType(OdRxObject* obj, OdGiLinetype& LType) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBaseLayerPE object pointers.
*/
typedef OdSmartPtr<OdDbBaseLayerPE> OdDbBaseLayerPEPtr;


/** \details
  This class is the protocol extension to be used as a visual style abstraction, independent of the underlying database (OdDbDatabase or OdDgDatabase).

  \sa
  TD_DbRoot

  <group OdDbRoot_Classes> 
*/
class DBROOT_EXPORT OdDbBaseVisualStylePE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseVisualStylePE);
  //DOM-IGNORE-BEGIN

  /** \details
    Returns the name of a specified visual style object.

    \param obj [in]  A pointer to a visual style object.
  */
  virtual OdString name(const OdRxObject* obj) const = 0;

  /** \details
    Determines whether a visual style object is internal.
    
    \param obj [in]  A pointer to a visual style object.
  
    \returns Returns true if the visual style object has been marked as internal, otherwise returns false.
  */
  virtual bool isInternal(const OdRxObject* obj) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBaseVisualStylePE object pointers.
*/
typedef OdSmartPtr<OdDbBaseVisualStylePE> OdDbBaseVisualStylePEPtr;

/** \details
  This class is the protocol extension to be used as an annotation scale abstraction, 
  independent of the underlying database.

  \sa
  TD_DbRoot

  <group OdDbRoot_Classes> 
*/
class DBROOT_EXPORT OdDbBaseAnnotationScalePE : public OdRxObject
{
public:
  ODRX_DECLARE_MEMBERS(OdDbBaseAnnotationScalePE);

  /** \details
    Returns the scale ratio value for a given viewport.
    
    \param obj [in]  Viewport object.
           scale [out] Scale ratio.
  */
  virtual bool scaleRatio(const OdRxObject* obj, double* scale) const = 0;
};

typedef OdSmartPtr<OdDbBaseAnnotationScalePE> OdDbBaseAnnotationScalePEPtr;


/** \details
  This class is the protocol extension to be used as a layout abstraction, independent of the underlying database (OdDbDatabase or OdDgDatabase).

  \sa
  TD_DbRoot

  <group OdDbRoot_Classes> 
*/
class DBROOT_EXPORT OdDbBaseLayoutPE : public OdRxObject
{
public:

  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseLayoutPE);
  //DOM-IGNORE-END

  /** \details 
    Determines the plot paper units.
  */
  enum PlotPaperUnits
  {
    kInches         = 0,   // Inches
    kMillimeters    = 1,   // Millimeters
    kPixels         = 2    // Pixels
  };

  /** \details 
    Determines the types of plot rotations.
  */
  enum PlotRotation
  {
    k0degrees       = 0,   //!< No rotation.
    k90degrees      = 1,   //!<  90° CCW.
    k180degrees     = 2,   //!<  Inverted.
    k270degrees     = 3    //!<  90° CW.
  };
  
  /** \details
    Determines the plot types.
  */
  enum PlotType
  {
    kDisplay        = 0,   // Display
    kExtents        = 1,   // Extents
    kLimits         = 2,   // Limits
    kView           = 3,   // View
    kWindow         = 4,   // Window
    kLayout         = 5    // Layout
  };

  /** \details
    Determines standard scale types.
  */
  enum StdScaleType
  {
    kScaleToFit     = 0,   // Scaled to Fit
    k1_128in_1ft    = 1,   // 1/128" = 1'
    k1_64in_1ft     = 2,   // 1/64" = 1'
    k1_32in_1ft     = 3,   // 1/32" = 1'
    k1_16in_1ft     = 4,   // 1/16" = 1'
    k3_32in_1ft     = 5,   // 3/32" = 1'
    k1_8in_1ft      = 6,   // 1/8" = 1'
    k3_16in_1ft     = 7,   // 3/16" = 1'
    k1_4in_1ft      = 8,   // 1/4" = 1'
    k3_8in_1ft      = 9,   // 3/8" = 1'
    k1_2in_1ft      = 10,  // 1/2" = 1'
    k3_4in_1ft      = 11,  // 3/4" = 1'
    k1in_1ft        = 12,  // 1" = 1'
    k3in_1ft        = 13,  // 3" = 1'
    k6in_1ft        = 14,  // 6" = 1'
    k1ft_1ft        = 15,  // 1' = 1'
    k1_1            = 16,  // 1:1
    k1_2            = 17,  // 1:2
    k1_4            = 18,  // 1:4
    k1_5            = 19,  // 1:5
    k1_8            = 20,  // 1:8
    k1_10           = 21,  // 1:10
    k1_16           = 22,  // 1:16
    k1_20           = 23,  // 1:20
    k1_30           = 24,  // 1:30
    k1_40           = 25,  // 1:40
    k1_50           = 26,  // 1:50
    k1_100          = 27,  // 1:100
    k2_1            = 28,  // 2:1
    k4_1            = 29,  // 4:1
    k8_1            = 30,  // 8:1
    k10_1           = 31,  // 10:1
    k100_1          = 32,  // 100:1
    k1000_1         = 33,  // 1000:1
    k1and1_2in_1ft  = 34  // 1.5"= 1' 
  };

  /** \details 
    Returns the current name of a specified layout.
    
    \param [in] A pointer to a layout object.
  */
  virtual OdString name(const OdRxObject*) = 0;
  
  /** \details 
    Determines whether a specified layout belongs to model space.
    
    \param [in] A pointer to a layout object.
    
    \returns Returns true if the layout belongs to model space, otherwise returns false.
  */
  virtual bool isModelLayout(const OdRxObject*) = 0;
  
  /** \details 
    Determines whether lineweights should be printed for a specified layout.
    
    \param [in] A pointer to a layout object.
    
    \returns Returns true if lineweights should be printed, otherwise returns false.
  */
  virtual bool printLineweights(const OdRxObject*) = 0;
  
  /** \details 
    Determines whether plot styles should be shown for a specified layout.
    
    \param [in] A pointer to a layout object.
    
    \returns Returns true if plot styles should be shown, otherwise returns false.
  */
  virtual bool showPlotStyles(const OdRxObject*) = 0;
  
  /** \details
    Returns the name of the paper orientation for a specified layout (for example, "A4"). 
    
    \param [in] A pointer to a layout object.
  */
  virtual OdString paperName(const OdRxObject*) = 0;
  
  /** \details
    Returns the current paper size for a specified layout.
    
    \param             [in]  A pointer to a layout object.
    \param paperWidth  [out] A reference for storing and retrieving paper width value.
    \param paperHeight [out] A reference for storing and retrieving paper height value.
  */
  virtual void getPaperSize(const OdRxObject*, double& paperWidth, double& paperHeight) const = 0;
  
  /** \details
    Returns the current plot rotation mode for a specified layout.
    
    \param [in] A pointer to a layout object.
  */
  virtual PlotRotation plotRotation(const OdRxObject*) = 0;
  
  /** \details
    Returns the current top margin value for a specified layout.
    
    \param [in] A pointer to a layout object.
  */
  virtual double getTopMargin(const OdRxObject*) = 0;
  
  /** \details
    Returns the current right margin value for a specified layout.
    
    \param [in] A pointer to a layout object.
  */
  virtual double getRightMargin(const OdRxObject*) = 0;
  
  /** \details
    Returns the current bottom margin value for a specified layout.
    
    \param [in] A pointer to a layout object.
  */
  virtual double getBottomMargin(const OdRxObject*) = 0;
  
  /** \details
    Returns the current left margin value for a specified layout.
    
    \param [in] A pointer to a layout object.
  */
  virtual double getLeftMargin(const OdRxObject*) = 0;
  
  /** \details
    Determines whether the overall viewport of a layout is erased.
    
    \param [in] A pointer to a layout object.
    
    \returns Returns true if the overall viewport of the layout is erased, otherwise returns false.
  */
  virtual bool isOverallVPortErased(const OdRxObject*) = 0;

  /** \details
    Returns current geometry extents of a specified layout object.
    
    \param     [in]   A pointer to a layout object.
    \param ext [out]  A reference to an OdGeExtents3d object to store and retrieve the layout's geometry extents.
  */
  virtual OdResult getGeomExtents(const OdRxObject*,OdGeExtents3d& ext) = 0;
  

  //DOM-IGNORE-BEGIN
  // methods inherited by OdDbLayoutImpl class // dgn dwf export
  //DOM-IGNORE-END
  
  /** \details 
    Determines whether a standard scale is used for a specified layout.
    
    \param [in] A pointer to a layout object.
    
    \returns Returns true if a standard scale is used for the layout, otherwise returns false.
  */
  virtual bool useStandardScale(const OdRxObject*) = 0;
  
  /** \details 
    Returns the standard scale value for a specified layout.
    
    \param        [in]  A pointer to a layout object.
    \param scale  [out] A reference to a double value to store and return the scale value.
  */
  virtual void getStdScale(const OdRxObject*, double& scale) = 0;
  
  /** \details 
    Returns a custom print scale value for a specified layout.
    
    \param [in]  A pointer to a layout object.
    \param numerator    [out] A reference to a double value to store and return the real world units.
    \param denominator  [out] A reference to a double value to store and return the drawing units.
  */
  virtual void getCustomPrintScale(const OdRxObject*, double& numerator, double& denominator) = 0;
  
  /** \details 
    Returns the current plot type for a specified layout.
    
    \param [in]  A pointer to a layout object.
  */
  virtual int plotType(const OdRxObject*) = 0;
  virtual OdResult getPlotType(const OdRxObject*, PlotType&) const = 0;

  /** \details 
    Returns a plot viewport name for a specified layout.
    
    \param A pointer to a layout object.
  */
  virtual OdString getPlotViewName(const OdRxObject*) = 0;
  
  /** \details 
    Returns the current information about the plot window area for a specified layout.
    
    \param      [in]  A pointer to a layout object.
    \param xmin [out] A reference to be used for returning the X coordinate of the window area's lower left corner.
    \param ymin [out] A reference to be used for returning the Y coordinate of the window area's lower left corner.
    \param xmax [out] A reference to be used for returning the X coordinate of the window area's upper right corner.
    \param ymax [out] A reference to be used for returning the Y coordinate of the window area's upper right corner.
  */
  virtual void getPlotWindowArea(const OdRxObject*, double& xmin, double& ymin, double& xmax, double& ymax) = 0;
  
  /** \details 
    Returns the current origin plot for a specified layout.
    
    \param   [in]  A pointer to a layout object.
    \param x [out] A reference to be used for returning the X coordinate of the layout's plot origin.
    \param y [out] A reference to be used for returning the Y coordinate of the layout's plot origin.
  */
  virtual void getPlotOrigin(const OdRxObject*, double& x, double& y) = 0;
  
  /** \details 
    Returns the current paper size for a specified layout.
    
    \param   [in]  A pointer to a layout object.
    \param x [out] A reference to be used for returning the paper width value.
    \param y [out] A reference to be used for returning the paper height value.
  */
  virtual void getPlotPaperSize(const OdRxObject*, double& paperWidth, double& paperHeight) = 0;
  
  /** \details 
    Returns the current plot units for a specified layout in paper space.
    
    \param [in]  A pointer to a layout object.
  */
  virtual int plotPaperUnits(const OdRxObject*) = 0;
  virtual OdResult getPlotPaperUnits(const OdRxObject*, PlotPaperUnits&) const = 0;

  //DOM-IGNORE-BEGIN
  // for Gs
  //DOM-IGNORE-END
  
  /** \details 
    Returns the block identifier for a specified layout.
    
    \param [in]  A pointer to a layout object.
  */
  virtual OdDbStub* getBlockId(const OdRxObject*) const = 0;
  
  /** \details 
    Determines whether generation of linetypes for polylines is used in paper space for the specified layout.
    
    \param [in]  A pointer to a layout object.
  */
  virtual bool scalePSLinetypes(const OdRxObject*) const = 0;
  
  /** \details 
    Returns the approximate extents of a specified layout.
    
    \param        [in] A pointer to a layout object.
    \param extMin [in] A reference to an OdGePoint3d object to store and return the left lower corner of the extents rectangle.
    \param extMax [in] A reference to an OdGePoint3d object to store and return the right upper corner of the extents rectangle.
  */
  virtual bool getApproxExtents(const OdRxObject*, OdGePoint3d& extMin, OdGePoint3d& extMax) const = 0;

  /** \details
    Returns the standard scale type for this PlotSettings object (DXF 75).
    
    \remarks
    stdScaleType() returns one of the of the following
    
    <table>
    Name            Value   Scale
    kScaleToFit     0       Scaled to Fit
    k1_128in_1ft    1       1/128=1'
    k1_64in_1ft     2       1/64=1'
    k1_32in_1ft     3       1/32=1'
    k1_16in_1ft     4       1/16=1'
    k3_32in_1ft     5       3/32=1'
    k1_8in_1ft      6       1/8=1'
    k3_16in_1ft     7       3/16=1'
    k1_4in_1ft      8       1/4=1'
    k3_8in_1ft      9       3/8=1'
    k1_2in_1ft      10      1/2=1'
    k3_4in_1ft      11      3/4=1'
    k1in_1ft        12      1=1'
    k3in_1ft        13      3=1'
    k6in_1ft        14      6=1'
    k1ft_1ft        15      1'=1'
    k1_1            16      1:1
    k1_2            17      1:2
    k1_4            18      1:4
    k1_5            19      1:5
    k1_8            20      1:8
    k1_10           21      1:10
    k1_16           22      1:16
    k1_20           23      1:20
    k1_30           24      1:30
    k1_40           25      1:40
    k1_50           26      1:50
    k1_100          27      1:100
    k2_1            28      2:1
    k4_1            29      4:1
    k8_1            30      8:1
    k10_1           31      10:1
    k100_1          32      100:1
    k1000_1         33      1000:1
    k1and1_2in_1ft  34      1.5"=1'
    </table>
  */
  virtual OdResult getStdScaleType(const OdRxObject*, StdScaleType&) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBaseLayoutPE object pointers.
*/
typedef OdSmartPtr<OdDbBaseLayoutPE> OdDbBaseLayoutPEPtr;

/** \details
This class is a base plot settings protocol extension class.
<group OdDb_Classes> 

*/
class DBROOT_EXPORT OdDbSetBasePlotSettingsPE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbSetBasePlotSettingsPE);
  //DOM-IGNORE-END

  /** \details
    Sets the plot device name for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotCfgName [in]  Name of the system printer or plot configuration PC3 file.

	  \returns Returns eOk if successful or an appropriate error code otherwise. 
  */
  virtual OdResult setPlotCfgName(OdRxObject* pPlotSetObj, const OdString &plotCfgName) = 0;

  /** \details
    Sets plot layout flags for the specified PlotSettings object.

    \param pPlotSettings [in] Pointer to the PlotSettings object.
    \param flags [in] Plot layout flags to be set.

	  \returns Returns eOk if successful or an appropriate error code otherwise. 
  */
  virtual OdResult setPlotLayoutFlags(OdRxObject* pPlotSetObj, OdInt16 flags) = 0;

  /** \details
    Sets the margins of the paper for the specified PlotSettings object.
    
	  \param pPlotSettings [in] Pointer to the PlotSettings object.
    \param left [in] Sets the left margin. 
    \param Bottom [in] Sets the bottom margin.
    \param right [in] Sets the right margin. 
    \param top [in] Sets the top margin.
    
    \remarks
    Margins are measured from their respective edges to the plot area and are independent of plot rotation.
    All distances are returned in millimeters, regardless of the drawing units. 

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setMargins(OdRxObject* pPlotSetObj, double left, double Bottom, double right, double top) = 0;

  /** \details
    Sets the size of the paper for the specified PlotSettings object.
	
    \param pPlotSettings [in] Pointer to the PlotSettings object.
    \param paperWidth [in] Sets the paper width. 
    \param paperHeight [in] Sets the paper height.

    \remarks
    Paper size is the physical paper size and includes margins.
    All distances are returned in millimeters, regardless of the drawing units. 

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotPaperSize(OdRxObject* pPlotSetObj, double paperWidth, double paperHeight) = 0;
    
  /** \details
    Sets the canonical (locale-independent) media name for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param mediaName [in]  Canonical media name (case-sensitive).
    
    \remarks
    The canonical media name is locale-independent.
    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setMediaName(OdRxObject* pPlotSetObj, OdString mediaName) = 0;

  /** \details
    Sets the plot origin for the specified PlotSettings object.

    \param pPlotSettings [in] Pointer to the PlotSettings object.
    \param origin [in]  Plot origin.
        
    \remarks
    The plot origin is measured with respect to the plot margin. All distances are returned in millimeters, regardless of the drawing units.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setOrigin(OdRxObject* pPlotSetObj, const OdGePoint2d &origin) = 0;

  /** \details
    Sets the plot paper units for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotPaperUnits [in]  Plot paper units.
        
    \remarks
    The plot paper units determine the units of the margins, offsets, paper size, and drawing units.
    
    plotPaperUnits must be one of the following:
    
    <table>
    Name            Value   Description
    kInches         0       Inches
    kMillimeters    1       Millimeters
    kPixels         2       Pixels
    </table>

	\returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotPaperUnits(OdRxObject* pPlotSetObj, OdDbBaseLayoutPE::PlotPaperUnits units) = 0;

  /** \details
    Sets the plot rotation for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotRotation [in]  Plot rotation.
    
    \remarks
    plotRotation must be one of the following:
    
    <table>
    Name         Value    Description
    k0degrees    0        No rotation
    k90degrees   1        90° CCW
    k180degrees  2        Inverted°
    k270degrees  3        90° CW
    </table>

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotRotation(OdRxObject* pPlotSetObj, OdDbBaseLayoutPE::PlotRotation plotRotation) = 0;

  /** \details
    Sets the plot type for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotType [in]  Plot type.

    \remarks
    plotType must be one of the following:
    
    <table>
    Name        Value   Description
    kDisplay    0       Display
    kExtents    1       Extents
    kLimits     2       Limits
    kView       3       View
    kWindow     4       Window
    kLayout     5       Layout
    </table>

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotType(OdRxObject* pPlotSetObj, OdDbBaseLayoutPE::PlotType plotType) = 0;

  /** \details
    Sets the corners of the plot window area for the specified PlotSettings object.

    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param xMin [in]  X-coordinate of the lower-left corner.      
    \param yMin [in]  Y-coordinate of the lower-left corner.      
    \param xMax [in]  X-coordinate of the upper-right corner.      
    \param yMax [in]  Y-coordinate of the upper-right corner.      
    
    \remarks
    The corners define the area to be plotted if and only if plotType == kWindow.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotWindowArea(OdRxObject* pPlotSetObj, double xmin, double ymin, double xmax, double ymax) = 0;

  /** \details
    Sets the plot view for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param plotViewId [in]  Plot view Object Id to set.
  
    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPlotView(OdRxObject* pPlotSetObj, OdDbStub* plotViewId) = 0;

  /** \details
    Sets the custom print scale for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param numerator [in]  PaperSpace units.
    \param denominator [in]  Media units.
    
    \remarks
    The custom print scale always reflects the scale that this PlotSettings object will use to plot.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setPrintScale(OdRxObject* pPlotSetObj, double numerator, double denominator) = 0;

  /** \details
    Sets the current style sheet for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param styleSheet [in]  Name of current style sheet.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setStyleSheet(OdRxObject* pPlotSetObj, OdString styleSheet) = 0;

  /** \details
    Sets the standard scale type for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param scaleType [in]  Standard scale type to be set.

    \returns Returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setStdScaleType(OdRxObject* pPlotSetObj, OdDbBaseLayoutPE::StdScaleType scaleType) = 0;

  /** \details
    Sets the scale factor, as a floating point value, for the specified PlotSettings object.
    
    \param pPlotSettings [in]  Pointer to the PlotSettings object.
    \param dScaleFactor [in]  Scale factor.
  */
  virtual OdResult setScaleFactor(OdRxObject* pPlotSetObj, double dScaleFactor) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbSetBasePlotSettingsPE object pointers.
*/
typedef OdSmartPtr<OdDbSetBasePlotSettingsPE> OdDbSetBasePlotSettingsPEPtr;


/** \details
  This class is the protocol extension to be used as a block table record abstraction, independent of the underlying database (OdDbDatabase or OdDgDatabase).

  \sa
  TD_DbRoot

  <group OdDbRoot_Classes>
*/
class DBROOT_EXPORT OdDbBaseBlockPE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseBlockPE);
  //DOM-IGNORE-END

  /** \details 
    Determines whether a block reference has been added to a specified block.
    
    \param pBlock [in] A pointer to a block.
    
    \returns Returns true if a block reference has been added, otherwise returns false.
  */
  virtual bool isBlockReferenceAdded(OdRxObject *pBlock) const = 0;

  /** \details 
    Determines whether a specified block is an external reference.
    
    \param pBlock [in] A pointer to a block.
    
    \returns Returns true if the block is an external reference, otherwise returns false.
  */
  virtual bool isFromExternalReference(const OdRxObject *pBlock) const = 0;
  
  /** \details 
    Returns a referenced database for a specified block.
    
    \param pBlock [in] A pointer to a block.
  */
  virtual OdDbBaseDatabase *xrefDatabase(const OdRxObject *pBlock) const = 0;

  /** \details 
    Returns a specified block's layout identifier.
    
    \param pBlock [in] A pointer to a block.
  */
  virtual OdDbStub *getLayoutId(const OdRxObject *pBlock) const = 0;

  /** \details
    Returns true if and only if block is an anonymous block.

    \param pBlock [in] A pointer to a block.
  */
  virtual bool isAnonymous( const OdRxObject *pBlock ) const = 0;

  /** \details
    Returns the name of block.

    \param pBlock [in] A pointer to a block.
  */
  virtual OdString getName( const OdRxObject *pBlock ) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBaseBlockPE object pointers.
*/
typedef OdSmartPtr<OdDbBaseBlockPE> OdDbBaseBlockPEPtr;


/** \details
  This class is the protocol extension to be used as a block reference abstraction, independent of the underlying database (OdDbDatabase or OdDgDatabase).

  \sa
  TD_DbRoot

  <group OdDbRoot_Classes>
*/
class DBROOT_EXPORT OdDbBaseBlockRefPE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseBlockRefPE);
  //DOM-IGNORE-END

  /** \details 
    Returns the identifier of a specified block reference. 
    
    \param pBlockRef [in] A pointer to a block reference.
  */
  virtual OdDbStub *blockId(const OdRxObject *pBlockRef) const = 0;
  
  /** \details 
    Returns the transformation matrix for a specified block reference. 
    
    \param pBlockRef [in] A pointer to a block reference.
  */
  virtual OdGeMatrix3d blockTransform(const OdRxObject *pBlockRef) const = 0;

  /** \details 
    Returns the newly created attribute iterator for a specified block reference.
    
    \param pBlockRef    [in] A pointer to a block reference.
    \param bSkipErased  [in] A flag that determines whether erased elements should be skipped when moving through the iterator. If the parameter value is equal to true, erased elements are skipped.
  */
  virtual OdRxIteratorPtr newAttribIterator(const OdRxObject *pBlockRef, bool bSkipErased = true) const = 0;
  
  /** \details 
    Returns whether a specified object is an attribute.
    
    \param pAttrib [in] A pointer to an object.
  */
  virtual bool isAttribute(const OdRxObject *pAttrib) const = 0;

  /** \details 
    Returns whether a specified block reference is generic.
    
    \param pBlockRef [in] A pointer to a block reference.
    
    \returns Returns true if the block reference is a generic one, otherwise returns false.
    
    \remarks 
    Gs system will not process a drawable as a block reference if this method returns false. 
    Therefore the method can be used to filter out incompatible entities inherited from the OdDbBaseBlockRef class.
  */  
  virtual bool isGeneric(const OdRxObject *pBlockRef) const = 0;
  
  /** \details 
    Returns whether a specified block reference is an array of block references (an instance of the OdMInsertBlock class).
    
    \param pBlockRef [in] A pointer to a block reference.
    
    \returns Returns true if the block reference is an array of block references, otherwise returns false.
  */
  virtual bool isMInsert(const OdRxObject *pBlockRef) const = 0;
  
  /** \details 
    Returns whether a specified block reference is a basic one.
    
    \param pBlockRef [in] A pointer to a block reference.
    
    \returns Returns true if the block reference is a basic one, otherwise returns false.
  */  
  virtual bool isBasic(const OdRxObject *pBlockRef) const = 0;

  /** \details
  Returns whether create child entity node if entity is self dependent.

  \param pEntity [in] A pointer to a entity.

  \returns Returns true if it is possible to create child entity node if entity is self dependent, 
  \otherwise returns false.
  */
  virtual bool isSelfReferential(const OdRxObject *pEntity) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBaseBlockRefPE object pointers.
*/
typedef OdSmartPtr<OdDbBaseBlockRefPE> OdDbBaseBlockRefPEPtr;


/** \details
  This class is the protocol extension to be used as an entity object sorting abstraction, independent of the underlying database (OdDbDatabase or OdDgDatabase).

  \sa
  TD_DbRoot

  <group OdDbRoot_Classes>
*/
class DBROOT_EXPORT OdDbBaseSortEntsPE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseSortEntsPE);
  //DOM-IGNORE-END

  /** \details 
    Returns a pointer to the identifier of a specified sorted entity protocol extension instance.
    
    \param pSortents [in] A pointer to a sorted entity protocol extension instance.
  */  
  virtual OdDbStub *blockId(const OdRxObject *pSortents) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBaseSortEntsPE object pointers.
*/
typedef OdSmartPtr<OdDbBaseSortEntsPE> OdDbBaseSortEntsPEPtr;


/** \details
  This class is the protocol extension to be used as a long transaction abstraction, independent of the underlying database (OdDbDatabase or OdDgDatabase).

  \sa
  TD_DbRoot

  <group OdDbRoot_Classes>
*/
class DBROOT_EXPORT OdDbBaseLongTransactionPE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseLongTransactionPE);
  //DOM-IGNORE-END

  /** \details 
    Returns a pointer to the identifier of a block where entities have been copied.
    
    \param pLT [in] A pointer to a long transaction object.
  */  
  virtual OdDbStub *destinationBlock(const OdRxObject *pLT) const = 0;
  
  /** \details 
    Returns a pointer to the database, to which a specified long transaction object belongs.
    
    \param pLT [in] A pointer to a long transaction object.
  */    
  virtual OdDbBaseDatabase *getDatabase(const OdRxObject *pLT) const = 0;

  /** \details 
    Determines whether a specified long transaction object contains a specified object in its workset.
    
    \param pLT [in] A pointer to a long transaction object.
    \param pId [in] A pointer to an identifier of an object.
    
    \returns Returns true if the long transaction workset contains the object specified with the identifier pId parameter. Otherwise returns false.
  */
  virtual bool workSetHas(const OdRxObject *pLT, OdDbStub *pId) const = 0;
  
  /** \details 
    Creates an iterator for a specified long transaction workset.
    
    \param pLT          [in] A pointer to a long transaction object.
    \param incRemoved   [in] A flag that determines whether the iterator includes removed objects (if equal to true).
    \param incSecondary [in] A flag that determines whether the iterator includes secondary objects (if equal to true).
  */
  virtual OdRxIteratorPtr newWorkSetIterator(const OdRxObject *pLT, bool incRemoved = false, bool incSecondary = false) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBaseLongTransactionPE object pointers.
*/
typedef OdSmartPtr<OdDbBaseLongTransactionPE> OdDbBaseLongTransactionPEPtr;

/** \details
This class defines the interface for the hatch protocol extension classes.
<group OdDbRoot_Classes>
*/
class DBROOT_EXPORT OdDbBaseHatchPE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseHatchPE);
  //DOM-IGNORE-END
  
  /** \details 
    Determines hatch style values.
  */
  enum HatchStyle
  {
    kNormal = 0, //!< Hatch toggles on each boundary.
    kOuter  = 1, //!< Hatch turns off after first inner loop.
    kIgnore = 2  //!< Hatch ignores inner loop
  };

  /** \details 
    Determines types of hatch loops.
  */
  enum HatchLoopType
  {
    kDefault = 0,                 //!< Not yet specified.
    kExternal = 1,                //!< Defined by external entities.
    kPolyline = 2,                //!< Defined by OdGe polyline.
    kDerived = 4,                 //!< Derived from a picked point.
    kTextbox = 8,                 //!< Defined by text.
    kOutermost = 0x10,            //!< Outermost loop.
    kNotClosed = 0x20,            //!< Open loop.
    kSelfIntersecting = 0x40,     //!< Self-intersecting loop.
    kTextIsland = 0x80,           //!< Text loop surrounded by an even number of loops.
    kDuplicate = 0x100,           //!< Duplicate loop.
    kIsAnnotative = 0x200,        //!< The bounding area is an annotative block.
    kDoesNotSupportScale = 0x400, //!< The bounding type does not support scaling.
    kForceAnnoAllVisible = 0x800, //!< Forces all annotatives to be visible.
    kOrientToPaper = 0x1000,      //!< Orients hatch loop to paper.
    kIsAnnotativeBlock = 0x2000   //!< Describes whether the hatch is an annotative block.
  };

  /** \details 
    Returns the quantity of loops in a specified hatch.
    
    \param pHatch [in] A pointer to a hatch object.
  */
  virtual int numLoops(const OdRxObject* pHatch) const = 0;

  /** \details 
    Returns the type of loop, determined by its index, in a specified hatch.
    
    \param pHatch     [in] A pointer to a hatch object.
    \param loopIndex  [in] An index of a hatch loop.
  */
  virtual OdInt32 loopTypeAt(const OdRxObject* pHatch, int loopIndex) const = 0;
  
  /** \details 
    Returns curves of a loop, determined by its index, in a specified hatch.
    
    \param pHatch     [in]  A pointer to a hatch object.
    \param loopIndex  [in]  An index of a hatch loop.
    \param edgePtrs   [out] A reference to an OdGeCurve2d object array to store information about the curves that form the loop.
  */
  virtual void getLoopAt(const OdRxObject* pHatch, int loopIndex, OdArray<OdGeCurve2d*>& edgePtrs) const = 0;

  /** \details 
    Returns curves of a loop, determined by its index, in a specified hatch.
    
    \param pHatch     [in]  A pointer to a hatch object.
    \param loopIndex  [in]  An index of a hatch loop.
    \param edgePtrs   [out] A reference to an OdGeCurve2d object array to store information about the curves that form the loop.
  */
  virtual void getLoopAt(const OdRxObject* pHatch, int loopIndex, OdGePoint2dArray& vertices, OdGeDoubleArray& bulges) const = 0;
  
  /** \details 
    Returns the style of a specified hatch.
    
    \param pHatch     [in]  A pointer to a hatch object.
  */
  virtual HatchStyle hatchStyle(const OdRxObject* pHatch) const = 0;

  /** \details 
    Determines whether a specified hatch is a gradient hatch.
    
    \param pHatch     [in]  A pointer to a hatch object.
    
    \returns Returns true if a hatch is a gradient hatch, otherwise returns false.
  */
  virtual bool isGradient(const OdRxObject* pHatch) const = 0;

  /** \details 
    Determines whether a specified hatch is a solid hatch.
    
    \param pHatch     [in]  A pointer to a hatch object.
    
    \returns Returns true if a hatch is a solid hatch, otherwise returns false.
  */
  virtual bool isSolidFill(const OdRxObject* pHatch) const = 0;

  /** \details 
    Determines whether a specified hatch is a real hatch.
    
    \param pHatch     [in]  A pointer to a hatch object.
    
    \returns Returns true if a hatch is a real hatch, otherwise returns false.
    
    \remarks
    For .dwg functionality a hatch is always a hatch protocol extension, but for .dgn functionality a hatch protocol extension can be added to any entity.
  */
  virtual bool isReallyHatch(const OdRxObject* pHatch) const = 0;
};

/** \details
This template class is a specialization of the OdSmartPtr class for OdDbHatchPE object pointers.
*/
typedef OdSmartPtr<OdDbBaseHatchPE> OdDbBaseHatchPEPtr;



/** \details
  This class is the protocol extension to be used as a material abstraction, independent of the underlying database (OdDbDatabase or OdDgDatabase).

  \sa
  TD_DbRoot

  <group OdDbRoot_Classes> 
*/
class DBROOT_EXPORT OdDbBaseMaterialPE : public OdRxObject
{
public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbBaseMaterialPE);
  //DOM-IGNORE-BEGIN

  /** \details
    Returns the name of a specified material object.

    \param obj [in]  A pointer to a material object.
  */
  virtual OdString name(const OdRxObject* obj) const = 0;

  /** \details
    Determines whether a material object is internal.
    
    \param obj [in]  A pointer to a material object.
  
    \returns Returns true if the material object has been marked as internal, otherwise returns false.
  */
  virtual bool isDefault(const OdRxObject* obj) const = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for OdDbBaseMaterialPE object pointers.
*/
typedef OdSmartPtr<OdDbBaseMaterialPE> OdDbBaseMaterialPEPtr;

#endif // _ODDB_BASEDATABASE_INCLUDED_
