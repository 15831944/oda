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
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.ExGripPoints
{
  public class OdExDrawingGripDrag : OdGiDrawable
  {
    private OdDbFullSubentPath subentPath { get; set; }
    private OdDbEntity m_pCloneEnt { get; set; }
    private OdExDrawingGripManager m_pOwner { get; set; }

    public OdExDrawingGripDrag()
    {
      m_pOwner = null;
    }

    public void uninit()
    {
      m_pCloneEnt = null;
      m_pOwner = null;
    }

    public OdExDrawingGripDrag(OdDbObjectId id, OdExDrawingGripManager pOwner)
    {
      OdDbObjectIdArray arrId = new OdDbObjectIdArray();
      arrId.Add(id);

      subentPath = new OdDbFullSubentPath(arrId, new OdDbSubentId());
      m_pOwner = pOwner;
    }

    public OdExDrawingGripDrag(OdDbFullSubentPath entPath, OdExDrawingGripManager pOwner)
    {
      subentPath = entPath;
      m_pOwner = pOwner;
    }

    #region OdGiDrawable Overrides
    public override bool isPersistent()
    {
      return false;
    }

    private OdGsCache m_gsNode;

    public override OdGsCache gsNode()
    {
      return m_gsNode;
    }
    public override void setGsNode(OdGsCache pGsNode)
    {
      m_gsNode = pGsNode;
    }

    public override OdDbStub id()
    {
      return null;
    }

    protected override uint subSetAttributes(OdGiDrawableTraits traits)
    {
      if (null == m_pCloneEnt)
        return (uint)SetAttributesFlags.kDrawableIsInvisible;

      uint iRet = m_pCloneEnt.setAttributes(traits);

      OdGiSubEntityTraits subEntTraits = OdGiSubEntityTraits.cast(traits);
      if (subEntTraits != null)
        subEntTraits.setFillType(OdGiFillType.kOdGiFillNever);

      return iRet;
    }

    protected override void subViewportDraw(OdGiViewportDraw vd)
    {
      if (m_pCloneEnt != null)
        m_pCloneEnt.viewportDraw(vd);
    }

    protected override bool subWorldDraw(OdGiWorldDraw pWorldDraw)
    {
      if (m_pCloneEnt == null)
        return true;
      return m_pCloneEnt.worldDraw(pWorldDraw);
    }

    #endregion

    public OdDbObjectId entityId()
    {
      OdDbObjectIdArray Ids = subentPath.objectIds();
      return Ids.Last();
    }

    public void cloneEntity()
    {
      OdDbEntity entity = OdDbEntity.cast(entityId().openObject(OpenMode.kForRead));
      if (entity == null)
        return;

      if (entity.cloneMeForDragging())
        m_pCloneEnt = OdDbEntity.cast(entity.clone());

      if (m_pCloneEnt != null)
      {
        m_pCloneEnt.disableUndoRecording(true);
        m_pCloneEnt.setPropertiesFrom(entity, false);
      }
    }

    bool locateActiveGrips(out OdIntArray aIndices)
    {
      aIndices = new OdIntArray();
      OdExDrawingGripDataCollection grDataCol = entPath() ? OdExDrawingGripManager.getSubentGripData(m_pOwner.GripDataDict[entityId()], subentPath).SubData :
                                                   m_pOwner.GripDataDict[entityId()].DataArray;

      bool bExMethod = true;
      int i = 0;
      foreach (OdExDrawingGripData grData in grDataCol)
      {
        if (null == grData.Data)
          bExMethod = false;

        if (DrawType.kDragImageGrip == grData.Status)
          aIndices.Add(i);
        i++;
      }
      return bExMethod;
    }

    bool entPath()
    {
      return (subentPath.subentId() != null && subentPath.subentId().type() != Teigha.Core.SubentType.kNullSubentType);
    }

    bool entPath(out OdDbFullSubentPath pPath)
    {
      pPath = subentPath;
      return entPath();
    }

    public void cloneEntity(OdGePoint3d ptMoveAt)
    {
      cloneEntity();
      if (null == m_pCloneEnt)
        return;

      OdIntArray aIndices;
      bool bExMethod = locateActiveGrips(out aIndices);
      OdGeVector3d vOffset = ptMoveAt - m_pOwner.BasePoint;
      if (bExMethod)
      {
        OdDbGripDataPtrArray cloneDataColl = new OdDbGripDataPtrArray();
        if (entPath())
        {
          m_pCloneEnt.getGripPointsAtSubentPath(subentPath, cloneDataColl,
            m_pOwner.activeViewUnitSize(), m_pOwner.GRIPSIZE, m_pOwner.activeViewDirection(), (uint)GetGripPointsFlags.kGripPointsOnly);
        }
        else
        {
          m_pCloneEnt.getGripPoints(cloneDataColl, m_pOwner.activeViewUnitSize(),
            m_pOwner.GRIPSIZE, m_pOwner.activeViewDirection(), (int)GetGripPointsFlags.kGripPointsOnly);
        }

        OdDbVoidPtrArray aIds = new OdDbVoidPtrArray(aIndices.Count);
        foreach (int i in aIndices)
        {
          if (i < cloneDataColl.Count)
            aIds.Add(cloneDataColl[i].appData());
          else
            Debug.Assert(false);
        }

        foreach (OdDbGripData aCloneData in cloneDataColl)
        {
          Globals.GripOpStatusPtrDelegate gripOpStatusHandler = aCloneData.gripOpStatFunc();
          if (gripOpStatusHandler == null)
            continue;
          gripOpStatusHandler(aCloneData, OdDbObjectId.kNull.AsOdDbStubPointer(), GripStatus.kGripStart);
        }


        if (entPath())
        {
          OdDbFullSubentPathArray aPaths = new OdDbFullSubentPathArray(1);
          aPaths[0] = subentPath;
          m_pCloneEnt.moveGripPointsAtSubentPaths(aPaths, aIds, vOffset, 0);
          m_pCloneEnt.subentGripStatus(GripStat.kGripsToBeDeleted, subentPath);
        }
        else
        {
          m_pCloneEnt.moveGripPointsAt(aIds, vOffset, (int)MoveGripPointsFlags.kOsnapped);
          m_pCloneEnt.gripStatus(GripStat.kGripsToBeDeleted);
        }

        foreach (OdDbGripData aCloneData in cloneDataColl)
        {
          Globals.GripOpStatusPtrDelegate gripOpStatusHandler = aCloneData.gripOpStatFunc();
          if (gripOpStatusHandler == null)
            continue;
          gripOpStatusHandler(aCloneData, null, GripStatus.kGripEnd);
        }

      }
      else
      {
        m_pCloneEnt.moveGripPointsAt(aIndices, vOffset);
        m_pCloneEnt.gripStatus(GripStat.kGripsToBeDeleted);
      }

      m_pCloneEnt.downgradeOpen();
    }

    public void moveEntity(OdGePoint3d ptMoveAt)
    {
      OdIntArray aIndices;
      bool bExMethod = locateActiveGrips(out aIndices);
      OdGeVector3d vOffset = ptMoveAt - m_pOwner.BasePoint;

      OdDbEntity pEntity = OdDbEntity.cast(entityId().openObject(OpenMode.kForWrite));

      if (pEntity != null)
      {
        OdExDrawingGripDataCollection grDataCol = entPath() ?
          OdExDrawingGripManager.getSubentGripData(m_pOwner.GripDataDict[entityId()], subentPath).SubData :
                                                     m_pOwner.GripDataDict[entityId()].DataArray;

        if (bExMethod)
        {
          OdDbVoidPtrArray aIds = new OdDbVoidPtrArray(aIndices.Count);
          foreach (int i in aIndices)
          {
            if (i < grDataCol.Count)
              aIds.Add(grDataCol[i].Data.appData());
            else
              Debug.Assert(false);
          }

          if (entPath())
          {
            OdDbFullSubentPathArray aPaths = new OdDbFullSubentPathArray();
            aPaths.Add(subentPath);
            pEntity.moveGripPointsAtSubentPaths(aPaths, aIds, vOffset, 0);
          }
          else
          {
            pEntity.moveGripPointsAt(aIds, vOffset, 0/*(int)MoveGripPointsFlags.kOsnapped*/);
          }
        }
        else
        {
          pEntity.moveGripPointsAt(aIndices, vOffset);
        }
      }

      pEntity.downgradeOpen();
    }

    public void notifyDragStarted()
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDbEntity ent = OdDbEntity.cast(entityId().openObject(OpenMode.kForRead));
      if (ent != null)
        ent.dragStatus(DragStat.kDragStart);
      resetDragging(true);
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }

    public void resetDragging(bool bOp)
    {
      if (m_pOwner == null)
        return;

      OdRxDictionary props = m_pOwner.Device.properties();
      if (props == null)
        return;
      if (!props.has("DrawDragging"))
        return;
      props.putAt("DrawDragging", new OdRxVariantValue(bOp));
    }

    public void notifyDragEnded()
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDbEntity ent = OdDbEntity.cast(entityId().openObject(OpenMode.kForRead));
      if (ent != null)
        ent.dragStatus(DragStat.kDragEnd);
      resetDragging(false);
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }

    public void notifyDragAborted()
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDbEntity ent = OdDbEntity.cast(entityId().openObject(OpenMode.kForRead));
      if (ent != null)
        ent.dragStatus(DragStat.kDragAbort);
      resetDragging(false);
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
  };
}