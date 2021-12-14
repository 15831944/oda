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

namespace OdDrawingsExamplesUIMgdLib
{
	partial class OdExAuditInfoDlg
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
			this.listViewSummary = new System.Windows.Forms.ListView();
			this.columnSummary = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.labelErrors = new System.Windows.Forms.Label();
			this.listViewErrors = new System.Windows.Forms.ListView();
			this.colName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.Value = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.Validation = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.DefaultValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.SuspendLayout();
			// 
			// listViewSummary
			// 
			this.listViewSummary.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.listViewSummary.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnSummary});
			this.listViewSummary.Location = new System.Drawing.Point(8, 12);
			this.listViewSummary.Name = "listViewSummary";
			this.listViewSummary.Size = new System.Drawing.Size(395, 86);
			this.listViewSummary.TabIndex = 0;
			this.listViewSummary.UseCompatibleStateImageBehavior = false;
			this.listViewSummary.View = System.Windows.Forms.View.Details;
			// 
			// columnSummary
			// 
			this.columnSummary.Text = "Summary";
			this.columnSummary.Width = 370;
			// 
			// labelErrors
			// 
			this.labelErrors.AutoSize = true;
			this.labelErrors.Location = new System.Drawing.Point(12, 101);
			this.labelErrors.Name = "labelErrors";
			this.labelErrors.Size = new System.Drawing.Size(37, 13);
			this.labelErrors.TabIndex = 2;
			this.labelErrors.Text = "Errors:";
			// 
			// listViewErrors
			// 
			this.listViewErrors.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.listViewErrors.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.colName,
            this.Value,
            this.Validation,
            this.DefaultValue});
			this.listViewErrors.Location = new System.Drawing.Point(8, 117);
			this.listViewErrors.Name = "listViewErrors";
			this.listViewErrors.Size = new System.Drawing.Size(395, 172);
			this.listViewErrors.TabIndex = 3;
			this.listViewErrors.UseCompatibleStateImageBehavior = false;
			this.listViewErrors.View = System.Windows.Forms.View.Details;
			// 
			// colName
			// 
			this.colName.Text = "Name";
			this.colName.Width = 100;
			// 
			// Value
			// 
			this.Value.Text = "Value";
			this.Value.Width = 100;
			// 
			// Validation
			// 
			this.Validation.Text = "Validation";
			// 
			// DefaultValue
			// 
			this.DefaultValue.Text = "Default Value";
			this.DefaultValue.Width = 100;
			// 
			// AuditInfoDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(410, 295);
			this.Controls.Add(this.listViewErrors);
			this.Controls.Add(this.labelErrors);
			this.Controls.Add(this.listViewSummary);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
			this.Name = "AuditInfoDlg";
			this.Text = "Recover Info";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ListView listViewSummary;
		private System.Windows.Forms.Label labelErrors;
		private System.Windows.Forms.ListView listViewErrors;
		private System.Windows.Forms.ColumnHeader colName;
		private System.Windows.Forms.ColumnHeader Value;
		private System.Windows.Forms.ColumnHeader Validation;
		private System.Windows.Forms.ColumnHeader DefaultValue;
		private System.Windows.Forms.ColumnHeader columnSummary;
	}
}