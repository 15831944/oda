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

// GiBaseVectorizer.h: interface for the OdGiBaseVectorizer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _ODGIBASEVECTORIZER_INCLUDED_
#define _ODGIBASEVECTORIZER_INCLUDED_

#include "Gi/GiDrawImpl.h"
#include "Gs/Gs.h"
#include "Ge/GeExtents3d.h"
#include "Gi/GiViewport.h"
#include "Gi/GiXform.h"
#include "Gi/GiModelToViewProc.h"
#include "Gi/GiConveyorEntryPoint.h"
#include "Gi/GiConveyorConnector.h"
#include "Gi/GiDeviation.h"
#include "Ge/GePlane.h"
#include "Gi/GiDgLinetype.h"
#include "Gi/GiOrthoClipper.h"
#include "Gi/GiExtAccum.h"
#include "Gi/GiSubEntityTraitsData.h"
#include "Gi/GiTextStyle.h"
#include "Gi/GiLayerTraitsData.h"
#include "Gi/GiMaterialTraitsData.h"
#include "Gi/GiTraitsCache.h"
#include "Gi/GiDummyGeometry.h"

class OdGiVisualStyle;

class OdGiBaseVectorizer;

class OdDbStub;

typedef OdSmartPtr<OdGiFill> OdGiFillPtr;

#include "TD_PackPush.h"

// Enable traits optimization
#define ENABLE_GITRAITSOPTIMIZATION

/** \details
    This class provides an implementation of the OdGiWorldDraw::geometry() function.

    \sa
    TD_Gi

    <group OdGi_Classes>
*/
class ODGI_EXPORT ODRX_ABSTRACT OdGiWorldDrawImpl : public OdGiDummyCommonDraw<OdGiWorldDraw_>
                                                  , public OdGiSubEntityTraits
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdGiDummyCommonDraw<OdGiWorldDraw_>);

  OdGiSubEntityTraitsData m_entityTraitsData;
  OdGiContext*            m_pContext;

  OdGiWorldDrawImpl();
public:
  /** \details
    Sets the OdGiContext instance associated with this object.

    \param pUserContext [in]  Pointer to the user context.
  */
  virtual void setContext(OdGiContext* pUserContext);
  
  /** \details
    Retrieves the OdGiContext instance associated with this object.

    \param
    pointer to the context associated with this object.
  */
  static OdGiContext* dummyGiContext();

  /** \details
    Retrieves entity color index (ACI).

    \returns
    Entity color index as OdUInt16 value.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  OdUInt16 color() const;
  
  /** \details
    Retrieves entity color.

    \returns
    Entity color as OdCmEntityColor instance.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  OdCmEntityColor trueColor() const;
  
  /** \details
      Retrieves the current layer.
      
    \returns
    Pointer to the OdDbStub that represents layer identifier.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  OdDbStub* layer() const;
  
  /** \details
      Retrieves the current linetype.
      
    \returns
    Pointer to the OdDbStub that represents linetype identifier. 
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  OdDbStub* lineType() const;
  
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
  OdGiFillType fillType() const;
  
  /** \details
    Retrieves the current lineweight.
    
    \returns
    Current lineweight as an OdDb::LineWeight value.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  OdDb::LineWeight lineWeight() const;
  
  /** \details
    Retrieves the current linetype scale.
    
    \returns
    Linetype scale as a value of the double type.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  double lineTypeScale() const;
  
  /** \details
    Retrieves the current thickness.
    
    \returns
    Current thickness as a value of the double type.
  */
  double thickness() const;
  
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
  OdDb::PlotStyleNameType plotStyleNameType() const;
  
  /** \details
    Retrieves object ID of the the plot style name.
    
    \returns
    Pointer to the OdDbStub that represents a plot style name identifier.
  */
  OdDbStub* plotStyleNameId() const;
  
  /** \details
    Retrieves the identifier of the material.
    
    \returns
    Pointer to the OdDbStub that represents a material identifier.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  OdDbStub* material() const;
  
  /** \details
    Retrieves pointer to the current material mapper.
    
    \returns
    Pointer to the OdDbStub that represents a mapper identifier.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  const OdGiMapper* mapper() const;
  
  /** \details
    Retrieves an identifier of the visual style.
    
    \returns
    Pointer to the OdDbStub that represents a visual style identifier.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  OdDbStub* visualStyle() const;
  
  /** \details
    Retrieves the current transparency.
    
    \returns
    Transparency as an instance of the OdCmTransparency class.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  OdCmTransparency transparency() const;
  
  /** \details
    Retrieves the current DrawFlags bitmask.
    
    \returns
    Draw bit flags as the OdUInt32 value.
  */
  OdUInt32 drawFlags() const;
  
  /** \details
    Retrieves the current LockFlags bitmask.
    
    \returns
    Lock bit flags as the OdUInt32 value.
  */
  OdUInt32 lockFlags() const;
  
  /** \details
    Retrieves the current selection geometry flag.
    
    \returns
    true if the selection geometry flag is used, false otherwise.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  bool selectionGeom() const;
  
  /** \details
    Retrieves the current ShadowFlags.
    
    \returns
    One of the ShadowFlags enumeration values.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  ShadowFlags shadowFlags() const;
  
  /** \details
    Retrieves the current sectionable flag.
    
    \returns
    true if geometry is the sectionable flag is used, false otherwise.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  bool sectionable() const;
  
  /** \details
    Retrieves the current SelectionFlags.
    
    \returns
    One of the SelectionFlags enumeration values.
  */
  SelectionFlags selectionFlags() const;
  
  /** \details
    Sets the current secondary color from an OdCmEntityColor instance.
    
    \param color [in]  OdCmEntityColor object.
  */
  OdCmEntityColor secondaryTrueColor() const;
  
  /** \details
    Retrieves pointer to the current Dgn Line Style modifiers structure.
    
    \returns
    pointer to the OdGiDgLinetypeModifiers.
  */
  const OdGiDgLinetypeModifiers* lineStyleModifiers() const;
  
  /** \details
    Retrieves boundary filling parameters.
    
    \returns
    pointer to the instance of the OdGiFill class.
  */
  const OdGiFill* fill() const;
  
  /** \details
    Retrieves pointer to auxiliary data context.
    
    \returns
    smart pointer to the auxiliary data context object.
  */
  OdGiAuxiliaryDataPtr auxData() const;

  
  /** \details
    Sets the current color from an OdCmEntityColor instance.
    
    \param color [in]  OdCmEntityColor object.
  */
  void setTrueColor(const OdCmEntityColor& color);
  
  /** \details
    Sets the current PlotStyleName.

    \param plotStyleNameType [in]  Plot Style Name Type.
    \param pPlotStyleNameId [in]  Object ID of the Plot Style Name.
    
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
  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId = 0);
  
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
  void setColor(OdUInt16 color);
  
  /** \details
    Sets the current layer.
    \param layerId [in]  Object ID of the LayerTableRecord.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  void setLayer(OdDbStub* layerId);
  
  /** \details
    Sets the current linetype.
    \param lineTypeId [in]  Object ID of the LinetypeTableRecord.  
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  void setLineType(OdDbStub* lineTypeId);
  
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
  void setFillType(OdGiFillType fillType);
  
  /** \details
    Sets the current lineweight.

    \param lineWeight [in]  Lineweight.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  void setLineWeight(OdDb::LineWeight lineWeight);
  
  /** \details
    Sets the current linetype scale.
    \param lineTypeScale [in]  Linetype scale factor.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  void setLineTypeScale(double lineTypeScale);
  
  /** \details
    Sets the current thickness.
    \param thickness [in]  Thickness.
  */
  void setThickness(double thickness);
  
  /** \details
    Sets the current selection marker.
    \param selectionMarker [in]  Selection marker.
  */
  void setSelectionMarker(OdGsMarker selectionMarker);
  
  /** \details
    Sets the current material.
    \param materialId [in]  Object ID of the material.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  void setMaterial(OdDbStub* pMaterialId);
  
  /** \details
    Sets the current material mapper.
    \param pMapper [in]  Pointer to the GI mapper object.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  void setMapper(const OdGiMapper *pMapper);
  
  /** \details
    Sets the current visual style.
    \param visualStyleId [in]  Object ID of the visual style.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  void setVisualStyle(const OdDbStub* visualStyleId);
  
  /** \details
    Sets the current transparency.
    
    \param transparency [in]  OdCmTransparency object.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  void setTransparency(const OdCmTransparency &transparency);
  
  /** \details
    Sets the drawing flags.
    
    \param drawFlags [in]  Input bitmask with DrawFlags.
  */
  void setDrawFlags(OdUInt32 drawFlags);
  
  /** \details
    Sets the lock flags.
    
    \param lockFlags [in]  Input bitmask with LockFlags.
  */
  void setLockFlags(OdUInt32 lockFlags);
  
  /** \details
    Sets selection geometry flag.
    
    \param bSelectionFlag [in]  New selection geometry flag.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  void setSelectionGeom(bool bSelectionFlag);
  
  /** \details
    Sets the shadow flags.
    
    \param shadowFlags [in]  Input ShadowFlags.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  void setShadowFlags(ShadowFlags shadowFlags);
  
  /** \details
    Sets sectionable flag.
    
    \param bSectionableFlag [in]  New sectionable flag.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  void setSectionable(bool bSectionableFlag);
  
  /** \details
    Sets the selection flags.
    
    \param selectionFlags [in]  Input SelectionFlags.
  */
  void setSelectionFlags(SelectionFlags selectionFlags);
  
  /** \details
    Sets the current secondary color from an OdCmEntityColor instance.
    
    \param color [in]  OdCmEntityColor object.
  */
  void setSecondaryTrueColor(const OdCmEntityColor& color);
  
  /** \details
    Sets Dgn Line Style modifiers.

    \param pLSMod [in]  Pointer to structure with Dgn Line Style modifiers.
  */
  void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod);
  
  /** \details
    Sets boundary filling.

    \param pFill [in]  Boundary filling parameters.
  */
  void setFill(const OdGiFill* pFill);
  
  /** \details
  Sets pointer to auxiliary data context.

  \param pAuxData [in]  pointer to auxiliary data context.
  */
  void setAuxData(const OdGiAuxiliaryDataPtr& pAuxData);

  /////////////////////////////////////////////////////////////////////////////
  // OdGiCommonDraw Overrides

  /** \details
    Retrieves context associated with this object.
    
    \returns
    Pointer to the associated context.
  */
  OdGiContext* context() const;

  /** \details
    Retrieves sub-entity traits associated with this object.
    
    \returns
    sub-entity traits. 
    \remarks
    The subEntityTraits object gives control of, and access to the attribute (color, layer, linetype, etc.) settings of the current geometry. For more information, see the <link OdGiSubEntityTraits, OdGiSubEntityTraits> documentation. 
  */
  OdGiSubEntityTraits& subEntityTraits() const;

  /** \details
    Retrieves raw geometry associated with this object.
    
    \returns
    raw geometry.
    \remarks
    The method cannot return NULL.
  */
  OdGiGeometry& rawGeometry() const;

  /** \details
    Retrieves count of isolines that are used to represent an entity.
    
    \returns
    number of isolines.
    \remarks
    Isolines are lines or curves which are used to represent the entity. 
    For example, if an entity is a sphere and this method returns 3, the 
    sphere should be displayed as 3 circles with a common center and planes that are
    perpendicular to each other.
  */
  OdUInt32 numberOfIsolines() const;
};

