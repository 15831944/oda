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


#ifndef _OD_MODELERGEOMETRY_INCLUDED_
#define _OD_MODELERGEOMETRY_INCLUDED_

#define STL_USING_MAP
#include "OdaSTL.h"
#include "RxObject.h"
#include "ModelerDefs.h"
#include "DbEntity.h"
#include "Db3dSolid.h"
#include "DbRegion.h"
#include "DbSubDMesh.h"
#include "DbNurbSurface.h"
#include "Ge/GeMatrix3dArray.h"
#include "Ge/GeNurbCurve3d.h"
#include "Ge/GeCurve3dPtrArray.h"
#include "MaterialResolver.h"
#include "ModelerGeometryDefs.h"

class OdStreamBuf;
class OdBrBrep;
class OdBrEntity;
class OdGiCommonDraw;
class OdGiViewportDraw;
class OdDbCurve;
class OdDbRevolveOptions;
class OdDbSweepOptions;
class OdDbLoftOptions;
class OdDbBlendOptions;
class OdDbSection;
class SectArgs;
class OdBrMesh2dFilter;
class OdBrMesh2d;
namespace GeMesh { class OdGeTrMesh; }
class wrTriangulationParams;

#include "TD_PackPush.h"

/** \details
  This class defines the interface for classes that provide conversion services
  for ACIS SAT and SAB data.

  \remarks
  This class is used to convert ACIS data between various versions of SAT and
  SAB.

  For example, if a version 2004 .dwg file is saved as an R15 .dxf file, the SAB
  data in the 2004 file must be converted to version 700 SAT, so as to be valid
  within the R15 .dxf file.

  Corresponding C++ library: ModelerGeometry
  <group Other_Classes>
*/
class DBENT_EXPORT OdModelerGeometry : public OdRxObject
{
protected:
  //DOM-IGNORE-BEGIN
  OdModelerGeometry() {}
  //DOM-IGNORE-END

public:
  //DOM-IGNORE-BEGIN
  ODRX_DECLARE_MEMBERS(OdModelerGeometry);
  //DOM-IGNORE-END

  // in/out functionality

  /** \details
    Reads the ACIS data from the specified StreamBuf object into this
    ModelerGeometry object.

    \param pStreamBuf       [in] Pointer to the StreamBuf object from which the data is to be read.
    \param typeVer         [out] Receives the type and version of the ACIS file.
    \param standardSaveFlag [in] Controls the saving and restoring of used count data in the saved file.
    \param bEnableACISAudit [in] True to do additional checks of ACIS data.

    \returns eOk if successful, or an appropriate error code otherwise.

    \remarks
    The implementation of this function is responsible for determining the type
    (SAT/SAB) and version of the data read.
    \remarks
    pStreamBuf can be NULL.

    \sa
    AfTypeVerEnum
  */
  virtual OdResult in(OdStreamBuf* pStreamBuf, AfTypeVer *typeVer = 0, bool standardSaveFlag = true) = 0;


  /** \details
    Writes the ACIS data from this ModelerGeometry object to the specified
    StreamBuf object.

    \param pStreamBuf       [in] Pointer to the StreamBuf object to which the data is to be written.
    \param typeVer          [in] Type and version of the ACIS file.
    \param standardSaveFlag [in] Controls the saving and restoring of used count data in the saved file.
    \param bEnableACISAudit [in] True to do additional checks of acis data.

    \returns eOk if successful, or an appropriate error code otherwise.

    \remarks
    This function is responsible for converting the contained ACIS data to the
    specified type and format. If typeVer == kAfTypeVerAny, then any type or
    version can be written.

    \sa
    AfTypeVerEnum
  */
  virtual OdResult out(OdStreamBuf* pStreamBuf, AfTypeVer typeVer, bool standardSaveFlag = true) const = 0;

  /** \details
    Populates the specified BrBrep object with the ACIS BREP data of this
    ModelerGeometry object.
    \param brBrep [out] Receives the BREP data.
    \returns True if and only if successful; false otherwise.
  */
  virtual bool brep(OdBrBrep& brBrep) const = 0;

  virtual OdResult getEdgeSubentityGeometry(OdArray<OdUInt32> indexesOfEdgeCalculation, OdGeCurve3d*& pEdgeCurve) = 0;

  /** \details
    This method is not implemented.
    \returns eNotImplemented.
  */
  virtual OdResult setSubentPath(OdBrEntity & /*pInpEnt*/, OdDbFullSubentPath& /*subpath*/) { return eNotImplemented; }

  /** \details
    This method is not implemented.
    \returns eNotImplemented.
  */
  virtual OdResult generateMesh(const OdBrMesh2dFilter& filter, OdBrMesh2d &mesh2d) { return eNotImplemented; }

  /** \details
    Sets the Facetres for this ModelerGeometry object.
    \param facetRes [in] Facet resolution.
    \returns True if and only if successful; false otherwise.
  */
  virtual bool setFACETRES(double facetRes) = 0;

  /** \details
    Stores the number of isolines in U and V directions.
  */
  struct NumIsolines
  {
    /** Number of isolines in U direction. */
    OdUInt32 numU,
    /** Number of isolines in V direction. */
             numV;
  };

  /** \details
    Creates a viewport-independent geometric representation of this
    ModelerGeometry object.

    \param pWd          [in] Pointer to the OdGiCommonDraw interface.
    \param geomType     [in] Geometry type.
    \param pNumIsolines [in] Number of isolines in U and V directions.

    \returns True if and only if the geometric representation can be generated
    in a viewport-independent manner.

    \remarks
    A return value of false indicates that viewportDraw() must be called for
    this object.
    \remarks
    The 3D GS will call this function at least once, but may cache subsequent
    display updates.
    \remarks
    geomType must be a combination of one or more of the following:
    <table>
    Name             Value
    kNothing         0
    kIsolines        1
    kEdges           2
    kShells          4
    kOrderedEdges    8
    kSurfIsolines    16
    </table>
  */
  virtual bool worldDraw(OdGiCommonDraw* pWd, OdUInt32 geomType, const NumIsolines *pNumIsolines = 0) = 0;

  /** \details
    Creates a viewport-dependent silhouette representation of this
    ModelerGeometry object.

    \param pVd [in] Pointer to the OdGiViewportDraw interface.

    \remarks
    Causes this ModelerGeometry object to describe its geometry to the specified
    OdGiViewportDraw object.
    This function is called once per viewport.
    \remarks
    It is possible to have totally different images in each viewport: a
    schematic in one viewport, a layout in another.
  */
  virtual bool drawSilhouettes(OdGiViewportDraw* pVd) = 0;

  virtual OdResult getCachedSilhouettes(OdGeCurve3dPtrArray* cachedSilhouettes) = 0;

  /** \details
    Creates a view-dependent representation which consists of visible and hidden
    curves of input objects.

    \param edgeVisitor [in/out] Receives edges as OdGeCurves and it's visible/hidden segments.
    \param viewDir         [in] View direction (normal to projection plane).
    \param entities        [in] Entities (solids and surfaces) for which hidden and visible lines will be calculated.
    \param xforms          [in] Transformation matrices.
    \param edgeType        [in] Flag for excluding some type of edges (tangent edges for example).

    \returns eOk if successful, or an appropriate error code if not.
  */
  virtual OdResult getHlrCurves( OdEdgeHLREdgeVisitor& edgeVisitor,
                                const OdGeVector3d& viewDir,
                                const OdDbEntityPtrArray& entities,
                                const OdGeMatrix3dArray* xforms = NULL,
                                OdEdgeTypeFlags edgeType = kTangent) = 0;


  /** \details
    Explodes this ModelerGeometry object into a set of OdDbEntity objects.

    \param entitySet [in/out] Receives an array of pointers to the new entities.

    \returns True if successful; false otherwise.

    \remarks
    Entities resulting from the explosion are appended to the specified array.
    The newly created entities are not database residents.
  */
  virtual bool explode(OdDbEntityPtrArray& entitySet) const = 0;

  /** \details
    Returns the transformation matrix for this ModelerGeometry object.
    \param xfm [out] Receives the transformation matrix.
    \returns True if successful; false otherwise.
  */
  virtual bool getTransformation(OdGeMatrix3d& xfm) = 0;

  /** \details
    Applies the specified 3D transformation matrix to this ModelerGeometry
    object.

    \param xfm [in] 3D transformation matrix.
  */
  virtual void transformBy( const OdGeMatrix3d& xfm ) = 0;

  /** \details
    Creates a WCS aligned solid box centered about the world origin.

    \param xLen [in] Dimension along the WCS X-axis.
    \param yLen [in] Dimension along the WCS Y-axis.
    \param zLen [in] Dimension along the WCS Z-axis.

    \remarks
    The following constraints apply:
      * xLen >= 1e-6
      * yLen >= 1e-6
      * zLen >= 1e-6
  */
  virtual void createBox( double xLen, double yLen, double zLen ) = 0;

  /** \details
    Creates a WCS aligned frustum centered about the world origin.

    \param height     [in] Dimension along the WCS Z-axis.
    \param xRadius    [in] Base radius along the WCS X-axis.
    \param yRadius    [in] Base radius along the WCS Y-axis.
    \param topXRadius [in] Top radius along the WCS X-axis.

    \remarks
    The function can generate a frustum that is any one of the following:

    * A circular cylinder
    * An elliptical cylinder
    * A circular cone
    * An elliptical cone
    * A truncated circular cone
    * A truncated elliptical cone

    \remarks
    The following constraints apply:
      * height >= 1e-6
      * xRadius >= 1e-6
      * yRadius >= 1e-6
      * topXRadius >= 1e-6
  */
  virtual void createFrustum( double height, double xRadius, double yRadius, double topXRadius ) = 0;

  /** \details
    Creates a sphere centered about the world origin.

    \param radius [in] Sphere radius.

    \remarks
    The following constraints apply:
      * radius >= 1e-6
  */
  virtual void createSphere( double radius ) = 0;

  /** \details
    Creates a WCS aligned torus centered about the world origin and with the
    Z-axis as its axis of rotation.

    \param majorRadius [in] Radius of the torus.
    \param minorRadius [in] Radius of the tube.

    \remarks
    The following constraints apply:
      * minorRadius >= 1e-6
      * |majorRadius| >= 1e-6
      * if majorRadius < 0, then minorRadius > |majorRadius| + 1e-6
  */
  virtual void createTorus( double majorRadius, double minorRadius ) = 0;

