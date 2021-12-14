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

#ifndef __OD_GS_BASE_VECTORIZER_H_
#define __OD_GS_BASE_VECTORIZER_H_

#include "TD_PackPush.h"

#include "Gs/GsExport.h"
#include "Gs/GiBaseVectorizerImpl.h"
#include "Gs/GsExtAccum.h"
#include "Gs/GsModel.h"
#include "GsMarkerArray.h"
#include "Gs/GsHighlightData.h"

#define STL_USING_SET
#include <set>

class OdGsBaseModel;
class OdGsLayerNode;
class OdGsUpdateContext;
class OdGiSelectProc;
class OdGiCollideProc;
class OdGsEntityNode;
class OdGsBlockReferenceNode;
class OdGsBlockReferenceNodeImpl;
struct OdGsGeomPortion;
class OdGsMaterialNode;
class OdGsBaseVectorizer;
class OdGsBaseContext;
class OdGsDisplayContext;
struct OdSiShape;

class OdGsMtContext;
class OdGsUpdateState;
class OdGsNestedMetafile;
class OdGiSharedRefDesc;
class OdGiHistory;
class OdGiSectionGeometry;
class OdGsApplyModelOverrides;

class OdPerfTimerBase;


typedef std::set<OdGsMarker> OdGsMarkerSet;
typedef OdSharedPtr< OdGsApplyModelOverrides > OdGsApplyModelOverridesPtr;

/** \details

  Corresponding C++ library: TD_Gs

  <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsWriter
{
public:
  /** \details
    Default constructor for objects of the OdGsWriter class.
    Sets the following data members:
    <table>
    Data member            Value                          Description
    m_pGsModel             NULL                           Pointer to a model.
    m_pGeomPortionHead     NULL                           Pointer to a geometry portion head.
    m_pGeomPortion         NULL                           Pointer to a geometry portion.
    m_vectorizer           NULL                           Vectorizer in use.
    m_bFinalizedMetafile   false                          Flag that shows whether there is currently no recording process of a metafile.
    m_pExtAccum            OdGsExtAccum::createObject()   Accumulated extents.
    </table>
  */
  OdGsWriter();

  /** \details
    Returns the accumulated extents when writing to the object.
  */
  OdGsExtAccum& extentsAccum()
  {
    return *m_pExtAccum.get();
  }

  /** \details
    Returns pointer to GS model.
  */
  OdGsBaseModel* gsModel() const { return m_pGsModel; }

  /** \details
    Checks whether metafile recording is active.
    \returns
    True if metafile is currently recording, false otherwise.
  */
  bool isRecordingMetafile() { return m_pGeomPortion != 0; }

  /** \details
    Retrieves pointer to current geometry portion.
  */
  OdGsGeomPortion *currentGeomPortion() const { return m_pGeomPortion; }

  /** \details
    Retrieves pointer to head geometry portion.
  */
  OdGsGeomPortion *headGeomPortion() const { return m_pGeomPortionHead; }

  /** \details
    Begins metafile recording.
    \param pGeomPortion [in]  Pointer to geometry portion.
  */
  void beginMetafileRecording(OdGsGeomPortion* pGeomPortion) { m_pGeomPortionHead = m_pGeomPortion = pGeomPortion; }

  /** \details
    Ends metafile recording.
  */
  void endMetafileRecording();
  void onNestedDrawable(OdDbStub* layerId, const OdGsNestedMetafile *pNestedMf = NULL);
  void separateMetafile();

  /** \details
    Checks whether the specified layer node is frozen.
    \param pLayerNode [in]  Layer node pointer for check.
    \returns
    True if the layer node is frozen, false otherwise.
  */
  bool isLayerFrozen(OdGsLayerNode *pLayerNode) const;

  /** \details
    Checks whether the specified layer node is already invoked in geometry portions list of active metafile.
    \param pLayerNode [in]  Layer node pointer for check.
    \param pTillPortion [in]  Geometry portion pointer till which metafile geometry portions will be checked.
    \returns
    True if the layer node is available in metafile geometry portions list, false otherwise.
  */
  bool hasFrozenLayerBefore(OdGsLayerNode *pLayerNode, const OdGsGeomPortion *pTillPortion = NULL) const;
private:
  //DOM-IGNORE-BEGIN
  friend class OdGsBaseVectorizer;
  void set(OdGsBaseVectorizer* v) { m_vectorizer = v; }
  void onLayerModified(OdDbStub* layerId);
  void onLayerModified(OdDbStub* layerId, bool bFrozen);
  void onFrozenLayerModified(OdDbStub* layerId) const;

public:
  OdGsBaseModel* m_pGsModel;

protected:
  bool isCurrentGeomPortionDiscardable();
protected:
  OdGsExtAccumPtr m_pExtAccum;
  OdGsGeomPortion* m_pGeomPortionHead;
  OdGsGeomPortion* m_pGeomPortion;
  OdGsBaseVectorizer* m_vectorizer;
  bool m_bFinalizedMetafile;
  //DOM-IGNORE-END
};

