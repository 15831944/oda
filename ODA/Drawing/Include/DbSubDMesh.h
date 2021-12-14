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




#ifndef _OD_DB_SUBDMESH
#define _OD_DB_SUBDMESH

#include "TD_PackPush.h"

#include "DbEntity.h"
#include "DoubleArray.h"
#include "DbSurface.h"
#include "Db3dSolid.h"
#include "CmEntityColorArray.h"

#include "Int32Array.h"
#include "Gi/GiGeometry.h"

class OdDbSurface;
class OdDb3dSolid;

/** \details
  This class represents SubDMesh entities in an OdDbDatabase instance.

  
  <group OdDb_Classes>
*/
class DBENT_EXPORT OdDbSubDMesh : public OdDbEntity
{
public:
  //DOM-IGNORE-BEGIN
  ODDB_DECLARE_MEMBERS(OdDbSubDMesh);
  //DOM-IGNORE-END

  /** \details
    Default constructor.
  */
  OdDbSubDMesh();

  /** \details
    Draws OdDbSubDMesh to the specified OdGiWorldDraw object.

    \param pWd [in] Pointer to the OdGiWorldDraw object.
    \returns True if the entity has no view-dependent graphics, false otherwise.
  */
  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  virtual OdDbObjectPtr decomposeForSave(OdDb::DwgVersion ver,
    OdDbObjectId& replaceId,
    bool& exchangeXData);

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  /** \details
    Applies the specified 3D transformation matrix to this entity.

    \param xfm [in] 3D transformation matrix.

    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm);

  /** \details
    Calculates geometrical extents of this object by vertices coordinates.

    \param extents [out] Output extents.
    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult subGetGeomExtents(
    OdGeExtents3d& extents) const;

  /** \details
    Explodes this entity into a set of simpler entities.

    \param entitySet [out] Array of pointers to the new entities.

    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;

  /** \details
    Creates a mesh using input arrays of vertices and faces.

    \param vertexArray [in] Array of vertices.
    \param faceArray   [in] Array of integers, describing faces.
    \param subDLevel   [in] Level of smoothness.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
    \remarks
    Face array is arranged using the following pattern: the first number is the
    number a vertices a face has, the following numbers are indices of vertices,
    that form a face.
  */
  OdResult setSubDMesh (const OdGePoint3dArray& vertexArray,
    const OdInt32Array& faceArray,
    OdInt32 subDLevel);

