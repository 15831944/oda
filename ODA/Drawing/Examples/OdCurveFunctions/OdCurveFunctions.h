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

#include "DbCurvePE.h"

/************************************************************************/
/* Sample implementation of the OdDbCurvePE interface, which defines a  */
/* series of advanced curve operations.                                 */
/*                                                                      */
/* Drawings SDK attempts to use this interface for some curve           */
/* operations (OdDbCurve::getClosestPointTo, etc.).                     */
/*                                                                      */
/* This implementation supports only arcs and lines. Support for        */
/* additional entity types may be added by the client.                  */
/************************************************************************/

/************************************************************************/
/* This class is the Curve Protocol Extension class for OdDbLine        */
/* objects.                                                             */ 
/************************************************************************/
class OdDbCurvePE_Line : public OdDbCurvePE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbCurvePE_Line);
  
  virtual OdResult getOrthoProjectedCurve(const OdDbCurve* pCurve, 
    const OdGePlane& projPlane, 
    OdDbCurvePtr* pProjCurve) const;
  
  virtual OdResult getProjectedCurve(const OdDbCurve* pCurve, 
    const OdGePlane& projPlane,
    const OdGeVector3d& projDirection, 
    OdDbCurvePtr* pProjCurve) const;
  
  virtual OdResult getOffsetCurves(const OdDbCurve* pCurve, 
    double offsetDistance,  
    OdRxObjectPtrArray& offsetCurves) const;
  
  virtual OdResult getOffsetCurvesGivenPlaneNormal(const OdDbCurve* pCurve, 
    const OdGeVector3d& normal, 
    double offsetDistance,
    OdRxObjectPtrArray& offsetCurves) const;          
  
  virtual OdResult getSplitCurves (const OdDbCurve* pCurve, 
    const OdGeDoubleArray& params, 
    OdRxObjectPtrArray& curveSegments) const;
  
  virtual OdResult getSplitCurves (const OdDbCurve* pCurve, 
    const OdGePoint3dArray& points, 
    OdRxObjectPtrArray& curveSegments) const;
  
  virtual OdResult extend(OdDbCurve* pCurve, 
    double param);
  
  virtual OdResult extend(OdDbCurve* pCurve, bool extendStart,
    const OdGePoint3d& toPoint);
  
  virtual OdResult getSpline (const OdDbCurve* pCurve, 
    OdDbSplinePtr* spline) const;
  
  virtual OdResult getClosestPointTo(const OdDbCurve* pCurve, 
    const OdGePoint3d& givenPoint,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const;
  
  virtual OdResult getClosestPointTo(const OdDbCurve* pCurve, 
    const OdGePoint3d& givenPoint,
    const OdGeVector3d& direction,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const;
};

/************************************************************************/
/* This class is the Curve Protocol Extension class for OdDbArc         */
/* objects.                                                             */ 
/************************************************************************/
class OdDbCurvePE_Arc : public OdDbCurvePE
{
public:
  ODRX_DECLARE_MEMBERS(OdDbCurvePE_Arc);

  virtual OdResult getOrthoProjectedCurve(const OdDbCurve* pCurve, 
    const OdGePlane& projPlane, 
    OdDbCurvePtr* pProjCurve) const;

  virtual OdResult getProjectedCurve(const OdDbCurve* pCurve, 
    const OdGePlane& projPlane,
    const OdGeVector3d& projDirection, 
    OdDbCurvePtr* pProjCurve) const;

  virtual OdResult getOffsetCurves(const OdDbCurve* pCurve, 
    double offsetDistance,  
    OdRxObjectPtrArray& offsetCurves) const;

  virtual OdResult getOffsetCurvesGivenPlaneNormal(const OdDbCurve* pCurve, 
    const OdGeVector3d& normal, 
    double offsetDistance,
    OdRxObjectPtrArray& offsetCurves) const;          

  virtual OdResult getSplitCurves (const OdDbCurve* pCurve, 
    const OdGeDoubleArray& params, 
    OdRxObjectPtrArray& curveSegments) const;

  virtual OdResult getSplitCurves (const OdDbCurve* pCurve, 
    const OdGePoint3dArray& points, 
    OdRxObjectPtrArray& curveSegments) const;

  virtual OdResult extend(OdDbCurve* pCurve, 
    double param);

  virtual OdResult extend(OdDbCurve* pCurve, bool extendStart,
    const OdGePoint3d& toPoint);

  virtual OdResult getSpline (const OdDbCurve* pCurve, 
    OdDbSplinePtr* spline) const;

  virtual OdResult getClosestPointTo(const OdDbCurve* pCurve, 
    const OdGePoint3d& givenPoint,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const;

  virtual OdResult getClosestPointTo(const OdDbCurve* pCurve, 
    const OdGePoint3d& givenPoint,
    const OdGeVector3d& normal,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const;
};
