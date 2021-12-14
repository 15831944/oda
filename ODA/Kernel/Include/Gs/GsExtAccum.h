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

#ifndef ODGSEXTACCUM_INC
#define ODGSEXTACCUM_INC

#include "TD_PackPush.h"

#include "OdArray.h"

#include "Ge/GeExtents3d.h"

#include "Gi/GiExtAccum.h"

#include "Gs/GsExport.h"
#include "Gs/Gs.h"

class OdGsViewImpl;

/** \details
    Base class for Gs conveyor node implementations.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsConveyorNodeBase : public OdGiConveyorInput,
                                               public OdGiConveyorOutput
{
protected:
  typedef OdArray<OdGiConveyorOutput*, OdMemoryAllocator<OdGiConveyorOutput*> > SourceNodeArray;
  SourceNodeArray m_sources;
  OdGiConveyorGeometry *m_pDestGeom;

  class update_geometry
  {
    OdGiConveyorGeometry* m_pGeometry;
  public:
    update_geometry(OdGiConveyorGeometry* pGeometry) : m_pGeometry(pGeometry) {}
    void operator ()(OdGiConveyorOutput* pSourceNode)
    {
      pSourceNode->setDestGeometry(*m_pGeometry);
    }
  };
public:
  /** \details
    Default constructor for the OdGsConveyorNodeBase class. Creates an object with destination geometry set to OdGiEmptyGeometry::kVoid.
  */
  OdGsConveyorNodeBase();

  /** \details
    Adds the specified source node to this Conveyor object.
    \param sourceNode [in] Source node to add.
  */
  void addSourceNode(OdGiConveyorOutput& sourceNode);
  
  /** \details
    Removes the specified source node from this Conveyor object.
    
    \param sourceNode [in] Source node to remove.
  */
  void removeSourceNode(OdGiConveyorOutput& sourceNode);

  /** \details
    Sets the destination geometry for this Conveyor object.
    
    \param destGeometry [in] Destination geometry.
  */
  void setDestGeometry(OdGiConveyorGeometry& destGeometry);
  
  /** \details
    Returns the destination geometry for this Conveyor object.
  */
  OdGiConveyorGeometry& destGeometry() const;

  /** \details
    Updates optional geometry if exits, otherwise updates destination geometry.
  */
  void updateLink();
  
  /** \details
    Updates conveyor geometry.
    
    \param pGeometry [in] Geometry to update.
  */
  void updateLink(OdGiConveyorGeometry* pGeometry);

  /** \details
    Retrieves a pointer to optional geometry.
  */
  virtual OdGiConveyorGeometry* optionalGeometry() = 0;
  
  /** \details
    Destructor for the OdGsConveyorNodeBase class.
  */
  virtual ~OdGsConveyorNodeBase() {}
};

