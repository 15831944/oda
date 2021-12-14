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

#ifndef __ODGISUBENTITYTRAITSDATA_H__
#define __ODGISUBENTITYTRAITSDATA_H__

#include "TD_PackPush.h"

#include "Gi/GiMaterial.h"

struct OdGiDgLinetypeModifiers;

/** \details
    Wraps traits changes flags into importable namespace.
    <group OdGi_Classes> 
*/
struct OdGiSubEntityTraitsChangedFlags
{
  enum ChangedTraits
  {
    /** First changed flag. For internal use. */
    kFirstChangedFlag      = 1,
    /** Color is changed. */
    kColorChanged          = kFirstChangedFlag <<  0,
    /** Layer is changed. */
    kLayerChanged          = kFirstChangedFlag <<  1,
    /** Linetype is changed. */
    kLineTypeChanged       = kFirstChangedFlag <<  2,
    /** Fill type is changed. */
    kFillTypeChanged       = kFirstChangedFlag <<  3,
    /** Lineweight is changed. */
    kLineWeightChanged     = kFirstChangedFlag <<  4,
    /** Linetype scale is changed. */
    kLineTypeScaleChanged  = kFirstChangedFlag <<  5,
    /** Thickness is changed. */
    kThicknessChanged      = kFirstChangedFlag <<  6,
    /** Plot style is changed. */
    kPlotStyleChanged      = kFirstChangedFlag <<  7,
    /** Material is changed. */
    kMaterialChanged       = kFirstChangedFlag <<  8,
    /** Mapper is changed. */
    kMapperChanged         = kFirstChangedFlag <<  9,
    /** Visual style is changed. */
    kVisualStyleChanged    = kFirstChangedFlag << 10,
    /** Transparency is changed. */
    kTransparencyChanged   = kFirstChangedFlag << 11,
    /** Draw flag is changed. */
    kDrawFlagsChanged      = kFirstChangedFlag << 12,
    /** Selection geometry type is changed. */
    kSelectionGeomChanged  = kFirstChangedFlag << 13,
    /** Shadow flags have changed. */
    kShadowFlagsChanged    = kFirstChangedFlag << 14,
    /** The sectionable status has changed. */
    kSectionableChanged    = kFirstChangedFlag << 15,
    /** Selection flags have changed. */
    kSelectionFlagsChanged = kFirstChangedFlag << 16,
    /** Secondary color has changed. */
    kSecColorChanged       = kFirstChangedFlag << 17,
    /** Linetype modifier has changed. */
    kLSModifiersChanged    = kFirstChangedFlag << 18,
    /** Fill has changed. */
    kFillChanged           = kFirstChangedFlag << 19,
    kAwareFlagChanged      = kFirstChangedFlag << 20,
    /** Lock flags have changed. */
    kLockFlagsChanged      = kFirstChangedFlag << 21,
    /** Auxiliary flags have changed. */
    kAuxDataChanged        = kFirstChangedFlag << 22,
    /** All flags have changed. */
    kAllChanged            = 0x7FFFFF, // bin: 11111111111111111111111
    /** Some flags have changed. */
    kSomeChanged           = kAllChanged
  };
};

/** \details

    <group OdGi_Classes> 
*/
class OdGiSubEntityTraitsData
{
  // Members
  //
  OdUInt16                         m_flags;
  OdUInt16                         m_selFlags;
  OdCmEntityColor                  m_cmColor;
  OdDbStub*                        m_layerId;
  OdDbStub*                        m_lineTypeId;
  OdGiFillType                     m_fillType;
  OdDb::LineWeight                 m_lineWeight;
  double                           m_lineTypeScale;
  double                           m_thickness;
  OdDb::PlotStyleNameType          m_plotStyleNameType;
  OdDbStub*                        m_plotStyleNameId;
  OdDbStub*                        m_materialId;
  const OdGiMapper*                m_mapper;
  OdDbStub*                        m_visualStyleId;
  OdCmTransparency                 m_cmTransparency;
  OdUInt32                         m_drawFlags;
  OdUInt32                         m_lockFlags;
  OdGiSubEntityTraits::ShadowFlags m_shadowFlags;
  OdCmEntityColor                  m_cmSecondaryColor;
  const OdGiDgLinetypeModifiers*   m_lsModifiers;
  const OdGiFill*                  m_fill;
  OdGiAuxiliaryDataPtr             m_pAuxData;

public:
  enum
  {
    kSelectionGeom    = 0x100,
    kSectionable      = 0x200,
    // Flags which can be inherited by nested drawables.
    kInheritableSelFlags = kSectionable,
    kSelectionFlagsMask = 0xFF
  };
  enum
  {
    kLayerOff         = 1,
    kLayerFrozen      = 2
  };
  // Geometry visibility flags returned by geomVisibilityFlags method.
  // This is more easy-to-use representation than analysis of selectionGeom and selectionFlags return values.
  enum
  {
    kGeomDisplayNormally    = (1 << 0), // Display unhighlighted geometry
    kGeomSelectNormally     = (1 << 1), // Select unhighlighted geometry 
    kGeomDisplayHighlighted = (1 << 2), // Display highlighted geometry
    kGeomSelectHighlighted  = (1 << 3), // Select highlighted geometry
    kGeomAllVisibilityFlags = 0x0F
  };

