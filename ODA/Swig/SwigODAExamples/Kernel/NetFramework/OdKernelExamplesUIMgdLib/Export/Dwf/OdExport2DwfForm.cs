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

using OdKernelExamplesMgdLib.Export.Dwf;
using System;
using System.Text;
using System.Windows.Forms;

namespace OdKernelExamplesUIMgdLib.Export.Dwf
{
  public class OdExport2DwfForm<TDwfExportParamsFiller>
    where TDwfExportParamsFiller : OdExDwfExportParamsFiller, new()
  {
    private SaveFileDialog dwfDlg = new SaveFileDialog();

    public OdExport2DwfForm()
    {
      DwfExportParametersFiller = new TDwfExportParamsFiller();
    }

    public DialogResult ShowDialog()
    {
      dwfDlg.Title = "Export to DWF";
      dwfDlg.Filter = CreateSaveFileDialogFilter();
      DialogResult result = dwfDlg.ShowDialog();
      if (result == DialogResult.OK)
        FillExportParameters();
      return result;
    }

    public OdExDwfExportParamsFiller DwfExportParametersFiller { get; private set; }


    private string Filepath { get { return dwfDlg.FileName; } }

    private OdExDwfExportType DwfExportType { get { return (OdExDwfExportType)(dwfDlg.FilterIndex - 1); } }

    private void FillExportParameters()
    {
      DwfExportParametersFiller.DwfExportType = DwfExportType;
      DwfExportParametersFiller.OutDwfFilepath = Filepath;
      DwfExportParametersFiller.ColorMapOptimize = false;
      DwfExportParametersFiller.InkedArea = false;
      DwfExportParametersFiller.ExportInvisibleText = true;
    }

    private static string CreateSaveFileDialogFilter()
    {
      Type enumType = typeof(OdExDwfExportType);
      Array enum_values = Enum.GetValues(enumType);
      StringBuilder builder = new StringBuilder();

      int enum_idx = 0;
      foreach (object enum_value in enum_values)
      {
        OdExDwfExportType type = (OdExDwfExportType)Enum.Parse(enumType, enum_value.ToString());
        switch (type)
        {
          case OdExDwfExportType.BinaryDWF_v60:
            builder.Append("Binary DWF v6.0|*.dwf");
            break;
          case OdExDwfExportType.ZippedStreamDWF_v60:
            builder.Append("Zipped Stream DWF v6.0|*.dwf");
            break;
          case OdExDwfExportType.CompressedDWF_v55:
            builder.Append("Compressed DWF v5.5|*.dwf");
            break;
          case OdExDwfExportType.BinaryDWF_v55:
            builder.Append("Binary DWF v5.5|*.dwf");
            break;
          case OdExDwfExportType.AsciiDWF_v55:
            builder.Append("Ascii DWF v5.5|*.dwf");
            break;
          case OdExDwfExportType.CompressedDWF_v42:
            builder.Append("Compressed DWF v4.2|*.dwf");
            break;
          case OdExDwfExportType.BinaryDWF_v42:
            builder.Append("Binary DWF v4.2|*.dwf");
            break;
          case OdExDwfExportType.AsciiDWF_v42:
            builder.Append("Ascii DWF v4.2|*.dwf");
            break;
          case OdExDwfExportType.XPS:
            builder.Append("XPS|*.dwfx");
            break;
          default:
            throw new Exception("Dwf export format & version not implemented in example");
        }

        if (enum_idx + 1 != enum_values.Length)
          builder.Append("|");

        enum_idx++;
      }

      return builder.ToString();
    }
  }
}