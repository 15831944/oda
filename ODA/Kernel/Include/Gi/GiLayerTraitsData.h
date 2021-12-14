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

#ifndef _ODGILAYERTRAITSDATA_INCLUDED_
#define _ODGILAYERTRAITSDATA_INCLUDED_

#include "Gi/GiNonEntityTraits.h"
#include "CmColorBase.h"
#include "GiDrawImpl.h"
#include "GiDummyGeometry.h"
class OdGiBaseVectorizer;

class OdDbStub;

#include "TD_PackPush.h"

/** \details
    This class provides an implementation of the OdGiLayerTraits::color() function.
    
    \sa
    TD_Gi
   
    <group OdGi_Classes>
*/
class OdGiLayerTraits_ : public OdGiLayerTraits
{
  virtual OdCmEntityColor trueColor() const = 0;
protected:
  OdCmEntityColor color() const { return trueColor(); }
};

/** \details
    This class provides an implementation of the OdGisubEntityTraits::color() function.
    
    \sa
    TD_Gi
   
    <group OdGi_Classes>
*/
class OdGiSubEntityTraits_ : public OdGiDummySubEntityTraits< OdGiSubEntityTraits >
{
  virtual OdCmEntityColor trueColor() const = 0;
protected:
  OdUInt16 color() const { return trueColor().colorIndex(); }
};

/** \details

    <group !!RECORDS_TD_APIRef>
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
*/
struct OdGiLayerTraitsData
{
  /** \details
    Default constructor for the OdGiLayerTraitsData structure. Sets the following data members:
    <table>
    <B>Data member</B>    <B>Value</B>                              <B>Description</B>
    m_flags               OdGiLayerTraits::kPlottable               Layer traits flags
    m_lineweight          OdDb::kLnWtByLwDefault                    LineWeight.
    m_plotStyleNameType   OdDb::kPlotStyleNameById                  Plot style name type.
    m_color               OdCmEntityColor::kForeground              Layer color.
    m_linetypeId          0                                         Linetype identifier.
    m_plotStyleNameId     0                                         Plot style name identifier.
    m_materialId          0                                         Material identifier.
    m_cmTransparency      OdUInt8(0xFF)                             Transparency color.
    </table>
  */
  OdGiLayerTraitsData()
    : m_flags(OdGiLayerTraits::kPlottable)
    , m_lineweight(OdDb::kLnWtByLwDefault)
    , m_plotStyleNameType(OdDb::kPlotStyleNameById)
    , m_color(OdCmEntityColor::kForeground)
    , m_linetypeId(0)
    , m_plotStyleNameId(0)
    , m_materialId(0)
    , m_cmTransparency(OdUInt8(0xFF))
  {
  }
  
  /** \details
    Assignment operator for the OdGiLayerTraitsData class.

    \param other [in]  Properties of other OdGiLayerTraitsData object to assign.
  */
  void operator =(const OdGiLayerTraitsData& other)
  {
    m_flags             = other.m_flags;
    m_lineweight        = other.m_lineweight;
    m_plotStyleNameType = other.m_plotStyleNameType;
    m_color             = other.m_color;
    m_linetypeId        = other.m_linetypeId;
    m_plotStyleNameId   = other.m_plotStyleNameId;
    m_materialId        = other.m_materialId;
    m_cmTransparency    = other.m_cmTransparency;
  }

  
  /** \details
    Retrieves layer traits flags.
  */
  OdUInt32 flags() const { return m_flags; }
  
  /** \details
    Retrieves lineweight associated with this layer.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  OdDb::LineWeight lineweight() const { return m_lineweight; }
  
  /** \details
    Retrieves persistent linetype identifier associated with the layer.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  OdDbStub* linetype() const { return m_linetypeId; }
  
  /** \details
    Retrieves plot style name type of this layer.
  */
  OdDb::PlotStyleNameType plotStyleNameType() const { return m_plotStyleNameType; }
  
