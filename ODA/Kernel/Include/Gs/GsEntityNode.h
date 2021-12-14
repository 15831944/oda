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

#ifndef __OD_GS_ENTITY_NODE__
#define __OD_GS_ENTITY_NODE__

#include "Gs/GsNode.h"
#include "Si/SiSpatialIndex.h"
#include "OdArrayMemAlloc.h"
#include "Gs/GsHighlightData.h"
#include "Gs/GsUpdateManager.h"

class OdGsBaseVectorizer;
class OdGsBaseVectorizeDevice;

#include "TD_PackPush.h"

/** <group !!RECORDS_TD_APIRef>
*/
struct OdGsGeomPortion
{
  /** Pointer to an associated layer node.*/
  OdGsLayerNode*    m_pLayer;
  /** Smart pointer to a metafile.*/
  OdRxObjectPtr     m_pGsMetafile;
  /** Pointer to a next geometry portion.*/
  OdGsGeomPortion*  m_pNext;

  /** \details
    Default constructor for the OdGsGeomPortion structure.
  */
  OdGsGeomPortion()
    : m_pLayer(0)
    , m_pGsMetafile(0)
    , m_pNext(0)
  {
  }

  /** \details
    Copies properties of the input geometry portion object.
    
    \param c [in]  Geometry portion to copy.
  */
  void copyFrom(OdGsGeomPortion& c)
  {
    m_pLayer = c.m_pLayer;
    m_pGsMetafile = c.m_pGsMetafile;
    m_pNext = c.m_pNext;
  }

  /** \details
    Sets properties of this objects to NULL.
  */
  void clear()
  {
    m_pLayer = NULL;
    m_pGsMetafile = NULL;
    m_pNext = NULL;
  }

  /** \details
    Erases nested portions, if exist.
  */
  void freeNestedPortions()
  {
    OdGsGeomPortion* pPortion = m_pNext;
    while (pPortion)
    {
      OdGsGeomPortion* pNextPortion = pPortion->m_pNext;
      delete pPortion;
      pPortion = pNextPortion;
    }
  }

  /** \details
    Checks whether this geometry portion is empty.
  */
  bool isEmptyPortion() const { return (m_pLayer == NULL) && m_pGsMetafile.isNull(); }
  
  /** \details
    Erases empty nested geometry portions.
  */
  void filterEmptyNestedPortions()
  {
    while (m_pNext && m_pNext->isEmptyPortion())
    {
      OdGsGeomPortion* pNextPortion = m_pNext->m_pNext;
      delete m_pNext;
      m_pNext = pNextPortion;
    }
    if (m_pNext)
      m_pNext->filterEmptyNestedPortions();
  }
};

class OdGsBaseVectorizeDevice;
class OdGsBaseContext;

/** \details
    This class implements OdGsNode descendant objects that
    can serve as containers for other OdGsNode objects.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsEntityNode : public OdGsNode, public OdSiEntity
{
  OdGsEntityNode*             m_pNextEntity;

  /** \details
    Represents next entity entry of this entity node.
    <group !!RECORDS_TD_APIRef>
  */
  struct NextEntityEntry
  {
    OdGsEntityNode*           m_pNextEntity;
    OdUInt32                  m_entFlags;
  };
  /** \details
    Allows next entity to be an array of entities.
    \param nMaxVpId [in]  Viewport ID.
  */
  void allocNextEntityArray(OdUInt32 nMaxVpId);
  
  /** \details
    Retrieves pointer to a first entity entry. 
  */
  inline NextEntityEntry *firstEntEntry() { return reinterpret_cast<NextEntityEntry*>(m_pNextEntity); }
  
  /** \details
    Retrieves pointer to a first entity entry. 
  */
  inline const NextEntityEntry *firstEntEntry() const { return reinterpret_cast<const NextEntityEntry*>(m_pNextEntity); }
  inline void checkEntEntry(OdUInt32 nVpId) const { if ((OdUInt32)OdPtrToInt32(firstEntEntry()->m_pNextEntity) <= nVpId) throw OdError(eInvalidIndex); }
  
  /** \details
    Retrieves pointer to a next entity entry relative to the specified index.
    \param nVpId [in]  Index.
  */
  inline NextEntityEntry *nextEntEntry(OdUInt32 nVpId) { checkEntEntry(nVpId); return reinterpret_cast<NextEntityEntry*>(m_pNextEntity) + (nVpId + 1); }
  
  /** \details
    Retrieves pointer to a next entity entry relative to the specified index.
    \param nVpId [in]  Index.
  */
  inline const NextEntityEntry *nextEntEntry(OdUInt32 nVpId) const { checkEntEntry(nVpId); return reinterpret_cast<const NextEntityEntry*>(m_pNextEntity) + (nVpId + 1); }
protected:
  mutable OdGeExtents3d       m_extents;

