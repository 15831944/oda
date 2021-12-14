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

using OdKernelExamplesMgdLib;
using OdKernelExamplesUIMgdLib.Helpers;
using System;
using System.Windows.Forms;

namespace OdDrawingsExamplesUIMgdLib.ExJigs.WinForms
{
  public abstract class OdExJigWinForms
  {
    protected Control m_targetControl;
    protected OdExMouseDragerBase m_jig;

    public OdExJigWinForms(Control targetControl, OdExMouseDragerBase jig)
    {
      m_targetControl = targetControl;
      m_jig = jig;
      setupJig();
    }

    public OdExMouseDragerBase Jig { get { return m_jig; } }

    protected virtual void setupJig()
    {
      if (m_jig == null)
        throw new Exception("variable m_jig not initialized");

      m_jig.InitDragerEvent += Init;
      m_jig.UnInitDragerEvent += UnInit;
      m_jig.PostPaintEvent += PostPaint;
      m_jig.ApplicationDoProcessingEvent += ApplicationDoProcessingHandler;
    }

    protected abstract void Init();
    protected abstract void UnInit();

    protected virtual void ApplicationDoProcessingHandler()
    {
      //Application do events
      Application.DoEvents();
    }
    protected virtual void PostPaint()
    {
      m_targetControl.Invalidate();
    }

    protected virtual void OnMouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      m_jig.OnMouseDown(sender, OdExWinFormsMouseHelper.ConvertMouseButtons(e.Button), e.X, e.Y);
    }
    protected virtual void OnMouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
    {
      m_jig.OnMouseMove(sender, OdExWinFormsMouseHelper.ConvertMouseButtons(e.Button), e.X, e.Y);
    }
    protected virtual void OnKeyPress(object sender, KeyPressEventArgs e)
    {
      m_jig.OnKeyPress(sender, e.KeyChar);
    }
  }
}