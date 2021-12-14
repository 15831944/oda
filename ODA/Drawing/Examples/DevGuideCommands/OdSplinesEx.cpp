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

#include "StdAfx.h"
#include "DbSpline.h"

/************************************************************************/
/* ODSPLINESEX command demonstates the OdDbSpline entity                */
/* functionality and pupulates database with Spline entities.           */
/************************************************************************/


void showStat(OdDbSpline* pSpline, OdDbUserIO* pIO)
{
  OdString message, about;
  OdResult eRes; 
  OdGePlane plane3d;
  double a, b, c, d, vArea, vStartParam, vEndParam, vTotalLength, controlPtTol, knotTol, fitTolerance;
  OdDb::Planarity planarity;
  OdGePoint3d start, end;
  OdGeVector3d vecFirst, vecSecond, pStartTan, pEndTan;
  int degree;
  bool rational, closed, periodic, tangentsExist;
  OdGePoint3dArray controlPoints, fitPoints;
  OdGeDoubleArray knots, weights;

  pIO->putString(L"\n/////////////START DISPLAYING SPLINE DATA/////////////");
  
  //Spline type
  switch(pSpline->type())
  {
    case OdDbSpline::kControlPoints: about = L"'by control points'"; break;
    case OdDbSpline::kFitPoints: about = L"'by fit points'"; break;
  }
  message.format(L"\nSpline type is %s", about.c_str());
  pIO->putString(message);

  // 'Closed' status
  message.format(L"\nSpline is %s", ((pSpline->isClosed()) ? L"closed" : L"opened"));
  pIO->putString(message);

  // 'Periodic' status
  message.format(L"\nSpline is %s", ((pSpline->isPeriodic()) ? L"periodic" : L"aperiodic"));
  pIO->putString(message);

  // Plane
  message.format(L"\nSpline is %s", ((pSpline->isPlanar()) ? L"planar" : L"non-planar"));
  pIO->putString(message);
  eRes = pSpline->getPlane(plane3d, planarity);
  if(eRes == eOk && planarity != OdDb::kNonPlanar) 
  {
    plane3d.getCoefficients(a, b, c, d);
    message.format(L"\nSpline plane is (%g * X + %g * Y + %g * Z + %g)", a, b, c, d);
    pIO->putString(message);
  }

  // Area
  eRes = pSpline->getArea(vArea);
  if(eRes == eOk)
  {
    message.format(L"\nSpline area = %g", vArea);
    pIO->putString(message);
  }

  // Start parameter
  eRes = pSpline->getStartParam(vStartParam);
  if(eRes == eOk)
  {
    message.format(L"\nStart parameter = %g", vStartParam);
    pIO->putString(message);
  }

  // End parameter
  eRes = pSpline->getEndParam(vEndParam);
  if(eRes == eOk)
  {
    message.format(L"\nEnd parameter = %g", vEndParam);
    pIO->putString(message);
  }

  // Start point
  eRes = pSpline->getStartPoint(start);
  if(eRes == eOk)
  {
    message.format(L"\nStart point = (%g,%g,%g)", start.x, start.y, start.z);
    pIO->putString(message);
  }

  // First and second derivatives of the first point
  eRes = pSpline->getFirstDeriv(vStartParam, vecFirst);
  if(eRes == eOk) 
  {
    message.format(L"\nFirst derivative vector of the first point is (%g,%g,%g)", vecFirst.x, vecFirst.y, vecFirst.z);
    pIO->putString(message);
  }
  eRes = pSpline->getSecondDeriv(vStartParam, vecSecond);
  if(eRes == eOk)
  {
    message.format(L"\nSecond derivative vector of the first point is (%g,%g,%g)", vecSecond.x, vecSecond.y, vecSecond.z);
    pIO->putString(message);
  }

  // End point
  eRes = pSpline->getEndPoint(end); 
  if(eRes == eOk)
  {
    message.format(L"\nEnd point = (%g,%g,%g)", end.x, end.y, end.z);
    pIO->putString(message);
  }
  
  // First and second derivatives of the last point
  eRes = pSpline->getFirstDeriv(vEndParam, vecFirst);
  if(eRes == eOk) 
  {
    message.format(L"\nFirst derivative vector of the last point is (%g,%g,%g)", vecFirst.x, vecFirst.y, vecFirst.z);
    pIO->putString(message);
  }
  eRes = pSpline->getSecondDeriv(vEndParam, vecSecond);
  if(eRes == eOk)
  {
    message.format(L"\nSecond derivative vector of the last point is (%g,%g,%g)", vecSecond.x, vecSecond.y, vecSecond.z);
    pIO->putString(message);
  }

  // Total lenght of the spline
  eRes= pSpline->getDistAtParam(vEndParam, vTotalLength);
  if(eRes == eOk)
  {
    message.format(L"\nSpline length = %g", vTotalLength);
    pIO->putString(message);
  }

  // Get NURBS data
  pSpline->getNurbsData(degree, rational, closed, periodic, controlPoints, knots, weights, controlPtTol, knotTol);

  // Degree
  message.format(L"\nDegree = %d", degree);
  pIO->putString(message);

  // 'Rational' status
  message.format(L"\nSpline is %s", ((rational) ? L"rational" : L"nonrational"));
  pIO->putString(message);

  // Control point tolerance
  message.format(L"\nControl point tolerance: %f", controlPtTol);
  pIO->putString(message);

  // Knot tolerance
  message.format(L"\nKnot point tolerance: %f", knotTol);
  pIO->putString(message);

  // Control points
  for (unsigned int i = 0; i < controlPoints.size(); i++)
  {
    message.format(L"\nControl point %d coordinates: %f %f %f", i, controlPoints[i].x, controlPoints[i].y, controlPoints[i].z);
    pIO->putString(message);
  }

  // Knots
  for (unsigned int i = 0; i < knots.size(); i++)
  {
    message.format(L"\nKnot %d is %f", i, knots[i]);
    pIO->putString(message);
  }

  // Get fit data
  pSpline->getFitData(fitPoints, degree, fitTolerance, tangentsExist, pStartTan, pEndTan);
  if (pSpline->hasFitData())
  {
    // Fit points
    for (unsigned int i = 0; i < fitPoints.size(); i++)
    {
      message.format(L"\nFit point %d coordinates: %f %f %f", i, fitPoints[i].x, fitPoints[i].y, fitPoints[i].z);
      pIO->putString(message);
    }
  
    // Fit tolerance
    message.format(L"\nSpline fit tolerance is %f", fitTolerance);
    pIO->putString(message);

    // Tangent vectors
    if (tangentsExist)
    {
      message.format(L"\nStart tangent vector: %f %f %f", pStartTan.x, pStartTan.y, pStartTan.z);
      pIO->putString(message);
      message.format(L"\nEnd tangent vector: %f %f %f", pEndTan.x, pEndTan.y, pEndTan.z);
      pIO->putString(message);
    }
  }

  pIO->putString(L"\n//////////////END DISPLAYING SPLINE DATA//////////////");
}