/** \details
    Wrapper extensions accumulator class adopted for Gs.

    Corresponding C++ library: TD_Gs

    <group OdGs_Classes>
*/
class GS_TOOLKIT_EXPORT OdGsExtAccum : public OdGiExtAccum,
                                       public OdGsConveyorNodeBase,
                                       public OdGiConveyorGeometry
{
  OdGiExtAccumPtr m_pExtAccum;
  OdGsViewImpl *m_pBaseVV;
  OdDb::LineWeight m_maxLineweight;
  OdGiConveyorContext *m_pConContext;
protected:
  OdGsExtAccum();
public:
  ODRX_DECLARE_MEMBERS(OdGsExtAccum);

  // OdGsExtAccum functions

  /** \details
    Retrieves a pointer to optional geometry.
  */
  OdGiConveyorGeometry* optionalGeometry();

  /** \details
    Retrieves a pointer to the underlying OdGiExtAccum object.
  */
  OdGiExtAccum *giExtAccum() { return m_pExtAccum.get(); }
  
  /** \details
    Retrieves a pointer to the underlying OdGiExtAccum object.
  */
  const OdGiExtAccum *giExtAccum() const { return m_pExtAccum.get(); }

  /** \details
    Setup current OdGsViewImpl object for optional use of additional information into extents calculations.
    \param pBaseVV [in]  Pointer to the view
  */
  virtual void setBaseView(OdGsViewImpl *pBaseVV);
  
  /** \details
    Retrieves a pointer to a base view.
  */
  OdGsViewImpl* baseView() { return m_pBaseVV; }

  // OdGiExtAccum functions

  /** \details
    Sets the draw context associated with this object.
    \param pDrawContext [in]  Pointer to the draw context.
  */
  virtual void setDrawContext(OdGiConveyorContext* pDrawContext);
  
  /** \details
    Returns the OdGiConveyorGeometry object associated with this object.  
    
    \returns
    a reference to the OdGiConveyorGeometry instance.
    
    \remarks
    This instance contains the functions that can be used by an entity to
    vectorize itself.
  */
  virtual OdGiConveyorGeometry& geometry();
  
  /** \details
    Retrieves the accumulated extents for the OdGiConveyorGeometry object associated with this object.
    \param extents [out]  Receives the extents.
    
    \returns
    true if extents is valid, false otherwise.
  */
  virtual bool getExtents(OdGeExtents3d& extents) const;
  
  /** \details
    Resets the accumulated extents for the OdGiConveyorGeometry object associated with this object.
    \param newExtents [out]  New extents.
  */
  virtual void resetExtents(const OdGeExtents3d& newExtents = OdGeExtents3d());
  
  /** \details
    Adds the specified extents to the the accumulated extents for the OdGiConveyorGeometry object associated with this object.
    \param extents [out]  Extents to be added.
  */
  virtual void addExtents(const OdGeExtents3d& extents);
  
  /** \details
    Checks whether pline primitives contain bulges.
    
    \returns
    true if any of input pline primitives contain bulges, false otherwise.
  */
  virtual bool plineContainBulges() const;

  /** \details
    Sets max deviation for curve tessellation.
    
    \param deviations [in]  Deviation values for curve tessellation.
  */
  virtual void setDeviation(const OdGeDoubleArray& deviations)
  {
    m_pExtAccum->setDeviation(deviations);
  }
  
  /** \details
    Sets deviation object to obtain max deviation for curve tessellation.
    
    \param pDeviation [in]  Pointer to the deviation object.
  */
  virtual void setDeviation(const OdGiDeviation* pDeviation)
  {
    m_pExtAccum->setDeviation(pDeviation);
  }

  /** \details
    Returns lineweight of this object.
  */
  OdDb::LineWeight getLineweight() const;
  
  /** \details
    Replaces lineweight of this object with the passed lineweight if the passed lineweight has greater width.
    \param lw  [in]  Lineweight.
  */
  void addLineweight(OdDb::LineWeight lwd);

  // OdGiConveyorNode functions

  /** \details
    Returns the input from this ConveyorNode object.
  */
  virtual OdGiConveyorInput& input();
  
  /** \details
    Returns the output from this ConveyorNode object.
  */
  virtual OdGiConveyorOutput& output();

  // OdGiConveyorGeometry functions

  /** \details
    Processes OdGiPolyline data.

    \param polyline [in]  Polyline data to be processed.
    \param pXfm [in]  Pointer to a transformation matrix.
    \param fromIndex [in]  Index of the first segment to be processed.
    \param numSegs [in]  Number of segments to be processed.
    \remarks
    numSegs == 0 indicates all segments starting at fromIndex are to be processed. 
    \sa
    <link tv_geometry_lines.html, Work with Lines>
  */
  virtual void plineProc(const OdGiPolyline& polyline,
    const OdGeMatrix3d* pXfm = 0,
    OdUInt32 fromIndex = 0,
    OdUInt32 numSegs = 0);
  
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
    <link tv_geometry_lines.html, Work with Lines>
  */
  virtual void polylineProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker baseSubEntMarker = -1);
  
  /** \details
    Processes polygon data.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.

    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_polygons.html, Work with Polygons>
  */
  virtual void polygonProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    Processes Xline data.
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \remarks
    An Xline is an infinite line passing through the 
    specified points.
    \sa
    <link tv_geometry_lines.html, Work with Lines>
  */
  virtual void xlineProc(
    const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);
  
  /** \details
    Processes Ray data.
    
    \param basePoint [in]  Base point.
    \param throughPoint [in]  Through point
    
    \remarks
    A Ray is a semi-infinite line that starts at the basePoint, and passes 
    through the throughPoint.
    \sa
    <link tv_geometry_lines.html, Work with Lines>
  */
  virtual void rayProc(
    const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint);
  
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
    <link tv_geometry_meshes.html, Work with Meshes>
  */
  virtual void meshProc(
    OdInt32 numRows, 
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);
  
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
    <link tv_geometry_shells.html, Work with Shells>
  */
  virtual void shellProc(
    OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);
  
  /** \details
    Process circle data. 

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_circles.html, Work with Circles>
  */
  virtual void circleProc(
    const OdGePoint3d& center, 
    double radius, 
    const OdGeVector3d& normal, 
    const OdGeVector3d* pExtrusion = 0);
  
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
    <link tv_geometry_circles.html, Work with Circles>
  */
  virtual void circleProc(
    const OdGePoint3d& firstPoint, 
    const OdGePoint3d& secondPoint, 
    const OdGePoint3d& thirdPoint, 
    const OdGeVector3d* pExtrusion = 0);
  
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
    <link tv_geometry_arcs.html, Work with Arcs>
  */
  virtual void circularArcProc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);
  
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
    <link tv_geometry_arcs.html, Work with Arcs>
  */
  virtual void circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);
  
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
    <link tv_geometry_arcs.html, Work with Arcs>
  */
  virtual void ellipArcProc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    Processes OdGeNurbCurve3d data.
    \param nurbsCurve [in]  NURBS curve data.
    \sa
    <link tv_geometry_nurbs.html, Work with NURBS>
  */
  virtual void nurbsProc(
    const OdGeNurbCurve3d& nurbsCurve);
  
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
    <link tv_geometry_text.html, Work with Text>
  */
  virtual void textProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    const OdChar* msg, 
    OdInt32 numBytes, 
    bool raw, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);
  
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
    <link tv_geometry_text.html, Work with Text>
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
    const OdGeExtents3d* extentsBox = 0);
  
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
    const OdGeVector3d* pExtrusion = 0);
  
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
    <link tv_geometry_raster_images.html, Work with Raster Image Geometry Data>
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
    double fade = 0.0);
  
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
    bool allowClipping = false);
  virtual void setExtentsProc(const OdGePoint3d *pPoints, bool bTransform = true);
  
  /** \details
    Processes array of points.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Array of vertices.
    \param pColors [in]  Pointer to point colors.
    \param pTransparency [in]  Pointer to point transparency.
    \param pNormals [in]  Array of normal vectors.
    \param pExtrusions [in]  Array of vectors that defines extrusions.
    \param pSubEntMarkers [in]  Array of sub-entity markers.
    \param nPointSize [in]  Points size.
    \remarks
    The extrusion vectors specifies the direction and distance of the extrusion.
  */
  virtual void polypointProc(
    OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency = 0,
    const OdGeVector3d* pNormals = 0,
    const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0,
    OdInt32 nPointSize = 0);
  
  /** \details
    Processes a row of points.

    \param numPoints [in]  Number of points.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset to next point.
  */
  virtual void rowOfDotsProc(
    OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  /** \details
    Processes point cloud object.

    \param pCloud [in]  Point cloud object.
    \param pFilter [in]  Optional set of point cloud object conveyor filters.
  */
  virtual void pointCloudProc(
    const OdGiPointCloud &pCloud, const OdGiPointCloudFilter *pFilter = NULL);

  /** \details
    Processes hatch boundary.

    \param edges [in]  2d curves array.
    \param pXform [in]  Optional boundary transform matrix.
  */
  void edgeProc(
    const OdGiEdge2dArray& edges, const OdGeMatrix3d* pXform = 0);
};

/** \details
    This template class is a specialization of the OdSmartPtr class for OdGsExtAccum object pointers.
*/
typedef OdSmartPtr<OdGsExtAccum> OdGsExtAccumPtr;

#include "TD_PackPop.h"

#endif // ODGSEXTACCUM_INC
