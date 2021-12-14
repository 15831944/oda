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

#ifndef __OD_GS_NODE__
#define __OD_GS_NODE__

#include "TD_PackPush.h"

#include "Gs/GsBaseModel.h"
#include "ChunkAllocator.h"

class OdGsContainerNode;
class OdGsLayerNode;
	
class OdGsBaseModel;
class OdGsUpdateContext;
class OdGsDisplayContext;
class OdSiSpatialIndex;
class OdGsUpdateManager;

/** \details
    This class is the base class for custom GsNode objects which implement custom caching support 
    for client vectorization applications.

    \remarks
    Cached rendering information may be stored in a custom GsNode descendant
    for each drawable in an OdDbDatabase object.
    
    This cached information can be fed into the ODA Platform vectorization
    framework during a redraw instead of making the usual calls to subWorldDraw() and 
    subViewportDraw(), thereby greatly improving the speed of redraws.

    \remarks
    Clients should derive their custom node classes from this class.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT ODRX_ABSTRACT OdGsNode : public OdGsCache
{
  friend class OdGsBaseModel;
  friend class OdGsViewImpl;
protected:
  mutable OdGsNode* m_pPrev;
  mutable OdGsNode* m_pNext;

  GS_STATIC_EXPORT static AllocatorArray s_aAlloc;

  OdGsBaseModel*    m_pModel;
  void*             m_underlyingDrawable;
  /** \details
    Enumerates GsNode states
  */
  enum
  {
    kPersistent   = 0x00000001,
    kContainer    = 0x00000002,
    kHLT          = 0x00000004, // Highlight
    kHLT_All      = 0x00000008, // Highlight whole branch
    kSyncDrawable = 0x00000010,
    kHidden       = 0x00000020,
    kSelectHidden = 0x00000040,
    kHiddenAll    = 0x00000080, // The whole node is hidden
    kSelStyle     = 0x00000100, // Selection style provided
    kLastFlag     = kSelStyle
  };

  mutable OdUInt32 m_flags;

  // Flags to provide some custom data for OdGsBaseModel-derived class at application level
  OdUInt32 m_userFlags;

  enum { kInvalidateVp = 0x80000000 };
public:
  inline bool invalidVp() const { return GETBIT(m_flags, kInvalidateVp); }
protected:
  inline void setInvalidVp(bool val) { SETBIT(m_flags, kInvalidateVp, val); }

  /** \details
    Removes the associations between the underlying Drawable object and this GsNode object.
  */
  void clearDrawable();

  /** \details
    Internal part of selection style setter implementation.
    \param nStyle [in] Selection style index.
    \remarks
    For internal use only.
    \sa highlight() method.
  */
  void setSelectionStyleImpl(OdUInt32 nStyle);
  /** \details
    Internal part of selection style getter implementation.
    \remarks
    For internal use only.
    \sa selectionStyle() method.
  */
  OdUInt32 getSelectionStyleImpl() const;
public:
  /** \details
    Sets underlying drawable for current Node object.
    \param pUnderlyingDrawble [in] Drawable object pointer.
  */
  void setToDrawable(const OdGiDrawable* pUnderlyingDrawable);

public:
  ODRX_DECLARE_MEMBERS(OdGsNode);

  void* operator new(size_t);
  void operator delete(void *);

  /** \remarks
      OdGsNode objects perform no reference counting. This function does nothing but return.
  */
  void addRef();

  /** \remarks
      OdGsNode objects perform no reference counting. This function does nothing but return.
  */
  void release();

  /** \details
    Constructor for the OdGsNode class.
    
    \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
  */
  OdGsNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable);
protected:
  OdGsNode(const OdGsNode& c);
