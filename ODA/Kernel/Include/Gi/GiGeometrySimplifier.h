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

#ifndef __ODGIGEOMETRYSIMPLIFIER_H__
#define __ODGIGEOMETRYSIMPLIFIER_H__

#include "Gi/GiFill.h"
#include "Gi/GiMapperItem.h"
#include "Gi/GiExport.h"
#include "Gi/GiConveyorGeometry.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GePoint3dArray.h"
#include "Ge/GeVector3dArray.h"
#include "Gs/Gs.h"

class OdGiDeviation;

class OdGeLineSeg2d;
class OdGeLine2d;
class OdGeCircArc2d;
class OdGeCircArc3d;
class OdGeEllipArc3d;

#include "TD_PackPush.h"

/** \details
    This class provides tessellation functionality for the ODA Platform vectorization framework.
    
    \remarks
    Tessellation consists of breaking complex entities into sets of simpler entities.  
    
    An instance of this class is used to simplify the geometry produced by the ODA Platform vectorization
    framework.

    <group OdGi_Classes> 
*/
class ODGI_EXPORT OdGiGeometrySimplifier : public OdGiConveyorGeometry
{
  // Old version do not override !!!
  // Overide shellFaceOut() or triangleOut() instead
  virtual int polygonWithHolesOut(OdUInt32, const OdGePoint3d*, OdUInt32 , OdUInt32*);
protected:
  /** \details
    Draws an arc of the specified type.
    \param arcType [in]  Arc type.
    \param center [in]  Center of arc.
    \param points [out]  Receives an array of points comprising the arc.
    \param pNormal [in]  Pointer to the normal to the arc.
    \param pExtrusion [in]  Pointer to the extrusion direction of the arc.
  */
  void drawTypedArc(OdGiArcType arcType, const OdGePoint3d& center,
    OdGePoint3dArray& points, const OdGeVector3d* pNormal, const OdGeVector3d* pExtrusion);
protected:
  OdGiConveyorContext*  m_pDrawCtx;
  OdGiSubEntityTraits*  m_pTraits;
  OdGsMarker            m_baseSubEntMarker;

  enum SimplifierFlags
  {
    kSimplProcessingText   = (1 << 0), // True if simplifier is inside TrueType text processing.
    kSimplFillModeDisabled = (1 << 1), // True if shell/mesh primitives must be filled during vectorization.
    kSimplEyeSpaceSilhs    = (1 << 2), // Enables silhouettes processing in ECS (Eye Space).
    kSimplEdgeVisible      = (1 << 3), // Indexed polylineOut() call represents visible edge.
    kSimplEdgeSilhouette   = (1 << 4), // Indexed polylineOut() call represents silhouette edge.
    kSimplLastFlag         = kSimplEdgeSilhouette
  };
  OdUInt32              m_simplFlags;
private:
  OdInt32               m_nVertexCount;
  const OdGePoint3d*    m_pVertexList;
  const OdGiVertexData* m_pVertexData;
  OdGePoint2dArray      m_loopsPnts;
  OdGePoint3dArray      m_points3d1; // for shellProc/plineProc level
  OdGePoint3dArray      m_points3d2; // for triangleOut/polylineOut level
protected:
  const OdGiDeviation*  m_pDeviation;
  OdGeDoubleArray       m_deviations;
  OdGsView::RenderMode  m_renderMode;

  /** \details
    Returns the recommended maximum deviation of the current vectorization 
    for the specified point on the curve or surface being tesselated.
    \param deviationType [in]  Deviation type.
    \param pointOnCurve [in]  Point on the curve.
  */
  double deviation(const OdGiDeviationType deviationType, const OdGePoint3d& pointOnCurve) const;

  /** \details
    Returns the FillMode and DrawEdges for this Vectorizer object.
    \param drawEdges [out]  Receives the DrawEdges value.
    \remarks
    As implemented, this function returns false if called without arguments
  */
  bool fillMode();
  bool fillMode(bool& drawEdges);
  bool fillMode(bool& drawEdges, bool& secColor);
  bool circleArcFillMode();
  bool shmFillMode(bool& drawEdges);
  bool shmFillMode(bool& drawEdges, bool& secColor);

  /** \details
    Returns true if and only if this Vectorizer object is processing TrueType text.
  */
  bool processingText() const { return GETBIT(m_simplFlags, kSimplProcessingText); }

  /** \details
    Returns true if silhouettes processing in ECS (Eye Coordinates Space) enabled.
  */
  bool silhouettesProcessingInEyeSpace() const { return GETBIT(m_simplFlags, kSimplEyeSpaceSilhs); }
  /** \details
    Enables processing silhouettes in ECS (Eye Coordinates Space).
  */
  void enableSilhouettesProcessingInEyeSpace(bool bEnable) { SETBIT(m_simplFlags, kSimplEyeSpaceSilhs, bEnable); }

  /** \details
    Returns the OdGiContext for this Vectorizer object.
  */
  OdGiContext& giCtx() const { return const_cast<OdGiContext&>(m_pDrawCtx->giContext()); }

  /** \details
    Controls plineProc arc segment output.
    \param drawContextFlags [in]  Input conveyor context draw flags.
    \returns
    Returns true if plineProc arc segments as arc output enabled.
    Returns false to output plineProc arc segments as polyline segments chain.
  */
  virtual bool plineArcSegmentsAsArcProc(OdUInt32 drawContextFlags) const;
public:
  OdGiGeometrySimplifier();
  ~OdGiGeometrySimplifier();

