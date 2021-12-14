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
  class OdDgRasterAttachmentHeader_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgRasterAttachmentHeader element = (OdDgRasterAttachmentHeader)pObject;
      startDescription(element);
      {
        OdGeExtents3d extents = new OdGeExtents3d();
        element.getGeomExtents(extents);
        writeFieldValue("The result of getGeomExtents()", extents);
      }
      {
        OdGePoint3d origin = new OdGePoint3d();
        OdGeVector3d u = new OdGeVector3d();
        OdGeVector3d v = new OdGeVector3d();    //these values are logged later with OdDgRasterAttachmentComponentGeo object

        element.getOrientation(origin, u, v);
        writeFieldValue("Origin", origin);
      }
      writeFieldValue("Extent", element.getExtent());
      writeFieldValue("Display gamma", element.getDisplayGamma());
      writeFieldValue("Plot gamma", element.getPlotGamma());
      writeFieldValue("Apply rotation", element.getApplyRotationFlag());
      writeFieldValue("Clipping", element.getClippingFlag());
      writeFieldValue("Plot", element.getPlotFlag());
      writeFieldValue("Invert", element.getInvertFlag());
      {
        for (int i = 1; i <= 8; i++)
        {
          writeFieldValue(string.Format("View {0}", i), element.getViewFlag(i));
        }
      }
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgRasterAttachmentHeader";
    }
  }
}