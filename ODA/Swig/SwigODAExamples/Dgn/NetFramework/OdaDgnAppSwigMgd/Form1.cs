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
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TG;
using OdKernelExamplesMgdLib.Export.Stl;
using OdKernelExamplesMgdLib.Export.Svg;
using OdDgnExamplesMgdLib.Export.Pdf;
using OdDgnExamplesMgdLib.Export.Stl;
using OdDgnExamplesMgdLib.Export.Dwf;
using OdDgnExamplesUIMgdLib.Export.Svg;
using OdKernelExamplesUIMgdLib.ExReactors;
using OdKernelExamplesUIMgdLib.Export.Pdf;
using OdKernelExamplesUIMgdLib.Export.Dwf;
using OdKernelExamplesUIMgdLib.Export.Stl;
using OdDgnExamplesMgdLib.ExDumper;

namespace OdaDgnAppSwigMgd
{
  public partial class Form1 : Form
  {
    private OdDgHostAppServices m_HostAppServices;
    private OdExSystemServices m_SystemServices;
    private OdDgDumper m_Dumper;
    private OdDgDatabase m_CurrentDatabase;

    public Form1()
    {
      InitializeComponent();
      EnableVectorizerToolButtons(false);
    }

    #region Init/UnInit Application
    private void Form1_Load(object sender, EventArgs e)
    {
      Init();
    }

    private void Form1_FormClosing(object sender, FormClosingEventArgs e)
    {
      UnInit();
    }

    private void Init()
    {
      InitFormTransaction();

      m_SystemServices = new OdExSystemServices();
      m_HostAppServices = new OdExDgnHostApplicationServices();

      Globals.odrxInitialize(m_SystemServices);
      OdExLoadExtensionsForm.rxInit();

      Globals.odgsInitialize();

      LoadModules();

      //Create and initialize Dgn Database dumper
      m_Dumper = new OdDgDumper();
    }

    private void LoadModules()
    {
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("TG_Db");
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("TD_Db.dll", false);
    }

    private void UnInit()
    {
      FinishFormTransaction();

      try
      {
        OdExLoadExtensionsForm.rxUninit();

        Teigha.Core.Globals.odgsUninitialize();
        Teigha.Core.Globals.odrxUninitialize();
      }
      catch (OdError err)
      {
        Console.WriteLine("Cleanup error: " + err.description());
      }
      finally
      {
        Teigha.Core.Helpers.odUninit();
      }
    }

    #endregion

    #region FormTransaction
    MemoryTransaction rootTransaction = null;
    private void InitFormTransaction()
    {
      rootTransaction = MemoryManager.GetMemoryManager().StartTransaction();
    }

    private void FinishFormTransaction()
    {
      MemoryManager.GetMemoryManager().StopTransaction(rootTransaction);
      MemoryManager.GetMemoryManager().StopAll();
    }
    #endregion

    #region Tree Model Helpers
    private void InitTreeModel(OdDgDatabase db)
    {
      this.m_CurrentDatabase = db;
      Tree t = new Tree(this.m_CurrentDatabase);
      t.FormClosed += new FormClosedEventHandler(TreeFormClosed);
      t.MdiParent = this;
      t.Show();
      t.FillTree();
    }
    private void TreeFormClosed(object sender, FormClosedEventArgs e)
    {
      UpdateMainMenu(false);
    }
    #endregion

    #region GUI Helpers
    private void UpdateMainMenu(bool flag)
    {
      toolStripSeparator1.Visible = flag;
      toolStripSeparator2.Visible = flag;
      pageSetupToolStripMenuItem.Visible = flag;
      ExportSVG.Visible = flag;
      ExportPDF.Visible = flag;
      ExportDWF.Visible = flag;
      ExportSTL.Visible = flag;
    }

    public void setStatus(string s)
    {
      this.toolStripStatusLabel1.Text = s;
    }