  /** \details
    Creates a mesh of a sphere.

    \param radius    [in] Sphere radius.
    \param divAxis   [in] Specifies how many divisions will be made for the base circle.
    \param divHeight [in] Specifies how many divisions will be made in the direction of the height of a sphere.
    \param subDLevel [in] Level of smoothness.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setSphere (double radius,
    OdInt32 divAxis,
    OdInt32 divHeight,
    OdInt32 subDLevel);

  /** \details
    Creates a mesh of a cylinder.

    \param majorRadius [in] Major radius.
    \param minorRadius [in] Minor radius.
    \param height      [in] Height of a cylinder.
    \param divAxis     [in] Specifies how many divisions will be made for the base circle.
    \param divHeight   [in] Specifies how many divisions will be made in the direction of the height of a cylinder.
    \param divCap      [in] Specifies how many radial divisions are made for the base circle.
    \param subDLevel   [in] Level of smoothness.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setCylinder (double majorRadius,
    double minorRadius,
    double height,
    OdInt32 divAxis,
    OdInt32 divHeight,
    OdInt32 divCap,
    OdInt32 subDLevel);

  /** \details
    Creates a mesh of a cone.

    \param majorRadius [in] Major radius.
    \param minorRadius [in] Minor radius.
    \param height      [in] Height of a cone.
    \param divAxis     [in] Specifies how many divisions will be made for the base circle.
    \param divHeight   [in] Specifies how many divisions will be made in the direction of the height of a cone.
    \param divCap      [in] Specifies how many radial divisions will be made for the base circle.
    \param radiusRatio [in] Ratio of top to bottom radii.
    \param subDLevel   [in] Level of smoothness.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setCone (double majorRadius,
    double minorRadius,
    double height,
    OdInt32 divAxis,
    OdInt32 divHeight,
    OdInt32 divCap,
    double radiusRatio,
    OdInt32 subDLevel);

  /** \details
    Creates a mesh of a torus.

    \param majorRadius        [in] Major radius of a torus.
    \param divSection         [in] Specifies how many divisions will be made for the base circle.
    \param divSweepPath       [in] Specifies how many divisions will be made along the torus' path.
    \param sectionRadiusRatio [in] Ratio of major to minor radii.
    \param sectionRotate      [in] Angle of section rotation.
    \param subDLevel          [in] Level of smoothness.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setTorus (double majorRadius,
    OdInt32 divSection,
    OdInt32 divSweepPath,
    double sectionRadiusRatio,
    double sectionRotate,
    OdInt32 subDLevel);

  /** \details
    Creates a mesh of a box.

    \param xLen      [in] Box depth.
    \param yLen      [in] Box width.
    \param zLen      [in] Box height.
    \param divX      [in] Specifies how many divisions will be made along the depth direction.
    \param divY      [in] Specifies how many divisions will be made along the width direction.
    \param divZ      [in] Specifies how many divisions will be made along the height direction.
    \param subDLevel [in] Level of smoothness.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setBox (double xLen,
    double yLen,
    double zLen,
    OdInt32 divX,
    OdInt32 divY,
    OdInt32 divZ,
    OdInt32 subDLevel);

  /** \details
    Creates a mesh of a wedge.

    \param xLen      [in] Wedge depth.
    \param yLen      [in] Wedge width.
    \param zLen      [in] Wedge height.
    \param divLength [in] Specifies how many divisions will be made along the depth direction.
    \param divWidth  [in] Specifies how many divisions will be made along the width direction.
    \param divHeight [in] Specifies how many divisions will be made along the height direction.
    \param divSlope  [in] Specifies how many divisions will be made along the slope direction.
    \param divCap    [in] Specifies how many radial divisions will be made for the base face.
    \param subDLevel [in] Level of smoothness.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setWedge (double xLen,
    double yLen,
    double zLen,
    OdInt32 divLength,
    OdInt32 divWidth,
    OdInt32 divHeight,
    OdInt32 divSlope,
    OdInt32 divCap,
    OdInt32 subDLevel);

  /** \details
    Creates a mesh of a pyramid.

    \param radius      [in] Radius of a circle inscribed in a pyramid.
    \param height      [in] Pyramid height.
    \param divLength   [in] Specifies how many divisions will be made along the length direction.
    \param divHeight   [in] Specifies how many divisions will be made along the height direction.
    \param divCap      [in] Specifies how many radial divisions will be made for the base face.
    \param nSides      [in] Number of sides that pyramid will have.
    \param radiusRatio [in] Ratio of major to minor radii.
    \param subDLevel   [in] Level of smoothness.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setPyramid (double radius,
    double height,
    OdInt32 divLength,
    OdInt32 divHeight,
    OdInt32 divCap,
    OdInt32 nSides,
    double radiusRatio,
    OdInt32 subDLevel);

  /** \details
    Calculates an array of indices of all faces of current SubDMash entity that
    are intersected by a specified ray. Also the method returns two additional
    arrays containing the intersection points for each face and distances
    between the start point of the ray and intersection points.

    \param rayStart           [in] Start point of the ray.
    \param rayDir             [in] Ray direction.
    \param retSubents        [out] Array of indices of faces intersected by the ray.
    \param retIntersectDist  [out] Array of distances between the start point of
                                   the ray and the intersection point for
                                   intersected subentities.
    \param retIntersectPoint [out] The array of intersection points for
                                   intersected subentities.
    \returns A value of the OdResult type containing the result of the method
    execution.
  */
  OdResult computeRayIntersection(const OdGePoint3d &rayStart,
    const OdGeVector3d &rayDir,
    OdArray<OdDbSubentId> &retSubents,
    OdArray<double> &retIntersectDist,
    OdGePoint3dArray& retIntersectPoint) const;

