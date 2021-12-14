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
using System.Text;
using System.Runtime.Remoting;
using System.Reflection;
using Teigha.Core;
using Teigha.TG;

namespace ExDgnDumpSwigMgd
{
  class OdDgLineGripPointsPE : OdDgGripPointsPE
  {
    public override OdResult getGripPoints(OdDgElement ent, OdGePoint3dArray gripPoints)
    {
      Console.WriteLine("OdDgLineGripPointsPE getGripPoints call");
      return base.getGripPoints(ent, gripPoints);
    }
    public override OdResult moveGripPointsAt(OdDgElement ent, OdIntArray indices, OdGeVector3d offset)
    {
      Console.WriteLine("OdDgLineGripPointsPE moveGripPointsAt call");
      return base.moveGripPointsAt(ent, indices, offset);
    }
    public override OdResult getStretchPoints(OdDgElement ent, OdGePoint3dArray stretchPoints)
    {
      Console.WriteLine("OdDgLineGripPointsPE getStretchPoints call");
      return base.getStretchPoints(ent, stretchPoints);
    }
    public override OdResult moveStretchPointsAt(OdDgElement ent, OdIntArray indices, OdGeVector3d offset)
    {
      Console.WriteLine("OdDgLineGripPointsPE moveStretchPointsAt call");
      return base.moveStretchPointsAt(ent, indices, offset);
    }
    public override OdResult getOsnapPoints(OdDgElement ent, OdDgElement.OsnapMode osnapMode, IntPtr gsSelectionMark, OdGePoint3d pickPoint, OdGePoint3d lastPoint, OdGeMatrix3d viewXform, OdGePoint3dArray snapPoints)
    {
      Console.WriteLine("OdDgLineGripPointsPE getOsnapPoints call");
      return base.getOsnapPoints(ent, osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints);
    }
  };
}