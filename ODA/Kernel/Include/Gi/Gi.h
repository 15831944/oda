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

#ifndef _OD_GI_H_
#define _OD_GI_H_

#include "RxObject.h"
#include "CmColorBase.h"
#include "GiAuxData.h"

class OdDbStub;
class OdCmEntityColor;
class OdGeVector3d;
class OdGiMapper;
class OdCmTransparency;
struct OdGiDgLinetypeModifiers;
class OdGiFill;
struct OdGiLineweightOverride;
class OdGiPalette;

typedef enum
{
  kOdGiFillAlways = 1,  // Always fill closed graphic primitives.
  kOdGiFillNever  = 2   // Never fill closed graphic primitives.
} OdGiFillType;

typedef enum
{
  kOdGiInvisible      = 0, // Invisible
  kOdGiVisible        = 1, // Visible
  kOdGiSilhouette     = 2  // Silhouette edge
} OdGiVisibility;

#include "TD_PackPush.h"

/** \details
    This class provides an interface to the graphical attributes of graphic
    primitives.
  
    \remarks
    OdGiDrawableTraits is a superset of OdGiSubEntityTraits, and exposes
    additional traits that are consistant for the entire OdGiDrawable.
    
    An OdGiDrawableTraits instance is available during calls to subSetAttributes.
    Graphical attribute settings are used for all graphical primitives until they are changed.
    
    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiDrawableTraits : public OdRxObject
{ 
public:
  ODRX_DECLARE_MEMBERS(OdGiDrawableTraits);
};

/** \details
    This class provides an interface to the graphical attributes of graphic
    primitives.
  
    \remarks
    An OdGiSubEntityTraits instance is available during calls to subWorldDraw and subViewportDraw,
    so that drawable classes can control attributes during the vectorization process. 
    Graphical attribute settings are used for all graphical primitives until they are changed.
    
    The functions in this class are implemented by the OdGiAbstractVectorizer
    class.

    \sa
    TD_Gi
    
    <group OdGi_Classes> 
*/
class FIRSTDLL_EXPORT OdGiSubEntityTraits : public OdGiDrawableTraits
{
public:
  
  /** \details
    Represents valid bitmasks that are passed to the setDrawFlags() method calls.
  */
  enum DrawFlags
  {
    kNoDrawFlags        = 0x0000,   // Default value. No DrawFlags specified.
    kDrawBackfaces      = 0x0001,   // Forces backfaces vectorization.
    kDrawHatchGroup     = 0x0002,   // Next shells and meshes should draw hatches.
    kDrawFrontfacesOnly = 0x0004,   // Forces discard backfaces mode.
    kDrawGradientFill   = 0x0008,   // Draws with gradient fill.
    kDrawSolidFill      = 0x0010,   // Draws with solid fill.
    kDrawNoLineWeight   = 0x0020,   // Disables lineweight.
    kDrawNoPlotstyle    = 0x0080,   // Disables plotstyles.
    // the range before 0x8000 is reserved.
    kDrawContourFill            = 0x08000, // Specific to DGN files.
    kDisableLayoutCache         = 0x10000, // Disable metafile caching for layout switching.
    kDrawBoundaryForClipping    = 0x20000, // Draw (rough) boundary of object to be checked
                                           // when clipping is performed, set by vectorizer.
    kDrawBoundaryForClippingDrw = 0x40000, // Draw (rough) boundary of object to be checked
                                           // when clipping is performed, set by a drawable.
    kClipSetIsEmpty             = 0x80000,
    kDrawPolygonFill            = 0x100000, // Fill in all modes if FillAlways mode enabled.
    kExcludeFromViewExt         = 0x200000, // Don't use extents of the entity when calculating total extents.
    kDrawDgLsPolyWithoutBreaks  = 0x400000, // Draw polylines/polygons with Dgn Line Style without segmentation.
    kPolygonProcessing          = 0x800000, // Polygon filling logic must be applied for shell primitive.
    kDisableAutoGenerateNormals = 0x1000000, // Avoid automatic generation of normals for mesh/shell primitives.
    kDisableFillModeCheck       = 0x2000000, // Avoid checking of OdGiContext::fillMode during filling of geometry primitives.
    kDrawUnderlayReference      = 0x4000000, // Draw underlay reference object.
    kLineStyleScaleOverride     = 0x8000000, // Doesn't multiply current line scale to byBlock line scale. Only for multiplyByBlockLinetypeScales mode.
    kDisableDisplayClipping     = 0x10000000, // Disable display clipping
    kRegenTypeDependent2dDraw   = 0x20000000, // Specific for spamodeler. Draw edges as polyline created by points generated by the faceter
    kInternalDrawFlags = kDisableLayoutCache | kClipSetIsEmpty | kRegenTypeDependent2dDraw,
    // Flags which can be inherited by nested drawables.
    kInheritableDrawFlags = kDrawNoLineWeight | kDrawNoPlotstyle | kDrawBoundaryForClipping | kDrawBoundaryForClippingDrw |
    kInternalDrawFlags | kExcludeFromViewExt | kDrawBackfaces | kDrawFrontfacesOnly | kDisableDisplayClipping | kRegenTypeDependent2dDraw
  };

