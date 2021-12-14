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
  class OdDgLineString3d_Dumper : OdRxObject_Dumper
  {
    public void writeLinkagesInfoLS3(OdRxObject pObject)
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
    public void writeElementInfoLS3(OdRxObject pObject)
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
      writeFieldValue("Is Locked", element.getLockedFlag());

      m_nesting--;
    }


    public void startDescriptionLS3(OdRxObject pObject)
    {
      writeShift();
      //OdRxObject m_object = pObject;
      //write it name
      String className;
      {
        OdRxObject_Dumper dumper = GetProperType(pObject);

        //OdRxObject_Dumper dumper = (OdRxObject_Dumper)pObject;

        className = dumper.getClassName();
        Program.DumpStream.WriteLine(className);
      }

      //add the path
      //{
      //    Program.DumpStream.WriteLine(" Path: */");

      //    int i, j = m_nestedTypes.Count;

      //    for (i = 0; i < j; i++)
      //    {
      //        Program.DumpStream.WriteLine(m_nestedTypes[i].ToString());
      //    }
      //}

      //final actions
      Program.DumpStream.WriteLine();
      //m_nestedTypes.Add(className);
      m_nesting++;

      //dump specific information
      writeLinkagesInfo(pObject);//(m_object);
                                 //writeLinkagesInfoLS3(pObject);
      writeElementInfo(pObject);//(m_object); // - linkagesInfo + ElementInfo = failure
                                //writeElementInfoLS3(pObject);//(m_object); // - linkagesInfo + ElementInfo = failure
                                //writeGraphicsElementInfoLS3(pObject);//(m_object);
      writeGraphicsElementInfo(pObject);//(m_object); // - failure
    }

    public void finishDescriptionLS3()
    {
      m_nesting--;

      writeShift();
      //Program.DumpStream.WriteLine(m_nestedTypes[m_nestedTypes.Count - 1]);

      //m_nestedTypes.RemoveAt(m_nestedTypes.Count - 1);
    }

    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      OdDgLineString3d element = (OdDgLineString3d)pObject;
      uint i, j = element.getVerticesCount();
      OdGePoint3d point = new OdGePoint3d();
      startDescription(element);
      writeShift();
      Program.DumpStream.WriteLine("Number of vertices: {0}\n", j);

      for (i = 0; i < j; i++)
      {
        point = element.getVertexAt(i);
        writeFieldValue(string.Format("Vertex {0}", i), point);
      }
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }
    public override String getClassName()
    {
      return "OdDgLineString3d";
    }
  }

}