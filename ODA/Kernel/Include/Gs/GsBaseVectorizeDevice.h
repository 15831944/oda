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

#ifndef ODGSBASEVECTORIZEDEVICE_INC
#define ODGSBASEVECTORIZEDEVICE_INC

#include "TD_PackPush.h"
#include "Gs/GsBaseModule.h"
#include "Gs/GsDCRectArray.h"
#include "Gs/GsVectPerformance.h"
#include "Gs/GsModel.h"
#include "GsOverlayDefs.h"
#include "SlotManager.h"
#include "ThreadsCounter.h"

#include "Gs/GsUpdateManager.h"

/** \details
    This template class is a specialization of the OdArray class for ODCOLORREF objects.
*/
typedef OdArray<ODCOLORREF, OdMemoryAllocator<ODCOLORREF> > ODGSPALETTE;

class OdGiPathNode;
class OdGiTransientManager;
class OdGiSectionGeometryManager;

class OdGsBaseVectorizeView;
class OdGsViewImpl;

/** \details
  This template class is a specialization of the OdSmartPtr class for OdGiTransientManager object pointers. 
*/
typedef OdSmartPtr<OdGiTransientManager> OdGiTransientManagerPtr;

/** \details
    This class implements bitmapped GUI display windows.

    \remarks
    OdGsBaseVectorizeDevice objects own, update, and refresh one or more OdGsView objects.
    
    In addition, they responds to operating system notifications,
    such as window resize, forwarded by the graphics system client.

    Corresponding C++ library: Gs

    <group OdGs_Classes>
    \sa
    <link tv_working_with_base_classes_device, Work with Visualize Graphics Devices>
*/
class GS_TOOLKIT_EXPORT OdGsBaseVectorizeDevice : public OdGsDevice
{
  friend class OdGsViewImpl;
  friend class OdGsBaseVectorizeView;

  friend class OdGsBaseModule;
  OdGsBaseModulePtr       m_pModule;
  OdSlotManager           m_slotManager;
protected:
  OdArray<OdGsViewPtr>    m_views;
  ODGSPALETTE             m_logPalette;
  ODCOLORREF              m_Background;
  OdGsDCRect              m_outputRect;
  OdSmartPtr<OdGiContext> m_pUserContext;
  OdRxObjectPtr           m_sectionGeometryMap;
  OdRxObjectPtr           m_sectionManager;
  OdGiTransientManagerPtr m_transientManager;
  OdGsModel::RenderType   m_transientRenderType;

  OdGsUpdateManagerPtr    m_pUpdateManager;

  OdUInt32                m_flags;
  enum
  {
    kFirstFlag              = 1,

    kInvalid                = kFirstFlag << 0,
    kSortRenderTypes        = kFirstFlag << 1,
    kSpatialIndexDisabled   = kFirstFlag << 2,
    kDynamicHighlight       = kFirstFlag << 3,
    kDynamicSubhighlight    = kFirstFlag << 4,
    kSupportBlocks          = kFirstFlag << 5,
    kSupportOverlays        = kFirstFlag << 6,
    kSupportComposition     = kFirstFlag << 7,
    kEnableComposition      = kFirstFlag << 8,
    kMtDisplay              = kFirstFlag << 9,
    kUseSectionGeometryMap  = kFirstFlag << 10,
    kUseVpLtypeScaleMult    = kFirstFlag << 11,
    kUseVpFilterFunction    = kFirstFlag << 12,
    kCullingVolumeEnabled   = kFirstFlag << 13,
    kDisplayOffLayers       = kFirstFlag << 14,
    kSupportInteractiveView = kFirstFlag << 15,
    kHighlightTwoPass       = kFirstFlag << 16,
    kSupportSelectionStyles = kFirstFlag << 17,
    kSuppressHide           = kFirstFlag << 18, //Indicates that GS should not skip hidden entities, device will hide they by himself

    kLastFlag              = kSuppressHide
  };

