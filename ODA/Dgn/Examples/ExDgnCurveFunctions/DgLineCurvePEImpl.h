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

#ifndef _ODDGLINECURVEFUNCTIONS_INCLUDED
#define _ODDGLINECURVEFUNCTIONS_INCLUDED

#include "DgCurveElement2d.h"
#include "DgCurveElement3d.h"

/************************************************************************/
/* This class is the Curve Protocol Extension class for OdDgLine2d      */
/* objects.                                                             */ 
/************************************************************************/
class OdDgLineElement2dCurvePE : public OdDgCurveElement2dPE
{
public:

  virtual OdResult getOffsetCurves(const OdDgCurveElement2d* pCurve, 
    double offsetDistance,  
    OdRxObjectPtrArray& offsetCurves) const;

  virtual OdResult getSplitCurves (const OdDgCurveElement2d* pCurve, 
    const OdGeDoubleArray& params, 
    OdRxObjectPtrArray& curveSegments) const;

  virtual OdResult getSplitCurves (const OdDgCurveElement2d* pCurve, 
    const OdGePoint2dArray& points, 
    OdRxObjectPtrArray& curveSegments) const;

  virtual OdResult extend(OdDgCurveElement2d* pCurve, 
    double param);

  virtual OdResult extend(OdDgCurveElement2d* pCurve, bool extendStart,
    const OdGePoint2d& toPoint);

  virtual OdResult getSpline (const OdDgCurveElement2d* pCurve, 
    OdDgBSplineCurve2dPtr& spline) const;

  virtual OdResult getClosestPointTo(const OdDgCurveElement2d* pCurve, 
    const OdGePoint2d& givenPoint,
    OdGePoint2d& pointOnCurve, 
    bool extend = false) const;
};

typedef OdSmartPtr<OdDgLineElement2dCurvePE> OdDgLineElement2dCurvePEPtr;

/************************************************************************/
/* This class is the Curve Protocol Extension class for OdDgLine3d      */
/* objects.                                                             */ 
/************************************************************************/
class OdDgLineElement3dCurvePE : public OdDgCurveElement3dPE
{
public:

  virtual OdResult getOrthoProjectedCurve(const OdDgCurveElement3d* pCurve, 
    const OdGePlane& projPlane, 
    OdDgCurveElement3dPtr* pProjCurve) const;

  virtual OdResult getProjectedCurve(const OdDgCurveElement3d* pCurve, 
    const OdGePlane& projPlane,
    const OdGeVector3d& projDirection, 
    OdDgCurveElement3dPtr* pProjCurve) const;

  virtual OdResult getOffsetCurves(const OdDgCurveElement3d* pCurve, 
    double offsetDistance,  
    OdRxObjectPtrArray& offsetCurves) const;

  virtual OdResult getOffsetCurvesGivenPlaneNormal(const OdDgCurveElement3d* pCurve, 
    const OdGeVector3d& normal, 
    double offsetDistance,
    OdRxObjectPtrArray& offsetCurves) const;          

  virtual OdResult getSplitCurves (const OdDgCurveElement3d* pCurve, 
    const OdGeDoubleArray& params, 
    OdRxObjectPtrArray& curveSegments) const;

  virtual OdResult getSplitCurves (const OdDgCurveElement3d* pCurve, 
    const OdGePoint3dArray& points, 
    OdRxObjectPtrArray& curveSegments) const;

  virtual OdResult extend(OdDgCurveElement3d* pCurve, 
    double param);

  virtual OdResult extend(OdDgCurveElement3d* pCurve, bool extendStart,
    const OdGePoint3d& toPoint);

  virtual OdResult getSpline (const OdDgCurveElement3d* pCurve, 
    OdDgBSplineCurve3dPtr& spline) const;

  virtual OdResult getClosestPointTo(const OdDgCurveElement3d* pCurve, 
    const OdGePoint3d& givenPoint,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const;

  virtual OdResult getClosestPointTo(const OdDgCurveElement3d* pCurve, 
    const OdGePoint3d& givenPoint,
    const OdGeVector3d& normal,
    OdGePoint3d& pointOnCurve, 
    bool extend = false) const;
};

typedef OdSmartPtr<OdDgLineElement3dCurvePE> OdDgLineElement3dCurvePEPtr;

#endif
