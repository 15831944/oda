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
#include "DbSolid.h"

/************************************************************************/
/* ODLEADEREX command demonstates the OdDbLeader entity                 */
/* functionality and pupulates database with Leader entities.           */
/************************************************************************/


void showStat(OdDbSolid* pSolid, OdDbUserIO* pIO)
{
  OdString message;
  OdResult eRes; 
  OdGePlane plane3d;
  double a, b, c, d;
  OdDb::Planarity planarity;
  OdGePoint3d point;
 
  pIO->putString(L"\n/////////////START DISPLAYING SOLID DATA/////////////");
  
  // Points
  for (int i = 0; i<4; i++)
  {
      pSolid->getPointAt(i, point);
      message.format(L"\nSolid point's %d coordinates: %f %f %f", i, point.x, point.y, point.z);
      pIO->putString(message);
  }

  // Thickness
  plane3d.getCoefficients(a, b, c, d);
  message.format(L"\nSolid thickness: %f ", pSolid->thickness());

  // Plane
  eRes = pSolid->getPlane(plane3d, planarity);
  if(eRes == eOk && planarity != OdDb::kNonPlanar) 
  {
    plane3d.getCoefficients(a, b, c, d);
    message.format(L"\nSolid plane is (%g * X + %g * Y + %g * Z + %g)", a, b, c, d);
    pIO->putString(message);
  }

  // Normal
  message.format(L"\nSolid normal is (%g * X + %g * Y + %g * Z)", pSolid->normal().x, pSolid->normal().y, pSolid->normal().z);
  pIO->putString(message);

  pIO->putString(L"\n//////////////END DISPLAYING SOLID DATA//////////////");
}


void _OdSolidEx_func(OdEdCommandContext* pCmdCtx)
{
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  /**********************************************************************/
  /* Create the pointer to current active space                         */
  /**********************************************************************/
  OdDbBlockTableRecordPtr bBTR = DevGuideCommandsUtils::activeBlockTableRecord(pDb).safeOpenObject(OdDb::kForWrite);

  

  /**********************************************************************/
  /* Create a Solid                                                     */
  /**********************************************************************/
  double w = 5;
  double h = 10;
  OdDbSolidPtr pSolid = OdDbSolid::createObject();
  pSolid->setDatabaseDefaults(bBTR->database());
  bBTR->appendOdDbEntity(pSolid);

  // Set Solid plane data
  pSolid->setPointAt(0, OdGePoint3d(5,5,1));
  pSolid->setPointAt(1, OdGePoint3d(5,6,1));
  pSolid->setPointAt(2, OdGePoint3d(6,5,1));
  pSolid->setPointAt(3, OdGePoint3d(6,6,1));
 
  // Set Solid normal
  pSolid->setNormal(OdGeVector3d(0,0,1));

  // Set thickness value
  pSolid->setThickness(2);

  showStat(pSolid, pIO);
}
