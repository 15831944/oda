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

using OdKernelExamplesMgdLib.Export.Pdf;

namespace OdKernelExamplesUIMgdLib.Export.Pdf
{
  partial class OdExport2PdfForm<TPdfExportParamsFiller>
        where TPdfExportParamsFiller : OdExPdfExportParamsFiller, new()
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
      this.PdfFilePathString = new System.Windows.Forms.TextBox();
      this.Browse = new System.Windows.Forms.Button();
      this.Properties = new System.Windows.Forms.GroupBox();
      this.btnUsePrcAsBRep = new System.Windows.Forms.CheckBox();
      this.btnUsePrc = new System.Windows.Forms.CheckBox();
      this.textBox2 = new System.Windows.Forms.TextBox();
      this.label11 = new System.Windows.Forms.Label();
      this.textBox1 = new System.Windows.Forms.TextBox();
      this.label10 = new System.Windows.Forms.Label();
      this.ExportHyperlinks1 = new System.Windows.Forms.CheckBox();
      this.CreatorString = new System.Windows.Forms.TextBox();
      this.ProducerString = new System.Windows.Forms.TextBox();
      this.SubjectString = new System.Windows.Forms.TextBox();
      this.AuthorString = new System.Windows.Forms.TextBox();
      this.TitleString = new System.Windows.Forms.TextBox();
      this.KeywordsString = new System.Windows.Forms.TextBox();
      this.label9 = new System.Windows.Forms.Label();
      this.label8 = new System.Windows.Forms.Label();
      this.label7 = new System.Windows.Forms.Label();
      this.label6 = new System.Windows.Forms.Label();
      this.label5 = new System.Windows.Forms.Label();
      this.label4 = new System.Windows.Forms.Label();
      this.All = new System.Windows.Forms.RadioButton();
      this.Active = new System.Windows.Forms.RadioButton();
      this.PaperHeightString = new System.Windows.Forms.TextBox();
      this.PaperWidthString = new System.Windows.Forms.TextBox();
      this.label3 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.label1 = new System.Windows.Forms.Label();
      this.Encoded_small = new System.Windows.Forms.CheckBox();
      this.Off_layers = new System.Windows.Forms.CheckBox();
      this.Layer_support = new System.Windows.Forms.CheckBox();
      this.Hidden_line = new System.Windows.Forms.CheckBox();
      this.Zoom_extents = new System.Windows.Forms.CheckBox();
      this.Enable_optimization = new System.Windows.Forms.CheckBox();
      this.SHX_text = new System.Windows.Forms.CheckBox();
      this.True_type = new System.Windows.Forms.CheckBox();
      this.Embedded_fonts = new System.Windows.Forms.CheckBox();
      this.Export = new System.Windows.Forms.Button();
      this.Cancel = new System.Windows.Forms.Button();
      this.btnExportMultipleDatabases = new System.Windows.Forms.CheckBox();
      this.Properties.SuspendLayout();
      this.SuspendLayout();
      // 
      // PdfFilePathString
      // 
      this.PdfFilePathString.Location = new System.Drawing.Point(12, 24);
      this.PdfFilePathString.Name = "PdfFilePathString";
      this.PdfFilePathString.ReadOnly = true;
      this.PdfFilePathString.Size = new System.Drawing.Size(509, 20);
      this.PdfFilePathString.TabIndex = 0;
      // 
      // Browse
      // 
      this.Browse.Location = new System.Drawing.Point(527, 22);
      this.Browse.Name = "Browse";
      this.Browse.Size = new System.Drawing.Size(75, 23);
      this.Browse.TabIndex = 1;
      this.Browse.Text = "Browse";
      this.Browse.UseVisualStyleBackColor = true;
      this.Browse.Click += new System.EventHandler(this.Browse_Click);
      // 
      // Properties
      // 
      this.Properties.Controls.Add(this.btnExportMultipleDatabases);
      this.Properties.Controls.Add(this.btnUsePrcAsBRep);
      this.Properties.Controls.Add(this.btnUsePrc);
      this.Properties.Controls.Add(this.textBox2);
      this.Properties.Controls.Add(this.label11);
      this.Properties.Controls.Add(this.textBox1);
      this.Properties.Controls.Add(this.label10);
      this.Properties.Controls.Add(this.ExportHyperlinks1);
      this.Properties.Controls.Add(this.CreatorString);
      this.Properties.Controls.Add(this.ProducerString);
      this.Properties.Controls.Add(this.SubjectString);
      this.Properties.Controls.Add(this.AuthorString);
      this.Properties.Controls.Add(this.TitleString);
      this.Properties.Controls.Add(this.KeywordsString);
      this.Properties.Controls.Add(this.label9);
      this.Properties.Controls.Add(this.label8);
      this.Properties.Controls.Add(this.label7);
      this.Properties.Controls.Add(this.label6);
      this.Properties.Controls.Add(this.label5);
      this.Properties.Controls.Add(this.label4);
      this.Properties.Controls.Add(this.All);
      this.Properties.Controls.Add(this.Active);
      this.Properties.Controls.Add(this.PaperHeightString);
      this.Properties.Controls.Add(this.PaperWidthString);
      this.Properties.Controls.Add(this.label3);
      this.Properties.Controls.Add(this.label2);
      this.Properties.Controls.Add(this.label1);
      this.Properties.Controls.Add(this.Encoded_small);
      this.Properties.Controls.Add(this.Off_layers);
      this.Properties.Controls.Add(this.Layer_support);
      this.Properties.Controls.Add(this.Hidden_line);
      this.Properties.Controls.Add(this.Zoom_extents);
      this.Properties.Controls.Add(this.Enable_optimization);
      this.Properties.Controls.Add(this.SHX_text);
      this.Properties.Controls.Add(this.True_type);
      this.Properties.Controls.Add(this.Embedded_fonts);
      this.Properties.Location = new System.Drawing.Point(12, 50);
      this.Properties.Name = "Properties";
      this.Properties.Size = new System.Drawing.Size(590, 470);
      this.Properties.TabIndex = 2;
      this.Properties.TabStop = false;
      this.Properties.Text = "Properties";
      // 
      // btnUsePrcAsBRep
      // 
      this.btnUsePrcAsBRep.AutoSize = true;
      this.btnUsePrcAsBRep.Location = new System.Drawing.Point(76, 229);
      this.btnUsePrcAsBRep.Name = "btnUsePrcAsBRep";
      this.btnUsePrcAsBRep.Size = new System.Drawing.Size(166, 17);
      this.btnUsePrcAsBRep.TabIndex = 34;
      this.btnUsePrcAsBRep.Text = "As BRep (otherwise As Mesh)";
      this.btnUsePrcAsBRep.UseVisualStyleBackColor = true;
      // 
      // btnUsePrc
      // 
      this.btnUsePrc.AutoSize = true;
      this.btnUsePrc.Location = new System.Drawing.Point(6, 230);
      this.btnUsePrc.Name = "btnUsePrc";
      this.btnUsePrc.Size = new System.Drawing.Size(64, 17);
      this.btnUsePrc.TabIndex = 33;
      this.btnUsePrc.Text = "Use Prc";
      this.btnUsePrc.UseVisualStyleBackColor = true;
      // 
      // textBox2
      // 
      this.textBox2.Location = new System.Drawing.Point(402, 115);
      this.textBox2.Name = "textBox2";
      this.textBox2.Size = new System.Drawing.Size(100, 20);
      this.textBox2.TabIndex = 32;
      this.textBox2.Text = "600";
      // 
      // label11
      // 
      this.label11.AutoSize = true;
      this.label11.Location = new System.Drawing.Point(296, 117);
      this.label11.Name = "label11";
      this.label11.Size = new System.Drawing.Size(88, 13);
      this.label11.TabIndex = 31;
      this.label11.Text = "Paper width, mm:";
      // 
      // textBox1
      // 
      this.textBox1.Location = new System.Drawing.Point(402, 89);
      this.textBox1.Name = "textBox1";
      this.textBox1.Size = new System.Drawing.Size(100, 20);
      this.textBox1.TabIndex = 30;
      this.textBox1.Text = "150";
      // 
      // label10
      // 
      this.label10.AutoSize = true;
      this.label10.Location = new System.Drawing.Point(296, 95);
      this.label10.Name = "label10";
      this.label10.Size = new System.Drawing.Size(97, 13);
      this.label10.TabIndex = 29;
      this.label10.Text = "Hatch to BMP, dpi:";
      // 
      // ExportHyperlinks1
      // 
      this.ExportHyperlinks1.AutoSize = true;
      this.ExportHyperlinks1.Location = new System.Drawing.Point(6, 206);
      this.ExportHyperlinks1.Name = "ExportHyperlinks1";
      this.ExportHyperlinks1.Size = new System.Drawing.Size(108, 17);
      this.ExportHyperlinks1.TabIndex = 28;
      this.ExportHyperlinks1.Text = "Export Hyperlinks";
      this.ExportHyperlinks1.UseVisualStyleBackColor = true;
      // 
      // CreatorString
      // 
      this.CreatorString.Location = new System.Drawing.Point(65, 414);
      this.CreatorString.Name = "CreatorString";
      this.CreatorString.Size = new System.Drawing.Size(519, 20);
      this.CreatorString.TabIndex = 27;
      // 
      // ProducerString
      // 
      this.ProducerString.Location = new System.Drawing.Point(65, 439);
      this.ProducerString.Name = "ProducerString";
      this.ProducerString.Size = new System.Drawing.Size(519, 20);
      this.ProducerString.TabIndex = 26;
      // 
      // SubjectString
      // 
      this.SubjectString.Location = new System.Drawing.Point(65, 364);
      this.SubjectString.Name = "SubjectString";
      this.SubjectString.Size = new System.Drawing.Size(519, 20);
      this.SubjectString.TabIndex = 25;
      // 
      // AuthorString
      // 
      this.AuthorString.Location = new System.Drawing.Point(65, 339);
      this.AuthorString.Name = "AuthorString";
      this.AuthorString.Size = new System.Drawing.Size(519, 20);
      this.AuthorString.TabIndex = 24;
      // 
      // TitleString
      // 
      this.TitleString.Location = new System.Drawing.Point(65, 314);
      this.TitleString.Name = "TitleString";
      this.TitleString.Size = new System.Drawing.Size(519, 20);
      this.TitleString.TabIndex = 23;
      // 
      // KeywordsString
      // 
      this.KeywordsString.Location = new System.Drawing.Point(65, 389);
      this.KeywordsString.Name = "KeywordsString";
      this.KeywordsString.Size = new System.Drawing.Size(519, 20);
      this.KeywordsString.TabIndex = 22;
      // 
      // label9
      // 
      this.label9.AutoSize = true;
      this.label9.Location = new System.Drawing.Point(3, 442);
      this.label9.Name = "label9";
      this.label9.Size = new System.Drawing.Size(53, 13);
      this.label9.TabIndex = 21;
      this.label9.Text = "Producer:";
      // 
      // label8
      // 
      this.label8.AutoSize = true;
      this.label8.Location = new System.Drawing.Point(3, 417);
      this.label8.Name = "label8";
      this.label8.Size = new System.Drawing.Size(44, 13);
      this.label8.TabIndex = 20;
      this.label8.Text = "Creator:";
      // 
      // label7
      // 
      this.label7.AutoSize = true;
      this.label7.Location = new System.Drawing.Point(3, 392);
      this.label7.Name = "label7";
      this.label7.Size = new System.Drawing.Size(56, 13);
      this.label7.TabIndex = 19;
      this.label7.Text = "Keywords:";
      // 
      // label6
      // 
      this.label6.AutoSize = true;
      this.label6.Location = new System.Drawing.Point(3, 367);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(46, 13);
      this.label6.TabIndex = 18;
      this.label6.Text = "Subject:";
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(3, 342);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(41, 13);
      this.label5.TabIndex = 17;
      this.label5.Text = "Author:";
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(3, 317);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(30, 13);
      this.label4.TabIndex = 16;
      this.label4.Text = "Title:";
      // 
      // All
      // 
      this.All.AutoSize = true;
      this.All.Location = new System.Drawing.Point(390, 184);
      this.All.Name = "All";
      this.All.Size = new System.Drawing.Size(36, 17);
      this.All.TabIndex = 15;
      this.All.Text = "All";
      this.All.UseVisualStyleBackColor = true;
      // 
      // Active
      // 
      this.Active.AutoSize = true;
      this.Active.Checked = true;
      this.Active.Location = new System.Drawing.Point(390, 159);
      this.Active.Name = "Active";
      this.Active.Size = new System.Drawing.Size(55, 17);
      this.Active.TabIndex = 14;
      this.Active.TabStop = true;
      this.Active.Text = "Active";
      this.Active.UseVisualStyleBackColor = true;
      // 
      // PaperHeightString
      // 
      this.PaperHeightString.Location = new System.Drawing.Point(403, 68);
      this.PaperHeightString.Name = "PaperHeightString";
      this.PaperHeightString.Size = new System.Drawing.Size(100, 20);
      this.PaperHeightString.TabIndex = 13;
      this.PaperHeightString.Text = "297";
      // 
      // PaperWidthString
      // 
      this.PaperWidthString.Location = new System.Drawing.Point(403, 42);
      this.PaperWidthString.Name = "PaperWidthString";
      this.PaperWidthString.Size = new System.Drawing.Size(100, 20);
      this.PaperWidthString.TabIndex = 12;
      this.PaperWidthString.Text = "210";
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(296, 161);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(91, 13);
      this.label3.TabIndex = 11;
      this.label3.Text = "Layouts to export:";
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(297, 72);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(92, 13);
      this.label2.TabIndex = 10;
      this.label2.Text = "Paper height, mm:";
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(297, 45);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(88, 13);
      this.label1.TabIndex = 9;
      this.label1.Text = "Paper width, mm:";
      // 
      // Encoded_small
      // 
      this.Encoded_small.AutoSize = true;
      this.Encoded_small.Checked = true;
      this.Encoded_small.CheckState = System.Windows.Forms.CheckState.Checked;
      this.Encoded_small.Location = new System.Drawing.Point(299, 230);
      this.Encoded_small.Name = "Encoded_small";
      this.Encoded_small.Size = new System.Drawing.Size(122, 17);
      this.Encoded_small.TabIndex = 8;
      this.Encoded_small.Text = "Encoded (small size)";
      this.Encoded_small.UseVisualStyleBackColor = true;
      // 
      // Off_layers
      // 
      this.Off_layers.AutoSize = true;
      this.Off_layers.Enabled = false;
      this.Off_layers.Location = new System.Drawing.Point(6, 182);
      this.Off_layers.Name = "Off_layers";
      this.Off_layers.Size = new System.Drawing.Size(121, 17);
      this.Off_layers.TabIndex = 7;
      this.Off_layers.Text = "Export Off layers too";
      this.Off_layers.UseVisualStyleBackColor = true;
      // 
      // Layer_support
      // 
      this.Layer_support.AutoSize = true;
      this.Layer_support.Location = new System.Drawing.Point(6, 159);
      this.Layer_support.Name = "Layer_support";
      this.Layer_support.Size = new System.Drawing.Size(173, 17);
      this.Layer_support.TabIndex = 6;
      this.Layer_support.Text = "Enable layer support (pdf v 1.5)";
      this.Layer_support.UseVisualStyleBackColor = true;
      // 
      // Hidden_line
      // 
      this.Hidden_line.AutoSize = true;
      this.Hidden_line.Checked = true;
      this.Hidden_line.CheckState = System.Windows.Forms.CheckState.Checked;
      this.Hidden_line.Location = new System.Drawing.Point(6, 136);
      this.Hidden_line.Name = "Hidden_line";
      this.Hidden_line.Size = new System.Drawing.Size(184, 17);
      this.Hidden_line.TabIndex = 5;
      this.Hidden_line.Text = "Use hidden line removal algorithm";
      this.Hidden_line.UseVisualStyleBackColor = true;
      // 
      // Zoom_extents
      // 
      this.Zoom_extents.AutoSize = true;
      this.Zoom_extents.Checked = true;
      this.Zoom_extents.CheckState = System.Windows.Forms.CheckState.Checked;
      this.Zoom_extents.Location = new System.Drawing.Point(6, 113);
      this.Zoom_extents.Name = "Zoom_extents";
      this.Zoom_extents.Size = new System.Drawing.Size(131, 17);
      this.Zoom_extents.TabIndex = 4;
      this.Zoom_extents.Text = "Zoom to extents mode";
      this.Zoom_extents.UseVisualStyleBackColor = true;
      // 
      // Enable_optimization
      // 
      this.Enable_optimization.AutoSize = true;
      this.Enable_optimization.Location = new System.Drawing.Point(6, 90);
      this.Enable_optimization.Name = "Enable_optimization";
      this.Enable_optimization.Size = new System.Drawing.Size(195, 17);
      this.Enable_optimization.TabIndex = 3;
      this.Enable_optimization.Text = "Enable simple geometry optimization";
      this.Enable_optimization.UseVisualStyleBackColor = true;
      // 
      // SHX_text
      // 
      this.SHX_text.AutoSize = true;
      this.SHX_text.Checked = true;
      this.SHX_text.CheckState = System.Windows.Forms.CheckState.Checked;
      this.SHX_text.Location = new System.Drawing.Point(6, 67);
      this.SHX_text.Name = "SHX_text";
      this.SHX_text.Size = new System.Drawing.Size(128, 17);
      this.SHX_text.TabIndex = 2;
      this.SHX_text.Text = "SHX text as geometry";
      this.SHX_text.UseVisualStyleBackColor = true;
      // 
      // True_type
      // 
      this.True_type.AutoSize = true;
      this.True_type.Checked = true;
      this.True_type.CheckState = System.Windows.Forms.CheckState.Checked;
      this.True_type.Location = new System.Drawing.Point(6, 44);
      this.True_type.Name = "True_type";
      this.True_type.Size = new System.Drawing.Size(131, 17);
      this.True_type.TabIndex = 1;
      this.True_type.Text = "True type as geometry";
      this.True_type.UseVisualStyleBackColor = true;
      // 
      // Embedded_fonts
      // 
      this.Embedded_fonts.AutoSize = true;
      this.Embedded_fonts.Location = new System.Drawing.Point(6, 21);
      this.Embedded_fonts.Name = "Embedded_fonts";
      this.Embedded_fonts.Size = new System.Drawing.Size(103, 17);
      this.Embedded_fonts.TabIndex = 0;
      this.Embedded_fonts.Text = "Embedded fonts";
      this.Embedded_fonts.UseVisualStyleBackColor = true;
      // 
      // Export
      // 
      this.Export.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.Export.Enabled = false;
      this.Export.Location = new System.Drawing.Point(127, 526);
      this.Export.Name = "Export";
      this.Export.Size = new System.Drawing.Size(75, 23);
      this.Export.TabIndex = 3;
      this.Export.Text = "Export";
      this.Export.UseVisualStyleBackColor = true;
      this.Export.Click += new System.EventHandler(this.Export_Click);
      // 
      // Cancel
      // 
      this.Cancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this.Cancel.Location = new System.Drawing.Point(359, 526);
      this.Cancel.Name = "Cancel";
      this.Cancel.Size = new System.Drawing.Size(75, 23);
      this.Cancel.TabIndex = 4;
      this.Cancel.Text = "Cancel";
      this.Cancel.UseVisualStyleBackColor = true;
      // 
      // btnExportMultipleDatabases
      // 
      this.btnExportMultipleDatabases.AutoSize = true;
      this.btnExportMultipleDatabases.Location = new System.Drawing.Point(6, 253);
      this.btnExportMultipleDatabases.Name = "btnExportMultipleDatabases";
      this.btnExportMultipleDatabases.Size = new System.Drawing.Size(149, 17);
      this.btnExportMultipleDatabases.TabIndex = 35;
      this.btnExportMultipleDatabases.Text = "Export Multiple Databases";
      this.btnExportMultipleDatabases.UseVisualStyleBackColor = true;
      // 
      // OdExport2PdfForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(619, 561);
      this.ControlBox = false;
      this.Controls.Add(this.Cancel);
      this.Controls.Add(this.Export);
      this.Controls.Add(this.Properties);
      this.Controls.Add(this.Browse);
      this.Controls.Add(this.PdfFilePathString);
      this.Name = "OdExport2PdfForm";
      this.Text = "Export2PdfForm";
      this.Shown += new System.EventHandler(this.Export2PdfForm_Shown);
      this.Properties.ResumeLayout(false);
      this.Properties.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.TextBox PdfFilePathString;
    private System.Windows.Forms.Button Browse;
    private System.Windows.Forms.GroupBox Properties;
    private System.Windows.Forms.CheckBox Off_layers;
    private System.Windows.Forms.CheckBox Layer_support;
    private System.Windows.Forms.CheckBox Hidden_line;
    private System.Windows.Forms.CheckBox Zoom_extents;
    private System.Windows.Forms.CheckBox Enable_optimization;
    private System.Windows.Forms.CheckBox SHX_text;
    private System.Windows.Forms.CheckBox True_type;
    private System.Windows.Forms.CheckBox Embedded_fonts;
    private System.Windows.Forms.TextBox PaperHeightString;
    private System.Windows.Forms.TextBox PaperWidthString;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.CheckBox Encoded_small;
    private System.Windows.Forms.Label label9;
    private System.Windows.Forms.Label label8;
    private System.Windows.Forms.Label label7;
    private System.Windows.Forms.Label label6;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.RadioButton All;
    private System.Windows.Forms.RadioButton Active;
    private System.Windows.Forms.TextBox CreatorString;
    private System.Windows.Forms.TextBox ProducerString;
    private System.Windows.Forms.TextBox SubjectString;
    private System.Windows.Forms.TextBox AuthorString;
    private System.Windows.Forms.TextBox TitleString;
    private System.Windows.Forms.TextBox KeywordsString;
    private System.Windows.Forms.Button Export;
    private System.Windows.Forms.Button Cancel;
    private System.Windows.Forms.CheckBox ExportHyperlinks1;
    private System.Windows.Forms.TextBox textBox2;
    private System.Windows.Forms.Label label11;
    private System.Windows.Forms.TextBox textBox1;
    private System.Windows.Forms.Label label10;
    private System.Windows.Forms.CheckBox btnUsePrc;
    private System.Windows.Forms.CheckBox btnUsePrcAsBRep;
    private System.Windows.Forms.CheckBox btnExportMultipleDatabases;
  }
}