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
  class OdDgDatabase_Dumper : OdRxObject_Dumper
  {
    public override void dump(OdRxObject pObject)
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();

      OdDgDatabase element = (OdDgDatabase)pObject;
      dumpSummaryInfo(element);
      startDescription(element);
      writeFieldValue("Filename", element.getFilename());
      writeFieldValue("Original version", element.getOriginalFileVersion());
      writeFieldValue("Major version", element.getMajorVersion());
      writeFieldValue("Minor version", element.getMinorVersion());
      writeFieldValue("Control", element.getControlFlags());
      writeFieldValue("Control 1", element.getControl1Flags());
      writeFieldValue("Fbfdcn", element.getLockFlags());
      writeFieldValue("Ext locks", element.getExtLockFlags());
      writeFieldValue("Active angle", element.getActAngle());
      writeFieldValue("Angle round", element.getAngleRnd());
      writeFieldValue("X active scale", element.getXActScale());
      writeFieldValue("Y active scale", element.getYActScale());
      writeFieldValue("Z active scale", element.getZActScale());
      writeFieldValue("Round scale", element.getRoundScale());
      writeFieldValue("Azumuth", element.getAzimuth());
      writeFieldValue("Low", element.getExtMin());
      writeFieldValue("High", element.getExtMax());
      writeFieldValue("Active level", element.getActiveLevelEntryId());
      writeFieldValue("Active line style", element.getActiveLineStyleEntryId());
      writeFieldValue("Active line weight", element.getActiveLineWeight());
      writeFieldValue("Active color index", element.getActiveColorIndex());
      writeFieldValue("Fill color", element.getFillColorIndex());
      writeFieldValue("Active props", element.getActiveProps());
      writeFieldValue("Line style", element.getLineStyle());
      writeFieldValue("Line style scale", element.getLineStyleScale());
      writeFieldValueHex("Multiline flags", element.getMultiLineFlags());
      writeFieldValue("Text style ID", element.getActiveTextStyleEntryId());
      writeFieldValue("Text scale lock", element.getTextScaleLock());
      writeFieldValue("Active view group ID", element.getActiveViewGroupId().getHandle().ascii());
      writeFieldValue("Active model ID", element.getActiveModelId().getHandle().ascii());
      writeFieldValue("Angle format", element.getAngleFormat());
      writeFieldValue("Angle readout precision", element.getAngleReadoutPrec());
      writeFieldValue("Tentative mode", element.getTentativeMode());
      writeFieldValue("Tentative sub-mode", element.getTentativeSubMode());
      writeFieldValue("Keypoint divident", element.getKeyPointDividend());
      writeFieldValue("Default snap mode", element.getDefaultSnapMode());
      writeFieldValue("System class", element.getSystemClass());
      writeFieldValueHex("DMRSF flag", element.getDMRSFlag());
      writeFieldValue("DMRS linkage generation mode", element.getDMRSLinkageGenerationMode());
      writeFieldValueHex("Auto dimenstions flag", element.getAutoDimFlags());
      writeFieldValueHex("Associative lock mask", element.getAssocLockMask());
      writeFieldValue("Active cell", element.getActiveCell());
      writeFieldValue("Active term cell", element.getActiveTermCell());
      writeFieldValue("Active term scale", element.getActiveTermScale());
      writeFieldValue("Active pattern cell", element.getActivePatternCell());
      writeFieldValue("Active pattern scale", element.getActivePatternScale());
      writeFieldValue("Active pattern angle", element.getActivePatternAngle());
      writeFieldValue("Active pattern angle 2", element.getActivePatternAngle2());
      writeFieldValue("Active pattern row spacing", element.getActivePatternRowSpacing());
      writeFieldValue("Active pattern column spacing", element.getActivePatternColumnSpacing());
      writeFieldValue("Pattern tolerance", element.getPatternTolerance());
      writeFieldValue("Active point type", element.getActivePointType());
      writeFieldValue("Active point symbol", element.getActivePointSymbol());
      writeFieldValue("Active point cell", element.getActivePointCell());
      writeFieldValue("Area pattern angle", element.getAreaPatternAngle());
      writeFieldValue("Area pattern row spacing", element.getAreaPatternRowSpacing());
      writeFieldValue("Area pattern column spacing", element.getAreaPatternColumnSpacing());
      writeFieldValue("Reserved cell", element.getReservedCell());
      writeFieldValue("Z range 2D low", element.getZRange2dLow());
      writeFieldValue("Z range 2D high", element.getZRange2dHigh());
      writeFieldValue("Stream delta", element.getStreamDelta());
      writeFieldValue("Stream tolerance", element.getStreamTolerance());
      writeFieldValue("Angle tolerance", element.getAngleTolerance());
      writeFieldValue("Area tolerance", element.getAreaTolerance());
      writeFieldValue("Highlight color", element.getHighlightColorIndex());
      writeFieldValue("XOR color", element.getXorColorIndex());
      writeFieldValue("Axis lock angle", element.getAxisLockAngle());
      writeFieldValue("Axis lock origin", element.getAxisLockOrigin());
      writeFieldValue("Chamfer distance 1", element.getChamferDist1());
      writeFieldValue("Chamfer distance 2", element.getChamferDist2());
      writeFieldValue("Autochain tolerance", element.getAutochainTolerance());
      writeFieldValue("Consline distance", element.getConslineDistance());
      writeFieldValue("Arc radius", element.getArcRadius());
      writeFieldValue("Arc length", element.getArcLength());
      writeFieldValue("Cone radius 1", element.getConeRadius1());
      writeFieldValue("Cone radius 2", element.getConeRadius2());
      writeFieldValue("Polygon radius", element.getPolygonRadius());
      writeFieldValue("Surrev angle", element.getSurrevAngle());
      writeFieldValue("Extend distance", element.getExtendDistance());
      writeFieldValue("Fillet radius", element.getFilletRadius());
      writeFieldValue("Coppar distance", element.getCopparDistance());
      writeFieldValue("Array row distance", element.getArrayRowDistance());
      writeFieldValue("Array column distance", element.getArrayColumnDistance());
      writeFieldValue("Array fill angle", element.getArrayFillAngle());
      writeFieldValue("Point distance", element.getPointDistance());
      writeFieldValue("Polygon edges", element.getPolygonEdges());
      writeFieldValue("Points between", element.getPointsBetween());
      writeFieldValue("Array num of items", element.getArrayNumItems());
      writeFieldValue("Array num of rows", element.getArrayNumRows());
      writeFieldValue("Array num of columns", element.getArrayNumCols());
      writeFieldValue("Array rotate", element.getArrayRotate());
      writeFieldValue("B-spline order", element.getBSplineOrder());
      writeFieldValue("Display attribute type", element.getDispAttrType());
      //  writeFieldValueHex( "Render flags", element.getRenderFlags() );
      writeFieldValue("Latitude", element.getLatitude());
      writeFieldValue("Longitude", element.getLongitude());
      writeFieldValue("Solar time", element.getSolarTime());
      writeFieldValue("Solar year", element.getSolarYear());
      writeFieldValue("GMT offset", element.getGMTOffset());
      writeFieldValue("Solar direction", element.getSolarDirection());
      writeFieldValue("Solar vector override", element.getSolarVectorOverride());
      writeFieldValue("Solar intensity", element.getSolarIntensity());
      writeFieldValue("Ambient intensity", element.getAmbientIntensity());
      writeFieldValue("Flash intensity", element.getFlashIntensity());
      writeFieldValue("Near depth density", element.getNearDepthDensity());
      writeFieldValue("Far depth density", element.getFarDepthDensity());
      writeFieldValue("Near depth distance", element.getNearDepthDistance());
      writeFieldValue("Haze color", element.getHazeColor());
      writeFieldValue("Shadow tolerance", element.getShadowTolerance());
      writeFieldValue("Stroke tolerance", element.getStrokeTolerance());
      writeFieldValue("Max polygon size", element.getMaxPolygonSize());
      writeFieldValue("Arc minimum", element.getArcMinimum());
      writeFieldValue("Exact Hline accuracy", element.getExactHLineAccuracy());
      writeFieldValue("Exact Hline tolerance", element.getExactHLineTolerance());
      writeFieldValue("Selection Highlight override", element.getSelectionHighlightOverride());
      writeFieldValue("Selection Highlight color", element.getSelectionHighlightColor());
      writeFieldValue("Cell filename", element.getCellFileName());
      writeFieldValue("Background file", element.getBackgroundFile());
      //writeFieldValue( "Default model is 3D", element.getDefaultModelIs3D() );
      //writeFieldValue( "Version", element.getVersion() );
      //writeFieldValue( "Sub version", element.getSubVersion() );
      //writeFieldValue( "Format", element.getFormat() );
      //writeFieldValue( "Highest model ID", element.getHighestModelID() );
      writeFieldValue("Handseed", element.getHandseed().ToString());
      writeFieldValue("Last saved time", element.getLastSaveTime());
      //writeFieldValue( "Next graphics group", element.getNextGraphicGroup() );
      //writeFieldValue( "Next text node", element.getNextTextNode() );
      //writeFieldValue( "Original format", element.getOriginalFormat() );
      //writeFieldValue( "Number of model specific digital signatures", element.getModelSpecificDigitalSignatures() );
      //writeFieldValue( "Number of file-wide digital signatures", element.getFileWideDigitalSignatures() );
      //writeFieldValue( "Primary application ID", element.getPrimaryApplicationID() );
      writeFieldValue("Is persistent", element.isPersistent());

      OdDgElementId modelId = element.getDefaultModelId();
      if (!modelId.isNull())
      {
        OdDgModel pModel = OdDgModel.cast(modelId.safeOpenObject());
        if (null != pModel)
        {
          writeFieldValue("Default Model Name", pModel.getName());
        }
      }

      //get all models
      {
        //writeShift();
        //Program.DumpStream.WriteLine("Number of models in the database: %I32u\n", number );

        OdDgModelTable pModelTable = element.getModelTable();
        if (null != pModelTable)
        {
          OdDgElementIterator pIter = pModelTable.createIterator();
          for (; !pIter.done(); pIter.step())
          {
            OdDgModel pModel = OdDgModel.cast(pIter.item().openObject());
            if (null != pModel)
            {
              writeShift();
              Program.DumpStream.WriteLine("Model #{0}\n", pModel.getEntryId());
              OdDgModel_Dumper pModelDumper = (OdDgModel_Dumper)GetProperType(pModel);//new OdDgModel_Dumper();//(OdDgModel_Dumper)pModel;

              pModelDumper.dump(pModel);
            }
          }
        }
      }

      //look through the tables
      {
        OdRxObject curObject = null;
        OdRxObject_Dumper dumper;

        //table of levels
        curObject = element.getLevelTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of level filters
        curObject = element.getLevelFilterTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of fonts
        curObject = element.getFontTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        OdDgTextStyleTable pTextStyleTbl = element.getTextStyleTable();
        if (null != pTextStyleTbl)
        {
          //Program.DumpStream.WriteLine(">>>>> Dumps Default Text Style\n" );
          Program.DumpStream.WriteLine(">>>>> Dumps Default Text Style\n");
          curObject = pTextStyleTbl.getDefaultData();
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of text styles
        curObject = element.getTextStyleTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of dimension styles
        curObject = element.getDimStyleTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of multiline styles
        curObject = element.getMultilineStyleTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of line styles
        curObject = element.getLineStyleTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of registered applications
        curObject = element.getRegAppTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of views
        curObject = element.getViewGroupTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of named views
        curObject = element.getNamedViewTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of shared cell definitions
        curObject = element.getSharedCellDefinitionTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of tag set definitions
        curObject = element.getTagDefinitionSetTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }

        //table of colors
        curObject = element.getColorTable();
        if (null != curObject)
        {
          dumper = GetProperType(curObject);
          dumper.dump(curObject);
        }
        else
        {
          writeShift();
          Program.DumpStream.WriteLine("No color table\n");
        }
      }
      finishDescription();
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }

    public override String getClassName()
    {
      return "OdDgDatabase";
    }

    private void dumpSummaryInfo(OdDgDatabase pDb)
    {
      Program.DumpStream.WriteLine("> Summary Information\n");
      m_nesting++;

      // Summary Information
      OdDgSummaryInformation pSi = TG_Db.oddgGetSummaryInformation(pDb);
      writeFieldValue("Title", pSi.getTitle());
      writeFieldValue("Subject", pSi.getSubject());
      writeFieldValue("Author", pSi.getAuthor());
      writeFieldValue("Keywords", pSi.getKeywords());
      //writeFieldValue( "Comments", pSi.getComments() ); // PIDSI_COMMENTS == 6
      OdDgPropertyValue pPropComments = pSi.getProperty(6);
      if (null != pPropComments)
      {
        writeFieldValue("Comments", pPropComments.getValue());
      }
      writeFieldValue("Template", pSi.getTemplate());
      writeFieldValue("LastSavedBy", pSi.getLastSavedBy());
      writeFieldValue("RevisionNumber", pSi.getRevisionNumber());
      writeFieldValue("ApplicationName", pSi.getApplicationName());
      String timeStr = "";
      OdTimeStamp ts = pSi.getTotalEditingTime();
      if (ts != new OdTimeStamp(OdTimeStamp.InitialValue.kInitZero))
        ts.strftime("%H:%M:%S", ref timeStr);
      else
        timeStr = "0";
      writeFieldValue("TotalEditingTime", timeStr);
      ts = pSi.getLastPrintedTime();
      if (ts != new OdTimeStamp(OdTimeStamp.InitialValue.kInitZero))
        ts.strftime("%H:%M:%S", ref timeStr);
      else
        timeStr = "0";
      writeFieldValue("LastPrintedTime", timeStr);
      timeStr = pSi.getCreatedTime().ToString();
      writeFieldValue("CreatedTime", timeStr);
      timeStr = pSi.getLastSavedTime().ToString();
      writeFieldValue("LastSavedTime", timeStr);
      writeFieldValue("Security", pSi.getSecurity());
      OdBinaryData dibData = new OdBinaryData();
      pSi.getThumbnailBitmap(dibData);
      if (0 != dibData.Count)
      {
        writeFieldValue("ThumbnailBitmap Size", dibData.Count);
      }

      // Document Summary Information
      OdDgDocumentSummaryInformation pDsi = TG_Db.oddgGetDocumentSummaryInformation(pDb);
      writeFieldValue("Category", pDsi.getCategory());
      writeFieldValue("Manager", pDsi.getManager());
      //writeFieldValue( "Company", pDSi.getCompany() ); // PIDDSI_COMPANY == 0x0000000F
      OdDgPropertyValue pPropCompany = pDsi.getProperty(0x0F);
      writeFieldValue("Company", pPropCompany.getValue());

      // UserDefined Properties
      OdRxDictionary pPropDic = pDsi.getCustomProperties();
      if (null != pPropDic)
      {
        OdRxDictionaryIterator pIt = pPropDic.newIterator();
        for (; !pIt.done(); pIt.next())
        {
          String propName = pIt.getKey();
          OdDgPropertyValue pPropValue = OdDgPropertyValue.cast(pIt.getObject()); //(OdDgPropertyValue)pIt.getObject();
          writeFieldValue(propName, pPropValue.getValue().ToString());
        }
      }
      m_nesting--;
    }
  };
}

