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
  class OdDgText3d_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgText3d element = (OdDgText3d)pObject;
      startDescription(element);
      writeFieldValue("Text", element.getText());
      writeFieldValue("Font ID", element.getFontEntryId());
      // Gets Font name
      OdDgFontTable pFontTable = element.database().getFontTable();
      OdDgFontTableRecord pFont = pFontTable.getFont(element.getFontEntryId());
      if (null != pFont)
      {
        writeFieldValue("Font Name", pFont.getName());
        writeFieldValue("Font number", pFont.getNumber());
        writeFieldValue("Font type", pFont.getType());
      }

      writeFieldValue("Justification", element.getJustification());
      //writeFieldValue( "Length", element.getLength() );
      //writeFieldValue( "Height", element.getHeight() );
      writeFieldValue("Length multiplier", element.getLengthMultiplier());
      writeFieldValue("Height multiplier", element.getHeightMultiplier());
      writeFieldValue("Rotation", element.getRotation());
      writeFieldValue("Origin", element.getOrigin());
      writeFieldValue("TextColor", element.getColorIndex());
      writeFieldValue("Slant", element.getSlant());
      writeFieldValue("ItalicsOverrideFlag", element.getItalicsOverrideFlag());
      writeFieldValue("SlantFlag", element.getSlantFlag());
      writeFieldValue("SlantOverrideFlag", element.getSlantOverrideFlag());
      writeFieldValue("SuperscriptFlag", element.getSuperscriptFlag());
      writeFieldValue("SuperscriptOverrideFlag", element.getSuperscriptOverrideFlag());


      writeFieldValue("HeightOverrideFlag", element.getHeightOverrideFlag());

      writeFieldValue("UnderlineOverrideFlag", element.getUnderlineOverrideFlag());
      writeFieldValue("UnderlineFlag", element.getUnderlineFlag());

      writeFieldValue("TextStyle bit", element.getTextStyleFlag());
      writeFieldValue("TextStyle ID", element.getTextStyleEntryId());

      OdDgTextStyleTable pTextStyleTable = element.database().getTextStyleTable();
      if (null != pTextStyleTable)
      {
        OdDgElementId id = pTextStyleTable.getAt(element.getTextStyleEntryId());
        if (!id.isNull())
        {
          OdDgTextStyleTableRecord pTextStyle = OdDgTextStyleTableRecord.cast(id.safeOpenObject());
          writeFieldValue("TextStyle Name", pTextStyle.getName());
          writeFieldValue("Height from TextStyle", pTextStyle.getTextHeight());
        }
      }
      writeFieldValue("StyleOverridesFlag", element.getStyleOverridesFlag());

      Int16 nCount = element.getTextEditFieldCount();
      writeFieldValue("The number of enter data fields in the text element is ", nCount);
      writeShift();
      for (Int16 i = 0; i < nCount; i++)
      {
        OdDgTextEditField textEditField = element.getTextEditFieldAt(i);

        writeFieldValue("StartPosition", element.getHeight());
        writeFieldValue("Length", element.getRotation());
        writeFieldValue("Justification", element.getOrigin());
      }
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgText3d";
    }
  }

}