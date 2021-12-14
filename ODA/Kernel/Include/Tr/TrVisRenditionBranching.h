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
// Rendition branching interface

#ifndef ODTRVISRENDITIONBRANCHING
#define ODTRVISRENDITIONBRANCHING

#include "TD_PackPush.h"

#include "TrVisRendition.h"
#define STL_USING_MAP
#include "OdaSTL.h"

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenditionBranching : public OdTrVisRendition
{
  protected:
    OdTrVisRenditionPtr m_pRenditionA, m_pRenditionB, m_pRenditionC;
    mutable struct IdRegistratorBranching : public IdRegistrator
    {
      IdRegistrator *m_pIdRegA, *m_pIdRegB;
      IdRegistratorBranching() : m_pIdRegA(NULL), m_pIdRegB(NULL) { }

      void setIdRegA(IdRegistrator *pReg) { m_pIdRegA = pReg; }
      IdRegistrator *getIdRegA() { return m_pIdRegA; }
      const IdRegistrator *getIdRegA() const { return m_pIdRegA; }
      bool hasIdRegA() const { return (m_pIdRegA != NULL); }

      void setIdRegB(IdRegistrator *pReg) { m_pIdRegB = pReg; }
      IdRegistrator *getIdRegB() { return m_pIdRegB; }
      const IdRegistrator *getIdRegB() const { return m_pIdRegB; }
      bool hasIdRegB() const { return (m_pIdRegB != NULL); }

      void setIdRegs(IdRegistrator *pRegA, IdRegistrator *pRegB)
      { setIdRegA(pRegA); setIdRegB(pRegB); }
      bool hasIdReg() const  { return hasIdRegA() || hasIdRegB(); }
      bool hasIdRegs() const { return hasIdRegA() && hasIdRegB(); }

      virtual void reg(DataType type, OdTrVisId id, OdDbStub *pPersId)
      {
        if (hasIdRegA())
          getIdRegA()->reg(type, id, pPersId);
        if (hasIdRegB())
          getIdRegB()->reg(type, id, pPersId);
      }
      virtual void reg(DataType type, OdTrVisId id, const OdGiDrawable *pTransId)
      {
        if (hasIdRegA())
          getIdRegA()->reg(type, id, pTransId);
        if (hasIdRegB())
          getIdRegB()->reg(type, id, pTransId);
      }
      virtual void reg(DataType type, OdTrVisId id, const OdTrVisUniqueID &uId)
      {
        if (hasIdRegA())
          getIdRegA()->reg(type, id, uId);
        if (hasIdRegB())
          getIdRegB()->reg(type, id, uId);
      }
      virtual void kill(DataType type, OdTrVisId id)
      {
        if (hasIdRegA())
          getIdRegA()->kill(type, id);
        if (hasIdRegB())
          getIdRegB()->kill(type, id);
      }
      virtual OdTrVisUniqueID *get(DataType type, OdTrVisId id, bool bKill = false) const
      {
        if (hasIdRegA())
          return getIdRegA()->get(type, id, bKill);
        if (hasIdRegB())
          return getIdRegB()->get(type, id, bKill);
        return NULL;
      }
    } m_idRegsBranching;
    static const MtHandlerType g_mtHandlerPriority[6][6];
  protected:
    struct DisplayIdMapping
    {
      // Map between metafileId and first displayId.
      typedef std::map<OdTrVisMetafileId, OdTrVisDisplayId> MetafileRegMap;
      MetafileRegMap m_metafileIdMap;
      // Map between first displayId and second displayId.
      typedef std::map<OdTrVisDisplayId, OdTrVisDisplayId> DisplayIdsMap;
      DisplayIdsMap m_displayIdMap;
      // Temporary displayId's array for second displayId's.
      mutable OdVector<OdTrVisDisplayId, OdMemoryAllocator<OdTrVisDisplayId> > m_temporaryDisplayIds;
      // Check does mapping exist.
      bool hasMapping() const { return !m_metafileIdMap.empty(); }
      // Insert mapping.
      void regMapping(OdTrVisMetafileId mfId, OdTrVisDisplayId ds1, OdTrVisDisplayId ds2)
      { m_metafileIdMap[mfId] = ds1; m_displayIdMap[ds1] = ds2; }
      // Remove mapping.
      void unregMapping(OdTrVisMetafileId mfId)
      { if (hasMapping()) { MetafileRegMap::iterator it = m_metafileIdMap.find(mfId);
        if (it != m_metafileIdMap.end()) { m_displayIdMap.erase(it->second); m_metafileIdMap.erase(it); } } }
      // Clear mapping by request.
      void clearMapping()
      { if (hasMapping()) { m_metafileIdMap.clear(); m_displayIdMap.clear(); m_temporaryDisplayIds.setPhysicalLength(0); } }
      // Make temporary displayId's mapping.
      OdTrVisDisplayId remap(OdTrVisDisplayId id) const
      { if (hasMapping()) { DisplayIdsMap::const_iterator it = m_displayIdMap.find(id);
                            if (it != m_displayIdMap.end()) return it->second; }
        return id; }
      const OdTrVisDisplayId *remap(const OdTrVisDisplayId *pIdsList, OdUInt32 nData) const
      { if (!nData || !hasMapping()) { return pIdsList; }
        m_temporaryDisplayIds.resize(nData);
        OdTrVisDisplayId *pOutIds = m_temporaryDisplayIds.asArrayPtr();
        for (OdUInt32 nId = 0; nId < nData; nId++)
        { DisplayIdsMap::const_iterator it = m_displayIdMap.find(pIdsList[nId]);
          if (it != m_displayIdMap.end()) pOutIds[nId] = it->second;
          else pOutIds[nId] = pIdsList[nId]; } return pOutIds; }
    } m_displayIdMapping;
    void metafileStreamRemapping(const OdTrVisMetafileDef &pDef);
  protected:
    void updateIdRegA() { m_idRegsBranching.setIdRegA((hasRenditionA()) ? getRenditionA()->idRegistrator() : NULL); }
    void updateIdRegB() { m_idRegsBranching.setIdRegB((hasRenditionB()) ? getRenditionB()->idRegistrator() : NULL); }
  public:
    OdTrVisRenditionBranching() { }

    void setRenditionA(OdTrVisRendition *pRendition) { m_pRenditionA = pRendition; updateIdRegA(); }
    OdTrVisRendition *getRenditionA() { return m_pRenditionA; }
    const OdTrVisRendition *getRenditionA() const { return m_pRenditionA; }
    bool hasRenditionA() const { return !m_pRenditionA.isNull(); }

    void setRenditionB(OdTrVisRendition *pRendition) { if (pRendition != m_pRenditionB) { m_displayIdMapping.clearMapping(); }
                                                       m_pRenditionB = pRendition; updateIdRegB(); }
    OdTrVisRendition *getRenditionB() { return m_pRenditionB; }
    const OdTrVisRendition *getRenditionB() const { return m_pRenditionB; }
    bool hasRenditionB() const { return !m_pRenditionB.isNull(); }

    void setRenditions(OdTrVisRendition *pRenditionA, OdTrVisRendition *pRenditionB)
    { setRenditionA(pRenditionA); setRenditionB(pRenditionB); }
    bool hasRendition() const  { return hasRenditionA() || hasRenditionB(); }
    bool hasRenditions() const { return hasRenditionA() && hasRenditionB(); }

    void setClientSettingsRendition(bool bRenditionA = true, bool bRenditionB = false)
    {
      if (bRenditionA)
      { if (bRenditionB) m_pRenditionC = NULL;
        else m_pRenditionC = m_pRenditionA;
      } else if (bRenditionB) m_pRenditionC = m_pRenditionB;
      else m_pRenditionC = NULL;
    }
    OdTrVisRendition *getClientSettingsRendition() { return m_pRenditionC; }
    const OdTrVisRendition *getClientSettingsRendition() const { return m_pRenditionC; }
    bool hasClientSettingsRendition() const { return !m_pRenditionC.isNull(); }

    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient *pDevice)
    {
      if (hasRenditionA())
        getRenditionA()->onDeviceUpdateBegin(pDevice), updateIdRegA();
      if (hasRenditionB())
        getRenditionB()->onDeviceUpdateBegin(pDevice), updateIdRegB();
    }
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient *pDevice, bool bRender = true)
    {
      if (hasRenditionA())
        getRenditionA()->onDeviceUpdateEnd(pDevice, bRender);
      if (hasRenditionB())
        getRenditionB()->onDeviceUpdateEnd(pDevice, bRender);
    }
    // Checks that vectorizer update started.
    virtual bool isWithinDeviceUpdate() const
    { bool bRes = false;
      if (hasRenditionA())
        bRes |= getRenditionA()->isWithinDeviceUpdate();
      if (hasRenditionB())
        bRes |= getRenditionB()->isWithinDeviceUpdate();
      return bRes;
    }

    // Called by underlying application to store frame changing information.
    virtual void onFrameSwitchInfo(OdUInt32 nInterval)
    {
      if (hasRenditionA())
        getRenditionA()->onFrameSwitchInfo(nInterval);
      if (hasRenditionB())
        getRenditionB()->onFrameSwitchInfo(nInterval);
    }

    // Returns set of bit flags which indicate rendition capabilities/settings
    virtual OdUInt32 queryRenditionCaps(OdUInt32 requestFor = 0xFFFFFFFF) const
    {
      if (hasRenditions())
        return getRenditionA()->queryRenditionCaps(requestFor) & getRenditionB()->queryRenditionCaps(requestFor);
      if (hasRenditionA())
        return getRenditionA()->queryRenditionCaps(requestFor);
      if (hasRenditionB())
        return getRenditionB()->queryRenditionCaps(requestFor);
      return 0;
    }

    // Partial update support for local renditions:
    // Called to check does specified viewport/overlay combination support partial update query.
    virtual bool onPartialUpdateCheckLimit(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId)
    { bool bRVal = false;
      if (hasRenditionA())
        bRVal |= getRenditionA()->onPartialUpdateCheckLimit(viewportId, overlayId);
      if (hasRenditionB())
        bRVal |= getRenditionB()->onPartialUpdateCheckLimit(viewportId, overlayId);
      return bRVal;
    }
    // Called to setup invalid rectangles and overlays on partial update.
    virtual void onPartialUpdateInvalidRects(OdTrVisOverlayId overlayId, const OdGsDCRectArray &invalidRects, bool bOverlayInvalid = false)
    {
      if (hasRenditionA())
        getRenditionA()->onPartialUpdateInvalidRects(overlayId, invalidRects, bOverlayInvalid);
      if (hasRenditionB())
        getRenditionB()->onPartialUpdateInvalidRects(overlayId, invalidRects, bOverlayInvalid);
    }
    // Called on partial update query to set entire actual visibilities list.
    virtual void onPartialUpdateList(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries)
    {
      if (hasRenditionA())
        getRenditionA()->onPartialUpdateList(viewportId, overlayId, pList, nEntries);
      if (hasRenditionB())
        getRenditionB()->onPartialUpdateList(viewportId, overlayId, m_displayIdMapping.remap(pList, nEntries), nEntries);
    }
  protected:
    class RemapABInvocation : public OdTrVisDirectRenderPartialUpdateInvocation, protected OdTrVisRenderDataInvokeRedirect
    {
      OdTrVisDirectRenderPartialUpdateInvocationPtr m_pInvocationA;
      OdTrVisDirectRenderPartialUpdateInvocationPtr m_pInvocationB;
      typedef std::map<OdTrVisDisplayId, OdTrVisDisplayId> DisplayIdsMap;
      const DisplayIdsMap *m_pDisplayIdMap;
      virtual bool incomeMetafile(OdTrVisDisplayId mfId)
      {
        if (m_pDisplayIdMap)
        {
          DisplayIdsMap::const_iterator it = m_pDisplayIdMap->find(mfId);
          if (it != m_pDisplayIdMap->end()) mfId = it->second;
        }
        return OdTrVisRenderDataInvokeRedirect::incomeMetafile(mfId);
      }
      public:
        TD_USING(OdTrVisDirectRenderPartialUpdateInvocation::setup);
        void setup(OdTrVisDirectRenderPartialUpdateInvocation *pInvocationA, OdTrVisDirectRenderPartialUpdateInvocation *pInvocationB, const DisplayIdsMap *pIdMap)
        { m_pInvocationA = pInvocationA; m_pInvocationB = pInvocationB; m_pDisplayIdMap = pIdMap; }
        virtual OdTrVisRenderDataInvoke *wrapInvoke(OdTrVisRenderDataInvokeHid *pInvoke) const
        { RemapABInvocation *pRemap = const_cast<RemapABInvocation*>(this);
          pRemap->setRedirection(OdTrVisDirectRenderPartialUpdateInvocation::wrapInvoke(pInvoke));
          return pRemap; }
        virtual const OdTrVisDirectRenderPartialUpdateInvocation *basicInvocation() const
        { return m_pInvocationA; }
    };
  public:
    // Can be called by client side to check that rendition supports direct rendering ability.
    virtual bool queryPartialUpdateDirectInvocation(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdTrVisDirectRenderPartialUpdateInvocationPtr &pInvocationPtr)
    {
      const bool bHasA = hasRenditionA() ? getRenditionA()->queryPartialUpdateDirectInvocation(viewportId, overlayId, pInvocationPtr) : false;
      if (!hasRenditionB())
        return bHasA;
      OdTrVisDirectRenderPartialUpdateInvocationPtr pInvocationA = pInvocationPtr;
      const bool bHasB = hasRenditionB() ? getRenditionB()->queryPartialUpdateDirectInvocation(viewportId, overlayId, pInvocationPtr) : false;
      if (bHasB)
      {
        OdSmartPtr<RemapABInvocation> rabInvocation = OdRxObjectImpl<RemapABInvocation>::createObject();
        rabInvocation->setup(pInvocationPtr);
        rabInvocation->setup(pInvocationA, pInvocationPtr, (m_displayIdMapping.hasMapping()) ? &m_displayIdMapping.m_displayIdMap : NULL);
        pInvocationPtr = rabInvocation;
      }
      return bHasA || bHasB;
    }
    // Can be called by client side to setup direct invocation interface for partial update instead of partial update list usage.
    virtual void onPartialUpdateDirectInvocation(const OdTrVisDirectRenderPartialUpdateInvocation *pInvocation, OdTrVisRenderDataDirectAccessor *pAccessor)
    {
      if (hasRenditionA())
        getRenditionA()->onPartialUpdateDirectInvocation(pInvocation->basicInvocation(), pAccessor);
      if (hasRenditionB())
        getRenditionB()->onPartialUpdateDirectInvocation(pInvocation, pAccessor);
    }

    // Query snapshot
    virtual void querySnapshot(OdUInt8 *pScanlines, long minX, long minY, long nWidth, long nHeight)
    {
      if (hasRenditionA())
        getRenditionA()->querySnapshot(pScanlines, minX, minY, nWidth, nHeight);
      if (hasRenditionB())
        getRenditionB()->querySnapshot(pScanlines, minX, minY, nWidth, nHeight);
    }

    // Renders viewport to selection texture. Return top object id with (x, y) position in viewport.
    virtual void getSelectedObject(OdTrVisViewportId id, OdUInt32 x, OdUInt32 y, OdArray<OdTrVisId> &outIDs, OdTrVisGsMarker *outMarker = NULL)
    {
      if (hasRenditionA())
        getRenditionA()->getSelectedObject(id, x, y, outIDs, outMarker);
      if (hasRenditionB())
        getRenditionB()->getSelectedObject(id, x, y, outIDs, outMarker);
    }
    // Validates selection texture.
    virtual bool isSelectTextureValid(OdTrVisViewportId id) const
    { bool bRes = false;
      if (hasRenditionA())
        bRes |= getRenditionA()->isSelectTextureValid(id);
      if (hasRenditionB())
        bRes |= getRenditionB()->isSelectTextureValid(id);
      return bRes;
    }

    // Allocate new resource sharing provider compatible with rendition
    virtual OdTrVisSharingProviderClientPtr createSharingProvider()
    { // @@@TODO: probably we need branching of sharing providers?
      if (hasRenditionA())
        return getRenditionA()->createSharingProvider();
      if (hasRenditionB())
        return getRenditionB()->createSharingProvider();
      return OdTrVisSharingProviderClientPtr();
    }

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType &type) const
    { // Each rendition must be protected by own Mt handler, but we still can sort handler types by priority.
      if (hasRenditions())
      {
        const MtHandlerType typeA = getRenditionA()->mtDataTypeProcessing(type), typeB = getRenditionB()->mtDataTypeProcessing(type);
        const MtHandlerType rv = g_mtHandlerPriority[MtComplex::decode(typeA)][MtComplex::decode(typeB)];
        if (rv == kMtComplex)
        {
          if (MtComplex::decode(typeA) == kMtComplex)
            return typeA;
          return typeB;
        }
        return rv;
      }
      if (hasRenditionA())
        return getRenditionA()->mtDataTypeProcessing(type);
      if (hasRenditionB())
        return getRenditionB()->mtDataTypeProcessing(type);
      return kMtSkip;
    }

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const
    {
      if (m_idRegsBranching.hasIdRegs())
        return &m_idRegsBranching;
      if (m_idRegsBranching.hasIdRegA())
        return m_idRegsBranching.getIdRegA();
      if (m_idRegsBranching.hasIdRegB())
        return m_idRegsBranching.getIdRegB();
      return NULL;
    }

    // Returns true if metafile streams accessible directly, returns stream by Id
    virtual bool queryMetafileStream(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId,
                                     OdTrVisDisplayId displayId, const OdTrVisMetafileContainer **pStream)
    {
      if (hasRenditionA() && getRenditionA()->queryMetafileStream(viewportId, overlayId, displayId, pStream))
        return true;
      if (hasRenditionB()) 
        return getRenditionB()->queryMetafileStream(viewportId, overlayId, m_displayIdMapping.remap(displayId), pStream);
      return false;
    }
    // Unlock metafile stream, queried by queryMetafileStream method
    virtual bool unlockMetafileStream(const OdTrVisMetafileContainer *pStream)
    {
      if (hasRenditionA() && getRenditionA()->unlockMetafileStream(pStream))
        return true;
      if (hasRenditionB())
        return getRenditionB()->unlockMetafileStream(pStream);
      return false;
    }

    // Returns non-kTrVisNegativeId in case if vectorizer supports ProgramID's embedded into metafile stream
    virtual OdTrVisProgramId queryProgramId(OdGsView::RenderMode mode, OdTrVisVisualStyleId vsId = kTrVisNegativeId,
                                            OdTrVisGeomType geomType = OdTrVisGeomType_Default, OdUInt16 attrFlags = 0, OdUInt16 shadingFlags = 0) const
    {
      if (hasRenditionA())
      {
        const OdTrVisProgramId prgId = getRenditionA()->queryProgramId(mode, vsId, geomType, attrFlags, shadingFlags);
        if (prgId != kTrVisNegativeId)
          return true;
      }
      if (hasRenditionB())
        return getRenditionB()->queryProgramId(mode, vsId, geomType, attrFlags, shadingFlags);
      return kTrVisNegativeId;
    }

    // GsFiler support
    virtual bool saveRenditionState(OdGsFiler *pFiler) const
    {
      if (hasRenditionA() && !getRenditionA()->saveRenditionState(pFiler))
        return false;
      if (hasRenditionB())
        return getRenditionB()->saveRenditionState(pFiler);
      return false;
    }
    virtual bool loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap)
    {
      if (hasRenditionA() && !getRenditionA()->loadRenditionState(pFiler, pIdMap))
        return false;
      if (hasRenditionB())
        return getRenditionB()->loadRenditionState(pFiler, pIdMap);
      return false;
    }

    // Mid-level callbacks

    // Client information

    // Returns rendering client settings.
    virtual void obtainClientSettings(ClientSettings &clientSets);

    // Surface

    // Called when output render surface parameters changed.
    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onSurfaceChanged(pDef);
      if (hasRenditionB())
        getRenditionB()->onSurfaceChanged(pDef);
    }
    // Called when rendering surface needs to be invalidated forcibly.
    virtual void onSurfaceInvalidated()
    {
      if (hasRenditionA())
        getRenditionA()->onSurfaceInvalidated();
      if (hasRenditionB())
        getRenditionB()->onSurfaceInvalidated();
    }

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onOverlayAdded(overlayId, pDef);
      if (hasRenditionB())
        getRenditionB()->onOverlayAdded(overlayId, pDef);
    }
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId overlayId)
    {
      if (hasRenditionA())
        getRenditionA()->onOverlayDeleted(overlayId);
      if (hasRenditionB())
        getRenditionB()->onOverlayDeleted(overlayId);
    }
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility)
    {
      if (hasRenditionA())
        getRenditionA()->onOverlayVisibilityChanged(overlayId, bVisibility);
      if (hasRenditionB())
        getRenditionB()->onOverlayVisibilityChanged(overlayId, bVisibility);
    }
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder)
    {
      if (hasRenditionA())
        getRenditionA()->onOverlayRenderOrderChanged(overlayId, nRenderOrder);
      if (hasRenditionB())
        getRenditionB()->onOverlayRenderOrderChanged(overlayId, nRenderOrder);
    }

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportAdded(viewportId, pDef);
      if (hasRenditionB())
        getRenditionB()->onViewportAdded(viewportId, pDef);
    }
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId viewportId, int nIndex, const OdTrVisViewportDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportInserted(viewportId, nIndex, pDef);
      if (hasRenditionB())
        getRenditionB()->onViewportInserted(viewportId, nIndex, pDef);
    }
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId viewportId)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportDeleted(viewportId);
      if (hasRenditionB())
        getRenditionB()->onViewportDeleted(viewportId);
    }
    // Called when viewport visibility changed.
    virtual void onViewportVisibilityChanged(OdTrVisViewportId viewportId, bool bVisibility)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportVisibilityChanged(viewportId, bVisibility);
      if (hasRenditionB())
        getRenditionB()->onViewportVisibilityChanged(viewportId, bVisibility);
    }

    // Primary viewports configuration

    // Called when viewport view parameters is modified.
    virtual void onViewportViewParamsModified(OdTrVisViewportId viewportId, const OdTrVisViewParamsDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportViewParamsModified(viewportId, pDef);
      if (hasRenditionB())
        getRenditionB()->onViewportViewParamsModified(viewportId, pDef);
    }
    // Called when viewport linestyle settings is modified.
    virtual void onViewportLineStylesModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineStyleDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportLineStylesModified(viewportId, pDef);
      if (hasRenditionB())
        getRenditionB()->onViewportLineStylesModified(viewportId, pDef);
    }
    // Called when viewport fading settings is modified.
    virtual void onViewportFadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportFadingDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportFadingModified(viewportId, pDef);
      if (hasRenditionB())
        getRenditionB()->onViewportFadingModified(viewportId, pDef);
    }
    // Called when viewport lineweight settings is modified.
    virtual void onViewportLineweightsModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineweightsDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportLineweightsModified(viewportId, pDef);
      if (hasRenditionB())
        getRenditionB()->onViewportLineweightsModified(viewportId, pDef);
    }
    // Called when viewport rendering mode is modified.
    virtual void onViewportShadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportShadingDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportShadingModified(viewportId, pDef);
      if (hasRenditionB())
        getRenditionB()->onViewportShadingModified(viewportId, pDef);
    }
    // Called when viewport clipping region is modified.
    virtual void onViewportClipModified(OdTrVisViewportId viewportId, const OdTrVisViewportClipDef &pDef,
                                        bool bNrcClipModified = true, bool bBorderModified = true)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportClipModified(viewportId, pDef, bNrcClipModified, bBorderModified);
      if (hasRenditionB())
        getRenditionB()->onViewportClipModified(viewportId, pDef, bNrcClipModified, bBorderModified);
    }

    // Extended viewports configuration

    // Called when background stream in viewport is changed.
    virtual void onViewportBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                             OdTrVisProgramId baseProgramId = kTrVisNegativeId)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportBackgroundChanged(viewportId, pStream, baseProgramId);
      if (hasRenditionB())
        getRenditionB()->onViewportBackgroundChanged(viewportId, pStream, baseProgramId);
    }
    // Called when viewport fog changed.
    virtual void onViewportFogChanged(OdTrVisViewportId viewportId, const OdTrVisFogDef &pDef = OdTrVisFogDef())
    {
      if (hasRenditionA())
        getRenditionA()->onViewportFogChanged(viewportId, pDef);
      if (hasRenditionB())
        getRenditionB()->onViewportFogChanged(viewportId, pDef);
    }
    // Called when viewport sectioning modified.
    virtual void onViewportSectioningChanged(OdTrVisViewportId viewportId, const OdTrVisSectionDef &pDef = OdTrVisSectionDef())
    {
      if (hasRenditionA())
        getRenditionA()->onViewportSectioningChanged(viewportId, pDef);
      if (hasRenditionB())
        getRenditionB()->onViewportSectioningChanged(viewportId, pDef);
    }
    // Called when viewport anti-aliasing setting changed.
    virtual void onViewportAntiAliasingChanged(OdTrVisViewportId viewportId, OdUInt32 nAntiAliasing)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportAntiAliasingChanged(viewportId, nAntiAliasing);
      if (hasRenditionB())
        getRenditionB()->onViewportAntiAliasingChanged(viewportId, nAntiAliasing);
    }

    // Viewport default lights configuration

    // Called when viewport ambient light setting modified.
    virtual void onViewportAmbientLightChanged(OdTrVisViewportId viewportId, const OdTrVisMaterialColor &ambientLight)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportAmbientLightChanged(viewportId, ambientLight);
      if (hasRenditionB())
        getRenditionB()->onViewportAmbientLightChanged(viewportId, ambientLight);
    }
    // Called when viewport default light setting modified.
    virtual void onViewportDefaultLightsChanged(OdTrVisViewportId viewportId, bool bDefaultLightsOn,
                                                const OdTrVisLightDef *pLightsList = NULL, OdUInt32 nLightsListEntries = 0)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportDefaultLightsChanged(viewportId, bDefaultLightsOn, pLightsList, nLightsListEntries);
      if (hasRenditionB())
        getRenditionB()->onViewportDefaultLightsChanged(viewportId, bDefaultLightsOn, pLightsList, nLightsListEntries);
    }

    // Viewport lights management

    // Called when light source added into viewport light sources list.
    virtual void onViewportLightAttached(OdTrVisViewportId viewportId, OdTrVisLightId lightId,
                                         OdUInt32 nOverrides = 0, const OdTrVisLightDef &pOverrides = OdTrVisLightDef())
    {
      if (hasRenditionA())
        getRenditionA()->onViewportLightAttached(viewportId, lightId, nOverrides, pOverrides);
      if (hasRenditionB())
        getRenditionB()->onViewportLightAttached(viewportId, lightId, nOverrides, pOverrides);
    }
    // Called when light source removed from viewport light sources list.
    virtual void onViewportLightDetached(OdTrVisViewportId viewportId, OdTrVisLightId lightId)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportLightDetached(viewportId, lightId);
      if (hasRenditionB())
        getRenditionB()->onViewportLightDetached(viewportId, lightId);
    }

    // Per-overlay viewport overrides

    // Called when viewport needs override view parameters for specific overlay.
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride,
                                             const OdTrVisViewParamsDef &pDef = OdTrVisViewParamsDef())
    {
      if (hasRenditionA())
        getRenditionA()->onOverlayViewParamsOverride(viewportId, overlayId, bOverride, pDef);
      if (hasRenditionB())
        getRenditionB()->onOverlayViewParamsOverride(viewportId, overlayId, bOverride, pDef);
    }
    // Called when viewport extents changed.
    virtual void onOverlayExtentsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onOverlayExtentsOverride(viewportId, overlayId, pDef);
      if (hasRenditionB())
        getRenditionB()->onOverlayExtentsOverride(viewportId, overlayId, pDef);
    }

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed.
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries)
    {
      if (hasRenditionA())
        getRenditionA()->onMetafileOrderChanged(viewportId, overlayId, pList, nEntries);
      if (hasRenditionB())
        getRenditionB()->onMetafileOrderChanged(viewportId, overlayId, m_displayIdMapping.remap(pList, nEntries), nEntries);
    }
    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen)
    {
      if (hasRenditionA())
        getRenditionA()->onMetafileOrderChanged(viewportId, overlayId, nInvalidFrom, nInvalidLen, pValidFrom, nValidLen);
      if (hasRenditionB())
        getRenditionB()->onMetafileOrderChanged(viewportId, overlayId, nInvalidFrom, nInvalidLen, m_displayIdMapping.remap(pValidFrom, nValidLen), nValidLen);
    }
    // Called when order of metafiles can be inherit from other source.
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 )
    {
      if (hasRenditionA())
        getRenditionA()->onMetafileOrderInheritance(viewportId1, overlayId1, viewportId2);
      if (hasRenditionB())
        getRenditionB()->onMetafileOrderInheritance(viewportId1, overlayId1, viewportId2);
    }

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef)
    { OdTrVisDisplayId id1, id2;
      if (hasRenditionA())
      { id1 = getRenditionA()->onMetafileAdded(metafileId, pDef);
        if (!hasRenditionB())
          id2 = id1; }
      if (hasRenditionB())
      { metafileStreamRemapping(pDef);
        id2 = getRenditionB()->onMetafileAdded(metafileId, pDef);
        if (!hasRenditionA())
          id1 = id2; }
      if (id1 != id2)
        m_displayIdMapping.regMapping(metafileId, id1, id2);
      return id1;
    }
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId metafileId)
    {
      if (hasRenditionA())
        getRenditionA()->onMetafileDeleted(metafileId);
      if (hasRenditionB())
        getRenditionB()->onMetafileDeleted(metafileId);
      m_displayIdMapping.unregMapping(metafileId);
    }
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef)
    //{
    //  if (hasRenditionA())
    //    getRenditionA()->onMetafileModified(metafileId, pDef);
    //  if (hasRenditionB())
    //    getRenditionB()->onMetafileModified(metafileId, pDef);
    //}
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility)
    {
      if (hasRenditionA())
        getRenditionA()->onMetafileVisibilityChanged(metafileId, bVisibility);
      if (hasRenditionB())
        getRenditionB()->onMetafileVisibilityChanged(metafileId, bVisibility);
    }
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight)
    {
      if (hasRenditionA())
        getRenditionA()->onMetafileHighlightingChanged(metafileId, bHighlight);
      if (hasRenditionB())
        getRenditionB()->onMetafileHighlightingChanged(metafileId, bHighlight);
    }
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade)
    {
      if (hasRenditionA())
        getRenditionA()->onMetafileFadingChanged(metafileId, bFade);
      if (hasRenditionB())
        getRenditionB()->onMetafileFadingChanged(metafileId, bFade);
    }

    // Lights

    // Called when new light source added.
    virtual void onLightAdded(OdTrVisLightId lightId, const OdTrVisLightDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onLightAdded(lightId, pDef);
      if (hasRenditionB())
        getRenditionB()->onLightAdded(lightId, pDef);
    }
    // Called when exist light source modified.
    virtual void onLightModified(OdTrVisLightId lightId, OdUInt32 nKindOfMod, const OdTrVisLightDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onLightModified(lightId, nKindOfMod, pDef);
      if (hasRenditionB())
        getRenditionB()->onLightModified(lightId, nKindOfMod, pDef);
    }
    // Called when exist light source deleted.
    virtual void onLightDeleted(OdTrVisLightId lightId)
    {
      if (hasRenditionA())
        getRenditionA()->onLightDeleted(lightId);
      if (hasRenditionB())
        getRenditionB()->onLightDeleted(lightId);
    }

    // State branches (highlighting, hide)

    // Called when new state branch added.
    virtual void onStateBranchAdded(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onStateBranchAdded(stateBranchId, pDef);
      if (hasRenditionB())
        getRenditionB()->onStateBranchAdded(stateBranchId, pDef);
    }
    // Called when exist state branch modified.
    virtual void onStateBranchModified(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef,
                                       bool bChildsModified, bool bMarkersModified)
    {
      if (hasRenditionA())
        getRenditionA()->onStateBranchModified(stateBranchId, pDef, bChildsModified, bMarkersModified);
      if (hasRenditionB())
        getRenditionB()->onStateBranchModified(stateBranchId, pDef, bChildsModified, bMarkersModified);
    }
    // Called when exist state branch deleted.
    virtual void onStateBranchDeleted(OdTrVisStateBranchId stateBranchId)
    {
      if (hasRenditionA())
        getRenditionA()->onStateBranchDeleted(stateBranchId);
      if (hasRenditionB())
        getRenditionB()->onStateBranchDeleted(stateBranchId);
    }

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onTextureAdded(textureId, pDef);
      if (hasRenditionB())
        getRenditionB()->onTextureAdded(textureId, pDef);
    }
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId textureId)
    {
      if (hasRenditionA())
        getRenditionA()->onTextureDeleted(textureId);
      if (hasRenditionB())
        getRenditionB()->onTextureDeleted(textureId);
    }

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onMaterialAdded(materialId, pDef);
      if (hasRenditionB())
        getRenditionB()->onMaterialAdded(materialId, pDef);
    }
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId materialId)
    {
      if (hasRenditionA())
        getRenditionA()->onMaterialDeleted(materialId);
      if (hasRenditionB())
        getRenditionB()->onMaterialDeleted(materialId);
    }

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onVisualStyleAdded(visualStyleId, pDef);
      if (hasRenditionB())
        getRenditionB()->onVisualStyleAdded(visualStyleId, pDef);
    }
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onVisualStyleModified(visualStyleId, pDef);
      if (hasRenditionB())
        getRenditionB()->onVisualStyleModified(visualStyleId, pDef);
    }
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId)
    {
      if (hasRenditionA())
        getRenditionA()->onVisualStyleDeleted(visualStyleId);
      if (hasRenditionB())
        getRenditionB()->onVisualStyleDeleted(visualStyleId);
    }

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
    {
      if (hasRenditionA())
        getRenditionA()->onLayerAdded(layerId, pDef);
      if (hasRenditionB())
        getRenditionB()->onLayerAdded(layerId, pDef);
    }
    // Called when exist layer modified.
    //virtual void onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
    //{
    //  if (hasRenditionA())
    //    getRenditionA()->onLayerModified(layerId, pDef);
    //  if (hasRenditionB())
    //    getRenditionB()->onLayerModified(layerId, pDef);
    //}
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId layerId)
    {
      if (hasRenditionA())
        getRenditionA()->onLayerDeleted(layerId);
      if (hasRenditionB())
        getRenditionB()->onLayerDeleted(layerId);
    }
    // Called when exist layer visibility changed.
    virtual void onLayerVisibilityChanged(OdTrVisLayerId layerId, bool bVisibility)
    {
      if (hasRenditionA())
        getRenditionA()->onLayerVisibilityChanged(layerId, bVisibility);
      if (hasRenditionB())
        getRenditionB()->onLayerVisibilityChanged(layerId, bVisibility);
    }
    // Called when exist layer fading changed.
    virtual void onLayerFadingChanged(OdTrVisLayerId layerId, bool bFade)
    {
      if (hasRenditionA())
        getRenditionA()->onLayerFadingChanged(layerId, bFade);
      if (hasRenditionB())
        getRenditionB()->onLayerFadingChanged(layerId, bFade);
    }
    // Called when layer viewport-dependent properties changed.
    virtual void onLayerVpDepPropsOverride(OdTrVisLayerId layerId, OdTrVisViewportId viewportId, bool bOverride,
                                           const OdTrVisLayerProps &pProps = OdTrVisLayerProps())
    {
      if (hasRenditionA())
        getRenditionA()->onLayerVpDepPropsOverride(layerId, viewportId, bOverride, pProps);
      if (hasRenditionB())
        getRenditionB()->onLayerVpDepPropsOverride(layerId, viewportId, bOverride, pProps);
    }

    // Extension objects

    // Called when new extension object added.
    virtual void onExtensionObjectAdded(OdTrVisExtensionId extId, const OdTrVisExtensionObject *pObj)
    {
      if (hasRenditionA())
        getRenditionA()->onExtensionObjectAdded(extId, pObj);
      if (hasRenditionB())
        getRenditionB()->onExtensionObjectAdded(extId, pObj);
    }
    // Called when exist extension object deleted.
    virtual void onExtensionObjectDeleted(OdTrVisExtensionId extId)
    {
      if (hasRenditionA())
        getRenditionA()->onExtensionObjectDeleted(extId);
      if (hasRenditionB())
        getRenditionB()->onExtensionObjectDeleted(extId);
    }
    // Called to send special extension object specific data.
    virtual void onExtensionObjectSpecificData(OdTrVisExtensionId extId, OdUInt32 nDataId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onExtensionObjectSpecificData(extId, nDataId, propDef);
      if (hasRenditionB())
        getRenditionB()->onExtensionObjectSpecificData(extId, nDataId, propDef);
    }
    // Called to change extension object visibility.
    virtual void onExtensionObjectVisibilityChanged(OdTrVisExtensionId extId, bool bVisibility)
    {
      if (hasRenditionA())
        getRenditionA()->onExtensionObjectVisibilityChanged(extId, bVisibility);
      if (hasRenditionB())
        getRenditionB()->onExtensionObjectVisibilityChanged(extId, bVisibility);
    }

    // Groups

    // Called when new group added.
    virtual void onGroupAdded(OdTrVisGroupId groupId)
    {
      if (hasRenditionA())
        getRenditionA()->onGroupAdded(groupId);
      if (hasRenditionB())
        getRenditionB()->onGroupAdded(groupId);
    }
    // Called when exist group deleted.
    virtual void onGroupDeleted(OdTrVisGroupId groupId)
    {
      if (hasRenditionA())
        getRenditionA()->onGroupDeleted(groupId);
      if (hasRenditionB())
        getRenditionB()->onGroupDeleted(groupId);
    }
    // Called to attach metafiles to group.
    virtual void onGroupMetafilesAttached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles)
    {
      if (hasRenditionA())
        getRenditionA()->onGroupMetafilesAttached(groupId, pMetafilesList, nMetafiles);
      if (hasRenditionB())
        getRenditionB()->onGroupMetafilesAttached(groupId, pMetafilesList, nMetafiles);
    }
    // Called to detach metafiles from group (if no metafiles specified - clear entire list of groupped metafiles).
    virtual void onGroupMetafilesDetached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles)
    {
      if (hasRenditionA())
        getRenditionA()->onGroupMetafilesDetached(groupId, pMetafilesList, nMetafiles);
      if (hasRenditionB())
        getRenditionB()->onGroupMetafilesDetached(groupId, pMetafilesList, nMetafiles);
    }
    // Called to delete all groupped metafiles from graphics scene.
    //virtual void onGroupMetafilesDeleted(OdTrVisGroupId groupId)
    //{
    //  if (hasRenditionA())
    //    getRenditionA()->onGroupMetafilesDeleted(groupId);
    //  if (hasRenditionB())
    //    getRenditionB()->onGroupMetafilesDeleted(groupId);
    //}
    // Called to change visibility state for all groupped metafiles.
    virtual void onGroupMetafilesVisibilityChanged(OdTrVisGroupId groupId, bool bVisibility)
    {
      if (hasRenditionA())
        getRenditionA()->onGroupMetafilesVisibilityChanged(groupId, bVisibility);
      if (hasRenditionB())
        getRenditionB()->onGroupMetafilesVisibilityChanged(groupId, bVisibility);
    }
    // Called to change highlighting state for all groupped metafiles.
    virtual void onGroupMetafilesHighlightingChanged(OdTrVisGroupId groupId, bool bHighlight)
    {
      if (hasRenditionA())
        getRenditionA()->onGroupMetafilesHighlightingChanged(groupId, bHighlight);
      if (hasRenditionB())
        getRenditionB()->onGroupMetafilesHighlightingChanged(groupId, bHighlight);
    }
    // Called to change fading state for all groupped metafiles.
    virtual void onGroupMetafilesFadingChanged(OdTrVisGroupId groupId, bool bFade)
    {
      if (hasRenditionA())
        getRenditionA()->onGroupMetafilesFadingChanged(groupId, bFade);
      if (hasRenditionB())
        getRenditionB()->onGroupMetafilesFadingChanged(groupId, bFade);
    }

    // Properties

    // Metafile-related properties.
    // Called to attach property to metafile.
    virtual void onMetafilePropertyAttached(OdTrVisMetafileId metafileId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onMetafilePropertyAttached(metafileId, propDef);
      if (hasRenditionB())
        getRenditionB()->onMetafilePropertyAttached(metafileId, propDef);
    }
    // Called to detach property from metafile.
    virtual void onMetafilePropertyDetached(OdTrVisMetafileId metafileId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onMetafilePropertyDetached(metafileId, propType);
      if (hasRenditionB())
        getRenditionB()->onMetafilePropertyDetached(metafileId, propType);
    }
    // Group-related properties.
    // Called to attach property to group.
    virtual void onGroupPropertyAttached(OdTrVisGroupId groupId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onGroupPropertyAttached(groupId, propDef);
      if (hasRenditionB())
        getRenditionB()->onGroupPropertyAttached(groupId, propDef);
    }
    // Called to detach property from group.
    virtual void onGroupPropertyDetached(OdTrVisGroupId groupId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onGroupPropertyDetached(groupId, propType);
      if (hasRenditionB())
        getRenditionB()->onGroupPropertyDetached(groupId, propType);
    }
    // Layer-related properties.
    // Called to attach property to layer.
    virtual void onLayerPropertyAttached(OdTrVisLayerId layerId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onLayerPropertyAttached(layerId, propDef);
      if (hasRenditionB())
        getRenditionB()->onLayerPropertyAttached(layerId, propDef);
    }
    // Called to detach property from layer.
    virtual void onLayerPropertyDetached(OdTrVisLayerId layerId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onLayerPropertyDetached(layerId, propType);
      if (hasRenditionB())
        getRenditionB()->onLayerPropertyDetached(layerId, propType);
    }
    // VisualStyle-related properties.
    // Called to attach property to visual style.
    virtual void onVisualStylePropertyAttached(OdTrVisVisualStyleId visualStyleId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onVisualStylePropertyAttached(visualStyleId, propDef);
      if (hasRenditionB())
        getRenditionB()->onVisualStylePropertyAttached(visualStyleId, propDef);
    }
    // Called to detach property from visual style.
    virtual void onVisualStylePropertyDetached(OdTrVisVisualStyleId visualStyleId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onVisualStylePropertyDetached(visualStyleId, propType);
      if (hasRenditionB())
        getRenditionB()->onVisualStylePropertyDetached(visualStyleId, propType);
    }
    // Material-related properties.
    // Called to attach property to material.
    virtual void onMaterialPropertyAttached(OdTrVisMaterialId materialId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onMaterialPropertyAttached(materialId, propDef);
      if (hasRenditionB())
        getRenditionB()->onMaterialPropertyAttached(materialId, propDef);
    }
    // Called to detach property from material.
    virtual void onMaterialPropertyDetached(OdTrVisMaterialId materialId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onMaterialPropertyDetached(materialId, propType);
      if (hasRenditionB())
        getRenditionB()->onMaterialPropertyDetached(materialId, propType);
    }
    // Texture-related properties.
    // Called to attach property to texture.
    virtual void onTexturePropertyAttached(OdTrVisTextureId textureId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onTexturePropertyAttached(textureId, propDef);
      if (hasRenditionB())
        getRenditionB()->onTexturePropertyAttached(textureId, propDef);
    }
    // Called to detach property from texture.
    virtual void onTexturePropertyDetached(OdTrVisTextureId textureId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onTexturePropertyDetached(textureId, propType);
      if (hasRenditionB())
        getRenditionB()->onTexturePropertyDetached(textureId, propType);
    }
    // Light-related properties.
    // Called to attach property to light source.
    virtual void onLightPropertyAttached(OdTrVisLightId lightId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onLightPropertyAttached(lightId, propDef);
      if (hasRenditionB())
        getRenditionB()->onLightPropertyAttached(lightId, propDef);
    }
    // Called to detach property from light source.
    virtual void onLightPropertyDetached(OdTrVisLightId lightId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onLightPropertyDetached(lightId, propType);
      if (hasRenditionB())
        getRenditionB()->onLightPropertyDetached(lightId, propType);
    }
    // StateBranch-related properties.
    // Called to attach property to state branch.
    virtual void onStateBranchPropertyAttached(OdTrVisStateBranchId stateBranchId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onStateBranchPropertyAttached(stateBranchId, propDef);
      if (hasRenditionB())
        getRenditionB()->onStateBranchPropertyAttached(stateBranchId, propDef);
    }
    // Called to detach property from state branch.
    virtual void onStateBranchPropertyDetached(OdTrVisStateBranchId stateBranchId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onStateBranchPropertyDetached(stateBranchId, propType);
      if (hasRenditionB())
        getRenditionB()->onStateBranchPropertyDetached(stateBranchId, propType);
    }
    // Viewport-related properties.
    // Called to attach property to viewport.
    virtual void onViewportPropertyAttached(OdTrVisViewportId viewportId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportPropertyAttached(viewportId, propDef);
      if (hasRenditionB())
        getRenditionB()->onViewportPropertyAttached(viewportId, propDef);
    }
    // Called to detach property from viewport.
    virtual void onViewportPropertyDetached(OdTrVisViewportId viewportId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onViewportPropertyDetached(viewportId, propType);
      if (hasRenditionB())
        getRenditionB()->onViewportPropertyDetached(viewportId, propType);
    }
    // Overlay-related properties.
    // Called to attach property to overlay.
    virtual void onOverlayPropertyAttached(OdTrVisOverlayId overlayId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onOverlayPropertyAttached(overlayId, propDef);
      if (hasRenditionB())
        getRenditionB()->onOverlayPropertyAttached(overlayId, propDef);
    }
    // Called to detach property from overlay.
    virtual void onOverlayPropertyDetached(OdTrVisOverlayId overlayId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onOverlayPropertyDetached(overlayId, propType);
      if (hasRenditionB())
        getRenditionB()->onOverlayPropertyDetached(overlayId, propType);
    }
    // ExtensionObject-related properties.
    // Called to attach property to extension object.
    virtual void onExtensionObjectPropertyAttached(OdTrVisExtensionId extId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRenditionA())
        getRenditionA()->onExtensionObjectPropertyAttached(extId, propDef);
      if (hasRenditionB())
        getRenditionB()->onExtensionObjectPropertyAttached(extId, propDef);
    }
    // Called to detach property from extension object.
    virtual void onExtensionObjectPropertyDetached(OdTrVisExtensionId extId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRenditionA())
        getRenditionA()->onExtensionObjectPropertyDetached(extId, propType);
      if (hasRenditionB())
        getRenditionB()->onExtensionObjectPropertyDetached(extId, propType);
    }
};

#include "TD_PackPop.h"

#endif // ODTRVISRENDITIONBRANCHING