  /** \details
    Creates a WCS aligned wedge centered about the world origin.

    \param xLen [in] Dimension along the WCS X-axis.
    \param yLen [in] Dimension along the WCS Y-axis.
    \param zLen [in] Dimension along the WCS Z-axis.

    \remarks
    The inclined face is defined by the points:

              ( xLen/2, -yLen/2, -zLen/2)
              (-xLen/2, -yLen/2,  zLen/2)
              ( xLen/2,  yLen/2, -zLen/2)
              (-xLen/2, +yLen/2,  zLen/2)

    \remarks
    The following constraints apply:
      * xLen >= 1e-6
      * yLen >= 1e-6
      * zLen >= 1e-6
  */
  virtual void createWedge( double xLen, double yLen, double zLen ) = 0;

  /** \details
    Creates a solid by extruding the specified region.

    \param pRegion    [in] Pointer to the region to be extruded.
    \param height     [in] Height of the extrusion.
    \param taperAngle [in] Taper angle.
    \param isSolid    [in] Boolean flag that indicates that this ModelerGeometry object is solid.

    \remarks
    The extrusion direction is along the normal of the region. Height can be
    negative.

    \remarks
    All angles are expressed in radians.

    The following constraints apply:
      * |height| >= 1e-6
      * |taperAngle| < OdaPI2 - 1e-6
  */
  virtual OdResult extrude(const OdDbRegion* pRegion, double height, double taperAngle, bool isSolid = true) = 0;

  /** \details
    Creates a solid by revolving the specified region.

    \param pRegion           [in] Pointer to the region to be revolved.
    \param axisPoint         [in] Point on the axis of revolution.
    \param axisDir           [in] Vector defining the axis of revolution.
    \param angleOfRevolution [in] Angle of revolution.
    \param isSolid           [in] Boolean flag that indicates that this ModelerGeometry object is solid.

    \remarks
    The axis of revolution is projected onto the plane of the region, parallel
    to its normal.

    \remarks
    All angles are expressed in radians.

    The following constraints apply:
      * The projected axis of revolution cannot intersect the region
      * angleOfRevolution >= 1e-6
  */
  virtual OdResult revolve(const OdDbRegion* pRegion, const OdGePoint3d& axisPoint,
    const OdGeVector3d& axisDir, double angleOfRevolution, bool isSolid = true) = 0;

  /** \details
    Clears the color attributes of this ModelerGeometry object.
  */
  virtual void ClearColorAttributes() = 0;

  /** \details
    Clears the material attributes of this ModelerGeometry object.
  */
  virtual void ClearMaterialAttributes(const OdArray<OdDbSubentId>* aSubents = NULL) = 0;

  /** \details
    Clears the material mapper attributes of this ModelerGeometry object.
  */
  virtual void ClearMaterialMapperAttributes() = 0;

  /** \details
    Describes a material state.
  */
  enum MaterialState
  {
    /** No materials. */
    kNoMaterials = 0, 
    /** Has materials. */
    kHasMaterials = 1,
    /** Unknown. */
    kUnknown = 2
  };

  /** \details
    Returns the material state of this ModelerGeometry object.
    \returns Material state.
    \remarks
    Currently not implemented. Always returns kUnknown.
  */
  virtual MaterialState hasMaterials() const {return kUnknown; }

  /** \details
    Returns true if and only if TrueColor attributes are attached to the faces
    and/or edges of this ModelerGeometry object.
    \returns True if TrueColor attributes are attached to the faces and/or edges
    of this ModelerGeometry object; false otherwise.
  */
  virtual bool hasTrueColorAttributes() const = 0;

  /** \details
    Returns the plane of this ModelerGeometry object.

    \param regionPlane [out] Receives the region plane.

    \returns eOk if successful, or an appropriate error code if
    (bodyType() != kRegion).
  */
  virtual OdResult getPlane(OdGePlane& regionPlane) const = 0;

  /** \details
    Sets the mm/unit value for this ModelerGeometry object.
    \param mmPerUnit [in] mm/unit value.
    \returns True if and only if successful.
  */
  virtual bool setMMPerUnit(double  mmPerUnit) = 0;

  /** \details
    Returns the mm/unit value for this ModelerGeometry object.
    \param mmPerUnit [out] mm/unit value.
    \returns True if and only if successful.
  */
  virtual bool getMMPerUnit(double &mmPerUnit) const = 0;

  /** \details
    Defines geometry types.
  */
  enum geomType
  {
    /** Undefined. */
    kUndefined = 0,
    /** Body. */
    kBody      = 1,
    /** Solid. */
    kSolid     = 2,
    /** Region. */
    kRegion    = 3,
    /** Surface. */
    kSurface   = 4
  };

  /** \details
    Returns the body type of this ModelerGeometry object.
    \remarks
    bodyType returns one of the following:

    <table>
    Name          Value
    kUndefined    0
    kBody         1
    kSolid        2
    kRegion       3
    </table>
  */
  virtual geomType bodyType() const = 0;

  
  /** \details
    Performs a Boolean operation between this ModelerGeometry object and another
    ModelerGeometry object.

    \param database              [in] Database of the ModelerGeometry objects.
    \param operation             [in] Type of Boolean operation.
    \param otherBody             [in] Pointer to the other ModelerGeometry object.
    \param typeThis              [in] Type of this ModelerGeometry object.
    \param typeOther             [in] Type of the other ModelerGeometry object.
    \param result               [out] (Optional) Receives pointer to the resulting ModelerGeometry object.
    \param intersectionEntities [out] (Optional) Array of intersection entities.

    \returns eOk if the Boolean operation is successful or an appropriate error
    code if not. If ModelerGeometry is NULL, eInvalidInput is returned.

    \remarks
    Possible operation types are:
      * kBoolUnite &mdash; Unites two ModelerGeometry objects into one.
      * kBoolIntersect &mdash; Returns the intersection of two ModelerGeometry objects.
      * kBoolSubtract &mdash; Subtracts the second ModelerGeometry object from the first one.
    Both ModelerGeometry objects need to be from the same database.
    The result is saved in this ModelerGeometry object.
  */
  virtual OdResult booleanOper(OdDbDatabase* database, OdDb::BoolOperType operation, const OdModelerGeometry* otherBody,
    geomType typeThis, geomType typeOther, OdSmartPtr<OdModelerGeometry>* result = NULL, OdDbEntityPtrArray* intersectionEntities = NULL) = 0;

  /** \details
    Returns the area of this ModelerGeometry object.
    \param regionArea [out] Reference for returning the area value.
    \returns eOk if successful or an appropriate error code otherwise.
    \remarks
    Area value is returned in square drawing units.
  */
  virtual OdResult getArea(double& regionArea) const = 0;

  /** \details
    Returns the perimeter of this ModelerGeometry object.
    \param dRegionPerimeter [out] Reference to a double value to store the perimeter.

    \returns eOk if the perimeter was successfully returned to the calling
    subroutine or an appropriate error code in the other case.
  */
  virtual OdResult getPerimeter(double&) const = 0;

  /** \details
    Calculates the area properties of this ModelerGeometry object.
    The method validates the origin, xAxis, and yAxis parameters to ensure that:
      * The axes are perpendicular to each other.
      * The axes and the origin lie in the same plane as the region.

    \param origin         [in] Origin of the coordinate system to use for evaluation (WCS).
    \param xAxis          [in] X-axis of the coordinate system to use for evaluation (WCS).
    \param yAxis          [in] Y-axis of the coordinate system to use for evaluation (WCS).
    \param perimeter     [out] Reference for returning the value of the ModelerGeometry object's perimeter.
    \param area          [out] Reference for returning the value of the ModelerGeometry object's area.
    \param centroid      [out] Reference for returning the centroid of the ModelerGeometry object.
    \param momInertia    [out] Static array for returning moments of inertia for the ModelerGeometry object.
    \param prodInertia   [out] Reference for returning the ModelerGeometry object's product of inertia.
    \param prinMoments   [out] Static array for returning principal moments for the ModelerGeometry object.
    \param prinAxes      [out] Static array for returning principle axes of the ModelerGeometry object.
    \param radiiGyration [out] Static array for returning radii of gyration of the ModelerGeometry object.
    \param extentsLow    [out] Reference for returning the ModelerGeometry object's minimum extents point.
    \param extentsHigh   [out] Reference for returning the ModelerGeometry object's maximum extents point.

    \returns eOk if successful, or an appropriate error code otherwise.

    \remarks
    Returned values are measured using the values of origin, xAxis, and yAxis
    parameters, and are represented in WCS coordinates.
  */
  virtual OdResult getAreaProp( const OdGePoint3d& origin, const OdGeVector3d& xAxis,
    const OdGeVector3d& yAxis, double& perimeter,
    double& area, OdGePoint2d& centroid,
    double momInertia[2], double& prodInertia,
    double prinMoments[2], OdGeVector2d prinAxes[2],
    double radiiGyration[2], OdGePoint2d& extentsLow,
    OdGePoint2d& extentsHigh ) const = 0;

  /** \details
    Clears the contents of this ModelerGeometry object.

    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult clear( ) = 0;

  /** \details
    Creates a ModelerGeometry object by extruding a region along a path curve.

    \param region     [in] Pointer to a region object for extruding from.
    \param path       [in] Pointer to a curve object to extrude along.
                           Must be one of the following:
                             * Line
                             * OdDbArc
                             * OdDbCircle
                             * Ellipse
                             * OdDbSpline
                             * OdDb2dPolyline
                             * non-spline fit OdDb3dPolyline
    \param taperAngle [in] Extrusion taper angle in radians. Default value is 0.0.
                           Value must be between 0.5 pi and -0.5 pi.
                           If the path represented by the OdDbCurve object is closed, the taper angle parameter is ignored.
    \param isSolid    [in] Boolean flag that indicates that the ModelerGeometry object is a solid.

    \returns eOk if extruding operation is successful or an appropriate error
    code otherwise.

    \remarks
    Entity path must not have self-intersections, and it is recommended that the
    path does not have high curvature areas.
    The following constraints are applied: |taper Angle| >= 1e-6.
  */
  virtual OdResult extrudeAlongPath(const OdDbRegion* region, const OdDbCurve* path, double taperAngle = 0.0, bool isSolid = true) = 0;

  /** \details
    Intersects the given entity with this ModelerGeometry object and imprints
    their intersection graph onto the ModelerGeometry object.

    \param pEntity [in] Pointer to the entity to be imprinted.

    \returns eOk if imprinting entity onto the ModelerGeometry object is
    successful or an appropriate error code if not.

    \remarks
    The given entity must be one of the following:
      * Curve
      * OdDb3dSolid
      * Body
      * Region
      * Trace
      * OdDbSolid
      * Face

    If the ModelerGeometry object is NULL, or if the given entity is not one of
    the entities listed above, eInvalidInput is returned.
    If as a result of intersection a closed loop of new edges is created, a new
    face will be made.
    If an open loop of edges is made, it can be added to an existing loop as a
    spur. Or it can be added as a slit in the face.
  */
  virtual OdResult imprintEntity(const OdDbEntity *pEntity) = 0;

