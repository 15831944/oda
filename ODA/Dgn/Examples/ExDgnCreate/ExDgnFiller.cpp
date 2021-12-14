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

#include "RxDictionary.h"

#include "DgEllipse.h"
#include "DgLineString.h"
#include "DgModel.h"
#include "DgArc.h"
#include "DgCone.h"
#include "DgLine.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgShape.h"
#include "DgDimOptions.h"
#include "DgDimension.h"
#include "DgCurve.h"
#include "DgConic.h"
#include "DgPointString.h"
#include "DgComplexString.h"
#include "DgLevelTableRecord.h"
#include "DgTable.h"
#include "DgFontTableRecord.h"
#include "DgDatabase.h"
#include "DgMultiline.h"
#include "DgComplexShape.h"
#include "DgBSplineCurve.h"
#include "DgBSplineSurface.h"
#include "Dg3DObject.h"
#include "DgRaster.h"
#include "DgRasterAttach.h"
#include "DgTagElement.h"
#include "DgMesh.h"
#include "DgReferenceAttach.h"
#include "DgLevelMask.h"
#include "DgSharedCellDefinition.h"
#include "DgSharedCellReference.h"
#include "DgBRepEntityPE.h"
#include "DgCellHeader.h"
#include "DgTextStyleTableRecord.h"
#include "DgAttributeLinkage.h"
#include "DgHostAppServices.h"
#include "DgDimStyleTableRecord.h"
#include "DgLight.h"
#include "DgLevelFilterTableRecord.h"
#include "DgLineStyleTableRecord.h"
#include "DgLineStyleDefTableRecord.h"
#include "DgMaterialTableRecord.h"
#include "DgOle2Frame.h"
#include "DgRasterFrame.h"
#include "DgColorBook.h"
#include "DgDetailingSymbol.h"
#include "DgEnvironmentMap.h"
#include "DgItemTypeLibrary.h"
#include "DgTableElement.h"

#include "DgSummaryInfo.h"
#include "TGVersion.h"

#include "MemoryStream.h"

#include "ExDgnFiller.h"
#include "EntityBoxes.h"
#include "Ge/GeScale3d.h"
#include "OleSampleData.h"
#include "RasterSampleData.h"

#include "DgNamedGroup.h"
#include "DgElementTemplate.h"
#include "DgXAttribute.h"
#include "DgACS.h"
#include "DgSectionClip.h"
#include "DgDisplayStyle.h"

#define STL_USING_IOSTREAM
#include "OdaSTL.h"
#define  STD(a)  std:: a
#ifndef _tcout
#ifdef TD_UNICODE
#define _tcout wcout
#else
#define _tcout cout
#endif
#endif

/************************************************************************/
/* Define the entity box widths                                         */
/************************************************************************/
const int EntityBoxes::BoxSizes[VER_BOXES][HOR_BOXES] = 
{
  {1,1,1,1,2,1,1,1,1,1,0},
  {1,3,2,2,1,2,0,0,0,0},
  {2,3,3,1,2,0,0,0,0,0,0},
  {1,1,1,2,1,2,2,1,0,0,0},
  {2,2,2,1,1,2,1,0,0,0,0},
  {3,2,2,1,1,1,1,0,0,0,0},
  {1,2,1,1,1,1,1,1,1,1,0},
  {2,2,2,2,2,1,0,0,0,0,0},
  {2,2,2,2,2,1,0,0,0,0,0}
};

OdExDgnFiller::OdExDgnFiller()
{
  m_bCreateSS3Materials = false;
}

void OdExDgnFiller::enableCreationOfSS3Materials( bool bEnable )
{
  m_bCreateSS3Materials = bEnable;
}

void OdExDgnFiller::createEntityBoxes3d()
{
  OdGePoint3d currentPoint;

  /**********************************************************************/
  /* Create a 2D polyline for each box                                  */
  /**********************************************************************/
  // TODO: set color & level
  for ( int j=0; j < VER_BOXES; j++)
  {
    for ( int i=0; i < HOR_BOXES; i++)
    {
      if ( ! m_EntityBoxes.isBox (j,i) )
        break;

      double wCurBox = m_EntityBoxes.getWidth(j,i);
      currentPoint = m_EntityBoxes.getBox(j,i);

      OdDgLineString3dPtr pLineString = OdDgLineString3d::createObject();

      OdGePoint3d pos(currentPoint.x, currentPoint.y, 0);
      pLineString->addVertex(pos);

      pos.x += wCurBox;
      pLineString->addVertex(pos);

      pos.y -= m_EntityBoxes.getHeight();
      pLineString->addVertex(pos);

      pos.x -= wCurBox;
      pLineString->addVertex(pos);

      pos.x = currentPoint.x;
      pos.y = currentPoint.y;
      pLineString->addVertex(pos);

      pLineString->setColorIndex(OdCmEntityColor::kACIBlue);

      m_pModel3d->addElement( pLineString );
    }
  }
}

void OdExDgnFiller::createEntityBoxes2d( OdDgModelPtr pModel )
{
  OdGePoint3d currentPoint;

  /**********************************************************************/
  /* Create a 2D polyline for each box                                  */
  /**********************************************************************/
  // TODO: set color & level
  for ( int j=0; j < VER_BOXES; j++)
  {
    for ( int i=0; i < HOR_BOXES; i++)
    {
      if ( ! m_EntityBoxes.isBox (j,i) )
        break;

      double wCurBox = m_EntityBoxes.getWidth(j,i);
      currentPoint = m_EntityBoxes.getBox(j,i);

      OdDgLineString2dPtr pLineString = OdDgLineString2d::createObject();

      OdGePoint2d pos(currentPoint.x, currentPoint.y);
      pLineString->addVertex(pos);

      pos.x += wCurBox;
      pLineString->addVertex(pos);

      pos.y -= m_EntityBoxes.getHeight();
      pLineString->addVertex(pos);

      pos.x -= wCurBox;
      pLineString->addVertex(pos);

      pos.x = currentPoint.x;
      pos.y = currentPoint.y;
      pLineString->addVertex(pos);

      pLineString->setColorIndex(OdCmEntityColor::kACIBlue);

      pModel->addElement( pLineString );
    }
  }
}
void OdExDgnFiller::fillDatabase(OdDgDatabase* pDb)
{
  pDb->disableUndoRecording(true);

  /********************************************************************/
  /* Fill the summary information                                     */
  /********************************************************************/
  fillSummaryInfo(pDb);

  /********************************************************************/
  /* Fill the model                                                   */
  /********************************************************************/
  m_pModel3d = pDb->getActiveModelId().safeOpenObject(OdDg::kForWrite);

  //initialize measure units
  {
    OdDgModel::UnitDescription description;

    OdDgModel::fillUnitDescriptor( OdDgModel::kMeters, description );
    m_pModel3d->setMasterUnit( description );
    OdDgModel::fillUnitDescriptor( OdDgModel::kMillimeters, description );
    m_pModel3d->setSubUnit( description );

    m_pModel3d->setWorkingUnit( OdDgModel::kWuMasterUnit );
  }

  // create additional 2d Model
  OdDgModelTablePtr pModelTable = pDb->getModelTable();
  m_pModel2d = OdDgModel::createObject();
  m_pModel2d->setModelIs3dFlag(false);
  m_pModel2d->setName("Model for 2d elements");
  m_pModel2d->setWorkingUnit( OdDgModel::kWuMasterUnit );
  pModelTable->add( m_pModel2d );

  // create additional 2d Sheet Model
  m_pSheetModel = OdDgSheetModel::createObject();

  m_pSheetModel->setModelIs3dFlag(false);                        // Model 2d or 3d.
  m_pSheetModel->setName("Sheet Model");                         // Model name.
  m_pSheetModel->setDescription("Sheet Model example");          // Model description
  m_pSheetModel->setType( OdDgModel::kSheetModel);               // Model type ( sheet or design ).
  m_pSheetModel->setBackground( ODRGB(255,255,255) );            // Model background color.
  m_pSheetModel->setUseBackgroundColorFlag(true);                // Use background color value or not.
  m_pSheetModel->setWorkingUnit( OdDgModel::kWuMasterUnit );
  pModelTable->add( m_pSheetModel );

  // Sample page properties for sheet model.

  double    dPaperWidth   = 16.5;
  double    dPaperHeight  = 13.5;
  double    dPaperXOfsset = -1.0;
  double    dPaperYOfsset = -1.0;
  double    dMarginOffset = 0.25;
  OdUInt32  uPageNumber   = 17;
  double    dAnotationScale = 2.0;

  m_pSheetModel->setDrawBorderFlag(true);
  m_pSheetModel->setDrawMarginsFlag(true);
  m_pSheetModel->setSheetRotation(0);
  m_pSheetModel->setSheetOffset( OdGePoint2d(dPaperXOfsset,dPaperYOfsset) );
  m_pSheetModel->setSheetNumber(uPageNumber);
  m_pSheetModel->setAnnotationScale(dAnotationScale);
  m_pSheetModel->setPaper( OdDgSheetModel::kCustom );
  m_pSheetModel->setSheetHeight( dPaperHeight );
  m_pSheetModel->setSheetWidth( dPaperWidth );
  m_pSheetModel->setSheetLeftMargin( dMarginOffset );
  m_pSheetModel->setSheetRightMargin( dMarginOffset );
  m_pSheetModel->setSheetTopMargin( dMarginOffset );
  m_pSheetModel->setSheetBottomMargin( dMarginOffset );

  fillModels();

  m_pModel3d->fitToView();
  m_pModel2d->fitToView();
  m_pSheetModel->fitToView();

  pDb->setActiveLevelEntryId( 2 );
}

void OdExDgnFiller::fillSummaryInfo(OdDgDatabase* pDb)
{
  // Summary Information
  OdDgSummaryInformation* pSi = oddgGetSummaryInformation(pDb);
  pSi->setTitle(L"Title");
  pSi->setSubject(L"Subject");
  pSi->setAuthor(L"Author");
  pSi->setKeywords(L"Keywords");
  pSi->setTemplate(L"Template");
  pSi->setLastSavedBy(L"LastSavedBy");
  pSi->setRevisionNumber(L"1");
  pSi->setApplicationName(OdString(L"ExDgnCreate, ") + OdString(TG_BUILD_COMMENTS_W));
  //pSi->setComments(L"Comments"); // PIDSI_COMMENTS == 6, VT_LPWSTR == 31
  pSi->setProperty( 6, OdDgPropertyValue::createObject( 31, OdVariant(OdString(L"Comments")) ));

  // Document Summary Information
  OdDgDocumentSummaryInformation* pDsi = oddgGetDocumentSummaryInformation(pDb);
  pDsi->setManager(L"Manager");
  pDsi->setCompany(L"Open Design Alliance");
  //pDsi->setCategory(L"Category"); // PIDDSI_CATEGORY == 2, VT_LPWSTR == 31
  pDsi->setProperty( 2, OdDgPropertyValue::createObject( 31, OdVariant(OdString(L"Category")) ));

  pDsi->setCustomProperty("CustomProperty1", OdDgPropertyValue::createObject(OdDgPropertyValue::kCustomText, OdVariant(OdAnsiString("Value 1"))));
  pDsi->setCustomProperty("CustomProperty2", OdDgPropertyValue::createObject(OdDgPropertyValue::kCustomNumber, OdVariant(OdInt32(255))));
  // (64-bit value representing the number of 100-nanosecond intervals since January 1, 1601)
  pDsi->setCustomProperty("CustomProperty3", OdDgPropertyValue::createObject(OdDgPropertyValue::kCustomDate, OdVariant(OdUInt64(DDLL(129459024000000000)))));  // 30.03.2011
  pDsi->setCustomProperty("CustomProperty4", OdDgPropertyValue::createObject(OdDgPropertyValue::kCustomBool, OdVariant(true)));
  //OdRxDictionaryPtr pPropDic = pDsi->getCustomProperties();
  //pPropDic->putAt("CustomProperty5", OdDgPropertyValue::createObject(OdDgPropertyValue::kCustomText, OdVariant(OdAnsiString("Value 5"))));
  //pDsi->setCustomProperties(pPropDic);
}

void OdExDgnFiller::fillModels()
{
  /**********************************************************************/
  /* Create table record elements                                       */
  /**********************************************************************/

  addMaterialTableRecord();

  if( m_bCreateSS3Materials )
  {
    addMaterialV8TableRecord();
  }

  addLevelTableRecord();
  addLevelFilterTableRecord();
  addFontTableRecord();
  addDimStyleTableRecord();
  addLineStyleTableRecord();
  addMultilineStyleTableRecord();
  addRegAppTableRecord();
  addTextStyleTableRecord();
  addColorTableRecord();
  addElementTemplates();
  addColorBooks();
  addItemTypesLibrary();
  addVariblesList();
  addXmlCustomScheme();
  addPrintStyle();
  addDictionaryTableAndFence();

  /**********************************************************************/
  /* Create boxes for elements                                          */
  /**********************************************************************/
  createEntityBoxes3d();
  createEntityBoxes2d( m_pModel2d );
  createEntityBoxes2d( m_pSheetModel );

  /**********************************************************************/
  /* Add ACS element                                                    */
  /**********************************************************************/

  addACS();
  
  /**********************************************************************/
  /* Add EnvironmentMapTable element                                    */
  /**********************************************************************/

  addEnvironmentMapTable();

  /**********************************************************************/
  /* Add Display Style element                                          */
  /**********************************************************************/

  addDisplayStyleTableRecord();

  /**********************************************************************/
  /* Add ellipse                                                        */
  /**********************************************************************/
  addEllipse(0,4);

  /**********************************************************************/
  /* Add ellipse                                                        */
  /**********************************************************************/
  addArc(0,5);

  /**********************************************************************/
  /* Add cone                                                           */
  /**********************************************************************/
  addCone(6,3);

  /**********************************************************************/
  /* Add lines                                                          */
  /**********************************************************************/
  addLines(0,0);

  /**********************************************************************/
  /* Add lines with line style                                          */
  /**********************************************************************/
  addLineStyleLines( 2, 0 );

  /**********************************************************************/
  /* Add text                                                           */
  /**********************************************************************/
  addText( 2, 1 );

  /**********************************************************************/
  /* Add text with justification                                        */
  /**********************************************************************/
  addTextWithJustifucation( 3, 5 );

  /**********************************************************************/
  /* Add text node                                                      */
  /**********************************************************************/
  addTextNode( 1, 1 );

  /**********************************************************************/
  /* Add shape                                                          */
  /**********************************************************************/
  addShape( 4, 4 );

  /**********************************************************************/
  /* Add shape with hole                                                */
  /**********************************************************************/
  addShapeWithHole( 1, 2 );

  /**********************************************************************/
  /* Add curve                                                          */
  /**********************************************************************/
  addCurve( 6, 4 );

  /**********************************************************************/
  /* Add point string                                                   */
  /**********************************************************************/
  addPointString( 6, 6 );

  /**********************************************************************/
  /* Add point string                                                   */
  /**********************************************************************/
  addLineStringWithBreak( 0, 7 );

  /**********************************************************************/
  /* Add dimension                                                      */
  /**********************************************************************/
  addDimension( 0, 9 );

  /**********************************************************************/
  /* Add complex string                                                 */
  /**********************************************************************/
  addComplexString(0, 1);

  /**********************************************************************/
  /* Multiline                                                          */
  /**********************************************************************/
  addMultiline( 0, 6 );

  /**********************************************************************/
  /* Complex shape                                                      */
  /**********************************************************************/
  addComplexShape( 3, 4 );

  /**********************************************************************/
  /* B-spline curve                                                     */
  /**********************************************************************/
  addBSplineCurveWithKnots(3,0);
  addClosedBSplineCurveWithKnots(3,1);
  addBSplineCurve( 3, 3 );

  /**********************************************************************/
  /* B-spline surface                                                   */
  /**********************************************************************/
  addBSplineSurface( 4, 1 );

  /**********************************************************************/
  /* Surface                                                            */
  /**********************************************************************/
  addSurface( 2, 3 );

  /**********************************************************************/
  /* Solid                                                              */
  /**********************************************************************/
  addSolid( 1, 4 );
  addLights( 4, 1 );

  /**********************************************************************/
  /* Raster                                                             */
  /**********************************************************************/
  addRaster( 6, 7 ); 
  addByteRaster( 5, 4 );

  /**********************************************************************/
  /* Raster attach                                                      */
  /**********************************************************************/
  addRasterAttach( 6, 8 ); 
  addRasterAttachBmp( 5, 6 );
  addRasterAttachPdf( 8, 2 );

  /**********************************************************************/
  /* Tag element                                                        */
  /**********************************************************************/
  addTagElement( 6, 9 );

  /**********************************************************************/
  /* Mesh                                                               */
  /**********************************************************************/
  addMesh( 4, 2 );
  addColorMesh( 5, 1 );
  addNormAndTexMesh( 4, 5 );

  /**********************************************************************/
  /* Reference attach                                                   */
  /**********************************************************************/
  addAttach( 5, 5 );

  /**********************************************************************/
  /* Shared cell                                                        */
  /**********************************************************************/
  addSharedCells( 1, 5 );

  /**********************************************************************/
  /* Cell                                                               */
  /**********************************************************************/
  addCell( 2, 4 );

  /**********************************************************************/
  /* Smart Solid                                                        */
  /**********************************************************************/
  addSmartSolid( 2, 5 );

  /**********************************************************************/
  /* Patterns                                                           */
  /**********************************************************************/
  addPatterns( 5, 0 );

  /**********************************************************************/
  /* DB Linkages                                                        */
  /**********************************************************************/
  addDBLinkages( 4, 0 );

  /**********************************************************************/
  /* Something with true colors                                         */
  /**********************************************************************/
  addTrueColorShapes( 0, 2 );

  /**********************************************************************/
  /* Something with a fill color                                        */
  /**********************************************************************/
  addFilledShape( 0, 3 );

  /**********************************************************************/
  /* Ole Frame                                                          */
  /**********************************************************************/
  addOleFrame( 5, 2 );

  /**********************************************************************/
  /* Named Group                                                        */
  /**********************************************************************/
  addNamedGroup();

  /**********************************************************************/
  /* Element with template                                              */
  /**********************************************************************/

  addLineWithTemp( 5, 3 );

  /**********************************************************************/
  /* Detailing symbol: title text                                       */
  /**********************************************************************/

  addTitleText( 7, 0 );

  /**********************************************************************/
  /* Detailing symbol: section marker                                   */
  /**********************************************************************/

  addSectionMarker( 7, 1 );

  /**********************************************************************/
  /* Detailing symbol: arrow marker                                   */
  /**********************************************************************/

  addArrowMarker( 7, 2 );

  /**********************************************************************/
  /* Detailing symbol: detail marker                                   */
  /**********************************************************************/

  addDetailMarker( 7, 3 );

  /**********************************************************************/
  /* Annotation Cell                                                    */
  /**********************************************************************/

  addAnnotationCell( 7, 4 );

  /**********************************************************************/
  /* Elements with volume clipping on 0 view                            */
  /**********************************************************************/

  addTextWithVolumeClipping(6,1);

  /**********************************************************************/
  /* Elements with section clipping on 1 view                           */
  /**********************************************************************/

  addSectionClip(3,6);

  /**********************************************************************/
  /* Add _ELEMENT_                                                      */
  /**********************************************************************/

  addShapeWithTransparency( 4, 3 );

  /**********************************************************************/
  /* Add terrain element                                                */
  /**********************************************************************/

  addTerrain( 1, 3 );

  /**********************************************************************/
  /* Add item types to elements                                                */
  /**********************************************************************/

  addItemTypes( 1, 0 );  
  
  /**********************************************************************/
  /* Add table element                                                */
  /**********************************************************************/

  addTableElement( 8, 0 );

	/**********************************************************************/
	/* Add associated region                                               */
	/**********************************************************************/

	addAssocRegion(8, 1);
}

void OdExDgnFiller::addLevelTableRecord()
{
  OdDgLevelTableRecordPtr pLevel1 = OdDgLevelTableRecord::createObject();
  pLevel1->setName("ExDgnCreate level 1");
  //pLevel1->setElementColorIndex(6);
  // ##### example code to replace commented line above
  // ##### BEGIN ##### 
  OdDgCmColor color;
  OdUInt32 colorIndex;
  colorIndex = OdDgColorTable::getColorIndexByRGB( m_pModel3d->database(), ODRGB(91, 91, 91) );
  color.setColorIndex(colorIndex);
  colorIndex = OdDgColorTable::getColorIndexByRGB( m_pModel3d->database(), ODRGB(148, 152, 77) );
  color.setColorIndex(colorIndex);
  pLevel1->setElementColor(color);
  // ##### END ##### 
  pLevel1->setElementLineWeight(10);
  pLevel1->setIsDisplayedFlag(false);
  pLevel1->setIsPlotFlag(false);       // won't be displayed by Print Preview

  OdDgLevelTableRecordPtr pLevel2 = OdDgLevelTableRecord::createObject();
  pLevel2->setName("level 2");
  pLevel2->setElementColorIndex(4);
  pLevel2->setElementLineWeight(0);
  pLevel2->setIsDisplayedFlag(true);
  pLevel2->setIsPlotFlag(false); 

  OdDgLevelTableRecordPtr pLevel3 = OdDgLevelTableRecord::createObject();
  pLevel3->setName("Custom name");
  pLevel3->setElementColorIndex(2);
  pLevel3->setElementLineWeight(0);
  pLevel3->setIsDisplayedFlag(true);
  pLevel3->setIsPlotFlag(false); 

  OdDgLevelTableRecordPtr pLevel4 = OdDgLevelTableRecord::createObject();
  pLevel4->setName("level - true color 1");
  colorIndex = OdDgColorTable::getColorIndexByRGB( m_pModel3d->database(), ODRGB(91, 91, 91) );
  color.setColorIndex(colorIndex);
  pLevel4->setElementColor(color);
  pLevel4->setElementLineWeight(0);
  pLevel4->setIsDisplayedFlag(true);
  pLevel4->setIsPlotFlag(false); 

  OdDgLevelTableRecordPtr pLevel5 = OdDgLevelTableRecord::createObject();
  pLevel5->setName("level - true color 2");
  colorIndex = OdDgColorTable::getColorIndexByRGB( m_pModel3d->database(), ODRGB(92, 92, 92) );
  color.setColorIndex(colorIndex);
  pLevel5->setElementColor(color);
  pLevel5->setElementLineWeight(0);
  pLevel5->setIsDisplayedFlag(true);
  pLevel5->setIsPlotFlag(false); 

  OdDgLevelTablePtr pLevelTable = m_pModel3d->database()->getLevelTable(OdDg::kForWrite);
  pLevelTable->add(pLevel1);
  pLevelTable->add(pLevel2);
  pLevelTable->add(pLevel3);
  pLevelTable->add(pLevel4);
  pLevelTable->add(pLevel5);

  OdDgMaterialTablePtr pMatTable = m_pModel3d->database()->getMaterialTable(OdDg::kForRead);

  OdDgElementId idMat1 = pMatTable->getAt(L"Simple Mat 1");
  OdDgElementId idMat2 = pMatTable->getAt(L"Simple Mat 2");
  OdDgElementId idMat3 = pMatTable->getAt(L"Simple Mat 3");

  OdDgAssignedMaterial assocMat;

  pLevel1->setOverrideMaterial( idMat1 );
  pLevel1->setUseOverrideMaterialFlag(true);

  pLevel2->setByLevelMaterial( idMat2 );
  pLevel2->setByLevelMaterialPalette(L"automobile.pal");

  assocMat.m_strMaterialName = L"Simple Mat 1";
  assocMat.m_uColorIndex = 8;
  pLevel2->addAssignedMaterial( assocMat);
  assocMat.m_uColorIndex = 125;
  pLevel2->addAssignedMaterial( assocMat);

  pLevel3->setOverrideMaterial( idMat3 );
  pLevel3->setByLevelMaterial( idMat1 );
  pLevel3->setUseOverrideMaterialFlag(false);

  assocMat.m_strMaterialName = L"Simple Mat 2";
  assocMat.m_uColorIndex = 10;
  pLevel3->addAssignedMaterial( assocMat);
  assocMat.m_uColorIndex = 40;
  pLevel3->addAssignedMaterial( assocMat);
  assocMat.m_strMaterialName = L"Simple Mat 3";
  assocMat.m_uColorIndex = 2;
  pLevel3->addAssignedMaterial( assocMat);
  assocMat.m_strMaterialName = L"Simple Mat 1";
  assocMat.m_uColorIndex = 8;
  pLevel3->addAssignedMaterial( assocMat );
  assocMat.m_uColorIndex = 16;
  pLevel3->addAssignedMaterial( assocMat );
  assocMat.m_uColorIndex = 32;
  pLevel3->addAssignedMaterial( assocMat );
}

void OdExDgnFiller::addLevelFilterTableRecord()
{
  OdDgLevelNameAndGroupFilterPtr pFilter0 = OdDgLevelNameAndGroupFilter::createObject();
  pFilter0->setName(OD_T("All Levels"));

  OdDgLevelNameAndGroupFilterPtr pFilter1 = OdDgLevelNameAndGroupFilter::createObject();

  pFilter1->setName(OD_T("Layer Name Filter"));
  pFilter1->setFilterType( OdDgLevelFilterTableRecord::kUser );

  OdDgLevelNameAndGroupFilterPtr pFilter2 = OdDgLevelNameAndGroupFilter::createObject();

  pFilter2->setName(OD_T("Layer Group Filter"));
  pFilter2->setFilterType( OdDgLevelFilterTableRecord::kUser );

  OdDgLevelFilterTablePtr pFilterTable = m_pModel3d->database()->getLevelFilterTable(OdDg::kForWrite);
  pFilterTable->add( pFilter0 );
  pFilterTable->add( pFilter1 );
  pFilterTable->add( pFilter2 );

  pFilter1->setNameFilterExpression(OD_T("level"));

  OdDgLevelTablePtr pLevelTable = m_pModel3d->database()->getLevelTable(OdDg::kForWrite);

  OdDgElementIteratorPtr pLevelIter = pLevelTable->createIterator();

  int iNumber = 0;

  for(; !pLevelIter->done(); pLevelIter->step() )
  {
    OdDgLevelTableRecordPtr pLevel = pLevelIter->item().openObject(OdDg::kForRead);

    if( iNumber > 1)
    {
      pFilter2->addLevelToGroup( pLevel );
    }
    iNumber++;
  }
}

void OdExDgnFiller::addFontTableRecord()
{
  OdDgFontTableRecordPtr pFont = OdDgFontTableRecord::createObject();
  pFont->setName("GothicE");
  pFont->setType(kFontTypeTrueType);

  OdDgFontTablePtr pFontTable = m_pModel3d->database()->getFontTable(OdDg::kForWrite);
  pFontTable->add(pFont);

  pFont = OdDgFontTableRecord::createObject();
  pFont->setName("Verdana");
  pFont->setType(kFontTypeTrueType);

  pFontTable->add(pFont);

  pFont = OdDgFontTableRecord::createObject();
  pFont->setName("Italic");
  pFont->setType(kFontTypeTrueType);

  pFontTable->add(pFont);

  pFont = OdDgFontTableRecord::createObject();
  pFont->setName("Arial");
  pFont->setType(kFontTypeTrueType);

  pFontTable->add(pFont);

  pFont = OdDgFontTableRecord::createObject();
  pFont->setName("ROMANS");
  pFont->setType(kFontTypeShx);

  pFontTable->add(pFont);

  // there is no necessity to add RSC (MicroStation Resource file) font
  try 
  {
    pFont = OdDgFontTableRecord::createObject();
    pFont->setName("ENGINEERING");
    pFont->setType(kFontTypeRsc);

    pFontTable->add(pFont);
  }
  catch (OdError err)
  {
    ODA_ASSERT(err.code() == eAlreadyInDb);
  }
}

void OdExDgnFiller::addDimStyleTableRecord()
{
  OdDgDimStyleTablePtr pTbl = m_pModel3d->database()->getDimStyleTable(OdDg::kForWrite);

  OdDgDimStyleTableRecordPtr pRecDefault = pTbl->getActualDimensionSettings(OdDg::kForWrite);
  pRecDefault->setName("");
  pRecDefault->setDescription("");
  pRecDefault->setTextHeightOverrideFlag(true);
  pRecDefault->setTextHeight(2);
  pRecDefault->setTextWidthOverrideFlag(true);
  pRecDefault->setTextWidth(2);

  OdDgDimStyleTableRecordPtr pRec = OdDgDimStyleTableRecord::createObject();
  pRec->setName("DimStyle1");
  pRec->setDescription("Dimension Style 1");
  pRec->setTerminatorColorOverrideFlag(true);
  pRec->setTerminatorColor(4);
  pRec->setTerminatorLineStyleOverrideFlag(true);
  pRec->setTerminatorLineStyleId( 2 );

  pTbl->add( pRec );
  pTbl->setActiveDimensionStyle(pRec, true);
}

void OdExDgnFiller::addLineStyleTableRecord()
{
  OdDgLineStyleDefTablePtr pLineStyleDefTable = m_pModel3d->database()->getLineStyleDefTable( OdDg::kForWrite );

  double dScaleToUORs = m_pModel3d->convertWorkingUnitsToUORs(1.0);

  // Create shape for line style

  OdDgCellHeader2dPtr pSmileyCell = OdDgCellHeader2d::createObject();

  pLineStyleDefTable->addSymbol( pSmileyCell );

  OdDgEllipse2dPtr pHeadEllipse = OdDgEllipse2d::createObject();
  pHeadEllipse->setOrigin( OdGePoint2d(0,0) );
  pHeadEllipse->setPrimaryAxis( 1.5 );
  pHeadEllipse->setSecondaryAxis( 1.5 );
  pHeadEllipse->setLineStyleScale(1.0 / dScaleToUORs);

  OdDgEllipse2dPtr pEyeLeftEllipse = OdDgEllipse2d::createObject();
  pEyeLeftEllipse->setOrigin( OdGePoint2d(-0.75,0.5) );
  pEyeLeftEllipse->setPrimaryAxis( 0.25 );
  pEyeLeftEllipse->setSecondaryAxis( 0.25 );
  pEyeLeftEllipse->setLineStyleScale(1.0 / dScaleToUORs);

  OdDgEllipse2dPtr pEyeRightEllipse = OdDgEllipse2d::createObject();
  pEyeRightEllipse->setOrigin( OdGePoint2d(0.75,0.5) );
  pEyeRightEllipse->setPrimaryAxis( 0.25 );
  pEyeRightEllipse->setSecondaryAxis( 0.25 );
  pEyeRightEllipse->setLineStyleScale(1.0 / dScaleToUORs);

  OdDgLineString2dPtr pNoseLine = OdDgLineString2d::createObject();

  pNoseLine->addVertex( OdGePoint2d(-0.25,0.25) );
  pNoseLine->addVertex( OdGePoint2d(-0.25,-0.5) );
  pNoseLine->addVertex( OdGePoint2d(0.25,-0.5) );
  pNoseLine->setLineStyleScale(1.0 / dScaleToUORs);

  OdDgArc2dPtr pMouthArc = OdDgArc2d::createObject();

  pMouthArc->setOrigin( OdGePoint2d(0,0) );
  pMouthArc->setPrimaryAxis( 1.0 );
  pMouthArc->setSecondaryAxis( 1.0 );
  pMouthArc->setStartAngle( 5.0 * OdaPI / 4.0 );
  pMouthArc->setSweepAngle( OdaPI2 );
  pMouthArc->setLineStyleScale(1.0 / dScaleToUORs);

  pSmileyCell->add( pHeadEllipse );
  pSmileyCell->add( pEyeLeftEllipse );
  pSmileyCell->add( pEyeRightEllipse );
  pSmileyCell->add( pNoseLine );
  pSmileyCell->add( pMouthArc );
  pSmileyCell->setName(OD_T("Linestyle Definition Cell"));

  pSmileyCell->setOrigin( OdGePoint2d(0,0) );
  pSmileyCell->setTransformation( OdGeMatrix2d::scaling(dScaleToUORs) );

  // Create point symbol resource

  OdDgLineStyleDefTableRecordPtr pSmileySymbolDef = OdDgLineStyleDefTableRecord::createObject( OdDg::kLsTypePointSymbol );

  OdDgPointSymbolResourcePtr pRes = pSmileySymbolDef->getResource();
  pRes->setDependedCellHeaderHandle( (OdUInt64)(pSmileyCell->elementId().getHandle()) );
  pRes->setDescription(OD_T("Smiley cell"));

  pLineStyleDefTable->add( pSmileySymbolDef );

  // Create stroke resource

  OdDgLineStyleDefTableRecordPtr pSmileyStrokeDef = OdDgLineStyleDefTableRecord::createObject( OdDg::kLsTypeLineCode );

  OdDgLineCodeResourcePtr pStrokeRes = pSmileyStrokeDef->getResource();

  OdDgLineCodeResourceStrokeData dashData;

  dashData.setDashFlag(true);
  dashData.setLength(2.0*dScaleToUORs);
  dashData.setWidthMode( OdDgLineCodeResourceStrokeData::kLsNoWidth );

  pStrokeRes->addStroke( dashData );

  dashData.setDashFlag(false);
  dashData.setLength(4.0*dScaleToUORs);

  pStrokeRes->addStroke( dashData );

  dashData.setDashFlag(true);
  dashData.setLength(2.0*dScaleToUORs);

  pStrokeRes->addStroke( dashData );

  pStrokeRes->setDescription(OD_T("Smiley stroke"));

  pLineStyleDefTable->add( pSmileyStrokeDef );

  // Create line point resource

  OdDgLineStyleDefTableRecordPtr pSmileyLinePtDef = OdDgLineStyleDefTableRecord::createObject( OdDg::kLsTypeLinePoint );

  OdDgLinePointResourcePtr pLinePtRes = pSmileyLinePtDef->getResource();

  pLinePtRes->setBasePatternType( OdDgLineStyleResource::kLsLineCodeRes );
  pLinePtRes->setBasePatternHandleId( (OdUInt64)(pSmileyStrokeDef->elementId().getHandle()) );

  OdDgLinePointResourceSymInfo symInfo;

  symInfo.setPointSymbolHandleId( (OdUInt64)(pSmileySymbolDef->elementId().getHandle()) );
  symInfo.setSymbolPosOnStroke( OdDgLinePointResourceSymInfo::kLsAtCenterOfStroke );
  symInfo.setSymbolType( OdDgLineStyleResource::kLsPointSymbolRes );
  symInfo.setSymbolStrokeNo(1);
  symInfo.setDoNotScaleElementFlag(true);

  pLinePtRes->addSymbol(symInfo);

  pLinePtRes->setDescription(OD_T("Smiley symbol"));

  pLineStyleDefTable->add( pSmileyLinePtDef );

  // Create compound resource

  OdDgLineStyleDefTableRecordPtr pSmileyCompoundDef = OdDgLineStyleDefTableRecord::createObject( OdDg::kLsTypeCompound );

  OdDgCompoundLineStyleResourcePtr pCompoundRes = pSmileyCompoundDef->getResource();

  pCompoundRes->setDescription(OD_T("Smiley compound"));

  OdDgCompoundLineStyleComponentInfo infoComp;

  infoComp.setComponentType( OdDgLineStyleResource::kLsLineCodeRes );
  infoComp.setComponentHandleId( (OdUInt64)(pSmileyStrokeDef->elementId().getHandle()) );
  infoComp.setComponentOffset( 0.0 );

  pCompoundRes->addComponent( infoComp );

  infoComp.setComponentType( OdDgLineStyleResource::kLsLinePointRes );
  infoComp.setComponentHandleId( (OdUInt64)(pSmileyLinePtDef->elementId().getHandle()) );

  pCompoundRes->addComponent( infoComp );

  pSmileyCompoundDef->setName(OD_T("{ Smiley }"));

  pLineStyleDefTable->add( pSmileyCompoundDef );

  // Create line style

  OdDgLineStyleTableRecordPtr pLineStyle1 = OdDgLineStyleTableRecord::createObject();
  pLineStyle1->setName(OD_T("{ Smiley }"));
  pLineStyle1->setType(OdDg::kLsTypeCompound);
  pLineStyle1->setRefersToElementFlag(true);
  pLineStyle1->setRefersToId( pSmileyCompoundDef->elementId() );
  pLineStyle1->setUnitsType( OdDg::kLsUORS );

  OdDgLineStyleTablePtr pLSTbl = m_pModel3d->database()->getLineStyleTable(OdDg::kForWrite);

  if( !pLSTbl.isNull() )
  {
    pLSTbl->add( pLineStyle1 );
  }

  // Add rsc line style

  OdDgLineStyleTableRecordPtr pRscLineStyle = OdDgLineStyleTableRecord::createObject();
  pRscLineStyle->setName(OD_T("{ Rail Road }"));

  if( !pLSTbl.isNull() )
  {
    pLSTbl->add( pRscLineStyle );
  }
}

void OdExDgnFiller::addMultilineStyleTableRecord()
{
  OdDgMultilineStyleTableRecordPtr pMLStyle = OdDgMultilineStyleTableRecord::createObject();
  pMLStyle->setName(OD_T("Multiline Style 1"));
  pMLStyle->setUseFillColorFlag(true);
  pMLStyle->setFillColorIndex(135);

  double dScaleWorkingUnitsToUORs = m_pSheetModel->convertWorkingUnitsToUORs(1);
  // Offset of lines into multiline style
  double dOffset1 = 0.1;
  double dOffset2 = -0.1;
  double dOffset3 = 0.025;
  double dOffset4 = -0.025;
  double dOffset5 = 0.15;
  double dOffset6 = -0.15;
  
  OdDgMultilineProfile profile;
  OdDgMultilineSymbology mlSymb;
  mlSymb.setColorIndex(3);
  mlSymb.setLineStyleEntryId(3);
  mlSymb.setUseColorFlag(true);
  mlSymb.setUseStyleFlag(true);
  profile.setDistance(dOffset1*dScaleWorkingUnitsToUORs);
  profile.setSymbology( mlSymb );
  pMLStyle->addProfile( profile );
  profile.setDistance(dOffset2*dScaleWorkingUnitsToUORs);
  pMLStyle->addProfile( profile );
  profile.setDistance(dOffset3*dScaleWorkingUnitsToUORs);
  mlSymb.setColorIndex(1);          // MKU 23/12/09 - ('warning' issue)
  mlSymb.setLineStyleEntryId(4);
  profile.setSymbology( mlSymb );
  pMLStyle->addProfile( profile );
  profile.setDistance(dOffset4*dScaleWorkingUnitsToUORs);
  pMLStyle->addProfile( profile );
  profile.setDistance(dOffset5*dScaleWorkingUnitsToUORs);
  mlSymb.setColorIndex(5);
  mlSymb.setUseStyleFlag(false);
  profile.setSymbology( mlSymb );
  pMLStyle->addProfile( profile );
  profile.setDistance(dOffset6*dScaleWorkingUnitsToUORs);
  pMLStyle->addProfile( profile );
  OdDgMultilineSymbology mlStartCap;
  mlStartCap.setUseColorFlag(true);
  mlStartCap.setColorIndex(3);
  mlStartCap.setCapOutArcFlag(true);
  mlStartCap.setCapLineFlag(true);
  pMLStyle->setOriginCap( mlStartCap );
  OdDgMultilineSymbology mlEndCap = mlStartCap;
  mlEndCap.setColorIndex(2);
  mlEndCap.setCapInArcFlag(true);
  mlEndCap.setCapLineFlag(true);
  pMLStyle->setEndCap( mlEndCap );
  pMLStyle->setOriginCapAngle(90.0);
  pMLStyle->setEndCapAngle(90.0);
  OdDgMultilineSymbology mlMiddleCap;
  mlMiddleCap.setUseColorFlag(true);
  mlMiddleCap.setUseStyleFlag(true);
  mlMiddleCap.setColorIndex(8);
  mlMiddleCap.setLineStyleEntryId(2);
  mlMiddleCap.setCapLineFlag(true);
  pMLStyle->setMiddleCap(mlMiddleCap);
  
  OdDgMultilineStyleTablePtr pTbl = m_pModel3d->database()->getMultilineStyleTable(OdDg::kForWrite);
  pTbl->add( pMLStyle );
}

void OdExDgnFiller::addRegAppTableRecord()
{
#pragma MARKMESSAGE("TODO: Examples. Implement addRegAppTableRecord().");
}

void OdExDgnFiller::addTextStyleTableRecord()
{
  // Set Default Text Style
  OdDgTextStyleTablePtr pTbl = m_pModel3d->database()->getTextStyleTable(OdDg::kForWrite);
  OdDgTextStyleTableRecordPtr pRec = pTbl->getDefaultData();
  pRec->setFontEntryId(41);   // set 'Architectural' font
  pRec->setTextWidth(m_pModel3d->convertWorkingUnitsToUORs(0.27));
  pRec->setTextHeight(m_pModel3d->convertWorkingUnitsToUORs(0.27));
  pTbl->setDefaultData(pRec);

  OdDgTextStyleTablePtr pTextStyleTable = m_pModel3d->database()->getTextStyleTable();
  OdDgFontTablePtr pFontTable           = m_pModel3d->database()->getFontTable();

  // Indemnify the code against some font absence
  OdUInt32 nAlternateFontNumber = pFontTable->getFont(m_pModel3d->database()->appServices()->getAlternateFontName())->getNumber();

  OdDgTextStyleTableRecordPtr pTextStyle = OdDgTextStyleTableRecord::createObject();
  pTextStyle->setName("TextStyle1");
  pTextStyle->setTextHeight( m_pModel3d->convertWorkingUnitsToUORs(0.14) );
  pTextStyle->setTextWidth( m_pModel3d->convertWorkingUnitsToUORs(0.14) );
  //pTextStyle->setItalicsFlag(true);
  pTextStyle->setSlant( OdaPI / 6 );
  pTextStyle->setColorIndex( 2 );
  pTextStyle->setFontEntryId( (!(pFontTable->getFont("Verdana")).isNull()) ? pFontTable->getFont("Verdana")->getNumber() 
                                                                           : nAlternateFontNumber );
  pTextStyleTable->add(pTextStyle);

  pTextStyle = OdDgTextStyleTableRecord::createObject();
  pTextStyle->setName("TextStyle2");
  pTextStyle->setTextHeight( m_pModel3d->convertWorkingUnitsToUORs(0.23) );
  pTextStyle->setTextWidth( m_pModel3d->convertWorkingUnitsToUORs(0.23) );
  pTextStyle->setColorIndex( 7 );
  pTextStyle->setFontEntryId( (!(pFontTable->getFont("ENGINEERING")).isNull()) ? pFontTable->getFont("ENGINEERING")->getNumber() 
                                                                               : nAlternateFontNumber );
  pTextStyleTable->add(pTextStyle);

  pTextStyle = OdDgTextStyleTableRecord::createObject();
  pTextStyle->setName("TextStyle3");
  OdDgElementId id = pTextStyleTable->getAt("TextStyle1");
  if (!id.isNull())
  {
    OdDgTextStyleTableRecordPtr pParentTextStyle = id.safeOpenObject();
    pTextStyle->setParentTextStyleEntryId( pParentTextStyle->getEntryId() );
  }
  pTextStyle->setItalicsFlag(false);
  pTextStyle->setColorIndex( 4 );
  pTextStyle->setFontEntryId( (!(pFontTable->getFont("Arial")).isNull()) ? pFontTable->getFont("Arial")->getNumber() 
                                                                         : nAlternateFontNumber );
  pTextStyleTable->add(pTextStyle);

//
  // Set a couple of RSC fonts with a specific number without any conditions
  pTextStyle = OdDgTextStyleTableRecord::createObject();
  pTextStyle->setName("TextStyleRSC_245");
  pTextStyle->setTextHeight( m_pModel2d->convertWorkingUnitsToUORs(0.14) );
  pTextStyle->setTextWidth( m_pModel2d->convertWorkingUnitsToUORs(0.14) );
  pTextStyle->setSlant( OdaPI / 6 );
  pTextStyle->setColorIndex( 5 );
  pTextStyle->setFontEntryId ( 245 );
  pTextStyleTable->add(pTextStyle);

  pTextStyle = OdDgTextStyleTableRecord::createObject();
  pTextStyle->setName("TextStyleRSC_255");
  pTextStyle->setTextHeight( m_pModel2d->convertWorkingUnitsToUORs(0.27) );
  pTextStyle->setTextWidth( m_pModel2d->convertWorkingUnitsToUORs(0.27) );
  pTextStyle->setColorIndex( 6 );
  pTextStyle->setFontEntryId ( 255 );
  pTextStyleTable->add(pTextStyle);

  // Text styles for terrain element.

  pTextStyle = OdDgTextStyleTableRecord::createObject();
  pTextStyle->setName("Terrain Contour");
  pTextStyle->setTextHeight( m_pModel2d->convertWorkingUnitsToUORs(0.01) );
  pTextStyle->setTextWidth( m_pModel2d->convertWorkingUnitsToUORs(0.01) );
  pTextStyle->setColorIndex( 1 );
  pTextStyle->setFontEntryId( (!(pFontTable->getFont("ENGINEERING")).isNull()) ? pFontTable->getFont("ENGINEERING")->getNumber() 
                              : nAlternateFontNumber );
  pTextStyleTable->add(pTextStyle);

  pTextStyle = OdDgTextStyleTableRecord::createObject();
  pTextStyle->setName("Terrain High Point");
  pTextStyle->setTextHeight( m_pModel2d->convertWorkingUnitsToUORs(0.025) );
  pTextStyle->setTextWidth( m_pModel2d->convertWorkingUnitsToUORs(0.025) );
  pTextStyle->setColorIndex( 5 );
  pTextStyle->setFontEntryId( (!(pFontTable->getFont("Arial")).isNull()) ? pFontTable->getFont("Arial")->getNumber() 
                              : nAlternateFontNumber );
  pTextStyleTable->add(pTextStyle);
}

void OdExDgnFiller::addColorTableRecord()
{
  OdDgColorTablePtr pColorTbl = m_pModel3d->database()->getColorTable(OdDg::kForWrite);
  if (!pColorTbl.isNull())
  {
    const ODCOLORREF* refColors = pColorTbl->defaultPalette();
    OdArray<ODCOLORREF> colors;
    for (int i = 0; i < 256; i++)
    {
      colors.append(refColors[i]);
    }
    pColorTbl->setPalette(colors);
  }
}

void OdExDgnFiller::addElementTemplates()
{
  OdDgElementTemplateTablePtr pTemplateTable = m_pModel3d->database()->getElementTemplateTable(OdDg::kForWrite);

  if( !pTemplateTable.isNull() )
  {
    // add template group

    OdDgElementTemplatePtr pTemplateGroup = OdDgElementTemplate::createObject();
    pTemplateGroup->setTemplateGroup( true );
    pTemplateGroup->setTemplateName( L"Main Tmp Group");
    pTemplateGroup->setTemplateOrder( 1 );

    pTemplateTable->add( pTemplateGroup );

    // add level and line style template

    OdDgElementTemplatePtr pLevelAndLSTemplate = OdDgElementTemplate::createObject();
    pLevelAndLSTemplate->setTemplateGroup( false );
    pLevelAndLSTemplate->setTemplateName( L"Level And Line Style" );
    pLevelAndLSTemplate->setTemplateOrder( 1 );

    pTemplateGroup->addElementTemplate( pLevelAndLSTemplate );

    OdDgTemplateItem levelItem;
    levelItem.addPathItem(L"Levels");
    levelItem.addPathItem(L"Level");
    levelItem.addPathItem(L"Value");

    OdDgTemplateValueVariant levelValue;
    levelValue.setDataType(OdDgTemplateValueVariant::kString);
    levelValue.setStringValue(L"level 2");

    levelItem.addValue( levelValue );

    levelValue.setStringValue(L"custom name");

    levelItem.addValue( levelValue );

    pLevelAndLSTemplate->addTemplateItem( levelItem );

    OdDgTemplateItem lineStyleItem;
    lineStyleItem.addPathItem(L"LineStyles");
    lineStyleItem.addPathItem(L"LineStyle");
    lineStyleItem.addPathItem(L"Value");

    OdDgTemplateValueVariant lsValue;
    lsValue.setDataType(OdDgTemplateValueVariant::kString);
    lsValue.setStringValue(L"( Center )");

    lineStyleItem.addValue( lsValue );

    lsValue.setStringValue(L"{ Gas Line }");

    lineStyleItem.addValue( lsValue );

    pLevelAndLSTemplate->addTemplateItem( lineStyleItem );

    // add fill properties template

    OdDgElementTemplatePtr pFillParamTemplate = OdDgElementTemplate::createObject();
    pFillParamTemplate->setTemplateGroup( false );
    pFillParamTemplate->setTemplateName( L"Fill Properties" );
    pFillParamTemplate->setTemplateOrder( 2 );

    pTemplateGroup->addElementTemplate( pFillParamTemplate );

    OdDgTemplateItem areaItem;
    areaItem.addPathItem(L"AreaModes");
    areaItem.addPathItem(L"AreaMode");
    areaItem.addPathItem(L"Value");

    OdDgTemplateValueVariant areaValue;
    areaValue.setDataType(OdDgTemplateValueVariant::kByte);
    areaValue.setByteValue(1);

    areaItem.addValue( areaValue );

    pFillParamTemplate->addTemplateItem( areaItem );

    OdDgTemplateItem fillModeItem;
    fillModeItem.addPathItem(L"FillModes");
    fillModeItem.addPathItem(L"FillMode");
    fillModeItem.addPathItem(L"Value");

    OdDgTemplateValueVariant fillValue;
    fillValue.setDataType(OdDgTemplateValueVariant::kByte);
    fillValue.setByteValue(1);

    fillModeItem.addValue( fillValue );

    pFillParamTemplate->addTemplateItem( fillModeItem );

    OdDgTemplateItem fillColorItem;
    fillColorItem.addPathItem(L"FillColors");
    fillColorItem.addPathItem(L"FillColor");
    fillColorItem.addPathItem(L"Value");

    OdBinaryData extData;

    extData.resize(40, 0);

    OdDgTemplateColorValue tmpColor;
    tmpColor.setColorType( OdDgTemplateColorValue::kColorBook );
    tmpColor.setBookAndColorName(L"Standardburlywood");
    tmpColor.setColorIndex(0);
    tmpColor.setColorBookIndex(8);
    tmpColor.setColorIndexInBook(9);
    tmpColor.setColorRGB( ODRGB( 222, 184, 135 ) );
    tmpColor.setExtendedColorData( extData ); // Only for fill color param

    OdDgTemplateValueVariant fillColorValue;
    fillColorValue.setDataType(OdDgTemplateValueVariant::kColor);
    fillColorValue.setColorValue(tmpColor);

    fillColorItem.addValue(fillColorValue);

    pFillParamTemplate->addTemplateItem( fillColorItem );

    // add child template group

    OdDgElementTemplatePtr pChildTemplateGroup = OdDgElementTemplate::createObject();
    pChildTemplateGroup->setTemplateGroup( true );
    pChildTemplateGroup->setTemplateName( L"Child Tmp Group");
    pChildTemplateGroup->setTemplateOrder( 3 );

    pTemplateGroup->addElementTemplate( pChildTemplateGroup );

    // add color template

    OdDgElementTemplatePtr pColorTemplate = OdDgElementTemplate::createObject();
    pColorTemplate->setTemplateGroup( false );
    pColorTemplate->setTemplateName( L"Colors");
    pColorTemplate->setTemplateOrder( 1 );

    pChildTemplateGroup->addElementTemplate( pColorTemplate );

    OdDgTemplateItem colorItem;
    colorItem.addPathItem(L"Colors");
    colorItem.addPathItem(L"Color");
    colorItem.addPathItem(L"Value");

    OdDgTemplateColorValue valColor;
    valColor.setColorType( OdDgTemplateColorValue::kColorByLevel );

    OdDgTemplateValueVariant colorValue;
    colorValue.setDataType(OdDgTemplateValueVariant::kColor);
    colorValue.setColorValue(valColor);

    colorItem.addValue(colorValue);

    valColor.setColorType( OdDgTemplateColorValue::kColorByCell );
    colorValue.setColorValue(valColor);

    colorItem.addValue(colorValue);

    valColor.setColorType( OdDgTemplateColorValue::kColorIndex );
    valColor.setColorIndex( 9 );

    colorValue.setColorValue(valColor);

    colorItem.addValue(colorValue);

    valColor.setColorType( OdDgTemplateColorValue::kColorRGB );
    valColor.setColorIndex( 0 );
    valColor.setColorRGB( ODRGB( 234,56,78) );

    colorValue.setColorValue(valColor);

    colorItem.addValue(colorValue);

    valColor.setColorType( OdDgTemplateColorValue::kColorBook );
    valColor.setColorIndex( 0 );
    valColor.setColorRGB( ODRGB( 240,255,255) );
    valColor.setColorBookIndex( 8 );
    valColor.setColorIndexInBook( 5 );
    valColor.setBookAndColorName(L"Standardazure");

    colorValue.setColorValue(valColor);

    colorItem.addValue(colorValue);

    pColorTemplate->addTemplateItem( colorItem );
  }
}

void OdExDgnFiller::addColorBooks()
{
  OdDgColorBookTablePtr pColorBookTable = m_pModel3d->database()->getColorBookTable(OdDg::kForWrite);

  OdDgColorBookPtr pColorBook1 = OdDgColorBook::createObject();
  pColorBook1->setColorBookName( L" DgnCreate ColorBook 1");

  OdDgColorBookItem tmp;

  tmp.m_strColorName = L"First Color";
  tmp.m_clrColor =ODRGB( 255,0,255 );
  pColorBook1->addColorBookItem(tmp);

  tmp.m_strColorName = L"Second Color";
  tmp.m_clrColor =ODRGB( 255,255,0 );
  pColorBook1->addColorBookItem(tmp);

  tmp.m_strColorName = L"Third Color";
  tmp.m_clrColor =ODRGB( 128,0,255 );
  pColorBook1->addColorBookItem(tmp);

  OdDgColorBookPtr pColorBook2 = OdDgColorBook::createObject();
  pColorBook2->setColorBookName( L" DgnCreate ColorBook 2");


  tmp.m_strColorName = L"Red Color";
  tmp.m_clrColor =ODRGB( 255,0,0 );
  pColorBook2->addColorBookItem(tmp);

  tmp.m_strColorName = L"Green Color";
  tmp.m_clrColor =ODRGB( 0,255,0 );
  pColorBook2->addColorBookItem(tmp);

  tmp.m_strColorName = L"Blue Color";
  tmp.m_clrColor =ODRGB( 0,0,255 );
  pColorBook2->addColorBookItem(tmp);

  pColorBookTable->add( pColorBook1 );
  pColorBookTable->add( pColorBook2 );
}

void OdExDgnFiller::addACS()
{
  OdDgACSPtr pACS = OdDgACS::createObject();

  pACS->setName(L"Custom ACS");
  pACS->setDescription(L"Dgn Create ACS");
  pACS->setOrigin( OdGePoint3d(20.5, 35.8, 8.89));
  pACS->setRotation( OdGeMatrix3d::rotation(OdaPI/4, OdGeVector3d::kZAxis) );
  pACS->setType( OdDgModel::kCylindrical );

  m_pModel3d->addElement( pACS );
}

void OdExDgnFiller::addEnvironmentMapTable()
{
  OdDgEnvironmentMapTablePtr pTable = m_pModel3d->database()->getEnvironmentMapTable( OdDg::kForWrite );

  OdDgEnvironmentMap envMap;

  envMap.setMapName( L"Dgn Create map");
  envMap.setEnableFlag( true );

  pTable->addMap( envMap );
}

void OdExDgnFiller::addPrintStyle()
{
  OdDgPrintStyleTableRecordPtr pPrintStyle = OdDgPrintStyleTableRecord::createObject();

  pPrintStyle->setName(L"DgnCreatePrintStyle");
  pPrintStyle->setDefaultStyleFlag(true);

  OdDgPrintStyleTablePtr pPrintStyleTable = m_pModel3d->database()->getPrintStyleTable();
  pPrintStyleTable->add(pPrintStyle);
}

void OdExDgnFiller::addDictionaryTableAndFence()
{
  OdDgDictionaryPtr pRootDictionary = m_pModel3d->database()->getDictionaryTable(OdDg::kForWrite);

  // Create simple dictionary table.

  OdDgDictionaryPtr pSimpleDictionary = OdDgDictionary::createObject();
  pSimpleDictionary->setName(L"DgnCreate Dictionary");
  pSimpleDictionary->setOwnerFlags(0);
  pSimpleDictionary->setCloneFlags(0);

  OdDgDictionaryPtr pSubDictionary1 = OdDgDictionary::createObject();
  pSubDictionary1->setName(L"SubDictionary 1");

  OdDgDictionaryPtr pSubDictionary2 = OdDgDictionary::createObject();
  pSubDictionary2->setName(L"SubDictionary 2");

  OdDgDictionaryPtr pSubDictionary3 = OdDgDictionary::createObject();
  pSubDictionary3->setName(L"SubDictionary 3");

  OdDgDictionaryPtr pLevel2Dictionary1 = OdDgDictionary::createObject();
  pLevel2Dictionary1->setName(L"ChildSubDictionary 1");

  OdDgDictionaryPtr pLevel2Dictionary2 = OdDgDictionary::createObject();
  pLevel2Dictionary2->setName(L"ChildSubDictionary 2");

  pSubDictionary3->add( pLevel2Dictionary1 );
  pSubDictionary3->add( pLevel2Dictionary2 );
  pSimpleDictionary->add( pSubDictionary1 );
  pSimpleDictionary->add( pSubDictionary2 );
  pSimpleDictionary->add( pSubDictionary3 );

  pRootDictionary->add(pSimpleDictionary);

  // Create 2 named fences for first model.

  OdDgDictionaryPtr pFenceRoot = OdDgDictionary::createObject();
  pFenceRoot->setName(L"NamedFencesForModel_0");

  OdDgDictionaryPtr pFence1 = OdDgDictionary::createObject();
  pFence1->setName(L"DgnFence 1");

  OdDgShape3dPtr pFence1Base = OdDgShape3d::createObject();
  pFence1Base->addVertex( OdGePoint3d(6.0,0.2,0.0) );
  pFence1Base->addVertex( OdGePoint3d(8.0,0.2,0.0) );
  pFence1Base->addVertex( OdGePoint3d(8.0,3.0,0.0) );
  pFence1Base->addVertex( OdGePoint3d(6.0,3.0,0.0) );
  pFence1Base->addVertex( OdGePoint3d(6.0,0.2,0.0) );
  pFence1Base->setInvisibleFlag(true);

  m_pModel3d->addElement( pFence1Base );

  OdDgDepLinkageElementIdPtr pDepToFence1Base = OdDgDepLinkageElementId::createObject();
  pDepToFence1Base->add( (OdUInt64)(pFence1Base->elementId().getHandle()) );
  pDepToFence1Base->setAppId(9994);
  pDepToFence1Base->setAppValue(0);

  pFence1->addLinkage( pDepToFence1Base->getPrimaryId(), pDepToFence1Base.get(), false);

  OdDgDictionaryPtr pFence2 = OdDgDictionary::createObject();
  pFence2->setName(L"DgnFence 2");

  OdDgShape3dPtr pFence2Base = OdDgShape3d::createObject();
  pFence2Base->addVertex( OdGePoint3d(8.2,0.2,0.0) );
  pFence2Base->addVertex( OdGePoint3d(9.3,3.0,0.0) );
  pFence2Base->addVertex( OdGePoint3d(10.6,0.2,0.0) );
  pFence2Base->addVertex( OdGePoint3d(8.2,0.2,0.0) );
  pFence2Base->setInvisibleFlag(true);

  m_pModel3d->addElement( pFence2Base );

  OdDgDepLinkageElementIdPtr pDepToFence2Base = OdDgDepLinkageElementId::createObject();
  pDepToFence2Base->add( (OdUInt64)(pFence2Base->elementId().getHandle()) );
  pDepToFence2Base->setAppId(9994);
  pDepToFence2Base->setAppValue(0);

  pFence2->addLinkage( pDepToFence2Base->getPrimaryId(), pDepToFence2Base.get(), false);

  pFenceRoot->add( pFence1 );
  pFenceRoot->add( pFence2 );

  pRootDictionary->add( pFenceRoot );
}

void OdExDgnFiller::addXmlCustomScheme()
{
  OdDgTagDefinitionSetTablePtr pTagSetTable = m_pModel3d->database()->getTagDefinitionSetTable(OdDg::kForRead);
  OdDgXMLCustomSchemaContainerPtr pCustomScheme = OdDgXMLCustomSchemaContainer::createObject();
  pCustomScheme->setName(L"CustomXmlSchemeSample");
  pCustomScheme->setVersion(L"01.00");

  OdDgCustomItemTypePtr pItemType = OdDgCustomItemType::createObject();
  pItemType->setName(L"CustomItem");

  OdDgCustomItemTypePropertyPtr pProp1 = OdDgCustomItemTypeProperty::createObject();
  pProp1->setName(L"Property1");
  pProp1->setType(OdDgItemTypeProperty::kString);

  OdDgCustomItemTypePropertyPtr pProp2 = OdDgCustomItemTypeProperty::createObject();
  pProp2->setName(L"Property2");
  pProp2->setType(OdDgItemTypeProperty::kInteger);
  pProp2->setArrayFlag(true);

  OdDgCustomItemTypePropertyPtr pProp3 = OdDgCustomItemTypeProperty::createObject();
  pProp3->setName(L"Property3");
  pProp3->setType(OdDgItemTypeProperty::kBoolean);

  OdDgCustomItemTypePropertyPtr pProp4 = OdDgCustomItemTypeProperty::createObject();
  pProp4->setName(L"Property4");
  pProp4->setType(OdDgItemTypeProperty::kDouble);

  pItemType->setProperty(pProp1);
  pItemType->setProperty(pProp2);
  pItemType->setProperty(pProp3);
  pItemType->setProperty(pProp4);
  pCustomScheme->addCustomItem(pItemType);

  pTagSetTable->add(pCustomScheme);
}

void OdExDgnFiller::addVariblesList()
{
  // Create variable list and add it to the model.
  OdDgVariablesListPtr pVarList = OdDgVariablesList::createObject();
  m_pModel3d->addElement(pVarList);

  // Create local variable with type "distance".
  OdDgVariablePtr pVar = OdDgVariable::createObject();
  pVar->setName(L"Variable1");
  pVar->setReadOnlyFlag(false);
  pVar->setScope( OdDgVariable::kDefinition );
  pVar->setType( OdDgVariable::kDistance );
  pVar->setVisibleFlag( true );
  pVar->setDisplayName(L"DistanceVar");
  pVar->setDefaultValue( OdDgVariableValueVariant( OdDgVariable::kDistance, 12.3) );
  pVarList->addLocalVariable(pVar);

  // Create local variable with type "integer".
  OdDgVariablePtr pVar2 = OdDgVariable::createObject();
  pVar2->setName(L"Variable2");
  pVar2->setReadOnlyFlag(false);
  pVar2->setScope( OdDgVariable::kInstance );
  pVar2->setType( OdDgVariable::kInteger );
  pVar2->setVisibleFlag( true );
  pVar2->setDisplayName(L"IntegerVar");
  pVar2->setDefaultValue( OdDgVariableValueVariant((OdInt32)6) );
  pVarList->addLocalVariable(pVar2);
  
  // Create variable value set.
  pVarList->addVariableValuesSet(L"Variations");

  // Create local variable with type "numeric".
  OdDgVariablePtr pVar3 = OdDgVariable::createObject();
  pVar3->setName(L"Variable3");
  pVar3->setReadOnlyFlag(false);
  pVar3->setScope( OdDgVariable::kInstance );
  pVar3->setType( OdDgVariable::kNumber );
  pVar3->setVisibleFlag( true );
  pVar3->setDisplayName(L"NumericVar");
  pVar3->setDefaultValue( OdDgVariableValueVariant(OdDgVariable::kNumber, 56.7) );
  pVarList->addLocalVariable(pVar3);

  // Add item type to variables list to use item based variables.
  OdDgTagDefinitionSetTablePtr pTagSetTable = m_pModel3d->database()->getTagDefinitionSetTable(OdDg::kForRead);
  OdDgItemTypeLibraryPtr pItemTypeLib = pTagSetTable->getAt(convertStringToECSchemaItemName(L"Dgn Create Lib")).openObject(OdDg::kForRead);
  pVarList->setItemTypeLibrary( pItemTypeLib->elementId(), L"Sample Item");

  // Add expression to "Var2" ( var2 = Simple__x0020__Integer * 4 )
  OdDgExpressionItemPtr pItemVarIntOper = OdDgExpressionItem::createObject(OdDgExpressionItem::kVariable);
  OdDgVariableValue* pItemVarInt = (OdDgVariableValue*)(pItemVarIntOper.get());
  pItemVarInt->setVariableName(convertStringToECSchemaItemName(L"Simple Integer"));

  OdDgExpressionItemPtr pConst4Oper = OdDgExpressionItem::createObject(OdDgExpressionItem::kConstant);
  OdDgConstantValue* pConst4 = (OdDgConstantValue*)(pConst4Oper.get());
  pConst4->setValue( OdDgExpressionConstValueVariant((OdInt32)4) );

  OdDgExpressionItemPtr pMulOper = OdDgExpressionItem::createObject(OdDgExpressionItem::kMultiply);
  OdDgArithmeticOperator* pMul = (OdDgArithmeticOperator*)(pMulOper.get());
  pMul->setOperand( 0, pItemVarIntOper );
  pMul->setOperand( 1, pConst4Oper );

  OdDgExpressionVariablePtr pIntMul = OdDgExpressionVariable::createObject();
  pIntMul->setExpression(pMulOper);
  pVar2->setExpression(pIntMul);

  // Add expression to "Var3" ( var3 = num( Var1 + Var2 ) )
  OdDgExpressionItemPtr pVar1Oper = OdDgExpressionItem::createObject(OdDgExpressionItem::kVariable);
  OdDgVariableValue* pVar1Name = (OdDgVariableValue*)(pVar1Oper.get());
  pVar1Name->setVariableName(L"Variable1");

  OdDgExpressionItemPtr pVar2Oper = OdDgExpressionItem::createObject(OdDgExpressionItem::kVariable);
  OdDgVariableValue* pVar2Name = (OdDgVariableValue*)(pVar2Oper.get());
  pVar2Name->setVariableName(L"Variable2");

  OdDgExpressionItemPtr pAddOper = OdDgExpressionItem::createObject(OdDgExpressionItem::kAdd);
  OdDgArithmeticOperator* pAdd = (OdDgArithmeticOperator*)(pAddOper.get());
  pAdd->setOperand( 0, pVar1Oper );
  pAdd->setOperand( 1, pVar2Oper );

  OdDgExpressionVariablePtr pNumAdd = OdDgExpressionVariable::createObject();
  pNumAdd->setExpression(pAddOper);
  pVar3->setExpression(pNumAdd);

  // Set "Var1" and "Simple__x0020__Integer" values for variable value set.

  OdDgElementId idVarSet = pVarList->getVariableValuesSet(L"Variations");
  OdDgVariableValuesSetPtr pSet = idVarSet.openObject(OdDg::kForWrite);
  pSet->setVariableValue(L"Simple__x0020__Integer", OdDgVariableValueVariant(OdInt32(77)) );
  pSet->setVariableValue(L"Variable1", OdDgVariableValueVariant(OdDgVariable::kDistance, 84.67) );

  // Clone variables list and set it to other model

  OdDgVariablesListPtr pCloneList = pVarList->clone();
  m_pModel2d->addElement(pCloneList);
}

void OdExDgnFiller::addItemTypesLibrary()
{
  OdDgTagDefinitionSetTablePtr pTagSetTable = m_pModel3d->database()->getTagDefinitionSetTable(OdDg::kForWrite);

  OdDgItemTypeLibraryPtr pItemLibrary = OdDgItemTypeLibrary::createObject();
  pItemLibrary->setName(L"Dgn Create Lib");
  pItemLibrary->setDescription(L"Sample item set library");

  // Simple types.

  OdDgItemTypePtr pItem1 = OdDgItemType::createObject();
  pItem1->setCategory( OdDgItemType::kItemType );
  pItem1->setName(L"Sample Item");
  pItem1->setDescription(L"This item contains only simple properties.");
  pItem1->setUseNameForElementFlag(true);

  OdDgItemTypePropertyValue defaultValue;
  OdDgItemTypePropertyUnits unitData;

  OdDgItemTypePropertyPtr pProperty11 = OdDgItemTypeProperty::createObject();
  pProperty11->setName(L"Simple String");
  pProperty11->setType( OdDgItemTypeProperty::kString );
  pProperty11->setDescription(L"Simple string property. Default Value is \"Dgn Create\". No Units.");
  pProperty11->setReadOnlyFlag(false);
  pProperty11->setUsePropertyTypeFlag(false);
  pProperty11->setArrayFlag(false);
  defaultValue.setStringValue(L"Dgn Create");
  pProperty11->setDefaultValue( defaultValue );

  OdDgItemTypePropertyPtr pProperty12 = OdDgItemTypeProperty::createObject();
  pProperty12->setName(L"Simple Integer");
  pProperty12->setType( OdDgItemTypeProperty::kInteger );
  pProperty12->setDescription(L"Simple integer property. Default Value is \"128\". No Units.");
  pProperty12->setReadOnlyFlag(false);
  pProperty12->setUsePropertyTypeFlag(false);
  pProperty12->setArrayFlag(false);
  defaultValue.setIntegerValue(128);
  pProperty12->setDefaultValue( defaultValue );

  OdDgItemTypePropertyPtr pProperty13 = OdDgItemTypeProperty::createObject();
  pProperty13->setName(L"Simple Double");
  pProperty13->setType( OdDgItemTypeProperty::kDouble );
  pProperty13->setDescription(L"Simple double property. Default Value is \"16.75\". Area Working Units.");
  pProperty13->setReadOnlyFlag(false);
  pProperty13->setUsePropertyTypeFlag(false);
  pProperty13->setArrayFlag(false);
  defaultValue.setDoubleValue(16.75);
  pProperty13->setDefaultValue( defaultValue );
  unitData.setUnitsType( OdDgItemTypePropertyUnits::kSquareWorkingUnits );
  pProperty13->setUnits( unitData );

  OdDgItemTypePropertyPtr pProperty14 = OdDgItemTypeProperty::createObject();
  pProperty14->setName(L"Simple Boolean");
  pProperty14->setType( OdDgItemTypeProperty::kBoolean );
  pProperty14->setDescription(L"Simple bool property. Default Value is \"False\". No Units.");
  pProperty14->setReadOnlyFlag(false);
  pProperty14->setUsePropertyTypeFlag(false);
  pProperty14->setArrayFlag(false);
  defaultValue.setBooleanValue(false);
  pProperty14->setDefaultValue( defaultValue );

  OdDgItemTypePropertyPtr pProperty15 = OdDgItemTypeProperty::createObject();
  pProperty15->setName(L"Simple Point3d");
  pProperty15->setType( OdDgItemTypeProperty::kPoint3d );
  pProperty15->setDescription(L"Simple point3d property. Default Value is \"{0.1,0.5,3.4}\". CENTIMETRE Units.");
  pProperty15->setReadOnlyFlag(false);
  pProperty15->setUsePropertyTypeFlag(false);
  pProperty15->setArrayFlag(false);
  defaultValue.setPointValue(OdGePoint3d(0.1,0.5,3.4));
  pProperty15->setDefaultValue( defaultValue );
  unitData.setUnitsName(L"CENTIMETRE");
  pProperty15->setUnits(unitData);

  OdDgItemTypePropertyPtr pProperty16 = OdDgItemTypeProperty::createObject();
  pProperty16->setName(L"Simple DateTime");
  pProperty16->setType( OdDgItemTypeProperty::kDateTime);
  pProperty16->setDescription(L"Simple dateTime property. Default Value is \"{19.11.1981 03:15:00}\". No Units.");
  pProperty16->setReadOnlyFlag(false);
  pProperty16->setUsePropertyTypeFlag(false);
  pProperty16->setArrayFlag(false);
  OdTimeStamp defData;
  defData.setDate(11,19,1981);
  defData.setTime(3,15,00,00);
  defaultValue.setDateTimeValue(defData);
  pProperty16->setDefaultValue( defaultValue );

  pItem1->addProperty( pProperty11 );
  pItem1->addProperty( pProperty12 );
  pItem1->addProperty( pProperty13 );
  pItem1->addProperty( pProperty14 );
  pItem1->addProperty( pProperty15 );
  pItem1->addProperty( pProperty16 );

  // Array types.

  OdDgItemTypePtr pItem2 = OdDgItemType::createObject();
  pItem2->setCategory( OdDgItemType::kItemType );
  pItem2->setName(L"Array Item");
  pItem2->setDescription(L"This item contains array properties.");
  pItem2->setUseNameForElementFlag(false);

  OdDgItemTypePropertyPtr pProperty21 = OdDgItemTypeProperty::createObject();
  pProperty21->setName(L"String Array");
  pProperty21->setType( OdDgItemTypeProperty::kString );
  pProperty21->setDescription(L"String array property. Default Value is \"Str Array Item Sample\". No Units.");
  pProperty21->setReadOnlyFlag(false);
  pProperty21->setUsePropertyTypeFlag(false);
  pProperty21->setArrayFlag(true);
  defaultValue.setStringValue(L"Str Array Item Sample");
  pProperty21->setDefaultValue( defaultValue );

  OdDgItemTypePropertyPtr pProperty22 = OdDgItemTypeProperty::createObject();
  pProperty22->setName(L"Integer Array");
  pProperty22->setType( OdDgItemTypeProperty::kInteger );
  pProperty22->setDescription(L"Integer array property. Default Value is \"777\". No Units.");
  pProperty22->setReadOnlyFlag(false);
  pProperty22->setUsePropertyTypeFlag(false);
  pProperty22->setArrayFlag(true);
  defaultValue.setIntegerValue(777);
  pProperty22->setDefaultValue( defaultValue );

  OdDgItemTypePropertyPtr pProperty23 = OdDgItemTypeProperty::createObject();
  pProperty23->setName(L"Double Array");
  pProperty23->setType( OdDgItemTypeProperty::kDouble );
  pProperty23->setDescription(L"Double array property. Default Value is \"-34.8\". Working Units.");
  pProperty23->setReadOnlyFlag(false);
  pProperty23->setUsePropertyTypeFlag(false);
  pProperty23->setArrayFlag(true);
  defaultValue.setDoubleValue(-34.8);
  pProperty23->setDefaultValue( defaultValue );
  unitData.setUnitsType( OdDgItemTypePropertyUnits::kWorkingUnits );
  pProperty23->setUnits( unitData );

  OdDgItemTypePropertyPtr pProperty24 = OdDgItemTypeProperty::createObject();
  pProperty24->setName(L"Boolean Array");
  pProperty24->setType( OdDgItemTypeProperty::kBoolean );
  pProperty24->setDescription(L"Bool array property. Default Value is \"True\". No Units.");
  pProperty24->setReadOnlyFlag(false);
  pProperty24->setUsePropertyTypeFlag(false);
  pProperty24->setArrayFlag(true);
  defaultValue.setBooleanValue(true);
  pProperty24->setDefaultValue( defaultValue );

  OdDgItemTypePropertyPtr pProperty25 = OdDgItemTypeProperty::createObject();
  pProperty25->setName(L"Point3d Array");
  pProperty25->setType( OdDgItemTypeProperty::kPoint3d );
  pProperty25->setDescription(L"Point3d array property. Default Value is \"{-5.7,0.25,13.8}\". DECIMETRE Units.");
  pProperty25->setReadOnlyFlag(false);
  pProperty25->setUsePropertyTypeFlag(false);
  pProperty25->setArrayFlag(true);
  defaultValue.setPointValue(OdGePoint3d(-5.7,0.25,13.8));
  pProperty25->setDefaultValue( defaultValue );
  unitData.setUnitsName(L"DECIMETRE");
  pProperty25->setUnits(unitData);

  OdDgItemTypePropertyPtr pProperty26 = OdDgItemTypeProperty::createObject();
  pProperty26->setName(L"DateTime Array");
  pProperty26->setType( OdDgItemTypeProperty::kDateTime);
  pProperty26->setDescription(L"DateTime array property. Default Value is \"{07.07.2008 20:00:00}\". No Units.");
  pProperty26->setReadOnlyFlag(false);
  pProperty26->setUsePropertyTypeFlag(false);
  pProperty26->setArrayFlag(true);
  defData.setDate(7,7,2008);
  defData.setTime(20,00,00,00);
  defaultValue.setDateTimeValue(defData);
  pProperty26->setDefaultValue( defaultValue );

  pItem2->addProperty( pProperty21 );
  pItem2->addProperty( pProperty22 );
  pItem2->addProperty( pProperty23 );
  pItem2->addProperty( pProperty24 );
  pItem2->addProperty( pProperty25 );
  pItem2->addProperty( pProperty26 );

  // Property type.

  OdDgItemTypePtr pItem3 = OdDgItemType::createObject();
  pItem3->setCategory( OdDgItemType::kPropertyType );
  pItem3->setName(L"Complex Property");
  pItem3->setDescription(L"Sample of property type.");
  pItem3->setUseNameForElementFlag(true);

  OdDgItemTypePropertyPtr pProperty31 = OdDgItemTypeProperty::createObject();
  pProperty31->setName(L"Complex String");
  pProperty31->setType( OdDgItemTypeProperty::kString );
  pProperty31->setDescription(L"String property. Default Value is \"Complex String\". No Units.");
  pProperty31->setReadOnlyFlag(false);
  pProperty31->setUsePropertyTypeFlag(false);
  pProperty31->setArrayFlag(false);
  defaultValue.setStringValue(L"Complex String");
  pProperty31->setDefaultValue( defaultValue );

  OdDgItemTypePropertyPtr pProperty32 = OdDgItemTypeProperty::createObject();
  pProperty32->setName(L"Complex Integer Array");
  pProperty32->setType( OdDgItemTypeProperty::kInteger );
  pProperty32->setDescription(L"Complex integer array property. Default Value is \"19\". No Units.");
  pProperty32->setReadOnlyFlag(false);
  pProperty32->setUsePropertyTypeFlag(false);
  pProperty32->setArrayFlag(true);
  defaultValue.setIntegerValue(19);
  pProperty32->setDefaultValue( defaultValue );

  OdDgItemTypePropertyPtr pProperty33 = OdDgItemTypeProperty::createObject();
  pProperty33->setName(L"Complex Double");
  pProperty33->setType( OdDgItemTypeProperty::kDouble );
  pProperty33->setDescription(L"Complex double property. Default Value is \"123.456\". Volume Working Units.");
  pProperty33->setReadOnlyFlag(false);
  pProperty33->setUsePropertyTypeFlag(false);
  pProperty33->setArrayFlag(false);
  defaultValue.setDoubleValue(123.456);
  pProperty33->setDefaultValue( defaultValue );
  unitData.setUnitsType( OdDgItemTypePropertyUnits::kCubicWorkingUnits );
  pProperty33->setUnits( unitData );

  pItem3->addProperty( pProperty31 );
  pItem3->addProperty( pProperty32 );
  pItem3->addProperty( pProperty33 );

  // Item that use property type.

  OdDgItemTypePtr pItem4 = OdDgItemType::createObject();
  pItem4->setCategory( OdDgItemType::kItemType );
  pItem4->setName(L"Complex Item");
  pItem4->setDescription(L"Item that use property type.");
  pItem4->setUseNameForElementFlag(true);

  OdDgItemTypePropertyPtr pProperty41 = OdDgItemTypeProperty::createObject();
  pProperty41->setName(L"Single Property Type");
  pProperty41->setType( OdDgItemTypeProperty::kCustom );
  pProperty41->setTypeName( pItem3->getName() );
  pProperty41->setDescription(L"Single property type. No Default value. No Units.");
  pProperty41->setReadOnlyFlag(false);
  pProperty41->setUsePropertyTypeFlag(true);
  pProperty41->setArrayFlag(false);

  OdDgItemTypePropertyPtr pProperty42 = OdDgItemTypeProperty::createObject();
  pProperty42->setName(L"Array Of Property Types");
  pProperty42->setType( OdDgItemTypeProperty::kCustom );
  pProperty42->setTypeName( pItem3->getName() );
  pProperty42->setDescription(L"Array of property types. No Default Value. No Units.");
  pProperty42->setReadOnlyFlag(false);
  pProperty42->setUsePropertyTypeFlag(true);
  pProperty42->setArrayFlag(true);

  pItem4->addProperty( pProperty41 );
  pItem4->addProperty( pProperty42 );

  pItemLibrary->addItemType( pItem1 );
  pItemLibrary->addItemType( pItem2 );
  pItemLibrary->addItemType( pItem3 );
  pItemLibrary->addItemType( pItem4 );

  pTagSetTable->add( pItemLibrary );
}

void OdExDgnFiller::addDisplayStyleTableRecord()
{
  OdDgDisplayStyleTablePtr pDisplayStyleTable = m_pModel3d->database()->getDisplayStyleTable( OdDg::kForWrite );

  OdDgDisplayStyleTableRecordPtr pDisplayStyle1 = OdDgDisplayStyleTableRecord::createObject();
  pDisplayStyle1->setName(L"NewDisplayStyle1");
  pDisplayStyle1->setApplyEdgeStyleToLineFlag(false);
  pDisplayStyle1->setBackgroundColor(5);
  pDisplayStyle1->setEnvironmentTypeDisplayed( OdDgDisplayStyleTableRecord::kBackgroundColor );
  pDisplayStyle1->setDisplayVisibleEdgesFlag(true);
  pDisplayStyle1->setDisplayHiddenEdgesFlag(true);
  pDisplayStyle1->setVisibleEdgeColor(3);
  pDisplayStyle1->setUseVisibleEdgeColorFlag(true);
  pDisplayStyle1->setUsableForViewsFlag(true);
  pDisplayStyle1->setDisplayMode( OdDgDisplayStyleTableRecord::kShaded );

  pDisplayStyleTable->add( pDisplayStyle1 );

  OdDgDisplayStyleTableRecordPtr pDisplayStyle2 = pDisplayStyle1->clone();
  pDisplayStyle2->setName(L"NewDisplayStyle2");
  pDisplayStyle2->setDisplayMode( OdDgDisplayStyleTableRecord::kWireframe );

  pDisplayStyleTable->add( pDisplayStyle2 );

  OdDgViewGroupPtr pVG = m_pModel3d->database()->getActiveViewGroupId().openObject(OdDg::kForWrite);

  OdDgElementIteratorPtr pViewIter = pVG->createIterator();
  pViewIter->step();
  pViewIter->step();

  OdDgViewPtr pView3 = pViewIter->item().openObject(OdDg::kForWrite);
  OdDgDisplayStyleViewPEPtr pDSView3PE = pView3;
  pDSView3PE->setDisplayStyle( pView3, pDisplayStyle1->elementId() );

  m_pModel3d->database()->disableUndoRecording(false);
  m_pModel3d->database()->startTransaction();
  m_pModel3d->database()->startUndoRecord();
  pDisplayStyle1->setElementColor(8);
  pDisplayStyle1->setElementTransparency(0.5);
  pDisplayStyle1->setUseElementTransparencyOverrideFlag(true);
  m_pModel3d->database()->endTransaction();
  m_pModel3d->database()->undo();

  OdDgDisplayStyleTableRecordPtr pDisplayStyle = OdDgDisplayStyleTableRecord::createObject();
  pDisplayStyle->setName(L"TerrainShadedDisplayStyle");
  pDisplayStyle->setApplyEdgeStyleToLineFlag(false);
  pDisplayStyle->setDisplayVisibleEdgesFlag(false);
  pDisplayStyle->setDisplayHiddenEdgesFlag(false);
  pDisplayStyle->setVisibleEdgeColor(3);
  pDisplayStyle->setUseVisibleEdgeColorFlag(false);
  pDisplayStyle->setUsableForViewsFlag(true);
  pDisplayStyle->setDisplayMode( OdDgDisplayStyleTableRecord::kShaded );
  pDisplayStyleTable->add( pDisplayStyle );
}

void OdExDgnFiller::addURLandFileLink( OdDgElementPtr& pElement )
{
  OdDgLinkNodePtr       pURLLinkToAdd     = new OdDgURLLinkNode();
  OdDgLinkNodePtr       pFileLinkToAdd    = new OdDgFileLinkNode();

  OdDgURLLinkNode*      pURLLink    = (OdDgURLLinkNode*)pURLLinkToAdd.get();
  OdDgFileLinkNode*     pFileLink   = (OdDgFileLinkNode*)pFileLinkToAdd.get();

  // Create URL link.

  pURLLink->setName(OD_T("Open Design Alliance"));
  pURLLink->setURL(OD_T("https://www.opendesign.com/"));
  pURLLink->setValidFlag(true);

  // Create simple file link.

  pFileLink->setName(OD_T("Attachment File"));
  pFileLink->setFileName(OD_T("attach.dgn"));
  pFileLink->setValidFlag(true);

  // Add Link Nodes. 

  OdDgLinkNodeElementPEPtr pLinkNodePE = pElement;
  pLinkNodePE->addLinkNode( pElement, pURLLinkToAdd );
  pLinkNodePE->addLinkNode( pElement, pFileLinkToAdd );
}

void OdExDgnFiller::addMaterialTableRecord()
{
  OdDgMaterialTablePtr pMatTable = m_pModel3d->database()->getMaterialTable(OdDg::kForWrite);

  OdGiMaterialColor diffuseColor;
  diffuseColor.setFactor( 0.6 );
  diffuseColor.setColor( OdCmEntityColor(255,0,0) );
  OdGiMaterialColor specularColor;
  specularColor.setFactor( 0.1 );
  specularColor.setColor( OdCmEntityColor(255,0,255) );

  OdDgMaterialMap diffuseMap;
  OdDgMaterialMap specularMap;

  // Create material 1
  OdDgMaterialTableRecordPtr pMat1 = OdDgMaterialTableRecord::createObject();

  pMat1->setName(L"Simple Mat 1");
  pMat1->setDiffuse(diffuseColor,diffuseMap);
  pMat1->setSpecular(specularColor, OdDgMaterialTableRecord::kCustom, 0.3, OdDgMaterialTableRecord::kGlossFactorCustom);

  pMatTable->add( pMat1 );

  // Create material 2
  OdDgMaterialTableRecordPtr pMat2 = OdDgMaterialTableRecord::createObject();

  pMat2->setName(L"Simple Mat 2");
  diffuseColor.setFactor( 0.4 );
  diffuseColor.setColor( OdCmEntityColor(255,255,255) );
  pMat2->setDiffuse(diffuseColor,diffuseMap);
  pMat2->setSpecular(specularColor, OdDgMaterialTableRecord::kPlastic, 0.1, OdDgMaterialTableRecord::kGlossFactorOff);

  pMatTable->add( pMat2 );

  // Create material 3
  OdDgMaterialTableRecordPtr pMat3 = OdDgMaterialTableRecord::createObject();

  pMat3->setName(L"Simple Mat 3");
  diffuseColor.setFactor( 0.95 );
  diffuseColor.setColor( OdCmEntityColor(0,0,255) );
  pMat3->setDiffuse(diffuseColor,diffuseMap);
  specularColor.setFactor( 0.35 );
  specularColor.setColor( OdCmEntityColor(0,255,255) );
  pMat3->setSpecular(specularColor, OdDgMaterialTableRecord::kMetallic, 0.8, OdDgMaterialTableRecord::kGlossFactorSameAsSpacular);

  pMatTable->add( pMat3 );

  // Create material 1
  OdDgMaterialTableRecordPtr pMatTerrain = OdDgMaterialTableRecord::createObject();

  pMatTerrain->setName(L"Terrain Material");

  diffuseColor.setFactor( 0.95 );
  diffuseColor.setColor( OdCmEntityColor(0,255,0) );

  OdGiMaterialMap matMap;
  matMap.setSourceFileName(L"Terrain.bmp");

  OdGiMapper matMapper;
  matMapper.setTransform( OdGeMatrix3d::translation( OdGeVector3d(-3.59,6.71,0.0))*OdGeMatrix3d::scaling(1.0/3.12) );
  matMap.setMapper( matMapper );

  diffuseMap.setTextureScaleMode( OdDgMaterialMap::kMasterUnitMode );
  diffuseMap.setMappingType( OdDgMaterialMap::kElevationDrape );
  diffuseMap.setGiMaterialMap( matMap );
  pMatTerrain->setDiffuse(diffuseColor,diffuseMap);

  specularColor.setFactor( 0.3 );
  specularColor.setColor( OdCmEntityColor(255,255,255) );

  pMatTerrain->setSpecular(specularColor, OdDgMaterialTableRecord::kCustom, 0.3, OdDgMaterialTableRecord::kGlossFactorSameAsSpacular);

  pMatTable->add( pMatTerrain );
}

void OdExDgnFiller::addMaterialV8TableRecord()
{
  OdDgMaterialTablePtr pMatTable = m_pModel3d->database()->getMaterialTable(OdDg::kForWrite);

  OdGiMaterialColor diffuseColor;
  diffuseColor.setFactor( 0.6 );
  diffuseColor.setColor( OdCmEntityColor(255,0,0) );
  OdGiMaterialColor specularColor;
  specularColor.setFactor( 0.1 );
  specularColor.setColor( OdCmEntityColor(255,0,255) );

  OdDgMaterialMap diffuseMap;

  // Create material for Microstation V8i SS3

  OdDgMaterialTableRecordPtr pMat = OdDgMaterialTableRecord::createObject();
  pMat->setName(L"Microstation V8i SS3 material");
  diffuseColor.setFactor( 0.16 );
  diffuseColor.setColor( OdCmEntityColor(16,128,255) );
  pMat->setDiffuse(diffuseColor,diffuseMap);
  specularColor.setFactor( 0.25 );
  specularColor.setColor( OdCmEntityColor(45,8,128) );
  pMat->setSpecular(specularColor, OdDgMaterialTableRecord::kPlastic, 0.4, OdDgMaterialTableRecord::kGlossFactorCustom);
  pMat->setVersion(8); // It's version of materials that used into Microstation V8i SS3

  pMat->addExtendedParam( OdDgMaterialParam(L"transparent_color.r", L"0.7") );
  pMat->addExtendedParam( OdDgMaterialParam(L"transparent_color.g", L"0.85") );
  pMat->addExtendedParam( OdDgMaterialParam(L"transparent_color.b", L"1.00000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"translucent_color.r", L"0.4230") );
  pMat->addExtendedParam( OdDgMaterialParam(L"translucent_color.g", L"0.73") );
  pMat->addExtendedParam( OdDgMaterialParam(L"translucent_color.b", L"0.12") );
  pMat->addExtendedParam( OdDgMaterialParam(L"glow_color.r", L"0.6") );
  pMat->addExtendedParam( OdDgMaterialParam(L"glow_color.g", L"0.6") );
  pMat->addExtendedParam( OdDgMaterialParam(L"glow_color.b", L"0.8") );
  pMat->addExtendedParam( OdDgMaterialParam(L"reflect_color.r", L"0.05") );
  pMat->addExtendedParam( OdDgMaterialParam(L"reflect_color.g", L"0.96") );
  pMat->addExtendedParam( OdDgMaterialParam(L"reflect_color.b", L"0.18") );
  pMat->addExtendedParam( OdDgMaterialParam(L"exit_color.r", L"0.25") );
  pMat->addExtendedParam( OdDgMaterialParam(L"exit_color.g", L"0.79") );
  pMat->addExtendedParam( OdDgMaterialParam(L"exit_color.b", L"1.0000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"reflection_rays", L"32") );
  pMat->addExtendedParam( OdDgMaterialParam(L"blur_reflections", L"0") );
  pMat->addExtendedParam( OdDgMaterialParam(L"blur_refractions", L"0") );
  pMat->addExtendedParam( OdDgMaterialParam(L"refraction_rays", L"32") );
  pMat->addExtendedParam( OdDgMaterialParam(L"subsurface_samples", L"32") );
  pMat->addExtendedParam( OdDgMaterialParam(L"back_face_culling", L"0") );
  pMat->addExtendedParam( OdDgMaterialParam(L"use_fur", L"0") );
  pMat->addExtendedParam( OdDgMaterialParam(L"cut_section_use_material", L"0") );
  pMat->addExtendedParam( OdDgMaterialParam(L"displacement_distance", L"1.0000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"dispersion", L"0.1000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"clearcoat", L"0.0000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"anisotropy", L"0.75000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"front_weighting", L"70.0000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"scattering_distance", L"0.5000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"reflect_fresnel", L"0.0000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"dissolve", L"0.0000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"absorption_distance", L"0.8000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"refraction_roughness", L"7.5000") );
  pMat->addExtendedParam( OdDgMaterialParam(L"CutSectionMaterialName", L"") );
  pMat->addExtendedParam( OdDgMaterialParam(L"CutSectionMaterialPalette", L"") );

  OdDgMaterialParam matParam(L"Shader",L"",true);
  matParam.addChild( OdDgMaterialParam(L"Flags", L"253") );
  matParam.addChild( OdDgMaterialParam(L"shader_type", L"0") );
  matParam.addChild( OdDgMaterialParam(L"shader_effect", L"0") );
  matParam.addChild( OdDgMaterialParam(L"shader_blend", L"1") );
  matParam.addChild( OdDgMaterialParam(L"shader_indirect_illum_type", L"2") );
  matParam.addChild( OdDgMaterialParam(L"shader_rate", L"1.1000") );
  matParam.addChild( OdDgMaterialParam(L"shader_direct_mult", L"95.0000") );
  matParam.addChild( OdDgMaterialParam(L"shader_indirect_mult", L"150.0000") );
  matParam.addChild( OdDgMaterialParam(L"shader_saturation", L"125.0000") );
  matParam.addChild( OdDgMaterialParam(L"shader_opacity", L"75.0000") );

  pMat->addExtendedParam( matParam );

  // create diffuse pattern

  OdDgMaterialPatternPtr pDiffusePattern = OdDgMaterialPattern::createObject();

  pDiffusePattern->setType( OdDgMaterialPattern::kDiffusePattern );
  pDiffusePattern->setMappingType( OdDgMaterialMap::kPlanar );
  pDiffusePattern->setScaleMode( OdDgMaterialMap::kMasterUnitMode );
  pDiffusePattern->setPatternWeight(1.0);
  pDiffusePattern->setFileName(L"layers.pma");
  pDiffusePattern->setUsePattern(true);

  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"enable_antialiasing", L"0") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"texture_filter_type", L"1") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"pattern_proj_offset.x", L"12.5") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"pattern_proj_offset.y", L"0.35") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"pattern_proj_offset.z", L"18.45") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"layer", L"0") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"offset_z", L"0.0000") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"scale_z", L"1.0000") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"image_gamma", L"0.85") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"low_value", L"0.0000") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"high_value", L"100.0000") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"antialias_strength", L"50.0000") );
  pDiffusePattern->addExtendedParam( OdDgMaterialParam(L"minimum_spot", L"1.5000") );

  pMat->addMaterialPattern( pDiffusePattern );

  // Add layer 1

  OdDgMaterialLayerPatternPtr pMatLayer1 = OdDgMaterialLayerPattern::createObject();

  pMatLayer1->setLayerTypeStr(L"layer 8119LXOPROCEDURE C:/ProgramData/Bentley/MicroStation V8i (SELECTseries)/WorkSpace/System/materials/Patterns/lxoturbulence.jpg");
  pMatLayer1->setMappingType( OdDgMaterialMap::kPlanar);
  pMatLayer1->setScaleMode(OdDgMaterialMap::kMasterUnitMode);
  pMatLayer1->setVisibleLayerFlag(true);

  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"texture_filter_type", L"1") );
  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"pattern_opacity", L"1.0000") );
  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"pattern_opacity", L"1.0000") );
  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"offset_z", L"0.0000") );
  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"scale_z", L"1.0000") );
  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"image_gamma", L"0.95") );
  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"low_value", L"0.0000") );
  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"high_value", L"100.0000") );
  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"antialias_strength", L"100.0000") );
  pMatLayer1->addExtendedParam( OdDgMaterialParam(L"minimum_spot", L"2.5000") );

  OdDgMaterialParam layer1Param(L"LxoProcedure",L"",true);

  layer1Param.addChild( OdDgMaterialParam(L"LxoProcedureType", L"24"));
  layer1Param.addChild( OdDgMaterialParam(L"Complexity", L"6.0000"));

  pMatLayer1->addExtendedParam( layer1Param);

  pDiffusePattern->addPatternLayer( pMatLayer1 );

  // Add layer 2

  OdDgMaterialLayerPatternPtr pMatLayer2 = OdDgMaterialLayerPattern::createObject();

  pMatLayer2->setLayerTypeStr(L"layer NORMAL");
  pMatLayer2->setMappingType( OdDgMaterialMap::kParametric);
  pMatLayer2->setScaleMode(OdDgMaterialMap::kMasterUnitMode);
  pMatLayer2->setVisibleLayerFlag(false);

  pDiffusePattern->addPatternLayer( pMatLayer2 );

  // Add layer 3

  OdDgMaterialLayerPatternPtr pMatLayer3 = OdDgMaterialLayerPattern::createObject();

  pMatLayer3->setLayerTypeStr(L"layer 8119LXOPROCEDURE C:/ProgramData/Bentley/MicroStation V8i (SELECTseries)/WorkSpace/System/materials/Patterns/lxostone.jpg");
  pMatLayer3->setMappingType( OdDgMaterialMap::kPlanar);
  pMatLayer3->setScaleMode(OdDgMaterialMap::kMasterUnitMode);
  pMatLayer3->setLayerFlags(1);
  pMatLayer3->setDataFlags(512);
  pMatLayer3->setVisibleLayerFlag(true);

  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"texture_filter_type", L"1") );
  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"pattern_opacity", L"0.3333") );
  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"pattern_opacity", L"1.0000") );
  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"offset_z", L"0.0000") );
  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"scale_z", L"1.0000") );
  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"image_gamma", L"1.00") );
  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"low_value", L"0.0000") );
  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"high_value", L"100.0000") );
  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"antialias_strength", L"100.0000") );
  pMatLayer3->addExtendedParam( OdDgMaterialParam(L"minimum_spot", L"2.5000") );

  OdDgMaterialParam layer3Param(L"LxoProcedure",L"",true);

  layer3Param.addChild( OdDgMaterialParam(L"LxoProcedureType", L"23"));
  layer3Param.addChild( OdDgMaterialParam(L"TintColor.R", L"0.7490"));
  layer3Param.addChild( OdDgMaterialParam(L"TintColor.G", L"0.7490"));
  layer3Param.addChild( OdDgMaterialParam(L"TintColor.B", L"0.7490"));
  layer3Param.addChild( OdDgMaterialParam(L"MortarColor.R", L"0.9020"));
  layer3Param.addChild( OdDgMaterialParam(L"MortarColor.G", L"0.9020"));
  layer3Param.addChild( OdDgMaterialParam(L"MortarColor.B", L"0.9020"));
  layer3Param.addChild( OdDgMaterialParam(L"StoneColors", L"6"));
  layer3Param.addChild( OdDgMaterialParam(L"StoneColorOffset", L"0"));
  layer3Param.addChild( OdDgMaterialParam(L"MortarWidth", L"10.0000"));
  layer3Param.addChild( OdDgMaterialParam(L"Noisiness", L"30.0000"));

  pMatLayer3->addExtendedParam( layer3Param);

  pDiffusePattern->addPatternLayer( pMatLayer3 );

  // Add layer 4

  OdDgMaterialLayerPatternPtr pMatLayer4 = OdDgMaterialLayerPattern::createObject();

  pMatLayer4->setLayerTypeStr(L"layer NORMAL");
  pMatLayer4->setMappingType( OdDgMaterialMap::kParametric);
  pMatLayer4->setScaleMode(OdDgMaterialMap::kMasterUnitMode);
  pMatLayer4->setLayerFlags(1);
  pMatLayer4->setVisibleLayerFlag(true);

  pDiffusePattern->addPatternLayer( pMatLayer4 );

  OdDgMaterialPatternPtr pTransColorPattern = OdDgMaterialPattern::createObject( pDiffusePattern );
  pTransColorPattern->setType( OdDgMaterialPattern::kTransparentColorPattern );

  pMat->addMaterialPattern( pTransColorPattern );

  OdDgMaterialPatternPtr pSubSurfacePattern = OdDgMaterialPattern::createObject( pDiffusePattern );
  pSubSurfacePattern->setType( OdDgMaterialPattern::kSubSurfaceColorPattern );

  OdDgMaterialPatternPtr pGlowColorPattern = OdDgMaterialPattern::createObject( pDiffusePattern );
  pGlowColorPattern->setType( OdDgMaterialPattern::kGlowColorPattern );

  pMat->addMaterialPattern( pGlowColorPattern );

  pMat->addMaterialPattern( pSubSurfacePattern );

  pMatTable->add( pMat );
}

void OdExDgnFiller::addTextWithVolumeClipping( int boxRow, int boxCol )
{
  double minorRadius = m_EntityBoxes.getHeight();
  double majorRadius = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxOrigin3d = m_EntityBoxes.getBoxCenter( boxRow, boxCol ) - OdGeVector3d( majorRadius/2, minorRadius/2, 0. );

  OdDgText3dPtr pText3d;
  OdGeQuaternion rotation( -1., 0., 0., 0. );

  pText3d = OdDgText3d::createObject();

  pText3d->setJustification( OdDg::kCenterCenter );
  pText3d->setText( OdString( "Volume clipping" ) );
  pText3d->setOrigin( OdGePoint3d( boxOrigin3d.x + majorRadius/7, boxOrigin3d.y + minorRadius/2 + minorRadius/8, 0 ));
  pText3d->setRotation( rotation );
  pText3d->setHeightMultiplier(0.25);
  pText3d->setLengthMultiplier(0.25);

  m_pModel3d->addElement( pText3d );

  pText3d = OdDgText3d::createObject();

  pText3d->setJustification( OdDg::kCenterCenter );
  pText3d->setText( OdString( "Disabled" ) );
  pText3d->setOrigin( OdGePoint3d( boxOrigin3d.x + majorRadius/3, boxOrigin3d.y + minorRadius/4 + minorRadius/8, 0 ));
  pText3d->setRotation( rotation );
  pText3d->setHeightMultiplier(0.25);
  pText3d->setLengthMultiplier(0.25);

  m_pModel3d->addElement( pText3d );

  // Element for volume clipping

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();

  pShape->addVertex( OdGePoint3d(boxOrigin3d.x + majorRadius/3.5 - 0.2, boxOrigin3d.y + minorRadius*3/8.5 - 0.2, 0) );
  pShape->addVertex( OdGePoint3d(boxOrigin3d.x + majorRadius/3.5 - 0.2, boxOrigin3d.y + minorRadius*3/8.5 + 0.58, 0) );
  pShape->addVertex( OdGePoint3d(boxOrigin3d.x + majorRadius/3.5 + majorRadius/2.2 + 0.2, boxOrigin3d.y + minorRadius*3/8.5 + 0.58, 0) );
  pShape->addVertex( OdGePoint3d(boxOrigin3d.x + majorRadius/3.5 + majorRadius/2.2 + 0.2, boxOrigin3d.y + minorRadius*3/8.5 - 0.2, 0) );
  pShape->addVertex( OdGePoint3d(boxOrigin3d.x + majorRadius/3.5 - 0.2, boxOrigin3d.y + minorRadius*3/8.5 - 0.2, 0) );

  m_pModel3d->addElement( pShape );

  // Add volume clipping to view number 0 of first 3d model.

  OdDgDatabase* pDb = m_pModel3d->database();

  if( pDb )
  {
    OdDgViewGroupTablePtr pViewGroupTable = pDb->getViewGroupTable( OdDg::kForWrite );

    if( !pViewGroupTable.isNull() )
    {
      OdDgElementIteratorPtr pViewGroupIter = pViewGroupTable->createIterator();

      OdDgViewGroupPtr pViewGroup = pViewGroupIter->item().openObject(OdDg::kForWrite);

      if( !pViewGroup.isNull() )
      {
        OdDgElementIteratorPtr pViewIter = pViewGroup->createIterator();

        OdDgViewPtr pView = pViewIter->item().openObject( OdDg::kForWrite );

        if( !pView.isNull() )
        {
          pView->setVolumeClipMaskElementId( pShape->id() );
          pView->setShowClipVolumeFlag(true);
        }
      }
    }
  }
}

void OdExDgnFiller::addEllipse(int boxRow, int boxCol)
{
  // adds 2d ellipse
  //
  double minorRadius = m_EntityBoxes.getHeight() / 8;
  double majorRadius = m_EntityBoxes.getWidth(boxRow, boxCol) / 8;
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgEllipse2dPtr pEllipse = OdDgEllipse2d::createObject();
  pEllipse->setOrigin( OdGePoint2d( boxCenter.x + majorRadius, boxCenter.y * 1.0 ) );
  pEllipse->setPrimaryAxis( majorRadius );
  pEllipse->setSecondaryAxis( minorRadius );
  pEllipse->setRotationAngle( OdaPI4 );
  m_pModel2d->addElement( pEllipse );

  pEllipse->setColor(ODRGB( 249, 148, 146 ));

  OdDgEllipse2dPtr pEllipse1 = OdDgEllipse2d::createObject();
  pEllipse1->setOrigin( OdGePoint2d( boxCenter.x - majorRadius, boxCenter.y * 1.0 ) );
  pEllipse1->setPrimaryAxis( majorRadius );
  pEllipse1->setSecondaryAxis( minorRadius );
  pEllipse1->setRotationAngle( OdaPI4/2 );

  OdDgCmColor color;
  OdUInt32 colorIndex;
  colorIndex = OdDgColorTable::getColorIndexByRGB( m_pModel2d->database(), ODRGB(148, 152, 77) );
  color.setColorIndex(colorIndex);

  m_pModel2d->addElement( pEllipse1 );

  pEllipse1->setColor(color, false);
  ODCOLORREF color1 = pEllipse1->getColor();
  OdUInt32 ind1 = pEllipse1->getColorIndex();
  if (colorIndex != ind1)
  {
    ODA_ASSERT(false);
  }
  ODA_ASSERT( color1 == ODRGB(148, 152, 77));
  ODA_ASSERT (ind1 == OdDgColorTable::getColorIndexByRGB( m_pModel2d->database(), color1 ));

  // adds 3d ellipse
  //
  minorRadius = m_EntityBoxes.getHeight() / 8;
  majorRadius = m_EntityBoxes.getWidth(boxRow, boxCol) / 8;

  OdDgEllipse3dPtr pEllipse3d = OdDgEllipse3d::createObject();
  pEllipse3d->setOrigin( OdGePoint3d( boxCenter.x - majorRadius, boxCenter.y * 1.0, boxCenter.z ) );
  pEllipse3d->setPrimaryAxis( majorRadius );
  pEllipse3d->setSecondaryAxis( minorRadius );
  OdGeMatrix3d matrix;
  OdGeQuaternion quat;
  pEllipse3d->setRotation( quat.set( matrix.setToRotation( OdaPI4/2, OdGeVector3d::kZAxis )));

  m_pModel3d->addElement( pEllipse3d );
}


void OdExDgnFiller::addArc(int boxRow, int boxCol)
{
  // adds 2d arc
  //
  double minorRadius = m_EntityBoxes.getHeight() / 8;
  double majorRadius = m_EntityBoxes.getWidth(boxRow, boxCol) / 8;
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  pArc->setOrigin( OdGePoint2d( boxCenter.x + majorRadius, boxCenter.y / 1 ) );
  pArc->setPrimaryAxis( majorRadius );
  pArc->setSecondaryAxis( minorRadius );
  pArc->setRotationAngle( OdaPI4 / 4 );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel2d->addElement( pArc );

  // adds 3d arc
  //
  minorRadius = m_EntityBoxes.getHeight() / 4;
  majorRadius = m_EntityBoxes.getWidth(boxRow, boxCol) / 4;

  OdDgArc3dPtr pArc3d = OdDgArc3d::createObject();
  pArc3d->setOrigin( OdGePoint3d( boxCenter.x - majorRadius, boxCenter.y, boxCenter.z ) );
  pArc3d->setPrimaryAxis( majorRadius );
  pArc3d->setSecondaryAxis( minorRadius );
  pArc3d->setStartAngle( OdaPI * .1 );
  pArc3d->setSweepAngle( OdaPI * .4 );
  m_pModel3d->addElement( pArc3d );
}

void OdExDgnFiller::addCone(int boxRow, int boxCol)
{
  // adds Cone
  //
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgConePtr pCone = OdDgCone::createObject();
/*
Is solid = yes
Radius 1 = 0.323791
Radius 2 = 0.725507
Center 1 = ( 2.9033; 1.20283; 0 )
Center 2 = ( 2.35958; 1.91472; 0 )
Rotation = ( -0.22654; 0.669835; -0.669835; -0.22654 )
*/

  pCone->setCenter1( OdGePoint3d( boxCenter.x + (major/4), boxCenter.y + (minor/8), boxCenter.z ));
  pCone->setCenter2( OdGePoint3d( boxCenter.x - (major/5), boxCenter.y - (minor/8), boxCenter.z ));
  pCone->setRadius1( m_EntityBoxes.getWidth(boxRow, boxCol) / 10 );
  pCone->setRadius2( m_EntityBoxes.getWidth(boxRow, boxCol) / 5 );

  {
    OdGeMatrix3d    matrix;
    OdGeQuaternion  quaternion;

    matrix.setToRotation( OdaPI4/2, OdGeVector3d::kZAxis );
    quaternion.set( matrix );

    pCone->setRotation( quaternion );
  }

  pCone->setSolid();

  m_pModel3d->addElement( pCone );

  m_uNamedGroupElementArr.push_back( (OdUInt64)(pCone->elementId().getHandle()) );

}


void OdExDgnFiller::addLines( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d boxCenter3d = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );
  OdGePoint2d boxCenter2d( boxCenter3d.x, boxCenter3d.y );

  OdDgLine3dPtr pLine3d;

  OdDgLineStyleTablePtr pLineStyleTable = m_pModel2d->database()->getLineStyleTable(OdDg::kForRead);

  OdDgElementId idLineStyleRR = pLineStyleTable->getAt(OD_T("{ Rail Road }"));

  //Line2d
  {
    OdDgLine2dPtr pLine2d;

    pLine2d = OdDgLine2d::createObject();
    pLine2d->setStartPoint( boxCenter2d + OdGeVector2d( - major / 3., - minor / 3. ) );
    pLine2d->setEndPoint( boxCenter2d + OdGeVector2d(   major / 3., - minor / 3. ) );
    pLine2d->setLineStyleEntryId(4);
    m_pModel2d->addElement( pLine2d );

    pLine2d = OdDgLine2d::createObject();
    pLine2d->setStartPoint( boxCenter2d + OdGeVector2d( - major / 3., + minor / 3. ) );
    pLine2d->setEndPoint( boxCenter2d + OdGeVector2d(   major / 3., + minor / 3. ) );
    pLine2d->setLineStyleId(idLineStyleRR);
    pLine2d->setLineStyleScale(0.05);
    m_pModel2d->addElement( pLine2d );
  }

  //Line3d
  {
    OdDgLine3dPtr pLine3d;
    OdGeVector3d  pseudoX( major / 3., minor / 3., 0. ), pseudoY( - major / 3., minor / 3., 0. ),
                  stepZ( 0., 0., minor / 5. );
    double        i, angle;

    for( i = 0; i < 2.; i += .1 )
    {
      angle = i * OdaPI;
      pLine3d = OdDgLine3d::createObject();

      pLine3d->setStartPoint( boxCenter3d + pseudoX * cos( angle ) + pseudoY * sin( angle ) + stepZ * i );
      pLine3d->setEndPoint( boxCenter3d + ( pseudoX * cos( angle ) + pseudoY * sin( angle ) ) * .1 );
      m_pModel3d->addElement( pLine3d );
    }
  }
}

void OdExDgnFiller::addTextWithJustifucation( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d boxOrigin3d = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdGeQuaternion rotation;
  OdGeMatrix3d matRotation = OdGeMatrix3d::rotation(OdaPI/16, OdGeVector3d::kZAxis);
  rotation.set( matRotation );

  OdDgFontTablePtr pFontTable = m_pModel3d->database()->getFontTable(OdDg::kForRead);

  OdDgElementId idEngFont = pFontTable->getAt(L"ENGINEERING");

  OdUInt32 uFontEntryId = 0;

  if( !idEngFont.isNull() )
  {
    OdDgFontTableRecordPtr pEngFont = idEngFont.openObject(OdDg::kForRead);
    uFontEntryId = pEngFont->getNumber();
  }

  OdDgText3dPtr pText3d;

  pText3d = OdDgText3d::createObject();
  pText3d->setText( OdString( "[TextSample]" ) );
  pText3d->setFontEntryId(uFontEntryId);
  pText3d->setHeightMultiplier(0.2);
  pText3d->setLengthMultiplier(0.2);
  pText3d->setColorIndex(2);
  m_pModel3d->addElement( pText3d );

  // get extents.

  pText3d->setOrigin(OdGePoint3d(0,0,0));
  OdGeExtents3d extText;
  pText3d->getGeomExtents( extText );

  double dTextWidth  = extText.maxPoint().x;
  double dTextHeight = extText.maxPoint().y;

  pText3d->setRotation( rotation );

  OdGeVector3d vrTextDir = OdGeVector3d::kXAxis;
  OdGeVector3d vrTextYDir = OdGeVector3d::kYAxis;

  vrTextDir = rotation.rotate(vrTextDir);
  vrTextYDir = rotation.rotate(vrTextYDir);

  // create text with left - top justification 

  OdGePoint3d ptPos( boxOrigin3d.x, boxOrigin3d.y + minor/4.0,0 );

  pText3d->setJustification( OdDg::kLeftTop );
  pText3d->setPosition( ptPos );

  OdDgLineString3dPtr pTopLeftExtents = OdDgLineString3d::createObject();

  pTopLeftExtents->addVertex( ptPos );
  pTopLeftExtents->addVertex( ptPos + vrTextDir * dTextWidth);
  pTopLeftExtents->addVertex( ptPos + vrTextDir * dTextWidth - vrTextYDir * dTextHeight );
  pTopLeftExtents->addVertex( ptPos - vrTextYDir * dTextHeight );
  pTopLeftExtents->addVertex( ptPos );
  pTopLeftExtents->setColorIndex(3);
  pTopLeftExtents->setLineStyleEntryId(1);

  m_pModel3d->addElement( pTopLeftExtents );

  OdDgEllipse3dPtr pPositionMarker = OdDgEllipse3d::createObject();

  pPositionMarker->setPrimaryAxis( 0.03 );
  pPositionMarker->setSecondaryAxis( 0.03 );
  pPositionMarker->setOrigin( ptPos );
  pPositionMarker->setColorIndex( 3 );

  m_pModel3d->addElement( pPositionMarker );

  // create text with center - center justification 

  pText3d = OdDgText3d::createObject();
  pText3d->setText( OdString( "[TextSample]" ) );
  pText3d->setFontEntryId(uFontEntryId);
  pText3d->setHeightMultiplier(0.2);
  pText3d->setLengthMultiplier(0.2);
  pText3d->setColorIndex(4);
  pText3d->setRotation( rotation );
  m_pModel3d->addElement( pText3d );

  ptPos.set( boxOrigin3d.x, boxOrigin3d.y,0 );

  pText3d->setJustification( OdDg::kCenterCenter );
  pText3d->setPosition( ptPos );

  pTopLeftExtents = OdDgLineString3d::createObject();

  pTopLeftExtents->addVertex( ptPos - vrTextDir * dTextWidth/2.0 - vrTextYDir * dTextHeight / 2.0);
  pTopLeftExtents->addVertex( ptPos - vrTextDir * dTextWidth/2.0 + vrTextYDir * dTextHeight / 2.0);
  pTopLeftExtents->addVertex( ptPos + vrTextDir * dTextWidth/2.0 + vrTextYDir * dTextHeight / 2.0);
  pTopLeftExtents->addVertex( ptPos + vrTextDir * dTextWidth/2.0 - vrTextYDir * dTextHeight / 2.0);
  pTopLeftExtents->addVertex( ptPos - vrTextDir * dTextWidth/2.0 - vrTextYDir * dTextHeight / 2.0);
  pTopLeftExtents->setColorIndex(3);
  pTopLeftExtents->setLineStyleEntryId(1);

  m_pModel3d->addElement( pTopLeftExtents );

  pPositionMarker = OdDgEllipse3d::createObject();

  pPositionMarker->setPrimaryAxis( 0.03 );
  pPositionMarker->setSecondaryAxis( 0.03 );
  pPositionMarker->setOrigin( ptPos );
  pPositionMarker->setColorIndex( 3 );

  m_pModel3d->addElement( pPositionMarker );

  // create text with right - bottom justification 

  pText3d = OdDgText3d::createObject();
  pText3d->setText( OdString( "[TextSample]" ) );
  pText3d->setFontEntryId(uFontEntryId);
  pText3d->setHeightMultiplier(0.2);
  pText3d->setLengthMultiplier(0.2);
  pText3d->setColorIndex(1);
  pText3d->setRotation( rotation );
  m_pModel3d->addElement( pText3d );

  ptPos.set( boxOrigin3d.x, boxOrigin3d.y - minor/4.0,0 );

  pText3d->setJustification( OdDg::kRightBottom );
  pText3d->setPosition( ptPos );

  pTopLeftExtents = OdDgLineString3d::createObject();

  pTopLeftExtents->addVertex( ptPos );
  pTopLeftExtents->addVertex( ptPos - vrTextDir * dTextWidth);
  pTopLeftExtents->addVertex( ptPos - vrTextDir * dTextWidth + vrTextYDir * dTextHeight );
  pTopLeftExtents->addVertex( ptPos + vrTextYDir * dTextHeight );
  pTopLeftExtents->addVertex( ptPos );
  pTopLeftExtents->setColorIndex(3);
  pTopLeftExtents->setLineStyleEntryId(1);

  m_pModel3d->addElement( pTopLeftExtents );

  pPositionMarker = OdDgEllipse3d::createObject();

  pPositionMarker->setPrimaryAxis( 0.03 );
  pPositionMarker->setSecondaryAxis( 0.03 );
  pPositionMarker->setOrigin( ptPos );
  pPositionMarker->setColorIndex( 3 );

  m_pModel3d->addElement( pPositionMarker );
}


void OdExDgnFiller::addText( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d boxOrigin3d = m_EntityBoxes.getBoxCenter( boxRow, boxColumn ) - OdGeVector3d( major/2, minor/2, 0. );

  OdDgElementId idLevel = m_pModel3d->database()->getLevelTable()->getAt("ExDgnCreate level 1");

  //3D with default Font
  {
    OdDgText3dPtr pText3d;
    OdGeQuaternion rotation( -1., 0., 0., 0. );

    pText3d = OdDgText3d::createObject();

    pText3d->setJustification( OdDg::kCenterCenter );
    pText3d->setText( OdString( "Default font" ) );
    pText3d->setOrigin( OdGePoint3d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/2 + minor/8, 0 ));
    pText3d->setRotation( rotation );
    pText3d->setHeightMultiplier(0.5);
    pText3d->setLengthMultiplier(0.5);

    //if (!idLevel.isNull())
    //{
    //  pText3d->setLevelId( idLevel );
    //}
   
    m_pModel3d->addElement( pText3d );

    m_uNamedGroupElementArr.push_back( (OdUInt64)(pText3d->elementId().getHandle()) );
  }
  //3D with Font
  {
    OdDgText3dPtr pText3d;

    pText3d = OdDgText3d::createObject();
    OdGeQuaternion rotation( -1., 0., 0., 0. );

    pText3d->setJustification( OdDg::kCenterCenter );
    pText3d->setText( OdString( "ARIAL font" ) );

    OdDgFontTablePtr pFontTable = m_pModel3d->database()->getFontTable();
    OdDgFontTableRecordPtr pFont = pFontTable->getFont("Arial");
    if (!pFont.isNull())
    {
      pText3d->setFontEntryId(pFont->getNumber());
    }

    pText3d->setOrigin( OdGePoint3d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/8, 0 ));
    pText3d->setRotation( rotation );
    pText3d->setHeightMultiplier(0.75);
    pText3d->setLengthMultiplier(0.75);
    pText3d->setColorIndex(3);
    pText3d->setSlant( OdaPI / 5 );

    //if (!idLevel.isNull())
    //{
    //  pText3d->setLevelId( idLevel );
    //}

    m_pModel3d->addElement( pText3d );

    // test justification
    //
    OdDgText3dPtr pText3d_CB;

    pText3d_CB = OdDgText3d::createObject();

    pText3d_CB->setJustification( OdDg::kCenterBottom );
    pText3d_CB->setText( OdString( "ARIAL font" ) );

    if (!pFont.isNull())
    {
      pText3d_CB->setFontEntryId(pFont->getNumber());
    }

    pText3d_CB->setOrigin( OdGePoint3d( boxOrigin3d.x + major/20, boxOrigin3d.y + 2* minor/8, 0 ));
    pText3d_CB->setRotation( rotation );
    pText3d_CB->setHeightMultiplier(0.75);
    pText3d_CB->setLengthMultiplier(0.75);
    pText3d_CB->setColorIndex(1);
    pText3d_CB->setSlant( OdaPI / 5 );

    //if (!idLevel.isNull())
    //{
    //  pText3d->setLevelId( idLevel );
    //}

    m_pModel3d->addElement( pText3d_CB );

    OdGePoint3d ptPosition1 =  pText3d->getPosition();
    OdGePoint3d ptOrigin1 = pText3d->getOrigin();

    OdDgLine3dPtr pLine3d;
    pLine3d = OdDgLine3d::createObject();
    pLine3d->setStartPoint( ptPosition1 );
    pLine3d->setEndPoint( ptOrigin1 );
    pLine3d->setColorIndex(2);
    m_pModel3d->addElement( pLine3d );
    ODA_ASSERT(ptOrigin1 == OdGePoint3d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/8, 0 ));

    OdGePoint3d ptPosition2 =  pText3d_CB->getPosition();
    OdGePoint3d ptOrigin2 = pText3d_CB->getOrigin();

    OdDgLine3dPtr pLine3d_2;
    pLine3d_2 = OdDgLine3d::createObject();
    pLine3d_2->setStartPoint( ptPosition2 );
    pLine3d_2->setEndPoint( ptOrigin2 );
    pLine3d_2->setColorIndex(4);
    m_pModel3d->addElement( pLine3d_2 );
  }

  //3D
  boxColumn++;
  major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  boxOrigin3d = m_EntityBoxes.getBoxCenter( boxRow, boxColumn ) - OdGeVector3d( major/2, minor/2, 0. );
  {
    OdDgText3dPtr pText3d;
    OdGeQuaternion rotation( -1., 0., 0., 0. );

    pText3d = OdDgText3d::createObject();
    pText3d->setJustification( OdDg::kCenterCenter );
    pText3d->setText( OdString( "Verdana font" ) );

    OdDgFontTablePtr pFontTable = m_pModel3d->database()->getFontTable();
    OdDgFontTableRecordPtr pFont = pFontTable->getFont("Verdana");
    if (!pFont.isNull())
    {
      pText3d->setFontEntryId(pFont->getNumber());
    }

    pText3d->setOrigin( OdGePoint3d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/2 + minor/8, 0 ));
    pText3d->setRotation( rotation );
    pText3d->setHeightMultiplier(0.8);
    pText3d->setLengthMultiplier(0.8);
    
    m_pModel3d->addElement( pText3d );
  }

  {
    OdDgText3dPtr pText3d;

    pText3d = OdDgText3d::createObject();
    OdGeQuaternion rotation( -1., 0., 0., 0. );

    pText3d->setJustification( OdDg::kCenterCenter );
    pText3d->setText( OdString( "Verdana (TextStyle1)" ) );

    //OdDgFontTablePtr pFontTable = m_pModel3d->database()->getFontTable();
    //OdDgFontTableRecordPtr pFont = pFontTable->getFont("Arial");
    //if (!pFont.isNull())
    //{
    //  pText3d->setFontEntryId(pFont->getNumber());
    //}
    OdDgTextStyleTablePtr pTextStyleTable = m_pModel3d->database()->getTextStyleTable();
    OdDgElementId id = pTextStyleTable->getAt("TextStyle1");

    // Firstly to add a text element to model;
    //  Next to apply TextStyle properties.
    m_pModel3d->addElement( pText3d );

    //if (!id.isNull())
    //{
    //  OdDgTextStyleTableRecordPtr pTextStyle = id.safeOpenObject();
    //  pText3d->setTextStyleEntryId( pTextStyle->getEntryId() );
    //}
    if (!id.isNull())
    {
      pText3d->applyTextStyle(id);
    }
 
    pText3d->setOrigin( OdGePoint3d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/8, 0 ));
    pText3d->setRotation( rotation );
    pText3d->setHeightMultiplier(0.35);
    pText3d->setLengthMultiplier(0.35);
    pText3d->setColorIndex(6);
    
    //m_pModel3d->addElement( pText3d );
  }

  // Apply TextStyle
  {
    OdDgText3dPtr pText3d;
    pText3d = OdDgText3d::createObject();
    m_pModel3d->addElement( pText3d );

    OdGeQuaternion rotation( -1., 0., 0., 0. );

    pText3d->setJustification( OdDg::kCenterCenter );
    pText3d->setText( OdString( "TextStyle2" ) );

    OdDgTextStyleTablePtr pTextStyleTable = m_pModel3d->database()->getTextStyleTable();
    OdDgElementId id = pTextStyleTable->getAt("TextStyle2");
    if (!id.isNull())
    {
      // applyTextStyle() should be called after adding element to model
      pText3d->applyTextStyle(id);
    }
    pText3d->setOrigin( OdGePoint3d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/6, 0 ));
    pText3d->setRotation( rotation );
  }

  {
    OdDgText3dPtr pText3d = OdDgText3d::createObject();
    m_pModel3d->addElement( pText3d );

    OdDgTextStyleTablePtr pTextStyleTable = m_pModel3d->database()->getTextStyleTable();
    OdDgElementId id = pTextStyleTable->getAt("TextStyle3");
    if (!id.isNull())
    {
      // applyTextStyle() should be called after adding element to model
      pText3d->applyTextStyle(id);
    }

    OdGeQuaternion rotation( -1., 0., 0., 0. );

    pText3d->setText( OdString( "Apply TextStyle3" ) );

    pText3d->setOrigin( OdGePoint3d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/3, 0 ));
    pText3d->setRotation( rotation );
    //pText3d->setColorIndex(9);
    pText3d->setColorIndex(8);
  }

  //2D with ttf
  {
    OdDgText2dPtr pText2d;

    pText2d = OdDgText2d::createObject();
    //OdGeQuaternion rotation( -1., 0., 0., 0. );

    pText2d->setJustification( OdDg::kCenterCenter );
    pText2d->setText( OdString( "ARIAL font" ) );

    OdDgFontTablePtr pFontTable = m_pModel2d->database()->getFontTable();
    OdDgFontTableRecordPtr pFont = pFontTable->getFont("Arial");
    if (!pFont.isNull())
    {
      pText2d->setFontEntryId(pFont->getNumber());
    }

    pText2d->setOrigin( OdGePoint2d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/8 ));
    pText2d->setHeightMultiplier(0.75);
    pText2d->setLengthMultiplier(0.75);
    pText2d->setColorIndex(3);

    pText2d->setInterCharSpacingFlag( true );
    pText2d->setSpacing( 1.0, OdDgTextExtendedProperties::sInterCharSpacing );

    m_pModel2d->addElement( pText2d );
  }

  // TextStyle has an RSC fonts with a specific number without any conditions. Actually that RSC font is not loaded
  {
    OdDgText2dPtr pText2d = OdDgText2d::createObject();

    pText2d->setJustification( OdDg::kCenterCenter );
    pText2d->setText( OdString( "TextStyleRSC_245" ) );
    OdDgTextStyleTablePtr pTextStyleTable = m_pModel2d->database()->getTextStyleTable();
    OdDgElementId id = pTextStyleTable->getAt("TextStyleRSC_245");

    // Firstly to add a text element to model;
    //  Next to apply TextStyle properties.
    m_pModel2d->addElement( pText2d );

    if (!id.isNull())
    {
      pText2d->applyTextStyle(id);
    }
 
    pText2d->setOrigin( OdGePoint2d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/8 ));
    //pText2d->setRotation( rotation );
    //pText2d->setHeightMultiplier(0.35);
    //pText2d->setLengthMultiplier(0.35);
    //pText2d->setColorIndex(6);
  }
  {
    OdDgText2dPtr pText2d;

    pText2d = OdDgText2d::createObject();
    //OdGeQuaternion rotation( -1., 0., 0., 0. );

    pText2d->setJustification( OdDg::kCenterCenter );
    pText2d->setText( OdString( "font with 255 id" ) );

    OdDgFontTablePtr pFontTable = m_pModel2d->database()->getFontTable();

    // RSC fonts with a specific number without any conditions. Actually that RSC font is not loaded
    pText2d->setFontEntryId(255);

    pText2d->setOrigin( OdGePoint2d( boxOrigin3d.x + major/20, boxOrigin3d.y + minor/3 ));
    pText2d->setHeightMultiplier(0.75);
    pText2d->setLengthMultiplier(0.75);
    pText2d->setColorIndex(3);

    pText2d->setInterCharSpacingFlag( true );
    pText2d->setSpacing( 1.0, OdDgTextExtendedProperties::sInterCharSpacing );

    m_pModel2d->addElement( pText2d );
  }
}


void OdExDgnFiller::addTextNode( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d boxOrigin = m_EntityBoxes.getBoxCenter( boxRow, boxColumn ) - OdGeVector3d( minor, 0, 0. );

  OdDgTextNode3dPtr pTextNode = OdDgTextNode3d::createObject();
  
  //set parameters
  pTextNode->setLineSpacing( 1.2 );
  pTextNode->setOrigin( boxOrigin );
  pTextNode->setRotation( OdGeQuaternion( 1., 0., 0., 0. ) );
  pTextNode->setTextNodeNumber( 1 );
  pTextNode->setColorIndex( 4 );
  pTextNode->setLengthMultiplier( (major / 16) );
  pTextNode->setHeightMultiplier( minor / 8 );

    OdDgTextStyleTablePtr pTextStyleTable = m_pModel3d->database()->getTextStyleTable();
    OdDgElementId id = pTextStyleTable->getAt("TextStyle1");
    if (!id.isNull())
    {
      OdDgTextStyleTableRecordPtr pTextStyle = id.safeOpenObject();
      pTextNode->setTextStyleEntryId( pTextStyle->getEntryId() );
    }

  m_pModel3d->addElement( pTextNode );

  //add strings
  {
    OdDgText3dPtr pText;

    pText = OdDgText3d::createObject();
    pText->setText( OdString( "First line" ) );
    pText->setLengthMultiplier( (major / 12) );
    pText->setHeightMultiplier( minor / 6 );
    pText->setCrCount(1);

    pText->setOrigin( boxOrigin );
    pText->setRotation( OdGeQuaternion( 1., 0., 0., 0. ) );
    pTextNode->add( pText );

    pText = OdDgText3d::createObject();
    pText->setText( OdString( "Second line" ) );
    pText->setLengthMultiplier( (major / 12) );
    pText->setHeightMultiplier( minor / 8);

    pText->setOrigin( boxOrigin + OdGeVector3d( 0., -1., 0. ) );
    pText->setRotation( OdGeQuaternion( 1., 0., 0., 0. ) );

    pTextNode->add( pText );
    pTextNode->setTextNodeNumber(8);
  }
}


void OdExDgnFiller::addShape( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();

  pShape->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( - major / 3., minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major / 3., minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major / 3., - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ) );

  m_pModel3d->addElement( pShape );
}

void OdExDgnFiller::addShapeWithTransparency( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();

  pShape->addVertex( center + OdGeVector3d( - major / 4., - minor /4., 0. ) );
  pShape->addVertex( center + OdGeVector3d( - major / 4., minor / 4., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major / 4., minor / 4., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major / 4., - minor / 4., 0. ) );
  pShape->addVertex( center + OdGeVector3d( - major / 4., - minor / 4., 0. ) );

  m_pModel3d->addElement( pShape );

  pShape->setFillColor( 1, true );

  pShape = OdDgShape3d::createObject();

  pShape->addVertex( center + OdGeVector3d( - major / 3., - minor /3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( - major / 3., minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major / 3., minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major / 3., - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ) );

  m_pModel3d->addElement( pShape );

  pShape->setFillColor( 3 );
  pShape->setPriority( pShape->getPriority() + 10 );

  OdCmTransparency transparency;
  transparency.setAlphaPercent(0.5);
  pShape->setTransparency(transparency);
}

void OdExDgnFiller::addShapeWithHole   ( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight()/2.0;
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn )/2.0;

  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgCellHeader3dPtr pHoleCell = OdDgCellHeader3d::createObject();

  m_pModel3d->addElement( pHoleCell );

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();

  pShape->addVertex( center + OdGeVector3d( - major / 1.1, - minor / 1.1, 0. ) );
  pShape->addVertex( center + OdGeVector3d( - major / 1.1, minor / 1.1, 0. ) );
  pShape->addVertex( center + OdGeVector3d( major / 1.1, minor / 1.1, 0. ) );
  pShape->addVertex( center + OdGeVector3d( major / 1.1, - minor / 1.1, 0. ) );
  pShape->addVertex( center + OdGeVector3d( - major / 1.1, - minor / 1.1, 0. ) );

  pShape->setHbitFlag(false);

  pShape->setFillColor( 4 );
  
  OdCmTransparency transparency;
  transparency.setAlphaPercent(0.2);
  pShape->setTransparency(transparency);

  pHoleCell->add( pShape );

  OdDgShape3dPtr pHole = OdDgShape3d::createObject();

  pHole->addVertex( center + OdGeVector3d( - major / 2., - minor / 2., 0. ) );
  pHole->addVertex( center + OdGeVector3d( - major / 2., minor / 2., 0. ) );
  pHole->addVertex( center + OdGeVector3d( major / 2., minor / 2., 0. ) );
  pHole->addVertex( center + OdGeVector3d( major / 2., - minor / 2., 0. ) );
  pHole->addVertex( center + OdGeVector3d( - major / 2., - minor / 2., 0. ) );

  pHole->setHbitFlag(true);

  pHoleCell->add( pHole );

  m_uNamedGroupElementArr.push_back( (OdUInt64)(pHoleCell->elementId().getHandle()) );
}

void OdExDgnFiller::addCurve( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgCurve3dPtr pCurve = OdDgCurve3d::createObject();

  pCurve->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ) );
  pCurve->addVertex( center + OdGeVector3d( - major / 3., minor / 3., 0. ) );
  pCurve->addVertex( center + OdGeVector3d( major / 3., minor / 3., 0. ) );
  pCurve->addVertex( center + OdGeVector3d( major / 3., - minor / 3., 0. ) );

  pCurve->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ) );
  pCurve->addVertex( center + OdGeVector3d( - major / 3., minor / 3., 0. ) );
  pCurve->addVertex( center + OdGeVector3d( major / 3., minor / 3., 0. ) );
  pCurve->addVertex( center + OdGeVector3d( major / 3., - minor / 3., 0. ) );

  m_pModel3d->addElement( pCurve );
}


void OdExDgnFiller::addPointString( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );
  OdGeQuaternion rotation( 1., 0., 0., 0. );

  OdDgPointString3dPtr pPointString = OdDgPointString3d::createObject();

  pPointString->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ), rotation );
  pPointString->addVertex( center + OdGeVector3d( - major / 3., minor / 3., 0. ), rotation );
  pPointString->addVertex( center + OdGeVector3d( major / 3., minor / 3., 0. ), rotation );
  pPointString->addVertex( center + OdGeVector3d( major / 3., - minor / 3., 0. ), rotation );

  m_pModel3d->addElement( pPointString );
}

void OdExDgnFiller::addLineStringWithBreak( int boxRow, int boxColumn )
{
  double dMinor = m_EntityBoxes.getHeight();
  double dMajor = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d ptCenter = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgLineString3dPtr pLineString = OdDgLineString3d::createObject();

  OdGePoint3d ptStart = ptCenter - OdGeVector3d::kXAxis * 3.0 * dMajor / 8.0 - OdGeVector3d::kYAxis * 3.0 * dMinor / 8.0;
  OdGePoint3d ptEnd   = ptCenter + OdGeVector3d::kXAxis * 3.0 * dMajor / 8.0 + OdGeVector3d::kYAxis * 3.0 * dMinor / 8.0;

  pLineString->addVertex( ptStart );
  pLineString->addVertex( ptEnd );
  pLineString->insertVertex( 1, ptStart + OdGeVector3d::kXAxis * dMajor * 3.0 / 4.0 );
  pLineString->insertVertex( 2, ptEnd   - OdGeVector3d::kXAxis * dMajor * 3.0 / 4.0 );
  
  OdGeVector3d vr12 = pLineString->getVertexAt(2) - pLineString->getVertexAt(1);

  pLineString->insertVertex( 2, pLineString->getVertexAt(1) + vr12 * 3.0 / 8.0 );
  pLineString->insertVertex( 3, pLineString->getVertexAt(3) - vr12 * 3.0 / 8.0 );
  pLineString->insertVertex( 3, OdDgLineString3d::getBreakLineValue() );

  m_pModel3d->addElement( pLineString );

  // line string 2d.

  OdDgLineString2dPtr pLineString2d = OdDgLineString2d::createObject();

  OdGePoint3d ptStart1 = ptCenter - OdGeVector3d::kXAxis * 3.0 * dMajor / 8.0 - OdGeVector3d::kYAxis * 3.0 * dMinor / 8.0;
  OdGePoint3d ptEnd1   = ptCenter + OdGeVector3d::kXAxis * 3.0 * dMajor / 8.0 + OdGeVector3d::kYAxis * 3.0 * dMinor / 8.0;

  pLineString2d->addVertex( ptStart1.convert2d() );
  pLineString2d->addVertex( ptEnd1.convert2d() );
  pLineString2d->insertVertex( 1, pLineString2d->getVertexAt(0) + OdGeVector2d::kXAxis * dMajor * 3.0 / 4.0 );
  pLineString2d->insertVertex( 2, pLineString2d->getVertexAt(2) - OdGeVector2d::kXAxis * dMajor * 3.0 / 4.0 );

  OdGeVector2d vr12d = pLineString2d->getVertexAt(2) - pLineString2d->getVertexAt(1);

  pLineString2d->insertVertex( 2, pLineString2d->getVertexAt(1) + vr12d * 3.0 / 8.0 );
  pLineString2d->insertVertex( 3, pLineString2d->getVertexAt(3) - vr12d * 3.0 / 8.0 );
  pLineString2d->insertVertex( 3, OdDgLineString2d::getBreakLineValue() );

  m_pModel2d->addElement( pLineString2d );
}

void OdExDgnFiller::addDimension(int boxRow, int boxCol)
{
  OdDgDimStyleTablePtr pTbl = m_pModel3d->database()->getDimStyleTable(OdDg::kForWrite);
  OdDgElementId idDimStyle;
  
  if( !pTbl.isNull() )
    idDimStyle = pTbl->getAt("DimStyle1");

  OdDgDimStyleTableRecordPtr pDimStyle;

  if( !idDimStyle.isNull() )
  {
    pDimStyle = idDimStyle.openObject(OdDg::kForWrite);
    pDimStyle->setExtensionLineColorOverrideFlag(true);
    pDimStyle->setExtensionLineColor(3);
    pDimStyle->setTextHeightOverrideFlag(true);
    pDimStyle->setTextHeight(2);
    pDimStyle->setTextWidthOverrideFlag(true);
    pDimStyle->setTextWidth(2);
  }

  // adds Dimensions
  //
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  // Creates SIZE ARROW dimension
  //
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  m_pModel3d->addElement( pDim );

  // Point 1
  ptDim.setPoint( pt.set( boxCenter.x - major / 4.0, boxCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( minor / 5 );
  ptDim.setPrimaryText("The Point 1");
  pDim->addPoint( ptDim );

  // Point 2
  ptDim.setPoint( pt.set( boxCenter.x + major / 5.5, boxCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( -minor / 70 );
  ptDim.setPrimaryText("The Point 2");
  pDim->addPoint( ptDim );

  if( !pDimStyle.isNull() )
  {
    pDim->applyDimensionStyle( pDimStyle->getEntryId() );
  }

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion(1.0, 0.0, 0.0, 0.0 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setFontEntryId(0);
  pDim->setDimTextInfo( info );
  pDim->setAnnotationScaleFlag(true);
  pDim->setAnnotationScale(0.8);
}

void OdExDgnFiller::addComplexString(int boxRow, int boxColumn)
{
  //3d
  {
    double height = m_EntityBoxes.getHeight();
    double width = m_EntityBoxes.getWidth( boxRow, boxColumn );
    OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

    OdDgComplexStringPtr pComplexString = OdDgComplexString::createObject();
    m_pModel3d->addElement( pComplexString );
    pComplexString->setColorIndex(1);

    OdDgLineString3dPtr pLineString;
    OdGePoint3d pos;

    pLineString = OdDgLineString3d::createObject();
    pLineString->setColorIndex(2);
    pos.set(center.x - width*0.25, center.y + height*0.125, 0.0);
    pLineString->addVertex(pos);
    pos.y += height*0.25;
    pLineString->addVertex(pos);
    pos.x += width*0.5;
    pLineString->addVertex(pos);
    pos.y -= height*0.25;
    pLineString->addVertex(pos);

    pComplexString->add(pLineString);

    pLineString = OdDgLineString3d::createObject();
    pLineString->setColorIndex(3);
    pos.set(center.x - width*0.25, center.y - height*0.125, 0.0);
    pLineString->addVertex(pos);
    pos.y -= height*0.25;
    pLineString->addVertex(pos);
    pos.x += width*0.5;
    pLineString->addVertex(pos);
    pos.y += height*0.25;
    pLineString->addVertex(pos);

    pComplexString->add(pLineString);
  }

  //2d
  {
    double height = m_EntityBoxes.getHeight();
    double width = m_EntityBoxes.getWidth( boxRow, boxColumn );
    OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

    OdDgComplexStringPtr pComplexString = OdDgComplexString::createObject();
    m_pModel2d->addElement( pComplexString );

    OdDgLine2dPtr line;
    OdDgArc2dPtr arc;

    line = OdDgLine2d::createObject();
    line->setStartPoint( OdGePoint2d( center.x + width * .45, center.y + height * .25 ) );
    line->setEndPoint( OdGePoint2d( center.x + width * .25, center.y ) );
    pComplexString->add( line );
    
    arc = OdDgArc2d::createObject();
    arc->setOrigin( ( OdGePoint2d( center.x, center.y ) ) );
    arc->setPrimaryAxis( width * .25 );
    arc->setSecondaryAxis( width * .4 );
    arc->setStartAngle( 0. );
    arc->setSweepAngle( 6. );
    pComplexString->add( arc );
  }
}


void OdExDgnFiller::addMultiline( int boxRow, int boxColumn )
{
  double      minor = m_EntityBoxes.getHeight();
  double      major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgMultilinePtr multiline = OdDgMultiline::createObject();

  //add some profiles
  {
    OdDgMultilineProfile    profile;
    OdDgMultilineSymbology  symbology;

    symbology.setLineWeight( 1 );
    symbology.setColorIndex( 34 );
    symbology.setUseColorFlag( true );
    profile.setSymbology( symbology );
    profile.setDistance( 0. );
    multiline->addProfile( profile );

    symbology.setColorIndex( 53 );
    profile.setSymbology( symbology );
    profile.setDistance( .1 );
    multiline->addProfile( profile );

    symbology.setColorIndex( 150 );
    symbology.setUseStyleFlag( true );
    symbology.setLineStyleEntryId( 3 );
    profile.setSymbology( symbology );
    profile.setDistance( -.1 );
    multiline->addProfile( profile );
  }

  //define caps
  {
    OdDgMultilineSymbology symbology;

    multiline->setOriginCapAngle( 90. );
    multiline->setEndCapAngle( 90. );

    symbology.setCapLineFlag( true );
    multiline->setOriginCap( symbology );
    multiline->setEndCap( symbology );
  }

  //add some points
  {
    OdDgMultilinePoint point;
    point.setPoint( center + OdGeVector3d( -major / 3., -minor / 3., 0. ) );
    multiline->addPoint( point );
    point.setPoint( center + OdGeVector3d( -major / 3., minor / 3., 0. ) );
    multiline->addPoint( point );
    point.setPoint( center + OdGeVector3d( major / 3., minor / 3., 0. ) );
    multiline->addPoint( point );
  }
  multiline->setLineWeight(4);

  m_pModel3d->addElement( multiline );

  OdDgMultilinePtr multiline2 = OdDgMultiline::createObject();

  OdDgMultilinePoint point;
  point.setPoint( center + OdGeVector3d( -major / 6, -minor / 3., 0. ) );
  multiline2->addPoint( point );
  point.setPoint( center + OdGeVector3d( major / 3., -minor / 3., 0. ) );
  multiline2->addPoint( point );
  point.setPoint( center + OdGeVector3d( major / 3., minor / 5, 0. ) );
  multiline2->addPoint( point );

  m_pModel3d->addElement( multiline2 );

  OdDgMultilineStyleTablePtr pMLSTable = m_pModel3d->database()->getMultilineStyleTable(OdDg::kForRead);

  if( !pMLSTable.isNull() )
  {
    OdDgElementId idMLS = pMLSTable->createIterator()->item();

    if( !idMLS.isNull() )
    {
      multiline2->applyMultilineStyle( idMLS );
    }
  }
}


void OdExDgnFiller::addComplexShape( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  //create and add to the model
  OdDgComplexShapePtr complexShape = OdDgComplexShape::createObject();
  m_pModel3d->addElement( complexShape );

  //add some elements
  {
    OdDgCurve3dPtr      curve;
    OdDgLineString3dPtr lineString;

    curve = OdDgCurve3d::createObject();
    curve->addVertex( center + OdGeVector3d( -3. * major / 8., 0. * minor / 5., 0. ) );
    curve->addVertex( center + OdGeVector3d( -2. * major / 8., 2. * minor / 5., 0. ) );
    curve->addVertex( center + OdGeVector3d( -1. * major / 8., 1. * minor / 5., 0. ) );
    curve->addVertex( center + OdGeVector3d( 0. * major / 8., 2. * minor / 5., 0. ) );
    curve->addVertex( center + OdGeVector3d( 1. * major / 8., 1. * minor / 5., 0. ) );
    curve->addVertex( center + OdGeVector3d( 2. * major / 8., 2. * minor / 5., 0. ) );
    curve->addVertex( center + OdGeVector3d( 3. * major / 8., 0. * minor / 5., 0. ) );

    lineString = OdDgLineString3d::createObject();
    lineString->addVertex( center + OdGeVector3d( 3. * major / 8., 0. * minor / 5., 0. ) );
    lineString->addVertex( center + OdGeVector3d( 2. * major / 8., -2. * minor / 5., 0. ) );
    lineString->addVertex( center + OdGeVector3d( 1. * major / 8., -1. * minor / 5., 0. ) );
    lineString->addVertex( center + OdGeVector3d( 0. * major / 8., -2. * minor / 5., 0. ) );
    lineString->addVertex( center + OdGeVector3d( -1. * major / 8., -1. * minor / 5., 0. ) );
    lineString->addVertex( center + OdGeVector3d( -2. * major / 8., -2. * minor / 5., 0. ) );
    lineString->addVertex( center + OdGeVector3d( -3. * major / 8., 0. * minor / 5., 0. ) );

    complexShape->add( curve );
    complexShape->add( lineString );

  }
}

void OdExDgnFiller::addBSplineCurveWithKnots( int boxRow, int boxColumn )
{
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdGePoint3d ptBase = center - OdGeVector3d::kXAxis * dWidth / 2.0 + OdGeVector3d::kYAxis * dHeight / 2.0;

  OdDgBSplineCurve3dPtr curve = OdDgBSplineCurve3d::createObject();
  m_pModel3d->addElement( curve );

  OdGePoint3dArray arrCtrlPts;
  arrCtrlPts.push_back( ptBase + OdGeVector3d( 1.225, -0.745, 0. ) );
  arrCtrlPts.push_back( ptBase + OdGeVector3d( -0.51, -0.48,  0. ) );
  arrCtrlPts.push_back( ptBase + OdGeVector3d( 1.27 , -3.9,   0. ) );
  arrCtrlPts.push_back( ptBase + OdGeVector3d( 2.15 , -1.35,  0. ) );
  arrCtrlPts.push_back( ptBase + OdGeVector3d( 0.8  , -1.65,  0. ) );
  arrCtrlPts.push_back( ptBase + OdGeVector3d( 1.28 , -2.22,  0. ) );

  // <Number of knots> = <number of poles> - <order>
  OdGeKnotVector vrKnots;
  vrKnots.append(0.5);
  vrKnots.append(0.6);

  OdGeDoubleArray arrWeights;

  curve->setNurbsData( 4, false, false, arrCtrlPts, vrKnots, arrWeights );
  curve->setColorIndex(3);
}

void OdExDgnFiller::addClosedBSplineCurveWithKnots( int boxRow, int boxColumn )
{
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdGePoint3d ptBase = center - OdGeVector3d::kXAxis * dWidth / 2.0 + OdGeVector3d::kYAxis * dHeight / 2.0;

  OdDgBSplineCurve3dPtr curve = OdDgBSplineCurve3d::createObject();
  m_pModel3d->addElement( curve );

  OdGePoint3dArray arrCtrlPts;
  arrCtrlPts.push_back( ptBase + OdGeVector3d( 1.055, -0.17, 0. ) );
  arrCtrlPts.push_back( ptBase + OdGeVector3d( -0.31, -0.18, 0. ) );
  arrCtrlPts.push_back( ptBase + OdGeVector3d( 1.47 , -3.6,  0. ) );
  arrCtrlPts.push_back( ptBase + OdGeVector3d( 2.35 , -1.05, 0. ) );
  arrCtrlPts.push_back( ptBase + OdGeVector3d( 1.0  , -1.35, 0. ) );
  //arrCtrlPts.push_back( ptBase + OdGeVector3d( 1.055, -0.17, 0. ) );

  // <Number of knots> = <number of poles> - 1
  OdGeKnotVector vrKnots;
  vrKnots.insert(0.2);
  vrKnots.insert(0.3);
  vrKnots.insert(0.8);
  vrKnots.insert(0.9);
  //vrKnots.insert(0.95);

  OdGeDoubleArray arrWeights;

  curve->setNurbsData( 4, false, true, arrCtrlPts, vrKnots, arrWeights );
  curve->setColorIndex(2);
}

void OdExDgnFiller::addBSplineCurve( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight() / 6.;
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 6.;
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgBSplineCurve3dPtr curve = OdDgBSplineCurve3d::createObject();
  m_pModel3d->addElement( curve );

  OdGePoint3dArray arrCtrlPts;
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * major,  1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * major,  2. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -2. * major,  2. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -2. * major,  1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * major,  1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * major,  1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  2. * major,  1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  2. * major,  2. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * major,  2. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * major,  1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * major, -1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * major, -2. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  2. * major, -2. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  2. * major, -1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * major, -1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * major, -1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -2. * major, -1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -2. * major, -2. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * major, -2. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * major, -1. * minor, 0. ) );
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * major,  1. * minor, 0. ) );

  OdGeKnotVector vrKnots;
  OdGeDoubleArray arrWeights;

  curve->setNurbsData( 4, false, true, arrCtrlPts, vrKnots, arrWeights );

  OdDgElementPtr pCurveElm = curve;

  addURLandFileLink( pCurveElm );
}


void OdExDgnFiller::addBSplineSurface( int boxRow, int boxColumn )
{
  double sx = m_EntityBoxes.getHeight() / 4.;
  double sy = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 8.;
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgBSplineSurfacePtr surface = OdDgBSplineSurface::createObject();

  m_pModel3d->addElement( surface );

  OdGePoint3dArray arrCtrlPts;
  OdGeDoubleArray  arrEmptyWeights;
  OdGeKnotVector   arrEmptyKnots; // to set uniform knots.

  arrCtrlPts.push_back( center + OdGeVector3d( -2. * sx, -2. * sy, 0. ) ); // 0,0
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * sx, -1. * sy, 0. ) ); // 1,0
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * sx, -1. * sy, 0. ) ); // 2,0
  arrCtrlPts.push_back( center + OdGeVector3d(  2. * sx, -2. * sy, 0. ) ); // 3,0
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * sx, -1. * sy, 0. ) ); // 0,1
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * sx, -1. * sy, 2. ) ); // 1,1
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * sx, -1. * sy, 2. ) ); // 2,1
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * sx, -1. * sy, 0. ) ); // 3,1
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * sx,  1. * sy, 0. ) ); // 0,2
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * sx,  1. * sy, 2. ) ); // 1,2
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * sx,  1. * sy, 2. ) ); // 2,2
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * sx,  1. * sy, 0. ) ); // 3,2
  arrCtrlPts.push_back( center + OdGeVector3d( -2. * sx,  2. * sy, 0. ) ); // 0,3
  arrCtrlPts.push_back( center + OdGeVector3d( -1. * sx,  1. * sy, 0. ) ); // 1,3
  arrCtrlPts.push_back( center + OdGeVector3d(  1. * sx,  1. * sy, 0. ) ); // 2,3
  arrCtrlPts.push_back( center + OdGeVector3d(  2. * sx,  2. * sy, 0. ) ); // 3,3

  surface->set( 4, 4, false, false, false, 4, 4, arrCtrlPts, arrEmptyWeights, arrEmptyKnots, arrEmptyKnots );
  surface->setNumberOfSpansInU(10);
  surface->setNumberOfSpansInV(10);
}


void OdExDgnFiller::addSurface( int boxRow, int boxColumn )
{
  double sx = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 3.;
  double sy = m_EntityBoxes.getHeight() / 3.;
  double sz = sx * 2.;
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgSurfacePtr surface = OdDgSurface::createObject();
  OdDg3dObjectHelper helper( surface );

  m_pModel3d->addElement( surface );

  //make some surface
  {
    OdDgEllipse3dPtr ellipse;
    OdDgLine3dPtr line;
    const double bias = .8;

    //boundary #1
    ellipse = OdDgEllipse3d::createObject();
    ellipse->setPrimaryAxis( sx );
    ellipse->setSecondaryAxis( sy );
    ellipse->setOrigin( center );
    helper.addToBoundary( ellipse );

    //boundary #2
    ellipse = OdDgEllipse3d::createObject();
    ellipse->setPrimaryAxis( sx * bias );
    ellipse->setSecondaryAxis( sy * bias );
    ellipse->setOrigin( center + OdGeVector3d( 0., 0., sz ) );
    helper.addToBoundary( ellipse );

    //rule #1
    line = OdDgLine3d::createObject();
    line->setStartPoint( center + OdGeVector3d( -sx, 0., 0. ) );
    line->setEndPoint( center + OdGeVector3d( -sx * bias, 0., sz ) );
    helper.addToRule( line );

    //rule #2
    line = OdDgLine3d::createObject();
    line->setStartPoint( center + OdGeVector3d( 0., sy, 0. ) );
    line->setEndPoint( center + OdGeVector3d( 0., sy * bias, sz ) );
    helper.addToRule( line );

    //rule #3
    line = OdDgLine3d::createObject();
    line->setStartPoint( center + OdGeVector3d( sx, 0., 0. ) );
    line->setEndPoint( center + OdGeVector3d( sx * bias, 0., sz ) );
    helper.addToRule( line );

    //rule #4
    line = OdDgLine3d::createObject();
    line->setStartPoint( center + OdGeVector3d( 0., -sy, 0. ) );
    line->setEndPoint( center + OdGeVector3d( 0., -sy * bias, sz ) );
    helper.addToRule( line );
  }
}

void OdExDgnFiller::addSolid( int boxRow, int boxColumn )
{
  double sx = m_EntityBoxes.getHeight() / 4.;
  double sy = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 2.;
  double sz = sx * 3.;
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgSolidPtr solid = OdDgSolid::createObject();
  OdDg3dObjectHelper helper( solid );

  m_pModel3d->addElement( solid );

  //make some solid
  {
    OdDgShape3dPtr shape;
    OdDgLine3dPtr line;

    //boundary #1
    shape = OdDgShape3d::createObject();
    shape->addVertex( center + OdGeVector3d( sx, sy, 0. ) * .8 );
    shape->addVertex( center + OdGeVector3d( sx, -sy, 0. ) * .8 );
    shape->addVertex( center + OdGeVector3d( -sx, -sy, 0. ) * .8 );
    shape->addVertex( center + OdGeVector3d( -sx, sy, 0. ) * .8 );
    shape->addVertex( center + OdGeVector3d( sx, sy, 0. ) * .8 );
    helper.addToBoundary( shape );

    //boundary #2
    shape = OdDgShape3d::createObject();
    shape->addVertex( center + OdGeVector3d( sx, sy, sz ) * .3 );
    shape->addVertex( center + OdGeVector3d( sx, -sy, sz ) * .3 );
    shape->addVertex( center + OdGeVector3d( -sx, -sy, sz ) * .3 );
    shape->addVertex( center + OdGeVector3d( -sx, sy, sz ) * .3 );
    shape->addVertex( center + OdGeVector3d( sx, sy, sz ) * .3 );
    helper.addToBoundary( shape );

    //rule #1
    line = OdDgLine3d::createObject();
    line->setStartPoint( center + OdGeVector3d( sx, sy, 0. ) * .8 );
    line->setEndPoint( center + OdGeVector3d( sx, sy, sz ) * .3 );
    helper.addToRule( line );

    //rule #2
    line = OdDgLine3d::createObject();
    line->setStartPoint( center + OdGeVector3d( sx, -sy, 0. ) * .8 );
    line->setEndPoint( center + OdGeVector3d( sx, -sy, sz ) * .3 );
    helper.addToRule( line );

    //rule #3
    line = OdDgLine3d::createObject();
    line->setStartPoint( center + OdGeVector3d( -sx, -sy, 0. ) * .8 );
    line->setEndPoint( center + OdGeVector3d( -sx, -sy, sz ) * .3 );
    helper.addToRule( line );

    //rule #4
    line = OdDgLine3d::createObject();
    line->setStartPoint( center + OdGeVector3d( -sx, sy, 0. ) * .8 );
    line->setEndPoint( center + OdGeVector3d( -sx, sy, sz ) * .3 );
    helper.addToRule( line );
  }
}

void OdExDgnFiller::addLights( int boxRow, int boxColumn )
{
  double sy = m_EntityBoxes.getHeight();
  double sx = m_EntityBoxes.getWidth( boxRow, boxColumn );
  double sz = sy;
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  // Add point light

  OdDgLightPointPtr pPointLight = OdDgLightPoint::createObject();

  m_pModel3d->addElement( pPointLight );

  OdGePoint3d ptPointLightOrigin = center;

  ptPointLightOrigin.x -= sx / 4.0;
  ptPointLightOrigin.z += sz / 4.0;

  pPointLight->setOrigin(ptPointLightOrigin);

  pPointLight->setDefaultValues(true, 4.0);
  pPointLight->setColorRed(1.0);
  pPointLight->setColorGreen(0.0);
  pPointLight->setColorBlue(0.0);
  pPointLight->setIntensity(2.0);

  // Add distant light

  OdDgLightDistantPtr pDistantLight = OdDgLightDistant::createObject();

  m_pModel3d->addElement( pDistantLight );

  OdGePoint3d ptDistantLightOrigin = center;

  ptDistantLightOrigin.x += sx / 4.0;
  ptDistantLightOrigin.z += sz / 4.0;

  pDistantLight->setOrigin(ptDistantLightOrigin);

  OdGeMatrix3d matDistantDirection;
  matDistantDirection.setToRotation( OdaPI, OdGeVector3d::kZAxis );

  pDistantLight->setTransformation( matDistantDirection );

  pDistantLight->setDefaultValues(true, 4.0);
  pDistantLight->setColorRed(0.0);
  pDistantLight->setColorGreen(1.0);
  pDistantLight->setColorBlue(0.0);
  pDistantLight->setIntensity(2.0);

  // Add spot light

  OdDgLightSpotPtr pSpotLight = OdDgLightSpot::createObject();

  m_pModel3d->addElement( pSpotLight );

  OdGePoint3d ptSpotLightOrigin = center;

  ptSpotLightOrigin.y += sy / 3.0;
  ptSpotLightOrigin.z += sz / 4.0;

  pSpotLight->setOrigin(ptSpotLightOrigin);

  OdGeMatrix3d matSpotDirection;
  matSpotDirection.setToRotation( 3.0*OdaPI/2.0, OdGeVector3d::kZAxis );

  pSpotLight->setTransformation( matSpotDirection );

  pSpotLight->setDefaultValues(true, 1.0);
  pSpotLight->setColorRed(0.0);
  pSpotLight->setColorGreen(0.0);
  pSpotLight->setColorBlue(1.0);
  pSpotLight->setIntensity(2.0);
  pSpotLight->setConeAngle(OdaPI/6.0);
  pSpotLight->setDeltaAngle(OdaPI/32.0);

  // Add area light

  OdDgLightAreaPtr pAreaLight = OdDgLightArea::createObject();

  m_pModel3d->addElement( pAreaLight );

  OdGePoint3d ptAreaLightOrigin = center;

  ptAreaLightOrigin.y -= sy / 3.0;
  ptAreaLightOrigin.z += sz / 4.0;

  pAreaLight->setOrigin(ptAreaLightOrigin);

  OdGeMatrix3d matAreaDirection;
  matAreaDirection.setToRotation( -3.0*OdaPI/4.0, OdGeVector3d::kXAxis );

  pAreaLight->setTransformation( matAreaDirection );

  pAreaLight->setDefaultValues(true, 1.0);
  pAreaLight->setColorRed(0.0);
  pAreaLight->setColorGreen(1.0);
  pAreaLight->setColorBlue(1.0);
  pAreaLight->setIntensity(2.0);

  OdDgShape3dPtr pShapeArea = OdDgShape3d::createObject();

  pAreaLight->add( pShapeArea );

  OdGePoint3d ptCur = ptAreaLightOrigin;
  ptCur.x -= sx/32;
  ptCur.y += sx/32;
  ptCur.z += sx/32;
  pShapeArea->addVertex(ptCur);
  ptCur.x += sx/16;
  pShapeArea->addVertex(ptCur);
  ptCur.z -= sx/16;
  ptCur.y -= sx/16;
  pShapeArea->addVertex(ptCur);
  ptCur.x -= sx/16;
  pShapeArea->addVertex(ptCur);
  ptCur.z += sx/16;
  ptCur.y += sx/16;
  pShapeArea->addVertex(ptCur);

  // Add sky opening light

  OdDgLightOpenSkyPtr pSkyLight = OdDgLightOpenSky::createObject();

  m_pModel3d->addElement( pSkyLight );

  OdGePoint3d ptSkyLightOrigin = center;

  ptSkyLightOrigin.z += sz / 2.0;

  pSkyLight->setOrigin(ptSkyLightOrigin);

  OdGeMatrix3d matSkyDirection;
  matSkyDirection.setToRotation( OdaPI, OdGeVector3d::kXAxis );

  pSkyLight->setTransformation( matSkyDirection );
  pSkyLight->setDefaultValues(true, 1.0);
  pSkyLight->setMinSamples(4);
  pSkyLight->setMaxSamples(16);

  OdDgShape3dPtr pShapeSky = OdDgShape3d::createObject();

  pSkyLight->add( pShapeSky );

  ptCur = ptSkyLightOrigin;
  ptCur.x -= sx/32;
  ptCur.y -= sx/32;
  pShapeSky->addVertex(ptCur);
  ptCur.x += sx/16;
  pShapeSky->addVertex(ptCur);
  ptCur.y += sx/16;
  pShapeSky->addVertex(ptCur);
  ptCur.x -= sx/16;
  pShapeSky->addVertex(ptCur);
  ptCur.y -= sx/16;
  pShapeSky->addVertex(ptCur);
}

void OdExDgnFiller::addRaster( int boxRow, int boxColumn )
{
  OdDgRasterHeader3dPtr raster = OdDgRasterHeader3d::createObject();
  double sx = m_EntityBoxes.getHeight() / 2.;
  double sy = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 2.;

  raster->setXExtent( 8 );
  raster->setYExtent( 8 );
  raster->setFormat( OdDgRaster::kBitmap );
  raster->setHorizontalDataFlag( true );
  raster->setScale( sy / 8 );
  raster->setOrigin( m_EntityBoxes.getBoxCenter( boxRow, boxColumn ) - OdGeVector3d( sx / 3., 0., 0. ) );
  raster->setForeground( 4 );

  m_pModel3d->addElement( raster );

  //add rows
  {
    OdDgRasterComponentPtr  component;
    OdUInt8                 byte;

    for( OdUInt16 i = 0; i < 8; i++ )       // MKU 23/12/09 - ('warning' issue)
    {
      component = OdDgRasterComponent::createObject();
      component->setFormat( OdDgRaster::kBitmap );
      component->setOffsetX( 0 );
      component->setOffsetY( i );
      component->setNumberOfPixels( 8 );

      switch( i )
      {
      case 0 : byte = 0x8E; break;
      case 1 : byte = 0xB3; break;
      case 2 : byte = 0xC3; break;
      case 3 : byte = 0xE6; break;
      case 4 : byte = 0x58; break;
      case 5 : byte = 0x4C; break;
      case 6 : byte = 0x87; break;
      case 7 : byte = 0x83; break;
      }

      component->setData( &byte, 1 );
      raster->add( component );
    }
  }
}

void OdExDgnFiller::addByteRaster( int boxRow, int boxColumn )
{
  double sy = m_EntityBoxes.getHeight() / 2.;
  double sx = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 2.;
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  // Add raster header 3d

  OdDgRasterHeader3dPtr pDgnRaster = OdDgRasterHeader3d::createObject();
  m_pModel3d->addElement( pDgnRaster );

  pDgnRaster->setFormat( OdDgRaster::kByteData );
  pDgnRaster->setHorizontalDataFlag( true );
  pDgnRaster->setXExtent( m_uRasterWidth );
  pDgnRaster->setYExtent( m_uRasterHeight );
  pDgnRaster->setScale( sx * 16.0 / 9.0 / m_uRasterWidth );
  pDgnRaster->setOrigin( center - sx * 8.0 / 9.0 * OdGeVector3d::kXAxis + sy * 5.5 / 9.0 * OdGeVector3d::kYAxis);
  pDgnRaster->setTransparentFlag(false);
  pDgnRaster->setColorFlag(true);

  OdBinaryData pComponentData;

  pComponentData.resize( m_uRasterWidth );

  OdUInt32 i,j;

  for( i = 0; i < m_uRasterHeight; i++ )
  {
    OdDgRasterComponentPtr pDgnRasterComponent = OdDgRasterComponent::createObject();

    pDgnRaster->add( pDgnRasterComponent );

    pDgnRasterComponent->setTransparentFlag( pDgnRaster->getTransparentFlag() );
    pDgnRasterComponent->setBackground( pDgnRaster->getBackground() );
    pDgnRasterComponent->setForeground( pDgnRaster->getForeground() );
    pDgnRasterComponent->setFormat( pDgnRaster->getFormat() );
    pDgnRasterComponent->setHorizontalDataFlag( pDgnRaster->getHorizontalDataFlag() );
    pDgnRasterComponent->setRightJustifiedFlag( pDgnRaster->getRightJustifiedFlag() );
    pDgnRasterComponent->setLowerJustifiedFlag( pDgnRaster->getLowerJustifiedFlag() );
    pDgnRasterComponent->setColorFlag( pDgnRaster->getColorFlag() );
    pDgnRasterComponent->setPositiveFlag( pDgnRaster->getPositiveFlag() );
    pDgnRasterComponent->setOffsetX(0);
    pDgnRasterComponent->setOffsetY(i);
    pDgnRasterComponent->setNumberOfPixels( m_uRasterWidth );

    for( j = 0; j < m_uRasterWidth; j++ )
    {
      pComponentData[j] = m_uByteRasterDataArr[i*m_uRasterWidth + j];
    }

    pDgnRasterComponent->setData( pComponentData.asArrayPtr(), m_uRasterWidth );
  }

  // Add raster header 2d

  OdDgRasterHeader2dPtr pDgnRaster2d = OdDgRasterHeader2d::createObject();
  m_pModel2d->addElement( pDgnRaster2d );

  pDgnRaster2d->setFormat( OdDgRaster::kByteData );
  pDgnRaster2d->setHorizontalDataFlag( true );
  pDgnRaster2d->setXExtent( m_uRasterWidth );
  pDgnRaster2d->setYExtent( m_uRasterHeight );
  pDgnRaster2d->setScale( sx * 16.0 / 9.0 / m_uRasterWidth );
  pDgnRaster2d->setOrigin( center - sx * 8.0 / 9.0 * OdGeVector3d::kXAxis + sy * 5.5 / 9.0 * OdGeVector3d::kYAxis);
  pDgnRaster2d->setTransparentFlag(false);
  pDgnRaster2d->setColorFlag(true);

  pComponentData.resize( m_uRasterWidth );

  for( i = 0; i < m_uRasterHeight; i++ )
  {
    OdDgRasterComponentPtr pDgnRasterComponent = OdDgRasterComponent::createObject();

    pDgnRaster2d->add( pDgnRasterComponent );

    pDgnRasterComponent->setTransparentFlag( pDgnRaster->getTransparentFlag() );
    pDgnRasterComponent->setBackground( pDgnRaster->getBackground() );
    pDgnRasterComponent->setForeground( pDgnRaster->getForeground() );
    pDgnRasterComponent->setFormat( pDgnRaster->getFormat() );
    pDgnRasterComponent->setHorizontalDataFlag( pDgnRaster->getHorizontalDataFlag() );
    pDgnRasterComponent->setRightJustifiedFlag( pDgnRaster->getRightJustifiedFlag() );
    pDgnRasterComponent->setLowerJustifiedFlag( pDgnRaster->getLowerJustifiedFlag() );
    pDgnRasterComponent->setColorFlag( pDgnRaster->getColorFlag() );
    pDgnRasterComponent->setPositiveFlag( pDgnRaster->getPositiveFlag() );
    pDgnRasterComponent->setOffsetX(0);
    pDgnRasterComponent->setOffsetY(i);
    pDgnRasterComponent->setNumberOfPixels( m_uRasterWidth );

    for( j = 0; j < m_uRasterWidth; j++ )
    {
      pComponentData[j] = m_uByteRasterDataArr[i*m_uRasterWidth + j];
    }

    pDgnRasterComponent->setData( pComponentData.asArrayPtr(), m_uRasterWidth );
  }
}

void OdExDgnFiller::addRasterAttach( int boxRow, int boxColumn )
{
  OdDgRasterAttachmentHeaderPtr raster = OdDgRasterAttachmentHeader::createObject();
  double sx = m_EntityBoxes.getHeight() / 2.;
  double rotation = .1;
  double affinity = .03;  //some angles to demonstration

  m_pModel3d->addElement( raster );

  //raster->setFilename( OdString( "attach.jpg" ) );
  raster->setFilename( OdString( "attachUnkown.jpg" ) );
  raster->setExtent( OdGePoint2d( 1., 1. ) );
  raster->setOrientation( m_EntityBoxes.getBoxCenter( boxRow, boxColumn ) - OdGeVector3d( sx / 3., 0., 0. ),
                          OdGeVector3d( cos( rotation ), sin( rotation ), 0. ) * .01,
                          OdGeVector3d( -sin( rotation + affinity ), cos( rotation + affinity ), 0. ) * .01 );

  raster->setApplyRotationFlag( true );

  OdDgRasterFramePtr pRasterFrame = OdDgRasterFrame::createObject();

  m_pModel3d->addElement(pRasterFrame);

  pRasterFrame->setRasterReferenceId( raster->id() );

  pRasterFrame->setColorIndex(5);
  pRasterFrame->setLineStyleEntryId(2);
  pRasterFrame->setLineWeight(2);
  pRasterFrame->setPrintableFlag(true);

  pRasterFrame->setTintColorIndex(1536);
  pRasterFrame->setForegroundColorIndex(1536);
  pRasterFrame->setBackgroundColorIndex(2032);
  pRasterFrame->setHighlightColorIndex(0);

  // check binding between a raster frame and a raster attachment
  OdString sTmpAtH = raster->getFilename();
  OdString sTmpFrame = pRasterFrame->getFileName();
  pRasterFrame->setFileName( OdString( "attach.jpg" ) );
  sTmpAtH = raster->getFilename();
  sTmpFrame = pRasterFrame->getFileName();

  // add clipping

  OdGePoint2dArray arrClipBoundary;

  arrClipBoundary.push_back( OdGePoint2d(0,30) );
  arrClipBoundary.push_back( OdGePoint2d(30,0) );
  arrClipBoundary.push_back( OdGePoint2d(70,0) );
  arrClipBoundary.push_back( OdGePoint2d(100,30) );
  arrClipBoundary.push_back( OdGePoint2d(100,70) );
  arrClipBoundary.push_back( OdGePoint2d(70,100) );
  arrClipBoundary.push_back( OdGePoint2d(30,100) );
  arrClipBoundary.push_back( OdGePoint2d(0,70) );
  arrClipBoundary.push_back( OdGePoint2d(0,30) );

  raster->addBoundaryClipVertices( arrClipBoundary );

  OdGePoint2dArray arrClipMask;

  arrClipMask.push_back( OdGePoint2d(40,45) );
  arrClipMask.push_back( OdGePoint2d(45,40) );
  arrClipMask.push_back( OdGePoint2d(55,40) );
  arrClipMask.push_back( OdGePoint2d(60,45) );
  arrClipMask.push_back( OdGePoint2d(60,55) );
  arrClipMask.push_back( OdGePoint2d(55,60) );
  arrClipMask.push_back( OdGePoint2d(45,60) );
  arrClipMask.push_back( OdGePoint2d(40,55) );
  arrClipMask.push_back( OdGePoint2d(40,45) );

  raster->addMaskClipVertices( arrClipMask );
  raster->setClippingFlag(true);

  // 2d
  {
    OdDgRasterAttachmentHeaderPtr raster2d = OdDgRasterAttachmentHeader::createObject();
    double sx2 = m_EntityBoxes.getHeight() / 2.;
    double rotation = .1;
    double affinity = .03;  //some angles to demonstration

    m_pModel2d->addElement( raster2d );

    raster2d->setFilename( OdString( "attach.jpg" ) );
    raster2d->setExtent( OdGePoint2d( 1., 1. ) );
    raster2d->setOrientation( m_EntityBoxes.getBoxCenter( boxRow, boxColumn ) - OdGeVector3d( sx2 / 3., 0., 0. ),
                              OdGeVector3d( cos( rotation ), sin( rotation ), 0. ) * .01,
                              OdGeVector3d( -sin( rotation + affinity ), cos( rotation + affinity ), 0. ) * .01 );

    //raster2d->setApplyRotationFlag( true );
  }
}

void OdExDgnFiller::addRasterAttachBmp( int boxRow, int boxColumn )
{
  OdDgRasterAttachmentHeaderPtr raster = OdDgRasterAttachmentHeader::createObject();
  double sx = m_EntityBoxes.getHeight() / 2.;
  double rotation = OdaPI2, affinity = -.1;  //some angles to demonstration

  m_pModel3d->addElement( raster );

  raster->setFilename( OdString( "Example.bmp" ) );
  raster->setExtent( OdGePoint2d( 1., 1. ) );

  //apply some rotation made via U & V vectors
  raster->setOrientation( m_EntityBoxes.getBoxCenter( boxRow, boxColumn ) - OdGeVector3d( 0.,  sx * .7, 0. ),
                          OdGeVector3d( cos( rotation ), sin( rotation ), 0. ) * .008,
                          OdGeVector3d( -sin( rotation + affinity ), cos( rotation + affinity ), 0. ) * .008 );

  raster->setApplyRotationFlag( true );
}

void OdExDgnFiller::addRasterAttachPdf(int boxRow, int boxColumn)
{
  OdDgRasterAttachmentHeaderPtr raster = OdDgRasterAttachmentHeader::createObject();
  double sx = m_EntityBoxes.getHeight() / 2.;

  m_pModel3d->addElement(raster);

  raster->setFilename(OdString("Example.pdf"));
  raster->setExtent(OdGePoint2d(1., 1.));

  //apply some rotation made via U & V vectors
  raster->setOrientation(m_EntityBoxes.getBoxCenter(boxRow, boxColumn) - OdGeVector3d(sx * .9, sx * .8, 0.),
    OdGeVector3d(0., 1., 0.) * .003,
    OdGeVector3d(1., 0., 0.) * .003);

  raster->setApplyRotationFlag(true);
}

void OdExDgnFiller::addTagElement( int boxRow, int boxColumn )
{
  OdDgText3dPtr     text = OdDgText3d::createObject();
  double            sx = m_EntityBoxes.getHeight() / 2.,
                    sy = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 2.;
  OdGePoint3d       center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  //make the text
  OdDgElementId textId;
  {
    OdGeQuaternion rotation( .707, 0., 0., -.707 );

    text->setJustification( OdDg::kCenterCenter );
    text->setText( OdString( "text with tag" ) );
    text->setOrigin( center - OdGeVector3d( 0., sy, 0. ) );
    text->setRotation( rotation );
    text->setHeightMultiplier(sy / 3.);
    text->setLengthMultiplier( (sx * 2. / 20) );

    text->setColorIndex( 15 );
       
    textId = m_pModel3d->addElement( text );
  }

  //create new definition set with new tag definition inside
  OdDgElementId setId;
  {
    OdDgTagDefinitionSetPtr definitionSet = OdDgTagDefinitionSet::createObject();
    definitionSet->setName( "Set of definitions" );

    OdDgTagDefinitionSetTablePtr tagTable = m_pModel3d->database()->getTagDefinitionSetTable(OdDg::kForWrite);
    tagTable->add( definitionSet );
    setId = definitionSet->elementId();

    //add definition for CHAR
    OdDgTagDefinitionPtr pDefinition = definitionSet->addDefinition( 
      OdDgTagDefinition::kChar, OdString( "Name for CHAR" ) );
    pDefinition->setPrompt( OdString( "Prompt for CHAR" ));
    pDefinition->setString( "Have a nice day!" );

    OdDgElementId id = tagTable->getAt( "Set of definitions" );
    OdDgTagDefinitionSetPtr pTagDefinitionsSetTmp = id.safeOpenObject();
    if (!pTagDefinitionsSetTmp.isNull())
    {
      OdString sName =  pTagDefinitionsSetTmp->getName();
    }
  }

  //create new tag
  {
    OdDgTagElementPtr tag = OdDgTagElement::createObject();
    m_pModel3d->addElement( tag );

    //bind to the parent element
    tag->setAssociationId( textId );

    //bind to the tag definition
    tag->setTagDefinitionId( setId, 0 );

    //set some properties
    //tag->setSizeMultiplier( OdGePoint2d( 10., 10. ) );
    tag->setLengthMultiplier( 0.06 );
    tag->setHeightMultiplier( 0.06 );
    tag->setAssociatedFlag( true );
    tag->setOffsetUsedFlag( true );
    tag->setOffset( OdGeVector3d( .1, 2., 0. ) );
    tag->setOrigin( text->getOrigin() );
  }
  {
    OdDgTagElementPtr tag = OdDgTagElement::createObject();
    m_pModel3d->addElement( tag );

    //bind to the parent element
    tag->setAssociationId( textId );

    //bind to the tag definition
    tag->setTagDefinitionId( setId, 0 );

    //set some properties
    //tag->setSizeMultiplier( OdGePoint2d( 10., 10. ) );
    tag->setLengthMultiplier( 0.06 );
    tag->setHeightMultiplier( 0.06 );
    tag->setAssociatedFlag( true );
    tag->setOffsetUsedFlag( true );
    tag->setOffset( OdGeVector3d( .1, 2., 0. ) );
    tag->setOrigin( text->getOrigin() );

    bool bJust = tag->getJustificationUsedFlag();
    OdGePoint3d ptOtext = text->getOrigin();
    tag->setJustification(OdDg::kLeftCenter);
    bJust = tag->getJustificationUsedFlag();
    OdDg::TextJustification just = tag->getJustification();
    tag->setJustification(just);
    OdGePoint3d ptO = tag->getOrigin();
    OdGeVector3d off = tag->getOffset();
  }
}


void OdExDgnFiller::addMesh( int boxRow, int boxColumn )
{
  //take the transformation to transpose the rectangle { -100 < x < 100; -100 < y < 100 } to the given box
  OdGeMatrix3d toBox;
  {
    //scale the axises
    toBox.setToIdentity();
    toBox[ 0 ][ 0 ] = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 200.;
    toBox[ 1 ][ 1 ] = m_EntityBoxes.getHeight() / 200.;

    //shift to the center
    toBox.setTranslation( m_EntityBoxes.getBoxCenter( boxRow, boxColumn ).asVector() );
  }

  //face loops
  {
    OdDgMeshFaceLoopsPtr mesh = OdDgMeshFaceLoops::createObject();
    m_pModel3d->addElement( mesh );
    mesh->setColorIndex( 36 );

    mesh->setPointsNumber( 8 );
    mesh->setPoint( 0, OdGePoint3d( -70., 20., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, OdGePoint3d( -70., 80., 0. ).transformBy( toBox ) );
    mesh->setPoint( 2, OdGePoint3d( -60., 40., 0. ).transformBy( toBox ) );
    mesh->setPoint( 3, OdGePoint3d( -60., 60., 0. ).transformBy( toBox ) );
    mesh->setPoint( 4, OdGePoint3d( -50., 40., 0. ).transformBy( toBox ) );
    mesh->setPoint( 5, OdGePoint3d( -50., 60., 0. ).transformBy( toBox ) );
    mesh->setPoint( 6, OdGePoint3d( -40., 20., 0. ).transformBy( toBox ) );
    mesh->setPoint( 7, OdGePoint3d( -40., 80., 0. ).transformBy( toBox ) );

    OdDgMeshFaceLoops::VerticesArray vertices;
    vertices.resize( 4 );

    mesh->setFacesNumber( 4 );
    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 3; vertices[ 3 ].m_pointIndex = 2;
    vertices[ 0 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_impliesVisibleEdge = false; 
    mesh->setFace( 0, vertices );
    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 2; vertices[ 2 ].m_pointIndex = 4; vertices[ 3 ].m_pointIndex = 6;
    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    mesh->setFace( 1, vertices );
    vertices[ 0 ].m_pointIndex = 3; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 7; vertices[ 3 ].m_pointIndex = 5;
    vertices[ 0 ].m_impliesVisibleEdge = false; 
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    mesh->setFace( 2, vertices );
    vertices[ 0 ].m_pointIndex = 6; vertices[ 1 ].m_pointIndex = 4; vertices[ 2 ].m_pointIndex = 5; vertices[ 3 ].m_pointIndex = 7;
    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    mesh->setFace( 3, vertices );

    m_uNamedGroupElementArr.push_back( (OdUInt64)(mesh->elementId().getHandle()) );
  }

  //point cloud
  {
    OdDgMeshPointCloudPtr mesh = OdDgMeshPointCloud::createObject();
    m_pModel3d->addElement( mesh );

    mesh->setPointsNumber( 4 );
    mesh->setPoint( 0, OdGePoint3d( 40., 20., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, OdGePoint3d( 50., 40., 0. ).transformBy( toBox ) );
    mesh->setPoint( 2, OdGePoint3d( 60., 60., 0. ).transformBy( toBox ) );
    mesh->setPoint( 3, OdGePoint3d( 70., 80., 0. ).transformBy( toBox ) );
  }

  //triangle list
  {
    OdDgMeshTriangleListPtr mesh = OdDgMeshTriangleList::createObject();
    m_pModel3d->addElement( mesh );
    mesh->setColorIndex( 39 );

    OdDgMeshTriangleList::Triangle triangle;
    mesh->setTrianglesNumber( 3 );

    triangle.m_points[ 0 ] = OdGePoint3d( -14., 20., 0. ).transformBy( toBox );
    triangle.m_points[ 1 ] = OdGePoint3d( -6., 70., 0. ).transformBy( toBox );
    triangle.m_points[ 2 ] = OdGePoint3d( 2., 20., 0. ).transformBy( toBox );
    mesh->setTriangle( 0, triangle );
    triangle.m_points[ 0 ] = OdGePoint3d( 6., 20., 0. ).transformBy( toBox );
    triangle.m_points[ 1 ] = OdGePoint3d( -2., 70., 0. ).transformBy( toBox );
    triangle.m_points[ 2 ] = OdGePoint3d( 14., 70., 0. ).transformBy( toBox );
    mesh->setTriangle( 1, triangle );
    triangle.m_points[ 0 ] = OdGePoint3d( 10., 20., 0. ).transformBy( toBox );
    triangle.m_points[ 1 ] = OdGePoint3d( 14., 40., 0. ).transformBy( toBox );
    triangle.m_points[ 2 ] = OdGePoint3d( 18., 20., 0. ).transformBy( toBox );
    mesh->setTriangle( 2, triangle );
  }

  //quad list
  {
    OdDgMeshQuadListPtr mesh = OdDgMeshQuadList::createObject();
    m_pModel3d->addElement( mesh );
    mesh->setColorIndex( 10 );

    OdDgMeshQuadList::Quad quad;
    mesh->setQuadsNumber( 3 );

    quad.m_points[ 0 ] = OdGePoint3d( -40., -50., 0. ).transformBy( toBox );
    quad.m_points[ 1 ] = OdGePoint3d( -50.5, -80., 0. ).transformBy( toBox );
    quad.m_points[ 2 ] = OdGePoint3d( -43.5, -50., 0. ).transformBy( toBox );
    quad.m_points[ 3 ] = OdGePoint3d( -50.5, -20., 0. ).transformBy( toBox );
    mesh->setQuad( 0, quad );
    quad.m_points[ 0 ] = OdGePoint3d( -54., -20., 0. ).transformBy( toBox );
    quad.m_points[ 1 ] = OdGePoint3d( -47., -50., 0. ).transformBy( toBox );
    quad.m_points[ 2 ] = OdGePoint3d( -54., -80., 0. ).transformBy( toBox );
    quad.m_points[ 3 ] = OdGePoint3d( -61., -50., 0. ).transformBy( toBox );
    mesh->setQuad( 1, quad );
    quad.m_points[ 0 ] = OdGePoint3d( -57.5, -80., 0. ).transformBy( toBox );
    quad.m_points[ 1 ] = OdGePoint3d( -64.5, -50., 0. ).transformBy( toBox );
    quad.m_points[ 2 ] = OdGePoint3d( -57.5, -20., 0. ).transformBy( toBox );
    quad.m_points[ 3 ] = OdGePoint3d( -68., -50., 0. ).transformBy( toBox );
    mesh->setQuad( 2, quad );
  }

  //triangle grid
  {
    OdDgMeshTriangleGridPtr mesh = OdDgMeshTriangleGrid::createObject();
    m_pModel3d->addElement( mesh );
    mesh->setColorIndex( 40 );

    mesh->setGridSize( 2, 3 );
    mesh->setPoint( 0, 0, OdGePoint3d( -15., -50., 0. ).transformBy( toBox ) );
    mesh->setPoint( 0, 1, OdGePoint3d( 0., -30., 0. ).transformBy( toBox ) );
    mesh->setPoint( 0, 2, OdGePoint3d( 15., -10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, 0, OdGePoint3d( -15., -70., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, 1, OdGePoint3d( -10., -75., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, 2, OdGePoint3d( -5., -80., 0. ).transformBy( toBox ) );
  }

  //quad grid
  {
    OdDgMeshQuadGridPtr mesh = OdDgMeshQuadGrid::createObject();
    m_pModel3d->addElement( mesh );
    mesh->setColorIndex( 90 );

    mesh->setGridSize( 2, 4 );
    mesh->setPoint( 0, 0, OdGePoint3d( 40., -10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 0, 1, OdGePoint3d( 50., -30., 0. ).transformBy( toBox ) );
    mesh->setPoint( 0, 2, OdGePoint3d( 60., -30., 0. ).transformBy( toBox ) );
    mesh->setPoint( 0, 3, OdGePoint3d( 70., -40., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, 0, OdGePoint3d( 40., -90., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, 1, OdGePoint3d( 50., -70., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, 2, OdGePoint3d( 60., -70., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, 3, OdGePoint3d( 70., -60., 0. ).transformBy( toBox ) );
  }
}

void OdExDgnFiller::addColorMesh( int boxRow, int boxColumn )
{
  //take the transformation to transpose the rectangle { -100 < x < 100; -100 < y < 100 } to the given box
  OdGeMatrix3d toBox;
  {
    //scale the axises
    toBox.setToIdentity();
    toBox[ 0 ][ 0 ] = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 200.;
    toBox[ 1 ][ 1 ] = m_EntityBoxes.getHeight() / 200.;

    //shift to the center
    toBox.setTranslation( m_EntityBoxes.getBoxCenter( boxRow, boxColumn ).asVector() );
  }

  //face loops
  {
    OdDgMeshFaceLoopsPtr mesh = OdDgMeshFaceLoops::createObject();
    m_pModel3d->addElement( mesh );
    mesh->setColorIndex( 10 );

    mesh->setPointsNumber( 8 );
    mesh->setPoint( 0, OdGePoint3d( -80., -80., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, OdGePoint3d( -80., 80., 0. ).transformBy( toBox ) );
    mesh->setPoint( 2, OdGePoint3d( -50., -10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 3, OdGePoint3d( -50., 10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 4, OdGePoint3d( -40., -10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 5, OdGePoint3d( -40., 10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 6, OdGePoint3d( -10., -80., 0. ).transformBy( toBox ) );
    mesh->setPoint( 7, OdGePoint3d( -10., 80., 0. ).transformBy( toBox ) );

    OdDgMeshFaceLoops::VerticesArray vertices;
    vertices.resize( 4 );

    mesh->setUseColorTableIndexesFlag(true);

    mesh->setFacesNumber( 4 );
    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 3; vertices[ 3 ].m_pointIndex = 2;
    vertices[ 0 ].m_impliesVisibleEdge = true; 
    vertices[ 0 ].m_colorIndex = 2;
    vertices[ 1 ].m_impliesVisibleEdge = false; 
    vertices[ 1 ].m_colorIndex = 2;
    vertices[ 2 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_colorIndex = 2;
    vertices[ 3 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_colorIndex = 2;
    mesh->setFace( 0, vertices );
    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 2; vertices[ 2 ].m_pointIndex = 4; vertices[ 3 ].m_pointIndex = 6;
    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 0 ].m_colorIndex = 3;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_colorIndex = 3;
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_colorIndex = 3;
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_colorIndex = 3;
    mesh->setFace( 1, vertices );
    vertices[ 0 ].m_pointIndex = 3; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 7; vertices[ 3 ].m_pointIndex = 5;
    vertices[ 0 ].m_impliesVisibleEdge = false; 
    vertices[ 0 ].m_colorIndex = 5;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_colorIndex = 5;
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_colorIndex = 5;
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_colorIndex = 5;
    mesh->setFace( 2, vertices );
    vertices[ 0 ].m_pointIndex = 6; vertices[ 1 ].m_pointIndex = 4; vertices[ 2 ].m_pointIndex = 5; vertices[ 3 ].m_pointIndex = 7;
    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 0 ].m_colorIndex = 8;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_colorIndex = 8;
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_colorIndex = 8;
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_colorIndex = 8;
    mesh->setFace( 3, vertices );

    OdDgMeshFaceLoopsPtr meshDColor = OdDgMeshFaceLoops::createObject();
    m_pModel3d->addElement( meshDColor );
    meshDColor->setColorIndex( 25 );

    meshDColor->setPointsNumber( 8 );
    meshDColor->setPoint( 0, OdGePoint3d(  10., -80., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 1, OdGePoint3d(  10., 80., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 2, OdGePoint3d(  40., -10., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 3, OdGePoint3d(  40., 10., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 4, OdGePoint3d(  50., -10., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 5, OdGePoint3d(  50., 10., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 6, OdGePoint3d(  80., -80., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 7, OdGePoint3d(  80., 80., 0. ).transformBy( toBox ) );

    meshDColor->setUseDoubleColorsFlag(true);

    vertices[ 0 ].m_colorIndex = -1;
    vertices[ 1 ].m_colorIndex = -1;
    vertices[ 2 ].m_colorIndex = -1;
    vertices[ 3 ].m_colorIndex = -1;

    meshDColor->setFacesNumber( 4 );

    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 3; vertices[ 3 ].m_pointIndex = 2;

    vertices[ 0 ].m_impliesVisibleEdge = true; 
    vertices[ 0 ].m_dColorRGB[0] = 1.0;
    vertices[ 0 ].m_dColorRGB[1] = 1.0;
    vertices[ 0 ].m_dColorRGB[2] = 0.0;

    vertices[ 1 ].m_impliesVisibleEdge = false; 
    vertices[ 1 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 1 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 1 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    vertices[ 2 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 2 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 2 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    vertices[ 3 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 3 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 3 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    meshDColor->setFace( 0, vertices );

    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 2; vertices[ 2 ].m_pointIndex = 4; vertices[ 3 ].m_pointIndex = 6;

    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 0 ].m_dColorRGB[0] = 1.0;
    vertices[ 0 ].m_dColorRGB[1] = 0.0;
    vertices[ 0 ].m_dColorRGB[2] = 1.0;

    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 1 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 1 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 2 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 2 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 3 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 3 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    meshDColor->setFace( 1, vertices );

    vertices[ 0 ].m_pointIndex = 3; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 7; vertices[ 3 ].m_pointIndex = 5;

    vertices[ 0 ].m_impliesVisibleEdge = false; 
    vertices[ 0 ].m_dColorRGB[0] = 0.0;
    vertices[ 0 ].m_dColorRGB[1] = 0.5;
    vertices[ 0 ].m_dColorRGB[2] = 1.0;

    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 1 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 1 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 2 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 2 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 3 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 3 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    meshDColor->setFace( 2, vertices );

    vertices[ 0 ].m_pointIndex = 6; vertices[ 1 ].m_pointIndex = 4; vertices[ 2 ].m_pointIndex = 5; vertices[ 3 ].m_pointIndex = 7;

    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 0 ].m_dColorRGB[0] = 0.5;
    vertices[ 0 ].m_dColorRGB[1] = 1.0;
    vertices[ 0 ].m_dColorRGB[2] = 0.3;

    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 1 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 1 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 2 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 2 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_dColorRGB[0] = vertices[ 0 ].m_dColorRGB[0];
    vertices[ 3 ].m_dColorRGB[1] = vertices[ 0 ].m_dColorRGB[1];
    vertices[ 3 ].m_dColorRGB[2] = vertices[ 0 ].m_dColorRGB[2];

    meshDColor->setFace( 3, vertices );
  }
}

void OdExDgnFiller::addNormAndTexMesh( int boxRow, int boxColumn )
{
  //take the transformation to transpose the rectangle { -100 < x < 100; -100 < y < 100 } to the given box
  OdGeMatrix3d toBox;
  {
    //scale the axises
    toBox.setToIdentity();
    toBox[ 0 ][ 0 ] = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 200.;
    toBox[ 1 ][ 1 ] = m_EntityBoxes.getHeight() / 200.;

    //shift to the center
    toBox.setTranslation( m_EntityBoxes.getBoxCenter( boxRow, boxColumn ).asVector() );
  }

  //face loops
  {
    OdDgMeshFaceLoopsPtr mesh = OdDgMeshFaceLoops::createObject();
    m_pModel3d->addElement( mesh );
    mesh->setColorIndex( 10 );

    mesh->setPointsNumber( 8 );
    mesh->setPoint( 0, OdGePoint3d( -80., -80., 0. ).transformBy( toBox ) );
    mesh->setPoint( 1, OdGePoint3d( -80., 80., 0. ).transformBy( toBox ) );
    mesh->setPoint( 2, OdGePoint3d( -50., -10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 3, OdGePoint3d( -50., 10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 4, OdGePoint3d( -40., -10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 5, OdGePoint3d( -40., 10., 0. ).transformBy( toBox ) );
    mesh->setPoint( 6, OdGePoint3d( -10., -80., 0. ).transformBy( toBox ) );
    mesh->setPoint( 7, OdGePoint3d( -10., 80., 0. ).transformBy( toBox ) );

    OdDgMeshFaceLoops::VerticesArray vertices;
    vertices.resize( 4 );

    mesh->setUseNormalsFlag(true);

    OdGeVector3d vrNormal0 = -OdGeVector3d::kXAxis;
    vrNormal0 = vrNormal0.rotateBy(OdaPI4, OdGeVector3d::kZAxis );

    OdGeVector3d vrNormal1 = -OdGeVector3d::kXAxis;
    vrNormal1 = vrNormal1.rotateBy(-OdaPI4, OdGeVector3d::kZAxis );

    OdGeVector3d vrNormal6 = OdGeVector3d::kXAxis;
    vrNormal6 = vrNormal6.rotateBy(-OdaPI4, OdGeVector3d::kZAxis );

    OdGeVector3d vrNormal7 = OdGeVector3d::kXAxis;
    vrNormal7 = vrNormal7.rotateBy(OdaPI4, OdGeVector3d::kZAxis );

    mesh->setFacesNumber( 4 );
    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 3; vertices[ 3 ].m_pointIndex = 2;
    vertices[ 0 ].m_impliesVisibleEdge = true; 
    vertices[ 0 ].m_vrNormal = vrNormal0;
    vertices[ 1 ].m_impliesVisibleEdge = false; 
    vertices[ 1 ].m_vrNormal = vrNormal1;
    vertices[ 2 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_vrNormal = OdGeVector3d::kZAxis;
    vertices[ 3 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_vrNormal = OdGeVector3d::kZAxis;
    mesh->setFace( 0, vertices );
    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 2; vertices[ 2 ].m_pointIndex = 4; vertices[ 3 ].m_pointIndex = 6;
    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 0 ].m_vrNormal = vrNormal0;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_vrNormal = OdGeVector3d::kZAxis;
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_vrNormal = OdGeVector3d::kZAxis;
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_vrNormal = vrNormal6;
    mesh->setFace( 1, vertices );
    vertices[ 0 ].m_pointIndex = 3; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 7; vertices[ 3 ].m_pointIndex = 5;
    vertices[ 0 ].m_impliesVisibleEdge = false; 
    vertices[ 0 ].m_vrNormal = OdGeVector3d::kZAxis;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_vrNormal = vrNormal1;
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_vrNormal = vrNormal7;
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_vrNormal = OdGeVector3d::kZAxis;
    mesh->setFace( 2, vertices );
    vertices[ 0 ].m_pointIndex = 6; vertices[ 1 ].m_pointIndex = 4; vertices[ 2 ].m_pointIndex = 5; vertices[ 3 ].m_pointIndex = 7;
    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 0 ].m_vrNormal = vrNormal6;
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_vrNormal = OdGeVector3d::kZAxis;
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_vrNormal = OdGeVector3d::kZAxis;
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_vrNormal = vrNormal7;
    mesh->setFace( 3, vertices );

    OdDgMeshFaceLoopsPtr meshDColor = OdDgMeshFaceLoops::createObject();
    m_pModel3d->addElement( meshDColor );
    meshDColor->setColorIndex( 25 );

    meshDColor->setPointsNumber( 8 );
    meshDColor->setPoint( 0, OdGePoint3d(  10., -80., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 1, OdGePoint3d(  10., 80., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 2, OdGePoint3d(  40., -10., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 3, OdGePoint3d(  40., 10., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 4, OdGePoint3d(  50., -10., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 5, OdGePoint3d(  50., 10., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 6, OdGePoint3d(  80., -80., 0. ).transformBy( toBox ) );
    meshDColor->setPoint( 7, OdGePoint3d(  80., 80., 0. ).transformBy( toBox ) );

    meshDColor->setUseTextureCoordinatesFlag(true);

    meshDColor->setFacesNumber( 4 );

    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 3; vertices[ 3 ].m_pointIndex = 2;

    vertices[ 0 ].m_impliesVisibleEdge = true; 
    vertices[ 0 ].m_texCoords = OdGePoint2d( 0.0, 0.0 );
    vertices[ 1 ].m_impliesVisibleEdge = false; 
    vertices[ 1 ].m_texCoords = OdGePoint2d( 0.0, 2.0 );
    vertices[ 2 ].m_impliesVisibleEdge = true; 
    vertices[ 2 ].m_texCoords = OdGePoint2d( 0.9, 1.1 );
    vertices[ 3 ].m_impliesVisibleEdge = false; 
    vertices[ 3 ].m_texCoords = OdGePoint2d( 0.9, 0.9 );

    meshDColor->setFace( 0, vertices );

    vertices[ 0 ].m_pointIndex = 0; vertices[ 1 ].m_pointIndex = 2; vertices[ 2 ].m_pointIndex = 4; vertices[ 3 ].m_pointIndex = 6;

    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 0 ].m_texCoords = OdGePoint2d( 0.0, 0.0 );
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_texCoords = OdGePoint2d( 0.9, 0.9 );
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_texCoords = OdGePoint2d( 1.1, 0.9 );
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_texCoords = OdGePoint2d( 2.0, 0.0 );

    meshDColor->setFace( 1, vertices );

    vertices[ 0 ].m_pointIndex = 3; vertices[ 1 ].m_pointIndex = 1; vertices[ 2 ].m_pointIndex = 7; vertices[ 3 ].m_pointIndex = 5;

    vertices[ 0 ].m_impliesVisibleEdge = false; 
    vertices[ 0 ].m_texCoords = OdGePoint2d( 0.9, 1.1 );
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_texCoords = OdGePoint2d( 0.0, 2.0 );
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_texCoords = OdGePoint2d( 2.0, 2.0 );
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_texCoords = OdGePoint2d( 1.1, 1.1 );

    meshDColor->setFace( 2, vertices );

    vertices[ 0 ].m_pointIndex = 6; vertices[ 1 ].m_pointIndex = 4; vertices[ 2 ].m_pointIndex = 5; vertices[ 3 ].m_pointIndex = 7;

    vertices[ 0 ].m_impliesVisibleEdge = false;
    vertices[ 0 ].m_texCoords = OdGePoint2d( 2.0, 0.0 );
    vertices[ 1 ].m_impliesVisibleEdge = true; 
    vertices[ 1 ].m_texCoords = OdGePoint2d( 1.1, 0.9 );
    vertices[ 2 ].m_impliesVisibleEdge = false; 
    vertices[ 2 ].m_texCoords = OdGePoint2d( 1.1, 1.1 );
    vertices[ 3 ].m_impliesVisibleEdge = true; 
    vertices[ 3 ].m_texCoords = OdGePoint2d( 2.0, 2.0 );

    meshDColor->setFace( 3, vertices );

    // Create material with texture for mesh

    OdDgMaterialTablePtr pMatTable = m_pModel3d->database()->getMaterialTable(OdDg::kForWrite);

    OdDgMaterialTableRecordPtr pMat = OdDgMaterialTableRecord::createObject();
    OdGiMaterialColor matColor;
    matColor.setColor( OdDgCmEntityColor(255,255,255 ) );
    matColor.setMethod( OdGiMaterialColor::kOverride );
    matColor.setFactor( 1.0 );

    OdDgMaterialMap texture;
    OdGiMaterialMap matTexture;
    matTexture.setSourceFileName(L"attach.jpg");
    matTexture.setBlendFactor(1.0);
    OdGiMapper& map = matTexture.mapper();
    OdGeMatrix3d matTransform;
    matTransform[0][0] = 1.0;
    matTransform[1][1] = 2.0;
    matTransform[2][2] = 3.0;
    map.setTransform( matTransform );
    texture.setGiMaterialMap(matTexture);
    texture.setMappingType( OdDgMaterialMap::kElevationDrape );
    texture.setTextureScaleMode( OdDgMaterialMap::kMasterUnitMode );

    pMat->setDiffuse(matColor, texture);
    pMat->setName(L"Mesh material");

    pMatTable->add( pMat );

    meshDColor->setMaterial( pMat->elementId() );
  }
}

void OdExDgnFiller::addAttach( int boxRow, int boxColumn )
{
  // MKU 23/12/09 - ('warning' issue) //double            sx = m_EntityBoxes.getWidth( boxRow, boxColumn ) / 2.;
  double            sy = m_EntityBoxes.getHeight() / 2.;
  OdGePoint3d       center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgReferenceAttachmentHeaderPtr attach = OdDgReferenceAttachmentHeader::createObject();

  //create new element
  m_pModel3d->addElement( attach );
  attach->setFileName( L"attach.dgn" );
  attach->setDisplayFlag(true);
  attach->setSnapLockFlag(true);
  attach->setLocateLockFlag(true);
  attach->setTrueScaleFlag(true);
  attach->setScale( .3 );
  attach->setTransformation( OdGeMatrix3d::rotation( 1., OdGeVector3d( 0., 0., 1. ) ) );
  attach->setMasterOrigin( center - OdGePoint3d( 0., sy *.5, 0. ).asVector() );

  m_uNamedGroupElementArr.push_back( (OdUInt64)(attach->elementId().getHandle()) );

  // create level table for XRef in current database to xRef level names into level mask.

  OdDgLevelTablePtr pLevelTableForXRef  = attach->getLevelTable( OdDg::kForRead );

  if( pLevelTableForXRef.isNull() ) // XRef had not been loaded.
  {
    pLevelTableForXRef  = attach->getLevelTable( OdDg::kForWrite );

    OdDgLevelTablePtr pLevelTable = m_pModel3d->database()->getLevelTable(OdDg::kForRead);

    if( !pLevelTable.isNull() )
    {
      OdDgElementId idDefaultLevel = pLevelTable->getAt(L"Default");

      if( !idDefaultLevel.isNull() )
      {
        OdDgLevelTableRecordPtr pDefaultLevel = idDefaultLevel.openObject(OdDg::kForRead);

        OdDgLevelTableRecordPtr pLevel0 = pDefaultLevel->clone();
        OdDgLevelTableRecordPtr pLevel1 = pDefaultLevel->clone();
        pLevel1->setName(L"ExDgnCreate level 1");
        pLevel1->setElementColorIndex(6);
        pLevel1->setElementLineWeight(10);
        OdDgLevelTableRecordPtr pLevel2 = pDefaultLevel->clone();
        pLevel2->setName(L"XRef Level 1");
        pLevel2->setElementColorIndex(1);
        pLevel2->setElementLineWeight(3);
        OdDgLevelTableRecordPtr pLevel3 = pDefaultLevel->clone();
        pLevel3->setName(L"XRef Level 2");
        pLevel3->setElementColorIndex(2);
        pLevel3->setElementLineWeight(4);
        OdDgLevelTableRecordPtr pLevel4 = pDefaultLevel->clone();
        pLevel4->setName(L"XRef Level 3");
        pLevel4->setElementColorIndex(3);
        pLevel4->setElementLineWeight(5);

        pLevelTableForXRef->add( pLevel0 );
        pLevelTableForXRef->add( pLevel1 );
        pLevelTableForXRef->add( pLevel2 );
        pLevelTableForXRef->add( pLevel3 );
        pLevelTableForXRef->add( pLevel4 );
      }
    }
  }

  attach->setPaletteValue(75.0);
  attach->setPaletteSaturation(50.0);

  //define as viewable at all levels
  {
    OdDgLevelMaskPtr  levelMask = OdDgLevelMask::createObject();
    attach->add( levelMask );
    levelMask->setLevelIsVisible( 1, false );
    levelMask->setLevelIsVisible( 2, false );
    levelMask->setLevelIsVisible( 3, true );
    levelMask->setLevelIsVisible( 4, false );
    levelMask->setLevelIsVisible( 64, true );
  }
  {
    OdGePoint3d       center1 = m_EntityBoxes.getBoxCenter( 6, 0 );
    OdDgReferenceAttachmentHeaderPtr attach = OdDgReferenceAttachmentHeader::createObject();

    //create new element
    m_pModel3d->addElement( attach );
    attach->setFileName( L"attach1.dgn" );
    attach->setDisplayFlag(true);
    attach->setSnapLockFlag(true);
    attach->setLocateLockFlag(true);
    attach->setScale(0.1);
    attach->setTrueScaleFlag( true );
    attach->setMasterOrigin( center1 );

    //define as viewable at all levels
    {
      OdDgLevelMaskPtr  levelMask = OdDgLevelMask::createObject();
      attach->add( levelMask );

      int               i;
    
      for( i = 1; i <= 64; i++ )
      {
        levelMask->setLevelIsVisible( i, true );
      }
    }
  }
}

void OdExDgnFiller::addStringLinkage( OdDgElement* pElm )
{
  if ( pElm == 0 )
    return;

  OdUInt32 stringId = 5;
  pElm->setStringLinkage(stringId, L"ExDgnCreate Example String");
}


void OdExDgnFiller::addSharedCells( int boxRow, int boxColumn )
{
  OdDgSharedCellDefinitionPtr definition;
  OdDgSharedCellDefinitionTablePtr table = m_pModel3d->database()->getSharedCellDefinitionTable(OdDg::kForWrite);

  //create a definition
  {
    definition = OdDgSharedCellDefinition::createObject();
    definition->setName( "Named definition" );
    table->add( definition );
    definition->setAnnotationScaleAvailbleFlag(true);

    OdDgEllipse3dPtr ellipse;
    
    ellipse = OdDgEllipse3d::createObject();
    ellipse->setPrimaryAxis( 1. );
    ellipse->setSecondaryAxis( 1. );
    ellipse->setLineWeight(3);
    definition->add( ellipse );

    ellipse = OdDgEllipse3d::createObject();
    ellipse->setPrimaryAxis( .8 );
    ellipse->setSecondaryAxis( .8 );
    ellipse->setColorIndex( 7 );
    ellipse->setLineWeight((OdUInt32)OdDg::kLineWeightByCell);                        // MKU 23/12/09 - ('warning' issue)
    definition->add( ellipse );

    ellipse = OdDgEllipse3d::createObject();
    ellipse->setPrimaryAxis( .2 );
    ellipse->setSecondaryAxis( .2 );
    ellipse->setOrigin( OdGePoint3d( .4, 0., 0. ) );
    ellipse->setColorIndex((OdUInt32)OdDg::kColorByCell);                             // MKU 23/12/09 - ('warning' issue)
    definition->add( ellipse );

    ellipse = OdDgEllipse3d::createObject();
    ellipse->setPrimaryAxis( .2 );
    ellipse->setSecondaryAxis( .2 );
    ellipse->setOrigin( OdGePoint3d( -.4, 0., 0. ) );
    ellipse->setThickness(0.2);
    definition->add( ellipse );
  }

  double sx = m_EntityBoxes.getWidth( boxRow, boxColumn ), sy = m_EntityBoxes.getHeight();
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  //create references
  {
    OdDgSharedCellReferencePtr reference;
    OdGeMatrix3d transformation;

    reference = OdDgSharedCellReference::createObject();
    reference->setDefinitionName( "Named definition" );
    transformation.setToRotation( 70., OdGeVector3d::kZAxis );
    transformation *= transformation.scaling( sx * .1 );
    reference->setTransformation( transformation );
    reference->setOrigin( center + OdGeVector3d( -.2 * sx, -.1 * sy, 0. ) );
    reference->setColorIndex( (OdUInt32)OdDg::kColorByLevel );                // MKU 23/12/09 - ('warning' issue)
    reference->setColorOverrideFlag( true );
    reference->setUseAnnotationScaleFlag(true);
    reference->setAnnotationScale(1.5);
    m_pModel3d->addElement( reference );

    reference = OdDgSharedCellReference::createObject();
    reference->setDefinitionName( "Named definition" );
    transformation.setToRotation( 5., OdGeVector3d::kZAxis );
    transformation *= transformation.scaling( sx * .03 );
    reference->setTransformation( transformation );
    reference->setOrigin( center + OdGeVector3d( 0. * sx, .2 * sy, 0. ) );
    reference->setColorIndex( 4 );
    reference->setColorOverrideFlag( true );
    reference->setLineWeight(5);
    m_pModel3d->addElement( reference );

    reference = OdDgSharedCellReference::createObject();
    reference->setDefinitionName( "Named definition" );
    transformation.setToRotation( 67., OdGeVector3d::kZAxis );
    transformation *= transformation.scaling( sx * .07 );
    reference->setTransformation( transformation );
    reference->setOrigin( center + OdGeVector3d( .2 * sx, 0.1 * sy, 0. ) );
    reference->setColorIndex( 5 );
    reference->setColorOverrideFlag( true );

    reference->setLineWeight((OdUInt32)OdDg::kLineWeightByLevel);           // MKU 23/12/09 - ('warning' issue)
    reference->setWeightOverrideFlag( true );

    OdDgElementId idLevel = m_pModel3d->database()->getLevelTable()->getAt("ExDgnCreate level 1");
    if (!idLevel.isNull())
    {
      reference->setLevelId( idLevel );
      reference->setLevelOverrideFlag ( true );
    }

    m_pModel3d->addElement( reference );
  }
}


void OdExDgnFiller::addCell( int boxRow, int boxColumn )
{
  // MKU 23/12/09 - ('warning' issue) //double sx = m_EntityBoxes.getWidth( boxRow, boxColumn ), sy = m_EntityBoxes.getHeight();
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );
  OdGeVector3d offset = center.asVector();

  //2d
  {
    OdGeVector2d offset2d( offset.x, offset.y );

    //create a cell
    OdDgCellHeader2dPtr cell = OdDgCellHeader2d::createObject();
    m_pModel2d->addElement( cell );

    //add all elements
    OdDgArc2dPtr arc;
    OdDgLine2dPtr line;

    arc = OdDgArc2d::createObject();
    arc->setPrimaryAxis( 0.8865 );
    arc->setSecondaryAxis( 0.8865 );
    arc->setStartAngle( 1.3 );
    arc->setSweepAngle( 5.7 );
    arc->setOrigin( OdGePoint2d( -0.3046, 0.5490 ) + offset2d );
    arc->setColorIndex( 2 );
    cell->add( arc );

    line = OdDgLine2d::createObject();
    line->setStartPoint( OdGePoint2d( -.3, .3 ) + offset2d );
    line->setEndPoint( OdGePoint2d( .8, -0.8 ) + offset2d );
    line->setColorIndex( 10 );
    cell->add( line );

    line = OdDgLine2d::createObject();
    line->setStartPoint( OdGePoint2d( -.2, 0.4 ) + offset2d );
    line->setEndPoint( OdGePoint2d( .9, -.7 ) + offset2d );
    line->setColorIndex( 10 );
    cell->add( line );

    //origin
    cell->setOrigin( OdGePoint2d( center.x, center.y ) );

    //rotate its range
    cell->setTransformation( OdGeMatrix2d::rotation( OdaPI/6.0 ) );
  }
  //3d
  {
    //create a cell
    OdDgCellHeader3dPtr cell = OdDgCellHeader3d::createObject();
    m_pModel3d->addElement( cell );

    //add all elements
    OdDgArc3dPtr arc;
    OdDgLine3dPtr line;

    arc = OdDgArc3d::createObject();
    arc->setPrimaryAxis( 0.8865 );
    arc->setSecondaryAxis( 0.8865 );
    arc->setStartAngle( 0. );
    arc->setSweepAngle( 4. );
    arc->setOrigin( OdGePoint3d( -0.3046,0.5490,0.0000 ) + offset );
    arc->setColorIndex( 2 );
    cell->add( arc );

    arc = OdDgArc3d::createObject();
    arc->setPrimaryAxis( 0.7865 );
    arc->setSecondaryAxis( 0.7865 );
    arc->setStartAngle( -1. );
    arc->setSweepAngle( 4. );
    arc->setOrigin( OdGePoint3d( -0.3046,0.5490,0.0000 ) + offset );
    arc->setColorIndex( 2 );
    cell->add( arc );

    line = OdDgLine3d::createObject();
    line->setStartPoint( OdGePoint3d( -1.3020,-0.4469,0.0000 ) + offset );
    line->setEndPoint( OdGePoint3d( -0.6720,0.2865,0.0000 ) + offset );
    line->setColorIndex( 10 );
    cell->add( line );

    line = OdDgLine3d::createObject();
    line->setStartPoint( OdGePoint3d( -0.6720,0.2865,0.0000 ) + offset );
    line->setEndPoint( OdGePoint3d( -1.5190,-0.1796,0.0000 ) + offset );
    line->setColorIndex( 10 );
    cell->add( line );

    line = OdDgLine3d::createObject();
    line->setStartPoint( OdGePoint3d( -1.5190,-0.1796,0.0000 ) + offset );
    line->setEndPoint( OdGePoint3d( -1.3020,-0.4469,0.0000 ) + offset );
    line->setColorIndex( 10 );
    cell->add( line );

    //origin
    cell->setOrigin( center );

    //rotate its range
    cell->setTransformation( OdGeMatrix3d::rotation( OdaPI4, OdGeVector3d( 0., 0., 1. ) ) );
  }
}

void OdExDgnFiller::addSmartSolid( int boxRow, int boxColumn )
{
  const OdUInt8 pX_B[] = {
      0x50, 0x53, 0x00, 0x00, 0x00, 0x33, 0x3A, 0x20, 0x54, 0x52, 0x41, 0x4E, 0x53, 0x4D, 0x49, 0x54 , 0x20, 0x46, 0x49, 0x4C, 0x45, 0x20, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x62, 0x79
    , 0x20, 0x6D, 0x6F, 0x64, 0x65, 0x6C, 0x6C, 0x65, 0x72, 0x20, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F , 0x6E, 0x20, 0x31, 0x38, 0x30, 0x30, 0x31, 0x39, 0x37, 0x00, 0x00, 0x00, 0x11, 0x53, 0x43, 0x48
    , 0x5F, 0x31, 0x32, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5F, 0x31, 0x32, 0x30, 0x30, 0x36, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x0C, 0x00, 0x02, 0x00, 0x00, 0x00, 0x17, 0x00, 0x01, 0x00, 0x03, 0x00, 0x01
    , 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x40, 0x8F, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3E, 0x45 , 0x79, 0x8E, 0xE2, 0x30, 0x8C, 0x3A, 0x00, 0x01, 0x00, 0x04, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01
    , 0x01, 0x00, 0x05, 0x00, 0x06, 0x00, 0x07, 0x00, 0x01, 0x00, 0x08, 0x00, 0x09, 0x00, 0x01, 0x00 , 0x46, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00
    , 0x04, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0A, 0x00 , 0x0A, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x0D, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01
    , 0x00, 0x02, 0x00, 0x01, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01, 0x00, 0x0C, 0x00, 0x01, 0x00, 0x33 , 0x00, 0x06, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x0E, 0x00, 0x01, 0x00, 0x01
    , 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0xBF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x1F, 0x00, 0x07, 0x00, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x09, 0x00, 0x0F, 0x00
    , 0x01, 0x00, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x08, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02
    , 0x00, 0x0C, 0x00, 0x01, 0x00, 0x10, 0x56, 0x00, 0x10, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0C, 0x00 , 0x11, 0xC2, 0xBC, 0x92, 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x12, 0x00, 0x01, 0x00, 0x13, 0x00
    , 0x07, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x51, 0x00, 0x00, 0x00, 0x01, 0x00, 0x11, 0x00 , 0x00, 0x00, 0x16, 0x00, 0x14, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01, 0x00, 0x15, 0x00, 0x16, 0x00
    , 0x17, 0x00, 0x11, 0x00, 0x12, 0x00, 0x01, 0x00, 0x18, 0x00, 0x12, 0x00, 0x12, 0x00, 0x01, 0x00 , 0x19, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01, 0x2B, 0x00, 0x10, 0x00, 0x13, 0x00, 0x00, 0x00, 0x06
    , 0x00, 0x16, 0xC2, 0xBC, 0x92, 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x09, 0x00, 0x01 , 0x00, 0x0F, 0x00, 0x01, 0x00, 0x01, 0x00, 0x02, 0x00, 0x51, 0x00, 0x00, 0x00, 0x01, 0x00, 0x16
    , 0x00, 0x00, 0x00, 0x17, 0x00, 0x14, 0x00, 0x13, 0x00, 0x01, 0x00, 0x01, 0x00, 0x11, 0x00, 0x01 , 0x00, 0x1B, 0x00, 0x11, 0x00, 0x1A, 0x00, 0x01, 0x00, 0x1C, 0x00, 0x1A, 0x00, 0x1A, 0x00, 0x01
    , 0x00, 0x1D, 0x00, 0x13, 0x00, 0x01, 0x00, 0x01, 0x2B, 0x00, 0x1F, 0x00, 0x0F, 0x00, 0x00, 0x00 , 0x0A, 0x00, 0x01, 0x00, 0x13, 0x00, 0x01, 0x00, 0x07, 0x00, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x1C , 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00, 0x1A, 0x00, 0x0D, 0x00, 0x01, 0x00, 0x11, 0x00, 0x1D
    , 0x00, 0x01, 0x00, 0x1E, 0x00, 0x1D, 0x00, 0x1D, 0x00, 0x01, 0x00, 0x1A, 0x00, 0x13, 0x00, 0x01 , 0x00, 0x01, 0x2D, 0x00, 0x0F, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x08, 0x00, 0x01, 0x00, 0x1D, 0x00
    , 0x1F, 0x00, 0x01, 0x00, 0x0E, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x09, 0x00, 0x20, 0xC2, 0xBC, 0x92 , 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x0B, 0x00, 0x1E, 0x00, 0x05, 0x00, 0x21, 0x2B
    , 0x00, 0x01, 0x00, 0x01, 0x00, 0x0D, 0x00, 0x0B, 0x00, 0x10, 0x00, 0x51, 0x00, 0x00, 0x00, 0x01 , 0x00, 0x20, 0x00, 0x00, 0x00, 0x14, 0x00, 0x14, 0x00, 0x1F, 0x00, 0x01, 0x00, 0x01, 0x00, 0x22
    , 0x00, 0x15, 0x00, 0x23, 0x00, 0x0E, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x02, 0x00, 0x15, 0xC2, 0xBC , 0x92, 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x18, 0x00, 0x05, 0x00, 0x06
    , 0x2B, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x10, 0x00, 0x0E, 0x00, 0x0B, 0x00 , 0x00, 0x00, 0x0F, 0x00, 0x22, 0xC2, 0xBC, 0x92, 0x8F, 0x99, 0x6E, 0x00, 0x00, 0x00, 0x1F, 0x00
    , 0x01, 0x00, 0x24, 0x00, 0x05, 0x00, 0x0E, 0x2B, 0x00, 0x01, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x01 , 0x00, 0x10, 0x00, 0x32, 0x00, 0x21, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x1F, 0x00, 0x01
    , 0x00, 0x0E, 0x00, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0xBF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00 , 0x10, 0x00, 0x00, 0x00, 0x04, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00
    , 0x01, 0x00, 0x08, 0x00, 0x0B, 0x00, 0x32, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x11, 0x00, 0x01, 0x00 , 0x0B, 0x00, 0x21, 0x00, 0x06, 0x00, 0x01, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    , 0x00, 0x51, 0x00, 0x00, 0x00, 0x01, 0x00, 0x22, 0x00, 0x00, 0x00, 0x13, 0x00, 0x14, 0x00, 0x0B , 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x20, 0x00, 0x25, 0x00, 0x0F, 0x00, 0x24, 0x00, 0x00
    , 0x00, 0x0E, 0x00, 0x01, 0x00, 0x19, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x11, 0x00, 0x19, 0x00, 0x01 , 0x00, 0x24, 0x00, 0x19, 0x00, 0x19, 0x00, 0x01, 0x00, 0x12, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01
    , 0x2D, 0x00, 0x50, 0x00, 0x00, 0x00, 0x01, 0x00, 0x14, 0x00, 0x26, 0x00, 0x27, 0x00, 0x00, 0x23 , 0x28, 0x00, 0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
    , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x25 , 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x4F, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x27
    , 0x42, 0x53, 0x49, 0x5F, 0x45, 0x6E, 0x74, 0x69, 0x74, 0x79, 0x49, 0x64, 0x00, 0x51, 0x00, 0x00 , 0x00, 0x01, 0x00, 0x15, 0x00, 0x00, 0x00, 0x15, 0x00, 0x14, 0x00, 0x0D, 0x00, 0x01, 0x00, 0x01
    , 0x00, 0x20, 0x00, 0x11, 0x00, 0x28, 0x00, 0x0F, 0x00, 0x18, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x01 , 0x00, 0x12, 0x00, 0x0D, 0x00, 0x1C, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x28, 0x00, 0x00
    , 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x23, 0x00, 0x00 , 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x1B, 0x00, 0x00
    , 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x52, 0x00, 0x00, 0x00, 0x02, 0x00, 0x17, 0x00, 0x00 , 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x13, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x05, 0x00, 0x01
    , 0x00, 0x02, 0x00, 0x01, 0x00, 0x08, 0x00, 0x05, 0x53, 0x00, 0x4A, 0x00, 0x00, 0x00, 0x14, 0x00 , 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x16, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00
    , 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00 , 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00
    , 0x01, 0x00, 0x01 };

  double sx = m_EntityBoxes.getWidth( boxRow, boxColumn );
  // MKU 23/12/09 - ('warning' issue) //double sx = m_EntityBoxes.getWidth( boxRow, boxColumn ), sy = m_EntityBoxes.getHeight();
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );
  center.x += 2*sx;

  OdStreamBufPtr pBuf = OdMemoryStream::createNew();
  pBuf->putBytes(pX_B, sizeof(pX_B));
  pBuf->rewind();

  OdDgCellHeader3dPtr cell = OdDgCellHeader3d::createObject();

  // pCell must be DBRO entity.

  m_pModel3d->addElement( cell );

  //rotate and scale the cell
  {
    OdGeMatrix3d transformation;

    transformation.setToScaling( 0.5 );
    transformation *= OdGeMatrix3d::rotation( 10., OdGeVector3d( 1., 1., 1. ) );

    cell->setTransformation( transformation );
  }

  //origin
  cell->setOrigin( center );

  OdDgBRepEntityPEPtr(cell)->fillSmartSolid(*cell, *pBuf);
}

void OdExDgnFiller::addPatterns( int boxRow, int boxColumn )
{
  double sx = m_EntityBoxes.getWidth( boxRow, boxColumn ), sy = m_EntityBoxes.getHeight();
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );
  OdGeVector3d offset = center.asVector();

  OdDgEllipse3dPtr ellipse;

  //linear hatch
  {
    ellipse = OdDgEllipse3d::createObject();
    ellipse->setOrigin( center + OdGeVector3d( sx * -.3, sy * .1 , 0. ) );
    ellipse->setPrimaryAxis( sy / 5. );
    ellipse->setSecondaryAxis( sy / 5. );
    m_pModel3d->addElement( ellipse );

    OdDgLinearHatchPatternPtr pPattern = OdDgLinearHatchPattern::createObject();
    pPattern->setSpace( .2  );
    pPattern->setAngle( .45 );
    pPattern->setLineColorIndex( 4 );

    ellipse->setHatchPattern( pPattern );
  }

  //cross hatch
  {
    ellipse = OdDgEllipse3d::createObject();
    ellipse->setOrigin( center + OdGeVector3d( 0., sy * -.1 , 0. )  );
    ellipse->setPrimaryAxis( sy / 4. );
    ellipse->setSecondaryAxis( sy / 4. );
    m_pModel3d->addElement( ellipse );

    OdDgCrossHatchPatternPtr pCrossPattern = OdDgCrossHatchPattern::createObject();
    pCrossPattern->setSpace1( .1 );
    pCrossPattern->setAngle1( .1 );
    pCrossPattern->setSpace2( .3 );
    pCrossPattern->setAngle2( .6 );
    pCrossPattern->setLineColorIndex( 6 );

    ellipse->setHatchPattern( pCrossPattern );
  }

  //symbol hatch
  {
    ellipse = OdDgEllipse3d::createObject();
    ellipse->setOrigin( center + OdGeVector3d( sx * .3, sy * .2 , 0. )  );
    ellipse->setPrimaryAxis( sy / 3.5 );
    ellipse->setSecondaryAxis( sy / 3.5 );
    m_pModel3d->addElement( ellipse );

    OdDgSharedCellDefinitionTablePtr pShatrdCellDefTable = m_pModel3d->database()->getSharedCellDefinitionTable(OdDg::kForRead);

    OdDgSymbolHatchPatternPtr pSymbolHatch = OdDgSymbolHatchPattern::createObject();
    pSymbolHatch->setSpace1( .1 );
    pSymbolHatch->setAngle1( 0. );
    pSymbolHatch->setSpace2( .1 );
    pSymbolHatch->setAngle2( .5 );
    pSymbolHatch->setScale( .1 );
    pSymbolHatch->setSymbolId( pShatrdCellDefTable->getAt( "Named definition" ).getHandle() );

    ellipse->setHatchPattern( pSymbolHatch );
  }
}


void OdExDgnFiller::addDBLinkages( int boxRow, int boxColumn )
{
  //geometry stuff
  const int samplesNumber = 11; //10 for DB linkages + DMRS linkage
  double sx = m_EntityBoxes.getWidth( boxRow, boxColumn ), sy = m_EntityBoxes.getHeight();
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  //
  //add all linkages
  //

  //text to create
  OdDgText3dPtr text;

  //linkage to create (non-DRMS)
  OdDgDBLinkagePtr linkage;
  OdString linkageName;

  OdUInt16 sampleIndex;                                                           // MKU 23/12/09 - ('warning' issue)

  for( sampleIndex = 0; sampleIndex < samplesNumber; sampleIndex++ )
  {
    //create the bearing text
    {
      text = OdDgText3d::createObject();

      text->setOrigin( center + OdGeVector3d( sx * ( sampleIndex - samplesNumber / 2. + .7 ) / samplesNumber, sy * -.45, 0. ) );
      text->setJustification( OdDg::kCenterCenter );
      text->setRotation( OdGeQuaternion( .707, 0., 0., -.707 ) );
      text->setLengthMultiplier( sy / 16. );
      text->setHeightMultiplier( sx / samplesNumber / 2. );
      text->setColorIndex( 2 );

      m_pModel3d->addElement( text );
    }

    //add the DMRS linkage
    if( !sampleIndex )
    {
      OdDgDMRSLinkagePtr linkage = OdDgDMRSLinkage::createObject();

      linkage->setTableId( 1 );
      linkage->setMSLink( 0x1234 );

      text->addLinkage( linkage->getPrimaryId(), linkage.get(), false );
      text->setText( "DMRS" );
      continue;
    }

    //detect the linkage to create and the name to assign
    switch( sampleIndex )
    {
    case 1 : 
      linkageName = "BSI";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kBSI );
      break;
    case 2 : 
      linkageName = "FRAMME";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kFRAMME );
      break;
    case 3 : 
      linkageName = "Informix";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kInformix );
      break;
    case 4 : 
      linkageName = "Ingres";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kIngres );
      break;
    case 5 : 
      linkageName = "ODBC";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kODBC );
      break;
    case 6 : 
      linkageName = "OLE DB";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kOLEDB );
      break;
    case 7 : 
      linkageName = "Oracle";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kOracle );
      break;
    case 8 : 
      linkageName = "RIS";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kRIS );
      break;
    case 9 : 
      linkageName = "Sybase";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kSybase );
      break;
    case 10 : 
      linkageName = "xBase";
      linkage = OdDgDBLinkage::createObject( OdDgDBLinkage::kXbase );
      break;
    }

    linkage->setTableEntityId( sampleIndex );
    linkage->setMSLink( sampleIndex + samplesNumber );

    text->setText( linkageName );
    text->addLinkage( linkage->getPrimaryId(), linkage.get(), false );
  }
}

void OdExDgnFiller::addTrueColorShapes( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgDatabase* database = m_pModel3d->database();

  //first of all, the pink one
  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  pShape->addVertex( center + OdGeVector3d( major * -3. / 9, - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * -3. / 9, minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * -1. / 9, minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * -1. / 9, - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * -3. / 9, - minor / 3., 0. ) );
  m_pModel3d->addElement( pShape );
  pShape->setColor( ODRGB( 255, 150, 150 ) ); //note: an element should be added to the database to use true colors (because they are managed by the owning database only)

  //and other fancy colors...
  pShape = OdDgShape3d::createObject();
  pShape->addVertex( center + OdGeVector3d( major * -1. / 9, - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major *- 1. / 9, minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * 1. / 9, minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * 1. / 9, - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * -1. / 9, - minor / 3., 0. ) );
  m_pModel3d->addElement( pShape );
  pShape->setColor( ODRGB( 100, 250, 150 ) ); //note: an element should be added to the database to use true colors (because they are managed by the owning database only)

  pShape = OdDgShape3d::createObject();
  pShape->addVertex( center + OdGeVector3d( major * 1. / 9, - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * 1. / 9, minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * 3. / 9, minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * 3. / 9, - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * 1. / 9, - minor / 3., 0. ) );
  m_pModel3d->addElement( pShape );
  pShape->setColor( ODRGB( 100, 80, 255 ) ); //note: an element should be added to the database to use true colors (because they are managed by the owning database only)
}

void OdExDgnFiller::addFilledShape( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  pShape->addVertex( center + OdGeVector3d( major * -1. / 3, - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * -1. / 3, minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * 1. / 3, minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * 1. / 3, - minor / 3., 0. ) );
  pShape->addVertex( center + OdGeVector3d( major * -1. / 3, - minor / 3., 0. ) );
  m_pModel3d->addElement( pShape );

  //its boundary
  pShape->setColor( ODRGB( 255, 150, 150 ) );
  pShape->setLineWeight( 3 );

  //its inside
  {
    OdUInt32 uFillIndex = OdDgColorTable::getColorIndexByRGB( pShape->database(), ODRGB( 110, 100, 185 ) ); //any simple index 0...255 could be here
    pShape->setFillColor(uFillIndex);
  }
}

void OdExDgnFiller::addTitleText( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  // TitleText 2d

  OdDgTitleText2dPtr pTitleText2d = OdDgTitleText2d::createObject();

  OdDgTextNode2dPtr pTitleNode = OdDgTextNode2d::createObject();
  OdDgTextNode2dPtr pSubTitleNode = OdDgTextNode2d::createObject();

  pTitleNode->setHeightMultiplier(0.25);
  pTitleNode->setLengthMultiplier(0.25);
  pTitleNode->setColorIndex(3);
  pTitleNode->setLineSpacing(0.05);

  pSubTitleNode->setHeightMultiplier(0.25);
  pSubTitleNode->setLengthMultiplier(0.25);
  pSubTitleNode->setColorIndex(3);
  pSubTitleNode->setLineSpacing(0.05);

  pTitleText2d->set( pTitleNode, pSubTitleNode );

  OdGePoint2d ptOrigin( center.x, center.y );

  ptOrigin -= OdGeVector2d::kXAxis * (major * 3.0 / 8.0);
  ptOrigin -= OdGeVector2d::kYAxis * (minor / 8.0);

  pTitleText2d->setOrigin( ptOrigin );
  pTitleText2d->setTransformation( OdGeMatrix2d::rotation( OdaPI / 18.0) );

  OdDgText2dPtr pTitleLine1 = OdDgText2d::createObject();
  OdDgText2dPtr pTitleLine2_1 = OdDgText2d::createObject();
  OdDgText2dPtr pTitleLine2_2 = OdDgText2d::createObject();

  OdDgText2dPtr pSubTitleLine1_1 = OdDgText2d::createObject();
  OdDgText2dPtr pSubTitleLine1_2 = OdDgText2d::createObject();
  OdDgText2dPtr pSubTitleLine2 = OdDgText2d::createObject();

  pTitleLine1->setHeightMultiplier(0.2);
  pTitleLine1->setLengthMultiplier(0.2);
  pTitleLine1->setText(OD_T("Title:"));
  pTitleLine1->setColorIndex(3);
  pTitleLine1->setCrCount(1);

  pTitleLine2_1->setHeightMultiplier(0.15);
  pTitleLine2_1->setLengthMultiplier(0.15);
  pTitleLine2_1->setText(OD_T("Additional "));
  pTitleLine2_1->setColorIndex(2);
  pTitleLine2_1->setCrCount(0);

  pTitleLine2_2->setHeightMultiplier(0.15);
  pTitleLine2_2->setLengthMultiplier(0.15);
  pTitleLine2_2->setText(OD_T("info."));
  pTitleLine2_2->setColorIndex(18);
  pTitleLine2_2->setCrCount(0);

  pTitleNode->add( pTitleLine1 );
  pTitleNode->add( pTitleLine2_1 );
  pTitleNode->add( pTitleLine2_2 );

  pSubTitleLine1_1->setHeightMultiplier(0.2);
  pSubTitleLine1_1->setLengthMultiplier(0.2);
  pSubTitleLine1_1->setText(OD_T("Subtitle "));
  pSubTitleLine1_1->setColorIndex(3);
  pSubTitleLine1_1->setCrCount(0);

  pSubTitleLine1_2->setHeightMultiplier(0.2);
  pSubTitleLine1_2->setLengthMultiplier(0.2);
  pSubTitleLine1_2->setText(OD_T("text:"));
  pSubTitleLine1_2->setColorIndex(19);
  pSubTitleLine1_2->setCrCount(1);

  pSubTitleLine2->setHeightMultiplier(0.15);
  pSubTitleLine2->setLengthMultiplier(0.15);
  pSubTitleLine2->setText(OD_T("Present."));
  pSubTitleLine2->setColorIndex(2);
  pSubTitleLine2->setCrCount(0);

  pSubTitleNode->add( pSubTitleLine1_1 );
  pSubTitleNode->add( pSubTitleLine1_2 );
  pSubTitleNode->add( pSubTitleLine2 );

  pTitleText2d->setColorIndex(5);

  m_pModel2d->addElement( pTitleText2d );

  // Title Text 3d

  OdDgTitleText3dPtr pTitleText3d = OdDgTitleText3d::createObject();

  OdDgTextNode3dPtr pTitleNode3d = OdDgTextNode3d::createObject();
  OdDgTextNode3dPtr pSubTitleNode3d = OdDgTextNode3d::createObject();

  pTitleNode3d->setHeightMultiplier(0.25);
  pTitleNode3d->setLengthMultiplier(0.25);
  pTitleNode3d->setColorIndex(3);
  pTitleNode3d->setLineSpacing(0.05);

  pSubTitleNode3d->setHeightMultiplier(0.25);
  pSubTitleNode3d->setLengthMultiplier(0.25);
  pSubTitleNode3d->setColorIndex(3);
  pSubTitleNode3d->setLineSpacing(0.05);

  pTitleText3d->set( pTitleNode3d, pSubTitleNode3d );

  m_pModel3d->addElement( pTitleText3d );

  OdGePoint3d ptOrigin3d( center.x, center.y, 0 );

  ptOrigin3d -= OdGeVector3d::kXAxis * (major * 3.0 / 8.0);
  ptOrigin3d -= OdGeVector3d::kYAxis * (minor / 8.0);

  pTitleText3d->setOrigin( ptOrigin3d );

  OdGeMatrix3d matRotation = OdGeMatrix3d::rotation( OdaPI / 18, OdGeVector3d::kZAxis );

  pTitleText3d->setTransformation( matRotation );

  OdDgText3dPtr pTitle3dLine1 = OdDgText3d::createObject();
  OdDgText3dPtr pTitle3dLine2_1 = OdDgText3d::createObject();
  OdDgText3dPtr pTitle3dLine2_2 = OdDgText3d::createObject();

  OdDgText3dPtr pSubTitle3dLine1_1 = OdDgText3d::createObject();
  OdDgText3dPtr pSubTitle3dLine1_2 = OdDgText3d::createObject();
  OdDgText3dPtr pSubTitle3dLine2 = OdDgText3d::createObject();

  pTitle3dLine1->setHeightMultiplier(0.2);
  pTitle3dLine1->setLengthMultiplier(0.2);
  pTitle3dLine1->setText(OD_T("Title:"));
  pTitle3dLine1->setColorIndex(3);
  pTitle3dLine1->setCrCount(1);

  pTitle3dLine2_1->setHeightMultiplier(0.15);
  pTitle3dLine2_1->setLengthMultiplier(0.15);
  pTitle3dLine2_1->setText(OD_T("Additional "));
  pTitle3dLine2_1->setColorIndex(2);
  pTitle3dLine2_1->setCrCount(0);

  pTitle3dLine2_2->setHeightMultiplier(0.15);
  pTitle3dLine2_2->setLengthMultiplier(0.15);
  pTitle3dLine2_2->setText(OD_T("info."));
  pTitle3dLine2_2->setColorIndex(18);
  pTitle3dLine2_2->setCrCount(0);

  pTitleNode3d->add( pTitle3dLine1 );
  pTitleNode3d->add( pTitle3dLine2_1 );
  pTitleNode3d->add( pTitle3dLine2_2 );

  pSubTitle3dLine1_1->setHeightMultiplier(0.2);
  pSubTitle3dLine1_1->setLengthMultiplier(0.2);
  pSubTitle3dLine1_1->setText(OD_T("Subtitle "));
  pSubTitle3dLine1_1->setColorIndex(3);
  pSubTitle3dLine1_1->setCrCount(0);

  pSubTitle3dLine1_2->setHeightMultiplier(0.2);
  pSubTitle3dLine1_2->setLengthMultiplier(0.2);
  pSubTitle3dLine1_2->setText(OD_T("text:"));
  pSubTitle3dLine1_2->setColorIndex(19);
  pSubTitle3dLine1_2->setCrCount(1);

  pSubTitle3dLine2->setHeightMultiplier(0.15);
  pSubTitle3dLine2->setLengthMultiplier(0.15);
  pSubTitle3dLine2->setText(OD_T("Present."));
  pSubTitle3dLine2->setColorIndex(2);
  pSubTitle3dLine2->setCrCount(0);

  pSubTitleNode3d->add( pSubTitle3dLine1_1 );
  pSubTitleNode3d->add( pSubTitle3dLine1_2 );
  pSubTitleNode3d->add( pSubTitle3dLine2 );

  pTitleText3d->setColorIndex(5);
};


void OdExDgnFiller::addSectionMarker( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  // Section marker 2d

  OdDgSectionMarker2dPtr pSectionMarker2d = OdDgSectionMarker2d::createObject();

  OdDgTextNode2dPtr pTitleNode2d = OdDgTextNode2d::createObject();
  OdDgTextNode2dPtr pSubTitleNode2d = OdDgTextNode2d::createObject();
  OdDgTextNode2dPtr pReferenceNode2d = OdDgTextNode2d::createObject();
  OdDgTextNode2dPtr pSheetNode2d = OdDgTextNode2d::createObject();

  pTitleNode2d->setHeightMultiplier(0.25);
  pTitleNode2d->setLengthMultiplier(0.25);
  pTitleNode2d->setColorIndex(3);
  pTitleNode2d->setLineSpacing(0.05);

  pSubTitleNode2d->setHeightMultiplier(0.25);
  pSubTitleNode2d->setLengthMultiplier(0.25);
  pSubTitleNode2d->setColorIndex(3);
  pSubTitleNode2d->setLineSpacing(0.05);

  pReferenceNode2d->setHeightMultiplier(0.25);
  pReferenceNode2d->setLengthMultiplier(0.25);
  pReferenceNode2d->setColorIndex(3);
  pReferenceNode2d->setLineSpacing(0.05);

  pSheetNode2d->setHeightMultiplier(0.25);
  pSheetNode2d->setLengthMultiplier(0.25);
  pSheetNode2d->setColorIndex(3);
  pSheetNode2d->setLineSpacing(0.05);

  pSectionMarker2d->set( pTitleNode2d, pSubTitleNode2d, pReferenceNode2d, pSheetNode2d, 3.0 );

  OdGePoint2d ptOrigin2d( center.x, center.y );

  ptOrigin2d -= OdGeVector2d::kXAxis * (major * 3.0 / 8.0);
  ptOrigin2d -= OdGeVector2d::kYAxis * (minor / 8.0);

  m_pModel2d->addElement( pSectionMarker2d );

  pSectionMarker2d->setOrigin( ptOrigin2d );

  OdGeMatrix2d matRotation2d = OdGeMatrix2d::rotation( OdaPI / 18 );

  pSectionMarker2d->setTransformation( matRotation2d );

  OdDgText2dPtr pTitle2dLine1 = OdDgText2d::createObject();
  OdDgText2dPtr pTitle2dLine2_1 = OdDgText2d::createObject();
  OdDgText2dPtr pTitle2dLine2_2 = OdDgText2d::createObject();

  OdDgText2dPtr pSubTitle2dLine1_1 = OdDgText2d::createObject();
  OdDgText2dPtr pSubTitle2dLine1_2 = OdDgText2d::createObject();
  OdDgText2dPtr pSubTitle2dLine2 = OdDgText2d::createObject();

  pTitle2dLine1->setHeightMultiplier(0.2);
  pTitle2dLine1->setLengthMultiplier(0.2);
  pTitle2dLine1->setText(OD_T("Title text:"));
  pTitle2dLine1->setColorIndex(1);
  pTitle2dLine1->setCrCount(1);

  pTitle2dLine2_1->setHeightMultiplier(0.15);
  pTitle2dLine2_1->setLengthMultiplier(0.15);
  pTitle2dLine2_1->setText(OD_T("Sample "));
  pTitle2dLine2_1->setColorIndex(6);
  pTitle2dLine2_1->setCrCount(0);

  pTitle2dLine2_2->setHeightMultiplier(0.15);
  pTitle2dLine2_2->setLengthMultiplier(0.15);
  pTitle2dLine2_2->setText(OD_T("info."));
  pTitle2dLine2_2->setColorIndex(22);
  pTitle2dLine2_2->setCrCount(0);

  pTitleNode2d->add( pTitle2dLine1 );
  pTitleNode2d->add( pTitle2dLine2_1 );
  pTitleNode2d->add( pTitle2dLine2_2 );

  pSubTitle2dLine1_1->setHeightMultiplier(0.2);
  pSubTitle2dLine1_1->setLengthMultiplier(0.2);
  pSubTitle2dLine1_1->setText(OD_T("Subtitle "));
  pSubTitle2dLine1_1->setColorIndex(1);
  pSubTitle2dLine1_1->setCrCount(0);

  pSubTitle2dLine1_2->setHeightMultiplier(0.2);
  pSubTitle2dLine1_2->setLengthMultiplier(0.2);
  pSubTitle2dLine1_2->setText(OD_T("text:"));
  pSubTitle2dLine1_2->setColorIndex(17);
  pSubTitle2dLine1_2->setCrCount(1);

  pSubTitle2dLine2->setHeightMultiplier(0.15);
  pSubTitle2dLine2->setLengthMultiplier(0.15);
  pSubTitle2dLine2->setText(OD_T("Sample string."));
  pSubTitle2dLine2->setColorIndex(6);
  pSubTitle2dLine2->setCrCount(0);

  pSubTitleNode2d->add( pSubTitle2dLine1_1 );
  pSubTitleNode2d->add( pSubTitle2dLine1_2 );
  pSubTitleNode2d->add( pSubTitle2dLine2 );

  pSectionMarker2d->setColorIndex(12);

  OdDgText2dPtr pReference2dLine1 = OdDgText2d::createObject();
  OdDgText2dPtr pSheet2dLine1_1 = OdDgText2d::createObject();
  OdDgText2dPtr pSheet2dLine1_2 = OdDgText2d::createObject();

  pReference2dLine1->setHeightMultiplier(0.1);
  pReference2dLine1->setLengthMultiplier(0.1);
  pReference2dLine1->setText(OD_T("Reference. "));
  pReference2dLine1->setColorIndex(3);
  pReference2dLine1->setCrCount(0);

  pSheet2dLine1_1->setHeightMultiplier(0.07);
  pSheet2dLine1_1->setLengthMultiplier(0.07);
  pSheet2dLine1_1->setText(OD_T("Sheet "));
  pSheet2dLine1_1->setColorIndex(26);
  pSheet2dLine1_1->setCrCount(0);

  pSheet2dLine1_2->setHeightMultiplier(0.07);
  pSheet2dLine1_2->setLengthMultiplier(0.07);
  pSheet2dLine1_2->setText(OD_T("Model 1."));
  pSheet2dLine1_2->setColorIndex(26);
  pSheet2dLine1_2->setCrCount(0);

  pReferenceNode2d->add( pReference2dLine1 );
  pSheetNode2d->add( pSheet2dLine1_1 );
  pSheetNode2d->add( pSheet2dLine1_2 );

  // Section marker 3d

  OdDgSectionMarker3dPtr pSectionMarker3d = OdDgSectionMarker3d::createObject();

  OdDgTextNode3dPtr pTitleNode3d = OdDgTextNode3d::createObject();
  OdDgTextNode3dPtr pSubTitleNode3d = OdDgTextNode3d::createObject();
  OdDgTextNode3dPtr pReferenceNode3d = OdDgTextNode3d::createObject();
  OdDgTextNode3dPtr pSheetNode3d = OdDgTextNode3d::createObject();

  pTitleNode3d->setHeightMultiplier(0.25);
  pTitleNode3d->setLengthMultiplier(0.25);
  pTitleNode3d->setColorIndex(3);
  pTitleNode3d->setLineSpacing(0.05);

  pSubTitleNode3d->setHeightMultiplier(0.25);
  pSubTitleNode3d->setLengthMultiplier(0.25);
  pSubTitleNode3d->setColorIndex(3);
  pSubTitleNode3d->setLineSpacing(0.05);

  pReferenceNode3d->setHeightMultiplier(0.25);
  pReferenceNode3d->setLengthMultiplier(0.25);
  pReferenceNode3d->setColorIndex(3);
  pReferenceNode3d->setLineSpacing(0.05);

  pSheetNode3d->setHeightMultiplier(0.25);
  pSheetNode3d->setLengthMultiplier(0.25);
  pSheetNode3d->setColorIndex(3);
  pSheetNode3d->setLineSpacing(0.05);

  pSectionMarker3d->set( pTitleNode3d, pSubTitleNode3d, pReferenceNode3d, pSheetNode3d, 3.0 );

  OdGePoint3d ptOrigin3d( center.x, center.y, 0 );

  ptOrigin3d -= OdGeVector3d::kXAxis * (major * 3.0 / 8.0);
  ptOrigin3d -= OdGeVector3d::kYAxis * (minor / 8.0);

  pSectionMarker3d->setOrigin( ptOrigin3d );

  OdGeMatrix3d matRotation = OdGeMatrix3d::rotation( OdaPI / 18, OdGeVector3d::kZAxis );

  pSectionMarker3d->setTransformation( matRotation );

  OdDgText3dPtr pTitle3dLine1 = OdDgText3d::createObject();
  OdDgText3dPtr pTitle3dLine2_1 = OdDgText3d::createObject();
  OdDgText3dPtr pTitle3dLine2_2 = OdDgText3d::createObject();

  OdDgText3dPtr pSubTitle3dLine1_1 = OdDgText3d::createObject();
  OdDgText3dPtr pSubTitle3dLine1_2 = OdDgText3d::createObject();
  OdDgText3dPtr pSubTitle3dLine2 = OdDgText3d::createObject();

  pTitle3dLine1->setHeightMultiplier(0.2);
  pTitle3dLine1->setLengthMultiplier(0.2);
  pTitle3dLine1->setText(OD_T("Title text:"));
  pTitle3dLine1->setColorIndex(1);
  pTitle3dLine1->setCrCount(1);

  pTitle3dLine2_1->setHeightMultiplier(0.15);
  pTitle3dLine2_1->setLengthMultiplier(0.15);
  pTitle3dLine2_1->setText(OD_T("Sample "));
  pTitle3dLine2_1->setColorIndex(6);
  pTitle3dLine2_1->setCrCount(0);

  pTitle3dLine2_2->setHeightMultiplier(0.15);
  pTitle3dLine2_2->setLengthMultiplier(0.15);
  pTitle3dLine2_2->setText(OD_T("info."));
  pTitle3dLine2_2->setColorIndex(22);
  pTitle3dLine2_2->setCrCount(0);

  pTitleNode3d->add( pTitle3dLine1 );
  pTitleNode3d->add( pTitle3dLine2_1 );
  pTitleNode3d->add( pTitle3dLine2_2 );

  pSubTitle3dLine1_1->setHeightMultiplier(0.2);
  pSubTitle3dLine1_1->setLengthMultiplier(0.2);
  pSubTitle3dLine1_1->setText(OD_T("Subtitle "));
  pSubTitle3dLine1_1->setColorIndex(1);
  pSubTitle3dLine1_1->setCrCount(0);

  pSubTitle3dLine1_2->setHeightMultiplier(0.2);
  pSubTitle3dLine1_2->setLengthMultiplier(0.2);
  pSubTitle3dLine1_2->setText(OD_T("text:"));
  pSubTitle3dLine1_2->setColorIndex(17);
  pSubTitle3dLine1_2->setCrCount(1);

  pSubTitle3dLine2->setHeightMultiplier(0.15);
  pSubTitle3dLine2->setLengthMultiplier(0.15);
  pSubTitle3dLine2->setText(OD_T("Sample string."));
  pSubTitle3dLine2->setColorIndex(6);
  pSubTitle3dLine2->setCrCount(0);

  pSubTitleNode3d->add( pSubTitle3dLine1_1 );
  pSubTitleNode3d->add( pSubTitle3dLine1_2 );
  pSubTitleNode3d->add( pSubTitle3dLine2 );

  pSectionMarker3d->setColorIndex(12);

  OdDgText3dPtr pReference3dLine1 = OdDgText3d::createObject();
  OdDgText3dPtr pSheet3dLine1_1 = OdDgText3d::createObject();
  OdDgText3dPtr pSheet3dLine1_2 = OdDgText3d::createObject();

  pReference3dLine1->setHeightMultiplier(0.1);
  pReference3dLine1->setLengthMultiplier(0.1);
  pReference3dLine1->setText(OD_T("Reference. "));
  pReference3dLine1->setColorIndex(3);
  pReference3dLine1->setCrCount(0);

  pSheet3dLine1_1->setHeightMultiplier(0.07);
  pSheet3dLine1_1->setLengthMultiplier(0.07);
  pSheet3dLine1_1->setText(OD_T("Sheet "));
  pSheet3dLine1_1->setColorIndex(26);
  pSheet3dLine1_1->setCrCount(0);

  pSheet3dLine1_2->setHeightMultiplier(0.07);
  pSheet3dLine1_2->setLengthMultiplier(0.07);
  pSheet3dLine1_2->setText(OD_T("Model 1."));
  pSheet3dLine1_2->setColorIndex(26);
  pSheet3dLine1_2->setCrCount(0);

  pReferenceNode3d->add( pReference3dLine1 );
  pSheetNode3d->add( pSheet3dLine1_1 );
  pSheetNode3d->add( pSheet3dLine1_2 );

  m_pModel3d->addElement( pSectionMarker3d );
};

void OdExDgnFiller::addArrowMarker( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  // Arrow marker 2d

  OdDgArrowMarker2dPtr pArrowMarker2d = OdDgArrowMarker2d::createObject();

  OdDgTextNode2dPtr pReferenceNode2d = OdDgTextNode2d::createObject();
  OdDgTextNode2dPtr pSheetNode2d = OdDgTextNode2d::createObject();

  pReferenceNode2d->setHeightMultiplier(0.25);
  pReferenceNode2d->setLengthMultiplier(0.25);
  pReferenceNode2d->setColorIndex(3);
  pReferenceNode2d->setLineSpacing(0.05);

  pSheetNode2d->setHeightMultiplier(0.25);
  pSheetNode2d->setLengthMultiplier(0.25);
  pSheetNode2d->setColorIndex(3);
  pSheetNode2d->setLineSpacing(0.05);

  pArrowMarker2d->set( pReferenceNode2d, pSheetNode2d, 7.5, false );

  OdGePoint2d ptOrigin2d( center.x, center.y );

  ptOrigin2d -= OdGeVector2d::kXAxis * (major / 4.0);
  ptOrigin2d -= OdGeVector2d::kYAxis * (minor / 8.0);

  m_pModel2d->addElement( pArrowMarker2d );

  pArrowMarker2d->setOrigin( ptOrigin2d );

  OdGeMatrix2d matRotation2d = OdGeMatrix2d::rotation( OdaPI / 18 );

  pArrowMarker2d->setTransformation( matRotation2d );

  pArrowMarker2d->setColorIndex(12);

  OdDgText2dPtr pReference2dLine1 = OdDgText2d::createObject();
  OdDgText2dPtr pSheet2dLine1_1 = OdDgText2d::createObject();
  OdDgText2dPtr pSheet2dLine1_2 = OdDgText2d::createObject();

  pReference2dLine1->setHeightMultiplier(0.1);
  pReference2dLine1->setLengthMultiplier(0.1);
  pReference2dLine1->setText(OD_T("Reference. "));
  pReference2dLine1->setColorIndex(3);
  pReference2dLine1->setCrCount(0);

  pSheet2dLine1_1->setHeightMultiplier(0.1);
  pSheet2dLine1_1->setLengthMultiplier(0.1);
  pSheet2dLine1_1->setText(OD_T("Sheet "));
  pSheet2dLine1_1->setColorIndex(26);
  pSheet2dLine1_1->setCrCount(0);

  pSheet2dLine1_2->setHeightMultiplier(0.1);
  pSheet2dLine1_2->setLengthMultiplier(0.1);
  pSheet2dLine1_2->setText(OD_T("Model 1."));
  pSheet2dLine1_2->setColorIndex(26);
  pSheet2dLine1_2->setCrCount(0);

  pReferenceNode2d->add( pReference2dLine1 );
  pSheetNode2d->add( pSheet2dLine1_1 );
  pSheetNode2d->add( pSheet2dLine1_2 );

  pArrowMarker2d->addLeaderPoint( ptOrigin2d + OdGeVector2d::kXAxis * (major / 2.0) + OdGeVector2d::kYAxis * (minor / 2.0) );
  pArrowMarker2d->addLeaderPoint( ptOrigin2d + OdGeVector2d::kXAxis * (major * 2.0/ 3.0) - OdGeVector2d::kYAxis * (minor / 6.0) );

  // Arrow marker 3d

  OdDgArrowMarker3dPtr pArrowMarker3d = OdDgArrowMarker3d::createObject();

  OdDgTextNode3dPtr pReferenceNode3d = OdDgTextNode3d::createObject();
  OdDgTextNode3dPtr pSheetNode3d = OdDgTextNode3d::createObject();

  pReferenceNode3d->setHeightMultiplier(0.25);
  pReferenceNode3d->setLengthMultiplier(0.25);
  pReferenceNode3d->setColorIndex(3);
  pReferenceNode3d->setLineSpacing(0.05);

  pSheetNode3d->setHeightMultiplier(0.25);
  pSheetNode3d->setLengthMultiplier(0.25);
  pSheetNode3d->setColorIndex(3);
  pSheetNode3d->setLineSpacing(0.05);

  pArrowMarker3d->set( pReferenceNode3d, pSheetNode3d, 7.5, false );

  OdGePoint3d ptOrigin3d( center.x, center.y, 0 );

  ptOrigin3d -= OdGeVector3d::kXAxis * (major / 4.0);
  ptOrigin3d -= OdGeVector3d::kYAxis * (minor / 8.0);

  pArrowMarker3d->setOrigin( ptOrigin3d );

  OdGeMatrix3d matRotation3d = OdGeMatrix3d::rotation( OdaPI / 18, OdGeVector3d::kZAxis );

  pArrowMarker3d->setTransformation( matRotation3d );

  pArrowMarker3d->setColorIndex(12);

  OdDgText3dPtr pReference3dLine1 = OdDgText3d::createObject();
  OdDgText3dPtr pSheet3dLine1_1 = OdDgText3d::createObject();
  OdDgText3dPtr pSheet3dLine1_2 = OdDgText3d::createObject();

  pReference3dLine1->setHeightMultiplier(0.1);
  pReference3dLine1->setLengthMultiplier(0.1);
  pReference3dLine1->setText(OD_T("Reference. "));
  pReference3dLine1->setColorIndex(3);
  pReference3dLine1->setCrCount(0);

  pSheet3dLine1_1->setHeightMultiplier(0.1);
  pSheet3dLine1_1->setLengthMultiplier(0.1);
  pSheet3dLine1_1->setText(OD_T("Sheet "));
  pSheet3dLine1_1->setColorIndex(26);
  pSheet3dLine1_1->setCrCount(0);

  pSheet3dLine1_2->setHeightMultiplier(0.1);
  pSheet3dLine1_2->setLengthMultiplier(0.1);
  pSheet3dLine1_2->setText(OD_T("Model 1."));
  pSheet3dLine1_2->setColorIndex(26);
  pSheet3dLine1_2->setCrCount(0);

  pReferenceNode3d->add( pReference3dLine1 );
  pSheetNode3d->add( pSheet3dLine1_1 );
  pSheetNode3d->add( pSheet3dLine1_2 );

  pArrowMarker3d->addLeaderPoint( ptOrigin3d + OdGeVector3d::kXAxis * (major / 2.0) + OdGeVector3d::kYAxis * (minor / 2.0) );
  pArrowMarker3d->addLeaderPoint( ptOrigin3d + OdGeVector3d::kXAxis * (major * 2.0/ 3.0) - OdGeVector3d::kYAxis * (minor / 6.0) );


  m_pModel3d->addElement( pArrowMarker3d );
};

void OdExDgnFiller::addDetailMarker( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  // Detail marker 2d

  OdDgDetailMarker2dPtr pDetailMarker2d = OdDgDetailMarker2d::createObject();

  OdDgTextNode2dPtr pReferenceNode2d = OdDgTextNode2d::createObject();
  OdDgTextNode2dPtr pSheetNode2d = OdDgTextNode2d::createObject();

  pReferenceNode2d->setHeightMultiplier(0.25);
  pReferenceNode2d->setLengthMultiplier(0.25);
  pReferenceNode2d->setColorIndex(3);
  pReferenceNode2d->setLineSpacing(0.05);

  pSheetNode2d->setHeightMultiplier(0.25);
  pSheetNode2d->setLengthMultiplier(0.25);
  pSheetNode2d->setColorIndex(3);
  pSheetNode2d->setLineSpacing(0.05);

  pDetailMarker2d->set( pReferenceNode2d, pSheetNode2d, 7.5, 1.1 );

  OdGePoint2d ptOrigin2d( center.x, center.y );

  ptOrigin2d -= OdGeVector2d::kXAxis * (major / 4.0);
  ptOrigin2d -= OdGeVector2d::kYAxis * (minor / 8.0);

  m_pModel2d->addElement( pDetailMarker2d );

  pDetailMarker2d->setOrigin( ptOrigin2d );

  OdGeMatrix2d matRotation2d = OdGeMatrix2d::rotation( OdaPI / 18 );

  pDetailMarker2d->setTransformation( matRotation2d );

  pDetailMarker2d->setColorIndex(12);

  OdDgText2dPtr pReference2dLine1 = OdDgText2d::createObject();
  OdDgText2dPtr pSheet2dLine1_1 = OdDgText2d::createObject();
  OdDgText2dPtr pSheet2dLine1_2 = OdDgText2d::createObject();

  pReference2dLine1->setHeightMultiplier(0.1);
  pReference2dLine1->setLengthMultiplier(0.1);
  pReference2dLine1->setText(OD_T("Reference. "));
  pReference2dLine1->setColorIndex(3);
  pReference2dLine1->setCrCount(0);

  pSheet2dLine1_1->setHeightMultiplier(0.1);
  pSheet2dLine1_1->setLengthMultiplier(0.1);
  pSheet2dLine1_1->setText(OD_T("Sheet "));
  pSheet2dLine1_1->setColorIndex(26);
  pSheet2dLine1_1->setCrCount(0);

  pSheet2dLine1_2->setHeightMultiplier(0.1);
  pSheet2dLine1_2->setLengthMultiplier(0.1);
  pSheet2dLine1_2->setText(OD_T("Model 1."));
  pSheet2dLine1_2->setColorIndex(26);
  pSheet2dLine1_2->setCrCount(0);

  pReferenceNode2d->add( pReference2dLine1 );
  pSheetNode2d->add( pSheet2dLine1_1 );
  pSheetNode2d->add( pSheet2dLine1_2 );

  pDetailMarker2d->setBubblePoint( ptOrigin2d + OdGeVector2d::kXAxis * (major * 3.0/ 5.0));

  pDetailMarker2d->addLeaderPoint( ptOrigin2d + OdGeVector2d::kXAxis * (major / 3.0) + OdGeVector2d::kYAxis * (minor / 2.0) );

  // Detail marker 3d

  OdDgDetailMarker3dPtr pDetailMarker3d = OdDgDetailMarker3d::createObject();

  OdDgTextNode3dPtr pReferenceNode3d = OdDgTextNode3d::createObject();
  OdDgTextNode3dPtr pSheetNode3d = OdDgTextNode3d::createObject();

  pReferenceNode3d->setHeightMultiplier(0.25);
  pReferenceNode3d->setLengthMultiplier(0.25);
  pReferenceNode3d->setColorIndex(3);
  pReferenceNode3d->setLineSpacing(0.05);

  pSheetNode3d->setHeightMultiplier(0.25);
  pSheetNode3d->setLengthMultiplier(0.25);
  pSheetNode3d->setColorIndex(3);
  pSheetNode3d->setLineSpacing(0.05);

  pDetailMarker3d->set( pReferenceNode3d, pSheetNode3d, 7.5, 1.1 );

  OdGePoint3d ptOrigin3d = center;

  ptOrigin3d -= OdGeVector3d::kXAxis * (major / 4.0);
  ptOrigin3d -= OdGeVector3d::kYAxis * (minor / 8.0);

  pDetailMarker3d->setOrigin( ptOrigin3d );

  OdGeMatrix3d matRotation3d = OdGeMatrix3d::rotation( OdaPI / 18, OdGeVector3d::kZAxis );

  pDetailMarker3d->setTransformation( matRotation3d );

  pDetailMarker3d->setColorIndex(12);

  OdDgText3dPtr pReference3dLine1 = OdDgText3d::createObject();
  OdDgText3dPtr pSheet3dLine1_1 = OdDgText3d::createObject();
  OdDgText3dPtr pSheet3dLine1_2 = OdDgText3d::createObject();

  pReference3dLine1->setHeightMultiplier(0.1);
  pReference3dLine1->setLengthMultiplier(0.1);
  pReference3dLine1->setText(OD_T("Reference. "));
  pReference3dLine1->setColorIndex(3);
  pReference3dLine1->setCrCount(0);

  pSheet3dLine1_1->setHeightMultiplier(0.1);
  pSheet3dLine1_1->setLengthMultiplier(0.1);
  pSheet3dLine1_1->setText(OD_T("Sheet "));
  pSheet3dLine1_1->setColorIndex(26);
  pSheet3dLine1_1->setCrCount(0);

  pSheet3dLine1_2->setHeightMultiplier(0.1);
  pSheet3dLine1_2->setLengthMultiplier(0.1);
  pSheet3dLine1_2->setText(OD_T("Model 1."));
  pSheet3dLine1_2->setColorIndex(26);
  pSheet3dLine1_2->setCrCount(0);

  pReferenceNode3d->add( pReference3dLine1 );
  pSheetNode3d->add( pSheet3dLine1_1 );
  pSheetNode3d->add( pSheet3dLine1_2 );

  pDetailMarker3d->setBubblePoint( ptOrigin3d + OdGeVector3d::kXAxis * (major * 3.0/ 5.0));

  pDetailMarker3d->addLeaderPoint( ptOrigin3d + OdGeVector3d::kXAxis * (major / 3.0) + OdGeVector3d::kYAxis * (minor / 2.0) );

  m_pModel3d->addElement( pDetailMarker3d );
};

void OdExDgnFiller::createBox3d( OdGePoint3d ptCenter, 
                                   double dLength, 
                                     double dHeight, 
                                       double dDepth, 
                                         const OdGeMatrix3d& matRotation,
                                           OdDgModelPtr& pModel,
                                             OdUInt32 uColorIndex
                               )
{
  OdGeVector3d vrXOrd = OdGeVector3d::kXAxis;
  OdGeVector3d vrYOrd = OdGeVector3d::kYAxis;
  OdGeVector3d vrZOrd = OdGeVector3d::kZAxis;

  vrXOrd = vrXOrd.transformBy( matRotation );
  vrYOrd = vrYOrd.transformBy( matRotation );
  vrZOrd = vrZOrd.transformBy( matRotation );

  OdGePoint3d ptsLineString[4];

  ptsLineString[0] = ptCenter - vrZOrd * dDepth /2.0 - vrXOrd * dLength / 2.0 - vrYOrd * dHeight / 2.0;
  ptsLineString[1] = ptsLineString[0] + vrXOrd * dLength;
  ptsLineString[2] = ptsLineString[1] + vrYOrd * dHeight;
  ptsLineString[3] = ptsLineString[2] - vrXOrd * dLength;

  OdDgLineString3dPtr pLineStringBottom = OdDgLineString3d::createObject();

  pLineStringBottom->setColorIndex( uColorIndex );
  pLineStringBottom->addVertex( ptsLineString[0] );
  pLineStringBottom->addVertex( ptsLineString[1] );
  pLineStringBottom->addVertex( ptsLineString[2] );
  pLineStringBottom->addVertex( ptsLineString[3] );
  pLineStringBottom->addVertex( ptsLineString[0] );

  pModel->addElement( pLineStringBottom );

  for( OdUInt32 i = 0; i < 4; i++ )
  {
    OdDgLine3dPtr pLine = OdDgLine3d::createObject();
    pLine->setColorIndex( uColorIndex );
    pLine->setStartPoint( ptsLineString[i] );
    pLine->setEndPoint( ptsLineString[i] + vrZOrd * dDepth );

    pModel->addElement( pLine );

    ptsLineString[i] = ptsLineString[i] + vrZOrd * dDepth;
  }

  OdDgLineString3dPtr pLineStringTop = OdDgLineString3d::createObject();

  pLineStringTop->setColorIndex( uColorIndex );
  pLineStringTop->addVertex( ptsLineString[0] );
  pLineStringTop->addVertex( ptsLineString[1] );
  pLineStringTop->addVertex( ptsLineString[2] );
  pLineStringTop->addVertex( ptsLineString[3] );
  pLineStringTop->addVertex( ptsLineString[0] );

  pModel->addElement( pLineStringTop );
}

void OdExDgnFiller::addSectionClip( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdGeMatrix3d matRotation = OdGeMatrix3d::kIdentity;

  OdDgSectionClipPtr pSectionClip = OdDgSectionClip::createObject();
  pSectionClip->setOrigin( center - OdGeVector3d::kXAxis * major * 9.0 / 20.0 );
  pSectionClip->setPerspectiveUpFlag(true);
  pSectionClip->setRightCropFlag(true);
  pSectionClip->setLeftCropFlag(true);
  pSectionClip->setTopCropFlag(false);
  pSectionClip->setBottomCropFlag(false);
  pSectionClip->setFrontCropFlag(true);
  pSectionClip->setBackCropFlag(true);
  pSectionClip->setName(L"Section Clip");
  pSectionClip->setRotation( matRotation );
  pSectionClip->setClipWidth( major * 9.0 / 10.0 );
  pSectionClip->setTopHeight( minor * 2.0 / 5.0 );
  pSectionClip->setBottomHeight( - minor * 3.0 / 10.0 );
  pSectionClip->setFrontDepth( - minor * 2.0 / 5.0 );
  pSectionClip->setBackDepth( minor * 1.0 / 5.0 );

  OdGePoint3dArray arrClipBox;

  pSectionClip->getClipBox( arrClipBox );

  OdGePoint3d ptClipCenter;

  ptClipCenter.x = (arrClipBox[0].x + arrClipBox[6].x) / 2.0;
  ptClipCenter.y = (arrClipBox[0].y + arrClipBox[6].y) / 2.0;
  ptClipCenter.z = (arrClipBox[0].z + arrClipBox[6].z) / 2.0;

  double dBigLength = arrClipBox[0].distanceTo( arrClipBox[1] ) * 9.0 / 10.0;
  double dBigHeight = arrClipBox[0].distanceTo( arrClipBox[3] ) * 9.0 / 10.0;
  double dBigDepth  = arrClipBox[0].distanceTo( arrClipBox[4] ) * 9.0 / 10.0;

  createBox3d( ptClipCenter, dBigLength, dBigHeight, dBigDepth, matRotation, m_pModel3d, 1 );

  OdGePoint3d ptBoxCenter = ptClipCenter + OdGeVector3d::kXAxis * ( dBigLength / 2.0 );

  createBox3d( ptBoxCenter, dBigLength / 5.0, dBigHeight / 5.0, dBigDepth / 5.0, matRotation, m_pModel3d, 2 );

  ptBoxCenter = ptClipCenter - OdGeVector3d::kXAxis * ( dBigLength / 2.0 );

  createBox3d( ptBoxCenter, dBigLength / 5.0, dBigHeight / 5.0, dBigDepth / 5.0, matRotation, m_pModel3d, 3 );

  ptBoxCenter = ptClipCenter + OdGeVector3d::kYAxis * ( dBigHeight / 2.0 );

  createBox3d( ptBoxCenter, dBigLength / 5.0, dBigHeight / 5.0, dBigDepth / 5.0, matRotation, m_pModel3d, 4 );

  ptBoxCenter = ptClipCenter - OdGeVector3d::kYAxis * ( dBigHeight / 2.0 );

  createBox3d( ptBoxCenter, dBigLength / 5.0, dBigHeight / 5.0, dBigDepth / 5.0, matRotation, m_pModel3d, 5 );

  ptBoxCenter = ptClipCenter + OdGeVector3d::kZAxis * ( dBigDepth  / 2.0 );

  createBox3d( ptBoxCenter, dBigLength / 5.0, dBigHeight / 5.0, dBigDepth / 5.0, matRotation, m_pModel3d, 6 );

  ptBoxCenter = ptClipCenter - OdGeVector3d::kZAxis * ( dBigDepth / 2.0 );

  createBox3d( ptBoxCenter, dBigLength / 5.0, dBigHeight / 5.0, dBigDepth / 5.0, matRotation, m_pModel3d, 7 );

  m_pModel3d->addElement( pSectionClip );

  // Add volume clipping to view number 1 of first 3d model.

  OdDgDatabase* pDb = m_pModel3d->database();

  if( pDb )
  {
    OdDgViewGroupTablePtr pViewGroupTable = pDb->getViewGroupTable( OdDg::kForWrite );

    if( !pViewGroupTable.isNull() )
    {
      OdDgElementIteratorPtr pViewGroupIter = pViewGroupTable->createIterator();

      OdDgViewGroupPtr pViewGroup = pViewGroupIter->item().openObject(OdDg::kForWrite);

      if( !pViewGroup.isNull() )
      {
        OdDgElementIteratorPtr pViewIter = pViewGroup->createIterator();

        pViewIter->step();

        OdDgViewPtr pView = pViewIter->item().openObject( OdDg::kForWrite );

        if( !pView.isNull() )
        {
          pView->setVolumeClipElementId( pSectionClip->id() );
          pView->setShowClipVolumeFlag(true);
        }
      }
    }
  }
}

void OdExDgnFiller::addAnnotationCell( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgAnnotationCellHeader3dPtr pAnnotationCell3d = OdDgAnnotationCellHeader3d::createObject();

  m_pModel3d->addElement( pAnnotationCell3d );

  pAnnotationCell3d->setOrigin( OdGePoint3d(minor/2,minor/2,0) );

  OdDgLineString3dPtr pLineStr3d = OdDgLineString3d::createObject();

  pLineStr3d->addVertex(OdGePoint3d(0,minor/4,0) );
  pLineStr3d->addVertex(OdGePoint3d(minor,minor/4,0) );
  pLineStr3d->addVertex(OdGePoint3d(minor,minor*3/4,0) );
  pLineStr3d->addVertex(OdGePoint3d(0,minor*3/4,0) );
  pLineStr3d->addVertex(OdGePoint3d(0,minor/4,0) );

  pAnnotationCell3d->add( pLineStr3d );

  OdDgEllipse3dPtr pEllipse3d = OdDgEllipse3d::createObject();

  pEllipse3d->setOrigin(OdGePoint3d(minor/2,minor/2,0));
  pEllipse3d->setPrimaryAxis(minor/2);
  pEllipse3d->setSecondaryAxis(minor/4);

  pAnnotationCell3d->add(pEllipse3d);

  OdDgLine3dPtr pHorLine3d = OdDgLine3d::createObject();
  pHorLine3d->setStartPoint(OdGePoint3d(0,minor/2,0));
  pHorLine3d->setEndPoint(OdGePoint3d(minor,minor/2,0));

  pAnnotationCell3d->add( pHorLine3d );

  OdDgLine3dPtr pVerLine3d = OdDgLine3d::createObject();
  pVerLine3d->setStartPoint(OdGePoint3d(minor/2,minor/4,0));
  pVerLine3d->setEndPoint(OdGePoint3d(minor/2,minor * 3/4,0));

  pAnnotationCell3d->add( pVerLine3d );

  OdGeMatrix3d matTranslate = OdGeMatrix3d::translation( center - OdGePoint3d(minor/2,minor/2,0) - OdGeVector3d::kXAxis * minor / 2.0 );

  pAnnotationCell3d->transformBy( matTranslate );

  OdGeMatrix3d matRotation  = OdGeMatrix3d::rotation( OdaPI/6, OdGeVector3d::kZAxis);

  pAnnotationCell3d->setTransformation( matRotation * OdGeMatrix3d::scaling(0.08));
  pAnnotationCell3d->setUseAnnotationScaleFlag(true);
  pAnnotationCell3d->setAnnotationScale(5.0);

  OdDgAnnotationCellHeader3dPtr pCellClone = pAnnotationCell3d->clone();

  m_pModel3d->addElement(pCellClone);

  OdGeMatrix3d matTranslateClone = OdGeMatrix3d::translation( OdGeVector3d::kXAxis * major / 2.0 );

  pCellClone->transformBy(matTranslateClone);

  pCellClone->setAnnotationScale( 10.0 );

  // Add annotation cell 2d

  OdDgAnnotationCellHeader2dPtr pAnnotationCell2d = OdDgAnnotationCellHeader2d::createObject();

  m_pModel2d->addElement( pAnnotationCell2d );

  pAnnotationCell2d->setOrigin( OdGePoint2d(minor/2,minor/2) );

  OdDgLineString2dPtr pLineStr2d = OdDgLineString2d::createObject();

  pLineStr2d->addVertex(OdGePoint2d(0,minor/4) );
  pLineStr2d->addVertex(OdGePoint2d(minor,minor/4) );
  pLineStr2d->addVertex(OdGePoint2d(minor,minor*3/4) );
  pLineStr2d->addVertex(OdGePoint2d(0,minor*3/4) );
  pLineStr2d->addVertex(OdGePoint2d(0,minor/4) );

  pAnnotationCell2d->add( pLineStr2d );

  OdDgEllipse2dPtr pEllipse2d = OdDgEllipse2d::createObject();

  pEllipse2d->setOrigin(OdGePoint2d(minor/2,minor/2));
  pEllipse2d->setPrimaryAxis(minor/2);
  pEllipse2d->setSecondaryAxis(minor/4);

  pAnnotationCell2d->add(pEllipse2d);

  OdDgLine2dPtr pHorLine2d = OdDgLine2d::createObject();
  pHorLine2d->setStartPoint(OdGePoint2d(0,minor/2));
  pHorLine2d->setEndPoint(OdGePoint2d(minor,minor/2));

  pAnnotationCell2d->add( pHorLine2d );

  OdDgLine2dPtr pVerLine2d = OdDgLine2d::createObject();
  pVerLine2d->setStartPoint(OdGePoint2d(minor/2,minor/4));
  pVerLine2d->setEndPoint(OdGePoint2d(minor/2,minor * 3/4));

  pAnnotationCell2d->add( pVerLine2d );

  OdGeMatrix3d matTranslateFor2d = OdGeMatrix3d::translation( center - OdGePoint3d(minor/2,minor/2,0) - OdGeVector3d::kXAxis * minor / 2.0 );

  pAnnotationCell2d->transformBy( matTranslateFor2d );

  OdGeMatrix2d matRotation2d  = OdGeMatrix2d::rotation( OdaPI/6);

  pAnnotationCell2d->setTransformation( matRotation2d * OdGeMatrix2d::scaling(0.08));
  pAnnotationCell2d->setUseAnnotationScaleFlag(true);
  pAnnotationCell2d->setAnnotationScale(5.0);

  OdDgAnnotationCellHeader2dPtr pCell2dClone = pAnnotationCell2d->clone();

  m_pModel2d->addElement(pCell2dClone);

  OdGeMatrix3d matTranslateClone2d = OdGeMatrix3d::translation( OdGeVector3d::kXAxis * major / 2.0 );

  pCell2dClone->transformBy(matTranslateClone2d);

  pCell2dClone->setAnnotationScale( 10.0 );
}

void OdExDgnFiller::addLineWithTemp( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgLine3dPtr pLine = OdDgLine3d::createObject();
  OdGePoint3d ptStart = center - OdGeVector3d::kXAxis * major * 3.0 / 8.0 - OdGeVector3d::kYAxis * minor * 3.0 / 8.0;
  OdGePoint3d ptEnd   = center + OdGeVector3d::kXAxis * major * 3.0 / 8.0 + OdGeVector3d::kYAxis * minor * 3.0 / 8.0;
  pLine->setStartPoint( ptStart );
  pLine->setEndPoint( ptEnd );

  m_pModel3d->addElement( pLine );

  // Apply element template

  OdDgElementTemplateTablePtr pTemplateTable = m_pModel3d->database()->getElementTemplateTable(OdDg::kForRead);

  OdDgElementId idTemplate = pTemplateTable->getAt(L"Colors");

  if( !idTemplate.isNull() )
  {
    // correct element properties.
    //...
    // add template dependence.

    OdDgTemplateIdXAttributePtr pTemplateDep = OdDgTemplateIdXAttribute::createObject();

    pTemplateDep->setTemplateId( (OdUInt64)(idTemplate.getHandle()) );

    pLine->addXAttribute( OdDgTemplateIdXAttribute::kType, pTemplateDep.get() );
  }
  else
  {
    ODA_FAIL_ONCE();
  }
}

void OdExDgnFiller::addLineStyleLines( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d boxCenter3d = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgLineStyleTablePtr pLineStyleTable = m_pModel3d->database()->getLineStyleTable(OdDg::kForRead);

  OdDgElementId idLineStyle = pLineStyleTable->getAt(OD_T("{ Smiley }"));

  OdDgLineStyleTableRecordPtr pSmileyLineStyle = idLineStyle.openObject(OdDg::kForRead);

  OdDgLine3dPtr pLine3d;

  pLine3d = OdDgLine3d::createObject();

  pLine3d->setStartPoint( OdGePoint3d( boxCenter3d.x - major * 3 / 8, boxCenter3d.y - minor * 3.0 / 10.0, 0 ) );
  pLine3d->setEndPoint( OdGePoint3d( boxCenter3d.x + major * 3 / 8, boxCenter3d.y - minor * 3.0 / 10.0, 0 ) );
  pLine3d->setLineStyleId( idLineStyle );
  pLine3d->setLineStyleScale(0.2);
  m_pModel3d->addElement( pLine3d );

  pLine3d = OdDgLine3d::createObject();

  pLine3d->setStartPoint( OdGePoint3d( boxCenter3d.x - major * 3 / 8, boxCenter3d.y - minor * 1.0 / 10.0, 0 ) );
  pLine3d->setEndPoint( OdGePoint3d( boxCenter3d.x + major * 3 / 8, boxCenter3d.y - minor * 1.0 / 10.0, 0 ) );
  pLine3d->setLineStyleEntryId( pSmileyLineStyle->getEntryId() );
  pLine3d->setLineStyleScale(0.05);
  m_pModel3d->addElement( pLine3d );
}

void OdExDgnFiller::addOleFrame( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d boxCenter3d = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  // Add ole frame 3d

  OdGePoint3d ptPosition = boxCenter3d - OdGeVector3d::kXAxis * major * 0.4 - OdGeVector3d::kYAxis * major * 0.04;

  OdGeMatrix3d matPosOffset;

  matPosOffset.setToRotation(OdaPI/6.0, OdGeVector3d::kZAxis, boxCenter3d );

  ptPosition = ptPosition.transformBy( matPosOffset );

  OdDgOle2Frame3dPtr pOleFrame3d = OdDgOle2Frame3d::createObject();
  pOleFrame3d->setPosition( ptPosition );
  pOleFrame3d->setCanBePictureFlag( true );
  pOleFrame3d->setCanBeLinkedAndEmbeddedFlag( false );
  pOleFrame3d->setDrawAspect( OdDgOle2Frame::kContent);
  pOleFrame3d->setOleType(OdDgOle2Frame::kPicture);
  pOleFrame3d->setTransparentBackgroundFlag(false);
  pOleFrame3d->setViewRotationMode(OdDgOle2Frame::kViewDependent);
  pOleFrame3d->setObjectSizeX(1.0);
  pOleFrame3d->setObjectSizeY(0.1);
  pOleFrame3d->setScale( major * 0.8, major * 0.8 );

  OdGeQuaternion quatRotation;
  OdGeMatrix3d   matRotation;
  matRotation.setToRotation(OdaPI/6.0,OdGeVector3d::kZAxis);
  quatRotation.set( matRotation );

  pOleFrame3d->setRotation( quatRotation );

  OdStreamBufPtr pBuf = OdMemoryStream::createNew();
  pBuf->putBytes(m_uOleDataArr, sizeof(m_uOleDataArr));
  pBuf->rewind();

  pOleFrame3d->setCompoundDocument( sizeof(m_uOleDataArr), *pBuf );

  m_pModel3d->addElement( pOleFrame3d );

  // Add ole frame 2d

  OdGePoint2d boxCenter2d( boxCenter3d.x, boxCenter3d.y ); 

  OdGePoint2d ptPosition2d = boxCenter2d - OdGeVector2d::kXAxis * major * 0.4 - OdGeVector2d::kYAxis * major * 0.04;

  OdGeMatrix2d matPosOffset2d;

  matPosOffset2d.setToRotation(OdaPI/6.0, boxCenter2d );

  ptPosition2d = ptPosition2d.transformBy( matPosOffset2d );

  OdDgOle2Frame2dPtr pOleFrame2d = OdDgOle2Frame2d::createObject();
  pOleFrame2d->setPosition( ptPosition2d );
  pOleFrame2d->setCanBePictureFlag( true );
  pOleFrame2d->setCanBeLinkedAndEmbeddedFlag( false );
  pOleFrame2d->setDrawAspect( OdDgOle2Frame::kContent);
  pOleFrame2d->setOleType(OdDgOle2Frame::kPicture);
  pOleFrame2d->setTransparentBackgroundFlag(false);
  pOleFrame2d->setViewRotationMode(OdDgOle2Frame::kViewDependent);
  pOleFrame2d->setObjectSizeX(1.0);
  pOleFrame2d->setObjectSizeY(0.1);
  pOleFrame2d->setScale( major * 0.8, major * 0.8 );

  pOleFrame2d->setRotationAngle( OdaPI/6.0 );

  OdStreamBufPtr pBuf2d = OdMemoryStream::createNew();
  pBuf2d->putBytes(m_uOleDataArr, sizeof(m_uOleDataArr));
  pBuf2d->rewind();

  pOleFrame2d->setCompoundDocument( sizeof(m_uOleDataArr), *pBuf2d );

  m_pModel2d->addElement( pOleFrame2d );
}

void OdExDgnFiller::addNamedGroup()
{
  OdDgNamedGroupHeaderPtr pNamedGroup = OdDgNamedGroupHeader::createObject();

  m_pModel3d->addElement( pNamedGroup );

  OdDgNamedGroupItem namedGroupItem;
  OdArray<OdUInt64>  namedGroupItemPath;

  pNamedGroup->setName(L"Sample named group");
  pNamedGroup->setDescription(L"Named group contains elements from current model and from XRef.");
  pNamedGroup->setAllowDuplicatesFlag(true);
  pNamedGroup->setAllowFarReferencesFlag(true);
  pNamedGroup->setClosedGroupFlag(false);
  pNamedGroup->setExclusiveMembersFlag(false);
  pNamedGroup->setSelectMembersFlag(true);
  pNamedGroup->setAnonimusFlag(false);

  // add cone to group

  namedGroupItem.setForwardPropagate( OdDgNamedGroupItem::kApplyIfGroupLockOn);
  namedGroupItem.setBackwardPropagate( OdDgNamedGroupItem::kApplyIfGroupLockOn);
  namedGroupItem.setGroupPropagate( OdDgNamedGroupItem::kApplyIfGroupLockOn);
  namedGroupItemPath.push_back( m_uNamedGroupElementArr[0]);
  namedGroupItem.setPath( namedGroupItemPath );

  pNamedGroup->addNamedGroupItem( namedGroupItem );

  // add text to group

  namedGroupItem.setForwardPropagate( OdDgNamedGroupItem::kApplyAlways);
  namedGroupItem.setBackwardPropagate( OdDgNamedGroupItem::kApplyAlways);
  namedGroupItem.setGroupPropagate( OdDgNamedGroupItem::kApplyAlways);
  namedGroupItemPath.clear();
  namedGroupItemPath.push_back( m_uNamedGroupElementArr[1]);
  namedGroupItem.setPath( namedGroupItemPath );

  pNamedGroup->addNamedGroupItem( namedGroupItem );

  // add grouped hole to group

  namedGroupItem.setForwardPropagate( OdDgNamedGroupItem::kApplyNever);
  namedGroupItem.setBackwardPropagate( OdDgNamedGroupItem::kApplyNever);
  namedGroupItem.setGroupPropagate( OdDgNamedGroupItem::kApplyNever);
  namedGroupItemPath.clear();
  namedGroupItemPath.push_back( m_uNamedGroupElementArr[2]);
  namedGroupItem.setPath( namedGroupItemPath );

  pNamedGroup->addNamedGroupItem( namedGroupItem );

  // add mesh to group

  namedGroupItem.setForwardPropagate( OdDgNamedGroupItem::kApplyNever);
  namedGroupItem.setBackwardPropagate( OdDgNamedGroupItem::kApplyAlways);
  namedGroupItem.setGroupPropagate( OdDgNamedGroupItem::kApplyIfGroupLockOn);
  namedGroupItemPath.clear();
  namedGroupItemPath.push_back( m_uNamedGroupElementArr[3]);
  namedGroupItem.setPath( namedGroupItemPath );

  pNamedGroup->addNamedGroupItem( namedGroupItem );

  // add line string from XRef to group

  namedGroupItem.setForwardPropagate( OdDgNamedGroupItem::kApplyIfGroupLockOn);
  namedGroupItem.setBackwardPropagate( OdDgNamedGroupItem::kApplyAlways);
  namedGroupItem.setGroupPropagate( OdDgNamedGroupItem::kApplyIfGroupLockOn);
  namedGroupItemPath.clear();
  namedGroupItemPath.push_back( 0x4B );
  namedGroupItemPath.push_back( m_uNamedGroupElementArr[4]);
  namedGroupItem.setPath( namedGroupItemPath );

  pNamedGroup->addNamedGroupItem( namedGroupItem );

  // add line from XRef to group

  namedGroupItem.setForwardPropagate( OdDgNamedGroupItem::kApplyIfGroupLockOn);
  namedGroupItem.setBackwardPropagate( OdDgNamedGroupItem::kApplyNever);
  namedGroupItem.setGroupPropagate( OdDgNamedGroupItem::kApplyNever);
  namedGroupItemPath.clear();
  namedGroupItemPath.push_back( 0x4F );
  namedGroupItemPath.push_back( m_uNamedGroupElementArr[4]);
  namedGroupItem.setPath( namedGroupItemPath );

  pNamedGroup->addNamedGroupItem( namedGroupItem );
}

//---------------------------------------------------------------------------------------------

void OdExDgnFiller::addTerrain( int boxRow, int boxColumn )
{
  // Terrain data

  const OdGePoint3d terrainPts[] =
  {
    OdGePoint3d(0.0 ,0.0 ,0.0 ),   // 0
    OdGePoint3d(0.0 ,26.0,0.0 ),   // 1
    OdGePoint3d(26.0,26.0,0.0 ),   // 2
    OdGePoint3d(26.0,0.0 ,0.0 ),   // 3
    OdGePoint3d(1.0 ,1.0 ,1.0 ),   // 4
    OdGePoint3d(1.0 ,25.0,1.0 ),   // 5
    OdGePoint3d(25.0,25.0,1.0 ),   // 6
    OdGePoint3d(25.0,1.0 ,1.0 ),   // 7
    OdGePoint3d(2.0 ,2.0 ,1.0 ),   // 8
    OdGePoint3d(2.0 ,24.0,1.0 ),   // 9
    OdGePoint3d(24.0,24.0,1.0 ),   // 10
    OdGePoint3d(24.0,2.0 ,1.0 ),   // 11
    OdGePoint3d(3.0 ,3.0 ,3.0 ),   // 12
    OdGePoint3d(3.0 ,23.0,3.0 ),   // 13
    OdGePoint3d(23.0,23.0,3.0 ),   // 14
    OdGePoint3d(23.0,3.0 ,3.0 ),   // 15
    OdGePoint3d(4.0 ,4.0 ,1.0 ),   // 16
    OdGePoint3d(4.0 ,22.0,1.0 ),   // 17
    OdGePoint3d(22.0,22.0,1.0 ),   // 18
    OdGePoint3d(22.0,4.0 ,1.0 ),   // 19
    OdGePoint3d(5.0 ,5.0 ,1.0 ),   // 20
    OdGePoint3d(5.0 ,21.0,1.0 ),   // 21
    OdGePoint3d(21.0,21.0,1.0 ),   // 22
    OdGePoint3d(21.0,5.0 ,1.0 ),   // 23
    OdGePoint3d(6.0 ,6.0 ,0.0 ),   // 24
    OdGePoint3d(6.0 ,20.0,0.0 ),   // 25
    OdGePoint3d(20.0,20.0,0.0 ),   // 26
    OdGePoint3d(20.0,6.0 ,0.0 ),   // 27
    OdGePoint3d(8.0 ,8.0 ,0.0 ),   // 28
    OdGePoint3d(8.0 ,18.0,0.0 ),   // 29
    OdGePoint3d(18.0,18.0,0.0 ),   // 30
    OdGePoint3d(18.0,8.0 ,0.0 ),   // 31
    OdGePoint3d(9.0 ,9.0 ,2.0 ),   // 32
    OdGePoint3d(9.0 ,17.0,2.0 ),   // 33
    OdGePoint3d(17.0,17.0,2.0 ),   // 34
    OdGePoint3d(17.0,9.0 ,2.0 ),   // 35
    OdGePoint3d(11.0,11.0,3.0 ),   // 36
    OdGePoint3d(11.0,15.0,3.0 ),   // 37
    OdGePoint3d(15.0,15.0,3.0 ),   // 38
    OdGePoint3d(15.0,11.0,3.0 ),   // 39
    OdGePoint3d(12.0,12.0,8.0 ),   // 40
    OdGePoint3d(12.0,14.0,8.0 ),   // 41
    OdGePoint3d(14.0,14.0,8.0 ),   // 42
    OdGePoint3d(14.0,12.0,8.0 ),   // 43
    OdGePoint3d(13.0,13.0,10.0)    // 44
  };

  const OdDgTerrainTriangle arrTriangles[] = 
  {
    {0 ,5 ,1 ,true}, {0 ,4 ,5 ,true}, {5 ,2 ,1 ,true}, {5 ,6 ,2 ,true}, {7 ,2 ,6 ,true}, {7 ,3 ,2 ,true}, {0 ,7 ,4 ,true}, {0 ,3 ,7 ,true},
    {4 ,9 ,5 ,true}, {4 ,8 ,9 ,true}, {9 ,6 ,5 ,true}, {9 ,10,6 ,true}, {11,6 ,10,true}, {11,7 ,6 ,true}, {4 ,11,8 ,true}, {4 ,7 ,11,true},
    {8 ,13,9 ,true}, {8 ,12,13,true}, {13,10,9 ,true}, {13,14,10,true}, {15,10,14,true}, {15,11,10,true}, {8 ,15,12,true}, {8 ,11,15,true},
    {12,17,13,true}, {12,16,17,true}, {17,14,13,true}, {17,18,14,true}, {19,14,18,true}, {19,15,14,true}, {12,19,16,true}, {12,15,19,true},
    {16,21,17,true}, {16,20,21,true}, {21,18,17,true}, {21,22,18,true}, {23,18,22,true}, {23,19,18,true}, {16,23,20,true}, {16,19,23,true},
    {20,25,21,true}, {20,24,25,true}, {25,22,21,true}, {25,26,22,true}, {27,22,26,true}, {27,23,22,true}, {20,27,24,true}, {20,23,27,true},
    {24,29,25,false}, {24,28,29,false}, {29,26,25,false}, {29,30,26,false}, {31,26,30,false}, {31,27,26,false}, {24,31,28,false}, {24,27,31,false},
    {28,33,29,true}, {28,32,33,true}, {33,30,29,true}, {33,34,30,true}, {35,30,34,true}, {35,31,30,true}, {28,35,32,true}, {28,31,35,true},
    {32,37,33,true}, {32,36,37,true}, {37,34,33,true}, {37,38,34,true}, {39,34,38,true}, {39,35,34,true}, {32,39,36,true}, {32,35,39,true},
    {36,41,37,true}, {36,40,41,true}, {41,38,37,true}, {41,42,38,true}, {43,38,42,true}, {43,39,38,true}, {36,43,40,true}, {36,39,43,true},
    {40,44,41,true}, {41,44,42,true}, {42,44,43,true}, {40,43,44,true}
  };

  // Create terrain control element ( geometry of terrain ).

  OdDgTerrainControlElementPtr pTerrainControl = OdDgTerrainControlElement::createObject();

  // Set terrain vertices.

  OdGePoint3dArray arrTerrainPts;

  for( OdUInt32 i = 0; i < sizeof(terrainPts) / sizeof(OdGePoint3d); i++ )
  {
    arrTerrainPts.push_back( terrainPts[i] );
  }

  pTerrainControl->setTerrainVertices( arrTerrainPts );

  // Set terrain triangles.

  for( OdUInt32 j = 0; j < sizeof(arrTriangles) / sizeof(OdDgTerrainTriangle); j++ )
  {
    pTerrainControl->addTriangle( arrTriangles[j] );
  }

  // Set terrain features.

  OdDgTerrainFeature breakLine;
  OdDgTerrainFeature voidLine;
  OdDgTerrainFeature islandLine;

  OdUInt32Array arrBreakLineIndexes;
  arrBreakLineIndexes.push_back(12);
  arrBreakLineIndexes.push_back(13);
  arrBreakLineIndexes.push_back(14);
  arrBreakLineIndexes.push_back(15);
  arrBreakLineIndexes.push_back(12);

  OdUInt32Array arrVoidIndexes;
  arrVoidIndexes.push_back(24);
  arrVoidIndexes.push_back(25);
  arrVoidIndexes.push_back(26);
  arrVoidIndexes.push_back(27);
  arrVoidIndexes.push_back(24);

  OdUInt32Array arrIslandIndexes;
  arrIslandIndexes.push_back(28);
  arrIslandIndexes.push_back(31);
  arrIslandIndexes.push_back(30);
  arrIslandIndexes.push_back(29);
  arrIslandIndexes.push_back(28);

  breakLine.setFeatureNumber(0);
  breakLine.setFeatureType( OdDgTerrainFeature::kBreakLine );
  breakLine.setCorruptedFeatureTypeFlag( false, arrTerrainPts );
  breakLine.setFeatureVertexIndexes( arrBreakLineIndexes );

  pTerrainControl->addFeature( breakLine );

  voidLine.setFeatureNumber(1);
  voidLine.setFeatureType( OdDgTerrainFeature::kVoid );
  voidLine.setCorruptedFeatureTypeFlag( false, arrTerrainPts );
  voidLine.setFeatureVertexIndexes( arrVoidIndexes );

  pTerrainControl->addFeature( voidLine );

  islandLine.setFeatureNumber(2);
  islandLine.setFeatureType( OdDgTerrainFeature::kIsland );
  islandLine.setCorruptedFeatureTypeFlag( false, arrTerrainPts );
  islandLine.setFeatureVertexIndexes( arrIslandIndexes );

  pTerrainControl->addFeature( islandLine );

  // Set terrain name.

  pTerrainControl->setName(L"Terrain Sample");

  // Add terrain control element to model.

  m_pModel3d->addElement( pTerrainControl );

  // Create terrain graphics element.

  OdDgTerrainPtr pTerrain = OdDgTerrain::createObject();

  m_pModel3d->addElement( pTerrain );
  pTerrain->setTerrainControlElementId( pTerrainControl->elementId() );

  // Set terrain transformation

  double sy            = m_EntityBoxes.getHeight();
  OdGePoint3d ptOrigin = m_EntityBoxes.getBoxCenter( boxRow, boxColumn ) - 
                       OdGeVector3d::kXAxis * sy / 2.1 - OdGeVector3d::kYAxis * sy / 2.1;

  OdGeExtents3d extData;
  pTerrain->getGeomExtents(extData);

  double dScale = 1.0;
  
  if( extData.isValidExtents() )
  {
    dScale = sy * 20.0 / 21.0 / (extData.maxPoint().y - extData.minPoint().y);
  }

  OdGeMatrix3d matTransform = OdGeMatrix3d::translation(ptOrigin.asVector()) * OdGeMatrix3d::scaling(dScale); 

  pTerrain->setTransformation( matTransform );

  // Setup terrain settings.

  // Triangles.

  OdDgMaterialTablePtr pMatTable = m_pModel3d->database()->getMaterialTable(OdDg::kForRead);

  OdDgElementId idMaterial = pMatTable->getAt(L"Terrain Material");

  OdDgDisplayStyleTablePtr pDisplayStyleTable = m_pModel3d->database()->getDisplayStyleTable( OdDg::kForWrite );

  OdDgDisplayStyleTableRecordPtr pTerrainDisplayStyle = pDisplayStyleTable->getAt(L"TerrainShadedDisplayStyle").openObject(OdDg::kForRead);

  OdDgTerrainOptionsTrianglesPtr pTriangles = pTerrain->getOptions(OdDgTerrainOptions::kTriangles);
  pTriangles->setMaterialId( idMaterial.getHandle() );
  pTriangles->setDisplayStyleEntryId( pTerrainDisplayStyle->getEntryId() );
  pTerrain->setOption( pTriangles );

  // Boundary.

  OdDgTerrainOptionsSourceBoundaryPtr pBoundaryOptions = pTerrain->getOptions(OdDgTerrainOptions::kSourceBoundary);
  pBoundaryOptions->setColorIndex(4);
  pBoundaryOptions->setLineWeight(2);
  pBoundaryOptions->setOnFlag(true);

  // Void.

  OdDgTerrainOptionsSourceVoidPtr  pVoidOptions = pTerrain->getOptions(OdDgTerrainOptions::kSourceVoid);
  pVoidOptions->setColorIndex(1);
  pVoidOptions->setLineWeight(1);
  pVoidOptions->setLineStyleEntryId(3);
  pVoidOptions->setOnFlag(true);

  // Island.

  OdDgTerrainOptionsSourceIslandPtr  pIslandOptions = pTerrain->getOptions(OdDgTerrainOptions::kSourceIsland);
  pIslandOptions->setColorIndex(2);
  pIslandOptions->setLineWeight(1);
  pIslandOptions->setLineStyleEntryId(5);
  pIslandOptions->setOnFlag(true);

  // Break line

  OdDgTerrainOptionsSourceBreakLinePtr  pBreakOptions = pTerrain->getOptions(OdDgTerrainOptions::kSourceBreakLine);
  pBreakOptions->setColorIndex(7);
  pBreakOptions->setLineWeight(2);
  pBreakOptions->setOnFlag(true);

  // High points.

  OdDgTextStyleTablePtr pTbl = m_pModel3d->database()->getTextStyleTable(OdDg::kForWrite);

  OdDgTerrainOptionsHighPointsPtr  pHighPointsOptions = pTerrain->getOptions(OdDgTerrainOptions::kHighPoints);
  pHighPointsOptions->setColorIndex(6);
  pHighPointsOptions->setLineWeight(2);
  pHighPointsOptions->setDisplayTextFlag(true);
  pHighPointsOptions->setPointDisplayType( OdDgTerrainOptions::kSymbol );
  pHighPointsOptions->setCharSymbol(L'*');
  OdDgTextStyleTableRecordPtr pHighPointTextStyle = pTbl->getAt(L"Terrain High Point").openObject(OdDg::kForRead);
  pHighPointsOptions->setTextStyleEntryId(pHighPointTextStyle->getEntryId());
  pHighPointsOptions->setPrefix(L"<");
  pHighPointsOptions->setSuffix(L">");
  pHighPointsOptions->setOnFlag(true);

  // Flow arrows.

  OdDgTerrainOptionsFlowArrowsPtr pFlowArrowOptions = pTerrain->getOptions(OdDgTerrainOptions::kFlowArrows);
  pFlowArrowOptions->setColorIndex(12);
  pFlowArrowOptions->setPointDisplayType( OdDgTerrainOptions::kElement );
  pFlowArrowOptions->setLineWeight(2);
  pFlowArrowOptions->setCellScale( OdGeScale3d(0.06, 0.06, 0.06) );
  pFlowArrowOptions->setOnFlag(true);

  // Major contour.

  OdDgTerrainOptionsMajorContourPtr pMajorContourOptions = pTerrain->getOptions(OdDgTerrainOptions::kMajorContour);
  pMajorContourOptions->setOnFlag(true);
  pMajorContourOptions->setColorIndex(3);
  pMajorContourOptions->setDepressionColorIndex(3);
  pMajorContourOptions->setInterval(0.5);
  pMajorContourOptions->setDisplayTextFlag(true);
  pMajorContourOptions->setTextInterval(0.1);
  OdDgTextStyleTableRecordPtr pContourTextStyle = pTbl->getAt(L"Terrain Contour").openObject(OdDg::kForRead);
  pMajorContourOptions->setTextStyleEntryId(pContourTextStyle->getEntryId());

  // Minor contour.

  OdDgTerrainOptionsMinorContourPtr pMinorContourOptions = pTerrain->getOptions(OdDgTerrainOptions::kMinorContour);
  pMinorContourOptions->setOnFlag(true);
  pMinorContourOptions->setColorIndex(2);
  pMinorContourOptions->setDepressionColorIndex(2);
  pMinorContourOptions->setInterval(0.1);
}

void OdExDgnFiller::addItemTypes       ( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d ptCenter = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  // Create simple lines

  OdDgLine3dPtr pLine1 = OdDgLine3d::createObject();
  pLine1->setStartPoint( ptCenter - OdGeVector3d::kXAxis * major/2.0*0.9 - OdGeVector3d::kYAxis * minor /2.0 * 0.66 );
  pLine1->setEndPoint( ptCenter + OdGeVector3d::kXAxis * major/2.0*0.9 - OdGeVector3d::kYAxis * minor /2.0 * 0.66 );
  pLine1->setColorIndex(4);

  OdDgLine3dPtr pLine2 = OdDgLine3d::createObject();
  pLine2->setStartPoint( ptCenter - OdGeVector3d::kXAxis * major/2.0*0.9 - OdGeVector3d::kYAxis * minor /2.0 * 0.33 );
  pLine2->setEndPoint( ptCenter + OdGeVector3d::kXAxis * major/2.0*0.9 - OdGeVector3d::kYAxis * minor /2.0 * 0.33 );
  pLine2->setColorIndex(3);

  OdDgLine3dPtr pLine3 = OdDgLine3d::createObject();
  pLine3->setStartPoint( ptCenter - OdGeVector3d::kXAxis * major/2.0*0.9 );
  pLine3->setEndPoint( ptCenter + OdGeVector3d::kXAxis * major/2.0*0.9 );
  pLine3->setColorIndex(2);

  OdDgLine3dPtr pLine4 = OdDgLine3d::createObject();
  pLine4->setStartPoint( ptCenter - OdGeVector3d::kXAxis * major/2.0*0.9 + OdGeVector3d::kYAxis * minor /2.0 * 0.33 );
  pLine4->setEndPoint( ptCenter + OdGeVector3d::kXAxis * major/2.0*0.9 + OdGeVector3d::kYAxis * minor /2.0 * 0.33 );
  pLine4->setColorIndex(1);

  OdDgLine3dPtr pLine5 = OdDgLine3d::createObject();
  pLine5->setStartPoint( ptCenter - OdGeVector3d::kXAxis * major/2.0*0.9 + OdGeVector3d::kYAxis * minor /2.0 * 0.66 );
  pLine5->setEndPoint( ptCenter + OdGeVector3d::kXAxis * major/2.0*0.9 + OdGeVector3d::kYAxis * minor /2.0 * 0.66 );
  pLine5->setColorIndex(7);

  m_pModel3d->addElement( pLine1 );
  m_pModel3d->addElement( pLine2 );
  m_pModel3d->addElement( pLine3 );
  m_pModel3d->addElement( pLine4 );
  m_pModel3d->addElement( pLine5 );

  // Add simple item to line 1

  OdDgTagDefinitionSetTablePtr pTagSetTable = m_pModel3d->database()->getTagDefinitionSetTable(OdDg::kForRead);

  OdDgItemTypeLibraryPtr pItemLib = pTagSetTable->getAt(convertStringToECSchemaItemName(L"Dgn Create Lib")).openObject(OdDg::kForRead);

  OdDgECSchemaReference schemaRef = pItemLib->getReferenceToSchema();

  OdDgECClassReference classRefForItem1(convertStringToECSchemaItemName(L"Sample Item"), schemaRef);

  OdDgECClassInstancePtr pLine1Item1 = OdDgECClassInstance::createObject(classRefForItem1, m_pModel3d->database(), false);

  OdTimeStamp timeValue;
  timeValue.setDate(12, 28, 2016);
  timeValue.setTime(16, 4, 32, 12);

  OdDgECPropertyInstance prop11(convertStringToECSchemaItemName(L"Simple String"), OdString(L"Test String"));
  OdDgECPropertyInstance prop12(convertStringToECSchemaItemName(L"Simple Integer"), (OdInt8)(17));
  OdDgECPropertyInstance prop13(convertStringToECSchemaItemName(L"Simple Double"), -890.16);
  OdDgECPropertyInstance prop14(convertStringToECSchemaItemName(L"Simple Boolean"), true);
  OdDgECPropertyInstance prop15(convertStringToECSchemaItemName(L"Simple Point3d"), OdGePoint3d(-0.05, 1.6, 18.2));
  OdDgECPropertyInstance prop16(convertStringToECSchemaItemName(L"Simple DateTime"), timeValue);

  pLine1Item1->addPropertyInstance( prop11 );
  pLine1Item1->addPropertyInstance( prop12 );
  pLine1Item1->addPropertyInstance( prop13 );
  pLine1Item1->addPropertyInstance( prop14 );
  pLine1Item1->addPropertyInstance( prop15 );
  pLine1Item1->addPropertyInstance( prop16 );

  OdDgECClassInstanceElementPEPtr pItemTypeLine1PE = pLine1;
  pItemTypeLine1PE->addECClassInstance( pLine1, pLine1Item1);

  // Add array item to line 2

  OdDgECClassReference classRefForItem2(convertStringToECSchemaItemName(L"Array Item"), schemaRef);
  OdDgECClassInstancePtr pLine2Item1 = OdDgECClassInstance::createObject(classRefForItem2, m_pModel3d->database(), false);

  OdArray<OdString> strArr;
  strArr.push_back(L"Array Item 1");
  strArr.push_back(L"Array Item 2");
  strArr.push_back(L"Array Item 3");

  OdArray<OdInt32> intArray;
  intArray.push_back(12345);
  intArray.push_back(-123);
  intArray.push_back(463702);
  intArray.push_back(12);
  intArray.push_back(0);

  OdArray<double> doubleArray;
  doubleArray.push_back(0.0);
  doubleArray.push_back(815.65);
  doubleArray.push_back(-12.0);
  doubleArray.push_back(1774.2);

  OdArray<bool> boolArray;
  boolArray.push_back(true);
  boolArray.push_back(false);
  boolArray.push_back(false);
  boolArray.push_back(true);

  OdArray<OdGePoint3d> point3dArray;
  point3dArray.push_back(OdGePoint3d(0.5, 0.5, 0.5));
  point3dArray.push_back(OdGePoint3d(1.0, 1.5, 2.5));
  point3dArray.push_back(OdGePoint3d(-1.0, -1.5, -2.5));

  OdArray<OdTimeStamp> dateTimeArray;

  timeValue.setDate(12, 28, 2016);
  timeValue.setTime(18, 2, 26, 29);
  dateTimeArray.push_back(timeValue);
  timeValue.setDate(10, 05, 2010);
  timeValue.setTime(8, 3, 24, 11);
  dateTimeArray.push_back(timeValue);

  OdDgECPropertyInstance prop21(convertStringToECSchemaItemName(L"String Array"), strArr);
  OdDgECPropertyInstance prop22(convertStringToECSchemaItemName(L"Integer Array"), intArray);
  OdDgECPropertyInstance prop23(convertStringToECSchemaItemName(L"Double Array"), doubleArray);
  OdDgECPropertyInstance prop24(convertStringToECSchemaItemName(L"Boolean Array"), boolArray);
  OdDgECPropertyInstance prop25(convertStringToECSchemaItemName(L"Point3d Array"), point3dArray);
  OdDgECPropertyInstance prop26(convertStringToECSchemaItemName(L"DateTime Array"), dateTimeArray);

  pLine2Item1->addPropertyInstance( prop21 );
  pLine2Item1->addPropertyInstance( prop22 );
  pLine2Item1->addPropertyInstance( prop23 );
  pLine2Item1->addPropertyInstance( prop24 );
  pLine2Item1->addPropertyInstance( prop25 );
  pLine2Item1->addPropertyInstance( prop26 );

  OdDgECClassInstanceElementPEPtr pItemTypeLine2PE = pLine2;
  pItemTypeLine2PE->addECClassInstance( pLine2, pLine2Item1 );

  // Add complex item to line 3

  OdDgECClassReference classRefForItem3(convertStringToECSchemaItemName(L"Complex Item"), schemaRef);
  OdDgECClassInstancePtr pLine3Item1 = OdDgECClassInstance::createObject(classRefForItem3, m_pModel3d->database(), false);

  // Simple property type values.

  OdArray<OdInt32> arrComplexSimpleInt;
  arrComplexSimpleInt.push_back(-27);
  arrComplexSimpleInt.push_back(256);
  arrComplexSimpleInt.push_back(49);

  OdDgECClassReference classRefForSubItem31(convertStringToECSchemaItemName(L"Complex Property"), schemaRef);
  OdDgECClassInstancePtr pSubClassInstance = OdDgECClassInstance::createObject(classRefForSubItem31, m_pModel3d->database(), false);

  OdDgECPropertyInstance prop31_1(convertStringToECSchemaItemName(L"Complex String"), OdString(L"Simple Complex"));
  OdDgECPropertyInstance prop31_2(convertStringToECSchemaItemName(L"Complex Integer Array"), arrComplexSimpleInt);
  OdDgECPropertyInstance prop31_3(convertStringToECSchemaItemName(L"Complex Double"), -145.128);

  pSubClassInstance->addPropertyInstance(prop31_1);
  pSubClassInstance->addPropertyInstance(prop31_2);
  pSubClassInstance->addPropertyInstance(prop31_3);

  OdDgECPropertyInstance prop31(convertStringToECSchemaItemName(L"Single Property Type"), pSubClassInstance);

  pLine3Item1->addPropertyInstance( prop31 );

  // Array property types values.

  OdArray<OdInt32> arrComplexArrayInt;
  arrComplexArrayInt.push_back(4);
  arrComplexArrayInt.push_back(8);
  arrComplexArrayInt.push_back(16);

  OdArray<OdDgECClassInstancePtr> arrClassInstancePtr;

  OdDgECClassInstancePtr pSubClassInstanceForArr1 = OdDgECClassInstance::createObject(classRefForSubItem31, m_pModel3d->database(), false);

  OdDgECPropertyInstance prop32_0_1(convertStringToECSchemaItemName(L"Complex String"), OdString(L"Array Complex 1"));
  OdDgECPropertyInstance prop32_0_2(convertStringToECSchemaItemName(L"Complex Integer Array"), arrComplexArrayInt);
  OdDgECPropertyInstance prop32_0_3(convertStringToECSchemaItemName(L"Complex Double"), 77.123);

  pSubClassInstanceForArr1->addPropertyInstance(prop32_0_1);
  pSubClassInstanceForArr1->addPropertyInstance(prop32_0_2);
  pSubClassInstanceForArr1->addPropertyInstance(prop32_0_3);

  arrClassInstancePtr.push_back(pSubClassInstanceForArr1);

  arrComplexArrayInt.clear();
  arrComplexArrayInt.push_back(-10);
  arrComplexArrayInt.push_back(-20);

  OdDgECClassInstancePtr pSubClassInstanceForArr2 = OdDgECClassInstance::createObject(classRefForSubItem31, m_pModel3d->database(), false);

  OdDgECPropertyInstance prop32_1_1(convertStringToECSchemaItemName(L"Complex String"), OdString(L"Array Complex 2"));
  OdDgECPropertyInstance prop32_1_2(convertStringToECSchemaItemName(L"Complex Integer Array"), arrComplexArrayInt);
  OdDgECPropertyInstance prop32_1_3(convertStringToECSchemaItemName(L"Complex Double"), -12.8);

  pSubClassInstanceForArr2->addPropertyInstance(prop32_1_1);
  pSubClassInstanceForArr2->addPropertyInstance(prop32_1_2);
  pSubClassInstanceForArr2->addPropertyInstance(prop32_1_3);

  arrClassInstancePtr.push_back(pSubClassInstanceForArr2);

  arrComplexArrayInt.clear();
  arrComplexArrayInt.push_back(8);
  arrComplexArrayInt.push_back(-12);
  arrComplexArrayInt.push_back(16);
  arrComplexArrayInt.push_back(-20);

  OdDgECClassInstancePtr pSubClassInstanceForArr3 = OdDgECClassInstance::createObject(classRefForSubItem31, m_pModel3d->database(), false);

  OdDgECPropertyInstance prop32_2_1(convertStringToECSchemaItemName(L"Complex String"), OdString(L"Array Complex 3"));
  OdDgECPropertyInstance prop32_2_2(convertStringToECSchemaItemName(L"Complex Integer Array"), arrComplexArrayInt);
  OdDgECPropertyInstance prop32_2_3(convertStringToECSchemaItemName(L"Complex Double"), 1789.46);

  pSubClassInstanceForArr3->addPropertyInstance(prop32_2_1);
  pSubClassInstanceForArr3->addPropertyInstance(prop32_2_2);
  pSubClassInstanceForArr3->addPropertyInstance(prop32_2_3);

  arrClassInstancePtr.push_back(pSubClassInstanceForArr3);

  OdDgECPropertyInstance prop32(convertStringToECSchemaItemName(L"Array Of Property Types"), arrClassInstancePtr);

  pLine3Item1->addPropertyInstance(prop32);

  OdDgECClassInstanceElementPEPtr pItemTypeLine3PE = pLine3;
  pItemTypeLine3PE->addECClassInstance( pLine3, pLine3Item1 );

  // Add several items to line 4.

  // Simple.

  OdDgECClassReference classRefForItem41(convertStringToECSchemaItemName(L"Sample Item"), schemaRef);
  OdDgECClassInstancePtr pLine4Item1 = OdDgECClassInstance::createObject(classRefForItem41, m_pModel3d->database(), false);

  OdDgECPropertyInstance prop41(convertStringToECSchemaItemName(L"Simple String"), OdString(L"Line 4 String"));
  OdDgECPropertyInstance prop42(convertStringToECSchemaItemName(L"Simple Point3d"), OdGePoint3d(22.2, 33.3, 44.4));

  pLine4Item1->addPropertyInstance( prop41 );
  pLine4Item1->addPropertyInstance( prop42 );

  // Array.

  OdArray<OdString> strArr1;
  strArr1.push_back(L"Line 4 Item 1");
  strArr1.push_back(L"Line 4 Item 2");

  OdArray<double> doubleArray1;
  doubleArray1.push_back(11.11);
  doubleArray1.push_back(-22.22);
  doubleArray1.push_back(33.33);
  doubleArray1.push_back(-44.44);

  OdDgECClassReference classRefForItem42(convertStringToECSchemaItemName(L"Array Item"), schemaRef);
  OdDgECClassInstancePtr pLine4Item2 = OdDgECClassInstance::createObject(classRefForItem42, m_pModel3d->database(), false);

  OdDgECPropertyInstance prop43(convertStringToECSchemaItemName(L"String Array"), strArr1);
  OdDgECPropertyInstance prop44(convertStringToECSchemaItemName(L"Double Array"), doubleArray1);

  pLine4Item2->addPropertyInstance( prop43 );
  pLine4Item2->addPropertyInstance( prop44 );

  // Complex.

  OdArray<OdInt32> arrComplexSimpleInt1;
  arrComplexSimpleInt1.push_back(88);
  arrComplexSimpleInt1.push_back(16);

  OdDgECClassReference classRefForItem43(convertStringToECSchemaItemName(L"Complex Item"), schemaRef);
  OdDgECClassInstancePtr pLine4Item3 = OdDgECClassInstance::createObject(classRefForItem43, m_pModel3d->database(), false);

  OdDgECClassReference classRefForSubItem43_1(convertStringToECSchemaItemName(L"Complex Property"), schemaRef);
  OdDgECClassInstancePtr pSubClassInstance43_1 = OdDgECClassInstance::createObject(classRefForSubItem43_1, m_pModel3d->database(), false);

  OdDgECPropertyInstance prop41_1(convertStringToECSchemaItemName(L"Complex String"), OdString(L"Sample of String"));
  OdDgECPropertyInstance prop41_2(convertStringToECSchemaItemName(L"Complex Integer Array"), arrComplexSimpleInt1);
  OdDgECPropertyInstance prop41_3(convertStringToECSchemaItemName(L"Complex Double"), 777.777);

  pSubClassInstance43_1->addPropertyInstance(prop41_1);
  pSubClassInstance43_1->addPropertyInstance(prop41_2);
  pSubClassInstance43_1->addPropertyInstance(prop41_3);

  OdDgECPropertyInstance prop45(convertStringToECSchemaItemName(L"Single Property Type"), pSubClassInstance43_1);

  pLine4Item3->addPropertyInstance( prop45 );

  OdDgECClassInstanceElementPEPtr pItemTypeLine4PE = pLine4;
  pItemTypeLine4PE->addECClassInstance( pLine4, pLine4Item1 );
  pItemTypeLine4PE->addECClassInstance( pLine4, pLine4Item2 );
  pItemTypeLine4PE->addECClassInstance( pLine4, pLine4Item3 );

  // Add custom items to line 5.

  OdDgXMLCustomSchemaContainerPtr pCustomXmlScheme = pTagSetTable->getAt(convertStringToECSchemaItemName(L"CustomXmlSchemeSample")).openObject(OdDg::kForRead);

  OdDgECSchemaReference customXmlRef = pCustomXmlScheme->getReferenceToSchema();

  OdArray<OdInt32> arrProperty;
  arrProperty.push_back(18);
  arrProperty.push_back(19);
  arrProperty.push_back(20);
  arrProperty.push_back(21);

  OdDgECClassReference classRef51(convertStringToECSchemaItemName(L"CustomItem"), customXmlRef);
  OdDgECClassInstancePtr pLine5Item1 = OdDgECClassInstance::createObject(classRef51, m_pModel3d->database(), false);

  OdDgECPropertyInstance prop51(convertStringToECSchemaItemName(L"Property1"), OdString(L"Custom String"));
  OdDgECPropertyInstance prop52(convertStringToECSchemaItemName(L"Property2"), arrProperty);
  OdDgECPropertyInstance prop53(convertStringToECSchemaItemName(L"Property3"), true);
  OdDgECPropertyInstance prop54(convertStringToECSchemaItemName(L"Property4"), 19.467);

  pLine5Item1->addPropertyInstance( prop51 );
  pLine5Item1->addPropertyInstance( prop52 );
  pLine5Item1->addPropertyInstance( prop53 );
  pLine5Item1->addPropertyInstance( prop54 );

  OdDgECClassInstanceElementPEPtr pItemTypeLine5PE = pLine5;
  pItemTypeLine5PE->addECClassInstance( pLine5, pLine5Item1 );
}

//===========================================================================================================

void OdExDgnFiller::addAssocRegion(int boxRow, int boxColumn)
{
	double minor = m_EntityBoxes.getHeight();
	double major = m_EntityBoxes.getWidth(boxRow, boxColumn);
	OdGePoint3d ptCenter = m_EntityBoxes.getBoxCenter(boxRow, boxColumn);

	OdDgAssocRegion3dPtr pAssocRegion = OdDgAssocRegion3d::createObject();
	m_pModel3d->addElement(pAssocRegion);

	OdDgShape3dPtr pShapeForAssoc = OdDgShape3d::createObject();
	pShapeForAssoc->addVertex(ptCenter + OdGeVector3d::kXAxis * major / 2.2 + OdGeVector3d::kYAxis * minor / 2.2);
	pShapeForAssoc->addVertex(ptCenter + OdGeVector3d::kXAxis * major / 2.2 - OdGeVector3d::kYAxis * minor / 2.2);
	pShapeForAssoc->addVertex(ptCenter - OdGeVector3d::kXAxis * major / 2.2 - OdGeVector3d::kYAxis * minor / 2.2);
	pShapeForAssoc->addVertex(ptCenter - OdGeVector3d::kXAxis * major / 2.2 + OdGeVector3d::kYAxis * minor / 2.2);
	pShapeForAssoc->addVertex(ptCenter + OdGeVector3d::kXAxis * major / 2.2 + OdGeVector3d::kYAxis * minor / 2.2);

	m_pModel3d->addElement(pShapeForAssoc);

	OdDgElementIdArray arrElms;
	arrElms.push_back(pShapeForAssoc->elementId());
	pAssocRegion->setDependedElements(arrElms);
	
	OdDgShape3dPtr pAssocShape = pShapeForAssoc->clone();
	pAssocShape->setInvisibleFlag(true);
	pAssocRegion->add(pAssocShape);

	OdDgCrossHatchPatternPtr pHatch = OdDgCrossHatchPattern::createObject();
	pHatch->setSpace1(0.25);
	pHatch->setSpace2(0.25);
	pHatch->setUseLineColorFlag(true);
	pHatch->setLineColorIndex(4);
	pAssocRegion->setHatchPattern(pHatch);
	pAssocRegion->setColorIndex(2);
}

//===========================================================================================================

void OdExDgnFiller::addTableElement( int boxRow, int boxColumn )
{
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxColumn );
  OdGePoint3d ptCenter = m_EntityBoxes.getBoxCenter( boxRow, boxColumn );

  OdDgFontTablePtr pFontTable           = m_pModel3d->database()->getFontTable();
  double dScaleToUORs = m_pModel3d->convertWorkingUnitsToUORs(1.0);

  // Create text style for table element.
  OdDgTextStyleTablePtr pTextStyleTable = m_pModel3d->database()->getTextStyleTable(OdDg::kForWrite);
  OdDgTextStyleTableRecordPtr pTableTextStyle = pTextStyleTable->getDefaultData();
  pTableTextStyle->setName(L"TableTextStyle");
  pTableTextStyle->setTextHeight( 0.1 * dScaleToUORs );
  pTableTextStyle->setTextWidth( 0.1 * dScaleToUORs );
  pTableTextStyle->setFontEntryId( pFontTable->getFont("Arial")->getNumber() );
  pTableTextStyle->setColorIndex(250);
  pTextStyleTable->add( pTableTextStyle );

  OdDgTextStyleTableRecordPtr pMextStyle = pTextStyleTable->getDefaultData();
  pMextStyle->setName(L"TableMTextStyle");
  pMextStyle->setTextHeight( 0.05 * dScaleToUORs );
  pMextStyle->setTextWidth( 0.05 * dScaleToUORs );
  pMextStyle->setFontEntryId( pFontTable->getFont("Arial")->getNumber() );
  pMextStyle->setColorIndex(250);
  pTextStyleTable->add( pMextStyle );

  // init table default data

  OdDgTableElementPtr pTableElement = OdDgTableElement::createObject();
  m_pModel3d->addElement( pTableElement );
  pTableElement->setBodyTextStyle(pTableTextStyle->getEntryId());
  pTableElement->initTableDefaults();

  // add columns and rows

  pTableElement->addColumn();
  pTableElement->addColumn();
  pTableElement->addColumn();
  pTableElement->addColumn();
  pTableElement->addColumn();
  pTableElement->addRow();
  pTableElement->addRow();
  pTableElement->addRow();
  pTableElement->addRow();
  pTableElement->addRow();

  // Set table position, column widths and rows heights.

  pTableElement->setOrigin( ptCenter - OdGeVector3d::kXAxis * major * 2.25 / 5.0 + OdGeVector3d::kYAxis * minor * 2.25 / 5.0 );
  pTableElement->getRow(0)->setHeight( 0.25 * minor );
  pTableElement->getRow(1)->setHeight( 0.2 * minor );
  pTableElement->getRow(2)->setHeight( 0.15 * minor );
  pTableElement->getRow(3)->setHeight( 0.15 * minor );
  pTableElement->getRow(4)->setHeight( 0.15 * minor );
  pTableElement->getColumn(0)->setWidth( 0.2 * major );
  pTableElement->getColumn(1)->setWidth( 0.15 * major );
  pTableElement->getColumn(2)->setWidth( 0.2 * major );
  pTableElement->getColumn(3)->setWidth( 0.15 * major );
  pTableElement->getColumn(4)->setWidth( 0.2 * major );

  // Create merges.

  OdDgTableElementMerge titleMerge;
  titleMerge.setBaseRowIndex(0);
  titleMerge.setBaseColumnIndex(0);
  titleMerge.setRowCount(1);
  titleMerge.setColumnCount(5);

  OdDgTableElementMerge firstColumnMerge;
  firstColumnMerge.setBaseRowIndex(1);
  firstColumnMerge.setBaseColumnIndex(0);
  firstColumnMerge.setRowCount(4);
  firstColumnMerge.setColumnCount(1);

  OdDgTableElementMerge lastColumnMerge;
  lastColumnMerge.setBaseRowIndex(1);
  lastColumnMerge.setBaseColumnIndex(4);
  lastColumnMerge.setRowCount(4);
  lastColumnMerge.setColumnCount(1);

  OdDgTableElementMerge secondRowMerge;
  secondRowMerge.setBaseRowIndex(1);
  secondRowMerge.setBaseColumnIndex(1);
  secondRowMerge.setRowCount(1);
  secondRowMerge.setColumnCount(3);

  pTableElement->addMerge( titleMerge );
  pTableElement->addMerge( firstColumnMerge );
  pTableElement->addMerge( lastColumnMerge );
  pTableElement->addMerge( secondRowMerge );

  // Add title row and set fills

  pTableElement->setTitleRowCount(1);
  pTableElement->setOddFillColor( 1 );
  pTableElement->setOddFillColorFlag(true);
  pTableElement->setEvenFillColor( 2 );
  pTableElement->setEvenFillColorFlag(true);
  pTableElement->getCell(0,0)->setFillColor(9);
  pTableElement->getCell(1,0)->setFillColor(32);
  pTableElement->getCell(1,4)->setFillColor(32);
  pTableElement->getCell(1,1)->setFillColor(12);
  pTableElement->getCell(3,2)->setFillColor(4);

  // Create title row text.

  OdDgTextNode3dPtr pTitleTextNode = OdDgTextNode3d::createObject();
  OdDgText3dPtr pTitleText = OdDgText3d::createObject();
  pTitleTextNode->add( pTitleText );
  pTitleTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pTitleTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pTitleText->applyTextStyle( pTableTextStyle->elementId() );
  pTitleText->setText(L"Title Row");
  pTitleText->setHeightMultiplier(0.3);
  pTitleText->setLengthMultiplier(0.3);
  pTitleText->setColorIndex(3);

  pTableElement->getCell(0,0)->setCellText( pTitleTextNode );
  pTableElement->getCell(0,0)->setShowTextFlag(true);
  pTableElement->getCell(0,0)->setTextAlignment( OdDgTableCellElement::kCenterMiddle);

  // Create first column text.

  OdDgTextNode3dPtr pFirstColumnTextNode = OdDgTextNode3d::createObject();
  OdDgText3dPtr pFirstColumnText = OdDgText3d::createObject();
  pFirstColumnTextNode->add( pFirstColumnText );
  pFirstColumnTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pFirstColumnTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pFirstColumnText->applyTextStyle( pTableTextStyle->elementId() );
  pFirstColumnText->setText(L"Left Rotated");

  pTableElement->getCell(1,0)->setCellText( pFirstColumnTextNode );
  pTableElement->getCell(1,0)->setShowTextFlag(true);
  pTableElement->getCell(1,0)->setTextAlignment( OdDgTableCellElement::kCenterMiddle);
  pTableElement->getCell(1,0)->setTextOrientation( OdDgTableCellElement::kRotateLeft );

  // Create last column text.

  OdDgTextNode3dPtr pLastColumnTextNode = OdDgTextNode3d::createObject();
  OdDgText3dPtr pLastColumnText = OdDgText3d::createObject();
  pLastColumnTextNode->add( pLastColumnText );
  pLastColumnTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pLastColumnTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pLastColumnText->applyTextStyle( pTableTextStyle->elementId() );
  pLastColumnText->setText(L"Right Rotated");

  pTableElement->getCell(1,4)->setCellText( pLastColumnTextNode );
  pTableElement->getCell(1,4)->setShowTextFlag(true);
  pTableElement->getCell(1,4)->setTextAlignment( OdDgTableCellElement::kCenterMiddle);
  pTableElement->getCell(1,4)->setTextOrientation( OdDgTableCellElement::kRotateRight );

  // Create second row text.

  OdDgTextNode3dPtr pSecondRowTextNode = OdDgTextNode3d::createObject();
  OdDgText3dPtr pSecondRowText = OdDgText3d::createObject();
  pSecondRowTextNode->add( pSecondRowText );
  pSecondRowTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pSecondRowTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pSecondRowText->applyTextStyle( pTableTextStyle->elementId() );
  pSecondRowText->setText(L"Merged Cells");
  pSecondRowText->setHeightMultiplier(0.2);
  pSecondRowText->setLengthMultiplier(0.2);
  pSecondRowText->setColorIndex(5);

  pTableElement->getCell(1,1)->setCellText( pSecondRowTextNode );
  pTableElement->getCell(1,1)->setShowTextFlag(true);
  pTableElement->getCell(1,1)->setTextAlignment( OdDgTableCellElement::kCenterMiddle);

  // Create central multiline text.

  OdDgTextNode3dPtr pMTextNode = OdDgTextNode3d::createObject();
  OdDgText3dPtr pTextLine1 = OdDgText3d::createObject();
  OdDgText3dPtr pTextLine2 = OdDgText3d::createObject();
  OdDgText3dPtr pTextLine30 = OdDgText3d::createObject();
  OdDgText3dPtr pTextLine31 = OdDgText3d::createObject();
  OdDgText3dPtr pTextLine40 = OdDgText3d::createObject();
  OdDgText3dPtr pTextLine41 = OdDgText3d::createObject();
  pMTextNode->add( pTextLine1 );
  pMTextNode->add( pTextLine2 );
  pMTextNode->add( pTextLine30 );
  pMTextNode->add( pTextLine31 );
  pMTextNode->add( pTextLine40 );
  pMTextNode->add( pTextLine41 );
  pMTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pMTextNode->applyTextStyle( pMextStyle->elementId() );
  pMTextNode->setJustification( OdDg::kCenterCenter );
  pMTextNode->setLineSpacing(0.09);
  pMTextNode->setLineSpacingType( OdDgTextExtendedProperties::kExact );
  pTextLine1->applyTextStyle( pMextStyle->elementId() );
  pTextLine2->applyTextStyle( pMextStyle->elementId() );
  pTextLine30->applyTextStyle( pMextStyle->elementId() );
  pTextLine31->applyTextStyle( pMextStyle->elementId() );
  pTextLine40->applyTextStyle( pMextStyle->elementId() );
  pTextLine41->applyTextStyle( pMextStyle->elementId() );
  pTextLine1->setText(L"Multiline Text:");
  pTextLine1->setCrCount(1);
  pTextLine2->setText(L"- Line 1.");
  pTextLine2->setItalics(true);
  pTextLine2->setCrCount(1);
  pTextLine30->setText(L"- Line ");
  pTextLine30->setBoldFlag(true);
  pTextLine31->setText(L"2.");
  pTextLine31->setColorIndex(2);
  pTextLine31->setCrCount(1);
  pTextLine40->setText(L"- Line ");
  pTextLine40->setUnderlineFlag(true);
  pTextLine41->setText(L"3.");
  pTextLine41->setOverlineFlag(true);
  pTextLine41->setColorIndex(5);
  pTextLine41->setCrCount(1);

  pTableElement->getCell(3,2)->setCellText( pMTextNode );

  // Create all other texts.

  OdDgTextNode3dPtr pTextNode = OdDgTextNode3d::createObject();
  OdDgText3dPtr pText = OdDgText3d::createObject();
  pTextNode->add( pText );
  pTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pTextNode->setJustification( OdDg::kRightBottom );
  pText->applyTextStyle( pTableTextStyle->elementId() );
  pText->setText(L"R-B");

  pTableElement->getCell(2,1)->setCellText( pTextNode );

  pTextNode = OdDgTextNode3d::createObject();
  pText = OdDgText3d::createObject();
  pTextNode->add( pText );
  pTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pTextNode->setJustification( OdDg::kCenterBottom );
  pText->applyTextStyle( pTableTextStyle->elementId() );
  pText->setText(L"C-B");

  pTableElement->getCell(2,2)->setCellText( pTextNode );

  pTextNode = OdDgTextNode3d::createObject();
  pText = OdDgText3d::createObject();
  pTextNode->add( pText );
  pTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pTextNode->setJustification( OdDg::kLeftBottom );
  pText->applyTextStyle( pTableTextStyle->elementId() );
  pText->setText(L"L-B");

  pTableElement->getCell(2,3)->setCellText( pTextNode );

  pTextNode = OdDgTextNode3d::createObject();
  pText = OdDgText3d::createObject();
  pTextNode->add( pText );
  pTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pTextNode->setJustification( OdDg::kRightCenter );
  pText->applyTextStyle( pTableTextStyle->elementId() );
  pText->setText(L"R-C");

  pTableElement->getCell(3,1)->setCellText( pTextNode );

  pTextNode = OdDgTextNode3d::createObject();
  pText = OdDgText3d::createObject();
  pTextNode->add( pText );
  pTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pTextNode->setJustification( OdDg::kLeftCenter );
  pText->applyTextStyle( pTableTextStyle->elementId() );
  pText->setText(L"L-C");

  pTableElement->getCell(3,3)->setCellText( pTextNode );

  pTextNode = OdDgTextNode3d::createObject();
  pText = OdDgText3d::createObject();
  pTextNode->add( pText );
  pTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pTextNode->setJustification( OdDg::kRightTop );
  pText->applyTextStyle( pTableTextStyle->elementId() );
  pText->setText(L"R-T");

  pTableElement->getCell(4,1)->setCellText( pTextNode );

  pTextNode = OdDgTextNode3d::createObject();
  pText = OdDgText3d::createObject();
  pTextNode->add( pText );
  pTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pTextNode->setJustification( OdDg::kCenterTop );
  pText->applyTextStyle( pTableTextStyle->elementId() );
  pText->setText(L"C-T");

  pTableElement->getCell(4,2)->setCellText( pTextNode );

  pTextNode = OdDgTextNode3d::createObject();
  pText = OdDgText3d::createObject();
  pTextNode->add( pText );
  pTextNode->setDatabaseDefaults(m_pModel3d->database(), true);
  pTextNode->applyTextStyle( pTableTextStyle->elementId() );
  pTextNode->setJustification( OdDg::kLeftTop );
  pText->applyTextStyle( pTableTextStyle->elementId() );
  pText->setText(L"L-T");

  pTableElement->getCell(4,3)->setCellText( pTextNode );

  // Correct borders for central cell.

  OdDgTableCellSymbology borderData;
  borderData.setColorIndex(3);
  borderData.setLineStyle(2);
  borderData.setLineWeight(3);
  borderData.setLineVisibility(true);

  pTableElement->getCell(3,2)->setTopBorder(borderData);
  pTableElement->getCell(3,2)->setBottomBorder(borderData);
  pTableElement->getCell(3,2)->setLeftBorder(borderData);
  pTableElement->getCell(3,2)->setRightBorder(borderData);
}


