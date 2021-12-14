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
using System.Linq;
using System.Runtime.InteropServices;
using Teigha.Core;
using Teigha.TD;

namespace OdDrawingsExamplesMgdLib.ExGripPoints
{
  public class OdExDrawingGripData : OdGiDrawable
  {
    OdExDrawingGripManager m_pOwner;

    public OdExDrawingGripData()
    {
      Status = DrawType.kWarmGrip;
      Invisible = false;
      Shared = false;
      Point = OdGePoint3d.kOrigin;
      SubentPath = new OdDbFullSubentPath();
      m_pOwner = null;
      Data = null;
    }


    public OdExDrawingGripData(OdDbObjectId id, OdDbGripData pData, OdGePoint3d pt, OdExDrawingGripManager pOwner)
    {
      Status = DrawType.kWarmGrip;
      Invisible = false;
      Shared = false;
      Point = pt;
      Data = pData;
      OdDbObjectIdArray arrId = new OdDbObjectIdArray();
      arrId.Add(id);
      SubentPath = new OdDbFullSubentPath(arrId, new OdDbSubentId());
      m_pOwner = pOwner;
    }

    public OdExDrawingGripData(OdDbObjectId id, OdGePoint3d pt, OdExDrawingGripManager pOwner)
    {
      Status = DrawType.kWarmGrip;
      Invisible = false;
      Shared = false;
      Point = pt;
      OdDbObjectIdArray arrId = new OdDbObjectIdArray();
      arrId.Add(id);
      SubentPath = new OdDbFullSubentPath(arrId, new OdDbSubentId());
      m_pOwner = pOwner;
      Data = null;
    }

    public OdExDrawingGripData(OdDbFullSubentPath entPath, OdDbGripData pData, OdGePoint3d pt, OdExDrawingGripManager pOwner)
    {
      Status = DrawType.kWarmGrip;
      Invisible = false;
      Shared = false;
      Point = pt;
      Data = pData;
      SubentPath = entPath;
      m_pOwner = pOwner;
    }

    #region OdGiDrawable overrides
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

    protected override bool subWorldDraw(OdGiWorldDraw pWorldDraw)
    {
      double dGripSize = m_pOwner.GRIPSIZE;

      if (Data != null)
      {
        Globals.GripWorldDrawPtrDelegate worldDrawHandler = Data.worldDraw();
        if (worldDrawHandler != null)
        {
          OdGePoint3d ptComputed = new OdGePoint3d();
          OdGePoint3d pDrawAtDrag = null;
          if (computeDragPoint(out ptComputed))
            pDrawAtDrag = ptComputed;        

          using (OdGiDrawFlagsHelper _dfh = new OdGiDrawFlagsHelper(pWorldDraw.subEntityTraits(), (uint)OdGiSubEntityTraits.DrawFlags.kDrawNoPlotstyle))
          {
            return worldDrawHandler(Data,
              pWorldDraw,
              entityId.AsOdDbStubPointer(),
              Status,
              pDrawAtDrag,
              dGripSize);
          }
        }
      }

      return false;
    }

    protected override uint subSetAttributes(OdGiDrawableTraits traits)
    {
      if (Invisible == true)
        return (uint)SetAttributesFlags.kDrawableIsInvisible;

      OdGiSubEntityTraits subEntTraits = OdGiSubEntityTraits.cast(traits);
      if (subEntTraits == null)
        return (uint)SetAttributesFlags.kDrawableNone;

      OdGiSubEntityTraits entTraits = OdGiSubEntityTraits.cast(traits);
      switch (Status)
      {
        case DrawType.kWarmGrip:
          entTraits.setTrueColor(m_pOwner.GRIPCOLOR);
          break;
        case DrawType.kHotGrip:
        case DrawType.kDragImageGrip:
          entTraits.setTrueColor(m_pOwner.GRIPHOT);
          break;
        case DrawType.kHoverGrip:
          entTraits.setTrueColor(m_pOwner.GRIPHOVER);
          break;
      }

      entTraits.setMaterial(null/*entityId().database().byBlockMaterialId()*/);
      entTraits.setLineWeight(LineWeight.kLnWt000);
      return (uint)SetAttributesFlags.kDrawableRegenDraw;
    }

