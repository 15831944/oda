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

namespace OdExGUIComponentsMgdLib
{
	partial class OdExNewFileDlg
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
			this.rbtnEnglish = new System.Windows.Forms.RadioButton();
			this.rbtnMetric = new System.Windows.Forms.RadioButton();
			this.groupBoxMeasure = new System.Windows.Forms.GroupBox();
			this.btnOK = new System.Windows.Forms.Button();
			this.btnCancel = new System.Windows.Forms.Button();
			this.groupBoxMeasure.SuspendLayout();
			this.SuspendLayout();
			// 
			// rbtnEnglish
			// 
			this.rbtnEnglish.AutoSize = true;
			this.rbtnEnglish.Checked = true;
			this.rbtnEnglish.Location = new System.Drawing.Point(21, 25);
			this.rbtnEnglish.Name = "rbtnEnglish";
			this.rbtnEnglish.Size = new System.Drawing.Size(59, 17);
			this.rbtnEnglish.TabIndex = 0;
			this.rbtnEnglish.TabStop = true;
			this.rbtnEnglish.Text = "English";
			this.rbtnEnglish.UseVisualStyleBackColor = true;
			// 
			// rbtnMetric
			// 
			this.rbtnMetric.AutoSize = true;
			this.rbtnMetric.Location = new System.Drawing.Point(21, 56);
			this.rbtnMetric.Name = "rbtnMetric";
			this.rbtnMetric.Size = new System.Drawing.Size(54, 17);
			this.rbtnMetric.TabIndex = 1;
			this.rbtnMetric.Text = "Metric";
			this.rbtnMetric.UseVisualStyleBackColor = true;
			// 
			// groupBoxMeasure
			// 
			this.groupBoxMeasure.Controls.Add(this.rbtnMetric);
			this.groupBoxMeasure.Controls.Add(this.rbtnEnglish);
			this.groupBoxMeasure.Location = new System.Drawing.Point(11, 6);
			this.groupBoxMeasure.Name = "groupBoxMeasure";
			this.groupBoxMeasure.Size = new System.Drawing.Size(260, 85);
			this.groupBoxMeasure.TabIndex = 2;
			this.groupBoxMeasure.TabStop = false;
			this.groupBoxMeasure.Text = "Select default setting";
			// 
			// btnOK
			// 
			this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btnOK.Location = new System.Drawing.Point(30, 95);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(80, 23);
			this.btnOK.TabIndex = 3;
			this.btnOK.Text = "OK";
			this.btnOK.UseVisualStyleBackColor = true;
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// btnCancel
			// 
			this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnCancel.Location = new System.Drawing.Point(170, 95);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(80, 23);
			this.btnCancel.TabIndex = 4;
			this.btnCancel.Text = "Cancel";
			this.btnCancel.UseVisualStyleBackColor = true;
			// 
			// NewFileDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 121);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.groupBoxMeasure);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
			this.Name = "NewFileDlg";
			this.Text = "Create new drawing";
			this.groupBoxMeasure.ResumeLayout(false);
			this.groupBoxMeasure.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.RadioButton rbtnEnglish;
		private System.Windows.Forms.RadioButton rbtnMetric;
		private System.Windows.Forms.GroupBox groupBoxMeasure;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.Button btnCancel;
	}
}