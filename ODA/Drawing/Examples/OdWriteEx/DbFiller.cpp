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

#include "OdaCommon.h"
#include "DbDatabase.h"

#include "Db2LineAngularDimension.h"
#include "Db2dPolyline.h"
#include "Db2dVertex.h"
#include "Db3PointAngularDimension.h"
#include "Db3dPolyline.h"
#include "Db3dPolylineVertex.h"
#include "Db3dSolid.h"
#include "DbAlignedDimension.h"
#include "DbArc.h"
#include "DbArcAlignedText.h"
#include "DbArcDimension.h"
#include "DbAttribute.h"
#include "DbAttributeDefinition.h"
#include "DbBlockReference.h"
#include "DbBlockTable.h"
#include "DbBody.h"
#include "DbCircle.h"
#include "DbDiametricDimension.h"
#include "DbDimAssoc.h"
#include "DbBreakData.h"
#include "DbDimStyleTable.h"
#include "DbEllipse.h"
#include "DbFace.h"
#include "DbFaceRecord.h"
#include "DbFcf.h"
#include "DbField.h"
#include "DbGroup.h"
#include "DbHyperlink.h"
#include "DbLayerTable.h"
#include "DbLayout.h"
#include "DbLeader.h"
#include "DbMLeader.h"
#include "DbLine.h"
#include "DbLinetypeTable.h"
#include "DbMaterial.h"
#include "DbMInsertBlock.h"
#include "DbMline.h"
#include "DbMlineStyle.h"
#include "DbMText.h"
#include "DbOrdinateDimension.h"
#include "DbPoint.h"
#include "DbPolyFaceMesh.h"
#include "DbPolyFaceMeshVertex.h"
#include "DbPolygonMesh.h"
#include "DbPolygonMeshVertex.h"
#include "DbPolyline.h"
#include "DbRadialDimension.h"
#include "DbRadialDimensionLarge.h"
#include "DbRasterImage.h"
#include "DbRasterImageDef.h"
#include "DbRasterVariables.h"
#include "DbRay.h"
#include "DbRegion.h"
#include "DbRotatedDimension.h"
#include "DbShape.h"
#include "DbSolid.h"
#include "DbSortentsTable.h"
#include "DbSpline.h"
#include "DbTable.h"
#include "DbText.h"
#include "DbTextStyleTable.h"
#include "DbTrace.h"
#include "DbViewport.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "DbWipeout.h"
#include "DbXline.h"
#include "DbXrecord.h"
#include "RText.h"
#include "DbOle2Frame.h"
#include "DbHelix.h"
#include "DbUnderlayDefinition.h"
#include "DbUnderlayReference.h"
#include "DbLight.h"
#include "DbSubDMesh.h"
#include "DbSubDMeshData.h"
#include "DbExtrudedSurface.h"
#include "DbRevolvedSurface.h"
#include "DbPlaneSurface.h"
#include "DbLoftedSurface.h"
#include "DbSweptSurface.h"
#include "DbPointCloud.h"
#include "DbPointCloudDef.h"
#include "DbPointCloudEx.h"
#include "DbPointCloudDefEx.h"
#include "OdPlatformStreamer.h"

#include "Ge/GeCircArc2d.h"
#include "Ge/GeScale3d.h"
#include "Ge/GeExtents3d.h"
#include "Gi/GiMaterial.h"
#include "Gi/GiRasterWrappers.h"
#include "XRefMan.h"
#include "OleItemHandler.h"

#include "DbFiller.h"
#include "OdFileBuf.h"
#include <math.h>

#include "DbSymUtl.h"
#include "DbHostAppServices.h"
#include "HatchPatternManager.h"
#include "DbAnnotativeObjectPE.h"
#include "DbObjectContextCollection.h"
#include "DbObjectContextManager.h"
#include "DbObjectContextInterface.h"
#include "DbMLeaderBreaks.h"

#include "DbGeoData.h"
#include "DbGeoMap.h"
#include "DbGeoPositionMarker.h"
#include "DbBlockTableRecord.h"
#include "DbViewportTable.h"
#include "DbViewportTableRecord.h"
#include "ExCreateNurbSurfaceData.h"

#include "RcsFileServices/RxRcsFileServices.h"

// added due to release optimization problems in vc13
#if _MSC_VER == 1800
#pragma optimize( "", off )
#endif

/************************************************************************/
/* Define the entity box widths                                         */
/************************************************************************/
const int EntityBoxes::BoxSizes[VER_BOXES][HOR_BOXES] = 
{
  {1,1,1,1,2,1,1,1,1,1,0},
  {1,3,2,1,1,1,2,0,0,0,0},
  {2,3,3,1,2,0,0,0,0,0,0},
  {1,1,1,2,1,1,1,1,1,1,0},
  {2,2,2,1,1,2,1,0,0,0,0},
  {3,2,1,1,1,1,1,1,0,0,0},
  {1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,2,0}
};


void DbFiller::addCustomObjects(OdDbDatabase* pDb)
{
  //Open the main dictionary
  OdDbDictionaryPtr pMain = 
    pDb->getNamedObjectsDictionaryId().safeOpenObject(OdDb::kForWrite);

  // Create the new dictionary.
  OdDbDictionaryPtr pOdtDic = OdDbDictionary::createObject();
  
  // Add new dictionary to the main dictionary.
  OdDbObjectId dicId = pMain->setAt(OD_T("TEIGHA_OBJECTS"), pOdtDic);
  
  // Create a new xrecord object.
  OdDbXrecordPtr pXRec = OdDbXrecord::createObject();

  // Add the xrecord the owning dictionary.
  OdDbObjectId xrId = pOdtDic->setAt(OD_T("PROPERTIES_1"), pXRec);
  
  OdResBufPtr pRb, temp;
  temp = pRb = OdResBuf::newRb(1000);
  temp->setString(OD_T("Sample XRecord Data"));
  
  temp = appendXDataPair(temp, 40);
  temp->setDouble(3.14159);
  
  temp = appendXDataPair(temp, 70);
  temp->setInt16(312);
  
  pXRec->setFromRbChain(pRb);
} //end DbFiller::addCustomObjects

/************************************************************************/
/* Add a Layer to the specified database                                */
/*                                                                      */
/* The symbol table and symbol table record are implicitly closed when  */
/* this function returns.                                               */
/************************************************************************/
OdDbObjectId DbFiller::addLayer(OdDbDatabase* pDb, 
                                const OdString& name, 
                                const OdInt16 color,
                                const OdString& linetype)
{
  /**********************************************************************/
  /* Open the layer table                                               */
  /**********************************************************************/
  OdDbLayerTablePtr pLayers = pDb->getLayerTableId().safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create a layer table record                                        */
  /**********************************************************************/
  OdDbLayerTableRecordPtr pLayer = OdDbLayerTableRecord::createObject();

  /**********************************************************************/
  /* Layer must have a name before adding it to the table.              */
  /**********************************************************************/
  pLayer->setName(name);

  /**********************************************************************/
  /* Set the Color.                                                     */
  /**********************************************************************/
  pLayer->setColorIndex(color);

  /**********************************************************************/
  /* Set the Linetype.                                                  */
  /**********************************************************************/
  OdDbLinetypeTablePtr pLinetypes = pDb->getLinetypeTableId().safeOpenObject(OdDb::kForRead);
  OdDbObjectId linetypeId = pLinetypes->getAt(linetype);
  pLayer->setLinetypeObjectId(linetypeId);

  /**********************************************************************/
  /* Add the record to the table.                                       */
  /**********************************************************************/
  OdDbObjectId layerId = pLayers->add(pLayer);

  return layerId;
} 

/************************************************************************/
/* Add a Registered Application to the specified database               */
/************************************************************************/
bool DbFiller::addRegApp(OdDbDatabase* pDb, 
                         const OdString& name)
{
  return pDb->newRegApp(name);
}

/************************************************************************/
/* Add a Text Style to the specified database                           */
/*                                                                      */
/* The symbol table and symbol table record are implicitly closed when  */
/* this function returns.                                               */
/************************************************************************/
OdDbObjectId DbFiller::addStyle(OdDbDatabase* pDb,
                                const OdString& styleName,
                                double textSize,
                                double xScale,
                                double priorSize,
                                double obliquing,
                                const OdString& fileName,
                                bool isShapeFile,
                                const OdString& ttFaceName,
                                bool bold,
                                bool italic,
                                int charset,
                                int pitchAndFamily)
{
  OdDbObjectId styleId;

  OdDbTextStyleTablePtr pStyles = pDb->getTextStyleTableId().safeOpenObject(OdDb::kForWrite);
  OdDbTextStyleTableRecordPtr pStyle = OdDbTextStyleTableRecord::createObject();

  // Name must be set before a table object is added to a table.  The
  // isShapeFile flag must also be set (if true) before adding the object
  // to the database.
  pStyle->setName(styleName);
  pStyle->setIsShapeFile(isShapeFile);

  // Add the object to the table.
  styleId = pStyles->add(pStyle);

  // Set the remaining properties.
  pStyle->setTextSize(textSize);
  pStyle->setXScale(xScale);
  pStyle->setPriorSize(priorSize);
  pStyle->setObliquingAngle(obliquing);
  pStyle->setFileName(fileName);
  if (isShapeFile)
  {
    pStyle->setPriorSize(22.45);
  }
  if (!ttFaceName.isEmpty())
  {
    pStyle->setFont(ttFaceName, bold, italic, charset, pitchAndFamily);
  }

  return styleId;
}

/************************************************************************/
/* Add a Linetype to the specified database                             */
/*                                                                      */
/* The symbol table and symbol table record are implicitly closed when  */
/* this function returns.                                               */
/************************************************************************/
OdDbObjectId DbFiller::addLinetype(OdDbDatabase* pDb,
                                   const OdString& name,
                                   const OdString& comments)
{
  /**********************************************************************/
  /* Open the Linetype table                                            */
  /**********************************************************************/
  OdDbLinetypeTablePtr pLinetypes = pDb->getLinetypeTableId().safeOpenObject(OdDb::kForWrite);
  OdDbLinetypeTableRecordPtr pLinetype = OdDbLinetypeTableRecord::createObject();

  /**********************************************************************/
  /* Linetype must have a name before adding it to the table.           */
  /**********************************************************************/
  pLinetype->setName(name);

  /**********************************************************************/
  /* Add the record to the table.                                       */
  /**********************************************************************/
  OdDbObjectId linetypeId = pLinetypes->add(pLinetype);

  /**********************************************************************/
  /* Add the Comments.                                                  */
  /**********************************************************************/
  pLinetype->setComments(comments);

  return linetypeId;
}

/************************************************************************/
/* Add Several linetypes to the specified database                      */
/************************************************************************/
void DbFiller::addLinetypes(OdDbDatabase* pDb, 
                            OdDbObjectId& shapeStyleId, 
                            OdDbObjectId& txtStyleId )
{
  /**********************************************************************/
  /* Continuous linetype                                                */
  /**********************************************************************/
  addLinetype(pDb, OD_T("Continuous2"), OD_T("Solid Line"));

  /**********************************************************************/
  /* Hidden linetype                                                    */
  /* This is not the standard Hidden linetype, but is used by examples  */
  /**********************************************************************/
  OdDbObjectId ltId = addLinetype(pDb, OD_T("Hidden"),    OD_T("- - - - - - - - - - - - - - - - - - - - -"));
  OdDbLinetypeTableRecordPtr pLt;
  pLt = ltId.safeOpenObject(OdDb::kForWrite);
  pLt->setNumDashes(2);
  pLt->setPatternLength(0.1875);
  pLt->setDashLengthAt(0, 0.125);
  pLt->setDashLengthAt(1, -0.0625);

  /**********************************************************************/
  /* Linetype with text                                                 */
  /**********************************************************************/
  ltId = addLinetype(pDb, OD_T("HW_ODA"), OD_T("__ HW __ OD __ HW __ OD __"));
  pLt = ltId.safeOpenObject(OdDb::kForWrite);
  pLt->setNumDashes(6);
  pLt->setPatternLength(1.8);
  pLt->setDashLengthAt(0, 0.5);
  pLt->setDashLengthAt(1, -0.2);
  pLt->setDashLengthAt(2, -0.2);
  pLt->setDashLengthAt(3, 0.5);
  pLt->setDashLengthAt(4, -0.2);
  pLt->setDashLengthAt(5, -0.2);

  pLt->setShapeStyleAt(1, txtStyleId );
  pLt->setShapeOffsetAt(1, OdGeVector2d(-0.1, -0.05));
  pLt->setTextAt(1, OD_T("HW"));
  pLt->setShapeScaleAt(1, 0.5);

  pLt->setShapeStyleAt(4, txtStyleId );
  pLt->setShapeOffsetAt(4, OdGeVector2d(-0.1, -0.05));
  pLt->setTextAt(4, OD_T("OD"));
  pLt->setShapeScaleAt(4, 0.5);

  /**********************************************************************/
  /* ZIGZAG linetype                                                    */
  /**********************************************************************/
  ltId = addLinetype(pDb, OD_T("ZigZag"), OD_T("/\\/\\/\\/\\/\\/\\/\\/\\"));
  pLt = ltId.safeOpenObject(OdDb::kForWrite);
  pLt->setNumDashes(4);
  pLt->setPatternLength(0.8001);
  pLt->setDashLengthAt(0, 0.0001);
  pLt->setDashLengthAt(1, -0.2);
  pLt->setDashLengthAt(2, -0.4);
  pLt->setDashLengthAt(3, -0.2);

  pLt->setShapeStyleAt(1, shapeStyleId);
  pLt->setShapeOffsetAt(1, OdGeVector2d(-0.2, 0.0));
  pLt->setShapeNumberAt(1, 131); //ZIG shape
  pLt->setShapeScaleAt(1, 0.2);

  pLt->setShapeStyleAt(2, shapeStyleId);
  pLt->setShapeOffsetAt(2, OdGeVector2d(0.2, 0.0));
  pLt->setShapeNumberAt(2, 131); //ZIG shape
  pLt->setShapeScaleAt(2, 0.2);
  pLt->setShapeRotationAt(2, 3.1415926);
}

/************************************************************************/
/* Add a block definition to the specified database                     */
/*                                                                      */
/* Note that the BlockTable and BlockTableRecord are implicitly closed  */
/* when before this function returns.                                   */
/************************************************************************/
OdDbObjectId DbFiller::addBlock(OdDbDatabase* pDb, 
                                const OdString& name)
{
  OdDbObjectId            id;
  OdDbBlockTablePtr       pTable  = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  OdDbBlockTableRecordPtr pRecord = OdDbBlockTableRecord::createObject();
  
  /**********************************************************************/
  /* Block must have a name before adding it to the table.              */
  /**********************************************************************/
  pRecord->setName(name);
  
  /**********************************************************************/
  /* Add the record to the table.                                       */
  /**********************************************************************/
  id = pTable->add(pRecord);
  return id;
}

/************************************************************************/
/* Add a block reference to the specified BlockTableRecord              */
/************************************************************************/
OdDbObjectId DbFiller::addInsert(OdDbBlockTableRecord* bBTR,
                                 const OdDbObjectId& btrId,
                                 const OdGeMatrix3d& mTransform)
{
  OdDbObjectId brefId;
  
  /**********************************************************************/
  /* Add the block reference to the BlockTableRecord                    */
  /**********************************************************************/
  OdDbBlockReferencePtr pBlkRef = OdDbBlockReference::createObject();
  pBlkRef->setDatabaseDefaults(bBTR->database());
  brefId = bBTR->appendOdDbEntity(pBlkRef);

  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pBlkRef->setBlockTableRecord(btrId);
  pBlkRef->setBlockTransform(mTransform);
  return brefId;
}

/************************************************************************/
/* Add a text entity with the specified attributes to the specified     */
/* BlockTableRecord                                                     */
/************************************************************************/
OdDbObjectId DbFiller::addTextEnt(OdDbBlockTableRecord* bBTR,
                                  const OdGePoint3d& position,
                                  const OdGePoint3d& ap,
                                  const OdString& str,
                                  double height,
                                  OdDb::TextHorzMode hMode,
                                  OdDb::TextVertMode vMode,
                                  const OdDbObjectId& layerId,
                                  const OdDbObjectId& styleId,
                                  OdDbGroup* pGroup)
{
  /**********************************************************************/
  /* Create the text object                                             */
  /**********************************************************************/
  OdDbTextPtr pText = OdDbText::createObject();
  pText->setDatabaseDefaults(bBTR->database());
  OdDbObjectId textId = bBTR->appendOdDbEntity(pText);
  
  /**********************************************************************/
  /* Add the text to the specified group                                */
  /**********************************************************************/
  if (pGroup)
  {
    pGroup->append(textId);
  }

  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pText->setPosition(position);
  pText->setAlignmentPoint(ap);
  pText->setHeight(height);
  pText->setWidthFactor(1.0);
  pText->setTextString(str);
  pText->setHorizontalMode(hMode);
  pText->setVerticalMode(vMode);

  /**********************************************************************/
  /* Set the text to the specified style                                */
  /**********************************************************************/
  if (!styleId.isNull())
  {
    pText->setTextStyle(styleId);
  }
  /**********************************************************************/
  /* Set the text to the specified layer                                */
  /**********************************************************************/
  if (!layerId.isNull())
  {
    pText->setLayer(layerId, false);
  }
  
  return textId;
}

/************************************************************************/
/* Add a point entity with the specified attributes to the specified    */
/* BlockTableRecord                                                     */
/************************************************************************/
OdDbObjectId DbFiller::addPointEnt(OdDbBlockTableRecord* bBTR,
                                   const OdGePoint3d& point,
                                   const OdDbObjectId& layerId,
                                   OdDbGroup* pGroup)
{
  /**********************************************************************/
  /* Create the point object                                             */
  /**********************************************************************/
  OdDbPointPtr pPoint = OdDbPoint::createObject();
  pPoint->setDatabaseDefaults(bBTR->database());
  OdDbObjectId pointId = bBTR->appendOdDbEntity(pPoint);
  
  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pPoint->setPosition(point);

  /**********************************************************************/
  /* Add the point to the specified group                               */
  /**********************************************************************/
  if (pGroup)
  {
    pGroup->append(pointId);
  }
  /**********************************************************************/
  /* Set the point to the specified layer                               */
  /**********************************************************************/
  if (!layerId.isNull())
  {
    pPoint->setLayer(layerId, false);
  }
  return pointId;
}

/************************************************************************/
/* Add some text entities to the specified BlockTableRecord             */
/*                                                                      */
/* The newly created entities are placed in a group                     */
/************************************************************************/  
void DbFiller::addTextEnts(const OdDbObjectId& btrId,
                           const int boxRow,
                           const int boxCol,
                           const OdDbObjectId& layerId,
                           const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  // We want to place all text items into a newly created group, so
  // open the group dictionary here.
  
  /**********************************************************************/
  /* Open the Group Dictionary                                          */
  /**********************************************************************/
  OdDbDictionaryPtr pGroupDic = btrId.database()->getGroupDictionaryId().safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create a new Group                                                 */
  /**********************************************************************/
  OdDbGroupPtr pGroup = OdDbGroup::createObject();

  /**********************************************************************/
  /* Add it to the Group Dictionary                                     */
  /**********************************************************************/
  pGroupDic->setAt(OD_T("OdaGroup"), pGroup);

  /**********************************************************************/
  /* Set some properties                                                 */
  /**********************************************************************/
  pGroup->setName(OD_T("OdaGroup"));
  pGroup->setSelectable(true);

  /**********************************************************************/
  /* Get the Lower-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);

  double dx = w/16.0;
  double dy = h/12.0;

  double textHeight = m_EntityBoxes.getHeight()/12.0;

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("TEXT"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Add the text entities, and add them to the group                   */
  /*                                                                    */
  /* Show the relevant positions and alignment points                   */
  /**********************************************************************/
  OdGePoint3d position = point + OdGeVector3d(dx, dy*9.0, 0.0);
  addPointEnt(bBTR, position, layerId, pGroup);
  addTextEnt(bBTR, position, position, 
    OD_T("Left Text"), textHeight, OdDb::kTextLeft, OdDb::kTextBase, (OdDbObjectId)0, styleId, pGroup);

  OdGePoint3d alignmentPoint = point + OdGeVector3d(w/2.0, dy*9.0, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Center Text"), textHeight, OdDb::kTextCenter, OdDb::kTextBase, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w-dx, dy*9.0, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Right Text"), textHeight, OdDb::kTextRight, OdDb::kTextBase, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w/2.0, dy*8.0, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Middle Text"), textHeight, OdDb::kTextMid, OdDb::kTextBase, (OdDbObjectId)0, styleId, pGroup);

  position        = point + OdGeVector3d(dx, dy*1, 0.0);
  alignmentPoint  = point + OdGeVector3d(w-dx, dy, 0.0);
  addPointEnt(bBTR, position, layerId, pGroup);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, position, alignmentPoint, 
    OD_T("Aligned Text"), textHeight, OdDb::kTextAlign, OdDb::kTextBase, (OdDbObjectId)0, styleId, pGroup);

  position        = point + OdGeVector3d(dx, dy*5.5, 0.0);
  alignmentPoint  = point + OdGeVector3d(w-dx, dy*5.5, 0.0);
  addPointEnt(bBTR, position, layerId, pGroup);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, position, alignmentPoint, 
    OD_T("Fit Text"), textHeight, OdDb::kTextFit, OdDb::kTextBase, (OdDbObjectId)0, styleId, pGroup);

    
  /**********************************************************************/
  /* Start a new box                                                    */
  /**********************************************************************/
  point = m_EntityBoxes.getBox(boxRow, boxCol + 1);

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("TEXT"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;
  textHeight = h/16.0;

  /**********************************************************************/
  /* Create a new anonymous Group                                                 */
  /**********************************************************************/
  pGroup = OdDbGroup::createObject();

  /**********************************************************************/
  /* Add it to the Group Dictionary                                     */
  /**********************************************************************/
  pGroupDic->setAt(OD_T("*"), pGroup);

  /**********************************************************************/
  /* Set some properties                                                 */
  /**********************************************************************/
  pGroup->setName(OD_T("*"));
  pGroup->setAnonymous();
  pGroup->setSelectable(true);

  /**********************************************************************/
  /* Add the text entities, and add them to the group                   */
  /*                                                                    */
  /* Show the relevant positions and alignment points                   */
  /**********************************************************************/
  alignmentPoint = point + OdGeVector3d(dx, dy*9.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Top Left"), textHeight, OdDb::kTextLeft, OdDb::kTextTop, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w/2.0, dy*9.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Top Center"), textHeight, OdDb::kTextCenter, OdDb::kTextTop, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w-dx, dy*9.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Top Right"), textHeight, OdDb::kTextRight, OdDb::kTextTop, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(dx, dy*7.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Middle Left"), textHeight, OdDb::kTextLeft, OdDb::kTextVertMid, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w/2.0, dy*7.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Middle Center"), textHeight, OdDb::kTextCenter, OdDb::kTextVertMid, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w-dx, dy*7.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Middle Right"), textHeight, OdDb::kTextRight, OdDb::kTextVertMid, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(dx, dy*5.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Baseline Left"), textHeight, OdDb::kTextLeft, OdDb::kTextBase, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w/2.0, dy*5.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Baseline Center"), textHeight, OdDb::kTextCenter, OdDb::kTextBase, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w-dx, dy*5.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Baseline Right"), textHeight, OdDb::kTextRight, OdDb::kTextBase, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(dx, dy*3.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Bottom Left"), textHeight, OdDb::kTextLeft, OdDb::kTextBottom, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w/2.0, dy*3.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Bottom Center"), textHeight, OdDb::kTextCenter, OdDb::kTextBottom, (OdDbObjectId)0, styleId, pGroup);

  alignmentPoint = point + OdGeVector3d(w-dx, dy*3.5, 0.0);
  addPointEnt(bBTR, alignmentPoint, layerId, pGroup);
  addTextEnt(bBTR, alignmentPoint, alignmentPoint, 
    OD_T("Bottom Right"), textHeight, OdDb::kTextRight, OdDb::kTextBottom, (OdDbObjectId)0, styleId, pGroup);
}

/************************************************************************/
/* Append a PolygonMesh vertex to the specified PolygonMesh             */
/************************************************************************/
void DbFiller::appendPgMeshVertex(OdDbPolygonMesh* pPgMesh,
                                  OdGePoint3d pos)
{
  /**********************************************************************/
  /* Append a Vertex to the PolyFaceMesh                                */
  /**********************************************************************/
  OdDbPolygonMeshVertexPtr pVertex = OdDbPolygonMeshVertex::createObject();
  pPgMesh->appendVertex(pVertex);
  
  /**********************************************************************/
  /* Set the properties                                                 */
  /**********************************************************************/
  pVertex->setPosition(pos);
}

/************************************************************************/
/* Append a PolyFaceMesh vertex to the specified PolyFaceMesh           */
/************************************************************************/
void DbFiller::appendPfMeshVertex(OdDbPolyFaceMesh* pMesh,
                                  double x,
                                  double y,
                                  double z)
{
  /**********************************************************************/
  /* Append a MeshVertex to the PolyFaceMesh                            */
  /**********************************************************************/
  OdDbPolyFaceMeshVertexPtr pVertex = OdDbPolyFaceMeshVertex::createObject();
  pMesh->appendVertex(pVertex);
  
  /**********************************************************************/
  /* Set the properties                                                 */
  /**********************************************************************/
  pVertex->setPosition(OdGePoint3d(x, y, z));
}

