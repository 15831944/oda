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
// Xml server rendition

#ifndef ODTRXMLRENDITION
#define ODTRXMLRENDITION

#include "TD_PackPush.h"

#include "TrVisRenditionRecorder.h"
#include "TrRndLocalModuleRendition.h"
#include "TrXmlIdRegistratorImpl.h"
#include "XmlServer.h"
#include "TrXmlExports.h"

#define OD_TRXML_ENABLEPROGRAMDECISION

class OdTrXmlRenditionRedir;

/** \details
    <group ExRender_Windows_Classes> 
*/
class ODTRXML_EXPORT OdTrXmlRendition : public OdTrRndLocalModuleRendition, protected OdTrXmlRegistratorImpl::Traverser
{
  protected:
    // Settings
    ClientSettings m_clientSettings;
    // Id registrator
    mutable OdTrXmlRegistratorImpl m_idReg;
    // Output
    OdSmartPtr<OdTrXmlServer> m_pXml;
    // Current mode
    enum ProcMode
    {
      kIntermediate = 0,
      kGsUpdate
    } m_mode;
    // Redirection controller
    OdTrXmlRenditionRedir *m_pRedir;
    // Current output state
    OdTrXmlServer::OutputState m_outputState;
    // Current processing device
    OdTrVisRenderClient *m_pProcDevice;
  public:
    OdTrXmlRendition();
    ~OdTrXmlRendition();

    // XmlRendition methods

    OdIntPtr getClientSettings() const;
    void setClientSettings(OdIntPtr pClSet);

    OdIntPtr getXmlServer() const;
    void setXmlServer(OdIntPtr pXmlServer);

    OdTrXmlServer *xml();
    const OdTrXmlServer *xml() const;

    void setRedirectionController(OdTrXmlRenditionRedir *pRedir) { m_pRedir = pRedir; }