public:
  class GS_TOOLKIT_EXPORT Metafile : public OdRxObject
  {
  public:
    void* operator new(size_t);
    void operator delete(void *);

    /** \details
      Default constructor for the Metafile class. Sets aware flags to 0.
    */
    Metafile();
    
    /** \details
      Destructor for the Metafile class. Erases nested geometry portions.
    */
    ~Metafile();

    /** \details
      Plays a metafile for the specified enity node.
      
      \param view   [in]  Vectorizer view.
      \param eMode  [in]  Metafile playing mode.
      \param node   [in]  Entity node.
      \param ctx    [in]  Context.
      \returns
      true if metafile is played successfully, false otherwise.
    */
    bool play(OdGsBaseVectorizer& view, EMetafilePlayMode eMode, OdGsEntityNode& node, OdGsBaseContext* ctx = NULL);
    
    /** \details
      Plays a nested metafile for the specified entity node.
      
      \param view   [in]  GS view.
      \param eMode  [in]  Metafile playing mode.
      \param node   [in]  Entity node.
      \param ctx    [in]  Context.
      \returns
      true if metafile is played successfully, false otherwise.
    */
    bool playNested(const OdGsView& view, OdGsEntityNode& node, OdGsBaseContext& ctx);
    
    /** \details
      Checks whether this metafile has data to play within a metafile play mode.
      
      \param view   [in]  GS view.
      \param eMode  [in]  Metafile playing mode.
      \returns
      true if there is data to play, false otherwise.
    */
    bool hasDataToPlay(const OdGsView& view, EMetafilePlayMode eMode = kMfDisplay) const;
    
    /** \details
      Checks whether at least one geometry portion of this metafile has a layer that changed.
      
      \param nVpId   [in]  Viewport ID.
      \returns
      true if one or more layers changed, false if all the layer are up to date.
    */
    bool layersChanged(OdUInt32 nVpId) const;
    
    /** \details
      Checks whether this metafile contains viewport-frozen layers.
      
      \returns
      true if there viewport-frozen layers, false otherwise.
    */
    bool containsVpFrozenLayers() const;
    
    /** \details
      Checks whether this metafile contains frozen layers.
      
      \returns
      true if there frozen layers, false otherwise.
    */
    bool containsFrozenLayers() const;
    
    /** \details
      Checks whether this metafile contains at least one of the provided layers.
      
      \param layers   [in]  Pointer to an array of layers.
      \param count    [in]  Number of layers.
      \returns
      true if any of the passed layers found, false otherwise.
    */
    bool containsAnyOfLayers( OdDbStub*const* layers, OdUInt32 count ) const;

    /** \details
      Checks whether this metafile is empty.
      
      \returns
      true if this metafile is empty, false otherwise.
    */
    bool isEmpty() const;
    
    /** \details
      Checks whether all layers are turned off.
      \param viewportId    [in]  Viewport ID.
      \returns
      true all layers are off, false otherwise.
    */
    bool isAllLayersOff( OdUInt32 viewportId ) const;

    /** Aware flags.*/
    OdUInt32            m_nAwareFlags;
    /** Geometry extents.*/
    OdGeExtents3d       m_extents;
    /** Geometry portion.*/
    OdGsGeomPortion     m_first;
  };
  friend class Metafile;

  /** \details
    This template class is a specialization of the OdSmartPtr class for Metafile object pointers.
  */
  typedef OdSmartPtr<Metafile> MetafilePtr;

  class GS_TOOLKIT_EXPORT MetafileEx : public Metafile, public OdGsUpdateManager::OdGsUpdateManagerElement
  {
  protected:
    OdRxObjectPtr m_pDeviceRef;
    OdUInt64 m_nLength;
  public:
    class GS_TOOLKIT_EXPORT OdGsDeviceSelfReference : public OdRxObject
    {
      OdGsBaseVectorizeDevice* m_pDevice;
    public:
      OdGsDeviceSelfReference()
      {
        m_pDevice = NULL;
      };
      void setDevice( OdGsBaseVectorizeDevice* device ) 
      {
        m_pDevice = device;
      }
      OdGsBaseVectorizeDevice* device() { return m_pDevice; }
    };

    MetafileEx() : Metafile(), OdGsUpdateManager::OdGsUpdateManagerElement()
    {
      m_pDeviceRef = NULL;
    }
    ~MetafileEx();
    void setDevice( OdRxObjectPtr dev ) { m_pDeviceRef = dev; }
    OdRxObjectPtr device() { return m_pDeviceRef; }

    void setMetafileLength( OdUInt64 l ) { m_nLength = l; }
    OdUInt64 metafileLength() const { return m_nLength; }
  };

  friend class OdGsModuleObject;
  friend class OdGsBlockReferenceNode;