  /** \details
    Increases the smoothness level by one.

    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult subdDivideUp ();

  /** \details
    Decreases the smoothness level by one.

    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult subdDivideDown ();

  /** \details
    Applies the current level of smoothness to an array of vertices and faces.
    The level of smoothing will be set to 0.

    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult subdRefine ();

  /** \details
    Applies the current level of smoothness to an array of vertices and faces.
    The level of smoothing will be set to 0.

    \param subentPaths [in] Array of subentities.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
    \remarks
    Currently not implemented.
  */
  OdResult subdRefine (const OdDbFullSubentPathArray& subentPaths);

  /** \details
    Retrieves the current level of smoothness.

    \param result [out] Current level of smoothness.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult subdLevel (OdInt32& result) const;

  /** \details
    Cuts a face in two using two points on different edges of the original face.

    \param subentFaceId [in] ID of the face that will be cut.
    \param subent0      [in] First edge.
    \param point0       [in] Point on the first edge.
    \param subent1      [in] Second edge.
    \param point1       [in] Point on the second edge.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult splitFace (const OdDbSubentId& subentFaceId,
    const OdDbSubentId& subent0,
    const OdGePoint3d& point0,
    const OdDbSubentId& subent1,
    const OdGePoint3d& point1);

  /** \details
    Extrudes faces along the given direction and with a given taper angle.

    \param subentPaths [in] Array of faces to be extruded.
    \param length      [in] Length of extrusion.
    \param dir         [in] Direction of extrusion.
    \param taper       [in] Taper angle of extrusion.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult extrudeFaces (const OdDbFullSubentPathArray& subentPaths,
    double length,
    const OdGeVector3d& dir,
    double taper);

  /** \details
    Extrudes faces along the given direction and with a given taper angle.

    \param subentPaths [in] Array of faces to be extruded.
    \param alongPath   [in] Array of points, which describes the path, along which the faces will be extruded.
    \param taper       [in] Taper angle of extrusion.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult extrudeFaces (const OdDbFullSubentPathArray& subentPaths,
    const OdGePoint3dArray& alongPath,
    double taper);

  /** \details
    Extrudes connected faces along the given direction and with a given taper
    angle.

    \param subentPaths [in] Array of faces to be extruded.
    \param length      [in] Length of extrusion.
    \param dir         [in] Direction of extrusion.
    \param taper       [in] Taper angle of extrusion.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult extrudeConnectedFaces(const OdDbFullSubentPathArray& subentPaths,
    double length,
    const OdGeVector3d& dir,
    double taper);

  /** \details
    Inverts the vertex order of each face.

    \returns eOk if the mesh is successfully negated; corresponding error code
    otherwise.
  */
  OdResult negate();

  /** \details
    Checks whether a mesh is watertight.

    \param result [out] Resulting value. True if the mesh is watertight, false otherwise.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult isWatertight (bool& result) const;

  /** \details
    Retrieves the number of faces for the base level.

    \param result [out] Number of faces for the base level. 0 if the mesh hasn't been initialized yet.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult numOfFaces (OdInt32& result) const;

  /** \details
    Retrieves the number of faces with the current level of smoothness.

    \param result [out] Output number of smooth faces.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult numOfSubDividedFaces (OdInt32& result) const;

  /** \details
    Retrieves the number of smooth faces for given base mesh faces.

    \param subentPaths [out] Array of faces.
    \param result      [out] Output number of faces.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult numOfSubDividedFacesAt (const OdDbFullSubentPathArray& subentPaths, OdInt32& result) const;

  /** \details
    Retrieves the number of vertices for base level.

    \param result [out] Number of vertices for the base level.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult numOfVertices (OdInt32& result) const;

  /** \details
    Retrieves the number of vertices with the current level of smoothness.

    \param result [out] Output number of vertices.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult numOfSubDividedVertices (OdInt32& result) const;

  /** \details
    Retrieves the number of edges for the base level.

    \param result [out] Number of edges for the base level. 0 if the mesh hasn't been initialized yet.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult numOfEdges (OdInt32& result) const;

  /** \details
    Retrieves the base level vertices.

    \param vertexArray [out] Output array of the base level vertices.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getVertices (OdGePoint3dArray& vertexArray) const;

  /** \details
    Retrieves the base level edges.

    \param edgeArray [out] Output array of the base level edges.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getEdgeArray (OdInt32Array& edgeArray) const;

  /** \details
    Retrieves the base level faces.

    \param faceArray [out] Output array of the base level faces.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getFaceArray (OdInt32Array& faceArray) const;

  /** \details
    Calculates normals for all vertices of the current SubDMash entity.

    \param normalArray [out] Array of normals.
    \returns A value of the OdResult type containing the result of the method
    execution.
    \remarks
    The normal for a vertex is calculated as the average of all normals for the
    faces that include the vertex.
    This method assumes that the smoothness level is zero.
  */
  OdResult getNormalArray (OdGeVector3dArray& normalArray) const;