  /** \details
    Default constructor for the OdGiSubEntityTraitsData class. Sets the following data members:
    <table>
    Name                  Value                                           Description
    m_flags               0                                               Image width in pixels.
    m_selFlags            0                                               Selection flags.
    m_cmColor             OdCmEntityColor::kForeground                    Subentity color (set to the same color as foreground).
    m_layerId             NULL                                            Layer identifier.
    m_lineTypeId          NULL                                            Linetype identifier.
    m_fillType            kOdGiFillNever                                  Fill type (never fills closed graphic primitives).
    m_lineWeight          OdDb::kLnWt000                                  Lineweight.
    m_lineTypeScale       1.0                                             Linetype scale.
    m_thickness           0.0                                             Subentity thickness.
    m_plotStyleNameType   OdDb::kPlotStyleNameIsDictDefault               Plot style name type (default).
    m_plotStyleNameId     NULL                                            Identifier of the plot style name.
    m_materialId          NULL                                            Material identifier.
    m_mapper              NULL                                            Mapper identifier.
    m_visualStyleId       NULL                                            Visual style identifier.
    m_cmTransparency      OdUInt8(0xFF)                                   Transparency value for subentity.
    m_drawFlags           0                                               Draw flags.
    m_lockFlags           0                                               Lock flags.
    m_shadowFlags         OdGiSubEntityTraits::kShadowsCastAndReceive     Shadow flags (subentities cast and receive shadows).
    m_cmSecondaryColor    OdCmEntityColor::kForeground                    Secondary color.
    m_lsModifiers         NULL                                            Linetype modifier.
    m_fill                NULL                                            The fill object for subentity.
    </table>
  */
  OdGiSubEntityTraitsData()
    : m_flags(0)
    , m_selFlags(0)
    , m_cmColor(OdCmEntityColor::kForeground)
    , m_layerId(NULL)
    , m_lineTypeId(NULL)
    , m_fillType(kOdGiFillNever)
    , m_lineWeight(OdDb::kLnWt000)
    , m_lineTypeScale(1.0)
    , m_thickness(0.0)
    , m_plotStyleNameType(OdDb::kPlotStyleNameIsDictDefault)
    , m_plotStyleNameId(NULL)
    , m_materialId(NULL)
    , m_mapper(NULL)
    , m_visualStyleId(NULL)
    , m_cmTransparency(OdUInt8(0xFF))
    , m_drawFlags(0)
    , m_lockFlags(0)
    , m_shadowFlags(OdGiSubEntityTraits::kShadowsCastAndReceive)
    , m_cmSecondaryColor(OdCmEntityColor::kForeground)
    , m_lsModifiers(NULL)
    , m_fill(NULL)
  {
  }

  // Accessors / transformers
  //
  /** \details
    Retrieves subentity flags.
  */
  OdUInt16 flags() const;
  
  /** \details
    Retrieves selection flags.
  */
  OdUInt16 selFlags() const;
  
  /** \details
    Retrieves visibility flags.
  */
  OdUInt16 geomVisibilityFlags() const;
  
  /** \details
    Gets first two visibility flags if the passed highlighting state is false, or all visibility flags if bHighlighted is true.
    \param bHighlighted [in]  The highlighting state.
  */
  OdUInt16 geomVisibilityFlags(bool bHighlighted) const;
  
