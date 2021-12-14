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
  class OdDgMultiline_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgMultiline element = (OdDgMultiline)pObject;
      startDescription(element);
      writeFieldValue("Origin cap angle", element.getOriginCapAngle());
      writeFieldValue("End cap angle", element.getEndCapAngle());
      {
        OdGeVector3d vector;

        vector = element.getZVector();
        writeFieldValue("Z vector", vector);
      }
      writeFieldValue("Is closed", element.getClosedFlag());

      //symbologies
      {
        OdDgMultilineSymbology symbology = new OdDgMultilineSymbology();

        element.getOriginCap(symbology);
        writeFieldValue("Origin cap", symbology);
        element.getMiddleCap(symbology);
        writeFieldValue("Joint cap", symbology);
        element.getEndCap(symbology);
        writeFieldValue("End cap", symbology);
      }

      //points
      {
        UInt32 j = element.getPointsCount();
        OdDgMultilinePoint point = new OdDgMultilinePoint();

        writeShift();
        Program.DumpStream.WriteLine(string.Format("Number of points: {0}\n", j));

        for (int i = 0; i < j; i++)
        {
          element.getPoint(i, point);
          writeFieldValue(string.Format("Point {0}", i), point);
        }
      }

      //profiles
      {
        UInt32 j = element.getProfilesCount();
        OdDgMultilineProfile profile = new OdDgMultilineProfile();

        writeShift();
        Program.DumpStream.WriteLine(string.Format("Number of profiles: %d\n", j));

        for (int i = 0; i < j; i++)
        {
          element.getProfile(i, profile);
          writeFieldValue(string.Format("Profile {0}", i), profile);
        }
      }
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgMultiline";
    }
  }
}