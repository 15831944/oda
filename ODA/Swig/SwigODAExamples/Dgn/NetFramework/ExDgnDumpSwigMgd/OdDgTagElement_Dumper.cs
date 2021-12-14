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
  class OdDgTagElement_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgTagElement element = (OdDgTagElement)pObject;
      startDescription(element);
      writeFieldValue("Origin", element.getOrigin());
      writeFieldValue("Offset", element.getOffset());
      writeFieldValue("Rotation (3d)", element.getRotation3d());
      writeFieldValue("Associated", element.getAssociatedFlag());
      writeFieldValue("Offset is used", element.getOffsetUsedFlag());
      {
        OdDgElementId setId = new OdDgElementId();
        ushort definitionId;

        element.getTagDefinitionId(setId, out definitionId);
        writeFieldValue("Id of the set", setId);
        writeFieldValue("Entry Id of the definition", definitionId);
        OdDgTagDefinitionSet pTagSet = OdDgTagDefinitionSet.cast(setId.safeOpenObject());
        if (null != pTagSet)
        {
          writeFieldValue("TagDefinitionsSet Name", pTagSet.getName());
          OdDgTagDefinition pTagDefinition = pTagSet.getByEntryId(definitionId);
          if (null != pTagDefinition)
          {
            writeFieldValue("TagDefinition Name", pTagDefinition.getName());
            writeFieldValue("TagDefinition Type", pTagDefinition.getType());
          }
        }

      }
      writeFieldValue("Id of the associated element", element.getAssociationId());
      writeFieldValue("Freeze group", element.getFreezeGroup());

      switch (element.getDataType())
      {
        case OdDgTagDefinition.Type.kChar:
          writeFieldValue("Type", "char");
          writeFieldValue("Value", element.getString());
          break;
        case OdDgTagDefinition.Type.kInt16:
          writeFieldValue("Type", "int16");
          writeFieldValue("Value", element.getInt16());
          break;
        case OdDgTagDefinition.Type.kInt32:
          writeFieldValue("Type", "int32");
          writeFieldValue("Value", element.getInt32());
          break;
        case OdDgTagDefinition.Type.kDouble:
          writeFieldValue("Type", "int16");
          writeFieldValue("Value", element.getDouble());
          break;
        case OdDgTagDefinition.Type.kBinary:
          writeFieldValue("Type", "int16");
          writeFieldValue("Size", element.getBinarySize());
          break;
        default:
          //ODA_FAIL();
          break;
      }

      writeFieldValue("Use interChar spacing", element.getUseInterCharSpacingFlag());
      writeFieldValue("Fixed width spacing", element.getFixedWidthSpacingFlag());
      writeFieldValue("Underlined", element.getUnderlineFlag());
      writeFieldValue("Use slant", element.getUseSlantFlag());
      writeFieldValue("Vertical", element.getVerticalFlag());
      writeFieldValue("Right-to-left", element.getRightToLeftFlag());
      writeFieldValue("Reverse MLine", element.getReverseMlineFlag());
      writeFieldValue("Kerning", element.getKerningFlag());
      writeFieldValue("Use codepage", element.getUseCodepageFlag());
      writeFieldValue("Use SHX big font", element.getUseShxBigFontFlag());
      writeFieldValue("Subscript", element.getSubscriptFlag());
      writeFieldValue("Superscript", element.getSuperscriptFlag());
      writeFieldValue("Use text style", element.getUseTextStyleFlag());
      writeFieldValue("Overlined", element.getOverlineFlag());
      writeFieldValue("Bold", element.getBoldFlag());
      writeFieldValue("Full justification", element.getFullJustificationFlag());
      writeFieldValue("ACAD interChar spacing", element.getAcadInterCharSpacingFlag());
      writeFieldValue("Backwards", element.getBackwardsFlag());
      writeFieldValue("Upside down", element.getUpsideDownFlag());
      writeFieldValue("ACAD fitted text", element.getAcadFittedTextFlag());

      writeFieldValue("Slant", element.getSlant());
      writeFieldValue("Character spacing", element.getCharacterSpacing());
      writeFieldValue("Underline spacing", element.getUnderlineSpacing());
      writeFieldValue("Length multiplier", element.getLengthMultiplier());
      writeFieldValue("Height multiplier", element.getHeightMultiplier());

      writeFieldValue("Text style ID", element.getTextStyleEntryId());
      writeFieldValue("SHX big font", element.getShxBigFont());
      writeFieldValue("Font ID", element.getFontEntryId());
      writeFieldValue("Justification", element.getJustification());
      writeFieldValue("Codepage", element.getCodepage());
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgTagElement";
    }
  }
}