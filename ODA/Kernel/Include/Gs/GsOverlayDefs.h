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

#ifndef __GSOVERLAYDEFS_H_INCLUDED_
#define __GSOVERLAYDEFS_H_INCLUDED_

#include "OdArray.h"

#include "TD_PackPush.h"

/** \details
  This enumeration contains similar definitions as inside OdGsModel::RenderType, but fundamental difference is that
  values inside OdGsOverlayId is given in priority order instead of rendering order. Priority order is more flexible
  for type definitions, arrays, flags and internal structures. We can use mapping tables to provide conversions
  between priority and rendering orders.

  <group OdGs_Classes>
  \sa
  <link tv_graphSysOverlays, Work with Graphic System Overlays>
*/
enum OdGsOverlayId
{
  kGsMainOverlay = 0,           // "Main" scene overlay
  kGsDirectOverlay,             // "Direct" overlay
  kGsUserFg3Overlay,            // "User Foreground 3" overlay
  kGsUserBg1Overlay,            // "User Background 1" overlay
  kGsUserBg3Overlay,            // "User Background 3" overlay
  kGsUserFg1Overlay,            // "User Foreground 1" overlay
  kGsHighlightOverlay,          // "Highlight" overlay
  kGsHighlightSelectionOverlay, // "Highlight Selection" overlay
  kGsDirectTopmostOverlay,      // "Direct Topmost" overlay
  kGsSpriteOverlay,             // "Sprite" overlay
  kGsContrastOverlay,           // "Contrast" overlay
  kGsUserFg2Overlay,            // "User Foreground 2" overlay
  kGsUserBg2Overlay,            // "User Background 2" overlay
  kNumGsOverlays,               // Number of Gs overlays
  kGsAllOverlays = 0x1FFF,      // Mask for all overlays
  kGsNoOverlays  = 0,           // Mask for no overlays
  kGsUndefinedOverlay = -1      // Special code for undefined overlay
};

