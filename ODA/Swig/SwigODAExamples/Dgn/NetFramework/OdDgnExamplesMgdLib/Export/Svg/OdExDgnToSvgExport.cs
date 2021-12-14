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


using OdKernelExamplesMgdLib.Export.Svg;
using System;
using Teigha.Core;
using Teigha.TG;

namespace OdDgnExamplesMgdLib
{
  public static class OdExDgnToSvgExport
  {
    public static void Export(OdDgDatabase db, String outFilePath)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        PrepareActiveGroup(db);

        OdGiContextForDgDatabase pDgnContext = CreateGiContext(db);

        uint backgroudColor = GetBackgroudColor(db);

        uint[] pallete = OdDgColorTable.currentPalette(db);

        OdExSvgExport.Export(pDgnContext, outFilePath, pallete, backgroudColor);
      }
      catch (Exception err)
      {
        throw err;
      }
      finally
      {
        MemoryManager.GetMemoryManager().StopTransaction(mTr);
      }
    }

    private static void PrepareActiveGroup(OdDgDatabase db)
    {
      OdDgElementId vectorizedModelId = GetVectorizedModelId(db);

      PrepareActiveGroup(vectorizedModelId, db);
    }

    private static OdDgElementId GetVectorizedModelId(OdDgDatabase db)
    {
      OdDgElementId vectorizedModelId;

      vectorizedModelId = db.getActiveModelId();
      if (vectorizedModelId.isNull())
      {
        vectorizedModelId = db.getDefaultModelId();
        db.setActiveModelId(vectorizedModelId);
      }
      if (vectorizedModelId.isNull())
        throw new Exception("vectorizedModelId is NULL");

      return vectorizedModelId;
    }

    private static void PrepareActiveGroup(OdDgElementId vectorizedModelId, OdDgDatabase db)
    {
      OdDgViewGroup pViewGroup;
      {
        pViewGroup = OdDgViewGroup.cast(db.getActiveViewGroupId().openObject());
        if (pViewGroup == null)
        {
          pViewGroup = OdDgViewGroup.cast(db.recommendActiveViewGroupId().openObject());

          if (pViewGroup == null)
          {
            // Add View group
            OdDgModel pModel = OdDgModel.cast(vectorizedModelId.openObject());
            pModel.createViewGroup();
            pModel.fitToView();
            pViewGroup = OdDgViewGroup.cast(db.recommendActiveViewGroupId().openObject());
          }
        }
        if (pViewGroup == null)
          throw new Exception("pViewGroup is NULL");
        db.setActiveViewGroupId(pViewGroup.elementId());
      }
    }

    private static OdGiContextForDgDatabase CreateGiContext(OdDgDatabase db)
    {
      OdGiContextForDgDatabase pDgnContext;

      pDgnContext = OdGiContextForDgDatabaseToExport.createObject();
      pDgnContext.setDatabase(db);

      // do not render paper
      pDgnContext.setPlotGeneration(true);

      return pDgnContext;
    }

    private static uint GetBackgroudColor(OdDgDatabase db)
    {
      uint backgroudColor;
      OdDgModel pModel = (OdDgModel)db.getActiveModelId().safeOpenObject();
      backgroudColor = pModel.getBackground();

      return backgroudColor;
    }
  }
}