  /** \details
    Retrieves the vertices for a smoothened mesh.

    \param vertexArray [out] Output array of the vertices.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getSubDividedVertices (OdGePoint3dArray& vertexArray) const;

  /** \details
    Retrieves the faces for a smoothened mesh.

    \param faceArray [out] Output array of the faces.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getSubDividedFaceArray (OdInt32Array& faceArray) const;

  /** \details
    Calculates normals for all vertices of the current SubDMash entity taking
    into account the level of smoothing.

    \param normalArray [out] Array of normals.
    \returns A value of the OdResult type containing the result of the method
    execution.
    \remarks
    The normal for a vertex is calculated as the average of all normals for the
    faces that include the vertex.
    This method takes into account the level of smoothness, which can take any
    value from 0 to 4.
  */
  OdResult getSubDividedNormalArray(OdGeVector3dArray& normalArray) const;

  /** \details
    Retrieves a vertex from a specified position on a mesh.

    \param nIndex  [in] Index of the vertex.
    \param vertex [out] Output vertex.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getVertexAt (OdInt32 nIndex, OdGePoint3d& vertex) const;

  /** \details
    Sets a vertex using the given index.

    \param nIndex [in] Index of the vertex.
    \param vertex [in] Vertex to be set.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setVertexAt (OdInt32 nIndex, const OdGePoint3d& vertex);

  /** \details
    Retrieves a vertex using a specified subentity ID.

    \param id      [in] Subentity ID of the vertex.
    \param vertex [out] Output vertex.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getVertexAt (const OdDbSubentId& id, OdGePoint3d& vertex) const;

  /** \details
    Sets a vertex using the given subentity ID.

    \param id     [in] Subentity ID of the vertex.
    \param vertex [in] Vertex to be set.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setVertexAt (const OdDbSubentId& id, const OdGePoint3d& vertex);

  /** \details
    Retrieves a vertex of a smooth mesh using the given index.

    \param nIndex  [in] Index of the vertex.
    \param vertex [out] Output vertex.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getSubDividedVertexAt (OdInt32 nIndex, OdGePoint3d& vertex) const;

  /** \details
    Retrieves a vertex of a smooth mesh using the given subentity ID.

    \param id      [in] Subentity ID of the vertex.
    \param vertex [out] Output vertex.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getSubDividedVertexAt (const OdDbSubentId& id, OdGePoint3d& vertex) const;

  /** \details
    Sharpens the edges of a mesh.

    \param creaseVal [in] Flag, defining when to apply the crease.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
    \remarks 
    The value of the creaseVal parameter can be one of the following:
    <table>
    <b>Value</b>          <b>Description</b>
    -1                    Always apply the crease.
    0                     Never apply the crease.
    any positive value    Apply the crease starting from this level.
    </table>
  */
  OdResult setCrease (double creaseVal);

  /** \details
    Sharpens faces, edges, vertices of a mesh.

    \param subentPaths [in] Array of subentities, to which the crease will be applied.
    \param creaseVal   [in] Flag, defining when to apply the crease.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
    \remarks
    The value of the creaseVal parameter can be one of the following:
    <table>
    <b>Value</b>          <b>Description</b>
    -1                    Always apply the crease.
    0                     Never apply the crease.
    any positive value    Apply the crease starting from this level.
    </table>
  */
  OdResult setCrease (const OdDbFullSubentPathArray& subentPaths, double creaseVal);