public:
  static void* Alloc(size_t nBytes) { return OdGsNode::s_aAlloc.getAt(1)->alloc((int)nBytes); }
  static void Free(void* pMemBlock) { OdGsNode::s_aAlloc.getAt(1)->release(pMemBlock); }
  static void* Realloc(void* pMemBlock, size_t newSize, size_t)
  {
    if(pMemBlock)
      OdGsNode::s_aAlloc.getAt(1)->release(pMemBlock);
    return OdGsNode::s_aAlloc.getAt(1)->alloc((int)newSize);
  }
  /** \details
      This template class is a specialization of the OdArray class for MetafilePtr objects.
  */
  class MetafilePtrArray: public OdArrayMemAlloc<MetafilePtr, OdObjectsAllocator<MetafilePtr>, OdGsEntityNode>
  {
  public:
    ~MetafilePtrArray(){}
  };

  /** \details
    MetafileHolder helper class.

    Corresponding C++ library: TD_Gs
    <group OdGs_Classes> 
  */
  class GS_TOOLKIT_EXPORT MetafileHolder
  {
  public:
    enum 
    {
      kArray = 1,
      kVpDependent = 2,
      kAwareFlagsRegenType = 4,

      kRegenTypeStandard = 8,
      kRegenTypeHideOrShade = 16,
      kRegenTypeRenderCommand = 32,
      kRegenTypeMask = kRegenTypeStandard | kRegenTypeHideOrShade | kRegenTypeRenderCommand,

      kDependentGeometry = 64, // Marks PaperSpace entities
      kLayerDependent = 128 // Marks compound entities w/o blocks cache (#15036)
    };
    
    /** \details
      Default constructor for the MetafileHolder class. Initializes flags to 0.
    */
    MetafileHolder(): m_flags(0)
    {
      *reinterpret_cast<Metafile**>(rawPtr()) = 0;
    }
    
    /** \details
      Copy constructor for the MetafileHolder class. Copies the passed metafile holder. If passed metafile holder is not copied if it is invalid.
      
      \param c [in]  Metafile holder to copy.
    */
    MetafileHolder(const MetafileHolder& c): m_flags(0)
    {
      *reinterpret_cast<Metafile**>(rawPtr()) = 0;
      if(!c.isValid())
        return;
      if(c.isArray())
      {
        allocateArray();
        getArray() = const_cast<MetafileHolder&>(c).getArray();
      }
      else
      {
        *reinterpret_cast<Metafile**>(rawPtr()) = const_cast<MetafileHolder&>(c).get();
        get()->addRef();
      }
      m_flags = c.m_flags;
    }
    
    /** \details
      Destructor for the MetafileHolder class.
    */
    ~MetafileHolder()
    {
      destroy();
    }
    
    /** \details
      Checks whether this metafile holder is valid.
      \returns
      true if this metafile holder is valid, false otherwise.
    */
    bool isValid() const { return *((const Metafile**)rawPtr()) != NULL; }
    
    /** \details
      Checks whether this metafile holder has the kArray flag set to true.
      \returns
      true if kArray flag of this metafile holder is set to true, false otherwise.
    */
    bool isArray() const { return GETBIT(flags(), kArray); }
    
    /** \details
      Checks whether this metafile holder is viewport-dependent.
      \returns
      true if this metafile holder is viewport-dependent, false otherwise.
    */
    bool isVpDependent() const { return GETBIT(flags(), kVpDependent); }
    
    /** \details
      Specifies whether this metafile holder is viewport-dependent.
      \param bOn [in]  Flag that specifies whether this metafile holder is viewport-dependent.
    */
    void setVpDependent(bool bOn)
    {
      SETBIT(m_flags, kVpDependent, bOn);
      if(bOn)
      {
        SETBIT(m_flags, kRegenTypeMask, false);
        setAwareFlagsRegenType(false);
      }
    }

    bool isAwareFlagsRegenType() const { return GETBIT(flags(), kAwareFlagsRegenType); }
    bool isRegenTypeDependent() const { return GETBIT(flags(), kRegenTypeMask); }

    void setDependentGeometry(bool bSet) { SETBIT(m_flags, kDependentGeometry, bSet); }
    bool isDependentGeometry() const { return GETBIT(flags(), kDependentGeometry); }

    void setLayerDependent(bool bSet) { SETBIT(m_flags, kLayerDependent, bSet); }
    bool isLayerDependent() const { return GETBIT(m_flags, kLayerDependent); }

    /** \details
      Retrieves a pointer to a metafile stored by this holder.
    */
    Metafile* get()
    {
        ODA_ASSERT(!isArray());
        return *reinterpret_cast<Metafile**>(rawPtr());
    }
    
    /** \details
      Retrieves a pointer to a metafile stored by this holder.
    */
    const Metafile* get() const
    {
        ODA_ASSERT(!isArray());
        return *reinterpret_cast<Metafile**>(const_cast<MetafileHolder*>(this)->rawPtr());
    }
    
    /** \details
      Retrieves a dynamic array of metafiles.
    */
    MetafilePtrArray& getArray()
    {
        ODA_ASSERT(isArray());
        return *reinterpret_cast<MetafilePtrArray*>(rawPtr());
    }
    
    /** \details
      Retrieves a dynamic array of metafiles.
    */
    const MetafilePtrArray& getArray() const
    {
        ODA_ASSERT(isArray());
        return *reinterpret_cast<const MetafilePtrArray*>(rawPtr());
    }
    
    /** \details
      Sets a metafile for this metafile holder. Existing metafile or an array of metafiles are erased from this holder.
      \param pMf [in]  Pointer to a metafile to set.
    */
    void set(Metafile* pMf)
    {
      destroy();
      *reinterpret_cast<Metafile**>(rawPtr()) = pMf;
      if(pMf)
        pMf->addRef();
    }

    /** \details
      Sets a metafile and specifies a regeneration type if metafile holder depends on a regeneration type.
      \param pMf        [in]  Pointer to a metafile to set.
      \param eRegenType [in]  Regeneration type.
    */
    void setAtRegenType(Metafile* pMf, OdGiRegenType eRegenType);
    
    /** \details
      Retrieves a pointer to metafile for the specified regeneration type.
      \param eRegenType [in]  Regeneration type.
    */
    Metafile* getAtRegenType(OdGiRegenType& eRegenType)
    {
      ODA_ASSERT(isRegenTypeDependent());
      ODA_ASSERT(!isArray());
      if(GETBIT(flags(), kRegenTypeStandard))
        eRegenType = kOdGiStandardDisplay;
      else if(GETBIT(flags(), kRegenTypeHideOrShade))
        eRegenType = kOdGiHideOrShadeCommand;
      else
        eRegenType = kOdGiRenderCommand;
      return get();
    }

    /** \details
      Allocates memory for an array to store metafiles.
    */
    void allocateArray();

    /** \details
      Removes metafile or an entire array of metafiles for this holder.
    */
    void destroy();
    
    /** \details
      Checks whether this metafile holder is valid.
      \returns
      a number of valid metafiles if this object stores an array of metafiles or 1 if it stores only one metafile which is valid.
      If this object contains an array with no valid metafiles or one metafile which is not valid, this method returns 0.
    */
    int checkValid();

    /** \details
      Copies data of this object to the specified object.
      \param c [in]   Metafile holder where to copy data.
      \param n [out]  Receives a number of valid metafiles of the copied metafile holder. See checkValid() method.
    */
    void moveTo(MetafileHolder& c, int& n);
    OdUInt32 awareFlags(OdUInt32 nVpID) const
    {
      if(isArray())
      {
        if (isVpDependent() && (getArray().size() > nVpID))
        { const MetafilePtr &pMetafile = getArray().at(nVpID);
          return (!pMetafile.isNull()) ? pMetafile->m_nAwareFlags : 0;
        }
        return kVpRenderMode | (isAwareFlagsRegenType() ? kVpRegenType : 0);
      }
      return isValid() ? get()->m_nAwareFlags : 0;
    }

    /** \details
      Sets a metafile at the specified index of array. Works only if the matafile holder stores an array of metafiles.
      
      \param n    [in]  Insertion index.
      \param pMf  [in]  Pointer to a metafile to set.
    */
    void setAt(int n, Metafile* pMf)
    {
      ODA_ASSERT(isArray());
      if(getArray().size() <= OdUInt32(n))
        getArray().resize(n + 1);
      getArray()[n] = pMf;
    }

  protected:
    int flags() const { return (int)m_flags; }
    void* rawPtr() { return &m_data; }
    const void* rawPtr() const { return &m_data; }
    bool isValidMf(const Metafile& mf) const;
    void setAwareFlagsRegenType(bool bOn) { SETBIT(m_flags, kAwareFlagsRegenType, bOn); }

  protected:
    OdUInt8 m_flags;
#ifdef TD_STRICT_ALIGNMENT
    OdIntPtr m_data[odmax(sizeof(MetafilePtrArray),sizeof(OdRxObject*)) / sizeof(OdIntPtr)];
#else
    OdUInt8 m_data[odmax(sizeof(MetafilePtrArray),sizeof(OdRxObject*))];
#endif
  };

protected:
  OdGsStateBranchPtr m_hlBranch;
  OdGsStateBranchPtr m_visBranch;

  virtual void propagateLayerChanges(OdGsViewImpl& view);
protected:
  /** \details
    Returns a reference to the metafile array of this EntityNode object as an array of MetafilePtr.
  */
  MetafilePtrArray& metafileArray();

  mutable MetafileHolder m_metafile;
  /** \details
    Returns a reference to the metafile array of this EntityNode object as an array of MetafilePtr.
  */
  const MetafilePtrArray& metafileArray() const;

  /** \details
    Returns the array the objects in the metafile array of this EntityNode object.
  */
  OdGsEntityNode::MetafilePtr metafile() const;

  void setExtentsFromMetafiles();
