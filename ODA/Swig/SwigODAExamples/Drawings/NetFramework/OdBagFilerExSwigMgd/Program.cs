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
/* This console application demonstrates the use of the oddbEntGet()    */
/* function.  See dumpObject()                                          */
/*                                                                      */
/* Calling sequence:                                                    */
/*                                                                      */
/*    OdBagFilerExSwigMgd  <filename>                                   */
/*                                                                      */
/************************************************************************/

using System;
using Teigha.Core;
using Teigha.TD;

namespace OdBagFilerExSwigMgd
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
    public override String product()
    {
      return String.Format("Drawings.NET");
    }
  }

  class Program
  {
    static Program()
    {
      OdCreationNetSwigExampleLib.Helpers.OdNetSmokeTest.Use();
    }

    static void ShowUsage()
    {
      Console.WriteLine("usage: OdBagFilerExSwigMgd <filename>\n");
    }

    static void Main(string[] args)
    {
      MemoryTransaction mStartTrans = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OdExHostAppServices hostServices = new OdExHostAppServices();
        OdExSystemServices svcs = new OdExSystemServices();
        hostServices.setRecomputeDimBlocksRequired(false);
        TD_Db.odInitialize(svcs);

        Console.WriteLine("OdBagFilerExSwigMgd developed using {0} ver {1}",
          hostServices.product(), hostServices.versionString());

        if (args.Length != 1)
        {
          ShowUsage();
          return;
        }

        string inputFile = args[0];
        OdDbDatabase pDb = hostServices.readFile(inputFile);
        dump(pDb, hostServices);

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
        MemoryManager.GetMemoryManager().StopTransaction(mStartTrans);
        TD_Db.odUninitialize();
      }
    }


    /************************************************************************/
    /* Dump the specified database to the specified output stream           */
    /************************************************************************/
    public static void dump(OdDbDatabase pDb, OdDbHostAppServices pHostApp)
    {
      if (pDb == null)
        return;
      dumpLTypes(pDb);
      dumpBlocks(pDb);
    }

    /************************************************************************/
    /* Dump the line types table from the specified database to the specified    */
    /* output stream                                                        */
    /************************************************************************/
    public static void dumpLTypes(OdDbDatabase pDb)
    {
      OdDbSymbolTable pLTypes = (OdDbSymbolTable)pDb.getLinetypeTableId().safeOpenObject();

      Console.WriteLine("\nLine Types:");
      OdDbSymbolTableIterator pLTypesIter = pLTypes.newIterator();

      for (pLTypesIter.start(); !pLTypesIter.done(); pLTypesIter.step())
      {
        OdDbSymbolTableRecord pLType = (OdDbSymbolTableRecord)pLTypesIter.getRecordId().safeOpenObject();

        dumpObject(pLType.objectId());
        Console.WriteLine();
      }
    }

    /************************************************************************/
    /* Dump the block table from the specified database to the specified    */
    /* output stream                                                        */
    /************************************************************************/
    public static void dumpBlocks(OdDbDatabase pDb)
    {
      OdDbBlockTable pBlocks = (OdDbBlockTable)pDb.getBlockTableId().safeOpenObject();

      Console.WriteLine("\nBlocks:");
      OdDbSymbolTableIterator pBlkIter = pBlocks.newIterator();

      for (pBlkIter.start(); !pBlkIter.done(); pBlkIter.step())
      {
        OdDbBlockTableRecord pBlock = (OdDbBlockTableRecord)pBlkIter.getRecordId().safeOpenObject();

        dumpObject(pBlock.objectId());
        Console.WriteLine();

        OdDbObjectIterator pEntIter = pBlock.newIterator();

        for (; !pEntIter.done(); pEntIter.step())
        {
          dumpObject(pEntIter.objectId());
        }
      }
    }

    /************************************************************************/
    /* Dump the specified object object to the specified output stream      */
    /************************************************************************/
    public static void dumpObject(OdDbObjectId id)
    {
      Console.WriteLine("--------------------");
      for (OdResBuf rb = TD_Db.oddbEntGet(id); rb != null; rb = rb.next())
      {
        Console.WriteLine("{0}", FormatCode(rb));
        Console.WriteLine("{0}", FormatValue(rb));
      }
    }

    /************************************************************************/
    /* Format the value from the specified ResBuf                           */
    /************************************************************************/
    public static String FormatValue(OdResBuf rb)
    {
      String s = String.Empty;

      if (rb.restype() == (int)OdResBuf.ValueType.kRtEntName || rb.restype() == (int)OdResBuf.ValueType.kDxfEnd)
      {
        OdDbObjectId id = rb.getObjectId(null);
        s = id.getHandle().ascii();
        return s;
      }

      switch (rb.restype())
      {
        case (int)OdResBuf.ValueType.kRtColor:
          s = OdDbUnitsFormatter.formatColor(rb.getColor());
          break;
        default:
          switch (OdDxfCode._getType(rb.restype()))
          {
            case OdDxfCode.Type.Name:
            case OdDxfCode.Type.String:
            case OdDxfCode.Type.Handle:
            case OdDxfCode.Type.LayerName:
              s = rb.getString();
              break;
            case OdDxfCode.Type.Bool:
              s = rb.getBool().ToString();
              break;
            case OdDxfCode.Type.Integer8:
              s = rb.getInt8().ToString();
              break;
            case OdDxfCode.Type.Integer16:
              s = rb.getInt16().ToString();
              break;
            case OdDxfCode.Type.Integer32:
              s = rb.getInt32().ToString();
              break;
            case OdDxfCode.Type.Integer64:
              s = rb.getInt64().ToString();
              break;
            case OdDxfCode.Type.Double:
            case OdDxfCode.Type.Angle:
              s = rb.getDouble().ToString();
              break;
            case OdDxfCode.Type.Point:
              s = String.Format("({0}, {1}, {2})", rb.getPoint3d().x, rb.getPoint3d().y, rb.getPoint3d().z);
              break;
            case OdDxfCode.Type.ObjectId:
            case OdDxfCode.Type.SoftPointerId:
            case OdDxfCode.Type.HardPointerId:
            case OdDxfCode.Type.SoftOwnershipId:
            case OdDxfCode.Type.HardOwnershipId:
              s = rb.getHandle().ascii();
              break;
            default:
              break;
          }
          break;
      }
      return s;
    }

    /************************************************************************/
    /* Format the code from the specified ResBuf                            */
    /************************************************************************/
    public static String FormatCode(OdResBuf rb)
    {
      return String.Format("type: {0}", rb.restype());
    }

  }
}
