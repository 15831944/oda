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
// Scene Graph settings user interface component

#ifndef ODTRRNDSGSCENEGRAPHSETTINGSUI
#define ODTRRNDSGSCENEGRAPHSETTINGSUI

#include "../extbar/ExtBar.h"

#include "TD_PackPush.h"

/* @@@TODO: Currently we have duplication of OdTrRndSgSceneGraphGlobalSettings and
            OdTrRndSgRootConfig data structures here. I think more generic part
            of headers must be moved to public area in future. Moreover
            Scene Graph can be used for custom renderer building as part of
            TrRenderBase static library.                                           */

/** \details
    <group ExRender_Windows_Classes>
*/
class OdTrRndSgSceneGraphSettingsUI : public ExtBar::Callback
{
  public:
    // Global settings definitions
    enum SceneTreeType
    {
      kFlatList = 0,
      kSpatialIndex,
      kOctree,
      kKDTree
    };
    enum BalancingMode
    {
      kNoBalancing = 0,   // Disable balancing at all
      kSimpleBalancing,   // Enable balancing for compatible geometries only
      kWeightedBalancing, // Enable weighted balancing (balancing depends from weight of geometries)
      kChaoticBalancing   // Enable balancing without additional checks (helpful for performance measurements)
    };
  protected:
    enum Flags
    {
      // Global settings flags
      kDisplayFPS             = (1 << 0),
      kDisplayStats           = (1 << 1),
      kFadeUnions             = (1 << 2),
      kColorDataExt           = (1 << 3),
      kLineStripsExplode      = (1 << 4),
      kTriangleStripsExplode  = (1 << 5),
      kFlattenTtfTexts        = (1 << 6),
      kExplodeBlockRefs       = (1 << 7),
      kEmbedOrdering          = (1 << 8),
      kPurgeGrouppedRenders   = (1 << 9),
      // Roots configuration flags
      kVisualizeRenders       = (1 << 0), // Display renders
      kVisualizeTreeStructure = (1 << 1), // Display tree structure
      kVisualizeRenderExtents = (1 << 2)  // Display render extents
    };
  protected:
    enum UIElements
    {
      kUIGlobalSettingsGroup = 100,
      kUIDisplayFPSCheckBox,       kUIDisplayFPSText,
      kUIDisplayStatsCheckBox,     kUIDisplayStatsText,
      kUIUnionsFadingCheckBox,     kUIUnionsFadingText,
      kUIColorDataExtCheckBox,     kUIColorDataExtText,
      kUILineStripsExpCheckBox,    kUILineStripsExpText,
      kUITrgStripsExpCheckBox,     kUITrgStripsExpText,
      kUIFlattenTtfCheckBox,       kUIFlattenTtfText,
      kUIExplodeBlockRefsCheckBox, kUIExplodeBlockRefsText,
      kUIEmbedOrderingCheckBox,    kUIEmbedOrderingText,
      kUIPurgeGrouppedCheckBox,    kUIPurgeGrouppedText,
      kUISceneTreeTypeComboBox,    kUISceneTreeTypeText,
      kUIAuxTreeTypeComboBox,      kUIAuxTreeTypeText,
      kUIBalancingModeComboBox,    kUIBalancingModeText,
      kUIWeightLimitRangeBox,      kUIWeightLimitText,
      kUIBalancingLimitRangeBox,   kUIBalancingLimitText,
      kUIBalancingDepthRangeBox,   kUIBalancingDepthText,
      kUIMaxUnionSizeRangeBox,     kUIMaxUnionSizeText,
      kUILineStripsLimRangeBox,    kUILineStripsLimText,
      kUITrgStripsLimRangeBox,     kUITrgStripsLimText,
      kUIBlkRefExpLimRangeBox,     kUIBlkRefExpLimText,
      kUINumThreadsRangeBox,       kUINumThreadsText,
      kUIRootConfigGroup = 200,
      kUIVisualizeRendersCheckBox, kUIVisualizeRendersText,
      kUIVisualizeTreesCheckBox,   kUIVisualizeTreesText,
      kUIVisualizeExtentsCheckBox, kUIVisualizeExtentsText,
      kUIMaxTreeDepthRangeBox,     kUIMaxTreeDepthText,
      kUIMaxNodeObjectsRangeBox,   kUIMaxNodeObjectsText,
      kUIMinNodeSizeRangeBox,      kUIMinNodeSizeText,
      kUINodeFittingRatioRangeBox, kUINodeFittingRatioText,
      kUIFirstNodeMultRangeBox,    kUIFirstNodeMultText
    };
  protected:
    OdUInt16                   m_ngsFlags;             // Global settings flags
    OdUInt16                   m_nrcFlags;             // Root configuration flags
    // Global settings
    SceneTreeType              m_sceneTreeType;        // Tree structure, which will be used to store scene contents
    SceneTreeType              m_auxTreeType;          // Tree structure, which will be used to store auxiliary graphics
    BalancingMode              m_balancingMode;        // Rendering speed balancing mode
    OdUInt16                   m_prcMinWeight;         // Minimal balancing weight for weighted balancing mode
    OdUInt16                   m_prcMinDepth;          // Minimal balancing tree level (in percents of entire tree depth)
    OdUInt16                   m_prcMergeDepth;        // Merging inclusion childs depth (in percents of entire tree depth)
    OdUInt16                   m_nThreadsMax;          // Maximal number of utilizeable threads (can be null to disable multithreading)
    OdUInt32                   m_nMaxUnionSize;        // Maximal length of unionized data stream
    OdUInt32                   m_nLineStripsLim;       // Maximal number of vertex in line strips/loops which can be exploded into set of simple lines
    OdUInt32                   m_nTrgStripsLim;        // Maximal number of vertex in triangle strips/fans which can be exploded into set of simple triangles
    OdUInt32                   m_nBlkRefExpLim;        // Maximal number of nested block reference metafiles for explode inside primary scene graph
    // Roots configuration
    OdUInt32                   m_nMaxTreeDepth;        // Maximal tree depth (maximal number of tree branches subdivision)
    OdUInt32                   m_nMaxObjectsSubdivide; // Maximal number of objects onto tree leafs before subdivision (best between 8-16)
    double                     m_fMinTreeNodeSize;     // Minimal tree node size (should be small-enough)
    double                     m_fNodeFittingRatio;    // Coefficient between 1 (disabled) and 2 (maximal). Enlarges tree nodes area to overlap nearest nodes.
    double                     m_fInitialNodeMult;     // First tree node extents multiplier (actual for Octree)
  public:
    OdTrRndSgSceneGraphSettingsUI()
    { reset(); }
    void reset()
    {
      m_ngsFlags = kColorDataExt | kLineStripsExplode | kFlattenTtfTexts | kExplodeBlockRefs | kEmbedOrdering | kPurgeGrouppedRenders;
      m_nrcFlags = kVisualizeRenders;
      // Global settings
      m_sceneTreeType = kOctree;
      m_auxTreeType   = kKDTree;
      m_balancingMode = kWeightedBalancing;
      m_prcMinWeight  = 50;
      m_prcMinDepth   = 10;
      m_prcMergeDepth = 15;
      m_nThreadsMax   = 4;
      m_nMaxUnionSize = 0xFA00000; // 250 Mb
      m_nLineStripsLim = 512;
      m_nTrgStripsLim = 342;
      m_nBlkRefExpLim = 128;
      // Roots configuration
      m_nMaxTreeDepth        = 30;
      m_nMaxObjectsSubdivide = 20;
      m_fMinTreeNodeSize     = 1.e-4;
      m_fNodeFittingRatio    = 1.2;
      m_fInitialNodeMult     = 10.0;
    }

