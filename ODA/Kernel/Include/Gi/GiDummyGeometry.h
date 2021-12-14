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

#ifndef _ODGIDUMMYGEOM_INCLUDED_
#define _ODGIDUMMYGEOM_INCLUDED_

#include "Gi/Gi.h"
#include "Gi/GiWorldDraw.h"
#include "Gi/GiViewportDraw.h"
#include "Gi/GiSubEntityTraitsData.h"
#include "OdStack.h"
#include "StaticRxObject.h"

#include "TD_PackPush.h"

/** \details
This class is a dummy template for OdGiGeometry.
Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiWrapperGeometry : public T
{
  OdGiGeometry        *m_Geom;
protected:
  void setGeom(OdGiGeometry &Geom) { m_Geom = &Geom; }
  OdGiGeometry &getGeom() { return *m_Geom; }
public:
  
  /** \details
    Introduces a lightweight polyline into this vectorization context.

    \param polyline [in]  Polyline.
    \param fromIndex [in]  Index of the first segment to be processed.
    \param numSegs [in]  Number of segments to be processed (0 indicates all segments).
    \remarks
    The polyline may contain varying segment widths, straight segments
    and arc segments.    
    The polyline takes on the current color.  
    All points must be coplanar.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void pline(const OdGiPolyline& p, OdUInt32 i, OdUInt32 n)
  {
    m_Geom->pline(p, i, n);
  }

  /** \details
    Retrieves the model-to-world coordinate transform matrix for the entity.
    
    \returns
    model-to-world coordinate transform matrix.    
    \remarks
    This is the inverse of the matrix returned by getWorldToModelTransform(). 
    If an entity is in one or more blocks, this matrix can be used to
    determine the WCS coordinates of the entity.  
  */
  virtual OdGeMatrix3d getModelToWorldTransform() const
  {
    return m_Geom->getModelToWorldTransform();
  }
  
  /** \details
    Retrieves the world-to-model coordinate transform matrix for the entity being vectorized.
    
    \returns
    world-to-model coordinate transform matrix.    
    \remarks
    This is the inverse of the matrix returned by getModelToWorldTransform(). 
  */
  virtual OdGeMatrix3d getWorldToModelTransform() const
  {
    return m_Geom->getWorldToModelTransform();
  }

  /** \details
    Specifies a model transformation vector for the current transformation stack.  

    \param vNormal [in]  Normal vector.  
    \remarks
    When a vector is specified, the transformation is created using the arbitrary axis algorithm 
    on the vector.
    The specified transformation is concatenated to the current model transformation 
    (which is initially the identity transform).
  */
  virtual void pushModelTransform(const OdGeVector3d& vNormal)
  {
    m_Geom->pushModelTransform(vNormal);
  }
  
  /** \details
    Specifies a model transformation matrix for the current transformation stack.  
      
    \param xMat [in]  Transformation matrix. 
    \remarks
    The specified transformation is concatenated to the current model transformation 
    (which is initially the identity transform).     
  */
  virtual void pushModelTransform(const OdGeMatrix3d& xMat)
  {
    m_Geom->pushModelTransform(xMat);
  }
  
  /** \details
    Removes the top transformation off the current transformation stack.
  */
  virtual void popModelTransform()
  {
    m_Geom->popModelTransform();
  }

  // For drawing various primitives.
  //
  
  /** \details
    Introduces a circle to the vectorization context.  

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal.
    \remarks
    The circle is not filled and takes on the current color.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  virtual void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal)
  {
    m_Geom->circle(center, radius, normal);
  }

  /** \details
    Introduces a circle to the vectorization context.  

    \param p1 [in]  First point.
    \param p2 [in]  Second point.
    \param p3 [in]  Third point. 
    \remarks
    The circle is not filled and takes on the current color. Points must not be co-linear.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  virtual void circle(const OdGePoint3d& p1, const OdGePoint3d& p2, const OdGePoint3d& p3)
  {
    m_Geom->circle(p1, p2, p3);
  }

  /** \details
    Introduces a circular arc to the vectorization context.  
    
    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param startVector [in]  Start of the arc.
    \param sweepAngle [in]  Angle that defines the arc.
    \param arcType [in]  Arc type.
    
    \remarks
    The arc takes on the current color.
    If firstPoint, secondPoint, and thirdPoint are specified, they
    cannot be colinear and no two can be coincident. All angles are expressed in radians.
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
  virtual void circularArc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    m_Geom->circularArc(center, radius, normal, startVector, sweepAngle, arcType);
  }

  /** \details
    Introduces a circular arc to the vectorization context.
    
    \param start [in]  First point of a 3-point arc.
    \param point [in]  Second point of a 3-point arc.
    \param end [in]  Third point of a 3-point arc.
    \param arcType [in]  Arc type.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  virtual void circularArc(const OdGePoint3d& start,
    const OdGePoint3d& point,
    const OdGePoint3d& end,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    m_Geom->circularArc(start, point, end, arcType);
  }

  /** \details
    Introduces a polyline to the vectorization context.  

    \param nbPoints [in]  Number of vertices.
    \param pVertexList [in]  Pointer to an array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param baseSubEntMarker [in] Sub-entity marker. Not used.
    \remarks
    The polyline is unfilled and takes on the current color and thickness. Use polygon() to render filled areas.  
    The polyline is rendered as a series of lines connecting the first point
    in vertexList to the second, the second to the third, etc.
    All points must be coplanar.
    Use polygon() to render closed areas.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void polyline(OdInt32 nbPoints,
    const OdGePoint3d* pVertexList,
    const OdGeVector3d* pNormal = NULL,
    OdGsMarker lBaseSubEntMarker = -1)
  {
    m_Geom->polyline(nbPoints, pVertexList, pNormal, lBaseSubEntMarker);
  }

  /** \details
    Introduces a polygon to this vectorization context.  
    
    \remarks
    The polygon is rendered as a series of lines connecting the first point
    in vertexList to the second, the second to the third, etc.
    All points must be coplanar.
    \param nbPoints [in]  Number of vertices.
    \param pVertexList [in]  Pointer to an array of vertices.
    \sa
    <link tv_geometry_polygons, Work with Polygons>
  */
  virtual void polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList)
  {
    m_Geom->polygon(nbPoints, pVertexList);
  }
  
  /** \details
  Introduces a polygon (with a normal) to the vectorization context.

  \remarks
  The polygon is rendered as a series of lines connecting the first point
  in vertexList to the second, the second to the third, etc.
  All points must be coplanar.
  \param nbPoints [in]  Number of vertices.
  \param pVertexList [in]  Pointer to an array of vertices.
  \param pNormal [in]  Pointer to the normal vector.
  \sa
  <link tv_geometry_polygons, Work with Polygons>
  */
  virtual void polygon(OdInt32 nbPoints, const OdGePoint3d* pVertexList, const OdGeVector3d* pNormal)
  {
    m_Geom->polygon(nbPoints, pVertexList, pNormal);
  }

  /** \details
    Introduces a mesh into the vectorization context.  
    
    \param rows [in]  Number of rows.
    \param columns [in]  Number of columns.
    \param pVertexList [in]  Pointer to an array of vertices.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data. 
    \remarks
    A mesh is a surface defined by a grid of vertices, and corresponds to a Polygon Mesh.
    By default, a mesh takes on the current color. Color, linetype, and
    and other properties can be controlled supplying the appropriate
    data for the pEdgeData, pFaceData, and pVertexData arguments.
    \remarks
    * vertexList must have numRows x numColumns elements.
    * The arrays in *pEdgeData must be NULL or contain (numRows - 1) x numColumns + numRows x (numColumns -1) elements.
    * The arrays in *pFaceData must be NULL or contain (numRows - 1) x (numColumns - 1) elements.
    * The arrays in *pVertexData must be NULL or contain numRows x numColumns elements. 
    \sa
    <link tv_geometry_meshes, Work with Meshes>
  */
  virtual void mesh(OdInt32 rows,
    OdInt32 columns,
    const OdGePoint3d* pVertexList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL)
  {
    m_Geom->mesh(rows, columns, pVertexList,  pEdgeData, pFaceData, pVertexData);
  }

  /** \details
    Introduces a shell into the vectorization context.  
    
    \param nbVertex [in]  Number of vertices.
    \param pVertexList [in]  Pointer to an array of vertices.
    \param faceListSize [in]  Number of entries in facesList.
    \param pFaceList [in]  Array of integers defining faces.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data.   
    \remarks
    A shell is a set of faces that can contain holes, and corresponds to a Polyface mesh.    
    \remarks
    * The arrays in *pEdgeData must be NULL or contain the same number of elements as there are edges.
    * The arrays in *pFaceData must be NULL or contain the same number of elements as there are faces.
    * The arrays in *pVertexData must be NULL or contain numVertices elements. 
    \sa
    <link tv_geometry_shells, Work with Shells>
  */
  virtual void shell(OdInt32 nbVertex,
    const OdGePoint3d* pVertexList,
    OdInt32 faceListSize,
    const OdInt32* pFaceList,
    const OdGiEdgeData* pEdgeData = NULL,
    const OdGiFaceData* pFaceData = NULL,
    const OdGiVertexData* pVertexData = NULL)
  {
    m_Geom->shell(nbVertex, pVertexList, faceListSize, pFaceList, pEdgeData, pFaceData, pVertexData);
  }

   /** \details
    Introduces text into the vectorization context.
    
    \param position [in]  Position of the text string.
    \param normal [in]  Normal to a plane where text lies. Used to define v parameter as normal.crossProduct(direction).
    \param direction [in]  Direction vector that is interpreted as u parameter.
    \param height [in]  Height of the text.
    \param width [in]  Width factor of the text.
    \param oblique [in]  Oblique angle. Must not exceed an equivalent of 85 degrees in radians in both directions.
    \param msg [in]  Text string.
    \remarks
    If numBytes is not specified, msg must be null terminated.
    All angles are expressed in radians.
    \sa
    <link tv_geometry_text, Work with Text>
  */
  virtual void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    double height, double width, double oblique, const OdString& msg)
  {
    m_Geom->text(position, normal, direction, height, width, oblique, msg);
  }

  /** \details
    Introduces text into the vectorization context.
    
    \param position [in]  Position of the text string.
    \param normal [in]  Normal to a plane where text lies. Used to define v parameter as normal.crossProduct(direction).
    \param direction [in]  Direction vector that is interpreted as u parameter.
    \param msg [in]  Text string.
    \param length [in]  Number of characters in msg (not including the optional null character).
    \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters.
    \param pTextStyle [in]  Pointer to the TextStyle for the text.
    \sa
    <link tv_geometry_text, Work with Text>
  */
  virtual void text(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle)
  {
    m_Geom->text(position, normal, direction, msg, length, raw, pTextStyle);
  }

  /** \details
    Introduces a ray into the vectorization context.
    
    \param first [in]  Base point.
    \param second [in]  Point through which a ray extends.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void ray(const OdGePoint3d& first, const OdGePoint3d& second)
  {
    m_Geom->ray(first, second);
  }

  /** \details
    Introduces nurbs into the vectorization context.
    
    \param nurbs [in]  NURBS curve data.
    \sa
    <link tv_geometry_nurbs, Work with NURBS>
  */
  virtual void nurbs(const OdGeNurbCurve3d& nurbs)
  {
    m_Geom->nurbs(nurbs);
  }

  /** \details
    Introduces an xline into the vectorization context.
    
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void xline(const OdGePoint3d& p1, const OdGePoint3d& p2)
  {
    m_Geom->xline(p1, p2);
  }

  /** \details
    Introduces an elliptical arc into the vectorization context.

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
  virtual void ellipArc(const OdGeEllipArc3d& arc,
    const OdGePoint3d* pEndPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple)
  {
    m_Geom->ellipArc(arc, pEndPointsOverrides, arcType);
  }

  /** \details
    Introduces the specified object into the vectorization context.
    
    \param pD [in]  Pointer to the Drawable object.

    \remarks
    Implementations of this method are expected to do the following:
    * Call OdGiDrawable::subSetAttributes to set attribute information for the object.
    * Call subWorldDraw on the drawable object  to vectorize it into this context.
    * If subWorldDraw returns false, call subViewportDraw each viewport.
  */
  virtual void draw(const OdGiDrawable* pD)
  {
    m_Geom->draw(pD);
  }

  /** \details
    Specifies a clip boundary for the current clip stack.

    \param pBoundary [in]  Pointer to the boundary.
    \remarks
    Subsequent objects are clipped until popClipBoundary() is called.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  virtual void pushClipBoundary(OdGiClipBoundary* pBoundary)
  {
    m_Geom->pushClipBoundary(pBoundary);
  }
  
  /** \details
    Removes the top clip boundary from the clip stack.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  virtual void popClipBoundary()
  {
    m_Geom->popClipBoundary();
  }

  /** \details
    Introduces a line into the vectorization context.  

    \param points [in]  Array of WCS start and end points.

	  \remarks
    The current model transformation is not applied to the line.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void worldLine(const OdGePoint3d pnts[2])
  {
    m_Geom->worldLine(pnts);
  }

  /** \details
    Introduces an image in BGRA format into the vectorization context.
    
    \param img [in]  Image in BGRA format.
    \param origin [in]  Image position in WCS.
    \param uVec [in]  Image width and X-axis orientation.
    \param vVec [in]  Image height and Y-axis orientation.
    \param trpMode [in]  Image transparency processing mode.
    \sa
    <link tv_geometry_raster_images, Work with Raster Image Geometry Data>
  */
  virtual void image(const OdGiImageBGRA32& img,
                     const OdGePoint3d& origin,
                     const OdGeVector3d& uVec,
                     const OdGeVector3d& vVec,
                     OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit)
  {
    m_Geom->image(img, origin, uVec, vVec, trpMode);
  }

  /** \details
    Introduces a boundary for hatch filling.
    
    \param edges [in]  Array of 2d curves which represents filling boundary.
  */
  virtual void edge(const OdGiEdge2dArray& edges)
  {
    m_Geom->edge(edges);
  }

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
  virtual void polypoint(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency,
                         const OdGeVector3d* pNormals, const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize)
  {
    m_Geom->polypoint(numPoints, vertexList, pColors, pTransparency, pNormals, pSubEntMarkers, nPointSize);
  }

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
                           const OdCmEntityColor *pFillColors = NULL, const OdCmTransparency *pFillTransparencies = NULL)
  {
    m_Geom->polyPolygon(numIndices, pNumPositions, pPositions, pNumPoints, pPoints,
                        pOutlineColors, pOutlinePsLinetypes, pFillColors, pFillTransparencies);
  }

  /** \details
    Introduces a row of points into this vectorization context.

    \param numPoints [in]  Number of points.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset to next point.
  */
  virtual void rowOfDots(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint)
  {
    m_Geom->rowOfDots(numPoints, startPoint, dirToNextPoint);
  }

  /** \details
    Introduces a Point Clound object into this vectorization context.

    \param pCloud [in]  Point Cloud object.
  */
  virtual void pointCloud(const OdGiPointCloud &pCloud)
  {
    m_Geom->pointCloud(pCloud);
  }

  /** \details
    Introduces a BREP into this vectorization context.

    \param giBrep [in]  BREP.
    \return Returns true, if BREP input is supported, false otherwise
  */
  virtual bool brep( const OdGiBrep& giBrep )
  {
    return m_Geom->brep( giBrep );
  }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiWrapperWorldGeometry : public OdGiWrapperGeometry< T >
{
public:
  
  /** \details
    Sets geometrical extents.
    
    \param newExtents [in]  New geometrical extents to set.
  */
  virtual void setExtents(const OdGePoint3d *newExtents) { ((OdGiWorldGeometry&)(OdGiWrapperGeometry< T >::getGeom())).setExtents(newExtents); }
  
  /** \details
    Sets the OdGiBaseVectorizer::kAttributesSegment flag for the associated vectorizer to true.
  */
  virtual void startAttributesSegment() { ((OdGiWorldGeometry&)(OdGiWrapperGeometry< T >::getGeom())).startAttributesSegment(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiWrapperViewportGeometry : public OdGiWrapperGeometry< T >
{
public:
  /** \details
    Introduces a new polyline but in eye coordinate system.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  virtual void polylineEye(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).polylineEye(numVertices, vertexList); }

  /** \details
    Introduces a new polygon but in eye coordinate system.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  virtual void polygonEye(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).polygonEye(numVertices, vertexList); }

  /** \details
    Introduces a new polyline but in device coordinate system.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  virtual void polylineDc(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).polylineDc(numVertices, vertexList); }

  /** \details
    Introduces a new polygon but in device coordinate system.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  virtual void polygonDc(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).polygonDc(numVertices, vertexList); }

  /** \details
    Introduces a raster image into the vectorization context in device coordinate system.
  
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
  virtual void rasterImageDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).rasterImageDc(origin,u,v,pImage, uvBoundary, numBoundPts,transparency,brightness,contrast,fade); }

  /** \details
    Introduces a metafile into the vectorization context in device coordinate system.
  
    \param origin         [in]  Origin of a metafile content.
    \param u              [in]  Metafile content u-parameter.
    \param v              [in]  Metafile content v-parameter.
    \param pMetafile      [in]  Pointer to a metafile.
    \param bDcAligned     [in]  Flag that specifies whether metafile content should be aligned to device coordinate system.
    \param bAllowClipping [in]  Flag that specifies whether clipping should be allowed for a metafile.
  */
  virtual void  metafileDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,
    bool allowClipping = false) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).metafileDc(origin,u,v,pMetafile,dcAligned,allowClipping); }

  /** \details
    Passes to the visualization framework an object that can render itself.

    \param origin [in]  Object origin.
    \param u [in]  Object width vector.
    \param v [in]  Object height vector.
    \param pDrawable [in]  Pointer to the drawable.
    \param dcAligned     [in]  Flag that specifies whether drawable should be aligned to device coordinate system.
    \param allowClipping [in]  Flag that specifies whether clipping should be allowed.
    \remarks
    ODA Platform uses this function to render Ole2Frame entities. 
    
    \remarks
    ownerDrawDc is not conveyor primitive, so the ODA Platform visualization framework performs no processing 
    (transformation, linetyping, or clipping) for this object. 
    All coordinates are in Model coordinates.
  */
  virtual void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool dcAligned = true,
    bool allowClipping = false) { ((OdGiViewportGeometry&)(OdGiWrapperGeometry< T >::getGeom())).ownerDrawDc(origin,u,v,pDrawable,dcAligned,allowClipping); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class TInterface, class TData>
class OdGiSubEntityTraitsToDataWrapper : public TInterface
{
  protected:
    TData *m_pTraits;
  public:
    
    /** \details
      Default constructor for the OdGiSubEntityTraitsToDataWrapper class.
    */
    OdGiSubEntityTraitsToDataWrapper() : m_pTraits(NULL) {}

    /** \details
      Sets entity traits for this object.
    */
    void setTraits(TData &pTraits) { m_pTraits = &pTraits; }
    
    /** \details
      Retrieves entity traits for this object.
    */
    TData &getTraits() const { return *m_pTraits; }

    /** \details
    Sets the current color index (ACI).

    \param n [in]  Color index.
    \remarks
    The n parameter must be one of the following:
    
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
    void setColor(OdUInt16 n) { m_pTraits->setColor(n); }
    
    /** \details
      Sets the current color from an OdCmEntityColor instance.
      
      \param n [in]  OdCmEntityColor object.
      \sa
      <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
    */
    void setTrueColor(const OdCmEntityColor& n) { m_pTraits->setTrueColor(n); }
    
    /** \details
      Sets the current layer.
      \param n [in]  Object ID of the LayerTableRecord.  
      \sa
      <link tv_working_with_attributes_layers, Work with Layers>
    */
    void setLayer(OdDbStub* n) { m_pTraits->setLayer(n); }
    
    /** \details
      Sets the current linetype.
      \param n [in]  Object ID of the LinetypeTableRecord.
      \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
    */
    void setLineType(OdDbStub* n) { m_pTraits->setLineType(n); }
    
    /** \details
      Sets the current selection marker.
    */
    void setSelectionMarker(OdGsMarker /*n*/) { } // Stub
    
    /** \details
      Sets the current fill type.
      \param n [in]  Fill type.
      
      \remarks
      The n parameter must be one of the following:
      
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
    void setFillType(OdGiFillType n) { m_pTraits->setFillType(n); }
    
    /** \details
      Sets the current fill plane. Currently not implemented.
    */
    void setFillPlane(const OdGeVector3d* /*n*/ = 0) { } // Stub
    
    /** \details
      Sets the current lineweight.
  
      \param n [in]  Lineweight.
      \sa
      <link tv_working_with_attributes_lineweights, Work with Lineweights>
    */
    void setLineWeight(OdDb::LineWeight n) { m_pTraits->setLineWeight(n); }
    
    /** \details
      Sets the current linetype scale.
      \param n [in]  Linetype scale factor.
      \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
    */
    void setLineTypeScale(double n = 1.0) { m_pTraits->setLineTypeScale(n); }
    
    /** \details
      Sets the current thickness.
      \param n [in]  Thickness.
    */
    void setThickness(double n) { m_pTraits->setThickness(n); }
    
    /** \details
    Sets the current PlotStyleName.

      \param n [in]  Plot Style Name Type.
      \param m [in]  Object ID of the Plot Style Name.      
      \remarks
      The m parameter is used only when n == kPlotStyleNameById.     
      The n parameter must be one of the following:
      
      <table>
      Name                           Value                         
      kPlotStyleNameByLayer          0
      kPlotStyleNameByBlock          1
      kPlotStyleNameIsDictDefault    2
      kPlotStyleNameById             3
      </table>
    */
    void setPlotStyleName(OdDb::PlotStyleNameType n, OdDbStub* m = 0) { m_pTraits->setPlotStyleName(n, m); }
    
    /** \details
      Sets the current material.
      \param n [in]  Object ID of the material.
      \sa
      <link tv_working_with_attributes_materials, Work with Materials>
    */
    void setMaterial(OdDbStub* n) { m_pTraits->setMaterial(n); }
    
    /** \details
      Sets the current material mapper.
      \param n [in]  Pointer to the GI mapper object.
      \sa
      <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
    */
    void setMapper(const OdGiMapper * n) { m_pTraits->setMapper(n); }
    
    /** \details
      Sets the current visual style.
      \param n [in]  Object ID of the visual style.
      \sa
      <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
    */
    void setVisualStyle(const OdDbStub* n) { m_pTraits->setVisualStyle(const_cast<OdDbStub*>(n)); }
    
    /** \details
      Sets the current transparency.
    
      \param transparency [in]  OdCmTransparency object.
    */
    void setTransparency(const OdCmTransparency& transparency) { m_pTraits->setTransparency(transparency); }
    
    /** \details
      Sets the drawing flags.
    
      \param drawFlags [in]  Input bitmask with DrawFlags.
    */
    void setDrawFlags(OdUInt32 drawFlags) { m_pTraits->setDrawFlags(drawFlags); }
    
    /** \details
      Sets the lock flags.
    
      \param lockFlags [in]  Input bitmask with LockFlags.
    */
    void setLockFlags(OdUInt32 lockFlags) { m_pTraits->setLockFlags(lockFlags); }
    
    /** \details
      Sets selection geometry flag.
    
      \param bSelectionFlag [in]  New selection geometry flag.
      \sa
      <link tv_working_with_features_selections, Work with Selections>
    */
    void setSelectionGeom(bool bSelectionFlag) { m_pTraits->setSelectionGeom(bSelectionFlag); }
    
    /** \details
      Sets the shadow flags.
    
      \param shadowFlags [in]  Input ShadowFlags.
      \sa
      <link gi_shadows_rendering, Shadows Visualization>
    */
    void setShadowFlags(OdGiSubEntityTraits::ShadowFlags shadowFlags) { m_pTraits->setShadowFlags(shadowFlags); }
    
    /** \details
      Sets sectionable flag.
    
      \param bSectionableFlag [in]  New sectionable flag.
      \sa
      <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
    */
    void setSectionable(bool bSectionableFlag) { m_pTraits->setSectionable(bSectionableFlag); }
    
    /** \details
      Sets the selection flags.
    
      \param selectionFlags [in]  Input SelectionFlags.
    */
    void setSelectionFlags(OdGiSubEntityTraits::SelectionFlags selectionFlags) { m_pTraits->setSelectionFlags(selectionFlags); }
    
    /** \details
      Sets the current secondary color from an OdCmEntityColor instance.
    
      \param color [in]  OdCmEntityColor object.
    */
    void setSecondaryTrueColor(const OdCmEntityColor& n) { m_pTraits->setSecondaryTrueColor(n); }
    
    /** \details
      Sets Dgn Line Style modifiers.

      \param pLSMod [in]  Pointer to structure with Dgn Line Style modifiers.
    */
    void setLineStyleModifiers(const OdGiDgLinetypeModifiers* pLSMod) { m_pTraits->setLineStyleModifiers(pLSMod); }
    
    /** \details
      Sets boundary filling.

      \param pFill [in]  Boundary filling parameters.
    */
    void setFill(const OdGiFill* pFill) { m_pTraits->setFill(pFill); }
    
    /** \details
      Sets pointer to auxiliary data context.

      \param pAuxData [in]  Smart pointer to auxiliary data context.
    */
    void setAuxData(const OdGiAuxiliaryDataPtr& pAuxData) { m_pTraits->setAuxData(pAuxData); }

    /** \details
      Retrieves color index (ACI).

      \returns
      Color index as OdUInt16 value.
      \sa
      <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
    */
    OdUInt16 color() const { return m_pTraits->color(); }
    
    /** \details
      Retrieves entity color.

      \returns
      Entity color as OdCmEntityColor instance.
      \sa
      <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
    */
    OdCmEntityColor trueColor() const { return m_pTraits->trueColor(); }
    
    /** \details
      Retrieves the current layer.
        
      \returns
      Pointer to the OdDbStub that represents layer identifier.
      \sa
      <link tv_working_with_attributes_layers, Work with Layers>
    */
    OdDbStub* layer() const { return m_pTraits->layer(); }
    
    /** \details
      Retrieves the current linetype.
        
      \returns
      Pointer to the OdDbStub that represents linetype identifier.
      \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
    */
    OdDbStub* lineType() const { return m_pTraits->lineType(); }
    
    /** \details
      Retrieves the current fill type.
        
      \returns
      One of the following values:
      
      <table>
      Name              Value   Description
      kOdGiFillAlways   1       Fill closed graphic primitives.
      kOdGiFillNever    2       Don't fill closed graphic primitives.
      </table>
    */
    OdGiFillType fillType() const { return m_pTraits->fillType(); }
    
    /** \details
      Retrieves the current fill plane. Currently not implemented.
        
      \returns
      false in current implementation.
    */
    bool fillPlane(OdGeVector3d& /*n*/) { return false; } // Stub
    
    /** \details
      Retrieves the current lineweight.
      
      \returns
      Current lineweight as an OdDb::LineWeight value.
      \sa
      <link tv_working_with_attributes_lineweights, Work with Lineweights>
    */
    OdDb::LineWeight lineWeight() const { return m_pTraits->lineWeight(); }
    
    /** \details
      Retrieves the current linetype scale.
      
      \returns
      Linetype scale as a value of the double type.
      \sa
      <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
    */
    double lineTypeScale() const { return m_pTraits->lineTypeScale(); }
    
    /** \details
      Retrieves the current thickness.
      
      \returns
      Current thickness as a value of the double type.
    */
    double thickness() const { return m_pTraits->thickness(); }
    
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
    OdDb::PlotStyleNameType plotStyleNameType() const { return m_pTraits->plotStyleNameType(); }
    
    /** \details
      Retrieves object ID of the the plot style name.
      
      \returns
      Pointer to the OdDbStub that represents a plot style name identifier.
    */
    OdDbStub* plotStyleNameId() const { return  m_pTraits->plotStyleNameId(); }
    
    /** \details
      Retrieves the identifier of the material.
      
      \returns
      Pointer to the OdDbStub that represents a material identifier.
      \sa
      <link tv_working_with_attributes_materials, Work with Materials>
    */
    OdDbStub* material() const { return m_pTraits->material(); }
    
    /** \details
      Retrieves pointer to the current material mapper.
      
      \returns
      Pointer to the OdDbStub that represents a mapper identifier.
      \sa
      <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
    */
    const OdGiMapper* mapper() const { return m_pTraits->mapper(); }
    
    /** \details
      Retrieves an identifier of the visual style.
      
      \returns
      Pointer to the OdDbStub that represents a visual style identifier.
      \sa
      <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
    */
    OdDbStub* visualStyle() const { return m_pTraits->visualStyle(); }
    
    /** \details
      Retrieves the current transparency.
      
      \returns
      Transparency as an instance of the OdCmTransparency class.
      \sa
      <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
      \sa
      <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
    */
    OdCmTransparency transparency() const { return m_pTraits->transparency(); }
    
    /** \details
      Retrieves the current DrawFlags bitmask.
      
      \returns
      Draw bit flags as the OdUInt32 value.
    */
    OdUInt32 drawFlags() const { return m_pTraits->drawFlags(); }
    
    /** \details
      Retrieves the current LockFlags bitmask.
      
      \returns
      Lock bit flags as the OdUInt32 value.
    */
    OdUInt32 lockFlags() const { return m_pTraits->lockFlags(); }
    
    /** \details
      Retrieves the current selection geometry flag.
      
      \returns
      true if the selection geometry flag is used, false otherwise.
      \sa
      <link tv_working_with_features_selections, Work with Selections>
    */
    bool selectionGeom() const { return m_pTraits->selectionGeom(); }
    
    /** \details
      Retrieves the current ShadowFlags.
      
      \returns
      One of the ShadowFlags enumeration values.
      \sa
      <link gi_shadows_rendering, Shadows Visualization>
    */
    OdGiSubEntityTraits::ShadowFlags shadowFlags() const { return m_pTraits->shadowFlags(); }
    
    /** \details
      Retrieves the current sectionable flag.
      
      \returns
      true if the sectionable flag is used for geometry, false otherwise.
      \sa
      <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
    */
    bool sectionable() const { return m_pTraits->sectionable(); }
    
    /** \details
      Retrieves the current SelectionFlags.
      
      \returns
      One of the SelectionFlags enumeration values.
    */
    OdGiSubEntityTraits::SelectionFlags selectionFlags() const { return m_pTraits->selectionFlags(); }
    
    /** \details
      Retrieves the current secondary color.
      
      \returns
      secondary color as OdCmEntityColor instance.
    */
    OdCmEntityColor secondaryTrueColor() const { return m_pTraits->secondaryTrueColor(); }
    
    /** \details
      Retrieves pointer to the current Dgn Line Style modifiers structure.
      
      \returns
      pointer to the OdGiDgLinetypeModifiers.
    */
    const OdGiDgLinetypeModifiers* lineStyleModifiers() const { return m_pTraits->lineStyleModifiers(); }
    
    /** \details
      Retrieves boundary filling parameters.
      
      \returns
      pointer to the instance of the OdGiFill class.
    */
    const OdGiFill* fill() const { return m_pTraits->fill(); }
    
    /** \details
      Retrieves pointer to auxiliary data context.
      
      \returns
      smart pointer to the auxiliary data context object.
    */
    OdGiAuxiliaryDataPtr auxData() const { return m_pTraits->auxData(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiSubEntityTraitsToData : public OdGiSubEntityTraitsToDataWrapper<OdStaticRxObject<OdGiSubEntityTraits>, OdGiSubEntityTraitsData>
                                , public OdGiSubEntityTraitsChangedFlags
{
  public:
    /** \details
      Default constructor for the OdGiSubEntityTraitsToData class.
    */
    OdGiSubEntityTraitsToData() {}
    
    /** \details
      Constructor for the OdGiSubEntityTraitsToData class. Initializes the object with the specified traits.
      
      \param pData [in]  Entity traits to set.
    */
    OdGiSubEntityTraitsToData(OdGiSubEntityTraitsData &pData) { setTraits(pData); }

    
    /** \details
      Copies traits of the pFrom object to the pTo object.
      
      \param pFrom [in]  Traits to copy from.
      \param pTo [in]  Traits to copy to.
      \param nFlags [in]  Traits flags.
    */
    static void copyTraits(OdGiSubEntityTraits &pFrom, OdGiSubEntityTraits &pTo, OdUInt32 nFlags = kAllChanged)
    {
      if (GETBIT(nFlags, kColorChanged)) pTo.setTrueColor(pFrom.trueColor());
      if (GETBIT(nFlags, kLayerChanged)) pTo.setLayer(pFrom.layer());
      if (GETBIT(nFlags, kLineTypeChanged)) pTo.setLineType(pFrom.lineType());
      if (GETBIT(nFlags, kFillTypeChanged)) pTo.setFillType(pFrom.fillType());
      if (GETBIT(nFlags, kLineWeightChanged)) pTo.setLineWeight(pFrom.lineWeight());
      if (GETBIT(nFlags, kLineTypeScaleChanged)) pTo.setLineTypeScale(pFrom.lineTypeScale());
      if (GETBIT(nFlags, kThicknessChanged)) pTo.setThickness(pFrom.thickness());
      if (GETBIT(nFlags, kPlotStyleChanged)) pTo.setPlotStyleName(pFrom.plotStyleNameType(), pFrom.plotStyleNameId());
      if (GETBIT(nFlags, kMaterialChanged)) pTo.setMaterial(pFrom.material());
      if (GETBIT(nFlags, kMapperChanged)) pTo.setMapper(pFrom.mapper());
      if (GETBIT(nFlags, kVisualStyleChanged)) pTo.setVisualStyle(pFrom.visualStyle());
      if (GETBIT(nFlags, kTransparencyChanged)) pTo.setTransparency(pFrom.transparency());
      if (GETBIT(nFlags, kDrawFlagsChanged)) pTo.setDrawFlags(pFrom.drawFlags());
      if (GETBIT(nFlags, kLockFlagsChanged)) pTo.setLockFlags(pFrom.lockFlags());
      if (GETBIT(nFlags, kSelectionGeomChanged)) pTo.setSelectionGeom(pFrom.selectionGeom());
      if (GETBIT(nFlags, kShadowFlagsChanged)) pTo.setShadowFlags(pFrom.shadowFlags());
      if (GETBIT(nFlags, kSectionableChanged)) pTo.setSectionable(pFrom.sectionable());
      if (GETBIT(nFlags, kSelectionFlagsChanged)) pTo.setSelectionFlags(pFrom.selectionFlags());
      if (GETBIT(nFlags, kSecColorChanged)) pTo.setSecondaryTrueColor(pFrom.secondaryTrueColor());
      if (GETBIT(nFlags, kLSModifiersChanged)) pTo.setLineStyleModifiers(pFrom.lineStyleModifiers());
      if (GETBIT(nFlags, kFillChanged)) pTo.setFill(pFrom.fill());
      if (GETBIT(nFlags, kAuxDataChanged)) pTo.setAuxData(pFrom.auxData());
    }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiSubEntityTraitsWrapper : public OdGiSubEntityTraitsToDataWrapper<T, OdGiSubEntityTraits>
{
private:
  OdGiSubEntityTraits *&baseTraits() { return OdGiSubEntityTraitsToDataWrapper<T, OdGiSubEntityTraits>::m_pTraits; }
protected:
  void setTraits(OdGiSubEntityTraits &pTraits) { baseTraits() = &pTraits; }
  OdGiSubEntityTraits &getTraits() { return *baseTraits(); }
public:
  /** \details
    Default constructor for the OdGiSubEntityTraitsToData class.
  */
  OdGiSubEntityTraitsWrapper() { }

  //DOM-IGNORE-BEGIN
  // no refcounting
  void addRef() { baseTraits()->addRef(); }
  // no refcounting
  void release() { baseTraits()->release(); }
  //DOM-IGNORE-END

  /** \details
    Sets the current selection marker.
    \param n [in]  Selection marker.
  */
  void setSelectionMarker(OdGsMarker n) { baseTraits()->setSelectionMarker(n); }
  
  /** \details
    Sets the current fill plane.
    
    \param n [in]  Normal to a plane.
  */
  void setFillPlane(const OdGeVector3d* n = 0) { baseTraits()->setFillPlane(n); }

  /** \details
    Retrieves the normal to the current fill plane.
    
    \param normal [out]  Receives the normal.
    
    \returns
    false in the current implementation.
  */
  bool fillPlane(OdGeVector3d& n) { return baseTraits()->fillPlane(n); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiWrapperCommonDraw : public T
{
  OdGiCommonDraw *m_pD;
protected:
  void setDraw(OdGiCommonDraw &pD) { m_pD = &pD; }
  OdGiCommonDraw *getDraw() const { return m_pD; }
public:
  /** \details
    Default constructor for the OdGiWrapperCommonDraw class.
  */
  OdGiWrapperCommonDraw() : m_pD(0) {}
  
  /** \details
    Retrieves raw geometry associated with this object.
    
    \returns
    raw geometry.
  */
  virtual OdGiGeometry& rawGeometry() const            { return m_pD->rawGeometry(); }
  
  /** \details
    Returns the regeneration type for this object.

    \returns
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
  virtual OdGiRegenType regenType() const              { return m_pD->regenType(); }
  
  /** \details
    Checks whether the vectorization process associated with this object should be aborted.
    
    \returns
    true if this vectorization process should be aborted, false otherwise.
  */
  virtual bool regenAbort() const                      { return m_pD->regenAbort(); }
  
  /** \details
    Provides access to this object's sub-entity traits.

    \returns
    sub-entity traits of this object.
    
    \remarks
    This allows the modification of the vectorization attributes such as color, linetype, etc.
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const { return m_pD->subEntityTraits(); }
  
  /** \details
    Checks whether the associated vectorization process is the result of a "drag" operation.
    
    \returns
    true if vectorization process is the result of a "drag" operation, false otherwise.
  */
  virtual bool isDragging() const                      { return m_pD->isDragging(); }
  
  /** \details
    Retrieves context associated with this object.
    
    \returns
    Pointer to the associated context.
  */
  virtual OdGiContext* context() const                 { return m_pD->context(); }
  
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
  virtual OdUInt32 numberOfIsolines() const            { return m_pD->numberOfIsolines(); }
  
  /** \details
    Retrieves the current drawable nesting graph.
    
    \returns
    pointer to a current drawable nesting graph.
  */
  virtual const OdGiPathNode* currentGiPath() const    { return m_pD->currentGiPath(); }
  
  /** \details
    Checks whether generation of silhouettes is enabled for modeler geometry.
    
    \returns
    true if generation of silhouettes is enabled for modeler geometry.
  */
  virtual bool displayModelerSilhouettes() const       { return m_pD->displayModelerSilhouettes(); }
  
  /** \details
    Retrieves the recommended maximum deviation of the
    current vectorization for the specified point on a curve.

    \param t [in]  Deviation type.
    \param p [in]  Point on a curve.
        
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
  virtual double deviation(const OdGiDeviationType t, const OdGePoint3d& p) const { return m_pD->deviation(t, p); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiWrapperViewportDraw : public OdGiWrapperCommonDraw< T >
{
public:
  /** \details
    Retrieves a reference to a viewport.
  
    \returns
    reference to a viewport.
  */
  virtual OdGiViewport& viewport() const { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->viewport(); }
  
  /** \details
    Retrieves a viewport geometry associated with this object.
  
    \returns
    reference to a viewport geometry as OdGiViewportGeometry instance.
  */
  virtual OdGiViewportGeometry& geometry() const  { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->geometry(); }
  
  /** \details
      Retrieves sequence number. The default implementation of this function always returns 0.
      
      \returns
      sequence number.
  */
  virtual OdUInt32 sequenceNumber() const  { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->sequenceNumber(); }
  
  /** \details
      Checks whether the specified identifier of a viewport is valid.
      
      \param viewportId [in]  Identifier of a viewport to check.
      
      \returns
      true if the specified viewport is valid, false otherwise.
  */
  virtual bool isValidId(const OdUInt32 viewportId) const { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->isValidId(viewportId); }
  
  /** \details
      Retrieves identifier of a viewport.
  
  \returns
  pointer to an OdDbStub instance that represents a viewport identifier.
  */
  virtual OdDbStub* viewportObjectId() const { return ((OdGiViewportDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->viewportObjectId(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiWrapperWorldDraw : public OdGiWrapperCommonDraw< T >
{
public:
  /** \details
    Retrieves a geometry that can vectorize itself.
  
    \returns
    reference to a geometry as OdGiWorldGeometry instance.
  */
  virtual OdGiWorldGeometry& geometry() const  { return ((OdGiWorldDraw*)(OdGiWrapperCommonDraw< T >::getDraw()))->geometry(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiViewportDraw_Stub : public OdGiWrapperViewportDraw<OdGiViewportDraw>
                            , public OdGiWrapperViewportGeometry<OdGiViewportGeometry>
                            , public OdGiSubEntityTraitsWrapper<OdGiSubEntityTraits>
{
public:
  ODRX_USING_HEAP_OPERATORS( OdGiWrapperViewportDraw<OdGiViewportDraw> );

  /** \details
    Constructor for the OdGiViewportDraw_Stub class.
    
    \param pVD [in]  OdGiViewportDraw object for the wrapper to wrap.
    \param pVG [in]  OdGiViewportGeometry object for the wrapper to wrap.
    \param pST [in]  Sub-entity traits.
  */
  OdGiViewportDraw_Stub(OdGiViewportDraw &pVD, OdGiViewportGeometry &pVG, OdGiSubEntityTraits &pST)
  {
    setDraw(pVD);
    setGeom(pVG);
    setTraits(pST);
  }

  /** \details
    Retrieves raw geometry associated with this object.
    
    \returns
    raw geometry.
    \remarks
    The method cannot return NULL.
  */
  virtual OdGiGeometry& rawGeometry() const   { return *static_cast<OdGiGeometry*>(const_cast<OdGiViewportDraw_Stub*>(this)); }
  
  /** \details
    Retrieves sub-entity traits associated with this object.
    
    \returns
    sub-entity traits. 
    \remarks
    The subEntityTraits object gives control of, and access to the attribute (color, layer, linetype, etc.) settings of the current geometry. For more information, see the <link OdGiSubEntityTraits, OdGiSubEntityTraits> documentation. 
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const { return *static_cast<OdGiSubEntityTraits*>(const_cast<OdGiViewportDraw_Stub*>(this)); }
  
  /** \details
    Retrieves viewport geometry associated with this object.
    
    \returns
    viewport geometry as OdGiViewportGeometry instance.
  */
  virtual OdGiViewportGeometry& geometry() const  { return *static_cast<OdGiViewportGeometry*>(const_cast<OdGiViewportDraw_Stub*>(this)); }

  /** \details
    Adds reference to this object. Currently not implemented.
  */
  void addRef() { }
  
  /** \details
    Releases reference to this object. Currently not implemented.
  */
  void release() { }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiWorldDraw_Stub : public OdGiWrapperWorldDraw<OdGiWorldDraw>
                         , public OdGiWrapperWorldGeometry<OdGiWorldGeometry>
                         , public OdGiSubEntityTraitsWrapper<OdGiSubEntityTraits>
{
public:
  ODRX_USING_HEAP_OPERATORS( OdGiWrapperWorldDraw<OdGiWorldDraw> );

  /** \details
    Constructor for the OdGiWorldDraw_Stub class.
    
    \param pVD [in]  OdGiWorldDraw object for the wrapper to wrap.
    \param pVG [in]  OdGiWorldGeometry object for the wrapper to wrap.
    \param pST [in]  Sub-entity traits.
  */
  OdGiWorldDraw_Stub(OdGiWorldDraw &pVD, OdGiWorldGeometry &pVG, OdGiSubEntityTraits &pST)
  {
    setDraw(pVD);
    setGeom(pVG);
    setTraits(pST);
  }

  /** \details
    Retrieves raw geometry associated with this object.
    
    \returns
    raw geometry.
    \remarks
    The method cannot return NULL.
  */
  virtual OdGiGeometry& rawGeometry() const   { return *static_cast<OdGiGeometry*>(const_cast<OdGiWorldDraw_Stub*>(this)); }
  
  /** \details
    Retrieves sub-entity traits associated with this object.
    
    \returns
    sub-entity traits. 
    \remarks
    The subEntityTraits object gives control of, and access to the attribute (color, layer, linetype, etc.) settings of the current geometry. For more information, see the <link OdGiSubEntityTraits, OdGiSubEntityTraits> documentation. 
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const { return *static_cast<OdGiSubEntityTraits*>(const_cast<OdGiWorldDraw_Stub*>(this)); }
  
  /** \details
    Retrieves world geometry associated with this object.
    
    \returns
    viewport geometry as OdGiWorldGeometry instance.
  */
  virtual OdGiWorldGeometry& geometry() const  { return *static_cast<OdGiWorldGeometry*>(const_cast<OdGiWorldDraw_Stub*>(this)); }

  /** \details
    Adds reference to this object. Currently not implemented.
  */
  void addRef() { }
  
  /** \details
    Releases reference to this object. Currently not implemented.
  */
  void release() { }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyGeometry : public T
{
public:
  /** \details
    Introduces a circle to the vectorization context. Does nothing in current implementation.

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal.
    \remarks
    The circle is not filled and takes on the current color.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  void circle(const OdGePoint3d& center, double radius, const OdGeVector3d& normal) {}

  /** \details
    Introduces a circle to the vectorization context. Does nothing in current implementation.

    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \param thirdPoint [in]  Third point.   
    \remarks
    The circle is not filled and takes on the current color. Points must not be co-linear.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  void circle(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint) {}

  /** \details
    Introduces a circular arc to the vectorization context. Does nothing in current implementation.
    
    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param startVector [in]  Start of the arc.
    \param sweepAngle [in]  Angle that defines the arc.
    \param arcType [in]  Arc type.
    
    \remarks
    The arc takes on the current color.      
    If firstPoint, secondPoint, and thirdPoint are specified, they
    cannot be colinear and no two can be coincident. All angles are expressed in radians. 
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
    OdGiArcType arcType = kOdGiArcSimple) {}

  /** \details
    Introduces a circular arc to the vectorization context. Does nothing in current implementation.
    
    \param firstPoint [in]  First point of a 3-point arc.
    \param secondPoint [in]  Second point of a 3-point arc.
    \param thirdPoint [in]  Third point of a 3-point arc.
    \param arcType [in]  Arc type.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  void circularArc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple) {}

  /** \details
    Introduces a polyline to the vectorization context. Does nothing in current implementation.

    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param baseSubEntMarker [in] Sub-entity marker. Not used.
    \remarks
    The polyline is unfilled and takes on the current color and thickness. Use polygon() to render filled areas. 
    The polyline is rendered as a series of lines connecting the first point
    in vertexList to the second, the second to the third, etc.
    All points must be coplanar. Use polygon() to render closed areas.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void polyline(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
     OdGsMarker baseSubEntMarker = -1) {}

  /** \details
    Introduces a polygon to this vectorization context. Does nothing in current implementation.

    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Pointer to an array of vertices.
    \remarks
    The polygon is rendered as a series of lines connecting the first point
    in vertexList to the second, the second to the third, etc.
    All points must be coplanar.
    \sa
    <link tv_geometry_polygons, Work with Polygons>
  */
  void polygon(OdInt32 numVertices, const OdGePoint3d* vertexList) {}
  
  /** \details
  Introduces a polygon (with a normal) to the vectorization context. Does nothing in current implementation.

  \param numVertices [in]  Number of vertices.
  \param vertexList [in]  Pointer to an array of vertices.
  \param pNormal [in]  Pointer to the normal vector.
  \remarks
  The polygon is rendered as a series of lines connecting the first point
  in vertexList to the second, the second to the third, etc.
  All points must be coplanar.
  \sa
  <link tv_geometry_polygons, Work with Polygons>
  */
  void polygon(OdInt32 numVertices, const OdGePoint3d* vertexList, const OdGeVector3d* pNormal) {}

  /** \details
    Introduces a lightweight polyline into this vectorization context. Does nothing in current implementation.

    \param polylPine [in]  Polyline.
    \param fromIndex [in]  Index of the first segment to be processed.
    \param numSegs [in]  Number of segments to be processed (0 indicates all segments).
    \remarks
    The polyline may contain varying segment widths, straight segments
    and arc segments.    
    The polyline takes on the current color.  
    All points must be coplanar.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void pline(const OdGiPolyline& polylPine, OdUInt32 fromIndex = 0, OdUInt32 numSegs = 0) {}

  /** \details
    Introduces a shape into this vectorization context. Does nothing in current implementation.
    
    \param position [in]  Position of the shape.
    \param normal [in]  Normal vector.
    \param direction [in]  Baseline direction for the shape.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
  */
  void shape(const OdGePoint3d& position,
    const OdGeVector3d& normal, const OdGeVector3d& direction,
    int shapeNumber, const OdGiTextStyle* pTextStyle) {}

  /** \details
    Introduces a text into this vectorization context. Does nothing in current implementation.
    
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
    double height, double width, double oblique, const OdString& msg) {}

  /** \details
    Introduces a text into this vectorization context. Does nothing in current implementation.
    
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
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle) {}

  /** \details
    Introduces an xline into this vectorization context. Does nothing in current implementation.
    
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
  */
  void xline(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint) {}

  /** \details
    Introduces a ray into this vectorization context. Does nothing in current implementation.
    
    \param basePoint [in]  Base point.
    \param throughPoint [in]  Through point.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void ray(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint ) {}

  /** \details
    Introduces nurbs into this vectorization context. Does nothing in current implementation.
    
    \param nurbsCurve [in]  NURBS curve data.
    \sa
    <link tv_geometry_nurbs, Work with NURBS>
  */
  void nurbs(const OdGeNurbCurve3d& nurbsCurve) {}

  /** \details
    Introduces an elliptical arc into this vectorization context. Does nothing in current implementation.

    \param ellipArc [in]  Elliptical arc.
    \param endPointsOverrides [in]  Array of points to be used as the first and last points of the vectorized arc.
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
    OdGiArcType arcType = kOdGiArcSimple) {}

  /** \details
    Introduces a mesh into this vectorization context. Does nothing in current implementation.

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
    const OdGiVertexData* pVertexData = 0) {}

  /** \details
    Introduces a shell into this vectorization context. Does nothing in current implementation.

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
    const OdGiVertexData* pVertexData = 0) {}

  /** \details
    Introduces a world line into this vectorization context. Does nothing in current implementation.

    \param points [in]  Points of the world line. 
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void worldLine(const OdGePoint3d points[2]) {}

  /** \details
    Introduces a raster image into this vectorization context. Does nothing in current implementation.

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
    OdGiRasterImage::TransparencyMode trpMode = OdGiRasterImage::kTransparency8Bit) {}

  /** \details
    Introduces a hatch boundary into this vectorization context. Does nothing in current implementation.

    \param edges [in]  Array of 2d points that define a boundary.
  */
  virtual void edge(const OdGiEdge2dArray& edges) {}

  /** \details
    Introduces an array of points into this vectorization context. Does nothing in current implementation.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pColors [in]  Pointer to point colors.
    \param pTransparency [in]  Pointer to point transparency.
    \param pNormals [in]  Array of normal vectors.
    \param pSubEntMarkers [in]  Array of sub-entity markers.
    \param nPointSize [in]  Points size.
  */
  void polypoint(OdInt32 numPoints, const OdGePoint3d* vertexList, const OdCmEntityColor* pColors, const OdCmTransparency* pTransparency,
                 const OdGeVector3d* pNormals, const OdGsMarker* pSubEntMarkers, OdInt32 nPointSize) {}

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
                           const OdCmEntityColor *pFillColors = NULL, const OdCmTransparency *pFillTransparencies = NULL) {}

  /** \details
    Introduces a row of points into this vectorization context. Does nothing in current implementation.

    \param numPoints [in]  Number of points.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset to next point.
  */
  void rowOfDots(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint) {}

  /** \details
    Introduces a Point Clound object into this vectorization context.

    \param pCloud [in]  Point Cloud object.
  */
  void pointCloud(const OdGiPointCloud &pCloud) {}

  /** \details
    Sets geometrical extents for this vectorizer. Does nothing in current implementation.
    
    \param newExtents [in]  New geometrical extents to set.
  */
  void setExtents(const OdGePoint3d *newExtents) {}

  /** \details
    Pushes a clipping boundary onto the stack, which can possibly affect the display of subsequent primitives.
    Use the popClipBoundary() to remove this clipping boundary from the stack.

    \param pBoundary [in]  Pointer to a clipping boundary.
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
  */
  void pushClipBoundary(OdGiClipBoundary* pBoundary) {}
  
  /** \details
    Pops a top clipping boundary from the stack. This method should be used after a successful pushClipBoundary() method call
    from the worldDraw or viewportDraw of a drawable object.
    \sa
    <link gi_clipping_engine, Work with the Clipping Engine>
  */
  void popClipBoundary() {}
  
  /** \details
    Draws the specified drawable.
    
    \param pDrawable [in]  Pointer to the drawable to draw.
  */
  void draw(const OdGiDrawable* pDrawable) {}

  /** \details
    Pushes a transformation matrix onto the stack. The resulting transformation will be created by multiplying the specified transform matrix by the existing one.
    This allows you to easily adjust an entity coordinate system. Use the popModelTransform() to remove this transform matrix from the stack when you are finished with this transformation.

    \param xMat [in]  Transformation matrix.
  */
  void pushModelTransform(const OdGeMatrix3d& xMat) { }

  /** \details
    Pushes a transformation vector onto the stack. The resulting transformation will be created by using the arbitrary axis algorithm and the specified vector.
    This allows you to easily adjust an entity coordinate system. Use the popModelTransform() to remove this transform vector from the stack when you are finished with this transformation.

    \param normal [in]  Transformation vector.
  */
  void pushModelTransform(const OdGeVector3d& normal) { }

  /** \details
    Pops a transformation from the stack. This method should be used after a successful pushModelTransform() method call to restore previous model transform state.
  */
  void popModelTransform() { }

  /** \details
    Retrieves the block insert transform, which is the model-to-world coordinate transform.
    
    \returns
    identity matrix in current implementation.
  */
  OdGeMatrix3d getModelToWorldTransform() const { return OdGeMatrix3d::kIdentity; }

  /** \details
    Retrieves model-to-world coordinate transform which is inverse of the retrieved matrix from the getModelToWorldTransform() method. 
    The retrieved matrix can transform an entity's world coordinate representation back into the original model coordinates.
    
    \returns
    identity matrix in current implementation.
  */
  OdGeMatrix3d getWorldToModelTransform() const { return OdGeMatrix3d::kIdentity; }

  /** \details
    Introduces a BREP into this vectorization context.

    \returns
    false in current implementation.
  */
  bool brep(const OdGiBrep&) { return false; }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyWorldGeometry : public OdGiDummyGeometry< T >
{
public:
  /** \details
    Sets geometrical extents for this vectorizer. Currently not implemented.
    
    \param newExtents [in]  New extents to set.
  */
  virtual void setExtents(
    const OdGePoint3d * newExtents) { }

  /** \details
    Sets the OdGiBaseVectorizer::kAttributesSegment flag for the associated vectorizer to true.
  */
  virtual void startAttributesSegment() {}
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyViewportGeometry : public OdGiDummyGeometry< T >
{
public:
  
  /** \details
    Introduces a new polyline but in eye coordinate system.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  virtual void polylineEye(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) { }

  /** \details
    Introduces a new polygon but in eye coordinate system. Does nothing in current implementation.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  virtual void polygonEye(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) {  }

  /** \details
    Introduces a new polyline but in device coordinate system. Does nothing in current implementation.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  virtual void polylineDc(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) {  }

  /** \details
    Introduces a new polygon but in device coordinate system. Does nothing in current implementation.
  
    \param numVertices       [in]  Number of vertices.
    \param vertexList        [in]  Pointer to an array of vertices.
  */
  virtual void polygonDc(
    OdUInt32 numVertices, 
    const OdGePoint3d* vertexList) {  }

  /** \details
    Introduces a raster image into the vectorization context in device coordinate system. Does nothing in current implementation.
  
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
  */
  virtual void rasterImageDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage, 
    const OdGePoint2d* uvBoundary, 
    OdUInt32 numBoundPts,
    bool transparency = false,
    double brightness = 50.0,
    double contrast = 50.0,
    double fade = 0.0) {  }

  /** \details
    Introduces a metafile into the vectorization context in device coordinate system. Does nothing in current implementation.
  
    \param origin         [in]  Origin of a metafile content.
    \param u              [in]  Metafile content u-parameter.
    \param v              [in]  Metafile content v-parameter.
    \param pMetafile      [in]  Pointer to a metafile.
    \param bDcAligned     [in]  Flag that specifies whether metafile content should be aligned to device coordinate system.
    \param bAllowClipping [in]  Flag that specifies whether clipping should be allowed for a metafile.
  */
  virtual void  metafileDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool bDcAligned = true,
    bool bAllowClipping = false) {  }

  /** \details
    Passes to the visualization framework an object that can render itself. Does nothing in current implementation.

    \param origin [in]  Object origin.
    \param u [in]  Object width vector.
    \param v [in]  Object height vector.
    \param pDrawable [in]  Pointer to the drawable.
    \param dcAligned     [in]  Flag that specifies whether drawable should be aligned to device coordinate system.
    \param allowClipping [in]  Flag that specifies whether clipping should be allowed.
    \remarks
    ODA Platform uses this function to render Ole2Frame entities. 
    
    \remarks
    ownerDrawDc is not conveyor primitive, so the ODA Platform visualization framework performs no processing 
    (transformation, linetyping, or clipping) for this object. 

    All coordinates are in Model coordinates.
  */
  virtual void ownerDrawDc(
    const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiSelfGdiDrawable* pDrawable,
    bool dcAligned = true,
    bool allowClipping = false) {  }
};

// OdGiSubEntityTraits dummy template

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummySubEntityTraits : public T
{
public:
  /** \details
    Sets the current color index (ACI). Does nothing in current implementation.

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
  void setColor(OdUInt16 color) { }
  
  /** \details
    Sets the current color from an OdCmEntityColor instance. Does nothing in current implementation.
    
    \param color [in]  OdCmEntityColor object.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  void setTrueColor(const OdCmEntityColor& color) { }
  
  /** \details
    Sets the current layer. Does nothing in current implementation.
    \param layerId [in]  Object ID of the LayerTableRecord.  
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  void setLayer(OdDbStub* layerId) { }
  
  /** \details
    Sets the current linetype. Does nothing in current implementation.
    \param lineTypeId [in]  Object ID of the LinetypeTableRecord.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  void setLineType(OdDbStub* lineTypeId) { }
  
  /** \details
    Sets the current selection marker. Does nothing in current implementation.
    \param selectionMarker [in]  Selection marker.
  */
  void setSelectionMarker(OdGsMarker selectionMarker) { }
  
  /** \details
    Sets the current fill type. Does nothing in current implementation.
    \param fillType [in]  Fill type.
    
    \remarks
    fillType must be one of the following:
    
    <table>
    Name              Value   Description
    kOdGiFillAlways   1       Fill closed graphic primitives.
    kOdGiFillNever    2       Don't fill closed graphic primitives.
    </table>
  */
  void setFillType(OdGiFillType fillType) { }
  
  /** \details
    Sets the current fill plane. Does nothing in current implementation.
    
    \param pNormal [in]  Pointer to the normal to the plane.
  */
  void setFillPlane(const OdGeVector3d* pNormal = 0) {}
  
  /** \details
    Sets the current lineweight. Does nothing in current implementation.

    \param lineWeight [in]  Lineweight.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  void setLineWeight(OdDb::LineWeight lineWeight) { }
  
  /** \details
    Sets the current linetype scale. Does nothing in current implementation.
    \param lineTypeScale [in]  Linetype scale factor.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  void setLineTypeScale(double lineTypeScale = 1.0) { }
  
  /** \details
    Sets the current thickness. Does nothing in current implementation.
    \param thickness [in]  Thickness.
  */
  void setThickness(double thickness) { }
  
  /** \details
    Sets the current PlotStyleName. Does nothing in current implementation.

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
  void setPlotStyleName(OdDb::PlotStyleNameType plotStyleNameType, OdDbStub* pPlotStyleNameId = 0) { }
  
  /** \details
    Sets the current material. Does nothing in current implementation.
    \param materialId [in]  Object ID of the material.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  void setMaterial(OdDbStub* materialId) { }
  
  /** \details
    Sets the current material mapper. Does nothing in current implementation.
    \param pMapper [in]  Pointer to the GI mapper object.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  void setMapper(const OdGiMapper * pMapper) { }
  
  /** \details
    Sets the current visual style. Does nothing in current implementation.
    \param visualStyleId [in]  Object ID of the visual style.
    \sa
    <link tv_working_with_attributes_visual_styles, Work with Visual Styles>
  */
  void setVisualStyle(const OdDbStub* visualStyleId) { }
  
  /** \details
    Sets the current transparency. Does nothing in current implementation.
    
    \param transparency [in]  OdCmTransparency object.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  void setTransparency(const OdCmTransparency& transparency) { }
  
  /** \details
    Sets the drawing flags. Does nothing in current implementation.
    
    \param drawFlags [in]  Input bitmask with DrawFlags.
  */
  void setDrawFlags(OdUInt32 drawFlags) { }
  
  /** \details
    Sets the lock flags. Does nothing in current implementation.
    
    \param lockFlags [in]  Input bitmask with LockFlags.
  */
  void setLockFlags(OdUInt32 lockFlags) { }
  
  /** \details
    Sets selection geometry flag. Does nothing in current implementation.
    
    \param bSelectionFlag [in]  New selection geometry flag.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  void setSelectionGeom(bool bSelectionFlag) { }
  
  /** \details
    Sets the shadow flags. Does nothing in current implementation.
    
    \param shadowFlags [in]  Input ShadowFlags.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  void setShadowFlags(OdGiSubEntityTraits::ShadowFlags shadowFlags) { }
  
  /** \details
    Sets sectionable flag. Does nothing in current implementation.
    
    \param bSectionableFlag [in]  New sectionable flag.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  void setSectionable(bool bSectionableFlag) { }
  
  /** \details
    Sets the selection flags. Does nothing in current implementation.
    
    \param selectionFlags [in]  Input SelectionFlags.
  */
  void setSelectionFlags(OdGiSubEntityTraits::SelectionFlags selectionFlags) { }

  /** \details
    Retrieves color index (ACI).

    \returns
    0 in current implementation.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  OdUInt16 color() const { return 0; }
  
  /** \details
    Retrieves entity color.

    \returns
    a new instance of the OdCmEntityColor.
    \sa
    <exref target="https://docs.opendesign.com/td/cm.html">Working with Colors</exref>
  */
  OdCmEntityColor trueColor() const { return OdCmEntityColor(); }
  
  /** \details
    Retrieves the current layer.
      
    \returns
    null pointer in current implementation.
    \sa
    <link tv_working_with_attributes_layers, Work with Layers>
  */
  OdDbStub* layer() const { return NULL; }
  
  /** \details
    Retrieves the current linetype.
      
    \returns
    null pointer in current implementation. 
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype.html">Working with Linetypes</exref>
  */
  OdDbStub* lineType() const { return NULL; }
  
  /** \details
    Retrieves the current linetype.
      
    \returns
    the kOdGiFillNever value in current implementation. 
  */
  OdGiFillType fillType() const { return kOdGiFillNever; }
  
  /** \details
    Retrieves the current fill plane. Currently not implemented.
    
    \param normal [in]  Normal to a plane.
    
    \returns
    false in current implementation.
  */
  bool fillPlane(OdGeVector3d& normal) { return false; }
  
  /** \details
    Retrieves the current lineweight.
    
    \returns
    OdDb::kLnWt000 value in current implementation.
    \sa
    <link tv_working_with_attributes_lineweights, Work with Lineweights>
  */
  OdDb::LineWeight lineWeight() const { return OdDb::kLnWt000; }
  
  /** \details
    Retrieves the current linetype scale.
    
    \returns
    0.0 in current implementation.
    \sa
    <exref target="https://docs.opendesign.com/td/db_linetype_properties.html">Specific Properties of Linetypes</exref>
  */
  double lineTypeScale() const { return 0.0; }
  
  /** \details
    Retrieves the current thickness.
    
    \returns
    0.0 in current implementation.
  */
  double thickness() const { return 0.0; }
  
  /** \details
    Retrieves the current PlotStyleName type.
    
    \returns
    OdDb::kPlotStyleNameByBlock in current implementation.
  */
  OdDb::PlotStyleNameType plotStyleNameType() const { return OdDb::kPlotStyleNameByBlock; }
  
  /** \details
    Retrieves object ID of the the plot style name.
    
    \returns
    null pointer in current implementation. 
  */
  OdDbStub* plotStyleNameId() const { return NULL; }
  
  /** \details
    Retrieves the identifier of the material.
    
    \returns
    null pointer in current implementation.
    \sa
    <link tv_working_with_attributes_materials, Work with Materials>
  */
  OdDbStub* material() const { return NULL; }
  
  /** \details
    Retrieves pointer to the current material mapper.
    
    \returns
    null pointer in current implementation.
    \sa
    <link tv_working_with_attributes_materials_mapping, Work with Mapping Properties>
  */
  const OdGiMapper* mapper() const { return NULL; }
  
  /** \details
    Adds the specified light. Does nothing in the current implementation.
    \sa
    <link tv_working_with_lights, Work with Lights>
  */
  void addLight(const OdDbStub* /*lightId*/) { }
  
  /** \details
    Retrieves the current transparency.
    
    \returns
    a new instance of the OdCmTransparency in current implementation.
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_alpha.html">Transparency Functionality as an Alpha-value</exref>
    \sa
    <exref target="https://docs.opendesign.com/td/cm_transparency_integer.html">Transparency Functionality as an Integer-value</exref>
  */
  OdCmTransparency transparency() const { return OdCmTransparency(); }
  
  /** \details
    Retrieves the current DrawFlags bitmask.
    
    \returns
    0 in current implementation.
  */
  OdUInt32 drawFlags() const { return 0; }
  
  /** \details
    Retrieves the current LockFlags bitmask.
    
    \returns
    0 in current implementation.
  */
  OdUInt32 lockFlags() const { return 0; }
  
  /** \details
    Retrieves the current selection geometry flag.
    
    \returns
    false in current implementation.
    \sa
    <link tv_working_with_features_selections, Work with Selections>
  */
  bool selectionGeom() const { return false; }
  
  /** \details
    Retrieves the current ShadowFlags.
    
    \returns
    the OdGiSubEntityTraits::kShadowsCastAndReceive value in current implementation.
    \sa
    <link gi_shadows_rendering, Shadows Visualization>
  */
  OdGiSubEntityTraits::ShadowFlags shadowFlags() const { return OdGiSubEntityTraits::kShadowsCastAndReceive; }
  
  /** \details
    Retrieves the current sectionable flag.
    
    \returns
    false in current implementation.
    \sa
    <link tv_planarClip_sample, Planar Clipping Section Generation for a Custom Entity>
  */
  bool sectionable() const { return false; }
  
  /** \details
    Retrieves the current SelectionFlags.
    
    \returns
    the OdGiSubEntityTraits::kNoSelectionFlags value in current implementation.
  */
  OdGiSubEntityTraits::SelectionFlags selectionFlags() const { return OdGiSubEntityTraits::kNoSelectionFlags; }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
\sa
<link tv_working_with_base_classes_view, Work with Visualize Graphical Views>
*/
template <class T> class OdGiDummyViewport : public T
{
public:
  /** \details
    Returns the transformation matrix from ModelSpace to EyeSpace for this object.

    \returns
    identity matrix in current implementation.
  */
  virtual OdGeMatrix3d getModelToEyeTransform() const { return OdGeMatrix3d::kIdentity; }
  
  /** \details
    Returns the transformation matrix from EyeSpace to ModelSpace for this object.

    \returns
    identity matrix in current implementation.
  */
  virtual OdGeMatrix3d getEyeToModelTransform() const { return OdGeMatrix3d::kIdentity; }
  
  /** \details
    Returns the transformation matrix from WCS to EyeSpace for this object.

    \returns
    identity matrix in current implementation.
  */
  virtual OdGeMatrix3d getWorldToEyeTransform() const { return OdGeMatrix3d::kIdentity; }
  
  /** \details
    Returns the transformation matrix from EyeSpace to WCS for this Viewport object.

    \returns
    identity matrix in current implementation.
  */
  virtual OdGeMatrix3d getEyeToWorldTransform() const { return OdGeMatrix3d::kIdentity; }
  
  /** \details
    Checks whether perspective mode is on for this Viewport.
    
    \returns
    false in current implementation.
  */
  virtual bool isPerspective() const { return false; }
  
  /** \details
    Applies the current perspective transformation to the specified point.

    \param normal [in]  Point relative to which the transformation is applied.

    \remarks
    The point is transformed from EyeSpace coordinates to normalized device coordinates.

    \returns
    false in current implementation.
  */
  virtual bool doPerspective(OdGePoint3d& p) const { return false; }
  
  /** \details
    Applies the inverse of the current perspective transformation to the specified point.
    
    \param normal [in]  Point relative to which the inverse transformation is applied.
    
    \remarks
    The point is transformed from normalized device coordinates to EyeSpace coordinates

    false in current implementation.
  */
  virtual bool doInversePerspective(OdGePoint3d& p) const  { return false; }
  
  /** \details
    Retrieves the display pixel density at the specified point for this Viewport object. Does nothing in current implementation.

    \param givenWorldpt [in]  WCS center of the unit square.
    \param pixelArea [out]  Receives the pixel density.
    \param includePerspective [in]  Flag that specifies whether perspective should be included into pixel density calculation.
    
    \remarks
    Pixel density is measured in pixels per WCS unit.
  */
  virtual void getNumPixelsInUnitSquare(const OdGePoint3d& givenWorldpt, OdGePoint2d& pixelArea, bool includePerspective = true) const {  }
  
  /** \details
    Retrieves the WCS camera (eye) location for this Viewport object.
    
    \returns
    OdGePoint3d::kOrigin in current implementation.
  */
  virtual OdGePoint3d getCameraLocation() const { return OdGePoint3d::kOrigin; }
  
  /** \details
    Retrieves the WCS camera target for this Viewport object.
    
    \returns
    OdGePoint3d::kOrigin + OdGeVector3d::kZAxis in current implementation.
  */
  virtual OdGePoint3d getCameraTarget() const { return OdGePoint3d::kOrigin + OdGeVector3d::kZAxis; }
  
  /** \details
    Retrieves the WCS camera "up" vector for this Viewport object.
    
    \returns
    OdGeVector3d::kYAxis in current implementation.
  */
  virtual OdGeVector3d getCameraUpVector() const { return OdGeVector3d::kYAxis; }
  
  /** \details
    Retrieves the vector from the camera target to the camera location.
    
    \returns
    view direction on a camera. The returned value depend on the getCameraTarget() and getCameraLocation() methods that are not currently implemented.
  */
  virtual OdGeVector3d viewDir() const { return getCameraTarget() - getCameraLocation();  }
  
  /** \details
    Retrieves the OdGi Viewport ID for this Viewport object.

    \remarks
    0xFFFFFFFF value in current implementation.
  */
  virtual OdUInt32 viewportId() const { return 0xFFFFFFFF; }
  
  /** \details
    Retrieves the ID for this Viewport object.

    \remarks
    -1 in current implementation.
  */
  virtual OdInt16  acadWindowId() const { return -1; }
  
  /** \details
    Retrieves lower-left and upper-right corners of this Viewport object in 
    normalized device Coordinates.

    \remarks
    Sets lower-left and upper-right corners to 0.0 and 100.0 values in current implementation.
  */
  virtual void getViewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const { lower_left.set(0,0); upper_right.set(100,100); }
  
  /** \details
    Checks whether front and back clip values are used and if so, retrieves the front and back clipping parameters for this Viewport object.

    \param clip_front [out]  Receives true if and only if front clipping is enabled.
    \param clip_back [out]   Receives true if and only if back clipping is enabled.

    \returns
    false in current implementation.
  */
  virtual bool getFrontAndBackClipValues(bool& clip_front, bool& clip_back, double& /*front*/, double& /*back*/) const { clip_front = clip_back = false; return false; }
  
  /** \details
    Retrieves a multiplier that is used to scale all linetypes in this Viewport object.
    
    \returns
    1.0 in current implementation.
  */
  virtual double linetypeScaleMultiplier() const { return 1.0; }
  
  /** \details
    Retrieves the WCS value below which linetype generation is not used.

    \returns
    1.0 in current implementation.
  */
  virtual double linetypeGenerationCriteria() const { return 1.0; }
  
  /** \details
    Returns true if and only if the specified layer is not frozen either globally or in this Viewport object.

    \param layerId [in]  Object ID of OdLayerTableRecord.
    
    \returns
    true in current implementation.
  */
  virtual bool layerVisible(OdDbStub* idLayer) const { return true; }
};

/** \details
This class is a dummy template for OdGiViewport.
Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiWrapperViewport : public T
{
  const OdGiViewport* m_ref;
protected:
  void set(const OdGiViewport& ref) { m_ref = &ref; }
  const OdGiViewport &get() const { return *m_ref; }
public:
  
  /** \details
    Returns the transformation matrix from ModelSpace to EyeSpace for this object.

    \returns
    ModelSpace to EyeSpace transform matrix.
  */
  virtual OdGeMatrix3d getModelToEyeTransform() const { return get().getModelToEyeTransform(); }
  
  /** \details
    Returns the transformation matrix from EyeSpace to ModelSpace for this object.

    \returns
    EyeSpace to ModelSpace transform matrix.
  */
  virtual OdGeMatrix3d getEyeToModelTransform() const { return get().getEyeToModelTransform(); }
  
  /** \details
    Returns the transformation matrix from WCS to EyeSpace for this object.

    \returns
    WorldSpace to EyeSpace transform matrix.
  */
  virtual OdGeMatrix3d getWorldToEyeTransform() const { return get().getWorldToEyeTransform(); }
  
  /** \details
    Returns the transformation matrix from EyeSpace to WCS for this object.

    \returns
    EyeSpace to WorldSpace transform matrix.
  */
  virtual OdGeMatrix3d getEyeToWorldTransform() const { return get().getEyeToWorldTransform(); }
  
  /** \details
    Checks whether perspective mode is on for this object.
    
    \returns
    true if the perspective mode is on, false otherwise.
  */
  virtual bool isPerspective() const { return get().isPerspective(); }
  
  /** \details
    Applies the current perspective transformation to the specified point.

    \param p [in/out]  Point relative to which the transformation is applied. Receives point coordinates after transformation if successful.

    \remarks
    The point is transformed from EyeSpace coordinates to normalized device coordinates.

    \returns
    true if the point was transformed; i.e., a perspective transform
    is active, and the point was neither too close or behind the camera.
  */
  virtual bool doPerspective(OdGePoint3d& p) const { return get().doPerspective(p); }
  
  /** \details
    Applies the inverse of the current perspective transformation to the specified point.

    \param point [in/out] Point relative to which the inverse transformation is applied. Receives point coordinates after transformation if successful.

    \remarks
    The point is transformed from normalized device coordinates to EyeSpace coordinates

    true if the point was transformed; i.e., an inverse perspective transform
    is active, and the point was neither too close or behind the camera.

    \sa
    Coordinate Systems
  */
  virtual bool doInversePerspective(OdGePoint3d& p) const  { return get().doInversePerspective(p); }
  
  /** \details
    Returns the display pixel density at the specified point for this object.

    \param givenWorldpt [in]  WCS center of the unit square.
    \param pixelArea [out]  Receives the pixel density.
    \param includePerspective [in]  Enable perspective inclusion into pixel density calculation.

    \remarks
    Pixel density is measured in pixels per WCS unit.

    This function can be used to determine if the geometry generated for an object will 
    be smaller than the size of a pixel.
  */
  virtual void getNumPixelsInUnitSquare(const OdGePoint3d& givenWorldpt,
    OdGePoint2d& pixelArea, bool includePerspective = true) const
  {
    get().getNumPixelsInUnitSquare(givenWorldpt, pixelArea, includePerspective);
  }
  
  /** \details
    Returns the WCS camera (eye) location for the associated camera.
    
    \returns
    camera location as OdGePoint3d value.
  */
  virtual OdGePoint3d getCameraLocation() const { return get().getCameraLocation(); }
  
  /** \details
    Returns the camera target point for the associated camera.
    
    \returns
    camera target point as OdGePoint3d value.
  */
  virtual OdGePoint3d getCameraTarget() const { return get().getCameraTarget(); }
  
  /** \details
    Returns the WCS camera "up" vector for for the associated camera.
    
    \returns
    camera up-vector as OdGeVector3d value.
  */
  virtual OdGeVector3d getCameraUpVector() const { return get().getCameraUpVector(); }
  
  /** \details
    Returns the vector from the camera target to the camera location.
    
    \returns
    camera view direction as OdGeVector3d value.
  */
  virtual OdGeVector3d viewDir() const { return get().viewDir();  }
  
  /** \details
    Returns the OdGi Viewport ID for this object.

    \returns
    Id of an associated viewport as OdUInt32 value.
    
    \remarks
    The value returned has no relationshp to the CVPORT system variable.
  */
  virtual OdUInt32 viewportId() const { return get().viewportId(); }
  
  /** \details
    Returns the ID for the associated window object.

    \returns
    Id of an associated window as OdInt16 value.
    
    \remarks
    The value returned corresponds to the CVPORT system variable.
  */
  virtual OdInt16  acadWindowId() const { return get().acadWindowId(); }
  
  /** \details
    Returns lower-left and upper-right corners of the associated Viewport object in 
    normalized device Coordinates.

    \param lowerLeft [out]  Receives the lower-left corner.
    \param upperRight [out]  Receives the upper-right.

    \remarks
    This information lets position items that are fixed in size and/or position
    with respect to a Viewport.

    \sa
    Coordinate Systems      
  */
  virtual void getViewportDcCorners(OdGePoint2d& lower_left, OdGePoint2d& upper_right) const
  { get().getViewportDcCorners(lower_left, upper_right); }
  
  /** \details
    Returns the front and back clipping parameters for the associated Viewport object.

    \param clipFront [out]  Receives true if and only if front clipping is enabled.
    \param clipBack [out]   Receives true if and only if back clipping is enabled.
    \param front [out]  Receives the front clipping distance.
    \param back [out]  Receives the back clipping distance.

    \returns
    true if clipFront || clipBack which means that one of the values is non-zero, false otherwise.
    
    \remarks
    front and back are the eye coordinate Z values. 

    Clipping planes are perpendicular to the view direction.

    If the front clipping plane is enabled, geometry in front of it is not displayed.

    If the back clipping plane is enabled, geometry in back of it is not displayed.
  */
  virtual bool getFrontAndBackClipValues(bool& clip_front, bool& clip_back, double& front, double& back) const
  { return get().getFrontAndBackClipValues(clip_front, clip_back, front, back); }
  
  /** \details
    Returns a multiplier that is used to scale all linetypes in the associated Viewport object.
    
    \returns
    multiplier for the linetype scale as value of the double type.
  */
  virtual double linetypeScaleMultiplier() const { return get().linetypeScaleMultiplier(); }
  
  /** \details
    Returns the WCS value below which linetype generation is not used.

    \returns
    linetype generation criteria as a value of the double type.
    
    \remarks
    If the WCS length an entire linetype pattern is less than the returned value, then
    the linetype will be rendered as continuous.
  */
  virtual double linetypeGenerationCriteria() const { return get().linetypeGenerationCriteria(); }
  
  /** \details
    Checks whether the specified layer is not frozen either globally or in the associated Viewport object.

    \param layerId [in]  Identifier of the OdLayerTableRecord to check.
    
    \returns
    true the specified layer is not frozen, false otherwise.
  */
  virtual bool layerVisible(OdDbStub* idLayer) const { return get().layerVisible(idLayer); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T1, class T2> class OdGiJoinCommonDraw : public T1, public T2
{
public:
  ODRX_USING_HEAP_OPERATORS(T1);
  
  /** \details
    Retrieves raw geometry associated with this object.
    
    \returns
    raw geometry.
  */
  virtual OdGiGeometry& rawGeometry() const            { return T1::rawGeometry(); }
  
  /** \details
    Returns the regeneration type for this object.

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
  virtual OdGiRegenType regenType() const              { return T1::regenType(); } // dont change from kOdGiStandardDisplay
  
  /** \details
    Checks whether the vectorization process associated with this object should be aborted.
    
    \returns
    true if this vectorization process should be aborted, false otherwise.
  */
  virtual bool regenAbort() const                      { return T1::regenAbort(); }
  
  /** \details
    Provides access to this object's sub-entity traits.

    \returns
    sub-entity traits of this object.
    
    \remarks
    This allows the modification of the vectorization attributes such as color, linetype, etc.
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const { return T1::subEntityTraits(); }
  
  /** \details
    Checks whether the associated vectorization process is the result of a "drag" operation.
    
    \returns
    true if vectorization process is the result of a "drag" operation, false otherwise.
  */
  virtual bool isDragging() const                      { return T1::isDragging(); }
  
  /** \details
    Retrieves context associated with this object.
    
    \returns
    Pointer to the associated context.
  */
  virtual OdGiContext* context() const                 { return T1::context(); }
  
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
  virtual OdUInt32 numberOfIsolines() const            { return T1::numberOfIsolines(); }
  
  /** \details
    Retrieves the current drawable nesting graph.
    
    \returns
    pointer to a current drawable nesting graph.
  */
  virtual const OdGiPathNode* currentGiPath() const    { return T1::currentGiPath(); }
  
  /** \details
    Checks whether generation of silhouettes is enabled for modeler geometry.
    
    \returns
    true if generation of silhouettes is enabled for modeler geometry.
  */
  virtual bool displayModelerSilhouettes() const       { return T1::displayModelerSilhouettes(); }
  
  /** \details
    Retrieves the recommended maximum deviation of the
    current vectorization for the specified point on a curve.

    \param t [in]  Deviation type.
    \param p [in]  Point on a curve.
        
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
  virtual double deviation(const OdGiDeviationType t, const OdGePoint3d& p) const { return T1::deviation(t, p); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template<class T> class OdGiDummyCommonDraw : public T
{
public:
  
  /** \details
    Returns the regeneration type for this object.

    \remarks
    the kOdGiStandardDisplay value in current implementation.
  */
  virtual OdGiRegenType regenType() const              { return kOdGiStandardDisplay; } // dont change from kOdGiStandardDisplay
  
  /** \details
    Checks whether the vectorization process associated with this object should be aborted.
    
    \returns
    false in current implementation.
  */
  virtual bool regenAbort() const                      { return false; }
  
  /** \details
    Checks whether the associated vectorization process is the result of a "drag" operation.
    
    \returns
    false in current implementation.
  */
  virtual bool isDragging() const                      { return false; }
  
  /** \details
    Retrieves count of isolines that are used to represent an entity.
    
    \returns
    4 in current implementation.
    
    \remarks
    Isolines are lines or curves which are used to represent the entity. 
    For example, if an entity is a sphere and this method returns 3, the 
    sphere should be displayed as 3 circles with a common center and planes that are
    perpendicular to each other.
  */
  virtual OdUInt32 numberOfIsolines() const { return 4; }
  
  /** \details
    Retrieves the recommended maximum deviation of the
    current vectorization for the specified point on a curve.

    \param t [in]  Deviation type.
    \param p [in]  Point on a curve.
        
    \returns
    0.0 in current implementation. 

    \remarks
    This method uses circle zoom percent or FacetRes as appropriate.
  */
  virtual double deviation(const OdGiDeviationType t, const OdGePoint3d& p) const { return 0.; }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T, class TVp = OdGiViewport> class OdGiDummyViewportDraw : public OdGiDummyCommonDraw< T >
{
private:
  // This class was moved from multiple inheritance to member, since it is crash VS2010 IntelliSense
  mutable OdGiDummyViewport< OdStaticRxObject<TVp> > m_dummyViewport;
public:
  
  /** \details
    Retrieves a reference to an associated viewport.
  
    \returns
    reference to a dummy viewport.
  */
  virtual OdGiViewport& viewport() const { return m_dummyViewport; }
  
  /** \details
      Retrieves sequence number. The default implementation of this function always returns 0.
      
      \returns
      0 in current implementation.
  */
  virtual OdUInt32 sequenceNumber() const  { return 0; }
  
  /** \details
      Checks whether the specified identifier of a viewport is valid.
      
      \param viewportId [in]  Identifier of a viewport to check.
      
      \returns
      false in current implementation.
  */
  virtual bool isValidId(const OdUInt32 viewportId) const { return false; }
  
  /** \details
      Retrieves identifier of a viewport.
  
  \returns
  0 in current implementation.
  */
  virtual OdDbStub* viewportObjectId() const { return 0; }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyWorldDraw : public OdGiDummyCommonDraw< T >
{
public:
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
template <class T> class OdGiDummyContext : public T
{
public:
  
  /** \details
      Retrieves a pointer to an associated database.
  
  \returns
  0 in current implementation.
  */
  virtual OdDbBaseDatabase* database() const { return 0;}
  
  /** \details
    Retrieves a pointer to an associated database.
      
    \param pDrawable       [in]  Pointer to a drawable object.
  
    \returns
    Pointer to a newly created smart pointer to a drawable in current implementation.
  */
  virtual OdGiDrawablePtr openDrawable(OdDbStub* pDrawable) { return OdGiDrawablePtr(); }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiViewportDraw_Dummy : public OdGiDummySubEntityTraits< OdGiSubEntityTraits >
                             , public OdGiDummyViewportDraw< OdGiViewportDraw >
                             , public OdGiDummyViewportGeometry< OdGiViewportGeometry >
                             , public OdGiDummyContext< OdGiContext >
{
  OdDbBaseDatabase *m_pDb;
public:
  ODRX_USING_HEAP_OPERATORS( OdGiDummySubEntityTraits< OdGiSubEntityTraits > );

  /** \details
    Constructor for the OdGiViewportDraw_Dummy class. Creates an object with the associated database specified by the pDb parameter.
    
    \param pDb [in]  Pointer to a database.
  */
  OdGiViewportDraw_Dummy(OdDbBaseDatabase *pDb = 0) : m_pDb(pDb) { }
  
  /** \details
    Retrieves a pointer to an associated database.
    
    \returns
    pointer to an associated database.
  */
  virtual OdDbBaseDatabase* database() const { return m_pDb; }

  /** \details
    Retrieves raw geometry associated with this object.
    
    \returns
    raw geometry.
  */
  virtual OdGiGeometry& rawGeometry() const   { return *static_cast<OdGiGeometry*>(const_cast<OdGiViewportDraw_Dummy*>(this)); }
  
  /** \details
    Provides access to this object's sub-entity traits.

    \returns
    sub-entity traits of this object.
    
    \remarks
    This allows the modification of the vectorization attributes such as color, linetype, etc.
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const { return *static_cast<OdGiSubEntityTraits*>(const_cast<OdGiViewportDraw_Dummy*>(this)); }
  
  /** \details
    Retrieves a viewport geometry associated with this object.
  
    \returns
    reference to a viewport geometry as OdGiViewportGeometry instance.
  */
  virtual OdGiViewportGeometry& geometry() const  { return *static_cast<OdGiViewportGeometry*>(const_cast<OdGiViewportDraw_Dummy*>(this)); }
  
  /** \details
    Retrieves context associated with this object.
    
    \returns
    Pointer to the associated context.
  */
  virtual OdGiContext* context() const            { return static_cast<OdGiContext*>(const_cast<OdGiViewportDraw_Dummy*>(this)); }

  /** \details
    Adds reference to this object. Currently not implemented.
  */
  void addRef() { }
  
  /** \details
    Releases reference to this object. Currently not implemented.
  */
  void release() { }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
class OdGiWorldDraw_Dummy : public OdGiDummySubEntityTraits< OdGiSubEntityTraits >
  , public OdGiDummyWorldDraw< OdGiWorldDraw >
  , public OdGiDummyWorldGeometry< OdGiWorldGeometry >
  , public OdGiDummyContext< OdGiContext >
{
  OdDbBaseDatabase *m_pDb;
public:
  ODRX_USING_HEAP_OPERATORS( OdGiDummySubEntityTraits< OdGiSubEntityTraits > );

  /** \details
    Constructor for the OdGiWorldDraw_Dummy class. Creates an object with the associated database specified by the pDb parameter.
    
    \param pDb [in]  Pointer to a database.
  */ 
  OdGiWorldDraw_Dummy(OdDbBaseDatabase *pDb = 0) : m_pDb(pDb) { }
  
  /** \details
    Retrieves a pointer to an associated database.
    
    \returns
    pointer to an associated database.
  */
  virtual OdDbBaseDatabase* database() const { return m_pDb; }

  /** \details
    Retrieves raw geometry associated with this object.
    
    \returns
    raw geometry.
  */
  virtual OdGiGeometry& rawGeometry() const   { return *static_cast<OdGiGeometry*>(const_cast<OdGiWorldDraw_Dummy*>(this)); }
  
  /** \details
    Provides access to this object's sub-entity traits.

    \returns
    sub-entity traits of this object.
    
    \remarks
    This allows the modification of the vectorization attributes such as color, linetype, etc.
  */
  virtual OdGiSubEntityTraits& subEntityTraits() const { return *static_cast<OdGiSubEntityTraits*>(const_cast<OdGiWorldDraw_Dummy*>(this)); }
  
  /** \details
    Retrieves a world geometry associated with this object.
  
    \returns
    reference to a world geometry as OdGiWorldGeometry instance.
  */
  virtual OdGiWorldGeometry& geometry() const  { return *static_cast<OdGiWorldGeometry*>(const_cast<OdGiWorldDraw_Dummy*>(this)); }
  
  /** \details
    Retrieves context associated with this object.
    
    \returns
    Pointer to the associated context.
  */
  virtual OdGiContext* context() const            { return static_cast<OdGiContext*>(const_cast<OdGiWorldDraw_Dummy*>(this)); }

  /** \details
    Adds reference to this object. Currently not implemented.
  */
  void addRef() { }
  
  /** \details
    Releases reference to this object. Currently not implemented.
  */
  void release() { }
};

/** \details

Corresponding C++ library: TD_Gi
<group OdGi_Classes>
*/
typedef OdStaticRxObject< OdGiDummySubEntityTraits< OdGiSubEntityTraits > > OdGiSubEntityTraits_Stub;

#include "TD_PackPop.h"

#endif // #ifndef _ODGIDUMMYGEOM_INCLUDED_
