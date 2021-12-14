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

#if NET35 || NET40 || NET45
/*No available , because PdfImport use pdfium (available from vc14). For using PdfImport functionality you need use platform >= vc14*/
#else
using System;
using System.Windows.Forms;
using Teigha.Core;
using Teigha.TD;
using OdDrawingsExamplesMgdLib.Import.Pdf;

namespace OdDrawingsExamplesUIMgdLib.Import.Pdf
{
  public static class OdExPDF2dImportGUIExample
  {
    public static void Import(OdDbDatabase db)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OpenFileDialog selectPdfForm = new OpenFileDialog();
        selectPdfForm.Filter = "Pdf files|*.pdf";

        if (DialogResult.Cancel == selectPdfForm.ShowDialog())
          throw new Exception("Pdf import canceled");

        String PdfFilePath = selectPdfForm.FileName;
        OdExPDF2dImportParamsFiller pdfParamsFiller = OdExPDF2dImportParamsFiller.CreateDefault(db, PdfFilePath);

        OdExPDF2dImport.Import(pdfParamsFiller);
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
#endif