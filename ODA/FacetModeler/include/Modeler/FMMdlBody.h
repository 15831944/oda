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

#ifndef __FMMDL_BODY_H__
#define __FMMDL_BODY_H__

#include "Modeler/FMMdlBase.h"
#include "FMContour3D.h"
#include "FMProfile3D.h"

class OdGeCircArc3d;
namespace GeMesh { class OdGeTrMesh; }

namespace FacetModeler
{
  /** \details
     Represents facet body objects.
  */
  class FMGEOMETRY_API Body
  {
  public:
    /** \details
       Default constructor.
    */
    Body();

    /** \details
       Assignment constructor.

       \param B [in] Body to assign.

       \remarks
       Doesn't create a copy of the body, only copies references.
    */
    Body(const Body& B);

    /** \details
       Assignment operator.

       \param B [in] Body to assign.
       \returns Reference to this body.

       \remarks
       Doesn't create a copy of the body, only copies the implementation (BodyImpl) pointer.
    */
    Body& operator = (const Body& B);

    /** \details
       Checks if the body is equal to the specified body.
      
       \param aBody [in] Body to compare with.
       \returns false if at least one vertex does not match with the other body, or true otherwise.
    */
    bool operator == (const Body& aBody) const;

    /** \details
       Checks if the body is not equal to the specified body.
      
       \param aBody [in] Body to compare with.
       \returns true if at least one vertex does not match with the other body, or false otherwise.
    */
    bool operator != (const Body& aBody) const;

    /** \details
       Clears the body.
    */
    void clear();

    /** \details
       Creates a copy of the body.
      
       \returns Copy of the Body object.
    */
    Body clone() const;

    /** \details
       Combines this body and operand into a single body.
      
       \param rAnother [in] Second body to combine with.
       \returns Resulting combined body.

       \remarks
       Clears this body and operand.
    */
    Body combine(Body& rAnother);

    /** \details
       Decomposes the body into one or more lump bodies.
      
       \param aLumps [out] Array of lump bodies.

       \remarks
       A “lump” is a body with a single continuous 3D volume.
       A lump has one outer shell and, possibly, several inner shells that represent cavities.
       Clears this body.
    */
    void decomposeIntoLumps(std::vector<Body>& aLumps);

    /** \details
       Transforms the body by the specified matrix.
      
       \param mMatrix [in] Transformation matrix as OdGeMatrix3d.
    */
    void transform(const OdGeMatrix3d& mMatrix);

    /** \details
       Inverts the body.
    */
    void negate();

    /** \details
       Regenerates pair edges.
    */
    void regeneratePairEdges();

    /** \details
       Deletes unused vertices from the body.
    */
    void deleteUnusedVertices();

    /** \details
       Merges coincident vertices.
      
       \returns true if at least two coincident vertices are merged.
    */
    bool mergeCoincidentVertices();


    /** \details
       Merges coplanar faces and collinear edges.
    */
    void mergeCoplanarEntities();

    /** \details
       Checks whether the body has non-manifold edges.
      
       \returns true if at least one non-manifold edge is found, or false otherwise.
    */
    bool containsNonManifoldEdges() const;

    /** \details
       Checks the body for validity.
      
       \param pFirstErrorDetected [out] (Optional) Error result string.
       \returns true if the body is valid, or false otherwise.

       \remarks
       If an error is detected and pFirstErrorDetected is provided, the method 
       returns false and assigns an error message to the passed OdString; 
       otherwise OdError with the error message is thrown.
    */
    bool checkInternalConsistence(OdString* pFirstErrorDetected = NULL) const;

    /** \details
       Checks whether the body has coincident edges.

       \returns true if the face has at least two coincident edges, or false otherwise.
    */
    bool hasCoincidentEdges() const;

    /** \details
       Checks whether the body has coincident vertices.
      
       \returns true if the face has at least two coincident vertices, or false otherwise.
    */
    bool hasCoincidentVertices() const;