  struct GsDeviceOverlayData
  {
    OdGsDCRectArray m_invalidRects;
    bool m_bHasInvalidRectsInVports;
    GsDeviceOverlayData() : m_bHasInvalidRectsInVports(false) {}
  };
  class GS_TOOLKIT_EXPORT GsDeviceOverlayDataContainer : public OdGsOverlayDataContainer<GsDeviceOverlayData>
  {
    public:
      void clearInvalidRects(OdUInt32 nOverlays = kGsAllOverlays);
      bool hasInvalidRects(OdUInt32 nOverlays = kGsAllOverlays) const;
      bool mergeInvalidRects(OdGsDCRect* pRect, OdUInt32 nOverlays = kGsAllOverlays) const;
      bool containsInvalidRects(const OdGsViewImpl &view, bool bIfHas = false, OdUInt32 nOverlays = kGsAllOverlays) const;
  };
  GsDeviceOverlayDataContainer m_overlayData;

  void registerOverlay(OdGsViewImpl *pView, const OdGsModel *pModel);
  void unregisterOverlay(OdGsViewImpl *pView, const OdGsModel *pModel);

  virtual void onOverlayActivated(OdGsOverlayId nOverlay);
  virtual void onOverlayDeactivated(OdGsOverlayId nOverlay);
public:
  /** \details
    Returns true in case if device in completely invalid state.
  */
  bool invalid() const { return GETBIT(m_flags, kInvalid); }
  /** \details
    Reset device completely invalid state flag.
    \param bSet [in]  New device invalid state.
  */
  void setInvalid(bool bSet) { SETBIT(m_flags, kInvalid, bSet); }
  /** \details
    Reset all invalidation device state parameters.
    \param bFlag [in]  New device validity state.
  */
  void setValid(bool bFlag);

public:
  /** \details
    Checks whether this device supports partial update (of a device surface, i.e. screen).
    \returns
    true if partial update is supported, false otherwise.
    \remarks
    Must be overridden to return true in child class to inform base implementation that
    child class supports partial update.
  */
  virtual bool supportPartialUpdate() const;

  /** \details
    Returns true in case if device support partial update of separate viewports.
  */
  virtual bool supportPartialScreenUpdate() const;

  /** \details
    Notification method that should be called whenever a view is added to this device.
    \param pView [in]  Pointer to a view that is added.
  */
  void onViewAdded(OdGsView* pView);
public:
  ODRX_DECLARE_MEMBERS(OdGsBaseVectorizeDevice);

  /** \details
    Destructor for the OdGsBaseVectorizeDevice class. Erases all contained view.
  */
  ~OdGsBaseVectorizeDevice();

  /** \details
    Returns the user context object associated with this device.
  */
  OdGiContext* userGiContext() const;
  
  /** \details
    Sets the user context object associated with this device.
    
    \param pUserGiContext   [in]  Pointer to the user context.
  */
  void setUserGiContext(OdGiContext* pUserGiContext);

  /** \details
    Returns smart pointer to a dictionary that stores device properties.
  */
  OdRxDictionaryPtr properties();
  
  /** \details
    Default constructor for the OdGsBaseVectorizeDevice class. Sets the object with default background color, flags and other properties.
    The created default device is invalid.
  */
  OdGsBaseVectorizeDevice();

  /** \details
    Returns the height of this Device object output surface in pixels.
  */
  int height() const;

  /** \details
    Returns the width of this Device object output surface in pixels.
  */
  int width() const;

  /** \details
    Returns horizontal offset of this Device object output surface in pixels.
  */
  int hOffset() const;

  /** \details
    Returns vertical offset of this Device object output surface in pixels.
  */
  int vOffset() const;

  /** \details
    Marks for refresh the GUI window related to the device.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
  */
  void invalidate();
  
  /** \details
    Marks for refresh a specified region of the GUI window related to the device.
    \param screenRect [in]  A rectangle that specifies the window region to be refreshed.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
  */
  void invalidate(const OdGsDCRect &screenRect);