/** \details
  Provides mapping between overlays priority order and rendering order representations.

  <group OdGs_Classes>
*/
struct OdGsOverlayMapping
{
  protected:
    enum OverlayRenderOrder
    {
      kUserBg1 = 0, kUserBg2, kUserBg3,
      kMain, kSprite, kDirect, kHighlight, kHighlightSelection, kDirectTopmost, kContrast,
      kUserFg1, kUserFg2, kUserFg3,
      kNumOverlays
    };
  public:
    /** \details
      Overlays classification flags.
    */
    enum OverlayTypeDefs
    {
      kMainDepth     = 0, // Use main depth buffer
      kNoDepth       = 1, // Don't use depth buffer
      kOwnDepth      = 2, // Use own (alternative) depth buffer
      kNoFrameBuf    = 4, // Use direct rendering without frame buffer
      kHltStyle      = 8, // Draw with highlighting style
      kContrastStyle = 16 // Draw with contast style
    };
    /** \details
      Returns overlay classification flags for specified overlay.
      \param id [in]  Overlay index.
    */
    inline static OdUInt32 overlayFlags(OdGsOverlayId id)
    {
      switch (id)
      {
        case kGsUserBg1Overlay:            return kNoDepth;
        case kGsUserBg2Overlay:            return kOwnDepth;
        case kGsUserBg3Overlay:            return kMainDepth;
        case kGsMainOverlay:               return kMainDepth;
        case kGsSpriteOverlay:             return kOwnDepth;
        case kGsDirectOverlay:             return kNoDepth;
        case kGsHighlightOverlay:          return kNoDepth | kNoFrameBuf;
        case kGsHighlightSelectionOverlay: return kNoDepth | kNoFrameBuf | kHltStyle;
        case kGsDirectTopmostOverlay:      return kNoDepth;
        case kGsContrastOverlay:           return kNoDepth | kNoFrameBuf | kContrastStyle;
        case kGsUserFg1Overlay:            return kMainDepth;
        case kGsUserFg2Overlay:            return kOwnDepth;
        case kGsUserFg3Overlay:            return kNoDepth;
        default:                           ODA_FAIL();
      }
      return kNoDepth;
    }
  public:
    /** \details
      Converts overlay index into overlay rendering order.
      \param id [in]  Overlay index.
    */
    inline static OdUInt32 overlayIndexToRenderingOrder(OdGsOverlayId id)
    {
      switch (id)
      {
        case kGsMainOverlay:               return kMain;
        case kGsDirectOverlay:             return kDirect;
        case kGsUserFg3Overlay:            return kUserFg3;
        case kGsUserBg1Overlay:            return kUserBg1;
        case kGsUserBg3Overlay:            return kUserBg3;
        case kGsUserFg1Overlay:            return kUserFg1;
        case kGsHighlightOverlay:          return kHighlight;
        case kGsHighlightSelectionOverlay: return kHighlightSelection;
        case kGsDirectTopmostOverlay:      return kDirectTopmost;
        case kGsSpriteOverlay:             return kSprite;
        case kGsContrastOverlay:           return kContrast;
        case kGsUserFg2Overlay:            return kUserFg2;
        case kGsUserBg2Overlay:            return kUserBg2;
        default:                           ODA_FAIL();
      }
      return kMain;
    }
    /** \details
      Converts overlay rendering order into overlay index.
      \param renderOrder [in]  Overlay rendering order.
    */
    inline static OdGsOverlayId overlayRenderingOrderToIndex(OdUInt32 renderOrder)
    {
      switch (renderOrder)
      {
        case kUserBg1:            return kGsUserBg1Overlay;
        case kUserBg2:            return kGsUserBg2Overlay;
        case kUserBg3:            return kGsUserBg3Overlay;
        case kMain:               return kGsMainOverlay;
        case kSprite:             return kGsSpriteOverlay;
        case kDirect:             return kGsDirectOverlay;
        case kHighlight:          return kGsHighlightOverlay;
        case kHighlightSelection: return kGsHighlightSelectionOverlay;
        case kDirectTopmost:      return kGsDirectTopmostOverlay;
        case kContrast:           return kGsContrastOverlay;
        case kUserFg1:            return kGsUserFg1Overlay;
        case kUserFg2:            return kGsUserFg2Overlay;
        case kUserFg3:            return kGsUserFg3Overlay;
        default:                  ODA_FAIL();
      }
      return kGsMainOverlay;
    }
    /** \details
      Validate overlay definitions.
      \param gsModelDef [in]  Number of overlay buffers defined in OdGsModel class.
      \remarks For debug purposes.
    */
    inline static bool validateDefinitions(OdUInt32 gsModelDef)
    {
      return (gsModelDef == kNumOverlays) && (gsModelDef == kNumGsOverlays);
    }
    /** \details
      Validate overlay index.
      \param id [in]  Overlay index.
    */
    inline static bool validateOverlayIndex(OdGsOverlayId id)
    {
      return (id >= kGsMainOverlay) && (id < kNumGsOverlays);
    }
    /** \details
      Validate rendering order.
      \param renderOrder [in]  Overlay rendering order.
    */
    inline static bool validateRenderingOrder(OdUInt32 renderOrder)
    { // #CORE-12139 : expression is always true
      return /* (renderOrder >= kUserBg1) && */ (renderOrder < kNumOverlays);
    }
};

class OdGsOverlayDataPlaceholder
{
  public:
    /** \details
      Default constructor for the OdGsOverlayDataPlaceholder class.
    */
    OdGsOverlayDataPlaceholder() { }
    
    /** \details
      Destructor for the OdGsOverlayDataPlaceholder class.
    */
    ~OdGsOverlayDataPlaceholder() { }
};

template <typename OverlayDataType = OdGsOverlayDataPlaceholder>
class OdGsOverlayDataContainer
{
  public: // public allocator
    /** \details
      Overlay data type allocator.
    */
    struct Allocator
    {
      /** \details
        Creates a new instance of the overlay data type and returns a pointer to it.
      */
      virtual OverlayDataType *createData() { return new OverlayDataType(); }
      
