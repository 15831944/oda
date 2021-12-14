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

#include "CmColorBase.h"

#include "DgModel.h"
#include "DgLineString.h"
#include "DgDimOptions.h"
#include "DgDimension.h"
#include "DgDimDefs.h"
#include "DgArc.h"
#include "DgShape.h"
#include "DgTable.h"
#include "DgFontTableRecord.h"
#include "DgDatabase.h"
#include "DgLine.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgAttributeLinkage.h"
/*
#include "DgEllipse.h"
#include "DgCone.h"
#include "DgLine.h"
#include "DgText.h"
#include "DgTextNode.h"
#include "DgCurve.h"
#include "DgConic.h"
#include "DgPointString.h"
#include "DgComplexString.h"
#include "DgLevelTableRecord.h"
#include "DgTable.h"
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
*/

#include "ExDgnDimFiller.h"
#include "EntityBoxes.h"

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
  {2,2,2,2,2,1,0,0,0,0,0},
  {2,2,2,2,2,1,0,0,0,0,0},
  {2,2,2,2,2,1,0,0,0,0,0},
  {3,4,3,1,0,0,0,0,0,0,0},
  {2,3,3,1,2,0,0,0,0,0,0},
  {1,2,2,1,1,1,1,1,1,0,0},
  {2,2,2,1,1,2,1,0,0,0,0}
};

OdExDgnFiller::OdExDgnFiller()
  : m_pModel3d(0)
{
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

  // there is no necessity to add RSC (MicroStation Recource file) font
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

void addExtraLargeBoxes(OdExDgnFiller* pFiller,OdDgModel* pModel);
void OdExDgnFiller::createEntityBoxes(OdDgModel* pModel)
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

      MakeEntityBox(pModel,currentPoint,m_EntityBoxes.getHeight(),wCurBox);
    }
  }
  addExtraLargeBoxes(this,pModel);
}

void OdExDgnFiller::fillModel(OdDgModel* pModel)
{
  m_pModel3d = pModel;

  /**********************************************************************/
  /* Create table record elements                                       */
  /**********************************************************************/
/*
  addLevelTableRecord();
  addFontTableRecord();
  addDimStyleTableRecord();
  addLineStyleTableRecord();
  addMultilineStyleTableRecord();
  addRegAppTableRecord();
  addTextStyleTableRecord();
*/
  /**********************************************************************/
  /* Create boxes for elements                                          */
  /**********************************************************************/
 // addAngleAxisDimension( 4, 0 );
 // return;

  addFontTableRecord();
  createEntityBoxes(m_pModel3d);


  //////////////////////////////
  // Creates SIZE ARROW dimension
  //
  // this is for three dimensions stacked together and rotated vertical position
  addStackedSizeArrowDimension(6,6);

  // this is a single sizearrow dimension
  addSizeArrowDimension( 3, 0 );// textmargin and textlength is same as for box 3,1
  //////////////////////////////
  //this is important, the AUTO feature of the longer box 3,1
  //sets this one to "inside" orientation, because textmargin and textlength is short enough to fit.
  addSizeArrowDimension( 3, 1 );
  //////////////////////////////
  // these will check each orientation mode and arrow type
  for(int ix = 0; ix <= 3; ix++) // 4 options of term_mode == TerminatorOrientation
  {
    addInsideSizeArrowDimension(0,ix);
  }
  addSingleLocationDimension(0, 4);
  addSizeStrokeDimension(0, 5);
  addStackedLocationDimension(1, 4);
  addCustomLinearDimension(1, 5);

  //////////////////////////////
  // Creates RADIUS dimension
  //
  addRadiusDimension( 1, 0 );
  addRadiusDimension( 1, 1 );
  addRadiusDimension( 1, 2 );
  addRadiusDimension( 1, 3 );


  //////////////////////////////
  // Creates RADIUS EXTENDED dimension
  //
  addRadiusExtendedDimension( 2, 0 );
  addRadiusExtendedDimension( 2, 1 );
  addRadiusExtendedDimension( 2, 2 );
  addRadiusExtendedDimension( 2, 3 );


  //////////////////////////////
  // Creates CENTER MARK dimension
  //
  addCenterMarkDimension( 5, 0 );

  addAngleAxisXDimension( 4, 0 );
  addAngleAxisYDimension();
  addLabelLineDimension();
  addNoteDimension();
  addMicrostationStyleNote( 5, 1 );
  addAngleSizeDimension( 4, 1 );
  addAngleSizeDimension2( 4, 2 );
  addAngleLinesDimension( 4, 3 );
  addAngleLinesDimension2( 5, 2 );
  addAngleLocationDimension( 4, 4 );
  addArcSizeDimension( 3, 2 );
  addArcLocationDimension( 3, 3 );

  addAlignmentOptions( 6, 2 );

  addDiameterExtended();
  addDiameterExtended2();
  addRotVarSizeArrows();
  addOrdinates();
  addCustomDim();

  addLinearDimensionRelativePoints();
  addAssociativeLinearDimension();
  addLinearDimensionWithLeaderAndRotatedText();
  addLinearDimensionWithUserDefinedText();
}

/*
void OdExDgnFiller::addLevelTableRecord()
{
  OdDgLevelTableRecordPtr pLevel = OdDgLevelTableRecord::createObject();
  pLevel->setName("ExDgnCreate level 1");
  pLevel->setElementColorIndex(6);

  OdDgLevelTablePtr pLevelTable = m_pModel3d->database()->getLevelTable();
  pLevelTable->add(pLevel);
}

void OdExDgnFiller::addFontTableRecord()
{
#pragma MARKMESSAGE("TODO: Examples. Implement addFontTableRecord().");
}

void OdExDgnFiller::addDimStyleTableRecord()
{
#pragma MARKMESSAGE("TODO: Examples. Implement addDimStyleTableRecord().");
}

void OdExDgnFiller::addLineStyleTableRecord()
{
#pragma MARKMESSAGE("TODO: Examples. Implement addLineStyleTableRecord().");
}

void OdExDgnFiller::addMultilineStyleTableRecord()
{
#pragma MARKMESSAGE("TODO: Examples. Implement addMultilineStyleTableRecord().");
}

void OdExDgnFiller::addRegAppTableRecord()
{
#pragma MARKMESSAGE("TODO: Examples. Implement addRegAppTableRecord().");
}

void OdExDgnFiller::addTextStyleTableRecord()
{
#pragma MARKMESSAGE("TODO: Examples. Implement addTextStyleTableRecord().");
}
*/


/**********************************************************************/
// Creates various dimensions
/**********************************************************************/