class OdGiPointLightTraitsData;
class OdGiSpotLightTraitsData;
class OdGiDistantLightTraitsData;
class OdGiWebLightTraitsData;

/** \details

  Corresponding C++ library: TD_Gs

  <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsBaseVectorizer: public OdGiBaseVectorizerImpl
{
public:
  /** \details
    Default constructor for objects of the OdGsBaseVectorizer class.
  */
  OdGsBaseVectorizer();

protected:
  //DOM-IGNORE-BEGIN
  ODRX_USING_HEAP_OPERATORS(OdGiBaseVectorizer);
  //DOM-IGNORE-END

public:  
#ifdef SWIG
  /** \details
    Draws the specified drawable.
    \param pDrawable [in]  Pointer to a drawable to draw.
  */
  virtual void draw(const OdGiDrawable* pDrawable) {draw(pDrawable);};
#endif
  /** \details
    Retrieves the writer associated with the vectorizer.
  */
  OdGsWriter& gsWriter() { return m_gsWriter; }

  /** \details
    Retrieves the writer associated with the vectorizer.
  */
  const OdGsWriter& gsWriter() const { return m_gsWriter; }

  /** \details
    Returns the accumulated extents when writing to the object.
  */
  OdGsExtAccum& gsExtentsAccum() { return gsWriter().extentsAccum(); }

  /** \details
    Creates a new GsMetafile (cache) object.
    \returns
    Returns a SmartPointer to the newly created object.
    \remarks
    GsMetafiles are used to record vectorizations, and rapidly 'redraw' them to
    a display device.

    \sa
    * beginMetafile
    * endMetafile
    * playMetafile
  */
  virtual OdRxObjectPtr newGsMetafile();

  /** \details
    Begins vectorization to the specified GsMetafile (cache) object.
    \param pMetafile [in]  Pointer to the GsMetafile.
    \remarks
    GsMetafiles are used to record vectorizations, and rapidly 'redraw' them to
    a display device.

    \sa
    * endMetafile
    * newGsMetafile
    * playMetafile
  */
  virtual void beginMetafile(OdRxObject* pMetafile);

  /** \details
    Ends vectorization to the specified GsMetafile (cache) object.
    \param pMetafile [in]  Pointer to the GsMetafile.
    \remarks
    GsMetafiles are used to record vectorizations, and rapidly 'redraw' them to
    a display device.

    \sa
    * beginMetafile
    * newGsMetafile
    * playMetafile
  */
  virtual void endMetafile(OdRxObject* pMetafile);

  /** \details
    Plays (redraws) the specified Metafile (cache) object.
    \param pMetafile [in]  Pointer to the GsMetafile.
    \remarks
    GsMetafiles are used to record vectorizations, and rapidly 'redraw' them to
    a display device.
    \sa
    * beginMetafile
    * endMetafile
    * newGsMetafile
  */
  virtual void playMetafile(const OdRxObject* pMetafile);

  /** \details
    Stores the specified Metafile (cache) into OdGsFiler object.
    \param pMetafile [in]  Pointer to the GsMetafile.
    \param pFiler    [in]  Pointer to the OdGsFiler object.
  */
  virtual bool saveMetafile(const OdRxObject* pMetafile, OdGsFiler *pFiler);

  /** \details
    Loads the Metafile (cache) from OdGsFiler object.
    \param pFiler [in]  Pointer to the OdGsFiler object.
  */
  virtual OdRxObjectPtr loadMetafile(OdGsFiler *pFiler);

  /** \details
	This method can be called right after endMetafile() to check whether the
	last recorded metafile was empty and optimize stored data.
  */
  bool isMetafileEmpty() const { return m_nMetafileSize == 0; }
  
  /** \details
    This method can be called right after endMetafile() to obtain
    metafile size for Update Manager filtration.
  */
  OdUInt64 lastMetafileSize() const 
  {
    if( m_nMetafileSize != OdUInt64( -1 ) ) return m_nMetafileSize;
    return 0;
  }

  /** \details
    Sets viewport's common data before display() call.
  */
  virtual void loadViewport();

  virtual bool forceMetafilesDependence() const;

  /** \details
    Checks whether the view is regenerated.
    \returns
    True if view is regenerated or false if view is not regenerated or if
    vectorizer output is suppressed (during extents computation or selection
    procedure).
  */
  virtual bool isViewRegenerated() const;

  /** \details
    Draws the frame (border) for the viewport.
  */
  virtual void drawViewportFrame();

  /** \details
    Updates the viewport (loads viewport traits, updates extents(if necessary),
    displays associated drawables and draws viewport frame).
  */
  virtual void updateViewport();


  // Materials support

  /** \details
    Processes a material node.

    \param materialId [in]  Persistent material identifier.
    \param pNode      [in]  Pointer to a material node.
  */
  virtual void processMaterialNode(OdDbStub *materialId, OdGsMaterialNode *pNode);
  
  /** \details
    Saves material cache to the specified filer.
    \param pNode  [in]  Pointer to a material node.
    \param pFiler [in]  Pointer to a filer where to save the material item.
    \returns
    True if material cache is successfully saved, false otherwise.
  */
  virtual bool saveMaterialCache(const OdGsMaterialNode *pNode, OdGsFiler *pFiler);

  /** \details
    Loads material cache from the specified filer.
    \param pNode  [in]  Pointer to a material node.
    \param pFiler [in]  Pointer to a filer where to load the material item from.
    \returns
    True if material cache is successfully loaded, false otherwise.
  */
  virtual bool loadMaterialCache(OdGsMaterialNode *pNode, OdGsFiler *pFiler);

  /** \details
    Adds a point light source. Must be implemented in derived classes.
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  virtual void addPointLight(const OdGiPointLightTraitsData&) {}

  /** \details
    Adds a spot light source. Must be implemented in derived classes.
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  virtual void addSpotLight(const OdGiSpotLightTraitsData&) {}

  /** \details
    Adds a distant light source. Must be implemented in derived classes.
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  virtual void addDistantLight(const OdGiDistantLightTraitsData&) {}

  /** \details
    Adds a web light source. Must be implemented in derived classes.
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  virtual void addWebLight(const OdGiWebLightTraitsData&);
  // default implementation call's addPointLight

  /** \details
    Destructor for objects of the OdGsBaseVectorizer class. Deletes the recorded
    history and timing, if exists.
  */
  ~OdGsBaseVectorizer();

  /** OdGiGeometry Overrides *
  */

  /** \details
    Pushes a transformation matrix onto the stack. The resulting transformation
    will be created by multiplying the specified transform matrix by the
    existing one. This allows you to easily adjust an entity coordinate system.
    Use the popModelTransform() to remove this transform matrix from the stack
    when you are finished with this transformation.

    \param xfm [in]  Transformation matrix.
  */
  void pushModelTransform(const OdGeMatrix3d& xfm);

  /** \details
    Pushes a transformation vector onto the stack. The resulting transformation
    will be created by using the arbitrary axis algorithm and the specified
    vector. This allows you to easily adjust an entity coordinate system. Use
    the popModelTransform() to remove this transform vector from the stack when
    you are finished with this transformation.

    \param normal [in]  Transformation matrix.
  */
  void pushModelTransform(const OdGeVector3d& normal);

  /** \details
    Pops a transformation from the stack. This method should be used after a
    successful pushModelTransform() method call to restore previous model
    transform state.
  */
  void popModelTransform();

  /** \details
    Pushes a clipping boundary onto the stack, which can possibly affect the
    display of subsequent primitives. Use the popClipBoundary() to remove this
    clipping boundary from the stack.

    \param pBoundary [in]  Pointer to a clipping boundary.
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
  */
  void pushClipBoundary(OdGiClipBoundary* pBoundary);

  /** \details
    Pushes a clipping boundary onto the stack, which can possibly affect the
    display of subsequent primitives. Use the popClipBoundary() to remove this
    clipping boundary from the stack.

    \param pBoundary [in]  Pointer to a clipping boundary.
    \param pClipInfo [in]  Clipping details.
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
  */
  void pushClipBoundary(OdGiClipBoundary* pBoundary, OdGiAbstractClipBoundary* pClipInfo);

  /** \details
    Pops a top clipping boundary from the stack. This method should be used
    after a successful pushClipBoundary() method call from the worldDraw or
    viewportDraw of a drawable object.
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
  */
  void popClipBoundary();

  /** OdGiBaseVectorizer Overrides *
  */

  /** \details
    Marks as changed the EntityTraitsData data for this Vectorizer object.
  */
  virtual void setEntityTraitsDataChanged();

  /** \details
    Marks the specfied trait as changed (or not) the EntityTraitsData data for
    this Vectorizer object.

    \param bit   [in]  Bit that represents an entity traits to set.
    \param value [in]  Flag that specifies whether trait should be set to the state "changed" or "unchanged".
  */
  virtual void setEntityTraitsDataChanged(int bit, bool value = true);

  /** \details
    Retrieves the transformation matrix from object space to device space.
  */
  virtual OdGeMatrix3d objectToDeviceMatrix() const;

  /** \details
    Pushes palette override into palette overrides stack.
    \param pOverride [in]  New palette override.
    \returns 
    True if palette overrides are supported by underlying vectorization pipeline.
  */
  virtual bool pushPaletteOverride(const OdGiPalette* pOverride);

  /** \details
    Pops the palette override from palette overrides stack.
  */
  virtual void popPaletteOverride();

  /** \details
    Checks whether there are any palette overrides in the palette overrides stack.
    \returns
    True if at least one override is present in the palette overrides stack,
    false otherwise.
  */
  virtual bool hasPaletteOverrides() const;

  /** OdGsBaseVectorizer methods *
  */

  /** \details
    Checks whether this vectorizer has infinite geometry extents disabled.
    \returns
    True if infinite geometry extents are disabled, false otherwise.
  */
  bool disableInfiniteGeomExtents() const { return GETBIT(m_flags, kDisableInfiniteGeomExtents); }
  bool sectionableGeomExtentsOnly() const { return GETBIT(m_flags, kSectionableGeomExtentsOnly); }
  void setSectionableGeomExtentsOnly(bool bOn) { SETBIT(m_flags, kSectionableGeomExtentsOnly, bOn); }

  /** \details
    This method sets internal state flag indicating playing mode and calls
    playMetafile(). The flag can be accessed via isPlayingMfAsGeometry(),
    see below.

    \param pMetafile [in]  Pointer to a metafile to play.
    \param eMode     [in]  Metafile play mode.
    \param node      [in]  Entity node.
    \param ctx       [in]  Pointer to a context.
  */
  void playMetafileMode(const OdRxObject* pMetafile, EMetafilePlayMode eMode, OdGsEntityNode& node, OdGsBaseContext* ctx);

  /** \details
    Plays (redraws) the specified Nested metafile (cache) object.
    \param pPortion [in]  Pointer to the geometry portion.
    \param pContext [in]  Pointer to the Gs context.
    \param node     [in]  Reference to the Gs node.
    \returns
    True if metafile was played successfully, false otherwise.
    \remarks
    GsMetafiles are used to record vectorization, and rapidly 'redraw' them to a display device.
  */
  virtual bool playNestedMetafile(const OdGsGeomPortion* pPortion, OdGsBaseContext* pContext, OdGsEntityNode& node);

  /** \details
	This method returns internal state indicating metafile playing mode.
  */
  EMetafilePlayMode metafilePlayMode() const { return m_eMfPlayMode;  }

  enum MetafileTransformFlags
  {
    kSharedRefTransform = (1 << 0),
    kSharedRefUpdate    = (1 << 1),
    kSharedRefSelect    = (1 << 2)
  };

  /** \details
    Pushes a transformation matrix for a metafile onto the stack. The resulting
    transformation will be created by multiplying the specified transform matrix
    by the existing one. This allows you to easily adjust an entity coordinate
    system. Use the popMetafileTransform() to remove this transform matrix from
    the stack when you are finished with this transformation.

    \param xfm   [in]  Transformation matrix.
    \param flags [in]  Metafile transform flags.
    \remarks
    This method takes effect only if the passed flags parameter has the
    kSharedRefUpdate flag set to true.
  */
  virtual void pushMetafileTransform(const OdGeMatrix3d&, OdUInt32 = 0);

  /** \details
    Pops a transformation from the stack. This method should be used after a
    successful pushMetafileTransform() method call to restore previous transform
    state.
  */
  virtual void popMetafileTransform(OdUInt32 = 0);

  /** \details
    Retrieves the block insert transform, which is the model-to-world coordinate
    transform.

    \returns
    Model-to-world transform matrix.
  */
  virtual OdGeMatrix3d metafileTransform() const;

  virtual bool useSharedBlockReferences() const { return true; }
  virtual bool useMetafileAsGeometry() const { return false; }
  virtual OdGiConveyorOutput& outputForMetafileGeometry();
  virtual void setTransformForMetafileGeometry(const OdGeMatrix3d&) {}
  virtual OdGeMatrix3d getTransformForMetafileGeometry() const { return OdGeMatrix3d::kIdentity; }
  virtual void reportUpdateError(OdDbStub* /*entityId*/, const OdError& error)
  {
#ifdef ODA_DIAGNOSTICS
    throw error;
#else
    ODA_ASSERT(error); // prevent arg unused warning
#endif
  }

  /** \details
    Retrieves a pointer to a layer node associated with the passed data.

    \param layerId [in]  Pointer to a persistent layer ID.
    \param pModel  [in]  Pointer to a model.
  */
  OdGsLayerNode* gsLayerNode(OdDbStub* layerId, OdGsBaseModel* pModel);

  /** \details
    Returns internal state indicating color fading mode.

    \returns
    True if any type of fading is 'true' and fading value is positive, false
    otherwise.
  */
  inline bool isFaded() const;

  /** \details
    Returns fading percentage in range [0..100].
  */
  inline OdUInt32 fadingIntensity() const;

  /** \details
    Checks whether this vectorizer has the kHighlighted flag set to true.

    \returns
    True if the kHighlighted flag set to true, false otherwise.
  */
  inline bool isHighlighted() const;

  /** \details
    Checks whether this vectorizer has an associated selection procedure.

    \returns
    True if this vectorizer has an associated selection procedure, false
    otherwise.
  */
  inline bool isSelecting() const;

  /** \details
    Returns vectorizer hidden geometry state.

    \returns
    True if geometry should be hidden, false otherwise.
  */
  inline bool isHidden() const;

  /** \details
    Specifies vectorizer hidden geometry state.

    \param bHidden [in]  Defines vectorizer hidden geometry state.
  */
  inline void setHidden( bool bHidden );

  /** \details
    Checks whether an associated device is in the dragging state.

    \returns
    True if an associated device is in the dragging state, false otherwise.
  */
  bool isDragging() const;

  virtual OdGiConveyorOutput& gsExtentsOutput() { return secondaryOutput(); }

  /** \details
    Controls analytic linetype support for circles and circular arcs in this
    VectorizeView object.

    \param analytic [in]  Analytic linetype support.

    \sa
    <link AnalyticLinetypeSupport, Analytic Linetype Support>
  */
  virtual void setAnalyticLinetypingCircles(bool analytic);

  /** \details
    Returns true if and only if analytic linetype support for circles and
    circular arcs is enabled in this VectorizeView object.

    \sa
    <link AnalyticLinetypeSupport, Analytic Linetype Support>
  */
  virtual bool isAnalyticLinetypingCircles() const;

  /** \details
    Controls analytic linetype support for ellipses, elliptical curves, and
    NURBS curves in this VectorizeView object.

    \param analytic [in]  Analytic linetype support.
    \sa
    <link AnalyticLinetypeSupport, Analytic Linetype Support>
  */
  virtual void setAnalyticLinetypingComplexCurves(bool analytic);

  /** \details
    Returns true if and only if analytic linetype support for ellipses,
    elliptical curves, and NURBS curves is enabled in this VectorizeView object.

    \sa
    <link AnalyticLinetypeSupport, Analytic Linetype Support>
  */
  virtual bool isAnalyticLinetypingComplexCurves() const;

  void checkSelection();

  /** \details
    Checks whether selection can be performed by checking extents without
    playing content.

    \param extWc [in] Data extents.
    \returns
    True if content should be selected without playing, false otherwise.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  bool handleSelectionByExtents( const OdGeExtents3d& extWc );

  virtual bool displayViewportProperties(OdGsPropertiesDirectRenderOutput *pdro = NULL,
      OdUInt32 incFlags = OdGsProperties::kAll);

  /** \details
    Returns the secondary output from this vectorizer object.
  */
  OdGiConveyorOutput& secondaryOutput();

  // OdGiBaseVectorizerImpl methods

  /** \details
    Sets up this vectorizer with the passed view implementation object.
    \param view [in] View implementation.
  */
  virtual void setUp(OdGsViewImpl& view);

  // OdGiBaseVectorizer methods

  /** \details
    Begins the view vectorization procedure.
  */
  virtual void beginViewVectorization();

  /** \details
    Ends the view vectorization procedure.
  */
  virtual void endViewVectorization();

  /** \details
    Called whenever traits of this vectorizer are changed.
  */
  virtual void onTraitsModified();

  /** \details
    Returns the recommended maximum deviation of the current vectorization, for
    the specified point on the curve or surface being tesselated.
    \param deviationType [in]  Deviation type.
    \param pointOnCurve  [in]  Point on the curve.

    \returns
    Recommended maximum deviation of the current vectorization as a double
    value.

    \remarks
    deviationType must be one of the following:

    <table>
    Name                     Value
    kOdGiMaxDevForCircle     0
    kOdGiMaxDevForCurve      1
    kOdGiMaxDevForBoundary   2
    kOdGiMaxDevForIsoline    3
    kOdGiMaxDevForFacet      4
    </table>
  */
  virtual double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const;

  /** \details
    Checks whether this vectorization process should be aborted.
    \returns
    True if this vectorization process should be aborted, false otherwise.
  */
  virtual bool regenAbort() const;

  /** \details
    Vectorizes the specified OdGiDrawable object.
    \param drawableFlags [in]  Drawable flags.
    \param pDrawable     [in]  Pointer to the Drawable object.
    \returns
    True if drawable is successfully drawn, false otherwise.
  */
  virtual bool doDraw(OdUInt32 drawableFlags, const OdGiDrawable* pDrawable);

  /** \details
    Sets the current selection marker.
    \param selectionMarker [in]  Selection marker.
  */
  virtual void setSelectionMarker(OdGsMarker selectionMarker);
  virtual OdGiConveyorOutput& output();

  /** \details
    This method can be called on each Visual Style displaying. It may be used by
    an inherited device for making own changes for Visual Style support.
    \param visualStyle [in]  Reference to the current Visual Style state.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  virtual void setVisualStyle(const OdGiVisualStyle& visualStyle);

  /** \details
    Returns a pointer to a set of markers that represent highlighted
    sub-enities.
  */
  const OdGsMarkerSet* highlightedSubentities() const;

  /** \details
    Returns a pointer to a subnode branch of the top highlighted node.
    \returns
    Pointer to the subnode branch.
  */
  OdGsStateBranch* findHighlightedSubnodeBranch();
  OdUInt32 threadIndex() const;

  /** \details
    Sets the highlight flag for the vectorizer.
    \param bHighlight [in]  Enables or disables highlighting.
    \param nSelStyle  [in]  Selection style.
    \sa
    <link tv_working_with_features_highlighting, Work with Highlighting>
  */
  void highlight(bool bHighlight, OdUInt32 nSelStyle = 0);

  /** \details
    Returns current highlighting data.
    \returns
    Pointer to the current highlighting branch.
  */
  const OdGsStateBranch* currentHighlightBranch() const;

  /** \details
    Returns current visibility branch.
    \returns
    Pointer to the current visibility branch.
  */
  const OdGsStateBranch* currentVisibilityBranch() const;

  /** \details
    Finds the subnode visibility branch.
    \returns
    Pointer to the subnode visibility branch.
  */
  OdGsStateBranch* findHiddenSubnodeBranch();

  /** \details
    Retrieves a pointer to a currently active layer node.
    \param bSync [in]  Flag that specifies whether to synchronize effective layer traits.
  */
  OdGsLayerNode *activeLayerNode(bool bSync = false) const;

  /** \details
    Retrieves the active render type.
  */
  OdGsModel::RenderType activeRenderType() const;

  /** \details
    Retrieves the active buffer overlay.
  */
  OdGsOverlayId activeOverlay() const;

  /** \details
    Checks whether the spatial index is disabled.
    \returns
    False if the spatial index should be used when displaying, otherwise all
    metafiles will be forced even if they are outside of the view frustum.
  */
  virtual bool isSpatialIndexDisabled() const { return false; }

  /** \details
    Begins metafile recording.
    \param pGeomPortion [in]  Pointer to geometry portion.
  */
  virtual void beginMetafileRecording(OdGsGeomPortion* pGeomPortion);

  /** \details
    Ends metafile recording.
  */
  virtual void endMetafileRecording();

