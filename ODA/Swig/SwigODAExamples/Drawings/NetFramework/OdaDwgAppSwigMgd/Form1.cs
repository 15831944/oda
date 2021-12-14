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
using System.Drawing.Printing;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TD;
using OdKernelExamplesMgdLib;
using OdDrawingsExamplesMgdLib;
using OdExGUIComponentsMgdLib;
using OdKernelExamplesUIMgdLib.ExReactors;
using OdDrawingsExamplesMgdLib.Export.Dwf;
using OdKernelExamplesUIMgdLib.Export.Dwf;
using OdKernelExamplesUIMgdLib.Export.Pdf;
using OdDrawingsExamplesUIMgdLib.Export.Svg;
using OdDrawingsExamplesUIMgdLib.Import.Dwf;
using OdDrawingsExamplesUIMgdLib.Import.Dgn;

#if NET35 || NET40 || NET45
/*No available , because PdfImport use pdfium (available from vc14). For using PdfImport functionality you need use platform >= vc14*/
#else
using OdDrawingsExamplesUIMgdLib.Import.Pdf;
#endif

using OdDrawingsExamplesUIMgdLib.ExPrinting;
using OdKernelExamplesUIMgdLib.Helpers;
using OdDrawingsExamplesMgdLib.ExTrackers;
using OdDrawingsExamplesUIMgdLib.ExJigs.WinForms;
using OdKernelExamplesMgdLib.ExJigs;
using OdDrawingsExamplesMgdLib.ExJigs;
using OdDrawingsExamplesMgdLib.ExDbLayout;
using OdDrawingsExamplesMgdLib.Export.Pdf;

namespace OdaDwgAppSwigMgd
{
  public partial class Form1 : Form
  {
    private OdExHostApplicationServices _hostApp;
    private OdExSystemServices _sysSrv;
    private OdDbDatabase CurDb = null;

    public Form1()
    {
      InitializeComponent();
      this.Load += Form1_Load;
      this.FormClosing += Form1_FormClosing;

      EnableVectorizerToolButtons(false);
    }

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

    #region Action_Handlers_MainWindow

    private void Form1_FormClosing(object sender, FormClosingEventArgs e)
    {
      UnInit();
    }

    private void Form1_Load(object sender, EventArgs e)
    {
      Init();
    }

    private void onNew(object sender, EventArgs e)
    {
      OdExNewFileDlg newFileDlg = new OdExNewFileDlg();
      bool bLoaded = true;
      if (newFileDlg.ShowDialog() == DialogResult.OK)
      {
        MeasurementValue measure = MeasurementValue.kEnglish;
        if (!newFileDlg.isEnglishChoice())
          measure = MeasurementValue.kMetric;

        try
        {
          OdDbDatabase db = _hostApp.createDatabase(true, measure);

          InitTreeModel(db);
        }
        catch (OdError er)
        {
          MessageBox.Show(er.Message);
          bLoaded = false;
        }

        if (bLoaded)
        {
          UpdateMainMenu(true);
          setStatusLabel("Default Database Created.");
        }
      }
    }

    private void onOpen(object sender, EventArgs e)
    {
      if (openDwgDialog.ShowDialog() == DialogResult.OK)
      {
        bool bLoaded = true;
        try
        {
          OdDbDatabase db = openFile(openDwgDialog.FileName);
          InitTreeModel(db);
        }
        catch (Exception ex)
        {
          MessageBox.Show(ex.Message);
          bLoaded = false;
        }
        if (bLoaded)
        {
          UpdateMainMenu(true);
        }
      }

    }

    private void onPartialOpen(object sender, EventArgs e)
    {
      _hostApp.setPartialOption(true);
      onOpen(sender, e);
      _hostApp.setPartialOption(false);
    }

    private void onRecover(object sender, EventArgs e)
    {
      _hostApp.setRecoverOption(true);
      onOpen(sender, e);
      _hostApp.setRecoverOption(false);
    }

    private void onLoadApps(object sender, EventArgs e)
    {
      OdExLoadExtensionsForm loadApps = new OdExLoadExtensionsForm();
      loadApps.ShowDialog();
    }

    private void onAboutOdaDwg(object sender, EventArgs e)
    {
      OdExAboutBoxForm aboutBox = new OdExAboutBoxForm();
      aboutBox.SetProductName("OdaDwgAppSwigMgd sample application");
      aboutBox.ShowDialog();
    }

