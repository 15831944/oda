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
  public class OdExViewRotation : OdExViewPaintEventsInterafce, OdExViewEnableFunctionalityInterface
  {
    public event OdExViewDelegates.PostPaintDelegate PostPaintEvent;
    public event OdExViewDelegates.PrePaintDelegate PrePaintEvent;

    public bool IsEnable { get; set; }

    public OdGsView TargetView { get; set; }

    public OdExViewRotation()
    {
      IsEnable = false;
    }
    private OdGePoint3d m_viewCenter;
    private OdGePoint3d m_prevPt;
    private double m_D;

    private bool m_started;
    public void OnStartRotationHandler(int xPos, int yPos)
    {
      if (!IsEnable || TargetView == null || m_started)
        return;

      // calculate click point in WCS
      OdGePoint3d pt = OdExGsDeviceHelpers.GetWorldCoordinateByScreen(TargetView, new OdGePoint2d(xPos, yPos));

      // transfer point to the eye coordinate system
      m_prevPt = TargetView.viewingMatrix() * pt;

      // calculate mouse move to rotation angle conversion coefficient in base of viewport dc size
      OdGePoint2d pt1, pt2;
      OdExGsDeviceHelpers.GetViewDcCorners(TargetView, out pt1, out pt2);
      pt2.x -= pt1.x;
      pt2.y -= pt1.y;
      double r = Math.Min(pt2.x, pt2.y);
      m_D = r;

      OdAbstractViewPE pVpPE = OdAbstractViewPE.cast(TargetView);

      OdGeBoundBlock3d extents = new OdGeBoundBlock3d();
      bool bBboxValid = pVpPE.viewExtents(TargetView, extents);

      OdGePoint3d m_viewCenter = extents.center();
      m_viewCenter.transformBy(pVpPE.eyeToWorld(TargetView));

      m_started = true;
    }

    public void OnMoveRotationHandler(int xPos, int yPos)
    {
      if (!IsEnable || TargetView == null || !m_started)
        return;

      OdExHelpers.SafeCallDelegate(PrePaintEvent);

      //calculate click point in WCS
      OdGePoint2d ptScreen = new OdGePoint2d(xPos, yPos);
      OdGePoint3d pt = OdExGsDeviceHelpers.GetWorldCoordinateByScreen(TargetView, ptScreen);

      //transfer point to the eye coordinate system
      OdGePoint3d ptView = TargetView.viewingMatrix() * pt;

      //calculate the angles for the rotation about appropriate axes
      double distX = ptView.x - m_prevPt.x; // around vertical
      double distY = ptView.y - m_prevPt.y; // around horizontal

      distX *= -Globals.OdaPI / m_D;
      distY *= Globals.OdaPI / m_D;

      //perform camera orbiting
      OdExGsDeviceHelpers.Orbit(TargetView, distY, distX, m_viewCenter);

      // store previous click point
      m_prevPt = ptView;

      OdExHelpers.SafeCallDelegate(PostPaintEvent);
    }
    public void OnFinishRotationHandler()
    {
      m_started = false;
    }
  }
}