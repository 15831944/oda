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
using System.IO;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.Import.Pdf
{
  public static class OdExPDF2dImport
  {
    public static void Import(OdExPDF2dImportParamsFiller importParameters)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OdRxModule module = Globals.odrxDynamicLinker().loadApp("TD_PdfImport", false);
        if (module == null)
          throw new System.Exception("Module TD_PdfImport.tx is missing");

        OdPdfImportModule pdfImportModule = new OdPdfImportModule(OdRxModule.getCPtr(module).Handle, false);

        OdPdfImport importer = pdfImportModule.create();

        importParameters.FillParameters(importer);

        OdPdfImport.ImportResult res = importer.import();

        switch (res)
        {
          case OdPdfImport.ImportResult.success:
            break;
          case OdPdfImport.ImportResult.bad_database:
            throw new Exception(String.Concat("PDF 2D import: ", "The import process has been failed because of incorrect database object to be imported to"));
          case OdPdfImport.ImportResult.bad_file:
            throw new Exception(String.Concat("PDF 2D import: ", "The import process has been failed because of incorrect input .pdf file."));
          case OdPdfImport.ImportResult.encrypted_file:
            throw new Exception(String.Concat("PDF 2D import: ", "The import process has been failed because of encrypted input .pdf file."));
          case OdPdfImport.ImportResult.bad_password:
            throw new Exception(String.Concat("PDF 2D import: ", "The import process has been failed because of incorrect password."));
          case OdPdfImport.ImportResult.invalid_page_number:
            throw new Exception(String.Concat("PDF 2D import: ", "The import process has been failed because of invalid page number of the input .pdf file."));
          case OdPdfImport.ImportResult.image_file_error:
            throw new Exception(String.Concat("PDF 2D import: ", "The import process has been failed because of it is impossible to save the image file"));
          case OdPdfImport.ImportResult.fail:
          default:
            throw new Exception(String.Concat("PDF 2D import: ", "Unknown import error"));
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
#endif