    /** \details
       Checks whether the body is empty.
      
       \returns false if the body has at least one face, or true otherwise.
    */
    bool isNull() const;

    /** \details
       Checks whether the body is closed (any face is connected to another one at any edge).
      
       \returns true if the body is closed, or false otherwise.
    */
    bool isClosed() const;

    /** \details
       Splits a single face with a plane while preserving topology.
      
       \param pFace     [in] Face to split.
       \param pPlane    [in] Splitting plane.
       \param pNewOuter [in] Resulting outer faces.
       \param pNewInner [in] Resulting inner faces.
    */
    void splitFace(Face* pFace,
      const OdGePlane& pPlane,
      std::vector<Face*>* pNewOuter = NULL,
      std::vector<Face*>* pNewInner = NULL);

    ////////////////////////////////////////////////////////////////
    // Tags and flags
    ////////////////////////////////////////////////////////////////

    /** \details
       Resets vertex tags.

       \param clearValue [in] New value to set.
    */
    void clearVertexTags(TagType clearValue = 0);

    /** \details
       Resets body tags.

       \param clearValue [in] New value to set.
    */
    void clearSurfaceTags(TagType clearValue = 0);

    /** \details
       Resets edge tags.

       \param clearValue [in] New value to set.
    */
    void clearEdgeTags(TagType clearValue = 0);

    /** \details
       Resets face tags.

       \param clearValue [in] New value to set.
    */
    void clearFaceTags(TagType clearValue = 0);

    /** \details
       Resets face flags.

       \param clearValue [in] New value to set.
    */
    void clearFaceFlags(OdUInt32 clearValue = 0);

    /** \details
       Resets body tags.

       \param clearValue [in] New value to set.
    */
    inline void clearBodyTags(TagType clearValue = 0) { setTag(clearValue); }

    /** \details
       Gets the first tag.
      
       \returns Tag value.
    */
    TagType tag() const;

    /** \details
       Sets the first tag.
      
       \param iTag [in] New tag value.
    */
    void setTag(TagType iTag);

    ////////////////////////////////////////////////////////////////
    // Geometric properties
    ////////////////////////////////////////////////////////////////

    /** \details
       Gets geometric extents.
      
       \returns Calculated geometric extents as OdGeExtents3d.
    */
    OdGeExtents3d interval() const;

    /** \details
       Calculates volume.
      
       \returns Volume value.
    */
    double volume() const;

    ////////////////////////////////////////////////////////////////
    // Intersections
    ////////////////////////////////////////////////////////////////

    /** \details
       Gets a slice profile created by the specified plane.
      
       \param rCutter          [in] Cutting plane to slice the body.
       \param rResult         [out] Resulting profile.
       \param bIncludeBoundary [in] true to include bounds in the resulting profile, false to exclude.
    */
    void slice(const OdGePlane& rCutter,
      Profile3D& rResult, bool bIncludeBoundary = true) const;

    /** \details
       Gets a slice half-body created by the specified plane.

       \param rCutter          [in] Cutting plane to slice the body.
       \param result          [out] Resulting body.
    */
    void slice(const OdGePlane& rCutter, Body& result, TagType tagSection = 0) const;

    /** \details
       Intersects this body with the given line.
      
       \param gLine        [in] Line to intersect with.
       \param aPoints     [out] Intersection points array.
       \param bSortResults [in] true to sort points behind their parameters on the line gLine.
       \returns true if there are intersections, or false otherwise.

       \remarks
       There can be multiple equal intersections.
    */
    bool intersectLine(const OdGeLinearEnt3d& gLine, 
      std::vector< OdGePoint3d >& aPoints, bool bSortResults = false) const;

    ////////////////////////////////////////////////////////////////
    // Primitives
    ////////////////////////////////////////////////////////////////

