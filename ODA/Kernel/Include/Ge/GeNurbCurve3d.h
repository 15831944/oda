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

#ifndef OD_GENURB3D_H
#define OD_GENURB3D_H /*!DOM*/

class OdGeEllipArc3d;
class OdGeLineSeg3d;
class OdGePolyline3d;
class OdGeKnotVector;

#include "Ge/GeSplineEnt3d.h"
#include "OdPlatformSettings.h"

#include "TD_PackPush.h"

/** \details
    This class represents non-uniform rational B-splines (NURBS) in 3D space.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeNurbCurve3d, Working with NURBS Curves>
*/
class GE_TOOLKIT_EXPORT OdGeNurbCurve3d : public OdGeSplineEnt3d
{
public:

  /** \details
    Default constructor for the OdGeNurbCurve3d class. Constructs an empty NURB curve.
  */
  OdGeNurbCurve3d();
  
  /** \details
    Copy constructor for the OdGeNurbCurve3d class.
    
    \param source [in]  Object to be copied.
  */
  OdGeNurbCurve3d(
    const OdGeNurbCurve3d& source);
  
  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param degree [in]  Degree of curve.
    \param knots [in]  Knot vector.
    \param controlPoints [in]  Array of 3D control points.
    \param isPeriodic [in]  true if and only if this spline is to be periodic.
    
    \remarks
    High degree values make curve more flexible. For example degree 1 is used
    to define a polyline, degree 2 is used to define a nurb circle, etc.
    
