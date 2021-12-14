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




#ifndef GEOMENT_DBSOL3D_H
#define GEOMENT_DBSOL3D_H 

#include "TD_PackPush.h"
#include "ModelerDefs.h"
#include "DbRegion.h"
#include "DbSubentId.h"

class OdDbSubentId;
class OdDbCurve;
class OdBrBrep;
class OdModelerGeometry;
class OdDbLoftOptions;
class OdDbSweepOptions;
class OdDbRevolveOptions;
class OdDbSurface;
namespace GeMesh { class OdGeTrMesh; }
class wrTriangulationParams;

#include "DbEntity.h"
#include "OdArray.h"
#include "DwgDeclareMembers.h"



class OdDb3dSolid;

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdDb3dSolid object pointers.
*/
typedef OdSmartPtr<OdDb3dSolid> OdDb3dSolidPtr;

/** \details
  This template class is a specialization of the OdArray class for OdDb3dSolid
  object pointer arrays.
*/
typedef OdArray<OdDb3dSolidPtr> OdDb3dSolidPtrArray;


/** \details
  This class represents 3D Solid entities in an OdDbDatabase instance.

  \remarks
  An OdDb3dSolid entity is a wrapper for an ACIS model that represents the
  geometry of the OdDb3dSolid entity.

  <group OdDb_Classes>
*/
class DBENT_EXPORT OdDb3dSolid : public OdDbEntity
{
public:
  //DOM-IGNORE-BEGIN
  DWGMAP_DECLARE_MEMBERS(OdDb3dSolid);
  //DOM-IGNORE-END

  /** \details
    Default constructor for objects of the OdDb3dSolid class.
  */
  OdDb3dSolid();

  /** \details
    Returns true if and only if there is no ACIS model associated with this
    entity.

    \returns True if there is no ACIS model associated with this entity; false
    otherwise.
  */
  bool isNull() const;

  /** \details
    Writes the ACIS data of this entity to the specified StreamBuf object.

    \param pStreamBuf [in] Pointer to the StreamBuf object to which the data is to be written.
    \param typeVer    [in] Type and version of the ACIS data to write.

    \returns eOk if successful or an appropriate error code if not.
  */
  OdResult acisOut(
    OdStreamBuf* pStreamBuf,
    AfTypeVer typeVer = kAfTypeVerAny) const;

  /** \details
    Reads the ACIS data for this entity from the specified StreamBuf object.

    \param pStreamBuf [in] Pointer to the StreamBuf object from which the data is to be read.
    \param pTypeVer   [in] Receives the type and version of the ACIS data read.

    \returns eOk if successful or an appropriate error code if not.
  */
  OdResult acisIn(
    OdStreamBuf* pStreamBuf,
    AfTypeVer *pTypeVer = 0);

  /** \details
    Gets the boundary representation of the 3D solid entity.

    \param brep [out] Receives the boundary representation.
  */
  void brep(
    OdBrBrep& brep);

  /** \details
    Gets the mesh of the particular face of this Renderer object.

    \param mesh               [out] Resulting mesh.
    \param iFace               [in] Face marker.
    \param triangulationParams [in] Triangulation Parameters.

    \returns
    True if successful; false otherwise.
  */
  bool getFaceMesh(GeMesh::OdGeTrMesh& mesh, OdGsMarker iFace, const wrTriangulationParams& triangulationParams) const;

  virtual bool subWorldDraw(
    OdGiWorldDraw* pWd) const;

  virtual void subViewportDraw(
    OdGiViewportDraw* pVd) const;

  virtual OdResult dwgInFields(
    OdDbDwgFiler* pFiler);

  virtual void dwgOutFields(
    OdDbDwgFiler* pFiler) const;

  virtual OdResult dxfInFields(
    OdDbDxfFiler* pFiler);

  virtual void dxfOutFields(
    OdDbDxfFiler* pFiler) const;

  virtual OdResult subGetClassID(
    void* pClsid) const;

  /** \details
    Creates a WCS aligned solid box centered about the world origin.

    \param xLen [in] Dimension along the WCS X-axis.
    \param yLen [in] Dimension along the WCS Y-axis.
    \param zLen [in] Dimension along the WCS Z-axis.

    \remarks
    The following constraints are applied:
    * xLen >= 1e-6
    * yLen >= 1e-6
    * zLen >= 1e-6
  */
  virtual void createBox(
    double xLen,
    double yLen,
    double zLen);

  /** \details
    Creates a WCS aligned frustum centered about the world origin.

    \param height     [in] Dimension along the WCS Z-axis.
    \param xRadius    [in] Base radius along the WCS X-axis.
    \param yRadius    [in] Base radius along the WCS Y-axis.
    \param topXRadius [in] Top radius along the WCS X-axis.

    \remarks
    The function can generate a frustum that is any one of the following:
    * A circular cylinder (when xRadius is equal to topXRadius and xRadius and
      yRadius are the same)
    * An elliptical cylinder (when xRadius is equal to topXRadius and xRadius
      and yRadius are not the same)
    * A circular cone (when topXRadius is zero and xRadius and yRadius are the
      same)
    * An elliptical cone (when topXRadius is zero and xRadius and yRadius are
      not the same)
    * A truncated circular cone (when topXRadius is not zero but not equal to
      xRadius and xRadius and yRadius are the same)
    * A truncated elliptical cone (when topXRadius is not zero  but not equal to
      xRadius and xRadius and yRadius are not the same)

    The following constraints are applied:
    * height >= 1e-6
    * xRadius >= 1e-6
    * yRadius >= 1e-6
    * topXRadius >= 1e-6
  */
  virtual void createFrustum(
    double height,
    double xRadius,
    double yRadius,
    double topXRadius);