  /** \details
    Checks whether layer that contains this subentity is frozen.
    \returns
    true if the layer is frozen, false otherwise.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  bool isLayerFrozen() const;
  
  /** \details
    Checks whether layer that contains this subentity is turned off.
    \returns
    true if the layer is turned off, false otherwise.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  bool isLayerOff() const;
  
  /** \details
    Checks whether layer that contains this subentity is visible.
    \returns
    true if the layer is visible, false otherwise.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  bool isLayerVisible() const;
  
  /** \details
    Checks whether layer that contains this subentity is visible.
    \returns
    true if the layer is visible, false otherwise.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  bool visibility() const { return isLayerVisible(); }
  
  /** \details
    Retrieves subentity true color as a reference to OdCmEntityColor.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  const OdCmEntityColor& trueColor() const;
  
  /** \details
    Retrieves subentity color index.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  OdUInt16 color() const;
  
  /** \details
    Retrieves layer identifier as pointer to OdDbStub.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  OdDbStub* layer() const;
  
  /** \details
    Retrieves linetype identifier as pointer to OdDbStub.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  OdDbStub* lineType() const;
  
  /** \details
    Retrieves subentity fill type.
  */
  OdGiFillType fillType() const;
  
  /** \details
    Retrieves lineweight for this subentity.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  OdDb::LineWeight lineWeight() const;
  
  /** \details
    Retrieves linetype scale for this subentity.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  double lineTypeScale() const;
  
  /** \details
    Retrieves thickness for this subentity.
  */
  double thickness() const;
  
  /** \details
    Retrieves plot style name type for this subentity.
  */
  OdDb::PlotStyleNameType plotStyleNameType() const;
  
  /** \details
    Retrieves plot style name identifier for this subentity.
  */
  OdDbStub* plotStyleNameId() const;
  
  /** \details
    Retrieves material identifier for this subentity.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  OdDbStub* material() const;
  
  /** \details
    Retrieves mapper for this subentity.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  const OdGiMapper* mapper() const;
  
  /** \details
    Retrieves visual style identifier for this subentity.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  OdDbStub* visualStyle() const;
  
  /** \details
    Retrieves transparency for this subentity as reference to OdCmTransparency.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  const OdCmTransparency& transparency() const;
  
  /** \details
    Retrieves the draw flags for this subentity.
  */
  OdUInt32 drawFlags() const;
  
  /** \details
    Retrieves the lock flags for this subentity.
  */
  OdUInt32 lockFlags() const;
  
  /** \details
    Retrieves the selection geometry flag for this subentity.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  bool selectionGeom() const;
  
  /** \details
    Retrieves the shadow flags for this subentity.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  OdGiSubEntityTraits::ShadowFlags shadowFlags() const;
  
  /** \details
    Checks whether this subentity is sectionable.
    \returns
    true if this subentity is sectionable, false otherwise.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  bool sectionable() const;
  
  /** \details
    Retrieves the selection flags for this subentity.
  */
  OdGiSubEntityTraits::SelectionFlags selectionFlags() const;
  
  /** \details
    Retrieves the secondary color for this subentity as a reference to OdCmEntityColor.
  */
  const OdCmEntityColor& secondaryTrueColor() const;
  
  /** \details
    Retrieves the linetype modifiers for this subentity as a pointer to OdGiDgLinetypeModifiers.
  */
  const OdGiDgLinetypeModifiers* lineStyleModifiers() const;
  
  /** \details
    Retrieves the fill for this subentity as a pointer to OdGiFill.
  */
  const OdGiFill* fill() const;
  
  /** \details
    Retrieves a smart pointer to the auxiliary data for this subentity.
  */
  OdGiAuxiliaryDataPtr auxData() const;

  
  
  /** \details
    Sets the subentity flags.
    \param flags [in]  Flags to set.
  */
  void setFlags(OdUInt16 flags);
  
  /** \details
    Sets the selection flags.
    \param selFlags [in]  Selection flags to set.
  */
  void setSelFlags(OdUInt16 selFlags);
  
  /** \details
    Sets true color for this subentity.
    \param trueColor [in]  Color object to set.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  void setTrueColor(const OdCmEntityColor& trueColor);
  
  /** \details
    Sets the color index for this subentity.
    \param trueColor [in]  Color object to set.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  void setColor(OdUInt16 colorIndex);
  
  /** \details
    Sets the layer for this subentity.
    \param layerId [in]  Pointer to the persistent layer identifier.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  void setLayer(OdDbStub* layerId);
  
  /** \details
    Sets the layer for this subentity.
    \param lineTypeId [in]  Pointer to the persistent linetype identifier.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  void setLineType(OdDbStub* lineTypeId);
  
  /** \details
    Sets the fill type for this subentity.
    \param fillType [in]  Pointer to the persistent linetype identifier.
  */
  void setFillType(OdGiFillType fillType);
  
