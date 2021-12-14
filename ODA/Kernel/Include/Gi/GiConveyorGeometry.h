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

#ifndef __ODGICONVEYORGEOMETRY_H__
#define __ODGICONVEYORGEOMETRY_H__

#include "Gi/GiGeometry.h"
#include "Gi/GiCommonDraw.h"
#include "Gs/GsSelectionReactor.h"
#include "Gi/GiSubEntityTraitsData.h"

class OdGiRasterImage;
class OdGiMetafile;
class OdGiViewport;
class OdGiPointCloudFilter;
class OdGsView;
class OdGiCommonDraw;
class OdGiDeviation;
class OdGeExtents3d;
class OdGeBoundBlock3d;

/** \details
    This class implements an interface that allows conveyor nodes to interact with parent objects.
    
    \remarks
    This interface gives
    abstract access to OdGiBaseVectorizer level data and services.
    Many conveyor nodes require a pointer to an OdGiConveyorContext interface,
    in order to be initialized.

    Corresponding C++ library: TD_Gi
    <group OdGi_Classes> 
*/
class OdGiConveyorContext
{
public:
  /** \details
    Destructor for the OdGiConveyorContext class.
  */
  virtual ~OdGiConveyorContext() {}

  /** \details
    Retrieves the OdGiContext object associated with this ConveyorContext object.
    
    \returns
    a reference to the corresponding OdGiContext object.
  */
  virtual OdGiContext& giContext() const = 0;

  /** \details
    Retrieves the OdGiSubEntityTraits instance associated with this ConveyorContext object.
    
    \returns
    OdGiSubEntityTraits instance associated with this ConveyorContext object.
    
    \remarks
    The OdGiSubEntityTraits object is used to register attribute changes with
    the vectorization framework.
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const = 0;

  /** \details
    Retrieves the traits currently in effect for this ConveyorContext object.
    
    \returns
    effective traits as OdGiSubEntityTraitsData object.
  */
  virtual const OdGiSubEntityTraitsData& effectiveTraits() const = 0;

  /** \details
    Sets the traits currently in effect for this ConveyorContext object.
    \param traits [in]  New traits data which will be used for effective traits substitution.
    \param fillNormal [in]  Optional fill normal.
  */
  virtual void setEffectiveTraits(const OdGiSubEntityTraitsData& traits,
                                  const OdGeVector3d* fillNormal = NULL) = 0;

  /** \details
    Checks the current visibility state for current traits.
    
    \returns
    true if effective traits are visible, false otherwise.
  */
  virtual bool effectivelyVisible() const { return effectiveTraits().visibility(); }

  /** \details
    Retrieves the current OdGiDrawableDesc instance for this ConveyorContext object.
    
    \returns
    pointer to an instance of OdGiDrawableDesc that provides information about a drawable.
    
    \remarks
    This instance contains information about the current drawable object (the object itself, rendering flags, etc.).
  */
  virtual const OdGiDrawableDesc* currentDrawableDesc() const = 0;

  /** \details
    Retrieves the current OdGiDrawable object for this ConveyorContext object.
    
    \returns
    pointer to an instance of OdGiDrawable that represents a drawable.
    
    \remarks
    This is the object currently being vectorized by the framework.
  */
  virtual const OdGiDrawable* currentDrawable() const = 0;

  /** \details
    Retrieves the OdGiViewport associated with this ConveyorContext object.
    
    \returns
    pointer to an instance of OdGiViewport that represents a viewport.
  */
  virtual const OdGiViewport* giViewport() const = 0;

  /** \details
    Retrieves the OdGsView associated with this ConveyorContext object.
    
    \returns
    pointer to an instance of OdGsView that represents a view.
    \sa
    <link tv_working_with_base_classes_view, Work with Visualize Graphical Views>
  */
  virtual const OdGsView* gsView() const = 0;

  /** \details
    Notification function called by the vectorization framework
    whenever the rendering attributes have changed.

    \remarks
    This function is called before sending any geometry through the geometry conveyor.

    This function provides a hook
    for setting rendering attributes for the final geometry handler (e.g. Win GDI device).
  */
  virtual void onTraitsModified() = 0;