  /** \details
    Marks overlays for refresh.
    \param nOverlays [in]  Number of overlays.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
    \sa
    <link tv_graphSysOverlays, Work with Graphic System Overlays>
  */
  virtual void invalidate(OdUInt32 nOverlays);
  
  /** \details
    Marks the specified screen rectangle of overlays for refresh.
    \param nOverlays [in]  Number of overlays.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
    \sa
    <link tv_graphSysOverlays, Work with Graphic System Overlays>
  */
  virtual void invalidate(OdUInt32 nOverlays, const OdGsDCRect &screenRect);
  
  /** \details
    Marks for refresh a specified region of the GUI window related to the device for an owned view.
    \param pView [in]  Pointer to a view.
    \param screenRect [in]  A rectangle that specifies the window region to be refreshed.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
  */
  void invalidate(const OdGsViewImpl *pView, const OdGsDCRect &screenRect);
  
  /** \details
    Marks for refresh a specified region of the GUI window related to the device for a model.
    \param pModel [in]  Pointer to a model.
    \param screenRect [in]  A rectangle that specifies the window region to be refreshed.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
  */
  void invalidate(const OdGsModel *pModel, const OdGsDCRect &screenRect);

  /** \details
    Marks for refresh a specified path node.
    \param path [in]  Path to a mode that needs to be invalidated.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
  */
  void invalidateRegion(const OdGiPathNode& path);

  const OdGsDCRectArray& invalidRects() const;
  const OdGsDCRectArray* invalidRects(OdGsOverlayId overlayId) const;

  /** \details
    Checks whether this device is valid. Device is considered invalid if it has the kInvalid flag set to true or if one of the owned visible views is invalid.
    \returns
    true if this device is valid, false otherwise.
  */
  bool isValid() const;

  /** \details
    Updates the GUI window (or its region) associated with the device object.
    \param pUpdatedRect         [in] A pointer to the rectangle object that represents the window region to be updated.
    \param bSkipPartialRequest  [in] A partial viewing flag value. If equal to true, partial viewing is disabled during the method call.
    \remarks
    The method affects all OdGsView objects owned by the device object, and so it displays the correct image on the GUI window (or its region).
    If the pointer to a window region (the pUpdatedRect parameter value) is NULL, the whole window is updated.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
  */
  virtual void update(OdGsDCRect* pUpdatedRect);
  
  /** \details
    Updates the display of geometry objects viewed by all valid visible views owned by this device.
  */
  virtual void updateGeometry();
  virtual void updateScreen();

  /** \details
    The handler for the resize event for a GUI window associated with the device object. 
    The method is called whenever the size of the GUI window is changed. 
    \param outputRect [in]  A new size of the output rectangle in device coordinates (pixels).
    \remarks
    This method should not be called if the device object has zero width and (or) height.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
  */
  void onSize(const OdGsDCRect& outputRect);

  /** \details
    Retrieves the device output rectangle.
  */
  const OdGsDCRect& outputRect() const;
  
  /** \details
    Retrieves the device output rectangle. The method fills the passed rectangle object with the data of the device output rectangle and returns it to a calling subroutine.
    \param outputRect [out]  A placeholder for the device's output rectangle object. Size of the rectangle is represented in the device coordinates (pixels).
  */
  void getSize(OdGsDCRect& outputRect) const;

  /** \details
    Notification method that tells the device to realize its palette in the foreground. As a rule this method is called by the client code when the GUI window gets focus. 
  */
  void onRealizeForegroundPalette();

  /** \details
    Notification method that tells the device to realize its palette in the background. As a rule this method is called by the client code when the GUI window gets focus. 
  */
  void onRealizeBackgroundPalette();

