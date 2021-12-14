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

#ifndef __OD_GS_LAYER_NODE__
#define __OD_GS_LAYER_NODE__

#include "TD_PackPush.h"

#include "Gs/GsNode.h"
#include "Gs/GsViewImpl.h"
#include "Gi/GiLayerTraitsData.h"
#include "ThreadsCounter.h"
#include "GsEmptyMetafileCache.h"
#include "OdVector.h"

typedef OdVector<OdGiLayerTraitsData, OdMemoryAllocator<OdGiLayerTraitsData> > OdGiLayerTraitsDataArray;

/** <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsLayerNode : public OdGsNode
{
  enum
  {
    kDirty     = OdGsNode::kLastFlag << 1,
    kVpDep     = OdGsNode::kLastFlag << 2,
    kVpFrozen  = OdGsNode::kLastFlag << 3,

    kLastFlag = kVpFrozen
  };
protected:
  /** \details
    Updates this container node. Does nothing in default implementation.
    
    \param ctx          [in]  GS update context.
    \param pParent      [in]  Pointer to a parent container node.
    \param pParentIndex [in]  Pointer to a parent spatial index.
  */
  void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex* pParentIndex);

  /** \details
    Paints the specified view with the cached data associated with this Node object. Does nothing in default implementation.
    \param ctx [in]  Display context.
  */
  virtual void display(OdGsDisplayContext& ctx);

  /** \details
    Returns extents of this container node object. Does nothing in default implementation.
    \returns
    false
  */
  bool extents(OdGeExtents3d& ) const;

  /** \details
    Invalidates parts of cache depended on layers modified since last regeneration.
    
    \param view [in]  Implementation view object.
    \remarks
    For internal use only.
    Returns this if and only if the this node is one of modified layers.
  */
  void propagateLayerChanges(OdGsViewImpl& view);

  /** \details
    Updates the viewport-frozen flag state of this layer node.
    If this layer is not visible for one of the views in an associated base model,
    the kVpFrozen flag for this layer node is set to true.
    
    \param view [in]  Implementation view object. Not used.
  */
  void updateVpFrozen(OdGsViewImpl *pView);

  OdGiLayerTraitsData m_layerTraits;
  mutable OdGiLayerTraitsDataArray m_vpLayerTraits;

  friend class OdGsBaseModel;
  OdGsLayerNode* m_pNextChanged;
