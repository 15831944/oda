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

#ifndef OD_GENURBSF_H
#define OD_GENURBSF_H /*!DOM*/

#include "Ge/GeSurface.h"
#include "Ge/GeKnotVector.h"
#include "Ge/GeEllipCylinder.h"
#include "Ge/GeEllipCone.h"
#include "OdPlatformSettings.h"
#include "TD_PackPush.h"

class OdGeNurbCurve3d;

/** \details
    Defines the size of derivatives array.
*/
static const int derivArraySize = 3;
/** \details
    Defines VectorDerivArray type.
*/
typedef OdGeVector3d VectorDerivArray[derivArraySize][derivArraySize];


/** \details
    This class represents non-uniform, rational B-Spline (NURBS) surfaces.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes>

    \sa
    <link ge_OdGeNurbSurface, Working with NURBS Surfaces>
*/
class GE_TOOLKIT_EXPORT OdGeNurbSurface : public OdGeSurface
{
public:


  /** \details
    Default constructor for the OdGeNurbSurface class. Constructs an empty NURB surface.
  */
  OdGeNurbSurface();
  
  /** \details
    Constructor for the OdGeNurbSurface class.
    
    \param degreeInU [in]  Degree in the U direction. 
    \param degreeInV [in]  Degree in the V direction.
    \param propsInU [in]  Properties in the U direction.
    \param propsInV [in]  Properties in the V direction.
    \param numControlPointsInU [in]  Number of control points in the U direction.
    \param numControlPointsInV [in]  Number of control points in the V direction.
    \param controlPoints [in]  Array of 3D control points.
    \param weights [in]  Array of weights.
    \param uKnots [in]  Knot vector in the U direction.
    \param vKnots [in]  Knot vector in the V direction.
    \param tol [in]  Geometric tolerance.
   
    \remarks
    Constructs a NURB surface defined by input data.
    propsInU and propsInV utilize OdGe::NurbSurfaceProperties values.
  */
  OdGeNurbSurface(
    int degreeInU, 
    int degreeInV, 
    int propsInU, 
    int propsInV,
    int numControlPointsInU, 
    int numControlPointsInV,
    const OdGePoint3dArray& controlPoints,
    const OdGeDoubleArray& weights,
    const OdGeKnotVector& uKnots,
    const OdGeKnotVector& vKnots,
    const OdGeTol& tol = OdGeContext::gTol);
  
  /** \details
    Copy constructor for the OdGeNurbSurface class.
    
    \param source [in]  Object to be copied.
  */
  OdGeNurbSurface(
    const OdGeNurbSurface& source);
  
  /** \details
    Constructor for the OdGeNurbSurface class.
    
    \param cylinder [in]  Input cylinder from which a nurb surface is constructed.
  */
  OdGeNurbSurface(const OdGeEllipCylinder& cylinder);
  
  /** \details
    Constructor for the OdGeNurbSurface class.
    
    \param cone [in]  Input cone from which a nurb surface is constructed.
  */
  OdGeNurbSurface(const OdGeEllipCone& cone);

  /** \details
    Assignment operator for the OdGeNurbSurface object.

    \param nurb [in]  Nurb surface to assign.
    
    \returns
    Reference to this OdGeNurbSurface object.
  */
  OdGeNurbSurface& operator =(
    const OdGeNurbSurface& nurb);

  
  /** \details
    Checks if this surface is rational in the U direction.

    \returns
    true if and only if this surface is rational in the U direction.
    
    \remarks
    The U direction is considered rational when it has weighted control points.
  */
  bool isRationalInU() const;

  /** \details
    Checks if this surface is periodic in the U direction, and returns the period.

    \param period [out]  Period in the U direction.

    \returns
    true if and only if this surface is periodic in the U direction, and returns the period.
    
    \remarks
    If a surface is closed in one direction (opposite sides are the same line in object space) and the 
    parameterization and derivatives also match at these boundaries, the surface is periodic in the parameter direction.
  */
  bool isPeriodicInU(double& period) const;

  /** \details
    Checks if this surface is rational in the V direction.

    \returns
    true if and only if this surface is rational in the V direction.
    
    \remarks
    The V direction is considered rational when it has weighted control points.
  */
  bool isRationalInV() const;

  /** \details
    Checks if this surface is periodic in the V direction, and returns the period.

    \param period [out]  Period in the V direction.

    \returns
    true if and only if this surface is periodic in the U direction, and returns the period.
    
    \remarks
    If a surface is closed in one direction (opposite sides are the same line in object space) and the 
    parameterization and derivatives also match at these boundaries, the surface is periodic in the parameter direction.
  */
  bool isPeriodicInV(double& period) const;
  
