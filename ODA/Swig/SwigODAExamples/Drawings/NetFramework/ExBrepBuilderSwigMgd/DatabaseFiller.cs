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
using System.Collections.Generic;

using System.Text;
using Teigha.Core;
using Teigha.TD;
using System.Runtime.InteropServices;

namespace ExBrepBuilderSwigMgd
{
  public class DatabaseFiller
  {
    private OdDbObjectIdArray m_layoutEntities = new OdDbObjectIdArray();
    private double m_textSize;
    private OdGeVector3d m_textOffset;
    private OdGeVector3d m_textLine;

    #region DatabaseFillerUtils
    /************************************************************************/
    /* Add a layout                                                         */
    /************************************************************************/
    private void addLayout(OdDbDatabase pDb)
    {
      /********************************************************************/
      /* Create a new Layout                                              */
      /********************************************************************/
      OdDbObjectId layoutId = pDb.createLayout("ODA Layout");
      OdDbLayout pLayout = (OdDbLayout)layoutId.safeOpenObject();

      /********************************************************************/
      /* Make it current, creating the overall PaperSpace viewport        */
      /********************************************************************/
      pDb.setCurrentLayout(layoutId);

      /********************************************************************/
      /* Open the overall viewport for this layout                        */
      /********************************************************************/
      OdDbViewport pOverallViewport = (OdDbViewport)pLayout.overallVportId().safeOpenObject();

      /********************************************************************/
      /* Get some useful parameters                                       */
      /********************************************************************/
      OdGePoint3d overallCenter = pOverallViewport.centerPoint();

      /********************************************************************/
      /* Note:                                                            */
      /* If a viewport is an overall viewport,                            */
      /* the values returned by width() and height() must be divided by a */
      /* factor of 1.058, and the parameters of setWidth and setHeight()  */
      /* must be multiplied a like factor.                                */
      /********************************************************************/
      const double margin = 0.25;
      double overallWidth = pOverallViewport.width() / 1.058 - 2 * margin;
      double overallHeight = pOverallViewport.height() / 1.058 - 2 * margin;
      OdGePoint3d overallLLCorner = overallCenter -
        new OdGeVector3d(0.5 * overallWidth, 0.5 * overallHeight, 0.0);

      /********************************************************************/
      /* Open the PaperSpace BlockTableRecord for this layout             */
      /********************************************************************/
      OdDbBlockTableRecord pPS = (OdDbBlockTableRecord)pLayout.getBlockTableRecordId().safeOpenObject(OpenMode.kForWrite);

      /********************************************************************/
      /* Create a new viewport, and append it to PaperSpace               */
      /********************************************************************/
      OdDbViewport pViewport = OdDbViewport.createObject();
      pViewport.setDatabaseDefaults(pDb);
      pPS.appendOdDbEntity(pViewport);

      /********************************************************************/
      /* Set some parameters                                              */
      /*                                                                  */
      /* This viewport occupies the upper half of the overall viewport,   */
      /* and displays all objects in model space                          */
      /********************************************************************/

      pViewport.setWidth(overallWidth);
      pViewport.setHeight(overallHeight * 0.5);
      pViewport.setCenterPoint(overallCenter + new OdGeVector3d(0.0, 0.5 * pViewport.height(), 0.0));
      pViewport.setViewCenter(pOverallViewport.viewCenter());
      pViewport.zoomExtents();

      /********************************************************************/
      /* Create viewports for each of the entities that have been         */
      /* pushBacked onto m_layoutEntities                                 */
      /********************************************************************/
      if (m_layoutEntities.Count != 0)
      {
        double widthFactor = 1.0 / m_layoutEntities.Count;
        for (int i = 0; i < m_layoutEntities.Count; ++i)
        {
          OdDbEntity pEnt = (OdDbEntity)m_layoutEntities[i].safeOpenObject();
          OdGeExtents3d entityExtents = new OdGeExtents3d();
          if (pEnt.getGeomExtents(entityExtents) == OdResult.eOk)
          {
            /**************************************************************/
            /* Create a new viewport, and append it to PaperSpace         */
            /**************************************************************/
            pViewport = OdDbViewport.createObject();
            pViewport.setDatabaseDefaults(pDb);
            pPS.appendOdDbEntity(pViewport);

            /**************************************************************/
            /* The viewports are tiled along the bottom of the overall    */
            /* viewport                                                   */
            /**************************************************************/
            pViewport.setWidth(overallWidth * widthFactor);
            pViewport.setHeight(overallHeight * 0.5);
            pViewport.setCenterPoint(overallLLCorner +
              new OdGeVector3d((i + 0.5) * pViewport.width(), 0.5 * pViewport.height(), 0.0));

            /**************************************************************/
            /* The target of the viewport is the midpoint of the entity   */
            /* extents                                                    */
            /**************************************************************/
            OdGePoint3d minPt = entityExtents.minPoint();
            OdGePoint3d maxPt = entityExtents.maxPoint();
            OdGePoint3d viewTarget = new OdGePoint3d();
            viewTarget.x = (minPt.x + maxPt.x) / 2.0;
            viewTarget.y = (minPt.y + maxPt.y) / 2.0;
            viewTarget.z = (minPt.z + maxPt.z) / 2.0;
            pViewport.setViewTarget(viewTarget);

            /**************************************************************/
            /* The viewHeight is the larger of the height as defined by   */
            /* the entityExtents, and the height required to display the  */
            /* width of the entityExtents                                 */
            /**************************************************************/
            double viewHeight = Math.Abs(maxPt.y - minPt.y);
            double viewWidth = Math.Abs(maxPt.x - minPt.x);
            viewHeight = Math.Max(viewHeight, viewWidth * pViewport.height() / pViewport.width());
            pViewport.setViewHeight(viewHeight * 1.05);
          }
        }
      }
      pDb.setTILEMODE(true); // Disable PaperSpace
    }

    /************************************************************************/
    /* Add entity boxes to specified BlockTableRecord                       */
    /************************************************************************/
    private void createEntityBoxes(OdDbObjectId btrId,
                                     OdDbObjectId layerId)
    {
      OdDbDatabase pDb = btrId.database();

      /**********************************************************************/
      /* Open the BlockTableRecord                                          */
      /**********************************************************************/
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)btrId.safeOpenObject(OpenMode.kForWrite);

      OdGePoint3d currentPoint;

      /**********************************************************************/
      /* Create a 2D polyline for each box                                  */
      /**********************************************************************/
      for (int j = 0; j < EntityBoxes.VER_BOXES; j++)
      {
        for (int i = 0; i < EntityBoxes.HOR_BOXES; i++)
        {
          if (!EntityBoxes.isBox(j, i))
            break;

          double wCurBox = EntityBoxes.getWidth(j, i);
          currentPoint = EntityBoxes.getBox(j, i);

          OdDb2dPolyline pPline = OdDb2dPolyline.createObject();
          pPline.setDatabaseDefaults(pDb);

          bBTR.appendOdDbEntity(pPline);

          OdDb2dVertex pVertex;

          pVertex = OdDb2dVertex.createObject();
          pVertex.setDatabaseDefaults(pDb);
          pPline.appendVertex(pVertex);
          OdGePoint3d pos = currentPoint;
          pVertex.setPosition(pos);

          pVertex = OdDb2dVertex.createObject();
          pPline.appendVertex(pVertex);
          pos.x += wCurBox;
          pVertex.setPosition(pos);

          pVertex = OdDb2dVertex.createObject();
          pPline.appendVertex(pVertex);
          pos.y -= EntityBoxes.getHeight();
          pVertex.setPosition(pos);

          pVertex = OdDb2dVertex.createObject();
          pPline.appendVertex(pVertex);
          pos.x -= wCurBox;
          pVertex.setPosition(pos);


          pPline.makeClosed();

          pPline.setColorIndex((UInt16)OdCmEntityColor.ACIcolorMethod.kACIBlue, true);
          pPline.setLayer(layerId, true);
        }
      }
      /**********************************************************************/
      /* 'Zoom' the box array by resizing the active tiled MS viewport      */
      /**********************************************************************/
      OdDbViewportTable pVpTable = (OdDbViewportTable)pDb.getViewportTableId().safeOpenObject(OpenMode.kForWrite);
      OdDbObjectId vpID = pVpTable.getActiveViewportId();
      OdDbViewportTableRecord vPortRec = (OdDbViewportTableRecord)vpID.safeOpenObject(OpenMode.kForWrite);

      OdGePoint3d center = EntityBoxes.getArrayCenter();
      vPortRec.setCenterPoint(center.convert2d());

      OdGeVector3d size = EntityBoxes.getArraySize();
      vPortRec.setHeight(1.05 * Math.Abs(size.y));
      vPortRec.setWidth(1.05 * Math.Abs(size.x));
      vPortRec.setCircleSides(20000);
    }

    /************************************************************************/
    /* Add a Layer to the specified database                                */
    /*                                                                      */
    /* The symbol table and symbol table record are implicitly closed when  */
    /* this function returns.                                               */
    /************************************************************************/
    private OdDbObjectId addLayer(OdDbDatabase pDb, string name, Int16 color, string linetype)
    {
      /**********************************************************************/
      /* Open the layer table                                               */
      /**********************************************************************/
      OdDbLayerTable pLayers = (OdDbLayerTable)pDb.getLayerTableId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Create a layer table record                                        */
      /**********************************************************************/
      OdDbLayerTableRecord pLayer = OdDbLayerTableRecord.createObject();

      /**********************************************************************/
      /* Layer must have a name before adding it to the table.              */
      /**********************************************************************/
      pLayer.setName(name);

      /**********************************************************************/
      /* Set the Color.                                                     */
      /**********************************************************************/
      pLayer.setColorIndex(color);

      /**********************************************************************/
      /* Set the Linetype.                                                  */
      /**********************************************************************/
      OdDbLinetypeTable pLinetypes = (OdDbLinetypeTable)pDb.getLinetypeTableId().safeOpenObject();
      OdDbObjectId linetypeId = pLinetypes.getAt(linetype);
      pLayer.setLinetypeObjectId(linetypeId);

      /**********************************************************************/
      /* Add the record to the table.                                       */
      /**********************************************************************/
      OdDbObjectId layerId = pLayers.add(pLayer);

      return layerId;
    }

    /************************************************************************/
    /* Add a Text Style to the specified database                           */
    /*                                                                      */
    /* The symbol table and symbol table record are implicitly closed when  */
    /* this function returns.                                               */
    /************************************************************************/
    private OdDbObjectId addStyle(OdDbDatabase pDb,
                                    string styleName,
                                    double textSize,
                                    double xScale,
                                    double priorSize,
                                    double obliquing,
                                    string fileName,
                                    bool isShapeFile,
                                    string ttFaceName,
                                    bool bold,
                                    bool italic,
                                    int charset,
                                    int pitchAndFamily)
    {
      OdDbObjectId styleId;

      OdDbTextStyleTable pStyles = (OdDbTextStyleTable)pDb.getTextStyleTableId().safeOpenObject(OpenMode.kForWrite);
      OdDbTextStyleTableRecord pStyle = OdDbTextStyleTableRecord.createObject();

      // Name must be set before a table object is added to a table.  The
      // isShapeFile flag must also be set (if true) before adding the object
      // to the database.
      pStyle.setName(styleName);
      pStyle.setIsShapeFile(isShapeFile);

      // Add the object to the table.
      styleId = pStyles.add(pStyle);

      // Set the remaining properties.
      pStyle.setTextSize(textSize);
      pStyle.setXScale(xScale);
      pStyle.setPriorSize(priorSize);
      pStyle.setObliquingAngle(obliquing);
      pStyle.setFileName(fileName);
      if (isShapeFile)
      {
        pStyle.setPriorSize(22.45);
      }
      if (ttFaceName != "")
      {
        pStyle.setFont(ttFaceName, bold, italic, charset, pitchAndFamily);
      }

      return styleId;
    }


    /************************************************************************/
    /* Add a text entity with the specified attributes to the specified     */
    /* BlockTableRecord                                                     */
    /************************************************************************/
    OdDbObjectId addTextEnt(OdDbBlockTableRecord bBTR, OdGePoint3d position, OdGePoint3d ap,
       string str, double height, TextHorzMode hMode, TextVertMode vMode, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      return addTextEnt(bBTR, position, ap, str, height, hMode, vMode, layerId, styleId, null);
    }

