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

using System;
using System.Collections.Generic;

using System.Text;
using Teigha.Core;

namespace ExBrepBuilderSwigMgd
{
  public class NurbData
  {
    public void FillNurbData(OdGeNurbSurface nurbSurf, ref OdGeNurbCurve3dArray2d arrLoops, ref OdGeNurbCurve2dArray2d arrLoopsPar)
    {
      /********************************************************************/
      /* Create NURBS surface                                             */
      /********************************************************************/
      OdGePoint3dArray controlPoints = null;
      OdGeDoubleArray arrWeights = new OdGeDoubleArray();
      controlPoints = new OdGePoint3dArray(arrCPNURBSt0);
      OdGeKnotVector uKnots = new OdGeKnotVector(arrUKnotst0);
      OdGeKnotVector vKnots = new OdGeKnotVector(arrVKnotst0);
      nurbSurf.set(3, degreeInVt0, propsInUt0, propsInVt0, numControlPointsInUt0, numControlPointsInVt0, controlPoints, arrWeights, uKnots, vKnots);
      /********************************************************************/
      /* Create trimming-loop, edge0                                      */
      /********************************************************************/
      controlPoints = new OdGePoint3dArray(arrTrimmingLoopCPSt0);
      OdGeKnotVector vecTrimmingLoopKnots00 = new OdGeKnotVector(arrTrimmingLoopKnotst0);
      OdGeNurbCurve3d pTrimmigLoopCurve00 = new OdGeNurbCurve3d(3, vecTrimmingLoopKnots00, controlPoints, arrWeights, false);
      /********************************************************************/
      /* Trimming-loop, edge1                                             */
      /********************************************************************/
      controlPoints = new OdGePoint3dArray(arrTrimmingLoopCPSt01);
      OdGeKnotVector vecTrimmingLoopKnots01 = new OdGeKnotVector(arrTrimmingLoopKnotst01);
      OdGeNurbCurve3d pTrimmigLoopCurve01 = new OdGeNurbCurve3d(3, vecTrimmingLoopKnots01, controlPoints, arrWeights, false);
      /********************************************************************/
      /* Trimming-loop, edge2                                             */
      /********************************************************************/
      controlPoints = new OdGePoint3dArray(arrTrimmingLoopCPSt02);
      OdGeKnotVector vecTrimmingLoopKnots02 = new OdGeKnotVector(arrTrimmingLoopKnotst02);
      OdGeNurbCurve3d pTrimmigLoopCurve02 = new OdGeNurbCurve3d(3, vecTrimmingLoopKnots02, controlPoints, arrWeights, false);
      /********************************************************************/
      /* Trimming-loop, edge3                                             */
      /********************************************************************/
      controlPoints = new OdGePoint3dArray(arrTrimmingLoopCPSt03);
      OdGeKnotVector vecTrimmingLoopKnots03 = new OdGeKnotVector(arrTrimmingLoopKnotst03);
      OdGeNurbCurve3d pTrimmigLoopCurve03 = new OdGeNurbCurve3d(3, vecTrimmingLoopKnots03, controlPoints, arrWeights, false);

      /********************************************************************/
      /* Create second loop curve(hole), edge0                            */
      /********************************************************************/
      controlPoints = new OdGePoint3dArray(arrHoleLoopCPSh00);
      OdGeKnotVector vecHoleLoopKnots00 = new OdGeKnotVector(arrHoleLoopKnotsh00);
      OdGeNurbCurve3d pHoleLoopCurve00 = new OdGeNurbCurve3d(3, vecHoleLoopKnots00, controlPoints, arrWeights, false);
      /********************************************************************/
      /* Hole, edge1                                                      */
      /********************************************************************/
      controlPoints = new OdGePoint3dArray(arrHoleLoopCPSh01);
      OdGeKnotVector vecHoleLoopKnots01 = new OdGeKnotVector(arrHoleLoopKnotsh01);
      OdGeNurbCurve3d pHoleLoopCurve01 = new OdGeNurbCurve3d(3, vecHoleLoopKnots01, controlPoints, arrWeights, false);
      /********************************************************************/
      /* Hole, edge2                                                      */
      /********************************************************************/
      controlPoints = new OdGePoint3dArray(arrHoleLoopCPSh02);
      OdGeKnotVector vecHoleLoopKnots02 = new OdGeKnotVector(arrHoleLoopKnotsh02);
      OdGeNurbCurve3d pHoleLoopCurve02 = new OdGeNurbCurve3d(3, vecHoleLoopKnots02, controlPoints, arrWeights, false);
      /********************************************************************/
      /* Hole, edge3                                                      */
      /********************************************************************/
      controlPoints = new OdGePoint3dArray(arrHoleLoopCPSh03);
      OdGeKnotVector vecHoleLoopKnots03 = new OdGeKnotVector(arrHoleLoopKnotsh03);
      OdGeNurbCurve3d pHoleLoopCurve03 = new OdGeNurbCurve3d(3, vecHoleLoopKnots03, controlPoints, arrWeights, false);
      /********************************************************************/
      /* Create arrays - first is a trimming-loop, next - hole           */
      /********************************************************************/

      OdGeNurbCurve3dArrayPtr arrCurvesTrimmingLoop = new OdGeNurbCurve3dArrayPtr(), arrCurvesHole = new OdGeNurbCurve3dArrayPtr();
      arrCurvesTrimmingLoop.Add(pTrimmigLoopCurve00);
      arrCurvesTrimmingLoop.Add(pTrimmigLoopCurve01);
      arrCurvesTrimmingLoop.Add(pTrimmigLoopCurve02);
      arrCurvesTrimmingLoop.Add(pTrimmigLoopCurve03);
      GC.SuppressFinalize(pTrimmigLoopCurve00);
      GC.SuppressFinalize(pTrimmigLoopCurve01);
      GC.SuppressFinalize(pTrimmigLoopCurve02);
      GC.SuppressFinalize(pTrimmigLoopCurve03);
      arrLoops.Add(arrCurvesTrimmingLoop);
      GC.SuppressFinalize(arrCurvesTrimmingLoop);

      arrCurvesHole.Add(pHoleLoopCurve00);
      arrCurvesHole.Add(pHoleLoopCurve01);
      arrCurvesHole.Add(pHoleLoopCurve02);
      arrCurvesHole.Add(pHoleLoopCurve03);
      GC.SuppressFinalize(pHoleLoopCurve00);
      GC.SuppressFinalize(pHoleLoopCurve01);
      GC.SuppressFinalize(pHoleLoopCurve02);
      GC.SuppressFinalize(pHoleLoopCurve03);
      arrLoops.Add(arrCurvesHole);
      GC.SuppressFinalize(arrCurvesHole);


      /********************************************************************/
      /* Create parametric curves. For geometry of edge0                  */
      /********************************************************************/
      OdGePoint2dArray controlPoints2d = null;
      controlPoints2d = new OdGePoint2dArray(arrTrimmingLoopParCPSt0);
      OdGeKnotVector vecTrimmingLoopParKnots = new OdGeKnotVector(arrTrimmingLoopParKnotst0);
      OdGeNurbCurve2d pParTrimmigLoopCurve00 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots, controlPoints2d, arrWeights, false);
      /********************************************************************/
      /* For geometry of edge1                                            */
      /********************************************************************/
      controlPoints2d = new OdGePoint2dArray(arrTrimmingLoopParCPSt01);
      OdGeKnotVector vecTrimmingLoopParKnots01 = new OdGeKnotVector(arrTrimmingLoopParKnotst01);
      OdGeNurbCurve2d pParTrimmigLoopCurve01 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots01, controlPoints2d, arrWeights, false);
      /********************************************************************/
      /* For geometry of edge2                                            */
      /********************************************************************/
      controlPoints2d = new OdGePoint2dArray(arrTrimmingLoopParCPSt02);
      OdGeKnotVector vecTrimmingLoopParKnots02 = new OdGeKnotVector(arrTrimmingLoopParKnotst02);
      OdGeNurbCurve2d pParTrimmigLoopCurve02 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots02, controlPoints2d, arrWeights, false);
      /********************************************************************/
      /* For geometry of edge3                                            */
      /********************************************************************/
      controlPoints2d = new OdGePoint2dArray(arrTrimmingLoopParCPSt03);
      OdGeKnotVector vecTrimmingLoopParKnots03 = new OdGeKnotVector(arrTrimmingLoopParKnotst03);
      OdGeNurbCurve2d pParTrimmigLoopCurve03 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots03, controlPoints2d, arrWeights, false);

      /********************************************************************/
      /*  Create parametric curves for hole. For edge0                    */
      /********************************************************************/
      controlPoints2d = new OdGePoint2dArray(arrHoleLoopParCPSh00);
      OdGeKnotVector vecHoleLoopParKnots = new OdGeKnotVector(arrHoleLoopParKnotsh00);
      OdGeNurbCurve2d pParHoleLoopCurve00 = new OdGeNurbCurve2d(1, vecTrimmingLoopParKnots, controlPoints2d, arrWeights, false);
      /********************************************************************/
      /*  Create parametric curves for hole. For edge1                    */
      /********************************************************************/
      controlPoints2d = new OdGePoint2dArray(arrTrimmingLoopParCPSh01);
      OdGeKnotVector vecHoleLoopParKnots01 = new OdGeKnotVector(arrTrimmingLoopParKnotsh01);
      OdGeNurbCurve2d pParHoleLoopCurve01 = new OdGeNurbCurve2d(1, vecHoleLoopParKnots01, controlPoints2d, arrWeights, false);
      /********************************************************************/
      /*  Create parametric curves for hole. For edge2                    */
      /********************************************************************/
      controlPoints2d = new OdGePoint2dArray(arrHoleLoopParCPSh02);
      OdGeKnotVector vecHoleLoopParKnots02 = new OdGeKnotVector(arrHoleLoopParKnotsh02);
      OdGeNurbCurve2d pParHoleLoopCurve02 = new OdGeNurbCurve2d(1, vecHoleLoopParKnots02, controlPoints2d, arrWeights, false);
      /********************************************************************/
      /*  Create parametric curves for hole. For edge3                    */
      /********************************************************************/
      controlPoints2d = new OdGePoint2dArray(arrHoleLoopParCPSh03);
      OdGeKnotVector vecHoleLoopParKnots03 = new OdGeKnotVector(arrHoleLoopParKnotsh03);
      OdGeNurbCurve2d pParHoleLoopCurve03 = new OdGeNurbCurve2d(1, vecHoleLoopParKnots03, controlPoints2d, arrWeights, false);

      /********************************************************************/
      /* Create parametric curves arrays                                  */
      /********************************************************************/
      OdGeNurbCurve2dArrayPtr arrTrimmingLoopPar = new OdGeNurbCurve2dArrayPtr(), arrCurvesParHole = new OdGeNurbCurve2dArrayPtr();
      arrTrimmingLoopPar.Add(pParTrimmigLoopCurve00);
      arrTrimmingLoopPar.Add(pParTrimmigLoopCurve01);
      arrTrimmingLoopPar.Add(pParTrimmigLoopCurve02);
      arrTrimmingLoopPar.Add(pParTrimmigLoopCurve03);
      GC.SuppressFinalize(pParTrimmigLoopCurve00);
      GC.SuppressFinalize(pParTrimmigLoopCurve01);
      GC.SuppressFinalize(pParTrimmigLoopCurve02);
      GC.SuppressFinalize(pParTrimmigLoopCurve03);

      arrCurvesParHole.Add(pParHoleLoopCurve00);
      arrCurvesParHole.Add(pParHoleLoopCurve01);
      arrCurvesParHole.Add(pParHoleLoopCurve02);
      arrCurvesParHole.Add(pParHoleLoopCurve03);
      GC.SuppressFinalize(pParHoleLoopCurve00);
      GC.SuppressFinalize(pParHoleLoopCurve01);
      GC.SuppressFinalize(pParHoleLoopCurve02);
      GC.SuppressFinalize(pParHoleLoopCurve03);

      arrLoopsPar.Add(arrTrimmingLoopPar);
      arrLoopsPar.Add(arrCurvesParHole);
      GC.SuppressFinalize(arrTrimmingLoopPar);
      GC.SuppressFinalize(arrCurvesParHole);
    }

    #region NurbData
    int degreeInUt0 = 3;
    int degreeInVt0 = 3;
    int propsInUt0 = (int)(OdGe.NurbSurfaceProperties.kOpen | OdGe.NurbSurfaceProperties.kNoPoles);
    int propsInVt0 = (int)(OdGe.NurbSurfaceProperties.kOpen | OdGe.NurbSurfaceProperties.kNoPoles);
    int numControlPointsInUt0 = 6;
    int numControlPointsInVt0 = 6;

    OdGeDoubleArray arrHoleLoopKnotsh00 = new OdGeDoubleArray{ -48.125460674052619,-48.125460674052619,-48.125460674052619,-48.125460674052619,-16.041820224684201,
    16.041820224684209,48.125460674052619,48.125460674052619,48.125460674052619,48.125460674052619 };

    OdGePoint2dArray arrHoleLoopParCPSh00 = new OdGePoint2dArray { new OdGePoint2d(46.304913083018910, -30.324126422369094), new OdGePoint2d(46.304913083018889, -39.949218557179620) };
    OdGeDoubleArray arrHoleLoopParKnotsh00 = new OdGeDoubleArray { -48.125460674052619, -48.125460674052619, 48.125460674052619, 48.125460674052619 };

    //edge2
    OdGePoint3dArray arrHoleLoopCPSh01 = new OdGePoint3dArray
    {
      new OdGePoint3d(-865.59771257789123,197.05848201972219,98.221357298149528),
      new OdGePoint3d(-864.37352815795521,197.05848201972219,98.221357298149513),
      new OdGePoint3d(-861.92515931808339,197.05848201972219,98.221357298149528),
      new OdGePoint3d(-858.25260605827555,197.05848201972219,98.221357298149513),
      new OdGePoint3d(-855.80423721840361,197.05848201972219,98.221357298149513),
      new OdGePoint3d(-854.58005279846782,197.05848201972219,98.221357298149528)
    };
    OdGeDoubleArray arrHoleLoopKnotsh01 = new OdGeDoubleArray { -55.088298897117014,-55.088298897117014,-55.088298897117014,-55.088298897117014,-18.362766299039009,
    18.362766299038995,55.088298897116999,55.088298897116999,55.088298897116999,55.088298897116999 };

    OdGePoint2dArray arrTrimmingLoopParCPSh01 = new OdGePoint2dArray { new OdGePoint2d(46.304913083018910, -39.949218557179698), new OdGePoint2d(35.287253303595314, -39.949218557179577) };
    OdGeDoubleArray arrTrimmingLoopParKnotsh01 = new OdGeDoubleArray { -55.088298897116999, -55.088298897116999, 55.088298897116999, 55.088298897116999 };

    //edge3
    OdGePoint3dArray arrHoleLoopCPSh02 = new OdGePoint3dArray
    {
      new OdGePoint3d(-854.58005279846782,197.05848201972219,98.221357298149528),
      new OdGePoint3d(-854.58005279846782,193.85011797478538,98.221357298149528),
      new OdGePoint3d(-854.58005279846770,190.64175392984851,98.221357298149513),
      new OdGePoint3d(-854.58005279846782,187.43338988491166,98.221357298149528)
    };
    OdGeDoubleArray arrHoleLoopKnotsh02 = new OdGeDoubleArray { -48.125460674052619,-48.125460674052619,-48.125460674052619,-48.125460674052619,
    48.125460674052619,48.125460674052619,48.125460674052619,48.125460674052619 };

    OdGePoint2dArray arrHoleLoopParCPSh02 = new OdGePoint2dArray { new OdGePoint2d(35.287253303595399, -39.949218557179591), new OdGePoint2d(35.287253303595357, -30.324126422369066) };
    OdGeDoubleArray arrHoleLoopParKnotsh02 = new OdGeDoubleArray { -48.125460674052619, -48.125460674052619, 48.125460674052619, 48.125460674052619 };

    //edge4
    OdGePoint3dArray arrHoleLoopCPSh03 = new OdGePoint3dArray
    {
      new OdGePoint3d(-854.58005279846782,187.43338988491166,98.221357298149528),
      new OdGePoint3d(-858.25260605827543,187.43338988491166,98.221357298149513),
      new OdGePoint3d(-861.92515931808339,187.43338988491166,98.221357298149513),
      new OdGePoint3d(-865.59771257789123,187.43338988491166,98.221357298149528)
    };
    OdGeDoubleArray arrHoleLoopKnotsh03 = new OdGeDoubleArray { -55.088298897116999,-55.088298897116999,-55.088298897116999,-55.088298897116999,
    55.088298897116999,55.088298897116999,55.088298897116999,55.088298897116999 };

    OdGePoint2dArray arrHoleLoopParCPSh03 = new OdGePoint2dArray { new OdGePoint2d(35.287253303595342, -30.324126422369062), new OdGePoint2d(46.304913083018860, -30.3241264223691) };
    OdGeDoubleArray arrHoleLoopParKnotsh03 = new OdGeDoubleArray { -55.088298897116999, -55.088298897116999, 55.088298897116999, 55.088298897116999 };

    OdGePoint3dArray arrHoleLoopCPSh00 = new OdGePoint3dArray
    {
      new OdGePoint3d(-865.59771257789123,187.43338988491166,98.221357298149528),
      new OdGePoint3d(-865.59771257789123,188.50284456655729,98.221357298149513),
      new OdGePoint3d(-865.59771257789123,190.64175392984851,98.221357298149528),
      new OdGePoint3d(-865.59771257789112,193.85011797478535,98.221357298149513),
      new OdGePoint3d(-865.59771257789123,195.98902733807660,98.221357298149528),
      new OdGePoint3d(-865.59771257789123,197.05848201972219,98.221357298149528)
    };

    OdGePoint3dArray arrCPNURBSt0 = new OdGePoint3dArray {
      new OdGePoint3d(-764.20450059775521,205.23472413659516,98.221357298149513),
      new OdGePoint3d(-764.20450059775544,194.54017732013909,98.221357298149556),
      new OdGePoint3d(-764.20450059775521,173.15108368722673,98.221357298149499),
      new OdGePoint3d(-764.20450059775533,141.06744323785836,98.221357298149542),
      new OdGePoint3d(-764.20450059775499,119.67834960494608,98.221357298149485),
      new OdGePoint3d(-764.20450059775521,108.98380278848995,98.221357298149513),
      new OdGePoint3d(-776.44634479711408,205.23472413659510,98.221357298149499),
      new OdGePoint3d(-776.44634479711431,194.54017732013912,98.221357298149542),
      new OdGePoint3d(-776.44634479711408,173.15108368722665,98.221357298149499),
      new OdGePoint3d(-776.44634479711431,141.06744323785844,98.221357298149513),
      new OdGePoint3d(-776.44634479711397,119.67834960494601,98.221357298149499),
      new OdGePoint3d(-776.44634479711408,108.98380278848991,98.221357298149499),
      new OdGePoint3d(-800.93003319583340,205.23472413659513,98.221357298149499),
      new OdGePoint3d(-800.93003319583363,194.54017732013907,98.221357298149542),
      new OdGePoint3d(-800.93003319583352,173.15108368722670,98.221357298149499),
      new OdGePoint3d(-800.93003319583340,141.06744323785836,98.221357298149513),
      new OdGePoint3d(-800.93003319583340,119.67834960494606,98.221357298149499),
      new OdGePoint3d(-800.93003319583340,108.98380278848997,98.221357298149499),
      new OdGePoint3d(-837.65556579391148,205.23472413659525,98.221357298149556),
      new OdGePoint3d(-837.65556579391171,194.54017732013921,98.221357298149599),
      new OdGePoint3d(-837.65556579391159,173.15108368722673,98.221357298149542),
      new OdGePoint3d(-837.65556579391159,141.06744323785844,98.221357298149570),
      new OdGePoint3d(-837.65556579391136,119.67834960494605,98.221357298149542),
      new OdGePoint3d(-837.65556579391148,108.98380278848994,98.221357298149556),
      new OdGePoint3d(-862.13925419262989,205.23472413659513,98.221357298149499),
      new OdGePoint3d(-862.13925419263012,194.54017732013907,98.221357298149542),
      new OdGePoint3d(-862.13925419262989,173.15108368722670,98.221357298149499),
      new OdGePoint3d(-862.13925419262989,141.06744323785838,98.221357298149513),
      new OdGePoint3d(-862.13925419262966,119.67834960494601,98.221357298149499),
      new OdGePoint3d(-862.13925419262989,108.98380278848992,98.221357298149499),
      new OdGePoint3d(-874.38109839198921,205.23472413659516,98.221357298149513),
      new OdGePoint3d(-874.38109839198944,194.54017732013907,98.221357298149556),
      new OdGePoint3d(-874.38109839198898,173.15108368722670,98.221357298149499),
      new OdGePoint3d(-874.38109839198933,141.06744323785836,98.221357298149542),
      new OdGePoint3d(-874.38109839198933,119.67834960494606,98.221357298149485),
      new OdGePoint3d(-874.38109839198921,108.98380278848992,98.221357298149513)
    };
    OdGeDoubleArray arrUKnotst0 = new OdGeDoubleArray
    {
      -55.088298897116999, -55.088298897116999, -55.088298897116999, -55.088298897116999, -18.362766299038995, 18.362766299039009,  55.088298897117014, 55.088298897117014, 55.088298897117014, 55.088298897117014
    };
    OdGeDoubleArray arrVKnotst0 = new OdGeDoubleArray
    {
      -48.125460674052619, -48.125460674052619, -48.125460674052619, -48.125460674052619, -16.041820224684209, 16.041820224684201, 48.125460674052619, 48.125460674052619, 48.125460674052619, 48.125460674052619
    };

    OdGePoint3d[] arrTrimmingLoopCPSt0 = new OdGePoint3d[]
    {
      new OdGePoint3d(-874.38109839198921,205.23472413659516,98.221357298149513),
      new OdGePoint3d(-874.38109839198944,194.54017732013907,98.221357298149556),
      new OdGePoint3d(-874.38109839198898,173.1510836872267,98.221357298149499),
      new OdGePoint3d(-874.38109839198933,141.06744323785836,98.221357298149542),
      new OdGePoint3d(-874.38109839198933,119.67834960494606,98.221357298149485),
      new OdGePoint3d(-874.38109839198921,108.98380278848992,98.221357298149513)
    };
    OdGeDoubleArray arrTrimmingLoopKnotst0 = new OdGeDoubleArray { -48.125460674052619, -48.125460674052619, -48.125460674052619, -48.125460674052619, -16.041820224684209,
    16.041820224684201, 48.125460674052619, 48.125460674052619, 48.125460674052619, 48.125460674052619 };

    OdGePoint2d[] arrTrimmingLoopParCPSt0 = new OdGePoint2d[] { new OdGePoint2d(55.088298897117006, -48.125460674052619), new OdGePoint2d(55.088298897117014, 48.125460674052619) };
    OdGeDoubleArray arrTrimmingLoopParKnotst0 = new OdGeDoubleArray { -48.125460674052619, -48.125460674052619, 48.125460674052619, 48.125460674052619 };

    //edge2
    OdGePoint3d[] arrTrimmingLoopCPSt01 = new OdGePoint3d[]
    {
      new OdGePoint3d(-874.38109839198921,108.98380278848994,98.221357298149513),
      new OdGePoint3d(-837.65556579391125,108.98380278848994,98.221357298149471),
      new OdGePoint3d(-800.93003319583181,108.98380278848983,98.221357298149471),
      new OdGePoint3d(-764.20450059775521,108.98380278848995,98.221357298149513)
    };
    OdGeDoubleArray arrTrimmingLoopKnotst01 = new OdGeDoubleArray{ -55.08829889711699934, -55.08829889711699934,-55.08829889711699934,-55.08829889711699934,
    55.08829889711699934, 55.08829889711699934, 55.08829889711699934, 55.08829889711699934 };

    OdGePoint2d[] arrTrimmingLoopParCPSt01 = new OdGePoint2d[] { new OdGePoint2d(55.08829889711700645, 48.12546067405261141), new OdGePoint2d(-55.08829889711699934, 48.12546067405261141) };
    OdGeDoubleArray arrTrimmingLoopParKnotst01 = new OdGeDoubleArray { -55.08829889711699934, -55.08829889711699934, 55.08829889711699934, 55.08829889711699934 };

    //edge3
    OdGePoint3d[] arrTrimmingLoopCPSt02 = new OdGePoint3d[]
    {
      new OdGePoint3d(-764.20450059775521,108.98380278848995,98.221357298149513),
      new OdGePoint3d(-764.20450059775453,141.0674432378583,98.221357298149428),
      new OdGePoint3d(-764.2045005977559,173.15108368722696,98.221357298149641),
      new OdGePoint3d(-764.20450059775521,205.23472413659516,98.221357298149513)
    };
    OdGeDoubleArray arrTrimmingLoopKnotst02 = new OdGeDoubleArray { -48.125460674052619, -48.125460674052619, -48.125460674052619, -48.125460674052619,
    48.125460674052619, 48.125460674052619, 48.125460674052619, 48.125460674052619 };

    OdGePoint2d[] arrTrimmingLoopParCPSt02 = new OdGePoint2d[] { new OdGePoint2d(-55.088298897116999, 48.125460674052611), new OdGePoint2d(-55.088298897116999, -48.125460674052619) };
    OdGeDoubleArray arrTrimmingLoopParKnotst02 = new OdGeDoubleArray { -48.125460674052619, -48.125460674052619, 48.125460674052619, 48.125460674052619 };

    //edge4
    OdGePoint3d[] arrTrimmingLoopCPSt03 = new OdGePoint3d[]
    {
      new OdGePoint3d(-764.20450059775521,205.23472413659516,98.221357298149513),
      new OdGePoint3d(-776.44634479711408,205.2347241365951,98.221357298149499),
      new OdGePoint3d(-800.9300331958334,205.23472413659513,98.221357298149499),
      new OdGePoint3d(-837.65556579391148,205.23472413659525,98.221357298149556),
      new OdGePoint3d(-862.13925419262989,205.23472413659513,98.221357298149499),
      new OdGePoint3d(-874.38109839198921,205.23472413659516,98.221357298149513)
    };
    OdGeDoubleArray arrTrimmingLoopKnotst03 = new OdGeDoubleArray { -55.088298897116999, -55.088298897116999, -55.088298897116999, -55.088298897116999, -18.362766299038995,
    18.362766299039009, 55.088298897117014, 55.088298897117014, 55.088298897117014, 55.088298897117014 };

    OdGePoint2d[] arrTrimmingLoopParCPSt03 = new OdGePoint2d[] { new OdGePoint2d(-55.088298897116999, -48.125460674052619), new OdGePoint2d(55.088298897117014, -48.125460674052619) };
    OdGeDoubleArray arrTrimmingLoopParKnotst03 = new OdGeDoubleArray { -55.08829889711699934, -55.08829889711699934, 55.08829889711699934, 55.08829889711699934 };
    #endregion NurbData

  }
}