/** \details
    This class is a base implementation of the OdGiWorldDraw and OdGiViewportDraw interfaces.
    Corresponding C++ library: TD_Gi    
  
    \remarks
    This class takes a drawable graphics and passes it into OdGiConveyorGeometry.
    
    It is the base class for specialized frameworks such as OdGiExtCalc.
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiBaseVectorizer : public OdGiWorldDrawImpl
                                     , public OdGiViewportDraw_
                                     , public OdGiConveyorContext
                                     , public OdGiDeviation 
                                     , public OdGiSubEntityTraitsChangedFlags
{
protected:
  ODRX_HEAP_OPERATORS();

  mutable OdGiSubEntityTraitsData m_effectiveEntityTraitsData;

  ODGI_EXPORT_STATIC static const OdGiSubEntityTraitsData g_entityTraitsDataReset;

  mutable OdDbStub*               m_layerId;
  mutable OdGiLayerTraitsTaker    m_effectiveLayerTraits;
  struct LayerTraitsCache : public OdGiLayerTraitsData
  { OdDbStub* m_effectiveLayerId;
    LayerTraitsCache() : m_effectiveLayerId(NULL) {} };
  typedef std::map<OdDbStub*, LayerTraitsCache> LayerTraitsCacheMap;
  mutable LayerTraitsCacheMap m_layerTraitsCache;

  typedef OdGiTraitsCache<OdGiMaterialTraitsData, OdGiMaterialTraitsTaker, OdGiMaterialTraits::kByBlock | OdGiMaterialTraits::kByLayer> OdGiMaterialTraitsCache;
  mutable OdDbStub*               m_materialId;
  mutable OdDbStub*               m_byLayerMaterialId;
  mutable OdDbStub*               m_byBlockMaterialId;
  mutable OdUInt32                m_materialAttributes;
  mutable OdGiMaterialTraitsCache m_effectiveMaterialTraitsData;
  mutable OdDbStub*               m_effectiveMaterialId;
  void actualizeMaterialAttributes(OdDbStub* materialId) const;

  virtual OdDbStub*               switchLayer(OdDbStub* layerId) const;
  virtual const OdGiLayerTraitsData& effectiveLayerTraits() const;
  mutable OdDbStub*               m_effectiveLayerId;

  OdGsMarker                      m_nSelectionMarker;
  OdGsMarker                      m_nPrevSelectionMarker;

  OdGiMapper                      m_materialMapper;
  OdGiMapper*                     m_pCurMaterialMapper;

  OdGiDgLinetypeModifiers         m_lsModifiers;
  OdGiDgLinetypeModifiers*        m_pCurLSModifiers;

  OdGiFillPtr                     m_pFill;
  OdGiAuxiliaryDataPtr            m_pAuxData;

  virtual bool layerVisible(OdDbStub* layerId) const;
  /** \details
    Updates the layer traits for this Vectorizer object from the specified subentity traits.
    \param subEntityTraits [in]  Subentity traits.
  */
  void updateLayerTraits(OdGiSubEntityTraitsData& subEntityTraits) const;
  
