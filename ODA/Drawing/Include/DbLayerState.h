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

#ifndef DBLAYERSTATEMANAGER_H
#define DBLAYERSTATEMANAGER_H

#include "TD_PackPush.h"
#include "StringArray.h"

class OdDbLayerStateManagerReactor;
class OdDbLayerStateManagerImpl;

/** \details
  This class represents Layer States Manager objects. 
  Corresponding C++ library: TD_Db
  <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbLayerStateManager: public OdRxObject
{
protected:
  OdDbLayerStateManager(OdDbLayerStateManagerImpl*);
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbLayerStateManager);
//DOM-IGNORE-END

  /** \details
    Default constructor for objects of the OdDbLayerStateManager class.
  */
  OdDbLayerStateManager();

  /** \details
    Default destructor for objects of the OdDbLayerStateManager class.
  */
  ~OdDbLayerStateManager();

  /** \details
    Enumeration that determines which layer attributes are to be stored in the
    LayerState.
  */
  enum LayerStateMask {
    kNone           = 0x0000,  //None
    kOn             = 0x0001,  //On
    kFrozen         = 0x0002,  //Frozen
    kLocked         = 0x0004,  //Locked
    kPlot           = 0x0008,  //Plot
    kNewViewport    = 0x0010,  //New viewport
    kColor          = 0x0020,  //Color
    kLineType       = 0x0040,  //Line type
    kLineWeight     = 0x0080,  //Line weight
    kPlotStyle      = 0x0100,  //Plot style
    kCurrentViewport = 0x0200, //Current viewport
    kTransparency   = 0x0400,  //Transparency
    kAll            = kOn | kFrozen | kLocked | kPlot | kNewViewport |
    kColor | kLineType | kLineWeight | kPlotStyle |
    kCurrentViewport | kTransparency, //All
    kStateIsHidden  = 0x8000,  //State is hidden
    kLastRestored   = 0x10000, //Last restored
    kDecomposition  = kAll | 0x20000 //Decomposition
  };

  /** \details
    This enumeration determines flags that are used in restore layer state
    operations. The first three flags indicate what to do with the layers that
    are not included in the layer state being restored.
  */
  enum {
    kUndefDoNothing = 0,    //Restore as is
    kUndefTurnOff = 1,      //Restore as turned off
    kUndefFreeze = 2,       //Restore as frozen
    kRestoreAsOverrides = 4 //Restore as overrides
  };

  /** \details
    Adds a state manager reactor to this state manager object.

    \param OdDbLayerStateManagerReactor [in]  Pointer to the state manager reactor object to be added.

    \returns True if the state manager reactor was added successfully; false
             means that this reactor has been already added before.
  */
  bool                addReactor(OdDbLayerStateManagerReactor * pReactor);

  /** \details
    Removes a state manager reactor to this state manager object.

    \param OdDbLayerStateManagerReactor [in]  Pointer to the state manager reactor object to be removed.

    \returns True if the state manager reactor was removed successfully; false
             otherwise.
  */
  bool                removeReactor(OdDbLayerStateManagerReactor * pReactor);

  /** \details
    Gets the layer state dictionary ID if present in the current database. The
    layer state dictionary can be created if it's not present and the
    bCreateIfNotPresent parameter is set to true.

    \param bCreateIfNotPresent [in]  Create a layer state dictionary if it's not
           present in the current database.

    \returns The layer state dictionary ID if it's present in the current
    database or the bCreateIfNotPresent parameter is set tu true; kNull
    otherwise.
  */
  OdDbObjectId        layerStatesDictionaryId(bool bCreateIfNotPresent=false);

  /** \details
    Indicates if the layer state with the given name is managed by this layer
    state manager object.

    \param sName [in]  Name of the layer state.

    \returns True if the layer state is managed by this layer state manager
    object; false otherwise.
  */
  bool                hasLayerState(const OdString& sName);

  /** \details
    Saves a layer state to the current database.

    \param sName [in]  Name of the layer state to be saved.
    \param mask  [in]  Layer state mask.

    \remarks
    Layer state mask determines which layer attributes are to be stored in the
    layer state. Layer state mask must be a combination of one or more of the
    following:

    <table>
    Name                   Value
    kNone                  0x0000
    kOn                    0x0001
    kFrozen                0x0002
    kLocked                0x0004
    kPlot                  0x0008
    kNewViewport           0x0010
    kColor                 0x0020
    kLineType              0x0040
    kLineWeight            0x0080
    kPlotStyle             0x0100
    kCurrentViewport       0x0200
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency
    kHidden                0x8000
    </table>

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   saveLayerState(const OdString& sName, LayerStateMask mask);

  /** \details
    Restores a layer state from the current database.

    \param sName [in]  Name of the layer state to be restored.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   restoreLayerState(const OdString& sName);

  /** \details
    Sets the layer state mask for a specific layer state.

    \param sName [in]  Name of the layer state to set the mask for.
    \param mask  [in]  Layer state mask.

    \remarks
    Layer state mask determines which layer attributes are to be stored in the
    layer state. Layer state mask must be a combination of one or more of the
    following:

    <table>
    Name                   Value
    kNone                  0x0000
    kOn                    0x0001
    kFrozen                0x0002
    kLocked                0x0004
    kPlot                  0x0008
    kNewViewport           0x0010
    kColor                 0x0020
    kLineType              0x0040
    kLineWeight            0x0080
    kPlotStyle             0x0100
    kCurrentViewport       0x0200
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency
    kHidden                0x8000
    </table>

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   setLayerStateMask(const OdString& sName, LayerStateMask mask);

  /** \details
    Retrieves a layer state mask from a specific layer state.

    \param sName       [in]  Name of the layer state to get the mask from.
    \param returnMask [out]  Layer state mask.

    \remarks
    Layer state mask determines which layer attributes are stored in the
    layer state. Layer state mask is a combination of one or more of the
    following:

    <table>
    Name                   Value
    kNone                  0x0000
    kOn                    0x0001
    kFrozen                0x0002
    kLocked                0x0004
    kPlot                  0x0008
    kNewViewport           0x0010
    kColor                 0x0020
    kLineType              0x0040
    kLineWeight            0x0080
    kPlotStyle             0x0100
    kCurrentViewport       0x0200
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency
    kHidden                0x8000
    </table>

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   getLayerStateMask(const OdString& sName, LayerStateMask &returnMask);

  /** \details
    Deletes a layer state.

    \param sName [in]  Name of the layer state to be deleted.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   deleteLayerState(const OdString& sName);

  /** \details
    Renames a layer state.

    \param sName    [in]  Name of the layer state to renamed.
    \param sNewName [in]  New name to be assigned to the layer state.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   renameLayerState(const OdString& sName, const OdString& sNewName);

  /** \details
    Imports a layer state.

    \param pStreamBuf [in]  Stream buffer to import a layer state from.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   importLayerState(OdStreamBuf* pStreamBuf);

  /** \details
    Imports a layer state.

    \param pStreamBuf [in]  Stream buffer to import a layer state from.
    \param sName     [out]  Name of the imported layer state.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   importLayerState(OdStreamBuf* pStreamBuf, OdString& sName);

  /** \details
    Exports a specific layer state.

    \param sNameToExport [in]  Name of the layer state to export.
    \param pStreamBuf    [in]  Stream buffer to export the layer state to. 

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   exportLayerState(const OdString& sNameToExport, OdStreamBuf* pStreamBuf);

  /** \details
    Saves a layer state to the current database.

    \param sName [in]  Name of the layer state to be saved.
    \param mask  [in]  Layer state mask.
    \param idVp  [in]  ID of the viewport whose layer state is to be saved.

    \remarks
    Layer state mask determines which layer attributes are to be stored in the
    layer state. Layer state mask must be a combination of one or more of the
    following:

    <table>
    Name                   Value
    kNone                  0x0000
    kOn                    0x0001
    kFrozen                0x0002
    kLocked                0x0004
    kPlot                  0x0008
    kNewViewport           0x0010
    kColor                 0x0020
    kLineType              0x0040
    kLineWeight            0x0080
    kPlotStyle             0x0100
    kCurrentViewport       0x0200
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency
    kHidden                0x8000
    </table>

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   saveLayerState(const OdString& sName, LayerStateMask mask, const OdDbObjectId& idVp);

  /** \details
    Restores the specified layer state.

    \param sName         [in]  Name of the layer state to be restored.
    \param idVp          [in]  ID of the viewport to restore the layer state for.
    \param nRestoreFlags [in]  Restore flags.
    \param pClientMask   [in]  Layer state mask of the target viewport.

    \remarks
    Layer state mask determines which layer attributes are restored from the
    layer state. Layer state mask must be a combination of one or more of the
    following:

    <table>
    Name                   Value
    kNone                  0x0000
    kOn                    0x0001
    kFrozen                0x0002
    kLocked                0x0004
    kPlot                  0x0008
    kNewViewport           0x0010
    kColor                 0x0020
    kLineType              0x0040
    kLineWeight            0x0080
    kPlotStyle             0x0100
    kCurrentViewport       0x0200
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency
    kHidden                0x8000
    </table>

    Restore flags determine what to do with layers that are missing in the layer
    state being restored, but are present in the viewport. The
    kRestoreAsOverrides flag determines whether the properties that can be
    overriden in a viewport (such as color) will be restored as overrides.
    The nRestoreFlags flags parameter must be a combination of one or more of
    the following flags:

    <table>
    Name                   Value
    kUndefDoNothing        0x0000
    kUndefTurnOff          0x0001
    kUndefFreeze           0x0002
    kRestoreAsOverrides    0x0004
    </table>

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   restoreLayerState(const OdString& sName, const OdDbObjectId& idVp, int nRestoreFlags = 0, const LayerStateMask* pClientMask = NULL);

  /** \details
    Sets a description for a specified layer state.

    \param sName [in]  Name of the layer state to set description for.
    \param sDesc [in]  Description text.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   setLayerStateDescription(const OdString& sName, const OdString& sDesc);

  /** \details
    Retrieves a description from a specified layer state.

    \param sName  [in]  Name of the layer state to retrieve a description from.
    \param sDesc [out]  Description text.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   getLayerStateDescription(const OdString& sName, OdString& sDesc);

  /** \details
    Checks if a specified layer has viewport data.

    \param sName [in]  Name of the layer state.

    \returns True if the specified layer state has viewport data, meaning that
    the layer state was saved using some viewport and could use viewport
    overrides; false otherwise.
  */
  bool       layerStateHasViewportData(const OdString&  sName);

  /** \details
    Imports a layer state from a specified database.

    \param pStateName [in]  Name of the layer state to be imported.
    \param pDb        [in]  Database to import the layer state from.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   importLayerStateFromDb(const OdString& pStateName, OdDbDatabase* pDb);

  /** \details
    Retrieves a list of names of layer states contained in the current database.

    \param lsArray       [out]  Array of names of layer states.
    \param bIncludeHidden [in]  If true (default) hidden layer states are included in the resulting list.
    \param bIncludeXref   [in]  If true (default) cross-reference layer states are included in the resulting list.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   getLayerStateNames(OdStringArray& lsArray, bool bIncludeHidden = true, bool bIncludeXref = true);

  /** \details
    Gets the name of the last restored layer state and the ID of the
    corresponding LayerState object.

    \param sName        [out]  Name of the layer state.
    \param restoredLSId [out]  ID of the LayerState object.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   getLastRestoredLayerState(OdString& sName, OdDbObjectId &restoredLSId);

  /** \details
    Retrieves a list of names of layers associated with a specified layer state.

    \param layerArray [out]  Array of names of layers.
    \param sName       [in]  Name of layer state to get the list of layer names for.
    \param bInvert     [in]  If true the method will return the inverted list,
                             i.e. the list of layer names in the current
                             database, that are not associated with a specified
                             layer state.

    \returns eOk if operation succeeded; error code otherwise.

    \remarks
    If bInvert is true, the resulting list of layer names is not cleared, just
    appended.
  */
  OdResult   getLayerStateLayers(OdStringArray& layerArray, const OdString& sName, bool bInvert = false);

  /** \details
    Compares a specified layer state to the layer state, associated with a
    specified viewport.

    \param sName [in]  Layer state name.
    \param idVp  [in]  Viewport ID.

    \returns True if layer states are equal; false otherwise.

    \remarks
    This method is not implemented yet. Returns false in any case.
  */
  bool       compareLayerStateToDb(const OdString& sName, const OdDbObjectId& idVp);

  /** \details
    Adds layers to the layer state.

    \param sName    [in]  Name of the layer state to add layers to.
    \param layerIds [in]  Array of ID's of layers to be added.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   addLayerStateLayers(const OdString& sName, const OdDbObjectIdArray& layerIds);

  /** \details
    Removes layers from layer state.

    \param sName      [in]  Name of the layer state to remove layers from.
    \param layerNames [in]  Array of names of layers to be removed.

    \returns eOk if operation succeeded; error code otherwise.
  */
  OdResult   removeLayerStateLayers(const OdString& sName, const OdStringArray& layerNames);

  /** \details
    Shows if the layer state is from xref.

    \param sName [in]  Name of the layer state.

    \returns True if the specified layer state is from xref; false otherwise.
  */
  bool       isDependentLayerState(const OdString& sName);

  /** \details
    Gets the database associated with this layer state manager.

    \returns Pointer to the database.
  */
  OdDbDatabase* getDatabase() const;

