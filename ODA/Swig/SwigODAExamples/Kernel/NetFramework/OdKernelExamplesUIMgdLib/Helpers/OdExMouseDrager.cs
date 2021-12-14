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

namespace OdKernelExamplesUIMgdLib.ExTrackers
{
  public class OdExStringEventArgs : EventArgs
  {
    public OdExStringEventArgs(string str)
    {
      this.Text = str;
    }

    public string Text { get; }
  }

  public delegate void TextChangedEventHandler(object sender, OdExStringEventArgs args);

  public abstract class OdExMouseDragerWinFormsBase : OdExMouseDragerBase
  {
    protected System.Windows.Forms.Control m_targetControl;

    public OdExMouseDragerWinFormsBase(System.Windows.Forms.Control targetControl, OdGsView targetView) :base(targetView)
    {
      m_targetControl = targetControl;
    }

    protected void Init()
    {
      this.m_targetControl.MouseClick += OnMouseDown;
      this.m_targetControl.MouseMove += OnMouseMove;
      this.m_targetControl.KeyPress += OnKeyPress;
    }

    protected void UInit()
    {
      this.m_targetControl.MouseClick -= OnMouseDown;
      this.m_targetControl.MouseMove -= OnMouseMove;
      this.m_targetControl.KeyPress -= OnKeyPress;
    }

    private void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      OnMouseDown(sender, ConvertMouseButtons(e.Button), e.X, e.Y);
    }
    private void OnMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      OnMouseMove(sender, ConvertMouseButtons(e.Button), e.X, e.Y);
    }
    private void OnKeyPress(object sender, KeyPressEventArgs e)
    {
      OnKeyPress(sender, e.KeyChar);
    }

    private OdExMouseButton ConvertMouseButtons(MouseButtons button)
    {
      switch(button)
      {
        case MouseButtons.Left: return OdExMouseButton.Left;
        case MouseButtons.Middle: return OdExMouseButton.Middle;
        case MouseButtons.Right: return OdExMouseButton.Right;
        case MouseButtons.XButton1: return OdExMouseButton.XButton1;
        case MouseButtons.XButton2: return OdExMouseButton.XButton2;
        case MouseButtons.None:
        default: return OdExMouseButton.None;
      }
    }
  }

  public abstract class OdExMouseDragerWpfBase : OdExMouseDragerBase
  {
    protected System.Windows.Controls.UserControl m_targetControl;

    public OdExMouseDragerWpfBase(System.Windows.Controls.UserControl targetControl, OdGsView targetView) : base(targetView)
    {
      m_targetControl = targetControl;
    }

    protected void Init()
    {
      this.m_targetControl.MouseDown += OnMouseDown;
      this.m_targetControl.MouseMove += OnMouseMove;
      this.m_targetControl.KeyDown += OnKeyPress;
    }


    protected void UInit()
    {
      this.m_targetControl.MouseDown -= OnMouseDown;
      this.m_targetControl.MouseMove -= OnMouseMove;
      this.m_targetControl.KeyDown -= OnKeyPress;
    }

    private void OnMouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
    {
      Point point = e.GetPosition(m_targetControl);
      OnMouseDown(sender, ConvertMouseButtons(e.ChangedButton), (int)point.X, (int)point.Y);
    }
    private void OnMouseMove(object sender, System.Windows.Input.MouseEventArgs e)
    {
      Point point = e.GetPosition(m_targetControl);
      OnMouseMove(sender, ConvertMouseButtons(e), (int)point.X, (int)point.Y);
    }
    private void OnKeyPress(object sender, System.Windows.Input.KeyEventArgs e)
    {
      char ch = e.Key.ToString()[0];
      OnKeyPress(sender, ch);
    }

    private OdExMouseButton ConvertMouseButtons(System.Windows.Input.MouseEventArgs button)
    {
      OdExMouseButton result = OdExMouseButton.None;

      if (button.LeftButton == MouseButtonState.Pressed)
        result |= OdExMouseButton.Left;
      if (button.RightButton == MouseButtonState.Pressed)
        result |= OdExMouseButton.Right;
      if (button.MiddleButton == MouseButtonState.Pressed)
        result |= OdExMouseButton.Middle;
      if (button.XButton1 == MouseButtonState.Pressed)
        result |= OdExMouseButton.XButton1;
      if (button.XButton2 == MouseButtonState.Pressed)
        result |= OdExMouseButton.XButton2;

      return result;
    }

    private OdExMouseButton ConvertMouseButtons(MouseButton button)
    {
      switch (button)
      {
        case MouseButton.Left: return OdExMouseButton.Left;
        case MouseButton.Middle: return OdExMouseButton.Middle;
        case MouseButton.Right: return OdExMouseButton.Right;
        case MouseButton.XButton1: return OdExMouseButton.XButton1;
        case MouseButton.XButton2: return OdExMouseButton.XButton2;
        default: return OdExMouseButton.None;
      }
    }
  }

  public abstract class OdExMouseDragerBase
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

    protected OdGsView m_targetView;

    public OdExMouseDragerBase(OdGsView targetView)
    {
      m_targetView = targetView;
    }

    protected void OnTextInfoChanged(String text)
    {
      if (TextInfoChanged != null)
        TextInfoChanged(this, new OdExStringEventArgs(text));
    }

    public abstract bool DoIt();
    protected abstract void OnMouseDown(object sender, OdExMouseButton buttons, int mouseX, int mouseY);
    protected abstract void OnMouseMove(object sender, OdExMouseButton buttons, int mouseX, int mouseY);
    protected abstract void OnKeyPress(object sender, char keyChar);

    public event TextChangedEventHandler TextInfoChanged;
  }
}