public:
  /** \details
    Checks whether EntityTraitsData for this Vectorizer object have been changed.
    
    \param bit [in]  Bit that represents an entity traits to check.
    
    \returns
    true if the EntityTraitsData for this Vectorizer object have been changed.
  */
  bool isEntityTraitsDataChanged(int bit = kSomeChanged) const { return GETBIT(m_entityTraitsDataChanged, bit); }

  /** \details
    Marks as changed the EntityTraitsData data for this Vectorizer object.
  */
  virtual void setEntityTraitsDataChanged() { m_entityTraitsDataChanged = kAllChanged; }
  /** \details
    Marks the specfied trait as changed (or not) the EntityTraitsData data for this Vectorizer object.
    
    \param bit [in]  Bit that represents an entity traits to set.
    \param value [in]  Flag that specifies whether trait should be set to the state "changed" or "unchanged".
  */
  virtual void setEntityTraitsDataChanged(int bit, bool value = true) { SETBIT(m_entityTraitsDataChanged, bit, value); }
  
  /** \details
    Marks as unchanged the EntityTraitsData data for this Vectorizer object.
  */
  void clearEntityTraitsDataChanged() const { m_entityTraitsDataChanged = 0; }

  /** \details
    Checks whether the layer of this Vectorizer object is effectively visible.
    
    \remarks
    true if the following expression succeeds:
    
                    (GETBIT(m_flags, kDrawLayerOff   ) || !traits.isLayerOff()) &&
                    (GETBIT(m_flags, kDrawLayerFrozen) || !traits.isLayerFrozen())
  */
  bool effectivelyVisible() const
  {
    const OdGiSubEntityTraitsData& traits = effectiveTraits();
    return (
      (GETBIT(m_flags, kDrawLayerOff   ) || !traits.isLayerOff()) &&
      (GETBIT(m_flags, kDrawLayerFrozen) || !traits.isLayerFrozen())
    );
  }  

public:
  /////////////////////////////////////////////////////////////////////////////
  // OdGiConveyorContext Overrides

  /** \details
    Retrieves context associated with this object.
    
    \returns
    Pointer to the associated context.
  */
  OdGiContext& giContext() const;
  
  /** \details
    Retrieves description for a drawable associated with this object.
    
    \returns
    Pointer to the OdGiDrawableDesc object that represents a description of a drawable.
  */
  const OdGiDrawableDesc* currentDrawableDesc() const;
  
  /** \details
    Retrieves current drawable object.
    
    \returns
    Pointer to the current drawable.
  */
  const OdGiDrawable* currentDrawable() const;
  
  /** \details
    Retrieves gi viewport associated with this object.
    
    \returns
    Pointer to the OdGiViewport object.
  */
  const OdGiViewport* giViewport() const;
  
  /** \details
    Retrieves gs view associated with this object.
    
    \returns
    Pointer to the OdGsView object.
    \sa
    <link tv_working_with_base_classes_view, Work with Visualize Graphical Views>
  */
  const OdGsView* gsView() const;
  
  /////////////////////////////////////////////////////////////////////////////

private:
  // Data Members

  OdGiTextStyle                   m_textStyle;
  OdGePlane                       m_primitivePlane;
  OdGeVector3d                    m_extrusion;
  OdGiXformPtr                    m_pDcInputToOutput[4]; // WCS->ECS, ECS->Output, NDC->ECS, DCS->NDC
  OdGiConveyorEntryPoint          m_dcEntryPoint[4]; // WCS, ECS, NDC, DCS
  bool                            m_dcTransformUpdate[4]; // ECS->Output is always actual
  OdGiContext::CoordinatesSystem updateDcXform(OdGiContext::CoordinatesSystem cs);
#ifdef ENABLE_GITRAITSOPTIMIZATION
  bool                            m_bResetEntityTraitsData; //reset them before setAttributes()
#endif
protected:
  OdRxObjectPtr                   m_pGiReserved;
  mutable OdUInt32                m_entityTraitsDataChanged;
  OdGiConveyorConnector           m_eyeEntryPoint;

  /** \details
    Retrieves an extrusion vector normal to the specified plane.
    
    \param firstPoint [in]  First point in the plane.
    \param secondPoint [in]  Second point in the plane.
    \param thirdPoint [in]  Third point in the plane.
    \param origin [in]  Origin of the plane.
    \param u [in]  U-vector in the plane.
    \param v [in]  V-vector in the plane.
    \param normal [in]  Normal to the plane.
    \param pNormal [in]  Pointer to the normal to the plane.
    \remarks
    The magnitude of the extrusion vector is effectiveTraits().thickness().
  */
  const OdGeVector3d* extrusion(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint);
  const OdGeVector3d* extrusion(const OdGeVector3d& normal);
  const OdGeVector3d* extrusion(const OdGeVector3d* pNormal)
  {
    if(pNormal)
      return extrusion(*pNormal);
    return 0;
  }
  const OdGeVector3d* extrusion(const OdGePoint3d& origin, const OdGeVector3d& u, const OdGeVector3d& v);

  OdGiConveyorConnector           m_modelEntryPoint;
  OdGiConveyorConnector*          m_pActiveEntryPoint;

  OdGiModelToViewProcPtr          m_pModelToEyeProc;
  OdGiConveyorConnector           m_output;

public:
  /** Drawable description */
  OdGiDrawableDesc*               m_pDrawableDesc;
  /** Drawable attributes */
  OdUInt32                        m_nDrawableAttributes;
  
  /** \details
    Retrieves drawable description.
    
    \returns
    pointer to the OdGiDrawableDesc object.
  */
  const OdGiDrawableDesc *drawableDesc() const { return m_pDrawableDesc; }
protected:

  enum Flags
  {
    kFirstFlag            = 1,

    kSuppressViewportDraw = kFirstFlag << 0,
    kDrawInvisibleEnts    = kFirstFlag << 1,
    kDrawLayerOff         = kFirstFlag << 2,
    kDrawLayerFrozen      = kFirstFlag << 3,
    kIgnoreFillPlane      = kFirstFlag << 4,
    kSelectionMode        = kFirstFlag << 5,
    kAttributesSegment    = kFirstFlag << 6,
    kXrefOverride         = kFirstFlag << 7,

    kLastFlag             = kXrefOverride
  };
  mutable OdUInt32                m_flags;
  OdUInt32                        m_drawContextFlags;

  const OdGiSubEntityTraitsData*  m_pByBlock;

  /** \details
    Sets the specfied EntityTraitsData data from the specified EntityTraitsData.
    
    \param pSource [in]  Pointer to the source SubEntityTraitsData.
    \param destination [out]  Receives the SubEntityTraitsData.
    
    \remarks
    If pSource is NULL, default SubEntityTraitsData is used.
    
  */
  virtual void affectTraits(const OdGiSubEntityTraitsData* pSource, OdGiSubEntityTraitsData& destination) const;

  /** \details
    Retrieves the dash length below which linetypes are not generated by this Vectorizer object.
  */
  virtual double linetypeGenerationCriteria() const;

#ifdef ENABLE_GITRAITSOPTIMIZATION
  inline void resetEntityTraitsDataKeepAuxData();
  inline void resetEntityTraitsData();
  inline void updateTraitsDataByBlockTraits(const OdGiSubEntityTraitsData* pBlockTraits);