  /** \details
    Sets the lineweight for this subentity.
    \param lineWeight [in]  LineWeight.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  void setLineWeight(OdDb::LineWeight lineWeight);
  
  /** \details
    Sets the linetype scale for this subentity.
    \param lineTypeScale [in]  Linetype scale.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  void setLineTypeScale(double lineTypeScale);
  
  /** \details
    Sets the thickness for this subentity.
    \param thickness [in]  Thickness in drawing units.
  */
  void setThickness(double thickness);
  
  /** \details
    Sets the plot style name and type for this subentity.
    \param plotStyleNameType [in]  Plot style name type.
    \param pPlotStyleNameId [in]  Pointer to the plot style name identifier.
  */
  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId);
  
  /** \details
    Sets the material for this subentity.
    \param materialId [in]  Pointer to the material identifier.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  void setMaterial(OdDbStub* materialId);
  
  /** \details
    Sets the mapper for this subentity.
    \param pMapper [in]  Pointer to the mapper.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  void setMapper(const OdGiMapper* pMapper);
  
  /** \details
    Sets the visual style identifier for this subentity.
    \param visualStyleId [in]  Pointer to the visual style identifier.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  void setVisualStyle(OdDbStub* visualStyleId);
  
  /** \details
    Sets the transparency object for this subentity.
    \param transparency [in]  Transparency data.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  void setTransparency(const OdCmTransparency& transparency);
  
  /** \details
    Sets the draw flags for this subentity.
    \param drawFlags [in]  Draw flags.
  */
  void setDrawFlags(OdUInt32 drawFlags);
  
  /** \details
    Sets the lock flags for this subentity.
    \param lockFlags [in]  Lock flags.
  */
  void setLockFlags(OdUInt32 lockFlags);
  
  /** \details
    Sets the geometry selection flags for this subentity.
    \param bSelectionFlag [in]  Lock flags.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  void setSelectionGeom(bool bSelectionFlag);
  
  /** \details
    Sets the shadow flags for this subentity.
    \param shadowFlags [in]  Shadow flags.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  void setShadowFlags(OdGiSubEntityTraits::ShadowFlags shadowFlags);
  
  /** \details
    Specifies whether the subentity should be sectionable.
    \param bSectionableFlag [in]  Flag that specifies whether this subentity should be sectionable.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  void setSectionable(bool bSectionableFlag);
  
  /** \details
    Sets the selection flags for this subentity.
    \param selectionFlags [in]  Selection flags.
  */
  void setSelectionFlags(OdGiSubEntityTraits::SelectionFlags selectionFlags);
  
  /** \details
    Sets the secondary true color for this subentity.
    \param trueColor [in]  Secondary true color.
  */
  void setSecondaryTrueColor(const OdCmEntityColor& trueColor);
  
  /** \details
    Sets the line style modifiers.
    \param pLSMod [in]  Pointer to the line style modifiers.
  */
  void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod);
  
  /** \details
    Sets the fill for this subentity.
    \param pFill [in]  Pointer to the subentity fill.
  */
  void setFill(const OdGiFill* pFill);
  
  /** \details
    Sets the auxiliary data for this subentity.
    \param pAuxData [in]  Smart pointer to the auxiliary data.
  */
  void setAuxData(const OdGiAuxiliaryDataPtr& pAuxData);
};


