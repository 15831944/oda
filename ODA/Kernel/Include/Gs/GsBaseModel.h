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

#ifndef __ODGSMODELIMPL_H__
#define __ODGSMODELIMPL_H__

#define OD_MATERIAL_CACHE_SUPPORTED

#include "TD_PackPush.h"

#define STL_USING_MAP
#include "OdaSTL.h"
#include "OdList.h"
#include "UInt8Array.h"
#include "UInt32Array.h"
#include "DbStubPtrArray.h"
#include "StaticRxObject.h"
#include "OdVector.h"

#include "Gi/GiDrawable.h"
#include "Gi/GiCommonDraw.h"
#include "Gi/GiPathNode.h"

#include "Gs/GsExport.h"
#include "Gs/GsModel.h"
#include "Gs/GsViewProps.h"
#include "Gs/GsExtAccum.h"
#include "Gs/GsMaterialCache.h"
#include "Gs/GsViewLocalId.h"
#include "Gs/GsHighlightData.h"
#include "Gs/GsBaseModelReactor.h"

#define CACHE_STATISTICS

class OdGsLayerNode;
class OdGsBaseVectorizeDevice;
class OdGsBaseVectorizer;
class OdGsBaseModule;

class OdGsBaseModelImpl;
class OdGsEntityNode;
class OdGsContainerNode;
class OdGsLightNode;

class OdGsViewImpl;
class OdGsCacheRedirectionManager;

enum ENodeType
{
  kBlockNode,
  kContainerNode,
  kLayerNode,
  kEntityNode,
  kMaterialNode,
  kLastNodeType, //number of types enumerated above
  kLightNode,
  kBlockReferenceNode,
  kMInsertBlockNode,
};

/** \details
    This class is the base class for custom GsBaseModel objects which coordinate custom caching support 
    for client vectorization applications.

    \remarks
    Clients should derive their custom model classes from this class.

    Corresponding C++ library: TD_Gs
    
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseModel : public OdGsModel
{
protected:
  OdGiOpenDrawableFn m_openDrawableFn;
  OdGsNode*          m_aNodes[kLastNodeType];

  friend class OdGsNode;
  friend class OdGsViewImpl;
  friend class OdGsBaseVectorizer;
  friend class OdGsBaseVectorizeDevice;

  /** \details
    Adds the specified Node object to this BaseModel object.
    \param pNode [in]  Pointer to the Node to be added.  
  */
  virtual void addNode(OdGsNode* pNode);

public:
  virtual void invalidateEntRect(OdGiDrawable* pDrawable, OdGiDrawable* pParent, bool bForceIfNoExtents = false);
  virtual void invalidateEntRect(OdGsEntityNode* pNode, OdGsContainerNode* pParent, bool bForceIfNoExtents);

#ifdef BS_CHANGES_ENABLED // @@@TODO: Feature is obsolete and marked to be removed.
  /** \details
    Detach OdGsNodes from underlying objects.
    Returned object keeps mapping drawable->OdGsNode and the nodes preventing them from destruction.
    \param db [in]  Database drawable
  */
  virtual OdRxObjectPtr saveAndDetach(OdGiDrawable& db);

  /** \details
    This is the operation opposite to saveAndDetach().
    OdGsNodes are set back to the drawables.
    \param db [in]  Database drawable
    \param pFrom [in]  The nodes keeper returned by saveAndDetach().
  */
  virtual void restoreAndAttach(OdGiDrawable& db, OdRxObject* pFrom);
#endif // BS_CHANGES_ENABLED

#if defined(CACHE_STATISTICS)
  OdRefCounter m_nMfCached; // For safety and perhaps future extension
  OdRefCounter m_nMfReused; // Can be incremented from worker threads
