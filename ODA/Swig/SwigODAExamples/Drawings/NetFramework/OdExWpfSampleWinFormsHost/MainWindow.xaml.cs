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
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using Teigha.Core;
using Teigha.TD;
using OdDrawingsExamplesMgdLib;
using OdDrawingsExamplesMgdLib.ExJigs;
using OdDrawingsExamplesUIMgdLib.ExJigs.Wpf;
using OdKernelExamplesMgdLib.ExJigs;
using OdKernelExamplesMgdLib;
using OdDrawingsExamplesMgdLib.ExDbLayout;
using OdKernelExamplesMgdLib.Export.Pdf;
using OdDrawingsExamplesMgdLib.Export.Pdf;
using OdDrawingsExamplesUIMgdLib.ExJigs.WinForms;
using OdDrawingsExamplesUIMgdLib.ExView;

namespace OdExWpfSampleWinFormsHost
{
  /// <summary>
  /// Interaction logic for MainWindow.xaml
  /// </summary>
  public partial class MainWindow : Window
  {
    private OdDbDatabase m_CurrentDatabase;
    private OdExSystemServices m_SystemServices;
    private OdExCustomServices m_HostAppServices;

    public MainWindow()
    {
      InitializeComponent();
      this.Loaded += AppWindow_Loaded;
      this.Closing += AppWindow_Closing;
      this.Title = "OdExWpfSampleWinFormsHost";
    }

    #region Load/Close form event handlers

    private void AppWindow_Loaded(object sender, RoutedEventArgs e)
    {
      InitOdaSdk();

      InitLayoutManagerReactor();

      EnableFunctionality(false);
    }

    private void AppWindow_Closing(object sender, System.ComponentModel.CancelEventArgs e)
    {
      if (m_dwgControl.IsRenderingInitialized())
        m_dwgControl.UnInit();

      UnInitLayoutManagerReactor();

      UnInitOdaSdk();
    }
    #endregion

    #region Initialize/Uninitialize ODA SDK
    private void InitOdaSdk()
    {
      StartRootTransaction();

      m_SystemServices = new OdExSystemServices();
      m_HostAppServices = new OdExCustomServices();
      TD_Db.odInitialize(m_SystemServices);

      LoadModules();
    }

    private void LoadModules()
    {
      // Sample grip/snap points implementation
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("GripPoints");
    }