    /** \details
       Creates a box.

       \param ptOrigin [in] Origin.
       \param vSizes   [in] Three box sizes.

       \returns Resulting body.
    */
    static Body box(const OdGePoint3d& ptOrigin,
      const OdGeVector3d& vSizes);

    /** \details
       Creates a pyramid.

       \param rBase     [in] Base profile (on XY plane).
       \param ptApex    [in] Pyramid apex.
       \param deviation [in] Deviation parameters.

       \returns Resulting body.
    */
    static Body pyramid(const Profile2D& rBase,
      const OdGePoint3d& ptApex,
      const DeviationParams& deviation = FMGeGbl::gDefDev);

    /** \details
       Creates an extrusion.

       \param rBase     [in] Base profile (on XY plane).
       \param vDir      [in] Extrusion direction.
       \param deviation [in] Deviation parameters.

       \returns Resulting body.
    */
    static Body extrusion(const Profile2D& rBase,
      const OdGeVector3d& vDir,
      const DeviationParams& deviation = FMGeGbl::gDefDev);

    /** \details
       Creates an extrusion of a 2D profile along the vDir vector.
      
       \param rBase      [in] Base profile (on XY plane).
       \param mBasePlane [in] Initial 3D plane of the profile as OdGeMatrix3d.
       \param vDir       [in] Extrusion direction and length.
       \param deviation  [in] Deviation parameters.
       \returns Resulting body.

       \remarks
       The profile is placed into the 3D plane defined by the mBasePlane matrix 
       and then extruded along the vDir path of the extrusion. 
       vDir must be non-perpendicular to the mProfilePlane z-axis.
    */
    static Body extrusion(const Profile2D& rBase,
      const OdGeMatrix3d& mBasePlane,
      const OdGeVector3d& vDir,
      const DeviationParams& deviation = FMGeGbl::gDefDev);

    /** \details
       Creates a revolution body using a base profile, radius, height and deviation parameters.
      
       \param rBase     [in] Base profile (X - along height, Y - along radius).
       \param dRadius   [in] Radius.
       \param dHeight   [in] Height.
       \param deviation [in] Deviation parameters.
       \returns Resulting body.
    */
    static Body revolution(const Profile2D& rBase,
      double dRadius, double dHeight,
      const DeviationParams& deviation = FMGeGbl::gDefDev);

    /** \details
       Creates a revolution body using a base profile, circle arc with revolution axis and angles, and deviation parameters.
      
       \param base             [in] Base profile (on XY plane).
       \param revAxisAndAngles [in] Circle arc with specified revolution axis as normal and revolution angles.
       \param deviation        [in] Deviation parameters.
       \param pBaseTransform   [in] (Optional) Transform to be applied to the base profile while being revolved.
       \returns Resulting body.
    */
    static Body revolution(
      const Profile2D& base,
      const OdGeCircArc3d& revAxisAndAngles,
      const DeviationParams& deviation = FMGeGbl::gDefDev,
      const OdGeMatrix2d* pBaseTransform = NULL);

    /** \details
      Creates a revolution body using a base profile, revolution axis, position of the revolution axis, 
      start and end angles of revolution, and deviation parameters. 

      \param base         [in] Base profile (on XY plane).
      \param revAxis      [in] Revolution axis.
      \param axisPosition [in] Position of the revolution axis.
      \param startAng     [in] Start angle of revolution.
      \param endAng       [in] End angle of revolution.
      \param deviation    [in] Deviation parameters.

      \returns Resulting body.

      \remarks
      If base profile is not closed, this makes boolean operations with the resulting body impossible.
  */
    static Body revolution(
      const Profile2D& base,
      const OdGeVector3d& revAxis,
      const OdGePoint3d& axisPosition,
      double startAng,
      double endAng,
      const DeviationParams& deviation = FMGeGbl::gDefDev);

    /** \details
       Performs a boolean operation destroying both operands.

       \param eOperation    [in] Boolean operation type.
       \param rOperandA     [in] Operand A.
       \param rOperandB     [in] Operand B.
       \param bOptimization [in] Optimization flag

       \returns Resulting body.
    */
    static Body boolOper(BooleanOperation eOperation,
      Body& rOperandA, Body& rOperandB, bool bOptimization = false);

