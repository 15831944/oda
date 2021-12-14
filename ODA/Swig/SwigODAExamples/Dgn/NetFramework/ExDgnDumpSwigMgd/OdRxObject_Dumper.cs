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
using System.Collections;
using System.Runtime.Remoting;
using Teigha.Core;
using Teigha.TG;

namespace ExDgnDumpSwigMgd
{
  public class OdRxObject_Dumper : OdRxObject
  {
    public const int eOk = 0;
    public OdRxObject_Dumper()
        : base(new IntPtr(), true)
    {
    }
    public OdRxObject_Dumper(OdRxObject pObject) : base(OdRxObject.getCPtr(pObject).Handle, true)
    {
    }
    public virtual void dump(OdRxObject pObject) { }
    public virtual String getClassName() { return String.Empty; }               //each pObject class has to define its own name

    public static OdRxObject_Dumper GetProperType(OdRxObject pObject)
    {
      String CurType = pObject.GetType().ToString();
      CurType = CurType.Replace("Teigha.TG", "ExDgnDumpSwigMgd");
      CurType = string.Format("{0}_Dumper", CurType);
      try
      {
        ObjectHandle obj = AppDomain.CurrentDomain.CreateInstance("ExDgnDumpSwigMgd", CurType);
        return (OdRxObject_Dumper)obj.Unwrap();
      }
      catch
      {
        return null;
      }
    }

    public static UInt32 TestCtr = 0;
    //some useful methods for the other dump()'s
    public void startDescription(OdRxObject pObject)
    {
      writeShift();
      m_object = pObject;
      //write it name
      String className;
      {
        OdRxObject_Dumper dumper = GetProperType(pObject);

        //OdRxObject_Dumper dumper = (OdRxObject_Dumper)pObject;

        className = dumper.getClassName();
        Program.DumpStream.WriteLine(className);
      }

      //add the path
      {
        Program.DumpStream.WriteLine(" Path: */");

        int i, j = m_nestedTypes.Count;

        for (i = 0; i < j; i++)
        {
          Program.DumpStream.WriteLine(m_nestedTypes[i].ToString());
        }
      }

      //final actions
      Program.DumpStream.WriteLine();
      m_nestedTypes.Add(className);
      m_nesting++;

      //dump specific information
      writeLinkagesInfo(m_object);
      writeElementInfo(m_object);
      writeGraphicsElementInfo(m_object);
    }


    public void finishDescription()
    {
      m_nesting--;

      writeShift();
      Program.DumpStream.WriteLine(m_nestedTypes[m_nestedTypes.Count - 1]);

      m_nestedTypes.RemoveAt(m_nestedTypes.Count - 1);
    }