    // Global settings

    void setDisplayFPS(bool bEnable) { SETBIT(m_ngsFlags, kDisplayFPS, bEnable); }
    bool displayFPS() const { return GETBIT(m_ngsFlags, kDisplayFPS); }

    void setDisplayStatistics(bool bEnable) { SETBIT(m_ngsFlags, kDisplayStats, bEnable); }
    bool displayStatistics() const { return GETBIT(m_ngsFlags, kDisplayStats); }

    void setUnionsFading(bool bEnable) { SETBIT(m_ngsFlags, kFadeUnions, bEnable); }
    bool fadeUnions() const { return GETBIT(m_ngsFlags, kFadeUnions); }

    void setColorDataExtensible(bool bEnable) { SETBIT(m_ngsFlags, kColorDataExt, bEnable); }
    bool colorDataExtensible() const { return GETBIT(m_ngsFlags, kColorDataExt); }

    void setLineStripsExplode(bool bEnable) { SETBIT(m_ngsFlags, kLineStripsExplode, bEnable); }
    bool explodeLineStrips() const { return GETBIT(m_ngsFlags, kLineStripsExplode); }

    void setTriangleStripsExplode(bool bEnable) { SETBIT(m_ngsFlags, kTriangleStripsExplode, bEnable); }
    bool explodeTriangleStrips() const { return GETBIT(m_ngsFlags, kTriangleStripsExplode); }

