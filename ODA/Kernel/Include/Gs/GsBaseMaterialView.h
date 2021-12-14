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

#ifndef __OD_GS_BASE_MATERIAL_VIEW__
#define __OD_GS_BASE_MATERIAL_VIEW__

#include "Gs/GsBaseInclude.h"
#include "Gs/GsExtAccum.h"
#include "Gi/GiMapperItem.h"
#include "Gi/GiMaterialItem.h"
#include "Gi/GiLightTraitsData.h"

#include "TD_PackPush.h"

class OdGsMaterialNode;

/** <group !!RECORDS_TD_APIRef>
*/
struct OdGsMaterialTextureDataFiler
{
  /** \details
    Saves client material texture data to the specified filer. Currently not implemented.
    \param pTextureData [in]  Material texture data to save. Not used.
    \param pFiler [in]  Filer where to save material texture data. Not used.
    \returns
    false.
  */
  virtual bool saveClientMaterialTextureData(const OdGiMaterialTextureData * /*pTextureData*/, OdGsFiler * /*pFiler*/)
  {
    return false;
  }
  
  /** \details
    Loads client material texture data from the specified filer. Currently not implemented.
    \param pFiler [in]  Filer to load material texture data from. Not used.
  */
  virtual OdGiMaterialTextureDataPtr loadClientMaterialTextureData(OdGsFiler * /*pFiler*/)
  {
    return OdGiMaterialTextureDataPtr();
  }
};