    A knot vector defines a sequence of parameter values that determine where and how 
    the control points affect the curve curve. The knot vector is a sequence of parameter 
    values and divides the parametric space into intervals, usually called knot spans.
    Values of knot vector must be provided in ascending order, for example: 
    (0, 0, 0, 1, 2, 3, 3, 3) can be valid while (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid.
    
    The control points specify the placement of the curve and adjust its shape.
    The quantity of control points must be at least the degree + 1.
    
    If isPeriodic is false, then knots.length() must equal controlPoints.length() + degree + 1
    If isPeriodic is true, then knots.length() must equal controlPoints.length(), the first and last 
    controlPoints must be equal.
  */
  OdGeNurbCurve3d(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& controlPoints,
    bool isPeriodic = false);
  
  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param degree [in]  Degree of curve.
    \param knots [in]  Knot vector.
    \param controlPoints [in]  Array of 3D control points.
    \param weights [in]  Array of 3D control points.
    \param isPeriodic [in]  true if and only if the curve is periodic.
    
    \remarks
    High degree values make curve more flexible. For example degree 1 is used
    to define a polyline, degree 2 is used to define a nurb circle, etc.
    
    A knot vector defines a sequence of parameter values that determine where and how 
    the control points affect the curve. The knot vector is a sequence of parameter 
    values and divides the parametric space into intervals, usually called knot spans.
    Values of knot vector must be provided in ascending order, for example: 
    (0, 0, 0, 1, 2, 3, 3, 3) can be valid while (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid.
    Knot vector does not affect the form of the nurb curve, it only affects its parameterization.
    
    The control points specify the placement of the curve and adjust its shape.
    The quantity of control points must be at least the degree + 1.
    
    The closer the weight value is to zero, the closer the curve is to a straight line near 
    the corresponding control point. If the weight increases, the curve pulls toward the 
    corresponding control point. If the weight decreases, the curve pushes from the corresponding control point.
    
    If isPeriodic is false, then knots.length() must equal controlPoints.length() + degree + 1
    If isPeriodic is true, then knots.length() must equal controlPoints.length(), the first and last 
    controlPoints must be equal.
  */
  OdGeNurbCurve3d(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& controlPoints,
    const OdGeDoubleArray& weights,
    bool isPeriodic = false);
  // TD Special
  
  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param degree [in]  Degree of curve.
    \param knots [in]  Knot vector.
    \param controlPoints [in]  A pointer to an array of 3D control points.
    \param numControlPoints [in]  A number of control points in the passed array.
    \param weights [in]  Array of 3D control points.
    \param numWeights [in]  A number of weight values in the passed array.
    \param isPeriodic [in]  true if and only if the curve is periodic.
    
    \remarks
    High degree values make curve more flexible. For example degree 1 is used
    to define a polyline, degree 2 is used to define a nurb circle, etc.
    
    A knot vector defines a sequence of parameter values that determine where and how 
    the control points affect the curve. The knot vector is a sequence of parameter 
    values and divides the parametric space into intervals, usually called knot spans.
    Values of knot vector must be provided in ascending order, for example: 
    (0, 0, 0, 1, 2, 3, 3, 3) can be valid while (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid.
    Knot vector does not affect the form of the nurb curve, it only affects its parameterization.
    
    The control points specify the placement of the curve and adjust its shape.
    The quantity of control points must be at least the degree + 1.
    
    The closer the weight value is to zero, the closer the curve is to a straight line near 
    the corresponding control point. If the weight increases, the curve pulls toward the 
    corresponding control point. If the weight decreases, the curve pushes from the corresponding control point.
    
    If isPeriodic is false, then knots.length() must equal controlPoints.length() + degree + 1
    If isPeriodic is true, then knots.length() must equal controlPoints.length(), the first and last 
    controlPoints must be equal.
  */
  OdGeNurbCurve3d(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint3d* controlPoints,
    OdUInt32 numControlPoints,
    const double* weights,
    OdUInt32 numWeights,
    bool isPeriodic = false);
  //
  
  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param degree       [in]  Degree of curve.
    \param fitPolyline  [in]  Input polyline fit points of which are interpolated by the curve.
    \param isPeriodic   [in]  true if and only if the curve is periodic.
    
    \remarks  
    Constructs a 3D spline by a degree and fit points provided by a polyline. 
    If isPeriodic is true then the first and the last fitPoints from fitPolyLine must be identical.
    
    High degree values make curve more flexible. For example degree 1 is used
    to define a segment, degree 2 is used to define a nurb circle, etc.
  */
  OdGeNurbCurve3d(
    int degree, 
    const OdGePolyline3d& fitPolyline,
    bool isPeriodic = false);
  
  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param fitPoints            [in]  Array of points through which to fit the curve.
    \param startTangent         [in]  Input start tangent vector.
    \param endTangent           [in]  Input end tangent vector.
    \param startTangentDefined  [in]  Input flag indicating whether startTangent should be used.
    \param endTangentDefined    [in]  Input flag indicating whether endTangent should be used.
    \param fitTol               [in]  Tolerance to which the curve should approximate fitPoints.
    
    \remarks
    Constructs a 3D curve interpolating a given array of 3D points within given tolerance and having 
    given derivatives at the start point and endpoint. If startTangentDefined is 0, then startTangent will be ignored 
    and the constructor will compute the start tangent vector. The same is true for endTangentDefined.
  */
  OdGeNurbCurve3d(
    const OdGePoint3dArray& fitPoints,
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent,
    bool startTangentDefined = true,
    bool endTangentDefined = true,
    const OdGeTol& fitTol = OdGeContext::gTol);
  
  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param fitPoints     [in]  Array of points through which to fit the curve.
    \param fitTolerance  [in]  Input maximal deviation of the curve from the fitPoints.
    
    \remarks   
    Constructs a 3D curve interpolating a given array of 3D points within given tolerance.
  */
  OdGeNurbCurve3d(
    const OdGePoint3dArray& fitPoints,
    const OdGeTol& fitTolerance = OdGeContext::gTol);
  
  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param fitPoints      [in]  Array of points through which to fit the curve.
    \param fitTangents    [in]  Input vectors interpolating the curve's derivatives at fitPoints.
    \param fitTolerance   [in]  Tolerance to which the curve should approximate fitPoints.
    \param isPeriodic     [in]  true if and only if the curve is periodic.
    
    \remarks
    Constructs a 3D curve interpolating a given array of 3D points within given tolerance. 
    If isPeriodic is true, then the first and the last fitPoints must be identical. 
    Same must be true for fitTangents.
  */
  OdGeNurbCurve3d(
    const OdGePoint3dArray& fitPoints,
    const OdGeVector3dArray& fitTangents,
    const OdGeTol& fitTolerance = OdGeContext::gTol,
    bool isPeriodic = false);

  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param fitPoints            [in]  Array of points through which to fit the curve.
    \param startTangent         [in]  Input start tangent vector.
    \param endTangent           [in]  Input end tangent vector.
    \param startTangentDefined  [in]  Input flag indicating whether startTangent should be used.
    \param endTangentDefined    [in]  Input flag indicating whether endTangent should be used.
    \param knotParam            [in]  knot parameterization which defines the knot values.
    \param fitTolerance         [in]  Tolerance to which the curve should approximate fitPoints.
    
    \remarks
    Constructs a 3D curve interpolating a given array of 3D points within given tolerance and having 
    given derivatives at the start point and endpoint. If startTangentDefined is 0, then startTangent will be ignored 
    and the constructor will compute the start tangent vector. The same is true for endTangentDefined.
    
    If fitTolerance is not 0, then the constructed spline may not pass through the fit points, but the maximum distance from any fit point to the constructed spline will be no greater than fitTolerance.
  */
  OdGeNurbCurve3d(
    const OdGePoint3dArray& fitPoints, 
    const OdGeVector3d& startTangent, 
    const OdGeVector3d& endTangent,
    bool startTangentDefined,
    bool endTangentDefined,
    OdGeKnotParameterization knotParam,
    const OdGeTol& fitTolerance = OdGeContext::gTol);

  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param ellipse   [in]  Input elliptic arc from which nurb curve is constructed.
    \param numSpans  [in]  The number of knot spans in nurb curve.
    
    \remarks
    Constructs curve geometrically identical to an elliptic arc.
    If numSpans == 0 (default) it is automatically calculated from the ellipse domain.   
  */
  OdGeNurbCurve3d(
    const OdGeEllipArc3d& ellipse, 
    int numSpans = 0);

  /** \details
    Constructor for the OdGeNurbCurve3d class.
    
    \param lineSeg   [in]  Input line segment from which nurb curve is constructed.
    
    \remarks       
    Constructs curve geometrically identical to a line segment.   
  */
  OdGeNurbCurve3d(
    const OdGeLineSeg3d& lineSeg);

  /** \details
    Returns the number of fit points.

    \returns
    Number of fit points as an integer value.
  */
  int numFitPoints() const;

  /** \details
    Gets the knot parametrization of the spline created with fit points.

    \param knotParam [out]  Receives the knot parametrization.

    \returns
    true if and only if knot parametrization is retrieved successfully.
  */
  bool getFitKnotParameterization(OdGeKnotParameterization& knotParam) const;

  /** \details
    Retrieves the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param point [out]  Receives the fit point. 

    \returns
    true if and only if 0 <= fitPointIndex < numFitPoints().
  */
  bool getFitPointAt(
    int fitPointIndex, 
    OdGePoint3d& point) const;

  /** \details
    Retrieves the fit tolerance that is used to construct this spline
    if this spline was constructed with fit points.
    
    \param fitTolerance [out]  Receives the geometric tolerance.

    \returns
    true if and only if this spline was constructed with fit points.
  */
  bool getFitTolerance(
    OdGeTol& fitTolerance) const;

  /** \details
    Retrieves the starting and ending tangents
    if this spline was constructed with fit data.
    
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.

    \returns
    true if and only if this spline was constructed with fit data.
  */
   bool getFitTangents(
    OdGeVector3d& startTangent,
    OdGeVector3d& endTangent) const;
 
  /** \details
    Retrieves the starting and ending tangents
    if this spline was constructed with fit data.
    
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.
    \param startTangentDefined [out]  Receives the if true, startTangent was used.
    \param endTangentDefined [out]  Receives the if true, endTangent was used.

    \returns
    true if and only if this spline was constructed with fit data.
  */
  bool getFitTangents(
    OdGeVector3d& startTangent,
    OdGeVector3d& endTangent,
    bool& startTangentDefined,
    bool& endTangentDefined) const;

  /** \details
    Retrieves all the fit data used to construct this spline
    if this spline was constructed with fit data.
    
    \param fitPoints [out]  Receives the interpolation points.
    \param fitTolerance [out]  Receives the geometric tolerance.
    \param tangentsExist [out]  Receives true if and only if tangents were used in constructing this spline.
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.

    \returns
    true if and only if this spline was constructed with fit data.
 
    \remarks
    startTangent and endTangent are meaningful if and only if tangentsExist == true.
  */
  bool getFitData(
    OdGePoint3dArray& fitPoints,
    OdGeTol& fitTolerance,
    bool& tangentsExist,
    OdGeVector3d& startTangent,
    OdGeVector3d& endTangent) const;

  /** \details
    Retrieves all the fit data used to construct this spline
    if this spline was constructed with fit data.
    
    \param fitPoints [out]  Receives the interpolation points.
    \param fitTolerance [out]  Receives the geometric tolerance.
    \param tangentsExist [out]  Receives true if and only if tangents were used in constructing this spline.
    \param startTangent [out]  Receives the starting tangent
    \param endTangent [out]  Receives the ending tangent.
    \param knotParam [out]  Receives the knot parameterization.

    \returns
    true if and only if this spline was constructed with fit data.
 
    \remarks
    startTangent and endTangent are meaningful if and only if tangentsExist == true.
  */
  bool getFitData(
    OdGePoint3dArray& fitPoints,
    OdGeTol& fitTolerance,
    bool& tangentsExist,
    OdGeVector3d& startTangent, 
    OdGeVector3d& endTangent,
    OdGeKnotParameterization& knotParam) const;

  // NURBS data query functions

  /** \details
    Retrieves the data used to define this spline.

    \param degree [out]  Receives the degree of spline.
    \param periodic [out]  Receives true if and only if this spline is periodic.
    \param rational [out]  Receives true if and only if this spline is rational.
    \param knots [out]  Receives the knot vector.
    \param controlPoints [out]  Receives an array of 3D control points.
    \param weights [out]  Receives an array of weights
    
    \remarks
    High degree values represent flexible curves. For example degree 1 is used
    to define a polyline, degree 2 is used to define a nurb circle, etc.
    
    A knot vector defines a sequence of parameter values that determine where and how 
    the control points affect the curve. The knot vector is a sequence of parameter 
    values and divides the parametric space into intervals, usually called knot spans.
    Values of knot vector must be provided in ascending order, for example: 
    (0, 0, 0, 1, 2, 3, 3, 3) can be valid while (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid.
    Knot vector does not affect the form of the nurb curve, it only affects its parameterization.
    
    The control points specify the placement of the curve and adjust its shape.
    The quantity of control points must be at least the degree + 1.
    
    The closer the weight value is to zero, the closer the curve is to a straight line near 
    the corresponding control point. If the weight increases, the curve pulls toward the 
    corresponding control point. If the weight decreases, the curve pushes from the corresponding control point.
    The weights array will be empty if the spline is not rational.
  */
  void getDefinitionData(
    int& degree,
    bool& rational,
    bool& periodic,
    OdGeKnotVector& knots,
    OdGePoint3dArray& controlPoints,
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

    \param weightIndex [in]  Weight index.

    \returns
    Weight at the specified index as a double value.
  */
  double weightAt(
    int weightIndex) const;

  /** \details
    Sets the weight at the specified index.

    \param weightIndex [in]  Weight index.
    \param weight [in]  Weight at the specified fit point.

    \returns
    Reference to this OdGeNurbCurve3d object.

    \remarks
    The closer the weight value is to zero, the closer the curve is to a straight line near
    the corresponding control point. If the weight increases, the curve pulls toward the
    corresponding control point. If the weight decreases, the curve pushes from the corresponding control point.
  */
  OdGeNurbCurve3d& setWeightAt(
    int weightIndex,
    double weight);

  /** \details
    Returns the current evaluation mode.

    \returns
    true if and only if the evaluator optimization is turned on. 

    \remarks
    If the method returns false, there is no optimization of an internal evaluator. 
    This mode generally should be used if only one or two values per interval between two consecutive knots are evaluated.  

    If the method returns true, then the evaluator optimization is turned on. 
    This provides better performance if three or more values per knot interval are computed.
  */
  bool evalMode() const;

  /** \details
    Gets the parameters of G1 discontinuity.

    \param params [out] Parameters of discontinuity.
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

  // Fit data edit functions

  /** \details
    Sets the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Fit point. 

    \returns
    true if and only if the fit point is set successfully.
  */
  bool setFitPointAt(
    int fitPointIndex, 
    const OdGePoint3d& point);

  /** \details
    Inserts the fit point at the specified index.

    \param fitPointIndex [in]  Fit point index.
    \param point [in]  Fit point.

    \returns
    true if and only if this spline was created with fit data
    and the fit point is set successfully.
  */
  bool addFitPointAt(
    int fitPointIndex, 
    const OdGePoint3d& point);

  /** \details
    Removes a fit point with the specified index.

    \param fitPointIndex [in]  The index of fit point that is to be removed.

    \returns
    true if and only if the fit point is deleted successfully.

    \remarks
    The index must be greater than or equal to 0 and less than or equal to the number of fit points.
  */
  bool deleteFitPointAt(
    int fitPointIndex);

  /** \details
    Adds a control point at the specified knot.
    
    \param newKnot [in]  The knot parameter to add the control point at.
    \param point [in]  Coordinates of the control point to be added.
    \param weight [in] The weight to be assigned to the control point.

    \returns
    true if and only if the control point is added successfully.
    
    \remarks
    If called for a non-rational spline, weight is ignored.
  */
  bool addControlPointAt(double newKnot, const OdGePoint3d& point, double weight = 1.0);

  /** \details
    Deletes a control point at the specified index.
    
    \param index [in]  The index of the control point to be deleted.

    \returns
    true if and only if the control point is deleted successfully.
  */
  bool deleteControlPointAt(int index);

  /** \details
    Sets the specified knot parameterization if the spline is created by fit points.

    \param knotParam [in]  The knot parameterization to be set.

    \returns
    true if and only if the knot parameterization is set successfully.
  */
  bool setFitKnotParameterization(OdGeKnotParameterization knotParam);

  /** \details
    Sets the fit tolerance that is used for interpolating fit points.

    \param fitTol [in]  Geometric tolerance.
            
    \returns
    true if and only if this spline was constructed with fit data and tolerance was successfully set.
  */
  bool setFitTolerance(
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit tangents for this spline.
    
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.

    \returns
    true if and only if this spline was constructed with fit data and fit tangents were successfully set. 
  */
  bool setFitTangents(
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent);
 
  /** \details
    Sets the fit tangents for this spline.
    
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
    \param startTangentDefined [in]  Defines if the specified value of starting tangent is to be used.
    \param endTangentDefined [in]  Defines if the specified value of starting tangent is to be used.

    \returns
    true if and only if this spline was constructed with fit data and fit tangents were successfully set. 
  */  
  bool setFitTangents(
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent,
    bool startTangentDefined,
    bool endTangentDefined);

  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.

    \returns
    Reference to this OdGeNurbCurve3d object.

    \remarks
    Recomputes the spline from the input fit points, start and end tangents, and fit tolerance. 
    If fitTol is not 0, then the constructed spline may not pass through the fit points, but the 
    maximum distance from any fit point to the constructed spline will be no greater than fitTol.
  */
  OdGeNurbCurve3d& setFitData(
    const OdGePoint3dArray& fitPoints,
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit data for this spline.
    
    \param fitKnots [in] Fit knots.
    \param fitPoints [in]  Interpolation points.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
    \param fitTol [in]  Geometric tolerance.
    \param isPeriodic [in]  true if and only if this spline is to be periodic.

    \returns
    Reference to this OdGeNurbCurve3d object.

    \remarks
    Recomputes the spline from the input fit points, start and end tangents, and fit tolerance. 
    The first parameter specifies the knot vector for the new spline. If fitTol is not 0, 
    then the constructed spline may not pass through the fit points but the maximum distance 
    from any fit point to the constructed spline will be no greater than fitTol.
  */  
  OdGeNurbCurve3d& setFitData(
    const OdGeKnotVector& fitKnots,
    const OdGePoint3dArray& fitPoints,
    const OdGeVector3d& startTangent,
    const OdGeVector3d& endTangent,
    const OdGeTol& fitTol = OdGeContext::gTol,
    bool isPeriodic = false);

  /** \details
    Sets the fit data for this spline.
    
    \param degree [in]  Degree of curve.
    \param fitPoints [in]  Interpolation points.
    \param fitTol [in]  Geometric tolerance.

    \returns
    Reference to this OdGeNurbCurve3d object.
    
    \remarks
    Recomputes the spline from the input fit points and makes it of the specified degree. 
    If fitTol is not 0, then the constructed spline may not pass through the fit points. 
    But the maximum distance from any fit point to the constructed spline will be no greater than fitTol.

    High degree values make curve more flexible. For example degree 1 is used
    to define a polyline, degree 2 is used to define a nurb circle, etc.   
  */
  OdGeNurbCurve3d& setFitData(
    int degree,
    const OdGePoint3dArray& fitPoints,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Sets the fit data for this spline.
    
    \param fitPoints [in]  Interpolation points.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
    \param knotParam [in]  The knot parameterization method to be set.
    \param fitTol [in]  Geometric tolerance.

    \returns
    Reference to this OdGeNurbCurve3d object.
    
    \remarks
    Replaces any existing spline data (fit or NURBS) with the fit data fitPoints, startTangent, 
    endTangent, knot parameterization, fitTolerance. fitPoints must contain 2 or more points.
  */
  OdGeNurbCurve3d& setFitData(
    const OdGePoint3dArray& fitPoints,                                             
    const OdGeVector3d& startTangent, 
    const OdGeVector3d& endTangent,
    OdGeKnotParameterization knotParam,
    const OdGeTol& fitTol = OdGeContext::gTol);

  /** \details
    Purges the fit data defining this spline.

    \returns
    true if and only if this spline was constructed with fit data.
    
    \remarks
    The fit data consists of the fit points, fit tolerance, start tangent, and end tangent.
    The definition of the spline is unchanged. 
  */
  bool purgeFitData();

  /** \details
    Adds new knot to the spline knot array.

    \param newKnot [in]  New knot value.

    \returns
    Reference to this OdGeNurbCurve3d object.
  */
  OdGeNurbCurve3d& addKnot(double newKnot);

  /** \details
    Inserts new knot to the spline knot array.

    \param newKnot [in]  New knot.

    \returns
    Reference to this OdGeNurbCurve3d object.

    \remarks
    If there is already such knot, then the multiplicity of the knot is increased by 1.
  */
  OdGeNurbCurve3d& insertKnot(double newKnot);

  /** \details
    Sets the evaluation mode.
    
    \param evalMode  [in]  Input state of the evaluation mode.

    \returns
    Reference to this OdGeNurbCurve3d object.
    
    \remarks
    If evalMode is false, there is no optimization of an internal evaluator.
    This mode generally should be used if only one or two values per interval between two consecutive knots are evaluated.

    If evalMode is true, then the evaluator optimization is turned on.
    This provides better performance if three or more values per knot interval are computed.
  */
  OdGeNurbCurve3d& setEvalMode(
    bool evalMode = false);

  /** \details
    Joins the specified OdGeNurbCurve3d with this spline and returns a reference to the recomputed spline. 
    
    \param curve        [in]  Spline to be joined with this spline.
    \param iTolerance   [in]  Geometric tolerance.

    \returns
    Reference to this recomputed OdGeNurbCurve3d object.

    \remarks
    Both splines must be open and the start point of the input spline must be equal to the endpoint of the initial spline.
  */
  OdGeNurbCurve3d& joinWith(
    const OdGeNurbCurve3d& curve, const OdGeTol &iTolerance = OdGeContext::gTol);

  /** \details
    Trims the OdGeNurbCurve3d by the new start and end parameters.
    
    \param newStartParam [in]  New start parameter.  
    \param newEndParam [in]  New end parameter.

    \returns
    Reference to this OdGeNurbCurve3d object.
  */
  OdGeNurbCurve3d& hardTrimByParams(
    double newStartParam,
    double newEndParam);

  /** \details
    Makes this spline rational (if it is not).
    
    \param weight [in]  Weight to be applied to each control point. 

    \returns
    Reference to this OdGeNurbCurve3d object.
    
    \remarks
    If this spline was already rational, the weight at each control point is multiplied by the specified weight.
  */
  OdGeNurbCurve3d& makeRational(
    double weight = 1.0);

  /** \details
    Makes the opened spline closed, and returns a reference to this spline.

    \returns
    Reference to this OdGeNurbCurve3d object.
    
    \remarks
    If this spline is already closed, then the method does nothing.
    Closed spline is made by creating a smooth spline between the first and the last points and joining it with the initial spline.
  */
  OdGeNurbCurve3d& makeClosed();

  /** \details
    Makes this spline periodic (if it is not).

    \returns
    Reference to this OdGeNurbCurve3d object.
  */
  OdGeNurbCurve3d& makePeriodic();

  /** \details
    Makes periodic spline non-periodic. 

    \returns
    Reference to this OdGeNurbCurve3d object.
    
    \remarks
    Currently not implemented.
  */
  OdGeNurbCurve3d& makeNonPeriodic();

  /** \details
    Makes closed spline open. 

    \returns
    Reference to this OdGeNurbCurve3d object.
  */
  OdGeNurbCurve3d& makeOpen();

  /** \details
    Increases spline degree by the specified value and returns a reference to this spline object.
    
    \param plusDegree [in]  Degree increment value.

    \returns
    Reference to this OdGeNurbCurve3d object.
  */
  OdGeNurbCurve3d& elevateDegree(int plusDegree);

  /** \details
    Assignment operator for the OdGeNurbCurve3d object.

    \param spline [in]  Nurb spline to assign.
    
    \returns
    Reference to this recomputed OdGeNurbCurve3d object.
  */
  OdGeNurbCurve3d& operator =(
    const OdGeNurbCurve3d& spline);

  ///////////////////////////////////////////////////////////////////////////
  // TD Special :

  // These functions are only for internal use
  //DOM-IGNORE-BEGIN
  TD_USING(OdGeSplineEnt3d::evalPoint);

  OdGePoint3d evalPoint(
    double param, 
    int hint) const;  

  // This is needed in surface (for rational case only)
  void evalPointDivider(
    double param, 
    OdGePoint3d& point, 
    double& divider, 
    int hint) const;
  // NURBS data edit functions
  
  /** \details
    Sets the parameters for this spline according to the arguments. 

    \param degree [in]  Degree of spline.
    \param knots [in]  Knot vector.
    \param controlPoints [in]  Array of 3D control points.
    \param weights [in]  Array of weights
    \param isPeriodic [in]  true if and only if this spline is to be periodic.
    
    \remarks
    High degree values make curve more flexible. For example degree 1 is used
    to define a polyline, degree 2 is used to define a nurb circle, etc.
    
    A knot vector defines a sequence of parameter values that determine where and how 
    the control points affect the curve curve. The knot vector is a sequence of parameter 
    values and divides the parametric space into intervals, usually called knot spans.
    Values of knot vector must be provided in ascending order, for example: 
    (0, 0, 0, 1, 2, 3, 3, 3) can be valid while (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid.
    Knot vector does not affect the form of the nurb curve, it only affects its parameterization.
    
    The control points specify the placement of the curve and adjust its shape.
    
    A rational spline is constructed, and controlPoints.length() must equal weights.length()
    If isPeriodic is false, then knots.length() must equal controlPoints.length() + degree + 1
    If isPeriodic is true, then knots.length() must equal controlPoints.length(), the first and last controlPoints must be equal
    and the first and last weights (if provided) must be equal.
  */
  void set(
    int degree,
    const OdGeKnotVector& knots,
    const OdGePoint3dArray& controlPoints,
    const OdGeDoubleArray& weights,
    bool isPeriodic = false);

  /** \details
    Calculates fit data for the spline constructed by control points. 

    \returns
    true if and only if fit data is calculated successfully.
        
    \remarks
    Currently the method calculates fit data only for NURB curves of degree less than or equal to three.  
  */
  bool buildFitData(OdGeKnotParameterization knotParam = OdGe::kChord);

  /** \details
    Converts source curve to spline with given tolerance and parametrization settings.
    
    \param source [in] Source curve to convert.
    \param domain [in] Interval of source curve to convert.
    \param tol [in]  Accuracy of conversion.
    \param sameParametrization [in]  Necessity of parametrization preserving during conversion.

    \returns
    Raw pointer to converted spline or null pointer if conversion failed.
    
    \remarks
    * If source curve already is spline, a copy is returned.
    * If provided null-pointer to source curve or curve has no analytic representation, a null-pointer is returned
  */
  static OdGeNurbCurve3d* convertFrom(const OdGeCurve3d* source, const OdGeInterval& domain, const OdGeTol& tol = OdGeContext::gTol, bool sameParametrization = false);

  /** \details
    Converts source curve to spline with given tolerance and parametrization settings in native range of source curve.
    
    \param source [in] Source curve to convert.
    \param tol [in]  Accuracy of conversion.
    \param sameParametrization [in]  Necessity of parametrization preserving during conversion.
    
    \returns
    Raw pointer to converted spline or null pointer if conversion failed.
    
    \remarks
    * If source curve already is spline, a copy is returned.
    * If provided null-pointer to source curve or curve has no analytic representation, a null-pointer is returned
  */
  static OdGeNurbCurve3d* convertFrom(const OdGeCurve3d* source, const OdGeTol& tol = OdGeContext::gTol, bool sameParametrization = false);

protected:
  /** \details
    Modifies the position and tangent of the point with a specified parameter.

    \param param [in]  Parameter value of the point.
    \param point [in]  New position of the point.
    \param deriv [in]  New value of the tangent vector at the point.
           
    \returns
    true if and only if the point is adjusted successfully.
  */
  bool modifyPositionAndTangent(double param, const OdGePoint3d& point, const OdGeVector3d* deriv);
  void makeClosedFit();

  /** \details
    Sets the parameters for this spline according to the arguments.

    \param degree[in]  Degree of spline.
    \param controlPoints[in]  Array of 3D control points.
    \param knots[in]  Knot vector.
    \param weights[in]  Array of weights
    \param fitPoints [in]  Interpolation points.
    \param startTangent [in]  Starting tangent.
    \param endTangent [in]  Ending tangent.
    \param isStartTangentDefined [in]  Defines if the specified value of starting tangent is to be used.
    \param isEndTangentDefined [in]  Defines if the specified value of starting tangent is to be used.
    \param isPeriodic[in]  true if and only if this spline is to be periodic.
    \param fitTol [in]  Geometric tolerance.
  */
  void setData(int degree, const OdGePoint3dArray& controlPoints, const OdGeKnotVector& knots, const OdGeDoubleArray& weights,
    const OdGePoint3dArray& fitPoints, const OdGeVector3d& startTangent, const OdGeVector3d& endTangent,
    bool isStartTangentDefined, bool isEndTangentDefined, bool isPeriodic, const OdGeTol& fitTol);

  /** \details
    Sets the knot tolerance of this spline.

    \param tol [in]  Knot tolerance.
  */
  void setKnotTolerance(double tol);
  //DOM-IGNORE-END
};

#include "TD_PackPop.h"

#endif // OD_GENURB3D_H

