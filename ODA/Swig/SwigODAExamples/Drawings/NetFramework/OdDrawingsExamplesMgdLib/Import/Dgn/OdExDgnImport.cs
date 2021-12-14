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
  public static class OdExDgnImport
  {
    public static void Import(OdExDGNImportParamsFiller importParameters)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OdRxModule module = Globals.odrxDynamicLinker().loadApp("TD_DgnImport", false);
        if (module == null)
          throw new System.Exception("Module TD_DgnImport.tx is missing");

        OdDgnImportModule dgnImportModule = new OdDgnImportModule(OdRxModule.getCPtr(module).Handle, false);

        OdDgnImport importer = dgnImportModule.create();

        importParameters.FillParameters(importer);

        OdDgnImport.ImportResult res = importer.import();

        switch (res)
        {
          case OdDgnImport.ImportResult.success:
            break;
          case OdDgnImport.ImportResult.bad_database:
            throw new Exception(String.Concat("DGN import: ", "Bad database"));
          case OdDgnImport.ImportResult.bad_file:
            throw new Exception(String.Concat("DGN import: ", "Bad file"));
          case OdDgnImport.ImportResult.encrypted_file:
            throw new Exception(String.Concat("DGN import: ", "The file is encrypted"));
          case OdDgnImport.ImportResult.bad_password:
            throw new Exception(String.Concat("DGN import: ", "Bad Password"));
          case OdDgnImport.ImportResult.fail:
          default:
            throw new Exception(String.Concat("DGN import: ", "Unknown import error"));
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