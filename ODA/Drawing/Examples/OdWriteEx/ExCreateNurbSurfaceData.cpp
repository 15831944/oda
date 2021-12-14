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
#include "ExCreateNurbSurfaceData.h"
#include "Ge/GeNurbCurve3d.h"

//test 0
int degreeInUt0 = 3;
int degreeInVt0 = 3;
int propsInUt0 = OdGe::kOpen | OdGe::kNoPoles;
int propsInVt0 = OdGe::kOpen | OdGe::kNoPoles;
int numControlPointsInUt0 = 6;
int numControlPointsInVt0 = 6;
OdGePoint3d arrCPNURBSt0[] = {
  OdGePoint3d(-764.20450059775521,205.23472413659516,98.221357298149513),
  OdGePoint3d(-764.20450059775544,194.54017732013909,98.221357298149556),
  OdGePoint3d(-764.20450059775521,173.15108368722673,98.221357298149499),
  OdGePoint3d(-764.20450059775533,141.06744323785836,98.221357298149542),
  OdGePoint3d(-764.20450059775499,119.67834960494608,98.221357298149485),
  OdGePoint3d(-764.20450059775521,108.98380278848995,98.221357298149513),
  OdGePoint3d(-776.44634479711408,205.23472413659510,98.221357298149499),
  OdGePoint3d(-776.44634479711431,194.54017732013912,98.221357298149542),
  OdGePoint3d(-776.44634479711408,173.15108368722665,98.221357298149499),
  OdGePoint3d(-776.44634479711431,141.06744323785844,98.221357298149513),
  OdGePoint3d(-776.44634479711397,119.67834960494601,98.221357298149499),
  OdGePoint3d(-776.44634479711408,108.98380278848991,98.221357298149499),
  OdGePoint3d(-800.93003319583340,205.23472413659513,98.221357298149499),
  OdGePoint3d(-800.93003319583363,194.54017732013907,98.221357298149542),
  OdGePoint3d(-800.93003319583352,173.15108368722670,98.221357298149499),
  OdGePoint3d(-800.93003319583340,141.06744323785836,98.221357298149513),
  OdGePoint3d(-800.93003319583340,119.67834960494606,98.221357298149499),
  OdGePoint3d(-800.93003319583340,108.98380278848997,98.221357298149499),
  OdGePoint3d(-837.65556579391148,205.23472413659525,98.221357298149556),
  OdGePoint3d(-837.65556579391171,194.54017732013921,98.221357298149599),
  OdGePoint3d(-837.65556579391159,173.15108368722673,98.221357298149542),
  OdGePoint3d(-837.65556579391159,141.06744323785844,98.221357298149570),
  OdGePoint3d(-837.65556579391136,119.67834960494605,98.221357298149542),
  OdGePoint3d(-837.65556579391148,108.98380278848994,98.221357298149556),
  OdGePoint3d(-862.13925419262989,205.23472413659513,98.221357298149499),
  OdGePoint3d(-862.13925419263012,194.54017732013907,98.221357298149542),
  OdGePoint3d(-862.13925419262989,173.15108368722670,98.221357298149499),
  OdGePoint3d(-862.13925419262989,141.06744323785838,98.221357298149513),
  OdGePoint3d(-862.13925419262966,119.67834960494601,98.221357298149499),
  OdGePoint3d(-862.13925419262989,108.98380278848992,98.221357298149499),
  OdGePoint3d(-874.38109839198921,205.23472413659516,98.221357298149513),
  OdGePoint3d(-874.38109839198944,194.54017732013907,98.221357298149556),
  OdGePoint3d(-874.38109839198898,173.15108368722670,98.221357298149499),
  OdGePoint3d(-874.38109839198933,141.06744323785836,98.221357298149542),
  OdGePoint3d(-874.38109839198933,119.67834960494606,98.221357298149485),
  OdGePoint3d(-874.38109839198921,108.98380278848992,98.221357298149513)
};
double arrUKnotst0[] =
{
  -55.088298897116999, -55.088298897116999, -55.088298897116999, -55.088298897116999,	-18.362766299038995, 18.362766299039009,	55.088298897117014,	55.088298897117014,	55.088298897117014,	55.088298897117014
};
double arrVKnotst0[] =
{
  -48.125460674052619, -48.125460674052619, -48.125460674052619, -48.125460674052619, -16.041820224684209, 16.041820224684201, 48.125460674052619, 48.125460674052619, 48.125460674052619, 48.125460674052619
};