public:
  ODRX_DECLARE_MEMBERS(OdGsLayerNode);

  /** \details
    Constructor for the OdGsLayerNode class.
    
    \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
    \param bSetGsNode [in]  Flag that controls whether to set GS node to the underlying drawable.
  */
  OdGsLayerNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable, bool bSetGsNode = true);
  
  /** \details
    Destructor for the OdGsLayerNode class.
  */
  ~OdGsLayerNode();
  
  /** \details
    Destroys a layer node. Does nothing in default implementation.
  */
  void destroy() {}

  /** \details
    Retrieves layer traits.
    
    \param nVpId [in]  Viewport ID for viewport-dependent layer nodes.
  */
  OdGiLayerTraitsData& layerTraits(OdUInt32 nVpId);

  /** \details
    Retrieves layer traits by the passed viewport identifier.
    
    \param nVpId [in]  Viewport ID for viewport-dependent layer nodes.
  */
  const OdGiLayerTraitsData& layerTraits(OdUInt32 nVpId) const;

  void reserveLayerTraits(OdUInt32 nVpId);

  bool isDirty() const;
  void setDirty(bool bDirty);
  
  /** \details
    Checks whether this layer node is up to date.
    
    \param nVpId [in]  Viewport ID for viewport-dependent layer nodes.
    \returns
    true if this layer node is up to date, false otherwise
  */
  bool isUpToDate(OdUInt32 nVpId) const;
  
  /** \details
    Checks whether cache of this layer node is valid.
    
    \param nVpId [in]  Viewport ID for viewport-dependent layer nodes.
    \returns
    true if cache of this layer node is valid, false otherwise
  */
  bool isValidCache(OdUInt32 nVpId) const;
  
  /** \details
    Specifies that cache of this layer node is valid.
    
    \param nVpId [in]  Viewport ID for viewport-dependent layer nodes.
  */
  void setCacheUpToDate(OdUInt32 nVpId);
  
  /** \details
    Checks whether this layer node is marked as invalid.
    
    \returns
    true if this layer node is marked as invalid, false otherwise.
  */
  bool isInvalidated() const;
  
  /** \details
    Checks whether this layer node is viewport-dependent.
    
    \returns
    true if this layer node is viewport-dependent, false otherwise.
  */
  bool isVpDep() const;
  
  /** \details
    Retrieves a node type of this object.
  */
  virtual ENodeType nodeType() const { return kLayerNode; }
  
  /** \details
    Checks whether this layer node is frozen.
    
    \returns
    true if this layer node is frozen, false otherwise.
  */
  inline bool isVpFrozen() const;
  
  /** \details
    Checks whether this layer node is frozen.
    
    \returns
    true if this layer node is frozen, false otherwise.
  */
  inline bool isFrozen() const;

  /** \details
    Invalidates this layer node.
    
    \param pParent  [in]  Parent container node.
    \param view     [in]  Implementation view object for which data is invalidated.
    \param mask     [in]  Mask.
  */
  void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);

  /** \details
    Updates this layer node.
    
    \param view     [in]  Vectorizer view.
  */
  virtual void update(OdGsBaseVectorizer& view);

  /** \details
    Checks whether layer traits of the specified viewports are compatible.
    Layer traits are compatible if:
    * Layer colors are equal
    * Layer lineweights are equal
    * Layer linetypes are equal
    * Layer plot style name types are equal OR plot style name identifiers are equal in case if plotStyleNameType() == OdDb::kPlotStyleNameById
    * Layer material identifiers are equal
    * At least of the layers that some flags set.
    
    \param nVpId1  [in]  First viewport that has layer traits associated to it.
    \param nVpId2  [in]  Second viewport that has layer traits associated to it.
    \returns
    true if layer traits are compatible.
  */
  bool isTraitsCompatible(OdUInt32 nVpId1, OdUInt32 nVpId2) const;

  /** \details
    Stores current state of this layer node into filer object.

    \param pFiler       [out]  Pointer to a filer object.
    \param pVectorizer  [in/out]  Pointer to a vectorizer. May be needed to save vectorizer client metafile (cache).
    \returns
    true if node state is successfully saved, false otherwise.
  */
  bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  
  /** \details
    Restores node state from filer object.

    \param pFiler       [in]  Pointer to a filer object. Filer object.
    \param pVectorizer  [in]  Pointer to a vectorizer. May be needed to load vectorizer client metafile (cache).
    \returns
    true if node state is successfully loaded, false otherwise.
  */
  bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
private:
  friend class OdGsEmptyMetafileCache;
  OdGsEmptyMetafileCache m_emfCache;
  OdMutexPtr m_mt;
};

inline bool
OdGsLayerNode::isDirty() const
{
  return GETBIT(m_flags, kDirty);
}

inline void
OdGsLayerNode::setDirty(bool bDirty)
{
  SETBIT(m_flags, kDirty, bDirty);
}

inline bool OdGsLayerNode::isUpToDate(OdUInt32 nVpId) const
{
  if(isInvalidated())
    return false;
  if(isVpDep())
  {
    if(nVpId > 0 && m_vpLayerTraits.size() < nVpId)
      return false;
    return !GETBIT(layerTraits(nVpId).flags(), 0x80000000);
  }
  return !GETBIT(layerTraits(0).flags(), 0x80000000);
}

inline bool OdGsLayerNode::isInvalidated() const
{
  return (m_layerTraits.m_flags == 0xFFFFFFFF) && !m_vpLayerTraits.size();
}

inline bool OdGsLayerNode::isVpDep() const
{
  return GETBIT(m_flags, kVpDep);
}

inline OdGiLayerTraitsData& OdGsLayerNode::layerTraits(OdUInt32 nVpId)
{
  return const_cast<OdGiLayerTraitsData&>(static_cast<const OdGsLayerNode*>(this)->layerTraits(nVpId));
}

