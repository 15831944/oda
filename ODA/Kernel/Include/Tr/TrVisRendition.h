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
// Basic rendition interface

#ifndef ODTRVISRENDITION
#define ODTRVISRENDITION

#include "TD_PackPush.h"

#include "TrVisExtensionObject.h"
#include "TrVisInfoString.h"
#include "TrVisSharingProviderClient.h"
#include "TrVisSurfaceDef.h"
#include "TrVisViewportDef.h"
#include "TrVisMetafileDef.h"
#include "TrVisTextureDef.h"
#include "TrVisMaterialDef.h"
#include "TrVisLightDef.h"
#include "TrVisExtentsDef.h"
#include "TrVisFogDef.h"
#include "TrVisSectionDef.h"
#include "TrVisStateBranchDef.h"
#include "TrVisVisualStyle.h"
#include "TrVisLayerDef.h"
#include "TrVisPropertyDef.h"

#include "Ge/GeExtents3d.h"
#include "Gs/GsDCRectArray.h"

class OdTrVisRenderClient;

class OdTrVisUniqueID;
class OdGiDrawable;

class OdGsFiler;
class OdTrVisIdMap;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRendition : public OdRxObject
{
  public:
    // Core rendition data types
    enum DataType
    { //kNoData = 0, // No data type entrance
      kSurfaceData = 1, // Surface data
      kViewportData,    kViewportPropsData,
      kVpOverrideData,  kVpOverridePropsData,
      kMetafileData,    kMetafilePropsData,
      kLightData,       kLightPropsData,
      kStateBranchData, kStateBranchPropsData,
      kTextureData,     kTexturePropsData,
      kMaterialData,    kMaterialPropsData,
      kProgramData,     kPropertiesData,
      kVisualStyleData, kVisualStylePropsData,
      kLayerData,       kLayerPropsData,
      kOverlayData,     kOverlayPropsData,
      kGroupData,       kGroupPropsData,
      kExtensionData,   kExtensionPropsData,

      kNDataTypes, // Limited by 6 bits (63 - maximum value)
      kNGenericDataTypes = kNDataTypes >> 1 // Useful for IdMap's economy
    };
    // Mt handling types
    enum MtHandlerType
    {
      kMtSkip = 0, // Skip data
      kMtRedirect, // Redirect data without handling
      kMtGlobal,   // Redirect under global mutex
      kMtCompete,  // Redirect under specified mutex
      kMtRecord,   // Record till completion (placed under own recorder mutex)
      kMtComplex   // Redirect under complex set of mutexes (up to 4)
    };
    struct MtComplex
    { // It's recommended to set mutexes in strict order to avoid deadlocks.
      static inline MtHandlerType encode(DataType mutex1, DataType mutex2 = (DataType)0, DataType mutex3 = (DataType)0, DataType mutex4 = (DataType)0)
      { return (MtHandlerType)(kMtComplex + (mutex1 << 6) + (mutex2 << 12) + (mutex3 << 18) + (mutex4 << 24)); }
      static inline MtHandlerType decode(MtHandlerType handler) { return (MtHandlerType)(handler & 0x3F); }
      static inline DataType decode(MtHandlerType handler, OdUInt32 nElem) { return (DataType)((handler >> (6 * nElem)) & 0x3F); }
      static inline OdUInt32 toGenericDataType(DataType dt) { return dt >> 1; }
      static inline DataType fromGenericDataType(OdUInt32 nDt) { return (DataType)((nDt) ? (nDt << 1) : 1); }
    };
    // Special DisplayId codes, provide fast access to renderer functionality directly from display list
    enum DisplayIdCodes
    {
      kDisplayCodeZero        = 0x00, // Empty display code
      kDisplayCodeHighlight   = 0x01, // Execute highlighting procedure
      kDisplayCodeUnhighlight = 0x02, // Execute unhighlighting procedure
      kDisplayCodePushMatrix  = 0x03, // Execute push transformation matrix procedure
      kDisplayCodePopMatrix   = 0x04, // Execute pop transformation matrix procedure
      kDisplayCodeFade        = 0x05, // Execute fading procedure
      kDisplayCodeUnfade      = 0x06, // Execute unfading procedure
      kDisplayCodeHlMarkers   = 0x07, // Setup highlight markers
      kDisplayCodeVsMarkers   = 0x08, // Setup visibility markers
      kDisplayCodeDrawOrder   = 0x09, // Provide drawing order for upcoming geometry
      kDisplayCodeExploded    = 0x0A, // Initiates exploded union geometry rendering
      kDisplayCodeNonExploded = 0x0B, // Stops exploded union geometry rendering
      kDisplayCodeSelStyle    = 0x0C, // Setup selection style
      kDisplayCodeSelectionID = 0x0D, // Entity id for selection

      kDisplayCodeRange       = 0x10 // We reserve first 16 codes for special procedures
    };
    static const double kDisplayMultiDrawOrderMarker; // Marker for kDisplayCodeDrawOrder which means multi draw order (union stream)
    // Rendition flags
    enum RenditionFlags
    {
      kSupportPartialUpdate       = (1 << 0), // isSupportPartialUpdate
      kSupportSnapshotQueries     = (1 << 1), // isSupportSnapshotQueries
      kSupportCompositeMetafiles  = (1 << 2), // isSupportCompositeMetafiles
      kSupportVisualStyles        = (1 << 3), // isSupportVisualStyles
      kSupportGsStateSaving       = (1 << 4), // isSupportGsStateSaving
      kSupportOverlayBuffers      = (1 << 5), // isSupportOverlayBuffers
      kSupportGPUSelection        = (1 << 6), // isSupportGPUSelection
      kUpdateClientSectionOnly    = (1 << 7), // updateClientDependentSectionOnly
      kUpdateShareableSectionOnly = (1 << 8), // updateShareableSectionOnly
    };
  public:
    struct IdRegistrator
    {
      // Register persistent resource
      virtual void reg(DataType type, OdTrVisId id, OdDbStub *pPersId) = 0;
      // Register transient resource
      virtual void reg(DataType type, OdTrVisId id, const OdGiDrawable *pTransId) = 0;
      // Register persistent or transient resource
      void reg(DataType type, OdTrVisId id, OdDbStub *pPersId, const void *pTransId)
      { if (pPersId) reg(type, id, pPersId); else reg(type, id, reinterpret_cast<const OdGiDrawable*>(pTransId)); }
      // Copy resource registration
      virtual void reg(DataType type, OdTrVisId id, const OdTrVisUniqueID &uId) = 0;
      // Remove resource registration
      virtual void kill(DataType type, OdTrVisId id) = 0;
      // Access resource registration
      virtual OdTrVisUniqueID *get(DataType type, OdTrVisId id, bool bKill = false) const = 0;
    };
  public:
    // Client information
    struct ClientInfo
    {
      OdTrVisInfoString m_glVendor;     // OpenGL vendor
      OdTrVisInfoString m_glRenderer;   // OpenGL renderer name
      OdTrVisInfoString m_glVersion;    // OpenGL version
      OdTrVisInfoString m_glSLVersion;  // GLSL version
      OdTrVisInfoString m_glExtensions; // List of extensions
    };
    // Client settings
    struct ClientSettings
    {
      bool m_bObtained; // True if settings obtained from client.
      // Client information
      ClientInfo m_ciInfo;
      // Flags
      enum Flags
      {
        // Texture settings
        kNonPow2Textures = (1 << 0), // True if support non power of two textures.
        kNonPow2Wrapping = (1 << 1), // True if support all non power of two textures wrapping modes.
        kSupportsBGR     = (1 << 2), // True if support BGR textures format.
        kSupportsBGRA    = (1 << 3), // True if support BGRA textures format.
        kRequireBGR      = (1 << 4), // True if support only BGR/BGRA textures format.
        kOverrideLut     = (1 << 5), // True if client override Lut device settings.
        kLutMonochrome   = (1 << 6), // True if client prefer Lut monochrome textures.
        kLutPalette      = (1 << 7), // True if client prefer Lut 4-8bpp textures.
        kCubeMapping     = (1 << 8), // True if support cube textures.
        // Performance/quality degradation
        kShadeByVertex   = (1 << 9)  // Mobile devices could have serious performance loss in shaded modes, since per-pixel
                                     // calculations too massive. Enable this flag to use per-vertex lighting calculation
                                     // instead. This will degenerate lighting quality, but will increase performance.
      };
      OdUInt32 m_uFlags; // Client flags.
      // Texture limits
      OdUInt32 m_nMaxTextureSize; // Maximal texture size
      OdUInt32 m_nTextureAlignment; // Required default texture alignment
      // Lineweights
      float m_nMaxLineWidth; // Maximal line width
      float m_nMaxPointSize; // Maximal point size
      // System limits
      OdUInt32 m_nMaxTextures; // Maximal number of textures
      OdUInt32 m_nMaxLights;   // Maximal number of lights

      ClientSettings()
        : m_bObtained(false)
        , m_ciInfo()
        , m_uFlags(0)
        , m_nMaxTextureSize(2048)
        , m_nTextureAlignment(4)
        , m_nMaxLineWidth(1.0f)
        , m_nMaxPointSize(1.0f)
        , m_nMaxTextures(8)
        , m_nMaxLights(4)
      {
      }

      // Flag accessors

      // Textures

      bool isSupportNonPow2Textures() const { return GETBIT(m_uFlags, kNonPow2Textures); }
      void setSupportNonPow2Textures(bool bSet) { SETBIT(m_uFlags, kNonPow2Textures, bSet); }

      bool isSupportNonPow2TexturesWrapping() const { return GETBIT(m_uFlags, kNonPow2Wrapping); }
      void setSupportNonPow2TexturesWrapping(bool bSet) { SETBIT(m_uFlags, kNonPow2Wrapping, bSet); }

