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
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Teigha.TD;

namespace OdDrawingsExamplesUIMgdLib.ExPrinting
{
  public partial class OdExCustomMediaDlg : Form
  {
    private OdDbPlotSettingsValidatorPE.psvPaperInfo paperinf;
    public OdExCustomMediaDlg()
    {
      InitializeComponent();
    }
    public OdExCustomMediaDlg(OdDbPlotSettingsValidatorPE.psvPaperInfo paperinf)
    {
      InitializeComponent();
      this.paperinf = paperinf;
      this.textBMargin.KeyPress += double_textBox_validate_keys;
      this.textTMargin.KeyPress += double_textBox_validate_keys;
      this.textRMargin.KeyPress += double_textBox_validate_keys;
      this.textLMargin.KeyPress += double_textBox_validate_keys;
      this.textEditHeight.KeyPress += double_textBox_validate_keys;
      this.textEditWidth.KeyPress += double_textBox_validate_keys;
    }

    private void double_textBox_validate_keys(object sender, KeyPressEventArgs e)
    {
      //48 - 57 will be numbers
      //46 - decimal point
      //8 - backspace
      TextBox tb = sender as TextBox;
      if ((e.KeyChar < 48 || e.KeyChar > 57) && e.KeyChar != 46 && e.KeyChar != 8)
        e.Handled = true;
      if (e.KeyChar == 46 && tb.Text.Contains("."))
        e.Handled = true;
    }

    private void CustomMediaDlg_Load(object sender, EventArgs e)
    {
      if (paperinf == null)
        throw new Exception("OdDbPlotSettingsValidatorPE.psvPaperInfo paperinf , not initialized");

      comboMUnits.Items.Clear();
      comboMUnits.Items.Insert(0, "Inches");
      comboMUnits.Items.Insert(1, "Millimeters");
      comboMUnits.Items.Insert(2, "Pixels");
      comboMUnits.SelectedIndex = 1;
    }

    private void buttonOK_Click(object sender, EventArgs e)
    {
      if (String.IsNullOrEmpty(textEditName.Text))
      {
        MessageBox.Show("CanonicalName cannot may is empty");
        return;
      }

      paperinf.w = String.IsNullOrEmpty(textEditWidth.Text) ? 0 : Convert.ToDouble(textEditWidth.Text);
      paperinf.h = String.IsNullOrEmpty(textEditHeight.Text) ? 0 : Convert.ToDouble(textEditHeight.Text);
      paperinf.bottom = String.IsNullOrEmpty(textBMargin.Text) ? 0 : Convert.ToDouble(textBMargin.Text);
      paperinf.top = String.IsNullOrEmpty(textTMargin.Text) ? 0 : Convert.ToDouble(textTMargin.Text);
      paperinf.left = String.IsNullOrEmpty(textLMargin.Text) ? 0 : Convert.ToDouble(textLMargin.Text);
      paperinf.right = String.IsNullOrEmpty(textRMargin.Text) ? 0 : Convert.ToDouble(textRMargin.Text);

      paperinf.canonicalName = textEditName.Text.Trim().Replace(" ", "_");
      paperinf.localeName = paperinf.canonicalName;
      paperinf.units = (OdDbPlotSettings.PlotPaperUnits)comboMUnits.SelectedIndex;

      this.DialogResult = DialogResult.OK;
    }

    private void buttonCancel_Click(object sender, EventArgs e)
    {
      this.DialogResult = DialogResult.Cancel;
    }
  }
}
