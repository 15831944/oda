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
  public class OdExCircleRadiusJig : OdExJigBase
  {
    private OdExRadiusTracker m_tracker;
    private OdDbDatabase m_database;
    private int m_options;
    private OdGePoint3d m_centerPnt = null;
    private bool m_cancel = false;
    private bool m_centerSelected = false;
    private bool m_radiusSelected = false;

    public OdExCircleRadiusJig(int options, OdDbDatabase db, OdGsView targetView, OdExCircleTracker pCrTr)
      : base(targetView)
    {
      this.m_tracker = new OdExRadiusTracker(pCrTr);
      this.m_database = db;
      this.m_options = options;
    }

    public OdExRadiusTracker Tracker { get { return m_tracker; } }

    public static OdExCircleRadiusJig CreateDefaultCircleRadiusJig(OdDbDatabase db, OdGsLayoutHelper gsView)
    {
      OdDbCircle pCircle = OdDbCircle.createObject();
      pCircle.setDatabaseDefaults(db);
      pCircle.setNormal(new OdGeVector3d(OdExAuxUtilities.ucsNormal(db)));

      int m_options;
      unchecked
      {
        m_options =
          (int)((int)OdDbInputOptions.kGdsFromLastPoint |
          (int)OdDbInputOptions.kGptRubberBand |
          (long)CommonInputOptions.kInpThrowEmpty);
      }

      OdExCircleTracker trCircle = new OdExCircleTracker(pCircle);
      OdGsView targetView = OdExAuxUtilities.activeTopView(db, gsView);

      OdExCircleRadiusJig cJig = new OdExCircleRadiusJig(m_options, db, targetView, trCircle);

      return cJig;
    }

    public override bool DoIt()
    {
      Init();

      OnTextInfoChanged("Select Center Point of Circle (Mouse Left Click - Accept, Right Click - Cancel)");

      for (; !m_cancel && !m_centerSelected;)
        ApplicationDoEvents();

      OdGsView activeTopView = m_targetView;

      if (m_cancel)
      {
        PrePaint();
        m_tracker.removeDrawables(activeTopView);
        OnTextInfoChanged("Canceled");
        PostPaint();
        UnInit();
        return false;
      }

      OnTextInfoChanged("Select radius of circle (Mouse Left Click - Accept, Right Click - Cancel)");

      PrePaint();
      m_tracker.addDrawables(activeTopView);
      PostPaint();

      for (; !m_cancel && !m_radiusSelected;)
        ApplicationDoEvents();

      OnTextInfoChanged("Success");

      PrePaint();
      m_tracker.removeDrawables(activeTopView);
      PostPaint();

      UnInit();

      bool finished = !m_cancel;

      return finished;
    }

    public override void OnMouseDown(object sender, OdExMouseButton buttons, int mouseX, int mouseY)
    {
      if (m_cancel)
        return;

      if (!m_centerSelected)
      {
        if (buttons == OdExMouseButton.Left)
        {
          PrePaint();
          m_centerPnt = OdExGsDeviceHelpers.ScreenPointToWorld(m_targetView, mouseX, mouseY);
          m_tracker.m_CircleTracker.m_TargetCircle.setCenter(m_centerPnt);
          m_centerSelected = true;
          PostPaint();
        }
        else if (buttons == OdExMouseButton.Right)
          m_cancel = true;
      }
      else if (!m_radiusSelected)
      {
        if (buttons == OdExMouseButton.Left)
          m_radiusSelected = true;
        else if (buttons == OdExMouseButton.Right)
          m_cancel = true;
      }
    }

    public override void OnMouseMove(object sender, OdExMouseButton buttons, int mouseX, int mouseY)
    {
      if (!m_centerSelected || m_cancel || m_radiusSelected)
        return;

      PrePaint();

      MemoryTransaction tr = MemoryManager.GetMemoryManager().StartTransaction();

      OdGePoint3d currentPoint = OdExGsDeviceHelpers.ScreenPointToWorld(m_targetView, mouseX, mouseY);

      m_tracker.setValue(getDistByOptions(currentPoint, m_database, m_options));

      MemoryManager.GetMemoryManager().StopTransaction(tr);

      PostPaint();
    }

    public override void OnKeyPress(object sender, char keyChar)
    {
    }

    private double getDistByOptions(OdGePoint3d pt, OdDbDatabase pDb, int m_options)
    {
      OdGeVector3d vect = pt - m_centerPnt;
      if (OdExBitsOpHelpers.GETBIT(m_options, (int)OdDbInputOptions.kGdsSignedDist) && pDb != null)
        vect = pDb.formatter().toUCS(pt) - pDb.formatter().toUCS(m_centerPnt);
      if (OdExBitsOpHelpers.GETBIT(m_options, (int)OdDbInputOptions.kGds2d))
        vect.z = 0;
      double dist = vect.length();
      return dist;
    }
  }
}