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
  /// <summary>
  /// This example is base class some any render device
  /// Contains base operations which need, so that render some any database
  /// </summary>
  public abstract class OdExViewRenderDeviceBase
  {
    /// <summary>
    /// Target database
    /// </summary>
    protected OdRxObject m_Database;

    /// <summary>
    /// Render mode type
    /// </summary>
    public OdGsView.RenderMode m_RenderMode { get; set; }

    /// <summary>
    /// If render device initialized method return true; otherwise false;
    /// </summary>
    public bool IsInitialized { get { return m_GsDevice != null; } }

    /// <summary>
    /// Get initialized OdGsDevice
    /// </summary>
    public OdGsDevice m_GsDevice { get; private set; }

    public OdExViewRenderDeviceBase()
    {
      m_RenderMode = OdGsView.RenderMode.kFlatShaded;
    }

    /// <summary>
    /// Method initialize render device for database
    /// </summary>
    /// <param name="database"></param>
    public virtual void InitRenderDevice(OdRxObject database)
    {
      m_Database = database;
      try
      {
        m_GsDevice = Create_GsDeviceFromGsModule();

        OdGiContext giContext = Create_GiContext();

        m_GsDevice = CreateGsDevice(m_GsDevice, giContext);

        SetupColorPalette();

        //Initial set Size
        UpdateSizeGsDevice();

        //Initial update
        UpdateGsDevice();
      }
      catch (Exception ex)
      {
        UnInitRenderDevice();
        throw new Exception("Error Initialize Rendering Device");
      }
    }

    /// <summary>
    /// Method uninitialize render device
    /// </summary>
    public virtual void UnInitRenderDevice()
    {
      m_GsDevice = null;
    }

    /// <summary>
    /// Update OdGsDevice
    /// </summary>
    public void UpdateGsDevice()
    {
      if (m_GsDevice == null)
        return;

      m_GsDevice.update();
    }

    protected virtual void CallPostPaint()
    {
      if (PostPaintEvent == null)
        throw new Exception("Event PostPaintEvent not exist handler. Please provide handler");

      PostPaintEvent();
    }

    protected virtual void CallPrePaint()
    {
      if (PrePaintEvent == null)
        return;

      PrePaintEvent();
    }

    /// <summary>
    /// Method update size OdGsDevice & call RePaintEvent
    /// </summary>
    public virtual void UpdateSizeGsDevice()
    {
      CallPrePaint();

      if (NeedClientRectangleSizeEvent == null)
        throw new Exception("Event NeedClientRectangleSizeEvent not exist handler. Please provide handler");
      OdGsDCRect gsRect = NeedClientRectangleSizeEvent();

      if (m_GsDevice == null)
        return;

      // Define a device coordinate rectangle equal to the client rectangle
      m_GsDevice.onSize(gsRect);

      UpdateGsDevice();

      CallPostPaint();
    }

    /// <summary>
    /// Default setup color palette to OdGsDevice
    /// </summary>
    protected virtual void SetupColorPalette()
    {
      if (m_GsDevice == null)
        throw new Exception("OdGsDevice not Created");
      // Set the device background color and palette
      uint[] CurPalette = Teigha.Core.AllPalettes.getLightPalette();
      m_GsDevice.setBackgroundColor(CurPalette[0]);
      m_GsDevice.setLogicalPalette(CurPalette, 256);
    }

    /// <summary>
    /// Creation OdGsDevice is according to specific ODA SDK graphics module
    /// </summary>
    /// <returns></returns>
    protected abstract OdGsDevice Create_GsDeviceFromGsModule();

    /// <summary>
    /// Creation graphic context is according to specific database
    /// </summary>
    /// <returns></returns>
    protected abstract OdGiContext Create_GiContext();

    /// <summary>
    /// Creation specific graphics layout helper is according to specific target database
    /// </summary>
    /// <param name="gsDevice"></param>
    /// <param name="giContext"></param>
    /// <returns></returns>
    protected abstract OdGsDevice CreateGsDevice(OdGsDevice gsDevice, OdGiContext giContext);

    /// <summary>
    /// Setup render mode is according to specific target database
    /// </summary>
    protected abstract void SetupRenderMode();

    public delegate void PostPaintDelegate();
    /// <summary>
    /// Event after need update/invalidate/refresh target control
    /// </summary>
    public event PostPaintDelegate PostPaintEvent;

    public delegate void PrePaintDelegate();
    /// <summary>
    /// Event before need update/invalidate/refresh target control
    /// </summary>
    public event PrePaintDelegate PrePaintEvent;

    public delegate OdGsDCRect NeedClientRectangleSizeDelegate();
    /// <summary>
    /// Event requests actual render size target control
    /// </summary>
    public event NeedClientRectangleSizeDelegate NeedClientRectangleSizeEvent;

  }
}