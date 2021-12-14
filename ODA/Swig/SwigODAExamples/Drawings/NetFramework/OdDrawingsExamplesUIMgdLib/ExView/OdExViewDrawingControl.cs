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

using OdDrawingsExamplesMgdLib.ExEditors;
using OdDrawingsExamplesMgdLib.ExView;
using OdKernelExamplesUIMgdLib.ExView;
using System;
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesUIMgdLib.ExView
{
  public class OdExViewDrawingControl : OdExViewProductControlBase
  {
    private OdExDrawingEditorWinForms m_mouseEditorFunctionality;

    public OdExViewDrawingControl() : base()
    {
      m_mouseEditorFunctionality = new OdExDrawingEditorWinForms(this);
      m_RenderDevice = new OdExViewDrawingRenderDevice();
      //m_RenderDevice.m_RenderMode = OdGsView.RenderMode.k2DOptimized;
    }

    public OdGsLayoutHelper GsDevice { get { return OdGsLayoutHelper.cast(m_GsDevice); } }

    protected override OdGsView ActiveView()
    {
      return GsDevice.activeView();
    }

    public void EnableMouseEditor(bool state)
    {
      EnableFunctionality(m_mouseEditorFunctionality, state);
    }

    protected override void InitializeAdditionalFunctional(OdRxObject database)
    {
      base.InitializeAdditionalFunctional(database);

      OdDbDatabase dwgDatabase = OdDbDatabase.cast(database);
      if (dwgDatabase == null)
        throw new Exception("dwgDatabase is NULL");
      m_mouseEditorFunctionality.Init(dwgDatabase, GsDevice);
    }

    protected override void UnInitializeAdditionalFunctional()
    {
      base.UnInitializeAdditionalFunctional();

      m_mouseEditorFunctionality.UnInit();
    }

    protected override void OnMouseDoubleClick(MouseEventArgs e)
    {
      if (e.Button == MouseButtons.Middle)
      {
        m_RenderDevice.OnZoomToExtents();
      }
    }
  }

}