protected:
  OdDbLayerStateManagerImpl* m_pImpl;
  friend class OdDbLayerStateManagerImpl;
  friend class OdDbDatabase;
};


/** \details
  This class defines reactor objects for layer state manager objects (objects
  of the OdDbLayerStateManager class).

  Corresponding C++ library: TD_Db
  <group OdDb_Classes> 
*/
class TOOLKIT_EXPORT OdDbLayerStateManagerReactor : public OdRxObject
{
public:
//DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdDbLayerStateManagerReactor);
//DOM-IGNORE-END

  /** \details
    This function is triggered every time a layer state is saved to the database.

    \param layerStateName [in]  Name of the layer state.
    \param layerStateId   [in]  ID of the corresponding OdDbLayerState object.
  */
  virtual void layerStateCreated(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);

  /** \details
    This function is triggered every time when comparison of a specified layer
    state with the layer of a viewport fails.

    \param layerStateName [in]  Name of the layer state.
    \param layerStateId   [in]  ID of the corresponding OdDbLayerState object.

    \remarks
    Currently not used.
  */
  virtual void layerStateCompareFailed(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);

  /** \details
    This function is triggered every time before a layer state is restored.

    \param layerStateName [in]  Name of the layer state.
    \param layerStateId   [in]  ID of the corresponding OdDbLayerState object.
  */
  virtual void layerStateToBeRestored(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);