    public void describeSubElements(OdDgElementIterator iterator)
    {
      int counter = 0;
      for (; !iterator.done(); iterator.step(), counter++)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("Sub element {0}:\n", counter));
        describeSubElement(iterator.item().openObject(OpenMode.kForRead));
        Program.DumpStream.Flush();
      }
    }

    public void describeSubElement(OdRxObject someObject)
    {
      OdRxObject_Dumper pDumper = GetProperType(someObject);// (OdRxObject_Dumper)someObject;
      {
        if (null != pDumper)
        {
          pDumper.dump(someObject);
        }
        else
        {
          m_nesting++;
          Program.DumpStream.WriteLine("Object does not have its dumper\n");
          writeElementInfo(someObject);
          m_nesting--;
        }
      }
    }

    public void writeShift()
    {
      int i;

      for (i = 0; i < m_nesting; i++)
      {
        Program.DumpStream.WriteLine("  ");
      }
    }

    public void writeElementInfo(OdRxObject pObject)
    {
      OdDgElement element = OdDgElement.cast(pObject);
      if (null == element)
      {
        return;
      }

      writeShift();
      Program.DumpStream.WriteLine("Common information for DGN elements:\n");
      m_nesting++;

      writeFieldValue("ID", element.elementId().getHandle().ascii());
      writeFieldValue("Type", element.getElementType().ToString());
      if (element.isDBRO())
      {
        writeFieldValue("Is Locked", element.getLockedFlag());
      }

      m_nesting--;
    }

    public void writeGraphicsElementInfoLS3(OdRxObject pObject)
    {
      //OdDgGraphicsElement element = new OdDgGraphicsElement(OdRxObject.getCPtr(pObject).Handle, false);
      OdDgGraphicsElement element = OdDgGraphicsElement.cast(pObject);
      if (null == element)
      {
        return;
      }

      writeShift();
      Program.DumpStream.WriteLine("Common information for Graphics Elements:\n");

      m_nesting++;

      UInt32 level = element.getLevelEntryId();
      OdDgElementId id = element.database().getLevelTable().getAt(level);
      writeShift();
      if (id.isNull())
      {
        Program.DumpStream.WriteLine("level id is incorrect\n");
      }
      else
      {
        Program.DumpStream.WriteLine("level id is correct\n");
      }

      writeFieldValue("Level ID", element.getLevelEntryId());

      UInt32 uColorIndex = element.getColorIndex();

      if ((uColorIndex != (UInt32)ColorIndexConstants.kColorByLevel) & (uColorIndex != (UInt32)ColorIndexConstants.kColorByCell))
      {
        writeFieldValue("Color", element.getColor());
      }

      writeFieldValue("Color index", uColorIndex);

      switch (uColorIndex)
      {
        case (UInt32)ColorIndexConstants.kColorByLevel: Program.DumpStream.WriteLine("Color is defined by level\n"); break;
        case (UInt32)ColorIndexConstants.kColorByCell: Program.DumpStream.WriteLine("Color is defined by cell\n"); break;
      }

      writeFieldValue("Graphics group", element.getGraphicsGroupEntryId());
      writeFieldValue("Class", element.getClass());
      writeFieldValue("Line style", element.getLineStyleEntryId());
      if (element.getLineStyleEntryId() == (int)LineStyleConstants.kLineStyleByLevel)
      {
        Program.DumpStream.WriteLine("Element has LineStyleByLevel.\n");
      }
      writeFieldValue("Line weight", element.getLineWeight());
      if (element.getLineWeight() == (uint)LineWeightConstants.kLineWeightByLevel)
      {
        Program.DumpStream.WriteLine("Element has WeightByLevel.\n");
      }

      writeFieldValue("Is Invisible", element.getInvisibleFlag());
      writeFieldValue("Is 3D Format Element", element.get3dFormatFlag());
      writeFieldValue("View Independent", element.getViewIndependentFlag());
      writeFieldValue("Non Planar", element.getNonPlanarFlag());
      writeFieldValue("Not Snappable", element.getNotSnappableFlag());
      writeFieldValue("Hbit", element.getHbitFlag());

      //show the extents: recalculated one  saved one
      {
        OdGeExtents3d recalculatedExtent = new OdGeExtents3d();
        OdGeExtents3d savedExtent = new OdGeExtents3d();

        //recalculated
        {
          if (element.getGeomExtents(recalculatedExtent) == eOk)
          {
            writeFieldValue("Result of getGeomExtents()", recalculatedExtent);
          }
          else
          {
            writeFieldValue("Result of getGeomExtents()", "invalid value");
          }
        }
        //saved
        OdDgGraphicsElementPE pElementPE = OdDgGraphicsElementPE.cast(element);//(OdRxObject.getCPtr(element).Handle, true);
                                                                               //OdDgGraphicsElementPEPtr pElementPE = OdDgGraphicsElementPEPtr(OdRxObjectPtr(element));
        if (null != pElementPE)
        {
          if (pElementPE.getRange(element, savedExtent) == eOk)
          {
            writeFieldValue("Saved extents", savedExtent);
          }
          else
          {
            writeFieldValue("Saved extents", "invalid value");
          }
        }
      }

      m_nesting--;
    }

    public void writeGraphicsElementInfo(OdRxObject pObject)
    {
      OdDgGraphicsElement element = OdDgGraphicsElement.cast(pObject);
      if (null == element)
      {
        return;
      }
      else
      {
        ///////// test bug 16516 ////////////
        OdGiDrawable pDr = OdGiDrawable.cast(pObject);
        OdGeExtents3d extents = new OdGeExtents3d();
        //System.Windows.Forms.MessageBox.Show("before getgeomextents test");
        OdResult res = pDr.getGeomExtents(extents);
        /*if (OdResult.eOk != res)
        {
            System.Windows.Forms.MessageBox.Show("after getgeomextents test");
        }*/
        /////////////////////////////////////
      }

      writeShift();
      Program.DumpStream.WriteLine("Common information for Graphics Elements:\n");

      m_nesting++;

      UInt32 level = element.getLevelEntryId();
      OdDgElementId id = element.database().getLevelTable().getAt(level);
      writeShift();
      if (id.isNull())
      {
        Program.DumpStream.WriteLine("level id is incorrect\n");
      }
      else
      {
        Program.DumpStream.WriteLine("level id is correct\n");
      }

      writeFieldValue("Level ID", element.getLevelEntryId());

      UInt32 uColorIndex = element.getColorIndex();

      if ((uColorIndex != (UInt32)ColorIndexConstants.kColorByLevel) & (uColorIndex != (UInt32)ColorIndexConstants.kColorByCell))
      {
        writeFieldValue("Color", element.getColor());
      }

      writeFieldValue("Color index", uColorIndex);

      switch (uColorIndex)
      {
        case (UInt32)ColorIndexConstants.kColorByLevel: Program.DumpStream.WriteLine("Color is defined by level\n"); break;
        case (UInt32)ColorIndexConstants.kColorByCell: Program.DumpStream.WriteLine("Color is defined by cell\n"); break;
      }

      writeFieldValue("Graphics group", element.getGraphicsGroupEntryId());
      writeFieldValue("Class", element.getClass());
      writeFieldValue("Line style", element.getLineStyleEntryId());
      if (element.getLineStyleEntryId() == (int)LineStyleConstants.kLineStyleByLevel)
      {
        Program.DumpStream.WriteLine("Element has LineStyleByLevel.\n");
      }
      writeFieldValue("Line weight", element.getLineWeight());
      if (element.getLineWeight() == (uint)LineWeightConstants.kLineWeightByLevel)
      {
        Program.DumpStream.WriteLine("Element has WeightByLevel.\n");
      }

      writeFieldValue("Is Invisible", element.getInvisibleFlag());
      writeFieldValue("Is 3D Format Element", element.get3dFormatFlag());
      writeFieldValue("View Independent", element.getViewIndependentFlag());
      writeFieldValue("Non Planar", element.getNonPlanarFlag());
      writeFieldValue("Not Snappable", element.getNotSnappableFlag());
      writeFieldValue("Hbit", element.getHbitFlag());

      //show the extents: recalculated one  saved one
      {
        OdGeExtents3d recalculatedExtent = new OdGeExtents3d();
        OdGeExtents3d savedExtent = new OdGeExtents3d();

        //recalculated
        {
          if (element.getGeomExtents(recalculatedExtent) == eOk)
          {
            writeFieldValue("Result of getGeomExtents()", recalculatedExtent);
          }
          else
          {
            writeFieldValue("Result of getGeomExtents()", "invalid value");
          }
        }
        //saved
        OdDgGraphicsElementPE pElementPE = OdDgGraphicsElementPE.cast(element);//(OdRxObject.getCPtr(element).Handle, true);
                                                                               //OdDgGraphicsElementPEPtr pElementPE = OdDgGraphicsElementPEPtr(OdRxObjectPtr(element));
        if (null != pElementPE)
        {
          if (pElementPE.getRange(element, savedExtent) == eOk)
          {
            writeFieldValue("Saved extents", savedExtent);
          }
          else
          {
            writeFieldValue("Saved extents", "invalid value");
          }
        }
      }

      m_nesting--;
    }


    public void writeLinkagesInfo(OdRxObject pObject)
    {
      OdDgElement element = OdDgElement.cast(pObject);

      if (null == element)
      {
        return;
      }

      //take all linkages
      OdRxObjectPtrArray linkages = new OdRxObjectPtrArray();
      element.getLinkages(linkages);
      int linkagesNumber = linkages.Count;

      if (linkagesNumber > 0)
      {
        writeShift();
        Program.DumpStream.WriteLine(string.Format("> Attribute Linkages (%{0} items)\n", linkagesNumber));
        m_nesting++;

        for (int i = 0; i < linkagesNumber; ++i)
        {
          OdDgAttributeLinkage pLinkage = (OdDgAttributeLinkage)linkages[i];

          OdBinaryData data = new OdBinaryData();
          pLinkage.getData(data);
          writeShift();
          Program.DumpStream.WriteLine(string.Format("Primary ID = 0x%{0}, data size = %{1}", pLinkage.getPrimaryId(), data.Count));

          //additionary info depending on the type
          switch (pLinkage.getPrimaryId())
          {
            case (ushort)OdDgPatternLinkage.PrimaryIds.kHatch:
              {
                OdDgPatternLinkage pPatternLinkage = OdDgPatternLinkage.cast(pLinkage);
                if (null != pPatternLinkage)
                {
                  OdDgHatchPattern pHatchPattern = pPatternLinkage.getHatchPattern();
                  if (pHatchPattern != null)
                  {
                    String namedType;
                    switch (pHatchPattern.getType())
                    {
                      case OdDgHatchPattern.PatternType.kLinearPattern: namedType = "LinearPattern"; break;
                      case OdDgHatchPattern.PatternType.kCrossPattern: namedType = "CrossPattern"; break;
                      case OdDgHatchPattern.PatternType.kSymbolPattern: namedType = "SymbolPattern"; break;
                      case OdDgHatchPattern.PatternType.kDWGPattern: namedType = "DWGPattern"; break;
                      default: namedType = "Unknown"; break;
                    }
                    Program.DumpStream.WriteLine(string.Format(" ( Pattern type = {0}", namedType));
                    if (pHatchPattern.getUseOffsetFlag())
                    {
                      OdGePoint3d offset = new OdGePoint3d();
                      pHatchPattern.getOffset(offset);
                      //Program.DumpStream.WriteLine("; offset = (%g %g %g)", offset.x, offset.y, offset.z );
                      Program.DumpStream.WriteLine(string.Format("; offset = (%g %g %g)", offset.x, offset.y, offset.z));
                    }
                    Program.DumpStream.WriteLine(")");
                  }
                }
              }
              break;
            case (ushort)OdDgPatternLinkage.PrimaryIds.kThickness:
              {
                OdDgThicknessLinkage pThicknessLinkage = OdDgThicknessLinkage.cast(pLinkage);

                if (null != pThicknessLinkage)
                {
                  Program.DumpStream.WriteLine(string.Format(" ( Thickness Linkage, thickness = {0} )", pThicknessLinkage.getThickness()));
                }
              }
              break;
            case (ushort)OdDgPatternLinkage.PrimaryIds.kString:
              {
                OdDgStringLinkage pStrLinkage = OdDgStringLinkage.cast(pLinkage);

                if (null != pStrLinkage)
                {
                  Program.DumpStream.WriteLine(string.Format(" ( String Linkage, ID = {0}; value = {1})", pStrLinkage.getStringId(), pStrLinkage.getString()));
                }
              }
              break;
            case (ushort)OdDgPatternLinkage.PrimaryIds.kDMRS:
              {
                OdDgDMRSLinkage DMRSLinkage = OdDgDMRSLinkage.cast(pLinkage);
                if (null != DMRSLinkage)
                {
                  Program.DumpStream.WriteLine(string.Format(" ( DMRS Linkage, tableId = {0}, MSLink = {1}, type = {2} )", DMRSLinkage.getTableId(), DMRSLinkage.getMSLink(), DMRSLinkage.getType()));
                }
              }
              break;
            case 0x56D5:
              {
                OdDgProxyLinkage linkage = OdDgProxyLinkage.cast(pLinkage);
                if (null != linkage)
                {
                  OdBinaryData d_data = new OdBinaryData();
                  linkage.getData(d_data);
                  Int32 ii = d_data.Count;
                  Int32 j = d_data.Count;
                  Program.DumpStream.WriteLine(" ( Proxy linkage )\n");
                  m_nesting++;
                  writeFieldValue("Size of the proxy linkage", j);
                  for (ii = 0; ii < j; ii++)
                  {
                    if (0 == (ii % 16))
                    {
                      if (ii > 0)
                      {
                        Program.DumpStream.WriteLine("\n");
                      }
                      writeShift();
                      //Program.DumpStream.WriteLine("%.4X: ", i );
                      Program.DumpStream.WriteLine(ii.ToString());
                    }
                    //Program.DumpStream.WriteLine("%.2X ", data[ i ] );
                    Program.DumpStream.WriteLine(d_data[ii].ToString());
                  }
                  m_nesting--;
                }
              }
              break;
            case (ushort)OdDgPatternLinkage.PrimaryIds.kFRAMME: // DB Linkage - FRAMME tag data signature
            case (ushort)OdDgPatternLinkage.PrimaryIds.kBSI: // DB Linkage - secondary id link (BSI radix 50)
            case (ushort)OdDgPatternLinkage.PrimaryIds.kXBASE: // DB Linkage - XBase (DBase)
            case (ushort)OdDgPatternLinkage.PrimaryIds.kINFORMIX: // DB Linkage - Informix
            case (ushort)OdDgPatternLinkage.PrimaryIds.kINGRES: // DB Linkage - INGRES
            case (ushort)OdDgPatternLinkage.PrimaryIds.kSYBASE: // DB Linkage - Sybase
            case (ushort)OdDgPatternLinkage.PrimaryIds.kODBC: // DB Linkage - ODBC
            case (ushort)OdDgPatternLinkage.PrimaryIds.kOLEDB: // DB Linkage - OLEDB
            case (ushort)OdDgPatternLinkage.PrimaryIds.kORACLE: // DB Linkage - Oracle
            case (ushort)OdDgPatternLinkage.PrimaryIds.kRIS: // DB Linkage - RIS
              {
                OdDgDBLinkage dbLinkage = OdDgDBLinkage.cast(pLinkage);
                if (null != dbLinkage)
                {
                  String namedType;

                  switch (dbLinkage.getDBType())
                  {
                    case OdDgDBLinkage.DBType.kBSI: namedType = "BSI"; break;
                    case OdDgDBLinkage.DBType.kFRAMME: namedType = "FRAMME"; break;
                    case OdDgDBLinkage.DBType.kInformix: namedType = "Informix"; break;
                    case OdDgDBLinkage.DBType.kIngres: namedType = "Ingres"; break;
                    case OdDgDBLinkage.DBType.kODBC: namedType = "ODBC"; break;
                    case OdDgDBLinkage.DBType.kOLEDB: namedType = "OLE DB"; break;
                    case OdDgDBLinkage.DBType.kOracle: namedType = "Oracle"; break;
                    case OdDgDBLinkage.DBType.kRIS: namedType = "RIS"; break;
                    case OdDgDBLinkage.DBType.kSybase: namedType = "Sybase"; break;
                    case OdDgDBLinkage.DBType.kXbase: namedType = "xBase"; break;
                    default: namedType = "Unknown"; break;
                  }

                  Program.DumpStream.WriteLine(string.Format(" ( DB Linkage, tableId = {0}, MSLink = {1}, type = {2} )", dbLinkage.getTableEntityId(), dbLinkage.getMSLink(), namedType));
                }
              }
              break;
            case (ushort)OdDgPatternLinkage.PrimaryIds.kDimension: // Dimension Linkage
              {
                OdDgDimensionLinkage dimLinkage = OdDgDimensionLinkage.cast(pLinkage);
                if (null != dimLinkage)
                {
                  String sDimType;
                  switch (dimLinkage.getType())
                  {
                    case OdDgDimensionLinkage.DimensionSubType.kOverall: sDimType = "Overall"; break;
                    case OdDgDimensionLinkage.DimensionSubType.kSegment: sDimType = "Segment"; break;
                    case OdDgDimensionLinkage.DimensionSubType.kPoint: sDimType = "Point"; break;
                    case OdDgDimensionLinkage.DimensionSubType.kSegmentFlags: sDimType = "SegmentFlags"; break;
                    case OdDgDimensionLinkage.DimensionSubType.kDimensionInfo: sDimType = "DimensionInfo"; break;
                    default: sDimType = "Unkown"; break;
                  }
                  Program.DumpStream.WriteLine(" ( Dimension Linkage, type = {0} )", sDimType);
                  switch (dimLinkage.getType())
                  {
                    case OdDgDimensionLinkage.DimensionSubType.kOverall:
                      break;
                    case OdDgDimensionLinkage.DimensionSubType.kSegment:
                      break;
                    case OdDgDimensionLinkage.DimensionSubType.kPoint:
                      break;
                    case OdDgDimensionLinkage.DimensionSubType.kSegmentFlags:
                      break;
                    case OdDgDimensionLinkage.DimensionSubType.kDimensionInfo:
                      {
                        OdDgDimensionInfoLinkage pDimInfoLinkage = (OdDgDimensionInfoLinkage)pLinkage;

                        if (pDimInfoLinkage.getUseAnnotationScaleValue())
                          writeFieldValue("  Annotation Scale", pDimInfoLinkage.getAnnotationScale());

                        if (pDimInfoLinkage.getUseDatumValue())
                        {
                          double dDatumValue = pDimInfoLinkage.getDatumValue();

                          OdDgDatabase pDb = element.database();
                          OdDgElementId idModel = new OdDgElementId();

                          if (null != pDb)
                            idModel = pDb.getActiveModelId();

                          if (null != idModel)
                          {
                            OdDgModel pModel = OdDgModel.cast(idModel.openObject());

                            if (null != pModel)
                            {
                              dDatumValue = pModel.convertUORsToWorkingUnits(dDatumValue);
                            }
                          }
                          else
                          {
                            dDatumValue /= 10000000000; // Storage units default factor
                          }

                          writeFieldValue("  Datum Value", dDatumValue);
                        }

                        if (pDimInfoLinkage.getUseRetainFractionalAccuracy())
                        {
                          writeFieldValue("  Detriment in reverse direction", pDimInfoLinkage.getUseDecrimentInReverceDirection());
                          writeFieldValue("  Primary retain fractional accuracy", pDimInfoLinkage.getPrimaryRetainFractionalAccuracy());
                          writeFieldValue("  Secondary retain fractional accuracy", pDimInfoLinkage.getSecondaryRetainFractionalAccuracy());
                          writeFieldValue("  Primary alt format retain fractional accuracy", pDimInfoLinkage.getPrimaryAltFormatRetainFractionalAccuracy());
                          writeFieldValue("  Secondary alt format retain fractional accuracy", pDimInfoLinkage.getSecondaryAltFormatRetainFractionalAccuracy());
                          writeFieldValue("  Primary tolerance retain fractional accuracy", pDimInfoLinkage.getPrimaryTolerRetainFractionalAccuracy());
                          writeFieldValue("  Secondary tolerance retain fractional accuracy", pDimInfoLinkage.getSecondaryTolerRetainFractionalAccuracy());
                          writeFieldValue("  Label line mode", pDimInfoLinkage.getLabelLineDimensionMode());
                        }

                        if (pDimInfoLinkage.getUseFitOptionsFlag())
                        {
                          writeFieldValue("  Suppress unfit terminators", pDimInfoLinkage.getUseSuppressUnfitTerm());
                          writeFieldValue("  Use inline leader length", pDimInfoLinkage.getUseInlineLeaderLength());
                          writeFieldValue("  Text above optimal fit", pDimInfoLinkage.getUseTextAboveOptimalFit());
                          writeFieldValue("  Narrow font optimal fit", pDimInfoLinkage.getUseNarrowFontOptimalFit());
                          writeFieldValue("  Use Min Leader Terminator Length", pDimInfoLinkage.getUseMinLeaderTermLength());
                          writeFieldValue("  Use auto mode for dimension leader", pDimInfoLinkage.getUseAutoLeaderMode());
                          writeFieldValue("  Fit Options ", pDimInfoLinkage.getFitOptions());
                        }

                        if (pDimInfoLinkage.getUseTextLocation())
                        {
                          writeFieldValue("  Free location of text", pDimInfoLinkage.getUseFreeLocationText());
                          writeFieldValue("  Note spline fit", pDimInfoLinkage.getUseNoteSplineFit());
                          writeFieldValue("  Text location ", pDimInfoLinkage.getTextLocation());
                        }

                        if (pDimInfoLinkage.getUseInlineLeaderLengthValue())
                        {
                          double dLengthValue = pDimInfoLinkage.getInlineLeaderLength();

                          OdDgDatabase pDb = element.database();
                          OdDgElementId idModel = new OdDgElementId();

                          if (null != pDb)
                            idModel = pDb.getActiveModelId();

                          if (null != idModel)
                          {
                            OdDgModel pModel = OdDgModel.cast(idModel.openObject());

                            if (null != pModel)
                            {
                              dLengthValue = pModel.convertUORsToWorkingUnits(dLengthValue);
                            }
                          }
                          else
                          {
                            dLengthValue /= 10000000000; // Storage units default factor
                          }

                          writeFieldValue("  Inline leader length value", dLengthValue);
                        }

                        if (pDimInfoLinkage.getUseInlineTextLift())
                          writeFieldValue("  Inline text lift", pDimInfoLinkage.getInlineTextLift());

                        if (pDimInfoLinkage.getUseNoteFrameScale())
                          writeFieldValue("  Note frame scale", pDimInfoLinkage.getUseNoteFrameScale());

                        if (pDimInfoLinkage.getUseNoteLeaderLength())
                          writeFieldValue("  Note leader length", pDimInfoLinkage.getNoteLeaderLength());

                        if (pDimInfoLinkage.getUseNoteLeftMargin())
                          writeFieldValue("  Note left margin", pDimInfoLinkage.getUseNoteLeftMargin());

                        if (pDimInfoLinkage.getUseNoteLowerMargin())
                          writeFieldValue("  Note lower margin", pDimInfoLinkage.getUseNoteLowerMargin());

                        if (pDimInfoLinkage.getUsePrimaryToleranceAccuracy())
                          writeFieldValue("  Primary tolerance accuracy", pDimInfoLinkage.getPrimaryToleranceAccuracy());

                        if (pDimInfoLinkage.getUseSecondaryToleranceAccuracy())
                          writeFieldValue("  Secondary tolerance accuracy", pDimInfoLinkage.getSecondaryToleranceAccuracy());

                        if (pDimInfoLinkage.getUseStackedFractionScale())
                          writeFieldValue("  Stacked fraction scale", pDimInfoLinkage.getStackedFractionScale());
                      }
                      break;
                    default:
                      break;
                  }
                }
                break;
              }
            case (ushort)OdDgPatternLinkage.PrimaryIds.kFilterMember:
              {
                OdDgFilterMemberLinkage pFilterLinkage = (OdDgFilterMemberLinkage)pLinkage;
                writeFieldValue("  Member Id", pFilterLinkage.getMemberId());
                writeFieldValue("  Member Type", pFilterLinkage.getMemberType());
                writeFieldValue("  Name String", pFilterLinkage.getNameString());
                writeFieldValue("  Expression String", pFilterLinkage.getExpressionString());
              }
              break;
            case (ushort)OdDgPatternLinkage.PrimaryIds.kDependency:
              {
                OdDgDependencyLinkage dependencyLinkage = OdDgDependencyLinkage.cast(pLinkage);
                if (null != dependencyLinkage)
                {
                  Program.DumpStream.WriteLine(string.Format("( Root type = {0}; App ID = {1}; App Value = {2} )",
                    dependencyLinkage.getRootDataType(),
                    dependencyLinkage.getAppValue(),
                    dependencyLinkage.getAppValue()));

                  //some additional information
                  m_nesting++;
                  switch (dependencyLinkage.getRootDataType())
                  {
                    case OdDgDependencyLinkage.RootDataType.kElementId:
                      OdDgDepLinkageElementId elementIdLinkage = OdDgDepLinkageElementId.cast(dependencyLinkage);
                      if (null != elementIdLinkage)
                      {
                        UInt32 j = elementIdLinkage.getCount();
                        UInt32 ii = elementIdLinkage.getCount();
                        Program.DumpStream.WriteLine("\n");
                        m_nesting++;
                        writeShift();
                        Program.DumpStream.WriteLine(string.Format("Number of IDs: {0}; They are:", j));
                        for (ii = 0; ii < j; ii++)
                        {
                          Program.DumpStream.WriteLine(string.Format(" {0}.8I64X", elementIdLinkage.getAt(ii)));
                        }
                        m_nesting--;
                      }
                      break;
                    case OdDgDependencyLinkage.RootDataType.kAssocPoint_I:
                      {
                        OdDgDepLinkageAssocPointI pDepLinkageAssocPt = OdDgDepLinkageAssocPointI.cast(dependencyLinkage);
                        var idCount = pDepLinkageAssocPt.getCount();

                        for (UInt32 j = 0; j < idCount; ++j)
                        {
                          /*tmp#18875*/
                          OdDgAssocPointIData rootData = pDepLinkageAssocPt.getAt(j);
                          Program.DumpStream.WriteLine("\n");
                          m_nesting++;
                          writeShift();
                          Program.DumpStream.WriteLine(string.Format("Root Number {0}", j));
                          Program.DumpStream.WriteLine(string.Format("PointData {0}", rootData.getPointData().ToString()));
                          Program.DumpStream.WriteLine(string.Format("   Int 1 {0}", rootData.m_iParam1));
                          Program.DumpStream.WriteLine(string.Format("   Int 2 {0}", rootData.m_iParam2));
                          m_nesting--;
                        }
                      }
                      break;
                  }
                  m_nesting--;
                }
              }
              break;
          }
          Program.DumpStream.WriteLine("\n");
        }

        m_nesting--;
        writeShift();
        Program.DumpStream.WriteLine("< Attribute Linkages\n");
      }
    }


    public void writeFieldValue(String name, String value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}\n", value));
    }


    public void writeFieldValue(String name, UInt16 value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}\n", value));
    }


    public void writeFieldValueHex(String name, UInt16 value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0:X}\n", value));
    }


    public void writeFieldValue(String name, UInt32 value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}\n", value));
    }

    public void writeFieldValue(String name, OdDgLevelFilterTable.OdDgFilterMemberType value)
    {
      String strValue;

      switch (value)
      {
        case OdDgLevelFilterTable.OdDgFilterMemberType.kMemberTypeShort:
          {
            strValue = ("kTypeShort");
          }
          break;

        case OdDgLevelFilterTable.OdDgFilterMemberType.kMemberTypeInt:
          {
            strValue = ("kTypeInt");
          }
          break;

        case OdDgLevelFilterTable.OdDgFilterMemberType.kMemberTypeBool:
          {
            strValue = ("kTypeBool");
          }
          break;

        case OdDgLevelFilterTable.OdDgFilterMemberType.kMemberTypeChar:
          {
            strValue = ("kTypeChar");
          }
          break;

        case OdDgLevelFilterTable.OdDgFilterMemberType.kMemberTypeWChar:
          {
            strValue = ("kTypeWChar");
          }
          break;

        case OdDgLevelFilterTable.OdDgFilterMemberType.kMemberTypeDouble:
          {
            strValue = ("kTypeDouble");
          }
          break;

        case OdDgLevelFilterTable.OdDgFilterMemberType.kMemberTypeTime:
          {
            strValue = ("kTypeTime");
          }
          break;

        default:
          {
            strValue = ("kTypeNull");
          }
          break;
      }

      writeFieldValue(name, strValue);
    }

    public void writeFieldValueHex(String name, UInt32 value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0:X}\n", value));
    }


    public void writeFieldValue(String name, double value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0:F}\n", value));
    }


    public void writeFieldValue(String name, OdGePoint2d value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}; {1}\n", value.x, value.y));
    }


    public void writeFieldValue(String name, OdGePoint3d value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}; {1}; {2}\n", value.x, value.y, value.z));
    }


    public void writeFieldValue(String name, OdGeVector3d value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}; {1}; {2}\n", value.x, value.y, value.z));
    }


    public void writeFieldValue(String name, OdCmEntityColor value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("R: {0} G: {1} B: {2}\n",
        value.red(), value.green(), value.blue()));
    }


    public void writeFieldValue(String name, OdDgLineStyleInfo value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("Modifiers: {0:X} Scale: {1} Shift: {2} Start width: {3} End width: {4}\n",
        value.getModifiers(), value.getScale(), value.getShift(), value.getStartWidth(), value.getEndWidth()));
    }


    public void writeFieldValue(String name, UInt64 value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}\n", value));
    }


    public void writeFieldValue(String name, OdDgLightColor value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("R: {0} G: {1} B: {2} Intensity: {3}\n",
                value.getRed(), value.getGreen(), value.getBlue(), value.getIntensityScale()));
    }


    public void writeFieldValue(String name, bool value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(value ? "true\n" : "false\n");
    }


    public void writeFieldValue(String name, OdAngleCoordinate value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}° {1}' {2}\"\n",
                value.getDegrees(), value.getMinutes(), value.getSeconds()));
    }


    public void writeFieldValue(String name, Int16 value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}\n", value));
    }


    public void writeFieldValue(String name, Int32 value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}I32i\n", value));
    }

    public void writeFieldValue(String name, Int64 value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}iI64\n", value));
    }


    public void writeFieldValue(String name, OdDgModel.WorkingUnit value)
    {
      writeFieldName(name);

      switch (value)
      {
        case OdDgModel.WorkingUnit.kWuUnitOfResolution: Program.DumpStream.WriteLine("unit of resolution\n"); break;
        case OdDgModel.WorkingUnit.kWuStorageUnit: Program.DumpStream.WriteLine("storage unit\n"); break;
        case OdDgModel.WorkingUnit.kWuWorldUnit: Program.DumpStream.WriteLine("world unit\n"); break;
        case OdDgModel.WorkingUnit.kWuMasterUnit: Program.DumpStream.WriteLine("master unit\n"); break;
        case OdDgModel.WorkingUnit.kWuSubUnit: Program.DumpStream.WriteLine("sub unit\n"); break;
      }
    }


    public void writeFieldValue(String name, TextJustification value)
    {
      writeFieldName(name);

      switch (value)
      {
        case TextJustification.kLeftTop: Program.DumpStream.WriteLine("left top\n"); break;
        case TextJustification.kLeftCenter: Program.DumpStream.WriteLine("left center\n"); break;
        case TextJustification.kLeftBottom: Program.DumpStream.WriteLine("left bottom\n"); break;
        case TextJustification.kLeftMarginTop: Program.DumpStream.WriteLine("left margin top\n"); break;
        case TextJustification.kLeftMarginCenter: Program.DumpStream.WriteLine("left margin center\n"); break;
        case TextJustification.kLeftMarginBottom: Program.DumpStream.WriteLine("left margin bottom\n"); break;
        case TextJustification.kCenterTop: Program.DumpStream.WriteLine("center top\n"); break;
        case TextJustification.kCenterCenter: Program.DumpStream.WriteLine("center center\n"); break;
        case TextJustification.kCenterBottom: Program.DumpStream.WriteLine("center bottom\n"); break;
        case TextJustification.kRightMarginTop: Program.DumpStream.WriteLine("right margin top\n"); break;
        case TextJustification.kRightMarginCenter: Program.DumpStream.WriteLine("right margin center\n"); break;
        case TextJustification.kRightMarginBottom: Program.DumpStream.WriteLine("right margin bottom\n"); break;
        case TextJustification.kRightTop: Program.DumpStream.WriteLine("right top\n"); break;
        case TextJustification.kRightCenter: Program.DumpStream.WriteLine("right center\n"); break;
        case TextJustification.kRightBottom: Program.DumpStream.WriteLine("right bottom\n"); break;
      }
    }


    public void writeFieldValue(String name, OdGeQuaternion value)
    {
      writeFieldName(name);

      Program.DumpStream.WriteLine(string.Format("{0}; {1}; {2}; {3}\n", value.w, value.x, value.y, value.z));
    }

    public void writeFieldValue(String name, OdDgGraphicsElement.Class value)
    {
      writeFieldName(name);

      switch (value)
      {
        case OdDgGraphicsElement.Class.kClassPrimary: Program.DumpStream.WriteLine("Primary\n"); break;
        case OdDgGraphicsElement.Class.kClassPatternComponent: Program.DumpStream.WriteLine("Pattern component\n"); break;
        case OdDgGraphicsElement.Class.kClassConstruction: Program.DumpStream.WriteLine("Construction\n"); break;
        case OdDgGraphicsElement.Class.kClassDimension: Program.DumpStream.WriteLine("Dimension\n"); break;
        case OdDgGraphicsElement.Class.kClassPrimaryRule: Program.DumpStream.WriteLine("Primary rule\n"); break;
        case OdDgGraphicsElement.Class.kClassLinearPatterned: Program.DumpStream.WriteLine("Linear patterned\n"); break;
        case OdDgGraphicsElement.Class.kClassConstructionRule: Program.DumpStream.WriteLine("Construction rule\n"); break;
      }
    }


    public void writeFieldValue(String name, OdGeMatrix2d value)
    {
      writeFieldName(name);
      Program.DumpStream.WriteLine(string.Format("{0}; {1}; {2}; {3}\n", value[0, 0], value[1, 0], value[0, 1], value[1, 1]));
    }


    public void writeFieldValue(String name, OdDgDimension.ToolType value)
    {
      writeFieldName(name);
      OdDgDimension pDim = OdDgDimension.cast(m_object);
      switch (value)
      {
        case OdDgDimension.ToolType.kToolTypeInvalid: Program.DumpStream.WriteLine("Invalid\n"); break;
        case OdDgDimension.ToolType.kToolTypeSizeArrow: Program.DumpStream.WriteLine("Size arrow\n"); break;
        case OdDgDimension.ToolType.kToolTypeSizeStroke: Program.DumpStream.WriteLine("Size stroke\n"); break;
        case OdDgDimension.ToolType.kToolTypeLocateSingle: Program.DumpStream.WriteLine("Locate single\n"); break;
        case OdDgDimension.ToolType.kToolTypeLocateStacked: Program.DumpStream.WriteLine("Locate stacked\n"); break;
        case OdDgDimension.ToolType.kToolTypeAngleSize: Program.DumpStream.WriteLine("Angle size\n"); break;
        case OdDgDimension.ToolType.kToolTypeArcSize: Program.DumpStream.WriteLine("Arc size\n"); break;
        case OdDgDimension.ToolType.kToolTypeAngleLocation: Program.DumpStream.WriteLine("Angle location\n"); break;
        case OdDgDimension.ToolType.kToolTypeArcLocation: Program.DumpStream.WriteLine("Arc location\n"); break;
        case OdDgDimension.ToolType.kToolTypeAngleLines: Program.DumpStream.WriteLine("Angle lines\n"); break;
        case OdDgDimension.ToolType.kToolTypeAngleAxis: Program.DumpStream.WriteLine("Angle axis\n"); break;
        case OdDgDimension.ToolType.kToolTypeRadius: Program.DumpStream.WriteLine("Radius\n"); break;
        case OdDgDimension.ToolType.kToolTypeDiameter: Program.DumpStream.WriteLine("Diameter\n"); break;
        case OdDgDimension.ToolType.kToolTypeDiameterPara: Program.DumpStream.WriteLine("Diameter para\n"); break;
        case OdDgDimension.ToolType.kToolTypeDiameterPerp: Program.DumpStream.WriteLine("Diameter perp\n"); break;
        case OdDgDimension.ToolType.kToolTypeCustomLinear: Program.DumpStream.WriteLine("Custom linear\n"); break;
        case OdDgDimension.ToolType.kToolTypeOrdinate:
          {
            Program.DumpStream.WriteLine("Ordinate\n");

            OdDgDimOrdinate pOrdinateDim = OdDgDimOrdinate.cast(pDim);

            if (null != pOrdinateDim)
            {
              m_nesting++;

              if (pOrdinateDim.getArcSymbolFlag())
                Program.DumpStream.WriteLine("Tool Arc Symbol Flag set\n");

              Program.DumpStream.WriteLine(string.Format("Tool Datum Value: {0}\n", pOrdinateDim.getDatumValue()));

              if (pOrdinateDim.getDecrementInReverseDirectionFlag())
                Program.DumpStream.WriteLine("Tool DecrementInReverse Direction Flag set\n");

              if (pOrdinateDim.getFreeLocationOfTxtFlag())
                Program.DumpStream.WriteLine("Tool FreeLocationOfTxt Flag set\n");

              if (pOrdinateDim.getStackExtLinesFlag())
                Program.DumpStream.WriteLine("Tool StackExtLines Flag set\n");

              m_nesting--;
            }
          }
          break;
        case OdDgDimension.ToolType.kToolTypeRadiusExtended: Program.DumpStream.WriteLine("Radius extended\n"); break;
        case OdDgDimension.ToolType.kToolTypeDiameterExtended: Program.DumpStream.WriteLine("Diameter extended\n"); break;
        case OdDgDimension.ToolType.kToolTypeCenter: Program.DumpStream.WriteLine("Center\n"); break;
      }
    }


    public void writeFieldValue(String name, OdDgDimPoint value)
    {
      writeShift();
      Program.DumpStream.WriteLine(name);

      m_nesting++;

      writeFieldValue("  Point", value.getPoint());
      writeFieldValue("  Offset to dimension line", value.getOffsetToDimLine());
      writeFieldValue("  Offset Y", value.getOffsetY());

      switch (value.getJustification())
      {
        case OdDgDimTextInfo.TextAlignment.kLeftText:
          {
            writeFieldValue("  Text alignment", "kLeftText");
          }
          break;

        case OdDgDimTextInfo.TextAlignment.kCenterLeftText:
          {
            writeFieldValue("  Text alignment", ("kCenterLeftText"));
          }
          break;

        case OdDgDimTextInfo.TextAlignment.kCenterRightText:
          {
            writeFieldValue("  Text alignment", ("kCenterRightText"));
          }
          break;

        case OdDgDimTextInfo.TextAlignment.kRightText:
          {
            writeFieldValue("  Text alignment", ("kRightText"));
          }
          break;

        case OdDgDimTextInfo.TextAlignment.kManualText:
          {
            writeFieldValue("  Text alignment", ("kManualText"));
          }
          break;
      }

      writeFieldName("  Flags:");
      writeFieldValue("    Associative", value.getAssociativeFlag());
      writeFieldValue("    Relative", value.getRelative() != 0);
      writeFieldValue("    WitnessControlLocal", value.getWitnessControlLocalFlag());
      writeFieldValue("    NoWitnessLine", value.getNoWitnessLineFlag());
      writeFieldValue("    UseAltSymbology", value.getUseAltSymbologyFlag());

      if (value.getPrimaryTextFlag())
      {
        writeFieldValue("  Primary text", value.getPrimaryText());
      }

      if (value.getPrimaryTopToleranceTextFlag())
      {
        writeFieldValue("  Primary Top text", value.getPrimaryTopToleranceText());
      }

      if (value.getPrimaryBottomToleranceTextFlag())
      {
        writeFieldValue("  Primary Bottom text", value.getPrimaryBottomToleranceText());
      }

      if (value.getSecondaryTextFlag())
      {
        writeFieldValue("  Secondary text", value.getSecondaryText());
      }

      if (value.getSecondaryTopToleranceTextFlag())
      {
        writeFieldValue("  Secondary Top text", value.getSecondaryTopToleranceText());
      }

      if (value.getSecondaryBottomToleranceTextFlag())
      {
        writeFieldValue("  Secondary Bottom text", value.getSecondaryBottomToleranceText());
      }

      m_nesting--;
    }


    public void writeFieldValue(String name, OdDgDimTextInfo value)
    {
      writeShift();
      Program.DumpStream.WriteLine(name);

      m_nesting++;

      writeFieldValue("  Width", value.getWidth());
      writeFieldValue("  Height", value.getHeight());
      writeFieldValue("  Font ID", value.getFontEntryId());
      writeFieldValue("  Color", value.getColorIndex());
      writeFieldValue("  Weight", value.getWeight());

      switch (value.getStackedFractionType())
      {
        case OdDgDimTextInfo.StackedFractionType.kFractionFromText:
          {
            writeFieldValue("  Stacked Fraction Type", ("kFractionFromText"));
          }
          break;

        case OdDgDimTextInfo.StackedFractionType.kFractionHorizontal:
          {
            writeFieldValue("  Stacked Fraction Type", ("kFractionHorizontal"));
          }
          break;

        case OdDgDimTextInfo.StackedFractionType.kFractionDiagonal:
          {
            writeFieldValue("  Stacked Fraction Type", ("kFractionDiagonal"));
          }
          break;
      }

      switch (value.getStackFractAlignment())
      {
        case OdDgDimTextInfo.StackFractionAlignment.kFractAlignmentTop:
          {
            writeFieldValue("  Stacked Fraction Alignment", ("kFractAlignmentTop"));
          }
          break;

        case OdDgDimTextInfo.StackFractionAlignment.kFractAlignmentCenter:
          {
            writeFieldValue("  Stacked Fraction Alignment", ("kFractAlignmentCenter"));
          }
          break;

        case OdDgDimTextInfo.StackFractionAlignment.kFractAlignmentBottom:
          {
            writeFieldValue("  Stacked Fraction Alignment", ("kFractAlignmentBottom"));
          }
          break;
      }

      writeFieldName("  Text Flags:");
      writeFieldValue("    Use text color", value.getUseColorFlag());
      writeFieldValue("    Use weight ", value.getUseWeightFlag());
      writeFieldValue("    Show primary master units ", !value.getPrimaryNoMasterUnitsFlag());
      writeFieldValue("    Has primary alt format ", !value.getHasAltFormatFlag());
      writeFieldValue("    Show secondary master units ", !value.getSecNoMasterUnitsFlag());
      writeFieldValue("    Has secondary alt format ", !value.getHasSecAltFormatFlag());

      m_nesting--;
    }


    public void writeFieldValue(String name, OdDgDimTextFormat value)
    {
      writeShift();
      Program.DumpStream.WriteLine(name);

      m_nesting++;

      writeFieldValue("  Primary accuracy", value.getPrimaryAccuracy());
      writeFieldValue("  Secondary accuracy", value.getSecondaryAccuracy());

      switch (value.getAngleMode())
      {
        case OdDgDimTextFormat.AngleValueDisplayMode.kAngle_D:
          {
            writeFieldValue("  Angle display mode", ("kAngle_D"));
          }
          break;

        case OdDgDimTextFormat.AngleValueDisplayMode.kAngle_DM:
          {
            writeFieldValue("  Angle display mode", ("kAngle_DM"));
          }
          break;

        case OdDgDimTextFormat.AngleValueDisplayMode.kAngle_DMS:
          {
            writeFieldValue("  Angle display mode", ("kAngle_DMS"));
          }
          break;

        case OdDgDimTextFormat.AngleValueDisplayMode.kAngle_C:
          {
            writeFieldValue("  Angle display mode", ("kAngle_C"));
          }
          break;

        case OdDgDimTextFormat.AngleValueDisplayMode.kAngle_Radian:
          {
            writeFieldValue("  Angle display mode", ("kAngle_Radian"));
          }
          break;
      }

      writeFieldName("  Text Format Flags:");
      writeFieldValue("    AngleMeasure", value.getAngleMeasureFlag());
      writeFieldValue("    AngleFormat", value.getAngleFormatFlag());
      writeFieldValue("    PrimarySubUnits", value.getPrimarySubUnitsFlag());
      writeFieldValue("    PrimaryLabels", value.getPrimaryLabelsFlag());
      writeFieldValue("    PrimaryDelimiter", value.getPrimaryDelimiterFlag());
      writeFieldValue("    DecimalComma", value.getDecimalCommaFlag());
      writeFieldValue("    SuperScriptLSD", value.getSuperScriptLSDFlag());
      writeFieldValue("    RoundLSD", value.getRoundLSDFlag());
      writeFieldValue("    OmitLeadDelimiter", value.getOmitLeadDelimiterFlag());
      writeFieldValue("    LocalFileUnits", value.getLocalFileUnitsFlag());
      writeFieldValue("    UnusedDeprecated", value.getUnusedDeprecatedFlag());
      writeFieldValue("    ThousandSeparator", value.getThousandSepFlag());
      writeFieldValue("    MetricSpace", value.getMetricSpaceFlag());
      writeFieldValue("    SecondarySubUnits", value.getSecondarySubUnitsFlag());
      writeFieldValue("    SecondaryLabels", value.getSecondaryLabelsFlag());
      writeFieldValue("    SecondaryDelimiter", value.getSecondaryDelimiterFlag());
      writeFieldValue("    Radians", value.getRadiansFlag());
      writeFieldValue("    Show primary master units if zero", value.getPriAllowZeroMastFlag());
      writeFieldValue("    Show secondary master units if zero", value.getSecAllowZeroMastFlag());
      writeFieldValue("    Show primary sub units if zero", !value.getPriSubForbidZeroMastFlag());
      writeFieldValue("    Show secondary sub units if zero", !value.getSecSubForbidZeroMastFlag());
      writeFieldValue("    HideAngleSeconds", !value.getHideAngleSecondsFlag());
      writeFieldValue("    SkipNonStackedFractionSpace", !value.getSkipNonStackedFractionSpaceFlag());

      m_nesting--;
    }


    public void writeFieldValue(String name, OdDgDimTextFormat.Accuracy value)
    {
      writeFieldName(name);

      switch (value)
      {
        case OdDgDimTextFormat.Accuracy.kAccuracyNone: Program.DumpStream.WriteLine("1 digit\n"); break;

        case OdDgDimTextFormat.Accuracy.kDecimal1: Program.DumpStream.WriteLine("Decimal, 2 digit\n"); break;
        case OdDgDimTextFormat.Accuracy.kDecimal2: Program.DumpStream.WriteLine("Decimal, 3 digit\n"); break;
        case OdDgDimTextFormat.Accuracy.kDecimal3: Program.DumpStream.WriteLine("Decimal, 4 digit\n"); break;
        case OdDgDimTextFormat.Accuracy.kDecimal4: Program.DumpStream.WriteLine("Decimal, 5 digit\n"); break;
        case OdDgDimTextFormat.Accuracy.kDecimal5: Program.DumpStream.WriteLine("Decimal, 6 digit\n"); break;
        case OdDgDimTextFormat.Accuracy.kDecimal6: Program.DumpStream.WriteLine("Decimal, 7 digit\n"); break;
        case OdDgDimTextFormat.Accuracy.kDecimal7: Program.DumpStream.WriteLine("Decimal, 8 digit\n"); break;
        case OdDgDimTextFormat.Accuracy.kDecimal8: Program.DumpStream.WriteLine("Decimal, 9 digit\n"); break;

        case OdDgDimTextFormat.Accuracy.kFractional2: Program.DumpStream.WriteLine("Fractional, 2-th\n"); break;
        case OdDgDimTextFormat.Accuracy.kFractional4: Program.DumpStream.WriteLine("Fractional, 4-th\n"); break;
        case OdDgDimTextFormat.Accuracy.kFractional8: Program.DumpStream.WriteLine("Fractional, 8-th\n"); break;
        case OdDgDimTextFormat.Accuracy.kFractional16: Program.DumpStream.WriteLine("Fractional, 16-th\n"); break;
        case OdDgDimTextFormat.Accuracy.kFractional32: Program.DumpStream.WriteLine("Fractional, 32-th\n"); break;
        case OdDgDimTextFormat.Accuracy.kFractional64: Program.DumpStream.WriteLine("Fractional, 64-th\n"); break;

        case OdDgDimTextFormat.Accuracy.kExponential1: Program.DumpStream.WriteLine("Exponential, 1 digit for mantissa\n"); break;
        case OdDgDimTextFormat.Accuracy.kExponential2: Program.DumpStream.WriteLine("Exponential, 2 digit for mantissa\n"); break;
        case OdDgDimTextFormat.Accuracy.kExponential3: Program.DumpStream.WriteLine("Exponential, 3 digit for mantissa\n"); break;
        case OdDgDimTextFormat.Accuracy.kExponential4: Program.DumpStream.WriteLine("Exponential, 4 digit for mantissa\n"); break;
        case OdDgDimTextFormat.Accuracy.kExponential5: Program.DumpStream.WriteLine("Exponential, 5 digit for mantissa\n"); break;
        case OdDgDimTextFormat.Accuracy.kExponential6: Program.DumpStream.WriteLine("Exponential, 6 digit for mantissa\n"); break;
        case OdDgDimTextFormat.Accuracy.kExponential7: Program.DumpStream.WriteLine("Exponential, 7 digit for mantissa\n"); break;
        case OdDgDimTextFormat.Accuracy.kExponential8: Program.DumpStream.WriteLine("Exponential, 8 digit for mantissa\n"); break;
      }
    }


    public void writeFieldValue(String name, OdDgDimGeometry value)
    {
      writeShift();
      Program.DumpStream.WriteLine(name);

      m_nesting++;

      writeFieldValue("  Witness line offset", value.getWitnessLineOffset());
      writeFieldValue("  Witness line extend", value.getWitnessLineExtend());
      writeFieldValue("  Text lift", value.getTextLift());
      writeFieldValue("  Text margin", value.getTextMargin());
      writeFieldValue("  Terminator width", value.getTerminatorWidth());
      writeFieldValue("  Terminator height", value.getTerminatorHeight());
      writeFieldValue("  Stack offset", value.getStackOffset());
      writeFieldValue("  Center size", value.getCenterSize());

      if (value.getUseMargin())
        writeFieldValue("  Min leader", value.getMargin());
      else
        writeFieldValue("  Min leader", value.getTerminatorWidth() * 2.0);

      m_nesting--;
    }


    public void writeFieldValue(String name, OdDgDimOption value)
    {
      writeShift();
      Program.DumpStream.WriteLine(name);

      m_nesting++;

      writeShift();
      if (null != value)
      {
        switch (value.getType())
        {
          case OdDgDimOption.Type.kNone:
            {
              writeFieldValue("  Type", (("kNone")));
            }
            break;

          case OdDgDimOption.Type.kTolerance:
            {
              OdDgDimOptionTolerance pTolerOptions = (OdDgDimOptionTolerance)value;
              writeFieldValue("", pTolerOptions);
            }
            break;

          case OdDgDimOption.Type.kTerminators:
            {
              OdDgDimOptionTerminators pTermOptions = (OdDgDimOptionTerminators)value;
              writeFieldValue("", pTermOptions);
            }
            break;

          case OdDgDimOption.Type.kPrefixSymbol:
            {
              OdDgDimOptionSymbol pSymbolOptions = (OdDgDimOptionSymbol)value;
              writeFieldValue("  Type", (("kPrefixSymbol")));
              writeFieldValue("", pSymbolOptions);
            }
            break;

          case OdDgDimOption.Type.kSuffixSymbol:
            {
              OdDgDimOptionSymbol pSymbolOptions = (OdDgDimOptionSymbol)value;
              writeFieldValue("  Type", (("kSuffixSymbol")));
              writeFieldValue("", pSymbolOptions);
            }
            break;

          case OdDgDimOption.Type.kDiameterSymbol:
            {
              OdDgDimOptionSymbol pSymbolOptions = (OdDgDimOptionSymbol)value;
              writeFieldValue("  Type", (("kDiameterSymbol")));
              writeFieldValue("", pSymbolOptions);
            }
            break;

          case OdDgDimOption.Type.kPrefixSuffix:
            {
              OdDgDimOptionPrefixSuffix pPrefixSuffixOptions = (OdDgDimOptionPrefixSuffix)value;
              writeFieldValue("", pPrefixSuffixOptions);
            }
            break;

          case OdDgDimOption.Type.kPrimaryUnits:
            {
              OdDgDimOptionUnits pUnitsOptions = (OdDgDimOptionUnits)value;
              writeFieldValue("  Type", (("kPrimaryUnits")));
              writeFieldValue("", pUnitsOptions);
            }
            break;

          case OdDgDimOption.Type.kSecondaryUnits:
            {
              OdDgDimOptionUnits pUnitsOptions = (OdDgDimOptionUnits)value;
              writeFieldValue("  Type", (("kSecondaryUnits")));
              writeFieldValue("", pUnitsOptions);
            }
            break;

          case OdDgDimOption.Type.kTerminatorSymbology:
            {
              OdDgDimOptionTerminatorSymbology pTermSymbolOptions = (OdDgDimOptionTerminatorSymbology)value;
              writeFieldValue("", pTermSymbolOptions);
            }
            break;

          case OdDgDimOption.Type.kView:
            {
              OdDgDimOptionView pViewOptions = (OdDgDimOptionView)value;
              writeFieldValue("  Type", (("kView")));

              if (null != pViewOptions)
                writeFieldValue("  Rotation", pViewOptions.getQuaternion());
            }
            break;

          case OdDgDimOption.Type.kAlternatePrimaryUnits:
            {
              OdDgDimOptionAltFormat pAltOptions = (OdDgDimOptionAltFormat)value;
              writeFieldValue("  Type", (("kAlternativePrimaryUnits")));
              writeFieldValue("", pAltOptions);
            }
            break;

          case OdDgDimOption.Type.kOffset:
            {
              OdDgDimOptionOffset pOffsetOptions = (OdDgDimOptionOffset)value;
              writeFieldValue("", pOffsetOptions);
            }
            break;

          case OdDgDimOption.Type.kAlternateSecondaryUnits:
            {
              OdDgDimOptionAltFormat pAltOptions = (OdDgDimOptionAltFormat)value;
              writeFieldValue("  Type", (("kAlternativeSecondaryUnits")));
              writeFieldValue("", pAltOptions);
            }
            break;

          case OdDgDimOption.Type.kAlternatePrefixSymbol:
            {
              OdDgDimOptionSymbol pSymbolOptions = (OdDgDimOptionSymbol)value;
              writeFieldValue("  Type", (("kAlternatePrefixSymbol")));
              writeFieldValue("", pSymbolOptions);
            }
            break;

          case OdDgDimOption.Type.kAlternateSuffixSymbol:
            {
              OdDgDimOptionSymbol pSymbolOptions = (OdDgDimOptionSymbol)value;
              writeFieldValue("  Type", (("kAlternateSuffixSymbol")));
              writeFieldValue("", pSymbolOptions);
            }
            break;

          case OdDgDimOption.Type.kProxyCell:
            {
              OdDgDimOptionProxyCell pCellOptions = (OdDgDimOptionProxyCell)value;
              writeFieldValue("", pCellOptions);
            }
            break;
        }
      }
      else
      {
        writeFieldValue("  Type", (("[value unknown]")));
      }

      m_nesting--;
    }

    public void writeFieldValue(String name, OdDgDimLabelLine.LabelLineDimensionMode value)
    {
      String val;

      switch (value)
      {
        case OdDgDimLabelLine.LabelLineDimensionMode.kAngleLength:
          val = ("Angle/Length"); break;
        case OdDgDimLabelLine.LabelLineDimensionMode.kLengthAbove:
          val = ("Length above"); break;
        case OdDgDimLabelLine.LabelLineDimensionMode.kAngleAbove:
          val = ("Angle above"); break;
        case OdDgDimLabelLine.LabelLineDimensionMode.kLengthBelow:
          val = ("Length below"); break;
        case OdDgDimLabelLine.LabelLineDimensionMode.kAngleBelow:
          val = ("Angle below"); break;
        case OdDgDimLabelLine.LabelLineDimensionMode.kLengthAngleAbove:
          val = ("Length Angle above"); break;
        case OdDgDimLabelLine.LabelLineDimensionMode.kLengthAngleBelow:
          val = ("Length Angle below"); break;
        default:
          val = ("Length/Angle"); break;
      }

      writeFieldValue(name, val);
    }

    public void writeFieldValue(String name, OdDgDimTextInfo.FitOptions value)
    {
      String strVal = String.Empty;

      switch (value)
      {
        case OdDgDimTextInfo.FitOptions.kTermMovesFirst: strVal = "kTermMovesFirst"; break;
        case OdDgDimTextInfo.FitOptions.kTermReversed: strVal = "kTermReversed"; break;
        case OdDgDimTextInfo.FitOptions.kTermInside: strVal = "kTermInside"; break;
        case OdDgDimTextInfo.FitOptions.kTermOutside: strVal = "kTermOutside"; break;
        case OdDgDimTextInfo.FitOptions.kTextInside: strVal = "kTextInside"; break;
        case OdDgDimTextInfo.FitOptions.kTextMovesFirst: strVal = "kTextMovesFirst"; break;
        case OdDgDimTextInfo.FitOptions.kBothMoves: strVal = "kBothMoves"; break;
        case OdDgDimTextInfo.FitOptions.kSmallestMoves: strVal = "kSmallestMoves"; break;
      }

      writeFieldValue(name, strVal);
    }

    public void writeFieldValue(String name, OdDgDimTextInfo.TextLocation value)
    {
      String strVal;

      switch (value)
      {
        case OdDgDimTextInfo.TextLocation.kTextInline: strVal = "kTextInline"; break;
        case OdDgDimTextInfo.TextLocation.kTextAbove: strVal = "kTextAbove"; break;
        case OdDgDimTextInfo.TextLocation.kTextOutside: strVal = "kTextOutside"; break;
        default: strVal = "kTextTopLeft"; break;
      }

      writeFieldValue(name, strVal);
    }

    public void writeFieldValue(String Val, OdDgDimOptionTolerance value)
    {
      writeFieldValue("  Type", (("kTolerance")));

      if (null == value)
        return;

      writeFieldValue("  Upper value", value.getToleranceUpper());
      writeFieldValue("  Lower value", value.getToleranceLower());
      writeFieldValue("  Stack if equal", value.getStackEqualFlag());
      writeFieldValue("  Show sign for zero", value.getShowSignForZeroFlag());
      writeFieldValue("  Left margin", value.getToleranceHorizSep());
      writeFieldValue("  Separator margin", value.getToleranceVertSep());
      writeFieldValue("  Font entry Id", value.getFontEntryId());
      writeFieldValue("  Text Width", value.getToleranceTextWidth());
      writeFieldValue("  Text Height", value.getToleranceTextHeight());

      if (0 != value.getTolerancePlusMinusSymbol())
        writeFieldValue("  Plus/Minus symbol", value.getTolerancePlusMinusSymbol());

      if (0 != value.getTolerancePrefixSymbol())
        writeFieldValue("  Prefix symbol", value.getTolerancePrefixSymbol());

      if (0 != value.getToleranceSuffixSymbol())
        writeFieldValue("  Suffix symbol", value.getToleranceSuffixSymbol());

      writeFieldValue("  Stack align", value.getStackAlign());
    }

    public void writeFieldValue(String Val, OdDgDimOptionTerminators value)
    {
      writeFieldValue("  Type", (("kTerminators")));

      if (null == value)
        return;

      if (value.getArrowTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermDefault)
      {
        writeFieldValue("  Arrow style", (("kTermDefault")));
      }
      else if (value.getArrowTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermSymbol)
      {
        writeFieldValue("  Arrow style", (("kTermSymbol")));
        writeFieldValue("  Arrow Font entry Id", value.getArrowFontID());
        writeFieldValue("  Arrow Symbol code", value.getArrowSymbol());
      }
      else if (value.getArrowTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermCell)
      {
        writeFieldValue("  Arrow style", (("kTermCell")));
        writeFieldValue("  Arrow Cell Id", value.getArrowCellID());
      }
      else
      {
        writeFieldValue("  Arrow style", (("kTermScaledCell")));
        writeFieldValue("  Arrow Cell Id", value.getArrowCellID());
        writeFieldValue("  Arrow Cell scale", value.getSharedCellScale());
      }

      if (value.getStrokeTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermDefault)
      {
        writeFieldValue("  Stroke style", (("kTermDefault")));
      }
      else if (value.getStrokeTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermSymbol)
      {
        writeFieldValue("  Stroke style", (("kTermSymbol")));
        writeFieldValue("  Stroke Font entry Id", value.getStrokeFontID());
        writeFieldValue("  Stroke Symbol code", value.getStrokeSymbol());
      }
      else if (value.getStrokeTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermCell)
      {
        writeFieldValue("  Stroke style", (("kTermCell")));
        writeFieldValue("  Stroke Cell Id", value.getStrokeCellID());
      }
      else
      {
        writeFieldValue("  Stroke style", (("kTermScaledCell")));
        writeFieldValue("  Stroke Cell Id", value.getStrokeCellID());
        writeFieldValue("  Stroke Cell scale", value.getSharedCellScale());
      }

      if (value.getOriginTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermDefault)
      {
        writeFieldValue("  Origin style", (("kTermDefault")));
      }
      else if (value.getOriginTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermSymbol)
      {
        writeFieldValue("  Origin style", (("kTermSymbol")));
        writeFieldValue("  Origin Font entry Id", value.getOriginFontID());
        writeFieldValue("  Origin Symbol code", value.getOriginSymbol());
      }
      else if (value.getOriginTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermCell)
      {
        writeFieldValue("  Origin style", (("kTermCell")));
        writeFieldValue("  Origin Cell Id", value.getOriginCellID());
      }
      else
      {
        writeFieldValue("  Origin style", (("kTermScaledCell")));
        writeFieldValue("  Origin Cell Id", value.getOriginCellID());
        writeFieldValue("  Origin Cell scale", value.getSharedCellScale());
      }

      if (value.getDotTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermDefault)
      {
        writeFieldValue("  Dot style", (("kTermDefault")));
      }
      else if (value.getDotTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermSymbol)
      {
        writeFieldValue("  Dot style", (("kTermSymbol")));
        writeFieldValue("  Dot Font entry Id", value.getDotFontID());
        writeFieldValue("  Dot Symbol code", value.getDotSymbol());
      }
      else if (value.getDotTermStyle() == OdDgDimOptionTerminators.TerminatorStyle.kTermCell)
      {
        writeFieldValue("  Dot style", (("kTermCell")));
        writeFieldValue("  Dot Cell Id", value.getDotCellID());
      }
      else
      {
        writeFieldValue("  Dot style", (("kTermScaledCell")));
        writeFieldValue("  Dot Cell Id", value.getDotCellID());
        writeFieldValue("  Dot Cell scale", value.getSharedCellScale());
      }
    }

    public void writeFieldValue(String Val, OdDgDimOptionTerminatorSymbology value)
    {
      writeFieldValue("  Type", "kTerminatorSymbology");

      if (null == value)
        return;

      writeFieldValue("  Use Line type", value.getStyleFlag());
      writeFieldValue("  Use Line weight", value.getWeightFlag());
      writeFieldValue("  Use Color", value.getColorFlag());

      if (value.getStyleFlag())
        writeFieldValue("  Line Type entry Id", value.getStyle());

      if (value.getWeightFlag())
        writeFieldValue("  Line Weight", value.getWeight());

      if (value.getColorFlag())
        writeFieldValue("  Color", value.getColor());
    }

    public void writeFieldValue(String Val, OdDgDimOptionSymbol value)
    {
      if (null == value)
        return;

      writeFieldValue("  Font entry Id", value.getSymbolFont());
      writeFieldValue("  Symbol code", value.getSymbolChar());
    }

    public void writeFieldValue(String Val, OdDgDimOptionPrefixSuffix value)
    {
      writeFieldValue("  Type", "kPrefixSuffix");

      if (null == value)
        return;

      if (0 != value.getMainPrefix())
      {
        writeFieldValue("  Main prefix", value.getMainPrefix());
      }

      if (0 != value.getMainSuffix())
      {
        writeFieldValue("  Main suffix", value.getMainSuffix());
      }

      if (0 != value.getUpperPrefix())
      {
        writeFieldValue("  Upper prefix", value.getUpperPrefix());
      }

      if (0 != value.getUpperSuffix())
      {
        writeFieldValue("  Upper suffix", value.getUpperSuffix());
      }

      if (0 != value.getLowerPrefix())
      {
        writeFieldValue("  Lower prefix", value.getLowerPrefix());
      }

      if (0 != value.getLowerSuffix())
      {
        writeFieldValue("  Lower suffix", value.getLowerSuffix());
      }
    }

    public void writeFieldValue(String name, OdDgModel.UnitBase value)
    {
      if (value == OdDgModel.UnitBase.kNone)
      {
        writeFieldValue(name, (("kNone")));
      }
      else if (value == OdDgModel.UnitBase.kMeter)
      {
        writeFieldValue(name, (("kMeter")));
      }
      else
      {
        writeFieldValue(name, (("Unknown")));
      }
    }

    public void writeFieldValue(String name, OdDgModel.UnitSystem value)
    {
      if (value == OdDgModel.UnitSystem.kCustom)
      {
        writeFieldValue(name, (("kCustom")));
      }
      else if (value == OdDgModel.UnitSystem.kMetric)
      {
        writeFieldValue(name, (("kMetric")));
      }
      else if (value == OdDgModel.UnitSystem.kEnglish)
      {
        writeFieldValue(name, (("kEnglish")));
      }
      else
      {
        writeFieldValue(name, (("Unknown")));
      }
    }

    public void writeFieldValue(String Val, OdDgDimOptionUnits value)
    {
      if (null == value)
        return;

      //SWIGTYPE_p_OdDgModel__UnitDescription descr;
      OdDgModel.UnitDescription descr = new OdDgModel.UnitDescription();
      value.getMasterUnit(descr);
      writeFieldName("  Master units:");
      writeFieldValue("    Unit base", descr.m_base);
      writeFieldValue("    Unit system", descr.m_system);
      writeFieldValue("    Denominator", descr.m_denominator);
      writeFieldValue("    Numerator", descr.m_numerator);
      writeFieldValue("    Name", descr.m_name);
      value.getSubUnit(descr);
      writeFieldName("  Sub units:");
      writeFieldValue("    Unit base", descr.m_base);
      writeFieldValue("    Unit system", descr.m_system);
      writeFieldValue("    Denominator", descr.m_denominator);
      writeFieldValue("    Numerator", descr.m_numerator);
      writeFieldValue("    Name", descr.m_name);
    }

    public void writeFieldValue(String Val, OdDgDimOptionAltFormat value)
    {
      if (null == value)
        return;

      writeFieldValue("  Accuracy", value.getAccuracy());
      writeFieldValue("  Show sub units", value.getSubUnits());
      writeFieldValue("  Show unit labels", value.getLabel());
      writeFieldValue("  Show delimiter", value.getDelimiter());
      writeFieldValue("  Show sub units only", value.getNoMasterUnits());
      writeFieldValue("  Allow zero master units", value.getAllowZeroMasterUnits());

      if (value.getMoreThanThreshold())
      {
        if (value.getEqualToThreshold())
        {
          writeFieldValue("  Condition", ((">=")));
        }
        else
        {
          writeFieldValue("  Condition", ((">")));
        }
      }
      else
      {
        if (value.getEqualToThreshold())
        {
          writeFieldValue("  Condition", (("<=")));
        }
        else
        {
          writeFieldValue("  Condition", (("<")));
        }
      }

      writeFieldValue("  Threshold", value.getThreshold());
    }

    public void writeFieldValue(String name, OdDgDimOptionOffset.ChainType value)
    {
      String strValue = String.Empty;

      switch (value)
      {
        case OdDgDimOptionOffset.ChainType.kNone:
          {
            strValue = "kNone";
          }
          break;

        case OdDgDimOptionOffset.ChainType.kLine:
          {
            strValue = "kLine";
          }
          break;

        case OdDgDimOptionOffset.ChainType.kArc:
          {
            strValue = "kArc";
          }
          break;

        case OdDgDimOptionOffset.ChainType.kBSpline:
          {
            strValue = "kBSpline";
          }
          break;
      }

      writeFieldValue(name, strValue);
    }

    public void writeFieldValue(String name, OdDgDimOptionOffset.LeaderAlignment value)
    {
      String strValue = String.Empty;

      switch (value)
      {
        case OdDgDimOptionOffset.LeaderAlignment.kAutoAlignment:
          {
            strValue = ("kAutoAlignment");
          }
          break;

        case OdDgDimOptionOffset.LeaderAlignment.kLeftAlignment:
          {
            strValue = ("kLeftAlignment");
          }
          break;

        case OdDgDimOptionOffset.LeaderAlignment.kRightAlignment:
          {
            strValue = ("kRightAlignment");
          }
          break;
      }

      writeFieldValue(name, strValue);
    }


    public void writeFieldValue(String Val, OdDgDimOptionOffset value)
    {
      writeFieldValue("  Type", (("kOffset")));

      if (null == value)
        return;

      writeFieldValue("  Terminator", value.getTerminator());
      writeFieldValue("  Chain type", value.getChainType());
      writeFieldValue("  Elbow", value.getElbowFlag());
      writeFieldValue("  Alignment", value.getAlignment());
      writeFieldValue("  No dock on dim line", value.getNoDockOnDimLineFlag());
    }

    public void writeFieldValue(String Val, OdDgDimOptionProxyCell value)
    {
      writeFieldValue("  Type", (("kProxyCell")));

      if (null == value)
        return;

      writeFieldValue("  Origin", value.getOrigin());
      writeFieldValue("  Rotation Matrix", value.getRotScale());
      writeFieldValue("  Check Sum", value.getCheckSum());
    }


    public void writeFieldValue(String name, OdDgMultilineSymbology value)
    {
      writeShift();
      Program.DumpStream.WriteLine(name);

      m_nesting++;

      writeFieldValue("Style", value.getLineStyleEntryId());
      writeFieldValue("Weight", value.getLineWeight());
      writeFieldValue("Color", value.getColorIndex());
      writeFieldValue("Use style", value.getUseStyleFlag());
      writeFieldValue("Use weight", value.getUseWeightFlag());
      writeFieldValue("Use color", value.getUseColorFlag());
      writeFieldValue("Use class", value.getUseClassFlag());
      writeFieldValue("Inside arc", value.getCapInArcFlag());
      writeFieldValue("Outside arc", value.getCapOutArcFlag());
      writeFieldValue("Cap line", value.getCapLineFlag());
      writeFieldValue("Custom style", value.getCustomStyleFlag());
      writeFieldValue("Cap color from segment", value.getCapColorFromSegmentFlag());
      writeFieldValue("Construction class", value.getConstructionClassFlag());

      m_nesting--;
    }


    public void writeFieldValue(String name, OdDgMultilinePoint value)
    {
      writeShift();
      Program.DumpStream.WriteLine(name);

      m_nesting++;

      {
        OdGePoint3d point = new OdGePoint3d();

        value.getPoint(point);
        writeFieldValue("Point", point);
      }

      {
        UInt32 i, j = value.getBreaksCount();
        OdDgMultilineBreak break_ = new OdDgMultilineBreak();
        String fieldName = String.Empty;

        writeShift();
        Program.DumpStream.WriteLine(string.Format("Number of breaks: {0}\n", j));

        for (i = 0; i < j; i++)
        {
          //Program.DumpStream.WriteLine(string.Format("{0} Break {1}", fieldName, i));
          //sprintf( fieldName, "Break %d", i );
          fieldName = string.Format("Break {0}", i);
          value.getBreak(i, break_);
          writeFieldValue(fieldName, break_);
        }
      }

      m_nesting--;
    }


    public void writeFieldValue(String name, OdDgMultilineBreak value)
    {
      writeShift();
      Program.DumpStream.WriteLine(name);

      m_nesting++;

      writeFieldValue("Lines mask", value.getLinesMask());
      writeFieldValue("Offset", value.getOffset());
      writeFieldValue("Length", value.getLength());

      {
        String flagValue = String.Empty;

        switch (value.getFlags())
        {
          case OdDgMultilineBreak.Flags.kStandardByDistance: flagValue = "Standard by distance"; break;
          case OdDgMultilineBreak.Flags.kFromJoint: flagValue = "from joint"; break;
          case OdDgMultilineBreak.Flags.kToJoint: flagValue = "to joing"; break;
        }

        writeFieldValue("Flag", flagValue);
      }

      m_nesting--;
    }


    public void writeFieldValue(String name, OdDgMultilineProfile value)
    {
      writeShift();
      Program.DumpStream.WriteLine(name);

      m_nesting++;

      writeFieldValue("Distance", value.getDistance());

      {
        OdDgMultilineSymbology symbology = new OdDgMultilineSymbology();

        value.getSymbology(symbology);
        writeFieldValue("Symbology", symbology);
      }

      m_nesting--;
    }


    public void writeFieldValue(String name, OdDgTextExtendedPropertiesImpl.OdDgTextExtendedProperties_LineSpacingType value)
    {
      writeFieldName(name);

      switch (value)
      {
        case OdDgTextExtendedPropertiesImpl.OdDgTextExtendedProperties_LineSpacingType.kExact: Program.DumpStream.WriteLine("Exact\n"); break;
        case OdDgTextExtendedPropertiesImpl.OdDgTextExtendedProperties_LineSpacingType.kAutomatic: Program.DumpStream.WriteLine("Automatic\n"); break;
        case OdDgTextExtendedPropertiesImpl.OdDgTextExtendedProperties_LineSpacingType.kFromLineTop: Program.DumpStream.WriteLine("FromLineTop\n"); break;
        case OdDgTextExtendedPropertiesImpl.OdDgTextExtendedProperties_LineSpacingType.kAtLeast: Program.DumpStream.WriteLine("AtLeast\n"); break;
      }
    }

    public void writeFieldValue(String name, OdDgRaster.RasterFormat value)
    {
      writeFieldName(name);

      switch (value)
      {
        case OdDgRaster.RasterFormat.kBitmap: Program.DumpStream.WriteLine("Bitmap raster\n"); break;
        case OdDgRaster.RasterFormat.kByteData: Program.DumpStream.WriteLine("Byte data raster\n"); break;
        case OdDgRaster.RasterFormat.kBinaryRLE: Program.DumpStream.WriteLine("Binary RLE raster\n"); break;
        case OdDgRaster.RasterFormat.kByteRLE: Program.DumpStream.WriteLine("Byte RLE raster\n"); break;
      }
    }

    public void writeFieldValue(String name, OdDgTagDefinition.Type value)
    {
      writeFieldName(name);

      switch (value)
      {
        case OdDgTagDefinition.Type.kChar: Program.DumpStream.WriteLine("Char\n"); break;
        case OdDgTagDefinition.Type.kInt16: Program.DumpStream.WriteLine("Short int\n"); break;
        case OdDgTagDefinition.Type.kInt32: Program.DumpStream.WriteLine("Long int\n"); break;
        case OdDgTagDefinition.Type.kDouble: Program.DumpStream.WriteLine("Double\n"); break;
        case OdDgTagDefinition.Type.kBinary: Program.DumpStream.WriteLine("Binary\n"); break;
      }
    }


    public void writeFieldValue(String name, TextDirection value)
    {
      writeFieldName(name);

      switch (value)
      {
        case TextDirection.kHorizontal: Program.DumpStream.WriteLine("Horizontal\n"); break;
        case TextDirection.kVertical: Program.DumpStream.WriteLine("Vertical\n"); break;
        case TextDirection.kJapanese: Program.DumpStream.WriteLine("Japanese\n"); break;
        case TextDirection.kRightToLeft: Program.DumpStream.WriteLine("Right-to-left\n"); break;
      }
    }


    public void writeFieldValue(String name, OdGeMatrix3d value)
    {
      writeFieldName(name);

      Program.DumpStream.WriteLine(string.Format("{0}; {1}; {2}; {3}; {4}; {5}; {6}; {7}; {8}\n",
                value.GetItem(0, 0), value.GetItem(1, 0), value.GetItem(2, 0),
                value.GetItem(0, 1), value.GetItem(1, 1), value.GetItem(2, 1),
                value.GetItem(0, 2), value.GetItem(1, 2), value.GetItem(2, 2)));
    }


    public void writeFieldValue(String name, OdGsDCRect value)
    {
      writeFieldName(name);

      Program.DumpStream.WriteLine(string.Format("( {0:X}; {1:X} ) - ( {2:X}; {3:X} )\n", value.m_min.x, value.m_min.y, value.m_max.x, value.m_max.y));
    }


    public void writeFieldValue(String name, OdDgElementId value)
    {
      writeFieldName(name);

      Program.DumpStream.WriteLine(string.Format("{0:X}\n", value.getHandle()));
    }

    public void writeFieldValue(String name, Object val)
    {
      switch (val.GetType().ToString())
      {
        case "System.Bool":
          writeFieldValue(name, (bool)val);
          break;
        case "System.Byte":
          writeFieldValue(name, (byte)val);
          break;
        case "System.Int16":
          writeFieldValue(name, (System.Int16)val);
          break;
        case "System.Int32":
          writeFieldValue(name, (Int32)val);
          break;
        case "System.Int64":
          writeFieldValue(name, (Int64)val);
          break;
        case "System.Double":
          writeFieldValue(name, (Double)val);
          break;
        case "System.String":
          writeFieldValue(name, (System.String)val);
          break;
      }
    }

    public void writeFieldValue(String name, OdGeExtents2d value)
    {
      writeFieldName(name);

      OdGePoint2d min = value.minPoint(), max = value.maxPoint();

      Program.DumpStream.WriteLine(string.Format("Low point: {0}; {1};  High point: {2}; {3}\n",
        min.x, min.y,
        max.x, max.y
        ));
    }

    public void writeFieldValue(String name, OdGeExtents3d value)
    {
      writeFieldName(name);

      OdGePoint3d min = value.minPoint(), max = value.maxPoint();

      Program.DumpStream.WriteLine("Low point: {0}; {1}; {2};  High point: {3}; {4}; {5}\n",
        min.x, min.y, min.z,
        max.x, max.y, max.z
        );
    }

    public void writeFieldName(String fieldName)
    {
      writeShift();
      Program.DumpStream.WriteLine(fieldName);
    }

    public void writeFieldValue(String name, OdDgDimTool.TerminatorType iType)
    {
      switch (iType)
      {
        case OdDgDimTool.TerminatorType.kTtNone:
          {
            writeFieldValue(name, (("kTtNone")));
          }
          break;

        case OdDgDimTool.TerminatorType.kTtArrow:
          {
            writeFieldValue(name, (("kTtArrow")));
          }
          break;

        case OdDgDimTool.TerminatorType.kTtStroke:
          {
            writeFieldValue(name, (("kTtStroke")));
          }
          break;

        case OdDgDimTool.TerminatorType.kTtCircle:
          {
            writeFieldValue(name, (("kTtCircle")));
          }
          break;

        case OdDgDimTool.TerminatorType.kTtFilledCircle:
          {
            writeFieldValue(name, (("kTtFilledCircle")));
          }
          break;
      }
    }

    public void writeFieldValue(String name, OdDgDimTool.TextType iType)
    {
      switch (iType)
      {
        case OdDgDimTool.TextType.kStandard:
          {
            writeFieldValue(name, (("kStandard")));
          }
          break;

        case OdDgDimTool.TextType.kVertical:
          {
            writeFieldValue(name, (("kVertical")));
          }
          break;

        case OdDgDimTool.TextType.kMixed:
          {
            writeFieldValue(name, (("kMixed")));
          }
          break;
      }
    }

    public void writeFieldValue(String name, OdDgDimTool.CustomSymbol iSymbol)
    {
      switch (iSymbol)
      {
        case OdDgDimTool.CustomSymbol.kCsNone:
          {
            writeFieldValue(name, (("kCsNone")));
          }
          break;

        case OdDgDimTool.CustomSymbol.kCsDiameter:
          {
            writeFieldValue(name, (("kCsDiameter")));
          }
          break;

        case OdDgDimTool.CustomSymbol.kCsRadius:
          {
            writeFieldValue(name, (("kCsRadius")));
          }
          break;

        case OdDgDimTool.CustomSymbol.kCsSquare:
          {
            writeFieldValue(name, (("kCsSquare")));
          }
          break;

        case OdDgDimTool.CustomSymbol.kCsSR:
          {
            writeFieldValue(name, (("kCsSR")));
          }
          break;

        case OdDgDimTool.CustomSymbol.kCsSDiameter:
          {
            writeFieldValue(name, (("kCsSDiameter")));
          }
          break;
      }
    }

    public void writeFieldValue(String name, OdDgDimTool.Leader iLeader)
    {
      switch (iLeader)
      {
        case OdDgDimTool.Leader.kRadius:
          {
            writeFieldValue(name, (("kRadius")));
          }
          break;

        case OdDgDimTool.Leader.kRadiusExt1:
          {
            writeFieldValue(name, (("kRadiusExt1")));
          }
          break;

        case OdDgDimTool.Leader.kRadiusExt2:
          {
            writeFieldValue(name, (("kRadiusExt2")));
          }
          break;

        case OdDgDimTool.Leader.kDiameter:
          {
            writeFieldValue(name, (("kDiameter")));
          }
          break;
      }
    }

    public void writeFieldValue(String name, OdDgDimension pElement)
    {
      switch (pElement.getDimensionType())
      {
        case OdDgDimension.ToolType.kToolTypeSizeArrow:
          {
            OdDgDimSizeArrow pDimSize = (OdDgDimSizeArrow)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeSizeStroke:
          {
            OdDgDimSizeStroke pDimSize = (OdDgDimSizeStroke)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeLocateSingle:
          {
            OdDgDimSingleLocation pDimSize = (OdDgDimSingleLocation)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeLocateStacked:
          {
            OdDgDimStackedLocation pDimSize = (OdDgDimStackedLocation)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeCustomLinear:
          {
            OdDgDimCustomLinear pDimSize = (OdDgDimCustomLinear)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeAngleSize:
          {
            OdDgDimAngleSize pDimSize = (OdDgDimAngleSize)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeAngleLines:
          {
            OdDgDimAngleLines pDimSize = (OdDgDimAngleLines)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeAngleLocation:
          {
            OdDgDimAngleLocation pDimSize = (OdDgDimAngleLocation)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeArcLocation:
          {
            OdDgDimArcLocation pDimSize = (OdDgDimArcLocation)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeAngleAxisX:
          {
            OdDgDimAngleAxisX pDimSize = (OdDgDimAngleAxisX)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeAngleAxisY:
          {
            OdDgDimAngleAxisY pDimSize = (OdDgDimAngleAxisY)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeArcSize:
          {
            OdDgDimArcSize pDimSize = (OdDgDimArcSize)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeRadius:
          {
            OdDgDimRadius pDimSize = (OdDgDimRadius)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeRadiusExtended:
          {
            OdDgDimRadiusExtended pDimSize = (OdDgDimRadiusExtended)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeDiameter:
          {
            OdDgDimDiameter pDimSize = (OdDgDimDiameter)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeDiameterExtended:
          {
            OdDgDimDiameterExtended pDimSize = (OdDgDimDiameterExtended)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeDiameterPara:
          {
            OdDgDimDiameterParallel pDimSize = (OdDgDimDiameterParallel)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeDiameterPerp:
          {
            OdDgDimDiameterPerpendicular pDimSize = (OdDgDimDiameterPerpendicular)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeOrdinate:
          {
            OdDgDimOrdinate pDimSize = (OdDgDimOrdinate)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;

        case OdDgDimension.ToolType.kToolTypeCenter:
          {
            OdDgDimCenter pDimSize = (OdDgDimCenter)pElement;

            if (null != pDimSize)
              writeFieldValue(name, pDimSize);
          }
          break;
      }
    }

    public void writeFieldValue(String name, OdDgDimSizeArrow pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  FirstTerminator", pElement.getFirstTerminator());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  JointTerminator", pElement.getJointTerminator());
      writeFieldValue("  TextType", pElement.getTextType());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }

    public void writeFieldValue(String name, OdDgDimSizeStroke pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  FirstTerminator", pElement.getFirstTerminator());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  JointTerminator", pElement.getJointTerminator());
      writeFieldValue("  TextType", pElement.getTextType());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }

    public void writeFieldValue(String name, OdDgDimSingleLocation pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  FirstTerminator", pElement.getFirstTerminator());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  JointTerminator", pElement.getJointTerminator());
      writeFieldValue("  TextType", pElement.getTextType());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }

    public void writeFieldValue(String name, OdDgDimStackedLocation pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  FirstTerminator", pElement.getFirstTerminator());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  JointTerminator", pElement.getJointTerminator());
      writeFieldValue("  TextType", pElement.getTextType());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }

    public void writeFieldValue(String name, OdDgDimCustomLinear pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  FirstTerminator", pElement.getFirstTerminator());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  JointTerminator", pElement.getJointTerminator());
      writeFieldValue("  TextType", pElement.getTextType());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }

    public void writeFieldValue(String name, OdDgDimAngleSize pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }
    public void writeFieldValue(String name, OdDgDimAngleLines pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }
    public void writeFieldValue(String name, OdDgDimDiameterParallel pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }
    public void writeFieldValue(String name, OdDgDimDiameterPerpendicular pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }

    public void writeFieldValue(String name, OdDgDimAngleLocation pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  FirstTerminator", pElement.getFirstTerminator());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }
    public void writeFieldValue(String name, OdDgDimArcLocation pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  FirstTerminator", pElement.getFirstTerminator());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }
    public void writeFieldValue(String name, OdDgDimAngleAxisX pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  FirstTerminator", pElement.getFirstTerminator());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }
    public void writeFieldValue(String name, OdDgDimAngleAxisY pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  FirstTerminator", pElement.getFirstTerminator());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }

    public void writeFieldValue(String name, OdDgDimRadius pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  CenterMarkFlag", pElement.getCenterMarkFlag());
      writeFieldValue("  CenterMarkLeftExtendFlag", pElement.getCenterMarkLeftExtendFlag());
      writeFieldValue("  CenterMarkRightExtendFlag", pElement.getCenterMarkRightExtendFlag());
      writeFieldValue("  CenterMarkTopExtendFlag", pElement.getCenterMarkTopExtendFlag());
      writeFieldValue("  CenterMarkBottomExtendFlag", pElement.getCenterMarkBottomExtendFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
      writeFieldValue("  Leader", pElement.getLeader());
    }
    public void writeFieldValue(String name, OdDgDimRadiusExtended pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  CenterMarkFlag", pElement.getCenterMarkFlag());
      writeFieldValue("  CenterMarkLeftExtendFlag", pElement.getCenterMarkLeftExtendFlag());
      writeFieldValue("  CenterMarkRightExtendFlag", pElement.getCenterMarkRightExtendFlag());
      writeFieldValue("  CenterMarkTopExtendFlag", pElement.getCenterMarkTopExtendFlag());
      writeFieldValue("  CenterMarkBottomExtendFlag", pElement.getCenterMarkBottomExtendFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
      writeFieldValue("  Leader", pElement.getLeader());
    }
    public void writeFieldValue(String name, OdDgDimDiameter pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  CenterMarkFlag", pElement.getCenterMarkFlag());
      writeFieldValue("  CenterMarkLeftExtendFlag", pElement.getCenterMarkLeftExtendFlag());
      writeFieldValue("  CenterMarkRightExtendFlag", pElement.getCenterMarkRightExtendFlag());
      writeFieldValue("  CenterMarkTopExtendFlag", pElement.getCenterMarkTopExtendFlag());
      writeFieldValue("  CenterMarkBottomExtendFlag", pElement.getCenterMarkBottomExtendFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
      writeFieldValue("  Leader", pElement.getLeader());
    }
    public void writeFieldValue(String name, OdDgDimDiameterExtended pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  CenterMarkFlag", pElement.getCenterMarkFlag());
      writeFieldValue("  CenterMarkLeftExtendFlag", pElement.getCenterMarkLeftExtendFlag());
      writeFieldValue("  CenterMarkRightExtendFlag", pElement.getCenterMarkRightExtendFlag());
      writeFieldValue("  CenterMarkTopExtendFlag", pElement.getCenterMarkTopExtendFlag());
      writeFieldValue("  CenterMarkBottomExtendFlag", pElement.getCenterMarkBottomExtendFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
      writeFieldValue("  Leader", pElement.getLeader());
    }

    public void writeFieldValue(String name, OdDgDimArcSize pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  LeftExtLinesPresentFlag", pElement.getLeftExtLinesPresentFlag());
      writeFieldValue("  RightExtLinesPresentFlag", pElement.getRightExtLinesPresentFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  ChordAlignFlag", pElement.getChordAlignFlag());
      writeFieldValue("  LeftTerminator", pElement.getLeftTerminator());
      writeFieldValue("  RightTerminator", pElement.getRightTerminator());
      writeFieldValue("  Prefix", pElement.getPrefix());
      writeFieldValue("  Suffix", pElement.getSuffix());
    }

    public void writeFieldValue(String name, OdDgDimOrdinate pElement)
    {
      writeFieldName(name);
      writeFieldValue("  StackExtLinesFlag", pElement.getStackExtLinesFlag());
      writeFieldValue("  ArcSymbolFlag", pElement.getArcSymbolFlag());
      writeFieldValue("  DecrementInReverseDirectionFlag", pElement.getDecrementInReverseDirectionFlag());
      writeFieldValue("  FreeLocationOfTxtFlag", pElement.getFreeLocationOfTxtFlag());
      writeFieldValue("  Datum value", pElement.getDatumValue());
    }

    public void writeFieldValue(String name, OdDgDimCenter pElement)
    {
      writeFieldName(name);
      writeFieldValue("  CenterMarkLeftExtendFlag", pElement.getCenterMarkLeftExtendFlag());
      writeFieldValue("  CenterMarkRightExtendFlag", pElement.getCenterMarkRightExtendFlag());
      writeFieldValue("  CenterMarkTopExtendFlag", pElement.getCenterMarkTopExtendFlag());
      writeFieldValue("  CenterMarkBottomExtendFlag", pElement.getCenterMarkBottomExtendFlag());
    }

    public void writeFieldValue(String name, OdDgSurface.Type value)
    {
      switch (value)
      {
        case OdDgSurface.Type.kSurfaceProjection: Program.DumpStream.WriteLine("Projection"); break;
        case OdDgSurface.Type.kSurfaceBoundedPlane: Program.DumpStream.WriteLine("Bounded plane"); break;
        case OdDgSurface.Type.kSurfaceUnboundedPlane: Program.DumpStream.WriteLine("Unbounded plane"); break;
        case OdDgSurface.Type.kSurfaceRight_CIRCULAR_Cylinder: Program.DumpStream.WriteLine("Right circular cylinder"); break;
        case OdDgSurface.Type.kSurfaceRight_CIRCULAR_Cone: Program.DumpStream.WriteLine("Right circular cone"); break;
        case OdDgSurface.Type.kSurfaceTabulatedCylinder: Program.DumpStream.WriteLine("Tabulated cylinder"); break;
        case OdDgSurface.Type.kSurfaceTabulatedCone: Program.DumpStream.WriteLine("Tabulated cone"); break;
        case OdDgSurface.Type.kSurfaceConvolute: Program.DumpStream.WriteLine("Convolute"); break;
        case OdDgSurface.Type.kSurfaceRevolution: Program.DumpStream.WriteLine("Revolution"); break;
        case OdDgSurface.Type.kSurfaceWarped: Program.DumpStream.WriteLine("Warped"); break;
      }
    }

    public void writeFieldValue(String name, OdDgSolid.Type value)
    {
      switch (value)
      {
        case OdDgSolid.Type.kSolidProjection: Program.DumpStream.WriteLine("Projection"); break;
        case OdDgSolid.Type.kSolidRevolution: Program.DumpStream.WriteLine("Revolution"); break;
        case OdDgSolid.Type.kSolidBoundary: Program.DumpStream.WriteLine("Boundary"); break;
      }
    }
    public OdRxObject getObject() { return m_object; }
    //it is common for all dumpers
    public static int m_nesting;

    private OdRxObject m_object;
    //it is common for all dumpers
    private static ArrayList m_nestedTypes = new ArrayList();
  }

}