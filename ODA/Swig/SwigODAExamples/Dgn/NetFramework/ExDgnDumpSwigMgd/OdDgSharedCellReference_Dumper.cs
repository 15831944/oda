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
  class OdDgSharedCellReference_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgSharedCellReference element = (OdDgSharedCellReference)pObject;
      startDescription(element);
      writeFieldValue("Name of definition", element.getDefinitionName());

      writeFieldValue("Transformation", element.getTransformation());
      writeFieldValue("Origin", element.getOrigin());

      writeFieldValueHex("Class map", element.getClassMap());

      writeFieldValue("Override level", element.getLevelOverrideFlag());
      writeFieldValue("Override relative", element.getRelativeOverrideFlag());
      writeFieldValue("Override class", element.getClassOverrideFlag());
      writeFieldValue("Override color", element.getColorOverrideFlag());
      writeFieldValue("Override weight", element.getWeightOverrideFlag());
      writeFieldValue("Override style", element.getStyleOverrideFlag());
      writeFieldValue("Override associative point", element.getAssociativePointOverrideFlag());

      writeFieldValue("ColorIndex", element.getColorIndex());
      writeFieldValue("LineWeight", element.getLineWeight());
      writeFieldValue("LevelEntryId", element.getLevelEntryId());
      UInt32 level = element.getLevelEntryId();
      OdDgElementId idLevel = element.database().getLevelTable().getAt(level);
      if (idLevel.isNull())
      {
        OdDgLevelTableRecord pLevel = OdDgLevelTableRecord.cast(idLevel.safeOpenObject());
        writeFieldValue("Level Name ", pLevel.getName());
      }
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgSharedCellReference";
    }
  }
}