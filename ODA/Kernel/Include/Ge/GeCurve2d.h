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

#ifndef OD_GE_CURVE_2D_H
#define OD_GE_CURVE_2D_H /*!DOM*/

class OdGePointOnCurve2d;
class OdGeInterval;
class OdGeMatrix2d;
class OdGeLine2d;
class OdGePointOnCurve2dData;
class OdGeBoundBlock2d;
class OdGeExtents2d;

#include "Ge/GeEntity2d.h"
#include "Ge/GeIntArray.h"
#include "Ge/GeInterval.h"
#include "Ge/GePoint2d.h"
#include "Ge/GePoint2dArray.h"
#include "Ge/GeVector2d.h"
#include "Ge/GeVector2dArray.h"
#include "Ge/GeCurve2dPtrArray.h"
#include "Ge/GeDoubleArray.h"

#include "Ge/GeCurve3dPtrArray.h"

#include "TD_PackPush.h"

/** \details
  This class is the base class for all OdGe 2D curves.

  Corresponding C++ library: TD_Ge

  <group OdGe_Classes>
*/
class GE_TOOLKIT_EXPORT OdGeCurve2d : public OdGeEntity2d
{
public:

  /** \details
    Creates a new 3D variant of the curve (z-coordinate equal to zero).

    \returns Pointer to the created 3D variant of the curve (z-coordinate equal
    to zero).

    \remarks
    For OdGeOffsetCurve a base curve will be created that should be MANUALLY
    deleted. See documentation about OdGeOffsetCurve.
  */
  OdGeCurve3d* convertTo3d() const;

  /** \details
    Gets a 3D variant of the curve (z-coordinate equal to zero).

    \param curve3d [out] Reference to the resulting 3D curve of suitable type.

    \remarks
    The user of this method should pass a 3D curve to the method that
    corresponds to the type of 2D curve, otherwise an eNotThatKindOfClass
    exception will be thrown.
    Examples of corresponding curve types:
    * OdGeRay2d - OdGeRay3d;
    * OdGeLine2d - OdGeLine3d;
    * OdGeLineSeg2d - OdGeLineSeg3d;
    * OdGeCircArc2d - OdGeCircArc3d;
    * OdGeEllipArc2d - OdGeEllipArc3d;
    * OdGePolyline2d - OdGePolyline3d;
    * OdGeNurbCurve2d - OdGeNurbCurve3d;
  */
  void convertTo3d(OdGeCurve3d& curve3d) const;