OdGePoint3d arrTrimmingLoopCPSt0[] =
{
  OdGePoint3d(-874.38109839198921,205.23472413659516,98.221357298149513),
  OdGePoint3d(-874.38109839198944,194.54017732013907,98.221357298149556),
  OdGePoint3d(-874.38109839198898,173.1510836872267,98.221357298149499),
  OdGePoint3d(-874.38109839198933,141.06744323785836,98.221357298149542),
  OdGePoint3d(-874.38109839198933,119.67834960494606,98.221357298149485),
  OdGePoint3d(-874.38109839198921,108.98380278848992,98.221357298149513)
};
double arrTrimmingLoopKnotst0[] = { -48.125460674052619, -48.125460674052619, -48.125460674052619, -48.125460674052619, -16.041820224684209,
16.041820224684201, 48.125460674052619, 48.125460674052619, 48.125460674052619, 48.125460674052619 };
OdGePoint2d arrTrimmingLoopParCPSt0[] = { OdGePoint2d(55.088298897117006,-48.125460674052619), OdGePoint2d(55.088298897117014,48.125460674052619) };
double arrTrimmingLoopParKnotst0[] = { -48.125460674052619, -48.125460674052619, 48.125460674052619, 48.125460674052619 };

//edge2
OdGePoint3d arrTrimmingLoopCPSt01[] =
{
  OdGePoint3d(-874.38109839198921,108.98380278848994,98.221357298149513),
  OdGePoint3d(-837.65556579391125,108.98380278848994,98.221357298149471),
  OdGePoint3d(-800.93003319583181,108.98380278848983,98.221357298149471),
  OdGePoint3d(-764.20450059775521,108.98380278848995,98.221357298149513)
};
double arrTrimmingLoopKnotst01[] = { -55.08829889711699934, -55.08829889711699934,-55.08829889711699934,-55.08829889711699934,
55.08829889711699934, 55.08829889711699934, 55.08829889711699934, 55.08829889711699934 };
OdGePoint2d arrTrimmingLoopParCPSt01[] = { OdGePoint2d(55.08829889711700645, 48.12546067405261141), OdGePoint2d(-55.08829889711699934, 48.12546067405261141) };
double arrTrimmingLoopParKnotst01[] = { -55.08829889711699934, -55.08829889711699934, 55.08829889711699934, 55.08829889711699934 };

//edge3
OdGePoint3d arrTrimmingLoopCPSt02[] =
{
  OdGePoint3d(-764.20450059775521,108.98380278848995,98.221357298149513),
  OdGePoint3d(-764.20450059775453,141.0674432378583,98.221357298149428),
  OdGePoint3d(-764.2045005977559,173.15108368722696,98.221357298149641),
  OdGePoint3d(-764.20450059775521,205.23472413659516,98.221357298149513)

};
double arrTrimmingLoopKnotst02[] = { -48.125460674052619, -48.125460674052619, -48.125460674052619, -48.125460674052619,
48.125460674052619, 48.125460674052619, 48.125460674052619, 48.125460674052619 };
OdGePoint2d arrTrimmingLoopParCPSt02[] = { OdGePoint2d(-55.088298897116999, 48.125460674052611), OdGePoint2d(-55.088298897116999,-48.125460674052619) };
double arrTrimmingLoopParKnotst02[] = { -48.125460674052619, -48.125460674052619, 48.125460674052619, 48.125460674052619 };