public:
  virtual ~OdGsNode();

  /** \details
    Returns a base model that is associated with this node.
    \returns
    Pointer to the BaseModel object associated with this Node object.
  */
  OdGsBaseModel* baseModel() const;

  /** \details
    Returns a model that is associated with this node.
    \returns
    Pointer to the Model object associated with this Node object.
  */
  OdGsModel* model() const;

  /** \details
    Checks whether this node is a container for other nodes.
    \returns
    true if and only if this Node object serves as a container for other Node objects, false otherwise.
  */
  bool isContainer() const;

  virtual ENodeType nodeType() const = 0;

  /** \details
    Checks whether this node is in sync with underlying drawable.
    \returns 
    true if the node is in sync with underlying drawable, false otherwise.
  */
  bool isSyncDrawable() const;

  /** \details
    Invalidates the cached data within this object.
    
    \param pParent [in]  Pointer to the ContainerNode object for this Node object.
    \param pView   [in]  Pointer to the VectorizeView object for which data are to be invalidated.
    \param mask    [in]  Mask.
    \remarks
    The cached data will be regenerated the next time this Node object is accessed.
  */
  virtual void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask) = 0;

  /** \details
    Returns the underlying Drawable object associated with this Node object.
  */
  OdGiDrawablePtr underlyingDrawable() const;

  /** \details
    Retrieves the underlying drawable that is associated with this node.
    \returns 
    Pointer to an ObjectID object of the underlying Drawable object associated with this Node object.
  */
  OdDbStub* underlyingDrawableId() const;

  /** \details
    Updates the cached data within this Node object for the specified VectorizeView and ContainerNode objects. 
    \param pParent [in]  Pointer to the ContainerNode object for this Node object.
    \param view    [in]  VectorizeView object for which data are to be updated.
  */
  virtual void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex* pParentIndex) = 0;

  /** \details
    Updates the cached data if Node object is visible in specified view.
    \param pViewImpl [in] pointer to the View Implementation.
  */
  virtual void updateVisible( OdGsViewImpl* pViewImpl ) {};

  /** \details
    Paints the specified view with the cached data associated with this Node object.
    \param ctx [in]  Current GS update context.
  */
  virtual void display(OdGsDisplayContext& ctx) = 0;

  /** \details
    Invalidates parts of cache depended on layers modified since last regeneration.
    
    \param view [in] View Implementation object.
    \remarks
    For internal use only.
    Returns this if and only if the this node is one of modified layers.
  */
  virtual void propagateLayerChanges(OdGsViewImpl& view) = 0;

  /** \details
    Highlights the specified node.
    \param bDoIt        [in] Flag that specifies whether to highlight. If true - highlight, if false - unhighlight.
    \param bWholeBranch [in] Specifies whether the node highlight state will be applied to all subentitites.
    \param nSelStyle    [in] Optional selection style index.
  */
  virtual void highlight(bool bDoIt, bool bWholeBranch, OdUInt32 nSelStyle = 0);

  /** \details
    Returns current node highlight state.
    \returns 
    true if current node highlighted, false otherwise.
  */
  bool isHighlighted() const;
  /** \details
    Checks whether current node highlight state is applied to all subentities.
    \returns 
    true if current highlight state is applies to all subentities, false otherwise.
  */
  bool isHighlightedAll() const;

  virtual void destroy() = 0;
  /** \details
    Hides the specified node.
    \param bDoIt        [in] Flag that specifies whether to hide this node (true) or show it (false).
    \param bSelectable  [in] Flag that specifies whether this node should be selectable when it is hidden.
    \param bWholeBranch [in] Flag that specifies whether the node hidden state should be changed for all branches.
  */
  virtual void hide(bool bDoIt, bool bSelectable = false, bool bWholeBranch = true);
  /** \details
    Returns current node invisibility state.
    \returns 
    true if current node is invisible, false otherwise.
  */
  bool isHidden() const;
  /** \details
    Returns a flag that specifies whether this node should be selectable when it is hidden.
    \returns 
    true if the node should be selectable when it is hidden, false otherwise.
  */
  bool isSelectableIfHidden() const;
  /** \details
    Returns true if and only if the whole node is hidden
    \returns
    Returns true if and only if the whole node is hidden
  */
  bool isHiddenAll() const;

  /** \details
    Returns node selection style.
    \returns 
    Current node selection style.
  */
  OdUInt32 selectionStyle() const;
  /** \details
    Checks whether this node has a non-default selection style.
    \returns 
    true if this node has a non-default selection style, false otherwise.
  */
  bool hasSelectionStyle() const;

  /** \details
    Returns user-defined flags.
    \returns 
    Set of user-defined flags.
  */
  OdUInt32 userFlags() const;
  /** \details
    Sets user-defined flags.
    \param val [in] Set of user-defined flags.
  */
  void setUserFlags(OdUInt32 val);

  virtual bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  virtual bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
  virtual bool saveClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer) const;
  virtual bool loadClientNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer);
  virtual bool postprocessNodeLoading(OdGsFiler *pFiler);

  TD_USING(OdGsCache::extents);

  virtual bool extents(const OdGsView* pView, OdGeExtents3d& ext) const;

  bool modelExtents(OdGeExtents3d& ext, bool bUseModelTf = true) const;
  bool modelExtents(const OdGsView* pView, OdGeExtents3d& ext, bool bUseModelTf = true) const;

  virtual OdDb::LineWeight getMaxLineweightUsed() const;

  /** \details
    Resets pointer to the underlying GiDrawable object associated with this Node object.
  */
  virtual void setDrawableNull();

  struct UpdateManagerContext
  {
    enum ContextType
    {
      kForMemoryLimit = 0,
      kForDrawOrder
    };
    ContextType contextType;
    OdGsViewImpl* pView;
  };

  /** \details
    Adds content of node to the update manager.
    \param viewportId [in]  Id of a viewport.
    \param pManager   [in] Pointer to the Update Manager instance.
    \param context    [in] UpdateManagerContext that specify what values should be initialized.
  */
  virtual void addContentToUpdateManager( OdUInt32 viewportId, OdGsUpdateManager* pManager, const UpdateManagerContext& context ) {}

  /** \details
    Checks whether metafiles are compatible with for the specified vectorize views.
    \param pView1 [in] Pointer to the VectorizeView
    \param pView2 [in] Pointer to the VectorizeView
    \returns 
    true if and only if metafiles are compatible for the specified vectorize views.
  */
  virtual bool isMetafilesCompatible( const OdGsViewImpl* pView1, const OdGsViewImpl* pView2, OdGsUpdateContext* ctx ) { return true; }
};