  /** \details
    This function is triggered every time when a layer state is restored.

    \param layerStateName [in]  Name of the layer state.
    \param layerStateId   [in]  ID of the corresponding OdDbLayerState object.
  */
  virtual void layerStateRestored(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);

  /** \details
    This function is triggered every time when a layer state restoration is
    aborted.

    \param layerStateName [in]  Name of the layer state.
    \param layerStateId   [in]  ID of the corresponding OdDbLayerState object.
  */
  virtual void abortLayerStateRestore(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);


  /** \details
    This function is triggered every time before a layer state is deleted.

    \param layerStateName [in]  Name of the layer state.
    \param layerStateId   [in]  ID of the corresponding OdDbLayerState object.
  */
  virtual void layerStateToBeDeleted(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);

  /** \details
    This function is triggered every time when a layer state is deleted.

    \param layerStateName [in]  Name of the layer state.
  */
  virtual void layerStateDeleted(const OdString& layerStateName);

  /** \details
    This function is triggered every time when a layer state deletion is
    aborted.

    \param layerStateName [in]  Name of the layer state.
    \param layerStateId   [in]  ID of the corresponding OdDbLayerState object.
  */
  virtual void abortLayerStateDelete(const OdString& layerStateName,
    const OdDbObjectId& layerStateId);