//edge4
OdGePoint3d arrTrimmingLoopCPSt03[] =
{
  OdGePoint3d(-764.20450059775521,205.23472413659516,98.221357298149513),
  OdGePoint3d(-776.44634479711408,205.2347241365951,98.221357298149499),
  OdGePoint3d(-800.9300331958334,205.23472413659513,98.221357298149499),
  OdGePoint3d(-837.65556579391148,205.23472413659525,98.221357298149556),
  OdGePoint3d(-862.13925419262989,205.23472413659513,98.221357298149499),
  OdGePoint3d(-874.38109839198921,205.23472413659516,98.221357298149513)

};
double arrTrimmingLoopKnotst03[] = { -55.088298897116999, -55.088298897116999, -55.088298897116999, -55.088298897116999, -18.362766299038995,
18.362766299039009, 55.088298897117014, 55.088298897117014, 55.088298897117014, 55.088298897117014 };

OdGePoint2d arrTrimmingLoopParCPSt03[] = { OdGePoint2d(-55.088298897116999, -48.125460674052619), OdGePoint2d(55.088298897117014, -48.125460674052619) };
double arrTrimmingLoopParKnotst03[] = { -55.08829889711699934, -55.08829889711699934, 55.08829889711699934, 55.08829889711699934 };

//test 1
//hole-loop
//edge1
OdGePoint3d arrHoleLoopCPSh00[] =
{
  OdGePoint3d(-865.59771257789123,187.43338988491166,98.221357298149528),
  OdGePoint3d(-865.59771257789123,188.50284456655729,98.221357298149513),
  OdGePoint3d(-865.59771257789123,190.64175392984851,98.221357298149528),
  OdGePoint3d(-865.59771257789112,193.85011797478535,98.221357298149513),
  OdGePoint3d(-865.59771257789123,195.98902733807660,98.221357298149528),
  OdGePoint3d(-865.59771257789123,197.05848201972219,98.221357298149528)
};
double arrHoleLoopKnotsh00[] = { -48.125460674052619,-48.125460674052619,-48.125460674052619,-48.125460674052619,-16.041820224684201,
16.041820224684209,48.125460674052619,48.125460674052619,48.125460674052619,48.125460674052619 };

OdGePoint2d arrHoleLoopParCPSh00[] = { OdGePoint2d(46.304913083018910,-30.324126422369094), OdGePoint2d(46.304913083018889,-39.949218557179620) };
double arrHoleLoopParKnotsh00[] = { -48.125460674052619, -48.125460674052619, 48.125460674052619, 48.125460674052619 };

//edge2
OdGePoint3d arrHoleLoopCPSh01[] =
{
  OdGePoint3d(-865.59771257789123,197.05848201972219,98.221357298149528),
  OdGePoint3d(-864.37352815795521,197.05848201972219,98.221357298149513),
  OdGePoint3d(-861.92515931808339,197.05848201972219,98.221357298149528),
  OdGePoint3d(-858.25260605827555,197.05848201972219,98.221357298149513),
  OdGePoint3d(-855.80423721840361,197.05848201972219,98.221357298149513),
  OdGePoint3d(-854.58005279846782,197.05848201972219,98.221357298149528)
};
double arrHoleLoopKnotsh01[] = { -55.088298897117014,-55.088298897117014,-55.088298897117014,-55.088298897117014,-18.362766299039009,
18.362766299038995,55.088298897116999,55.088298897116999,55.088298897116999,55.088298897116999 };

OdGePoint2d arrTrimmingLoopParCPSh01[] = { OdGePoint2d(46.304913083018910,-39.949218557179698),OdGePoint2d(35.287253303595314,-39.949218557179577) };
double arrTrimmingLoopParKnotsh01[] = { -55.088298897116999,-55.088298897116999,55.088298897116999,55.088298897116999 };

//edge3
OdGePoint3d arrHoleLoopCPSh02[] =
{
  OdGePoint3d(-854.58005279846782,197.05848201972219,98.221357298149528),
  OdGePoint3d(-854.58005279846782,193.85011797478538,98.221357298149528),
  OdGePoint3d(-854.58005279846770,190.64175392984851,98.221357298149513),
  OdGePoint3d(-854.58005279846782,187.43338988491166,98.221357298149528)
};
double arrHoleLoopKnotsh02[] = { -48.125460674052619,-48.125460674052619,-48.125460674052619,-48.125460674052619,
48.125460674052619,48.125460674052619,48.125460674052619,48.125460674052619 };