#endif//CACHE_STATISTICS
protected:
  enum GsModelFlags
  {
    kCheckFaded      = (1 << 0),
    kEnableLinetypes = (1 << 1),
    kModelSelectable = (1 << 2),
    kLightsInBlocks  = (1 << 3),
    kSkipViewClip    = (1 << 4),
    kViewExtCalcs    = (1 << 5),
    kSkipViewSection = (1 << 6),
    kIdentityXform   = (1 << 7),
    kVectorizing     = (1 << 8),
    kKeepSelStyles   = (1 << 9)
#ifdef BS_CHANGES_ENABLED // @@@TODO: Feature is obsolete and marked to be removed.
  , kDisableAddNode  = (1 << 10)
#endif // BS_CHANGES_ENABLED
  };
  OdUInt32 m_gsModelFlags;
public:
  bool checkFaded() const { return GETBIT(m_gsModelFlags, kCheckFaded); }
  void setCheckFaded(bool bOn) { SETBIT(m_gsModelFlags, kCheckFaded, bOn); }
  void setVectorizing() { SETBIT_1(m_gsModelFlags, kVectorizing); }
  void resetVectorizing() { SETBIT_0(m_gsModelFlags, kVectorizing); }
#ifdef ODA_DIAGNOSTICS
  OdRefCounter m_nNodes; //number of OdGsNodes allocated within the model (not necessary in 
                //m_pNodes list _directly_, can be owned by insert/block nodes).
#endif

  typedef std::pair<OdGsViewImpl*, OdUInt32> ViewRef;
  OdArray<ViewRef, OdMemoryAllocator<ViewRef> > m_views;
  typedef std::pair<const OdGsBaseModule*, OdUInt32> ModuleRef;
  OdArray<ModuleRef, OdMemoryAllocator<ModuleRef> > m_modules;
  const ViewPropsArray& getViewProps() const { return m_viewProps; }
  OdGsViewImpl *refView() const;
  OdGsBaseVectorizeDevice *refDevice() const;
  OdUInt32 refModulesCount() const { return m_modules.size(); }
  virtual void attachLocalViewportId(OdGsViewImpl* pView, OdGsViewImpl* pFrom = NULL);
  virtual OdUInt32 getLocalViewportId(OdGsViewImpl* pView);
  virtual OdUInt32 getMaxLocalViewportId() const;

  /** \details
    For internal needs. Do not use!
  */
  bool invalidVp(unsigned int viewportId) const
  {
    if(m_viewProps.size()>viewportId)
      return m_viewProps[viewportId].isInvalid();
    return true;
  }

  OdGsBaseModelImpl* impl() { return m_impl; }
  void invalidateRegenDrawBlocks(OdGsViewImpl& view, const OdDbStub* layoutId);
  void invalidateSectionableBlocks();

  struct SectioningSettings
  {
    bool             m_bEnabled;
    bool             m_bTopSet;
    bool             m_bBottomSet;
    bool             m_bVisualStyle;
    OdGePoint3dArray m_points;
    OdGeVector3d     m_upVector;
    double           m_dTop;
    double           m_dBottom;
    OdDbStub        *m_visualStyle;
    SectioningSettings()
      : m_bEnabled(false), m_bTopSet(false), m_bBottomSet(false), m_bVisualStyle(false)
      , m_dTop(0.0), m_dBottom(0.0), m_visualStyle(NULL) { }
    bool isEnabled() const { return m_bEnabled && !m_points.isEmpty(); }
  };

protected:
#ifdef BS_CHANGES_ENABLED // @@@TODO: Feature is obsolete and marked to be removed.
  bool disableAddNode() const { return GETBIT(m_gsModelFlags, kDisableAddNode); }