  /** \details
    Creates a sphere centered about the world origin with the specified radius.

    \param radius [in] Radius for the sphere.

    \remarks
    The following constraints are applied:
    * radius >= 1e-6
  */
  virtual void createSphere(
    double radius);

  /** \details
    Creates a WCS aligned torus centered about the world origin and the Z-axis
    as its axis of rotation.

    \param majorRadius [in] Radius of the torus.
    \param minorRadius [in] Radius of the tube.

    \remarks
    majorRadius in contrast to minorRadius can't be equal to zero.
    If majorRadius is smaller than zero, minorRadius must be larger than the
    absolute value of majorRadius.

    The following constraints are applied:
    * minorRadius >= 1e-6
    * |majorRadius| >= 1e-6
    * if majorRadius < 0, then minorRadius > |majorRadius| + 1e-6
  */
  virtual void createTorus(
    double majorRadius,
    double minorRadius);

  /** \details
    Creates a WCS aligned wedge centered about the world origin.

    \param xLen [in] Dimension along the WCS X-axis.
    \param yLen [in] Dimension along the WCS Y-axis.
    \param zLen [in] Dimension along the WCS Z-axis.

    \returns eOk if successful or an appropriate error code if not.

    \remarks
    The inclined face is defined by the points

              ( xLen/2, -yLen/2, -zLen/2)
              (-xLen/2, -yLen/2,  zLen/2)
              ( xLen/2,  yLen/2, -zLen/2)
              (-xLen/2, +yLen/2,  zLen/2)

    The following constraints are applied:
    * xLen >= 1e-6
    * yLen >= 1e-6
    * zLen >= 1e-6
  */
  virtual void createWedge(
    double xLen,
    double yLen,
    double zLen);

  /** \details
    Creates a solid by extruding the specified region to the specified height
    and with specified taper angle.

    \param pRegion    [in] Pointer to the region to be extruded.
    \param height     [in] Height of the extrusion.
    \param taperAngle [in] Taper angle in radians.

    \returns eOk if extrusion is successful or an appropriate error code if not.

    \remarks
    The extrusion direction is along the normal of the region. The height value
    may be negative. pRegion should not have self-intersections. Creates a solid
    by extruding pRegion, a distance of height with a taper angle of taper. The
    extrusion direction is along the normal of the region if the height is
    positive.

    The following constraints are applied:
    * |height| >= 1e-6
    * |taperAngle| < OdaPI2 - 1e-6

    Taper should be between half pi and -half pi. If the absolute value of
    taper < 1e-6, then the taper angle is set to 0.

    This method is implemented for Spatial modeler and ModelerGeometry.

    ModelerGeometry implementation return eNotImplementedYet if taperAngle != 0.
  */
  virtual OdResult extrude(
    const OdDbRegion* pRegion,
    double height,
    double taperAngle = 0.0);

  /** \details
    Creates a solid by revolving the specified region around the axis. Axis is
    specified with a point and vector. Revolution can be produced with an angle.

    \param pRegion           [in] Pointer to the region to be revolved.
    \param axisPoint         [in] Point on the axis of revolution.
    \param axisDir           [in] Vector defining the axis of revolution.
    \param angleOfRevolution [in] Angle of revolution in radians.

    \returns
    eOk if creating a solid by revolving is successful or an appropriate error
    code if not.

    \remarks
    The axis of revolution is projected onto the plane of the region, parallel
    to its normal. Revolution axis must be valid for a rotation of region:
    axisDir is not perpendicular to the plane of a region, axisDir does not pass
    through a region, etc.

    The following constraints are applied:
    * The projected axis of revolution cannot intersect the region.
    * angleOfRevolution >= 1e-6.

    This method is implemented for Spatial modeler and ModelerGeometry.
  */
  virtual OdResult revolve(
    const OdDbRegion* pRegion,
    const OdGePoint3d& axisPoint,
    const OdGeVector3d& axisDir,
    double angleOfRevolution);

  /** \details
    Performs a boolean operation between this solid and another solid object.

    \param operation [in] Type of boolean operation.
    \param solid    [out] Pointer to other solid object.

    \returns eOk if boolean operation is successful or an appropriate error code
    if not. If solid is null, eInvalidInput is returned.

    \remarks
    The result of the boolean operation will be written to this solid object.

    For ModelerGeometry works only with the attached protocol extension OdDbBooleanEvaluationPE.
    To learn how to attach it, see for example, Drawing/Examples/ExCustObjs/ExModelerCmds.cpp.

    Possible operation types are:
    * kBoolUnite &mdash; unites two solids into one.
    * kBoolIntersect &mdash; returns the intersection of two solids.
    * kBoolSubtract &mdash; subtracts the second solid from the first one.
  */
  virtual OdResult booleanOper(OdDb::BoolOperType operation, OdDb3dSolid* solid);

