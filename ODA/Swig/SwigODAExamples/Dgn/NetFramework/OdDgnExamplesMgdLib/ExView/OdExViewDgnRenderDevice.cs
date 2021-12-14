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

using OdKernelExamplesMgdLib.ExView;
using System;
using Teigha.Core;
using Teigha.TG;

namespace OdDgnExamplesMgdLib.ExView
{
  /// <summary>
  /// This example specific render device. In this case for rendering OdDgDatabase
  /// Class contains overrides methods for creation specific render device
  /// </summary>
  public class OdExViewDgnRenderDevice : OdExViewProductRenderDeviceBase
  {
    private OdDgElementId m_ModelId = null;
    private OdDgElementId m_ViewId = null;

    public void PrepareInit(OdDgElementId modelId, OdDgElementId viewId)
    {
      m_ModelId = modelId;
      m_ViewId = viewId;
    }

    /// <summary>
    /// Creation instance with specific graphics module type
    /// </summary>
    /// <param name="gsModule"></param>
    public OdExViewDgnRenderDevice(OdExGsDeviceCreator.OdExGsModuleType gsModule) : base(gsModule)
    { }

    /// <summary>
    /// Creation instance with by default graphics module type.
    /// Module type by default WinGLES2.
    /// </summary>
    public OdExViewDgnRenderDevice() : this(OdExGsDeviceCreator.OdExGsModuleType.WinGLES2)
    {
    }

    /// <summary>
    /// Creation graphic context is according to database of OdDgDatabase type
    /// </summary>
    /// <returns></returns>
    protected override OdGiContext Create_GiContext()
    {
      OdDgDatabase dgnDatabase = OdDgDatabase.cast(m_Database);
      OdGiContextForDgDatabase ctx = OdGiContextForDgDatabase.createObject(dgnDatabase, OdDgView.cast(m_ViewId.openObject()));
      ctx.enableGsModel(true);
      ctx.setDatabase(dgnDatabase);
      return ctx;
    }

    /// <summary>
    /// Creation specific graphics layout helper is according to database of OdDgDatabase type
    /// </summary>
    /// <param name="gsDevice"></param>
    /// <param name="giContext"></param>
    /// <returns></returns>
    protected override OdGsDevice CreateGsDevice(OdGsDevice gsDevice, OdGiContext giContext)
    {
      OdGiContextForDgDatabase dgnGiContext = OdGiContextForDgDatabase.cast(giContext);
      OdGsDeviceForDgModel hDevice = OdGsDeviceForDgModel.setupModelView(m_ModelId, m_ViewId, gsDevice, dgnGiContext);
      return hDevice;
    }

    /// <summary>
    /// Get Active view from GsDevice for dgn database
    /// </summary>
    /// <returns></returns>
    protected override OdGsView ActiveView()
    {
      OdGsDeviceForDgModel gsHelper = OdGsDeviceForDgModel.cast(m_GsDevice);
      return gsHelper.activeView();
    }

    /// <summary>
    /// Example zoom to extents for database of OdDgDatabase type
    /// </summary>
    /// <param name="database"></param>
    protected override void OnZoomToExtents(OdRxObject database)
    {
      // it is not calculated yet for perspective cameras
      // initial actions for perspective camera
      // find a better solution
      OdGsView view = ActiveView();
      if (view.isPerspective())
      {
        view.zoom(1.0);
      }

      OdDgDatabase db = OdDgDatabase.cast(m_Database);

      OdDgModel pModel = OdDgModel.cast(db.getActiveModelId().openObject());

      //get the extents of that pModel
      OdGeExtents3d extents = new OdGeExtents3d();
      pModel.getGeomExtents(m_ViewId, extents);
      if (!extents.isValidExtents())
      {
        return;
      }

      //get all points
      OdGePoint3d[] realPoints = new OdGePoint3d[8];
      {
        OdGePoint3d minPt = extents.minPoint();
        OdGePoint3d maxPt = extents.maxPoint();

        realPoints[0] = minPt;
        realPoints[1] = maxPt;
        realPoints[2] = new OdGePoint3d(minPt.x, maxPt.y, minPt.z);
        realPoints[3] = new OdGePoint3d(maxPt.x, maxPt.y, minPt.z);
        realPoints[4] = new OdGePoint3d(maxPt.x, minPt.y, minPt.z);
        realPoints[5] = new OdGePoint3d(minPt.x, minPt.y, maxPt.z);
        realPoints[6] = new OdGePoint3d(minPt.x, maxPt.y, maxPt.z);
        realPoints[7] = new OdGePoint3d(maxPt.x, minPt.y, maxPt.z);
      }

      //get 'relative' points
      OdGePoint2d[] relativePoints = new OdGePoint2d[8];
      for (int i = 0; i < 8; i++)
        relativePoints[i] = new OdGePoint2d();

      OdGePoint3d position = view.position(), target = view.target();
      OdGeVector3d n = view.upVector(), m = (target - position).crossProduct(n).normal();
      {
        for (int i = 0; i < 8; i++)
        {
          relativePoints[i].x = (realPoints[i] - position).dotProduct(m);
          relativePoints[i].y = (realPoints[i] - position).dotProduct(n);
        }
      }

      //get characteristic points
      OdGeVector2d min = new OdGeVector2d();
      OdGeVector2d medium = new OdGeVector2d();
      OdGeVector2d max = new OdGeVector2d();
      {
        max.x = relativePoints[0].x;
        max.y = relativePoints[0].y;

        min = new OdGeVector2d(max);

        for (int i = 0; i < 8; i++)
        {
          if (min.x > relativePoints[i].x)
          {
            min.x = relativePoints[i].x;
          }
          if (max.x < relativePoints[i].x)
          {
            max.x = relativePoints[i].x;
          }
          if (min.y > relativePoints[i].y)
          {
            min.y = relativePoints[i].y;
          }
          if (max.y < relativePoints[i].y)
          {
            max.y = relativePoints[i].y;
          }
        }

        medium = (max + min) / 2.0;
      }

      //shift the camera (if new size is not zero; it is prohibited by Ge library)
      if (min.x < max.x || min.y < max.y)
      {
        OdGePoint3d pPos = position + m * medium.x + n * medium.y;
        OdGePoint3d pTar = target + m * medium.x + n * medium.y;
        OdGeVector3d vDir = pTar - pPos;
        double dirLen = vDir.length();
        if (Globals.OdZero(dirLen, 10e-8))
        {
          pTar += new OdGeVector3d(0, 0, -1);
          vDir = pTar - pPos;
          dirLen = vDir.length();
        }
        double targetLen = pTar.asVector().length();

        // #9204 (Possibly Invalid Model extents - cannot zoom to full extents)
        //        As a result 'position' point is equal 'target' point that it is wrong.
        if (targetLen > 1.0 && Globals.OdZero(dirLen / targetLen, 10e-8))
        {
          view.setView(pPos.Div(dirLen) * targetLen, pTar, n,
            (max.x - min.x) * 1.1, (max.y - min.y) * 1.1, view.isPerspective() ? OdGsView.Projection.kPerspective : OdGsView.Projection.kParallel);
        }
        else
        {
          view.setView(pPos, pTar, n,
            (max.x - min.x) * 1.1, (max.y - min.y) * 1.1, view.isPerspective() ? OdGsView.Projection.kPerspective : OdGsView.Projection.kParallel);
        }
      }
    }
  }
}