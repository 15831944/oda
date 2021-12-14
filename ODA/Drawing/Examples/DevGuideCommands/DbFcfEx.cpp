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
#include "DbFcf.h"

/************************************************************************/
/* DBFCFEX command demonstates the OdDbFcf entity functionality         */
/* and pupulates database with Feature Control Frame entity.            */
/************************************************************************/


void showStat(OdDbFcf* pFcf, OdDbUserIO* pIO)
{
  OdString message;

  pIO->putString(L"\n/////////////START DISPLAYING FEATURE CONTROL FRAME DATA/////////////");
  

  // Text
  message.format(L"\nText of fcf object is %s", pFcf->text().c_str());
  pIO->putString(message);

  // Text size
  message.format(L"\nFcf text size is %g", pFcf->dimtxt());
  pIO->putString(message);

  // Color
  message.format(L"\nFcf lines color is(RGB) (%x %x %x)", pFcf->dimclrd().red(), pFcf->dimclrd().green(), pFcf->dimclrd().blue());
  pIO->putString(message);
  message.format(L"\nFcf text color is(RGB) (%x %x %x)", pFcf->dimclrt().red(), pFcf->dimclrt().green(), pFcf->dimclrt().blue());
  pIO->putString(message);

  // Normal and directrion
  message.format(L"\nFcf normal is (%g * X + %g * Y + %g * Z)", pFcf->normal().x, pFcf->normal().y, pFcf->normal().z);
  pIO->putString(message);
  message.format(L"\nFcf direction is (%g * X + %g * Y + %g * Z)", pFcf->direction().x, pFcf->direction().y, pFcf->direction().z);
  pIO->putString(message);

  // Scale factor
  message.format(L"\nScale factor is %f",pFcf->dimscale());
  pIO->putString(message);

  // Location
  message.format(L"\nFcf location is (%g %g %g)", pFcf->location().x, pFcf->location().y, pFcf->location().z);
  pIO->putString(message);

  // Bordering points
  OdGePoint3dArray pntArr;
  pFcf->getBoundingPoints(pntArr);
  message.format(L"\nPoint 0: (%g %g %g)", pntArr[0].x,pntArr[0].y, pntArr[0].z);
  pIO->putString(message);
  message.format(L"\nPoint 1: (%g %g %g)", pntArr[1].x,pntArr[1].y, pntArr[1].z);
  pIO->putString(message);
  message.format(L"\nPoint 2: (%g %g %g)", pntArr[2].x,pntArr[2].y, pntArr[2].z);
  pIO->putString(message);
  message.format(L"\nPoint 3: (%g %g %g)", pntArr[3].x,pntArr[3].y, pntArr[3].z);
  pIO->putString(message);
  
  pIO->putString(L"//////////////END DISPLAYING FEATURE CONTROL FRAME DATA//////////////\n");
}


void _DbFcfEx_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  /**********************************************************************/
  /* Create the pointer to current active space                         */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = DevGuideCommandsUtils::activeBlockTableRecord(pDb).safeOpenObject(OdDb::kForWrite);

  /**********************************************************************/
  /* Create the Feature Control Frame                                   */
  /**********************************************************************/
  OdDbFcfPtr pFcf = OdDbFcf::createObject();
  
  // Add a fcf with database defaults to the database 
  pFcf->setDatabaseDefaults(pDb);
  bBTR->appendOdDbEntity(pFcf);
  
  // Set location
  OdGePoint3d pnt(1,5,-2);
  pFcf->setLocation(pnt);

  // Set text
  pFcf->setText(L"{\\Fgdt;j}%%v{\\Fgdt;n}0.45{\\Fgdt;m}%%vA%%vB{\\Fgdt;m}%%vC%%v");

  // Set color
  OdCmColor colFcf;
  colFcf.setRGB(60, 150, 0);
  pFcf->setDimclrd(colFcf); // Color of lines
  colFcf.setRGB(0, 50, 200);
  pFcf->setDimclrt(colFcf); // Color of text

  // Set text size
  pFcf->setDimtxt(1.3);

  // Set scale value
  pFcf->setDimscale(0.3);

  //Se normal anf direction
  OdGeVector3d normal(0.5, -0.6, 0.7);
  OdGeVector3d dir(0.6, 0.7, 0.5);
  pFcf->setOrientation(normal, dir);

  // Show fcf data
  showStat(pFcf, pIO);
}
