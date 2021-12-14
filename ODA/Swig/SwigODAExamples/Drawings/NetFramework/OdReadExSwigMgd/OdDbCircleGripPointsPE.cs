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
using Teigha.TD;
using System.IO;
using Microsoft.Win32;

namespace OdReadExSwigMgd
{
  /************************************************************************/
  /*      Define custom OdDbGripPointsPE                                  */
  /************************************************************************/
  class OdDbCircleGripPointsPE : OdDbGripPointsPE
  {
    public OdGePoint3d getPlanePoint(OdDbCircle circle, OdGePoint3d Point)
    {
      OdGePlane plane = new OdGePlane();                     // recalculated Point in plane of pCircle
      Planarity planarity = new Planarity();

      circle.getPlane(plane, out planarity);
      plane.intersectWith(new OdGeLine3d(Point, circle.normal()), Point);

      return Point;
    }

    public bool projectOffset(OdDbDatabase pDb, OdGeVector3d vNormal, OdGeVector3d vOffset)
    {
      OdDbObjectId idVp = pDb.activeViewportId();
      OdDbObject pVp = (OdDbObject)idVp.openObject();
      OdAbstractViewPE pAVp = OdAbstractViewPE.cast(pVp);
      if (null != pAVp)
      {
        OdGeVector3d vViewDir = pAVp.direction(pVp);
        if (!vViewDir.isPerpendicularTo(vNormal))
        {
          OdGePlane pln = new OdGePlane(OdGePoint3d.kOrigin, vNormal);
          OdGeMatrix3d mProj = new OdGeMatrix3d();
          mProj.setToProjection(pln, vViewDir);
          vOffset.transformBy(mProj);
          return true;
        }
      }
      return false;
    }

    public override OdResult getGripPoints(OdDbEntity ent, OdGePoint3dArray gripPoints)
    {
      Console.WriteLine("OdDbCircleGripPointsPE getGripPoints call");
      return base.getGripPoints(ent, gripPoints);
    }
    //----------------------------------------------------------------------------------------------------------------------------
    // Move circle or change radius
    public override OdResult moveGripPointsAt(OdDbEntity pEnt, OdIntArray indices, OdGeVector3d vOffset)
    {
      Console.WriteLine("OdDbCircleGripPointsPE moveGripPointsAt call");
      return base.moveGripPointsAt(pEnt, indices, vOffset);
    }
    //----------------------------------------------------------------------------------------------------------------------------
    // Cannot be stretched
    public override OdResult getStretchPoints(OdDbEntity pEnt, OdGePoint3dArray stretchPoints)
    {
      Console.WriteLine("OdDbCircleGripPointsPE getStretchPoints call");
      return base.getStretchPoints(pEnt, stretchPoints);
    }

    public override OdResult moveStretchPointsAt(OdDbEntity pEnt, OdIntArray indices_, OdGeVector3d offset)
    {
      Console.WriteLine("OdDbCircleGripPointsPE moveStretchPointsAt call");
      return base.moveStretchPointsAt(pEnt, indices_, offset);
    }
    //----------------------------------------------------------------------------------------------------------------------------
    // Return snap Points into snapPoints, depending on type sonapMode
    //                 getOsnapPoints(OdDbEntity pEntity, OsnapMode osnapMode, IntPtr /*SIMPLETYPE*/ gsSelectionMark, OdGePoint3d pickPoint, OdGePoint3d lastPoint, OdGeMatrix3d xWorldToEye, OdGePoint3dArray snapPoints) {
    public override OdResult getOsnapPoints(OdDbEntity ent, OsnapMode sonapMode, IntPtr gsSelectionMark, OdGePoint3d pickPoint_, OdGePoint3d lastPoint_, OdGeMatrix3d xWorldToEye, OdGePoint3dArray snapPoints)
    {
      Console.WriteLine("OdDbCircleGripPointsPE getOsnapPoints call");
      return base.getOsnapPoints(ent, sonapMode, gsSelectionMark, pickPoint_, lastPoint_, xWorldToEye, snapPoints);
    }
  }

}