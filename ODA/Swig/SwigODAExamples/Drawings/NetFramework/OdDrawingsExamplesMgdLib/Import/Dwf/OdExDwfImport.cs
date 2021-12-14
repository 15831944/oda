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
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.Import.Dwf
{
  public static class OdExDwfImport
  {

    public static void Import(OdDbDatabase db, OdExDWFImportParamsFiller importParameters)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OdRxModule module = Teigha.Core.Globals.odrxDynamicLinker().loadModule("TD_Dwf7Import");
        if (null == module)
          throw new Exception("Module TD_Dwf7Import.tx is missing");

        OdDwfImportModule dwfImportModule = new OdDwfImportModule(OdRxModule.getCPtr(module).Handle, false);

        OdDwfImport importer = dwfImportModule.create();

        importParameters.FillImportParameters(importer, db);

        OdDwfImport.ImportResult importState = importer.import();

        switch (importState)
        {
          case OdDwfImport.ImportResult.success:
            break;
          case OdDwfImport.ImportResult.bad_database:
            throw new Exception(String.Concat("DWF import error: ", "bad database"));
          case OdDwfImport.ImportResult.bad_file:
            throw new Exception(String.Concat("DWF import error: ", "bad file"));
          case OdDwfImport.ImportResult.bad_password:
            throw new Exception(String.Concat("DWF import error: ", "bad password"));
          case OdDwfImport.ImportResult.encrypted_file:
            throw new Exception(String.Concat("DWF import error: ", "encrypted file"));
          case OdDwfImport.ImportResult.fail:
            throw new Exception(String.Concat("DWF import error: ", "unknown failure"));
          default:
            throw new Exception(String.Concat("DWF import error: ", "other failure"));
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