      bool isSupportBGR() const { return GETBIT(m_uFlags, kSupportsBGR); }
      void setSupportBGR(bool bSet) { SETBIT(m_uFlags, kSupportsBGR, bSet); }

      bool isSupportBGRA() const { return GETBIT(m_uFlags, kSupportsBGRA); }
      void setSupportBGRA(bool bSet) { SETBIT(m_uFlags, kSupportsBGRA, bSet); }

      bool isRequireBGR() const { return GETBIT(m_uFlags, kRequireBGR); }
      void setRequireBGR(bool bSet) { SETBIT(m_uFlags, kRequireBGR, bSet); }

      bool isLutOverride() const { return GETBIT(m_uFlags, kOverrideLut); }
      void setLutOverride(bool bSet) { SETBIT(m_uFlags, kOverrideLut, bSet); }

      bool isLutMonochrome() const { return GETBIT(m_uFlags, kLutMonochrome); }
      void setLutMonochrome(bool bSet) { SETBIT(m_uFlags, kLutMonochrome, bSet); }
      bool overridenLutMonochromeState(bool bDeviceSetting) const
        { return (isLutOverride()) ? isLutMonochrome() : bDeviceSetting; }

      bool isLutPalette() const { return GETBIT(m_uFlags, kLutPalette); }
      void setLutPalette(bool bSet) { SETBIT(m_uFlags, kLutPalette, bSet); }
      bool overridenLutPaletteState(bool bDeviceSetting) const
        { return (isLutOverride()) ? isLutPalette() : bDeviceSetting; }

      bool isSupportCubeMaps() const { return GETBIT(m_uFlags, kCubeMapping); }
      void setSupportCubeMaps(bool bSet) { SETBIT(m_uFlags, kCubeMapping, bSet); }

      // Performance/quality degradation

      bool isShadeByVertex() const { return GETBIT(m_uFlags, kShadeByVertex); }
      void setShadeByVertex(bool bSet) { SETBIT(m_uFlags, kShadeByVertex, bSet); }
    };
  public:
    ODRX_USING_HEAP_OPERATORS(OdRxObject);

