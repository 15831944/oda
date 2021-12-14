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
using System.Windows.Forms;
using Teigha.Core;
using OdKernelExamplesMgdLib.ExView;
using OdKernelExamplesUIMgdLib.Helpers;

namespace OdKernelExamplesUIMgdLib.ExView
{
  public abstract class OdExViewProductControlBase : Control
  {
    private OdExControlHDCHelper m_ControlHDCHelper;
    protected OdExViewProductRenderDeviceBase m_RenderDevice;

    private OdExViewZoomWinForms m_zoomFunctionality;
    private OdExViewPanWinForms m_panFunctionality;
    private OdExViewRotationWinForms m_rotationFunctionality;

    public OdExViewProductControlBase() : base()
    {
      m_ControlHDCHelper = new OdExControlHDCHelper(this);

      m_zoomFunctionality = new OdExViewZoomWinForms(this);
      m_panFunctionality = new OdExViewPanWinForms(this);
      m_rotationFunctionality = new OdExViewRotationWinForms(this);
    }

    public OdGsDCRect GetGsClientRectangle { get { return GetClientRectangleSize(); } }

    protected OdGsDevice m_GsDevice { get { return m_RenderDevice.m_GsDevice; } }

    public bool IsInitialized { get { return m_RenderDevice.IsInitialized; } }

    public virtual void Init(OdRxObject database)
    {
      if (m_RenderDevice.IsInitialized)
        throw new Exception("Render device already initialized. Please uninitialize before.");

      PreInit();

      InitRenderDevice(database);
      InitializeAdditionalFunctional(database);

      PostInit();
    }
    protected virtual void PreInit() { }
    protected virtual void PostInit() { }

    public virtual void UnInit()
    {
      if (!m_RenderDevice.IsInitialized)
        return;

      PreUnInit();

      UnInitializeAdditionalFunctional();

      UnInitRenderDevice();

      PostUnInit();
    }
    protected virtual void PreUnInit() { }
    protected virtual void PostUnInit() { }

    #region Rendering

    private void UnInitRenderDevice()
    {
      m_ControlHDCHelper.UnInit();
      m_RenderDevice.NeedClientRectangleSizeEvent -= GetClientRectangleSize;
      m_RenderDevice.PostPaintEvent -= RePaintHandler;
      m_RenderDevice.UnInitRenderDevice();
    }

    protected void InitRenderDevice(OdRxObject database)
    {
      m_ControlHDCHelper.Init();
      m_RenderDevice.NeedClientRectangleSizeEvent += GetClientRectangleSize;
      m_RenderDevice.PostPaintEvent += RePaintHandler;
      m_RenderDevice.InitRenderDevice(database, m_ControlHDCHelper.GetControlHWND(), m_ControlHDCHelper.GetControlHDC());
    }

    private OdGsDCRect GetClientRectangleSize()
    {
      return new OdGsDCRect(ClientRectangle.Left, ClientRectangle.Right, ClientRectangle.Bottom, ClientRectangle.Top);
    }

    private void RePaintHandler()
    {
      Invalidate();
    }

    //It is necessary to override this empty method, so that the default background of Control is not painted.
    protected override void OnPaintBackground(PaintEventArgs pevent)
    {
    }

    //It is necessary to override this method, so that update render view.
    protected override void OnPaint(PaintEventArgs e)
    {
      Form targetForm = this.FindForm();
      if (targetForm != null && targetForm.WindowState != FormWindowState.Minimized && this.Height > 0)
      {
        if (m_RenderDevice.IsInitialized)
        m_RenderDevice.UpdateGsDevice();
      }
    }

    protected override void OnSizeChanged(EventArgs e)
    {
      base.OnSizeChanged(e);
      Form targetForm = this.FindForm();
      if (targetForm != null && targetForm.WindowState != FormWindowState.Minimized && this.Height > 0)
      {
        if (m_RenderDevice.IsInitialized)
          m_RenderDevice.UpdateSizeGsDevice();
      }
    }
    #endregion

    #region Additional Funtionality

    protected virtual void InitializeAdditionalFunctional(OdRxObject database)
    {
      if (!m_RenderDevice.IsInitialized)
        throw new Exception("Render device not initialized yet");

      m_panFunctionality.TargetView = ActiveView();
      m_zoomFunctionality.TargetView = ActiveView();
      m_rotationFunctionality.TargetView = ActiveView();
    }

    protected abstract OdGsView ActiveView();


    protected virtual void UnInitializeAdditionalFunctional()
    {
      m_panFunctionality.TargetView = null;
      m_zoomFunctionality.TargetView = null;
      m_rotationFunctionality.TargetView = null;
    }

    public void OnZoomToExtents()
    {
      m_RenderDevice.OnZoomToExtents();
    }

    public void EnableZoom(bool state)
    {
      EnableFunctionality(m_zoomFunctionality, state);
    }

    public void EnablePan(bool state)
    {
      EnableFunctionality(m_panFunctionality, state);

    }

    public void EnableRotation(bool state)
    {
      EnableFunctionality(m_rotationFunctionality, state);
    }

    protected void EnableFunctionality(OdExViewFunctionalityInterface functionality, bool state)
    {
      if (functionality.IsEnable == state)
        return;

      functionality.IsEnable = state;
      if (state)
      {
        functionality.InitEvents();
        functionality.PostPaintEvent += RePaintHandler;
      }
      else
      {
        functionality.UnInitEvents();
        functionality.PostPaintEvent -= RePaintHandler;
      }
    }
    #endregion
  }

}