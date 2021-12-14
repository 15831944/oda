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

#include "StdAfx.h"
#include "DbAlignedDimension.h"
#include "DbRotatedDimension.h"
#include "DbLine.h"
#include "DbArc.h"
#include "Db2LineAngularDimension.h"
#include "Db3PointAngularDimension.h"
#include "DbArcDimension.h"
#include "DbDiametricDimension.h"
#include "DbOrdinateDimension.h"
#include "DbRadialDimension.h"
#include "DbRadialDimensionLarge.h"
#include "DbDimStyleTable.h"
#include "DbLinetypeTableRecord.h"
#include "DbLinetypeTable.h"
#include "DbMText.h"
#include "DbLeader.h"
#include "DbCircle.h"
/************************************************************************/
/* DBDIMENSIONSEX command demonstates the Dimension entities            */
/* functionality and pupulates database with Dimension entities.        */
/************************************************************************/

void _DbDimensionsEx_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  /**********************************************************************/
  /* Create the pointer to current active space                         */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = DevGuideCommandsUtils::activeBlockTableRecord(pDb).safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Aligned dimension                                                  */
  /**********************************************************************/
  // Create two points for line to be dimensioned
  OdGePoint3d line1Pt, line2Pt;
  line1Pt.x  = 0;
  line1Pt.y  = 7;
  line2Pt    = line1Pt + OdGeVector3d(3.0, 2.0, 0.0); 

  // Create a line and set some properties
  OdDbLinePtr  pLine  = OdDbLine::createObject();
  pLine->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pLine);
  pLine->setStartPoint(line1Pt);
  pLine->setEndPoint(line2Pt);

  // Create an aligned dimension
  OdDbAlignedDimensionPtr pDimensionAligned = OdDbAlignedDimension::createObject();
  pDimensionAligned->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pDimensionAligned);
  
  // Set dimension line point
  OdGePoint3d dimLinePt;
  dimLinePt.x = 1;
  dimLinePt.y = 5;
 
  // Set first definition point
  pDimensionAligned->setXLine1Point(pLine->startPoint());
  // Set second definition point
  pDimensionAligned->setXLine2Point(pLine->endPoint());
  // Set dimension line point
  pDimensionAligned->setDimLinePoint(dimLinePt);
  // Use default text position
  pDimensionAligned->useDefaultTextPosition();
 
  

  /**********************************************************************/
  /* Rotated dimension                                                 */
  /**********************************************************************/
  // Create two points for line to be dimensioned
  line1Pt.x  = 4;
  line1Pt.y  = 4;
  line2Pt    = line1Pt + OdGeVector3d(3.0, 2.0, 0.0); 

  // Create a line and set some properties
  OdDbLinePtr  pLine1  = OdDbLine::createObject();
  pLine1->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pLine1);
  pLine1->setStartPoint(line1Pt);
  pLine1->setEndPoint(line2Pt);

  // Create a rotated dimension
  OdDbRotatedDimensionPtr pDimensionRotated = OdDbRotatedDimension::createObject();
  pDimensionRotated->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pDimensionRotated);
  
  // Dimension line point
  dimLinePt.x = 6;
  dimLinePt.y = 2;

  // Set first definition point
  pDimensionRotated->setXLine1Point(pLine1->startPoint());
  // Set second definition point
  pDimensionRotated->setXLine2Point(pLine1->endPoint());
  // Set dimension line point
  pDimensionRotated->setDimLinePoint(dimLinePt);
  // Use default text position
  pDimensionRotated->useDefaultTextPosition();
  // Set rotation angle
  pDimensionRotated->setRotation(1);


  /**********************************************************************/
  /* 3-Point angular dimension                                          */
  /**********************************************************************/
  // Create an arc to be dimensioned   
  OdDbArcPtr pArc = OdDbArc::createObject();
  pArc->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pArc);
  // Set arc properties
  OdGePoint3d center(6,6,0); 
  pArc->setCenter(center);
  pArc->setStartAngle(OdaToRadian(45.0));
  pArc->setEndAngle(OdaToRadian(135.0));
  pArc->setRadius(1);

  // Create 3-Point angular dimension
  OdDb3PointAngularDimensionPtr pDimension3Point = OdDb3PointAngularDimension::createObject();
  pDimension3Point->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pDimension3Point);
  pDimension3Point->setDatabaseDefaults(pDb);

  // Set dimension center point
  pDimension3Point->setCenterPoint(pArc->center());
  // Set dimension arc point
  pDimension3Point->setArcPoint(pArc->center() + OdGeVector3d(pArc->radius() + 0.0, 1.0, 0.0));
 
  // Set start point
  OdGePoint3d startPoint;
  pArc->getStartPoint(startPoint);
  pDimension3Point->setXLine1Point(startPoint);
  
  // Set end point
  OdGePoint3d endPoint;
  pArc->getEndPoint(endPoint);
  pDimension3Point->setXLine2Point(endPoint);


  /**********************************************************************/
  /* 2-Line angular dimension                                           */
  /**********************************************************************/
  // Create 2 lines for dimensioning the angle between them 
  center.x = 8; 
  center.y = 6;
  OdGeVector3d v1(0.5, 0.1, 0);
  OdGeVector3d v2(0.6, 0.7, 0);
  OdGeVector3d v3 = v2 + OdGeVector3d(0.5, 1.0, 0.0);
  
  pLine1 = OdDbLine::createObject();
  pLine1->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pLine1);
  pLine1->setStartPoint(center + v1);
  pLine1->setEndPoint  (center + v2);

  double rot = OdaToRadian(60.0);
  v1.rotateBy(rot, OdGeVector3d::kZAxis);
  v2.rotateBy(rot, OdGeVector3d::kZAxis);

  OdDbLinePtr pLine2 = OdDbLine::createObject();
  pLine2->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pLine2);
  pLine2->setStartPoint(center + v1);
  pLine2->setEndPoint  (center + v2);

  // Create 2-Line Angular Dimensionn
  OdDb2LineAngularDimensionPtr pDimension2Line = OdDb2LineAngularDimension::createObject();
  bBTR->appendOdDbEntity(pDimension2Line);
  pDimension2Line->setDatabaseDefaults(pDb);

  // Set dimension arc point
  v3.rotateBy(rot/2.0, OdGeVector3d::kZAxis);  
  pDimension2Line->setArcPoint(center + v3);

  // Set start point of the first line
  pLine1->getStartPoint(startPoint);
  pDimension2Line->setXLine1Start(startPoint);
  
  // Set end point of the first line
  pLine1->getEndPoint(endPoint);
  pDimension2Line->setXLine1End(endPoint);
  
  // Set start point of the second line
  pLine2->getStartPoint(startPoint);
  pDimension2Line->setXLine2Start(startPoint);
  
  // Set end point of the second line
  pLine2->getEndPoint(endPoint);
  pDimension2Line->setXLine2End(endPoint);


  /**********************************************************************/
  /* Arc-length dimension                                               */
  /**********************************************************************/
  // Create an arc to be dimensioned
  pArc = OdDbArc::createObject();
  pArc->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pArc);
  // Set Arc properties
  center.x = 0;
  center.y = 0;
  pArc->setCenter(center);
  pArc->setStartAngle(OdaToRadian(0.0));
  pArc->setEndAngle(OdaToRadian(100.0));
  pArc->setRadius(2.0);

  // Create an Arc Dimension
  OdDbArcDimensionPtr pDimensionArc = OdDbArcDimension::createObject();

  // Set default setting to the dimension
  pDimensionArc->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pDimensionArc);
  pDimensionArc->setDatabaseDefaults(pDb);

  // Set center
  pDimensionArc->setCenterPoint(pArc->center());
  // Set arc point
  pDimensionArc->setArcPoint(pArc->center() + OdGeVector3d(pArc->radius() + 2.0, 0.0, 0.0));
  // Set start point
  pArc->getStartPoint(startPoint);
  pDimensionArc->setXLine1Point(startPoint);
  // Set end point
  pArc->getEndPoint(endPoint);
  pDimensionArc->setXLine2Point(endPoint);
  // Set Arc symbol before the dimension text
  pDimensionArc->setArcSymbolType(0);
  // Add arc-leader
  pDimensionArc->setHasLeader(true);


  /**********************************************************************/
  /* Diametric dimension                                                */
  /**********************************************************************/
  // Create a circle to be dimensioned  
  center.x = 7;
  center.y = 0;
  OdDbCirclePtr pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pCircle);
  // Set Circle properties
  pCircle->setCenter(center);
  pCircle->setRadius(1.0);
  
  // Create a Diametric Dimension 
  OdDbDiametricDimensionPtr pDimensionDiametric = OdDbDiametricDimension::createObject();
  pDimensionDiametric->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pDimensionDiametric);
  // Create a vector to determine the diamter of the circle
  OdGeVector3d chordVector(pCircle->radius(), 0.0, 0.0);
  chordVector.rotateBy(OdaToRadian(60.0), OdGeVector3d::kZAxis);
  // Set first and second chord points
  pDimensionDiametric->setChordPoint(pCircle->center() + chordVector);
  pDimensionDiametric->setFarChordPoint(pCircle->center() - chordVector);
  // Set leader length
  pDimensionDiametric->setLeaderLength(0.5);
  pDimensionDiametric->useDefaultTextPosition();

  /**********************************************************************/
  /* Ordinate dimension                                                 */
  /**********************************************************************/
  // Create the line to be dimensioned
  pLine = OdDbLine::createObject();
  pLine->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pLine);
  // Set start and end point
  startPoint.x = 0;
  startPoint.y = -5;
  endPoint.x = 2;
  endPoint.y = -1;
  pLine->setStartPoint(startPoint);
  pLine->setEndPoint  (endPoint);

  // Create the base ordinate dimension
  OdDbOrdinateDimensionPtr pDimensionOrdinateBase = OdDbOrdinateDimension::createObject();
  bBTR->appendOdDbEntity(pDimensionOrdinateBase);
  pDimensionOrdinateBase->setDatabaseDefaults(pDb);

  // Set leader end point parameters
  OdGePoint3d leaderEndPoint;
  leaderEndPoint    = startPoint + OdGeVector3d(1.0, 0, 0.0);
  // Set origin point
  pDimensionOrdinateBase->setOrigin         (startPoint);
  // Set defining point
  pDimensionOrdinateBase->setDefiningPoint  (startPoint);
  // Set leader end point
  pDimensionOrdinateBase->setLeaderEndPoint (leaderEndPoint);
  // Measure distance by Y-Axis
  pDimensionOrdinateBase->useYAxis();

  // Create an ordinate dimension
  OdDbOrdinateDimensionPtr pDimensionOrdinate = OdDbOrdinateDimension::createObject();
  bBTR->appendOdDbEntity(pDimensionOrdinate);
  pDimensionOrdinate->setDatabaseDefaults(pDb);

  // Leader end point
  leaderEndPoint    = endPoint + OdGeVector3d(3.0, 1.0, 0.0);
  // Set origin point
  pDimensionOrdinate->setOrigin         (startPoint);
  // Set defining point
  pDimensionOrdinate->setDefiningPoint  (endPoint);
  // Set leader end point
  pDimensionOrdinate->setLeaderEndPoint (leaderEndPoint);
  // Measure distance by Y-Axis
  pDimensionOrdinate->useYAxis();
  

  /**********************************************************************/
  /* Radial dimension                                                   */
  /**********************************************************************/
  // Create a circle to be dimensioned
  pCircle = OdDbCircle::createObject();
  pCircle->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pCircle);
  // Set circle properties
  pCircle->setCenter(OdGePoint3d(5.0, -3.0, 0));
  pCircle->setRadius(1.0);

  // Create a Radial Dimension
  OdDbRadialDimensionPtr pDimensionRadial = OdDbRadialDimension::createObject();
  pDimensionRadial->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pDimensionRadial);
  // Set dimension center
  pDimensionRadial->setCenter(pCircle->center());
  // Set chord point
  OdGeVector3d chordVector1(pCircle->radius(), 0.0, 0.0);
  chordVector1.rotateBy(OdaToRadian(30.0), OdGeVector3d::kZAxis);
  pDimensionRadial->setChordPoint(pDimensionRadial->center() + chordVector1);
  // Set leader length
  pDimensionRadial->setLeaderLength(0.2);
  // use default text position
  pDimensionRadial->useDefaultTextPosition();
  

  /**********************************************************************/
  /* Radial large dimension                                             */
  /**********************************************************************/
  // Create an arc to be dimensioned
  pArc = OdDbArc::createObject();
  pArc->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pArc);
  
  // Set arc parameters
  center.x = 8;
  center.y = -5;
  pArc->setRadius(2.0);
 
  pArc->setCenter(center);
  pArc->setStartAngle(OdaToRadian(0.0));
  pArc->setEndAngle(OdaToRadian(90.0));

  // Create Radial large dimension
  OdDbRadialDimensionLargePtr pDimensionRadialLarge = OdDbRadialDimensionLarge::createObject();
  bBTR->appendOdDbEntity(pDimensionRadialLarge);
  pDimensionRadialLarge->setDatabaseDefaults(pDb);

  OdGePoint3d centerPoint, chordPoint, overrideCenter, jogPoint, textPosition;
  
  // The centerPoint of the dimension is the center of the arc
  centerPoint = pArc->center();

  // The chordPoint of the dimension is the midpoint of the arc
  chordPoint =  centerPoint +  
    OdGeVector3d(pArc->radius(), 0.0, 0.0).rotateBy(0.5*(pArc->startAngle()+pArc->endAngle()), OdGeVector3d::kZAxis);
  
  // Override center
  overrideCenter = centerPoint + OdGeVector3d(0.0, -1.5, 0.0);

  // The jogPoint is halfway between the overrideCenter and the chordCoint
  jogPoint          = overrideCenter + 0.5 * OdGeVector3d(chordPoint - overrideCenter);

  // The textPosition is along the vector between the centerPoint and the chordPoint.
  textPosition      = centerPoint + 0.6 * OdGeVector3d(chordPoint       - centerPoint);
  // Jog angle
  double jogAngle = OdaToRadian(30.0);
  // Set center
  pDimensionRadialLarge->setCenter(centerPoint);
  // Set chord point
  pDimensionRadialLarge->setChordPoint(chordPoint);
  // Set override center
  pDimensionRadialLarge->setOverrideCenter(overrideCenter);
  // Set jog point
  pDimensionRadialLarge->setJogPoint(jogPoint);
  // Set text position
  pDimensionRadialLarge->setTextPosition(textPosition);
  // Set jog angle
  pDimensionRadialLarge->setJogAngle(jogAngle);
}