  /** \details
    Checks whether the solid interferes with another solid object.

    \param otherSolid         [in] Pointer to another solid object for checking interference.
    \param createNewSolid     [in] Flag determining where a new solid object containing the interference must be created.
    \param solidsInterfere   [out] Reference to a boolean variable where the result of the interference checking will be stored and returned.
    \param commonVolumeSolid [out] Reference to a pointer to a new solid containing the interference of the origin solid and otherSolid.

    \returns eOk if interference checking is successful or an appropriate error
    code in other case.

    \remarks
    The caller of this function is responsible for the memory used by the object
    pointed to by commonVolumeSolid (resulting from intersection of this solid
    and otherSolid) when the function finishes.
    If commonVolumeSolid is added to a database, the database takes
    responsibility for the memory pointed to by commonVolumeSolid.
    If commonVolumeSolid is not added to a database, the caller must free memory
    when it is no longer needed to avoid a memory leak.
  */
  virtual OdResult checkInterference(const OdDb3dSolid* otherSolid,
    bool createNewSolid,
    bool& solidsInterfere,
    OdDb3dSolidPtr &commonVolumeSolid)
    const = 0;

  /** \details
    Returns volume properties of the ModelerGeometry object.

    \param volume        [out] Reference for returning the volume of the ModelerGeometry object.
    \param centroid      [out] Reference to an OdGePoint3d object for returning the point, representing the centroid of the ModelerGeometry object.
    \param momInertia    [out] Static array for returning X, Y, Z moments of inertia of the ModelerGeometry object.
    \param prodInertia   [out] Static array for returning X, Y, Z products of inertia of the ModelerGeometry object.
    \param prinMoments   [out] Static array for returning X, Y, Z principal moments of the ModelerGeometry object.
    \param prinAxes      [out] Static array for returning an OdGeVector3d object, determining principal axes of the ModelerGeometry object.
    \param radiiGyration [out] Static array for returning X, Y, Z radii of gyration of the ModelerGeometry object.
    \param extents       [out] Reference to an OdGeExtents3d object for returning the bounding box of the ModelerGeometry object.

    \returns The result of getting volume properties of the ModelerGeometry
    object: eOk if successful or an appropriate error code in other case. All
    properties are evaluated in the WCS.
  */
  virtual OdResult getMassProp(double& volume,
    OdGePoint3d& centroid,
    double momInertia[3],
    double prodInertia[3],
    double prinMoments[3],
    OdGeVector3d prinAxes[3],
    double radiiGyration[3],
    OdGeExtents3d& extents) const = 0;

  /** \details
    Creates a DbRegion entity as a result of the intersection between the
    specified plane and the ModelerGeometry object.

    \param plane           [in] Reference to the OdGePlane object to use as the section cutting plane.
    \param sectionRegion  [out] Reference to a new region created as the result
                                of the intersection between the specified plane
                                and the ModelerGeometry object.
    \param pSectionCurves [out] Pointer to an array of section curves.

    \returns The result of intersecting the ModelerGeometry object with a plane:
    eOk if successful or an appropriate error code in other case. Value eOk is
    also returned when the specified  plane and the ModelerGeometry object do
    not intersect each other or if the intersection region is not a valid
    region, that is, it intersects at a point, along a bounding face of the
    ModelerGeometry object and so on.
  */
  virtual OdResult getSection(const OdGePlane& plane, OdDbRegionPtr &sectionRegion, OdDbEntityPtrArray* pSectionCurves = NULL) const = 0;

  /** \details
    Slices the ModelerGeometry object with a plane and transforms the native
    ModelerGeometry object to be the portion remaining on the positive side of
    the normal of the slicing plane.

    \param plane         [in] Reference to a slicing plane.
    \param getNegHalfToo [in] Flag determining whether the other (negative) part
                              of the native ModelerGeometry object should be
                              created as a separate ModelerGeometry object (when
                              true).
    \param negHalfSolid [out] Reference to a pointer to the created
                              ModelerGeometry object representing the negative
                              half of the native ModelerGeometry object. If
                              getNegHalfToo is false, NULL value is returned.

    \returns eOk if the native ModelerGeometry object was successfully sliced or
    an appropriate error code in other case. If the slicing plane does not
    intersect the ModelerGeometry object, the ModelerGeometry object is left
    unchanged, negHalfSolid is set to NULL, and eOk is returned.

    \remarks
    The caller of this function is responsible for the memory used by the object
    pointed to by negHalfSolid when the function finishes. If negHalfSolid is
    added to a database, the database takes responsibility for the memory. If
    negHalfSolid is not added to a database, the caller must free memory when it
    is no longer needed to avoid a memory leak.
  */
  virtual OdResult getSlice(const OdGePlane& plane, bool getNegHalfToo, OdDb3dSolidPtr &negHalfSolid) = 0;

  /** \details
    Slices the ModelerGeometry object with a surface and transforms the native
    ModelerGeometry object to be the portion remaining on the positive side of
    the slicing surface normal.

    \param pSurface       [in] Pointer to the slicing surface.
    \param bGetNegHalfToo [in] Flag determining whether the other (negative)
                               part of the native ModelerGeometry object should
                               be created as a separate ModelerGeometry object
                               (when true).
    \param pNegHalfSolid [out] Reference to a pointer to the created
                               ModelerGeometry object representing the negative
                               half of the native ModelerGeometry object. If
                               bGetNegHalfToo is false, NULL value is returned.

    \returns eOk if the native ModelerGeometry object was successfully sliced or
    an appropriate error code in other case. If pSurface pointer is NULL or the
    ModelerGeometry object is empty, returns eInvalidInput.

    \remarks
    The native ModelerGeometry object will be transformed to the resulting body
    which is located on the negative side of the surface normal.

    The caller of this function is responsible for the memory used by the object
    pointed to by pNegHalfSolid when the function finishes. If pNegHalfSolid is
    added to a database, the database takes responsibility for the memory. If
    pNegHalfSolid is not added to a database, the caller must free memory when
    it is no longer needed to avoid a memory leak.
  */
  virtual OdResult getSlice(OdDbSurface* pSurface, bool bGetNegHalfToo, OdDb3dSolidPtr& pNegHalfSolid) = 0;

  /** \details
    Removes all edges and faces that are not needed for supporting the topology
    of the ModelerGeometry object.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult cleanBody() = 0;

  /** \details
    Offsets all faces by the specified distance.

    \param offsetDistance [in] Distance for offset.

    \returns eOk if all faces are successfully offset, or an appropriate error
    code in other case. Returns eInvalidInput if the offsetDistance is less than
    -maxSide/2, where maxSide is the size of the greater side of the
    ModelerGeometry object's bounding box. Also, eInvalidInput is returned when
    the absolute value of offsetDistance is less than 1e-6.

    \remarks
    If a face has surfaces that cannot be offset, it is removed. Therefore, the
    offset operation can cause the ModelerGeometry object to become invalid.
  */
  virtual OdResult offsetBody(double offsetDistance) = 0;

  //virtual OdResult offsetBody(OdDbEntity* pOffsetEnt, double offsetDistance) = 0;

  /** \details
    Separates the ModelerGeometry object into an array of solids representing
    the additional disjoint volumes. This ModelerGeometry object is reduced to a
    solid with one volume.

    \param newSolids [out] Reference to an array of pointers to OdDb3dSolid objects representing the additional disjoint volumes.

    \returns eOk if separating was successful or an appropriate error code
    otherwise.

    \remarks
    The calling application is responsible for the resulting entities (either
    appending them to a database or deleting them when they are no longer
    needed). If the new solids from the output array are not appended to the
    database and not deleted, there will be a memory leak.
  */
  virtual OdResult separateBody(OdDb3dSolidPtrArray &newSolids) = 0;

  /** \details
    Creates an object by sweeping a planar curve, region, or planar surface in
    the direction of the given vector using a distance equal to the length of
    the vector.

    \param pSweepEnt       [in] Pointer to the swept entity.
    \param directionVec    [in] Vector determining the direction and length of extruding.
    \param sweepOptions    [in] Set of sweeping options.
    \param isSolid         [in] Boolean flag that indicates that this ModelerGeometry object is solid.
    \param bHistoryEnabled [in] Boolean flag that indicates whether history recording is enabled.

    \returns eOk if extruding was successful or an appropriate error code in
    other case.

    \remarks
    Sweeping parameters (for example, angle) are set through the sweepOptions
    object reference.
  */
  virtual OdResult createExtrudedObject ( OdDbEntity* pSweepEnt,
                                          const OdGeVector3d& directionVec,
                                          OdDbSweepOptions& sweepOptions,
                                          bool isSolid = true,
                                          bool bHistoryEnabled = false) = 0;

  /** \details
    Creates a lofted object from the given curves.

    \param crossSectionCurves [in] Array of curve entities to be used as cross sections for the lofting operation.
    \param guideCurves        [in] Array of guide curves.
    \param pPathCurve         [in] Path curve.
    \param loftOptions        [in] Options for lofting.
    \param isSolid            [in] Boolean flag that indicates that this ModelerGeometry object is solid.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    Lofting parameters can be passed with the loftOptions parameter.
  */
  virtual OdResult createLoftedObject (
    OdDbEntityPtrArray& crossSectionCurves,
    OdDbEntityPtrArray& guideCurves,
    OdDbEntity* pPathCurve,
    OdDbLoftOptions& loftOptions,
    bool isSolid = true) = 0;

  /** \details
    Creates a blend object from the given edges.

    \param blendedEdges [in] Array of blended edges.
    \param blendOptions [in] Options for blending.
    \param isSolid      [in] Boolean flag that indicates that this ModelerGeometry object is solid.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    Blending parameters can be passed with the blendOptions parameter.
  */
  virtual OdResult createBlendObject(
    OdDbEntityPtrArray& blendedEdges,
    OdDbBlendOptions& blendOptions,
    bool isSolid = true) = 0;

  /** \details
    Creates a lofted object from the given curves by coedges.

    \param crossSectionCurves [in] Array of curve entities to be used as cross sections for the lofting operation.
    \param guideCurves        [in] Array of guide curves.
    \param pPathCurve         [in] Path curve.
    \param loftOptions        [in] Options for lofting.
    \param isSolid            [in] Boolean flag that indicates that this ModelerGeometry object is solid.

    \returns Returns eOk if successful or an appropriate error code otherwise.

    \remarks
    Lofting parameters can be passed with the loftOptions parameter.
  */
  //virtual OdResult createLoftedObjectByCoedges(
  //  OdDbEntityPtrArray& crossSectionCurves,
  //  OdDbEntityPtrArray& guideCurves,
  //  OdDbEntity* pPathCurve,
  //  OdDbLoftOptions& loftOptions,
  //  bool isSolid = true) = 0;