    void setTtfTextsFlattening(bool bEnable) { SETBIT(m_ngsFlags, kFlattenTtfTexts, bEnable); }
    bool flattenTtfTexts() const { return GETBIT(m_ngsFlags, kFlattenTtfTexts); }

    void setBlockRefsExploding(bool bEnable) { SETBIT(m_ngsFlags, kExplodeBlockRefs, bEnable); }
    bool explodeBlockRefs() const { return GETBIT(m_ngsFlags, kExplodeBlockRefs); }

    void setDrawOrderEmbedding(bool bEnable) { SETBIT(m_ngsFlags, kEmbedOrdering, bEnable); }
    bool embedDrawOrder() const { return GETBIT(m_ngsFlags, kEmbedOrdering); }

    void setPurgeGrouppedRenders(bool bEnable) { SETBIT(m_ngsFlags, kPurgeGrouppedRenders, bEnable); }
    bool purgeGrouppedRenders() const { return GETBIT(m_ngsFlags, kPurgeGrouppedRenders); }

    void setSceneTreeType(SceneTreeType treeType) { m_sceneTreeType = treeType; }
    SceneTreeType sceneTreeType() const { return m_sceneTreeType; }

    void setAuxiliaryTreeType(SceneTreeType treeType) { m_auxTreeType = treeType; }
    SceneTreeType auxiliaryTreeType() const { return m_auxTreeType; }

    void setBalancingMode(BalancingMode balancingMode) { m_balancingMode = balancingMode; }
    BalancingMode balancingMode() const { return m_balancingMode; }

    void setWeightedBalancingLimit(OdUInt32 weightLimit) { m_prcMinWeight = (OdUInt16)weightLimit; }
    OdUInt32 weightedBalancingLimit() const { return m_prcMinWeight; }

    void setBalancingLevelLimit(OdUInt32 minDepthLimit) { m_prcMinDepth = (OdUInt16)minDepthLimit; }
    OdUInt32 balancingLevelLimit() const { return m_prcMinDepth; }

    void setBalancingMergeDepth(OdUInt32 mergeDepth) { m_prcMergeDepth = (OdUInt16)mergeDepth; }
    OdUInt32 balancingMergeDepth() const { return m_prcMergeDepth; }

    void setMaximalUnionSize(OdUInt32 nBytes) { m_nMaxUnionSize = nBytes; }
    OdUInt32 maximalUnionSize() const { return m_nMaxUnionSize; }

    void setLineStripsExplodeLimit(OdUInt32 expLimit) { m_nLineStripsLim = expLimit; }
    OdUInt32 lineStripsExplodeLimit() const { return m_nLineStripsLim; }

    void setTriangleStripsExplodeLimit(OdUInt32 expLimit) { m_nTrgStripsLim = expLimit; }
    OdUInt32 triangleStripsExplodeLimit() const { return m_nTrgStripsLim; }