  /** \details
    Creates a solid by extruding a region along a path curve.

    \param region     [in] Pointer to a region object to extrude from.
    \param path       [in] Pointer to a curve object to extrude along.
                           Must be one of the following:
                           * Line
                           * OdDbArc
                           * OdDbCircle
                           * Ellipse
                           * OdDbSpline
                           * OdDb2dPolyline
                           * non-spline fit OdDb3dPolyline
    \param taperAngle [in] Extrusion taper angle in radians. The value must be
                           between 0.5*pi and -0.5*pi. Default value is 0.0.
                           If the path represented by the OdDbCurve object is
                           closed, the taper angle parameter is ignored.

    \returns eOk if the extruding operation is successful or an appropriate
    error code if not. If the solid is null, eInvalidInput is returned.

    \remarks
    Entity path must not have self-intersections, and it is recommended that the
    path does not have high curvature areas.

    The following constraints are applied: |taper Angle| >= 1e-6.

    This method is implemented for Spatial modeler and partially for
    ModelerGeometry.

    The ModelerGeometry implementation returns the eNotImplementedYet status
    when one of the following conditions is true:
    * taperAngle != 0.
    * The path isn't perpendicular to the region plane at the path base point.
      The path base point is a point on the path that is the closest to the
      region center from the set of:
      * All interscetion points of the region plane and the path in case when
        the path end point lies on the region plane.
      * Path endpoints in case when both endpoints don't lie on the region
        plane.
    * Both path endpoints lie on region plane and aren't equal.
  */
  virtual OdResult extrudeAlongPath(const OdDbRegion* region, const OdDbCurve* path, double taperAngle = 0.0);