  /** \details
    Creates a pyramid. World origin is centered about the base of the pyramid
    and the pyramid is positioned at half of the specified height above the
    world origin.

    \param height    [in] Value of the pyramid's height.
    \param sides     [in] Quantity of sides for the pyramid.
    \param radius    [in] Radius for circle inscribed into the bottom polygon.
    \param topRadius [in] Radius for circle inscribed into the top polygon.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    The pyramid's base is located in the X-Y plane of the WCS. The pyramid's
    direction (from the base to the top) coincides with the positive direction
    of the Z-axis.
    When radius and topRadius values are equal, a prism is created.

    The following constraints are applied:
      * |height| >= 1e-6
      * |radius| >= 1e-6
      * |topRadius| >= 1e-6
      * 3 <= sides <= 64
  */
  virtual OdResult createPyramid(double height, int sides,
    double radius, double topRadius = 0.0 ) = 0;

  /** \details
    Creates an object by revolving a specified curve, region, or planar surface
    around the specified axis of revolution.

    \param pRevEnt         [in] Pointer to the planar curve, region, or planar surface that must be revolved.
    \param axisPnt         [in] Point on the axis of revolution.
    \param axisDir         [in] Direction of the revolution axis.
    \param revAngle        [in] Revolve angle in radians.
    \param startAngle      [in] Start angle of rotation. If 0, rotation will start from the current position of pRevEnt.
    \param revolveOptions  [in] Revolve options.
    \param isSolid         [in] Boolean flag that indicates that this ModelerGeometry object is solid.
    \param bHistoryEnabled [in] Boolean flag that indicates whether history recording is enabled.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    Axis of revolution is defined by a point (axisPnt) and vector (axisDir).
    pRevEnt will be revolved according to the start and revolve angles
    counter-clockwise about the axisDir vector.

    Options for revolving can be specified with the revolveOptions object reference.
  */
  virtual OdResult createRevolvedObject ( OdDbEntity* pRevEnt,
    const OdGePoint3d& axisPnt,
    const OdGeVector3d& axisDir,
    double revAngle, double startAngle,
    OdDbRevolveOptions& revolveOptions,
    bool isSolid = true,
    bool bHistoryEnabled = false) = 0;

  /** \details
    Creates a swept object by sweeping a face along the specified path.

    \param pSweepEnt       [in] Pointer to the face entity for sweeping.
    \param pPathEnt        [in] Pointer to the curve entity that determines the path to sweep along.
    \param sweepOptions    [in] Sweep options.
    \param isSolid         [in] Boolean flag that indicates that this ModelerGeometry object is solid.
    \param bHistoryEnabled [in] Boolean flag that indicates whether history recording is enabled.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    To set optional sweep parameters (for example, draft angle) use the
    sweepOptions parameter.
    The default value for the alignment option of OdDbSweepOptions (returned by
    the align() method) is kNoAlignment. Set it to kAlignSweepEntityToPath
    before calling this method.
  */
  virtual OdResult createSweptObject ( OdDbEntity* pSweepEnt,
    OdDbEntity* pPathEnt,
    OdDbSweepOptions& sweepOptions,
    bool isSolid = true,
    bool bHistoryEnabled = false) = 0;

  // Loft options utility functions

  /** \details
    Checks whether cross section curves are valid for the lofting operation.

    \param crossSectionCurves   [in] List of cross-section curves.
    \param allOpen             [out] Passed-in parameter indicating whether all of the cross-section curves are open.
    \param allClosed           [out] Passed-in parameter indicating whether all of the cross-section curves are closed.
    \param allPlanar           [out] Passed-in parameter indicating whether all of the cross-section curves are planar.
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.

    \returns eOk if curves are valid or eInvalidInput otherwise.
  */
  virtual OdResult checkCrossSectionCurves (
    OdDbEntityPtrArray& crossSectionCurves,
    bool& allOpen, bool& allClosed, bool& allPlanar,
    bool displayErrorMessages = false ) = 0;

  /** \details
    Checks whether guide curves are valid for the lofting operation.

    \param guideCurves          [in] List of guide curves.
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.

    \returns eOk if guide curves are valid or eInvalidInput otherwise.
  */
  virtual OdResult checkGuideCurves ( OdDbEntityPtrArray& guideCurves,
    bool displayErrorMessages = false ) = 0;

  /** \details
    Checks whether a path curve is valid for the lofting operation.

    \param pPathCurve           [in] Pointer to the path curve (can be NULL).
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.

    \returns eOk if a path curve is valid or eInvalidInput otherwise.
  */
  virtual OdResult checkPathCurve ( OdDbEntity *pPathCurve,
    bool displayErrorMessages = false ) = 0;

  // Sweep options utility functions

  /** \details
    Determines if the specified sweep entity is valid and returns the planarity
    of the sweep entity.

    \param pSweepEnt            [in] Pointer to the curve, region, or planar surface to be swept.
    \param planarity           [out] Passed-in parameter for returning the planarity flag of the entity.
    \param pnt                 [out] Passed-in parameter for returning the point
                                     on the sweep entity if planarity is
                                     kPlanar, or a point on the line if
                                     planarity is kLinear.
    \param vec                 [out] Passed-in parameter for returning the
                                     normal vector of the entity if planarity is
                                     kPlanar, or the line direction if planarity
                                     is kLinear.
    \param closed              [out] Passed-in parameter for returning the closed flag determining whether an entity to be swept is a closed entity.
    \param approxArcLen        [out] Passed-in parameter for returning the approximate arc length.
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.

    \returns eOk if successful or an appropriate error code in the other case.

    \remarks
    If kPlanar is returned, then pnt and vec will specify the normal plane of
    the entity. If kLinear is returned, then pnt and vec will specify a defining
    point and direction of the sweep entity.
  */
  virtual OdResult  checkSweepCurve ( OdDbEntity *pSweepEnt,
    OdDb::Planarity& planarity, OdGePoint3d& pnt, OdGeVector3d& vec,
    bool& closed, double& approxArcLen, bool displayErrorMessages = false ) = 0;

  /** \details
    Checks whether a specified sweep path curve entity is valid.

    \param pPathEnt             [in] Pointer to the curve entity that determines the path to sweep along.
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.

    \returns eOk if the specified entity is valid, or eInvalidInput otherwise.
  */
  virtual OdResult  checkSweepPathCurve ( OdDbEntity *pPathEnt,    bool displayErrorMessages = false ) = 0;

  // Revolve options utility functions

  /** \details
    Checks whether a specified entity is valid for the revolve operation.

    \param pRevEnt              [in] Pointer to the entity (curve or region) to be revolved.
    \param axisPnt              [in] Point on the axis of revolution.
    \param axisDir              [in] Vector determining the direction of the revolution axis.
    \param closed              [out] Passed-in parameter for returning the indicator determining whether the revolved entity is closed.
    \param endPointsOnAxis     [out] Passed-in parameter for returning the
                                     indicator determining whether the revolved
                                     entity is open and whether its endpoints
                                     lie on the axis of revolution.
    \param planar              [out] Passed-in parameter for returning the indicator determining whether the revolved entity is planar.
    \param displayErrorMessages [in] Indicates whether error messages are displayed. Default value is false.

    \returns eOk if the specified entity is valid for revolving, or
    eInvalidInput otherwise.
  */
  virtual OdResult checkRevolveCurve(
    OdDbEntity* pRevEnt,
    const OdGePoint3d& axisPnt,
    const OdGeVector3d& axisDir,
    bool& closed,
    bool& endPointsOnAxis,
    bool& planar,
    bool displayErrorMessages = false) = 0;

  /** \details
    Creates an extruded surface when given a planar entity with thickness, or
    creates a plane surface when given a simple closed planar entity.

    \param pFromEntity [in] Pointer to either a planar entity with thickness, a
                            simple closed planar entity, or an OdDbSolid entity
                            pointed to by pFromEntity.

    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult createFrom ( const OdDbEntity* pFromEntity ) = 0;

  /** \details
    Creates region (OdDbRegion class) entities for each face of the
    ModelerGeometry object.

    \param regions [out] Reference to an array of the OdDbRegion entities created from the face of the ModelerGeometry object.

    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult convertToRegion ( OdDbEntityPtrArray& regions ) = 0;

  /** \details
    Creates a 3D solid object by thickening the surface by a specified amount.

    \param thickness  [in] Amount to thicken the surface by.
    \param bBothSides [in] Flag indicating whether the thickening should be applied to both sides of the surface.
    \param pSolid    [out] Reference to an OdDb3dSolid object pointer created during the thickening. If thickening fails, this parameter is NULL.

    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult thicken(double thickness, bool bBothSides, OdDb3dSolidPtr& pSolid) const = 0;

  /** \details
    Retrieves the intesection points between this entity and another specified
    entity.

    \param ent           [in] Entity which this entity is intersected with.
    \param intType       [in] Type of intersection.
    \param points       [out] Array of points in which the intersection points should be appended.
    \param thisGsMarker  [in] GS marker of the subentity of this entity which is involved in the intersection.
    \param otherGsMarker [in] GS marker of the subentity of the entity pointed to by ent which is involved in the intersection.

    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult intersectWith( const OdDbEntity* ent, OdDb::Intersect intType,
    OdGePoint3dArray& points, OdGsMarker thisGsMarker, OdGsMarker otherGsMarker ) const = 0;

  /** \details
    Retrieves the intesection points on a projection plane between this entity
    and another specified entity.

    \param ent           [in] Entity which this entity is intersected with.
    \param intType       [in] Type of intersection.
    \param projPlane     [in] Projection plane for the intersection of the two entities.
    \param points       [out] Array of points in which the intersection points should be appended.
    \param thisGsMarker  [in] GS marker of the subentity of this entity which is involved in the intersection.
    \param otherGsMarker [in] GS marker of the subentity of the entity pointed to by ent which is involved in the intersection.

    \returns eOk if successful, or an appropriate error code otherwise.
  */
  virtual OdResult intersectWith( const OdDbEntity* ent, OdDb::Intersect intType, const OdGePlane& projPlane,
    OdGePoint3dArray& points, OdGsMarker thisGsMarker, OdGsMarker otherGsMarker ) const = 0;

