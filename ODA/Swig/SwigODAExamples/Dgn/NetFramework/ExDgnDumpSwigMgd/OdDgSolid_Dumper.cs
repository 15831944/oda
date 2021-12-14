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
  class OdDgSolid_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgSolid element = (OdDgSolid)pObject;
      startDescription(element);
      writeFieldValue("Type of solid", element.getType());

      //extract the structure
      OdDg3dObjectHelper helper = new OdDg3dObjectHelper(element);
      EntireObject structure = new EntireObject();
      helper.extractInformation(structure);

      Int32 currentBoundaryIndex = 0, boundariesAtAll = structure.Count;
      writeFieldValue("Number of boundaries", boundariesAtAll);

      m_nesting++;

      //iterate through boundaries
      for (; currentBoundaryIndex < boundariesAtAll; currentBoundaryIndex++)
      {
        OdDgGraphicsElementPtrArray boundaries = structure[currentBoundaryIndex].m_boundary, rules = structure[currentBoundaryIndex].m_rules;

        Int32 currentIndex, topIndex;

        writeShift();
        Program.DumpStream.WriteLine(string.Format("Boundary # {0}\n", currentBoundaryIndex));
        m_nesting++;
        topIndex = boundaries.Count;
        for (currentIndex = 0; currentIndex < topIndex; currentIndex++)
        {
          describeSubElement(boundaries[currentIndex]);
        }
        m_nesting--;

        writeShift();
        Program.DumpStream.WriteLine("Rules\n");
        m_nesting++;
        topIndex = rules.Count;
        for (currentIndex = 0; currentIndex < topIndex; currentIndex++)
        {
          describeSubElement(rules[currentIndex]);
        }
        m_nesting--;
      }

      m_nesting--;
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgSolid";
    }
  }
}