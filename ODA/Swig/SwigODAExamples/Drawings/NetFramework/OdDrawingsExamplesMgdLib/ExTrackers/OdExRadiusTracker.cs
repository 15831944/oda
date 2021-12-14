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

namespace OdDrawingsExamplesMgdLib.ExTrackers
{
  public class OdExRadiusTracker : OdEdRealTracker //--- use to drag the radius  -----------------------------
  {
    public OdExCircleTracker m_CircleTracker { get; private set; }
    private bool m_bDiameter { get; set; }

    public OdExRadiusTracker(OdExCircleTracker pCrTr) : base(IntPtr.Zero, false)
    {
      m_CircleTracker = pCrTr;
      m_bDiameter = false;
    }

    public override void setValue(double dRadius)        // callback function, call OnMouseMove,
    {
      if (m_bDiameter)
        dRadius /= 2.0;
      m_CircleTracker.m_TargetCircle.setRadius(dRadius);
      m_CircleTracker.update();
    }

    public override int addDrawables(OdGsView pView)
    {
      return m_CircleTracker.addDrawables(pView);
    }

    public override void removeDrawables(OdGsView pView)
    {
      m_CircleTracker.removeDrawables(pView);
    }
  }

}