/************************************************************************/
/* Append a FaceRecord to the specified PolyFaceMesh                    */
/************************************************************************/
void DbFiller::appendFaceRecord(OdDbPolyFaceMesh* pMesh,
                                OdInt16 i1,
                                OdInt16 i2,
                                OdInt16 i3,
                                OdInt16 i4)
{
  /**********************************************************************/
  /* Append a FaceRecord to the PolyFaceMesh                            */
  /**********************************************************************/
  OdDbFaceRecordPtr pFr = OdDbFaceRecord::createObject();
  pMesh->appendFaceRecord(pFr);
  
  /**********************************************************************/
  /* Set the properties                                                 */
  /**********************************************************************/
  pFr->setVertexAt(0, i1);
  pFr->setVertexAt(1, i2);
  pFr->setVertexAt(2, i3);
  pFr->setVertexAt(3, i4);
}

/************************************************************************/
/* Add an MLine Style to the specified database                         */
/************************************************************************/
OdDbObjectId DbFiller::addMLineStyle(OdDbDatabase* pDb, 
                                     const OdString& name, 
                                     const OdString& desc)
{
  /**********************************************************************/
  /* Open the MLineStyle dictionary                                     */
  /**********************************************************************/
  OdDbDictionaryPtr pMLDic = pDb->getMLStyleDictionaryId().safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create an Mline Style                                              */
  /**********************************************************************/
  OdDbMlineStylePtr pStyle = OdDbMlineStyle::createObject();
  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/
  pStyle->setName(name);
  pStyle->setDescription(desc);
  pStyle->setStartAngle(OdaToRadian(105.0));
  pStyle->setEndAngle(OdaToRadian(75.0));
  pStyle->setShowMiters(true);
  pStyle->setStartSquareCap(true);
  pStyle->setEndSquareCap(true);

  /**********************************************************************/
  /* Get the object ID of the desired linetype                          */
  /**********************************************************************/
  OdDbLinetypeTablePtr pLtTable = pDb->getLinetypeTableId().safeOpenObject();
  OdDbObjectId linetypeId = pLtTable->getAt(OD_T("Hidden"));

  OdCmColor color;

  /**********************************************************************/
  /* Add some elements                                                  */
  /**********************************************************************/
  color.setRGB(255, 0,0);
  pStyle->addElement(0.1, color, linetypeId);
  color.setRGB(0, 255, 0);
  pStyle->addElement(0.0, color, linetypeId);

  /**********************************************************************/
  /* Update the MLine dictionary                                        */
  /**********************************************************************/
  return pMLDic->setAt(name, pStyle);
}
/************************************************************************/
/* Add a Material to the specified database                             */
/************************************************************************/
OdDbObjectId DbFiller::addMaterial(OdDbDatabase* pDb, 
                                     const OdString& name, 
                                     const OdString& desc)
{
  /**********************************************************************/
  /* Open the Material dictionary                                     */
  /**********************************************************************/
  OdDbDictionaryPtr pMatDic = pDb->getMaterialDictionaryId().safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create a Material                                                  */
  /**********************************************************************/
  OdDbMaterialPtr pMaterial = OdDbMaterial::createObject();
  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/
  pMaterial->setName(name);
  pMaterial->setDescription(desc);
  
  OdGiMaterialColor materialColor;
  materialColor.setMethod(OdGiMaterialColor::kOverride);
  materialColor.setFactor(0.75);
  materialColor.color()=OdCmEntityColor(192, 32, 255);

  OdGiMaterialMap materialMap;
  materialMap.setBlendFactor(0.0);
  materialMap.setSource(OdGiMaterialMap::kFile);
  
  pMaterial->setAmbient(materialColor);
  pMaterial->setBump(materialMap);
  pMaterial->setDiffuse(materialColor, materialMap);
  pMaterial->setOpacity(1.0, materialMap);
  pMaterial->setReflection(materialMap);
  pMaterial->setRefraction(1.0, materialMap);
  pMaterial->setTranslucence(0.0);
  pMaterial->setSelfIllumination(0.0);
  pMaterial->setReflectivity(0.0);
  pMaterial->setMode(OdGiMaterialTraits::kRealistic);
  pMaterial->setChannelFlags(OdGiMaterialTraits::kNone);
  pMaterial->setIlluminationModel(OdGiMaterialTraits::kBlinnShader);

  materialColor.setFactor(1.0);
  materialColor.color()=OdCmEntityColor(255, 255, 255);
  pMaterial->setSpecular(materialColor, materialMap, 0.67);  
  /**********************************************************************/
  /* Update the Material dictionary                                        */
  /**********************************************************************/
  return pMatDic->setAt(name, pMaterial);
}
/************************************************************************/
/* Add some lines to the specified BlockTableRecord                     */
/************************************************************************/
void DbFiller::addLines(const OdDbObjectId& btrId,
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId)
{

  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
//  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("LINEs"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the center of the box                                          */
  /**********************************************************************/
  point = m_EntityBoxes.getBoxCenter(0,0);
  
  /**********************************************************************/
  /* Add the lines that describe a 12 pointed star                      */
  /**********************************************************************/
  OdGeVector3d toStart(1.0, 0.0, 0.0);

  for (int i = 0; i < 12; i++)
  {
    OdDbLinePtr pLine = OdDbLine::createObject();
    pLine->setDatabaseDefaults(bBTR->database());
    bBTR->appendOdDbEntity(pLine);
    pLine->setStartPoint(point + toStart);
    pLine->setEndPoint(point + toStart.rotateBy(OdaToRadian(160.0), OdGeVector3d::kZAxis));
  }
}

/************************************************************************/
/* Add a 2D (heavy) polyline to the specified BlockTableRecord          */
/************************************************************************/
void DbFiller::add2dPolyline(const OdDbObjectId& btrId,
                             const int boxRow,
                             const int boxCol,
                             const OdDbObjectId& layerId,
                             const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("2D POLYLINE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Add a 2dPolyline to the database                                   */
  /**********************************************************************/
  OdDb2dPolylinePtr pPline = OdDb2dPolyline::createObject();
  pPline->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPline);
 
  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  OdGePoint3d pos = point;
  pos.y -= h;
  pos.x += w/8;
  pos.y += h/8;

  const double width[2][4] = 
  {
    {0.0, w/12, w/4, 0.0},
    {w/4, w/12, 0.0, 0.0}
  };
  
  for (int i = 0; i < 4; i++)
  {
    OdDb2dVertexPtr pVertex = OdDb2dVertex::createObject();
    pVertex->setDatabaseDefaults(bBTR->database());
    pPline->appendVertex(pVertex);
    pVertex->setPosition(pos);
    pos.x += w/4.0;
    pos.y += h/4.0;
    pVertex->setStartWidth (width[0][i]);
    pVertex->setEndWidth   (width[1][i]);
  }
}

/************************************************************************/
/* Add a 3D polyline to the specified BlockTableRecord                  */
/************************************************************************/
void DbFiller::add3dPolyline(const OdDbObjectId& btrId,
                             const int boxRow,
                             const int boxCol,
                             const OdDbObjectId& layerId,
                             const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
//  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("3D POLYLINE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Add a 3dPolyline to the database                                   */
  /**********************************************************************/
  OdDb3dPolylinePtr pPline = OdDb3dPolyline::createObject();
  pPline->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPline);
 
  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  OdGePoint3d pos = point;
  OdGePoint3d center  = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
 
  double radius       = w * 3.0 / 8.0;
  double height       = 0.0;
  double theta        = 0.0;
  
  int turns           = 4;
  int segs            = 16;
  int points          = segs * turns;

  double deltaR       = radius / points;
  double deltaTheta   = Oda2PI / segs;
  double deltaH       = 2 * radius / points;
  
  OdGeVector3d vec(radius, 0, 0);
   
  for (int i = 0; i < points; i++)
  {
    OdDb3dPolylineVertexPtr pVertex = OdDb3dPolylineVertex::createObject();
    pVertex->setDatabaseDefaults(bBTR->database());
    pPline->appendVertex(pVertex);
    pVertex->setPosition(center + vec);
    
    radius    -= deltaR;
    height    += deltaH;
    theta     += deltaTheta;
    vec       = OdGeVector3d(radius, 0, height).rotateBy(theta,OdGeVector3d::kZAxis);
  }
}
/************************************************************************/
/* Add MText to the specified BlockTableRecord                          */
/************************************************************************/
void DbFiller::addMText(const OdDbObjectId& btrId,
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("MTEXT"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Add MText to the database                                          */
  /**********************************************************************/
  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pMText);
 
  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pMText->setLocation(point + OdGeVector3d( w/8.0, -h*2.0/8.0, 0 ));
  pMText->setTextHeight(0.4);
  pMText->setAttachment(OdDbMText::kTopLeft);
  pMText->setContents(OD_T("Sample {\\C1;MTEXT} created by {\\C5;OdWriteEx}"));
  pMText->setWidth(w * 6.0 / 8.0);
  pMText->setTextStyle(styleId);


  /**********************************************************************/
  /* Add annotation scales                                              */
  /**********************************************************************/
  OdDbAnnotativeObjectPEPtr(pMText)->setAnnotative(pMText, true);
  OdDbObjectContextCollection* contextCollection = bBTR->database()->objectContextManager()->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION);
  OdDbAnnotationScalePtr scale1 = OdDbAnnotationScale::createObject();
  scale1->setName(L"MyScale");
  scale1->setDrawingUnits(1.0);
  scale1->setPaperUnits(4.0);
  contextCollection->addContext(scale1);
  // add custom scale
  OdDbObjectContextInterfacePtr(pMText)->addContext(pMText, *scale1);
  // add predefined scale
  OdDbDatabase* pDb = btrId.database();
  if (pDb->getMEASUREMENT() == OdDb::kEnglish)
    OdDbObjectContextInterfacePtr(pMText)->addContext(pMText, *contextCollection->getContext(L"6\" = 1\'-0\""));
  else
    OdDbObjectContextInterfacePtr(pMText)->addContext(pMText, *contextCollection->getContext(L"1:2"));
}

/************************************************************************/
/* Add a Block Reference to the specified BlockTableRecord              */
/************************************************************************/
void DbFiller::addBlockRef(const OdDbObjectId& btrId,
                           const int boxRow,
                           const int boxCol,
                           const OdDbObjectId& layerId,
                           const OdDbObjectId& styleId,
                           const OdDbObjectId& insertId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
//  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("INSERT"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Create a transformation matrix for the block and attributes        */
  /**********************************************************************/
  OdGePoint3d insPoint  = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  OdGeMatrix3d blkXfm;
  blkXfm.setTranslation(insPoint.asVector());

  /**********************************************************************/
  /* Insert the Block                                                   */
  /**********************************************************************/
  OdDbObjectId bklRefId = addInsert(bBTR, insertId, blkXfm);

  /**********************************************************************/
  /* Open the insert                                                    */
  /**********************************************************************/
  OdDbBlockReferencePtr pBlkRef = bklRefId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Scan the block definition for non-constant attribute definitions   */
  /* and use them as templates for attributes                           */
  /**********************************************************************/
  OdDbBlockTableRecordPtr pBlockDef = insertId.safeOpenObject();
  OdDbObjectIteratorPtr pIter       =  pBlockDef->newIterator();
  OdDbEntityPtr                     pEntity;
  OdDbAttributeDefinitionPtr        pAttDef;
   
	for(pIter->start(); !pIter->done(); pIter->step())
	{
		pEntity = pIter->entity();
 		pAttDef = OdDbAttributeDefinition::cast(pEntity);
		if((!pAttDef.isNull()) && (!pAttDef->isConstant()))
		{
      OdDbAttributePtr pAtt = OdDbAttribute::createObject();
      pBlkRef->appendAttribute(pAtt);

      /******************************************************************/
      /* Copy Attribute Definition properties                           */
      /* and transform it as the block was transformed                  */
      /******************************************************************/
      pAtt->setAttributeFromBlock(pAttDef, blkXfm);

      /******************************************************************/
      /* Specify a new value for the attribute                          */
      /******************************************************************/
      pAtt->setTextString     (L"The Value");
		}
	}
}
/************************************************************************/
/* Add a MInsert to the specified BlockTableRecord                      */
/************************************************************************/
void DbFiller::addMInsert(const OdDbObjectId& btrId,
                           const int boxRow,
                           const int boxCol,
                           const OdDbObjectId& layerId,
                           const OdDbObjectId& styleId,
                           const OdDbObjectId& insertId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("MInsert"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Add MInsert to the database                                        */
  /**********************************************************************/
  OdDbMInsertBlockPtr pMInsert = OdDbMInsertBlock::createObject();
  pMInsert->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pMInsert);

  /**********************************************************************/
  /* Set some Parameters                                                */
  /**********************************************************************/
  pMInsert->setBlockTableRecord(insertId);
  OdGePoint3d insPnt = point + OdGeVector3d(w * 2.0/8.0, h * 2.0/8.0, 0.0);
  pMInsert->setPosition(insPnt);
  pMInsert->setScaleFactors(OdGeScale3d(2.0/8.0));
  pMInsert->setRows(2);
  pMInsert->setColumns(3);
  pMInsert->setRowSpacing(h * 4.0/8.0);
  pMInsert->setColumnSpacing(w * 2.0/8.0);
}

/************************************************************************/
/* Add a PolyFaceMesh to the specified BlockTableRecord                 */
/************************************************************************/
void DbFiller::addPolyFaceMesh(const OdDbObjectId& btrId,
                               const int boxRow,
                               const int boxCol,
                               const OdDbObjectId& layerId,
                               const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("PolyFaceMesh"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Add a PolyFaceMesh to the database                                 */
  /**********************************************************************/
  OdDbPolyFaceMeshPtr pMesh = OdDbPolyFaceMesh::createObject();
  pMesh->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pMesh);
 
 
  /**********************************************************************/
  /* Add the faces and vertices that define a pup tent                  */                                                  /**********************************************************************/

  double dx = w * 3.0 / 8.0;
  double dy = h * 3.0 / 8.0;
  double dz = dy;
   
  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  appendPfMeshVertex(pMesh, center.x+dx, center.y+dy);
  appendPfMeshVertex(pMesh, center.x+0,  center.y+dy, center.z+dz);
  appendPfMeshVertex(pMesh, center.x-dx, center.y+dy);
  appendPfMeshVertex(pMesh, center.x-dx, center.y-dy);
  appendPfMeshVertex(pMesh, center.x+0,  center.y-dy,  center.z+dz);
  appendPfMeshVertex(pMesh, center.x+dx, center.y-dy);

  appendFaceRecord(pMesh, 1, 2, 5, 6);
  appendFaceRecord(pMesh, 2, 3, 4, 5);
  appendFaceRecord(pMesh, 6, 5, 4, 0);
  appendFaceRecord(pMesh, 3, 2, 1, 0);
}

/************************************************************************/
/* Add PolygonMesh to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addPolygonMesh(const OdDbObjectId& btrId,
                              const int boxRow,
                              const int boxCol,
                              const OdDbObjectId& layerId,
                              const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("PolygonMesh"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Add a PolygonMesh to the database                                 */
  /**********************************************************************/
  OdDbPolygonMeshPtr pMesh = OdDbPolygonMesh::createObject();
  pMesh->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pMesh);
 
  /**********************************************************************/
  /* Define the size of the mesh                                        */
  /**********************************************************************/
  OdInt16 mSize = 16, nSize = 4;
  pMesh->setMSize(mSize);
  pMesh->setNSize(nSize);


  /**********************************************************************/
  /* Define a profile                                                   */
  /**********************************************************************/
  double dx = w * 3.0 / 8.0;
  double dy = h * 3.0 / 8.0;

  OdGeVector3dArray vectors;
  vectors.setLogicalLength(nSize);
  
  vectors[0] = OdGeVector3d (0,  -dy, 0);
  vectors[1] = OdGeVector3d (dx, -dy, 0);
  vectors[2] = OdGeVector3d (dx,  dy, 0);
  vectors[3] = OdGeVector3d (0,  dy, 0);
    
  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  /**********************************************************************/
  /* Append the vertices to the mesh                                    */
  /**********************************************************************/
  for (int i = 0; i < mSize; i++)
  {
    for (int j = 0; j < nSize; j++)
    {
      appendPgMeshVertex(pMesh, center + vectors[j]);
      vectors[j].rotateBy(OdaToRadian(360.0 / mSize), OdGeVector3d::kYAxis);
    }
  }
  pMesh->makeMClosed();
}

/************************************************************************/
/* Add some curves to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addCurves(const OdDbObjectId& btrId,
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Create a Circle                                                    */
  /**********************************************************************/
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pCircle);

  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  center.x -= w * 2.5 / 8.0;
  pCircle->setCenter(center);
  pCircle->setRadius(w * 1.0 / 8.0); 

  /**********************************************************************/
  /* Add a Hyperlink to the Circle                                      */
  /**********************************************************************/
  OdDbEntityHyperlinkPEPtr    hpe = pCircle;
  OdDbHyperlinkCollectionPtr  urls = hpe->getHyperlinkCollection(pCircle);
  
  urls->addTail(L"http://forum.opendesign.com/forumdisplay.php?s=&forumid=17", 
    L"Open Design Alliance Forum > Teigha, C++ version");
  
  hpe->setHyperlinkCollection(pCircle, urls);
  
  ODA_ASSERT(hpe->hasHyperlink(pCircle));

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  double textY = point.y - m_textSize/2.0;

  addTextEnt(bBTR, 
    OdGePoint3d(center.x, textY, 0), OdGePoint3d(center.x, textY, 0), 
    OD_T("CIRCLE"), m_textSize, OdDb::kTextCenter, OdDb::kTextTop, layerId, styleId );
  addTextEnt(bBTR, 
    OdGePoint3d(center.x, textY - 1.6 * m_textSize, 0), OdGePoint3d(center.x, textY - 1.6 * m_textSize, 0), 
    OD_T("w/Hyperlink"), m_textSize, OdDb::kTextCenter, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Create an Arc                                                      */
  /**********************************************************************/
  OdDbArcPtr pArc = OdDbArc::createObject();
  pArc->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pArc);

  pArc->setRadius(w * 1.0 / 8.0);
  
  center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
 
  center.y += pArc->radius() / 2.0;

  pArc->setCenter(center);
  pArc->setStartAngle(OdaToRadian(0.));
  pArc->setEndAngle(OdaToRadian(180.));

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, 
    OdGePoint3d(center.x, textY, 0),OdGePoint3d(center.x, textY, 0), 
    OD_T("ARC"), m_textSize, OdDb::kTextCenter, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Add an Ellipse                                                     */
  /**********************************************************************/
  OdDbEllipsePtr pEllipse = OdDbEllipse::createObject();
  pEllipse->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pEllipse);

  double majorRadius  = w * 1.0 / 8.0;
  double radiusRatio  = 0.25;

  center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  center.x += w * 2.5 / 8.0;
  center.y += majorRadius;

  OdGeVector3d majorAxis(majorRadius, 0.0, 0.0);
  majorAxis.rotateBy(OdaToRadian(30.0), OdGeVector3d::kZAxis);
  
  pEllipse->set(center, OdGeVector3d::kZAxis, majorAxis, radiusRatio);

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, 
    OdGePoint3d(center.x, textY, 0),OdGePoint3d(center.x, textY, 0), 

    OD_T("ELLIPSE"), m_textSize, OdDb::kTextCenter, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Add a Point                                                        */
  /**********************************************************************/
  OdDbPointPtr pPoint = OdDbPoint::createObject();
  pPoint->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPoint);

  center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  center.y -= h * 1.0 / 8.0;
  
  pPoint->setPosition(center);

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  center.y += h * 1.0 / 8.0;
  addTextEnt(bBTR, center, center, 
    OD_T("POINT"), m_textSize, OdDb::kTextCenter, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Set the point display mode so we can see it                        */
  /**********************************************************************/
  pDb->database()->setPDMODE(3);
  pDb->database()->setPDSIZE(0.1);
}
/************************************************************************/
/* Add a tolerance to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addTolerance(const OdDbObjectId& btrId,
                            const int boxRow,
                            const int boxCol,
                            const OdDbObjectId& layerId,
                            const OdDbObjectId& styleId)
{

  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("TOLERANCE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );


  /**********************************************************************/
  /* Add a Frame Control Feature (Tolerance) to the database            */
  /**********************************************************************/
  OdDbFcfPtr pTol = OdDbFcf::createObject();
  pTol->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pTol);

  /**********************************************************************/
  /* Set the properties                                                 */
  /**********************************************************************/
  pTol->setDatabaseDefaults(pDb);
  point.x += w/6.0;
  point.y -= h/4.0;
  pTol->setLocation(point);
  pTol->setText(OD_T("{\\Fgdt;r}%%v{\\Fgdt;n}3.2{\\Fgdt;m}%%v%%v%%v%%v"));
}

/************************************************************************/
/* Add some leaders the specified BlockTableRecord                      */
/************************************************************************/
void DbFiller::addLeaders(const OdDbObjectId& btrId,
                          const int boxRow,
                          const int boxCol,
                          const OdDbObjectId& layerId,
                          const OdDbObjectId& styleId)
{

  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
      OD_T("LEADERs"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Define an annotation block -- A circle with radius 0.5             */
  /**********************************************************************/
  OdDbBlockTablePtr       pBlocks  = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  OdDbBlockTableRecordPtr pAnnoBlock = OdDbBlockTableRecord::createObject();
  pAnnoBlock->setName(OD_T("AnnoBlock"));
  OdDbObjectId annoBlockId = pBlocks->add(pAnnoBlock);
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());
  pAnnoBlock->appendOdDbEntity(pCircle);
  OdGePoint3d center(0.5, 0, 0);
  pCircle->setCenter(center);
  pCircle->setRadius(0.5); 

  /**********************************************************************/
  /* Add a leader with database defaults to the database                */
  /**********************************************************************/
  OdDbLeaderPtr pLeader = OdDbLeader::createObject();
  pLeader->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pLeader);
  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  point.x += w * 1.0 / 8.0;
  point.y -= h * 3.0 / 8.0;
  pLeader->appendVertex(point);
  point.x += w * 2.0 / 8.0;
  point.y += h * 1.0 / 8.0;
  pLeader->appendVertex(point);

  /**********************************************************************/
  /* Insert the annotation                                              */
  /**********************************************************************/
  OdDbBlockReferencePtr pBlkRef = OdDbBlockReference::createObject();
  OdDbObjectId blkRefId = bBTR->appendOdDbEntity(pBlkRef);
  pBlkRef->setBlockTableRecord(annoBlockId);
  pBlkRef->setPosition(point);
  pBlkRef->setScaleFactors(OdGeScale3d (0.375, 0.375, 0.375));
  
  /**********************************************************************/
  /* Attach the Block Reference as annotation to the Leader             */
  /**********************************************************************/
  pLeader->attachAnnotation(blkRefId);

  /**********************************************************************/
  /* Add a leader with database defaults to the database                */
  /**********************************************************************/
  pLeader = OdDbLeader::createObject();
  pLeader->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pLeader);

  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  point = m_EntityBoxes.getBox(boxRow,boxCol);
  point.x += w * 1.0 / 8.0;
  point.y -= h * 5.0 / 8.0;
  pLeader->appendVertex(point);
  point.x += w * 1.0 / 8.0;
  point.y += h * 1.0 / 8.0;
  pLeader->appendVertex(point);
  point.x += w * 1.0 / 8;

  /**********************************************************************/
  /* Set the arrowhead                                                  */
  /**********************************************************************/
  pLeader->setDimldrblk(OD_T("DOT"));

  /**********************************************************************/
  /* Create MText at a 30° angle                                        */
  /**********************************************************************/
  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(pDb);
  OdDbObjectId mTextId = bBTR->appendOdDbEntity(pMText);
  const double textHeight = 0.15;
  const double textWidth = 1.0;
  pMText->setLocation(point);
  pMText->setRotation(OdaToRadian(10.0));
  pMText->setTextHeight(textHeight);
  pMText->setWidth(textWidth);
  pMText->setAttachment(OdDbMText::kMiddleLeft);
  pMText->setContents(OD_T("MText"));
  pMText->setTextStyle(styleId);

  /**********************************************************************/
  /* Set a background color                                             */
  /**********************************************************************/
  OdCmColor cBackground;
  cBackground.setRGB(255, 255, 0); // Yellow
  pMText->setBackgroundFillColor(cBackground);
  pMText->setBackgroundFill(true);
  pMText->setBackgroundScaleFactor(2.);

  /**********************************************************************/
  /* Attach the MText as annotation to the Leader                       */
  /**********************************************************************/
  pLeader->attachAnnotation(mTextId);

  /**********************************************************************/
  /* Add a leader with database defaults to the database                */
  /**********************************************************************/
  pLeader = OdDbLeader::createObject();
  bBTR->appendOdDbEntity(pLeader);
  pLeader->setDatabaseDefaults(pDb);


  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  point = m_EntityBoxes.getBox(boxRow,boxCol);
  point.x += w * 1.0 / 8.0;
  point.y -= h * 7.0 / 8.0;
  pLeader->appendVertex(point);
  point.x += w * 1.0 / 8.0;
  point.y += h * 1.0 / 8.0;
  pLeader->appendVertex(point);

  /**********************************************************************/
  /* Create a Frame Control Feature (Tolerance)                         */
  /**********************************************************************/
  OdDbFcfPtr pTol = OdDbFcf::createObject();
  pTol->setDatabaseDefaults(pDb);
  pTol->setLocation(point);
  pTol->setText(OD_T("{\\Fgdt;r}%%v{\\Fgdt;n}3.2{\\Fgdt;m}%%v%%v%%v%%v"));

  /**********************************************************************/
  /* Attach the FCF as annotation to the Leader                         */
  /**********************************************************************/
  pLeader->attachAnnotation(bBTR->appendOdDbEntity(pTol));
}


