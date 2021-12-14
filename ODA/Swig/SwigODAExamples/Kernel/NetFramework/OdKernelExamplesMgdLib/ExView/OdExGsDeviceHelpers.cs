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

namespace OdKernelExamplesMgdLib.ExView
{
  public static class OdExGsDeviceHelpers
  {
    public static String RenderModeToVisualStyle(OdGsView.RenderMode rm)
    {
      // @@@TODO: create ODA-specific "FlatShaded" and "FlatShadedWithEdges" visual styles based onto "Shaded" and "ShadedWithEdges" visual styles.
      switch (rm)
      {
        case OdGsView.RenderMode.kWireframe: return "Wireframe";
        case OdGsView.RenderMode.kHiddenLine: return "Hidden";
        case OdGsView.RenderMode.kFlatShaded: return "Shaded"; // unfortunately there is no direct comparison with any correctly configured VS's. // OdDb::kszVSFlat;
        case OdGsView.RenderMode.kGouraudShaded: return "kszVSShaded";
        case OdGsView.RenderMode.kFlatShadedWithWireframe: return "kszVSShadedWithEdges"; // unfortunately there is no direct comparison with any correctly configured VS's. // OdDb::kszVSFlatWithEdges;
        case OdGsView.RenderMode.kGouraudShadedWithWireframe: return "kszVSShadedWithEdges";
        default: return "2dWireframe";
      }
    }

    public static OdGsDCPoint[] SelectionPointsByPointOnScreen(OdGsView pView, short pickBox, int x, int y)
    {
      OdGePoint3d pt = ScreenPointToWorld(pView, x, y);
      pt = pt.transformBy(pView.worldToDeviceMatrix());
      OdGsDCPoint pt1 = new OdGsDCPoint((int)pt.x - pickBox, (int)pt.y - pickBox);
      OdGsDCPoint pt2 = new OdGsDCPoint((int)pt.x + pickBox, (int)pt.y + pickBox);
      OdGsDCPoint[] pts = new OdGsDCPoint[] { pt1, pt2 };
      return pts;
    }

    public static OdGePoint3d ScreenPointToWorld(OdGsView pView, int x, int y)
    {
      MemoryTransaction tr = MemoryManager.GetMemoryManager().StartTransaction();
      OdGePoint3d wcsPt = new OdGePoint3d(x, y, 0.0);
      wcsPt = wcsPt.transformBy((pView.screenMatrix() * pView.projectionMatrix()).inverse());
      wcsPt = new OdGePoint3d(wcsPt.x, wcsPt.y, 0.0);
      OdAbstractViewPE pVpPE = OdAbstractViewPE.cast(pView);
      OdGePoint3d result = wcsPt.transformBy(pVpPE.eyeToWorld(pView));
      MemoryManager.GetMemoryManager().StopTransaction(tr);
      return result;
    }

    public static OdGePoint3d GetWorldCoordinateByScreen(OdGsView view, int x, int y)
    {
      return GetWorldCoordinateByScreen(view, new OdGePoint2d(x, y));
    }

    public static OdGePoint3d GetWorldCoordinateByScreen(OdGsView view, OdGePoint2d screenPoint)
    {
      //directions, origin, etc
      OdGePoint3d target = view.target();
      OdGeVector3d direction = target - view.position();
      OdGeVector3d yVector = view.upVector();
      OdGeVector3d xVector = direction.crossProduct(yVector).normal();

      OdGePoint3d center, stepX, stepY;
      {
        center = new OdGePoint3d(target);
        stepX = target + xVector;
        stepY = target + yVector;

        OdGeMatrix3d matrix = view.worldToDeviceMatrix();
        stepX = stepX.transformBy(matrix);
        stepY = stepY.transformBy(matrix);
        center = center.transformBy(matrix);
      }

      //make the calculation
      OdGePoint3d result;
      {
        double x, y;
        x = (screenPoint.x - center.x) / (stepX.x - center.x);
        y = (screenPoint.y - center.y) / (stepY.y - center.y);

        result = target + (xVector * x) + (yVector * y);
      }

      return result;
    }

    public static void GetViewDcCorners(OdGsView pView, out OdGePoint2d lower_left, out OdGePoint2d upper_right)
    {
      OdGePoint3d targetPt = pView.viewingMatrix() * pView.target();
      double halfFieldWidth = pView.fieldWidth() / 2.0;
      double halfFieldHeight = pView.fieldHeight() / 2.0;
      lower_left = new OdGePoint2d();
      upper_right = new OdGePoint2d();
      lower_left.x = targetPt.x - halfFieldWidth;
      lower_left.y = targetPt.y - halfFieldHeight;
      upper_right.x = targetPt.x + halfFieldWidth;
      upper_right.y = targetPt.y + halfFieldHeight;
    }

    public static void Orbit(OdGsView pView, double xOrbit, double yOrbit, OdGePoint3d pt)
    {
      OdGeVector3d delta = new OdGeVector3d();

      if (pt != null)
      {
        OdGePoint3d v = pView.target();
        OdGeVector3d target = v.asVector();
        delta = pt.asVector() - target;
        delta.transformBy(pView.viewingMatrix());
      }

      //the rotation should be performed around the 'pt'.
      //That's why at first we should move the camera with purpose make target equal to the 'pt.
      //Then we should perform the rotation and finally we should to move the camera in opposite direction
      pView.dolly(delta);
      pView.orbit(xOrbit, yOrbit);
      pView.dolly(-delta);
    }
  }
}