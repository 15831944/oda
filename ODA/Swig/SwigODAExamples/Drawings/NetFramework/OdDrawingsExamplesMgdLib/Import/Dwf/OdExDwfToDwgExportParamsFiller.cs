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
using System.IO;
using Teigha.Core;
using Teigha.TD;
using OdKernelExamplesMgdLib.ExExecuteCommands;

namespace OdDrawingsExamplesMgdLib.Import.Dwf
{
  public class OdExDWFImportParamsFiller
  {
    public String DwfFilePath { get; set; }
    public String Password { get; set; }
    public double PaperWidth { get; set; }
    public double PaperHeight { get; set; }
    public bool PreserveColorIndices { get; set; }
    public int LayoutNumber { get; set; }
    public bool Import_3dDWF_HSF_Model { get; set; }

    public OdExDWFImportParamsFiller()
    {
      DwfFilePath = String.Empty;
      Password = "dwfin";
      PaperWidth = 297.0;
      PaperHeight = 210.0;
      PreserveColorIndices = false;
      LayoutNumber = -1;
      Import_3dDWF_HSF_Model = true;
    }

    public String GenerateCommandLine()
    {
      String result = String.Format("{0} {1} {2} {3} {4} {5}",
        Password,
        OdExCommandHelpers.DoubleToStringCommaNormalize(PaperWidth),
        OdExCommandHelpers.DoubleToStringCommaNormalize(PaperHeight),
        OdExCommandHelpers.BooleanToStringNormalize_YesNo(PreserveColorIndices),
        LayoutNumber,
        OdExCommandHelpers.BooleanToStringNormalize_YesNo(Import_3dDWF_HSF_Model));

      return result;
    }

    public void FillImportParameters(OdDwfImport importer, OdDbDatabase db)
    {
      if (String.IsNullOrEmpty(this.DwfFilePath))
        throw new Exception("DWF file path, for import, not exist");

      if (!File.Exists(this.DwfFilePath))
        throw new Exception("DWF file path, for import, not exist");

      if (importer == null)
        throw new Exception("OdDwfImport is null");

      if (db == null)
        throw new Exception("OdDbDatabase is null");

      OdRxDictionary Properties = importer.properties();

      Properties.putAt("Database", db);
      Properties.putAt("DwfPath", new OdRxVariantValue(this.DwfFilePath));
      Properties.putAt("PreserveColorIndices", new OdRxVariantValue(this.PreserveColorIndices));
      Properties.putAt("Password", new OdRxVariantValue(this.Password));
      Properties.putAt("PaperWidth", new OdRxVariantValue(this.PaperWidth));
      Properties.putAt("PaperHeight", new OdRxVariantValue(this.PaperHeight));
      Properties.putAt("LayoutNumber", new OdRxVariantValue(this.LayoutNumber));
      Properties.putAt("ImportW3d", new OdRxVariantValue(this.Import_3dDWF_HSF_Model));
    }

    public static OdExDWFImportParamsFiller CreateDefault()
    {
      OdExDWFImportParamsFiller result = new OdExDWFImportParamsFiller();
      result.Password = "dwfin";
      result.PaperWidth = 297.0;
      result.PaperHeight = 210.0;
      result.PreserveColorIndices = false;
      result.LayoutNumber = -1;
      result.Import_3dDWF_HSF_Model = true;
      return result;
    }
  }
}