protected:
  //DOM-IGNORE-BEGIN
  void doSelect(const OdGePoint2d* aPtDc, int numPoints,
                OdGsSelectionReactor* pReactor, OdGsView::SelectionMode mode);
  bool doViewExtents(OdGeBoundBlock3d& extents);
  /** \details
    Paints the display with each of the drawable objects associated with this VectorizeView object.
    
    \param update [in]  If and only if true, the cache is updated before displaying the drawable.
     
    \remarks
    The display is painted for each drawable from one of the following methods in the following order.
    
    1.  Model cache. 
    2.  The metafile for the drawable.
    3.  The drawable itself.
  */
  virtual void display(bool bUpdate);
  /** \details
    Recomputes the extents for this VectorizeView object.
    
    \param buildCache [in]  Builds Metafile (cache) objects if and only if true.
  */
  virtual void updateExtents(bool bBuildCache);

  // OdGiBaseVectorizer methods
  virtual const OdGiLayerTraitsData& effectiveLayerTraits() const;
  virtual OdDbStub* switchLayer(OdDbStub* layerId) const;

  void selectionMarkerOnChange(OdGsMarker nSelectionMarker);

  void checkRenderType(const OdGsModel *pModel);
  virtual void renderTypeOnChange(OdGsModel::RenderType renderType);
  virtual void switchOverlay(OdGsOverlayId overlayId);

  /** \details
    Initialize deviation muliplier from active device "Deviation" peroperty value.
    \remarks
    Called at beginning of beginViewVectorization method.
  */
  void initDeviationMultiplier();
  /** \details
    Applies deviation multiplier for provided deviation value.
    \param deviationType [in]  Deviation type.
    \param dDeviationValue [in]  Exist deviation value which needs to be multiplied.
  */
  double handleDeviationMultiplier(const OdGiDeviationType deviationType, double dDeviationValue) const;

  void displayWithoutNesting(OdGsEntityNode *pNode);

  virtual void displayNode(OdGsNode& node, OdGsDisplayContext& ctx);
  virtual void displaySubnode(OdGsEntityNode& node, OdGsDisplayContext& ctx, bool bHighlighted);

  void doDrawLight(const OdGiDrawable* pDrawable);
  bool isSharedGraphicsState() const;
  const OdGsUpdateState* currentState() const { return m_curState; }

  virtual bool updateExtentsOnly() const ODRX_OVERRIDE;
  //DOM-IGNORE-END