  /** \details
    Represents bitmask lock flags.
  */
  enum LockFlags
  {
    kNoLockFlags                = 0x00000000,   // No lock flags are used.
    kLockColors                 = 0x00000001,   // Lock colors.
    kLockLineWeight             = 0x00000002,   // Lock lineweight.
    kLockLineType               = 0x00000004,   // Lock linetype.
    kLockLineTypeScale          = 0x00000008,   // Lock linetype scale.
    kLockLayer                  = 0x00000010,   // Lock layer.
    kLockByBlockLayer           = 0x00000020,   // Lock the ByBlock layer.
    kKeepAuxDataAndLockFlags    = 0x00000040,   // Keep auxiliary data and lock flags.
    kInheritableLockFlags       = kLockColors | kLockLineWeight | kLockLineType | kLockLineTypeScale | kLockLayer | kKeepAuxDataAndLockFlags  // Inherited lock flags.
  };

  /** \details
    Represents shadow flags.
  */
  enum ShadowFlags
  {
    kShadowsCastAndReceive = 0,   // Geometry could cast and receive shadows.
    kShadowsDoesNotCast    = 1,   // Geometry doesn't cast shadows.
    kShadowsDoesNotReceive = 2,   // Geometry doesn't receive shadows.
    kShadowsIgnore         = kShadowsDoesNotCast | kShadowsDoesNotReceive // Geometry doesn't cast and receive shadows.
  };

  /** \details
    Represents selection flags.
  */
  enum SelectionFlags
  {
    kNoSelectionFlags     = 0, // No selection flags set.
    kSelectionIgnore      = 1, // Avoid geometry selection.
    kHighlightingGeometry = 2, // Geometry drawn only in case if entire entity highlighted.
    kHiddenInHighlight    = 4  // Geometry hidden only in case if entire entity highlighted. 
  };

  /** \details
    Represents flags that hint which attributes to set.
  */
  enum SubEntityHints
  {
    kHint2DAttributes  = 1,  // Setup attributes only available in 2DWireframe mode (plotStyles and etc.).
    kHint3DAttributes  = 2,  // Setup attributes only available in shaded modes (materials and etc.).
    kHintAllAttributes = kHint2DAttributes | kHint3DAttributes,  // Setup attributes that available in both 2D and 3D modes.
    kHintSnapModeOn  = 4,  // Setup that we currently inside selection for snap method.
    kHintDefaultState  = kHintAllAttributes  // Default setup flag for the attributes.
  };
public:
  ODRX_DECLARE_MEMBERS(OdGiSubEntityTraits);

  /** \details
    Sets the current color index (ACI).

    \param color [in]  Color index.
    
    \remarks
    color must be one of the following:
    
    <table>
    Name              Value   Description
    kACIbyBlock       0       ByBlock.
    kACIforeground    7       Foreground color.
    kACIbyLayer       256     ByLayer. 
    kACIRed           1       Red. 
    kACIYellow        2       Yellow. 
    kACIGreen         3       Green. 
    kACICyan          4       Cyan. 
    kACIBlue          5       Blue. 
    kACIMagenta       6       Magenta. 
    ..                8-255   Defined by AcadPalette.h
    </table>
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  virtual void setColor(
    OdUInt16 color) = 0;

  /** \details
    Sets the current color from an OdCmEntityColor instance.
    
    \param color [in]  OdCmEntityColor object.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  virtual void setTrueColor(
    const OdCmEntityColor& color) = 0;

  /** \details
    Sets the current layer.
    \param layerId [in]  Object ID of the LayerTableRecord.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  virtual void setLayer(
    OdDbStub* layerId) = 0;

  /** \details
    Sets the current linetype.
    \param lineTypeId [in]  Object ID of the LinetypeTableRecord.  
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual void setLineType(
    OdDbStub* lineTypeId) = 0;

  /** \details
    Sets the current selection marker.
    \param marker [in]  Selection marker.
  */
  virtual void setSelectionMarker(
    OdGsMarker selectionMarker) = 0;