OdGePoint2d arrHoleLoopParCPSh02[] = { OdGePoint2d(35.287253303595399,-39.949218557179591),OdGePoint2d(35.287253303595357,-30.324126422369066) };
double arrHoleLoopParKnotsh02[] = { -48.125460674052619,-48.125460674052619,48.125460674052619,48.125460674052619 };

//edge4
OdGePoint3d arrHoleLoopCPSh03[] =
{
  OdGePoint3d(-854.58005279846782,187.43338988491166,98.221357298149528),
  OdGePoint3d(-858.25260605827543,187.43338988491166,98.221357298149513),
  OdGePoint3d(-861.92515931808339,187.43338988491166,98.221357298149513),
  OdGePoint3d(-865.59771257789123,187.43338988491166,98.221357298149528)
};
double arrHoleLoopKnotsh03[] = { -55.088298897116999,-55.088298897116999,-55.088298897116999,-55.088298897116999,
55.088298897116999,55.088298897116999,55.088298897116999,55.088298897116999 };

OdGePoint2d arrHoleLoopParCPSh03[] = { OdGePoint2d(35.287253303595342,-30.324126422369062),OdGePoint2d(46.304913083018860,-30.3241264223691) };
double arrHoleLoopParKnotsh03[] = { -55.088298897116999,-55.088298897116999,55.088298897116999,55.088298897116999 };


