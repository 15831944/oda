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

#ifndef __OD_GI_BASE_VECTORIZER_IMPL_H_
#define __OD_GI_BASE_VECTORIZER_IMPL_H_

#include "TD_PackPush.h"

#include "Gs/GsExport.h"
#include "Gi/GiBaseVectorizer.h"
#include "Gi/GiConveyorEmbranchment.h"
#include "Gi/GiLinetypeRedir.h"
#include "Gi/GiLineweightOverride.h"
#include "Gi/GiStack.h"
#include "Gi/GiPalette.h"
#include "Ps/PlotStyles.h"
#include "Gs/GsViewPropsDef.h"
#include "Gs/GsViewImpl.h"

/** \details
    
    Corresponding C++ library: TD_Gs

    <group OdGs_Classes> 
*/
class GS_TOOLKIT_EXPORT OdGiBaseVectorizerImpl: public OdGiViewport, public OdGiBaseVectorizer
{
protected:
  ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);

public:
  /** \details
    Default constructor for the OdGiBaseVectorizerImpl class.
  */
  OdGiBaseVectorizerImpl();

  /** \details
    This method is to be called when setting up the vectorizer to work
    on the given view.
  **/
  virtual void setUp(OdGsViewImpl& view);

  /** \details
    Returns view associated with this vectorizer.
  */
  OdGsViewImpl& view() const { ODA_ASSERT(m_view); return *m_view; }
  /** \details
    Returns the Aware flags for portion of geometry that had been vectorized
    since last call resetAwareFlags().

    \remarks
    awareFlags() returns a combination of zero or more of the following:

    <table>
    Name                    Value         Geometry_depends_on
    kVpID                   0x00000001    viewport Id (OdGiViewport::viewportId()).
    kVpRegenType            0x00000002    regeneration type (OdGiCommonDraw::regenType()).
    kVpRenderMode           0x00000004    render mode (OdGsView::mode()).
    kVpWorldToEye           0x00000008    description to come...
    kVpPerspective          0x00000010    description to come...
    kVpResolution           0x00000020    description to come...
    kVpMaxDevForCircle      0x00000040    description to come...
    kVpMaxDevForCurve       0x00000080    description to come...
    kVpMaxDevForBoundary    0x00000100    description to come...
    kVpMaxDevForIsoline     0x00000200    description to come...
    kVpMaxDevForFacet       0x00000400    description to come...
    kVpCamLocation          0x00000800    description to come...
    kVpCamTarget            0x00001000    description to come...
    kVpCamUpVector          0x00002000    description to come...
    kVpCamViewDir           0x00004000    description to come...
    kVpViewport             0x00008000    description to come...
    kVpFrontBack            0x00010000    description to come...
    kVpFrozenLayers         0x00020000    description to come...
    kVpLtypeScaleMult       0x00040000    description to come...
    kEntityRegenDraw        0x00080000    description to come...
    kVpDisableLayoutCache   0x00100000    description to come...
    kVpModelTf              0x00200000    description to come...
    kVpExcludeFromExtMinMax 0x00400000    description to come...
    kSectionable            0x00800000    description to come...
    kVpRenderModule         0x01000000    description to come...
    kVpAnnoScale            0x02000000    description to come...
    kVpFilterFunction       0x04000000    description to come...
    kVpVisualStyle          0x08000000    description to come...
    kVpHighlightingGeom     0x10000000    description to come...
    </table>

    \sa

    * resetAwareFlags
  */
  OdUInt32 awareFlags() const { return m_awareFlags.m_nAwareFlags; }
  
  /** \details
    Returns the Aware flags for portion of geometry that had been vectorized
    since last call resetAwareFlags().

    \remarks
    awareFlags() returns a combination of zero or more of the following:

    <table>
    Name                    Value         Geometry_depends_on
    kVpID                   0x00000001    viewport Id (OdGiViewport::viewportId()).
    kVpRegenType            0x00000002    regeneration type (OdGiCommonDraw::regenType()).
    kVpRenderMode           0x00000004    render mode (OdGsView::mode()).
    kVpWorldToEye           0x00000008    description to come...
    kVpPerspective          0x00000010    description to come...
    kVpResolution           0x00000020    description to come...
    kVpMaxDevForCircle      0x00000040    description to come...
    kVpMaxDevForCurve       0x00000080    description to come...
    kVpMaxDevForBoundary    0x00000100    description to come...
    kVpMaxDevForIsoline     0x00000200    description to come...
    kVpMaxDevForFacet       0x00000400    description to come...
    kVpCamLocation          0x00000800    description to come...
    kVpCamTarget            0x00001000    description to come...
    kVpCamUpVector          0x00002000    description to come...
    kVpCamViewDir           0x00004000    description to come...
    kVpViewport             0x00008000    description to come...
    kVpFrontBack            0x00010000    description to come...
    kVpFrozenLayers         0x00020000    description to come...
    kVpLtypeScaleMult       0x00040000    description to come...
    kEntityRegenDraw        0x00080000    description to come...
    kVpDisableLayoutCache   0x00100000    description to come...
    kVpModelTf              0x00200000    description to come...
    kVpExcludeFromExtMinMax 0x00400000    description to come...
    kSectionable            0x00800000    description to come...
    kVpRenderModule         0x01000000    description to come...
    kVpAnnoScale            0x02000000    description to come...
    kVpFilterFunction       0x04000000    description to come...
    kVpVisualStyle          0x08000000    description to come...
    kVpHighlightingGeom     0x10000000    description to come...
    </table>

    \sa

    * resetAwareFlags
  */
  OdUInt32 &awareFlags() { return m_awareFlags.m_nAwareFlags; }

  /** \details
    Resets the Aware flags.

    \sa
    awareFlags
  */
  void resetAwareFlags()
  {
    //NS: traits processing was optimized, but it is not correct
    //Aware flags can be changed when calulating for effective traits in affectTraits(),
    //but on the other hand the flags are reset independently.
    //As a result it is possible that aware flags have no kVpFrozenLayers flag set, but
    //the traits are based on the layer (effective traits are used, unchanged from 
    //the previously processed entity.
#ifdef ENABLE_GITRAITSOPTIMIZATION
    if (m_awareFlags.getFlag(kVpFrozenLayers))
      setEntityTraitsDataChanged(); //force resetting traits
#endif
    m_awareFlags.reset();
  }
  /** \details
    Returns the effective plot style for this vectorizer object.
  */
  const OdPsPlotStyleData& effectivePlotStyle() const;

  /** \details
    Returns the render mode for this object.
       
    \returns
    OdGsView::RenderMode value which can be one of the following:
    
    <table>
    Name                            Value      Description
    kBoundingBox                    -1         Bounding box. For internal use only.
    k2DOptimized                    0          Standard display. Optimized for 2D.
    kWireframe                      1          Standard display. Uses 3D pipeline.
    kHiddenLine                     2          Wireframe display. Hidden lines removed.
    kFlatShaded                     3          Faceted display. One color per face.
    kGouraudShaded                  4          Smooth shaded display. Colors interpolated between vertices.
    kFlatShadedWithWireframe        5          Faceted display with wireframe overlay.
    kGouraudShadedWithWireframe     6          Smooth shaded display with wireframe overlay.
    </table>
    \sa
    <exref target="https://docs.opendesign.com/tv/tv_working_with_base_classes_view_functionality.html#7">Managing Render Modes</exref>
  */
  OdGsView::RenderMode getRenderMode() const;

  /** \details
    Clears cache that is used for linetypes.
  */
  void clearLinetypeCache();

  /** \details
    Enables or disables linetypes.
    
    \param bEnable [in]  Flag to enable or disable linetypes.
  */
  void setLinetypesEnabled(bool bEnable);
  
  /** \details
    Checks whether linetypes are enabled.
    
    \returns
    true if linetypes are enabled, false otherwise.
  */
  bool isLinetypesEnabled() const;

  /** \details
    Specifies whether to skip view sectioning.
    
    \param bEnable [in]  Flag that specifies whether to skip view sectioning.
  */
  void setSkipViewSectioning(bool bEnable);
  
  /** \details
    Checks whether view sectioning is skipped.
    
    \returns
    true if view sectioning is skipped, false otherwise.
  */
  bool isViewSectioningSkipped() const;

  /** \details
    Sets a model section.
    
    \param pSection               [in]  Pointer to the section to set.
    \param bUseSectionGeometryMap [in]  Flag that specifies whether to use geometry map.
  */
  void setModelSection(OdGiDrawable* pSection, bool bUseSectionGeometryMap);
  
  /** \details
    Retrieves the live section.
  */
  const OdGiDrawable* liveSection() const { return m_liveSection; }

  /** \details
    Retrieves the transformation matrix from model space to world space.
  */
  OdGeMatrix3d modelToWorldTransform() const;
  
  /** \details
    Retrieves the transformation matrix from world space to model space.
  */
  OdGeMatrix3d worldToModelTransform() const;

  /** \details
    Returns the matrix that transforms coordinates from model space to screen space for this Viewport object.

    \sa
    Coordinate Systems.
  */
  virtual OdGeMatrix3d objectToDeviceMatrix() const;

  /** \details
    Returns the number of pixels corresponding to the specified lineweight for this view object.
    \param lineweight [in]  Lineweight.
  */
  int lineweightToPixels(OdDb::LineWeight lineweight) const;
  
  /** \details
    Returns the number of pixels corresponding to the specified lineweight for this view object.
    \param lineweight [in]  Lineweight.
  */
  double lineweightToPixels(double lineweight) const;

  /** \details
    Checks whether this object has lineweight override.
    
    \returns
    true if lineweight override, false otherwise.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  bool hasLineweightOverride() const;
  
  /** \details
    Checks whether this object has line style override.
    
    \returns
    true if line style override, false otherwise.
  */
  bool hasLinestyleOverride() const;
  
  /** \details
    Checks whether line style scale multiplier is enabled.
    
    \returns
    true if line style scale multiplier is enabled, false otherwise.
  */
  bool isMultiplyLineStyleScale() const;
  
  /** \details
    Retrieves pointer to current lineweight override.
  */
  const OdGiLineweightOverride *currentLineweightOverride() const;

  /** \details
    Retrieves palette color at the specified index.
    
    \param colorIndex [in]  Index in the palette.
  */
  ODCOLORREF paletteColor(OdUInt16 colorIndex) const;
  
  /** \details
    Retrieves palette color at the specified index.
    
    \param colorIndex [in]  Color index.
  */
  ODCOLORREF paletteColor(OdInt16 colorIndex) const { return paletteColor(OdUInt16(colorIndex)); }
  
  /** \details
    Returns the logical palette of this object.
    
    \param pNumColors [out]  Returns number of colors inside logical palette.
  */
  const ODCOLORREF *getPalette(int *pNumColors = NULL) const;
  
  /** \details
    Checks whether this object has palette override.
    
    \returns
    true if palette override is enabled, false otherwise.
  */
  bool hasPaletteOverride() const { return !m_paletteOverrideStack.empty(); }

  /** OdGiViewport methods **/
  
  /** \details
    Returns the transformation matrix from ModelSpace to EyeSpace for this object.
  */
  virtual OdGeMatrix3d getModelToEyeTransform() const;
  
  /** \details
    Returns the transformation matrix from EyeSpace to ModelSpace for this object.
  */
  virtual OdGeMatrix3d getEyeToModelTransform() const;
  
  /** \details
    Returns the transformation matrix from WCS to EyeSpace for this object.
  */
  virtual OdGeMatrix3d getWorldToEyeTransform() const;
  
  /** \details
    Returns the transformation matrix from EyeSpace to WCS for this object.
  */
  virtual OdGeMatrix3d getEyeToWorldTransform() const;

  /** \details
    Retrieves model-to-world coordinate transform which is inverse of the retrieved matrix from the getModelToWorldTransform() method. 
    The retrieved matrix can transform an entity's world coordinate representation back into the original model coordinates.
  */
  virtual OdGeMatrix3d getWorldToModelTransform() const;
  
  /** \details
    Retrieves the block insert transform, which is the model-to-world coordinate transform.
  */
  virtual OdGeMatrix3d getModelToWorldTransform() const;

  /** \details
    Checks whether perspective mode is turned on for this vectorizer.
    \returns
    true if perspective mode is on for this vectorizer, false otherwise.
  */
  virtual bool isPerspective() const;
  
  /** \details
    Applies the current perspective transformation to the specified point.

    \param point [in/out] Any 3D point.
    \remarks
    The point is transformed from EyeSpace coordinates to normalized device coordinates.
    \returns 
    true if the point was transformed; i.e., a perspective transform
    is active, and the point was neither too close or behind the camera.
  */
  virtual bool doPerspective(OdGePoint3d& point) const;
  
  /** \details
    Applies the inverse of the current perspective transformation to the specified point.

    \param point [in/out] Any 3D point.
    \remarks
    The point is transformed from normalized device coordinates to EyeSpace coordinates
    \returns 
    true the point was transformed; i.e., a perspective transform
    is active, and the point was neither too close or behind the camera.
  */
  virtual bool doInversePerspective(OdGePoint3d& point) const;
  
  /** \details
    Returns the display pixel density at the specified point for this vectorizer.

    \param point [in]  WCS center of the unit square.
    \param pixelDensity [out]  Receives the pixel density.
    \param bUsePerspective [in]  Enable perspective inclusion into pixel density calculation.
    \remarks
    Pixel density is measured in pixels per WCS unit.
    This function can be used to determine if the geometry generated for an object will 
    be smaller than the size of a pixel.
  */
  virtual void getNumPixelsInUnitSquare(
    const OdGePoint3d& point, 
    OdGePoint2d& pixelDensity,
    bool includePerspective = true) const;

  
  /** \details
    Returns the WCS camera (eye) location for this vectorizer.
  */
  virtual OdGePoint3d getCameraLocation() const;
  
  /** \details
    Returns the WCS camera target for this vectorizer.
  */
  virtual OdGePoint3d getCameraTarget() const;
  
  /** \details
    Returns the WCS camera "up" vector for this vectorizer.
  */
  virtual OdGeVector3d getCameraUpVector() const;
  
  
  /** \details
    Returns the vector from the camera target to the camera location.
  */
  virtual OdGeVector3d viewDir() const;
  
  /** \details
    Returns the OdGi Viewport ID for this vectorizer.
    \remarks
    The value returned has no relationship to the CVPORT system variable.
  */
  virtual OdUInt32 viewportId() const;
  
  /** \details
    Returns the ID for this vectorizer window.
    \remarks
    The value returned corresponds to the CVPORT system variable.
  */
  virtual OdInt16 acadWindowId() const;
  
  /** \details
    Returns lower-left and upper-right corners of this vectorizer in 
    normalized device Coordinates.

    \param lowerLeft [out]  Receives the lower-left corner.
    \param upperRight [out]  Receives the upper-right.
    \remarks
    This information lets position items that are fixed in size and/or position
    with respect to a Viewport.     
  */
  virtual void getViewportDcCorners(
    OdGePoint2d& lowerLeft, 
    OdGePoint2d& upperRight) const;
  
  /** \details
    Returns the front and back clipping parameters for this vectorizer.

    \param clipFront [out]  Receives true if and only if front clipping is enabled.
    \param clipBack [out]   Receives true if and only if back clipping is enabled.
    \param front [out]  Receives the front clipping distance.
    \param back [out]  Receives the back clipping distance.
    \returns 
    true only if both front clipping and back clipping is enabled, false otherwise.
    \remarks
    front and back are the eye coordinate Z values. 
    Clipping planes are perpendicular to the view direction.
    If the front clipping plane is enabled, geometry in front of it is not displayed.
    If the back clipping plane is enabled, geometry in back of it is not displayed.
  */
  virtual bool getFrontAndBackClipValues(
    bool& clipFront, 
    bool& clipBack, 
    double& front, 
    double& back) const;
  
  /** \details
    Returns a multiplier that is used to scale all linetypes in this vectorizer.
  */
  virtual double linetypeScaleMultiplier() const;
  
  /** \details
    Returns the WCS value below which linetype generation is not used.

    \remarks
    If the WCS length an entire linetype pattern is less than the returned value, then
    the linetype will be rendered as continuous.
  */
  virtual double linetypeGenerationCriteria() const;
  
  /** \details
    Checks whether the specified layer is not frozen either globally or in the associated Viewport of this vectorizer.

    \param layerId [in]  Object ID of OdLayerTableRecord.
    \returns
    true the specified layer is not frozen either globally or in the associated Viewport of this vectorizer, false otherwise.
  */
  virtual bool layerVisible(
    OdDbStub* layerId) const;  

  /** \details
    Returns pointer to context-dependent colors storage.

    \remarks
    This pointer could be null if contextual colors doesn't specified by context.
  */
  const OdGiContextualColors *contextualColors() const;

  /** OdGiGeometry methods **/

  /** \details
    Pushes a transformation vector onto the stack. The resulting transformation will be created by using the arbitrary axis algorithm and the specified vector.
    This allows you to easily adjust an entity coordinate system. Use the popModelTransform() to remove this transform vector from the stack when you are finished with this transformation.

    \param normal [in]  Transformation matrix.
  */
  virtual void pushModelTransform(const OdGeVector3d& normal);
  
  /** \details
    Pushes a transformation matrix onto the stack. The resulting transformation will be created by multiplying the specified transform matrix by the existing one.
    This allows you to easily adjust an entity coordinate system. Use the popModelTransform() to remove this transform matrix from the stack when you are finished with this transformation.

    \param xfm [in]  Transformation matrix.
  */
  virtual void pushModelTransform(const OdGeMatrix3d & xfm);
  
  /** \details
    Pops a transformation from the stack. This method should be used after a successful pushModelTransform() method call to restore previous model transform state.
  */
  virtual void popModelTransform();
  
  /** \details
    Introduces an xline into this vectorization context.
    
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \sa
    <link tv_geometry_lines.html, Work with Lines>
  */
  virtual void xline(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);
  
  /** \details
    Introduces a ray into this vectorization context.
    
    \param basePoint [in]  Base point.
    \param throughPoint [in]  Through point
    \sa
    <link tv_geometry_lines.html, Work with Lines>
  */
  virtual void ray(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint);
  
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
    <link tv_geometry_shells.html, Work with Shells>
  */
  virtual void shell(OdInt32 numVertices, const OdGePoint3d* vertexList, OdInt32 faceListSize, const OdInt32* faceList,
                     const OdGiEdgeData* pEdgeData = 0, const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0);
  
  /** \details
    Introduces a mesh into this vectorization context.

    \param numRows [in]  Number of rows.
    \param numColumns [in]  Number of columns.
    \param vertexList [in]  Array of vertices.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data. 
    \sa
    <link tv_geometry_meshes.html, Work with Meshes>
  */
  virtual void mesh(OdInt32 numRows, OdInt32 numColumns, const OdGePoint3d* vertexList,
                    const OdGiEdgeData* pEdgeData = 0, const OdGiFaceData* pFaceData = 0, const OdGiVertexData* pVertexData = 0);

  /** OdGiWorldGeometry methods **/

  /** \details
    Sets geometrical extents for this vectorizer.
    
    \param newExtents [in]  New geometrical extents to set.
  */
  virtual void setExtents(const OdGePoint3d *newExtents);

  /** OdGiCommonDraw methods **/

  /** \details
    Returns the recommended maximum deviation of the current vectorization, for the specified point on the curve or surface being tesselated.
    \sa
    OdGiGeometrySimplifier::setDeviation
    \param deviationType [in]  Deviation type.
    \param pointOnCurve [in]  Point on the curve.
    
    \returns
    recommended maximum deviation of the current vectorization as double value.
    
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
  */
  virtual double deviation(const OdGiDeviationType deviationType,
      const OdGePoint3d& pointOnCurve) const;
  
  /** \details
    Retrieves the regeneration type of the current vectorization process.

    \returns
    one of the following regeneration types:
    
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
  virtual OdGiRegenType regenType() const;
  
  /** OdGiViewportDraw methods **/

  /** \details
    Returns the number of viewports that have been deleted from the current database 
    during this editing session.
  */
  virtual OdUInt32 sequenceNumber() const;
  
  /** \details
    Checks whether the specified Viewport ID is a valid Viewport ID.
    
    \param viewportId [in]  Viewport ID.
    \returns 
    true if the specified Viewport ID is a valid Viewport ID, false otherwise.
    \remarks
    viewportId is typically obtained from viewport().viewportId().
  */
  virtual bool isValidId(const OdUInt32 viewportId) const;
  
  /** \details
    Returns the Object ID of the OdDbViewport object associated with this object.
  */
  virtual OdDbStub* viewportObjectId() const;
  
  /** \details
    Returns a reference to the OdGiViewport object associated with this object.
    
    \remarks
    The OdGiViewport contains information specific to the viewport being drawn.
  */
  virtual OdGiViewport& viewport() const;

  /** OdGiSubEntityTraits methods **/

  /** \details
    Sets the current fill plane.
    
    \param pNormal [in]  Pointer to the normal to the plane.
  */
  virtual void setFillPlane(const OdGeVector3d* pNormal = 0);
  
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
  */
  virtual void setFillType(OdGiFillType fillType);
  
  /** \details
    Retrieves an identifier of the visual style.
    
    \returns
    Pointer to the OdDbStub that represents a visual style identifier.
    \sa
    <link tv_working_with_attributes_visual_styles.html, Work with Visual Styles>
  */
  virtual OdDbStub* visualStyle() const;
  
  /** \details
    Retrieves the set of SubEntityHints flags which can be used to provide more flexible tuning of
    subSetAttributes traits processing.
    
    \returns
    SubEntityHints flags as the OdUInt32 value.
  */
  virtual OdUInt32 setupForEntity() const;
  
  /** \details
    Pushes lineweight scale and style override into lineweight overrides stack.
    \param pOverride [in]  New lineweight override.
    \returns
    true in case if lineweight overrides supported by underlying vectorization pipeline.
  */
  virtual bool pushLineweightOverride(const OdGiLineweightOverride *pOverride);
  
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
    Checks whether this vectorizer has non-empty palette overrides stack.
    \returns
    true if this vectorizer has palette overrides, false otherwise.
  */
  virtual bool hasPaletteOverrides() const;

  /** OdGiConveyorContext methods **/

  /** \details
    Retrieves gi viewport associated with this vectorizer.
  */
  virtual const OdGiViewport* giViewport() const;
  
  /** \details
    Retrieves gs view associated with this object.
    \sa
    <link tv_working_with_base_classes_view.html, Work with Visualize Graphical Views>
  */
  virtual const OdGsView* gsView() const;
  
  /** \details
    Retrieves current annotationScale value of conveyor context.
  */
  virtual double annotationScale() const;

  /** OdGiBaseVectorizer methods **/

  /** \details
    Begins the ViewVectorization process for this Vectorizer object.
  */
  virtual void beginViewVectorization();
  
  /** \details
    Ends the ViewVectorization process for this Vectorizer object.
  */
  virtual void endViewVectorization();
  
  /** \details
    This method can be called whenever entity traits are changed.
  */
  virtual void onTraitsModified();

  /** \details
    Retrieves the EntityTraitsData in effect for this Vectorization object.
    
    \returns
    entity traits that are in effect for this vectorizer.
  */
  virtual const OdGiSubEntityTraitsData& effectiveTraits() const;

  /** \details
    begin/endVectorization() needs an argument indicating a mode, but introducing such argument causes changes in all derived classes.
    Hence it is done by means of m_vectorizingFlags, it is set before begin/endVectorization() is called.
  */
  enum VectorizingFlags
  {
    kNotVectorizing = 0,   // Intemediate process (not vectorizing)
    kVectorizing = 1 << 0, // Vectorizing
    kDisplaying  = 1 << 1, // Displaying
    kSelecting   = 1 << 2, // Selection
    kExtentsComp = 1 << 3  // Extents computation
  };

  /** \details
    Returns a set of vectorization flags.
  */
  int vectorizingFlags() const { return m_vectorizingFlags; }
  
  /** \details
    Sets vectorization flags.
    
    \param flags [in]  New palette override.
  */
  void setVectorizingFlags(int flags) { m_vectorizingFlags = flags; }
