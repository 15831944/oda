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
using Teigha.TD;

namespace ExBrepBuilderSwigMgd
{
  class SystemServices : RxSystemServicesImpl
  {
    public SystemServices()
    {
      Teigha.Core.Globals.odActivate(ActivationData.userInfo, ActivationData.userSignature);
    }
  }

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

      /**********************************************************************/
      /* Parse Command Line inputs                                          */
      /**********************************************************************/
      bool bInvalidArgs = (args.Length < 1);
      if (bInvalidArgs)
        bInvalidArgs = true;

      SaveType fileType = SolveSaveType(args);
      DwgVersion outVer = SolveDwgVersion(args);

      if (bInvalidArgs || fileType == SaveType.kUnknown || outVer == DwgVersion.kDHL_Unknown)
      {
        Console.WriteLine("\n\tusage: ExBrepBuilderSwigMgd <Out_filename> [OutVer] [OutType] [-DO]");
        Console.WriteLine("\tOutVer can be any of ACAD12, ACAD13, ACAD14, ACAD2000, ACAD2004, ACAD2007, ACAD2010\n");
        Console.WriteLine("\tOutType can be any of DWG, DXF, DXB\n");
        Console.WriteLine("\t-DO disables progress meter output.\n");
        return;
      }

      bool disableOutput = false;
      /**********************************************************************/
      /* Disable progress meter                                             */
      /**********************************************************************/
      if (args.Length >= 4 && args[3] == "-DO")
      {
        disableOutput = true;
      }

      String dstFileNameOrigin = args[0];
      bool bDwg = dstFileNameOrigin.EndsWith(".dwg");
      if (!bDwg)
      {
        Console.WriteLine("\nExBrepBuilderSwigMgd: not .dwg file ");
        return;
      }

      SystemServices Serv = new SystemServices();
      ExHostAppServices HostApp = new ExHostAppServices();
      Console.WriteLine("ExBrepBuilderSwigMgd developed using {0} ver {1}",
        HostApp.product(), HostApp.versionString());

      /************************************************************************/
      /* Disable/Enable progress meter                                       */
      /************************************************************************/
      HostApp.disableOutput(disableOutput);

      /**********************************************************************/
      /* Initialize ODA Drawings.NET                                        */
      /**********************************************************************/
      TD_Db.odInitialize(Serv);

      /**********************************************************************/
      /* Load and Release ModelerGeometry                                   */
      /**********************************************************************/
      Globals.odrxDynamicLinker().loadApp("ModelerGeometry").Dispose();
      /************************************************************************/
      /* File creation                                                        */
      /************************************************************************/
      MemoryTransaction mTr1 = mMan.StartTransaction();
      try
      {
        /********************************************************************/
        /* Create a default OdDbDatabase object                             */
        /********************************************************************/
        OdDbDatabase pDb = HostApp.createDatabase(true, MeasurementValue.kEnglish);
        OdDbDatabaseSummaryInfo pSumInfo = TD_Db.oddbGetSummaryInfo(pDb);
        pSumInfo.setComments("File was created by OdWriteEx");
        TD_Db.oddbPutSummaryInfo(pSumInfo);

        /********************************************************************/
        /* Fill the database                                                */
        /********************************************************************/
        MemoryTransaction mTr2 = mMan.StartTransaction();
        DatabaseFiller filler = new DatabaseFiller();
        filler.FillDatabase(pDb);
        mMan.StopTransaction(mTr2);

        /********************************************************************/
        /* Write the database                                                */
        /********************************************************************/
        String dstFileName = dstFileNameOrigin;
        pDb.writeFile(dstFileName, fileType, outVer, true /* save preview */);
        Console.WriteLine("Database has been saved as {0}", pDb.getFilename());
      }
      catch (OdError e)
      {
        Console.WriteLine(string.Format("Error: code: {0}, description: {1}", e.code(), e.description()));
      }
      catch (Exception err)
      {
        Console.WriteLine("\nUnexpected error.");
        throw;
      }
      finally
      {
        mMan.StopTransaction(mTr1);
      }

      /**********************************************************************/
      /* Uninitialize ODA Drawings.NET                                      */
      /**********************************************************************/
      mMan.StopTransaction(mStartTrans);
      mMan.StopAll();
      try
      {
        TD_Db.odUninitialize();
      }
      catch (OdError e)
      {
        Console.WriteLine("Cleanup error: " + e.description());
      }
      Teigha.Core.Helpers.odUninit();
    }

    private static DwgVersion SolveDwgVersion(string[] args)
    {
      DwgVersion outVer = DwgVersion.vAC24;
      /**********************************************************************/
      /* Set file version                                                   */
      /**********************************************************************/
      if (args.Length < 2)
        return outVer;

      switch (args[1])
      {
        case "ACAD12":
          outVer = DwgVersion.vAC12; // R11-12
          break;
        case "ACAD13":
          outVer = DwgVersion.vAC13; // R13
          break;
        case "ACAD14":
          outVer = DwgVersion.vAC14; // R14
          break;
        case "ACAD2000":
          outVer = DwgVersion.vAC15; // 2000-2002
          break;
        case "ACAD2004":
          outVer = DwgVersion.vAC18; // 2004-2006
          break;
        case "ACAD2007":
          outVer = DwgVersion.vAC21; // 2007
          break;
        case "ACAD2010":
          outVer = DwgVersion.vAC24; // 2010
          break;
        default:
          outVer = DwgVersion.kDHL_Unknown;
          break;
      }

      return outVer;
    }

    private static SaveType SolveSaveType(string[] args)
    {
      SaveType fileType = SaveType.kDwg;
      /**********************************************************************/
      /* Set file type                                                      */
      /**********************************************************************/
      if (args.Length < 3)
        return fileType;

      switch (args[2])
      {
        case "DWG":
          fileType = SaveType.kDwg;
          break;
        case "DXF":
          fileType = SaveType.kDxf;
          break;
        case "DXB":
          fileType = SaveType.kDxb;
          break;
        default:
          fileType = SaveType.kUnknown;
          break;
      }

      return fileType;
    }

  }
}