  /** \details
    Sets the current fill type.
    \param fillType [in]  Fill type.
    
    \remarks
    fillType must be one of the following:
    
    <table>
    Name              Value   Description
    kOdGiFillAlways   1       Fill closed graphic primitives.
    kOdGiFillNever    2       Don't fill closed graphic primitives.
    </table>
       
    Closed graphic primitives consist of the following:
    * arcs with (OdGiArcType == kOdGiArcSector) || (OdGiArcType == kOdGiArcChord) 
    * circles 
    * meshes 
    * polygons 
    * shells 
  */
  virtual void setFillType(
    OdGiFillType fillType) = 0;

  /** \details
    Sets the current fill plane.
    
    \param pNormal [in]  Pointer to the normal to the plane.
    
    \remarks
    As implemented, this function does nothing but return.
    It will be fully implemented in a future release.
  */
  virtual void setFillPlane(
    const OdGeVector3d* pNormal = 0);

  /** \details
    Sets the current lineweight.

    \param lineWeight [in]  Lineweight.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual void setLineWeight(
    OdDb::LineWeight lineWeight) = 0;

  /** \details
    Sets the current linetype scale.
    \param lineTypeScale [in]  Linetype scale factor.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  virtual void setLineTypeScale(
    double lineTypeScale = 1.0) = 0;

  /** \details
    Sets the current thickness.
    \param thickness [in]  Thickness.
  */
  virtual void setThickness(
    double thickness) = 0;

  /** \details
    Sets the current PlotStyleName.

    \param plotStyleNameType [in]  Plot Style Name Type.
    \param plotStyleNameId [in]  Object ID of the Plot Style Name.
    
    \remarks
    plotStyleNameId is used only when plotStyleNameType == kPlotStyleNameById.
    
    plotStyleNameType must be one of the following:
    
    <table>
    Name                           Value                         
    kPlotStyleNameByLayer          0
    kPlotStyleNameByBlock          1
    kPlotStyleNameIsDictDefault    2
    kPlotStyleNameById             3
    </table>
  */
  virtual void setPlotStyleName(
    OdDb::PlotStyleNameType plotStyleNameType, 
    OdDbStub* plotStyleNameId = 0) = 0;

  /** \details
    Sets the current material.
    \param materialId [in]  Object ID of the material.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  virtual void setMaterial(
    OdDbStub* materialId) = 0;

  /** \details
    Sets the current material mapper.
    \param pMapper [in]  Pointer to the GI mapper object.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  virtual void setMapper(
    const OdGiMapper *pMapper) = 0;

  /** \details
    Sets the current visual style.
    \param visualStyleId [in]  Object ID of the visual style.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual void setVisualStyle(
    const OdDbStub* visualStyleId);

  /** \details
    Sets the current transparency.
    
    \param transparency [in]  OdCmTransparency object.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  virtual void setTransparency(
    const OdCmTransparency &transparency);

  /** \details
    Sets the drawing flags.
    
    \param drawFlags [in]  Input bitmask with DrawFlags.
  */
  virtual void setDrawFlags(
    OdUInt32 drawFlags);

  /** \details
    Sets the lock flags.
    
    \param lockFlags [in]  Input bitmask with LockFlags.
  */
  virtual void setLockFlags(
    OdUInt32 lockFlags);

  /** \details
    Sets selection geometry flag.
    
    \param bSelectionFlag [in]  New selection geometry flag.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  virtual void setSelectionGeom(
    bool bSelectionFlag);

  /** \details
    Sets the shadow flags.
    
    \param shadowFlags [in]  Input ShadowFlags.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  virtual void setShadowFlags(
    ShadowFlags shadowFlags);

  /** \details
    Sets sectionable flag.
    
    \param bSectionableFlag [in]  New sectionable flag.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  virtual void setSectionable(
    bool bSectionableFlag);

  /** \details
    Sets the selection flags.
    
    \param selectionFlags [in]  Input SelectionFlags.
  */
  virtual void setSelectionFlags(
    SelectionFlags selectionFlags);

