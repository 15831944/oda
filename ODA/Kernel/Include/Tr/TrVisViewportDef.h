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
// GLES2 device viewport definition

#ifndef ODTRVISVIEWPORTDEF
#define ODTRVISVIEWPORTDEF

#include "TD_PackPush.h"

#include "Ge/GeMatrix3d.h"
#include "IntArray.h"
#include "Ge/GePoint2dArray.h"
#include "UInt8Array.h"

#include "TrVisRenderMode.h"
#include "TrVisOverlayDef.h"

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportDef
{
  // Flags
  enum ViewportFlags
  {
    kPSOverallViewport = (1 << 0), // Overall PaperSpace viewport
    kPSModelViewport   = (1 << 1), // ModelSpace viewport in PaperSpace
    kPSHelperViewport  = (1 << 2), // Helper PaperSpace viewport
    kVpVisible         = (1 << 3), // Viewport is visible
    // Store last definition flag for extensibility
    kLastViewportDefFlag = kVpVisible
  };

  OdUInt16 m_vpFlags;
  OdTrVisViewportId m_overallViewportId;

  // Reset viewport flags
  void resetViewportFlags(OdUInt16 vpFlags = 0) { m_vpFlags = vpFlags; }

  // PSOverallViewport flag
  bool isPSOverallViewport() const { return GETBIT(m_vpFlags, kPSOverallViewport); }
  void setPSOverallViewport(bool bSet) { SETBIT(m_vpFlags, kPSOverallViewport, bSet); }

  // PSModelViewport flag
  bool isPSModelViewport() const { return GETBIT(m_vpFlags, kPSModelViewport); }
  void setPSModelViewport(bool bSet) { SETBIT(m_vpFlags, kPSModelViewport, bSet); }

  // PSHelperViewport flag
  bool isPSHelperViewport() const { return GETBIT(m_vpFlags, kPSHelperViewport); }
  void setPSHelperViewport(bool bSet) { SETBIT(m_vpFlags, kPSHelperViewport, bSet); }

  // VpVisible flag
  bool isViewportVisible() const { return GETBIT(m_vpFlags, kVpVisible); }
  void setViewportVisible(bool bSet) { SETBIT(m_vpFlags, kVpVisible, bSet); }

  // Setup default values
  void setDefault()
  {
    resetViewportFlags();
    m_overallViewportId = kTrVisNullId;
  }

  // Overall viewport Id
  OdTrVisViewportId overallViewportId() const { return m_overallViewportId; }
  void setOverallViewportId(OdTrVisViewportId vpId) { m_overallViewportId = vpId; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportRegionDef
{
  OdIntArray   m_counts;
  OdFloatArray m_points;

  void reset()
  {
    m_counts.setPhysicalLength(0);
    m_points.setPhysicalLength(0);
  }
  bool hasRegion() const { return !m_counts.isEmpty(); }

  bool hasCounts() const { return !m_counts.isEmpty(); }
  bool hasPoints() const { return !m_points.isEmpty(); }

  OdUInt32 numCounts() const { return m_counts.size(); }
  OdUInt32 numPoints() const { return m_points.size() / 2; }

  void setCounts(const OdIntArray &aCounts) { m_counts = aCounts; }
  void setPoints(const OdFloatArray &aPoints) { m_points = aPoints; }

  const int *counts() const { return m_counts.getPtr(); }
  const float *points() const { return m_points.getPtr(); }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportRegionXform
{
  // Full (non-clipped) viewport rectangle in screen coordinates
  OdGsDCRectDouble m_viewportRect;
  // Screen coordinates to local coordinates transform
  OdGeMatrix3d     m_regionXform;

  void reset()
  {
    m_viewportRect.m_min = OdGePoint2d(0.0, 1.0);
    m_viewportRect.m_max = OdGePoint2d(1.0, 0.0);
    m_regionXform = OdGeMatrix3d::kIdentity;
  }

  const OdGsDCRectDouble &viewportRect() const { return m_viewportRect; }
  void setViewportRect(const OdGsDCRectDouble &vptRect) { m_viewportRect = vptRect; }

  const OdGeMatrix3d &regionXform() const { return m_regionXform; }
  void setRegionXform(const OdGeMatrix3d &regXform) { m_regionXform = regXform; }

  void localViewportRect(OdGsDCRectDouble &rectOut) const
  { rectOut = m_viewportRect; xform2d(rectOut.m_min); xform2d(rectOut.m_max); }
  OdGsDCRectDouble localViewportRect() const
  { OdGsDCRectDouble viewportRect; localViewportRect(viewportRect); return viewportRect; }

  OdGePoint2d localPoint(const OdGePoint2d &screenPt) const
  { OdGePoint2d pt(screenPt); xform2d(pt); return pt; }
  void localPoint(float *ptOut, const float *ptIn) const
  { ptOut[0] = ptIn[0]; ptOut[1] = ptIn[1]; xform2d(ptOut); }

  void localRegion(OdTrVisViewportRegionDef &regionOut, const OdTrVisViewportRegionDef &regionIn) const
  { regionOut = regionIn; const OdUInt32 nPoints = regionOut.m_points.size();
    float *pPtOut = regionOut.m_points.asArrayPtr();
    for (OdUInt32 nPoint = 0; nPoint < nPoints; nPoint++, pPtOut += 2)
      xform2d(pPtOut); }
  OdTrVisViewportRegionDef localRegion(const OdTrVisViewportRegionDef &region) const
  { OdTrVisViewportRegionDef regionOut; localRegion(regionOut, region); return regionOut; }
protected:
  inline void xform2d(OdGePoint2d &pt) const
  { pt = OdGePoint3d(pt.x, pt.y, 0.0).transformBy(m_regionXform).convert2d(); }
  inline void xform2d(float *pPt) const
  { OdGePoint3d transformPt(pPt[0], pPt[1], 0.0); transformPt.transformBy(m_regionXform);
    pPt[0] = (float)transformPt.x; pPt[1] = (float)transformPt.y; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportBorderDef
{
  ODCOLORREF m_color;
  int m_width;
  OdTrVisViewportRegionDef m_region;

  void setDefault()
  {
    m_color = 0;
    m_width = 1;
    m_region.reset();
  }

  void setColor(ODCOLORREF color) { m_color = color; }
  ODCOLORREF color() const { return m_color; }

  void setWidth(int nWidth) { m_width = nWidth; }
  int width() const { return m_width; }

  void setRegion(const OdTrVisViewportRegionDef &region) { m_region = region; }
  const OdTrVisViewportRegionDef &region() const { return m_region; }
  OdTrVisViewportRegionDef &region() { return m_region; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportClipDef
{
  enum Flags
  {
    // Enable non-rectangular clipping.
    kEnableNrcClip = (1 << 0),
    // Enable borders rendering.
    kEnableBorder  = (1 << 1)
  };
  // Viewport clipping flags.
  OdUInt16 m_flags;
  // Viewport clipping rectangle in screen coordinates.
  OdGsDCRectDouble m_screenRect;
  // Viewport local data convertions.
  OdTrVisViewportRegionXform m_localXform;
  // Non-rectangular clipping region in screen coordinates.
  OdTrVisViewportRegionDef m_nrcClip;
  // Border in screen coordinates.
  OdTrVisViewportBorderDef m_border;

  void resetFlags(OdUInt16 nFlags = 0)
  {
    m_flags = nFlags;
  }
  void setDefault()
  {
    resetFlags();
    m_screenRect.m_min = OdGePoint2d(0.0, 1.0);
    m_screenRect.m_max = OdGePoint2d(1.0, 0.0);
    m_localXform.reset();
    m_nrcClip.reset();
    m_border.setDefault();
  }

  void apply(const OdTrVisViewportClipDef &pDef, bool bApplyNrcClip, bool bApplyBorder)
  {
    m_screenRect = pDef.m_screenRect;
    m_localXform = pDef.m_localXform;
    if (bApplyNrcClip)
      enableNrcClip(pDef.isNrcClipEnabled()),
      m_nrcClip = pDef.m_nrcClip;
    if (bApplyBorder)
      enableBorder(pDef.isBorderEnabled()),
      m_border = pDef.m_border;
  }

  void enableNrcClip(bool bSet) { SETBIT(m_flags, kEnableNrcClip, bSet); }
  bool isNrcClipEnabled() const { return GETBIT(m_flags, kEnableNrcClip) && m_nrcClip.hasRegion(); }

  void enableBorder(bool bSet) { SETBIT(m_flags, kEnableBorder, bSet); }
  bool isBorderEnabled() const { return GETBIT(m_flags, kEnableBorder) && m_border.region().hasRegion(); }

  void setScreenRect(const OdGsDCRectDouble &screenRect) { m_screenRect = screenRect; }
  const OdGsDCRectDouble &screenRect() const { return m_screenRect; }

  void setLocalXform(const OdTrVisViewportRegionXform &locXform) { m_localXform = locXform; }
  bool hasLocalXform() const { return !m_localXform.regionXform().isEqualTo(OdGeMatrix3d::kIdentity); }
  const OdTrVisViewportRegionXform &localXform() const { return m_localXform; }
  OdTrVisViewportRegionXform &localXform() { return m_localXform; }

  void setNrcClipRegion(const OdTrVisViewportRegionDef &nrcClip) { m_nrcClip = nrcClip; }
  const OdTrVisViewportRegionDef &nrcClipRegion() const { return m_nrcClip; }
  OdTrVisViewportRegionDef &nrcClipRegion() { return m_nrcClip; }

  void setBorder(const OdTrVisViewportBorderDef &border) { m_border = border; }
  const OdTrVisViewportBorderDef &border() const { return m_border; }
  OdTrVisViewportBorderDef &border() { return m_border; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportShadingDef
{
  OdTrVisProgramId m_rmShader;
  OdTrVisVisualStyleId m_visualStyle;
  OdTrVisRenderMode m_renderMode;
  ODCOLORREF m_faceFillColor;

  void setDefault()
  {
    m_rmShader      = kTrVisNegativeId;
    m_visualStyle   = kTrVisNegativeId;
    m_renderMode    .reset();
    m_faceFillColor = 0;
  }

  void setRenderProgram(OdTrVisProgramId programId) { m_rmShader = programId; }
  OdTrVisProgramId renderProgram() const { return m_rmShader; }
  bool hasRenderProgram() const { return m_rmShader != kTrVisNegativeId; }

  void setVisualStyle(OdTrVisVisualStyleId visualStyleId) { m_visualStyle = visualStyleId; }
  OdTrVisVisualStyleId visualStyle() const { return m_visualStyle; }
  bool hasVisualStyle() const { return m_visualStyle != kTrVisNegativeId; }

  void setRenderMode(const OdTrVisRenderMode &rm) { m_renderMode = rm; }
  const OdTrVisRenderMode &renderMode() const { return m_renderMode; }
  OdTrVisRenderMode &renderMode() { return m_renderMode; }

  void setFaceFillColor(ODCOLORREF fillColor) { m_faceFillColor = fillColor; }
  ODCOLORREF faceFillColor() const { return m_faceFillColor; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportLineStyleDef
{
  ODCOLORREF m_lineStyleConfig;

  void setDefault()
  {
    m_lineStyleConfig = ODRGBA(1, 2, 2, 1);
  }

  void setAsCref(ODCOLORREF cref) { m_lineStyleConfig = cref; }
  ODCOLORREF getAsCref() const { return m_lineStyleConfig; }

  void enablePointsLineweight(bool bEnable)
  { m_lineStyleConfig = ODRGBA((bEnable) ? 1 : 0, ODGETGREEN(m_lineStyleConfig), ODGETBLUE(m_lineStyleConfig), ODGETALPHA(m_lineStyleConfig)); }
  bool isPointsLineweightEnabled() const { return ODGETRED(m_lineStyleConfig) != 0; }

  void setLineCapStyle(OdUInt32 nStyle)
  { m_lineStyleConfig = ODRGBA(ODGETRED(m_lineStyleConfig), nStyle, ODGETBLUE(m_lineStyleConfig), ODGETALPHA(m_lineStyleConfig)); updateDefStyle(); }
  void setLineJoinStyle(OdUInt32 nStyle)
  { m_lineStyleConfig = ODRGBA(ODGETRED(m_lineStyleConfig), ODGETGREEN(m_lineStyleConfig), nStyle, ODGETALPHA(m_lineStyleConfig)); updateDefStyle(); }

  OdUInt32 lineCapStyle() const { return ODGETGREEN(m_lineStyleConfig); }
  OdUInt32 lineJoinStyle() const { return ODGETBLUE(m_lineStyleConfig); }

  void setLineStyle(OdUInt32 nCapStyle, OdUInt32 nJoinStyle)
  { m_lineStyleConfig = ODRGBA(ODGETRED(m_lineStyleConfig), nCapStyle, nJoinStyle, ODGETALPHA(m_lineStyleConfig)); updateDefStyle(); }
  void setLineStyleConfig(bool bPointLwds = true, OdUInt32 nCapStyle = 2, OdUInt32 nJoinStyle = 2)
  { m_lineStyleConfig = ODRGBA((bPointLwds) ? 1 : 0, nCapStyle, nJoinStyle, 0xFF); updateDefStyle(); }

  void updateDefStyle() { m_lineStyleConfig = OD_TRVIS_SETALPHA(m_lineStyleConfig, ((ODGETGREEN(m_lineStyleConfig) == 2) && (ODGETBLUE(m_lineStyleConfig) == 2)) ? 1 : 0); }
  bool isDefStyle() const { return ODGETALPHA(m_lineStyleConfig) != 0; }

  bool operator ==(const OdTrVisViewportLineStyleDef &def) const { return m_lineStyleConfig == def.m_lineStyleConfig; }
  bool operator !=(const OdTrVisViewportLineStyleDef &def) const { return m_lineStyleConfig != def.m_lineStyleConfig; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportFadingDef
{
  ODCOLORREF m_fadingConfig;

  void setDefault()
  {
    m_fadingConfig = ODRGBA(0, 0, 0, 7);
  }

  void setAsCref(ODCOLORREF cref) { m_fadingConfig = cref; }
  ODCOLORREF getAsCref() const { return m_fadingConfig; }

  void setLayerFade(OdUInt8 nFade) { m_fadingConfig = ODRGBA(nFade, ODGETGREEN(m_fadingConfig), ODGETBLUE(m_fadingConfig), ODGETALPHA(m_fadingConfig)); }
  void setXrefFade(OdUInt8 nFade) { m_fadingConfig = ODRGBA(ODGETRED(m_fadingConfig), nFade, ODGETBLUE(m_fadingConfig), ODGETALPHA(m_fadingConfig)); }
  void setMetafileFade(OdUInt8 nFade) { m_fadingConfig = ODRGBA(ODGETRED(m_fadingConfig), ODGETGREEN(m_fadingConfig), nFade, ODGETALPHA(m_fadingConfig)); }
  void setFading(OdUInt8 nLayerFade = 0, OdUInt8 nXrefFade = 0, OdUInt8 nMfFade = 0) { m_fadingConfig = ODRGBA(nLayerFade, nXrefFade, nMfFade, 0); }

  OdUInt8 layerFade() const { return ODGETRED(m_fadingConfig); }
  OdUInt8 xrefFade() const { return ODGETGREEN(m_fadingConfig); }
  OdUInt8 metafileFade() const { return ODGETBLUE(m_fadingConfig); }

  void updateEffectiveFlags()
  { m_fadingConfig = OD_TRVIS_SETALPHA(m_fadingConfig, ((ODGETRED(m_fadingConfig)   != 0xFF) ? 1 : 0) |
                                                       ((ODGETGREEN(m_fadingConfig) != 0xFF) ? 2 : 0) |
                                                       ((ODGETBLUE(m_fadingConfig)  != 0xFF) ? 4 : 0)); }
  OdUInt8 effectiveFlags() const { return ODGETALPHA(m_fadingConfig); }

  bool operator ==(const OdTrVisViewportFadingDef &def) const { return m_fadingConfig == def.m_fadingConfig; }
  bool operator !=(const OdTrVisViewportFadingDef &def) const { return m_fadingConfig != def.m_fadingConfig; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportLineweightsDef
{
  OdUInt8Array m_lineweightsEnum;
  double m_lineweightsCoef;

  void setDefault()
  {
    m_lineweightsEnum.setPhysicalLength(0);
    m_lineweightsCoef = 1.0;
  }

  void setLineweightsEnum(const OdUInt8Array &lwdEnum) { m_lineweightsEnum = lwdEnum; }
  const OdUInt8 *lineweightsEnum() const { return m_lineweightsEnum.getPtr(); }
  OdUInt32 numLineweightsEnum() const { return m_lineweightsEnum.size(); }
  bool hasLineweightsEnum() const { return !m_lineweightsEnum.isEmpty(); }

  void setLineweightsCoef(double lwdCoef) { m_lineweightsCoef = lwdCoef; }
  double lineweightsCoef() const { return m_lineweightsCoef; }
};

/** \details
    <group ExRender_Windows_Classes>
*/
struct OdTrVisViewportData : public OdTrVisViewportDef
{
  OdTrVisViewParamsDef m_viewParams;
  OdTrVisViewportClipDef m_clip;
  OdTrVisViewportShadingDef m_shading;
  OdTrVisViewportLineStyleDef m_linestyle;
  OdTrVisViewportFadingDef m_fading;
  OdTrVisViewportLineweightsDef m_lineweights;

  void setDefault(bool bInitVpDef = true)
  {
    if (bInitVpDef) OdTrVisViewportDef::setDefault();
    m_viewParams.setDefault();
    m_clip.setDefault();
    m_shading.setDefault();
    m_linestyle.setDefault();
    m_fading.setDefault();
    m_lineweights.setDefault();
  }

  void setViewportDef(const OdTrVisViewportDef &vpDef) { *static_cast<OdTrVisViewportDef*>(this) = vpDef; }
  const OdTrVisViewportDef &viewportDef() const { return *this; }
  OdTrVisViewportDef &viewportDef() { return *this; }

  void setViewParams(const OdTrVisViewParamsDef &viewProps) { m_viewParams = viewProps; }
  const OdTrVisViewParamsDef &viewParams() const { return m_viewParams; }
  OdTrVisViewParamsDef &viewParams() { return m_viewParams; }

  void setViewportClip(const OdTrVisViewportClipDef &vpClip, bool bSetNrcClip = true, bool bSetBorder = true)
  { m_clip.apply(vpClip, bSetNrcClip, bSetBorder); }
  const OdTrVisViewportClipDef &viewportClip() const { return m_clip; }
  OdTrVisViewportClipDef &viewportClip() { return m_clip; }

  void setViewportShading(const OdTrVisViewportShadingDef &vpShad) { m_shading = vpShad; }
  const OdTrVisViewportShadingDef &viewportShading() const { return m_shading; }
  OdTrVisViewportShadingDef &viewportShading() { return m_shading; }

  void setViewportLinestyles(const OdTrVisViewportLineStyleDef &vpLs) { m_linestyle = vpLs; }
  const OdTrVisViewportLineStyleDef &viewportLinestyles() const { return m_linestyle; }
  OdTrVisViewportLineStyleDef &viewportLinestyles() { return m_linestyle; }

  void setViewportFading(const OdTrVisViewportFadingDef &vpFade) { m_fading = vpFade; }
  const OdTrVisViewportFadingDef &viewportFading() const { return m_fading; }
  OdTrVisViewportFadingDef &viewportFading() { return m_fading; }

  void setViewportLineweights(const OdTrVisViewportLineweightsDef &vpLwd) { m_lineweights = vpLwd; }
  const OdTrVisViewportLineweightsDef &viewportLineweights() const { return m_lineweights; }
  OdTrVisViewportLineweightsDef &viewportLineweights() { return m_lineweights; }
};

#include "TD_PackPop.h"

#endif // ODTRVISVIEWPORTDEF
