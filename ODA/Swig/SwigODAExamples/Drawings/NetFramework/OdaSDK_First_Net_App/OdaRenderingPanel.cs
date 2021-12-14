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
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TD;

namespace OdaSdk_First_NET_App
{
  public partial class OdaRenderingPanel : UserControl
  {
    public OdaRenderingPanel()
    {
      InitializeComponent();
      this.SizeChanged += OdaRenderingPanel_SizeChanged;
    }

    private OdGsLayoutHelper _hDevice;

    public void SetDatabase(OdDbDatabase db)
    {
      if (_hDevice != null)
        UnInitRenderDevice();
      InitRenderDevice(db);
    }

    public void ResetControl()
    {
      UnInitRenderDevice();
    }

    private void InitRenderDevice(OdDbDatabase db)
    {
      try
      {
        CreateFormGraphics();

        OdGsDevice pDevice = CreateGsDevice(GetHWND(), GetHDC());

        OdGiContextForDbDatabase ctx = CreateContextForDbDatabase(db);

        _hDevice = CreateLayoutHelper(pDevice, ctx);

        // Define a device coordinate rectangle equal to the client rectangle
        OdGsDCRect gsRect = new OdGsDCRect(ClientRectangle.Left, ClientRectangle.Right, ClientRectangle.Bottom, ClientRectangle.Top);

        //Initial set Size
        _hDevice.onSize(gsRect);

        //Initial update
        _hDevice.update();

        //Do ZoomToExtents
        OdDbObject pView = db.activeViewportId().openObject();
        OdDbAbstractViewportData pViewPE = OdDbAbstractViewportData.cast(pView);
        pViewPE.zoomExtents(pView);
      }
      catch (Exception ex)
      {
        UnInitRenderDevice();
        MessageBox.Show(ex.Message, "Error Initialize Rendering Control");
      }
    }

    private void UnInitRenderDevice()
    {
      ReleaseHDC();
      _hDevice = null;
    }

    protected override void OnPaint(PaintEventArgs e)
    {
      if (_hDevice == null)
        return;

      if (Form.ActiveForm.WindowState != FormWindowState.Minimized)
      {
        _hDevice.update();
      }
    }

    private void OdaRenderingPanel_SizeChanged(object sender, EventArgs e)
    {
      if (_hDevice == null)
        return;
      _hDevice.onSize(new OdGsDCRect(new OdGsDCPoint(ClientRectangle.Left, ClientRectangle.Bottom), new OdGsDCPoint(ClientRectangle.Right, ClientRectangle.Top)));
      Invalidate();
    }

    #region Graphics
    private Graphics _graphics = null;
    private void CreateFormGraphics()
    {
      _graphics = Graphics.FromHwnd(GetHWND());
    }

    private IntPtr GetHWND()
    {
      return Handle;
    }

    bool IsHdcUsed;
    private IntPtr GetHDC()
    {
      if (IsHdcUsed || _graphics == null)
        return IntPtr.Zero;
      IsHdcUsed = true;
      return _graphics.GetHdc();
    }

    private void ReleaseHDC()
    {
      if (!IsHdcUsed || _graphics == null)
        return;
      _graphics.ReleaseHdc();
      IsHdcUsed = false;
    }
    #endregion

    #region Helpers
    private static OdGiContextForDbDatabase CreateContextForDbDatabase(OdDbDatabase db)
    {
      OdGiContextForDbDatabase ctx = OdGiContextForDbDatabase.createObject();

      ctx.enableGsModel(true);

      ctx.setDatabase(db);

      return ctx;
    }

    private static OdGsDevice CreateGsDevice(IntPtr HWND, IntPtr HDC)
    {
      // Load the vectorization module
      OdGsModule pGs = (OdGsModule)Globals.odrxDynamicLinker().loadModule("WinGLES2.txv", false);
      // Create a new OdGsDevice object, and associate with the vectorization GsDevice
      OdGsDevice pDevice = pGs.createDevice();
      // Return a pointer to the dictionary entity containing the device properties
      OdRxDictionary Properties = pDevice.properties();

      if (Properties != null)
      {
        if (Properties.has("WindowHWND")) // Check if property is supported
          Properties.putAt("WindowHWND", new OdRxVariantValue(HWND));
        if (Properties.has("WindowHDC")) // Check if property is supported
          Properties.putAt("WindowHDC", new OdRxVariantValue(HDC)); // hWindowDC necessary for Bitmap device
        if (Properties.has("DoubleBufferEnabled")) // Check if property is supported
          Properties.putAt("DoubleBufferEnabled", new OdRxVariantValue(true));
        if (Properties.has("DiscardBackFaces")) // Check if property is supported
          Properties.putAt("DiscardBackFaces", new OdRxVariantValue(true));
        // Enable scene graph usage onto renderer side
        if (Properties.has("UseSceneGraph")) // Check if property is supported
          Properties.putAt("UseSceneGraph", new OdRxVariantValue(true));
        if (Properties.has("UseVisualStyles")) // Check if property is supported
          Properties.putAt("UseVisualStyles", new OdRxVariantValue(true));
      }

      return pDevice;
    }

    private static OdGsLayoutHelper CreateLayoutHelper(OdGsDevice pDevice, OdGiContextForDbDatabase ctx)
    {
      OdGsLayoutHelper hDevice = TD_Db.setupActiveLayoutViews(pDevice, ctx);
      OdDbDatabase db = ctx.getDatabase();

      // Set the device background color and palette
      uint[] CurPalette = Teigha.Core.AllPalettes.getLightPalette();
      hDevice.setBackgroundColor(CurPalette[0]);
      hDevice.setLogicalPalette(CurPalette, 256);

      // Return true if and only the current layout is a paper space layout
      bool bModelSpace = db.getTILEMODE();

      OdGsView.RenderMode renderMode = OdGsView.RenderMode.kFlatShaded;

      //Set render mode
      hDevice.activeView().setMode(renderMode);

      if (pDevice.properties().has("UseVisualStyles"))
      {
        OdDbDictionary dic = db.getVisualStyleDictionaryId().safeOpenObject() as OdDbDictionary;
        hDevice.activeView().setVisualStyle(dic.getAt(renderModeToVisualStyle(renderMode)).AsOdDbStubPointer());
      }

      return hDevice;
    }

    static string renderModeToVisualStyle(OdGsView.RenderMode rm)
    {
      // @@@TODO: create ODA-specific "FlatShaded" and "FlatShadedWithEdges" visual styles based onto "Shaded" and "ShadedWithEdges" visual styles.
      switch (rm)
      {
        case OdGsView.RenderMode.kWireframe: return "Wireframe";
        case OdGsView.RenderMode.kHiddenLine: return "Hidden";
        case OdGsView.RenderMode.kFlatShaded: return "Shaded"; // unfortunately there is no direct comparison with any correctly configured VS's. // OdDb::kszVSFlat;
        case OdGsView.RenderMode.kGouraudShaded: return "kszVSShaded";
        case OdGsView.RenderMode.kFlatShadedWithWireframe: return "kszVSShadedWithEdges"; // unfortunately there is no direct comparison with any correctly configured VS's. // OdDb::kszVSFlatWithEdges;
        case OdGsView.RenderMode.kGouraudShadedWithWireframe: return "kszVSShadedWithEdges";
        default: return "2dWireframe";
      }
    }
    #endregion

  }
}