//
// Creates LINEAR dimensions
//
void OdExDgnFiller::addSizeArrowDimension(int boxRow, int boxCol)
{
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  //pDim->setAlignment(OdDgDimension::kPaTrue);
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1
  ptDim.setPoint( pt.set( boxCenter.x - dWidth / 8.0, boxCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dHeight / 5 );
  pDim->addPoint( ptDim );

  // Point 2
  ptDim.setPoint( pt.set( boxCenter.x + dWidth / 5.5, boxCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dHeight / 7 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( 1., 0., 0., 0. ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.25);
  geom.setWitnessLineExtend(0.25);
  geom.setTextLift(0.25);
  geom.setTextMargin(0.25);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.5);
  geom.setTerminatorHeight(0.25);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.5);
  info.setHeight(0.5);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // tool
  pDim->setLeftTerminator( OdDgDimTool::kTtCircle );
  pDim->setRightExtLinesPresentFlag(false);
  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addInsideSizeArrowDimension(int boxRow, int boxCol)
{
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1
  ptDim.setPoint( pt.set( boxCenter.x - dWidth / 8.0, boxCenter.y + ((boxCol & 1)*dHeight/7.0), 0 ) );
  ptDim.setOffsetToDimLine( dHeight / 6 );
  pDim->addPoint( ptDim );

  // Point 2
  ptDim.setPoint( pt.set( boxCenter.x + dWidth / (boxCol*3/4+3), boxCenter.y + ((boxCol & 1)*dHeight/7.0), 0 ) );
  ptDim.setOffsetToDimLine( dHeight / 7 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );

  pDim->setTermMode((OdDgDimension::TerminatorOrientation)(boxCol % 4));// TerminatorOrientations: Auto, Reversed, Inside, Outside
  pDim->setArrowHead((OdDgDimension::TerminatorArrowHeadType)(boxCol % 3));//types: open, filled, closed

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  ///////////
  format.setPrimaryAccuracy( (OdDgDimTextFormat::Accuracy) ( 1<<(boxCol + 1) ));// fractions
  format.setSecondaryAccuracy(  (OdDgDimTextFormat::Accuracy) (1<<(boxCol + 1) ));
  format.setDecimalCommaFlag( true );
  ///////////
  format.setOmitLeadDelimiterFlag( true );
  format.setPriAllowZeroMastFlag( true );
  format.setSecAllowZeroMastFlag( true );
  format.setDualFormat( 1 );
  format.setLocalFileUnitsFlag( true );
  format.setMetricSpaceFlag( true );
  format.setSecondaryDelimiterFlag( true );
  format.setSecondarySubUnitsFlag( true );
  ///////////
  format.setSuperScriptLSDFlag( true );
  format.setAngleMeasureFlag( true );
  format.setAngleFormatFlag( true );
  format.setAngleMode( OdDgDimTextFormat::kAngle_D );
  ///////////
  pDim->setDimTextFormat( format );
  
  pDim->setRotation( OdGeQuaternion( 1., 0., 0., 0. ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.25);
  geom.setWitnessLineExtend(0.25);
  geom.setTextLift(0.25*boxCol);
  geom.setTextMargin(0.25);
  geom.setMargin(1.0);
  geom.setTerminatorWidth(0.5);
  geom.setTerminatorHeight(0.25);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  
  info.setPrimaryNoMasterUnitsFlag( true );
  info.setSecNoMasterUnitsFlag( true );
  {
    pDim->setStackFractFlag(true);//{ if false, defaults to FromFont }
    info.setStackedFractionType( OdDgDimTextInfo::kFractionHorizontal );// {0=FromFont,1=Horizontal,2=Vertical}
    info.setStackFractAlignment( OdDgDimTextInfo::kFractAlignmentCenter );//{ 0=Top,1=Center,2=Bottom } when vertical only?
  }
  //info.setTextFlags( 0x01 );
  info.setWidth(0.5);
  info.setHeight(0.5);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator((OdDgDimTool::TerminatorType)(boxCol % 4));// right terminator defaults to arrow

  m_pModel3d->addElement( pDim );
}
void OdExDgnFiller::addStackedSizeArrowDimension(int boxRow, int boxCol)
{ // taken from a client's example file (typical)
  // notice that the middle two witness lines are extended slightly.
  // this shows that the information necessary was merely the points of the baseline (object) and the rotation of the text
  // note: for rotation, we use a quaternion if the file is 3d and a 2x2 matrix if the file is 2d
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1
  ptDim.setPoint( pt.set( 1.5, 3.0, 0 ) );
  ptDim.setOffsetToDimLine( 1.1 );
  pDim->addPoint( ptDim );

  // Point 2
  ptDim.setPoint( pt.set( 1.6, 2.7, 0 ) );
  ptDim.setOffsetToDimLine( 5.2 );
  pDim->addPoint( ptDim );

  // Point 3
  ptDim.setPoint( pt.set( 1.6, 2.0, 0 ) );
  ptDim.setOffsetToDimLine( 0.2 );
  pDim->addPoint( ptDim );

  // Point 4
  ptDim.setPoint( pt.set( 1.5, 0.1, 0 ) );
  ptDim.setOffsetToDimLine( 1.1 );
  pDim->addPoint( ptDim );

  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  pDim->setArrowHead(OdDgDimension::kFilled);//types: open, filled, closed

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  ///////////
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setSecondaryAccuracy(  OdDgDimTextFormat::kDecimal2 );
  format.setDecimalCommaFlag( false );
  format.setPrimarySubUnitsFlag(true);
  format.setPrimaryLabelsFlag(true);
  format.setPrimaryDelimiterFlag(true);
  ///////////
  format.setOmitLeadDelimiterFlag( true );
  format.setPriAllowZeroMastFlag( true );
  format.setSecAllowZeroMastFlag( true );
  format.setDualFormat( 1 );
  format.setLocalFileUnitsFlag( true );
  format.setMetricSpaceFlag( true );
  format.setSecondaryDelimiterFlag( true );
  format.setSecondarySubUnitsFlag( true );
  ///////////
  format.setSuperScriptLSDFlag( true );
  format.setAngleMeasureFlag( true );
  format.setAngleFormatFlag( true );
  format.setAngleMode( OdDgDimTextFormat::kAngle_D );
  ///////////
  pDim->setDimTextFormat( format );
  
  pDim->setRotation( OdGeQuaternion( .707, 0., 0., -0.707 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.1);
  geom.setWitnessLineExtend(0.1);
  geom.setTextLift(0.12);
  geom.setTextMargin(0.25);
  geom.setMargin(1.0);
  geom.setTerminatorWidth(0.09);
  geom.setTerminatorHeight(0.03);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  
  info.setPrimaryNoMasterUnitsFlag( true );
  info.setSecNoMasterUnitsFlag( true );
  {
    pDim->setStackFractFlag(true);//{ if false, defaults to FromFont }
    info.setStackedFractionType( OdDgDimTextInfo::kFractionHorizontal );// {0=FromFont,1=Horizontal,2=Vertical}
    info.setStackFractAlignment( OdDgDimTextInfo::kFractAlignmentCenter );//{ 0=Top,1=Center,2=Bottom } when vertical only?
  }
  //info.setTextFlags( 0x01 );
  info.setWidth(0.05);
  info.setHeight(0.075);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);// right terminator defaults to arrow

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addSingleLocationDimension(int boxRow, int boxCol)
{ 
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgModel::UnitDescription masterUnits;
  masterUnits.m_base = OdDgModel::kMeter;
  masterUnits.m_system = OdDgModel::kMetric;
  masterUnits.m_numerator = 10;
  masterUnits.m_denominator = 1;
  masterUnits.m_name = L" dm";
  OdDgModel::UnitDescription subUnits;
  subUnits.m_base = OdDgModel::kMeter;
  subUnits.m_system = OdDgModel::kMetric;
  subUnits.m_numerator = 1000;
  subUnits.m_denominator = 1;
  subUnits.m_name = L" mm";

  OdDgDimOptionPrimaryUnitsPtr pPrimaryUnits = OdDgDimOption::createObject( OdDgDimOption::kPrimaryUnits );
  pPrimaryUnits->setMasterUnit(masterUnits);
  pPrimaryUnits->setSubUnit(subUnits);

  masterUnits.m_numerator = 1;
  masterUnits.m_denominator = 1000;
  masterUnits.m_name = L" km";

  subUnits.m_numerator = 100;
  subUnits.m_denominator = 1;
  subUnits.m_name = L" cm";

  OdDgDimOptionSecondaryUnitsPtr pSecondaryUnits = OdDgDimOption::createObject( OdDgDimOption::kSecondaryUnits );
  pSecondaryUnits->setMasterUnit(masterUnits);
  pSecondaryUnits->setSubUnit(subUnits);

  OdDgDimOptionTerminatorSymbologyPtr pTermSymbology = OdDgDimOption::createObject( OdDgDimOption::kTerminatorSymbology );
  pTermSymbology->setColorFlag(true);
  pTermSymbology->setColor(4);

  OdDgDimOptionTolerancePtr pTolerance = OdDgDimOption::createObject( OdDgDimOption::kTolerance );

  pTolerance->setFontEntryId(0);
  pTolerance->setShowSignForZeroFlag(0);
  pTolerance->setStackAlign(0);
  pTolerance->setToleranceTextWidth(0.05);
  pTolerance->setToleranceTextHeight(0.05);
  pTolerance->setToleranceVertSep(0.12);
  pTolerance->setToleranceHorizSep(0);
  pTolerance->setToleranceLower(2.345);
  pTolerance->setToleranceUpper(3.127);

  OdDgDimSingleLocationPtr pDim = OdDgDimSingleLocation::createObject();

  pDim->setOption( pPrimaryUnits );
  pDim->setOption( pTermSymbology );
  pDim->setOption( pSecondaryUnits );
  pDim->setOption( pTolerance );
  pDim->setPrimaryToleranceAccuracy( OdDgDimTextFormat::kFractional32 );
  pDim->setSecondaryToleranceAccuracy( OdDgDimTextFormat::kDecimal7 );
  pDim->setPrimaryToleranceRetainFractionalAccuracy( true );

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1
  ptDim.setPoint( boxCenter + pt.set( dWidth * 0.45, dHeight * 0.15, 0 ).asVector());
  ptDim.setOffsetToDimLine( dWidth * 0.5 );
  ptDim.setJustification(OdDgDimTextInfo::kCenterRightText);
  ptDim.setUseAltSymbologyFlag(true);
  pDim->addPoint( ptDim );

  // Point 2
  ptDim.setPoint( boxCenter + pt.set( dWidth * 0.35, dHeight * 0.1, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.4 );
  ptDim.setUseAltSymbologyFlag(false);
  pDim->addPoint( ptDim );

  // Point 3
  ptDim.setPoint( boxCenter - pt.set( -dWidth * 0.35, dHeight * 0.3, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.4 );
  ptDim.setUseAltSymbologyFlag(true);
  pDim->addPoint( ptDim );

  // Point 4
  ptDim.setPoint( boxCenter - pt.set( -dWidth * 0.49, dHeight * 0.35, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.5 );
  ptDim.setUseAltSymbologyFlag(false);
  pDim->addPoint( ptDim );

  pDim->setTermMode(OdDgDimension::kAutomatic);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  pDim->setArrowHead(OdDgDimension::kClosed);//types: open, filled, closed

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  ///////////
  format.setPrimaryAccuracy( OdDgDimTextFormat::kAccuracyNone);
  format.setSecondaryAccuracy(  OdDgDimTextFormat::kAccuracyNone );
  format.setDecimalCommaFlag( false );
  ///////////
  format.setOmitLeadDelimiterFlag( true );
  format.setPriAllowZeroMastFlag( true );
  format.setSecAllowZeroMastFlag( true );
  format.setDualFormat( 0 );
  format.setLocalFileUnitsFlag( true );
  format.setMetricSpaceFlag( true );
  format.setSecondaryDelimiterFlag( true );
  format.setSecondarySubUnitsFlag( true );
  format.setSecondaryLabelsFlag(true);
  format.setPrimarySubUnitsFlag(true);
  format.setPrimaryLabelsFlag(true);
  format.setPrimaryDelimiterFlag(true);
  ///////////
  format.setSuperScriptLSDFlag( true );
  format.setAngleMeasureFlag( true );
  format.setAngleFormatFlag( true );
  format.setAngleMode( OdDgDimTextFormat::kAngle_D );
  ///////////
  pDim->setDimTextFormat( format );
  pDim->setTrailingZerosFlag(true);
  pDim->setTrailingZeros2Flag(true);
  pDim->setDualFlag(true);
  pDim->setToleranceFlag(true);
  
  pDim->setRotation( OdGeQuaternion( .707, 0., 0., -0.707 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.1);
  geom.setWitnessLineExtend(0.1);
  geom.setTextLift(0.12);
  geom.setTextMargin(0.05);
  geom.setMargin(0.05);
  geom.setTerminatorWidth(0.09);
  geom.setTerminatorHeight(0.03);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );
  pDim->setHorizontalFlag(true);

  // Text Info
  OdDgDimTextInfo info;
  
  info.setPrimaryNoMasterUnitsFlag( false );
  info.setSecNoMasterUnitsFlag( false );
  {
    pDim->setStackFractFlag(true);//{ if false, defaults to FromFont }
    info.setStackedFractionType( OdDgDimTextInfo::kFractionHorizontal );// {0=FromFont,1=Horizontal,2=Vertical}
    info.setStackFractAlignment( OdDgDimTextInfo::kFractAlignmentCenter );//{ 0=Top,1=Center,2=Bottom } when vertical only?
  }
  //info.setTextFlags( 0x01 );
  info.setWidth(0.05);
  info.setHeight(0.075);
  info.setFontEntryId(1025);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);// right terminator defaults to arrow
  pDim->setColorIndex(2);
  pDim->setAltColorIndex(3);

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addSizeStrokeDimension(int boxRow, int boxCol)
{ 
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgDimSizeStrokePtr pDim = OdDgDimSizeStroke::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1
  ptDim.setPoint( boxCenter + pt.set( dWidth * 0.45, dHeight * 0.25, 0 ).asVector());
  ptDim.setOffsetToDimLine( dWidth * 0.5 );
  pDim->addPoint( ptDim );

  // Point 2
  ptDim.setPoint( boxCenter + pt.set( dWidth * 0.35, dHeight * 0.2, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.4 );
  pDim->addPoint( ptDim );

  // Point 3
  ptDim.setPoint( boxCenter - pt.set( -dWidth * 0.35, dHeight * 0.2, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.4 );
  pDim->addPoint( ptDim );

  // Point 4
  ptDim.setPoint( boxCenter - pt.set( -dWidth * 0.49, dHeight * 0.25, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.5 );
  pDim->addPoint( ptDim );

  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  pDim->setArrowHead(OdDgDimension::kClosed);//types: open, filled, closed

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  ///////////
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setSecondaryAccuracy(  OdDgDimTextFormat::kDecimal2 );
  format.setDecimalCommaFlag( false );
  ///////////
  format.setOmitLeadDelimiterFlag( true );
  format.setPriAllowZeroMastFlag( true );
  format.setSecAllowZeroMastFlag( true );
  format.setDualFormat( 1 );
  format.setLocalFileUnitsFlag( true );
  format.setMetricSpaceFlag( true );
  format.setSecondaryDelimiterFlag( true );
  format.setSecondarySubUnitsFlag( true );
  ///////////
  format.setSuperScriptLSDFlag( true );
  format.setAngleMeasureFlag( true );
  format.setAngleFormatFlag( true );
  format.setAngleMode( OdDgDimTextFormat::kAngle_D );
  ///////////
  pDim->setDimTextFormat( format );
  
  pDim->setRotation( OdGeQuaternion( .707, 0., 0., -0.707 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.1);
  geom.setWitnessLineExtend(0.1);
  geom.setTextLift(0.12);
  geom.setTextMargin(0.25);
  geom.setMargin(1.0);
  geom.setTerminatorWidth(0.09);
  geom.setTerminatorHeight(0.03);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  
  info.setPrimaryNoMasterUnitsFlag( true );
  info.setSecNoMasterUnitsFlag( true );
  {
    pDim->setStackFractFlag(true);//{ if false, defaults to FromFont }
    info.setStackedFractionType( OdDgDimTextInfo::kFractionHorizontal );// {0=FromFont,1=Horizontal,2=Vertical}
    info.setStackFractAlignment( OdDgDimTextInfo::kFractAlignmentCenter );//{ 0=Top,1=Center,2=Bottom } when vertical only?
  }
  //info.setTextFlags( 0x01 );
  info.setWidth(0.05);
  info.setHeight(0.075);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtStroke);// right terminator defaults to arrow
  pDim->setRightTerminator(OdDgDimTool::kTtStroke);// right terminator defaults to arrow

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addStackedLocationDimension(int boxRow, int boxCol)
{ 
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgDimStackedLocationPtr pDim = OdDgDimStackedLocation::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1
  ptDim.setPoint( boxCenter + pt.set( dWidth * 0.45, dHeight * 0.25, 0 ).asVector());
  ptDim.setOffsetToDimLine( dWidth * 0.5 );
  ptDim.setPrimaryText( L"Value 1 = *");
  pDim->addPoint( ptDim );

  // Point 2
  ptDim.setPoint( boxCenter + pt.set( dWidth * 0.35, dHeight * 0.2, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.4 );
  ptDim.setPrimaryText( L"Value 2 = *");
  pDim->addPoint( ptDim );

  // Point 3
  ptDim.setPoint( boxCenter - pt.set( -dWidth * 0.35, dHeight * 0.2, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.4 );
  ptDim.setPrimaryText( L"Value 3 = *");
  pDim->addPoint( ptDim );

  // Point 4
  ptDim.setPoint( boxCenter - pt.set( -dWidth * 0.49, dHeight * 0.25, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.5 );
  ptDim.setPrimaryText( L"Value 4 = *");
  pDim->addPoint( ptDim );

  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  pDim->setArrowHead(OdDgDimension::kClosed);//types: open, filled, closed

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  ///////////
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setSecondaryAccuracy(  OdDgDimTextFormat::kDecimal2 );
  format.setDecimalCommaFlag( false );
  ///////////
  format.setOmitLeadDelimiterFlag( true );
  format.setPriAllowZeroMastFlag( true );
  format.setSecAllowZeroMastFlag( true );
  format.setDualFormat( 1 );
  format.setLocalFileUnitsFlag( true );
  format.setMetricSpaceFlag( true );
  format.setSecondaryDelimiterFlag( true );
  format.setSecondarySubUnitsFlag( true );
  ///////////
  format.setSuperScriptLSDFlag( true );
  format.setAngleMeasureFlag( true );
  format.setAngleFormatFlag( true );
  format.setAngleMode( OdDgDimTextFormat::kAngle_D );
  ///////////
  pDim->setDimTextFormat( format );
  
  pDim->setRotation( OdGeQuaternion( .707, 0., 0., -0.707 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.1);
  geom.setWitnessLineExtend(0.1);
  geom.setTextLift(0.12);
  geom.setTextMargin(0.25);
  geom.setMargin(1.0);
  geom.setTerminatorWidth(0.09);
  geom.setTerminatorHeight(0.03);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  
  info.setPrimaryNoMasterUnitsFlag( true );
  info.setSecNoMasterUnitsFlag( true );
  {
    pDim->setStackFractFlag(true);//{ if false, defaults to FromFont }
    info.setStackedFractionType( OdDgDimTextInfo::kFractionHorizontal );// {0=FromFont,1=Horizontal,2=Vertical}
    info.setStackFractAlignment( OdDgDimTextInfo::kFractAlignmentCenter );//{ 0=Top,1=Center,2=Bottom } when vertical only?
  }
  //info.setTextFlags( 0x01 );
  info.setWidth(0.05);
  info.setHeight(0.075);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);// right terminator defaults to arrow

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addCustomLinearDimension(int boxRow, int boxCol)
{ 
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgDimCustomLinearPtr pDim = OdDgDimCustomLinear::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1
  ptDim.setPoint( boxCenter + pt.set( dWidth * 0.45, dHeight * 0.25, 0 ).asVector());
  ptDim.setOffsetToDimLine( dWidth * 0.5 );
  pDim->addPoint( ptDim );

  // Point 2
  ptDim.setPoint( boxCenter + pt.set( dWidth * 0.35, dHeight * 0.2, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.4 );
  pDim->addPoint( ptDim );

  // Point 3
  ptDim.setPoint( boxCenter - pt.set( -dWidth * 0.35, dHeight * 0.2, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.4 );
  pDim->addPoint( ptDim );

  // Point 4
  ptDim.setPoint( boxCenter - pt.set( -dWidth * 0.49, dHeight * 0.25, 0 ).asVector() );
  ptDim.setOffsetToDimLine( dWidth * 0.5 );
  pDim->addPoint( ptDim );

  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  pDim->setArrowHead(OdDgDimension::kClosed);//types: open, filled, closed

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  ///////////
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setSecondaryAccuracy(  OdDgDimTextFormat::kDecimal2 );
  format.setDecimalCommaFlag( false );
  ///////////
  format.setOmitLeadDelimiterFlag( true );
  format.setPriAllowZeroMastFlag( true );
  format.setSecAllowZeroMastFlag( true );
  format.setDualFormat( 1 );
  format.setLocalFileUnitsFlag( true );
  format.setMetricSpaceFlag( true );
  format.setSecondaryDelimiterFlag( true );
  format.setSecondarySubUnitsFlag( true );
  ///////////
  format.setSuperScriptLSDFlag( true );
  format.setAngleMeasureFlag( true );
  format.setAngleFormatFlag( true );
  format.setAngleMode( OdDgDimTextFormat::kAngle_D );
  ///////////
  pDim->setDimTextFormat( format );
  
  pDim->setRotation( OdGeQuaternion( .707, 0., 0., -0.707 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.1);
  geom.setWitnessLineExtend(0.1);
  geom.setTextLift(0.12);
  geom.setTextMargin(0.25);
  geom.setMargin(1.0);
  geom.setTerminatorWidth(0.09);
  geom.setTerminatorHeight(0.03);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  
  info.setPrimaryNoMasterUnitsFlag( true );
  info.setSecNoMasterUnitsFlag( true );
  {
    pDim->setStackFractFlag(true);//{ if false, defaults to FromFont }
    info.setStackedFractionType( OdDgDimTextInfo::kFractionHorizontal );// {0=FromFont,1=Horizontal,2=Vertical}
    info.setStackFractAlignment( OdDgDimTextInfo::kFractAlignmentCenter );//{ 0=Top,1=Center,2=Bottom } when vertical only?
  }
  //info.setTextFlags( 0x01 );
  info.setWidth(0.05);
  info.setHeight(0.075);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(4);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);// right terminator defaults to arrow

  m_pModel3d->addElement( pDim );
}

//
// Creates RADIAL dimensions
//
void OdExDgnFiller::addRadiusDimension(int boxRow, int boxCol)
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x - dWidth/8.0, boxCenter.y - dHeight/8.0 );
  pArc->setOrigin( ptCenter );
  double dRadius = dHeight/6.0;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );


  // Adds immediately Radius dim
  //

  OdDgDimRadiusPtr pDim = OdDgDimRadius::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 3, ptCenter.y + dRadius, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal3 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal2 );// seems to pay no attention to Primary.
  format.setAngleMeasureFlag( true );
  format.setAngleFormatFlag( true );
  //format.setDualFormat(true);
  format.setDecimalCommaFlag(false);
  format.setMetricSpaceFlag(true);
  pDim->setDimTextFormat( format );
  pDim->setTermMode((OdDgDimension::TerminatorOrientation)(boxCol % 4));// TerminatorOrientations: Auto, Reversed, Inside, Outside

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.25);
  geom.setWitnessLineExtend(0.25);
  geom.setTextLift(0.25);
  geom.setTextMargin(0.25);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.5);
  geom.setTerminatorHeight(0.25);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
 
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.5);
  info.setHeight(0.5);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  
  pDim->setDimTextInfo( info );


  // Tool
  pDim->setCenterMarkFlag( true );
  pDim->setLeader( (OdDgDimTool::Leader) (boxCol % 4));//LEADER_RADIUS LEADER_RADIUS_EXT1 LEADER_RADIUS_EXT2 LEADER_DIAMETER
  pDim->setArcSymbolFlag( true );
  pDim->setPrefix((OdDgDimTool::CustomSymbol) ((boxCol % 5) ? (boxCol % 5) : 5) );

  pDim->setLeadingZerosFlag( (boxCol & 1) );
  pDim->setTrailingZerosFlag( (boxCol & 1) );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addRadiusExtendedDimension(int boxRow, int boxCol)
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x - dWidth/4, boxCenter.y - dHeight/8 );
  pArc->setOrigin( ptCenter );
  double dRadius = dHeight/4 * 1.0001;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );


  // Adds immediately RADIUS EXTENDED dim
  //

  OdDgDimRadiusExtendedPtr pDim = OdDgDimRadiusExtended::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 3, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal6 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal6 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.1);
  geom.setWitnessLineExtend(0.1);
  geom.setTextLift(0.2);
  geom.setTerminatorWidth(0.2);
  geom.setTerminatorHeight(0.1);
  geom.setCenterSize(0.1);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.3);
  info.setHeight(0.3);
  info.setFontEntryId(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setCenterMarkFlag( true );
  pDim->setLeader( OdDgDimTool::kDiameter );

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}
//
// Creates CENTER MARK dimension
//
void OdExDgnFiller::addCenterMarkDimension(int boxRow, int boxCol)
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x - dWidth/16.0, boxCenter.y - dHeight/16.0 );
  pArc->setOrigin( ptCenter );
  double dRadius = dHeight/4.0;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( 0 );
  pArc->setSweepAngle( OdaPI * 2 );
  m_pModel3d->addElement( pArc );


  // Adds immediately CENTER MARK dim
  //

  OdDgDimCenterPtr pDim = OdDgDimCenter::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // Geometry

  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setCenterSize(0.3);
  geom.setWitnessLineOffset(0.2);
  geom.setWitnessLineExtend(0.3);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  pDim->setDimTextInfo( info );

  pDim->setCenterMarkLeftExtendFlag(true);
  pDim->setCenterMarkRightExtendFlag(true);
  pDim->setCenterMarkTopExtendFlag(true);
  pDim->setCenterMarkBottomExtendFlag(true);

  m_pModel3d->addElement( pDim );
}


//
// Creates ANGULAR and AXIAL dimensions
//
void OdExDgnFiller::addAngleAxisXDimension(int boxRow, int boxCol)
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);
  //OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x, boxCenter.y );
  //pArc->setOrigin( ptCenter );
  double dRadius = dMinDim/8. * 1.0001;
  /*pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );
  */
  // Adds immediately ANGLE AXIS dim
  //

  OdDgDimAngleAxisXPtr pDim = OdDgDimAngleAxisX::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim/8. );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y  + dRadius, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 0.9, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 0.8, ptCenter.y  - dRadius * 0.6, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 0.7, ptCenter.y  - dRadius * 0.4, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal6 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal6 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.1);
  geom.setWitnessLineExtend(0.1);
  geom.setTextLift(0.2);
  geom.setTerminatorWidth(0.2);
  geom.setTerminatorHeight(0.1);
  geom.setCenterSize(0.1);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.15);
  info.setHeight(0.15);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  // Tool
  pDim->setArcSymbolFlag( true );
  pDim->setFirstTerminator( OdDgDimTool::kTtArrow);

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );
  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addAngleAxisYDimension()
{
  //8,1 MBOX(5.75,-6.00,11.00,-11.00) 
  double dHeight = 5.25;
  double dWidth = 5;
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter( 8.375, -8.5, 0);
  OdGePoint2d ptCenter( boxCenter.x, boxCenter.y );
  double dRadius = dMinDim/8. * 1.0001;

  // Adds immediately ANGLE AXIS Y dim
  //

  OdDgDimAngleAxisYPtr pDim = OdDgDimAngleAxisY::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius/2.0, ptCenter.y + dRadius / 2.0, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim/4. );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x + dRadius/3.0, ptCenter.y  - dRadius/5.0, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal8 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.3);
  geom.setWitnessLineExtend(0.2);
  geom.setTextLift(0.25);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.15);
  geom.setCenterSize(1.1);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.15);
  info.setHeight(0.15);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kOutside);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  // Tool
  pDim->setArcSymbolFlag( true );
  pDim->setFirstTerminator(OdDgDimTool::kTtCircle);

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );
  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addLabelLineDimension()
{
  //10,0 MBOX(0.0,-18.75,5.00,-24.00)

  // Add line
  OdDgLineString2dPtr pLineString = OdDgLineString2d::createObject();

  OdGePoint2d pos(0.8, -23.2);
  pLineString->addVertex(pos);

  pos.set(4.2, -19.55);
  pLineString->addVertex(pos);

  m_pModel3d->addElement( pLineString );

  // Adds immediately LABEL LINE dim
  //

  OdDgDimLabelLinePtr pDim = OdDgDimLabelLine::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The First Point
  ptDim.setPoint( pt.set( 0.8, -23.2, 0 ) );
  ptDim.setOffsetToDimLine( 3.1 );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Second Point
  ptDim.setPoint( pt.set( 4.2, -19.55, 0 ) );
  ptDim.setOffsetToDimLine( 1.2 );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal8 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.3);
  geom.setWitnessLineExtend(0.2);
  geom.setTextLift(0.25);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.15);
  geom.setCenterSize(1.1);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.15);
  info.setHeight(0.15);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );
  pDim->setAltColorIndex( 2 );

  pDim->setLabelLineMode( OdDgDimLabelLine::kLengthAngleAbove );

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addMicrostationStyleNote( int boxRow, int boxColumn )
{
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxColumn);
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxColumn);

  // 1. Add dim note

  OdDgDimNotePtr pDim = OdDgDimNote::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The First Point
  ptDim.setPoint( boxCenter - OdGeVector3d::kXAxis * ( 0.9 * dWidth / 2.0 ) - OdGeVector3d::kYAxis * ( 0.9 * dHeight / 2.0 )  );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Second Point
  ptDim.setPoint( boxCenter - OdGeVector3d::kXAxis * ( 0.5 * dWidth / 2.0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal8 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0);
  geom.setWitnessLineExtend(0);
  geom.setTextLift(0.2);
  geom.setTextMargin(0.5);
  geom.setMargin(0.1);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.15);
  geom.setCenterSize(1.1);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.25);
  info.setHeight(0.25);
  info.setColorIndex(7);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );
  pDim->setAltColorIndex( 2 );
  pDim->setEmbedFlag(true);
  pDim->setHorizontalFlag(true);

  m_pModel3d->addElement( pDim );

  // 2. Create depended text note

  OdDgCellHeader3dPtr pNoteHeader = OdDgCellHeader3d::createObject();

  pNoteHeader->setOrigin( boxCenter - OdGeVector3d::kXAxis * ( 0.5 * dWidth / 2.0 ) );

  m_pModel3d->addElement( pNoteHeader );

  OdDgLine3dPtr pNoteLine = OdDgLine3d::createObject();
  pNoteLine->setStartPoint( boxCenter - OdGeVector3d::kXAxis * ( 0.5 * dWidth / 2.0 ) );
  pNoteLine->setEndPoint( boxCenter - OdGeVector3d::kXAxis * ( 0.1 * dWidth / 2.0 ) );
  pNoteLine->setColorIndex(2);

  OdDgTextNode3dPtr pNoteTextNode = OdDgTextNode3d::createObject();
  pNoteTextNode->setOrigin( boxCenter - OdGeVector3d::kXAxis * ( 0.1 * dWidth / 2.0 ) );

  OdDgText3dPtr pNoteText = OdDgText3d::createObject();
  pNoteText->setText( OD_T("Simple Note") );
  pNoteText->setHeightMultiplier(0.25);
  pNoteText->setLengthMultiplier(0.25);
  pNoteText->setOrigin( boxCenter - OdGeVector3d::kXAxis * ( 0.1 * dWidth / 2.0 ) - OdGeVector3d::kYAxis * 0.125 );

  pNoteTextNode->add( pNoteText );

  pNoteHeader->add( pNoteLine );
  pNoteHeader->add( pNoteTextNode );

  // 3. Set Note dependency

  OdDgAssocPointIData assocData;

  OdDgOriginAssociationPtr pOriginRoot = OdDgOriginAssociation::createObject();

  pOriginRoot->setElementId( (OdUInt64)(pNoteHeader->elementId().getHandle()) );
  pOriginRoot->setRefAttachmentId(0);
  pOriginRoot->setTextOriginOption( OdDgOriginAssociation::kInsertionPoint );

  assocData.m_assocPointData = pOriginRoot;
  assocData.m_iParam1 = 1;
  assocData.m_iParam2 = 0;

  OdDgDepLinkageAssocPointIPtr pDimDepLinkage = OdDgDepLinkageAssocPointI::createObject();
  pDimDepLinkage->setCopyOptions( OdDgDependencyLinkage::kCopyEx );
  pDimDepLinkage->setAppId( 10000 );
  pDimDepLinkage->setAppValue( 1 );
  pDimDepLinkage->add( assocData );

  pDim->addLinkage( pDimDepLinkage->getPrimaryId(), pDimDepLinkage.get(), false );

  OdDgDepLinkageElementIdPtr pDepLinkage = OdDgDepLinkageElementId::createObject();
  pDepLinkage->setCopyOptions( OdDgDependencyLinkage::kCopyEx );
  pDepLinkage->setAppId( 10000 );
  pDepLinkage->setAppValue( 1 );
  pDepLinkage->add( (OdUInt64)(pDim->elementId().getHandle()) );

  pNoteHeader->addLinkage( pDepLinkage->getPrimaryId(), pDepLinkage.get(), false);

  OdDgNoteLinkagePtr pNoteLinkage = OdDgNoteLinkage::createObject();

  pNoteHeader->addLinkage( pNoteLinkage->getPrimaryId(), pNoteLinkage.get(), false);
}

