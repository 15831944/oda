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
// Rendering device local interface

#ifndef ODTRRNDLOCALRENDERER
#define ODTRRNDLOCALRENDERER

#include "TD_PackPush.h"

#include "TrRndLocalRenditionHost.h"

// Forward declarations
class OdTrRndLocalViewer;
class OdTrRndLocalTransientManager;
class OdTrVisCamera;

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndLocalRenderer
{
  public:
    // Selection
    enum SelectionType
    {
      kSelCrossing,
      kSelWindow
    };
    enum SelectionFeedback
    {
      kSelContinue,
      kSelBreak
    };
    enum SelectionFlags
    {
      // Flags related to select() method only
      kSelOnce       = 0x0001, // Select single entity only
      kSelTop        = 0x0002, // Select top entity only
      kSelLast       = 0x0004, // Select last entity only
      kSelSubEnts    = 0x0008, // Enable subentities selection
      kSelInserts    = 0x0010, // Avoid selection of nested metafiles
      kSelExplGroups = 0x0020, // Explode metafile groups
      // Flags related to both select() and selectAll() methods
      kSelEnableHlt  = 0x0040, // Highlight metafiles (flag is only to pass into client code)
      kSelInvertHlt  = 0x0080, // Invert metafiles highlighting (flag is only to pass into client code)
      kSelSkipHlt    = 0x0100, // Enable skipping of highlighted/unhighlighted metafiles during selection
      kSelSkipHltSet = 0x0200  // if kSelSkipHlt enabled this flag represents that highlighted or unhighlighted metafiles should be skipped
    };
    struct SelectionInfo
    {
      SelectionType m_type;
      OdUInt32 m_flags;
      SelectionInfo() : m_type(kSelCrossing), m_flags(0) {}
      SelectionInfo(SelectionType type, OdUInt32 flags = 0) : m_type(type), m_flags(flags) {}

#define ODTRRND_SI_DEF_FLAG(flagName, getterName, setterName) \
      bool getterName() const { return GETBIT(m_flags, flagName); } \
      void setterName(bool bSet) { SETBIT(m_flags, flagName, bSet); }
      ODTRRND_SI_DEF_FLAG(kSelOnce      , isSelectOnce                        , setSelectOnce);
      ODTRRND_SI_DEF_FLAG(kSelTop       , isSelectTop                         , setSelectTop);
      ODTRRND_SI_DEF_FLAG(kSelLast      , isSelectLast                        , setSelectLast);
      ODTRRND_SI_DEF_FLAG(kSelSubEnts   , isSelectSubentities                 , setSelectSubentities);
      ODTRRND_SI_DEF_FLAG(kSelInserts   , isSelectInsertsOnly                 , setSelectInsertsOnly);
      ODTRRND_SI_DEF_FLAG(kSelExplGroups, isSelectGroupsExploded              , setSelectGroupsExploded);
      ODTRRND_SI_DEF_FLAG(kSelEnableHlt , isSelectHighlightingEnabled         , setSelectHighlightingEnabled);
      ODTRRND_SI_DEF_FLAG(kSelInvertHlt , isSelectHighlightingInversionEnabled, setSelectHighlightingInversionEnabled);
      ODTRRND_SI_DEF_FLAG(kSelSkipHlt   , isSelectSkipHighlighted             , setSelectSkipHighlighted);
      ODTRRND_SI_DEF_FLAG(kSelSkipHltSet, selectSkipHighlightedSetting        , setSelectSkipHighlightedSetting);
#undef ODTRRND_SI_DEF_FLAG
    };
    struct SelectionResult
    {
      OdTrVisMetafileId m_mfId;
      OdTrVisGroupId    m_grId;
      SelectionResult(OdTrVisMetafileId mfId = kTrVisNullId, OdTrVisGroupId grId = kTrVisNullId)
        : m_mfId(mfId), m_grId(grId) {}
      bool hasGroup() const { return (m_grId != kTrVisNullId); }
    };
    struct SelectionReactor
    {
      virtual OdTrRndLocalRenderer::SelectionFeedback selected(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const SelectionResult *pRes, OdUInt32 nInserts, OdTrVisGsMarker selMarker,
                                                               const OdTrRndLocalRenderer::SelectionInfo &selectionInfo, const OdTrRndLocalRenderer &renderer) const = 0;
    };
    // Viewport borders setup
    enum ViewportBorderType
    {
      kVpBorderMS = 0,   // ModelSpace viewport
      kVpBorderMSActive, // Active ModelSpace viewport
      kVpBorderPS,       // ModelSpace viewport inside PaperSpace
      kVpBorderPSActive, // Active ModelSpace viewport inside PaperSpace
      kNumVpBorderTypes  // Number of types (should be last enumeration member)
    };
    enum ViewportBorderState
    {
      kVpBorderDefault = 0, // Use default viewport setting
      kVpBorderEnabled,     // Enable viewport border override
      kVpBorderDisabled     // Disable viewport border override
    };
    struct ViewportBorderProps
    {
      ViewportBorderState m_bEnable; // Enable viewport border
      ODCOLORREF          m_color;   // Viewport border color
      int                 m_nWidth;  // Viewport border line width
      ViewportBorderProps(ViewportBorderState bEnable = kVpBorderDefault, ODCOLORREF color = ODRGBA(255, 255, 255, 255), int nWidth = 1)
        : m_bEnable(bEnable)
        , m_color(color)
        , m_nWidth(nWidth)
      { }
    };
  public:
    virtual OdTrVisRendition *rendition() = 0;
    virtual const OdTrVisRendition *rendition() const = 0;

    virtual OdTrRndLocalViewer *viewer() = 0;
    virtual const OdTrRndLocalViewer *viewer() const = 0;

    virtual bool detachWindowContext() = 0;
    virtual void attachWindowContext(OdTrVisRenderClient *pRenderClient) = 0;

    // Data accessors

    virtual OdTrVisViewportData *getViewportData(OdTrVisViewportId vpId) const = 0;
    virtual OdTrVisCamera *getCamera(OdTrVisViewportId vpId, OdTrVisOverlayId orId) const = 0;
    virtual OdTrVisMetafileContainer *getMetafileData(OdTrVisMetafileId mfId) const = 0;
    virtual OdTrVisOwning getMetafileOwner(OdTrVisMetafileId mfId) const = 0;

    virtual const OdGeExtents3d &getOverlayExtents(OdTrVisViewportId vpId, OdTrVisOverlayId orId) const = 0;

    virtual OdTrVisStateBranchDef *getStateBranchDef(OdTrVisStateBranchId branchId) const = 0;
    virtual OdTrVisOwning getStateBranchOwner(OdTrVisStateBranchId branchId) const = 0;
    virtual OdTrVisStateBranchId getNestedStateBranch(OdTrVisStateBranchId branchId, OdTrVisOwning ownerId) const = 0;
    virtual OdTrVisStateBranchId getMetafileHlBranch(OdTrVisMetafileId mfId) const = 0;
    virtual OdTrVisStateBranchId getMetafileVsBranch(OdTrVisMetafileId mfId) const = 0;

    virtual OdTrVisOwning getGroupOwner(OdTrVisGroupId groupId) const = 0;
    virtual OdTrVisStateBranchId getGroupHlBranch(OdTrVisGroupId groupId) const = 0;
    virtual OdTrVisStateBranchId getGroupVsBranch(OdTrVisGroupId groupId) const = 0;

    //virtual long screenWidth() const = 0;
    //virtual long screenHeight() const = 0;

    enum CoordinatesSystem
    {
      kScreenSpace,
      kEyeSpace,
      kWorldSpace
    };
    virtual OdGeMatrix3d coordinateSpaceTransform(OdTrVisViewportId vpId, OdTrVisOverlayId orId, CoordinatesSystem inSpace, CoordinatesSystem toSpace, bool bLocalScreenSpace = false) const = 0;

    // Viewport borders

    virtual const ViewportBorderProps &getViewportBorderProps(ViewportBorderType nType) const = 0;
    virtual void setViewportBorderProps(ViewportBorderType nType, const ViewportBorderProps& props) = 0;
    virtual bool isActiveViewport(OdTrVisViewportId vpId) const = 0;

    // Draw order modifiers

    virtual OdUInt32 getViewportMetafileOrderLength(OdTrVisViewportId vpId, OdTrVisOverlayId orId) const = 0;
    virtual OdTrVisMetafileId getViewportMetafileOrderElement(OdTrVisViewportId vpId, OdTrVisOverlayId orId, OdUInt32 nElement) const = 0;
    virtual OdUInt32 findViewportMetafileOrderElement(OdTrVisViewportId vpId, OdTrVisOverlayId orId, OdTrVisMetafileId mfId) const = 0;

    virtual void appendViewportMetafileOrderElement(OdTrVisViewportId vpId, OdTrVisOverlayId overlayId, OdTrVisMetafileId mfId) = 0;
    virtual void prependViewportMetafileOrderElement(OdTrVisViewportId vpId, OdTrVisOverlayId overlayId, OdTrVisMetafileId mfId) = 0;
    virtual void modifyViewportMetafileOrderElement(OdTrVisViewportId vpId, OdTrVisOverlayId overlayId, OdTrVisMetafileId mfId) = 0;
    virtual void deleteViewportMetafileOrderElement(OdTrVisViewportId vpId, OdTrVisOverlayId overlayId, OdTrVisMetafileId mfId) = 0;

    // Selection

    virtual OdUInt32 select(OdTrVisViewportId viewportId, OdTrVisOverlayId overlayId, const OdGsDCRect &selRect,
                            const SelectionInfo &selInfo = SelectionInfo(), SelectionReactor *pSelReactor = NULL) const = 0;

    virtual OdUInt32 selectAll(OdTrVisViewportId vpId, OdTrVisOverlayId overlayId, SelectionReactor *pSelReactor = NULL, OdUInt32 nFlags = 0) = 0;

    // Highlighting

    virtual void highlightMetafile(OdTrVisMetafileId mfId, const SelectionInfo &hltInfo) = 0;
    virtual void highlightMetafilesGroup(OdTrVisGroupId grId, const SelectionInfo &hltInfo) = 0;

    // Transient drawables manager
    virtual void setTransientManager(OdTrRndLocalTransientManager *pTransientManager) const = 0;
    virtual OdTrRndLocalTransientManager *transientManager(bool createIfMissing = true) const = 0;
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndLocalRendererTPtrAdaptor
{
  public:
    static void addRef(OdTrRndLocalRenderer* pObj)  { if (pObj) pObj->rendition()->addRef(); }
    static void release(OdTrRndLocalRenderer* pObj) { if (pObj) pObj->rendition()->release(); }
};

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndLocalRendererPtr : public TPtr<OdTrRndLocalRenderer, OdTrRndLocalRendererTPtrAdaptor>
{
  public:
    OdTrRndLocalRendererPtr()
      : TPtr<OdTrRndLocalRenderer, OdTrRndLocalRendererTPtrAdaptor>() {}
    OdTrRndLocalRendererPtr(const OdTrRndLocalRendererPtr& ref)
      : TPtr<OdTrRndLocalRenderer, OdTrRndLocalRendererTPtrAdaptor>(ref) {}
    OdTrRndLocalRendererPtr(const OdTrRndLocalRenderer* pObj, bool bAttach = false)
      : TPtr<OdTrRndLocalRenderer, OdTrRndLocalRendererTPtrAdaptor>(pObj, bAttach) {}
    OdTrRndLocalRendererPtr &operator =(OdTrRndLocalRenderer *pObj)
    { return static_cast<OdTrRndLocalRendererPtr&>(TPtr<OdTrRndLocalRenderer, OdTrRndLocalRendererTPtrAdaptor>::operator =(pObj)); }
    OdTrRndLocalRendererPtr &operator =(const OdTrRndLocalRendererPtr& ref)
    { return static_cast<OdTrRndLocalRendererPtr&>(TPtr<OdTrRndLocalRenderer, OdTrRndLocalRendererTPtrAdaptor>::operator =(ref)); }
    void release() { *this = TPtr<OdTrRndLocalRenderer, OdTrRndLocalRendererTPtrAdaptor>::operator =(NULL); }
};

#include "TD_PackPop.h"

#endif // ODTRRNDLOCALRENDERER