      /** \details
        Deletes the provided overlay data type.
      */
      virtual void deleteData(OverlayDataType *pObj) { delete pObj; }
    };
  protected: // data structures and members
    struct OverlayData
    {
      OverlayDataType* m_data;
      OdUInt32 m_numOverlayRefs;
      OverlayData() : m_data(NULL), m_numOverlayRefs(0) { }
    };
    typedef OdArray<OverlayData> OverlayDataArray;
    mutable OverlayDataArray m_overlaysData;
    Allocator        m_defAllocator;
    Allocator       *m_pAllocator;
    mutable OdUInt32 m_uActiveOverlays;
    OdUInt32         m_uInvalidOverlays;
  protected: // implementations
    void subAllocator(Allocator *pAllocator)
    {
      if ((pAllocator != m_pAllocator) && (pAllocator != NULL))
      { // Apply recreation of OverlayData nodes
        const OdUInt32 nOverlays = m_overlaysData.size();
        OverlayData *pOverlays = m_overlaysData.asArrayPtr();
        for (OdUInt32 nOverlay = 0; nOverlay < nOverlays; nOverlay++)
        {
          if (pOverlays[nOverlay].m_data)
          {
            OverlayDataType *pNewData = pAllocator->createData();
            *pNewData = *pOverlays[nOverlay].m_data;
            m_pAllocator->deleteData(pOverlays[nOverlay].m_data);
            pOverlays[nOverlay].m_data = pNewData;
          }
        }
        m_pAllocator = pAllocator;
      }
    }
    void activate(OdGsOverlayId id) const
    {
      if (m_overlaysData.size() <= (unsigned)id)
        m_overlaysData.resize(id + 1);
      OverlayData &overlay = m_overlaysData.at(id);
      if (!overlay.m_numOverlayRefs)
        overlay.m_data = m_pAllocator->createData();
      overlay.m_numOverlayRefs++;
      SETBIT_1(m_uActiveOverlays, 1 << id);
    }
    void deactivate(OdGsOverlayId id)
    {
      if (m_overlaysData.size() > (unsigned)id)
      {
        OverlayData &overlay = m_overlaysData.at(id);
        if (overlay.m_numOverlayRefs)
        {
          overlay.m_numOverlayRefs--;
          if (!overlay.m_numOverlayRefs)
          {
            m_pAllocator->deleteData(overlay.m_data);
            overlay.m_data = NULL;
            SETBIT_0(m_uActiveOverlays, 1 << id);
            SETBIT_0(m_uInvalidOverlays, 1 << id);
          }
        }
      }
    }
  public:
    /** \details
      Default constructor for the OdGsOverlayDataContainer class. Sets the allocator and activates main overlay.
    */
    OdGsOverlayDataContainer()
      : m_overlaysData(1, 1)
      , m_uActiveOverlays(0)
      , m_uInvalidOverlays(0)
    { // Always keep main overlay activated
      m_pAllocator = &m_defAllocator;
      activate(kGsMainOverlay);
    }
    
    /** \details
      Destructor for the OdGsOverlayDataContainer class. Frees resources allocated for overlays.
    */
    ~OdGsOverlayDataContainer()
    {
      const OdUInt32 nOverlays = m_overlaysData.size();
      OverlayData *pOverlays = m_overlaysData.asArrayPtr();
      for (OdUInt32 nOverlay = 0; nOverlay < nOverlays; nOverlay++)
      {
        if (pOverlays[nOverlay].m_data)
          m_pAllocator->deleteData(pOverlays[nOverlay].m_data);
      }
    }

    /** \details
      Replaces current allocator with the passed allocator. Current overlay data is not destroyed.
      
      \param pAllocator  [in]  Pointer to a new allocator to set.
    */
    void substituteAllocator(Allocator *pAllocator)
    {
      subAllocator(pAllocator);
    }
    