inline const OdGiLayerTraitsData& OdGsLayerNode::layerTraits(OdUInt32 nVpId) const
{
  if(isVpDep() && nVpId > 0)
  {
    ODA_ASSERT(m_vpLayerTraits.size() >= nVpId);
    return m_vpLayerTraits[--nVpId];
  }
  return m_layerTraits;
}

inline bool OdGsLayerNode::isVpFrozen() const
{
  return GETBIT(m_flags, kVpFrozen);
}

inline bool OdGsLayerNode::isFrozen() const
{
  return m_layerTraits.isFrozen();
}

/** \details
    This is a helper class to substitute OdGiLayerTraitsTaker in
    OdGsBaseModel::propagateLayerChanges() where vectorizers are not available
    to be set by setRefView(), and not actually required.
    <group OdGs_Classes>
*/
class OdGiLayerTraitsTakerView : public OdGiLayerTraitsTaker, public OdGiViewport
{
  ODRX_USING_HEAP_OPERATORS(OdGiLayerTraitsTaker);
  public:
    /** \details
      Constructor for the OdGiLayerTraitsTakerView class.
      
      \param view           [in]  Implementation view object.
      \param pInheritTaker  [in]  Pointer to the layer traits taker object.
    */
    OdGiLayerTraitsTakerView(OdGsViewImpl& view, const OdGiLayerTraitsTaker *pInheritTaker = NULL)
      : m_view(view) { if (pInheritTaker) reset(*pInheritTaker); }

    
    /** OdGiViewportDraw methods **/
    
    
    /** \details
      Returns a reference to the OdGiViewport object associated with this object.
      
      \remarks
      The OdGiViewport contains information specific to the viewport being drawn.
    */
    OdGiViewport& viewport() const { return *const_cast<OdGiLayerTraitsTakerView*>(this); }
    
    /** \details
      Returns the number of viewports that have been deleted from the current database 
      during this editing session.
    */
    OdUInt32 sequenceNumber() const { return 0; }
    
    /** \details
      Returns true if and only if specified Viewport ID is a valid Viewport ID.
      
      \param viewportId [in]  Viewport ID.
      
      \remarks
      viewportId is typically obtained from viewport().viewportId().
    */
    bool isValidId(const OdUInt32 acgiId) const { return m_view.isValidViewportId(acgiId); }
    
    /** \details
      Returns the Object ID of the OdDbViewport object associated with this object.
      
      \remarks
      Returns NULL if TileMode == 1.
    */
    OdDbStub* viewportObjectId() const { return m_view.getViewportObjectId(); }

    
    /** OdGiViewport methods **/
    
    
    /** \details
      Returns the transformation matrix from ModelSpace to EyeSpace for this Viewport object.

      \sa
      <link CoordinateSystems, Coordinate Systems>
    */
    virtual OdGeMatrix3d getModelToEyeTransform() const { return getWorldToEyeTransform(); }
    
    /** \details
      Returns the transformation matrix from EyeSpace to ModelSpace for this Viewport object.

      \sa
      <link CoordinateSystems, Coordinate Systems>
    */
    virtual OdGeMatrix3d getEyeToModelTransform() const { return getEyeToWorldTransform(); }
    
    /** \details
      Returns the transformation matrix from WCS to EyeSpace for this Viewport object.

      \sa
      <link CoordinateSystems, Coordinate Systems>
    */
    virtual OdGeMatrix3d getWorldToEyeTransform() const { return m_view.worldToEyeMatrix(); }
    
    /** \details
      Returns the transformation matrix from EyeSpace to WCS for this Viewport object.

      \sa
      <link CoordinateSystems, Coordinate Systems>
    */
    virtual OdGeMatrix3d getEyeToWorldTransform() const { return m_view.eyeToWorldMatrix(); }
    
    /** \details
      Checks whether perspective mode is turned on for this Viewport.
      \returns 
      true if perspective mode is on for this Viewport, false otherwise.
    */
    virtual bool isPerspective() const { return m_view.isPerspective(); }
    