  /** \details
    The handler for the display settings change event for the device object. 
    The method is called whenever the operating system screen resolution and (or) color depth are changed. 
    \param bitsPerPixel [in]  A new screen color depth.
    \param xPixels      [in]  A new screen resolution along the X-axis (in pixels).
    \param yPixels      [in]  A new screen resolution along the Y-axis (in pixels).
    \remarks
    This method is called after the display settings are changed.
    \sa
    <EXTLINK https://docs.opendesign.com/tv/tv_working_with_base_classes_device_functionality.html#5">Redrawing Content of a Device</EXTLINK>
  */
  void onDisplayChange(
    int bitsPerPixel, 
    int xPixels, 
    int yPixels);

  
  /** \details
    Creates a new OdGsView object, and associates it with this Device object.
    
    \param pViewInfo [in]  Pointer to the Client View Information for this Device object.
    \param enableLayerVisibilityPerView [in]  Layer visibility per viewport is supported, if and only if true.    
    \returns
    Returns a SmartPointer to the newly created object.
    pViewInfo contains information returned by OdGiViewport::acadWindowId() and OdGiViewportDraw::viewportObjectId()
  */
  OdGsViewPtr createView(
    const OdGsClientViewInfo* pViewInfo = 0, 
    bool enableLayerVisibilityPerView = false);

  /** \details
    Associates the specified Viewport object with this Device object.
    
    \param pView [in]  Pointer to the OdGsView object.
  */
  void addView(OdGsView* pView);

  /** \details
    Inserts the specified Viewport object to the specified position in this Device object.

    \param viewIndex [in]  View index.
    \param pView [in]  Pointer to the OdGsView object.
  */
  void insertView(int viewIndex, OdGsView* pView);

  /** \details
    Erases the specified View object.
    \param pView [in]  Pointer to the OdGsView object.

    \returns
    Returns true if and only if successful.
  */
  bool eraseView(OdGsView* pView);

  /** \details
    Returns the number of views associated with this Device object.
  */
  int numViews() const;

  /** \details
    Returns the specified OdGsView object associated with this Device object.
    \param viewIndex [in]  View index.
  */
  OdGsView* viewAt(int viewIndex);
  
  /** \details
    Returns the specified OdGsView object associated with this Device object.
    \param viewIndex [in]  View index.
  */
  const OdGsView* viewAt(int viewIndex) const;
  
  /** \details
    Returns the implementation OdGsViewImpl object associated with this Device object.
    \param viewIndex [in]  View index.
  */
  OdGsViewImpl* viewImplAt(int viewIndex);
  
  /** \details
    Returns the implementation OdGsViewImpl object associated with this Device object.
    \param viewIndex [in]  View index.
  */
  const OdGsViewImpl* viewImplAt(int viewIndex) const;

  /** \details
    Erases the specified View object.

    \param viewIndex [in]  View index.
    \returns
    Returns true if and only if successful.
  */
  bool eraseView(int viewIndex);

  /** \details
    Erases all views associated with this Device object.
  */
  void eraseAllViews();

  /** \details
    Sets the Background Color of the GUI window of this Device object.
    \param backgroundColor [in]  Background color.
  */
  bool setBackgroundColor(ODCOLORREF backgroundColor);

  /** \details
    Returns the Background Color of the GUI window of this Device object.
  */
  ODCOLORREF getBackgroundColor();

  /** \details
    Sets the logical palette to be used by this Device object.
    
    \param logicalPalette [in]  Logical palette.
    \param numColors [in]  Number of colors in palette.
    
    \remarks
    The logical palette is used with calls to OdGiSubEntityTraits::setColor().
  */
  void setLogicalPalette(const ODCOLORREF* logicalPalette, int numColors);

  /** \details
    Returns image of current device buffer. Currently not implemented.

    \param pImage [out]  Pointer for image to return.
    \param region [in]  Rectangle specifies region to capture.
    \remarks
    Implementation can throw eNotImplementedYet exception if current vectorization module doesn't support snap shots.
  */
  void getSnapShot(OdGiRasterImagePtr &pImage, const OdGsDCRect &region);

  /** \details
    Returns the specified color in the logical palette of this Device object.
    \param colorIndex [in]  Color index.
  */
  ODCOLORREF getColor(OdUInt16 colorIndex) const;

