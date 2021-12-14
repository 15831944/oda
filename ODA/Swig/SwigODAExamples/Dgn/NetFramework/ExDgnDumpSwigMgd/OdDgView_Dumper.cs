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
  class OdDgView_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgView element = (OdDgView)pObject;
      startDescription(element);
      writeFieldValue("Model ID", element.getModelId());

      writeFieldValue("Is named", element.isNamed());
      writeFieldValue("Name", element.getName());

      writeFieldValue("Index", element.getIndex());

      writeFieldValue("View rectangle", element.getViewRectangle());

      writeFieldValue("Is visible", element.getVisibleFlag());
      writeFieldValue("Fast curves", element.getFastCurveFlag());
      writeFieldValue("Hide texts", element.getHideTextsFlag());
      writeFieldValue("High quality fonts", element.getHighQualityFontsFlag());
      writeFieldValue("Show line weights", element.getShowLineWeightsFlag());
      writeFieldValue("Show patterns", element.getShowPatternsFlag());
      writeFieldValue("Show text nodes", element.getShowTextNodesFlag());
      writeFieldValue("Show data fields", element.getShowDataFieldsFlag());
      writeFieldValue("Show grid", element.getShowGridFlag());
      writeFieldValue("Show level symbology", element.getShowLevelSymbologyFlag());
      writeFieldValue("Show construction", element.getShowConstructionFlag());
      writeFieldValue("Show dimensions", element.getShowDimensionsFlag());
      writeFieldValue("Fast cells", element.getFastCellsFlag());
      writeFieldValue("Show fills", element.getShowFillsFlag());
      writeFieldValue("Show axis triad", element.getShowAxisTriadFlag());
      writeFieldValue("Show background", element.getShowBackgroundFlag());
      writeFieldValue("Show boundary", element.getShowBoundaryFlag());

      if (element.getUseCameraFlag())
      {
        writeShift();
        Program.DumpStream.WriteLine("Specific parameters for perspective camera:\n");
        m_nesting++;

        OdGeMatrix3d rotation = new OdGeMatrix3d();
        OdGePoint3d position = new OdGePoint3d();
        OdGeExtents2d rectangle = new OdGeExtents2d();
        element.getCameraRotation(rotation);
        element.getCameraPosition(position);
        element.getCameraVisibleRectangle(rectangle);

        writeFieldValue("Position", position);
        writeFieldValue("Rotation", rotation);
        writeFieldValue("Focal length", element.getCameraFocalLength());
        writeFieldValue("Visible rectangle", rectangle);
        writeFieldValue("Front clipping", element.getCameraFrontClippingDistance());
        writeFieldValue("Back clipping", element.getCameraBackClippingDistance());

        m_nesting--;
      }
      else
      {
        writeShift();
        Program.DumpStream.WriteLine("Specific parameters for orthography:\n");
        m_nesting++;

        OdGeMatrix3d rotation = new OdGeMatrix3d();
        OdGeExtents3d box = new OdGeExtents3d();
        element.getOrthographyRotation(rotation);
        element.getOrthographyVisibleBox(box);

        writeFieldValue("Rotation", rotation);
        writeFieldValue("Visible box", box);

        m_nesting--;
      }

      OdDgElementIterator pIter = element.createIterator();
      for (pIter.start(); !pIter.done(); pIter.step())
      {
        OdDgElement SubElement = OdDgElement.cast(pIter.item().openObject());
        if (SubElement.isKindOf(OdDgLevelMask.desc()))
        {
          OdDgLevelMask levelmask = OdDgLevelMask.cast(SubElement);
          String sLevelMask;
          UInt32 maxLevelEntryId = levelmask.getMaxLevelEntryId();

          OdDgElementIterator pIt = OdDgElementIterator.cast(element.database().getLevelTable().createIterator());
          for (; !pIt.done(); pIt.step())
          {
            OdDgLevelTableRecord pLevel = OdDgLevelTableRecord.cast(pIt.item().safeOpenObject());
            UInt32 levelEntryId = pLevel.getEntryId();
            sLevelMask = string.Format("LevelMask {0}", levelEntryId);
            bool levelIsVisible = true;
            if (levelEntryId <= maxLevelEntryId || levelEntryId == 64)
            {
              levelIsVisible = levelmask.getLevelIsVisible(levelEntryId);
              writeFieldValue(sLevelMask, levelIsVisible);
            }
            else
            {
              writeFieldValue(sLevelMask, "Unexpected");
            }
          }
        }
      }
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgView";
    }
  }
}