  /** \details
    Retrieves persistent plot style name identifier associated with the layer.
  */
  OdDbStub* plotStyleNameId() const { return m_plotStyleNameId; }
  
  /** \details
    Retrieves color for this layer.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  const OdCmEntityColor& trueColor() const { return m_color; }
  
  /** \details
    Retrieves persistent material identifier associated with the layer.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  OdDbStub* materialId() const { return m_materialId; }
  
  /** \details
    Retrieves transparency associated with the layer.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  OdCmTransparency transparency() const { return m_cmTransparency; }
  
  /** \details
    Sets color for this layer.
    \param cl [in]  Color to set.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  void setTrueColor(const OdCmEntityColor& cl) { m_color = cl; }
  
  /** \details
    Sets lineweight associated for this layer.
    \param lw [in]  LineWeight to set.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  void setLineweight(OdDb::LineWeight lw) { m_lineweight = lw; }
  
  /** \details
    Sets linetype for this layer.
    \param id [in]  Persistent identifier of a linetype to set.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  void setLinetype(OdDbStub* id) { m_linetypeId = id; }
  
  /** \details
    Sets material for this layer.
    \param id [in]  Persistent identifier of a material to set.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  void setMaterial(OdDbStub* id) { m_materialId = id; }
  
  /** \details
    Sets transparency for this layer.
    \param transparency [in]  Transparency to set.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  void setTransparency(const OdCmTransparency& transparency) { m_cmTransparency = transparency; }

  /** \details
    Checks whether this layer is turned off.
    \returns
    true if this layer is off, false otherwise.
  */
  bool isOff() const { return GETBIT(m_flags, OdGiLayerTraits::kOff); }
  
  /** \details
    Switches the layer on or off.
    \param bVal [in]  Layer state.
  */
  void setOff(bool bVal) { SETBIT(m_flags, OdGiLayerTraits::kOff, bVal); }

  /** \details
    Checks whether this layer is plottable.
    \returns
    true if this layer is plottable, false otherwise.
  */
  bool isPlottable() const { return GETBIT(m_flags, OdGiLayerTraits::kPlottable); }
  
  /** \details
    Specifies whether this layer is plottable.
    \param bVal [in]  Flag that specifies whether this layer should be plottable.
  */
  void setPlottable(bool bVal) { SETBIT(m_flags, OdGiLayerTraits::kPlottable, bVal); }

  /** \details
    Checks whether this layer is locked.
    \returns
    true if this layer is locked, false otherwise.
  */
  bool isLocked() const { return GETBIT(m_flags, OdGiLayerTraits::kLocked); }
  
  /** \details
    Specifies whether this layer is locked.
    \param bVal [in]  Flag that specifies whether this layer should be locked.
  */
  void setLocked(bool bVal) { SETBIT(m_flags, OdGiLayerTraits::kLocked, bVal); }

  /** \details
    Checks whether this layer is frozen.
    \returns
    true if this layer is frozen, false otherwise.
  */
  bool isFrozen() const { return GETBIT(m_flags, OdGiLayerTraits::kFrozen); }
  
  /** \details
    Checks whether this layer inherits traits by block.
    \returns
    true if this layer takes traits from block, false otherwise.
  */
  bool isByBlock() const { return GETBIT(m_flags, OdGiLayerTraits::kByBlock); }

  /** Retrieves layer traits flags. */
  OdUInt32                m_flags;
  /** Lineweight associated with this layer. */
  OdDb::LineWeight        m_lineweight;
  /** Plot style name type of this layer. */
  OdDb::PlotStyleNameType m_plotStyleNameType;

  /** Color for this layer. */
  OdCmEntityColor         m_color;
  /** Persistent linetype identifier associated with the layer. */
  OdDbStub*               m_linetypeId;
  /** Persistent plot style name identifier associated with the layer. */
  OdDbStub*               m_plotStyleNameId;

  /** Persistent material identifier associated with the layer. */
  OdDbStub*               m_materialId;
  /** Transparency for this layer. */
  OdCmTransparency        m_cmTransparency;
};