  /** \details
    Returns a description of the singularity in the U direction:

    \returns
    Description of the singularity in the U direction as a OdGe::NurbSurfaceProperties value.
  */
  int singularityInU() const;

  /** \details
    Returns a description of the singularity in the V direction:

    \returns
    Description of the singularity in the V direction as a OdGe::NurbSurfaceProperties value.
  */
  int singularityInV() const;

  /** \details
    Returns the degree in the U direction.

    \returns
    Degree in the U direction as an integer value.
    
    \remarks
    High degree values represent flexible surfaces.
  */
  int degreeInU() const;

  /** \details
    Returns the number of control points in the U direction.

    \returns
    Number of control points in the U direction as an integer value.
  */
  int numControlPointsInU() const;
  
  /** \details
    Returns the degree in the V direction.

    \returns
    Degree in the V direction as an integer value.
    
    \remarks
    High degree values represent flexible surfaces.
  */
  int degreeInV() const;
  
  /** \details
    Returns the number of control points in the V direction.

    \returns
    Number of control points in the V direction as an integer value.
  */
  int numControlPointsInV() const;

  /** \details
    Retrieves the array of control points.
    
    \param controlPoints [out] Receives an array of 3D control points.
    
    \remarks
    The control points specify the placement of the surface and adjust its shape.
  */
  void getControlPoints(
    OdGePoint3dArray& controlPoints) const;
    
  /** \details
    Retrieves the array of weights.
    
    \param weights [out]  Array of weight values.

    \returns
    true if and only if this surface has weighted control points.
    
    \remarks
    The weight array is an array of double values, each value is associated with a corresponding control
    vertex. If the weight increases, the surface pulls toward the corresponding control point. 
    If the weight decreases, the surface pushes from the corresponding control point.
  */
  bool getWeights(
    OdGeDoubleArray& weights) const;
  
  /** \details
    Returns the number of knots in the U direction.

    \returns
    Number of knots in the U direction as an integer value.
  */
  int numKnotsInU() const;
  
  /** \details
    Retrieves the knot vector in the U direction.
    
    \param uKnots [out]  Knot vector in the U direction.
    
    \remarks
    A knot vector defines a sequence of parameter values that determine where and how 
    the control points affect the surface.
    Values of knot vector are provided in ascending order, for example: 
    (0, 0, 0, 1, 2, 3, 3, 3) can be valid while (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid.
  */
  void getUKnots(
    OdGeKnotVector& uKnots) const;
  
  /** \details
    Returns the number of knots in the V direction.

    \returns
    Number of knots in the V direction as an integer value.
  */
  int numKnotsInV() const;
  
  /** \details
    Retrieves the knot vector in the V direction.
    
    \param vKnots [out]  Knot vector in the V direction.
    
    \remarks
    A knot vector defines a sequence of parameter values that determine where and how 
    the control points affect the surface.
    Values of knot vector are provided in ascending order, for example: 
    (0, 0, 0, 1, 2, 3, 3, 3) can be valid while (0, 0, 2, 0, 3, 1, 3, 3) cannot be valid.
  */
  void getVKnots(
    OdGeKnotVector& vKnots) const;
    
  /** \details
    Retrieves the data used to define this surface.
    
    \param degreeInU [out]  Receives the Degree in the U direction. 
    \param degreeInV [out]  Receives the Degree in the V direction.
    \param propsInU [out]  Receives the properties in the U direction.
    \param propsInV [out]  Receives the properties in the V direction.
    \param numControlPointsInU [out]  Receives the number of control points in the U direction.
    \param numControlPointsInV [out]  Receives the number of control points in the V direction.
    \param controlPoints [out]  Receives an array of 3D control points.
    \param weights [out]  Receives an array of weights.
    \param uKnots [out]  Receives the knot vector in the U direction.
    \param vKnots [out]  Receives the knot vector in the V direction.
   
    \remarks
    propsInU and propsInV utilize OdGe::NurbSurfaceProperties values.
  */
  void getDefinition(
    int& degreeInU, 
    int& degreeInV,
    int& propsInU, 
    int& propsInV,
    int& numControlPointsInU,
    int& numControlPointsInV,
    OdGePoint3dArray& controlPoints,
    OdGeDoubleArray& weights,
    OdGeKnotVector& uKnots,
    OdGeKnotVector& vKnots) const;