protected:
  /** \details
    Returns true if and only if this VectorizeView object is configured to use a fill plane.
  */
  bool useFillPlane() const;

  /** \details
    Returns true in case if vectorizer execute vectorization process, which is not related to graphics rendering:
    Extents computation or selection process.
  */
  inline bool isOutputSuppressed() const;

  /** \details
    Returns true if and only if the effective linetype generation is continuous for vectorizer object.
  */
  bool isEffectiveLinetypeContinuous() const;

  /** \details
    Returns true in case if Plot Styles support enabled for this Vectorizer object.
  */
  bool isSupportPlotStyles() const;

  /** OdGiBaseVectorizer methods **/

  /** \details
    Affects effective sub-entity traits.
    
    \param pSource [in]  Source sub-entity traits data.
    \param destination [in]  Destination sub-entity traits data.
  */
  virtual void affectTraits(const OdGiSubEntityTraitsData* pSource, OdGiSubEntityTraitsData& destination) const;

  /** \details
    Returns lineweight, multiplied by active lineweight to pixels override.

    \param lineweight [in]  Input lineweight value in millimeters.
    \param bAbsolute [in]  If set as true, will multiply onto scale override, or onto pixel scale elsewhere.
  */
  double lineweightToPixelsOverride(OdDb::LineWeight lineweight, bool bAbsolute = false) const;
  /** \details
    Returns lineweight, multiplied by active lineweight to pixels override and converted into integer value.

    \param lineweight [in]  Input lineweight value in millimeters.
  */
  int lineweightToPixelsOverrideInt(OdDb::LineWeight lineweight) const;
  /** \details
    Returns lineweight, multiplied by active lineweight to pixels override.

    \param lineweight [in]  Input lineweight as floating point value.
    \param bAbsolute [in]  If set as true, will multiply onto scale override, or onto pixel scale elsewhere.
  */
  double lineweightToPixelsOverride(double lineweight, bool bAbsolute = false) const;

  /** \details
    Updates internal flags, related to current lineweights override state.

    \param lwdOverride [in]  Current lineweights override state.
  */
  virtual void updateLineweightOverride(const OdGiLineweightOverride &lwdOverride);
  /** \details
    Updates internal data, related to current palette override state. Default implementation does nothing.

    \param pPalette [in]  Current palette override state.
  */
  virtual void updatePaletteOverride(const OdGiPalette *pPalette);
