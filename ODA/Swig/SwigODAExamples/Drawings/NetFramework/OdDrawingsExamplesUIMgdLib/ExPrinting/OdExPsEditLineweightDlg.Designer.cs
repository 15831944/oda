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

namespace OdDrawingsExamplesUIMgdLib.ExPrinting
{
	partial class OdExPsEditLineweightDlg
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
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.radioInches = new System.Windows.Forms.RadioButton();
			this.radioMM = new System.Windows.Forms.RadioButton();
			this.buttonEditLW = new System.Windows.Forms.Button();
			this.buttonSortLW = new System.Windows.Forms.Button();
			this.button3 = new System.Windows.Forms.Button();
			this.button4 = new System.Windows.Forms.Button();
			this.listView1 = new System.Windows.Forms.ListView();
			this.columnValue = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnInUse = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(5, 5);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(66, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Lineweights:";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.radioInches);
			this.groupBox1.Controls.Add(this.radioMM);
			this.groupBox1.Location = new System.Drawing.Point(155, 5);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(126, 78);
			this.groupBox1.TabIndex = 2;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Units For Listing";
			// 
			// radioInches
			// 
			this.radioInches.AutoSize = true;
			this.radioInches.Location = new System.Drawing.Point(7, 47);
			this.radioInches.Name = "radioInches";
			this.radioInches.Size = new System.Drawing.Size(74, 17);
			this.radioInches.TabIndex = 1;
			this.radioInches.Text = "Inches (in)";
			this.radioInches.UseVisualStyleBackColor = true;
			// 
			// radioMM
			// 
			this.radioMM.AutoSize = true;
			this.radioMM.Location = new System.Drawing.Point(7, 24);
			this.radioMM.Name = "radioMM";
			this.radioMM.Size = new System.Drawing.Size(98, 17);
			this.radioMM.TabIndex = 0;
			this.radioMM.Text = "Millimeters (mm)";
			this.radioMM.UseVisualStyleBackColor = true;
			// 
			// buttonEditLW
			// 
			this.buttonEditLW.Location = new System.Drawing.Point(155, 89);
			this.buttonEditLW.Name = "buttonEditLW";
			this.buttonEditLW.Size = new System.Drawing.Size(126, 23);
			this.buttonEditLW.TabIndex = 3;
			this.buttonEditLW.Text = "Edit Lineweight";
			this.buttonEditLW.UseVisualStyleBackColor = true;
			// 
			// buttonSortLW
			// 
			this.buttonSortLW.Location = new System.Drawing.Point(155, 118);
			this.buttonSortLW.Name = "buttonSortLW";
			this.buttonSortLW.Size = new System.Drawing.Size(126, 23);
			this.buttonSortLW.TabIndex = 4;
			this.buttonSortLW.Text = "Sort Lineweights";
			this.buttonSortLW.UseVisualStyleBackColor = true;
			// 
			// button3
			// 
			this.button3.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.button3.Location = new System.Drawing.Point(102, 147);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(80, 23);
			this.button3.TabIndex = 5;
			this.button3.Text = "OK";
			this.button3.UseVisualStyleBackColor = true;
			// 
			// button4
			// 
			this.button4.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.button4.Location = new System.Drawing.Point(201, 147);
			this.button4.Name = "button4";
			this.button4.Size = new System.Drawing.Size(80, 23);
			this.button4.TabIndex = 6;
			this.button4.Text = "Cancel";
			this.button4.UseVisualStyleBackColor = true;
			// 
			// listView1
			// 
			this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnValue,
            this.columnInUse});
			this.listView1.Location = new System.Drawing.Point(8, 21);
			this.listView1.Name = "listView1";
			this.listView1.Size = new System.Drawing.Size(141, 120);
			this.listView1.TabIndex = 7;
			this.listView1.UseCompatibleStateImageBehavior = false;
			this.listView1.View = System.Windows.Forms.View.Details;
			// 
			// columnValue
			// 
			this.columnValue.Text = "Value";
			// 
			// columnInUse
			// 
			this.columnInUse.Text = "In Use";
			// 
			// PsEditLineweightDlg
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 172);
			this.Controls.Add(this.listView1);
			this.Controls.Add(this.button4);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.buttonSortLW);
			this.Controls.Add(this.buttonEditLW);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.label1);
			this.Name = "PsEditLineweightDlg";
			this.Text = "Edit Lineweights";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton radioInches;
		private System.Windows.Forms.RadioButton radioMM;
		private System.Windows.Forms.Button buttonEditLW;
		private System.Windows.Forms.Button buttonSortLW;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.Button button4;
		private System.Windows.Forms.ListView listView1;
		private System.Windows.Forms.ColumnHeader columnValue;
		private System.Windows.Forms.ColumnHeader columnInUse;
	}
}