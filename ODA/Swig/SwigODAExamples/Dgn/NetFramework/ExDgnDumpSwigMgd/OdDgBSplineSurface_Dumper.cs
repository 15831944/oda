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
  class OdDgBSplineSurface_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgBSplineSurface element = (OdDgBSplineSurface)pObject;

      startDescription(element);

      byte uOrderU = 0;
      byte uOrderV = 0;
      bool bRational = false;
      bool bClosedInU = false;
      bool bClosedInV = false;
      int nCtrlPtsInU = 0;
      int nCtrlPtsInV = 0;
      OdGePoint3dArray arrCtrlPts = new OdGePoint3dArray();
      OdGeDoubleArray arrWeights = new OdGeDoubleArray();
      OdGeKnotVector arrKnotsU = new OdGeKnotVector();
      OdGeKnotVector arrKnotsV = new OdGeKnotVector();
      uint nRulesU = 0;
      uint nRulesV = 0;

      element.get(out uOrderU, out uOrderV, out bRational, out bClosedInU, out bClosedInV, out nCtrlPtsInU, out nCtrlPtsInV, arrCtrlPts,
                  arrWeights, arrKnotsU, arrKnotsV);
      element.getNumberOfSpansInU(out nRulesU);
      element.getNumberOfSpansInV(out nRulesV);

      //simple fields
      writeFieldValue("Order U", uOrderU);
      writeFieldValue("Order V", uOrderV);
      writeFieldValue("Closed U", bClosedInU);
      writeFieldValue("Closed V", bClosedInV);
      writeFieldValue("Display curve", element.getSurfaceDisplayFlag());
      writeFieldValue("Display polygon", element.getControlNetDisplayFlag());
      writeFieldValue("Rational", bRational);
      writeFieldValue("Number of rules U", nRulesU);
      writeFieldValue("Number of rules V", nRulesV);
      writeFieldValue("Number of poles U", nCtrlPtsInU);
      writeFieldValue("Number of poles V", nCtrlPtsInV);
      writeFieldValue("Number of knots U", arrKnotsU.length());
      writeFieldValue("Number of knots V", arrKnotsV.length());
      writeFieldValue("Number of boundaries", element.getBoundariesCount());
      writeFieldValue("Hole", element.getHoleFlag());

      for (uint nBoundariesCount = element.getBoundariesCount(), i = 0;
          i < nBoundariesCount;
          i++
          )
      {
        writeShift();
        //fwprintf( DumpStream, L"Boundary %d:\n", i );
        writeFieldValue("Boundary ", i.ToString());
        OdGePoint2dArray arrBoundaryPts = new OdGePoint2dArray();
        element.getBoundary(i, arrBoundaryPts);
        writeFieldValue("Number of boundary vertices", arrBoundaryPts.Count);
        for (int BoundaryVerticesCount = arrBoundaryPts.Count, j = 0;
                j < BoundaryVerticesCount;
                j++
                )
        {
          writeFieldValue("Vertex", arrBoundaryPts[j]);
        }
      }

      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgBSplineSurface";
    }
  }
}