public:
  /** \details
    Retrieves a static instance of the empty metafile with a certain set of aware flags frequently used in the model. 
    This allows to share the same data without duplication.
    \param flags      [in]  A set of aware flags.
    \param pLayerNode [in]  Layer node with which a starting geometry portion of a metafile is associated.
  */
  static OdGsEntityNode::MetafilePtr getEmptyMetafile(OdUInt32 flags, OdGsLayerNode *pLayerNode = NULL);

  /** \details
    Returns the specified MetafilePtr from the metafile array of this EntityNode object.
    \param metafileIndex [in]  Metafile index.
  */
  OdGsEntityNode::MetafilePtr metafileAt(int metafileIndex) const;
  
  /** \details
    Sets the metafile array for this EntityNode object from the specified array of objects.
    \param metafiles [in] Array of metafile objects.
  */
  void setMetafile(OdGsEntityNode::Metafile* metafiles);
  
  /** \details
    Sets the specified entry in the metafile array of this EntityNode object.
    \param metafileIndex [in]  Metafile index.
    \param pMetafile [in]  Pointer to the Metafile object
  */
  void setMetafileAt(int metafileIndex, OdGsEntityNode::Metafile* pMetafile);

  /** \details
    Convert a RegenType dependent cache to Viewport dependent cache.
    \param ctx [in]  Current update context.
  */
  void convertToViewportDependent(const OdGsUpdateContext& ctx);

  /** \details
    This enumeration used with OdGsEntityNode::metafile method.
  */
  enum MetafileCompatibilityLevel
  {
    /**Avoid check of viewport compatibility. */
    kSkipCheckCompatible = 0,
    /**Check for view changes. */
    kCheckViewChanges    = 1,
    /**Avoid check for view changes, but find compatible metafile for other viewport. */
    kCheckCompatibleView = 2,
    /**Check for view changes and try to find compatible for other viewports. */
    kFindCompatible      = 3
  };

  /** \details
    Returns metafile pointer compatible with requrested view configuration.
    \param view [in]  View object for which metafile is to be returned.
    \param ctx [in]  Current update context.
    \param findCompatibleLevel [in]  Check for viewport changes if and only if true.
  */
  OdGsEntityNode::Metafile* metafile(const OdGsViewImpl& view, OdGsUpdateContext* ctx,
                                     MetafileCompatibilityLevel findCompatibleLevel = kFindCompatible);
  /** \details
    Sets metafile for current view configuration.
    \param ctx [in]  Current update context.
    \param metafiles [in]  Pointer onto new metafile object.
  */
  void setMetafile(OdGsUpdateContext& ctx, OdGsEntityNode::Metafile* metafiles);
  
  /** \details
    Finds compatible cache for the passed view.
    \param keyView    [in]  View implementation object for which to find compatible cache.
    \param ctx        [in]  Current update context.
    \param awareMask  [in]  Aware flags.
  */
  OdRxObject* findCompatibleCache(const OdGsViewImpl& keyView, OdGsUpdateContext* ctx, OdUInt32 awareMask = 0);
protected:
  // m_flags (don't add new flags here, we already utilized ~30 bits)
  enum
  {
    kHasExtents         = OdGsNode::kLastFlag << 1,
    kSpatiallyIndexed   = OdGsNode::kLastFlag << 2,
    kInWorkset          = OdGsNode::kLastFlag << 3,
    kSingleThreaded     = OdGsNode::kLastFlag << 4,
    kOwned              = OdGsNode::kLastFlag << 5,
    kDrawableRegenDraw  = OdGsNode::kLastFlag << 6,
    kEntityUnerased     = OdGsNode::kLastFlag << 7,
    kInvisible          = OdGsNode::kLastFlag << 8,
    kExtentsOutOfMS     = OdGsNode::kLastFlag << 9,
    kNonSelectable      = OdGsNode::kLastFlag << 10,
    kSkipSelection      = OdGsNode::kLastFlag << 11,
    kErased             = OdGsNode::kLastFlag << 12,
    kDrawableIsXref     = OdGsNode::kLastFlag << 13,
    kMultilistNextPtr   = OdGsNode::kLastFlag << 14,
    kLastFlag           = kMultilistNextPtr
  };
  // m_entFlags (add all new flags related to EntityNode here, keep some free space inside m_flags for inherited classes)
  enum
  {
    kLineweightBit1     = (1 << 0), // reserve 8 bits for maximal lineweight
    kLineweightBit2     = (1 << 1), kLineweightOffset = 0,
    kLineweightBit3     = (1 << 2),
    kLineweightBit4     = (1 << 3),
    kLineweightBit5     = (1 << 4),
    kLineweightBit6     = (1 << 5),
    kLineweightBit7     = (1 << 6),
    kLineweightBit8     = (1 << 7),
    kLineweightBitMask  = kLineweightBit1 | kLineweightBit2 | kLineweightBit3 | kLineweightBit4 | kLineweightBit5 | kLineweightBit6 | kLineweightBit7 | kLineweightBit8,
    kLightSourceOwner   = (1 << 8), // owning at least one light source
    kRegenOnHighlight   = (1 << 9), // cache require regeneration during highlighting state change
    kMarkedByUpdateManager = (1 << 10),
    kSkipDisplayClipping = (1 << 11),
    kLastEntFlag        = kSkipDisplayClipping
  };
  // m_markToSkipFlags
  enum
  {
    kMarkedToSkipFirst  = 0x00000001,
    kMarkedToSkipLast   = kMarkedToSkipFirst << 30,
    kMarkedToSkipAll    = 0x7FFFFFFF
  };

public:
  ODRX_DECLARE_MEMBERS(OdGsEntityNode);

  /** \details
    Constructor for the OdGsEntityNode class.
    
    \param pModel [in]  Pointer to the BaseModel object.
    \param pUnderlyingDrawable [in]  Pointer to the underlying Drawable object.
    \param bSetGsNode [in]  Flag that controls whether to set GS node to the underlying drawable.
  */
  OdGsEntityNode(OdGsBaseModel* pModel, const OdGiDrawable* pUnderlyingDrawable, bool bSetGsNode = false);