  /** \details
    Creates a copy of a specified subentity of this entity.

    \param subentId   [in] ID of the subendity to be copied.
    \param newEntity [out] Reference to the created copy of the subentity.

    \returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult copySubEntity(const OdDbSubentId &subentId, OdDbEntityPtr &newEntity) const = 0;

  /** \details
    Tapers specified faces about a specified base point and draft vector for a
    specified draft angle.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be tapered.
    \param basePoint     [in] Reference to the base point of the draft plane.
    \param draftVector   [in] Reference to the draft direction vector.
    \param draftAngle    [in] Draft angle in radians.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    The base point and draft vector define a draft plane about which faces are
    tapered. If a face is lying in the draft plane, it will not be modified.
  */
  virtual OdResult taperFaces(const OdArray<OdDbSubentId *> &faceSubentIds, const OdGePoint3d &basePoint,
                              const OdGeVector3d &draftVector, double draftAngle) = 0;

  /** \details
    Removes the specified faces from this ModelerGeometry object.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be removed.

    \returns eOk if successful or an appropriate error code in the other case.

    \remarks
    After removing the specified faces, the adjacent faces will enlarge to fill
    the gaps.
  */
  virtual OdResult removeFaces(const OdArray<OdDbSubentId *> &faceSubentIds) = 0;

  /** \details
    Offsets the specified faces of this ModelerGeometry object for the specified
    distance.

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be offset.
    \param offsetDistance [in] Offset distance.

    \returns eOk if successful or an appropriate error code in the other case.

    \remarks
    The following constraints are applied:
      * The number of specified faces (in faceSubentIds parameter) must be more than zero.
      * offsetDistance >= minus half of the max side of the ModelerGeometry object's bounding box.
      * |offsetDistance| >= 1e-6.
  */
  virtual OdResult offsetFaces(const OdArray<OdDbSubentId *> &faceSubentIds, double offsetDistance) = 0;

  /** \details
    Transforms this ModelerGeometry object into a thin-walled ModelerGeometry
    object shell by offsetting faces.

    \param faceSubentIds  [in] Array of faces' subentity IDs. Faces with specified IDs will be removed from the shell.
    \param offsetDistance [in] Offset distance for each face.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    Offset can have an outside or inside direction related to the
    ModelerGeometry object, depending on the sign of the offsetDistance
    parameter:
      * Positive value means outside direction.
      * Negative value means inside direction.
  */
  virtual OdResult shellBody(const OdArray<OdDbSubentId *> &faceSubentIds, double offsetDistance) = 0;

  /** \details
    Transforms the specified faces of a ModelerGeometry object by applying the
    transformation matrix for rotation and (or) moving the faces.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be transformed.
    \param matrix        [in] Transformation matrix to be applied to the specified faces.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult transformFaces(const OdArray<OdDbSubentId *> &faceSubentIds, const OdGeMatrix3d &matrix) = 0;

  /** \details
    Retrieves full subentity paths of subentities, represented by a GS marker.

    \param type              [in] Subentity type specified by the OdDb::SubentType enumeration.
    \param gsMark            [in] GS marker representing a specific subentity.
    \param pickPoint         [in] WCS coordinates of the point picked during the selection of the entity.
    \param viewXform         [in] Transformation matrix to transform from WCS to DCS.
    \param subentPaths      [out] Array of full subentity path objects identifying the subentities generated from the GS marker.
    \param entAndInsertStack [in] Array of object IDs that are the nested containers of the subentity.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getSubentPathsAtGsMarker(OdDb::SubentType type,
                                            OdGsMarker gsMark, 
                                            const OdGePoint3d& pickPoint,
                                            const OdGeMatrix3d& viewXform, 
                                            OdDbFullSubentPathArray& subentPaths, 
                                            const OdDbObjectIdArray* entAndInsertStack = 0) const = 0;

  /** \details
    Returns an array of GS markers.

    \param subPath    [in] Full subentity path object identifying the subentity.
    \param gsMarkers [out] Array with GS markers for the subentities.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getGsMarkersAtSubentPath(const OdDbFullSubentPath& subPath,
                                            OdGsMarkerArray& gsMarkers) const = 0;

  //DOM-IGNORE-BEGIN
  /** \details
    For internal use only.

    \param ent [in] ACIS object pointer.
    \returns Internal subentity ID.
  */
  virtual OdDbSubentId internalSubentId(void* ent) const = 0;

  /** \details
    For internal use only.

    \param id [out] Subentity ID in the ACIS object.
  */
  virtual void* internalSubentPtr(const OdDbSubentId& id) const = 0;

  virtual bool getNurbCurvesCache(OdGeCurve3dPtrArray &)  const { return false; }

  // for internal use only
  virtual OdResult setBody(const void * /*pBody*/) { return eNotImplemented; }
  virtual void * body( ) const { return 0; }
  //DOM-IGNORE-END

  /** \details
    Sets the color of the specified face or edge subentity.

    \param subentId [in] ID of the subentity (face or edge) to be colored.
    \param color    [in] Color value to set.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setSubentColor(const OdDbSubentId &subentId, const OdCmColor &color) = 0;

  /** \details
    Returns the color of the specified subentity.

    \param subentId [in] ID of the subentity.
    \param color    [in] Reference to an OdCmColor object to store the current color of the specified subentity.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getSubentColor(const OdDbSubentId &subentId, OdCmColor &color) const = 0;

  /** \details
    Sets a material for the specified subentity.

    \param subentId [in] Subentity ID.
    \param matId    [in] Object ID for the material.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    To remove the currently assigned material, set the matId parameter value to
    NULL.
  */
  virtual OdResult setSubentMaterial(const OdDbSubentId &subentId, OdDbObjectId matId) = 0;

  /** \details
    Returns the material of the specified subentity.

    \param subentId [in] Subentity ID.
    \param matId   [out] Reference to an OdDbObjectId for returning the material object ID.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getSubentMaterial(const OdDbSubentId &subentId, OdUInt64& matId) const = 0;

  /** \details
    Sets a material mapper for the specified subentity.

    \param subentId      [in] Subentity ID.
    \param mx            [in] 3D transformation matrix.
    \param projection    [in] Projection type.
    \param tiling        [in] Tiling options.
    \param autoTransform [in] Auto transform options.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setSubentMaterialMapper( const OdDbSubentId &subentId,
                                            OdGeMatrix3d &mx,
                                            OdUInt8 &projection,
                                            OdUInt8 &tiling,
                                            OdUInt8 &autoTransform) = 0;

  /** \details
    Retrieves a material mapper for the specified subentity.

    \param subentId       [in] Subentity ID.
    \param mx            [out] 3D transformation matrix.
    \param projection    [out] Projection type.
    \param tiling        [out] Tiling options.
    \param autoTransform [out] Auto transform options.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getSubentMaterialMapper( const OdDbSubentId &subentId,
                                            OdGeMatrix3d &mx,
                                            OdUInt8 &projection,
                                            OdUInt8 &tiling,
                                            OdUInt8 &autoTransform) const = 0;

  /** \details
    Creates chamfers at the edges of a ModelerGeometry object.

    \param edgeSubentIds    [in] Array of edge object IDs. The chamfer will be applied at the specified edges.
    \param baseFaceSubentId [in] Base face object IDs. Chamfer will be applied on the specified face.
    \param baseDist         [in] Chamfer distance for the base face.
    \param otherDist        [in] Chamfer distance for the other faces.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult chamferEdges(const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdDbSubentId& baseFaceSubentId,
                                double baseDist,
                                double otherDist) = 0;

  /** \details
    Creates rounded fillets on the edges of a ModelerGeometry object.

    \param edgeSubentIds [in] Array of edges' object IDs. Fillet will be applied on the specified edges.
    \param radius        [in] Array of fillet radius values for the corresponding edges.
    \param startSetback  [in] Array of start setback values for the corresponding edges.
    \param endSetback    [in] Array of end setback values for the corresponding edges.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult filletEdges( const OdArray<OdDbSubentId *> &edgeSubentIds,
                                const OdGeDoubleArray& radius,
                                const OdGeDoubleArray& startSetback,
                                const OdGeDoubleArray& endSetback) = 0;

  /** \details
    Intersects the specified plane with this ModelerGeometry object and creates
    the intersection in a resulting array.

    \param sectionPlane    [in] Pointer to a plane object used for intersection (section cutting plane).
    \param sectionObjects [out] Reference to an entity array for representing resulting sections.

    \returns eOk if the intersection operation is successfully performed or an
    appropriate error code otherwise.
  */
  virtual OdResult createSectionObjects(const OdGePlane& sectionPlane, OdDbEntityPtrArray& sectionObjects) const = 0;

  /** \details
    Creates new entities that represent the result of interference between this
    ModelerGeometry object and a specified entity. Created entities are appended
    to the specified array.

    \param interferenceObjects [out] Output array of interference entities.
    \param pEntity              [in] Pointer to the entity the object must
                                     interfere with. pEntity can point to the
                                     following entity types: OdDbSurface,
                                     OdDb3dSolid, OdDbRegion, or OdDbBody.
    \param flags                [in] Interference flags; equal to kDefault (0x00).

    \returns eOk if interference entities are successfully created or an
    appropriate error code otherwise. For example, if interference entities are
    not created, eNoIntersection error code is returned. eInvalidInput is
    returned if an unsupported entity or NULL pointer is provided as a value of
    pEntity.
  */
  virtual OdResult createInterferenceObjects( OdDbEntityPtrArray& interferenceObjects,
                                              OdDbEntityPtr pEntity,
                                              unsigned int flags ) const = 0;

  /** \details
    Slices this ModelerGeometry object with the specified plane and sets the
    object to be the portion on the positive normal side plane.

    \param slicePlane       [in] Reference to a plane object to be used for slicing the ModelerGeometry object.
    \param pNegHalfSurface [out] Pointer to a new surface representing the negative side of the object that is sliced by the specified plane.
    \param pNewSurface     [out] Pointer to a new surface representing the positive half of the object that is sliced by the specified plane.
    \param bNotModifyItself [in] Boolean flag.

    \returns eOk if the slicing operation is successfully performed or an
    appropriate error code otherwise.
  */
  virtual OdResult sliceByPlane(const OdGePlane& slicePlane, OdDbSurfacePtr& pNegHalfSurface, OdDbSurfacePtr& pNewSurface, bool bNotModifyItself) = 0;

  /** \details
    Slices this ModelerGeometry object with the specified surface.

    \param pSlicingSurface  [in] Pointer to the surface to be used to slice this object.
    \param pNegHalfSurface [out] Pointer to a new surface object representing the other (negative) side of the sliced surface.
    \param pNewSurface     [out] Pointer to a new surface object representing the positive side of the sliced surface.
    \param bNotModifyItself [in] Boolean flag.

    \returns eOk if the slicing operation is successfully performed or an
    appropriate error code otherwise.
  */
  virtual OdResult sliceBySurface(const OdDbSurfacePtr pSlicingSurface, OdDbSurfacePtr& pNegHalfSurface, OdDbSurfacePtr& pNewSurface, bool bNotModifyItself) = 0;