/** \details
  This class implements OdGiLayerTraits, OdGiWorldDraw, and OdGIViewportDraw together 
	as a single class that uses the OdDbLayerTableRecord::subSetAttributes(), 
	OdDbLayerTableRecord::subWorldDraw(), and OdDbLayerTableRecord::subViewportDraw() functions to return 
	layer trait values and store them temporarily.
  \sa
  <link tv_working_with_attributes_layers, Work with Layers>
   
    <group OdGi_Classes>
*/
class ODGI_EXPORT OdGiLayerTraitsTaker : public OdGiLayerTraits_
                                       , public OdGiDummyWorldDraw< OdGiDummyWorldGeometry< OdGiWorldDraw_ > >
                                       , public OdGiDummyViewportDraw< OdGiWrapperViewportGeometry< OdGiViewportDraw_ > >
                                       , public OdGiSubEntityTraits_
                                       , public OdGiLayerTraitsData
{
protected:
  ODRX_USING_HEAP_OPERATORS( OdGiDummyWorldDraw< OdGiDummyWorldGeometry< OdGiWorldDraw_ > > );
private:
  const OdGiBaseVectorizer* m_pUnderlayingView;
  OdGiContext*              m_pGiContext;
  OdGiAuxiliaryDataPtr      m_pAuxData;

public:
  /** \details
    Default constructor for the OdGiLayerTraitsTaker class. Sets underlaying view and GI context to NULL.
  */
  OdGiLayerTraitsTaker();
  
  /** \details
    Constructor for the OdGiLayerTraitsTaker class. Sets the object with the vectorizer and GI context.
    \param pRefView [in]  Pointer to a vectorizer.
    \param pContext [in]  Pointer to GI context.
  */
  OdGiLayerTraitsTaker(const OdGiBaseVectorizer* pRefView, OdGiContext* pContext = NULL);

  /** \details
    Adds one reference to this object.
  */
  void addRef();
  /** \details
    Removes one reference from this object.
  */
  void release();

  void setRefView(const OdGiBaseVectorizer* pRefView);
  
  /** \details
    Resets layer traits to default.
  */
  void reset(const OdGiLayerTraitsData &ltData = OdGiLayerTraitsData())
  { static_cast<OdGiLayerTraitsData&>(*this) = ltData; }

  /** \details
    Retrieves lineweight associated with this layer.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  OdDb::LineWeight lineweight() const;
  
  /** \details
    Retrieves a pointer to persistent linetype identifier associated with the layer.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  OdDbStub* linetype() const;
  
  /** \details
    Retrieves plot style name type of this layer.
  */
  OdDb::PlotStyleNameType plotStyleNameType() const;
  
  /** \details
    Retrieves pointer to persistent plot style name identifier associated with the layer.
  */
  OdDbStub* plotStyleNameId() const;
  
  /** \details
    Retrieves pointer to persistent material identifier associated with the layer.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  OdDbStub* materialId() const;
  
  /** \details
    Sets color for this layer.
    \param color [in]  Color to set.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  void setColor(const OdCmEntityColor& color);
  
  /** \details
    Sets lineweight associated for this layer.
    \param lineweight [in]  LineWeight to set.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  void setLineweight(OdDb::LineWeight lineweight);
  
  /** \details
    Sets linetype for this layer.
    \param pLinetypeId [in]  Pointer to persistent identifier of a linetype to set.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  void setLinetype(OdDbStub* pLinetypeId);
  
  /** \details
    Sets plot style name for this layer.
    \param plotStyleNameType [in]  Plot style name type.
    \param pPlotStyleNameId [in]  Pointer to persistent plot style name identifier.
  */
  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId= 0);

  /** \details
    Checks whether this layer is turned off.
    \returns
    true if this layer is off, false otherwise.
  */
  bool isOff() const { return GETBIT(m_flags, kOff); }
  
  /** \details
    Switches the layer on or off.
    \param bVal [in]  Layer state.
  */
  void setOff(bool bVal) { SETBIT(m_flags, kOff, bVal); }

  /** \details
    Checks whether this layer is plottable.
    \returns
    true if this layer is plottable, false otherwise.
  */
  bool isPlottable() const { return GETBIT(m_flags, kPlottable); }
  
  /** \details
    Specifies whether this layer is plottable.
    \param bVal [in]  Flag that specifies whether this layer should be plottable.
  */
  void setPlottable(bool bVal) { SETBIT(m_flags, kPlottable, bVal); }

  /** \details
    Checks whether this layer is locked.
    \returns
    true if this layer is locked, false otherwise.
  */
  bool isLocked() const { return GETBIT(m_flags, kLocked); }
  
  /** \details
    Specifies whether this layer is locked.
    \param bVal [in]  Flag that specifies whether this layer should be locked.
  */
  void setLocked(bool bVal) { SETBIT(m_flags, kLocked, bVal); }

  
  /** \details
    Retrieves color for this layer.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  OdCmEntityColor trueColor() const;
  
  /** \details
    Retrieves persistent layer identifier of the underlaying layer.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  OdDbStub* layer() const;
  
  /** \details
    Retrieves persistent linetype identifier.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  OdDbStub* lineType() const;
  
  /** \details
    Retrieves fill type for a layer.
  */
  OdGiFillType fillType() const;
  
  /** \details
    Retrieves lineweight for a layer.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  OdDb::LineWeight lineWeight() const;
  
  /** \details
    Retrieves linetype scale for a layer.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  double lineTypeScale() const;
  
  /** \details
    Retrieves thickness for a layer.
  */
  double thickness() const;
  
  /** \details
    Retrieves persistent material identifier.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  OdDbStub* material() const;
  
  /** \details
    Retrieves pointer a mapper for this layer.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  const OdGiMapper* mapper() const;
  
  /** \details
    Retrieves persistent visual style identifier.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  OdDbStub* visualStyle() const;
  
  /** \details
    Retrieves transparency for this layer.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  OdCmTransparency transparency() const;
  
  /** \details
    Retrieves draw flags for this layer.
  */
  OdUInt32 drawFlags() const;
  
  /** \details
    Checks whether geometry of this layer can be selected.
    \returns
    true if geometry of this layer can be selected, false otherwise.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  bool selectionGeom() const;
  
  /** \details
    Retrieves shadow flags for this layer.
    \returns
    true if geometry of this layer can be selected, false otherwise.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  ShadowFlags shadowFlags() const;
  
  /** \details
    Checks whether content of this layer is sectionable.
    \returns
    true if content of this layer is sectionable, false otherwise.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  bool sectionable() const;
  
  /** \details
    Retrieves shadow flags of for this layer.
    \returns
    true if geometry of this layer can be selected, false otherwise.
  */
  SelectionFlags selectionFlags() const;

  /** \details
    Sets color for this layer.
    \param color [in]  Color to set.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  void setTrueColor(const OdCmEntityColor& color);
  
  /** \details
    Sets color for this layer.
    \param color [in]  Color to set.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  void setColor(OdUInt16 color);
  
  /** \details
    Sets linetype identifier for this layer.
    \param lineTypeId [in]  Linetype identifier to set.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  void setLineType(OdDbStub* lineTypeId);
  
  /** \details
    Sets lineweight for this layer.
    \param lineWeight [in]  Lineweight to set.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  void setLineWeight(OdDb::LineWeight lineWeight);
  
  /** \details
    Sets material for this layer.
    \param pMaterialId [in]  Pointer to persistent material identifier.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  void setMaterial(OdDbStub* pMaterialId);
  
  /** \details
    Sets transparency for this layer.
    \param transparency [in]  Transparency to set.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  void setTransparency(const OdCmTransparency &transparency);

  
  /** \details
    Retrieves secondary color for this layer.
  */
  OdCmEntityColor secondaryTrueColor() const;
  
  /** \details
    Retrieves pointer to linetype modifiers for this layer.
  */
  const OdGiDgLinetypeModifiers* lineStyleModifiers() const;
  
  /** \details
    Retrieves pointer to the fill for this layer.
  */
  const OdGiFill* fill() const;

  
  /** \details
    Retrieves regeneration type for this layer.
  */
  OdGiRegenType regenType() const;
  
  /** \details
    Checks whether the vectorization process should be aborted.
    \returns
    true if vectorization process should be aborted, false otherwise.
  */
  bool regenAbort() const;
  
  /** \details
    Retrieves subentity traits associated with this layer.
  */
  OdGiSubEntityTraits& subEntityTraits() const;
  
  /** \details
    Retrieves geometry data for this layer.
  */
  OdGiGeometry& rawGeometry() const;
  
  /** \details
    Checks whether the associated vectorization process is the result of a "drag" operation.
    
    \returns
    true if vectorization process is the result of a "drag" operation, false otherwise.
  */
  bool isDragging() const;
  
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
    This method uses circle zoom percent as appropriate.
  */
  double deviation(const OdGiDeviationType deviationType, 
    const OdGePoint3d& pointOnCurve) const;
  
  /** \details
    Retrieves count of isolines that are used to represent an entity.
    \remarks
    Isolines are lines or curves which are used to represent the entity. 
    For example, if an entity is a sphere and this method returns 3, the 
    sphere should be displayed as 3 circles with a common center and planes that are
    perpendicular to each other.
  */
  OdUInt32 numberOfIsolines() const;
  
  /** \details
    Retrieves context associated with this object.
  */
  OdGiContext* context() const;

  /** \details
    Sets GI context for this object.
    
    \param pContext [in]  Pointer to GI context.
  */
  void setContext( OdGiContext* pContext);

  /** \details
    Retrieves a reference to a viewport.
  */
  OdGiViewport& viewport() const;
  
  /** \details
    Returns the number of viewports that have been deleted from the current database 
    during this editing session.
  */
  OdUInt32 sequenceNumber() const;
  
  /** \details
    Checks whether the identifier of an associated viewport is valid.
    
    \param viewportId [in]  Identifier of a viewport to check.    
    \returns
    true if the viewport identifier is valid, false otherwise.
  */
  bool isValidId(const OdUInt32 viewportId) const;
  
  /** \details
    Retrieves the persistent identifier of an associated viewport.
  */
  OdDbStub* viewportObjectId() const;

  /** \details
    Retrieves the model to world transformation matrix.
  */
  OdGeMatrix3d getModelToWorldTransform() const;
  
  /** \details
    Retrieves the world to model transformation matrix.
  */
  OdGeMatrix3d getWorldToModelTransform() const;

  /** \details
    Retrieves smart pointer to the auxiliary data for this object.
  */
  virtual OdGiAuxiliaryDataPtr auxData() const { return m_pAuxData; }
  
  /** \details
    Sets the auxiliary data for this object.
    
    \param pAuxData [in]  Smart pointer to the auxiliary data. 
  */
  virtual void setAuxData(const OdGiAuxiliaryDataPtr& pAuxData) { m_pAuxData = pAuxData; }
};

inline void
OdGiLayerTraitsTaker::setRefView(const OdGiBaseVectorizer* pRefView)
{
  m_pUnderlayingView = pRefView;
}

inline void
OdGiLayerTraitsTaker::setContext( OdGiContext* pContext)
{
  m_pGiContext = pContext;
}

inline
OdGiLayerTraitsTaker::OdGiLayerTraitsTaker()
  : m_pUnderlayingView(0),
    m_pGiContext( 0 )
{
}

inline
OdGiLayerTraitsTaker::OdGiLayerTraitsTaker(const OdGiBaseVectorizer* pRefView, OdGiContext* pContext)
  : m_pUnderlayingView(pRefView),
    m_pGiContext( pContext )
{
}

#include "TD_PackPop.h"

#endif // #ifndef _ODGILAYERTRAITSDATA_INCLUDED_