  /** \details
    Intersects the given entity with the solid and imprints their intersection
    graph onto the solid.

    \param pEntity [in] Pointer to the entity to be imprinted.

    \returns eOk if imprinting of the entity onto the solid is successful or an
    appropriate error code if not.

    \remarks
    The given entity must be one of the following:
    * Curve
    * OdDb3dSolid
    * Body
    * Region
    * Trace
    * OdDbSolid
    * Face

    If the solid is null, or if the given entity is not one of the entities
    listed above, eInvalidInput is returned.

    If as a result of intersection a closed loop of new edges is created, a new
    face will be made.
    If an open loop of edges is made, it can be added to an existing loop as a
    spur. Or it can be added as a slit in the face.

    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult imprintEntity(const OdDbEntity *pEntity);

  /** \details
    Calculates the area of the solid.

    \param area [out] Reference for returning the area value.

    \returns eOk if successful or an appropriate error code in other case. The
    area value is returned in square drawing units.

    \remarks
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult getArea(double& area) const;

  /** \details
    Checks whether the solid interferes with another solid object.

    \param otherSolid         [in] Pointer to another solid object for checking interference.
    \param createNewSolid     [in] Flag determining whether a new solid object containing the interference must be created.
    \param solidsInterfere   [out] Reference to a boolean variable where the result of the interference checking will be stored and returned.
    \param commonVolumeSolid [out] Reference to a pointer to a new solid containing the interference of the origin solid and otherSolid. 

    \returns eOk if interference checking is successful or an appropriate error
    code in other case.

    \remarks
    The caller of this function is responsible for the memory used by the object
    pointed by commonVolumeSolid (resulting from intersection of this solid and
    otherSolid) when the function finishes.
    In case when commonVolumeSolid is added to a database, the database takes
    responsibility for the memory pointed by commonVolumeSolid.
    In case when commonVolumeSolid is not added to a database, the caller must
    release memory when it is no longer needed to avoid a memory leak.

    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult checkInterference(const OdDb3dSolid* otherSolid,
    bool createNewSolid,
    bool& solidsInterfere,
    OdDb3dSolidPtr &commonVolumeSolid)
    const;

  /** \details
    Returns volume properties of the solid.

    \param volume        [out] Reference for returning the volume of the solid.
    \param centroid      [out] Reference to an OdGePoint3d object for returning the point, representing the centroid of the solid.
    \param momInertia    [out] Static array for returning X, Y, Z moments of inertia of the solid.
    \param prodInertia   [out] Static array for returning X, Y, Z products of inertia of the solid.
    \param prinMoments   [out] Static array for returning X, Y, Z principal moments of the solid.
    \param prinAxes      [out] Static array for returning an OdGeVector3d object, determining principal axes of the solid.
    \param radiiGyration [out] Static array for returning X, Y, Z radii of gyration of the solid.
    \param extents       [out] Reference to an OdGeExtents3d object for returning the bounding box of the solid.

    \returns eOk if successful or an appropriate error code in other case. All
    properties are evaluated in the WCS.
    
    \remarks
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult getMassProp(double& volume,
    OdGePoint3d& centroid,
    double momInertia[3],
    double prodInertia[3],
    double prinMoments[3],
    OdGeVector3d prinAxes[3],
    double radiiGyration[3],
    OdGeExtents3d& extents) const;

  /** \details
    Creates a DbRegion entity as a result of the intersection between the
    specified plane and the solid.

    \param plane          [in] Reference to an OdGePlane object to use as the section cutting plane.
    \param sectionRegion [out] Reference to a new region created as the result of the intersection between the specified plane and the solid.

    \returns eOk if successful or an appropriate error code in other case. eOk
    is also returned when a plane and the solid do not intersect each other or
    if the intersection region is not a valid region: it intersects at a point,
    along a bounding face of the solid and so on.

    \remarks
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult getSection(const OdGePlane& plane, OdDbRegionPtr &sectionRegion) const;

  /** \details
    Slices the solid with a plane and transforms the native solid to be the
    portion remaining on the positive side of the normal of the slicing plane.

    \param plane          [in] Reference to a slicing plane.
    \param bGetNegHalfToo [in] Flag determining whether the other (negative)
                               part of the native solid should be created as a
                               separate solid (when true).
    \param pNegHalfSolid [out] Reference to a pointer to the created solid
                               representing the negative half of the native
                               solid. If bGetNegHalfToo is false, the NULL value
                               is returned.

    \returns eOk if the native solid was successfully sliced or an appropriate
    error code in other case. If the slicing plane does not intersect the solid,
    the solid is left unchanged, pNegHalfSolid is set to NULL, and eOk is
    returned.

    \remarks
    The caller of this function is responsible for the memory used by the object
    pointed to by pNegHalfSolid (negative part of the sliced solid) when the
    function finishes.
    In case when pNegHalfSolid is added to a database, the database takes
    responsibility for the memory.
    In case when pNegHalfSolid is not added to a database, the caller must
    release memory when it is no longer needed to avoid a memory leak.

    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult getSlice(const OdGePlane& plane, bool bGetNegHalfToo, OdDb3dSolidPtr &pNegHalfSolid);

  /** \details
    Slices the solid with a surface and transforms the native solid to be the
    portion remaining on the positive side of the slicing surface normal.

    \param pSurface       [in] Pointer to a slicing surface.
    \param bGetNegHalfToo [in] Flag determining whether the other (negative)
                               part of the native solid should be created as a
                               separate solid (when true).
    \param pNegHalfSolid [out] Reference to a pointer to the created solid
                               representing the negative half of the native
                               solid. If bGetNegHalfToo is false, the NULL value
                               is returned.

    \returns eOk if the native solid was successfully sliced or an appropriate
    error code in other case. If pSurface pointer is NULL or the solid is empty,
    returns eInvalidInput.

    \remarks
    The native solid will be transformed to a resulting body which is located in
    the negative side of the surface normal.

    The caller of this function is responsible for the memory used by the object
    pointed to by pNegHalfSolid (negative part of the sliced solid) when the
    function finishes.
    In case when pNegHalfSolid is added to a database, the database takes
    responsibility for the memory.
    In case when pNegHalfSolid is not added to a database, the caller must
    release memory when it is no longer needed to avoid a memory leak.

    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult getSlice(OdDbSurface* pSurface,  bool bGetNegHalfToo, OdDb3dSolidPtr &pNegHalfSolid);

  /** \details
    Removes all edges and faces that are not needed for supporting the topology
    of the solid.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult cleanBody();

  /** \details
    Offsets all faces by the specified distance.

    \param offsetDistance [in] Distance for offset.

    \returns eOk if all faces are successfully offset, or an appropriate error
    code in other case. Returns eInvalidInput if the offsetDistance is less than
    -maxSide/2, where maxSide is the size of the greater side of the solid's
    bounding box. Also eInvalidInput is returned when the absolute value of
    offsetDistance is less than 1e-6.

    \remarks
    If a face has surfaces that cannot be offset, it is removed. Therefore, the
    offset operation can cause the solid to become invalid.

    This method is implemented only for Spatial modeler and returns the 
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult offsetBody(double offsetDistance);

  /** \details
    Separates the solid into an array of solids representing the additional
    disjoint volumes. This solid is reduced to a solid with one volume.

    \param newSolids [out] Reference to an array of pointers to OdDb3dSolid
                           objects representing the additional disjoint volumes.

    \returns eOk if separating was successful or an appropriate error code in
    other case.

    \remarks
    The calling application is responsible for the resulting entities (either
    appending them to a database or deleting them when they are no longer
    needed).
    If the new solids from the output array are not appended to the database,
    there will be a memory leak.

    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult separateBody(OdDb3dSolidPtrArray &newSolids);

  /** \details
    Creates a solid by sweeping a planar curve, region, or planar surface in the
    direction of the given vector using a distance equal to the length of the
    vector.

    \param pSweepEnt    [in] Pointer to the swept entity.
    \param directionVec [in] Vector determining the direction and length of extruding.
    \param sweepOptions [in] Set of sweeping options.

    \returns eOk if extruding was successful or an appropriate error code in
    other case.

    \remarks
    Sweeping parameters (for example, angle) are set through the sweepOptions
    object reference.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult createExtrudedSolid ( OdDbEntity* pSweepEnt,
    const OdGeVector3d& directionVec,
    OdDbSweepOptions& sweepOptions );

  /** \details
    Creates a solid by extruding a face.

    \param pSweepEnt    [in] Pointer to the swept entity.
    \param faceSubentId [in] Object ID for the face that must be extruded.
    \param directionVec [in] Vector determining the direction and length of extruding.
    \param sweepOptions [in] Set of sweeping options.

    \returns eOk if extruding was successful or an appropriate error code in
    other case.

    \remarks
    Sweeping parameters (for example, angle) are set through the sweepOptions
    object reference.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult createExtrudedSolid ( OdDbEntity* pSweepEnt,
    const OdDbSubentId& faceSubentId,
    const OdGeVector3d& directionVec,
    OdDbSweepOptions& sweepOptions );

  /** \details
    Creates a solid by extruding a face.

    \param pSweepEnt    [in] Pointer to the swept entity.
    \param faceSubentId [in] Object ID for the face that must be extruded.
    \param height       [in] Multiplier for the direction vector.
    \param sweepOptions [in] Set of sweeping options.

    \returns eOk if extruding was successful or an appropriate error code in
    other case.

    \remarks
    Sweeping parameters (for example, angle) are set through the sweepOptions
    object reference.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult createExtrudedSolid ( OdDbEntity* pSweepEnt,
    const OdDbSubentId& faceSubentId,
    double height,
    OdDbSweepOptions& sweepOptions );

  /** \details
    Creates a lofted solid from the given curves.

    \param crossSectionCurves [in] Array of curve entities to be used as cross sections for the lofting operation.
    \param guideCurves        [in] Array of guide curves.
    \param pPathCurve         [in] Path curve.
    \param loftOptions        [in] Options for lofting.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    Lofting parameters can be passed with the loftOptions parameter.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult createLoftedSolid (
    OdDbEntityPtrArray& crossSectionCurves,
    OdDbEntityPtrArray& guideCurves,
    OdDbEntity* pPathCurve, OdDbLoftOptions& loftOptions );

  /** \details
    Creates a pyramid. World origin is centered about the base of the pyramid
    and the pyramid is positioned at a half of the specified height above the
    world origin.

    \param height    [in] Value of the pyramid's height.
    \param sides     [in] Quantity of sides for the pyramid.
    \param radius    [in] Radius for circle inscribed into the bottom polygon.
    \param topRadius [in] Radius for circle inscribed into the top polygon.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    The pyramid's base is located in the X-Y plane of the WCS. The pyramid's
    direction (from the base to the top) coincides with the positive direction
    of the Z axis.
    When radius and topRadius values are equal, a prism is created.
    The following constraints are applied:
    * |height| >= 1e-6
    * |radius| >= 1e-6
    * |topRadius| >= 1e-6
    * 3 <= sides <= 64
  */
  virtual OdResult createPyramid(double height, int sides,
    double radius, double topRadius = 0.0 );