  /** \details
    Gets the parametric interval of the curve.

    \param interval [out]  Receives the parametric interval of the curve.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getInterval(
    OdGeInterval& interval) const;

  /** \details
    Gets the parametric interval of the curve.

    \param interval [out]  Receives the parametric interval of the curve.
    \param start    [out]  Receives the start point of the interval.
    \param end      [out]  Receives the end point of the interval. 

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getInterval(
    OdGeInterval& interval,
    OdGePoint2d& start,
    OdGePoint2d& end) const;

  /** \details
    Reverses the parameter direction of this curve.

    \returns Curve with the reversed direction parameter.
    \remarks
    The point set of this curve is unchanged.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  OdGeCurve2d& reverseParam();

  /** \details
    Sets to natural domain of this curve.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  OdGeCurve2d& setInterval();

  /** \details
    Sets the parametric interval of this curve.

    \param interval [in]  Parametric interval of this curve.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool setInterval(
    const OdGeInterval& interval);

  /** \details
    Returns the distance between the input point and the closest point on this
    curve.
    \param point [in]  Input point.
    \param tol   [in]  Geometric tolerance.
    \returns Distance value.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  double distanceTo(
    const OdGePoint2d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the distance of the closest points between this curve and the input
    curve.

    \param otherCur [in]  Input curve.
    \param tol      [in]  Geometric tolerance.
    \returns Distance value.
  */
  double distanceTo(
    const OdGeCurve2d& otherCur,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Finds the closest point on this curve to the input point.

    \param point [in]  Input point according to which the closest point on this curve is searched.
    \param tol   [in]  Geometric tolerance.
    \returns The closest point on this curve.
    \remarks
    This function is the same as getClosestPointTo().
  */
  OdGePoint2d closestPointTo(
    const OdGePoint2d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Finds the closest point on this curve to the input point.

    \param point  [in]  Point to be evaluated.
    \param param [out]  uv-coord of closest point.
    \param range  [in]  Interval on the curve.
    \param tol    [in]  Geometric tolerance.
    \returns The closest point on this curve.
    \remarks
    Native interval is not considered.
    If projection of the point does not lie in range-interval, the closest bound
    of the interval is returned.
    In case of uvBox == NULL closestPointTo(point, range, tol) is called where
    range is a native curves interval.
    An unbounded interval should not be used here.
  */
  OdGePoint2d closestPointTo(
    const OdGePoint2d& point,
    double& param,
    const OdGeInterval* range,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Finds the closest point between this curve and the point of an input curve.
    \param curve2d        [in]  Input curve.
    \param pntOnOtherCrv [out]  Closest point on the input curve.
    \param tol            [in]  Geometric tolerance.
    
    \returns 
    The closest point of this curve.
    
    \remarks
    This function is the same as getClosestPointTo().
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  OdGePoint2d closestPointTo(
    const OdGeCurve2d& curve2d,
    OdGePoint2d& pntOnOtherCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Finds the closest point on this curve to the input point. 
    \param point     [in]  Input point.
    \param pntOnCrv [out]  Closest point on this curve to the input point.
    \param tol       [in]  Geometric tolerance.
    
    \returns
    The closest point of this curve.
    
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getClosestPointTo(
    const OdGePoint2d& point,
    OdGePointOnCurve2d& pntOnCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Finds the point on this curve which is the closest to the point of an input
    curve.
    \param curve2d       [in]  Input curve.
    \param pntOnThisCrv [out]  Receives the closest point on this curve.
    \param pntOnOtherCrv [in]  A point on the other curve.
    \param tol           [in]  Geometric tolerance.
    
    \returns
    The closest point of this curve.
    
    \remarks
    This function is the same as getClosestPointTo().
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getClosestPointTo(
    const OdGeCurve2d& curve2d,
    OdGePointOnCurve2d& pntOnThisCrv,
    OdGePointOnCurve2d& pntOnOtherCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Finds the point on the input curve whose normal passes through the input
    point.
    \param point     [in]  Input point.
    \param pntOnCrv [out]  Closest point on this curve whose normal passes through the input point.
    \param tol       [in]  Geometric tolerance.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool getNormalPoint(
    const OdGePoint2d& point,
    OdGePointOnCurve2d& pntOnCrv,
    const OdGeTol& tol = OdGeContext::gTol) const;

  //DOM-IGNORE-BEGIN
  // Tests if point is on *curve*.
  TD_USING(OdGeEntity2d::isOn);
  //DOM-IGNORE-END

  /** \details
    Determines if a point lies on the curve.

    \param point  [in]  Input point.
    \param param [out]  Output parameter value of point if the point lies on the curve.
    \param tol    [in]  Geometric tolerance.

    \remarks
    The param parameter is only set to a meaningful value if this function
    returns true.
  */
  bool isOn(
    const OdGePoint2d& point,
    double& param,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines if an input parameter value is within a valid parametric range of
    the curve.
    \param param [in]  Input parameter value.
    \param tol   [in]  Geometric tolerance.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool isOn(
    double param,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the parameter value of a point.

    \param point [in]  Input point on the curve.
    \param tol   [in]  Geometric tolerance.

    \remarks
    The returned parameters specify a point within tol of point.
    If point is not on the curve, the results are unpredictable.
    If you are not sure the point is on the curve, use isOn () instead of this
    function.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  double paramOf(
    const OdGePoint2d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the parameter value of a point.

    \param point [in]  Input point on the curve.
    \param range [in]  Interval on the curve.
    \param tol   [in]  Geometric tolerance.

    \remarks
    The returned parameters specify a point within tol of point.
    In case of uvBox == NULL paramOf(point, tol) is called.
    If you are not sure the point is on the curve, use isOn() instead of this
    function.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  double paramOf(const OdGePoint2d & point,
    const OdGeInterval * range,
    const OdGeTol & tol) const;

  /** \details
    Gets one or more offset curves that represent the trimmed offset of the
    curve.
    \param distance         [in]  Offset distance.
    \param offsetCurveList [out]  List that stores offset curves.
    \param extensionType    [in]  Determines how the offset curve will be extended at points of C1 discontinuity.
    \param tol              [in]  Geometric tolerance.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getTrimmedOffset(
    double distance,
    OdGeCurve2dPtrArray& offsetCurveList,
    OdGe::OffsetCrvExtType extensionType = OdGe::kFillet,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if the curve is closed within the specified
    tolerance.

    \param tol [in]  Geometric tolerance.
  */
  bool isClosed(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines if the curve is periodic.
    \param period [in]  Period of the curve value.
    \returns true if the curve is periodic; false otherwise.
    \remarks
    The curve is periodic if and only if it is closed and the two points
    evalPoint(t) and evalPoint(t+period) are always the same point for all
    values of t.
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool isPeriodic(
    double& period) const;

  /** \details
    Checks if this curve is linear.
    \param line [out]  Receives the line coincident with this curve.
    \param tol   [in]  Geometric tolerance.
    \returns true if the curve is linear; false otherwise.
    \remarks
    Returns true, and a linear entity coincident with this curve, if and only if
    this curve is linear.
  */
  bool isLinear(
    OdGeLine2d& line,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the arc length of the curve between the two parameter values.
    \param fromParam [in] First parameter value that represents the start of the curve.
    \param toParam   [in] Second parameter value that represents the end of the curve.
    \param tol       [in] Geometric tolerance.
    \returns 
    \remarks Arc length value.
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
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
    Returns the arc length of the curve.
    \param tol [in] Geometric tolerance.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  double length(
    double tol
#ifndef SWIG
    = OdGeContext::gTol.equalPoint()
#endif
    ) const;

  /** \details
    Returns the parameter value of the point whose arc length distance is the
    input length.
    \param datumParam  [in] Input parameter value.
    \param length      [in] Input arc length.
    \param posParamDir [in] Input parameter direction.
    \param tol         [in] Geometric tolerance.
    \returns Parameter value of the point at the specified distance.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
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
    Gets the area of the interval of the curve defined by startParam and
    endParam.
    \param startParam [in] Parameter value of interval start.
    \param endParam   [in] Parameter value of interval end (must be greater than startParam).
    \param value     [out] Output area of curve between startParam and endParam.
    \param tol        [in] Geometric tolerance.
    \returns true if the area was successfully calculated with the given
    tolerance; false otherwise.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool area(
    double startParam, double endParam,
    double& value,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines if the curve is degenerate and if so returns the type of entity
    that the curve degenerates into.
    \param degenerateType [out]  Output type of degenerate entity.
    \param tol             [in]  Geometric tolerance.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool isDegenerate(
    OdGe::EntityId& degenerateType,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Determines if the curve is degenerate and if so returns the entity that the
    curve degenerates into.
    \param pConvertedEntity [out] Output entity that the curve degenerates into.
    \param tol               [in] Geometric tolerance.
    \returns true if the curve is degenerate; false otherwise.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool isDegenerate(
    OdGeEntity2d*& pConvertedEntity,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns two segments that are obtained by splitting the curve at the input
    parameter value.
    \param param   [in] Parameter value at which the curve is to be split.
    \param piece1 [out] Pointer to the first curve segment.
    \param piece2 [out] Pointer to the second curve segment.
    \remarks
    If the specified parameter value does not lie within the parametric interval
    of the curve or if it corresponds to the start or endpoint of the curve,
    then piece1 and piece2 are set to null.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getSplitCurves(
    double param,
    OdGeCurve2d*& piece1,
    OdGeCurve2d*& piece2) const;

  /** \details
    Explodes the curve into its component sub-curves.
    \param explodedCurves   [out] List of curves into which this curve was exploded.
    \param newExplodedCurve [out] Array of flags telling whether the returned curves should be deleted.
    \param interval          [in] Interval of curve that is to be exploded.
    \returns true if the curve is exploded (that is, the curve was a composite
    or a discontinuous spline) and false otherwise.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool explode(
    OdGeCurve2dPtrArray& explodedCurves,
    OdGeIntArray& newExplodedCurve,
    const OdGeInterval* interval = NULL) const;

  /** \details
    Searches for the local minimum distance between this curve and the input
    point using the input point as an initial guess.
    \param point         [in] Input 2D point.
    \param approxPnt [in/out] Input/output point on this curve.
    \param nbhd          [in] Input interval for searching on this curve.
    \param tol           [in] Geometric tolerance.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getLocalClosestPoints(
    const OdGePoint2d& point,
    OdGePointOnCurve2d& approxPnt,
    const OdGeInterval* nbhd = NULL,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Gets local closest points.
    Searches for the local minimum distance between two curves using the two
    input points as initial guesses.
    \param otherCurve              [in] Input 2D curve.
    \param approxPntOnThisCrv  [in/out] Input/output point on this curve.
    \param approxPntOnOtherCrv [in/out] Input/output point on input curve.
    \param nbhd1                   [in] Interval for searching on this curve.
    \param nbhd2                   [in] Interval for searching on input curve.
    \param tol                     [in] Geometric tolerance.
    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getLocalClosestPoints(
    const OdGeCurve2d& otherCurve,
    OdGePointOnCurve2d& approxPntOnThisCrv,
    OdGePointOnCurve2d& approxPntOnOtherCrv,
    const OdGeInterval* nbhd1 = NULL,
    const OdGeInterval* nbhd2 = NULL,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Calculates the oriented bounding box of curve.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.

    \returns
    Bounding box as an OdGeBoundBlock2d entity.
  */
  OdGeBoundBlock2d boundBlock() const;

  /** \details
    Calculates the oriented bounding box of curve.

    \param range [in]  Sub-interval of this curve to be bounded.

    \returns
    Bounding box as a OdGeBoundBlock2d entity.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  OdGeBoundBlock2d boundBlock(
    const OdGeInterval& range) const;

  /** \details
    Calculates the bounding box whose edges are aligned with the coordinate axes.

    \returns
    Bounding box as an OdGeBoundBlock2d entity.
  */
  OdGeBoundBlock2d orthoBoundBlock() const;

  /** \details
    Calculates the bounding box whose edges are aligned with the coordinate axes.

    \param range [in]  Interval of this curve to be bounded.

    \returns
    Bounding box as an OdGeBoundBlock2d entity.
  */
  OdGeBoundBlock2d orthoBoundBlock(
    const OdGeInterval& range) const;

  /** \details
    Returns bounding box of the whole curve or a part of it. The box is
    axis-aligned in the specified local coordinate system. It is guaranteed to
    enclose the curve, but not guaranteed to be tight/minimal.

    \param range       [in]  The part of the curve to be bounded.
    \param coordSystem [in]  The local coordinate system in which box extents are computed.

    \remarks
    If the range is unbound (e.g. by default), the bounding box of the whole
    curve (range can be obtained by the getInterval method) is computed.
    Otherwise the bounding box is computed only for the set of points with the
    parameter in the specified range. If coordSystem is NULL (e.g. by default),
    then bounding box is aligned with global coordinate axes. Otherwise, the box
    will be axis-aligned in the local coordinate system specified by the
    coordSystem matrix.
  */
  OdGeExtents2d getGeomExtents(
    const OdGeInterval& range = OdGeInterval(),
    const OdGeMatrix2d* coordSystem = NULL) const;

  /** \details
    Returns true, and the start point, if and only if the parametric interval of
    the curve has a lower bound.

    \param startPoint [out]  Receives the start point of the interval.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool hasStartPoint(
    OdGePoint2d& startPoint) const;

  /** \details
    Returns true, and the end point, if and only if the parametric interval of
    the curve has an upper bound.

    \param endPoint [out]  Receives the end point of the interval.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  bool hasEndPoint(
    OdGePoint2d& endPoint) const;

  /** \details
    Checks the boundaries of the curve interval and calculates a point from the
    curve interval that corresponds to the specified coefficient.

    \param point [out] Receives the point from the curve interval that corresponds to the coefficient.
    \param coef   [in] Coefficient from the interval [0; 1].
    \returns true if and only if the parametric interval of the curve is bounded
    and the value of the coef variable lies within [0; 1].

    \remarks
    The value of the coef variable must be within [0; 1], where 
    * 0 is the beginning of the curve interval
    * 0.5 is the middle of the curve interval
    * 1 is the end of the curve interval
  */
  bool hasMidPoint(
    OdGePoint2d& point,
    double coef = 0.5) const;

  /** \details
    Calculates a point from the curve interval that corresponds to the specified
    coefficient.

    \param coef [in] Coefficient from the interval [0; 1].
    \returns Point from the curve interval that corresponds to the coefficient
    parameter.

    \remarks
    The value of the coef variable must be within [0; 1], where
    * 0 is the beginning of the curve interval
    * 0.5 is the middle of the curve interval
    * 1 is the end of the curve interval
    Otherwise, an exception eNotApplicable will be thrown.

    \remarks
    The curve must have a bounded interval otherwise, an exception
    eCurveEndsMissed will be thrown.
  */
  OdGePoint2d midPoint(
    double coef = 0.5) const;

  /** \details
    Returns the point on the curve corresponding to the specified parameter
    value.

    \param param [in]  Parameter to be evaluated.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  OdGePoint2d evalPoint(
    double param) const;

  /** \details
    Returns the point on the curve corresponding to the specified parameter
    value and the derviatives at that point.

    \param param        [in] Parameter to be evaluated.
    \param numDeriv     [in] The number of derivatives to be computed (should not be set larger than 2).
    \param derivatives [out] Receives an array of derivatives at the point corresponding to param.

    \remarks
    By default this function throws the exception "not Implemented". Should be
    implemented in derived classes.
  */
  OdGePoint2d evalPoint(
    double param,
    int numDeriv,
    OdGeVector2dArray& derivatives) const;

  /** \details
    Returns sample points along this curve and their parameter values in the
    specified arrays.

    \param fromParam   [in] Starting parameter value.
    \param toParam     [in] Ending parameter value.
    \param approxEps   [in] Approximate spacing along the curve.
    \param pointArray [out] Output array of sample points.
    \param paramArray [out] Output array of parameters at each point.
  */
  void getSamplePoints(
    double fromParam,
    double toParam,
    double approxEps,
    OdGePoint2dArray& pointArray,
    OdGeDoubleArray& paramArray) const;

  /** \details
    Returns a quantity of sample points along this curve and the points
    themselves in the specified arrays.
    \param numSample  [out] Receives a quantity of sample points.
    \param pointArray [out] Receives sample points.
  */
  void getSamplePoints(
    int numSample,
    OdGePoint2dArray& pointArray) const;

  /** \details
    The assignment operator for objects of the OdGeCurve2d class.
    \param curve [in]  Input 2D curve.
    \returns Reference to this object.
    \remarks
    Assigns input 2D curve to this curve.
  */
  OdGeCurve2d& operator =(const OdGeCurve2d& curve);

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
    Gets sample points along this curve and their parameter values to the
    specified arrays.

    \param fromParam    [in]  Starting parameter value.
    \param toParam      [in]  Ending parameter value.
    \param approxEps    [in]  Approximate spacing along a curve.
    \param pointArray  [out]  Receives an array of sample points.
    \param pParamArray [out]  Receives a pointer to an array of parameter values for each point.
    \remarks
    By default this function throws exception "not Implemented". Should be
    implemented in derived classes.
  */
  void getSamplePoints(
    double fromParam,
    double toParam,
    double approxEps,
    OdGePoint2dArray& pointArray,
    OdGeDoubleArray* pParamArray = NULL) const;

  /** \details
    Appends sample points along this curve and their parameter values to the
    specified arrays.

    \param fromParam   [in]  Starting parameter value.
    \param toParam     [in]  Ending parameter value.
    \param approxEps   [in]  Approximate spacing along a curve.
    \param pointArray [out]  Pointer an array of sample points which are appended.
    \param pParamArray [in]  Pointer to an array to receive the parameters at each point.
    \remarks
    By default this function throws exception "not Implemented". Should be
    implemented in derived classes.
  */
  void appendSamplePoints(
    double fromParam,
    double toParam,
    double approxEps,
    OdGePoint2dArray& pointArray,
    OdGeDoubleArray* pParamArray = NULL) const;

  /** \details
    Appends sample points along this curve and their parameter values to the
    specified arrays.

    \param numSample   [in]  Number of samples.
    \param pointArray [out]  Receives an array of sample points.
  */
  void appendSamplePoints(
    int numSample,
    OdGePoint2dArray& pointArray) const;

  /** \details
    Restores a 2D parametric curve on a surface from a 3D curve.

    \param curve   [in]  3D curve from which this curve is restored.
    \param surface [in]  Surface on which a restored curve lies.
    \param tol     [in]  Geometric tolerance.

    \returns 2D curve representing the projection of the 3D curve on the
    surface.

    \remarks
    NULL is returned if projection failed.
  */
  static OdGeCurve2d* restoreUvCurve(
    const OdGeCurve3d* curve,
    const OdGeSurface* surface,
    const OdGeTol& tol = OdGeContext::gTol);
//DOM-IGNORE-BEGIN
protected:
  /** \param source [in]  Object to be cloned.
  */
  OdGeCurve2d();
  OdGeCurve2d(
    const OdGeCurve2d& source);
//DOM-IGNORE-END
};

#include "TD_PackPop.h"

#endif // OD_GE_CURVE_2D_H