  /** \details
    Notification function called by the vectorization framework
    whenever the text is become to be rendered.

    \remarks
    This function provides a hook for setting geometry transformation before text was drawn.
  */
  virtual void onTextProcessing(const OdGePoint3d& /*position*/,
                                const OdGeVector3d& /*direction*/,
                                const OdGeVector3d& /*upVector*/) { }

  /** \details
    Checks whenever regen was aborted.
    
    \returns
    true regeneration was aborted, false otherwise.
  */
  virtual bool regenAbort() const = 0;

  /** \details
    Retrieves current drawable nesting graph.
    
    \returns
    pointer to the OdGiPathNode instance.
  */
  virtual const OdGiPathNode* currentGiPath() const = 0;

  /** \details
    Retrieves deviations array accessor for world coordinate system.
    
    \returns
    the OdGiDeviation instance that represents world deviation.
  */
  virtual const OdGiDeviation& worldDeviation() const = 0;
  /** \details
    Retrieves deviations array accessor for model coordinate system.
    
    \returns
    the OdGiDeviation instance that represents model deviation.
  */
  virtual const OdGiDeviation& modelDeviation() const = 0;
  /** \details
    Retrieves deviations array accessor for eye coordinate system.
    
    \returns
    the OdGiDeviation instance that represents eye deviation.
  */
  virtual const OdGiDeviation& eyeDeviation() const = 0;

  /** \details
    Retrieves the model-to-world coordinate transform matrix for the entity being vectorized.
    
    \returns
    model-to-world coordinate transform matrix.
    
    \remarks
    This is the inverse of the matrix returned by getWorldToModelTransform(). 

    If an entity is in one or more blocks, this matrix can be used to
    determine the WCS coordinates of the entity.  
  */
  virtual OdGeMatrix3d getModelToWorldTransform() const = 0;

  /** \details
    Retrieves the world-to-model coordinate transform matrix for the entity being vectorized.
    
    \returns
    world-to-model coordinate transform matrix.
    
    \remarks
    This is the inverse of the matrix returned by getModelToWorldTransform(). 
  */
  virtual OdGeMatrix3d getWorldToModelTransform() const = 0;

  /** \details
    Retrieves current lineweight override (if present).
    
    \returns
    pointer to the OdGiLineweightOverride instance.
  */
  virtual const OdGiLineweightOverride *currentLineweightOverride() const { return NULL; }

  /** \details
    Conveyor geometry context flags.
  */
  enum ConveyorContextFlags
  {
    // Spatial filter as simplifier
    kSpatialFilterSimplPline = 1, // Simplify plineProc() by spatial filter
    kSpatialFilterSimplNurbs = 2, // Simplify nurbsProc() by spatial filter
    kSpatialFilterSimplText  = 4, // Simplify textProc () by spatial filter
    kSpatialFilterSimplShape = 8, // Simplify shapeProc() by spatial filter
    kSpatialFilterSimplAll   = (kSpatialFilterSimplPline | kSpatialFilterSimplNurbs |
                                kSpatialFilterSimplText  | kSpatialFilterSimplShape), // All spatial filter options
    // Conveyor embranchment as simplifier
    kEmbranchmentSimplText   = 16, // Simplify textProc () by conveyor embranchment
    kEmbranchmentSimplNurbs  = 32, // Simplify nurbsProc() by conveyor embranchment
    kEmbranchmentSimplAll    = (kEmbranchmentSimplText | kEmbranchmentSimplNurbs), // All conveyor embranchment options
    kConveyorSimplAll        = (kSpatialFilterSimplAll | kEmbranchmentSimplAll), // All simplification options
    // Pline subentity markers
    kPlineMarkers            = 64, // Enable subentity markers set per pline() call
    kForceMarkersOnModified  = 128, // Force onTraitsModified call for each subentity marker change
    // Polyline subentity markers
    kPolylineMarkers         = 256, // Enable subentity markers set per polyline() call
    // Pline geometry
    kPlineAllowArcProc       = 512, // Vectorizer will draw arc segments of polyline as arcs (otherwise as sample points)
    // Enable test-specific functionality
    kTestMode                = 1024, // Enable test mode
    // Specific vectorization mode
    kLineTyperAfterMetafile  = 2048  // Specific mode (for example in GDI device) when the Line type node are follows by the metafile
  };
  /** \details
    Retrieves set of conveyor context flags.
    
    \returns
    context flags as OdUInt32 value.
  */
  virtual OdUInt32 drawContextFlags() const { return 0; }