    /** \details
       Creates a custom body from the specified definition arrays.

       \param aVertices    [in] Array of vertex points.
       \param aFaceData    [in] Array of integers describing the face definition based on the input vertex array.
       \param aEdgeFlags   [in] Edge flags array.
       \param aFaceFlags   [in] Face flags array.
       \param aVertexFlags [in] Vertex flags array.
       \param pFaceColors  [in] Colors array of the faces.
       \param pEdgeColors  [in] Colors array of the edges.
       \param pMappingCoords  [in] The mapping coordinates array for every face.

       \returns Resulting body.

       \remarks
       Only suitable for good input topology (without t-junctions).

       The aFaceData is an array of faces, and each face is represented in it
       by a sequence of numbers. The first number in the sequence
       determines the number of vertices for the face and must be greater than or equal to 3.
       The following numbers define the indices of vertices in aVertices.
    */
    static Body createFromMesh(
      const std::vector<OdGePoint3d>& aVertices,
      const std::vector<OdInt32>& aFaceData,
      const std::vector<OdUInt32>* aFaceFlags = NULL,
      const std::vector<OdUInt32>* aEdgeFlags = NULL,
      const std::vector<OdUInt32>* aVertexFlags = NULL,
      const std::vector<OdUInt32>* pFaceColors = NULL,
      const std::vector<OdUInt32>* pEdgeColors = NULL,
      const std::vector<OdGePoint2d>* pMappingCoords = NULL
    );

    /** \details
       Creates triangles mesh representing this body.

       \param mesh [out] The generated triangles mesh.
    */
    void generateMesh(GeMesh::OdGeTrMesh& mesh) const;

    ////////////////////////////////////////////////////////////////
    // Working with topological entities
    ////////////////////////////////////////////////////////////////

    /** \details
       Sets face and edge colors to the specified value.

       \param iColor [in] New color value as OdUInt32.
    */
    void setColor(OdUInt32 iColor);

    /** \details
       Adds a face.

       \param pFace [in] Face to add.

       \remarks
       For internal use only.
    */
    void addFace(Face* pFace);

    /** \details
       Removes a face.

       \param pFace [in] Face to remove.

       \remarks
       For internal use only.
    */
    void removeFace(Face* pFace);

    /** \details
       Removes and deletes a face.

       \param pFace [in] Face to delete.

       \remarks
       For internal use only.
    */
    void deleteFace(Face* pFace);

    /** \details
       Adds the specified surface to the body.

       \param pSurface [in] Surface to add.
    */
    void addSurface(Surface* pSurface);

    /** \details
       Adds the specified vertex to the body.

       \param pVertex [in] Vertex to add.
    */
    void addVertex(Vertex* pVertex);

    /** \details
       Adds a vertex.

       \param pt [in] Vertex coordinates as OdGePoint3d.
       \returns Pointer to the newly created vertex.
    */
    Vertex* addVertex(const OdGePoint3d& pt);

    /** \details
       Adds a face.

       \param pSurface [in] Surface to assign.
       \returns Pointer to the newly created face.
    */
    Face* addFace(Surface* pSurface = NULL);

    /** \details
       Gets a vertex count.

       \returns Count of vertices as OdUInt32.
    */
    OdUInt32 vertexCount() const;

    /** \details
       Counts and returns the number of edges.

       \returns Edge count as OdUInt32.
    */
    OdUInt32 countEdges() const;

    /** \details
       Gets the cached edge count.

       \returns Cached edge count.

       \remarks
       The returned edge count is what was calculated in the most recent countEdges() call.
    */
    OdUInt32 cachedEdgesCount() const;

    /** \details
       Gets a faces count.

       \returns Count of faces as OdUInt32.
    */
    OdUInt32 faceCount() const;