    OdTrVisRendition() { }
    ~OdTrVisRendition() { }

    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient *pDevice) = 0;
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient *pDevice, bool bRender = true) = 0;
    // Checks that vectorizer update started.
    virtual bool isWithinDeviceUpdate() const = 0;

    // Called by underlying application to store frame changing information.
    virtual void onFrameSwitchInfo(OdUInt32 nInterval) = 0;

    // Returns set of bit flags which indicate rendition capabilities/settings
    virtual OdUInt32 queryRenditionCaps(OdUInt32 requestFor = 0xFFFFFFFF) const = 0;
    // Rendition capabilities as separate methods
    // Called to check does rendition supports partial update (have sense for local renditions only).
    bool isSupportPartialUpdate() const { return GETBIT(queryRenditionCaps(kSupportPartialUpdate), kSupportPartialUpdate); }
    // Check does rendition support snapshot queries
    bool isSupportSnapshotQueries() const { return GETBIT(queryRenditionCaps(kSupportSnapshotQueries), kSupportSnapshotQueries); }
    // Check does rendition supports composite metafiles
    bool isSupportCompositeMetafiles() const { return GETBIT(queryRenditionCaps(kSupportCompositeMetafiles), kSupportCompositeMetafiles); }
    // Check does rendition supports visual styles
    bool isSupportVisualStyles() const { return GETBIT(queryRenditionCaps(kSupportVisualStyles), kSupportVisualStyles); }
    // Check does rendition supports GsFiler-compatible state saving/loading
    bool isSupportGsStateSaving() const { return GETBIT(queryRenditionCaps(kSupportGsStateSaving), kSupportGsStateSaving); }
    // Check does rendition supports multiple overlapping rendering buffers
    bool isSupportOverlayBuffers() const { return GETBIT(queryRenditionCaps(kSupportOverlayBuffers), kSupportOverlayBuffers); }
    // Check does rendition supports GPU selection
    bool isSupportGPUSelection() const { return GETBIT(queryRenditionCaps(kSupportGPUSelection), kSupportGPUSelection); }
    // Update client-dependent section only
    bool updateClientDependentSectionOnly() const { return GETBIT(queryRenditionCaps(kUpdateClientSectionOnly), kUpdateClientSectionOnly); }
    // Update shareable section only
    bool updateShareableSectionOnly() const { return GETBIT(queryRenditionCaps(kUpdateShareableSectionOnly), kUpdateShareableSectionOnly); }

    // Partial update support for local renditions:
    // Called to check does specified viewport/overlay combination support partial update query.
    virtual bool onPartialUpdateCheckLimit(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId) = 0;
    // Called to setup invalid rectangles and overlays on partial update.
    virtual void onPartialUpdateInvalidRects(OdTrVisOverlayId overlayId, const OdGsDCRectArray &invalidRects, bool bOverlayInvalid = false) = 0;
    // Called on partial update query to set entire actual visibilities list.
    virtual void onPartialUpdateList(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries) = 0;
    // Can be called by client side to check that rendition supports direct rendering ability.
    virtual bool queryPartialUpdateDirectInvocation(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdTrVisDirectRenderPartialUpdateInvocationPtr &pInvocationPtr) = 0;
    // Can be called by client side to setup direct invocation interface for partial update instead of partial update list usage.
    virtual void onPartialUpdateDirectInvocation(const OdTrVisDirectRenderPartialUpdateInvocation *pInvocation, OdTrVisRenderDataDirectAccessor *pAccessor) = 0;

    // Query snapshot
    virtual void querySnapshot(OdUInt8 *pScanlines, long minX, long minY, long nWidth, long nHeight) = 0;

    /** \details
      Renders viewport to selection texture. Return top object id with (x, y) position in viewport
      \param id [in] Processing viewport id
      \param x [in] X coordinate in the screen space
      \param y [in] Y coordinate in the screen space
      \param outIDs [out] id chain of selected object(for example entity id and sub entity id). Empty if nothing selected
      \param outMarker [out] marker in selected object.
    */
    virtual void getSelectedObject(OdTrVisViewportId id, OdUInt32 x, OdUInt32 y, OdArray<OdTrVisId> &outIDs, OdTrVisGsMarker *outMarker = NULL) = 0;
    /** \details
      Checks if selection texture is valid.
      \param pDevice [in] Processing device.
      \param id [in] Processing viewport id.
      \returns true if selection texture is valid or false otherwise.
    */
    virtual bool isSelectTextureValid(OdTrVisViewportId id) const = 0;

    // Allocate new resource sharing provider compatible with rendition
    virtual OdTrVisSharingProviderClientPtr createSharingProvider() = 0;

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType &type) const = 0;

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const = 0;

    // Returns true if metafile streams accessible directly, returns stream by Id
    virtual bool queryMetafileStream(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId,
                                     OdTrVisDisplayId displayId, const OdTrVisMetafileContainer **pStream) = 0;
    // Unlock metafile stream, queried by queryMetafileStream method
    virtual bool unlockMetafileStream(const OdTrVisMetafileContainer *pStream) = 0;

    // Returns non-kTrVisNegativeId in case if vectorizer supports ProgramID's embedded into metafile stream
    virtual OdTrVisProgramId queryProgramId(OdGsView::RenderMode mode, OdTrVisVisualStyleId vsId = kTrVisNegativeId,
                                            OdTrVisGeomType geomType = OdTrVisGeomType_Default, OdUInt16 attrFlags = 0, OdUInt16 shadingFlags = 0) const = 0;

    // GsFiler support
    virtual bool saveRenditionState(OdGsFiler *pFiler) const = 0;
    virtual bool loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap) = 0;

    // Mid-level callbacks

    // Client information

    // Returns rendering client settings.
    virtual void obtainClientSettings(ClientSettings &clientSets) = 0;

    // Surface

    // Called when output render surface parameters changed.
    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef) = 0;
    // Called when rendering surface needs to be invalidated forcibly.
    virtual void onSurfaceInvalidated() = 0;

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef) = 0;
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId overlayId) = 0;
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility) = 0;
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder) = 0;

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef) = 0;
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId viewportId, int nIndex, const OdTrVisViewportDef &pDef) = 0;
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId viewportId) = 0;
    // Called when viewport visibility changed.
    virtual void onViewportVisibilityChanged(OdTrVisViewportId viewportId, bool bVisibility) = 0;

    // Primary viewports configuration

    // Called when viewport view parameters is modified.
    virtual void onViewportViewParamsModified(OdTrVisViewportId viewportId, const OdTrVisViewParamsDef &pDef) = 0;
    // Called when viewport linestyle settings is modified.
    virtual void onViewportLineStylesModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineStyleDef &pDef) = 0;
    // Called when viewport fading settings is modified.
    virtual void onViewportFadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportFadingDef &pDef) = 0;
    // Called when viewport lineweight settings is modified.
    virtual void onViewportLineweightsModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineweightsDef &pDef) = 0;
    // Called when viewport rendering mode is modified.
    virtual void onViewportShadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportShadingDef &pDef) = 0;
    // Called when viewport clipping region is modified.
    virtual void onViewportClipModified(OdTrVisViewportId viewportId, const OdTrVisViewportClipDef &pDef,
                                        bool bNrcClipModified = true, bool bBorderModified = true) = 0;

    // Extended viewports configuration

    // Called when background stream in viewport is changed.
    virtual void onViewportBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                             OdTrVisProgramId baseProgramId = kTrVisNegativeId) = 0;
    // Called when viewport fog changed.
    virtual void onViewportFogChanged(OdTrVisViewportId viewportId, const OdTrVisFogDef &pDef = OdTrVisFogDef()) = 0;
    // Called when viewport sectioning modified.
    virtual void onViewportSectioningChanged(OdTrVisViewportId viewportId, const OdTrVisSectionDef &pDef = OdTrVisSectionDef()) = 0;
    // Called when viewport anti-aliasing setting changed.
    virtual void onViewportAntiAliasingChanged(OdTrVisViewportId viewportId, OdUInt32 nAntiAliasing) = 0;

    // Viewport default lights configuration

    // Called when viewport ambient light setting modified.
    virtual void onViewportAmbientLightChanged(OdTrVisViewportId viewportId, const OdTrVisMaterialColor &ambientLight) = 0;
    // Called when viewport default light setting modified.
    virtual void onViewportDefaultLightsChanged(OdTrVisViewportId viewportId, bool bDefaultLightsOn,
                                                const OdTrVisLightDef *pLightsList = NULL, OdUInt32 nLightsListEntries = 0) = 0;

    // Viewport lights management

    // Called when light source added into viewport light sources list.
    virtual void onViewportLightAttached(OdTrVisViewportId viewportId, OdTrVisLightId lightId,
                                         OdUInt32 nOverrides = 0, const OdTrVisLightDef &pOverrides = OdTrVisLightDef()) = 0;
    // Called when light source removed from viewport light sources list.
    virtual void onViewportLightDetached(OdTrVisViewportId viewportId, OdTrVisLightId lightId) = 0;

    // Per-overlay viewport overrides

    // Called when viewport needs override view parameters for specific overlay.
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride,
                                             const OdTrVisViewParamsDef &pDef = OdTrVisViewParamsDef()) = 0;
    // Called when viewport extents changed.
    virtual void onOverlayExtentsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef) = 0;

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed (for obtain full list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries) = 0;
    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen) = 0;
    // Called when order of metafiles can be inherit from other source.
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 ) = 0;

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef) = 0;
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId metafileId) = 0;
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef) = 0;
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility) = 0;
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight) = 0;
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade) = 0;

    // Lights

    // Called when new light source added.
    virtual void onLightAdded(OdTrVisLightId lightId, const OdTrVisLightDef &pDef) = 0;
    // Called when exist light source modified.
    virtual void onLightModified(OdTrVisLightId lightId, OdUInt32 nKindOfMod, const OdTrVisLightDef &pDef) = 0;
    // Called when exist light source deleted.
    virtual void onLightDeleted(OdTrVisLightId lightId) = 0;

    // State branches (highlighting, hide)

    // Called when new state branch added.
    virtual void onStateBranchAdded(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef) = 0;
    // Called when exist state branch modified.
    virtual void onStateBranchModified(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef,
                                       bool bChildsModified, bool bMarkersModified) = 0;
    // Called when exist state branch deleted.
    virtual void onStateBranchDeleted(OdTrVisStateBranchId stateBranchId) = 0;

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef) = 0;
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId textureId) = 0;

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef) = 0;
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId materialId) = 0;

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef) = 0;
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef) = 0;
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId) = 0;

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef) = 0;
    // Called when exist layer modified.
    //virtual void onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef) = 0;
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId layerId) = 0;
    // Called when exist layer visibility changed.
    virtual void onLayerVisibilityChanged(OdTrVisLayerId layerId, bool bVisibility) = 0;
    // Called when exist layer fading changed.
    virtual void onLayerFadingChanged(OdTrVisLayerId layerId, bool bFade) = 0;
    // Called when layer viewport-dependent properties changed.
    virtual void onLayerVpDepPropsOverride(OdTrVisLayerId layerId, OdTrVisViewportId viewportId, bool bOverride,
                                           const OdTrVisLayerProps &pProps = OdTrVisLayerProps()) = 0;

    // Extension objects

    // Called when new extension object added.
    virtual void onExtensionObjectAdded(OdTrVisExtensionId extId, const OdTrVisExtensionObject *pObj) = 0;
    // Called when exist extension object deleted.
    virtual void onExtensionObjectDeleted(OdTrVisExtensionId extId) = 0;
    // Called to send special extension object specific data.
    virtual void onExtensionObjectSpecificData(OdTrVisExtensionId extId, OdUInt32 nDataId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to change extension object visibility.
    virtual void onExtensionObjectVisibilityChanged(OdTrVisExtensionId extId, bool bVisibility) = 0;

    // Groups

    // Called when new group added.
    virtual void onGroupAdded(OdTrVisGroupId groupId) = 0;
    // Called when exist group deleted.
    virtual void onGroupDeleted(OdTrVisGroupId groupId) = 0;
    // Called to attach metafiles to group.
    virtual void onGroupMetafilesAttached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles) = 0;
    // Called to detach metafiles from group (if no metafiles specified - clear entire list of groupped metafiles).
    virtual void onGroupMetafilesDetached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles) = 0;
    // Called to delete all groupped metafiles from graphics scene.
    //virtual void onGroupMetafilesDeleted(OdTrVisGroupId groupId) = 0;
    // Called to change visibility state for all groupped metafiles.
    virtual void onGroupMetafilesVisibilityChanged(OdTrVisGroupId groupId, bool bVisibility) = 0;
    // Called to change highlighting state for all groupped metafiles.
    virtual void onGroupMetafilesHighlightingChanged(OdTrVisGroupId groupId, bool bHighlight) = 0;
    // Called to change fading state for all groupped metafiles.
    virtual void onGroupMetafilesFadingChanged(OdTrVisGroupId groupId, bool bFade) = 0;

    // Properties

    // Metafile-related properties.
    // Called to attach property to metafile.
    virtual void onMetafilePropertyAttached(OdTrVisMetafileId metafileId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from metafile.
    virtual void onMetafilePropertyDetached(OdTrVisMetafileId metafileId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // Group-related properties.
    // Called to attach property to group.
    virtual void onGroupPropertyAttached(OdTrVisGroupId groupId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from group.
    virtual void onGroupPropertyDetached(OdTrVisGroupId groupId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // Layer-related properties.
    // Called to attach property to layer.
    virtual void onLayerPropertyAttached(OdTrVisLayerId layerId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from layer.
    virtual void onLayerPropertyDetached(OdTrVisLayerId layerId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // VisualStyle-related properties.
    // Called to attach property to visual style.
    virtual void onVisualStylePropertyAttached(OdTrVisVisualStyleId visualStyleId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from visual style.
    virtual void onVisualStylePropertyDetached(OdTrVisVisualStyleId visualStyleId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // Material-related properties.
    // Called to attach property to material.
    virtual void onMaterialPropertyAttached(OdTrVisMaterialId materialId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from material.
    virtual void onMaterialPropertyDetached(OdTrVisMaterialId materialId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // Texture-related properties.
    // Called to attach property to texture.
    virtual void onTexturePropertyAttached(OdTrVisTextureId textureId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from texture.
    virtual void onTexturePropertyDetached(OdTrVisTextureId textureId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // Light-related properties.
    // Called to attach property to light source.
    virtual void onLightPropertyAttached(OdTrVisLightId lightId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from light source.
    virtual void onLightPropertyDetached(OdTrVisLightId lightId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // HlBranch-related properties.
    // Called to attach property to state branch.
    virtual void onStateBranchPropertyAttached(OdTrVisStateBranchId stateBranchId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from state branch.
    virtual void onStateBranchPropertyDetached(OdTrVisStateBranchId stateBranchId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // Viewport-related properties.
    // Called to attach property to viewport.
    virtual void onViewportPropertyAttached(OdTrVisViewportId viewportId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from viewport.
    virtual void onViewportPropertyDetached(OdTrVisViewportId viewportId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // Overlay-related properties.
    // Called to attach property to overlay.
    virtual void onOverlayPropertyAttached(OdTrVisOverlayId overlayId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from overlay.
    virtual void onOverlayPropertyDetached(OdTrVisOverlayId overlayId, OdTrVisPropertyDef::PropertyType propType) = 0;
    // ExtensionObject-related properties.
    // Called to attach property to extension object.
    virtual void onExtensionObjectPropertyAttached(OdTrVisExtensionId extId, const OdTrVisPropertyDef &propDef) = 0;
    // Called to detach property from extension object.
    virtual void onExtensionObjectPropertyDetached(OdTrVisExtensionId extId, OdTrVisPropertyDef::PropertyType propType) = 0;
};

typedef OdSmartPtr<OdTrVisRendition> OdTrVisRenditionPtr;

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenditionDef : public OdTrVisRendition
{
  protected:
    struct IdRegistratorStub : public IdRegistrator
    {
      virtual void reg(DataType /*type*/, OdTrVisId /*id*/, OdDbStub * /*pPersId*/) { }
      virtual void reg(DataType /*type*/, OdTrVisId /*id*/, const OdGiDrawable * /*pTransId*/) { }
      virtual void reg(DataType /*type*/, OdTrVisId /*id*/, const OdTrVisUniqueID & /*uId*/) { }
      virtual void kill(DataType /*type*/, OdTrVisId /*id*/) { }
      virtual OdTrVisUniqueID *get(DataType /*type*/, OdTrVisId /*id*/, bool /*bKill*/ = false) const { return NULL; }
    } m_IdRegistratorStub;
  public:
    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient * /*pDevice*/) { }
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient * /*pDevice*/, bool /*bRender*/ = true) { }
    // Checks that vectorizer update started.
    virtual bool isWithinDeviceUpdate() const { return false; }

    // Called by underlying application to store frame changing information.
    virtual void onFrameSwitchInfo(OdUInt32 /*nInterval*/) { }

    // Returns set of bit flags which indicate rendition capabilities/settings
    virtual OdUInt32 queryRenditionCaps(OdUInt32 /*requestFor*/ = 0xFFFFFFFF) const { return 0; }

    // Partial update support for local renditions:
    // Called to check does specified viewport/overlay combination support partial update query.
    virtual bool onPartialUpdateCheckLimit(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/) { return false; }
    // Called to setup invalid rectangles and overlays on partial update.
    virtual void onPartialUpdateInvalidRects(OdTrVisOverlayId /*overlayId*/, const OdGsDCRectArray & /*invalidRects*/, bool /*bOverlayInvalid*/ = false) { }
    // Called on partial update query to set entire actual visibilities list.
    virtual void onPartialUpdateList(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, const OdTrVisDisplayId * /*pList*/, OdUInt32 /*nEntries*/) { }
    // Can be called by client side to check that rendition supports direct rendering ability.
    virtual bool queryPartialUpdateDirectInvocation(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, OdTrVisDirectRenderPartialUpdateInvocationPtr & /*pInvocationPtr*/) { return false; }
    // Can be called by client side to setup direct invocation interface for partial update instead of partial update list usage.
    virtual void onPartialUpdateDirectInvocation(const OdTrVisDirectRenderPartialUpdateInvocation * /*pInvocation*/, OdTrVisRenderDataDirectAccessor * /*pAccessor*/) { }

    // Query snapshot
    virtual void querySnapshot(OdUInt8 * /*pScanlines*/, long /*minX*/, long /*minY*/, long /*nWidth*/, long /*nHeight*/) { }

    // Renders viewport to selection texture. Return top object id with (x, y) position in viewport.
    virtual void getSelectedObject(OdTrVisViewportId /*id*/, OdUInt32 /*x*/, OdUInt32 /*y*/, OdArray<OdTrVisId> & /*outIDs*/, OdTrVisGsMarker * /*outMarker*/ = NULL) { }
    // Validates selection texture.
    virtual bool isSelectTextureValid(OdTrVisViewportId /*id*/) const { return false; }

    // Allocate new resource sharing provider compatible with rendition
    virtual OdTrVisSharingProviderClientPtr createSharingProvider() { return OdTrVisSharingProviderClientPtr(); }

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType & /*type*/) const { return kMtSkip; }

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const { return NULL; }

    // Returns true if metafile streams accessible directly, returns stream by Id
    virtual bool queryMetafileStream(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/,
                                     OdTrVisDisplayId /*displayId*/, const OdTrVisMetafileContainer ** /*pStream*/) { return false; }
    // Unlock metafile stream, queried by queryMetafileStream method
    virtual bool unlockMetafileStream(const OdTrVisMetafileContainer * /*pStream*/) { return false; }

    // Returns non-kTrVisNegativeId in case if vectorizer supports ProgramID's embedded into metafile stream
    virtual OdTrVisProgramId queryProgramId(OdGsView::RenderMode /*mode*/, OdTrVisVisualStyleId /*vsId*/ = kTrVisNegativeId,
                                            OdTrVisGeomType /*geomType*/ = OdTrVisGeomType_Default, OdUInt16 /*attrFlags*/ = 0, OdUInt16 /*shadingFlags*/ = 0) const
    { return kTrVisNegativeId; }

    // GsFiler support
    virtual bool saveRenditionState(OdGsFiler * /*pFiler*/) const { return false; }
    virtual bool loadRenditionState(OdGsFiler * /*pFiler*/, OdTrVisIdMap * /*pIdMap*/) { return false; }

    // Mid-level callbacks

    // Client information

    // Returns rendering client settings.
    virtual void obtainClientSettings(ClientSettings & /*clientSets*/) { }

    // Surface

    // Called when output render surface parameters changed.
    virtual void onSurfaceChanged(const OdTrVisSurfaceDef & /*pDef*/) { }
    // Called when rendering surface needs to be invalidated forcibly.
    virtual void onSurfaceInvalidated() { }

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId /*overlayId*/, const OdTrVisOverlayDef & /*pDef*/) { }
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId /*overlayId*/) { }
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId /*overlayId*/, bool /*bVisibility*/) { }
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId /*overlayId*/, OdInt16 /*nRenderOrder*/) { }

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId /*viewportId*/, const OdTrVisViewportDef & /*pDef*/) { }
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId /*viewportId*/, int /*nIndex*/, const OdTrVisViewportDef & /*pDef*/) { }
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId /*viewportId*/) { }
    // Called when viewport visibility changed.
    virtual void onViewportVisibilityChanged(OdTrVisViewportId /*viewportId*/, bool /*bVisibility*/) { }

    // Primary viewports configuration

    // Called when viewport view parameters is modified.
    virtual void onViewportViewParamsModified(OdTrVisViewportId /*viewportId*/, const OdTrVisViewParamsDef & /*pDef*/) { }
    // Called when viewport linestyle settings is modified.
    virtual void onViewportLineStylesModified(OdTrVisViewportId /*viewportId*/, const OdTrVisViewportLineStyleDef & /*pDef*/) { }
    // Called when viewport fading settings is modified.
    virtual void onViewportFadingModified(OdTrVisViewportId /*viewportId*/, const OdTrVisViewportFadingDef & /*pDef*/) { }
    // Called when viewport lineweight settings is modified.
    virtual void onViewportLineweightsModified(OdTrVisViewportId /*viewportId*/, const OdTrVisViewportLineweightsDef & /*pDef*/) { }
    // Called when viewport rendering mode is modified.
    virtual void onViewportShadingModified(OdTrVisViewportId /*viewportId*/, const OdTrVisViewportShadingDef & /*pDef*/) { }
    // Called when viewport clipping region is modified.
    virtual void onViewportClipModified(OdTrVisViewportId /*viewportId*/, const OdTrVisViewportClipDef & /*pDef*/,
                                        bool /*bNrcClipModified*/ = true, bool /*bBorderModified*/ = true) { }

    // Extended viewports configuration

    // Called when background stream in viewport is changed.
    virtual void onViewportBackgroundChanged(OdTrVisViewportId /*viewportId*/, OdTrVisFlatMetafileContainerPtr /*pStream*/,
                                             OdTrVisProgramId /*baseProgramId*/ = kTrVisNegativeId) { }
    // Called when viewport fog changed.
    virtual void onViewportFogChanged(OdTrVisViewportId /*viewportId*/, const OdTrVisFogDef & /*pDef*/ = OdTrVisFogDef()) { }
    // Called when viewport sectioning modified.
    virtual void onViewportSectioningChanged(OdTrVisViewportId /*viewportId*/, const OdTrVisSectionDef & /*pDef*/ = OdTrVisSectionDef()) { }
    // Called when viewport anti-aliasing setting changed.
    virtual void onViewportAntiAliasingChanged(OdTrVisViewportId /*viewportId*/, OdUInt32 /*nAntiAliasing*/) { }

    // Viewport default lights configuration

    // Called when viewport ambient light setting modified.
    virtual void onViewportAmbientLightChanged(OdTrVisViewportId /*viewportId*/, const OdTrVisMaterialColor & /*ambientLight*/) { }
    // Called when viewport default light setting modified.
    virtual void onViewportDefaultLightsChanged(OdTrVisViewportId /*viewportId*/, bool /*bDefaultLightsOn*/,
                                                const OdTrVisLightDef * /*pLightsList*/ = NULL, OdUInt32 /*nLightsListEntries*/ = 0) { }

    // Viewport lights management

    // Called when light source added into viewport light sources list.
    virtual void onViewportLightAttached(OdTrVisViewportId /*viewportId*/, OdTrVisLightId /*lightId*/,
                                         OdUInt32 /*nOverrides*/ = 0, const OdTrVisLightDef & /*pOverrides*/ = OdTrVisLightDef()) { }
    // Called when light source removed from viewport light sources list.
    virtual void onViewportLightDetached(OdTrVisViewportId /*viewportId*/, OdTrVisLightId /*lightId*/) { }

    // Per-overlay viewport overrides

    // Called when viewport needs override view parameters for specific overlay.
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, bool /*bOverride*/,
                                             const OdTrVisViewParamsDef & /*pDef*/ = OdTrVisViewParamsDef()) { }
    // Called when viewport extents changed.
    virtual void onOverlayExtentsOverride(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, const OdTrVisExtentsDef & /*pDef*/) { }

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed.
    virtual void onMetafileOrderChanged(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, const OdTrVisDisplayId * /*pList*/, OdUInt32 /*nEntries*/) { }
    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId /*viewportId*/, OdTrVisOverlayId /*overlayId*/, OdUInt32 /*nInvalidFrom*/, OdUInt32 /*nInvalidLen*/,
                                        const OdTrVisDisplayId * /*pValidFrom*/, OdUInt32 /*nValidLen*/) { }
    // Called when order of metafiles can be inherit from other source.
    virtual void onMetafileOrderInheritance(OdTrVisViewportId /*viewportId1*/, OdTrVisOverlayId /*overlayId1*/, OdTrVisViewportId /*viewportId2*/) { }

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef & /*pDef*/) { return (OdTrVisDisplayId)metafileId; }
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId /*metafileId*/) { }
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId /*metafileId*/, const OdTrVisMetafileDef & /*pDef*/) { }
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId /*metafileId*/, bool /*bVisibility*/) { }
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId /*metafileId*/, bool /*bHighlight*/) { }
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId /*metafileId*/, bool /*bFade*/) { }

    // Lights

    // Called when new light source added.
    virtual void onLightAdded(OdTrVisLightId /*lightId*/, const OdTrVisLightDef & /*pDef*/) { }
    // Called when exist light source modified.
    virtual void onLightModified(OdTrVisLightId /*lightId*/, OdUInt32 /*nKindOfMod*/, const OdTrVisLightDef & /*pDef*/) { }
    // Called when exist light source deleted.
    virtual void onLightDeleted(OdTrVisLightId /*lightId*/) { }

    // State branches (highlighting, hide)

    // Called when new state branch added.
    virtual void onStateBranchAdded(OdTrVisStateBranchId /*stateBranchId*/, const OdTrVisStateBranchDef & /*pDef*/) { }
    // Called when exist state branch modified.
    virtual void onStateBranchModified(OdTrVisStateBranchId /*stateBranchId*/, const OdTrVisStateBranchDef & /*pDef*/,
                                       bool /*bChildsModified*/, bool /*bMarkersModified*/) { }
    // Called when exist state branch deleted.
    virtual void onStateBranchDeleted(OdTrVisStateBranchId /*stateBranchId*/) { }

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId /*textureId*/, const OdTrVisTextureDef & /*pDef*/) { }
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId /*textureId*/) { }

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId /*materialId*/, const OdTrVisMaterialDef & /*pDef*/) { }
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId /*materialId*/) { }

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId /*visualStyleId*/, const OdTrVisVisualStyle & /*pDef*/) { }
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId /*visualStyleId*/, const OdTrVisVisualStyle & /*pDef*/) { }
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId /*visualStyleId*/) { }

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId /*layerId*/, const OdTrVisLayerDef & /*pDef*/) { }
    // Called when exist layer modified.
    //virtual void onLayerModified(OdTrVisLayerId /*layerId*/, const OdTrVisLayerDef & /*pDef*/) { }
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId /*layerId*/) { }
    // Called when exist layer visibility changed.
    virtual void onLayerVisibilityChanged(OdTrVisLayerId /*layerId*/, bool /*bVisibility*/) { }
    // Called when exist layer fading changed.
    virtual void onLayerFadingChanged(OdTrVisLayerId /*layerId*/, bool /*bFade*/) { }
    // Called when layer viewport-dependent properties changed.
    virtual void onLayerVpDepPropsOverride(OdTrVisLayerId /*layerId*/, OdTrVisViewportId /*viewportId*/, bool /*bOverride*/,
                                           const OdTrVisLayerProps & /*pProps*/ = OdTrVisLayerProps()) { }

    // Extension objects

    // Called when new extension object added.
    virtual void onExtensionObjectAdded(OdTrVisExtensionId /*extId*/, const OdTrVisExtensionObject* /*pObj*/) { }
    // Called when exist extension object deleted.
    virtual void onExtensionObjectDeleted(OdTrVisExtensionId /*extId*/) { }
    // Called to send special extension object specific data.
    virtual void onExtensionObjectSpecificData(OdTrVisExtensionId /*extId*/, OdUInt32 /*nDataId*/, const OdTrVisPropertyDef& /*propDef*/) { }
    // Called to change extension object visibility.
    virtual void onExtensionObjectVisibilityChanged(OdTrVisExtensionId /*extId*/, bool /*bVisibility*/) { }

    // Groups

    // Called when new group added.
    virtual void onGroupAdded(OdTrVisGroupId /*groupId*/) { }
    // Called when exist group deleted.
    virtual void onGroupDeleted(OdTrVisGroupId /*groupId*/) { }
    // Called to attach metafiles to group.
    virtual void onGroupMetafilesAttached(OdTrVisGroupId /*groupId*/, const OdTrVisMetafileId * /*pMetafilesList*/, OdUInt32 /*nMetafiles*/) { }
    // Called to detach metafiles from group (if no metafiles specified - clear entire list of groupped metafiles).
    virtual void onGroupMetafilesDetached(OdTrVisGroupId /*groupId*/, const OdTrVisMetafileId * /*pMetafilesList*/, OdUInt32 /*nMetafiles*/) { }
    // Called to delete all groupped metafiles from graphics scene.
    //virtual void onGroupMetafilesDeleted(OdTrVisGroupId /*groupId*/) { }
    // Called to change visibility state for all groupped metafiles.
    virtual void onGroupMetafilesVisibilityChanged(OdTrVisGroupId /*groupId*/, bool /*bVisibility*/) { }
    // Called to change highlighting state for all groupped metafiles.
    virtual void onGroupMetafilesHighlightingChanged(OdTrVisGroupId /*groupId*/, bool /*bHighlight*/) { }
    // Called to change fading state for all groupped metafiles.
    virtual void onGroupMetafilesFadingChanged(OdTrVisGroupId /*groupId*/, bool /*bFade*/) { }

    // Properties

    // Metafile-related properties.
    // Called to attach property to metafile.
    virtual void onMetafilePropertyAttached(OdTrVisMetafileId /*metafileId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from metafile.
    virtual void onMetafilePropertyDetached(OdTrVisMetafileId /*metafileId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // Group-related properties.
    // Called to attach property to group.
    virtual void onGroupPropertyAttached(OdTrVisGroupId /*groupId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from group.
    virtual void onGroupPropertyDetached(OdTrVisGroupId /*groupId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // Layer-related properties.
    // Called to attach property to layer.
    virtual void onLayerPropertyAttached(OdTrVisLayerId /*layerId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from layer.
    virtual void onLayerPropertyDetached(OdTrVisLayerId /*layerId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // VisualStyle-related properties.
    // Called to attach property to visual style.
    virtual void onVisualStylePropertyAttached(OdTrVisVisualStyleId /*visualStyleId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from visual style.
    virtual void onVisualStylePropertyDetached(OdTrVisVisualStyleId /*visualStyleId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // Material-related properties.
    // Called to attach property to material.
    virtual void onMaterialPropertyAttached(OdTrVisMaterialId /*materialId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from material.
    virtual void onMaterialPropertyDetached(OdTrVisMaterialId /*materialId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // Texture-related properties.
    // Called to attach property to texture.
    virtual void onTexturePropertyAttached(OdTrVisTextureId /*textureId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from texture.
    virtual void onTexturePropertyDetached(OdTrVisTextureId /*textureId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // Light-related properties.
    // Called to attach property to light source.
    virtual void onLightPropertyAttached(OdTrVisLightId /*lightId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from light source.
    virtual void onLightPropertyDetached(OdTrVisLightId /*lightId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // HlBranch-related properties.
    // Called to attach property to state branch.
    virtual void onStateBranchPropertyAttached(OdTrVisStateBranchId /*stateBranchId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from state branch.
    virtual void onStateBranchPropertyDetached(OdTrVisStateBranchId /*stateBranchId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // Viewport-related properties.
    // Called to attach property to viewport.
    virtual void onViewportPropertyAttached(OdTrVisViewportId /*viewportId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from viewport.
    virtual void onViewportPropertyDetached(OdTrVisViewportId /*viewportId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // Overlay-related properties.
    // Called to attach property to overlay.
    virtual void onOverlayPropertyAttached(OdTrVisOverlayId /*overlayId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from overlay.
    virtual void onOverlayPropertyDetached(OdTrVisOverlayId /*overlayId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
    // ExtensionObject-related properties.
    // Called to attach property to extension object.
    virtual void onExtensionObjectPropertyAttached(OdTrVisExtensionId /*extId*/, const OdTrVisPropertyDef & /*propDef*/) { }
    // Called to detach property from extension object.
    virtual void onExtensionObjectPropertyDetached(OdTrVisExtensionId /*extId*/, OdTrVisPropertyDef::PropertyType /*propType*/) { }
};