public:
  /** \details
    Destructor for the OdGsEntityNode class.
  */
  ~OdGsEntityNode();

  /** \details
    Sets the next entity for this EntityNode object to the specified EntityNode object.
    \param pNextEntity [in]  Pointer to the next EntityNode.
  */
  void setNextEntity(OdGsEntityNode* pNextEntity);
  
  /** \details
    Sets the next entity for this EntityNode object to the specified EntityNode object.
    \param pNextEntity [in]  Pointer to the next EntityNode.
    \param nVpId [in]  Viewport ID.
  */
  void setNextEntity(OdUInt32 nVpId, OdGsEntityNode* pNextEntity);
  /** \details
    Returns the next entity for this EntityNode object.
  */
  OdGsEntityNode* nextEntity();
  
  /** \details
    Returns the next entity for this EntityNode object.
    \param nVpId [in]  Viewport ID.
  */
  OdGsEntityNode* nextEntity(OdUInt32 nVpId);
  /** \details
    Returns the next entity for this EntityNode object.
  */
  const OdGsEntityNode* nextEntity() const;
  
  /** \details
    Returns the next entity for this EntityNode object.
    \param nVpId [in]  Viewport ID.
  */
  const OdGsEntityNode* nextEntity(OdUInt32 nVpId) const;
  /** \details
    Returns number of next entities.
  */
  OdUInt32 numNextEntity() const;
  /** \details
    Enable multiple entity links.
    \param nMaxVpId [in]  Viewport ID.
  */
  void enableMultipleNextEntities(OdUInt32 nMaxVpId);

  /** \details
    Updates this entity node.
    \param ctx          [in]  Current update context.
    \param pParent      [in]  Pointer to a parent container node.
    \param pParentIndex [in]  Pointer to a parent spatial index.
  */
  void update(OdGsUpdateContext& ctx, OdGsContainerNode* pParent,
      OdSiSpatialIndex* pParentIndex);

  /** \details
    Updates visible content.
    \param pViewImpl    [in]  View in which content is visible.
  */
  void updateVisible( OdGsViewImpl* pViewImpl );
  
  /** \details
    Regenerates a metafile and returns a smart pointer to it.
    \param ctx         [in]  View in which content is visible.
    \param vpDepFlags  [in]  Viewport-dependent flags.
  */
  virtual OdGsEntityNode::MetafilePtr regenMetafile(OdGsUpdateContext &ctx, OdUInt32 vpDepFlags);

  /** \details
    Returns the Aware flags for the specified Viewport for this ContainerNode object.
    
    \param viewportId [in]  Viewport ID. 
    
    \remarks
    awareFlags() returns a combination of zero or more of the following:
    
    <table>
    Name                            Value
    kSuppressViewportDraw           0x01
    kDrawInvisibleEnts              0x02
    kDrawLayerOff                   0x04
    kDrawLayerFrozen                0x08
    kIgnoreFillPlane                0x10
    kFillPlaneSet                   0x20
    </table>
  */
  virtual OdUInt32 awareFlags(OdUInt32 viewportId) const;

  /** \details
    Retrieves the node type.
  */
  virtual ENodeType nodeType() const { return kEntityNode; }
  
  virtual bool isReference() const { return false; }
  virtual bool isLight() const { return false; }
  virtual bool isSelfReferential() const { return false; }

  /** \details
    Paints the specified view with the cached data associated with this Node object.
    \param ctx [in]  Current GS update context.
  */
  void display(OdGsDisplayContext& ctx);

  void select(OdGsBaseVectorizer& view);

  /** \details
    Invalidates this entity node.
    
    \param pParent  [in]  Parent container node.
    \param view     [in]  BaseVectorizeView object for which data are to be invalidated.
    \param mask     [in]  Mask.
  */
  void invalidate(OdGsContainerNode* pParent, OdGsViewImpl* pView, OdUInt32 mask);

  /** \details
    Returns the extents of the underlying GiDrawable object(s) associated with this Node object.
    
    \param extents [out]  Receives the extents.   
    \returns
    Returns true if and only if the GiDrawable object(s) have extents.
  */
  bool extents(OdGeExtents3d& extents) const;

  /** \details
    Returns true if and only if this EntityNode object is spatially indexed.
    \param nVpId [in]  Viewport ID.
  */
  bool spatiallyIndexed() const;
  
  /** \details
    Returns true if and only if this EntityNode object is spatially indexed.
    \param nVpId [in]  Viewport ID.
  */
  bool spatiallyIndexed(OdUInt32 nVpId) const;
  
  /** \details
    Controls the spatial indexing of this EntityNode object.

    \param spatiallyIndexed [in]  Controls spatially indexed.
  */
  void setSpatiallyIndexed(bool spatiallyIndexed);
  
  /** \details
    Controls the spatial indexing of this EntityNode object.
    \param nVpId [in]  Viewport ID.
    \param spatiallyIndexed [in]  Controls spatially indexed.
  */
  void setSpatiallyIndexed(OdUInt32 nVpId, bool spatiallyIndexed);

  /** \details
    Checks whether this EntityNode object is the child of a ContainerNode object.
    \returns 
    true if this EntityNode object is the child of a ContainerNode object.
  */
  bool owned() const;
  
  /** \details
    Checks whether this EntityNode object is the child of a ContainerNode object.    
    \param nVpId [in]  Viewport ID.
    \returns 
    true if this EntityNode object is the child of a ContainerNode object.
  */
  bool owned(OdUInt32 nVpId) const;
  
  /** \details
    Controls the owned status of this EntityNode object.

    \param owned [in]  Controls owned status.
  */
  void setOwned(bool owned);
  
  /** \details
    Controls the owned status of this EntityNode object.
    \param nVpId [in]  Viewport ID.
    \param owned [in]  Controls owned status.
  */
  void setOwned(OdUInt32 nVpId, bool owned);

  /** \details
    Returns true if and only if this EntityNode object is marked by Update Manager.
  */
  bool markedByUpdateManager() const;

  /** \details
    Controls the marking by Update Manager status of this EntityNode object.
    \param marked [in]  Controls marked status.
  */
  void setMarkedByUpdateManager( bool marked );

  /** \details
    Returns true if and only if the underlying GiDrawable object(s) have extents.
  */
  bool hasExtents() const;

  /** \details
    Returns extents of the underlying GiDrawable object(s).
  */
  const OdGeExtents3d& extents() const;

  /** \details
    Checks whether this node has no graphical content.
    \returns 
    true if the node has no graphical content, false otherwise.
  */
  virtual bool isEmpty();

  /** \details
    Checks whether this node has frozen layers.
    \returns 
    true if this node has frozen layers, false otherwise.
  */
  virtual bool hasFrozenLayers();

  /** \details
    Checks whether this node is marked to be skipped with the passed mask.
    \param mask [in]  Mask.
    \returns 
    true if this EntityNode object is marked to be skipped with the given mask (thread), false otherwise.
  */
  inline bool markedToSkip(unsigned mask);

  /** \details
    Controls the MarkToSkip status of this EntityNode object.
    \param markToSkip [in]  Controls MarkToSkip.
    \param mask [in]  the mask(thread).
  */
  inline void markToSkip(unsigned mask, bool markToSkip);

  /** \details
    Specifies to skip this node by all threads.
  */
  inline void markToSkipAll();

  /** \details
    Specifies an index of thread to skip this node.
  */
  static unsigned markToSkipMask(OdUInt32 threadIndex)
  {
    ODA_ASSERT(threadIndex >= 0 && threadIndex < 31);
    return kMarkedToSkipFirst << threadIndex;
  }

  /** \details
    Destroys this node.
  */
  void destroy(){}

  /** \details
    Checks whether at least one geometry portion of an metafile has a layer that changed.
    
    \param view   [in]  GS view.
    \returns
    true if one or more layers changed, false if all the layer are up to date.
  */
  virtual bool layersChanged(OdGsViewImpl& view) const;

  /** \details
    Checks whether this node is marked as non-selectable.
    \returns
    true if if this node is marked as non-selectable, false otherwise.
  */
  bool markedAsNonSelectable() const;
  
  /** \details
    Specifies whether this node should be non-selectable.
    \param val   [in]  Specifies whether this node is non-selectable.
  */
  void markAsNonSelectable(bool val);
  bool isMarkedSkipSelection() const;
  void markSkipSelection(bool val);
  
  /** \details
    Checks whether this node is marked as erased.
    \returns
    true if this node is marked as erased, false otherwise.
  */
  bool isMarkedErased() const;
  
  /** \details
    Marks this node as erased or not erased.
    
    \param val   [in]  Specifies whether to mark this node as erased.
  */
  void markErased(bool val);

  bool isInWorkset() const;
  void setInWorkset(bool val);

  /** \details
    Checks whether this node is regenerated on draw.
    \returns
    true if this node is regenerated on draw, false otherwise.
  */
  bool isRegenOnDraw() const;
  
  /** \details
    Specifies whether this node should be regenerated on draw.
    
    \param val   [in]  Specifies whether to regenerate this node on draw.
  */
  void setRegenOnDraw(bool val);

  /** \details
    Checks whether this node is updated in a single-threaded mode.
    \returns
    true if this node is updated in a single-threaded mode, false otherwise.
  */
  bool isSingleThreaded() const;
  
  /** \details
    Specifies whether this node should be updated in a single-threaded mode.
    
    \param val   [in]  Specifies whether to use single-threaded mode.
  */
  void setSingleThreaded(bool val);

  /** \details
    Checks whether this node skips clipping procedure.
    \returns
    true if this node skips clipping procedure, false otherwise.
  */
  bool skipDisplayClipping() const;
  
  /** \details
    Specifies whether this node should should skip clipping procedure.
    
    \param bOn   [in]  Specifies whether to skip clipping.
  */
  void setSkipDisplayClipping(bool bOn);

  /** \details
    Highlights or unhighlights this node and the branch of child node, if specified.
    
    \param bDoIt          [in]  Flag that specifies whether to highlight this entity node.
    \param bWholeBranch   [in]  Flag that specifies whether to highlight or unhighlight a branch of subentities and nested entities, if specified with setHlBranch method.
    \param nSelStyle      [in]  Optional selection style index.
  */
  virtual void highlight(bool bDoIt, bool bWholeBranch, OdUInt32 nSelStyle = 0);
  
  /** \details
    Sets a branch of subentities and nested entities to highlight or unhighlight.
    
    \param pBr          [in]  Pointer to a branch of subentities and nested entities.
  */
  void setHlBranch(OdGsStateBranch* pBr) { m_hlBranch = pBr; }
  
  /** \details
    Retrieves a branch of subentities and nested entities to highlight or unhighlight.
  */
  OdGsStateBranch* hlBranch() { return m_hlBranch; }
  
  /** \details
    Retrieves a branch of subentities and nested entities to highlight or unhighlight.
  */
  const OdGsStateBranch* hlBranch() const { return m_hlBranch; }
  /** \details
    Hides the specified node.
    \param bDoit [in] Specifies should node be hidden (true) or shown (false)
    \param bSelectable [in] Specifies should node be selectable if it is hidden
    \param bWholeBranch [in] Specifies should node hidden state be changed with all branches
  */
  void hide(bool bDoIt, bool bSelectable = false, bool bWholeBranch = true);
  /** \details
    Specifies node visibility branch.
  */
  void setVisibilityBranch( OdGsStateBranch* pBr ) { m_visBranch = pBr; }
  /** \details
    Retrieves node visibility branch.
  */
  OdGsStateBranch* visibilityBranch() { return m_visBranch; }
  /** \details
    Retrieves node visibility branch.
  */
  const OdGsStateBranch* visibilityBranch() const { return m_visBranch; }

  virtual void playAsGeometry(OdGsBaseVectorizer& view, EMetafilePlayMode eMode);

  /** \details
    Retrieves extents of this node object.
    \param pView [in]   Pointer to a GS view.
    \param ext   [out]  Reference to the OdGeExtents3d object which will receive current entity node extents.
  */
  bool extents(const OdGsView* pView, OdGeExtents3d& ext) const;
  
  /** \details
    Retrieves a maximal lineweight for this entity node.
    \returns
    lineweight in 100ths of a millimeter
  */
  OdDb::LineWeight getMaxLineweightUsed() const;
  
  /** \details
    Sets a maximal lineweight for this entity node if it is greater than current maximal lineweight for this entity node.
    
    \param lwd [in]   New lineweight value to compare and set if necessary.
  */
  void addMaxLineweightUsed(OdDb::LineWeight lwd);
  
  /** \details
    Sets a maximal lineweight for this entity node.
    
    \param lwd [in]   New lineweight value to set.
  */
  void setMaxLineweightUsed(OdDb::LineWeight lwd);

  /** \details
    Retrieves the entity unerased flag for this object.
    \returns
    true if the entity unerased flag is set, false otherwise.
  */
  bool entityUnerased() const;
  
  /** \details
    Sets the entity unerased flag for this object.
    
    \param flag [in]   Flag to set.
  */
  void setEntityUnerased(bool flag);

  /** \details
    Checks whether this entity node is invisible.
    \returns
    true if this entity node is invisible, false otherwise.
  */
  bool isInvisible() const;

  bool isExtentsOutOfModelSpace() const;
  void setExtentsOutOfModelSpace(bool flag);

  /** \details
    Sets the invalid viewport flag to false.
  */
  void resetInvalidVpFlag();

  /** \details
    Specifies whether this entity node is an owner of a light source.
    
    \param flag [in]   Flag to set.
  */
  void setAsLightSourceOwner(bool bSet);
  
  /** \details
    Checks whether this entity node is an owner of a light source.
    \returns
    true if this entity node is an owner of a light source, false otherwise.
  */
  bool isLightSourceOwner() const;

  /** \details
    Specifies whether cache requires a regeneration on a highlight state change.
    \returns
    true if cache needs to be regenerated during highlighting state change, false otherwise.
  */
  bool isRequireRegenOnHighlightChange() const;

