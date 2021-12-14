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
using Teigha.PointCloud;

namespace OdExRcsPointCloudReadExSwig
{

  class Program
  {
    static Program()
    {
      OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
    }

    private static String sUsage =
       "Usage: OdExRcsPointCloudReadExSwig <inputFileType> <inputFilePath>\n" +
       "  <inputFileType> can be:\n" +
       "                     rcs\n" +
       "                     rcp\n" +
       "  <inputFilePath>  - full path to the input file\n" +
       "\nPress ENTER to continue...\n";

    static void Main(string[] args)
    {
      if (args.Length < 2)
      {
        Console.WriteLine(sUsage);
        return;
      }

      /**********************************************************************/
      /* Start Root Memory Transaction                                      */
      /**********************************************************************/
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      /**********************************************************************/
      /* Initialize Runtime Extension environment                           */
      /**********************************************************************/
      OdExSystemServices svcs = new OdExSystemServices();
      Globals.odrxInitialize(svcs);

      String format = args[0], filePath = args[1];

      try
      {
        OdRxModule pModule = Globals.odrxDynamicLinker().loadModule("RcsFileServices");
        if (pModule == null)
          throw new OdError("RcsFileServices is missing");

        OdRxRcsFileServices rcsModule = new OdRxRcsFileServices(OdRxModule.getCPtr(pModule).Handle, false);

        OdExConsoleDumper dumper = null;
        if (String.Compare(format, "rcs", true) == 0)
        {
          OdPointCloudScanDatabase pScanDb = rcsModule.readRcsFile(filePath);
          dumper = new OdExRcsFileDumper(pScanDb);
        }
        else if (String.Compare(format, "rcp", true) == 0)
        {
          OdPointCloudProjectDatabase pProjectDb = rcsModule.readRcpFile(filePath);
          dumper = new OdExRcpFileDumper(pProjectDb);
        }

        if (dumper == null)
          throw new OdError("\nUnsupported file format specified!\n");

        dumper.Dump();

        Globals.odrxDynamicLinker().unloadModule("RcsFileServices");
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
      Globals.odrxUninitialize();
    }
  }
}
