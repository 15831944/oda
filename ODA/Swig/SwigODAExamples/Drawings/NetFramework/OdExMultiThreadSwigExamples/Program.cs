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
using System.Text;
using Teigha.Core;
using System.IO;
using Teigha.TD;
using System.Threading;

namespace OdExMultiThreadSwigExamples
{
  class Program
  {
    static Program()
    {
      OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
    }

    private static String sUsage =
    "Usage: OdExMultiThreadSwigExamples \"LowLevelApiSample or HighLevelApiSample\" <inputFolderPath>\n" +
    "  <inputFolderPath>  - full path to the input dwg files\n" +
    " The examples demonstrated rendering for each DWG file and save as BMP file. \n"+
    " Once executed, bmp files will be created in the input folder for each DWG file.\n" +
    "\nPress ENTER to continue...\n";

    static void Main(string[] args)
    {
      if (args.Length < 2 || args.Length > 2)
      {
        Console.WriteLine(sUsage);
        return;
      }

      String selectExample = args[0];
      String folderPath = args[1];

      if (String.IsNullOrEmpty(folderPath) || !Directory.Exists(folderPath))
      {
        Console.WriteLine(sUsage);
        return;
      }

      /**********************************************************************/
      /* Start Root Memory Transaction                                      */
      /**********************************************************************/
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      OdExSystemServices svcs = new OdExSystemServices();
      TD_Db.odInitialize(svcs);

      try
      {
        if(String.Compare(selectExample, "LowLevelApiSample", true) == 0)
          OdExMT_HighLevelApiSample.Run(folderPath);
        else if(String.Compare(selectExample, "HighLevelApiSample", true) == 0)
          OdExMT_LowLevelApiSample.Run(folderPath);
        else
          Console.WriteLine(sUsage);
      }
      catch (OdError e)
      {
        Console.WriteLine(e.description());
      }
      finally
      {
        /**********************************************************************/
        /* Stop Root Memory Transaction                                      */
        /**********************************************************************/
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }

      /**********************************************************************/
      /* Uninitialize Runtime Extension environment                         */
      /**********************************************************************/
      TD_Db.odUninitialize();
    }
  }
}
