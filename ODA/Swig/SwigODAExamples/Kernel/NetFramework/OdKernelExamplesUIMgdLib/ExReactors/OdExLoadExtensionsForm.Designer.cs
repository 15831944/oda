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

namespace OdKernelExamplesUIMgdLib.ExReactors
{
	partial class OdExLoadExtensionsForm
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
			this.listViewApps = new System.Windows.Forms.ListView();
			this.columnApps = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.btnLoad = new System.Windows.Forms.Button();
			this.btnUnload = new System.Windows.Forms.Button();
			this.btnOK = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// listViewApps
			// 
			this.listViewApps.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnApps});
			this.listViewApps.Location = new System.Drawing.Point(3, 3);
			this.listViewApps.Name = "listViewApps";
			this.listViewApps.Size = new System.Drawing.Size(360, 185);
			this.listViewApps.Sorting = System.Windows.Forms.SortOrder.Ascending;
			this.listViewApps.TabIndex = 0;
			this.listViewApps.UseCompatibleStateImageBehavior = false;
			this.listViewApps.View = System.Windows.Forms.View.Details;
			this.listViewApps.SelectedIndexChanged += new System.EventHandler(this.listViewApps_SelectedIndexChanged);
			// 
			// columnApps
			// 
			this.columnApps.Text = "Loaded applications";
			this.columnApps.Width = 335;
			// 
			// btnLoad
			// 
			this.btnLoad.Location = new System.Drawing.Point(3, 194);
			this.btnLoad.Name = "btnLoad";
			this.btnLoad.Size = new System.Drawing.Size(85, 23);
			this.btnLoad.TabIndex = 1;
			this.btnLoad.Text = "Load";
			this.btnLoad.UseVisualStyleBackColor = true;
			this.btnLoad.Click += new System.EventHandler(this.OnLoadApp);
			// 
			// btnUnload
			// 
			this.btnUnload.Location = new System.Drawing.Point(99, 194);
			this.btnUnload.Name = "btnUnload";
			this.btnUnload.Size = new System.Drawing.Size(85, 23);
			this.btnUnload.TabIndex = 2;
			this.btnUnload.Text = "Unload";
			this.btnUnload.UseVisualStyleBackColor = true;
			this.btnUnload.Click += new System.EventHandler(this.OnUnloadApp);
			// 
			// btnOK
			// 
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.Location = new System.Drawing.Point(280, 194);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(85, 23);
			this.btnOK.TabIndex = 3;
			this.btnOK.Text = "OK";
			this.btnOK.UseVisualStyleBackColor = true;
			// 
			// LoadAppsDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(369, 222);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.btnUnload);
			this.Controls.Add(this.btnLoad);
			this.Controls.Add(this.listViewApps);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "LoadAppsDlg";
			this.Text = "Load / Unload Applications";
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ListView listViewApps;
		private System.Windows.Forms.Button btnLoad;
		private System.Windows.Forms.Button btnUnload;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.ColumnHeader columnApps;
	}
}