  /** \details
    Returns background color in the logical palette of this Device object.
  */
  ODCOLORREF getPaletteBackground() const;

  /** \details
    Returns the logical palette of this Device object.
  */
  const ODCOLORREF* getPalette() const;

  /** \details
    Returns the logical palette of this Device object.
    \param numColors [out]  Returns number of colors inside logical palette.
  */
  const ODCOLORREF* getLogicalPalette(int &numColors) const;

  /** \details
    Creates a new OdGsModel object, and associates it with this Device object.
    
    \returns
    Returns a SmartPointer to the newly created object.
  */
  OdGsModelPtr createModel();
  
  /** \details
    Checks compatibility between specified OdGsModel and this Device object.

    \param pModel [in]  Model object pointer to check compatibility.
    \returns
    true if the provided model is compatible with this device, false otherwise.
    \remarks
    Use this method to check compatibility of exist Model object and newly created device
    to be sure the exist Model object could be used with this Device object.
    If Device object use overriden Model object
    it should return false if input Model object doesn't overriden.
    Typical way to check compatibility is using of RTTI.
    If your metafiles cache doesn't support per-device sharing override and return false.
  */
  bool isModelCompatible(OdGsModel* pModel) const;

  /** \details
    Returns pointer to the root view associated with this device.
  */
  const OdGsView* rootView() const;

  // renderTypes support
  bool sortRenderTypes() const;
  void setSortRenderTypes(bool flag);
  virtual OdUInt32 renderTypeWeight(OdGsModel::RenderType renderType) const;
  OdGsModel::RenderType transientRenderType() const;
  void setTransientRenderType(OdGsModel::RenderType renderType);
  // overlays support
  
  /** \details
    Checks whether overlays are supported by this device.
    \returns
    true if overlays are supported by this device, false otherwise.
  */
  bool supportOverlays() const;
  
  /** \details
    Specifies whether graphics system overlays are supported.
    \returns
    true if overlays are supported by this device, false otherwise.
  */
  void setSupportOverlays(bool bFlag);
  
  /** \details
    Returns the overlay identifier (from the OdGsOverlayId enumeration) of the specified render type.
    \param renderType [in]  Render type.
  */
  virtual OdGsOverlayId renderTypeOverlay(OdGsModel::RenderType renderType) const;
  
  /** \details
    Returns the overlay identifier (from the OdGsOverlayId enumeration) of the specified model.
    \param pModel [in]  Pointer to a model.
  */
  OdGsOverlayId gsModelOverlay(const OdGsModel *pModel) const;
  
  /** \details
    Checks whether the specified overlay needs to be updated.
    \param overlayId [in]  Overlay ID.
    \returns
    one of the following values:
    * 0 - overlay is completely valid and update is not needed
    * 1 - overlay is partially invalid and update is required
    * 2 - overlay is completely invalid and update is required
  */
  int isOverlayRequireUpdate(OdGsOverlayId overlayId) const;

  /** \details
    Checks whether spatial indexes are disable.
    \returns
    true if spatial indexes are disabled, false otherwise.
  */
  bool isSpatialIndexDisabled() const;
  
  /** \details
    Disables or enables spatial indexes.
    \param bSet [in]  Flag that specifies whether to disable spatial indexes. True to disable, false to enable.
  */
  void disableSpatialIndex(bool bSet);

  /** \details
    Checks whether culling volume is enabled.
    \returns
    true if culling volume is enabled, false otherwise.
  */
  bool isCullingVolumeEnabled() const;
  
  /** \details
    Enables or disables culling volume usage.
    \param bSet [in]  Flag that specifies whether to enable culling volume.
  */
  void enableCullingVolume(bool bSet);

  /** \details
    Checks whether this device supports secondary highlighting rendering pass.
    \returns
    true if secondary highlighting rendering pass supported, false otherwise.
  */
  bool supportHighlightTwoPass() const;