/************************************************************************/
/* Add some MLeaders the specified BlockTableRecord                     */
/************************************************************************/
void DbFiller::addMLeaders(const OdDbObjectId& btrId,
                          const int boxRow,
                          const int boxCol,
                          const OdDbObjectId& layerId,
                          const OdDbObjectId& styleId)
{
  int  llIndex;

  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(
      bBTR, 
      point + m_textOffset, point + m_textOffset,
      OD_T("MLeaders"), 
      m_textSize, 
      OdDb::kTextLeft, OdDb::kTextTop, 
      layerId, styleId );

  /**********************************************************************/
  /* Add a MLeader with database defaults to the database               */
  /**********************************************************************/
  OdDbMLeaderPtr pMLeader = OdDbMLeader::createObject();
  pMLeader->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pMLeader);

  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(pDb);
  pMLeader->setEnableFrameText( true );
  pMText->setContents( OD_T("MText") );

  const double textHeight = 0.15;
  const double textWidth = 1.0;

  point.x += w * 3.0 / 8.0;
  point.y -= h * 1.0 / 6.0;
  pMText->setLocation(point);
//  pMText->setRotation(OdaToRadian(10.0));
  pMText->setTextHeight(textHeight);
  pMText->setWidth(textWidth);
  pMText->setAttachment(OdDbMText::kMiddleLeft);
  pMText->setTextStyle(styleId);
  pMLeader->setMText( pMText );
  pMLeader->setDoglegLength( 0.18 );

  point.x -= w * 2.0 / 8.0;
  point.y -= h * 1.0 / 8.0;
  pMLeader->addLeaderLine( point, llIndex );
  point.x += w * 1.0 / 8.0;
//  point.y -= h * 3.0 / 8.0;
  pMLeader->addLeaderLine( point, llIndex );
  OdGePoint3d nextPoint = point;
  point.x += w * 1.0 / 6.0;
  point.y -= h * 1.0 / 8.0;
  pMLeader->addFirstVertex( llIndex, point );

  OdGeVector3d d = nextPoint - point;
  OdArray<OdDbMLeaderBreaks::BreakInfo> breaks;
  breaks.resize(1);
  breaks[0].m_nSegment = 0;
  breaks[0].m_StartPoints.append(point + d*0.4);
  breaks[0].m_EndPoints.append(point + d*0.6);
  OdDbMLeaderBreaks::setBreaks(pMLeader, llIndex, breaks);


  point.x += w * 3.0 / 8.0;
  point.y -= h * 1.0 / 8.0;
  pMLeader->addLeaderLine( point, llIndex );

  OdDbAnnotativeObjectPEPtr(pMLeader)->setAnnotative(pMLeader, true);
  if (pDb->getMEASUREMENT() == OdDb::kEnglish)
  {
    OdDbObjectContextInterfacePtr(pMLeader)->addContext(pMLeader,
      *pDb->objectContextManager()->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION)->getContext(L"6\" = 1\'-0\""));
  }
  else
  {
    OdDbObjectContextInterfacePtr(pMLeader)->addContext(pMLeader,
      *pDb->objectContextManager()->contextCollection(ODDB_ANNOTATIONSCALES_COLLECTION)->getContext(L"1:2"));
  }

  /**********************************************************************/
  /* Add block with attribute for second MLeader contents               */
  /**********************************************************************/
  OdDbObjectId contentBlockId;
  {
    OdDbBlockTablePtr pBlocks = OdDbBlockTable::cast(pDb->getBlockTableId().openObject(OdDb::kForWrite));
    OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::createObject();
    pBlock->setName(OD_T("AnnoBlock2"));
    contentBlockId = pBlocks->add(pBlock);
    OdDbCirclePtr pCircle = OdDbCircle::createObject();
    pCircle->setDatabaseDefaults(pDb);
    pBlock->appendOdDbEntity(pCircle);
    pCircle->setCenter(OdGePoint3d::kOrigin);
    pCircle->setRadius(0.5);
    OdDbAttributeDefinitionPtr pAttr = OdDbAttributeDefinition::createObject();
    pAttr->setDatabaseDefaults(pDb);
    pBlock->appendOdDbEntity(pAttr);
    pAttr->setPrompt(OD_T("Enter Attr: "));
    pAttr->setTag(OD_T("Attr"));
    pAttr->setHorizontalMode(OdDb::kTextCenter);
    pAttr->setHeight(0.2);
    pAttr->setTextString(OD_T("Block"));
  }

  /**********************************************************************/
  /* Add block for second MLeader arrow heads                           */
  /**********************************************************************/
  OdDbObjectId arrowBlockId;
  {
    OdDbBlockTablePtr pBlocks = OdDbBlockTable::cast(pDb->getBlockTableId().openObject(OdDb::kForWrite));
    OdDbBlockTableRecordPtr pBlock = OdDbBlockTableRecord::createObject();
    pBlock->setName(OD_T("ArrowBlock2"));
    pBlock->setComments(OD_T("Block for MLeader arrow heads."));
    arrowBlockId = pBlocks->add(pBlock);
    OdDbPolylinePtr pPl = OdDbPolyline::createObject();
    pPl->setDatabaseDefaults(pDb);
    pBlock->appendOdDbEntity(pPl);
    pPl->addVertexAt(0, OdGePoint2d(0.0, -0.5));
    pPl->addVertexAt(1, OdGePoint2d(0.5, 0.0));
    pPl->addVertexAt(2, OdGePoint2d(0.0, 0.5));
    pPl->addVertexAt(3, OdGePoint2d(-0.5, 0.0));
    pPl->addVertexAt(4, OdGePoint2d(0.0, -0.5));
    pPl->transformBy(OdGeMatrix3d::scaling(2.0));
  }

  /**********************************************************************/
  /* Create style for second MLeader                                    */
  /**********************************************************************/
  pMLeader = OdDbMLeader::createObject();
  OdDbObjectId styleId2;
  {
    OdDbDictionaryPtr pMLeaderStyleDic = pDb->getMLeaderStyleDictionaryId().safeOpenObject(OdDb::kForWrite);
    OdDbMLeaderStylePtr pMLeaderStyle = OdDbMLeaderStyle::createObject();
    pMLeaderStyle->setDatabaseDefaults(pDb);
    pMLeaderStyle->setContentType(OdDbMLeaderStyle::kBlockContent);
    styleId2 = pMLeaderStyleDic->setAt(OD_T("BlockMLeaderStyle2"), pMLeaderStyle);

    pMLeaderStyle->setBlockId(contentBlockId);
    pMLeaderStyle->setBlockScale(OdGeScale3d(0.5, 0.5, 1.0));
    pMLeaderStyle->setArrowSymbolId(arrowBlockId);
    pMLeaderStyle->setDoglegLength(0.1);
    pMLeaderStyle->setLandingGap(0.0);
    //pMLeaderStyle->setArrowSize(0.5);
  }

  /**********************************************************************/
  /* Set second MLeader vertices                                        */
  /**********************************************************************/
  pMLeader->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pMLeader);
  pMLeader->setMLeaderStyle(styleId2);
  pMLeader->setBlockPosition(OdGePoint3d(12.2, 1.0, 0.0));

  pMLeader->addLeaderLine(OdGePoint3d(12.0, 1.5, 0.0), llIndex);
  pMLeader->addFirstVertex(llIndex, OdGePoint3d(12.7, 1.9, 0.0));
  pMLeader->addFirstVertex(llIndex, OdGePoint3d(12.3, 2.3, 0.0));

  pMLeader->addLeaderLine(OdGePoint3d(13.0, 1.65, 0.0), llIndex);
  pMLeader->addFirstVertex(llIndex, OdGePoint3d(13.0, 2.3, 0.0));

  pMLeader->setArrowSize(pMLeader->arrowSize() * 0.5);

  /**********************************************************************/
  /* Create style for third MLeader                                    */
  /**********************************************************************/
  OdDbObjectId styleId3;
  {
    OdDbDictionaryPtr pMLeaderStyleDic = pDb->getMLeaderStyleDictionaryId().safeOpenObject(OdDb::kForWrite);
    OdDbMLeaderStylePtr pMLeaderStyle = OdDbMLeaderStyle::createObject();
    pMLeaderStyle->setDatabaseDefaults(pDb);
    pMLeaderStyle->setContentType(OdDbMLeaderStyle::kNoneContent);
    styleId3 = pMLeaderStyleDic->setAt(OD_T("NoneMLeaderStyle"), pMLeaderStyle);
  }
  pMLeader = OdDbMLeader::createObject();
  pMLeader->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pMLeader);
  pMLeader->setMLeaderStyle(styleId3);
  point = m_EntityBoxes.getBox(boxRow, boxCol);
  point.x += w * 3.0 / 4.0;
  point.y -= h * 3.0 / 4.0;
  pMLeader->addLeaderLine(point, llIndex);
  point.x += w * 2.0 / 10.0;
  point.y -= h * 2.0 / 10.0;
  pMLeader->addFirstVertex(llIndex, point);
}

/************************************************************************/
/* Add some lights to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addLights(const OdDbObjectId& btrId,
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);

  /**********************************************************************/
  /* Create a Light                                                     */
  /**********************************************************************/
  OdDbLightPtr pLight = OdDbLight::createObject();
  pLight->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pLight);

  const OdGePoint3d ptLight = m_EntityBoxes.getBoxCenter(boxRow,boxCol);
  pLight->setPosition(ptLight);
  pLight->setLightType(OdGiDrawable::kPointLight);

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Light"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
}

/************************************************************************/
/* Add some SubDMeshes to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addSubDMeshes(const OdDbObjectId& btrId,
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Create a SubDMesh                                                     */
  /**********************************************************************/
  OdDbSubDMeshPtr pSubDMesh = OdDbSubDMesh::createObject();
  pSubDMesh->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSubDMesh);

  OdGePoint3dArray vertexArray;
  OdInt32Array faceArray;
  OdGeExtents3d ext;
  setDbSubDMeshData(vertexArray, faceArray, ext);

  pSubDMesh->setSubDMesh(vertexArray, faceArray, 0);

  OdGeMatrix3d xfm;
  const double scaleX = w*0.7/(ext.maxPoint().x - ext.minPoint().x);
  const double scaleY = h*0.7/(ext.maxPoint().y - ext.minPoint().y);
  xfm.setToScaling(odmin(scaleX, scaleY), ext.center());
  pSubDMesh->transformBy( xfm );
  xfm.setToTranslation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector() - ext.center().asVector());
  pSubDMesh->transformBy( xfm );

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("SubDMesh"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
}


/**************************************************************************/
/* Add plane SubDMeshes with DbMaterial to the specified BlockTableRecord */
/**************************************************************************/
void DbFiller::addSubDMeshesWithDbMaterial(const OdDbObjectId& blockId, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = blockId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Create a SubDMesh                                                     */
  /**********************************************************************/
  OdDbSubDMeshPtr pSubDMesh = OdDbSubDMesh::createObject();
  pSubDMesh->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSubDMesh);

  OdGePoint3dArray vertexArray;
  vertexArray.append(OdGePoint3d(0, 0, 0));
  vertexArray.append(OdGePoint3d(1000, 0, 0));
  vertexArray.append(OdGePoint3d(1000, 1000, 0));
  vertexArray.append(OdGePoint3d(0, 1000, 0));
  OdInt32Array faceArray;
  faceArray.append(4);
  faceArray.append(0);
  faceArray.append(1);
  faceArray.append(2);
  faceArray.append(3);

  OdGeExtents3d ext;
  for (unsigned int i = 0; i < vertexArray.size(); ++i)
  {
    ext.addPoint(vertexArray[i]);
  }

  pSubDMesh->setSubDMesh(vertexArray, faceArray, 0);

  OdGeMatrix3d xfm;
  const double scaleX = w * 0.7 / (ext.maxPoint().x - ext.minPoint().x);
  const double scaleY = h * 0.7 / (ext.maxPoint().y - ext.minPoint().y);
  xfm.setToScaling(odmin(scaleX, scaleY), ext.center());
  pSubDMesh->transformBy(xfm);
  xfm.setToTranslation(m_EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
  pSubDMesh->transformBy(xfm);

  //
  // Adding wood material
  //
  OdDbDatabasePtr pDb = bBTR->database();
  OdDbDictionaryPtr pMatDic = pDb->getMaterialDictionaryId().safeOpenObject(OdDb::kForWrite);

  OdGiImageFileTexturePtr tex = OdGiImageFileTexture::createObject();
  tex->setSourceFileName("wood.jpg");// set here path to image

  OdDbMaterialPtr pMaterial = OdDbMaterial::createObject();
  pMaterial->setName("Test material 1");
  OdGiMapper mapper;
  mapper.setProjection(OdGiMapper::kPlanar);

  OdGiMaterialMap map;
  map.setSourceFileName(OdString("wood.jpg")); // set here path to image
  map.setTexture(tex);
  map.setMapper(mapper);

  OdGiMaterialColor diffuseColor;
  pMaterial->setDiffuse(diffuseColor, map);
  OdDbObjectId matId = pMatDic->setAt("Wood material", pMaterial);

  //
  // Set texture coordinates
  //
  OdGePoint3dArray arrTextureSet;
  arrTextureSet.append(OdGePoint3d(0, 0, 0));
  arrTextureSet.append(OdGePoint3d(1, 0, 0));
  arrTextureSet.append(OdGePoint3d(1, 1, 0));
  arrTextureSet.append(OdGePoint3d(0, 1, 0));
  pSubDMesh->setVertexTextureArray(arrTextureSet);

  pSubDMesh->setMaterial("Wood material");
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset,
    OD_T("DbSubDMesh"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);

  addTextEnt(bBTR,
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine,
    OD_T("with Material"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);
}

/************************************************************************/
/* Add some ExtrudedSurfaces to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addExtrudedSurfaces(const OdDbObjectId& btrId,
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset,
    OD_T("Extruded surf"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);

  // base curve 
  OdDbEllipse ellipse;
  ellipse.set(OdGePoint3d(0., 0., 0.), OdGeVector3d(0., 0., 1.), OdGeVector3d(2., 0., 0.), 0.5);

  /**********************************************************************/
  /* Create extruded surface                                                     */
  /**********************************************************************/
  try
  {
    OdStreamBufPtr pFile = odSystemServices()->createFile(OD_T("extrudedsurf.sat"));
    OdDbExtrudedSurfacePtr pExtruded = OdDbExtrudedSurface::createObject();
    OdDbSweepOptions options;
    pExtruded->createExtrudedSurface(&ellipse, OdGeVector3d(0., 1., 3.), options, pFile);
    pExtruded->setDatabaseDefaults(bBTR->database());
    bBTR->appendOdDbEntity(pExtruded);
    OdGeExtents3d ext;
    pExtruded->getGeomExtents(ext);
    OdGeMatrix3d xfm;
    xfm.setToTranslation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector() - ext.center().asVector());
    pExtruded->transformBy(xfm);
    const double scaleX = w*0.7/(ext.maxPoint().x - ext.minPoint().x);
    const double scaleY = h*0.7/(ext.maxPoint().y - ext.minPoint().y);
    xfm.setToScaling(odmin(scaleX, scaleY), m_EntityBoxes.getBoxCenter(boxRow,boxCol));
    pExtruded->transformBy( xfm );
  }
  catch(OdError_FileException &)
  {
    // just skip entity creation
  }
}

/************************************************************************/
/* Add some RevolvedSurfaces to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addRevolvedSurfaces(const OdDbObjectId& btrId,
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset,
    OD_T("Revolved surf"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);

  // base curve 
  OdDbEllipse ellipse;
  ellipse.set(OdGePoint3d(0., 0., 0.), OdGeVector3d(0., 0., 1.), OdGeVector3d(2., 0., 0.), 0.5);

  /**********************************************************************/
  /* Create revolved surface                                                     */
  /**********************************************************************/
  try
  {
    OdStreamBufPtr pFile = odSystemServices()->createFile(OD_T("revolvedsurf.sat"));
    OdDbRevolvedSurfacePtr pRevolved = OdDbRevolvedSurface::createObject();
    OdDbRevolveOptions options;
    pRevolved->createRevolvedSurface(&ellipse, OdGePoint3d(5, 0, 0), OdGeVector3d(0, 1, 0), 3.14, 0, options, pFile);
    pRevolved->setDatabaseDefaults(bBTR->database());
    bBTR->appendOdDbEntity(pRevolved);
    OdGeExtents3d ext;
    pRevolved->getGeomExtents(ext);
    OdGeMatrix3d xfm;
    xfm.setToTranslation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector() - ext.center().asVector());
    pRevolved->transformBy(xfm);
    const double scaleX = w*0.7/(ext.maxPoint().x - ext.minPoint().x);
    const double scaleY = h*0.7/(ext.maxPoint().y - ext.minPoint().y);
    xfm.setToScaling(odmin(scaleX, scaleY), m_EntityBoxes.getBoxCenter(boxRow,boxCol));
    pRevolved->transformBy( xfm );

  }
  catch(OdError_FileException &)
  {
    // just skip entity creation
  }
}

/************************************************************************/
/* Add some PlaneSurfaces to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addPlaneSurfaces(const OdDbObjectId& btrId,
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  // base curve 
  OdDbEllipse ellipse;
  ellipse.set(OdGePoint3d(0., 0., 0.), OdGeVector3d(0., 0., 1.), OdGeVector3d(2., 0., 0.), 0.5);

  /**********************************************************************/
  /* Create plane surface                                                     */
  /**********************************************************************/
  OdDbPlaneSurfacePtr pPlane = OdDbPlaneSurface::createObject();
  OdRxObjectPtrArray curveSegments;
  curveSegments.setLogicalLength(1);
  curveSegments[0] = &ellipse;
  OdRxObjectPtrArray regions;
  OdDbRegion::createFromCurves(curveSegments, regions);
  pPlane->createFromRegion(OdDbRegion::cast(regions[0]));
  pPlane->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPlane);
  OdGeExtents3d ext;
  pPlane->getGeomExtents(ext);
  OdGeMatrix3d xfm;
  xfm.setToTranslation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector() - ext.center().asVector());
  pPlane->transformBy(xfm);
  const double scaleX = w*0.7/(ext.maxPoint().x - ext.minPoint().x);
  const double scaleY = h*0.7/(ext.maxPoint().y - ext.minPoint().y);
  xfm.setToScaling(odmin(scaleX, scaleY), m_EntityBoxes.getBoxCenter(boxRow,boxCol));
  pPlane->transformBy( xfm );

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Plane surf"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
}

/************************************************************************/
/* Add some LoftedSurfaces to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addLoftedSurfaces(const OdDbObjectId& btrId,
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset,
    OD_T("Lofted surf"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);

  // base curve 
  OdDbEllipse ellipse;
  ellipse.set(OdGePoint3d(0., 0., 0.), OdGeVector3d(0., 0., 1.), OdGeVector3d(2., 0., 0.), 0.5);

  /**********************************************************************/
  /* Create lofted surface                                                     */
  /**********************************************************************/
  try
  {
    OdStreamBufPtr pFile = odSystemServices()->createFile(OD_T("loftedsurf.sat"));
    OdDbLoftedSurfacePtr pLofted = OdDbLoftedSurface::createObject();
    OdDbEntityPtrArray crossSectionCurves;
    crossSectionCurves.setLogicalLength(2);
    crossSectionCurves[0] = &ellipse;
    OdGeMatrix3d mat;
    mat.setToScaling(0.5);
    ellipse.getTransformedCopy(mat, crossSectionCurves[1]);
    mat.setToTranslation(OdGeVector3d(0., 0., 3.));
    crossSectionCurves[1]->transformBy(mat);
    OdDbEntityPtrArray guideCurves;
    OdDbLoftOptions options;
    pLofted->createLoftedSurface(crossSectionCurves, guideCurves, NULL, options, pFile);
    pLofted->setDatabaseDefaults(bBTR->database());
    bBTR->appendOdDbEntity(pLofted);
    OdGeExtents3d ext;
    pLofted->getGeomExtents(ext);
    OdGeMatrix3d xfm;
    xfm.setToTranslation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector() - ext.center().asVector());
    pLofted->transformBy(xfm);
    const double scaleX = w*0.7/(ext.maxPoint().x - ext.minPoint().x);
    const double scaleY = h*0.7/(ext.maxPoint().y - ext.minPoint().y);
    xfm.setToScaling(odmin(scaleX, scaleY), m_EntityBoxes.getBoxCenter(boxRow,boxCol));
    pLofted->transformBy( xfm );
  }
  catch(OdError_FileException &)
  {
    // just skip entity creation
  }
}

/************************************************************************/
/* Add some SweptSurfaces to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addSweptSurfaces(const OdDbObjectId& btrId,
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Get the origin and size of the box                                 */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double      w     = m_EntityBoxes.getWidth(boxRow, boxCol);
  double      h     = m_EntityBoxes.getHeight();

  /**********************************************************************/
  /* Add a label                                                        */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset,
    OD_T("Swept surf"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);

  // base curve 
  OdDbEllipse ellipse;
  ellipse.set(OdGePoint3d(0., 0., 0.), OdGeVector3d(0., 0., 1.), OdGeVector3d(2., 0., 0.), 0.5);

  /**********************************************************************/
  /* Create swept surface                                                     */
  /**********************************************************************/
  try
  {
    OdStreamBufPtr pFile = odSystemServices()->createFile(OD_T("sweptsurf.sat"));
    OdDbSweptSurfacePtr pSwept = OdDbSweptSurface::createObject();
    OdDbSweepOptions options;
    OdDb3dPolylineVertex aVx[3];
    aVx[0].setPosition(OdGePoint3d(0., 0., 0.));
    aVx[1].setPosition(OdGePoint3d(0.5, 0., 2.));
    aVx[2].setPosition(OdGePoint3d(-0.5, 0., 4.));
    OdDb3dPolyline poly;
    poly.appendVertex(&aVx[0]);
    poly.appendVertex(&aVx[1]);
    poly.appendVertex(&aVx[2]);
    pSwept->createSweptSurface(&ellipse, &poly, options, pFile);
    pSwept->setDatabaseDefaults(bBTR->database());
    bBTR->appendOdDbEntity(pSwept);
    OdGeExtents3d ext;
    pSwept->getGeomExtents(ext);
    OdGeMatrix3d xfm;
    xfm.setToTranslation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector() - ext.center().asVector());
    pSwept->transformBy(xfm);
    const double scaleX = w*0.7/(ext.maxPoint().x - ext.minPoint().x);
    const double scaleY = h*0.7/(ext.maxPoint().y - ext.minPoint().y);
    xfm.setToScaling(odmin(scaleX, scaleY), m_EntityBoxes.getBoxCenter(boxRow,boxCol));
    pSwept->transformBy( xfm );
  }
  catch(OdError_FileException &)
  {
    // just skip entity creation
  }
}

