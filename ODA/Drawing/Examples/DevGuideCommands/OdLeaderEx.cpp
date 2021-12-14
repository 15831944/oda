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
#include "DbMText.h"
#include "DbLeader.h"
#include "DbCircle.h"
#include "Ge/GeScale3d.h"
#include "DbFcf.h"
#include "DbEllipse.h"
#include "DbBlockReference.h"
#include "DbBlockTable.h"

/************************************************************************/
/* ODLEADEREX command demonstates the OdDbLeader entity                 */
/* functionality and pupulates database with Leader entities.           */
/************************************************************************/


void showStat(OdDbLeader* pLeader, OdDbUserIO* pIO)
{
  OdString message;
OdResult eRes; 
  OdGePlane plane3d;
  double a, b, c, d;
  OdDb::Planarity planarity;
  OdGePoint3d point = OdGePoint3d(0,0,0);
  OdCmColor color;
 
  pIO->putString(L"\n/////////////START DISPLAYING LEADER DATA/////////////");
  
  // Plane
  eRes = pLeader->getPlane(plane3d, planarity);
  if(eRes == eOk) 
  {
    plane3d.getCoefficients(a, b, c, d);
    message.format(L"\nLeader plane is (%g * X + %g * Y + %g * Z + %g)", a, b, c, d);
    pIO->putString(message);
  }

  // Scale factor
  message.format(L"\nScale factor is %f",pLeader->dimscale());
  pIO->putString(message);

  // Arrowhead status
  message.format(L"\nArrowhead is %s", ((pLeader->isArrowHeadEnabled()) ? L"enabled" : L"disabled"));
  pIO->putString(message);

  // Arrowhead type
  message.format(L"\nArrowhead type is %s", OdDmUtil::arrowName(pLeader->dimldrblk()).c_str());
  pIO->putString(message);

  // Line fit type
  message.format(L"\nLeader line is %s", ((pLeader->isSplined()) ? L"curve" : L"polyline"));
  pIO->putString(message);

  // Number of vertices
  message.format(L"\nNumber of vertices is %d", pLeader->numVertices());
  pIO->putString(message);

  // Points
  for (int i=0; i < pLeader->numVertices(); i++)
  {
    point = pLeader->vertexAt(i);
    message.format(L"\n vertex (%d) = (%g,%g,%g)", i, point.x, point.y, point.z);
    pIO->putString(message);
  }

  // Line color
  color = pLeader->dimclrd();
  message.format(L"\nColor (%d %d %d)", color.red(), color.green(), color.blue());
  pIO->putString(message);

  // Annotation type
  switch (pLeader->annoType())
  {
    case OdDbLeader::kMText:
    {
      message.format(L"\nAnnoType is 'MText'");
      break;
    }
    case OdDbLeader::kFcf:
    {
      message.format(L"\nAnnoType is 'Fcf'");
      break;
    }
    case OdDbLeader::kBlockRef:
    {
      message.format(L"\nAnnoType is 'BlockRef'");
      break;
    }
    case OdDbLeader::kNoAnno:
    {
      message.format(L"\nAnnoType is 'NoAnno'");
      break;
    }
  }
  pIO->putString(message);

  // Annotation size
  if (pLeader->annoType()!= OdDbLeader::kNoAnno)
  {
      message.format(L"\nAnnotation height = %f, Annotation width = %f", pLeader->annoHeight(), pLeader->annoWidth());
      pIO->putString(message);
  }

  // Vertical text position
  switch (pLeader->dimtad())
  {
    case 0:
    {
      message.format(L"\nText position type is 'Centered'");
      break;
    }
    case 1:
    {
      message.format(L"\nText position type is 'Above'");
      break;
    }
    case 2:
    {
      message.format(L"\nText position type is 'Side'");
      break;
    }
    case 3:
    {
      message.format(L"\nText position type is 'JIS'");
      break;
    }
  }
  pIO->putString(message);

  // Text gap
  message.format(L"\nText gap is %g", pLeader->dimgap());
  pIO->putString(message);

  pIO->putString(L"\n//////////////END DISPLAYING LEADER DATA//////////////");
}