public:

  /** \details
    Sets the default drawable traits for this Vectorizer object, and sets the
    drawable flags for the specified OdGiDrawable object.
    \param pDrawable [in]  Pointer to the Drawable object.
  */
  virtual OdUInt32 setAttributes(const OdGiDrawable* pDrawable);

  void updateExtentsInThreadInit(OdGsMtContext& mtContext);

private:
  //DOM-IGNORE-BEGIN
  void setInitGsState(bool bOn);
  bool isRecordingHistory();
  void deleteHistory();
  void setThreadIndex(OdUInt32 idx) { m_threadIndex = idx; }
  bool drawSectionable(const OdGiDrawable& drawable, bool& bRes);
  bool drawSectionGeometry(OdGiSectionGeometry& geom, bool bVpModelTfAwareBefore);
  void setCurrentState(OdGsUpdateState& newState);
  OdGsUpdateState* currentState() { return m_curState; }
  void applyState(const OdGsUpdateState& s, const OdGsUpdateState& prev);
  //DOM-IGNORE-END
public:
  OdGsStateBranchPtr m_pCurHltBr; // Current highlight branch
  OdGsStateBranchPtr m_pCurVisibilityBr; // Current visibility branch 
protected:
  //DOM-IGNORE-BEGIN
  OdArray<OdGeMatrix3d> m_metafileTransfStack;
  OdUInt64 m_nMetafileSize;
  OdGsWriter m_gsWriter;
  OdGiSelectProc* m_pSelectProc;
  OdGiCollideProc* m_pCollideProc;
  OdGiConveyorGeometry* m_pDetachedOutput;
  OdGiConveyorGeometry* m_pDetachedSecondaryOutput;
  const OdSiShape* m_MfPlayQuery; // Currently playing query
  const OdGsEntityNode* m_MfPlayNode; // Currently playing node
  EMetafilePlayMode m_eMfPlayMode;
  OdGsUpdateState* m_curState;
  OdGsModel::RenderType m_curRenderType;
  OdGsOverlayId m_curOverlay;
  double m_dDeviationMultiplier;
  OdIntPtr m_drawableFilterFunction;
  OdGsMaterialNode *m_pCachedMaterial;
  // Fading support
  enum FadingFlags
  {
    kFfLockedLayer       = (1 << 0),
    kFfXref              = (1 << 1),
    kFfRefEdit           = (1 << 2),
    kFfFlagsMask         = kFfLockedLayer  | kFfXref  | kFfRefEdit,
    kEFfLockedLayer      = (1 << 4),
    kEFfXref             = (1 << 5),
    kEFfRefEdit          = (1 << 6),
    kEFfFlagsMask        = kEFfLockedLayer | kEFfXref | kEFfRefEdit,
    kEFfFlagsOffset      = 4,
    kFvLockedLayerOffset = 8,
    kFvLockedLayerMask   = (0xFF << kFvLockedLayerOffset),
    kFvXrefOffset        = 16,
    kFvXrefMask          = (0xFF << kFvXrefOffset),
    kFvRefEditOffset     = 24,
    kFvRefEditMask       = (0xFF << kFvRefEditOffset)
  };
  OdUInt32 m_fadingFlags;
  template <OdGiContext::FadingType fadingType> friend class OdGsLockFadingFlag;

  OdUInt32 m_selectionStyle;
  //DOM-IGNORE-END
