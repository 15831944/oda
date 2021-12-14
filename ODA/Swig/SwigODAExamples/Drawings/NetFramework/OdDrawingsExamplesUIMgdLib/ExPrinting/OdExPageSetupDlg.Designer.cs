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
	partial class OdExPageSetupDlg
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
      this.groupBoxPageSetup = new System.Windows.Forms.GroupBox();
      this.labelName = new System.Windows.Forms.Label();
      this.labelN = new System.Windows.Forms.Label();
      this.groupBoxPrinterPlotter = new System.Windows.Forms.GroupBox();
      this.labelDescription = new System.Windows.Forms.Label();
      this.labelWhere = new System.Windows.Forms.Label();
      this.labelPlotter = new System.Windows.Forms.Label();
      this.labelD = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      this.labelP = new System.Windows.Forms.Label();
      this.comboPlotDeviceName = new System.Windows.Forms.ComboBox();
      this.label2 = new System.Windows.Forms.Label();
      this.groupBoxPaperSize = new System.Windows.Forms.GroupBox();
      this.buttonAddMedia = new System.Windows.Forms.Button();
      this.comboPaperSize = new System.Windows.Forms.ComboBox();
      this.groupBoxPlotArea = new System.Windows.Forms.GroupBox();
      this.buttonWindow = new System.Windows.Forms.Button();
      this.comboViews = new System.Windows.Forms.ComboBox();
      this.comboAreaType = new System.Windows.Forms.ComboBox();
      this.label4 = new System.Windows.Forms.Label();
      this.groupBoxPlotOffset = new System.Windows.Forms.GroupBox();
      this.checkCenterPlot = new System.Windows.Forms.CheckBox();
      this.labelOffsetY = new System.Windows.Forms.Label();
      this.labelOffsetX = new System.Windows.Forms.Label();
      this.textOffsetY = new System.Windows.Forms.TextBox();
      this.textOffsetX = new System.Windows.Forms.TextBox();
      this.label6 = new System.Windows.Forms.Label();
      this.label5 = new System.Windows.Forms.Label();
      this.groupBoxPlotScale = new System.Windows.Forms.GroupBox();
      this.checkScaleLW = new System.Windows.Forms.CheckBox();
      this.label11 = new System.Windows.Forms.Label();
      this.label10 = new System.Windows.Forms.Label();
      this.comboMMInches = new System.Windows.Forms.ComboBox();
      this.textDrawingUnit = new System.Windows.Forms.TextBox();
      this.textPaperUnit = new System.Windows.Forms.TextBox();
      this.comboScaleValues = new System.Windows.Forms.ComboBox();
      this.label9 = new System.Windows.Forms.Label();
      this.checkFit2Paper = new System.Windows.Forms.CheckBox();
      this.groupBoxMargins = new System.Windows.Forms.GroupBox();
      this.textMarginBottom = new System.Windows.Forms.TextBox();
      this.textMarginTop = new System.Windows.Forms.TextBox();
      this.textMarginRight = new System.Windows.Forms.TextBox();
      this.textMarginLeft = new System.Windows.Forms.TextBox();
      this.label15 = new System.Windows.Forms.Label();
      this.label14 = new System.Windows.Forms.Label();
      this.label13 = new System.Windows.Forms.Label();
      this.label12 = new System.Windows.Forms.Label();
      this.groupBoxCanonicalPaper = new System.Windows.Forms.GroupBox();
      this.textCanonicalName = new System.Windows.Forms.TextBox();
      this.groupBoxPaperImageOrigin = new System.Windows.Forms.GroupBox();
      this.textPIOY = new System.Windows.Forms.TextBox();
      this.textPIOX = new System.Windows.Forms.TextBox();
      this.label17 = new System.Windows.Forms.Label();
      this.label16 = new System.Windows.Forms.Label();
      this.groupBoxWindow = new System.Windows.Forms.GroupBox();
      this.textMaxY = new System.Windows.Forms.TextBox();
      this.textMaxX = new System.Windows.Forms.TextBox();
      this.textMinY = new System.Windows.Forms.TextBox();
      this.textMinX = new System.Windows.Forms.TextBox();
      this.label21 = new System.Windows.Forms.Label();
      this.label20 = new System.Windows.Forms.Label();
      this.label19 = new System.Windows.Forms.Label();
      this.label18 = new System.Windows.Forms.Label();
      this.groupBoxPlotStyleTable = new System.Windows.Forms.GroupBox();
      this.buttonPlotStyleFiles = new System.Windows.Forms.Button();
      this.checkDisplayPlotStyles = new System.Windows.Forms.CheckBox();
      this.comboPlotStyleFiles = new System.Windows.Forms.ComboBox();
      this.groupBoxShadedViewportOptions = new System.Windows.Forms.GroupBox();
      this.textDPI = new System.Windows.Forms.TextBox();
      this.comboQuality = new System.Windows.Forms.ComboBox();
      this.comboShadePlot = new System.Windows.Forms.ComboBox();
      this.label1 = new System.Windows.Forms.Label();
      this.labelQuality = new System.Windows.Forms.Label();
      this.labelShade = new System.Windows.Forms.Label();
      this.groupBoxPlotOptions = new System.Windows.Forms.GroupBox();
      this.checkHidePaperObj = new System.Windows.Forms.CheckBox();
      this.checkPlotPaperLast = new System.Windows.Forms.CheckBox();
      this.checkPlotStyles = new System.Windows.Forms.CheckBox();
      this.checkPlotTransparencies = new System.Windows.Forms.CheckBox();
      this.checkPlotLineweights = new System.Windows.Forms.CheckBox();
      this.groupBoxDrawOrientation = new System.Windows.Forms.GroupBox();
      this.checkUpsideDown = new System.Windows.Forms.CheckBox();
      this.radioLandscape = new System.Windows.Forms.RadioButton();
      this.radioPortrait = new System.Windows.Forms.RadioButton();
      this.buttonOK = new System.Windows.Forms.Button();
      this.buttonCancel = new System.Windows.Forms.Button();
      this.groupBoxPageSetup.SuspendLayout();
      this.groupBoxPrinterPlotter.SuspendLayout();
      this.groupBoxPaperSize.SuspendLayout();
      this.groupBoxPlotArea.SuspendLayout();
      this.groupBoxPlotOffset.SuspendLayout();
      this.groupBoxPlotScale.SuspendLayout();
      this.groupBoxMargins.SuspendLayout();
      this.groupBoxCanonicalPaper.SuspendLayout();
      this.groupBoxPaperImageOrigin.SuspendLayout();
      this.groupBoxWindow.SuspendLayout();
      this.groupBoxPlotStyleTable.SuspendLayout();
      this.groupBoxShadedViewportOptions.SuspendLayout();
      this.groupBoxPlotOptions.SuspendLayout();
      this.groupBoxDrawOrientation.SuspendLayout();
      this.SuspendLayout();
      // 
      // groupBoxPageSetup
      // 
      this.groupBoxPageSetup.Controls.Add(this.labelName);
      this.groupBoxPageSetup.Controls.Add(this.labelN);
      this.groupBoxPageSetup.Location = new System.Drawing.Point(12, 12);
      this.groupBoxPageSetup.Name = "groupBoxPageSetup";
      this.groupBoxPageSetup.Size = new System.Drawing.Size(461, 67);
      this.groupBoxPageSetup.TabIndex = 0;
      this.groupBoxPageSetup.TabStop = false;
      this.groupBoxPageSetup.Text = "Page Setup";
      // 
      // labelName
      // 
      this.labelName.AutoSize = true;
      this.labelName.Location = new System.Drawing.Point(69, 33);
      this.labelName.Name = "labelName";
      this.labelName.Size = new System.Drawing.Size(45, 13);
      this.labelName.TabIndex = 1;
      this.labelName.Text = "_name_";
      // 
      // labelN
      // 
      this.labelN.AutoSize = true;
      this.labelN.Location = new System.Drawing.Point(13, 32);
      this.labelN.Name = "labelN";
      this.labelN.Size = new System.Drawing.Size(38, 13);
      this.labelN.TabIndex = 0;
      this.labelN.Text = "Name:";
      // 
      // groupBoxPrinterPlotter
      // 
      this.groupBoxPrinterPlotter.Controls.Add(this.labelDescription);
      this.groupBoxPrinterPlotter.Controls.Add(this.labelWhere);
      this.groupBoxPrinterPlotter.Controls.Add(this.labelPlotter);
      this.groupBoxPrinterPlotter.Controls.Add(this.labelD);
      this.groupBoxPrinterPlotter.Controls.Add(this.label3);
      this.groupBoxPrinterPlotter.Controls.Add(this.labelP);
      this.groupBoxPrinterPlotter.Controls.Add(this.comboPlotDeviceName);
      this.groupBoxPrinterPlotter.Controls.Add(this.label2);
      this.groupBoxPrinterPlotter.Location = new System.Drawing.Point(12, 85);
      this.groupBoxPrinterPlotter.Name = "groupBoxPrinterPlotter";
      this.groupBoxPrinterPlotter.Size = new System.Drawing.Size(461, 157);
      this.groupBoxPrinterPlotter.TabIndex = 1;
      this.groupBoxPrinterPlotter.TabStop = false;
      this.groupBoxPrinterPlotter.Text = "Printer/plotter";
      // 
      // labelDescription
      // 
      this.labelDescription.AutoSize = true;
      this.labelDescription.Location = new System.Drawing.Point(84, 109);
      this.labelDescription.Name = "labelDescription";
      this.labelDescription.Size = new System.Drawing.Size(70, 13);
      this.labelDescription.TabIndex = 9;
      this.labelDescription.Text = "_description_";
      // 
      // labelWhere
      // 
      this.labelWhere.AutoSize = true;
      this.labelWhere.Location = new System.Drawing.Point(84, 82);
      this.labelWhere.Name = "labelWhere";
      this.labelWhere.Size = new System.Drawing.Size(48, 13);
      this.labelWhere.TabIndex = 8;
      this.labelWhere.Text = "_where_";
      // 
      // labelPlotter
      // 
      this.labelPlotter.AutoSize = true;
      this.labelPlotter.Location = new System.Drawing.Point(84, 59);
      this.labelPlotter.Name = "labelPlotter";
      this.labelPlotter.Size = new System.Drawing.Size(48, 13);
      this.labelPlotter.TabIndex = 7;
      this.labelPlotter.Text = "_plotter_";
      // 
      // labelD
      // 
      this.labelD.AutoSize = true;
      this.labelD.Location = new System.Drawing.Point(13, 109);
      this.labelD.Name = "labelD";
      this.labelD.Size = new System.Drawing.Size(63, 13);
      this.labelD.TabIndex = 6;
      this.labelD.Text = "Description:";
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(13, 82);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(42, 13);
      this.label3.TabIndex = 5;
      this.label3.Text = "Where:";
      // 
      // labelP
      // 
      this.labelP.AutoSize = true;
      this.labelP.Location = new System.Drawing.Point(13, 59);
      this.labelP.Name = "labelP";
      this.labelP.Size = new System.Drawing.Size(40, 13);
      this.labelP.TabIndex = 4;
      this.labelP.Text = "Plotter:";
      // 
      // comboPlotDeviceName
      // 
      this.comboPlotDeviceName.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comboPlotDeviceName.FormattingEnabled = true;
      this.comboPlotDeviceName.Location = new System.Drawing.Point(87, 25);
      this.comboPlotDeviceName.Name = "comboPlotDeviceName";
      this.comboPlotDeviceName.Size = new System.Drawing.Size(260, 21);
      this.comboPlotDeviceName.TabIndex = 3;
      this.comboPlotDeviceName.SelectedIndexChanged += new System.EventHandler(this.comboPlotDeviceName_SelectedIndexChanged);
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(13, 25);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(38, 13);
      this.label2.TabIndex = 2;
      this.label2.Text = "Name:";
      // 
      // groupBoxPaperSize
      // 
      this.groupBoxPaperSize.Controls.Add(this.buttonAddMedia);
      this.groupBoxPaperSize.Controls.Add(this.comboPaperSize);
      this.groupBoxPaperSize.Location = new System.Drawing.Point(12, 248);
      this.groupBoxPaperSize.Name = "groupBoxPaperSize";
      this.groupBoxPaperSize.Size = new System.Drawing.Size(461, 60);
      this.groupBoxPaperSize.TabIndex = 2;
      this.groupBoxPaperSize.TabStop = false;
      this.groupBoxPaperSize.Text = "Paper size";
      // 
      // buttonAddMedia
      // 
      this.buttonAddMedia.Location = new System.Drawing.Point(320, 24);
      this.buttonAddMedia.Name = "buttonAddMedia";
      this.buttonAddMedia.Size = new System.Drawing.Size(75, 23);
      this.buttonAddMedia.TabIndex = 1;
      this.buttonAddMedia.Text = "Add media...";
      this.buttonAddMedia.UseVisualStyleBackColor = true;
      this.buttonAddMedia.Click += new System.EventHandler(this.OnClickAddMediaButton);
      // 
      // comboPaperSize
      // 
      this.comboPaperSize.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comboPaperSize.FormattingEnabled = true;
      this.comboPaperSize.Location = new System.Drawing.Point(16, 24);
      this.comboPaperSize.Name = "comboPaperSize";
      this.comboPaperSize.Size = new System.Drawing.Size(278, 21);
      this.comboPaperSize.TabIndex = 0;
      this.comboPaperSize.SelectionChangeCommitted += new System.EventHandler(this.OnSelChangeMediaList);
      // 
      // groupBoxPlotArea
      // 
      this.groupBoxPlotArea.Controls.Add(this.buttonWindow);
      this.groupBoxPlotArea.Controls.Add(this.comboViews);
      this.groupBoxPlotArea.Controls.Add(this.comboAreaType);
      this.groupBoxPlotArea.Controls.Add(this.label4);
      this.groupBoxPlotArea.Location = new System.Drawing.Point(12, 314);
      this.groupBoxPlotArea.Name = "groupBoxPlotArea";
      this.groupBoxPlotArea.Size = new System.Drawing.Size(265, 80);
      this.groupBoxPlotArea.TabIndex = 3;
      this.groupBoxPlotArea.TabStop = false;
      this.groupBoxPlotArea.Text = "Plot Area";
      // 
      // buttonWindow
      // 
      this.buttonWindow.Location = new System.Drawing.Point(141, 46);
      this.buttonWindow.Name = "buttonWindow";
      this.buttonWindow.Size = new System.Drawing.Size(108, 21);
      this.buttonWindow.TabIndex = 9;
      this.buttonWindow.Text = "Window";
      this.buttonWindow.UseVisualStyleBackColor = true;
      this.buttonWindow.Click += new System.EventHandler(this.buttonWindow_Click);
      // 
      // comboViews
      // 
      this.comboViews.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comboViews.FormattingEnabled = true;
      this.comboViews.Location = new System.Drawing.Point(141, 46);
      this.comboViews.Name = "comboViews";
      this.comboViews.Size = new System.Drawing.Size(115, 21);
      this.comboViews.TabIndex = 2;
      this.comboViews.SelectedIndexChanged += new System.EventHandler(this.OnSelChangeViewsList);
      // 
      // comboAreaType
      // 
      this.comboAreaType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comboAreaType.FormattingEnabled = true;
      this.comboAreaType.Location = new System.Drawing.Point(16, 46);
      this.comboAreaType.Name = "comboAreaType";
      this.comboAreaType.Size = new System.Drawing.Size(115, 21);
      this.comboAreaType.TabIndex = 1;
      this.comboAreaType.SelectedIndexChanged += new System.EventHandler(this.OnSelChangePlotAreaType);
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(17, 19);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(68, 13);
      this.label4.TabIndex = 0;
      this.label4.Text = "What to plot:";
      // 
      // groupBoxPlotOffset
      // 
      this.groupBoxPlotOffset.Controls.Add(this.checkCenterPlot);
      this.groupBoxPlotOffset.Controls.Add(this.labelOffsetY);
      this.groupBoxPlotOffset.Controls.Add(this.labelOffsetX);
      this.groupBoxPlotOffset.Controls.Add(this.textOffsetY);
      this.groupBoxPlotOffset.Controls.Add(this.textOffsetX);
      this.groupBoxPlotOffset.Controls.Add(this.label6);
      this.groupBoxPlotOffset.Controls.Add(this.label5);
      this.groupBoxPlotOffset.Location = new System.Drawing.Point(12, 397);
      this.groupBoxPlotOffset.Name = "groupBoxPlotOffset";
      this.groupBoxPlotOffset.Size = new System.Drawing.Size(265, 80);
      this.groupBoxPlotOffset.TabIndex = 4;
      this.groupBoxPlotOffset.TabStop = false;
      this.groupBoxPlotOffset.Text = "Plot Offset";
      // 
      // checkCenterPlot
      // 
      this.checkCenterPlot.AutoSize = true;
      this.checkCenterPlot.Location = new System.Drawing.Point(147, 38);
      this.checkCenterPlot.Name = "checkCenterPlot";
      this.checkCenterPlot.Size = new System.Drawing.Size(89, 17);
      this.checkCenterPlot.TabIndex = 6;
      this.checkCenterPlot.Text = "Center to plot";
      this.checkCenterPlot.UseVisualStyleBackColor = true;
      this.checkCenterPlot.CheckedChanged += new System.EventHandler(this.OnCheckCenterThePlot);
      // 
      // labelOffsetY
      // 
      this.labelOffsetY.AutoSize = true;
      this.labelOffsetY.Location = new System.Drawing.Point(102, 53);
      this.labelOffsetY.Name = "labelOffsetY";
      this.labelOffsetY.Size = new System.Drawing.Size(38, 13);
      this.labelOffsetY.TabIndex = 5;
      this.labelOffsetY.Text = "inches";
      // 
      // labelOffsetX
      // 
      this.labelOffsetX.AutoSize = true;
      this.labelOffsetX.Location = new System.Drawing.Point(102, 24);
      this.labelOffsetX.Name = "labelOffsetX";
      this.labelOffsetX.Size = new System.Drawing.Size(38, 13);
      this.labelOffsetX.TabIndex = 4;
      this.labelOffsetX.Text = "inches";
      // 
      // textOffsetY
      // 
      this.textOffsetY.Location = new System.Drawing.Point(33, 50);
      this.textOffsetY.Name = "textOffsetY";
      this.textOffsetY.Size = new System.Drawing.Size(63, 20);
      this.textOffsetY.TabIndex = 3;
      this.textOffsetY.TextChanged += new System.EventHandler(this.OnChangeEditOffsetXY);
      // 
      // textOffsetX
      // 
      this.textOffsetX.Location = new System.Drawing.Point(33, 21);
      this.textOffsetX.Name = "textOffsetX";
      this.textOffsetX.Size = new System.Drawing.Size(63, 20);
      this.textOffsetX.TabIndex = 2;
      this.textOffsetX.TextChanged += new System.EventHandler(this.OnChangeEditOffsetXY);
      // 
      // label6
      // 
      this.label6.AutoSize = true;
      this.label6.Location = new System.Drawing.Point(10, 53);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(17, 13);
      this.label6.TabIndex = 1;
      this.label6.Text = "Y:";
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(10, 24);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(17, 13);
      this.label5.TabIndex = 0;
      this.label5.Text = "X:";
      // 
      // groupBoxPlotScale
      // 
      this.groupBoxPlotScale.Controls.Add(this.checkScaleLW);
      this.groupBoxPlotScale.Controls.Add(this.label11);
      this.groupBoxPlotScale.Controls.Add(this.label10);
      this.groupBoxPlotScale.Controls.Add(this.comboMMInches);
      this.groupBoxPlotScale.Controls.Add(this.textDrawingUnit);
      this.groupBoxPlotScale.Controls.Add(this.textPaperUnit);
      this.groupBoxPlotScale.Controls.Add(this.comboScaleValues);
      this.groupBoxPlotScale.Controls.Add(this.label9);
      this.groupBoxPlotScale.Controls.Add(this.checkFit2Paper);
      this.groupBoxPlotScale.Location = new System.Drawing.Point(283, 316);
      this.groupBoxPlotScale.Name = "groupBoxPlotScale";
      this.groupBoxPlotScale.Size = new System.Drawing.Size(190, 161);
      this.groupBoxPlotScale.TabIndex = 5;
      this.groupBoxPlotScale.TabStop = false;
      this.groupBoxPlotScale.Text = "Plot Scale";
      // 
      // checkScaleLW
      // 
      this.checkScaleLW.AutoSize = true;
      this.checkScaleLW.Location = new System.Drawing.Point(49, 126);
      this.checkScaleLW.Name = "checkScaleLW";
      this.checkScaleLW.Size = new System.Drawing.Size(108, 17);
      this.checkScaleLW.TabIndex = 8;
      this.checkScaleLW.Text = "Scale lineweights";
      this.checkScaleLW.UseVisualStyleBackColor = true;
      this.checkScaleLW.CheckedChanged += new System.EventHandler(this.OnCheckScaleLW);
      // 
      // label11
      // 
      this.label11.AutoSize = true;
      this.label11.Location = new System.Drawing.Point(117, 97);
      this.label11.Name = "label11";
      this.label11.Size = new System.Drawing.Size(29, 13);
      this.label11.TabIndex = 7;
      this.label11.Text = "units";
      // 
      // label10
      // 
      this.label10.AutoSize = true;
      this.label10.Location = new System.Drawing.Point(172, 72);
      this.label10.Name = "label10";
      this.label10.Size = new System.Drawing.Size(13, 13);
      this.label10.TabIndex = 6;
      this.label10.Text = "=";
      // 
      // comboMMInches
      // 
      this.comboMMInches.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comboMMInches.FormattingEnabled = true;
      this.comboMMInches.Location = new System.Drawing.Point(108, 68);
      this.comboMMInches.Name = "comboMMInches";
      this.comboMMInches.Size = new System.Drawing.Size(62, 21);
      this.comboMMInches.TabIndex = 5;
      this.comboMMInches.SelectedIndexChanged += new System.EventHandler(this.comboMMInches_SelectedIndexChanged);
      // 
      // textDrawingUnit
      // 
      this.textDrawingUnit.Enabled = false;
      this.textDrawingUnit.Location = new System.Drawing.Point(49, 94);
      this.textDrawingUnit.Name = "textDrawingUnit";
      this.textDrawingUnit.Size = new System.Drawing.Size(53, 20);
      this.textDrawingUnit.TabIndex = 4;
      // 
      // textPaperUnit
      // 
      this.textPaperUnit.Enabled = false;
      this.textPaperUnit.Location = new System.Drawing.Point(49, 68);
      this.textPaperUnit.Name = "textPaperUnit";
      this.textPaperUnit.Size = new System.Drawing.Size(53, 20);
      this.textPaperUnit.TabIndex = 3;
      // 
      // comboScaleValues
      // 
      this.comboScaleValues.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comboScaleValues.Enabled = false;
      this.comboScaleValues.FormattingEnabled = true;
      this.comboScaleValues.Location = new System.Drawing.Point(49, 41);
      this.comboScaleValues.Name = "comboScaleValues";
      this.comboScaleValues.Size = new System.Drawing.Size(121, 21);
      this.comboScaleValues.TabIndex = 2;
      this.comboScaleValues.SelectionChangeCommitted += new System.EventHandler(this.OnSelchangeScaleValues);
      // 
      // label9
      // 
      this.label9.AutoSize = true;
      this.label9.Location = new System.Drawing.Point(6, 44);
      this.label9.Name = "label9";
      this.label9.Size = new System.Drawing.Size(37, 13);
      this.label9.TabIndex = 1;
      this.label9.Text = "Scale:";
      // 
      // checkFit2Paper
      // 
      this.checkFit2Paper.AutoSize = true;
      this.checkFit2Paper.Location = new System.Drawing.Point(11, 19);
      this.checkFit2Paper.Name = "checkFit2Paper";
      this.checkFit2Paper.Size = new System.Drawing.Size(79, 17);
      this.checkFit2Paper.TabIndex = 0;
      this.checkFit2Paper.Text = "Fit to paper";
      this.checkFit2Paper.UseVisualStyleBackColor = true;
      this.checkFit2Paper.CheckedChanged += new System.EventHandler(this.checkFit2Paper_CheckedChanged);
      // 
      // groupBoxMargins
      // 
      this.groupBoxMargins.Controls.Add(this.textMarginBottom);
      this.groupBoxMargins.Controls.Add(this.textMarginTop);
      this.groupBoxMargins.Controls.Add(this.textMarginRight);
      this.groupBoxMargins.Controls.Add(this.textMarginLeft);
      this.groupBoxMargins.Controls.Add(this.label15);
      this.groupBoxMargins.Controls.Add(this.label14);
      this.groupBoxMargins.Controls.Add(this.label13);
      this.groupBoxMargins.Controls.Add(this.label12);
      this.groupBoxMargins.Location = new System.Drawing.Point(12, 483);
      this.groupBoxMargins.Name = "groupBoxMargins";
      this.groupBoxMargins.Size = new System.Drawing.Size(145, 130);
      this.groupBoxMargins.TabIndex = 6;
      this.groupBoxMargins.TabStop = false;
      this.groupBoxMargins.Text = "Margins";
      // 
      // textMarginBottom
      // 
      this.textMarginBottom.Enabled = false;
      this.textMarginBottom.Location = new System.Drawing.Point(55, 96);
      this.textMarginBottom.Name = "textMarginBottom";
      this.textMarginBottom.Size = new System.Drawing.Size(80, 20);
      this.textMarginBottom.TabIndex = 7;
      // 
      // textMarginTop
      // 
      this.textMarginTop.Enabled = false;
      this.textMarginTop.Location = new System.Drawing.Point(55, 70);
      this.textMarginTop.Name = "textMarginTop";
      this.textMarginTop.Size = new System.Drawing.Size(80, 20);
      this.textMarginTop.TabIndex = 6;
      // 
      // textMarginRight
      // 
      this.textMarginRight.Enabled = false;
      this.textMarginRight.Location = new System.Drawing.Point(55, 44);
      this.textMarginRight.Name = "textMarginRight";
      this.textMarginRight.Size = new System.Drawing.Size(80, 20);
      this.textMarginRight.TabIndex = 5;
      // 
      // textMarginLeft
      // 
      this.textMarginLeft.Enabled = false;
      this.textMarginLeft.Location = new System.Drawing.Point(55, 18);
      this.textMarginLeft.Name = "textMarginLeft";
      this.textMarginLeft.Size = new System.Drawing.Size(80, 20);
      this.textMarginLeft.TabIndex = 4;
      // 
      // label15
      // 
      this.label15.AutoSize = true;
      this.label15.Location = new System.Drawing.Point(10, 99);
      this.label15.Name = "label15";
      this.label15.Size = new System.Drawing.Size(40, 13);
      this.label15.TabIndex = 3;
      this.label15.Text = "Bottom";
      // 
      // label14
      // 
      this.label14.AutoSize = true;
      this.label14.Location = new System.Drawing.Point(10, 73);
      this.label14.Name = "label14";
      this.label14.Size = new System.Drawing.Size(26, 13);
      this.label14.TabIndex = 2;
      this.label14.Text = "Top";
      // 
      // label13
      // 
      this.label13.AutoSize = true;
      this.label13.Location = new System.Drawing.Point(10, 47);
      this.label13.Name = "label13";
      this.label13.Size = new System.Drawing.Size(32, 13);
      this.label13.TabIndex = 1;
      this.label13.Text = "Right";
      // 
      // label12
      // 
      this.label12.AutoSize = true;
      this.label12.Location = new System.Drawing.Point(10, 21);
      this.label12.Name = "label12";
      this.label12.Size = new System.Drawing.Size(25, 13);
      this.label12.TabIndex = 0;
      this.label12.Text = "Left";
      // 
      // groupBoxCanonicalPaper
      // 
      this.groupBoxCanonicalPaper.Controls.Add(this.textCanonicalName);
      this.groupBoxCanonicalPaper.Location = new System.Drawing.Point(12, 619);
      this.groupBoxCanonicalPaper.Name = "groupBoxCanonicalPaper";
      this.groupBoxCanonicalPaper.Size = new System.Drawing.Size(151, 45);
      this.groupBoxCanonicalPaper.TabIndex = 7;
      this.groupBoxCanonicalPaper.TabStop = false;
      this.groupBoxCanonicalPaper.Text = "Canonical paper name";
      // 
      // textCanonicalName
      // 
      this.textCanonicalName.Enabled = false;
      this.textCanonicalName.Location = new System.Drawing.Point(6, 19);
      this.textCanonicalName.Name = "textCanonicalName";
      this.textCanonicalName.Size = new System.Drawing.Size(134, 20);
      this.textCanonicalName.TabIndex = 0;
      // 
      // groupBoxPaperImageOrigin
      // 
      this.groupBoxPaperImageOrigin.Controls.Add(this.textPIOY);
      this.groupBoxPaperImageOrigin.Controls.Add(this.textPIOX);
      this.groupBoxPaperImageOrigin.Controls.Add(this.label17);
      this.groupBoxPaperImageOrigin.Controls.Add(this.label16);
      this.groupBoxPaperImageOrigin.Location = new System.Drawing.Point(163, 483);
      this.groupBoxPaperImageOrigin.Name = "groupBoxPaperImageOrigin";
      this.groupBoxPaperImageOrigin.Size = new System.Drawing.Size(150, 80);
      this.groupBoxPaperImageOrigin.TabIndex = 8;
      this.groupBoxPaperImageOrigin.TabStop = false;
      this.groupBoxPaperImageOrigin.Text = "Paper image origin";
      // 
      // textPIOY
      // 
      this.textPIOY.Enabled = false;
      this.textPIOY.Location = new System.Drawing.Point(33, 50);
      this.textPIOY.Name = "textPIOY";
      this.textPIOY.Size = new System.Drawing.Size(100, 20);
      this.textPIOY.TabIndex = 3;
      // 
      // textPIOX
      // 
      this.textPIOX.Enabled = false;
      this.textPIOX.Location = new System.Drawing.Point(33, 23);
      this.textPIOX.Name = "textPIOX";
      this.textPIOX.Size = new System.Drawing.Size(100, 20);
      this.textPIOX.TabIndex = 2;
      // 
      // label17
      // 
      this.label17.AutoSize = true;
      this.label17.Location = new System.Drawing.Point(10, 53);
      this.label17.Name = "label17";
      this.label17.Size = new System.Drawing.Size(17, 13);
      this.label17.TabIndex = 1;
      this.label17.Text = "Y:";
      // 
      // label16
      // 
      this.label16.AutoSize = true;
      this.label16.Location = new System.Drawing.Point(10, 26);
      this.label16.Name = "label16";
      this.label16.Size = new System.Drawing.Size(17, 13);
      this.label16.TabIndex = 0;
      this.label16.Text = "X:";
      // 
      // groupBoxWindow
      // 
      this.groupBoxWindow.Controls.Add(this.textMaxY);
      this.groupBoxWindow.Controls.Add(this.textMaxX);
      this.groupBoxWindow.Controls.Add(this.textMinY);
      this.groupBoxWindow.Controls.Add(this.textMinX);
      this.groupBoxWindow.Controls.Add(this.label21);
      this.groupBoxWindow.Controls.Add(this.label20);
      this.groupBoxWindow.Controls.Add(this.label19);
      this.groupBoxWindow.Controls.Add(this.label18);
      this.groupBoxWindow.Location = new System.Drawing.Point(319, 483);
      this.groupBoxWindow.Name = "groupBoxWindow";
      this.groupBoxWindow.Size = new System.Drawing.Size(154, 120);
      this.groupBoxWindow.TabIndex = 9;
      this.groupBoxWindow.TabStop = false;
      this.groupBoxWindow.Text = "Window";
      // 
      // textMaxY
      // 
      this.textMaxY.Enabled = false;
      this.textMaxY.Location = new System.Drawing.Point(47, 92);
      this.textMaxY.Name = "textMaxY";
      this.textMaxY.Size = new System.Drawing.Size(100, 20);
      this.textMaxY.TabIndex = 7;
      // 
      // textMaxX
      // 
      this.textMaxX.Enabled = false;
      this.textMaxX.Location = new System.Drawing.Point(47, 68);
      this.textMaxX.Name = "textMaxX";
      this.textMaxX.Size = new System.Drawing.Size(100, 20);
      this.textMaxX.TabIndex = 6;
      // 
      // textMinY
      // 
      this.textMinY.Enabled = false;
      this.textMinY.Location = new System.Drawing.Point(47, 44);
      this.textMinY.Name = "textMinY";
      this.textMinY.Size = new System.Drawing.Size(100, 20);
      this.textMinY.TabIndex = 5;
      // 
      // textMinX
      // 
      this.textMinX.Enabled = false;
      this.textMinX.Location = new System.Drawing.Point(47, 20);
      this.textMinX.Name = "textMinX";
      this.textMinX.Size = new System.Drawing.Size(100, 20);
      this.textMinX.TabIndex = 4;
      // 
      // label21
      // 
      this.label21.AutoSize = true;
      this.label21.Location = new System.Drawing.Point(11, 95);
      this.label21.Name = "label21";
      this.label21.Size = new System.Drawing.Size(33, 13);
      this.label21.TabIndex = 3;
      this.label21.Text = "maxY";
      // 
      // label20
      // 
      this.label20.AutoSize = true;
      this.label20.Location = new System.Drawing.Point(11, 73);
      this.label20.Name = "label20";
      this.label20.Size = new System.Drawing.Size(33, 13);
      this.label20.TabIndex = 2;
      this.label20.Text = "maxX";
      // 
      // label19
      // 
      this.label19.AutoSize = true;
      this.label19.Location = new System.Drawing.Point(11, 47);
      this.label19.Name = "label19";
      this.label19.Size = new System.Drawing.Size(30, 13);
      this.label19.TabIndex = 1;
      this.label19.Text = "minY";
      // 
      // label18
      // 
      this.label18.AutoSize = true;
      this.label18.Location = new System.Drawing.Point(11, 23);
      this.label18.Name = "label18";
      this.label18.Size = new System.Drawing.Size(30, 13);
      this.label18.TabIndex = 0;
      this.label18.Text = "minX";
      // 
      // groupBoxPlotStyleTable
      // 
      this.groupBoxPlotStyleTable.Controls.Add(this.buttonPlotStyleFiles);
      this.groupBoxPlotStyleTable.Controls.Add(this.checkDisplayPlotStyles);
      this.groupBoxPlotStyleTable.Controls.Add(this.comboPlotStyleFiles);
      this.groupBoxPlotStyleTable.Location = new System.Drawing.Point(479, 12);
      this.groupBoxPlotStyleTable.Name = "groupBoxPlotStyleTable";
      this.groupBoxPlotStyleTable.Size = new System.Drawing.Size(238, 92);
      this.groupBoxPlotStyleTable.TabIndex = 10;
      this.groupBoxPlotStyleTable.TabStop = false;
      this.groupBoxPlotStyleTable.Text = "Plot Style Table (pen assignments)";
      // 
      // buttonPlotStyleFiles
      // 
      this.buttonPlotStyleFiles.Location = new System.Drawing.Point(196, 25);
      this.buttonPlotStyleFiles.Name = "buttonPlotStyleFiles";
      this.buttonPlotStyleFiles.Size = new System.Drawing.Size(26, 21);
      this.buttonPlotStyleFiles.TabIndex = 2;
      this.buttonPlotStyleFiles.Text = "...";
      this.buttonPlotStyleFiles.UseVisualStyleBackColor = true;
      this.buttonPlotStyleFiles.Click += new System.EventHandler(this.buttonPlotStyleFiles_Click);
      // 
      // checkDisplayPlotStyles
      // 
      this.checkDisplayPlotStyles.AutoSize = true;
      this.checkDisplayPlotStyles.Location = new System.Drawing.Point(24, 58);
      this.checkDisplayPlotStyles.Name = "checkDisplayPlotStyles";
      this.checkDisplayPlotStyles.Size = new System.Drawing.Size(109, 17);
      this.checkDisplayPlotStyles.TabIndex = 1;
      this.checkDisplayPlotStyles.Text = "Display plot styles";
      this.checkDisplayPlotStyles.UseVisualStyleBackColor = true;
      this.checkDisplayPlotStyles.CheckedChanged += new System.EventHandler(this.OnCheckDisplayPlotStyles);
      // 
      // comboPlotStyleFiles
      // 
      this.comboPlotStyleFiles.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comboPlotStyleFiles.FormattingEnabled = true;
      this.comboPlotStyleFiles.Location = new System.Drawing.Point(11, 25);
      this.comboPlotStyleFiles.Name = "comboPlotStyleFiles";
      this.comboPlotStyleFiles.Size = new System.Drawing.Size(179, 21);
      this.comboPlotStyleFiles.TabIndex = 0;
      this.comboPlotStyleFiles.SelectedIndexChanged += new System.EventHandler(this.comboPlotStyleFiles_SelectedIndexChanged);
      // 
      // groupBoxShadedViewportOptions
      // 
      this.groupBoxShadedViewportOptions.Controls.Add(this.textDPI);
      this.groupBoxShadedViewportOptions.Controls.Add(this.comboQuality);
      this.groupBoxShadedViewportOptions.Controls.Add(this.comboShadePlot);
      this.groupBoxShadedViewportOptions.Controls.Add(this.label1);
      this.groupBoxShadedViewportOptions.Controls.Add(this.labelQuality);
      this.groupBoxShadedViewportOptions.Controls.Add(this.labelShade);
      this.groupBoxShadedViewportOptions.Location = new System.Drawing.Point(479, 110);
      this.groupBoxShadedViewportOptions.Name = "groupBoxShadedViewportOptions";
      this.groupBoxShadedViewportOptions.Size = new System.Drawing.Size(238, 104);
      this.groupBoxShadedViewportOptions.TabIndex = 11;
      this.groupBoxShadedViewportOptions.TabStop = false;
      this.groupBoxShadedViewportOptions.Text = "Shaded viewport options";
      // 
      // textDPI
      // 
      this.textDPI.Location = new System.Drawing.Point(72, 77);
      this.textDPI.Name = "textDPI";
      this.textDPI.Size = new System.Drawing.Size(150, 20);
      this.textDPI.TabIndex = 5;
      this.textDPI.TextChanged += new System.EventHandler(this.OnChangeEditDPI);
      // 
      // comboQuality
      // 
      this.comboQuality.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comboQuality.FormattingEnabled = true;
      this.comboQuality.Location = new System.Drawing.Point(72, 50);
      this.comboQuality.Name = "comboQuality";
      this.comboQuality.Size = new System.Drawing.Size(150, 21);
      this.comboQuality.TabIndex = 4;
      this.comboQuality.SelectedIndexChanged += new System.EventHandler(this.OnSelChangeQualityList);
      // 
      // comboShadePlot
      // 
      this.comboShadePlot.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
      this.comboShadePlot.FormattingEnabled = true;
      this.comboShadePlot.Location = new System.Drawing.Point(72, 23);
      this.comboShadePlot.Name = "comboShadePlot";
      this.comboShadePlot.Size = new System.Drawing.Size(150, 21);
      this.comboShadePlot.TabIndex = 3;
      this.comboShadePlot.SelectedIndexChanged += new System.EventHandler(this.OnSelChangeShadePlotList);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(41, 80);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(25, 13);
      this.label1.TabIndex = 2;
      this.label1.Text = "DPI";
      // 
      // labelQuality
      // 
      this.labelQuality.AutoSize = true;
      this.labelQuality.Location = new System.Drawing.Point(27, 53);
      this.labelQuality.Name = "labelQuality";
      this.labelQuality.Size = new System.Drawing.Size(39, 13);
      this.labelQuality.TabIndex = 1;
      this.labelQuality.Text = "Quality";
      // 
      // labelShade
      // 
      this.labelShade.AutoSize = true;
      this.labelShade.Location = new System.Drawing.Point(8, 26);
      this.labelShade.Name = "labelShade";
      this.labelShade.Size = new System.Drawing.Size(58, 13);
      this.labelShade.TabIndex = 0;
      this.labelShade.Text = "Shade plot";
      // 
      // groupBoxPlotOptions
      // 
      this.groupBoxPlotOptions.Controls.Add(this.checkHidePaperObj);
      this.groupBoxPlotOptions.Controls.Add(this.checkPlotPaperLast);
      this.groupBoxPlotOptions.Controls.Add(this.checkPlotStyles);
      this.groupBoxPlotOptions.Controls.Add(this.checkPlotTransparencies);
      this.groupBoxPlotOptions.Controls.Add(this.checkPlotLineweights);
      this.groupBoxPlotOptions.Location = new System.Drawing.Point(479, 215);
      this.groupBoxPlotOptions.Name = "groupBoxPlotOptions";
      this.groupBoxPlotOptions.Size = new System.Drawing.Size(238, 137);
      this.groupBoxPlotOptions.TabIndex = 12;
      this.groupBoxPlotOptions.TabStop = false;
      this.groupBoxPlotOptions.Text = "Plot Options";
      // 
      // checkHidePaperObj
      // 
      this.checkHidePaperObj.AutoSize = true;
      this.checkHidePaperObj.Location = new System.Drawing.Point(11, 116);
      this.checkHidePaperObj.Name = "checkHidePaperObj";
      this.checkHidePaperObj.Size = new System.Drawing.Size(144, 17);
      this.checkHidePaperObj.TabIndex = 4;
      this.checkHidePaperObj.Text = "Hide paperspace objects";
      this.checkHidePaperObj.UseVisualStyleBackColor = true;
      this.checkHidePaperObj.CheckedChanged += new System.EventHandler(this.checkHidePaperObj_CheckedChanged);
      // 
      // checkPlotPaperLast
      // 
      this.checkPlotPaperLast.AutoSize = true;
      this.checkPlotPaperLast.Location = new System.Drawing.Point(11, 93);
      this.checkPlotPaperLast.Name = "checkPlotPaperLast";
      this.checkPlotPaperLast.Size = new System.Drawing.Size(122, 17);
      this.checkPlotPaperLast.TabIndex = 3;
      this.checkPlotPaperLast.Text = "Plot paperspace last";
      this.checkPlotPaperLast.UseVisualStyleBackColor = true;
      this.checkPlotPaperLast.CheckedChanged += new System.EventHandler(this.checkPlotPaperLast_CheckedChanged);
      // 
      // checkPlotStyles
      // 
      this.checkPlotStyles.AutoSize = true;
      this.checkPlotStyles.Location = new System.Drawing.Point(11, 70);
      this.checkPlotStyles.Name = "checkPlotStyles";
      this.checkPlotStyles.Size = new System.Drawing.Size(115, 17);
      this.checkPlotStyles.TabIndex = 2;
      this.checkPlotStyles.Text = "Plot with plot styles";
      this.checkPlotStyles.UseVisualStyleBackColor = true;
      this.checkPlotStyles.CheckedChanged += new System.EventHandler(this.checkPlotStyles_CheckedChanged);
      // 
      // checkPlotTransparencies
      // 
      this.checkPlotTransparencies.AutoSize = true;
      this.checkPlotTransparencies.Location = new System.Drawing.Point(11, 47);
      this.checkPlotTransparencies.Name = "checkPlotTransparencies";
      this.checkPlotTransparencies.Size = new System.Drawing.Size(116, 17);
      this.checkPlotTransparencies.TabIndex = 1;
      this.checkPlotTransparencies.Text = "Plot transparencies";
      this.checkPlotTransparencies.UseVisualStyleBackColor = true;
      this.checkPlotTransparencies.CheckedChanged += new System.EventHandler(this.OnCheckPlotTransparencies);
      // 
      // checkPlotLineweights
      // 
      this.checkPlotLineweights.AutoSize = true;
      this.checkPlotLineweights.Location = new System.Drawing.Point(11, 24);
      this.checkPlotLineweights.Name = "checkPlotLineweights";
      this.checkPlotLineweights.Size = new System.Drawing.Size(131, 17);
      this.checkPlotLineweights.TabIndex = 0;
      this.checkPlotLineweights.Text = "Plot object lineweights";
      this.checkPlotLineweights.UseVisualStyleBackColor = true;
      this.checkPlotLineweights.CheckedChanged += new System.EventHandler(this.checkPlotLineweights_CheckedChanged);
      // 
      // groupBoxDrawOrientation
      // 
      this.groupBoxDrawOrientation.Controls.Add(this.checkUpsideDown);
      this.groupBoxDrawOrientation.Controls.Add(this.radioLandscape);
      this.groupBoxDrawOrientation.Controls.Add(this.radioPortrait);
      this.groupBoxDrawOrientation.Location = new System.Drawing.Point(479, 358);
      this.groupBoxDrawOrientation.Name = "groupBoxDrawOrientation";
      this.groupBoxDrawOrientation.Size = new System.Drawing.Size(238, 89);
      this.groupBoxDrawOrientation.TabIndex = 13;
      this.groupBoxDrawOrientation.TabStop = false;
      this.groupBoxDrawOrientation.Text = "Drawing orientation";
      // 
      // checkUpsideDown
      // 
      this.checkUpsideDown.AutoSize = true;
      this.checkUpsideDown.Location = new System.Drawing.Point(17, 65);
      this.checkUpsideDown.Name = "checkUpsideDown";
      this.checkUpsideDown.Size = new System.Drawing.Size(107, 17);
      this.checkUpsideDown.TabIndex = 2;
      this.checkUpsideDown.Text = "Plot upside down";
      this.checkUpsideDown.UseVisualStyleBackColor = true;
      // 
      // radioLandscape
      // 
      this.radioLandscape.AutoSize = true;
      this.radioLandscape.Location = new System.Drawing.Point(17, 42);
      this.radioLandscape.Name = "radioLandscape";
      this.radioLandscape.Size = new System.Drawing.Size(78, 17);
      this.radioLandscape.TabIndex = 1;
      this.radioLandscape.Text = "Landscape";
      this.radioLandscape.UseVisualStyleBackColor = true;
      this.radioLandscape.MouseClick += new System.Windows.Forms.MouseEventHandler(this.OnRadioLandscapeChecked);
      // 
      // radioPortrait
      // 
      this.radioPortrait.AutoSize = true;
      this.radioPortrait.Location = new System.Drawing.Point(17, 19);
      this.radioPortrait.Name = "radioPortrait";
      this.radioPortrait.Size = new System.Drawing.Size(58, 17);
      this.radioPortrait.TabIndex = 0;
      this.radioPortrait.Text = "Portrait";
      this.radioPortrait.UseVisualStyleBackColor = true;
      this.radioPortrait.MouseClick += new System.Windows.Forms.MouseEventHandler(this.OnRadioPortraitChecked);
      // 
      // buttonOK
      // 
      this.buttonOK.DialogResult = System.Windows.Forms.DialogResult.OK;
      this.buttonOK.Location = new System.Drawing.Point(528, 469);
      this.buttonOK.Name = "buttonOK";
      this.buttonOK.Size = new System.Drawing.Size(80, 23);
      this.buttonOK.TabIndex = 14;
      this.buttonOK.Text = "OK";
      this.buttonOK.UseVisualStyleBackColor = true;
      // 
      // buttonCancel
      // 
      this.buttonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
      this.buttonCancel.Location = new System.Drawing.Point(626, 469);
      this.buttonCancel.Name = "buttonCancel";
      this.buttonCancel.Size = new System.Drawing.Size(80, 23);
      this.buttonCancel.TabIndex = 15;
      this.buttonCancel.Text = "Cancel";
      this.buttonCancel.UseVisualStyleBackColor = true;
      // 
      // PageSetupDlg
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(726, 667);
      this.Controls.Add(this.buttonCancel);
      this.Controls.Add(this.buttonOK);
      this.Controls.Add(this.groupBoxDrawOrientation);
      this.Controls.Add(this.groupBoxPlotOptions);
      this.Controls.Add(this.groupBoxShadedViewportOptions);
      this.Controls.Add(this.groupBoxPlotStyleTable);
      this.Controls.Add(this.groupBoxWindow);
      this.Controls.Add(this.groupBoxPaperImageOrigin);
      this.Controls.Add(this.groupBoxCanonicalPaper);
      this.Controls.Add(this.groupBoxMargins);
      this.Controls.Add(this.groupBoxPlotScale);
      this.Controls.Add(this.groupBoxPlotOffset);
      this.Controls.Add(this.groupBoxPlotArea);
      this.Controls.Add(this.groupBoxPaperSize);
      this.Controls.Add(this.groupBoxPrinterPlotter);
      this.Controls.Add(this.groupBoxPageSetup);
      this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
      this.Name = "PageSetupDlg";
      this.Text = "Page Setup";
      this.groupBoxPageSetup.ResumeLayout(false);
      this.groupBoxPageSetup.PerformLayout();
      this.groupBoxPrinterPlotter.ResumeLayout(false);
      this.groupBoxPrinterPlotter.PerformLayout();
      this.groupBoxPaperSize.ResumeLayout(false);
      this.groupBoxPlotArea.ResumeLayout(false);
      this.groupBoxPlotArea.PerformLayout();
      this.groupBoxPlotOffset.ResumeLayout(false);
      this.groupBoxPlotOffset.PerformLayout();
      this.groupBoxPlotScale.ResumeLayout(false);
      this.groupBoxPlotScale.PerformLayout();
      this.groupBoxMargins.ResumeLayout(false);
      this.groupBoxMargins.PerformLayout();
      this.groupBoxCanonicalPaper.ResumeLayout(false);
      this.groupBoxCanonicalPaper.PerformLayout();
      this.groupBoxPaperImageOrigin.ResumeLayout(false);
      this.groupBoxPaperImageOrigin.PerformLayout();
      this.groupBoxWindow.ResumeLayout(false);
      this.groupBoxWindow.PerformLayout();
      this.groupBoxPlotStyleTable.ResumeLayout(false);
      this.groupBoxPlotStyleTable.PerformLayout();
      this.groupBoxShadedViewportOptions.ResumeLayout(false);
      this.groupBoxShadedViewportOptions.PerformLayout();
      this.groupBoxPlotOptions.ResumeLayout(false);
      this.groupBoxPlotOptions.PerformLayout();
      this.groupBoxDrawOrientation.ResumeLayout(false);
      this.groupBoxDrawOrientation.PerformLayout();
      this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBoxPageSetup;
		private System.Windows.Forms.GroupBox groupBoxPrinterPlotter;
		private System.Windows.Forms.GroupBox groupBoxPaperSize;
		private System.Windows.Forms.GroupBox groupBoxPlotArea;
		private System.Windows.Forms.GroupBox groupBoxPlotOffset;
		private System.Windows.Forms.GroupBox groupBoxPlotScale;
		private System.Windows.Forms.GroupBox groupBoxMargins;
		private System.Windows.Forms.GroupBox groupBoxCanonicalPaper;
		private System.Windows.Forms.GroupBox groupBoxPaperImageOrigin;
		private System.Windows.Forms.GroupBox groupBoxWindow;
		private System.Windows.Forms.GroupBox groupBoxPlotStyleTable;
		private System.Windows.Forms.GroupBox groupBoxShadedViewportOptions;
		private System.Windows.Forms.GroupBox groupBoxPlotOptions;
		private System.Windows.Forms.GroupBox groupBoxDrawOrientation;
		private System.Windows.Forms.Button buttonOK;
		private System.Windows.Forms.Button buttonCancel;
		private System.Windows.Forms.CheckBox checkHidePaperObj;
		private System.Windows.Forms.CheckBox checkPlotPaperLast;
		private System.Windows.Forms.CheckBox checkPlotStyles;
		private System.Windows.Forms.CheckBox checkPlotTransparencies;
		private System.Windows.Forms.CheckBox checkPlotLineweights;
		private System.Windows.Forms.CheckBox checkUpsideDown;
		private System.Windows.Forms.RadioButton radioLandscape;
		private System.Windows.Forms.RadioButton radioPortrait;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label labelQuality;
		private System.Windows.Forms.Label labelShade;
		private System.Windows.Forms.ComboBox comboQuality;
		private System.Windows.Forms.ComboBox comboShadePlot;
		private System.Windows.Forms.TextBox textDPI;
		private System.Windows.Forms.ComboBox comboPlotStyleFiles;
		private System.Windows.Forms.Button buttonPlotStyleFiles;
		private System.Windows.Forms.CheckBox checkDisplayPlotStyles;
		private System.Windows.Forms.Label labelName;
		private System.Windows.Forms.Label labelN;
		private System.Windows.Forms.Label labelD;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label labelP;
		private System.Windows.Forms.ComboBox comboPlotDeviceName;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label labelPlotter;
		private System.Windows.Forms.Label labelWhere;
		private System.Windows.Forms.Label labelDescription;
		private System.Windows.Forms.Button buttonAddMedia;
		private System.Windows.Forms.ComboBox comboPaperSize;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.ComboBox comboAreaType;
		private System.Windows.Forms.ComboBox comboViews;
		private System.Windows.Forms.CheckBox checkCenterPlot;
		private System.Windows.Forms.Label labelOffsetY;
		private System.Windows.Forms.Label labelOffsetX;
		private System.Windows.Forms.TextBox textOffsetY;
		private System.Windows.Forms.TextBox textOffsetX;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.CheckBox checkScaleLW;
		private System.Windows.Forms.Label label11;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.ComboBox comboMMInches;
		private System.Windows.Forms.TextBox textDrawingUnit;
		private System.Windows.Forms.TextBox textPaperUnit;
		private System.Windows.Forms.ComboBox comboScaleValues;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.CheckBox checkFit2Paper;
		private System.Windows.Forms.TextBox textMarginBottom;
		private System.Windows.Forms.TextBox textMarginTop;
		private System.Windows.Forms.TextBox textMarginRight;
		private System.Windows.Forms.TextBox textMarginLeft;
		private System.Windows.Forms.Label label15;
		private System.Windows.Forms.Label label14;
		private System.Windows.Forms.Label label13;
		private System.Windows.Forms.Label label12;
		private System.Windows.Forms.TextBox textCanonicalName;
		private System.Windows.Forms.TextBox textPIOY;
		private System.Windows.Forms.TextBox textPIOX;
		private System.Windows.Forms.Label label17;
		private System.Windows.Forms.Label label16;
		private System.Windows.Forms.TextBox textMaxY;
		private System.Windows.Forms.TextBox textMaxX;
		private System.Windows.Forms.TextBox textMinY;
		private System.Windows.Forms.TextBox textMinX;
		private System.Windows.Forms.Label label21;
		private System.Windows.Forms.Label label20;
		private System.Windows.Forms.Label label19;
		private System.Windows.Forms.Label label18;
		private System.Windows.Forms.Button buttonWindow;
	}
}