inline OdUInt16 OdGiSubEntityTraitsData::flags() const
{
  return m_flags;
}
inline OdUInt16 OdGiSubEntityTraitsData::selFlags() const
{
  return m_selFlags;
}
inline OdUInt16 OdGiSubEntityTraitsData::geomVisibilityFlags() const
{
  OdUInt16 visFlags = kGeomAllVisibilityFlags;
  if (GETBIT(m_selFlags, kSelectionGeom | 
    OdGiSubEntityTraits::kSelectionIgnore | 
    OdGiSubEntityTraits::kHighlightingGeometry |
    OdGiSubEntityTraits::kHiddenInHighlight))
  {
    if (GETBIT(m_selFlags, kSelectionGeom)) visFlags &= ~(kGeomDisplayNormally | kGeomDisplayHighlighted);
    if (GETBIT(m_selFlags, OdGiSubEntityTraits::kSelectionIgnore)) visFlags &= ~(kGeomSelectNormally | kGeomSelectHighlighted);
    if (GETBIT(m_selFlags, OdGiSubEntityTraits::kHighlightingGeometry)) visFlags &= ~kGeomDisplayNormally;
    if (GETBIT(m_selFlags, OdGiSubEntityTraits::kHiddenInHighlight)) visFlags = kGeomDisplayNormally;
  }
  return visFlags;
}
inline OdUInt16 OdGiSubEntityTraitsData::geomVisibilityFlags(bool bHighlighted) const
{ // Returns only first two flags depending onto highlighting state
  const OdUInt16 visFlags = geomVisibilityFlags();
  return (!bHighlighted) ? visFlags : (visFlags >> 2);
}
inline bool OdGiSubEntityTraitsData::isLayerVisible() const
{
  return (m_flags & (kLayerOff|kLayerFrozen))==0;
}
inline bool OdGiSubEntityTraitsData::isLayerFrozen() const
{
  return GETBIT(m_flags, kLayerFrozen);
}
inline bool OdGiSubEntityTraitsData::isLayerOff() const
{
  return GETBIT(m_flags, kLayerOff);
}
inline OdUInt16 OdGiSubEntityTraitsData::color() const
{
  return m_cmColor.colorIndex();
}
inline const OdCmEntityColor& OdGiSubEntityTraitsData::trueColor() const
{
  return m_cmColor;
}
inline OdDbStub* OdGiSubEntityTraitsData::layer() const
{
  return m_layerId;
}
inline OdDbStub* OdGiSubEntityTraitsData::lineType() const
{
  return m_lineTypeId;
}
inline OdGiFillType OdGiSubEntityTraitsData::fillType() const
{
  return m_fillType;
}
inline OdDb::LineWeight OdGiSubEntityTraitsData::lineWeight() const
{
  return m_lineWeight;
}
inline double OdGiSubEntityTraitsData::lineTypeScale() const
{
  return m_lineTypeScale;
}
inline double OdGiSubEntityTraitsData::thickness() const
{
  return m_thickness;
}
inline OdDb::PlotStyleNameType OdGiSubEntityTraitsData::plotStyleNameType() const
{
  return m_plotStyleNameType;
}
inline OdDbStub* OdGiSubEntityTraitsData::plotStyleNameId() const
{
  return m_plotStyleNameId;
}
inline OdDbStub* OdGiSubEntityTraitsData::material() const
{
  return m_materialId;
}
inline const OdGiMapper* OdGiSubEntityTraitsData::mapper() const
{
  return m_mapper;
}
inline OdDbStub* OdGiSubEntityTraitsData::visualStyle() const
{
  return m_visualStyleId;
}
inline const OdCmTransparency& OdGiSubEntityTraitsData::transparency() const
{
  return m_cmTransparency;
}
inline OdUInt32 OdGiSubEntityTraitsData::drawFlags() const
{
  return m_drawFlags;
}
inline OdUInt32 OdGiSubEntityTraitsData::lockFlags() const
{
  return m_lockFlags;
}
inline bool OdGiSubEntityTraitsData::selectionGeom() const
{
  return GETBIT(m_selFlags, kSelectionGeom);
}
inline OdGiSubEntityTraits::ShadowFlags OdGiSubEntityTraitsData::shadowFlags() const
{
  return m_shadowFlags;
}
inline bool OdGiSubEntityTraitsData::sectionable() const
{
  return GETBIT(m_selFlags, kSectionable);
}
inline OdGiSubEntityTraits::SelectionFlags OdGiSubEntityTraitsData::selectionFlags() const
{
  return (OdGiSubEntityTraits::SelectionFlags)(m_selFlags & 0xFF);
}
inline const OdCmEntityColor& OdGiSubEntityTraitsData::secondaryTrueColor() const
{
  return m_cmSecondaryColor;
}
inline const OdGiDgLinetypeModifiers* OdGiSubEntityTraitsData::lineStyleModifiers() const
{
  return m_lsModifiers;
}
inline const OdGiFill* OdGiSubEntityTraitsData::fill() const
{
  return m_fill;
}
inline OdGiAuxiliaryDataPtr OdGiSubEntityTraitsData::auxData() const
{
  return m_pAuxData;
}