  /** \details
    Specifies the maximum deviation allowed during the 
    tessellation process.
    
    \param deviations [in]  Array of deviation values. Specifies the limit for the maximum difference 
    between the actual curve or surface, and the tessellated curve or surface.
    
    \remarks
    The deviation values (in isotropic world space) are  
    the deviation types defined by the OdGiDeviationType enum.

    Each OdGiDeviationType value is used as an index into this array.
  */
  void setDeviation(const OdGeDoubleArray& deviations);

  /** \details
    Specifies the maximum deviation allowed during the 
    tessellation process. 

    \param pDeviation [in]  Deviation to be used for anisotropic space (perspective view).
  */
  void setDeviation(const OdGiDeviation* pDeviation);

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
  OdGsView::RenderMode renderMode() { return m_renderMode; }

  /** \details
     Sets the OdGiConveyorContext for this object.  
     \param pDrawCtx [in]  Pointer to the draw context.
     \remarks
     The draw context must be set before 
     calling any of the tessellation functions.
  */
  void setDrawContext(OdGiConveyorContext* pDrawCtx);

  /** \details
    Retrieves the OdGiConveyorContext associated with this Vectorizer object.
    
    \returns
    pointer to the OdGiConveyorContext object associated with this Vectorizer.
  */
  OdGiConveyorContext* drawContext( ) { return m_pDrawCtx; }
  
  /** \details
    Retrieves the OdGiConveyorContext associated with this Vectorizer object.
    
    \returns
    pointer to the OdGiConveyorContext object associated with this Vectorizer.
  */
  const OdGiConveyorContext* drawContext( ) const { return m_pDrawCtx; }

  /** \details
    Passes polyline data to this Vectorizer object.

    \param numPoints [in]  Number of points in the polyline.
    \param vertexList [in]  Array of vertices that make up the polyline.
    \remarks
    This function is called by the OdGiGeometrySimplifier class to 
    pass polyline data generated during tessellation, to a client application.
    Client applications have the option of overriding this function to process 
    the data themselves.  If the function is not overridden, 
    the default implementation of this function calls polylineOut with the vertex list.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void polylineOut(OdInt32 numPoints, const OdGePoint3d* vertexList);

  /** \details
      Passes polygon data to this Vectorizer object.

      \param numPoints [in]  Number of points in the polygon.
      \param vertexList [in]  Array of vertices that make up the polygon.
      \param pNormal [in]  Normal vector for the polygon.
      \remarks
      This function is called by the OdGiGeometrySimplifier class to 
      pass polygon data generated during tessellation, to a client application.
      Clients should override this function.
      \sa
      <link tv_geometry_polygons, Work with Polygons>
  */
  virtual void polygonOut(OdInt32 numPoints, 
    const OdGePoint3d* vertexList, 
    const OdGeVector3d* pNormal = 0);

  /** \details
      Sets vertex data for tessellation of meshes and shells.

      \param numVertices [in]  Number of vertices in the mesh or shell.
      \param vertexList [in]  Array of vertices that make up the mesh or shell.
      \param pVertexData [in]  Pointer to additional vertex data.
      \remarks
      Called by the default implementations of meshProc and shellProc, to 
      set vertex data that will be used in the tessellation of these objects.
  */
  void setVertexData(OdInt32 numVertices,
    const OdGePoint3d* vertexList,
    const OdGiVertexData* pVertexData = 0);

  /** \details
    Retrieves the vertex count set in the most recent call to setVertexData.
    
    \returns
    vertex count as OdInt32 value.
  */
  OdInt32 vertexDataCount() const
  {
    return m_nVertexCount;
  }

  /** \details
    Retrieves the vertex list set in the most recent call to setVertexData.
      
    \returns
    pointer to the vertex list.
  */
  const OdGePoint3d* vertexDataList() const
  {
    return m_pVertexList;
  }

  /** \details
    Retrieves the vertex attribute data set in the most recent call to setVertexData.
      
    \returns
    pointer to the vertex attributes.
  */
  const OdGiVertexData* vertexData() const
  {
    return m_pVertexData;
  }

  /** \details
    Retrieves base sub-entity selection marker for polylineOut().
    
    \returns
    sub-entity selection markers.
  */
  OdGsMarker baseSubEntMarker() const
  {
    return m_baseSubEntMarker;
  }

  /** \details
    Retrieves edge visibility state for indexed polylineOut() call.
    
    \returns
    Edge visibility state.
  */
  OdGiVisibility polylineOutEdgeVisibility() const
  {
    return (GETBIT(m_simplFlags, kSimplEdgeVisible)) ? kOdGiVisible :
             ((GETBIT(m_simplFlags, kSimplEdgeSilhouette)) ? kOdGiSilhouette : kOdGiInvisible);
  }