#endif
public:
  /////////////////////////////////////////////////////////////////////////////
  // OdGiConveyorContext Override

  /** \details
    Retrieves the EntityTraitsData in effect for this Vectorization object.
    
    \returns
    entity traits that are in effect for this vectorizer.
  */
  virtual const OdGiSubEntityTraitsData& effectiveTraits() const;

  /** \details
    Sets entity traits that should be in effect for this Vectorization object.
    
    \param traits [in]  Entity traits to set.
    \param fillNormal [in]  Normal for a fill plane. Reserves for future use.
  */
  virtual void setEffectiveTraits(const OdGiSubEntityTraitsData& traits,
                                  const OdGeVector3d* fillNormal = NULL);

  /////////////////////////////////////////////////////////////////////////////
protected:

  /** \details
    Retrieves the a pointer to the OdRxClass object that contains a description
    of this object type, for objects of known class type.

    \remarks
    This function is for use only when the class type of this object is known.
    If the class type of this object is unknown, use instead isA().
  */
  static OdRxClass* desc() { return ::OdRxObject::desc(); }

  /** \details
    Called when selection marker is being to be changed.

    \param nSelectionMarker [in]  New selection marker.

    \remarks
    Previous selection marker accessible through selectionMarker method inside override.
    This method should be called for base class from override to change selection marker.
  */
  virtual void selectionMarkerOnChange(OdGsMarker nSelectionMarker);

  virtual bool updateExtentsOnly() const { return false; }