    private void snapshotToolStripMenuItem_Click(object sender, EventArgs e)
    {
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      if (act == null)
        return;
      act.SnapShot();
    }

    private void invertColorPaletteToolStripMenuItem_Click(object sender, EventArgs e)
    {
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      act.InvertColorPalette();
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

    private void exitToolStripMenuItem_Click(object sender, EventArgs e)
    {
      this.Close();
    }

    private void TreeFormClosed(object sender, FormClosedEventArgs e)
    {
      UpdateMainMenu(false);
    }

    private void createCircle_CPointAndRadius_toolStripMenuItem_Click(object sender, EventArgs e)
    {
      this.btnRotation.Checked = false;
      this.btnPan.Checked = false;

      EnableFunctionality(false);
      VectorizeForm targetForm = (VectorizeForm)this.ActiveMdiChild;
      targetForm.EnableMouseEditor(false);

      CreateCircle(CurDb);

      EnableFunctionality(true);
      targetForm.EnableMouseEditor(true);
    }

    private void create_line_ToolStripMenuItem_Click(object sender, EventArgs e)
    {
      this.btnRotation.Checked = false;
      this.btnPan.Checked = false;

      EnableFunctionality(false);
      VectorizeForm targetForm = (VectorizeForm)this.ActiveMdiChild;
      targetForm.EnableMouseEditor(false);

      CreateLine(CurDb);

      EnableFunctionality(true);
      targetForm.EnableMouseEditor(true);
    }

    private void createtext_ToolStripMenuItem_Click(object sender, EventArgs e)
    {
      this.btnRotation.Checked = false;
      this.btnPan.Checked = false;

      EnableFunctionality(false);
      VectorizeForm targetForm = (VectorizeForm)this.ActiveMdiChild;
      targetForm.EnableMouseEditor(false);

      CreateText(CurDb);

      EnableFunctionality(true);
      targetForm.EnableMouseEditor(true);
    }

    private void printToolStripMenuItem_Click(object sender, EventArgs e)
    {
      VectorizeForm act = this.ActiveMdiChild as VectorizeForm;
      if (act == null)
        return;
      Print(CurDb, act, false);
    }

    private void printPreviewMenuItem_Click(object sender, EventArgs e)
    {
      VectorizeForm act = this.ActiveMdiChild as VectorizeForm;
      if (act == null)
        return;
      Print(CurDb, act, true);
    }

    private void OnPageSetup(object sender, EventArgs e)
    {
      OnPageSetup();
    }

    private void zoomExtents_Click(object sender, EventArgs e)
    {
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      if (act == null)
        return;
      act.OnZoomToExtents();
    }

    private void toolStripButtonPan_CheckedChanged(object sender, EventArgs e)
    {
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      if (act == null)
        return;

      bool panState = this.btnPan.Checked;
      if (panState)
        this.btnRotation.Checked = false;

      act.EnableMouseEditor(!panState);
      act.EnablePan(panState);
    }

    private void toolStripButtonRotation_CheckedChanged(object sender, EventArgs e)
    {
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      if (act == null)
        return;

      bool rotationState = this.btnRotation.Checked;

      if (rotationState)
        this.btnPan.Checked = false;

      act.EnableMouseEditor(!rotationState);
      act.EnableRotation(rotationState);
    }

    private void ImportDWF_AS_New_Database_Click(object sender, EventArgs e)
    {
      Import_Dwf_AsNewDatabase();
    }

    private void ImportDWF_Click(object sender, EventArgs e)
    {
      Import_Dwf();
    }

    private void ImportDGN_Click(object sender, EventArgs e)
    {
      Import_Dgn();
    }

    private void ImportPDF_Click(object sender, EventArgs e)
    {
      Import_Pdf();
    }

    private void ExportPDF_Click(object sender, EventArgs e)
    {
      Export_Pdf();
    }

    private void ExportSVG_Click(object sender, EventArgs e)
    {
      Export_Svg();
    }

    private void ExportDWF_Click(object sender, EventArgs e)
    {
      Export_Dwf();
    }

    private void EnableFunctionality(bool state)
    {
      btnPan.Enabled = state;
      btnRotation.Enabled = state;
      btnZoomToExtents.Enabled = state;
      btnCreateCircle.Enabled = state;
      btnCreateLine.Enabled = state;
      btnCreateText.Enabled = state;
      openDocBtn.Enabled = state;
      newDocBtn.Enabled = state;
    }

    private void EnableFunctionalityExceptSender(object sender, bool state)
    {
      if (sender != btnPan)
        btnPan.Enabled = state;
      if (sender != btnRotation)
        btnRotation.Enabled = state;
      if (sender != btnZoomToExtents)
        btnZoomToExtents.Enabled = state;
      if (sender != btnCreateCircle)
        btnCreateCircle.Enabled = state;
      if (sender != btnCreateLine)
        btnCreateLine.Enabled = state;
      if (sender != openDocBtn)
        openDocBtn.Enabled = state;
      if (sender != newDocBtn)
        newDocBtn.Enabled = state;
    }

    #endregion Action_Handlers_MainWindow

    #region Create Circle
    private void CreateCircle(OdDbDatabase db)
    {
      MemoryTransaction tr = MemoryManager.GetMemoryManager().StartTransaction();

      OdDbCircle pCircle = trackRadius(db);

      AppendEntityToDatabase(db, pCircle);

      MemoryManager.GetMemoryManager().StopTransaction(tr);
    }

    private OdDbCircle trackRadius(OdDbDatabase db)
    {
      VectorizeForm targetForm = (VectorizeForm)this.ActiveMdiChild;

      Control targetControl = targetForm.GetViewControl;

      OdExCircleRadiusJig jig = OdExCircleRadiusJig.CreateDefaultCircleRadiusJig(db, targetForm.GetGsLayoutHelper);

      OdExCircleRadiusJigWinForms circleJig = new OdExCircleRadiusJigWinForms(targetControl, jig);

      OdExMouseDragerBase.OdExTextChangedEventHandler funcTxtInfoChange = (OdExMouseDragerBase.OdExStringEventArgs e) =>
      {
        toolStripStatusLabel1.Text = e.Text;
      };

      circleJig.Jig.TextInfoChanged += funcTxtInfoChange;

      bool created = (circleJig.Jig as OdExJigBase).DoIt();

      circleJig.Jig.TextInfoChanged -= funcTxtInfoChange;

      if (created)
        return jig.Tracker.m_CircleTracker.m_TargetCircle;
      else
        return null;
    }
    #endregion

    #region Create Line
    private void CreateLine(OdDbDatabase db)
    {
      MemoryTransaction tr = MemoryManager.GetMemoryManager().StartTransaction();

      OdGePoint3d ptStart = null;
      for (bool finish = false; !finish;)
      {
        MemoryTransaction tr2 = MemoryManager.GetMemoryManager().StartTransaction();

        OdDbLine pNewLine = trackLine(db, ptStart);
        if (pNewLine != null)
        {
          AppendEntityToDatabase(db, pNewLine);

          ptStart = new OdGePoint3d(pNewLine.endPoint());
        }
        else
          finish = true;

        MemoryManager.GetMemoryManager().StopTransaction(tr2);
      }

      MemoryManager.GetMemoryManager().StopTransaction(tr);
    }

    private OdDbLine trackLine(OdDbDatabase db, OdGePoint3d ptStart)
    {
      VectorizeForm targetForm = (VectorizeForm)this.ActiveMdiChild;

      Control targetControl = targetForm.GetViewControl;

      OdExLineJig jig = OdExLineJig.CreateDefaultJig(db, targetForm.GetGsLayoutHelper, ptStart);

      OdExLineJigWinForms lineJig = new OdExLineJigWinForms(targetControl, jig);

      OdExMouseDragerBase.OdExTextChangedEventHandler funcTxtInfoChange = (OdExMouseDragerBase.OdExStringEventArgs e) =>
      {
        toolStripStatusLabel1.Text = e.Text;
      };

      lineJig.Jig.TextInfoChanged += funcTxtInfoChange;

      bool created = (lineJig.Jig as OdExJigBase).DoIt();

      lineJig.Jig.TextInfoChanged -= funcTxtInfoChange;

      if (created)
        return jig.Tracker.TargetLine;
      else
        return null;
    }
    #endregion

    #region Create Text
    private void CreateText(OdDbDatabase db)
    {
      MemoryTransaction tr = MemoryManager.GetMemoryManager().StartTransaction();

      OdDbText pText = trackText(db);

      AppendEntityToDatabase(db, pText);

      MemoryManager.GetMemoryManager().StopTransaction(tr);
    }

    OdDbText trackText(OdDbDatabase db)
    {
      VectorizeForm targetForm = (VectorizeForm)this.ActiveMdiChild;

      Control targetControl = targetForm.GetViewControl;

      OdExTextJig jig = OdExTextJig.CreateDefaultJig(db, targetForm.GetGsLayoutHelper);

      OdExTextJigWinForms textJig = new OdExTextJigWinForms(targetControl, jig);

      OdExMouseDragerBase.OdExTextChangedEventHandler funcTxtInfoChange = (OdExMouseDragerBase.OdExStringEventArgs e) =>
      {
        toolStripStatusLabel1.Text = e.Text;
      };

      textJig.Jig.TextInfoChanged += funcTxtInfoChange;

      bool created = (textJig.Jig as OdExJigBase).DoIt();

      textJig.Jig.TextInfoChanged -= funcTxtInfoChange;

      if (created)
        return jig.Tracker.TargetText;
      else
        return null;
    }
    #endregion

    #region Helpers
    private void AppendEntityToDatabase(OdDbDatabase db, OdDbEntity ent)
    {
      if (ent == null)
        return;

      OdDbObjectId spaceId = OdExAuxUtilities.activeBlockTableRecord(db);
      OdDbBlockTableRecord pSpace = spaceId.safeOpenObject(OpenMode.kForWrite) as OdDbBlockTableRecord;
      pSpace.appendOdDbEntity(ent);
      ((VectorizeForm)this.ActiveMdiChild).Invalidate();
    }
    #endregion

    #region Print

    private void Print(OdDbDatabase db, VectorizeForm act, bool isPreview)
    {
      try
      {
        if (act == null || db == null)
          return;
        OdExPrinting pr = new OdExPrinting();
        pr.Print(db, act.GetGsLayoutHelper.activeView(), isPreview);
      }
      catch (Exception err)
      {
        MessageBox.Show("Error print: " + err.Message);
      }
    }

    private void OnPageSetup()
    {
      CurDb.startTransaction();

      OdDbObjectId idLayout = ((OdDbBlockTableRecord)CurDb.getActiveLayoutBTRId().safeOpenObject()).getLayoutId();
      OdDbLayout pLayout = (OdDbLayout)idLayout.safeOpenObject(OpenMode.kForWrite);

      OdDbPlotSettings pPlotSettings = pLayout;
      OdExPageSetupDlg pageSetup = new OdExPageSetupDlg(pPlotSettings);

      if (pageSetup.ShowDialog() != DialogResult.OK)
      {
        CurDb.abortTransaction();
        return;
      }
      CurDb.endTransaction();
    }

    #endregion Print

    #region Export

    private void Export_Pdf()
    {
      OdExPdfExportUI.Export<OdExDwgToPdfExportParamsFiller>(CurDb);
    }

    private void Export_Svg()
    {
      OdExportSvgGUIExample.Export(CurDb);
    }

    private void Export_Dwf()
    {
      OdExDwfExportUI.Export<OdExDwgToDwfExportParamsFiller>(CurDb);
    }

    #endregion

    #region Import
    private void Import_Dwf()
    {
      OdExDwfImportByCommandGUIExample.Import(CurDb);
    }

    private void Import_Dwf_AsNewDatabase()
    {
      OdDbDatabase db = _hostApp.createDatabase();

      OdExDwfImportGUIExample.Import(db);

      InitTreeModel(db);
      UpdateMainMenu(true);
    }

    private void Import_Dgn()
    {
      OdDbDatabase db = _hostApp.createDatabase();

      OdExDgnImportGUIExample.Import(_hostApp, db);

      InitTreeModel(db);
      UpdateMainMenu(true);
    }

    private void Import_Pdf()
    {
#if NET35 || NET40 || NET45
      /*No available , because PdfImport use pdfium (available from vc14). For using PdfImport functionality you need use platform >= vc14*/
      MessageBox.Show("No available, because PdfImport use pdfium (available from vc14). For using PdfImport functionality you need use platform >= vc14");
#else
      OdDbDatabase db = _hostApp.createDatabase();

      OdExPDF2dImportGUIExample.Import(db);

      InitTreeModel(db);
      UpdateMainMenu(true);
#endif

    }

    #endregion

    #region Undo/Redo 
    private void undovariant1ToolStripMenuItem_Click(object sender, EventArgs e)
    {
      // 2 lines were added - thus undo is called twice
      CurDb.undo();
      CurDb.undo();
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      act.Invalidate();
      act.Update();
      redoToolStripMenuItem.Enabled = CurDb.hasRedo();
    }

    private void appendLinesToolStripMenuItem_Click(object sender, EventArgs e)
    {
      // code provided as an illustration of user described issue
      try
      {
        CurDb.disableUndoRecording(false);
        CurDb.startUndoRecord(); //First Time
        {

          //b) We have put getBlockTableId().safeOpenObject() call in the undo record scope.
          using (OdDbBlockTableRecord pBlockTableRecord = (OdDbBlockTableRecord)CurDb.getModelSpaceId().safeOpenObject(Teigha.TD.OpenMode.kForWrite))
          {
            OdDbLine pLine = OdDbLine.createObject();
            OdGePoint3d spoint = new OdGePoint3d();
            spoint.x = 66;
            spoint.y = 86;
            spoint.z = 0;
            OdGePoint3d epoint = new OdGePoint3d();
            epoint.x = 110;
            epoint.y = 86;
            epoint.z = 0;
            pLine.setStartPoint(spoint);
            pLine.setEndPoint(epoint);

            pLine.setDatabaseDefaults(CurDb);
            pBlockTableRecord.appendOdDbEntity(pLine);

            // DownGrade Open
            pBlockTableRecord.downgradeOpen();
            pLine.Dispose();
            spoint.Dispose();
            epoint.Dispose();
            pLine = null;
            spoint = null;
            epoint = null;
          }
        }

        CurDb.startUndoRecord();//Second Time
        {
          //b) We have put getBlockTableId().safeOpenObject() call in the undo record scope.
          using (OdDbBlockTableRecord pBlockTableRecord = (OdDbBlockTableRecord)CurDb.getModelSpaceId().safeOpenObject(Teigha.TD.OpenMode.kForWrite))
          {
            OdDbLine pLine1 = OdDbLine.createObject();
            OdGePoint3d spoint1 = new OdGePoint3d();
            OdGePoint3d epoint1 = new OdGePoint3d();
            spoint1.x = 14;
            spoint1.y = 96;
            spoint1.z = 0;

            epoint1.x = 47;
            epoint1.y = 125;
            epoint1.z = 0;
            pLine1.setStartPoint(spoint1);
            pLine1.setEndPoint(epoint1);
            pLine1.setDatabaseDefaults(CurDb);
            pBlockTableRecord.appendOdDbEntity(pLine1);

            //Downgrade Open
            pBlockTableRecord.downgradeOpen();
            pLine1.Dispose();
            spoint1.Dispose();
            epoint1.Dispose();
            pLine1 = null;
            spoint1 = null;
            epoint1 = null;
          }
        }
        VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
        act.Invalidate();
        act.Update();
      }
      catch (OdError err)
      {
        MessageBox.Show(err.description());
      }
    }

    private void undovariant2ToolStripMenuItem_Click(object sender, EventArgs e)
    {
      try
      {
        // we added 2 lines, thus we need to make 2 steps back
        ExStringIO strIO = ExStringIO.create("2");
        OdDbCommandContext pCon = ExDbCommandContext.createObject(strIO, CurDb);
        OdEdCommandStack pStack = Globals.odedRegCmds();
        Globals.odrxDynamicLinker().loadModule("DbCommands");

        pStack.executeCommand("UNDO", pCon);
        VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
        act.Invalidate();
        act.Update();
      }
      catch (OdError err)
      {
        MessageBox.Show(err.description());
      }
      redoToolStripMenuItem.Enabled = CurDb.hasRedo();
    }

    private void redoToolStripMenuItem_Click(object sender, EventArgs e)
    {
      CurDb.redo();
      VectorizeForm act = (VectorizeForm)this.ActiveMdiChild;
      act.Invalidate();
      act.Update();
      redoToolStripMenuItem.Enabled = CurDb.hasRedo();
    }
    #endregion Undo/Redo 

    private void Init()
    {
      InitFormTransaction();

      _sysSrv = new OdExSystemServices();
      _hostApp = new OdExHostApplicationServices();

      TD_Db.odInitialize(_sysSrv);

      OdExLoadExtensionsForm.rxInit();

      Environment.SetEnvironmentVariable("DDPLOTSTYLEPATHS", OdExAppRegistryFindConfigHelpers.FindConfigPath(String.Format("PrinterStyleSheetDir")));

      LoadModules();
    }

    private void UnInit()
    {
      FinishFormTransaction();
      try
      {
        OdExLoadExtensionsForm.rxUninit();
        TD_Db.odUninitialize();
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

    private void LoadModules()
    {
      // Sample grip/snap points implementation
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("GripPoints");
      // DimAssoc Support
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("ExDimAssoc");
      // Additional commands support (ERASE,EXPLODE,PURGE, etc.)
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("DbCommands");
      // Sample custom objects + debug commands
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("ExCustObjs");

      Teigha.Core.Globals.odrxDynamicLinker().loadModule("ExCommands");

      Teigha.Core.Globals.odrxDynamicLinker().loadModule("TD_RasterExport");

      Teigha.Core.Globals.odrxDynamicLinker().loadModule("ExDeepCloning");
      // Core dynamic blocks support
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("TD_DynBlocks");
      // Sample commands illustrating dynamic blocks usage
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("ExDynamicBlocks");
      // Support of old style materials, lights, etc. (AVE_*)
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("TD_Ave");
      // Field evaluator for "AcVar", and "AcDiesel" fields
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("ExFieldEvaluator");
      // custom OdDbPlotSettingsValidator implementation
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("PlotSettingsValidator");
      // OdAsSurfBody object support module
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("AutoSurfServices");
      // Teigha format import/export module
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("TD_Tf");
    }

    private void InitTreeModel(OdDbDatabase db)
    {
      this.CurDb = db;
      Tree t = new Tree(this.CurDb);
      t.FormClosed += new FormClosedEventHandler(TreeFormClosed);
      t.MdiParent = this;
      t.Show();
      t.FillTree();
    }

    public void UpdateRenderFlags(bool flag)
    {
      snapshotToolStripMenuItem.Enabled = flag;
      invertColorPaletteToolStripMenuItem.Enabled = flag;
    }

    public void SolveEnableInsertItems(bool flag)
    {
      insert_toolStripMenuItem.Enabled = flag;
    }

    private void UpdateMainMenu(bool flag)
    {
      ExportSVG.Visible = flag;
      ExportPDF.Visible = flag;
      ExportDWF.Visible = flag;
      ImportDWF.Visible = flag;
      ImportAsNewDatabase.Visible = flag;
      ImportDGN.Visible = flag;
      ImportPDF.Visible = flag;
      renderToolStripMenuItem.Visible = flag;
      pageSetupToolStripMenuItem.Visible = flag;
    }

    public void setStatusLabel(string s)
    {
      this.toolStripStatusLabel1.Text = s;
    }


    public VectorizeForm createVectorizationWindow(OdDbDatabase db)
    {
      VectorizeForm vectorizer = new VectorizeForm(db);
      vectorizer.MdiParent = this;
      vectorizer.Activated += (object _sender, EventArgs _e) =>
      {
        this.UpdateRenderFlags(true);
        this.SolveEnableInsertItems(true);
        this.EnableVectorizerToolButtons(true);
      };
      vectorizer.EnableZoom(true);
      vectorizer.EnableMouseEditor(true);
      vectorizer.Show();
      return vectorizer;
    }


    public void EnableVectorizerToolButtons(bool state)
    {
      this.btnPan.Enabled = state;
      this.btnRotation.Enabled = state;
      this.btnZoomToExtents.Enabled = state;
    }

    private OdDbDatabase openFile(String fileName)
    {
      OdDbDatabase pDb;

      if (fileName.EndsWith(".dgn"))
      {
        OdDgnImportModule module = (OdDgnImportModule)OdRxDynamicLinker.createObject().loadApp("TD_DgnImport", false);
        pDb = null;
      }
      else if (fileName.EndsWith(".dwf") || fileName.EndsWith(".dwfx"))
      {
        OdDwfImportModule module = (OdDwfImportModule)OdRxDynamicLinker.createObject().loadApp("TD_Dwf7Import", false);
        pDb = null;
      }
      else if (_hostApp.isRecoverOption())
      {
        OdExDbAppAuditInfo aiInfo = new OdExDbAppAuditInfo();
        aiInfo.setHostAppServices(_hostApp);
        RxSystemServicesImpl sysServ = new RxSystemServicesImpl();
        _hostApp.InitAuditInfoDlg();
        pDb = _hostApp.recoverFile(sysServ.createFile(openDwgDialog.FileName), aiInfo);
        _hostApp.getAuditInfoDlg().ShowDialog();
      }
      else
      {
        pDb = _hostApp.readFile(openDwgDialog.FileName, false, _hostApp.isPartial());
      }
      return pDb;
    }
  };
}
