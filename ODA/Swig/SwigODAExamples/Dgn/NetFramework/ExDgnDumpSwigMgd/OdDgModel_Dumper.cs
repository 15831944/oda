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
  class OdDgModel_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgModel element = (OdDgModel)pObject;
      startDescription(element);
      //writeFieldValue("OdDgModel", "OdDgModel");

      //storage unit
      {
        OdDgModel.StorageUnitDescription description = new OdDgModel.StorageUnitDescription();
        element.getStorageUnit(description);
        writeShift();
        Program.DumpStream.WriteLine("Storage unit:\n");
        m_nesting++;

        writeFieldValue("Base", description.m_base);
        writeFieldValue("System", description.m_system);
        writeFieldValue("Numerator", description.m_numerator);
        writeFieldValue("Denominator", description.m_denominator);
        writeFieldValue("Uors per Storage", description.m_uorPerStorageUnit);

        m_nesting--;
      }

      //master unit
      {
        OdDgModel.UnitDescription description = new OdDgModel.UnitDescription();
        element.getMasterUnit(description);

        writeShift();
        Program.DumpStream.WriteLine("Master unit:\n");
        m_nesting++;

        writeFieldValue("Base", description.m_base);
        writeFieldValue("System", description.m_system);
        writeFieldValue("Numerator", description.m_numerator);
        writeFieldValue("Denominator", description.m_denominator);
        writeFieldValue("Name", description.m_name);

        m_nesting--;
      }

      //sub unit
      {
        OdDgModel.UnitDescription description = new OdDgModel.UnitDescription();
        element.getSubUnit(description);

        writeShift();
        Program.DumpStream.WriteLine("Sub unit:\n");
        m_nesting++;

        writeFieldValue("Base", description.m_base);
        writeFieldValue("System", description.m_system);
        writeFieldValue("Numerator", description.m_numerator);
        writeFieldValue("Denominator", description.m_denominator);
        writeFieldValue("Name", description.m_name);

        m_nesting--;
      }

      writeFieldValue("Working unit", element.getWorkingUnit());
      writeFieldValue("Global Origin", element.getGlobalOrigin());

      {
        OdGeExtents3d extent = new OdGeExtents3d();

        if (element.getGeomExtents(extent) == eOk)
        {
          writeFieldValue("Extent", extent);
        }
        else
        {
          writeFieldValue("Extent", "invalid value");
        }
      }

      //describes all graphics elements
      writeShift();
      Program.DumpStream.WriteLine("All graphics sub-elements:\n");
      describeSubElements(element.createGraphicsElementsIterator());
      //describes all non-graphics elements
      writeShift();
      Program.DumpStream.WriteLine("All control sub-elements:\n");
      describeSubElements(element.createControlElementsIterator());

      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgModel";
    }
  }
}