    private void UnInitOdaSdk()
    {
      StopRootTransaction();

      try
      {
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
    #endregion

    #region Root Application Memory Transaction
    private MemoryTransaction rootTransaction;
    private void StartRootTransaction()
    {
      if (rootTransaction != null)
        return;
      rootTransaction = MemoryManager.GetMemoryManager().StartTransaction();
    }

    private void StopRootTransaction()
    {
      if (rootTransaction == null)
        return;
      MemoryManager.GetMemoryManager().StopTransaction(rootTransaction);
      rootTransaction = null;
    }
    #endregion

    #region Current OdDbDatabase Memory Transaction
    private MemoryTransaction mTrCurrentDatabase = null;

    private void StartTransactionForCurrentDatabase()
    {
      if (mTrCurrentDatabase != null)
        StopTransactionForCurrentDatabase();
      mTrCurrentDatabase = MemoryManager.GetMemoryManager().StartTransaction();
    }

    private void StopTransactionForCurrentDatabase()
    {
      if (mTrCurrentDatabase == null)
        return;
      MemoryManager.GetMemoryManager().StopTransaction(mTrCurrentDatabase);
      mTrCurrentDatabase = null;
    }
    #endregion

    #region Layout Manager Reactor
    private OdExDbLayoutManagerReactor m_layoutReactor;
    private void InitLayoutManagerReactor()
    {
      if (m_layoutReactor != null || m_HostAppServices == null)
        return;

      m_layoutReactor = new OdExDbLayoutManagerReactor();
      m_HostAppServices.layoutManager().addReactor(m_layoutReactor);

      AddLayoutManagerReactorHandlers();
    }

    private void UnInitLayoutManagerReactor()
    {
      if (m_layoutReactor == null || m_HostAppServices == null)
        return;

      RemoveLayoutManagerReactorHandlers();

      m_HostAppServices.layoutManager().removeReactor(m_layoutReactor);
      m_layoutReactor = null;
    }


    private void AddLayoutManagerReactorHandlers()
    {
      if (m_layoutReactor == null)
        return;
      m_layoutReactor.OdExLayoutSwitchedEvent += OnReInitializeRendreingAfterChangeLayout;
    }

    private void RemoveLayoutManagerReactorHandlers()
    {
      if (m_layoutReactor == null)
        return;
      m_layoutReactor.OdExLayoutSwitchedEvent -= OnReInitializeRendreingAfterChangeLayout;
    }
    #endregion Layout Manager Reactor

    #region Button Event Handlers

    private void btnOpenLayoutManager_Click(object sender, RoutedEventArgs e)
    {
      OdExLayoutManagerWpfWindow window = new OdExLayoutManagerWpfWindow(m_CurrentDatabase);
      window.Owner = this;
      window.ShowDialog();
    }

    private void btnFileOpen_Click(object sender, RoutedEventArgs e)
    {
      OnOpenAndRenderDwgFile();
    }

    private void ExportPDF_Click(object sender, RoutedEventArgs e)
    {
      OnExportPDF2d();
    }

    private void ZoomToExtents_Click(object sender, RoutedEventArgs e)
    {
      OnZoomToExtents();
    }

    private void EnablePan_Click(object sender, RoutedEventArgs e)
    {
      bool isEnable = (sender as ToggleButton).IsChecked == true;
      OnPan(isEnable);
    }

    private void EnableRotation_Click(object sender, RoutedEventArgs e)
    {
      bool isEnable = (sender as ToggleButton).IsChecked == true;
      OnRotation(isEnable);
    }

    private void DrawCircle_Click(object sender, RoutedEventArgs e)
    {
      EnableFunctionalityExceptSender(null, false);
      ResetPanRotation();
      OnMouseEditor(false);

      OnDrawCircle();

      OnMouseEditor(true);
      EnableFunctionalityExceptSender(null, true);
    }

    private void DrawText_Click(object sender, RoutedEventArgs e)
    {
      EnableFunctionalityExceptSender(null, false);
      ResetPanRotation();
      OnMouseEditor(false);

      OnDrawText();

      OnMouseEditor(true);
      EnableFunctionalityExceptSender(null, true);
    }

    private void DrawLine_Click(object sender, RoutedEventArgs e)
    {
      EnableFunctionalityExceptSender(null, false);
      ResetPanRotation();
      OnMouseEditor(false);

      OnDrawLine();

      OnMouseEditor(true);
      EnableFunctionalityExceptSender(null, true);
    }

    #endregion

    #region Functionality

    private void OnOpenAndRenderDwgFile()
    {
      bool enableFuntionality = true;
      try
      {
        String dwgFilePath = OnFormDwgFileOpen();
        if (String.IsNullOrEmpty(dwgFilePath))
          return;

        if (m_dwgControl.IsRenderingInitialized())
          m_dwgControl.UnInit();

        StartTransactionForCurrentDatabase();

        m_CurrentDatabase = m_HostAppServices.readFile(dwgFilePath);

        m_dwgControl.Init(m_CurrentDatabase);
      }
      catch (Exception err)
      {
        enableFuntionality = false;
        StopTransactionForCurrentDatabase();
      }

      OnZoom(enableFuntionality);
      OnMouseEditor(enableFuntionality);
      EnableFunctionality(enableFuntionality);
    }

    private void OnZoomToExtents()
    {
      if (!m_dwgControl.IsInitialized)
        return;

      m_dwgControl.ZoomToExtents();
    }

    private void OnZoom(bool isEnable)
    {
      m_dwgControl.EnableZoom(isEnable);
    }

    private void OnPan(bool isEnable)
    {
      if (isEnable)
      {
        btnEnableRotation.IsChecked = false;
        m_dwgControl.EnableRotation(false);
      }

      m_dwgControl.EnableMouseEditor(!isEnable);
      m_dwgControl.EnablePan(isEnable);
    }

    private void OnRotation(bool isEnable)
    {
      if (isEnable)
      {
        btnEnablePan.IsChecked = false;
        m_dwgControl.EnablePan(false);
      }

      m_dwgControl.EnableMouseEditor(!isEnable);
      m_dwgControl.EnableRotation(isEnable);
    }

    private void OnMouseEditor(bool isEnable)
    {
      m_dwgControl.EnableMouseEditor(isEnable);
    }

    private void OnExportPDF2d()
    {
      if (m_CurrentDatabase == null)
        return;

      Microsoft.Win32.SaveFileDialog savePdf = new Microsoft.Win32.SaveFileDialog();
      savePdf.DefaultExt = ".pdf";
      savePdf.Title = "Save pdf file";
      savePdf.Filter = "PDF files|*.pdf";

      if (savePdf.ShowDialog() != true)
        return;

      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      OdExPdfExportParamsFiller paramsFiller = OdExDwgToPdfExportParamsFiller.CreateDefault();

      paramsFiller.PdfFilePath = savePdf.FileName;

      OdStringArray layouts = new OdStringArray();
      OdDbLayoutExamples.FillLayoutList(m_CurrentDatabase, layouts, paramsFiller.AllLayouts, false);
      paramsFiller.LayoutList = layouts;

      try
      {
        using (PDFExportParams parametrsExport = paramsFiller.Generate(m_CurrentDatabase))
        {
          OdExPdfExport exportExample = new OdExPdfExport();
          exportExample.Export(parametrsExport);
        }
      }
      catch (Exception err)
      {
        MessageBox.Show(
        err.Message,
        "Information");
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }

    }

    private void OnDrawCircle()
    {
      MemoryTransaction tr = MemoryManager.GetMemoryManager().StartTransaction();

      OdDbCircle circle = CreateCircleByJig();

      AppendEntityToDatabase(circle);

      MemoryManager.GetMemoryManager().StopTransaction(tr);
    }

    private void OnDrawText()
    {
      //Set keyboard focus to control
      m_dwgControl.RenderControl.Focus();

      MemoryTransaction tr = MemoryManager.GetMemoryManager().StartTransaction();

      OdDbText pText = CreateTextByJig();

      AppendEntityToDatabase(pText);

      MemoryManager.GetMemoryManager().StopTransaction(tr);

      //UnSet keyboard focus to control
      //m_dwgControl.Focusable = false;
      //Keyboard.Focus(this);
    }

    private void OnDrawLine()
    {
      MemoryTransaction tr = MemoryManager.GetMemoryManager().StartTransaction();

      OdDbObjectId spaceId = OdExAuxUtilities.activeBlockTableRecord(m_CurrentDatabase);
      OdDbBlockTableRecord pSpace = spaceId.safeOpenObject(OpenMode.kForWrite) as OdDbBlockTableRecord;

      OdGePoint3d ptStart = null;
      for (bool finish = false; !finish;)
      {
        MemoryTransaction tr2 = MemoryManager.GetMemoryManager().StartTransaction();

        OdDbLine line = CreateLineByJig(ptStart);
        if (line != null)
        {
          AppendEntityToDatabase(line);

          ptStart = new OdGePoint3d(line.endPoint());
        }
        else
          finish = true;


        MemoryManager.GetMemoryManager().StopTransaction(tr2);

      }

      MemoryManager.GetMemoryManager().StopTransaction(tr);
    }

    private void OnReInitializeRendreingAfterChangeLayout(string newLayoutName, OdDbObjectId newLayoutId)
    {
      if (m_dwgControl == null)
        return;
      if (!m_dwgControl.IsRenderingInitialized())
        return;
      m_dwgControl.ReInit();
    }

    #endregion

    #region Helpers
    private void EnableFunctionality(bool state)
    {
      btnExportPdf.IsEnabled = state;
      btnEnablePan.IsEnabled = state;
      btnEnableRotation.IsEnabled = state;
      btnZoomToExtents.IsEnabled = state;
      btnDrawCircle.IsEnabled = state;
      btnDrawText.IsEnabled = state;
      btnDrawLine.IsEnabled = state;
      btnLayoutManager.IsEnabled = state;
    }

    private void EnableFunctionalityExceptSender(object sender, bool state)
    {
      if (sender != btnDrawCircle)
        btnDrawCircle.IsEnabled = state;
      if (sender != btnDrawLine)
        btnDrawLine.IsEnabled = state;
      if (sender != btnDrawText)
        btnDrawText.IsEnabled = state;
      if (sender != btnEnablePan)
        btnEnablePan.IsEnabled = state;
      if (sender != btnEnableRotation)
        btnEnableRotation.IsEnabled = state;
      if (sender != btnExportPdf)
        btnExportPdf.IsEnabled = state;
      if (sender != btnOpenFile)
        btnOpenFile.IsEnabled = state;
      if (sender != btnZoomToExtents)
        btnZoomToExtents.IsEnabled = state;
      if (sender != btnLayoutManager)
        btnLayoutManager.IsEnabled = state;
    }

    private void ResetPanRotation()
    {
      m_dwgControl.EnableRotation(false);
      m_dwgControl.EnablePan(false);
    }

    private String OnFormDwgFileOpen()
    {
      // Create OpenFileDialog 
      Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

      // Set filter for file extension and default file extension 
      dlg.DefaultExt = ".dwg";
      dlg.Filter = "DWG files (.dwg)|*.dwg";

      // Display OpenFileDialog by calling ShowDialog method 
      if (dlg.ShowDialog() != true)
        return String.Empty;
      return dlg.FileName;
    }

    private void AppendEntityToDatabase(OdDbEntity ent)
    {
      if (ent == null || m_CurrentDatabase == null)
        return;
      OdDbDatabase db = m_CurrentDatabase;

      OdDbObjectId spaceId = OdExAuxUtilities.activeBlockTableRecord(db);
      OdDbBlockTableRecord pSpace = spaceId.safeOpenObject(OpenMode.kForWrite) as OdDbBlockTableRecord;
      pSpace.appendOdDbEntity(ent);

      if (m_dwgControl.IsRenderingInitialized())
        m_dwgControl.RenderControl.Invalidate();
    }

    #endregion

    #region DrawCircle

    private OdDbCircle CreateCircleByJig()
    {
      OdDbDatabase db = m_CurrentDatabase;
      OdExViewDrawingControl targetControl = m_dwgControl.RenderControl;
      OdGsLayoutHelper gsView = m_dwgControl.m_GsLayoutHelper;

      OdExCircleRadiusJig jig = OdExCircleRadiusJig.CreateDefaultCircleRadiusJig(db, gsView);

      OdExCircleRadiusJigWinForms winFormsJig = new OdExCircleRadiusJigWinForms(targetControl, jig);

      OdExMouseDragerBase.OdExTextChangedEventHandler funcTxtInfoChange = (OdExMouseDragerBase.OdExStringEventArgs e) =>
      {
        MessageBox.Show(e.Text, "Information of example");
      };

      winFormsJig.Jig.TextInfoChanged += funcTxtInfoChange;
      winFormsJig.Jig.PostPaintEvent += targetControl.Invalidate;

      bool selectFinish = (winFormsJig.Jig as OdExJigBase).DoIt();

      winFormsJig.Jig.TextInfoChanged -= funcTxtInfoChange;
      winFormsJig.Jig.PostPaintEvent -= targetControl.Invalidate;

      if (selectFinish)
        return jig.Tracker.m_CircleTracker.m_TargetCircle;
      else
        return null;
    }

    #endregion

    #region DrawText
    OdDbText CreateTextByJig()
    {
      OdExViewDrawingControl targetControl = m_dwgControl.RenderControl;

      OdExTextJig jig = OdExTextJig.CreateDefaultJig(m_CurrentDatabase, m_dwgControl.m_GsLayoutHelper);

      OdExTextJigWinForms winFormsJig = new OdExTextJigWinForms(targetControl, jig);

      OdExMouseDragerBase.OdExTextChangedEventHandler funcTxtInfoChange = (OdExMouseDragerBase.OdExStringEventArgs e) =>
      {
        MessageBox.Show(e.Text, "Information of example");
      };

      winFormsJig.Jig.TextInfoChanged += funcTxtInfoChange;
      winFormsJig.Jig.PostPaintEvent += targetControl.Invalidate;

      bool created = (winFormsJig.Jig as OdExJigBase).DoIt();

      winFormsJig.Jig.TextInfoChanged -= funcTxtInfoChange;
      winFormsJig.Jig.PostPaintEvent -= targetControl.Invalidate;

      if (created)
        return jig.Tracker.TargetText;
      else
        return null;
    }
    #endregion

    #region DrawLine
    private OdDbLine CreateLineByJig(OdGePoint3d ptStart)
    {
      OdExLineJig jig = OdExLineJig.CreateDefaultJig(m_CurrentDatabase, m_dwgControl.m_GsLayoutHelper, ptStart);

      OdExViewDrawingControl targetControl = m_dwgControl.RenderControl;

      OdExLineJigWinForms winFormsJig = new OdExLineJigWinForms(targetControl, jig);

      OdExMouseDragerBase.OdExTextChangedEventHandler funcTxtInfoChange = (OdExMouseDragerBase.OdExStringEventArgs e) =>
      {
        MessageBox.Show(e.Text, "Information of example");
      };

      winFormsJig.Jig.TextInfoChanged += funcTxtInfoChange;
      winFormsJig.Jig.PostPaintEvent += targetControl.Invalidate;

      bool created = (winFormsJig.Jig as OdExJigBase).DoIt();

      winFormsJig.Jig.TextInfoChanged -= funcTxtInfoChange;
      winFormsJig.Jig.PostPaintEvent -= targetControl.Invalidate;

      if (created)
        return jig.Tracker.TargetLine;
      else
        return null;
    }
    #endregion
  }
}