  /** \details
    Retrieves current annotationScale value of conveyor context.
    
    \returns
    annotation scale as double value.
  */
  virtual double annotationScale() const { return 1.0; }
};

/** \details

    \sa
    TD_Gi 
    <group OdGi_Classes> 
*/
class OdGiConveyorContextWrapper : public OdGiConveyorContext
{
protected:
  OdGiConveyorContext *m_pCtx;
public:
  /** \details
    Retrieves original drawing context associated with this object.
    
    \returns
    pointer to the OdGiConveyorContext instance.
  */
  OdGiConveyorContext *getOriginalContext() const { return m_pCtx; }
  
  /** \details
    Sets original drawing context for this object.
    
    \param pCtx [in]  Pointer to original drawing context to set.
  */
  void setOriginalContext(OdGiConveyorContext *pCtx) { m_pCtx = pCtx; }
  
  /** \details
    Constructor for the OdGiConveyorContextWrapper class.
    
    \param pCtx [in]  Pointer to a conveyor context.
  */
  explicit OdGiConveyorContextWrapper(OdGiConveyorContext *pCtx = NULL) : m_pCtx(pCtx) { }

  
  /** \details
    Retrieves Gi context.
    
    \returns
    Gi context as OdGiContext instance.
  */
  OdGiContext& giContext() const { return m_pCtx->giContext(); }
  
  /** \details
    Retrieves entity traits.
    
    \returns
    entity traits as OdGiSubEntityTraits instance.
  */
  OdGiSubEntityTraits& subEntityTraits() const { return m_pCtx->subEntityTraits(); }
  
  /** \details
    Retrieves the traits currently in effect for this ConveyorContext object.
    
    \returns
    effective traits as OdGiSubEntityTraitsData object.
  */
  const OdGiSubEntityTraitsData& effectiveTraits() const { return m_pCtx->effectiveTraits(); }
  
  /** \details
    Sets the traits currently in effect for this ConveyorContext object.
    \param traits [in]  New traits data which will be used for effective traits substitution.
    \param fillNormal [in]  Optional fill normal.
  */
  void setEffectiveTraits(const OdGiSubEntityTraitsData& traits,
                          const OdGeVector3d* fillNormal = NULL) { m_pCtx->setEffectiveTraits(traits, fillNormal); }
  
  /** \details
    Checks the current visibility state for current traits.
    
    \returns
    true if effective traits are visible, false otherwise.
  */
  bool effectivelyVisible() const { return m_pCtx->effectivelyVisible(); }
  
  /** \details
    Retrieves the current OdGiDrawableDesc instance for this object.
    
    \returns
    pointer to an instance of OdGiDrawableDesc that provides information about a drawable.
    
    \remarks
    This instance contains information about the current drawable object (the object itself, rendering flags, etc.).
  */
  const OdGiDrawableDesc* currentDrawableDesc() const { return m_pCtx->currentDrawableDesc(); }
  
  /** \details
    Retrieves the current OdGiDrawable object for this object.
    
    \returns
    pointer to an instance of OdGiDrawable that represents a drawable.
    
    \remarks
    This is the object currently being vectorized by the framework.
  */
  const OdGiDrawable* currentDrawable() const { return m_pCtx->currentDrawable(); }
  
  /** \details
    Retrieves the OdGiViewport associated with this object.
    
    \returns
    pointer to an instance of OdGiViewport that represents a viewport.
  */
  const OdGiViewport* giViewport() const { return m_pCtx->giViewport(); }
  
  /** \details
    Retrieves the OdGsView associated with this object.
    
    \returns
    pointer to an instance of OdGsView that represents a view.
    \sa
    <link tv_working_with_base_classes_view, Work with Visualize Graphical Views>
  */
  const OdGsView* gsView() const { return m_pCtx->gsView(); }
  
