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
using Teigha.Core;

namespace OdKernelExamplesMgdLib.Export.Pdf
{
  public class OdExPdfExport
  {
    public void Export(PDFExportParams Params)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      try
      {
        OdRxModule module_TD_PdfExport = Globals.odrxDynamicLinker().loadApp("TD_PdfExport");
        if (null == module_TD_PdfExport)
          throw new Exception("Failed to load PDF export module");

        PdfExportModule module = new PdfExportModule(OdRxModule.getCPtr(module_TD_PdfExport).Handle, false);
        OdPdfExport exporter = module.create();
        UInt32 errCode = exporter.exportPdf(Params);

        if (errCode != 0)
        {
          String errMessage = exporter.exportPdfErrorCode(errCode);
          errMessage = string.Format("Error number : {0}. \n {1}", errCode, errMessage);

          if (errCode == 0x10008)
            errMessage += "\nPlease enable Zoom to extents check box or\ndefine page parameters for layout in page setup dialog.";

          throw new Exception(String.Format("PDF Export Error:{0}", errMessage));
        }

      }
      catch (Exception err)
      {
        throw err;
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }
    }
  }
}