inline void OdGiSubEntityTraitsData::setFlags(OdUInt16 flags)
{
  m_flags = flags;
}
inline void OdGiSubEntityTraitsData::setSelFlags(OdUInt16 selFlags)
{
  m_selFlags = selFlags;
}
inline void OdGiSubEntityTraitsData::setTrueColor(const OdCmEntityColor& trueColor)
{
  m_cmColor = trueColor;
}
inline void OdGiSubEntityTraitsData::setColor(OdUInt16 colorIndex)
{
  m_cmColor.setColorIndex(colorIndex);
}
inline void OdGiSubEntityTraitsData::setLayer(OdDbStub* layerId)
{
  // Apply layer overrides

  if( !m_pAuxData.isNull() )
  {
    OdGiLayerOverridesPtr pLayerOverrides = m_pAuxData->getLevelOverrides();

    if( !pLayerOverrides.isNull() )
    {
      OdDbStub* pOverride = pLayerOverrides->getLayerOverride(layerId);

      if( pOverride )
        layerId = pOverride;
    }
  }

  //

  m_layerId = layerId;
}
inline void OdGiSubEntityTraitsData::setLineType(OdDbStub* lineTypeId)
{
  m_lineTypeId = lineTypeId;
}
inline void OdGiSubEntityTraitsData::setFillType(OdGiFillType fillType)
{
  m_fillType = fillType;
}
inline void OdGiSubEntityTraitsData::setLineWeight(OdDb::LineWeight lineWeight)
{
  m_lineWeight = lineWeight;
}
inline void OdGiSubEntityTraitsData::setLineTypeScale(double lineTypeScale)
{
  m_lineTypeScale = lineTypeScale;
}
inline void OdGiSubEntityTraitsData::setThickness(double thickness)
{
  m_thickness = thickness;
}
inline void OdGiSubEntityTraitsData::setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* plotStyleNameId)
{
  m_plotStyleNameType = plotStyleNameType;
  m_plotStyleNameId = plotStyleNameId;
}
inline void OdGiSubEntityTraitsData::setMaterial(OdDbStub* materialId)
{
  m_materialId = materialId;
}
inline void OdGiSubEntityTraitsData::setMapper(const OdGiMapper* pMapper)
{
  m_mapper = pMapper;
}
inline void OdGiSubEntityTraitsData::setVisualStyle(OdDbStub* visualStyleId)
{
  m_visualStyleId = visualStyleId;
}
inline void OdGiSubEntityTraitsData::setTransparency(const OdCmTransparency &transparency)
{
  m_cmTransparency = transparency;
}
inline void OdGiSubEntityTraitsData::setDrawFlags(OdUInt32 drawFlags)
{
  m_drawFlags = drawFlags;
}
inline void OdGiSubEntityTraitsData::setLockFlags(OdUInt32 lockFlags)
{
  m_lockFlags = lockFlags;
}
inline void OdGiSubEntityTraitsData::setSelectionGeom(bool bSelectionFlag)
{
  SETBIT(m_selFlags, kSelectionGeom, bSelectionFlag);
}
inline void OdGiSubEntityTraitsData::setShadowFlags(OdGiSubEntityTraits::ShadowFlags shadowFlags)
{
  m_shadowFlags = shadowFlags;
}
inline void OdGiSubEntityTraitsData::setSectionable(bool bSectionableFlag)
{
  SETBIT(m_selFlags, kSectionable, bSectionableFlag);
}
inline void OdGiSubEntityTraitsData::setSelectionFlags(OdGiSubEntityTraits::SelectionFlags selectionFlags)
{
  m_selFlags = (m_selFlags & (~0xFF)) | (OdUInt16)selectionFlags;
}
inline void OdGiSubEntityTraitsData::setSecondaryTrueColor(const OdCmEntityColor& trueColor)
{
  m_cmSecondaryColor = trueColor;
}
inline void OdGiSubEntityTraitsData::setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod)
{
  m_lsModifiers = pLSMod;
}
inline void OdGiSubEntityTraitsData::setFill(const OdGiFill* pFill)
{
  m_fill = pFill;
}

inline void OdGiSubEntityTraitsData::setAuxData(const OdGiAuxiliaryDataPtr& pAuxData)
{
  m_pAuxData = pAuxData;
}

#include "TD_PackPop.h"

#endif // __ODGISUBENTITYTRAITSDATA_H__