    void setBlockRefsExplodeLimit(OdUInt32 nLimit) { m_nBlkRefExpLim = nLimit; }
    OdUInt32 blockRefsExplodeLimit() const { return m_nBlkRefExpLim; }

    void setThreadsMaximum(OdUInt32 nThreadsMax) { m_nThreadsMax = (OdUInt16)nThreadsMax; }
    OdUInt32 threadsMaximum() const { return m_nThreadsMax; }

    // Roots configuration

    bool visualizeRenders() const { return GETBIT(m_nrcFlags, kVisualizeRenders); }
    void setVisualizeRenders(bool bSet) { SETBIT(m_nrcFlags, kVisualizeRenders, bSet); }

    bool visualizeTreeStructure() const { return GETBIT(m_nrcFlags, kVisualizeTreeStructure); }
    void setVisualizeTreeStructure(bool bSet) { SETBIT(m_nrcFlags, kVisualizeTreeStructure, bSet); }

    bool visualizeRenderExtents() const { return GETBIT(m_nrcFlags, kVisualizeRenderExtents); }
    void setVisualizeRenderExtents(bool bSet) { SETBIT(m_nrcFlags, kVisualizeRenderExtents, bSet); }

    OdUInt32 maxTreeDepth() const { return m_nMaxTreeDepth; }
    void setMaxTreeDepth(OdUInt32 nMaxTreeDepth) { m_nMaxTreeDepth = nMaxTreeDepth; }

    OdUInt32 maxObjectsSubdivide() const { return m_nMaxObjectsSubdivide; }
    void setMaxObjectsSubdivide(OdUInt32 nMaxObjectSubdivide) { m_nMaxObjectsSubdivide = nMaxObjectSubdivide; }

    double minTreeNodeSize() const { return m_fMinTreeNodeSize; }
    void setMinTreeNodeSize(double fMinTreeNodeSize) { m_fMinTreeNodeSize = fMinTreeNodeSize; }

    double nodeFittingRatio() const { return m_fNodeFittingRatio; }
    void setNodeFittingRatio(double fNodeFittingRatio) { m_fNodeFittingRatio = fNodeFittingRatio; }

    double firstNodeExtraMultiplier() const { return m_fInitialNodeMult; }
    void setFirstNodeExtraMultiplier(double fInitialNodeMult) { m_fInitialNodeMult = fInitialNodeMult; }

    // Cloning/Copying ability

    template <typename TypeTo, typename TypeFrom>
    static void copyGlobalSettings(TypeTo &to, const TypeFrom &from)
    {
      to.setDisplayFPS(from.displayFPS());
      to.setDisplayStatistics(from.displayStatistics());
      to.setUnionsFading(from.fadeUnions());
      to.setColorDataExtensible(from.colorDataExtensible());
      to.setLineStripsExplode(from.explodeLineStrips());
      to.setTriangleStripsExplode(from.explodeTriangleStrips());
      to.setTtfTextsFlattening(from.flattenTtfTexts());
      to.setBlockRefsExploding(from.explodeBlockRefs());
      to.setDrawOrderEmbedding(from.embedDrawOrder());
      to.setPurgeGrouppedRenders(from.purgeGrouppedRenders());
      to.setSceneTreeType((OD_TYPENAME TypeTo::SceneTreeType)from.sceneTreeType());
      to.setAuxiliaryTreeType((OD_TYPENAME TypeTo::SceneTreeType)from.auxiliaryTreeType());
      to.setBalancingMode((OD_TYPENAME TypeTo::BalancingMode)from.balancingMode());
      to.setWeightedBalancingLimit(from.weightedBalancingLimit());
      to.setBalancingLevelLimit(from.balancingLevelLimit());
      to.setBalancingMergeDepth(from.balancingMergeDepth());
      to.setMaximalUnionSize(from.maximalUnionSize());
      to.setLineStripsExplodeLimit(from.lineStripsExplodeLimit());
      to.setTriangleStripsExplodeLimit(from.triangleStripsExplodeLimit());
      to.setBlockRefsExplodeLimit(from.blockRefsExplodeLimit());
      to.setThreadsMaximum(from.threadsMaximum());
    }
    template <typename TypeTo, typename TypeFrom>
    static void copyRootsConfig(TypeTo &to, const TypeFrom &from)
    {
      to.setVisualizeRenders(from.visualizeRenders());
      to.setVisualizeTreeStructure(from.visualizeTreeStructure());
      to.setVisualizeRenderExtents(from.visualizeRenderExtents());
      to.setMaxTreeDepth(from.maxTreeDepth());
      to.setMaxObjectsSubdivide(from.maxObjectsSubdivide());
      to.setMinTreeNodeSize(from.minTreeNodeSize());
      to.setNodeFittingRatio(from.nodeFittingRatio());
      to.setFirstNodeExtraMultiplier(from.firstNodeExtraMultiplier());
    }
    OdTrRndSgSceneGraphSettingsUI &operator =(const OdTrRndSgSceneGraphSettingsUI &uiSets)
    {
      copyGlobalSettings(*this, uiSets);
      copyRootsConfig(*this, uiSets);
      return *this;
    }