  /** \details
    Retrieves the current color index (ACI).
    
    \returns
    Color index as OdUInt16 value.
    
    \remarks
    color() returns one of the following:
    
    <table>
    Name              Value   Description
    kACIbyBlock       0       ByBlock.
    kACIforeground    7       Foreground color.
    kACIbyLayer       256     ByLayer. 
    kACIRed           1       Red. 
    kACIYellow        2       Yellow. 
    kACIGreen         3       Green. 
    kACICyan          4       Cyan. 
    kACIBlue          5       Blue. 
    kACIMagenta       6       Magenta. 
    ..                8-255   Defined by AcadPalette.h
    </table>
    
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  virtual OdUInt16 color() const = 0;

  /** \details
    Retrieves the current color.
    
    \returns
    Current color as an OdCmEntityColor instance.
    
    \returns
    Color index as OdUInt16 value.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  virtual OdCmEntityColor trueColor() const = 0;

  /** \details
      Retrieves the current layer.
      
    \returns
    Pointer to the OdDbStub that represents layer identifier. 
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  virtual OdDbStub* layer() const = 0;

  /** \details
      Retrieves the current linetype.
      
    \returns
    Pointer to the OdDbStub that represents linetype identifier.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  virtual OdDbStub* lineType() const = 0;

  /** \details
    Retrieves the current fill type.
      
    \returns
    One of the following values:
    
    <table>
    Name              Value   Description
    kOdGiFillAlways   1       Fill closed graphic primitives.
    kOdGiFillNever    2       Don't fill closed graphic primitives.
    </table>
       
    Closed graphic primitives consist of the following:
    * arcs with (OdGiArcType == kOdGiArcSector) || (OdGiArcType == kOdGiArcChord) 
    * circles 
    * meshes 
    * polygons 
    * shells 
  */
  virtual OdGiFillType fillType() const = 0;

  /** \details
    Retrieves the normal to the current fill plane.
    
    \param normal [out]  Receives the normal.
    
    \returns
    false in the current implementation.
    
    \remarks
    As implemented, this function does nothing but return false.
    It will be fully implemented in a future release.
  */
  virtual bool fillPlane(
    OdGeVector3d& normal);

  /** \details
    Retrieves the current lineweight.
    
    \returns
    Current lineweight as an OdDb::LineWeight value.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  virtual OdDb::LineWeight lineWeight() const = 0;

  /** \details
    Retrieves the current linetype scale.
    
    \returns
    Linetype scale as a value of the double type.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  virtual double lineTypeScale() const = 0;

  /** \details
    Retrieves the current thickness.
    
    \returns
    Current thickness as a value of the double type.
  */
  virtual double thickness() const = 0;

  /** \details
    Retrieves the current PlotStyleName type.
    
    \returns
    One of the following values:
    
    <table>
    Name                           Value                         
    kPlotStyleNameByLayer          0
    kPlotStyleNameByBlock          1
    kPlotStyleNameIsDictDefault    2
    kPlotStyleNameById             3
    </table>
  */
  virtual OdDb::PlotStyleNameType plotStyleNameType() const = 0;

  /** \details
    Retrieves object ID of the the plot style name.
    
    \returns
    Pointer to the OdDbStub that represents a plot style name identifier.
  */
  virtual OdDbStub* plotStyleNameId() const = 0;

  /** \details
    Retrieves the identifier of the material.
    
    \returns
    Pointer to the OdDbStub that represents a material identifier.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  virtual OdDbStub* material() const = 0;
  
  /** \details
    Retrieves pointer to the current material mapper.
    
    \returns
    Pointer to the OdDbStub that represents a mapper identifier.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  virtual const OdGiMapper* mapper() const = 0;

  /** \details
    Retrieves an identifier of the visual style.
    
    \returns
    Pointer to the OdDbStub that represents a visual style identifier.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual OdDbStub* visualStyle() const;

  /** \details
    Retrieves the current transparency.
    
    \returns
    Transparency as an instance of the OdCmTransparency class.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  virtual OdCmTransparency transparency() const;

  /** \details
    Retrieves the current DrawFlags bitmask.
    
    \returns
    Draw bit flags as the OdUInt32 value.
  */
  virtual OdUInt32 drawFlags() const;

