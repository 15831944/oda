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

#ifndef OD_GESURF_H
#define OD_GESURF_H /*!DOM*/

#include "Ge/GeEntity3d.h"
#include "Ge/GeVector3dArray.h"
#include "Ge/GePoint2d.h"
#include "Ge/GeUvBox.h"
#include "Ge/GeDoubleArray.h"

class OdGePoint2d;
class OdGeCurve3d;
class OdGePointOnCurve3d;
class OdGePointOnSurface;
class OdGePointOnSurfaceData;
class OdGeInterval;
class OdGeExtents3d;

#include "TD_PackPush.h"

/** \details
    This class is the base class for all OdGe parametric surfaces.

    Corresponding C++ library: TD_Ge

    <group OdGe_Classes> 
*/
class GE_TOOLKIT_EXPORT OdGeSurface : public OdGeEntity3d
{
public:
  /** \details
    Returns the 2D pair of parameter values of a point on this surface.

    \param point [in]  Point to be evaluated.
    \param tol [in]  Geometric tolerance.

    \remarks
    The returned parameters specify a point within tol of point.
    If point is not on this surface, the results are unpredictable.
    If you are not sure the point is on this surface, use 
    isOn() instead of this function.
  */
   OdGePoint2d paramOf(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the 2D pair of parameter values of a point on this surface.

    \param point [in]  Point to be evaluated.
    \param uvBox [in]  uv-box of closest point.
    \param tol [in]  Geometric tolerance.

    \remarks
    The returned parameters specify a point within tol of point.
    If point is not on this surface, the results are unpredictable.
    If you are not sure the point is on this surface, use
    isOn() instead of this function.

    \remarks
    If the 2D projection of the point didn't lie in the u or v interval of uvBox-bounds, the closest bound of the interval is set as a corresponding u/v parameter of resulting point.
    In case of uvBox == NULL paramOf(point, box, tol) is called in which box is a native envelope of the surface.
    
    \remarks
    By default this function throws the exception "not Implemented". Should be implemented in derived classes.
  */
   OdGePoint2d paramOf(
    const OdGePoint3d& point, 
    const OdGeUvBox* uvBox, 
    const OdGeTol& tol = OdGeContext::gTol) const;

   //DOM-IGNORE-BEGIN
   // Tests if point is on *surface*.
   TD_USING(OdGeEntity3d::isOn);
   //DOM-IGNORE-END

  /** \details
    Determines if the passed point is on this surface and retrieves a 2D point that represents
    parametric values for the passed 3D point if it is on this surface.
    
    \param point [in]  Point that is checked to be on this surface.
    \param paramPoint [out]  Receives the 2D pair of parameter values at the point if the input point lies on the surface.
    \param tol [in]  Geometric tolerance.
  */
   bool isOn( 
    const OdGePoint3d& point, 
    OdGePoint2d& paramPoint,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this surface closest to the specified point.

    \param point [in]  Input 3D point to which the closest point on this surface is calculated.
    \param tol [in]  Geometric tolerance.
  */
   OdGePoint3d closestPointTo(
    const OdGePoint3d& point,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this surface closest to the specified point.

    \param point [in]  Any 3D point.
    \param param [out]  uv-coord of closest point.
    \param uvBox [in]  uv-box of closest point.
    \param tol [in]  Geometric tolerance.

    \remarks
    Native envelope is not considered in calculation process. The nearest point is searched on the surface limited by uvBox-envelope.
    In case of uvBox == NULL && param == NULL closestPointTo(point, tol) is called.
    If uvBox == NULL, the nearest point is searched for surface bounded by its envelope.
    If param == NULL, param is not calculated.
    Don't use for unbounded uvBox
  */
   OdGePoint3d closestPointTo(
    const OdGePoint3d & point, 
    OdGePoint2d * param, 
    const OdGeUvBox* uvBox, 
    const OdGeTol & tol = OdGeContext::gTol) const;

  /** \details
    Returns the point on this surface closest to the specified point.

    \param point [in]  Input 3D point to which the closest point on this surface is calculated.
    \param pntOnSurface [out]  Receives the closest point on the surface. 
    \param tol [in]  Geometric tolerance.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   void getClosestPointTo(
    const OdGePoint3d& point, 
    OdGePointOnSurface& pntOnSurface,
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the distance to the point on this curve closest to the specified point.

    \param point [in]  Any 3D point distance to which is measured.
    \param tol [in]  Geometric tolerance.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   double distanceTo(
    const OdGePoint3d& point, 
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details 
     Checks normal direction.

     \returns 
     true if and only if the normal of this surface has been reversed an odd number of times, false - otherwise.
   */
   bool isNormalReversed() const;

  /** \details 
     Checks orientation of the surface.

     \returns true if "natural" and "nominal" normals are opposite (surface is "left-handed"), 
       false - otherwise (surface is "right-handed").

     \remarks
     Let U and V be surface's partial derivatives by U and V respectively (at some point).
     Consider two normals:
      * "natural" normal is computed as [U x V]  (i.e. cross product of partial derivatives).
      * "nominal" normal is the vector returned by evalPoint method.
   */
   bool isLeftHanded() const;


  /** \details 
     Reverses the normal of this surface.
   
     \returns 
     Reference to this surface.
   */
   OdGeSurface& reverseNormal();

  /** \details
    Returns the minimum rectangle in parameter space that contains the parameter
    domain of this surface.

    \param intrvlU [out]  Receives the u interval.
    \param intrvlV [out]  Receives the v interval.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   void getEnvelope(
    OdGeInterval& intrvlU, 
    OdGeInterval& intrvlV) const;

  /** \details
    Returns the minimum rectangle in parameter space that contains the parameter
    domain of this surface.

    \param uvbox [out]  Receives the uv-box

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   void getEnvelope(
    OdGeUvBox& uvbox) const;

  /** \details
    Returns true if and only if this surface is closed in the U direction.

    \param tol [in]  Geometric tolerance.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
     bool isClosedInU(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns true if and only if this surface is closed in the V direction.

    \param tol [in]  Geometric tolerance.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
     bool isClosedInV(
    const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Returns the point corresponding to the parameter pair, as well as the
    derivatives and the normal at that point.

    \param param [in]  The parameter pair to be evaluated.

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
   OdGePoint3d evalPoint(
    const OdGePoint2d& param) const;
  
  /** \details
    Returns the point corresponding to the parameter pair, as well as the
    derivatives and the normal at that point.

    \param param [in]  The parameter pair to be evaluated.
    \param numDeriv [in]  Maximum order of derivatives to be computed.
    \param derivatives [out]  Receives an array of derivatives at the point corresponding to param.
    
    \remarks
    If numDeriv is 1, then derivatives are ordered: du, dv
    If numDeriv is 2, then derivatives are ordered: du, dudu, dv, dvdv, dudv
    numDeriv >= 3 is not supported

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGePoint3d evalPoint(
    const OdGePoint2d& param, 
    int numDeriv,
    OdGeVector3dArray& derivatives) const;
  
  /** \details
    Returns the point corresponding to the parameter pair, as well as the
    derivatives and the normal at that point.

    \param param [in]  The parameter pair to be evaluated.
    \param numDeriv [in]  Maximum order of derivatives to be computed.
    \param derivatives [out]  Receives an array of derivatives at the point corresponding to param.
    \param normal [out]  Receives the normal at the point corresponding to param.
    
    \remarks
    If numDeriv is 1, then derivatives are ordered: du, dv
    If numDeriv is 2, then derivatives are ordered: du, dudu, dv, dvdv, dudv
    numDeriv >= 3 is not supported

    \remarks
    By default this function throws exception "not Implemented". Should be implemented in derived classes.
  */
  OdGePoint3d evalPoint(
    const OdGePoint2d& param,
    int numDeriv,
    OdGeVector3dArray& derivatives, 
    OdGeVector3d& normal) const;

  /** \details
    Assignment operator for the OdGeSurface class.

    \param surf [in]  Surface to assign.

    \returns
    A reference to this OdGeSurface object.
  */
  OdGeSurface& operator=(
    const OdGeSurface& surf);

  /** \details
    Returns bounding box of the whole surface or a part of it.
    The box is axis-aligned in the specified local coordinate system.
    It is guaranteed to enclose the surface, but not guaranteed to be tight/minimal.

    \param range [in]  The part of the surface to be bounded.
    \param coordSystem [in]  The local coordinate system in which box extents are computed.

    \remarks
    If range is unbounded (e.g. by default), then bounding box of the whole surface (which range can be
    obtained by getEnvelope method) is computed. Otherwise bounding box is computed only for the set of
    points with parameter in the specified range.
    If coordSystem is NULL (e.g. by default), then bounding box is aligned with global coordinate axes.
    Otherwise, the box will be axis-aligned in the local coordinate system specified by coordSystem matrix.
  */
  OdGeExtents3d getGeomExtents(
    const OdGeUvBox& range = OdGeUvBox(),
    const OdGeMatrix3d* coordSystem = NULL) const;

  //////////////////////////////////////////////////////////////////////////
  // TD Special :

  /** \details 
     Projects a point onto this surface.
   
     \param p      [in]  Any 3D point.
     \param projP [out]  Receives the point on this surface.
     \param tol    [in]  Geometric tolerance.

     \returns 
     true if and only if there is a point on this surface, projP, where a normal to this surface passes through the point p.
   */
   bool project(const OdGePoint3d& p, OdGePoint3d& projP, const OdGeTol& tol = OdGeContext::gTol) const;

  /** \details
    Set the rectangle in parameter space that defines the parameter
    domain of this surface.
    \returns
    true if and only if the new parameter domain was successfully set
    for this surface.

    \param realIntrvlU [in] A reference to a new real u interval to be set.
    \param realIntrvlV [in] A reference to a new real v interval to be set.
  */
  bool setEnvelope(
    const OdGeInterval& realIntrvlU,
    const OdGeInterval& realIntrvlV);


  /** \details
    Returns isoparametric curve for surface.

    \param byU [in]  true means: U = const, V - varying.
    \param param [in]  Value of the const parameter.

    \returns
    Isoparametric curve as a pointer to OdGeCurve3d object.

    \remarks
    Caller is responsible to delete the returned curve.
    Surface envelope is used to define curve interval.
  */
  OdGeCurve3d* makeIsoparamCurve(
    bool byU, 
    double param) const;

  /** \details
    Returns isoparametric curve for surface.

    \param byU [in]  true means: U = const, V - varying.
    \param param [in]  Value of the const parameter.
    \param interval [in]  Interval of the desired curve (by varying parameter).

    \returns
    Isoparametric curve as a pointer to OdGeCurve3d object.

    \remarks
    Caller is responsible to delete the returned curve.
  */
  OdGeCurve3d* makeIsoparamCurve(
    bool byU, 
    double param, 
    const OdGeInterval& interval) const;

  /** \details
    Finds all poles of surface. Surface envelope not considered.
    In case of NurbSurface extreme points of envelope are checked. 
    Input arrays may be not empty. In this case new poles will be written to the ends of the arrays.

    \param uParams [out]  (Optional) Pointer to an array that receives U-params for poles with U = const.
    \param vParams [out]  (Optional) Pointer to an array that receives V-params for poles with V = const.
    \param uPoints [out]  (Optional) Pointer to an array that receives 3D points for poles with U = const.
    \param vPoints [out]  (Optional) Pointer to an array that receives 3D points for poles with V = const.
    \param tol [in]  Geometric tolerance.
    
    \returns
    true if the surface has poles, false otherwise.
  */
  bool getPoles(
    OdGeDoubleArray *uParams, 
    OdGeDoubleArray *vParams, 
    OdGePoint3dArray *uPoints, 
    OdGePoint3dArray *vPoints,
    double tol = 1e-13) const;
  
protected:
  OdGeSurface();
  OdGeSurface(
    const OdGeSurface& surf);

};

#include "TD_PackPop.h"

#endif // OD_GESURF_H