public:
  virtual void makeStock();
  virtual void releaseStock();
  virtual void propagateLayerChangesStock();

  /** \details
    Stores current state of this entity node into filer object.

    \param pFiler       [out]  Pointer to a filer object.
    \param pVectorizer  [in/out]  Pointer to a vectorrizer. May be needed to save vectorizer client metafile (cache).
    \returns
    true if node state is successfully saved, false otherwise.
  */
  bool saveNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL) const;
  
  /** \details
    Restores node state from filer object.

    \param pFiler       [in]  Pointer to a filer object. Filer object.
    \param pVectorizer  [in]  Pointer to a vectorrizer. May be needed to load vectorizer client metafile (cache).
    \returns
    true if node state is successfully loaded, false otherwise.
  */
  bool loadNodeState(OdGsFiler *pFiler, OdGsBaseVectorizer *pVectorizer = NULL);
protected:
  bool findMetafileInStock(OdGsUpdateContext& ctx);
  void insertToSpatialIndex(OdUInt32 nVpId, OdSiSpatialIndex& parentIndex,
                            const OdGeExtents3d& prevExtents);
  inline void setExtentsFlags();

private:
  MetafileHolder m_mfStock;
  volatile int m_markToSkipFlags;
protected:
  // This flags willn't increase memory usage (at least on Win32) since utilize free space in 8 bytes alignment (__alignof(entity_node) == 8).
  OdUInt32 m_entFlags;