  /** \details
    Creates a solid by revolving a specified curve, region, or planar surface
    around the specified axis of revolution.

    \param pRevEnt        [in] Pointer to the planar curve, region, or planar surface that must be revolved.
    \param axisPnt        [in] Point on the axis of revolution.
    \param axisDir        [in] Direction of the revolution axis.
    \param revAngle       [in] Revolve angle in radians.
    \param startAngle     [in] Start angle of rotation. If 0, then rotation will start from the current position of pRevEnt.
    \param revolveOptions [in] Revolve options.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    Axis of revolution is defined by a point (axisPnt) and vector (axisDir).
    pRevEnt will be revolved according to the start and revolve angles
    counter-clockwise about the axisDir vector.
    Options for revolving can be specified with the revolveOptions object
    reference.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult createRevolvedSolid ( OdDbEntity* pRevEnt,
    const OdGePoint3d& axisPnt,
    const OdGeVector3d& axisDir,
    double revAngle, double startAngle,
    OdDbRevolveOptions& revolveOptions );

  /** \details
    Creates a solid by revolving a face about the axis of revolution.

    \param pRevEnt        [in] Pointer to the entity containing the face for revolving.
    \param faceSubentId   [in] Face's subentity ID.
    \param axisPnt        [in] Point on the axis of revolution.
    \param axisDir        [in] Direction of revolution axis.
    \param revAngle       [in] Revolve angle in radians.
    \param startAngle     [in] Start angle of rotation. If 0, then rotation will start from the current position of pRevEnt.
    \param revolveOptions [in] Revolve options.

    \returns eOk if successful, or an appropriate error code in other case.

    \remarks
    Entity for revolving must be a valid planar curve, region, or planar
    surface.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult createRevolvedSolid ( OdDbEntity* pRevEnt,
    const OdDbSubentId& faceSubentId,
    const OdGePoint3d& axisPnt,
    const OdGeVector3d& axisDir,
    double revAngle, double startAngle,
    OdDbRevolveOptions& revolveOptions );

  /** \details
    Creates a swept solid by sweeping a face along the specified path.

    \param pSweepEnt    [in] Pointer to the face entity for sweeping.
    \param faceSubentId [in] Face's subentity ID.
    \param pPathEnt     [in] Pointer to the curve entity that determines the path to sweep along.
    \param sweepOptions [in] Sweep options.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    To set optional sweep parameters (for example, draft angle) use the
    sweepOptions parameter.
    The default value for the alignment option of OdDbSweepOptions (returned by
    the align() method) is kNoAlignment. Set it to kAlignSweepEntityToPath
    before calling this method.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult createSweptSolid ( OdDbEntity *pSweepEnt,
    const OdDbSubentId& faceSubentId,
    OdDbEntity* pPathEnt,
    OdDbSweepOptions& sweepOptions );

  /** \details
    Creates a swept solid by sweeping a curve, region, or planar surface along
    the specified path.

    \param pSweepEnt    [in] Pointer to the face entity for sweeping.
    \param pPathEnt     [in] Pointer to the curve entity that determines the path to sweep along.
    \param sweepOptions [in] Sweep options.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    To set optional sweep parameters (for example, draft angle) use the
    sweepOptions parameter.
    The default value for the alignment option of OdDbSweepOptions (returned by
    the align() method) is kNoAlignment. Set it to kAlignSweepEntityToPath
    before calling this method.
    This method is implemented for Spatial modeler and partially for
    ModelerGeometry. The ModelerGeometry implementation at almost all cases
    returns the eNotImplementedYet status.
  */
  virtual OdResult createSweptSolid ( OdDbEntity* pSweepEnt,
    OdDbEntity* pPathEnt,
    OdDbSweepOptions& sweepOptions );