  /** \details
    Retrieves crease values for mesh subentities.

    \param subentPaths [in] Array of mesh subentities.
    \param result     [out] Array of output crease values.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getCrease (const OdDbFullSubentPathArray& subentPaths, OdDoubleArray& result) const;

  /** \details
    Retrieves the crease value for a mesh subentity via it's ID.

    \param id      [in] Subentity ID.
    \param result [out] Output crease value.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getCrease (const OdDbSubentId& id, double& result) const;

  /** \details
    Retrieves subentities adjacent to a given one.

    \param path         [in] Original subentity.
    \param type         [in] Type of subentities to be returned.
    \param subentPaths [out] Array of subentities adjacent to the original one.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getAdjacentSubentPath (const OdDbFullSubentPath& path,
    OdDb::SubentType type,
    OdDbFullSubentPathArray& subentPaths) const;

  /** \details
    Retrieves all subentity paths by the specified index and subentity type.

    \param nIndex       [in] Subentity index. If it's equal to -1, then all subentities of the specified type will be returned as an array.
    \param type         [in] Type of the input subentity.
    \param subentPaths [out] Array of subentities.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getSubentPath (OdInt32 nIndex,
    OdDb::SubentType type,
    OdDbFullSubentPathArray& subentPaths) const;

  /** \details
    Creates a new OdDbSurface object from a part of this mesh by the specified
    sub-entity ID.

    \param bConvertAsSmooth [in] True for converting to smooth surface; false for converting to faceted surface.
    \param id               [in] Sub-entity ID to convert.
    \param pSurface        [out] Pointer to the newly created OdDbSurface object.
    \returns
    <table>
    <b>Value</b>          <b>Description</b>
    eNotImplementedYet    The method is not implemented yet for the current modeler.
    eInvalidContext       The mesh is empty.
    eInvalidInput         ID is not kFaceSubentType or the mesh has self-intersections or zero area faces.
    eOk                   The surface created successfully.
    </table>

    \remarks
    The mesh should not have self-intersections and zero area faces.
    The case when bConvertAsSmooth == true is not implemented yet.
  */
  OdResult convertToSurface (bool bConvertAsSmooth, const OdDbSubentId& id, OdDbSurfacePtr& pSurface) const;

  /** \details
    Creates a new OdDbSurface object from this mesh.

    \param bConvertAsSmooth [in] True for converting to smooth surface; false for converting to faceted surface.
    \param optimize         [in] True for converting as optimized surface; false otherwise.
    \param pSurface        [out] Pointer to the newly created OdDbSurface object.
    \returns
    <table>
    <b>Value</b>          <b>Description</b>
    eNotImplementedYet    The method is not implemented yet for the current modeler.
    eInvalidContext       The mesh is empty.
    eInvalidInput         The mesh has self-intersections or zero area faces.
    eOk                   The surface created successfully.
    </table>

    \remarks
    The mesh should not have self-intersections and zero area faces.
    The case when bConvertAsSmooth == true or optimize == true is not
    implemented yet.
  */
  OdResult convertToSurface (bool bConvertAsSmooth, bool optimize, OdDbSurfacePtr& pSurface) const;

  /** \details
    Creates a new OdDb3dSolid object from this mesh.

    \param bConvertAsSmooth [in] True for converting to smooth surface; false for converting to faceted surface.
    \param optimize         [in] True for converting as optimized surface; false otherwise.
    \param pSolid          [out] Pointer to the newly created OdDb3dSolid object.
    \returns
    <table>
    <b>Value</b>          <b>Description</b>
    eNotImplementedYet    The method is not implemented yet for the current modeler.
    eInvalidContext       The mesh is empty.
    eInvalidInput         The mesh has self-intersections or zero area faces or the mesh does not form a closed volume.
    eOk                   3dSolid created successfully.
    </table>

    \remarks
    The mesh should not have self-intersections and zero area faces and should
    form a closed volume.
    The case when bConvertAsSmooth == true or optimize == true is not
    implemented yet.
  */
  OdResult convertToSolid (bool bConvertAsSmooth, bool optimize, OdDb3dSolidPtr& pSolid) const;

