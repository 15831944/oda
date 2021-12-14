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
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.Import.Pdf
{
  public class OdExPDF2dImportParamsFiller
  {
    public OdDbDatabase Database { get; set; }
    public String PdfPath { get; set; }

    public static OdExPDF2dImportParamsFiller CreateDefault(OdDbDatabase db, String pdfPath)
    {
      OdExPDF2dImportParamsFiller result = new OdExPDF2dImportParamsFiller();
      result.Database = db;
      result.PdfPath = pdfPath;
      return result;
    }

    public void FillParameters(OdPdfImport importer)
    {
      OdRxDictionary Properties = importer.properties();
      Properties.putAt("Database", Database);
      Properties.putAt("PdfPath", new OdRxVariantValue(PdfPath));
    }
  }
}
#endif