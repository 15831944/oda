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
using System.Windows.Forms;
using Teigha.Core;
using OdKernelExamplesMgdLib.Export.Dwf;

namespace OdKernelExamplesUIMgdLib.Export.Dwf
{
  public static class OdExDwfExportUI
  {
    public static void Export<TDwfExportParamsFiller>(OdRxObject db)
      where TDwfExportParamsFiller : OdExDwfExportParamsFiller, new()
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OdExport2DwfForm<TDwfExportParamsFiller> formExport = new OdExport2DwfForm<TDwfExportParamsFiller>();
        if (formExport.ShowDialog() != DialogResult.OK)
          throw new Exception("DWF Export canceled");

        using (DwExportParams parametrsExport = formExport.DwfExportParametersFiller.Generate(db))
        {
          OdExDwfExport exportExample = new OdExDwfExport();
          exportExample.Export(parametrsExport);
        }
      }
      catch (Exception err)
      {
        MessageBox.Show(
        err.Message,
        "Information",
        MessageBoxButtons.OK,
        MessageBoxIcon.Information);
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }
    }
  }
}