    /** \details
      Replaces current allocator with standard allocator. Current overlay data is not destroyed.
    */
    void resetAllocator()
    {
      subAllocator(&m_defAllocator);
    }

    /** \details
      Activates the passed overlay.
      
      \param id  [in]  Overlay index.
      \returns
      true if the passed overlay was not active before but now is, false otherwise.
    */
    bool activateOverlay(OdGsOverlayId id)
    {
      const bool bActive = isOverlayActive(id);
      activate(id);
      return bActive != isOverlayActive(id);
    }
    
    /** \details
      Deactivates the passed overlay and removes overlay data from the allocator.
      
      \param id  [in]  Overlay index.
      \returns
      true if the passed overlay was active before but now is not, false otherwise.
    */
    bool deactivateOverlay(OdGsOverlayId id)
    {
      const bool bActive = isOverlayActive(id);
      deactivate(id);
      return bActive != isOverlayActive(id);
    }

    /** \details
      Retrives the overlay number of references.

      \param id [in] Overlay index.
      \returns
      number of specified overlay references.
    */
    OdUInt32 overlayNumRefs( OdGsOverlayId id ) const
    {
      if( m_overlaysData.size() > (unsigned)id )
      {
        OverlayData& overlay = m_overlaysData.at( id );
        return overlay.m_numOverlayRefs;
      }
      return 0;
    }

    /** \details
      Returns a number of active overlays.
    */
    OdUInt32 activeOverlays() const { return m_uActiveOverlays; }
    
    /** \details
      Checks whether the passed overlay is active.
      
      \param id  [in]  Overlay index.
      \returns
      true if passed overlay is active, false otherwise.
    */
    bool isOverlayActive(OdGsOverlayId id) const { return GETBIT(m_uActiveOverlays, 1 << id); }
    
    /** \details
      Checks whether multiple overlays are used.
      \returns
      true if multiple overlays are used, false otherwise.
    */
    bool isMultipleOverlays() const { return (m_uActiveOverlays & ~1) != 0; }

    /** \details
      Retrieves a pointer to the overlay data.
      
      \param id         [in]  Overlay index.
      \param bActivate  [in]  Flag that specifies whether to activate overlay if it is not currently active.
    */
    OverlayDataType *getOverlayData(OdGsOverlayId id, bool bActivate = false)
    {
      if (!isOverlayActive(id))
      {
        if (bActivate)
          activate(id);
        else
          return NULL;
      }
      return m_overlaysData[id].m_data;
    }
    
    /** \details
      Retrieves a pointer to the overlay data.
      
      \param id         [in]  Overlay index.
      \param bActivate  [in]  Flag that specifies whether to activate overlay if it is not currently active.
    */
    const OverlayDataType *getOverlayData(OdGsOverlayId id, bool bActivate = false) const
    {
      if (!isOverlayActive(id))
      {
        if (bActivate)
          activate(id);
        else
          return NULL;
      }
      return m_overlaysData[id].m_data;
    }

    struct ForEach
    {
      virtual bool call(OdGsOverlayId overlayId, OverlayDataType *pData) = 0;
    };
    bool execForEach(ForEach &impl, OdUInt32 nMask = kGsAllOverlays) const
    {
      OdUInt32 bitSet = m_uActiveOverlays & nMask, nOlay = 0;
      while (bitSet)
      {
        if (GETBIT(bitSet, 1 << nOlay))
        {
          if (!impl.call((OdGsOverlayId)nOlay, m_overlaysData[nOlay].m_data))
            return false;
          SETBIT_0(bitSet, 1 << nOlay);
        }
        nOlay++;
      }
      return true;
    }

