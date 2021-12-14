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
using Microsoft.Win32;

namespace OdColladaExportExSwigMgd
{
  class OdExSystemServices : RxSystemServicesImpl
  {
    public OdExSystemServices()
    {
      Teigha.Core.Globals.odActivate(ActivationData.userInfo, ActivationData.userSignature);
    }
  }

  class OdExHostAppServices : ExHostAppServices
  {
    //OdGsDevice _bd = null;
    //public override OdGsDevice gsBitmapDevice()
    //{
    //  if (_bd == null)
    //  {
    //    try
    //    {
    //      using (OdGsModule pGsModule = (OdGsModule)Globals.odrxDynamicLinker().loadModule("WinBitmap.txv"))
    //        _bd = pGsModule.createBitmapDevice();
    //    }
    //    catch (OdError)
    //    {
    //    }
    //  }
    //  return _bd;
    //}
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
      {
        bInvalidArgs = true;
      }

      if (bInvalidArgs)
      {
        Console.WriteLine("\n\tusage: OdColladaExportExSwigMgd <input file> <output file>");
        Console.WriteLine("   <input file>  - .dxf or .dwg file\n");
        Console.WriteLine("   <output file> - .dae file\n");
        return;
      }

      String inFile = args[0];
      String outFile = args[1];

      OdExSystemServices Serv = new OdExSystemServices();
      OdExHostAppServices HostApp = new OdExHostAppServices();
      Console.WriteLine("OdColladaExportExSwigMgd developed using {0} ver {1}",
        HostApp.product(), HostApp.versionString());

      /**********************************************************************/
      /* Initialize ODA Drawings.NET                                        */
      /**********************************************************************/
      TD_Db.odInitialize(Serv);

      OdDbDatabase pDb = HostApp.readFile(inFile, false, false, FileShareMode.kShareDenyNo);

      if (pDb != null)
      {
        OdRxModule module = Globals.odrxDynamicLinker().loadModule("TD_ColladaExport");
        ColladaModule colladaModule = new ColladaModule(OdRxModule.getCPtr(module).Handle, false);
        if (colladaModule != null)
        {
          OdResult res = colladaModule.exportCollada(pDb, outFile, AllPalettes.getLightPalette(), 256);
          if (OdResult.eOk == res)
          {
            Console.WriteLine("Successfully exported.");
          }
          else
          {
            String tmp = "Error : " + (new OdError(res)).description();
            Console.WriteLine(tmp);
          }
        }
        else
        {
          Console.WriteLine("Cannot load module : TD_ColladaExport");
        }
      }

      Console.WriteLine("Done.");
      /**********************************************************************/
      /* Uninitialize ODA Drawings.NET                                      */
      /**********************************************************************/
      mMan.StopTransaction(mStartTrans);

      TD_Db.odUninitialize();
    }
  }
}
