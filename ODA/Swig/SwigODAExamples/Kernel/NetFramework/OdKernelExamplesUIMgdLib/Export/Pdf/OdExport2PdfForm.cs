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
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace OdKernelExamplesUIMgdLib.Export.Pdf
{
  public partial class OdExport2PdfForm<TPdfExportParamsFiller> : Form
    where TPdfExportParamsFiller : OdExPdfExportParamsFiller, new()
  {
    public OdExPdfExportParamsFiller PdfExportParamsFiller { get; private set; }

    public OdExport2PdfForm()
    {
      InitializeComponent();
      Init();
    }

    private void Init()
    {
      PdfExportParamsFiller = new TPdfExportParamsFiller();
      PaperWidthString.Text = PdfExportParamsFiller.PaperW.ToString();
      PaperHeightString.Text = PdfExportParamsFiller.PaperH.ToString();
      PdfExportParamsFiller.HatchDPI = 150;
      PdfExportParamsFiller.GeomDPI = 600;
      textBox1.Text = PdfExportParamsFiller.HatchDPI.ToString();
      textBox2.Text = PdfExportParamsFiller.GeomDPI.ToString();
      Off_layers.Checked = PdfExportParamsFiller.ExportOffLayers;
      All.Checked = PdfExportParamsFiller.AllLayouts;
      Active.Checked = !All.Checked;
      btnExportMultipleDatabases.Checked = PdfExportParamsFiller.MultipleDatabases;
    }

    private void Browse_Click(object sender, EventArgs e)
    {
      SaveFileDialog saveSvgDialog = new SaveFileDialog();
      saveSvgDialog.Filter = "PDF files|*.pdf";
      if (saveSvgDialog.ShowDialog() != DialogResult.OK)
      {
        return;
      }
      PdfFilePathString.Text = saveSvgDialog.FileName;
      Export.Enabled = true;
    }

    private void Export_Click(object sender, EventArgs e)
    {
      PdfExportParamsFiller.PaperW = Convert.ToDouble(PaperWidthString.Text);
      PdfExportParamsFiller.PaperH = Convert.ToDouble(PaperHeightString.Text);
      PdfExportParamsFiller.HatchDPI = Convert.ToUInt16(textBox1.Text);
      PdfExportParamsFiller.GeomDPI = Convert.ToUInt16(textBox2.Text);
      PdfExportParamsFiller.EmbeddedFonts = Embedded_fonts.Checked; // 1
      PdfExportParamsFiller.TTFAsGeometry = True_type.Checked; // 2
      PdfExportParamsFiller.SHXAsGeometry = SHX_text.Checked; // 3
      PdfExportParamsFiller.SimpleGeomOpt = Enable_optimization.Checked; // 4
      PdfExportParamsFiller.ZoomToExtents = Zoom_extents.Checked; // 5
      PdfExportParamsFiller.UseHLR = Hidden_line.Checked; // 6
      PdfExportParamsFiller.EnableLayers = Layer_support.Checked; // 7
      PdfExportParamsFiller.ExportOffLayers = Off_layers.Checked; // 8
      PdfExportParamsFiller.Encoded = Encoded_small.Checked;
      PdfExportParamsFiller.PdfFilePath = PdfFilePathString.Text;
      PdfExportParamsFiller.Title = TitleString.Text;
      PdfExportParamsFiller.Author = AuthorString.Text;
      PdfExportParamsFiller.Subject = SubjectString.Text;
      PdfExportParamsFiller.Keywords = KeywordsString.Text;
      PdfExportParamsFiller.Creator = CreatorString.Text;
      PdfExportParamsFiller.Producer = ProducerString.Text;
      PdfExportParamsFiller.AllLayouts = All.Checked;
      PdfExportParamsFiller.ExportHyperlinks = ExportHyperlinks1.Checked;
      PdfExportParamsFiller.UsePrc = btnUsePrc.Checked;
      PdfExportParamsFiller.UsePRCAsBRep = btnUsePrcAsBRep.Checked;
      PdfExportParamsFiller.MultipleDatabases = btnExportMultipleDatabases.Checked;
    }

    private void Export2PdfForm_Shown(object sender, EventArgs e)
    {
      PaperWidthString.Text = PdfExportParamsFiller.PaperW.ToString();
      PaperHeightString.Text = PdfExportParamsFiller.PaperH.ToString();
      textBox1.Text = PdfExportParamsFiller.HatchDPI.ToString();
      textBox2.Text = PdfExportParamsFiller.GeomDPI.ToString();
    }
  }
}