    /** \details
      Represents overlay iterator.
    */
    class Iterator
    {
      protected:
        OverlayDataArray &overlaysData;
        OdUInt32 bitSet, nOlay;
        friend class OdGsOverlayDataContainer<OverlayDataType>;
        Iterator(OverlayDataArray &_overlaysData, OdUInt32 _bitSet)
          : overlaysData(_overlaysData), bitSet(_bitSet), nOlay(0)
        { init(); }
        void init()
        {
          if (bitSet)
          {
            while (!GETBIT(bitSet, 1 << nOlay))
              nOlay++;
          }
        }
      public:
        /** \details
          References next overlay.
          \returns
          true if next overlay could be referenced, false otherwise.
        */
        bool next()
        {
          SETBIT_0(bitSet, 1 << nOlay);
          if (bitSet)
          {
            do { nOlay++; } while (!GETBIT(bitSet, 1 << nOlay));
            return true;
          }
          return false;
        }
        
        /** \details
          Checks whether the iterator is done iterating through overlays.
          \returns
          true if there are no more overlays to iterate, false otherwise.
        */
        bool done() const { return !bitSet; }
        
        /** \details
          Retrieves currently referenced overlay ID.
          \returns
          true if there are no more overlays to iterate, false otherwise.
        */
        OdGsOverlayId overlayId() const { return (OdGsOverlayId)nOlay; }
        
        /** \details
          Retrieves a pointer to overlay data of currently referenced overlay.
        */
        const OverlayDataType *overlayData() const { return overlaysData.getPtr()[nOlay].m_data; }
        
        /** \details
          Retrieves a pointer to overlay data of currently referenced overlay.
        */
        OverlayDataType *overlayData() { return overlaysData[nOlay].m_data; }
    };
    /** \details
      Creates and returns new overlay iterator.
      
      \param nMask         [in]  Overlay mask.
    */
    Iterator newIterator(OdUInt32 nMask = kGsAllOverlays) const
    {
      return Iterator(m_overlaysData, m_uActiveOverlays & nMask);
    }

    /** \details
      Returns a number that represents set of invalid overlays.
    */
    OdUInt32 invalidOverlays() const { return m_uInvalidOverlays; }
    
    /** \details
      Checks whether overlay by the specified index is invalid.
      
      \param id  [in]  Overlay index.
    */
    bool isOverlayInvalid(OdGsOverlayId id) const { return GETBIT(m_uInvalidOverlays, 1 << id); }
    
    /** \details
      Sets overlay by the specified index as invalid or valid.
      
      \param id     [in]  Overlay index.
      \param bSet   [in]  Flag that specifies whether overlay should be invalid.
    */
    void setOverlayInvalid(OdGsOverlayId id, bool bSet = true) { SETBIT(m_uInvalidOverlays, 1 << id, bSet); }
    
    /** \details
      Specifies whether currently active overlays should be invalid.
      
      \param bSet   [in]  Flag that specifies whether overlays should be invalid.
    */
    void setOverlaysInvalid(bool bSet = true) { SETBIT(m_uInvalidOverlays, kGsAllOverlays & m_uActiveOverlays, bSet); }
    
    /** \details
      Specifies whether currently active overlays should be invalid.
      
      \param nOverlays    [in]  Mask for active overlays.
      \param bSet         [in]  Flag that specifies whether overlays should be invalid.
    */
    void setOverlaysInvalid(OdUInt32 nOverlays, bool bSet = true) { SETBIT(m_uInvalidOverlays, nOverlays & m_uActiveOverlays, bSet); }
    
    /** \details
      Checks whether there is any invalid overlay.
      
      \returns
      true if there is an invalid overlay, false otherwise.
    */
    bool isAnyOverlayInvalid() const { return (m_uInvalidOverlays & m_uActiveOverlays) != 0; }
    
    /** \details
      Checks whether all overlay are invalid.
      
      \returns
      true if overlays are invalid, false otherwise.
    */
    bool isAllOverlaysIvalid() const { return (m_uInvalidOverlays & m_uActiveOverlays) == m_uActiveOverlays; }
};

#include "TD_PackPop.h"

#endif // __GSOVERLAYDEFS_H_INCLUDED_
