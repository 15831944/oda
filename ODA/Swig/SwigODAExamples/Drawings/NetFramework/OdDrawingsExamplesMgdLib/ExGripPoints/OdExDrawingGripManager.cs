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
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.ExGripPoints
{
  public class OdExDrawingGripManager
  {
    OdGsLayoutHelper m_pDevice;
    OdExDrawingGripDataCollection m_hoverGripsColl;
    OdExDrawingGripDragCollection m_aDrags;
    OdExDrawingGripDataCollection aActiveKeys;

    public OdExDrawingGripManager()
    {
      m_hoverGripsColl = new OdExDrawingGripDataCollection();
      BasePoint = OdGePoint3d.kOrigin;
      LastPoint = OdGePoint3d.kOrigin;
      GripDataDict = new Dictionary<OdDbObjectId, OdExGripDataExt>();
      m_aDrags = new OdExDrawingGripDragCollection();
    }

    public void UnInit()
    {
      m_pDevice = null;
      Model = null;
      m_hoverGripsColl = null;
      GripDataDict = null;
      m_aDrags = null;
    }

    public void Init(OdGsLayoutHelper pDevice, OdGsModel gsModule, OdDbDatabase pDb/* OdDbCommandContext ctx*/ )
    {
      m_pDevice = pDevice;
      Model = gsModule;

      //pDb->addReactor(&m_cDbReactor);

      OdDbHostAppServices pAppSvcs = pDb.appServices();

      GRIPSIZE = pAppSvcs.getGRIPSIZE();
      GRIPOBJLIMIT = pAppSvcs.getGRIPOBJLIMIT();

      GRIPCOLOR = new OdCmEntityColor();
      GRIPCOLOR.setColorIndex(pAppSvcs.getGRIPCOLOR());

      GRIPHOVER = new OdCmEntityColor();
      GRIPHOVER.setColorIndex(pAppSvcs.getGRIPHOVER());

      GRIPHOT = new OdCmEntityColor();
      GRIPHOT.setColorIndex(pAppSvcs.getGRIPHOT());
      //m_pGetSelectionSetPtr = pGetSSet;

    }

    public void SetValue(OdGePoint3d ptValue)
    {
      foreach (OdExDrawingGripDrag grDrag in m_aDrags)
      {
        grDrag.cloneEntity(ptValue);
      }
      LastPoint = ptValue;
    }

    public bool OnMouseMove(int x, int y, bool bShift)
    {
      // Restarts hover operation.
      OdResult eRet = startHover(x, y, bShift);
      if (eRet == OdResult.eGripOpFailure)
        return false;
      else if (eRet == OdResult.eGripOpGetNewGripPoints)
      {
        throw new NotImplementedException();
      }
      return true;
    }

    private bool TriggerGripProcessing(int x, int y, bool bShift)
    {
      OdExDrawingGripDataCollection aKeys = new OdExDrawingGripDataCollection();
      locateGripsAt(x, y, ref aKeys);

      if (aKeys.Count == 0)
        return false;
      //{
      if (bShift)
      {
        // Modify Grip  status().
        DrawType eNewStatus = DrawType.kHotGrip;
        int i, iSize = aKeys.Count;
        for (i = 0; i < iSize; i++)
        {
          if (DrawType.kHotGrip == aKeys[i].Status)
          {
            eNewStatus = DrawType.kWarmGrip;
            break;
          }
        }

        for (i = 0; i < iSize; i++)
        {
          DrawType eCurStatus = eNewStatus;
          OdExDrawingGripData pGrip = aKeys[i];
          if (pGrip.Data != null)
          {
            if (pGrip.Data.triggerGrip())
            {
              eCurStatus = DrawType.kWarmGrip;
            }
            else
            {
              Globals.GripOperationPtrDelegate hotGripFuncDelegate = pGrip.Data.hotGripFunc();
              if (hotGripFuncDelegate != null)
              {
                int iFlags = (int)GripContextFlags.kMultiHotGrip;
                if (pGrip.Shared)
                  iFlags |= (int)GripContextFlags.kSharedGrip;

                OdResult eRet = hotGripFuncDelegate( pGrip.Data, pGrip.entityId.AsOdDbStubPointer(), iFlags);

                switch (eRet)
                {
                  case OdResult.eGripOpGripHotToWarm:
                    eCurStatus = DrawType.kWarmGrip;
                    break;
                  default:
                    // no op
                    break;
                }
              }
            }
          }
          aKeys[i].Status = eCurStatus;
        }
      }
      else // of if ( bShift )
      {
        // Launch Grip Edit.
        bool bMakeHot = true;
        {
          foreach (var cur_gripData in GripDataDict)
          {
            if (!bMakeHot)
              break;

            OdExDrawingGripDataCollection aData = cur_gripData.Value.DataArray;
            int i, iSize = aData.Count;
            for (i = 0; i < iSize; i++)
            {
              if (DrawType.kHotGrip == aData[i].Status)
              {
                bMakeHot = false;
                break;
              }
            }
            for (i = 0; (i < cur_gripData.Value.DataSub.Count) && bMakeHot; i++)
            {
              OdExDrawingGripDataCollection aData_ = cur_gripData.Value.DataSub[i].SubData;
              int j, jSize = aData.Count;
              for (j = 0; j < iSize; j++)
              {
                if (DrawType.kHotGrip == aData[j].Status)
                {
                  bMakeHot = false;
                  break;
                }
              }
            }
          }
        }

        bool bGetNew = false;
        OdDbObjectId idEntityToUpdate = null;
        if (bMakeHot)
        {
          int i, iSize = aKeys.Count;
          for (i = 0; i < iSize; i++)
          {
            OdExDrawingGripData pGrip = aKeys[i];

            DrawType eNew = DrawType.kHotGrip;

            if (pGrip.Data != null)
            {
              Globals.GripOperationPtrDelegate hotGripFuncDelegate = pGrip.Data.hotGripFunc();
              if (hotGripFuncDelegate != null)
              {
                int iFlags = 0;
                if (pGrip.Shared)
                  iFlags |= (int)GripContextFlags.kSharedGrip;

                if (pGrip.Data.triggerGrip())
                {
                  if (!pGrip.Shared)
                  {
                    OdResult eRet = hotGripFuncDelegate( pGrip.Data,pGrip.entityId.AsOdDbStubPointer(), iFlags);

                    switch (eRet)
                    {
                      case OdResult.eOk:
                      case OdResult.eGripOpGripHotToWarm:
                        eNew = DrawType.kWarmGrip;
                        break;
                      case OdResult.eGripOpGetNewGripPoints:
                        bGetNew = true;
                        idEntityToUpdate = pGrip.entityId;
                        break;
                      default:
                        // no op
                        break;
                    }
                  }
                }
                else // of if (pGrip->data()->triggerGrip())
                {
                  OdResult eRet = hotGripFuncDelegate( pGrip.Data,pGrip.entityId.AsOdDbStubPointer(), iFlags);

                  if (!pGrip.Shared)
                  {
                    switch (eRet)
                    {
                      case OdResult.eGripOpGripHotToWarm:
                        eNew = DrawType.kWarmGrip;
                        break;
                      case OdResult.eGripOpGetNewGripPoints:
                        bGetNew = true;
                        idEntityToUpdate = pGrip.entityId;
                        break;
                      default:
                        // no op
                        break;
                    }
                  }
                } // else // of if (pGrip->data()->triggerGrip())
                pGrip.Status = eNew;
              }
            }
          }

          if (bGetNew)
          {
            updateEntityGrips(idEntityToUpdate);

            OdDbEntity ent = OdDbEntity.cast(idEntityToUpdate.openObject(OpenMode.kForRead));
            ent.downgradeOpen();
            return true;
          }
          //////////////////////////////////////////////////////////
        } // of else of if (bShift)
      }

      return false;
    }

    private bool DragGripProcessing(int x, int y, bool bShift)
    {
      OdExDrawingGripDataCollection aKeys = new OdExDrawingGripDataCollection();
      locateGripsAt(x, y, ref aKeys);

      if (aKeys.Count > 0)
      {
        //if ( bShift )  TODO
        {
          bool bMakeHot = true;
          foreach (KeyValuePair<OdDbObjectId, OdExGripDataExt> grData in GripDataDict)
          {
            foreach (OdExDrawingGripData exgrData in grData.Value.DataArray)
            {
              if (DrawType.kHotGrip == exgrData.Status)
              {
                bMakeHot = false;
                break;
              }
            }

            foreach (OdExGripDataSubent grDatSub in grData.Value.DataSub)
            {
              foreach (OdExDrawingGripData exgrData in grDatSub.SubData)
              {
                if (DrawType.kHotGrip == exgrData.Status)
                {
                  bMakeHot = false;
                  break;
                }
              }
              if (bMakeHot == false)
                break;
            }

            if (bMakeHot == false)
              break;
          }
          if (bMakeHot)
          {
            foreach (OdExDrawingGripData exgrData in aKeys)
            {
              exgrData.Status = DrawType.kHotGrip;
            }
          }

          aActiveKeys = new OdExDrawingGripDataCollection();
          locateGripsByStatus(DrawType.kHotGrip, ref aActiveKeys);
          if (aActiveKeys.Count == 0)
          {
            // Valid situation.
            // If trigger grip performed entity modification and returned eGripHotToWarm
            // then nothing is to be done cause entity modification will cause reactor to regen grips.
            return false;
          }

          foreach (OdExDrawingGripData exgrData in aActiveKeys)
          {
            exgrData.Status = DrawType.kDragImageGrip;
          }

          foreach (KeyValuePair<OdDbObjectId, OdExGripDataExt> grData in GripDataDict)
          {
            bool bActive = false;
            foreach (OdExDrawingGripData exgrData in grData.Value.DataArray)
            {
              if (DrawType.kDragImageGrip == exgrData.Status)
              {
                bActive = true;
                m_aDrags.Add(new OdExDrawingGripDrag(grData.Key, this));
              }
            }
            foreach (OdExGripDataSubent grDatSub in grData.Value.DataSub)
            {
              foreach (OdExDrawingGripData exgrData in grDatSub.SubData)
              {
                if (DrawType.kDragImageGrip == exgrData.Status)
                {
                  bActive = true;
                  m_aDrags.Add(new OdExDrawingGripDrag(exgrData.SubentPath, this));
                }
              }
              if (bActive == true)
              {
                break;
              }
            }
          }

          var pView = m_pDevice.activeView();
          foreach (OdExDrawingGripDrag grDrag in m_aDrags)
          {
            grDrag.notifyDragStarted();
            grDrag.cloneEntity();
            pView.add(grDrag, null);
          }

          BasePoint = aKeys[0].Point;
          LastPoint = BasePoint;
        }
      }
      else
      {
        return false;
      }
      return true;
    }

    public bool OnMouseDown(int x,
      int y,
      bool bShift,
      out bool isTriggerGripProcessing,
      out bool isDragGripProcessing)
    {
      endHover();

      bool bOperationSuccess = false;
      isDragGripProcessing = false;
      isTriggerGripProcessing = false;

      isTriggerGripProcessing = TriggerGripProcessing(x, y, bShift);
      if (isTriggerGripProcessing)
      {
        bOperationSuccess = true;
        return bOperationSuccess;
      }

      isDragGripProcessing = DragGripProcessing(x, y, bShift);
      if (isDragGripProcessing)
      {
        bOperationSuccess = true;
        return bOperationSuccess;
      }

      return bOperationSuccess;
    }

    public void ResetSelection()
    {
      if (GripDataDict == null)
        return;

      var overall = GripDataDict.Keys.ToArray();
      foreach (OdDbObjectId id in overall)
        removeEntityGrips(id, true);
    }

    public void UpdateSelection(OdDbObjectIdArray selected)
    {
      foreach (OdDbObjectId id in selected)
      {
        if (!GripDataDict.ContainsKey(id))
        {
          updateEntityGrips(id);
        }
      }

      updateInvisibleGrips();
    }

    public bool MoveAllSelected(int x, int y)
    {
      endHover();

      OdExDrawingGripDataCollection aKeys = new OdExDrawingGripDataCollection();
      locateGripsAt(x, y, ref aKeys, false);
      if (aKeys.Count > 0)
      {
        aActiveKeys = new OdExDrawingGripDataCollection();
        locateGripsByStatus(DrawType.kWarmGrip, ref aActiveKeys);
        if (aActiveKeys.Count == 0)
        {
          return false;
        }
        foreach (OdExDrawingGripData exgrData in aActiveKeys)
        {
          exgrData.Status = DrawType.kDragImageGrip;
        }


        foreach (KeyValuePair<OdDbObjectId, OdExGripDataExt> grData in GripDataDict)
        {
          bool bActive = false;
          foreach (OdExDrawingGripData exgrData in grData.Value.DataArray)
          {
            bActive = true;
            m_aDrags.Add(new OdExDrawingGripDrag(grData.Key, this));
          }
          foreach (OdExGripDataSubent grDatSub in grData.Value.DataSub)
          {
            foreach (OdExDrawingGripData exgrData in grDatSub.SubData)
            {
              bActive = true;
              m_aDrags.Add(new OdExDrawingGripDrag(exgrData.SubentPath, this));
            }
            if (bActive)
              break;
          }
        }

        var pView = m_pDevice.activeView();
        foreach (OdExDrawingGripDrag grDrag in m_aDrags)
        {
          grDrag.notifyDragStarted();
          grDrag.cloneEntity();
          pView.add(grDrag, null);
        }

        BasePoint = aKeys[0].Point;
        LastPoint = BasePoint;
      }
      else
      {
        return false;
      }
      return true;
    }

    public void DragFinal(OdGePoint3d ptFinal, bool bOk)
    {
      var pView = m_pDevice.activeView();
      foreach (OdExDrawingGripDrag grDrag in m_aDrags)
      {
        if (bOk)
        {
          grDrag.moveEntity(ptFinal);
          grDrag.notifyDragEnded();
        }
        else
          grDrag.notifyDragAborted();
        pView.erase(grDrag);
        updateEntityGrips(grDrag.entityId());
        grDrag.uninit();
      }

      m_aDrags.Clear();

      if (bOk)
        updateInvisibleGrips();

      if (aActiveKeys != null)
      {
        foreach (OdExDrawingGripData exgrData in aActiveKeys)
        {
          exgrData.Status = DrawType.kWarmGrip;
        }
      }
      aActiveKeys = null;
    }


    public OdGeVector3d activeViewDirection()
    {
      var pView = m_pDevice.activeView();
      OdGeVector3d vec = pView.position() - pView.target();
      return vec.normal();
    }

    public static OdExGripDataSubent getSubentGripData(OdExGripDataExt ext, OdDbFullSubentPath entPath)
    {
      int i = 0;
      foreach (OdExGripDataSubent subData in ext.DataSub)
      {
        if (subData.EntPath == entPath)
          return ext.DataSub[i];
        ++i;
      }
      return ext.DataSub[0];
    }

    public double activeViewUnitSize()
    {
      OdGeVector3d v;
      var pView = m_pDevice.activeView();

      OdGePoint2d ptDim; // getNumPixelsInUnitSquare
      {
        OdGsDCRect scrRect = new OdGsDCRect();
        pView.getViewport(scrRect);
        ptDim = new OdGePoint2d(Math.Abs(((double)scrRect.m_max.x - scrRect.m_min.x) / pView.fieldWidth()),
                            Math.Abs(((double)scrRect.m_max.y - scrRect.m_min.y) / pView.fieldHeight()));
      }
      v = new OdGeVector3d(GRIPSIZE / ptDim.x, 0, 0);
      v = v.transformBy(pView.viewingMatrix());
      return v.length() / GRIPSIZE;
    }

    private long m_iClockStartHover = 0;

    private OdResult startHover(int x, int y, bool bShift)
    {
      OdResult eRet = OdResult.eOk;

      if (!endHover())
      {
        eRet = OdResult.eGripOpFailure;
        m_iClockStartHover = 0;
      }

      OdExDrawingGripDataCollection aKeys = new OdExDrawingGripDataCollection();
      locateGripsAt(x, y, ref aKeys);

      if (0 != aKeys.Count)
      {
        m_hoverGripsColl = aKeys;
        foreach (OdExDrawingGripData gr in m_hoverGripsColl)
        {
          if (DrawType.kWarmGrip == gr.Status)
          {
            gr.Status = DrawType.kHoverGrip;

            if (gr.Data != null)
            {
              Globals.GripOperationPtrDelegate hoverFuncDelegate = gr.Data.hoverFunc();

              if ((hoverFuncDelegate != null) && !bShift)
              {
                if (m_iClockStartHover == 0)
                  m_iClockStartHover = Stopwatch.GetTimestamp();

                // 300 ms delay before hoverFunc() 
                if ((Stopwatch.GetTimestamp() - m_iClockStartHover) * 1000 / Stopwatch.Frequency > 300)
                {
                  int iFlags = 0;
                  if (gr.Shared)
                    iFlags = (int)GripContextFlags.kSharedGrip;

                  eRet = hoverFuncDelegate(gr.Data, gr.entityId.AsOdDbStubPointer(), iFlags);

                  if (eRet == OdResult.eGripOpGetNewGripPoints)
                  {
                    m_iClockStartHover = 0;
                    gr.Status = DrawType.kHotGrip;

                    BasePoint = aKeys[0].Point;
                    LastPoint = BasePoint;
                    {
                      // Use alternative point if needed.
                      OdDbGripData pFirstData = aKeys[0].Data;
                      if (pFirstData != null)
                      {
                        if (pFirstData.alternateBasePoint() != null)
                        {
                          BasePoint = pFirstData.alternateBasePoint();
                        }
                      }
                    }
                  }
                }
              }
            }


            if (Model != null)
              Model.onModified(gr, null as OdGiDrawable);
            else
              m_pDevice.invalidate();
          }
        }
      }

      return eRet;
    }

    private bool endHover()
    {
      if (0 == m_hoverGripsColl.Count)
        return false;
      foreach (OdExDrawingGripData gr in m_hoverGripsColl)
      {
        if (DrawType.kHoverGrip == gr.Status)
        {
          gr.Status = DrawType.kWarmGrip;
          if (Model != null)
            Model.onModified(gr, null as OdGiDrawable);
          else
            m_pDevice.invalidate();
        }
      }
      m_hoverGripsColl.Clear();
      return true;
    }

    private void locateGripsAtInt(int x, int y, ref OdExDrawingGripDataCollection aRes, OdExDrawingGripDataCollection coll, OdGePoint3d ptFirst, OdGeTol tol, bool bFfindGripPoint)
    {
      foreach (OdExDrawingGripData exgrData in coll)
      {
        if (bFfindGripPoint)
        {
          OdGePoint3d ptCurrent = new OdGePoint3d(exgrData.Point);
          if (aRes.Count == 0)
          {
            // First grip is obtained by comparing
            // grip point device position with cursor position.
            var pView = m_pDevice.activeView();
            OdGePoint3d ptDC = ptCurrent.transformBy(pView.worldToDeviceMatrix());

            double dDeltaX = Math.Abs(x - ptDC.x);
            double dDeltaY = Math.Abs(y - ptDC.y);
            bool bOk = (dDeltaX <= GRIPSIZE) && (dDeltaY <= GRIPSIZE);
            if (bOk)
            {
              ptFirst = ptCurrent;
              aRes.Add(exgrData);
            }
          }
          else
          {
            if (ptCurrent.isEqualTo(ptFirst, tol))
            {
              aRes.Add(exgrData);
            }
          }
        }
        else
        {
          aRes.Add(exgrData);
        }
      }
    }

    private void locateGripsAt(int x, int y, ref OdExDrawingGripDataCollection aRes)
    {
      locateGripsAt(x, y, ref aRes, true);
    }

    private void locateGripsAt(int x, int y, ref OdExDrawingGripDataCollection aRes, bool bFindGripPoint)
    {
      OdGePoint3d ptFirst = new OdGePoint3d();
      OdGeTol tol = new OdGeTol(1E-4, 1E-4);
      foreach (KeyValuePair<OdDbObjectId, OdExGripDataExt> grData in GripDataDict)
      {
        locateGripsAtInt(x, y, ref aRes, grData.Value.DataArray, ptFirst, tol, bFindGripPoint);
        foreach (OdExGripDataSubent exgrData in grData.Value.DataSub)
        {
          locateGripsAtInt(x, y, ref aRes, exgrData.SubData, ptFirst, tol, bFindGripPoint);
        }
      }
    }

    private void removeEntityGrips(OdDbEntity ent, OdDbObjectId id, bool bFireDone)
    {
      OdExGripDataExt grDataExt;
      if (GripDataDict.TryGetValue(id, out grDataExt))
      {
        ent.gripStatus(GripStat.kGripsToBeDeleted);
        foreach (OdExDrawingGripData gr in grDataExt.DataArray)
        {
          hideGrip(gr);
          if (gr.Data != null)
          {
            Globals.GripOpStatusPtrDelegate gripOpStatFuncHandler = gr.Data.gripOpStatFunc();
            if (gripOpStatFuncHandler != null)
            {
              gripOpStatFuncHandler(gr.Data,id.AsOdDbStubPointer(), GripStatus.kGripEnd);
            }
          }
        }
        grDataExt.DataArray = null;
        int sz = grDataExt.DataSub.Count;
        for (int iSz = 0; iSz < sz; iSz++)
        {
          foreach (OdExDrawingGripData grData in grDataExt.DataSub[iSz].SubData)
          {
            hideGrip(grData);
          }
        }
        GripDataDict.Remove(id);
      }
      ent.highlight(false);

      if (bFireDone)
      {
        ent.gripStatus(GripStat.kGripsDone);
      }

      grDataExt = null;
    }

    private void removeEntityGrips(OdDbObjectId id, bool bFireDone)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDbEntity ent = OdDbEntity.cast(id.openObject(OpenMode.kForRead));
      removeEntityGrips(ent, id, bFireDone);
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }

    private void updateEntityGrips(OdDbObjectId id)
    {
      if (id == null)
        return;

      OdDbEntity ent = OdDbEntity.cast(id.openObject(OpenMode.kForRead));
      try
      {
        try // grip points are implemented not for all entities
        {
          removeEntityGrips(ent, id, false);
          OdExDrawingGripDataCollection exGrDataColl = new OdExDrawingGripDataCollection();
          OdDbGripDataPtrArray grips = new OdDbGripDataPtrArray();
          if (ent.getGripPoints(grips, activeViewUnitSize(), GRIPSIZE, activeViewDirection(), (int)GetGripPointsFlags.kGripPointsOnly) == OdResult.eOk)
          {
            foreach (OdDbGripData gr in grips)
            {
              exGrDataColl.Add(new OdExDrawingGripData(id, gr, gr.gripPoint(), this));
            }
          }
          else
          {
            OdGePoint3dArray gripsPts = new OdGePoint3dArray();
            ent.getGripPoints(gripsPts);
            foreach (OdGePoint3d pt in gripsPts)
            {
              exGrDataColl.Add(new OdExDrawingGripData(id, pt, this));
            }
          }

          if (null != exGrDataColl)
          {
            OdExGripDataExt dExt = new OdExGripDataExt();
            foreach (OdExDrawingGripData grDat in exGrDataColl)
            {
              OdDbFullSubentPath entPath;
              if (grDat.entPath(out entPath))
              {
                bool bFound = false;
                OdExGripDataSubentCollection grSubColl = dExt.DataSub;
                for (int j = 0; j < grSubColl.Count; j++)
                {
                  if (grSubColl[j].EntPath == entPath)
                  {
                    bFound = true;
                    grSubColl[j].SubData.Add(grDat);
                    break;
                  }
                }
                if (!bFound)
                {
                  OdExGripDataSubent se = new OdExGripDataSubent();
                  se.EntPath = entPath;
                  se.SubData.Add(grDat);
                  dExt.DataSub.Add(se);
                }
              }
              else
              {
                if (dExt.DataArray == null)
                  dExt.DataArray = new OdExDrawingGripDataCollection();
                dExt.DataArray.Add(grDat);
              }
            }

            GripDataDict.Add(id, dExt);

            foreach (OdExDrawingGripData grData in exGrDataColl)
            {
              showGrip(grData);
            }
          }
        }
        catch (System.Exception err)
        {
          // just skip non-supported entities
        }
        ent.highlight(true);
      }
      catch (System.Exception)
      {
        ent.highlight(true);
      }
    }

    private void showGrip(OdExDrawingGripData pGrip)
    {
      int nViews = m_pDevice.numViews();
      for (int i = 0; i < nViews; i++)
        m_pDevice.viewAt(i).add(pGrip, null);
    }

    private void hideGrip(OdExDrawingGripData pGrip)
    {
      int nViews = m_pDevice.numViews();
      for (int i = 0; i < nViews; i++)
        m_pDevice.viewAt(i).erase(pGrip);
    }

    private void locateGripsByStatus(DrawType eStatus, ref OdExDrawingGripDataCollection aResult)
    {
      foreach (KeyValuePair<OdDbObjectId, OdExGripDataExt> grData in GripDataDict)
      {
        foreach (OdExDrawingGripData exgrDat in grData.Value.DataArray)
        {
          if (exgrDat.Status == eStatus)
          {
            aResult.Add(exgrDat);
          }
        }

        foreach (OdExGripDataSubent exgrData in grData.Value.DataSub)
        {
          foreach (OdExDrawingGripData exgrDat in exgrData.SubData)
          {
            if (exgrDat.Status == eStatus)
            {
              aResult.Add(exgrDat);
            }
          }
        }
      }
    }

    private void updateInvisibleGrips()
    {
      OdExDrawingGripDataCollection aOverall = new OdExDrawingGripDataCollection();
      foreach (KeyValuePair<OdDbObjectId, OdExGripDataExt> grDataDc in GripDataDict)
      {
        foreach (OdExDrawingGripData grData in grDataDc.Value.DataArray)
        {
          aOverall.Add(grData);
        }
        foreach (OdExGripDataSubent grDataSub in grDataDc.Value.DataSub)
        {
          foreach (OdExDrawingGripData grData in grDataSub.SubData)
          {
            aOverall.Add(grData);
          }
        }
      }

      int iSize = aOverall.Count;
      for (int i = 0; i < iSize; i++)
      {
        OdExDrawingGripData grData = aOverall[i];
        grData.Invisible = false;
        grData.Shared = false;

        OdIntArray aEq = new OdIntArray();
        aEq.Add(i);

        OdGePoint3d ptIni = grData.Point;

        int iNext = i + 1;
        OdGeTol tc = new OdGeTol(1E-6, 1E-6);
        while (iNext < iSize)
        {
          OdGePoint3d ptCur = aOverall[iNext].Point;
          if (ptIni.isEqualTo(ptCur, tc))
          {
            aEq.Add(iNext);
            iNext++;
          }
          else
            break;
        }

        if (aEq.Count >= 2)
        {
          int iVisible = 0;
          int jSize = aEq.Count;
          for (int j = 0; j < jSize; j++)
          {
            OdExDrawingGripData pGrip = aOverall[aEq[j]];

            bool bOk = true;
            if (pGrip.Data != null)
            {
              if (pGrip.Data.skipWhenShared())
                bOk = false;
            }

            if (bOk)
            {
              iVisible = j;
              break;
            }
          }

          for (int j = 0; j < jSize; j++)
          {
            OdExDrawingGripData pGrip = aOverall[aEq[j]];
            pGrip.Shared = true;
            pGrip.Invisible = (j != iVisible);
          }
        }
      }
    }

    public OdGePoint3d BasePoint { get; private set; }

    public OdGePoint3d LastPoint { get; private set; }

    public int GRIPSIZE { get; private set; }

    public int GRIPOBJLIMIT { get; private set; }

    public OdCmEntityColor GRIPCOLOR { get; private set; }

    public OdCmEntityColor GRIPHOVER { get; private set; }

    public OdCmEntityColor GRIPHOT { get; private set; }

    public OdGsModel Model { get; private set; }

    public OdGsDevice Device
    {
      get { return m_pDevice; }
    }

    public Dictionary<OdDbObjectId, OdExGripDataExt> GripDataDict { get; private set; }

  }
}