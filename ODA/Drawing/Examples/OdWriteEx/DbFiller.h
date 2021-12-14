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

#ifndef _OD_DB_FILLER_H_
#define _OD_DB_FILLER_H_

class OdDbPolyFaceMesh;
class OdDbPolygonMesh;
class OdDbGroup;

#include "TextDefs.h"
#include "EntityBoxes.h"
#include "ExPrintConsole.h"


class DbFiller
{
  EntityBoxes       m_EntityBoxes;
  OdDbObjectIdArray m_layoutEntities;
  double            m_textSize;
  OdGeVector3d      m_textOffset, m_textLine;
public:
  DbFiller() : 
    m_textSize(0.2), 
    m_textOffset(OdGeVector3d(0.5 * m_textSize, -0.5 * m_textSize,0)),
    m_textLine(OdGeVector3d(0, -1.6 * m_textSize, 0))
  {}

  void fillDatabase(OdDbDatabase* pDb);

  void appendPfMeshVertex(OdDbPolyFaceMesh* pMesh, double x = 0.0, double y = 0.0, double z = 0.0);

  void appendPgMeshVertex(OdDbPolygonMesh* pPgMesh, OdGePoint3d pos);

  void appendFaceRecord(OdDbPolyFaceMesh* pMesh, OdInt16 i1, OdInt16 i2, OdInt16 i3, OdInt16 i4);

  void addPolyFaceMesh(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addPolygonMesh(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId,const OdDbObjectId& styleId);

  void addCurves(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addTolerance(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addLeaders(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addMLeaders(const OdDbObjectId& btrId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  OdDbObjectId addLayer(OdDbDatabase* pDb, const OdString& layerName, const OdInt16 color, const OdString& linetype);
  OdDbObjectId addDimStyle(OdDbDatabase* pDb, const OdString& dimStyleName);
  

  void addLinetypes(OdDbDatabase* pDb, OdDbObjectId& ltStyleId,  OdDbObjectId& txtStyleId );

  OdDbObjectId addLinetype(OdDbDatabase* pDb, const OdString& name, const OdString& comments);

  bool addRegApp(OdDbDatabase* pDb, const OdString& name);

  OdDbObjectId addBlock(OdDbDatabase* pDb, const OdString& name);


  OdDbObjectId addInsert(OdDbBlockTableRecord* pBlock, const OdDbObjectId& blockId, const OdGeMatrix3d& mTrans);

  OdDbObjectId addBlockDef(OdDbDatabase* pDb, const OdString& name, const int boxRow, const int boxCol);

  void addCustomObjects(OdDbDatabase* pDb);

  void addTextEnts(const OdDbObjectId& blockId, const int boxRow, const int boxCol,const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  OdDbObjectId addTextEnt(OdDbBlockTableRecord* pBlock, const OdGePoint3d& position, const OdGePoint3d& ap, const OdString& str, double height, OdDb::TextHorzMode hMode, OdDb::TextVertMode vMode, const OdDbObjectId& layerId, const OdDbObjectId& styleId, OdDbGroup* pGroup = 0);


  OdDbObjectId addStyle(OdDbDatabase* pDb, const OdString& styleName, double textSize, double xScale, double priorSize, double obliquing, const OdString& fileName, bool isShapeFile = false, const OdString& ttFaceName = OD_T(""), bool bold = false, bool italic = false, int charset = false, int pitchAndFamily = 0);

  void addExtendedData(OdDbObjectId& id);

  void addDimAssoc(const OdDbObjectId& blockId, const int boxRow, const int boxCol, 
    const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addDimBreak(const OdDbObjectId& btrId, const int boxRow, const int boxCol, 
    const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addAlignedDimension(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId, const OdDbObjectId& dimStyleId );

  void addArcDimension(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addRadialDimension(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addDiametricDimension(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addShape(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void add3dFace(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addSolid(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addRay(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addRText(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addHatches(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addMLine(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void add2LineAngularDimension(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void add3PointAngularDimension(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addOrdinateDimensions(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addSpline(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addTraces(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addPolyline(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addArcText(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addWipeout(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addRadialDimensionLarge(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addXline(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addACIS(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

  void addXRef(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& styleId,  const OdDbObjectId& layerId);

OdDbObjectId addPointEnt(OdDbBlockTableRecord* pBlock, const OdGePoint3d& point, const OdDbObjectId& layerId = (OdDbObjectId)0, OdDbGroup* pGroup = 0);

OdResBufPtr appendXDataPair(OdResBufPtr pCurr, int code);

void createEntityBoxes(const OdDbObjectId& blockId, const OdDbObjectId& layerId);
void addPsViewport(OdDbDatabase* pDb, const OdDbObjectId& layerId);

OdDbObjectId addMLineStyle(OdDbDatabase* pDb, const OdString& name, const OdString& desc);

OdDbObjectId addMaterial(OdDbDatabase* pDb, const OdString& name, const OdString& desc);
OdDbObjectId addField(OdDbDatabase* pDb);

void addTable(const OdDbObjectId& blockId, const OdDbObjectId& addedBlockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addLines(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void add2dPolyline(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void add3dPolyline(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addMText(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addBlockRef(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId, const OdDbObjectId& insertId);

  void addMInsert(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId, const OdDbObjectId& insertId);

  void addImage(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addLayout(OdDbDatabase* pDb);

  void addTextWithField(const OdDbObjectId& btrId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId, const OdDbObjectId& noteStyleId);

  void addOLE2FrameFromFile(const OdDbObjectId& btrId, const int boxRow, const int boxCol, const OdString& fileName, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  OdString inCurrentFolder(const OdString fileName);
  
  void addBox(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addFrustum(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addSphere(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addTorus(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  
  void addWedge(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addRegion(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  
  void addExtrusion(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addSolRev(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addHelix(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addDwfUnderlay(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addDgnUnderlay(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addPdfUnderlay(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addLights(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addSubDMeshes(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addSubDMeshesWithDbMaterial(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addExtrudedSurfaces(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addRevolvedSurfaces(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addPlaneSurfaces(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addLoftedSurfaces(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addSweptSurfaces(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addPointCloud(OdDbDatabase* pDb, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);
  void addPointCloudEx(OdDbDatabase* pDb, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addGeoStuff(OdDbDatabase* pDb, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

  void addNURBSurface(const OdDbDatabase* pDb, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId);

#ifdef EMCC
  void fillDatabaseWeb(OdDbDatabase* pDb);
#endif

};

#endif
