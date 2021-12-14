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
using System.Timers;
using Teigha.Core;
using Teigha.TD;
using OdDrawingsExamplesMgdLib.ExTrackers;
using OdKernelExamplesMgdLib.ExJigs;
using OdKernelExamplesMgdLib.ExView;
using OdKernelExamplesMgdLib;

namespace OdDrawingsExamplesMgdLib.ExJigs
{
  public class OdExTextJig : OdExJigBase
  {
    private OdGePoint3d m_positionPoint = null;
    private double m_textHeight = 10.0;
    private double m_angelRotation = 0.0;
    private bool m_cancel = false;
    private bool m_textSetted = false;
    private Timer m_cursorTimer = new Timer();
    private bool m_cursorShowed = false;
    private string m_textBuffer = String.Empty;
    private bool m_startPoint_IsValid { get { return m_positionPoint != null; } }

    public OdExTextJig(OdGsView targetView, OdExStringTracker tracker) : base(targetView)
    {
      this.Tracker = tracker;
    }

    public OdExStringTracker Tracker { get; private set; }

    public static OdExTextJig CreateDefaultJig(OdDbDatabase db, OdGsLayoutHelper gsView)
    {
      OdDbText pText = OdDbText.createObject();
      pText.setDatabaseDefaults(db);

      OdExStringTracker trackerText = new OdExStringTracker(pText);

      OdGsView targetView = OdExAuxUtilities.activeTopView(db, gsView);

      OdExTextJig jig = new OdExTextJig(targetView, trackerText);
      return jig;
    }

    public override bool DoIt()
    {
      Init();
      if (!m_startPoint_IsValid)
      {
        OnTextInfoChanged("Select Text Position (Mouse Left Click - Accept, Right Click - Cancel)");

        for (; !m_cancel && !m_startPoint_IsValid;)
          ApplicationDoEvents();
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

      //May change dynamic
      TextHorzMode hMode = TextHorzMode.kTextLeft;
      TextVertMode vMode = TextVertMode.kTextBase;
      Tracker.TargetText.setHorizontalMode(hMode);
      Tracker.TargetText.setVerticalMode(vMode);

      //May change dynamic
      Tracker.TargetText.setHeight(m_textHeight);

      //May change dynamic
      Tracker.TargetText.setRotation(m_angelRotation);

      OnTextInfoChanged("Set Text (Mouse Left Click - Accept, Right Click - Cancel)");

      m_cursorTimer.Elapsed += CursorTimer_Tick;
      m_cursorTimer.Interval = 500;
      m_cursorTimer.Start();

      PrePaint();
      Tracker.addDrawables(activeTopView);
      PostPaint();

      for (; !m_cancel && !m_textSetted;)
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
      if (!m_startPoint_IsValid)
      {
        if (buttons == OdExMouseButton.Left)
        {
          m_positionPoint = OdExGsDeviceHelpers.ScreenPointToWorld(m_targetView, mouseX, mouseY);
          Tracker.TargetText.setPosition(m_positionPoint);
        }
        else if (buttons == OdExMouseButton.Right)
          m_cancel = true;
      }
      else if (!m_textSetted)
      {
        m_cursorTimer.Stop();
        if (buttons == OdExMouseButton.Left)
          m_textSetted = true;
        else if (buttons == OdExMouseButton.Right)
          m_cancel = true;
      }
    }

    public override void OnMouseMove(object sender, OdExMouseButton buttons, int mouseX, int mouseY)
    {
    }

    public override void OnKeyPress(object sender, char ch)
    {
      if (ch == '\r')
        return;

      if (ch == '\b')
        m_textBuffer = m_textBuffer.Substring(0, m_textBuffer.Length - 1);
      else
        m_textBuffer += ch;

      PrePaint();
      Tracker.TargetText.setTextString(m_textBuffer);
      Tracker.setCursor(m_cursorShowed = false);
      PostPaint();
      m_cursorTimer.Start();
    }

    private void CursorTimer_Tick(object sender, EventArgs e)
    {
      PrePaint();
      if (m_cancel || m_textSetted)
      {
        m_cursorTimer.Stop();
        m_cursorShowed = false;
        Tracker.setCursor(m_cursorShowed);
      }
      else
        Tracker.setCursor(m_cursorShowed = !m_cursorShowed);
      PostPaint();
    }
  }
}