public:

  /** \details
    Returns the current selection style.
    \returns Current selection style index.
  */
  OdUInt32 currentSelectionStyle() const { return m_selectionStyle; }

  /** \details
    Checks whether the current selection style is set.
    \return True if the current selection style set, false otherwise.
  */
  bool hasSelectionStyle() const { return m_selectionStyle != 0; }

  /** \details
    Sets the render abort flag.
    \param bSet [in]  Render abort flag value.
  */
  void setRenderAbort( bool bSet );

  /** \details
    Checks whether the render abort flag is set.
    \returns
    True if the render abort flag is set, false otherwise.
  */
  virtual bool renderAbort();
protected:
  //DOM-IGNORE-BEGIN
  enum BaseVectorizerFlags
  {
    kRenderAbort            = OdGiBaseVectorizerImpl::kLastImplFlag << 1,
    kViewInteractive        = OdGiBaseVectorizerImpl::kLastImplFlag << 2,
    kHighlightFirstPass     = OdGiBaseVectorizerImpl::kLastImplFlag << 3,
    kAbsoluteDeviation      = OdGiBaseVectorizerImpl::kLastImplFlag << 4,

    kLastBaseVectorizerFlag = kAbsoluteDeviation
  };
  double m_dRenderAbortCondition;
  OdPerfTimerBase* m_viewTimingProcessor;

  const OdGsExtAccum *secondaryOutputExtents() const { return m_pOutputExtents.get(); }
  OdGsExtAccum *secondaryOutputExtents() { return m_pOutputExtents.get(); }