inline OdGiDrawablePtr OdGsNode::underlyingDrawable() const
{
  if(GETBIT(m_flags, kPersistent))
    return m_pModel->open(reinterpret_cast<OdDbStub*>(m_underlyingDrawable));
  return reinterpret_cast<OdGiDrawable*>(m_underlyingDrawable);
}

inline OdDbStub* OdGsNode::underlyingDrawableId() const
{
  if(GETBIT(m_flags, kPersistent))
    return reinterpret_cast<OdDbStub*>(m_underlyingDrawable);
  return 0;
}

inline OdUInt32 OdGsNode::selectionStyle() const
{
  if (hasSelectionStyle())
    return getSelectionStyleImpl();
  return 0;
}

inline bool OdGsNode::hasSelectionStyle() const
{
  return GETBIT(m_flags, kSelStyle);
}

inline void OdGsNode::highlight(bool bDoIt, bool bWholeBranch, OdUInt32 nSelStyle)
{
  SETBIT(m_flags, kHLT, bDoIt);
  SETBIT(m_flags, kHLT_All, bDoIt ? bWholeBranch : false);
  if ((bDoIt && (nSelStyle || hasSelectionStyle())) || (!bDoIt && hasSelectionStyle()))
    setSelectionStyleImpl(nSelStyle);
}

inline bool OdGsNode::isHighlighted() const
{
  return GETBIT(m_flags, kHLT);
}

inline bool OdGsNode::isHighlightedAll() const
{
  return GETBIT(m_flags, kHLT_All);
}

inline OdGsBaseModel* OdGsNode::baseModel() const
{
  return const_cast<OdGsBaseModel*>(m_pModel);
}

inline bool OdGsNode::isContainer() const
{
  return GETBIT(m_flags, kContainer);
}

inline bool OdGsNode::isSyncDrawable() const
{
  return GETBIT(m_flags, kSyncDrawable);
}

inline bool OdGsNode::isHidden() const
{
  return GETBIT(m_flags, kHidden);
}

inline bool OdGsNode::isSelectableIfHidden() const
{
  return GETBIT(m_flags, kSelectHidden);
}

inline bool OdGsNode::isHiddenAll() const
{
  return GETBIT(m_flags, kHiddenAll);
}

inline OdUInt32 OdGsNode::userFlags() const
{
  return m_userFlags;
}

inline void OdGsNode::setUserFlags(OdUInt32 val)
{
  m_userFlags = val;
}

#include "TD_PackPop.h"

#endif // __OD_GS_NODE__
