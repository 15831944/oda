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

namespace OdExNonCOMPropertiesSwig
{
  public class OdExHostAppServices : ExHostAppServices
  { }

  class Program
  {
    static Program()
    {
      OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
    }

    private static String sUsage =
    "Usage: OdExNonCOMPropertiesSwig <inputFilePath>\n" +
    "  <inputFilePath>  - full path to the input dwg file\n" +
    "\nPress ENTER to continue...\n";

    static void Main(string[] args)
    {
      if (args.Length < 1)
      {
        Console.WriteLine(sUsage);
        return;
      }

      String dwgFile = args[0];

      /**********************************************************************/
      /* Start Root Memory Transaction                                      */
      /**********************************************************************/
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      /**********************************************************************/
      /* Initialize Runtime Extension environment                           */
      /**********************************************************************/
      OdExSystemServices svcs = new OdExSystemServices();
      OdExHostAppServices hostAppServices = new OdExHostAppServices();
      hostAppServices.disableOutput(true);

      TD_Db.odInitialize(svcs);

      try
      {
        if (Globals.odrxDynamicLinker().loadModule("RxProperties") == null)
          throw new OdError("Module RxProperties is missing");
        if (Globals.odrxDynamicLinker().loadModule("DbProperties") == null)
          throw new OdError("Module DbProperties is missing");

        OdDbDatabase database = hostAppServices.readFile(dwgFile, true, false, FileShareMode.kShareDenyNo, "");
        OdDbBlockTableRecord modelspace = (OdDbBlockTableRecord)database.getModelSpaceId().safeOpenObject();
        OdDbObjectIterator iter = modelspace.newIterator();
        Console.WriteLine("---------------Start model space dump------------------");
        Console.WriteLine();
        for (iter.start(); !iter.done(); iter.step())
          dumpObject(iter.entity());
        Console.WriteLine();
        Console.WriteLine("---------------End model space dump--------------------");
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

    private static void dumpObject(OdDbObject pObj)
    {
      if (pObj == null)
        return;
      Console.WriteLine(String.Format("Dumping properties for {0} <{1}>", pObj.isA().name(), pObj.objectId().getHandle().ascii()));
      Console.WriteLine("---------------------------------------------------------");
      for (OdRxMemberIterator pMi = OdRxMemberQueryEngine.theEngine().newMemberIterator(pObj); !pMi.done(); pMi.next())
      {
        OdRxProperty pProp = OdRxProperty.cast(pMi.current());
        if (pProp != null)
          dumpProperty(pObj, pProp);
      }
      Console.WriteLine("---------------------------------------------------------");
    }

    private static void dumpProperty(OdRxObject pObj, OdRxProperty pProp)
    {
      String s = pProp.name() + " = ";
      try
      {
        OdRxValue v = OdRxValue.create(), converted = OdRxValue.create();
        if (OdResult.eOk == pProp.getValue(pObj, v))
        {
          if (v.type().IsEqual(OdRxValueTypeDesc_KERNEL.value_Desc_OdString()))
            s += OdRxValueHelper.rxvalue_cast<String>(v);
          else if (v.type().toValueType(OdRxValueTypeDesc_KERNEL.value_Desc_OdString(), v, converted))
            s += OdRxValueHelper.rxvalue_cast<String>(converted);
          else if (v.type().isEnum())
            s += v.getEnumTag().name();
          else
          {
            OdRxDisplayAsAttribute dsa = (OdRxDisplayAsAttribute)pProp.attributes().get(OdRxDisplayAsAttribute.desc());
            if (dsa != null)
              s += dsa.getDisplayValue(v);
            else if (v.type().IsEqual(OdRxValueTypeDesc_KERNEL.value_Desc_int()))
              s += v.toString();
            else if (v.type().toValueType(OdRxValueTypeDesc_KERNEL.value_Desc_int(), v, converted))
              s += converted.toString();
            else // fallback
              s += v.toString();
          }
        }
      }
      catch (OdError err)
      {
        s += String.Format("Read Error: {0}", err.description());
      }
      finally
      {
        Console.WriteLine("   {0}", s);
      }
    }
  }
}
