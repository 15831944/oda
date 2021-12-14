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
  class OdDgTextStyleTableRecord_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgTextStyleTableRecord element = (OdDgTextStyleTableRecord)pObject;
      startDescription(element);
      writeFieldValue("Text Style Name", element.getName());
      writeFieldValue("Entry ID", element.getEntryId());

      writeFieldValue("Font number", element.getFontEntryId());
      // Gets Font name
      OdDgFontTable pFontTable = element.database().getFontTable();
      OdDgFontTableRecord pFont = pFontTable.getFont(element.getFontEntryId());
      if (null != pFont)
      {
        writeFieldValue("Font Name", pFont.getName());
      }

      writeFieldValue("Justification", element.getJustification());
      writeFieldValue("Line length", element.getLineLength());
      writeFieldValue("Line offset", element.getLineOffset());
      writeFieldValue("Line spacing", element.getLineSpacing());
      writeFieldValue("Font number", element.getFontEntryId());
      writeFieldValue("Interchar spacing", element.getInterCharacter());
      writeFieldValue("Parent text style ID", element.getParentTextStyleEntryId());
      writeFieldValue("ShxBigFontId", element.getShxBigFontId());
      writeFieldValue("Slant", element.getSlant());
      writeFieldValue("Direction", element.getTextDirection());
      writeFieldValue("Height", element.getTextHeight());
      writeFieldValue("Width", element.getTextWidth());
      writeFieldValue("Node justification", element.getTextNodeJustification());
      //writeFieldValue( "Is from table", element.getIsFromTableFlag() );

      writeFieldValue("Underline", element.getUnderlineFlag());
      writeFieldValue("Overline", element.getOverlineFlag());
      writeFieldValue("Italic", element.getItalicsFlag());
      writeFieldValue("Bold", element.getBoldFlag());
      writeFieldValue("Superscript", element.getSuperscriptFlag());
      writeFieldValue("Subscript", element.getSubscriptFlag());
      writeFieldValue("Background flag", element.getBackgroundFlag());
      writeFieldValue("OverlineStyle", element.getOverlineStyleFlag());
      writeFieldValue("UnderlineStyle", element.getUnderlineStyleFlag());
      writeFieldValue("Fixed spacing", element.getFixedSpacingFlag());
      writeFieldValue("Fractions", element.getFractionsFlag());
      writeFieldValue("Color flag", element.getColorFlag());
      writeFieldValue("AcadIntercharSpacingFlag", element.getAcadIntercharSpacingFlag());
      writeFieldValue("FullJustificationFlag", element.getFullJustificationFlag());
      writeFieldValue("AcadShapeFileFlag", element.getAcadShapeFileFlag());

      writeFieldValue("Background border", element.getBackgroundBorder());
      writeFieldValue("Background fill color index", element.getBackgroundFillColorIndex());
      writeFieldValue("Background line color index", element.getBackgroundColorIndex());
      writeFieldValue("Background line style entry id", element.getBackgroundLineStyleEntryId());
      writeFieldValue("Background line weight", element.getBackgroundLineWeight());
      writeFieldValue("Overline line color", element.getOverlineColorIndex());
      writeFieldValue("Overline line style ID", element.getOverlineLineStyleEntryId());
      writeFieldValue("Overline line weight", element.getOverlineLineWeight());
      writeFieldValue("Overline offset", element.getOverlineOffset());
      writeFieldValue("Underline line color", element.getUnderlineColorIndex());
      writeFieldValue("Underline line style ID", element.getUnderlineLineStyleEntryId());
      writeFieldValue("Underline line weight", element.getUnderlineLineWeight());
      writeFieldValue("Underline offset", element.getUnderlineOffset());
      writeFieldValue("LineSpacinType offset", element.getLineSpacingType());

      writeFieldValue("Color index", element.getColorIndex());
      if (element.getColorIndex() == (uint)ColorIndexConstants.kColorByLevel)
      {
        Program.DumpStream.WriteLine("TextStyle has ColorByLevel.\n");
      }

      writeFieldValue("ColorFlagOverrideFlag", element.getColorFlagOverrideFlag());
      writeFieldValue("TextColorOverrideFlag", element.getColorOverrideFlag());
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgTextStyleTableRecord";
    }
  }
}