    OdDbObjectId addTextEnt(OdDbBlockTableRecord bBTR,
                                    OdGePoint3d position,
                                    OdGePoint3d ap,
                                    string str,
                                    double height,
                                    TextHorzMode hMode,
                                    TextVertMode vMode,
                                    OdDbObjectId layerId,
                                    OdDbObjectId styleId,
                                    OdDbGroup pGroup)
    {
      /**********************************************************************/
      /* Create the text object                                             */
      /**********************************************************************/
      OdDbText pText = OdDbText.createObject();
      pText.setDatabaseDefaults(bBTR.database());
      OdDbObjectId textId = bBTR.appendOdDbEntity(pText);

      /**********************************************************************/
      /* Add the text to the specified group                                */
      /**********************************************************************/
      if (pGroup != null)
      {
        pGroup.append(textId);
      }

      /**********************************************************************/
      /* Set some properties                                                */
      /**********************************************************************/
      pText.setPosition(position);
      pText.setAlignmentPoint(ap);
      pText.setHeight(height);
      pText.setWidthFactor(1.0);
      pText.setTextString(str);
      pText.setHorizontalMode(hMode);
      pText.setVerticalMode(vMode);

      /**********************************************************************/
      /* Set the text to the specified style                                */
      /**********************************************************************/
      if (!styleId.isNull())
      {
        pText.setTextStyle(styleId);
      }
      /**********************************************************************/
      /* Set the text to the specified layer                                */
      /**********************************************************************/
      if (!layerId.isNull())
      {
        pText.setLayer(layerId, false);
      }

      return textId;
    }
    #endregion DatabaseFillerUtils

    public DatabaseFiller()
    {
      m_textSize = 0.2;
      m_textOffset = new OdGeVector3d(0.5 * m_textSize, -0.5 * m_textSize, 0);
      m_textLine = new OdGeVector3d(0, -1.6 * m_textSize, 0);
    }
    /************************************************************************/
    /* Populate the Database                                                */
    /************************************************************************/