/** \details
    Lights data storage.
    
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
struct GS_TOOLKIT_EXPORT OdGsLightsAccumulationContainter
{
  typedef OdVector<OdGiLightTraitsData *> LightDataVector;
  OdVector<OdGiLightTraitsData *> m_accumulatedLightsData;
  OdMutexPtr m_lightsAccumMutex;
  OdGsLightsAccumulationContainter() {}
  ~OdGsLightsAccumulationContainter() { clear(); }
  bool has() const { return !m_accumulatedLightsData.isEmpty(); }
  void append(bool bMtSync, OdGiLightTraitsData *pLightData);
  void append(bool bMtSync, OdGsLightsAccumulationContainter &lightsAccum);
  void clear();
};

/** \details
    This class provides material support for OdGsBaseVectorizeView.

    \remarks
    OdGsBaseMaterialView works only with diffuse material/mapping channel and not applicable for
    complete rendering devices.
    
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseMaterialVectorizer : public OdGsBaseVectorizer, public OdGsMaterialTextureDataFiler
{
  public:
    enum MaterialViewFlags
    {
      kProcessMappers               = 1,        // Process material mappers
      kProcessMaterials             = (1 << 1), // Process materials

      kProcessMappersAndMaterials   = 3,        // Process both material mappers and materials

      kMappersForRender             = (1 << 2), // OdGiMapperRenderItem instead of OdGiMapperItem
      kMaterialsForRender           = (1 << 3), // OdGiMaterialRenderItem instead of OdGiMaterialItem

      kMappersAndMaterialsForRender = 12,       // kMappersForRender | kMaterialsForRender

      kProcessForRender             = 15,       // kProcessMappersAndMaterials | kMappersAndMaterialsForRender

      // #5127 fix. Delay cache implementation.
      kEnableDelayCache             = (1 << 4)  // Enable cache, which drop extents data after drawable extents computation
    };

    // Cache structures
    /** \details
      Represents delay cache entry type.
    */
    enum DelayCacheEntryType
    {
      kInternal = 0,  // Internal.
      kUser           // User-defined.
    };
    /** \details
      Represents delay cache entry.
    */
    struct DelayCacheEntry
    {
      /** \details
        Default constructor for the DelayCacheEntry structure. Sets pointer to the next entry to NULL.
      */
      DelayCacheEntry() : m_pNext(NULL) { }
      
      /** \details
        Destructor for the DelayCacheEntry structure.
      */
      virtual ~DelayCacheEntry() { }

      /** \details
        Pointer to the next delay cache entry.
      */
      DelayCacheEntry *m_pNext;

      /** \details
        Returns internal type of a cache entry.
        \remarks
        Don't return kInternal here if you want to get callback.
      */
      virtual DelayCacheEntryType internalType() const { return kUser; }

      /** \details
        Returns pointer to the next delay cache entry.
      */
      DelayCacheEntry *nextEntry() { return m_pNext; }
      
      /** \details
        Sets next delay cache entry.
        \param pNext [in]  Pointer to the next delay cache entry. 
      */
      void setNextEntry(DelayCacheEntry *pNext) { m_pNext = pNext; }

      virtual void play(OdGsBaseMaterialVectorizer &mView) const
      {
        mView.playDelayCacheEntry(this);
      }
    };
    friend class OdDefDelayCacheEntryMap;
    friend class OdManageMViewPerDrawableData;
  private:
    enum MaterialViewRuntimeFlags
    {
      kMaterialCommited    = (1 << 0), // Material already emitted by GsModel
      kAccumNonCacheLights = (1 << 1), // Accumulate lights which is not relate to GsModel
      kAccumCacheLights    = (1 << 2), // Accumulate GsModel-resident lights
      kAccumLights = kAccumNonCacheLights | kAccumCacheLights
    };
    struct ExtentsContainer
    {
      const OdGiDrawable *m_pCurrentDrawable;
      OdGeExtents3d       m_Extents;
      enum Flags
      {
        kExtentsValid        = (1 << 0),
        kExtentsAwaitingCalc = (1 << 1),
        kLastFlag            = kExtentsAwaitingCalc
      };
      ExtentsContainer() : m_pCurrentDrawable(NULL) {}
      void clear() { m_pCurrentDrawable = NULL; }
    };
    struct DelayCache
    {
      const OdGiMapper *m_pCurMapper,   *m_pCopyMapper;
      const OdDbStub   *m_pCurMaterial, *m_pCopyMaterial;
      DelayCacheEntry   m_Cache;
      DelayCacheEntry  *m_pTail;

      DelayCache() : m_pCurMapper(NULL), m_pCopyMapper(NULL)
                   , m_pCurMaterial(NULL), m_pCopyMaterial(NULL)
                   , m_pTail(&m_Cache)
      {
      }

      void clear();
    };
    struct MViewPerDrawableData : public ExtentsContainer
    {
      DelayCache   m_DelayCache;
      OdGeMatrix3d m_ModelTransform;
      OdUInt32     m_flags;
      enum Flags
      {
        kMapperNeedExtents = (ExtentsContainer::kLastFlag << 1), // Mapper awaiting current drawable extents
        kBaseLevel         = (ExtentsContainer::kLastFlag << 2),
        kHasModelTransform = (ExtentsContainer::kLastFlag << 3)
      };
      MViewPerDrawableData()
        : m_flags(0)
      {
      }
      ~MViewPerDrawableData()
      {
      }
      void clear()
      {
        ExtentsContainer::clear();
        SETBIT_0(m_flags, ~kBaseLevel);
      }
    } *m_pCurMvDrawableData, m_mvBaseDrawableData, m_mvGlobalDrawableDataInitializer;
  protected:
    OdUInt32            m_uMaterialViewFlags;   // Set of material view flags
    OdUInt32            m_uMaterialViewRtFlags; // Runtime flags
    OdGiMaterialItemPtr m_pCurrentMaterialItem; // Current material
    OdGiMapperItemPtr   m_pCurrentMapperItem;   // Current mapper
    OdGiMaterialTraitsData m_nullMaterialTraitsData;
    OdGiMaterialItemPtr m_pNullMaterialItem;
    OdGsLightsAccumulationContainter m_lightsAccum;
  public:
    /** \details
      Default constructor for the OdGsBaseMaterialVectorizer class.
      \param uMaterialViewFlags [in]  Material flag for a view.
    */
    explicit OdGsBaseMaterialVectorizer(OdUInt32 uMaterialViewFlags = kProcessMappersAndMaterials);
    
    /** \details
      Destructor for the OdGsBaseMaterialVectorizer class.
    */
    ~OdGsBaseMaterialVectorizer();

    // OdGsBaseVectorizerView overrides

    /** \details
      Called whenever material view traits are changed.
    */
    virtual void onTraitsModified();

    /** \details
      Begins processing a metafile.
      \param pMetafile [in]  Pointer to a persistent object identifier of a metafile.
    */
    virtual void beginMetafile(OdRxObject* pMetafile);
    
    /** \details
      Ends processing a metafile.
      \param pMetafile [in]  Pointer to persistent object identifier of a metafile.
    */
    virtual void endMetafile(OdRxObject* pMetafile);
    
    /** \details
      Begins view vectorization.
    */
    virtual void beginViewVectorization();
    
    /** \details
      Ends view vectorization.
    */
    virtual void endViewVectorization();

    /** \details
      Processes a material node.
      \param materialId [in]  Pointer to persistent object identifier of a material.
      \param node [in]  Pointer to a material node.
    */
    virtual void processMaterialNode(OdDbStub *materialId, OdGsMaterialNode *node);
    
    /** \details
      Saves material cache to the specified filer.
      \param pNode [in]  Pointer to a material node.
      \param pFiler [in]  Pointer to a filer where to save the material item.
      \returns
      true if material cache is successfully saved, false otherwise.
    */
    virtual bool saveMaterialCache(const OdGsMaterialNode *pNode, OdGsFiler *pFiler);
    
    /** \details
      Loads material cache from the specified filer.
      \param pNode [in]  Pointer to a material node.
      \param pFiler [in]  Pointer to a filer where to load the material item from.
      \returns
      true if material cache is successfully loaded, false otherwise.
    */
    virtual bool loadMaterialCache(OdGsMaterialNode *pNode, OdGsFiler *pFiler);

    /** \details
      Draws the specified drawable.
      \param drawableFlags [in]  Drawable flags.
      \param pDrawable [in]  Pointer to the drawable to draw.
      \returns
      true if drawable is successfully drawn, false otherwise.
    */
    virtual bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable *pDrawable);

    // Lights-related functionality

    void beginLightsAccumulation(bool bAccumNonCached = true, bool bAccumCached = false, bool bClear = false);
    void playAccumulatedLights(OdGsLightsAccumulationContainter *pAccumLights = NULL, bool bClear = true);
    const OdGsLightsAccumulationContainter &getAccumulatedLights() const { return m_lightsAccum; }
    OdGsLightsAccumulationContainter &getAccumulatedLights() { return m_lightsAccum; }
    bool isLightsAccumulation() const
    { return (GETBIT(m_uMaterialViewRtFlags, kAccumNonCacheLights) && !GETBIT(m_flags, kModelCache)) ||
             (GETBIT(m_uMaterialViewRtFlags, kAccumCacheLights)    &&  GETBIT(m_flags, kModelCache)); }

    /** \details
      Adds a point light source.
    */
    virtual void addPointLight(const OdGiPointLightTraitsData&);
    
    /** \details
      Adds a spot light source.
    */
    virtual void addSpotLight(const OdGiSpotLightTraitsData&);
    
    /** \details
      Adds a distance light source.
    */
    virtual void addDistantLight(const OdGiDistantLightTraitsData&);
    
    /** \details
      Adds a web light source.
    */
    virtual void addWebLight(const OdGiWebLightTraitsData&);

    // OdGsBaseMaterialView methods

    /** \details
      Returns current material item.
    */
    OdGiMaterialItemPtr currentMaterial() const;
    /** \details
      Sets current material item.
      \param pMaterial [in]  New material item.
    */
    void resetCurrentMaterial(OdGiMaterialItemPtr pMaterial);
    /** \details
      Checks whether material support is enabled.
      \returns 
      true if materials support enabled, false otherwise.
    */
    bool isMaterialEnabled() const;
    /** \details
      Checks whether current material is available.
      \returns 
      true if current material is available, false otherwise.
    */
    bool isMaterialAvailable() const;
    /** \details
      Removes current material item.
    */
    void resetCurrentMaterial();
    /** \details
      Returns current material mapper item.
      \param bForCoords [in]  Set to true if mapper item pointer will be used for texture coordinates calculations.
    */
    OdGiMapperItemPtr currentMapper(bool bForCoords = true) const;
    /** \details
      Sets current material mapper item.
      \param pMapper [in]  New material mapper item.
    */
    void resetCurrentMapper(OdGiMapperItemPtr pMapper);
    /** \details
      Checks whether mappers are supported. 
      \returns 
      true if mappers support enabled, false otherwise.
    */
    bool isMapperEnabled() const;
    /** \details
      Checks whether current mapper item is available.
      \returns 
      true if current mapper item available, false otherwise.
    */
    bool isMapperAvailable() const;

    /** \details
      Checks whether the material mapping is delayed.
      \returns 
      true if delay cache is enabled and drawable extents is not available, false otherwise.
    */
    bool isMappingDelayed() const;

    // Output callbacks

    /** \details
      Checks whether this object can be overrided by device for setting up additional transform matrix to material mapper.
      \param dm [out]  Transform matrix.
      \returns
      true if transform matrix is initialized, false otherwise.
    */
    virtual bool getDeviceMapperMatrix(OdGeMatrix3d &dm) const;
    /** \details
      Checks whether the current material needs to be skipped.
      \param materialId [in]  Material id.
      \returns
      true if material needs to be skipped.
      As default returns true for 2d optimized, wireframe and hidden render modes and if materialId is null.
    */
    virtual bool skipMaterialProcess(OdDbStub *materialId) const;
    /** \details
      Can be overrided by device for process null materials.
    */
    virtual void nullMaterialStub();
    /** \details
      Called by OdGsBaseMaterialView for initialize/reinitialize material cache.
      \param prevCache [in]  Previous cache, if available.
      \param materialId [in]  Material id.
      \param materialData [in]  Current material traits.
      \returns
      As default returns null pointer.
    */
    virtual OdGiMaterialItemPtr fillMaterialCache(OdGiMaterialItemPtr prevCache, OdDbStub *materialId, const OdGiMaterialTraitsData &materialData);
    /** \details
      Called by OdGsBaseMaterialView for render material cache.
      \param pCache [in]  Material cache.
      \param materialId [in]  Material id.
      \remarks
      As default does nothing.
    */
    virtual void renderMaterialCache(OdGiMaterialItemPtr pCache, OdDbStub *materialId);

    /** \details
      Called by delay cache entry as default handler.
      \param pEntry [in]  Pointer to caller entry.
    */
    virtual void playDelayCacheEntry(const DelayCacheEntry *pEntry);
    /** \details
      Sets user entry to delay cache.
      \param pEntry [in]  Pointer to newly created cache entry.
    */
    void appendDelayCacheEntry(DelayCacheEntry *pEntry);
    /** \details
      Called by delay cache writer if mapping entry was changed.
      \remarks
      Can be used for link cache markers with texture coordinates which must be recalculated after extents
      will be calculated.
    */
    virtual void mapperChangedForDelayCache();
    /** \details
      Called by delay cache writer after execution of all cache entries.
      \remarks
      Can be overrided for clear device's internal caches. If you want clear caches before execution,
      you could override computeDelayedExtents method instead of delayCacheProcessed.
    */
    virtual void delayCacheProcessed(const OdGiDrawable *pDrawable);

    /** \details
      Saves material item to the specified filer.
      \param pMatItem [in]  Material item to save.
      \param pFiler [out]  Filer where to save material item.
      \returns
      true if all material texture entries are successfully saved (including cached data, if exists), false otherwise.
    */
    bool saveMaterialItem(const OdGiMaterialItem *pMatItem, OdGsFiler *pFiler);
    
    /** \details
      Loads material item from the specified filer and returns a smart pointer to the loaded material item.
      \param pFiler [in]  Filer to load material item from.
    */
    OdGiMaterialItemPtr loadMaterialItem(OdGsFiler *pFiler);
    
    /** \details
      Saves material texture to the specified filer.
      \param pMatItem [in]  Material texture to save.
      \param pFiler [out]  Filer where to save material texture.
      \returns
      true when material texture data is saved. If the specified pTexture does not point to material texture data, then 0 is written to the filer and methods also returns true.
    */
    static bool saveMaterialTexture(const OdGiMaterialTexture *pTexture, OdGsFiler *pFiler);
    
    /** \details
      Loads material texture from the specified filer.
      \param pFiler [in]  Filer to load material texture from.
    */
    static OdGiMaterialTexturePtr loadMaterialTexture(OdGsFiler *pFiler);
    
    /** \details
      Saves material texture manager to the specified filer.
      \param pManager [in]  Material texture manager from which data is saved.
      \param pFiler [out]  Filer where to save material textures.
      \param pSaver [in]  Filer from which to read client material texture data.
      \returns
      true if all material texture data from the manager is successfully saved (including client material texture data, if exists), false otherwise.
    */
    static bool saveMaterialTextureManager(const OdGiMaterialTextureManager *pManager, OdGsFiler *pFiler, OdGsMaterialTextureDataFiler *pSaver);
    
    /** \details
      Loads material texture manager from the specified filer.
      \param pManager [in/out]  Material texture manager to which data is saved.
      \param pFiler [in]  Filer to read material textures from.
      \param pSaver [in/out]  Filer where client material texture data is saved (if exists).
      \returns
      true if all material texture data is successfully loaded to the texture manager, false otherwise.
    */
    static bool loadMaterialTextureManager(OdGiMaterialTextureManager *pManager, OdGsFiler *pFiler, OdGsMaterialTextureDataFiler *pSaver);
    
    /** \details
      Saves client material cached data to the filer. Currently not implemented.
      \param pMtl [in]  Client material cached data. Not used.
      \param pFiler [out]  Filer to where data is saved. Not used.
      \returns
      false.
    */
    virtual bool saveClientMaterialCache(const OdRxObject* pMtl, OdGsFiler *pFiler);
    
    /** \details
      Loads client material cached data to the filer. Currently not implemented.
      \param pFiler [in]  Client material cached data. Not used.
      \param pMatItem [out]  Material item. Not used.
      \returns
      Generic OdRxObjectPtr smart pointer.
    */
    virtual OdRxObjectPtr loadClientMaterialCache(OdGsFiler *pFiler, OdGiMaterialItem *pMatItem);
  protected:
    /** \details
      Called internally if extents needed for current drawable (delay cache disabled).
      \param pDrawable [in]  Pointer to current drawable.
      \param extents [out]  Computed extents.
      \remarks
      Returns true if and only if computed extents valid.
      Can be overrided by device if it needs more fast implementation.
    */
    virtual bool computeDrawableExtents(const OdGiDrawable *pDrawable, OdGeExtents3d &extents);
    /** \details
      Called internally if extents needed for current drawable (if delay cache enabled).
      \param pDrawable [in]  Pointer to current drawable.
      \param extents [out]  Computed extents.
      \remarks
      Returns true if and only if computed extents valid.
      Can be overrided by device if it needs more fast implementation.
    */
    virtual bool computeDelayedExtents(const OdGiDrawable *pDrawable, OdGeExtents3d &extents);
  private:
    void updateMViewModelTransform();
    void computeDrawableExtents();
    bool tryGsModelExtents(OdGeExtents3d &extents) const;
  protected:
    OdGiMapperItemPtr _itlCreateMapperItem() const;
    OdGiMaterialItemPtr _itlCreateMaterialItem() const;
};