public:
  /** \details
    Adds content to the update manager
    \param viewportId [in]  Viewport ID.
    \param pManager   [in]  Pointer to a manager for which to add content.
    \param context    [in]  Update manager context.
  */
  void addContentToUpdateManager( OdUInt32 viewportId, OdGsUpdateManager* pManager, const UpdateManagerContext& context );
  
  /** \details
    Checks whether the passed views have compatible metafiles.
    \param pView1   [in]  Pointer to the first implementation view to compare metafiles.
    \param pView12  [in]  Pointer to the second implementation view to compare metafiles.
    \param ctx      [in]  GS update context.
  */
  bool isMetafilesCompatible( const OdGsViewImpl* pView1, const OdGsViewImpl* pView2, OdGsUpdateContext* ctx );
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsEntityNode object pointers. 
*/
typedef OdSmartPtr<OdGsEntityNode> OdGsEntityNodePtr;


inline void OdGsEntityNode::setNextEntity(OdGsEntityNode* pNextEntity)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  m_pNextEntity = pNextEntity;
}

inline void OdGsEntityNode::setNextEntity(OdUInt32 nVpId, OdGsEntityNode* pNextEntity)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    nextEntEntry(nVpId)->m_pNextEntity = pNextEntity;
  else
    m_pNextEntity = pNextEntity;
}

inline void OdGsEntityNode::enableMultipleNextEntities(OdUInt32 nMaxVpId)
{
  if (nMaxVpId == 0xFFFFFFFF) nMaxVpId = 0;
  if ((numNextEntity() <= nMaxVpId) || ((nMaxVpId == 0) && GETBIT(m_flags, kMultilistNextPtr)))
    allocNextEntityArray(nMaxVpId);
}

inline OdGsEntityNode* OdGsEntityNode::nextEntity()
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  return m_pNextEntity;
}

inline OdGsEntityNode* OdGsEntityNode::nextEntity(OdUInt32 nVpId)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return nextEntEntry(nVpId)->m_pNextEntity;
  //if (nVpId > 0) // Normal case, since we can request from non-VpDep container
  //  throw OdError(eInvalidIndex);
  return m_pNextEntity;
}

inline const OdGsEntityNode* OdGsEntityNode::nextEntity() const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  return m_pNextEntity;
}

inline const OdGsEntityNode* OdGsEntityNode::nextEntity(OdUInt32 nVpId) const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return nextEntEntry(nVpId)->m_pNextEntity;
  //if (nVpId > 0) // Normal case, since we can request from non-VpDep container
  //  throw OdError(eInvalidIndex);
  return m_pNextEntity;
}

inline OdUInt32 OdGsEntityNode::numNextEntity() const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return (OdUInt32)OdPtrToInt32(firstEntEntry()->m_pNextEntity);
  return 1;
}

inline bool OdGsEntityNode::hasExtents() const
{
  return GETBIT(m_flags, kHasExtents);
}

inline const OdGeExtents3d& OdGsEntityNode::extents() const
{
  return m_extents;
}

inline bool OdGsEntityNode::markedToSkip(unsigned mask)
{
  ODA_ASSERT((mask >= kMarkedToSkipFirst) && (mask <= kMarkedToSkipLast));
  //atomic function is required, called in MT mode (display)
#ifndef TD_SINGLE_THREAD
  return (mask & OdInterlockedExchangeAdd(&m_markToSkipFlags, 0)) != 0;
#else
  return GETBIT(m_markToSkipFlags, mask);
#endif
}

inline void OdGsEntityNode::markToSkip(unsigned mask, bool bSkip)
{
  ODA_ASSERT((mask >= kMarkedToSkipFirst) && (mask <= kMarkedToSkipLast));
  ODA_ASSERT(mask && !(mask & (mask-1))); // this function does not accept multi-bit masks
  if(markedToSkip(mask) != bSkip)
  {
    //atomic function is required, called in MT mode (display)
#ifndef TD_SINGLE_THREAD
    if(bSkip)
      OdInterlockedExchangeAdd(&m_markToSkipFlags, mask);
    else
      OdInterlockedExchangeAdd(&m_markToSkipFlags, -(int)mask);
#else
    SETBIT(m_markToSkipFlags, mask, bSkip);
#endif
  }
}

inline void OdGsEntityNode::markToSkipAll()
{
  //atomic function is NOT required, is NOT called in MT mode
  m_markToSkipFlags = kMarkedToSkipAll;
}

inline bool OdGsEntityNode::spatiallyIndexed() const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  return GETBIT(m_flags, kSpatiallyIndexed);
}

inline bool OdGsEntityNode::spatiallyIndexed(OdUInt32 nVpId) const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return GETBIT(nextEntEntry(nVpId)->m_entFlags, kSpatiallyIndexed);
  //if (nVpId > 0) // Normal case, since we can request from non-VpDep container
  //  throw OdError(eInvalidIndex);
  return GETBIT(m_flags, kSpatiallyIndexed);
}

inline void OdGsEntityNode::setSpatiallyIndexed(bool bIndexed)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  SETBIT(m_flags, kSpatiallyIndexed, bIndexed);
}

inline void OdGsEntityNode::setSpatiallyIndexed(OdUInt32 nVpId, bool bIndexed)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    SETBIT(nextEntEntry(nVpId)->m_entFlags, kSpatiallyIndexed, bIndexed);
  else
    SETBIT(m_flags, kSpatiallyIndexed, bIndexed);
}

inline bool OdGsEntityNode::owned() const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  return GETBIT(m_flags, kOwned);
}

inline bool OdGsEntityNode::owned(OdUInt32 nVpId) const
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    return GETBIT(nextEntEntry(nVpId)->m_entFlags, kOwned);
  //if (nVpId > 0) // Normal case, since we can request from non-VpDep container
  //  throw OdError(eInvalidIndex);
  return GETBIT(m_flags, kOwned);
}

inline void OdGsEntityNode::setOwned(bool bOwned)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    throw OdError(eInvalidIndex);
  SETBIT(m_flags, kOwned, bOwned);
}