  /** \details
    Enables or disables secondary highlighting rendering pass support.
    \param bOn [in]  Flag that specifies whether to enable secondary highlighting rendering pass support.
  */
  void setSupportHighlightTwoPass(bool bOn);

  /** \details
    Checks whether this device displays image in the multi-threaded mode.
    \returns
    true if this device displays image in the multi-threaded mode, false otherwise.
  */
  bool isMtDisplay() const;

  /** \details
    Checks whether this device supports dynamic entities highlight.
    \returns
    true if this device supports dynamic entities highlight, false otherwise.
  */
  bool supportDynamicHighlight() const;
  
  /** \details
    Checks whether this device supports dynamic sub-entities highlight.
    \returns
    true if this device supports dynamic sub-entities highlight, false otherwise.
  */
  bool supportDynamicSubhighlight() const;

  /** \details
    Checks whether this device supports multiple selection (highlighting) styles.
    \returns
    true if this device supports multiple selection (highlighting) styles.
  */
  bool supportSelectionStyles() const;

  /** \details
    Checks whether this device supports blocks.
    \returns
    true if this device supports dynamic sub-entities highlight, false otherwise.
  */
  bool supportBlocks() const;

  /** \details
    Checks whether this device supports vectorization modules compositions.
    \returns
    true if this device supports composition, false otherwise.
    \sa
    <link WinComposite, Viewport-Specific Render Modes using WinComposite Vectorization>
  */
  bool supportComposition() const;

  /** \details
    Checks whether vectorization modules compositions is enabled for this device.
    \returns
    true if vectorization modules compositions is enabled for this device, false otherwise.
    \sa
    <link WinComposite, Viewport-Specific Render Modes using WinComposite Vectorization>
  */
  bool compositionEnabled() const;
  
  /** \details
    Enables or disables vectorization modules compositions for this device.
    \param bSet [in]  Flag that specifies whether to enable vectorization modules compositions.
    \sa
    <link WinComposite, Viewport-Specific Render Modes using WinComposite Vectorization>
  */
  void enableComposition(bool bSet);

  /** \details
    Checks whether off-layers displaying enabled.
    \returns
    true if off-layers displaying enabled, false otherwise.
  */
  bool isDisplayOffLayersEnabled() const;

  /** \details
    Enables or disables displaying of off-layers.
    \param bSet [in]  Flag that specifies whether to enable displaying of off-layers.
  */
  void enableDisplayOffLayers(bool bSet);

  /** \details
    Checks whether the passed viewport identifier is valid.
    
    \param acgiId [in]  Viewport ID.
    \returns
    true if the specified viewport identifier is valid, false otherwise.
  */
  bool isValidViewportId(const OdUInt32 acgiId) const
  {
    return m_slotManager.contains(acgiId);
  }

  bool useVpLtypeScaleMult() const { return GETBIT(m_flags, kUseVpLtypeScaleMult); }
  bool useVpFilterFunction() const { return GETBIT(m_flags, kUseVpFilterFunction); }

  /** \details
    Retrieves pointer to the section geometry manager.
  */
  virtual OdGiSectionGeometryManager *getSectionGeometryManager();
  
  /** \details
    Retrieves smart pointer to the section geometry map.
  */
  OdRxObjectPtr getSectionGeometryMap();
  
  /** \details
    Clears the section geometry map.
  */
  void clearSectionGeometryMap();
  OdRxObject* mtServices() { return m_mtServices; }

  /** \details
    Retrieves a pointer to the transient manager.
  */
  OdGiTransientManager *transientManager();
  
  /** \details
    Sets a transient drawable manager for this device.
    \param pManager [in]  Pointer to a transient manager.
  */
  void setTransientManager(OdGiTransientManager *pManager);

  /** \details
    Checks whether this device supports saving its state.
    \returns
    true if this device supports state saving, false otherwise.
  */
  virtual bool isSupportDeviceStateSaving() const;
  
  /** \details
    Saves device data to the specified filer.
    \param pFiler [in]  Pointer to a filer where to save device state.
    \returns
    true if the state of this device is saved successfully, false otherwise.
  */
  bool saveDeviceState(OdGsFiler *pFiler) const;
  