  /** \details
    Retrieves the current LockFlags bitmask.
    
    \returns
    Lock bit flags as the OdUInt32 value.
  */
  virtual OdUInt32 lockFlags() const;

  /** \details
    Retrieves the current selection geometry flag.
    
    \returns
    true if the selection geometry flag is used, false otherwise.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  virtual bool selectionGeom() const;

  /** \details
    Retrieves the current ShadowFlags.
    
    \returns
    One of the ShadowFlags enumeration values.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  virtual ShadowFlags shadowFlags() const;

  /** \details
    Retrieves the current sectionable flag.
    
    \returns
    true if geometry is the sectionable flag is used, false otherwise.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  virtual bool sectionable() const;

  /** \details
    Retrieves the current SelectionFlags.
    
    \returns
    One of the SelectionFlags enumeration values.
  */
  virtual SelectionFlags selectionFlags() const;

  // Specific to DGN files

  /** \details
    Sets the current secondary color from an OdCmEntityColor instance.
    
    \param color [in]  OdCmEntityColor object.
  */
  virtual void setSecondaryTrueColor(
    const OdCmEntityColor& color);

  /** \details
    Retrieves the current secondary color.
    
    \returns
    Secondary color as an instance of the OdCmEntityColor class.
  */
  virtual OdCmEntityColor secondaryTrueColor() const;

  /** \details
    Sets Dgn Line Style modifiers.

    \param pLSMod [in]  Pointer to structure with Dgn Line Style modifiers.
  */
  virtual void setLineStyleModifiers(
    const OdGiDgLinetypeModifiers* pLSMod);

  /** \details
    Retrieves pointer to the current Dgn Line Style modifiers structure.
    
    \returns
    pointer to the OdGiDgLinetypeModifiers.
  */
  virtual const OdGiDgLinetypeModifiers* lineStyleModifiers() const;

  // eof Specific to DGN files section

  /** \details
    Sets boundary filling.

    \param pFill [in]  Boundary filling parameters.
  */
  virtual void setFill(
    const OdGiFill* pFill);

  /** \details
    Retrieves boundary filling parameters.
    
    \returns
    pointer to the instance of the OdGiFill class.
  */
  virtual const OdGiFill* fill() const;

  /** \details
  Sets pointer to auxiliary data context.

  \param pAuxData [in]  pointer to auxiliary data context.
  */
  virtual void setAuxData(
    const OdGiAuxiliaryDataPtr& pAuxData);

  /** \details
    Retrieves pointer to auxiliary data context.
    
    \returns
    smart pointer to the auxiliary data context object.
  */
  virtual OdGiAuxiliaryDataPtr auxData() const;

  /** \details
    Pushes lineweight scale and style override into lineweight overrides stack.
    \param pOverride [in]  New lineweight override.
    \returns
    true in case if lineweight overrides supported by underlying vectorization pipeline.
  */
  virtual bool pushLineweightOverride(const OdGiLineweightOverride* pOverride);

  /** \details
    Pops lineweight scale and style override from lineweight overrides stack.
  */
  virtual void popLineweightOverride();

  /** \details
    Push palette override into palette overrides stack.
    \param pOverride [in]  New palette override.
    \returns Returns true in case if palette overrides supported by underlying vectorization pipeline.
  */
  virtual bool pushPaletteOverride(const OdGiPalette* pOverride);

  /** \details
    Pop palette override from palette overrides stack.
  */
  virtual void popPaletteOverride();

  /** \details
    Retrieves the set of SubEntityHints flags which can be used to provide more flexible tuning of
    subSetAttributes traits processing.
    
    \returns
    SubEntityHints flags as the OdUInt32 value.
  */
  virtual OdUInt32 setupForEntity() const;

  /** \details
    Adds a light object to the model.

    \param lightId [in]  Object ID of the light.

    \remarks
    Through this interface, drawables that emit light can add light objects to the model.
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  virtual void addLight(
    const OdDbStub* lightId);
};

/** \details
  A data type that represents a smart pointer to an <link OdGiSubEntityTraits, OdGiSubEntityTraits> object.
*/
typedef OdSmartPtr<OdGiSubEntityTraits> OdGiSubEntityTraitsPtr;

#include "TD_PackPop.h"

#endif //_OD_GI_H_
