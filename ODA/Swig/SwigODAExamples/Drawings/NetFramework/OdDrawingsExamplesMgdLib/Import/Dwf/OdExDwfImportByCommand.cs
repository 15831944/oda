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
using OdDrawingsExamplesMgdLib.ExExecuteCommands;

namespace OdDrawingsExamplesMgdLib.Import.Dwf
{
  public static class OdExDwfImportByCommand
  {
    public static void Import(OdDbDatabase db)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OdRxModule pModule = Teigha.Core.Globals.odrxDynamicLinker().loadModule("TD_Dwf7Import");
        if (null == pModule)
          throw new Exception("Module TD_Dwf7Import.tx is missing");

        OdExDWFImportParamsFiller importParamsFiller = OdExDWFImportParamsFiller.CreateDefault();

        String command = importParamsFiller.GenerateCommandLine();

        OdDbCommandContext commandContext = OdExExecuteCommands.CreateDbCommandContext(command, db);

        OdEdCommandStack pStack = Globals.odedRegCmds();

        pStack.executeCommand("DWFIN", commandContext);
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