  /** \details
    Loads device data from the specified filer.
    \param pFiler [in]  Pointer to a filer from which to read device state.
    \returns
    true if the state of this device is loaded successfully, false otherwise.
  */
  bool loadDeviceState(OdGsFiler *pFiler);
  
  /** \details
    Saves data of a client device to the specified filer.
    \param pFiler [in]  Pointer to a filer where to save device state.
    \returns
    true if the state of client device is saved successfully, false otherwise.
  */
  virtual bool saveClientDeviceState(OdGsFiler *pFiler) const;
  
  /** \details
    Loads data of a client device from the specified filer.
    \param pFiler [in]  Pointer to a filer from which to read device state.
    \returns
    true if the state of client device is loaded successfully, false otherwise.
  */
  virtual bool loadClientDeviceState(OdGsFiler *pFiler);
  
  virtual bool saveLinkedDeviceState(OdGsFiler *pFiler) const;
  virtual bool loadLinkedDeviceState(OdGsFiler *pFiler);

  /** \details
    Checks whether this device supports interactivity mode for owned GS views.
    \returns
    true if this device supports interactivity mode for its views, false otherwise.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  bool supportInteractiveViewMode() const { return GETBIT( m_flags, kSupportInteractiveView ); }
  
  /** \details
    Switches on or off the interactivity mode for owned GS views.
    \param bSet [in]  Flag that specifies whether to enable interactivity mode.
    \sa
    <link gs_views_interactivity_mode, Work with Interactivity Mode for Views>
  */
  void setSupportInteractiveViewMode( bool bSet ) { SETBIT( m_flags, kSupportInteractiveView, bSet ); }

  /** \details
    Retrieves smart pointer to an update manager for this device.
    \param createIfNotExist [in]  Flag that specifies whether to create a new update manager if there is no update for this device.
  */
  OdGsUpdateManagerPtr getUpdateManager( bool createIfNotExist = false );
  
  /** \details
    Sets an update manager for this device.
    \param pManager [in]  Smart pointer to an update manager for this device.
  */
  void setUpdateManager( OdGsUpdateManagerPtr pManager );
  
  /** \details
    Retrieves a pointer to an update manager for this device.
  */
  OdGsUpdateManager* updateManager() { return m_pUpdateManager.get(); }

  /** \details
    Indicates that GS should not skip hidden entities.
    \returns Returns true if device will hide entities by himself, otherwise returns false.
  */
  bool isSuppressHide() const { return GETBIT( m_flags, kSuppressHide ); }

protected:
  OdRxObjectPtr m_pDeviceRef;
public:
  OdRxObjectPtr deviceSelfReference() { return m_pDeviceRef; }

protected:
  friend class DisplayScheduler;
  virtual bool supportParallelDisplay() const { return false; }
  void initMtServices();
  void viewUpdateScreen(OdGsBaseVectorizeView* pView, bool bUpdate);
  void collectInvalidRectsFromVports(OdGsOverlayId nOverlay, GsDeviceOverlayData& d) const;
  void getInvalidRectsFromVports(OdUInt32 nOverlays = kGsAllOverlays);

public:
  GsVectPerformanceData m_vectPerfData;
private:
  OdRxObjectPtr m_mtServices;
  OdRefCounter m_vectThreadIndex;
  OdMutexPtr m_mtInvalidate;
  OdMutexPtr m_mtSectionGeometryMap;
  mutable OdMutex m_mtInvalidRects;
};

inline bool OdGsBaseVectorizeDevice::sortRenderTypes() const
{
  return GETBIT(m_flags, kSortRenderTypes);
}

inline void OdGsBaseVectorizeDevice::setSortRenderTypes(bool bFlag)
{
  SETBIT(m_flags, kSortRenderTypes, bFlag);
}

inline bool OdGsBaseVectorizeDevice::supportOverlays() const
{
  return GETBIT(m_flags, kSupportOverlays);
}