private:
  /** \details
    Returns active Plot Style data.
  */
  const OdPsPlotStyleData& plotStyle() const;

  /** \details
    Affects part of effective sub-entity traits data, which is related to fill plane and lineweights support.
    
    \param pFrom [in]  Source sub-entity traits data.
    \param ton [in]  Destination sub-entity traits data.
  */
  void affect2dTraits(const OdGiSubEntityTraitsData* pFrom, OdGiSubEntityTraitsData& to) const;

  /** \details
    Updates the Xline and Ray clipper prior to rendering each Xline and Ray.
    \remarks
    Called by ray() and xline().
  */
  void updateXlineNRayClipper();
  /** \details
    Updates underlying transforms and transformation matrices, which is depend from modelToEye transformation.
  */
  void onModelToEyeChanged();
protected:
  /** \details
    Structure represents container and handling methods for Aware-flags and related members.
  */
  mutable struct AwareFlagsHandler
  {
    OdUInt32 m_nAwareFlags; // Active Aware-flags accumulation register.
    OdUInt32 m_nAffectAwareFlags; // Aware-flags state, accumulated during last affectTraits() call (ref. #CORE-16731).
    /** \details
      Default constructor. Resets all Aware-flags related registers.
    */
    AwareFlagsHandler() : m_nAwareFlags(0), m_nAffectAwareFlags(0) { }
    /** \details
      Returns requested Aware-flag state.

      \param nFlag [in]  Requested Aware-flag.
    */
    bool getFlag(OdUInt32 nFlag) const { return GETBIT(m_nAwareFlags, nFlag); }
    /** \details
      Sets specified Aware-flag to enabled state.

      \param nFlag [in]  Aware-flag.
    */
    void setFlag(OdUInt32 nFlag) { SETBIT_1(m_nAwareFlags, nFlag); }
    /** \details
      Sets Aware-flag to specified state.

      \param nFlag [in]  Aware-flag.
      \param bSet [in]  New flag state.
    */
    void setFlag(OdUInt32 nFlag, bool bSet) { SETBIT(m_nAwareFlags, nFlag, bSet); }
    /** \details
      Swaps active Aware-flags with Aware-flags, calculated during last affectTraits() call and
      enable specified Aware-flag in active Aware-flags register.

      \param nFlag [in]  Aware-flag to enable in active Aware-flags register after swapping.
    */
    void swapAndSet(OdUInt32 nFlag) { swapFlags(); setFlag(nFlag); }
    /** \details
      Swaps active Aware-flags with Aware-flags, calculated during last affectTraits() call.
    */
    void swapFlags() { std::swap(m_nAwareFlags, m_nAffectAwareFlags); }
    /** \details
      Swaps active Aware-flags with Aware-flags, calculated during last affectTraits() call and
      reset active Aware-flags register.
    */
    void swapAndReset() { swapFlags(); reset(); }
    /** \details
      Merge active Aware-flags with Aware-flags, calculated during last affectTraits() call.
    */
    void mergeFlags() { m_nAwareFlags |= m_nAffectAwareFlags; }
    /** \details
      Swaps active Aware-flags with Aware-flags, calculated during last affectTraits() call and
      merge them together into active Aware-flags register.
    */
    void swapAndMerge() { swapFlags(); mergeFlags(); }
    /** \details
      Resets active Aware-flags register.
    */
    void reset() { m_nAwareFlags = 0; }
  } m_awareFlags;
  OdUInt32                    m_implFlags;
  OdGeVector3d                m_fillPlane;
  OdGiContext::PStyleType     m_pstype;
  mutable OdPsPlotStyleData   m_plotStyle;
  OdUInt32                    m_setAttributesHints;
  int                         m_vectorizingFlags;
  OdGiOrthoClipperPtr         m_pXlineNRayClipper;
  OdGiConveyorEntryPoint      m_xlineNRayEntryPoint;
  OdGiLinetypeRedirPtr        m_pLinetyper;
  OdGiConveyorEmbranchmentPtr m_pOutputBranch;
  OdRxObjectPtr               m_pGsReserved; // Reserved pointer for internal use (binary incompatible changes), do not delete.
  OdGiDrawablePtr             m_liveSection;
  OdRxObjectPtr               m_sectionGeometryMap;
  OdCmTransparency            m_sectForegroundFace;
  OdCmTransparency            m_sectForegroundEdge;

  OdGiXformPtr                m_pXToLtp;