#endif // BS_CHANGES_ENABLED

  void addViewRef(OdGsViewImpl* pView);
  void removeViewRef(OdGsViewImpl* pView);

  /** \details
    For internal needs. Do not use!
  */
  void setInvalidVp(unsigned int viewportId, bool val)
  {
    if(m_viewProps.size()>viewportId)
      m_viewProps[viewportId].setInvalid(val);
  }
  virtual void clearChangedLayersList();

  bool supportSelectionStyles() const { return GETBIT(m_gsModelFlags, kKeepSelStyles); }
  void setSelectionStyleRef(const OdGsNode* pNode, OdUInt32 nStyle);
  OdUInt32 selectionStyleRef(const OdGsNode* pNode) const;
  void extendSelStyleFlag(OdGsViewImpl* pView);
  void rescanSelStyleFlag();

protected:

  OdArray<ViewProps>           m_viewProps;
  OdUInt32                     m_nLayers;
  OdGsLayerNode*               m_pFirstChangedLayer;

  OdGsView::RenderMode         m_renderModeOverride;
  OdGeMatrix3d                 m_xForm;
  OdDbStub*                    m_modelBackground;
  OdDbStub*                    m_modelVisualStyle;
  OdGiDrawablePtr              m_pTransVisualStyle;

  RenderType                   m_renderType;
  // Reserved pointer for internal use (binary incompatible changes), do not delete.
  OdRxObjectPtr                m_pGsReserved;

  // Material cache support
  OdGsMaterialCachePtr         m_pMaterialCache;

  OdGsBaseModelImpl*           m_impl;

  SectioningSettings           m_sectioning;

  OdGsBaseModelLocalIdsPtr     m_pViewIds;
  OdGsCacheRedirectionManager *m_pRedirectionMgr;

  OdVector<OdGsModelReactor*, OdMemoryAllocator<OdGsModelReactor*> > m_modelReactors;
  OdVector<OdGsBaseModelReactor*, OdMemoryAllocator<OdGsBaseModelReactor*> > m_baseModelReactors;

  typedef std::map<const OdGsNode*, OdUInt32> SelectionStyleRefMap;
  SelectionStyleRefMap         m_selStyleRefs;

protected:

  OdGsBaseModel();
  ~OdGsBaseModel();

