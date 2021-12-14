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

using OdKernelExamplesMgdLib.ExView;
using System;
using Teigha.Core;

namespace OdKernelExamplesMgdLib.ExView
{
  public abstract class OdExViewProductRenderDeviceBase : OdExViewRenderDeviceBase
  {
    /// <summary>
    /// Graphics module type which use for rendering
    /// </summary>
    private OdExGsDeviceCreator.OdExGsModuleType m_gsModuleType;

    /// <summary>
    /// HWND pointer to render
    /// </summary>
    private IntPtr m_HWND;

    /// <summary>
    /// HDC pointer to render
    /// </summary>
    private IntPtr m_HDC;

    /// <summary>
    /// Creation instance with specific graphics module type
    /// </summary>
    /// <param name="gsModule"></param>
    public OdExViewProductRenderDeviceBase(OdExGsDeviceCreator.OdExGsModuleType gsModule) : base()
    {
      m_gsModuleType = gsModule;
      m_HWND = IntPtr.Zero;
      m_HDC = IntPtr.Zero;
    }

    /// <summary>
    /// Method initialize render device for database
    /// </summary>
    /// <param name="database">Target database</param>
    /// <param name="hwnd">HWND pointer window/control</param>
    /// <param name="hdc">Parameters use for WinForms controls</param>
    public new void InitRenderDevice(OdRxObject database, IntPtr hwnd, IntPtr hdc /*= IntPtr.Zero*/)
    {
      m_HWND = hwnd;
      m_HDC = hdc;
      base.InitRenderDevice(database);
    }

    /// <summary>
    /// Creation OdGsDevice is according to specific ODA SDK graphics module in m_gsModuleType variable
    /// </summary>
    /// <returns></returns>
    protected override OdGsDevice Create_GsDeviceFromGsModule()
    {
      OdGsDevice gsDevice = OdExGsDeviceCreator.CreateGsDeviceFromGsModule(m_gsModuleType, m_HWND, m_HDC);
      return gsDevice;
    }

    /// <summary>
    /// Setup render mode is according to database of OdDbDatabase type
    /// </summary>
    protected override void SetupRenderMode()
    {
      OdGsView view = ActiveView();
      view.setMode(m_RenderMode);
    }

    /// <summary>
    /// Do zoom to extents
    /// </summary>
    public void OnZoomToExtents()
    {
      CallPrePaint();

      OnZoomToExtents(m_Database);

      CallPostPaint();
    }

    protected abstract void OnZoomToExtents(OdRxObject database);

    protected abstract OdGsView ActiveView();
  }
}