private:
  OdGsViewImpl*               m_view;
  OdGiConveyorConnector       m_ltpEntryPoint;

  mutable int                 m_nPenIndex;
  mutable OdDbStub*           m_psnId;
  mutable OdPsPlotStyleData   m_effectivePlotStyle;

  OdArray<OdGiLineweightOverride, OdMemoryAllocator<OdGiLineweightOverride> > m_lwdOverrideStack; // Lineweight overrides stack
  OdGiStack<OdGiPalettePtr>   m_paletteOverrideStack; // Palette overrides stack
protected:
  mutable OdGeMatrix3d        m_objectToDeviceMatrix;
protected:
  enum Flags
  {
    kFillPlaneSet               = OdGiBaseVectorizer::kLastFlag <<  1,
    kLinetypeContinuous         = OdGiBaseVectorizer::kLastFlag <<  2,
    kModelCache                 = OdGiBaseVectorizer::kLastFlag <<  3,
    kHighlighted                = OdGiBaseVectorizer::kLastFlag <<  4,
    kDontAbortRegen             = OdGiBaseVectorizer::kLastFlag <<  5,
    kCheckLinePattern           = OdGiBaseVectorizer::kLastFlag <<  6,
    kBreakSelection             = OdGiBaseVectorizer::kLastFlag <<  7,
    kSelectionGeometry          = OdGiBaseVectorizer::kLastFlag <<  8, // Processing geometry which is intended only for selection
    kSelectionDisabled          = OdGiBaseVectorizer::kLastFlag <<  9, // Processing geometry which shouldn't be used for selection
    kObjectToDeviceValid        = OdGiBaseVectorizer::kLastFlag <<  10,
    kDisableInfiniteGeomExtents = OdGiBaseVectorizer::kLastFlag <<  11,
    kSectionableGeomExtentsOnly = OdGiBaseVectorizer::kLastFlag <<  12,
    kDisableSection             = OdGiBaseVectorizer::kLastFlag <<  13,
    kSectionForegroundGeometry  = OdGiBaseVectorizer::kLastFlag <<  14,
    kInitGsState                = OdGiBaseVectorizer::kLastFlag <<  15,
    kSetVpLtypeScaleMult        = OdGiBaseVectorizer::kLastFlag <<  16,
    kSetVpFilterFunction        = OdGiBaseVectorizer::kLastFlag <<  17,
    kLineweightOverride         = OdGiBaseVectorizer::kLastFlag <<  18,
    kLinestyleOverride          = OdGiBaseVectorizer::kLastFlag <<  19,
    kLastOptionFlag             = kLinestyleOverride
    // Note: m_flags and Flags enum bits is nearly complete utilization. Please don't add new flags here.
    //       use m_implFlags and ImplFlags enum instead.
  };
  enum ImplFlags
  {
    kFirstImplFlag              = 1,
    kSectionableGeomEnabled     = kFirstImplFlag << 0,
    kMultiplyLinestyleScale     = kFirstImplFlag << 1,
    kSkipViewSectioning         = kFirstImplFlag << 2,
    /** Geometry hidden flag */
    kHiddenImplFlag             = kFirstImplFlag << 3,
    kLastImplFlag               = kHiddenImplFlag
  };
};

