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
  public class OdExViewZoom : OdExViewPaintEventsInterafce, OdExViewEnableFunctionalityInterface
  {
    public bool IsEnable { get; set; }

    public OdGsView TargetView { get; set; }

    public event OdExViewDelegates.PostPaintDelegate PostPaintEvent;
    public event OdExViewDelegates.PrePaintDelegate PrePaintEvent;

    public OdExViewZoom()
    {
      IsEnable = false;
    }

    public void OnZoom(int xMouseLoc, int yMouseLoc, int wheelDelta)
    {
      if (!IsEnable || TargetView == null)
        return;

      OdExHelpers.SafeCallDelegate(PrePaintEvent);

      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OdGePoint2d correctScreenPoint = new OdGePoint2d(xMouseLoc, yMouseLoc);

        OdGePoint3d worldPoint = OdExGsDeviceHelpers.GetWorldCoordinateByScreen(TargetView, correctScreenPoint);

        OnZoomActiveView((double)wheelDelta / 120);

        OnMoveToCenterZoom(correctScreenPoint, worldPoint);

      }
      catch (Exception err)
      {
        throw err;
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }

      OdExHelpers.SafeCallDelegate(PostPaintEvent);
    }

    private void OnZoomActiveView(double steps)
    {
      if (TargetView.isPerspective())
      {
        OdGePoint3d position = TargetView.position(), target = TargetView.target();
        OdGeVector3d direction = target - position;
        OdGeVector3d upVector = TargetView.upVector();
        double width = TargetView.fieldWidth(), height = TargetView.fieldHeight();

        //calculate an appropriate offset using real size of the frame
        if (width < height)
          steps *= width / 2.0;
        else
          steps *= height / 2.0;

        direction *= steps;

        position += direction;
        target += direction;

        TargetView.setView(position, target, upVector, width, height, OdGsView.Projection.kPerspective);
      }
      else
      {
        TargetView.zoom(Math.Pow(1.11, steps));
      }
    }

    private void OnMoveToCenterZoom(OdGePoint2d correctScreenPoint, OdGePoint3d worldPoint)
    {
      //get new screen point of the same world point
      OdGeMatrix3d worldToDevice = TargetView.worldToDeviceMatrix();
      OdGePoint3d newScreenPoint = worldToDevice * worldPoint;

      //get world points on the focal plane
      OdGePoint3d movingPoint = OdExGsDeviceHelpers.GetWorldCoordinateByScreen(TargetView, new OdGePoint2d(newScreenPoint.x, newScreenPoint.y));
      OdGePoint3d destinationPoint = OdExGsDeviceHelpers.GetWorldCoordinateByScreen(TargetView, correctScreenPoint);

      OdGePoint3d position = TargetView.position(), target = TargetView.target();
      OdGeVector3d upVector = TargetView.upVector();
      double width = TargetView.fieldWidth(), height = TargetView.fieldHeight();
      bool isPerspective = TargetView.isPerspective();

      //shift the camera so points coincide
      OdGeVector3d offset = destinationPoint - movingPoint;
      position -= offset;
      target -= offset;

      TargetView.setView(position, target, upVector, width, height, isPerspective ? OdGsView.Projection.kPerspective : OdGsView.Projection.kParallel);
    }
  }
}