private:
  mutable const OdGiLayerTraitsData* m_pCachedLayer;
  OdGsLayerNode* m_effectiveLayer;
  OdGsExtAccumPtr m_pOutputExtents;
  OdGiHistory* m_giHistory;
  OdUInt32 m_threadIndex;
  bool m_bTraitsDataChanged;
  bool m_bByBlockTraitsChanged;
  friend class OdGsUpdateContext;
  friend class OdGsDisplayContext;
  friend class OdGsBaseVectorizeView;
  friend class OdGsApplyModelOverrides;

protected:
  virtual void doCollide( OdGiPathNode const*const* pInputList, OdUInt32 nInputListSize, OdGsCollisionDetectionReactor* pReactor, OdGiPathNode const*const* pCollisionWithList = NULL, OdUInt32 nCollisionWithListSize = 0, const OdGsCollisionDetectionContext* pCtx = NULL );
  virtual void doCollideAll( OdGsCollisionDetectionReactor* pReactor, const OdGsCollisionDetectionContext* pCtx = NULL );
  void processDrawableForCollide( const OdGiPathNode* pStartNode, const OdGiDrawable* pDrawable, OdGsBaseModel*& pCurModel, OdGsApplyModelOverridesPtr& pCurOverrides );
  void processAllDrawablesForCollide( const OdGeExtents3d* pExtents = NULL, OdGiPathNode const*const* pCollisionWithList = NULL, OdUInt32 nCollisionWithListSize = 0, bool bIgnoreViewExtents = false );
  //DOM-IGNORE-END
};