  /** \details
    Processes polylines.
    
    \param polyline [in]  Polyline to process.
    \param pXfm [in]  Transformation matrix.
    \param fromIndex [in]  Index of the first segment to be processed.
    \param numSegs [in]  Number of segments to be processed.
    \remarks
    Client applications have the option of overriding this function to process the polyline
    data themselves.
    If the function is not overridden, the default implementation of 
    this function processes the polyline. 
    In the default implementation of this function, line segments with a non-zero width 
    will be converted into calls to shellProc, and segments with no width will generate
    calls to polylineProc.  Arc segments with non-zero width will be converted into 
    calls to polygonProc, and arc segments with no width will generate calls to 
    polylineProc.      
    OdGiGeometrySimplifier::plineProc is only called for polylines with a continuous
    linetype. 
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void plineProc(const OdGiPolyline& polyline,
    const OdGeMatrix3d* pXfm = 0,
    OdUInt32 fromIndex = 0,
    OdUInt32 numSegs = 0);

  /** \details
    Processes polylines.

    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param pExtrusion [in]  Extrusion vector.
    \param baseSubEntMarker [in]  Base sub-entity marker. 
    \remarks
    Client applications have the option of overriding this function to process the polyline
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the polyline, calling polylineOut if the data is not 
    extruded, or meshProc if an extrusion value is present.
    OdGiGeometrySimplifier::polylineProc is only called for polylines with a continuous
    linetype. 
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void polylineProc(OdInt32 numPoints, 
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0, 
    const OdGeVector3d* pExtrusion = 0,
    OdGsMarker baseSubEntMarker = -1);

  /** \details
    Processes polygons.
      
    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pNormal [in]  Pointer to the normal vector.
    \param pExtrusion [in]  Extrusion vector.
    \remarks
    Client applications have the option of overriding this function to process the polygon
    data themselves. If the function is not overridden, the default implementation
    of this function processes the polygon, calling polygonOut if the data is not 
    extruded, or shellProc if an extrusion value is present.
    \sa
    <link tv_geometry_polygons, Work with Polygons>
  */
  virtual void polygonProc(OdInt32 numPoints, 
    const OdGePoint3d* vertexList,
    const OdGeVector3d* pNormal = 0, 
    const OdGeVector3d* pExtrusion = 0);

  /** \details
    Processes x-lines.
    
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.    
    \remarks
    This function should not be overridden by clients.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void xlineProc(const OdGePoint3d& firstPoint, const OdGePoint3d& secondPoint);

  /** \details
    Processes rays.
    
    \param basePoint [in]  Base point.
    \param throughPoint [in]  Through point   
    \remarks
    This function should not be overridden by clients.
    \sa
    <link tv_geometry_lines, Work with Lines>
  */
  virtual void rayProc(const OdGePoint3d& basePoint, const OdGePoint3d& throughPoint);

  /** \details
    Processes meshes.
    
    \param numRows [in]  Number of rows.
    \param numColumns [in]  Number of columns.
    \param vertexList [in]  Array of vertices.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data.   
    \remarks
    Client applications have the option of overriding this function to process the mesh
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the mesh into a set of facets.  More specifically,
    the default meshProc function calls setVertexData, and the calls either
    generateMeshFaces for filled facets, or generateMeshWires for a wireframe.
    \sa
    <link tv_geometry_meshes, Work with Meshes>
  */
  virtual void meshProc(OdInt32 numRows,
    OdInt32 numColumns,
    const OdGePoint3d* vertexList,
    const OdGiEdgeData* pEdgeData = 0,
    const OdGiFaceData* pFaceData = 0,
    const OdGiVertexData* pVertexData = 0);

  /** \details
    Tessellates a wireframe mesh.
    
    \param numRows [in]  Number of rows in the mesh.
    \param numColumns [in]  Number of columns in the mesh.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    
    \remarks
    This function is called from the default implementation of meshProc
    to tessellate a wireframe mesh. Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.

    Client applications have the option of overriding this function to process the mesh
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the mesh into a set of facets.  More specifically,
    the default generateMeshWires function processes mesh attributes, and calls
    polylineOut with the tessellated mesh data.
  */
  virtual void generateMeshWires(OdInt32 numRows, 
    OdInt32 numColumns,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData);

  /** \details
    Tessellates a filled mesh.
    
    \param numRows [in]  Number of rows in the mesh.
    \param numColumns [in]  Number of columns in the mesh.
    \param pFaceData [in]  Pointer to additional face data.
    
    \remarks
    This function is called from the default implementation of meshProc
    to tessellate a filled mesh. Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.

    Client applications have the option of overriding this function to process the mesh
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the mesh into a set of facets.  More specifically,
    the default generateMeshFaces function processes mesh attributes, and calls
    triangleOut function with the tessellated mesh data.
  */
  virtual void generateMeshFaces(OdInt32 numRows, OdInt32 numColumns,
                                 const OdGiFaceData* pFaceData);

  /** \details
    Outputs a face of a tessilated mesh.
    \param faceList [in]  Array of numbers that define the face.
    \param pNormal [in]  Pointer to the normal to the face.
    \sa
    Vectorization Primitives
  */
  virtual void meshFaceOut(const OdInt32* faceList, const OdGeVector3d* pNormal);


  /** \details
    BIM-specific version of shellProc which also generates fill pattern data
    for shell faces.

    \param vertexList [in]  OdGePoint3d Array which represents vertices.
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the faces in the shell.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \sa
    <link tv_geometry_shells, Work with Shells>
    \sa
    <link tv_working_with_attributes_fill_patterns, Work with Fill Patterns>
  */
  virtual void filledShellProc(
                 const OdGePoint3d* vertexList,
                 OdInt32 faceListSize,
                 const OdInt32* faceList,
                 const OdGiEdgeData* pEdgeData = 0,
                 const OdGiFaceData* pFaceData = 0);


