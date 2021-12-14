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
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Teigha.Core;
using Teigha.TD;
using OdDrawingsExamplesMgdLib.ExView;
using OdKernelExamplesMgdLib.ExView;
using OdKernelExamplesUIMgdLib.ExView;
using OdDrawingsExamplesMgdLib.ExEditors;
using Microsoft.Win32;
using System.Windows.Threading;

namespace OdExWpfSampleDirectX
{
  /// <summary>
  /// This UserControl render only database of OdDbDatabase type.
  /// Rendering model done by means of DirectX only, as image source of type D3DImage
  /// </summary>
  public partial class OdExViewDirectXWpfControl : UserControl
  {
    /// <summary>
    /// Current database
    /// </summary>
    private OdDbDatabase m_database;

    /// <summary>
    /// Example render device for database of OdDbDatabase type, which have base functionality for rendering.
    /// </summary>
    private OdExViewDrawingRenderDevice m_RenderDevice;

    /// <summary>
    /// Example D3DImage on which do rendering D3DSurface
    /// </summary>
    private OdExRenderD3DImage m_D3DImage;

    /// <summary>
    /// Example Pan functionality
    /// </summary>
    private OdExViewPanWpf m_panFunctionality;

    /// <summary>
    /// Example Zoom functionality
    /// </summary>
    private OdExViewZoomWpf m_zoomFunctionality;

    /// <summary>
    /// Example Rotation functionality
    /// </summary>
    private OdExViewRotationWpf m_rotationFunctionality;

    /// <summary>
    /// Example Editor functionality
    /// </summary>
    private OdExDrawingEditorWpf m_mouseEditorFunctionality;

    public OdGsLayoutHelper m_GsLayoutHelper { get { return OdGsLayoutHelper.cast(m_RenderDevice.m_GsDevice); } }

    public OdExViewDirectXWpfControl()
    {
      InitializeComponent();
      m_RenderDevice = new OdExViewDrawingRenderDevice(OdExGsDeviceCreator.OdExGsModuleType.WinDirectX);
      m_D3DImage = new OdExRenderD3DImage();

      this.SizeChanged += SizeChangedHandler;

      renderImage.Source = GetOdaLogo();
      m_panFunctionality = new OdExViewPanWpf(this);
      m_zoomFunctionality = new OdExViewZoomWpf(this);
      m_rotationFunctionality = new OdExViewRotationWpf(this);
      m_mouseEditorFunctionality = new OdExDrawingEditorWpf(this);

      SystemEvents.DisplaySettingsChanging += SystemEvents_DisplaySettingsChanged;
    }

    /// <summary>
    /// If display properties was been changed, that need reinitialize rendering
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void SystemEvents_DisplaySettingsChanged(object sender, EventArgs e)
    {
      if (!IsInitialized)
        return;

      EventHandler reInitializeHandler = new EventHandler((object s, EventArgs ea) =>
      {
        (s as DispatcherTimer).Stop();
        ReInit();
      });

      DispatcherTimer timer = new DispatcherTimer(
        new TimeSpan(10000),
        DispatcherPriority.ApplicationIdle,
        reInitializeHandler,
        this.Dispatcher
        );

      timer.Start();
    }

    /// <summary>
    /// Method enable/disable zoom by mouse wheel 
    /// </summary>
    /// <param name="state"></param>
    public void EnableZoom(bool state)
    {
      EnableFunctionality(m_zoomFunctionality, state);
    }

    /// <summary>
    /// Method enable/disable pan by mouse
    /// </summary>
    /// <param name="state"></param>
    public void EnablePan(bool state)
    {
      EnableFunctionality(m_panFunctionality, state);
    }

    /// <summary>
    /// Method enable/disable rotation by mouse
    /// </summary>
    /// <param name="state"></param>
    public void EnableRotation(bool state)
    {
      EnableFunctionality(m_rotationFunctionality, state);
    }

    /// <summary>
    /// Method enable/disable editor by mouse (selection, grip points, and etc)
    /// </summary>
    /// <param name="state"></param>
    public void EnableMouseEditor(bool state)
    {
      EnableFunctionality(m_mouseEditorFunctionality, state);
    }


