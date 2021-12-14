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
namespace OdaDwgAppSwigMgd
{
  partial class Form1
  {
    /// <summary>
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary>
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
      this.menuStrip1 = new System.Windows.Forms.MenuStrip();
      this.fileToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
      this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.partialOpenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.recoverToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
      this.printPreviewMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.printToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.pageSetupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
      this.ExportSVG = new System.Windows.Forms.ToolStripMenuItem();
      this.ExportPDF = new System.Windows.Forms.ToolStripMenuItem();
      this.ExportDWF = new System.Windows.Forms.ToolStripMenuItem();
      this.ImportDWF = new System.Windows.Forms.ToolStripMenuItem();
      this.ImportDGN = new System.Windows.Forms.ToolStripMenuItem();
      this.ImportPDF = new System.Windows.Forms.ToolStripMenuItem();
      this.ImportAsNewDatabase = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
      this.exitToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
      this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolbarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.statusBarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.renderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.snapshotToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.invertColorPaletteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolsToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
      this.loadApps = new System.Windows.Forms.ToolStripMenuItem();
      this.undoTestToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.appendLinesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.undoLineAddingToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.undovariant2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.redoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.aboutOdaDwgAppToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.insert_toolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.btnCreateCircle = new System.Windows.Forms.ToolStripMenuItem();
      this.btnCreateLine = new System.Windows.Forms.ToolStripMenuItem();
      this.btnCreateText = new System.Windows.Forms.ToolStripMenuItem();
      this.openDwgDialog = new System.Windows.Forms.OpenFileDialog();
      this.statusStrip1 = new System.Windows.Forms.StatusStrip();
      this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
      this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
      this.toolBar = new System.Windows.Forms.ToolStrip();
      this.newDocBtn = new System.Windows.Forms.ToolStripButton();
      this.openDocBtn = new System.Windows.Forms.ToolStripButton();
      this.btnPan = new System.Windows.Forms.ToolStripButton();
      this.btnRotation = new System.Windows.Forms.ToolStripButton();
      this.btnZoomToExtents = new System.Windows.Forms.ToolStripButton();
      this.menuStrip1.SuspendLayout();
      this.statusStrip1.SuspendLayout();
      this.toolBar.SuspendLayout();
      this.SuspendLayout();
      // 
      // menuStrip1
      // 
      this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem1,
            this.viewToolStripMenuItem,
            this.toolsToolStripMenuItem1,
            this.undoTestToolStripMenuItem,
            this.helpToolStripMenuItem,
            this.insert_toolStripMenuItem});
      this.menuStrip1.Location = new System.Drawing.Point(0, 0);
      this.menuStrip1.Name = "menuStrip1";
      this.menuStrip1.Size = new System.Drawing.Size(784, 24);
      this.menuStrip1.TabIndex = 1;
      this.menuStrip1.Text = "menuStrip1";
      // 
      // fileToolStripMenuItem1
      // 
      this.fileToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.partialOpenToolStripMenuItem,
            this.recoverToolStripMenuItem,
            this.toolStripSeparator1,
            this.printPreviewMenuItem,
            this.printToolStripMenuItem,
            this.pageSetupToolStripMenuItem,
            this.toolStripSeparator4,
            this.ExportSVG,
            this.ExportPDF,
            this.ExportDWF,
            this.ImportDWF,
            this.ImportDGN,
            this.ImportPDF,
            this.ImportAsNewDatabase,
            this.toolStripSeparator3,
            this.exitToolStripMenuItem1});
      this.fileToolStripMenuItem1.Name = "fileToolStripMenuItem1";
      this.fileToolStripMenuItem1.Size = new System.Drawing.Size(37, 20);
      this.fileToolStripMenuItem1.Text = "&File";
      // 
      // newToolStripMenuItem
      // 
      this.newToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("newToolStripMenuItem.Image")));
      this.newToolStripMenuItem.Name = "newToolStripMenuItem";
      this.newToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
      this.newToolStripMenuItem.Size = new System.Drawing.Size(245, 22);
      this.newToolStripMenuItem.Text = "New";
      this.newToolStripMenuItem.Click += new System.EventHandler(this.onNew);
      // 
      // openToolStripMenuItem1
      // 
      this.openToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripMenuItem1.Image")));
      this.openToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.openToolStripMenuItem.Name = "openToolStripMenuItem1";
      this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
      this.openToolStripMenuItem.Size = new System.Drawing.Size(245, 22);
      this.openToolStripMenuItem.Text = "&Open";
      this.openToolStripMenuItem.Click += new System.EventHandler(this.onOpen);
      // 
      // partialOpenToolStripMenuItem
      // 
      this.partialOpenToolStripMenuItem.Name = "partialOpenToolStripMenuItem";
      this.partialOpenToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.R)));
      this.partialOpenToolStripMenuItem.Size = new System.Drawing.Size(245, 22);
      this.partialOpenToolStripMenuItem.Text = "Partial Open";
      this.partialOpenToolStripMenuItem.Click += new System.EventHandler(this.onPartialOpen);
      // 
      // recoverToolStripMenuItem
      // 
      this.recoverToolStripMenuItem.Name = "recoverToolStripMenuItem";
      this.recoverToolStripMenuItem.Size = new System.Drawing.Size(245, 22);
      this.recoverToolStripMenuItem.Text = "Recover";
      this.recoverToolStripMenuItem.Click += new System.EventHandler(this.onRecover);
      // 
      // toolStripSeparator1
      // 
      this.toolStripSeparator1.Name = "toolStripSeparator1";
      this.toolStripSeparator1.Size = new System.Drawing.Size(242, 6);
      // 
      // printPreviewMenuItem
      // 
      this.printPreviewMenuItem.Name = "printPreviewMenuItem";
      this.printPreviewMenuItem.Size = new System.Drawing.Size(245, 22);
      this.printPreviewMenuItem.Text = "Print Preview";
      this.printPreviewMenuItem.Click += new System.EventHandler(this.printPreviewMenuItem_Click);
      // 
      // printToolStripMenuItem
      // 
      this.printToolStripMenuItem.Name = "printToolStripMenuItem";
      this.printToolStripMenuItem.Size = new System.Drawing.Size(245, 22);
      this.printToolStripMenuItem.Text = "Print";
      this.printToolStripMenuItem.Click += new System.EventHandler(this.printToolStripMenuItem_Click);
      // 
      // pageSetupToolStripMenuItem
      // 
      this.pageSetupToolStripMenuItem.Name = "pageSetupToolStripMenuItem";
      this.pageSetupToolStripMenuItem.Size = new System.Drawing.Size(245, 22);
      this.pageSetupToolStripMenuItem.Text = "Page Setup";
      this.pageSetupToolStripMenuItem.Visible = false;
      this.pageSetupToolStripMenuItem.Click += new System.EventHandler(this.OnPageSetup);
      // 
      // toolStripSeparator4
      // 
      this.toolStripSeparator4.Name = "toolStripSeparator4";
      this.toolStripSeparator4.Size = new System.Drawing.Size(242, 6);
      // 
      // ExportSVG
      // 
      this.ExportSVG.Name = "ExportSVG";
      this.ExportSVG.Size = new System.Drawing.Size(245, 22);
      this.ExportSVG.Text = "Export to SVG";
      this.ExportSVG.Visible = false;
      this.ExportSVG.Click += new System.EventHandler(this.ExportSVG_Click);
      // 
      // ExportPDF
      // 
      this.ExportPDF.Name = "ExportPDF";
      this.ExportPDF.Size = new System.Drawing.Size(245, 22);
      this.ExportPDF.Text = "Export to PDF";
      this.ExportPDF.Visible = false;
      this.ExportPDF.Click += new System.EventHandler(this.ExportPDF_Click);
      // 
      // ExportDWF
      // 
      this.ExportDWF.Name = "ExportDWF";
      this.ExportDWF.Size = new System.Drawing.Size(245, 22);
      this.ExportDWF.Text = "Export to DWF";
      this.ExportDWF.Visible = false;
      this.ExportDWF.Click += new System.EventHandler(this.ExportDWF_Click);
      // 
      // ImportDWF
      // 
      this.ImportDWF.Name = "ImportDWF";
      this.ImportDWF.Size = new System.Drawing.Size(245, 22);
      this.ImportDWF.Text = "Import DWF In Current Database";
      this.ImportDWF.Visible = false;
      this.ImportDWF.Click += new System.EventHandler(this.ImportDWF_Click);
      // 
      // ImportDGN
      // 
      this.ImportDGN.Name = "ImportDGN";
      this.ImportDGN.Size = new System.Drawing.Size(245, 22);
      this.ImportDGN.Text = "Import DGN As New Database";
      this.ImportDGN.Click += new System.EventHandler(this.ImportDGN_Click);
      // 
      // ImportPDF
      // 
      this.ImportPDF.Name = "ImportPDF";
      this.ImportPDF.Size = new System.Drawing.Size(245, 22);
      this.ImportPDF.Text = "Import PDF As New Database";
      this.ImportPDF.Click += new System.EventHandler(this.ImportPDF_Click);
      // 
      // ImportAsNewDatabase
      // 
      this.ImportAsNewDatabase.Name = "ImportAsNewDatabase";
      this.ImportAsNewDatabase.Size = new System.Drawing.Size(245, 22);
      this.ImportAsNewDatabase.Text = "Import DWF As New Database";
      this.ImportAsNewDatabase.Click += new System.EventHandler(this.ImportDWF_AS_New_Database_Click);
      // 
      // toolStripSeparator3
      // 
      this.toolStripSeparator3.Name = "toolStripSeparator3";
      this.toolStripSeparator3.Size = new System.Drawing.Size(242, 6);
      this.toolStripSeparator3.Visible = false;
      // 
      // exitToolStripMenuItem1
      // 
      this.exitToolStripMenuItem1.Name = "exitToolStripMenuItem1";
      this.exitToolStripMenuItem1.Size = new System.Drawing.Size(245, 22);
      this.exitToolStripMenuItem1.Text = "E&xit";
      // 
      // viewToolStripMenuItem
      // 
      this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolbarToolStripMenuItem,
            this.statusBarToolStripMenuItem,
            this.renderToolStripMenuItem});
      this.viewToolStripMenuItem.Name = "viewToolStripMenuItem";
      this.viewToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
      this.viewToolStripMenuItem.Text = "View";
      // 
      // toolbarToolStripMenuItem
      // 
      this.toolbarToolStripMenuItem.Checked = true;
      this.toolbarToolStripMenuItem.CheckOnClick = true;
      this.toolbarToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
      this.toolbarToolStripMenuItem.Name = "toolbarToolStripMenuItem";
      this.toolbarToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
      this.toolbarToolStripMenuItem.Text = "Toolbar";
      this.toolbarToolStripMenuItem.Click += new System.EventHandler(this.showHideToolbar);
      // 
      // statusBarToolStripMenuItem
      // 
      this.statusBarToolStripMenuItem.Checked = true;
      this.statusBarToolStripMenuItem.CheckOnClick = true;
      this.statusBarToolStripMenuItem.CheckState = System.Windows.Forms.CheckState.Checked;
      this.statusBarToolStripMenuItem.Name = "statusBarToolStripMenuItem";
      this.statusBarToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
      this.statusBarToolStripMenuItem.Text = "Status Bar";
      this.statusBarToolStripMenuItem.Click += new System.EventHandler(this.showHideStatusBar);
      // 
      // renderToolStripMenuItem
      // 
      this.renderToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.snapshotToolStripMenuItem,
            this.invertColorPaletteToolStripMenuItem});
      this.renderToolStripMenuItem.Name = "renderToolStripMenuItem";
      this.renderToolStripMenuItem.Size = new System.Drawing.Size(126, 22);
      this.renderToolStripMenuItem.Text = "Render";
      this.renderToolStripMenuItem.Visible = false;
      // 
      // snapshotToolStripMenuItem
      // 
      this.snapshotToolStripMenuItem.Enabled = false;
      this.snapshotToolStripMenuItem.Name = "snapshotToolStripMenuItem";
      this.snapshotToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
      this.snapshotToolStripMenuItem.Text = "Snapshot";
      this.snapshotToolStripMenuItem.Click += new System.EventHandler(this.snapshotToolStripMenuItem_Click);
      // 
      // invertColorPaletteToolStripMenuItem
      // 
      this.invertColorPaletteToolStripMenuItem.Enabled = false;
      this.invertColorPaletteToolStripMenuItem.Name = "invertColorPaletteToolStripMenuItem";
      this.invertColorPaletteToolStripMenuItem.Size = new System.Drawing.Size(173, 22);
      this.invertColorPaletteToolStripMenuItem.Text = "Invert color palette";
      this.invertColorPaletteToolStripMenuItem.Click += new System.EventHandler(this.invertColorPaletteToolStripMenuItem_Click);
      // 
      // toolsToolStripMenuItem1
      // 
      this.toolsToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadApps});
      this.toolsToolStripMenuItem1.Name = "toolsToolStripMenuItem1";
      this.toolsToolStripMenuItem1.Size = new System.Drawing.Size(46, 20);
      this.toolsToolStripMenuItem1.Text = "&Tools";
      // 
      // loadApps
      // 
      this.loadApps.Name = "loadApps";
      this.loadApps.Size = new System.Drawing.Size(169, 22);
      this.loadApps.Text = "Load Applications";
      this.loadApps.Click += new System.EventHandler(this.onLoadApps);
      // 
      // undoTestToolStripMenuItem
      // 
      this.undoTestToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.appendLinesToolStripMenuItem,
            this.undoLineAddingToolStripMenuItem,
            this.undovariant2ToolStripMenuItem,
            this.redoToolStripMenuItem});
      this.undoTestToolStripMenuItem.Name = "undoTestToolStripMenuItem";
      this.undoTestToolStripMenuItem.Size = new System.Drawing.Size(70, 20);
      this.undoTestToolStripMenuItem.Text = "Undo test";
      // 
      // appendLinesToolStripMenuItem
      // 
      this.appendLinesToolStripMenuItem.Name = "appendLinesToolStripMenuItem";
      this.appendLinesToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
      this.appendLinesToolStripMenuItem.Text = "Add 2 lines";
      this.appendLinesToolStripMenuItem.Click += new System.EventHandler(this.appendLinesToolStripMenuItem_Click);
      // 
      // undoLineAddingToolStripMenuItem
      // 
      this.undoLineAddingToolStripMenuItem.Name = "undoLineAddingToolStripMenuItem";
      this.undoLineAddingToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
      this.undoLineAddingToolStripMenuItem.Text = "Undo (variant 1)";
      this.undoLineAddingToolStripMenuItem.Click += new System.EventHandler(this.undovariant1ToolStripMenuItem_Click);
      // 
      // undovariant2ToolStripMenuItem
      // 
      this.undovariant2ToolStripMenuItem.Name = "undovariant2ToolStripMenuItem";
      this.undovariant2ToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
      this.undovariant2ToolStripMenuItem.Text = "Undo (variant 2)";
      this.undovariant2ToolStripMenuItem.Click += new System.EventHandler(this.undovariant2ToolStripMenuItem_Click);
      // 
      // redoToolStripMenuItem
      // 
      this.redoToolStripMenuItem.Enabled = false;
      this.redoToolStripMenuItem.Name = "redoToolStripMenuItem";
      this.redoToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
      this.redoToolStripMenuItem.Text = "Redo";
      this.redoToolStripMenuItem.Click += new System.EventHandler(this.redoToolStripMenuItem_Click);
      // 
      // helpToolStripMenuItem
      // 
      this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutOdaDwgAppToolStripMenuItem});
      this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
      this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
      this.helpToolStripMenuItem.Text = "Help";
      // 
      // aboutOdaDwgAppToolStripMenuItem
      // 
      this.aboutOdaDwgAppToolStripMenuItem.Name = "aboutOdaDwgAppToolStripMenuItem";
      this.aboutOdaDwgAppToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
      this.aboutOdaDwgAppToolStripMenuItem.Text = "About OdaDwgApp...";
      this.aboutOdaDwgAppToolStripMenuItem.Click += new System.EventHandler(this.onAboutOdaDwg);
      // 
      // insert_toolStripMenuItem
      // 
      this.insert_toolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.btnCreateCircle,
            this.btnCreateLine,
            this.btnCreateText});
      this.insert_toolStripMenuItem.Name = "insert_toolStripMenuItem";
      this.insert_toolStripMenuItem.Size = new System.Drawing.Size(48, 20);
      this.insert_toolStripMenuItem.Text = "Insert";
      // 
      // createCircle_CPointAndRadius_toolStripMenuItem
      // 
      this.btnCreateCircle.Name = "createCircle_CPointAndRadius_toolStripMenuItem";
      this.btnCreateCircle.Size = new System.Drawing.Size(250, 22);
      this.btnCreateCircle.Text = "Circle by Center Point and Radius";
      this.btnCreateCircle.Click += new System.EventHandler(this.createCircle_CPointAndRadius_toolStripMenuItem_Click);
      // 
      // create_line_ToolStripMenuItem
      // 
      this.btnCreateLine.Name = "create_line_ToolStripMenuItem";
      this.btnCreateLine.Size = new System.Drawing.Size(250, 22);
      this.btnCreateLine.Text = "Line";
      this.btnCreateLine.Click += new System.EventHandler(this.create_line_ToolStripMenuItem_Click);
      // 
      // createtext_ToolStripMenuItem
      // 
      this.btnCreateText.Name = "createtext_ToolStripMenuItem";
      this.btnCreateText.Size = new System.Drawing.Size(250, 22);
      this.btnCreateText.Text = "Text";
      this.btnCreateText.Click += new System.EventHandler(this.createtext_ToolStripMenuItem_Click);
      // 
      // openDwgDialog
      // 
      this.openDwgDialog.FileName = "openDwnDialog";
      this.openDwgDialog.Filter = "DWG files|*.dwg|DXF files|*.dxf";
      // 
      // statusStrip1
      // 
      this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
      this.statusStrip1.Location = new System.Drawing.Point(0, 359);
      this.statusStrip1.Name = "statusStrip1";
      this.statusStrip1.Size = new System.Drawing.Size(784, 22);
      this.statusStrip1.TabIndex = 3;
      this.statusStrip1.Text = "statusStrip1";
      // 
      // toolStripStatusLabel1
      // 
      this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
      this.toolStripStatusLabel1.Size = new System.Drawing.Size(39, 17);
      this.toolStripStatusLabel1.Text = "Ready";
      // 
      // toolStripSeparator2
      // 
      this.toolStripSeparator2.Name = "toolStripSeparator2";
      this.toolStripSeparator2.Size = new System.Drawing.Size(172, 6);
      // 
      // toolBar
      // 
      this.toolBar.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
      this.toolBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newDocBtn,
            this.openDocBtn,
            this.btnPan,
            this.btnRotation,
            this.btnZoomToExtents});
      this.toolBar.Location = new System.Drawing.Point(0, 24);
      this.toolBar.Name = "toolBar";
      this.toolBar.Size = new System.Drawing.Size(784, 25);
      this.toolBar.TabIndex = 5;
      this.toolBar.Text = "toolBar";
      // 
      // newDocBtn
      // 
      this.newDocBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.newDocBtn.Image = ((System.Drawing.Image)(resources.GetObject("newDocBtn.Image")));
      this.newDocBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.newDocBtn.Name = "newDocBtn";
      this.newDocBtn.Size = new System.Drawing.Size(23, 22);
      this.newDocBtn.Text = "New";
      this.newDocBtn.Click += new System.EventHandler(this.onNew);
      // 
      // openDocBtn
      // 
      this.openDocBtn.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.openDocBtn.Image = ((System.Drawing.Image)(resources.GetObject("openDocBtn.Image")));
      this.openDocBtn.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.openDocBtn.Name = "openDocBtn";
      this.openDocBtn.Size = new System.Drawing.Size(23, 22);
      this.openDocBtn.Text = "Open";
      this.openDocBtn.Click += new System.EventHandler(this.onOpen);
      // 
      // toolStripButtonPan
      // 
      this.btnPan.CheckOnClick = true;
      this.btnPan.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.btnPan.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonPan.Image")));
      this.btnPan.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btnPan.Name = "toolStripButtonPan";
      this.btnPan.Size = new System.Drawing.Size(31, 22);
      this.btnPan.Text = "Pan";
      this.btnPan.ToolTipText = "Pan";
      this.btnPan.CheckedChanged += new System.EventHandler(this.toolStripButtonPan_CheckedChanged);
      // 
      // toolStripButtonRotation
      // 
      this.btnRotation.CheckOnClick = true;
      this.btnRotation.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.btnRotation.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonRotation.Image")));
      this.btnRotation.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btnRotation.Name = "toolStripButtonRotation";
      this.btnRotation.Size = new System.Drawing.Size(56, 22);
      this.btnRotation.Text = "Rotation";
      this.btnRotation.CheckedChanged += new System.EventHandler(this.toolStripButtonRotation_CheckedChanged);
      // 
      // toolStripButtonZoomToExtents
      // 
      this.btnZoomToExtents.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.btnZoomToExtents.Image = ((System.Drawing.Image)(resources.GetObject("toolStripButtonZoomToExtents.Image")));
      this.btnZoomToExtents.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btnZoomToExtents.Name = "toolStripButtonZoomToExtents";
      this.btnZoomToExtents.Size = new System.Drawing.Size(99, 22);
      this.btnZoomToExtents.Text = "Zoom To Extents";
      this.btnZoomToExtents.Click += new System.EventHandler(this.zoomExtents_Click);
      // 
      // Form1
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(784, 381);
      this.Controls.Add(this.toolBar);
      this.Controls.Add(this.statusStrip1);
      this.Controls.Add(this.menuStrip1);
      this.IsMdiContainer = true;
      this.MainMenuStrip = this.menuStrip1;
      this.Name = "Form1";
      this.Text = "OdaDwgAppSwigMgd";
      this.menuStrip1.ResumeLayout(false);
      this.menuStrip1.PerformLayout();
      this.statusStrip1.ResumeLayout(false);
      this.statusStrip1.PerformLayout();
      this.toolBar.ResumeLayout(false);
      this.toolBar.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.MenuStrip menuStrip1;
    private System.Windows.Forms.OpenFileDialog openDwgDialog;
    private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem1;
    private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
    private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem1;
    private System.Windows.Forms.StatusStrip statusStrip1;
    private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
    private System.Windows.Forms.ToolStripMenuItem ExportSVG;
    private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem1;
    private System.Windows.Forms.ToolStripMenuItem ExportPDF;
    private System.Windows.Forms.ToolStripMenuItem ExportDWF;
    private System.Windows.Forms.ToolStripMenuItem ImportDWF;
    private System.Windows.Forms.ToolStripMenuItem ImportDGN;
    private System.Windows.Forms.ToolStripMenuItem ImportAsNewDatabase;
    //private System.Windows.Forms.ToolStripMenuItem Print;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
    private System.Windows.Forms.ToolStripMenuItem printToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem renderToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem snapshotToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem undoTestToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem appendLinesToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem undoLineAddingToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem undovariant2ToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem redoToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem partialOpenToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem recoverToolStripMenuItem;
		private System.Windows.Forms.ToolStrip toolBar;
		private System.Windows.Forms.ToolStripButton newDocBtn;
		private System.Windows.Forms.ToolStripButton openDocBtn;
		private System.Windows.Forms.ToolStripMenuItem toolbarToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem statusBarToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem loadApps;
		private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem aboutOdaDwgAppToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem pageSetupToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem invertColorPaletteToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem printPreviewMenuItem;
    private System.Windows.Forms.ToolStripMenuItem ImportPDF;
    private System.Windows.Forms.ToolStripMenuItem insert_toolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem btnCreateCircle;
    private System.Windows.Forms.ToolStripMenuItem btnCreateLine;
    private System.Windows.Forms.ToolStripMenuItem btnCreateText;
    private System.Windows.Forms.ToolStripButton btnPan;
    private System.Windows.Forms.ToolStripButton btnRotation;
    private System.Windows.Forms.ToolStripButton btnZoomToExtents;
    //private System.Windows.Forms.ToolStripMenuItem mTOpenToolStripMenuItem;
  }
}