public:
  
  /** \details
    Default constructor for the OdGiBaseVectorizer class. Initializes data members in the member initializer list:

    <table>
    Data member                       Value   
    m_layerId                         0       
    m_materialId                      NULL    
    m_byLayerMaterialId               NULL    
    m_byBlockMaterialId               NULL    
    m_materialAttributes              OdGiMaterialTraitsCache::kCacheByMask
    m_effectiveMaterialTraitsData     0
    m_effectiveMaterialId             0
    m_effectiveLayerId                5
    m_nSelectionMarker                kNullSubentIndex
    m_nPrevSelectionMarker            kNullSubentIndex
    m_pDrawableDesc                   0
    m_nDrawableAttributes             0
    m_flags                           kSuppressViewportDraw
    m_drawContextFlags                0
    m_pByBlock                        0
    m_bResetEntityTraitsData*         true
    </table>
    
    Additionally some other data members are set in the constructor body.
  */
  OdGiBaseVectorizer();
  
  /** \details
    Destructor for the OdGiBaseVectorizer class.
  */
  virtual ~OdGiBaseVectorizer();

  /** \details
    Retrieves the a pointer to the OdRxClass object that contains a description
    of this object type, for objects of unknown class type.
    
    \returns
    pointer to the OdRxClass object.
    
    \remarks
    This function may be used only when the class type of this object is unknown. 
    If the class type of this object is known, use instead desc().
    \sa
    <exref target="https://docs.opendesign.com/tkernel/rtti.html">Run-Time Type Identification</exref>
  */
  OdRxClass* isA() const;

  /** \details
     Retrieves the Protocol Extension object for this object.
     
     \returns
     pointer to the OdRxClass object.
     
     \remarks
     Returns NULL if no protocol extension class exists.
     \sa
    <exref target="https://docs.opendesign.com/tkernel/rtti.html">Run-Time Type Identification</exref>
  */
  OdRxObject* queryX(const OdRxClass* pProtocolClass) const;

  /** \details
    Retrieves a reference to the output of this Vectorizer object.
    
    \returns
    reference to the output.
  */
  virtual OdGiConveyorOutput& output();

  /** \details
    Sets the Eye to Output transformation matrix for this Vectorizer object.
    \param xfm [in]  Transformation matrix.
  */
  void setEyeToOutputTransform(const OdGeMatrix3d& xfm);

  /** \details
    Retrieves the Eye to Output transformation matrix for this Vectorizer object.
    
    \returns
    transformation matrix.
  */
  const OdGeMatrix3d& eyeToOutputTransform() const;
  
  /** \details
    Retrieves the Eye to Output transformation matrix for this Vectorizer object.
    \param xfm [out]  Receives the transformation matrix.
  */
  void eyeToOutputTransform(OdGeMatrix3d& xfm) const;

  /** \details
    Retrieves the OdGiConveyorContext associated with this object.

    \returns
    "this"
    
    \remarks
    Since OdGiConveyorContext is a parent class, this function always returns "this".
  */
  OdGiConveyorContext* drawContext() { return this; }

  /** \details
    Retrieves the OdGiConveyorContext associated with this object.

    \returns
    "this"
    
    \remarks
    Since OdGiConveyorContext is a parent class, this function always returns "this".
  */
  const OdGiConveyorContext* drawContext() const { return this; }

  /** \details
    Begins the ViewVectorization process for this Vectorizer object.
  */
  virtual void beginViewVectorization();

  /** \details
    Ends the ViewVectorization process for this Vectorizer object.
  */
  virtual void endViewVectorization();

  /////////////////////////////////////////////////////////////////////////////
  // OdGiSubEntityTraits Overrides

  /** \details
    Sets the current color from an OdCmEntityColor instance.
    
    \param color [in]  OdCmEntityColor object.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  void setTrueColor(const OdCmEntityColor& trueColor);
  /** \details
    Sets the Plot Style Name Type for this Vectorizer object.
    \param plotStyleNameType [in]  Plot Style Name Type.
    
    \remarks
    plotStyleNameType must be one of the following:
    
    <table>
    Name                                 Value                         
    OdDb::kPlotStyleNameByLayer          0
    OdDb::kPlotStyleNameByBlock          1
    OdDb::kPlotStyleNameIsDictDefault    2
    OdDb::kPlotStyleNameById             3
    </table>
  */
  void setPlotStyleNameType(OdDb::PlotStyleNameType plotStyleNameType);
  /** \details
    Sets the PlotStyleName for this Vectorizer object.
    \param plotStyleNameId [in]  Pointer to the Object ID of the plot style name.
  */
  void setPlotStyleNameId(OdDbStub* plotStyleNameId);
  
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
  void setColor(OdUInt16 color);
  
  /** \details
    Sets the current layer.
    \param layerId [in]  Object ID of the LayerTableRecord.  
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  void setLayer(OdDbStub* layerId);
  
  /** \details
    Sets the current linetype.
    \param lineTypeId [in]  Object ID of the LinetypeTableRecord. 
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>    
  */
  void setLineType(OdDbStub* lineTypeId);
  
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
  void setFillType(OdGiFillType fillType);
  
  /** \details
    Sets the current lineweight.

    \param lineWeight [in]  Lineweight.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  void setLineWeight(OdDb::LineWeight lineWeight);
  
  /** \details
    Sets the current linetype scale.
    \param lineTypeScale [in]  Linetype scale factor.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  void setLineTypeScale(double lineTypeScale);
  
  /** \details
    Sets the current thickness.
    \param thickness [in]  Thickness.
  */
  void setThickness(double thickness);

  /** \details
    Sets the current PlotStyleName.

    \param plotStyleNameType [in]  Plot Style Name Type.
    \param pPlotStyleNameId [in]  Object ID of the Plot Style Name.
    
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
  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId= 0);
  
  /** \details
    Sets the current selection marker.
    \param selectionMarker [in]  Selection marker.
  */
  void setSelectionMarker(OdGsMarker selectionMarker);
  
  /** \details
    Sets the current material.
    \param materialId [in]  Object ID of the material.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  void setMaterial(OdDbStub* pMaterialId);
  
  /** \details
    Sets the current material mapper.
    \param pMapper [in]  Pointer to the GI mapper object.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  void setMapper(const OdGiMapper *pMapper);
  
  /** \details
    Sets the current visual style.
    \param visualStyleId [in]  Object ID of the visual style.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  void setVisualStyle(const OdDbStub* pVisualStyleId);
  
  /** \details
    Sets the current transparency.
    
    \param transparency [in]  OdCmTransparency object.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  void setTransparency(const OdCmTransparency& transparency);
  
  /** \details
    Sets the drawing flags.
    
    \param drawFlags [in]  Input bitmask with DrawFlags.
  */
  void setDrawFlags(OdUInt32 drawFlags);
  
  /** \details
    Sets the lock flags.
    
    \param lockFlags [in]  Input bitmask with LockFlags.
  */
  void setLockFlags(OdUInt32 lockFlags);
  
  /** \details
    Sets selection geometry flag.
    
    \param bSelectionFlag [in]  New selection geometry flag.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  void setSelectionGeom(bool bSelectionFlag);
  
  /** \details
    Sets the shadow flags.
    
    \param shadowFlags [in]  Input ShadowFlags.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  void setShadowFlags(ShadowFlags shadowFlags);
  
  /** \details
    Sets sectionable flag.
    
    \param bSectionableFlag [in]  New sectionable flag.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  void setSectionable(bool bSectionableFlag);
  
  /** \details
    Sets the selection flags.
    
    \param selectionFlags [in]  Input SelectionFlags.
  */
  void setSelectionFlags(SelectionFlags selectionFlags);

  /** \details
    Sets the current secondary color from an OdCmEntityColor instance.
    
    \param color [in]  OdCmEntityColor object.
  */
  void setSecondaryTrueColor(const OdCmEntityColor& color);
  
  /** \details
    Sets Dgn Line Style modifiers.

    \param pLSMod [in]  Pointer to structure with Dgn Line Style modifiers.
  */
  void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod);

  /** \details
    Sets boundary filling.

    \param pFill [in]  Boundary filling parameters.
  */
  void setFill(const OdGiFill* pFill);
  
  /** \details
  Sets pointer to auxiliary data context.

  \param pAuxData [in]  pointer to auxiliary data context.
  */
  void setAuxData(const OdGiAuxiliaryDataPtr& pAuxData);

  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // OdGiCommonDraw Overrides

  
  /** \details
    Retrieves context associated with this object.
    
    \returns
    Pointer to the associated context.
  */
  OdGiContext* context() const;
  
  /** \details
    Checks whether this vectorization process should be aborted.
    
    \returns
    true if this vectorization process should be aborted, false otherwise.
  */
  bool regenAbort() const;
  
  /** \details
    Provides access to this object's subentity traits.

    \remarks
    This allows the modification of the vectorization attributes such as color, linetype, etc.
  */
  OdGiSubEntityTraits& subEntityTraits() const;
  
  /** \details
    Retrieves the recommended maximum deviation of the
    current vectorization for the specified point on a curve.

    \param deviationType [in]  Deviation type.
    \param pointOnCurve [in]  Point on a curve.
        
    \returns
    recommended maximum difference (with respect to the current active viewport) between the actual curve or surface, 
    and the tessellated curve or surface. 
    
    \remarks
    deviationType must be one of the following:
    
    <table>
    Name                       Value
    kOdGiMaxDevForCircle       0      
    kOdGiMaxDevForCurve        1      
    kOdGiMaxDevForBoundary     2      
    kOdGiMaxDevForIsoline      3
    kOdGiMaxDevForFacet        4
    </table>

    \remarks
    This method uses circle zoom percent or FacetRes as appropriate.
  */
  double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const;
  
  /** \details
    Returns the regeneration type of the current vectorization process.

    \remarks
    regenType() returns one of the following:
    
    <table>
    Name                            Value 
    eOdGiRegenTypeInvalid           0
    kOdGiStandardDisplay            2
    kOdGiHideOrShadeCommand         3
    kOdGiRenderCommand              4
    kOdGiForExplode                 5
    kOdGiSaveWorldDrawForProxy      6
    kOdGiForExtents                 7
    </table>
  */
  OdGiRegenType regenType() const;
  
  /** \details
    Retrieves count of isolines that are used to represent an entity.
    
    \returns
    number of isolines.
    \remarks
    Isolines are lines or curves which are used to represent the entity. 
    For example, if an entity is a sphere and this method returns 3, the 
    sphere should be displayed as 3 circles with a common center and planes that are
    perpendicular to each other.
  */
  OdUInt32 numberOfIsolines() const;
  
  /** \details
    Retrieves raw geometry associated with this object.
    
    \returns
    raw geometry.
    \remarks
    The method cannot return NULL.
  */
  OdGiGeometry& rawGeometry() const;
  
  /** \details
    Checks whether this vectorization process is the result of a "drag" operation.
    
    \returns
    true if this vectorization process is the result of a "drag" operation, false otherwise.
  */
  bool isDragging() const;

  /////////////////////////////////////////////////////////////////////////////

  /** \details
    Retrieves draw context flags of this vectorizer.
    
    \returns
    draw context flags of this vectorizer as OdUInt32 value.
  */
  OdUInt32 drawContextFlags() const;
  
  /** \details
    Sets draw context flags for this vectorizer.
    
    \param flags [in]  Draw context flags.
    \param bFlag [in]  Enables or disables specified flags.
  */
  void setDrawContextFlags(OdUInt32 flags, bool bFlag);

  /////////////////////////////////////////////////////////////////////////////
  // OdGiViewportDraw Override
  //

  /** \details
      Retrieves sequence number. The default implementation of this function always returns 0.
      
      \returns
      0
  */
  OdUInt32  sequenceNumber() const;

  /** \details
      Checks whether the specified identifier of a viewport is valid. The default implementation of this function always returns 0.
      
      \param viewportId [in]  Identifier of a viewport to check.
      
      \returns
      false
  */
  bool isValidId(const OdUInt32 viewportId) const;

  /** \details
      Retrieves identifier of a viewport. The default implementation of this function always returns 0.
  
  \returns
  viewport identifier when implemented, currently returns 0.
  */
  OdDbStub* viewportObjectId() const;

  /** \details
      Retrieves a reference to a viewport. The default implementation of this function returns a reference to a dummy viewport.
  
  \returns
  reference to a viewport.
  */
  OdGiViewport& viewport() const;

  /////////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////////
  // OdGiGeometry Overrides

  /** \details
    Introduces a circle into this vectorization context.

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal);

  /** \details
    Introduces a circle into this vectorization context.

    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \param thirdPoint [in]  Third point.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  void circle(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint);

  /** \details
    Introduces a circular arc into this vectorization context.

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal to a plane on which a circular arc lies.
    \param startVector [in]  Defines the start of this arc.
    \param sweepAngle [in]  Angle that defines the arc.
    \param arcType [in]  Arc type.
    
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     2         Filled area bounded by the arc and its end points
    </table>
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  void circularArc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple);

  /** \details
    Introduces a circular arc into this vectorization context.

    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \param thirdPoint [in]  Third point.
    \param arcType [in]  Arc type.
    
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     2         Filled area bounded by the arc and its end points
    </table>
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  void circularArc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple);

  /** \details
    Introduces a polyline into this vectorization context.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param baseSubEntMarker [in]  Base sub-entity marker.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void polyline(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    OdGsMarker baseSubEntMarker = -1);

  /** \details
    Introduces a polygon into this vectorization context.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \sa
    <link tv_geometry_polygons, Work with Polygons>
  */
  void polygon(OdInt32 numVertices, const OdGePoint3d* vertexList);
  
  /** \details
    Introduces a polygon into this vectorization context.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \sa
    <link tv_geometry_polygons, Work with Polygons>
  */
  void polygon(OdInt32 numVertices, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal);

  /** \details
    Introduces a pline into this vectorization context.

    \param polyline [in]  Polyline data to be processed.
    \param fromIndex [in]  Index of the first segment to be processed.
    \param numSegs [in]  Number of segments to be processed.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void pline(const OdGiPolyline& polyline, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0);

  /** \details
    Introduces a shape into this vectorization context.
    
    \param position [in]  Position of the shape.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
    \param direction [in]  Baseline direction for the shape.
    \param normal [in]  Normal vector.
  */
  void shape(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    int shapeNumber, const OdGiTextStyle* pTextStyle);
  
  /** \details
    Introduces a text into this vectorization context.
    
    \param position [in]  Position of the text.
    \param normal [in]  Normal to a plane where text lies. Used to define v parameter as normal.crossProduct(direction).
    \param direction [in]  Direction vector that is interpreted as u parameter.
    \param height [in]  Height of the text.
    \param width [in]  Width of the text.
    \param oblique [in]  Oblique angle. Must not exceed an equivalent of 85 degrees in radians in both directions.
    \param msg [in]  Message string.
    \sa
    <link tv_geometry_text, Work with Text>
  */
  void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    double height, double width, double oblique, const OdString& msg);

  /** \details
    Introduces a text into this vectorization context.
    
    \param position [in]  Position of the text.
    \param normal [in]  Normal to a plane where text lies. Used to define v parameter as normal.crossProduct(direction).
    \param direction [in]  Direction vector that is interpreted as u parameter.
    \param msg [in]  Pointer to a message string.
    \param length [in]  String length.
    \param raw [in]  Flag that specifies whether escape sequences, such as %%P, will not be converted to special characters.
    \param pTextStyle [in]  Pointer to a text style.
    \sa
    <link tv_geometry_text, Work with Text>
  */
  void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle);

  /** \details
    Introduces an xline into this vectorization context.
    
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void xline(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);

  /** \details
    Introduces a ray into this vectorization context.
    
    \param basePoint [in]  Base point.
    \param throughPoint [in]  Through point
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void ray(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint );

  /** \details
    Introduces nurbs into this vectorization context.
    
    \param nurbsCurve [in]  NURBS curve data.
    \sa
    <link tv_geometry_nurbs, Work with NURBS>
  */
  void nurbs(const OdGeNurbCurve3d& nurbsCurve);

  /** \details
    Introduces an elliptical arc into this vectorization context.

    \param ellipArc [in]  Elliptical arc.
    \param endPointOverrides [in]  Array of points to be used as the first and last points of the vectorized arc.
    \param arcType [in]  Arc type.
    
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     2         Filled area bounded by the arc and its end points
    </table>
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  void ellipArc(const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple);

  /** \details
    Introduces a mesh into this vectorization context.

    \param numRows [in]  Number of rows.
    \param numColumns [in]  Number of columns.
    \param vertexList [in]  Array of vertices.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data. 
    \sa
    <link tv_geometry_meshes, Work with Meshes>
  */
  void mesh(OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  /** \details
    Introduces a shell into this vectorization context.

    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices.
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  Array of numbers that define the faces.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data.  
    \sa
    <link tv_geometry_shells, Work with Shells>
  */
  void shell(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  /** \details
    Introduces a world line into this vectorization context.

    \param points [in]  Points of the world line. 
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void worldLine(const OdGePoint3d points[2]);

  /** \details
    Introduces a raster image into this vectorization context.

    \param img [in]  Reference to a raster image.
    \param origin [in]  Lower-left corner of an image.
    \param uVec [in]  Image width vector.
    \param vVec [in]  Image height vector.
    \param trpMode [in]  Image transparency mode.
    \sa
    <link tv_geometry_raster_images, Work with Raster Image Geometry Data>
  */
  void image(const OdGiImageBGRA32& img,
    const OdGePoint3d& origin,
    const OdGeVector3d& uVec,
    const OdGeVector3d& vVec,
    OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit);

  /** \details
    Introduces a hatch boundary into this vectorization context.

    \param edges [in]  Array of 2d points that define a boundary.
  */
  void edge(const OdGiEdge2dArray& edges);

  /** \details
    Introduces an array of points into this vectorization context.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pColors [in]  Pointer to point colors.
    \param pTransparency [in]  Pointer to point transparency.
    \param pNormals [in]  Array of normal vectors.
    \param pSubEntMarkers [in]  Array of sub-entity markers.
    \param nPointSize [in]  Points size.
  */
  void polypoint(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency,
                 const OdGeVector3d* pNormals, const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize);

  /** \details
    Introduces set of polygons to this vectorization context.

    \param numIndices [in]  Number of polgon indices.
    \param pNumPositions [in]  Number of positions per polygon.
    \param pPositions [in]  Array of polygon offsets for all polygon invocations.
    \param pNumPoint [in]  Number of points per polygon.
    \param pPoints [in]  Array of polygon points.
    \param pOutlineColors [in]  Colors of polygon outlines.
    \param pOutlinePsLinetypes [in]  Linetypes of polygon outlines (see OdPs::LineType enumeration).
    \param pFillColors [in]  Polygon fill colors.
    \param pFillTransparencies [in]  Polygon fill transparencies.
  */
  virtual void polyPolygon(OdUInt32 numIndices, const OdUInt32 *pNumPositions, const OdGePoint3d *pPositions,
                           const OdUInt32 *pNumPoints, const OdGePoint3d *pPoints,
                           const OdCmEntityColor *pOutlineColors = NULL, const OdUInt32 *pOutlinePsLinetypes = NULL,
                           const OdCmEntityColor *pFillColors = NULL, const OdCmTransparency *pFillTransparencies = NULL);

  /** \details
    Introduces a row of points into this vectorization context.

    \param numPoints [in]  Number of points.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset to next point.
  */
  void rowOfDots(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  /** \details
    Introduces a Point Clound object into this vectorization context.

    \param pCloud [in]  Point Cloud object.
  */
  void pointCloud(const OdGiPointCloud &pCloud);

  /** \details
    Pushes a clipping boundary onto the stack, which can possibly affect the display of subsequent primitives.
    Use the popClipBoundary() to remove this clipping boundary from the stack.

    \param pBoundary [in]  Pointer to a clipping boundary.
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
  */
  void pushClipBoundary(OdGiClipBoundary* pBoundary);
  
  /** \details
    Pushes a clipping boundary onto the stack, which can possibly affect the display of subsequent primitives.
    Use the popClipBoundary() to remove this clipping boundary from the stack.

    \param pBoundary [in]  Pointer to a clipping boundary.
    \param pClipInfo [in]  Clipping details.
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
  */
  void pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);
  
  /** \details
    Draws the specified clipping boundary.

    \param pBoundary [in]  Pointer to a clipping boundary.
    \param pClipInfo [in]  Clipping details.
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
  */
  void drawClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);
  
  /** \details
    Pops a top clipping boundary from the stack. This method should be used after a successful pushClipBoundary() method call
    from the worldDraw or viewportDraw of a drawable object.
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
  */
  void popClipBoundary();
  
  
  bool isClipping() const;

  /** \details
    Pushes a transformation matrix onto the stack. The resulting transformation will be created by multiplying the specified transform matrix by the existing one.
    This allows you to easily adjust an entity coordinate system. Use the popModelTransform() to remove this transform matrix from the stack when you are finished with this transformation.

    \param xfm [in]  Transformation matrix.
  */
  void pushModelTransform(const OdGeMatrix3d& xfm);
  
  /** \details
    Pushes a transformation vector onto the stack. The resulting transformation will be created by using the arbitrary axis algorithm and the specified vector.
    This allows you to easily adjust an entity coordinate system. Use the popModelTransform() to remove this transform vector from the stack when you are finished with this transformation.

    \param normal [in]  Transformation matrix.
  */
  void pushModelTransform(const OdGeVector3d& normal);
  
  /** \details
    Pops a transformation from the stack. This method should be used after a successful pushModelTransform() method call to restore previous model transform state.
  */
  void popModelTransform();

  /** \details
    Retrieves the block insert transform, which is the model-to-world coordinate transform.
    
    \returns
    model-to-world transform matrix.
  */
  OdGeMatrix3d getModelToWorldTransform() const;
  
  /** \details
    Retrieves model-to-world coordinate transform which is inverse of the retrieved matrix from the getModelToWorldTransform() method. 
    The retrieved matrix can transform an entity's world coordinate representation back into the original model coordinates.
    
    \returns
    world-to-model transform matrix.
  */
  OdGeMatrix3d getWorldToModelTransform() const;

  bool isXrefOverride() const { return GETBIT(m_flags, kXrefOverride); }
  void setXrefOverride(bool bSet) { SETBIT(m_flags, kXrefOverride, bSet); }

  /** \details
    Draws the specified drawable.
    
    \param pDrawable [in]  pointer to the drawable to draw.
  */
  void draw(const OdGiDrawable* pDrawable);

  /** \details
    Retrieves the current drawable nesting graph.
    
    \returns
    current drawable nesting graph.
  */
  const OdGiPathNode* currentGiPath() const;

  /////////////////////////////////////////////////////////////////////////////

  /** \details
    Sets the default drawable traits for this Vectorizer object,
    and sets the drawable flags for the specified OdGiDrawable object.
    
    \param pDrawable [in]  Pointer to the Drawable object.
  */
  virtual OdUInt32 setAttributes(const OdGiDrawable* pDrawable);

  /** \details
    Retrieves the  drawable flags of the last vectorized OdGiDrawable object.
  */
  OdUInt32 drawableAttributes() const { return m_nDrawableAttributes; }

  /** \details
    Returnes true if and only the Drawable object associated with
    this Vectorizer is visible.
    \param drawableFlags [in]  Drawable flags.
    \remarks
    Called by draw().
  */
  bool needDraw(OdUInt32 drawableFlags);

  /** \details
    Vectorizes the specified OdGiDrawable object.
    \param drawableFlags [in]  Drawable flags.
    \param pDrawable [in]  Pointer to the Drawable object.
    
    \remarks
    Called by draw() after subSetAttributes().
    
    Returns pDrawable->subWorldDraw(this). If pDrawable->subWorldDraw returns false, 
    and kSuppressViewportDraw is not set, this function evokes pDrawable->subViewportDraw(this).
  */
  virtual bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable);

  /////////////////////////////////////////////////////////////////////////////
  // OdGiWorldGeometry Override

  /** \details
    Sets geometrical extents for this vectorizer. Currently not implemented.
    
    \param newExtents [in]  New geometrical extents to set.
  */
  void setExtents(const OdGePoint3d *newExtents);
  
  /** \details
    Sets the OdGiBaseVectorizer::kAttributesSegment flag for this vectorizer to true.
  */
  void startAttributesSegment();
  
  /** \details
    Checks whether this vectorizer has the OdGiBaseVectorizer::kAttributesSegment flag set to true.
    
    \returns
    true if this vectorizer has the OdGiBaseVectorizer::kAttributesSegment flag set to true, false otherwise.
  */
  bool isAttributesSegmentEnabled();

  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // OdGiViewportGeometry

  /** \details
    Introduces a raster image into this vectorization context in device coordinate system.
  
    \param origin       [in]  Image origin.
    \param u            [in]  Image width vector.
    \param v            [in]  Image height vector.
    \param pImage       [in]  pointer to a raster image.
    \param uvBoundary   [in]  pointer to an array of boundary points.
    \param numBoundPts  [in]  number of boundary points.
    \param transparency [in]  Flag that specifies image transparency.
    \param brightness   [in]  Brightness of an image in range of [0..100].
    \param contrast     [in]  Contrast of an image in range of [0..100].
    \param fade         [in]  Fade of an image in range of [0..100].
    \sa
    <link tv_geometry_raster_images, Work with Raster Image Geometry Data>
  */
  void rasterImageDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage,
    const OdGePoint2d* uvBoundary,
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0);

  /** \details
    Introduces a metafile into this vectorization context in device coordinate system.
  
    \param origin         [in]  Origin of a metafile content.
    \param u              [in]  Metafile content u-parameter.
    \param v              [in]  Metafile content v-parameter.
    \param pMetafile      [in]  Pointer to a metafile.
    \param bDcAligned     [in]  Flag that specifies whether metafile content should be aligned to device coordinate system.
    \param bAllowClipping [in]  Flag that specifies whether clipping should be allowed for a metafile.
  */
  void metafileDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool bDcAligned = true,
    bool bAllowClipping = false);

  /** \details
    Introduces a new polyline but in eye coordinate system.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  void polylineEye(OdUInt32 numVertices, const OdGePoint3d* vertexList);
  
  /** \details
    Introduces a new polygon but in eye coordinate system.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  void polygonEye(OdUInt32 numVertices, const OdGePoint3d* vertexList);

  /** \details
    Introduces a new polyline but in device coordinate system.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  void polylineDc(OdUInt32 numVertices, const OdGePoint3d* vertexList);
  
  /** \details
    Introduces a new polygon but in device coordinate system.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  void polygonDc(OdUInt32 numVertices, const OdGePoint3d* vertexList);

  /** \details
    Introduces a new polyline in selected coordinates system.
  
    \param cs                [in]  Coordinates system.
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  void polylineCs(OdGiContext::CoordinatesSystem cs, OdUInt32 numVertices, const OdGePoint3d* vertexList);
  
  /** \details
    Introduces a new polygon in selected coordinates system.
  
    \param cs                [in]  Coordinates system.
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  void polygonCs(OdGiContext::CoordinatesSystem cs, OdUInt32 numVertices, const OdGePoint3d* vertexList);


  /////////////////////////////////////////////////////////////////////////////
  // OdGiConveyorContext Overrides

  /** \details
    This method can be called whenever entity traits are changed. Default implementation does nothing.
  */
  virtual void onTraitsModified();

  /////////////////////////////////////////////////////////////////////////////

  /** \details
    Retrieves the maximum ModelSpace (UCS) deviation for this VectorizeView object.
    
    \returns
    maximum ModelSpace (UCS) deviation as the OdGiDeviation instance.
  */
  const OdGiDeviation& modelDeviation() const;
  /** \details
    Retrieves the maximum world (WCS) deviation for this VectorizeView object.
    
    \returns
    maximum world space (WCS) deviation as the OdGiDeviation instance.
  */
  const OdGiDeviation& worldDeviation() const;
  /** \details
    Retrieves the maximum EyeSpace (ECS) deviation for this VectorizeView object.
    
    \returns
    maximum eye space (ECS) deviation as the OdGiDeviation instance.
  */
  const OdGiDeviation& eyeDeviation() const;

  // materials
  
  /** \details
    Retrieves the material traits data from the effective entity traits.
    If no material is assigned before the call, then default material is created
    and assigned to the entity inside the method.
    
    \returns
    material traits data.
  */
  const OdGiMaterialTraitsData &effectiveMaterialTraitsData() const;
  
  /** \details
    Retrieves the material traits data from the effective entity traits.
    If no material is assigned before the call, then default material is created
    and assigned to the entity inside the method.
    
    \param data        [out]  Receives material traits data.
  */
  void effectiveMaterialTraitsData(OdGiMaterialTraitsData &data) const;
  
  /** \details
    Sets the specified material traits data.
    
    \param materialId       [in]  Material identifier.
    \param data             [in]  Material traits data to set.
    \param bForce           [in]  Flag that specifies whether to force material traits setting. This flag is needed to set traits even if the specified material identifier is the same as material identifier of this vectorizer.
  */
  void setEffectiveMaterialTraitsData(OdDbStub *materialId, const OdGiMaterialTraitsData &data, bool bForce = false);

  /** \details
    Retrieves current selection marker state.
    
    \returns
    current selection marker state as an OdGsMarker instance.
  */
  OdGsMarker selectionMarker() const;

  const OdGiSubEntityTraitsData *byBlockTraits() const { return m_pByBlock; }
  void setByBlockTraits(const OdGiSubEntityTraitsData *pByBlock) { m_pByBlock = pByBlock; }
  OdRxObjectPtr saveByBlockTraits();
};