void _OdLeaderEx_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  /**********************************************************************/
  /* Create the pointer to current active space                         */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = DevGuideCommandsUtils::activeBlockTableRecord(pDb).safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Leader with block reference annotation                             */
  /**********************************************************************/
  /**********************************************************************/
  /* Add a leader with database defaults to the database                */
  /**********************************************************************/
  OdDbLeaderPtr pLeader = OdDbLeader::createObject();
  bBTR->appendOdDbEntity(pLeader);
  pLeader->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  OdGePoint3d point(5,5,0);
  pLeader->appendVertex(point);
  point.x = 8;
  point.y = 8;
  pLeader->appendVertex(point);
  point.x = 10;
  point.y = 8;

  /**********************************************************************/
  /* Define an annotation block -- An ellipse                           */
  /**********************************************************************/
  OdDbEllipsePtr pEllipse = OdDbEllipse::createObject();
  pEllipse->setDatabaseDefaults(pDb);
  pEllipse->set(point, OdGeVector3d::kZAxis, OdGeVector3d(2, 0., 0.), 0.5);
  OdDbBlockTablePtr pBlocks  = pDb->getBlockTableId().safeOpenObject(OdDb::kForWrite);
  OdDbBlockTableRecordPtr pAnnoBlock = OdDbBlockTableRecord::createObject();
  pAnnoBlock->setName(L"AnnoBlock");
  OdDbObjectId annoBlockId = pBlocks->add(pAnnoBlock);
  pAnnoBlock->appendOdDbEntity(pEllipse);

  /**********************************************************************/
  /* Insert the annotation                                              */
  /**********************************************************************/
  OdDbBlockReferencePtr pBlkRef = OdDbBlockReference::createObject();
  OdDbObjectId blkRefId = bBTR->appendOdDbEntity(pBlkRef);
  pBlkRef->setBlockTableRecord(annoBlockId);
  
  // Attach the Block Reference as annotation to the Leader
  pLeader->attachAnnotation(blkRefId);

  // Set arrowhead type
  pLeader->setDimldrblk(L"BOXBLANK");

  // Set scale
  pLeader->setDimscale(2);

  // Set color
  OdCmColor colLeader;
  colLeader.setRGB(0, 0, 255); // Blue
  pLeader->setDimclrd(colLeader);

  // Evaluate leader
  pLeader->evaluateLeader();

  // Show statistic
  showStat(pLeader, pIO);



  /**********************************************************************/
  /* Leader with text annotation                                        */
  /**********************************************************************/
  /**********************************************************************/
  /* Add a leader with database defaults to the database                */
  /**********************************************************************/
  pLeader = OdDbLeader::createObject();
  bBTR->appendOdDbEntity(pLeader);
  pLeader->setDatabaseDefaults(pDb);
  
  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  point.set(5.,0.,0.);
  pLeader->appendVertex(point);
  point.x = 7;
  point.y = 2;
  pLeader->appendVertex(point);
  point.x = 9;
  point.y = 3;
  pLeader->appendVertex(point);
  point.x = 9.5;

  /**********************************************************************/
  /* Create MText                                                       */
  /**********************************************************************/
  OdDbMTextPtr pMText = OdDbMText::createObject();
  pMText->setDatabaseDefaults(pDb);
  OdDbObjectId mTextId = bBTR->appendOdDbEntity(pMText);
  const double textHeight = 0.5;
  const double textWidth = 2.0;
  pMText->setLocation(point);
  pMText->setTextHeight(textHeight);
  pMText->setWidth(textWidth);
  pMText->setAttachment(OdDbMText::kMiddleLeft);
  pMText->setContents(L"Some text");

  /**********************************************************************/
  /* Set a background color                                             */
  /**********************************************************************/
  OdCmColor cBackground;
  cBackground.setRGB(0, 255, 0); // Green
  pMText->setBackgroundFillColor(cBackground);
  pMText->setBackgroundFill(true);
  pMText->setBackgroundScaleFactor(2);

  // Attach the MText as annotation to the Leader
  pLeader->attachAnnotation(mTextId);

  // Set arrowhead type
  pLeader->setDimldrblk(L"ARCHTICK");
  //odPrintConsoleString(L"\nArrow is %s", OdDmUtil::arrowName(pLeader->dimldrblk()).c_str());
  // Set color
  colLeader.setRGB(0, 255, 0); // Green
  pLeader->setDimclrd(colLeader);

  // Set scale
  pLeader->setDimscale(3);

  // Set text justification
  pLeader->setDimtad(1);

  // Set text gap
  pLeader->setDimgap(0.3);

  // Evaluate leader
  pLeader->evaluateLeader();

  // Show statistic
  showStat(pLeader, pIO);



  /**********************************************************************/
  /* Leader with tolerance                                              */
  /**********************************************************************/
  /**********************************************************************/
  /* Add a leader with database defaults to the database                */
  /**********************************************************************/
  pLeader = OdDbLeader::createObject();
  bBTR->appendOdDbEntity(pLeader);
  pLeader->setDatabaseDefaults(pDb);

  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  point.set(15., 5., 0.);
  pLeader->appendVertex(point);
  point.x = 17;
  pLeader->appendVertex(point);
  point.y = 7;
  pLeader->appendVertex(point);
  point.x = 18;
  point.y = 8;
  pLeader->appendVertex(point);

  /**********************************************************************/
  /* Create a Frame Control Feature (Tolerance)                         */
  /**********************************************************************/
  OdDbFcfPtr pTol = OdDbFcf::createObject();
  pTol->setDatabaseDefaults(pDb);
  pTol->setLocation(point);
  pTol->setText(L"{\\Fgdt;k}%%v{\\Fgdt;o}%%v{\\Fgdt;p}%%v%%v%%v%%v");
  colLeader.setRGB(255, 0, 0); // Red
  pTol->setColor(colLeader);

  // Attach the FCF as annotation to the Leader
  pLeader->attachAnnotation(bBTR->appendOdDbEntity(pTol));

  // Set Arrowhead type
  pLeader->setDimldrblk(L"DOT");

  // Set scale
  pLeader->setDimscale(0.5);
  
  // Set color
  colLeader.setRGB(255, 0, 0); // Red
  pLeader->setDimclrd(colLeader);

  // Evaluate leader
  pLeader->evaluateLeader();

  // Show statistic
  showStat(pLeader, pIO);




  /**********************************************************************/
  /* Leader without annotation and with splined leader line             */
  /**********************************************************************/
  /**********************************************************************/
  /* Add a leader with database defaults to the database                */
  /**********************************************************************/
  pLeader = OdDbLeader::createObject();
  bBTR->appendOdDbEntity(pLeader);
  pLeader->setDatabaseDefaults(pDb);
  
  /**********************************************************************/
  /* Add the vertices                                                   */
  /**********************************************************************/
  point.set(15.,0.,0.);
  pLeader->appendVertex(point);
  point.x = 15;
  point.y = 2;
  pLeader->appendVertex(point);
  point.x = 17;
  point.y = 2;
  pLeader->appendVertex(point);
  point.x = 17;
  point.y = 0;
  pLeader->appendVertex(point);

  // Set plane
  OdGePlane plane3d = OdGePlane();
  plane3d.set(0, -0.2, 0.5, 0);
  pLeader->setPlane(plane3d);
  
  // Set 'splined' type
  pLeader->setToSplineLeader();

  // Elevate leader
  pLeader->evaluateLeader();

  // Show statistic
  showStat(pLeader, pIO);
}