  /** \details
    Writes out a Stereo Lithography (STL) representation of the solid to a file
    with the specified name.

    \param output      [in] Output stream buffer.
    \param asciiFormat [in] Flag determining the format of the output file (if
                            true, the file has ASCII text format, or in other
                            case the file has binary format).

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    If the specified file already exists, it will be overwritten.

    \remarks
    If at least one of the obtained triangle points has at least one coordinate
    from the (-inf, 0] interval, then all points will be shifted to an octant,
    where all coordinates are positive numbers from the [0.01, +inf) interval.
  */
  virtual OdResult stlOut(OdStreamBuf* output, bool asciiFormat, double maxSurfaceDeviation = 0.0) const;

  /** \details
    Writes out a Stereo Lithography (STL) representation of the solid to a file
    with the specified name.

    \param filename    [in] Full file name.
    \param asciiFormat [in] Flag determining the format of the output file (if
                            true, the file has ASCII text format, or in other
                            case the file has binary format).

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    If the specified file already exists, it will be overwritten.

    \remarks
    If at least one of the obtained triangle points has at least one coordinate
    from the (-inf, 0] interval, then all points will be shifted to an octant,
    where all coordinates are positive numbers from the [0.01, +inf) interval.
  */
  virtual OdResult stlOut(const OdChar* filename, bool asciiFormat, double maxSurfaceDeviation = 0.0) const;

  virtual OdDbObjectPtr decomposeForSave(
    OdDb::DwgVersion ver,
    OdDbObjectId& replaceId,
    bool& exchangeXData);

  virtual OdResult subExplode(
    OdRxObjectPtrArray& entitySet) const;

  virtual OdResult subTransformBy(
    const OdGeMatrix3d& xfm );

  /** \details
    Creates a geometric representation of the entity for saving proxy graphics
    or converting to previous formats.

    \param pWd [in] Pointer to the OdGiWorldDraw object.
    \param ver [in] Specifies the reason why the method is called.

    \remarks
    This method can be overridden in custom classes.
  */
  virtual void saveAs(
    OdGiWorldDraw* pWd,
    OdDb::DwgVersion ver ) const;

  /** \details
    Directly sets the contained modeler geometry of this entity.

    \param pGeometry [in] Pointer to the modeler geometry.

    \returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult setBody( const void* pGeometry );

  /** \details
    Returns contained modeler geometry of this entity.
  */
  virtual void *body( ) const;