  /** \details
    Processes shells.
    
    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Array of vertices.
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  Array of numbers that define the faces.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.
    \param pVertexData [in]  Pointer to additional vertex data.
    \remarks
    Client applications have the option of overriding this function to process the shell
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the shell into a set of facets.  More specifically,
    the default shellProc function calls setVertexData, and the calls either
    generateShellFaces for filled facets, or generateShellWires for a wireframe.
    \sa
    <link tv_geometry_shells, Work with Shells>
  */
  virtual void shellProc(OdInt32 numVertices,
                 const OdGePoint3d* vertexList,
                 OdInt32 faceListSize,
                 const OdInt32* faceList,
                 const OdGiEdgeData* pEdgeData = 0,
                 const OdGiFaceData* pFaceData = 0,
                 const OdGiVertexData* pVertexData = 0);

  /** \details
    Tessellates a wireframe shell.
    
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the faces in the shell.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.  
    \remarks
    This function is called from the default implementation of shellProc
    to tessellate a wireframe shell. Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.
    Client applications have the option of overriding this function to process the shell
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the shell into a set of facets.  More specifically,
    the default generateShellWires function processes shell attributes, and calls
    polylineOut with the tessellated shell data.
    \sa
    OdGiEdgeData, OdGiFaceData, Vectorization Primitives
  */
   virtual void generateShellWires(OdInt32 faceListSize,
                                   const OdInt32* faceList,
                                   const OdGiEdgeData* pEdgeData = 0,
                                   const OdGiFaceData* pFaceData = 0);

  /** \details
    Tessellates a filled wireframe shell.
    
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the faces in the shell.
    \param pEdgeData [in]  Pointer to additional edge data.
    \param pFaceData [in]  Pointer to additional face data.    
    \remarks
    This function is called from the default implementation of shellProc
    to tessellate a filled wireframe shell. Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.
    Client applications have the option of overriding this function to process the shell
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the shell into a set of facets.  More specifically,
    the default generateShellFaces function processes shell attributes, and calls
    shellFaceOut with the tessellated shell data.   
    \sa
    OdGiEdgeData, OdGiFaceData, Vectorization Primitives
  */
  virtual void generateShellFaces(OdInt32 faceListSize,
                                  const OdInt32* faceList,
                                  const OdGiEdgeData* pEdgeData = 0,
                                  const OdGiFaceData* pFaceData = 0);

  /** \details
    Outputs the face of a tessellated shell.

    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the face.
    \param pNormal [in]  Normal vector for this face.      
    \remarks
    This function is called from the default implementation of generateShellFaces,
    to process a single face in a shell along with the holes in that face.
    Vertex data needed for this function can be obtained by calling 
    vertexDataList and vertexData.
    Client applications have the option of overriding this function to process the face
    data themselves.  If the function is not overridden, the default implementation
    of this function processes the face information calls
    triangleOut with the resulting triangle data.
    \sa
    Vectorization Primitives
  */
    virtual void shellFaceOut(OdInt32 faceListSize,
                        const OdInt32* faceList,
                        const OdGeVector3d* pNormal);

  /** \details
    Output a triangle.
    
    \param vertices [in]  List of 3 numbers that define the vertices in the triangle.
    \param pNormal [in]  Normal vector for this triangle.    
    \remarks
    This function is called from the default implementations of shellFaceOut
    and generateMeshFaces.  Vertex data needed for this function
    can be obtained by calling vertexDataList and vertexData.    
    Client applications have the option of overriding this function to process 
    the triangle data themselves.  If the function is not overridden, 
    the default implementation of this function processes the color 
    attributes for the triangle, and calls polygonOut.
  */
  virtual void triangleOut(const OdInt32* vertices,
                           const OdGeVector3d* pNormal);

  /** \details
    Tessellates a shell.
    
    \param faceListSize [in]  Number of entries in faceList.
    \param faceList [in]  List of numbers that define the faces in the shell.
    \param pFaceData [in]  Pointer to additional face data.
    \param maxFacetSize [in]  Maximum number of edges in the tessellated faces produced
      by this function.   
    \remarks
    This auxiliary function that can be used to tessellate a shell up into a set of faces, 
    each with a maximum number of edges.    
    Vertex data needed for this function is obtained by calling 
    vertexDataList and vertexData (so the caller of this function is responsible for 
    setting this data).  Face data is passed to the facetOut function, which 
    can be overridden by client applications to capture the tessellated data.    
    \sa
    OdGiFaceData, Vectorization Primitives
  */
  virtual void generateShellFacets(OdInt32 faceListSize,
    const OdInt32* faceList,
    const OdGiFaceData* pFaceData = 0,
    OdInt32 maxFacetSize = 3);

  /** \details
    Outputs a facet.
    
    \param faceList [in]  List of numbers that define the vertices in the facet.  
    \param edgeIndices [in]  Currently not used.
    \param pNormal [in]  Pointer to the normal vector for this face.    
    \remarks
    This function is called from the default implementations of generateShellFacets.  
    Vertex data needed for this function is obtained by calling 
    vertexDataList and vertexData.
    Client applications have the option of overriding this function to process 
    the face data themselves.  If the function is not overridden, 
    the default implementation of this function calls polygonOut with the face data.
    \sa
    Vectorization Primitives
  */
  virtual void facetOut(const OdInt32* faceList,
        const OdInt32* edgeIndices,
        const OdGeVector3d* pNormal);