  /** \details
    Notification function called by the vectorization framework
    whenever the rendering attributes have changed.

    \remarks
    This function is called before sending any geometry through the geometry conveyor.

    This function provides a hook
    for setting rendering attributes for the final geometry handler (e.g. Win GDI device).
  */
  void onTraitsModified() { m_pCtx->onTraitsModified(); }
  
  /** \details
    Notification function called by the vectorization framework
    whenever the text is become to be rendered.
    
    \param position [in]  Text position.
    \param direction [in]  Text direction vector.
    \param upVector [in]  Text up-vector.
    
    \remarks
    This function provides a hook for setting geometry transformation before text was drawn.
  */
  void onTextProcessing(const OdGePoint3d& position, const OdGeVector3d& direction, const OdGeVector3d& upVector)
    { m_pCtx->onTextProcessing(position, direction, upVector); }
  
  /** \details
    Checks whenever regen was aborted.
    
    \returns
    true regeneration was aborted, false otherwise.
  */
  bool regenAbort() const { return m_pCtx->regenAbort(); }
  
  /** \details
    Retrieves current drawable nesting graph.
    
    \returns
    pointer to the OdGiPathNode instance.
  */
  const OdGiPathNode* currentGiPath() const { return m_pCtx->currentGiPath(); }
  
  /** \details
    Retrieves deviations array accessor for world coordinate system.
    
    \returns
    the OdGiDeviation instance that represents world deviation.
  */
  const OdGiDeviation& worldDeviation() const { return m_pCtx->worldDeviation(); }
  
  /** \details
    Retrieves deviations array accessor for model coordinate system.
    
    \returns
    the OdGiDeviation instance that represents model deviation.
  */
  const OdGiDeviation& modelDeviation() const { return m_pCtx->modelDeviation(); }
  
  /** \details
    Retrieves deviations array accessor for eye coordinate system.
    
    \returns
    the OdGiDeviation instance that represents eye deviation.
  */
  const OdGiDeviation& eyeDeviation() const { return m_pCtx->eyeDeviation(); }
  
  /** \details
    Retrieves the model-to-world coordinate transform matrix for the entity being vectorized.
    
    \returns
    model-to-world coordinate transform matrix.
    
    \remarks
    This is the inverse of the matrix returned by getWorldToModelTransform(). 

    If an entity is in one or more blocks, this matrix can be used to
    determine the WCS coordinates of the entity.  
  */
  OdGeMatrix3d getModelToWorldTransform() const { return m_pCtx->getModelToWorldTransform(); }
  
  /** \details
    Retrieves the world-to-model coordinate transform matrix for the entity being vectorized.
    
    \returns
    world-to-model coordinate transform matrix.
    
    \remarks
    This is the inverse of the matrix returned by getModelToWorldTransform(). 
  */
  OdGeMatrix3d getWorldToModelTransform() const { return m_pCtx->getWorldToModelTransform(); }
  
  /** \details
    Retrieves current lineweight override (if present).
    
    \returns
    pointer to the OdGiLineweightOverride instance.
  */
  const OdGiLineweightOverride *currentLineweightOverride() const { return m_pCtx->currentLineweightOverride(); }
  
  /** \details
    Retrieves set of conveyor context flags.
    
    \returns
    context flags as OdUInt32 value.
  */
  OdUInt32 drawContextFlags() const { return m_pCtx->drawContextFlags(); }
};

/** \details
    This class defines the interface through which nodes in the ODA Platform vectorization pipeline transfer geometric data.
    \sa
    TD_Gi
    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiConveyorGeometry
{
public:
  /** \details
    Destructor for the OdGiConveyorGeometry class.
  */
  virtual ~OdGiConveyorGeometry() {}

  /** \details
    Processes OdGiPolyline data.

    \param polyline [in]  Polyline data to be processed.
    \param pXfm [in]  Pointer to a transformation matrix.
    \param fromIndex [in]  Index of the first segment to be processed.
    \param numSegs [in]  Number of segments to be processed.
    \remarks
    numSegs == 0 indicates all segments starting at fromIndex are to be processed. 
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void plineProc(const OdGiPolyline& polyline,
    const OdGeMatrix3d* pXfm = 0,
    OdUInt32 fromIndex = 0,
    OdUInt32 numSegs = 0) = 0;

  /** \details
    Processes simple polyline data.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \param baseSubEntMarker [in]  Currently unused.
    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void polylineProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker baseSubEntMarker = -1) = 0;


#if defined(_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning (push)
#pragma warning (disable: 4481) // Non-standard extension used

// The right function prototype was changed (see above). Old one left to cause compilation error
  virtual void polylineProc(
    OdInt32 /*numPoints*/, const OdGePoint3d* /*vertexList*/,
    const OdGeVector3d* /*pNormal*/,
    const OdGeVector3d* /*pExtrusion*/,
    OdInt32 /*baseSubEntMarker*/) sealed {ODA_FAIL();}