/************************************************************************/
/* Add a Block Definition to the specified database                     */
/************************************************************************/
OdDbObjectId DbFiller::addBlockDef(OdDbDatabase* pDb,
                                   const OdString& name,
                                   const int boxRow,
                                   const int boxCol)
{
  /**********************************************************************/
  /* Open the block table                                               */
  /**********************************************************************/
  OdDbBlockTablePtr       pBlocks  = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Create a BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = OdDbBlockTableRecord::createObject();
  
  /**********************************************************************/
  /* Block must have a name before adding it to the table.              */
  /**********************************************************************/
  bBTR->setName(name);
  
  /**********************************************************************/
  /* Add the record to the table.                                       */
  /**********************************************************************/
  OdDbObjectId btrId = pBlocks->add(bBTR);
//  double       h       = m_EntityBoxes.getHeight();
  double       w       = m_EntityBoxes.getWidth(boxRow, boxCol);

  /**********************************************************************/
  /* Add a Circle                                                       */
  /**********************************************************************/
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pCircle);

  OdGePoint3d center(0, 0, 0);
  center.x -= w * 2.5 / 8.0;
  
  pCircle->setCenter(center);
  pCircle->setRadius(w * 1.0 / 8.0); 

  /**********************************************************************/
  /* Add an Arc                                                         */
  /**********************************************************************/
  OdDbArcPtr pArc = OdDbArc::createObject();
  pArc->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pArc);

  pArc->setRadius(w * 1.0 / 8.0);
  
  center = OdGePoint3d(0, 0, 0);
  center.y -= pArc->radius() / 2.0;

  pArc->setCenter(center);
  pArc->setStartAngle(OdaToRadian(0.));
  pArc->setEndAngle(OdaToRadian(180.));

  /**********************************************************************/
  /* Add an Ellipse                                                     */
  /**********************************************************************/
  OdDbEllipsePtr pEllipse = OdDbEllipse::createObject();
  pEllipse->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pEllipse);

  center = OdGePoint3d(0, 0, 0);
  center.x += w * 2.5 / 8.0;

  double majorRadius  = w * 1.0 / 8.0;
  OdGeVector3d majorAxis(majorRadius, 0.0, 0.0);
  majorAxis.rotateBy(OdaToRadian(30.0), OdGeVector3d::kZAxis);

  double radiusRatio  = 0.25;
  
  pEllipse->set(center, OdGeVector3d::kZAxis, majorAxis, radiusRatio);

  /**********************************************************************/
  /* Add an Attdef                                                      */
  /**********************************************************************/
  OdDbAttributeDefinitionPtr pAttDef = OdDbAttributeDefinition::createObject();
  pAttDef->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pAttDef);

  pAttDef->setPrompt(OD_T("Enter ODT_ATT: "));
  pAttDef->setTag(OD_T("Oda_ATT"));
  pAttDef->setHorizontalMode(OdDb::kTextCenter);
  pAttDef->setHeight(0.1);
  pAttDef->setTextString(OD_T("Default"));

  /**********************************************************************/
  /* Return the ObjectId of the BlockTableRecord                        */
  /**********************************************************************/
  return btrId;
}

/************************************************************************/
/* Append an XData Pair to the specified ResBuf                         */
/************************************************************************/
OdResBufPtr DbFiller::appendXDataPair(OdResBufPtr pCurr, 
                                      int code)
{
  pCurr->setNext(OdResBuf::newRb(code));
  return pCurr->next();
}

void DbFiller::addExtendedData(OdDbObjectId& id)
{
  OdDbObjectPtr pObject = id.safeOpenObject(OdDb::kForWrite);
  OdResBufPtr xIter = OdResBuf::newRb(1001);

  OdResBufPtr temp = xIter;
  temp->setString(OD_T("ODA"));  // Application

  temp = appendXDataPair(temp, 1000);
  temp->setString(OD_T("Extended Data for ODA app"));

  temp = appendXDataPair(temp, 1000);
  temp->setString(OD_T("Double"));

  temp = appendXDataPair(temp, 1041);
  temp->setDouble(5.2);

  pObject->setXData(xIter);
}

// Adds an external reference called OD_T("XRefBlock") to the passed in database,
// which references the file OD_T("xref.dwg").