  /** \details
    Introduces a circle into this vectorization context.
    
    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param pExtrusion [in]  Extrusion vector.    
    \remarks
    Client applications have the option of overriding these function to process the circle
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in circle using the current kOdGiMaxDevForCircle deviation,
    and calls polylineProc with the resulting data.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  virtual void circleProc(const OdGePoint3d& center,
    double radius, 
    const OdGeVector3d& normal,
    const OdGeVector3d* pExtrusion = 0);

  /** \details
    Introduces a circle into this vectorization context.

    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal vector.
    \param startVector [in]  Normal vector.
    \param pExtrusion [in]  Extrusion vector.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  void circleProc2(const OdGePoint3d& center, double radius,
    const OdGeVector3d& normal, const OdGeVector3d& startVector,
    const OdGeVector3d* pExtrusion = 0);

  /** \details
    Processes a circle.

    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \param thirdPoint [in]  Third point.
    \param pExtrusion [in]  Extrusion vector.
    \sa
    <link tv_geometry_circles, Work with Circles>
  */
  virtual void circleProc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint, 
    const OdGePoint3d& thirdPoint,
    const OdGeVector3d* pExtrusion = 0);

  /** \details
    Introduces a circular arc into this vectorization context.
    
    \param center [in]  Center point.
    \param radius [in]  Radius.
    \param normal [in]  Normal to a plane on which a circular arc lies.
    \param startVector [in]  Defines the start of this arc.
    \param sweepAngle [in]  Angle that defines the arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Extrusion vector.    
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     2         Filled area bounded by the arc and its end points
    </table>
    \remarks
    Client applications have the option of overriding this function to process the circular arc
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in circular arc using the current kOdGiMaxDevForCircle deviation,
    and calls polylineProc with the resulting data (for kOdGiArcSimple type arcs).  For 
    kOdGiArcSector and kOdGiArcChord arc types, polygonOut is called.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  virtual void circularArcProc(const OdGePoint3d& center,
    double radius,
    const OdGeVector3d& normal,
    const OdGeVector3d& startVector,
    double sweepAngle,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  /** \details
    Introduces a circular arc into this vectorization context.
    
    \param firstPoint [in]  First point.
    \param secondPoint [in]  Second point.
    \param thirdPoint [in]  Third point.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Extrusion vector.
    \remarks
    arcType must be one of the following:
    
    <table>
    Name              Value     Description
    kOdGiArcSimple    0         Unfilled.
    kOdGiArcSector    1         Filled area bounded by the arc and its center.
    kOdGiArcChord     2         Filled area bounded by the arc and its end points
    </table>    
    \remarks
    Client applications have the option of overriding this function to process the circular arc
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in circular arc using the current kOdGiMaxDevForCircle deviation,
    and calls polylineProc with the resulting data (for kOdGiArcSimple type arcs).  For 
    kOdGiArcSector and kOdGiArcChord arc types, polygonOut is called.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  virtual void circularArcProc(const OdGePoint3d& firstPoint,
    const OdGePoint3d& secondPoint,
    const OdGePoint3d& thirdPoint,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  /** \details
    Introduces a text into this vectorization context.
    
    \param position [in]  Position of the text.
    \param u [in]  U parameter of the text.
    \param v [in]  V parameter of the text.
    \param msg [in]  Message string.
    \param length [in]  Message length.
    \param raw [in]  Flag that specifies whether escape sequences, such as %%P, will not be converted to special characters.
    \param pTextStyle [in]  Pointer to a text style.
    \param pExtrusion [in]  Extrusion vector.
    \remarks
    Client applications have the option of overriding this function to process the text
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in text string, by calling the textProc function
    on the associated OdGiContext object.  TrueType text will result in calls to 
    shellProc, and SHX text will get sent to polylineProc and polygonProc.
    \sa
    <link tv_geometry_text, Work with Text>
  */
  virtual void textProc(const OdGePoint3d& position,
    const OdGeVector3d& u, 
    const OdGeVector3d& v,
    const OdChar* msg, 
    OdInt32 length, 
    bool raw,
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);
  
  /** \details
    Introduces a text into this vectorization context.
    
    \param position [in]  Position of the text.
    \param u [in]  U parameter of the text.
    \param v [in]  V parameter of the text.
    \param msg [in]  Message string.
    \param length [in]  Message length.
    \param raw [in]  Flag that specifies whether escape sequences, such as %%P, will not be converted to special characters.
    \param pTextStyle [in]  Pointer to a text style.
    \param pExtrusion [in]  Extrusion vector.
    \param extentsBox [in]  Text extents bounds. 
    \remarks
    Client applications have the option of overriding this function to process the text
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in text string, by calling the textProc function
    on the associated OdGiContext object.  TrueType text will result in calls to 
    shellProc, and SHX text will get sent to polylineProc and polygonProc.
    \sa
    <link tv_geometry_text, Work with Text>
  */
  virtual void textProc2(const OdGePoint3d& position,
    const OdGeVector3d& u, const OdGeVector3d& v,
    const OdChar* msg, OdInt32 length, bool raw, const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0, const OdGeExtents3d* extentsBox = 0);

  /** \details
    Introduces a shape into this vectorization context.
    
    \param position [in]  Position of the shape.
    \param direction [in]  Baseline direction for the shape.
    \param upVector [in]  Text up vector.
    \param shapeNumber [in]  Shape number.
    \param pTextStyle [in]  Pointer to the TextStyle for the shape.
    \param pExtrusion [in]  Extrusion vector.    
    \remarks
    Client applications have the option of overriding this function to process the shape
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in shape, by calling the shapeProc function
    on the associated OdGiContext object.  The resulting geometry 
    will get sent to polylineProc and polygonProc.
  */
  virtual void shapeProc(const OdGePoint3d& position,
    const OdGeVector3d& direction, 
    const OdGeVector3d& upVector,
    int shapeNumber, 
    const OdGiTextStyle* pTextStyle,
    const OdGeVector3d* pExtrusion = 0);

  /** \details
    Introduces nurbs into this vectorization context.
    
    \param nurbsCurve [in]  NURBS curve data.    
    \remarks
    Client applications have the option of overriding this function to process the NURBS
    data themselves.  If the function is not overridden, the default implementation
    tessellates the passed in NURBS curve using the current kOdGiMaxDevForCurve deviation,
    and calls polylineProc with the resulting data.
    \sa
    <link tv_geometry_nurbs, Work with NURBS>
  */
  virtual void nurbsProc(const OdGeNurbCurve3d& nurbsCurve);

  /** \details
    Introduces an elliptical arc into this vectorization context.

    \param ellipArc [in]  Elliptical arc.
    \param endPointOverrides [in]  Array of points to be used as the first and last points of the vectorized arc.
    \param arcType [in]  Arc type.
    \param pExtrusion [in]  Extrusion vector.    
    \remarks
    Client applications have the option of overriding this function to process the elliptical arc
    data themselves. If the function is not overridden, the default implementation
    tessellates the passed in elliptical arc using the current kOdGiMaxDevForCurve deviation,
    and calls polylineProc with the resulting data.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  virtual void ellipArcProc(
    const OdGeEllipArc3d& ellipArc,
    const OdGePoint3d* endPointOverrides = 0,
    OdGiArcType arcType = kOdGiArcSimple,
    const OdGeVector3d* pExtrusion = 0);

  /** \details
    Introduces an elliptical arc into this vectorization context.
    
    \param ellipArc [in]  Elliptical arc.
    \param width [in]  Arc width.    
    \remarks
    Client applications have the option of overriding this function to process the elliptical arc
    data themselves. If the function is not overridden, the default implementation
    tessellates the passed in elliptical arc using the current kOdGiMaxDevForCurve deviation,
    and calls polylineProc with the resulting data.
    \sa
    <link tv_geometry_arcs, Work with Arcs>
  */
  virtual bool ellipArcProc(const OdGeEllipArc3d& ellipArc, double width);

  /** \details
    Introduces a raster image into this vectorization context.
    
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
    \remarks
    Client applications have the option of overriding this function to process raster images
    data themselves.
    \sa
    <link tv_geometry_raster_images, Work with Raster Image Geometry Data>
  */
  virtual void rasterImageProc(const OdGePoint3d& origin,
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
    Initializes a texture map for this Vectorizer object.

    \param origin [in]  Lower-left corner. 
    \param u [in]  Image width vector.
    \param v [in]  Image height vector.
    \param pImage [in]  Pointer to the RasterImage object.
    \param transparency [in]  True if and only if image transparency is on.
    \param brightness [in]  Image brightness [0.0 .. 100.0].
    \param contrast [in]  Image contrast [0.0 .. 100.0].
    \param fade [in]  Image fade value [0.0 .. 100.0].
  */
  virtual void initTexture(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiRasterImage* pImage,
    bool transparency,
    double brightness,
    double contrast,
    double fade);

  /** \details
    Releases a texture map for this Vectorizer object.
  */
  virtual void uninitTexture();

  /** \details
    Processes a metafile. Currently not implemented.
    
    \param origin         [in]  Origin of a metafile content.
    \param u              [in]  Metafile content u-parameter.
    \param v              [in]  Metafile content v-parameter.
    \param pMetafile      [in]  Pointer to a metafile.
    \param dcAligned      [in]  Flag that specifies whether metafile content should be aligned to device coordinate system.
    \param allowClipping  [in]  Flag that specifies whether clipping should be allowed for a metafile.
  */
  virtual void metafileProc(const OdGePoint3d& origin,
    const OdGeVector3d& u,
    const OdGeVector3d& v,
    const OdGiMetafile* pMetafile,
    bool dcAligned = true,        
    bool allowClipping = false); 

  /** \details
    Introduces a polypoint into this vectorization context.
    
    \param numPoints [in]  Number of points.
    \param vertexList [in]  Pointer to an array of vertices.
    \param pColors [in]  Pointer to point colors.
    \param pTransparency [in]  Pointer to point transparency.
    \param pNormals [in]  Array of normal vectors.
    \param pSubEntMarkers [in]  Array of sub-entity markers.
    \param nPointSize [in]  Points size.
    
    \remarks
    Client applications have the option of overriding this function to process the polypoint
    data themselves. If the function is not overridden, the default implementation
    of this function processes the polypoint, calling polylineProc for each point.
  */
  virtual void polypointProc(OdInt32 numPoints,
    const OdGePoint3d* vertexList,
    const OdCmEntityColor* pColors,
    const OdCmTransparency* pTransparency = 0,
    const OdGeVector3d* pNormals = 0,
    const OdGeVector3d* pExtrusions = 0,
    const OdGsMarker* pSubEntMarkers = 0,
    OdInt32 nPointSize = 0);

  /** \details
    Introduces a row of points into this vectorization context.

    \param numPoints [in]  Number of points.
    \param startPoint [in]  First point to draw.
    \param dirToNextPoint [in]  Offset to next point.
    
    \remarks
    Client applications have the option of overriding this function to process the rowOfDots
    data themselves. If the function is not overridden, the default implementation
    of this function processes the rowOfDots, calling polylineProc for each point.
  */
  virtual void rowOfDotsProc(OdInt32 numPoints, const OdGePoint3d& startPoint, const OdGeVector3d& dirToNextPoint);

  /** \details
    Introduces an point cloud object into this vectorization context.

    \param pCloud [in]  Point cloud object.
    \param pFilter [in]  Optional set of point cloud object conveyor filters.

    \remarks
    Client applications have the option of overriding this function to process the point cloud
    data themselves. If the function is not overridden, the default implementation
    of this function processes the point cloud, calling polypointProc for set of points.
  */
  virtual void pointCloudProc(const OdGiPointCloud &pCloud, const OdGiPointCloudFilter *pFilter = NULL);

  /** \details
    Introduces an edge into this vectorization context.
    
    \param edges [in]  Array of 2d points that define a boundary.
    \param pXform [in]  Transformation matrix.
    
    \remarks
    Client applications have the option of overriding this function to process the edge
    data themselves. If the function is not overridden, the default implementation
    of this function processes the edge, calling polylineProc for each hatch line or
    shellProc for solid and gradient filling.
  */
  virtual void edgeProc(const OdGiEdge2dArray& edges, const OdGeMatrix3d* pXform = 0);

  /** \details
    Passes polyline data to this Vectorizer object.
    
    \param numPoints [in]  Number of points.
    \param vertexIndexList [in]  Pointer to an array of vertex indices.
    
    \remarks
    This function is called by the OdGiGeometrySimplifier class to 
    pass polyline data generated during tessellation, to a client application.

    Client applications have the option of overriding this function to process 
    the data themselves.  If the function is not overridden, 
    the default implementation of this function calls polylineOut with the vertex list.
  */
  virtual void polylineOut(OdInt32 numPoints, const OdInt32* vertexIndexList);

  /** \details
    Utility function to be called from client implementation of meshProc()
    to unify mesh primitives processing.
    
    \param rows [in]  Number of rows.
    \param columns [in]  Number of columns.
    \param pVertexList [in]  Pointer to an array of vertices.
    \param pEdgeData [in]  Additional edge data.
    \param pFaceData [in]  Additional face data.
    \param pVertexData [in]  Additional vertex data.
    \sa
    <link tv_geometry_meshes, Work with Meshes>
    \sa
    <link tv_geometry_shells, Work with Shells>
  */
  void convertMeshToShell(
    OdInt32 rows, OdInt32 columns,
    const OdGePoint3d* pVertexList,
    const OdGiEdgeData* pEdgeData,
    const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData);

  /** \details
    Processes TrueType font characters in dynamic polyDraw mode which is similar to shellProc() method, 
    except here additional information about bezier data is passed for processing. 
    
    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Pointer to an array of vertices.
    \param faceListSize [in]  Number of faces.
    \param faceList [in]  Pointer to an array of faces.
    \param pBezierTypes [in]  Additional bezier data for the geometry conveyor, so client code can tessellate text using any quality that is required for TrueType font processing.
    \param pFaceData [in]  Additional face data.    
    \remarks
    Client applications have the option of overriding this function to process the PolyDraw
    data themselves. If the function is not overridden, the default implementation
    tessellates the passed in PolyDraw contours using the current kOdGiMaxDevForCurve deviation,
    and calls shellProc with the resulting data.
    \sa
    <link gi_ttf_render_quality, Adjusting the Quality of TrueType Font Text>
  */
  virtual void ttfPolyDrawProc(
    OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdUInt8* pBezierTypes, const OdGiFaceData* pFaceData = 0);

  /** \details
    Utility function to be called from client implementation of shellProc()
    to subdivide shell onto set of smaller shell primitives by vertexes number limit.
    
    \param numVertices [in]  Number of vertices.
    \param vertexList [in]  Pointer to an array of vertices.
    \param faceListSize [in]  Number of faces.
    \param faceList [in]  Pointer to an array of faces.
    \param pEdgeData [in]  Additional edge data.
    \param pFaceData [in]  Additional face data.
    \param pVertexData [in]  Additional vertex data.
    \param nLimit [in]  Vertex limit.
  */
  void subdivideShellByVertexLimit(OdInt32 numVertices, const OdGePoint3d* vertexList,
    OdInt32 faceListSize, const OdInt32* faceList,
    const OdGiEdgeData* pEdgeData, const OdGiFaceData* pFaceData,
    const OdGiVertexData* pVertexData, OdUInt32 nLimit);


  /** \details
    Applies given fill pattern to the shell.
    Returns true if pattern was applied succesfully and false 
    if pattern is too dense and solid fill is required.
    
    \param pHatch [in]  Smart pointer to hatch pattern to be applied.
    \param fillDensity [in]  Fill density.
    \param pVertexList [in]  Pointer to a vertex list.
    \param faceListSize [in]  Number of faces.
    \param pFaceList [in]  Pointer to a face list.
    \param pFaceData [in]  Additional face data.
    \param pMapper [in]  Pointer to a material mapper.
  */
  virtual bool generateShellFill( OdGiHatchPatternPtr pHatch,
                                  OdDouble& fillDensity,
                                  const OdGePoint3d* pVertexList,
                                  OdInt32 faceListSize,
                                  const OdInt32* pFaceList,
                                  const OdGiFaceData* pFaceData = 0,
                                  OdGiMapperItemEntry* pMapper = 0
                                  );

protected:
  // Pline primitive generation helpers
  bool jointLineWithArc(const OdGiPolyline& lwBuf, OdInt32 LineSegNo, OdInt32  ArcSegNo, OdGePoint3dArray& points);
  bool jointLineWithLine(const OdGiPolyline& lwBuf, OdInt32 LineSegNo1, OdInt32 LineSegNo2, OdGePoint3dArray& points);
  bool jointArcWithLine(const OdGiPolyline& lwBuf, OdGeCircArc2d& arc, OdInt32 arcSegNo, OdInt32 lineSegNo,
                        OdGePoint3d& point1, OdGePoint3d& point2);
  // Mesh primitive generation helpers
  bool isMeshRequireVertexNormals(const OdGiVertexData *pVertexData);
  void generateMeshVertexNormals(OdInt32 numRows, OdInt32 numCols, const OdGePoint3d *pVertexList, const OdGiVertexData *pVertexData,
                                 OdGiVertexData &vertexData, OdGeVector3dArray &normals);
  // Indexed polylineOut helper
  inline void polylineOut(OdInt32 numPoints, const OdInt32* vertexIndexList, const OdGiEdgeData* pEdgeData, OdInt32 nEdgeIndex)
  {
    if (pEdgeData && pEdgeData->visibility())
    { const OdUInt8 edgeVisibility = pEdgeData->visibility()[nEdgeIndex];
      SETBIT(m_simplFlags, kSimplEdgeVisible,    edgeVisibility == kOdGiVisible);
      SETBIT(m_simplFlags, kSimplEdgeSilhouette, edgeVisibility == kOdGiSilhouette);
    }
    polylineOut(numPoints, vertexIndexList);
    // Restore default flags state
    SETBIT_0(m_simplFlags, kSimplEdgeSilhouette);
    SETBIT_1(m_simplFlags, kSimplEdgeVisible);
  }
  // Block of internal optimizations
  OdGeLineSeg2d *m_pLineSeg2d; OdGeLineSeg2d &tmpLineSeg2d();
  OdGeLineSeg2d *m_pLineSeg2dEx[2]; OdGeLineSeg2d &tmpLineSeg2dEx(int n);
  OdGeLine2d *m_pLine2dEx[4]; OdGeLine2d &tmpLine2dEx(int n);
  OdGeCircArc2d *m_pCircArc2d; OdGeCircArc2d &tmpCircArc2d();
  OdGeCircArc3d *m_pCircArc3d; OdGeCircArc3d &tmpCircArc3d();
  OdGeEllipArc3d *m_pEllipArc3d; OdGeEllipArc3d &tmpEllipArc3d();
  OdGePoint3dArray &tmpPoints3d(int nLevel) { return (!nLevel) ? m_points3d1 : m_points3d2; }
};

