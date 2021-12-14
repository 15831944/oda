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
namespace OdaDgnAppSwigMgd
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
      this.components = new System.ComponentModel.Container();
      System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
      this.menuStrip1 = new System.Windows.Forms.MenuStrip();
      this.fileToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
      this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.openToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
      this.pageSetupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
      this.ExportSTL = new System.Windows.Forms.ToolStripMenuItem();
      this.ExportSVG = new System.Windows.Forms.ToolStripMenuItem();
      this.ExportPDF = new System.Windows.Forms.ToolStripMenuItem();
      this.ExportDWF = new System.Windows.Forms.ToolStripMenuItem();
      this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
      this.exitToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
      this.viewToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolbarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.statusBarToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.toolsToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
      this.loadApplicationsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.openDgnDialog = new System.Windows.Forms.OpenFileDialog();
      this.statusStrip1 = new System.Windows.Forms.StatusStrip();
      this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
      this.toolBar = new System.Windows.Forms.ToolStrip();
      this.newButton = new System.Windows.Forms.ToolStripButton();
      this.openButton = new System.Windows.Forms.ToolStripButton();
      this.bindingSource1 = new System.Windows.Forms.BindingSource(this.components);
      this.btnPan = new System.Windows.Forms.ToolStripButton();
      this.btnRotation = new System.Windows.Forms.ToolStripButton();
      this.btnZoomToExtents = new System.Windows.Forms.ToolStripButton();
      this.menuStrip1.SuspendLayout();
      this.statusStrip1.SuspendLayout();
      this.toolBar.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).BeginInit();
      this.SuspendLayout();
      // 
      // menuStrip1
      // 
      this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem1,
            this.viewToolStripMenuItem,
            this.toolsToolStripMenuItem1});
      this.menuStrip1.Location = new System.Drawing.Point(0, 0);
      this.menuStrip1.Name = "menuStrip1";
      this.menuStrip1.Size = new System.Drawing.Size(807, 24);
      this.menuStrip1.TabIndex = 1;
      this.menuStrip1.Text = "menuStrip1";
      // 
      // fileToolStripMenuItem1
      // 
      this.fileToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem1,
            this.toolStripSeparator1,
            this.pageSetupToolStripMenuItem,
            this.toolStripSeparator2,
            this.ExportSTL,
            this.ExportSVG,
            this.ExportPDF,
            this.ExportDWF,
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
      this.newToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
      this.newToolStripMenuItem.Text = "New";
      this.newToolStripMenuItem.Click += new System.EventHandler(this.OnNew);
      // 
      // openToolStripMenuItem1
      // 
      this.openToolStripMenuItem1.Image = ((System.Drawing.Image)(resources.GetObject("openToolStripMenuItem1.Image")));
      this.openToolStripMenuItem1.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.openToolStripMenuItem1.Name = "openToolStripMenuItem1";
      this.openToolStripMenuItem1.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
      this.openToolStripMenuItem1.Size = new System.Drawing.Size(150, 22);
      this.openToolStripMenuItem1.Text = "&Open";
      this.openToolStripMenuItem1.Click += new System.EventHandler(this.OnOpen);
      // 
      // toolStripSeparator1
      // 
      this.toolStripSeparator1.Name = "toolStripSeparator1";
      this.toolStripSeparator1.Size = new System.Drawing.Size(147, 6);
      this.toolStripSeparator1.Visible = false;
      // 
      // pageSetupToolStripMenuItem
      // 
      this.pageSetupToolStripMenuItem.Name = "pageSetupToolStripMenuItem";
      this.pageSetupToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
      this.pageSetupToolStripMenuItem.Text = "Page Setup";
      this.pageSetupToolStripMenuItem.Visible = false;
      this.pageSetupToolStripMenuItem.Click += new System.EventHandler(this.OnPageSetup);
      // 
      // toolStripSeparator2
      // 
      this.toolStripSeparator2.Name = "toolStripSeparator2";
      this.toolStripSeparator2.Size = new System.Drawing.Size(147, 6);
      this.toolStripSeparator2.Visible = false;
      // 
      // ExportSTL
      // 
      this.ExportSTL.Name = "ExportSTL";
      this.ExportSTL.Size = new System.Drawing.Size(150, 22);
      this.ExportSTL.Text = "Export to STL";
      this.ExportSTL.Visible = false;
      this.ExportSTL.Click += new System.EventHandler(this.ExportSTL_Click);
      // 
      // ExportSVG
      // 
      this.ExportSVG.Name = "ExportSVG";
      this.ExportSVG.Size = new System.Drawing.Size(150, 22);
      this.ExportSVG.Text = "Export to SVG";
      this.ExportSVG.Visible = false;
      this.ExportSVG.Click += new System.EventHandler(this.ExportSVG_Click);
      // 
      // ExportPDF
      // 
      this.ExportPDF.Name = "ExportPDF";
      this.ExportPDF.Size = new System.Drawing.Size(150, 22);
      this.ExportPDF.Text = "Export to PDF";
      this.ExportPDF.Visible = false;
      this.ExportPDF.Click += new System.EventHandler(this.ExportPDF_Click);
      // 
      // ExportDWF
      // 
      this.ExportDWF.Name = "ExportDWF";
      this.ExportDWF.Size = new System.Drawing.Size(150, 22);
      this.ExportDWF.Text = "Export to DWF";
      this.ExportDWF.Visible = false;
      this.ExportDWF.Click += new System.EventHandler(this.ExportDWF_Click);
      // 
      // toolStripSeparator3
      // 
      this.toolStripSeparator3.Name = "toolStripSeparator3";
      this.toolStripSeparator3.Size = new System.Drawing.Size(147, 6);
      // 
      // exitToolStripMenuItem1
      // 
      this.exitToolStripMenuItem1.Name = "exitToolStripMenuItem1";
      this.exitToolStripMenuItem1.Size = new System.Drawing.Size(150, 22);
      this.exitToolStripMenuItem1.Text = "E&xit";
      // 
      // viewToolStripMenuItem
      // 
      this.viewToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolbarToolStripMenuItem,
            this.statusBarToolStripMenuItem});
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
      // toolsToolStripMenuItem1
      // 
      this.toolsToolStripMenuItem1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadApplicationsToolStripMenuItem});
      this.toolsToolStripMenuItem1.Name = "toolsToolStripMenuItem1";
      this.toolsToolStripMenuItem1.Size = new System.Drawing.Size(46, 20);
      this.toolsToolStripMenuItem1.Text = "&Tools";
      // 
      // loadApplicationsToolStripMenuItem
      // 
      this.loadApplicationsToolStripMenuItem.Name = "loadApplicationsToolStripMenuItem";
      this.loadApplicationsToolStripMenuItem.Size = new System.Drawing.Size(169, 22);
      this.loadApplicationsToolStripMenuItem.Text = "Load Applications";
      this.loadApplicationsToolStripMenuItem.Click += new System.EventHandler(this.loadApplicationsToolStripMenuItem_Click);
      // 
      // openDgnDialog
      // 
      this.openDgnDialog.FileName = "openDgnDialog";
      this.openDgnDialog.Filter = "DGN files|*.dgn";
      // 
      // statusStrip1
      // 
      this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
      this.statusStrip1.Location = new System.Drawing.Point(0, 408);
      this.statusStrip1.Name = "statusStrip1";
      this.statusStrip1.Size = new System.Drawing.Size(807, 22);
      this.statusStrip1.TabIndex = 3;
      this.statusStrip1.Text = "statusStrip1";
      // 
      // toolStripStatusLabel1
      // 
      this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
      this.toolStripStatusLabel1.Size = new System.Drawing.Size(39, 17);
      this.toolStripStatusLabel1.Text = "Ready";
      // 
      // toolBar
      // 
      this.toolBar.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
      this.toolBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newButton,
            this.openButton,
            this.btnPan,
            this.btnRotation,
            this.btnZoomToExtents});
      this.toolBar.Location = new System.Drawing.Point(0, 24);
      this.toolBar.Name = "toolBar";
      this.toolBar.Size = new System.Drawing.Size(807, 25);
      this.toolBar.TabIndex = 5;
      this.toolBar.Text = "toolbar";
      // 
      // newButton
      // 
      this.newButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.newButton.Image = ((System.Drawing.Image)(resources.GetObject("newButton.Image")));
      this.newButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.newButton.Name = "newButton";
      this.newButton.Size = new System.Drawing.Size(23, 22);
      this.newButton.Text = "toolStripButton2";
      this.newButton.ToolTipText = "New";
      this.newButton.Click += new System.EventHandler(this.OnNew);
      // 
      // openButton
      // 
      this.openButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
      this.openButton.Image = ((System.Drawing.Image)(resources.GetObject("openButton.Image")));
      this.openButton.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.openButton.Name = "openButton";
      this.openButton.Size = new System.Drawing.Size(23, 22);
      this.openButton.Text = "toolStripButton1";
      this.openButton.ToolTipText = "Open";
      this.openButton.Click += new System.EventHandler(this.OnOpen);
      // 
      // btnPan
      // 
      this.btnPan.CheckOnClick = true;
      this.btnPan.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.btnPan.Image = ((System.Drawing.Image)(resources.GetObject("btnPan.Image")));
      this.btnPan.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btnPan.Name = "btnPan";
      this.btnPan.Size = new System.Drawing.Size(31, 22);
      this.btnPan.Text = "Pan";
      this.btnPan.CheckedChanged += new System.EventHandler(this.btnPan_CheckedChanged);
      // 
      // btnRotation
      // 
      this.btnRotation.CheckOnClick = true;
      this.btnRotation.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.btnRotation.Image = ((System.Drawing.Image)(resources.GetObject("btnRotation.Image")));
      this.btnRotation.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btnRotation.Name = "btnRotation";
      this.btnRotation.Size = new System.Drawing.Size(56, 22);
      this.btnRotation.Text = "Rotation";
      this.btnRotation.ToolTipText = "Rotation";
      this.btnRotation.CheckedChanged += new System.EventHandler(this.btnRotation_CheckedChanged);
      // 
      // btnZoomToExtents
      // 
      this.btnZoomToExtents.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
      this.btnZoomToExtents.Image = ((System.Drawing.Image)(resources.GetObject("btnZoomToExtents.Image")));
      this.btnZoomToExtents.ImageTransparentColor = System.Drawing.Color.Magenta;
      this.btnZoomToExtents.Name = "btnZoomToExtents";
      this.btnZoomToExtents.Size = new System.Drawing.Size(99, 22);
      this.btnZoomToExtents.Text = "Zoom To Extents";
      this.btnZoomToExtents.Click += new System.EventHandler(this.btnZoomToExtents_Click);
      // 
      // Form1
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(807, 430);
      this.Controls.Add(this.toolBar);
      this.Controls.Add(this.statusStrip1);
      this.Controls.Add(this.menuStrip1);
      this.IsMdiContainer = true;
      this.MainMenuStrip = this.menuStrip1;
      this.Name = "Form1";
      this.Text = "OdaDgnAppSwig";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
      this.Load += new System.EventHandler(this.Form1_Load);
      this.menuStrip1.ResumeLayout(false);
      this.menuStrip1.PerformLayout();
      this.statusStrip1.ResumeLayout(false);
      this.statusStrip1.PerformLayout();
      this.toolBar.ResumeLayout(false);
      this.toolBar.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).EndInit();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.MenuStrip menuStrip1;
    private System.Windows.Forms.OpenFileDialog openDgnDialog;
    private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem1;
    private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem1;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
    private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem1;
    private System.Windows.Forms.StatusStrip statusStrip1;
    private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
    private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
    private System.Windows.Forms.ToolStripMenuItem ExportSVG;
    private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem1;
    private System.Windows.Forms.ToolStripMenuItem ExportPDF;
    private System.Windows.Forms.ToolStripMenuItem ExportDWF;
    private System.Windows.Forms.ToolStripMenuItem ExportSTL;
		private System.Windows.Forms.ToolStripMenuItem loadApplicationsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem viewToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem toolbarToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem statusBarToolStripMenuItem;
		private System.Windows.Forms.ToolStrip toolBar;
		private System.Windows.Forms.ToolStripButton openButton;
		private System.Windows.Forms.ToolStripButton newButton;
		private System.Windows.Forms.ToolStripMenuItem pageSetupToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.BindingSource bindingSource1;
    private System.Windows.Forms.ToolStripButton btnPan;
    private System.Windows.Forms.ToolStripButton btnRotation;
    private System.Windows.Forms.ToolStripButton btnZoomToExtents;
  }
}

