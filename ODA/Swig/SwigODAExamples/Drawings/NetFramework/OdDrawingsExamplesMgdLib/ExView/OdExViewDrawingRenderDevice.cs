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
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.ExView
{
  /// <summary>
  /// This example specific render device. In this case for rendering OdDbDatabase
  /// Class contains overrides methods for creation specific render device
  /// </summary>
  public class OdExViewDrawingRenderDevice : OdExViewProductRenderDeviceBase
  {

    /// <summary>
    /// Creation instance with specific graphics module type
    /// </summary>
    /// <param name="gsModule"></param>
    public OdExViewDrawingRenderDevice(OdExGsDeviceCreator.OdExGsModuleType gsModule) : base(gsModule)
    { }

    /// <summary>
    /// Creation instance with by default graphics module type.
    /// Module type by default WinGLES2.
    /// </summary>
    public OdExViewDrawingRenderDevice() : this(OdExGsDeviceCreator.OdExGsModuleType.WinGLES2)
    {
    }

    /// <summary>
    /// Creation graphic context is according to database of OdDbDatabase type
    /// </summary>
    /// <returns></returns>
    protected override OdGiContext Create_GiContext()
    {
      OdDbDatabase database = OdDbDatabase.cast(m_Database);
      OdGiContextForDbDatabase ctx = OdGiContextForDbDatabase.createObject();
      ctx.enableGsModel(true);
      ctx.setDatabase(database);
      return ctx;
    }

    /// <summary>
    /// Creation specific graphics layout helper is according to database of OdDbDatabase type
    /// </summary>
    /// <param name="gsDevice"></param>
    /// <param name="giContext"></param>
    /// <returns></returns>
    protected override OdGsDevice CreateGsDevice(OdGsDevice gsDevice, OdGiContext giContext)
    {
      OdGiContextForDbDatabase dwgGiContext = OdGiContextForDbDatabase.cast(giContext);

      OdGsLayoutHelper hDevice = TD_Db.setupActiveLayoutViews(gsDevice, dwgGiContext);
      OdDbDatabase db = dwgGiContext.getDatabase();

      if (hDevice.properties().has("UseVisualStyles"))
      {
        OdDbDictionary dic = db.getVisualStyleDictionaryId().safeOpenObject() as OdDbDictionary;
        hDevice.activeView().setVisualStyle(dic.getAt(OdExGsDeviceHelpers.RenderModeToVisualStyle(m_RenderMode)).AsOdDbStubPointer());
      }

      return hDevice;
    }

    /// <summary>
    /// Get Active view from GsDevice for dwg database
    /// </summary>
    /// <returns></returns>
    protected override OdGsView ActiveView()
    {
      OdGsLayoutHelper gsHelper = OdGsLayoutHelper.cast(m_GsDevice);
      return gsHelper.activeView();
    }

    /// <summary>
    /// Example zoom to extents for database of OdDbDatabase type
    /// </summary>
    /// <param name="database"></param>
    protected override void OnZoomToExtents(OdRxObject database)
    {
      OdDbDatabase dwgDatabase = OdDbDatabase.cast(database);
      if (dwgDatabase == null)
        throw new Exception("dwgDatabase is NULL");

      OdDbObject pVpObj = OdExAuxUtilities.active_viewport_id(dwgDatabase).openObject(OpenMode.kForWrite);

      OdDbAbstractViewportData pAVD = OdDbAbstractViewportData.cast(pVpObj);
      OdGsView pView = pAVD.gsView(pVpObj);
      // do actual zooming - change GS view
      // here protocol extension is used again, that provides some helpful functions
      OdAbstractViewPE pVpPE = OdAbstractViewPE.cast(pView);

      OdGeBoundBlock3d bbox = new OdGeBoundBlock3d();
      bool bBboxValid = pVpPE.viewExtents(pView, bbox);

      // paper space overall view    
      if (OdGsView.cast(pVpObj) != null && (OdGsView.cast(pVpObj)).device().viewAt(1) == pView)
      {
        if (!bBboxValid || !(bbox.minPoint().x < bbox.maxPoint().x && bbox.minPoint().y < bbox.maxPoint().y))
        {
          bBboxValid = OdExAuxUtilities.get_layout_extents(dwgDatabase, pView, ref bbox);
        }
      }
      else if (!bBboxValid) // model space viewport
      {
        bBboxValid = OdExAuxUtilities.get_layout_extents(dwgDatabase, pView, ref bbox);
      }

      if (!bBboxValid)
      {
        // set to somewhat reasonable (e.g. paper size)
        if (dwgDatabase.getMEASUREMENT() == MeasurementValue.kMetric)
        {
          bbox.set(OdGePoint3d.kOrigin, new OdGePoint3d(297.0, 210.0, 0.0)); // set to papersize ISO A4 (portrait)
        }
        else
        {
          bbox.set(OdGePoint3d.kOrigin, new OdGePoint3d(11.0, 8.5, 0.0)); // ANSI A (8.50 x 11.00) (landscape)
        }
        bbox.transformBy(pView.viewingMatrix());
      }

      pVpPE.zoomExtents(pView, bbox);
    }

  }
}