/** \details
    This class is an implementation of the subWorldDraw() framework to calculate the WCS extents of an OdGiDrawable object.
    Corresponding C++ library: TD_Gi
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiExtCalc : public OdGiBaseVectorizer
{
protected:
  bool            m_BBoxSet;
  OdGiExtAccumPtr m_pExtAccum;

public:

  /** \details
    Default constructor for the OdGiExtCalc class. Initializes the object by setting drawing contexts, adding a source node, etc.
  */
  OdGiExtCalc();
  
  /** \details
    Destructor for the OdGiExtCalc class.
  */
  virtual ~OdGiExtCalc();

  /** \details
    Retrieves regeneration type.
    
    \returns
    regeneration type as an OdGiRegenType instance.
  */
  OdGiRegenType regenType() const;
  
  /** \details
    Checks whether regeneration process should be aborted.
    
    \returns
    true if the regeneration process should be aborted, false otherwise.
  */
  bool regenAbort() const;
  
  /** \details
    Draws the specified drawable.
    
    \returns
    \param pDrawable    [in]  Pointer to a drawable to draw.
  */
  void draw(const OdGiDrawable* pDrawable);
  
  /** \details
    Sets extents for this object.
    
    \returns
    \param newExtents    [in]  Pointer to an extents to set.
  */
  void setExtents(const OdGePoint3d *newExtents);
  /** \details
    Resets the accumulated extents for the OdGiDrawable object associated with this Vectorization object.
  */
  void resetExtents();

  /** \details
    Retrieves the accumulated extents for the OdGiDrawable object associated with this Vectorization object.
    \param extents [out]  Receives the extents.
  */
  void getExtents(OdGeExtents3d& extents) const;

  /** \details
    Sets the drawing context for this object.
    \param pUserContext [in]  Drawing context to set.
  */
  void setContext(OdGiContext* pUserContext);

  // optimization

  /** \details
    Retrieves sub-entity effective traits.
    
    \returns
    sub-entity effective traits as OdGiSubEntityTraitsData.
  */
  const OdGiSubEntityTraitsData& effectiveTraits() const;
  
  /** \details
    Affects effective sub-entity traits. Default implementation does noting.
    
    \param pSource [in]  Source.
    \param destination [in]  Destination.
  */
  void affectTraits(const OdGiSubEntityTraitsData* pSource, OdGiSubEntityTraitsData& destination) const;
};