void OdExDgnFiller::addNoteDimension( )
{
  // 11,0  MBOX(0.0,-24.50,5.25,-29.75)

  // Draw box

  OdDgLineString2dPtr pLineString = OdDgLineString2d::createObject();

  OdGePoint2d pos(0.8, -28.95);
  pLineString->addVertex(pos);

  pos.set(2.2, -28.95);
  pLineString->addVertex(pos);

  pos.set(2.2, -27.95);
  pLineString->addVertex(pos);

  pos.set(0.8, -27.95);
  pLineString->addVertex(pos);

  pos.set(0.8, -28.95);
  pLineString->addVertex(pos);

  m_pModel3d->addElement( pLineString );

  // Draw Note dimension

  OdDgDimNotePtr pDim = OdDgDimNote::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The First Point
  ptDim.setPoint( pt.set( 1.5, -27.85, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Second Point
  ptDim.setPoint( pt.set( 2.2, -25.1, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Third Point
  ptDim.setPoint( pt.set( 3.2, -26.3, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( false );
  ptDim.setPrimaryText(L"Note text");
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal8 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal4 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0);
  geom.setWitnessLineExtend(0);
  geom.setTextLift(0.2);
  geom.setTextMargin(0.5);
  geom.setMargin(0.1);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.15);
  geom.setCenterSize(1.1);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.2);
  info.setHeight(0.2);
  info.setColorIndex(7);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );
  pDim->setAltColorIndex( 2 );
  pDim->setEmbedFlag(true);
  pDim->setHorizontalFlag(true);
  pDim->setSplineFit(true);

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addAngleSizeDimension(int boxRow, int boxCol)
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x, boxCenter.y );
  pArc->setOrigin( ptCenter );
  double dRadius = dMinDim/8. * 1.0001;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );


  // Adds immediately ANGLE Size dim
  //

  OdDgDimAngleSizePtr pDim = OdDgDimAngleSize::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim/3. );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y  + dRadius, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 1.2, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 1.1, ptCenter.y  - dRadius * 1.3, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 0.8, ptCenter.y  - dRadius * 1.4, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.2);
  geom.setWitnessLineExtend(0.2);
  geom.setTextLift(0.2);
  geom.setTerminatorWidth(0.2);
  geom.setTerminatorHeight(0.1);
  geom.setCenterSize(0.07);

  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.07);
  info.setHeight(0.07);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}