/************************************************************************/
/* Add an XRef to the specified BlockTableRecord                        */
/************************************************************************/
void DbFiller::addXRef(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("XREF INSERT"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Open the block table                                               */
  /**********************************************************************/
  OdDbBlockTablePtr       pBlocks  = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Create a BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr pXRef = OdDbXRefManExt::addNewXRefDefBlock(pDb, OD_T("OdWriteEx XRef.dwg"), OD_T("XRefBlock"), false);

  /**********************************************************************/
  /* Insert the Xref                                                    */
  /**********************************************************************/
  OdDbObjectId xRefId = addInsert(bBTR, pXRef->objectId(), OdGeMatrix3d::kIdentity);
  
  /**********************************************************************/
  /* Open the insert                                                    */
  /**********************************************************************/
  OdDbBlockReferencePtr pXRefIns = xRefId.safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Set the insertion point                                            */
  /**********************************************************************/
  pXRefIns->setPosition(point);

  /**********************************************************************/
  /* Move\Scale XREF to presentation rectangle                          */
  /**********************************************************************/

  OdGeExtents3d extents;
  if (pXRefIns->getGeomExtents(extents) == eOk && extents.isValidExtents())
  {
    double dScale = odmin(w / (extents.maxPoint().x - extents.minPoint().x),  h * (7./8.) / (extents.maxPoint().y - extents.minPoint().y));
    pXRefIns->setScaleFactors(OdGeScale3d(dScale, dScale, 1));
    pXRefIns->setPosition(point - dScale*(extents.minPoint() - point.asVector()).asVector());
  }
}


/************************************************************************/
/* Populate the Database                                                */
/*                                                                      */
/* PaperSpace Viewports                                                 */
/* Linetypes with embedded shapes, and custom linetypes                 */
/*                                                                      */
/************************************************************************/
void DbFiller::fillDatabase(OdDbDatabase* pDb)
{
  
  odPrintConsoleString(L"\n\nPopulating the database...");

  /**********************************************************************/
  /* Set Creation and Last Update times                                 */
  /**********************************************************************/
  OdDbDate date;

  date.setDate(1, 1, 2006);
  date.setTime(12, 0, 0, 0); 
  date.localToUniversal();
  odDbSetTDUCREATE(*pDb, date);

  date.getUniversalTime();
  odDbSetTDUUPDATE(*pDb, date);

  /**********************************************************************/
  /* Add some Registered Applications                                   */
  /**********************************************************************/
  addRegApp(pDb, OD_T("ODA"));
  addRegApp(pDb, OD_T("AVE_FINISH")); // for materials
  /**********************************************************************/
  /* Add an SHX text style                                              */
  /**********************************************************************/
  OdDbObjectId shxTextStyleId = addStyle(pDb, OD_T("OdaShxStyle"), 0.0, 1.0, 0.2, 0.0, OD_T("txt"));

  /**********************************************************************/
  /* Add a TTF text style                                               */
  /**********************************************************************/
  OdDbObjectId ttfStyleId = addStyle(pDb, OD_T("OdaTtfStyle"), 0.0, 1.0, 0.2, 0.0, 
      OD_T("VERDANA.TTF"), false, OD_T("Verdana"), false, false, 0, 34);
  
  /**********************************************************************/
  /* Add a Shape file style for complex linetypes                       */
  /**********************************************************************/
  OdDbObjectId shapeStyleId = addStyle(pDb, OD_T(""), 0.0, 1.0, 0.2, 0.0, OD_T("ltypeshp.shx"), true);

  /**********************************************************************/
  /* Add some linetypes                                                 */
  /**********************************************************************/  
  addLinetypes(pDb, shapeStyleId, shxTextStyleId);

  /**********************************************************************/
  /* Add a Layer                                                        */
  /**********************************************************************/
  OdDbObjectId odaLayer1Id = addLayer(pDb, OD_T("Oda Layer 1"), OdCmEntityColor::kACIRed, OD_T("CONTINUOUS"));

  /**********************************************************************/
  /* Add a block definition                                             */
  /**********************************************************************/ 
  OdDbObjectId odaBlock1Id = addBlockDef(pDb, OD_T("OdaBlock1"), 1, 2); 

  /**********************************************************************/
  /* Add a DimensionStyle                                               */
  /**********************************************************************/ 
  OdDbObjectId odaDimStyleId = addDimStyle(pDb, OD_T("OdaDimStyle")); 

  /**********************************************************************/
  /* Add an MLine style                                                 */
  /**********************************************************************/ 
  OdDbObjectId odaMLineStyleId = addMLineStyle(pDb, OD_T("OdaStandard"), OD_T("ODA Standard Style")); 

  /**********************************************************************/
  /* Add a Material                                                     */
  /**********************************************************************/ 
  OdDbObjectId odaMaterialId = addMaterial(pDb, OD_T("OdaMaterial"), OD_T("ODA Defined Material")); 
  
  /**********************************************************************/
  /* Add a PaperSpace Viewport                                          */
  /**********************************************************************/
  addPsViewport(pDb, odaLayer1Id);

  /**********************************************************************/
  /* Add ModelSpace Entity Boxes                                        */
  /**********************************************************************/  
  createEntityBoxes(pDb->getModelSpaceId(), odaLayer1Id);

  /**********************************************************************/
  /* Add some lines                                                     */
  /**********************************************************************/ 
  addLines(pDb->getModelSpaceId(), 0, 0, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a 2D (heavy) polyline                                          */
  /**********************************************************************/  
  add2dPolyline(pDb->getModelSpaceId(), 0, 1, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a PolyFace Mesh                                                */
  /**********************************************************************/  
  addPolyFaceMesh(pDb->getModelSpaceId(), 0, 2, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a PolygonMesh                                                */
  /**********************************************************************/ 
  addPolygonMesh(pDb->getModelSpaceId(), 0, 3, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add some curves                                                    */
  /**********************************************************************/ 
  addCurves(pDb->getModelSpaceId(), 0, 4, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Tolerance                                                    */
  /**********************************************************************/ 
  addTolerance(pDb->getModelSpaceId(), 0, 5, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add some Leaders                                                   */
  /**********************************************************************/ 
  addLeaders(pDb->getModelSpaceId(), 0, 6, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add an Aligned Dimension                                           */
  /**********************************************************************/  
  addAlignedDimension(pDb->getModelSpaceId(), 0, 7, odaLayer1Id, ttfStyleId, odaDimStyleId);

  /**********************************************************************/
  /* Add a MultiLine                                                    */
  /**********************************************************************/ 
  addMLine(pDb->getModelSpaceId(), 0, 8, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add an Arc Dimension                                               */
  /**********************************************************************/  
  addArcDimension(pDb->getModelSpaceId(), 0, 9, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a 3D Polyline                                                  */
  /**********************************************************************/
  add3dPolyline(pDb->getModelSpaceId(), 1, 0, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add MText                                                          */
  /**********************************************************************/
  addMText(pDb->getModelSpaceId(), 1, 1, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Block Reference                                                */
  /**********************************************************************/  
  addBlockRef(pDb->getModelSpaceId(), 1, 2, odaLayer1Id, ttfStyleId, odaBlock1Id);
  
  /**********************************************************************/
  /* Add Radial Dimension                                               */
  /**********************************************************************/
  addRadialDimension(pDb->getModelSpaceId(), 1, 3, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add 3D Face                                                       */
  /**********************************************************************/
  add3dFace(pDb->getModelSpaceId(), 1, 4, odaLayer1Id, ttfStyleId);
  
  /**********************************************************************/
  /* Add RText                                                          */
  /**********************************************************************/
  addRText(pDb->getModelSpaceId(), 1, 6, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Hatches                                                          */
  /**********************************************************************/ 
  addHatches(pDb->getModelSpaceId(), 2, 0, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add some text entities to ModelSpace                               */
  /**********************************************************************/  
  addTextEnts(pDb->getModelSpaceId(), 2, 1, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Solid                                                          */
  /**********************************************************************/ 
  addSolid(pDb->getModelSpaceId(), 2, 3, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add an Associative Dimension                                       */
  /**********************************************************************/  
  addDimAssoc(pDb->getModelSpaceId(), 2, 4, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add an Associative Dimension                                       */
  /**********************************************************************/
  addDimBreak(pDb->getModelSpaceId(), 7, 9, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add an Ray                                       */
  /**********************************************************************/ 
  addRay(pDb->getModelSpaceId(), 3, 0, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a 3 Point Angular Dimension                                       */
  /**********************************************************************/
  add3PointAngularDimension(pDb->getModelSpaceId(), 3, 1, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Ordinate Dimensions                                            */
  /**********************************************************************/ 
  addOrdinateDimensions(pDb->getModelSpaceId(), 3, 2, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a Spline                                                       */
  /**********************************************************************/  
  addSpline(pDb->getModelSpaceId(), 3, 3, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add some Traces                                                    */
  /**********************************************************************/
  addTraces(pDb->getModelSpaceId(), 3, 4, odaLayer1Id, ttfStyleId);
  
  /**********************************************************************/
  /* Add a Polyline                                                     */
  /**********************************************************************/  
  addPolyline(pDb->getModelSpaceId(), 3, 5, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add an ArcAlignedText                                              */
  /**********************************************************************/  
  addArcText(pDb->getModelSpaceId(), 3, 6, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a Wipeout                                                      */
  /**********************************************************************/  
  addWipeout(pDb->getModelSpaceId(), 3, 7, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a RadialDimensionLarge                                         */
  /**********************************************************************/ 
  addRadialDimensionLarge(pDb->getModelSpaceId(), 3, 8, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a 2 Line Angular Dimension                                       */
  /**********************************************************************/ 
  add2LineAngularDimension(pDb->getModelSpaceId(), 3, 9, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add an ACIS Solid                                                  */
  /**********************************************************************/
  addACIS(pDb->getModelSpaceId(), 1, 5, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add an Image                                                       */
  /**********************************************************************/  
  addImage(pDb->getModelSpaceId(), 4, 0, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add an Xref                                                        */
  /**********************************************************************/ 
  addXRef(pDb->getModelSpaceId(), 4, 1, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a Table                                                        */
  /**********************************************************************/  
  addTable(pDb->getModelSpaceId(), odaBlock1Id, 4, 2, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a Diametric Dimension                                               */
  /**********************************************************************/  
  addDiametricDimension(pDb->getModelSpaceId(), 4, 3, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a Shape                                                        */
  /**********************************************************************/
  addShape(pDb->getModelSpaceId(), 4, 4, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add a MInsert                                                      */
  /**********************************************************************/  
  addMInsert(pDb->getModelSpaceId(), 4, 5, odaLayer1Id, ttfStyleId, odaBlock1Id);

  /**********************************************************************/
  /* Add an Xline                                                       */
  /**********************************************************************/  
  addXline(pDb->getModelSpaceId(), 4, 6, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add custom objects                                                 */
  /**********************************************************************/  
  addCustomObjects(pDb);

  /**********************************************************************/
  /* Add Text with Field                                                */
  /**********************************************************************/ 
  addTextWithField(pDb->getModelSpaceId(), 5, 0, odaLayer1Id, shxTextStyleId, ttfStyleId);

  /**********************************************************************/
  /* Add OLE object                                                     */
  /**********************************************************************/  
  addOLE2FrameFromFile(pDb->getModelSpaceId(), 5, 1, OD_T("OdWriteEx.xls"), odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Box                                                            */
  /**********************************************************************/  
  addBox(pDb->getModelSpaceId(), 5, 2, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Frustum                                                        */
  /**********************************************************************/  
  addFrustum(pDb->getModelSpaceId(), 5, 3, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Sphere                                                         */
  /**********************************************************************/ 
  addSphere(pDb->getModelSpaceId(), 5, 4, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Torus                                                          */
  /**********************************************************************/
  addTorus(pDb->getModelSpaceId(), 5, 5, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Wedge                                                          */
  /**********************************************************************/  
  addWedge(pDb->getModelSpaceId(), 5, 6, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Region                                                         */
  /**********************************************************************/  
  addRegion(pDb->getModelSpaceId(), 5, 7, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Extrusion                                                          */
  /**********************************************************************/  
  addExtrusion(pDb->getModelSpaceId(), 6, 0, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Revolution                                                          */
  /**********************************************************************/  
  addSolRev(pDb->getModelSpaceId(), 6, 1, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Helix                                                          */
  /**********************************************************************/
  addHelix(pDb->getModelSpaceId(), 6, 2, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add Underlays                                                   */
  /**********************************************************************/ 
  addDwfUnderlay(pDb->getModelSpaceId(), 6, 3, odaLayer1Id, ttfStyleId);
  addDgnUnderlay(pDb->getModelSpaceId(), 6, 4, odaLayer1Id, ttfStyleId);
  addPdfUnderlay(pDb->getModelSpaceId(), 6, 5, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add some MLeaders                                                  */
  /**********************************************************************/  
  addMLeaders(pDb->getModelSpaceId(), 7, 4, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add some Lights                                                  */
  /**********************************************************************/
  addLights(pDb->getModelSpaceId(), 6, 6, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add some SubDMeshes                                                  */
  /**********************************************************************/  
  addSubDMeshes(pDb->getModelSpaceId(), 6, 7, odaLayer1Id, ttfStyleId);

  /**********************************************************************/
  /* Add some Surfaces                                                  */
  /**********************************************************************/  
  addExtrudedSurfaces(pDb->getModelSpaceId(), 6, 8, odaLayer1Id, ttfStyleId);
  addRevolvedSurfaces(pDb->getModelSpaceId(), 6, 9, odaLayer1Id, ttfStyleId);
  addPlaneSurfaces(pDb->getModelSpaceId(), 6, 10, odaLayer1Id, ttfStyleId);
  addLoftedSurfaces(pDb->getModelSpaceId(), 7, 0, odaLayer1Id, ttfStyleId);
  addSweptSurfaces(pDb->getModelSpaceId(), 7, 1, odaLayer1Id, ttfStyleId);
  addNURBSurface(pDb, 7, 2, odaLayer1Id, ttfStyleId);
  /**********************************************************************/
  /* Add PointCloud
  /**********************************************************************/ 
  //addPointCloud(pDb, 7, 3, odaLayer1Id, ttfStyleId);

  //
  addPointCloudEx(pDb, 7, 3, odaLayer1Id, ttfStyleId);
  //

  /**********************************************************************/
  /* Add a Layout                                                       */
  /**********************************************************************/  
  addLayout(pDb);


  // If preview bitmap is already available it can be specified to avoid wasting
  // time on generating it by DD
  const OdChar* pBmpFileName = OD_T("preview.bmp");
  if(::odSystemServices()->accessFile(pBmpFileName, Oda::kFileRead))
  {
    OdRdFileBuf bmpFile(pBmpFileName);

    OdArray<OdUInt8> buf;
    buf.resize((unsigned)bmpFile.length());
    OdUInt8 * pData = buf.asArrayPtr(); 
    bmpFile.getBytes(pData, buf.length());
    // Get length taking care about big-endian
    OdUInt32 length = pData[2] + (pData[3] << 8);
    pData += 14;  // Skip BITMAPFILEHEADER
    pDb->setThumbnailBitmap(pData, length);
  }

  /**********************************************************************/
  /* Add GeoData, GeoMap, GeoPositionMarker                             */
  /**********************************************************************/
  addGeoStuff(pDb, 7, 5, odaLayer1Id, ttfStyleId);

  addSubDMeshesWithDbMaterial(pDb->getModelSpaceId(), 7, 6, odaLayer1Id, ttfStyleId);
}

/************************************************************************/
/* Add a layout                                                         */
/************************************************************************/
void DbFiller::addLayout(OdDbDatabase* pDb)
{
    /********************************************************************/
    /* Create a new Layout                                              */
    /********************************************************************/  
    OdDbObjectId layoutId = pDb->createLayout(OD_T("ODA Layout"));
    OdDbLayoutPtr pLayout = layoutId.safeOpenObject();

    /********************************************************************/
    /* Make it current, creating the overall PaperSpace viewport        */
    /********************************************************************/  
    pDb->setCurrentLayout(layoutId);
     
    /********************************************************************/
    /* Open the overall viewport for this layout                        */
    /********************************************************************/  
    OdDbViewportPtr pOverallViewport = pLayout->overallVportId().safeOpenObject();

    /********************************************************************/
    /* Get some useful parameters                                       */
    /********************************************************************/
    OdGePoint3d overallCenter   = pOverallViewport->centerPoint();
    
    /********************************************************************/
    /* Note:                                                            */
    /* If a viewport is an overall viewport,                            */
    /* the values returned by width() and height() must be divided by a */
    /* factor of 1.058, and the parameters of setWidth and setHeight()  */
    /* must be multiplied a like factor.                                */
    /********************************************************************/
    const double margin = 0.25;
    double overallWidth         = pOverallViewport->width()  / 1.058 - 2 * margin;
    double overallHeight        = pOverallViewport->height() / 1.058 - 2 * margin;
    OdGePoint3d overallLLCorner = overallCenter -                         
      OdGeVector3d( 0.5 * overallWidth, 0.5 * overallHeight, 0.0 );
    
    /********************************************************************/
    /* Open the PaperSpace BlockTableRecord for this layout             */ 
    /********************************************************************/  
    OdDbBlockTableRecordPtr pPS = pLayout->getBlockTableRecordId().safeOpenObject(OdDb::kForWrite);
    
    /********************************************************************/
    /* Create a new viewport, and append it to PaperSpace               */ 
    /********************************************************************/
    OdDbViewportPtr pViewport = OdDbViewport::createObject();
    pViewport->setDatabaseDefaults(pDb);
    pPS->appendOdDbEntity(pViewport);

    /********************************************************************/
    /* Set some parameters                                              */ 
    /*                                                                  */
    /* This viewport occupies the upper half of the overall viewport,   */
    /* and displays all objects in model space                          */
    /********************************************************************/  

    pViewport->setWidth         (overallWidth);
    pViewport->setHeight        (overallHeight*0.5);
    pViewport->setCenterPoint   (overallCenter + OdGeVector3d(0.0, 0.5*pViewport->height(), 0.0));
    pViewport->setViewCenter    (pOverallViewport->viewCenter());
    pViewport->zoomExtents();

    /********************************************************************/
    /* Create viewports for each of the entities that have been         */ 
    /* pushBacked onto m_layoutEntities                                 */
    /********************************************************************/
    if (m_layoutEntities.size())
    {
      double widthFactor = 1.0/m_layoutEntities.size();
      for (unsigned i=0; i<m_layoutEntities.size(); ++i)
      
      {
        OdDbEntityPtr pEnt = m_layoutEntities[i].safeOpenObject();
        OdGeExtents3d entityExtents;
        if (pEnt->getGeomExtents(entityExtents) == eOk)
        {
          /**************************************************************/
          /* Create a new viewport, and append it to PaperSpace         */ 
          /**************************************************************/  
          pViewport = OdDbViewport::createObject();
          pViewport->setDatabaseDefaults(pDb);
          pPS->appendOdDbEntity(pViewport);

          /**************************************************************/
          /* The viewports are tiled along the bottom of the overall    */
          /* viewport                                                   */
          /**************************************************************/
          pViewport->setWidth       (overallWidth*widthFactor);
          pViewport->setHeight      (overallHeight*0.5);
          pViewport->setCenterPoint (overallLLCorner + 
            OdGeVector3d((i+0.5) * pViewport->width(), 0.5*pViewport->height(), 0.0));
          
          /**************************************************************/
          /* The target of the viewport is the midpoint of the entity   */
          /* extents                                                    */
          /**************************************************************/
          const OdGePoint3d& minPt = entityExtents.minPoint();
          const OdGePoint3d& maxPt = entityExtents.maxPoint();
          OdGePoint3d viewTarget;
          viewTarget.x =(minPt.x + maxPt.x) / 2.0;
          viewTarget.y =(minPt.y + maxPt.y) / 2.0;
          viewTarget.z =(minPt.z + maxPt.z) / 2.0;
          pViewport->setViewTarget(viewTarget);
          
          /**************************************************************/
          /* The viewHeight is the larger of the height as defined by   */
          /* the entityExtents, and the height required to display the  */
          /* width of the entityExtents                                 */
          /**************************************************************/
          double viewHeight = fabs(maxPt.y - minPt.y);
          double viewWidth  = fabs(maxPt.x - minPt.x);
          viewHeight = odmax(viewHeight, viewWidth * pViewport->height() / pViewport->width());
          pViewport->setViewHeight(viewHeight*1.05);
        }
      }
    }
    pDb->setTILEMODE(1); // Disable PaperSpace
}

/************************************************************************/
/* Add entity boxes to specified BlockTableRecord                       */
/************************************************************************/
void DbFiller::createEntityBoxes(const OdDbObjectId& btrId, 
                                 const OdDbObjectId& layerId)
{
  OdDbDatabase* pDb = btrId.database();
  
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  OdGePoint3d currentPoint;
  
  /**********************************************************************/
  /* Create a 2D polyline for each box                                  */
  /**********************************************************************/  
  for ( int j=0; j < VER_BOXES; j++)
  {
    for ( int i=0; i < HOR_BOXES; i++)
    {
      if ( ! m_EntityBoxes.isBox (j,i) )
        break;

      double wCurBox = m_EntityBoxes.getWidth(j,i);
      currentPoint = m_EntityBoxes.getBox(j,i);

      OdDb2dPolylinePtr pPline = OdDb2dPolyline::createObject();
      pPline->setDatabaseDefaults(pDb);

      bBTR->appendOdDbEntity(pPline);

      OdDb2dVertexPtr pVertex;

      pVertex = OdDb2dVertex::createObject();
      pVertex->setDatabaseDefaults(pDb);
      pPline->appendVertex(pVertex);
      OdGePoint3d pos = currentPoint;
      pVertex->setPosition(pos);

      pVertex = OdDb2dVertex::createObject();
      pPline->appendVertex(pVertex);
      pos.x += wCurBox;
      pVertex->setPosition(pos);

      pVertex = OdDb2dVertex::createObject();
      pPline->appendVertex(pVertex);
      pos.y -= m_EntityBoxes.getHeight();
      pVertex->setPosition(pos);

      pVertex = OdDb2dVertex::createObject();
      pPline->appendVertex(pVertex);
      pos.x -= wCurBox;
      pVertex->setPosition(pos);


      pPline->makeClosed();

      pPline->setColorIndex(OdCmEntityColor::kACIBlue, true);
      pPline->setLayer(layerId, true); 
    }
  }
  /**********************************************************************/
  /* 'Zoom' the box array by resizing the active tiled MS viewport      */
  /**********************************************************************/  
  OdDbViewportTablePtr          pVpTable  = pDb->getViewportTableId().safeOpenObject(OdDb::kForWrite);
  OdDbObjectId                  vpID      = pVpTable->getActiveViewportId();
  OdDbViewportTableRecordPtr    vPortRec  = vpID.safeOpenObject(OdDb::kForWrite);

  OdGePoint3d center = m_EntityBoxes.getArrayCenter();
  vPortRec->setCenterPoint(center.convert2d());
  
  OdGeVector3d size  = m_EntityBoxes.getArraySize();
  vPortRec->setHeight(1.05 * fabs(size.y));
  vPortRec->setWidth(1.05 * fabs(size.x));
  vPortRec->setCircleSides(20000);
}
/************************************************************************/
/* Add a PaperSpace viewport to the specified database                  */
/************************************************************************/
void DbFiller::addPsViewport(OdDbDatabase* pDb, 
                             const OdDbObjectId& layerId)
{
  /**********************************************************************/
  /* Enable PaperSpace                                                  */
  /*                                                                    */
  /* NOTE: This is required to cause Teigha to automatically create     */
  /* the overall viewport. If not called before opening PaperSpace      */
  /* BlockTableRecord,   the first viewport created IS the the overall  */
  /* viewport.                                                          */
  /**********************************************************************/
  pDb->setTILEMODE(0);

  /**********************************************************************/
  /* Open PaperSpace                                                    */
  /**********************************************************************/
  OdDbBlockTableRecordPtr pPs = pDb->getPaperSpaceId().safeOpenObject(OdDb::kForWrite);
  
  /**********************************************************************/
  /* Disable PaperSpace                                                 */
  /**********************************************************************/  
  // pDb->setTILEMODE(1);

  /**********************************************************************/
  /* Create the viewport                                                */
  /**********************************************************************/
  OdDbViewportPtr pVp = OdDbViewport::createObject();
  pVp->setDatabaseDefaults(pDb);
  /**********************************************************************/
  /* Add it to PaperSpace                                               */
  /**********************************************************************/
  pPs->appendOdDbEntity(pVp);
  
  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/
  pVp->setCenterPoint(OdGePoint3d(5.25, 4.0, 0));
  pVp->setWidth(10.0);
  pVp->setHeight(7.5);
  pVp->setViewTarget(OdGePoint3d(0, 0, 0));
  pVp->setViewDirection(OdGeVector3d(0, 0, 1));
  pVp->setViewHeight(8.0);
  pVp->setLensLength(50.0);
  pVp->setViewCenter(OdGePoint2d(5.25, 4.0));
  pVp->setSnapIncrement(OdGeVector2d(0.25, 0.25));
  pVp->setGridIncrement(OdGeVector2d(0.25, 0.25));
  pVp->setCircleSides(OdUInt16(20000));

  /**********************************************************************/
  /* Freeze a layer in this viewport                                    */
  /**********************************************************************/
  OdDbObjectIdArray frozenLayers;
  frozenLayers.append(layerId);
  pVp->freezeLayersInViewport(frozenLayers);

  /**********************************************************************/
  /* Add a circle to this PaperSpace Layout                             */
  /**********************************************************************/  
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(pDb);
  pPs->appendOdDbEntity(pCircle);
  pCircle->setRadius(1.0);
  pCircle->setCenter(OdGePoint3d(1.0, 1.0, 0.0));
  pCircle->setLayer(layerId, false);

  /**********************************************************************/
  /* Disable PaperSpace                                                 */
  /**********************************************************************/
  pDb->setTILEMODE(1);
}

/************************************************************************/
/* Add a dimension style to the specified database                      */
/************************************************************************/
OdDbObjectId DbFiller::addDimStyle(OdDbDatabase* pDb, 
                                   const OdString& dimStyleName)
{
  /**********************************************************************/
  /* Create the DimStyle                                                */
  /**********************************************************************/
  OdDbDimStyleTableRecordPtr pDimStyle = OdDbDimStyleTableRecord::createObject();
  /**********************************************************************/
  /* Set the name                                                       */
  /**********************************************************************/
  pDimStyle->setName(dimStyleName);
  /**********************************************************************/
  /* Open the DimStyleTable                                             */
  /**********************************************************************/
  OdDbDimStyleTablePtr pTable = pDb->getDimStyleTableId().safeOpenObject(OdDb::kForWrite);
  /**********************************************************************/
  /* Add the DimStyle                                                   */
  /**********************************************************************/
  OdDbObjectId dimStyleId = pTable->add(pDimStyle);
  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pDimStyle->setDimtxsty(pDb->getTextStyleStandardId());
  pDimStyle->setDimsah(true);
  pDimStyle->setDimblk1(OD_T("_OBLIQUE"));
  pDimStyle->setDimblk2(OD_T("_DOT"));
  return dimStyleId;
}

/************************************************************************/
/* Add an Associative Dimension to the specified BlockTableRecord       */
/************************************************************************/
void DbFiller::addDimAssoc(const OdDbObjectId& btrId, 
                           const int boxRow,
                           const int boxCol,
                           const OdDbObjectId& layerId,
                           const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Associative"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dimension"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a line to be dimensioned                                    */
  /**********************************************************************/
  OdGePoint3d line1Pt, line2Pt;
  line1Pt.x  = point.x + w * 1.0 / 8.0;
  line1Pt.y  = point.y + h * 2.0 / 8.0;
  line2Pt.x  = line1Pt.x + 3.75;
  line2Pt.y  = point.y + h * 7.0 / 8.0;

  OdDbLinePtr pLine   = OdDbLine::createObject();
  pLine->setDatabaseDefaults(pDb);
  OdDbObjectId lineId = bBTR->appendOdDbEntity(pLine);
  pLine->setStartPoint(line1Pt);
  pLine->setEndPoint(line2Pt);

  /**********************************************************************/
  /* Create a rotated dimension and dimension the ends of the line      */
  /**********************************************************************/
  OdDbRotatedDimensionPtr pDimension = OdDbRotatedDimension::createObject();
  pDimension->setDatabaseDefaults(pDb);
  OdDbObjectId dimensionId = bBTR->appendOdDbEntity(pDimension);

  OdGePoint3d dimLinePt;
  dimLinePt.x = point.x + w / 2.0;
  dimLinePt.y = point.y + h * 1.0 / 8.0;
  pDimension->setDatabaseDefaults(pDb);
  pDimension->setXLine1Point(pLine->startPoint());
  pDimension->setXLine2Point(pLine->endPoint());
  pDimension->setDimLinePoint(dimLinePt);
  pDimension->useDefaultTextPosition();
  pDimension->createExtensionDictionary();

  /**********************************************************************/
  /* Create an associative dimension                                    */
  /**********************************************************************/
  OdDbDimAssocPtr pDimAssoc = OdDbDimAssoc::createObject();

  /**********************************************************************/
  /* Associate the DimAssoc with the rotated dimension by               */
  /* adding it to the extension dictionary of the rotated dimension     */
  /**********************************************************************/
  OdDbObjectId dimAssId;
  OdResult res = pDimAssoc->post(dimensionId, dimAssId);

  pDimAssoc->setRotatedDimType(OdDbDimAssoc::kUnknown);

  /**********************************************************************/
  /* Attach the line to the associative dimension                       */
  /**********************************************************************/
  OdDbOsnapPointRefPtr pointRef = OdDbOsnapPointRef::createObject();
  pointRef->setPoint(pLine->startPoint());
  pointRef->setOsnapType(OdDb::kOsModeStart);
  pointRef->setNearPointParam(0.0);

  pointRef->mainEntity().objectIds().append(lineId);
  pointRef->mainEntity().subentId().setType(OdDb::kVertexSubentType);

  pDimAssoc->setPointRef(OdDbDimAssoc::kXline1Point, pointRef);

  pointRef = OdDbOsnapPointRef::createObject();
  pointRef->setPoint(pLine->endPoint());
  pointRef->setOsnapType(OdDb::kOsModeEnd);
  pointRef->setNearPointParam(1.0);

  pointRef->mainEntity().objectIds().append(lineId);
  pointRef->mainEntity().subentId().setType(OdDb::kEdgeSubentType);
  
  pDimAssoc->setPointRef(OdDbDimAssoc::kXline2Point, pointRef);

  /**********************************************************************/
  /* Add Persistent reactor DimAssoc to the line                        */
  /**********************************************************************/
  pLine->addPersistentReactor(dimAssId);
}

/************************************************************************/
/* Add an Dimension Break sample to the specified BlockTableRecord       */
/************************************************************************/
void DbFiller::addDimBreak(const OdDbObjectId& btrId, 
  const int boxRow,
  const int boxCol,
  const OdDbObjectId& layerId,
  const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);

  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Break"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dimension"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a line to be dimensioned                                    */
  /**********************************************************************/
  OdGePoint3d line1Pt, line2Pt;
  line1Pt.x  = point.x + w * 1.0 / 8.0;
  line1Pt.y  = point.y + h * 2.0 / 8.0;
  line2Pt.x  = line1Pt.x + 3.75;
  line2Pt.y  = point.y + h * 7.0 / 8.0;

  OdDbLinePtr pLine   = OdDbLine::createObject();
  pLine->setDatabaseDefaults(pDb);
  OdDbObjectId lineId = bBTR->appendOdDbEntity(pLine);
  OdGeVector3d v = OdGeVector3d::kXAxis; 
  v *= (w * 1.0 / 10.0); 
  pLine->setStartPoint(line1Pt - v);
  pLine->setEndPoint(line2Pt + v);

  /**********************************************************************/
  /* Create a rotated dimension and dimension the ends of the line      */
  /**********************************************************************/
  OdDbRotatedDimensionPtr pDimension = OdDbRotatedDimension::createObject();
  pDimension->setDatabaseDefaults(pDb);
  OdDbObjectId dimensionId = bBTR->appendOdDbEntity(pDimension);

  line1Pt.y  = point.y + h * 5.0 / 8.0;
  OdGePoint3d dimLinePt;
  dimLinePt.x = point.x + w / 2.0;
  dimLinePt.y = point.y + h * 1.0 / 8.0;
  pDimension->setDatabaseDefaults(pDb);
  pDimension->setXLine1Point(line1Pt);
  pDimension->setXLine2Point(line2Pt);
  pDimension->setDimLinePoint(dimLinePt);
  pDimension->useDefaultTextPosition();
  pDimension->createExtensionDictionary();

  /**********************************************************************/
  /* Create an break data                                               */
  /**********************************************************************/
  OdDbBreakDataPtr pDimBreakData = OdDbBreakData::createObject();

  pDimBreakData->setDimObjId(dimensionId);
  /**********************************************************************/
  /* Attach the line to the break data dimension                       */
  /**********************************************************************/

  OdDbBreakPointRefPtr pointRef  = OdDbBreakPointRef::createObject();
  pointRef->setPointType(OdDbBreakPointRef::kDynamic);

  OdDbXrefFullSubentPath id;
  id.objectIds().append(lineId);
  pointRef->setBreakPointId(id);
  pointRef->setLineIndex(-1);

  pDimBreakData->addBreakPointRef(pointRef);

  OdDbDictionaryPtr pDict = pDimension->extensionDictionary().safeOpenObject(OdDb::kForWrite);
  pDict->setAt(L"ACAD_BREAKDATA", pDimBreakData);
  OdDbObjectId idBrData = pDict->getAt(L"ACAD_BREAKDATA");
  pLine->addPersistentReactor(idBrData);
}

/************************************************************************/
/* Add an Aligned Dimension to the specified BlockTableRecord           */
/************************************************************************/
void DbFiller::addAlignedDimension(const OdDbObjectId& btrId, 
                                   const int boxRow,
                                   const int boxCol,
                                   const OdDbObjectId& layerId,
                                   const OdDbObjectId& styleId,
                                   const OdDbObjectId& dimStyleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Aligned"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dimension"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a line to be dimensioned                                    */
  /**********************************************************************/
  OdGePoint3d line1Pt, line2Pt;
  line1Pt.x  = point.x + w * 0.5 / 8.0;
  line1Pt.y  = point.y + h * 1.5 / 8.0;
  line2Pt    = line1Pt + OdGeVector3d(1.5, 2.0, 0.0); 

  OdDbLinePtr  pLine  = OdDbLine::createObject();
  pLine->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pLine);
  pLine->setStartPoint(line1Pt);
  pLine->setEndPoint(line2Pt);

  /**********************************************************************/
  /* Create an aligned dimension and dimension the ends of the line     */
  /**********************************************************************/
  OdDbAlignedDimensionPtr pDimension = OdDbAlignedDimension::createObject();
  pDimension->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pDimension);
  
  OdGePoint3d dimLinePt;
  dimLinePt.x = point.x + w * 3.5 / 8.0;
  dimLinePt.y = point.y + h * 2.0 / 8.0;

  pDimension->setDimensionStyle(dimStyleId);
  pDimension->setXLine1Point(pLine->startPoint());
  pDimension->setXLine2Point(pLine->endPoint());
  pDimension->setDimLinePoint(dimLinePt);
  pDimension->useDefaultTextPosition();
  pDimension->setJogSymbolHeight(1.5);
}

/************************************************************************/
/* Add a Radial Dimension to the specified BlockTableRecord             */
/************************************************************************/
void DbFiller::addRadialDimension(const OdDbObjectId& btrId, 
                                  const int boxRow,
                                  const int boxCol,
                                  const OdDbObjectId& layerId,
                                  const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
//  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Radial"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dimension"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a circle to be dimensioned                                    */
  /**********************************************************************/
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pCircle);
  pCircle->setCenter(point + OdGeVector3d(0.625, h * 3.0 / 8.0, 0));
  pCircle->setRadius(0.5);
  
  /**********************************************************************/
  /* Create a Radial Dimension                                         */
  /**********************************************************************/
  OdDbRadialDimensionPtr pDimension = OdDbRadialDimension::createObject();
  pDimension->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity      (pDimension);

  pDimension->setCenter         (pCircle->center());
  OdGeVector3d chordVector      (pCircle->radius(), 0.0, 0.0);
  chordVector.rotateBy          (OdaToRadian(75.0), OdGeVector3d::kZAxis);
  pDimension->setChordPoint     (pDimension->center() + chordVector);
  pDimension->setLeaderLength   (0.125);
  pDimension->useDefaultTextPosition();
}

/************************************************************************/
/* Add a Diametric Dimension to the specified BlockTableRecord             */
/************************************************************************/
void DbFiller::addDiametricDimension(const OdDbObjectId& btrId, 
                                     const int boxRow,
                                     const int boxCol,
                                     const OdDbObjectId& layerId,
                                     const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
//  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Diametric"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dimension"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a circle to be dimensioned                                    */
  /**********************************************************************/
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pCircle);
  pCircle->setCenter(point + OdGeVector3d(0.625, h * 3.0 / 8.0, 0));
  pCircle->setRadius(0.5);
  
  /**********************************************************************/
  /* Create a Diametric Dimension                                       */
  /**********************************************************************/
  OdDbDiametricDimensionPtr pDimension = OdDbDiametricDimension::createObject();
  pDimension->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity        (pDimension);

  OdGeVector3d chordVector      (pCircle->radius(), 0.0, 0.0);
  chordVector.rotateBy          (OdaToRadian(75.0), OdGeVector3d::kZAxis);

  pDimension->setChordPoint     (pCircle->center() + chordVector);
  pDimension->setFarChordPoint  (pCircle->center() - chordVector);
  pDimension->setLeaderLength   (0.125);
  pDimension->useDefaultTextPosition();
}
/************************************************************************/
/* Add a Shape to the specified BlockTableRecord                        */
/************************************************************************/
void DbFiller::addShape(const OdDbObjectId& btrId, 
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Shape"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the center of the box                                          */
  /**********************************************************************/
  OdGePoint3d pCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  /**********************************************************************/
  /* Create a Shape                                                     */
  /**********************************************************************/
  OdDbShapePtr pShape = OdDbShape::createObject();
  pShape->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity        (pShape);
  double size = w * 3.0 / 8.0;
  pShape->setSize               (size);
  pShape->setPosition           (pCenter + OdGeVector3d( 0.0, -size, 0.0 ) );
  pShape->setRotation           (OdaToRadian(90.0));
  pShape->setName               (OD_T("CIRC1"));
}

/************************************************************************/
/* Add a 3D Face to the specified BlockTableRecord                      */
/************************************************************************/
void DbFiller::add3dFace(const OdDbObjectId& btrId, 
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("3DFACE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a 3D Face                                                   */
  /**********************************************************************/
  OdDbFacePtr pFace = OdDbFace::createObject();
  pFace->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pFace);

  pFace->setVertexAt(0, point + OdGeVector3d( w * 1.0 / 8.0, h * 1.0/8.0, 0.0 ));
  pFace->setVertexAt(1, point + OdGeVector3d( w * 7.0 / 8.0, h * 1.0/8.0, 0.0 ));
  pFace->setVertexAt(2, point + OdGeVector3d( w * 7.0 / 8.0, h * 6.0/8.0, 0.0 ));
  pFace->setVertexAt(3, point + OdGeVector3d( w * 1.0 / 8.0, h * 6.0/8.0, 0.0 ));
}

/************************************************************************/
/* Add a Solid to the specified BlockTableRecord                          */
/************************************************************************/
void DbFiller::addSolid(const OdDbObjectId& btrId, 
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("SOLID"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a Solid                                                   */
  /**********************************************************************/
  OdDbSolidPtr pSolid = OdDbSolid::createObject();
  pSolid->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSolid);

  pSolid->setPointAt(0, point + OdGeVector3d( w * 1.0 / 8.0, h * 1.0/8.0, 0.0 ));
  pSolid->setPointAt(1, point + OdGeVector3d( w * 7.0 / 8.0, h * 1.0/8.0, 0.0 ));
  pSolid->setPointAt(2, point + OdGeVector3d( w * 1.0 / 8.0, h * 6.0/8.0, 0.0 ));
  pSolid->setPointAt(3, point + OdGeVector3d( w * 7.0 / 8.0, h * 6.0/8.0, 0.0 ));
}

/************************************************************************/
/* Add an ACIS Solid to the specified BlockTableRecord                  */
/************************************************************************/
void DbFiller::addACIS(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double h    = m_EntityBoxes.getHeight();
//  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("3DSOLID"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  OdDbEntityPtrArray entities;
  OdGeMatrix3d xfm = OdGeMatrix3d::translation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector());
  
  /**********************************************************************/
  /* Read the solids in the .sat file                                   */
  /**********************************************************************/
  if (OdDbBody::acisIn(OD_T("OdWriteEx.sat"), entities) == eOk)
  {
    /********************************************************************/
    /* Read the solids in the .sat file                                 */
    /********************************************************************/
    addTextEnt(bBTR,
      point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
      OD_T("from SAT file"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
    for ( unsigned int i = 0; i < entities.size(); ++i )
    {
      /******************************************************************/
      /* Move the solid into the center of the box                      */
      /******************************************************************/
      OdDbObjectId id = bBTR->appendOdDbEntity( entities[i] );
      OdDbEntityPtr p3dSolid = id.safeOpenObject(OdDb::kForWrite);
      p3dSolid->transformBy(xfm);
      /******************************************************************/
      /* Each of these entities will later get its own viewport         */
      /******************************************************************/
      m_layoutEntities.push_back(id);
    }
  }
  else
  {
    /********************************************************************/
    /* Create a simple solid                                            */
    /********************************************************************/
    OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
    p3dSolid->setDatabaseDefaults(bBTR->database());
    OdDbObjectId id = bBTR->appendOdDbEntity(p3dSolid);

    p3dSolid->createSphere( 1.0 );
    p3dSolid->transformBy( xfm );

    /********************************************************************/
    /* This entity will later get its own viewport                      */
    /********************************************************************/
    m_layoutEntities.push_back(id);
  }
}
/************************************************************************/
/* Add a Box to the specified BlockTableRecord                          */
/************************************************************************/
void DbFiller::addBox(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Box"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
  p3dSolid->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(p3dSolid);

  OdGeMatrix3d xfm = OdGeMatrix3d::translation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector());

  p3dSolid->createBox( w * 6.0 / 8.0, h * 6.0 / 8.0 ,w * 6.0 / 8.0);
  p3dSolid->transformBy( xfm );
}
/************************************************************************/
/* Add a Frustum to the specified BlockTableRecord                      */
/************************************************************************/
void DbFiller::addFrustum(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Frustum"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
  p3dSolid->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(p3dSolid);

  OdGeMatrix3d xfm = OdGeMatrix3d::translation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector());

  p3dSolid->createFrustum( w * 6.0 / 8.0, w * 3.0 / 8.0, h * 3.0 / 8.0,  w * 1.0 / 8.0);
  p3dSolid->transformBy( xfm );
}
/************************************************************************/
/* Add a Sphere to the specified BlockTableRecord                       */
/************************************************************************/
void DbFiller::addSphere(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  //double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Sphere"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
  p3dSolid->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(p3dSolid);

  OdGeMatrix3d xfm = OdGeMatrix3d::translation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector());

  p3dSolid->createSphere( w * 3.0 / 8.0 );
  p3dSolid->transformBy( xfm );
}
/************************************************************************/
/* Add a Sphere to the specified BlockTableRecord                       */
/************************************************************************/
void DbFiller::addTorus(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Torus"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
  p3dSolid->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(p3dSolid);

  OdGeMatrix3d xfm = OdGeMatrix3d::translation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector());

  p3dSolid->createTorus( w * 2.0 / 8.0, w * 1.0 / 8.0 );
  p3dSolid->transformBy( xfm );
}
/************************************************************************/
/* Add a Wedge to the specified BlockTableRecord                       */
/************************************************************************/
void DbFiller::addWedge(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Wedge"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
  p3dSolid->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(p3dSolid);

  OdGeMatrix3d xfm = OdGeMatrix3d::translation(m_EntityBoxes.getBoxCenter(boxRow,boxCol).asVector());

  p3dSolid->createWedge( w * 6.0 / 8.0, h * 6.0 / 8.0, w * 6.0 / 8.0 );
  p3dSolid->transformBy( xfm );
}
/************************************************************************/
/* Add a Region to the specified BlockTableRecord                       */
/************************************************************************/
void DbFiller::addRegion(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Region"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Create a Circle                                                    */
  /**********************************************************************/
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());

  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  pCircle->setCenter(center);
  pCircle->setRadius(w * 3.0 / 8.0); 

  
  /**********************************************************************/
  /* Add it to the array of curves                                      */
  /**********************************************************************/
  OdRxObjectPtrArray curveSegments;
  curveSegments.push_back((OdRxObjectPtr)pCircle.get());
  
  /**********************************************************************/
  /* Create the region                                                  */
  /**********************************************************************/
  OdRxObjectPtrArray regions;
  OdResult res = OdDbRegion::createFromCurves(curveSegments, regions);
  ODA_ASSERT(res == eOk);

  /**********************************************************************/
  /* Append it to the block table record                                */
  /**********************************************************************/
  if (res == eOk)
  {
    bBTR->appendOdDbEntity((OdDbEntityPtr)regions[0]);  
  }
}
/************************************************************************/
/* Add an Extrusion to the specified BlockTableRecord                   */
/************************************************************************/
void DbFiller::addExtrusion(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Extrusion"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
  p3dSolid->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(p3dSolid);

  /**********************************************************************/
  /* Create a Circle                                                    */
  /**********************************************************************/
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());

  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  pCircle->setCenter(center);
  pCircle->setRadius(w * 3.0 / 8.0); 

  /**********************************************************************/
  /* Add it to the array of curves                                      */
  /**********************************************************************/
  OdRxObjectPtrArray curveSegments;
  curveSegments.push_back((OdRxObjectPtr)pCircle.get());
  
  /**********************************************************************/
  /* Create a region                                                    */
  /**********************************************************************/
  OdRxObjectPtrArray regions;
  OdDbRegion::createFromCurves(curveSegments, regions);

  /**********************************************************************/
  /* Extrude the region                                                 */
  /**********************************************************************/
  p3dSolid->extrude((OdDbRegionPtr) regions[0], w * 6.0 / 8.0);
  
}
/************************************************************************/
/* Add an Solid of Revolution to the specified BlockTableRecord         */
/************************************************************************/
void DbFiller::addSolRev(const OdDbObjectId& btrId, 
                       const int boxRow,
                       const int boxCol,
                       const OdDbObjectId& layerId,
                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Solid of"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  addTextEnt(bBTR,
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Revolution"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  OdDb3dSolidPtr p3dSolid = OdDb3dSolid::createObject();
  p3dSolid->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(p3dSolid);

  /**********************************************************************/
  /* Create a Circle                                                    */
  /**********************************************************************/
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());

  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  pCircle->setCenter(center + OdGeVector3d(w * 2.5 / 8.0, 0.0, 0.0));
  pCircle->setRadius(w * 1.0 / 8.0); 

  /**********************************************************************/
  /* Add it to the array of curves                                      */
  /**********************************************************************/
  OdRxObjectPtrArray curveSegments;
  curveSegments.push_back((OdRxObjectPtr)pCircle.get());
  
  /**********************************************************************/
  /* Create a region                                                    */
  /**********************************************************************/
  OdRxObjectPtrArray regions;
  OdDbRegion::createFromCurves(curveSegments, regions);

  /**********************************************************************/
  /* revolve the region                                                 */
  /**********************************************************************/
  p3dSolid->revolve((OdDbRegionPtr)regions[0], center, OdGeVector3d(0.0, -1.0, 0.0), Oda2PI);
}

void DbFiller::addHelix(const OdDbObjectId& blockId, 
                        const int boxRow, 
                        const int boxCol, 
                        const OdDbObjectId& layerId, 
                        const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = blockId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Helix"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Create the Helix                                                   */
  /**********************************************************************/
  OdDbHelixPtr pHelix = OdDbHelix::createObject();
  pHelix->setDatabaseDefaults(bBTR->database());

  /**********************************************************************/
  /* Add the Helix to the database                                      */
  /**********************************************************************/
  bBTR->appendOdDbEntity(pHelix);
  
  /**********************************************************************/
  /* Set the Helix's parameters                                         */
  /**********************************************************************/
  pHelix->setConstrain(OdDbHelix::kHeight);
  pHelix->setHeight(h);
  pHelix->setAxisPoint(point + OdGeVector3d(w/2.0, -h/2.0, 0.0));
  pHelix->setStartPoint(pHelix->axisPoint() + OdGeVector3d(w/6.0, 0.0, 0.0));
  pHelix->setTwist(false);
  pHelix->setTopRadius(w*3.0/8.0);
  pHelix->setTurns(6);
  
  /**********************************************************************/
  /* Create the Helix geometry (confirm parameters are set)             */
  /**********************************************************************/
  pHelix->createHelix();
}

/************************************************************************/
/* Add an Image to the specified BlockTableRecord                       */
/************************************************************************/
void DbFiller::addImage(const OdDbObjectId& btrId, 
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Open the Image Dictionary                                          */
  /**********************************************************************/
  OdDbObjectId imageDictId = OdDbRasterImageDef::createImageDictionary(pDb);
  OdDbDictionaryPtr pImageDict = imageDictId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create an ImageDef object                                          */
  /**********************************************************************/
  OdDbRasterImageDefPtr pImageDef = OdDbRasterImageDef::createObject();
  OdDbObjectId imageDefId = pImageDict->setAt(OD_T("OdWriteEx"), pImageDef);

  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/
  pImageDef->setSourceFileName(OD_T("OdWriteEx.jpg"));
  // Use next line to set image size manually without loading actual raster file.
  // This method sets "dummy" image instead. It's OK for saving drawing to DXF/DWG.
  // But image will not be rendered/exported to other formats without file saving and opening again
  pImageDef->setImage(OdGiRasterImageDesc::createObject(1024, 650, OdGiRasterImage::kInch));

  // Use next line to set size from the actual raster file.
  // This is also requried if you are going to render/export the drawing immediately
  // without saving to DWG and loading again
//pImageDef->image();    // Force image loading from file (findFile() should be able to locate the image).


  /**********************************************************************/
  /* Create an Image object                                             */
  /**********************************************************************/
  OdDbRasterImagePtr pImage = OdDbRasterImage::createObject();
  pImage->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pImage);

  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/
  pImage->setImageDefId(imageDefId);
  pImage->setOrientation(point, OdGeVector3d(w, 0, 0), OdGeVector3d(0.0, h, 0));
  pImage->setDisplayOpt(OdDbRasterImage::kShow, true);
  pImage->setDisplayOpt(OdDbRasterImage::kShowUnAligned, true);

  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  point = m_EntityBoxes.getBox(boxRow, boxCol);
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("IMAGE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
}

/************************************************************************/
/* Add a Ray to the specified BlockTableRecord                          */
/************************************************************************/
void DbFiller::addRay(const OdDbObjectId& btrId, 
                      const int boxRow,
                      const int boxCol,
                      const OdDbObjectId& layerId,
                      const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
//  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("RAY"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a Ray from the center of the box and passing through        */
  /* the lower-left corner of the box                                   */
  /**********************************************************************/
  OdDbRayPtr pRay = OdDbRay::createObject();
  pRay->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pRay);

  OdGePoint3d  basePoint  = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  OdGeVector3d unitDir    = (point - basePoint).normalize();

  pRay->setBasePoint(basePoint);
  pRay->setUnitDir  (unitDir);  
}

/************************************************************************/
/* Add an Xline to the specified BlockTableRecord                       */
/************************************************************************/
void DbFiller::addXline(const OdDbObjectId& btrId, 
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
//  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("XLINE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a Ray from the center of the box and passing through        */
  /* the lower-left corner of the box                                   */
  /**********************************************************************/
  OdDbXlinePtr pXline = OdDbXline::createObject();
  pXline->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pXline);

  OdGePoint3d  basePoint  = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  OdGeVector3d unitDir    = (point - basePoint).normalize();

  pXline->setBasePoint(basePoint);
  pXline->setUnitDir  (unitDir);  
}

/************************************************************************/
/* Add RText to the specified BlockTableRecord                          */
/************************************************************************/
void DbFiller::addRText(const OdDbObjectId& btrId, 
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
 
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  point += m_textOffset;
  addTextEnt(bBTR, point, point, 
    OD_T("RTEXT"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Create RText DIESEL expression with no MText sequences             */
  /**********************************************************************/
  
  OdRTextPtr pRText = RText::createObject();
  pRText->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pRText);

  point += m_textLine + m_textLine;
  point.x += w/16.0;
  pRText->setHeight(m_textSize);
  pRText->setPoint(point);
  pRText->setRotAngle(0.0);
  pRText->setToExpression(true);
  pRText->enableMTextSequences(false);
  pRText->setStringContents(OD_T("Expression: 123{\\C5;456}"));
  pRText->setTextStyle(styleId);

  /**********************************************************************/
  /* Create RText DIESEL expression with MText sequences                */
  /**********************************************************************/
  pRText = RText::createObject();
  pRText->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pRText);

  point += m_textLine;
  pRText->setHeight(m_textSize);
  pRText->setPoint(point);
  pRText->setRotAngle(0.0);
  pRText->setToExpression(true);
  pRText->enableMTextSequences(true);
  pRText->setStringContents(OD_T("Expression: 123{\\C5;456}"));
  pRText->setTextStyle(styleId);

  /**********************************************************************/
  /* Create RText External with no MText sequences                      */
  /**********************************************************************/
  pRText = RText::createObject();
  pRText->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pRText);

  point += m_textLine;
  pRText->setHeight(m_textSize);
  pRText->setPoint(point);
  pRText->setRotAngle(0.0);
  pRText->setToExpression(false);
  pRText->enableMTextSequences(false);
  pRText->setStringContents(inCurrentFolder(OD_T("OdWriteEx.txt")));
  pRText->setTextStyle(styleId);

  /**********************************************************************/
  /* Create RText External with MText sequences                         */
  /**********************************************************************/
  pRText = RText::createObject();
  pRText->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pRText);

  point += m_textLine;
  pRText->setHeight(m_textSize);
  pRText->setPoint(point);
  pRText->setRotAngle(0.0);
  pRText->setToExpression(false);
  pRText->enableMTextSequences(true);
  pRText->setStringContents(inCurrentFolder(OD_T("OdWriteEx.txt")));
  pRText->setTextStyle(styleId);

}

/************************************************************************/
/* Add Hatches to the specified BlockTableRecord                          */
/************************************************************************/
void DbFiller::addHatches(const OdDbObjectId& btrId, 
                          const int boxRow, 
                          const int boxCol, 
                          const OdDbObjectId& layerId,  
                          const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();
  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
//  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  double delta = w/12.0;
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("HATCHs"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Create a rectangular Hatch with a circular hole                    */
  /**********************************************************************/
  OdDbHatchPtr pHatch = OdDbHatch::createObject();
  pHatch->setDatabaseDefaults(pDb);
  OdDbObjectId whiteHatchId = bBTR->appendOdDbEntity(pHatch);

  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pHatch->setAssociative(false);
  pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
  pHatch->setHatchStyle(OdDbHatch::kNormal);

  /**********************************************************************/
  /* Define the outer loop with an OdGePolyline2d                       */
  /**********************************************************************/
  OdGePoint2dArray  vertexPts;
  OdGeDoubleArray   vertexBulges;
  vertexPts.resize(4);
  vertexPts[0].set(point.x + delta,   point.y - delta);
  vertexPts[1].set(point.x + delta*5, point.y - delta);
  vertexPts[2].set(point.x + delta*5, point.y - delta*5);
  vertexPts[3].set(point.x + delta,   point.y - delta*5);
  pHatch->appendLoop(OdDbHatch::kExternal | OdDbHatch::kPolyline, 
    vertexPts, vertexBulges);

  
  /**********************************************************************/
  /* Define an inner loop with an array of edges                        */
  /**********************************************************************/
  OdGePoint2d cenPt(point.x + delta*3, point.y - delta*3);
  OdGeCircArc2d *cirArc = new OdGeCircArc2d();
  cirArc->setCenter(cenPt);
  cirArc->setRadius(delta);
  cirArc->setAngles(0.0, Oda2PI);

  EdgeArray edgePtrs;
  edgePtrs.append(cirArc);
  pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);

  /**********************************************************************/
  /* Create a circular Hatch                                            */
  /**********************************************************************/
  pHatch = OdDbHatch::createObject();
  pHatch->setDatabaseDefaults(pDb);
  OdDbObjectId redHatchId = bBTR->appendOdDbEntity(pHatch);
  
  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pHatch->setAssociative(false);
  pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
  pHatch->setHatchStyle(OdDbHatch::kNormal);
  OdCmColor col;
  col.setRGB(255, 0, 0);
  pHatch->setColor(col);

  /**********************************************************************/
  /* Define an outer loop with an array of edges                        */
  /**********************************************************************/
  cirArc = new OdGeCircArc2d();
  cirArc->setCenter(cenPt - OdGeVector2d(delta, 0.0));
  cirArc->setRadius(delta);
  cirArc->setAngles(0.0, Oda2PI);
  edgePtrs.clear();
  edgePtrs.push_back(cirArc);
  pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);

  /**********************************************************************/
  /* Create a circular Hatch                                            */
  /**********************************************************************/
  pHatch = OdDbHatch::createObject();
  pHatch->setDatabaseDefaults(pDb);
  OdDbObjectId greenHatchId = bBTR->appendOdDbEntity(pHatch);

  pHatch->setAssociative(false);
  pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("SOLID"));
  pHatch->setHatchStyle(OdDbHatch::kNormal);
  col.setRGB(0, 255, 0);
  pHatch->setColor(col);

  /**********************************************************************/
  /* Define an outer loop with an array of edges                        */
  /**********************************************************************/
  cirArc = new OdGeCircArc2d();
  cirArc->setCenter(cenPt - OdGeVector2d(0.0, delta));
  cirArc->setRadius(delta);
  cirArc->setAngles(0.0, Oda2PI);
  edgePtrs.clear();
  edgePtrs.push_back(cirArc);
  pHatch->appendLoop(OdDbHatch::kDefault, edgePtrs);

  /**********************************************************************/
  /* Use the SortentsTable to manipulate draw order                     */
  /*                                                                    */
  /* The draw order now is white, red, green                            */
  /**********************************************************************/
  OdDbSortentsTablePtr pSET = bBTR->getSortentsTable();

  /**********************************************************************/
  /* Move the green hatch below the red hatch                           */
  /* The draw order now is white, green, red                            */
  /**********************************************************************/
  OdDbObjectIdArray id;
  id.resize(1);
  id[0] = greenHatchId;
  pSET->moveBelow(id, redHatchId);

  /**********************************************************************/
  /* Create an associative user-defined hatch                           */
  /**********************************************************************/
  pHatch = OdDbHatch::createObject();
  pHatch->setDatabaseDefaults(pDb);
  OdDbObjectId hatchId = bBTR->appendOdDbEntity(pHatch);

  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pHatch->setAssociative(true);
  pHatch->setDatabaseDefaults(pDb); // make hatch aware of DB for the next call
  pHatch->setPattern(OdDbHatch::kUserDefined, OD_T("_USER"));
  pHatch->setPatternSpace(0.125);
  pHatch->setPatternAngle(OdaToRadian(30.0));
  pHatch->setPatternDouble(true);
  pHatch->setHatchStyle(OdDbHatch::kNormal);
 
  /**********************************************************************/
  /* Define the loops                                                */
  /**********************************************************************/
  OdDbObjectIdArray loopIds;
  OdDbEllipsePtr pEllipse = OdDbEllipse::createObject();
  pEllipse->setDatabaseDefaults(pDb);
  loopIds.push_back(bBTR->appendOdDbEntity(pEllipse));

  OdGePoint3d centerPt = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  centerPt.x += delta;
  centerPt.y += delta * 1.5;
  pEllipse->set(centerPt, OdGeVector3d::kZAxis, OdGeVector3d(delta, 0., 0.), 0.5);

  /**********************************************************************/
  /* Append the loops to the hatch                                      */
  /**********************************************************************/
  pHatch->appendLoop(OdDbHatch::kDefault, loopIds);
  
  /**********************************************************************/
  /* Define a custom hatch pattern OD_T("MY_STARS")                           */
  /**********************************************************************/
  OdHatchPattern     stars;
  OdHatchPatternLine line;

  line.m_dLineAngle = 0.0;
  line.m_patternOffset = OdGeVector2d(0, 0.866);
  line.m_dashes.push_back(0.5);
  line.m_dashes.push_back(-0.5);
  stars.push_back(line);
  line.m_dLineAngle = 1.0472;
  line.m_patternOffset = OdGeVector2d(0, 0.866);
  stars.push_back(line);
  line.m_dLineAngle = 2.0944;
  line.m_basePoint = OdGePoint2d(0.25, 0.433);
  line.m_patternOffset = OdGeVector2d(0, 0.866);
  stars.push_back(line);
  
  /**********************************************************************/
  /* Register the pattern                                               */
  /**********************************************************************/
  pDb->appServices()->patternManager()->appendPattern(OdDbHatch::kCustomDefined, 
    OD_T("MY_STARS"), stars);
  
  /**********************************************************************/
  /* Create an associative custom defined hatch                         */
  /**********************************************************************/
  pHatch = OdDbHatch::createObject();
  pHatch->setDatabaseDefaults(pDb);
  hatchId = bBTR->appendOdDbEntity(pHatch);

  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/
  pHatch->setAssociative(true);
  pHatch->setDatabaseDefaults(pDb); // make hatch aware of DB for the next call
  pHatch->setPattern(OdDbHatch::kCustomDefined, OD_T("MY_STARS"));
  pHatch->setPatternScale(0.125);
  pHatch->setPatternAngle(OdaToRadian(30.0));
  pHatch->setHatchStyle(OdDbHatch::kNormal);

  /**********************************************************************/
  /* Define the loops                                                */
  /**********************************************************************/
  loopIds.clear();
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(pDb);
  loopIds.push_back(bBTR->appendOdDbEntity(pCircle));

  centerPt = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  centerPt.x += delta*4.0 ;
  centerPt.y += delta;
  pCircle->setCenter(centerPt);
  pCircle->setRadius(delta*1.5); 
   
  /**********************************************************************/
  /* Append the loops to the hatch                                      */
  /**********************************************************************/
  pHatch->appendLoop(OdDbHatch::kDefault, loopIds);

  try
  {
    /********************************************************************/
    /* Create an associative predefined hatch                           */
    /********************************************************************/
    pHatch = OdDbHatch::createObject();
    pHatch->setDatabaseDefaults(pDb);
    hatchId = bBTR->appendOdDbEntity(pHatch);

    /********************************************************************/
    /* Set some properties                                              */
    /********************************************************************/
    point = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
    // Set the hatch properties.
    pHatch->setAssociative(true);
    pHatch->setDatabaseDefaults(pDb);// make hatch aware of DB for the next call
    pHatch->setPattern(OdDbHatch::kPreDefined, OD_T("ANGLE"));
    pHatch->setPatternScale(0.5);
    pHatch->setPatternAngle(0.5); // near 30 degrees
    pHatch->setHatchStyle(OdDbHatch::kNormal);


    /********************************************************************/
    /* Define the loops                                                 */
    /********************************************************************/
    loopIds.clear();
    OdDbCirclePtr pCircle = OdDbCircle::createObject();
    pCircle->setDatabaseDefaults(pDb);
    loopIds.push_back(bBTR->appendOdDbEntity(pCircle));
    centerPt.x -= delta*2.0/* delta*3 */;
    centerPt.y -= delta*2.5;
    pCircle->setCenter(centerPt);
    pCircle->setRadius(delta*1.5);  
    
    /********************************************************************/
    /* Append the loops to the hatch                                    */
    /********************************************************************/
    pHatch->appendLoop(OdDbHatch::kDefault, loopIds);
  }
  catch (const OdError& e)
  {
    odPrintConsoleString(L"\n\nException occurred: %ls\n", e.description().c_str());
    odPrintConsoleString(L"\nHatch with predefined pattern \"ANGLE\" was not added.\n");
    odPrintConsoleString(L"\nMake sure PAT file with pattern definition is available to Teigha.");
    odPrintConsoleString(L"\n\nPress ENTER to continue...");
    STD(cin).get();
  }
}

/************************************************************************/
/* Add an Arc Dimension to the specified BlockTableRecord               */
/************************************************************************/
void DbFiller::addArcDimension(const OdDbObjectId& btrId, 
                               const int boxRow,
                               const int boxCol,
                               const OdDbObjectId& layerId,
                               const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Arc"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dimension"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create an arc to be dimensioned                                    */
  /**********************************************************************/
  OdDbArcPtr pArc = OdDbArc::createObject();
  pArc->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pArc);
  OdGePoint3d center = point + OdGeVector3d(w * 1.0 / 8.0, h * 1.0 / 8.0, 0.0); 
  pArc->setCenter(center);
  pArc->setStartAngle(OdaToRadian(0.0));
  pArc->setEndAngle(OdaToRadian(90.0));
  pArc->setRadius(4.0 / OdaPI);
  
  
  /**********************************************************************/
  /* Create an ArcDimension                                             */
  /**********************************************************************/
  OdDbArcDimensionPtr pDimension = OdDbArcDimension::createObject();
  pDimension->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pDimension);

  /**********************************************************************/
  /* Use the default dim variables                                      */
  /**********************************************************************/
  pDimension->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/
  pDimension->setCenterPoint(pArc->center());
  pDimension->setArcPoint(pArc->center() + OdGeVector3d(pArc->radius() + 0.45, 0.0, 0.0));
 
  OdGePoint3d startPoint;
  pArc->getStartPoint(startPoint);
  pDimension->setXLine1Point(startPoint);
  
  OdGePoint3d endPoint;
  pArc->getEndPoint(endPoint);
  pDimension->setXLine2Point(endPoint);

  pDimension->setArcSymbolType(1);
  
}

/************************************************************************/
/* Add a 3 Point Angular Dimension to the specified BlockTableRecord    */
/************************************************************************/
void DbFiller::add3PointAngularDimension(const OdDbObjectId& btrId, 
                                         const int boxRow,
                                         const int boxCol,
                                         const OdDbObjectId& layerId,
                                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("3 Point Angular"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dimension"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create an arc to be dimensioned                                    */
  /**********************************************************************/
  OdDbArcPtr pArc = OdDbArc::createObject();
  pArc->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pArc);
  OdGePoint3d center = point + OdGeVector3d(w * 1.0 / 8.0, h * 1.0 / 8.0, 0.0); 
  pArc->setCenter(center);
  pArc->setStartAngle(OdaToRadian(0.0));
  pArc->setEndAngle(OdaToRadian(90.0));
  pArc->setRadius(w * 3.0 / 8.0);
  
  /**********************************************************************/
  /* Create 3 point angular dimension                                   */
  /**********************************************************************/
  OdDb3PointAngularDimensionPtr pDimension = OdDb3PointAngularDimension::createObject();
  pDimension->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pDimension);

  /**********************************************************************/
  /* Use the default dim variables                                      */
  /**********************************************************************/
  pDimension->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/
  pDimension->setCenterPoint(pArc->center());
  pDimension->setArcPoint(pArc->center() + OdGeVector3d(pArc->radius() + 0.45, 0.0, 0.0));
 
  OdGePoint3d startPoint;
  pArc->getStartPoint(startPoint);
  pDimension->setXLine1Point(startPoint);
  
  OdGePoint3d endPoint;
  pArc->getEndPoint(endPoint);
  pDimension->setXLine2Point(endPoint);
}
/************************************************************************/
/* Add a 2 Line Angular Dimension to the specified BlockTableRecord     */
/************************************************************************/
void DbFiller::add2LineAngularDimension(const OdDbObjectId& btrId, 
                                         const int boxRow,
                                         const int boxCol,
                                         const OdDbObjectId& layerId,
                                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("2 Line Angular"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dimension"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create the lines to be dimensioned                                 */
  /**********************************************************************/
  OdGePoint3d center = point + OdGeVector3d(w * 1.0 / 8.0, h * 1.0 / 8.0, 0.0); 
  OdGeVector3d v1(w * 1.0 / 8.0, 0.0, 0.0);
  OdGeVector3d v2(w * 4.0 / 8.0, 0.0, 0.0);
  OdGeVector3d v3 = v2 + OdGeVector3d(0.45, 0.0, 0.0);
  
  OdDbLinePtr pLine1 = OdDbLine::createObject();
  pLine1->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pLine1);
  pLine1->setStartPoint(center + v1);
  pLine1->setEndPoint  (center + v2);

  double rot = OdaToRadian(75.0);
  v1.rotateBy(rot, OdGeVector3d::kZAxis);
  v2.rotateBy(rot, OdGeVector3d::kZAxis);

  OdDbLinePtr pLine2 = OdDbLine::createObject();
  pLine2->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pLine2);
  pLine2->setStartPoint(center + v1);
  pLine2->setEndPoint  (center + v2);
    
  /**********************************************************************/
  /* Create 2 Line Angular Dimensionn                                   */
  /**********************************************************************/
  OdDb2LineAngularDimensionPtr pDimension = OdDb2LineAngularDimension::createObject();
  bBTR->appendOdDbEntity(pDimension);

  /**********************************************************************/
  /* Use the default dim variables                                      */
  /**********************************************************************/
  pDimension->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/

  v3.rotateBy(rot/2.0, OdGeVector3d::kZAxis);  
  pDimension->setArcPoint(center + v3);
 
  OdGePoint3d startPoint;
  pLine1->getStartPoint(startPoint);
  pDimension->setXLine1Start(startPoint);
  
  OdGePoint3d endPoint;
  pLine1->getEndPoint(endPoint);
  pDimension->setXLine1End(endPoint);

//  pDimension->setArcPoint(endPoint + 0.45*(endPoint - startPoint).normalize());
  
  pLine2->getStartPoint(startPoint);
  pDimension->setXLine2Start(startPoint);
  
  pLine2->getEndPoint(endPoint);
  pDimension->setXLine2End(endPoint);
}
/************************************************************************/
/* Add a RadialDimensionLarge to the specified BlockTableRecord         */
/************************************************************************/
void DbFiller::addRadialDimensionLarge(const OdDbObjectId& btrId, 
                                       const int boxRow,
                                       const int boxCol,
                                       const OdDbObjectId& layerId,
                                       const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Radial"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dim Large"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create an arc to be dimensioned                                    */
  /**********************************************************************/
  OdDbArcPtr pArc = OdDbArc::createObject();
  pArc->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pArc);
 
  OdGePoint3d center = point + OdGeVector3d(w * 1.0 / 8.0, h * 1.0 / 8.0, 0.0);
  pArc->setRadius(2.0);
 
  pArc->setCenter(center);
  pArc->setStartAngle(OdaToRadian(30.0));
  pArc->setEndAngle(OdaToRadian(90.0));
  
  /**********************************************************************/
  /* Create RadialDimensionLarge                                        */
  /**********************************************************************/
  OdDbRadialDimensionLargePtr pDimension = OdDbRadialDimensionLarge::createObject();
  bBTR->appendOdDbEntity(pDimension);

  /**********************************************************************/
  /* Use the default dim variables                                      */
  /**********************************************************************/
  pDimension->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/
  OdGePoint3d centerPoint, chordPoint, overrideCenter, jogPoint, textPosition;
  
  // The centerPoint of the dimension is the center of the arc
  centerPoint = pArc->center();

  // The chordPoint of the dimension is the midpoint of the arc
  chordPoint =  centerPoint +  
    OdGeVector3d(pArc->radius(), 0.0, 0.0).rotateBy(0.5*(pArc->startAngle()+pArc->endAngle()), OdGeVector3d::kZAxis);

  // The overrideCenter is just to the right of the actual center
  overrideCenter = centerPoint + OdGeVector3d(w * 3.0/8.0, 0.0, 0.0);

  // The jogPoint is halfway between the overrideCenter and the chordCoint
  jogPoint          = overrideCenter + 0.5 * OdGeVector3d(chordPoint - overrideCenter);

  // The textPosition is along the vector between the centerPoint and the chordPoint.
  textPosition      = centerPoint + 0.7 * OdGeVector3d(chordPoint       - centerPoint);

  double jogAngle = OdaToRadian(45.0);

  pDimension->setCenter(centerPoint);
  pDimension->setChordPoint(chordPoint);
  pDimension->setOverrideCenter(overrideCenter);
  pDimension->setJogPoint(jogPoint);
  pDimension->setTextPosition(textPosition);
  pDimension->setJogAngle(jogAngle);

}
/************************************************************************/
/* Add Ordinate Dimensions to the specified BlockTableRecord            */
/************************************************************************/
void DbFiller::addOrdinateDimensions(const OdDbObjectId& btrId, 
                                     const int boxRow,
                                     const int boxCol,
                                     const OdDbObjectId& layerId,
                                     const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Ordinate"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("Dimension"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
  
  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;
  
  double dx = w / 8.0;
  double dy = h / 8.0;
  /**********************************************************************/
  /* Create a line to be dimensioned                                    */
  /**********************************************************************/
  OdDbLinePtr pLine = OdDbLine::createObject();
  pLine->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pLine);
  
  OdGePoint3d point1  = point  + OdGeVector3d(dx,  dy, 0.0);
  OdGePoint3d point2  = point1 + OdGeVector3d(0.0, 1.5, 0);
  pLine->setStartPoint(point1);
  pLine->setEndPoint  (point2);
  
  /**********************************************************************/
  /* Create the base ordinate dimension                                 */
  /**********************************************************************/
  OdDbOrdinateDimensionPtr pDimension = OdDbOrdinateDimension::createObject();
  bBTR->appendOdDbEntity(pDimension);

  /**********************************************************************/
  /* Use the default dim variables                                      */
  /**********************************************************************/
  pDimension->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/

  OdGePoint3d startPoint;
  OdGePoint3d endPoint;
  OdGePoint3d leaderEndPoint;

  pLine->getStartPoint(startPoint);
  pLine->getEndPoint(endPoint);
  
  leaderEndPoint    = startPoint + OdGeVector3d(3.0 * dx, 0, 0.0);
  pDimension->setOrigin         (startPoint);
  pDimension->setDefiningPoint  (startPoint);
  pDimension->setLeaderEndPoint (leaderEndPoint);
  pDimension->useYAxis();

  /**********************************************************************/
  /* Create an ordinate dimension                                       */
  /**********************************************************************/
  pDimension = OdDbOrdinateDimension::createObject();
  bBTR->appendOdDbEntity(pDimension);

  /**********************************************************************/
  /* Use the default dim variables                                      */
  /**********************************************************************/
  pDimension->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Set some parameters                                                */
  /**********************************************************************/
  leaderEndPoint    = endPoint + OdGeVector3d(3.0 * dx, -dy, 0.0);

  pDimension->setOrigin         (startPoint);
  pDimension->setDefiningPoint  (endPoint);
  pDimension->setLeaderEndPoint (leaderEndPoint);
  pDimension->useYAxis();
}
/************************************************************************/
/* Add a Spline to the specified BlockTableRecord                       */
/************************************************************************/
void DbFiller::addSpline(const OdDbObjectId& btrId, 
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("SPLINE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create Spline                                                      */
  /**********************************************************************/
  OdDbSplinePtr pSpline = OdDbSpline::createObject();
  pSpline->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSpline);

  /**********************************************************************/
  /* Create the fit points                                              */
  /**********************************************************************/

  double dx   = w / 8.0;
  double dy   = h / 8.0;

  OdGePoint3dArray fitPoints;
  fitPoints.push_back(point + OdGeVector3d(1.0 * dx, 1.0 * dy, 0.0));
  fitPoints.push_back(point + OdGeVector3d(3.0 * dx, 6.0 * dy, 0.0));
  fitPoints.push_back(point + OdGeVector3d(4.0 * dx, 2.0 * dy, 0.0));
  fitPoints.push_back(point + OdGeVector3d(7.0 * dx, 7.0 * dy, 0.0));

  pSpline->setFitData(
    fitPoints,                    // Fit Points
    3,                            // Degree
    0.0,                          // Fit tolerance
    OdGeVector3d(0.0, 0.0, 0.0),  // startTangent
    OdGeVector3d(1.0, 0.0, 0.0)); // endTangent
}
/************************************************************************/
/* Add some Traces to the specified BlockTableRecord                    */
/************************************************************************/
void DbFiller::addTraces(const OdDbObjectId& btrId, 
                         const int boxRow,
                         const int boxCol,
                         const OdDbObjectId& layerId,
                         const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("TRACEs"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a Trace                                                     */
  /**********************************************************************/
  OdDbTracePtr pTrace = OdDbTrace::createObject();
  pTrace->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pTrace);

  double dx   = w / 8.0;
  double dy   = h / 8.0;
  pTrace->setPointAt(0, point + OdGeVector3d(1.0 * dx, 2.0 * dx, 0.0));
  pTrace->setPointAt(1, point + OdGeVector3d(1.0 * dx, 1.0 * dx, 0.0));
  pTrace->setPointAt(2, point + OdGeVector3d(6.0 * dx, 2.0 * dx, 0.0));
  pTrace->setPointAt(3, point + OdGeVector3d(7.0 * dx, 1.0 * dx, 0.0));

  /**********************************************************************/
  /* Create a Trace                                                     */
  /**********************************************************************/
  pTrace = OdDbTrace::createObject();
  pTrace->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pTrace);

  pTrace->setPointAt(0, point + OdGeVector3d(6.0 * dx, 2.0 * dx, 0.0));
  pTrace->setPointAt(1, point + OdGeVector3d(7.0 * dx, 1.0 * dx, 0.0));
  pTrace->setPointAt(2, point + OdGeVector3d(6.0 * dx, 7.0 * dy, 0.0));
  pTrace->setPointAt(3, point + OdGeVector3d(7.0 * dx, 7.0 * dy, 0.0));
   
}
/************************************************************************/
/* Add an Mline to the specified BlockTableRecord                       */
/************************************************************************/
void DbFiller::addMLine(const OdDbObjectId& btrId, 
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);
  OdDbDatabase* pDb = btrId.database();

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the labels                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("MLINE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of MLine                                 */
  /**********************************************************************/
  point.x +=  w/10.0;
  point.y -=  h/2;

  /**********************************************************************/
  /* Create an MLine and add it to the database                         */
  /**********************************************************************/
  OdDbMlinePtr pMLine = OdDbMline::createObject();
  pMLine->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pMLine);

  /**********************************************************************/
  /* Open the MLineStyle dictionary, and set the style                  */
  /**********************************************************************/
  OdDbDictionaryPtr pMLDic = pDb->getMLStyleDictionaryId().safeOpenObject();
  pMLine->setStyle(pMLDic->getAt(OD_T("OdaStandard")));

  /**********************************************************************/
  /* Add some segments                                                  */
  /**********************************************************************/
  point.y -= h/2.2;
  pMLine->appendSeg(point);

  point.y += h/3.0;
  pMLine->appendSeg(point);

  point.y += h/5.0;
  point.x += w/4.0;
  pMLine->appendSeg(point);

  point.x += w/4.0;
  pMLine->appendSeg(point);

  point.y += h/3.0;
  pMLine->appendSeg(point);

  point.x += w/3;
  pMLine->appendSeg(point);

  point.y -= h/2;
  pMLine->appendSeg(point);

  point.x -= w/4;
  point.y -= h/3;
  pMLine->appendSeg(point);
}

/************************************************************************/
/* Add a Polyline to the specified BlockTableRecord                     */
/************************************************************************/
void DbFiller::addPolyline(const OdDbObjectId& btrId, 
                           const int boxRow,
                           const int boxCol,
                           const OdDbObjectId& layerId,
                           const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("LWPOLYLINE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a polyline                                                  */
  /**********************************************************************/
  OdDbPolylinePtr pPolyline = OdDbPolyline::createObject();
  pPolyline->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pPolyline);

  /**********************************************************************/
  /* Create the vertices                                                */
  /**********************************************************************/

  double dx   = w / 8.0;
  double dy   = h / 8.0;

  OdGePoint2d point2d(point.x + 1.5*dx, point.y + 3.0*dy);

  pPolyline->addVertexAt(0, point2d);

  point2d.y -= 0.5*dy;
  pPolyline->addVertexAt(1, point2d);
  pPolyline->setBulgeAt(1, 1.0);

  point2d.x += 5.0*dx;
  pPolyline->addVertexAt(2, point2d);

  point2d.y += 4.0*dy;
  pPolyline->addVertexAt(3, point2d);

  point2d.x -= 1.0*dx;
  pPolyline->addVertexAt(4, point2d);

  point2d.y -= 4.0*dy;
  pPolyline->addVertexAt(5, point2d);
  pPolyline->setBulgeAt(5, -1.0);
  
  point2d.x -= 3.0*dx;
  pPolyline->addVertexAt(6, point2d);

  point2d.y += 0.5*dy;
  pPolyline->addVertexAt(7, point2d);

  pPolyline->setClosed(true);
}

/************************************************************************/
/* Add Arc Aligned Text to the specified BlockTableRecord               */
/************************************************************************/
void DbFiller::addArcText(const OdDbObjectId& btrId, 
                          const int boxRow,
                          const int boxCol,
                          const OdDbObjectId& layerId,
                          const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
//  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("ARCALIGNED-"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  addTextEnt(bBTR, 
    point + m_textOffset + m_textLine, point + m_textOffset + m_textLine, 
    OD_T("TEXT"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );


  /**********************************************************************/
  /* Create an arc                                                       */
  /**********************************************************************/
  OdDbArcPtr pArc = OdDbArc::createObject();
  pArc->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pArc);

//  double dx   = w / 8.0;
  double dy   = h / 8.0;

  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol) + OdGeVector3d(0.0, -2.0*dy, 0);
  pArc->setCenter(center);
  pArc->setRadius(3.0*dy);
  pArc->setStartAngle(OdaToRadian(45.0));
  pArc->setEndAngle(OdaToRadian(135.0));

  /**********************************************************************/
  /* Create the ArcAlignedText                                          */
  /**********************************************************************/
  OdDbArcAlignedTextPtr pArcText = OdDbArcAlignedText::createObject();
  pArcText->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pArcText);

  pArcText->setTextString(OD_T("ArcAligned"));
  pArcText->setArcId(pArc->objectId());
  pArcText->setTextStyle(styleId);
}

/************************************************************************/
/* Add a Wipeout to to the specified BlockTableRecord                   */
/************************************************************************/
void DbFiller::addWipeout(const OdDbObjectId& btrId, 
                          const int boxRow,
                          const int boxCol,
                          const OdDbObjectId& layerId,
                          const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the lower-left corner and center of the box                    */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                     */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("WIPEOUT"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Get the lower-left corner of the box                               */
  /**********************************************************************/
  point.y -= h;

  /**********************************************************************/
  /* Create a hatch object to be wiped out                              */
  /**********************************************************************/
  OdDbHatchPtr pHatch = OdDbHatch::createObject();
  pHatch->setDatabaseDefaults(bBTR->database());
  OdDbObjectId hatchId = bBTR->appendOdDbEntity(pHatch);

  /**********************************************************************/
  /* Create a hatch object to be wiped out                              */
  /**********************************************************************/
  pHatch->setAssociative(true);
  pHatch->setPattern(OdDbHatch::kUserDefined, OD_T("_USER"));
  pHatch->setPatternSpace(0.125);
  pHatch->setPatternAngle(0.5); // near 30 degrees
  pHatch->setPatternDouble(true); // Cross hatch
  pHatch->setHatchStyle(OdDbHatch::kNormal);

  /**********************************************************************/
  /* Create an outer loop for the hatch                                 */
  /**********************************************************************/
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());
  OdDbObjectIdArray loopIds;
  loopIds.push_back(bBTR->appendOdDbEntity(pCircle));
  pCircle->setCenter(center);
  pCircle->setRadius( odmin(w, h) * 0.4);
  pHatch->appendLoop(OdDbHatch::kDefault, loopIds);

  /**********************************************************************/
  /* Create the wipeout                                                  */
  /**********************************************************************/
  OdDbWipeoutPtr pWipeout = OdDbWipeout::createObject();
  pWipeout->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pWipeout);

  OdGePoint3dArray boundary;
  boundary.append(center + OdGeVector3d(-w * 0.4, -h *0.4, 0.0));
  boundary.append(center + OdGeVector3d( w * 0.4, -h *0.4, 0.0));
  boundary.append(center + OdGeVector3d( 0.0,      h *0.4, 0.0));
  boundary.append(center + OdGeVector3d(-w * 0.4, -h *0.4, 0.0));

  pWipeout->setBoundary(boundary);

  pWipeout->setDisplayOpt(OdDbRasterImage::kShow,true);
  pWipeout->setDisplayOpt(OdDbRasterImage::kClip,true);
  pWipeout->setDisplayOpt(OdDbRasterImage::kShowUnAligned,true);
  pWipeout->setDisplayOpt(OdDbRasterImage::kTransparent,false);
}

/************************************************************************/
/* Add a Table to the specified BlockTableRecord                        */
/************************************************************************/
void DbFiller::addTable(const OdDbObjectId& btrId, 
                        const OdDbObjectId& addedBlockId, 
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr  pRecord = btrId.safeOpenObject(OdDb::kForWrite);
  // OdDbBlockTableRecordPtr bBTR = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the lower-left corner and center of the box                    */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Create the Table                                                  */
  /**********************************************************************/
  OdDbTablePtr pAcadTable = OdDbTable::createObject();
  OdDbObjectId tableId = pRecord->appendOdDbEntity(pAcadTable);
  /**********************************************************************/
  /* This entity will later get its own viewport                        */
  /**********************************************************************/
  m_layoutEntities.push_back(tableId);

  /**********************************************************************/
  /* Set the parameters                                                 */
  /**********************************************************************/
  pAcadTable->setDatabaseDefaults(pRecord->database());
  pAcadTable->setNumColumns(3);
  pAcadTable->setNumRows(4);

  pAcadTable->generateLayout();
  pAcadTable->setColumnWidth(w/pAcadTable->numColumns()); 
  pAcadTable->setRowHeight(h/pAcadTable->numRows());

  pAcadTable->setPosition(point);
  pAcadTable->setTextStyle(styleId);

  pAcadTable->setTextHeight(0.500*pAcadTable->rowHeight(0), OdDb::kTitleRow);
  pAcadTable->setTextHeight(0.300*pAcadTable->rowHeight(1), OdDb::kHeaderRow);
  pAcadTable->setTextHeight(0.250*pAcadTable->rowHeight(2), OdDb::kDataRow);

  /**********************************************************************/
  /* Set the alignments                                                 */
  /**********************************************************************/
  for (int row = 1; row < (int) pAcadTable->numRows(); row ++)
  {
   for (int col = 0; col < (int) pAcadTable->numColumns(); col ++)
   {
     pAcadTable->setAlignment(row, col, OdDb::kMiddleCenter);
   }
  }
   
  /**********************************************************************/
  /* Define the title row                                               */
  /**********************************************************************/
  pAcadTable->mergeCells(0,0,0,pAcadTable->numColumns() - 1);
  pAcadTable->setTextString(0,0, OD_T("Title of TABLE"));

  /**********************************************************************/
  /* Define the header row                                              */
  /**********************************************************************/
  pAcadTable->setTextString(1,0, OD_T("Header0"));
  pAcadTable->setTextString(1,1, OD_T("Header1"));
  pAcadTable->setTextString(1,2, OD_T("Header2"));

  /**********************************************************************/
  /* Define the first data row                                          */
  /**********************************************************************/
  pAcadTable->setTextString(2,0, OD_T("Data0"));
  pAcadTable->setTextString(2,1, OD_T("Data1"));
  pAcadTable->setTextString(2,2, OD_T("Data2"));

  /**********************************************************************/
  /* Define the second data row                                         */
  /**********************************************************************/
  pAcadTable->setCellType(3,0, OdDb::kBlockCell);
  pAcadTable->setBlockTableRecordId(3, 0, addedBlockId);
  pAcadTable->setBlockScale(3,0, 1.0);
  pAcadTable->setAutoScale(3,0, true);
  pAcadTable->setBlockRotation(3,0, 0.0);

  pAcadTable->setTextString(3,1, OD_T("<-Block Cell->"));

  pAcadTable->setCellType(3,2, OdDb::kBlockCell);
  pAcadTable->setBlockTableRecordId(3, 2, addedBlockId);
  pAcadTable->setAutoScale(3,2, true);
  pAcadTable->setBlockRotation(3,2, OdaToRadian(30.0));

  pAcadTable->recomputeTableBlock();

  /**********************************************************************/
  /* Add the label                                                     */
  /**********************************************************************/
  addTextEnt(pRecord,
    point + m_textOffset, point + m_textOffset, 
    OD_T("ACAD_TABLE"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
}

/************************************************************************/
/* Add a Text with Field to the specified BlockTableRecord              */
/************************************************************************/
void DbFiller::addTextWithField(const OdDbObjectId& btrId, 
                        const int boxRow,
                        const int boxCol,
                        const OdDbObjectId& layerId,
                        const OdDbObjectId& styleId,
                        const OdDbObjectId& noteStyleId)
{
  OdDbBlockTableRecordPtr  pRecord = btrId.safeOpenObject(OdDb::kForWrite);

  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);

  //  double dx = w/16.0;
//  double dy = h/12.0;

  OdGePoint3d  textPos1 = point;
  textPos1.x += w/15.0;
  textPos1.y -= h/3.0;

  OdGePoint3d  textPos2 = point;
  textPos2.x += w/15.0;
  textPos2.y -= h/2.0+h/12.0;

  double textHeight = m_EntityBoxes.getHeight()/12.0;

  /**********************************************************************/
  /* Prepare the text entities                                           */
  /**********************************************************************/
  OdDbTextPtr pText1 = OdDbText::createObject();
  OdDbObjectId textId = pRecord->appendOdDbEntity(pText1);
  OdDbTextPtr pText2 = OdDbText::createObject();
  OdDbObjectId textId2 = pRecord->appendOdDbEntity(pText2);

  pText1->setPosition(textPos1);
  pText1->setHeight(textHeight);
  pText2->setPosition(textPos2);
  pText2->setHeight(textHeight);
  if (!styleId.isNull())
  {
    pText1->setTextStyle(styleId);
    pText2->setTextStyle(styleId);
  }

  /**********************************************************************/
  /* Create field objects                                               */
  /**********************************************************************/
  OdDbFieldPtr pTextField1 = OdDbField::createObject();
  OdDbFieldPtr pField1_1 = OdDbField::createObject();

  OdDbFieldPtr pTextField2 = OdDbField::createObject();
  OdDbFieldPtr pField2_1 = OdDbField::createObject();
  OdDbFieldPtr pField2_2 = OdDbField::createObject();

  /**********************************************************************/
  /* Set field objects                                                  */
  /**********************************************************************/
  OdDbObjectId textFldId1 = pText1->setField(OD_T("TEXT"), pTextField1);
  OdDbObjectId fldId1_1 = pTextField1->setField(OD_T(""), pField1_1);

  OdDbObjectId textFldId2 = pText2->setField(OD_T("TEXT"), pTextField2);

  /**********************************************************************/
  /* Set field property                                                 */
  /**********************************************************************/

  pField1_1->setEvaluationOption(OdDbField::kAutomatic);
  OdString fc1(OD_T("\\AcVar Comments"));
  pField1_1->setFieldCode(fc1);

  pTextField1->setEvaluationOption(OdDbField::kAutomatic);
  OdString fc2(OD_T("%<\\_FldIdx 0>%"));
  pTextField1->setFieldCode(fc2, OdDbField::FieldCodeFlag(OdDbField::kTextField | OdDbField::kPreserveFields));

  /**********************************************************************/
  /* Evaluate field                                                     */
  /**********************************************************************/
  pField1_1->evaluate(OdDbField::kAutomatic);

  OdDbFieldArray fldArray;
  fldArray.push_back(pField2_1);
  fldArray.push_back(pField2_2);

  pTextField2->setEvaluationOption(OdDbField::kAutomatic);
  OdString fc3(OD_T("Date %<\\_FldIdx 0>% Time %<\\_FldIdx 1>%"));
  pTextField2->setFieldCode(fc3, OdDbField::FieldCodeFlag(OdDbField::kTextField), &fldArray);

  pField2_1->setEvaluationOption(OdDbField::kAutomatic);
  OdString fc4(OD_T("\\AcVar Date \\f M/dd/yyyy"));
  pField2_1->setFieldCode(fc4);

  pField2_2->setEvaluationOption(OdDbField::kAutomatic);
  OdString fc5(OD_T("\\AcVar Date \\f h:mm tt"));
  pField2_2->setFieldCode(fc5);

  /**********************************************************************/
  /* Evaluate fields                                                    */
  /**********************************************************************/
  pField2_1->evaluate(OdDbField::kAutomatic);
  pField2_2->evaluate(OdDbField::kAutomatic);

  /**********************************************************************/
  /* Creating MTEXT with hiperlink                                      */
  /**********************************************************************/

  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(pRecord->database());
  pRecord->appendOdDbEntity(pMText);

  /**********************************************************************/
  /* Set some properties                                                */
  /**********************************************************************/

  pMText->setLocation(point + OdGeVector3d( w/15.0, -h*3.0/4.0, 0 ));
  pMText->setTextHeight(0.2);
  pMText->setAttachment(OdDbMText::kTopLeft);
  pMText->setContents(OD_T("Sample Hyperlink"));
  pMText->setWidth(w * 7.0 / 8.0);
  pMText->setTextStyle(styleId);
  
  OdDbFieldPtr pTextField3 = OdDbField::createObject();
  OdDbFieldPtr pField3_1 = OdDbField::createObject();
  OdDbFieldPtr pField3_2 = OdDbField::createObject();

  OdDbObjectId textFldId3 = pMText->setField(OD_T("TEXT"), pTextField3);
  OdDbObjectId fldId3_1 = pTextField3->setField(OD_T(""), pField3_1);
  OdDbObjectId fldId3_2 = pTextField3->setField(OD_T(""), pField3_2);

  pField3_1->setEvaluationOption(OdDbField::kAutomatic);
  OdString fc6(OD_T("\\AcVar \\href \"http://forum.opendesign.com/showthread.php?14172-How-do-I-add-a-hyperlink-to-an-MTEXT-entity##Open Design Alliance Forum > Teigha, C++ version#0\""));
  pField3_1->setFieldCode(fc6);

  OdDbEntityHyperlinkPEPtr    hpe = pField3_1;
  OdDbHyperlinkCollectionPtr  urls = hpe->getHyperlinkCollection(pField3_1);
  
  urls->addTail(L"http://forum.opendesign.com/showthread.php?14172-How-do-I-add-a-hyperlink-to-an-MTEXT-entity##ForumLink#0\"", 
    L"Open Design Alliance Forum > Teigha, C++ version");
  
  hpe->setHyperlinkCollection(pField3_1, urls);


  pField3_2->setEvaluationOption(OdDbField::kAutomatic);
  OdString fc8(OD_T("\\AcVar \\href \"http://www.opendesign.com##Open Design Alliance #1\""));
  pField3_2->setFieldCode(fc8);

  OdDbEntityHyperlinkPEPtr    hpe1 = pField3_2;
  OdDbHyperlinkCollectionPtr  urls1 = hpe->getHyperlinkCollection(pField3_2);

  urls1->addTail(L"http://www.opendesign.com##OpenDesignAlliance#1\"", 
    L"Open Design Alliance");

  hpe1->setHyperlinkCollection(pField3_2, urls1);

  pTextField3->setEvaluationOption(OdDbField::kAutomatic);
  OdString fc7(OD_T("HyperLink sample %<\\_FldIdx 0>%  and  %<\\_FldIdx 1>%"));
  pTextField3->setFieldCode(fc7, OdDbField::FieldCodeFlag(OdDbField::kTextField | OdDbField::kPreserveFields));

  pField3_1->evaluate(OdDbField::kAutomatic);
  pField3_2->evaluate(OdDbField::kAutomatic);

  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(pRecord,
    point + m_textOffset, point + m_textOffset, 
    OD_T("FIELDS"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, noteStyleId );
}


#if defined(_MSC_VER) && !defined(_WINRT)
/**********************************************************************/
/* Minimal code to create OLE compound document from any file.        */
/**********************************************************************/
#include "ole2.h"
#include "objidl.h"
#include "DbOle2Frame.h"
#include "OleItemHandler.h"
#include "FlatMemStream.h"
#include "StaticRxObject.h"

OdDbOle2FramePtr CreateFromFile(const OdString& fileName)
{
  OdDbOle2FramePtr pOle2Frame;
  HRESULT hr = OleInitialize(0);
  if(SUCCEEDED(hr))
  {
    ILockBytes *pLockBytes;
    hr = CreateILockBytesOnHGlobal(NULL, TRUE, &pLockBytes);
    if(SUCCEEDED(hr))
    {
      IStorage *pStrg;
      hr = StgCreateDocfileOnILockBytes(pLockBytes,
        STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &pStrg);
      if(SUCCEEDED(hr))
      {
        IOleObject *pObj;
        hr = OleCreateFromFile(CLSID_NULL, OdString(fileName), IID_IOleObject, OLERENDER_DRAW, 0, 0, pStrg, (void**)&pObj);
        if(SUCCEEDED(hr))
        {
          IPersistStorage* pPersist;
          hr = pObj->QueryInterface(IID_IPersistStorage, (void**)&pPersist);
          if(pPersist!=0)
          {
            hr = OleSave(pPersist, pStrg, FALSE);
            pPersist->Release();
          }

          SIZE size;
          hr = pObj->GetExtent(DVASPECT_CONTENT, &size);
          if (FAILED(hr))
            size.cx = size.cy = 1000;

          pObj->Release();

          hr = pStrg->Commit(STGC_DEFAULT);
        
          HGLOBAL hGlobal = 0;
          hr = GetHGlobalFromILockBytes(pLockBytes, &hGlobal);
        
          DWORD dwSize = (DWORD)GlobalSize(hGlobal);
          void *pCompoundDocData = GlobalLock(hGlobal);
        
          /**********************************************************************/
          /* Create OdDbOle2Frame and copy OLE object's data into it.           */
          /**********************************************************************/

          try
          {
            pOle2Frame = OdDbOle2Frame::createObject();
            OdOleItemHandlerPtr pHandler = pOle2Frame->getItemHandler();
            OdStaticRxObject<OdFlatMemStream> strm;

            strm.init(pCompoundDocData, dwSize);
            pHandler->setCompoundDocument(dwSize, strm);

            pHandler->setDrawAspect(OdOleItemHandler::kContent);

            pOle2Frame->unhandled_setHimetricSize(OdUInt16(size.cx), OdUInt16(size.cy));
          }
          catch(...)
          {
            GlobalUnlock(hGlobal);
            pStrg->Release();
            pLockBytes->Release();
            OleUninitialize();
            throw;
          }
          GlobalUnlock(hGlobal);
        }
        pStrg->Release();
      }
      pLockBytes->Release();
    }
    OleUninitialize();
  }
  return pOle2Frame;
}

#endif

/**********************************************************************/
/* Save persistent OLE object data to file that can be used later for */
/* creating OLE2 frame entity on any platform.                        */
/**********************************************************************/
void saveOleData(const OdString& fileName, const OdOleItemHandler* pHandler)
{
  try
  {
    OdStreamBufPtr pFile =
      odSystemServices()->createFile(fileName,
      Oda::kFileWrite,
      Oda::kShareDenyReadWrite,
      Oda::kCreateAlways);
    pHandler->getCompoundDocument(*pFile.get());
  }
  catch(const OdError& ) //ignore errors
  {
  }
}


/************************************************************************/
/* Prefix a file name with the Current Directory                        */
/************************************************************************/
OdString DbFiller::inCurrentFolder(const OdString fileName)
{
#if defined(_MSC_VER) && !defined(_WINRT)
  if(fileName.findOneOf(OD_T("\\/"))==-1)
  {
    TCHAR buf[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, buf);
    OdString sPath(buf);
    sPath.trimRight(OD_T("\\/"));
    sPath += '/';
    sPath += fileName;
    return sPath;
  }
  else
#endif
  {
    return fileName;
  }
}

/************************************************************************/
/* Add an OLE object to the specified BlockTableRecord                  */
/************************************************************************/
void DbFiller::addOLE2FrameFromFile(const OdDbObjectId& btrId,
                                    const int boxRow,
                                    const int boxCol,
                                    const OdString& fileName,
                                    const OdDbObjectId& layerId,
                                    const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr  pBlock = btrId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the lower-left corner and center of the box                    */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  OdGePoint3d center = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);

#if defined(_MSC_VER) && !defined(_WINRT)
  /**********************************************************************/
  /* Create an ole2frame entity from arbitrary file using Windows OLE RT*/
  /**********************************************************************/
  // OleCreateFromFile() requires a full path
  OdDbOle2FramePtr pOle2Frame = CreateFromFile(inCurrentFolder(fileName));

  // OLE compound document can be saved to file for later use (on any platform).
  //saveOleData(fileName + OD_T(".ole"), pOle2Frame->getItemHandler());
#else
  /**********************************************************************/
  /* Create an ole2frame entity in a platform-neutral manner            */
  /* Important: open file that is a compound document.                  */
  /* OLE2 frame can't be created from arbitrary file such way.          */
  /**********************************************************************/
  OdDbOle2FramePtr pOle2Frame;

  try
  {
    OdStreamBufPtr pFile =
      odSystemServices()->createFile(fileName + OD_T(".ole"), Oda::kFileRead,
                                    Oda::kShareDenyReadWrite, Oda::kOpenExisting);

    pOle2Frame = OdDbOle2Frame::createObject();

    OdOleItemHandlerPtr pHandler = pOle2Frame->getItemHandler();

    pHandler->setCompoundDocument(pFile->length(), *pFile.get());

    pHandler->setDrawAspect(OdOleItemHandler::kContent);

    pOle2Frame->unhandled_setHimetricSize(6879, 3704);
  }
  catch (OdError& e)
  {
    odPrintConsoleString(L"Ole file: %ls.ole not found, no OdDbOle2Frame entity created.\n", fileName.c_str());
  }
#endif
  if(pOle2Frame.get())
  {
    pOle2Frame->setDatabaseDefaults(pBlock->database());
    pBlock->appendOdDbEntity(pOle2Frame);
    
    /**********************************************************************/
    /* Add the label                                                      */
    /**********************************************************************/
    addTextEnt(pBlock, point + m_textOffset, point + m_textOffset, 
      OdString(OD_T("OLE2: ")) + pOle2Frame->getUserType(), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );
    
    
    /**********************************************************************/
    /* Inscribe OLE frame in entity box                                   */
    /**********************************************************************/
    h += m_textOffset.y;
    h -= (m_textSize * 1.5);
    center.y += (m_textOffset.y / 2.);
    center.y -= (m_textSize * 1.5 / 2.);
    
    h *= 0.95;
    w *= 0.95;
    
    h /= 2.0;
    w /= 2.0;
    
    double oh = pOle2Frame->unhandled_himetricHeight();
    double ow = pOle2Frame->unhandled_himetricWidth();
    if(oh/ow < h/w)
    {
      h = w * oh / ow;
    }
    else
    {
      w = h * ow / oh;
    }
    
    OdRectangle3d rect;
    rect.upLeft.x  = rect.lowLeft.x  = center.x - w;
    rect.upLeft.y  = rect.upRight.y  = center.y + h;
    rect.upRight.x = rect.lowRight.x = center.x + w;
    rect.lowLeft.y = rect.lowRight.y = center.y - h;
    pOle2Frame->setPosition(rect);
  } else
  {
    addTextEnt(pBlock,
    point + m_textOffset, point + m_textOffset,
    OD_T("OLE2Frame"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);
  }
}

void DbFiller::addDwfUnderlay(const OdDbObjectId& blockId, 
                              const int boxRow, 
                              const int boxCol, 
                              const OdDbObjectId& layerId, 
                              const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = blockId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  double w    = m_EntityBoxes.getWidth(boxRow, boxCol);
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Dwf reference"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Create the Dwf definition                                          */
  /**********************************************************************/
  OdDbDwfDefinitionPtr pDwfDef = OdDbDwfDefinition::createObject();
  OdString itemName(OD_T("Unsaved Drawing-Model"));
  pDwfDef->setSourceFileName(OD_T("OdWriteEx.dwf"));
  pDwfDef->setItemName(itemName);

  // Post to database
  OdDbObjectId idDef = pDwfDef->postDefinitionToDb(blockId.database(),
                                                      OD_T("OdWriteEx - ") + itemName);

  /**********************************************************************/
  /* Create the Dwf reference                                           */
  /**********************************************************************/  
  OdDbDwfReferencePtr pDwfRef = OdDbDwfReference::createObject();
  pDwfRef->setDatabaseDefaults(bBTR->database());

  /**********************************************************************/
  /* Add the Dwf reference to the database                              */
  /**********************************************************************/
  bBTR->appendOdDbEntity(pDwfRef);
  
  /**********************************************************************/
  /* Set the Dwf reference's parameters                                 */
  /**********************************************************************/
  pDwfRef->setDefinitionId(idDef);
  pDwfRef->setPosition(point + OdGeVector3d(-w/4, -h/2, 0.0));
  pDwfRef->setScaleFactors(OdGeScale3d(0.001));
}

void DbFiller::addDgnUnderlay(const OdDbObjectId& blockId, 
                              const int boxRow, 
                              const int boxCol, 
                              const OdDbObjectId& layerId, 
                              const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = blockId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Dgn reference"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Create the Dwf definition                                          */
  /**********************************************************************/
  OdDbDgnDefinitionPtr pDgnDef = OdDbDgnDefinition::createObject();
  OdString itemName(OD_T("Model"));
  pDgnDef->setSourceFileName(OD_T("OdWriteEx.dgn"));
  pDgnDef->setItemName(itemName);

  // Post to database
  OdDbObjectId idDef = pDgnDef->postDefinitionToDb(blockId.database(),
                                                      OD_T("OdWriteEx - ") + itemName);

  /**********************************************************************/
  /* Create the Dgn reference                                           */
  /**********************************************************************/  
  OdDbDgnReferencePtr pDgnRef = OdDbDgnReference::createObject();
  pDgnRef->setDatabaseDefaults(bBTR->database());

  /**********************************************************************/
  /* Add the Dgn reference to the database                              */
  /**********************************************************************/
  bBTR->appendOdDbEntity(pDgnRef);
  
  /**********************************************************************/
  /* Set the Dgn reference's parameters                                 */
  /**********************************************************************/
  pDgnRef->setDefinitionId(idDef);
  pDgnRef->setPosition(point + OdGeVector3d(0., -h, 0.0));
  pDgnRef->setScaleFactors(OdGeScale3d(0.0004));
}

void DbFiller::addPdfUnderlay(const OdDbObjectId& blockId, 
                              const int boxRow, 
                              const int boxCol, 
                              const OdDbObjectId& layerId, 
                              const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = blockId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);
  double h    = m_EntityBoxes.getHeight();
  
  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    point + m_textOffset, point + m_textOffset, 
    OD_T("Pdf reference"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Create the Pdf definition                                          */
  /**********************************************************************/
  OdDbPdfDefinitionPtr pPdfDef = OdDbPdfDefinition::createObject();
  OdString itemName(OD_T("1"));
  pPdfDef->setSourceFileName(OD_T("OdWriteEx.pdf"));
  pPdfDef->setItemName(itemName);

  // Post to database
  OdDbObjectId idDef = pPdfDef->postDefinitionToDb(blockId.database(),
                                                      OD_T("OdWriteEx - ") + itemName);

  /**********************************************************************/
  /* Create the Pdf reference                                           */
  /**********************************************************************/  
  OdDbPdfReferencePtr pPdfRef = OdDbPdfReference::createObject();
  pPdfRef->setDatabaseDefaults(bBTR->database());

  /**********************************************************************/
  /* Add the Pdf reference to the database                              */
  /**********************************************************************/
  bBTR->appendOdDbEntity(pPdfRef);
  
  /**********************************************************************/
  /* Set the Pdf reference's parameters                                 */
  /**********************************************************************/
  pPdfRef->setDefinitionId(idDef);
  pPdfRef->setPosition(point + OdGeVector3d(0., -h, 0.));
  pPdfRef->setScaleFactors(OdGeScale3d(0.2));
}


void DbFiller::addPointCloud(OdDbDatabase* pDb, 
                             const int boxRow, 
                             const int boxCol, 
                             const OdDbObjectId& layerId, 
                             const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbObjectId blockId = pDb->getModelSpaceId();
  OdDbBlockTableRecordPtr bBTR = blockId.safeOpenObject(OdDb::kForWrite);

  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);

  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset, 
    OD_T("Point Cloud"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );


  /**********************************************************************/
  /* File name for PCG file                                             */
  /**********************************************************************/
  OdString pcgFileName = "bunny.pcg";

  OdGePoint3d p1, p2;
  OdGeExtents3d extents(p1, p2);
  OdGeVector3d translation;

  /**********************************************************************/
  /* Start parsing PCG file                                             */
  /**********************************************************************/
  if(::odSystemServices()->accessFile(pcgFileName, Oda::kFileRead))
  {
    OdRdFileBuf pcgFile(pcgFileName);

    bool canRead = true;
    // read pcg file version
    pcgFile.seek(3, OdDb::kSeekFromStart);
    OdUInt16 pcgFileVer = OdPlatformStreamer::rdInt16(pcgFile);
    //seek to extents info
    if (pcgFileVer == 0x0003)
      pcgFile.seek(7, OdDb::kSeekFromStart);
    else if (pcgFileVer == 0x0004)
      pcgFile.seek(19, OdDb::kSeekFromStart);
    else
      canRead = false;

    if (canRead)
    {
      // extents reading
      double x = OdPlatformStreamer::rdDouble(pcgFile);
      double y = OdPlatformStreamer::rdDouble(pcgFile);
      double z = OdPlatformStreamer::rdDouble(pcgFile);
      p1.set(x, y, z);

      x = OdPlatformStreamer::rdDouble(pcgFile);
      y = OdPlatformStreamer::rdDouble(pcgFile);
      z = OdPlatformStreamer::rdDouble(pcgFile);
      p2.set(x, y, z);
      extents.set(p1, p2);

      // translation reading
      x = OdPlatformStreamer::rdDouble(pcgFile);
      y = OdPlatformStreamer::rdDouble(pcgFile);
      z = OdPlatformStreamer::rdDouble(pcgFile);
      translation.set(x, y, z);
    }
  }
  /**********************************************************************/
  /* Calculate point cloud scale and center position                    */
  /**********************************************************************/
  double scale = 1;
  if (extents.maxPoint().x - extents.minPoint().x != 0)
    scale = m_EntityBoxes.getWidth(boxRow, boxCol) / (extents.maxPoint().x - extents.minPoint().x);

  OdGePoint3d location = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  translation = location.asVector() - translation * scale;
  location.set(translation.x, translation.y, translation.z);

  /**********************************************************************/
  /* Load point cloud module                                            */
  /**********************************************************************/

  OdRxModulePtr pPointCloudModule = ::odrxDynamicLinker()->loadApp(DbPointCloudObjModuleName);
  if (!pPointCloudModule.isNull())
  {
    /**********************************************************************/
    /* Create point cloud entity                                          */
    /**********************************************************************/
    OdDbPointCloudPtr pPointCloud;
    oddbCreatePointCloudEntity(bBTR, pPointCloud, pcgFileName, OdString::kEmpty, location, scale);

    /**********************************************************************/
    /* Append extents info to point cloud                                 */
    /**********************************************************************/
    pPointCloud->setExtents(extents);
  }

}

void DbFiller::addPointCloudEx(OdDbDatabase* pDb, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbObjectId blockId = pDb->getModelSpaceId();
  OdDbBlockTableRecordPtr bBTR = blockId.safeOpenObject(OdDb::kForWrite);

  OdGePoint3d point = m_EntityBoxes.getBox(boxRow, boxCol);

  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR, point + m_textOffset, point + m_textOffset,
    OD_T("Point Cloud Ex"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);

  /**********************************************************************/
  /* File name for RCS file                                             */
  /**********************************************************************/
  OdString rcsFileName = "bunny2.rcs";

  OdGePoint3d p1, p2;
  OdGeExtents3d extents(p1, p2);
  OdGeVector3d translation;

  if (::odSystemServices()->accessFile(rcsFileName, Oda::kFileRead))
  {
    /**********************************************************************/
    /* Start parsing RCS file                                             */
    /**********************************************************************/

    OdRxRcsFileServicesPtr pRcsFileServices;
    pRcsFileServices = odrxDynamicLinker()->loadApp(RX_RCS_FILE_SERVICES);
    if (!pRcsFileServices.isNull())
    {
      OdPointCloudScanDatabasePtr pScanDb;
      pScanDb = pRcsFileServices->readRcsFile(rcsFileName);
      if (!pScanDb.isNull())
      {
        extents = pScanDb->getTransformedExtents();
        translation = pScanDb->getTranslation();
      }
    }

    /**********************************************************************/
    /* Calculate point cloud scale and center position                    */
    /**********************************************************************/
    double scale = 1;
    if (extents.maxPoint().x - extents.minPoint().x != 0)
      scale = m_EntityBoxes.getWidth(boxRow, boxCol) / (extents.maxPoint().x - extents.minPoint().x);

    OdGePoint3d location = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
    translation = location.asVector() - translation * scale;
    location.set(translation.x, translation.y, translation.z);

    /**********************************************************************/
    /* Load point cloud module                                            */
    /**********************************************************************/

    OdRxModulePtr pPointCloudModule = ::odrxDynamicLinker()->loadApp(DbPointCloudObjModuleName);
    if (!pPointCloudModule.isNull())
    {
      /**********************************************************************/
      /* Create point cloud entity                                          */
      /**********************************************************************/
      OdDbPointCloudExPtr pPointCloud;
      oddbCreatePointCloudExEntity(bBTR, pPointCloud, rcsFileName, location, scale);
    }
  }
}

void DbFiller::addGeoStuff(OdDbDatabase* pDb, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Check modules loading                                              */
  /**********************************************************************/
  if (odrxDynamicLinker()->loadModule(OdGeolocationObjModuleName).isNull()
    || odrxDynamicLinker()->loadModule(OdGeoDataModuleName).isNull()
    || odrxDynamicLinker()->loadModule(OdGeoMapPEModuleName).isNull()
    )
  {
    return;
  }

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d ptUpperLeft = m_EntityBoxes.getBox(boxRow, boxCol);
  double h = m_EntityBoxes.getHeight();
  double w = m_EntityBoxes.getWidth(boxRow, boxCol);

  /**********************************************************************/
  /* Open the BlockTableRecord                                          */
  /**********************************************************************/
  OdDbObjectId modelSpaceId = pDb->getModelSpaceId();
  OdDbBlockTableRecordPtr bBTR = modelSpaceId.safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset, 
    OD_T("GeoStuff"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId );

  /**********************************************************************/
  /* Add the GeoData                                                    */
  /**********************************************************************/
  OdDbGeoDataPtr geoData = OdDbGeoData::createObject();
  geoData->setBlockTableRecordId(modelSpaceId);
  OdDbObjectId objId;
  ODA_VERIFY(eOk == geoData->postToDb(objId));
  ODA_VERIFY(eOk == geoData->setCoordinateSystem("WORLD-MERCATOR"));
  geoData->setCoordinateType(OdDbGeoData::kCoordTypGrid);
  geoData->setDesignPoint(OdGePoint3d(ptUpperLeft.x + w/3., ptUpperLeft.y - h/3., 0.));
  /**********************************************************************/
  /* Add the GeoMap                                                     */
  /**********************************************************************/
  OdDbObjectIdArray arObjId;
  {
    OdGePoint3d ptMin(ptUpperLeft), ptMax(ptUpperLeft);
    ptMin.y -= 0.8 * h;
    ptMax.x += 0.9 * w;
    ptMin.x += 0.1 * h;
    ptMax.y -= 0.2 * w;
    OdDbGeoMapPtr pGeoMap = OdDbGeoMap::createObject();
    pGeoMap->setDatabaseDefaults(pDb);
    bBTR->appendOdDbEntity(pGeoMap);
    ODA_VERIFY(eOk == pGeoMap->setOrientation(ptMin, OdGeVector3d(ptMax.x - ptMin.x, 0., 0.), OdGeVector3d(0., ptMax.y - ptMin.y, 0.)));
    ODA_VERIFY(eOk == pGeoMap->setMapType(kRoad));
    ODA_VERIFY(eOk == pGeoMap->updateMapImage());
    pGeoMap->setDisplayOpt(OdDbRasterImage::kShow, true);
    arObjId.append(pGeoMap->id());
  }
  
  /**********************************************************************/
  /* Add the GeoMap to Sortents table                                   */
  /**********************************************************************/
  OdDbSortentsTablePtr pSortentsTable = bBTR->getSortentsTable(true);
  pSortentsTable->moveToBottom(arObjId);

  /**********************************************************************/
  /* Add the GeoPositionMarker                                          */
  /**********************************************************************/
  OdDbGeoPositionMarkerPtr pGeoPosMarker = OdDbGeoPositionMarker::createObject();
  pGeoPosMarker->setPosition(OdGePoint3d(ptUpperLeft.x + w/2., ptUpperLeft.y - h/2., 0.));
  pGeoPosMarker->setRadius(h/10.);
  bBTR->appendOdDbEntity(pGeoPosMarker);
}

void DbFiller::addNURBSurface(const OdDbDatabase* pDb, const int boxRow, const int boxCol, const OdDbObjectId& layerId, const OdDbObjectId& styleId)
{
  /**********************************************************************/
  /* Open the Block Table Record                                        */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = pDb->getModelSpaceId().safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Get the Upper-left corner of the box and its size                  */
  /**********************************************************************/
  OdGePoint3d ptUpperLeft = m_EntityBoxes.getBox(boxRow, boxCol);
  double h = m_EntityBoxes.getHeight();
  double w = m_EntityBoxes.getWidth(boxRow, boxCol);

  /**********************************************************************/
  /* Add the label                                                      */
  /**********************************************************************/
  addTextEnt(bBTR,
    ptUpperLeft + m_textOffset, ptUpperLeft + m_textOffset,
    OD_T("NURBS surface"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);

  addTextEnt(bBTR,
    ptUpperLeft + m_textOffset + m_textLine, ptUpperLeft + m_textOffset + m_textLine,
    OD_T("by brep builder"), m_textSize, OdDb::kTextLeft, OdDb::kTextTop, layerId, styleId);
  /********************************************************************/
  /* Create NURBS surface, trimming-loop and holes                    */
  /********************************************************************/
  OdGeNurbSurface nurbSurf;
  OdArray<OdArray< OdGeNurbCurve3d*> > arrLoops;
  OdArray<OdArray< OdGeNurbCurve2d*> > arrLoopsPar;
  fillNurbData(nurbSurf, arrLoops, arrLoopsPar);
  OdDbNurbSurfacePtr pSurface = OdDbNurbSurface::createObject();
  /********************************************************************/
  /* BrepBuilder initialization                                       */
  /********************************************************************/
  OdDbHostAppServices *pService = pDb->appServices();
  BrepType bbType = kOpenShell;
  OdBrepBuilder brepBilder;
  pService->brepBuilder(brepBilder, bbType);
  /********************************************************************/
  /* Create NURBS Surface by BrepBuilder                              */
  /********************************************************************/
  try
  { 
    BRepBuilderGeometryId complexId = brepBilder.addComplex();
    BRepBuilderGeometryId shellId = brepBilder.addShell(complexId);
    BRepBuilderGeometryId faceId = brepBilder.addFace(&nurbSurf, OdBrepBuilder::kForward, shellId);
    ODA_ASSERT(arrLoops.size() == arrLoopsPar.size());
    for (unsigned int i = 0; i < arrLoops.size(); i++)
    {
      OdArray< OdGeNurbCurve3d*> currentLoop = arrLoops[i];
      OdArray< OdGeNurbCurve2d*> currentParLoop = arrLoopsPar[i];
      ODA_ASSERT(currentLoop.size() == currentParLoop.size());
      BRepBuilderGeometryId LoopId = brepBilder.addLoop(faceId);
      for (unsigned int j = 0; j < currentLoop.length(); j++)
      {
        BRepBuilderGeometryId edgeId = brepBilder.addEdge(currentLoop[j]);
        brepBilder.addCoedge(LoopId, edgeId, OdBrepBuilder::kForward, currentParLoop[j]);
      }
      brepBilder.finishLoop(LoopId);
    }
    brepBilder.finishFace(faceId);
    brepBilder.finishShell(shellId);
    brepBilder.finishComplex(complexId);
    OdRxObjectPtr pModelGeometry;
    pModelGeometry = brepBilder.finish();
    pSurface->setBody(pModelGeometry);
  }
  catch (const OdError& e)
  {
    odPrintConsoleString(L"\n\nException occurred: %ls\n", e.description().c_str());
  }

  /********************************************************************/
  /* Free created trimming-loop curves                                */
  /********************************************************************/
  for (unsigned int i = 0; i < arrLoops.length(); i++)
  {
    for (unsigned int j = 0; j < arrLoops[i].length(); j++)
    {
      delete arrLoops[i][j];
      delete arrLoopsPar[i][j];
    }
  }
  /**********************************************************************/
  /* Add NURB surface to database                                       */
  /**********************************************************************/
  OdGeExtents3d ext;
  pSurface->getGeomExtents(ext);
  OdGeMatrix3d xfm;
  const double scaleX = w*0.7 / (ext.maxPoint().x - ext.minPoint().x);
  const double scaleY = h*0.7 / (ext.maxPoint().y - ext.minPoint().y);
  xfm.setToScaling(odmin(scaleX, scaleY), ext.center());
  pSurface->transformBy(xfm);
  xfm.setToTranslation(m_EntityBoxes.getBoxCenter(boxRow, boxCol).asVector() - ext.center().asVector());
  pSurface->transformBy(xfm);

  pSurface->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSurface);
  /**********************************************************************/
  /* Add the NURB surface                                               */
  /**********************************************************************/
  OdDbObjectIdArray arObjId;
  arObjId.append(pSurface->id());

  /**********************************************************************/
  /* Add NURB surface to Sortents table                                 */
  /**********************************************************************/
  OdDbSortentsTablePtr pSortentsTable = bBTR->getSortentsTable(true);
  pSortentsTable->moveToBottom(arObjId);
}