    public void FillDatabase(OdDbDatabase pDb)
    {
      MemoryManager mMan = MemoryManager.GetMemoryManager();
      MemoryTransaction mStartTr = mMan.StartTransaction();
      Console.WriteLine("\nPopulating the database...");

      /**********************************************************************/
      /* Add a Layer                                                        */
      /**********************************************************************/
      OdDbObjectId odaLayer1Id = addLayer(pDb, "Oda Layer 1", (short)OdCmEntityColor.ACIcolorMethod.kACIRed, "CONTINUOUS");

      /**********************************************************************/
      /* Add ModelSpace Entity Boxes                                        */
      /**********************************************************************/
      MemoryTransaction mTr = mMan.StartTransaction();
      createEntityBoxes(pDb.getModelSpaceId(), odaLayer1Id);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a TTF text style                                               */
      /**********************************************************************/
      OdDbObjectId ttfStyleId = addStyle(pDb, "OdaTtfStyle", 0.0, 1.0, 0.2, 0.0,
          "VERDANA.TTF", false, "Verdana", false, false, 0, 34);

      /**********************************************************************/
      /* Add a NURB Surface by BrepBuilder                                  */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addNURBSurface(pDb, 0, 0, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a 3D Solid Box by BrepBuilder                                  */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      add3dSolidBox(pDb, 0, 1, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a Cylinder by BrepBuilder                                      */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addBRepCylinder(pDb, 0, 2, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a Cylinder by BrepBuilder                                      */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addSeamedCylinder(pDb, 0, 3, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Sample get initial data From Solid by BrepBuilderFiller            */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      sample_get_InitialDataFromSolid_ByBrepFiller(pDb, 0, 4, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Sample get initial data From Solid by SampleFillInitialData        */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      sample_get_InitialDataFromSolid_BySampleFillInitialData(pDb, 0, 5, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a RevolvedSurface example 1 by BrepBuilder                     */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addRevolvedSurface1(pDb, 0, 6, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a RevolvedSurface example 2 by BrepBuilder                     */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addRevolvedSurface2(pDb, 0, 7, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);


      /**********************************************************************/
      /* Add a RevolvedSurface example 3 by BrepBuilder                     */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addRevolvedSurface3(pDb, 0, 8, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a RevolvedSurface example 4 by BrepBuilder                     */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addRevolvedSurface4(pDb, 0, 9, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a RevolvedSurface example 5 by BrepBuilder                     */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addRevolvedSurface5(pDb, 1, 0, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a EllipCylinder by BrepBuilder                                 */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addEllipCylinder(pDb, 1, 1, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a EllipCone by BrepBuilder                                     */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addEllipCone(pDb, 1, 2, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a Cone by BrepBuilder                                          */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      bool addCone_useReverse = false;
      bool addCone_cutBeforeApex = false;
      addCone(pDb, 1, 3, odaLayer1Id, ttfStyleId, addCone_useReverse, addCone_cutBeforeApex);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a Torus by BrepBuilder                                         */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      TorusType torusType = TorusType.ttLemon;
      addTorus(pDb, 1, 4, odaLayer1Id, ttfStyleId, torusType);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a TorusQuarter by BrepBuilder                                  */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addTorusQuarter(pDb, 1, 5, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a Sphere by BrepBuilder                                        */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addSphere(pDb, 1, 6, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a HemiSphere by BrepBuilder                                    */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addHemiSphere(pDb, 2, 0, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a Pyramid by BrepBuilder                                       */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addPyramid(pDb, 2, 1, odaLayer1Id, ttfStyleId);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a Plane by BrepBuilder                                         */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      bool addPlane_useReverse = false;
      addPlane(pDb, 2, 2, odaLayer1Id, ttfStyleId, addPlane_useReverse);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a TCoedgeEllipCylinder by BrepBuilder                          */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      double deltaRadius_addTCoedgeEllipCylinder = 1e-3;
      addTCoedgeEllipCylinder(pDb, 2, 3, odaLayer1Id, ttfStyleId, deltaRadius_addTCoedgeEllipCylinder);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a TCoedgeFourthSphere by BrepBuilder                           */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      double deltaRadius_addTCoedgeFourthSphere = 1e-3;
      addTCoedgeFourthSphere(pDb, 2, 4, odaLayer1Id, ttfStyleId, deltaRadius_addTCoedgeFourthSphere);
      mMan.StopTransaction(mTr);

      /**********************************************************************/
      /* Add a Layout                                                       */
      /**********************************************************************/
      mTr = mMan.StartTransaction();
      addLayout(pDb);
      mMan.StopTransaction(mTr);

      mMan.StopTransaction(mStartTr);
    }

    void addNURBSurface(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      /**********************************************************************/
      /* Open the Block Table Record                                        */
      /**********************************************************************/
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      /**********************************************************************/
      /* Add the label                                                      */
      /**********************************************************************/
      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "NURBS surface", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      /********************************************************************/
      /* Create NURBS surface, trimming-loop and holes                    */
      /********************************************************************/
      OdGeNurbSurface nurbSurf = new OdGeNurbSurface();
      OdGeNurbCurve3dArray2d arrLoops = new OdGeNurbCurve3dArray2d();
      OdGeNurbCurve2dArray2d arrLoopsPar = new OdGeNurbCurve2dArray2d();

      NurbData nurbData = new NurbData();
      nurbData.FillNurbData(nurbSurf, ref arrLoops, ref arrLoopsPar);
      OdDbNurbSurface pSurface = OdDbNurbSurface.createObject();

      /********************************************************************/
      /* BrepBuilder initialization                                       */
      /********************************************************************/
      using (OdBrepBuilder brep = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kOpenShell;
        pService.brepBuilder(brep, bbType);
        /********************************************************************/
        /* Create NURBS Surface by BrepBuilder                              */
        /********************************************************************/
        try
        {
          UInt32 complexId = brep.addComplex();
          UInt32 shellId = brep.addShell(complexId);
          UInt32 faceId = brep.addFace(nurbSurf, OdBrepBuilder.EntityDirection.kForward, shellId);

          if (arrLoops.Count != arrLoopsPar.Count)
            throw new OdError("arrLoops.Count == arrLoopsPar.Count");

          for (int i = 0; i < arrLoops.Count; i++)
          {
            OdGeNurbCurve3dArrayPtr currentLoop = arrLoops[i];
            OdGeNurbCurve2dArrayPtr currentParLoop = arrLoopsPar[i];
            if (currentLoop.Count != currentParLoop.Count)
              throw new OdError("currentLoop.Count == currentParLoop.Count");

            UInt32 LoopId = brep.addLoop(faceId);
            for (int j = 0; j < currentLoop.Count; j++)
            {
              UInt32 edgeId = brep.addEdge(currentLoop[j]);
              brep.addCoedge(LoopId, edgeId, OdBrepBuilder.EntityDirection.kForward, currentParLoop[j]);
            }
            brep.finishLoop(LoopId);
          }
          brep.finishFace(faceId);
          brep.finishShell(shellId);
          brep.finishComplex(complexId);
          OdRxObject pModelGeometry;
          pModelGeometry = brep.finish();
          pSurface.setBody(OdRxObject.getCPtr(pModelGeometry).Handle);
        }
        catch (OdError e)
        {
          Console.WriteLine("\n\nException occurred: %ls\n", e.description());
        }
      }
      /********************************************************************/
      /* Free created trimming-loop curves                                */
      /********************************************************************/
      for (int i = 0; i < arrLoops.Count; i++)
      {
        for (int j = 0; j < arrLoops[i].Count; j++)
        {
          arrLoops[i][j].Dispose();
          arrLoopsPar[i][j].Dispose();
        }
      }
      /**********************************************************************/
      /* Add NURB surface to database                                       */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSurface.getGeomExtents(ext);
      OdGeMatrix3d xfm = new OdGeMatrix3d();
      double scaleX = w * 0.7 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.7 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSurface.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSurface.transformBy(xfm);

      pSurface.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSurface);
      /**********************************************************************/
      /* Add the NURB surface                                               */
      /**********************************************************************/
      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSurface.objectId());

      /**********************************************************************/
      /* Add NURB surface to Sortents table                                 */
      /**********************************************************************/
      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void add3dSolidBox(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "3DSolidBox", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      /**********************************************************************/
      /* Create input 3D Solid Box                                          */
      /**********************************************************************/
      OdDb3dSolid input3dSolid = OdDb3dSolid.createObject();

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      input3dSolid.createBox(w * 6.0 / 8.0, h * 6.0 / 8.0, w * 6.0 / 8.0);
      input3dSolid.transformBy(xfm);

      OdBrBrep brep = new OdBrBrep();
      input3dSolid.brep(brep);

      /**********************************************************************/
      /* Fill BrepBuilder by means of OdBrepBuilderFillerModule             */
      /**********************************************************************/

      OdRxObject brepData;
      using (OdBrepBuilder bbuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kOpenShell;
        pService.brepBuilder(bbuilder, bbType);

        OdBrepBuilderFiller BBFiller = new OdBrepBuilderFiller();
        OdResult res = BBFiller.initFrom(bbuilder, brep);
        if (res != OdResult.eOk)
          throw new OdError(res);

        brepData = bbuilder.finish();
      }
      /**********************************************************************/
      /* Get OdDb3dSolid from brep geometry data                            */
      /**********************************************************************/
      OdDb3dSolid output3dSolid = OdDb3dSolid.createObject();
      output3dSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      output3dSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      output3dSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      output3dSolid.transformBy(xfm);

      output3dSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(output3dSolid);
      /**********************************************************************/
      /* Add the 3D Solid                                               */
      /**********************************************************************/
      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(output3dSolid.objectId());

      /**********************************************************************/
      /* Add 3D Solid to Sortents table                                 */
      /**********************************************************************/
      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addBRepCylinder(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Cylinder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      OdRxObject brepData = null;
      using (OdBrepBuilder bbuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kOpenShell;
        pService.brepBuilder(bbuilder, bbType);

        // Create complex
        UInt32 complexId = bbuilder.addComplex();

        // Create shell
        UInt32 shellId = bbuilder.addShell(complexId);

        OdGeInterval intrvlV = new OdGeInterval(-50.0, 50.0);
        OdGeCylinder cylSurf = new OdGeCylinder(50, new OdGePoint3d(50, -100, 250), new OdGeVector3d(0, 0, 1), new OdGeVector3d(0, 1, 0), intrvlV, 0, Globals.Oda2PI);

        OdGePlane top = new OdGePlane(new OdGePoint3d(0, 0, 300), new OdGeVector3d(1, 0, 0), new OdGeVector3d(0, 1, 0));
        OdGePlane bottom = new OdGePlane(new OdGePoint3d(0, 0, 200), new OdGeVector3d(1, 0, 0), new OdGeVector3d(0, 1, 0));

        // Add Faces
        UInt32 cylFaceId = bbuilder.addFace(cylSurf, OdBrepBuilder.EntityDirection.kForward, shellId);
        UInt32 topFaceId = bbuilder.addFace(top, OdBrepBuilder.EntityDirection.kForward, shellId);
        UInt32 bottomFaceId = bbuilder.addFace(bottom, OdBrepBuilder.EntityDirection.kReversed, shellId);

        OdGeEllipArc3d edgeBottom = new OdGeEllipArc3d(new OdGePoint3d(50, -100, 200), OdGeVector3d.kXAxis, -OdGeVector3d.kYAxis, 50, 50, 0, Globals.Oda2PI);
        OdGeEllipArc3d edgeTop = new OdGeEllipArc3d(new OdGePoint3d(50, -100, 300), OdGeVector3d.kXAxis, OdGeVector3d.kYAxis, 50, 50, 0, Globals.Oda2PI);

        UInt32 edgeBottomId = bbuilder.addEdge(edgeBottom);
        UInt32 edgeTopId = bbuilder.addEdge(edgeTop);

        UInt32 loopId_cylTop = bbuilder.addLoop(cylFaceId);
        UInt32 loopId_cylBottom = bbuilder.addLoop(cylFaceId);
        UInt32 loopId_Top = bbuilder.addLoop(topFaceId);
        UInt32 loopId_Bottom = bbuilder.addLoop(bottomFaceId);
        bbuilder.finishFace(cylFaceId);
        bbuilder.finishFace(topFaceId);
        bbuilder.finishFace(bottomFaceId);

        bbuilder.addCoedge(loopId_cylTop, edgeTopId, OdBrepBuilder.EntityDirection.kReversed);
        bbuilder.addCoedge(loopId_cylBottom, edgeBottomId, OdBrepBuilder.EntityDirection.kReversed);
        bbuilder.addCoedge(loopId_Top, edgeTopId, OdBrepBuilder.EntityDirection.kForward);
        bbuilder.addCoedge(loopId_Bottom, edgeBottomId, OdBrepBuilder.EntityDirection.kForward);
        bbuilder.finishLoop(loopId_cylTop);
        bbuilder.finishLoop(loopId_cylBottom);
        bbuilder.finishLoop(loopId_Top);
        bbuilder.finishLoop(loopId_Bottom);

        bbuilder.finishShell(shellId);
        bbuilder.finishComplex(complexId);

        brepData = bbuilder.finish();
      }

      /**********************************************************************/
      /* Get Cylinder from brep geometry data                               */
      /**********************************************************************/
      OdDb3dSolid pRetSurface = OdDb3dSolid.createObject();
      pRetSurface.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pRetSurface.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pRetSurface.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pRetSurface.transformBy(xfm);

      pRetSurface.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pRetSurface);
      /**********************************************************************/
      /* Add the Cylinder                                                   */
      /**********************************************************************/
      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pRetSurface.objectId());

      /**********************************************************************/
      /* Add Cylinder to Sortents table                                     */
      /**********************************************************************/
      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addSeamedCylinder(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Seamed Cylinder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());


      OdRxObject brepData = null;
      using (OdBrepBuilder bbuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kOpenShell;
        pService.brepBuilder(bbuilder, bbType);

        // Create complex
        UInt32 complexId = bbuilder.addComplex();

        // Create shell
        UInt32 shellId = bbuilder.addShell(complexId);

        double radius = 3.15;
        OdGeInterval intrvlV = new OdGeInterval(-5.0, 5.0);
        OdGeCylinder cylSurf = new OdGeCylinder(new OdGeCylinder(radius, new OdGePoint3d(0, 0, 5), new OdGeVector3d(0, 0, 1), new OdGeVector3d(0, 1, 0), intrvlV, 0, Globals.Oda2PI));

        OdGePlane top = new OdGePlane(new OdGePoint3d(0, 0, 10), new OdGeVector3d(1, 0, 0), new OdGeVector3d(0, 1, 0));
        OdGePlane bottom = new OdGePlane(new OdGePoint3d(0, 0, 0), new OdGeVector3d(1, 0, 0), new OdGeVector3d(0, 1, 0));

        // Add Faces
        UInt32 cylFaceId = bbuilder.addFace(cylSurf, OdBrepBuilder.EntityDirection.kForward, shellId);
        UInt32 topFaceId = bbuilder.addFace(top, OdBrepBuilder.EntityDirection.kForward, shellId);
        UInt32 bottomFaceId = bbuilder.addFace(bottom, OdBrepBuilder.EntityDirection.kReversed, shellId);

        OdGeEllipArc3d edgeBottom = new OdGeEllipArc3d(new OdGePoint3d(0, 0, 0), OdGeVector3d.kXAxis, -OdGeVector3d.kYAxis, radius, radius, 0, Globals.Oda2PI);
        OdGeEllipArc3d edgeTop = new OdGeEllipArc3d(new OdGePoint3d(0, 0, 10), OdGeVector3d.kXAxis, OdGeVector3d.kYAxis, radius, radius, 0, Globals.Oda2PI);
        OdGeLineSeg3d edgeSeam = new OdGeLineSeg3d(edgeBottom.evalPoint(0.0), edgeTop.evalPoint(0.0));

        UInt32 edgeBottomId = bbuilder.addEdge(edgeBottom);
        UInt32 edgeTopId = bbuilder.addEdge(edgeTop);
        UInt32 edgeSeamId = bbuilder.addEdge(edgeSeam);

        UInt32 loopId_cylinder = bbuilder.addLoop(cylFaceId);
        UInt32 loopId_Top = bbuilder.addLoop(topFaceId);
        UInt32 loopId_Bottom = bbuilder.addLoop(bottomFaceId);

        bbuilder.finishFace(cylFaceId);
        bbuilder.finishFace(topFaceId);
        bbuilder.finishFace(bottomFaceId);

        bbuilder.addCoedge(loopId_cylinder, edgeTopId, OdBrepBuilder.EntityDirection.kReversed);
        bbuilder.addCoedge(loopId_cylinder, edgeSeamId, OdBrepBuilder.EntityDirection.kReversed);
        bbuilder.addCoedge(loopId_cylinder, edgeBottomId, OdBrepBuilder.EntityDirection.kReversed);
        bbuilder.addCoedge(loopId_cylinder, edgeSeamId, OdBrepBuilder.EntityDirection.kForward);
        bbuilder.addCoedge(loopId_Top, edgeTopId, OdBrepBuilder.EntityDirection.kForward);
        bbuilder.addCoedge(loopId_Bottom, edgeBottomId, OdBrepBuilder.EntityDirection.kForward);
        bbuilder.finishLoop(loopId_cylinder);
        bbuilder.finishLoop(loopId_Top);
        bbuilder.finishLoop(loopId_Bottom);

        bbuilder.finishShell(shellId);
        bbuilder.finishComplex(complexId);

        brepData = bbuilder.finish();
      }

      /**********************************************************************/
      /* Get Seamed Cylinder from brep geometry data                        */
      /**********************************************************************/
      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      /**********************************************************************/
      /* Add the Seamed Cylinder                                            */
      /**********************************************************************/
      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      /**********************************************************************/
      /* Add Seamed Cylinder to Sortents table                              */
      /**********************************************************************/
      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    private static void SupressFinalizeForItems(System.Collections.IEnumerable coll)
    {
      foreach (Object o in coll)
      {
        GC.SuppressFinalize(o);
      }
    }

    void addRevolvedSurface1(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Revolved Surf.1", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      double radiusBottom = 4;
      double radiusTop = 3;
      double height = 2;
      OdGeVector3d rotAxis = new OdGeVector3d(OdGeVector3d.kZAxis);
      OdGeVector3d refAxis = new OdGeVector3d(OdGeVector3d.kXAxis);
      OdGeVector3d yAxis = new OdGeVector3d(rotAxis.crossProduct(refAxis).normalize());
      OdGePoint3d centerBottom = new OdGePoint3d(OdGePoint3d.kOrigin);
      OdGePoint3d centerTop = new OdGePoint3d(centerBottom + rotAxis * height);

      // bottom -> top
      OdGeLineSeg3d line = new OdGeLineSeg3d(centerBottom + refAxis * radiusBottom, centerTop + refAxis * radiusTop);
      OdGeLineSeg3d line2 = new OdGeLineSeg3d(centerBottom - refAxis * radiusBottom, centerTop - refAxis * radiusTop);
      // right -> left
      OdGeCircArc3d arcBottom = new OdGeCircArc3d(centerBottom, rotAxis, refAxis, radiusBottom, 0, Globals.OdaPI);
      OdGeCircArc3d arcTop = new OdGeCircArc3d(centerTop, rotAxis, refAxis, radiusTop, 0, Globals.OdaPI);
      // left -> right
      OdGeLineSeg3d lineBottom = new OdGeLineSeg3d(centerBottom - refAxis * radiusBottom, centerBottom + refAxis * radiusBottom);
      OdGeLineSeg3d lineTop = new OdGeLineSeg3d(centerTop - refAxis * radiusTop, centerTop + refAxis * radiusTop);

      OdGePlane planeBottom = new OdGePlane(centerBottom, -rotAxis);
      OdGePlane planeTop = new OdGePlane(centerTop, rotAxis);
      OdGePlane planeSide = new OdGePlane(centerBottom, -yAxis);

      OdGeMatrix3d surfCoord = new OdGeMatrix3d();
      surfCoord.setCoordSystem(centerBottom, refAxis, yAxis, rotAxis).invert();
      OdGeLineSeg3d genLine = new OdGeLineSeg3d(line);
      genLine.transformBy(surfCoord);
      OdGeRevolvedSurface rotSurf = new OdGeRevolvedSurface(genLine, centerBottom, rotAxis, refAxis);
      rotSurf.setAngles(0, Globals.OdaPI);

      // u --- curve param, v --- rot param
      const int coedgeDegree = 1;

      OdGePoint2dArray pointsArcBottom = new OdGePoint2dArray(2);
      pointsArcBottom.Add(new OdGePoint2d(0.0, 0.0));
      pointsArcBottom.Add(new OdGePoint2d(0.0, Globals.OdaPI));
      SupressFinalizeForItems(pointsArcBottom);
      OdGePoint2dArray pointsLine = new OdGePoint2dArray(2);
      pointsLine.Add(new OdGePoint2d(0.0, 0.0));
      pointsLine.Add(new OdGePoint2d(1.0, 0.0));
      SupressFinalizeForItems(pointsLine);
      OdGePoint2dArray pointsLine2 = new OdGePoint2dArray(2);
      pointsLine2.Add(new OdGePoint2d(0.0, Globals.OdaPI));
      pointsLine2.Add(new OdGePoint2d(1.0, Globals.OdaPI));
      SupressFinalizeForItems(pointsLine2);
      OdGePoint2dArray pointsArcTop = new OdGePoint2dArray(2);
      pointsArcTop.Add(new OdGePoint2d(1.0, 0.0));
      pointsArcTop.Add(new OdGePoint2d(1.0, Globals.OdaPI));
      SupressFinalizeForItems(pointsArcTop);

      OdGeDoubleArray coedgeArrKnotsArcBottom = new OdGeDoubleArray() { 0.0, 0.0, Globals.OdaPI, Globals.OdaPI };
      OdGeDoubleArray coedgeArrKnotsLine = new OdGeDoubleArray() { 0.0, 0.0, 1.0, 1.0 };
      OdGeDoubleArray coedgeArrKnotsLine2 = new OdGeDoubleArray() { 0.0, 0.0, 1.0, 1.0 };
      OdGeDoubleArray coedgeArrKnotsArcTop = new OdGeDoubleArray() { 0.0, 0.0, Globals.OdaPI, Globals.OdaPI };
      if (coedgeArrKnotsArcBottom.Count != 4 ||
        coedgeArrKnotsLine.Count != 4 ||
        coedgeArrKnotsLine2.Count != 4 ||
        coedgeArrKnotsArcTop.Count != 4)
        throw new Exception("Error");

      OdGeKnotVector coedgeKnotsArcBottom = new OdGeKnotVector(coedgeArrKnotsArcBottom);
      OdGeKnotVector coedgeKnotsLine = new OdGeKnotVector(coedgeArrKnotsLine);
      OdGeKnotVector coedgeKnotsLine2 = new OdGeKnotVector(coedgeArrKnotsLine2);
      OdGeKnotVector coedgeKnotsArcTop = new OdGeKnotVector(coedgeArrKnotsArcTop);

      OdGeNurbCurve2d coedgeArcBottom = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArcBottom, pointsArcBottom);
      OdGeNurbCurve2d coedgeLine = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsLine, pointsLine);
      OdGeNurbCurve2d coedgeLine2 = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsLine2, pointsLine2);
      OdGeNurbCurve2d coedgeArcTop = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArcTop, pointsArcTop);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        OdResult res = pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        UInt32 edgeIdLine = brepBuilder.addEdge(line);
        UInt32 edgeIdLine2 = brepBuilder.addEdge(line2);
        UInt32 edgeIdArcBottom = brepBuilder.addEdge(arcBottom);
        UInt32 edgeIdArcTop = brepBuilder.addEdge(arcTop);
        UInt32 edgeIdLineBottom = brepBuilder.addEdge(lineBottom);
        UInt32 edgeIdLineTop = brepBuilder.addEdge(lineTop);

        {
          UInt32 faceId = brepBuilder.addFace(planeBottom, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.addCoedge(loopId, edgeIdLineBottom, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(planeTop, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.addCoedge(loopId, edgeIdLineTop, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(planeSide, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdLineTop, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.addCoedge(loopId, edgeIdLine2, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.addCoedge(loopId, edgeIdLineBottom, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(rotSurf, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kForward, coedgeArcBottom);
          brepBuilder.addCoedge(loopId, edgeIdLine2, OdBrepBuilder.EntityDirection.kForward, coedgeLine2);
          brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder.EntityDirection.kReversed, coedgeArcTop);
          brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder.EntityDirection.kReversed, coedgeLine);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }
      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addRevolvedSurface2(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Revolved Surf.2", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      double cospi4 = 0.5 * Math.Sqrt(2.0);
      double radiusBottom = 4;
      double height = 2;
      OdGeVector3d rotAxis = new OdGeVector3d(OdGeVector3d.kYAxis);
      OdGeVector3d refAxis = new OdGeVector3d(cospi4, 0.0, cospi4);
      OdGeVector3d yAxis = new OdGeVector3d(rotAxis.crossProduct(refAxis).normalize());
      OdGePoint3d centerBottom = new OdGePoint3d(0.0, 0.0, -2.0);
      OdGePoint3d centerTop = new OdGePoint3d(centerBottom + rotAxis * height);

      // bottom -> top
      OdGeLineSeg3d line = new OdGeLineSeg3d(centerBottom + refAxis * radiusBottom, centerTop);
      OdGeLineSeg3d line2 = new OdGeLineSeg3d(centerBottom - refAxis * radiusBottom, centerTop);
      // right -> left
      OdGeCircArc3d arcBottom = new OdGeCircArc3d(centerBottom, rotAxis, refAxis, radiusBottom, 0, Globals.OdaPI);
      // left -> right
      OdGeLineSeg3d lineBottom = new OdGeLineSeg3d(centerBottom - refAxis * radiusBottom, centerBottom + refAxis * radiusBottom);

      OdGePlane planeBottom = new OdGePlane(centerBottom, -rotAxis);
      OdGePlane planeSide = new OdGePlane(centerBottom, -yAxis);

      OdGeMatrix3d surfCoord = new OdGeMatrix3d();
      surfCoord = surfCoord.setCoordSystem(centerBottom, refAxis, yAxis, rotAxis).invert();
      OdGeLineSeg3d genLine = new OdGeLineSeg3d(line);
      genLine.transformBy(surfCoord);
      OdGeRevolvedSurface rotSurf = new OdGeRevolvedSurface(genLine, centerBottom, rotAxis, refAxis);
      rotSurf.setAngles(0, Globals.OdaPI);

      // u --- curve param, v --- rot param
      int coedgeDegree = 1;

      OdGePoint2dArray pointsArcBottom = new OdGePoint2dArray(2);
      pointsArcBottom.Add(new OdGePoint2d(0.0, 0.0));
      pointsArcBottom.Add(new OdGePoint2d(0.0, Globals.OdaPI));
      SupressFinalizeForItems(pointsArcBottom);
      OdGePoint2dArray pointsLine = new OdGePoint2dArray(2);
      pointsLine.Add(new OdGePoint2d(0.0, 0.0));
      pointsLine.Add(new OdGePoint2d(1.0, 0.0));
      SupressFinalizeForItems(pointsLine);
      OdGePoint2dArray pointsLine2 = new OdGePoint2dArray(2);
      pointsLine2.Add(new OdGePoint2d(0.0, Globals.OdaPI));
      pointsLine2.Add(new OdGePoint2d(1.0, Globals.OdaPI));
      SupressFinalizeForItems(pointsLine2);

      OdGeDoubleArray coedgeArrKnotsArcBottom = new OdGeDoubleArray() { 0.0, 0.0, Globals.OdaPI, Globals.OdaPI };
      OdGeDoubleArray coedgeArrKnotsLine = new OdGeDoubleArray() { 0.0, 0.0, 1.0, 1.0 };
      OdGeDoubleArray coedgeArrKnotsLine2 = new OdGeDoubleArray() { 0.0, 0.0, 1.0, 1.0 };
      if (coedgeArrKnotsArcBottom.Count != 4 ||
          coedgeArrKnotsLine.Count != 4 ||
          coedgeArrKnotsLine2.Count != 4)
        throw new Exception("Error");

      OdGeKnotVector coedgeKnotsArcBottom = new OdGeKnotVector(coedgeArrKnotsArcBottom);
      OdGeKnotVector coedgeKnotsLine = new OdGeKnotVector(coedgeArrKnotsLine);
      OdGeKnotVector coedgeKnotsLine2 = new OdGeKnotVector(coedgeArrKnotsLine2);

      OdGeNurbCurve2d coedgeArcBottom = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArcBottom, pointsArcBottom);
      OdGeNurbCurve2d coedgeLine = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsLine, pointsLine);
      OdGeNurbCurve2d coedgeLine2 = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsLine2, pointsLine2);


      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        UInt32 edgeIdLine = brepBuilder.addEdge(line);
        UInt32 edgeIdLine2 = brepBuilder.addEdge(line2);
        UInt32 edgeIdArcBottom = brepBuilder.addEdge(arcBottom);
        UInt32 edgeIdLineBottom = brepBuilder.addEdge(lineBottom);

        {
          UInt32 faceId = brepBuilder.addFace(planeBottom, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.addCoedge(loopId, edgeIdLineBottom, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(planeSide, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdLine2, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.addCoedge(loopId, edgeIdLineBottom, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(rotSurf, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kForward, coedgeArcBottom);
          brepBuilder.addCoedge(loopId, edgeIdLine2, OdBrepBuilder.EntityDirection.kForward, coedgeLine2);
          brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder.EntityDirection.kReversed, coedgeLine);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }


        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addRevolvedSurface3(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Revolved Surf.3", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      double majorRadius = 10.0;
      double minorRadius = 4.0;
      double rotSurfRadius = 2;
      double arcStartAngle = Globals.OdaPI2;
      double arcEndAngle = Globals.OdaPI2 + Globals.OdaPI / 3;
      OdGeVector3d rotAxis = new OdGeVector3d(OdGeVector3d.kZAxis);
      OdGeVector3d refAxis = new OdGeVector3d(OdGeVector3d.kXAxis);
      OdGeVector3d yAxis = new OdGeVector3d(rotAxis.crossProduct(refAxis).normalize());
      OdGePoint3d center = new OdGePoint3d(OdGePoint3d.kOrigin);
      OdGePoint3d arcCenter = new OdGePoint3d(center - refAxis * rotSurfRadius);

      // bottom -> top
      OdGeEllipArc3d arc = new OdGeEllipArc3d(arcCenter, -OdGeVector3d.kZAxis, OdGeVector3d.kXAxis, majorRadius, minorRadius, arcStartAngle, arcEndAngle);

      OdGeCircArc3d arcBottom = new OdGeCircArc3d(center, rotAxis, refAxis, rotSurfRadius, 0, Globals.Oda2PI);

      OdGePlane planeBottom = new OdGePlane(center, -rotAxis);

      OdGeMatrix3d surfCoord = new OdGeMatrix3d();
      surfCoord.setCoordSystem(center, refAxis, yAxis, rotAxis).invert();
      OdGeEllipArc3d genArc = new OdGeEllipArc3d(arc);
      genArc.transformBy(surfCoord);
      OdGeRevolvedSurface rotSurf = new OdGeRevolvedSurface(genArc, center, rotAxis, refAxis);
      rotSurf.setAngles(0, Globals.Oda2PI);

      // u --- curve param, v --- rot param
      const int coedgeDegree = 1;

      OdGePoint2dArray pointsArc = new OdGePoint2dArray(2);
      pointsArc.Add(new OdGePoint2d(arcStartAngle, 0.0));
      pointsArc.Add(new OdGePoint2d(arcEndAngle, 0.0));
      SupressFinalizeForItems(pointsArc);
      OdGePoint2dArray pointsArcBottom = new OdGePoint2dArray(2);
      pointsArcBottom.Add(new OdGePoint2d(arcStartAngle, 0.0));
      pointsArcBottom.Add(new OdGePoint2d(arcStartAngle, Globals.Oda2PI));
      SupressFinalizeForItems(pointsArcBottom);

      OdGeDoubleArray coedgeArrKnotsArc = new OdGeDoubleArray() { arcStartAngle, arcStartAngle, arcEndAngle, arcEndAngle };
      OdGeDoubleArray coedgeArrKnotsArcBottom = new OdGeDoubleArray() { 0.0, 0.0, Globals.Oda2PI, Globals.Oda2PI };
      if (coedgeArrKnotsArcBottom.Count != 4 ||
          coedgeArrKnotsArc.Count != 4)
        throw new Exception("Error");

      OdGeKnotVector coedgeKnotsArc = new OdGeKnotVector(coedgeArrKnotsArc);
      OdGeKnotVector coedgeKnotsArcBottom = new OdGeKnotVector(coedgeArrKnotsArcBottom);

      OdGeNurbCurve2d coedgeArc = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArc, pointsArc);
      OdGeNurbCurve2d coedgeArcBottom = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArcBottom, pointsArcBottom);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        UInt32 edgeIdArc = brepBuilder.addEdge(arc);
        UInt32 edgeIdArcBottom = brepBuilder.addEdge(arcBottom);

        {
          UInt32 faceId = brepBuilder.addFace(planeBottom, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(rotSurf, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder.EntityDirection.kReversed, coedgeArc);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kForward, coedgeArcBottom);
          brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder.EntityDirection.kForward, coedgeArc);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addRevolvedSurface4(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Revolved Surf.4", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      double majorRadius = 10.0;
      double minorRadius = 4.0;
      double rotSurfRadius = 2;
      double arcStartAngle = Globals.OdaPI2;
      double arcEndAngle = Globals.OdaPI2 + Globals.OdaPI / 3;
      OdGeVector3d rotAxis = new OdGeVector3d(OdGeVector3d.kZAxis);
      OdGeVector3d refAxis = new OdGeVector3d(OdGeVector3d.kXAxis);
      OdGeVector3d yAxis = new OdGeVector3d(rotAxis.crossProduct(refAxis).normalize());
      OdGePoint3d center = new OdGePoint3d(OdGePoint3d.kOrigin);
      OdGePoint3d arcCenter = new OdGePoint3d(OdGePoint3d.kOrigin);
      OdGePoint3d centerTop = new OdGePoint3d(center - rotAxis * majorRadius * Math.Cos(arcEndAngle));

      // bottom -> top
      OdGeEllipArc3d arc = new OdGeEllipArc3d(arcCenter, -OdGeVector3d.kZAxis, OdGeVector3d.kXAxis, majorRadius, minorRadius, arcStartAngle, arcEndAngle);

      OdGeCircArc3d arcBottom = new OdGeCircArc3d(center, rotAxis, refAxis, minorRadius, 0, Globals.Oda2PI);
      OdGeCircArc3d arcTop = new OdGeCircArc3d(centerTop, rotAxis, refAxis, rotSurfRadius, 0, Globals.Oda2PI);

      OdGePlane planeBottom = new OdGePlane(center, -rotAxis);
      OdGePlane planeTop = new OdGePlane(centerTop, rotAxis);

      OdGeMatrix3d surfCoord = new OdGeMatrix3d();
      surfCoord.setCoordSystem(center, refAxis, yAxis, rotAxis).invert();
      OdGeEllipArc3d genArc = new OdGeEllipArc3d(arc);
      genArc.transformBy(surfCoord);
      OdGeRevolvedSurface rotSurf = new OdGeRevolvedSurface(genArc, center, rotAxis, refAxis);
      rotSurf.setAngles(0, Globals.Oda2PI);

      // u --- curve param, v --- rot param
      int coedgeDegree = 1;

      OdGePoint2dArray pointsArc = new OdGePoint2dArray(2);
      pointsArc.Add(new OdGePoint2d(arcStartAngle, 0.0));
      pointsArc.Add(new OdGePoint2d(arcEndAngle, 0.0));
      SupressFinalizeForItems(pointsArc);
      OdGePoint2dArray pointsArcBottom = new OdGePoint2dArray(2);
      pointsArcBottom.Add(new OdGePoint2d(arcStartAngle, 0.0));
      pointsArcBottom.Add(new OdGePoint2d(arcStartAngle, Globals.Oda2PI));
      SupressFinalizeForItems(pointsArcBottom);
      OdGePoint2dArray pointsArcTop = new OdGePoint2dArray(2);
      pointsArcTop.Add(new OdGePoint2d(arcEndAngle, 0.0));
      pointsArcTop.Add(new OdGePoint2d(arcEndAngle, Globals.Oda2PI));
      SupressFinalizeForItems(pointsArcTop);

      OdGeDoubleArray coedgeArrKnotsArc = new OdGeDoubleArray() { arcStartAngle, arcStartAngle, arcEndAngle, arcEndAngle };
      OdGeDoubleArray coedgeArrKnotsArcBottom = new OdGeDoubleArray() { 0.0, 0.0, Globals.Oda2PI, Globals.Oda2PI };
      OdGeDoubleArray coedgeArrKnotsArcTop = new OdGeDoubleArray() { 0.0, 0.0, Globals.Oda2PI, Globals.Oda2PI };
      if (coedgeArrKnotsArcBottom.Count != 4 ||
          coedgeArrKnotsArc.Count != 4 ||
          coedgeArrKnotsArcTop.Count != 4)
        throw new Exception("Error");

      OdGeKnotVector coedgeKnotsArc = new OdGeKnotVector(coedgeArrKnotsArc);
      OdGeKnotVector coedgeKnotsArcBottom = new OdGeKnotVector(coedgeArrKnotsArcBottom);
      OdGeKnotVector coedgeKnotsArcTop = new OdGeKnotVector(coedgeArrKnotsArcTop);

      OdGeNurbCurve2d coedgeArc = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArc, pointsArc);
      OdGeNurbCurve2d coedgeArcBottom = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArcBottom, pointsArcBottom);
      OdGeNurbCurve2d coedgeArcTop = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArcTop, pointsArcTop);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);


        UInt32 edgeIdArc = brepBuilder.addEdge(arc);
        UInt32 edgeIdArcBottom = brepBuilder.addEdge(arcBottom);
        UInt32 edgeIdArcTop = brepBuilder.addEdge(arcTop);

        {
          UInt32 faceId = brepBuilder.addFace(planeBottom, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(planeTop, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(rotSurf, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder.EntityDirection.kReversed, coedgeArc);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kForward, coedgeArcBottom);
          brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder.EntityDirection.kForward, coedgeArc);
          brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder.EntityDirection.kReversed, coedgeArcTop);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addRevolvedSurface5(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Revolved Surf.5", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      double radius = 4.0;
      OdGePoint3d center = new OdGePoint3d(0.0, 0.0, 0.0);
      double arcStartAngle = 0.0;
      double arcEndAngle = Globals.OdaPI2;
      OdGeVector3d rotAxis = new OdGeVector3d(OdGeVector3d.kZAxis);
      OdGeVector3d refAxis = new OdGeVector3d(OdGeVector3d.kXAxis);
      OdGeVector3d yAxis = new OdGeVector3d(rotAxis.crossProduct(refAxis).normalize());

      // top -> bottom
      OdGeCircArc3d arc = new OdGeCircArc3d(center, OdGeVector3d.kYAxis, rotAxis, radius, arcStartAngle, arcEndAngle);
      // bottom -> top
      OdGeLineSeg3d line = new OdGeLineSeg3d(center + refAxis * radius, center + rotAxis * radius * 0.5);

      OdGeCircArc3d arcBottom = new OdGeCircArc3d(center, rotAxis, refAxis, radius, 0, Globals.Oda2PI);

      OdGeMatrix3d surfCoord = new OdGeMatrix3d();
      surfCoord.setCoordSystem(center, refAxis, yAxis, rotAxis).invert();
      OdGeCircArc3d genArc = new OdGeCircArc3d(arc);
      genArc.transformBy(surfCoord);
      OdGeRevolvedSurface rotSurf1 = new OdGeRevolvedSurface(genArc, center, rotAxis, refAxis);
      rotSurf1.setAngles(0, Globals.Oda2PI);
      OdGeLineSeg3d genLine = new OdGeLineSeg3d(line);
      genLine.transformBy(surfCoord);
      OdGeRevolvedSurface rotSurf2 = new OdGeRevolvedSurface(genLine, center, rotAxis, refAxis);
      rotSurf2.setAngles(0, Globals.Oda2PI);
      rotSurf2.reverseNormal();

      // u --- curve param, v --- rot param
      const int coedgeDegree = 1;

      // rotSurf1
      OdGePoint2dArray pointsArc = new OdGePoint2dArray(2);
      pointsArc.Add(new OdGePoint2d(arcStartAngle, 0.0));
      pointsArc.Add(new OdGePoint2d(arcEndAngle, 0.0));
      SupressFinalizeForItems(pointsArc);
      OdGePoint2dArray pointsArcBottom1 = new OdGePoint2dArray(2);
      pointsArcBottom1.Add(new OdGePoint2d(arcEndAngle, 0.0));
      pointsArcBottom1.Add(new OdGePoint2d(arcEndAngle, Globals.Oda2PI));
      SupressFinalizeForItems(pointsArcBottom1);

      OdGeDoubleArray coedgeArrKnotsArc = new OdGeDoubleArray() { arcStartAngle, arcStartAngle, arcEndAngle, arcEndAngle };
      OdGeDoubleArray coedgeArrKnotsArcBottom1 = new OdGeDoubleArray() { 0.0, 0.0, Globals.Oda2PI, Globals.Oda2PI };
      if (coedgeArrKnotsArc.Count != 4 ||
          coedgeArrKnotsArcBottom1.Count != 4)
        throw new Exception("Error");

      OdGeKnotVector coedgeKnotsArc = new OdGeKnotVector(coedgeArrKnotsArc);
      OdGeKnotVector coedgeKnotsArcBottom1 = new OdGeKnotVector(coedgeArrKnotsArcBottom1);

      // rotSurf2
      OdGePoint2dArray pointsLine = new OdGePoint2dArray(2);
      pointsLine.Add(new OdGePoint2d(0.0, 0.0));
      pointsLine.Add(new OdGePoint2d(1.0, 0.0));
      OdGePoint2dArray pointsArcBottom2 = new OdGePoint2dArray(2);
      pointsArcBottom2.Add(new OdGePoint2d(0.0, 0.0));
      pointsArcBottom2.Add(new OdGePoint2d(0.0, Globals.Oda2PI));

      OdGeDoubleArray coedgeArrKnotsArcBottom2 = new OdGeDoubleArray() { 0.0, 0.0, Globals.Oda2PI, Globals.Oda2PI };
      OdGeDoubleArray coedgeArrKnotsLine = new OdGeDoubleArray() { 0.0, 0.0, 1.0, 1.0 };
      if (coedgeArrKnotsArcBottom2.Count != 4 ||
          coedgeArrKnotsLine.Count != 4)
        throw new Exception("Error");

      OdGeKnotVector coedgeKnotsArcBottom2 = new OdGeKnotVector(coedgeArrKnotsArcBottom2);
      OdGeKnotVector coedgeKnotsLine = new OdGeKnotVector(coedgeArrKnotsLine);

      //
      OdGeNurbCurve2d coedgeArc = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArc, pointsArc);
      OdGeNurbCurve2d coedgeArcBottom1 = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArcBottom1, pointsArcBottom1);
      OdGeNurbCurve2d coedgeLine = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsLine, pointsLine);
      OdGeNurbCurve2d coedgeArcBottom2 = new OdGeNurbCurve2d(coedgeDegree, coedgeKnotsArcBottom2, pointsArcBottom2);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);


        UInt32 edgeIdArc = brepBuilder.addEdge(arc);
        UInt32 edgeIdLine = brepBuilder.addEdge(line);
        UInt32 edgeIdArcBottom = brepBuilder.addEdge(arcBottom);

        {
          UInt32 faceId = brepBuilder.addFace(rotSurf1, OdBrepBuilder.EntityDirection.kReversed, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder.EntityDirection.kForward, coedgeArc);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kForward, coedgeArcBottom1);
          brepBuilder.addCoedge(loopId, edgeIdArc, OdBrepBuilder.EntityDirection.kReversed, coedgeArc);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(rotSurf2, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder.EntityDirection.kReversed, coedgeLine);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kReversed, coedgeArcBottom2);
          brepBuilder.addCoedge(loopId, edgeIdLine, OdBrepBuilder.EntityDirection.kForward, coedgeLine);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addEllipCylinder(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "EllipCylinder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      double majorRadius = 4.0;
      double minorRadius = 2.0;
      double height = 5.0;
      OdGeInterval heightInterval = new OdGeInterval(0.0, height);
      double startAngle = 0.0;
      double endAngle = Globals.Oda2PI;
      OdGeVector3d majorAxis = new OdGeVector3d(OdGeVector3d.kXAxis);
      OdGeVector3d minorAxis = new OdGeVector3d(OdGeVector3d.kYAxis);
      OdGeVector3d normal = new OdGeVector3d(majorAxis.crossProduct(minorAxis));
      OdGePoint3d centerBottom = new OdGePoint3d(OdGePoint3d.kOrigin);
      OdGePoint3d centerTop = new OdGePoint3d(centerBottom + normal * height);

      //
      OdGeEllipArc3d arcBottom = new OdGeEllipArc3d(centerBottom, majorAxis, minorAxis, majorRadius, minorRadius, startAngle, endAngle);
      OdGeEllipArc3d arcTop = new OdGeEllipArc3d(centerTop, majorAxis, minorAxis, majorRadius, minorRadius, startAngle, endAngle);

      OdGePlane planeBottom = new OdGePlane(centerBottom, majorAxis, minorAxis);
      OdGePlane planeTop = new OdGePlane(centerTop, majorAxis, minorAxis);
      OdGeEllipCylinder cylinder = new OdGeEllipCylinder(minorRadius, majorRadius, centerBottom, normal, majorAxis, heightInterval, startAngle, endAngle);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);


        UInt32 edgeIdArcBottom = brepBuilder.addEdge(arcBottom);
        UInt32 edgeIdArcTop = brepBuilder.addEdge(arcTop);

        {
          UInt32 faceId = brepBuilder.addFace(planeBottom, OdBrepBuilder.EntityDirection.kReversed, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(planeTop, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(cylinder, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();

        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addEllipCone(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "EllipCone", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      double majorRadius = 4.0;
      double minorRadius = 2.0;
      double height = 5.0;
      double coneCos = height / Math.Sqrt(height * height + majorRadius * majorRadius);
      double coneSin = majorRadius / Math.Sqrt(height * height + majorRadius * majorRadius);
      OdGeInterval heightInterval = new OdGeInterval(-height, 0.0);
      double startAngle = 0.0;
      double endAngle = Globals.Oda2PI;
      OdGeVector3d majorAxis = new OdGeVector3d(OdGeVector3d.kXAxis);
      OdGeVector3d minorAxis = new OdGeVector3d(OdGeVector3d.kYAxis);
      OdGeVector3d normal = new OdGeVector3d(majorAxis.crossProduct(minorAxis));
      OdGePoint3d centerBottom = new OdGePoint3d(OdGePoint3d.kOrigin);
      OdGePoint3d apex = new OdGePoint3d(centerBottom + normal * height);

      //
      OdGeEllipArc3d arcBottom = new OdGeEllipArc3d(centerBottom, majorAxis, minorAxis, majorRadius, minorRadius, startAngle, endAngle);
      OdGeLineSeg3d apexLine = new OdGeLineSeg3d(apex, apex);

      OdGePlane planeBottom = new OdGePlane(centerBottom, majorAxis, minorAxis);
      OdGeEllipCone cone = new OdGeEllipCone(coneCos, coneSin, centerBottom, minorRadius, majorRadius, -normal, majorAxis, heightInterval, startAngle, endAngle);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);


        UInt32 edgeIdArcBottom = brepBuilder.addEdge(arcBottom);
        UInt32 edgeIdApexLine = brepBuilder.addEdge(apexLine);

        {
          UInt32 faceId = brepBuilder.addFace(planeBottom, OdBrepBuilder.EntityDirection.kReversed, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(cone, OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdApexLine, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addCone(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId, bool useReverse /* = false */, bool cutBeforeApex /*= false*/)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Cone", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      double angleCone = Globals.OdaPI / 6.0;
      OdGePoint3d center = new OdGePoint3d(0.0, 0.0, 0.0);
      OdGeVector3d axis = new OdGeVector3d(0.0, 0, -1.0);// TODO check
      double radius = 3.15;
      double coneCos = Math.Cos(angleCone);
      double coneSin = Math.Sin(angleCone);
      double heightRatio = 0.5;// height offset from center of cone to cut cone

      //ODA_ASSERT_ONCE(coneCos > 0);

      OdGeCone cone = new OdGeCone(coneCos, coneSin, center, radius, axis);
      if (useReverse)
        cone = (OdGeCone)cone.reverseNormal(); // !

      OdGePoint3d apexPnt = new OdGePoint3d(cone.apex());

      OdGeCircArc3d arc1 = new OdGeCircArc3d(center, axis, radius);// loop 0
      if (useReverse)//we reversed cone surface. Now it hole and will not checking by acis. 
      {
        arc1 = (OdGeCircArc3d)arc1.reverseParam();
      }
      OdGeLineSeg3d line2 = new OdGeLineSeg3d(apexPnt, apexPnt);// loop 1: apex
      OdGeCircArc3d arc2 = new OdGeCircArc3d(center + (apexPnt - center) * heightRatio, axis, radius * (1.0 - heightRatio));// or loop 1: arc
      if (useReverse)//we reversed cone surface. Now it hole and will not checking by acis. 
      {
        arc2 = (OdGeCircArc3d)arc2.reverseParam();
      }
      OdGeCurve3d pCurveLoop1 = (cutBeforeApex ? (OdGeCurve3d)arc2 : (OdGeCurve3d)line2);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kOpenShell;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        UInt32 faceId = brepBuilder.addFace(cone, OdBrepBuilder.EntityDirection.kForward, shellId);
        UInt32 loopId;
        UInt32 edgeId;

        loopId = brepBuilder.addLoop(faceId);
        edgeId = brepBuilder.addEdge(arc1);
        brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder.EntityDirection.kReversed);
        brepBuilder.finishLoop(loopId);

        loopId = brepBuilder.addLoop(faceId);
        edgeId = brepBuilder.addEdge(pCurveLoop1);
        brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder.EntityDirection.kForward);
        brepBuilder.finishLoop(loopId);

        brepBuilder.finishFace(faceId);
        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        //
        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }
      OdDbBody pBody = OdDbBody.createObject();
      pBody.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pBody.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pBody.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pBody.transformBy(xfm);

      pBody.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pBody);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pBody.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    enum TorusType
    {
      ttDoughnut,
      ttApple,
      ttVortex,
      ttLemon,
    };

    void addTorus(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId, TorusType torusType /*= TorusType.ttLemon */)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Torus", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      double majorRadius = 0;
      double minorRadius = 0;
      switch (torusType)
      {
        case TorusType.ttDoughnut: majorRadius = 3.5; minorRadius = 1.0; break;
        case TorusType.ttApple: majorRadius = 2.0; minorRadius = 2.4; break;
        case TorusType.ttVortex: majorRadius = 2.4; minorRadius = 2.4; break;
        default:
        case TorusType.ttLemon: majorRadius = -2.0; minorRadius = 4.0; break;
      }
      OdGePoint3d center = new OdGePoint3d(0.0, 0.0, 0.0);
      OdGeVector3d axis = new OdGeVector3d(0.0, 0, 1.0);
      // Define apex for case !isMajorGreateMinor:
      OdGeVector3d apexOffset = TorusType.ttDoughnut == torusType ? new OdGeVector3d()
        : axis * Math.Sqrt(minorRadius * minorRadius - majorRadius * majorRadius);
      OdGePoint3d apexPoint1 = new OdGePoint3d(center + apexOffset);
      OdGePoint3d apexPoint2 = new OdGePoint3d(center - apexOffset);

      OdGeTorus torus = new OdGeTorus(majorRadius, minorRadius, center, axis);

      OdGeLineSeg3d apex1 = new OdGeLineSeg3d(apexPoint1, apexPoint1);// loop 1: apex
      OdGeLineSeg3d apex2 = new OdGeLineSeg3d(apexPoint2, apexPoint2);// loop 2: apex

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        UInt32 faceId = brepBuilder.addFace(torus, OdBrepBuilder.EntityDirection.kForward, shellId);

        if (TorusType.ttDoughnut != torusType)
        {
          UInt32 loopId;
          UInt32 edgeId;

          loopId = brepBuilder.addLoop(faceId);
          edgeId = brepBuilder.addEdge(apex1);
          brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);

          loopId = brepBuilder.addLoop(faceId);
          edgeId = brepBuilder.addEdge(apex2);
          brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
        }

        brepBuilder.finishFace(faceId);
        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addTorusQuarter(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "TorusQuarter", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      OdGePoint3d centerTorus = new OdGePoint3d(0.0, 0.0, 0.0);
      OdGeVector3d axisOfSymmetryTorus = new OdGeVector3d(0.0, 1.0, 0.0);
      OdGeVector3d refAxisTorus = new OdGeVector3d(1.0, 0.0, 0.0);
      double startAngleUTorus = 0.0;
      double endAngleUTorus = Globals.Oda2PI;
      double startAngleVTorus = -Globals.OdaPI;
      double endAngleVTorus = Globals.OdaPI;
      double majorRadiusTorus = 5.0;
      double minorRadiusTorus = 5.0;
      bool revTorus = false;

      // plane 1
      OdGePoint3d centerPlane1 = new OdGePoint3d(5.0, 0.0, 0.0);
      OdGeVector3d uPlane1 = new OdGeVector3d(1.0, 0.0, 0.0);
      OdGeVector3d vPlane1 = new OdGeVector3d(0.0, 1.0, 0.0);
      bool revPlane1 = false;
      OdGeVector3d majorAxisPlane1 = -uPlane1;
      OdGeVector3d minorAxisPlane1 = -vPlane1;
      double majorRadiusPlane1 = 5.0;
      double minorRadiusPlane1 = 5.0;
      double startParamPlane1 = 0.0;
      double includedParamPlane1 = Globals.Oda2PI;
      bool revEdgePlane1 = false;

      // plane 2
      OdGePoint3d centerPlane2 = new OdGePoint3d(0.0, 0.0, -5.0);
      OdGeVector3d uPlane2 = new OdGeVector3d(0.0, 0.0, -1.0);
      OdGeVector3d vPlane2 = new OdGeVector3d(0.0, 1.0, 0.0);
      bool revPlane2 = true;
      OdGeVector3d majorAxisPlane2 = -uPlane2;
      OdGeVector3d minorAxisPlane2 = -vPlane2;
      double majorRadiusPlane2 = 5.0;
      double minorRadiusPlane2 = 5.0;
      double startParamPlane2 = 0.0;
      double includedParamPlane2 = Globals.Oda2PI;
      bool revEdgePlane2 = true;

      //
      OdGeTorus torus = new OdGeTorus(majorRadiusTorus, minorRadiusTorus, centerTorus, axisOfSymmetryTorus, refAxisTorus, startAngleUTorus, endAngleUTorus, startAngleVTorus, endAngleVTorus);
      OdGePlane plane1 = new OdGePlane(centerPlane1, uPlane1, vPlane1);
      OdGeEllipArc3d arc1 = new OdGeEllipArc3d(centerPlane1, majorAxisPlane1, minorAxisPlane1, majorRadiusPlane1, minorRadiusPlane1, startParamPlane1, includedParamPlane1);
      OdGePlane plane2 = new OdGePlane(centerPlane2, uPlane2, vPlane2);
      OdGeEllipArc3d arc2 = new OdGeEllipArc3d(centerPlane2, majorAxisPlane2, minorAxisPlane2, majorRadiusPlane2, minorRadiusPlane2, startParamPlane2, includedParamPlane2);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        UInt32 edgeArc1 = brepBuilder.addEdge(arc1);
        UInt32 edgeArc2 = brepBuilder.addEdge(arc2);

        {
          UInt32 faceId = brepBuilder.addFace(plane1, revPlane1 ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeArc1, revEdgePlane1 ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(plane2, revPlane2 ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeArc2, revEdgePlane2 ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          UInt32 faceId = brepBuilder.addFace(torus, revTorus ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward, shellId);
          UInt32 loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeArc1, !revEdgePlane1 ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.addCoedge(loopId, edgeArc2, !revEdgePlane2 ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addSphere(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Sphere", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      double radius = 3;
      OdGePoint3d center = new OdGePoint3d(0.0, 0.0, 0.0);

      OdGeSphere sphere = new OdGeSphere(radius, center);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        UInt32 faceId = brepBuilder.addFace(sphere, OdBrepBuilder.EntityDirection.kForward, shellId);

        brepBuilder.finishFace(faceId);
        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addHemiSphere(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "HemiSphere", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      double radius = 3;
      OdGePoint3d center = new OdGePoint3d(0.0, 0.0, 0.0);
      OdGeVector3d normal = new OdGeVector3d(OdGeVector3d.kZAxis);

      OdGeVector3d northAxis = OdGeVector3d.kZAxis;
      OdGeVector3d refAxis = OdGeVector3d.kXAxis;
      double startAngleU = -Globals.OdaPI2;
      double endAngleU = Globals.OdaPI2;
      double startAngleV = 0.0;
      double endAngleV = Globals.Oda2PI;
      OdGeSphere sphere = new OdGeSphere(radius, center, northAxis, refAxis,
        startAngleU, endAngleU, startAngleV, endAngleV);
      OdGePlane plane = new OdGePlane(center, -OdGeVector3d.kZAxis);

      OdGeCircArc3d arc1 = new OdGeCircArc3d(center, normal, refAxis, radius, startAngleV, endAngleV);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        UInt32 faceId;
        UInt32 loopId;
        UInt32 edgeId = brepBuilder.addEdge(arc1);

        faceId = brepBuilder.addFace(sphere, OdBrepBuilder.EntityDirection.kForward, shellId);
        loopId = brepBuilder.addLoop(faceId);
        brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder.EntityDirection.kForward);
        brepBuilder.finishLoop(loopId);
        brepBuilder.finishFace(faceId);

        faceId = brepBuilder.addFace(plane, OdBrepBuilder.EntityDirection.kForward, shellId);
        loopId = brepBuilder.addLoop(faceId);
        brepBuilder.addCoedge(loopId, edgeId, OdBrepBuilder.EntityDirection.kReversed);
        brepBuilder.finishLoop(loopId);
        brepBuilder.finishFace(faceId);

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    static OdGeVector3d getPlaneNormal(OdGePoint3d point1, OdGePoint3d point2, OdGePoint3d point3)
    {
      OdGeVector3d XVector = (point2 - point1);
      OdGeVector3d YVector = (point3 - point1);
      if (Globals.OdZero(XVector.normalizeGetLength()) || Globals.OdZero(YVector.normalizeGetLength()))
        throw new Exception("Error");
      return XVector.normal().crossProduct(YVector.normal());
    }

    void addPyramid(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Pyramid", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      // Set the points of the pyramid
      OdGePoint3d pnt_1 = new OdGePoint3d(0.0, 2.0, 0.0);
      OdGePoint3d pnt_2 = new OdGePoint3d(3.0, -3.0, 0.0);
      OdGePoint3d pnt_3 = new OdGePoint3d(-3.0, -3.0, 0.0);
      OdGePoint3d pnt_4 = new OdGePoint3d((pnt_1 + pnt_2.asVector() + pnt_3.asVector()) * (1.0 / 3) + OdGeVector3d.kZAxis * 5.0);

      // Create Planes
      OdGePlane plane4 = new OdGePlane(pnt_1, getPlaneNormal(pnt_1, pnt_2, pnt_3));
      OdGePlane plane12 = new OdGePlane(pnt_4, getPlaneNormal(pnt_4, pnt_2, pnt_1));
      OdGePlane plane23 = new OdGePlane(pnt_4, getPlaneNormal(pnt_4, pnt_3, pnt_2));
      OdGePlane plane31 = new OdGePlane(pnt_4, getPlaneNormal(pnt_4, pnt_1, pnt_3));

      // Bottom edges
      OdGeLineSeg3d edge12 = new OdGeLineSeg3d(pnt_1, pnt_2);
      OdGeLineSeg3d edge23 = new OdGeLineSeg3d(pnt_2, pnt_3);
      OdGeLineSeg3d edge31 = new OdGeLineSeg3d(pnt_3, pnt_1);

      // Side edges
      OdGeLineSeg3d edge14 = new OdGeLineSeg3d(pnt_1, pnt_4);
      OdGeLineSeg3d edge24 = new OdGeLineSeg3d(pnt_2, pnt_4);
      OdGeLineSeg3d edge34 = new OdGeLineSeg3d(pnt_3, pnt_4);

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        // Add Faces
        UInt32 faceId4 = brepBuilder.addFace(plane4, OdBrepBuilder.EntityDirection.kForward, shellId);
        UInt32 faceId12 = brepBuilder.addFace(plane12, OdBrepBuilder.EntityDirection.kForward, shellId);
        UInt32 faceId23 = brepBuilder.addFace(plane23, OdBrepBuilder.EntityDirection.kForward, shellId);
        UInt32 faceId31 = brepBuilder.addFace(plane31, OdBrepBuilder.EntityDirection.kForward, shellId);

        // Add Edges
        // Bottom edges
        UInt32 edgeId12 = brepBuilder.addEdge(edge12);
        UInt32 edgeId23 = brepBuilder.addEdge(edge23);
        UInt32 edgeId31 = brepBuilder.addEdge(edge31);

        // Side edges
        UInt32 edgeId14 = brepBuilder.addEdge(edge14);
        UInt32 edgeId24 = brepBuilder.addEdge(edge24);
        UInt32 edgeId34 = brepBuilder.addEdge(edge34);

        // Edge color
        OdCmEntityColor color = new OdCmEntityColor();
        color.setRGB(0xff, 0xa5, 0x00);// use this color for all edges
        OdResult res;
        UInt32[] allEdges = new UInt32[] { edgeId12, edgeId23, edgeId31, edgeId14, edgeId24, edgeId34 };
        for (int i = 0; i < 6; ++i)
        {
          res = brepBuilder.setEdgeColor(allEdges[i], color);
          if (res != OdResult.eOk)
            throw new Exception("Fail setup color");
        }

        // Add Loops
        UInt32 loopId4 = brepBuilder.addLoop(faceId4);
        UInt32 loopId12 = brepBuilder.addLoop(faceId12);
        UInt32 loopId23 = brepBuilder.addLoop(faceId23);
        UInt32 loopId31 = brepBuilder.addLoop(faceId31);

        // Add Coedge
        // Bottom face
        brepBuilder.addCoedge(loopId4, edgeId12, OdBrepBuilder.EntityDirection.kForward);
        brepBuilder.addCoedge(loopId4, edgeId23, OdBrepBuilder.EntityDirection.kForward);
        brepBuilder.addCoedge(loopId4, edgeId31, OdBrepBuilder.EntityDirection.kForward);
        brepBuilder.finishLoop(loopId4);
        color.setRGB(0xff, 0xff, 0x00);
        res = brepBuilder.setFaceColor(faceId4, color);
        if (res != OdResult.eOk)
          throw new Exception("Fail setup color");
        brepBuilder.finishFace(faceId4);

        //
        brepBuilder.addCoedge(loopId12, edgeId12, OdBrepBuilder.EntityDirection.kReversed);
        brepBuilder.addCoedge(loopId12, edgeId14, OdBrepBuilder.EntityDirection.kForward);
        brepBuilder.addCoedge(loopId12, edgeId24, OdBrepBuilder.EntityDirection.kReversed);
        brepBuilder.finishLoop(loopId12);
        color.setRGB(0xff, 0x00, 0x00);
        res = brepBuilder.setFaceColor(faceId12, color);
        if (res != OdResult.eOk)
          throw new Exception("Fail setup color");
        brepBuilder.finishFace(faceId12);

        //
        brepBuilder.addCoedge(loopId23, edgeId23, OdBrepBuilder.EntityDirection.kReversed);
        brepBuilder.addCoedge(loopId23, edgeId24, OdBrepBuilder.EntityDirection.kForward);
        brepBuilder.addCoedge(loopId23, edgeId34, OdBrepBuilder.EntityDirection.kReversed);
        brepBuilder.finishLoop(loopId23);
        color.setRGB(0x00, 0xff, 0x00);
        res = brepBuilder.setFaceColor(faceId23, color);
        if (res != OdResult.eOk)
          throw new Exception("Fail setup color");
        brepBuilder.finishFace(faceId23);

        //
        brepBuilder.addCoedge(loopId31, edgeId31, OdBrepBuilder.EntityDirection.kReversed);
        brepBuilder.addCoedge(loopId31, edgeId34, OdBrepBuilder.EntityDirection.kForward);
        brepBuilder.addCoedge(loopId31, edgeId14, OdBrepBuilder.EntityDirection.kReversed);
        brepBuilder.finishLoop(loopId31);
        color.setRGB(0x00, 0x00, 0xff);
        res = brepBuilder.setFaceColor(faceId31, color);
        if (res != OdResult.eOk)
          throw new Exception("Fail setup color");
        brepBuilder.finishFace(faceId31);

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception("Error");
      }

      OdDb3dSolid pSolid = OdDb3dSolid.createObject();
      pSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pSolid.transformBy(xfm);

      pSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pSolid);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pSolid.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addPlane(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId, bool useReverse /*= false*/)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "Plane", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      OdGePoint3d center = new OdGePoint3d(0.0, 0.0, 0.0);
      OdGeVector3d norm = new OdGeVector3d(0.0, 0.0, 1.0);

      OdGePlane pln = new OdGePlane(center, norm);
      if (useReverse)
      {
        pln.reverseNormal();
      }

      OdGeLineSeg3d l1 = new OdGeLineSeg3d(new OdGePoint3d(0, 2, 0), new OdGePoint3d(1, -2, 0));
      OdGeLineSeg3d l2 = new OdGeLineSeg3d(new OdGePoint3d(1, -2, 0), new OdGePoint3d(-3, -2, 0));
      OdGeLineSeg3d l3 = new OdGeLineSeg3d(new OdGePoint3d(-3, -2, 0), new OdGePoint3d(0, 2, 0));

      OdRxObject brepData = null;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kOpenShell;
        pService.brepBuilder(brepBuilder, bbType);

        // Construct
        UInt32 complexId = brepBuilder.addComplex();
        UInt32 shellId = brepBuilder.addShell(complexId);

        UInt32 faceId = brepBuilder.addFace(/*cylinder*/pln, OdBrepBuilder.EntityDirection.kForward, shellId);
        UInt32 loopId;
        UInt32 edgeId;

        loopId = brepBuilder.addLoop(faceId);
        edgeId = brepBuilder.addEdge(l1);
        brepBuilder.addCoedge(loopId, edgeId, !useReverse ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward);

        edgeId = !useReverse ? brepBuilder.addEdge(l3) : brepBuilder.addEdge(l2);
        brepBuilder.addCoedge(loopId, edgeId, !useReverse ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward);

        edgeId = !useReverse ? brepBuilder.addEdge(l2) : brepBuilder.addEdge(l3);
        brepBuilder.addCoedge(loopId, edgeId, !useReverse ? OdBrepBuilder.EntityDirection.kReversed : OdBrepBuilder.EntityDirection.kForward);

        brepBuilder.finishLoop(loopId);

        brepBuilder.finishFace(faceId);
        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        brepData = brepBuilder.finish();

        if (brepData == null)
          throw new Exception("Error");
      }

      OdDbBody pBody = OdDbBody.createObject();
      pBody.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      pBody.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      pBody.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      pBody.transformBy(xfm);

      pBody.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(pBody);

      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(pBody.objectId());

      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void sample_get_InitialDataFromSolid_ByBrepFiller(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "3DSolidBox", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);
      /**********************************************************************/
      /* Create input 3D Solid Box                                          */
      /**********************************************************************/
      OdDb3dSolid input3dSolid = OdDb3dSolid.createObject();

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      input3dSolid.createBox(w * 6.0 / 8.0, h * 6.0 / 8.0, w * 6.0 / 8.0);
      input3dSolid.transformBy(xfm);

      OdBrBrep brep = new OdBrBrep();
      input3dSolid.brep(brep);

      /**********************************************************************/
      /* Fill BrepBuilder by means of OdBrepBuilderFillerModule             */
      /**********************************************************************/
      OdRxObject brepData;
      using (OdBrepBuilder bbuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kOpenShell;
        pService.brepBuilder(bbuilder, bbType);

        BrepBuilderInitialData initData = new BrepBuilderInitialData();

        using (OdBaseMaterialAndColorHelper materialHelper = new OdBaseMaterialAndColorHelper(input3dSolid.materialId().AsOdDbStubPointer()))  //Be sure to use "using()"
        {
          //SampleFillInitialData sample = new SampleFillInitialData();
          OdBrepBuilderFiller BBFiller = new OdBrepBuilderFiller();

          OdResult resInitFrom = BBFiller.initFrom(bbuilder,brep);
          if (resInitFrom != OdResult.eOk)
            throw new OdError(resInitFrom);
        }

        brepData = bbuilder.finish();
      }
      /**********************************************************************/
      /* Get OdDb3dSolid from brep geometry data                            */
      /**********************************************************************/
      OdDb3dSolid output3dSolid = OdDb3dSolid.createObject();
      output3dSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      output3dSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      output3dSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      output3dSolid.transformBy(xfm);

      output3dSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(output3dSolid);
      /**********************************************************************/
      /* Add the 3D Solid                                               */
      /**********************************************************************/
      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(output3dSolid.objectId());

      /**********************************************************************/
      /* Add 3D Solid to Sortents table                                 */
      /**********************************************************************/
      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void sample_get_InitialDataFromSolid_BySampleFillInitialData(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "3DSolidBox", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);
      /**********************************************************************/
      /* Create input 3D Solid Box                                          */
      /**********************************************************************/
      OdDb3dSolid input3dSolid = OdDb3dSolid.createObject();

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      input3dSolid.createBox(w * 6.0 / 8.0, h * 6.0 / 8.0, w * 6.0 / 8.0);
      input3dSolid.transformBy(xfm);

      OdBrBrep brep = new OdBrBrep();
      input3dSolid.brep(brep);

      /**********************************************************************/
      /* Fill BrepBuilder by means of OdBrepBuilderFillerModule             */
      /**********************************************************************/
      OdRxObject brepData;
      using (OdBrepBuilder bbuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kOpenShell;
        pService.brepBuilder(bbuilder, bbType);

        BrepBuilderInitialData initData = new BrepBuilderInitialData();

        using (OdBaseMaterialAndColorHelper materialHelper = new OdBaseMaterialAndColorHelper(input3dSolid.materialId().AsOdDbStubPointer()))  //Be sure to use "using()"
        {
          SampleFillInitialData sample = new SampleFillInitialData();
          OdResult resGetDataFrom = sample.getDataFrom(brep, ref initData);
          //OdResult resGetDataFrom = pFiller.getDataFrom(brep, materialHelper, initData);
          if (OdResult.eOk != resGetDataFrom)
            throw new OdError(resGetDataFrom);

          OdBrepBuilderFiller BBFiller = new OdBrepBuilderFiller();
          OdResult resInitFrom = BBFiller.initFrom(bbuilder, initData);
          if (resInitFrom != OdResult.eOk)
            throw new OdError(resInitFrom);
        }

        brepData = bbuilder.finish();
      }
      /**********************************************************************/
      /* Get OdDb3dSolid from brep geometry data                            */
      /**********************************************************************/
      OdDb3dSolid output3dSolid = OdDb3dSolid.createObject();
      output3dSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      output3dSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      output3dSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      output3dSolid.transformBy(xfm);

      output3dSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(output3dSolid);
      /**********************************************************************/
      /* Add the 3D Solid                                               */
      /**********************************************************************/
      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(output3dSolid.objectId());

      /**********************************************************************/
      /* Add 3D Solid to Sortents table                                 */
      /**********************************************************************/
      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    void addTCoedgeEllipCylinder(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId, double deltaRadius /*= 1e-3*/)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "TCoedgeEllipCylinder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());

      // Ge
      const double majorRadius = 4.0;
      const double minorRadius = 2.0;
      const double height = 5.0;
      OdGeInterval heightInterval = new OdGeInterval(0.0, height);
      const double startAngle = 0.0;
      const double endAngle = Globals.Oda2PI;
      OdGeVector3d majorAxis = new OdGeVector3d(OdGeVector3d.kXAxis);
      OdGeVector3d minorAxis = new OdGeVector3d(OdGeVector3d.kYAxis);
      OdGeVector3d normal = new OdGeVector3d(majorAxis.crossProduct(minorAxis));
      OdGePoint3d centerBottom = new OdGePoint3d(OdGePoint3d.kOrigin);
      OdGePoint3d centerTop = new OdGePoint3d(centerBottom + normal * height);

      //
      double newMajorRadius = majorRadius + deltaRadius;
      double newMinorRadius = minorRadius;
      OdGeEllipArc3d arcBottom = new OdGeEllipArc3d(centerBottom, majorAxis, minorAxis, newMajorRadius, newMinorRadius, startAngle, endAngle);
      OdGeEllipArc3d arcTop = new OdGeEllipArc3d(centerTop, majorAxis, minorAxis, majorRadius, minorRadius, startAngle, endAngle);

      OdGeEllipArc2d arcBottom2d = new OdGeEllipArc2d(centerBottom.convert2d(), majorAxis.convert2d(), minorAxis.convert2d(), majorRadius, minorRadius, startAngle, endAngle);

      OdGePlane planeBottom = new OdGePlane(centerBottom, majorAxis, minorAxis);
      OdGePlane planeTop = new OdGePlane(centerTop, majorAxis, minorAxis);
      OdGeEllipCylinder cylinder = new OdGeEllipCylinder(minorRadius, majorRadius, centerBottom, normal, majorAxis, heightInterval, startAngle, endAngle);

      //
      OdGeNurbCurve2d coedgeArcBottom = new OdGeNurbCurve2d();
      setupUVCurve(ref coedgeArcBottom, new OdGePoint2d(0.0, startAngle), new OdGePoint2d(0.0, endAngle), startAngle, endAngle);
      OdGeNurbCurve2d pCoedgeArcBottomPlane = new OdGeNurbCurve2d(OdGeNurbCurve2d.convertFrom(arcBottom2d));

      OdRxObject brepData;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        OdResult res_brepBuilder = pService.brepBuilder(brepBuilder, bbType);

        if (res_brepBuilder != OdResult.eOk)
          throw new Exception();

        // Construct
        uint complexId = brepBuilder.addComplex();
        uint shellId = brepBuilder.addShell(complexId);


        uint edgeIdArcBottom = brepBuilder.addEdge(arcBottom);
        uint edgeIdArcTop = brepBuilder.addEdge(arcTop);

        {
          uint faceId = brepBuilder.addFace(planeBottom, OdBrepBuilder.EntityDirection.kReversed, shellId);
          uint loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kReversed, pCoedgeArcBottomPlane);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          uint faceId = brepBuilder.addFace(planeTop, OdBrepBuilder.EntityDirection.kForward, shellId);
          uint loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder.EntityDirection.kForward);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }
        {
          uint faceId = brepBuilder.addFace(cylinder, OdBrepBuilder.EntityDirection.kForward, shellId);
          uint loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcBottom, OdBrepBuilder.EntityDirection.kForward, coedgeArcBottom);
          brepBuilder.finishLoop(loopId);
          loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, edgeIdArcTop, OdBrepBuilder.EntityDirection.kReversed);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        //
        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception();

      }

      OdDb3dSolid output3dSolid = OdDb3dSolid.createObject();
      output3dSolid.setBody(OdRxObject.getCPtr(brepData).Handle);

      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      output3dSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      output3dSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      output3dSolid.transformBy(xfm);

      output3dSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(output3dSolid);
      /**********************************************************************/
      /* Add the 3D Solid                                               */
      /**********************************************************************/
      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(output3dSolid.objectId());

      /**********************************************************************/
      /* Add 3D Solid to Sortents table                                 */
      /**********************************************************************/
      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }

    static void setupUVCurve(ref OdGeNurbCurve2d uvCurve, OdGePoint2d pnt1, OdGePoint2d pnt2, double param1, double param2)
    {
      const int coedgeDegree = 1;

      OdGeDoubleArray uvKnotsVals = new OdGeDoubleArray();
      uvKnotsVals.Add(param1);
      uvKnotsVals.Add(param1);
      uvKnotsVals.Add(param2);
      uvKnotsVals.Add(param2);

      if (uvKnotsVals.Count != 4)
        throw new Exception();

      OdGeKnotVector uvKnots = new OdGeKnotVector(uvKnotsVals);

      OdGePoint2dArray uvPoints = new OdGePoint2dArray(2);
      uvPoints.Add(pnt1);
      uvPoints.Add(pnt2);

      OdGeDoubleArray weights = new OdGeDoubleArray();

      uvCurve.set(coedgeDegree, uvKnots, uvPoints, weights);
    }

    void addTCoedgeFourthSphere(OdDbDatabase pDb, int boxRow, int boxCol, OdDbObjectId layerId, OdDbObjectId styleId, double deltaRadius /*= 1e-3*/)
    {
      OdDbBlockTableRecord bBTR = (OdDbBlockTableRecord)pDb.getModelSpaceId().safeOpenObject(OpenMode.kForWrite);

      /**********************************************************************/
      /* Get the Upper-left corner of the box and its size                  */
      /**********************************************************************/
      OdGePoint3d ptUpperLeft = EntityBoxes.getBox(boxRow, boxCol);
      double h = EntityBoxes.getHeight();
      double w = EntityBoxes.getWidth(boxRow, boxCol);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
        "TCoedgeFourthSphere", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      addTextEnt(bBTR,
        ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
        "by brep builder", m_textSize, TextHorzMode.kTextLeft, TextVertMode.kTextTop, layerId, styleId);

      OdGeMatrix3d xfm = OdGeMatrix3d.translation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector());


      // Ge
      double radius = 3;
      OdGePoint3d center = new OdGePoint3d(0.0, 0.0, 0.0);
      OdGeVector3d normal = new OdGeVector3d(OdGeVector3d.kZAxis);

      OdGeVector3d northAxis = OdGeVector3d.kZAxis;
      OdGeVector3d refAxis = OdGeVector3d.kXAxis;
      double startAngleU = -Globals.OdaPI2;
      double endAngleU = Globals.OdaPI2;
      double startAngleV = 0.0;
      double endAngleV = Globals.Oda2PI;
      double cutAngleV = Globals.OdaPI;
      // all face normal is outer
      OdGeSphere sphere = new OdGeSphere(radius, center, northAxis, refAxis,
        startAngleU, endAngleU, startAngleV, endAngleV);
      OdGePlane planeBottom = new OdGePlane(center, -OdGeVector3d.kXAxis, OdGeVector3d.kYAxis);// normal = -OdGeVector3d.kZAxis
      OdGePlane planeSide = new OdGePlane(center, OdGeVector3d.kXAxis, OdGeVector3d.kZAxis);// normal = -OdGeVector3d.kYAxis

      OdGeCircArc3d arcBottom = new OdGeCircArc3d(center, -normal, -refAxis, radius + deltaRadius, startAngleV, cutAngleV);
      OdGeLineSeg3d lineBottom = new OdGeLineSeg3d(center - refAxis * (radius + deltaRadius), center + refAxis * (radius + deltaRadius));
      OdGeCircArc3d arcTop1 = new OdGeCircArc3d(center, -OdGeVector3d.kYAxis, refAxis, radius + deltaRadius, 0.0, Globals.OdaPI2);
      OdGeCircArc3d arcTop2 = new OdGeCircArc3d(center, -OdGeVector3d.kYAxis, refAxis, radius + deltaRadius, Globals.OdaPI2, Globals.OdaPI);

      //
      OdGeNurbCurve2d coedgeArcBottomSphere = new OdGeNurbCurve2d();
      setupUVCurve(ref coedgeArcBottomSphere, new OdGePoint2d(0.0, Globals.OdaPI), new OdGePoint2d(0.0, 0.0), 0.0, Globals.OdaPI);

      OdGeCircArc2d arcBottomPlaneBottom = new OdGeCircArc2d(center.convert2d(), radius, 0.0, Globals.OdaPI, OdGeVector2d.kXAxis);
      OdGeNurbCurve2d pCoedgeArcBottomPlaneBottom = new OdGeNurbCurve2d(OdGeNurbCurve2d.convertFrom(arcBottomPlaneBottom));

      OdGeNurbCurve2d coedgeLineBottomPlaneBottom = new OdGeNurbCurve2d();
      setupUVCurve(ref coedgeLineBottomPlaneBottom, new OdGePoint2d(radius, 0.0), new OdGePoint2d(-radius, 0.0), 0.0, 1.0);
      OdGeNurbCurve2d coedgeLineBottomPlaneSide = new OdGeNurbCurve2d();
      setupUVCurve(ref coedgeLineBottomPlaneSide, new OdGePoint2d(-radius, 0.0), new OdGePoint2d(radius, 0.0), 0.0, 1.0);

      OdGeNurbCurve2d coedgeArcTop1Sphere = new OdGeNurbCurve2d();
      setupUVCurve(ref coedgeArcTop1Sphere, new OdGePoint2d(0.0, 0.0), new OdGePoint2d(Globals.OdaPI2, 0.0), 0.0, Globals.OdaPI2);
      OdGeNurbCurve2d coedgeArcTop2Sphere = new OdGeNurbCurve2d();
      setupUVCurve(ref coedgeArcTop2Sphere, new OdGePoint2d(Globals.OdaPI2, Globals.OdaPI), new OdGePoint2d(0.0, Globals.OdaPI), Globals.OdaPI2, Globals.OdaPI);

      OdGeCircArc2d arcTop1PlaneSide = new OdGeCircArc2d(center.convert2d(), radius, 0.0, Globals.OdaPI2, OdGeVector2d.kXAxis);
      OdGeCircArc2d arcTop2PlaneSide = new OdGeCircArc2d(center.convert2d(), radius, Globals.OdaPI2, Globals.OdaPI, OdGeVector2d.kXAxis);
      OdGeNurbCurve2d pCoedgeArcTop1PlaneSide = new OdGeNurbCurve2d(OdGeNurbCurve2d.convertFrom(arcTop1PlaneSide));
      OdGeNurbCurve2d pCoedgeArcTop2PlaneSide = new OdGeNurbCurve2d(OdGeNurbCurve2d.convertFrom(arcTop2PlaneSide));

      OdRxObject brepData;
      using (OdBrepBuilder brepBuilder = new OdBrepBuilder()) //Be sure to use "using()"
      {
        OdDbHostAppServices pService = pDb.appServices();
        BrepType bbType = BrepType.kSolid;
        OdResult res_brepBuilder = pService.brepBuilder(brepBuilder, bbType);

        if (res_brepBuilder != OdResult.eOk)
          throw new Exception();

        // Construct
        uint complexId = brepBuilder.addComplex();
        uint shellId = brepBuilder.addShell(complexId);

        uint faceId;
        uint loopId;
        uint arcBottomEdgeId = brepBuilder.addEdge(arcBottom);
        uint lineBottomEdgeId = brepBuilder.addEdge(lineBottom);
        uint arcTop1EdgeId = brepBuilder.addEdge(arcTop1);
        uint arcTop2EdgeId = brepBuilder.addEdge(arcTop2);

        {
          faceId = brepBuilder.addFace(sphere, OdBrepBuilder.EntityDirection.kForward, shellId);
          loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, arcBottomEdgeId, OdBrepBuilder.EntityDirection.kReversed, coedgeArcBottomSphere);
          brepBuilder.addCoedge(loopId, arcTop2EdgeId, OdBrepBuilder.EntityDirection.kReversed, coedgeArcTop2Sphere);
          brepBuilder.addCoedge(loopId, arcTop1EdgeId, OdBrepBuilder.EntityDirection.kReversed, coedgeArcTop1Sphere);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        {
          faceId = brepBuilder.addFace(planeBottom, OdBrepBuilder.EntityDirection.kForward, shellId);
          loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, arcBottomEdgeId, OdBrepBuilder.EntityDirection.kForward, pCoedgeArcBottomPlaneBottom);
          brepBuilder.addCoedge(loopId, lineBottomEdgeId, OdBrepBuilder.EntityDirection.kReversed, coedgeLineBottomPlaneBottom);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        {
          faceId = brepBuilder.addFace(planeSide, OdBrepBuilder.EntityDirection.kForward, shellId);
          loopId = brepBuilder.addLoop(faceId);
          brepBuilder.addCoedge(loopId, lineBottomEdgeId, OdBrepBuilder.EntityDirection.kForward, coedgeLineBottomPlaneSide);
          brepBuilder.addCoedge(loopId, arcTop1EdgeId, OdBrepBuilder.EntityDirection.kForward, pCoedgeArcTop1PlaneSide);
          brepBuilder.addCoedge(loopId, arcTop2EdgeId, OdBrepBuilder.EntityDirection.kForward, pCoedgeArcTop2PlaneSide);
          brepBuilder.finishLoop(loopId);
          brepBuilder.finishFace(faceId);
        }

        brepBuilder.finishShell(shellId);
        brepBuilder.finishComplex(complexId);

        //
        brepData = brepBuilder.finish();
        if (brepData == null)
          throw new Exception();
      }

      OdDb3dSolid output3dSolid = OdDb3dSolid.createObject();
      output3dSolid.setBody(OdRxObject.getCPtr(brepData).Handle);


      /**********************************************************************/
      /* Add to database                                                    */
      /**********************************************************************/
      OdGeExtents3d ext = new OdGeExtents3d();
      output3dSolid.getGeomExtents(ext);
      double scaleX = w * 0.8 / (ext.maxPoint().x - ext.minPoint().x);
      double scaleY = h * 0.5 / (ext.maxPoint().y - ext.minPoint().y);

      xfm.setToScaling(Math.Min(scaleX, scaleY), ext.center());
      output3dSolid.transformBy(xfm);
      xfm.setToTranslation(EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
      output3dSolid.transformBy(xfm);

      output3dSolid.setDatabaseDefaults(bBTR.database());
      bBTR.appendOdDbEntity(output3dSolid);
      /**********************************************************************/
      /* Add the 3D Solid                                               */
      /**********************************************************************/
      OdDbObjectIdArray arObjId = new OdDbObjectIdArray();
      arObjId.Add(output3dSolid.objectId());

      /**********************************************************************/
      /* Add 3D Solid to Sortents table                                 */
      /**********************************************************************/
      OdDbSortentsTable pSortentsTable = bBTR.getSortentsTable(true);
      pSortentsTable.moveToBottom(arObjId);
    }
  }
}