template <typename ClientClass = OdGsBaseVectorizer>
class OdGsHighlightingStateSaver
{
  protected:
    ClientClass& m_vect;
    const bool m_bHighlighted;
    const OdUInt32 m_selStyle;
  public:
    OdGsHighlightingStateSaver(ClientClass &vect)
      : m_vect(vect)
      , m_bHighlighted(vect.isHighlighted())
      , m_selStyle(vect.currentSelectionStyle())
    { }
    ~OdGsHighlightingStateSaver()
    { m_vect.highlight(m_bHighlighted, m_selStyle); }
    bool isHighlighted() const { return m_bHighlighted; }
    OdUInt32 selectionStyle() const { return m_selStyle; }
};

template <OdGiContext::FadingType fadingType>
class OdGsLockFadingFlag
{
  protected:
    OdUInt32 &m_lockedFlags;
    bool m_bSavedBit;
  public:
    OdGsLockFadingFlag(OdGsBaseVectorizer &vectorizer)
      : m_lockedFlags(vectorizer.m_fadingFlags)
      , m_bSavedBit(GETBIT(vectorizer.m_fadingFlags, 1 << fadingType))
    {}
    void set(bool bNewVal) { SETBIT(m_lockedFlags, 1 << fadingType, bNewVal); }
    OdGsLockFadingFlag(OdGsBaseVectorizer &vectorizer, bool bNewVal)
      : m_lockedFlags(vectorizer.m_fadingFlags)
      , m_bSavedBit(GETBIT(vectorizer.m_fadingFlags, 1 << fadingType))
    { set(bNewVal); }
    ~OdGsLockFadingFlag() { set(m_bSavedBit); }
};