inline bool OdGiBaseVectorizerImpl::isOutputSuppressed() const
{
  return GETBIT(m_vectorizingFlags, kExtentsComp | kSelecting);
}

inline void OdGiBaseVectorizerImpl::setLinetypesEnabled(bool bEnable)
{
  if (bEnable) m_pLinetyper->enable();
  else         m_pLinetyper->disable();
}

inline bool OdGiBaseVectorizerImpl::isLinetypesEnabled() const
{
  return m_pLinetyper->enabled();
}

inline void OdGiBaseVectorizerImpl::setSkipViewSectioning(bool bEnable)
{
  SETBIT(m_implFlags, kSkipViewSectioning, bEnable);
}

inline bool OdGiBaseVectorizerImpl::isViewSectioningSkipped() const
{
  return GETBIT(m_implFlags, kSkipViewSectioning);
}

inline bool OdGiBaseVectorizerImpl::isSupportPlotStyles() const
{
  return (m_pstype != OdGiContext::kPsNone) && GETBIT(m_setAttributesHints, kHint2DAttributes);
}

inline int OdGiBaseVectorizerImpl::lineweightToPixels(OdDb::LineWeight lineweight) const
{
  if (!GETBIT(m_flags, OdGiBaseVectorizerImpl::kLineweightOverride))
    return view().lineweightToPixels(lineweight);
  return lineweightToPixelsOverrideInt(lineweight);
}

