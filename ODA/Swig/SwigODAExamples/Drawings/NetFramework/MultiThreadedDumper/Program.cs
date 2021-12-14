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
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;
using System.Threading.Tasks;
using Teigha.Core;
using Teigha.TD;

namespace MultiThreadedDumper
{
  class Program
  {
    class OdExSystemServices : RxSystemServicesImpl
    {
      public OdExSystemServices()
      {
        Teigha.Core.Globals.odActivate(ActivationData.userInfo, ActivationData.userSignature);
      }
    }

    static Program()
    {
      OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
    }

    static void Main(string[] args)
    {
      bool bInvalidArgs = false;

      if (args.Length == 0)
        bInvalidArgs = true;

      if (bInvalidArgs)
      {
        Console.WriteLine("Usage: \nMultiThreadedDumper <folder path>");
        return;
      }

      String pathToFolder = args[0];

      if (!Directory.Exists(pathToFolder))
      {
        Console.WriteLine(String.Format("Path not exist: {0}", pathToFolder));
        return;
      }

      // When working with a multi-threaded application it is recommended to specify thread id
      MemoryTransaction startTr = MemoryManager.GetMemoryManager().StartTransaction(Thread.CurrentThread.ManagedThreadId);

      try
      {
        // initialize services
        OdExSystemServices systemServices = new OdExSystemServices();
        ExHostAppServices hostAppServices = new ExHostAppServices();
        hostAppServices.disableOutput(true);
        hostAppServices.setMtMode(1);
        TD_Db.odInitialize(systemServices);

        // obtain files to dump
        string[] dumpFiles = Directory.GetFiles(pathToFolder, "*.dwg", SearchOption.TopDirectoryOnly);

        // create dump tasks - the same number as dumpFiles
        Task[] dumpTasks = new Task[dumpFiles.Length];
        for (int ctr = 0; ctr < dumpFiles.Length; ctr++)
          dumpTasks[ctr] = DumpFile(dumpFiles[ctr], hostAppServices);

        Task.WaitAll(dumpTasks);

      }
      catch (Exception err)
      {
        Console.WriteLine(err.Message);
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(startTr);
      }

      TD_Db.odUninitialize();
    }

    private static object m_objectLocker = new object();

    private static Task<int> DumpFile(string fileName, ExHostAppServices hostApp)
    {
      return Task.Factory.StartNew(() =>
     {
       MemoryTransaction dumpTr = MemoryManager.GetMemoryManager().StartTransaction(Thread.CurrentThread.ManagedThreadId);
       try
       {
         OdDbDatabase dumpDb = null;
         lock (m_objectLocker)
         {
           try
           {
             dumpDb = hostApp.readFile(fileName);
           }
           catch (OdError err)
           {
             Console.WriteLine("OdError caught {0}", err.Message);
           }
         }
         if (null == dumpDb)
           Console.WriteLine("DWG file {0} does not exist or is incorrect", fileName);
         else
         {
           // dumping will be performed to a txt file
           // txt file name is the same as dwg file name, only extension differs
           string txtDumpFile = fileName;
           txtDumpFile = txtDumpFile.Substring(0, txtDumpFile.LastIndexOf("."));
           txtDumpFile += ".txt";

           Console.WriteLine("Dump file is {0}", txtDumpFile);
           StreamWriter txtDump = new StreamWriter(txtDumpFile);
           DbDumper dumper = new DbDumper(txtDump);
           dumper.dump(dumpDb);
         }
       }
       catch (Exception err)
       {
         Console.WriteLine(err.Message);
       }
       finally
       {
         MemoryManager.GetMemoryManager().StopTransaction(dumpTr, Thread.CurrentThread.ManagedThreadId);
       }
       return 0;
     });
    }
  }
}
