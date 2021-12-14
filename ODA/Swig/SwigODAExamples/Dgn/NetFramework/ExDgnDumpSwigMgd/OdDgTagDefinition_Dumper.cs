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
  class OdDgTagDefinition_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgTagDefinition element = (OdDgTagDefinition)pObject;
      startDescription(element);
      writeFieldValue("Type", element.getType());
      writeFieldValue("ID", element.getEntryId());
      writeFieldValue("Name", element.getName());
      writeFieldValue("Prompt", element.getPrompt());
      switch (element.getType())
      {
        case OdDgTagDefinition.Type.kChar:
          writeFieldValue("Default char value", element.getString());
          break;
        case OdDgTagDefinition.Type.kInt16:
          writeFieldValue("Default int16 value", element.getInt16());
          break;
        case OdDgTagDefinition.Type.kInt32:
          writeFieldValue("Default int32 value", element.getInt32());
          break;
        case OdDgTagDefinition.Type.kDouble:
          writeFieldValue("Default double value", element.getDouble());
          break;
        case OdDgTagDefinition.Type.kBinary:
          writeFieldValue("Default binary data (size)", element.getBinarySize());
          break;
        default:
          //ODA_FAIL();
          break;
      }
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgTagDefinition";
    }
  }
}