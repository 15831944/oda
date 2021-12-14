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

/************************************************************************/
/* Simple application creating new DGN file                             */
/* and filling it with some objects                                     */
/************************************************************************/

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Teigha.Core;
using Teigha.TG;

namespace ExDgnSimpleProjectMgd
{
  class OdExSystemServices : RxSystemServicesImpl
  {
    public OdExSystemServices()
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
      // start memory manager
      MemoryManager mMan = MemoryManager.GetMemoryManager();
      // start first memory transaction
      MemoryTransaction mStartTrans = mMan.StartTransaction();

      Console.WriteLine("\nExDgnSimpleProjectMgd sample program. Copyright (c) 2019, Open Design Alliance\n");
      /**********************************************************************/
      /* Parse Command Line inputs                                          */
      /**********************************************************************/
      if (args.Length < 1)
      {
        Console.WriteLine("Usage: \nExDgnSimpleProjectMgd <filename>");
        return;
      }
      OdExSystemServices systemServices = new OdExSystemServices();
      Globals.odrxInitialize(systemServices);
      Globals.odgsInitialize();
      OdExDgnHostAppServices hostAppServices = new OdExDgnHostAppServices();

      Globals.odrxDynamicLinker().loadModule("TG_Db");
      try
      {
        OdStringArray arrayRscList = new OdStringArray();
        arrayRscList.Add("font.rsc");
        hostAppServices.setMS_SYMBRSRC(arrayRscList);

        /********************************************************************/
        /* Create a default OdDgDatabase object                             */
        /********************************************************************/
        OdDgDatabase pDb = hostAppServices.createDatabase();

        /********************************************************************/
        /* Fill the database                                                */
        /********************************************************************/

        // Filling the database procedure will create many different objects - wrap it in a memory transaction
        MemoryTransaction mTrans = mMan.StartTransaction();

        OdDgModel pModel = (OdDgModel)pDb.getActiveModelId().safeOpenObject(OpenMode.kForWrite);
        pModel.setWorkingUnit(OdDgModel.WorkingUnit.kWuMasterUnit);
        pModel.setModelIs3dFlag(true);

        // Create new line
        OdDgLine3d line3d = OdDgLine3d.createObject();

        // Set endpoints
        line3d.setStartPoint(new OdGePoint3d(0, 0, 0));
        line3d.setEndPoint(new OdGePoint3d(10, 10, 10));
        line3d.setColorIndex((uint)OdCmEntityColor.ACIcolorMethod.kACIBlue);

        // Add the line to the model
        pModel.addElement(line3d);

        line3d.setColorIndex((uint)OdCmEntityColor.ACIcolorMethod.kACIBlue);

        OdDgLevelTableRecord pLevel = OdDgLevelTableRecord.createObject();
        pLevel.setName("Simple");
        pLevel.setElementColorIndex(3);
        OdDgLevelTable pLevelTable = pDb.getLevelTable(OpenMode.kForWrite);
        pLevelTable.add(pLevel);
        uint levelid = pLevel.getEntryId();
        line3d.setLevelEntryId(levelid);

        /********************************************************************/
        /* Set an appropriate set of views                                  */
        /********************************************************************/
        pModel.fitToView();

        mMan.StopTransaction(mTrans);

        /********************************************************************/
        /* Write the database                                               */
        /********************************************************************/
        // no additional actions - after stopping the memory transaction we are sure that all objects are closed
        pDb.writeFile(args[0]);
      }
      catch (System.Exception ex)
      {
        Console.WriteLine("Unhandled exception: {0}", ex.Message);
      }

      // stop the initial transaction
      mMan.StopTransaction(mStartTrans);

      Globals.odgsUninitialize();
      Globals.odrxUninitialize();
    }
  }
}