void fillNurbData(OdGeNurbSurface &nurbSurf, OdArray<OdArray< OdGeNurbCurve3d*> > &arrLoops, OdArray<OdArray< OdGeNurbCurve2d*> > &arrLoopsPar)
{
  /********************************************************************/
  /* Create NURBS surface                                             */
  /********************************************************************/
  OdGePoint3dArray controlPoints;
  OdGeDoubleArray arrWeights;
  controlPoints.resize(sizeof(arrCPNURBSt0) / sizeof(OdGePoint3d));
  memcpy(controlPoints.asArrayPtr(), arrCPNURBSt0, sizeof(arrCPNURBSt0));
  OdGeKnotVector uKnots(sizeof(arrUKnotst0) / sizeof(double), arrUKnotst0);
  OdGeKnotVector vKnots(sizeof(arrVKnotst0) / sizeof(double), arrVKnotst0);
  nurbSurf.set(3, degreeInVt0, propsInUt0, propsInVt0, numControlPointsInUt0, numControlPointsInVt0, controlPoints, arrWeights, uKnots, vKnots);
  /********************************************************************/
  /* Create trimming-loop, edge0                                      */
  /********************************************************************/
  controlPoints.clear();
  controlPoints.resize(sizeof(arrTrimmingLoopCPSt0) / sizeof(OdGePoint3d));
  memcpy(controlPoints.asArrayPtr(), arrTrimmingLoopCPSt0, sizeof(arrTrimmingLoopCPSt0));
  OdGeKnotVector vecTrimmingLoopKnots00(sizeof(arrTrimmingLoopKnotst0) / sizeof(double), arrTrimmingLoopKnotst0);
  OdGeNurbCurve3d* pTrimmigLoopCurve00 = new OdGeNurbCurve3d(3, vecTrimmingLoopKnots00, controlPoints, arrWeights, false);
  /********************************************************************/
  /* Trimming-loop, edge1                                             */
  /********************************************************************/
  controlPoints.clear();
  controlPoints.resize(sizeof(arrTrimmingLoopCPSt01) / sizeof(OdGePoint3d));
  memcpy(controlPoints.asArrayPtr(), arrTrimmingLoopCPSt01, sizeof(arrTrimmingLoopCPSt01));
  OdGeKnotVector vecTrimmingLoopKnots01(sizeof(arrTrimmingLoopKnotst01) / sizeof(double), arrTrimmingLoopKnotst01);
  OdGeNurbCurve3d* pTrimmigLoopCurve01 = new OdGeNurbCurve3d(3, vecTrimmingLoopKnots01, controlPoints, arrWeights, false);
  /********************************************************************/
  /* Trimming-loop, edge2                                             */
  /********************************************************************/
  controlPoints.clear();
  controlPoints.resize(sizeof(arrTrimmingLoopCPSt02) / sizeof(OdGePoint3d));
  memcpy(controlPoints.asArrayPtr(), arrTrimmingLoopCPSt02, sizeof(arrTrimmingLoopCPSt02));
  OdGeKnotVector vecTrimmingLoopKnots02(sizeof(arrTrimmingLoopKnotst02) / sizeof(double), arrTrimmingLoopKnotst02);
  OdGeNurbCurve3d* pTrimmigLoopCurve02 = new OdGeNurbCurve3d(3, vecTrimmingLoopKnots02, controlPoints, arrWeights, false);
  /********************************************************************/
  /* Trimming-loop, edge3                                             */
  /********************************************************************/
  controlPoints.clear();
  controlPoints.resize(sizeof(arrTrimmingLoopCPSt03) / sizeof(OdGePoint3d));
  memcpy(controlPoints.asArrayPtr(), arrTrimmingLoopCPSt03, sizeof(arrTrimmingLoopCPSt03));
  OdGeKnotVector vecTrimmingLoopKnots03(sizeof(arrTrimmingLoopKnotst03) / sizeof(double), arrTrimmingLoopKnotst03);
  OdGeNurbCurve3d* pTrimmigLoopCurve03 = new OdGeNurbCurve3d(3, vecTrimmingLoopKnots03, controlPoints, arrWeights, false);

  /********************************************************************/
  /* Create second loop curve(hole), edge0                            */
  /********************************************************************/
  controlPoints.clear();
  controlPoints.resize(sizeof(arrHoleLoopCPSh00) / sizeof(OdGePoint3d));
  memcpy(controlPoints.asArrayPtr(), arrHoleLoopCPSh00, sizeof(arrHoleLoopCPSh00));
  OdGeKnotVector vecHoleLoopKnots00(sizeof(arrHoleLoopKnotsh00) / sizeof(double), arrHoleLoopKnotsh00);
  OdGeNurbCurve3d* pHoleLoopCurve00 = new OdGeNurbCurve3d(3, vecHoleLoopKnots00, controlPoints, arrWeights, false);
  /********************************************************************/
  /* Hole, edge1                                                      */
  /********************************************************************/
  controlPoints.clear();
  controlPoints.resize(sizeof(arrHoleLoopCPSh01) / sizeof(OdGePoint3d));
  memcpy(controlPoints.asArrayPtr(), arrHoleLoopCPSh01, sizeof(arrHoleLoopCPSh01));
  OdGeKnotVector vecHoleLoopKnots01(sizeof(arrHoleLoopKnotsh01) / sizeof(double), arrHoleLoopKnotsh01);
  OdGeNurbCurve3d* pHoleLoopCurve01 = new OdGeNurbCurve3d(3, vecHoleLoopKnots01, controlPoints, arrWeights, false);
  /********************************************************************/
  /* Hole, edge2                                                      */
  /********************************************************************/
  controlPoints.clear();
  controlPoints.resize(sizeof(arrHoleLoopCPSh02) / sizeof(OdGePoint3d));
  memcpy(controlPoints.asArrayPtr(), arrHoleLoopCPSh02, sizeof(arrHoleLoopCPSh02));
  OdGeKnotVector vecHoleLoopKnots02(sizeof(arrHoleLoopKnotsh02) / sizeof(double), arrHoleLoopKnotsh02);
  OdGeNurbCurve3d* pHoleLoopCurve02 = new OdGeNurbCurve3d(3, vecHoleLoopKnots02, controlPoints, arrWeights, false);
  /********************************************************************/
  /* Hole, edge3                                                      */
  /********************************************************************/
  controlPoints.clear();
  controlPoints.resize(sizeof(arrHoleLoopCPSh03) / sizeof(OdGePoint3d));
  memcpy(controlPoints.asArrayPtr(), arrHoleLoopCPSh03, sizeof(arrHoleLoopCPSh03));
  OdGeKnotVector vecHoleLoopKnots03(sizeof(arrHoleLoopKnotsh03) / sizeof(double), arrHoleLoopKnotsh03);
  OdGeNurbCurve3d* pHoleLoopCurve03 = new OdGeNurbCurve3d(3, vecHoleLoopKnots03, controlPoints, arrWeights, false);
 
  /********************************************************************/
  /* Create arrays - first is a trimming-loop, next - hole           */
  /********************************************************************/
  OdArray< OdGeNurbCurve3d*> arrCurvesTrimmingLoop, arrCurvesHole;
  arrCurvesTrimmingLoop.append(pTrimmigLoopCurve00);
  arrCurvesTrimmingLoop.append(pTrimmigLoopCurve01);
  arrCurvesTrimmingLoop.append(pTrimmigLoopCurve02);
  arrCurvesTrimmingLoop.append(pTrimmigLoopCurve03);
  arrLoops.append(arrCurvesTrimmingLoop);

  arrCurvesHole.append(pHoleLoopCurve00);
  arrCurvesHole.append(pHoleLoopCurve01);
  arrCurvesHole.append(pHoleLoopCurve02);
  arrCurvesHole.append(pHoleLoopCurve03);
  arrLoops.append(arrCurvesHole);


  /********************************************************************/
  /* Create parametric curves. For geometry of edge0                  */
  /********************************************************************/
  OdGePoint2dArray controlPoints2d;
  controlPoints2d.resize(sizeof(arrTrimmingLoopParCPSt0) / sizeof(OdGePoint2d));
  memcpy(controlPoints2d.asArrayPtr(), arrTrimmingLoopParCPSt0, sizeof(arrTrimmingLoopParCPSt0));
  OdGeKnotVector vecTrimmingLoopParKnots(sizeof(arrTrimmingLoopParKnotst0) / sizeof(double), arrTrimmingLoopParKnotst0);
  OdGeNurbCurve2d* pParTrimmigLoopCurve00 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots, controlPoints2d, arrWeights, false);
  /********************************************************************/
  /* For geometry of edge1                                            */
  /********************************************************************/
  controlPoints2d.resize(sizeof(arrTrimmingLoopParCPSt01) / sizeof(OdGePoint2d));
  memcpy(controlPoints2d.asArrayPtr(), arrTrimmingLoopParCPSt01, sizeof(arrTrimmingLoopParCPSt01));
  OdGeKnotVector vecTrimmingLoopParKnots01(sizeof(arrTrimmingLoopParKnotst01) / sizeof(double), arrTrimmingLoopParKnotst01);
  OdGeNurbCurve2d* pParTrimmigLoopCurve01 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots01, controlPoints2d, arrWeights, false);
  /********************************************************************/
  /* For geometry of edge2                                            */
  /********************************************************************/
  controlPoints2d.resize(sizeof(arrTrimmingLoopParCPSt02) / sizeof(OdGePoint2d));
  memcpy(controlPoints2d.asArrayPtr(), arrTrimmingLoopParCPSt02, sizeof(arrTrimmingLoopParCPSt02));
  OdGeKnotVector vecTrimmingLoopParKnots02(sizeof(arrTrimmingLoopParKnotst02) / sizeof(double), arrTrimmingLoopParKnotst02);
  OdGeNurbCurve2d* pParTrimmigLoopCurve02 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots02, controlPoints2d, arrWeights, false);
  /********************************************************************/
  /* For geometry of edge3                                            */
  /********************************************************************/
  controlPoints2d.resize(sizeof(arrTrimmingLoopParCPSt03) / sizeof(OdGePoint2d));
  memcpy(controlPoints2d.asArrayPtr(), arrTrimmingLoopParCPSt03, sizeof(arrTrimmingLoopParCPSt03));
  OdGeKnotVector vecTrimmingLoopParKnots03(sizeof(arrTrimmingLoopParKnotst03) / sizeof(double), arrTrimmingLoopParKnotst03);
  OdGeNurbCurve2d* pParTrimmigLoopCurve03 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots03, controlPoints2d, arrWeights, false);

  /********************************************************************/
  /*  Create parametric curves for hole. For edge0                    */
  /********************************************************************/
  controlPoints2d.resize(sizeof(arrHoleLoopParCPSh00) / sizeof(OdGePoint2d));
  memcpy(controlPoints2d.asArrayPtr(), arrHoleLoopParCPSh00, sizeof(arrHoleLoopParCPSh00));
  OdGeKnotVector vecHoleLoopParKnots(sizeof(arrHoleLoopParKnotsh00) / sizeof(double), arrHoleLoopParKnotsh00);
  OdGeNurbCurve2d* pParHoleLoopCurve00 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots, controlPoints2d, arrWeights, false);
  /********************************************************************/
  /*  Create parametric curves for hole. For edge1                    */
  /********************************************************************/
  controlPoints2d.resize(sizeof(arrTrimmingLoopParCPSh01) / sizeof(OdGePoint2d));
  memcpy(controlPoints2d.asArrayPtr(), arrTrimmingLoopParCPSh01, sizeof(arrTrimmingLoopParCPSh01));
  OdGeKnotVector vecHoleLoopParKnots01(sizeof(arrTrimmingLoopParKnotsh01) / sizeof(double), arrTrimmingLoopParKnotsh01);
  OdGeNurbCurve2d* pParHoleLoopCurve01 = new OdGeNurbCurve2d(1, vecHoleLoopParKnots01, controlPoints2d, arrWeights, false);  
  /********************************************************************/
  /*  Create parametric curves for hole. For edge2                    */
  /********************************************************************/
  controlPoints2d.resize(sizeof(arrHoleLoopParCPSh02) / sizeof(OdGePoint2d));
  memcpy(controlPoints2d.asArrayPtr(), arrHoleLoopParCPSh02, sizeof(arrHoleLoopParCPSh02));
  OdGeKnotVector vecHoleLoopParKnots02(sizeof(arrHoleLoopParKnotsh02) / sizeof(double), arrHoleLoopParKnotsh02);
  OdGeNurbCurve2d* pParHoleLoopCurve02 = new OdGeNurbCurve2d(1, vecHoleLoopParKnots02, controlPoints2d, arrWeights, false);
  /********************************************************************/
  /*  Create parametric curves for hole. For edge3                    */
  /********************************************************************/
  controlPoints2d.resize(sizeof(arrHoleLoopParCPSh03) / sizeof(OdGePoint2d));
  memcpy(controlPoints2d.asArrayPtr(), arrHoleLoopParCPSh03, sizeof(arrHoleLoopParCPSh03));
  OdGeKnotVector vecHoleLoopParKnots03(sizeof(arrHoleLoopParKnotsh03) / sizeof(double), arrHoleLoopParKnotsh03);
  OdGeNurbCurve2d* pParHoleLoopCurve03 = new OdGeNurbCurve2d(1, vecHoleLoopParKnots03, controlPoints2d, arrWeights, false);

  /********************************************************************/
  /* Create parametric curves arrays                                  */
  /********************************************************************/
  OdArray< OdGeNurbCurve2d*> arrTrimmingLoopPar, arrCurvesParHole;
  arrTrimmingLoopPar.append(pParTrimmigLoopCurve00);
  arrTrimmingLoopPar.append(pParTrimmigLoopCurve01);
  arrTrimmingLoopPar.append(pParTrimmigLoopCurve02);
  arrTrimmingLoopPar.append(pParTrimmigLoopCurve03);

  arrCurvesParHole.append(pParHoleLoopCurve00);
  arrCurvesParHole.append(pParHoleLoopCurve01);
  arrCurvesParHole.append(pParHoleLoopCurve02);
  arrCurvesParHole.append(pParHoleLoopCurve03);

  arrLoopsPar.append(arrTrimmingLoopPar);
  arrLoopsPar.append(arrCurvesParHole);
}
