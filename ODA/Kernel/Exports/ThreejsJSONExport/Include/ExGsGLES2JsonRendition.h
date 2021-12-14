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
// GLES2 device json server rendition

#ifndef ODGLES2JSONRENDITION
#define ODGLES2JSONRENDITION

#include "TD_PackPush.h"

#include "Tr/TrVisRenditionRecorder.h"
#include "TrXmlIdRegistratorImpl.h"
#include "JsonServer.h"
#include "ThreejsJSONExportDef.h"
#include "JsonObjectFormat.h"

#define OD_GLES2JSON_ENABLEPROGRAMDECISION

class OdGLES2JsonRenditionRedir;

/** \details
    <group ExRender_Windows_Classes> 
*/
class THREEJSJSONEXPORT_DLL OdGLES2JsonRendition : public OdTrVisRenditionDef, protected OdTrXmlRegistratorImpl::Traverser
{
  protected:
    // Settings
    ClientSettings m_clientSettings;
    // Id registrator
    mutable OdTrXmlRegistratorImpl m_idReg;
    // Output
    OdSmartPtr<OdGLES2JsonServer> m_pJson;
    OdSmartPtr<JsonObjectFormat> m_pJsonObj;
    // Current mode
    enum ProcMode
    {
      kIntermediate = 0,
      kGsUpdate
    } m_mode;
    // Redirection controller
    OdGLES2JsonRenditionRedir *m_pRedir;
    // Current output state
    OdGLES2JsonServer::OutputState m_outputState;
    // Current processing device
    OdTrVisRenderClient *m_pProcDevice;
    // Overall viewport state
    OdTrVisViewportDef m_overallViewport;
    // Lights list
    typedef std::map<OdTrVisLightId, OdTrVisLightDef> LightsMap;
    LightsMap m_lights;
  public:
    OdGLES2JsonRendition();
    ~OdGLES2JsonRendition();

    // JsonRendition methods

    OdIntPtr getClientSettings() const;
    void setClientSettings(OdIntPtr pClSet);

    OdIntPtr getJsonServer() const;
    void setJsonServer(OdIntPtr pJsonServer);

    OdGLES2JsonServer *json();
    const OdGLES2JsonServer *json() const;

    JsonObjectFormat *json_obj();
    const JsonObjectFormat *json_obj() const;

    void setRedirectionController(OdGLES2JsonRenditionRedir *pRedir) { m_pRedir = pRedir; }

