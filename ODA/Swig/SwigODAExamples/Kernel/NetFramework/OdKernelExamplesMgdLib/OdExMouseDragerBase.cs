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

namespace OdKernelExamplesMgdLib
{
  public enum OdExMouseButton
  {
    None = -1,
    Left = 0,
    Middle = 1,
    Right = 2,
    XButton1 = 3,
    XButton2 = 4
  }

  public abstract class OdExMouseDragerBase
  {
    public OdExMouseDragerBase()
    {
    }

    protected void OnTextInfoChanged(String text)
    {
      OdExHelpers.SafeCallDelegate(TextInfoChanged, new OdExStringEventArgs(text));
    }

    protected void Init()
    {
      OdExHelpers.SafeCallDelegate(InitDragerEvent);
    }
    protected void UnInit()
    {
      OdExHelpers.SafeCallDelegate(UnInitDragerEvent);
    }

    protected void PrePaint()
    {
      OdExHelpers.SafeCallDelegate(PrePaintEvent);
    }

    protected void PostPaint()
    {
      OdExHelpers.SafeCallDelegate(PostPaintEvent);
    }

    protected void ApplicationDoEvents()
    {
      OdExHelpers.SafeCallDelegate(ApplicationDoProcessingEvent);
    }

    public abstract void OnMouseDown(object sender, OdExMouseButton buttons, int mouseX, int mouseY);
    public abstract void OnMouseMove(object sender, OdExMouseButton buttons, int mouseX, int mouseY);
    public abstract void OnKeyPress(object sender, char keyChar);

    public class OdExStringEventArgs : EventArgs
    {
      public OdExStringEventArgs(string str) { this.Text = str; }
      public string Text { get; private set; }
    }

    public delegate void OdExTextChangedEventHandler(OdExStringEventArgs args);
    public delegate void InitDragerDelegate();
    public delegate void UnInitDragerDelegate();
    public delegate void PrePaintViewDelegate();
    public delegate void PostPaintViewDelegate();
    public delegate void ApplicationDoEventsDelegate();

    public event OdExTextChangedEventHandler TextInfoChanged;
    public event InitDragerDelegate InitDragerEvent;
    public event UnInitDragerDelegate UnInitDragerEvent;
    public event PrePaintViewDelegate PrePaintEvent;
    public event PostPaintViewDelegate PostPaintEvent;
    public event ApplicationDoEventsDelegate ApplicationDoProcessingEvent;
  }
}