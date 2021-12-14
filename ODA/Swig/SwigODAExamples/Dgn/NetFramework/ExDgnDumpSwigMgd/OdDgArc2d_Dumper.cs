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
using Teigha.Core;
using Teigha.TG;

namespace ExDgnDumpSwigMgd
{
  class OdDgArc2d_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgArc2d element = (OdDgArc2d)pObject;
      startDescription(element);
      writeFieldValue("Primary axis", element.getPrimaryAxis());
      writeFieldValue("Secondary axis", element.getSecondaryAxis());
      writeFieldValue("Rotation", element.getRotationAngle());
      {
        OdGePoint2d point;

        point = element.getOrigin();
        writeFieldValue("Origin", point);
      }
      writeFieldValue("Start angle", element.getStartAngle());
      writeFieldValue("Sweep angle", element.getSweepAngle());

      OdGeEllipArc2d pArc = element.getEllipArc();
      writeFieldValue("isClockWise", pArc.isClockWise());
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgArc2d";
    }
  }
}