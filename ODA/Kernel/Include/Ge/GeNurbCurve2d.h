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


#ifndef OD_GENURB2d_H
#define OD_GENURB2d_H /*!DOM*/

#include "Ge/GeSplineEnt2d.h"
#include "OdPlatformSettings.h"

class OdGeNurbCurve2dImpl;
class OdGePolyline2d;
class OdGeEllipArc2d;
class OdGeLineSeg2d;
class OdGeKnotVector;

#include "TD_PackPush.h"

/** \details
  This class represents non-uniform rational B-splines (NURBS) in 2D space.

  Corresponding C++ library: TD_Ge

  <group OdGe_Classes>

  \sa
  <link ge_OdGeNurbCurve3d, Working with NURBS curves>
*/
class GE_TOOLKIT_EXPORT OdGeNurbCurve2d : public OdGeSplineEnt2d
{
public:
  /** \details
    Default constructor for objects of the OdGeNurbCurve2d class. Constructs an
    empty NURB curve.
  */
  OdGeNurbCurve2d();

  /** \details
    Copy constructor for objects of the OdGeNurbCurve2d class.

    \param source [in] Object to be copied.
  */
  OdGeNurbCurve2d(
    const OdGeNurbCurve2d& source);

  /** \details
    Constructor for objects of the OdGeNurbCurve2d class.

    \param degree        [in] Degree of the curve.
    \param knots         [in] Knot vector.
    \param controlPoints [in] Array of 2D control points.
    \param isPeriodic    [in] true if and only if the curve is periodic.

    \remarks
    High degree values make a curve more flexible. For example degree 1 is used
    to define a segment, degree 2 is used to define a NURB circle, etc.

    A knot vector defines a sequence of parameter values that determine where
    and how the control points affect the curve. The knot vector is a sequence
    of parameter values and divides the parametric space into intervals, usually
    called knot spans. Values of knot vector must be provided in ascending
    order, for example: (0, 0, 0, 1, 2, 3, 3, 3) can be valid while
    (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid.

    The control points specify the placement of the curve and adjust its shape.
    The quantity of control points must be at least the degree + 1.

    If isPeriodic is false, then knots.length() must be equal to
    controlPoints.length() + degree + 1.
    If isPeriodic is true, then knots.length() must be equal to
    controlPoints.length(), the first and the last controlPoints must be equal.
  */
  OdGeNurbCurve2d(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint2dArray& controlPoints,
    bool isPeriodic = false);

  /** \details
    Constructor for objects of the OdGeNurbCurve2d class.

    \param degree        [in] Degree of the curve.
    \param knots         [in] Knot vector.
    \param controlPoints [in] Array of 2D control points.
    \param weights       [in] Array of weight values.
    \param isPeriodic    [in] true if and only if the curve is periodic.

    \remarks
    High degree values make a curve more flexible. For example degree 1 is used
    to define a polyline, degree 2 is used to define a NURB circle, etc.

    A knot vector defines a sequence of parameter values that determine where
    and how the control points affect the curve. The knot vector is a sequence
    of parameter values and divides the parametric space into intervals, usually
    called knot spans. Values of knot vector must be provided in ascending
    order, for example: (0, 0, 0, 1, 2, 3, 3, 3) can be valid while
    (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid.

    The control points specify the placement of the curve and adjust its shape.
    The quantity of control points must be at least the degree + 1.

    The closer the weight value is to zero, the closer the curve is to a
    straight line near the corresponding control point. If the weight increases,
    the curve pulls toward the corresponding control point. If the weight
    decreases, the curve pushes from the corresponding control point.

    If isPeriodic is false, then knots.length() must be equal to
    controlPoints.length() + degree + 1.
    If isPeriodic is true, then knots.length() must be equal to
    controlPoints.length(), the first and last controlPoints must be equal.
  */
  OdGeNurbCurve2d(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint2dArray& controlPoints,
    const OdGeDoubleArray& weights,
    bool isPeriodic = false);