    void setIntermediateMode();
    void setGsUpdateMode();
    bool isIntermediateMode() const { return (m_mode == kIntermediate); }
    bool isGsUpdateMode() const { return (m_mode == kGsUpdate); }
    void DropMetadata(const OdTrVisFlatMetafileContainer *pMetafile);

    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient *pDevice);
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient *pDevice, bool bRender = true);
    // Checks that vectorizer update started.
    virtual bool isWithinDeviceUpdate() const { return m_pProcDevice != NULL; }

    // Called by underlying application to store frame changing information.
    virtual void onFrameSwitchInfo(OdUInt32 nInterval);

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

    // Returns rendering client settings.
    virtual void obtainClientSettings(ClientSettings &clientSets);

    // Surface

    // Called when output render surface parameters changed.
    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef);
    // Called when rendering surface needs to be invalidated forcibly.
    virtual void onSurfaceInvalidated();

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
    // Called when viewport visibility changed.
    virtual void onViewportVisibilityChanged(OdTrVisViewportId viewportId, bool bVisibility);

    // Primary viewports configuration

    // Called when viewport view parameters is modified.
    virtual void onViewportViewParamsModified(OdTrVisViewportId viewportId, const OdTrVisViewParamsDef &pDef);
    // Called when viewport linestyle settings is modified.
    virtual void onViewportLineStylesModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineStyleDef &pDef);
    // Called when viewport fading settings is modified.
    virtual void onViewportFadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportFadingDef &pDef);
    // Called when viewport lineweight settings is modified.
    virtual void onViewportLineweightsModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineweightsDef &pDef);
    // Called when viewport rendering mode is modified.
    virtual void onViewportShadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportShadingDef &pDef);
    // Called when viewport clipping region is modified.
    virtual void onViewportClipModified(OdTrVisViewportId viewportId, const OdTrVisViewportClipDef &pDef,
                                        bool bNrcClipModified = true, bool bBorderModified = true);

    // Extended viewports configuration

    // Called when background stream in viewport is changed.
    virtual void onViewportBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                             OdTrVisProgramId baseProgramId = kTrVisNegativeId);
    // Called when viewport fog changed.
    virtual void onViewportFogChanged(OdTrVisViewportId viewportId, const OdTrVisFogDef &pDef = OdTrVisFogDef());
    // Called when viewport sectioning modified.
    virtual void onViewportSectioningChanged(OdTrVisViewportId viewportId, const OdTrVisSectionDef &pDef = OdTrVisSectionDef());
    // Called when viewport anti-aliasing setting changed.
    virtual void onViewportAntiAliasingChanged(OdTrVisViewportId viewportId, OdUInt32 nAntiAliasing);

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
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 );

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
    virtual void onStateBranchAdded(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef);
    // Called when exist state branch modified.
    virtual void onStateBranchModified(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef,
                                       bool bChildsModified, bool bMarkersModified);
    // Called when exist state branch deleted.
    virtual void onStateBranchDeleted(OdTrVisStateBranchId stateBranchId);

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
    // Called when exist layer visibility changed.
    virtual void onLayerVisibilityChanged(OdTrVisLayerId layerId, bool bVisibility);
    // Called when exist layer fading changed.
    virtual void onLayerFadingChanged(OdTrVisLayerId layerId, bool bFade);
    // Called when layer viewport-dependent properties changed.
    virtual void onLayerVpDepPropsOverride(OdTrVisLayerId layerId, OdTrVisViewportId viewportId, bool bOverride,
                                           const OdTrVisLayerProps &pProps = OdTrVisLayerProps());

    // Extension objects

    // Called when new extension object added.
    virtual void onExtensionObjectAdded(OdTrVisExtensionId extId, const OdTrVisExtensionObject *pObj);
    // Called when exist extension object deleted.
    virtual void onExtensionObjectDeleted(OdTrVisExtensionId extId);
    // Called to send special extension object specific data.
    virtual void onExtensionObjectSpecificData(OdTrVisExtensionId extId, OdUInt32 nDataId, const OdTrVisPropertyDef &propDef);
    // Called to change extension object visibility.
    virtual void onExtensionObjectVisibilityChanged(OdTrVisExtensionId extId, bool bVisibility);

    // Groups

    // Called when new group added.
    virtual void onGroupAdded(OdTrVisGroupId groupId);
    // Called when exist group deleted.
    virtual void onGroupDeleted(OdTrVisGroupId groupId);
    // Called to attach metafiles to group.
    virtual void onGroupMetafilesAttached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles);
    // Called to detach metafiles from group (if no metafiles specified - clear entire list of groupped metafiles).
    virtual void onGroupMetafilesDetached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles);
    // Called to delete all groupped metafiles from graphics scene.
    //virtual void onGroupMetafilesDeleted(OdTrVisGroupId groupId);
    // Called to change visibility state for all groupped metafiles.
    virtual void onGroupMetafilesVisibilityChanged(OdTrVisGroupId groupId, bool bVisibility);
    // Called to change highlighting state for all groupped metafiles.
    virtual void onGroupMetafilesHighlightingChanged(OdTrVisGroupId groupId, bool bHighlight);
    // Called to change fading state for all groupped metafiles.
    virtual void onGroupMetafilesFadingChanged(OdTrVisGroupId groupId, bool bFade);

    // Properties

    // Metafile-related properties.
    // Called to attach property to metafile.
    virtual void onMetafilePropertyAttached(OdTrVisMetafileId metafileId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from metafile.
    virtual void onMetafilePropertyDetached(OdTrVisMetafileId metafileId, OdTrVisPropertyDef::PropertyType propType);
    // Group-related properties.
    // Called to attach property to group.
    virtual void onGroupPropertyAttached(OdTrVisGroupId groupId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from group.
    virtual void onGroupPropertyDetached(OdTrVisGroupId groupId, OdTrVisPropertyDef::PropertyType propType);
    // Layer-related properties.
    // Called to attach property to layer.
    virtual void onLayerPropertyAttached(OdTrVisLayerId layerId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from layer.
    virtual void onLayerPropertyDetached(OdTrVisLayerId layerId, OdTrVisPropertyDef::PropertyType propType);
    // VisualStyle-related properties.
    // Called to attach property to visual style.
    virtual void onVisualStylePropertyAttached(OdTrVisVisualStyleId visualStyleId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from visual style.
    virtual void onVisualStylePropertyDetached(OdTrVisVisualStyleId visualStyleId, OdTrVisPropertyDef::PropertyType propType);
    // Material-related properties.
    // Called to attach property to material.
    virtual void onMaterialPropertyAttached(OdTrVisMaterialId materialId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from material.
    virtual void onMaterialPropertyDetached(OdTrVisMaterialId materialId, OdTrVisPropertyDef::PropertyType propType);
    // Texture-related properties.
    // Called to attach property to texture.
    virtual void onTexturePropertyAttached(OdTrVisTextureId textureId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from texture.
    virtual void onTexturePropertyDetached(OdTrVisTextureId textureId, OdTrVisPropertyDef::PropertyType propType);
    // Light-related properties.
    // Called to attach property to light source.
    virtual void onLightPropertyAttached(OdTrVisLightId lightId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from light source.
    virtual void onLightPropertyDetached(OdTrVisLightId lightId, OdTrVisPropertyDef::PropertyType propType);
    // StateBranch-related properties.
    // Called to attach property to state branch.
    virtual void onStateBranchPropertyAttached(OdTrVisStateBranchId stateBranchId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from state branch.
    virtual void onStateBranchPropertyDetached(OdTrVisStateBranchId stateBranchId, OdTrVisPropertyDef::PropertyType propType);
    // Viewport-related properties.
    // Called to attach property to viewport.
    virtual void onViewportPropertyAttached(OdTrVisViewportId viewportId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from viewport.
    virtual void onViewportPropertyDetached(OdTrVisViewportId viewportId, OdTrVisPropertyDef::PropertyType propType);
    // Overlay-related properties.
    // Called to attach property to overlay.
    virtual void onOverlayPropertyAttached(OdTrVisOverlayId overlayId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from overlay.
    virtual void onOverlayPropertyDetached(OdTrVisOverlayId overlayId, OdTrVisPropertyDef::PropertyType propType);
    // ExtensionObject-related properties.
    // Called to attach property to extension object.
    virtual void onExtensionObjectPropertyAttached(OdTrVisExtensionId extId, const OdTrVisPropertyDef &propDef);
    // Called to detach property from extension object.
    virtual void onExtensionObjectPropertyDetached(OdTrVisExtensionId extId, OdTrVisPropertyDef::PropertyType propType);
  protected:
    void beginDeviceProcessing(OdTrVisRenderClient *pDevice);
    void endDeviceProcessing(OdTrVisRenderClient *pDevice);
    OdTrVisRenderClient *processingDevice() const;

    bool isSupportCompositeMetafiles() const;
    bool isSupportVisualStyles() const;
    bool isSupportOverlayBuffers() const;

    virtual bool idRegTraverseCallback(OdTrVisRendition::DataType type, OdTrVisId id, OdTrVisUniqueID *pRegId);
    void finalizeXmlServerUsage();

    struct DDL2ndPassInfo
    {
      bool m_bScan;
      OdUInt32 m_nTransFirst, m_nTransLast;
      DDL2ndPassInfo() : m_bScan(false), m_nTransFirst(0xFFFFFFFF), m_nTransLast(0xFFFFFFFF) { }
    };
    bool DropDisplayList(const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, DDL2ndPassInfo *p2ndPass = NULL);
    void DropDisplayList2ndPass(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom,
                                const OdTrVisDisplayId *pDispList, OdUInt32 nListLen, DDL2ndPassInfo &p2ndPass);
    void onMetafileOrderChangedImpl(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                    const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen, bool bFirstPass = true);
    void onMetafileOrderInheritanceImpl( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 );

    void DropPropertyData(const OdTrVisPropertyDef &propDef);
    void DropMaterialChannel(const OdTrVisMaterialChannelDef &pChannel);
    void DropMaterialChannelComponent(const OdTrVisMaterialChannelComponent &pComponent);
    void DropLightData(const OdTrVisLightDef &pLight, OdUInt32 nKindOf = 0xFFFFFFFF);
    void DropStateBranch(const OdTrVisStateBranchDef &pStateBranch, bool bDropChilds, bool bDropMarkers);
    void DropVisualStyleData(const OdTrVisVisualStyle &pVStyle, bool bDropNonModified = true);
    void DropLayerProps(const OdTrVisLayerProps &pLayerProps);
    void ParseMetafileStream(const OdTrVisFlatMetafileContainer *pMetafile);

    void OutputState(OdTrXmlServer::OutputState newState, bool bForce = false);

    static void DropViewParams(OdTrXmlServer *pXml, const OdTrVisViewParamsDef &pDef);
    void DropViewportData(const OdTrVisViewportDef &pDef, bool bDropOverallVpId = true);
  public:
    static void ParseMetafileStream(OdTrXmlServer *pXml, const OdTrVisFlatMetafileContainer *pMetafile)
    {
      OdSmartPtr<OdTrXmlRendition> res = OdRxObjectImpl<OdTrXmlRendition>::createObject();
      res->setXmlServer((OdIntPtr)pXml);
      res->ParseMetafileStream(pMetafile);
      res->DropMetadata(pMetafile);
    }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrXmlRenditionRedir : public OdTrVisRenditionRedir, protected OdTrVisRenditionRedir::IdRegistratorRedir
{
  protected:
    OdStaticRxObject<OdTrXmlRendition> m_xmlRendition;
    OdStaticRxObject<OdTrVisRenditionRecorder> m_recorder;
  public:
    OdTrXmlRenditionRedir()
      : OdTrVisRenditionRedir()
    {
      setRedirection(&m_recorder);
      setInformationalRedirection(&m_xmlRendition);
      setIdRegRedirection(m_recorder.idRegistrator());
      m_xmlRendition.setRedirectionController(this);
    }

    void setupClientContext(OdTrRndLocalRenditionClient *pClient, OdTrRndRenderModule *pModule)
    {
      m_xmlRendition.setupClientContext(pClient, pModule);
    }

    void setForRecord() { setRedirection(&m_recorder); }
    bool isSetForRecord() const { return (getRedirection() == &m_recorder); }
    void setForPlay() { setRedirection(&m_xmlRendition); }
    bool isSetForPlay() const { return (getRedirection() == &m_xmlRendition); }

    void dropRecords() { m_recorder.playRecords(&m_xmlRendition, true); }

    OdTrXmlRendition &xmlRendition() { return m_xmlRendition; }
    const OdTrXmlRendition &xmlRendition() const { return m_xmlRendition; }

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

#endif // ODTRXMLRENDITION
