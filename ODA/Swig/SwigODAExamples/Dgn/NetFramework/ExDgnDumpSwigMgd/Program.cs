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
using System.Collections;
using System.Text;
using System.IO;
using Teigha.Core;
using Teigha.TG;
using System.Runtime.Remoting;
using System.Reflection;

namespace ExDgnDumpSwigMgd
{
  class Program
  {
    static Program()
    {
      OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
    }

    public static StreamWriter DumpStream { get; set; }

    static void Main(string[] args)
    {
      MemoryManager mMan = MemoryManager.GetMemoryManager();
      MemoryTransaction mStartTrans = mMan.StartTransaction();

      if (args.Length < 2)
      {
        Console.WriteLine("\n\tUsage: ExDgnDumpSwigMgd <srcfilename> <dstfilename> [-Multirun] ");
        Console.WriteLine("\tfor multirun usage:  <dstfilename> - short name");
        Console.WriteLine("\t                     -Multirun = 1");
        Console.WriteLine("\t                     and /output/ subfolder should be existed ");
        return;
      }

      String szSource = args[0];
      if (!File.Exists(szSource))
      {
        Console.WriteLine(string.Format("File {0} does not exist", szSource));
        return;
      }

      String szDump = args[1];
      bool bMultirun = false;
      if (args.Length > 2)
      {
        bMultirun = true;
      }
      else
      {
        DumpStream = new StreamWriter(szDump);
        DumpStream.WriteLine("Dump of the file " + szDump);
      }

      OdExSystemServices systemSerives = new OdExSystemServices();
      OdExCustomHostAppServices hostAppServices = new OdExCustomHostAppServices();
      Globals.odrxInitialize(systemSerives);
      Globals.odgsInitialize();
      int j = (bMultirun) ? 0 : 19;
      for (; j < 20; j++)
      {
        if (bMultirun)
        {
          String out_directory = Path.Combine(Directory.GetCurrentDirectory(), "ExDgnDumpSwigMgd_output");
          if (!Directory.Exists(out_directory))
            Directory.CreateDirectory(out_directory);
          DumpStream = new StreamWriter(Path.Combine(out_directory, String.Format("{0}_{1}", j, szDump)));
        }

        MemoryTransaction mTrans = mMan.StartTransaction();
        try
        {
          if (bMultirun)
          {
            DumpStream.WriteLine("Dump of the file " + j.ToString() + szDump);
          }
          Globals.odrxDynamicLinker().loadModule("TG_Db", false);
          OdDgDatabase pDb = null;
          ArrayList arrayRscList = new ArrayList();
          String sRscPath = "C:\\Documents and Settings\\All Users\\Application Data\\Bentley\\WorkSpace\\System\\Symb\\font.rsc";
          arrayRscList.Add(sRscPath);
          if (String.Empty != hostAppServices.findFile(szSource))
          {
            pDb = hostAppServices.readFile(szSource);
          }
          OdDgDumper dumper = new OdDgDumper();
          dumper.rootDump(pDb);
        }
        catch (OdError Err)
        {
          Console.WriteLine(string.Format("OdError - {0}", Err.description()));
        }
        catch (Exception Err)
        {
          Console.WriteLine(string.Format("Other error {0}", Err.Message));
        }
        finally
        {
          DumpStream.Close();
          mMan.StopTransaction(mTrans);
        }
      }
      mMan.StopTransaction(mStartTrans);

      Globals.odgsUninitialize();
      Globals.odrxUninitialize();
    }
  }
}