  /** \details
    Constructor for objects of the OdGeNurbCurve2d class.

    \param degree      [in] Degree of the curve.
    \param fitPolyline [in] Input polyline fit points of which are interpolated by the curve.
    \param isPeriodic  [in] true if and only if the curve is periodic (closed curve).

    \remarks
    High degree values make a curve more flexible. For example degree 1 is used
    to define a polyline, degree 2 is used to define a NURB circle, etc.
  */
  OdGeNurbCurve2d(
    int degree,
    const OdGePolyline2d& fitPolyline,
    bool isPeriodic = false);

  /** \details
    Constructor for objects of the OdGeNurbCurve2d class.

    \param fitPoints           [in] Array of 2D points through which to fit the curve.
    \param startTangent        [in] Input start tangent vector.
    \param endTangent          [in] Input end tangent vector.
    \param startTangentDefined [in] Input flag indicating whether startTangent should be used.
    \param endTangentDefined   [in] Input flag indicating whether endTangent should be used.
    \param fitTolerance        [in] Tolerance to which the curve should approximate fitPoints.

    \remarks
    Constructs a 2D curve interpolating a given array of 2D points within the
    given tolerance and having given derivatives at the start point and end
    point.
    If startTangentDefined is 0, then startTangent will be ignored and the
    constructor will compute the start tangent vector. The same is true for
    endTangentDefined.
  */
  OdGeNurbCurve2d(
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent,
    bool startTangentDefined = true,
    bool endTangentDefined = true,
    const OdGeTol& fitTolerance = OdGeContext::gTol);

  /** \details
    Constructor for objects of the OdGeNurbCurve2d class.

    \param fitPoints    [in] Array of 2D points through which to fit the curve.
    \param fitTolerance [in] Input maximal deviation of the curve from the fitPoints.

    \remarks
    Constructs a 2D curve interpolating a given array of 2D points within the
    given tolerance.
  */
  OdGeNurbCurve2d(
    const OdGePoint2dArray& fitPoints,
    const OdGeTol& fitTolerance = OdGeContext::gTol);

  /** \details
    Constructor for objects of the OdGeNurbCurve2d class.

    \param fitPoints    [in] Array of 2D points through which to fit the curve.
    \param fitTangents  [in] Input vectors interpolating the curve's derivatives at fitPoints.
    \param fitTolerance [in] Input maximal deviation of the curve from the fitPoints.
    \param isPeriodic   [in] true if and only if the curve is periodic (closed curve).

    \remarks
    Constructs a 2D polynomial curve interpolating a given array if 2D points
    within the given tolerance.
    If isPeriodic is true, then the first and the last fitPoints must be
    identical. The same must be true for fitTangents.
  */
  OdGeNurbCurve2d(
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2dArray& fitTangents,
    const OdGeTol& fitTolerance = OdGeContext::gTol,
    bool isPeriodic = false);

  /** \details
    Constructor for objects of the OdGeNurbCurve2d class.

    \param ellipse [in] Input elliptical arc from which a NURB curve is constructed.

    \remarks
    Constructs a 2D curve from a passed 2D elliptical arc.
  */
  OdGeNurbCurve2d(
    const OdGeEllipArc2d& ellipse);

  /** \details
    Constructor for objects of the OdGeNurbCurve2d class.

    \param linSeg [in] Input line segment from which a NURB curve is constructed.

    \remarks
    Constructs a 2D curve from a passed 2D line segment.
  */
  OdGeNurbCurve2d(
    const OdGeLineSeg2d& linSeg);