  /** \details
    Retrieves the color of a given subentity.

    \param id     [in] ID of the subentity.
    \param color [out] Color of the subentity.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getSubentColor (const OdDbSubentId& id, OdCmColor& color) const;

  /** \details
    Sets the color of a given subentity.

    \param id    [in] ID of the subentity.
    \param color [in] Color to be set to the subentity.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setSubentColor (const OdDbSubentId& id, const OdCmColor& color);

  /** \details
    Retrieves the material of a given subentity.

    \param id        [in] ID of the subentity.
    \param material [out] Material of the subentity.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getSubentMaterial (const OdDbSubentId& id, OdDbObjectId& material) const;

  /** \details
    Sets the material of a given subentity.

    \param id        [in] ID of the subentity.
    \param material [out] Material to be set to the subentity.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setSubentMaterial (const OdDbSubentId& id, const OdDbObjectId& material);

  /** \details
    Retrieves the material mapper of a subentity.

    \param id      [in] ID of the subentity.
    \param mapper [out] Material mapper of the subentity.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getSubentMaterialMapper (const OdDbSubentId& id, OdGiMapper& mapper) const;

  /** \details
    Sets the material mapper of a subentity.

    \param id     [in] ID of the subentity.
    \param mapper [in] Material mapper to be set to the subentity.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setSubentMaterialMapper (const OdDbSubentId& id, const OdGiMapper& mapper);

  /** \details
    Retrieves the face plane of a given face.

    \param id         [in] ID of the face.
    \param facePlane [out] Face plane of the given face.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getFacePlane (const OdDbSubentId& id, OdGePlane& facePlane) const;

  /** \details
    Calculates the current volume of a watertight mesh.

    \param retVolume [out] Volume of the mesh.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult computeVolume (double &retVolume) const;

  /** \details
    Calculates the current surface area of a mesh.

    \param retSurfArea [out] Surface area of the mesh.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult computeSurfaceArea (double &retSurfArea) const;

  /** \details
    Retrieves the array of the normals for vertices.

    \param arrNorm [out] Output array of normals.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getVertexNormalArray(OdGeVector3dArray& arrNorm);

  /** \details
    Retrieves the array of texture coordinates.

    \param arrPts [out] Output array of texture coordinates.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getVertexTextureArray(OdGePoint3dArray& arrPts);

  /** \details
    Retrieves the array of vertex colors.

    \param arrColor [out] Output array of texture coordinates.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult getVertexColorArray(OdCmEntityColorArray& arrColor);

  /** \details
    Sets the array of vertex normals.

    \param arrNorm [in] Input array of vertex normals.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setVertexNormalArray(OdGeVector3dArray& arrNorm);

  /** \details
    Sets the array of texture coordinates.

    \param arrPts [in] Input array of texture coordinates.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setVertexTextureArray(OdGePoint3dArray& arrPts);

  /** \details
    Sets the array of vertex colors.

    \param arrColor [in] Input array of vertex colors.
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult setVertexColorArray(OdCmEntityColorArray& arrColor);

  /** \details
    Clears the array of vertex normals.

    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult clearVertexNormalArray();

  /** \details
    Clears the array of vertex textures.

    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult clearVertexTextureArray();

  /** \details
    Clears the array of vertex colors.

    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult clearVertexColorArray();

  /** \details
	Gets all subentity paths by graphic system marker.

	\param type               [in] Subentity type being queried (OdDb::kClassSubentType only).
	\param gsMarker           [in] The GS marker of the subentity being queried.
	\param pickPoint          [in] The WCS point being queried (not used).
	\param xrm                [in] The WCS->DCS transformation matrix (not used).
	\param subentPaths       [out] Array of OdDbFullSubentPath objects.
	\param pEntAndInsertStack [in] Array of objectIds of the objects that are the nested containers of the subentity.
	\returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult subGetSubentPathsAtGsMarker( OdDb::SubentType type,
    OdGsMarker gsMarker,
    const OdGePoint3d& pickPoint,
    const OdGeMatrix3d& xrm,
    OdDbFullSubentPathArray& subentPaths,
    const OdDbObjectIdArray* pEntAndInsertStack ) const;

  /** \details
    Gets all graphic system markers by subentity paths.

    \param subPath    [in] Full subentity path to the subentity.
    \param gsMarkers [out] Array of integer identifiers (GS markers).
    \returns eOk if the operation completed successfully, or an appropriate
    error code otherwise.
  */
  OdResult subGetGsMarkersAtSubentPath( const OdDbFullSubentPath& subPath,
    OdGsMarkerArray& gsMarkers) const;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdDbSubDMesh object pointers.
*/
typedef OdSmartPtr<OdDbSubDMesh> OdDbSubDMeshPtr;

/** \details
  Corresponding C++ library: TD_Db

  <group !!RECORDS_tdrawings_apiref>
*/
struct MeshFaceterSettings
{
  /** Maximum surface deviation. */
  double faceterDevSurface;
  /** Maximum normal deviation. */
  double faceterDevNormal;
  /** Grid ratio. */
  double faceterGridRatio;
  /** Maximum edge length. */
  double faceterMaxEdgeLength;
  /** Maximum aspect ratio for the grid quads. */
  OdUInt16 faceterMaxGrid;
  /** Minimum number of U parameter grid lines. */
  OdUInt16 faceterMinUGrid;
  /** Minimum number of V parameter grid lines. */
  OdUInt16 faceterMinVGrid;
  /** Faceter mesh type.
    * 0 &mdash; quad type,
    * 1 &mdash; hybrid (triangle & quad) type,
    * 2 &mdash; triangle type.
  */
  OdInt16 faceterMeshType;