/** \details
    <group ExRender_Windows_Classes> 
*/
class OdTrVisRenditionRedir : public OdTrVisRendition
{
  protected:
    OdTrVisRendition *m_pRedir;
    OdTrVisRendition *m_pInformationalRedir;
  public:
    struct IdRegistratorRedir : public IdRegistrator
    {
      IdRegistrator *m_pIdRegRedir;
      IdRegistratorRedir() : m_pIdRegRedir(NULL) { }

      void setIdRegRedirection(IdRegistrator *pRedir) { m_pIdRegRedir = pRedir; }
      IdRegistrator *getIdRegRedirection() { return m_pIdRegRedir; }
      const IdRegistrator *getIdRegRedirection() const { return m_pIdRegRedir; }
      bool hasIdRegRedirection() const { return (m_pIdRegRedir != NULL); }

      virtual void reg(DataType type, OdTrVisId id, OdDbStub *pPersId)
      {
        if (hasIdRegRedirection())
          getIdRegRedirection()->reg(type, id, pPersId);
      }
      virtual void reg(DataType type, OdTrVisId id, const OdGiDrawable *pTransId)
      {
        if (hasIdRegRedirection())
          getIdRegRedirection()->reg(type, id, pTransId);
      }
      virtual void reg(DataType type, OdTrVisId id, const OdTrVisUniqueID &uId)
      {
        if (hasIdRegRedirection())
          getIdRegRedirection()->reg(type, id, uId);
      }
      virtual void kill(DataType type, OdTrVisId id)
      {
        if (hasIdRegRedirection())
          getIdRegRedirection()->kill(type, id);
      }
      virtual OdTrVisUniqueID *get(DataType type, OdTrVisId id, bool bKill = false) const
      {
        if (hasIdRegRedirection())
          return getIdRegRedirection()->get(type, id, bKill);
        return NULL;
      }
    };
  public:
    OdTrVisRenditionRedir() : m_pRedir(NULL), m_pInformationalRedir(NULL) { }