inline void OdGsBaseVectorizeDevice::setSupportOverlays(bool bFlag)
{
  SETBIT(m_flags, kSupportOverlays, bFlag);
}

inline bool OdGsBaseVectorizeDevice::isSpatialIndexDisabled() const
{
  return GETBIT(m_flags, kSpatialIndexDisabled);
}

inline void OdGsBaseVectorizeDevice::disableSpatialIndex(bool bSet)
{
  SETBIT(m_flags, kSpatialIndexDisabled, bSet);
}

inline bool OdGsBaseVectorizeDevice::isCullingVolumeEnabled() const
{
  return GETBIT(m_flags, kCullingVolumeEnabled);
}

inline void OdGsBaseVectorizeDevice::enableCullingVolume(bool bSet)
{
  SETBIT(m_flags, kCullingVolumeEnabled, bSet);
}

inline bool OdGsBaseVectorizeDevice::supportHighlightTwoPass() const
{
  return GETBIT(m_flags, kHighlightTwoPass);
}

inline void OdGsBaseVectorizeDevice::setSupportHighlightTwoPass(bool bOn)
{
  SETBIT(m_flags, kHighlightTwoPass, bOn);
}

inline bool OdGsBaseVectorizeDevice::isMtDisplay() const
{
  return GETBIT(m_flags, kMtDisplay);
}

inline bool OdGsBaseVectorizeDevice::supportDynamicHighlight() const
{
  return GETBIT(m_flags, kDynamicHighlight);
}

inline bool OdGsBaseVectorizeDevice::supportDynamicSubhighlight() const
{
  return GETBIT(m_flags, kDynamicSubhighlight);
}

inline bool OdGsBaseVectorizeDevice::supportSelectionStyles() const
{
  return GETBIT(m_flags, kSupportSelectionStyles);
}

inline bool OdGsBaseVectorizeDevice::supportBlocks() const
{
  return GETBIT(m_flags, kSupportBlocks);
}

inline bool OdGsBaseVectorizeDevice::supportComposition() const
{
  return GETBIT(m_flags, kSupportComposition);
}

inline bool OdGsBaseVectorizeDevice::compositionEnabled() const
{
  return GETBIT(m_flags, kEnableComposition);
}

inline void OdGsBaseVectorizeDevice::enableComposition(bool bSet)
{
  SETBIT(m_flags, kEnableComposition, bSet);
}

inline bool OdGsBaseVectorizeDevice::isDisplayOffLayersEnabled() const
{
  return GETBIT(m_flags, kDisplayOffLayers);
}

inline void OdGsBaseVectorizeDevice::enableDisplayOffLayers(bool bSet)
{
  SETBIT(m_flags, kDisplayOffLayers, bSet);
}

inline OdGsModel::RenderType OdGsBaseVectorizeDevice::transientRenderType() const
{
  return m_transientRenderType;
}

inline void OdGsBaseVectorizeDevice::setTransientRenderType(OdGsModel::RenderType renderType)
{
  m_transientRenderType = renderType;
}

inline const
OdGsDCRect& OdGsBaseVectorizeDevice::outputRect() const
{
  return m_outputRect;
}

inline
void OdGsBaseVectorizeDevice::getSize(OdGsDCRect& outputRect) const
{
  outputRect = this->outputRect();
}

inline
ODCOLORREF OdGsBaseVectorizeDevice::getColor(OdUInt16 colorIndex) const
{
  return m_logPalette[colorIndex];
}

inline
const ODCOLORREF* OdGsBaseVectorizeDevice::getPalette() const
{
  return m_logPalette.getPtr();
}

inline
const ODCOLORREF* OdGsBaseVectorizeDevice::getLogicalPalette(int &numColors) const
{
  numColors = (int)m_logPalette.size();
  return m_logPalette.getPtr();
}

#include "TD_PackPop.h"

#endif // ODGSBASEVECTORIZEDEVICE_INC