inline double OdGiBaseVectorizerImpl::lineweightToPixels(double lineweight) const
{
  if (!GETBIT(m_flags, OdGiBaseVectorizerImpl::kLineweightOverride))
    return view().lineweightToPixels(lineweight);
  return lineweightToPixelsOverride(lineweight);
}

inline double OdGiBaseVectorizerImpl::lineweightToPixelsOverride(OdDb::LineWeight lineweight, bool bAbsolute) const
{
  return lineweightToPixelsOverride(double(lineweight) / 100.0, bAbsolute);
}

inline const OdGiLineweightOverride *OdGiBaseVectorizerImpl::currentLineweightOverride() const
{
  if (GETBIT(m_flags, kLineweightOverride | kLinestyleOverride))
    return  &m_lwdOverrideStack.last();
  return NULL;
}

inline bool OdGiBaseVectorizerImpl::hasLineweightOverride() const
{
  return GETBIT(m_flags, OdGiBaseVectorizerImpl::kLineweightOverride);
}

inline bool OdGiBaseVectorizerImpl::hasLinestyleOverride() const
{
  return GETBIT(m_flags, OdGiBaseVectorizerImpl::kLinestyleOverride);
}

inline bool OdGiBaseVectorizerImpl::isMultiplyLineStyleScale() const
{
  return GETBIT(m_implFlags, OdGiBaseVectorizerImpl::kMultiplyLinestyleScale);
}

#include "TD_PackPop.h"

#endif // __OD_GI_BASE_VECTORIZER_IMPL_H_
