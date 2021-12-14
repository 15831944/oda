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
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib
{
  public static class OdExAuxUtilities
  {
    public static OdDbObjectId activeBlockTableRecord(OdDbDatabase pDb)
    {
      OdDbObjectId idSpace = pDb.getActiveLayoutBTRId();
      if (idSpace == pDb.getPaperSpaceId())
      {
        OdDbViewport pVp = pDb.activeViewportId().safeOpenObject() as OdDbViewport;
        if (pVp.number() > 1)
          idSpace = pDb.getModelSpaceId();
      }
      return idSpace;
    }

    public static OdGeExtents3d CalcExtents(IEnumerable<OdDbEntity> entities)
    {
      OdGeExtents3d extents = new OdGeExtents3d();
      foreach (var ent in entities)
      {
        if (ent == null)
          continue;
        OdGeExtents3d tEntExt = new OdGeExtents3d();
        if (ent.getGeomExtents(tEntExt) == OdResult.eOk)
          extents.addExt(tEntExt);
      }
      return extents;
    }

    public static OdGeVector3d ucsNormal(OdDbDatabase pDb)
    {
      OdGeVector3d vNormal = new OdGeVector3d(OdGeVector3d.kZAxis);
      OdDbObjectId idSpace = activeBlockTableRecord(pDb);
      if (idSpace == pDb.getModelSpaceId())
      {
        OdDbObjectId idVp = pDb.activeViewportId();
        OdDbObject pVp = idVp.openObject();
        OdAbstractViewPE pAVp = OdAbstractViewPE.cast(pVp);
        if (pAVp != null)
        {
          OdGePoint3d ptOrigin = new OdGePoint3d();
          OdGeVector3d vX = new OdGeVector3d(), vY = new OdGeVector3d();
          pAVp.getUcs(pVp, ptOrigin, vX, vY);
          vNormal = vX.crossProduct(vY).normalize();
        }
      }
      return vNormal;
    }

    public static OdDbObjectId active_viewport_id(OdDbDatabase database)
    {
      if (database.getTILEMODE())
      {
        OdDbViewportTable pVPT = (OdDbViewportTable)database.getViewportTableId().safeOpenObject();
        OdDbObjectId pActiveVPId = pVPT.getActiveViewportId();

        return pActiveVPId;
      }
      else
      {
        using (OdDbBlockTableRecord paperBTR = (OdDbBlockTableRecord)database.getActiveLayoutBTRId().openObject(OpenMode.kForRead))
        {
          OdDbLayout l = (OdDbLayout)paperBTR.getLayoutId().openObject(OpenMode.kForRead);
          return l.activeViewportId();
        }
      }
    }

    public static bool preparePlotstyles(OdDbDatabase database, OdGiContextForDbDatabase ctx)
    {
      bool acceptPlotStyles = false;
      using (OdDbBlockTableRecord paperBTR = (OdDbBlockTableRecord)database.getActiveLayoutBTRId().openObject(OpenMode.kForRead))
      {
        using (OdDbLayout pLayout = (OdDbLayout)paperBTR.getLayoutId().openObject(OpenMode.kForRead))
        {
          if (ctx.isPlotGeneration() ? pLayout.plotPlotStyles() : pLayout.showPlotStyles())
          {
            String pssFile = pLayout.getCurrentStyleSheet();
            if (pssFile.Length > 0)
            {
              String path = database.appServices().findFile(pssFile, database, OdDbBaseHostAppServices.FindFileHint.kDefault);
              if (path.Length > 0)
              {
                using (OdStreamBuf pFileBuf = Globals.odrxSystemServices().createFile(path))
                {
                  ctx.loadPlotStyleTable(pFileBuf);
                  acceptPlotStyles = true;
                }
              }
            }
          }
        }
      }
      return acceptPlotStyles;
    }

    public static bool get_layout_extents(OdDbDatabase db, OdGsView pView, ref OdGeBoundBlock3d bbox)
    {
      OdDbBlockTable bt = (OdDbBlockTable)db.getBlockTableId().openObject(OpenMode.kForRead);
      OdDbBlockTableRecord pSpace = (OdDbBlockTableRecord)bt.getPaperSpaceId().openObject(OpenMode.kForRead);
      OdDbLayout pLayout = (OdDbLayout)pSpace.getLayoutId().openObject(OpenMode.kForRead);
      OdGeExtents3d ext = new OdGeExtents3d();
      if (pLayout.getViewportArray().Count > 0)
      {
        ext.transformBy(pView.viewingMatrix());
        bbox.set(ext.minPoint(), ext.maxPoint());
      }
      else
      {
        pLayout.getGeomExtents(ext);
      }
      bbox.set(ext.minPoint(), ext.maxPoint());
      return ext.minPoint() != ext.maxPoint();
    }

    public static void SetViewportBorderProperties(OdGsLayoutHelper pDevice, bool bModel, uint color7)
    {
      int n = pDevice.numViews();

      if (n > 1)
      {
        for (int i = bModel ? 0 : 1; i < n; ++i)
        {
          // Get the viewport
          OdGsView pView = pDevice.viewAt(i);

          // Make it visible
          pView.setViewportBorderVisibility(true);

          // Set the color and width
          pView.setViewportBorderProperties(color7, 1);
        }
      }
    }

    public static void InvertColorPalette(OdGsLayoutHelper _hDevice)
    {
      if (_hDevice == null)
        return;
      uint[] CurPalette = null;
      uint bgcolor = _hDevice.getBackgroundColor();
      if (bgcolor == 0 /*0x00ffffff*/)
        CurPalette = Teigha.Core.AllPalettes.getLightPalette();
      else
        CurPalette = Teigha.Core.AllPalettes.getDarkPalette();

      _hDevice.setBackgroundColor(CurPalette[0]);
      _hDevice.setLogicalPalette(CurPalette, 256);
      _hDevice.invalidate();
      if (_hDevice.gsModel() != null)
        _hDevice.gsModel().invalidate(OdGsModel.InvalidationHint.kInvalidateAll);
    }

    public static void DoSnapShot(OdGsLayoutHelper _hDevice, OdGsDCRect gsRect, uint imgPixelWidth, uint imgPixelHeight, String bmpSaveFilePath)
    {
      if (_hDevice == null ||
        gsRect == null ||
        imgPixelWidth == 0 ||
        imgPixelHeight == 0 ||
        String.IsNullOrEmpty(bmpSaveFilePath))
        return;

      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      try
      {
        OdRxRasterServices RasSvcs = (OdRxRasterServices)Teigha.Core.Globals.odrxDynamicLinker().loadApp("RxRasterServices");
        if (RasSvcs == null)
          throw new Exception("Failed to load RxRasterServices module");

        OdGiRasterImage img = OdGiRasterImageDesc.createObject(imgPixelWidth, imgPixelHeight, OdGiRasterImage.Units.kInch);

        _hDevice.getSnapShot(out img, gsRect);

        bool statusSave = RasSvcs.saveRasterImage(img, bmpSaveFilePath);

        if (!statusSave)
          throw new Exception("Failed save image operation");
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

    public static OdGsView activeTopView(OdDbDatabase _db, OdGsLayoutHelper _hDevice)
    {
      OdGsView pView = _hDevice.activeView();
      if (!_db.getTILEMODE())
      {
        OdDbObject pVpObj = _db.activeViewportId().safeOpenObject();
        OdDbAbstractViewportData pAVD = OdDbAbstractViewportData.cast(pVpObj);
        if (pAVD != null && pAVD.gsView(pVpObj) != null)
          pView = pAVD.gsView(pVpObj);
      }
      return pView;
    }
  }
}