  /** \details
    Extrudes faces of the ModelerGeometry object with specified distance and
    taper angle.

    \param faceSubentIds [in] Face's subentity ID in the drawing object.
    \param height        [in] Distance of extrusion.
    \param taper         [in] Taper angle.
  
    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    The distance of extrusion is determined by the height parameter and the
    taper angle (the taper parameter).

    The extrusion direction can be along or opposite the normal of each extruded
    face depending on the height value:
      * If the height is positive, the direction is along the normal (extruding
        faces outward from the drawing object).
      * If the height is negative, the direction is opposite the normal
        (extruding faces into the drawing object).

    If the absolute value of taper is less than 1e-6, taper angle is assumed to
    be equal to 0.0.

    If the taper value does not equal 0.0, each face should have edges that join
    together smoothly (i.e. tangents at the points of connection must be equal).

    The following constraints are applied:
      * The number of specified faces (in faceSubentIds parameter) must be more than zero
      * |height| >= 1e-6
      * |taper| >= (PI/2) - 1e-6
  */
  virtual OdResult extrudeFaces(const OdArray<OdDbSubentId *> &faceSubentIds, double height, double taper) = 0;
  
  /** \details
    Extrudes specified faces along the specified path determined by a curve
    entity.

    \param faceSubentIds [in] Array of faces' subentity IDs. Faces with specified IDs will be extruded.
    \param path          [in] Pointer to a curve object to extrude along (extrusion path). Cannot be NULL.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    Faces for extrusion must be planar.

    The curve entity must be one of the following:
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
  */
  virtual OdResult extrudeFacesAlongPath(const OdArray<OdDbSubentId *> &faceSubentIds, const OdDbCurve* path) = 0;

  /** \details
    Sets the parameter indicating whether faces are double-sided or not.

    \param isDoubleSided [in] True for double-sided faces; false for non-double-sided faces.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult ChangeFacesDoubleSidedParam(bool isDoubleSided) = 0;

  /** \details
    Converts this ModelerGeometry object to nurb surfaces.

    \param nurbSurfaceArray [in] Array of generated nurb surfaces.

    \returns eOk if converting operation was successfully finished or an
    appropriate error code otherwise.
  */
  virtual OdResult convertToNurbSurface(OdDbNurbSurfacePtrArray& nurbSurfaceArray) = 0;

  /** \details
    Retrieves parameters of this ModelerGeometry object.

    \param iUDegree           [out] Degree of the ModelerGeometry object in the U direction.
    \param iVDegree           [out] Degree of the ModelerGeometry object in the V direction.
    \param bRational          [out] Flag that indicates whether the ModelerGeometry object is rational.
    \param iUNumControlPoints [out] Quantity of control points in the U direction.
    \param iVNumControlPoints [out] Quantity of control points in the V direction.
    \param ctrlPtsArr         [out] Array of control points.
    \param weights            [out] Array of weight values.
    \param uKnots             [out] Knot vector in the U direction.
    \param vKnots             [out] Knot vector in the V direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult get( int& iUDegree, int& iVDegree, bool& bRational, int& iUNumControlPoints, int& iVNumControlPoints,
                        OdGePoint3dArray& ctrlPtsArr, OdGeDoubleArray& weights, 
                        OdGeKnotVector& uKnots, OdGeKnotVector& vKnots) const = 0;

  /** \details
    Sets parameters of this ModelerGeometry object.

    \param iUDegree           [in] Degree of the ModelerGeometry object in the U direction.
    \param iVDegree           [in] Degree of the ModelerGeometry object in the V direction.
    \param bRational          [in] Flag that indicates whether the ModelerGeometry object is rational.
    \param iUNumControlPoints [in] Quantity of control points in the U direction.
    \param iVNumControlPoints [in] Quantity of control points in the V direction.
    \param ctrlPtsArr         [in] Array of control points.
    \param weights            [in] Array of weight values.
    \param uKnots             [in] Knot vector in the U direction.
    \param vKnots             [in] Knot vector in the V direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult set (int iUDegree, int iVDegree, bool bRational, int iUNumControlPoints, int iVNumControlPoints,
                        const OdGePoint3dArray& ctrlPtsArr, const OdGeDoubleArray& weights, 
                        const OdGeKnotVector& uKnots, const OdGeKnotVector& vKnots) = 0;

  /** \details
    Returns the quantity of control points in the U direction.

    \param iCount [out] Passed-in parameter for returning the quantity of control points in the U direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfControlPointsInU(int& iCount) const = 0;

  /** \details
    Returns the quantity of control points in the V direction.

    \param iCount [out] Passed-in parameter for returning the quantity of control points in the V direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfControlPointsInV(int& iCount) const = 0;

  /** \details
    Returns the quantity of knots in the U direction.

    \param iCount [out] Passed-in parameter for returning the quantity of knots in the U direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfKnotsInU(int& iCount) const = 0;

  /** \details
    Returns the quantity of knots in the V direction.

    \param iCount [out] Passed-in parameter for returning the quantity of knots in the V direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfKnotsInV(int& iCount) const = 0;

  /** \details
    Returns the knot vector in the U direction.

    \param knots [out] Passed-in parameter for returning the knot vector in the U direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getUKnots(OdGeKnotVector& knots) const = 0;

  /** \details
    Returns the knot vector in the V direction.

    \param knots [out] Passed-in parameter for returning the knot vector in the v direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getVKnots(OdGeKnotVector& knots) const = 0;

  /** \details
    Returns the degree of the ModelerGeometry object in the U direction.

    \param iDegree [out] Passed-in parameter for returning the degree in the U direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getDegreeInU(int& iDegree) const = 0;

  /** \details
    Returns the degree of the ModelerGeometry object in the V direction.

    \param iDegree [out] Passed-in parameter for returning the degree in the V direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getDegreeInV(int& iDegree) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is closed in the U direction.

    \param bIsClosed [out] Passed-in parameter for returning the indicator of
                           whether the ModelerGeometry object is closed in the U
                           direction. bIsClosed is true if the ModelerGeometry
                           object is closed in the U direction, otherwise it is
                           false.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult isClosedInU(bool& bIsClosed) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is closed in the V direction.

    \param bIsClosed [out] Passed-in parameter for returning the indicator of
                           whether the ModelerGeometry object is closed in the V
                           direction. bIsClosed is true if the ModelerGeometry
                           object is closed in the V direction, otherwise it is
                           false.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult isClosedInV(bool& bIsClosed) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is periodic in the U
    direction.

    \param bIsPeriodic [out] Passed-in parameter for returning the indicator of
                             whether the ModelerGeometry object is periodic in
                             the U direction. bIsPeriodic is true if the
                             ModelerGeometry object is periodic in the U
                             direction, otherwise it is false.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult isPeriodicInU(bool& bIsPeriodic) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is periodic in the V
    direction.

    \param bIsPeriodic [out] Passed-in parameter for returning the indicator of
                             whether the ModelerGeometry object is periodic in
                             the V direction. bIsPeriodic is true if the
                             ModelerGeometry object is periodic in the V
                             direction, otherwise it is false.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult isPeriodicInV(bool& bIsPeriodic) const = 0;

  /** \details
    Returns the period value in the U direction for the ModelerGeometry object.
    Returned value is valid only if the ModelerGeometry object is periodic in
    the U direction.

    \param dPeriod [out] Passed-in parameter for returning the period value in the U direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getPeriodInU(double& dPeriod) const = 0;

  /** \details
    Returns the period value in the V direction for the ModelerGeometry object.
    Returned value is valid only if the ModelerGeometry object is periodic in
    the V direction.

    \param dPeriod [out] Passed-in parameter for returning the period value in the V direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getPeriodInV(double& dPeriod) const = 0;

  /** \details
    Evaluates the position on the ModelerGeometry object at the specified
    parameter value (U, V).

    \param dU   [in] U direction parameter.
    \param dV   [in] V direction parameter.
    \param pos [out] Passed-in parameter for returning the evaluated position on the ModelerGeometry object.

    \returns eOk if evaluation is successfully done. If the U or V value is out
    of range, returns eOutOfRange.
  */
  virtual OdResult evaluate(double dU, double dV, OdGePoint3d& pos) const = 0;

  /** \details
    Evaluates the position and first derivatives on the ModelerGeometry object
    at the specified parameter value (U, V).

    \param dU      [in] U direction parameter.
    \param dV      [in] V direction parameter.
    \param pos    [out] Passed-in parameter for returning the evaluated position on the ModelerGeometry object.
    \param uDeriv [out] Passed-in parameter for returning the first derivative with respect to U.
    \param vDeriv [out] Passed-in parameter for returning the first derivative with respect to V.

    \returns eOk if evaluation is successfully done. If the U or V value is out
    of range, returns eOutOfRange.
  */
  virtual OdResult evaluate(double dU, double dV, OdGePoint3d& pos, OdGeVector3d& uDeriv, OdGeVector3d& vDeriv) const = 0;

  /** \details
    Evaluates the position and first and second derivatives on the
    ModelerGeometry object at the specified parameter value (U, V).

    \param dU       [in] U direction parameter.
    \param dV       [in] V direction parameter.
    \param pos     [out] Passed-in parameter for returning the evaluated position on the surface.
    \param uDeriv  [out] Passed-in parameter for returning the first derivative with respect to U.
    \param vDeriv  [out] Passed-in parameter for returning the first derivative with respect to V.
    \param uuDeriv [out] Passed-in parameter for returning the second derivative with respect to U twice.
    \param uvDeriv [out] Passed-in parameter for returning the second derivative with respect to U and V.
    \param vvDeriv [out] Passed-in parameter for returning the second derivative with respect to V twice.

    \returns eOk if evaluation is successfully done. If the U or V value is out
    of range, returns eOutOfRange.
  */
  virtual OdResult evaluate(double dU, double dV, OdGePoint3d& pos, OdGeVector3d& uDeriv, OdGeVector3d& vDeriv,
                            OdGeVector3d& uuDeriv, OdGeVector3d& uvDeriv, OdGeVector3d& vvDeriv) const = 0;

  /** \details
    Evaluates the position and an arbitrary number of derivatives of the
    ModelerGeometry object at the specified parameter value (U, V).

    \param dU           [in] U direction parameter.
    \param dV           [in] V direction parameter.
    \param iDerivDegree [in] Degree of derivatives to evaluate.
    \param point       [out] Passed-in parameter for returning the evaluated position on the ModelerGeometry object.
    \param derivatives [out] Passed-in parameter for returning derivatives array
                             of vectors, in the order of uDeriv, vDeriv,
                             uuDeriv, uvDeriv, vvDeriv.

    \returns eOk if evaluation is successfully done. If the U or V value is out
    of range, returns eOutOfRange.
  */
  virtual OdResult evaluate(double dU, double dV, int iDerivDegree, OdGePoint3d& point, OdGeVector3dArray& derivatives) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is rational. If the
    ModelerGeometry object is rational, the method returns true, otherwise the
    method returns false.