    /** \details
      Applies the current perspective transformation to the specified point.

      \param point [in/out] Any 3D point.
      \returns 
      true if the point was transformed; i.e., a perspective transform
      is active, and the point was neither too close or behind the camera.
      \remarks
      The point is transformed from EyeSpace coordinates to normalized device coordinates.
    */
    virtual bool doPerspective(OdGePoint3d& pt) const { return m_view.doPerspectivePt(pt); }
    
    /** \details
      Applies the inverse of the current perspective transformation to the specified point.

      \param point [in/out] Any 3D point.
      \returns 
      true if the point was transformed; i.e., a perspective transform
      is active, and the point was neither too close or behind the camera.
      \remarks
      The point is transformed from normalized device coordinates to EyeSpace coordinates
    */
    virtual bool doInversePerspective(OdGePoint3d& pt) const { return m_view.doInversePerspectivePt(pt); }
    
    /** \details
      Returns the display pixel density at the specified point for this Viewport object.

      \param point [in]  WCS center of the unit square.
      \param pixelDensity [out]  Receives the pixel density.
      \param bUsePerspective [in]  Enable perspective inclusion into pixel density calculation.
      \remarks
      Pixel density is measured in pixels per WCS unit.
      This function can be used to determine if the geometry generated for an object will 
      be smaller than the size of a pixel.
    */
    virtual void getNumPixelsInUnitSquare(const OdGePoint3d& pt, OdGePoint2d& pixelDensity, bool bUsePerspective = true) const
    { m_view.getNumPixelsInUnitSquare(pt, pixelDensity, bUsePerspective); }
    
    /** \details
      Returns the WCS camera (eye) location for this Viewport object.
    */
    virtual OdGePoint3d getCameraLocation() const { return m_view.position(); }
    
    /** \details
      Returns the WCS camera target for this Viewport object.
    */
    virtual OdGePoint3d getCameraTarget() const { return m_view.target(); }
    
    /** \details
      Returns the WCS camera "up" vector for this Viewport object.
    */
    virtual OdGeVector3d getCameraUpVector() const { return m_view.upVector(); }
    
    /** \details
      Returns the vector from the camera target to the camera location.
    */
    virtual OdGeVector3d viewDir() const { return m_view.eyeVector(); }
    
    /** \details
      Returns the OdGi Viewport ID for this Viewport object.

      \remarks
      The value returned has no relationshp to the CVPORT system variable.
    */
    virtual OdUInt32 viewportId() const { return m_view.viewportId(); }
    
    /** \details
      Returns the ID for this Viewport object.

      \remarks
      The value returned corresponds to the CVPORT system variable.
    */
    virtual OdInt16 acadWindowId() const
    { OdGsClientViewInfo vi;
      m_view.clientViewInfo(vi);
      return OdInt16(vi.acadWindowId);
    }
    
    /** \details
      Returns lower-left and upper-right corners of this Viewport object in 
      normalized device Coordinates.

      \param lowerLeft [out]  Receives the lower-left corner.
      \param upperRight [out]  Receives the upper-right.
      \remarks
      This information lets position items that are fixed in size and/or position
      with respect to a Viewport.   
    */
    virtual void getViewportDcCorners(OdGePoint2d& lowerLeft, OdGePoint2d& upperRight) const
    { m_view.viewportDcCorners(lowerLeft, upperRight); }
    
    /** \details
      Returns the front and back clipping parameters for this Viewport object.

      \param clipFront [out]  Receives true if and only if front clipping is enabled.
      \param clipBack [out]   Receives true if and only if back clipping is enabled.
      \param front [out]  Receives the front clipping distance.
      \param back [out]  Receives the back clipping distance.
      \returns 
      result of clipFront || clipBack.
      \remarks
      * Front and back are the eye coordinate Z values. 
      * Clipping planes are perpendicular to the view direction.
      * If the front clipping plane is enabled, geometry in front of it is not displayed.
      * If the back clipping plane is enabled, geometry in back of it is not displayed.
    */
    virtual bool getFrontAndBackClipValues(bool& clipFront, bool& clipBack, double& front, double& back) const
    { clipFront = m_view.isFrontClipped();
      clipBack = m_view.isBackClipped();
      front = m_view.frontClip();
      back = m_view.backClip();
      return (clipFront || clipBack);
    }
    