    // ExtBar interface

    virtual void fillExtBar(ExtBar *pExtBar)
    { // Initialize Global Settings
      ExtBar::GroupElement *pGroup = pExtBar->createGroup(kUIGlobalSettingsGroup);
      ExtBar::TextElement::cast(pGroup)->setText(OD_T("Global Settings"));
      // Display FPS
      pExtBar->createCheckBox(kUIDisplayFPSCheckBox, pGroup, ExtBar::k8d8, displayFPS())->setText(OD_T("Display FPS"));
      // Display Statistics
      pExtBar->createCheckBox(kUIDisplayStatsCheckBox, pGroup, ExtBar::k8d8, displayStatistics())->setText(OD_T("Display Statistics and Measurements"));
      // Display Unions Faded
      pExtBar->createCheckBox(kUIUnionsFadingCheckBox, pGroup, ExtBar::k8d8, fadeUnions())->setText(OD_T("Display Unions Faded"));
      // Color Data Extensible
      pExtBar->createCheckBox(kUIColorDataExtCheckBox, pGroup, ExtBar::k8d8, colorDataExtensible())->setText(OD_T("Extend Color Data if Possible"));
      // Line Strips Explode
      pExtBar->createCheckBox(kUILineStripsExpCheckBox, pGroup, ExtBar::k8d8, explodeLineStrips())->setText(OD_T("Explode Line Strips/Loops"));
      // Triangle Strips Explode
      pExtBar->createCheckBox(kUITrgStripsExpCheckBox, pGroup, ExtBar::k8d8, explodeTriangleStrips())->setText(OD_T("Explode Triangle Strips/Fans"));
      // TrueType Texts Flattening
      pExtBar->createCheckBox(kUIFlattenTtfCheckBox, pGroup, ExtBar::k8d8, flattenTtfTexts())->setText(OD_T("Flatten TrueType Texts"));
      // Block References Exploding
      pExtBar->createCheckBox(kUIExplodeBlockRefsCheckBox, pGroup, ExtBar::k8d8, explodeBlockRefs())->setText(OD_T("Explode Block References"));
      // Draw Order Embedding
      pExtBar->createCheckBox(kUIEmbedOrderingCheckBox, pGroup, ExtBar::k8d8, embedDrawOrder())->setText(OD_T("Enable Draw Order Embedding"));
      // Groupped Renders Purging
      pExtBar->createCheckBox(kUIPurgeGrouppedCheckBox, pGroup, ExtBar::k8d8, purgeGrouppedRenders())->setText(OD_T("Purge Groupped Renders"));
      // Scene Tree Type
      pExtBar->createText(kUISceneTreeTypeText, pGroup, ExtBar::k8d8)->setText(OD_T("Scene Tree Type:"));
      { ExtBar::ListElement *pList = pExtBar->createComboBox(kUISceneTreeTypeComboBox, pGroup);
        pList->addString(OD_T("Flat List")); pList->addString(OD_T("Spatial Index"));
        pList->addString(OD_T("Octree"));    pList->addString(OD_T("KdTree"));
        pList->setSelString((int)sceneTreeType()); }
      // Auxiliary Tree Type
      pExtBar->createText(kUIAuxTreeTypeText, pGroup, ExtBar::k8d8)->setText(OD_T("Auxiliary Tree Type:"));
      { ExtBar::ListElement *pList = pExtBar->createComboBox(kUIAuxTreeTypeComboBox, pGroup);
        pList->addString(OD_T("Flat List")); pList->addString(OD_T("Spatial Index"));
        pList->addString(OD_T("Octree"));    pList->addString(OD_T("KdTree"));
        pList->setSelString((int)auxiliaryTreeType()); }
      // Balancing Mode
      pExtBar->createText(kUIBalancingModeText, pGroup, ExtBar::k8d8)->setText(OD_T("Balancing Mode:"));
      { ExtBar::ListElement *pList = pExtBar->createComboBox(kUIBalancingModeComboBox, pGroup);
        pList->addString(OD_T("No Balancing"));       pList->addString(OD_T("Simple Balancing"));
        pList->addString(OD_T("Weighted Balancing")); pList->addString(OD_T("Chaotic Balancing"));
        pList->setSelString((int)balancingMode()); }
      // Weighted Balancing Limit
      pExtBar->createText(kUIWeightLimitText, pGroup, ExtBar::k6d8)->setText(OD_T("Weighted Balancing Limit (%):"));
      pExtBar->createRangeBox(kUIWeightLimitRangeBox, pGroup, ExtBar::k2d8)->setUInt(weightedBalancingLimit(), 0, 100);
      // Balancing Level Limit
      pExtBar->createText(kUIBalancingLimitText, pGroup, ExtBar::k6d8)->setText(OD_T("Balancing Level Limit (%):"));
      pExtBar->createRangeBox(kUIBalancingLimitRangeBox, pGroup, ExtBar::k2d8)->setUInt(balancingLevelLimit(), 0, 100);
      // Balancing Merge Depth
      pExtBar->createText(kUIBalancingDepthText, pGroup, ExtBar::k6d8)->setText(OD_T("Balancing Merge Depth (%):"));
      pExtBar->createRangeBox(kUIBalancingDepthRangeBox, pGroup, ExtBar::k2d8)->setUInt(balancingMergeDepth(), 0, 100);
      // Maximal unionized stream length
      pExtBar->createText(kUIMaxUnionSizeText, pGroup, ExtBar::k6d8)->setText(OD_T("Maximal Union Size (Bytes):"));
      pExtBar->createRangeBox(kUIMaxUnionSizeRangeBox, pGroup, ExtBar::k2d8)->setUInt(maximalUnionSize(), 0x100000, 0xFFFFFFFF);
      // Line Strips Exploding Limit
      pExtBar->createText(kUILineStripsLimText, pGroup, ExtBar::k6d8)->setText(OD_T("Line Strips Explode Limit:"));
      pExtBar->createRangeBox(kUILineStripsLimRangeBox, pGroup, ExtBar::k2d8)->setUInt(lineStripsExplodeLimit());
      // Triangle Strips Exploding Limit
      pExtBar->createText(kUITrgStripsLimText, pGroup, ExtBar::k6d8)->setText(OD_T("Triangle Strips Explode Limit:"));
      pExtBar->createRangeBox(kUITrgStripsLimRangeBox, pGroup, ExtBar::k2d8)->setUInt(triangleStripsExplodeLimit());
      // Block References Exploding Limit
      pExtBar->createText(kUIBlkRefExpLimText, pGroup, ExtBar::k6d8)->setText(OD_T("Block References Explode Limit:"));
      pExtBar->createRangeBox(kUIBlkRefExpLimRangeBox, pGroup, ExtBar::k2d8)->setUInt(blockRefsExplodeLimit());
      // Number of Working Threads
      pExtBar->createText(kUINumThreadsText, pGroup, ExtBar::k6d8)->setText(OD_T("Number of Working Threads:"));
      pExtBar->createRangeBox(kUINumThreadsRangeBox, pGroup, ExtBar::k2d8)->setUInt(threadsMaximum(), 0, 64);
      // Roots configuration
      pGroup = pExtBar->createGroup(kUIRootConfigGroup);
      ExtBar::TextElement::cast(pGroup)->setText(OD_T("Roots Configuration"));
      // Visualize Renders
      pExtBar->createCheckBox(kUIVisualizeRendersCheckBox, pGroup, ExtBar::k8d8, visualizeRenders())->setText(OD_T("Visualize Renders"));
      // Visualize Tree Structure
      pExtBar->createCheckBox(kUIVisualizeTreesCheckBox, pGroup, ExtBar::k8d8, visualizeTreeStructure())->setText(OD_T("Visualize Tree Structure"));
      // Visualize Render Extents
      pExtBar->createCheckBox(kUIVisualizeExtentsCheckBox, pGroup, ExtBar::k8d8, visualizeRenderExtents())->setText(OD_T("Visualize Renders and Unions Extents"));
      // Maximal Tree Depth
      pExtBar->createText(kUIMaxTreeDepthText, pGroup, ExtBar::k6d8)->setText(OD_T("Maximal Tree Depth:"));
      pExtBar->createRangeBox(kUIMaxTreeDepthRangeBox, pGroup, ExtBar::k2d8)->setUInt(maxTreeDepth(), 2);
      // Maximal Objects Subdivide
      pExtBar->createText(kUIMaxNodeObjectsText, pGroup, ExtBar::k6d8)->setText(OD_T("Maximal Tree Node Objects before Subdivide:"));
      pExtBar->createRangeBox(kUIMaxNodeObjectsRangeBox, pGroup, ExtBar::k2d8)->setUInt(maxObjectsSubdivide(), 2);
      // Minimal Tree Node Size
      pExtBar->createText(kUIMinNodeSizeText, pGroup, ExtBar::k6d8)->setText(OD_T("Minimal Tree Node Size:"));
      pExtBar->createRangeBox(kUIMinNodeSizeRangeBox, pGroup, ExtBar::k2d8, false, true)->setFP(minTreeNodeSize(), 1.e-10, 1.e+10, 1.e-3);
      // Node Fitting Ratio
      pExtBar->createText(kUINodeFittingRatioText, pGroup, ExtBar::k6d8)->setText(OD_T("Tree Node Fitting Ratio:"));
      pExtBar->createRangeBox(kUINodeFittingRatioRangeBox, pGroup, ExtBar::k2d8, false, true)->setFP(nodeFittingRatio(), 1.0, 2.0, 0.01);
      // First Node Extra Multiplier
      pExtBar->createText(kUIFirstNodeMultText, pGroup, ExtBar::k6d8)->setText(OD_T("Initial Tree Node Extra Extents Multiplier:"));
      pExtBar->createRangeBox(kUIFirstNodeMultRangeBox, pGroup, ExtBar::k2d8, false, true)->setFP(firstNodeExtraMultiplier(), 1.e-2, 1.e+4, 0.01);
    }
    virtual void closeExtBar(ExtBar *pExtBar, int nReason)
    { if (!nReason) return;
      // Grab Global Settings
      setDisplayFPS(ExtBar::CheckElement::cast(pExtBar->getElement(kUIDisplayFPSCheckBox))->isChecked());
      setDisplayStatistics(ExtBar::CheckElement::cast(pExtBar->getElement(kUIDisplayStatsCheckBox))->isChecked());
      setUnionsFading(ExtBar::CheckElement::cast(pExtBar->getElement(kUIUnionsFadingCheckBox))->isChecked());
      setColorDataExtensible(ExtBar::CheckElement::cast(pExtBar->getElement(kUIColorDataExtCheckBox))->isChecked());
      setLineStripsExplode(ExtBar::CheckElement::cast(pExtBar->getElement(kUILineStripsExpCheckBox))->isChecked());
      setTriangleStripsExplode(ExtBar::CheckElement::cast(pExtBar->getElement(kUITrgStripsExpCheckBox))->isChecked());
      setTtfTextsFlattening(ExtBar::CheckElement::cast(pExtBar->getElement(kUIFlattenTtfCheckBox))->isChecked());
      setBlockRefsExploding(ExtBar::CheckElement::cast(pExtBar->getElement(kUIExplodeBlockRefsCheckBox))->isChecked());
      setDrawOrderEmbedding(ExtBar::CheckElement::cast(pExtBar->getElement(kUIEmbedOrderingCheckBox))->isChecked());
      setPurgeGrouppedRenders(ExtBar::CheckElement::cast(pExtBar->getElement(kUIPurgeGrouppedCheckBox))->isChecked());
      setSceneTreeType((SceneTreeType)ExtBar::ListElement::cast(pExtBar->getElement(kUISceneTreeTypeComboBox))->getSelString());
      setAuxiliaryTreeType((SceneTreeType)ExtBar::ListElement::cast(pExtBar->getElement(kUIAuxTreeTypeComboBox))->getSelString());
      setBalancingMode((BalancingMode)ExtBar::ListElement::cast(pExtBar->getElement(kUIBalancingModeComboBox))->getSelString());
      setWeightedBalancingLimit((OdUInt16)ExtBar::RangeElement::cast(pExtBar->getElement(kUIWeightLimitRangeBox))->getPosUInt());
      setBalancingLevelLimit((OdUInt16)ExtBar::RangeElement::cast(pExtBar->getElement(kUIBalancingLimitRangeBox))->getPosUInt());
      setBalancingMergeDepth((OdUInt16)ExtBar::RangeElement::cast(pExtBar->getElement(kUIBalancingDepthRangeBox))->getPosUInt());
      setMaximalUnionSize(ExtBar::RangeElement::cast(pExtBar->getElement(kUIMaxUnionSizeRangeBox))->getPosUInt());
      setLineStripsExplodeLimit(ExtBar::RangeElement::cast(pExtBar->getElement(kUILineStripsLimRangeBox))->getPosUInt());
      setTriangleStripsExplodeLimit(ExtBar::RangeElement::cast(pExtBar->getElement(kUITrgStripsLimRangeBox))->getPosUInt());
      setBlockRefsExplodeLimit(ExtBar::RangeElement::cast(pExtBar->getElement(kUIBlkRefExpLimRangeBox))->getPosUInt());
      setThreadsMaximum((OdUInt16)ExtBar::RangeElement::cast(pExtBar->getElement(kUINumThreadsRangeBox))->getPosUInt());
      // Grab Roots Configuration
      setVisualizeRenders(ExtBar::CheckElement::cast(pExtBar->getElement(kUIVisualizeRendersCheckBox))->isChecked());
      setVisualizeTreeStructure(ExtBar::CheckElement::cast(pExtBar->getElement(kUIVisualizeTreesCheckBox))->isChecked());
      setVisualizeRenderExtents(ExtBar::CheckElement::cast(pExtBar->getElement(kUIVisualizeExtentsCheckBox))->isChecked());
      setMaxTreeDepth(ExtBar::RangeElement::cast(pExtBar->getElement(kUIMaxTreeDepthRangeBox))->getPosUInt());
      setMaxObjectsSubdivide(ExtBar::RangeElement::cast(pExtBar->getElement(kUIMaxNodeObjectsRangeBox))->getPosUInt());
      setMinTreeNodeSize(ExtBar::RangeElement::cast(pExtBar->getElement(kUIMinNodeSizeRangeBox))->getPosFP());
      setNodeFittingRatio(ExtBar::RangeElement::cast(pExtBar->getElement(kUINodeFittingRatioRangeBox))->getPosFP());
      setFirstNodeExtraMultiplier(ExtBar::RangeElement::cast(pExtBar->getElement(kUIFirstNodeMultRangeBox))->getPosFP());
    }
    virtual void elementChanged(ExtBar::Element * /*pElem*/) { /* if some element will require special handling */ }
};

#include "TD_PackPop.h"

#endif // ODTRRNDSGSCENEGRAPHSETTINGSUI