    void setRedirection(OdTrVisRendition *pRedir) { m_pRedir = pRedir; }
    OdTrVisRendition *getRedirection() { return m_pRedir; }
    const OdTrVisRendition *getRedirection() const { return m_pRedir; }
    bool hasRedirection() const { return (m_pRedir != NULL); }

    void setInformationalRedirection(OdTrVisRendition *pRedir) { m_pInformationalRedir = pRedir; }
    OdTrVisRendition *getInformationalRedirection()
    {
      if (!m_pInformationalRedir)
        throw OdError(eNullPtr);
      return m_pInformationalRedir;
    }
    const OdTrVisRendition *getInformationalRedirection() const
    {
      if (!m_pInformationalRedir)
        throw OdError(eNullPtr);
      return m_pInformationalRedir;
    }
    bool hasInformationalRedirection() const { return (m_pInformationalRedir != NULL); }

    void setRedirections(OdTrVisRendition *pRedir)
    {
      setRedirection(pRedir);
      setInformationalRedirection(pRedir);
    }
    bool hasRedirections() const { return hasRedirection() && hasInformationalRedirection(); }

    // High-level callbacks

    // Called on starting vectorizer update.
    virtual void onDeviceUpdateBegin(OdTrVisRenderClient *pDevice)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onDeviceUpdateBegin(pDevice);
    }
    // Called on ending vectorizer update.
    virtual void onDeviceUpdateEnd(OdTrVisRenderClient *pDevice, bool bRender = true)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onDeviceUpdateEnd(pDevice, bRender);
    }
    // Checks that vectorizer update started.
    virtual bool isWithinDeviceUpdate() const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->isWithinDeviceUpdate();
      return false;
    }

    // Called by underlying application to store frame changing information.
    virtual void onFrameSwitchInfo(OdUInt32 nInterval)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onFrameSwitchInfo(nInterval);
    }

    // Returns set of bit flags which indicate rendition capabilities/settings
    virtual OdUInt32 queryRenditionCaps(OdUInt32 requestFor = 0xFFFFFFFF) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->queryRenditionCaps(requestFor);
      return 0;
    }

    // Partial update support for local renditions:
    // Called to check does specified viewport/overlay combination support partial update query.
    virtual bool onPartialUpdateCheckLimit(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId)
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->onPartialUpdateCheckLimit(viewportId, overlayId);
      return false;
    }
    // Called to setup invalid rectangles and overlays on partial update.
    virtual void onPartialUpdateInvalidRects(OdTrVisOverlayId overlayId, const OdGsDCRectArray &invalidRects, bool bOverlayInvalid = false)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onPartialUpdateInvalidRects(overlayId, invalidRects, bOverlayInvalid);
    }
    // Called on partial update query to set entire actual visibilities list.
    virtual void onPartialUpdateList(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onPartialUpdateList(viewportId, overlayId, pList, nEntries);
    }
    // Can be called by client side to check that rendition supports direct rendering ability.
    virtual bool queryPartialUpdateDirectInvocation(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdTrVisDirectRenderPartialUpdateInvocationPtr &pInvocationPtr)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->queryPartialUpdateDirectInvocation(viewportId, overlayId, pInvocationPtr);
      return false;
    }
    // Can be called by client side to setup direct invocation interface for partial update instead of partial update list usage.
    virtual void onPartialUpdateDirectInvocation(const OdTrVisDirectRenderPartialUpdateInvocation *pInvocation, OdTrVisRenderDataDirectAccessor *pAccessor)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->onPartialUpdateDirectInvocation(pInvocation, pAccessor);
    }

    // Query snapshot
    virtual void querySnapshot(OdUInt8 *pScanlines, long minX, long minY, long nWidth, long nHeight)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->querySnapshot(pScanlines, minX, minY, nWidth, nHeight);
    }

    // Renders viewport to selection texture. Return top object id with (x, y) position in viewport.
    virtual void getSelectedObject(OdTrVisViewportId id, OdUInt32 x, OdUInt32 y, OdArray<OdTrVisId> &outIDs, OdTrVisGsMarker *outMarker = NULL)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->getSelectedObject(id, x, y, outIDs, outMarker);
    }
    // Validates selection texture.
    virtual bool isSelectTextureValid(OdTrVisViewportId id) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->isSelectTextureValid(id);
      return false;
    }

    // Allocate new resource sharing provider compatible with rendition
    virtual OdTrVisSharingProviderClientPtr createSharingProvider()
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->createSharingProvider();
      return OdTrVisSharingProviderClientPtr();
    }

    // Multithread processing flags
    virtual MtHandlerType mtDataTypeProcessing(DataType &type) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->mtDataTypeProcessing(type);
      return kMtSkip;
    }

    // Returns Id registrator, or NULL if no registration supported
    virtual IdRegistrator *idRegistrator() const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->idRegistrator();
      return NULL;
    }

    // Returns true if metafile streams accessible directly, returns stream by Id
    virtual bool queryMetafileStream(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId,
                                     OdTrVisDisplayId displayId, const OdTrVisMetafileContainer **pStream)
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->queryMetafileStream(viewportId, overlayId, displayId, pStream);
      return false;
    }
    // Unlock metafile stream, queried by queryMetafileStream method
    virtual bool unlockMetafileStream(const OdTrVisMetafileContainer *pStream)
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->unlockMetafileStream(pStream);
      return false;
    }

    // Returns non-kTrVisNegativeId in case if vectorizer supports ProgramID's embedded into metafile stream
    virtual OdTrVisProgramId queryProgramId(OdGsView::RenderMode mode, OdTrVisVisualStyleId vsId = kTrVisNegativeId,
                                            OdTrVisGeomType geomType = OdTrVisGeomType_Default, OdUInt16 attrFlags = 0, OdUInt16 shadingFlags = 0) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->queryProgramId(mode, vsId, geomType, attrFlags, shadingFlags);
      return kTrVisNegativeId;
    }

    // GsFiler support
    virtual bool saveRenditionState(OdGsFiler *pFiler) const
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->saveRenditionState(pFiler);
      return false;
    }
    virtual bool loadRenditionState(OdGsFiler *pFiler, OdTrVisIdMap *pIdMap)
    {
      if (hasInformationalRedirection())
        return getInformationalRedirection()->loadRenditionState(pFiler, pIdMap);
      return false;
    }

    // Mid-level callbacks

    // Client information

    // Returns rendering client settings.
    virtual void obtainClientSettings(ClientSettings &clientSets)
    {
      if (hasInformationalRedirection())
        getInformationalRedirection()->obtainClientSettings(clientSets);
    }

    // Surface

    // Called when output render surface parameters changed.
    virtual void onSurfaceChanged(const OdTrVisSurfaceDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onSurfaceChanged(pDef);
    }
    // Called when rendering surface needs to be invalidated forcibly.
    virtual void onSurfaceInvalidated()
    {
      if (hasRedirection())
        getRedirection()->onSurfaceInvalidated();
    }

    // Overlays

    // Called when new overlay added into device.
    virtual void onOverlayAdded(OdTrVisOverlayId overlayId, const OdTrVisOverlayDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onOverlayAdded(overlayId, pDef);
    }
    // Called when exist overlay removed from device.
    virtual void onOverlayDeleted(OdTrVisOverlayId overlayId)
    {
      if (hasRedirection())
        getRedirection()->onOverlayDeleted(overlayId);
    }
    // Called when overlay visibility changed.
    virtual void onOverlayVisibilityChanged(OdTrVisOverlayId overlayId, bool bVisibility)
    {
      if (hasRedirection())
        getRedirection()->onOverlayVisibilityChanged(overlayId, bVisibility);
    }
    // Called when overlay rendering order changed.
    virtual void onOverlayRenderOrderChanged(OdTrVisOverlayId overlayId, OdInt16 nRenderOrder)
    {
      if (hasRedirection())
        getRedirection()->onOverlayRenderOrderChanged(overlayId, nRenderOrder);
    }

    // Viewports

    // Called when new viewport added into device.
    virtual void onViewportAdded(OdTrVisViewportId viewportId, const OdTrVisViewportDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onViewportAdded(viewportId, pDef);
    }
    // Called when viewport inserted in list with some index.
    virtual void onViewportInserted(OdTrVisViewportId viewportId, int nIndex, const OdTrVisViewportDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onViewportInserted(viewportId, nIndex, pDef);
    }
    // Called when viewport removed from device.
    virtual void onViewportDeleted(OdTrVisViewportId viewportId)
    {
      if (hasRedirection())
        getRedirection()->onViewportDeleted(viewportId);
    }
    // Called when viewport visibility changed.
    virtual void onViewportVisibilityChanged(OdTrVisViewportId viewportId, bool bVisibility)
    {
      if (hasRedirection())
        getRedirection()->onViewportVisibilityChanged(viewportId, bVisibility);
    }

    // Primary viewports configuration

    // Called when viewport view parameters is modified.
    virtual void onViewportViewParamsModified(OdTrVisViewportId viewportId, const OdTrVisViewParamsDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onViewportViewParamsModified(viewportId, pDef);
    }
    // Called when viewport linestyle settings is modified.
    virtual void onViewportLineStylesModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineStyleDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onViewportLineStylesModified(viewportId, pDef);
    }
    // Called when viewport fading settings is modified.
    virtual void onViewportFadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportFadingDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onViewportFadingModified(viewportId, pDef);
    }
    // Called when viewport lineweight settings is modified.
    virtual void onViewportLineweightsModified(OdTrVisViewportId viewportId, const OdTrVisViewportLineweightsDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onViewportLineweightsModified(viewportId, pDef);
    }
    // Called when viewport rendering mode is modified.
    virtual void onViewportShadingModified(OdTrVisViewportId viewportId, const OdTrVisViewportShadingDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onViewportShadingModified(viewportId, pDef);
    }
    // Called when viewport clipping region is modified.
    virtual void onViewportClipModified(OdTrVisViewportId viewportId, const OdTrVisViewportClipDef &pDef,
                                        bool bNrcClipModified = true, bool bBorderModified = true)
    {
      if (hasRedirection())
        getRedirection()->onViewportClipModified(viewportId, pDef, bNrcClipModified, bBorderModified);
    }

    // Extended viewports configuration

    // Called when background stream in viewport is changed.
    virtual void onViewportBackgroundChanged(OdTrVisViewportId viewportId, OdTrVisFlatMetafileContainerPtr pStream,
                                             OdTrVisProgramId baseProgramId = kTrVisNegativeId)
    {
      if (hasRedirection())
        getRedirection()->onViewportBackgroundChanged(viewportId, pStream, baseProgramId);
    }
    // Called when viewport fog changed.
    virtual void onViewportFogChanged(OdTrVisViewportId viewportId, const OdTrVisFogDef &pDef = OdTrVisFogDef())
    {
      if (hasRedirection())
        getRedirection()->onViewportFogChanged(viewportId, pDef);
    }
    // Called when viewport sectioning modified.
    virtual void onViewportSectioningChanged(OdTrVisViewportId viewportId, const OdTrVisSectionDef &pDef = OdTrVisSectionDef())
    {
      if (hasRedirection())
        getRedirection()->onViewportSectioningChanged(viewportId, pDef);
    }
    // Called when viewport anti-aliasing setting changed.
    virtual void onViewportAntiAliasingChanged(OdTrVisViewportId viewportId, OdUInt32 nAntiAliasing)
    {
      if (hasRedirection())
        getRedirection()->onViewportAntiAliasingChanged(viewportId, nAntiAliasing);
    }

    // Viewport default lights configuration

    // Called when viewport ambient light setting modified.
    virtual void onViewportAmbientLightChanged(OdTrVisViewportId viewportId, const OdTrVisMaterialColor &ambientLight)
    {
      if (hasRedirection())
        getRedirection()->onViewportAmbientLightChanged(viewportId, ambientLight);
    }
    // Called when viewport default light setting modified.
    virtual void onViewportDefaultLightsChanged(OdTrVisViewportId viewportId, bool bDefaultLightsOn,
                                                const OdTrVisLightDef *pLightsList = NULL, OdUInt32 nLightsListEntries = 0)
    {
      if (hasRedirection())
        getRedirection()->onViewportDefaultLightsChanged(viewportId, bDefaultLightsOn, pLightsList, nLightsListEntries);
    }

    // Viewport lights management

    // Called when light source added into viewport light sources list.
    virtual void onViewportLightAttached(OdTrVisViewportId viewportId, OdTrVisLightId lightId,
                                         OdUInt32 nOverrides = 0, const OdTrVisLightDef &pOverrides = OdTrVisLightDef())
    {
      if (hasRedirection())
        getRedirection()->onViewportLightAttached(viewportId, lightId, nOverrides, pOverrides);
    }
    // Called when light source removed from viewport light sources list.
    virtual void onViewportLightDetached(OdTrVisViewportId viewportId, OdTrVisLightId lightId)
    {
      if (hasRedirection())
        getRedirection()->onViewportLightDetached(viewportId, lightId);
    }

    // Per-overlay viewport overrides

    // Called when viewport needs override view parameters for specific overlay.
    virtual void onOverlayViewParamsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, bool bOverride,
                                             const OdTrVisViewParamsDef &pDef = OdTrVisViewParamsDef())
    {
      if (hasRedirection())
        getRedirection()->onOverlayViewParamsOverride(viewportId, overlayId, bOverride, pDef);
    }
    // Called when viewport extents changed.
    virtual void onOverlayExtentsOverride(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisExtentsDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onOverlayExtentsOverride(viewportId, overlayId, pDef);
    }

    // Viewports ordering lists

    // Called when order of metafiles in viewport is changed.
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdTrVisDisplayId *pList, OdUInt32 nEntries)
    {
      if (hasRedirection())
        getRedirection()->onMetafileOrderChanged(viewportId, overlayId, pList, nEntries);
    }
    // Called when order of metafiles in viewport is changed (for obtain modified part of list by renderer).
    virtual void onMetafileOrderChanged(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, OdUInt32 nInvalidFrom, OdUInt32 nInvalidLen,
                                        const OdTrVisDisplayId *pValidFrom, OdUInt32 nValidLen)
    {
      if (hasRedirection())
        getRedirection()->onMetafileOrderChanged(viewportId, overlayId, nInvalidFrom, nInvalidLen, pValidFrom, nValidLen);
    }
    // Called when order of metafiles can be inherit from other source.
    virtual void onMetafileOrderInheritance( OdTrVisViewportId viewportId1, OdTrVisOverlayId overlayId1, OdTrVisViewportId viewportId2 )
    {
      if( hasRedirection() )
        getRedirection()->onMetafileOrderInheritance( viewportId1, overlayId1, viewportId2 );
    }

    // Metafiles

    // Called when new metafile added (returns visualId, equal to metafileId by default).
    virtual OdTrVisDisplayId onMetafileAdded(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef)
    {
      if (hasRedirection())
        return getRedirection()->onMetafileAdded(metafileId, pDef);
      return metafileId;
    }
    // Called when exist metafile deleted.
    virtual void onMetafileDeleted(OdTrVisMetafileId metafileId)
    {
      if (hasRedirection())
        getRedirection()->onMetafileDeleted(metafileId);
    }
    // Called when exist metafile changed.
    //virtual void onMetafileModified(OdTrVisMetafileId metafileId, const OdTrVisMetafileDef &pDef)
    //{
    //  if (hasRedirection())
    //    getRedirection()->onMetafileModified(metafileId, pDef);
    //}
    // Called when exist metafile visibility changed.
    virtual void onMetafileVisibilityChanged(OdTrVisMetafileId metafileId, bool bVisibility)
    {
      if (hasRedirection())
        getRedirection()->onMetafileVisibilityChanged(metafileId, bVisibility);
    }
    // Called when exist metafile highlighting changed.
    virtual void onMetafileHighlightingChanged(OdTrVisMetafileId metafileId, bool bHighlight)
    {
      if (hasRedirection())
        getRedirection()->onMetafileHighlightingChanged(metafileId, bHighlight);
    }
    // Called when exist metafile fading changed.
    virtual void onMetafileFadingChanged(OdTrVisMetafileId metafileId, bool bFade)
    {
      if (hasRedirection())
        getRedirection()->onMetafileFadingChanged(metafileId, bFade);
    }

    // Lights

    // Called when new light source added.
    virtual void onLightAdded(OdTrVisLightId lightId, const OdTrVisLightDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onLightAdded(lightId, pDef);
    }
    // Called when exist light source modified.
    virtual void onLightModified(OdTrVisLightId lightId, OdUInt32 nKindOfMod, const OdTrVisLightDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onLightModified(lightId, nKindOfMod, pDef);
    }
    // Called when exist light source deleted.
    virtual void onLightDeleted(OdTrVisLightId lightId)
    {
      if (hasRedirection())
        getRedirection()->onLightDeleted(lightId);
    }

    // State branches (highlighting, hide)

    // Called when new state branch added.
    virtual void onStateBranchAdded(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onStateBranchAdded(stateBranchId, pDef);
    }
    // Called when exist state branch modified.
    virtual void onStateBranchModified(OdTrVisStateBranchId stateBranchId, const OdTrVisStateBranchDef &pDef,
                                       bool bChildsModified, bool bMarkersModified)
    {
      if (hasRedirection())
        getRedirection()->onStateBranchModified(stateBranchId, pDef, bChildsModified, bMarkersModified);
    }
    // Called when exist state branch deleted.
    virtual void onStateBranchDeleted(OdTrVisStateBranchId stateBranchId)
    {
      if (hasRedirection())
        getRedirection()->onStateBranchDeleted(stateBranchId);
    }

    // Textures

    // Called when new texture added.
    virtual void onTextureAdded(OdTrVisTextureId textureId, const OdTrVisTextureDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onTextureAdded(textureId, pDef);
    }
    // Called when exist texture deleted.
    virtual void onTextureDeleted(OdTrVisTextureId textureId)
    {
      if (hasRedirection())
        getRedirection()->onTextureDeleted(textureId);
    }

    // Materials

    // Called when new material added.
    virtual void onMaterialAdded(OdTrVisMaterialId materialId, const OdTrVisMaterialDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onMaterialAdded(materialId, pDef);
    }
    // Called when exist material deleted.
    virtual void onMaterialDeleted(OdTrVisMaterialId materialId)
    {
      if (hasRedirection())
        getRedirection()->onMaterialDeleted(materialId);
    }

    // Visual Styles

    // Called when new visual style added.
    virtual void onVisualStyleAdded(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
    {
      if (hasRedirection())
        getRedirection()->onVisualStyleAdded(visualStyleId, pDef);
    }
    // Called when exist visual style modified.
    virtual void onVisualStyleModified(OdTrVisVisualStyleId visualStyleId, const OdTrVisVisualStyle &pDef)
    {
      if (hasRedirection())
        getRedirection()->onVisualStyleModified(visualStyleId, pDef);
    }
    // Called when exist visual style deleted.
    virtual void onVisualStyleDeleted(OdTrVisVisualStyleId visualStyleId)
    {
      if (hasRedirection())
        getRedirection()->onVisualStyleDeleted(visualStyleId);
    }

    // Layers

    // Called when new layer added.
    virtual void onLayerAdded(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
    {
      if (hasRedirection())
        getRedirection()->onLayerAdded(layerId, pDef);
    }
    // Called when exist layer modified.
    //virtual void onLayerModified(OdTrVisLayerId layerId, const OdTrVisLayerDef &pDef)
    //{
    //  if (hasRedirection())
    //    getRedirection()->onLayerModified(layerId, pDef);
    //}
    // Called when exist layer deleted.
    virtual void onLayerDeleted(OdTrVisLayerId layerId)
    {
      if (hasRedirection())
        getRedirection()->onLayerDeleted(layerId);
    }
    // Called when exist layer visibility changed.
    virtual void onLayerVisibilityChanged(OdTrVisLayerId layerId, bool bVisibility)
    {
      if (hasRedirection())
        getRedirection()->onLayerVisibilityChanged(layerId, bVisibility);
    }
    // Called when exist layer fading changed.
    virtual void onLayerFadingChanged(OdTrVisLayerId layerId, bool bFade)
    {
      if (hasRedirection())
        getRedirection()->onLayerFadingChanged(layerId, bFade);
    }
    // Called when layer viewport-dependent properties changed.
    virtual void onLayerVpDepPropsOverride(OdTrVisLayerId layerId, OdTrVisViewportId viewportId, bool bOverride,
                                           const OdTrVisLayerProps &pProps = OdTrVisLayerProps())
    {
      if (hasRedirection())
        getRedirection()->onLayerVpDepPropsOverride(layerId, viewportId, bOverride, pProps);
    }

    // Extension objects

    // Called when new extension object added.
    virtual void onExtensionObjectAdded(OdTrVisExtensionId extId, const OdTrVisExtensionObject *pObj)
    {
      if (hasRedirection())
        getRedirection()->onExtensionObjectAdded(extId, pObj);
    }
    // Called when exist extension object deleted.
    virtual void onExtensionObjectDeleted(OdTrVisExtensionId extId)
    {
      if (hasRedirection())
        getRedirection()->onExtensionObjectDeleted(extId);
    }
    // Called to send special extension object specific data.
    virtual void onExtensionObjectSpecificData(OdTrVisExtensionId extId, OdUInt32 nDataId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onExtensionObjectSpecificData(extId, nDataId, propDef);
    }
    // Called to change extension object visibility.
    virtual void onExtensionObjectVisibilityChanged(OdTrVisExtensionId extId, bool bVisibility)
    {
      if (hasRedirection())
        getRedirection()->onExtensionObjectVisibilityChanged(extId, bVisibility);
    }

    // Groups

    // Called when new group added.
    virtual void onGroupAdded(OdTrVisGroupId groupId)
    {
      if (hasRedirection())
        getRedirection()->onGroupAdded(groupId);
    }
    // Called when exist group deleted.
    virtual void onGroupDeleted(OdTrVisGroupId groupId)
    {
      if (hasRedirection())
        getRedirection()->onGroupDeleted(groupId);
    }
    // Called to attach metafiles to group.
    virtual void onGroupMetafilesAttached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles)
    {
      if (hasRedirection())
        getRedirection()->onGroupMetafilesAttached(groupId, pMetafilesList, nMetafiles);
    }
    // Called to detach metafiles from group (if no metafiles specified - clear entire list of groupped metafiles).
    virtual void onGroupMetafilesDetached(OdTrVisGroupId groupId, const OdTrVisMetafileId *pMetafilesList, OdUInt32 nMetafiles)
    {
      if (hasRedirection())
        getRedirection()->onGroupMetafilesDetached(groupId, pMetafilesList, nMetafiles);
    }
    // Called to delete all groupped metafiles from graphics scene.
    //virtual void onGroupMetafilesDeleted(OdTrVisGroupId groupId)
    //{
    //  if (hasRedirection())
    //    getRedirection()->onGroupMetafilesDeleted(groupId);
    //}
    // Called to change visibility state for all groupped metafiles.
    virtual void onGroupMetafilesVisibilityChanged(OdTrVisGroupId groupId, bool bVisibility)
    {
      if (hasRedirection())
        getRedirection()->onGroupMetafilesVisibilityChanged(groupId, bVisibility);
    }
    // Called to change highlighting state for all groupped metafiles.
    virtual void onGroupMetafilesHighlightingChanged(OdTrVisGroupId groupId, bool bHighlight)
    {
      if (hasRedirection())
        getRedirection()->onGroupMetafilesHighlightingChanged(groupId, bHighlight);
    }
    // Called to change fading state for all groupped metafiles.
    virtual void onGroupMetafilesFadingChanged(OdTrVisGroupId groupId, bool bFade)
    {
      if (hasRedirection())
        getRedirection()->onGroupMetafilesFadingChanged(groupId, bFade);
    }

    // Properties

    // Metafile-related properties.
    // Called to attach property to metafile.
    virtual void onMetafilePropertyAttached(OdTrVisMetafileId metafileId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onMetafilePropertyAttached(metafileId, propDef);
    }
    // Called to detach property from metafile.
    virtual void onMetafilePropertyDetached(OdTrVisMetafileId metafileId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onMetafilePropertyDetached(metafileId, propType);
    }
    // Group-related properties.
    // Called to attach property to group.
    virtual void onGroupPropertyAttached(OdTrVisGroupId groupId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onGroupPropertyAttached(groupId, propDef);
    }
    // Called to detach property from group.
    virtual void onGroupPropertyDetached(OdTrVisGroupId groupId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onGroupPropertyDetached(groupId, propType);
    }
    // Layer-related properties.
    // Called to attach property to layer.
    virtual void onLayerPropertyAttached(OdTrVisLayerId layerId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onLayerPropertyAttached(layerId, propDef);
    }
    // Called to detach property from layer.
    virtual void onLayerPropertyDetached(OdTrVisLayerId layerId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onLayerPropertyDetached(layerId, propType);
    }
    // VisualStyle-related properties.
    // Called to attach property to visual style.
    virtual void onVisualStylePropertyAttached(OdTrVisVisualStyleId visualStyleId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onVisualStylePropertyAttached(visualStyleId, propDef);
    }
    // Called to detach property from visual style.
    virtual void onVisualStylePropertyDetached(OdTrVisVisualStyleId visualStyleId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onVisualStylePropertyDetached(visualStyleId, propType);
    }
    // Material-related properties.
    // Called to attach property to material.
    virtual void onMaterialPropertyAttached(OdTrVisMaterialId materialId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onMaterialPropertyAttached(materialId, propDef);
    }
    // Called to detach property from material.
    virtual void onMaterialPropertyDetached(OdTrVisMaterialId materialId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onMaterialPropertyDetached(materialId, propType);
    }
    // Texture-related properties.
    // Called to attach property to texture.
    virtual void onTexturePropertyAttached(OdTrVisTextureId textureId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onTexturePropertyAttached(textureId, propDef);
    }
    // Called to detach property from texture.
    virtual void onTexturePropertyDetached(OdTrVisTextureId textureId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onTexturePropertyDetached(textureId, propType);
    }
    // Light-related properties.
    // Called to attach property to light source.
    virtual void onLightPropertyAttached(OdTrVisLightId lightId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onLightPropertyAttached(lightId, propDef);
    }
    // Called to detach property from light source.
    virtual void onLightPropertyDetached(OdTrVisLightId lightId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onLightPropertyDetached(lightId, propType);
    }
    // StateBranch-related properties.
    // Called to attach property to state branch.
    virtual void onStateBranchPropertyAttached(OdTrVisStateBranchId stateBranchId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onStateBranchPropertyAttached(stateBranchId, propDef);
    }
    // Called to detach property from state branch.
    virtual void onStateBranchPropertyDetached(OdTrVisStateBranchId stateBranchId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onStateBranchPropertyDetached(stateBranchId, propType);
    }
    // Viewport-related properties.
    // Called to attach property to viewport.
    virtual void onViewportPropertyAttached(OdTrVisViewportId viewportId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onViewportPropertyAttached(viewportId, propDef);
    }
    // Called to detach property from viewport.
    virtual void onViewportPropertyDetached(OdTrVisViewportId viewportId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onViewportPropertyDetached(viewportId, propType);
    }
    // Overlay-related properties.
    // Called to attach property to overlay.
    virtual void onOverlayPropertyAttached(OdTrVisOverlayId overlayId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onOverlayPropertyAttached(overlayId, propDef);
    }
    // Called to detach property from overlay.
    virtual void onOverlayPropertyDetached(OdTrVisOverlayId overlayId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onOverlayPropertyDetached(overlayId, propType);
    }
    // ExtensionObject-related properties.
    // Called to attach property to extension object.
    virtual void onExtensionObjectPropertyAttached(OdTrVisExtensionId extId, const OdTrVisPropertyDef &propDef)
    {
      if (hasRedirection())
        getRedirection()->onExtensionObjectPropertyAttached(extId, propDef);
    }
    // Called to detach property from extension object.
    virtual void onExtensionObjectPropertyDetached(OdTrVisExtensionId extId, OdTrVisPropertyDef::PropertyType propType)
    {
      if (hasRedirection())
        getRedirection()->onExtensionObjectPropertyDetached(extId, propType);
    }
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrVisRenditionDeviceUpdateScopes
{
  protected:
    OdTrVisRendition *m_pRendition;
    OdTrVisRenderClient *m_pRenderClient;
    bool m_bRender;
  public:
    OdTrVisRenditionDeviceUpdateScopes(OdTrVisRendition *pRendition, OdTrVisRenderClient *pDevice,
                                       bool bReentrant = true, bool bRender = true)
      : m_pRendition(pRendition), m_pRenderClient(pDevice), m_bRender(bRender)
    { if (pRendition)
      { if (pRendition->isWithinDeviceUpdate())
        { if (!bReentrant)
            throw OdError(eAlreadyActive);
          m_pRendition = NULL;
        } else
          pRendition->onDeviceUpdateBegin(pDevice);
      }
    }
    bool isReentered() const { return m_pRendition == NULL; }
    ~OdTrVisRenditionDeviceUpdateScopes()
    { if (m_pRendition)
        m_pRendition->onDeviceUpdateEnd(m_pRenderClient, m_bRender);
    }
};

#include "TD_PackPop.h"

#endif // ODTRVISRENDITION