  /** \details
    Copies the specified edge as a separate entity (instance of the OdDbLine,
    OdDbCircle, OdDbArc, OdDbEllipse, OdDbSpline class).

    \param subentId  [in] Edge's subentity ID in the solid.
    \param newEntity [in] Reference to a new entity for the specified edge.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    The properties of the resulting entity (color, layer, line type, lineweight)
    are inherited from the corresponding current document.
    The calling application is responsible for the memory allocated for the
    resulting entity.
    If the resulting entity is not appended to a database, it can cause a memory
    leak (if it is not deleted when it isn't needed anymore).
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult copyEdge(const OdDbSubentId &subentId, OdDbEntityPtr &newEntity);

  /** \details
    Copies the specified face as a separate entity (OdDbRegion or OdDbBody).

    \param subentId  [in] Face's subentity ID in the solid.
    \param newEntity [in] Reference to a new entity containing a copy of the specified face.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    If the face is planar, an OdDbRegion entity is created. Otherwise an
    OdDbBody entity is created.
    The properties of the resulting entity (color, layer, line type, lineweight)
    are inherited from the corresponding current document.
    The calling application is responsible for the memory allocated for the
    resulting entity.
    If the resulting entity is not appended to a database, it can cause a memory
    leak (if it is not deleted when it isn't needed anymore).
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult copyFace(const OdDbSubentId &subentId, OdDbEntityPtr &newEntity);

  virtual OdResult subGetSubentPathsAtGsMarker(OdDb::SubentType type,
                                               OdGsMarker gsMark,
                                               const OdGePoint3d& pickPoint,
                                               const OdGeMatrix3d& viewXform,
                                               OdDbFullSubentPathArray& subentPaths,
                                               const OdDbObjectIdArray* pEntAndInsertStack = 0) const;

  virtual OdDbSubentId internalSubentId(void* ent) const;

  virtual void* internalSubentPtr(const OdDbSubentId& id) const;

  virtual OdResult subGetGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath,
                                               OdGsMarkerArray& gsMarkers) const;

  virtual OdDbEntityPtr subSubentPtr(const OdDbFullSubentPath& id) const;

  /** \details
    Extrudes faces of the solid with the specified distance and taper angle.

    \param faceSubentIds [in] Face's subentity ID in the solid.
    \param height        [in] Distance of extrusion.
    \param taper         [in] Taper angle.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    The distance of extrusion is determined by the height parameter and the
    taper angle (the taper parameter).
    The extrusion direction can be along or opposite the normal of each extruded
    face depending on the height value:
    * If the height is positive, direction is along the normal (extruding faces
      outward from the solid).
    * If the height is negative, direction is opposite the normal (extruding
      faces into the solid).

    If the absolute value of the taper is less than 1e-6, the taper angle is
    assumed to be equal to 0.0.
    If the taper value is not equal to 0.0, each face should have edges that
    join together smoothly (i.e. tangents at the points of connection must be
    equal).
    The following constraints are applied:
    * Number of specified faces (in the faceSubentIds parameter) must be more
      than zero.
    * |height| >= 1e-6
    * |taper| >= (PI/2) - 1e-6

    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult extrudeFaces(const OdArray<OdDbSubentId *> &faceSubentIds,
    double height, double taper);

  /** \details
    Extrudes specified faces along the specified path determined by a curve
    entity.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be extruded.
    \param path          [in] Pointer to a curve object to extrude along (extrusion path). Can not be NULL.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    Faces for extrusion must be planar.
    Curve entity must be one of the following:
    * OdDbLine
    * OdDbArc
    * OdDbEllipse
    * OdDbSpline
    * OdDb2dPolyline
    * Non-spline fit OdDb3dPolyline

    Extrusion path must not have self-intersections. Also it is recommended that
    the path doesn't have high curvature areas.
    If one of the end points of the path is not located on the region's plane,
    the path will move the point on the plane.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult extrudeFacesAlongPath(const OdArray<OdDbSubentId *> &faceSubentIds,
    const OdDbCurve* path);

  /** \details
    Tapers the specified faces about the specified base point and draft vector
    for a specified draft angle.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be tapered.
    \param basePoint     [in] Reference to the base point of the draft plane.
    \param draftVector   [in] Reference to the draft direction vector.
    \param draftAngle    [in] Draft angle in radians.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    The base point and the draft vector define a draft plane about which the
    faces are tapered.
    If the face is in the draft plane, it will not be modified.

    Tapering faces can cause the solid to become invalid in some cases.

    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult taperFaces(const OdArray<OdDbSubentId *> &faceSubentIds,
    const OdGePoint3d &basePoint,
    const OdGeVector3d &draftVector,
    double draftAngle);

  /** \details
    Transforms the specified faces of a solid by applying the transformation
    matrix for rotation and (or) moving the faces.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be transformed.
    \param matrix        [in] Transformation matrix to be applied to the specified faces.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    Transformation faces can cause the solid to become invalid in some cases.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult transformFaces(const OdArray<OdDbSubentId *> &faceSubentIds,  const OdGeMatrix3d &matrix);

  /** \details
    Removes the specified faces from the solid.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be removed.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    After removing specified faces, the adjacent faces will enlarge to fill the
    gaps.
    This method implements gap filling only if there is no other solution using
    adjacent faces.
    If adjacent faces can't be used, the method leaves the solid unmodified.
    Removing faces can cause the solid to become invalid in some cases.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult removeFaces(const OdArray<OdDbSubentId *> &faceSubentIds);

  /** \details
    Offsets the specified faces of the solid for the specified distance.

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be offset.
    \param offsetDistance [in] Offset distance.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    Faces with surfaces that cannot be offset are removed and the solid is
    healed, if possible.
    The following constraints are applied:
    * Number of specified faces (in the faceSubentIds parameter) must be more
      than zero.
    * offsetDistance >= minus half of the max side of the solid's bounding box.
    * |offsetDistance| >= 1e-6.

    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult offsetFaces(const OdArray<OdDbSubentId *> &faceSubentIds, double offsetDistance);

  /** \details
    Transforms the solid into a thin-walled solid shell by offsetting faces.

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be removed from the shell.
    \param offsetDistance [in] Offset distance for each face.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    Offset can have an outside or inside direction related to the solid,
    depending on the sign of the offsetDistance parameter:
    * positive value means outside direction;
    * negative value means inside direction.

    If a face has surfaces that cannot be offset by the thickness, it will be
    removed from the solid. Removal wound is healed with the surrounding face
    surfaces.
    Edges and vertices on the specified faces will be merged if possible.
    Isolated edges in tweaked faces and neighboring faces may degenerate to
    points and will be removed.
    However, when removing loops or faces, insertion of edges or other topology
    changes are prohibited.
    Operation can cause the solid to become invalid in some cases.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult shellBody(const OdArray<OdDbSubentId *> &faceSubentIds, double offsetDistance);

  /** \details
    Returns the number of changes occurred since the solid's creation.
  */
  virtual OdUInt32 numChanges() const;

  /** \details
    Sets the color of the specified face or edge subentity.

    \param subentId [in] ID of the subentity (face or edge) to be colored.
    \param color    [in] Color value to set.

    \returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult setSubentColor(const OdDbSubentId &subentId, const OdCmColor &color);

  /** \details
    Returns the color of the specified subentity.

    \param subentId [in] ID of the subentity.
    \param color   [out] Reference to an OdCmColor object to store the current color of the specified subentity.

    \returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult getSubentColor(const OdDbSubentId &subentId, OdCmColor &color) const;

  /** \details
    Sets a material for the specified subentity.

    \param subentId [in] Subentity ID.
    \param matId    [in] Object ID for the material.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    To remove the currently assigned material, set the matId parameter value to
    NULL.
  */
  virtual OdResult setSubentMaterial(const OdDbSubentId &subentId, OdDbObjectId matId);

