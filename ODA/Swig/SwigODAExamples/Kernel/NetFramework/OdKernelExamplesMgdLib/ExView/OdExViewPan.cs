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

using Teigha.Core;

namespace OdKernelExamplesMgdLib.ExView
{
  public class OdExViewPan : OdExViewPaintEventsInterafce, OdExViewEnableFunctionalityInterface
  {
    public bool IsEnable { get; set; }

    public OdGsView TargetView { get; set; }

    private OdGePoint3d m_pos;
    private OdGePoint3d m_prevPt;

    private bool m_started = false;

    public event OdExViewDelegates.PostPaintDelegate PostPaintEvent;
    public event OdExViewDelegates.PrePaintDelegate PrePaintEvent;

    public OdExViewPan()
    {
      IsEnable = false;
    }

    public void OnStartPan(int xPos, int yPos)
    {
      if (!IsEnable || TargetView == null || m_started)
        return;
      m_pos = TargetView.position();

      // remember the difference between click point in WCS and camera current position
      m_prevPt = OdExGsDeviceHelpers.GetWorldCoordinateByScreen(TargetView, new OdGePoint2d(xPos, yPos)) - m_pos.asVector();

      m_started = true;
    }

    public void OnMovePan(int xPos, int yPos)
    {
      if (!IsEnable || TargetView == null || !m_started)
        return;

      OdExHelpers.SafeCallDelegate(PrePaintEvent);

      // calculate click point in WCS
      OdGePoint2d ptScreen = new OdGePoint2d(xPos, yPos);
      OdGePoint3d pt = OdExGsDeviceHelpers.GetWorldCoordinateByScreen(TargetView, ptScreen);

      //obtain delta for dolly
      OdGePoint3d v = m_prevPt - (pt - m_pos);
      OdGeVector3d delta = v.asVector();

      //transform delta to eye
      delta = delta.transformBy(TargetView.viewingMatrix());

      //perform camera moving
      TargetView.dolly(delta.x, delta.y, delta.z);

      //remember the difference between click point in WCS and camera previous position
      m_prevPt = pt - m_pos.asVector();

      //remember camera current position
      m_pos = TargetView.position();

      OdExHelpers.SafeCallDelegate(PostPaintEvent);
    }

    public void OnFinishPan()
    {
      m_started = false;
    }
  }
}