void _OdSplinesEx_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  /**********************************************************************/
  /* Create the pointer to current active space                         */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = DevGuideCommandsUtils::activeBlockTableRecord(pDb).safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create Splines                                                     */
  /**********************************************************************/
  /**********************************************************************/
  /* Create first Spline via the fit data                               */
  /**********************************************************************/
  // Create and initialize the spline
  OdDbSplinePtr pSpline = OdDbSpline::createObject();
  pSpline->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSpline);

  // Declare an array of fit points
  OdGePoint3d point = OdGePoint3d (0,0,0);
  double dx   = 2.0;
  double dy   = 2.0;

  OdGePoint3dArray fitPoints;
  fitPoints.push_back(point + OdGeVector3d(1.0 * dx, 1.0 * dy, 0.0));
  fitPoints.push_back(point + OdGeVector3d(3.0 * dx, 6.0 * dy, 0.0));
  fitPoints.push_back(point + OdGeVector3d(4.0 * dx, 2.0 * dy, 0.0));
  fitPoints.push_back(point + OdGeVector3d(7.0 * dx, 7.0 * dy, 0.0));
  
  // Declare degree
  int degree = 3;

  // Declare fit tolerance
  double fitTolerance = 0.0;

  // Declare start tangent
  OdGeVector3d pStartTan = OdGeVector3d(0.0, 0.0, 0.0);
  OdGeVector3d pEndTan = OdGeVector3d(1.0, 0.0, 0.0);
  
  // Set fit data 
  pSpline->setFitData(fitPoints, degree, fitTolerance, pStartTan, pEndTan);
  
  // Show spline data
  showStat(pSpline, pIO);


  /**********************************************************************/
  /* Create second Spline via the NURBS data                            */
  /**********************************************************************/
  // Create and initialize the spline
  OdDbSplinePtr pSpline2 = OdDbSpline::createObject();
  pSpline2->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSpline2);

  // Get NURBS data from the first spline to make tha same spline
  bool rational;
  bool closed;
  bool periodic;
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray knots;
  OdGeDoubleArray weights;
  double controlPtTol;
  double knotTol;
  pSpline->getNurbsData(degree, rational, closed, periodic, controlPoints, knots, weights, controlPtTol, knotTol);
  
  //Shift all points by 100 on X axis to make second spline visible under the first spline
  for (unsigned int i = 0; i <controlPoints.size(); i++)
      controlPoints[i].x +=20;
  
  // Make a periodic spline
  periodic = true;

  // Set NURBS data to the second spline
  pSpline2->setNurbsData(degree, rational, closed, periodic, controlPoints, knots, weights, controlPtTol, knotTol);
  
  // Show spline data
  showStat(pSpline2, pIO);
}
