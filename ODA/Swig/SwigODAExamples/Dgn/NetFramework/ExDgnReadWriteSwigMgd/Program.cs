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

using OdKernelExamplesMgdLib;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Teigha.Core;
using Teigha.TG;

namespace ExDgnReadWrite
{
  class Program
  {
    static Program()
    {
      OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
    }

    static void Main(string[] args)
    {
      MemoryManager mMan = MemoryManager.GetMemoryManager();
      MemoryTransaction mStartTrans = mMan.StartTransaction();

      Console.WriteLine("\nExDgnReadWriteSwigMgd sample program. Copyright (c) 2020, Open Design Alliance\n");
      if (args.Length < 2)
      {
        Console.WriteLine("Usage: ExDgnReadWriteSwigMgd <srcfilename> <dstfilename>");
        return;
      }

      String srcFileName = args[0];
      String dstFileName = args[1];
      if (!File.Exists(srcFileName))
      {
        Console.WriteLine(string.Format("File {0} does not exist", srcFileName));
        return;
      }

      OdExSystemServices systemServices = new OdExSystemServices();
      OdExDgnHostAppServices hostAppServices = new OdExDgnHostAppServices();
      Teigha.Core.Globals.odrxInitialize(systemServices);
      try
      {
        /**********************************************************************/
        /* Initialize ODA™ for .dgn files                                     */
        /**********************************************************************/
        Teigha.Core.Globals.odrxDynamicLinker().loadModule("TG_Db", false);

        /********************************************************************/
        /* Read the DGN file into the OdDgDatabase object                   */
        /********************************************************************/
        OdDgDatabase inputDatabase = null;
        try
        {
          if (String.Empty != hostAppServices.findFile(srcFileName))
            inputDatabase = hostAppServices.readFile(srcFileName);
        }
        catch (Exception Err)
        {
          do
          {
            Console.WriteLine(Err.GetType().Name + ": ");
            Console.WriteLine(Err.Message);
            Err = Err.InnerException;
          } while (Err != null);
        }

        /********************************************************************/
        /* Write the OdDgDatabase data into the DGN file                    */
        /********************************************************************/
        if (null != inputDatabase)
          inputDatabase.writeFile(dstFileName);
        else
          Console.WriteLine("pDb is null");
      }
      catch (OdError Err)
      {
        Console.WriteLine("\nODA Drawings API for DGN Error: \n" + Err.description());
      }
      catch (Exception Err)
      {
        Console.WriteLine("\nUnknown Error.\n" + Err.Message);
      }
      finally
      {
        // stop transaction
        mMan.StopTransaction(mStartTrans);
        mMan.StopAll();
      }

      Teigha.Core.Globals.odrxUninitialize();
    }
  }
}