    /// <summary>
    /// Is rendering already initialized, so that method return true; otherwise false;
    /// </summary>
    /// <returns></returns>
    public bool IsRenderingInitialized()
    {
      if (m_RenderDevice == null)
        return false;
      return m_RenderDevice.IsInitialized;
    }

    /// <summary>
    /// Method release surface object which use in D3DImage.
    /// Before some any action with OdGsDevice, need release surface on Image
    /// </summary>
    public void OnReleaseImageSurface()
    {
      Application.Current.Dispatcher.Invoke((Action)m_D3DImage.ReleaseSurface);
    }

    /// <summary>
    /// Method update surface object which use in D3DImage.
    /// After some any action with OdGsView, need update surface on Image
    /// This method need execute after OnReleaseImageSurface() method
    /// </summary>
    public void OnUpdateImageSurface()
    {
      Application.Current.Dispatcher.Invoke((Action)m_D3DImage.UpdateSurface);
    }

    /// <summary>
    /// Method do zoom to extents
    /// </summary>
    public void ZoomToExtents()
    {
      if (!m_RenderDevice.IsInitialized)
        return;

      m_RenderDevice.OnZoomToExtents();
    }

    /// <summary>
    /// Method initialize rendering. After initialization will be rendering database of OdDbDatabase type
    /// </summary>
    /// <param name="database"></param>
    public void Init(OdDbDatabase database)
    {
      if (m_RenderDevice.IsInitialized)
        throw new Exception("Render device already initialized. Please uninitialize before.");
      m_database = database;
      InitRenderDevice(m_database);
      InitAddittionalFunctionality(m_database);
    }

    /// <summary>
    /// Method uninitialize rendering
    /// </summary>
    public void UnInit()
    {
      if (!m_RenderDevice.IsInitialized)
        return;
      UnInitializeAdditionalFunctional();
      UnInitRenderDevice();
    }

    /// <summary>
    /// Method reinitialize rendering
    /// </summary>
    private void ReInit()
    {
      //Uninitialize rendering
      UnInit();

      //Create new instance D3DImage, because DirectX device may changed
      m_D3DImage = new OdExRenderD3DImage();

      //Initialize rendering
      Init(m_database);
    }

    #region AddtitionalFuntionality 
    private void EnableFunctionality(OdExViewFunctionalityInterface functionality, bool state)
    {
      if (functionality.IsEnable == state)
        return;

      functionality.IsEnable = state;
      if (state)
      {
        functionality.InitEvents();
        functionality.PostPaintEvent += PostPaintEventHandler;
        functionality.PrePaintEvent += PrePaintEventHandler;
      }
      else
      {
        functionality.UnInitEvents();
        functionality.PostPaintEvent -= PostPaintEventHandler;
        functionality.PrePaintEvent -= PrePaintEventHandler;
      }
    }

    private void InitAddittionalFunctionality(OdDbDatabase database)
    {
      if (!m_RenderDevice.IsInitialized)
        throw new Exception("m_RenderDevice not initialized yet");
      m_panFunctionality.TargetView = m_GsLayoutHelper.activeView();
      m_zoomFunctionality.TargetView = m_GsLayoutHelper.activeView();
      m_rotationFunctionality.TargetView = m_GsLayoutHelper.activeView();
      m_mouseEditorFunctionality.Init(database, m_GsLayoutHelper);
    }

    private void UnInitializeAdditionalFunctional()
    {
      m_panFunctionality.TargetView = null;
      m_zoomFunctionality.TargetView = null;
      m_rotationFunctionality.TargetView = null;
      m_mouseEditorFunctionality.UnInit();
    }
    #endregion

    private void InitRenderDevice(OdDbDatabase database)
    {
      try
      {
        //Must Initialize handlers for events
        m_RenderDevice.NeedClientRectangleSizeEvent += GetClientRectangleSizeEventHandler;
        m_RenderDevice.PrePaintEvent += PrePaintEventHandler;
        m_RenderDevice.PostPaintEvent += PostPaintEventHandler;

        //Need create hidden window, so that OdGsDevice render model on window
        HwndSource renderWindow = GetHWND_FromHiddenWindowForRendering();

        //Initialize Render Device
        m_RenderDevice.InitRenderDevice(database, renderWindow.Handle, IntPtr.Zero);

        //Set Target OdGsDevice
        m_D3DImage.Initialize(m_RenderDevice.m_GsDevice);

        //Initial update size;
        m_RenderDevice.UpdateSizeGsDevice();

        //Set ImageSource
        renderImage.Source = m_D3DImage;
      }
      catch (Exception err)
      {
        MessageBox.Show(err.Message, "Error");
        renderImage.Source = GetOdaLogo();
      }
    }