  /** \details
    Constructor for objects of the OdGeNurbCurve2d class.

    \param fitPoints           [in] Array of 2D points through which to fit the curve.
    \param startTangent        [in] Input start tangent vector.
    \param endTangent          [in] Input end tangent vector.
    \param startTangentDefined [in] Input flag indicating whether startTangent should be used.
    \param endTangentDefined   [in] Input flag indicating whether endTangent should be used.
    \param knotParam           [in] Knot parameterization which define the knot values.
    \param fitTol              [in] Tolerance to which the spline should approximate fitPoints.

    \remarks
    Constructs a 2D curve interpolating a given array of 2D points within the
    given tolerance and knot parameterization, and having given derivatives at
    the start point and end point.
    If startTangentDefined is 0, then startTangent is ignored and the
    constructor computes the start tangent vector. Similarly for
    endTangentDefined.
  */
  OdGeNurbCurve2d(
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent,
    bool startTangentDefined,
    bool endTangentDefined,
    OdGeKnotParameterization knotParam,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Returns the number of fit points.

    \returns
    Number of fit points as an integer value.
  */
  int numFitPoints() const;

  /** \details
    Gets the knot parametrization of the spline created with fit points.

    \param knotParam [out] Receives the knot parametrization.

    \returns
    true if and only if knot parametrization is retrieved successfully.
  */
  bool getFitKnotParameterization(OdGeKnotParameterization& knotParam) const;

  /** \details
    Retrieves the fit point at the specified index.

    \param fitPointIndex [in] Fit point index.
    \param point        [out] Receives the fit point.

    \returns
    true if and only if 0 &lt;= fitPointIndex &lt; numFitPoints().
  */
  bool getFitPointAt(
    int fitPointIndex,
    OdGePoint2d& point) const;

  /** \details
    Retrieves the fit tolerance that is used to construct this spline if this
    spline was constructed with fit points.

    \param fitTolerance [out] Recieves the geometric tolerance.

    \returns
    true if and only if this spline was constructed with fit points.
  */
  bool getFitTolerance(
    OdGeTol& fitTolerance) const;

  /** \details
    Retrieves the starting and ending tangents if this spline was constructed
    with fit data.

    \param startTangent [out] Receives the starting tangent.
    \param endTangent   [out] Receives the ending tangent.

    \returns
    true if and only if this spline was constructed with fit data.
  */
  bool getFitTangents(
    OdGeVector2d& startTangent,
    OdGeVector2d& endTangent) const;

  /** \details
    Retrieves all the fit data used to construct this spline if this spline was
    constructed with fit data.

    \param fitPoints     [out] Receives the interpolation points.
    \param fitTolerance  [out] Receives the geometric tolerance.
    \param tangentsExist [out] Receives true if and only if tangents were used in constructing this spline.
    \param startTangent  [out] Receives the starting tangent.
    \param endTangent    [out] Receives the ending tangent.

    \returns
    true if and only if this spline was constructed with fit data.

    \remarks
    startTangent and endTangent parameters are meaningful if and only if
    tangentsExist == true.
  */
  bool getFitData(
    OdGePoint2dArray& fitPoints,
    OdGeTol& fitTolerance,
    bool& tangentsExist,
    OdGeVector2d& startTangent,
    OdGeVector2d& endTangent) const;

  /** \details
    Retrieves all the fit data used to construct this spline if this spline was
    constructed with fit data.

    \param fitPoints     [out] Receives the interpolation points.
    \param fitTolerance  [out] Receives the geometric tolerance.
    \param tangentsExist [out] Receives true if and only if tangents were used in constructing this spline.
    \param startTangent  [out] Receives the starting tangent.
    \param endTangent    [out] Receives the ending tangent.
    \param knotParam     [out] Receives the knot parameterization.

    \returns
    true if and only if this spline was constructed with fit data.

    \remarks
    startTangent and endTangent parameters are meaningful if and only if
    tangentsExist == true.
  */
  bool getFitData(
    OdGePoint2dArray& fitPoints,
    OdGeTol& fitTolerance,
    bool& tangentsExist,
    OdGeVector2d& startTangent,
    OdGeVector2d& endTangent,
    OdGeKnotParameterization& knotParam) const;

  /** \details
    Retrieves all the data that define the spline.

    \param degree        [out] Receives the degree of spline.
    \param knots         [out] Receives the knot vector.
    \param controlPoints [out] Receives an array of 2D control points.
    \param weights       [out] Receives an array of weights.
    \param rational      [out] Receives true if and only if the spline is rational.
    \param periodic      [out] Receives true if and only if the spline is periodic.

    \remarks
    High degree values represent flexible curves. For example degree 1 is used
    to define a polyline, degree 2 is used to define a NURB circle, etc.

    A knot vector defines a sequence of parameter values that determine where
    and how the control points affect the curve. The knot vector is a sequence
    of parameter values and divides the parametric space into intervals, usually
    called knot spans. Values of knot vector must be provided in ascending
    order, for example: (0, 0, 0, 1, 2, 3, 3, 3) can be valid while
    (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid. Knot vector does not affect the
    form of the NURB curve, it only affects its parameterization.

    The closer the weight value is to zero, the closer the curve is to a
    straight line near the corresponding control point. If the weight increases,
    the curve pulls toward the corresponding control point. If the weight
    decreases, the curve pushes from the corresponding control point.
    The weights array will be empty if the spline is not rational.
  */
  void getDefinitionData(
    int& degree,
    bool& rational,
    bool& periodic,
    OdGeKnotVector& knots,
    OdGePoint2dArray& controlPoints,
    OdGeDoubleArray& weights) const;

  /** \details
    Returns the number of weights in the spline.

    \returns
    Number of weights in the spline as an integer value.

    \remarks
    Returns numControlPoints() if this spline is rational, 0 if it is not.
  */
  int numWeights() const;

  /** \details
    Returns the weight at the specified index.

    \param weightIndex [in] Weight index.

    \returns
    Weight at the specified index as a double value.
  */
  double weightAt(
    int weightIndex) const;

  /** \details
    Sets the weight at the specified index.

    \param weightIndex [in] Weight index.
    \param weight      [in] Weight value at the fit point with the specified index.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    The closer the weight value is to zero, the closer the curve is to a
    straight line near the corresponding control point. If the weight increases,
    the curve pulls toward the corresponding control point. If the weight
    decreases, the curve pushes from the corresponding control point.
  */
  OdGeNurbCurve2d& setWeightAt(
    int weightIndex,
    double weight);

  /** \details
    Returns the current evaluation mode.

    \returns
    true if and only if the evaluator optimization is turned on.

    \remarks
    If the method returns false, there is no optimization of an internal
    evaluator. This mode generally should be used if only one or two values per
    interval between two consecutive knots are evaluated.

    If the method returns true, then the evaluator optimization is turned on.
    This provides better performance if three or more values per knot interval
    are computed.
  */
  bool evalMode() const;

  /** \details
    Gets the parameters of C1 discontinuity.

    \param params [out] Parameters of C1 discontinuity.
    \param tol     [in] Geometric tolerance.

    \returns
    true if any parameters are returned; false otherwise.

    \remarks
    Currently not implemented.
  */
  bool getParamsOfC1Discontinuity(
    OdGeDoubleArray& params,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Gets the parameters of G1 discontinuity.

    \param params [out] Parameters of discontinuity.
    \param tol     [in] Geometric tolerance.

    \returns
    true if any parameters are returned; false otherwise.

    \remarks
    Currently not implemented.
  */
  bool getParamsOfG1Discontinuity(
    OdGeDoubleArray& params,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Sets the fit point at the specified index.

    \param fitPointIndex [in] Fit point index.
    \param point         [in] Fit point.

    \returns
    true if and only if the fit point is set successfully.
  */
  bool setFitPointAt(
    int fitPointIndex,
    const OdGePoint2d& point);

  /** \details
    Inserts the fit point at the specified index.

    \param fitPointIndex [in] Fit point index.
    \param point         [in] Fit point.

    \returns
    true if and only if the this spline was created with fit data and the fit
    point is set successfully.
  */
  bool addFitPointAt(
    int fitPointIndex,
    const OdGePoint2d& point);

  /** \details
    Removes a fit point with the specified index.

    \param fitPointIndex [in] Index of the fit point to be removed.

    \returns
    true if and only if the fit point is deleted successfully.

    \remarks
    The index must be greater than or equal to 0 and less than or equal to the
    number of fit points.
  */
  bool deleteFitPointAt(
    int fitPointIndex);

  /** \details
    Adds a control point at the specified knot.

    \param newKnot [in] Knot parameter to add the control point at.
    \param point   [in] Coordinates of the control point to be added.
    \param weight  [in] Weight to be assigned to the control point.

    \returns
    true if and only if the control point is added successfully.

    \remarks
    If called for a non-rational spline, the weight is ignored.
  */
  bool addControlPointAt(double newKnot, const OdGePoint2d& point, double weight = 1.0);

  /** \details
    Deletes a control point at the specified index.

    \param index [in] Index of the control point to be deleted.

    \returns
    true if and only if the control point is deleted successfully.
  */
  bool deleteControlPointAt(int index);

  /** \details
    Sets the specified knot parameterization if the spline is created by fit
    points.

    \param knotParam [in] Knot parameterization to be set.

    \returns
    true if and only if the knot parameterization is set successfully.
  */
  bool setFitKnotParameterization(OdGeKnotParameterization knotParam);

  /** \details
    Sets the fit tolerance that is used for interpolating fit points.

    \param fitTol [in] Geometric tolerance.

    \returns
    true if and only if this spline was constructed with fit data.
  */
  bool setFitTolerance(
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit tangents for this spline.

    \param startTangent [in] Starting tangent.
    \param endTangent   [in] Ending tangent.

    \returns
    true if and only if this spline was constructed with fit data.
  */
  bool setFitTangents(
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent);

  /** \details
    Sets the fit data for this spline.

    \param fitPoints    [in] Interpolation points.
    \param fitTol       [in] Geometric tolerance.
    \param startTangent [in] Starting tangent.
    \param endTangent   [in] Ending tangent.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    Recomputes the spline from the input fit points, start and end tangents, and
    fit tolerance. If fitTol is not 0, then the constructed spline may not pass
    through the fit points, but the maximum distance from any fit point to the
    constructed spline will be no greater than fitTol.
  */
  OdGeNurbCurve2d& setFitData(
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit data for this spline.

    \param fitKnots     [in] Fit knots.
    \param fitPoints    [in] Interpolation points.
    \param startTangent [in] Starting tangent.
    \param endTangent   [in] Ending tangent.
    \param fitTol       [in] Geometric tolerance.
    \param isPeriodic   [in] true if and only if this spline is to be periodic.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    Recomputes the spline from the input fit points, start and end tangents, and
    fit tolerance. The first parameter specifies the knot vector for the new
    spline. If fitTol is not 0, then the constructed spline may not pass through
    the fit points but the maximum distance from any fit point to the
    constructed spline will be no greater than fitTol.
  */
  OdGeNurbCurve2d& setFitData(
    const OdGeKnotVector& fitKnots,
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent,
    const OdGeTol& fitTol = OdGeContext::gTol,
    bool isPeriodic = false);

  /** \details
    Sets the fit data for this spline.

    \param degree    [in] Degree of curve.
    \param fitPoints [in] Interpolation points.
    \param fitTol    [in] Geometric tolerance.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    Recomputes the spline from the input fit points and makes it of the
    specified degree. If fitTol is not 0, then the constructed spline may not
    pass through the fit points. But the maximum distance from any fit point to
    the constructed spline will be no greater than fitTol.

    High degree values make a curve more flexible. For example degree 1 is used
    to define a polyline, degree 2 is used to define a NURB circle, etc.
  */
  OdGeNurbCurve2d& setFitData(
    int degree,
    const OdGePoint2dArray& fitPoints,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit data for this spline.

    \param fitPoints    [in] Interpolation points.
    \param startTangent [in] Starting tangent.
    \param endTangent   [in] Ending tangent.
    \param knotParam    [in] The knot parameterization method to be set.
    \param fitTol       [in] Geometric tolerance.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    Replaces any existing spline data (fit or NURBS) with the fit data
    fitPoints, startTangent, endTangent, knot parameterization, fitTolerance.
    fitPoints must contain 2 or more points.
  */
  OdGeNurbCurve2d& setFitData(
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent,
    OdGeKnotParameterization knotParam,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Purges the fit data defining this spline.

    \returns
    true if and only if this spline was constructed with fit data.

    \remarks
    The fit data consists of the fit points, fit tolerance, start tangent and
    end tangent.
    The definition of the spline is unchanged.
  */
  bool purgeFitData();

  /** \details
    Adds a new knot to the spline knot array.

    \param newKnot [in] New knot value.

    \returns
    Reference to this OdGeNurbCurve2d object.
  */
  OdGeNurbCurve2d& addKnot(
    double newKnot);

  /** \details
    Inserts a new knot to the spline knot array.

    \param newKnot [in] New knot.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    If there is already such knot, then the multiplicity of the knot is
    increased by 1.
  */
  OdGeNurbCurve2d& insertKnot(
    double newKnot);

  /** \details
    Sets the evaluation mode.

    \param evalMode [in] Input state of the evaluation mode.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    If evalMode is false, there is no optimization of an internal evaluator.
    This mode generally should be used if only one or two values per interval
    between two consecutive knots are evaluated.

    If evalMode is true, then the evaluator optimization is turned on. This
    provides better performance if three or more values per knot interval are
    computed.
  */
  OdGeNurbCurve2d& setEvalMode(
    bool evalMode = false);

  /** \details
    Joins the specified OdGeNurbCurve2d with this spline and returns a reference
    to the recomputed spline.

    \param curve [in] Spline to be joined with this spline.

    \returns
    Reference to this recomputed OdGeNurbCurve2d object.

    \remarks
    Both splines must be open and the start point of the input spline must be
    equal to the endpoint of the initial spline.
  */
  OdGeNurbCurve2d& joinWith(
    const OdGeNurbCurve2d& curve);

  /** \details
    Trims the OdGeNurbCurve2d by the new start and end parameters.

    \param newStartParam [in] New start parameter.
    \param newEndParam   [in] New end parameter.

    \returns
    Reference to this OdGeNurbCurve2d object.
  */
  OdGeNurbCurve2d& hardTrimByParams(
    double newStartParam,
    double newEndParam);

  /** \details
    Makes this spline rational (if it is not).

    \param weight [in] Weight to be applied to each control point.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    If this spline is already rational, the weight at each control point is
    multiplied by the specified weight.
  */
  OdGeNurbCurve2d& makeRational(
    double weight = 1.0);

  /** \details
    Makes the opened spline closed and returns a reference to this spline.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    If this spline is already closed, then the method does nothing.
    Closed spline is made by creating a smooth spline between the first and the
    last points and joining it with the initial spline.
  */
  OdGeNurbCurve2d& makeClosed();

  /** \details
    Makes this spline periodic (if it is not).

    \returns
    Reference to this OdGeNurbCurve2d object.
  */
  OdGeNurbCurve2d& makePeriodic();

  /** \details
    Makes a periodic spline non-periodic.

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    Currently not implemented.
  */
  OdGeNurbCurve2d& makeNonPeriodic();

  /** \details
    Makes a closed spline open.

    \returns
    Reference to this OdGeNurbCurve2d object.
  */
  OdGeNurbCurve2d& makeOpen();

  /** \details
    Increases the spline degree by the specified value and returns a reference
    to this spline object.

    \param plusDegree [in] Degree increment value.

    \returns
    Reference to this OdGeNurbCurve2d object.
  */
  OdGeNurbCurve2d& elevateDegree(
    int plusDegree);

  /** \details
    Assignment operator for the OdGeNurbCurve2d object.

    \param spline [in] Another OdGeNurbCurve2d object to be assigned to this OdGeNurbCurve2d object.

    \returns
    Reference to this OdGeNurbCurve2d object.
  */
  OdGeNurbCurve2d& operator =(
    const OdGeNurbCurve2d& spline);

  //////////////////////////////////////////////////////////////////////////
  // Overrides :

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details
    Retrieves the intersection points as well as the parameters of this spline
    said points.

    \param line2d  [in] 2D linear entity that intersects with this NURB curve.
    \param pnts2d [out] Receives an array of 2D points.
    \param tol     [in] Geometric tolerance.
    \param params [out] Receives an array of parameters.

    \returns
    true if and only if this spline intersects with the specified linear entity.
  */
  bool intersectWith(
    const OdGeLine2d &line2d,
    OdGePoint2dArray &pnts2d,
    const OdGeTol& tol = OdGeContext::gTol,
    OdGeDoubleArray *params = NULL) const;

  /** \details
    Sets the parameters for this spline according to the arguments.

    \param degree        [in] Degree of spline.
    \param knots         [in] Knot vector.
    \param controlPoints [in] Array of 2D control points.
    \param weights       [in] Array of weights.
    \param isPeriodic    [in] true if and only if the spline is to be periodic (closed).

    \returns
    Reference to this OdGeNurbCurve2d object.

    \remarks
    * If the weights parameter is not specified, a non-rational spline is
      constructed.
    * If the weights parameter is specified, then a rational spline is
      constructed, and controlPoints.length() must be equal to weights.length().
    * If isPeriodic is false, then knots.length() must be equal to
      controlPoints.length() + degree + 1.
    * If isPeriodic is true, then knots.length() must be equal to
      controlPoints.length(), the first and the last controlPoints must be
      equal, and the first and the last weights (if provided) must be equal.
  */
  void set(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint2dArray& controlPoints,
    const OdGeDoubleArray& weights,
    bool isPeriodic = false);

  /** \details
    Converts a source curve to a spline with the given tolerance and
    parametrization settings.

    \param source              [in] Source curve to convert.
    \param domain              [in] Interval of source curve to convert.
    \param tol                 [in] Accuracy of conversion.
    \param sameParametrization [in] Necessity of parametrization preserving during conversion.

    \returns
    Raw pointer to the converted spline or the NULL pointer if conversion
    failed.

    \remarks
    * If the source curve is already a spline, a copy is returned.
    * If a NULL-pointer to the source curve is provided or the curve has no
    analytic representation, a NULL-pointer is returned.
  */
  static OdGeNurbCurve2d* convertFrom(const OdGeCurve2d* source, const OdGeInterval& domain, const OdGeTol& tol = OdGeContext::gTol, bool sameParametrization = false);

  /** \details
    Converts a source curve to a spline with the given tolerance and
    parametrization settings in the native range of the source curve.

    \param source              [in] Source curve to convert.
    \param tol                 [in] Accuracy of conversion.
    \param sameParametrization [in] Necessity of parametrization preserving during conversion.

    \returns
    Raw pointer to the converted spline or a NULL pointer if conversion failed.

    \remarks
    * If the source curve is already a spline, a copy is returned.
    * If a NULL-pointer to the source curve is provided or the curve has no
    analytic representation, a NULL-pointer is returned.
  */
  static OdGeNurbCurve2d* convertFrom(const OdGeCurve2d* source, const OdGeTol& tol = OdGeContext::gTol, bool sameParametrization = false);
//DOM-IGNORE-BEGIN
protected:
  friend class OdDbGeEdgesDwgIO;
  friend class OdDbGeEdgesDxfIO;

  /** \details
    Sets the parameters for this spline according to the arguments.

    \param degree        [in] Degree of spline.
    \param knots         [in] Knot vector.
    \param controlPoints [in] Array of 2D control points.
    \param weights       [in] Array of weights.
    \param isPeriodic    [in] true if and only if this spline is to be periodic.
    \param fitPoints     [in] Interpolation points.
    \param startTangent  [in] Starting tangent.
    \param endTangent    [in] Ending tangent.
  */
  void setData(int degree, const OdGeKnotVector& knots,
    const OdGePoint2dArray& controlPoints, const OdGeDoubleArray& weights, bool isPeriodic,
    const OdGePoint2dArray& fitPoints,
    const OdGeVector2d& startTangent,
    const OdGeVector2d& endTangent);
//DOM-IGNORE-END
};

#include "TD_PackPop.h"

#endif