    \param bIsRational [out] Passed-in parameter for returning the indicator of whether the ModelerGeometry object is rational.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult isRational(bool& bIsRational) const = 0;

  /** \details
    Determines whether the ModelerGeometry object is a planar surface. If the
    surface is planar, also returns the plane information and normal. If the
    surface is planar, the method returns true via the bIsPlanar parameter,
    otherwise the method returns false.

    \param bIsPlanar   [out] Passed-in parameter for returning the indicator of whether the surface is planar.
    \param ptOnSurface [out] Passed-in parameter for returning the point on the surface (if the surface is planar).
    \param normal      [out] Passed-in parameter for returning the plane normal (if the surface is planar).

    \returns eOk if successful. If the surface doesn't contain any NURBS
    information, returns eFail.
  */
  virtual OdResult isPlanar(bool& bIsPlanar, OdGePoint3d& ptOnSurface, OdGeVector3d& normal) const = 0;

  /** \details
    Determines whether a specified point is on the ModelerGeometry object. If
    the specified point is on the ModelerGeometry object, the method returns
    true via bOnSurface, otherwise the method returns false.

    \param point       [in] Point.
    \param bOnSurface [out] Passed-in parameter for returning the indicator of whether the specified point is on the surface.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult isPointOnSurface(const OdGePoint3d& point, bool& bOnSurface) const = 0;

  /** \details
    Returns the normal vector at the specified parameter location ([U, V]).

    \param dU      [in] U parameter.
    \param dV      [in] V parameter.
    \param normal [out] Passed-in parameter for returning the normal vector.

    \returns eOk if successful. If the dU or dV values are invalid, returns
    eOutOfRange.
  */
  virtual OdResult getNormal(double dU, double dV, OdGeVector3d& normal) const = 0;

  /** \details
    Returns the simple patches in the U direction.

    \param iSpan [out] Passed-in parameter for returning the number of simple patches in the U direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfSpansInU(int& iSpan) const = 0;

  /** \details
    Returns the simple patches in the V direction.

    \param iSpan [out] Passed-in parameter for returning the number of simple patches in the V direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getNumberOfSpansInV(int& iSpan) const = 0;

  /** \details
    Returns the specified number of isolines created in the V direction.

    \param dNumberSegments [in] Number of isoline segments.
    \param lineSegments   [out] Array of isoline segments.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getIsolineAtU(double dNumberSegments, OdDbCurvePtrArray& lineSegments) const = 0;

  /** \details
    Returns the specified number of isolines created in the U direction.

    \param dNumberSegments [in] Number of isoline segments.
    \param lineSegments   [out] Array of isoline segments.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getIsolineAtV(double dNumberSegments, OdDbCurvePtrArray& lineSegments) const = 0;

  /** \details
    Inserts a knot at the specified U or V parameter. The knot insertion doesn't
    modify the shape of the ModelerGeometry object.

    \param dVal  [in] New knot.
    \param iUorV [in] U or V parameter.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult InsertKnot(double dVal, int iUorV) = 0;

  /** \details
    Inserts a row of control points at the given U knot parameter.

    \param dU       [in] U parameter.
    \param vCtrlPts [in] Array of control points to be added.
    \param vWeights [in] Array of weight values to add if the surface is rational.

    \returns eOk if successful. If the dU parameter is out of range, returns
    eOutOfRange.
  */
  virtual OdResult InsertControlPointsAtU(double dU, const OdGePoint3dArray& vCtrlPts, const OdGeDoubleArray& vWeights) = 0;

  /** \details
    Inserts a row of control points at the given V knot parameter.

    \param dV       [in] V parameter.
    \param uCtrlPts [in] Array of control points to be added.
    \param uWeights [in] Array of weight values to add if the surface is rational.

    \returns eOk if successful. If the dU parameter is out of range, returns
    eOutOfRange.
  */
  virtual OdResult InsertControlPointsAtV(double dV, const OdGePoint3dArray& uCtrlPts, const OdGeDoubleArray& uWeights) = 0;

  /** \details
    Removes a row of control points at the specified position in the control
    points array. The method modifies the shape of the ModelerGeometry object.

    \param iUDegree [in] Index of the control point to remove from the array.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult RemoveControlPointsAtU(int iUDegree) = 0;

  /** \details
    Removes a row of control points at the specified position in the control
    points array. The method modifies the shape of the ModelerGeometry object.

    \param iVDegree [in] Index of the control point to remove from the array.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult RemoveControlPointsAtV(int iVDegree) = 0;

  /** \details
    Rebuilds this ModelerGeometry object with a specified new degree and
    quantity of control points in the U and V directions. This operation
    modifies the shape of the ModelerGeometry object.

    \param iUDegree     [in] New degree value in the U direction.
    \param iVDegree     [in] New degree value in the V direction.
    \param iNumUCtrlPts [in] New quantity of control points in the U direction.
    \param iNumVCtrlPts [in] New quantity of control points in the V direction.
    \param bRestore     [in] Restore flag.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult rebuild(int iUDegree, int iVDegree, int iNumUCtrlPts, int iNumVCtrlPts, bool bRestore) = 0;

  /** \details
    Adjusts the location and tangent of a point on this ModelerGeometry object
    with the specified parameter ([U, V]). The local control points are adjusted
    accordingly. If the pointers uDeriv and vDeriv are equal to NULL, then only
    the location of the point is adjusted.

    \param dU     [in] U parameter.
    \param dV     [in] V parameter.
    \param point  [in] New location of the point on the surface.
    \param uDeriv [in] Tangent vector in the U direction.
    \param vDeriv [in] Tangent vector in the V direction.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult modifyPositionAndTangent(double dU, double dV, const OdGePoint3d& point, const OdGeVector3d* uDeriv, const OdGeVector3d* vDeriv) = 0;

  /** \details
    Returns the U and V parameter for a point on this ModelerGeometry object.

    \param point [in] Point on the ModelerGeometry object.
    \param dU    [in] Passed-in parameter for returning the value of the U parameter.
    \param dV    [in] Passed-in parameter for returning the value of the V parameter.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getParameterOfPoint(const OdGePoint3d& point, double& dU, double& dV) const = 0;

  /** \details
    Returns the control points and the quantity of control points in both U and
    V directions.

    \param iUCount [out] Quantity of control points in the U direction.
    \param iVCount [out] Quantity of control points in the V direction.
    \param points  [out] Array of control points (in WCS coordinates).

    \returns eOk if control points were successfully returned, or an appropriate
    error code otherwise.

    \remarks
    The control points will be allocated in the passed-in OdGePoint3d array. The
    column indices are for the V direction, and row indices are for the U
    direction. For example, if the surface has n control points in the U
    direction and m control points in the V direction, the array looks like
    [0,0], [0,1], ...[0,vCount-1], ..., [uCount-1,0], [uCount-1,1], ...[uCount-1,vCount-1].
  */
  virtual OdResult getControlPoints(int& iUCount, int& iVCount, OdGePoint3dArray& points) const = 0;

  /** \details
    Sets the control points.

    \param iUCount [in] New value of the quantity of control points in the U direction.
    \param iVCount [in] New value of the quantity of control points in the V direction.
    \param points  [in] New array of control points (in WCS coordinates).

    \returns eOk if control points were successfully set or an appropriate error
    code otherwise.

    \remarks
    New quantity of control points in the U and V direction must be equal to the
    current values. New control points are allocated in an AcGePoint3d array.
    The column indices are for the V direction, and row indices are for the U
    direction. For example, if the surface has n control points in the U
    direction and m control points in the V direction, the array looks like
    [0,0], [0,1], ...[0,vCount-1], ..., [uCount-1,0], [uCount-1,1], ...[uCount-1,vCount-1].
  */
  virtual OdResult setControlPoints(int iUCount, int iVCount, const OdGePoint3dArray& points) = 0;

  /** \details
    Retrieves the control point and weight on the ModelerGeometry object by the
    specified indices (U, V).

    \param iUIndex      [in] U direction index.
    \param iVIndex      [in] V direction index.
    \param point       [out] Control point.
    \param weight      [out] Weight value.
    \param bIsRational [out] Flag determining whether this control point is rational.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getControlPointAndWeight(int iUIndex, int iVIndex, OdGePoint3d& point, double& weight, bool& bIsRational) const = 0;

  /** \details
    Sets the control point and weight on the ModelerGeometry object by the
    specified indices (U, V).

    \param iUIndex [in] U direction index.
    \param iVIndex [in] V direction index.
    \param point   [in] Control point.
    \param weight  [in] Weight value.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setControlPointAndWeight(int iUIndex, int iVIndex, const OdGePoint3d& point, double weight) = 0;

  /** \details
    Sets the new color for subentities.

    \param color [in] New color value.

    \returns eOk if the new color value was successfully set or an appropriate
    error code otherwise.
  */
  virtual OdResult setColorToSubents(OdCmColor const& color) = 0;

  /** \details
    Sets the material for subentities.

    \param materialId [in] New material.

    \returns eOk if the new material value was successfully set or an
    appropriate error code otherwise.
  */
  virtual OdResult setMaterialToSubents(OdDbObjectId materialId) = 0;

  /** \details
    This method is not implemented.

    \returns eNotImplemented.
  */
  virtual OdResult setMaterialResolver(const OdMaterialResolver *pResolver) { return eNotImplemented; }

  /** \details
    Sets the material mapper for subentities.

    \param mx            [in] 3D transformation matrix.
    \param projection    [in] Projection type.
    \param tiling        [in] Tiling options.
    \param autoTransform [in] Auto transform options.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult setMaterialMapperToSubents(OdGeMatrix3d &mx, OdUInt8 &projection,
                                              OdUInt8 &tiling, OdUInt8 &autoTransform) = 0;

  /** \details 
    For internal use only.
  */
  virtual OdResult generateSectionGeometry(const OdDbSection*      pSection,
                                           OdDbEntityPtrArray&     sourceEntArr,
                                           OdArray<OdDbEntityPtr>& intBoundaryEnts,
                                           OdArray<OdDbEntityPtr>& intFillEnts,
                                           OdArray<OdDbEntityPtr>& backgroundEnts,
                                           OdArray<OdDbEntityPtr>& foregroundEnts,
                                           OdArray<OdDbEntityPtr>& curveTangencyEnts,
                                           bool                    bIsSetProperties) = 0;