    private void UnInitRenderDevice()
    {
      try
      {
        //Must uninitialize handlers for events
        m_RenderDevice.NeedClientRectangleSizeEvent -= GetClientRectangleSizeEventHandler;
        m_RenderDevice.PrePaintEvent -= PrePaintEventHandler;
        m_RenderDevice.PostPaintEvent -= PostPaintEventHandler;

        //Uninitialize Render Device
        m_RenderDevice.UnInitRenderDevice();

        //Uninitialize D3DSurface Image data
        m_D3DImage.UnInitializeSurface();

      }
      catch (Exception err)
      {
        MessageBox.Show(err.Message, "Error");
      }
      finally
      {
        renderImage.Source = GetOdaLogo();
      }

    }

    private void PrePaintEventHandler()
    {
      OnReleaseImageSurface();
    }

    private void PostPaintEventHandler()
    {
      OnPostPaintUpdate();
    }

    public void OnPostPaintUpdate()
    {
      Application.Current.Dispatcher.Invoke((Action)m_RenderDevice.UpdateGsDevice);

      OnUpdateImageSurface();
    }

    public void OnPrePaintUpdate()
    {
      OnReleaseImageSurface();
    }

    private OdGsDCRect GetClientRectangleSizeEventHandler()
    {
      Size rect = this.RenderSize;
      int Left = (int)0;
      int Right = (int)rect.Width;
      int Top = (int)0;
      int Bottom = (int)rect.Height;
      return new OdGsDCRect(Left, Right, Bottom, Top);
    }

    /// <summary>
    /// This hidden window is need, so that OdGsDevice can do rendering 
    /// But, we use result image as D3DSurface, from OdGsDevice
    /// </summary>
    private Window m_HiddenWindowForRendering;
    private HwndSource GetHWND_FromHiddenWindowForRendering()
    {
      Window hiddenWindow = GetHideWindowForRendering();
      return HwndSource.FromVisual(m_HiddenWindowForRendering) as HwndSource;
    }

    private Window GetHideWindowForRendering()
    {
      if (m_HiddenWindowForRendering != null)
        return m_HiddenWindowForRendering;
      m_HiddenWindowForRendering = new Window();
      m_HiddenWindowForRendering.Owner = Application.Current.MainWindow;
      m_HiddenWindowForRendering.Visibility = System.Windows.Visibility.Hidden;
      m_HiddenWindowForRendering.WindowStyle = System.Windows.WindowStyle.None;
      m_HiddenWindowForRendering.ShowInTaskbar = false;
      m_HiddenWindowForRendering.Background = Brushes.Transparent;
      m_HiddenWindowForRendering.AllowsTransparency = true;
      m_HiddenWindowForRendering.Show();
      m_HiddenWindowForRendering.Hide();
      return m_HiddenWindowForRendering;
    }

    /// <summary>
    /// If control size changed, need update size on OdGsDevice
    /// Note: Before update OdGsDevice need release D3DSurface
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void SizeChangedHandler(object sender, SizeChangedEventArgs e)
    {
      if (m_RenderDevice.IsInitialized)
      {
        m_RenderDevice.UpdateSizeGsDevice();
      }
    }

    private static BitmapSource GetOdaLogo()
    {
      BitmapSource result;
      try
      {
        System.Drawing.Bitmap source = OdExWpfSampleDirectX.Resources.oda_logo;

        result = System.Windows.Interop.Imaging.CreateBitmapSourceFromHBitmap(
          source.GetHbitmap(),
          IntPtr.Zero,
          System.Windows.Int32Rect.Empty,
          BitmapSizeOptions.FromWidthAndHeight(source.Width, source.Height));
      }
      catch (Exception err)
      {
        result = null;
      }

      return result;
    }

  }
}
