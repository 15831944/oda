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
using Teigha.TG;
using System.IO;

namespace ExDgnVectorizeMgd
{
  #region Declaration System & Host Application Services
  class OdExSystemServices : RxSystemServicesImpl
  {
    public OdExSystemServices()
    {
      Teigha.Core.Globals.odActivate(ActivationData.userInfo, ActivationData.userSignature);
    }
  }

  class OdExHostAppServices : OdExDgnHostAppServices
  {
    public override OdDgDatabase readFile(String fileName)
    {
      if (!File.Exists(fileName))
        return null;
      return base.readFile(fileName);
    }
    public override OdDgDatabase readFile(String fileName, FileShareMode sm)
    {
      if (!File.Exists(fileName))
        return null;
      return base.readFile(fileName, sm);
    }
  }
  #endregion

  class Program
  {
    static Program()
    {
      OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
    }

    static void Main(string[] args)
    {
      if (args.Length < 1)
      {
        Console.WriteLine("Usage: ExDgnVectorizeMgd <filename>");
        return;
      }
      if (!File.Exists(args[0]))
      {
        Console.WriteLine(string.Format("File {0} does not exist", args[0]));
        return;
      }

      MemoryManager mMan = MemoryManager.GetMemoryManager();
      MemoryTransaction mStartTrans = mMan.StartTransaction();

      OdExSystemServices systemServices = new OdExSystemServices();
      Teigha.Core.Globals.odrxInitialize(systemServices);
      Teigha.Core.Globals.odgsInitialize();
      Teigha.Core.Globals.odrxDynamicLinker().loadModule("TG_Db");

      try
      {
        for (int loop = 0; loop < 2; loop++)
        {
          MemoryTransaction mTr = mMan.StartTransaction();

          OdExHostAppServices hostApp = new OdExHostAppServices();
          OdDgDatabase pDb = hostApp.readFile(args[0]);
          if (pDb == null)
            throw new Exception(String.Format("Can't open {0}", args[0]));

          OdGsDevice pDevice = new OdExGsSimpleDevice();
          OdDgViewGroup pViewGroup = (OdDgViewGroup)pDb.getActiveViewGroupId().openObject();
          if (pViewGroup != null)
          {
            OdDgElementId vectorizedModelId = pDb.getActiveModelId();
            OdDgElementIterator pIt = pViewGroup.createIterator();
            OdGsDCRect screenRect = new OdGsDCRect(0, 1000, 0, 1000);
            for (; !pIt.done(); pIt.step())
            {
              OdDgView pView = pIt.item().openObject() as OdDgView;
              if (pView != null && pView.getVisibleFlag())
              {
                //create the context with OdDgView element given (to transmit some properties)
                OdGiContextForDgDatabase pDgnContext = OdGiContextForDgDatabase.createObject(pDb, pView);
                OdDgElementId vectorizedViewId = pIt.item();
                pDevice = OdGsDeviceForDgModel.setupModelView(vectorizedModelId, vectorizedViewId, pDevice, pDgnContext);

                pDevice.onSize(screenRect);
                pDevice.update();
              }
            }
          }
          mMan.StopTransaction(mTr);
        }
      }
      catch (Exception err)
      {
        Console.WriteLine(err.Message);
      }
      finally
      {
        mMan.StopTransaction(mStartTrans);
      }

      Teigha.Core.Globals.odgsUninitialize();
      Teigha.Core.Globals.odrxUninitialize();
    }
  }
}