public:
  /** \details
    Returns the OdGsNode object associated with the specified Drawable.
    
    \param pDrawable [in]  Pointer to the Drawable object.
    \remarks
    If no Node object is associated with the Drawable object, one is created.
  */
  virtual OdGsNode* gsNode(OdGiDrawable* pDrawable);

  /** \details
    Removes the associations between the specified Node object and this BaseModel object.
    \param pNode [in]  Pointer to the Node object. 
  */
  virtual void detach(OdGsNode* pNode);

  /** \details
    Removes all Node objects from this BaseModel object.
  */
  virtual void detachAll();

  void detachAllFromDb(const OdDbBaseDatabase* pDb);

  /** \details
    Opens the specified object.
    \param objectID [in]  Object ID of the Drawable object to be opened.
    
    \returns
    Returns a SmartPointer to the opened object.
  */
  OdGiDrawablePtr open(OdDbStub* objectId);

  /** \details
    Sets OdGsCache object in specified Drawable object.
    \param pDrawable [in]  Drawable object pointer.
    \param pNode [in]  OdGsCache object to set.
  */
  void setDrawableGsNode(OdGiDrawable *pDrawable, OdGsCache *pNode);
  /** \details
    Gets OdGsCache object from specified Drawable object.
    \param pDrawable [in]  Drawable object pointer.
  */
  OdGsCache *drawableGsNode(const OdGiDrawable *pDrawable);

  void setOpenDrawableFn(OdGiOpenDrawableFn openDrawableFn);
  OdGiOpenDrawableFn openDrawableFn() const;

  enum AdditionMode
  {
    kAddDrawable = 0, // New drawable added
    kUneraseDrawable  // Exist drawable unerased
  };

  void onAdded(OdGiDrawable* pAdded, OdGiDrawable* pParent);
  void onAdded(OdGiDrawable* pAdded, OdDbStub* parentID);
  virtual void onAddedImpl(OdGiDrawable* pAdded, OdGiDrawable* pParent, AdditionMode additionMode = kAddDrawable);

  void onModified(OdGiDrawable* pModified, OdGiDrawable* pParent);
  void onModified(OdGiDrawable* pModified, OdDbStub* parentID);
  virtual void onModifiedImpl(OdGiDrawable* pModified, OdGiDrawable* pParent);

  virtual void onModifiedGraphics(const OdGiDrawable* pModified, OdDbStub* parentID) ODRX_OVERRIDE;

  void onErased(OdGiDrawable* pErased, OdGiDrawable* pParent);
  void onErased(OdGiDrawable* pErased, OdDbStub* parentID);
  virtual void onErasedImpl(OdGiDrawable* pErased, OdGiDrawable* pParent);

  void onUnerased(OdGiDrawable* pUnerased, OdGiDrawable* pParent);
  void onUnerased(OdGiDrawable* pUnerased, OdDbStub* parentID);

  void invalidate(InvalidationHint hint);
  void invalidate(OdGsView* pView);
  void invalidateVisible( OdGsDevice* pDevice );
  /** \param view [in]  BaseVectorizeView object for which data are to be invalidated.
    \param mask [in]  Mask.
  */
  void invalidate(OdGsViewImpl& view, OdUInt32 mask);

  /** \details
    Returns the view properties that have changed for the specified Viewport
    
    \param viewportId [in]  Viewport ID. 
  */
  OdUInt32 viewChanges(OdUInt32 viewportId) const;

  /** \details
    Returns the size of the ViewProperties array for this BaseModel object.
  */
  OdUInt32 numViewProps() const;
  
  /** \details
    Returns a reference to the the ViewProp structure for the specified Viewport.
    
    \param viewportId [in]  Viewport ID. 
  */
  const ViewProps& viewProps(OdUInt32 viewportId) const;

  /** \details
    Updates the ViewProps structure for the specified Viewport.
    \param viewport [in]  Viewport.
  */
  virtual void updateViewProps(const OdGsViewImpl& viewport);

  /** \details
    Checks whether view properties are valid for the specified viewport.
    \param viewport [in]  Viewport.
  */
  virtual bool isViewPropsValid(OdUInt32 viewportId) const;

  /** \details
    Returns view pointer by specified Viewport ID.

    \param viewportId [in]  Viewport ID.
  */
  const OdGsViewImpl *viewById(OdUInt32 viewportId) const;

  /** \details
    Checks whether view properties are valid for the specified viewport.
    \param layerId [in]  Pointer to the persistent layer identifier.
    \param pView [in]  Pointer to the view for .
  */
  virtual OdGsLayerNode* gsLayerNode(OdDbStub* layerId, const OdGsBaseVectorizer* pView);

  /** \details
    Notifies layer changes to the specified device.
    \param device [in]  Vectorization device.
  */
  virtual void propagateLayerChanges(OdGsBaseVectorizeDevice& device);

  /** \details
    Sets transformation matrix.
    \param xForm [in]  Transformation matrix.
  */
  void setTransform(const OdGeMatrix3d& xForm);

  /** \details
    Returns transformation matrix.
  */
  OdGeMatrix3d transform() const;

  /** \details
    Checks whether this object stores an identity matrix (default matrix) for transformations.
    \returns
    true if this object has an identity matrix, false otherwise. 
  */
  bool isIdentityTransform() const;

  /** \details
    Transforms extents of the passed object by the transformation matrix of this object and returns the transformed extents.
    \param pExts [in/out]  Pointer to the extents to transform.
  */
  OdGeExtents3d &transformExtents(OdGeExtents3d &pExts) const;

  /** \details
    Returns the render mode override.
    \sa
    <exref target="https://docs.opendesign.com/tv/tv_working_with_base_classes_view_functionality.html#7">Managing Render Modes</exref>
  */
  OdGsView::RenderMode renderModeOverride() const;

  /** \details
    Sets the render mode override.
    \param mode [in]  Render mode override.
    \sa
    <exref target="https://docs.opendesign.com/tv/tv_working_with_base_classes_view_functionality.html#7">Managing Render Modes</exref>
  */
  void setRenderModeOverride(OdGsView::RenderMode mode = OdGsView::kNone);

  /** \details
    Highlights or unhighlights the passed path.
    \param path   [in] A path to subitems that should be highlighted.
    \param bDoIt  [in] A flag value that determines whether highlighting is enabled (if true) or disabled (if false). This flag can switch the highlighting on or off.
    \param nStyle [in] Selection style index.
    \param pView  [in] Pointer to the view in which to highlight or unhighlight the path.
    \sa
    <link tv_working_with_features_highlighting, Work with Highlighting>
  */
  void highlight(const OdGiPathNode& path, bool bDoIt = true, OdUInt32 nStyle = 0, const OdGsView* pView = NULL);

  /** \details
    Highlights or unhighlights the passed path.
    \param path     [in] A path to subitems that should be highlighted.
    \param pMarkers [in] Pointer to an array of markers.
    \param nMarkers [in] Number of markers in the array.
    \param bDoIt    [in] A flag value that determines whether highlighting is enabled (if true) or disabled (if false). This flag can switch the highlighting on or off.
    \param nStyle   [in] Selection style index.
    \param pView    [in] Pointer to the view in which to highlight or unhighlight the path.
    \sa
    <link tv_working_with_features_highlighting, Work with Highlighting>
  */
  void highlight(const OdGiPathNode& path, const OdGsMarker* pMarkers, OdUInt32 nMarkers, bool bDoIt = true,
                 OdUInt32 nStyle = 0, const OdGsView* pView = NULL);
  //DOM-IGNORE-BEGIN
  virtual void highlightImpl(const OdGiPathNode& path, const OdGsMarker* pMarkers, OdUInt32 nMarkers, bool bDoIt = true,
                             OdUInt32 nStyle = 0, const OdGsView* pView = NULL);
  //DOM-IGNORE-END
  /** \details
    Hides the specified entity in this Model object.
    
    \param path [in]  Path to the entity for hide.
    \param bDoIt [in]  Flag that indicates whether hiding is enabled (true value) or disabled (false value).
    \param bSelectHidden [in] Flag that indicates whether hidden object is selectable or not.
    \param pView [in]  Pointer to the view where the specified entity should be hidden.
  */
  void hide(const OdGiPathNode& path, bool bDoIt = true, bool bSelectHidden = false, const OdGsView* pView = NULL);
  /** \details
    Hides the specified subentities of entity in this Model object..
    
    \param path [in]  Path to the entity for hide.
    \param pMarkers [in] Array of subentity markers.
    \param nMarkers [in] Count of subentity markers.
    \param bDoIt [in]  Flag that indicates whether hiding is enabled (true value) or disabled (false value).
    \param bSelectHidden [in] Flag that indicates whether hidden object is selectable or not.
    \param pView [in]  Pointer to the view where the specified entity should be hidden.
  */
  void hide(const OdGiPathNode& path, const OdGsMarker* pMarkers, OdUInt32 nMarkers, bool bDoIt = true,
            bool bSelectHidden = false, const OdGsView* pView = NULL);
  /** \details
    Hides the specified subentities of entity in this Model object..
    
    \param path [in]  Path to the entity for hide.
    \param pMarkers [in] Array of subentity markers.
    \param nMarkers [in] Count of subentity markers.
    \param bDoIt [in]  Flag that indicates whether hiding is enabled (true value) or disabled (false value).
    \param bSelectHidden [in] Flag that indicates whether hidden object is selectable or not.
    \param pView [in]  Pointer to the view where the specified entity should be hidden.
  */
  virtual void hideImpl(const OdGiPathNode& path, const OdGsMarker* pMarkers, OdUInt32 nMarkers, bool bDoIt = true,
                        bool bSelectHidden = false, const OdGsView* pView = NULL);

  // Material cache support
  
  /** \details
    Retrieves material cache.
  */
  OdGsMaterialCache* materialCache() { return m_pMaterialCache.get(); }

  /** \details
    Sets the render type.
    
    \param renderType [in]  Render mode.
    \sa
    <exref target="https://docs.opendesign.com/tv/tv_working_with_base_classes_view_functionality.html#7">Managing Render Modes</exref>
  */
  void setRenderType(RenderType renderType);
  
  /** \details
    Returns the render type.
    \sa
    <exref target="https://docs.opendesign.com/tv/tv_working_with_base_classes_view_functionality.html#7">Managing Render Modes</exref>
  */
  RenderType renderType() const;

  /** \details
    Sets the background.
    
    \param backgroundId [in]  Persistent identifier of a background.
    \sa
    <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
  */
  void setBackground(OdDbStub *backgroundId);
  
  /** \details
    Returns the background.
    \sa
    <link tv_working_with_attributes_view_backgrounds, Work with View Backgrounds>
  */
  OdDbStub *background() const;

  /** \details
    Sets the visual style.
    
    \param visualStyleId [in]  Persistent identifier of a background.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  void setVisualStyle(OdDbStub *visualStyleId);
  
  /** \details
    Returns the visual style.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  OdDbStub *visualStyle() const;
  
  /** \details
    Sets the visual style.
    
    \param visualStyle [in]  Visual style to set.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  void setVisualStyle(const OdGiVisualStyle &visualStyle);
  
  /** \details
    Returns the visual style.
    \param visualStyleId [out]  Receives visual style.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  bool visualStyle(OdGiVisualStyle &visualStyle) const;
  const OdGiDrawable *visualStyleDrawable() const;

  virtual bool makeStock(OdDbStub* layoutId);
  virtual void releaseStock(OdDbStub* layoutId);

  /** \details
    Checks whether this model has changed layers.
    \returns
    true if this model has changed layers, false otherwise.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  bool hasChangedLayers() const { return m_pFirstChangedLayer != NULL; }

  /** \details
    Adds a model reactor to process notifications.
    \param pReactor [in]  Pointer to a model reactor.
  */
  void addModelReactor(OdGsModelReactor *pReactor);
  
  /** \details
    Removes a model reactor.
    \param pReactor [in]  Pointer to a model reactor.
  */
  void removeModelReactor(OdGsModelReactor *pReactor);

  /** \details
    Specifies whether sectioning should be enabled for this model.
    \param bEnable [in]  Enables or disables sectioning.
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
    \sa
    <link tv_planarClip_classes, Main Classes for Planar Clipping Section Generation>
  */
  void setEnableSectioning(bool bEnable);
  
  /** \details
    Checks whether sectioning is enabled for this model.
    \returns
    true if sectioning is enabled for this model, false otherwise.
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
    \sa
    <link tv_planarClip_classes, Main Classes for Planar Clipping Section Generation>
  */
  bool isSectioningEnabled() const;
  
  /** \details
    Sets active sectioning for this model. After specifying the sectioning, it should be enabled by the setEnableSectioning() method.
    
    \param points   [in]  Array of co-planar points specifying a sectioning region.
    \param upVector [in]  Vector that specifies the orientation of the plane that contains points.
    \returns
     true if the specified sectioning is valid. Returns false if the specified sectioning is invalid or malformed.
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
    \sa
    <link tv_planarClip_classes, Main Classes for Planar Clipping Section Generation>
  */
  bool setSectioning(const OdGePoint3dArray &points, const OdGeVector3d &upVector);
  
  /** \details
    Sets active sectioning for this model. The sectioning is capped vertically using the specified top and bottom heights. 
    After specifying the sectioning, it should be enabled by the setEnableSectioning() method.
    
    \param points   [in]  Array of co-planar points specifying a sectioning region.
    \param upVector [in]  Vector that specifies the orientation of the plane that contains points.
    \param dTop     [in]  Specifies the top capping height of the sectioning.
    \param dBottom  [in]  Specifies the bottom capping height of the sectioning.
    \returns
     true if the specified sectioning is valid. Returns false if the specified sectioning is invalid or malformed.
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
    \sa
    <link tv_planarClip_classes, Main Classes for Planar Clipping Section Generation>
  */
  bool setSectioning(const OdGePoint3dArray &points, const OdGeVector3d &upVector,
                     double dTop, double dBottom);
  
  /** \details
    Sets the visual style for the objects on the clipped side of the sectioning. 
    
    \param visualStyleId   [in]  Visual style object ID.
    \remarks
    If the input parameter is a null object ID, the clipped side should not be drawn.
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
    \sa
    <link tv_planarClip_classes, Main Classes for Planar Clipping Section Generation>
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  void setSectioningVisualStyle(OdDbStub *visualStyleId);
  
  /** \details
    Sets the visual style for the objects on the clipped side of the sectioning. 
    
    \param visualStyleId   [in]  Visual style object ID.
    \remarks
    If the input parameter is a null object ID, the clipped side should not be drawn.
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
    \sa
    <link tv_planarClip_classes, Main Classes for Planar Clipping Section Generation>
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  const SectioningSettings &getSectioning() const { return m_sectioning; }

  /** \details
    Specifies whether this model's geometry should ignore the view's front and back clipping planes (if any) and viewport 3d clipping (if set). 
    If the method's input value is false, the view's clipping planes and viewport 3d clipping are allowed by this Model object.
    
    \param bEnable   [in]  Flag that specifies whether the model ignores the view's clipping planes and viewport 3d clipping. 
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
  */
  void setViewClippingOverride(bool bEnable);
  
  /** \details
    Checks whether view clipping override is enabled for this Model object.
    \returns 
    true if view clipping override is enabled for this Model object, false otherwise.   
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
  */
  bool viewClippingOverride() const;

  /** \details
    Enables or disables linetype display in any OdGsView object that views this Model object.
    
    \param bEnable   [in]  Flag that specifies whether to enable linetype display.
  */
  void setEnableLinetypes(bool bEnable);
  
  /** \details
    Checks whether linetype display in any OdGsView object that views this Model object is enabled.
    \returns
    true if linetype display is enabled, false otherwise.
  */
  bool isLinetypesEnabled() const;

  /** \details
    Enables or disables selectability of this model (ability to select entities inside this Model object).
    
    \param bEnable   [in]  Flag that specifies whether to enable selectability of this model.
  */
  void setSelectable(bool bEnable);
  
  /** \details
    Checks whether this model is selectable (provides an ability to select entities inside this Model object).
   \returns
   true if this model is selectable, false otherwise.
  */
  bool isSelectable() const;

  /** \details
    Enables or disables extents calculation for entities inside this Model object. 
    \param bEnable   [in]  Extents calculation for entities inside this model.
  */
  void setEnableViewExtentsCalculation(bool bEnable);
  
  /** \details
    Checks whether extents calculation for entities inside this Model object is enabled. 
    \returns
    true if extents calculation for entities inside this Model object is enabled, false otherwise.
  */
  bool isViewExtentsCalculationEnabled() const;

  /** \details
    Enables or disables lights in blocks. 
    \param bEnable   [in]  Flag that specifies whether to enable lights inside blocks.
    \sa
    <link tv_working_with_lights, Work with Lights>
    \sa
    <link tv_working_with_base_classes_reference_geometry, Work with Reference Geometry>
  */
  void setEnableLightsInBlocks(bool bEnable);
  
  /** \details
    Checks whether lights are enabled inside blocks. 
    \returns
    true if lights are enabled inside blocks, false otherwise. 
    \sa
    <link tv_working_with_lights, Work with Lights>
    \sa
    <link tv_working_with_base_classes_reference_geometry, Work with Reference Geometry>
  */
  bool isLightsInBlocksEnabled() const;

  /** \details
    Specifies whether this model's geometry should ignore the view's front and back clipping planes (if any) and viewport 3d clipping (if set). 
    If the method's input value is false, the view's clipping planes and viewport 3d clipping are allowed by this Model object.
    
    \param bEnable   [in]  Flag that specifies whether to enable view sectioning override in this Model object. 
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
    \sa
    <link tv_planarClip, Main Classes for Planar Clipping Section Generation>
  */
  void setViewSectioningOverride(bool bEnable);
  
  /** \details
    Checks the current state of view sectioning override.
    \returns
    true if view sectioning override is enabled, false otherwise. 
    \sa
    <link tv_working_with_features_cutting_planes, Work with Cutting Planes>
    \sa
    <link tv_planarClip, Main Classes for Planar Clipping Section Generation>
  */
  bool viewSectioningOverride() const;

  virtual bool saveModelState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  virtual bool loadModelState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
  virtual bool saveClientModelState(OdGsFiler *pFiler) const;
  virtual bool loadClientModelState(OdGsFiler *pFiler);
  bool postprocessModelLoading(OdGsFiler *pFiler);
