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

namespace OdaDwgAppSwigMgd
{
  partial class Tree
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
      this.treeView = new System.Windows.Forms.TreeView();
      this.menuStrip1 = new System.Windows.Forms.MenuStrip();
      this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.vectorizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.zoomExtentsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
      this.listView = new System.Windows.Forms.ListView();
      this.Groupcode = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
      this.Type = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
      this.Value = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
      this.menuStrip1.SuspendLayout();
      this.SuspendLayout();
      // 
      // treeView
      // 
      this.treeView.Location = new System.Drawing.Point(0, 102);
      this.treeView.Name = "treeView";
      this.treeView.Size = new System.Drawing.Size(196, 287);
      this.treeView.TabIndex = 0;
      // 
      // menuStrip1
      // 
      this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
      this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolsToolStripMenuItem});
      this.menuStrip1.Location = new System.Drawing.Point(62, 31);
      this.menuStrip1.Name = "menuStrip1";
      this.menuStrip1.Size = new System.Drawing.Size(55, 24);
      this.menuStrip1.TabIndex = 1;
      this.menuStrip1.Text = "menuStrip1";
      // 
      // toolsToolStripMenuItem
      // 
      this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.vectorizeToolStripMenuItem,
            this.zoomExtentsToolStripMenuItem});
      this.toolsToolStripMenuItem.MergeAction = System.Windows.Forms.MergeAction.MatchOnly;
      this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
      this.toolsToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
      this.toolsToolStripMenuItem.Text = "&Tools";
      // 
      // vectorizeToolStripMenuItem
      // 
      this.vectorizeToolStripMenuItem.Name = "vectorizeToolStripMenuItem";
      this.vectorizeToolStripMenuItem.Size = new System.Drawing.Size(143, 22);
      this.vectorizeToolStripMenuItem.Text = "Vectorize";
      this.vectorizeToolStripMenuItem.Click += new System.EventHandler(this.vectorizeToolStripMenuItem_Click);
      // 
      // zoomExtentsToolStripMenuItem
      // 
      this.zoomExtentsToolStripMenuItem.Name = "zoomExtentsToolStripMenuItem";
      this.zoomExtentsToolStripMenuItem.Size = new System.Drawing.Size(143, 22);
      this.zoomExtentsToolStripMenuItem.Text = "ZoomExtents";
      this.zoomExtentsToolStripMenuItem.Visible = false;
      this.zoomExtentsToolStripMenuItem.Click += new System.EventHandler(this.zoomExtentsToolStripMenuItem_Click);
      // 
      // listView
      // 
      this.listView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.Groupcode,
            this.Type,
            this.Value});
      this.listView.FullRowSelect = true;
      this.listView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
      this.listView.HideSelection = false;
      this.listView.Location = new System.Drawing.Point(239, 9);
      this.listView.MultiSelect = false;
      this.listView.Name = "listView";
      this.listView.ShowGroups = false;
      this.listView.Size = new System.Drawing.Size(297, 401);
      this.listView.TabIndex = 0;
      this.listView.UseCompatibleStateImageBehavior = false;
      this.listView.View = System.Windows.Forms.View.Details;
      // 
      // Groupcode
      // 
      this.Groupcode.Text = "Groupcode";
      this.Groupcode.Width = 65;
      // 
      // Type
      // 
      this.Type.Text = "Type";
      this.Type.Width = 90;
      // 
      // Value
      // 
      this.Value.Text = "Value";
      this.Value.Width = 300;
      // 
      // Tree
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.AutoSize = true;
      this.ClientSize = new System.Drawing.Size(548, 401);
      this.Controls.Add(this.treeView);
      this.Controls.Add(this.listView);
      this.Controls.Add(this.menuStrip1);
      this.Name = "Tree";
      this.Text = "Tree view";
      this.Activated += new System.EventHandler(this.Tree_Activated);
      this.menuStrip1.ResumeLayout(false);
      this.menuStrip1.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.TreeView treeView;
    private System.Windows.Forms.ListView listView;
    private System.Windows.Forms.MenuStrip menuStrip1;
    private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
    private System.Windows.Forms.ColumnHeader Groupcode;
    private System.Windows.Forms.ColumnHeader Type;
    private System.Windows.Forms.ToolStripMenuItem vectorizeToolStripMenuItem;
    private System.Windows.Forms.ToolStripMenuItem zoomExtentsToolStripMenuItem;
    private System.Windows.Forms.ColumnHeader Value;
  }
}