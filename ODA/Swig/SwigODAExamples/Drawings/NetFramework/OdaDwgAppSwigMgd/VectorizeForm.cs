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
using System.Drawing;
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TD;
using OdDrawingsExamplesMgdLib;
using OdDrawingsExamplesMgdLib.ExSelection;
using OdDrawingsExamplesUIMgdLib.ExView;
using OdKernelExamplesUIMgdLib.ExView;
using OdKernelExamplesMgdLib;

namespace OdaDwgAppSwigMgd
{
  public partial class VectorizeForm : Form
  {
    private OdExViewDrawingControl viewCtrl;
    private OdExSubEntitySelection subEntitySelectFunctionality;

    public VectorizeForm(OdDbDatabase db)
    {
      Database = db;
      InitializeComponent();

      viewCtrl = new OdExViewDrawingControl();
      viewCtrl.Dock = DockStyle.Fill;
      this.Controls.Add(viewCtrl);

      viewCtrl.MouseClick += ViewCtrl_MouseClick;

      subEntitySelectFunctionality = new OdExSubEntitySelection();
    }

    public void EnableZoom(bool state)
    {
      viewCtrl.EnableZoom(state);
    }

    public void EnablePan(bool state)
    {
      viewCtrl.EnablePan(state);
    }

    public void EnableRotation(bool state)
    {
      viewCtrl.EnableRotation(state);
    }

    public void EnableMouseEditor(bool state)
    {
      viewCtrl.EnableMouseEditor(state);
    }

    private void UnInitViewControl()
    {

    }

    public OdDbDatabase Database { get; private set; }

    public OdGsLayoutHelper GetGsLayoutHelper { get { return viewCtrl.GsDevice; } }

    public OdExViewDrawingControl GetViewControl { get { return viewCtrl; } }

    #region SubEntitySelection

    private void OnSelectSubEntity(int xMouse, int yMouse)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OdGsView pView = viewCtrl.GsDevice.activeView();

        subEntitySelectFunctionality.IsEnable = true;
        subEntitySelectFunctionality.TargetDatabase = Database;
        subEntitySelectFunctionality.TargetView = pView;

        OdDbObjectId objId = subEntitySelectFunctionality.OnSubSelection(xMouse, yMouse);

        HideSubEntityInfo();

        if (!objId.IsEqual(OdDbObjectId.kNull))
          ShowSubEntityInfo(Cursor.Position, objId);
      }
      catch (Exception err)
      {
        throw err;
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }
    }

    public void ShowSubEntityInfo(Point showLocation, OdDbObjectId objId)
    {
      String handle = objId.getHandle().ascii();
      String className = objId.openObject().isA().name();
      Text = String.Concat("Selected SubEntity -> Handle: ", handle, "  ClassName:", className);
    }

    public void HideSubEntityInfo()
    {
      Text = String.Empty;
    }
    #endregion SubEntitySelection

    #region ZoomToExtents
    public void OnZoomToExtents()
    {
      viewCtrl.OnZoomToExtents();
    }
    #endregion

    #region Init/UnInit Form

    private void VectorizeForm_Load(object sender, EventArgs e)
    {
      Init();
    }

    private void VectorizeForm_FormClosed(object sender, FormClosedEventArgs e)
    {
      UnInit();
    }

    private void Init()
    {
      try
      {
        InitFormTransaction();

        viewCtrl.Init(Database);

        bool replaceColorDuringRendering = true;
        if (replaceColorDuringRendering)
          OdExModifyDrawnColors.attachColorConverter(viewCtrl.GsDevice);

      }
      catch (Exception ex)
      {
        UnInit();
        MessageBox.Show(ex.Message, "Error Initialize Vectorize Form");
      }
    }

    private void UnInit()
    {
      viewCtrl.UnInit();

      UnInitFormTransaction();
    }

    #endregion

    #region FormTransaction
    MemoryTransaction manTr = null;
    private void InitFormTransaction()
    {
      manTr = MemoryManager.GetMemoryManager().StartTransaction();
    }

    private void UnInitFormTransaction()
    {
      if (manTr != null)
        MemoryManager.GetMemoryManager().StopTransaction(manTr);
    }
    #endregion

    private void ViewCtrl_MouseClick(object sender, MouseEventArgs e)
    {
      if (e.Button == MouseButtons.Left)
      {
        OnSelectSubEntity(e.X, e.Y);
      }
    }

    public void InvertColorPalette()
    {
      OdExAuxUtilities.InvertColorPalette(viewCtrl.GsDevice);
    }

    public void SnapShot()
    {
      OnSnapShot();
    }

    private void OnSnapShot()
    {
      OdGsLayoutHelper _hDevice = viewCtrl.GsDevice;

      OdGsDCRect gsRect = viewCtrl.GetGsClientRectangle;

      uint imgPixelWidth = 1024;
      uint imgPixelHeight = 768;

      SaveFileDialog saveImageDialog = new SaveFileDialog();
      saveImageDialog.Filter = "Bmp files|*.bmp";
      if (saveImageDialog.ShowDialog() != DialogResult.OK)
        return;

      String saveFilePath = saveImageDialog.FileName;

      OdExAuxUtilities.DoSnapShot(_hDevice, gsRect, imgPixelWidth, imgPixelHeight, saveFilePath);
    }
  }
}