    public VectorizeForm createVectorizationWindow(OdDgDatabase db, OdDgElementId vectorizedViewId, OdDgElementId vectorizedModelId)
    {
      VectorizeForm vectorizer = new VectorizeForm(db, vectorizedViewId, vectorizedModelId);
      vectorizer.MdiParent = this;
      vectorizer.Activated += (object _sender, EventArgs _e) =>
      {
        EnableVectorizerToolButtons(true);
      };
      vectorizer.EnableZoom(true);
      vectorizer.Show();
      return vectorizer;
    }

    public void EnableVectorizerToolButtons(bool state)
    {
      this.btnPan.Enabled = state;
      this.btnRotation.Enabled = state;
      this.btnZoomToExtents.Enabled = state;
    }
    #endregion

    #region Form Handlers
    private void OnNew(object sender, EventArgs e)
    {
      bool bLoaded = true;
      try
      {
        InitTreeModel(m_HostAppServices.createDatabase());
      }
      catch (OdError er)
      {
        MessageBox.Show(er.Message);
        bLoaded = false;
      }

      if (bLoaded)
      {
        UpdateMainMenu(true);
        setStatus("Default Database Created.");
      }
    }

    private void OnOpen(object sender, EventArgs e)
    {
      bool bLoaded = true;
      if (openDgnDialog.ShowDialog() == DialogResult.OK)
      {
        try
        {
          InitTreeModel(m_HostAppServices.readFile(openDgnDialog.FileName));
        }
        catch (Exception ex)
        {
          bLoaded = false;
          MessageBox.Show(ex.Message);
        }

        if (bLoaded)
        {
          UpdateMainMenu(true);
          setStatus("Default Database Loaded.");
        }
      }
    }

    private void showHideToolbar(object sender, EventArgs e)
    {
      if (this.toolbarToolStripMenuItem.Checked)
        this.toolBar.Show();
      else this.toolBar.Hide();
    }

    private void showHideStatusBar(object sender, EventArgs e)
    {
      if (this.statusBarToolStripMenuItem.Checked)
        this.statusStrip1.Show();
      else this.statusStrip1.Hide();
    }

    private void OnPageSetup(object sender, EventArgs e)
    {
      //TODO
    }

    private void loadApplicationsToolStripMenuItem_Click(object sender, EventArgs e)
    {
      OdExLoadExtensionsForm dlg = new OdExLoadExtensionsForm();
      dlg.ShowDialog();
    }

    private void zoomExtents_Click(object sender, EventArgs e)
    {
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      act.OnZoomToExtents();
    }

    private void ExportSVG_Click(object sender, EventArgs e)
    {
      Export_Svg();
    }

    private void ExportPDF_Click(object sender, EventArgs e)
    {
      Export_Pdf();
    }

    private void ExportDWF_Click(object sender, EventArgs e)
    {
      Export_Dwf();
    }

    private void ExportSTL_Click(object sender, EventArgs e)
    {
      Export_Stl();
    }

    private void btnPan_CheckedChanged(object sender, EventArgs e)
    {
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      if (act == null)
        return;

      bool panState = this.btnPan.Checked;
      if (panState)
        this.btnRotation.Checked = false;

      act.EnablePan(panState);
    }

    private void btnRotation_CheckedChanged(object sender, EventArgs e)
    {
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      if (act == null)
        return;

      bool rotationState = this.btnRotation.Checked;

      if (rotationState)
        this.btnPan.Checked = false;

      act.EnableRotation(rotationState);
    }

    private void btnZoomToExtents_Click(object sender, EventArgs e)
    {
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      if (act == null)
        return;
      act.OnZoomToExtents();
    }
    #endregion

    #region Export 
    private void Export_Svg()
    {
      OdExDgnToSvgExportUI.Export(m_CurrentDatabase);
    }

    private void Export_Pdf()
    {
      OdExPdfExportUI.Export<OdExDgnToPdfExportParamsFiller>(m_CurrentDatabase);
    }

    private void Export_Dwf()
    {
      OdExDwfExportUI.Export<OdExDgnToDwfExportParamsFiller>(m_CurrentDatabase);
    }
    private void Export_Stl()
    {
      OdExStlExportUI.Export<OdExDgnToStlExportParamsFiller>(m_CurrentDatabase);
    }
    #endregion
  };
}