  /** \details
    Trims the surface with specified cutting curves. When curves are used, the
    actual trimming boundary is formed by projecting the curves onto the
    surface, so the additional information of project direction needs to be
    provided.

    \param toolIds             [in] Reference to an array of the cutting
                                    entity's IDs. Bodies of specified entities
                                    are used to trim the specified surface.
    \param toolCurveIds        [in] Reference to an array of the cutting curve's
                                    IDs. The specified surface will be trimmed
                                    by projecting curves to it.
    \param projVectors         [in] Reference to an array of projection direction for each cutting curve.
    \param pickPoint           [in] Pick point that specifies which area of the specified surface must be trimmed.
    \param viewVector          [in] Vector representing the view direction for creating a ray starting from the pick point.
    \param bAutoExtend         [in] If this option is set, then when a tool body
                                    consists of a single face with analytic
                                    geometry, the underlying geometry will be
                                    extended as much as possible to make sure
                                    the surface is trimmed. The tool body
                                    supplied will not be modified.
    \param bAssociativeEnabled [in] Specifies whether the surface trimming operation should be associative.

    \returns eOk if trimming operation was successfully finished or an
    appropriate error code otherwise.
  */
  virtual OdResult trimSurface( const OdDbObjectIdArray& toolIds,
                                const OdDbObjectIdArray& toolCurveIds,
                                const OdGeVector3dArray& projVectors,
                                const OdGePoint3d&       pickPoint,
                                const OdGeVector3d&      viewVector,
                                bool                     bAutoExtend,
                                bool                     bAssociativeEnabled) = 0;

  /** \details
    Creates entities by projecting a specified entity onto the surface.

    \param pEntityToProject    [in] Entity to project.
    \param projectionDirection [in] Projection direction vector.
    \param projectedEntities  [out] Array of projected entities.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult projectOnToEntity( const OdDbEntity*   pEntityToProject,
                                      const OdGeVector3d& projectionDirection,
                                      OdDbEntityPtrArray& projectedEntities ) const = 0;

  /** \details
    Creates a solid by trimming and (or) extending the specified surfaces and 3D
    solids, forming an enclosed volume.

    \param limitingBodies [in] Reference to the entity array (representing surface and solids) that are used to create a solid.
    \param limitingFlags  [in] Reserved for internal use.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult createSculptedSolid(OdDbEntityPtrArray& limitingBodies,
                                       const OdGeIntArray& limitingFlags) = 0;

  /** \details
    Creates a ray which starts from a specified point and has a specified
    direction.

    \param rayBasePoint [in] Base point to start a ray from.
    \param rayDir       [in] Ray direction.
    \param rayRadius    [in] Ray radius. It defines the tolerance used for intersection checking.
    \param subEntIds   [out] Reference to subentities array to return. Returned subentities can be one of the following: faces, edges, or vertices.
    \param parameters  [out] Array of parameters to return. Parameters shows the intersection between a ray and subentities.

    \returns eOk if a ray was successfully created or an appropriate error code
    otherwise.
  */
  virtual OdResult rayTest( const OdGePoint3d&      rayBasePoint,
                            const OdGeVector3d&     rayDir,
                            double                  rayRadius,
                            OdArray<OdDbSubentId> & subEntIds,
                            OdGeDoubleArray&        parameters) const = 0;

  /** \details
    Creates a tangent surface between two edges of a surface with constant
    radius and trimming or no trimming of the original surfaces.

    \param surf1   [out] Pointer to the first source surface.
    \param pickPt1  [in] 3D point specifying the side on the first source surface that will be kept after filleting.
    \param surf2   [out] Pointer to the second source surface.
    \param pickPt2  [in] 3D point specifying the side on the second source surface that will be kept after filleting.
    \param dRadius  [in] Radius for fillet operation (positive value).
    \param trimMode [in] Flag that specifies the OdDb::FilletTrimMode value.
    \param projDir  [in] 3D vector specifying the projection (view) direction for pick points.

    \returns eOk if successful or an appropriate error code otherwise.

    \remarks
    The trimMode can be one of the following:
      * kTrimNone
      * kTrimFirst
      * kTrimSecond
      * kTrimBoth
  */
  virtual OdResult createFilletSurface (OdDbSurfacePtr&      surf1,
                                        const OdGePoint3d&   pickPt1,
                                        OdDbSurfacePtr&      surf2,
                                        const OdGePoint3d&   pickPt2,
                                        double               dRadius,
                                        OdDb::FilletTrimMode trimMode,
                                        const OdGeVector3d&  projDir) = 0;

  /** \details
    Extends the edges of this ModelerGeometry object.

    \param edgesId  [out] Array of full sub-entity paths of the edges that need to be extended.
    \param dExtDist  [in] Distance at which the ModelerGeometry object should be expanded.
    \param extOption [in] Extension option, determining whether edges need to be extended or stretched.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult extendEdges( OdDbFullSubentPathArray& edgesId,
                                double                   dExtDist,
                                EdgeExtensionType        extOption ) = 0;

  /** \details
    Gets the mesh of this ModelerGeometry object.

    \param faceter      [in] Faceter settings.
    \param vertexArray [out] Array of vertices.
    \param faceArray   [out] Array of faces.
    \param faceData    [out] Face data.

    \returns eOk if successful or an appropriate error code in the other case.
  */
  virtual OdResult getObjectMesh( const OdDbFaceterSettings *faceter,
                                  OdGePoint3dArray& vertexArray,
                                  OdInt32Array& faceArray,
                                  OdGiFaceData*& faceData ) = 0;
                                  
  /** \details
    Returns the bounding box.

    \param box [out] 3D bounding box as minimum and maximum 3D points.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult getBoundingBox(OdGeExtents3d &box) = 0;

  /** \details
    For internal use only.
  */
  virtual OdResult generateSectionGeometry( SectArgs& sectArgs, OdDbEntity *pEnt, bool* bHasForeground ) = 0;

  /** \details
    This function generates a data to show ViewSection. Sectionable entities
    include Db3dSolid, DbSurface, DbBody, and DbRegion.

    \param pEnts                 [in] Array of the sectionable entities.
    \param pPoints               [in] Input vertex points on the section line; should contain at least two points.
    \param normal                [in] Input vector on the first segment's plane, normal to the section line.
    \param backgroundEnts       [out] Array of resut entities after señtion array of entities pEnts.
    \param intHatchEnts         [out] Array of planes section of resuts entities backgroundEnts.
    \param intBoundaryEnts      [out] Array of intHatchEnt boundaries.
    \param curveVisibilityEdges [out] Array of visibility edges to draw the view section.
    \param sectType              [in] Section type.
    \param edgeVis               [in] Specifies the visibility of tangent, interference and other some tipes of edges.

    \returns eOk if successful. Returns an error status if the function fails to
    open the objector or if the plane and the solid do not intersect.

    \remarks
    This method is implemented for Spatial modeler only and returns the
    eNotImplementedYet status for other modelers.
  */
  virtual OdResult getSectionViewInfo(const OdArray<OdDbEntityPtr>& pEnts, const OdGePoint3dArray& pPoints, const OdGeVector3d& normal,  OdArray<OdDbEntityPtr>& backgroundEnts, OdArray<OdDbEntityPtr>& intHatchEnts, OdArray<OdDbEntityPtr>& intBoundaryEnts, OdArray<OdDbEntityPtr>& curveVisibilityEdges, SectionType sectType = kFull_ST, OdEdgeTypeFlags edgeVis = kTangent) = 0;

  /** \details
    Creates cached curves.

    \param pCurves [out] Array of Curve3dPtr objects.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult createCachedCurves(OdGeCurve3dPtrArray& pCurves) = 0;

  /** \details
    Restores attributes from old ModelerGeometry object.

    \param oldmodeler [out] ModelerGeometry object to restore attributes.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult restoreAttributes(OdModelerGeometry* oldmodeler) = 0;

  /** \details
    Adds special settings to a subentity.

    \param addedDT      [out] Added ACIS data type.
    \param addedDTFirst [out] Added ACIS data type.
    \param subentId      [in] Subentity ID.
    \param retArray      [in] Settings data array.
    \param idx           [in] Settings index.

    \returns eOk if successful or an appropriate error code otherwise.
  */
  virtual OdResult addSubentitySpecialSettings(AcisDataType addedDT, AcisDataTypeArrFirst addedDTFirst, const OdDbSubentId subentId, OdArray<OdUInt32>& retArray, OdUInt32 idx) = 0;

  /** \details
    Creates a new OdDb3dSolid or OdDbSurface object.

    \param arrVertexes       [in] Array containing vertices.
    \param arrEdges          [in] Array containing vertex indices.
    \param arrFaces          [in] Array containing faces defined by loops.
    \param fd                [in] Face attributes (materials, colors).
    \param arrFacesColors    [in] True at a particular index means that the face has colors.
    \param arrFacesMaterials [in] True at a particular index means that the face has material.
    \param pEntity          [out] Created entity.

    \returns
    * eOk &mdash; if entity created successfully.
    * eInvalidContext &mdash; if it is not possible to create an OdDb3dSolid.
    * eInvalidInput &mdash; if any other error occured.
  */
  virtual OdResult convertTo(const OdGePoint3dArray &arrVertexes, const OdInt32Array &arrEdges, const OdInt32Array &arrFaces, OdGiFaceData &fd,
    OdBoolArray &arrFacesColors, OdBoolArray &arrFacesMaterials, OdDbEntity* pEntity) = 0;

  /** \details
    Gets a mesh of the face.

    \param mesh               [out] Resulting mesh.
    \param iFace               [in] Face GS marker.
    \param triangulationParams [in] Triangulation parameters.

    \returns Thue if successful or false otherwise.
  */
  virtual bool getFaceMesh(GeMesh::OdGeTrMesh& mesh, OdGsMarker iFace, const wrTriangulationParams& triangulationParams) = 0;

  /** \details
    This method is not implemented.

    \returns eNotImplemented.
  */
  virtual OdResult auditAcisData(OdDbAuditInfo* pInfo, const OdRxObject* pObj) { return eNotImplemented; }

  /** \details
    Enables or disables ACIS data audit.

    \param bEnable [in] True to enable ACIS data audit; false to disable ACIS data audit.
  */
  virtual void enableAcisAudit(bool bEnable) = 0;
};

/** \details
  This template class is a specialization of the OdSmartPtr class for
  OdModelerGeometry object pointers.
*/
typedef OdSmartPtr<OdModelerGeometry> OdModelerGeometryPtr;

/** \details
  This template class is a specialization of the OdArray class for
  OdModelerGeometryPtr object pointers.
*/
typedef OdArray<OdModelerGeometryPtr> OdModelerGeometryPtrArray;
#include "TD_PackPop.h"

#endif // _OD_MODELERGEOMETRY_INCLUDED_