  /** \details
    Sets all settings to default values.
  */
  MeshFaceterSettings()
    : faceterDevSurface(0.)
    , faceterDevNormal(0.)
    , faceterGridRatio(0.)
    , faceterMaxEdgeLength(0)
    , faceterMaxGrid(0)
    , faceterMinUGrid(0)
    , faceterMinVGrid(0)
    , faceterMeshType(0)  {}
};

typedef struct MeshFaceterSettings  OdDbFaceterSettings;

/** \details
  Creates a mesh from the specified object based on the specified mesh faceter
  settings.

  \param pObj         [in] Pointer to the object from which the mesh will be generated.
  \param faceter      [in] Parameters that specify the rules for creating the mesh. Optional.
  \param vertexArray [out] Array of vertex points.
  \param faceArray   [out] Array of integers describing the face definition based on the input vertex array.
  \param faceData    [out] Pointer to the array of properties for each face.
  \returns
  * eNullObjectPointer &mdash; if the pointer to the object is null;
  * eNotThatKindOfClass &mdash; if the object is not a Polyline/3dSolid/Surface/Region or their direct inheritor;
  * eDegenerateGeometry &mdash; if an error occurred while getting the mesh;
  * eOk &mdash; if 3dSolid created successfully.

  \remarks
  The modeler of the object (ModelerGeometry, SpaModeler or C3dModeler) affects
  the result of the method. Current triangulation settings also affect the
  result, as MeshFaceterSettings may not override all of them.

  \remarks
  If the faceter parameter is set to NULL, the current triangulation parameters
  of the modeler are used.

  \remarks
  If the value of the faceData pointer is non-null, a new OdGiFaceData array is
  not created. It's created only if at least one of the properties is redefined:
  color, material or materialMapper.

  \remarks
  If the method is called for the OdDbPolyline, it will attempt to create a
  region and get a mesh from it, therefore the curve should be closed and
  without self-intersections.
*/
DBENT_EXPORT OdResult oddbGetObjectMesh(OdDbObject *pObj,
                                           const OdDbFaceterSettings *faceter,
                                           OdGePoint3dArray& vertexArray,
                                           OdInt32Array& faceArray,
                                           OdGiFaceData*& faceData);


#include "TD_PackPop.h"

#endif