    /** \details
      Returns a multiplier that is used to scale all linetypes in this Viewport object.
    */
    virtual double linetypeScaleMultiplier() const { return m_view.getLinetypeScaleMultiplier(); }
    
    /** \details
      Returns the WCS value below which linetype generation is not used.

      \remarks
      If the WCS length an entire linetype pattern is less than the returned value, then
      the linetype will be rendered as continuous.
    */
    virtual double linetypeGenerationCriteria() const { return m_view.getLinetypeGenerationCriteria(); }
    
    /** \details
      Checks whether the specified layer is not frozen either globally or in this Viewport object.

      \param layerId [in]  Object ID of OdLayerTableRecord.
      \returns 
      true if the specified layer is not frozen either globally or in this Viewport object, false otherwise.
    */
    virtual bool layerVisible(OdDbStub* layerId) const { return m_view.isLayerVisible(layerId); }


    /** OdRxObject methods **/
    
    
    /** \details
      Adds reference to this object. Does nothing in current implementation.
    */
    virtual void addRef() {}
    
    /** \details
      Releases reference to this object. Does nothing in current implementation.
    */
    virtual void release() {}


    /** OdGiCommonDraw methods **/
    
    
    /** \details
      Retrieves the recommended maximum deviation of the
      current vectorization for the specified point on a curve.

      \param type [in]  Deviation type.
      \param pt [in]  Point on a curve.
          
      \returns
      recommended maximum difference (with respect to the current active viewport) between the actual curve or surface, 
      and the tessellated curve or surface. 
      
      \remarks
      deviationType must be one of the following:
      
      <table>
      Name                       Value
      kOdGiMaxDevForCircle       0      
      kOdGiMaxDevForCurve        1      
      kOdGiMaxDevForBoundary     2      
      kOdGiMaxDevForIsoline      3
      kOdGiMaxDevForFacet        4
      </table>
    */
    virtual double deviation(const OdGiDeviationType type, const OdGePoint3d& pt) const
    { return m_view.getDeviation(type, pt, true); }
    
    /** \details
      Retrieves the number of isolines to be drawn on surfaces during this vectorization.
      \remarks
      Isolines are lines or curves which are used to represent the entity. 
      For example, if an entity is a sphere and this method returns 3, the 
      sphere should be displayed as 3 circles with a common center and planes that are
      perpendicular to each other.
    */
    virtual OdUInt32 numberOfIsolines() const { return m_view.userGiContext()->numberOfIsolines(); }
    
    /** \details
      Retrieves the regeneration type of the current vectorization process.

      \returns
      one of the following regeneration types:
      
      <table>
      Name                            Value 
      eOdGiRegenTypeInvalid           0
      kOdGiStandardDisplay            2
      kOdGiHideOrShadeCommand         3
      kOdGiRenderCommand              4
      kOdGiForExplode                 5
      kOdGiSaveWorldDrawForProxy      6
      kOdGiForExtents                 7
      </table>
    */
    virtual OdGiRegenType regenType() const { return m_view.getRegenType(); }
    
    /** \details
      Checks whether this vectorization process should be aborted.
      
      \returns
      true if this vectorization process should be aborted.
    */
    virtual bool regenAbort() const { return m_view.userGiContext()->regenAbort(); }
    
    /** \details
      Retrieves pointer to a context associated with this object.
    */
    virtual OdGiContext* context() const { return m_view.userGiContext(); }
  protected:
    OdGsViewImpl& m_view;
};

// Include inline OdGsEmptyMetafileCache methods
#ifndef __OD_GS_EMPTY_METAFILE_CACHE_INLINES_INCLUDE__
#define __OD_GS_EMPTY_METAFILE_CACHE_INLINES_INCLUDE__
#include "GsEmptyMetafileCache.h"
#endif
//

#include "TD_PackPop.h"

#endif // __OD_GS_LAYER_NODE__
