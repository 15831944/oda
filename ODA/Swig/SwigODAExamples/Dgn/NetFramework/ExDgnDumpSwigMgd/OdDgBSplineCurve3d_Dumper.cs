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
using Teigha.Core;
using Teigha.TG;

namespace ExDgnDumpSwigMgd
{
  class OdDgBSplineCurve2d_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgBSplineCurve2d element = (OdDgBSplineCurve2d)pObject;
      startDescription(element);
      writeFieldValue("Order", element.getOrder());
      writeFieldValue("Display curve", element.getCurveDisplayFlag());
      writeFieldValue("Display polygon", element.getPolygonDisplayFlag());
      writeFieldValue("Closed", element.getClosedFlag());

      if (element.hasFitData())
      {
        writeFieldValue("Construction type", "kDefinedByFitPoints");
        writeFieldValue("Natural tangents", element.getNaturalTangentsFlag());
        writeFieldValue("Chord length tangents", element.getChordLenTangentsFlag());
        writeFieldValue("Collinear tangents", element.getColinearTangentsFlag());

        OdGePoint2dArray fitPoints = new OdGePoint2dArray();
        UInt32 uOrder = 0;
        bool bTangentExists = false;
        OdGeVector2d vrStartTangent = new OdGeVector2d();
        OdGeVector2d vrEndTangent = new OdGeVector2d();
        bool bUniformKnots = false;
        element.getFitData(fitPoints, out uOrder, out bTangentExists, vrStartTangent, vrEndTangent, out bUniformKnots);

        OdGeKnotVector vrKnots = element.getKnots();

        writeFieldValue("Num Fit Points", fitPoints.Count);

        for (int i = 0; i < fitPoints.Count; i++)
        {
          string strFitPtsName = "  Point " + i.ToString();
          writeFieldValue(strFitPtsName, fitPoints[i]);
        }

        writeFieldValue("Start Tangent", vrStartTangent);
        writeFieldValue("End Tangent", vrEndTangent);
        writeFieldValue("Uniform Knots Flag", bUniformKnots);

        writeFieldValue("Num Knots", vrKnots.length());

        for (int j = 0; j < vrKnots.length(); j++)
        {
          String strKnotName = "  Knot " + j.ToString();
          writeFieldValue(strKnotName, vrKnots[j]);
        }
      }
      else
      {
        writeFieldValue("Construction type", "kDefinedByNurbsData");
        writeFieldValue("Rational", element.isRational());

        OdGePoint2dArray arrCtrlPts = new OdGePoint2dArray();
        OdGeKnotVector vrKnots = new OdGeKnotVector();
        OdGeDoubleArray arrWeights = new OdGeDoubleArray();
        UInt32 uOrder = 0;
        bool bClosed = false;
        bool bRational = false;

        element.getNurbsData(out uOrder, out bRational, out bClosed, arrCtrlPts, vrKnots, arrWeights);

        writeFieldValue("Num Control Points", arrCtrlPts.Count);

        for (int i = 0; i < arrCtrlPts.Count; i++)
        {
          String strCtrlPtsName = "  Point %d" + i.ToString();
          writeFieldValue(strCtrlPtsName, arrCtrlPts[i]);
        }

        writeFieldValue("Num Knots", vrKnots.length());

        for (int j = 0; j < vrKnots.length(); j++)
        {
          String strKnotName = "  Knot %d" + j.ToString();
          writeFieldValue(strKnotName, vrKnots[j]);
        }

        if (bRational)
        {
          writeFieldValue("Num Weights", arrWeights.Count);

          for (int k = 0; k < arrWeights.Count; k++)
          {
            String strWeightName = "  Weight %d" + k.ToString();
            writeFieldValue(strWeightName, arrWeights[k]);
          }
        }
      }

      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgBSplineCurve2d";
    }
  }
}