  /** \details
    Sets the parameters for this spline according to the arguments, 
    and returns a reference to this spline.

    \param degreeInU [in]  Degree in the U direction. 
    \param degreeInV [in]  Degree in the V direction.
    \param propsInU [in]  Properties in the U direction.
    \param propsInV [in]  Properties in the V direction.
    \param numControlPointsInU [in]  Number of control points in the U direction.
    \param numControlPointsInV [in]  Number of control points in the V direction.
    \param controlPoints [in]  Array of 3D control points.
    \param weights [in]  Array of weights
    \param uKnots [in]  Knot vector in the U direction.
    \param vKnots [in]  Knot vector in the V direction.
    \param tol [in]  Geometric tolerance.

    \returns
    Reference to this OdGeNurbSurface object.
   
    \remarks
    propsInU and propsInV utilize OdGe::NurbSurfaceProperties values.
  */
  OdGeNurbSurface& set(
    int degreeInU, 
    int degreeInV,
    int propsInU, 
    int propsInV,
    int numControlPointsInU,
    int numControlPointsInV,
    const OdGePoint3dArray& controlPoints,
    const OdGeDoubleArray& weights,
    const OdGeKnotVector& uKnots,
    const OdGeKnotVector& vKnots,
    const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Sets the fit data for this spline and returns a reference to this surface object.
    
    \param fitPoints      [in]  Interpolation points.
    \param arrTangentsInU [in]  An array of tangent vectors for U direction.
    \param arrTangentsInV [in]  An array of tangent vectors for V direction.
    \param arrMixedDerivs [in]  An array of derivatives for both directions.
    \param uKnots         [in]  Knot vector in the U direction.
    \param vKnots         [in]  Knot vector in the V direction.
    \param tol            [in]  Geometric tolerance.  

    \returns
    Reference to this OdGeNurbSurface object.   
  */
  OdGeNurbSurface& setFitData(
      const OdGePoint3dArray& fitPoints,
      const OdGeVector3dArray& arrTangentsInU,
      const OdGeVector3dArray& arrTangentsInV, 
      const OdGeVector3dArray& arrMixedDerivs,
      const OdGeKnotVector& uKnots,
      const OdGeKnotVector& vKnots,
      const OdGeTol& tol);
  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  // computes isolines
  
  /** \details
    Computes and retrieves the isoline for the specified value of V parameter.

    \param V [in]  Value of V parameter for which an isoline is computed.
    \param isoline [out]  Receives the isoline for the specified value of V parameter. 
  */
  void computeVIsoLine(
    double V, 
    OdGeNurbCurve3d& isoline) const;

  /** \details
    Computes and retrieves the isoline for the specified value of U parameter.

    \param U [in]  Value of U parameter for which an isoline is computed.
    \param isoline [out]  Receives the isoline for the specified value of U parameter. 
  */
  void computeUIsoLine(
    double U, 
    OdGeNurbCurve3d& isoline) const;

  /** \details
    Returns the 2D pair of parameter values of a point on this surface.

    \param point [in]  Point to be evaluated.
    \param tol [in]  Geometric tolerance.

    \returns
    2D pair of parameter values of a point on this surface as a OdGePoint2d object.

    \remarks
    The returned parameters specify a point within tol of point.
    If point is not on this surface, the results are unpredictable.
    If you are not sure the point is on this surface, use 
    isOn() instead of this function. This method is slower than paramOf, 
    but more accurate.
  */
  OdGePoint2d paramOfPrec(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;
   
  /** \details
    Retrieves the derivatives at the point specified by param.

    \param param [in]  Parameter to be evaluated.
    \param numDeriv [in]  Number of derivatives to be computed.
    \param derivatives [out]  Receives an array of derivatives at the point corresponding to param.

    \returns
    true if and only if derivatives were calculated successfully.

    \remarks
    Not implemented. For third-party use only.
  */
  bool getDerivativesAt(
    const OdGePoint2d& param, 
    OdUInt32 numDeriv,
    VectorDerivArray derivatives) const;

  // the index in "m_controlPoints" of control *point* (i,j):

//DOM-IGNORE-BEGIN
  /** \details
    Returns the index of the control point (i,j):

    \param i [in]  Index of the control point in the U direction.
    \param j [in]  Index of the control point in the V direction.

    \returns
    Index of the control point (i,j) as an integer value.
  */
  int loc(
    int i, 
    int j) const;
//DOM-IGNORE-END
  /** \details
    Converts source surface to spline with given tolerance and parametrization settings.
    
    \param source [in]  Source surface to convert.
    \param domain [in]  UV-range of source surface to convert.
    \param tol [in]  Accuracy of conversion.
    \param sameParametrization [in]  Necessity of parametrization preserving during conversion.
    
    \returns
    Raw pointer to converted spline or null pointer if conversion failed.

    \remarks
    * If source surface already is spline, a copy is returned.
    * If provided null-pointer to source surface or surface has no analytic representation, a null-pointer is returned
    * If input domain is not coincident to native domain, the intersection of input and native domain is used to build nurb 
  */
  static OdGeNurbSurface* convertFrom(const OdGeSurface* source, const OdGeUvBox& domain, const OdGeTol& tol = OdGeContext::gTol, bool sameParametrization = false);
  
  /** \details
  Converts source surface to spline with given tolerance and parametrization settings in native domain of source surface.

  \param source [in]  Source surface to convert.
  \param tol [in]  Accuracy of conversion.
  \param sameParametrization [in]  Necessity of parametrization preserving during conversion.

  \returns
  Raw pointer to converted spline or null pointer if conversion failed.

  \remarks
  * If source surface already is spline, a copy is returned.
  * If provided null-pointer to source surface or surface has no analytic representation, a null-pointer is returned
  * If input domain is not coincident to native domain, the intersection of input and native domain is used to build nurb
  */
  static OdGeNurbSurface* convertFrom(const OdGeSurface* source, const OdGeTol& tol = OdGeContext::gTol, bool sameParametrization = false);

  /** \details
    Defines OdGeNurbSurface joining rules.

    \remarks
    Used for joinWith method.
  */
  enum ConnectionSide
  {
    /** The object will be connected by lower bound in the U direction */
    kLowerU = 1, 

    /** The object will be connected by upper bound in the U direction */
    kUpperU = 2, 

    /** The object will be connected by lower bound in the V direction */
    kLowerV = 3, 

    /** The object will be connected by upper bound in the V direction */
    kUpperV = 4
  };

  /** \details
    Joins the specified OdGeNurbSurface with this OdGeNurbSurface by specified sides and returns a reference to the recomputed OdGeNurbSurface.
    
    \param surface [in]  Surface to be joined with this surface.  
    \param thisConnectionSide [in]  The side of this surface to which another surface will be attached.
    \param surfaceConnectionSide [in]  The side of the other surface to which this surface will be attached.
    \param tol [in]  Accuracy of joining.

    \returns
    Reference to this recomputed OdGeNurbSurface object.

    \remarks
    Joining works in the case of coinciding edges of the same parameter (U-U, V-V).
  */
  OdGeNurbSurface& joinWith(const OdGeNurbSurface& surface, 
    ConnectionSide thisConnectionSide, 
    ConnectionSide surfaceConnectionSide, 
    const OdGeTol& tol = OdGeContext::gTol);

  /** \details
    Increases spline degree for corresponding dimension (U or V) by the specified value and returns a reference to this spline object.

    \param iByU [in]  true means U-dimension.
    \param iPlusDegree [in]  Degree increment value.

    \returns
    Reference to this OdGeNurbSurface object.
  */
  OdGeNurbSurface& elevateDegree(bool iByU, int iPlusDegree);

  /** \details
    Inserts new knot of provided multiplicity to the spline knot array for corresponding dimension (U or V).

    \param iByU [in]  true means U-dimension.
    \param iNewKnot [in]  New knot.
    \param iTimes [in]  Times to insert knot.

    \returns
    Reference to this OdGeNurbSurface object.

    \remarks
    If there is already such knot, then the multiplicity of the knot is increased by iTimes.
  */
  OdGeNurbSurface& insertKnot(bool iByU, double iNewKnot, int iTimes = 1);

  /** \details
    Returns a knot value for the specified knot.

    \param iByU [in]  true means U-dimension.
    \param iKnotIndex [in]  Knot index.
  */
   double knotAt(bool iByU, int iKnotIndex) const;

  /** \details
    Returns the specified control point in the control point array.

    \param iIdxU [in]  Control point index for U-dimension.
    \param iIdxV [in]  Control point index for V-dimension.
  */
  OdGePoint3d controlPointAt(int iIdxU, int iIdxV) const;

  /** \details
    Sets the specified control point in the control point array.

    \param iIdxU [in]  Control point index for U-dimension in the array of control points.
    \param iIdxV [in]  Control point index for V-dimension in the array of control points.
    \param iPoint [in]  New point to be set at the specified index.

    \returns
    Reference to this OdGeNurbSurface object.
  */
  OdGeNurbSurface& setControlPointAt(int iIdxU, int iIdxV, const OdGePoint3d& iPoint);
};

#include "TD_PackPop.h"

#endif // OD_GENURBSF_H
