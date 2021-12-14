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

namespace OdExPointCloudConverterSampleSwig
{
  class Program
  {
    static Program()
    {
      OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
    }

    private static String sUsage =
      "Usage: OdExPointCloudConverterSampleSwig <inputFileType> <inputFilePath> <outputFilePath>\n" +
      "  <inputFileType> can be:\n" +
      "                     pts\n" +
      "  <inputFilePath>  - full path to the input file\n" +
      "  <outputFilePath> - full path to the output .rcs file.\n" +
      "\nPress ENTER to continue...\n";

    static void Main(string[] args)
    {
      if (args.Length < 3)
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

      String format = args[0], filePath = args[1], outFilePath = args[2];

      try
      {
        Console.WriteLine("\nProcessing convert from '.{0}' to '.rcs' format .......\n", format);
        OdRxModule pModule = Globals.odrxDynamicLinker().loadModule("RcsFileServices");
        if (pModule == null)
          throw new OdError("RcsFileServices is missing");

        OdRxRcsFileServices rcsModule = new OdRxRcsFileServices(OdRxModule.getCPtr(pModule).Handle, false);

        if (String.Compare(format, "pts", true) == 0)
        {
          OdPointCloudDataSource pDataSource = new OdExPointCloudPtsDataSource(filePath);
          OdPointCloudConverter pConverter = rcsModule.getPointCloudConverter(pDataSource);

          pConverter.convertToRcsFormat(outFilePath);
        }
        else
        {
          Console.WriteLine("\nFormat of the input file is not supported by example!\n");
        }

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