#pragma warning (pop)
#endif

  /** \details
    Processes polygon data.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.

    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_polygons, Work with Polygons>
  */
  virtual void polygonProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Processes Xline data.
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \remarks
    An Xline is an infinite line passing through the 
    specified points.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void xlineProc(
    const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint) = 0;

  /** \details
    Processes Ray data.
    
    \param basePoint [in]  Base point.
    \param throughPoint [in]  Through point
    
    \remarks
    A Ray is a semi-infinite line that starts at the basePoint, and passes 
    through the throughPoint.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void rayProc(
    const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint) = 0;

  /** \details
    Processes mesh data.

    \param numRows [in]  Number of rows.
    \param numColumns [in]  Number of columns.
    \param vertexList [in]  Array of vertices.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data. 

    \sa
    OdGiEdgeData
    OdGiFaceData
    OdGiVertexData
    Vectorization Primitives
    \sa
    <link tv_geometry_meshes, Work with Meshes>
  */
  virtual void meshProc(
    OdInt32 numRows, 
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0) = 0;

  /** \details
    Processes shell data.

    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices.
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  Array of numbers that define the faces.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data. 

    \sa
    Vectorization Primitives
    \sa
    <link tv_geometry_shells, Work with Shells>
  */
  virtual void shellProc(
    OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0) = 0;

  /** \details
    Process circle data. 

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  virtual void circleProc(
    const OdGePoint3d& center, 
    double radius, 
    const OdGeVector3d& normal, 
    const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Process circle data. 

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param startVector [in]  Start vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  virtual void circleProc2(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& /*startVector*/,
    const OdGeVector3d* pExtrusion = 0)
  {
    circleProc(center, radius, normal, pExtrusion);
  }

   /** \details
    Process circle data. 
    \param firstPoint [in]  First point of a 3-point circle.
    \param secondPoint [in]  Second point of a 3-point circle.
    \param thirdPoint [in]  Third point of a 3-point circle.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    
    \remarks
    If firstPoint, secondPoint, and thirdPoint are specified, they
    cannot be colinear and no two can be coincident.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  virtual void circleProc(
    const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint, 
    const OdGePoint3d& thirdPoint, 
    const OdGeVector3d* pExtrusion = 0) = 0;
  
  /** \details
    Processes circular arc data.

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param startVector [in]  Defines the start of this arc.
    \param sweepAngle [in]  Angle that defines the arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     2         Filled area bounded by the arc and its end points
    </table>

    The extrusion vector specifies the direction and distance of the extrusion.
    
    \remarks
    All angles are expressed in radians.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  virtual void circularArcProc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0) = 0;
  
  /** \details
    Processes circular arc data.

    \param firstPoint [in]  First point of a 3-point arc.
    \param secondPoint [in]  Second point of a 3-point arc.
    \param thirdPoint [in]  Third point of a 3-point arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    
    \remarks
    If firstPoint, secondPoint, and thirdPoint are specified, they
    cannot be colinear and no two can be coincident.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  virtual void circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Processes OdGeEllipArc3d data.

    \param ellipArc [in]  Elliptical arc.
    \param endPointOverrides [in]  Array of points to be used as the first and last points of the vectorized arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Pointer to the extrusion vector.
        
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     3         Filled area bounded by the arc and its end points
    </table>

    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  virtual void ellipArcProc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Processes OdGeNurbCurve3d data.
    \param nurbsCurve [in]  NURBS curve data.
    \sa
    <link tv_geometry_nurbs, Work with NURBS>
  */
  virtual void nurbsProc(
    const OdGeNurbCurve3d& nurbsCurve) = 0;

  /** \details
    Processes text data.

    \param position [in]  Position of the text.
    \param direction [in]  Baseline direction of the text.
    \param upVector [in]  Up vector for the text.
    \param msg [in]  Text string.
    \param numBytes [in]  Number of bytes in msg.
    \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters.
    \param pTextStyle [in]  Pointer to the TextStyle for the text.
    \param pExtrusion [in]  Pointer to the extrusion vector.

    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_text, Work with Text>
  */
  virtual void textProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Processes text data.

    \param position [in]  Position of the text.
    \param direction [in]  Baseline direction of the text.
    \param upVector [in]  Up vector for the text.
    \param msg [in]  Text string.
    \param numBytes [in]  Number of bytes in msg.
    \param raw [in]  Flag that specifies whether escape sequences, such as %%P, should not be converted to special characters.
    \param pTextStyle [in]  Pointer to the TextStyle for the text.
    \param pExtrusion [in]  Pointer to the extrusion vector.

    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_text, Work with Text>
  */
  virtual void textProc2(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0,
    const OdGeExtents3d* /*extentsBox*/ = 0)
  {
    textProc(position, direction, upVector, msg, numBytes, raw, pTextStyle, pExtrusion);
  }

  /** \details
    Processes shape data.

    \param position [in]  Position of the shape.
    \param direction [in]  Baseline direction of the shape.
    \param upVector [in]  Up vector for the shape.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
    \param pExtrusion [in]  Pointer to the extrusion vector.

    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
  */
  virtual void shapeProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    int shapeNumber, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0) = 0;

  /** \details
    Processes raster images data.

    \param origin [in]  Lower-left corner. 
    \param u [in]  Image width vector.
    \param v [in]  Image height vector.
    \param pImage [in]  Pointer to the RasterImage object.
    \param uvBoundary [in]  Array of image boundary points (may not be null).
    \param numBoundPts [in]  Number of boundary points.
    \param transparency [in]  True if and only if image transparency is on.
    \param brightness [in]  Image brightness [0.0 .. 100.0].
    \param contrast [in]  Image contrast [0.0 .. 100.0].
    \param fade [in]  Image fade value [0.0 .. 100.0].
    \sa
    <link tv_geometry_raster_images, Work with Raster Image Geometry Data>
  */
  virtual void rasterImageProc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0) = 0;

  /** \details
    Processes metafile data.

    \param origin [in]  Metafile origin.
    \param u [in]  Metafile width vector.
    \param v [in]  Metafile height vector.
    \param pMetafile [in]  Pointer to the metafile object.
    \param dcAligned [in]  reserved.
    \param allowClipping [in]  reserved.
  */
  virtual void metafileProc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,
    bool allowClipping = false) = 0;

  /** \details
    Processes Xline data.
    \param basePoint [in]  Base point.
    \param direction [in]  Direction of an xline.
    \remarks
    An Xline is an infinite line passing through the 
    specified basePoint along the passed direction.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void xlineProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction)
  {
    xlineProc(basePoint, basePoint + direction);
  }
  
  /** \details
    Processes Ray data.
    
    \param basePoint [in]  Base point.
    \param direction [in]  Direction of a ray.
    
    \remarks
    A Ray is a semi-infinite line that starts at the basePoint, and extends 
    through the specified direction.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void rayProc2(const OdGePoint3d& basePoint, const OdGeVector3d& direction)
  {
    rayProc(basePoint, basePoint + direction);
  }

  /** \details
    Sets geometric extents.
  */
  virtual void setExtentsProc(const OdGePoint3d * /*pPoints*/, bool /*bTransform*/ = true) { }

  /** \details
    TrueType fonts character processing flags.
  */
  enum ETtfCharProcFlags
  {
    kTtfCharProcEnable     = 1,
    kTtfCharProcBoundBlock = 2
  };
  
  /** \details
    Retrieves TrueType fonts character processing flags.
    
    \returns
    TrueType fonts character processing flags as integer value.
  */
  virtual int ttfCharProcFlags() const { return 0; }
  virtual bool ttfCharProc(OdChar /*character*/, bool /*isGlyph*/, const OdGePoint3d& /*position*/,
    const OdGeBoundBlock3d* /*pBoundBlock*/) { return false; }

  /** \details
    Processes TrueType font characters in dynamic polyDraw mode
    which is similar to shellProc() method, except here additional information about 
    bezier data is passed for processing.
    
    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices.
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  Array of numbers that define the faces.
    \param pBezierTypes [in]  Bezier types.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data.   
  */
  virtual void ttfPolyDrawProc(
    OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdUInt8* /*pBezierTypes*/, const OdGiFaceData* pFaceData = 0)
  {
    shellProc(numVertices, vertexList, faceListSize, faceList, 0, pFaceData);
  }
  
  enum EConveyorBoundaryInfoFlags
  {
    kBoundaryProcXform       = 1, // Non-identity Xform available at least once
    kBoundaryProcXformNonUni = 2, // Non-uni-scale Xform available at least once
    kBoundaryProcProjection  = 4, // Projective Xform available at least once
    kBoundaryProcClip        = 8, // Boundary clipped partially or completely at least once
    kBoundaryProcClipFull   = 16 // Boundary completely clipped at least once
  };
  virtual void conveyorBoundaryInfoProc(
    const OdGeBoundBlock3d & /*boundBlock*/, // {I} represent block for apply check
    OdUInt32 & /*outputFlags*/) // {O} will represent set of EConveyorBoundaryInfoFlags flags on out
  {
  }

  /** \details
    Processes an array of points.

    \param pContext [in]  Pointer to a context.
    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pColors [in]  Pointer to point colors.
    \param pTransparency [in]  Pointer to point transparency.
    \param pNormals [in]  Array of normal vectors.
    \param pExtrusions [in]  Array of vectors that defines extrusions.
    \param pSubEntMarkers [in]  Array of sub-entity markers.
    \param nPointSize [in]  Points size.
  */
  void polypointProc2(OdGiConveyorContext* pContext,
    OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors = 0,
    const OdCmTransparency *pTransparency = 0, const OdGeVector3d* pNormals = 0, const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0, OdInt32 nPointSize = 0);
  
  /** \details
    Processes a row of points.

    \param numPoints [in]  Number of points.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset to next point.
  */
  void rowOfDotsProc2(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  /** \details
    Processes array of points.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Array of vertices.
    \param pNormals [in]  Array of normal vectors.
    \param pExtrusions [in]  Array of extrusion vectors.
    \param pSubEntMarkers [in]  Array of subentity markers.
    \remarks
    The extrusion vectors specifies the direction and distance of the extrusion.
  */
  virtual void polypointProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency = 0, const OdGeVector3d* pNormals = 0, const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0, OdInt32 nPointSize = 0) = 0;

  /** \details
    Processes set of polygons.

    \param pContext [in]  Pointer to a context.
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
  void polyPolygonProc2(OdGiConveyorContext *pContext,
    OdUInt32 numIndices, const OdUInt32 *pNumPositions, const OdGePoint3d *pPositions,
    const OdUInt32 *pNumPoints, const OdGePoint3d *pPoints,
    const OdCmEntityColor *pOutlineColors = NULL, const OdUInt32 *pOutlinePsLinetypes = NULL,
    const OdCmEntityColor *pFillColors = NULL, const OdCmTransparency *pFillTransparencies = NULL);

  /** \details
    Processes row of points.

    \param numPoints [in]  Number of points.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset to next point.
  */
  virtual void rowOfDotsProc(
    OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint) = 0;

  void pointCloudProc2(OdGiConveyorContext* pContext, const OdGiPointCloud &pCloud, const OdGiPointCloudFilter *pFilter);
  virtual void pointCloudProc(const OdGiPointCloud &pCloud, const OdGiPointCloudFilter *pFilter = NULL) = 0;

  /** \details
    Processes hatch boundary.

    \param edges [in]  2d curves array.
    \param pXform [in]  Optional boundary transform matrix.
  */
  virtual void edgeProc(
    const OdGiEdge2dArray& edges, const OdGeMatrix3d* pXform = 0) = 0;
};

#endif //#ifndef __ODGICONVEYORGEOMETRY_H__
