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

using OdDrawingsExamplesMgdLib.ExTrackers;
using OdKernelExamplesMgdLib;
using OdKernelExamplesMgdLib.ExJigs;
using OdKernelExamplesMgdLib.ExView;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.ExJigs
{
  public class OdExLineJig : OdExJigBase
  {
    private bool m_StartPoint_IsValid { get { return StartPoint != null; } }
    private bool m_EndPoint_IsValid { get { return EndPoint != null; } }
    private bool m_cancel = false;

    public OdExLineJig(OdExLineTracker pLineTr, OdGsView targetView) : base(targetView)
    {
      this.Tracker = pLineTr;
    }

    public OdExLineJig(OdExLineTracker pLineTr, OdGsView targetView, OdGePoint3d startPoint) : this(pLineTr, targetView)
    {
      this.StartPoint = startPoint;
    }

    public OdExLineTracker Tracker { get; private set; }
    public OdGePoint3d StartPoint { get; private set; }
    public OdGePoint3d EndPoint { get; private set; }

    public static OdExLineJig CreateDefaultJig(OdDbDatabase db, OdGsLayoutHelper gsView, OdGePoint3d startPoint)
    {
      OdGeVector3d vNormal = new OdGeVector3d(OdExAuxUtilities.ucsNormal(db));

      OdDbLine pNewLine = OdDbLine.createObject();
      pNewLine.setDatabaseDefaults(db);
      pNewLine.setNormal(vNormal);

      OdExLineTracker lineTr = new OdExLineTracker(pNewLine);

      OdGsView targetView = OdExAuxUtilities.activeTopView(db, gsView);

      OdExLineJig jig = new OdExLineJig(lineTr, targetView, startPoint);

      return jig;
    }

    public override bool DoIt()
    {
      Init();

      if (!m_StartPoint_IsValid)
      {
        OnTextInfoChanged("Select Start Point Of Line (Mouse Left Click - Accept, Right Click - Cancel)");

        for (; !m_cancel && !m_StartPoint_IsValid;)
          ApplicationDoEvents();
      }
      else
      {
        Tracker.TargetLine.setStartPoint(StartPoint);
      }

      OdGsView activeTopView = m_targetView;

      if (m_cancel)
      {
        PrePaint();
        Tracker.removeDrawables(activeTopView);
        PostPaint();
        OnTextInfoChanged("Canceled");
        UnInit();
        return false;
      }

      OnTextInfoChanged("Select End Point Of Line (Mouse Left Click - Accept, Right Click - Cancel)");

      Tracker.addDrawables(activeTopView);

      for (; !m_cancel && !m_EndPoint_IsValid;)
        ApplicationDoEvents();

      OnTextInfoChanged("Success");

      PrePaint();
      Tracker.removeDrawables(activeTopView);
      PostPaint();
      UnInit();

      bool finished = !m_cancel;

      return finished;
    }

    public override void OnMouseDown(object sender, OdExMouseButton buttons, int mouseX, int mouseY)
    {
      if (m_cancel)
        return;

      if (!m_StartPoint_IsValid)
      {
        if (buttons == OdExMouseButton.Left)
        {
          StartPoint = OdExGsDeviceHelpers.ScreenPointToWorld(m_targetView, mouseX, mouseY);
          Tracker.TargetLine.setStartPoint(StartPoint);
        }
        else if (buttons == OdExMouseButton.Right)
          m_cancel = true;
      }
      else if (!m_EndPoint_IsValid)
      {
        if (buttons == OdExMouseButton.Left)
        {
          EndPoint = OdExGsDeviceHelpers.ScreenPointToWorld(m_targetView, mouseX, mouseY);
          Tracker.TargetLine.setEndPoint(EndPoint);
        }
        else if (buttons == OdExMouseButton.Right)
          m_cancel = true;
      }
    }

    public override void OnMouseMove(object sender, OdExMouseButton buttons, int mouseX, int mouseY)
    {
      if (m_EndPoint_IsValid || !m_StartPoint_IsValid || m_cancel)
        return;

      PrePaint();

      MemoryTransaction tr = MemoryManager.GetMemoryManager().StartTransaction();

      OdGePoint3d currentPoint = OdExGsDeviceHelpers.ScreenPointToWorld(m_targetView, mouseX, mouseY);

      Tracker.setValue(currentPoint);

      MemoryManager.GetMemoryManager().StopTransaction(tr);

      PostPaint();
    }

    public override void OnKeyPress(object sender, char e)
    {
    }
  }
}