void OdExDgnFiller::addAngleSizeDimension2(int boxRow, int boxCol)
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x, boxCenter.y );
  pArc->setOrigin( ptCenter );
  double dRadius = dMinDim/8. * 1.0001;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );


  // Adds immediately ANGLE Size dim
  //

  OdDgDimAngleSizePtr pDim = OdDgDimAngleSize::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim/3. );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y  + dRadius, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 1.2, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 2.1, ptCenter.y  - dRadius * 2.3, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 0.8, ptCenter.y  - dRadius * 1.4, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.2);
  geom.setWitnessLineExtend(0.2);
  geom.setTextLift(0.2);
  geom.setTerminatorWidth(0.2);
  geom.setTerminatorHeight(0.1);
  geom.setCenterSize(0.07);

  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.07);
  info.setHeight(0.07);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  // Tool
  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}
void OdExDgnFiller::addAngleLinesDimension( int boxRow, int boxCol )
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x, boxCenter.y );
  pArc->setOrigin( ptCenter );
  double dRadius = dMinDim/8. * 1.0001;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );


  // Adds immediately ANGLE Size dim
  //

  OdDgDimAngleLinesPtr pDim = OdDgDimAngleLines::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim/2. );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y  + dRadius, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 1.2, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 2.1, ptCenter.y  - dRadius * 2.3, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 0.8, ptCenter.y  - dRadius * 1.4, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal6 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal6 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.2);
  geom.setWitnessLineExtend(0.2);
  geom.setTextLift(0.2);
  geom.setTerminatorWidth(0.2);
  geom.setTerminatorHeight(0.1);
  geom.setCenterSize(0.07);

  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.07);
  info.setHeight(0.07);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}
void OdExDgnFiller::addAngleLinesDimension2( int boxRow, int boxCol )
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  //OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x, boxCenter.y );
  //pArc->setOrigin( ptCenter );
  double dRadius = dMinDim/8. * 1.0001;
  /*pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );*/


  // Adds immediately ANGLE Size dim
  //

  OdDgDimAngleLinesPtr pDim = OdDgDimAngleLines::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim/3. );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 0.8, ptCenter.y  - dRadius * 1.4, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal6 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal6 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.2);
  geom.setWitnessLineExtend(0.2);
  geom.setTextLift(0.2);
  geom.setTerminatorWidth(0.2);
  geom.setTerminatorHeight(0.1);
  geom.setCenterSize(0.07);

  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.07);
  info.setHeight(0.07);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}
void OdExDgnFiller::addAngleLocationDimension( int boxRow, int boxCol )
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x, boxCenter.y );
  pArc->setOrigin( ptCenter );
  double dRadius = dMinDim/8. * 1.0001;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );


  // Adds immediately ANGLE Size dim
  //

  OdDgDimAngleLocationPtr pDim = OdDgDimAngleLocation::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim/3. );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y  + dRadius, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 1.2, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 2.1, ptCenter.y  - dRadius * 2.3, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 0.8, ptCenter.y  - dRadius * 1.4, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.2);
  geom.setWitnessLineExtend(0.2);
  geom.setTextLift(0.2);
  geom.setTerminatorWidth(0.2);
  geom.setTerminatorHeight(0.1);
  geom.setCenterSize(0.07);

  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.07);
  info.setHeight(0.07);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kReversed);// TerminatorOrientations: Auto, Reversed, Inside, Outside
  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addArcSizeDimension( int boxRow, int boxCol )
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x, boxCenter.y );
  pArc->setOrigin( ptCenter );
  double dRadius = dMinDim/8. * 1.0001;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );


  // Adds immediately ANGLE Size dim
  //

  OdDgDimArcSizePtr pDim = OdDgDimArcSize::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim/3. );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y  + dRadius, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 1.2, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 2.1, ptCenter.y  - dRadius * 2.3, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 0.8, ptCenter.y  - dRadius * 1.4, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.2);
  geom.setWitnessLineExtend(0.2);
  geom.setTextLift(0.2);
  geom.setTerminatorWidth(0.2);
  geom.setTerminatorHeight(0.1);
  geom.setCenterSize(0.07);

  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.07);
  info.setHeight(0.07);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}

void OdExDgnFiller::addArcLocationDimension( int boxRow, int boxCol )
{
  // Adds 2d Arc
  //
  double dHeight = m_EntityBoxes.getHeight();
  double dWidth = m_EntityBoxes.getWidth(boxRow, boxCol);
  double dMinDim = ((dHeight > dWidth) ? dWidth : dHeight);
  OdGePoint3d boxCenter = m_EntityBoxes.getBoxCenter(boxRow, boxCol);

  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter( boxCenter.x, boxCenter.y );
  pArc->setOrigin( ptCenter );
  double dRadius = dMinDim/8. * 1.0001;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( - OdaPI4 / 1.0 );
  pArc->setSweepAngle( OdaPI * 1.5 );
  m_pModel3d->addElement( pArc );


  // Adds immediately ANGLE Size dim
  //

  OdDgDimArcLocationPtr pDim = OdDgDimArcLocation::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  + dRadius, ptCenter.y, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim/3. );
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x, ptCenter.y  + dRadius, 0 ) );
  ptDim.setOffsetToDimLine( dMinDim ); // doesn't matter
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 1.2, ptCenter.y, 0 ) ); // 20% increase (1.2) (just past radius to left by 20%)
  ptDim.setOffsetToDimLine( dMinDim ); // doesn't matter
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( ptCenter.x  - dRadius * 2.1, ptCenter.y  - dRadius * 2.3, 0 ) ); // double - 10% on x and double - 30% on y (past radius by radius amount and some extra, not 45degrees
  ptDim.setOffsetToDimLine( dMinDim ); // doesn't matter
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( ptCenter.x + dRadius * 0.8, ptCenter.y  - dRadius * 1.4, 0 ) ); // down to the right lower quadrant.
  ptDim.setOffsetToDimLine( dMinDim ); // doesn't matter ????
  ptDim.setNoWitnessLineFlag( false );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal2 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  // Geometry
  OdDgDimGeometry geom;
  geom.setDefaultValues();
  geom.setWitnessLineOffset(0.2);
  geom.setWitnessLineExtend(0.2);
  geom.setTextLift(0.2);
  geom.setTerminatorWidth(0.2);
  geom.setTerminatorHeight(0.1);
  geom.setCenterSize(0.07);

  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setDefaultValues();
  info.setWidth(0.07);
  info.setHeight(0.07);
  pDim->setDimTextInfo( info );

  // terminator (arrow) direction; seems to get ignored by MStation, defaulting to kAutomatic
  pDim->setTermMode(OdDgDimension::kInside);// TerminatorOrientations: Auto, Reversed, Inside, Outside

  pDim->setLeadingZerosFlag( false );
  pDim->setTrailingZerosFlag( true );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}



