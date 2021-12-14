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
  class OdDgTagDefinitionSet_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgTagDefinitionSet pTagDefinitionsSet = (OdDgTagDefinitionSet)pObject;
      startDescription(pTagDefinitionsSet);
      writeFieldValue("Name", pTagDefinitionsSet.getName());
      {
        UInt32 i, j = pTagDefinitionsSet.getCount();
        OdRxObject CurObject = null;
        OdRxObject_Dumper dumper = new OdRxObject_Dumper();

        writeFieldValue("Number of definitions", j);
        for (i = 0; i < j; i++)
        {
          CurObject = pTagDefinitionsSet.getByIndex(i);

          //MUshakov, 26/05/2010.
          //At the moment, tag definition sets can wrap some information for SmartSolid objects.
          //In this case, they collect OdDgTagDefinitionDgnStore elements.
          //The latters have no dumpers, so let us avoid it without exceptions 'no dumper'.
          dumper = GetProperType(CurObject);
          if (null != dumper)
          {
            dumper.dump(CurObject);
          }
          else
          {
            m_nesting++;
            Program.DumpStream.WriteLine("Object does not have its dumper\n");
            m_nesting--;
          }
        }
      }
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgTagDefinitionSet";
    }
  }
}