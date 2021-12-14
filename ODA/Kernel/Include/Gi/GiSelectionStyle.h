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

#ifndef __ODGISELECTIONSTYLE_H__
#define __ODGISELECTIONSTYLE_H__

#include "TD_PackPush.h"

#include "Gi/Gi.h"

/** \details
    This class represents selection style data.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class FIRSTDLL_EXPORT OdGiSelectionStyle
{
  public:
    /** \details
      Color mask data.
    */
    class ColorMask
    {
      protected:
        OdCmEntityColor m_color;
        OdUInt32 m_transparency;
      public:
        ColorMask() : m_transparency(255) {}

        /** \details
          Sets color mask color.
          \param color [in]  New color value.
        */
        void setColor(const OdCmEntityColor &color) { m_color = color; }
        /** \details
          Returns currently set color mask color.
        */
        const OdCmEntityColor &color() const { return m_color; }

        /** \details
          Sets color mask transparency.
          \param transparency [in]  New transparency value.
        */
        void setTransparency(OdUInt8 transparency) { m_transparency = transparency; }
        /** \details
          Returns currently set color mask transparency.
        */
        OdUInt8 transparency() const { return isVisible() ? OdUInt8(m_transparency) : 255; }

        /** \details
          Sets color mask color and transparency.
          \param color [in]  New color value.
          \param transparency [in]  New transparency value.
        */
        void setColor(const OdCmEntityColor &color, OdUInt8 transparency) { setColor(color); setTransparency(transparency); }

        /** \details
          Returns true in case if color mask visible (i. e. not fully transparent).
        */
        bool isVisible() const { return m_transparency < 255; }
        /** \details
          Returns true in case if color mask completely opaque.
        */
        bool isOpaque() const { return !m_transparency; }

        /** \details
          Resets color mask color and transparency to default values.
        */
        void reset() { m_color = OdCmEntityColor(); m_transparency = 255; }

        bool operator ==(const ColorMask &cm2) const
        { return (transparency() == cm2.transparency()) && (!isVisible() || color() == cm2.color()); }
        bool operator !=(const ColorMask &cm2) const
        { return (transparency() != cm2.transparency()) ||  (isVisible() && color() != cm2.color()); }
    };
    /** \details
      Basic element style data actual for edges and faces.
    */
    class ElementStyle
    {
      protected:
        enum ElementFlags
        {
          kDrawElement    = (1 << 0), // Enable displaying of edges/faces
          kPatternElement = (1 << 1), // Apply classic highlighting pattern for edges/faces
          kMaskElement    = (1 << 2), // Apply color mask effect to edges/faces
          // Global element flags
          kGOnTopOfDepth  = (1 << 3), // Display element on top of depth buffer
          kGlobalFlags    = kGOnTopOfDepth, // List of global flags
          //
          kLastElementStyleFlag = kGOnTopOfDepth
        };
        OdUInt32 m_nFlags; // Flags
        ColorMask m_colorMask; // Color mask effect properties
      public:
        ElementStyle() : m_nFlags(0) { }

        /** \details
          Enables or disables visibility of current graphics element.
          \param bVisible [in]  New element visibility state.
        */
        void setVisible(bool bVisible) { SETBIT(m_nFlags, kDrawElement, bVisible); }
        /** \details
          Returns current visibility state of current graphics element.
        */
        bool isVisible() const { return GETBIT(m_nFlags, kDrawElement); }

        /** \details
          Enables or disables stippling pattern highlighting for current graphics element.
          \param bEnable [in]  New stippling pattern visibility state.
        */
        void enablePattern(bool bEnable) { SETBIT(m_nFlags, kPatternElement, bEnable); }
        /** \details
          Returns current stippling pattern visibility state for current graphics element.
        */
        bool isPatternEnabled() const { return GETBIT(m_nFlags, kPatternElement); }

        /** \details
          Enables or disables application of color mask for current graphics element.
          \param bEnable [in]  New color mask displaying state.
        */
        void enableColorMasking(bool bEnable) { SETBIT(m_nFlags, kMaskElement, bEnable); }
        /** \details
          Returns current color mask application state for current graphics element.
        */
        bool isColorMaskingEnabled() const { return GETBIT(m_nFlags, kMaskElement); }

        /** \details
          Returns current color mask properties container.
        */
        const ColorMask &colorMask() const { return m_colorMask; }
        /** \details
          Returns current color mask properties editable container.
        */
        ColorMask &colorMask() { return m_colorMask; }

        /** \details
          Returns true in case if current graphics element should be displayed on top of scene graphics in 3d rendering modes.
        */
        bool isOnTopOfDepth() const { return GETBIT(m_nFlags, kGOnTopOfDepth); }

        /** \details
          Returns true in case if any type of visual effect (like stippling pattern or color mask) is
          applicable for current configuration of graphics element.
        */
        bool hasEffect() const { return isPatternEnabled() || (isColorMaskingEnabled() && colorMask().isVisible()); }

        /** \details
          Setup graphics element to display geometry as is, without any visual effects.
        */
        void setByDefault() { m_nFlags = kDrawElement; m_colorMask.reset(); }
        /** \details
          Disable diplaying of graphics element and all visual effects.
        */
        void setAsDisabled() { m_nFlags = 0; m_colorMask.reset(); }
        /** \details
          Setup graphics element to display stippling pattern highlighting visual effect.
        */
        void setForStippling() { m_nFlags = kDrawElement | kPatternElement; m_colorMask.reset(); }
        /** \details
          Setup graphics element to display color mask visual effect with some predefined color and transparency.
        */
        void setForColorMasking() { m_nFlags = kDrawElement | kMaskElement; m_colorMask.setColor(OdCmEntityColor(48, 115, 250), 140); }

        bool operator ==(const ElementStyle &secStyle) const
        { return ((m_nFlags & ~kGlobalFlags) == (secStyle.m_nFlags & ~kGlobalFlags)) && (m_colorMask == secStyle.m_colorMask); }
        bool operator !=(const ElementStyle &secStyle) const
        { return ((m_nFlags & ~kGlobalFlags) != (secStyle.m_nFlags & ~kGlobalFlags)) || (m_colorMask != secStyle.m_colorMask); }
    };
    /** \details
      Face style data.
    */
    typedef ElementStyle FaceStyle;
    /** \details
      Edge style data.
    */
    class EdgeStyle : public ElementStyle
    {
      public:
        /** \details
          Edges diplaying mode.
        */
        enum Mode
        {
          kExistEdges,   // Draw exist edges, if them enabled.
          kIsolineEdges, // Draw isolines for 3d objects and exist edges for others.
          kContourEdges  // Draw contours for 3d objects and exist edges for others.
        };
      protected:
        enum EdgeFlags
        {
          kDrawIsolines = (kLastElementStyleFlag << 1), // Draw isolines for 3d objects
          kDrawContours = (kLastElementStyleFlag << 2)  // Draw contours for 3d objects
        };
        OdInt32 m_lineWeightExtension; // Number of pixels for lineweight enlargement
      public:
        EdgeStyle() : m_lineWeightExtension(0) { }

        /** \details
          Sets edges diplaying mode.
          \param edgeMode [in]  New edges displaying mode.
        */
        void setMode(Mode edgeMode)
        { switch (edgeMode) {
            case kIsolineEdges: SETBIT_1(m_nFlags, kDrawIsolines); SETBIT_0(m_nFlags, kDrawContours); break;
            case kContourEdges: SETBIT_0(m_nFlags, kDrawIsolines); SETBIT_1(m_nFlags, kDrawContours); break;
            default: SETBIT_0(m_nFlags, kDrawIsolines | kDrawContours);
          }
        }
        /** \details
          Return edges diplaying mode.
        */
        Mode mode() const
        { if (GETBIT(m_nFlags, kDrawIsolines)) return kIsolineEdges;
          if (GETBIT(m_nFlags, kDrawContours)) return kContourEdges;
          return kExistEdges;
        }

        /** \details
          Sets edges width extension in pixels.
          \param nLwdExtension [in]  New edges width extension value.
        */
        void setLineWeightExtension(OdInt32 nLwdExtension) { m_lineWeightExtension = nLwdExtension; }
        /** \details
          Returns current edges width extension value in pixels.
        */
        OdInt32 lineWeightExtension() const { return m_lineWeightExtension; }
        /** \details
          Returns true in case if edges width extension enabled (i.e. non zero) for this edge style object.
        */
        bool hasLineWeightExtension() const { return m_lineWeightExtension != 0; }

        /** \details
          Returns true in case if any type of visual effect (like color mask or edges width) is
          applicable for current configuration of this edge style.
        */
        bool hasEffect() const { return ElementStyle::hasEffect() || hasLineWeightExtension() || (mode() != kExistEdges); }

        /** \details
          Setup edges style to display edges geometry as is, without any visual effects.
        */
        void setByDefault() { ElementStyle::setByDefault(); m_lineWeightExtension = 0; }
        /** \details
          Disable diplaying of edges and all visual effects.
        */
        void setAsDisabled() { ElementStyle::setAsDisabled(); m_lineWeightExtension = 0; }
        /** \details
          Setup edges style to display stippling pattern highlighting visual effect.
        */
        void setForStippling() { ElementStyle::setForStippling(); m_lineWeightExtension = 0; }
        /** \details
          Setup edges style to display color mask visual effect with some predefined color and transparency.
          Additionally enables edges width extension and rendering of 3d geometry isolines.
        */
        void setForColorMasking() { ElementStyle::setForColorMasking(); m_colorMask.setTransparency(128);
                                    m_nFlags |= kDrawIsolines; m_lineWeightExtension = 5; }

        bool operator ==(const EdgeStyle &secStyle) const
        { return ElementStyle::operator ==(secStyle) && (m_lineWeightExtension == secStyle.m_lineWeightExtension); }
        bool operator !=(const EdgeStyle &secStyle) const
        { return ElementStyle::operator !=(secStyle) || (m_lineWeightExtension != secStyle.m_lineWeightExtension); }
    };
    /** \details
      Combination of edge and face styles.
    */
    class StyleEntry
    {
      protected:
        EdgeStyle m_edgeStyle;
        FaceStyle m_faceStyle;
      public:
        /** \details
          Returns editable edge style data.
        */
        EdgeStyle &edgeStyle() { return m_edgeStyle; }
        /** \details
          Returns edge style data.
        */
        const EdgeStyle &edgeStyle() const { return m_edgeStyle; }

        /** \details
          Returns editable face style data.
        */
        FaceStyle &faceStyle() { return m_faceStyle; }
        /** \details
          Returns face style data.
        */
        const FaceStyle &faceStyle() const { return m_faceStyle; }

        /** \details
          Returns true in case if edges and faces should be displayed on top of scene graphics in 3d rendering modes.
        */
        bool isOnTopOfDepth() const
        { return m_edgeStyle.isOnTopOfDepth() && m_faceStyle.isOnTopOfDepth(); }

        /** \details
          Returns true in case if edges or faces is visible.
        */
        bool isVisible() const { return m_edgeStyle.isVisible() || m_faceStyle.isVisible(); }
        /** \details
          Returns true in case if any type of visual effect (like color mask, for example) is
          applicable for current configuration of face or edge style.
        */
        bool hasEffect() const { return m_edgeStyle.hasEffect() || m_faceStyle.hasEffect(); }

        /** \details
          Setup face and edge style to display geometry as is, without any visual effects.
        */
        void setByDefault() { m_edgeStyle.setByDefault(); m_faceStyle.setByDefault(); }
        /** \details
          Disable diplaying of faces, edges and all visual effects.
        */
        void setAsDisabled() { m_edgeStyle.setAsDisabled(); m_faceStyle.setAsDisabled(); }
        /** \details
          Setup face and edge styles to display stippling pattern highlighting visual effect.
        */
        void setForStippling() { m_edgeStyle.setForStippling(); m_faceStyle.setForStippling(); }
        /** \details
          Setup face and edge styles to display color mask visual effect with some predefined color and transparency.
          Additionally enables edges width extension and rendering of 3d geometry isolines.
        */
        void setForColorMasking() { m_edgeStyle.setForColorMasking(); m_faceStyle.setForColorMasking(); }

        /** \details
          Setup face and edge styles to display color mask visual effect with specified color and transparency.
          \param color [in]  Color mask color value.
          \param transparency [in]  Color mask transparency value.
        */
        void setForColorMasking(const OdCmEntityColor &color, OdUInt8 transparency)
        { setByDefault();
          m_edgeStyle.enableColorMasking(true); m_edgeStyle.colorMask().setColor(color, transparency);
          m_faceStyle.enableColorMasking(true); m_faceStyle.colorMask().setColor(color, transparency); }

        bool operator ==(const StyleEntry &secStyle) const
        { return (m_edgeStyle == secStyle.m_edgeStyle) && (m_faceStyle == secStyle.m_faceStyle); }
        bool operator !=(const StyleEntry &secStyle) const
        { return (m_edgeStyle != secStyle.m_edgeStyle) || (m_faceStyle != secStyle.m_faceStyle); }
    };
  protected:
    StyleEntry m_styleEntry[4];
  protected:
    struct GIntAccessor : public ElementStyle
    {
      static OdUInt32 getIntFlags(const ElementStyle &elemStyle)
      { return static_cast<const GIntAccessor &>(elemStyle).m_nFlags; }
      static OdUInt32 extractIntFlags(ElementStyle &elemStyle)
      { GIntAccessor &gStyle = static_cast<GIntAccessor &>(elemStyle);
        const OdUInt32 flags = gStyle.m_nFlags & kGlobalFlags; gStyle.m_nFlags &= ~kGlobalFlags;
        return flags; }
      static void installIntFlags(ElementStyle &elemStyle, OdUInt32 flags)
      { GIntAccessor &gStyle = static_cast<GIntAccessor &>(elemStyle);
        gStyle.m_nFlags = (gStyle.m_nFlags & ~kGlobalFlags) | flags; }
      static bool isOnTopOfDepth(OdUInt32 flags) { return GETBIT(flags, kGOnTopOfDepth); }
      static void setOnTopOfDepth(OdUInt32 &flags, bool bSet) { SETBIT(flags, kGOnTopOfDepth, bSet); }
    };
    class WrapGIntFlags { StyleEntry *m_pStyle; OdUInt32 m_flags[2];
      public: WrapGIntFlags(StyleEntry &style, bool bWrap = true) : m_pStyle(bWrap ? &style : NULL)
              { if (m_pStyle) { m_flags[0] = GIntAccessor::extractIntFlags(style.edgeStyle());
                                m_flags[1] = GIntAccessor::extractIntFlags(style.faceStyle()); } }
              ~WrapGIntFlags() { if (m_pStyle) { GIntAccessor::installIntFlags(m_pStyle->edgeStyle(), m_flags[0]);
                                                 GIntAccessor::installIntFlags(m_pStyle->faceStyle(), m_flags[1]); } }
    };
    int makeEntryId(bool bDrawOnTop, bool bDrawIn3d) const { return ((bDrawOnTop) ? 1 : 0) | ((bDrawIn3d) ? 2 : 0); }
  public:
    OdGiSelectionStyle() { }

    /** \details
      Returns editable combination of face and edge styles for specified selection style configuration.
      \param bDrawOnTop [in]  If set to true - returns "draw on top" selection styles, elsewhere returns scene geometry style.
      \param bDrawIn3d [in]  If set to true - returns 3d selection style, elsewhere returns 2d selection style.
    */
    StyleEntry &styleFor(bool bDrawOnTop = false, bool bDrawIn3d = false)
    { return m_styleEntry[makeEntryId(bDrawOnTop, bDrawIn3d)]; }
    /** \details
      Returns combination of face and edge styles for specified selection style configuration.
      \param bDrawOnTop [in]  If set to true - returns "draw on top" selection styles, elsewhere returns scene geometry style.
      \param bDrawIn3d [in]  If set to true - returns 3d selection style, elsewhere returns 2d selection style.
    */
    const StyleEntry &styleFor(bool bDrawOnTop = false, bool bDrawIn3d = false) const 
    { return m_styleEntry[makeEntryId(bDrawOnTop, bDrawIn3d)]; }

    /** \details
      Copies 2d selection style configuration into 3d selection style configuration (copies both top and bottom style components).
    */
    void sync3d() { WrapGIntFlags _wint(styleFor(true, true));
                    styleFor(false, true) = styleFor(false, false); styleFor(true, true) = styleFor(true, false); }
    /** \details
      Copies 3d selection style configuration into 2d selection style configuration (copies both top and bottom style components).
    */
    void sync2d() { WrapGIntFlags _wint(styleFor(true, true));
                    styleFor(false, false) = styleFor(false, true); styleFor(true, false) = styleFor(true, true); }

    /** \details
      Exchanges "draw on top" style components with scene geometry style components for specified selection style configurations.
      \param bFor2d [in]  Exchange components for 2d selection style.
      \param bFor3d [in]  Exchange components for 3d selection style.
    */
    void exchangeTopBottom(bool bFor2d = true, bool bFor3d = true)
    { StyleEntry tmpEntry; WrapGIntFlags _wint(styleFor(true, true), bFor3d);
      if (bFor2d) { tmpEntry = styleFor(false, false); styleFor(false, false) = styleFor(true, false); styleFor(true, false) = tmpEntry; }
      if (bFor3d) { tmpEntry = styleFor(false, true);  styleFor(false, true)  = styleFor(true, true);  styleFor(true, true)  = tmpEntry; }
    }

    /** \details
      By default "draw on top" style redered on top of order, but with this flag it can be rendered on top of depth in 3d rendering modes.
      \param bSet [in]  New value for "draw on top of depth" flag.
    */
    void setOnTopOfDepth(bool bSet) { StyleEntry &ent = styleFor(true, true);
      OdUInt32 flags = GIntAccessor::getIntFlags(ent.edgeStyle()); GIntAccessor::setOnTopOfDepth(flags, bSet); GIntAccessor::installIntFlags(ent.edgeStyle(), flags);
               flags = GIntAccessor::getIntFlags(ent.faceStyle()); GIntAccessor::setOnTopOfDepth(flags, bSet); GIntAccessor::installIntFlags(ent.faceStyle(), flags);
    }
    /** \details
      Returns current state of "draw on top of depth" flag.
    */
    bool isOnTopOfDepth() const { return styleFor(true, true).isOnTopOfDepth(); }

    /** \details
      Setup selection style display scene geometry as is, without any visual effects.
    */
    void setByDefault() { styleFor(false).setByDefault(); styleFor(true).setAsDisabled(); sync3d(); }
    /** \details
      Disable diplaying of scene geometry and all visual effects.
    */
    void setAsDisabled() { styleFor(false).setAsDisabled(); styleFor(true).setAsDisabled(); sync3d(); }
    /** \details
      Setup selection style to display classing stippling pattern highlighting visual effect. Renders original object
      with stippling pattern. Don't render any geometry on top.
    */
    void setForStippling() { styleFor(false).setForStippling(); styleFor(true).setAsDisabled(); sync3d(); }
    /** \details
      Setup selection style for default color masking behavior. Draw semi-transparent faces and edges on top of scene geometry.
      Additionally enables edges width extension and rendering of 3d geometry isolines.
    */
    void setForColorMasking() { styleFor(false).setByDefault(); styleFor(false).edgeStyle().setMode(EdgeStyle::kIsolineEdges);
                                styleFor(true).setForColorMasking(); sync3d(); }
};

#include "TD_PackPop.h"

#endif //__ODGISELECTIONSTYLE_H__