#ifdef ENABLE_GITRAITSOPTIMIZATION

/** \details
    Resets entity traits but keeps auxiliary data.
  */
inline void OdGiBaseVectorizer::resetEntityTraitsDataKeepAuxData()
{
  OdGiAuxiliaryDataPtr pAuxData   = m_entityTraitsData.auxData();
  OdUInt32             uLockFlags = m_entityTraitsData.lockFlags();

  m_bResetEntityTraitsData = false;
  m_entityTraitsData = g_entityTraitsDataReset;

  m_entityTraitsData.setAuxData(pAuxData);
  m_entityTraitsData.setLockFlags(uLockFlags & kInheritableLockFlags);

  setEntityTraitsDataChanged();
}

/** \details
    Resets all entity traits.
  */
inline void OdGiBaseVectorizer::resetEntityTraitsData()
{
  if (m_entityTraitsData.lockFlags() & OdGiSubEntityTraits::kKeepAuxDataAndLockFlags)
    resetEntityTraitsDataKeepAuxData();
  else
  {
    m_bResetEntityTraitsData = false;
    m_entityTraitsData = g_entityTraitsDataReset;

    if (m_pByBlock)
      updateTraitsDataByBlockTraits(m_pByBlock);

    setEntityTraitsDataChanged();
  }
}

