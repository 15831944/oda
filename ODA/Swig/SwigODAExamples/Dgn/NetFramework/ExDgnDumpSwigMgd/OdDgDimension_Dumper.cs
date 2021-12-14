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
  class OdDgDimension_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgDimension element = (OdDgDimension)pObject;
      startDescription(element);
      writeFieldValue("Tool type", element.getDimensionType());
      writeFieldValue("Scale", element.getScaleFactor());
      writeFieldValueHex("Flags", element.getFlags());
      writeShift();
      switch (element.getAlignment())
      {
        case OdDgDimension.PlacementAlignment.kPaArbitrary:
          Program.DumpStream.WriteLine("Placement Alignment: kPaArbitrary\n");
          break;
        case OdDgDimension.PlacementAlignment.kPaDrawing:
          Program.DumpStream.WriteLine("Placement Alignment: kPaDrawing\n");
          break;
        case OdDgDimension.PlacementAlignment.kPaTrue:
          Program.DumpStream.WriteLine("Placement Alignment: kPaTrue\n");
          break;
        case OdDgDimension.PlacementAlignment.kPaView:
          Program.DumpStream.WriteLine("Placement Alignment: kPaView\n");
          writeShift();
          Program.DumpStream.WriteLine("Alignment View = %d\n", element.getAlignmentView());
          break;
      };
      writeShift();
      switch (element.getArrowHead())
      {
        case OdDgDimension.TerminatorArrowHeadType.kOpened:
          Program.DumpStream.WriteLine("Arrow Head Type: kOpened\n");
          break;
        case OdDgDimension.TerminatorArrowHeadType.kClosed:
          Program.DumpStream.WriteLine("Arrow Head Type: kClosed\n");
          break;
        case OdDgDimension.TerminatorArrowHeadType.kFilled:
          Program.DumpStream.WriteLine("Arrow Head Type: kFilled\n");
          break;
      };
      writeShift();
      if (element.Is3D())
      {
        OdGeQuaternion q = new OdGeQuaternion();
        element.getRotation(q);
        Program.DumpStream.WriteLine(string.Format("Quaternion: [ {0} {1} {2} {3} ]\n", q.w, q.x, q.y, q.z));
      }
      else
      {
        OdGeMatrix2d q = new OdGeMatrix2d();
        element.getRotation(q);
        Program.DumpStream.WriteLine(string.Format("Matrix2d: [[ {0} {1} ] [ {2} {3} ]]\n", q.GetItem(0, 0), q.GetItem(0, 1), q.GetItem(1, 0), q.GetItem(1, 1)));
      }

      writeShift();
      Program.DumpStream.WriteLine("Flags:\n");
      m_nesting++;

      bool CurFlag;
      CurFlag = element.getBoxTextFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getCapsuleTextFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getCentesimalFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getCrossCenterFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getDualFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getEmbedFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getHorizontalFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getJoinerFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getLeadingZero2Flag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getLeadingZerosFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getNoAutoTextLiftFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getNoLevelSymbFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getRelDimLineFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getRelStatFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getStackFractFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getTextHeapPadFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getThousandCommaFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getToleranceFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getTolmodeFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getTrailingZerosFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getTrailingZeros2Flag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }
      CurFlag = element.getUnderlineTextFlag();
      if (CurFlag)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("{0}\n", CurFlag));
      }

      UInt32 i, j = element.getPointsCount();
      OdDgDimPoint point = new OdDgDimPoint();

      writeShift();
      Program.DumpStream.WriteLine(string.Format("Number of points: {0}\n", j));

      for (i = 0; i < j; i++)
      {
        point = element.getPoint(i);
        writeFieldValue(string.Format("Point {0}\n", i), point);
      }

      {
        OdDgDimTextInfo textInfo = new OdDgDimTextInfo();

        element.getDimTextInfo(textInfo);
        writeFieldValue("Text info", textInfo);
      }

      //text format
      {
        OdDgDimTextFormat textFormat = new OdDgDimTextFormat();

        element.getDimTextFormat(textFormat);
        writeFieldValue("Text format", textFormat);
      }

      //geometry
      {
        OdDgDimGeometry geometry = new OdDgDimGeometry();

        element.getDimGeometry(geometry);
        writeFieldValue("Geometry", geometry);
      }

      // Symbology
      {
        Int32 altLineStyle = element.getAltLineStyleEntryId();
        UInt32 altLineWeight = element.getAltLineWeight();
        UInt32 altColorIndex = element.getAltColorIndex();

        writeFieldValue("Alternative LineStyle", altLineStyle);
        writeFieldValue("Alternative LineWeight", altLineWeight);
        writeFieldValue("Alternative ColorIndex", altColorIndex);
      }

      // tools

      writeFieldValue("Tools:", element);

      //options

      UInt32 iOptionsCount = 0;
      if (element.getOption(OdDgDimOption.Type.kTolerance) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kTerminators) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kPrefixSymbol) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kSuffixSymbol) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kDiameterSymbol) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kPrefixSuffix) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kPrimaryUnits) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kSecondaryUnits) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kTerminatorSymbology) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kView) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kAlternatePrimaryUnits) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kOffset) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kAlternateSecondaryUnits) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kAlternatePrefixSymbol) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kAlternateSuffixSymbol) != null) iOptionsCount++;
      if (element.getOption(OdDgDimOption.Type.kProxyCell) != null) iOptionsCount++;

      writeShift();
      writeFieldValue("Number of options", iOptionsCount);
      OdDgDimOption pDimOptions = new OdDgDimOption(IntPtr.Zero, true);
      iOptionsCount = 0;

      pDimOptions = element.getOption(OdDgDimOption.Type.kTolerance);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kTerminators);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kPrefixSymbol);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kSuffixSymbol);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kDiameterSymbol);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kPrefixSuffix);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kPrimaryUnits);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kSecondaryUnits);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kTerminatorSymbology);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kView);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kAlternatePrimaryUnits);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kOffset);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kAlternateSecondaryUnits);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kAlternatePrefixSymbol);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kAlternateSuffixSymbol);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }
      pDimOptions = element.getOption(OdDgDimOption.Type.kProxyCell);
      if (null != pDimOptions)
      {
        writeShift();
        writeFieldValue(string.Format("Dimension option {0}", iOptionsCount), pDimOptions);
        iOptionsCount++;
      }

      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgDimension";
    }
  }
}