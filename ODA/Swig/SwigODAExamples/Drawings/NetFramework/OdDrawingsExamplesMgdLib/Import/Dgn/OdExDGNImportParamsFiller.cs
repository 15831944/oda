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

namespace OdDrawingsExamplesMgdLib.Import.Dgn
{
  public class OdExDGNImportParamsFiller
  {
    public OdDbDatabase Database { get; set; }
    public OdRxObject HostAppServices { get; set; }
    public String DgnPath { get; set; }
    public byte XRefImportMode { get; set; }
    public bool ImportActiveModelToModelSpace { get; set; }
    public bool ImportPaperSpaceModels { get; set; }
    public bool RecomputeDimensionsAfterImport { get; set; }
    public byte ImportViewIndex { get; set; }
    public byte Shape3dImportMode { get; set; }
    public String shxFontsPath { get; set; }

    public OdExDGNImportParamsFiller()
    {
      DgnPath = String.Empty;
      XRefImportMode = 2;
      ImportActiveModelToModelSpace = true;
      ImportPaperSpaceModels = true;
      RecomputeDimensionsAfterImport = false;
      ImportViewIndex = 0;
      Shape3dImportMode = 1;
      shxFontsPath = String.Empty;
    }

    public static OdExDGNImportParamsFiller CreateDefault(OdDbDatabase db, OdRxObject hostApp, String dgnPath)
    {
      OdExDGNImportParamsFiller result = new OdExDGNImportParamsFiller();

      result.Database = db;
      result.HostAppServices = hostApp;
      result.DgnPath = dgnPath;

      result.XRefImportMode = 2;
      result.ImportActiveModelToModelSpace = true;
      result.ImportPaperSpaceModels = true;
      result.RecomputeDimensionsAfterImport = false;
      result.ImportViewIndex = 0;
      result.Shape3dImportMode = 1;
      result.shxFontsPath = String.Empty;
      return result;
    }

    public void FillParameters(OdDgnImport importer)
    {
      if (Database == null)
        throw new Exception("Database is null");

      if (HostAppServices == null)
        throw new Exception("HostAppServices is null");

      if (String.IsNullOrEmpty(DgnPath))
        throw new Exception("DGN file path, for import, not exist");

      if (!File.Exists(DgnPath))
        throw new Exception("DGN file path, for import, not exist");

      if (importer == null)
        throw new Exception("OdDgnImport is null");

      OdRxDictionary Properties = importer.properties();
      Properties.putAt("Database", Database);
      Properties.putAt("Services", HostAppServices);
      Properties.putAt("DgnPath", new OdRxVariantValue(DgnPath));

      //Properties.putAt("MS_SYMBRSRC", OdRxVariantValue(arrRsrc) );
      // !!! be careful when using numbers as arguments for OdRxVariantalue
      // !!! there are several ctors: with byte, sbyte, (U)Int16,32,64
      // pay attention to legacy C++ code and use the proper one

      Properties.putAt("XRefImportMode", new OdRxVariantValue(XRefImportMode));
      Properties.putAt("ImportActiveModelToModelSpace", new OdRxVariantValue(ImportActiveModelToModelSpace));
      Properties.putAt("ImportPaperSpaceModels", new OdRxVariantValue(ImportPaperSpaceModels));
      Properties.putAt("RecomputeDimensionsAfterImport", new OdRxVariantValue(RecomputeDimensionsAfterImport));
      Properties.putAt("ImportViewIndex", new OdRxVariantValue(ImportViewIndex));
      Properties.putAt("3dShapeImportMode", new OdRxVariantValue(Shape3dImportMode));
      Properties.putAt("shxFontsPath", new OdRxVariantValue(shxFontsPath));

      setLineWeightForDgnIndex(Properties);
    }

    private void setLineWeightForDgnIndex(OdRxDictionary Properties)
    {
      OdRxObject lw = Properties.getAt("LineWeightsMap");
      OdDgnImportLineWeightsMap pLWMap = new OdDgnImportLineWeightsMap(OdRxObject.getCPtr(lw).Handle, false);
      if (null != pLWMap)
      {
        for (UInt32 i = 0; i < 32; i++)
        {
          if (i % 2 == 0)
            pLWMap.setLineWeightForDgnIndex(i, LineWeight.kLnWt030);
          else
            pLWMap.setLineWeightForDgnIndex(i, LineWeight.kLnWt100);
        }
      }
    }
  }
}