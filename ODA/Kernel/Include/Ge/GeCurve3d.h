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

#ifndef _OD_GECURVE3D_H_
#define _OD_GECURVE3D_H_ /*!DOM*/

#include "Ge/GeEntity3d.h"
#include "Ge/GeDoubleArray.h"
#include "Ge/GeVector3dArray.h"
#include "Ge/GeCurve3dPtrArray.h"
#include "Ge/GeIntArray.h"
#include "Ge/GeInterval.h"
#include "Ge/GeBoundBlock3d.h"
class OdGeCurve2d;
class OdGeSurface;
class OdGePoint3d;
class OdGePlane;
class OdGeVector3d;
class OdGeLinearEnt3d;
class OdGeLine3d;
class OdGePointOnCurve3d;
class OdGePointOnSurface;
class OdGeMatrix3d;
class OdGePointOnCurve3dData;

#include "TD_PackPush.h"

/** \details
  This class is the base class for all OdGe 3D curves.

  Corresponding C++ library: TD_Ge

  <group OdGe_Classes>
*/
class GE_TOOLKIT_EXPORT OdGeCurve3d : public OdGeEntity3d
{
public:

  /** \details
    Gets the parametric interval of this curve.

    \param interval [out]  Receives the parametric interval of this curve.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getInterval(
    OdGeInterval& interval) const;

  /** \details
    Gets the parametric interval of this curve.

    \param interval [out]  Receives the parametric interval of this curve.
    \param start    [out]  Receives the start point of the interval.
    \param end      [out]  Receives the end point of the interval. 

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getInterval(
    OdGeInterval& interval,
    OdGePoint3d& start,
    OdGePoint3d& end) const;

  /** \details
    Reverses the parameter direction this curve.

    \returns Curve with the reversed direction parameter.
    \remarks
    The point set of this curve is unchanged.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.

    Throws:
    <table>
    Exception
    eNotImplemented
    </table>
  */
  OdGeCurve3d& reverseParam();

  /** \details
    Sets to natural domain of this curve.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.

    Throws:
    <table>
    Exception
    eNotImplemented
    </table>
  */
  OdGeCurve3d& setInterval();

  /** \details
    Sets the parametric interval of this curve.

    \param interval [in]  Parametric interval of this curve.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.

    Throws:
    <table>
    Exception
    eNotImplemented
    </table>
    \returns
    true if interval is successfully set, false otherwise.
  */
  bool setInterval(
    const OdGeInterval& interval);

