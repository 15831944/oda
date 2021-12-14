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
using OdDrawingsExamplesUIMgdLib.ExView;
using System.Windows.Forms.Integration;

namespace OdExWpfSampleWinFormsHost
{
  public partial class OdExViewWpfControlWinFormsHost : UserControl
  {
    /// <summary>
    /// Current database
    /// </summary>
    private OdDbDatabase m_database;

    /// <summary>
    /// WinForms control for rendering
    /// </summary>
    private OdExViewDrawingControl m_renderControl;

    public OdGsLayoutHelper m_GsLayoutHelper { get { return m_renderControl.GsDevice; } }

    public OdExViewDrawingControl RenderControl { get { return m_renderControl; } }

    public OdExViewWpfControlWinFormsHost()
    {
      InitializeComponent();

      m_renderControl = new OdExViewDrawingControl();

      WindowsFormsHost winFormsHost = new WindowsFormsHost();
      winFormsHost.Child = m_renderControl;
      winFormsHost.VerticalAlignment = VerticalAlignment.Stretch;
      winFormsHost.HorizontalAlignment = HorizontalAlignment.Stretch;
      this.RenderArea.Children.Add(winFormsHost);
    }

    /// <summary>
    /// Method enable/disable zoom by mouse wheel 
    /// </summary>
    /// <param name="state"></param>
    public void EnableZoom(bool state)
    {
      m_renderControl.EnableZoom(state);
    }

    /// <summary>
    /// Method enable/disable pan by mouse
    /// </summary>
    /// <param name="state"></param>
    public void EnablePan(bool state)
    {
      m_renderControl.EnablePan(state);
    }

    /// <summary>
    /// Method enable/disable rotation by mouse
    /// </summary>
    /// <param name="state"></param>
    public void EnableRotation(bool state)
    {
      m_renderControl.EnableRotation(state);
    }

    /// <summary>
    /// Method enable/disable editor by mouse (selection, grip points, and etc)
    /// </summary>
    /// <param name="state"></param>
    public void EnableMouseEditor(bool state)
    {
      m_renderControl.EnableMouseEditor(state);
    }


    /// <summary>
    /// Is rendering already initialized, so that method return true; otherwise false;
    /// </summary>
    /// <returns></returns>
    public bool IsRenderingInitialized()
    {
      if (m_renderControl == null)
        return false;
      return m_renderControl.IsInitialized;
    }

    /// <summary>
    /// Method do zoom to extents
    /// </summary>
    public void ZoomToExtents()
    {
      if (!m_renderControl.IsInitialized)
        return;

      m_renderControl.OnZoomToExtents();
    }

    /// <summary>
    /// Method initialize rendering. After initialization will be rendering database of OdDbDatabase type
    /// </summary>
    /// <param name="database"></param>
    public void Init(OdDbDatabase database)
    {
      m_database = database;
      m_renderControl.Init(database);
    }


    /// <summary>
    /// Method reinitialize rendering for current database
    /// </summary>
    public void ReInit()
    {
      ReInit(m_database);
    }

    /// <summary>
    /// Method reinitialize rendering for new database
    /// </summary>
    /// <param name="database"></param>
    public void ReInit(OdDbDatabase database)
    {
      UnInit();
      Init(database);
    }

    /// <summary>
    /// Method uninitialize rendering
    /// </summary>
    public void UnInit()
    {
      m_renderControl.UnInit();
    }
  }
}