protected:
  void onModifiedNode(OdGsNode* pCache, OdGiDrawable* pDrawable, OdGiDrawable* pParent);
  void onModifiedDrawable(OdGiDrawable* pDrawable, OdGiDrawable* pParent);
  void onModifiedNodeImp(OdGsNode* pCache, OdGiDrawable* pDrawable, OdGiDrawable* pParent, bool bGraphicsOnly);
  void onPropertyModifiedImpl(OdGsBaseModelReactor::ModelProperty nProp, bool &bReturnValue);
  virtual bool onPropertyModified(OdGsBaseModelReactor::ModelProperty nProp);
public:
  virtual OdGsNode* newNode(ENodeType ntp, const OdGiDrawable* drawable, bool bSetGsNode);
};

inline OdGsView::RenderMode OdGsBaseModel::renderModeOverride() const
{
  return m_renderModeOverride;
}

inline bool OdGsBaseModel::viewClippingOverride() const
{
  return GETBIT(m_gsModelFlags, kSkipViewClip);
}

inline bool OdGsBaseModel::viewSectioningOverride() const
{
  return GETBIT(m_gsModelFlags, kSkipViewSection);
}

inline OdGiDrawablePtr OdGsBaseModel::open(OdDbStub* objectId) 
{
  if(m_openDrawableFn)
    return m_openDrawableFn(objectId);
  //ODA_ASSERT_ONCE(objectId==0);
  return OdGiDrawablePtr();
}

inline OdUInt32 OdGsBaseModel::numViewProps() const
{
  return m_viewProps.size();
}

inline const ViewProps& OdGsBaseModel::viewProps(OdUInt32 viewportId) const
{
  ODA_ASSERT(m_viewProps.size() > viewportId);
  return m_viewProps[viewportId];
}

inline OdUInt32 OdGsBaseModel::viewChanges(OdUInt32 viewportId) const
{
  return viewProps(viewportId).m_nViewChanges;
}

inline OdGsViewImpl *OdGsBaseModel::refView() const
{
  if (m_views.empty())
    return NULL;
  return m_views.first().first;
}

inline OdGsModel::RenderType OdGsBaseModel::renderType() const
{
  return m_renderType;
}

inline bool OdGsBaseModel::isIdentityTransform() const
{
  return GETBIT(m_gsModelFlags, kIdentityXform);
}

inline const OdGiDrawable *OdGsBaseModel::visualStyleDrawable() const
{
  return m_pTransVisualStyle.get();
}

#include "TD_PackPop.h"

#endif // __ODGSMODELIMPL_H__
