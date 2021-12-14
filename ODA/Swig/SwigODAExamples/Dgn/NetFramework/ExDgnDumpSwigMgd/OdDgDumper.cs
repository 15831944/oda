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
  class OdDgDumper : IDisposable
  {
    public OdDgDumper()
    {
      //OdRxObject_Dumper::rxInit();

      OdDgDatabase.desc().addX(OdRxObject_Dumper.desc(), m_databaseDumper);
      OdDgModel.desc().addX(OdRxObject_Dumper.desc(), m_modelDumper);

      OdDgLine2d.desc().addX(OdRxObject_Dumper.desc(), m_line2dDumper);
      OdDgLine3d.desc().addX(OdRxObject_Dumper.desc(), m_line3dDumper);
      //////////// test of grip points /////////////////////////////////////
      //OdDgLine2d.desc().addX(OdDgGripPointsPE.desc(), m_gripLine);
      //OdDgLine3d.desc().addX(OdDgGripPointsPE.desc(), m_gripLine);

      OdDgLineString2d.desc().addX(OdRxObject_Dumper.desc(), m_lineString2dDumper);
      OdDgLineString3d.desc().addX(OdRxObject_Dumper.desc(), m_lineString3dDumper);
      OdDgText2d.desc().addX(OdRxObject_Dumper.desc(), m_text2dDumper);
      OdDgText3d.desc().addX(OdRxObject_Dumper.desc(), m_text3dDumper);
      OdDgTextNode2d.desc().addX(OdRxObject_Dumper.desc(), m_textNode2dDumper);
      OdDgTextNode3d.desc().addX(OdRxObject_Dumper.desc(), m_textNode3dDumper);
      OdDgShape2d.desc().addX(OdRxObject_Dumper.desc(), m_shape2dDumper);
      OdDgShape3d.desc().addX(OdRxObject_Dumper.desc(), m_shape3dDumper);
      OdDgCurve2d.desc().addX(OdRxObject_Dumper.desc(), m_curve2dDumper);
      OdDgCurve3d.desc().addX(OdRxObject_Dumper.desc(), m_curve3dDumper);
      OdDgEllipse2d.desc().addX(OdRxObject_Dumper.desc(), m_ellipse2dDumper);
      OdDgEllipse3d.desc().addX(OdRxObject_Dumper.desc(), m_ellipse3dDumper);
      OdDgArc2d.desc().addX(OdRxObject_Dumper.desc(), m_arc2dDumper);
      OdDgArc3d.desc().addX(OdRxObject_Dumper.desc(), m_arc3dDumper);
      OdDgCone.desc().addX(OdRxObject_Dumper.desc(), m_coneDumper);
      OdDgComplexString.desc().addX(OdRxObject_Dumper.desc(), m_complexStringDumper);
      OdDgComplexShape.desc().addX(OdRxObject_Dumper.desc(), m_complexShapeDumper);
      OdDgPointString2d.desc().addX(OdRxObject_Dumper.desc(), m_pointString2dDumper);
      OdDgPointString3d.desc().addX(OdRxObject_Dumper.desc(), m_pointString3dDumper);
      OdDgDimension.desc().addX(OdRxObject_Dumper.desc(), m_dimensionDumper);
      OdDgMultiline.desc().addX(OdRxObject_Dumper.desc(), m_multilineDumper);
      OdDgBSplineCurve2d.desc().addX(OdRxObject_Dumper.desc(), m_bSplineCurve2dDumper);
      OdDgBSplineCurve3d.desc().addX(OdRxObject_Dumper.desc(), m_bSplineCurve3dDumper);
      OdDgSurface.desc().addX(OdRxObject_Dumper.desc(), m_surfaceDumper);
      OdDgSolid.desc().addX(OdRxObject_Dumper.desc(), m_solidDumper);
      OdDgRasterAttachmentHeader.desc().addX(OdRxObject_Dumper.desc(), m_rasterAttachmentHeaderDumper);
      OdDgRasterHeader2d.desc().addX(OdRxObject_Dumper.desc(), m_rasterHeader2dDumper);
      OdDgRasterHeader3d.desc().addX(OdRxObject_Dumper.desc(), m_rasterHeader3dDumper);
      OdDgRasterComponent.desc().addX(OdRxObject_Dumper.desc(), m_rasterComponentDumper);
      OdDgTagElement.desc().addX(OdRxObject_Dumper.desc(), m_tagElementDumper);
      OdDgCellHeader2d.desc().addX(OdRxObject_Dumper.desc(), m_cellHeader2dDumper);
      OdDgCellHeader3d.desc().addX(OdRxObject_Dumper.desc(), m_cellHeader3dDumper);
      OdDgBSplineSurface.desc().addX(OdRxObject_Dumper.desc(), m_bSplineSurfaceDumper);
      OdDgLevelTable.desc().addX(OdRxObject_Dumper.desc(), m_levelTableDumper);
      OdDgLevelTableRecord.desc().addX(OdRxObject_Dumper.desc(), m_levelTableRecordDumper);
      OdDgLevelFilterTable.desc().addX(OdRxObject_Dumper.desc(), m_levelFilterTableDumper);
      OdDgLevelFilterTableRecord.desc().addX(OdRxObject_Dumper.desc(), m_levelFilterTableRecordDumper);
      OdDgFontTable.desc().addX(OdRxObject_Dumper.desc(), m_fontTableDumper);
      OdDgFontTableRecord.desc().addX(OdRxObject_Dumper.desc(), m_fontTableRecordDumper);
      OdDgTextStyleTable.desc().addX(OdRxObject_Dumper.desc(), m_textStyleTableDumper);
      OdDgTextStyleTableRecord.desc().addX(OdRxObject_Dumper.desc(), m_textStyleTableRecordDumper);
      OdDgDimStyleTable.desc().addX(OdRxObject_Dumper.desc(), m_dimStyleTableDumper);
      OdDgDimStyleTableRecord.desc().addX(OdRxObject_Dumper.desc(), m_dimStyleTableRecordDumper);
      OdDgMultilineStyleTable.desc().addX(OdRxObject_Dumper.desc(), m_multilineStyleTableDumper);
      OdDgMultilineStyleTableRecord.desc().addX(OdRxObject_Dumper.desc(), m_multilineStyleTableRecordDumper);
      OdDgLineStyleTable.desc().addX(OdRxObject_Dumper.desc(), m_lineStyleTableDumper);
      OdDgLineStyleTableRecord.desc().addX(OdRxObject_Dumper.desc(), m_lineStyleTableRecordDumper);
      OdDgRegAppTable.desc().addX(OdRxObject_Dumper.desc(), m_regAppTableDumper);
      OdDgRegAppTableRecord.desc().addX(OdRxObject_Dumper.desc(), m_regAppTableRecordDumper);
      OdDgViewGroupTable.desc().addX(OdRxObject_Dumper.desc(), m_viewGroupTableDumper);
      OdDgViewGroup.desc().addX(OdRxObject_Dumper.desc(), m_viewGroupDumper);
      OdDgView.desc().addX(OdRxObject_Dumper.desc(), m_viewDumper);
      OdDgNamedViewTable.desc().addX(OdRxObject_Dumper.desc(), m_namedViewTableDumper);
      OdDgSharedCellDefinitionTable.desc().addX(OdRxObject_Dumper.desc(), m_sharedCellDefinitionTableDumper);
      OdDgSharedCellDefinition.desc().addX(OdRxObject_Dumper.desc(), m_sharedCellDefinitionDumper);
      OdDgSharedCellReference.desc().addX(OdRxObject_Dumper.desc(), m_sharedCellReferenceDumper);
      OdDgTagDefinitionSetTable.desc().addX(OdRxObject_Dumper.desc(), m_tagSetDefinitionTableDumper);
      OdDgTagDefinitionSet.desc().addX(OdRxObject_Dumper.desc(), m_tagSetDefinitionDumper);
      OdDgTagDefinition.desc().addX(OdRxObject_Dumper.desc(), m_tagDefinitionDumper);
      OdDgColorTable.desc().addX(OdRxObject_Dumper.desc(), m_colorTableDumper);
      OdDgReferenceAttachmentHeader.desc().addX(OdRxObject_Dumper.desc(), m_referenceAttachmentDumper);
      OdDgMeshFaceLoops.desc().addX(OdRxObject_Dumper.desc(), m_meshFaceLoopsDumper);
      OdDgMeshPointCloud.desc().addX(OdRxObject_Dumper.desc(), m_meshPointCloudDumper);
      OdDgMeshTriangleList.desc().addX(OdRxObject_Dumper.desc(), m_meshTriangleListDumper);
      OdDgMeshQuadList.desc().addX(OdRxObject_Dumper.desc(), m_meshQuadListDumper);
      OdDgMeshTriangleGrid.desc().addX(OdRxObject_Dumper.desc(), m_meshTriangleGridDumper);
      OdDgMeshQuadGrid.desc().addX(OdRxObject_Dumper.desc(), m_meshQuadGridDumper);
      OdDgProxyElement.desc().addX(OdRxObject_Dumper.desc(), m_proxyDumper);
      OdDgApplicationData.desc().addX(OdRxObject_Dumper.desc(), m_applicationDataDumper);
    }
    public void Dispose()
    {
      OdDgDatabase.desc().delX(OdRxObject_Dumper.desc());
      OdDgModel.desc().delX(OdRxObject_Dumper.desc());
      OdDgLine2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgLine3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgLineString2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgLineString3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgText2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgText3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgTextNode2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgTextNode3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgShape2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgShape3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgCurve2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgCurve3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgEllipse2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgEllipse3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgArc2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgArc3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgCone.desc().delX(OdRxObject_Dumper.desc());
      OdDgComplexString.desc().delX(OdRxObject_Dumper.desc());
      OdDgComplexShape.desc().delX(OdRxObject_Dumper.desc());
      OdDgPointString2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgPointString3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgDimension.desc().delX(OdRxObject_Dumper.desc());
      OdDgMultiline.desc().delX(OdRxObject_Dumper.desc());
      OdDgBSplineCurve2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgBSplineCurve3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgSurface.desc().delX(OdRxObject_Dumper.desc());
      OdDgSolid.desc().delX(OdRxObject_Dumper.desc());
      OdDgRasterAttachmentHeader.desc().delX(OdRxObject_Dumper.desc());
      OdDgRasterHeader2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgRasterHeader3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgRasterComponent.desc().delX(OdRxObject_Dumper.desc());
      OdDgTagElement.desc().delX(OdRxObject_Dumper.desc());
      OdDgCellHeader2d.desc().delX(OdRxObject_Dumper.desc());
      OdDgCellHeader3d.desc().delX(OdRxObject_Dumper.desc());
      OdDgBSplineSurface.desc().delX(OdRxObject_Dumper.desc());
      OdDgLevelTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgLevelTableRecord.desc().delX(OdRxObject_Dumper.desc());
      OdDgLevelFilterTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgLevelFilterTableRecord.desc().delX(OdRxObject_Dumper.desc());
      OdDgFontTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgFontTableRecord.desc().delX(OdRxObject_Dumper.desc());
      OdDgTextStyleTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgTextStyleTableRecord.desc().delX(OdRxObject_Dumper.desc());
      OdDgDimStyleTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgDimStyleTableRecord.desc().delX(OdRxObject_Dumper.desc());
      OdDgMultilineStyleTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgMultilineStyleTableRecord.desc().delX(OdRxObject_Dumper.desc());
      OdDgLineStyleTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgLineStyleTableRecord.desc().delX(OdRxObject_Dumper.desc());
      OdDgRegAppTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgRegAppTableRecord.desc().delX(OdRxObject_Dumper.desc());
      OdDgViewGroupTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgViewGroup.desc().delX(OdRxObject_Dumper.desc());
      OdDgView.desc().delX(OdRxObject_Dumper.desc());
      OdDgNamedViewTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgSharedCellDefinitionTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgSharedCellDefinition.desc().delX(OdRxObject_Dumper.desc());
      OdDgSharedCellReference.desc().delX(OdRxObject_Dumper.desc());
      OdDgTagDefinitionSetTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgTagDefinitionSet.desc().delX(OdRxObject_Dumper.desc());
      OdDgTagDefinition.desc().delX(OdRxObject_Dumper.desc());
      OdDgColorTable.desc().delX(OdRxObject_Dumper.desc());
      OdDgReferenceAttachmentHeader.desc().delX(OdRxObject_Dumper.desc());
      OdDgMeshFaceLoops.desc().delX(OdRxObject_Dumper.desc());
      OdDgMeshPointCloud.desc().delX(OdRxObject_Dumper.desc());
      OdDgMeshTriangleList.desc().delX(OdRxObject_Dumper.desc());
      OdDgMeshQuadList.desc().delX(OdRxObject_Dumper.desc());
      OdDgMeshTriangleGrid.desc().delX(OdRxObject_Dumper.desc());
      OdDgMeshQuadGrid.desc().delX(OdRxObject_Dumper.desc());
      OdDgProxyElement.desc().delX(OdRxObject_Dumper.desc());
      OdDgApplicationData.desc().delX(OdRxObject_Dumper.desc());

      //OdRxObject_Dumper::rxUninit();    
    }

    public void rootDump(OdRxObject database)  //root dumper
    {
      MemoryTransaction mTr = MemoryManager.GetMemoryManager().StartTransaction();
      //OdRxObject_Dumper database_dumper = (OdRxObject_Dumper)database;//OdRxObject_Dumper.cast(database);
      //String str = database.GetType().ToString();
      OdRxObject_Dumper database_dumper = OdRxObject_Dumper.GetProperType(database);
      //String str = ((OdDgDatabase)database)..ToString();
      //run the ordinal method
      database_dumper.dump(database);
      // OdDgDatabase_Dumper ExDgnDump
      MemoryManager.GetMemoryManager().StopTransaction(mTr);
    }

    private OdDgDatabase_Dumper m_databaseDumper = new OdDgDatabase_Dumper();
    private OdDgModel_Dumper m_modelDumper = new OdDgModel_Dumper();
    private OdDgLine2d_Dumper m_line2dDumper = new OdDgLine2d_Dumper();
    private OdDgLine3d_Dumper m_line3dDumper = new OdDgLine3d_Dumper();
    //private OdDgLineGripPointsPE m_gripLine = new OdDgLineGripPointsPE();
    private OdDgLineString2d_Dumper m_lineString2dDumper = new OdDgLineString2d_Dumper();
    private OdDgLineString3d_Dumper m_lineString3dDumper = new OdDgLineString3d_Dumper();
    private OdDgText2d_Dumper m_text2dDumper = new OdDgText2d_Dumper();
    private OdDgText3d_Dumper m_text3dDumper = new OdDgText3d_Dumper();
    private OdDgTextNode2d_Dumper m_textNode2dDumper = new OdDgTextNode2d_Dumper();
    private OdDgTextNode3d_Dumper m_textNode3dDumper = new OdDgTextNode3d_Dumper();
    private OdDgShape2d_Dumper m_shape2dDumper = new OdDgShape2d_Dumper();
    private OdDgShape3d_Dumper m_shape3dDumper = new OdDgShape3d_Dumper();
    private OdDgCurve2d_Dumper m_curve2dDumper = new OdDgCurve2d_Dumper();
    private OdDgCurve3d_Dumper m_curve3dDumper = new OdDgCurve3d_Dumper();
    private OdDgEllipse2d_Dumper m_ellipse2dDumper = new OdDgEllipse2d_Dumper();
    private OdDgEllipse3d_Dumper m_ellipse3dDumper = new OdDgEllipse3d_Dumper();
    private OdDgArc2d_Dumper m_arc2dDumper = new OdDgArc2d_Dumper();
    private OdDgArc3d_Dumper m_arc3dDumper = new OdDgArc3d_Dumper();
    private OdDgCone_Dumper m_coneDumper = new OdDgCone_Dumper();
    private OdDgComplexString_Dumper m_complexStringDumper = new OdDgComplexString_Dumper();
    private OdDgComplexShape_Dumper m_complexShapeDumper = new OdDgComplexShape_Dumper();
    private OdDgPointString2d_Dumper m_pointString2dDumper = new OdDgPointString2d_Dumper();
    private OdDgPointString3d_Dumper m_pointString3dDumper = new OdDgPointString3d_Dumper();
    private OdDgDimension_Dumper m_dimensionDumper = new OdDgDimension_Dumper();
    private OdDgMultiline_Dumper m_multilineDumper = new OdDgMultiline_Dumper();
    private OdDgBSplineCurve2d_Dumper m_bSplineCurve2dDumper = new OdDgBSplineCurve2d_Dumper();
    private OdDgBSplineCurve3d_Dumper m_bSplineCurve3dDumper = new OdDgBSplineCurve3d_Dumper();
    private OdDgSurface_Dumper m_surfaceDumper = new OdDgSurface_Dumper();
    private OdDgSolid_Dumper m_solidDumper = new OdDgSolid_Dumper();
    private OdDgRasterAttachmentHeader_Dumper m_rasterAttachmentHeaderDumper = new OdDgRasterAttachmentHeader_Dumper();
    private OdDgRasterHeader2d_Dumper m_rasterHeader2dDumper = new OdDgRasterHeader2d_Dumper();
    private OdDgRasterHeader3d_Dumper m_rasterHeader3dDumper = new OdDgRasterHeader3d_Dumper();
    private OdDgRasterComponent_Dumper m_rasterComponentDumper = new OdDgRasterComponent_Dumper();
    private OdDgTagElement_Dumper m_tagElementDumper = new OdDgTagElement_Dumper();
    private OdDgCellHeader2d_Dumper m_cellHeader2dDumper = new OdDgCellHeader2d_Dumper();
    private OdDgCellHeader3d_Dumper m_cellHeader3dDumper = new OdDgCellHeader3d_Dumper();
    private OdDgBSplineSurface_Dumper m_bSplineSurfaceDumper = new OdDgBSplineSurface_Dumper();
    private OdDgLevelTable_Dumper m_levelTableDumper = new OdDgLevelTable_Dumper();
    private OdDgLevelTableRecord_Dumper m_levelTableRecordDumper = new OdDgLevelTableRecord_Dumper();
    private OdDgLevelFilterTable_Dumper m_levelFilterTableDumper = new OdDgLevelFilterTable_Dumper();
    private OdDgLevelFilterTableRecord_Dumper m_levelFilterTableRecordDumper = new OdDgLevelFilterTableRecord_Dumper();
    private OdDgFontTable_Dumper m_fontTableDumper = new OdDgFontTable_Dumper();
    private OdDgFontTableRecord_Dumper m_fontTableRecordDumper = new OdDgFontTableRecord_Dumper();
    private OdDgTextStyleTable_Dumper m_textStyleTableDumper = new OdDgTextStyleTable_Dumper();
    private OdDgTextStyleTableRecord_Dumper m_textStyleTableRecordDumper = new OdDgTextStyleTableRecord_Dumper();
    private OdDgDimStyleTable_Dumper m_dimStyleTableDumper = new OdDgDimStyleTable_Dumper();
    private OdDgDimStyleTableRecord_Dumper m_dimStyleTableRecordDumper = new OdDgDimStyleTableRecord_Dumper();
    private OdDgMultilineStyleTable_Dumper m_multilineStyleTableDumper = new OdDgMultilineStyleTable_Dumper();
    private OdDgMultilineStyleTableRecord_Dumper m_multilineStyleTableRecordDumper = new OdDgMultilineStyleTableRecord_Dumper();
    private OdDgLineStyleTable_Dumper m_lineStyleTableDumper = new OdDgLineStyleTable_Dumper();
    private OdDgLineStyleTableRecord_Dumper m_lineStyleTableRecordDumper = new OdDgLineStyleTableRecord_Dumper();
    private OdDgRegAppTable_Dumper m_regAppTableDumper = new OdDgRegAppTable_Dumper();
    private OdDgRegAppTableRecord_Dumper m_regAppTableRecordDumper = new OdDgRegAppTableRecord_Dumper();
    private OdDgViewGroupTable_Dumper m_viewGroupTableDumper = new OdDgViewGroupTable_Dumper();
    private OdDgViewGroup_Dumper m_viewGroupDumper = new OdDgViewGroup_Dumper();
    private OdDgView_Dumper m_viewDumper = new OdDgView_Dumper();
    private OdDgNamedViewTable_Dumper m_namedViewTableDumper = new OdDgNamedViewTable_Dumper();
    private OdDgSharedCellDefinitionTable_Dumper m_sharedCellDefinitionTableDumper = new OdDgSharedCellDefinitionTable_Dumper();
    private OdDgSharedCellDefinition_Dumper m_sharedCellDefinitionDumper = new OdDgSharedCellDefinition_Dumper();
    private OdDgSharedCellReference_Dumper m_sharedCellReferenceDumper = new OdDgSharedCellReference_Dumper();
    private OdDgTagDefinitionSetTable_Dumper m_tagSetDefinitionTableDumper = new OdDgTagDefinitionSetTable_Dumper();
    private OdDgTagDefinitionSet_Dumper m_tagSetDefinitionDumper = new OdDgTagDefinitionSet_Dumper();
    private OdDgTagDefinition_Dumper m_tagDefinitionDumper = new OdDgTagDefinition_Dumper();
    private OdDgColorTable_Dumper m_colorTableDumper = new OdDgColorTable_Dumper();
    private OdDgReferenceHeader_Dumper m_referenceAttachmentDumper = new OdDgReferenceHeader_Dumper();
    private OdDgMeshFaceLoops_Dumper m_meshFaceLoopsDumper = new OdDgMeshFaceLoops_Dumper();
    private OdDgMeshPointCloud_Dumper m_meshPointCloudDumper = new OdDgMeshPointCloud_Dumper();
    private OdDgMeshTriangleList_Dumper m_meshTriangleListDumper = new OdDgMeshTriangleList_Dumper();
    private OdDgMeshQuadList_Dumper m_meshQuadListDumper = new OdDgMeshQuadList_Dumper();
    private OdDgMeshTriangleGrid_Dumper m_meshTriangleGridDumper = new OdDgMeshTriangleGrid_Dumper();
    private OdDgMeshQuadGrid_Dumper m_meshQuadGridDumper = new OdDgMeshQuadGrid_Dumper();
    private OdDgProxyElement_Dumper m_proxyDumper = new OdDgProxyElement_Dumper();
    private OdDgApplicationData_Dumper m_applicationDataDumper = new OdDgApplicationData_Dumper();
  }

}