  /** \details
    Returns the material of the specified subentity.

    \param subentId [in] Subentity ID.
    \param matId   [out] Reference to an OdDbObjectId for returning the material object ID.

    \returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult getSubentMaterial(const OdDbSubentId &subentId, OdDbObjectId& matId) const;

  /** \details
    Sets an explicit material mapper for the specified subentity.

    \param subentId [in] Subentity ID.
    \param mapper   [in] Pointer to an OdGiMapper object.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    To remove the current material mapper assignment, set the map parameter
    value to NULL.
  */
  virtual OdResult setSubentMaterialMapper(const OdDbSubentId &subentId, const OdGiMapper& mapper);

  /** \details
    Returns current material mapper for the specified subentity.

    \param subentId [in] Subentity ID.
    \param mapper  [out] Reference to an OdGiMapper object containing the current subentity material mapper.

    \returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult getSubentMaterialMapper(const OdDbSubentId &subentId, OdGiMapper& mapper) const;

  /** \details
    Creates chamfers at the edges of the solid.

    \param edgeSubentIds    [in] Array of edge object IDs. The chamfer will be applied at the specified edges.
    \param baseFaceSubentId [in] Base face object IDs. Chamfer will be applied on the specified face.
    \param baseDist         [in] Chamfer distance for the base face.
    \param otherDist        [in] Chamfer distance for other faces.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult chamferEdges(const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdDbSubentId& baseFaceSubentId,
                                double baseDist,
                                double otherDist);

  /** \details
    Creates rounded fillets on the edges of the solid.

    \param edgeSubentIds [in] Array of edges' object IDs. Fillet will be applied on the specified edges.
    \param radius        [in] Array of fillet radius values for the corresponding edges.
    \param startSetback  [in] Array of start setback values for the corresponding edges.
    \param endSetback    [in] Array of end setback values for the corresponding edges.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult filletEdges( const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdGeDoubleArray& radius,
                                const OdGeDoubleArray& startSetback,
                                const OdGeDoubleArray& endSetback);

  /** \details
    Creates a 3D solid from the specified entity.

    \param pFromEntity [in] Pointer to an entity from which the solid must be created.

    \returns eOk if conversion is successful or an appropriate error code in other case.

    \remarks
    An entity to create from must enclose a volume and can be one of the following:
    * OdDbSurface
    * OdDbSubDMesh
    * closed curve with thickness
    * thick OdDbSolid
    * thick OdDbTrace
    * polyline with constant width and thickness

    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult createFrom(const OdDbEntity* pFromEntity);

  /** \details
    Indicates whether the solid records its operations history.

    \returns True if the solid records its operations history; false otherwise.

    \remarks
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual bool recordHistory() const;

  /** \details
    Indicates whether the items from the solid's operation history are drawn.

    \returns True if the items from the solid's operation history are drawn;
    false otherwise.

    \remarks
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual bool showHistory() const;

  /** \details
    Sets a new value for the flag that determines whether the solid records its
    operation history.

    \param bRecord [in] Flag determining whether the solid records its operation history.

    \returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult setRecordHistory(bool bRecord);

  /** \details
    Sets a new value for the flag that determines whether to draw items from the
    solid's operation history.

    \param bShow [in] Flag determining whether to draw items from the operation history.

    \returns eOk if successful or an appropriate error code in other case.
  */
  virtual OdResult setShowHistory(bool bShow);

  /** \details
    Converts individual history items from a parameterized type to a simple
    boundary-representation without its parameter or type information.

    \param paths [in] Array of references to history items.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    This method is not implemented.
  */
  virtual OdResult convertToBrepAtSubentPaths(const OdDbFullSubentPathArray& /*paths*/);

  virtual OdUInt32 subSetAttributes(OdGiDrawableTraits* pTraits) const;

  /** \details
    Creates non-database resident entities by projecting the specified entity
    along the projection direction to the solid.

    \param pEntityToProject    [in] Pointer to the entity to be projected.
    \param projectionDirection [in] Vector determining the projection direction.
    \param projectedEntities  [out] Array of pointers to non-database resident entities created by the projection.

    \returns eOk if successful or an appropriate error code in other case.

    \remarks
    The specified pointer to the entity must not be equal to NULL and the entity
    must be of a supported type.
    When an OdDbPoint is specified as the entity to be projected, the result of
    the projection will be an AcDbPoint representing the nearest point on the
    solid.
    This method is implemented only for Spatial modeler and returns the
    eNotImplementedYet status for other modelers.
  */
  OdResult projectOnToSolid( const OdDbEntity*   pEntityToProject,
                             const OdGeVector3d& projectionDirection,
                             OdDbEntityPtrArray& projectedEntities) const;

  /** \details
    Creates a solid by trimming and (or) extending the specified surfaces and 3D
    solids, forming an enclosed volume.

    \param limitingBodies [in] Reference to the entity array (representing surface and solids) that are used to create a solid.
    \param limitingFlags  [in] Reserved for internal use.

    \returns eOk if successful or an appropriate error code in other case.
    
    \remarks
    Implemented for ModelerGeometry and Spatial Modeler, not supported in the C3D modeler.

    Works only with the attached protocol extension OdDbBooleanEvaluationPE.
    To learn how to attach it, see for example, Drawing/Examples/ExCustObjs/ExModelerCmds.cpp.
  */
  OdResult createSculptedSolid(OdDbEntityPtrArray& limitingBodies,
                               const OdGeIntArray& limitingFlags);

  /** \details
    This method is not implemented.
  */
  virtual void subClose();

  virtual OdResult subGetGeomExtents(OdGeExtents3d& extents) const;
};

#include "TD_PackPop.h"

#endif