inline bool OdGsBaseVectorizer::isFaded() const
{ // Returns true in case if any type of fading is 'true' and fading value is positive
  return !!((m_fadingFlags & kFfFlagsMask) & ((m_fadingFlags & kEFfFlagsMask) >> kEFfFlagsOffset));
}

inline OdUInt32 OdGsBaseVectorizer::fadingIntensity() const
{ // Returns percentage in [0-100] range
  OdUInt32 nIntensityMix = 100;
  if (GETBIT(m_fadingFlags, kFfLockedLayer) && GETBIT(m_fadingFlags, kEFfLockedLayer))
  {
    OdUInt32 mix = 100 - ((m_fadingFlags & kFvLockedLayerMask) >> kFvLockedLayerOffset);
    nIntensityMix = mix;
  }
  if (GETBIT(m_fadingFlags, kFfXref) && GETBIT(m_fadingFlags, kEFfXref))
  {
    OdUInt32 mix = 100 - ((m_fadingFlags & kFvXrefMask) >> kFvXrefOffset);
    nIntensityMix = odmin(nIntensityMix, mix);
  }
  if (GETBIT(m_fadingFlags, kFfRefEdit) && GETBIT(m_fadingFlags, kEFfRefEdit))
  {
    OdUInt32 mix = 100 - ((m_fadingFlags & kFvRefEditMask) >> kFvRefEditOffset);
    nIntensityMix = nIntensityMix * mix / 100;
  }
  nIntensityMix = odmax(10, nIntensityMix);
  return 100 - nIntensityMix;
}

inline const OdGsMarkerSet* OdGsBaseVectorizer::highlightedSubentities() const
{
  return m_pCurHltBr.get() && m_pCurHltBr->markersSize() ? &m_pCurHltBr->markers() : NULL;
}

inline OdUInt32 OdGsBaseVectorizer::threadIndex() const
{
  return m_threadIndex;
}

inline const OdGsStateBranch* OdGsBaseVectorizer::currentHighlightBranch() const
{
  return m_pCurHltBr;
}

inline const OdGsStateBranch* OdGsBaseVectorizer::currentVisibilityBranch() const
{
  return m_pCurVisibilityBr;
}

inline OdGsLayerNode *OdGsBaseVectorizer::activeLayerNode(bool bSync) const
{
  if (bSync)
    updateLayerTraits(m_effectiveEntityTraitsData);
  return m_effectiveLayer;
}

inline OdGsModel::RenderType OdGsBaseVectorizer::activeRenderType() const
{
  return m_curRenderType;
}

inline OdGsOverlayId OdGsBaseVectorizer::activeOverlay() const
{
  return m_curOverlay;
}

inline bool OdGsBaseVectorizer::isSelecting() const
{
  return m_pSelectProc != 0;
}

inline void OdGsBaseVectorizer::highlight(bool bHighlight, OdUInt32 nSelStyle)
{
  SETBIT(m_flags, kHighlighted, bHighlight);
  m_selectionStyle = nSelStyle;
}

inline bool OdGsBaseVectorizer::isHighlighted() const
{
  return GETBIT(m_flags, kHighlighted);
}

inline bool OdGsBaseVectorizer::isHidden() const
{
  return GETBIT(m_implFlags, kHiddenImplFlag);
}

inline void OdGsBaseVectorizer::setHidden( bool bHidden )
{
  SETBIT( m_implFlags, kHiddenImplFlag, bHidden );
}

inline OdGeMatrix3d OdGsBaseVectorizer::metafileTransform() const
{ // Metafile transformation matrix
  return m_metafileTransfStack.isEmpty() ? OdGeMatrix3d::kIdentity : m_metafileTransfStack.last();
}

#include "TD_PackPop.h"

#endif // __OD_GS_BASE_VECTORIZER_H_
