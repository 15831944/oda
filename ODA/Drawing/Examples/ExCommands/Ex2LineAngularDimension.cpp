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
#include "Db2LineAngularDimension.h"

void _Dim2LineAngular_func(OdEdCommandContext* pCmdCtx) {
  OdDbCommandContextPtr pDbCmdCtx(pCmdCtx);
  OdDbDatabasePtr pDb = pDbCmdCtx->database();
  OdDbUserIO* pIO = pDbCmdCtx->dbUserIO();

  OdGePoint3d l1Start = pIO->getPoint("Set start first point");
  OdGePoint3d l1End   = pIO->getPoint("Set end first point");

  OdGePoint3d l2Start = pIO->getPoint("Set start second point");
  OdGePoint3d l2End   = pIO->getPoint("Set end second point");

  OdGePoint3d pos = pIO->getPoint("Set dimension position");

  OdDbObjectId spaceId = ExCommandsUtils::activeBlockTableRecord(pDb);
  OdDbBlockTableRecordPtr pSpace = spaceId.safeOpenObject(OdDb::kForWrite);

  OdDb2LineAngularDimensionPtr pDimension = OdDb2LineAngularDimension::createObject();

  pDimension->setArcPoint(pos);

  pDimension->setXLine1Start(l1Start);
  pDimension->setXLine1End(l1End);

  pDimension->setXLine2Start(l2Start);
  pDimension->setXLine2End(l2End);

  pSpace->appendOdDbEntity(pDimension);
}