    void setIntermediateMode();
    void setGsUpdateMode();
    bool isIntermediateMode() const { return (m_mode == kIntermediate); }
    bool isGsUpdateMode() const { return (m_mode == kGsUpdate); }
    void DropMetadata(const OdTrVisFlatMetafileContainer *pMetafile, const OdUInt64 &metafile_reg_id, const OdUInt64 &layer_id);

    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient *pDevice);
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient *pDevice, bool bRender = true);
    // Checks that vectorizer update started.
    virtual bool isWithinDeviceUpdate() const { return m_pProcDevice != NULL; }

    // Returns set of bit flags which indicate rendition capabilities/settings
    virtual OdUInt32 queryRenditionCaps(OdUInt32 requestFor = 0xFFFFFFFF) const;

    // Allocate new resource sharing provider compatible with rendition
    virtual OdTrVisSharingProviderClientPtr createSharingProvider();

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType &type) const;

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const;

    // GsFiler support
    virtual bool saveRenditionState(OdGsFiler *pFiler) const;
    virtual bool loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap);

    // Mid-level callbacks

    // Client information

    virtual void obtainClientSettings(ClientSettings &clientSets);

    // Surface

    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef);

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef);
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId overlayId);
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility);
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder);

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef);
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId viewportId, int nIndex, const OdTrVisViewportDef &pDef);
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId viewportId);

    // Primary viewports configuration

    // Called when viewport view parameters is modified.
    virtual void onViewportViewParamsModified(OdTrVisViewportId viewportId, const OdTrVisViewParamsDef &pDef);
    // Called when viewport lineweight settings is modified.
    virtual void onViewportLineweightsModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineweightsDef &pDef);

    // Extended viewports configuration

    // Called when background stream in viewport is changed.
    virtual void onViewportBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                             OdTrVisProgramId baseProgramId = kTrVisNegativeId);

    // Viewport default lights configuration

    // Called when viewport ambient light setting modified.
    virtual void onViewportAmbientLightChanged(OdTrVisViewportId viewportId, const OdTrVisMaterialColor &ambientLight);
    // Called when viewport default light setting modified.
    virtual void onViewportDefaultLightsChanged(OdTrVisViewportId viewportId, bool bDefaultLightsOn,
                                                const OdTrVisLightDef *pLightsList = NULL, OdUInt32 nLightsListEntries = 0);

    // Viewport lights management

    // Called when light source added into viewport light sources list.
    virtual void onViewportLightAttached(OdTrVisViewportId viewportId, OdTrVisLightId lightId,
                                         OdUInt32 nOverrides = 0, const OdTrVisLightDef &pOverrides = OdTrVisLightDef());
    // Called when light source removed from viewport light sources list.
    virtual void onViewportLightDetached(OdTrVisViewportId viewportId, OdTrVisLightId lightId);

    // Per-overlay viewport overrides

    // Called when viewport needs override view parameters for specific overlay.
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride,
                                             const OdTrVisViewParamsDef &pDef = OdTrVisViewParamsDef());
    // Called when viewport extents changed.
    virtual void onOverlayExtentsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef);

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen);

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef);
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId metafileId);
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef);
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility);
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight);
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade);

    // Lights

    // Called when new light source added.
    virtual void onLightAdded(OdTrVisLightId lightId, const OdTrVisLightDef &pDef);
    // Called when exist light source modified.
    virtual void onLightModified(OdTrVisLightId lightId, OdUInt32 nKindOfMod, const OdTrVisLightDef &pDef);
    // Called when exist light source deleted.
    virtual void onLightDeleted(OdTrVisLightId lightId);

    // State branches (highlighting, hide)

    // Called when new state branch added.
    virtual void onStateBranchAdded(OdTrVisStateBranchId hlBranchId, const OdTrVisStateBranchDef &pDef);
    // Called when exist state branch modified.
    virtual void onStateBranchModified(OdTrVisStateBranchId hlBranchId, const OdTrVisStateBranchDef &pDef,
                                    bool bChildsModified, bool bMarkersModified);
    // Called when exist state branch deleted.
    virtual void onStateBranchDeleted(OdTrVisStateBranchId hlBranchId);

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef);
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId textureId);

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef);
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId materialId);

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef);
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef);
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId);

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef);
    // Called when exist layer modified.
    //virtual void onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef);
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId layerId);
    // Called when layer viewport-dependent properties changed.
    virtual void onLayerVpDepPropsOverride(OdTrVisLayerId layerId, OdTrVisViewportId viewportId, bool bOverride,
                                           const OdTrVisLayerProps &pProps = OdTrVisLayerProps());

  protected:
    void beginDeviceProcessing(OdTrVisRenderClient *pDevice);
    void endDeviceProcessing(OdTrVisRenderClient *pDevice);
    OdTrVisRenderClient *processingDevice() const;

    bool isSupportCompositeMetafiles() const;
    bool isSupportVisualStyles() const;
    bool isSupportOverlayBuffers() const;

    virtual bool idRegTraverseCallback(OdTrVisRendition::DataType type, OdTrVisId id, OdTrVisUniqueID *pRegId);
    void finalizeJsonServerUsage();

    struct DDL2ndPassInfo
    {
      bool m_bScan;
      OdUInt32 m_nTransFirst, m_nTransLast;
      DDL2ndPassInfo() : m_bScan(false), m_nTransFirst(0xFFFFFFFF), m_nTransLast(0xFFFFFFFF) { }
    };
    bool DropDisplayList(const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, const OdTrVisId &ViewPortId, DDL2ndPassInfo *p2ndPass = NULL);
    void DropDisplayList2ndPass(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom,
                                const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, DDL2ndPassInfo &p2ndPass);
    void onMetafileOrderChangedImpl(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                    const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen, bool bFirstPass = true);

    void DropStateBranch(const OdTrVisStateBranchDef &pHlBranch, bool bDropChilds, bool bDropMarkers);
    void DropVisualStyleData(const OdTrVisVisualStyle &pVStyle, bool bDropNonModified = true);
    void DropLayerData(const OdTrVisLayerDef &pLayer, bool bDropOwnerId = true);
    void DropLayerProps(const OdTrVisLayerProps &pLayerProps);

    void OutputState(OdGLES2JsonServer::OutputState newState, bool bForce = false);

    static void DropViewParams(JsonObjectFormat *pJson, const OdTrVisViewParamsDef &pDef, const OdTrVisId &view_id);
  public:
    static void ParseMetafileStream(OdGLES2JsonServer *pJson, const OdTrVisFlatMetafileContainer *pMetafile)
    {
      OdSmartPtr<OdGLES2JsonRendition> res = OdRxObjectImpl<OdGLES2JsonRendition>::createObject();
      res->setJsonServer((OdIntPtr)pJson);
      //res->ParseMetafileStream(pMetafile);
      //res->DropMetadata(pMetafile);
    }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdGLES2JsonRenditionRedir : public OdTrVisRenditionRedir, protected OdTrVisRenditionRedir::IdRegistratorRedir
{
  protected:
    OdStaticRxObject<OdGLES2JsonRendition> m_jsonRendition;
    OdStaticRxObject<OdTrVisRenditionRecorder> m_recorder;
  public:
    OdGLES2JsonRenditionRedir()
      : OdTrVisRenditionRedir()
      , OdTrVisRenditionRedir::IdRegistratorRedir()
    {
      setRedirection(&m_recorder);
      setInformationalRedirection(&m_jsonRendition);
      setIdRegRedirection(m_recorder.idRegistrator());
      m_jsonRendition.setRedirectionController(this);
    }

    void setForRecord() { setRedirection(&m_recorder); }
    bool isSetForRecord() const { return (getRedirection() == &m_recorder); }
    void setForPlay() { setRedirection(&m_jsonRendition); }
    bool isSetForPlay() const { return (getRedirection() == &m_jsonRendition); }

    void dropRecords() { m_recorder.playRecords(&m_jsonRendition, true); }

    OdGLES2JsonRendition &jsonRendition() { return m_jsonRendition; }
    const OdGLES2JsonRendition &jsonRendition() const { return m_jsonRendition; }

    // Returns Id registrator, or NULL if no registration supported
    virtual OdTrVisRendition::IdRegistrator *idRegistrator() const
    {
      if (isSetForRecord() && getInformationalRedirection()->idRegistrator())
        return (OdTrVisRendition::IdRegistrator*)this;
      return getInformationalRedirection()->idRegistrator();
    }
  protected:
    // IdRegistrator override
    virtual OdTrVisUniqueID *get(DataType type, OdTrVisId id, bool bKill = false) const
    {
      OdTrVisRenditionRedir::IdRegistratorRedir::get(type, id, bKill);
      return getInformationalRedirection()->idRegistrator()->get(type, id);
    }
};

#include "TD_PackPop.h"

#endif // ODGLES2JSONRENDITION