  /** \details
    This function is triggered every time before a layer state is renamed.

    \param oldLayerStateName [in]  Old name of the layer state.
    \param newLayerStateName [in]  New name of the layer state.
  */
  virtual void layerStateToBeRenamed(const OdString& oldLayerStateName,
    const OdString& newLayerStateName);

  /** \details
    This function is triggered every time when a layer state is renamed.

    \param oldLayerStateName [in]  Old name of the layer state.
    \param newLayerStateName [in]  New name of the layer state.
  */
  virtual void layerStateRenamed(const OdString& oldLayerStateName,
    const OdString& newLayerStateName);

  /** \details
    This function is triggered every time when a layer state renaming is
    aborted.

    \param oldLayerStateName [in]  Old name of the layer state.
    \param newLayerStateName [in]  New name of the layer state.
  */
  virtual void abortLayerStateRename(const OdString& oldLayerStateName,
    const OdString& newLayerStateName);
};


/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdDbLayerStateManagerReactor object pointers.
*/
typedef OdSmartPtr<OdDbLayerStateManagerReactor> OdDbLayerStateManagerReactorPtr;

/** \details
  This class implements an interface for manipulating LayerStates in multiple OdDbDatabase instances.

  Corresponding C++ library: TD_Db
  <group OdDb_Classes>
*/
class TOOLKIT_EXPORT OdDbLayerState
{
public:

  /** \details
    Returns the Object ID of the LayerStates dictionary for the specified database.

    \param pDb [in]  Pointer to the database.
    \param createIfNotFound [in]  Controls the creation of the dictionary if it does not exist.

    \returns Object ID of the LayerStates dictionary.
  */
  static OdDbObjectId dictionaryId(OdDbDatabase *pDb, bool createIfNotFound = false);

  /** \details
    Returns true if and only if the specified database
    has a LayerState with the specified name.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.

    \returns True if the specified database has a LayerState with the specified
    name; false otherwise.
  */
  static bool has(OdDbDatabase *pDb, const OdString &layerStateName);

  /** \details
    Saves the current LayerState of the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerStateMask [in]  LayerState mask.
    \param viewportId [in]  Viewport whose LayerState is to be saved.

    \remarks
    layerStateMask determines which layer attributes are to be stored in the LayerState.
    layerStateMask must be a combination of one or more of the following:

    <table>
    Name                   Value
    kNone                  0x0000
    kOn                    0x0001
    kFrozen                0x0002
    kLocked                0x0004
    kPlot                  0x0008
    kNewViewport           0x0010
    kColor                 0x0020
    kLineType              0x0040
    kLineWeight            0x0080
    kPlotStyle             0x0100
    kCurrentViewport       0x0200
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency
    kHidden                0x8000
    </table>

  */
  static void save(OdDbDatabase *pDb, const OdString &layerStateName, int layerStateMask, const OdDbObjectId &viewportId = OdDbObjectId::kNull);

  /** \details
    Restores the specified LayerState of the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerStateMask [in]  LayerState mask.
    \param viewportId [in]  Viewport whose LayerState is to be saved.
    \param undefAction [in]  Action for undefined layers.

    \remarks
    undefAction specified the action for layers not defined in the LayerState.
    undefAction must be one of the following:

    <table>
    Name               Value
    kUndefDoNothing    0
    kUndefTurnOff      1
    kUndefFreeze       2
    </table>

    layerStateMask determines which layer attributes are to be restored from the LayerState.
    layerStateMask must be a combination of one or more of the following:

    <table>
    Name                   Value
    kDefMask               0xFFFFFFFF
    kNone                  0x0000
    kOn                    0x0001
    kFrozen                0x0002
    kLocked                0x0004
    kPlot                  0x0008
    kNewViewport           0x0010
    kColor                 0x0020
    kLineType              0x0040
    kLineWeight            0x0080
    kPlotStyle             0x0100
    kCurrentViewport       0x0200
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency
    kHidden                0x8000
    </table>

  */
  static void restore(OdDbDatabase *pDb, const OdString &layerStateName,
    int flags = OdDbLayerStateManager::kUndefDoNothing, const int layerStateMask = -1, const OdDbObjectId& viewportId = OdDbObjectId::kNull);
  /** \details
    Sets the LayerState mask for the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerStateMask [in]  LayerState mask.

    \remarks
    layerStateMask determines which layer attributes are to be restored from the LayerState.
    layerStateMask must be a combination of one or more of the following:

    <table>
    Name                   Value
    kNone                  0x0000
    kOn                    0x0001
    kFrozen                0x0002
    kLocked                0x0004
    kPlot                  0x0008
    kNewViewport           0x0010
    kColor                 0x0020
    kLineType              0x0040
    kLineWeight            0x0080
    kPlotStyle             0x0100
    kCurrentViewport       0x0200
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency
    kHidden                0x8000
    </table>

  */
  static void setMask(OdDbDatabase *pDb, const OdString &layerStateName, int layerStateMask);

