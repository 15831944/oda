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

#ifndef __ODGIEMPTYGEOMETRY_H__
#define __ODGIEMPTYGEOMETRY_H__

#include "Gi/GiExport.h"
#include "Gi/GiConveyorGeometry.h"

#include "TD_PackPush.h"

/** \details
  This class is an implementation of OdGiConveyorGeometry that provides no-ops for the all functions therein.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiEmptyGeometry : public OdGiConveyorGeometry
{
public:
  ODGI_EXPORT_STATIC static OdGiConveyorGeometry& kVoid;

  /** \details
    The default implementation of this function does nothing.
    
    \param xfm [in]  Pointer to a transformation matrix.
    \param fromIndex [in]  Index of the first segment to be processed.
    \param numSegs [in]  Number of segments to be processed.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void plineProc(const OdGiPolyline& polyline,
    const OdGeMatrix3d* xfm = 0,
    OdUInt32 fromIndex = 0,
    OdUInt32 numSegs = 0);

  /** \details
    The default implementation of this function does nothing.
      
    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  void circleProc(
    const OdGePoint3d& center, double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    The default implementation of this function does nothing.
      
    \param firstPoint [in]  First point of a 3-point circle.
    \param secondPoint [in]  Second point of a 3-point circle.
    \param thirdPoint [in]  Third point of a 3-point circle.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  void circleProc(
    const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint, const OdGePoint3d& thirdPoint,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    The default implementation of this function does nothing.
      
    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param startVector [in]  Defines the start of this arc.
    \param sweepAngle [in]  Angle that defines the arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Pointer to the extrusion vector.    
    \remarks
    All angles are expressed in radians.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  void circularArcProc(
    const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    The default implementation of this function does nothing.
      
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
  void circularArcProc(
    const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    The default implementation of this function does nothing.
      
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
  void polylineProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0,
    const OdGeVector3d* pExtrusion = 0, OdGsMarker baseSubEntMarker = -1);
  
  /** \details
    The default implementation of this function does nothing.
      
    \param numPoints [in]  Number of points.
    \param vertexList [in]  Array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \remarks
    The extrusion vector specifies the direction and distance of the extrusion.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void polygonProc(
    OdInt32 numPoints, const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0, const OdGeVector3d* pExtrusion = 0);

  /** \details
    The default implementation of this function does nothing.
      
    \param rows [in]  Number of rows.
    \param columns [in]  Number of columns.
    \param vertexList [in]  Array of vertices.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data. 
    \sa
    <link tv_geometry_meshes, Work with Meshes>
  */
  void meshProc(
    OdInt32 rows,
    OdInt32 columns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);
  
  /** \details
    The default implementation of this function does nothing.
      
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
  void shellProc(
    OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);
  
  /** \details
    The default implementation of this function does nothing.
      
    \param position [in]  Position of the text.
    \param direction [in]  Baseline direction of the text.
    \param upVector [in]  Up vector for the text.
    \param msg [in]  Text string.
    \param numChars [in]  Number of characters in msg.
    \param raw [in]  If and only if true, escape sequences, such as %%P, will not be converted to special characters.
    \param pTextStyle [in]  Pointer to the TextStyle for the text.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \sa
    <link tv_geometry_text, Work with Text>
  */
  void textProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, const OdGeVector3d& upVector,
    const OdChar* msg, OdInt32 numChars, bool raw, const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    The default implementation of this function does nothing.
      
    \param position [in]  Position of the shape.
    \param direction [in]  Baseline direction of the shape.
    \param upVector [in]  Up vector for the shape.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
    \param pExtrusion [in]  Pointer to the extrusion vector.
  */
  void shapeProc(
    const OdGePoint3d& position,
    const OdGeVector3d& direction, const OdGeVector3d& upVector,
    int shapeNumber, const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);

  /** \details
    The default implementation of this function does nothing.
      
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \remarks
    An Xline is an infinite line passing through the 
    specified points.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void xlineProc(
    const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);
  
  /** \details
    The default implementation of this function does nothing.
      
    \param basePoint [in]  Base point.
    \param throughPoint [in]  Through point 
    \remarks
    A Ray is a semi-infinite line that starts at the basePoint, and passes 
    through the throughPoint.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  void rayProc( const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint);
  
  /** \details
    The default implementation of this function does nothing.
    
    \param nurbsCurve [in]  NURBS curve data.
    \sa
    <link tv_geometry_nurbs, Work with NURBS>
  */
  void nurbsProc( const OdGeNurbCurve3d& nurbsCurve);

  /** \details
    The default implementation of this function does nothing.
      
    \param ellipArc [in]  Elliptical arc.
    \param endPointsOverrides [in]  Array of points to be used as the first and last points of the vectorized arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Pointer to the extrusion vector.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  void ellipArcProc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointsOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple, const OdGeVector3d* pExtrusion = 0);

  /** \details
    The default implementation of this function does nothing.
      
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
  void rasterImageProc(
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
    The default implementation of this function does nothing.
      
    \param origin [in]  Metafile origin.
    \param u [in]  Metafile width vector.
    \param v [in]  Metafile height vector.
    \param pMetafile [in]  Pointer to the metafile object.
    \param dcAligned [in]  Flag that specifies whether metafile content should be aligned to device coordinate system.
    \param allowClipping [in]  Flag that specifies whether clipping should be allowed for a metafile.
  */
	void metafileProc(
    const OdGePoint3d& origin,
		const OdGeVector3d& u,
		const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,           
    bool allowClipping = false);

  /** \details
    The default implementation of this function does nothing.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pColors [in]  Pointer to point colors.
    \param pTransparency [in]  Pointer to point transparency.
    \param pNormals [in]  Array of normal vectors.
    \param pExtrusions [in]  Array of vectors that defines extrusions.
    \param pSubEntMarkers [in]  Array of sub-entity markers.
    \param nPointSize [in]  Points size.
    \remarks
    The extrusion vectors specifies the direction and distance of the extrusion.
  */
  void polypointProc(
    OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency = 0,
    const OdGeVector3d* pNormals = 0,
    const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0,
    OdInt32 nPointSize = 0);

  /** \details
    The default implementation of this function does nothing.
      
    \param numPoints [in]  Number of points.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset to next point.
  */
  void rowOfDotsProc(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  /** \remarks
    The default implementation of this function does nothing.

    \param pCloud [in]  Point cloud object.
    \param pFilter [in]  Optional set of point cloud object conveyor filters.
  */
  void pointCloudProc(const OdGiPointCloud &pCloud, const OdGiPointCloudFilter *pFilter = NULL);

  /** \details
    The default implementation of this function does nothing.
      
    \param edges [in]  2d curves array.
    \param pXform [in]  Optional boundary transform matrix.
  */
  void edgeProc(const OdGiEdge2dArray& edges, const OdGeMatrix3d* pXform = 0);
};

#include "TD_PackPop.h"

#endif //#ifndef __ODGIEMPTYGEOMETRY_H__