  /** \details
    Returns the distance to the point on this curve closest to the specified
    point.

    \param point [in]  Any 3D point.
    \param tol   [in]  Geometric tolerance.
  */
  double distanceTo(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the distance to the point on this curve closest to the specified
    curve.

    \param curve [in]  Any 3D curve.
    \param tol   [in]  Geometric tolerance.
  */
  double distanceTo(
    const OdGeCurve3d& curve,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve closest to the specified point.

    \param point [in]  Any 3D point.
    \param tol   [in]  Geometric tolerance.
  */
  OdGePoint3d closestPointTo(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve closest to the specified curve, and the
    point on the other curve closest to this curve.

    \param curve          [in]  Any 3D curve.
    \param pntOnOtherCrv [out]  Receives the closest point on other curve.
    \param tol            [in]  Geometric tolerance.
  */
   OdGePoint3d closestPointTo(
    const OdGeCurve3d& curve,
    OdGePoint3d& pntOnOtherCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve closest to the specified point.

    \param point  [in]  Any 3D point.
    \param param [out]  uv-coord of closest point.
    \param range  [in]  interval of closest point.
    \param tol    [in]  Geometric tolerance.

    \remarks
    Native interval is not considered.
    If projection of the point does not lie in range-interval, the closest bound
    of the interval is returned. In case of uvBox == NULL
    closestPointTo(point, range, tol) is called where range is a native curves
    interval. An unbounded interval should not be used here.
  */
  OdGePoint3d closestPointTo(
    const OdGePoint3d & point,
    double & param,
    const OdGeInterval* range,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve closest to the specified point.

    \param point     [in]  Any 3D point.
    \param pntOnCrv [out]  Receives the closest point on this curve.
    \param tol       [in]  Geometric tolerance.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getClosestPointTo(
    const OdGePoint3d& point,
    OdGePointOnCurve3d& pntOnCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve closest to the specified curve, and the
    point on the other curve closest to this curve.

    \param curve          [in]  Any 3D curve.
    \param pntOnThisCrv  [out]  Receives the closest point on this curve.
    \param pntOnOtherCrv [out]  Receives the closest point on other curve.
    \param tol            [in]  Geometric tolerance.
  */
  void getClosestPointTo(
    const OdGeCurve3d& curve,
    OdGePointOnCurve3d& pntOnThisCrv,
    OdGePointOnCurve3d& pntOnOtherCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve closest to the specified point when this
    curve is projected in the specified direction.

    \param point            [in]  Any 3D point.
    \param projectDirection [in]  Projection direction.
    \param tol              [in]  Geometric tolerance.
  */
  OdGePoint3d projClosestPointTo(
    const OdGePoint3d& point,
    const OdGeVector3d& projectDirection,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve closest to the specified curve, and the
    point on the other curve closest to this curve, when this curve is projected
    in the specified direction.

    \param curve            [in]  Any 3D curve.
    \param projectDirection [in]  Projection Direction.
    \param pntOnOtherCrv   [out]  Receives the closest point on other curve.
    \param tol              [in]  Geometric tolerance.
  */
  OdGePoint3d projClosestPointTo(
    const OdGeCurve3d& curve,
    const OdGeVector3d& projectDirection,
    OdGePoint3d& pntOnOtherCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve closest to the specified point when this
    curve is projected in the specified direction.

    \param point            [in]  Any 3D point.
    \param projectDirection [in]  Projection Direction.
    \param pntOnCrv        [out]  Receives the closest point on this curve.
    \param tol              [in]  Geometric tolerance.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.

    Throws:
    <table>
    Exception
    eNotImplemented
    </table>
  */
  void getProjClosestPointTo(
    const OdGePoint3d& point,
    const OdGeVector3d& projectDirection,
    OdGePointOnCurve3d& pntOnCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve closest to the specified curve, and the
    point on the other curve closest to this curve, when this curve is projected
    in the specified direction.

    \param curve            [in]  Any 3D curve.
    \param projectDirection [in]  Projection Direction.
    \param pntOnThisCrv    [out]  Receives the closest point on this curve.
    \param pntOnOtherCrv   [out]  Receives the closest point on other curve.
    \param tol              [in]  Geometric tolerance.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.

    Throws:
    <table>
    Exception
    eNotImplemented
    </table>
  */
  void getProjClosestPointTo(
    const OdGeCurve3d& curve,
    const OdGeVector3d& projectDirection,
    OdGePointOnCurve3d& pntOnThisCrv,
    OdGePointOnCurve3d& pntOnOtherCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true, and the point on this curve whose normal vector passes through
    the specified point, if and only if a normal point was found.

    \param point     [in]  Any 3D point.
    \param pntOnCrv [out]  Receives the normal point.
    \param tol       [in]  Geometric tolerance.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.

    Throws:
    <table>
    Exception
    eNotImplemented
    </table>
  */
  bool getNormalPoint(
    const OdGePoint3d& point,
    OdGePointOnCurve3d& pntOnCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Calculates the oriented bounding box of the curve.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.

    \returns
    Bounding box as an OdGeBoundBlock2d entity.
  */
  OdGeBoundBlock3d boundBlock() const;

  /** \details
    Calculates the oriented bounding box of curve.

    \param range [in]  Sub-interval of this curve to be bounded.

    \returns
    Bounding box as a OdGeBoundBlock3d entity.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  OdGeBoundBlock3d boundBlock(
    const OdGeInterval& range) const;

  /** \details
    Calculates the bounding box whose edges are aligned with the coordinate axes.

    \returns
    Bounding box as a OdGeBoundBlock3d entity.
  */
   OdGeBoundBlock3d orthoBoundBlock() const;

   /** \details
     Calculates the bounding box whose edges are aligned with the coordinate axes.

     \param range [in]  Interval of this curve to be bounded.

     \returns
     Bounding box as an OdGeBoundBlock3d entity.
   */
   OdGeBoundBlock3d orthoBoundBlock(
    const OdGeInterval& range) const;

  /** \details
    Returns the bounding box of the whole curve or a part of it. The box is
    axis-aligned in the specified local coordinate system. It is guaranteed to
    enclose the curve, but not guaranteed to be tight/minimal.

    \param range       [in]  The part of the curve to be bounded.
    \param coordSystem [in]  The local coordinate system in which box extents are computed.

    \remarks
    If the range is unbounded (e.g. by default), then the bounding box of the
    whole curve (which range can be obtained by getInterval method) is computed.
    Otherwise the bounding box is computed only for the set of points with
    parameter in the specified range. If coordSystem is NULL (e.g. by default),
    than the bounding box is aligned with global coordinate axes. Otherwise, the
    box will be axis-aligned in the local coordinate system specified by the
    coordSystem matrix.
  */
  OdGeExtents3d getGeomExtents(
     const OdGeInterval& range = OdGeInterval(),
     const OdGeMatrix3d* coordSystem = NULL) const;

  /** \details
    Returns the entity that is the projection of the curve onto the projection
    plane in the specified direction.
    \param projectionPlane  [in]  Plane on which the curve is to be projected.
    \param projectDirection [in]  Direction in which the curve is to be projected.
    \param tol              [in]  Geometric tolerance.
    \remarks
    The projectDirection vector must not be parallel to the projection plane.
    \remarks
    The returned entity can be a different type than the original curve and can
    even not be a curve.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  OdGeEntity3d* project(
    const OdGePlane& projectionPlane,
    const OdGeVector3d& projectDirection,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the entity that is the orthogonal projection of the curve onto the
    projection plane.
    \param projectionPlane [in]  Plane on which the curve is to be projected.
    \param tol             [in]  Geometric tolerance.
    \remarks
    The returned entity can be a different type than the original curve and can
    even not be a curve.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  OdGeEntity3d* orthoProject(
    const OdGePlane& projectionPlane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  //DOM-IGNORE-BEGIN
  // Tests if point is on *curve*.
  TD_USING(OdGeEntity3d::isOn);
  //DOM-IGNORE-END

  /** \details
    Determines whether a point lies on the curve.
    \param point  [in]  Input point.
    \param param [out]  Output parameter value of the point if the point lies on the curve.
    \param tol    [in]  Geometric tolerance.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool isOn(
    const OdGePoint3d& point,
    double& param,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines whether an input parameter value is within a valid parametric
    range of the curve.
    \param param [in]  Input parameter value.
    \param tol   [in]  Geometric tolerance.
    \returns true if the input parameter value is within a valid parametric
    range of the curve; false otherwise.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool isOn(
    double param,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the parameter value of a point.

    \param point [in]  Point to be evaluated.
    \param tol   [in]  Geometric tolerance.
    \returns Parameter value of the point.
    \remarks
    The returned parameter specifies a point within the tolerance of the point.
    If the point is not on this curve, the results are unpredictable.
    If you are not sure whether the point is on this curve, use isOn() instead
    of this function.

    \remarks
    If projection of the point does not lie in range-interval, the closest bound
    of the interval is returned. In case of uvBox == NULL paramOf(point, tol) is
    called.
  */
  double paramOf(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the parameter value of a point.

    \param point [in]  Point to be evaluated.
    \param range [in]  Interval on the curve.
    \param tol   [in]  Geometric tolerance.

    \remarks
    The returned parameter specifies a point within the tolerance of the point.
    If the point is not on this curve, end or start of the interval will be set.
    In case of uvBox == NULL paramOf(point, rng, tol) is called in which range -
    native range of the curve. If you are not sure whether the point is on this
    curve, use isOn() instead of this function.
  */
  double paramOf(
    const OdGePoint3d& point,
    const OdGeInterval* range,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns one or more trimmed offset curves.

    \param distance         [in]  Offset distance.
    \param planeNormal      [in]  A normal to the plane of this curve.
    \param offsetCurveList [out]  Receives an array of trimmed offset curves.
    \param extensionType    [in]  How curves will be extended at discontinuities of type C1.
    \param tol              [in]  Geometric tolerance.

    \remarks
    The offsets are trimmed to eliminate self-intersecting loops.
    The curve is assumed to be planar, and planeNomal is assumed to be normal to
    the curve plane.
    The direction of pa ositive offset at any point on this curve is the cross
    product of planeNormal and the tangent to the curve at that point.
    The new operator is used to create the curves returned by offsetCurveList.
    It is up to the caller to delete these curves.

    \remarks
    Currently not implemented.

    Throws:
    <table>
    Exception
    eNotImplemented
    </table>
  */
  void getTrimmedOffset(
    double distance,
    const OdGeVector3d& planeNormal,
    OdGeCurve3dPtrArray& offsetCurveList,
    OdGe::OffsetCrvExtType extensionType = OdGe::kFillet,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if this curve is closed within the specified
    tolerance.

    \param tol [in]  Geometric tolerance.
  */
  bool isClosed(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if this curve is planar, and returns the plane of
    this curve.

    \param plane [out]  Receives the plane of this curve.
    \param tol    [in]  Geometric tolerance.

    \remarks
    Lines are considered to be planar; the returned plane is an arbitrary plane
    containing the line.
  */
  bool isPlanar(
    OdGePlane& plane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true, and a linear entity coincident with this curve, if and only if
    this curve is linear.

    \param line [out]  Receives the line coincident with this curve.
    \param tol   [in]  Geometric tolerance.

    \remarks
    An infinite line is returned, even if this curve is bounded.

    \remarks
    As implemented, this function does nothing but returns false.
    It will be fully implemented in future releases.
  */
  bool isLinear(
    OdGeLine3d& line,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if the specified curve entity is coplanar with this curve and
    returns the common plane.

    \param curve  [in]  Any 3D curve.
    \param plane [out]  Receives the plane of the curves.
    \param tol    [in]  Geometric tolerance.
  */
  bool isCoplanarWith(
    const OdGeCurve3d& curve,
    OdGePlane& plane,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if this curve is periodic for the specified period.

    \param period [out]  Receives the period of this curve.
  */
  bool isPeriodic(
    double& period) const;

  /** \details
    Returns the length of this curve over the specified parameter range.

    \param fromParam [in]  Starting parameter value.
    \param toParam   [in]  Ending parameter value.
    \param tol       [in]  Geometric tolerance.
  */
  double length(
    double fromParam,
    double toParam,
    double tol
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    ) const;

  /** \details
    Returns the length of this curve.

    \param tol [in]  Geometric tolerance.
  */
  double length(
    double tol
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    ) const;

  /** \details
    Returns the parameter of the point at a specified distance from the starting
    point corresponding to datumParam.

    \param datumParam  [in]  Parameter corresponding to the start point.
    \param length      [in]  Distance along curve from the start point.
    \param posParamDir [in]  true if and only if returned parameter is to be greater than dataParam.
    \param tol         [in]  Geometric tolerance.
  */
  double paramAtLength(
    double datumParam,
    double length,
    bool posParamDir
#ifndef SWIG
    = true
#endif
    , double tol
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    ) const;

  /** \details
    Determines the area of this curve between the specified parameters.

    \param startParam [in]  Starting parameter value.
    \param endParam   [in]  Ending parameter value.
    \param value     [out]  Receives the area.
    \param tol        [in]  Geometric tolerance.
    \returns true (and a value) if and only if this curve is planar.
  */
  bool area(
    double startParam,
    double endParam,
    double& value,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if this curve degenerates, and returns the type of
    entity to which this curve degenerates.

    \param degenerateType [out]  Receives the type of the curve to which this curve degenerates.
    \param tol             [in]  Geometric tolerance.

    \remarks
    If isDegenerate returns true, the returned object was created with the new
    operator, and it is the responsibility of the caller to delete it.
  */
  bool isDegenerate(
    OdGe::EntityId& degenerateType,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if this curve degenerates, and returns the entity
    to which this curve degenerates.

    \param pConvertedEntity [out]  Receives a pointer to the object to which this curve degenerates.
    \param tol               [in]  Geometric tolerance.

    \remarks
    If isDegenerate returns true, the returned object was created with the new
    operator, and it is the responsibility of the caller to delete it.
  */
  bool isDegenerate(
    OdGeEntity3d*& pConvertedEntity,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns pointers to the two curves that result from splitting this curve at
    the point corresponding to param.

    \param param   [in]  Parameter value corresponding to the split point.
    \param piece1 [out]  Receives a pointer to the first curve.
    \param piece2 [out]  Receives a pointer to the second curve.

    \remarks
    If param corresponds to a point not on this curve, or to the start point or
    end point of this curve, piece1 and piece2 are set to NULL.
    The curves pointed to by piece1 and piece2 are created with the new command,
    and must be deleted by the caller.
  */
  void getSplitCurves(
    double param,
    OdGeCurve3d*& piece1,
    OdGeCurve3d*& piece2) const;

  /** \details
    Explodes this curve over the specified interval.

    \param explodedCurves    [out]  Receives an array of pointers to the subcurves from the explosion.
    \param newExplodedCurves [out]  Receives an array of flags which, if true, correspond to those explodedCurves the caller must delete.
    \param pInterval          [in]  Pointer to the interval to be exploded. Defaults to entire curve.
    \returns true if the operation is successful; false otherwise.
    \remarks
    The original curve is not changed.
  */
  bool explode(
    OdGeCurve3dPtrArray& explodedCurves,
    OdGeIntArray& newExplodedCurves,
    const OdGeInterval* pInterval = NULL) const;

  /** \details
    Gets the point on this curve locally closest to the specified point.

    \param point                  [in] Any 3D point.
    \param approxPntOnThisCrv [in/out] Approximate point on this curve.
    \param pInterval1             [in] Pointer to the interval in which the point on this curve must lie.
    \param tol                    [in] Geometric tolerance.
  */
  void getLocalClosestPoints(
    const OdGePoint3d& point,
    OdGePointOnCurve3d& approxPntOnThisCrv,
    const OdGeInterval* pInterval1 = NULL,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this curve locally closest to the specified curve, and
    the point on the other curve locally closest to this curve.

    \param curve                   [in] Any 3D curve.
    \param approxPntOnThisCrv  [in/out] Approximate point on this curve.
    \param approxPntOnOtherCrv [in/out] Approximate point on other curve.
    \param pInterval1              [in] Pointer to the interval in which the point on this curve must lie.
    \param pInterval2              [in] Pointer to the interval in which the point on the other curve must lie.
    \param tol                     [in] Geometric tolerance.
  */
  void getLocalClosestPoints(
    const OdGeCurve3d& curve,
    OdGePointOnCurve3d& approxPntOnThisCrv,
    OdGePointOnCurve3d& approxPntOnOtherCrv,
    const OdGeInterval* pInterval1 = NULL,
    const OdGeInterval* pInterval2 = NULL,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true, and the start point, if and only if the parametric interval of
    this curve has a lower bound.

    \param startPoint [out]  Receives the start point.
  */
  bool hasStartPoint(
    OdGePoint3d& startPoint) const;

  /** \details
    Returns true, and the end point, if and only if the parametric interval of
    this curve has an upper bound.

    \param endPoint [out]  Receives the end point.
  */
  bool hasEndPoint(
    OdGePoint3d& endPoint) const;

  /** \details
    Checks the boundaries of the curve interval and calculates a point from the
    curve interval that corresponds to the specified coefficient.

    \param point [out] Receives the point from the curve interval that corresponds to the coefficient.
    \param coef   [in] Coefficient from the interval [0; 1].
    \returns true if and only if the parametric interval of the curve is bounded
    and the value of the coef variable lies within [0; 1].

    \remarks
    The value of the coef variable must be within [0; 1], where:
    * 0 is the beginning of the curve interval
    * 0.5 is the middle of the curve interval
    * 1 is the end of the curve interval
  */
  bool hasMidPoint(
    OdGePoint3d& point,
    double coef = 0.5) const;

  /** \details
    Calculates a point from the curve interval that corresponds to the specified
    coefficient.

    \param coef [in] Coefficient from the interval [0; 1].
    \returns Point from the curve interval that corresponds to the coefficient
    parameter.

    \remarks
    The value of the coef variable must be within [0; 1], where:
    * 0 is the beginning of the curve interval
    * 0.5 is the middle of the curve interval
    * 1 is the end of the curve interval
    Otherwise, an exception eNotApplicable will be thrown.

    \remarks
    The curve must have a bounded interval; otherwise, an exception
    eCurveEndsMissed will be thrown.
  */
  OdGePoint3d midPoint(
    double coef = 0.5) const;

  /** \details
    Returns the point on this curve corresponding to the specified parameter
    value.

    \param param [in]  Parameter to be evaluated.
  */
  OdGePoint3d evalPoint(
    double param) const;

  /** \details
    Returns the point on this curve corresponding to the specified parameter
    value, and the derivatives at that point.

    \param param        [in]  Parameter to be evaluated.
    \param numDeriv     [in]  Number of derivatives to be computed.
    \param derivatives [out]  Receives an array of derivatives at the point corresponding to param.
  */
  OdGePoint3d evalPoint(
    double param,
    int numDeriv,
    OdGeVector3dArray& derivatives) const;

  /** \details
    Returns sample points along this curve and their parameter values in the
    specified arrays.

    \param fromParam       [in]  Starting parameter value.
    \param toParam         [in]  Ending parameter value.
    \param approxEps       [in]  Approximate spacing along a curve.
    \param pointArray     [out]  Receives an array of sample points.
    \param paramArray     [out]  Receives an array of parameters at each point.
    \param forceResampling [in]  sample points are forced to be regenerated.
  */
  void getSamplePoints(
    double fromParam,
    double toParam,
    double approxEps,
    OdGePoint3dArray& pointArray,
    OdGeDoubleArray& paramArray,
    bool forceResampling = false) const;

  /** \details
    Gets sample points along this curve and their parameter values in the
    specified arrays.

    \param numSample   [in]  Number of samples.
    \param pointArray [out]  Receives an array of sample points.
  */
  void getSamplePoints(
    int numSample,
    OdGePoint3dArray& pointArray) const;

  /** \details
    Gets sample points along this curve and their parameter values in the
    specified arrays.

    \param numSample   [in]  Number of samples.
    \param pointArray [out]  Receives an array of sample points.
    \param paramArray [out]  Receives an array of parameters at each point.
  */
  void getSamplePoints(
    int numSample,
    OdGePoint3dArray& pointArray,
    OdGeDoubleArray& paramArray) const;

  /** \details
    The assignment operator for objects the OdGeCurve3d class.
    \param curve [in]  Input 3D curve.
    \returns Reference to this curve.
    \remarks
    Assigns input 3D curve to this curve.
  */
  OdGeCurve3d& operator =(const OdGeCurve3d& curve);

  ///////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
    Returns sample points along this curve and their parameter values in the
    specified arrays.

    \param fromParam   [in]  Starting parameter value.
    \param toParam     [in]  Ending parameter value.
    \param approxEps   [in]  Approximate spacing along a curve.
    \param pointArray [out]  Receives an array of sample points.
    \param paramArray [out]  Receives an array of parameters at each point.
  */
  void getSamplePoints(
    double fromParam,
    double toParam,
    double approxEps,
    OdGePoint3dArray& pointArray,
    OdGeDoubleArray* paramArray = NULL) const;

  /** \details
    Gets sample points along this curve and their parameter values in the
    specified arrays.

    \param paramInterval [in]  Pointer to the parametric interval.
    \param approxEps     [in]  Approximate spacing along a curve.
    \param pointArray    [out]  Receives an array of sample points.
    \param pParamArray   [out]  (Optional) Receives an array of parameters at each point.
    \remarks
    If paramInterval is null, the current curve interval will be used.
  */
  void getSamplePoints(
    const OdGeInterval* paramInterval,
    double approxEps,
    OdGePoint3dArray& pointArray,
    OdGeDoubleArray* pParamArray = NULL) const;

  /** \details
    Appends sample points along this curve and their parameter values to the
    specified arrays.

    \param fromParam    [in]  Starting parameter value.
    \param toParam      [in]  Ending parameter value.
    \param approxEps    [in]  Approximate spacing along a curve.
    \param pointArray  [out]  Receives an array of sample points.
    \param pParamArray [out]  Receives an array of parameters at each point.

    \remarks
    The signature
    appendSamplePoints(double, double, double, OdGePoint3dArray&, OdGeDoubleArray*)
    is currently not implemented.
  */
  void appendSamplePoints(
    double fromParam,
    double toParam,
    double approxEps,
    OdGePoint3dArray& pointArray,
    OdGeDoubleArray* pParamArray = NULL) const;

  /** \details
    Appends sample points along this curve and their parameter values to the
    specified arrays.

    \param numSample    [in]  Number of samples.
    \param pointArray  [out]  Receives an array of sample points.
    \param pParamArray [out]  Receives an array of parameters at each point.
  */
  void appendSamplePoints(
    int numSample,
    OdGePoint3dArray& pointArray,
    OdGeDoubleArray* pParamArray = NULL) const;

  /** \details
    Appends sample points along this curve and their parameter values to the
    specified arrays.

    \param paramInterval [in]  Pointer to the parametric interval.
    \param approxEps     [in]  Approximate spacing along a curve.
    \param pointArray   [out]  Receives an array of sample points.
    \param pParamArray  [out]  Receives an array of parameters at each point.

    \remarks
    If paramInterval is NULL, the current curve interval will be used.
  */
  void appendSamplePoints(
    const OdGeInterval *paramInterval, 
    double approxEps, 
    OdGePoint3dArray& pointArray, 
    OdGeDoubleArray* pParamArray = NULL) const;

  /** \details
    Returns a pointer to the 2D variant of the curve (z-coordinate will be
    ignored).

    \param tol                 [in]  Tolerance used in case of interpolation.
    \param sameParametrization [in]  Necessity of parametrization preserving during conversion.

    \remarks
    If it is possible, than the curve will be converted analytically (i.e. 3D
    line segment becomes a 2D line segment, etc), otherwise it will be projected
    onto an XY-plane.
  */
  OdGeCurve2d* convertTo2d(
    const OdGeTol& tol = OdGeContext::gTol,
    bool sameParametrization = false) const;
//DOM-IGNORE-BEGIN
protected:
  OdGeCurve3d();
  OdGeCurve3d(
    const OdGeCurve3d& source);
//DOM-IGNORE-END
};

#include "TD_PackPop.h"

#endif