    /** \details
       Gets a surface count.

       \returns Count of surfaces as OdUInt32.
    */
    OdUInt32 surfaceCount() const;

    /** \details
       Gets a face list.

       \returns Face list pointer.
    */
    Face* faceList() const;

    /** \details
       Gets a vertex list.

       \returns Vertex list pointer.
    */
    Vertex* vertexList() const;

    /** \details
       Gets a surface list.

       \returns Surface list pointer.
    */
    Surface* surfaceList() const;

    /** \details
       Sets the pointer to the first vertex of the body.

       \param pList [in] Vertex list pointer.
    */
    void setVertexList(Vertex* pList);

    /** \details
       Sets the pointer to the first face of the body.

       \param pList [in] Face list pointer.
    */
    void setFaceList(Face* pList);

    /** \details
       Sets the pointer to the first surface of the body.

       \param pList [in] Surface list pointer.
    */
    void setSurfaceList(Surface* pList);

    /** \details
       Deletes surfaces that are not set to any face

    */
    void deleteUnusedSurfaces();

    /** \details
       Sets the count of vertices to the specified number.

       \param count [in] New vertex count.

       \remarks
       Use with setVertexList.
    */
    void setVertexCount(OdUInt32 count);

    /** \details
       Sets the count of faces to the specified number.

       \param count [in] New face count.

       \remarks
       Use with setFaceList.
    */
    void setFaceCount(OdUInt32 count);

    /** \details
       Sets the count of surfaces to the specified number.

       \param count [in] New surface count.

       \remarks
       Use with setVertexList.
    */
    void setSurfaceCount(OdUInt32 count);

    /** Empty body object. */
    FMGEOMETRY_API_STATIC static const Body kEmpty;

    /** \details
      Destructor.
    */
    ~Body();

    /** \details
      Reduces the number of primitives (vertices and faces).

      \param param [in] Simplification percent if positive, or tolerance if negative.

      \remarks
      All the faces will be converted to triangles first. The result will contain the number of triangles
      according to given percent (for example, 10% of initial number). If param is negative, then the
      resulting body will contain the minimum number of triangles to match the given max deviation 
      from initial mesh.
    */
    void simplify(double param);

  protected:
    /** \details 
      Creates Body object with specified implementation object pointer.
    
      \param pImpl [in] Implementation object pointer.
    */
    explicit Body(class BodyImpl* pImpl);

  private:

    BodyImpl* m_pImpl; // Implementation object pointer.

    //DOM-IGNORE-BEGIN
    friend class Vertex;
    friend class Face;
    friend class Surface;
    friend class BodyImpl;
    friend class BodyCustom;
    friend class BodyInternalConsistencyRestorer;
    //DOM-IGNORE-END
  };

  /** \details
    Creates shaded surfaces for the given Body.

    \param aBody [inout] Body to process.
    \param angleTol [in] The angle to detect the crease between faces.
    \param bDetectSharpEdges [in] The flag to perform additional sharp edges detection for better quality.

    \remarks
    Shaded surfaces allow to calculate vertex normals for shaded rendering of the Body. This function
    finds 'smooth' areas of the Body and marks them with the common shaded surface.
  */
  FMGEOMETRY_API void MarkShadedSurfaces(Body& aBody, double angleTol, bool bDetectSharpEdges = false);

  /** \details
    Deletes all the shaded surfaces for the given Body.

    \param aBody [inout] Body to process.
  */
  FMGEOMETRY_API void ClearShadedSurfaces(Body& aBody);

  /** \details
    Merges together different shaded surfaces of the given Body. Replaces surfaces from the set with the first surface.

    \param aBody [inout] Body to process.
    \param shadedMergeData [in] The set of surfaces to be merged.
  */
  void MergeShadedSurfaces(Body& aBody, const OdArray<std::set<Surface*> >& shadedMergeData);

}

#endif //__FMMDL_BODY_H__