void OdExDgnFiller::addAlignmentOptions( int boxRow, int boxCol )
{
  {
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxCol );

  /// first box -- tilt box and set dimensions placement alignment value to kPaTrue
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxCol );

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  double rotang = 15./(OdaPI/180.);
  pShape->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );
  pShape->addVertex( center + OdGeVector3d( - major / 3., minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );
  pShape->addVertex( center + OdGeVector3d( major / 3., minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );
  pShape->addVertex( center + OdGeVector3d( major / 3., - minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );
  pShape->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );

  m_pModel3d->addElement( pShape );

  /// first dimension -- tilt box and set dimensions placement alignment value to true
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  pDim->setAlignment(OdDgDimension::kPaTrue);// sets the behavior for the rotation.  You must set the rotation correctly for this.
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 -- set baseline of dimension or associated object points
  {
  OdGePoint3d assocPt(center + OdGeVector3d( major / 3., minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis));
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( minor / 5 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(center + OdGeVector3d( - major / 3., minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis));
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( minor / 7 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );
  OdGeMatrix3d lq(OdGeMatrix3d::kIdentity);
  lq = lq.rotation(rotang,OdGeVector3d::kZAxis,center);
  OdGeQuaternion lm; 
  lm.set(lq);
  pDim->setRotation( lm );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.25);
  geom.setWitnessLineExtend(0.25);
  geom.setTextLift(0.25);
  geom.setTextMargin(0.25);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.5);
  geom.setTerminatorHeight(0.25);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.5);
  info.setHeight(0.5);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);

  m_pModel3d->addElement( pDim );
  }
  boxCol++;
  {
  double minor = m_EntityBoxes.getHeight();
  double major = m_EntityBoxes.getWidth( boxRow, boxCol );

  /// second box -- tilt box and set dimensions placement alignment value to kPaTrue
  OdGePoint3d center = m_EntityBoxes.getBoxCenter( boxRow, boxCol );

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  double rotang = 15./(OdaPI/180.);
  pShape->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );
  pShape->addVertex( center + OdGeVector3d( - major / 3., minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );
  pShape->addVertex( center + OdGeVector3d( major / 3., minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );
  pShape->addVertex( center + OdGeVector3d( major / 3., - minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );
  pShape->addVertex( center + OdGeVector3d( - major / 3., - minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis) );

  m_pModel3d->addElement( pShape );

  /// second dimension -- tilt box and set dimensions placement alignment value to true
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  pDim->setAlignment(OdDgDimension::kPaView);// sets the behavior for the rotation.  You must set the rotation correctly for this.
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 -- set baseline of dimension or associated object points
  {
  OdGePoint3d assocPt(center + OdGeVector3d( major / 3., minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis));
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( minor / 5 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(center + OdGeVector3d( major / 3., - minor / 3., 0. ).rotateBy(rotang,OdGeVector3d::kZAxis));
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( minor / 7 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( 1., 0., 0., 0. ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.25);
  geom.setWitnessLineExtend(0.25);
  geom.setTextLift(0.25);
  geom.setTextMargin(0.25);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.5);
  geom.setTerminatorHeight(0.25);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.5);
  info.setHeight(0.5);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);

  m_pModel3d->addElement( pDim );
  }
}
//
// Creates DIAMETER dimensions
//
void OdExDgnFiller::addDiameterExtended( )
{
  // Adds 2d Circle
  //
  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter;
  ptCenter.set( 7.59347,1.01289 );
  pArc->setOrigin( ptCenter );
  double dRadius = 0.70082;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( 0 );
  pArc->setSweepAngle( OdaPI * 2 );
  m_pModel3d->addElement( pArc );


  // Adds immediately Radius dim
  //

  OdDgDimDiameterExtendedPtr pDim = OdDgDimDiameterExtended::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( 7.59347, 1.01289, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( 8.15395, 1.43361, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( 7.36214, 0.83924, 0 ) );
  ptDim.setOffsetToDimLine( -2.55202 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal5 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal5 );// seems to pay no attention to Primary.
  //format.setDualFormat(true);
  format.setDecimalCommaFlag(false);
  format.setMetricSpaceFlag(true);
  pDim->setDimTextFormat( format );
  pDim->setTermMode(OdDgDimension::kAutomatic);// TerminatorOrientations: Auto, Reversed, Inside, Outside

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.125);
 
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  
  pDim->setDimTextInfo( info );


  // Tool
  pDim->setLeader( OdDgDimTool::kDiameter );
  pDim->setPrefix( OdDgDimTool::kCsDiameter );

  pDim->setLeadingZerosFlag( true );
  pDim->setTrailingZerosFlag( false );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}
void OdExDgnFiller::addDiameterExtended2( )
{
  // Adds 2d Circle
  //
  OdDgArc2dPtr pArc = OdDgArc2d::createObject();
  OdGePoint2d ptCenter;
  ptCenter.set( 29.8361,-1.49855 );
  pArc->setOrigin( ptCenter );
  double dRadius = 0.70082;
  pArc->setPrimaryAxis( dRadius );
  pArc->setSecondaryAxis( dRadius );
  pArc->setStartAngle( 0 );
  pArc->setSweepAngle( OdaPI * 2 );
  m_pModel3d->addElement( pArc );


  // Adds immediately Radius dim
  //

  OdDgDimDiameterExtendedPtr pDim = OdDgDimDiameterExtended::createObject();

  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // The Center Point
  ptDim.setPoint( pt.set( 29.8361, -1.49855, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The Crossing Point of the Arc
  ptDim.setPoint( pt.set( 29.8361, -2.19937, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // The End Point
  ptDim.setPoint( pt.set( 29.8361, -5.18856, 0 ) );
  ptDim.setOffsetToDimLine( 0 );
  ptDim.setNoWitnessLineFlag( true );
  pDim->addPoint( ptDim );

  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal5 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal5 );// seems to pay no attention to Primary.
  //format.setDualFormat(true);
  format.setDecimalCommaFlag(false);
  format.setMetricSpaceFlag(true);
  pDim->setDimTextFormat( format );
  pDim->setTermMode(OdDgDimension::kAutomatic);// TerminatorOrientations: Auto, Reversed, Inside, Outside

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.125);
 
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  
  pDim->setDimTextInfo( info );

  pDim->setLeader( OdDgDimTool::kDiameter );
  pDim->setPrefix( OdDgDimTool::kCsDiameter );

  pDim->setLeadingZerosFlag( true );
  pDim->setTrailingZerosFlag( false );

  pDim->setAltColorIndex( 2 );

  m_pModel3d->addElement( pDim );
}
//
// Create ORDINATE dimensions
//
void OdExDgnFiller::addOrdinates( )
{
  {
  // add rectangles
    // 7,2 MBOX(11.50,-0.50,28.25,-5.50)
  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(12.4586, -1.7495, 0.0000) );//Vertex #1:  (m) (12.4586, -1.7495, 0.0000) 
  pShape->addVertex( OdGePoint3d(14.5772, -1.2955, 0.0000) );//Vertex #2:  (m) (14.5772, -1.2955, 0.0000) 
  pShape->addVertex( OdGePoint3d(14.8915, -2.7622, 0.0000) );//Vertex #3:  (m) (14.8915, -2.7622, 0.0000) 
  pShape->addVertex( OdGePoint3d(12.7729, -3.2162, 0.0000) );//Vertex #4:  (m) (12.7729, -3.2162, 0.0000) 
  pShape->addVertex( OdGePoint3d(12.4586, -1.7495, 0.0000) );//Vertex #5:  (m) (12.4586, -1.7495, 0.0000) 

  m_pModel3d->addElement( pShape );

  pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(19.2106, -1.7061, 0.0000) );//Vertex #1:  (m) (19.2106, -1.7061, 0.0000) 
  pShape->addVertex( OdGePoint3d(21.3291, -1.2521, 0.0000) );//Vertex #2:  (m) (21.3291, -1.2521, 0.0000) 
  pShape->addVertex( OdGePoint3d(21.6435, -2.7188, 0.0000) );//Vertex #3:  (m) (21.6435, -2.7188, 0.0000) 
  pShape->addVertex( OdGePoint3d(19.5249, -3.1728, 0.0000) );//Vertex #4:  (m) (19.5249, -3.1728, 0.0000) 
  pShape->addVertex( OdGePoint3d(19.2106, -1.7061, 0.0000) );//Vertex #5:  (m) (19.2106, -1.7061, 0.0000) 

  m_pModel3d->addElement( pShape );

  pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(23.6976, -1.3946, 0.0000) );//Vertex #1:  (m) (23.6976, -1.3946, 0.0000) 
  pShape->addVertex( OdGePoint3d(25.8162, -0.9406, 0.0000) );//Vertex #2:  (m) (25.8162, -0.9406, 0.0000) 
  pShape->addVertex( OdGePoint3d(26.1305, -2.4073, 0.0000) );//Vertex #3:  (m) (26.1305, -2.4073, 0.0000) 
  pShape->addVertex( OdGePoint3d(24.0119, -2.8613, 0.0000) );//Vertex #4:  (m) (24.0119, -2.8613, 0.0000) 
  pShape->addVertex( OdGePoint3d(23.6976, -1.3946, 0.0000) );//Vertex #5:  (m) (23.6976, -1.3946, 0.0000) 

  m_pModel3d->addElement( pShape );
  // add dimension
  OdDgDimOrdinatePtr pDim = OdDgDimOrdinate::createObject();
  pDim->setAlignment(OdDgDimension::kPaView);
  pDim->setAlignmentView(0);
  pDim->setColorIndex( 2 );   // set dim color
  pDim->setNoAutoTextLiftFlag(true);
  pDim->setStackFractFlag(true);

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 
  {
  OdGePoint3d assocPt(12.7729, -3.2162, 0.0000);//Point #1:  (m) (12.7729, -3.2162, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.915005 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(14.8915, -2.7622, 0.0000);//Point #2:  (m) (14.8915, -2.7622, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 1.369 );
  pDim->addPoint( ptDim );
  }
  // Point 3
  {
  OdGePoint3d assocPt(19.5249, -3.1728, 0.0000);//Point #3:  (m) (19.5249, -3.1728, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.958417 );
  pDim->addPoint( ptDim );
  }
  // Point 4
  {
  OdGePoint3d assocPt(21.6435, -2.7188, 0.0000);//Point #4:  (m) (21.6435, -2.7188, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 1.41241 );
  pDim->addPoint( ptDim );
  }
  // Point 5
  {
  OdGePoint3d assocPt(24.0119, -2.8613, 0.0000);//Point #5:  (m) (24.0119, -2.8613, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 1.26994 );
  pDim->addPoint( ptDim );
  }
  // Point 6
  {
  OdGePoint3d assocPt(26.1305, -2.4073, 0.0000);//Point #6:  (m) (26.1305, -2.4073, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 1.72393 );
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( -0.707107, 0., 0., -0.707107 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.125);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  m_pModel3d->addElement( pDim );
  }
  {
  // add rectangles
    // 8,2 MBOX(11.50,-6.00,28.25,-11.00) // dim same as above, but with DatumValue
  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(11.8912, -6.7119, -0.0000) );//Vertex #1:  (m) (11.8912, -6.7119, -0.0000)  
  pShape->addVertex( OdGePoint3d(14.0098, -6.2579, -0.0000) );//Vertex #2:  (m) (14.0098, -6.2579, -0.0000) 
  pShape->addVertex( OdGePoint3d(14.3241, -7.7246, -0.0000) );//Vertex #3:  (m) (14.3241, -7.7246, -0.0000) 
  pShape->addVertex( OdGePoint3d(12.2056, -8.1786, -0.0000) );//Vertex #4:  (m) (12.2056, -8.1786, -0.0000) 
  pShape->addVertex( OdGePoint3d(11.8912, -6.7119, -0.0000) );//Vertex #5:  (m) (11.8912, -6.7119, -0.0000) 

  m_pModel3d->addElement( pShape );

  pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(18.4553, -6.6829, -0.0000) );//Vertex #1:  (m) (18.4553, -6.6829, -0.0000) 
  pShape->addVertex( OdGePoint3d(20.5738, -6.2289, -0.0000) );//Vertex #2:  (m) (20.5738, -6.2289, -0.0000) 
  pShape->addVertex( OdGePoint3d(20.8881, -7.6956, -0.0000) );//Vertex #3:  (m) (20.8881, -7.6956, -0.0000) 
  pShape->addVertex( OdGePoint3d(18.7696, -8.1496, -0.0000) );//Vertex #4:  (m) (18.7696, -8.1496, -0.0000) 
  pShape->addVertex( OdGePoint3d(18.4553, -6.6829, -0.0000) );//Vertex #5:  (m) (18.4553, -6.6829, -0.0000) 

  m_pModel3d->addElement( pShape );

  pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(23.2025, -6.6030, -0.0000) );//Vertex #1:  (m) (23.2025, -6.6030, -0.0000) 
  pShape->addVertex( OdGePoint3d(25.3211, -6.1490, -0.0000) );//Vertex #2:  (m) (25.3211, -6.1490, -0.0000) 
  pShape->addVertex( OdGePoint3d(25.6354, -7.6157, -0.0000) );//Vertex #3:  (m) (25.6354, -7.6157, -0.0000) 
  pShape->addVertex( OdGePoint3d(23.5168, -8.0697, -0.0000) );//Vertex #4:  (m) (23.5168, -8.0697, -0.0000) 
  pShape->addVertex( OdGePoint3d(23.2025, -6.6030, -0.0000) );//Vertex #5:  (m) (23.2025, -6.6030, -0.0000) 

  m_pModel3d->addElement( pShape );
  // add dimension
  OdDgDimOrdinatePtr pDim = OdDgDimOrdinate::createObject();
  pDim->setAlignment(OdDgDimension::kPaView);
  pDim->setAlignmentView(0);
  pDim->setColorIndex( 2 );   // set dim color
  pDim->setStackFractFlag(true);

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 
  {
  OdGePoint3d assocPt(12.2056, -8.1786, -0.0000);//Point #1:  (m) (12.2056, -8.1786, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 1.62226 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(14.3241, -7.7246, -0.0000);//Point #2:  (m) (14.3241, -7.7246, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.07625 );
  pDim->addPoint( ptDim );
  }
  // Point 3
  {
  OdGePoint3d assocPt(18.7696, -8.1496, -0.0000);//Point #3:  (m) (18.7696, -8.1496, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 1.6512 );
  pDim->addPoint( ptDim );
  }
  // Point 4
  {
  OdGePoint3d assocPt(20.8881, -7.6956, -0.0000);//Point #4:  (m) (20.8881, -7.6956, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.1052 );
  pDim->addPoint( ptDim );
  }
  // Point 5
  {
  OdGePoint3d assocPt(23.5168, -8.0697, -0.0000);//Point #5:  (m) (23.5168, -8.0697, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 1.73119 );
  pDim->addPoint( ptDim );
  }
  // Point 6
  {
  OdGePoint3d assocPt(25.6354, -7.6157, -0.0000);//Point #6:  (m) (25.6354, -7.6157, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.18519 );
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( 0.707107, 0., 0., 0.707107 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  m_pModel3d->addElement( pDim );
  pDim->setDatumValue(5.0);

  }
  {
  // add rectangles
    // 9,2 MBOX(11.50,-11.50,28.25,-18.25)
  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(12.1329, -16.3851, 0.0000) );//Vertex #1:  (m) (12.1329, -16.3851, 0.0000) 
  pShape->addVertex( OdGePoint3d(14.2514, -15.9311, 0.0000) );//Vertex #2:  (m) (14.2514, -15.9311, 0.0000) 
  pShape->addVertex( OdGePoint3d(14.5658, -17.3978, 0.0000) );//Vertex #3:  (m) (14.5658, -17.3978, 0.0000) 
  pShape->addVertex( OdGePoint3d(12.4472, -17.8518, 0.0000) );//Vertex #4:  (m) (12.4472, -17.8518, 0.0000) 
  pShape->addVertex( OdGePoint3d(12.1329, -16.3851, 0.0000) );//Vertex #5:  (m) (12.1329, -16.3851, 0.0000) 

  m_pModel3d->addElement( pShape );

  pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(18.6969, -16.3561, 0.0000) );//Vertex #1:  (m) (18.6969, -16.3561, 0.0000) 
  pShape->addVertex( OdGePoint3d(20.8155, -15.9021, 0.0000) );//Vertex #2:  (m) (20.8155, -15.9021, 0.0000) 
  pShape->addVertex( OdGePoint3d(21.1298, -17.3688, 0.0000) );//Vertex #3:  (m) (21.1298, -17.3688, 0.0000) 
  pShape->addVertex( OdGePoint3d(19.0112, -17.8228, 0.0000) );//Vertex #4:  (m) (19.0112, -17.8228, 0.0000) 
  pShape->addVertex( OdGePoint3d(18.6969, -16.3561, 0.0000) );//Vertex #5:  (m) (18.6969, -16.3561, 0.0000) 

  m_pModel3d->addElement( pShape );

  pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(23.4441, -16.2761, 0.0000) );//Vertex #1:  (m) (23.4441, -16.2761, 0.0000) 
  pShape->addVertex( OdGePoint3d(25.5627, -15.8221, 0.0000) );//Vertex #2:  (m) (25.5627, -15.8221, 0.0000) 
  pShape->addVertex( OdGePoint3d(25.8770, -17.2888, 0.0000) );//Vertex #3:  (m) (25.8770, -17.2888, 0.0000) 
  pShape->addVertex( OdGePoint3d(23.7585, -17.7428, 0.0000) );//Vertex #4:  (m) (23.7585, -17.7428, 0.0000) 
  pShape->addVertex( OdGePoint3d(23.4441, -16.2761, 0.0000) );//Vertex #5:  (m) (23.4441, -16.2761, 0.0000) 

  m_pModel3d->addElement( pShape );
  // add dimension
  OdDgDimOrdinatePtr pDim = OdDgDimOrdinate::createObject();
  pDim->setAlignment(OdDgDimension::kPaView);
  pDim->setAlignmentView(0);
  pDim->setColorIndex( 2 );   // set dim color
  pDim->setStackFractFlag(true);

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 
  {
  OdGePoint3d assocPt(12.1329, -16.3851, 0.0000);//Point #1:  (m) (12.1329, -16.3851, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 3.47709 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(14.2514, -15.9311, 0.0000);//Point #2:  (m) (14.2514, -15.9311, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 3.02309 );
  pDim->addPoint( ptDim );
  }
  // Point 3
  {
  OdGePoint3d assocPt(18.6969, -16.3561, 0.0000);//Point #3:  (m) (18.6969, -16.3561, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 3.44815 );
  pDim->addPoint( ptDim );
  }
  // Point 4
  {
  OdGePoint3d assocPt(20.8155, -15.9021, 0.0000);//Point #4:  (m) (20.8155, -15.9021, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.99415 );
  pDim->addPoint( ptDim );
  }
  // Point 5
  {
  OdGePoint3d assocPt(23.4441, -16.2761, 0.0000);//Point #5:  (m) (23.4441, -16.2761, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 3.36816 );
  pDim->addPoint( ptDim );
  }
  // Point 6
  {
  OdGePoint3d assocPt(25.5627, -15.8221, 0.0000);//Point #6:  (m) (25.5627, -15.8221, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.91416 );
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( 0., 0.707107, 0.707107, 0. ) ); // here, we set this one vertically-inverted

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  m_pModel3d->addElement( pDim );
  }
  {
  // add rectangles
    // 10,2 MBOX(11.50,-18.75,19.75,-29.75)
  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(17.0619, -20.8308, 0.0000) );//Vertex #1:  (m) (17.0619, -20.8308, -0.0000) 
  pShape->addVertex( OdGePoint3d(19.1805, -20.3768, 0.0000) );//Vertex #2:  (m) (19.1805, -20.3768, -0.0000) 
  pShape->addVertex( OdGePoint3d(19.4948, -21.8435, 0.0000) );//Vertex #3:  (m) (19.4948, -21.8435, -0.0000) 
  pShape->addVertex( OdGePoint3d(17.3762, -22.2975, 0.0000) );//Vertex #4:  (m) (17.3762, -22.2975, -0.0000) 
  pShape->addVertex( OdGePoint3d(17.0619, -20.8308, 0.0000) );//Vertex #5:  (m) (17.0619, -20.8308, -0.0000) 

  m_pModel3d->addElement( pShape );

  pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(17.1342, -26.6480, 0.0000) );//Vertex #1:  (m) (17.1342, -26.6480, -0.0000) 
  pShape->addVertex( OdGePoint3d(19.2527, -26.1940, 0.0000) );//Vertex #2:  (m) (19.2527, -26.1940, -0.0000) 
  pShape->addVertex( OdGePoint3d(19.5671, -27.6607, 0.0000) );//Vertex #3:  (m) (19.5671, -27.6607, -0.0000) 
  pShape->addVertex( OdGePoint3d(17.4485, -28.1147, 0.0000) );//Vertex #4:  (m) (17.4485, -28.1147, -0.0000) 
  pShape->addVertex( OdGePoint3d(17.1342, -26.6480, 0.0000) );//Vertex #5:  (m) (17.1342, -26.6480, -0.0000) 

  m_pModel3d->addElement( pShape );

  // add dimension
  OdDgDimOrdinatePtr pDim = OdDgDimOrdinate::createObject();
  pDim->setAlignment(OdDgDimension::kPaTrue);
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 
  {
  OdGePoint3d assocPt(17.0619, -20.8308, 0.0000);//Point #1:  (m) (17.0619, -20.8308, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.0798 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(17.3762, -22.2975, 0.0000);//Point #2:  (m) (17.3762, -22.2975, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.3941 );
  pDim->addPoint( ptDim );
  }
  // Point 3
  {
  OdGePoint3d assocPt(17.1342, -26.6480, 0.0000);//Point #3:  (m) (17.1342, -26.6480, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.15209 );
  pDim->addPoint( ptDim );
  }
  // Point 4
  {
  OdGePoint3d assocPt(17.4485, -28.1147, 0.0000);//Point #4:  (m) (17.4485, -28.1147, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.46639 );
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( 0., 0., 0., 1. ) ); // here, we set this one horizontally-left

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  m_pModel3d->addElement( pDim );
  }
  {
  // add rectangles
    // 10,3 MBOX(20.50,-18.75,31.00,-29.75)
  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(27.1405, -22.0126, 0.0000) );//Vertex #1:  (m) (27.1405, -22.0126, -0.0000) 
  pShape->addVertex( OdGePoint3d(29.2590, -21.5586, 0.0000) );//Vertex #2:  (m) (29.2590, -21.5586, -0.0000) 
  pShape->addVertex( OdGePoint3d(29.5733, -23.0253, 0.0000) );//Vertex #3:  (m) (29.5733, -23.0253, -0.0000) 
  pShape->addVertex( OdGePoint3d(27.4548, -23.4793, 0.0000) );//Vertex #4:  (m) (27.4548, -23.4793, -0.0000) 
  pShape->addVertex( OdGePoint3d(27.1405, -22.0126, 0.0000) );//Vertex #5:  (m) (27.1405, -22.0126, -0.0000) 

  m_pModel3d->addElement( pShape );

  pShape = OdDgShape3d::createObject();
  pShape->addVertex( OdGePoint3d(27.2127, -27.8298, 0.0000) );//Vertex #1:  (m) (27.2127, -27.8298, -0.0000) 
  pShape->addVertex( OdGePoint3d(29.3313, -27.3758, 0.0000) );//Vertex #2:  (m) (29.3313, -27.3758, -0.0000) 
  pShape->addVertex( OdGePoint3d(29.6456, -28.8425, 0.0000) );//Vertex #3:  (m) (29.6456, -28.8425, -0.0000) 
  pShape->addVertex( OdGePoint3d(27.5271, -29.2965, 0.0000) );//Vertex #4:  (m) (27.5271, -29.2965, -0.0000) 
  pShape->addVertex( OdGePoint3d(27.2127, -27.8298, 0.0000) );//Vertex #5:  (m) (27.2127, -27.8298, -0.0000) 

  m_pModel3d->addElement( pShape );

  // add dimension
  OdDgDimOrdinatePtr pDim = OdDgDimOrdinate::createObject();
  pDim->setAlignment(OdDgDimension::kPaTrue);
  pDim->setColorIndex( 2 );   // set dim color
  pDim->setNoAutoTextLiftFlag(true);
  pDim->setStackFractFlag(true);

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 
  {
  OdGePoint3d assocPt(27.1405, -22.0126, -2.1684e-029);//Point #1:  (m) (27.1405, -22.0126, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 2.62132 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(27.4548, -23.4793, -2.1684e-029);//Point #2:  (m) (27.4548, -23.4793, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 3.65838 );
  pDim->addPoint( ptDim );
  }
  // Point 3
  {
  OdGePoint3d assocPt(27.2127, -27.8298, -2.1684e-029);//Point #3:  (m) (27.2127, -27.8298, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 5.73475 );
  pDim->addPoint( ptDim );
  }
  // Point 4
  {
  OdGePoint3d assocPt(27.5271, -29.2965, -2.1684e-029);//Point #4:  (m) (27.5271, -29.2965, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 6.77181 );
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( -0.272646, 0., 0., 0.962115 ) ); // here, we set this one horizontally-left

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  m_pModel3d->addElement( pDim );
  }
}
//
// Create CUSTOM dimensions
//
void OdExDgnFiller::addCustomDim( )
{
}
//
// Create rotated orientation variations of SizeArrow dims
//
#undef MBOX
#define MBOX(ulx,uly,lrx,lry) double minor = -(lry)+(uly);double major = lrx-ulx;OdGePoint3d center((ulx+lrx)/2.,(uly+lry)/2.,0.);
void OdExDgnFiller::addRotVarSizeArrows( )
{
  {// 7,0 MBOX(0.0,-0.50,5.00,-5.50)
  // MKU 23/12/09 - ('warning' issue) //MBOX(0.0,-0.50,5.00,-5.50)

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  // MKU 23/12/09 - ('warning' issue) //double rotang = 15./(OdaPI/180.);
  pShape->addVertex( OdGePoint3d(0.3975, -2.8007, 0.0000) );//Vertex #1:  (m) (0.3975, -2.8007, 0.0000) 
  pShape->addVertex( OdGePoint3d(2.5161, -2.3467, 0.0000) );//Vertex #2:  (m) (2.5161, -2.3467, 0.0000) 
  pShape->addVertex( OdGePoint3d(2.8304, -3.8134, 0.0000) );//Vertex #3:  (m) (2.8304, -3.8134, 0.0000) 
  pShape->addVertex( OdGePoint3d(0.7118, -4.2674, 0.0000) );//Vertex #4:  (m) (0.7118, -4.2674, 0.0000) 
  pShape->addVertex( OdGePoint3d(0.3975, -2.8007, 0.0000) );//Vertex #5:  (m) (0.3975, -2.8007, 0.0000) 

  m_pModel3d->addElement( pShape );

  /// first dimension -- tilt box and set dimensions placement alignment value to true
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  pDim->setAlignment(OdDgDimension::kPaTrue);// sets the behavior for the rotation.  You must set the rotation correctly for this.
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 -- set baseline of dimension or associated object points
  {
  OdGePoint3d assocPt(2.5161, -2.3467, 0.0000);//Point #1:  (m) (2.5161, -2.3467, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 1.37965 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(2.8304, -3.8134, 0.0000);//Point #2:  (m) (2.8304, -3.8134, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.389291 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( -0.777669, 0., 0., -0.628674 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  m_pModel3d->addElement( pDim );
  }
  {// 8,0 MBOX(0.00,-6.00,5.00,-11.00)
  // MKU 23/12/09 - ('warning' issue) //MBOX(0.00,-6.00,5.00,-11.00)

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  // MKU 23/12/09 - ('warning' issue) //double rotang = 15./(OdaPI/180.);
  pShape->addVertex( OdGePoint3d(0.5889, -9.0402, 0.0000) );//Vertex #1:  (m) (0.5889, -9.0402, 0.0000) 
  pShape->addVertex( OdGePoint3d(2.7075, -8.5862, 0.0000) );//Vertex #2:  (m) (2.7075, -8.5862, 0.0000) 
  pShape->addVertex( OdGePoint3d(3.0218, -10.0529, 0.0000) );//Vertex #3:  (m) (3.0218, -10.0529, 0.0000) 
  pShape->addVertex( OdGePoint3d(0.9032, -10.5069, 0.0000) );//Vertex #4:  (m) (0.9032, -10.5069, 0.0000) 
  pShape->addVertex( OdGePoint3d(0.5889, -9.0402, 0.0000) );//Vertex #5:  (m) (0.5889, -9.0402, 0.0000) 

  m_pModel3d->addElement( pShape );

  /// second dimension -- tilt box and set dimensions placement alignment value to true
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  pDim->setAlignment(OdDgDimension::kPaArbitrary);// sets the behavior for the rotation.  You must set the rotation correctly for this, as this is actually IGNORED!! Used only during creation time; during parameter settings.
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 -- set baseline of dimension or associated object points
  {
  OdGePoint3d assocPt(2.7075, -8.5862, 0.0000);//Point #1:  (m) (2.7075, -8.5862, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine(  0.935804 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(3.0218, -10.0529, 0.0000);//Point #2:  (m) (3.0218, -10.0529, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.389291 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( -0.777669, 0., 0., -0.628674 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  m_pModel3d->addElement( pDim );
  }
  {// 9,0 MBOX(0.00,-11.50,5.00,-18.25)
  // MKU 23/12/09 - ('warning' issue) //MBOX(0.00,-11.50,5.00,-18.25)

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  // MKU 23/12/09 - ('warning' issue) //double rotang = 15./(OdaPI/180.);
  pShape->addVertex( OdGePoint3d(0.8330, -15.0032, 0.0000) );//Vertex #1:  (m) (0.8330, -15.0032, 0.0000)  
  pShape->addVertex( OdGePoint3d(2.9516, -14.5492, 0.0000) );//Vertex #2:  (m) (2.9516, -14.5492, 0.0000)  
  pShape->addVertex( OdGePoint3d(3.2659, -16.0159, 0.0000) );//Vertex #3:  (m) (3.2659, -16.0159, 0.0000) 
  pShape->addVertex( OdGePoint3d(1.1473, -16.4699, 0.0000) );//Vertex #4:  (m) (1.1473, -16.4699, 0.0000) 
  pShape->addVertex( OdGePoint3d(0.8330, -15.0032, 0.0000) );//Vertex #5:  (m) (0.8330, -15.0032, 0.0000) 

  m_pModel3d->addElement( pShape );

  /// second dimension -- tilt box and set dimensions placement alignment value to true
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  pDim->setAlignment(OdDgDimension::kPaView);// sets the behavior for the rotation.  You must set the rotation correctly for this, as this is actually IGNORED!! Used only during creation time; during parameter settings.
  pDim->setAlignmentView(0);
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 -- set baseline of dimension or associated object points
  {
  OdGePoint3d assocPt(2.9516, -14.5492, 0.0000);//Point #1:  (m) (2.9516, -14.5492, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.977432 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(3.2659, -16.0159, 0.0000);//Point #2:  (m) (3.2659, -16.0159, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( -2.17702 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( 0.707107, 0., 0., 0.707107 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  m_pModel3d->addElement( pDim );
  }
  {// 7,1 MBOX(5.75,-0.50,11.00,-5.50)
  // MKU 23/12/09 - ('warning' issue) //MBOX(5.75,-0.50,11.00,-5.50)

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  // MKU 23/12/09 - ('warning' issue) //double rotang = 15./(OdaPI/180.);
  pShape->addVertex( OdGePoint3d(7.6434, -2.5315, -0.0000) );//Vertex #1:  (m) (7.6434, -2.5315, -0.0000) 
  pShape->addVertex( OdGePoint3d(9.7620, -2.0775, -0.0000) );//Vertex #2:  (m) (9.7620, -2.0775, -0.0000) 
  pShape->addVertex( OdGePoint3d(10.0763, -3.5442, -0.0000) );//Vertex #3:  (m) (10.0763, -3.5442, -0.0000) 
  pShape->addVertex( OdGePoint3d(7.9577, -3.9982, -0.0000) );//Vertex #4:  (m) (7.9577, -3.9982, -0.0000) 
  pShape->addVertex( OdGePoint3d(7.6434, -2.5315, -0.0000) );//Vertex #5:  (m) (7.6434, -2.5315, -0.0000) 

  m_pModel3d->addElement( pShape );

  /// second dimension -- tilt box and set dimensions placement alignment value to true
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  pDim->setAlignment(OdDgDimension::kPaTrue);// sets the behavior for the rotation.  You must set the rotation correctly for this, as this is actually IGNORED!! Used only during creation time; during parameter settings.
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 -- set baseline of dimension or associated object points
  {
  OdGePoint3d assocPt(7.6434, -2.5315, -0.0000);//Point #1:  (m) (7.6434, -2.5315, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.896286 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(9.7620, -2.0775, -0.0000);//Point #2:  (m) (9.7620, -2.0775, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.432323 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( -0.994435, 0., 0., 0.105355 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  m_pModel3d->addElement( pDim );
  }
  {// 9,1 MBOX(5.75,-11.50,11.00,-18.25)
  // MKU 23/12/09 - ('warning' issue) //MBOX(5.75,-11.50,11.00,-18.25)

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  // MKU 23/12/09 - ('warning' issue) //double rotang = 15./(OdaPI/180.);
  pShape->addVertex( OdGePoint3d(7.5473, -14.7886, 0.0000) );//Vertex #1:  (m) (7.5473, -14.7886, 0.0000) 
  pShape->addVertex( OdGePoint3d(9.6659, -14.3346, 0.0000) );//Vertex #2:  (m) (9.6659, -14.3346, 0.0000) 
  pShape->addVertex( OdGePoint3d(9.9802, -15.8013, 0.0000) );//Vertex #3:  (m) (9.9802, -15.8013, 0.0000) 
  pShape->addVertex( OdGePoint3d(7.8616, -16.2553, 0.0000) );//Vertex #4:  (m) (7.8616, -16.2553, 0.0000) 
  pShape->addVertex( OdGePoint3d(7.5473, -14.7886, 0.0000) );//Vertex #5:  (m) (7.5473, -14.7886, 0.0000) 

  m_pModel3d->addElement( pShape );

  /// second dimension -- tilt box and set dimensions placement alignment value to true
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  pDim->setAlignment(OdDgDimension::kPaView);// sets the behavior for the rotation.  You must set the rotation correctly for this, as this is actually IGNORED!! Used only during creation time; during parameter settings.
  pDim->setAlignmentView(0);
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 -- set baseline of dimension or associated object points
  {
  OdGePoint3d assocPt(7.5473, -14.7886, 0.0000);//Point #1:  (m) (7.5473, -14.7886, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.954941 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(9.6659, -14.3346, 0.0000);//Point #2:  (m) (9.6659, -14.3346, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.408274 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( 0., 1., 0., 0. ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  m_pModel3d->addElement( pDim );
  }
  {// 10,1 MBOX(5.75,-18.75,11.00,-24.00)
  // MKU 23/12/09 - ('warning' issue) //MBOX(5.75,-18.75,11.00,-24.00)

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  // MKU 23/12/09 - ('warning' issue) //double rotang = 15./(OdaPI/180.);
  pShape->addVertex( OdGePoint3d(6.8125, -21.8538, 0.0000) );//Vertex #1:  (m) (6.8125, -21.8538, 0.0000) 
  pShape->addVertex( OdGePoint3d(8.9311, -21.3998, 0.0000) );//Vertex #2:  (m) (8.9311, -21.3998, 0.0000) 
  pShape->addVertex( OdGePoint3d(9.2454, -22.8665, 0.0000) );//Vertex #3:  (m) (9.2454, -22.8665, 0.0000) 
  pShape->addVertex( OdGePoint3d(7.1268, -23.3205, 0.0000) );//Vertex #4:  (m) (7.1268, -23.3205, 0.0000) 
  pShape->addVertex( OdGePoint3d(6.8125, -21.8538, 0.0000) );//Vertex #5:  (m) (6.8125, -21.8538, 0.0000) 

  m_pModel3d->addElement( pShape );

  /// second dimension -- tilt box and set dimensions placement alignment value to true
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  pDim->setAlignment(OdDgDimension::kPaView);// sets the behavior for the rotation.  You must set the rotation correctly for this, as this is actually IGNORED!! Used only during creation time; during parameter settings.
  pDim->setAlignmentView(0);
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 -- set baseline of dimension or associated object points
  {
  OdGePoint3d assocPt(8.9311, -21.3998, 0.0000);//Point #1:  (m) (8.9311, -21.3998, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.977432 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(9.2454, -22.8665, 0.0000);//Point #2:  (m) (9.2454, -22.8665, 0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( -2.17702 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( 0.707107, 0., 0., 0.707107 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  m_pModel3d->addElement( pDim );
  }
  {// 11,1 MBOX(5.75,-24.50,11.00,-29.75)
  // MKU 23/12/09 - ('warning' issue) //MBOX(5.75,-24.50,11.00,-29.75)

  OdDgShape3dPtr pShape = OdDgShape3d::createObject();
  // MKU 23/12/09 - ('warning' issue) //double rotang = 15./(OdaPI/180.);
  pShape->addVertex( OdGePoint3d(6.2814, -27.0203, -0.0000) );//Vertex #1:  (m) (6.2814, -27.0203, -0.0000) 
  pShape->addVertex( OdGePoint3d(8.3999, -26.5663, -0.0000) );//Vertex #2:  (m) (8.3999, -26.5663, -0.0000) 
  pShape->addVertex( OdGePoint3d(8.7143, -28.0330, -0.0000) );//Vertex #3:  (m) (8.7143, -28.0330, -0.0000) 
  pShape->addVertex( OdGePoint3d(6.5957, -28.4870, -0.0000) );//Vertex #4:  (m) (6.5957, -28.4870, -0.0000) 
  pShape->addVertex( OdGePoint3d(6.2814, -27.0203, -0.0000) );//Vertex #5:  (m) (6.2814, -27.0203, -0.0000) 

  m_pModel3d->addElement( pShape );

  /// second dimension -- tilt box and set dimensions placement alignment value to true
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();
  pDim->setAlignment(OdDgDimension::kPaTrue);// sets the behavior for the rotation.  You must set the rotation correctly for this, as this is actually IGNORED!! Used only during creation time; during parameter settings.
  pDim->setColorIndex( 2 );   // set dim color

  OdDgDimPoint ptDim;
  OdGePoint3d pt;

  // Point 1 -- set baseline of dimension or associated object points
  {
  OdGePoint3d assocPt(8.3999, -26.5663, -0.0000);//Point #1:  (m) (8.3999, -26.5663, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 1.37965 );
  pDim->addPoint( ptDim );
  }
  // Point 2
  {
  OdGePoint3d assocPt(8.7143, -28.0330, -0.0000);//Point #2:  (m) (8.7143, -28.0330, -0.0000) A 
  ptDim.setPoint( assocPt );
  ptDim.setOffsetToDimLine( 0.389291 ); // gets ignored!! sets dimline from first pt only.
  pDim->addPoint( ptDim );
  }
  // Text Format
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal1 );
  format.setAngleMeasureFlag( true );
  pDim->setDimTextFormat( format );

  pDim->setRotation( OdGeQuaternion( -0.777669, 0., -0., -0.628674 ) );

  // Geometry
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // Text Info
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);// Bentley tool sets margins,etc., as a ratio of this item.  But, the resulting values stored are final, as drawing units.
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // Tool
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  m_pModel3d->addElement( pDim );
  }
}

//----------------------------------------------------------------------
//   The function illustrates creation of simple linear dimension with 2 
// sections using relative dimension points. Also it shows of "no witness
// line" flag work.
//----------------------------------------------------------------------

void OdExDgnFiller::addLinearDimensionRelativePoints()
{
  // 1. Create size arrow dimension object.
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();

  // 2. Set color for dimension line and default color for other dimension parts.
  pDim->setColorIndex(2);

  // 3. Add 3 dimension points.
  OdDgDimPoint ptDimBaseline;
  OdDgDimPoint ptDimSeg1;
  OdDgDimPoint ptDimSeg2;

  // 3.1 Point 1 - set baseline of dimension. No relative flag.
  ptDimBaseline.setPoint( OdGePoint3d( 0.25, -34.5, 0.0) );
  ptDimBaseline.setOffsetToDimLine( 3.0 );
  pDim->addPoint( ptDimBaseline );

  // 3.2 Point 2 - set control point of first segment. Relative flag is enabled. Use offsets from baseline as coordinates.
  ptDimSeg1.setPoint( OdGePoint3d(2.055,2.0,0.0));
  ptDimSeg1.setJustification( OdDgDimTextInfo::kCenterLeftText); // set text justification for segment 1.
  ptDimSeg1.setOffsetToDimLine( 1.0 ); // It is ignored for center-left dimension text alignment.
  ptDimSeg1.setNoWitnessLineFlag( true ); // Hide witness line for 1 point.
  ptDimSeg1.setRelative(1); // enable relative coordinates.
  pDim->addPoint( ptDimSeg1 );

  // 3.3 Point 3 - set control point of second segment. Relative flag is enabled. Use offsets from baseline as coordinates.
  ptDimSeg2.setPoint( OdGePoint3d(4.75,1.5,0.0));
  ptDimSeg2.setJustification( OdDgDimTextInfo::kCenterLeftText); // set text justification for segment 2.
  ptDimSeg2.setOffsetToDimLine( 1.0 ); // It is ignored for default (center-left) dimension text alignment.
  ptDimSeg2.setNoWitnessLineFlag( false ); // Show witness line for 2 point. ( Default value )
  ptDimSeg2.setRelative(1); // enable relative coordinates.
  pDim->addPoint( ptDimSeg2 );

  // 4. Set dimension text format.
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal3 );
  pDim->setDimTextFormat( format );

  // 5. Set dimension geometry settings.
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // 6. Set dimension text info
  //   Bentley tool sets margins,etc., as a ratio of this item.  
  // But, the resulting values stored are final, as drawing units.
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // 7. Set dimension tool settings. ( Terminators )
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  // 8. Add new dimension element to model.
  m_pModel3d->addElement( pDim );
}

//----------------------------------------------------------------------
//   The function illustrates creation of simple linear dimension 
// associated with line. Also it shows of "Use Alt Symbology" flag work.
//----------------------------------------------------------------------

void OdExDgnFiller::addAssociativeLinearDimension()
{
  // 1. Create line and add it to model.
  OdDgLine3dPtr pAssocLine = OdDgLine3d::createObject();
  pAssocLine->setStartPoint( OdGePoint3d( 7.15, -35.0, 0.0) );
  pAssocLine->setEndPoint( OdGePoint3d( 10.03, -33.0, 0.0) );
  m_pModel3d->addElement( pAssocLine );

  // 2. Create size arrow dimension object.
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();

  // 3. Set color for dimension line and default color for other dimension parts.
  pDim->setColorIndex(2);

  // 4. Set alternate color for dimension line.
  pDim->setAltColorIndex(3);

  // 5. Change dimension alignment to use rotation.
  pDim->setAlignment( OdDgDimension::kPaTrue );

  // 6. Set dimension rotation by line angle.
  OdGeVector3d vrRotation = pAssocLine->getEndPoint() - pAssocLine->getStartPoint();
  vrRotation.normalize();
  OdGeMatrix3d matRotation = OdGeMatrix3d::rotation( vrRotation.angleTo(OdGeVector3d::kXAxis), OdGeVector3d::kZAxis );
  OdGeQuaternion quatRotation;
  quatRotation.set( matRotation );
  pDim->setRotation( quatRotation );

  // 7. Add 2 dimension points.
  OdDgDimPoint ptDimAssocLineStart;
  OdDgDimPoint ptDimAssocLineEnd;

  // 7.1 Point 1 - set baseline of dimension. Same as line start.
  ptDimAssocLineStart.setPoint( OdGePoint3d( 7.15, -35.0, 0.0) );
  ptDimAssocLineStart.setOffsetToDimLine( 1.25 );
  ptDimAssocLineStart.setAssociativeFlag(true); // Mark point to use as associative.
  pDim->addPoint( ptDimAssocLineStart );

  // 7.2 Point 2 - set control point of first segment. Same as line end.
  ptDimAssocLineEnd.setPoint( OdGePoint3d( 10.03, -33.0, 0.0) );
  ptDimAssocLineEnd.setJustification( OdDgDimTextInfo::kCenterLeftText); // set text justification for segment 1.
  ptDimAssocLineEnd.setOffsetToDimLine( 1.0 ); // It is ignored for default (center-left) dimension text alignment.
  ptDimAssocLineEnd.setUseAltSymbologyFlag(true); // Use alternate symbology for end witness line.
  ptDimAssocLineEnd.setAssociativeFlag(true); // Mark point to use as associative.
  pDim->addPoint( ptDimAssocLineEnd );

  // 8. Set dimension text format.
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal3 );
  pDim->setDimTextFormat( format );

  // 9. Set dimension geometry settings.
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // 10. Set dimension text info
  //   Bentley tool sets margins,etc., as a ratio of this item.  
  // But, the resulting values stored are final, as drawing units.
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // 11. Set dimension tool settings. ( Terminators )
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  // 12. Create association between line and dimension.

  // 12.1 Create dependency linkage for association.
  OdDgDepLinkageAssocPointIPtr pAssocLinkage = OdDgDepLinkageAssocPointI::createObject();
  pAssocLinkage->setAppId( 10000 ); // Microstation.
  pAssocLinkage->setAppValue( 1 );

  // 12.2 Create association with start line point.
  OdDgLinearAssociationPtr pStartLinearAssoc = OdDgLinearAssociation::createObject();
  pStartLinearAssoc->setElementId( (OdUInt64)(pAssocLine->elementId().getHandle()) ); // Line Id64
  pStartLinearAssoc->setRefAttachmentId(0); // No reference attachment
  pStartLinearAssoc->setNVertices(2); // Number of line points.
  pStartLinearAssoc->setNumerator(0); // Distance from base point. ( Numerator / Divisor )
  pStartLinearAssoc->setDivisor(1);
  pStartLinearAssoc->setVertexNumber(0); // Base point index.

  OdDgAssocPointIData startPtData;
  startPtData.m_assocPointData = pStartLinearAssoc;
  startPtData.m_iParam1 = 0; // Dimension point number.
  startPtData.m_iParam2 = 0;

  pAssocLinkage->add( startPtData );

  // 12.3 Create association with end line point.
  OdDgLinearAssociationPtr pEndLinearAssoc = OdDgLinearAssociation::createObject();
  pEndLinearAssoc->setElementId( (OdUInt64)(pAssocLine->elementId().getHandle()) );
  pEndLinearAssoc->setRefAttachmentId(0);
  pEndLinearAssoc->setNVertices(2);
  pEndLinearAssoc->setNumerator(1);
  pEndLinearAssoc->setDivisor(1);
  pEndLinearAssoc->setVertexNumber(0);

  OdDgAssocPointIData endPtData;
  endPtData.m_assocPointData = pEndLinearAssoc;
  endPtData.m_iParam1 = 1;
  endPtData.m_iParam2 = 0;

  pAssocLinkage->add( endPtData );

  pDim->addLinkage( pAssocLinkage->getPrimaryId(), pAssocLinkage.get(), false );

  // 13. Add new dimension element to model.
  m_pModel3d->addElement( pDim );
}

//----------------------------------------------------------------------
//   The function illustrates creation of simple linear dimension 
// with leader and rotated text. Also it shows manual text alignment.
//----------------------------------------------------------------------

void OdExDgnFiller::addLinearDimensionWithLeaderAndRotatedText()
{
  // 1. Create size arrow dimension object.
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();

  // 2. Set color for dimension line and default color for other dimension parts.
  pDim->setColorIndex(2);

  // 3. Change dimension alignment to use rotation.
  pDim->setAlignment( OdDgDimension::kPaTrue );

  // 4. Set dimension rotation.
  OdGeMatrix3d matRotation = OdGeMatrix3d::rotation( -OdaPI/6, OdGeVector3d::kZAxis );
  OdGeQuaternion quatRotation;
  quatRotation.set( matRotation );
  pDim->setRotation( quatRotation );

  // 5. Add 2 dimension points.
  OdDgDimPoint ptDimStart;
  OdDgDimPoint ptDimEnd;

  // 5.1 Point 1 - set baseline of dimension.
  ptDimStart.setPoint( OdGePoint3d( 11.7, -33.0, 0.0) );
  ptDimStart.setOffsetToDimLine( 1.34 );
  pDim->addPoint( ptDimStart );

  // 5.2 Point 2 - set control point of first segment.
  ptDimEnd.setPoint( OdGePoint3d( 16.47, -35.0, 0.0) );
  ptDimEnd.setJustification( OdDgDimTextInfo::kManualText ); // Set manual text alignment.
  ptDimEnd.setOffsetToDimLine( 4.1 ); // Offset to text insertion point from start of segment. (Dimension line direction).
  ptDimEnd.setOffsetY(1.6); // Offset to text insertion point from dimension line. (Perpendicular to dimension line direction).
  pDim->addPoint( ptDimEnd );

  // 6. Enable dim leader mode and set dimension leader settings.
  OdDgDimOptionOffsetPtr pDimLeaderOption = OdDgDimOptionOffset::createObject();
  pDimLeaderOption->setAlignment( OdDgDimOptionOffset::kRightAlignment ); // <-- Text or Text -->
  pDimLeaderOption->setChainType( OdDgDimOptionOffset::kLine );
  pDimLeaderOption->setTerminator( OdDgDimTool::kTtStroke );
  pDim->setOption( pDimLeaderOption );

  // 7. Set rotation of horizontal text.
  pDim->setHorizontalFlag(true); // enable horizontal text.

  //   Dimension text is horizontal to view. Dimension view option set rotation of view axis
  // when dimension was created.
  OdDgDimOptionViewPtr pViewOption = OdDgDimOptionView::createObject();
  OdGeMatrix3d matViewRotation = OdGeMatrix3d::rotation( 5 * OdaPI/3.0, OdGeVector3d::kZAxis);
  OdGeQuaternion quatViewRotation;
  quatViewRotation.set( matViewRotation );
  pViewOption->setQuaternion( quatViewRotation ); // quaternion for 3d dimensions and 2d matrix for 2d dimensions.
  pDim->setOption( pViewOption );

  // 8. Set dimension text format.
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal3 );
  pDim->setDimTextFormat( format );

  // 9. Set dimension geometry settings.
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // 10. Set dimension text info
  //   Bentley tool sets margins,etc., as a ratio of this item.  
  // But, the resulting values stored are final, as drawing units.
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // 11. Set dimension tool settings. ( Terminators )
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  // 13. Add new dimension element to model.
  m_pModel3d->addElement( pDim );
}

//----------------------------------------------------------------------
//   The function illustrates creation of simple linear dimension 
// with double units, tolerances and user defined text.
//----------------------------------------------------------------------

void OdExDgnFiller::addLinearDimensionWithUserDefinedText()
{
  // 1. Create size arrow dimension object.
  OdDgDimSizeArrowPtr pDim = OdDgDimSizeArrow::createObject();

  // 2. Set color for dimension line and default color for other dimension parts.
  pDim->setColorIndex(2);

  // 3. Enable secondary units.
  pDim->setDualFlag(true); // Use primary and secondary units.

  // 3.1. Set primary units.
  OdDgModel::UnitDescription masterPrimaryDescription;
  OdDgModel::UnitDescription subPrimaryDescription;
  OdDgModel::fillUnitDescriptor(OdDgModel::kMeters, masterPrimaryDescription);
  OdDgModel::fillUnitDescriptor(OdDgModel::kCentimeters, subPrimaryDescription);

  OdDgDimOptionPrimaryUnitsPtr pPrimaryUnits = OdDgDimOptionPrimaryUnits::createObject();
  pPrimaryUnits->setMasterUnit(masterPrimaryDescription);
  pPrimaryUnits->setSubUnit(subPrimaryDescription);
  pPrimaryUnits->setIndependentUnitsFlag(true); // use primary units as base units of dimension ( not model working units )
  pDim->setOption( pPrimaryUnits );

  // 3.2. Set secondary units.
  OdDgModel::UnitDescription masterSecondaryDescription;
  OdDgModel::UnitDescription subSecondaryDescription;
  OdDgModel::fillUnitDescriptor(OdDgModel::kYards, masterSecondaryDescription);
  OdDgModel::fillUnitDescriptor(OdDgModel::kInches, subSecondaryDescription);

  OdDgDimOptionSecondaryUnitsPtr pSecondaryUnits = OdDgDimOptionSecondaryUnits::createObject();
  pSecondaryUnits->setMasterUnit(masterSecondaryDescription);
  pSecondaryUnits->setSubUnit(subSecondaryDescription);
  pDim->setOption( pSecondaryUnits );

  // 4. Enable tolerances.

  OdDgDimOptionTolerancePtr pTolerance = OdDgDimOptionTolerance::createObject();
  pTolerance->setToleranceTextHeight(0.15);
  pTolerance->setToleranceTextWidth(0.15);
  pTolerance->setToleranceHorizSep( 0.1 ); // From end of units to begin of tolerances
  pTolerance->setToleranceVertSep( 0.05 ); // Between top and bottom tolerances.
  pTolerance->setToleranceUpper(0.01);  // +0.01  primary units
  pTolerance->setToleranceLower(0.005); // -0.005 primary units
  pDim->setOption( pTolerance );
  pDim->setToleranceFlag(true); // to show tolerances.

  // 5. Add 2 dimension points.
  OdDgDimPoint ptDimStart;
  OdDgDimPoint ptDimEnd;

  // 5.1 Point 1 - set baseline of dimension.
  ptDimStart.setPoint( OdGePoint3d( 21.0, -35.0, 0.0) );
  ptDimStart.setOffsetToDimLine( 3.3 );
  pDim->addPoint( ptDimStart );

  // 5.2 Point 2 - set control point of first segment. Set user defined text to all of dimension text sections.
  //               symbol "*" is native dimension text.
  ptDimEnd.setPoint( OdGePoint3d( 30.1467, -35.0, 0.0) );
  ptDimEnd.setJustification( OdDgDimTextInfo::kCenterLeftText );
  ptDimEnd.setOffsetToDimLine( 1.0 );
  // Set user defined texts.
  ptDimEnd.setPrimaryText( OD_T("Primary value = * m"));
  ptDimEnd.setSecondaryText( OD_T("Secondary value = * ft"));
  ptDimEnd.setPrimaryTopToleranceText( OD_T("Primary upper tolerance = * m") );
  ptDimEnd.setPrimaryBottomToleranceText( OD_T("Primary lower tolerance = * m") );
  ptDimEnd.setSecondaryTopToleranceText( OD_T("Secondary upper tolerance = * ft") );
  ptDimEnd.setSecondaryBottomToleranceText( OD_T("Secondary lower tolerance = * ft") );
  pDim->addPoint( ptDimEnd );

  // 6. Set dimension text format.
  OdDgDimTextFormat format;
  pDim->getDimTextFormat( format );
  format.setPrimaryAccuracy( OdDgDimTextFormat::kDecimal5 );
  format.setSecondaryAccuracy( OdDgDimTextFormat::kDecimal5 );
  pDim->setDimTextFormat( format );

  // 7. Set dimension geometry settings.
  OdDgDimGeometry geom;
  geom.setWitnessLineOffset(0.125);
  geom.setWitnessLineExtend(0.125);
  geom.setTextLift(0.125);
  geom.setTextMargin(0.125);
  geom.setMargin(0.5);
  geom.setTerminatorWidth(0.25);
  geom.setTerminatorHeight(0.125);
  geom.setStackOffset(0.0);
  geom.setCenterSize(0.25);
  pDim->setDimGeometry( geom );

  // 8. Set dimension text info
  //   Bentley tool sets margins,etc., as a ratio of this item.  
  // But, the resulting values stored are final, as drawing units.
  OdDgDimTextInfo info;
  info.setWidth(0.25);
  info.setHeight(0.25);
  info.setFontEntryId(0);
  info.setUseColorFlag( true );
  info.setColorIndex(2);
  info.setUseWeightFlag( true );
  info.setWeight(0);
  pDim->setDimTextInfo( info );

  // 9. Set dimension tool settings. ( Terminators )
  pDim->setLeftTerminator(OdDgDimTool::kTtArrow);
  pDim->setRightTerminator(OdDgDimTool::kTtArrow);

  // 10. Add new dimension element to model.
  m_pModel3d->addElement( pDim );
}

// 
// Entity box additions
//
void OdExDgnFiller::MakeEntityBox(OdDgModel* pModel,OdGePoint3d currentPoint,double hCurBox, double wCurBox)
{
      OdDgLineString2dPtr pLineString = OdDgLineString2d::createObject();

      OdGePoint2d pos(currentPoint.x, currentPoint.y);
      pLineString->addVertex(pos);

      pos.x += wCurBox;
      pLineString->addVertex(pos);

      pos.y -= hCurBox;
      pLineString->addVertex(pos);

      pos.x -= wCurBox;
      pLineString->addVertex(pos);

      pos.x = currentPoint.x;
      pos.y = currentPoint.y;
      pLineString->addVertex(pos);

      pLineString->setColorIndex(OdCmEntityColor::kACIBlue);

      pModel->addElement( pLineString );
}
#undef MBOX
#define MBOX(ulx,uly,lrx,lry) {currentPoint.set(ulx,uly,0.0);pFiller->MakeEntityBox(pModel,currentPoint,-(lry)+(uly),lrx-ulx);}
void addExtraLargeBoxes(OdExDgnFiller* pFiller,OdDgModel* pModel)
{
  OdGePoint3d currentPoint;
  MBOX(0.0,-0.50,5.00,-5.50)          // 7,0
  MBOX(0.00,-6.00,5.00,-11.00)      // 8,0
  MBOX(0.00,-11.50,5.00,-18.25)      // 9,0
  MBOX(0.0,-18.75,5.00,-24.00)      // 10,0
  MBOX(0.0,-24.50,5.25,-29.75)      // 11,0
  MBOX(5.75,-0.50,11.00,-5.50)      // 7,1
  MBOX(5.75,-6.00,11.00,-11.00)      // 8,1
  MBOX(5.75,-11.50,11.00,-18.25)      // 9,1
  MBOX(5.75,-18.75,11.00,-24.00)      // 10,1
  MBOX(5.75,-24.50,11.00,-29.75)      // 11,1
  MBOX(11.50,-0.50,28.25,-5.50)      // 7,2
  MBOX(11.50,-6.00,28.25,-11.00)      // 8,2
  MBOX(11.50,-11.50,28.25,-18.25)      // 9,2
  MBOX(11.50,-18.75,19.75,-29.75)      // 10,2
  MBOX(20.50,-18.75,31.00,-29.75)      // 10,3
  MBOX(28.75,-0.50,31.0,-5.50)      // 7,4
  MBOX(28.75,-6.0,31.0,-11.00)      // 8,4
  MBOX(28.75,-11.50,31.0,-18.25)      // 9,4
  MBOX(0.0,-30.25,5.25,-35.5)        // 12,0
  MBOX(5.75,-30.25,11.00,-35.5)      // 12,1
  MBOX(11.50,-30.25,19.75,-35.5)  // 12,2
  MBOX(20.50,-30.25,31.00,-35.5)  // 12,3
}