/** \details
    Updates entity traits with the traits of the owning block.
  */
inline void OdGiBaseVectorizer::updateTraitsDataByBlockTraits(const OdGiSubEntityTraitsData* pBlockTraits)
{
  m_entityTraitsData.setAuxData(pBlockTraits->auxData());
  m_entityTraitsData.setLockFlags(pBlockTraits->lockFlags() & OdGiSubEntityTraits::kInheritableLockFlags);
}
#endif

/** \details
    Calculates u and v parameters of the text by the specified parameters.
    
    \param u [out]  u-parameter to calculate.
    \param v [out]  v-parameter to calculate.
    \param normal [in]  Normal to a text.
    \param direction [in]  Direction of a text.
    \param height [in]  Text height.
    \param width [in]  Text width.
    \param oblique [in]  Oblique angle. Must not exceed an equivalent of 85 degrees in radians in both directions.
    \param bMirrorX [in]  Flag that specifies whether u parameter should be negated.
    \param bMirrorY [in]  Flag that specifies whether v parameter should be negated.
  */
ODGI_EXPORT void odgiCalculateTextBasis(OdGeVector3d& u, OdGeVector3d& v,
                                        const OdGeVector3d& normal,
                                        const OdGeVector3d& direction,
                                        double height,
                                        double width,
                                        double oblique,
                                        bool bMirrorX = false,
                                        bool bMirrorY = false);

/** \details
    Copies text style parameters from the pStyle parameter to the res parameter and set additional parameters for the copied style.
    
    \param pStyle [in]  Pointer to a text style.
    \param res    [in]  v-parameter to calculate.
    
    \remarks
    Additional text style methods that are used to set properties for the copied text style:
    <ul>
    * setBackward(false)
    * setObliquingAngle(0.)
    * setTextSize(1.)
    * setUpsideDown(false)
    * setXScale(1.)
    </ul>
  */
ODGI_EXPORT const OdGiTextStyle* odgiPrepareTextStyle(const OdGiTextStyle* pStyle, OdGiTextStyle& res);

#include "TD_PackPop.h"

#endif // #ifndef _ODGIBASEVECTORIZER_INCLUDED_