inline void OdGsEntityNode::setOwned(OdUInt32 nVpId, bool bOwned)
{
  if (GETBIT(m_flags, kMultilistNextPtr))
    SETBIT(nextEntEntry(nVpId)->m_entFlags, kOwned, bOwned);
  else
    SETBIT(m_flags, kOwned, bOwned);
}

inline OdGsEntityNode::MetafilePtrArray& OdGsEntityNode::metafileArray()
{
  return m_metafile.getArray();
}

inline const OdGsEntityNode::MetafilePtrArray& OdGsEntityNode::metafileArray() const
{
  return m_metafile.getArray();
}

inline OdGsEntityNode::MetafilePtr OdGsEntityNode::metafile() const
{
  return m_metafile.get();
}

inline OdGsEntityNode::MetafilePtr OdGsEntityNode::metafileAt(int metafileIndex) const
{
  return metafileArray()[metafileIndex];
}

#define VPMETAFILEHOLDERARRAY_DESTRUCTOR ~MetafilePtrArray

inline void OdGsEntityNode::setMetafile(OdGsEntityNode::Metafile* pMetafile)
{
  m_metafile.set(pMetafile);
}

inline bool OdGsEntityNode::entityUnerased() const
{
  return GETBIT(m_flags, kEntityUnerased);
}

inline void OdGsEntityNode::setEntityUnerased(bool flag)
{
  SETBIT(m_flags, kEntityUnerased, flag);
}

inline bool OdGsEntityNode::isInvisible() const
{
  return GETBIT(m_flags, kInvisible);
}

inline void OdGsEntityNode::resetInvalidVpFlag()
{
  if (invalidVp())
  {
    setInvalidVp(false);
  }
}

inline bool OdGsEntityNode::markedAsNonSelectable() const
{
  return GETBIT(m_flags, kNonSelectable);
}

inline void OdGsEntityNode::markAsNonSelectable(bool val)
{
  SETBIT(m_flags, kNonSelectable, val);
}

inline bool OdGsEntityNode::isMarkedSkipSelection() const
{
  return GETBIT(m_flags, kSkipSelection);
}

inline void OdGsEntityNode::markSkipSelection(bool val)
{
  SETBIT(m_flags, kSkipSelection, val);
}

inline bool OdGsEntityNode::isMarkedErased() const
{
  return GETBIT(m_flags, kErased);
}

inline void OdGsEntityNode::markErased(bool val)
{
  SETBIT(m_flags, kErased, val);
}

inline bool OdGsEntityNode::Metafile::isEmpty() const
{
  for(const OdGsGeomPortion* ptr = &m_first; ptr; ptr = ptr->m_pNext)
  {
    if(ptr->m_pGsMetafile.get())
      return false;
  }
  return true;
}

inline bool OdGsEntityNode::isExtentsOutOfModelSpace() const
{
  return GETBIT(m_flags, kExtentsOutOfMS);
}

inline void OdGsEntityNode::setExtentsOutOfModelSpace(bool val)
{
  SETBIT(m_flags, kExtentsOutOfMS, val);
}

inline void OdGsEntityNode::setAsLightSourceOwner(bool bSet)
{
  SETBIT(m_entFlags, kLightSourceOwner, bSet);
}

inline bool OdGsEntityNode::isLightSourceOwner() const
{
  return GETBIT(m_entFlags, kLightSourceOwner);
}

inline bool OdGsEntityNode::isRequireRegenOnHighlightChange() const
{
  return GETBIT(m_entFlags, kRegenOnHighlight);
}

inline bool OdGsEntityNode::isInWorkset() const
{
  return GETBIT(m_flags, kInWorkset);
}

inline void OdGsEntityNode::setInWorkset(bool val)
{
  SETBIT(m_flags, kInWorkset, val);
}

inline bool OdGsEntityNode::isSingleThreaded() const
{
  return GETBIT(m_flags, kSingleThreaded);
}

inline void OdGsEntityNode::setSingleThreaded(bool val)
{
  SETBIT(m_flags, kSingleThreaded, val);
}

inline bool OdGsEntityNode::isRegenOnDraw() const
{
  return GETBIT(m_flags, kDrawableRegenDraw);
}

inline void OdGsEntityNode::setRegenOnDraw(bool val)
{
  SETBIT(m_flags, kDrawableRegenDraw, val);
}

inline void OdGsEntityNode::highlight(bool bDoIt, bool bWholeBranch, OdUInt32 nSelStyle)
{
  OdGsNode::highlight(bDoIt, bWholeBranch, nSelStyle);
  if(hlBranch() && (!bDoIt || bWholeBranch))
    setHlBranch(NULL);
}

inline void OdGsEntityNode::hide(bool bDoIt, bool bSelectable, bool bWholeBranch)
{
  OdGsNode::hide(bDoIt, bSelectable, bWholeBranch);
  if( visibilityBranch() && (!bDoIt || bWholeBranch) )
    setVisibilityBranch(NULL);
}

inline void OdGsEntityNode::setExtentsFlags()
{
  SETBIT(m_flags, kHasExtents, m_extents.isValidExtents());
  SETBIT(m_flags, kExtentsOutOfMS, !OdSi::properExtents(m_extents));
}

inline OdDb::LineWeight OdGsEntityNode::getMaxLineweightUsed() const
{
  return (OdDb::LineWeight)((m_entFlags & kLineweightBitMask) >> kLineweightOffset);
}

inline void OdGsEntityNode::addMaxLineweightUsed(OdDb::LineWeight lwd)
{
  if (lwd)
  {
    OdDb::LineWeight maxLwd = getMaxLineweightUsed();
    if (lwd > maxLwd)
      setMaxLineweightUsed(lwd);
  }
}

inline void OdGsEntityNode::setMaxLineweightUsed(OdDb::LineWeight lwd)
{
  m_entFlags = ((OdUInt32(lwd) << kLineweightOffset) & kLineweightBitMask) | (m_entFlags & ~kLineweightBitMask);
}

inline bool OdGsEntityNode::markedByUpdateManager() const
{
  return GETBIT(m_entFlags, kMarkedByUpdateManager);
}

inline void OdGsEntityNode::setMarkedByUpdateManager(bool bMarked)
{
  SETBIT(m_entFlags, kMarkedByUpdateManager, bMarked);
}

inline bool OdGsEntityNode::skipDisplayClipping() const
{
  return GETBIT(m_entFlags, kSkipDisplayClipping);
}

inline void OdGsEntityNode::setSkipDisplayClipping(bool bOn)
{
  SETBIT(m_entFlags, kSkipDisplayClipping, bOn);
}

#include "TD_PackPop.h"

#endif // __OD_GS_ENTITY_NODE__