  /** \details
    Returns the LayerState mask for the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.

    \returns LayerState mask.

    \remarks
    The layer state mask determines which layer attributes are to be restored from the LayerState.

    mask() returns a combination of one or more of the following:

    <table>
    Name                   Value
    kNone                  0x0000
    kOn                    0x0001
    kFrozen                0x0002
    kLocked                0x0004
    kPlot                  0x0008
    kNewViewport           0x0010
    kColor                 0x0020
    kLineType              0x0040
    kLineWeight            0x0080
    kPlotStyle             0x0100
    kCurrentViewport       0x0200
    kTransparency          0x0400
    kAll                   kOn | kFrozen | kLocked | kPlot | kNewViewport |
                           kColor | kLineType | kLineWeight | kPlotStyle |
                           kCurrentViewport | kTransparency
    kHidden                0x8000
    </table>

  */
  static int  mask(OdDbDatabase *pDb, const OdString &layerStateName);

  /** \details
    Removes the specified LayerState from the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
  */
  static void remove(OdDbDatabase *pDb, const OdString &layerStateName);

  /** \details
    Renames the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param oldName [in]  Old name.
    \param newName [in]  New name.
  */
  static void rename(OdDbDatabase *pDb, const OdString &oldName, const OdString &newName);

  /** \details
    This enumeration specifies the results of the import operations for layer
    states.
  */
  enum ImportResult
  {
    kImported,     //Layer state was imported
    kAlreadyExists //Layer state already exists in the database
  };

  /** \details
    Imports the LayerState from the specified file to the specified database.

    \param pDb        [in]  Pointer to the database.
    \param pStreamBuf [in]  Pointer to the StreamBuf object from which the data are to be read.
    \param pName     [out]  Optional pointer to the string where imported layer state name will be stored.

    \returns Import result. Can be one of the following:

    <table>
    Name               Value
    kImported          0
    kAlreadyExists     1
    </table>
  */
  static ImportResult importData(OdDbDatabase *pDb, OdStreamBuf* pStreamBuf, OdString* pName = 0);

  /** \details
    Exports the LayerState from the specified database to the specified file.

    \param pDb [in]  Pointer to the database.
    \param pStreamBuf [in]  Pointer to the StreamBuf object to which the data are to be written.
    \param layerStateName [in]  LayerState name.
  */
  static void exportData(OdDbDatabase *pDb, const OdString &layerStateName, OdStreamBuf* pStreamBuf);

  /** \details
    Sets the description of the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param description [in]  Description.
  */
  static void setDescription(OdDbDatabase *pDb, const OdString &layerStateName, const OdString &description);
  /** \details
    Returns the description of the specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.

    \returns Description of the LayerState.
  */
  static OdString description(OdDbDatabase *pDb, const OdString &layerStateName);

  /** \details
    Returns true if and only if there is viewport data associated with specified LayerState in the specified database.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.

    \returns True if there is viewport data associated with specified
    LayerState; false otherwise.
  */
  static bool hasViewportData(OdDbDatabase *pDb, const OdString &layerStateName);

  /** \details
    Adds more layers to a layer state.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerIds [in]  Array of object Ids of layers to be added to the layer state

    \returns eOk if operation succeeded; error code otherwise.
  */
  static OdResult addLayerStateLayers(const OdString &sName, OdDbObjectIdArray layerIds);

  /** \details
    Removes layers from a layer state.

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerNames [in]  Array of names of layers to be removed from the layer state.

    \returns eOk if operation succeeded; error code otherwise.
  */
  static OdResult removeLayerStateLayers(OdDbDatabase* pDb, const OdString sName, const OdStringArray& layerNames);

  /** \details
    Creates a layer state where specified layers are assigned one state (e.g. ON),
    and all the others are assigned other state (e.g. OFF).

    \param pDb [in]  Pointer to the database.
    \param layerStateName [in]  LayerState name.
    \param layerIds [in]  Array of object Ids of layers to be added to the layer state
    \param mask [in]  created

    \returns eOk if operation succeeded; error code otherwise.
  */
  static OdResult saveGroup(OdDbDatabase *pDb, const OdString &sName,
    const OdDbObjectIdArray& pLayers, int mask = OdDbLayerStateManager::kOn,
    int includedLayersState = OdDbLayerStateManager::kOn, int otherLayersState = OdDbLayerStateManager::kNone);
};

#include "TD_PackPop.h"

#endif //#ifndef DBLAYERSTATEMANAGER_H