/** \details

    \sa
    TD_Gi 
    <group OdGi_Classes> 
*/
struct ODGI_EXPORT OdGiFillData
{
  /** \details
    Constructor for the OdGiFillData structure. 
    
    \param pDrawCtx [in]  Pointer to the draw context.
    \param pTraits [in]  Sub-entity traits.
    \param lweight [in]  Lineweight type.
    \param fillType [in]  Fill type.
  */
  OdGiFillData(OdGiConveyorContext* pDrawCtx, OdGiSubEntityTraits* pTraits,
    OdDb::LineWeight lweight = OdDb::kLnWt000,
    OdGiFillType fillType = kOdGiFillNever);
  
  /** \details
    Specifies various parameters for this fill data object. 
    
    \param lweight [in]  Lineweight type.
    \param fillType [in]  Fill type.
    \param fillNormal [in]  Pointer to a fill normal.
  */
  void set(OdDb::LineWeight lweight, OdGiFillType fillType, const OdGeVector3d* fillNormal);
  
  /** \details
    Destructor for the OdGiFillData structure.
  */
  ~OdGiFillData();

  OdDb::LineWeight m_lweight;
  OdGiFillType m_fillType;
  OdGeVector3d m_fillNormal;
  OdGeVector3d* m_pFillNormal;
  OdGiSubEntityTraits* m_pTraits;
  OdGiConveyorContext* m_pDrawCtx;
};

#include "TD_PackPop.h"

#endif //#ifndef __ODGIGEOMETRYSIMPLIFIER_H__