    protected override void subViewportDraw(OdGiViewportDraw vd)
    {
      OdGePoint3d ptComputed = null;
      OdGePoint3d pDrawAtDrag = null;
      if (computeDragPoint(out ptComputed))
        pDrawAtDrag = ptComputed;

      bool bDefault = true;
      if (null != Data)
      {
        Globals.GripViewportDrawPtrDelegate pViewportDrawFunc = Data.viewportDraw();
        if (pViewportDrawFunc != null)
        {
          pViewportDrawFunc(Data,
            vd,
            entityId.AsOdDbStubPointer(),
            Status,
            pDrawAtDrag,
            m_pOwner.GRIPSIZE);

          bDefault = false;
        }
      }

      if (bDefault)
      {
        var pViewport = vd.viewport();
        if (m_pOwner.Model == null || m_pOwner.Model.renderType().GetHashCode() < 2/*Teigha.GraphicsSystem.RenderType.kDirect*/)
        {
          // Commented since renderTypes implemented, so no need to translate objects for kDirect renderType
          OdGeVector3d vpDirection = pViewport.viewDir();
          OdGePoint3d vpOrigin = pViewport.getCameraLocation();
          OdGeVector3d vecTmp = ptComputed - vpOrigin;
          double ptLength = vecTmp.dotProduct(vpDirection);
          ptComputed -= vpDirection * ptLength;
        }

        OdGePoint2d ptDim = new OdGePoint2d();
        pViewport.getNumPixelsInUnitSquare(Point, ptDim);
        OdGeVector3d v = new OdGeVector3d(m_pOwner.GRIPSIZE / ptDim.x, 0, 0);
        v = v.transformBy(pViewport.getWorldToEyeTransform());
        double dGripSize = v.length();

        OdGePoint3d ptOnScreen = ptComputed.transformBy(pViewport.getWorldToEyeTransform());
        OdGiSubEntityTraits pSubTraits = vd.subEntityTraits();
        pSubTraits.setFillType(OdGiFillType.kOdGiFillAlways);

        OdGePoint3dArray ptColl = new OdGePoint3dArray();
        ptColl.Add(new OdGePoint3d(ptOnScreen.x - dGripSize, ptOnScreen.y - dGripSize, ptOnScreen.z));
        ptColl.Add(new OdGePoint3d(ptOnScreen.x + dGripSize, ptOnScreen.y - dGripSize, ptOnScreen.z));
        ptColl.Add(new OdGePoint3d(ptOnScreen.x + dGripSize, ptOnScreen.y + dGripSize, ptOnScreen.z));
        ptColl.Add(new OdGePoint3d(ptOnScreen.x - dGripSize, ptOnScreen.y + dGripSize, ptOnScreen.z));
        vd.geometry().polygonEye(ptColl.ToArray());
      }
    }

    #endregion

    public bool entPath()
    {
      return (SubentPath.subentId() != null && SubentPath.subentId().type() != Teigha.Core.SubentType.kNullSubentType);
    }

    public bool entPath(out OdDbFullSubentPath pPath)
    {
      pPath = SubentPath;
      return entPath();
    }

    bool computeDragPoint(out OdGePoint3d ptOverride)
    {
      OdGePoint3d ptBase = new OdGePoint3d(Point);
      if (Data != null && Data.alternateBasePoint() != null)
        ptBase = new OdGePoint3d(Data.alternateBasePoint());

      bool bOverride = false;
      ptOverride = ptBase;

      if (Status == DrawType.kDragImageGrip && Data != null && Data.drawAtDragImageGripPoint())
      {
        ptOverride = ptBase + (m_pOwner.LastPoint - m_pOwner.BasePoint);
        bOverride = true;
      }
      return bOverride;
    }

    public DrawType Status { get; set; }
    public bool Invisible { get; set; }
    public bool Shared { get; set; }
    public OdGePoint3d Point { get; private set; }
    public OdDbGripData Data { get; private set; }
    public OdDbObjectId entityId
    {
      get
      {
        OdDbObjectIdArray idArr = SubentPath.objectIds();
        return idArr.Last();
      }
    }
    public OdDbFullSubentPath SubentPath { get; private set; }
  };

}
