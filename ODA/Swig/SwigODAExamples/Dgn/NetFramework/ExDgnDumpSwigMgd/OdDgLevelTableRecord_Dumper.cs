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
  class OdDgLevelTableRecord_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgLevelTableRecord pLevel = (OdDgLevelTableRecord)pObject;
      startDescription(pLevel);
      writeFieldValue("Level Name ", pLevel.getName());
      writeFieldValue("Level Number", pLevel.getNumber());

      writeFieldValue("Entry ID", pLevel.getEntryId());
      writeFieldValue("Code", pLevel.getNumber());

      writeFieldValue("Element Color (byLevel)", pLevel.getElementColorIndex());
      writeFieldValue("Element LineStyle (byLevel)", pLevel.getElementLineStyleEntryId());
      writeFieldValue("Element LineWeight (byLevel)", pLevel.getElementLineWeight());

      writeFieldValue("Override Color", pLevel.getOverrideColorIndex());
      writeFieldValue("Override LineStyle", pLevel.getOverrideLineStyleEntryId());
      writeFieldValue("Override LineWeight", pLevel.getOverrideLineWeight());

      writeFieldValue("Use override color", pLevel.getUseOverrideColorFlag());
      writeFieldValue("Use override line style", pLevel.getUseOverrideLineStyleFlag());
      writeFieldValue("Use override line weight", pLevel.getUseOverrideLineWeightFlag());

      writeFieldValue("Displayed", pLevel.getIsDisplayedFlag());
      writeFieldValue("Can be Plotted", pLevel.getIsPlotFlag());
      writeFieldValue("Derived from a library level ", pLevel.getIsExternalFlag());
      writeFieldValue("Can be snapped ", pLevel.getIsSnapFlag());
      writeFieldValue("ReadOnly", pLevel.getIsReadOnlyFlag());
      writeFieldValue("Hidden", pLevel.getIsHiddenFlag());
      writeFieldValue("Frozen", pLevel.getIsFrozenFlag());
      writeFieldValue("CustomStyleFromMaster", pLevel.getIsCustomStyleFromMasterFlag());
      writeFieldValue("Displayed", pLevel.getIsDisplayedFlag());

      //writeFieldValueHex( "Element access flags", pLevel.getElementAccessFlags() );
      OdDgLevelTableRecord.ElementAccess elementAccess = pLevel.getElementAccess();
      writeShift();
      Program.DumpStream.WriteLine("Element access");
      switch (elementAccess)
      {
        case OdDgLevelTableRecord.ElementAccess.kAccessAll: Program.DumpStream.WriteLine("All\n"); break;
        case OdDgLevelTableRecord.ElementAccess.kAccessLocked: Program.DumpStream.WriteLine("Locked\n"); break;
        case OdDgLevelTableRecord.ElementAccess.kAccessReadOnly: Program.DumpStream.WriteLine("ReadOnly\n"); break;
        case OdDgLevelTableRecord.ElementAccess.kAccessViewOnly: Program.DumpStream.WriteLine("ViewOnly\n"); break;
      }

      writeFieldValue("Description", pLevel.getDescription());
      writeFieldValue("Priority", pLevel.getPriority());
      writeFieldValue("Transparency", pLevel.getTransparency());
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgLevelTableRecord";
    }
  }
}