/** \details
    Implements OdGsBaseMaterialVectorizer pseudo constructor.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseMaterialViewPC : public OdGsBaseMaterialVectorizer
{
  protected:
    void resetConstructorFlags(OdUInt32 uMaterialViewFlags);
  public:
    /** \details
      Default constructor for the OdGsBaseMaterialViewPC class.
    */
    OdGsBaseMaterialViewPC() { }
};

/** \details
    Joins OdGsBaseVectorizeView and OdGsBaseMaterialVectorizer classes with default behavior.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseMaterialView : public OdGsBaseVectorizeViewJoin<OdGsBaseMaterialViewPC>
{
  public:
    explicit OdGsBaseMaterialView(OdUInt32 uMaterialViewFlags = kProcessMappersAndMaterials)
    {
      resetConstructorFlags(uMaterialViewFlags);
    }
};

/** \details
    Joins OdGsBaseVectorizeView and OdGsBaseMaterialVectorizer classes with multithread behavior.

    Corresponding C++ library: Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGsBaseMaterialViewMT : public OdGsBaseVectorizeViewJoinMT<OdGsBaseMaterialViewPC>
{
  public:
    /** \details
      Constructor for the OdGsBaseMaterialViewMT class. Creates an object with the specified flags.
    */
    explicit OdGsBaseMaterialViewMT(OdUInt32 uMaterialViewFlags = kProcessMappersAndMaterials)
    {
      resetConstructorFlags(uMaterialViewFlags);
    }

    /** \details
      Checks whether view is multithreaded.
      \returns
      true if this view is multithreaded, false otherwise.
    */
    bool isMTView() const
    {
      return (&view() != this);
    }
};

#include "TD_PackPop.h"

#endif // __OD_GS_BASE_MATERIAL_VIEW__
